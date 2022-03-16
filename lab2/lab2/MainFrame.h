#pragma once
#include "wx/wx.h"

class MainFrame : public wxFrame
{
public:
    MainFrame();
private:
    void OnOpenFile(wxCommandEvent& WXUNUSED(event));
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
};