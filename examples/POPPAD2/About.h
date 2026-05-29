#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include "Resource.h"

class MyAboutDialog : public BaseDialog<MyAboutDialog>
{
public:
	UINT IDD = IDD_ABOUTBOX; // to use the base dialog class, specify the IDD.

	LRESULT OnOk(UINT uNotifyCode, int nID, Window wndCtl);
	void OnClose();
	BOOL OnInitDialog(Window wndFocus);

	DECLARE_DLG_CLASS(nullptr)

	BEGIN_MSG_MAP()
	DLG_WM_INITDIALOG(OnInitDialog)
	DLG_WM_CLOSE(OnClose)
	DLG_BEGIN_COMMAND_MAP()
		DLG_COMMAND_ID_HANDLER_EX(IDOK, OnOk) // Dialog messages use the COMMAND_ID_HANDLER_EX, this is for all the dialog controls.
	DLG_END_COMMAND_MAP()
	END_MSG_MAP()
};
