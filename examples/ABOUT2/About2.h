
#include "AboutDialog.h"
#ifndef __ABOUT2_H_
#define __ABOUT2_H_

typedef WinTraits<WS_OVERLAPPEDWINDOW,0> OverLappedTraits;
class About2 : public BaseWindow<About2, Window, OverLappedTraits>
{
    AboutDialog m_about;
    int iFigure, iColor;
    COLORREF crColor;
    void OnPaint(DC dc);
    void OnDestroy();
    void OnAbout(UINT uNotifyCode, int nID, Window wndCtl);
public:
    About2() : iFigure(IDC_ELLIPSE),iColor(IDC_BLACK){}
    
    DECLARE_WND_CLASS_1(IDC_ABOUT2, IDC_ABOUT2)

    BEGIN_MSG_MAP()
    MSG_WM_PAINT(OnPaint)
    MSG_WM_DESTROY(OnDestroy)
    BEGIN_COMMAND_MAP()
    COMMAND_ID_HANDLER_EX(IDM_APP_ABOUT, OnAbout)
    END_COMMAND_MAP()
    END_MSG_MAP()
};

#endif