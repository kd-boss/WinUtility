#ifndef __SINEWAVE_H__
#define __SINEWAVE_H__

#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include <WinUtility/ComPtr.h>
#include <d2d1.h>
#include <d2d1helper.h>

#include "Resource.h"
#include "About.h"

class MyWindow : public BaseWindow<MyWindow, Window, FrameWinTraits>
{
	MyAboutDialog about;
	ComPtr<ID2D1DCRenderTarget> m_renderTarget;
	ComPtr<ID2D1Factory> m_factory;
	ComPtr<ID2D1SolidColorBrush> m_blackBrush;
	ComPtr<ID2D1SolidColorBrush> m_sinewaveBrush;
	ComPtr<ID2D1PathGeometry> m_path;
	ComPtr<ID2D1GeometrySink> m_sink;
	std::vector<D2D1_POINT_2F> m_linepoints;
	void ReportError(HRESULT hr);
	void OnSize(UINT nType, Size size);
	void OnClose();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnAbout(UINT uNotifyCode, int nID, Window wndCtl);
	void OnExit(UINT uNotifyCode, int nID, Window wndCtl);
	HRESULT CreateDeviceIndependantResources();
	HRESULT CreateDeviceResources();
	void Render(const PAINTSTRUCT &ps);
	void OnPaint(DC dc);

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

	DECLARE_WND_CLASS_1(IDC_SINEWAVE, IDI_SMALL)

	HRESULT Initialize();

	BEGIN_MSG_MAP()
	BEGIN_COMMAND_MAP()
	COMMAND_ID_HANDLER_EX(IDM_ABOUT, OnAbout)
	COMMAND_ID_HANDLER_EX(IDOK, OnAbout)
	COMMAND_ID_HANDLER_EX(IDM_EXIT, OnExit)
	END_COMMAND_MAP()
	MSG_WM_CREATE(OnCreate)
	MSG_WM_CLOSE(OnClose)
	MSG_WM_SIZE(OnSize)
	MSG_WM_PAINT(OnPaint)
	END_MSG_MAP()
};
#endif