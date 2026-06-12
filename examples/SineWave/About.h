#ifndef __ABOUT_H__
#define __ABOUT_H__

#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include "Resource.h"

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

#endif