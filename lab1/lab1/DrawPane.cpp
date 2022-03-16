#include "DrawPane.h"

BEGIN_EVENT_TABLE(DrawPane, wxPanel)
    EVT_PAINT(DrawPane::paintEvent)
END_EVENT_TABLE()

DrawPane::DrawPane(wxFrame* parent)
    : wxPanel(parent)
{
}

void DrawPane::paintEvent(wxPaintEvent& evt)
{
    wxPaintDC dc(this);
    render(dc);
}

void DrawPane::render(wxDC& dc)
{
    dc.SetBrush(wxBrush(wxColor(252, 253, 244)));
    dc.SetPen(wxPen(wxColor(252, 253, 244), 1));
    dc.DrawEllipse(340, 70, 120, 280);

    dc.SetBrush(wxBrush(wxColor(1, 70, 104)));
    dc.SetPen(wxPen(wxColor(1, 70, 104), 1));

    wxPoint leftFlap[4];
    leftFlap[0] = wxPoint(345, 250);
    leftFlap[1] = wxPoint(320, 270);
    leftFlap[2] = wxPoint(320, 310);
    leftFlap[3] = wxPoint(345, 290);
    dc.DrawPolygon(4, leftFlap);

    wxPoint rightFlap[4];
    rightFlap[0] = wxPoint(480, 270);
    rightFlap[1] = wxPoint(455, 250);
    rightFlap[2] = wxPoint(455, 290);
    rightFlap[3] = wxPoint(480, 310);
    dc.DrawPolygon(4, rightFlap);

    wxPoint nose[3];
    nose[0] = wxPoint(367, 340);
    nose[1] = wxPoint(400, 425);
    nose[2] = wxPoint(431, 340);
    dc.SetBrush(wxBrush(wxColor(252, 238, 32)));
    dc.SetPen(wxPen(wxColor(252, 238, 32), 10));
    dc.DrawPolygon(3, nose);

    dc.SetBrush(wxBrush(wxColor(1, 70, 104)));
    dc.SetPen(wxPen(wxColor(255, 63, 60), 7));
    dc.DrawCircle(wxPoint(400, 170), 24);

    wxPoint fire[3];
    fire[0] = wxPoint(353, 120);
    fire[1] = wxPoint(400, 35);
    fire[2] = wxPoint(447, 120);
    dc.SetBrush(wxBrush(wxColor(255, 74, 70)));
    dc.SetPen(wxPen(wxColor(255, 74, 70), 1));
    dc.DrawPolygon(3, fire);
    
    wxPoint rim[5];
    rim[0] = wxPoint(358, 310);
    rim[1] = wxPoint(442, 310);
    rim[2] = wxPoint(435, 325);
    rim[3] = wxPoint(365, 325);
    rim[4] = wxPoint(358, 310);
    dc.SetBrush(wxBrush(wxColor(1, 70, 104)));
    dc.SetPen(wxPen(wxColor(1, 70, 104), 1));
    dc.DrawPolygon(5, rim);

    dc.SetBrush(wxBrush(wxColor(124, 139, 133)));
    dc.SetPen(wxPen(wxColor(124, 139, 133), 1));
    dc.DrawRectangle(365, 325, 70, 15);
}