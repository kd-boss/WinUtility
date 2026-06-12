#include "SystemMetricsD2D1.h"
#include <WinUtility/Numbers.h>

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int nShow)
{
	MSG msg;
		MyWindow win;
    	win.Create(nullptr, &Window::rcDefault, TEXT("System Metrics D2D1"));
    	win.ShowWindow(nShow);
    	win.UpdateWindow();


		HACCEL hAccelTable = LoadAccelerators(HINST_THISCOMPONENT, MAKEINTRESOURCE(IDC_SYSTEMMETRICSD2D1));

		INITCOMMONCONTROLSEX icc;

		// Initialise common controls.
		icc.dwSize = sizeof(icc);
		icc.dwICC = ICC_WIN95_CLASSES;
		InitCommonControlsEx(&icc);


		while (GetMessage(&msg, nullptr, 0, 0))
    	{	

			if(TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			continue;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
			
    	
	}

    return convert_to<int>(msg.wParam);
}
