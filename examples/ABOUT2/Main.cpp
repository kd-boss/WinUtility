#include "About2.h"
#include <WinUtility/Numbers.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR szCmd, int iCmdShow)
{
    About2 about;
    MSG msg;
    HRESULT rs = (::IsWindow(about.Create(nullptr, &Window::rcDefault,TEXT("About2"))) ? S_OK : E_FAIL);
    if(SUCCEEDED(rs))
    {
        about.ShowWindow(ShowWindowType::Normal);
        about.UpdateWindow();
        while(GetMessage(&msg,nullptr,0,0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return convert_to<int>(msg.wParam);
}