#include <fmx.h>
#pragma hdrstop

#include <System.IOUtils.hpp>

#include "MainForm.h"

#pragma package(smart_init)

USEFORM("MainForm.fmx", TraceVisualizerForm);

int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    try
    {
        Application->Initialize();
        Application->CreateForm(__classid(TTraceVisualizerForm), &TraceVisualizerForm);
        Application->Run();
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }
    catch (...)
    {
        try
        {
            throw Exception("");
        }
        catch (Exception &exception)
        {
            Application->ShowException(&exception);
        }
    }
    return 0;
}
