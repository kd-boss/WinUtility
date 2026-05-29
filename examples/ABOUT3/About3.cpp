#include "About3.h"

 int About3::OnCreate(LPCREATESTRUCT lpCreateStruct)
 {

    return 0;
 }

 BOOL About3::OnMenuAbout(UINT uNotifyCode, int nID, Window wndCtl)
 {
        auto res = m_dialog.DoModal();
        if(res == IDOK)
        {
            MessageBox::Show(TEXT("OK Pressed!"),TEXT("About3"),MessageBoxButtons::Ok, MessageBoxIcon::Information);
        }
        else if(res == IDCANCEL)
        {
            MessageBox::Show(TEXT("CANCEL Pressed!"),TEXT("About3"),MessageBoxButtons::Ok, MessageBoxIcon::Information);
        }
        return TRUE;
 }

 void About3::OnClose()
 {
    DestroyWindow();
 }

 void About3::OnDestroy()
 {
    PostQuitMessage(0);
 }