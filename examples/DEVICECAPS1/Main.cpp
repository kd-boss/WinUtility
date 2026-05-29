
#include "DEVICECAPS1.h"

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
    DEVICECAPS1 MyWindow;
    MSG msg;

    auto hr = CoInitialize(nullptr);
    if(SUCCEEDED(hr))
    {
        if(SUCCEEDED(MyWindow.Initialize()))
        {
            HACCEL hAccelTable = LoadAccelerators(HINST_THISCOMPONENT, MAKEINTRESOURCE(IDC_DEVICECAPS1));

            while(GetMessage(&msg, NULL ,0 ,0))
            {
                if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
        }
    }
    CoUninitialize();
    return msg.wParam;
}