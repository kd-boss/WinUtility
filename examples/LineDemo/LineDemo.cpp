
#include "LineDemo.h"


HRESULT MyWindow::Render(const PAINTSTRUCT& ps)
{
	HRESULT hr = S_OK;
	if(!m_renderTarget)
	{
		hr = CreateDeviceResources();
	}
	Rect rc;
	GetClientRect(&rc);
	hr = m_renderTarget->BindDC(ps.hdc,&rc);
	if(SUCCEEDED(hr))
	{

		m_renderTarget->BeginDraw();
		m_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		m_renderTarget->DrawRectangle(D2D1::RectF(static_cast<float>(rc.Width() / 8),
												  static_cast<float>(rc.Height() / 8),
												  static_cast<float>(7 * (rc.Width() / 8)),
												  static_cast<float>(7 * (rc.Height() / 8))),
												  m_blackBrush.Get(), stroke);
				
	    m_renderTarget->DrawLine(D2D1::Point2F(0.0f,0.0f),
								 D2D1::Point2F(static_cast<float>(rc.Width()),
								 static_cast<float>(rc.Height())),
								m_blackBrush.Get(),stroke);

		m_renderTarget->DrawLine(D2D1::Point2F(static_cast<float>(rc.Width()), 0.0f),
								 D2D1::Point2F(0.0f,static_cast<float>(rc.Height())),
								 m_blackBrush.Get(),stroke);
	    auto elipse = D2D1::Ellipse(
									D2D1::Point2F(rc.Width() / 2, rc.Height() / 2),
									static_cast<float>(3 * (rc.Width() / 8)),
									static_cast<float>(3 * (rc.Height() / 8)));
		m_renderTarget->DrawEllipse(elipse, 
									m_blackBrush.Get(),stroke + 4.0f);

	    m_renderTarget->FillEllipse(elipse, m_whiteBrush.Get());

		m_renderTarget->DrawRoundedRectangle(D2D1::RoundedRect(
											 D2D1::RectF(
												static_cast<float>(rc.Width() / 4),
												static_cast<float>(rc.Height() / 4),
												static_cast<float>(3 * rc.Width() / 4),
												static_cast<float>(3 * rc.Height() / 4)
											 			), 
												static_cast<float>(rc.Width() / 12),
											 	static_cast<float>(rc.Height() /12)
															  ),
												m_blackBrush.Get(),
												stroke
											);
		hr = m_renderTarget->EndDraw();
		if(hr == D2DERR_RECREATE_TARGET)
		{
			auto target = m_renderTarget.ReleaseAndGetAddressOf();
			target = nullptr;
			auto bb = m_blackBrush.ReleaseAndGetAddressOf();
			bb = nullptr;
			hr = CreateDeviceResources();
		}									
	}

	return hr;
}


HRESULT MyWindow::CreateDeviceIndependantResources()
{
	HRESULT hr = S_OK;
	if(!m_factory)
	{
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_factory.GetAddressOf());
	}
	return hr;
}

HRESULT MyWindow::CreateDeviceResources()
{
	HRESULT hr = S_OK;
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
                        D2D1_FEATURE_LEVEL_DEFAULT);
		
	    hr = m_factory->CreateDCRenderTarget(&properties, m_renderTarget.GetAddressOf());
	    if(SUCCEEDED(hr))
		{
			hr = m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), m_blackBrush.GetAddressOf());
			if(SUCCEEDED(hr))
			{
				hr = m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), m_whiteBrush.GetAddressOf());
			}
		}
	}
	return hr;
}

void MyWindow::OnPaint(DC dc)
{
	PAINTSTRUCT ps;
	BeginPaint(&ps);
	Render(ps);
	EndPaint(&ps);
}

HRESULT MyWindow::Initialize()
{
	HRESULT hr = S_OK;
	//get app title from resource file.
	std::tstring apptitle;
    apptitle.resize(256);
    LoadString(HINST_THISCOMPONENT,IDS_APP_TITLE,apptitle.data(),apptitle.length());
    apptitle.shrink_to_fit();

	hr = ::IsWindow(Create(nullptr, &Window::rcDefault, apptitle.c_str())) ? S_OK : E_FAIL;
	if(SUCCEEDED(hr))
	{ 
    	ShowWindow(ShowWindowType::Normal);
    	UpdateWindow();
	}
	return hr;
}

void MyWindow::OnExit(UINT uNotifyCode, int nID, Window wndCtl)
{
	OnClose();
}

void MyWindow::OnAbout(UINT uNotifyCode, int nID, Window wndCtl)
{

	if(!about)
	{
		about.DoModal();		
	}

}

void MyWindow::OnClose()
{
    PostQuitMessage(0);
}

int MyWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//additional window initalization would go here.
	CreateDeviceIndependantResources();
    return 0;
}

void MyWindow::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
		case VK_OEM_PLUS:
			stroke += 0.1f;
		break;
		case VK_OEM_MINUS:
			stroke -= 0.1f;
		break;
	}
	Invalidate();
	UpdateWindow();
}
