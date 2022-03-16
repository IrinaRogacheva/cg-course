#include "App.h"

wxIMPLEMENT_APP(CApp);

bool CApp::OnInit()
{    
    m_frame = new MainFrame();
    m_frame->Show(true);

    return true;
}
