#include "MainFrame.h"
#include "Canvas.h"
#include <wx/filesys.h>

MainFrame::MainFrame()
    : wxFrame(NULL, wxID_ANY, wxString("App"), wxPoint(250, 80), wxSize(800, 500))
{
    wxInitAllImageHandlers();

    SetMinSize(wxSize(250, 200));
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(0, wxString("Open"));
    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    SetMenuBar(menuBar);

    Bind(wxEVT_MENU, &MainFrame::OnOpenFile, this, 0);
}

void MainFrame::OnOpenFile(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog openFileDialog(this, _("Open image file"), "", "", "Image files (*.png;*.jpeg;*.bmp)|*.png;*.jpeg;*.bmp", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }
    
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    
    Canvas* canvas = new Canvas(this);
    wxImage img(openFileDialog.GetPath(), wxBITMAP_TYPE_ANY);
    canvas->SetImage(img);

    sizer->Add(canvas, 1, wxEXPAND);

    SetSizer(sizer);
}