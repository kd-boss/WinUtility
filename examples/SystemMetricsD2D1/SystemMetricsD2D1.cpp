/*
	SystemMetricsD2D1, A D2D1 and DirectWrite first approach to modernizing SystemMetrics.
	release build - compile with :
		make
	debug build - compile with :
		make debug
*/

#include "SystemMetricsD2D1.h"
#include <format>

void MyWindow::OnExit(UINT uNotifyCode, int nID, Window wndCtl)
{
	OnClose();
}

void MyWindow::OnAbout(UINT uNotifyCode, int nID, Window wndCtl)
{
	if (!about)
	{
		if (about.DoModal(*this) == IDOK)
		{
		}
	}
}

void MyWindow::OnClose()
{
	PostQuitMessage(0);
}

int MyWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// additional window initalization would go here.

	// initialize m_alignment;
	m_alignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER;

	// initialize the systemmetrics array value strings from ::GetSystemMetrics
	for (auto &e : systemmetrics)
	{
		e.szVal = std::format(TEXT("{:d}"), ::GetSystemMetrics(e.iIndex));
	}

	// get dpi scale
	HDC screen = ::GetDC(0);
	dpiScaleX_ = GetDeviceCaps(screen, LOGPIXELSX) / 96.0f;
	dpiScaleY_ = GetDeviceCaps(screen, LOGPIXELSY) / 96.0f;
	ReleaseDC(screen);

	// Create the factories.
	HR(CreateDeviceIndependentResources());

	// Create the render target, and Dwrite objects.
	HR(CreateDeviceDependentResources());

	// set up the scroll bar.
	CalculateScrollInfo();

	return 0;
}

void MyWindow::OnPaint(DC dc)
{
	PAINTSTRUCT ps;
	BeginPaint(&ps);

	if (!m_renderTarget)
	{
		HR(CreateDeviceDependentResources());
	}
	Rect rc;
	GetClientRect(&rc);
	auto linehight = m_textFormat->GetFontSize();
	auto colwidth = rc.right / 3.0f;
	D2D1_RECT_F col1{static_cast<float>(rc.left), static_cast<float>(rc.top), colwidth, linehight};
	D2D1_RECT_F col2{static_cast<float>(rc.left + colwidth), static_cast<float>(rc.top), colwidth * 2, linehight};
	D2D1_RECT_F col3{static_cast<float>(rc.left + (colwidth * 2)), static_cast<float>(rc.top), colwidth * 3, linehight};
	m_renderTarget->BeginDraw();
	m_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
	si.cbSize = sizeof(si);
	si.fMask = SIF_POS;
	GetScrollInfo(SB_VERT, &si);
	iVertPos = si.nPos;
	iPaintBeg = std::max(0, iVertPos);
	iPaintEnd = NUMLINES - 1;
	for (auto i = iPaintBeg; i <= iPaintEnd; i++)
	{
		auto a = &systemmetrics[i];
		m_renderTarget->DrawTextW(a->szLabel.c_str(), a->szLabel.length(), m_textFormat.Get(), col1, m_textBrush.Get(), D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);
		m_renderTarget->DrawTextW(a->szDesc.c_str(), a->szDesc.length(), m_textFormat.Get(), col2, m_textBrush.Get(), D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);
		auto alignment = m_textFormat->GetTextAlignment();
		m_textFormat->SetTextAlignment(m_alignment);
		m_renderTarget->DrawTextW(a->szVal.c_str(), a->szVal.length(), m_textFormat.Get(), col3, m_textBrush.Get(), D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);
		m_textFormat->SetTextAlignment(alignment);
		col1.bottom += linehight;
		col1.top += linehight;
		col2.bottom += linehight;
		col2.top += linehight;
		col3.top += linehight;
		col3.bottom += linehight;
	}

	m_renderTarget->EndDraw();
	EndPaint(&ps);
}

void MyWindow::OnVScroll(UINT nSBCode, UINT nPos, ScrollBarControl pScrollBar)
{
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	GetScrollInfo(SB_VERT, &si);
	iVertPos = si.nPos;
	switch (nSBCode)
	{
	case SB_TOP:
		si.nPos = si.nMin;
		break;
	case SB_BOTTOM:
		si.nPos = si.nMax;
		break;
	case SB_LINEUP:
		si.nPos -= 1;
		break;
	case SB_LINEDOWN:
		si.nPos += 1;
		break;
	case SB_PAGEUP:
		si.nPos -= si.nPage;
		break;
	case SB_PAGEDOWN:
		si.nPos += si.nPage;
		break;
	case SB_THUMBTRACK:
		si.nPos = si.nTrackPos;
		break;
	default:
		break;
	};

	si.fMask = SIF_POS;
	SetScrollInfo(SB_VERT, &si);
	GetScrollInfo(SB_VERT, &si);
	if (si.nPos != iVertPos)
	{
		ScrollWindow(0, 16 * (iVertPos - si.nPos));
		UpdateWindow();
	}
}

void MyWindow::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_HOME:
		SendMessage(WM_VSCROLL, SB_TOP);
		break;
	case VK_END:
		SendMessage(WM_VSCROLL, SB_BOTTOM);
		break;
	case VK_PRIOR:
		SendMessage(WM_VSCROLL, SB_PAGEUP);
		break;
	case VK_NEXT:
		SendMessage(WM_VSCROLL, SB_PAGEDOWN);
		break;
	case VK_UP:
		SendMessage(WM_VSCROLL, SB_LINEUP);
		break;
	case VK_DOWN:
		SendMessage(WM_VSCROLL, SB_LINEDOWN);
		break;
	case VK_ADD:
	{
		auto size = m_textFormat->GetFontSize();
		m_textFormat.ReleaseAndGetAddressOf();
		m_dwriteFactory->CreateTextFormat(TEXT("Verdana"), NULL,
										  DWRITE_FONT_WEIGHT_REGULAR,
										  DWRITE_FONT_STYLE_NORMAL,
										  DWRITE_FONT_STRETCH_NORMAL,
										  size + 1.0f,
										  TEXT("en-us"),
										  &m_textFormat);
		CalculateScrollInfo();
	}
	break;
	case VK_SUBTRACT:
	{
		auto size = m_textFormat->GetFontSize();
		m_textFormat.ReleaseAndGetAddressOf();
		m_dwriteFactory->CreateTextFormat(TEXT("Verdana"), NULL,
										  DWRITE_FONT_WEIGHT_REGULAR,
										  DWRITE_FONT_STYLE_NORMAL,
										  DWRITE_FONT_STRETCH_NORMAL,
										  size - 1.0f,
										  TEXT("en-us"),
										  &m_textFormat);

		CalculateScrollInfo();
	}
	case VK_TAB:
	{
		switch (m_alignment)
		{
		case DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING:
			m_alignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_TRAILING;
			break;
		case DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_TRAILING:
			m_alignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER;
			break;
		case DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER:
			m_alignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_JUSTIFIED;
			break;
		case DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_JUSTIFIED:
			m_alignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING;
			break;
		}
		Invalidate(FALSE);
		UpdateWindow();
	}
	break;
	};
}

HRESULT MyWindow::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_factory.GetAddressOf());
	if (hr == S_OK)
	{
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_ISOLATED, __uuidof(IDWriteFactory), &m_dwriteFactory);
	}
	return hr;
}

HRESULT MyWindow::CreateDeviceDependentResources()
{

	HRESULT hr = S_OK;
	if (!m_renderTarget)
	{
		RECT rc;
		GetClientRect(&rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
		try
		{

			HR(m_factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
												 D2D1::HwndRenderTargetProperties(m_hwnd, size), &m_renderTarget));

			HR(m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black),
													 &m_textBrush));
			m_dwriteFactory->CreateTextFormat(TEXT("Verdana"), NULL,
											  DWRITE_FONT_WEIGHT_REGULAR,
											  DWRITE_FONT_STYLE_NORMAL,
											  DWRITE_FONT_STRETCH_NORMAL,
											  16.0f,
											  TEXT("en-us"),
											  &m_textFormat);

			auto dpi = GetDpiForWindow();
			m_renderTarget->SetDpi(static_cast<float>(dpi), static_cast<float>(dpi));
		}
		catch (std::exception ex)
		{

			TRACE(ex.what());
		}
	}
	return hr;
}

void MyWindow::CalculateScrollInfo()
{
	if (m_renderTarget)
	{
		D2D1_SIZE_F size;
		m_renderTarget->GetSize(&size);
		float fontSize = 16.0f;

		if (m_textFormat)
		{

			fontSize = m_textFormat->GetFontSize();
		}
		auto screenCap = NUMLINES - static_cast<int>((size.height) / (fontSize));
		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nPage = screenCap;
		si.nMax = screenCap + si.nPage - 1;
		SetScrollInfo(SB_VERT, &si);
		Invalidate(FALSE);
	}
}

void MyWindow::OnSize(UINT nType, Size size)
{
	try
	{
		if (m_renderTarget)
		{
			// Note: This method can fail, but it's okay to ignore the
			// error here, because the error will be returned again
			// the next time EndDraw is called.

			m_renderTarget->Resize(D2D1::SizeU(size.cx, size.cy));
		}
		rtSize = D2D1::SizeF(size.cx, size.cy);
		if (!m_renderTarget)
		{
			CreateDeviceIndependentResources();
		}
		else
		{
			float fontSize = 16.0f;

			if (m_textFormat)
			{

				fontSize = m_textFormat->GetFontSize();
			}
			auto screenCap = NUMLINES - static_cast<int>((size.cy) / (fontSize));
			si.cbSize = sizeof(si);
			si.fMask = SIF_RANGE | SIF_PAGE;
			si.nMin = 0;
			si.nPage = screenCap;
			si.nMax = screenCap + si.nPage - 1;
			SetScrollInfo(SB_VERT, &si);
			Invalidate(FALSE);
			UpdateWindow();
		}
	}
	catch (std::exception ex)
	{
		TRACE(ex.what());
	}
}
