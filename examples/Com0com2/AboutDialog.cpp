#include "AboutDialog.hpp"

BOOL AboutDialog::OnInitDialog(Window wndFocus)
{
    SetLastError(ERROR_SUCCESS);
    m_icon = static_cast<HICON>(
        ::LoadImage(_BaseModule.m_hInstance,
                    MAKEINTRESOURCE(IDI_APP),
                    IMAGE_ICON, 16, 16,
                    LR_DEFAULTCOLOR));
    if (m_icon)
    {
        m_iconControl.Attach(GetDlgItem(IDC_ABOUT_ICON));
        SetLastError(ERROR_SUCCESS);
        auto ret = m_iconControl.SetIcon(m_icon);
        if(ret == 0)
              HR(__HRESULT_FROM_WIN32(GetLastError()));
              
    }
    else
    {
        HR(__HRESULT_FROM_WIN32(GetLastError()));
    }
    return TRUE;
}

LRESULT AboutDialog::OnLinkClick(NMHDR &pnmh)
{
    auto *pnml = reinterpret_cast<NMLINK *>(&pnmh);
    // szUrl is the href= value from the anchor tag.
    ::ShellExecuteW(*this, L"open",
                    pnml->item.szUrl,
                    nullptr, nullptr, SW_SHOWNORMAL);
    return TRUE;
}

void AboutDialog::OnClose(UINT uNotifyCode, int nID, Window wndCtl)
{
    if (m_icon){
        ::DestroyIcon(m_icon);
        m_icon = nullptr;
    }
    EndDialog(0);
}

void AboutDialog::OnDestroy()
{
    if (m_icon)
        ::DestroyIcon(m_icon);
}