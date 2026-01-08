#include <fmx.h>
#pragma hdrstop

#include <System.IOUtils.hpp>

#include "MainForm.h"

#pragma package(smart_init)
#pragma resource "*.fmx"

TTraceVisualizerForm *TraceVisualizerForm;

namespace {

struct TTraceEntry {
    String Name;
    String CallName;
    String Args;
    String ResultValue;
    int Depth = 0;
};

TTraceEntry ParseTraceLine(const String &line)
{
    TTraceEntry entry;
    int leadingSpaces = 1;
    while (leadingSpaces <= line.Length() && line[leadingSpaces] == L' ') {
        ++leadingSpaces;
    }
    entry.Depth = (leadingSpaces - 1) / 2;

    String trimmed = line.Trim();
    int firstArrow = trimmed.Pos(L"=>");
    int secondArrow = 0;
    if (firstArrow > 0) {
        secondArrow = trimmed.PosEx(L"=>", firstArrow + 2);
    }

    if (firstArrow == 0 || secondArrow == 0) {
        entry.Name = trimmed;
        return entry;
    }

    String prefix = trimmed.SubString(1, firstArrow - 1).Trim();
    String callPart = trimmed.SubString(firstArrow + 2, secondArrow - firstArrow - 2).Trim();
    entry.ResultValue = trimmed.SubString(secondArrow + 2, trimmed.Length()).Trim();
    entry.Name = prefix;

    int openParen = callPart.Pos(L"(");
    int closeParen = callPart.LastDelimiter(L")");
    if (openParen > 0 && closeParen > openParen) {
        entry.CallName = callPart.SubString(1, openParen - 1).Trim();
        entry.Args = callPart.SubString(openParen + 1, closeParen - openParen - 1).Trim();
    } else {
        entry.CallName = callPart;
    }

    return entry;
}

} // namespace

__fastcall TTraceVisualizerForm::TTraceVisualizerForm(TComponent *Owner)
    : TForm(Owner)
{
}

void __fastcall TTraceVisualizerForm::FormCreate(TObject *Sender)
{
    TraceDialog->Filter = L"DTrace files (*.dtrace)|*.dtrace|All files (*.*)|*.*";
    TraceDialog->Title = L"Select a DTrace file";
    FileEdit->Text = L"data/trace.dtrace";
    LoadTrace(FileEdit->Text);
}

void __fastcall TTraceVisualizerForm::BrowseButtonClick(TObject *Sender)
{
    if (TraceDialog->Execute()) {
        FileEdit->Text = TraceDialog->FileName;
    }
}

void __fastcall TTraceVisualizerForm::VisualizeButtonClick(TObject *Sender)
{
    LoadTrace(FileEdit->Text);
}

String TTraceVisualizerForm::ResolvePath(const String &fileName) const
{
    if (TFile::Exists(fileName)) {
        return fileName;
    }

    String basePath = ExtractFilePath(ParamStr(0));
    String combined = TPath::Combine(basePath, fileName);
    if (TFile::Exists(combined)) {
        return combined;
    }

    return fileName;
}

void TTraceVisualizerForm::LoadTrace(const String &fileName)
{
    TraceListView->BeginUpdate();
    try {
        TraceListView->Items->Clear();
        String resolved = ResolvePath(fileName);
        if (!TFile::Exists(resolved)) {
            AddErrorRow(L"Unable to locate trace file.");
            return;
        }

        auto lines = TFile::ReadAllLines(resolved);
        for (const auto &line : lines) {
            if (line.Trim().IsEmpty()) {
                continue;
            }
            AddTraceLine(line);
        }
    }
    __finally {
        TraceListView->EndUpdate();
    }
}

void TTraceVisualizerForm::AddTraceLine(const String &line)
{
    TTraceEntry entry = ParseTraceLine(line);
    TListViewItem *item = TraceListView->Items->Add();
    String indent(entry.Depth * 2, L' ');
    String callDisplay;
    if (!entry.CallName.IsEmpty()) {
        callDisplay = entry.CallName + L"(" + entry.Args + L")";
    }
    item->Text = indent + entry.Name + L" " + callDisplay + L" => " + entry.ResultValue;
}

void TTraceVisualizerForm::AddErrorRow(const String &message)
{
    TListViewItem *item = TraceListView->Items->Add();
    item->Text = message;
}
