#pragma once
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include "DarkTheme.hpp"
#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include "resource.h"
#include "Com0ComDriver.hpp"
#include "AddPairWindow.hpp"
#include "AboutDialog.hpp"
#include <format>
#include <memory>
#include <string>
#include <vector>

// ─── MainWindow ───────────────────────────────────────────────────────────────
class MainWindow : public BaseWindow<MainWindow, Window, FrameWinTraits>
{
public:
    DECLARE_WND_CLASS_1(IDC_COM_COM, IDI_APP);

    BEGIN_MSG_MAP()
        MSG_WM_CREATE(OnCreate)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_SIZE(OnSize)
        MSG_WM_ERASEBKGND(OnEraseBkgnd)
        MSG_WM_CTLCOLORSTATIC(OnCtlColorStatic)
        MSG_WM_CTLCOLOREDIT(OnCtlColorEdit)
        MSG_WM_CTLCOLORBTN(OnCtlColorBtn)
        MSG_WM_DRAWITEM(OnDrawItem)
        MSG_WM_SETTINGCHANGE(OnSettingChange)
        MSG_WM_ACTIVATE(OnActivate)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_EXITSIZEMOVE(OnExitSizeMove)
        MESSAGE_HANDLER(WM_MOVING, OnMoving)
        MESSAGE_HANDLER(WM_NCPAINT, OnNcPaint)
        MESSAGE_HANDLER(WM_NCACTIVATE, OnNcActivate)
        MESSAGE_HANDLER(WM_NCCALCSIZE, OnNcCalcSize)
        MESSAGE_HANDLER(WM_UAHDRAWMENU, OnUahDrawMenu)
        MESSAGE_HANDLER(WM_UAHDRAWMENUITEM, OnUahDrawMenuItem)
        MESSAGE_HANDLER(WM_PAIRWINDOW_DONE, OnPairWindowDone)
        MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
        BEGIN_COMMAND_MAP()
            COMMAND_ID_HANDLER_EX(IDC_BTN_ADD, OnBtnAdd)
            COMMAND_ID_HANDLER_EX(IDC_BTN_REMOVE, OnBtnRemove)
            COMMAND_ID_HANDLER_EX(IDC_BTN_REFRESH, OnBtnRefresh)
            COMMAND_ID_HANDLER_EX(IDM_FILE_EXIT, OnMenuExit)
            COMMAND_ID_HANDLER_EX(IDM_PAIRS_ADD, OnBtnAdd)
            COMMAND_ID_HANDLER_EX(IDM_PAIRS_REMOVE, OnBtnRemove)
            COMMAND_ID_HANDLER_EX(IDM_PAIRS_REFRESH, OnBtnRefresh)
            COMMAND_ID_HANDLER_EX(IDM_HELP_ABOUT, OnMenuAbout)
        END_COMMAND_MAP()
        BEGIN_NOTIFY_MAP()
            BEGIN_NOTIFY_CODE_MAP(IDC_LISTVIEW)
                NOTIFY_CODE_HANDLER(LVN_ITEMCHANGED, OnListViewItemChanged)
                NOTIFY_CODE_HANDLER(NM_DBLCLK, OnListViewDblClk)
            END_NOTIFY_CODE_MAP()
        END_NOTIFY_MAP()
    END_MSG_MAP()

private:
    int OnCreate(LPCREATESTRUCT pcs);
    void OnDestroy();
    void OnSize(UINT nType, Size sz);
    LRESULT OnGetMinMaxInfo(UINT, WPARAM, LPARAM lParam, BOOL &);

    BOOL OnEraseBkgnd(HDC hdc);
    HBRUSH OnCtlColorStatic(HDC hdc, HWND hWnd);
    HBRUSH OnCtlColorEdit(HDC hdc, HWND hWnd);
    HBRUSH OnCtlColorBtn(HDC hdc, HWND hWnd);
    void OnDrawItem(UINT nIDCtl, LPDRAWITEMSTRUCT dis);
    void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
    void ApplyThemeToControls();

    // Non-client and activation handlers
    void OnActivate(UINT nState, BOOL bMinimized, Window wndOther);
    void OnShowWindow(BOOL bShow, int nStatus);
    void OnExitSizeMove();
    LRESULT OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &);
    LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &);
    LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &);
    LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &);

    LRESULT OnUahDrawMenu(UINT, WPARAM, LPARAM lParam, BOOL &);
    LRESULT OnUahDrawMenuItem(UINT, WPARAM, LPARAM lParam, BOOL &);

    void OnBtnAdd(UINT, int, Window);
    void OnBtnRemove(UINT, int, Window);
    void OnBtnRefresh(UINT, int, Window);
    void OnMenuExit(UINT, int, Window);
    void OnMenuAbout(UINT, int, Window);

    LRESULT OnListViewItemChanged(const NMHDR *pnmh);
    LRESULT OnListViewDblClk(const NMHDR *pnmh);
    LRESULT OnPairWindowDone(UINT, WPARAM, LPARAM lParam, BOOL &);

    void BuildMenu();
    void BuildListView();
    void PopulateListView();
    void UpdateButtonStates();
    void SetStatus(const std::tstring &text);
    void ShowErrorBox(const std::tstring &msg);
    int GetSelectedPairIndex() const;
    Com0ComPair *GetSelectedPair();
    void ShowAddPairWindow();
    void ShowEditPairWindow(const Com0ComPair &pair);

    ListViewControl m_listView;
    PushButtonControl m_btnAdd;
    PushButtonControl m_btnRemove;
    PushButtonControl m_btnRefresh;
    StatusBarControl m_statusBar;

    std::vector<Com0ComPair> m_pairs;
    std::unique_ptr<AddPairWindow> m_addWin;
    DarkTheme m_theme;
    AboutDialog dlg;

    static constexpr int kBtnH = 28, kBtnW = 100, kBtnGap = 6, kPad = 8;
    enum Col
    {
        ColPair = 0,
        ColCncA,
        ColPortA,
        ColCncB,
        ColPortB,
        ColCount
    };
};
