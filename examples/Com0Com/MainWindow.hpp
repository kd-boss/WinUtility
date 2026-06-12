#pragma once
// MainWindow.hpp
//
// Two window classes:
//
//   AddPairWindow  —  Popup for creating a new pair or editing port names.
//                     Posts WM_PAIRWINDOW_DONE / WM_EDITWINDOW_DONE back
//                     to its owner when the user clicks OK or Cancel.
//
//   MainWindow     —  Main frame window.  ListView showing all com0com pairs,
//                     toolbar buttons, and a status bar.

#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include "resource.h"
#include "Com0ComDriver.hpp"

#include <format>
#include <memory>
#include <string>
#include <vector>

// ─── Result passed from AddPairWindow to MainWindow ──────────────────────────

struct PairWindowResult {
    bool      accepted   = false;   ///< true = OK, false = Cancel
    bool      isEdit     = false;   ///< true = editing existing pair
    int       pairIndex  = -1;      ///< Valid when isEdit = true
    PairParams params;
};

// ─── AddPairWindow ───────────────────────────────────────────────────────────

class AddPairWindow
    : public BaseWindow<AddPairWindow, Window,
                        WinTraits<WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
                                  WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE>>
{
public:
    DECLARE_WND_CLASS(TEXT("Com0ComAddPair"))

    // Call before Create() to configure the window.
    void SetupForNew()   { m_isEdit = false; m_pair = {}; }
    void SetupForEdit(const Com0ComPair& pair) { m_isEdit = true; m_pair = pair; }

    BEGIN_MSG_MAP()
        MSG_WM_CREATE(OnCreate)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_SIZE(OnSize)
        MSG_WM_CTLCOLORSTATIC(OnColorStatic)
        BEGIN_COMMAND_MAP()
            COMMAND_ID_HANDLER_EX(IDC_BTN_OK,     OnOK)
            COMMAND_ID_HANDLER_EX(IDC_BTN_CANCEL,  OnCancel)
            COMMAND_HANDLER_EX(IDC_EDIT_PORT_A, EN_CHANGE, OnPortAChanged)
            COMMAND_HANDLER_EX(IDC_EDIT_PORT_B, EN_CHANGE, OnPortBChanged)
        END_COMMAND_MAP()
    END_MSG_MAP()

private:
    int    OnCreate(LPCREATESTRUCT pcs);
    void   OnClose();
    void   OnDestroy();
    void   OnSize(UINT nType, Size sz);
    void   OnOK(UINT, int, Window);
    void   OnCancel(UINT, int, Window);
    void   OnPortAChanged(UINT, int, Window);
    void   OnPortBChanged(UINT, int, Window);

    void   UpdateOkButton();
    void   PostResult(bool accepted);
    void   PositionControls();
    HBRUSH OnColorStatic(DCT<true> dc, StaticControl wndStatic);

    bool          m_isEdit  = false;
    Com0ComPair   m_pair;             ///< Only valid when m_isEdit = true

    StaticControl            m_lblPortA;
    EditControl              m_editPortA;
    StaticControl            m_lblPortB;
    EditControl              m_editPortB;
    AutoCheckBoxControl      m_chkEmuBrA;
    AutoCheckBoxControl      m_chkEmuBrB;
    StaticControl            m_lblHint;
    PushButtonControl        m_btnOK;
    PushButtonControl        m_btnCancel;
    Brush m_background;
};

// ─── MainWindow ──────────────────────────────────────────────────────────────

class MainWindow
    : public BaseWindow<MainWindow, Window, FrameWinTraits>
{
public:
    DECLARE_WND_CLASS(TEXT("Com0ComMain"))   // resource-less class; no menu/icon resource IDs

    BEGIN_MSG_MAP()
        MSG_WM_CREATE(OnCreate)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_SIZE(OnSize)
        MESSAGE_HANDLER(WM_PAIRWINDOW_DONE, OnPairWindowDone)
        MESSAGE_HANDLER(WM_GETMINMAXINFO,   OnGetMinMaxInfo)
        BEGIN_COMMAND_MAP()
            COMMAND_ID_HANDLER_EX(IDC_BTN_ADD,      OnBtnAdd)
            COMMAND_ID_HANDLER_EX(IDC_BTN_REMOVE,   OnBtnRemove)
            COMMAND_ID_HANDLER_EX(IDC_BTN_REFRESH,  OnBtnRefresh)
            COMMAND_ID_HANDLER_EX(IDM_FILE_EXIT,    OnMenuExit)
            COMMAND_ID_HANDLER_EX(IDM_PAIRS_ADD,    OnBtnAdd)
            COMMAND_ID_HANDLER_EX(IDM_PAIRS_REMOVE, OnBtnRemove)
            COMMAND_ID_HANDLER_EX(IDM_PAIRS_REFRESH,OnBtnRefresh)
            COMMAND_ID_HANDLER_EX(IDM_HELP_ABOUT,   OnMenuAbout)
        END_COMMAND_MAP()
        BEGIN_NOTIFY_MAP()
            BEGIN_NOTIFY_CODE_MAP(IDC_LISTVIEW)
                NOTIFY_CODE_HANDLER(LVN_ITEMCHANGED,  OnListViewItemChanged)
                NOTIFY_CODE_HANDLER(NM_DBLCLK,        OnListViewDblClk)
            END_NOTIFY_CODE_MAP()
        END_NOTIFY_MAP()
    END_MSG_MAP()
    
    void  BuildMenu();
    void  BuildListView();
    void  PopulateListView();
    void  UpdateButtonStates();
private:
    // ── Window lifecycle ─────────────────────────────────────────────────────
    int    OnCreate(LPCREATESTRUCT pcs);
 
    void   OnDestroy();
    void   OnSize(UINT nType, Size sz);
    LRESULT OnGetMinMaxInfo(UINT, WPARAM, LPARAM lParam, BOOL&);

    // ── Button / menu handlers ───────────────────────────────────────────────
    void   OnBtnAdd    (UINT, int, Window);
    void   OnBtnRemove (UINT, int, Window);
    void   OnBtnRefresh(UINT, int, Window);
    void   OnMenuExit  (UINT, int, Window);
    void   OnMenuAbout (UINT, int, Window);

    // ── ListView notifications ───────────────────────────────────────────────
    LRESULT OnListViewItemChanged(NMHDR pnmh);
    LRESULT OnListViewDblClk     (NMHDR pnmh);

    // ── Pair window result ───────────────────────────────────────────────────
    LRESULT OnPairWindowDone(UINT, WPARAM wParam, LPARAM lParam, BOOL&);

    // ── Internal helpers ─────────────────────────────────────────────────────
    void  SetStatus(const std::tstring& text);
    void  ShowErrorBox(const std::tstring& msg);

    int   GetSelectedPairIndex() const;  ///< Returns -1 if nothing selected
    Com0ComPair* GetSelectedPair();       ///< Returns nullptr if nothing selected

    void  ShowAddPairWindow();
    void  ShowEditPairWindow(const Com0ComPair& pair);

    // ── Controls ─────────────────────────────────────────────────────────────
    ListViewControl    m_listView;
    PushButtonControl  m_btnAdd;
    PushButtonControl  m_btnRemove;
    PushButtonControl  m_btnRefresh;
    StatusBarControl   m_statusBar;

    // Pair data cache (refreshed from driver on each PopulateListView call)
    std::vector<Com0ComPair> m_pairs;

    // Pending add/edit popup
    std::unique_ptr<AddPairWindow> m_addWin;

    // ── Layout constants ─────────────────────────────────────────────────────
    static constexpr int kBtnH      = 28;
    static constexpr int kBtnW      = 100;
    static constexpr int kBtnGap    = 6;
    static constexpr int kPad       = 8;
    static constexpr int kStatusH   = 22;

    // ── ListView column indices ───────────────────────────────────────────────
    enum Col { ColPair=0, ColCncA, ColPortA, ColCncB, ColPortB, ColCount };
};
