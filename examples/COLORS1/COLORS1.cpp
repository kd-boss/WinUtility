/*
    WinTest, Small Demo application that creates a user interface window.
    release build - compile with :
		winres -i wintest.rc -o wintestrc.o
        g++ wintest.cpp wintestrc.o -municode -mwindows -s -O3 -o wintest.exe
    debug build - compile with :
		winres -i wintest.rc -o wintestrc.o
        g++ wintest.cpp wintestrc.o -municode -mwindows -g -o wintest.exe
*/

#include "COLORS1.h"
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
	
		}
	}

}

void MyWindow::OnClose()
{
	DeleteObject((HBRUSH)
SetClassLongPtr(*this,GCLP_HBRBACKGROUND,(LONG_PTR)GetStockObject(WHITE_BRUSH)));
    m_scrollBackground.DestroyWindow();
	for(auto a : m_scrolls)
	{
		a.DestroyWindow();
	}
	for(auto a : m_lables)
	{
		a.DestroyWindow();
	}
	for(auto a: m_values)
	{
		a.DestroyWindow();
	}
    PostQuitMessage(0);
}

int MyWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	Rect rc;
	GetClientRect(rc);
	rc.right = rc.right / 2;
	//additional window initalization would go here.
	
	m_scrollBackground.Create(TEXT("static"),*this,nullptr,nullptr,ControlTraits::GetStyle(),ControlTraits::GetStyleEx(),UMenuOrID{9}.Get());
	
	
	ZeroMemory(&sf,sizeof(sf));
	sf.cbSize = sizeof(sf);
	sf.fMask = SIF_RANGE | SIF_POS;
	sf.nMin = 0;
	sf.nMax = 255;
	sf.nPos = 0;


	for(int i = 0; i < 3; i++)
	{
		color[i] = 0;
		ScrollBarControl s;
		m_scrolls.push_back(s);
		m_scrolls[i].Create(*this,nullptr,classlables[i].c_str(),WS_CHILD | WS_VISIBLE | SB_VERT  ,0UL, i);
		m_scrolls[i].SetScrollInfo(&sf);
		StaticControl sl;
		m_lables.push_back(sl);
		m_lables[i].Create(*this,nullptr,classlables[i].c_str(),WS_CHILD | WS_VISIBLE | SS_CENTER,0UL,i + 3);
		StaticControl vl;
		m_values.push_back(vl);
		m_values[i].Create(*this,nullptr,TEXT("255"),WS_CHILD | WS_VISIBLE | SS_CENTER,0UL,i + 6);
		
	}
	hBrushStatic = ::CreateSolidBrush(GetSysColor(COLOR_BTNHIGHLIGHT));
	cyChar = HIWORD(GetDialogBaseUnits());

    return 0;
}

void MyWindow::OnLButtonDown(UINT nFlags, const Point &pt)
{
    	auto str = std::format(TEXT("Clicked At: {:d},{:d}"), pt.x , pt.y);
        MessageBox::Show(str.c_str(), TEXT("WinTest"),MessageBoxButtons::Ok , MessageBoxIcon::Information);
}

void MyWindow::OnSize(UINT nType, Size size)
{
	Rect rc;
	GetClientRect(&rc);
	cxClient = rc.Width();
	cyClient = rc.Height();
	SetRect(rcColor, cxClient / 2, 0, cxClient, cyClient);
	m_scrollBackground.MoveWindow(0,0,cxClient/2,cyClient);

	for(int i = 0; i < 3; i++)
	{
		m_scrolls[i].MoveWindow((2 * i + 1) * cxClient / 14, 2 * cyChar, cxClient / 14, cyClient - 4 * cyChar);
		m_lables[i].MoveWindow((4 * i + 1) * cxClient / 28, cyChar / 2, cxClient / 7, cyChar);
		m_values[i].MoveWindow((4 * i + 1) * cxClient / 28, cyClient - 3 * cyChar / 2, cxClient / 7, cyChar);
	}
	
	Invalidate();
	UpdateWindow();
	SetFocus();
}

void MyWindow::OnVScroll(UINT nSBCode, UINT nPos, ScrollBarControl pScrollBar)
{
	auto i = pScrollBar.GetWindowLongW(GWL_ID);
	sf.cbSize = sizeof(sf);
	sf.fMask = SIF_ALL;
	m_scrolls[i].GetScrollInfo(&sf);
	switch(nSBCode)
	{
		case SB_PAGEDOWN:
		color[i] += 15;
		case SB_LINEDOWN:
		color[i] = std::min(255, color[i] + 1);
		break;
		case SB_PAGEUP:
		color[i] -= 15;
		case SB_LINEUP:
		color[i] = std::max(0, color[i] - 1);
		break;
		case SB_TOP:
		color[i] = 0;
		break;
		case SB_BOTTOM:
		color[i] = 255;
		break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			color[i] = std::min(255,sf.nTrackPos);
		break;
	}
	
	m_scrolls[i].SetScrollPos(color[i]);
	//invert the values. 
	auto labeltxt = std::format(TEXT("{:d}"),sf.nMax - color[i]);
	m_values[i].SetWindowTextW(labeltxt.c_str());
	DeleteObject((HBRUSH) SetClassLongPtr(*this,GCLP_HBRBACKGROUND,(LONG_PTR)CreateSolidBrush(RGB(sf.nMax - color[0],sf.nMax - color[1],sf.nMax - color[2]))));
	Invalidate();
	UpdateWindow();
}

void MyWindow::OnSysColorChange()
{
	hBrushStatic.DeleteObject();
	hBrushStatic = CreateSolidBrush(GetSysColor(COLOR_BTNHIGHLIGHT));
}


HBRUSH MyWindow::OnCtlColorStatic(DCT<true> dc, StaticControl wndStatic)
{
	auto i = wndStatic.GetWindowLong(GWL_ID);
	if(i >= 3 && i <= 9)
	{
	   dc.SetTextColor(cPrim[i % 3]);
	   dc.SetBkColor(RGB(255,255,255));
	   dc.Detach();
	   return hBrushStatic;
	}
	return nullptr;
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int nShow)
{
	
	INITCOMMONCONTROLSEX icc;

	// Initialise common controls.
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);

    MyWindow win;
    win.Create(nullptr, &Window::rcDefault, TEXT("COLORS1"));
    win.ShowWindow(nShow);
    win.UpdateWindow();

    MSG msg;

	HACCEL hAccelTable = LoadAccelerators(HINST_THISCOMPONENT, MAKEINTRESOURCE(IDC_WINTEST));

    while (GetMessage(&msg, nullptr, 0, 0))
    {
			if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}	
    }
    return static_cast<int>(msg.wParam);
}