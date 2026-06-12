
#include "ownerdraw.h"
#include <WinUtility/Numbers.h>

HRESULT MyWindow::CreateDeviceResources()
{
	HRESULT hr = S_OK;
	if(!m_rt)
	{
		auto props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
		convert_to<float>(GetDpiForWindow()),convert_to<float>(GetDpiForWindow()));
		hr = m_factory->CreateDCRenderTarget(&props, m_rt.GetAddressOf());
	}
	if(m_rt)
	{
		hr = m_rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Turquoise),m_brush.GetAddressOf());
	}
	return hr;
}

HRESULT MyWindow::CreateDeviceIndependantResources()
{
	HRESULT hr = S_OK;
	if(!m_factory)
	{
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,m_factory.GetAddressOf());
	}
	if(!m_stroke){
	

	auto properties = D2D1::StrokeStyleProperties(D2D1_CAP_STYLE_FLAT,
								 D2D1_CAP_STYLE_FLAT,
								 D2D1_CAP_STYLE_FLAT,
								 D2D1_LINE_JOIN_MITER,
								 10.0f,
								 D2D1_DASH_STYLE_CUSTOM,
								0.0f);
		
	float dashes[] = {1.f,2.f,1.f,2.f,1.f,2.f};
	hr = m_factory->CreateStrokeStyle(properties,dashes,ARRAYSIZE(dashes), m_stroke.GetAddressOf());
	}
	return hr;
}

void MyWindow::DestroyDeviceResources()
{
	if(m_rt)
	{
		auto rt = m_rt.ReleaseAndGetAddressOf();
		rt = nullptr;
		auto br = m_brush.ReleaseAndGetAddressOf();
		br = nullptr;
	}	
}

HRESULT MyWindow::Initialize()
{
	HRESULT hr = S_OK;
	//get app title from resource file.
	std::tstring apptitle;
    apptitle.resize(256);
    LoadString(HINST_THISCOMPONENT,IDS_APP_TITLE,apptitle.data(),convert_to<int>(apptitle.length()));
    apptitle.shrink_to_fit();

	hr = ::IsWindow(Create(nullptr, &Window::rcDefault, apptitle.c_str())) ? S_OK : E_FAIL;
	if(SUCCEEDED(hr))
	{ 
    	ShowWindow(ShowWindowType::Normal);
    	UpdateWindow();
	}
	return hr;
}


void MyWindow::OnSmaller(UINT uNotifyCode, int nID, Window wndCtl)
{
	Rect rc;
	GetWindowRect(&rc);
	rc.left += rc.Width() / 20;
	rc.right -= rc.Width() / 20;
	rc.top += rc.Height() / 20;
	rc.bottom -= rc.Height() / 20;

	MoveWindow(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, true);
}

void MyWindow::OnLarger(UINT uNotifyCode, int nID, Window wndCtl)
{
	Rect rc;
	GetWindowRect(&rc);
	rc.left -= rc.Width() / 20;
	rc.right += rc.Width() / 20;
	rc.top -= rc.Height() / 20;
	rc.bottom += rc.Height() / 20;

	MoveWindow(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, true);
}

ID2D1PathGeometry* MyWindow::DrawTriangle(D2D1_POINT_2F a,D2D1_POINT_2F b,D2D1_POINT_2F c)
{
	ID2D1PathGeometry* ret;
	ID2D1GeometrySink* sink;
	auto hr = m_factory->CreatePathGeometry(&ret);
	if(SUCCEEDED(hr))
	{
		if(SUCCEEDED(ret->Open(&sink)))
		{
			sink->BeginFigure(a,D2D1_FIGURE_BEGIN_FILLED);
			sink->AddLine(b);
			sink->AddLine(c);
			sink->EndFigure(D2D1_FIGURE_END_CLOSED);
			if(SUCCEEDED(sink->Close()))
			{
				sink->Release();
				return ret;
			}
			else
			{
				sink->Release();
				return nullptr;
			}
		} 
		else 
		{
			return nullptr;
		}
	}
	else
	{
		return nullptr;
	}
}

void MyWindow::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	if(!m_rt)
	{
		CreateDeviceResources();
	}
	D2D1_COLOR_F current = m_brush->GetColor();
	m_rt->BindDC(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem);
	m_rt->BeginDraw();
	if(lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		m_rt->Clear(current);
		m_brush->SetColor(D2D1::ColorF(D2D1::ColorF::White));		
	}
	else
	{
		m_rt->Clear(D2D1::ColorF(D2D1::ColorF::White));
	}
	RECT rc = lpDrawItemStruct->rcItem;
	m_rt->DrawRectangle(D2D1::RectF(static_cast<float>(rc.left),
						static_cast<float>(rc.top),
						static_cast<float>(rc.right),
						static_cast<float>(rc.bottom)), m_brush.Get());
	
	auto cx = rc.right - rc.left;
	auto cy = rc.bottom - rc.top;
	auto cx8 = cx / 8.f;
	auto cy8 = cy / 8.f;
	switch(static_cast<ButtonID>(nIDCtl))
	{
		case ButtonID::ID_SMALLER:
		{
			auto point1 = D2D1::Point2F(3.f * cx8, 1.f * cy8);
			auto point2 = D2D1::Point2F(5.f * cx8, 1.f * cy8);
			auto point3 = D2D1::Point2F(4.f * cx8, 3.f * cy8);
			auto triangle1 = DrawTriangle(point1, point2, point3);
			m_rt->DrawGeometry(triangle1, m_brush.Get());
			m_rt->FillGeometry(triangle1, m_brush.Get());
			triangle1->Release();

			point1 = D2D1::Point2F(7.f * cx8, 3.f * cy8);
			point2 = D2D1::Point2F(7.f * cx8, 5.f * cy8);
			point3 = D2D1::Point2F(5.f * cx8, 4.f * cy8);
			triangle1 = DrawTriangle(point1, point2, point3);
			m_rt->DrawGeometry(triangle1, m_brush.Get());
			m_rt->FillGeometry(triangle1, m_brush.Get());
			triangle1->Release();

			
			point1 = D2D1::Point2F(5.f * cx8, 7.f * cy8);
			point2 = D2D1::Point2F(3.f * cx8, 7.f * cy8);
			point3 = D2D1::Point2F(4.f * cx8, 5.f * cy8);
			triangle1 = DrawTriangle(point1, point2, point3);
			m_rt->DrawGeometry(triangle1, m_brush.Get());
			m_rt->FillGeometry(triangle1, m_brush.Get());
			triangle1->Release();

			
			point1 = D2D1::Point2F(1.f * cx8, 5.f * cy8);
			point2 = D2D1::Point2F(1.f * cx8, 3.f * cy8);
			point3 = D2D1::Point2F(3.f * cx8, 4.f * cy8);
			triangle1 = DrawTriangle(point1, point2, point3);
			m_rt->DrawGeometry(triangle1, m_brush.Get());
			m_rt->FillGeometry(triangle1, m_brush.Get());
			triangle1->Release();

		}
		break;
		case ButtonID::ID_LARGER:
		{
			auto point1 = D2D1::Point2F(5.f * cx8, 3.f * cy8);
			auto point2 = D2D1::Point2F(3.f * cx8, 3.f * cy8);
			auto point3 = D2D1::Point2F(4.f * cx8, 1.f * cy8);
			auto triangle1 = DrawTriangle(point1, point2, point3);
			m_rt->DrawGeometry(triangle1, m_brush.Get());
			m_rt->FillGeometry(triangle1, m_brush.Get());
			triangle1->Release();

			point1 = D2D1::Point2F(5.f * cx8, 5.f * cy8);
			point2 = D2D1::Point2F(5.f * cx8, 3.f * cy8);
			point3 = D2D1::Point2F(7.f * cx8, 4.f * cy8);
			triangle1 = DrawTriangle(point1, point2, point3);
			m_rt->DrawGeometry(triangle1, m_brush.Get());
			m_rt->FillGeometry(triangle1, m_brush.Get());
			triangle1->Release();

			point1 = D2D1::Point2F(3.f * cx8, 5.f * cy8);
			point2 = D2D1::Point2F(5.f * cx8, 5.f * cy8);
			point3 = D2D1::Point2F(4.f * cx8, 7.f * cy8);
			triangle1 = DrawTriangle(point1, point2, point3);
			m_rt->DrawGeometry(triangle1, m_brush.Get());
			m_rt->FillGeometry(triangle1, m_brush.Get());
			triangle1->Release();

			point1 = D2D1::Point2F(3.f * cx8, 3.f * cy8);
			point2 = D2D1::Point2F(3.f * cx8, 5.f * cy8);
			point3 = D2D1::Point2F(1.f * cx8, 4.f * cy8);
			triangle1 = DrawTriangle(point1, point2, point3);
			m_rt->DrawGeometry(triangle1, m_brush.Get());
			m_rt->FillGeometry(triangle1, m_brush.Get());
			triangle1->Release();

		}
		break;
	};
	m_brush->SetColor(current);
	
	if(lpDrawItemStruct->itemState & ODS_FOCUS)
	{
		
		m_brush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
		
		m_rt->DrawRectangle(D2D1::RectF(static_cast<float>(rc.left += cx / 32),
						static_cast<float>(rc.top += cy / 32),
						static_cast<float>(rc.right -= cx / 32),
						static_cast<float>(rc.bottom -= cy / 32)), m_brush.Get(),1.f,m_stroke.Get());
		
		m_brush->SetColor(current);
	}
	auto hr = m_rt->EndDraw();
	if(hr == D2DERR_RECREATE_TARGET)
	{
		DestroyDeviceResources();
		CreateDeviceResources();
	}
}

void MyWindow::OnSize(UINT nFlags, Size sz)
{
	m_smaller.MoveWindow(sz.cx / 2 - 3 * BTN_WIDTH / 2,
						 sz.cy / 2 - BTN_HEIGHT / 2,
						 BTN_WIDTH, BTN_HEIGHT, TRUE);
	m_larger.MoveWindow( sz.cx / 2 + BTN_WIDTH / 2,
						 sz.cy / 2 - BTN_HEIGHT / 2,
						BTN_WIDTH, BTN_HEIGHT, TRUE);
}

void MyWindow::OnExit(UINT uNotifyCode, int nID, Window wndCtl)
{
	OnClose();
}


void MyWindow::OnAbout(UINT uNotifyCode, int nID, Window wndCtl)
{

	if(!about)
	{
		if(about.DoModal() == IDOK)
		{
			MessageBox::Show(TEXT("OK pressed!"), TEXT("WinTest"),MessageBoxButtons::Ok, MessageBoxIcon::Information);
		}
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
	auto cxChar = LOWORD(GetDialogBaseUnits());
	auto cyChar = HIWORD(GetDialogBaseUnits());
	BTN_WIDTH = 8 * cxChar;
	BTN_HEIGHT = 4 * cyChar;
	Rect rcSmaller = {0,0,static_cast<int>(BTN_WIDTH),static_cast<int>(BTN_HEIGHT)};
	Rect rcLarger = rcSmaller;
	m_smaller.Create(*this, &rcSmaller,TEXT("button"), UMenuOrID{std::to_underlying(ButtonID::ID_SMALLER)});
	m_larger.Create(*this, &rcLarger ,TEXT("button"), UMenuOrID{std::to_underlying(ButtonID::ID_LARGER)});
	m_smaller.ShowWindow(ShowWindowType::Default);
	m_larger.ShowWindow(ShowWindowType::Default);
    return 0;
}

void MyWindow::OnLButtonDown(UINT nFlags, const Point &pt)
{
    	std::tstringstream str;
        str << TEXT("Clicked At: ") << pt.x << TEXT(",") << pt.y;
        MessageBox::Show(str.str().c_str(), TEXT("WinTest"),MessageBoxButtons::Ok , MessageBoxIcon::Information);
}