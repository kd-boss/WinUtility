#include "Checker3.h"
#include <WinUtility/Numbers.h>

HRESULT Checker3::Initialize()
{
	
	HRESULT hr = S_OK;
	hr = CreateDeviceIndependantResources();
	// get app title from resource file.
	std::tstring apptitle;
	apptitle.resize(256);
	LoadString(HINST_THISCOMPONENT, IDS_APP_TITLE, apptitle.data(), convert_to<int>(apptitle.length()));
	apptitle.shrink_to_fit();
	hr = ::IsWindow(Create(nullptr, &Window::rcDefault, apptitle.c_str())) ? S_OK : E_FAIL;
	return hr;
}

HRESULT Checker3::CreateDeviceIndependantResources()
{
    HRESULT hr = S_OK;
    if(!m_factory)
    {
        hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_factory.GetAddressOf());
    }
    return hr;
}

void Checker3::OnExit(UINT uNotifyCode, int nID, Window wndCtl)
{
	OnClose();
}

void Checker3::OnAbout(UINT uNotifyCode, int nID, Window wndCtl)
{
	if (!about)
	{
		if (about.DoModal() == IDOK)
		{
			MessageBox::Show(TEXT("OK pressed!"), TEXT("WinTest"), MessageBoxButtons::Ok, MessageBoxIcon::Information);
		}
	}
}

void Checker3::OnSize(UINT nFlags, Size size)
{
	int i = 0, j = 0;
	for(int n = 0; n < 25 ; n++)
	{
		i = n / 5;
		j = n % 5;
		m_windows[i][j].MoveWindow(j * (size.cx / 5), i * (size.cy / 5),(size.cx / 5), (size.cy / 5), FALSE);
		m_windows[i][j].Invalidate(FALSE);	
	}
	Invalidate(FALSE);
}

void Checker3::OnClear(UINT uNotifyCode, int nID, Window wndCtl)
{
	int i = 0, j = 0;
	for(int n = 0; n < 25 ; n++)
	{
		i = n / 5;
		j = n % 5;
		m_windows[i][j].SetChecked(false);
		m_windows[i][j].Invalidate(FALSE);
		
	}
}

void Checker3::OnClose()
{
	PostQuitMessage(0);
}

int Checker3::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	Rect rc;
	GetClientRect(&rc);
	auto rwidth = rc.Width() / 5;
	auto rheight = rc.Width() / 5;

	for(int n = 0; n < 25 ; n++)
	{	
		int i = n / 5;
		int j = n % 5;
		if(SUCCEEDED(m_windows[i][j].Initalizie((IDC_CHECKER3_CHILD + n), *this,m_factory)))
		{
			m_windows[i][j].MoveWindow(j * rwidth, i * rheight, rwidth, rheight, false);
			m_windows[i][j].Invalidate(FALSE);
		}
	}
	ShowWindow(ShowWindowType::Normal);
	Invalidate(FALSE);
	return 0;
}

void Checker3::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	 	Point pt;
        GetCursorPos(&pt);
        ScreenToClient(&pt);
		Rect rc;
		GetClientRect(&rc);
    	auto divisioncx = rc.Width() / 5.f;
        auto divisioncy = rc.Height() / 5.f;
        auto x = static_cast<int>(std::max(0.f, std::min(4.f, pt.x / divisioncx)));
        auto y = static_cast<int>(std::max(0.f, std::min(4.f, pt.y / divisioncy)));

        switch(nChar)
        {
            case VK_UP:
            	y--;
            break;
            case VK_DOWN:
            	y++;
            break;
            case VK_RIGHT:
            	x++;
            break;
            case VK_LEFT:
            	x--;
            break;
            case VK_HOME:
            	x = y = 0;
            break;
            case VK_END:
            	x = y = 4;
            break;
            case VK_RETURN:
            case VK_SPACE:
            {
				m_windows[y][x].SendMessage(WM_LBUTTONDOWN, MK_LBUTTON,
                            MAKELONG(static_cast<int>(x * divisioncx + divisioncx/2.f),
                                             static_cast<int>(y * divisioncy + divisioncy/2.f))
                                    );
            }
            break;
        };

        pt.x = static_cast<int>(x * divisioncx + divisioncx/2.f);
        pt.y = static_cast<int>(y * divisioncy + divisioncy/2.f);
        ClientToScreen(&pt);
        SetCursorPos(pt.x, pt.y);
}