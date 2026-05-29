#ifndef _HEXCALC_
#define _HEXCALC_

#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include "Resource.h"

typedef WinTraits<WS_VISIBLE | WS_OVERLAPPED, 0> simpleTraits;

class HexCalc : public BaseDialog<HexCalc,Window>
{
    bool bNewNumber = true;
    bool Shifted = false;
    int iOperation = '=';
    UINT iNumber, iFirstNum;
    
    void OnClose();
    void OnDestroy();
    void ShowNumber(UINT iNumber);
    DWORD CalcIt(UINT iFirstNum, int iOperation, UINT iNum);
    BOOL OnInitDialog(Window wndFocus);
    LRESULT OnSetFocus(Window wndPrev);
    void OnChar(TCHAR ch, UINT nRepCntAndFlags);
    void OnKeyDown(UINT nVirtKey, UINT nRepCntAndFlags);
    void OnKeyUp(UINT nVirtKey, UINT nRepCntAndFlags);
    void OnCommand(UINT nNotifyCode, int nID, Window wndCtl);
public:

    UINT IDD = HEXCALC;

    DECLARE_DLG_CLASS(nullptr)

    BEGIN_MSG_MAP()
        DLG_WM_INITDIALOG(OnInitDialog)
        DLG_WM_SETFOCUS(OnSetFocus)
        DLG_WM_KEYDOWN(OnKeyDown)
        DLG_WM_KEYUP(OnKeyUp)
        DLG_WM_CHAR(OnChar)
        DLG_WM_COMMAND(OnCommand)
        DLG_WM_CLOSE(OnClose)
        DLG_WM_DESTROY(OnDestroy)  
    END_MSG_MAP()

};
#endif
