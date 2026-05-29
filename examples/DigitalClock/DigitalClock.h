
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include <WinUtility/ComPtr.h>
#include <d2d1.h>
#include <d2d1helper.h>

#include "Resource.h"
#include "About.h"

#define ID_TIMER 1

class DigitalClock : public BaseWindow<DigitalClock, Window, FrameWinTraits>
{
	const BOOL fSevenSegment[10][7] = {
		{1, 1, 1, 0, 1, 1, 1}, // 0
		{0, 0, 1, 0, 0, 1, 0}, // 1
		{1, 0, 1, 1, 1, 0, 1}, // 2
		{1, 0, 1, 1, 0, 1, 1}, // 3
		{0, 1, 1, 1, 0, 1, 0}, // 4
		{1, 1, 0, 1, 0, 1, 1}, // 5
		{1, 1, 0, 1, 1, 1, 1}, // 6
		{1, 0, 1, 0, 0, 1, 0}, // 7
		{1, 1, 1, 1, 1, 1, 1}, // 8
		{1, 1, 1, 1, 0, 1, 1}  // 9
	};

	const D2D1_POINT_2F ptSegment[7][6] = {
		{{7, 6}, {11, 2}, {31, 2}, {35, 6}, {31, 10}, {11, 10}},	  // top
		{{6, 7}, {10, 11}, {10, 31}, {6, 35}, {2, 31}, {2, 11}},	  // top-left
		{{36, 7}, {40, 11}, {40, 31}, {36, 35}, {32, 31}, {32, 11}},  // top-right
		{{7, 36}, {11, 32}, {31, 32}, {35, 36}, {31, 40}, {11, 40}},  // middle
		{{6, 37}, {10, 41}, {10, 61}, {6, 65}, {2, 61}, {2, 41}},	  // bot-left
		{{36, 37}, {40, 41}, {40, 61}, {36, 65}, {32, 61}, {32, 41}}, // bot-right
		{{7, 66}, {11, 62}, {31, 62}, {35, 66}, {31, 70}, {11, 70}}	  // bottom
	};

	// Colon diamond vertices
	const D2D1_POINT_2F ptColon[2][4] = {
		{{2, 21}, {6, 17}, {10, 21}, {6, 25}},
		{{2, 51}, {6, 47}, {10, 51}, {6, 55}}};

	BOOL m_f24Hour;
	BOOL m_fSuppress;
	Size m_size;
	int m_cxClient;
	int m_cyClient;
	float m_scale;
	ComPtr<ID2D1Factory> m_factory;
	ComPtr<ID2D1HwndRenderTarget> m_rt;
	ComPtr<ID2D1SolidColorBrush> m_brush;

	MyAboutDialog about;
	HRESULT CreateDeviceIndependantResources();
	HRESULT CreateDeviceResources();
	void DiscardDeviceResources();
	void DisplayTime(BOOL f24Hour, BOOL fSuppress);
	void DisplayColon(float &shiftX);
	void DisplayTwoDigits(int iNumber, BOOL fSuppress, float &shiftX);
	void DisplayDigit(int iNumber, float shiftX);
	void ApplyTransform(float shiftX);
	void FillPolygon(const D2D1_POINT_2F *pts, UINT count);
	void OnClose();
	void OnDestroy();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnAbout(UINT uNotifyCode, int nID, Window wndCtl);
	void OnExit(UINT uNotifyCode, int nID, Window wndCtl);
	void OnSize(UINT nType, Size size);
	void OnTimer(UINT_PTR nIDEvent);
	void OnPaint(DC dc);
	void OnSettingsChanged(UINT uiAction, LPCTSTR ActionName);
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

	DECLARE_WND_CLASS_1(IDC_DIGITALCLOCK, IDI_DIGITALCLOCK)

	HRESULT Initialize();

	BEGIN_MSG_MAP()
	BEGIN_COMMAND_MAP()
	COMMAND_ID_HANDLER_EX(IDM_ABOUT, OnAbout)
	COMMAND_ID_HANDLER_EX(IDOK, OnAbout)
	COMMAND_ID_HANDLER_EX(IDM_EXIT, OnExit)
	END_COMMAND_MAP()
	MSG_WM_SETTINGCHANGE(OnSettingsChanged)
	MSG_WM_PAINT(OnPaint)
	MSG_WM_TIMER(OnTimer)
	MSG_WM_SIZE(OnSize)
	MSG_WM_CREATE(OnCreate)
	MSG_WM_CLOSE(OnClose)
	MSG_WM_DESTROY(OnDestroy)
	END_MSG_MAP()
};
