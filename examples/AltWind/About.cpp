#include "About.h"


BOOL MyAboutDialog::OnInitDialog(Window , LPARAM )
{
	return CenterWindow();
}

void MyAboutDialog::OnClose()
{
	EndDialog(IDCLOSE);
	Detach();
}

void MyAboutDialog::OnOk(UINT , int nID, Window )
{
	EndDialog(nID);
}
