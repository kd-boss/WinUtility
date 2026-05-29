
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include "Resource.h"


class MyAboutDialog : public BaseDialog<MyAboutDialog>
{
public:
	UINT IDD = IDD_ABOUTBOX; //to use the base dialog class, specify the IDD. 
	
	void OnOk(UINT uNotifyCode, int nID, Window wndCtl);
	void OnClose();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(Window wndFocus, LPARAM lInitParam);
	
	BEGIN_MSG_MAP()
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
	BEGIN_COMMAND_MAP()
		COMMAND_ID_HANDLER_EX(IDOK,OnOk) //Dialog messages use the COMMAND_ID_HANDLER_EX, this is for all the dialog controls. 
	END_COMMAND_MAP()
	END_MSG_MAP()
};


class MyWindow : public BaseWindow<MyWindow, Window, FrameWinTraits>
{
  MyAboutDialog about;
  
  Window m_scrollBackground;
  
  std::vector<ScrollBarControl> m_scrolls;
  std::vector<StaticControl> m_lables;
  std::vector<StaticControl> m_values;
  int cyChar, cyClient, cxClient, cxChar, color[3];
  Rect rcColor;
  Brush hBrushStatic;
  std::vector<COLORREF> cPrim {RGB(255,0,0), RGB(0,255,0),RGB(0,0,255)};
  std::vector<std::tstring> classlables {TEXT("Red"),TEXT("Green"),TEXT("Blue")};
  SCROLLINFO sf;
  public:
    //wnd class delceration uses the resource ID for your project, and the small icon as it different from the large icon.
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
	void OnSize(UINT nType, Size size);
	void OnVScroll(UINT nSBCode, UINT nPos, ScrollBarControl pScrollBar);
	void OnSysColorChange();
	HBRUSH OnCtlColorStatic(DCT<true> dc, StaticControl wndStatic);

    BEGIN_MSG_MAP()
	BEGIN_COMMAND_MAP()
		COMMAND_ID_HANDLER_EX(IDM_ABOUT, OnAbout)
		COMMAND_ID_HANDLER_EX(IDOK, OnAbout)
		COMMAND_ID_HANDLER_EX(IDM_EXIT, OnExit)
	END_COMMAND_MAP()
    	MSG_WM_CREATE(OnCreate)
    	MSG_WM_CLOSE(OnClose)
    	MSG_WM_LBUTTONDBLCLK(OnLButtonDown)
		MSG_WM_SIZE(OnSize)
		MSG_WM_VSCROLL(OnVScroll)
		MSG_WM_SYSCOLORCHANGE(OnSysColorChange)
		MSG_WM_CTLCOLORSTATIC(OnCtlColorStatic)
    END_MSG_MAP()
};
