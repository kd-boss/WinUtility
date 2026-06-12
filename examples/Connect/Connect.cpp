#include "Connect.h"
#include <WinUtility/Numbers.h>
HRESULT Connect::CreatePathGeometry(ID2D1PathGeometry** out)
{
	HRESULT hr = S_OK;
	ID2D1PathGeometry* pPath;
	ID2D1GeometrySink* pSink;
	if(m_points.size() > 1)
	{
	hr = m_ft->CreatePathGeometry(&pPath);
	if(SUCCEEDED(hr))
	{
		pPath->Open(&pSink);
			pSink->BeginFigure(m_points[0], D2D1_FIGURE_BEGIN_HOLLOW);
			pSink->AddLines(&m_points[1], static_cast<UINT>(m_points.size() - 2));
			pSink->EndFigure(D2D1_FIGURE_END_OPEN);
			hr = pSink->Close();
			pSink->Release();
		
			if(SUCCEEDED(hr))
			{
				*out = pPath;
			}
		}
	} else return E_FAIL;	
	return hr;
}

HRESULT Connect::Render()
{
	ID2D1PathGeometry *pPath = nullptr;
	HRESULT hr = S_OK;
	m_rt->SetTransform(D2D1::Matrix3x2F::Identity());
	m_rt->BeginDraw();
	m_rt->Clear(m_backColor);
	hr = CreatePathGeometry(&pPath);
	if (SUCCEEDED(hr))
	{
		m_rt->DrawGeometry(pPath, m_brush.Get());
		pPath->Release();
		pPath = nullptr;
	}
	hr = m_rt->EndDraw();
	return hr;
}

HRESULT Connect::CreateFactories()
{
	HRESULT hr = S_OK;
	if(!m_ft)
	{
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,m_ft.GetAddressOf());
	}
	return hr;
}

HRESULT Connect::CreateRenderTargetResources()
{
	HRESULT hr = S_OK;
	if(!m_rt)
	{
		if(m_ft)
		{
			hr = m_ft->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
					D2D1::HwndRenderTargetProperties(
						*this,
						D2D1::SizeU(m_size.cx, m_size.cy)),
						m_rt.GetAddressOf()
					);
			if(SUCCEEDED(hr))
			{
				m_rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), m_brush.GetAddressOf());
			}
		}
		else
		{
			std::cerr << "Failed to create D2D1 Factory before trying to create Hwnd Render Target." << std::endl;
		}
	}
	return hr;
}

void Connect::DestoryRenderTargetResources()
{
	if(m_brush)
	{
		auto br = m_brush.ReleaseAndGetAddressOf();
		br = nullptr;
	}
	if(m_rt)
	{
		auto rt = m_rt.ReleaseAndGetAddressOf();
		rt = nullptr;
	}
}

HRESULT Connect::Initialize()
{
	HRESULT hr = S_OK;
	hr = CreateFactories();
	// get app title from resource file.
	if (SUCCEEDED(hr))
	{

		std::tstring apptitle;
		apptitle.resize(20);
		hr = LoadString(HINST_THISCOMPONENT, IDS_APP_TITLE, apptitle.data(), convert_to<int>(apptitle.length())) > 0 ? S_OK : E_FAIL;
		if (SUCCEEDED(hr))
		{
			hr = ::IsWindow(Create(nullptr, &Window::rcDefault, apptitle.c_str())) ? S_OK : E_FAIL;
			if (SUCCEEDED(hr))
			{
				ShowWindow(ShowWindowType::Normal);
				UpdateWindow();
			}
		}
		else
		{
			std::cerr << "Failed to load app title" << std::endl;
		}
	}

	return hr;
}

void Connect::OnExit(UINT uNotifyCode, int nID, Window wndCtl)
{
	OnDestroyed();
}

void Connect::OnAbout(UINT uNotifyCode, int nID, Window wndCtl)
{

	if(!about)
	{
		if(about.DoModal() == IDOK)
		{
			MessageBox::Show(TEXT("OK pressed!"), TEXT("WinTest"),MessageBoxButtons::Ok, MessageBoxIcon::Information);
		}
	}

}

int Connect::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//additional window initalization would go here.
	
    return 0;
}

void Connect::OnPaint(DC dc)
{
	HRESULT hr = S_OK;
	if (!m_rt)
	{
		hr = CreateRenderTargetResources();
	}
	if (SUCCEEDED(hr))
	{
		BeginPaint(&ps);
		hr = Render();
		EndPaint(&ps);
		if (hr == D2DERR_RECREATE_TARGET)
		{
			DestoryRenderTargetResources();
			CreateRenderTargetResources();
		}
	}
}

void Connect::OnLButtonDown(UINT nFlags, const Point &pt)
{
	m_points.emplace_back(static_cast<float>(pt.x),static_cast<float>(pt.y));
	Invalidate(FALSE);
}

void Connect::OnLButtonUp(UINT nFlags, const Point &pt)
{
	m_points.clear();
	Invalidate(FALSE);
}

void Connect::OnRButtonDown(UINT nFlags, const Point &pt)
{
}

void Connect::OnMouseMove(UINT nFlags, Point point)
{
	if(nFlags & MK_LBUTTON)
	{
		m_points.emplace_back(static_cast<float>(point.x),static_cast<float>(point.y));
	}
	Invalidate(FALSE);
}

void Connect::OnSize(UINT nFlags, Size size)
{
	m_size = size;
	if(m_rt)
	{
		m_rt->Resize(D2D1::SizeU(m_size.cx, m_size.cy));
	}
	Invalidate(FALSE);
}

void Connect::OnDestroyed()
{
	m_points.clear();
	DestoryRenderTargetResources();
	auto ft = m_ft.ReleaseAndGetAddressOf();
	ft = nullptr;
	PostQuitMessage(0);
}