#include "AboutDialog.h"

void AboutDialog::PaintWindow(HWND hwnd, int iColor, int iFigure)
{
    auto hdc = ::GetDC(hwnd);
    Rect rc;
    ::GetClientRect(hwnd, &rc);
    auto hBrush = ::CreateSolidBrush(crCurrentColor);
    hBrush = (HBRUSH)::SelectObject(hdc,hBrush);

    if(iFigure == IDC_RECT)
        ::Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
    else
        ::Ellipse(hdc, rc.left, rc.top, rc.right,rc.bottom);
    
    ::DeleteObject(SelectObject(hdc, hBrush));
    ::ReleaseDC(hwnd, hdc);
}

void AboutDialog::OnColor(UINT uNotifyCode, INT nId, Window wndCtl)
{
    iCurrentColor = nId;
    CheckRadioButton(IDC_BLACK, IDC_WHITE, iCurrentColor);
    int idxColor = iCurrentColor - IDC_BLACK;
    crCurrentColor = crColor[idxColor];
    auto block = GetDlgItem(IDC_PAINT);
    block.Invalidate(true);
    block.UpdateWindow();
    PaintWindow(block, iCurrentColor, iCurrentFigure);
}

void AboutDialog::OnFigure(UINT uNotifyCode, INT nId, Window wndCtl)
{
    iCurrentFigure = nId;
    CheckRadioButton(IDC_RECT, IDC_ELLIPSE, iCurrentFigure);
    auto block = GetDlgItem(IDC_PAINT);
    block.Invalidate(true);
    block.UpdateWindow();
    PaintWindow(block, iCurrentColor, iCurrentFigure);

}

void AboutDialog::OnOk(UINT uNotifyCode, INT nId, Window wndCtl)
{
    EndDialog(TRUE);
}

void AboutDialog::OnClose()
{

}

void AboutDialog::OnCancel(UINT uNotifyCode, INT nId, Window wndCtl)
{
    EndDialog(FALSE);
}

BOOL AboutDialog::OnInitDialog(Window focused, LPARAM initParam)
{
    CheckRadioButton(IDC_BLACK, IDC_WHITE, iCurrentColor);
    CheckRadioButton(IDC_RECT, IDC_ELLIPSE, iCurrentFigure);
    GetDlgItem(iCurrentColor).SetFocus();
    auto item = GetDlgItem(IDC_CYAN);
    if(item.GetDlgCtrlID()  != IDC_CYAN)
    {
        std::cout << "Error with CYAN" << std::endl;
    }
    
    
    return true;
}
