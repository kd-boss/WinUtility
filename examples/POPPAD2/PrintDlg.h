#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include "Resource.h"

class MyPrintDlg : public BaseDialog<MyPrintDlg>
{
    LRESULT OnCancel(UINT uNotifyCode, int nID, Window wndCtl);
public:
    UINT IDD = PRINTDLGBOX;
    DECLARE_DLG_CLASS(nullptr)

    BEGIN_MSG_MAP()
        DLG_BEGIN_COMMAND_MAP()
            DLG_COMMAND_ID_HANDLER_EX(IDCANCEL,OnCancel)
        DLG_END_COMMAND_MAP()
    END_MSG_MAP()
};