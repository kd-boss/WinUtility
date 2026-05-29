
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include <WinUtility/ComPtr.h>
#include "CustomScrollBar.h"
#include "DialControl.h"
#include "D2DEditControl.h"
#include "Resource.h"
#include "About.h"
#include "SerialPort.hpp"
#include "AudioSpectrumControl.h"
#include "WaveOut.h"

enum class Controls : DWORD
{
	ID_STATIC_R = 4000,
	ID_STATIC_G,
	ID_STATIC_B,
	ID_STATIC_A,
	ID_RED,
	ID_BLUE,
	ID_GREEN,
	ID_ALPHA,
	ID_DIAL,
	ID_EDIT,
	ID_COMBO,
	ID_AUDIO,
	ID_DEVICES
};

class MyWindow : public BaseWindow<MyWindow, Window, FrameWinTraits>
{
	ComboBoxControlEx m_combo;
	ComboBoxControlEx m_devices;

	ComPtr<ID2D1Factory> m_factory;
	ComPtr<ID2D1DCRenderTarget> m_rt;
	ComPtr<ID2D1SolidColorBrush> m_background;

	MyAboutDialog about;
	AudioSpectrumControl m_audioDisp;

	CustomScrollBarControl m_vscroll_r;
	CustomScrollBarControl m_vscroll_b;
	CustomScrollBarControl m_vscroll_g;
	CustomScrollBarControl m_vscroll_a;
	StaticControl m_static_r;
	StaticControl m_static_b;
	StaticControl m_static_g;
	StaticControl m_static_a;
	DialControl m_dial;
	D2DEditControl m_edit;
	WaveOut m_out;

	int m_dialPos = 0;
	int m_micsel = 0;
	float r = 0.f, g = 0.f, b = 0.f, a = 0.f;
	void OnClose();
	void OnLButtonDown(UINT nFlags, const Point &pt);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnAbout(UINT uNotifyCode, int nID, Window wndCtl);
	void OnExit(UINT uNotifyCode, int nID, Window wndCtl);
	void OnCombo(UINT uNoitfycode,int nID, Window wndCtl);
	void OnDevicesChange(UINT uNoitfycode,int nID, Window wndCtl);

	void OnVScroll(UINT nCode, UINT nPos, ScrollBarControl sb);
	void OnSize(UINT nType, Size size);
	void OnPaint(DC dc);

	HRESULT CreateFactory();
	HRESULT CreateRenderTarget();

	LRESULT OnThemeChanged(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnSettingChange(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnWaveOpen(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnWaveDone(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnWaveClose(UINT, WPARAM, LPARAM, BOOL& bHandled);
	void OnNcDestroy();
	void ReleaseResources();
public:
	
	MyWindow(){};
	DECLARE_WND_CLASS_1(IDC_WINTEST, IDI_SMALL)

	HRESULT Initialize();

	BEGIN_MSG_MAP()
	BEGIN_COMMAND_MAP()
		COMMAND_ID_HANDLER_EX(IDM_ABOUT, OnAbout)
		COMMAND_ID_HANDLER_EX(IDM_EXIT, OnExit)
		COMMAND_ID_HANDLER_EX(std::to_underlying(Controls::ID_COMBO),OnCombo)
		COMMAND_ID_HANDLER_EX(std::to_underlying(Controls::ID_DEVICES),OnDevicesChange)
	END_COMMAND_MAP()
	MESSAGE_HANDLER(WM_THEMECHANGED,    OnThemeChanged)
	MESSAGE_HANDLER(WM_SETTINGCHANGE,   OnSettingChange)
	MESSAGE_HANDLER(MM_WOM_OPEN, OnWaveOpen)
	MESSAGE_HANDLER(MM_WOM_DONE, OnWaveDone)
    MESSAGE_HANDLER(MM_WOM_CLOSE, OnWaveClose)
	MSG_WM_PAINT(OnPaint)
	MSG_WM_SIZE(OnSize)
	MSG_WM_CREATE(OnCreate)
	MSG_WM_CLOSE(OnClose)
	MSG_WM_LBUTTONDBLCLK(OnLButtonDown)
	MSG_WM_VSCROLL(OnVScroll)
	MSG_WM_NCDESTROY(OnNcDestroy)
	END_MSG_MAP()
};
