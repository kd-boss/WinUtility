#include "About.h"

 BOOL MyAbout::OnInitDialog(Window wndFocus, LPARAM lInitParam)
 {
     ShowWindow(ShowWindowType::Default);
     return CenterWindow();
 }

void MyAbout::OnOk(UINT uNotifyCode, int nID, Window wndCtl)
{
    EndDialog(nID);
}

void MyAbout::OnClose()
{
    EndDialog(IDCLOSE);  
}