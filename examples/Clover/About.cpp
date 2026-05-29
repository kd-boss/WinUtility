#include "About.h"


BOOL MyAboutDialog::OnInitDialog(Window wndFocus, LPARAM lInitParam)
{
	return CenterWindow();
}

void MyAboutDialog::OnClose()
{
	EndDialog(IDCLOSE);
	Detach();
}

void MyAboutDialog::OnOk(UINT uNotifyCode, int nID, Window wndCtl)
{
	EndDialog(nID);
}
