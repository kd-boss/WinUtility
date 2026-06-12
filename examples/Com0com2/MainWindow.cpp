#include "MainWindow.hpp"
#include <commctrl.h>
#ifdef _MSC_VER
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shlwapi.lib")
#endif

int MainWindow::OnCreate(LPCREATESTRUCT)
{
    SetWindowText(TEXT("com0com Manager"));
    m_theme.ApplyToWindow(m_hwnd);
    BuildMenu();
    BuildListView();

    Rect rc = {0, 0, 0, 0};
    m_btnAdd.Create(m_hwnd, rc, TEXT("Add Pair"), UMenuOrID{HMENU(IDC_BTN_ADD)});
    m_btnRemove.Create(m_hwnd, rc, TEXT("Remove Pair"), UMenuOrID{HMENU(IDC_BTN_REMOVE)});
    m_btnRefresh.Create(m_hwnd, rc, TEXT("Refresh"), UMenuOrID{HMENU(IDC_BTN_REFRESH)});
    m_statusBar.Create(m_hwnd, rc, nullptr, WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 0, HMENU(IDC_STATUSBAR));

    m_btnAdd.ModifyStyle(BS_TYPEMASK, BS_OWNERDRAW);
    m_btnRemove.ModifyStyle(BS_TYPEMASK, BS_OWNERDRAW);
    m_btnRefresh.ModifyStyle(BS_TYPEMASK, BS_OWNERDRAW);

    ApplyThemeToControls();
    PopulateListView();
    UpdateButtonStates();
    SetStatus(Com0ComDriver::DriverStatusString());
    return 0;
}

void MainWindow::OnDestroy() { ::PostQuitMessage(0); }

void MainWindow::OnSize(UINT, Size sz)
{
    if (!m_statusBar.IsWindow())
        return;
    ::SendMessage(m_statusBar, WM_SIZE, 0, 0);
    Rect sbRect;
    m_statusBar.GetWindowRect(&sbRect);
    const int sbH = sbRect.Height(), cw = sz.cx, ch = sz.cy;
    const int btnY = ch - sbH - kBtnH - kPad;
    int bx = kPad;
    m_btnAdd.SetWindowPos(nullptr, bx, btnY, kBtnW, kBtnH, SWP_NOZORDER);
    bx += kBtnW + kBtnGap;
    m_btnRemove.SetWindowPos(nullptr, bx, btnY, kBtnW, kBtnH, SWP_NOZORDER);
    bx += kBtnW + kBtnGap;
    m_btnRefresh.SetWindowPos(nullptr, bx, btnY, kBtnW, kBtnH, SWP_NOZORDER);
    m_listView.SetWindowPos(nullptr, kPad, kPad, cw - kPad * 2, btnY - kPad * 2, SWP_NOZORDER);
    InvalidateRect(nullptr, FALSE);
}

LRESULT MainWindow::OnGetMinMaxInfo(UINT, WPARAM, LPARAM lParam, BOOL &)
{
    auto *mmi = reinterpret_cast<MINMAXINFO *>(lParam);
    mmi->ptMinTrackSize.x = 520;
    mmi->ptMinTrackSize.y = 280;
    return 0;
}

BOOL MainWindow::OnEraseBkgnd(HDC hdc) { return m_theme.OnEraseBkgnd(hdc, m_hwnd); }
HBRUSH MainWindow::OnCtlColorStatic(HDC hdc, HWND h) { return m_theme.OnCtlColorStatic(hdc, h); }
HBRUSH MainWindow::OnCtlColorEdit(HDC hdc, HWND h) { return m_theme.OnCtlColorEdit(hdc, h); }
HBRUSH MainWindow::OnCtlColorBtn(HDC hdc, HWND h) { return m_theme.OnCtlColorBtn(hdc, h); }
void MainWindow::OnDrawItem(UINT, LPDRAWITEMSTRUCT dis) { m_theme.OnDrawButton(dis); }

LRESULT MainWindow::OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &)
{
    if (System::Information::isDebug::value)
        std::cout << "Move" << std::endl;
    m_theme.ApplyToWindow(m_hwnd);
    return ::DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

LRESULT MainWindow::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &)
{
    // Re-assert dark mode before every NC paint — this is the single common
    // path for all NC repaints regardless of trigger (snap, unsnap, restore,
    // focus change, occlusion).  WM_ACTIVATE misses the unsnap case because
    // the window is already active; this covers every code path.
    if (System::Information::isDebug::value)
        std::cout << "NcPaint" << std::endl;
    m_theme.ApplyToWindow(m_hwnd);
    LRESULT lr = ::DefWindowProc(m_hwnd, WM_NCPAINT, 1, lParam);
    // Separator fix only needed in dark mode; light mode uses standard NC paint.
    if (m_theme.IsDark())
        m_theme.OnNcPaintMenuSeparator(m_hwnd);

    return lr;
}

void MainWindow::OnActivate(UINT nState, BOOL /*bMinimized*/, Window /*wndOther*/)
{
    if (System::Information::isDebug::value)
        std::cout << "Activate" << std::endl;
    m_theme.ApplyToWindow(m_hwnd);
    m_theme.OnNcPaintMenuSeparator(m_hwnd);
}

void MainWindow::OnShowWindow(BOOL bShow, int /*nStatus*/)
{
    m_theme.ApplyToWindow(m_hwnd);
    if (!bShow)
        return;
    if (System::Information::isDebug::value)
        std::cout << "ShowWindow" << std::endl;
    ApplyThemeToControls();
    InvalidateRect(nullptr, TRUE);
}

void MainWindow::OnExitSizeMove()
{
    if (System::Information::isDebug::value)
        std::cout << "ExitSizeMove" << std::endl;
    m_theme.ApplyToWindow(m_hwnd);
    m_theme.OnNcPaintMenuSeparator(m_hwnd);
    ::SendMessage(m_hwnd, WM_NCPAINT, 1, 0);
}

LRESULT MainWindow::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &)
{
    m_theme.ApplyToWindow(m_hwnd);
    if (lParam == -1)
    {
        // Window is being destroyed — honour the sentinel and don't repaint.
        return ::DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    // Suppress DWM's default Aero NC repaint; drive our own below.
    LRESULT lr = ::DefWindowProc(m_hwnd, uMsg, wParam, -1);
    ::SendMessage(m_hwnd, WM_NCPAINT, 1, 0);
    return lr;
}

LRESULT MainWindow::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &handled)
{
    m_theme.ApplyToWindow(m_hwnd);
    LRESULT lr = DefWindowProc(uMsg, wParam, lParam);
    ::SendMessage(m_hwnd, WM_NCPAINT, 1, 0);
    handled = true;
    return lr;
}

void MainWindow::OnSettingChange(UINT, LPCTSTR lpszSection)
{
    if (lpszSection && ::lstrcmp(lpszSection, TEXT("ImmersiveColorSet")) == 0)
    {
        // Rebuild brushes and palette to match the new system preference,
        // then re-assert DWM attributes and repaint.
        m_theme.Refresh();
        DarkTheme::InitProcess();
        m_theme.ApplyToWindow(m_hwnd);
        if (m_theme.IsDark())
            m_theme.OnNcPaintMenuSeparator(m_hwnd);
        ApplyThemeToControls();
        InvalidateRect(nullptr, TRUE);
        // Trigger a full NC repaint so the title bar and menu bar update.
        ::SendMessage(m_hwnd, WM_NCPAINT, 1, 0);
    }
}

void MainWindow::ApplyThemeToControls()
{
    if (m_listView.IsWindow())
        m_theme.ApplyToListView(m_listView.m_hwnd);
    if (m_btnAdd.IsWindow())
        m_theme.ApplyToButton(m_btnAdd.m_hwnd);
    if (m_btnRemove.IsWindow())
        m_theme.ApplyToButton(m_btnRemove.m_hwnd);
    if (m_btnRefresh.IsWindow())
        m_theme.ApplyToButton(m_btnRefresh.m_hwnd);
    if (m_statusBar.IsWindow())
        m_theme.ApplyToStatusBar(m_statusBar.m_hwnd);
}

LRESULT MainWindow::OnUahDrawMenu(UINT, WPARAM, LPARAM lParam, BOOL &) { return m_theme.OnUahDrawMenu(m_hwnd, lParam); }
LRESULT MainWindow::OnUahDrawMenuItem(UINT, WPARAM, LPARAM lParam, BOOL &) { return m_theme.OnUahDrawMenuItem(lParam); }

void MainWindow::BuildMenu()
{
    HMENU hBar = ::CreateMenu(), hFile = ::CreatePopupMenu(),
          hPairs = ::CreatePopupMenu(), hHelp = ::CreatePopupMenu();
    ::AppendMenu(hFile, MF_STRING, IDM_FILE_EXIT, TEXT("E&xit\tAlt+F4"));
    ::AppendMenu(hPairs, MF_STRING, IDM_PAIRS_ADD, TEXT("&Add Pair..."));
    ::AppendMenu(hPairs, MF_STRING, IDM_PAIRS_REMOVE, TEXT("&Remove Selected\tDel"));
    ::AppendMenu(hPairs, MF_SEPARATOR, 0, nullptr);
    ::AppendMenu(hPairs, MF_STRING, IDM_PAIRS_REFRESH, TEXT("Re&fresh\tF5"));
    ::AppendMenu(hHelp, MF_STRING, IDM_HELP_ABOUT, TEXT("&About..."));
    ::AppendMenu(hBar, MF_POPUP, UINT_PTR(hFile), TEXT("&File"));
    ::AppendMenu(hBar, MF_POPUP, UINT_PTR(hPairs), TEXT("&Pairs"));
    ::AppendMenu(hBar, MF_POPUP, UINT_PTR(hHelp), TEXT("&Help"));
    ::SetMenu(m_hwnd, hBar);
}

void MainWindow::BuildListView()
{
    Rect rc = {0, 0, 0, 0};
    // dwExStyle = 0: no WS_EX_CLIENTEDGE (that creates the 3D sunken border).
    // LVS_EX_* flags are ListView-specific and must be set via
    // SetExtendedListViewStyle — they are NOT window extended styles.
    m_listView.Create(m_hwnd, rc, nullptr,
                      WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS,
                      0,
                      HMENU(IDC_LISTVIEW));
    m_listView.SetExtendedListViewStyle(
        LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);

    struct ColDef
    {
        LPCTSTR text;
        int width;
        int fmt;
    };
    constexpr ColDef cols[ColCount] = {
        {TEXT("Pair #"), 50, LVCFMT_CENTER},
        {TEXT("CNC Name A"), 80, LVCFMT_LEFT},
        {TEXT("Port A"), 90, LVCFMT_LEFT},
        {TEXT("CNC Name B"), 80, LVCFMT_LEFT},
        {TEXT("Port B"), 90, LVCFMT_LEFT},
    };
    for (int i = 0; i < ColCount; ++i)
    {
        LVCOLUMN lvc{};
        lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
        lvc.cx = cols[i].width;
        lvc.fmt = cols[i].fmt;
        lvc.pszText = const_cast<LPTSTR>(cols[i].text);
        m_listView.InsertColumn(i, &lvc);
    }
}

void MainWindow::PopulateListView()
{
    m_listView.DeleteAllItems();
    m_pairs = Com0ComDriver::EnumeratePairs();

    for (int row = 0; row < static_cast<int>(m_pairs.size()); ++row)
    {
        const auto &p = m_pairs[row];
        LVITEM lvi{};
        lvi.mask = LVIF_TEXT;
        lvi.iItem = row;
        std::tstring pn = std::format(TEXT("{}"), p.pairIndex);
        lvi.pszText = pn.data();
        m_listView.InsertItem(&lvi);
        auto setText = [&](int col, const std::tstring &text)
        {
            LVITEM sub{};
            sub.mask = LVIF_TEXT;
            sub.iItem = row;
            sub.iSubItem = col;
            sub.pszText = const_cast<LPTSTR>(text.c_str());
            m_listView.SetItem(&sub);
        };
        setText(ColCncA, p.cncNameA);
        setText(ColPortA, p.portNameA);
        setText(ColCncB, p.cncNameB);
        setText(ColPortB, p.portNameB);
    }

    if (m_pairs.empty())
    {
        LVITEM lvi{};
        lvi.mask = LVIF_TEXT;
        lvi.iItem = 0;
        lvi.pszText = const_cast<LPTSTR>(TEXT("No com0com pairs found."));
        m_listView.InsertItem(&lvi);
        LVITEM sub{};
        sub.mask = LVIF_TEXT;
        sub.iItem = 0;
        sub.iSubItem = ColPortA;
        sub.pszText = const_cast<LPTSTR>(TEXT("Click \"Add Pair\" to create one."));
        m_listView.SetItem(&sub);
    }
    UpdateButtonStates();
}

void MainWindow::OnBtnAdd(UINT, int, Window) { ShowAddPairWindow(); }

void MainWindow::OnBtnRemove(UINT, int, Window)
{
    Com0ComPair *p = GetSelectedPair();
    if (!p)
        return;
    std::tstring msg = std::format(
        TEXT("Remove pair {} ({} / {})?\n\nThis will permanently delete both virtual ports."),
        p->pairIndex, p->cncNameA, p->cncNameB);
    if (MessageBox::Show(*this, msg.c_str(), TEXT("Remove Pair"),
                         MessageBoxButtons::YesNo, MessageBoxIcon::Warning) != MessageBoxResult::Yes)
        return;
    std::tstring err;
    if (!Com0ComDriver::DeletePair(p->pairIndex, err))
        ShowErrorBox(err);
    PopulateListView();
    SetStatus(TEXT("Pair removed.  ") + Com0ComDriver::DriverStatusString());
}

void MainWindow::OnBtnRefresh(UINT, int, Window)
{
    PopulateListView();
    SetStatus(TEXT("Refreshed.  ") + Com0ComDriver::DriverStatusString());
}

void MainWindow::OnMenuExit(UINT, int, Window) { PostQuitMessage(0); }

void MainWindow::OnMenuAbout(UINT, int, Window)
{
    if (!dlg)
    {
        SetLastError(ERROR_SUCCESS);
        auto dlgres = dlg.DoModal();
        if (dlgres == IDOK)
        {
            dlg.DestroyWindow();
        }
        else if (dlgres == -1)
        {
            HR(__HRESULT_FROM_WIN32(GetLastError()));
        }
    }
}

LRESULT MainWindow::OnListViewItemChanged(const NMHDR *)
{
    UpdateButtonStates();
    return 0;
}

LRESULT MainWindow::OnListViewDblClk(const NMHDR *)
{
    Com0ComPair *p = GetSelectedPair();
    if (p)
        ShowEditPairWindow(*p);
    return 0;
}

LRESULT MainWindow::OnPairWindowDone(UINT, WPARAM, LPARAM lParam, BOOL &)
{
    EnableWindow(TRUE);
    SetForegroundWindow();
    auto *r = reinterpret_cast<PairWindowResult *>(lParam);
    if (!r)
        return 0;
    std::unique_ptr<PairWindowResult> guard(r);
    if (!r->accepted)
        return 0;
    std::tstring err;
    bool ok = false;
    if (!r->isEdit)
    {
        ok = Com0ComDriver::CreatePair(r->params, err);
        if (ok)
            SetStatus(TEXT("Pair created.  ") + Com0ComDriver::DriverStatusString());
    }
    else
    {
        Com0ComPair *p = nullptr;
        for (auto &pair : m_pairs)
            if (pair.pairIndex == r->pairIndex)
            {
                p = &pair;
                break;
            }
        if (p)
        {
            ok = Com0ComDriver::ChangePorts(*p, r->params.portNameA, r->params.portNameB, err);
            if (ok)
                SetStatus(TEXT("Pair updated.  ") + Com0ComDriver::DriverStatusString());
        }
    }
    if (!ok && !err.empty())
        ShowErrorBox(err);
    PopulateListView();
    return 0;
}

int MainWindow::GetSelectedPairIndex() const { return m_listView.GetNextItem(-1, LVNI_SELECTED); }

Com0ComPair *MainWindow::GetSelectedPair()
{
    int sel = GetSelectedPairIndex();
    if (sel < 0 || sel >= static_cast<int>(m_pairs.size()))
        return nullptr;
    return &m_pairs[sel];
}

void MainWindow::UpdateButtonStates()
{
    const bool hasSel = GetSelectedPairIndex() >= 0 && !m_pairs.empty();
    const bool hasDrv = Com0ComDriver::IsDriverInstalled();
    m_btnAdd.EnableWindow(hasDrv ? TRUE : FALSE);
    m_btnRemove.EnableWindow(hasSel ? TRUE : FALSE);
    ::EnableMenuItem(::GetMenu(m_hwnd), IDM_PAIRS_ADD, hasDrv ? MF_ENABLED : MF_GRAYED);
    ::EnableMenuItem(::GetMenu(m_hwnd), IDM_PAIRS_REMOVE, hasSel ? MF_ENABLED : MF_GRAYED);
}

void MainWindow::SetStatus(const std::tstring &text) { m_statusBar.SetText(0, text.c_str()); }

void MainWindow::ShowErrorBox(const std::tstring &msg)
{
    MessageBox::Show(*this, msg.c_str(), TEXT("Error"), MessageBoxButtons::Ok, MessageBoxIcon::Error);
}

void MainWindow::ShowAddPairWindow()
{
    m_addWin = std::make_unique<AddPairWindow>();
    m_addWin->SetupForNew();
    RECT rco;
    GetWindowRect(&rco);
    const int w = 320, h = 300;
    RECT rc = {rco.left + (rco.right - rco.left - w) / 2, rco.top + (rco.bottom - rco.top - h) / 2, 0, 0};
    rc.right = rc.left + w;
    rc.bottom = rc.top + h;
    m_addWin->Create(m_hwnd, &rc, TEXT("Add New Pair"),
                     WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE);
    m_addWin->ShowWindow(SW_SHOW);
    EnableWindow(FALSE);
}

void MainWindow::ShowEditPairWindow(const Com0ComPair &pair)
{
    m_addWin = std::make_unique<AddPairWindow>();
    m_addWin->SetupForEdit(pair);
    RECT rco;
    GetWindowRect(&rco);
    const int w = 320, h = 300;
    RECT rc = {rco.left + (rco.right - rco.left - w) / 2, rco.top + (rco.bottom - rco.top - h) / 2, 0, 0};
    rc.right = rc.left + w;
    rc.bottom = rc.top + h;
    m_addWin->Create(m_hwnd, &rc, TEXT("Edit Pair"),
                     WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE);
    m_addWin->ShowWindow(SW_SHOW);
    EnableWindow(FALSE);
}
