/*
    WinTest, Small Demo application that creates a user interface window.
    release build - compile with :
		winres -i wintest.rc -o wintestrc.o 
        g++ wintest.cpp wintestrc.o -municode -mwindows -s -O3 -o wintest.exe
    debug build - compile with :
		winres -i wintest.rc -o wintestrc.o 
        g++ wintest.cpp wintestrc.o -municode -mwindows -g -o wintest.exe
*/

#include "wintest.h"


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
    return 0;
}

void MyWindow::OnLButtonDown(UINT nFlags, const Point &pt)
{
    	std::tstringstream str;
        str << TEXT("Clicked At: ") << pt.x << TEXT(",") << pt.y;
        MessageBox::Show(str.str().c_str(), TEXT("WinTest"),MessageBoxButtons::Ok , MessageBoxIcon::Information);   
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPTSTR, int nShow)
{
    MyWindow win;
    win.Create(nullptr, &Window::rcDefault, TEXT("Test Window"));
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
    return msg.wParam;
}
