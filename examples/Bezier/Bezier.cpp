

#include "Bezier.h"

HRESULT MyWindow::RecreateResources()
{
	auto brush = m_blbrush.ReleaseAndGetAddressOf();
	brush = nullptr;
	auto target = m_rt.ReleaseAndGetAddressOf();
	target = nullptr;
	return CreateDeviceResources();
}

HRESULT MyWindow::Render(const PAINTSTRUCT &ps)
{
	HRESULT hr = S_OK;
	if (!m_rt)
	{
		hr = CreateDeviceResources();
		if (FAILED(hr))
			return hr;
	}
	Rect rc;
	GetClientRect(&rc);
	m_rt->BindDC(ps.hdc, &rc);
	m_rt->SetTransform(D2D1::Matrix3x2F::Identity());
	m_rt->BeginDraw();
	m_rt->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
	if (m_path)
	{
		m_rt->DrawGeometry(m_path.Get(), m_blbrush.Get());		
		m_rt->DrawLine(apt[0], apt[1], m_blbrush.Get());
		m_rt->DrawLine(apt[2], apt[3], m_blbrush.Get());
	}
	hr = m_rt->EndDraw();
	if (hr == D2DERR_RECREATE_TARGET)
	{
		RecreateResources();
	}
	return hr;
}

HRESULT MyWindow::CreateDeviceIndependantResources()
{
	HRESULT hr = S_OK;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_factory.GetAddressOf());
	if (FAILED(hr))
		return hr;

	return hr;
}

HRESULT MyWindow::CreateDeviceResources()
{
	HRESULT hr = S_OK;
	if (m_factory)
	{
		D2D1_RENDER_TARGET_PROPERTIES properties = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(
				DXGI_FORMAT_B8G8R8A8_UNORM,
				D2D1_ALPHA_MODE_PREMULTIPLIED),
			0,
			0,
			D2D1_RENDER_TARGET_USAGE_NONE);

		hr = m_factory->CreateDCRenderTarget(&properties, m_rt.GetAddressOf());
		if (SUCCEEDED(hr))
		{
			hr = m_rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), m_blbrush.GetAddressOf());
		}
	}
	return hr;
}

HRESULT MyWindow::Initialize()
{

	HRESULT hr = S_OK;
	hr = CreateDeviceIndependantResources();
	if (FAILED(hr))
	{
		return hr;
	}
	// get app title from resource file.
	std::tstring apptitle;
	apptitle.resize(256);
	LoadString(HINST_THISCOMPONENT, IDS_APP_TITLE, apptitle.data(), apptitle.length());
	apptitle.shrink_to_fit();
	
	hr = ::IsWindow(Create(nullptr, &Window::rcDefault, apptitle.c_str())) ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{
		Rect rc;
		GetClientRect(&rc);
		Size size = rc.Size();
		apt[0].x = static_cast<float>(size.cx / 4);
		apt[0].y = static_cast<float>(size.cy / 2);

		apt[1].x = static_cast<float>(size.cx / 2);
		apt[1].y = static_cast<float>(size.cy / 4);

		apt[2].x = static_cast<float>(size.cx / 2);
		apt[2].y = static_cast<float>(3 * size.cy / 4);

		apt[3].x = static_cast<float>(3 * size.cx / 4);
		apt[3].y = static_cast<float>(size.cy / 2);
		CalculateBezier();
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
	if (!about)
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
	// additional window initalization would go here.
	CreateDeviceResources();
	return 0;
}

void MyWindow::OnPaint(DC dc)
{
	BeginPaint(&ps);
	Render(ps);
	EndPaint(&ps);
}

void MyWindow::OnSize(UINT nType, Size size)
{
	apt[0].x = static_cast<float>(size.cx / 4);
	apt[0].y = static_cast<float>(size.cy / 2);

	apt[1].x = static_cast<float>(size.cx / 2);
	apt[1].y = static_cast<float>(size.cy / 4);

	apt[2].x = static_cast<float>(size.cx / 2);
	apt[2].y = static_cast<float>(3 * size.cy / 4);

	apt[3].x = static_cast<float>(3 * size.cx / 4);
	apt[3].y = static_cast<float>(size.cy / 2);
	CalculateBezier();
}

void MyWindow::CalculateBezier()
{
	if (m_path)
	{
		auto path = m_path.ReleaseAndGetAddressOf();
		path = nullptr;
		auto sink = m_sink.ReleaseAndGetAddressOf();
		sink = nullptr;
		m_factory->CreatePathGeometry(m_path.GetAddressOf());
	}
	else
	{
		m_factory->CreatePathGeometry(m_path.GetAddressOf());
	}

	m_path->Open(m_sink.GetAddressOf());

	m_sink->BeginFigure(apt[0], D2D1_FIGURE_BEGIN_HOLLOW);
	m_sink->AddBezier(D2D1::BezierSegment(apt[1], apt[2], apt[3]));
	m_sink->EndFigure(D2D1_FIGURE_END_OPEN);
	m_sink->Close();
	Invalidate(FALSE);
}

void MyWindow::OnMouseMove(UINT nFlags, Point point)
{

	Rect rc;
	GetClientRect(&rc);
	auto dc = GetDC();
	m_rt->BindDC(dc, &rc);
	if (nFlags & MK_LBUTTON or nFlags & MK_RBUTTON or nFlags & MK_SHIFT or nFlags & MK_CONTROL)
	{
		if (m_path)
		{
			auto color = m_blbrush->GetColor();
			m_blbrush->SetColor(D2D1::ColorF(D2D1::ColorF::SkyBlue));

			m_rt->SetTransform(D2D1::Matrix3x2F::Identity());
			m_rt->BeginDraw();
			m_rt->DrawGeometry(m_path.Get(), m_blbrush.Get(), 4.0f);
			m_rt->DrawLine(apt[0], apt[1], m_blbrush.Get(), 4.0f);
			m_rt->DrawLine(apt[2], apt[3], m_blbrush.Get(), 4.0f);
			if (m_rt->EndDraw() == D2DERR_RECREATE_TARGET)
			{
				RecreateResources();
			}
			m_blbrush->SetColor(color);
		}
	}

	if (nFlags & MK_LBUTTON)
	{
		apt[1].x = static_cast<float>(point.x);
		apt[1].y = static_cast<float>(point.y);
	}
	else if (nFlags & MK_RBUTTON)
	{
		apt[2].x = static_cast<float>(point.x);
		apt[2].y = static_cast<float>(point.y);
	}
	else if (nFlags & MK_CONTROL)
	{
		apt[0].x = static_cast<float>(point.x);
		apt[0].y = static_cast<float>(point.y);
	}
	else if (nFlags & MK_SHIFT)
	{
		apt[3].x = static_cast<float>(point.x);
		apt[3].y = static_cast<float>(point.y);
	}

	CalculateBezier();
	m_rt->BeginDraw();
	m_rt->DrawGeometry(m_path.Get(), m_blbrush.Get());
	m_rt->DrawLine(apt[0], apt[1], m_blbrush.Get());
	m_rt->DrawLine(apt[2], apt[3], m_blbrush.Get());
	if (m_rt->EndDraw() == D2DERR_RECREATE_TARGET)
	{
		RecreateResources();
	}
	ReleaseDC(dc);
}