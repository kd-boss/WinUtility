#include "GdiInterop.h"

int WINAPI wWinMain(HINSTANCE,HINSTANCE, LPTSTR,int)
{
    
                INITCOMMONCONTROLSEX icc;

                // Initialise common controls.
                icc.dwSize = sizeof(icc);
                icc.dwICC = ICC_WIN95_CLASSES;
                InitCommonControlsEx(&icc);

    MSG msg;
    if(SUCCEEDED(CoInitialize(nullptr)))
    {
        GdiInterop m_window;
        if(SUCCEEDED(m_window.Initalize()))
        {
            while(GetMessage(&msg,nullptr,0,0))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            CoUninitialize();
        }
        
    }
    return msg.wParam;
}