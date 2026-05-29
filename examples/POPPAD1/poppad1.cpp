/*
    WinTest, Small Demo application that creates a user interface window.
    release build - compile with :
		winres -i wintest.rc -o wintestrc.o
        g++ wintest.cpp wintestrc.o -municode -mwindows -s -O3 -o wintest.exe
    debug build - compile with :
		winres -i wintest.rc -o wintestrc.o
        g++ wintest.cpp wintestrc.o -municode -mwindows -g -o wintest.exe
*/

#include "poppad1.h"

HRESULT PopPad1::Initialize()
{
	HRESULT hr = S_OK;
	//get app title from resource file.
	std::tstring apptitle;
    apptitle.resize(256);
    LoadString(HINST_THISCOMPONENT,IDS_APP_TITLE,apptitle.data(),apptitle.length());
    apptitle.shrink_to_fit();

	hr = ::IsWindow(Create(nullptr, &Window::rcDefault, apptitle.c_str())) ? S_OK : E_FAIL;
	if(SUCCEEDED(hr))
	{ 
    	ShowWindow(ShowWindowType::Normal);
    	UpdateWindow();
	}
	return hr;
}

void PopPad1::OnExit(UINT uNotifyCode, int nID, Window wndCtl)
{
	OnClose();
}

void PopPad1::OnAbout(UINT uNotifyCode, int nID, Window wndCtl)
{

	if(!about)
	{
		if(about.DoModal() == IDOK)
		{
			MessageBox::Show(TEXT("OK pressed!"), TEXT("WinTest"),MessageBoxButtons::Ok, MessageBoxIcon::Information);
		}
	}

}

void PopPad1::OnClose()
{
    PostQuitMessage(0);
}

int PopPad1::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//additional window initalization would go here.
	Rect rc;
	GetClientRect(&rc);
	HR(::IsWindow(m_edit.Create(*this,rc,nullptr,WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_BORDER | ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,0,std::to_underlying(PopPadControls::EDIT), nullptr)) ? S_OK : E_FAIL);
    return 0;
}

void PopPad1::OnSize(UINT nFlags, Size sz)
{
	m_edit.MoveWindow(0,0, sz.cx, sz.cy);
}

void PopPad1::OnEdit(UINT uNotifyCode, int nID, Window wndCtl)
{
	switch(uNotifyCode)
	{
		case EN_ERRSPACE:
		case EN_MAXTEXT:
		MessageBox::Show(TEXT("Edit control out of space"), TEXT("PopPad1"), MessageBoxButtons::Ok, MessageBoxIcon::Error, MessageBoxOtherOptions::TopMost);
		break;
	};
}

void PopPad1::OnRightJustified(UINT uNotifyCode, int nID, Window wndCtl)
{
	auto style = m_edit.GetStyle();
	
	m_edit.ModifyStyle(ES_LEFT, ES_RIGHT);
	m_edit.ModifyStyle(ES_CENTER, ES_RIGHT);
	m_edit.ModifyStyle(0, ES_RIGHT);
	
	style = m_edit.GetStyle();
	if((style & ES_RIGHT) == ES_RIGHT)
		m_edit.Invalidate(TRUE);
}

void PopPad1::OnLeftJustified(UINT uNotifyCode, int nID, Window wndCtl)
{
	auto style = m_edit.GetStyle();

	m_edit.ModifyStyle(ES_RIGHT, ES_LEFT);
	m_edit.ModifyStyle(ES_CENTER, ES_LEFT);
	m_edit.ModifyStyle(0, ES_LEFT);

	style = m_edit.GetStyle();
	if((style & ES_LEFT) == ES_LEFT)
		m_edit.Invalidate(TRUE);
}

void PopPad1::OnCenterJustified(UINT uNotifyCode, int nID, Window wndCtl)
{
	auto style = m_edit.GetStyle();
	
	m_edit.ModifyStyle(ES_RIGHT, ES_CENTER);
	m_edit.ModifyStyle(ES_LEFT, ES_CENTER);
	m_edit.ModifyStyle(0, ES_CENTER);
	
	style = m_edit.GetStyle();
	if((style & ES_CENTER) == ES_CENTER)
		m_edit.Invalidate(TRUE);

}

void PopPad1::OnCopy(UINT uNotifyCode, int nID, Window wndCtl)
{
	m_edit.Copy();
}

void PopPad1::OnCut(UINT uNotifyCode, int nID, Window wndCtl)
{
	m_edit.Cut();
}

void PopPad1::OnPaste(UINT uNotifyCode, int nID, Window wndCtl)
{
	m_edit.Paste();
}

void PopPad1::OnClear(UINT uNotifyCode, int nID, Window wndCtl)
{
	m_edit.Clear();
}
