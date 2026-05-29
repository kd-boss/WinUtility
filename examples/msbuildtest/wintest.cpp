#include "wintest.h"
#include <format>
#include <utility>

void MyWindow::ReleaseResources()
{
	m_background.Reset();
	m_rt.Reset();
}

HRESULT MyWindow::Initialize()
{
	HRESULT hr = S_OK;
	// get app title from resource file.
	std::tstring apptitle;
	apptitle.resize(256);
	LoadString(HINST_THISCOMPONENT, IDS_APP_TITLE, apptitle.data(), static_cast<int>(apptitle.length()));
	apptitle.shrink_to_fit();

	hr = ::IsWindow(Create(nullptr, &Window::rcDefault, apptitle.c_str())) ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{
		ShowWindow(ShowWindowType::Normal);
		UpdateWindow();
	}
	return hr;
}

LRESULT MyWindow::OnWaveOpen(UINT msg, WPARAM wparam, LPARAM lparam, BOOL &bHandled)
{
	m_out.OnOpen();
	return 1;
}

LRESULT MyWindow::OnWaveDone(UINT, WPARAM, LPARAM lparam, BOOL& bHandled)
{
	m_out.OnDone(lparam);
	return 1;
}

LRESULT MyWindow::OnWaveClose(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	m_out.OnClose();
	return 1;
}

LRESULT MyWindow::OnThemeChanged(UINT msg, WPARAM wparam, LPARAM lparam, BOOL &bHandled)
{
	m_combo.SendMessage(msg, wparam, lparam);
	return m_dial.SendMessage(msg, wparam, lparam);
}

LRESULT MyWindow::OnSettingChange(UINT msg, WPARAM wparam, LPARAM lparam, BOOL &bHandled)
{
	m_combo.SendMessage(msg, wparam, lparam);
	return m_dial.SendMessage(msg, wparam, lparam);
}

void MyWindow::OnPaint(DC dc)
{
	if (!m_rt)
	{
		CreateRenderTarget();
	}

	PAINTSTRUCT ps{0};
	HDC ldc = BeginPaint(&ps);
	if (SUCCEEDED(m_rt->BindDC(ldc, &ps.rcPaint)))
	{
		Rect rc, rcc;
		GetClientRect(rc);
		m_vscroll_a.GetClientRect(&rcc);
		rc.left = rcc.left;
		D2D1_COLOR_F clear{r, g, b, a};
		m_rt->BeginDraw();
		m_background->SetColor(clear);
		m_rt->FillRectangle(D2D1::RectF(static_cast<float>(rc.left),
										static_cast<float>(rc.top),
										static_cast<float>(rc.right),
										static_cast<float>(rc.bottom)),
							m_background.Get());
		m_rt->EndDraw();
	}
	EndPaint(&ps);
}

void MyWindow::OnNcDestroy()
{
	PostQuitMessage(0);
}

void MyWindow::OnExit(UINT uNotifyCode, int nID, Window wndCtl)
{
	OnClose();
}

void MyWindow::OnVScroll(UINT nCode, UINT nPos, ScrollBarControl sb)
{
	switch (static_cast<Controls>(sb.GetDlgCtrlID()))
	{
	case Controls::ID_RED:
	{
		float pos = static_cast<float>(m_vscroll_r.GetScrollPos());
		r = (500.f - pos) / 500.f;

		m_static_r.SetWindowText(std::format(TEXT("{:1.6f}"), r).c_str());
		Invalidate(FALSE);
	}
	break;
	case Controls::ID_GREEN:
	{

		float pos = static_cast<float>(m_vscroll_g.GetScrollPos());
		g = (500.f - pos) / 500.f;

		m_static_g.SetWindowText(std::format(TEXT("{:1.6f}"), g).c_str());
		Invalidate(FALSE);
	}
	break;
	case Controls::ID_BLUE:
	{
		float pos = static_cast<float>(m_vscroll_b.GetScrollPos());
		b = (500.f - pos) / 500.f;
		m_static_b.SetWindowText(std::format(TEXT("{:1.6f}"), b).c_str());
		Invalidate(FALSE);
	}
	break;
	case Controls::ID_ALPHA:
	{
		float pos = static_cast<float>(m_vscroll_a.GetScrollPos());
		a = (500.f - pos) / 500.f;
		m_static_a.SetWindowText(std::format(TEXT("{:1.6f}"), a).c_str());
		Invalidate(FALSE);
	}
	break;
	case Controls::ID_DIAL:
	{
		int curPos = m_dial.GetPos();
		if (m_dialPos < curPos)
		{
			m_edit.SetFont(TEXT("New Times Roman"), static_cast<float>(curPos));
			// rotating clockwise, tuning up.

			SetWindowText(std::format(TEXT("Dial (clockwise) Pos: {:d}"), curPos).c_str());
		}
		else
		{
			m_edit.SetFont(TEXT("New Times Roman"), static_cast<float>(curPos));
			// rotating counterclockwise, tuning down.
			SetWindowText(std::format(TEXT("Dial (counter-clockwise) Pos: {:d}"), curPos).c_str());
		}
		m_dialPos = curPos;
	}
	break;
	case Controls::ID_DEVICES:
	{
	}
	break;
	case Controls::ID_STATIC_A:
	case Controls::ID_STATIC_B:
	case Controls::ID_STATIC_R:
	case Controls::ID_STATIC_G:
	default:
		break;
	};
}

void MyWindow::OnAbout(UINT uNotifyCode, int nID, Window wndCtl)
{
	INT_PTR res = 0;

	if (!about)
	{
		res = about.DoModal();
		if (res == IDOK)
		{
			MessageBox::Show(TEXT("OK pressed!"), TEXT("WinTest"), MessageBoxButtons::Ok, MessageBoxIcon::Information);
		}
		if(res == IDCANCEL)
		{
			MessageBox::Show(TEXT("CANCEL pressed!"), TEXT("WinTest"), MessageBoxButtons::Ok, MessageBoxIcon::Information);
		}
	}
}

void MyWindow::OnClose()
{
	m_dial.ReleaseResources();
	m_vscroll_r.ReleaseResources();
	m_vscroll_g.ReleaseResources();
	m_vscroll_b.ReleaseResources();
	m_vscroll_a.ReleaseResources();
	m_edit.ReleaseResources();
	ReleaseResources();

	while (m_factory.Reset() > 0)
		;

	DestroyWindow();
}

HRESULT MyWindow::CreateFactory()
{
	HRESULT hr = S_OK;
	if (!m_factory)
	{
		return ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_factory.GetAddressOf());
	}
	return hr;
}

HRESULT MyWindow::CreateRenderTarget()
{
	if (!m_factory || m_rt)
		return (m_rt ? S_OK : E_FAIL);

	D2D1_RENDER_TARGET_PROPERTIES props =
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE));

	HRESULT hr = m_factory->CreateDCRenderTarget(&props, m_rt.GetAddressOf());
	if (SUCCEEDED(hr))
	{
		hr = m_rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), m_background.GetAddressOf());
	}
	return hr;
}

int MyWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	std::vector<std::tstring> strings;
	strings.push_back(TEXT("Top"));
	strings.push_back(TEXT("Center"));
	strings.push_back(TEXT("Bottom"));

	CreateFactory();
	// additional window initalization would go here.
	Rect rc;
	GetClientRect(&rc);
	rc.left = 0;
	rc.right = 100;
	rc.top += 25;
	Rect stat{0, 100, 0, 25};
	m_vscroll_r.SetSharedFactory(m_factory.Get());
	m_vscroll_g.SetSharedFactory(m_factory.Get());
	m_vscroll_b.SetSharedFactory(m_factory.Get());
	m_vscroll_a.SetSharedFactory(m_factory.Get());
	m_dial.SetSharedFactory(m_factory.Get());
	m_edit.SetSharedFactory(m_factory.Get());
	m_audioDisp.SetFactory(m_factory.Get());

	m_static_r.Create(*this, &stat, TEXT("0"), WS_CHILD | WS_VISIBLE | SS_CENTER | WS_OVERLAPPED, 0, UMenuOrID{(HMENU) static_cast<DWORD>(Controls::ID_STATIC_R)}.Get());
	m_static_g.Create(*this, &stat, TEXT("0"), WS_CHILD | WS_VISIBLE | SS_CENTER | WS_OVERLAPPED, 0, UMenuOrID{(HMENU) static_cast<DWORD>(Controls::ID_STATIC_G)}.Get());
	m_static_b.Create(*this, &stat, TEXT("0"), WS_CHILD | WS_VISIBLE | SS_CENTER | WS_OVERLAPPED, 0, UMenuOrID{(HMENU) static_cast<DWORD>(Controls::ID_STATIC_B)}.Get());
	m_static_a.Create(*this, &stat, TEXT("0"), WS_CHILD | WS_VISIBLE | SS_CENTER | WS_OVERLAPPED, 0, UMenuOrID{(HMENU) static_cast<DWORD>(Controls::ID_STATIC_A)}.Get());
	m_vscroll_r.Create(*this, &rc, TEXT("red"), WS_CHILD | WS_VISIBLE | SBS_VERT | CSBS_MARKERS, 0, UMenuOrID{std::to_underlying(Controls::ID_RED)}.Get());
	m_vscroll_g.Create(*this, &rc, TEXT("blue"), WS_CHILD | WS_VISIBLE | SBS_VERT | CSBS_MARKERS, 0, UMenuOrID{std::to_underlying(Controls::ID_GREEN)}.Get());
	m_vscroll_b.Create(*this, &rc, TEXT("green"), WS_CHILD | WS_VISIBLE | SBS_VERT | CSBS_MARKERS, 0, UMenuOrID{std::to_underlying(Controls::ID_BLUE)}.Get());
	m_vscroll_a.Create(*this, &rc, TEXT("alpha"), WS_CHILD | WS_VISIBLE | SBS_VERT | CSBS_MARKERS, 0, UMenuOrID{std::to_underlying(Controls::ID_ALPHA)}.Get());
	m_dial.Create(m_hwnd, &rc, nullptr, WS_CHILD | WS_VISIBLE | WS_TABSTOP, 0, UMenuOrID{std::to_underlying(Controls::ID_DIAL)}.Get());
	m_edit.Create(m_hwnd, &rc, nullptr, WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 0, UMenuOrID{std::to_underlying(Controls::ID_EDIT)}.Get());
	m_audioDisp.Create(m_hwnd, &rc, TEXT("Audio Display"), WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_CLIPCHILDREN, 0, UMenuOrID{std::to_underlying(Controls::ID_AUDIO)}.Get());
	m_combo.Create(m_hwnd, &rc, TEXT("Text Alignment"), CBS_DROPDOWN | CBS_SIMPLE | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, UMenuOrID{std::to_underlying(Controls::ID_COMBO)}.Get());
	m_devices.Create(m_hwnd, &rc, TEXT("Audio Device"), CBS_DROPDOWN | CBS_SIMPLE | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, UMenuOrID{std::to_underlying(Controls::ID_DEVICES)}.Get());

	m_dial.SetRange(1, 100);
	m_dial.ShowMarker(false);
	m_dial.SetTracking(true);
	m_dial.SetWrapping(true);

	m_edit.SetFont(TEXT("New Times Roman"), 10.0f);
	m_edit.SetWindowText(TEXT("Testing!"));

	if (!m_audioDisp.IsWindow())
	{
		MessageBox::Show(TEXT("Audio Diplay Creation Failure"), TEXT("msbuildtest"), MessageBoxButtons::Ok, MessageBoxIcon::Information);
	}

	std::vector<std::wstring> devices = m_audioDisp.GetAudioInputDevices();

	for (auto i = 0; i < devices.size(); i++)
	{
		COMBOBOXEXITEM item{0};
		ZeroMemory(&item, sizeof(COMBOBOXEXITEM));

		item.iItem = i;

		item.mask = CBEIF_TEXT;
		item.pszText = (LPWSTR)devices[i].c_str();
		item.cchTextMax = static_cast<int>(devices[i].length());

		// int InsertItem(const COMBOBOXEXITEM *lpcCBItem)
		auto ret = m_devices.InsertItem(reinterpret_cast<const COMBOBOXEXITEM *>(&item));

		if (ret < 0)
			MessageBox::Show(m_hwnd, TEXT("Couldn't add item :("), TEXT("Wintest"), MessageBoxButtons::Ok, MessageBoxIcon::Information);
	}

	for (int i = 0; i <= 2; i++)
	{
		COMBOBOXEXITEM item{0};
		ZeroMemory(&item, sizeof(COMBOBOXEXITEM));
		item.iItem = i;
		item.mask = CBEIF_TEXT;
		item.pszText = (LPWSTR)strings[i].c_str();
		item.cchTextMax = static_cast<int>(strings[i].length());

		// int InsertItem(const COMBOBOXEXITEM *lpcCBItem)
		auto ret = m_combo.InsertItem(reinterpret_cast<const COMBOBOXEXITEM *>(&item));

		if (ret < 0)
			MessageBox::Show(m_hwnd, TEXT("Couldn't add item :("), TEXT("Wintest"), MessageBoxButtons::Ok, MessageBoxIcon::Information);
	}
	m_combo.SetCurSel(0);
	m_devices.SetCurSel(0);

	COMBOBOXINFO info{0};
	info.cbSize = sizeof(COMBOBOXINFO);

	auto edit = m_combo.GetEditCtrl();
	if (edit.IsWindow())
	{
		DWORD style = edit.GetStyle();
		edit.ModifyStyle(0, style | ES_CENTER);
		edit.Invalidate();
	}

	SCROLLINFO inf;
	inf.fMask = SIF_PAGE | SIF_RANGE;
	inf.nPage = 500 / 10;
	inf.nMax = 500;
	inf.nMin = 0;
	inf.cbSize = sizeof(SCROLLINFO);
	m_vscroll_r.SetScrollInfo(&inf);
	m_vscroll_b.SetScrollInfo(&inf);
	m_vscroll_g.SetScrollInfo(&inf);
	m_vscroll_a.SetScrollInfo(&inf);

	m_vscroll_r.SetScrollPos(inf.nMax);
	m_vscroll_g.SetScrollPos(inf.nMax);
	m_vscroll_b.SetScrollPos(inf.nMax);
	m_vscroll_a.SetScrollPos(inf.nMax);

	m_vscroll_r.SetThumbThickness(15);
	m_vscroll_g.SetThumbThickness(15);
	m_vscroll_b.SetThumbThickness(15);
	m_vscroll_a.SetThumbThickness(15);

	m_vscroll_r.SetMarkerColor(D2D1::ColorF(D2D1::ColorF::Red));
	m_vscroll_g.SetMarkerColor(D2D1::ColorF(D2D1::ColorF::Red));
	m_vscroll_b.SetMarkerColor(D2D1::ColorF(D2D1::ColorF::Red));
	m_vscroll_a.SetMarkerColor(D2D1::ColorF(D2D1::ColorF::Red));
	m_audioDisp.SetDevice(0);
	return 0;
}

void MyWindow::OnSize(UINT nType, Size size)
{
	Rect rc;
	m_vscroll_r.GetClientRect(&rc);

	m_static_r.MoveWindow(0, 0, rc.Width(), 25);
	m_static_g.MoveWindow(rc.Width(), 0, rc.Width(), 25);
	m_static_b.MoveWindow(rc.Width() * 2, 0, rc.Width(), 25);
	m_static_a.MoveWindow(rc.Width() * 3, 0, rc.Width(), 25);
	m_vscroll_r.MoveWindow(0, rc.top + 25, rc.Width(), size.cy - 25);
	m_vscroll_g.MoveWindow(rc.Width(), rc.top + 25, rc.Width(), size.cy - 25);
	m_vscroll_b.MoveWindow(rc.Width() * 2, rc.top + 25, rc.Width(), size.cy - 25);
	m_vscroll_a.MoveWindow(rc.Width() * 3, rc.top + 25, rc.Width(), size.cy - 25);
	m_dial.MoveWindow(rc.Width() * 4 + 2, rc.top + 5, 100, 100);

	rc.top += 105;
	rc.bottom += 105;
	m_edit.MoveWindow((rc.Width() * 4) + 2, rc.top + 10, 130, 100);

	rc.top += 100;
	rc.bottom += 100;

	m_edit.SetVertAlign(D2DEditControl::VertAlign::Top);
	m_edit.Invalidate();
	m_combo.MoveWindow((rc.Width() * 4) + 2, rc.top + 20, 130, 80);

	rc.top += 80;
	rc.bottom += 80;

	m_audioDisp.MoveWindow((rc.Width() * 4) + 132, 0, size.cx - ((rc.Width() * 4) + 132), 300);

	m_devices.MoveWindow((rc.Width() * 4) + 2, rc.top + 20, 350, 80);

	m_vscroll_r.SetScrollPos(m_vscroll_r.GetScrollPos());
	m_vscroll_g.SetScrollPos(m_vscroll_g.GetScrollPos());
	m_vscroll_b.SetScrollPos(m_vscroll_b.GetScrollPos());
	m_vscroll_a.SetScrollPos(m_vscroll_a.GetScrollPos());
}

void MyWindow::OnLButtonDown(UINT nFlags, const Point &pt)
{
	m_out.Start(30, 20000,440, *this);
	//std::tstring str = std::format(TEXT("Clicked At: {:d} , {:d}"), pt.x, pt.y);
	//MessageBox::Show(&str[0], TEXT("WinTest"), MessageBoxButtons::Ok, MessageBoxIcon::Information);
}

void MyWindow::OnDevicesChange(UINT uNotifyCode, int nID, Window wndCtl)
{
	int x = m_devices.GetCurSel();
	if (x != m_micsel)
	{
		m_micsel = x;
		m_audioDisp.SetDevice(m_micsel);
	}
}

void MyWindow::OnCombo(UINT uNoitfycode, int nID, Window wndCtl)
{
	m_edit.SetVertAlign(static_cast<D2DEditControl::VertAlign>(m_combo.GetCurSel()));
}