#include "AboutDialog.h"


BOOL AboutDialog::OnInitDialog(Window wndFocus, LPARAM lInitParam)
{
    TEXTMETRIC tm = {0};
    DCT<true> hdc = GetDC();
    hdc.GetTextMetricsW(&tm);
    ReleaseDC(hdc);
    int cxChar = tm.tmAveCharWidth;
    int cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
    int cyChar = tm.tmHeight + tm.tmExternalLeading;
  
    btn.SetBackColor(D2D1::ColorF(D2D1::ColorF::SkyBlue));
    Rect rc = {80 * cxChar/4 + (32 * cxChar/4)/2, 79 * cyChar/8 + (14 * cyChar/8)/2, 32 * cxChar/4 , 14 * cyChar/8};
    
    btn.Create(*this,&rc,TEXT("OK"),WS_CHILD | WS_TABSTOP | WS_VISIBLE, 0UL, (HMENU)IDOK);
    btn.MoveWindow(80 * cxChar/4 + (32 * cxChar/4)/2, 79 * cyChar/8 + (14 * cyChar/8)/2, 32 * cxChar/4 , 14 * cyChar/8);
    btn.ShowWindowAsync(ShowWindowType::Show); 
    
    Rect ra = {60 * cxChar/6, 79 * cyChar/8 + (14 * cyChar/8)/2, 32 * cxChar/4, 14 * cyChar/8};
    cancel.SetBackColor(D2D1::ColorF(D2D1::ColorF::SkyBlue));
    cancel.Create(*this,&ra, TEXT("CANCEL"), WS_CHILD | WS_TABSTOP | WS_VISIBLE, 0UL,(HMENU)IDCANCEL);
    cancel.MoveWindow(60 * cxChar/6, 79 * cyChar/8 + (14 * cyChar/8)/2, 32 * cxChar/4, 14 * cyChar/8);
    cancel.ShowWindowAsync(ShowWindowType::Show);
    Invalidate(true);
    return TRUE;
}

BOOL AboutDialog::OnOK(UINT uNotifyCode, int nID, Window wndCtl)
{
    btn.DestroyWindow();
    cancel.DestroyWindow();
    return EndDialog(IDOK);
}

BOOL AboutDialog::OnCancel(UINT uNotifyCode, int nID, Window wndCtl)
{
    btn.DestroyWindow();
    return EndDialog(IDCANCEL);
}