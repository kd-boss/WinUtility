
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include "Resource.h"

#define NUMLINES ((int) (sizeof systemmetrics / sizeof systemmetrics[0]))
struct
{
	int iIndex;
	std::tstring szLabel;
	std::tstring szDesc;
	std::tstring szVal;
} 
systemmetrics [] =
{
	SM_CXSCREEN, TEXT("SM_CXSCREEN"), TEXT("Screen width in pixels"),TEXT(""),
	SM_CYSCREEN, TEXT("SM_CYSCREEN"), TEXT("Screen height in pixels"),TEXT(""),
	SM_CXVSCROLL, TEXT("SM_CXVSCROLL"), TEXT("Vertical scroll height"),TEXT(""),
	SM_CYHSCROLL, TEXT("SM_CYHSCROLL"), TEXT("Horizontal scroll width"),TEXT(""),
	SM_CYCAPTION, TEXT("SM_CYCAPTION"), TEXT("Caption bar height"),TEXT(""),
	SM_CXBORDER, TEXT("SM_CXBORDER"), TEXT("Window border width"),TEXT(""),
	SM_CYBORDER, TEXT("SM_CYBORDER"), TEXT("Window border height"),TEXT(""),
	SM_CXFIXEDFRAME, TEXT("SM_CXFIXEDFRAME"),TEXT("Dialog window frame width"),TEXT(""),
	SM_CYFIXEDFRAME, TEXT("SM_CYFIXEDFRAME"), TEXT("Dialog window frame height"),TEXT(""),
	SM_CYVTHUMB, TEXT("SM_CYVTHUMB"),TEXT("Vertical scroll thumb height"),TEXT(""),
	SM_CXHTHUMB, TEXT("SM_CXHTHUMB"), TEXT("Horizontal scroll thumb width"),TEXT(""),
	SM_CXICON, TEXT("SM_CXICON"), TEXT("Icon width"),TEXT(""),
	SM_CYICON, TEXT("SM_CYICON"), TEXT("Icon height"),TEXT(""),
	SM_CXCURSOR, TEXT("SM_CXCURSOR"), TEXT("Cursor width"),TEXT(""),
	SM_CYCURSOR, TEXT("SM_CYCURSOR"), TEXT("Cursor height"),TEXT(""),
	SM_CYMENU, TEXT("SM_CYMENU"), TEXT("Menu bar height"),TEXT(""),
	SM_CXFULLSCREEN, TEXT("SM_CXFULLSCREEN"),TEXT("Full screen client area width"),TEXT(""),
	SM_CYFULLSCREEN, TEXT("SM_CYFULLSCREEN"), TEXT("Full screen client area height"),TEXT(""),
	SM_CYKANJIWINDOW, TEXT("SM_CYKANJIWINDOW"), TEXT("Kanji window height"),TEXT(""),
	SM_MOUSEPRESENT, TEXT("SM_MOUSEPRESENT"), TEXT("Mouse present flag"),TEXT(""),
	SM_CYVSCROLL, TEXT("SM_CYVSCROLL"), TEXT("Vertical scroll arrow height"),TEXT(""),
	SM_CXHSCROLL, TEXT("SM_CXHSCROLL"), TEXT("Horizontal scroll arrow width"),TEXT(""),
	SM_DEBUG, TEXT("SM_DEBUG"), TEXT("Debug version flag"),TEXT(""),
	SM_SWAPBUTTON, TEXT("SM_SWAPBUTTON"), TEXT("Mouse buttons swapped flag"),TEXT(""),
	SM_CXMIN, TEXT("SM_CXMIN"), TEXT("Minimum window width"),TEXT(""),
	SM_CYMIN, TEXT("SM_CYMIN"), TEXT("Minimum window height"),TEXT(""),
	SM_CXSIZE, TEXT("SM_CXSIZE"), TEXT("Min/Max/Close button width"),TEXT(""),
	SM_CYSIZE, TEXT("SM_CYSIZE"), TEXT("Min/Max/Close button height"),TEXT(""),
	SM_CXSIZEFRAME, TEXT("SM_CXSIZEFRAME"), TEXT("Window sizing frame width"),TEXT(""),
	SM_CYSIZEFRAME, TEXT("SM_CYSIZEFRAME"), TEXT("Window sizing frame height"),TEXT(""),
	SM_CXMINTRACK, TEXT("SM_CXMINTRACK"), TEXT("Minimum window tracking width"),TEXT(""),
	SM_CYMINTRACK, TEXT("SM_CYMINTRACK"), TEXT("Minimum window tracking height"),TEXT(""),
	SM_CXDOUBLECLK, TEXT("SM_CXDOUBLECLK"), TEXT("Double click x tolerence"),TEXT(""),
	SM_CYDOUBLECLK, TEXT("SM_CYDOUBLECLK"), TEXT("Double click y tolerence"), TEXT(""),
	SM_CXICONSPACING, TEXT("SM_CXICONSPACING"), TEXT("Horizontal icon spacing"),TEXT(""),
	SM_CYICONSPACING, TEXT("SM_CYICONSPACING"), TEXT("Vertical icon spacing"),TEXT(""),
	SM_MENUDROPALIGNMENT, TEXT("SM_MENUDROPALIGNMENT"), TEXT("Leftor right menu drop"),TEXT(""),
	SM_PENWINDOWS, TEXT("SM_PENWINDOWS"), TEXT("Pen extensions installed"),TEXT(""),
	SM_DBCSENABLED, TEXT("SM_DBCSENABLED"), TEXT("Double-Byte Char Set enabled"),TEXT(""),
	SM_CMOUSEBUTTONS, TEXT("SM_CMOUSEBUTTONS"), TEXT("Number of mouse buttons"),TEXT(""),
	SM_SECURE, TEXT("SM_SECURE"), TEXT("Security present flag"),TEXT(""),
	SM_CXEDGE, TEXT("SM_CXEDGE"), TEXT("3-D border width"),TEXT(""),
	SM_CYEDGE, TEXT("SM_CYEDGE"), TEXT("3-D border height"),TEXT(""),
	SM_CXMINSPACING, TEXT("SM_CXMINSPACING"),TEXT("Minimized window spacing width"),TEXT(""),
	SM_CYMINSPACING, TEXT("SM_CYMINSPACING"),TEXT("Minimized window spacing height"),TEXT(""),
	SM_CXSMICON, TEXT("SM_CXSMICON"), TEXT("Small icon width"),TEXT(""),
	SM_CYSMICON, TEXT("SM_CYSMICON"), TEXT("Small icon height"),TEXT(""),
	SM_CYSMCAPTION, TEXT("SM_CYSMCAPTION"), TEXT("Small caption height"),TEXT(""),
	SM_CXSMSIZE, TEXT("SM_CXSMSIZE"),TEXT("Small caption button width"),TEXT(""),
	SM_CYSMSIZE, TEXT("SM_CYSMSIZE"),TEXT("Small caption button height"),TEXT(""),
	SM_CXMENUSIZE, TEXT("SM_CXMENUSIZE"), TEXT("Menu bar button width"),TEXT(""),
	SM_CYMENUSIZE, TEXT("SM_CYMENUSIZE"), TEXT("Menu bar button height"),TEXT(""),
	SM_ARRANGE,  TEXT("SM_ARRANGE"), TEXT("How minimized windows arranged"),TEXT(""),
	SM_CXMINIMIZED, TEXT("SM_CXMINIMIZED"),TEXT("Mimized window width"),TEXT(""),
	SM_CYMINIMIZED, TEXT("SM_CYMINIMIZED"),TEXT("Mimized window height"),TEXT(""),
	SM_CXMAXTRACK, TEXT("SM_CXMAXTRACK"), TEXT("Maximum draggable width"),TEXT(""),
	SM_CYMAXTRACK, TEXT("SM_CYMAXTRACK"), TEXT("Maximum draggable height"),TEXT(""),
	SM_CXMAXIMIZED, TEXT("SM_CXMAXIMIZED"), TEXT("Height of maximized window"),TEXT(""),
	SM_CYMAXIMIZED, TEXT("SM_CYMAXIMIZED"), TEXT("Width of maximized window"),TEXT(""),
	SM_NETWORK, TEXT("SM_NETWORK"), TEXT("Network present flag"),TEXT(""),
	SM_CLEANBOOT, TEXT("SM_CLEANBOOT"), TEXT("How system was booted"),TEXT(""),
	SM_CXDRAG, TEXT("SM_CXDRAG"), TEXT("Avoid drag x tolerence"),TEXT(""),
	SM_CYDRAG, TEXT("SM_CYDRAG"), TEXT("Avoid drag y tolerence"),TEXT(""),
	SM_SHOWSOUNDS, TEXT("SM_SHOWSOUNDS"), TEXT("Present sounds visually"),TEXT(""),
	SM_CXMENUCHECK, TEXT("SM_CXMENUCHECK"), TEXT("Menu check-mark width"),TEXT(""),
	SM_CYMENUCHECK, TEXT("SM_CYMENUCHECK"), TEXT("Menu check-mark height"),TEXT(""),
	SM_SLOWMACHINE, TEXT("SM_SLOWMACHINE"), TEXT("Slow processor flag"),TEXT(""),
	SM_MIDEASTENABLED, TEXT("SM_MIDEASTENABLED"), TEXT("Hebrew and Aribic enabled flag"),TEXT(""),
	SM_MOUSEWHEELPRESENT,TEXT("SM_MOUSEWHEELPRESENT"), TEXT("Mouse wheel present flag"),TEXT(""),
	SM_XVIRTUALSCREEN, TEXT("SM_XVIRTUALSCREEN"), TEXT("Virtual screen x origin"),TEXT(""),
	SM_YVIRTUALSCREEN, TEXT("SM_YVIRTUALSCREEN"), TEXT("Virtual screen y origin"),TEXT(""),
	SM_CXVIRTUALSCREEN, TEXT("SM_CXVIRTUALSCREEN"), TEXT("Virtual screen width"),TEXT(""),
	SM_CYVIRTUALSCREEN, TEXT("SM_CYVIRTUALSCREEN"), TEXT("Virtual screen height"),TEXT(""),
	SM_CMONITORS, TEXT("SM_CMONITORS"), TEXT("Number of monitors"),TEXT(""),
	SM_SAMEDISPLAYFORMAT, TEXT("SM_SAMEDISPLAYFORMAT"),TEXT("Same color format flag"),TEXT("")
};

class MyAboutDialog : public BaseDialog<MyAboutDialog>
{
public:
	UINT IDD = IDD_ABOUTBOX; //to use the base dialog class, specify the IDD. 
	DECLARE_DLG_CLASS(nullptr)
	
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


class MyWindow : public BaseWindow<MyWindow, Window, FrameWinVScrollTraits>
{
  MyAboutDialog about;
  TEXTMETRIC tm;
  int cxChar, cyChar, cxCaps, iVscrollPos, cyClient, iMaxWidth,iVertPos,iHozPos;
  PAINTSTRUCT ps;
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
	
	DECLARE_WND_CLASS_1(IDC_WINTEST,IDI_WINTEST)                                                      
   
    void OnClose();
    void OnLButtonDown(UINT nFlags, const Point &pt);
    int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnAbout(UINT uNotifyCode, int nID, Window wndCtl);
	void OnExit(UINT uNotifyCode, int nID, Window wndCtl);
	void OnVScroll(UINT nSBCode, UINT nPos, ScrollBarControl pScrollBar);
	void OnPaint(DC dc);
	void OnSize(UINT nType, Size size);

    BEGIN_MSG_MAP()
    MSG_WM_CREATE(OnCreate)
    MSG_WM_CLOSE(OnClose)
    MSG_WM_LBUTTONDBLCLK(OnLButtonDown)
	MSG_WM_VSCROLL(OnVScroll)
	MSG_WM_PAINT(OnPaint)
	MSG_WM_SIZE(OnSize)
	BEGIN_COMMAND_MAP()
		COMMAND_ID_HANDLER_EX(IDM_ABOUT, OnAbout)
		COMMAND_ID_HANDLER_EX(IDOK, OnAbout)
		COMMAND_ID_HANDLER_EX(IDM_EXIT, OnExit)
	END_COMMAND_MAP()
    END_MSG_MAP()
};
