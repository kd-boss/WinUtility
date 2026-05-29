#include "About.h"

BOOL MyAboutDialog::OnInitDialog(Window wndFocus)
{
	return CenterWindow();
}

void MyAboutDialog::OnClose()
{
	EndDialog(IDCLOSE);
	Detach();
}

LRESULT MyAboutDialog::OnOk(UINT uNotifyCode, int nID, Window wndCtl)
{
	EndDialog(nID);
	return 0;
}
