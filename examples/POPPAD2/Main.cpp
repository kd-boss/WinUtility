#include "poppad2.h"
#include "winstring.h"

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int nShow)
{
	// Initialise common controls.
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	MSG msg;

	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	InitCommonControlsEx(&icc);

	PopPad2 win;
	if (SUCCEEDED(win.Initialize()))
	{

		HACCEL hAccelTable = LoadAccelerators(HINST_THISCOMPONENT, MAKEINTRESOURCE(IDC_POPPAD2));

		while (GetMessage(&msg, nullptr, 0, 0))
		{
			if (!TranslateAccelerator(win, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	CoUninitialize();
	return msg.wParam;
}
