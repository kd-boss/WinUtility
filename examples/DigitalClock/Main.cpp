#include "DigitalClock.h"
#include "winstring.h"
#include <WinUtility/Numbers.h>

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int nShow)
{
	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
	// Initialise common controls.
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);

    DigitalClock win;
    MSG msg;
    if(SUCCEEDED(win.Initialize()))
    {

	    HACCEL hAccelTable = LoadAccelerators(HINST_THISCOMPONENT, MAKEINTRESOURCE(IDC_DIGITALCLOCK));

        while (GetMessage(&msg, nullptr, 0, 0))
        {
	    	if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
	    	{
	    		TranslateMessage(&msg);
	    		DispatchMessage(&msg);
	    	}

        }
    }
	CoUninitialize();
    return convert_to<int>(msg.wParam);
}
