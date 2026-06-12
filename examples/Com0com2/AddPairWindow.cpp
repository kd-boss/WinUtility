#include "AddPairWindow.hpp"

// ════════════════════════════════════════════════════════════════════════════
//  AddPairWindow
// ════════════════════════════════════════════════════════════════════════════

int AddPairWindow::OnCreate(LPCREATESTRUCT)
{
    SetWindowText(m_isEdit ? TEXT("Edit Pair Port Names") : TEXT("Add New Pair"));
    m_theme.ApplyToWindow(m_hwnd);

    const DWORD kLbl = WS_CHILD | WS_VISIBLE | SS_LEFT;
    const DWORD kEdit = WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;
    Rect rc = {0, 0, 0, 0};

    m_lblPortA.Create(m_hwnd, &rc, TEXT("A-side port name:"), kLbl, 0, HMENU(IDC_LBL_PORT_A));
    m_editPortA.Create(m_hwnd, rc, TEXT(""), kEdit, 0, HMENU(IDC_EDIT_PORT_A));
    m_lblPortB.Create(m_hwnd, &rc, TEXT("B-side port name:"), kLbl, 0, HMENU(IDC_LBL_PORT_B));
    m_editPortB.Create(m_hwnd, rc, nullptr, kEdit, 0, HMENU(IDC_EDIT_PORT_B));
    m_chkEmuBrA.Create(m_hwnd, rc, TEXT("Emulate baud rate (A-side)"), HMENU(IDC_CHK_EMUBR_A));
    m_chkEmuBrB.Create(m_hwnd, rc, TEXT("Emulate baud rate (B-side)"), HMENU(IDC_CHK_EMUBR_B));
    m_lblHint.Create(m_hwnd, &rc,
                     TEXT("Leave a port name blank to let com0com assign it.\r\nUse a standard COMx name, e.g. COM10."),
                     kLbl | SS_NOPREFIX, 0, HMENU(IDC_LBL_HINT));
    m_btnCancel.Create(m_hwnd, rc, TEXT("Cancel"), UMenuOrID{IDC_BTN_CANCEL});
    m_btnOK.Create(m_hwnd, rc, TEXT("OK"), UMenuOrID{IDC_BTN_OK});
    m_btnOK.ModifyStyle(0, BS_DEFPUSHBUTTON);
    m_btnOK.ModifyStyle(BS_TYPEMASK, BS_OWNERDRAW);
    m_btnCancel.ModifyStyle(BS_TYPEMASK, BS_OWNERDRAW);

    m_theme.ApplyToEdit(m_editPortA);
    m_theme.ApplyToEdit(m_editPortB);
    m_theme.ApplyToButton(m_chkEmuBrA);
    m_theme.ApplyToButton(m_chkEmuBrB);

    if (m_isEdit)
    {
        m_editPortA.SetWindowText(m_pair.portNameA != TEXT("-") ? m_pair.portNameA.c_str() : nullptr);
        m_editPortB.SetWindowText(m_pair.portNameB != TEXT("-") ? m_pair.portNameB.c_str() : nullptr);
        ::CheckDlgButton(m_hwnd, IDC_CHK_EMUBR_A, m_pair.emuBrA ? BST_CHECKED : BST_UNCHECKED);
        ::CheckDlgButton(m_hwnd, IDC_CHK_EMUBR_B, m_pair.emuBrB ? BST_CHECKED : BST_UNCHECKED);
        m_lblHint.SetWindowText(TEXT("Leave a field blank to keep the current port name."));
    }
    return 0;
}

void AddPairWindow::OnDestroy() {}
void AddPairWindow::OnSize(UINT, Size) { PositionControls(); }
void AddPairWindow::OnClose() { PostResult(false); }

BOOL AddPairWindow::OnEraseBkgnd(HDC hdc) { return m_theme.OnEraseBkgnd(hdc, m_hwnd); }
HBRUSH AddPairWindow::OnCtlColorStatic(HDC hdc, HWND h) { return m_theme.OnCtlColorStatic(hdc, h); }
HBRUSH AddPairWindow::OnCtlColorEdit(HDC hdc, HWND h) { return m_theme.OnCtlColorEdit(hdc, h); }
HBRUSH AddPairWindow::OnCtlColorBtn(HDC hdc, HWND h) { return m_theme.OnCtlColorBtn(hdc, h); }
void AddPairWindow::OnDrawItem(UINT, LPDRAWITEMSTRUCT dis) { m_theme.OnDrawButton(dis); }

void AddPairWindow::OnOK(UINT, int, Window) { PostResult(true); }
void AddPairWindow::OnCancel(UINT, int, Window) { PostResult(false); }

void AddPairWindow::PostResult(bool accepted)
{
    auto *r = new PairWindowResult;
    r->accepted = accepted;
    r->isEdit = m_isEdit;
    r->pairIndex = m_isEdit ? m_pair.pairIndex : -1;

    if (accepted)
    {
        std::tstring portA(256, L'\0'), portB(256, L'\0');
        ::GetWindowText(m_editPortA, portA.data(), 256);
        ::GetWindowText(m_editPortB, portB.data(), 256);
        portA.resize(_tcslen(portA.c_str()));
        portB.resize(_tcslen(portB.c_str()));
        r->params.portNameA = portA;
        r->params.portNameB = portB;
        r->params.emuBrA = (::IsDlgButtonChecked(m_hwnd, IDC_CHK_EMUBR_A) == BST_CHECKED);
        r->params.emuBrB = (::IsDlgButtonChecked(m_hwnd, IDC_CHK_EMUBR_B) == BST_CHECKED);
    }
    HWND hOwner = ::GetWindow(m_hwnd, GW_OWNER);
    ::PostMessage(hOwner, WM_PAIRWINDOW_DONE, WPARAM(accepted ? 1 : 0), LPARAM(r));
    DestroyWindow();
}

void AddPairWindow::PositionControls()
{
    Rect rc;
    GetClientRect(&rc);
    const int w = rc.Width(), pad = 10;
    const int lblH = 16, editH = 22, chkH = 20, btnH = 26, btnW = 80, row = 4;
    const int btnY = rc.Height() - btnH - pad;
    int y = pad;
    m_lblPortA.SetWindowPos(nullptr, pad, y, w - 2 * pad, lblH, SWP_NOZORDER);
    y += lblH + row;
    m_editPortA.SetWindowPos(nullptr, pad, y, w - 2 * pad, editH, SWP_NOZORDER);
    y += editH + row;
    m_chkEmuBrA.SetWindowPos(nullptr, pad, y, w - 2 * pad, chkH, SWP_NOZORDER);
    y += chkH + pad;
    m_lblPortB.SetWindowPos(nullptr, pad, y, w - 2 * pad, lblH, SWP_NOZORDER);
    y += lblH + row;
    m_editPortB.SetWindowPos(nullptr, pad, y, w - 2 * pad, editH, SWP_NOZORDER);
    y += editH + row;
    m_chkEmuBrB.SetWindowPos(nullptr, pad, y, w - 2 * pad, chkH, SWP_NOZORDER);
    y += chkH + pad;
    m_lblHint.SetWindowPos(nullptr, pad, y, w - 2 * pad, lblH * 2, SWP_NOZORDER);
    m_btnCancel.SetWindowPos(nullptr, w - pad - btnW, btnY, btnW, btnH, SWP_NOZORDER);
    m_btnOK.SetWindowPos(nullptr, w - pad - btnW * 2 - row, btnY, btnW, btnH, SWP_NOZORDER);
}
