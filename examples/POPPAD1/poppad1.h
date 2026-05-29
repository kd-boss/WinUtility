
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include <utility>
#include "Resource.h"
#include "About.h"

enum class PopPadControls : UINT
{
	EDIT = WM_USER + 1
};

class PopPad1 : public BaseWindow<PopPad1, Window, FrameWinTraits>
{
  MyAboutDialog about;
  EditControl m_edit;
  void OnClose();
  
  int OnCreate(LPCREATESTRUCT lpCreateStruct);
  void OnAbout(UINT uNotifyCode, int nID, Window wndCtl);
  void OnExit(UINT uNotifyCode, int nID, Window wndCtl);
  void OnEdit(UINT uNotifyCode, int nID, Window wndCtl);
  void OnCopy(UINT uNotifyCode, int nID, Window wndCtl);
  void OnCut(UINT uNotifyCode, int nID, Window wndCtl);
  void OnPaste(UINT uNotifyCode, int nID, Window wndCtl);
  void OnClear(UINT uNotifyCode, int nID, Window wndCtl);
  void OnRightJustified(UINT uNotifyCode, int nID, Window wndCtl);
  void OnLeftJustified(UINT uNotifyCode, int nID, Window wndCtl);
  void OnCenterJustified(UINT uNotifyCode, int nID, Window wndCtl);
  void OnSize(UINT nFlags, Size sz);	
  public:

	
	DECLARE_WND_CLASS_1(IDC_POPPAD1,IDI_POPPAD1)                                                      
   
    HRESULT Initialize();
	
    BEGIN_MSG_MAP()
	BEGIN_COMMAND_MAP()
	COMMAND_ID_HANDLER_EX(IDM_CUT,OnCut)
	COMMAND_ID_HANDLER_EX(IDM_COPY,OnCopy)
	COMMAND_ID_HANDLER_EX(IDM_PASTE,OnPaste)
	COMMAND_ID_HANDLER_EX(IDM_ABOUT, OnAbout)
	COMMAND_ID_HANDLER_EX(IDM_CLEAR, OnClear)
	COMMAND_ID_HANDLER_EX(IDOK, OnAbout)
	COMMAND_ID_HANDLER_EX(IDM_EXIT, OnExit)
	COMMAND_ID_HANDLER_EX(IDM_RIGHT_JUSTIFIED, OnRightJustified)
	COMMAND_ID_HANDLER_EX(IDM_LEFT_JUSTIFIED, OnLeftJustified)
	COMMAND_ID_HANDLER_EX(IDM_CENTER_JUSTIFIED, OnCenterJustified)
	COMMAND_ID_HANDLER_EX(std::to_underlying(PopPadControls::EDIT), OnEdit)
	END_COMMAND_MAP()
	MSG_WM_SIZE(OnSize)
    MSG_WM_CREATE(OnCreate)
    MSG_WM_CLOSE(OnClose)
    END_MSG_MAP()
};
