
#include "poppad2.h"
#include <WinUtility/Numbers.h>

LPARAM PopPad2::OnFind(WPARAM wParam, LPARAM lParam)
{
    auto pfr = (LPFINDREPLACE)lParam;
    if (pfr->Flags & FR_DIALOGTERM)
    {
        HWND hdlg = m_findreplaceDialog.Detach();
        hdlg = nullptr;
    }

    if (pfr->Flags & FR_FINDNEXT)
    {
        if (!PopFindSearchText(iSearchText, pfr))
        {
            auto res = MessageBox::Show(std::format(TEXT("{:s}{:s}"), ((iSearchText > 0) ? TEXT("Reached the end") : TEXT("Text not found!")  ), ((iSearchText > 0) ? TEXT("Would you like to re-start from the beginning?") : TEXT(""))).c_str(),
                                        TEXT("POPPAD2"),
                                        iSearchText > 0 ? MessageBoxButtons::YesNoCancel : MessageBoxButtons::Ok,
                                        MessageBoxIcon::Information);
            switch (res)
            {
            case MessageBoxResult::Yes: 
                iSearchText = (iSearchText > 0 ? 0 : iSearchText);
                break;
            case MessageBoxResult::No:
                return 0;
                break;
            case MessageBoxResult::Cancel:
                return 0;
                break;
            default:
                return 0;
                break;
            }
        }
    }

    if (pfr->Flags & FR_REPLACE || pfr->Flags & FR_REPLACEALL)
    {
        if (!PopFindReplaceText(iSearchText, pfr))
        {
            auto res = MessageBox::Show(iSearchText > 0 && pfr->Flags & FR_REPLACEALL ? TEXT("All occurences have been replaced! Start from the beginning?") : TEXT("Text not found!"), TEXT("POPPAD2"), iSearchText > 0 && pfr->Flags & FR_REPLACEALL ? MessageBoxButtons::YesNo : MessageBoxButtons::Ok, MessageBoxIcon::Exclamation);
            if (res == MessageBoxResult::Ok && pfr->Flags & FR_REPLACEALL)
                iSearchText = 0;
        }
    }

    if (pfr->Flags & FR_REPLACEALL)
    {
        bool startedFront = iSearchText == 0;
        while (PopFindReplaceText(iSearchText, pfr))
            ;
        if (!startedFront)
        {
            iSearchText = 0;
            while (PopFindReplaceText(iSearchText, pfr))
                ;
        }

        // std::tstring data;
        // m_edit.GetWindowText(data);
        // std::tstring from = pfr->lpstrFindWhat;
        // std::tstring to = pfr->lpstrReplaceWith;

        // size_t start_pos = 0;
        // while( (start_pos = data.find(from,start_pos)) != std::string::npos)
        // {
        //     data.replace(start_pos, from.length(), to);
        //     start_pos += to.length();
        // }
        // m_edit.SetWindowText(data.c_str());
    }

    return 0;
}

HWND PopPad2::PopFindDlg()
{
    m_findText.clear();
    m_findText.resize(MAX_PATH, '\0');
    fr = {0};
    fr.lStructSize = sizeof(FINDREPLACE);
    fr.hwndOwner = *this;
    fr.hInstance = NULL;
    fr.Flags = FR_HIDEUPDOWN | FR_HIDEMATCHCASE | FR_HIDEWHOLEWORD;
    fr.lpstrFindWhat = &m_findText[0];
    fr.lpstrReplaceWith = nullptr;
    fr.wFindWhatLen = convert_to<WORD>(m_findText.length());
    fr.wReplaceWithLen = 0;
    fr.lCustData = 0;
    fr.lpTemplateName = nullptr;
    return FindText(&fr);
}

HWND PopPad2::PopFindReplaceDlg()
{
    if (m_findText.size() == 0)
    {
        m_findText.clear();
        m_findText.resize(MAX_PATH, '\0');
    }
    m_replaceText.clear();
    m_replaceText.resize(MAX_PATH, '\0');
    fr = {0};
    fr.lStructSize = sizeof(FINDREPLACE);
    fr.hwndOwner = *this;
    fr.hInstance = NULL;
    fr.Flags = FR_HIDEUPDOWN | FR_HIDEMATCHCASE | FR_HIDEWHOLEWORD;
    fr.lpstrFindWhat = &m_findText[0];
    fr.lpstrReplaceWith = &m_replaceText[0];
    fr.wFindWhatLen = convert_to<WORD>(m_findText.length());
    fr.wReplaceWithLen = convert_to<WORD>(m_replaceText.length());
    fr.lCustData = 0;
    fr.lpTemplateName = nullptr;
    return ReplaceText(&fr);
}

BOOL PopPad2::PopFindSearchText(const int &iSearchOffset, LPFINDREPLACE pfr)
{
    std::tstring buffer;
    m_edit.GetWindowText(buffer);
    auto pos = buffer.find(pfr->lpstrFindWhat, iSearchOffset);
    if (pos == std::string::npos)
        return FALSE;

    iSearchText = convert_to<int>(pos) + lstrlen(pfr->lpstrFindWhat);

    m_edit.SetSel(convert_to<int>(pos), iSearchText, 0);
    m_edit.ScrollCaret();
    m_edit.Invalidate();
    this->SetFocus();
    return TRUE;
}

BOOL PopPad2::PopFindNextText()
{
    fr = {0};
    fr.lpstrFindWhat = const_cast<wchar_t *>(m_findText.c_str());
    fr.wFindWhatLen = convert_to<WORD>(m_findText.length());
    return PopFindSearchText(iSearchText, &fr);
}

BOOL PopPad2::PopFindReplaceText(const int &iSearchOffset, LPFINDREPLACE pfr)
{
    if (!PopFindSearchText(iSearchOffset, pfr))
        return FALSE;

    m_edit.ReplaceSel(pfr->lpstrReplaceWith, TRUE);
    return TRUE;
}

BOOL PopPad2::PopFindValidFind()
{
    return m_findText.length() > 0;
}

void PopPad2::OnSearchFind(UINT uNotifyCode, int nID, Window wndCtl)
{
    if (!m_findreplaceDialog.IsWindow())
    {
        int istart = 0, iend = 0;
        m_edit.GetSel(istart, iend);
        iSearchText = istart > 0 ? istart : 0;

        m_findreplaceDialog.Attach(PopFindDlg());
    }
}

void PopPad2::OnSearchReplace(UINT uNotifyCode, int nID, Window wndCtl)
{
    if (!m_findreplaceDialog.IsWindow())
    {
        int istart = 0, iend = 0;
        m_edit.GetSel(istart, iend);
        iSearchText = istart > 0 ? istart : 0;

        m_findreplaceDialog.Attach(PopFindReplaceDlg());
        if (!m_findreplaceDialog.IsWindow())
            MessageBox::Show(TEXT("Failed to create the replace dialog wtf?"), TEXT("POPAD2"), MessageBoxButtons::Ok, MessageBoxIcon::Information);
    }
}
