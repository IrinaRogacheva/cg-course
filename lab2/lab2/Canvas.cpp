#include "Canvas.h"
#include <wx/dcbuffer.h>

Canvas::Canvas(wxFrame* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(parent->GetSize().GetWidth() - 15, parent->GetSize().GetHeight() - 60), wxFULL_REPAINT_ON_RESIZE)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);

    Bind(wxEVT_PAINT, &Canvas::OnPaint, this);
}

void Canvas::SetImage(const wxImage& image)
{
    m_image = image;

    CreateBitmap(image);
}

void Canvas::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();

    if (!m_bitmap.IsNull())
    {
        wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
        ImageOnCanvas imgData = CalculateImageSize(GetSize());

        if (m_image.HasAlpha())
        {
            DrawChessBoard(gc, imgData);
        }
        
        gc->DrawBitmap(m_bitmap, imgData.x, imgData.y, imgData.scale * m_bitmapWidth, imgData.scale * m_bitmapHeight);
        delete gc;
    }
}

void Canvas::CreateBitmap(const wxImage& image)
{
    wxGraphicsContext* gc = wxGraphicsContext::Create(this);
    m_bitmap = gc->CreateBitmapFromImage(image);

    m_bitmapWidth = image.GetWidth();
    m_bitmapHeight = image.GetHeight();
    delete gc;
}

ImageOnCanvas Canvas::CalculateImageSize(const wxSize& panelSize) const
{
    const double INDENT = 10;

    double wScale = 1.0;
    double hScale = 1.0;

    int x = panelSize.GetWidth() / 2 - m_bitmapWidth / 2;
    int y = panelSize.GetHeight() / 2 - m_bitmapHeight / 2;

    if (m_bitmapHeight > (panelSize.GetHeight() - INDENT * 2) || m_bitmapWidth > (panelSize.GetWidth() - INDENT * 2))
    {
        if ((m_bitmapHeight > 0) && (m_bitmapWidth > 0))
        {
            hScale = (panelSize.GetHeight() - INDENT * 2) / (double)m_bitmapHeight;
            wScale = (panelSize.GetWidth() - INDENT * 2) / (double)m_bitmapWidth;

            if (hScale < wScale)
            {
                wScale = hScale;
                y = INDENT;
                x = panelSize.GetWidth() / 2 - (m_bitmapWidth * hScale / 2);
            }
            else
            {
                hScale = wScale;
                x = INDENT;
                y = panelSize.GetHeight() / 2 - (m_bitmapHeight * hScale / 2);
            }
        }
    }

    return ImageOnCanvas{ x, y, hScale };
}

void Canvas::DrawChessBoard(wxGraphicsContext* gc, ImageOnCanvas imgData)
{
    const double RECT_SIZE = 20;
    const wxColour BACKGROUND_COLOR = wxColour(255, 255, 255);
    const wxColour CELL_COLOR = wxColour(238, 238, 238);

    gc->SetPen(BACKGROUND_COLOR);
    gc->SetBrush(BACKGROUND_COLOR);
    gc->DrawRectangle(imgData.x, imgData.y, imgData.scale * m_bitmapWidth, imgData.scale * m_bitmapHeight);

    gc->SetPen(CELL_COLOR);
    gc->SetBrush(CELL_COLOR);

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
        }
    }
}
