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
    void SetImage(const wxImage& bitmap);

private:
    void OnPaint(wxPaintEvent& event);

    void CreateBitmap(const wxImage& bitmap);
    ImageOnCanvas CalculateImageSize(const wxSize& panelSize) const;
    void DrawChessBoard(wxGraphicsContext* gc, ImageOnCanvas imgData);

    wxImage m_image;
    wxGraphicsBitmap m_bitmap;
    int m_bitmapWidth = 0;
    int m_bitmapHeight = 0;
};