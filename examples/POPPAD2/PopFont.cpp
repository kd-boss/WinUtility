#include "poppad2.h"

void PopPad2::OnFont(UINT uNotifyCode, int nID, Window wndCtl)
{
    if(PopFontChooseFont())
        PopFontSetFont();
}

BOOL PopPad2::PopFontChooseFont()
{
    CHOOSEFONT cf = {0};
    cf.lStructSize = sizeof(CHOOSEFONT);
    cf.hwndOwner = this->m_hwnd;
    cf.hDC = nullptr;
    cf.lpLogFont = &m_logFont;
    cf.iPointSize = 0;
    cf.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;
    cf.rgbColors = 0;
    cf.lCustData = 0;
    cf.lpfnHook = nullptr;
    cf.lpTemplateName = nullptr;
    cf.hInstance = NULL;
    cf.lpszStyle = nullptr;
    cf.nFontType = 0;
    cf.nSizeMin = 0;
    cf.nSizeMax = 0;

    return ChooseFont(&cf);
}

void PopPad2::PopFontInitialize()
{
    GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), (PTSTR)&m_logFont);
    hFont = CreateFontIndirect(&m_logFont);
    m_edit.SetFont(hFont);
}

void PopPad2::PopFontSetFont()
{
    HFONT hFontNew;
    Rect rc;

    hFontNew = CreateFontIndirect(&m_logFont);
    m_edit.SetFont(hFontNew);

    DeleteObject(hFont);
    hFont = hFontNew;
    m_edit.Invalidate();
}

void PopPad2::PopFontDeinitialize()
{
    DeleteObject(hFont);
}