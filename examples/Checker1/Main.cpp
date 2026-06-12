#include "Checker1.h"
#include "winstring.h"

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int nShow)
{
	// Initialise common controls.
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);
	CoInitialize(nullptr);
    Checker1 win;
    MSG msg;
    if(SUCCEEDED(win.Initialize()))
    {

	    HACCEL hAccelTable = LoadAccelerators(HINST_THISCOMPONENT, MAKEINTRESOURCE(IDC_CHECKER1));

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
    return static_cast<int>(msg.wParam);
}
