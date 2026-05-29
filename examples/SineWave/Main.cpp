#include "SineWave.h"

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int nShow)
{
	// Initialise common controls.
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);

    MyWindow win;
    MSG msg;
    if(SUCCEEDED(win.Initialize()))
    {
	    HACCEL hAccelTable = LoadAccelerators(HINST_THISCOMPONENT, MAKEINTRESOURCE(IDC_SINEWAVE));

        while (GetMessage(&msg, nullptr, 0, 0))
        {
	    	if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
	    	{
	    		TranslateMessage(&msg);
	    		DispatchMessage(&msg);
	    	}

        }
    }
    return msg.wParam;
}
