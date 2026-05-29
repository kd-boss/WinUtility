
#include "Clover.h"
#include <cmath>
#include <numbers>

HRESULT MyWindow::CombineGeoms(
	ID2D1Geometry *pA,
	ID2D1Geometry *pB,
	D2D1_COMBINE_MODE mode,
	ID2D1PathGeometry **ppOut)
{
	*ppOut = nullptr;

	ID2D1PathGeometry *pPath = nullptr;
	HRESULT hr = m_ft->CreatePathGeometry(&pPath);

	ID2D1GeometrySink *pSink = nullptr;
	if (SUCCEEDED(hr))
		hr = pPath->Open(&pSink);

	if (SUCCEEDED(hr))
	{
		pSink->SetFillMode(D2D1_FILL_MODE_WINDING);
		hr = pA->CombineWithGeometry(pB, mode, nullptr, 0.25f, pSink);
	}

	if (pSink)
	{
		pSink->Close(); // must Close() even on failure
		pSink->Release();
	}

	if (SUCCEEDED(hr))
		*ppOut = pPath;
	else if (pPath)
		pPath->Release();

	return hr;
}

HRESULT MyWindow::RebuildCloverGeometry()
{
	GetClientRect(&rc);
	auto size = rc.Size();
	if (m_cloverGeo)
	{
		auto g_pCloverGeom = m_cloverGeo.ReleaseAndGetAddressOf();
		g_pCloverGeom = nullptr;
	}

	if (size.cx == 0 || size.cy == 0)
		return S_OK;

	const float cx = static_cast<float>(size.cx);
	const float cy = static_cast<float>(size.cy);

	
	ID2D1EllipseGeometry *e[4] = {};

	HRESULT hr = m_ft->CreateEllipseGeometry( // left petal
		D2D1::Ellipse(D2D1::Point2F(cx / 4.f, cy / 2.f), cx / 4.f, cy / 6.f), &e[0]);
	if (SUCCEEDED(hr))
		hr = m_ft->CreateEllipseGeometry( // right petal
			D2D1::Ellipse(D2D1::Point2F(3.f * cx / 4.f, cy / 2.f), cx / 4.f, cy / 6.f), &e[1]);
	if (SUCCEEDED(hr))
		hr = m_ft->CreateEllipseGeometry( // top petal
			D2D1::Ellipse(D2D1::Point2F(cx / 2.f, cy / 4.f), cx / 6.f, cy / 4.f), &e[2]);
	if (SUCCEEDED(hr))
		hr = m_ft->CreateEllipseGeometry( // bottom petal
			D2D1::Ellipse(D2D1::Point2F(cx / 2.f, 3.f * cy / 4.f), cx / 6.f, cy / 4.f), &e[3]);

	
	ID2D1PathGeometry *pLR = nullptr; // left  OR right
	ID2D1PathGeometry *pTB = nullptr; // top   OR bottom
	if (SUCCEEDED(hr))
		hr = CombineGeoms(e[0], e[1], D2D1_COMBINE_MODE_UNION, &pLR);
	if (SUCCEEDED(hr))
		hr = CombineGeoms(e[2], e[3], D2D1_COMBINE_MODE_UNION, &pTB);

	
	if (SUCCEEDED(hr))
		hr = CombineGeoms(pLR, pTB, D2D1_COMBINE_MODE_XOR, m_cloverGeo.GetAddressOf());

	
	for (int i = 0; i < 4; ++i)
		if (e[i])
			e[i]->Release();
	if (pLR)
		pLR->Release();
	if (pTB)
		pTB->Release();

	return hr;
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
	if (!m_cloverGeo)
	{
		RebuildCloverGeometry();
	}

	GetClientRect(&rc);
	m_rt->BindDC(ps.hdc, &rc);
	auto size = rc.Size();
	auto radius = std::hypotf(size.cx / 2.0, size.cy / 2.0);
	auto center = D2D1::Point2F(size.cx / 2.0f, size.cy / 2.0f);
	
	auto twopi = (2.f * std::numbers::pi);
	m_rt->BeginDraw();
	D2D1_LAYER_PARAMETERS params = D2D1::LayerParameters(
		D2D1::RectF(static_cast<float>(rc.left), static_cast<float>(rc.top), static_cast<float>(rc.right), static_cast<float>(rc.bottom)),
		m_cloverGeo.Get(), D2D1_ANTIALIAS_MODE_PER_PRIMITIVE,
		D2D1::Matrix3x2F::Identity());

	m_rt->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
	m_rt->PushLayer(&params, m_clip.Get());
	m_rt->Clear(D2D1::ColorF(D2D1::ColorF::LawnGreen));

	m_rt->SetTransform(D2D1::Matrix3x2F::Translation(center.x, center.y));
	for (float angle = 0.f; angle < twopi; angle += twopi / 360.f)
	{
		m_rt->DrawLine(D2D1::Point2F(0.0f, 0.0f),
					   D2D1::Point2F(radius * std::cos(angle) + 0.5f, radius * sin(angle) + 0.5f),
					   m_brush.Get());
	}
	m_rt->PopLayer();
	m_rt->SetTransform(D2D1::Matrix3x2F::Identity());
	hr = m_rt->EndDraw();
	if (hr == D2DERR_RECREATE_TARGET)
	{
		DestroyDeviceResources();
		return CreateDeviceResources();
	}
	return hr;
}

HRESULT MyWindow::Initialize()
{
	HRESULT hr = S_OK;
	hr = CreateDeviceIndependantResources();
	if (FAILED(hr))
		return hr;
	// get app title from resource file.
	std::tstring apptitle;
	apptitle.resize(256);
	LoadString(HINST_THISCOMPONENT, IDS_APP_TITLE, apptitle.data(), apptitle.length());
	apptitle.shrink_to_fit();

	hr = ::IsWindow(Create(nullptr, &Window::rcDefault, apptitle.c_str())) ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{
		ShowWindow(ShowWindowType::Normal);
		UpdateWindow();
	}
	return hr;
}

HRESULT MyWindow::CreateDeviceIndependantResources()
{
	HRESULT hr = S_OK;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_ft.GetAddressOf());
	return hr;
}

HRESULT MyWindow::CreateDeviceResources()
{
	HRESULT hr = S_OK;
	if (!m_rt)
	{
		D2D1_RENDER_TARGET_PROPERTIES properties = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(
				DXGI_FORMAT_B8G8R8A8_UNORM,
				D2D1_ALPHA_MODE_PREMULTIPLIED),
			0,
			0);
		hr = m_ft->CreateDCRenderTarget(&properties, m_rt.GetAddressOf());
		if (SUCCEEDED(hr))
		{
			hr = m_rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), m_brush.GetAddressOf());
			if (SUCCEEDED(hr))
			{
				hr = m_rt->CreateLayer(m_clip.GetAddressOf());
			}
		}
	}
	return hr;
}

void MyWindow::DestroyDeviceResources()
{
	if (m_rt)
	{
		ID2D1SolidColorBrush **brush = m_brush.ReleaseAndGetAddressOf();
		while ((*brush)->Release() > 0)
		{
		}
		brush = nullptr;
		ID2D1DCRenderTarget **target = m_rt.ReleaseAndGetAddressOf();
		while ((*target)->Release() > 0)
		{
		}
		target = nullptr;
		ID2D1Layer **layer = m_clip.ReleaseAndGetAddressOf();
		while ((*layer)->Release() > 0)
		{
		}
		layer = nullptr;
	}
}

void MyWindow::OnPaint(DC dc)
{
	BeginPaint(&ps);
	Render(ps);
	EndPaint(&ps);
}

void MyWindow::OnSize(UINT nType, Size size)
{
	GetClientRect(&rc);
	auto ptr = m_cloverGeo.ReleaseAndGetAddressOf();
	ptr = nullptr;
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
	return 0;
}
