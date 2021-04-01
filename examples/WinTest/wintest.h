
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <utility/System.h>
#include <utility/BaseWindow.h>
#include "Resource.h"

class MyAboutDialog : public BaseDialog<MyAboutDialog>
{
public:
	UINT IDD = IDD_ABOUTBOX; //to use the base dialog class, specify the IDD. 
	
	void OnOk(UINT uNotifyCode, int nID, Window wndCtl);
	void OnClose();

	BEGIN_MSG_MAP()
	COMMAND_ID_HANDLER_EX(IDOK,OnOk) //Dialog messages use the COMMAND_ID_HANDLER_EX, this is for all the dialog controls. 
	MSG_WM_CLOSE(OnClose)
	END_MSG_MAP()
};


class MyWindow : public BaseWindow<MyWindow, Window, FrameWinTraits>
{
  MyAboutDialog about;
  public:
    //wnd class delceration uses the resource ID for your project, and the small icon as it differnt from the large icon.
	//Name your large icon IDC_(projectname), and your small icon IDI_SMALL or something that fits your needs, just not the same
	//thing as the large icon. Provide your main projct id and small icon id to the DECLARE_WND_CLASS_1 macro and the preprocessor
	//will create the approperate functions needed by the library for you. 
	
	/*
		At minimum, in the .rc file you'll need:
		1) a string table with the IDC_PROJECTNAME for the class name
		2) a large icon with the IDC_PROJECTNAME for an ID
		3) a small icon with the IDI_SMALL icon ID
		4) a menu defined with he IDC_PROJECTNAME ID. 
	*/
	
	DECLARE_WND_CLASS_1(IDC_WINTEST,IDI_SMALL)                                                      
   
    void OnClose();
    void OnLButtonDown(UINT nFlags, const Point &pt);
    int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnAbout(UINT uNotifyCode, int nID, Window wndCtl);
	void OnExit(UINT uNotifyCode, int nID, Window wndCtl);
	
    BEGIN_MSG_MAP()
	COMMAND_ID_HANDLER_EX(IDM_ABOUT, OnAbout)
	COMMAND_ID_HANDLER_EX(IDOK, OnAbout)
	COMMAND_ID_HANDLER_EX(IDM_EXIT, OnExit)
    MSG_WM_CREATE(OnCreate)
    MSG_WM_CLOSE(OnClose)
    MSG_WM_LBUTTONDBLCLK(OnLButtonDown)
    END_MSG_MAP()
};
