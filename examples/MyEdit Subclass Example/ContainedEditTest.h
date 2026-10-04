#ifndef CONTAINED_EDIT_TEST_H
#define CONTAINED_EDIT_TEST_H

#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include <format>

class MyEdit : public ContainedWindowT<EditControl>
{
    public:
    
    MyEdit() : ContainedWindowT<EditControl>(nullptr) {}

    BEGIN_MSG_MAP()
    MSG_WM_CHAR(OnChar)
    END_MSG_MAP()

    void OnChar([[maybe_unused]] TCHAR nChar,[[maybe_unused]] SHORT nRepCnt,[[maybe_unused]]  SHORT nFlags)
    {
        const auto WindowText = std::format(L"User Pressed {:c}", nChar);
        m_owner.SetWindowText(WindowText.c_str());
        SetHandled(false);
    }
};

class ContainedEditTestWindow : public BaseWindow<ContainedEditTestWindow, Window, FrameWinTraits>
{
public:
    DECLARE_WND_CLASS(TEXT("ContainedEditTestWindow"))
    
    int OnCreate(LPCREATESTRUCT lpCreateStruct);
    void OnSize(UINT nType, Size size);
    void OnDestroy();

    BEGIN_MSG_MAP()
        MSG_WM_CREATE(OnCreate)
        MSG_WM_SIZE(OnSize)
        MSG_WM_DESTROY(OnDestroy)
    END_MSG_MAP()

private:
    EditControl m_edit;
    MyEdit m_super;
};

#endif
