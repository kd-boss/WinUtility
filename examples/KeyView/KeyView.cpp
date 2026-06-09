
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#include <WinUtility/Numbers.h>
#include <WinUtility/System.h>
#include "KeyView.h"
#include <format>
#include <bitset>
#include <thread>
#include <mutex>

void MyWindow::RenderLine(const std::wstring &text, float x, float y)
{
	if (text.empty() || !m_rt || !m_tf)
		return;

	IDWriteTextLayout *pLayout = nullptr;
	HRESULT hr = m_dw->CreateTextLayout(
		text.c_str(),
		convert_to<UINT32>(text.size()),
		m_tf.Get(),
		convert_to<float>(g_cxClient > 0 ? g_cxClient * 2 : 4000),
		g_cyChar + 4.0f,
		&pLayout);

	if (SUCCEEDED(hr))
	{
		m_rt->DrawTextLayout(D2D1::Point2F(x, y), pLayout, m_brush.Get());
		pLayout->Release();
	}
}

HRESULT MyWindow::Render()
{
	HRESULT hr = S_OK;
	if (!m_rt)
	{
		hr = CreateDeviceResources();
	}
	if (SUCCEEDED(hr))
	{
		//std::lock_guard<std::mutex> lock(m_renderSync);
		m_rt->SetTransform(D2D1::Matrix3x2F::Identity());
		m_rt->BeginDraw();
		m_rt->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
		
		RenderLine(szTop, 0.0f, 0.0f);
		RenderLine(szUnd, 0.0f, 0.0f);

		
		
		
		
		Number<size_t> count = std::min(m_layouts.size(), static_cast<size_t>(visibleRows));
		for (Number<size_t> i = 0; i < count; i++)
		{
			float yPos = (g_cyChar *  i) + g_cyChar;
			m_rt->DrawTextLayout(D2D1::Point2F(0.0f, yPos),m_layouts[i].Get(), m_brush.Get());
		}
		hr = m_rt->EndDraw();
	}
	if (hr == D2DERR_RECREATE_TARGET)
	{
		DiscardDeviceResources();
		hr = CreateDeviceResources();
	}
	return hr;
}

HRESULT MyWindow::CreateDeviceIndependantResources()
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
								   m_ft.GetAddressOf());
	if (FAILED(hr))
		return hr;

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
							 __uuidof(IDWriteFactory),
							 reinterpret_cast<IUnknown **>(m_dw.GetAddressOf()));
	if (FAILED(hr))
		return hr;

	// Create a fixed-pitch text format (Courier New matches SYSTEM_FIXED_FONT look)
	hr = m_dw->CreateTextFormat(
		L"Consolas",
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		14.0f, // em size in DIPs
		L"en-us",
		&m_tf);
	if (FAILED(hr))
		return hr;

	m_tf->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	m_tf->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	// No wrapping — lines are intentionally wide
	m_tf->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

	// Measure a single character to get the cell size
	IDWriteTextLayout *pLayout = nullptr;
	hr = m_dw->CreateTextLayout(
		L"M", 1, m_tf.Get(), 1000.0f, 1000.0f, &pLayout);
	if (SUCCEEDED(hr))
	{
		DWRITE_TEXT_METRICS tm{};
		pLayout->GetMetrics(&tm);
		g_cxChar = tm.width;
		g_cyChar = tm.height;
		pLayout->Release();
	}
	return hr;
}

HRESULT MyWindow::CreateDeviceResources()
{
	Rect rc;
	GetClientRect(&rc);
	HRESULT hr = S_OK;
	if (!m_rt)
	{
		hr = m_ft->CreateHwndRenderTarget(D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_UNKNOWN)),
										  D2D1::HwndRenderTargetProperties(*this, D2D1::SizeU(static_cast<UINT32>(rc.right - rc.left),
																							  static_cast<UINT32>(rc.bottom - rc.top))),
										  m_rt.GetAddressOf());
	}
	if (SUCCEEDED(hr))
	{
		hr = m_rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), m_brush.GetAddressOf());
	}

	return hr;
}

void MyWindow::DiscardDeviceResources()
{
	if (m_rt)
	{
		auto rt = m_rt.ReleaseAndGetAddressOf();
		rt = nullptr;
	}
	if (m_brush)
	{
		auto br = m_brush.ReleaseAndGetAddressOf();
		br = nullptr;
	}
}

std::tstring MyWindow::LoadTextResource(UINT ID)
{
	std::tstring data;
	data.resize(8 + 1);
	::LoadString(HINST_THISCOMPONENT, ID, data.data(),convert_to<int>(data.length()));
	return data;
}

HRESULT MyWindow::Initialize()
{
	HRESULT hr = S_OK;
	while(hr = CreateDeviceIndependantResources() != S_OK);
	// get app title from resource file.
	if (SUCCEEDED(hr))
	{
		std::tstring apptitle = LoadTextResource(IDS_APP_TITLE);
		if (SUCCEEDED(hr))
		{

			apptitle.shrink_to_fit();
			
			hr = ::IsWindow(Create(nullptr, &Window::rcDefault, apptitle.c_str())) ? S_OK : E_FAIL;
			if (SUCCEEDED(hr))
			{

				ShowWindow(ShowWindowType::Normal);
				Invalidate(FALSE);
			}
		}
	}
	return hr;
}
void MyWindow::OnGetMinMaxInfo(LPMINMAXINFO lpMMI)
{
	HWND hDesktop = GetDesktopWindow();
	UINT dpi = ::GetDpiForWindow(hDesktop);
	auto scale = dpi / 96.0;
	
	lpMMI->ptMaxTrackSize.x = 516 * static_cast<long>(scale);
	lpMMI->ptMinTrackSize.x = 516 * static_cast<long>(scale); 

}

void MyWindow::OnSize(UINT nType, Size size)
{
	if (m_rt)
	{
		HRESULT hr = m_rt->Resize(D2D1::SizeU(size.cx, size.cy));
		g_cyClient = size.cy;
		visibleRows = (g_cyChar > 0.0f)
							  ? static_cast<int>(g_cyClient / g_cyChar) - 1
							  : 0;
		
		
		if (hr == D2DERR_RECREATE_TARGET)
		{
			DiscardDeviceResources();
			hr = CreateDeviceResources();
		}
	}
}

void MyWindow::OnClear(UINT uNotifyCode, int nID, Window wndCtl)
{
	m_layouts.clear();
	m_layouts.shrink_to_fit();
	Invalidate(FALSE);
}

void MyWindow::OnExit(UINT uNotifyCode, int nID, Window wndCtl)
{
	OnClose();
}

void MyWindow::OnAbout(UINT uNotifyCode, int nID, Window wndCtl)
{

	if (!about)
	{
		if (about.DoModal() == IDOK)
		{
			MessageBox::Show(TEXT("OK pressed!"), TEXT("WinTest"), MessageBoxButtons::Ok, MessageBoxIcon::Information);
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
	if (!m_rt)
	{
		CreateDeviceResources();
	}
	GetClientRect(&rc);
	m_rt->Resize(D2D1::SizeU(rc.Width(), rc.Height()));

	return 0;
}

void MyWindow::OnPaint(DC dc)
{
	BeginPaint(&ps);
	Render();
	EndPaint(&ps);
}

HRESULT MyWindow::FormatKeyMessage(
	const std::tstring& msgName,
	WPARAM wParam, LPARAM lParam,
	const std::tstring& keyName)
{

	const wchar_t *ext = (  HIWORD(lParam) & KF_EXTENDED) ? L"Yes": L"No";
	const wchar_t *alt = (  HIWORD(lParam) & KF_ALTDOWN) ?  L"Yes": L"No";
	const wchar_t *prev = ( HIWORD(lParam) & KF_REPEAT) ?  L"Down": L"Up";
	const wchar_t *tran = ( HIWORD(lParam) & KF_UP) ? L"Up" : L"Down";
	UINT repeat = LOWORD(lParam);
	int scan = static_cast<int>(HIWORD(lParam) & 0xFF);

	std::tstring text =  std::move(std::format(
		TEXT(" {:<13} {:>3} {:<15} {:>6} {:>4} {:>3} {:>3} {:>4} {:>4}"),
		msgName,
		static_cast<int>(wParam),
		keyName,
		repeat, scan,
		ext, alt, prev, tran));

		IDWriteTextLayout *pLayout = nullptr;
		HRESULT hr = m_dw->CreateTextLayout(
		text.c_str(),
		static_cast<UINT32>(text.size()),
		m_tf.Get(),
		static_cast<float>(g_cxClient > 0 ? g_cxClient * 2 : 4000),
		g_cyChar + 4.0f,
		&pLayout);
		if(SUCCEEDED(hr))
			m_layouts.emplace_back(pLayout);

		if(m_layouts.size() > visibleRows)
		{
			m_layouts.erase(m_layouts.begin(), m_layouts.begin() + m_layouts.size() - visibleRows);
		}
		return hr;

}

HRESULT MyWindow::FormatCharMessage(
	const std::tstring& msgName,
	WPARAM wParam, LPARAM lParam)
{
	const wchar_t *ext = ( HIWORD(lParam) & KF_EXTENDED) ? TEXT("Yes") : TEXT("No");
	const wchar_t *alt = ( HIWORD(lParam) & KF_ALTDOWN) ?  TEXT("Yes") : TEXT("No");
	const wchar_t *prev = (HIWORD(lParam) & KF_REPEAT) ?  TEXT("Down"): TEXT("Up");
	const wchar_t *tran = (HIWORD(lParam) & KF_UP) ? 	  TEXT("Up") : TEXT("Down");
	UINT repeat = LOWORD(lParam);
	int scan = static_cast<int>(HIWORD(lParam) & 0xFF);
	wchar_t ch = (wParam >= 32) ? static_cast<wchar_t>(wParam) : L' ';
	
	std::tstring text = std::move( std::format(
		TEXT(" {:<13}            0x{:04X} {:c} {:>6} {:>4} {:>3} {:>3} {:>4} {:>4}"),
		msgName,
		static_cast<UINT>(wParam),
		ch,
		repeat, scan,
		ext, alt, prev, tran));

		IDWriteTextLayout *pLayout = nullptr;
		HRESULT hr = m_dw->CreateTextLayout(
		text.c_str(),
		static_cast<UINT32>(text.size()),
		m_tf.Get(),
		static_cast<float>(g_cxClient > 0 ? g_cxClient * 2 : 4000),
		g_cyChar + 4.0f,
		&pLayout);
		if(SUCCEEDED(hr))
			m_layouts.emplace_back(pLayout);

		if(m_layouts.size() > visibleRows)
		{
			m_layouts.erase(m_layouts.begin(), m_layouts.begin() + m_layouts.size() - visibleRows);
		}
		return hr;
}

void MyWindow::OnKeyDown(UINT nChar, SHORT nRepCnt, SHORT nFlags)
{
	Number<long> lParam = MAKELPARAM(nRepCnt,nFlags);
	std::tstring buff;
	buff.resize(35);
	Number<int> len = buff.length();
	GetKeyNameText(lParam, buff.data(), len);	
	if(SUCCEEDED(FormatKeyMessage(messageNames[WM_KEYDOWN - WM_KEYFIRST].c_str(), nChar,lParam , buff.c_str())))
	Invalidate(FALSE);	
}

void MyWindow::OnKeyUp(UINT nChar, SHORT nRepCnt, SHORT nFlags)
{
	std::wstring buff;
	buff.resize(35);
	auto lParam = MAKELPARAM( nRepCnt, nFlags);	
	try{
		GetKeyNameText(convert_to<LONG>(lParam), buff.data(), convert_to<int>(buff.length()));
	}
	catch(const Bad_value&)
	{
       debug_println("OnKeyUp threw");
	}
	buff.shrink_to_fit();
	if(SUCCEEDED(FormatKeyMessage(messageNames[WM_KEYUP - WM_KEYFIRST].c_str(), nChar,lParam , buff.c_str())))
	Invalidate(FALSE);
}

void MyWindow::OnSysKeyUp(UINT nChar, SHORT nRepCnt, SHORT nFlags)
{
	std::wstring buff;
	buff.resize(35);
	auto lParam = MAKELPARAM( nRepCnt, nFlags);	
	GetKeyNameText(convert_to<LONG>(lParam), buff.data(), convert_to<int>(buff.length()));
	buff.shrink_to_fit();
	if(SUCCEEDED(FormatKeyMessage(messageNames[WM_SYSKEYUP - WM_KEYFIRST].c_str(), nChar,lParam , buff.c_str())))	
	Invalidate(FALSE);	
}

void MyWindow::OnSysKeyDown(UINT nChar, SHORT nRepCnt, SHORT nFlags)
{
	std::wstring buff;
	buff.resize(35);
	auto lParam = MAKELPARAM( nRepCnt, nFlags);	
	GetKeyNameText(convert_to<LONG>(lParam), buff.data(), convert_to<int>(buff.length()));
	buff.shrink_to_fit();
	if(SUCCEEDED(FormatKeyMessage(messageNames[WM_SYSKEYDOWN - WM_KEYFIRST].c_str(), nChar,lParam , buff.c_str())))
	Invalidate(FALSE);	
}

void MyWindow::OnChar(TCHAR nChar, SHORT nRepCnt, SHORT nFlags)
{
	std::wstring buff;
	buff.push_back(nChar);
	if(SUCCEEDED(FormatCharMessage(messageNames[WM_CHAR - WM_KEYFIRST].c_str(), nChar, MAKELPARAM(nRepCnt,nRepCnt))))
	Invalidate(FALSE);
}

void MyWindow::OnSysChar(TCHAR nChar, SHORT nRepCnt, SHORT nFlags)
{
	std::wstring buff;
	buff.push_back(nChar);
	if(SUCCEEDED(FormatCharMessage(messageNames[WM_SYSCHAR - WM_KEYFIRST].c_str(), nChar, MAKELPARAM(nRepCnt,nRepCnt))))
	Invalidate(FALSE);	
}

void MyWindow::OnDeadChar(TCHAR nChar,SHORT nRepCnt, SHORT nFlags)
{
	std::wstring buff;
	buff.push_back(nChar);
	if(SUCCEEDED(FormatCharMessage(messageNames[WM_DEADCHAR - WM_KEYFIRST].c_str(), nChar, MAKELPARAM(nRepCnt,nRepCnt))))
	Invalidate(FALSE);	
}
void MyWindow::OnSysDeadChar(TCHAR nChar,SHORT nRepCnt, SHORT nFlags)
{
	std::wstring buff;
	buff.push_back(nChar);
	if(SUCCEEDED(FormatCharMessage(messageNames[WM_SYSDEADCHAR - WM_KEYFIRST].c_str(), nChar, MAKELPARAM(nRepCnt,nRepCnt))))
	Invalidate(FALSE);
}