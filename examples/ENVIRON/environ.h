
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include "Resource.h"
#include "About.h"
#include <utility>

enum class EnvironControls : UINT
{
	ListBox = 130,
	Static,
	ListView
};

class Environ : public BaseWindow<Environ, Window, FrameWinTraits>
{
	MyAboutDialog about;
	ListBoxControl m_listbox;
	StaticControl m_text;
	ListViewControl m_listView;
	int cxChar, cyChar;
	std::wstring text;

	void OnClose();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnAbout(UINT uNotifyCode, int nID, Window wndCtl);
	void OnUp(UINT uNotifyCode, int nID, Window wndCtl);
	void OnDown(UINT uNotifyCode, int nID, Window wndCtl);
	void OnNew(UINT uNotifyCode, int nID, Window wndCtl);
	void OnSave(UINT uNotifyCode, int nID, Window wndCtl);
	void OnDelete(UINT uNotifyCode, int nID, Window wndCtl);
	void OnExit(UINT uNotifyCode, int nID, Window wndCtl);
	void OnListBox(UINT uNotifyCode, int nID, Window wndCtl);
	void OnSize(UINT nFlags, Size sz);
	BOOL OnListViewEndLabelEdit(const NMLVDISPINFOW& disp);
public:


	DECLARE_WND_CLASS_1(IDC_ENVIRON, IDI_ENVIRON)

	HRESULT Initialize();

	BEGIN_MSG_MAP()
	BEGIN_COMMAND_MAP()
	COMMAND_ID_HANDLER_EX(IDM_UP, OnUp)
	COMMAND_ID_HANDLER_EX(IDM_DOWN, OnDown)
	COMMAND_ID_HANDLER_EX(IDM_NEW, OnNew)
	COMMAND_ID_HANDLER_EX(IDM_ABOUT, OnAbout)
	COMMAND_ID_HANDLER_EX(IDM_EXIT, OnExit)
	COMMAND_ID_HANDLER_EX(IDM_SAVE, OnSave)
	COMMAND_ID_HANDLER_EX(IDM_DELETE, OnDelete)
	COMMAND_ID_HANDLER_EX(std::to_underlying(EnvironControls::ListBox), OnListBox);
	END_COMMAND_MAP()
	BEGIN_NOTIFY_MAP()
		BEGIN_NOTIFY_CODE_MAP(std::to_underlying(EnvironControls::ListView))
			LVN_ENDLABELEDIT_HANDLER(OnListViewEndLabelEdit)
		END_NOTIFY_CODE_MAP()
	END_NOTIFY_MAP()
	MSG_WM_CREATE(OnCreate)
	MSG_WM_CLOSE(OnClose)
	MSG_WM_SIZE(OnSize)
	END_MSG_MAP()
};
