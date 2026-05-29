#ifndef WINVER
#define WINVER 0x0A00
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif

#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include "CustomScrollBar.h"

CustomScrollBar::CustomScrollBar()
    : m_nMin(0), m_nMax(100), m_nPos(0), m_nPage(10), m_nThumbThickness(14), m_nMarkerCount(10), m_bVertical(true), m_bShowMarkers(false), m_bHovered(false), m_bDragging(false), m_nDragStartClientPos(0), m_nDragStartScrollPos(0), m_bOwnsFactory(true), m_pD2DFactory(nullptr), m_pRenderTarget(nullptr), m_pBrushBackground(nullptr), m_pBrushTrack(nullptr), m_pBrushThumb(nullptr), m_pBrushThumbHover(nullptr), m_pBrushThumbActive(nullptr), m_pBrushMarker(nullptr)
{
}

CustomScrollBar::~CustomScrollBar() { CustomScrollBar::ReleaseResources(); }

// ── ScrollBarControl-compatible API ───────────────────────────────────────
int CustomScrollBar::GetScrollPos() const { return m_nPos; }

int CustomScrollBar::SetScrollPos(int nPos, BOOL bRedraw)
{
    int nOld = m_nPos;
    m_nPos = Clamp(nPos, m_nMin, m_nMax);
    if (bRedraw && m_hwnd)
        Invalidate();
    return nOld;
}

void CustomScrollBar::GetScrollRange(LPINT lpMin, LPINT lpMax) const
{
    if (lpMin)
        *lpMin = m_nMin;
    if (lpMax)
        *lpMax = m_nMax;
}

void CustomScrollBar::SetScrollRange(int nMin, int nMax, BOOL bRedraw)
{
    m_nMin = nMin;
    m_nMax = nMax >= nMin ? nMax : nMin;
    m_nPos = Clamp(m_nPos, m_nMin, m_nMax);
    if (bRedraw && m_hwnd)
        Invalidate();
}

BOOL CustomScrollBar::GetScrollInfo(LPSCROLLINFO lpSI) const
{
    if (!lpSI)
        return FALSE;
    if (lpSI->fMask & SIF_RANGE)
    {
        lpSI->nMin = m_nMin;
        lpSI->nMax = m_nMax;
    }
    if (lpSI->fMask & SIF_POS)
    {
        lpSI->nPos = m_nPos;
    }
    if (lpSI->fMask & SIF_PAGE)
    {
        lpSI->nPage = static_cast<UINT>(m_nPage);
    }
    return TRUE;
}

int CustomScrollBar::SetScrollInfo(LPSCROLLINFO lpSI, BOOL bRedraw)
{
    if (!lpSI)
        return m_nPos;
    if (lpSI->fMask & SIF_RANGE)
    {
        m_nMin = lpSI->nMin;
        m_nMax = lpSI->nMax;
    }
    if (lpSI->fMask & SIF_PAGE)
    {
        m_nPage = static_cast<int>(lpSI->nPage);
    }
    if (lpSI->fMask & SIF_POS)
    {
        m_nPos = Clamp(lpSI->nPos, m_nMin, m_nMax);
    }
    m_nPos = Clamp(m_nPos, m_nMin, m_nMax);
    if (bRedraw && m_hwnd)
        Invalidate();
    return m_nPos;
}

void CustomScrollBar::SetThumbThickness(int nThickness)
{
    m_nThumbThickness = nThickness > 4 ? nThickness : 4;
    if (m_hwnd)
        Invalidate();
}
int CustomScrollBar::GetThumbThickness() const { return m_nThumbThickness; }

void CustomScrollBar::SetMarkerCount(int nCount)
{
    m_nMarkerCount = nCount >= 2 ? nCount : 2;
    if (m_hwnd)
        Invalidate();
}
int CustomScrollBar::GetMarkerCount() const { return m_nMarkerCount; }

void CustomScrollBar::SetMarkerColor(D2D1_COLOR_F c)
{
    if (m_pBrushMarker)
        m_pBrushMarker->SetColor(c);
}
D2D1_COLOR_F CustomScrollBar::GetMarkerColor()
{
    if (m_pBrushMarker)
        return m_pBrushMarker->GetColor();
    else
        return D2D1::ColorF(D2D1::ColorF::Black);
}

// ── Shared factory (call before Create) ──────────────────────────────────
void CustomScrollBar::SetSharedFactory(ID2D1Factory *pFactory)
{
    if (m_bOwnsFactory)
        SafeRelease(m_pD2DFactory);
    m_pD2DFactory = pFactory;
    m_bOwnsFactory = false;
}

// ── Public resource release ───────────────────────────────────────────────
void CustomScrollBar::ReleaseResources()
{
    SafeRelease(m_pBrushBackground);
    SafeRelease(m_pBrushTrack);
    SafeRelease(m_pBrushThumb);
    SafeRelease(m_pBrushThumbHover);
    SafeRelease(m_pBrushThumbActive);
    SafeRelease(m_pBrushMarker);
    SafeRelease(m_pRenderTarget);
    if (m_bOwnsFactory)
        SafeRelease(m_pD2DFactory);
    else
        m_pD2DFactory = nullptr;
}

void CustomScrollBar::Invalidate() const
{
    if (m_hwnd)
        ::InvalidateRect(m_hwnd, nullptr, FALSE);
}

float CustomScrollBar::ThumbLeadingPixel(int clientMain) const noexcept
{
    int range = m_nMax - m_nMin;
    if (range <= 0)
        return 0.0f;
    float trackLen = static_cast<float>(clientMain - m_nThumbThickness);
    if (trackLen < 0.0f)
        return 0.0f;
    return trackLen * static_cast<float>(m_nPos - m_nMin) / static_cast<float>(range);
}

bool CustomScrollBar::HitTestThumb(POINT pt) const noexcept
{
    RECT rc;
    ::GetClientRect(m_hwnd, &rc);
    float leading;
    int click;
    if (m_bVertical)
    {
        leading = ThumbLeadingPixel(rc.bottom - rc.top);
        click = pt.y;
    }
    else
    {
        leading = ThumbLeadingPixel(rc.right - rc.left);
        click = pt.x;
    }
    return click >= static_cast<int>(leading) && click < static_cast<int>(leading) + m_nThumbThickness;
}

void CustomScrollBar::NotifyParent(int nCode) const
{
    HWND hP = ::GetParent(m_hwnd);
    if (!hP)
        return;
    UINT msg = m_bVertical ? WM_VSCROLL : WM_HSCROLL;
    ::SendMessage(hP, msg,
                  MAKEWPARAM(static_cast<WORD>(nCode), static_cast<WORD>(m_nPos)),
                  reinterpret_cast<LPARAM>(m_hwnd));
}

HRESULT CustomScrollBar::CreateD2DFactory()
{
    if (m_pD2DFactory)
        return S_OK;
    m_bOwnsFactory = true;
    return ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
}

HRESULT CustomScrollBar::CreateRenderTarget()
{
    if (!m_pD2DFactory || m_pRenderTarget)
        return m_pRenderTarget ? S_OK : E_FAIL;
    D2D1_RENDER_TARGET_PROPERTIES props =
        D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
                                     D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE));
    return m_pD2DFactory->CreateDCRenderTarget(&props, &m_pRenderTarget);
}

HRESULT CustomScrollBar::CreateBrushes()
{
    if (!m_pRenderTarget)
        return E_FAIL;
    using namespace ScrollBarColors;
    struct
    {
        const D2D1_COLOR_F *color;
        ID2D1SolidColorBrush **brush;
    } table[] = {
        {&Background, &m_pBrushBackground},
        {&Track, &m_pBrushTrack},
        {&Thumb, &m_pBrushThumb},
        {&ThumbHover, &m_pBrushThumbHover},
        {&ThumbActive, &m_pBrushThumbActive},
        {&Marker, &m_pBrushMarker},
    };
    HRESULT hr = S_OK;
    for (auto &e : table)
        if (SUCCEEDED(hr))
            hr = m_pRenderTarget->CreateSolidColorBrush(*e.color, e.brush);
    return hr;
}

void CustomScrollBar::ReleaseBrushes()
{
    SafeRelease(m_pBrushBackground);
    SafeRelease(m_pBrushTrack);
    SafeRelease(m_pBrushThumb);
    SafeRelease(m_pBrushThumbHover);
    SafeRelease(m_pBrushThumbActive);
    SafeRelease(m_pBrushMarker);
}

void CustomScrollBar::RecoverDeviceLoss()
{
    ReleaseBrushes();
    SafeRelease(m_pRenderTarget);
    if (SUCCEEDED(CreateRenderTarget()))
        CreateBrushes();
}

void CustomScrollBar::DrawScrollBar(float w, float h)
{
    if (!m_pRenderTarget)
        return;
    m_pRenderTarget->FillRectangle(D2D1::RectF(0, 0, w, h), m_pBrushBackground);
    if (m_bVertical)
    {
        float railW = std::max(1.0f, w / 20.0f);
        float railX = (w - railW) * 0.5f;
        m_pRenderTarget->FillRectangle(D2D1::RectF(railX, 0, railX + railW, h), m_pBrushTrack);
        if (m_bShowMarkers && m_nMarkerCount >= 2)
        {
            float markerLen = w * 0.30f;
            float step = h / static_cast<float>(m_nMarkerCount);
            for (int i = 0; i <= m_nMarkerCount; ++i)
            {
                float y = i * step;
                float len = ((i % 5) == 0) ? markerLen : markerLen * 0.55f;
                m_pRenderTarget->DrawLine(
                    D2D1::Point2F(0, y), D2D1::Point2F(len, y), m_pBrushMarker, 1.0f);
            }
        }
        float ty = ThumbLeadingPixel(static_cast<int>(h));
        float thumbH = static_cast<float>(m_nThumbThickness);
        ID2D1SolidColorBrush *pBr = m_bDragging  ? m_pBrushThumbActive
                                    : m_bHovered ? m_pBrushThumbHover
                                                 : m_pBrushThumb;
        m_pRenderTarget->FillRectangle(D2D1::RectF(0, ty, w, ty + thumbH), pBr);
    }
    else
    {
        float railH = std::max(1.0f, h / 20.0f);
        float railY = (h - railH) * 0.5f;
        m_pRenderTarget->FillRectangle(D2D1::RectF(0, railY, w, railY + railH), m_pBrushTrack);
        if (m_bShowMarkers && m_nMarkerCount >= 2)
        {
            float markerLen = h * 0.30f;
            float step = w / static_cast<float>(m_nMarkerCount);
            for (int i = 0; i <= m_nMarkerCount; ++i)
            {
                float x = i * step;
                float len = ((i % 5) == 0) ? markerLen : markerLen * 0.55f;
                m_pRenderTarget->DrawLine(
                    D2D1::Point2F(x, 0), D2D1::Point2F(x, len), m_pBrushMarker, 1.0f);
            }
        }
        float tx = ThumbLeadingPixel(static_cast<int>(w));
        float thumbW = static_cast<float>(m_nThumbThickness);
        ID2D1SolidColorBrush *pBr = m_bDragging  ? m_pBrushThumbActive
                                    : m_bHovered ? m_pBrushThumbHover
                                                 : m_pBrushThumb;
        m_pRenderTarget->FillRectangle(D2D1::RectF(tx, 0, tx + thumbW, h), pBr);
    }
}

LRESULT CustomScrollBar::OnCreate(UINT, WPARAM, LPARAM, BOOL &bHandled)
{
    DWORD dwStyle = static_cast<DWORD>(::GetWindowLong(m_hwnd, GWL_STYLE));
    m_bVertical = (dwStyle & SBS_VERT) != 0;
    m_bShowMarkers = (dwStyle & CSBS_MARKERS) != 0;
    if (!m_pD2DFactory)
        CreateD2DFactory();
    if (SUCCEEDED(CreateRenderTarget()))
        CreateBrushes();
    bHandled = FALSE;
    return 0;
}

LRESULT CustomScrollBar::OnDestroy(UINT, WPARAM, LPARAM, BOOL &bHandled)
{
    ReleaseResources(); // explicit early release while scheduler is live
    bHandled = FALSE;
    return 0;
}

LRESULT CustomScrollBar::OnPaint(UINT, WPARAM, LPARAM, BOOL &bHandled)
{
    PAINTSTRUCT ps;
    HDC hdc = ::BeginPaint(m_hwnd, &ps);
    if (!m_pRenderTarget)
    {
        ::EndPaint(m_hwnd, &ps);
        bHandled = TRUE;
        return 0;
    }
    if (!m_pBrushBackground)
        CreateBrushes();
    RECT rc;
    ::GetClientRect(m_hwnd, &rc);
    HRESULT hr = m_pRenderTarget->BindDC(hdc, &rc);
    if (hr == D2DERR_RECREATE_TARGET)
    {
        RecoverDeviceLoss();
        hr = m_pRenderTarget ? m_pRenderTarget->BindDC(hdc, &rc) : E_FAIL;
    }
    if (SUCCEEDED(hr))
    {
        m_pRenderTarget->BeginDraw();
        DrawScrollBar((float)(rc.right - rc.left), (float)(rc.bottom - rc.top));
        hr = m_pRenderTarget->EndDraw();
        if (hr == D2DERR_RECREATE_TARGET)
            RecoverDeviceLoss();
    }
    ::EndPaint(m_hwnd, &ps);
    bHandled = TRUE;
    return 0;
}

LRESULT CustomScrollBar::OnSize(UINT, WPARAM, LPARAM, BOOL &bHandled)
{
    Invalidate();
    bHandled = FALSE;
    return 0;
}

LRESULT CustomScrollBar::OnLButtonDown(UINT, WPARAM, LPARAM lParam, BOOL &bHandled)
{
    POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
    ::SetCapture(m_hwnd);
    SetFocus();
    if (HitTestThumb(pt))
    {
        m_bDragging = true;
        m_bHovered = true;
        m_nDragStartClientPos = m_bVertical ? pt.y : pt.x;
        m_nDragStartScrollPos = m_nPos;
    }
    else
    {
        RECT rc;
        ::GetClientRect(m_hwnd, &rc);
        int main = m_bVertical ? (rc.bottom - rc.top) : (rc.right - rc.left);
        int click = m_bVertical ? pt.y : pt.x;
        float leading = ThumbLeadingPixel(main);
        int newPos = (click < static_cast<int>(leading))
                         ? Clamp(m_nPos - m_nPage, m_nMin, m_nMax)
                         : Clamp(m_nPos + m_nPage, m_nMin, m_nMax);
        if (newPos != m_nPos)
        {
            m_nPos = newPos;
            NotifyParent(SB_THUMBPOSITION);
            Invalidate();
        }
    }
    bHandled = TRUE;
    return 0;
}

LRESULT CustomScrollBar::OnLButtonUp(UINT, WPARAM, LPARAM, BOOL &bHandled)
{
    if (m_bDragging)
    {
        m_bDragging = false;
        NotifyParent(SB_THUMBPOSITION);
        Invalidate();
    }
    ::ReleaseCapture();
    bHandled = TRUE;
    return 0;
}

LRESULT CustomScrollBar::OnMouseMove(UINT, WPARAM, LPARAM lParam, BOOL &bHandled)
{
    POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
    if (m_bDragging)
    {
        RECT rc;
        ::GetClientRect(m_hwnd, &rc);
        int main = m_bVertical ? (rc.bottom - rc.top) : (rc.right - rc.left);
        int click = m_bVertical ? pt.y : pt.x;
        int delta = click - m_nDragStartClientPos;
        int range = m_nMax - m_nMin;
        int track = main - m_nThumbThickness;
        if (range > 0 && track > 0)
        {
            int newPos = m_nDragStartScrollPos + static_cast<int>(static_cast<float>(delta) * range / static_cast<float>(track) + 0.5f);
            newPos = Clamp(newPos, m_nMin, m_nMax);
            if (newPos != m_nPos)
            {
                m_nPos = newPos;
                NotifyParent(SB_THUMBTRACK);
                Invalidate();
            }
        }
    }
    bool wasHovered = m_bHovered;
    m_bHovered = m_bDragging || HitTestThumb(pt);
    if (m_bHovered && !wasHovered)
    {
        TRACKMOUSEEVENT tme{sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hwnd, 0};
        ::TrackMouseEvent(&tme);
    }
    if (m_bHovered != wasHovered)
        Invalidate();
    bHandled = TRUE;
    return 0;
}

LRESULT CustomScrollBar::OnMouseLeave(UINT, WPARAM, LPARAM, BOOL &bHandled)
{
    if (m_bHovered)
    {
        m_bHovered = false;
        Invalidate();
    }
    bHandled = TRUE;
    return 0;
}

LRESULT CustomScrollBar::OnMouseWheel(UINT, WPARAM wParam, LPARAM, BOOL &bHandled)
{
    int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
    int step = (zDelta > 0) ? -3 : 3;
    int newPos = Clamp(m_nPos + step, m_nMin, m_nMax);
    if (newPos != m_nPos)
    {
        m_nPos = newPos;
        NotifyParent(SB_THUMBPOSITION);
        Invalidate();
    }
    bHandled = TRUE;
    return 0;
}

LRESULT CustomScrollBar::OnKeyDown(UINT, WPARAM vk, LPARAM, BOOL &bHandled)
{
    int newPos = m_nPos, code = -1;
    switch (vk)
    {
    case VK_UP:
    case VK_LEFT:
        newPos--;
        code = SB_LINEUP;
        break;
    case VK_DOWN:
    case VK_RIGHT:
        newPos++;
        code = SB_LINEDOWN;
        break;
    case VK_PRIOR:
        newPos -= m_nPage;
        code = SB_PAGEUP;
        break;
    case VK_NEXT:
        newPos += m_nPage;
        code = SB_PAGEDOWN;
        break;
    case VK_HOME:
        newPos = m_nMin;
        code = SB_TOP;
        break;
    case VK_END:
        newPos = m_nMax;
        code = SB_BOTTOM;
        break;
    default:
        bHandled = FALSE;
        return 0;
    }
    newPos = Clamp(newPos, m_nMin, m_nMax);
    if (newPos != m_nPos)
    {
        m_nPos = newPos;
        NotifyParent(code);
        Invalidate();
    }
    bHandled = TRUE;
    return 0;
}

LRESULT CustomScrollBar::OnKillFocus(UINT, WPARAM, LPARAM, BOOL &bHandled)
{
    Invalidate();
    bHandled = FALSE;
    return 0;
}

LRESULT CustomScrollBar::OnSetFocus(UINT, WPARAM, LPARAM, BOOL &bHandled)
{
    Invalidate();
    bHandled = FALSE;
    return 0;
}

LRESULT CustomScrollBar::OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL &bHandled)
{
    bHandled = TRUE;
    return 1;
}