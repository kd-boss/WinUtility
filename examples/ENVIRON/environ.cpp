

#include "environ.h"
#include<WinUtility/Numbers.h>

HRESULT Environ::Initialize()
{
	HRESULT hr = S_OK;
	// get app title from resource file.
	std::tstring apptitle;
	apptitle.resize(256);
	LoadString(HINST_THISCOMPONENT, IDS_APP_TITLE, apptitle.data(), convert_to<int>(apptitle.length()));
	apptitle.shrink_to_fit();

	hr = ::IsWindow(Create(nullptr, &Window::rcDefault, apptitle.c_str())) ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{

		ShowWindow(ShowWindowType::Normal);
		typedef std::basic_string<TCHAR> tstring;
		auto free = [](LPTCH p)
		{ FreeEnvironmentStrings(p); };
		auto env_block = std::unique_ptr<TCHAR, decltype(free)>{GetEnvironmentStrings(), free};
		for (LPTCH i = env_block.get(); *i != TEXT('\0'); ++i)
		{
			tstring key;
			for (; *i != TEXT('='); ++i)
			{
				key += *i;
			}
			++i;
			for (; *i != TEXT('\0'); ++i)
			{
			}
			m_listbox.AddString(key.c_str());
		}
		m_listbox.Invalidate(true);

		UpdateWindow();
	}
	return hr;
}

void Environ::OnExit(UINT uNotifyCode, int nID, Window wndCtl)
{
	OnClose();
}

void Environ::OnAbout(UINT uNotifyCode, int nID, Window wndCtl)
{
	if (!about)
	{
		if (about.DoModal() == IDOK)
		{
			MessageBox::Show(TEXT("OK pressed!"), TEXT("WinTest"), MessageBoxButtons::Ok, MessageBoxIcon::Information);
		}
	}
	//	std::wcout << TEXT("ABOUT Called!") << std::endl;
}

void Environ::OnClose()
{
	PostQuitMessage(0);
}

void Environ::OnSize(UINT nFlags, Size sz)
{
	Rect rc;
	GetClientRect(rc);
	sz = rc.Size();
	m_listbox.MoveWindow(cxChar, cyChar * 3, rc.Width() / 2 - cxChar, rc.Height() - (cyChar * 6));
	m_listView.MoveWindow(sz.cx / 2, cyChar * 3, (sz.cx / 2) - cxChar * 3, rc.Height() - (cyChar * 6));
	m_listView.ShowWindowAsync(true);
}

int Environ::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// additional window initalization would go here.
	Rect rc, rt;
	cxChar = LOWORD(GetDialogBaseUnits());
	cyChar = HIWORD(GetDialogBaseUnits());
	rc.left = cxChar;
	rc.top = cyChar * 3;
	rc.right = cxChar * 64 + GetSystemMetrics(SM_CXVSCROLL);
	rc.bottom = lpCreateStruct->cy - cyChar * 3;

	rt.left = cxChar;
	rt.top = cyChar,
	rt.right = lpCreateStruct->cx;
	rt.bottom = cyChar * 2;

	m_listbox.Create(*this, rc, TEXT(""), WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_STANDARD, 0, std::to_underlying(EnvironControls::ListBox));
	m_text.Create(*this, rt, TEXT(""), WS_CHILD | WS_VISIBLE | SS_LEFT, 0, std::to_underlying(EnvironControls::Static));
	m_text.ShowWindow(ShowWindowType::Normal);

	rc.left = cxChar * 65 + GetSystemMetrics(SM_CXVSCROLL);
	rc.right = rt.right;
	rc.top = cyChar * 3;
	rc.bottom = lpCreateStruct->cy;
	m_listView.Create(*this, rc, TEXT(""), LVS_ALIGNLEFT | LVS_ALIGNTOP | LVS_EDITLABELS | LVS_LIST | LVS_REPORT | LVS_SMALLICON | WS_CHILD | WS_BORDER | WS_OVERLAPPED | WS_VISIBLE, WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_STATICEDGE | LVS_EX_GRIDLINES, std::to_underlying(EnvironControls::ListView));
	m_listView.AddColumn(TEXT("value"), 1, 0, 0, 0);
	m_listView.ShowWindow(ShowWindowType::Normal);

	return 0;
}

void Environ::OnListBox(UINT uNotifyCode, int nID, Window wndCtl)
{
	switch (uNotifyCode)
	{
	case LBN_SELCHANGE:
	{
		auto index = m_listbox.GetCurSel();
		auto textlen = m_listbox.GetTextLen(index);
		std::wstring str;
		str.resize(textlen);
		m_listbox.GetText(index, &str[0]);
		auto varlen = GetEnvironmentVariable(str.c_str(), nullptr, 0);
		std::wstring var;
		var.resize(varlen);
		GetEnvironmentVariable(str.c_str(), &var[0], varlen);
		size_t next = 0, last = 0;
		m_listView.DeleteAllItems();
		while ((next = var.find(TEXT(";"), last)) != std::wstring::npos)
		{
			m_listView.AddItem(m_listView.GetItemCount(), 0, var.substr(last, next - last).c_str());
			last = next + 1;
		}
		m_listView.AddItem(m_listView.GetItemCount(), 0, var.substr(last).c_str());
		m_text.SetWindowTextW(var.c_str());
		m_text.Invalidate(TRUE);
	}
	break;
	}
}

BOOL Environ::OnListViewEndLabelEdit(const NMLVDISPINFOW& disp)
{
		if (disp.item.pszText)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
}


void Environ::OnUp(UINT uNotifyCode, int nID, Window wndCtl)
{
	LVITEM item = {0};

	text.resize(255);
	item.cchTextMax = convert_to<int>(text.size());
	item.pszText = text.data();
	item.mask |= LVIF_TEXT;
	item.iItem = m_listView.GetSelectedIndex();
	if (item.iItem != 0)
	{
		m_listView.GetItemText(item.iItem, 0, item.pszText);
		m_listView.DeleteItem(item.iItem);
		m_listView.SetItemState(item.iItem, 0, LVIS_SELECTED);
		item.iItem -= 1;
		m_listView.AddItem(item.iItem, 0, item.pszText, item.iImage);
		m_listView.SelectItem(item.iItem);
	}
	m_listView.Invalidate(true);
}

void Environ::OnDown(UINT uNotifyCode, int nID, Window wndCtl)
{
	LVITEM item = {0};

	text.resize(255);
	item.cchTextMax = convert_to<int>(text.size());
	item.pszText = text.data();
	item.mask |= LVIF_TEXT;
	item.iItem = m_listView.GetSelectedIndex();
	if (item.iItem != m_listView.GetItemCount() - 1)
	{
		m_listView.GetItemText(item.iItem, 0, item.pszText);
		m_listView.DeleteItem(item.iItem);
		item.iItem += 1;
		m_listView.AddItem(item.iItem, 0, item.pszText, item.iImage);
		m_listView.SelectItem(item.iItem);
	}
	m_listView.Invalidate(true);
}

void Environ::OnNew(UINT uNotifyCode, int nID, Window wndCtl)
{
	m_listView.SelectItem(m_listView.AddItem(m_listView.GetItemCount(), 0, TEXT("")));
}

void Environ::OnSave(UINT uNotifyCode, int nID, Window wndCtl)
{
	auto index = m_listbox.GetCurSel();
	auto textlen = m_listbox.GetTextLen(index);
	std::wstring str;
	str.resize(textlen);
	m_listbox.GetText(index, &str[0]);

	std::wstring value = TEXT("");
	for (int i = 0; i < m_listView.GetItemCount(); i++)
	{
		text.clear();
		text.resize(255);
		m_listView.GetItemText(i, 0, &text[0], convert_to<int>( text.size()));
		value += text.c_str();
		value += TEXT(";");
	}

	SetEnvironmentVariable(str.c_str(), value.c_str());
}

void Environ::OnDelete(UINT uNotifyCode, int nID, Window wndCtl)
{
	auto selected = m_listView.GetSelectedIndex();
	m_listView.DeleteItem(selected);
	m_listView.SelectItem(selected);
}