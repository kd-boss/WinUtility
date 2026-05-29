#include "ScrollDialog.h"

BOOL ScrollDialog::OnInit(Window wndFocus, LPARAM lInitParam)
{
    sb_red = GetDlgItem(10);
    sb_green = GetDlgItem(11);
    sb_blue = GetDlgItem(12);

    sb_red.SetScrollRange(0,255);
    sb_green.SetScrollRange(0,255);
    sb_blue.SetScrollRange(0,255);

    sb_red.SetScrollPos(0,false);
    sb_green.SetScrollPos(0,false);
    sb_blue.SetScrollPos(0,false);

    return TRUE;
}

BOOL ScrollDialog::OnVScroll(UINT nSBCode, UINT nPos, ScrollBarControl pScrollBar)
{
    auto pos = pScrollBar.GetScrollPos();
    switch(nSBCode)
    {
        case SB_PAGEDOWN:
            pos += 15;
        break;
        case SB_LINEDOWN:
            pos = std::min(255, pos + 1);
        break;
        case SB_PAGEUP:
            pos -= 15;
        break;
        case SB_LINEUP:
            pos = std::max(0, pos - 1);
        break;
        case SB_TOP:
            pos = 0;
        break;
        case SB_BOTTOM:
            pos = 255;
        break;
        case SB_THUMBPOSITION:
        case SB_THUMBTRACK:
        {
            pos = nPos;
        }
        break;
        default:
            return FALSE;

    };
    
    pScrollBar.SetScrollPos(pos);
    SetDlgItemInt(*this,pScrollBar.GetDlgCtrlID() + 3,255 - pos, FALSE);
    DeleteObject((HGDIOBJ)SetClassLongPtr(this->GetParent(), GCLP_HBRBACKGROUND,(LONG_PTR)CreateSolidBrush(RGB(255 - sb_red.GetScrollPos(),255 - sb_green.GetScrollPos(), 255 - sb_blue.GetScrollPos()))));
    ::InvalidateRect(this->GetParent(), NULL, TRUE);
    return TRUE;

}