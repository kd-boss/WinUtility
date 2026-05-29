// =============================================================================
//  D2DEditControl.h  (rev 3 – unified naming, fixed duplicates, crash-safe)
//
//  A full-featured custom edit control rendered with Direct2D / GDI DC
//  interop and DirectWrite, built on the BaseWindow.h CRTP framework.
//  Naming and structure are consistent with CustomScrollBar.h and
//  DialControl.h – all three controls follow the same conventions.
//
//  Crash-safety / factory sharing – see CustomScrollBar.h header comment.
//  Pass a shared factory via SetSharedFactory() before Create().
//  Call CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED) in wWinMain
//  before creating anything, and CoUninitialize() after the message loop.
//
//  Changes from rev 2:
//    [1] All member variables renamed to full descriptive names (m_bMultiLine
//        vs m_bML, m_pRenderTarget vs m_pRT, etc.)
//    [2] All message handlers renamed from MH_Xxx to OnXxx.
//    [3] Internal helpers renamed to full English names.
//    [4] Duplicate message map entries removed:
//          EM_LIMITTEXT == EM_SETLIMITTEXT (same value 0x00C5) – one entry.
//    [5] ReleaseResources() is now public and is called from OnDestroy.
//    [6] SetSharedFactory() added; factory not released if shared.
//    [7] WM_PAINT guards against null render target without lazy-init.
//
//  Public API:
//    SetFont(face, pt) / SetFont(HFONT)
//    SetBackColor(COLORREF/D2D1_COLOR_F) / ResetBackColor()
//    SetTextColor(COLORREF/D2D1_COLOR_F) / ResetTextColor()
//    SetHorzAlign(HorzAlign) / SetVertAlign(VertAlign)
//    SetPlaceholderText(LPCWSTR)
//    SetSharedFactory(ID2D1Factory*)
//    ReleaseResources()   [public]
//    GetSel / SetSel / ReplaceSel
//    GetTextLen() / IsModified() / SetModified() / LimitText()
//    CanUndo() / EmptyUndo()
//    Full WM_SETTEXT / WM_GETTEXT / EM_* / EN_* surface
//
//  Dependencies: BaseWindow.h (first), d2d1.lib, dwrite.lib
// =============================================================================
#pragma once
#ifndef __D2D_EDIT_CONTROL_H__
#define __D2D_EDIT_CONTROL_H__

#ifndef __BASEWINDOW_H__
#error "D2DEditControl.h requires BaseWindow.h to be included first."
#endif

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <algorithm>
#include <cwctype>
#include <string>
#include <vector>

#ifdef _MSC_VER
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#endif

static constexpr float EDIT_BORDER_W = 1.0f;
static constexpr float EDIT_FOCUS_BAR_H = 2.0f;
static constexpr float EDIT_CORNER_R = 4.0f;
static constexpr float EDIT_PAD_H = 6.0f;
static constexpr float EDIT_PAD_V = 3.0f;
static constexpr float EDIT_SCROLL_W = 7.0f;
static constexpr float EDIT_MAX_DIM = 65536.0f;

struct EditColorScheme
{
    D2D1_COLOR_F background, backgroundReadOnly, backgroundDisabled;
    D2D1_COLOR_F text, textDisabled, placeholder;
    D2D1_COLOR_F selection;
    D2D1_COLOR_F border, borderHovered, borderFocused;
    D2D1_COLOR_F scrollThumb;
};

inline EditColorScheme EditDarkScheme() noexcept
{
    return {
        D2D1::ColorF(0.118f, 0.118f, 0.118f),
        D2D1::ColorF(0.098f, 0.098f, 0.098f),
        D2D1::ColorF(0.082f, 0.082f, 0.082f),
        D2D1::ColorF(0.961f, 0.961f, 0.961f),
        D2D1::ColorF(0.376f, 0.376f, 0.376f),
        D2D1::ColorF(0.427f, 0.427f, 0.427f),
        D2D1::ColorF(0.000f, 0.471f, 0.831f, 0.38f),
        D2D1::ColorF(0.243f, 0.243f, 0.243f),
        D2D1::ColorF(0.376f, 0.376f, 0.376f),
        D2D1::ColorF(0.000f, 0.471f, 0.831f),
        D2D1::ColorF(0.450f, 0.450f, 0.450f),
    };
}
inline EditColorScheme EditLightScheme() noexcept
{
    return {
        D2D1::ColorF(1.000f, 1.000f, 1.000f),
        D2D1::ColorF(0.961f, 0.961f, 0.961f),
        D2D1::ColorF(0.961f, 0.961f, 0.961f),
        D2D1::ColorF(0.067f, 0.067f, 0.067f),
        D2D1::ColorF(0.596f, 0.596f, 0.596f),
        D2D1::ColorF(0.596f, 0.596f, 0.596f),
        D2D1::ColorF(0.000f, 0.471f, 0.831f, 0.25f),
        D2D1::ColorF(0.737f, 0.737f, 0.737f),
        D2D1::ColorF(0.514f, 0.514f, 0.514f),
        D2D1::ColorF(0.000f, 0.471f, 0.831f),
        D2D1::ColorF(0.600f, 0.600f, 0.600f),
    };
}

enum : UINT
{
    EDITCMD_UNDO = 0xEB00,
    EDITCMD_CUT,
    EDITCMD_COPY,
    EDITCMD_PASTE,
    EDITCMD_DELETE,
    EDITCMD_SELECTALL
};

// =============================================================================
class D2DEditControl
    : public BaseWindow<D2DEditControl, Window,
                        WinTraits<WS_CHILD | WS_VISIBLE | WS_TABSTOP>>
{
public:
    DECLARE_WND_CLASS(TEXT("D2DEditControl"))

    enum class HorzAlign
    {
        Left = 0,
        Center,
        Right
    };
    enum class VertAlign
    {
        Top = 0,
        Center,
        Bottom
    };

    D2DEditControl()
        : m_nCaretPos(0), m_nSelAnchor(0), m_nMaxLength(0x7FFFFFFE), m_fScrollX(0.f), m_fScrollY(0.f), m_bMultiLine(false), m_bPassword(false), m_bReadOnly(false), m_bAutoHScroll(true), m_bAutoVScroll(false), m_bNoHideSel(false), m_bWantReturn(false), m_bUpperCase(false), m_bLowerCase(false), m_bNumbersOnly(false), m_bSystemHScroll(false), m_bSystemVScroll(false), m_horzAlign(HorzAlign::Left), m_vertAlign(VertAlign::Center), m_bCustomBackColor(false), m_bCustomTextColor(false), m_clrCustomBack(D2D1::ColorF(D2D1::ColorF::White)), m_clrCustomText(D2D1::ColorF(D2D1::ColorF::Black)), m_bDarkMode(false), m_bFocused(false), m_bHovered(false), m_bMouseDown(false), m_bModified(false), m_bLayoutDirty(true), m_bVThumbDrag(false), m_bHThumbDrag(false), m_fThumbDragStartPx(0.f), m_fThumbDragStartScroll(0.f), m_bUndoValid(false), m_bOwnsFactory(true), m_pD2DFactory(nullptr), m_pRenderTarget(nullptr), m_pDWriteFactory(nullptr), m_pTextFormat(nullptr), m_pTextLayout(nullptr), m_fFontPointSize(10.f), m_nFontWeight(DWRITE_FONT_WEIGHT_NORMAL), m_nFontStyle(DWRITE_FONT_STYLE_NORMAL), m_pBrushBackground(nullptr), m_pBrushText(nullptr), m_pBrushSelection(nullptr), m_pBrushBorder(nullptr), m_pBrushBorderHovered(nullptr), m_pBrushBorderFocused(nullptr), m_pBrushScrollThumb(nullptr)
    {
        m_sFontFace = L"Segoe UI";
    }

    ~D2DEditControl() { ReleaseResources(); }

    // ── Font ──────────────────────────────────────────────────────────────────
    void SetFont(LPCWSTR pszFace, float fPointSize,
                 DWRITE_FONT_WEIGHT nWeight = DWRITE_FONT_WEIGHT_NORMAL,
                 DWRITE_FONT_STYLE nStyle = DWRITE_FONT_STYLE_NORMAL)
    {
        m_sFontFace = pszFace ? pszFace : L"Segoe UI";
        m_fFontPointSize = fPointSize > 0.f ? fPointSize : 10.f;
        m_nFontWeight = nWeight;
        m_nFontStyle = nStyle;
        SafeRelease(m_pTextFormat);
        if (m_pDWriteFactory)
            CreateTextFormat();
        m_bLayoutDirty = true;
        if (m_hwnd)
        {
            EnsureCaretVisible();
            Invalidate();
        }
    }

    void SetFont(HFONT hFont, BOOL bRedraw = TRUE)
    {
        if (!hFont)
            return;
        LOGFONTW lf = {};
        ::GetObjectW(hFont, sizeof(lf), &lf);
        m_sFontFace = lf.lfFaceName;
        m_nFontWeight = static_cast<DWRITE_FONT_WEIGHT>(lf.lfWeight);
        m_nFontStyle = lf.lfItalic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL;
        if (lf.lfHeight < 0)
        {
            HDC hdc = m_hwnd ? ::GetDC(m_hwnd) : ::GetDC(nullptr);
            m_fFontPointSize = static_cast<float>(-lf.lfHeight) * 72.f /
                               static_cast<float>(::GetDeviceCaps(hdc, LOGPIXELSY));
            ::ReleaseDC(m_hwnd ? m_hwnd : nullptr, hdc);
        }
        else
            m_fFontPointSize = 10.f;
        SafeRelease(m_pTextFormat);
        if (m_pDWriteFactory)
            CreateTextFormat();
        m_bLayoutDirty = true;
        if (bRedraw && m_hwnd)
        {
            EnsureCaretVisible();
            Invalidate();
        }
    }

    // ── Colour overrides ──────────────────────────────────────────────────────
    void SetBackColor(COLORREF cr)
    {
        m_clrCustomBack = CrToD2D(cr);
        m_bCustomBackColor = true;
        RebuildBrushes();
        Invalidate();
    }
    void SetBackColor(D2D1_COLOR_F c)
    {
        m_clrCustomBack = c;
        m_bCustomBackColor = true;
        RebuildBrushes();
        Invalidate();
    }
    void ResetBackColor()
    {
        m_bCustomBackColor = false;
        RebuildBrushes();
        Invalidate();
    }
    void SetTextColor(COLORREF cr)
    {
        m_clrCustomText = CrToD2D(cr);
        m_bCustomTextColor = true;
        RebuildBrushes();
        Invalidate();
    }
    void SetTextColor(D2D1_COLOR_F c)
    {
        m_clrCustomText = c;
        m_bCustomTextColor = true;
        RebuildBrushes();
        Invalidate();
    }
    void ResetTextColor()
    {
        m_bCustomTextColor = false;
        RebuildBrushes();
        Invalidate();
    }

    // ── Alignment ─────────────────────────────────────────────────────────────
    void SetHorzAlign(HorzAlign ha)
    {
        m_horzAlign = ha;
        if (m_pTextFormat)
        {
            DWRITE_TEXT_ALIGNMENT da = DWRITE_TEXT_ALIGNMENT_LEADING;
            if (ha == HorzAlign::Center)
                da = DWRITE_TEXT_ALIGNMENT_CENTER;
            else if (ha == HorzAlign::Right)
                da = DWRITE_TEXT_ALIGNMENT_TRAILING;
            m_pTextFormat->SetTextAlignment(da);
        }
        m_bLayoutDirty = true;
        Invalidate();
    }
    void SetVertAlign(VertAlign va)
    {
        m_vertAlign = va;
        Invalidate();
    }
    HorzAlign GetHorzAlign() const { return m_horzAlign; }
    VertAlign GetVertAlign() const { return m_vertAlign; }

    void SetPlaceholderText(LPCWSTR psz)
    {
        m_sPlaceholder = psz ? psz : L"";
        Invalidate();
    }

    // ── Selection / text ──────────────────────────────────────────────────────
    void GetSel(int &nStart, int &nEnd) const
    {
        nStart = SelectionStart();
        nEnd = SelectionEnd();
    }
    void SetSel(int nStart, int nEnd, BOOL bNoScroll = FALSE)
    {
        int n = (int)m_sText.length();
        if (nStart < 0)
        {
            m_nSelAnchor = m_nCaretPos = n;
        }
        else
        {
            m_nSelAnchor = Clamp(nStart, 0, n);
            m_nCaretPos = nEnd < 0 ? n : Clamp(nEnd, 0, n);
        }
        if (!bNoScroll)
            EnsureCaretVisible();
        Invalidate();
        UpdateCaretPosition();
    }
    void ReplaceSel(LPCWSTR pszText, BOOL bCanUndo = TRUE)
    {
        if (bCanUndo)
            SaveUndo();
        DeleteSelection();
        if (pszText && *pszText)
            InsertRaw(m_nCaretPos, pszText);
        NotifyParent(EN_CHANGE);
        m_bLayoutDirty = true;
        EnsureCaretVisible();
        Invalidate();
        UpdateCaretPosition();
    }

    int GetTextLen() const { return (int)m_sText.length(); }
    bool IsModified() const { return m_bModified; }
    void SetModified(bool b) { m_bModified = b; }
    void LimitText(int n) { m_nMaxLength = n > 0 ? n : 0x7FFFFFFE; }
    bool CanUndo() const { return m_bUndoValid; }
    void EmptyUndo() { m_bUndoValid = false; }

    // ── Shared factory ────────────────────────────────────────────────────────
    void SetSharedFactory(ID2D1Factory *pFactory)
    {
        if (m_bOwnsFactory)
            SafeRelease(m_pD2DFactory);
        m_pD2DFactory = pFactory;
        m_bOwnsFactory = false;
    }

    // ── Public resource release ───────────────────────────────────────────────
    void ReleaseResources()
    {
        ::HideCaret(m_hwnd);
        ::DestroyCaret();
        ReleaseBrushes();
        SafeRelease(m_pTextLayout);
        SafeRelease(m_pTextFormat);
        SafeRelease(m_pRenderTarget);
        SafeRelease(m_pDWriteFactory);
        if (m_bOwnsFactory)
            SafeRelease(m_pD2DFactory);
        else
            m_pD2DFactory = nullptr;
    }

    // ── Message map ───────────────────────────────────────────────────────────
    BEGIN_MSG_MAP()
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    MESSAGE_HANDLER(WM_PAINT, OnPaint)
    MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
    MESSAGE_HANDLER(WM_SIZE, OnSize)
    MESSAGE_HANDLER(WM_ENABLE, OnEnable)
    MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
    MESSAGE_HANDLER(WM_MOUSEACTIVATE, OnMouseActivate)
    MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
    MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
    MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
    MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
    MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
    MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
    MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
    MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
    MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
    MESSAGE_HANDLER(WM_CHAR, OnChar)
    MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
    MESSAGE_HANDLER(WM_SETFONT, OnWmSetFont)
    MESSAGE_HANDLER(WM_GETFONT, OnWmGetFont)
    MESSAGE_HANDLER(WM_SETTEXT, OnSetText)
    MESSAGE_HANDLER(WM_GETTEXT, OnGetText)
    MESSAGE_HANDLER(WM_GETTEXTLENGTH, OnGetTextLength)
    MESSAGE_HANDLER(WM_CUT, OnCut)
    MESSAGE_HANDLER(WM_COPY, OnCopy)
    MESSAGE_HANDLER(WM_PASTE, OnPaste)
    MESSAGE_HANDLER(WM_CLEAR, OnClear)
    MESSAGE_HANDLER(WM_UNDO, OnUndo)
    MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
    MESSAGE_HANDLER(WM_VSCROLL, OnVScroll)
    MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
    MESSAGE_HANDLER(WM_THEMECHANGED, OnThemeChanged)
    MESSAGE_HANDLER(WM_SETTINGCHANGE, OnSettingChange)
    MESSAGE_HANDLER(EM_GETSEL, OnEmGetSel)
    MESSAGE_HANDLER(EM_SETSEL, OnEmSetSel)
    MESSAGE_HANDLER(EM_REPLACESEL, OnEmReplaceSel)
    MESSAGE_HANDLER(EM_GETLINE, OnEmGetLine)
    MESSAGE_HANDLER(EM_GETLINECOUNT, OnEmGetLineCount)
    MESSAGE_HANDLER(EM_LINEFROMCHAR, OnEmLineFromChar)
    MESSAGE_HANDLER(EM_LINEINDEX, OnEmLineIndex)
    MESSAGE_HANDLER(EM_LINELENGTH, OnEmLineLength)
    MESSAGE_HANDLER(EM_SCROLL, OnEmScroll)
    MESSAGE_HANDLER(EM_SCROLLCARET, OnEmScrollCaret)
    MESSAGE_HANDLER(EM_SETREADONLY, OnEmSetReadOnly)
    MESSAGE_HANDLER(EM_LIMITTEXT, OnEmLimitText) // == EM_SETLIMITTEXT (0x00C5)
    MESSAGE_HANDLER(EM_GETMODIFY, OnEmGetModify)
    MESSAGE_HANDLER(EM_SETMODIFY, OnEmSetModify)
    MESSAGE_HANDLER(EM_CANUNDO, OnEmCanUndo)
    MESSAGE_HANDLER(EM_UNDO, OnUndo)
    MESSAGE_HANDLER(EM_EMPTYUNDOBUFFER, OnEmEmptyUndoBuffer)
    MESSAGE_HANDLER(EM_GETFIRSTVISIBLELINE, OnEmGetFirstVisibleLine)
    END_MSG_MAP()

private:
    std::wstring m_sText, m_sPlaceholder;
    int m_nCaretPos, m_nSelAnchor, m_nMaxLength;
    float m_fScrollX, m_fScrollY;

    bool m_bMultiLine, m_bPassword, m_bReadOnly;
    bool m_bAutoHScroll, m_bAutoVScroll;
    bool m_bNoHideSel, m_bWantReturn;
    bool m_bUpperCase, m_bLowerCase, m_bNumbersOnly;
    bool m_bSystemHScroll, m_bSystemVScroll;

    HorzAlign m_horzAlign;
    VertAlign m_vertAlign;
    bool m_bCustomBackColor, m_bCustomTextColor;
    D2D1_COLOR_F m_clrCustomBack, m_clrCustomText;

    bool m_bDarkMode, m_bFocused, m_bHovered, m_bMouseDown, m_bModified, m_bLayoutDirty;
    bool m_bVThumbDrag, m_bHThumbDrag;
    float m_fThumbDragStartPx, m_fThumbDragStartScroll;

    struct UndoRecord
    {
        std::wstring sText;
        int nCaret, nAnchor;
    };
    UndoRecord m_undoRecord;
    bool m_bUndoValid;

    bool m_bOwnsFactory;
    ID2D1Factory *m_pD2DFactory;
    ID2D1DCRenderTarget *m_pRenderTarget;
    IDWriteFactory *m_pDWriteFactory;
    IDWriteTextFormat *m_pTextFormat;
    IDWriteTextLayout *m_pTextLayout;

    std::wstring m_sFontFace;
    float m_fFontPointSize;
    DWRITE_FONT_WEIGHT m_nFontWeight;
    DWRITE_FONT_STYLE m_nFontStyle;

    ID2D1SolidColorBrush *m_pBrushBackground, *m_pBrushText, *m_pBrushSelection;
    ID2D1SolidColorBrush *m_pBrushBorder, *m_pBrushBorderHovered;
    ID2D1SolidColorBrush *m_pBrushBorderFocused, *m_pBrushScrollThumb;

    // ── Static helpers ────────────────────────────────────────────────────────
    static int Clamp(int v, int lo, int hi) noexcept { return v < lo ? lo : v > hi ? hi
                                                                                   : v; }
    static float Clampf(float v, float lo, float hi) noexcept { return v < lo ? lo : v > hi ? hi
                                                                                            : v; }
    static D2D1_COLOR_F CrToD2D(COLORREF c) noexcept
    {
        return D2D1::ColorF(GetRValue(c) / 255.f, GetGValue(c) / 255.f, GetBValue(c) / 255.f);
    }
    template <class T>
    static void SafeRelease(T *&p) noexcept
    {
        if (p)
        {
            p->Release();
            p = nullptr;
        }
    }
    static bool DetectDarkMode() noexcept
    {
        DWORD v = 1, sz = sizeof(v);
        ::RegGetValueW(HKEY_CURRENT_USER,
                       L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                       L"AppsUseLightTheme", RRF_RT_REG_DWORD, nullptr, &v, &sz);
        return v == 0;
    }

    
    void NotifyParent(int code) const
    {
        HWND hP = ::GetParent(m_hwnd);
        if (hP)
            ::SendMessage(hP, WM_COMMAND,
                          MAKEWPARAM(::GetDlgCtrlID(m_hwnd), (WORD)code), (LPARAM)m_hwnd);
    }
    int SelectionStart() const { return std::min(m_nCaretPos, m_nSelAnchor); }
    int SelectionEnd() const { return std::max(m_nCaretPos, m_nSelAnchor); }
    bool HasSelection() const { return m_nCaretPos != m_nSelAnchor; }

    std::wstring GetDisplayText() const
    {
        if (m_bPassword)
            return std::wstring(m_sText.length(), L'\x25CF');
        return m_sText;
    }

    D2D1_RECT_F GetContentRect(float w, float h) const noexcept
    {
        return D2D1::RectF(
            EDIT_BORDER_W + EDIT_PAD_H, EDIT_BORDER_W + EDIT_PAD_V,
            w - EDIT_BORDER_W - EDIT_PAD_H, h - EDIT_BORDER_W - EDIT_PAD_V);
    }

    D2D1_POINT_2F GetLayoutOrigin(const D2D1_RECT_F &cr) const
    {
        float ox = cr.left - m_fScrollX, oy = cr.top;
        if (!m_bMultiLine && m_pTextLayout)
        {
            DWRITE_TEXT_METRICS tm = {};
            m_pTextLayout->GetMetrics(&tm);
            float ch = cr.bottom - cr.top;
            float oyMin = cr.top, oyMax = std::max(cr.top, cr.bottom - tm.height);
            switch (m_vertAlign)
            {
            case VertAlign::Top:
                oy = cr.top;
                break;
            case VertAlign::Center:
                oy = cr.top + (ch * 0.5f) - (tm.height * 0.5f);
                break;
            case VertAlign::Bottom:
                oy = cr.bottom - tm.height;
                break;
            }
            oy = Clampf(oy, oyMin, oyMax);
        }
        else
            oy -= m_fScrollY;
        return D2D1::Point2F(ox, oy);
    }

    // ── D2D / DWrite resource management ─────────────────────────────────────
    HRESULT CreateD2DFactory()
    {
        if (m_pD2DFactory)
            return S_OK;
        m_bOwnsFactory = true;
        return ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
    }
    HRESULT CreateDWriteFactory()
    {
        if (m_pDWriteFactory)
            return S_OK;
        return ::DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
                                     __uuidof(IDWriteFactory), (IUnknown **)&m_pDWriteFactory);
    }
    HRESULT CreateRenderTarget()
    {
        if (m_pRenderTarget)
            return S_OK;
        if (!m_pD2DFactory)
            return E_FAIL;
        D2D1_RENDER_TARGET_PROPERTIES p = D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE));
        return m_pD2DFactory->CreateDCRenderTarget(&p, &m_pRenderTarget);
    }
    HRESULT CreateTextFormat()
    {
        if (!m_pDWriteFactory || m_pTextFormat)
            return S_OK;
        float dip = m_fFontPointSize * (96.f / 72.f);
        HRESULT hr = m_pDWriteFactory->CreateTextFormat(m_sFontFace.c_str(), nullptr,
                                                        m_nFontWeight, m_nFontStyle, DWRITE_FONT_STRETCH_NORMAL, dip, L"", &m_pTextFormat);
        if (SUCCEEDED(hr))
        {
            DWRITE_TEXT_ALIGNMENT da = DWRITE_TEXT_ALIGNMENT_LEADING;
            if (m_horzAlign == HorzAlign::Center)
                da = DWRITE_TEXT_ALIGNMENT_CENTER;
            else if (m_horzAlign == HorzAlign::Right)
                da = DWRITE_TEXT_ALIGNMENT_TRAILING;
            m_pTextFormat->SetTextAlignment(da);
            m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
            m_pTextFormat->SetWordWrapping(
                (m_bMultiLine && !m_bAutoHScroll) ? DWRITE_WORD_WRAPPING_WRAP : DWRITE_WORD_WRAPPING_NO_WRAP);
        }
        return hr;
    }
    HRESULT CreateBrushes()
    {
        if (!m_pRenderTarget)
            return E_FAIL;
        EditColorScheme c = m_bDarkMode ? EditDarkScheme() : EditLightScheme();
        bool dis = !::IsWindowEnabled(m_hwnd);
        D2D1_COLOR_F bg = m_bCustomBackColor ? m_clrCustomBack : (m_bReadOnly ? c.backgroundReadOnly : (dis ? c.backgroundDisabled : c.background));
        D2D1_COLOR_F tx = m_bCustomTextColor ? m_clrCustomText : (dis ? c.textDisabled : c.text);
        auto MB = [&](ID2D1SolidColorBrush *&br, const D2D1_COLOR_F &cl) -> HRESULT
        {
            SafeRelease(br); return m_pRenderTarget->CreateSolidColorBrush(cl,&br); };
        HRESULT hr = S_OK;
        if (SUCCEEDED(hr))
            hr = MB(m_pBrushBackground, bg);
        if (SUCCEEDED(hr))
            hr = MB(m_pBrushText, tx);
        if (SUCCEEDED(hr))
            hr = MB(m_pBrushSelection, c.selection);
        if (SUCCEEDED(hr))
            hr = MB(m_pBrushBorder, c.border);
        if (SUCCEEDED(hr))
            hr = MB(m_pBrushBorderHovered, c.borderHovered);
        if (SUCCEEDED(hr))
            hr = MB(m_pBrushBorderFocused, c.borderFocused);
        if (SUCCEEDED(hr))
            hr = MB(m_pBrushScrollThumb, c.scrollThumb);
        return hr;
    }
    void RebuildBrushes()
    {
        if (m_pRenderTarget)
            CreateBrushes();
    }
    void ReleaseBrushes()
    {
        SafeRelease(m_pBrushBackground);
        SafeRelease(m_pBrushText);
        SafeRelease(m_pBrushSelection);
        SafeRelease(m_pBrushBorder);
        SafeRelease(m_pBrushBorderHovered);
        SafeRelease(m_pBrushBorderFocused);
        SafeRelease(m_pBrushScrollThumb);
    }
    void RecoverDeviceLoss()
    {
        ReleaseBrushes();
        SafeRelease(m_pTextLayout);
        SafeRelease(m_pRenderTarget);
        if (SUCCEEDED(CreateRenderTarget()))
            CreateBrushes();
        m_bLayoutDirty = true;
    }

    // ── Layout ────────────────────────────────────────────────────────────────
    void EnsureLayout()
    {
        if (!m_bLayoutDirty && m_pTextLayout)
            return;
        SafeRelease(m_pTextLayout);
        if (!m_pTextFormat || !m_pDWriteFactory)
            return;
        float cw = EDIT_MAX_DIM;
        if (m_hwnd && m_bMultiLine && !m_bAutoHScroll)
        {
            RECT rc = {};
            ::GetClientRect(m_hwnd, &rc);
            float w = (float)(rc.right - rc.left), h = (float)(rc.bottom - rc.top);
            cw = std::max(1.f, GetContentRect(w, h).right - GetContentRect(w, h).left);
        }
        std::wstring disp = GetDisplayText();
        UINT32 n = (UINT32)disp.length();
        m_pDWriteFactory->CreateTextLayout(n ? disp.c_str() : L"", n, m_pTextFormat, cw, EDIT_MAX_DIM, &m_pTextLayout);
        if (m_pTextLayout)
            m_pTextLayout->SetWordWrapping((m_bMultiLine && !m_bAutoHScroll) ? DWRITE_WORD_WRAPPING_WRAP : DWRITE_WORD_WRAPPING_NO_WRAP);
        m_bLayoutDirty = false;
    }

    int LayoutPointToPosition(float lx, float ly) const
    {
        if (!m_pTextLayout)
            return 0;
        BOOL tr = FALSE, in = FALSE;
        DWRITE_HIT_TEST_METRICS h = {};
        m_pTextLayout->HitTestPoint(lx, ly, &tr, &in, &h);
        // Use tr alone: clicking past last char has isInside=FALSE but tr=TRUE
        return Clamp((int)h.textPosition + (tr ? 1 : 0), 0, (int)m_sText.length());
    }

    bool PositionToLayoutPoint(int nPos, float &ox, float &oy, float &oh) const
    {
        if (!m_pTextLayout)
        {
            ox = oy = 0.f;
            oh = 14.f;
            return false;
        }
        DWRITE_HIT_TEST_METRICS h = {};
        m_pTextLayout->HitTestTextPosition((UINT32)Clamp(nPos, 0, (int)m_sText.length()), FALSE, &ox, &oy, &h);
        oh = h.height;
        return true;
    }

    int ClientPointToPosition(POINT pt)
    {
        EnsureLayout();
        RECT rc;
        ::GetClientRect(m_hwnd, &rc);
        float w = (float)(rc.right - rc.left), h = (float)(rc.bottom - rc.top);
        D2D1_RECT_F cr = GetContentRect(w, h);
        D2D1_POINT_2F o = GetLayoutOrigin(cr);
        return LayoutPointToPosition((float)pt.x - o.x, (float)pt.y - o.y);
    }

    void ClampScroll(float cw, float ch)
    {
        if (!m_pTextLayout)
            return;
        DWRITE_TEXT_METRICS tm = {};
        m_pTextLayout->GetMetrics(&tm);
        m_fScrollX = Clampf(m_fScrollX, 0.f, std::max(0.f, tm.widthIncludingTrailingWhitespace - cw));
        m_fScrollY = Clampf(m_fScrollY, 0.f, std::max(0.f, tm.height - ch));
    }

    void EnsureCaretVisible()
    {
        if (!m_hwnd)
            return;
        EnsureLayout();
        if (!m_pTextLayout)
            return;
        RECT rc;
        ::GetClientRect(m_hwnd, &rc);
        float w = (float)(rc.right - rc.left), h = (float)(rc.bottom - rc.top);
        D2D1_RECT_F cr = GetContentRect(w, h);
        float cw = cr.right - cr.left, ch = cr.bottom - cr.top;
        float cx = 0, cy = 0, lh = 0;
        PositionToLayoutPoint(m_nCaretPos, cx, cy, lh);
        if (cx < m_fScrollX)
            m_fScrollX = cx;
        if (cx > m_fScrollX + cw - 2.f)
            m_fScrollX = cx - cw + 2.f;
        if (m_bMultiLine)
        {
            if (cy < m_fScrollY)
                m_fScrollY = cy;
            if (cy + lh > m_fScrollY + ch)
                m_fScrollY = cy + lh - ch;
        }
        ClampScroll(cw, ch);
        UpdateScrollBars();
    }

    void UpdateScrollBars()
    {
        EnsureLayout();
        if (!m_hwnd || !m_pTextLayout)
            return;
        DWRITE_TEXT_METRICS tm = {};
        m_pTextLayout->GetMetrics(&tm);
        RECT rc;
        ::GetClientRect(m_hwnd, &rc);
        float w = (float)(rc.right - rc.left), h = (float)(rc.bottom - rc.top);
        D2D1_RECT_F cr = GetContentRect(w, h);
        if (m_bSystemHScroll)
        {
            SCROLLINFO si = {sizeof(si), SIF_RANGE | SIF_PAGE | SIF_POS};
            si.nMax = (int)tm.widthIncludingTrailingWhitespace;
            si.nPage = (UINT)(cr.right - cr.left);
            si.nPos = (int)m_fScrollX;
            ::SetScrollInfo(m_hwnd, SB_HORZ, &si, TRUE);
        }
        if (m_bSystemVScroll && m_bMultiLine)
        {
            SCROLLINFO si = {sizeof(si), SIF_RANGE | SIF_PAGE | SIF_POS};
            si.nMax = (int)tm.height;
            si.nPage = (UINT)(cr.bottom - cr.top);
            si.nPos = (int)m_fScrollY;
            ::SetScrollInfo(m_hwnd, SB_VERT, &si, TRUE);
        }
    }

    void UpdateCaretPosition()
    {
        if (!m_bFocused || !m_hwnd)
            return;
        EnsureLayout();
        if (!m_pTextLayout)
            return;
        RECT rc;
        ::GetClientRect(m_hwnd, &rc);
        float w = (float)(rc.right - rc.left), h = (float)(rc.bottom - rc.top);
        D2D1_RECT_F cr = GetContentRect(w, h);
        D2D1_POINT_2F o = GetLayoutOrigin(cr);
        float lx = 0, ly = 0, lh = 0;
        PositionToLayoutPoint(m_nCaretPos, lx, ly, lh);
        int ch = (int)lh;
        if (ch < 2)
            ch = 14;
        ::CreateCaret(m_hwnd, nullptr, 2, ch);
        ::SetCaretPos((int)(o.x + lx), (int)(o.y + ly));
        ::ShowCaret(m_hwnd);
    }

    // ── Line helpers ──────────────────────────────────────────────────────────
    std::vector<DWRITE_LINE_METRICS> GetLineMetrics() const
    {
        std::vector<DWRITE_LINE_METRICS> v;
        if (!m_pTextLayout)
            return v;
        UINT32 cnt = 0;
        m_pTextLayout->GetLineMetrics(nullptr, 0, &cnt);
        if (cnt)
        {
            v.resize(cnt);
            m_pTextLayout->GetLineMetrics(v.data(), cnt, &cnt);
        }
        return v;
    }
    int GetLineStart(int nLine) const
    {
        auto v = GetLineMetrics();
        int p = 0;
        for (int i = 0; i < nLine && i < (int)v.size(); ++i)
            p += (int)v[i].length;
        return p;
    }
    int CharToLine(int nChar) const
    {
        auto v = GetLineMetrics();
        int p = 0;
        for (int i = 0; i < (int)v.size(); ++i)
        {
            p += (int)v[i].length;
            if (nChar < p)
                return i;
        }
        return (int)v.size() - 1;
    }
    int GetLineCount() const { return (int)GetLineMetrics().size(); }
    int GetLineLength(int nChar) const
    {
        auto v = GetLineMetrics();
        int p = 0;
        for (auto &m : v)
        {
            int nx = p + (int)m.length;
            if (nChar >= p && nChar < nx)
                return (int)m.length - (int)m.newlineLength;
            p = nx;
        }
        return 0;
    }
    std::wstring GetLineText(int nLine) const
    {
        auto v = GetLineMetrics();
        if (nLine < 0 || nLine >= (int)v.size())
            return {};
        int s = GetLineStart(nLine), l = (int)v[nLine].length - (int)v[nLine].newlineLength;
        if (l < 0)
            l = 0;
        return m_sText.substr(Clamp(s, 0, (int)m_sText.length()), Clamp(l, 0, (int)m_sText.length() - s));
    }

    // ── Navigation ────────────────────────────────────────────────────────────
    void MoveCaretUpDown(bool bUp, bool bExtend)
    {
        if (!m_pTextLayout)
            return;
        float lx = 0, ly = 0, lh = 0;
        PositionToLayoutPoint(m_nCaretPos, lx, ly, lh);
        float ty = bUp ? (ly - lh * 0.5f) : (ly + lh * 1.5f);
        int np = Clamp(LayoutPointToPosition(lx, ty), 0, (int)m_sText.length());
        if (!bExtend)
            m_nSelAnchor = np;
        m_nCaretPos = np;
    }
    int WordStart(int nPos) const
    {
        if (nPos <= 0)
            return 0;
        while (nPos > 0 && iswspace(m_sText[nPos - 1]))
            --nPos;
        while (nPos > 0 && !iswspace(m_sText[nPos - 1]))
            --nPos;
        return nPos;
    }
    int WordEnd(int nPos) const
    {
        int n = (int)m_sText.length();
        while (nPos < n && iswspace(m_sText[nPos]))
            ++nPos;
        while (nPos < n && !iswspace(m_sText[nPos]))
            ++nPos;
        return nPos;
    }

    // ── Overlay thumb geometry ────────────────────────────────────────────────
    D2D1_RECT_F GetVertThumbRect(float w, float h) const
    {
        if (m_bSystemVScroll || !m_bMultiLine || !m_pTextLayout)
            return D2D1::RectF(0, 0, 0, 0);
        DWRITE_TEXT_METRICS tm = {};
        m_pTextLayout->GetMetrics(&tm);
        D2D1_RECT_F cr = GetContentRect(w, h);
        float ch = cr.bottom - cr.top;
        if (tm.height <= ch + 2.f)
            return D2D1::RectF(0, 0, 0, 0);
        float frac = ch / tm.height, tH = std::max(20.f, ch * frac);
        float maxSY = tm.height - ch, tY = cr.top + (maxSY > 0.f ? (m_fScrollY / maxSY) * (ch - tH) : 0.f);
        float tx = w - EDIT_SCROLL_W - 1.f;
        return D2D1::RectF(tx, tY, tx + EDIT_SCROLL_W - 1.f, tY + tH);
    }
    D2D1_RECT_F GetHorzThumbRect(float w, float h) const
    {
        if (m_bSystemHScroll || !m_pTextLayout)
            return D2D1::RectF(0, 0, 0, 0);
        DWRITE_TEXT_METRICS tm = {};
        m_pTextLayout->GetMetrics(&tm);
        D2D1_RECT_F cr = GetContentRect(w, h);
        float cw = cr.right - cr.left;
        if (tm.widthIncludingTrailingWhitespace <= cw + 2.f)
            return D2D1::RectF(0, 0, 0, 0);
        float frac = cw / tm.widthIncludingTrailingWhitespace, tW = std::max(20.f, cw * frac);
        float maxSX = tm.widthIncludingTrailingWhitespace - cw, tX = cr.left + (maxSX > 0.f ? (m_fScrollX / maxSX) * (cw - tW) : 0.f);
        float ty = h - EDIT_SCROLL_W - 1.f;
        return D2D1::RectF(tX, ty, tX + tW, ty + EDIT_SCROLL_W - 1.f);
    }
    static bool PointInRect(POINT pt, const D2D1_RECT_F &r) noexcept
    {
        return (float)pt.x >= r.left && (float)pt.x <= r.right && (float)pt.y >= r.top && (float)pt.y <= r.bottom;
    }

    // ── Text mutation ─────────────────────────────────────────────────────────
    void SaveUndo()
    {
        m_undoRecord = {m_sText, m_nCaretPos, m_nSelAnchor};
        m_bUndoValid = true;
    }
    void InsertRaw(int nPos, const std::wstring &s)
    {
        if (s.empty())
            return;
        nPos = Clamp(nPos, 0, (int)m_sText.length());
        m_sText.insert(nPos, s);
        m_nCaretPos = nPos + (int)s.length();
        m_nSelAnchor = m_nCaretPos;
        m_bLayoutDirty = true;
        m_bModified = true;
    }
    void DeleteSelection()
    {
        if (!HasSelection())
            return;
        m_sText.erase(SelectionStart(), SelectionEnd() - SelectionStart());
        m_nCaretPos = SelectionStart();
        m_nSelAnchor = m_nCaretPos;
        m_bLayoutDirty = true;
        m_bModified = true;
    }
    bool InsertChar(wchar_t c)
    {
        if (m_bReadOnly)
            return false;
        if (m_bUpperCase)
            c = (wchar_t)towupper(c);
        else if (m_bLowerCase)
            c = (wchar_t)towlower(c);
        if (m_bNumbersOnly && !iswdigit(c))
            return false;
        int sl = SelectionEnd() - SelectionStart();
        if ((int)m_sText.length() - sl + 1 > m_nMaxLength)
        {
            NotifyParent(EN_MAXTEXT);
            return false;
        }
        SaveUndo();
        DeleteSelection();
        m_sText.insert(m_nCaretPos, 1, c);
        ++m_nCaretPos;
        m_nSelAnchor = m_nCaretPos;
        m_bLayoutDirty = true;
        m_bModified = true;
        NotifyParent(EN_CHANGE);
        return true;
    }
    bool InsertString(const std::wstring &s)
    {
        if (m_bReadOnly || s.empty())
            return false;
        int sl = SelectionEnd() - SelectionStart();
        if ((int)m_sText.length() - sl + (int)s.length() > m_nMaxLength)
        {
            NotifyParent(EN_MAXTEXT);
            return false;
        }
        SaveUndo();
        DeleteSelection();
        std::wstring ins = s;
        if (m_bUpperCase)
            for (auto &ch : ins)
                ch = (wchar_t)towupper(ch);
        else if (m_bLowerCase)
            for (auto &ch : ins)
                ch = (wchar_t)towlower(ch);
        m_sText.insert(m_nCaretPos, ins);
        m_nCaretPos += (int)ins.length();
        m_nSelAnchor = m_nCaretPos;
        m_bLayoutDirty = true;
        m_bModified = true;
        NotifyParent(EN_CHANGE);
        return true;
    }

    // ── Clipboard ─────────────────────────────────────────────────────────────
    void CopyToClipboard()
    {
        if (!HasSelection())
            return;
        std::wstring s = m_sText.substr(SelectionStart(), SelectionEnd() - SelectionStart());
        HGLOBAL hm = ::GlobalAlloc(GMEM_MOVEABLE, (s.length() + 1) * sizeof(wchar_t));
        if (!hm)
            return;
        wchar_t *p = (wchar_t *)::GlobalLock(hm);
        if (p)
        {
            wcscpy_s(p, s.length() + 1, s.c_str());
            ::GlobalUnlock(hm);
        }
        if (::OpenClipboard(m_hwnd))
        {
            ::EmptyClipboard();
            ::SetClipboardData(CF_UNICODETEXT, hm);
            ::CloseClipboard();
        }
        else
            ::GlobalFree(hm);
    }
    void CutToClipboard()
    {
        if (!HasSelection() || m_bReadOnly)
            return;
        CopyToClipboard();
        SaveUndo();
        DeleteSelection();
        m_bLayoutDirty = true;
        NotifyParent(EN_CHANGE);
        EnsureCaretVisible();
        Invalidate();
        UpdateCaretPosition();
    }
    void PasteFromClipboard()
    {
        if (m_bReadOnly)
            return;
        if (!::OpenClipboard(m_hwnd))
            return;
        HANDLE hd = ::GetClipboardData(CF_UNICODETEXT);
        if (hd)
        {
            const wchar_t *p = (const wchar_t *)::GlobalLock(hd);
            if (p)
            {
                std::wstring s = p;
                if (!m_bMultiLine)
                {
                    s.erase(std::remove(s.begin(), s.end(), L'\n'), s.end());
                    s.erase(std::remove(s.begin(), s.end(), L'\r'), s.end());
                }
                InsertString(s);
                ::GlobalUnlock(hd);
            }
        }
        ::CloseClipboard();
        m_bLayoutDirty = true;
        EnsureCaretVisible();
        Invalidate();
        UpdateCaretPosition();
    }
    void PerformUndo()
    {
        if (!m_bUndoValid)
            return;
        UndoRecord cur = {m_sText, m_nCaretPos, m_nSelAnchor};
        m_sText = m_undoRecord.sText;
        m_nCaretPos = m_undoRecord.nCaret;
        m_nSelAnchor = m_undoRecord.nAnchor;
        m_undoRecord = cur;
        m_bLayoutDirty = true;
        m_bModified = true;
        NotifyParent(EN_CHANGE);
        EnsureCaretVisible();
        Invalidate();
        UpdateCaretPosition();
    }
    void SelectAll()
    {
        m_nSelAnchor = 0;
        m_nCaretPos = (int)m_sText.length();
        Invalidate();
        UpdateCaretPosition();
    }

    // ── Main draw ─────────────────────────────────────────────────────────────
    void DrawControl(float w, float h)
    {
        if (!m_pRenderTarget)
            return;
        if (!m_pBrushBackground) {
            CreateBrushes();
        }
        m_pRenderTarget->Clear(m_pBrushBackground->GetColor());
        bool dis = !::IsWindowEnabled(m_hwnd);
        D2D1_RECT_F cr = GetContentRect(w, h);
        EnsureLayout();

        D2D1_ROUNDED_RECT rr = D2D1::RoundedRect(D2D1::RectF(0.5f, 0.5f, w - 0.5f, h - 0.5f), EDIT_CORNER_R, EDIT_CORNER_R);
        m_pRenderTarget->FillRoundedRectangle(rr, m_pBrushBackground);

        // Selection highlight
        bool bShowSel = HasSelection() && (m_bFocused || m_bNoHideSel);
        if (bShowSel && m_pTextLayout)
        {
            D2D1_POINT_2F o = GetLayoutOrigin(cr);
            int ss = SelectionStart(), sl = SelectionEnd() - ss;
            UINT32 maxH = 64, actH = 0;
            std::vector<DWRITE_HIT_TEST_METRICS> hm(maxH);
            HRESULT hr = m_pTextLayout->HitTestTextRange((UINT32)ss, (UINT32)sl, o.x, o.y, hm.data(), maxH, &actH);
            if (hr == E_NOT_SUFFICIENT_BUFFER)
            {
                hm.resize(actH);
                actH = 0;
                m_pTextLayout->HitTestTextRange((UINT32)ss, (UINT32)sl, o.x, o.y, hm.data(), (UINT32)hm.size(), &actH);
            }
            m_pRenderTarget->PushAxisAlignedClip(cr, D2D1_ANTIALIAS_MODE_ALIASED);
            for (UINT32 i = 0; i < actH; ++i)
                m_pRenderTarget->FillRectangle(
                    D2D1::RectF(hm[i].left, hm[i].top, hm[i].left + hm[i].width, hm[i].top + hm[i].height), m_pBrushSelection);
            m_pRenderTarget->PopAxisAlignedClip();
        }

        // Text / placeholder
        m_pRenderTarget->PushAxisAlignedClip(cr, D2D1_ANTIALIAS_MODE_ALIASED);
        if (m_pTextLayout && !m_sText.empty())
        {
            m_pRenderTarget->DrawTextLayout(GetLayoutOrigin(cr), m_pTextLayout,
                                            dis ? m_pBrushBorder : m_pBrushText, D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);
        }
        else if (!m_bFocused && !m_sPlaceholder.empty())
        {
            IDWriteTextLayout *pPL = nullptr;
            if (m_pTextFormat && m_pDWriteFactory)
            {
                float pw = std::max(cr.right - cr.left, 1.f);
                m_pDWriteFactory->CreateTextLayout(m_sPlaceholder.c_str(),
                                                   (UINT32)m_sPlaceholder.length(), m_pTextFormat, pw, EDIT_MAX_DIM, &pPL);
            }
            if (pPL)
            {
                EditColorScheme sc = m_bDarkMode ? EditDarkScheme() : EditLightScheme();
                ID2D1SolidColorBrush *pBrPH = nullptr;
                m_pRenderTarget->CreateSolidColorBrush(sc.placeholder, &pBrPH);
                if (pBrPH)
                {
                    m_pRenderTarget->DrawTextLayout(GetLayoutOrigin(cr), pPL, pBrPH);
                    pBrPH->Release();
                }
                pPL->Release();
            }
        }
        m_pRenderTarget->PopAxisAlignedClip();

        // Border + focus bar
        ID2D1SolidColorBrush *pBrd = (m_bFocused || m_bHovered) ? m_pBrushBorderHovered : m_pBrushBorder;
        m_pRenderTarget->DrawRoundedRectangle(rr, pBrd, EDIT_BORDER_W);
        if (m_bFocused && !dis)
        {
            float bx0 = EDIT_CORNER_R, bx1 = w - EDIT_CORNER_R, by = h - EDIT_BORDER_W * 0.5f;
            m_pRenderTarget->DrawLine(D2D1::Point2F(bx0, by), D2D1::Point2F(bx1, by),
                                      m_pBrushBorderFocused, EDIT_FOCUS_BAR_H);
        }

        // Overlay scroll thumbs
        auto DrawThumb = [&](const D2D1_RECT_F &tr)
        {
            if(tr.right<=tr.left||tr.bottom<=tr.top) return;
            m_pBrushScrollThumb->SetOpacity(m_bHovered?0.65f:0.30f);
            m_pRenderTarget->FillRoundedRectangle(D2D1::RoundedRect(tr,3.f,3.f),m_pBrushScrollThumb);
            m_pBrushScrollThumb->SetOpacity(1.f); 
        };

        DrawThumb(GetVertThumbRect(w, h));
        DrawThumb(GetHorzThumbRect(w, h));
    }

    // ── Message handlers ──────────────────────────────────────────────────────
    LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        m_bDarkMode = DetectDarkMode();
        DWORD ds = (DWORD)::GetWindowLong(m_hwnd, GWL_STYLE);
        DWORD ex = (DWORD)::GetWindowLong(m_hwnd, GWL_EXSTYLE);
        m_bMultiLine = (ds & ES_MULTILINE) != 0;
        m_bPassword = (ds & ES_PASSWORD) != 0;
        m_bReadOnly = (ds & ES_READONLY) != 0;
        m_bAutoHScroll = (ds & ES_AUTOHSCROLL) != 0;
        m_bAutoVScroll = (ds & ES_AUTOVSCROLL) != 0;
        m_bNoHideSel = (ds & ES_NOHIDESEL) != 0;
        m_bWantReturn = (ds & ES_WANTRETURN) != 0;
        m_bUpperCase = (ds & ES_UPPERCASE) != 0;
        m_bLowerCase = (ds & ES_LOWERCASE) != 0;
        m_bNumbersOnly = (ds & ES_NUMBER) != 0;
        m_bSystemHScroll = (ds & WS_HSCROLL) != 0;
        m_bSystemVScroll = (ds & WS_VSCROLL) != 0;
        if (ds & ES_CENTER)
            m_horzAlign = HorzAlign::Center;
        else if (ds & ES_RIGHT)
            m_horzAlign = HorzAlign::Right;
        else
            m_horzAlign = HorzAlign::Left;
        m_vertAlign = m_bMultiLine ? VertAlign::Top : VertAlign::Center;
        if (ex & WS_EX_CLIENTEDGE)
        {
            ::SetWindowLong(m_hwnd, GWL_EXSTYLE, (LONG)(ex & ~WS_EX_CLIENTEDGE));
            ::SetWindowPos(m_hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
        }
        if (!m_pD2DFactory)
            CreateD2DFactory();
        if (SUCCEEDED(CreateDWriteFactory()) && SUCCEEDED(CreateRenderTarget()) && SUCCEEDED(CreateTextFormat()))
            CreateBrushes();
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        ReleaseResources();
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        bHandled = TRUE;
        return 1;
    }

    LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        PAINTSTRUCT ps;
        HDC hdc = ::BeginPaint(m_hwnd, &ps);
        if (!m_pRenderTarget)
        {
            ::EndPaint(m_hwnd, &ps);
            bHandled = TRUE;
            return 0;
        }
        RECT rc;
        ::GetClientRect(m_hwnd, &rc);
        HRESULT hr = m_pRenderTarget->BindDC(hdc, &rc);
        if (hr == D2DERR_RECREATE_TARGET)
        {
            RecoverDeviceLoss();
            hr = m_pRenderTarget ? m_pRenderTarget->BindDC(hdc, &rc) : E_FAIL;
        }
        if (SUCCEEDED(hr))
        {
            m_pRenderTarget->BeginDraw();
           
            DrawControl((float)(rc.right - rc.left), (float)(rc.bottom - rc.top));
            hr = m_pRenderTarget->EndDraw();
            if (hr == D2DERR_RECREATE_TARGET)
                RecoverDeviceLoss();
        }
        ::EndPaint(m_hwnd, &ps);
        bHandled = TRUE;
        return 0;
    }

    LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        m_bLayoutDirty = true;
        EnsureCaretVisible();
        Invalidate();
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnEnable(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        RebuildBrushes();
        Invalidate();
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnSetCursor(UINT, WPARAM, LPARAM lParam, BOOL &bHandled)
    {
        if (LOWORD(lParam) == HTCLIENT)
        {
            POINT pt;
            ::GetCursorPos(&pt);
            ::ScreenToClient(m_hwnd, &pt);
            RECT rc;
            ::GetClientRect(m_hwnd, &rc);
            float w = (float)(rc.right - rc.left), h = (float)(rc.bottom - rc.top);
            EnsureLayout();
            bool onThumb = (GetVertThumbRect(w, h).right > GetVertThumbRect(w, h).left && PointInRect(pt, GetVertThumbRect(w, h))) || (GetHorzThumbRect(w, h).right > GetHorzThumbRect(w, h).left && PointInRect(pt, GetHorzThumbRect(w, h)));
            ::SetCursor(::LoadCursor(nullptr, onThumb ? IDC_ARROW : IDC_IBEAM));
            bHandled = TRUE;
            return TRUE;
        }
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnMouseActivate(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        if (!m_bFocused)
            ::SetFocus(m_hwnd);
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnSetFocus(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        m_bFocused = true;
        NotifyParent(EN_SETFOCUS);
        Invalidate();
        UpdateCaretPosition();
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnKillFocus(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        m_bFocused = false;
        ::HideCaret(m_hwnd);
        ::DestroyCaret();
        NotifyParent(EN_KILLFOCUS);
        Invalidate();
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnGetDlgCode(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        bHandled = TRUE;
        LRESULT c = DLGC_WANTCHARS | DLGC_WANTARROWS | DLGC_HASSETSEL;
        if (m_bMultiLine && m_bWantReturn)
            c |= DLGC_WANTALLKEYS;
        return c;
    }

    LRESULT OnWmSetFont(UINT, WPARAM wp, LPARAM lp, BOOL &bHandled)
    {
        SetFont((HFONT)wp, LOWORD(lp));
        bHandled = TRUE;
        return 0;
    }
    LRESULT OnWmGetFont(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        bHandled = TRUE;
        return 0;
    }

    LRESULT OnSetText(UINT, WPARAM, LPARAM lParam, BOOL &bHandled)
    {
        LPCWSTR t = (LPCWSTR)lParam;
        m_sText = t ? t : L"";
        if (m_bUpperCase)
            for (auto &c : m_sText)
                c = (wchar_t)towupper(c);
        else if (m_bLowerCase)
            for (auto &c : m_sText)
                c = (wchar_t)towlower(c);
        m_nCaretPos = m_nSelAnchor = (int)m_sText.length();
        m_fScrollX = m_fScrollY = 0.f;
        m_bLayoutDirty = true;
        m_bModified = false;
        NotifyParent(EN_CHANGE);
        EnsureCaretVisible();
        Invalidate();
        bHandled = TRUE;
        return TRUE;
    }

    LRESULT OnGetText(UINT, WPARAM wp, LPARAM lParam, BOOL &bHandled)
    {
        wchar_t *buf = (wchar_t *)lParam;
        UINT cc = (UINT)wp;
        if (!buf || cc == 0)
        {
            bHandled = TRUE;
            return 0;
        }
        UINT n = std::min((UINT)m_sText.length(), cc - 1);
        wcsncpy_s(buf, cc, m_sText.c_str(), n);
        buf[n] = 0;
        bHandled = TRUE;
        return (LRESULT)n;
    }

    LRESULT OnGetTextLength(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        bHandled = TRUE;
        return (LRESULT)m_sText.length();
    }

    LRESULT OnCut(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        CutToClipboard();
        bHandled = TRUE;
        return 0;
    }
    LRESULT OnCopy(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        CopyToClipboard();
        bHandled = TRUE;
        return 0;
    }
    LRESULT OnPaste(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        PasteFromClipboard();
        bHandled = TRUE;
        return 0;
    }
    LRESULT OnClear(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        if (!m_bReadOnly && HasSelection())
        {
            SaveUndo();
            DeleteSelection();
            m_bLayoutDirty = true;
            NotifyParent(EN_CHANGE);
            EnsureCaretVisible();
            Invalidate();
            UpdateCaretPosition();
        }
        bHandled = TRUE;
        return 0;
    }
    LRESULT OnUndo(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        PerformUndo();
        bHandled = TRUE;
        return 0;
    }

    LRESULT OnHScroll(UINT, WPARAM wp, LPARAM, BOOL &bHandled)
    {
        EnsureLayout();
        if (!m_pTextLayout)
        {
            bHandled = TRUE;
            return 0;
        }
        DWRITE_TEXT_METRICS tm = {};
        m_pTextLayout->GetMetrics(&tm);
        RECT rc;
        ::GetClientRect(m_hwnd, &rc);
        float w = (float)(rc.right - rc.left), h = (float)(rc.bottom - rc.top);
        float cw = GetContentRect(w, h).right - GetContentRect(w, h).left;
        float maxSX = std::max(0.f, tm.widthIncludingTrailingWhitespace - cw);
        switch (LOWORD(wp))
        {
        case SB_LINELEFT:
            m_fScrollX -= m_fFontPointSize;
            break;
        case SB_LINERIGHT:
            m_fScrollX += m_fFontPointSize;
            break;
        case SB_PAGELEFT:
            m_fScrollX -= cw;
            break;
        case SB_PAGERIGHT:
            m_fScrollX += cw;
            break;
        case SB_THUMBTRACK:
        case SB_THUMBPOSITION:
        {
            SCROLLINFO si = {sizeof(si), SIF_TRACKPOS};
            ::GetScrollInfo(m_hwnd, SB_HORZ, &si);
            m_fScrollX = (float)si.nTrackPos;
            break;
        }
        case SB_LEFT:
            m_fScrollX = 0;
            break;
        case SB_RIGHT:
            m_fScrollX = maxSX;
            break;
        }
        m_fScrollX = Clampf(m_fScrollX, 0.f, maxSX);
        NotifyParent(EN_HSCROLL);
        Invalidate();
        UpdateCaretPosition();
        bHandled = TRUE;
        return 0;
    }

    LRESULT OnVScroll(UINT, WPARAM wp, LPARAM, BOOL &bHandled)
    {
        EnsureLayout();
        if (!m_pTextLayout || !m_bMultiLine)
        {
            bHandled = TRUE;
            return 0;
        }
        DWRITE_TEXT_METRICS tm = {};
        m_pTextLayout->GetMetrics(&tm);
        RECT rc;
        ::GetClientRect(m_hwnd, &rc);
        float w = (float)(rc.right - rc.left), h = (float)(rc.bottom - rc.top);
        float ch = GetContentRect(w, h).bottom - GetContentRect(w, h).top;
        float lh = m_fFontPointSize * (96.f / 72.f) * 1.2f, maxSY = std::max(0.f, tm.height - ch);
        switch (LOWORD(wp))
        {
        case SB_LINEUP:
            m_fScrollY -= lh;
            break;
        case SB_LINEDOWN:
            m_fScrollY += lh;
            break;
        case SB_PAGEUP:
            m_fScrollY -= ch;
            break;
        case SB_PAGEDOWN:
            m_fScrollY += ch;
            break;
        case SB_THUMBTRACK:
        case SB_THUMBPOSITION:
        {
            SCROLLINFO si = {sizeof(si), SIF_TRACKPOS};
            ::GetScrollInfo(m_hwnd, SB_VERT, &si);
            m_fScrollY = (float)si.nTrackPos;
            break;
        }
        case SB_TOP:
            m_fScrollY = 0;
            break;
        case SB_BOTTOM:
            m_fScrollY = maxSY;
            break;
        }
        m_fScrollY = Clampf(m_fScrollY, 0.f, maxSY);
        NotifyParent(EN_VSCROLL);
        Invalidate();
        UpdateCaretPosition();
        bHandled = TRUE;
        return 0;
    }

    LRESULT OnMouseWheel(UINT, WPARAM wp, LPARAM, BOOL &bHandled)
    {
        EnsureLayout();
        if (!m_bMultiLine || !m_pTextLayout)
        {
            bHandled = TRUE;
            return 0;
        }
        DWRITE_TEXT_METRICS tm = {};
        m_pTextLayout->GetMetrics(&tm);
        RECT rc;
        ::GetClientRect(m_hwnd, &rc);
        float h = (float)(rc.bottom - rc.top), w = (float)(rc.right - rc.left);
        float ch = GetContentRect(w, h).bottom - GetContentRect(w, h).top;
        float lh = m_fFontPointSize * (96.f / 72.f) * 1.2f;
        int d = GET_WHEEL_DELTA_WPARAM(wp);
        m_fScrollY = Clampf(m_fScrollY + (d > 0 ? -lh * 3 : lh * 3), 0.f, std::max(0.f, tm.height - ch));
        UpdateScrollBars();
        Invalidate();
        UpdateCaretPosition();
        bHandled = TRUE;
        return 0;
    }

    LRESULT OnLButtonDown(UINT, WPARAM wp, LPARAM lParam, BOOL &bHandled)
    {
        ::SetFocus(m_hwnd);
        ::SetCapture(m_hwnd);
        POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        EnsureLayout();
        RECT rc;
        ::GetClientRect(m_hwnd, &rc);
        float w = (float)(rc.right - rc.left), h = (float)(rc.bottom - rc.top);
        D2D1_RECT_F vtr = GetVertThumbRect(w, h);
        if (vtr.right > vtr.left && PointInRect(pt, vtr))
        {
            m_bVThumbDrag = true;
            m_fThumbDragStartPx = (float)pt.y;
            m_fThumbDragStartScroll = m_fScrollY;
            bHandled = TRUE;
            return 0;
        }
        D2D1_RECT_F htr = GetHorzThumbRect(w, h);
        if (htr.right > htr.left && PointInRect(pt, htr))
        {
            m_bHThumbDrag = true;
            m_fThumbDragStartPx = (float)pt.x;
            m_fThumbDragStartScroll = m_fScrollX;
            bHandled = TRUE;
            return 0;
        }
        m_bMouseDown = true;
        int p = ClientPointToPosition(pt);
        bool sh = (wp & MK_SHIFT) != 0;
        if (!sh)
            m_nSelAnchor = p;
        m_nCaretPos = p;
        EnsureCaretVisible();
        Invalidate();
        UpdateCaretPosition();
        bHandled = TRUE;
        return 0;
    }

    LRESULT OnLButtonDblClk(UINT, WPARAM, LPARAM lParam, BOOL &bHandled)
    {
        POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        int p = ClientPointToPosition(pt);
        m_nSelAnchor = WordStart(p);
        m_nCaretPos = WordEnd(p);
        Invalidate();
        UpdateCaretPosition();
        bHandled = TRUE;
        return 0;
    }

    LRESULT OnLButtonUp(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        m_bMouseDown = false;
        m_bVThumbDrag = m_bHThumbDrag = false;
        ::ReleaseCapture();
        bHandled = TRUE;
        return 0;
    }

    LRESULT OnMouseMove(UINT, WPARAM wp, LPARAM lParam, BOOL &bHandled)
    {
        POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        EnsureLayout();
        if (m_bVThumbDrag && m_pTextLayout)
        {
            DWRITE_TEXT_METRICS tm = {};
            m_pTextLayout->GetMetrics(&tm);
            RECT rc;
            ::GetClientRect(m_hwnd, &rc);
            float w = (float)(rc.right - rc.left), h = (float)(rc.bottom - rc.top);
            float ch = GetContentRect(w, h).bottom - GetContentRect(w, h).top;
            float tH = std::max(20.f, ch / (tm.height > 0.f ? tm.height : 1.f) * ch);
            float maxSY = std::max(0.f, tm.height - ch), trackH = ch - tH;
            float delta = (float)pt.y - m_fThumbDragStartPx;
            m_fScrollY = Clampf(m_fThumbDragStartScroll + (trackH > 0.f ? delta * maxSY / trackH : 0.f), 0.f, maxSY);
            UpdateScrollBars();
            Invalidate();
            UpdateCaretPosition();
            bHandled = TRUE;
            return 0;
        }
        if (m_bHThumbDrag && m_pTextLayout)
        {
            DWRITE_TEXT_METRICS tm = {};
            m_pTextLayout->GetMetrics(&tm);
            RECT rc;
            ::GetClientRect(m_hwnd, &rc);
            float w = (float)(rc.right - rc.left), h = (float)(rc.bottom - rc.top);
            float cw = GetContentRect(w, h).right - GetContentRect(w, h).left;
            float tW = std::max(20.f, cw / (tm.widthIncludingTrailingWhitespace > 0.f ? tm.widthIncludingTrailingWhitespace : 1.f) * cw);
            float maxSX = std::max(0.f, tm.widthIncludingTrailingWhitespace - cw), trackW = cw - tW;
            float delta = (float)pt.x - m_fThumbDragStartPx;
            m_fScrollX = Clampf(m_fThumbDragStartScroll + (trackW > 0.f ? delta * maxSX / trackW : 0.f), 0.f, maxSX);
            UpdateScrollBars();
            Invalidate();
            UpdateCaretPosition();
            bHandled = TRUE;
            return 0;
        }
        if (m_bMouseDown && (wp & MK_LBUTTON))
        {
            m_nCaretPos = ClientPointToPosition(pt);
            EnsureCaretVisible();
            Invalidate();
            UpdateCaretPosition();
        }
        if (!m_bHovered)
        {
            m_bHovered = true;
            TRACKMOUSEEVENT tme = {sizeof(tme), TME_LEAVE, m_hwnd, 0};
            ::TrackMouseEvent(&tme);
            Invalidate();
        }
        bHandled = TRUE;
        return 0;
    }

    LRESULT OnMouseLeave(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        m_bHovered = false;
        Invalidate();
        bHandled = TRUE;
        return 0;
    }

    LRESULT OnKeyDown(UINT, WPARAM vk, LPARAM, BOOL &bHandled)
    {
        bool sh = (::GetKeyState(VK_SHIFT) & 0x8000) != 0;
        bool ct = (::GetKeyState(VK_CONTROL) & 0x8000) != 0;
        int n = (int)m_sText.length(), np = m_nCaretPos;
        switch (vk)
        {
        case VK_LEFT:
            if (ct)
                np = WordStart(m_nCaretPos);
            else if (!sh && HasSelection())
                np = SelectionStart();
            else
                np = std::max(0, m_nCaretPos - 1);
            if (!sh)
                m_nSelAnchor = np;
            m_nCaretPos = np;
            break;
        case VK_RIGHT:
            if (ct)
                np = WordEnd(m_nCaretPos);
            else if (!sh && HasSelection())
                np = SelectionEnd();
            else
                np = std::min(n, m_nCaretPos + 1);
            if (!sh)
                m_nSelAnchor = np;
            m_nCaretPos = np;
            break;
        case VK_UP:
            if (m_bMultiLine)
            {
                MoveCaretUpDown(true, sh);
                if (!sh)
                    m_nSelAnchor = m_nCaretPos;
            }
            break;
        case VK_DOWN:
            if (m_bMultiLine)
            {
                MoveCaretUpDown(false, sh);
                if (!sh)
                    m_nSelAnchor = m_nCaretPos;
            }
            break;
        case VK_HOME:
        {
            if (ct)
                np = 0;
            else if (m_bMultiLine)
            {
                int li = CharToLine(m_nCaretPos), ls = GetLineStart(li);
                np = (m_nCaretPos == ls) ? 0 : ls;
            }
            else
                np = 0;
            if (!sh)
                m_nSelAnchor = np;
            m_nCaretPos = np;
            break;
        }
        case VK_END:
        {
            if (ct)
                np = n;
            else if (m_bMultiLine)
            {
                int li = CharToLine(m_nCaretPos), ls = GetLineStart(li);
                auto v = GetLineMetrics();
                int ll = (li < (int)v.size()) ? (int)v[li].length - (int)v[li].newlineLength : 0;
                int le = ls + ll;
                np = (m_nCaretPos == le) ? n : le;
            }
            else
                np = n;
            if (!sh)
                m_nSelAnchor = np;
            m_nCaretPos = np;
            break;
        }
        case VK_PRIOR:
            if (m_bMultiLine)
            {
                RECT rc;
                ::GetClientRect(m_hwnd, &rc);
                float w = (float)(rc.right - rc.left), h = (float)(rc.bottom - rc.top);
                float ch = GetContentRect(w, h).bottom - GetContentRect(w, h).top;
                m_fScrollY = Clampf(m_fScrollY - ch, 0.f, m_fScrollY);
                MoveCaretUpDown(true, sh);
                if (!sh)
                    m_nSelAnchor = m_nCaretPos;
                UpdateScrollBars();
            }
            break;
        case VK_NEXT:
            if (m_bMultiLine && m_pTextLayout)
            {
                DWRITE_TEXT_METRICS tm = {};
                m_pTextLayout->GetMetrics(&tm);
                RECT rc;
                ::GetClientRect(m_hwnd, &rc);
                float w = (float)(rc.right - rc.left), h = (float)(rc.bottom - rc.top);
                float ch = GetContentRect(w, h).bottom - GetContentRect(w, h).top;
                m_fScrollY = Clampf(m_fScrollY + ch, 0.f, std::max(0.f, tm.height - ch));
                MoveCaretUpDown(false, sh);
                if (!sh)
                    m_nSelAnchor = m_nCaretPos;
                UpdateScrollBars();
            }
            break;
        case VK_DELETE:
            if (!m_bReadOnly)
            {
                if (HasSelection())
                {
                    SaveUndo();
                    DeleteSelection();
                    m_bLayoutDirty = true;
                    NotifyParent(EN_CHANGE);
                }
                else if (m_nCaretPos < n)
                {
                    SaveUndo();
                    if (ct)
                    {
                        int we = WordEnd(m_nCaretPos);
                        m_sText.erase(m_nCaretPos, we - m_nCaretPos);
                    }
                    else
                        m_sText.erase(m_nCaretPos, 1);
                    m_bLayoutDirty = true;
                    m_bModified = true;
                    NotifyParent(EN_CHANGE);
                }
            }
            break;
        case 'A':
            if (ct)
            {
                SelectAll();
                bHandled = TRUE;
                return 0;
            }
            break;
        case 'C':
            if (ct)
            {
                CopyToClipboard();
                bHandled = TRUE;
                return 0;
            }
            break;
        case 'X':
            if (ct)
            {
                CutToClipboard();
                bHandled = TRUE;
                return 0;
            }
            break;
        case 'V':
            if (ct)
            {
                PasteFromClipboard();
                bHandled = TRUE;
                return 0;
            }
            break;
        case 'Z':
            if (ct)
            {
                PerformUndo();
                bHandled = TRUE;
                return 0;
            }
            break;
        default:
            bHandled = FALSE;
            return 0;
        }
        EnsureCaretVisible();
        Invalidate();
        UpdateCaretPosition();
        bHandled = TRUE;
        return 0;
    }

    LRESULT OnChar(UINT, WPARAM wp, LPARAM, BOOL &bHandled)
    {
        wchar_t c = (wchar_t)wp;
        bool ct = (::GetKeyState(VK_CONTROL) & 0x8000) != 0;
        if (c == L'\b')
        {
            if (!m_bReadOnly)
            {
                if (HasSelection())
                {
                    SaveUndo();
                    DeleteSelection();
                    m_bLayoutDirty = true;
                    NotifyParent(EN_CHANGE);
                }
                else if (m_nCaretPos > 0)
                {
                    SaveUndo();
                    if (ct)
                    {
                        int ws = WordStart(m_nCaretPos);
                        m_sText.erase(ws, m_nCaretPos - ws);
                        m_nCaretPos = ws;
                    }
                    else
                    {
                        --m_nCaretPos;
                        m_sText.erase(m_nCaretPos, 1);
                    }
                    m_nSelAnchor = m_nCaretPos;
                    m_bLayoutDirty = true;
                    m_bModified = true;
                    NotifyParent(EN_CHANGE);
                }
            }
            EnsureCaretVisible();
            Invalidate();
            UpdateCaretPosition();
            bHandled = TRUE;
            return 0;
        }
        if (c == L'\r' || c == L'\n')
        {
            if (m_bMultiLine && (m_bWantReturn || !(::GetKeyState(VK_CONTROL) & 0x8000)))
            {
                if (!m_bReadOnly)
                {
                    SaveUndo();
                    DeleteSelection();
                    m_sText.insert(m_nCaretPos, L"\r\n");
                    m_nCaretPos += 2;
                    m_nSelAnchor = m_nCaretPos;
                    m_bLayoutDirty = true;
                    m_bModified = true;
                    NotifyParent(EN_CHANGE);
                    EnsureCaretVisible();
                    Invalidate();
                    UpdateCaretPosition();
                }
            }
            bHandled = TRUE;
            return 0;
        }
        if (c == L'\t')
        {
            if (m_bMultiLine && !m_bReadOnly)
            {
                if (InsertChar(L'\t'))
                {
                    EnsureCaretVisible();
                    Invalidate();
                    UpdateCaretPosition();
                }
            }
            bHandled = TRUE;
            return 0;
        }
        if (c < L' ')
        {
            bHandled = TRUE;
            return 0;
        }
        if (InsertChar(c))
        {
            EnsureCaretVisible();
            Invalidate();
            UpdateCaretPosition();
        }
        bHandled = TRUE;
        return 0;
    }

    LRESULT OnContextMenu(UINT, WPARAM, LPARAM lParam, BOOL &bHandled)
    {
        HMENU hm = ::CreatePopupMenu();
        if (!hm)
        {
            bHandled = TRUE;
            return 0;
        }
        bool hs = HasSelection(), ht = !m_sText.empty();
        bool cp = ::IsClipboardFormatAvailable(CF_UNICODETEXT) != 0;
        ::AppendMenuW(hm, MF_STRING | (m_bUndoValid ? 0 : MF_GRAYED), EDITCMD_UNDO, L"Undo\tCtrl+Z");
        ::AppendMenuW(hm, MF_SEPARATOR, 0, nullptr);
        ::AppendMenuW(hm, MF_STRING | (hs && !m_bReadOnly ? 0 : MF_GRAYED), EDITCMD_CUT, L"Cut\tCtrl+X");
        ::AppendMenuW(hm, MF_STRING | (hs ? 0 : MF_GRAYED), EDITCMD_COPY, L"Copy\tCtrl+C");
        ::AppendMenuW(hm, MF_STRING | (cp && !m_bReadOnly ? 0 : MF_GRAYED), EDITCMD_PASTE, L"Paste\tCtrl+V");
        ::AppendMenuW(hm, MF_STRING | (hs && !m_bReadOnly ? 0 : MF_GRAYED), EDITCMD_DELETE, L"Delete");
        ::AppendMenuW(hm, MF_SEPARATOR, 0, nullptr);
        ::AppendMenuW(hm, MF_STRING | (ht ? 0 : MF_GRAYED), EDITCMD_SELECTALL, L"Select All\tCtrl+A");
        UINT cmd = (UINT)::TrackPopupMenu(hm, TPM_RIGHTBUTTON | TPM_RETURNCMD,
                                          GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, m_hwnd, nullptr);
        ::DestroyMenu(hm);
        switch (cmd)
        {
        case EDITCMD_UNDO:
            PerformUndo();
            break;
        case EDITCMD_CUT:
            CutToClipboard();
            break;
        case EDITCMD_COPY:
            CopyToClipboard();
            break;
        case EDITCMD_PASTE:
            PasteFromClipboard();
            break;
        case EDITCMD_DELETE:
            if (!m_bReadOnly && hs)
            {
                SaveUndo();
                DeleteSelection();
                m_bLayoutDirty = true;
                NotifyParent(EN_CHANGE);
                EnsureCaretVisible();
                Invalidate();
                UpdateCaretPosition();
            }
            break;
        case EDITCMD_SELECTALL:
            SelectAll();
            Invalidate();
            UpdateCaretPosition();
            break;
        }
        bHandled = TRUE;
        return 0;
    }

    LRESULT OnThemeChanged(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        m_bDarkMode = DetectDarkMode();
        ReleaseBrushes();
        Invalidate();
        bHandled = FALSE;
        return 0;
    }
    LRESULT OnSettingChange(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        m_bDarkMode = DetectDarkMode();
        ReleaseBrushes();
        Invalidate();
        bHandled = FALSE;
        return 0;
    }

    // EM_ handlers
    LRESULT OnEmGetSel(UINT, WPARAM wp, LPARAM lParam, BOOL &bHandled)
    {
        if (wp)
            *(DWORD *)wp = (DWORD)SelectionStart();
        if (lParam)
            *(DWORD *)lParam = (DWORD)SelectionEnd();
        bHandled = TRUE;
        return MAKELONG(SelectionStart(), SelectionEnd());
    }
    LRESULT OnEmSetSel(UINT, WPARAM wp, LPARAM lParam, BOOL &bHandled)
    {
        SetSel((int)wp, (int)lParam);
        bHandled = TRUE;
        return 0;
    }
    LRESULT OnEmReplaceSel(UINT, WPARAM wp, LPARAM lParam, BOOL &bHandled)
    {
        ReplaceSel((LPCWSTR)lParam, (BOOL)wp);
        bHandled = TRUE;
        return 0;
    }
    LRESULT OnEmGetLine(UINT, WPARAM wp, LPARAM lParam, BOOL &bHandled)
    {
        wchar_t *buf = (wchar_t *)lParam;
        if (!buf)
        {
            bHandled = TRUE;
            return 0;
        }
        WORD cc = *(WORD *)buf;
        std::wstring lt = GetLineText((int)wp);
        UINT n = std::min((UINT)lt.length(), (UINT)cc);
        wcsncpy_s(buf, cc, lt.c_str(), n);
        bHandled = TRUE;
        return (LRESULT)n;
    }
    LRESULT OnEmGetLineCount(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        EnsureLayout();
        bHandled = TRUE;
        return (LRESULT)std::max(1, GetLineCount());
    }
    LRESULT OnEmLineFromChar(UINT, WPARAM wp, LPARAM, BOOL &bHandled)
    {
        EnsureLayout();
        bHandled = TRUE;
        return (LRESULT)CharToLine(wp == ~0u ? m_nCaretPos : (int)wp);
    }
    LRESULT OnEmLineIndex(UINT, WPARAM wp, LPARAM, BOOL &bHandled)
    {
        EnsureLayout();
        int l = (int)wp == -1 ? CharToLine(m_nCaretPos) : (int)wp;
        bHandled = TRUE;
        return (LRESULT)GetLineStart(l);
    }
    LRESULT OnEmLineLength(UINT, WPARAM wp, LPARAM, BOOL &bHandled)
    {
        EnsureLayout();
        bHandled = TRUE;
        return (LRESULT)GetLineLength(wp == ~0u ? m_nCaretPos : (int)wp);
    }
    LRESULT OnEmScroll(UINT, WPARAM wp, LPARAM, BOOL &bHandled)
    {
        EnsureLayout();
        if (!m_pTextLayout || !m_bMultiLine)
        {
            bHandled = TRUE;
            return 0;
        }
        DWRITE_TEXT_METRICS tm = {};
        m_pTextLayout->GetMetrics(&tm);
        RECT rc;
        ::GetClientRect(m_hwnd, &rc);
        float w = (float)(rc.right - rc.left), h = (float)(rc.bottom - rc.top);
        float ch = GetContentRect(w, h).bottom - GetContentRect(w, h).top, lh = m_fFontPointSize * (96.f / 72.f) * 1.2f;
        float maxSY = std::max(0.f, tm.height - ch);
        switch (LOWORD(wp))
        {
        case SB_LINEUP:
            m_fScrollY -= lh;
            break;
        case SB_LINEDOWN:
            m_fScrollY += lh;
            break;
        case SB_PAGEUP:
            m_fScrollY -= ch;
            break;
        case SB_PAGEDOWN:
            m_fScrollY += ch;
            break;
        }
        m_fScrollY = Clampf(m_fScrollY, 0.f, maxSY);
        UpdateScrollBars();
        Invalidate();
        UpdateCaretPosition();
        bHandled = TRUE;
        return 0;
    }
    LRESULT OnEmScrollCaret(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        EnsureCaretVisible();
        Invalidate();
        UpdateCaretPosition();
        bHandled = TRUE;
        return 0;
    }
    LRESULT OnEmSetReadOnly(UINT, WPARAM wp, LPARAM, BOOL &bHandled)
    {
        m_bReadOnly = (wp != 0);
        DWORD s = (DWORD)::GetWindowLong(m_hwnd, GWL_STYLE);
        if (m_bReadOnly)
            s |= ES_READONLY;
        else
            s &= ~ES_READONLY;
        ::SetWindowLong(m_hwnd, GWL_STYLE, (LONG)s);
        RebuildBrushes();
        Invalidate();
        bHandled = TRUE;
        return 1;
    }
    LRESULT OnEmLimitText(UINT, WPARAM wp, LPARAM, BOOL &bHandled) // handles both EM_LIMITTEXT and EM_SETLIMITTEXT (same value 0x00C5)
    {
        m_nMaxLength = wp > 0 ? (int)wp : 0x7FFFFFFE;
        bHandled = TRUE;
        return 0;
    }
    LRESULT OnEmGetModify(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        bHandled = TRUE;
        return m_bModified ? 1 : 0;
    }
    LRESULT OnEmSetModify(UINT, WPARAM wp, LPARAM, BOOL &bHandled)
    {
        m_bModified = (wp != 0);
        bHandled = TRUE;
        return 0;
    }
    LRESULT OnEmCanUndo(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        bHandled = TRUE;
        return m_bUndoValid ? 1 : 0;
    }
    LRESULT OnEmEmptyUndoBuffer(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        m_bUndoValid = false;
        bHandled = TRUE;
        return 0;
    }
    LRESULT OnEmGetFirstVisibleLine(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        EnsureLayout();
        if (!m_bMultiLine || !m_pTextLayout)
        {
            bHandled = TRUE;
            return 0;
        }
        auto v = GetLineMetrics();
        float acc = 0.f;
        int fl = 0;
        for (int i = 0; i < (int)v.size(); ++i)
        {
            acc += v[i].baseline;
            if (acc >= m_fScrollY)
            {
                fl = i;
                break;
            }
        }
        bHandled = TRUE;
        return (LRESULT)fl;
    }
};

typedef D2DEditControl D2DEdit;
#endif // __D2D_EDIT_CONTROL_H__
