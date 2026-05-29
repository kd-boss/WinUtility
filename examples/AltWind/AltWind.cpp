

#include "AltWind.h"

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
	if(!m_rt)
	{
		D2D1_RENDER_TARGET_PROPERTIES properties = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(
				DXGI_FORMAT_B8G8R8A8_UNORM,
				D2D1_ALPHA_MODE_PREMULTIPLIED
			),
			0,
			0
		);
		hr = m_factory->CreateDCRenderTarget(&properties, m_rt.GetAddressOf());
		if(SUCCEEDED(hr))
		{
			hr = m_rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black),m_brush.GetAddressOf());
			if(SUCCEEDED(hr))
			{
				hr = m_rt->CreateSolidColorBrush(fillColor, m_fbrush.GetAddressOf());
			}
		}
	}	
	return hr;
}

HRESULT MyWindow::Render(const PAINTSTRUCT& ps)
{
	HRESULT hr = S_OK;
	if(!m_factory)
	{
		hr = CreateDeviceIndependantResources();
	}
	if(!m_rt)
	{
		hr = CreateDeviceResources();
	}
	if(!m_path)
	{
		CalculateGeometry();
	}
	Rect rc;
	GetClientRect(&rc);
	m_rt->BindDC(ps.hdc, &rc);
	m_rt->SetTransform(D2D1::Matrix3x2F::Identity());
	m_rt->BeginDraw();
	m_rt->Clear(background);
	m_rt->DrawGeometry(m_path.Get(), m_brush.Get());
	m_rt->DrawGeometry(m_path2.Get(), m_brush.Get());
	//auto color = m_brush->GetColor();
	//m_brush->SetColor(fillColor);
	m_rt->FillGeometry(m_path.Get(), m_fbrush.Get());
	m_rt->FillGeometry(m_path2.Get(), m_fbrush.Get()); 
	//m_brush->SetColor(color);
	hr = m_rt->EndDraw();
	if(hr == D2DERR_RECREATE_TARGET)
	{
		auto fbrush = m_fbrush.ReleaseAndGetAddressOf();
		fbrush = nullptr;
		auto brush = m_brush.ReleaseAndGetAddressOf();
		brush = nullptr;
		auto target = m_rt.ReleaseAndGetAddressOf();
		target = nullptr;
		hr = CreateDeviceResources();
	}
	return hr;
}

HRESULT MyWindow::Initialize()
{
	HRESULT hr = S_OK;
	hr = CreateDeviceIndependantResources();
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

    return 0;
}

void MyWindow::OnSize(UINT nType, Size size)
{
	Rect rc;
	GetClientRect(&rc);
	HDC dc = GetDC();
	if(!m_rt)
	{
		CreateDeviceResources();
	}
	m_rt->BindDC(dc, &rc);
	if(m_path)
	{
		auto color = m_brush->GetColor();
		m_brush->SetColor(background);
	
		m_rt->SetTransform(D2D1::Matrix3x2F::Identity());
		m_rt->BeginDraw();
		m_rt->DrawGeometry(m_path.Get(),m_brush.Get(), 5.0f);
		m_rt->FillGeometry(m_path.Get(),m_brush.Get());
		m_rt->DrawGeometry(m_path2.Get(), m_brush.Get(), 5.0f);
		m_rt->FillGeometry(m_path2.Get(), m_brush.Get());
		m_rt->EndDraw();
		m_brush->SetColor(color);
	}
	CalculateGeometry();
	m_rt->SetTransform(D2D1::Matrix3x2F::Identity());
	m_rt->BeginDraw();
	m_rt->DrawGeometry(m_path.Get(),m_brush.Get());
	m_rt->DrawGeometry(m_path2.Get(), m_brush.Get());
	
	m_rt->FillGeometry(m_path.Get(),m_fbrush.Get());
	m_rt->FillGeometry(m_path2.Get(),m_fbrush.Get());
	
	HRESULT hr = m_rt->EndDraw();
	if(hr == D2DERR_RECREATE_TARGET)
	{
		auto fbrush = m_fbrush.ReleaseAndGetAddressOf();
		fbrush = nullptr;
		auto brush = m_brush.ReleaseAndGetAddressOf();
		brush = nullptr;
		auto target = m_rt.ReleaseAndGetAddressOf();
		target = nullptr;
		hr = CreateDeviceResources();
	}
	ReleaseDC(dc);
}

void MyWindow::CalculateGeometry()
{
	Rect rc;
	GetClientRect(&rc);
	Size size = rc.Size();
	for(int i = 0 ; i < 10; i++)
	{
		apt[i].x = static_cast<float>(size.cx) * aptFigure[i].x / 200.0f;
		apt[i].y = static_cast<float>(size.cy) * aptFigure[i].y / 100.0f;
	}

	if(m_path)
	{
		auto path = m_path.ReleaseAndGetAddressOf();
		path = nullptr;
		auto path2 = m_path2.ReleaseAndGetAddressOf();
		path2 = nullptr;
		auto sink = m_sink.ReleaseAndGetAddressOf();
		sink = nullptr;
	}

	if(!m_path)
	{
		HRESULT hr = m_factory->CreatePathGeometry(m_path.GetAddressOf());
		hr = m_factory->CreatePathGeometry(m_path2.GetAddressOf());
		if(SUCCEEDED(hr))
		{
			hr = m_path->Open(m_sink.GetAddressOf());
			m_sink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);
			m_sink->BeginFigure(apt[0],D2D1_FIGURE_BEGIN_FILLED);
			m_sink->AddLines(&apt[1],9);
			m_sink->EndFigure(D2D1_FIGURE_END_CLOSED);
			hr = m_sink->Close();
			if(SUCCEEDED(hr))
			{
				m_path2->Open(m_sink.GetAddressOf());
				for(int i = 0; i < 10; i++)
				{
					apt[i].x += static_cast<float>(size.cx) / 2;
				}
				m_sink->SetFillMode(D2D1_FILL_MODE_WINDING);
				m_sink->BeginFigure(apt[0], D2D1_FIGURE_BEGIN_FILLED);
				m_sink->AddLines(&apt[1],9);
				m_sink->EndFigure(D2D1_FIGURE_END_CLOSED);
				hr = m_sink->Close();
				if(SUCCEEDED(hr))
				{
					Invalidate(FALSE);
					UpdateWindow();
				}
			}
		}
	}
}

void MyWindow::OnPaint(DC dc)
{
	BeginPaint(&ps);
	Render(ps);
	EndPaint(&ps);
}

void MyWindow::OnLButtonDown(UINT nFlags, const Point &pt)
{
    
}
