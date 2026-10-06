#include "HexCalc.h"
#include <WinUtility/Numbers.h>

#include <format>
#include <print>

void HexCalc::OnDestroy()
{
     PostQuitMessage(0);
}

void HexCalc::OnClose()
{
    DestroyWindow();
}

void HexCalc::OnCommand([[maybe_unused]]UINT nNotifyCode, [[maybe_unused]]int nID, [[maybe_unused]]Window wndCtl)
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
            ShowNumber(iNumber = 16 * iNumber + Number<int>(nID) - (isdigit(nID) ? TEXT('0') : TEXT('A') - 10));
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

LRESULT HexCalc::OnSetFocus([[maybe_unused]]Window wndPrev)
{
    return 0;
}

void HexCalc::OnKeyUp([[maybe_unused]]UINT nVirtKey, [[maybe_unused]]UINT nRepCntAndFlags)
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

void HexCalc::OnKeyDown([[maybe_unused]]UINT nVirtKey, [[maybe_unused]]UINT nRepCntAndFlags)
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

void HexCalc::OnChar([[maybe_unused]]TCHAR ch, [[maybe_unused]]UINT nRepCntAndFlags)
{
    
    if(reinterpret_cast<WPARAM>(CharUpper(&ch)) == VK_RETURN)
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

BOOL HexCalc::OnInitDialog([[maybe_unused]]Window wndFocus)
{
    this->SetFocus();
    CenterWindow();
    UpdateWindow();
    
    }

    return FALSE;
}

void HexCalc::ShowNumber([[maybe_unused]] UINT lNumber)
{
    auto caption = std::format(TEXT("{:X}"),lNumber);
    SetDlgItemText(*this, VK_ESCAPE, caption.c_str());
	iNumber = lNumber;
}

DWORD HexCalc::CalcIt([[maybe_unused]]UINT lFirstNum, [[maybe_unused]]int lOperation, [[maybe_unused]]UINT iNum)
{
	iFirstNum = lFirstNum;
	iOperation = lOperation;
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
