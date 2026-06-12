#include "MainWindow.hpp"
#include <commctrl.h>

HBRUSH AddPairWindow::OnColorStatic(DCT<true> dc, StaticControl wndStatic)
{

    if(m_background.Isnullptr())
        m_background = CreateSolidBrush(GetSysColor(COLOR_BTNHIGHLIGHT));

    dc.SetTextColor(RGB(0,0,0));
    dc.SetBkColor(RGB(255,255,255));
    dc.Detach();
    return m_background;
}
// ════════════════════════════════════════════════════════════════════════════
//  AddPairWindow
// ════════════════════════════════════════════════════════════════════════════

int AddPairWindow::OnCreate(LPCREATESTRUCT /*pcs*/)
{
    SetWindowText(m_isEdit ? TEXT("Edit Pair Port Names") : TEXT("Add New Pair"));

    const HINSTANCE hInst = HINST_THISCOMPONENT;
    const DWORD kLblStyle = WS_CHILD | WS_VISIBLE | SS_LEFT;
    const DWORD kEditStyle= WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;
    const DWORD kChkStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX;
    const DWORD kBtnStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;

    // Create controls at placeholder positions; PositionControls() will
    // size them correctly once we know the client rect.
    Rect rc = {0,0,0,0};

    m_lblPortA.Create(m_hwnd,&rc,TEXT("A-side port name:"),kLblStyle,0,HMENU(IDC_LBL_PORT_A),nullptr);

    m_lblPortA.Create(m_hwnd,&rc,TEXT("A-side port name:"),kLblStyle,0,  HMENU(IDC_LBL_PORT_A), nullptr);

    m_editPortA.Create(m_hwnd,rc,TEXT(""),kEditStyle,0, HMENU(IDC_EDIT_PORT_A), nullptr);

    m_lblPortB.Create(m_hwnd,&rc,TEXT("B-side port name:"),kLblStyle,0,HMENU(IDC_LBL_PORT_B),nullptr);

    // m_lblPortB   = ::CreateWindow(TEXT("STATIC"),  TEXT("B-side port name:"),
    //                               kLblStyle, 0,0,0,0, m_hwnd, HMENU(IDC_LBL_PORT_B),   hInst, nullptr);

    m_editPortB.Create(m_hwnd,rc,nullptr,kEditStyle,0,HMENU(IDC_EDIT_PORT_B),nullptr);

    // m_editPortB  = ::CreateWindow(TEXT("EDIT"),    nullptr,
    //                               kEditStyle,0,0,0,0, m_hwnd, HMENU(IDC_EDIT_PORT_B),  hInst, nullptr);

    m_chkEmuBrA.Create(m_hwnd,rc,TEXT("Emulate baud rate (A-side)"), HMENU(IDC_CHK_EMUBR_A));

    // m_chkEmuBrA.m_hwnd  = ::CreateWindow(TEXT("BUTTON"),  TEXT("Emulate baud rate (A-side)"),
    //                               kChkStyle, 0,0,0,0, m_hwnd, HMENU(IDC_CHK_EMUBR_A), hInst, nullptr);

    m_chkEmuBrB.Create(m_hwnd,rc,TEXT("Emulate baud rate (B-side)"),HMENU(IDC_CHK_EMUBR_B));
    // m_chkEmuBrB.m_hwnd  = ::CreateWindow(TEXT("BUTTON"),  TEXT("Emulate baud rate (B-side)"),
    //                               kChkStyle, 0,0,0,0, m_hwnd, HMENU(IDC_CHK_EMUBR_B), hInst, nullptr);

    m_lblHint.Create(m_hwnd,&rc,TEXT("Leave a port name blank to let com0com assign it.\r\nUse a standard COM port name, e.g. COM10."),kLblStyle | SS_NOPREFIX,0,HMENU(IDC_LBL_HINT));

    // m_lblHint    = ::CreateWindow(TEXT("STATIC"),
    //                               TEXT("Leave a port name blank to let com0com assign it.\r\n"
    //                                    "Use a standard COM port name, e.g. COM10."),
    //                               kLblStyle | SS_NOPREFIX,
    //                               0,0,0,0, m_hwnd, HMENU(IDC_LBL_HINT), hInst, nullptr);
    
    m_btnOK.Create(m_hwnd,rc,TEXT("OK"),UMenuOrID{IDC_BTN_OK});
    m_btnOK.ModifyStyle(0,BS_DEFPUSHBUTTON);
    // m_btnOK.m_hwnd      = ::CreateWindow(TEXT("BUTTON"),  TEXT("OK"),
    //                               kBtnStyle | BS_DEFPUSHBUTTON,
    //                               0,0,0,0, m_hwnd, HMENU(IDC_BTN_OK),     hInst, nullptr);
    m_btnCancel.Create(m_hwnd,rc,TEXT("Cancel"),UMenuOrID{IDC_BTN_CANCEL});
    // m_btnCancel.m_hwnd  = ::CreateWindow(TEXT("BUTTON"),  TEXT("Cancel"),
    //                               kBtnStyle,
    //                               0,0,0,0, m_hwnd, HMENU(IDC_BTN_CANCEL), hInst, nullptr);

    // Pre-fill if editing an existing pair
    if (m_isEdit) {
        m_editPortA.SetWindowText(m_pair.portNameA != TEXT("-") ? m_pair.portNameA.c_str() : nullptr);
        m_editPortB.SetWindowText(m_pair.portNameB != TEXT("-") ? m_pair.portNameB.c_str() : nullptr);
        ::CheckDlgButton(m_hwnd, IDC_CHK_EMUBR_A, m_pair.emuBrA ? BST_CHECKED : BST_UNCHECKED);
        ::CheckDlgButton(m_hwnd, IDC_CHK_EMUBR_B, m_pair.emuBrB ? BST_CHECKED : BST_UNCHECKED);

        // When editing, port name fields are optional (keep current name on empty)
        m_lblHint.SetWindowText(
            TEXT("Leave a field blank to keep the current port name."));
    }

    UpdateOkButton();
    return 0;
}

void AddPairWindow::OnClose()
{
    PostResult(false);
}

void AddPairWindow::OnDestroy()
{
    // No special teardown needed; child windows are destroyed with the parent.
   
}

void AddPairWindow::OnSize(UINT /*nType*/, Size sz)
{
    PositionControls();
}

void AddPairWindow::PositionControls()
{
    Rect rc;
    GetClientRect(&rc);
    const int w   = rc.Width();
    const int pad = 10;
    const int lblH= 16;
    const int editH = 22;
    const int chkH  = 20;
    const int btnH  = 26;
    const int btnW  = 80;
    const int row   = 4;   // gap between rows
    int y = pad;

    // A-side label + edit
    m_lblPortA.SetWindowPos(nullptr, pad, y, w - 2*pad, lblH, SWP_NOZORDER);
    y += lblH + row;
    m_editPortA.SetWindowPos(nullptr, pad, y, w - 2*pad, editH, SWP_NOZORDER);
    y += editH + row;
    m_chkEmuBrA.SetWindowPos(nullptr, pad, y, w - 2*pad, chkH, SWP_NOZORDER);
    y += chkH + pad;

    // B-side label + edit
    m_lblPortB.SetWindowPos(nullptr, pad, y, w - 2*pad, lblH, SWP_NOZORDER);
    y += lblH + row;
    m_editPortB.SetWindowPos(nullptr, pad, y, w - 2*pad, editH, SWP_NOZORDER);
    y += editH + row;
    m_chkEmuBrB.SetWindowPos(nullptr, pad, y, w - 2*pad, chkH, SWP_NOZORDER);
    y += chkH + pad;

    // Hint
    m_lblHint.SetWindowPos(nullptr, pad, y, w - 2*pad, lblH*2, SWP_NOZORDER);
    y += lblH*2 + pad;

    // OK / Cancel buttons (right-aligned)
    int btnY = rc.Height() - btnH - pad;
    m_btnCancel.SetWindowPos(nullptr, w - pad - btnW,        btnY, btnW, btnH, SWP_NOZORDER);
    m_btnOK.SetWindowPos    (nullptr, w - pad - btnW*2 - row, btnY, btnW, btnH, SWP_NOZORDER);
}

void AddPairWindow::UpdateOkButton()
{
    // For a new pair the OK button is always enabled (port names are optional).
    // For an edit we also allow OK unconditionally since leaving fields blank
    // means "keep current".
    m_btnOK.EnableWindow(TRUE);
}

void AddPairWindow::OnPortAChanged(UINT, int, Window) { UpdateOkButton(); }
void AddPairWindow::OnPortBChanged(UINT, int, Window) { UpdateOkButton(); }

void AddPairWindow::OnOK(UINT, int, Window)     { PostResult(true);  }
void AddPairWindow::OnCancel(UINT, int, Window) { PostResult(false); }

void AddPairWindow::PostResult(bool accepted)
{
    // Build the result on the heap; MainWindow takes ownership.
    auto* result       = new PairWindowResult;
    result->accepted   = accepted;
    result->isEdit     = m_isEdit;
    result->pairIndex  = m_isEdit ? m_pair.pairIndex : -1;

    if (accepted) {
        std::tstring portA(256, L'\0');
        std::tstring portB(256, L'\0');
        ::GetWindowText(m_editPortA, portA.data(), 256);
        ::GetWindowText(m_editPortB, portB.data(), 256);
        portA.resize(_tcslen(portA.c_str()));
        portB.resize(_tcslen(portB.c_str()));

        result->params.portNameA = portA;
        result->params.portNameB = portB;
        result->params.emuBrA    = (::IsDlgButtonChecked(m_hwnd, IDC_CHK_EMUBR_A) == BST_CHECKED);
        result->params.emuBrB    = (::IsDlgButtonChecked(m_hwnd, IDC_CHK_EMUBR_B) == BST_CHECKED);
    }

    // Post to our owner (the main window) and destroy ourselves.
    HWND hOwner = ::GetWindow(m_hwnd, GW_OWNER);
    ::PostMessage(hOwner, WM_PAIRWINDOW_DONE,
                  WPARAM(accepted ? 1 : 0),
                  LPARAM(result));
    DestroyWindow();
}

// ════════════════════════════════════════════════════════════════════════════
//  MainWindow
// ════════════════════════════════════════════════════════════════════════════

int MainWindow::OnCreate(LPCREATESTRUCT pcs)
{
    
    SetWindowText(TEXT("com0com Manager"));
    

    const HINSTANCE hInst    = HINST_THISCOMPONENT;
    const DWORD     kBtnStyle= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;

    m_btnAdd.Attach(::CreateWindow(TEXT("BUTTON"), TEXT("Add Pair"),
                                  kBtnStyle, 0,0,0,0, *this, HMENU(IDC_BTN_ADD),    hInst, nullptr));
    m_btnRemove.Attach(::CreateWindow(TEXT("BUTTON"), TEXT("Remove Pair"),
                                  kBtnStyle, 0,0,0,0, *this, HMENU(IDC_BTN_REMOVE), hInst, nullptr));
    m_btnRefresh.Attach( ::CreateWindow(TEXT("BUTTON"), TEXT("Refresh"),
                                  kBtnStyle, 0,0,0,0, *this, HMENU(IDC_BTN_REFRESH),hInst, nullptr));

    // Status bar (uses common controls)
    m_statusBar = ::CreateWindowEx(0, STATUSCLASSNAME, nullptr,
                                   WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
                                   0,0,0,0, *this, HMENU(IDC_STATUSBAR), hInst, nullptr);

    
    SetStatus(Com0ComDriver::DriverStatusString());
    return 0;
}

void MainWindow::OnDestroy()
{
    ::PostQuitMessage(0);
}

void MainWindow::OnSize(UINT /*nType*/, Size sz)
{
    // Resize status bar first (it positions itself)
    ::SendMessage(m_statusBar, WM_SIZE, 0, 0);

    // Get the height the status bar ended up with
    Rect sbRect;
    m_statusBar.GetWindowRect(&sbRect);
    const int sbH = sbRect.Height();

    const int cw = sz.cx;
    const int ch = sz.cy;

    // Button row just above the status bar
    const int btnY  = ch - sbH - kBtnH - kPad;
    const int totalBtnW = kBtnW * 3 + kBtnGap * 2;
    int bx = kPad;
    m_btnAdd.SetWindowPos    (nullptr, bx, btnY, kBtnW, kBtnH, SWP_NOZORDER); bx += kBtnW + kBtnGap;
    m_btnRemove.SetWindowPos (nullptr, bx, btnY, kBtnW, kBtnH, SWP_NOZORDER); bx += kBtnW + kBtnGap;
    m_btnRefresh.SetWindowPos(nullptr, bx, btnY, kBtnW, kBtnH, SWP_NOZORDER);

    // ListView fills the remaining space
    const int lvH = btnY - kPad * 2;
    if(m_listView.IsWindow()) m_listView.SetWindowPos(nullptr, kPad, kPad, cw - kPad*2, lvH, SWP_NOZORDER);

    InvalidateRect(nullptr, FALSE);
}

LRESULT MainWindow::OnGetMinMaxInfo(UINT, WPARAM, LPARAM lParam, BOOL&)
{
    auto* mmi = reinterpret_cast<MINMAXINFO*>(lParam);
    mmi->ptMinTrackSize.x = 520;
    mmi->ptMinTrackSize.y = 280;
    return 0;
}

// ─── Menu ─────────────────────────────────────────────────────────────────────

void MainWindow::BuildMenu()
{
    HMENU hMenuBar = ::CreateMenu();
    HMENU hFile    = ::CreatePopupMenu();
    HMENU hPairs   = ::CreatePopupMenu();
    HMENU hHelp    = ::CreatePopupMenu();

    ::AppendMenu(hFile,  MF_STRING,  IDM_FILE_EXIT,     TEXT("E&xit\tAlt+F4"));
    ::AppendMenu(hPairs, MF_STRING,  IDM_PAIRS_ADD,     TEXT("&Add Pair…"));
    ::AppendMenu(hPairs, MF_STRING,  IDM_PAIRS_REMOVE,  TEXT("&Remove Selected\tDel"));
    ::AppendMenu(hPairs, MF_SEPARATOR,0,                nullptr);
    ::AppendMenu(hPairs, MF_STRING,  IDM_PAIRS_REFRESH, TEXT("Re&fresh\tF5"));
    ::AppendMenu(hHelp,  MF_STRING,  IDM_HELP_ABOUT,    TEXT("&About…"));

    ::AppendMenu(hMenuBar, MF_POPUP, UINT_PTR(hFile),  TEXT("&File"));
    ::AppendMenu(hMenuBar, MF_POPUP, UINT_PTR(hPairs), TEXT("&Pairs"));
    ::AppendMenu(hMenuBar, MF_POPUP, UINT_PTR(hHelp),  TEXT("&Help"));

    SetMenu(hMenuBar);
}

// ─── ListView ────────────────────────────────────────────────────────────────

void MainWindow::BuildListView()
{
    m_listView.m_hwnd = ::CreateWindowEx(
        WS_EX_CLIENTEDGE,
        WC_LISTVIEW, nullptr,
        WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS,
        0, 0, 0, 0,
        m_hwnd, HMENU(IDC_LISTVIEW),
        HINST_THISCOMPONENT, nullptr);

    // Extended ListView styles: full-row select, grid lines
    ListView_SetExtendedListViewStyle(
        m_listView.m_hwnd,
        LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);

    struct ColDef { LPCTSTR text; int width; int fmt; };
    constexpr ColDef cols[ColCount] = {
        { TEXT("Pair #"),   50,  LVCFMT_CENTER },
        { TEXT("CNC Name A"), 80,  LVCFMT_LEFT   },
        { TEXT("Port A"),   90,  LVCFMT_LEFT   },
        { TEXT("CNC Name B"), 80,  LVCFMT_LEFT   },
        { TEXT("Port B"),   90,  LVCFMT_LEFT   },
    };

    for (int i = 0; i < ColCount; ++i) {
        LVCOLUMN lvc{};
        lvc.mask    = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
        lvc.cx      = cols[i].width;
        lvc.fmt     = cols[i].fmt;
        lvc.pszText = const_cast<LPTSTR>(cols[i].text);
        ListView_InsertColumn(m_listView.m_hwnd, i, &lvc);
    }
}

void MainWindow::PopulateListView()
{
    ListView_DeleteAllItems(m_listView.m_hwnd);
    m_pairs = Com0ComDriver::EnumeratePairs();

    for (int row = 0; row < static_cast<int>(m_pairs.size()); ++row) {
        const auto& p = m_pairs[row];

        LVITEM lvi{};
        lvi.mask    = LVIF_TEXT;
        lvi.iItem   = row;
        lvi.iSubItem= ColPair;
        std::tstring pairNum = std::format(TEXT("{}"), p.pairIndex);
        lvi.pszText = pairNum.data();
        ListView_InsertItem(m_listView.m_hwnd, &lvi);

        auto setText = [&](int col, const std::tstring& text) {
            LVITEM sub{};
            sub.mask     = LVIF_TEXT;
            sub.iItem    = row;
            sub.iSubItem = col;
            sub.pszText  = const_cast<LPTSTR>(text.c_str());
            ListView_SetItem(m_listView.m_hwnd, &sub);
        };

        setText(ColCncA,  p.cncNameA);
        setText(ColPortA, p.portNameA);
        setText(ColCncB,  p.cncNameB);
        setText(ColPortB, p.portNameB);
    }

    // Show a placeholder row if empty
    if (m_pairs.empty()) {
        LVITEM lvi{};
        lvi.mask    = LVIF_TEXT;
        lvi.iItem   = 0;
        lvi.iSubItem= 0;
        lvi.pszText = const_cast<LPTSTR>(TEXT("No com0com pairs found."));
        ListView_InsertItem(m_listView.m_hwnd, &lvi);

        LVITEM sub{};
        sub.mask     = LVIF_TEXT;
        sub.iItem    = 0;
        sub.iSubItem = ColPortA;
        sub.pszText  = const_cast<LPTSTR>(
            TEXT("Click \"Add Pair\" to create one."));
        ListView_SetItem(m_listView.m_hwnd, &sub);
    }

    UpdateButtonStates();
}

// ─── Button / menu handlers ───────────────────────────────────────────────────

void MainWindow::OnBtnAdd(UINT, int, Window)
{
    ShowAddPairWindow();
}

void MainWindow::OnBtnRemove(UINT, int, Window)
{
    Com0ComPair* p = GetSelectedPair();
    if (!p) return;

    std::tstring msg = std::format(
        TEXT("Remove pair {} ({} / {})?\n\n"
             "This will permanently delete both virtual ports."),
        p->pairIndex, p->cncNameA, p->cncNameB);

    if (::MessageBox(m_hwnd, msg.c_str(), TEXT("Remove Pair"),
                     MB_YESNO | MB_ICONWARNING) != IDYES)
        return;

    std::tstring err;
    if (!Com0ComDriver::DeletePair(p->pairIndex, err)) {
        ShowErrorBox(err);
    }
    PopulateListView();
    SetStatus(TEXT("Pair removed.  ")
              + Com0ComDriver::DriverStatusString());
}

void MainWindow::OnBtnRefresh(UINT, int, Window)
{
    PopulateListView();
    SetStatus(TEXT("Refreshed.  ") + Com0ComDriver::DriverStatusString());
}

void MainWindow::OnMenuExit(UINT, int, Window)  { DestroyWindow(); }

void MainWindow::OnMenuAbout(UINT, int, Window)
{
    ::MessageBox(m_hwnd,
        TEXT("com0com Manager\n\n"
             "A GUI front-end for the com0com virtual null-modem driver.\n\n"
             "Requires com0com (signed build from https://github.com/nickelc/com0com) "
             "and setupc.exe to be accessible.\n\n"
             "Operations that create, remove, or change pairs require an\n"
             "elevated (Administrator) process."),
        TEXT("About"),
        MB_OK | MB_ICONINFORMATION);
}

// ─── ListView notifications ───────────────────────────────────────────────────

LRESULT MainWindow::OnListViewItemChanged(NMHDR /*pnmh*/)
{
    UpdateButtonStates();
    return 0;
}

LRESULT MainWindow::OnListViewDblClk(NMHDR /*pnmh*/)
{
    // Double-click on a pair → open edit window
    Com0ComPair* p = GetSelectedPair();
    if (p) ShowEditPairWindow(*p);
    return 0;
}

// ─── Pair window result ───────────────────────────────────────────────────────

LRESULT MainWindow::OnPairWindowDone(UINT, WPARAM /*wParam*/, LPARAM lParam, BOOL&)
{
    // Re-enable the main window (we disabled it before showing the popup)
    EnableWindow(TRUE);
    SetForegroundWindow(*this);

    auto* result = reinterpret_cast<PairWindowResult*>(lParam);
    if (!result) return 0;

    std::unique_ptr<PairWindowResult> guard(result);  // ensure deletion

    if (!result->accepted) return 0;

    std::tstring err;
    bool ok = false;

    if (!result->isEdit) {
        // Create a new pair
        ok = Com0ComDriver::CreatePair(result->params, err);
        if (ok)
            SetStatus(TEXT("Pair created.  ") + Com0ComDriver::DriverStatusString());
    } else {
        // Edit existing pair
        Com0ComPair* p = nullptr;
        for (auto& pair : m_pairs)
            if (pair.pairIndex == result->pairIndex) { p = &pair; break; }

        if (p) {
            ok = Com0ComDriver::ChangePorts(
                *p, result->params.portNameA, result->params.portNameB, err);
            if (ok)
                SetStatus(TEXT("Pair updated.  ") + Com0ComDriver::DriverStatusString());
        }
    }

    if (!ok && !err.empty())
        ShowErrorBox(err);

    PopulateListView();
    return 0;
}

// ─── Helpers ─────────────────────────────────────────────────────────────────

int MainWindow::GetSelectedPairIndex() const
{
    int sel = ListView_GetNextItem(m_listView.m_hwnd, -1, LVNI_SELECTED);
    return sel;
}

Com0ComPair* MainWindow::GetSelectedPair()
{
    int sel = GetSelectedPairIndex();
    if (sel < 0 || sel >= static_cast<int>(m_pairs.size())) return nullptr;
    return &m_pairs[sel];
}

void MainWindow::UpdateButtonStates()
{
    bool hasSel = (GetSelectedPairIndex() >= 0) && !m_pairs.empty();
    m_btnRemove.EnableWindow(hasSel ? TRUE : FALSE);

    // Enable Add only if com0com is installed
    bool hasDriver = Com0ComDriver::IsDriverInstalled();
    m_btnAdd.EnableWindow(hasDriver ? TRUE : FALSE);
    ::EnableMenuItem(::GetMenu(m_hwnd), IDM_PAIRS_ADD,
                     hasDriver ? MF_ENABLED : MF_GRAYED);
    ::EnableMenuItem(::GetMenu(m_hwnd), IDM_PAIRS_REMOVE,
                     hasSel ? MF_ENABLED : MF_GRAYED);
}

void MainWindow::SetStatus(const std::tstring& text)
{
    ::SendMessage(m_statusBar, SB_SETTEXT, 0,
                  reinterpret_cast<LPARAM>(text.c_str()));
}

void MainWindow::ShowErrorBox(const std::tstring& msg)
{
    ::MessageBox(m_hwnd, msg.c_str(), TEXT("Error"), MB_OK | MB_ICONERROR);
}

void MainWindow::ShowAddPairWindow()
{
    m_addWin = std::make_unique<AddPairWindow>();
    m_addWin->SetupForNew();

    // Create as an owned popup so it stays in front of the main window
    RECT rcOwner;
    GetWindowRect(&rcOwner);
    const int w = 320, h = 300;
    const int x = rcOwner.left + (rcOwner.right - rcOwner.left - w) / 2;
    const int y = rcOwner.top  + (rcOwner.bottom - rcOwner.top  - h) / 2;

    RECT rc = { x, y, x + w, y + h };
    m_addWin->Create(
        m_hwnd, &rc,
        TEXT("Add New Pair"),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE);

    m_addWin->ShowWindow(SW_SHOW);

    // Disable main window so the popup behaves like a modal dialog
    EnableWindow(FALSE);
}

void MainWindow::ShowEditPairWindow(const Com0ComPair& pair)
{
    m_addWin = std::make_unique<AddPairWindow>();
    m_addWin->SetupForEdit(pair);

    RECT rcOwner;
    GetWindowRect(&rcOwner);
    const int w = 320, h = 300;
    const int x = rcOwner.left + (rcOwner.right - rcOwner.left - w) / 2;
    const int y = rcOwner.top  + (rcOwner.bottom - rcOwner.top  - h) / 2;

    RECT rc = { x, y, x + w, y + h };
    m_addWin->Create(
        m_hwnd, &rc,
        TEXT("Edit Pair"),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE);

    m_addWin->ShowWindow(SW_SHOW);
    EnableWindow(FALSE);
}
