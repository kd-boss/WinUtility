#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include "Resource.h"

class ScrollDialog : public BaseDialog<ScrollDialog, Window>
{
    ScrollBarControl sb_red,sb_green,sb_blue;
    BOOL OnInit(Window wndFocus, LPARAM lInitParam);
    BOOL OnVScroll(UINT nSBCode, UINT nPos, ScrollBarControl pScrollBar);
    public:
    UINT IDD = COLORSCRDLG;
    DECLARE_DLG_CLASS(TEXT("ScrollDialog"))
    BEGIN_MSG_MAP()
        MSG_WM_INITDIALOG(OnInit)
        DLG_MSG_WM_VSCROLL(OnVScroll)
    END_MSG_MAP()
};