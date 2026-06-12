#include "Checker3_child.h"
#include "Resource.h"

HRESULT Checker3_child::Render()
{
    HRESULT hr = S_OK;
    if(!m_rt)
    {
        hr = CreateDeviceResources();
    }
    if(SUCCEEDED(hr))
    {
        auto size = m_rt->GetSize();
        m_rt->SetTransform(D2D1::Matrix3x2F::Identity());
        m_rt->BeginDraw();
        m_rt->Clear(m_background);
        m_rt->DrawRectangle(D2D1::RectF(0.f, 0.f, size.width, size.height),m_brush.Get());
        if(m_checked)
        {
            m_rt->DrawLine(D2D1::Point2F(0.f,0.f), 
                          D2D1::Point2F(static_cast<float>(size.width), static_cast<float>(size.height)),
                        m_brush.Get());
            m_rt->DrawLine(D2D1::Point2F(0.f,static_cast<float>(size.height)), 
                          D2D1::Point2F(static_cast<float>(size.width), 0.f),
                        m_brush.Get());
        }
        hr = m_rt->EndDraw();
        if(hr == D2DERR_RECREATE_TARGET)
        {
            DestroyDeviceResources();
            hr = CreateDeviceResources();
        }
    }
    return hr;
}


HRESULT Checker3_child::CreateDeviceResources()
{
    HRESULT hr = S_OK;
    if(m_factory)
    {
        if(!m_rt)
        {
            Rect rc;
            GetWindowRect(&rc);
            hr = m_factory->CreateHwndRenderTarget(
                D2D1::RenderTargetProperties(),
                D2D1::HwndRenderTargetProperties(
                    *this,
                    D2D1::SizeU(rc.Width(), rc.Height())),
                    m_rt.GetAddressOf()
                );
            
            if(SUCCEEDED(hr))
            {
                hr = m_rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), m_brush.GetAddressOf());
            }
        }
    }
    return hr;
}

void Checker3_child::DestroyDeviceResources()
{
    if(m_brush)
    {
        auto brush = m_brush.ReleaseAndGetAddressOf();
        brush = nullptr;
    }
    if(m_rt)
    {
        auto rt = m_rt.ReleaseAndGetAddressOf();
        rt = nullptr;
    }
}

HRESULT Checker3_child::Initalizie(UINT ID, HWND parent, ComPtr<ID2D1Factory>& factory)
{
    m_factory = factory;
    HRESULT hr = S_OK;
    
    m_background = m_backgroundDef;
    if(SUCCEEDED(hr))
    {
        Rect rc = {0,0,10,10};
        
        if(ID == IDC_CHECKER3_CHILD) //110
        {
            hr = ::IsWindow(Create(parent,(LPRECT)&rc ,(LPCTSTR)TEXT(""),WS_CHILD | WS_VISIBLE,(DWORD)0,UMenuOrID{ID}.Get(), nullptr)) ? S_OK : E_FAIL;
        } 
        else
        {
            rc.left = 0;
            rc.bottom = 0;
            hr = (::IsWindow(Create(parent,(LPRECT)&rc,TEXT("Checker3_child"),(DWORD)WS_CHILD | WS_VISIBLE,(DWORD)0, UMenuOrID{ID}.Get(), nullptr)) ? S_OK : E_FAIL);
        }
    }
    return hr;
}

BOOL Checker3_child::Checked()
{
    return m_checked;
}

void Checker3_child::SetChecked(BOOL checked)
{
    m_checked = checked;
    Invalidate(FALSE);
}

void Checker3_child::OnMouseMove(UINT nFlags, const Point& point)
{
    if(!isTracking)
    {
        TRACKMOUSEEVENT ts = {0};
        ts.cbSize = sizeof(TRACKMOUSEEVENT);
        ts.dwFlags = TME_LEAVE | TME_HOVER;
        ts.dwHoverTime = HOVER_DEFAULT;
        ts.hwndTrack = *this;
        isTracking = TrackMouseEvent(&ts);
    }
    m_background = m_mouseOver;
    Invalidate(FALSE);
}

void Checker3_child::OnMouseLeave()
{
    if(isTracking)
    {    
        TRACKMOUSEEVENT ts = {0};
        ts.cbSize = sizeof(TRACKMOUSEEVENT);
        ts.dwFlags = TME_CANCEL | TME_LEAVE | TME_HOVER;
        ts.dwHoverTime = HOVER_DEFAULT;
        ts.hwndTrack = *this;
        TrackMouseEvent(&ts);
        isTracking = !isTracking;
    }
    m_background = m_backgroundDef;
    Invalidate(FALSE);
}

void Checker3_child::OnLButtonDown(UINT nFlags, const Point& point)
{
    m_background = m_mouseDown;
    m_checked = !m_checked;
    Invalidate(FALSE);
}

void Checker3_child::OnLButtonUp(UINT nFlags, const Point& point)
{
    m_background = m_mouseOver;
    Invalidate(FALSE);
}

void Checker3_child::OnPaint(DC dc)
{
    BeginPaint(&ps);
    Render();
    EndPaint(&ps);
}

void Checker3_child::OnSize(UINT nFlags,Size sz)
{
    if(m_rt){
        m_rt->Resize(D2D1::SizeU(sz.cx, sz.cy));
    }
    Invalidate(FALSE);
}

int Checker3_child::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    
    m_checked = false;
    return 0;
}

void Checker3_child::OnDestroy()
{
    DestroyDeviceResources();
    if(m_factory)
    {
        auto ft = m_factory.ReleaseAndGetAddressOf();
        ft = nullptr;
    }
}

void Checker3_child::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    auto lparam = MAKELPARAM(nRepCnt, nFlags);
    ::SendMessage(GetParent(), WM_KEYDOWN, (WPARAM)nChar, lparam);
}