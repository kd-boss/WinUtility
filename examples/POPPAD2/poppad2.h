
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00
#ifndef _POPPAD_H
#define _POPPAD_H
#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include <utility>
#include "Resource.h"
#include "About.h"

#define EDITED 1
#define UNTITLED TEXT("(untitled)")


enum class PopPadControls : UINT
{
    EDIT = WM_USER + 1
};

class PopPad2 : public BaseWindow<PopPad2, Window, FrameWinTraits>
{
    MyAboutDialog about;
    EditControl m_edit;
    std::tstring m_appname;
    std::tstring m_buffer;
    std::tstring m_findText;
    std::tstring m_replaceText;
    std::tstring m_filename;

    FINDREPLACE fr = {0};
    int iSearchText = 0;
    bool bNeedSave = false;
    UINT WM_FIND;  
    Window m_findreplaceDialog;
    LOGFONT m_logFont;
    HFONT hFont;
    std::vector<byte> buffer;
    std::wstring lbuffer;
    std::tstring caption;
//PopFind.cpp
    void OnSearchFind(UINT uNotifyCode, int nID, Window wndCtl);
    void OnSearchReplace(UINT uNotifyCode, int nID, Window wndCtl);
    LPARAM OnFind(WPARAM wParam, LPARAM lParam);
    HWND PopFindDlg();
    HWND PopFindReplaceDlg();
    BOOL PopFindSearchText(const int &iSearchOffset, LPFINDREPLACE pfr);
    BOOL PopFindNextText();
    BOOL PopFindReplaceText(const int &iSearchOffset, LPFINDREPLACE pfr);
    BOOL PopFindValidFind();
    
//poppad.cpp    
    void OnClose();
    void OnDestroy();
    void OnExit(UINT uNotifyCode, int nID, Window wndCtl);
    void OnSize(UINT nFlags, Size sz);
    void OnSetFocus(Window wndOld);
    void OnInitMenuPopup(HMENU menuPopup, UINT nIndex, BOOL bSysMenu);    
    void DoCaption(LPCTSTR Caption);
    int OnCreate(LPCREATESTRUCT lpCreateStruct);
    void OnAbout(UINT uNotifyCode, int nID, Window wndCtl);
    void OnEdit(UINT uNotifyCode, int nID, Window wndCtl);
    void OnCopy(UINT uNotifyCode, int nID, Window wndCtl);
    void OnCut(UINT uNotifyCode, int nID, Window wndCtl);
    void OnPaste(UINT uNotifyCode, int nID, Window wndCtl);
    void OnClear(UINT uNotifyCode, int nID, Window wndCtl);
    void OnUnDo(UINT uNotifyCode, int nID, Window wndCtl);
    void OnSelectAll(UINT uNotifyCode, int nID, Window wndCtl);

//PopFile.cpp
    void OnFileOpen(UINT uNotifyCode, int nID, Window wndCtl);
    void OnFileNew(UINT uNotifyCode, int nID, Window wndCtl);
    void OnFileSave(UINT uNotifyCode, int nID, Window wndCtl);
    void OnFileSaveAs(UINT uNotifyCode, int nID, Window wndCtl);
    void PopFileInitialize();
    BOOL PopFileOpenDlg(std::tstring &fileName, const std::tstring &dialogTitle);
    BOOL PopFileSaveDlg(std::tstring &fileName, const std::tstring &dialogTitle);
    BOOL PopFileRead(const std::tstring &fileName);
    BOOL PopFileWrite(const std::tstring &fileName);
    
    
//PopFont.cpp
    BOOL PopFontChooseFont();
    void PopFontInitialize();
    void PopFontSetFont();
    void PopFontDeinitialize();
    void OnFont(UINT uNotifyCode, int nID, Window wndCtl);

public:
    DECLARE_WND_CLASS_1(IDC_POPPAD2, IDI_POPPAD2)

    HRESULT Initialize();

    BEGIN_MSG_MAP()
    BEGIN_COMMAND_MAP()
        COMMAND_ID_HANDLER_EX(IDM_EDIT_CUT, OnCut)
        COMMAND_ID_HANDLER_EX(IDM_EDIT_COPY, OnCopy)
        COMMAND_ID_HANDLER_EX(IDM_EDIT_PASTE, OnPaste)
        COMMAND_ID_HANDLER_EX(IDM_ABOUT, OnAbout)
        COMMAND_ID_HANDLER_EX(IDM_EDIT_CLEAR, OnClear)
        COMMAND_ID_HANDLER_EX(IDM_EDIT_UNDO, OnUnDo)
        COMMAND_ID_HANDLER_EX(IDM_EDIT_SELECT_ALL, OnSelectAll)
        COMMAND_ID_HANDLER_EX(IDOK, OnAbout)
        COMMAND_ID_HANDLER_EX(IDM_EXIT, OnExit)
        COMMAND_ID_HANDLER_EX(std::to_underlying(PopPadControls::EDIT), OnEdit)
        COMMAND_ID_HANDLER_EX(IDM_FILE_OPEN, OnFileOpen)
        COMMAND_ID_HANDLER_EX(IDM_FILE_NEW, OnFileNew)
        COMMAND_ID_HANDLER_EX(IDM_FILE_SAVE, OnFileSave)
        COMMAND_ID_HANDLER_EX(IDM_FILE_SAVE_AS, OnFileSaveAs)
        COMMAND_ID_HANDLER_EX(IDM_SEARCH_FIND, OnSearchFind)
        COMMAND_ID_HANDLER_EX(IDM_SEARCH_REPLACE, OnSearchReplace)
        COMMAND_ID_HANDLER_EX(IDM_FORMAT_FONT, OnFont)
    END_COMMAND_MAP()
    MSG_WM_INITMENUPOPUP(OnInitMenuPopup)
    MSG_WM_SETFOCUS(OnSetFocus)
    MSG_WM_SIZE(OnSize)
    MSG_WM_CREATE(OnCreate)
    MSG_WM_CLOSE(OnClose)
    MSG_WM_DESTROY(OnDestroy)
    BEGIN_DEFAULT_MAP()
        MSG_HANDLER_EX(WM_FIND,OnFind)
    END_DEFAULT_MAP()
    END_MSG_MAP()
    
};
#endif