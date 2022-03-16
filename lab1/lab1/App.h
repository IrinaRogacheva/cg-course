#pragma once
#include "wx/wx.h"
#include "DrawPane.h"

class MyApp : public wxApp
{
public:
    bool OnInit();
private:
    wxFrame* m_frame;
    DrawPane* m_drawPane;
};