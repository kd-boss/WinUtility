#include "Checker1.h"
#include <format>

HRESULT Checker1::Initialize()
{
	for(auto &a : m_checkers)
	{
		a = std::make_pair<D2D1_RECT_F,bool>(D2D1_RECT_F(),false);
	}
	HRESULT hr = S_OK;
	hr = CreateFactories();
	// get app title from resource file.
	if (SUCCEEDED(hr))
	{
		std::tstring apptitle;
		apptitle.resize(8);
		LoadString(HINST_THISCOMPONENT, IDS_APP_TITLE, apptitle.data(), apptitle.length());

		hr = ::IsWindow(Create(nullptr, &Window::rcDefault, apptitle.c_str())) ? S_OK : E_FAIL;
		if (SUCCEEDED(hr))
		{
			ShowWindow(ShowWindowType::Normal);
			Invalidate(FALSE);
		}
	}
	return hr;
}

HRESULT Checker1::Render()
{
	HRESULT hr = S_OK;
	if (!m_rt)
	{
		hr = CreateRenderTargetResources();
	}
	if (m_rt)
	{
		auto sz = m_rt->GetSize();
		auto rectWidth = sz.width / 5.0f;
		auto rectHeight = sz.height / 5.0f;
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				m_checkers[j + (i * 5)].first = {
					j * rectWidth,
					i * rectHeight,
					rectWidth + (j * rectWidth),
					rectHeight + (i * rectHeight)};
			}
		}
		m_rt->SetTransform(D2D1::Matrix3x2F::Identity());
		m_rt->BeginDraw();
		m_rt->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));

		for (auto &a : m_checkers)
		{
			m_rt->DrawRectangle(a.first, m_br.Get(), 1.0);
			if (a.second)
			{
				m_rt->DrawLine(D2D1::Point2F(a.first.left, a.first.top), D2D1::Point2F(a.first.right, a.first.bottom), m_br.Get());
				m_rt->DrawLine(D2D1::Point2F(a.first.left, a.first.bottom), D2D1::Point2F(a.first.right, a.first.top), m_br.Get());
			}
		}

		hr = m_rt->EndDraw();

		if (hr == D2DERR_RECREATE_TARGET)
		{

			DestroyRenderTargetResources();
			hr = CreateRenderTargetResources();
		}
	}
	return hr;
}

HRESULT Checker1::CreateFactories()
{
	HRESULT hr = S_OK;
	if(!m_ft)
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_ft.GetAddressOf());

	return hr;
}

HRESULT Checker1::CreateRenderTargetResources()
{
	HRESULT hr = S_OK;
	if (m_ft)
	{
		if (!m_rt)
		{
			try
			{
				Rect rc;
				GetClientRect(&rc);
				D2D1_HWND_RENDER_TARGET_PROPERTIES hwndproperties = D2D1::HwndRenderTargetProperties(
					this->m_hwnd,
					D2D1::SizeU(rc.Width(), rc.Height()));
				D2D1_RENDER_TARGET_PROPERTIES properties = D2D1::RenderTargetProperties();
				hr = m_ft->CreateHwndRenderTarget(
					&properties,
					&hwndproperties,
					m_rt.GetAddressOf());
			}
			catch (...)
			{
				std::cout << "failed to create render target due to memory access." << std::endl;
			}
		}
		if (!m_br and SUCCEEDED(hr))
		{
			hr = m_rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), m_br.GetAddressOf());
		}
	}
	else
	{
		return E_FAIL;
	}
	return hr;
}

void Checker1::DestroyRenderTargetResources()
{
	if(m_br)
	{
		auto br = m_br.ReleaseAndGetAddressOf();
		br = nullptr;
	}
	if(m_rt)
	{
		auto rt = m_rt.ReleaseAndGetAddressOf();
		rt = nullptr;
	}
}

void Checker1::OnPaint(DC dc)
{
	BeginPaint(&ps);
	auto hr = Render();
	EndPaint(&ps);
	if(SUCCEEDED(hr))
	{}else{
		std::cout << "Error painting!" << std::endl;
	}
		//yay!
}

void Checker1::OnExit(UINT uNotifyCode, int nID, Window wndCtl)
{
	DestroyWindow();
}

void Checker1::OnAbout(UINT uNotifyCode, int nID, Window wndCtl)
{
	if (!about)
	{
		about.DoModal();
	}
}

void Checker1::OnClose()
{
	DestroyRenderTargetResources();
	if(m_ft){
		m_ft->Release();
		m_ft = nullptr;
	}
	PostQuitMessage(0);
}

int Checker1::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// additional window initalization would go here.
	return 0;
}

void Checker1::OnLButtonDown(UINT nFlags, const Point &pt)
{
	auto x = pt.x; 
	auto y = pt.y;

	for(auto &a : m_checkers)
	{
		bool hittest = x <= a.first.right and x >= a.first.left and y <= a.first.bottom and y >= a.first.top;
		if(hittest)
			a.second = !a.second;
	}
	Invalidate(FALSE);
}

void Checker1::OnClear(UINT uNotifyCode, int nID, Window wndCtl)
{
	for(auto &a : m_checkers)
	{
		a.second = false;
	}
	Invalidate(FALSE);
}

void Checker1::OnSize(UINT nFlags, Size size)
{
	m_size = size;
	auto rectWidth = m_size.cx / 5.0f;
	auto rectHeight = m_size.cy / 5.0f;
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			m_checkers[j + (i * 5)].first = {
				j * rectWidth,
				i * rectHeight,
				rectWidth + (j * rectWidth),
				rectHeight + (i * rectHeight)
			};
		}
	}
	if(m_rt) 
	{
		m_rt->Resize(D2D1::SizeU(m_size.cx, m_size.cy));
	}

}

void Checker1::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	Point pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
    auto divisioncx = m_checkers[0].first.right - m_checkers[0].first.left;
	auto divisioncy = m_checkers[0].first.bottom - m_checkers[0].first.top;
	auto x = static_cast<int>(std::max(0.f, std::min(4.f, pt.x / divisioncx)));
	auto y = static_cast<int>(std::max(0.f, std::min(4.f, pt.y / divisioncy)));

    //if(!(x = divisioncx/2 and ((y = divisioncy/2) or (y = 4 * divisioncy +  divisioncy/2))))
	switch(nChar)
	{
		case VK_UP:
		y--;
		break;
		case VK_DOWN:
		y++;
		break;
		case VK_RIGHT:
		x++;
		break;
		case VK_LEFT:
		x--;
		break;
		case VK_HOME:
		x = y = 0;
		break;
		case VK_END:
		x = y = 4;
		break;
		case VK_RETURN:
		case VK_SPACE:
		{
			SendMessage(WM_LBUTTONDBLCLK, MK_LBUTTON, 
				MAKELONG(static_cast<int>(x * divisioncx + divisioncx/2.f),
						 static_cast<int>(y * divisioncy + divisioncy/2.f))
					);
		}
		break;
	};
	pt.x = static_cast<int>(x * divisioncx + divisioncx/2.f);
	pt.y = static_cast<int>(y * divisioncy + divisioncy/2.f);
	ClientToScreen(&pt);
	SetCursorPos(pt.x, pt.y);
}