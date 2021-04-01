/*
    WinTest, Small Demo application that creates a user interface window.
    release build - compile with :
		winres -i wintest.rc -o wintestrc.o 
        g++ wintest.cpp wintestrc.o -municode -mwindows -s -O3 -o wintest.exe
    debug build - compile with :
		winres -i wintest.rc -o wintestrc.o 
        g++ wintest.cpp wintestrc.o -municode -mwindows -g -o wintest.exe
*/

#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <utility/System.h>
#include <utility/BaseWindow.h>
#include "Resource.h"

class MyAboutDialog : public BaseDialog<MyAboutDialog>
{
public:
	UINT IDD = IDD_ABOUTBOX; //to use the base dialog class, specify the IDD. 
	
	void OnOk(UINT uNotifyCode, int nID, Window wndCtl)
	{
		this->EndDialog(nID);
	}
	void OnClose()
	{
		this->EndDialog(IDCANCEL);
	}
	
	BEGIN_MSG_MAP()
	COMMAND_ID_HANDLER_EX(IDOK,OnOk) //Dialog messages use the COMMAND_ID_HANDLER_EX, this is for all the dialog controls. 
	MSG_WM_CLOSE(OnClose)
	END_MSG_MAP()
};


class MyWindow : public BaseWindow<MyWindow, Window, FrameWinTraits>
{
  MyAboutDialog about;
  public:
	DECLARE_WND_CLASS_1(IDC_WINTEST,IDI_SMALL)                                                      
   


    void OnClose();
    void OnLButtonDown(UINT nFlags, const Point &pt);
    int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnAbout(UINT uNotifyCode, int nID, Window wndCtl);
	void OnExit(UINT uNotifyCode, int nID, Window wndCtl);
	
    BEGIN_MSG_MAP()
	COMMAND_ID_HANDLER_EX(IDM_ABOUT, OnAbout)
	COMMAND_ID_HANDLER_EX(IDM_EXIT, OnExit)
    MSG_WM_CREATE(OnCreate)
    MSG_WM_CLOSE(OnClose)
    MSG_WM_LBUTTONDBLCLK(OnLButtonDown)
    END_MSG_MAP()
};

void MyWindow::OnExit(UINT uNotifyCode, int nID, Window wndCtl)
{
		OnClose();
}

void MyWindow::OnAbout(UINT uNotifyCode, int nID, Window wndCtl)
{		
	about.DoModal();
}

void MyWindow::OnClose()
{
    PostQuitMessage(0);
}

int MyWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    return 0;
}
void MyWindow::OnLButtonDown(UINT nFlags, const Point &pt)
{
    	std::tstringstream str;
        str << TEXT("Clicked At: ") << pt.x << TEXT(",") << pt.y;
        MessageBox(str.str().c_str(),TEXT("Mouse Position"), MB_OK);   
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPTSTR, int nShow)
{
    MyWindow win;
    win.Create(nullptr, &Window::rcDefault, TEXT("Test Win"));
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
