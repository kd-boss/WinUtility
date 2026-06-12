
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include <WinUtility/ComPtr.h>
#include <d2d1.h>
#include <d2d1helper.h>

#include "Resource.h"
#include "About.h"

class Connect : public BaseWindow<Connect, Window, FrameWinTraits>
{
	PAINTSTRUCT ps;
	ComPtr<ID2D1Factory> m_ft;
	ComPtr<ID2D1HwndRenderTarget> m_rt;
	ComPtr<ID2D1SolidColorBrush> m_brush;
	const D2D1_COLOR_F m_backColor = D2D1::ColorF(D2D1::ColorF::SkyBlue);
	std::vector<D2D1_POINT_2F> m_points;

	Size m_size;

	MyAboutDialog about;
	HRESULT Render();
	HRESULT CreateFactories();
	HRESULT CreateRenderTargetResources();
	HRESULT CreatePathGeometry(ID2D1PathGeometry** out);
	void DestoryRenderTargetResources();
	

	void OnDestroyed();
	void OnLButtonDown(UINT nFlags, const Point &pt);
	void OnLButtonUp(UINT nFlags, const Point &pt);
	void OnRButtonDown(UINT nFlags, const Point &pt);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnAbout(UINT uNotifyCode, int nID, Window wndCtl);
	void OnExit(UINT uNotifyCode, int nID, Window wndCtl);
	void OnMouseMove(UINT nFlags, Point point);
	void OnSize(UINT nFlags, Size size);
	void OnPaint(DC dc);
public:
	// wnd class delceration uses the resource ID for your project, and the small icon as it different from the large icon.
	// Name your large icon IDC_(projectname), and your small icon IDI_PROJECTNAME or something that fits your needs, just not the same
	// thing as the large icon. Provide your main projct id and small icon id to the DECLARE_WND_CLASS_1 macro and the preprocessor
	// will create the approperate functions needed by the library for you.

	/*
		At minimum, in the .rc file you'll need:
		1) a string table with the IDC_PROJECTNAME for the class name
		2) a icon with the IDI_PROJECTNAME for an ID that contains all needed sizes.
		4) a menu defined with he IDC_PROJECTNAME ID.
	*/

	DECLARE_WND_CLASS_1(IDC_CONNECT, IDI_CONNECT)

	HRESULT Initialize();

	BEGIN_MSG_MAP()
	BEGIN_COMMAND_MAP()
	COMMAND_ID_HANDLER_EX(IDM_ABOUT, OnAbout)
	COMMAND_ID_HANDLER_EX(IDOK, OnAbout)
	COMMAND_ID_HANDLER_EX(IDM_EXIT, OnExit)
	END_COMMAND_MAP()
	MSG_WM_PAINT(OnPaint)
	MSG_WM_SIZE(OnSize)
	MSG_WM_CREATE(OnCreate)
	MSG_WM_DESTROY(OnDestroyed)
	MSG_WM_MOUSEMOVE(OnMouseMove)
	MSG_WM_LBUTTONDOWN(OnLButtonDown)
	MSG_WM_LBUTTONUP(OnLButtonUp)
	END_MSG_MAP()
};
