
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include "Resource.h"
#include "About.h"
#include <utility>

enum class BtnLookControls : UINT
{
	OWNERDRAW = IDC_BTNLOOK + 1,
	TRISTATEBUTTON,
	CHECKBOX,
	SPLITBUTTON,
	GROUPBOX,
	DEFPUSHBUTTON,
	COMMANDLINK,
	PUSHBUTTON,
	AUTORADIO,
	AUTOTRISTATE,
	AUTOCHECKBOX
};

class BtnLook : public BaseWindow<BtnLook, Window, FrameWinTraits>
{
  MyAboutDialog about;
  OwnerDrawButtonControl 		m_ownerdraw;
  TriStateButtonControl 		m_triStateBtn;
  CheckBoxControl 				m_checkBoxCtrl;
  SplitButtonControl 			m_SplitButtonControl;
  GroupBoxControl 				m_groupBox;
  DefPushButtonControl 			m_DefPushButton;
  PushButtonControl 			m_PushButtonCtrl;
  CommandLinkControl 			m_commandLink;
  AutoRadioButtonControl 		m_autoRadioButton;
  AutoTriCheckBoxControl 		m_autoTriCheck;
  AutoCheckBoxControl 			m_AutoCheckBox;

  void OnClose();
  void OnLButtonDown(UINT nFlags, const Point &pt);
  int OnCreate(LPCREATESTRUCT lpCreateStruct);
  void OnAbout(UINT uNotifyCode, int nID, Window wndCtl);
  void OnExit(UINT uNotifyCode, int nID, Window wndCtl);
  void OnDefPushButton(UINT uNotifyCode, int nID, Window wndCtl);
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
	
	DECLARE_WND_CLASS_1(IDC_BTNLOOK,IDI_BTNLOOK)                                                      
   
    HRESULT Initialize();
	
    BEGIN_MSG_MAP()
	BEGIN_COMMAND_MAP()
	COMMAND_ID_HANDLER_EX(IDM_ABOUT, OnAbout)
	COMMAND_ID_HANDLER_EX(IDOK, OnAbout)
	COMMAND_ID_HANDLER_EX(IDM_EXIT, OnExit)
	COMMAND_ID_HANDLER_EX(std::to_underlying(BtnLookControls::DEFPUSHBUTTON), OnDefPushButton)
	END_COMMAND_MAP()
    MSG_WM_CREATE(OnCreate)
    MSG_WM_CLOSE(OnClose)
    MSG_WM_LBUTTONDBLCLK(OnLButtonDown)
    END_MSG_MAP()
};
