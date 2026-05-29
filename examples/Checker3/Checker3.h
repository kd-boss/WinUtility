#ifndef CHECKER3_H
#define CHECKER3_H
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include "Resource.h"
#include "About.h"
#include "Checker3_child.h"

class Checker3 : public BaseWindow<Checker3, Window, FrameWinTraits>
{
	MyAboutDialog about;
	ComPtr<ID2D1Factory> m_factory;
	void OnClose();
	void OnLButtonDown(UINT nFlags, const Point &pt);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnAbout(UINT uNotifyCode, int nID, Window wndCtl);
	void OnExit(UINT uNotifyCode, int nID, Window wndCtl);
	void OnClear(UINT uNotifyCode, int nID, Window wndCtl);
	void OnSize(UINT nFlags, Size size);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	HRESULT CreateDeviceIndependantResources();
	Checker3_child m_windows[5][5]{};
public:


	DECLARE_WND_CLASS_1(IDC_CHECKER3, IDI_CHECKER3)

	HRESULT Initialize();

	BEGIN_MSG_MAP()
	BEGIN_COMMAND_MAP()
	COMMAND_ID_HANDLER_EX(IDM_ABOUT, OnAbout)
	COMMAND_ID_HANDLER_EX(IDOK, OnAbout)
	COMMAND_ID_HANDLER_EX(IDM_CLEAR, OnClear)
	COMMAND_ID_HANDLER_EX(IDM_EXIT, OnExit)
	END_COMMAND_MAP()
	MSG_WM_SIZE(OnSize)
	MSG_WM_CREATE(OnCreate)
	MSG_WM_CLOSE(OnClose)
	MSG_WM_KEYDOWN(OnKeyDown)
	END_MSG_MAP()
};

#endif