#include "App.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    m_frame = new wxFrame(NULL, wxID_ANY, wxT("Hello wxDC"), wxPoint(50, 50), wxSize(800, 600));

    m_drawPane = new DrawPane(m_frame);
    
    sizer->Add(m_drawPane, 1, wxEXPAND);
    m_frame->SetSizer(sizer);
    m_frame->SetAutoLayout(true);

    m_frame->Show();
    return true;
}