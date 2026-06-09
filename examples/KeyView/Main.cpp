#include "KeyView.h"
#include "winstring.h"
#include <WinUtility/Numbers.h>

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

	    HACCEL hAccelTable = LoadAccelerators(HINST_THISCOMPONENT, MAKEINTRESOURCE(IDC_KEYVIEW));
		
        while (GetMessage(&msg, nullptr, 0, 0))
        {
			//set the HWND of TranslateAccelerator to the msg.hwnd rather than your class
			//This disables the translator from grabbing the control keys, and allows this app
			//to process them in key board events. 
	    	if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
	    	{
	    		TranslateMessage(&msg);
	    		DispatchMessage(&msg);
			}

        }
    }
    return convert_to<int>(msg.wParam);
}
