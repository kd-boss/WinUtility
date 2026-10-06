#include "HexCalc.h"
#include <WinUtility/Numbers.h>

int WINAPI wWinMain([[maybe_unused]]HINSTANCE hInstance, [[maybe_unused]]HINSTANCE, [[maybe_unused]]LPTSTR lpCmd,[[maybe_unused]]int nShow)
{
    HexCalc calc;

    calc.Create(nullptr,0);
    

    MSG msg = {};
    while(GetMessage(&msg, nullptr,0,0))
    {
        if(calc.m_hwnd == nullptr || (!calc.IsDialogMessageW(&msg)  || msg.message == WM_CHAR)) 
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return convert_to<int>(msg.wParam);
}
