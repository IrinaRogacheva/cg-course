#include "Canvas.h"
#include <wx/dcbuffer.h>

Canvas::Canvas(wxFrame* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(parent->GetSize().GetWidth() - 15, parent->GetSize().GetHeight() - 60), wxFULL_REPAINT_ON_RESIZE)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);

    Bind(wxEVT_PAINT, &Canvas::OnPaint, this);
}

void Canvas::SetBitmap(const wxBitmap& bitmap)
{
    wxGraphicsContext* gc = wxGraphicsContext::Create(this);
    m_bitmap = gc->CreateBitmap(bitmap);

    m_bitmapWidth = bitmap.GetWidth();
    m_bitmapHeight = bitmap.GetHeight();
    delete gc;
}

void Canvas::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();

    if (!m_bitmap.IsNull())
    {
        wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
        ImageOnCanvas imgData = CalculateImageSize(GetSize());

        DrawChessBoard(gc, imgData);
        gc->DrawBitmap(m_bitmap, imgData.x, imgData.y, imgData.scale * m_bitmapWidth, imgData.scale * m_bitmapHeight);
        delete gc;
    }
}

ImageOnCanvas Canvas::CalculateImageSize(const wxSize& panelSize) const
{
    double wScale = 1.0;
    double hScale = 1.0;

    int x = panelSize.GetWidth() / 2 - m_bitmapWidth / 2;
    int y = panelSize.GetHeight() / 2 - m_bitmapHeight / 2;

    if (m_bitmapHeight > (panelSize.GetHeight() - 20) || m_bitmapWidth > (panelSize.GetWidth() - 20))
    {
        if ((m_bitmapHeight > 0) && (m_bitmapWidth > 0))
        {
            hScale = (panelSize.GetHeight() - 20.0) / (double)m_bitmapHeight;
            wScale = (panelSize.GetWidth() - 20.0) / (double)m_bitmapWidth;

            if (hScale < wScale)
            {
                wScale = hScale;
                y = 10;
                x = panelSize.GetWidth() / 2 - (m_bitmapWidth * hScale / 2);
            }
            else
            {
                hScale = wScale;
                x = 10;
                y = panelSize.GetHeight() / 2 - (m_bitmapHeight * hScale / 2);
            }
        }
    }

    return ImageOnCanvas{ x, y, hScale };
}

void Canvas::DrawChessBoard(wxGraphicsContext* gc, ImageOnCanvas imgData)
{
    const double RECT_SIZE = 20;

    gc->SetPen(wxColour(255, 255, 255));
    gc->SetBrush(wxColour(255, 255, 255));
    gc->DrawRectangle(imgData.x, imgData.y, imgData.scale * m_bitmapWidth, imgData.scale * m_bitmapHeight);

    gc->SetPen(wxColour(238, 238, 238));
    gc->SetBrush(wxColour(238, 238, 238));

    double x, y;
    double w = imgData.x + imgData.scale * m_bitmapWidth;
    double h = imgData.y + imgData.scale * m_bitmapHeight;

    for (int i = 0; i < std::ceil(imgData.scale * m_bitmapWidth / RECT_SIZE); ++i)
    {
        for (int j = i % 2; j < std::ceil(imgData.scale * m_bitmapHeight / RECT_SIZE); j += 2)
        {
            x = imgData.x + i * RECT_SIZE;
            y = imgData.y + j * RECT_SIZE;

            gc->DrawRectangle(x, y, ((x + RECT_SIZE) < w ? RECT_SIZE : (w - x)),
                ((y + RECT_SIZE) < h ? RECT_SIZE : (h - y)));
            ;
        }
    }
}
