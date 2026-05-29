#include "DEVICECAPS1.h"
#include <format>


HRESULT DEVICECAPS1::Initialize()
{
    HRESULT hr = S_OK;
    std::tstring windowTitle = TEXT("DEVICECAPS1");
    Create(nullptr,&Window::rcDefault, windowTitle.c_str());
    hr = IsWindow() ? S_OK : E_FAIL;
    if(SUCCEEDED(hr))
    {
        ShowWindow(ShowWindowType::Show);
    }

    return hr;
}

int DEVICECAPS1::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    DC thisdc = GetWindowDC();
    for(auto &e: devicecaps)
    {
        e.szVal = std::format(TEXT("{:d}"), ::GetDeviceCaps(thisdc,e.iIndex));
    }
    ReleaseDC(thisdc);
    HR(CreateDeviceIndependantResources());
    HR(CreateDeviceResources());
    return 0;
}

void DEVICECAPS1::OnPaint(DC dc)
{
    PAINTSTRUCT ps;
    auto hdc = BeginPaint(&ps);
    Rect rc;
    GetClientRect(&rc);
    m_renderTarget->BindDC(hdc,rc);
    m_renderTarget->BeginDraw();
    m_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
    auto colwidth = rc.right / 3;
    auto lineheight = m_format->GetFontSize();
    D2D1_RECT_F col1 = {
        static_cast<float>(rc.left),
        static_cast<float>(rc.top),
        static_cast<float>(colwidth),
        static_cast<float>(rc.top) + lineheight
    };

    D2D1_RECT_F col2 = {
        static_cast<float>(rc.left) + colwidth,
        static_cast<float>(rc.top),
        static_cast<float>(colwidth * 2),
        static_cast<float>(rc.top) + lineheight
    };

    D2D1_RECT_F col3 = {
        static_cast<float>(rc.left) + colwidth * 2,
        static_cast<float>(rc.top),
        static_cast<float>(rc.right),
        static_cast<float>(rc.top) + lineheight
    };
    
    for(auto a : devicecaps)
    {
       m_renderTarget->DrawTextW(a.szLabel.c_str(), a.szLabel.length(), m_format.Get(),col1,m_blackbrush.Get());
       m_renderTarget->DrawTextW(a.szDesc.c_str(), a.szDesc.length(),m_format.Get(),col2,m_blackbrush.Get());
       m_renderTarget->DrawTextW(a.szVal.c_str(), a.szVal.length(), m_format.Get(),col3,m_blackbrush.Get());
       col1.top += lineheight;
       col1.bottom += lineheight;
       col2.top += lineheight;
       col2.bottom += lineheight;
       col3.top += lineheight;
       col3.bottom += lineheight;
    }

    m_renderTarget->EndDraw();
    EndPaint(&ps);
}

void DEVICECAPS1::OnAbout(UINT uNotifyCode, int nID, Window wndCtl)
{
      if(!about)
        {
                if(about.DoModal() == IDOK)
                {
                        MessageBox::Show(*this,
                            TEXT("OK pressed!"), 
                            TEXT("DEICECAPS1"),
                            MessageBoxButtons::Ok,
                            MessageBoxIcon::Information
                        );
                }
        }

}

void DEVICECAPS1::OnExit(UINT uNotifyCode, int nID, Window wndCtl)
{
    DestroyWindow();
}

void DEVICECAPS1::OnDestroy()
{
    PostQuitMessage(0);
}

void DEVICECAPS1::OnClose()
{
    DestroyWindow();
}

HRESULT DEVICECAPS1::CreateDeviceIndependantResources()
{
    HRESULT hr = S_OK;
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_factory.GetAddressOf());
    if(SUCCEEDED(hr))
    {
        hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_ISOLATED, __uuidof(IDWriteFactory),&m_dwriteFactory);
        if(SUCCEEDED(hr))
        {
            hr = m_dwriteFactory->CreateTextFormat(TEXT("Verdana"),nullptr,DWRITE_FONT_WEIGHT_NORMAL,DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 16.0f,TEXT("en-us"), &m_format);
        }
    }
    return hr;
}

HRESULT DEVICECAPS1::CreateDeviceResources()
{
    HRESULT hr = S_OK;
    
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

    hr = m_factory->CreateDCRenderTarget(&properties, &m_renderTarget);
    if(SUCCEEDED(hr))
    {
        hr = m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black),&m_blackbrush);
    }

    return hr;
}