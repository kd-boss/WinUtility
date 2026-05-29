#ifndef WINVER
#define WINVER 0x0A00000D 
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00000D
#endif
#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include <WinUtility/ComPtr.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>


typedef WinTraits<ES_LEFT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR> ChildTraits;
class EllipseButton : public BaseWindow<EllipseButton, Window, ChildTraits>
{

    COLORREF m_color = {0};
    ComPtr<ID2D1Factory> m_factory;
    ComPtr<ID2D1DCRenderTarget> m_target;
    ComPtr<ID2D1SolidColorBrush> m_brush;
    ComPtr<IDWriteTextFormat> m_fmt;
    ComPtr<IDWriteFactory> m_dwrite;
    D2D1_COLOR_F m_background;
    D2D1_COLOR_F m_foreground;
    HRESULT CreateDeviceIndependantResorces();
    HRESULT CreateDeviceResources();
    void DiscardDeviceResources();
    void OnPaint(DC dc);
    int OnCreate(LPCREATESTRUCT lpCreate);
    HRESULT Render(DC dc, Rect rect);
    void OnKeyUp(UINT nChar, INT nRepCnt, USHORT nFlags);
    void OnLButtonUp(UINT nFlags, Point point);
    void OnLButtonDown(UINT nFlags, Point point);
    void OnMouseMove(UINT nFlags, Point point);
     void OnMouseLeave();
    public:
    void SetBackColor(D2D1_COLOR_F clr) { m_background = clr;}
    DECLARE_WND_CLASS(TEXT("EllipseButton"))

    BEGIN_MSG_MAP()
      MSG_WM_CREATE(OnCreate)
      MSG_WM_PAINT(OnPaint)
      MSG_WM_KEYUP(OnKeyUp)
      MSG_WM_LBUTTONUP(OnLButtonUp)
      MSG_WM_LBUTTONDOWN(OnLButtonDown)
      MSG_WM_MOUSEMOVE(OnMouseMove)
      MSG_WM_MOUSELEAVE(OnMouseLeave)
    END_MSG_MAP()
};