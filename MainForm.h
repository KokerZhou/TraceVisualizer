#ifndef MainFormH
#define MainFormH

#include <System.Classes.hpp>
#include <System.Types.hpp>
#include <System.UITypes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Dialogs.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListView.hpp>
#include <FMX.Objects.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>

class TTraceVisualizerForm : public TForm
{
__published:
    TRectangle *Shell;
    TRectangle *Header;
    TLayout *HeaderLayout;
    TRectangle *ChipBox;
    TLabel *ChipLabel;
    TLabel *TitleLabel;
    TLayout *FileLayout;
    TLabel *FileLabel;
    TEdit *FileEdit;
    TButton *BrowseButton;
    TButton *VisualizeButton;
    TLayout *MetaLayout;
    TLabel *MetaLabel1;
    TLabel *MetaLabel2;
    TLabel *MetaLabel3;
    TListView *TraceListView;
    TRectangle *Footer;
    TLayout *FooterLayout;
    TLabel *FooterLabel1;
    TLabel *FooterLabel2;
    TLabel *FooterLabel3;
    TOpenDialog *TraceDialog;

    void __fastcall BrowseButtonClick(TObject *Sender);
    void __fastcall VisualizeButtonClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);

private:
    void LoadTrace(const String &fileName);
    String ResolvePath(const String &fileName) const;
    void AddTraceLine(const String &line);
    void AddErrorRow(const String &message);

public:
    __fastcall TTraceVisualizerForm(TComponent *Owner);
};

extern PACKAGE TTraceVisualizerForm *TraceVisualizerForm;

#endif
