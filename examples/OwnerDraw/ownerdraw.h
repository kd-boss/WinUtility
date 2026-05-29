
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include <WinUtility/ComPtr.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <utility>

#include "Resource.h"
#include "About.h"

enum class ButtonID : UINT
{
	ID_SMALLER = WM_USER + 1,
	ID_LARGER
};

class MyWindow : public BaseWindow<MyWindow, Window, FrameWinTraits>
{
	UINT BTN_HEIGHT, BTN_WIDTH;
	MyAboutDialog about;
	ComPtr<ID2D1Factory> m_factory;
	ComPtr<ID2D1DCRenderTarget> m_rt;
	ComPtr<ID2D1SolidColorBrush> m_brush;
	ComPtr<ID2D1StrokeStyle> m_stroke;
	OwnerDrawButtonControl m_smaller;
	OwnerDrawButtonControl m_larger;

	HRESULT CreateDeviceResources();
	HRESULT CreateDeviceIndependantResources();
	ID2D1PathGeometry* DrawTriangle(D2D1_POINT_2F a,D2D1_POINT_2F b,D2D1_POINT_2F c);
	void DestroyDeviceResources();
	void OnClose();
	void OnLButtonDown(UINT nFlags, const Point &pt);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnAbout(UINT uNotifyCode, int nID, Window wndCtl);
	void OnExit(UINT uNotifyCode, int nID, Window wndCtl);
	void OnSmaller(UINT uNotifyCode, int nID, Window wndCtl);
	void OnLarger(UINT uNotifyCode, int nID, Window wndCtl);
	void OnSize(UINT nFlags, Size sz);
	void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
public:


	DECLARE_WND_CLASS_1(IDC_OWNERDRAW, IDI_OWNERDRAW)

	HRESULT Initialize();

	BEGIN_MSG_MAP()
	BEGIN_COMMAND_MAP()
	COMMAND_ID_HANDLER_EX(IDM_ABOUT, OnAbout)
	COMMAND_ID_HANDLER_EX(IDOK, OnAbout)
	COMMAND_ID_HANDLER_EX(IDM_EXIT, OnExit)
	COMMAND_ID_HANDLER_EX(std::to_underlying(ButtonID::ID_SMALLER),OnSmaller)
	COMMAND_ID_HANDLER_EX(std::to_underlying(ButtonID::ID_LARGER), OnLarger)
	END_COMMAND_MAP()
	MSG_WM_DRAWITEM(OnDrawItem)
	MSG_WM_SIZE(OnSize)
	MSG_WM_CREATE(OnCreate)
	MSG_WM_CLOSE(OnClose)
	MSG_WM_LBUTTONDBLCLK(OnLButtonDown)
	END_MSG_MAP()
};
