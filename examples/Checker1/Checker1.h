
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include <WinUtility/ComPtr.h>
#include <d2d1.h>
#include <d2d1helper.h>

#include "Resource.h"
#include "About.h"

class Checker1 : public BaseWindow<Checker1, Window, FrameWinTraits>
{
	PAINTSTRUCT ps;
	Size m_size;
	Point m_clickedPoint;
	MyAboutDialog about;
	std::pair<D2D1_RECT_F, bool> m_checkers[25];
	HRESULT Render();
	HRESULT CreateFactories();
	HRESULT CreateRenderTargetResources();
	void DestroyRenderTargetResources();
	ComPtr<ID2D1HwndRenderTarget> m_rt;
	ComPtr<ID2D1Factory> m_ft;
	ComPtr<ID2D1SolidColorBrush> m_br;

	
	void OnClose();
	void OnLButtonDown(UINT nFlags, const Point &pt);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnAbout(UINT uNotifyCode, int nID, Window wndCtl);
	void OnExit(UINT uNotifyCode, int nID, Window wndCtl);
	void OnClear(UINT uNotifyCode, int nID, Window wndCtl);
	void OnSize(UINT nFlags, Size size);
	void OnPaint(DC dc);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
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

	DECLARE_WND_CLASS_1(IDC_CHECKER1, IDI_CHECKER1)

	HRESULT Initialize();

	BEGIN_MSG_MAP()
	BEGIN_COMMAND_MAP()
	COMMAND_ID_HANDLER_EX(IDM_ABOUT, OnAbout)
	COMMAND_ID_HANDLER_EX(IDOK, OnAbout)
	COMMAND_ID_HANDLER_EX(IDM_EXIT, OnExit)
	COMMAND_ID_HANDLER_EX(IDM_CLEAR,OnClear)
	END_COMMAND_MAP()
	MSG_WM_PAINT(OnPaint)
	MSG_WM_CREATE(OnCreate)
	MSG_WM_DESTROY(OnClose)
	MSG_WM_SIZE(OnSize)
	MSG_WM_KEYDOWN(OnKeyDown)
	MSG_WM_LBUTTONDBLCLK(OnLButtonDown)
	END_MSG_MAP()
};
