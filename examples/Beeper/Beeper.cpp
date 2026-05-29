
#include "Beeper.h"
VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT_PTR iTimerID, DWORD dwTime)
{
	Beeper* pthis = reinterpret_cast<Beeper*>(iTimerID);
	pthis->OnTimer(ID_TIMER);
}

HRESULT Beeper::Render()
{
	HRESULT hr = S_OK;
	if (m_rt)
	{
	
		m_rt->BeginDraw();
		m_rt->SetTransform(D2D1::Matrix3x2F::Identity());
		if (iFlipFlop)
		{
			m_rt->Clear(m_color_red);
		}
		else
		{
			m_rt->Clear(m_color_blue);
		}
		hr = m_rt->EndDraw();
		if (hr == D2DERR_RECREATE_TARGET)
		{
			DestroyDeviceResources();
			hr = InitDeviceResources();
		}
	}
	return hr;
}

HRESULT Beeper::InitFactories()
{
	HRESULT hr = S_OK;
	if (!m_factory)
	{
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_factory.GetAddressOf());
	}
	return hr;
}

HRESULT Beeper::InitDeviceResources()
{
	auto dpi = GetDpiForWindow();
	HRESULT hr = S_OK;
	Rect rc;
	GetClientRect(&rc);
	if (!m_rt)
	{
		hr = m_factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(
												   D2D1_RENDER_TARGET_TYPE_DEFAULT,
												   D2D1::PixelFormat(), static_cast<float>(dpi), static_cast<float>(dpi)),
											   D2D1::HwndRenderTargetProperties(*this, D2D1::SizeU(rc.Width(), rc.Height())),
											   m_rt.GetAddressOf());
	}
	return hr;
}

HRESULT Beeper::Initialize()
{
	iFlipFlop = false;
	HRESULT hr = S_OK;
	hr = InitFactories();
	if (SUCCEEDED(hr))
	{
		// get app title from resource file.
		std::tstring apptitle;
		apptitle.resize(7);
		hr = LoadString(HINST_THISCOMPONENT, IDS_APP_TITLE, apptitle.data(), apptitle.length()) > 0 ? S_OK : E_FAIL;
		if (SUCCEEDED(hr))
		{
			hr = ::IsWindow(Create(nullptr, &Window::rcDefault, apptitle.c_str())) ? S_OK : E_FAIL;
			if (SUCCEEDED(hr))
			{
				SetTimer(reinterpret_cast<UINT_PTR>(this), 1000, TimerProc);
				ShowWindow(ShowWindowType::Normal);
				Invalidate(FALSE);
			}
		}
	}
	return hr;
}

void Beeper::DestroyDeviceResources()
{
	if (m_rt)
	{
		auto rt = m_rt.ReleaseAndGetAddressOf();
		rt = nullptr;
	}
}

void Beeper::OnSize(UINT nType, Size size)
{
	if (m_rt)
	{
		auto hr = m_rt->Resize(D2D1::SizeU(size.cx, size.cy));
		if (hr == D2DERR_RECREATE_TARGET)
		{
			DestroyDeviceResources();
			hr = InitDeviceResources();
		}
	}
}

void Beeper::OnTimer(UINT_PTR nIDEvent)
{
	MessageBeep(-1);
	iFlipFlop = !iFlipFlop;
	BeginPaint(&ps);
	auto hr = Render();
	EndPaint(&ps);
}

void Beeper::OnPaint(DC dc)
{
	BeginPaint(&ps);
	auto hr = Render();
	EndPaint(&ps);
	if (FAILED(hr))
	{
		OnClose();
	}
}

void Beeper::OnExit(UINT uNotifyCode, int nID, Window wndCtl)
{
	OnClose();
}

void Beeper::OnAbout(UINT uNotifyCode, int nID, Window wndCtl)
{

	if (!about)
	{
		about.DoModal();
	}
}

void Beeper::OnClose()
{
	KillTimer(ID_TIMER);
	PostQuitMessage(0);
}

int Beeper::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	HRESULT hr = InitDeviceResources();
	if (SUCCEEDED(hr))
	{
		Invalidate(FALSE);
	}
	// additional window initalization would go here.
	return 0;
}
