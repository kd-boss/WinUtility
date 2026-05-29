
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include <WinUtility/ComPtr.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include "Resource.h"
#include "About.h"

VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT_PTR iTimerID, DWORD dwTime);
#define ID_TIMER 1
class Beeper : public BaseWindow<Beeper, Window, FrameWinTraits>
{
	PAINTSTRUCT ps;
	bool iFlipFlop;
	ComPtr<ID2D1Factory> m_factory;
	ComPtr<ID2D1HwndRenderTarget> m_rt;
	const D2D1_COLOR_F m_color_blue = D2D1::ColorF(D2D1::ColorF::Blue);
	const D2D1_COLOR_F m_color_red = D2D1::ColorF(D2D1::ColorF::Red);
	MyAboutDialog about;
	void OnPaint(DC dc);
	void OnClose();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnAbout(UINT uNotifyCode, int nID, Window wndCtl);
	void OnExit(UINT uNotifyCode, int nID, Window wndCtl);
	HRESULT InitFactories();
	HRESULT InitDeviceResources();
	HRESULT Render();
	void DestroyDeviceResources();
	void OnSize(UINT nType, Size size);
	
public:
	void OnTimer(UINT_PTR nIDEvent);
	// wnd class delceration uses the resource ID for your project, and the small icon as it different from the large icon.
	// Name your large icon IDC_(projectname), and your small icon IDI_SMALL or something that fits your needs, just not the same
	// thing as the large icon. Provide your main projct id and small icon id to the DECLARE_WND_CLASS_1 macro and the preprocessor
	// will create the approperate functions needed by the library for you.

	/*
		At minimum, in the .rc file you'll need:
		1) a string table with the IDC_PROJECTNAME for the class name
		2) a large icon with the IDC_PROJECTNAME for an ID
		3) a small icon with the IDI_SMALL icon ID
		4) a menu defined with he IDC_PROJECTNAME ID.
	*/

	DECLARE_WND_CLASS_1(IDC_BEEPER, IDI_BEEPER)

	HRESULT Initialize();

	BEGIN_MSG_MAP()
	BEGIN_COMMAND_MAP()
	COMMAND_ID_HANDLER_EX(IDM_ABOUT, OnAbout)
	COMMAND_ID_HANDLER_EX(IDOK, OnAbout)
	COMMAND_ID_HANDLER_EX(IDM_EXIT, OnExit)
	END_COMMAND_MAP()
	MSG_WM_PAINT(OnPaint)
	MSG_WM_CREATE(OnCreate)
	MSG_WM_CLOSE(OnClose)
	MSG_WM_TIMER(OnTimer)
	MSG_WM_SIZE(OnSize)
	END_MSG_MAP()
};
