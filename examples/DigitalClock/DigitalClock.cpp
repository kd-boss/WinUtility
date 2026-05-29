

#include "DigitalClock.h"
HRESULT DigitalClock::CreateDeviceIndependantResources()
{
	HRESULT hr = S_OK;
	if(!m_factory)
	{
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_factory.GetAddressOf());
	}
	return hr;
}
HRESULT DigitalClock::CreateDeviceResources()
{
	if (m_rt)
		return S_OK; // already created

	RECT rc;
	GetClientRect(&rc);

	HRESULT hr = m_factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			m_hwnd,
			D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
		m_rt.GetAddressOf());

	if (SUCCEEDED(hr))
		hr = m_rt->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Red), m_brush.GetAddressOf());

	return hr;
}

void DigitalClock::DiscardDeviceResources()
{
	if (m_brush)
	{
		auto br = m_brush.ReleaseAndGetAddressOf();
		br = nullptr;
	}
	if (m_rt)
	{
		auto rt = m_rt.ReleaseAndGetAddressOf();
		rt = nullptr;
	}
}

void DigitalClock::ApplyTransform(float shiftX)
{
	float tx = (shiftX - 138.f) * m_scale + m_size.cx * 0.5f;
	float ty = -36.f * m_scale + m_size.cy * 0.5f;

	m_rt->SetTransform(
		D2D1::Matrix3x2F::Scale(m_scale, m_scale) *
		D2D1::Matrix3x2F::Translation(tx, ty));
}

void DigitalClock::DisplayDigit(int iNumber, float shiftX)
{
	ApplyTransform(shiftX);
	for (int i = 0; i < 7; i++)
		if (fSevenSegment[iNumber][i])
			FillPolygon(ptSegment[i], 6);
}

// shiftX is passed by reference so callers see the updated position
void DigitalClock::DisplayTwoDigits(int iNumber, BOOL fSuppress, float &shiftX)
{
	if (!fSuppress || (iNumber / 10 != 0))
		DisplayDigit(iNumber / 10, shiftX);
	shiftX += 42.f;
	DisplayDigit(iNumber % 10, shiftX);
	shiftX += 42.f;
}

void DigitalClock::DisplayColon(float &shiftX)
{
	ApplyTransform(shiftX);
	FillPolygon(ptColon[0], 4);
	FillPolygon(ptColon[1], 4);
	shiftX += 12.f;
}

void DigitalClock::DisplayTime(BOOL f24Hour, BOOL fSuppress)
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	// Isotropic scale: fit 276×72 logical units into the client area
	m_scale = std::min(static_cast<float>(m_size.cx) / 276.f,
					   static_cast<float>(m_size.cy) / 72.f);

	float shiftX = 0.f; // accumulated rightward offset in logical units

	int hour = f24Hour ? st.wHour
					   : ((st.wHour % 12) ? (st.wHour % 12) : 12);

	DisplayTwoDigits(hour, fSuppress, shiftX);
	DisplayColon(shiftX);
	DisplayTwoDigits(st.wMinute, FALSE, shiftX);
	DisplayColon(shiftX);
	DisplayTwoDigits(st.wSecond, FALSE, shiftX);
}

void DigitalClock::FillPolygon(const D2D1_POINT_2F *pts, UINT count)
{
	ID2D1PathGeometry *pGeom = nullptr;
	if (FAILED(m_factory->CreatePathGeometry(&pGeom)))
		return;

	ID2D1GeometrySink *pSink = nullptr;
	if (SUCCEEDED(pGeom->Open(&pSink)))
	{
		pSink->SetFillMode(D2D1_FILL_MODE_WINDING);
		pSink->BeginFigure(pts[0], D2D1_FIGURE_BEGIN_FILLED);
		pSink->AddLines(pts + 1, count - 1);
		pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		pSink->Close();
		pSink->Release();
	}

	m_rt->FillGeometry(pGeom, m_brush.Get());
	pGeom->Release();
}

HRESULT DigitalClock::Initialize()
{
	HRESULT hr = S_OK;
	// get app title from resource file.
	std::tstring apptitle;
	apptitle.resize(14);
	hr = LoadString(HINST_THISCOMPONENT, IDS_APP_TITLE, apptitle.data(), apptitle.length()) > 0 ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{

		hr = ::IsWindow(Create(nullptr, &Window::rcDefault, apptitle.c_str())) ? S_OK : E_FAIL;
		if (SUCCEEDED(hr))
		{
			ShowWindow(ShowWindowType::Normal);
			UpdateWindow();
		}
	}
	return hr;
}

void DigitalClock::OnSettingsChanged(UINT uiAction, LPCTSTR ActionName)
{

	TCHAR szBuffer[2];
    GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITIME,   szBuffer, 2);
    m_f24Hour   = (szBuffer[0] == TEXT('1'));
    GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITLZERO, szBuffer, 2);
    m_fSuppress = (szBuffer[0] == TEXT('0'));
    Invalidate(FALSE); //FALSE Direct 2D draws everything.
}

void DigitalClock::OnExit(UINT uNotifyCode, int nID, Window wndCtl)
{
	OnClose();
}

void DigitalClock::OnAbout(UINT uNotifyCode, int nID, Window wndCtl)
{

	if (!about)
	{
		about.DoModal();
		
	}
}

void DigitalClock::OnClose()
{
	
    KillTimer( ID_TIMER);
	DestroyWindow();
}

void DigitalClock::OnDestroy()
{
	DiscardDeviceResources();
    auto ft = m_factory.ReleaseAndGetAddressOf();    
	ft = nullptr;
	PostQuitMessage(0);
}

int DigitalClock::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// additional window initalization would go here.
	CreateDeviceIndependantResources();
	SetTimer(ID_TIMER, 1000, nullptr);
	TCHAR szBuffer[2];
    GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITIME,   szBuffer, 2);
    m_f24Hour   = (szBuffer[0] == TEXT('1'));
    GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITLZERO, szBuffer, 2);
    m_fSuppress = (szBuffer[0] == TEXT('0'));
    Invalidate(FALSE); //FALSE Direct 2D draws everything.
	return 0;
}
void DigitalClock::OnPaint(DC dc)
{
	HRESULT hr = CreateDeviceResources();
        if (FAILED(hr)) return;

        PAINTSTRUCT ps;
        BeginPaint(&ps);         // validate the region

        m_rt->BeginDraw();

        // Clear to white (replaces the WHITE_BRUSH background)
        m_rt->Clear(D2D1::ColorF(D2D1::ColorF::Aquamarine));

        DisplayTime(m_f24Hour, m_fSuppress);

        // Reset the transform so nothing else is skewed
        m_rt->SetTransform(D2D1::Matrix3x2F::Identity());

        hr = m_rt->EndDraw();
        if (hr == D2DERR_RECREATE_TARGET)
        {
            // Device was lost – release and recreate next paint
            DiscardDeviceResources();
        }

        EndPaint(&ps);
}
void DigitalClock::OnTimer(UINT_PTR nIDEvent)
{
	Invalidate(FALSE);	
}

void DigitalClock::OnSize(UINT nType, Size size)
{
	m_size = size;
	if(m_rt)
		m_rt->Resize(D2D1::SizeU(m_size.cx, m_size.cy));
}
