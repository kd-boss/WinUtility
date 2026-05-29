#include "Resource.h"
#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include "EllipseButton.h"

class AboutDialog : public BaseDialog<AboutDialog, Window>
{
    EllipseButton btn;
    EllipseButton cancel;
    BOOL OnOK(UINT uNotifyCode, int nID, Window wndCtl);
    BOOL OnCancel(UINT uNotifyCode, int nID, Window wndCtl);
    BOOL OnInitDialog(Window wndFocus, LPARAM lInitParam);
    
    public:
    UINT IDD = IDC_ABOUT3;
    DECLARE_DLG_CLASS(nullptr)

    BEGIN_MSG_MAP()        
        MSG_WM_INITDIALOG(OnInitDialog)
        BEGIN_COMMAND_MAP()
         DLG_COMMAND_ID_HANDLER_EX(IDOK, OnOK)
         DLG_COMMAND_ID_HANDLER_EX(IDCANCEL,OnCancel)
        END_COMMAND_MAP()
    END_MSG_MAP();
};
