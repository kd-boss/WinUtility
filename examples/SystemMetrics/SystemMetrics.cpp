/*
    SystemMetrics, Small Demo application that creates a user interface window.
    release build - compile with : make
    debug build - compile with :   make debug
*/

#include "SystemMetrics.h"
#include <format>

BOOL MyAboutDialog::OnInitDialog(Window wndFocus, LPARAM lInitParam)
{
	return CenterWindow();
}

void MyAboutDialog::OnClose()
{
	EndDialog(IDCLOSE);
	Detach();
}

void MyAboutDialog::OnOk(UINT uNotifyCode, int nID, Window wndCtl)
{
	EndDialog(nID);
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
			/*MessageBox::Show(TEXT("OK pressed!"), TEXT("WinTest"),MessageBoxButtons::Ok, MessageBoxIcon::Information);*/
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
	DCT<true> hdc = GetDC();
	hdc.GetTextMetricsW(&tm);
	cxChar = tm.tmAveCharWidth;
	cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
	cyChar = tm.tmHeight + tm.tmExternalLeading;
	iMaxWidth = 40 * cxChar + 22 * cxCaps;
	iVscrollPos = 0;
	SetScrollRange(SB_VERT, 0, NUMLINES - 1, FALSE);
	SetScrollPos(SB_VERT, iVscrollPos, TRUE);
	for(auto &a : systemmetrics)
	{
		a.szVal = std::format(TEXT("{:d}"),GetSystemMetrics(a.iIndex));
	}
	ps = {0};
	Invalidate();
	UpdateWindow();
    return 0;
}

void MyWindow::OnSize(UINT nType, Size size)
{
	cyClient = size.cy;
	SCROLLINFO si = {0};
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = 0;
	si.nMax = NUMLINES - 1;
	si.nPage = cyClient / cyChar;
	SetScrollInfo(SB_VERT, &si);

	si.cbSize = sizeof(SCROLLINFO);
	si.nMax = 2 + iMaxWidth / cxChar;
	si.nPage = size.cx / cxChar;
	SetScrollInfo(SB_HORZ, &si);
}

void MyWindow::OnVScroll(UINT nSBCode, UINT nPos, ScrollBarControl pScrollBar)
{
	SCROLLINFO si = {0};
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	GetScrollInfo(SB_VERT,&si);
	iVertPos = si.nPos;
	switch(nSBCode)
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
	SetScrollInfo(SB_VERT,&si);
	GetScrollInfo(SB_VERT,&si);
	if(si.nPos != iVertPos)
	{
		ScrollWindow(0,cyChar * (iVertPos - si.nPos),NULL,NULL);
		UpdateWindow();
	}
	Invalidate();
	UpdateWindow();
}

void MyWindow::OnPaint(DC dc)
{
	std::tstring buffer;
	DC hdc = BeginPaint(&ps);
	SCROLLINFO si = {0};
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_POS;
	GetScrollInfo(SB_VERT,&si);
	iVertPos = si.nPos;
	int iPaintBeg = std::max(0l,iVertPos + ps.rcPaint.top / cyChar);
	int iPaintEnd = std::min((long)(NUMLINES - 1), iVertPos + ps.rcPaint.bottom / cyChar);
	for(int i = iPaintBeg; i <= iPaintEnd; i++)
	{
		int y = cyChar * (i - iVertPos);
		hdc.TextOut(0, y, systemmetrics[i].szLabel.c_str(), systemmetrics[i].szLabel.length());
		hdc.TextOut(22 * cxCaps, y, systemmetrics[i].szDesc.c_str(), systemmetrics[i].szDesc.length());
		hdc.SetTextAlign(TA_RIGHT | TA_TOP);
		hdc.TextOut(22 * cxCaps + 40 * cxChar, y, systemmetrics[i].szVal.c_str(), systemmetrics[i].szVal.length());
		hdc.SetTextAlign(TA_LEFT | TA_TOP);
	}
	EndPaint(&ps);
}

void MyWindow::OnLButtonDown(UINT nFlags, const Point &pt)
{
    	std::tstringstream str;
        str << TEXT("Clicked At: ") << pt.x << TEXT(",") << pt.y;
        MessageBox::Show(str.str().c_str(), TEXT("WinTest"),MessageBoxButtons::Ok , MessageBoxIcon::Information);
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int nShow)
{
    MyWindow win;
    win.Create(nullptr, &Window::rcDefault, TEXT("System Metrics"));
    win.ShowWindow(nShow);
    win.UpdateWindow();

    MSG msg;

	HACCEL hAccelTable = LoadAccelerators(HINST_THISCOMPONENT, MAKEINTRESOURCE(IDC_WINTEST));

	INITCOMMONCONTROLSEX icc;

	// Initialise common controls.
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);

    while (GetMessage(&msg, nullptr, 0, 0))
    {
		if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

    }
    return msg.wParam;
}
