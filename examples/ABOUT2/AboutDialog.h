

#include "Resource.h"
#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#ifndef __ABOUT_DIALOG_H
#define __ABOUT_DIALOG_H



class AboutDialog : public BaseDialog<AboutDialog, Window>
{    
    const COLORREF crColor[8] = {
        RGB(0,0,0),
        RGB(0,0,255),
        RGB(0,255,0),
        RGB(0,255,255),
        RGB(255,0,0),
        RGB(255,0,255),
        RGB(255,255,0),
        RGB(255,255,255)
    };
    
    COLORREF crCurrentColor;

    void OnOk(UINT uNotifyCode, INT nId, Window wndCtl);
    void OnCancel(UINT uNotifyCode, INT nId, Window wndCtl);
    void OnColor(UINT uNotifyCode, INT nId, Window wndCtl);
    void OnFigure(UINT uNotifyCode, INT nId, Window wndCtl);
    void OnClose();
    BOOL OnInitDialog(Window focused, LPARAM initParam);
    
public:
    UINT IDD = IDC_ABOUT2;
    int  iCurrentFigure,iCurrentColor;
    void PaintWindow(HWND hwnd, int iColor, int iFigure);
    DECLARE_WND_CLASS(TEXT("About2AboutDialog"))
    BEGIN_MSG_MAP()
    MSG_WM_INITDIALOG(OnInitDialog)
    BEGIN_COMMAND_MAP()
        COMMAND_ID_HANDLER_EX(IDOK, OnOk)
        COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
        COMMAND_ID_HANDLER_EX(IDC_BLACK,OnColor)
        COMMAND_ID_HANDLER_EX(IDC_RED,OnColor)
        COMMAND_ID_HANDLER_EX(IDC_CYAN,OnColor)
        COMMAND_ID_HANDLER_EX(IDC_GREEN,OnColor)
        COMMAND_ID_HANDLER_EX(IDC_YELLOW,OnColor)
        COMMAND_ID_HANDLER_EX(IDC_BLUE,OnColor)
        COMMAND_ID_HANDLER_EX(IDC_MAGENTA,OnColor)
        COMMAND_ID_HANDLER_EX(IDC_WHITE,OnColor)
        COMMAND_ID_HANDLER_EX(IDC_RECT,OnFigure)
        COMMAND_ID_HANDLER_EX(IDC_ELLIPSE,OnFigure)
    END_COMMAND_MAP()
    END_MSG_MAP()

};

#endif