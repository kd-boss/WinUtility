#include "About2.h"


void About2::OnPaint(DC dc)
{
    PAINTSTRUCT ps;
    BeginPaint(&ps);
    EndPaint(&ps);
    m_about.PaintWindow(*this, iColor,iFigure);
}

void About2::OnDestroy()
{
    PostQuitMessage(0);
}

void About2::OnAbout(UINT uNotifyCode, int nID, Window wndCtl)
{
    int cyan = IDC_CYAN;

    m_about.iCurrentColor = iColor;
    m_about.iCurrentFigure = iFigure;
    if(m_about.DoModal() == IDOK)
    {
        iFigure = m_about.iCurrentFigure;
        iColor = m_about.iCurrentColor;
        Invalidate(TRUE);
        m_about.PaintWindow(*this, iColor, iFigure);
    }
}
