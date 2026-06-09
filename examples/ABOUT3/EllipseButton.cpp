#include "EllipseButton.h"
#include "winuser.h"
#include <cmath>
#include <WinUtility/Numbers.h>

HRESULT EllipseButton::CreateDeviceIndependantResorces()
{
    HRESULT hr = S_OK;
    if (!m_factory)
    {
        hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_factory.GetAddressOf());
    }
    if (!m_dwrite)
    {
        hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_ISOLATED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown **>(m_dwrite.GetAddressOf()));
        if (SUCCEEDED(hr))
        {
            hr = m_dwrite->CreateTextFormat(TEXT("Consolas"), nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 12, TEXT("en-US"), m_fmt.GetAddressOf());
        }
    }

    m_foreground = D2D1::ColorF(D2D1::ColorF::Black);
    return hr;
}

HRESULT EllipseButton::CreateDeviceResources()
{
    HRESULT hr = S_OK;
    if (m_factory)
    {
        if (!m_target)
        {
            const auto properties = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
                                                                 D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
                                                                 convert_to<float>(::GetDpiForWindow(*this)), convert_to<float>(::GetDpiForWindow(*this)));
            hr = m_factory->CreateDCRenderTarget(&properties, m_target.GetAddressOf());
            if (SUCCEEDED(hr))
            {
                hr = m_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::SkyBlue), m_brush.GetAddressOf());
            }
        }
    }
    return hr;
}

void EllipseButton::DiscardDeviceResources()
{
    if (m_brush)
    {
        auto adr = m_brush.ReleaseAndGetAddressOf();
        adr = nullptr;
    }
    if (m_target)
    {
        auto adr = m_target.ReleaseAndGetAddressOf();
        adr = nullptr;
    }
}

int EllipseButton::OnCreate(LPCREATESTRUCT lpCreate)
{
    if (SUCCEEDED(CreateDeviceIndependantResorces()))
    {
        return 0;
    }
    else
        return 1;
}

HRESULT EllipseButton::Render(DC dc, Rect rect)
{
    HRESULT hr = S_OK;
    if (!m_target)
    {
        hr = CreateDeviceResources();
    }
    if (SUCCEEDED(hr))
    {
        std::tstring ts;
        GetWindowText(ts);
        m_target->BindDC(dc, &rect);
        m_target->SetTransform(D2D1::Matrix3x2F::Identity());
        m_target->BeginDraw();
        m_target->Clear(D2D1::ColorF(240.f, 240.f, 240.f,0.94f));
        auto pt = rect.CenterPoint();
        m_brush->SetColor(m_background);
        m_target->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(convert_to<float>(pt.x), convert_to<float>(pt.y)), convert_to<float> (rect.Width() / 2),convert_to<float>( rect.Height() / 2)), m_brush.Get());
        m_target->FillEllipse(D2D1::Ellipse(D2D1::Point2F(convert_to<float>(pt.x), convert_to<float>(pt.y)), convert_to<float> (rect.Width() / 2),convert_to<float>( rect.Height() / 2)), m_brush.Get());
        m_brush->SetColor(m_foreground);
        m_target->DrawTextW(ts.c_str(), convert_to<UINT32>(ts.length()), m_fmt.Get(), D2D1::RectF(rect.right /2 - (m_fmt->GetFontSize() / 2 * ts.length() /2), rect.bottom/2 - m_fmt->GetFontSize() /2 , convert_to<float>(rect.right),convert_to<float>( rect.bottom)), m_brush.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
        hr = m_target->EndDraw();
    }
    if (hr == D2DERR_RECREATE_TARGET)
    {
        DiscardDeviceResources();
    }
    return hr;
}

void EllipseButton::OnPaint(DC dc)
{
    PAINTSTRUCT ps;
    BeginPaint(&ps);
    Render(ps.hdc, ps.rcPaint);
    EndPaint(&ps);
}

void EllipseButton::OnKeyUp(UINT nChar, INT nRepCnt, USHORT nFlags)
{
    if (nChar == VK_SPACE)
    {
        ::SendMessage(GetParent(), WM_COMMAND, GetWindowLongPtr(GWL_ID), LPARAM(this->m_hwnd));
    }
}

void EllipseButton::OnMouseLeave()
{
        Rect rect;
        GetClientRect(rect);
        auto dc = GetDC();
        Render(dc, rect);
        ReleaseDC(dc);
}

void EllipseButton::OnMouseMove(UINT nFlags, const Point& point)
{
    TRACKMOUSEEVENT tme = {0};
    tme.cbSize = sizeof(TRACKMOUSEEVENT);
    tme.dwFlags = TME_LEAVE;
    tme.hwndTrack = *this;
    TrackMouseEvent(&tme);
    D2D1_COLOR_F back = m_background;
    m_background = D2D1::ColorF(D2D1::ColorF::LightBlue);
    Rect rect;
    GetClientRect(rect);
    Point pt = rect.CenterPoint();
    auto elip = D2D1::Ellipse(D2D1::Point2F(convert_to<float>(pt.x), convert_to<float>(pt.y)), convert_to<float>(rect.Width() / 2), convert_to<float>(rect.Height() / 2));
    if(((std::pow(static_cast<float>(point.x) - elip.point.x,2))/std::pow(elip.radiusX,2) + std::pow((static_cast<float>(point.y) - elip.point.y),2)/std::pow(elip.radiusY,2) <= 1)) 
    {
        auto dc = GetDC();
        Render(dc, rect);
        ReleaseDC(dc);       
    }
    m_background = back;
}

void EllipseButton::OnLButtonDown(UINT nFlags, const Point& point)
{
    D2D1_COLOR_F back = m_background;
    m_background = D2D1::ColorF(D2D1::ColorF::Blue);
    Rect rect;
    GetClientRect(rect);
    Point pt = rect.CenterPoint();
    auto elip = D2D1::Ellipse(D2D1::Point2F(convert_to<float>(pt.x), convert_to<float>(pt.y)), convert_to<float>(rect.Width() / 2), convert_to<float>(rect.Height() / 2));
    if(((std::pow(static_cast<float>(point.x) - elip.point.x,2))/std::pow(elip.radiusX,2) + std::pow((static_cast<float>(point.y) - elip.point.y),2)/std::pow(elip.radiusY,2) <= 1)) 
    {
        auto dc = GetDC();
        Render(dc, rect);
        ReleaseDC(dc);       
    }
    m_background = back;
}

void EllipseButton::OnLButtonUp(UINT nFlags, const Point& point)
{
    Rect rect;
    GetClientRect(rect);
    Point pt = rect.CenterPoint();
    auto elip = D2D1::Ellipse(D2D1::Point2F(convert_to<float>(pt.x), convert_to<float>(pt.y)), convert_to<float>(rect.Width() / 2), convert_to<float>(rect.Height() / 2));
    if(((std::pow(static_cast<float>(point.x) - elip.point.x,2))/std::pow(elip.radiusX,2) + std::pow((static_cast<float>(point.y) - elip.point.y),2)/std::pow(elip.radiusY,2) <= 1)) 
    {
        auto dc = GetDC();
        Render(dc, rect);
        ReleaseDC(dc);       
        ::SendMessage(GetParent(), WM_COMMAND, GetWindowLongPtr(GWL_ID), LPARAM(this->m_hwnd));
    }
}