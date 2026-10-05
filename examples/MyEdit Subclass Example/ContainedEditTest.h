#ifndef CONTAINED_EDIT_TEST_H
#define CONTAINED_EDIT_TEST_H

#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include <WinUtility/Numbers.h>
#include <format>

class MyEdit : public ContainedWindowT<EditControl>
{
    Window m_owner;
    public:
    void SetOwner(const Window& owner)
    {
        m_owner = owner;
    }

    MyEdit() : ContainedWindowT<EditControl>() {}

    BEGIN_MSG_MAP()
    MSG_WM_CHAR(OnChar)
    MSG_WM_KEYDOWN(OnKeyDown)
    MSG_WM_SYSKEYDOWN(OnSysKeyDown)
    END_MSG_MAP()

    void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
    {
        std::wstring WindowText;
        switch(nChar)
        {
            case VK_CONTROL:
                WindowText = std::format(TEXT("User Pressed {}"),TEXT("Control"));
            break;
            case VK_RCONTROL:
                WindowText = std::format(TEXT("User Pressed {}"),TEXT("Right Control"));
            break;
            case VK_LCONTROL:
                WindowText = std::format(TEXT("User Pressed {}"),TEXT("Left Control"));
            break;
            case VK_DELETE:            
                WindowText = std::format(TEXT("User Pressed {}"),TEXT("Delete"));
            break;
            case VK_LEFT:
                WindowText = std::format(TEXT("User Pressed {}"),TEXT("Left Arrow"));
            break;
            case VK_RIGHT:
                WindowText = std::format(TEXT("User Pressed {}"),TEXT("Right Arrow"));
            break;    
            case VK_UP:
                WindowText = std::format(TEXT("User Pressed {}"),TEXT("Up Arrow"));
            break;
            case VK_DOWN:
                WindowText = std::format(TEXT("User Pressed {}"),TEXT("Down Arrow"));
            break;
            case VK_HOME:
                WindowText = std::format(TEXT("User Pressed {}"),TEXT("Home"));
            break;
            case VK_END:
                WindowText = std::format(TEXT("User Pressed {}"),TEXT("End"));
            break; 
            case VK_PRIOR:
                WindowText = std::format(TEXT("User Pressed {}"),TEXT("Page Up"));
            break;
            case VK_NEXT:
                WindowText = std::format(TEXT("User Pressed {}"),TEXT("Page Down"));
            break;      
            case VK_ESCAPE:
                WindowText = std::format(TEXT("User Pressed {}"),TEXT("Escape"));
            break;  
            case VK_F1:
                WindowText = std::format(TEXT("User Pressed {}"),TEXT("F1"));
            break;
            case VK_F2:
                WindowText = std::format(TEXT("User Pressed {}"),TEXT("F2"));
            break;
            case VK_F3:
                WindowText = std::format(TEXT("User Pressed {}"),TEXT("F3"));
            break;
            case VK_F4:
                WindowText = std::format(TEXT("User Pressed {}"),TEXT("F4"));
            break;
            case VK_F5:
                WindowText = std::format(TEXT("User Pressed {}"),TEXT("F5"));
            break;
            case VK_F6:
                WindowText = std::format(TEXT("User Pressed {}"),TEXT("F6"));
            break;
            case VK_F7:
                WindowText = std::format(TEXT("User Pressed {}"),TEXT("F7"));
            break;
            case VK_F8:
                WindowText = std::format(TEXT("User Pressed {}"),TEXT("F8"));
            break;
            case VK_F9:
                WindowText = std::format(TEXT("User Pressed {}"),TEXT("F9"));
            break;
            case VK_F10:
                WindowText = std::format(TEXT("User Pressed {}"),TEXT("F10"));
            break;
            case VK_F11:
                WindowText = std::format(TEXT("User Pressed {}"),TEXT("F11")); 
            break;
            case VK_F12:
                WindowText = std::format(TEXT("User Pressed {}"),TEXT("F12"));
            break;
            case VK_F13:
                WindowText = std::format(TEXT("User Pressed {}"),TEXT("F13"));
            break;
            case VK_F14:
                WindowText = std::format(TEXT("User Pressed {}"),TEXT("F14"));
            break;
            case WM_SYSKEYDOWN:
                {
                    std::wstring buff;
	                buff.resize(35);
	                auto lParam = MAKELPARAM( nRepCnt, nFlags);	
	                GetKeyNameText(lParam, buff.data(), convert_to<int>(buff.length()));
                    WindowText = std::format(TEXT("User Pressed {}"), buff.c_str());
                }
            break;
        }
        m_owner.SetWindowText(WindowText.c_str());
        SetHandled(false);
    }

    void OnChar([[maybe_unused]] TCHAR nChar,[[maybe_unused]] SHORT nRepCnt,[[maybe_unused]]  SHORT nFlags)
    {
        std::wstring WindowText;
        switch(nChar)
        {
            case 0x0008:
            WindowText = std::format(TEXT("User Pressed {}"),TEXT("Backspace"));
            break;
            case 0x0009:
            WindowText = std::format(TEXT("User Pressed {}"),TEXT("Tab"));
            break;
            case 0x000D:
            WindowText = std::format(TEXT("User Pressed {}"),TEXT("Return"));
            break;
            default:
            WindowText = std::format(TEXT("User Pressed {}"),std::format(TEXT("{}"), nChar));
            break;
        }   
        
        m_owner.SetWindowText(WindowText.c_str());
        SetHandled(false);
    }

    void OnSysKeyDown([[maybe_unused]] UINT nChar, [[maybe_unused]] SHORT nRepCnt, [[maybe_unused]] SHORT nFlags)
    {
        std::wstring WindowText;
        std::wstring buff;
	    buff.resize(35);
	    auto lParam = MAKELPARAM( nRepCnt, nFlags);	
	    GetKeyNameText(lParam, buff.data(), convert_to<int>(buff.length()));
        WindowText = std::format(TEXT("User Pressed {}"), buff.c_str());
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
