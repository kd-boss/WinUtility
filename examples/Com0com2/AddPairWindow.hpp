#pragma once
#include "DarkTheme.hpp"
#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include "resource.h"
#include "Com0ComDriver.hpp"

#include <format>
#include <memory>
#include <string>
#include <vector>

struct PairWindowResult
{
    bool accepted = false;
    bool isEdit = false;
    int pairIndex = -1;
    PairParams params;
};

// ─── AddPairWindow ────────────────────────────────────────────────────────────
class AddPairWindow
    : public BaseWindow<AddPairWindow, Window,
                        WinTraits<WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
                                  WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE>>
{
public:
    DECLARE_WND_CLASS(TEXT("Com0ComAddPair"))
    void SetupForNew()
    {
        m_isEdit = false;
        m_pair = {};
    }
    void SetupForEdit(const Com0ComPair &p)
    {
        m_isEdit = true;
        m_pair = p;
    }

    BEGIN_MSG_MAP()
        MSG_WM_CREATE(OnCreate)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_SIZE(OnSize)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_ERASEBKGND(OnEraseBkgnd)
        MSG_WM_CTLCOLORSTATIC(OnCtlColorStatic)
        MSG_WM_CTLCOLOREDIT(OnCtlColorEdit)
        MSG_WM_CTLCOLORBTN(OnCtlColorBtn)
        MSG_WM_DRAWITEM(OnDrawItem)
        BEGIN_COMMAND_MAP()
            COMMAND_ID_HANDLER_EX(IDC_BTN_OK, OnOK)
            COMMAND_ID_HANDLER_EX(IDC_BTN_CANCEL, OnCancel)
        END_COMMAND_MAP()
    END_MSG_MAP()

private:
    int OnCreate(LPCREATESTRUCT pcs);
    void OnDestroy();
    void OnSize(UINT nType, Size sz);
    void OnClose();
    BOOL OnEraseBkgnd(HDC hdc);
    HBRUSH OnCtlColorStatic(HDC hdc, HWND hWnd);
    HBRUSH OnCtlColorEdit(HDC hdc, HWND hWnd);
    HBRUSH OnCtlColorBtn(HDC hdc, HWND hWnd);
    void OnDrawItem(UINT nIDCtl, LPDRAWITEMSTRUCT dis);
    void OnOK(UINT, int, Window);
    void OnCancel(UINT, int, Window);
    void PostResult(bool accepted);
    void PositionControls();

    bool m_isEdit = false;
    Com0ComPair m_pair;

    StaticControl m_lblPortA;
    EditControl m_editPortA;
    StaticControl m_lblPortB;
    EditControl m_editPortB;
    AutoCheckBoxControl m_chkEmuBrA;
    AutoCheckBoxControl m_chkEmuBrB;
    StaticControl m_lblHint;
    PushButtonControl m_btnOK;
    PushButtonControl m_btnCancel;
    DarkTheme m_theme;
};
