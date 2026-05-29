#include "HexCalc.h"

#include <format>


void HexCalc::OnDestroy()
{
     PostQuitMessage(0);
}

void HexCalc::OnClose()
{
    DestroyWindow();
}

void HexCalc::OnCommand(UINT nNotifyCode, int nID, Window wndCtl)
{
    SetFocus();
    if(nID == VK_BACK) 
    {
        ShowNumber(iNumber /= 16);
    }
    else if(nID == VK_ESCAPE)
    {
        ShowNumber(iNumber = 0);
    
    }
    else if(isxdigit(nID))
    {
        if(bNewNumber)
        {
            iFirstNum = iNumber;
            iNumber = 0;
        }
        bNewNumber = false;
        
        if(iNumber  <= MAXWORD >> 4)
            ShowNumber(iNumber = 16 * iNumber + nID - (isdigit(nID) ? TEXT('0') : TEXT('A') - 10));
        else
            MessageBeep(0);
    }
    else
    {
        if(!bNewNumber)
        {
            ShowNumber(iNumber = CalcIt(iFirstNum, iOperation, iNumber));
        }
        bNewNumber = true;
        iOperation = nID;
    }
}

LRESULT HexCalc::OnSetFocus(Window wndPrev)
{
    return 0;
}

void HexCalc::OnKeyUp(UINT nVirtKey, UINT nRepCntAndFlags)
{
    switch(nVirtKey)
    {
        case VK_SHIFT:
        case VK_LSHIFT:
        case VK_RSHIFT:
         Shifted = false;
        break;
    }
}

void HexCalc::OnKeyDown(UINT nVirtKey, UINT nRepCntAndFlags)
{
    switch(nVirtKey)
    {
        case VK_LEFT:
            nVirtKey = VK_BACK;
        break;
        case 107:
            nVirtKey = 61;
        break;
        case VK_SHIFT:
        case VK_LSHIFT:
        case VK_RSHIFT:
         Shifted = true;
         return;
        break;
        case 53:
            if(Shifted) nVirtKey = 37; 
        break;
        case 54:
            if(Shifted) nVirtKey = 94;
        break;
        case 55:
            if(Shifted) nVirtKey = 38;
        break;
        case 56:
            if(Shifted) nVirtKey = 42;
        break;
        case 187:
            if(Shifted) nVirtKey = 43;
            else nVirtKey = 61;
        break;
        case 188:
            if(Shifted) nVirtKey = 60;
        break;
        case 190:
            if(Shifted) nVirtKey = 62;
        break;
        case 191:
            nVirtKey = 47;
        break;
        case 111:
            nVirtKey = 47;
        break;
        case 220:
            if(Shifted) nVirtKey = 124;
        break;

    }   
    SetHandled(false);
}

void HexCalc::OnChar(TCHAR ch, UINT nRepCntAndFlags)
{
    
    if((WPARAM)CharUpper(&ch) == VK_RETURN)
        ch = TEXT('=');

    
    PushButtonControl btn { GetDlgItem(ch).m_hwnd};    
    if(btn)
    {
        btn.SetState(TRUE);
        btn.Invalidate();
        Sleep(100);
        btn.SetState(FALSE);
        btn.Invalidate();
    }
    else
    {
        MessageBeep(0);
    }
    OnCommand(nRepCntAndFlags,static_cast<int>(ch),btn.m_hwnd);
}

BOOL HexCalc::OnInitDialog(Window wndFocus)
{
    this->SetFocus();
    CenterWindow();
    return FALSE;
}

void HexCalc::ShowNumber(UINT iNumber)
{
    auto caption = std::format(TEXT("{:X}"),iNumber);
    SetDlgItemText(*this, VK_ESCAPE, caption.c_str());
}

DWORD HexCalc::CalcIt(UINT iFirstNum, int iOperation, UINT iNum)
{
    switch(iOperation)
    {
        case TEXT('='): return iNum;
        case TEXT('+'): return iFirstNum + iNum;
        case TEXT('-'): return iFirstNum - iNum;
        case TEXT('*'): return iFirstNum * iNum;
        case TEXT('&'): return iFirstNum & iNum;
        case TEXT('|'): return iFirstNum | iNum;
        case TEXT('^'): return iFirstNum ^ iNum;
        case TEXT('<'): return iFirstNum << iNum;
        case TEXT('>'): return iFirstNum >> iNum;
        case TEXT('/'): return iNum ? iFirstNum / iNum : MAXDWORD;
        case TEXT('%'): return iNum? iFirstNum % iNum : MAXWORD;
        default: return 0;
    };
}
