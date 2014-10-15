/*
    WinTest, Small Demo application that creates a user interface window.
    release build - compile with :
        g++ wintest.cpp -std=c++1y -municode -mwindows -static -s -O3 -ld2d1 -o wintest.exe
    debug build - compile with :
        g++ wintest.cpp -std=c++1y -municode -mwindows -g -ld2d1 -o wintest.exe
*/


#include <utility/BaseWindow.h>
#include <utility/ComPtr.h>
#include <d2d1.h>

class MyWindow : public BaseWindow<MyWindow,FrameWinTraits>{

ComPtr<ID2D1Factory> m_factory;
ComPtr<ID2D1HwndRenderTarget> m_target;
ComPtr<ID2D1SolidColorBrush> m_brush;

HRESULT CreateDeviceResources();
HRESULT CreateDeviceIndepenantResources();
void OnCreate(LPCREATESTRUCT lpCreate);
void OnPaint(LPPAINTSTRUCT ps);
void OnSize(UINT nFlags, Size sz);
void OnClose();

public:
BEGIN_MSG_MAP()
    MSG_WM_CREATE(OnCreate)
    MSG_WM_PAINT(OnPaint)
    MSG_WM_SIZE(OnSize)
    MSG_WM_CLOSE(OnClose)
END_MSG_MAP()

PCTSTR ClassName() const;
};

PCTSTR MyWindow::ClassName() const{
    return const_cast<WCHAR*>(L"TestWindow");
}

HRESULT MyWindow::CreateDeviceResources(){
    HRESULT hr = S_OK;
    if(!m_factory)
      hr = CreateDeviceIndepenantResources();
      
    if(!m_target){
        Rect rc;
        GetClientRect(rc);
        hr = m_factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
                                                D2D1::HwndRenderTargetProperties(m_hwnd,D2D1::SizeU(rc.Width(),rc.Height())),
                                                m_target.GetAddressOf());
    }
    
    if(!m_brush){
        hr = m_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black),m_brush.GetAddressOf());
    }
    return hr;
}

HRESULT MyWindow::CreateDeviceIndepenantResources(){
    HRESULT hr = S_OK;
    if(!m_factory)
        hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,m_factory.GetAddressOf());
    return hr;
}

void MyWindow::OnCreate(LPCREATESTRUCT lpCreate){
    CreateDeviceIndepenantResources();
}

void MyWindow::OnPaint(LPPAINTSTRUCT ps){
     if(!m_target)
        CreateDeviceResources();
     
     Rect rc;
     GetClientRect(rc);
     m_target->BeginDraw();
     m_target->Clear(D2D1::ColorF(D2D1::ColorF::Yellow));
     m_target->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(rc.Center().x,rc.Center().y),(rc.Width()/2) - 10.0f ,(rc.Height()/2) - 10.0f),m_brush.Get());
     m_target->EndDraw();
}

void MyWindow::OnSize(UINT nFlags, Size sz){
    if(m_target){
        m_target->Resize(D2D1::SizeU(sz.cx, sz.cy));
    }
    Invalidate();
}

void MyWindow::OnClose(){
    PostQuitMessage(0);
}

int WINAPI wWinMain(HINSTANCE,HINSTANCE, LPTSTR,int nShow){
    MyWindow win;
    win.Create(L"Test Win");
    win.Show(nShow);
    win.UpdateWindow();
    
    MSG msg;
    while(GetMessage(&msg,nullptr,0,0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

