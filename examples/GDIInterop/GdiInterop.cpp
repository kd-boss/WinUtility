#include "GdiInterop.h"
#include <stdlib.h>
#include <math.h>
#include <WinUtility/Numbers.h>

HRESULT GdiInterop::CreateDeviceIndependantResources()
{
    HRESULT hr = S_OK;
    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_ISOLATED,__uuidof(IDWriteFactory),&m_dwfactory);
    if(SUCCEEDED(hr))
    {      
        hr =  m_dwfactory->CreateTextFormat(TEXT("Verdana"),nullptr,DWRITE_FONT_WEIGHT_NORMAL,DWRITE_FONT_STYLE_NORMAL,DWRITE_FONT_STRETCH_NORMAL, 16.0f,TEXT("en-us"),&m_format);
        
    }
    if(SUCCEEDED(hr))
    {
        hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_factory.GetAddressOf());    
    }
    return hr;
}

HRESULT GdiInterop::CreateDeviceResources()
{
    HRESULT hr  = S_OK;
    if(!m_renderTarget)
    {
        D2D1_RENDER_TARGET_PROPERTIES properties = D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(
                DXGI_FORMAT_B8G8R8A8_UNORM,
                D2D1_ALPHA_MODE_PREMULTIPLIED),
                0,
                0,
                D2D1_RENDER_TARGET_USAGE_NONE,
                D2D1_FEATURE_LEVEL_DEFAULT
            );

        hr = m_factory->CreateDCRenderTarget(&properties,&m_renderTarget);
        if(SUCCEEDED(hr))
        {
            hr = m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black),&m_brush);
            if(SUCCEEDED(hr))
            {
                hr = m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(255.0f, 0.0f,0.0f, 0.5f),&m_alphabrush);
            }
        }
    }
    return hr;
}

HRESULT GdiInterop::OnRender(const PAINTSTRUCT& ps)
{
    HRESULT hr = S_OK;
    Rect rc;
    GetClientRect(&rc);

    hr = CreateDeviceResources();
    if(SUCCEEDED(hr))
    {
        m_renderTarget->BindDC(ps.hdc,&rc);
        m_renderTarget->BeginDraw();
        m_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
        m_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

        m_renderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(150.0f,150.0f),100.0f,100.0f),m_brush.Get(), 3.0f);
        std::tstring str =  TEXT("Some alpha blended text");
        m_renderTarget->DrawTextW(str.c_str(), convert_to<UINT32>(str.length()),m_format.Get(),D2D1::RectF(150.0f, 150.0f, 150.0f/2.0f + 150.0f, 150.0f/2.0f - 16.0f),m_alphabrush.Get());
        m_renderTarget->DrawLine(
           line1[0],line1[1],
        m_brush.Get(), 3.0f
        );

        m_renderTarget->DrawLine(
            line2[0], line2[1],
        m_brush.Get(), 3.0f
        );

        m_renderTarget->DrawLine(
            line3[0],line3[1],
            m_brush.Get(), 3.0f
        );

        hr = m_renderTarget->EndDraw();
        if(SUCCEEDED(hr))
        {
            HGDIOBJ origional = nullptr;
            origional = SelectObject(ps.hdc,GetStockObject(DC_PEN));
            Pen blackPen = ::CreatePen(PS_SOLID,3,0);
            SelectObject(ps.hdc,&blackPen);
            Ellipse(ps.hdc, 300,50,500,250);
            

            Polyline(ps.hdc,pntArray,2);
            Polyline(ps.hdc,pntArray2,2);
            Polyline(ps.hdc,pntArray3,2);
            blackPen.DeleteObject();
            SelectObject(ps.hdc,origional);
        }
        if(hr == D2DERR_RECREATE_TARGET)
        {
            hr = S_OK;
            DiscardDeviceResources();
        }
    }
return hr;
}

void GdiInterop::DiscardDeviceResources()
{
    auto ptr = m_renderTarget.ReleaseAndGetAddressOf();
    ptr = nullptr;
    auto ptrbrush = m_brush.ReleaseAndGetAddressOf();
    ptrbrush = nullptr;
}

void GdiInterop::OnPaint(DC dc)
{
    PAINTSTRUCT ps;
    BeginPaint(&ps);
    OnRender(ps);
    EndPaint(&ps);
}

int GdiInterop::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    m_btnSend.Create(*this,RECT{10,10,120,30},TEXT("Send"),UMenuOrID{1});
    m_editBox.Create(*this,RECT{10,50,150,80}, TEXT("Edit"),WS_CHILD | WS_VISIBLE,0L,UMenuOrID{2}.Get(),nullptr);
    if(m_editBox.IsWindow())
    {
       m_editBox.ShowWindow(ShowWindowType::Show);
    }
    else
    {
        std::cout << "failed to create the edit control" << std::endl;
    }   
    return TRUE;
}

void GdiInterop::OnDestroy()
{
    PostQuitMessage(0);
}

void GdiInterop::OnSendClick(UINT uNotifyCode, int nID, Window wndCtl)
{
    MessageBox::Show(*this,TEXT("Send Clicked!"), TEXT("GdiInterop example"),MessageBoxButtons::Ok,MessageBoxIcon::Information);
}

HRESULT GdiInterop::Initalize()
{
    auto hr =  CreateDeviceIndependantResources();
    if(SUCCEEDED(hr))
    {
        float dpiX = 96.0f;
        float dpiY = dpiX;
        
        Rect rc = {0,0,
                static_cast<int>(::ceil(640.0f * dpiX / 96.0f)),
                static_cast<int>(::ceil(480.0f * dpiY / 96.0f))
                };

        Create(nullptr,&rc,TEXT("GdiInterop"));

        hr = IsWindow() ? S_OK : E_FAIL;
        if(SUCCEEDED(hr))
        {
            dpiX = convert_to<float>(GetDpiForWindow());
            dpiY = dpiX;
            Rect rb = {0,0,
                static_cast<int>(::ceil(640.0f * dpiX / 96.0f)),
                static_cast<int>(::ceil(480.0f * dpiY / 96.0f))
                };
            MoveWindow(rb.left,rb.top,rb.Width(),rb.Height(),FALSE);
            
            ShowWindow(ShowWindowType::Show);
            UpdateWindow();
        }
    }
    return hr;
}
