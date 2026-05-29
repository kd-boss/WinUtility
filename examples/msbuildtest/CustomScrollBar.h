// =============================================================================
//  CustomScrollBar.h
//
//  A Direct2D / GDI-interop rendered vertical or horizontal scroll bar control
//  built on the BaseWindow.h CRTP framework.
//
//  Crash-safety notes
//  ------------------
//   * Create ONE shared ID2D1Factory in wWinMain (or MyWindow::OnCreate)
//     and pass it to each control via SetSharedFactory().  Never let
//     individual controls create their own factories.
//   * Call CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED) at the top
//     of wWinMain, and CoUninitialize() after your message loop exits.
//     This ensures D2D's internal COM apartment threads are properly
//     drained before process teardown.
//   * ReleaseResources() is called automatically from OnDestroy.  You may
//     also call it explicitly before destroying the window if needed.
//
//  Public API (mirrors ScrollBarControl / ScrollBarT<Window>):
//    GetScrollPos() / SetScrollPos(pos, redraw)
//    GetScrollRange(min, max) / SetScrollRange(min, max, redraw)
//    GetScrollInfo(LPSCROLLINFO) / SetScrollInfo(LPSCROLLINFO, redraw)
//    SetThumbThickness(px) / GetThumbThickness()
//    SetMarkerCount(n)     / GetMarkerCount()
//    SetSharedFactory(ID2D1Factory*)
//    ReleaseResources()    [public]
//
//  Style bits:
//    SBS_VERT   – vertical  (default)
//    SBS_HORZ   – horizontal
//    CSBS_MARKERS – draw graduated tick marks along the track
//
//  Notifications: WM_VSCROLL / WM_HSCROLL → parent
//    LOWORD(wParam) = SB_THUMBTRACK (drag) or SB_THUMBPOSITION (release)
//    HIWORD(wParam) = current position
//    lParam         = HWND of this control
//
//  Dependencies: BaseWindow.h (include first), d2d1.lib
// =============================================================================
#pragma once
#ifndef __CUSTOM_SCROLL_BAR_H__
#define __CUSTOM_SCROLL_BAR_H__

#ifndef __BASEWINDOW_H__
#error "CustomScrollBar.h requires BaseWindow.h to be included first."
#endif

#include <d2d1.h>
#include <d2d1helper.h>

#ifdef _MSC_VER
#pragma comment(lib, "d2d1.lib")
#endif

#ifndef CSBS_MARKERS
#define CSBS_MARKERS 0x0010UL
#endif

// ── Fluent colour palette ─────────────────────────────────────────────────────
namespace ScrollBarColors
{
    static constexpr D2D1_COLOR_F Background = {0.110f, 0.110f, 0.110f, 1.0f};
    static constexpr D2D1_COLOR_F Track = {0.227f, 0.227f, 0.227f, 1.0f};
    static constexpr D2D1_COLOR_F Thumb = {0.420f, 0.420f, 0.420f, 1.0f};
    static constexpr D2D1_COLOR_F ThumbHover = {0.553f, 0.553f, 0.553f, 1.0f};
    static constexpr D2D1_COLOR_F ThumbActive = {0.659f, 0.659f, 0.659f, 1.0f};
    static constexpr D2D1_COLOR_F Marker = {0.290f, 0.290f, 0.290f, 1.0f};
}

// =============================================================================
class CustomScrollBar
    : public BaseWindow<CustomScrollBar, Window,
                        WinTraits<WS_CHILD | WS_VISIBLE | WS_TABSTOP>>
{
public:
    DECLARE_WND_CLASS(TEXT("CustomScrollBar"))

    CustomScrollBar();
    ~CustomScrollBar();

    // ── ScrollBarControl-compatible API ───────────────────────────────────────
    int GetScrollPos() const;
    int SetScrollPos(int nPos, BOOL bRedraw = TRUE);
    void GetScrollRange(LPINT lpMin, LPINT lpMax) const;

    void SetScrollRange(int nMin, int nMax, BOOL bRedraw = TRUE);

    BOOL GetScrollInfo(LPSCROLLINFO lpSI) const;
    int SetScrollInfo(LPSCROLLINFO lpSI, BOOL bRedraw = TRUE);
    void SetThumbThickness(int nThickness);
    int GetThumbThickness() const;

    void SetMarkerCount(int nCount);
    int GetMarkerCount() const;

    void SetMarkerColor(D2D1_COLOR_F c);
    D2D1_COLOR_F GetMarkerColor();

    // ── Shared factory (call before Create) ──────────────────────────────────
    void SetSharedFactory(ID2D1Factory *pFactory);

    // ── Public resource release ───────────────────────────────────────────────
    void ReleaseResources();
    // ── Message map ───────────────────────────────────────────────────────────
    BEGIN_MSG_MAP()
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    MESSAGE_HANDLER(WM_PAINT, OnPaint)
    MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
    MESSAGE_HANDLER(WM_SIZE, OnSize)
    MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
    MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
    MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
    MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
    MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
    MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
    MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
    MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
    END_MSG_MAP()

private:
    int m_nMin, m_nMax, m_nPos, m_nPage;
    int m_nThumbThickness, m_nMarkerCount;
    bool m_bVertical, m_bShowMarkers;
    bool m_bHovered, m_bDragging;
    int m_nDragStartClientPos, m_nDragStartScrollPos;
    bool m_bOwnsFactory;

    ID2D1Factory *m_pD2DFactory;
    ID2D1DCRenderTarget *m_pRenderTarget;
    ID2D1SolidColorBrush *m_pBrushBackground, *m_pBrushTrack;
    ID2D1SolidColorBrush *m_pBrushThumb, *m_pBrushThumbHover, *m_pBrushThumbActive;
    ID2D1SolidColorBrush *m_pBrushMarker;

    // ── Helpers ───────────────────────────────────────────────────────────────
    static int Clamp(int v, int lo, int hi) noexcept { return v < lo ? lo : v > hi ? hi
                                                                                   : v; }
    template <class T>
    static void SafeRelease(T *&p) noexcept
    {
        if (p)
        {
            p->Release();
            p = nullptr;
        }
    }
    void Invalidate() const;

    float ThumbLeadingPixel(int clientMain) const noexcept;

    bool HitTestThumb(POINT pt) const noexcept;
    void NotifyParent(int nCode) const;

    // ── D2D resource management ───────────────────────────────────────────────
    HRESULT CreateD2DFactory();
    HRESULT CreateRenderTarget();

    HRESULT CreateBrushes();
    void ReleaseBrushes();

    void RecoverDeviceLoss();

    // ── Drawing ───────────────────────────────────────────────────────────────
    void DrawScrollBar(float w, float h);

    // ── Message handlers ──────────────────────────────────────────────────────

    LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL &bHandled);

    LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL &bHandled);

    LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL &bHandled);

    LRESULT OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL &bHandled);

    LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL &bHandled);

    LRESULT OnLButtonDown(UINT, WPARAM, LPARAM lParam, BOOL &bHandled);

    LRESULT OnLButtonUp(UINT, WPARAM, LPARAM, BOOL &bHandled);

    LRESULT OnMouseMove(UINT, WPARAM, LPARAM lParam, BOOL &bHandled);

    LRESULT OnMouseLeave(UINT, WPARAM, LPARAM, BOOL &bHandled);

    LRESULT OnMouseWheel(UINT, WPARAM wParam, LPARAM, BOOL &bHandled);

    LRESULT OnKeyDown(UINT, WPARAM vk, LPARAM, BOOL &bHandled);
    LRESULT OnSetFocus(UINT, WPARAM, LPARAM, BOOL &bHandled);
    LRESULT OnKillFocus(UINT, WPARAM, LPARAM, BOOL &bHandled);
};

typedef CustomScrollBar CustomScrollBarControl;
#endif // __CUSTOM_SCROLL_BAR_H__
