#include "About3.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR szCmd, int iCmdShow)
{
    About3 about;
    MSG msg;
    HRESULT rs = (::IsWindow(about.Create(nullptr, &Window::rcDefault,TEXT("About3"))) ? S_OK : E_FAIL);
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
    return msg.wParam;
}