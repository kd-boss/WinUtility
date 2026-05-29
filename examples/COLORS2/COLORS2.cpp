
#include "COLORS2.h"
HRESULT Colors2::Initialize()
{
	HRESULT hr = S_OK;
	//get app title from resource file.
	std::tstring apptitle;
    apptitle.resize(256);
    LoadString(HINST_THISCOMPONENT,IDS_APP_TITLE,apptitle.data(),apptitle.length());
    apptitle.shrink_to_fit();

	hr = ::IsWindow(Create(nullptr, &Window::rcDefault, apptitle.c_str(), IDC_COLORS2)) ? S_OK : E_FAIL;
	if(SUCCEEDED(hr))
	{   
		ModifyStyle(0,WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_BORDER | WS_OVERLAPPEDWINDOW);
		ModifyStyleEx(0,WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
    	ShowWindow(ShowWindowType::Normal);
    	UpdateWindow();
	}
	return hr;
}

void Colors2::OnExit(UINT uNotifyCode, int nID, Window wndCtl)
{
	OnClose();
}

void Colors2::OnAbout(UINT uNotifyCode, int nID, Window wndCtl)
{

	if(!about)
	{
		if(about.DoModal() == IDOK)
		{
			MessageBox::Show(TEXT("OK pressed!"), TEXT("WinTest"),MessageBoxButtons::Ok, MessageBoxIcon::Information);
		}
	}

}

void Colors2::OnClose()
{
    PostQuitMessage(0);
}

int Colors2::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//additional window initalization would go here.

	
	if(scrolldlg.Create(*this))
		scrolldlg.ShowWindowAsync(ShowWindowType::Normal);
    return 0;
}

void Colors2::OnLButtonDown(UINT nFlags, const Point &pt)
{
    	std::tstringstream str;
        str << TEXT("Clicked At: ") << pt.x << TEXT(",") << pt.y;
        MessageBox::Show(str.str().c_str(), TEXT("WinTest"),MessageBoxButtons::Ok , MessageBoxIcon::Information);
}
