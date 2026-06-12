
#ifndef WINVER
#define WINVER 0x0A00
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif

#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include "Resource.h"

class MyAbout : public BaseDialog<MyAbout>
{
    BOOL OnInitDialog(Window wndFocus, LPARAM lInitParam);
    void OnOk(UINT uNotifyCode, int nID, Window wndCtl);
    void OnClose();

public:
    UINT IDD = IDD_ABOUTBOX;
    DECLARE_DLG_CLASS(TEXT("MyAbout"))
    
    BEGIN_MSG_MAP()
        MSG_WM_INITDIALOG(OnInitDialog);
        MSG_WM_CLOSE(OnClose);
        BEGIN_COMMAND_MAP()
            COMMAND_ID_HANDLER_EX(IDOK,OnOk) 
        END_COMMAND_MAP()
    END_MSG_MAP()
};