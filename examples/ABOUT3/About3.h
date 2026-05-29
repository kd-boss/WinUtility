#define WINVER 0x0A00000D
#define _WIN32_WINNT 0x0A00000D

#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include "Resource.h"
#include "AboutDialog.h"

class About3 : public BaseWindow<About3, Window, FrameWinTraits>
{
    int OnCreate(LPCREATESTRUCT lpCreateStruct);
    BOOL OnMenuAbout(UINT uNotifyCode, int nID, Window wndCtl);
    void OnDestroy();
    void OnClose();
    AboutDialog m_dialog;
    public:
    DECLARE_WND_CLASS_1(IDC_ABOUT3, IDC_ABOUT3)

    BEGIN_MSG_MAP()
      MSG_WM_CREATE(OnCreate)
      MSG_WM_CLOSE(OnClose)
      MSG_WM_DESTROY(OnDestroy)
    BEGIN_COMMAND_MAP()
    COMMAND_ID_HANDLER_EX(IDM_APP_ABOUT, OnMenuAbout)
    END_COMMAND_MAP()
    END_MSG_MAP()
};