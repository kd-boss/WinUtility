
#include "BtnLook.h"
#include <format>
#include <WinUtility/Numbers.h>



HRESULT BtnLook::Initialize()
{
	HRESULT hr = S_OK;
	//get app title from resource file.
	std::tstring apptitle;
    apptitle.resize(convert_to<size_t>(256));
    LoadString(HINST_THISCOMPONENT,IDS_APP_TITLE,apptitle.data(),convert_to<int>(apptitle.length()));
    apptitle.shrink_to_fit();

	hr = ::IsWindow(Create(nullptr, &Window::rcDefault, apptitle.c_str())) ? S_OK : E_FAIL;
	if(SUCCEEDED(hr))
	{ 
    	ShowWindow(ShowWindowType::Normal);
    	UpdateWindow();
	}
	return hr;
}

void BtnLook::OnExit(UINT uNotifyCode, int nID, Window wndCtl)
{
	OnClose();
}

void BtnLook::OnAbout(UINT uNotifyCode, int nID, Window wndCtl)
{

	if(!about)
	{
		if(about.DoModal() == IDOK)
		{
			MessageBox::Show(TEXT("OK pressed!"), TEXT("WinTest"),MessageBoxButtons::Ok, MessageBoxIcon::Information);
		}
	}

}
void BtnLook::OnDefPushButton(UINT uNotifyCode, int nID, Window wndCtl)
{
	m_commandLink.EnableWindow(!m_commandLink.IsWindowEnabled());
	m_AutoCheckBox.EnableWindow(!m_AutoCheckBox.IsWindowEnabled());
	m_autoRadioButton.EnableWindow(!m_autoRadioButton.IsWindowEnabled());
	m_autoTriCheck.EnableWindow(!m_autoTriCheck.IsWindowEnabled());
	
}


void BtnLook::OnClose()
{
    PostQuitMessage(0);
}

int BtnLook::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//additional window initalization would go here.
	int height = 40;
	Rect rc{10,10,290,10 + height};
	
	m_ownerdraw.Create(*this,rc, TEXT("OWNERDRAWN"), std::to_underlying(BtnLookControls::OWNERDRAW));
	rc.bottom += height;
	rc.top += height;
	m_triStateBtn.Create(*this,rc, TEXT("TriState"), std::to_underlying(BtnLookControls::TRISTATEBUTTON));
	rc.bottom += height;
	rc.top += height;
	m_checkBoxCtrl.Create(*this,rc, TEXT("CheckBox"), std::to_underlying(BtnLookControls::CHECKBOX));
	rc.bottom += height;
	rc.top += height;
	m_SplitButtonControl.Create(*this,rc, TEXT("SplitButton"), std::to_underlying(BtnLookControls::SPLITBUTTON));
	rc.bottom += height;
	rc.top += height;
	m_AutoCheckBox.Create(*this,rc, TEXT(""), std::to_underlying(BtnLookControls::AUTOCHECKBOX));
	rc.bottom += height;
	rc.top += height;
	m_DefPushButton.Create(*this,rc, TEXT("DefPushButton"), std::to_underlying(BtnLookControls::DEFPUSHBUTTON));
	rc.bottom += height;
	rc.top += height;
	m_PushButtonCtrl.Create(*this,rc, TEXT(""), std::to_underlying(BtnLookControls::PUSHBUTTON));
	rc.bottom += height;
	rc.top += height;
	m_commandLink.Create(*this,rc, TEXT("https://www.google.com"), std::to_underlying(BtnLookControls::COMMANDLINK));
	rc.bottom += height;
	rc.top += height;
	m_autoRadioButton.Create(*this,rc, TEXT(""), std::to_underlying(BtnLookControls::AUTORADIO));
	rc.bottom += height;
	rc.top += height;
	m_autoTriCheck.Create(*this,rc, TEXT(""), std::to_underlying(BtnLookControls::AUTOTRISTATE));
	rc.bottom += height;
	rc.top += height;
	m_groupBox.Create(*this,rc, TEXT("GroupBox"), std::to_underlying(BtnLookControls::GROUPBOX));
	
    return 0;
}

void BtnLook::OnLButtonDown(UINT nFlags, const Point &pt)
{
		auto str = std::format(TEXT("Clicked At: {},{}"), pt.x, pt.y);
        MessageBox::Show(str.c_str(), TEXT("WinTest"),MessageBoxButtons::Ok , MessageBoxIcon::Information);
}
