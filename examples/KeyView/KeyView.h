
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include <WinUtility/ComPtr.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

#include "Resource.h"
#include "About.h"
#include <atomic>

class MyWindow : public BaseWindow<MyWindow, Window, FrameWinTraits>
{
	PAINTSTRUCT ps;
	MyAboutDialog about;
	Rect rc;
	std::vector<std::tstring> messageNames = {
		TEXT("WM_KEYDOWN"), TEXT("WM_KEYUP"),
		TEXT("WM_CHAR"), TEXT("WM_DEADCHAR"),
		TEXT("WM_SYSKEYDOWN"), TEXT("WM_SYSKEYUP"),
		TEXT("WM_SYSCHAR"), TEXT("WM_SYSDEADCHAR")};

	std::tstring szTop = TEXT(" Message        Key       Char     ")
		TEXT("Repeat Scan Ext ALT Prev Tran");
	std::tstring szUnd = TEXT(" _______        ___       ____     ")
		TEXT("______ ____ ___ ___ ____ ____");

	// Character cell size in DIPs (filled from the text format)
	float g_cxChar;
	float g_cyChar;

	// Client area in pixels
	int g_cxClient;
	int g_cyClient;

	// Circular / shifting message log
	int g_cLinesMax;
	int g_cLines;
	int visibleRows;
	
	std::vector<ComPtr<IDWriteTextLayout>> m_layouts;
	ComPtr<ID2D1Factory> m_ft;
	ComPtr<ID2D1HwndRenderTarget> m_rt;
	ComPtr<ID2D1SolidColorBrush> m_brush;

	ComPtr<IDWriteFactory> m_dw;
	ComPtr<IDWriteTextFormat> m_tf;
	
	HRESULT CreateDeviceIndependantResources();
	HRESULT CreateDeviceResources();
	void DiscardDeviceResources();
	HRESULT FormatKeyMessage(const std::tstring& msgName,WPARAM wParam, LPARAM lParam,const std::tstring& keyName);
	HRESULT FormatCharMessage(const std::tstring& msgName,WPARAM wParam, LPARAM lParam);
	void RenderLine(const std::tstring &text, float x, float y);
	HRESULT Render();
	void OnClose();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnAbout(UINT uNotifyCode, int nID, Window wndCtl);
	void OnExit(UINT uNotifyCode, int nID, Window wndCtl);
	void OnClear(UINT uNotifyCode, int nID, Window wndCtl);
	void OnKeyDown(UINT nChar, SHORT nRepCnt, SHORT nFlags);
	void OnKeyUp(UINT nChar, SHORT nRepCnt, SHORT nFlags);
	void OnChar(TCHAR nChar, SHORT nRepCnt, SHORT nFlags);
	void OnSysKeyUp(UINT nChar, SHORT nRepCnt, SHORT nFlags);
	void OnSysKeyDown(UINT nChar, SHORT nRepCnt, SHORT nFlags);
	void OnSysChar(TCHAR nChar, SHORT nRepCnt, SHORT nFlags);
	void OnDeadChar(TCHAR nChar,SHORT nRepCnt, SHORT nFlags);
	void OnSysDeadChar(TCHAR nChar,SHORT nRepCnt, SHORT nFlags);
	void OnPaint(DC dc);
	void OnSize(UINT nType, Size size);
	void OnGetMinMaxInfo(LPMINMAXINFO lpMMI);
	std::tstring LoadTextResource(UINT ID);
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

	DECLARE_WND_CLASS_1(IDC_KEYVIEW, IDI_SMALL)

	HRESULT Initialize();

	BEGIN_MSG_MAP()
	BEGIN_COMMAND_MAP()
	COMMAND_ID_HANDLER_EX(IDM_ABOUT, OnAbout)
	COMMAND_ID_HANDLER_EX(IDOK, OnAbout)
	COMMAND_ID_HANDLER_EX(IDM_EXIT, OnExit)
	COMMAND_ID_HANDLER_EX(IDM_CLEAR,OnClear)
	END_COMMAND_MAP()
	MSG_WM_GETMINMAXINFO(OnGetMinMaxInfo)
	MSG_WM_CREATE(OnCreate)
	MSG_WM_CLOSE(OnClose)
	MSG_WM_KEYDOWN(OnKeyDown)
	MSG_WM_CHAR(OnChar)
	MSG_WM_KEYUP(OnKeyUp)
	MSG_WM_PAINT(OnPaint)
	MSG_WM_SIZE(OnSize)
	MSG_WM_SYSKEYUP(OnSysKeyUp)
	MSG_WM_SYSKEYDOWN(OnSysKeyDown)
	MSG_WM_SYSCHAR(OnSysChar)
	MSG_WM_DEADCHAR(OnDeadChar)
	MSG_WM_SYSDEADCHAR(OnSysDeadChar)
	END_MSG_MAP()
};
