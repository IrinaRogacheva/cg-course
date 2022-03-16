#pragma once
#include "wx/wx.h"

class DrawPane : public wxPanel
{
public:
    DrawPane(wxFrame* parent);

    void paintEvent(wxPaintEvent& evt);

    void render(wxDC& dc);

    DECLARE_EVENT_TABLE()
};