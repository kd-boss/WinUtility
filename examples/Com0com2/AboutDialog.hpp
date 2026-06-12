#pragma once
// AboutDialog.hpp
//
// Self-contained About dialog implementation.
//
// Usage from MainWindow::OnMenuAbout:
//   AboutDialog::Show(m_hwnd);
//
// Requirements:
//   • com0com.rc compiled and linked (provides IDD_ABOUT, IDI_APP)
//   • commctrl.h / comctl32.lib (SysLink control, ICC_LINK_CLASS)
//   • shell32.lib  (ShellExecuteW)

#include "resource.h"
#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include <windows.h>
#include <commctrl.h>
#include <shellapi.h>

class AboutDialog : public BaseDialog<AboutDialog>
{
public:

    UINT IDD = IDD_ABOUT;
    
    BOOL OnInitDialog(Window wndFocus);
    LRESULT OnLinkClick(NMHDR& pnmh);
    void OnClose(UINT uNotifyCode, int nID, Window wndCtl);
    void OnDestroy();

    DECLARE_DLG_CLASS(TEXT("AboutBox"))

    BEGIN_MSG_MAP()
        DLG_WM_INITDIALOG(OnInitDialog)
        DLG_BEGIN_NOTIFY_MAP()
            DLG_BEGIN_NOTIFY_CODE_MAP(IDC_ABOUT_LINK)
                DLG_NOTIFY_CODE_HANDLER(NM_CLICK, OnLinkClick)
                DLG_NOTIFY_CODE_HANDLER(NM_RETURN, OnLinkClick)
            DLG_END_NOTIFY_CODE_MAP()
        END_NOTIFY_MAP()
        DLG_BEGIN_COMMAND_MAP()
            DLG_COMMAND_ID_HANDLER_EX(IDC_ABOUT_CLOSE,OnClose)
            DLG_COMMAND_ID_HANDLER_EX(IDCANCEL, OnClose);
        DLG_END_COMMAND_MAP()
        MSG_WM_DESTROY(OnDestroy)
    END_MSG_MAP()

private:
   
    StaticControl m_iconControl;
    HICON m_icon;
};
