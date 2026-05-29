#include "wintest.h"
#include "winstring.h"

template<typename T>
class CoInitHelperImpl
{
	public:

	CoInitHelperImpl(const CoInitHelperImpl& other) = delete;
	CoInitHelperImpl() = default;
	CoInitHelperImpl& operator=(CoInitHelperImpl& other) = delete;
	CoInitHelperImpl& operator=(CoInitHelperImpl&& other) = delete;

	HRESULT Initalize(LPVOID pvoidReserved, DWORD init)
	{
		return CoInitializeEx(pvoidReserved, init);
	}
	
	~CoInitHelperImpl()
	{
		CoUninitialize();
	}
};

using CoInitHelper = CoInitHelperImpl<void>;

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int nShow)
{
	MSG msg;
	CoInitHelper helper;
	if(SUCCEEDED(helper.Initalize(NULL, tagCOINIT::COINIT_APARTMENTTHREADED | tagCOINIT::COINIT_SPEED_OVER_MEMORY)))
	{
		// Initialise common controls.
		INITCOMMONCONTROLSEX icc;
		icc.dwSize = sizeof(icc);
		icc.dwICC = ICC_WIN95_CLASSES;
		if (InitCommonControlsEx(&icc) == TRUE)
		{
			MyWindow win{};
			if (SUCCEEDED(win.Initialize()))
			{

				HACCEL hAccelTable = LoadAccelerators(HINST_THISCOMPONENT, MAKEINTRESOURCE(IDC_WINTEST));

				while (GetMessage(&msg, nullptr, 0, 0))
				{
					if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}
			}
		}	
	}
    return static_cast<int>(msg.wParam);
}
