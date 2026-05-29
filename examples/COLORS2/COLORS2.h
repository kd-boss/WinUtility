
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include "ScrollDialog.h"
#include "Resource.h"
#include "About.h"

class Colors2 : public BaseWindow<Colors2, Window, FrameWinTraits>
{
  MyAboutDialog about;
  ScrollDialog  scrolldlg;

  void OnClose();
  void OnLButtonDown(UINT nFlags, const Point &pt);
  int OnCreate(LPCREATESTRUCT lpCreateStruct);
  void OnAbout(UINT uNotifyCode, int nID, Window wndCtl);
  void OnExit(UINT uNotifyCode, int nID, Window wndCtl);
	
  public:
    
	DECLARE_WND_CLASS_1(IDC_COLORS2,IDI_COLORS2)                                                      
   
    HRESULT Initialize();
	
    BEGIN_MSG_MAP()
	BEGIN_COMMAND_MAP()
	COMMAND_ID_HANDLER_EX(IDM_ABOUT, OnAbout)
	COMMAND_ID_HANDLER_EX(IDOK, OnAbout)
	COMMAND_ID_HANDLER_EX(IDM_EXIT, OnExit)
	END_COMMAND_MAP()
    MSG_WM_CREATE(OnCreate)
    MSG_WM_CLOSE(OnClose)
    MSG_WM_LBUTTONDBLCLK(OnLButtonDown)
    END_MSG_MAP()
};
