#ifndef CHECKER3_CHILD_H
#define CHECKER3_CHILD_H
#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include <WinUtility/ComPtr.h>
#include <d2d1.h>
#include <d2d1helper.h>

typedef WinTraits<WS_CHILD | WS_VISIBLE,0> ChildWindowTraits;

class Checker3_child : public BaseWindow<Checker3_child, Window, ChildWindowTraits>
{
    PAINTSTRUCT ps;
    BOOL m_checked;
    ComPtr<ID2D1Factory> m_factory;
    ComPtr<ID2D1HwndRenderTarget> m_rt;
    ComPtr<ID2D1SolidColorBrush> m_brush;
    D2D1_COLOR_F m_background;
    bool isTracking;
    const D2D1_COLOR_F m_backgroundDef = D2D1::ColorF(D2D1::ColorF::SkyBlue);
    const D2D1_COLOR_F m_mouseOver = D2D1::ColorF(D2D1::ColorF::Orange);
    const D2D1_COLOR_F m_mouseDown = D2D1::ColorF(D2D1::ColorF::Blue);

    void OnMouseMove(UINT nFlags, Point point);
    void OnLButtonDown(UINT nFlags, Point point);
    void OnLButtonUp(UINT nFlags, Point point);
    void OnMouseLeave();
    
    HRESULT CreateDeviceResources();
    HRESULT Render();
    void DestroyDeviceResources();
    void OnPaint(DC dc);
    void OnSize(UINT nFlags, Size sz);
    int OnCreate(LPCREATESTRUCT lpCreateStruct);
    void OnDestroy();
    void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
    HRESULT Initalizie(UINT ID, HWND parent,ComPtr<ID2D1Factory>& factory);
    BOOL Checked();
    void SetChecked(BOOL checked);

    DECLARE_WND_CLASS(TEXT("Checker3_child"));

    BEGIN_MSG_MAP()
        MSG_WM_MOUSELEAVE(OnMouseLeave)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_SIZE(OnSize)
        MSG_WM_PAINT(OnPaint)
        MSG_WM_MOUSEMOVE(OnMouseMove)  
        MSG_WM_LBUTTONDOWN(OnLButtonDown)
        MSG_WM_LBUTTONUP(OnLButtonUp)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_KEYDOWN(OnKeyDown)
    END_MSG_MAP()
};

#endif