

#include "poppad2.h"
#include <format>

void PopPad2::DoCaption(LPCTSTR Caption)
{
	caption = std::format(TEXT("{:s} - {:s}"), m_appname, Caption);
	SetWindowText(caption.c_str());
}

HRESULT PopPad2::Initialize()
{
	HRESULT hr = S_OK;
	// get app title from resource file.
	std::tstring apptitle;
	apptitle.resize(256);
	LoadString(HINST_THISCOMPONENT, IDS_APP_TITLE, apptitle.data(), apptitle.length());
	apptitle = System::Utility::trim(apptitle);
	m_appname = apptitle.c_str();

	hr = ::IsWindow(Create((HWND) nullptr, &Window::rcDefault, apptitle.c_str(), (DWORD)0UL, (DWORD)0UL)) ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{
		
		UpdateWindow();
	}
	return hr;
}

void PopPad2::OnExit(UINT uNotifyCode, int nID, Window wndCtl)
{
	OnClose();
}

void PopPad2::OnClose()
{
	DestroyWindow();
}

void PopPad2::OnDestroy()
{
	PostQuitMessage(0);
	PopFontDeinitialize();
}

void PopPad2::OnAbout(UINT uNotifyCode, int nID, Window wndCtl)
{
	if (!about)
	{
		auto dlgres = about.DoModal();
		if(dlgres == IDOK)
		{
			about.DestroyWindow();
		}	
		else if(dlgres == -1)
		{
			HR(__HRESULT_FROM_WIN32(GetLastError()));
		}
	}
}


int PopPad2::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// additional window initalization would go here.
	
	WM_FIND = RegisterWindowMessage(FINDMSGSTRING);
	Rect rc;
	GetClientRect(&rc);
	rc.top -= 100;
	rc.right -= 100;
	HR(::IsWindow(m_edit.Create(*this, rc, nullptr, WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL, 0, std::to_underlying(PopPadControls::EDIT), nullptr)) ? S_OK : E_FAIL);
	PopFontInitialize();
	ShowWindow(ShowWindowType::Default);	
	return 0;
}

void PopPad2::OnSize(UINT nFlags, Size sz)
{
	m_edit.MoveWindow(0, 0, sz.cx, sz.cy);
}

void PopPad2::OnEdit(UINT uNotifyCode, int nID, Window wndCtl)
{
	switch (uNotifyCode)
	{
	case EN_ERRSPACE:
	case EN_MAXTEXT:
		MessageBox::Show(TEXT("Edit control out of space"), TEXT("PopPad1"), MessageBoxButtons::Ok, MessageBoxIcon::Error, MessageBoxOtherOptions::TopMost);
		break;
	};
}

void PopPad2::OnCopy(UINT uNotifyCode, int nID, Window wndCtl)
{
	m_edit.Copy();
}

void PopPad2::OnCut(UINT uNotifyCode, int nID, Window wndCtl)
{
	m_edit.Cut();
}

void PopPad2::OnPaste(UINT uNotifyCode, int nID, Window wndCtl)
{
	m_edit.Paste();
}

void PopPad2::OnClear(UINT uNotifyCode, int nID, Window wndCtl)
{
	m_edit.Clear();
}

void PopPad2::OnUnDo(UINT uNotifyCode, int nID, Window wndCtl)
{
	m_edit.Undo();
}

void PopPad2::OnSelectAll(UINT uNotifyCode, int nID, Window wndCtl)
{
	m_edit.SetSelAll();
}

void PopPad2::OnSetFocus(Window wndOld)
{
	m_edit.SetFocus();
}

void PopPad2::OnInitMenuPopup(HMENU menuPopup, UINT nIndex, BOOL bSysMenu)
{
	if (nIndex == 1)
	{
		EnableMenuItem(menuPopup, IDM_EDIT_UNDO,
					   m_edit.CanUndo() ? MF_ENABLED : MF_GRAYED);

		EnableMenuItem(menuPopup, IDM_EDIT_PASTE,
					   IsClipboardFormatAvailable(CF_TEXT) ? MF_ENABLED : MF_GRAYED);
		int iEnabled;
		DWORD iSelect = m_edit.GetSel();
		if (HIWORD(iSelect) == LOWORD(iSelect))
		{
			iEnabled = MF_GRAYED;
		}
		else
		{
			iEnabled = MF_ENABLED;
		}

		EnableMenuItem(menuPopup, IDM_EDIT_CUT, iEnabled);
		EnableMenuItem(menuPopup, IDM_EDIT_COPY, iEnabled);
		EnableMenuItem(menuPopup, IDM_EDIT_CLEAR, iEnabled);
	}
}
