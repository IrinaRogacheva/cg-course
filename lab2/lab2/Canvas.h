#pragma once
#include "wx/wx.h"
#include <wx/graphics.h>

struct ImageOnCanvas
{
    int x;
    int y;
    double scale;
};

class Canvas : public wxPanel
{
public:
    Canvas(wxFrame* parent); 
    void SetBitmap(const wxBitmap& bitmap);

private:
    void OnPaint(wxPaintEvent& event);

    ImageOnCanvas CalculateImageSize(const wxSize& panelSize) const;
    void DrawChessBoard(wxGraphicsContext* gc, ImageOnCanvas imgData);

    wxGraphicsBitmap m_bitmap;
    int m_bitmapWidth = 0;
    int m_bitmapHeight = 0;
};