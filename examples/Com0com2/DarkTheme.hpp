#pragma once
// DarkTheme.hpp  —  Windows 11 Fluent dark theme for Win32
//
// Three-part initialisation
// ─────────────────────────
// 1. DarkTheme::InitProcess()      — WinMain, before any window exists.
//                                    Sets uxtheme ordinals 135/136 so Win32
//                                    menus render dark process-wide.
// 2. m_theme.ApplyToWindow(hwnd)   — each window's WM_CREATE.
// 3. m_theme.ApplyToListView(hwnd) — after the ListView is created.
//    m_theme.ApplyToStatusBar(hwnd)
//
// New this revision
// ─────────────────
// • OnNcPaintMenuSeparator() — erases the 1px bright separator Win32 draws
//   between the menu bar and the client area during WM_NCPAINT.
//   Use with MESSAGE_HANDLER(WM_NCPAINT, ...) so you can call DefWindowProc
//   first, then call this method to paint over the separator.
//
// • HeaderSubclassProc()  — SetWindowSubclass target for the ListView header.
//   Fully owner-draws each column (dark surface bg, white text, thin dividers)
//   matching the menu bar style.  Installed by ApplyToListView().
//
// • StatusBarSubclassProc() — SetWindowSubclass target for the status bar.
//   Calls DefSubclassProc first (so the text areas draw normally) then
//   repaints the grip: AccentDark background with white diagonal lines.
//   Installed by ApplyToStatusBar().
//
// Requires (link): dwmapi  uxtheme  comctl32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dwmapi.h>
#include <uxtheme.h>
#include <commctrl.h> // SetWindowSubclass, Header_*, ListView_*
#include <string>

#ifdef _MSC_VER
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "uxtheme.lib")
#pragma comment(lib, "comctl32.lib")
#endif

// ── UAH undocumented menu-bar draw messages (stable since Win10 1809) ────────
#ifndef WM_UAHDRAWMENU
#define WM_UAHDRAWMENU 0x0091
#define WM_UAHDRAWMENUITEM 0x0092
#endif

#define WCA_USEDARKMODECOLORS 26

typedef enum _WINDOWCOMPISITIONATTRIB
{
    WCA_UNDEFINED,
    WCA_NCRENDERING_ENABLED,
    WCA_NCRENDERING_POLICY,
    WCA_TRANSITIONS_FORCEDISABLED,
    WCA_ALLOW_NCPAINT,
    WCA_CAPTION_BUTTON_BOUNDS,
    WCA_NONCLIENT_RTL_LAYOUT,
    WCA_FORCE_ICONIC_REPRESENTATION,
    WCA_EXTEND_FRAME_BOUNDS,
    WCA_HAS_ICONIC_BITMAP,
    WCA_THEME_ATTRIBUTES,
    WCA_NCPRESENT_WITH_THE_WINDOW_STYLE,
    WCA_VISIBLE_FRAME_BORDER_THICKNESS,
    WCA_EXPANDED_STACK_DISP_RECT,
    WCA_PASSIVE_INTERACTION__WINDOW_STYLE,
    WCA_ACTIVE_TEXT_COLOR,
    WCA_USE_STANDARD_STATE_SIZE,
    WCA_WINDOW_RESIZE_GRIP_MODE,
    WCA_WINDOW_FEEDBACK_SETTING,
    WCA_PAN_GESTURE_HANDLER_CONDITION,
    WCA_TRANSITION_COMPLETED,
    WCA_FAST_TRANSITION_EX_WINDOW_STYLE,
    WCA_FULLSCREEN_PRESENT__WINDOW_STYLE,
    WCA_MOUSE_WHEEL_ROUTING_MODE,
    WCA_ANY_WINDOW_STYLE,
    WCA_ACCENT_POLICY,
    WCA_FREEZE_PRESENTATION,
    WCA_EVER_UNROLLED,
    WCA_LAST
} WINDOWCOMPISITIONATTRIB;

typedef struct _WINDOWCOMPOSITIONATTRIBDATA
{
    WINDOWCOMPISITIONATTRIB Attrib;
    PVOID pvData;
    SIZE_T cbData;
} WINDOWCOMPOSITIONATTRIBDATA;

typedef BOOL(WINAPI *fnSetWindowCompositionAttribute)(HWND hwnd, WINDOWCOMPOSITIONATTRIBDATA *pwcad);

struct UahMenu
{
    HMENU hmenu;
    HDC hdc;
    DWORD dwFlags;
};
struct UahMenuItemMetrics
{
    union
    {
        struct
        {
            DWORD cx;
            DWORD cy;
        } rgsizeBar[2];
        struct
        {
            DWORD cx;
            DWORD cy;
        } rgsizePopup[4];
    };
};
struct UahMenuPopupMetrics
{
    DWORD rgcx[4];
    DWORD fUpdateMaxWidths : 2;
};
struct UahMenuItem
{
    int iPosition;
    UahMenuItemMetrics umim;
    UahMenuPopupMetrics umpm;
};
struct UahDrawMenuItem
{
    DRAWITEMSTRUCT dis;
    UahMenu um;
    UahMenuItem umi;
};

// ── Runtime palette — holds all colors for whichever theme is active ─────────
// Replaced the static DarkColors namespace so light/dark can be selected at
// runtime and every drawing path (including subclass procs) sees the right color.
struct PaletteP
{
    COLORREF Background, Surface, SurfaceRaised;
    COLORREF ControlFill, ControlHot, ControlPressed, ControlDisabled;
    COLORREF InputBg, Border, BorderFocused;
    COLORREF MenuItemHot, HeaderDivider;
    COLORREF TextPrimary, TextSecondary, TextDisabled;
    COLORREF ListBg, AccentDark;
};

// WinUI 3 dark-mode token values
inline PaletteP DarkPalette()
{
    return {
        RGB(32, 32, 32),    // Background      #202020
        RGB(44, 44, 44),    // Surface         #2C2C2C
        RGB(54, 54, 54),    // SurfaceRaised   #363636
        RGB(61, 61, 61),    // ControlFill     #3D3D3D
        RGB(72, 72, 72),    // ControlHot      #484848
        RGB(40, 40, 40),    // ControlPressed  #282828
        RGB(48, 48, 48),    // ControlDisabled #303030
        RGB(19, 19, 19),    // InputBg         #131313
        RGB(68, 68, 68),    // Border          #444444
        RGB(96, 205, 255),  // BorderFocused   #60CDFF
        RGB(54, 54, 54),    // MenuItemHot     #363636
        RGB(64, 64, 64),    // HeaderDivider   #404040
        RGB(200, 200, 200), // TextPrimary     #C8C8C8
        RGB(171, 171, 171), // TextSecondary   #ABABAB
        RGB(99, 99, 99),    // TextDisabled    #636363
        RGB(32, 32, 32),    // ListBg          #202020
        RGB(0, 120, 212),   // AccentDark      #0078D4
    };
}

// WinUI 3 light-mode token values
inline PaletteP LightPalette()
{
    return {
        RGB(243, 243, 243), // Background      #F3F3F3
        RGB(255, 255, 255), // Surface         #FFFFFF
        RGB(251, 251, 251), // SurfaceRaised   #FBFBFB
        RGB(251, 251, 251), // ControlFill     #FBFBFB
        RGB(246, 246, 246), // ControlHot      #F6F6F6
        RGB(237, 237, 237), // ControlPressed  #EDEDED
        RGB(249, 249, 249), // ControlDisabled #F9F9F9
        RGB(255, 255, 255), // InputBg         #FFFFFF
        RGB(204, 204, 204), // Border          #CCCCCC
        RGB(0, 103, 192),   // BorderFocused   #0067C0
        RGB(229, 229, 229), // MenuItemHot     #E5E5E5
        RGB(204, 204, 204), // HeaderDivider   #CCCCCC
        RGB(0, 0, 0),       // TextPrimary     #000000
        RGB(89, 89, 89),    // TextSecondary   #595959
        RGB(160, 160, 160), // TextDisabled    #A0A0A0
        RGB(255, 255, 255), // ListBg          #FFFFFF
        RGB(0, 103, 192),   // AccentDark      #0067C0
    };
}

// ─────────────────────────────────────────────────────────────────────────────
class DarkTheme
{
public:
    DarkTheme()
    {
        m_isDark = IsSystemDark();
        m_pal = m_isDark ? DarkPalette() : LightPalette();
        CreateBrushes();
    }
    ~DarkTheme() { DestroyBrushes(); }
    DarkTheme(const DarkTheme &) = delete;
    DarkTheme &operator=(const DarkTheme &) = delete;

    // ── System theme detection ────────────────────────────────────────────────
    // Reads HKCU\...\Themes\Personalize\AppsUseLightTheme.
    // Returns true if the system is in dark mode (value == 0 or key missing).
    static bool IsSystemDark()
    {
        DWORD val = 1; // default: light (1 = light, 0 = dark)
        DWORD size = sizeof(val);
        ::RegGetValueW(HKEY_CURRENT_USER,
                       L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                       L"AppsUseLightTheme",
                       RRF_RT_REG_DWORD, nullptr, &val, &size);
        return val == 0; // 0 = dark mode active
    }

    // ── Refresh palette and brushes to match current system theme ─────────────
    // Call from WM_SETTINGCHANGE("ImmersiveColorSet") before re-applying.
    // Returns true if the theme is now dark.
    bool Refresh()
    {
        m_isDark = IsSystemDark();
        m_pal = m_isDark ? DarkPalette() : LightPalette();
        DestroyBrushes();
        CreateBrushes();
        return m_isDark;
    }

    bool IsDark() const { return m_isDark; }

    // ── 1. Process-wide init (call from WinMain, before any window) ──────────
    static void InitProcess()
    {
        auto &ux = UxTheme();
        // AllowDark (1): menus follow the system theme.
        // When system is dark, menus go dark; when light, menus stay light.
        if (ux.pSetPreferredAppMode)
            ux.pSetPreferredAppMode(1 /*AllowDark*/);
        if (ux.pFlushMenuThemes)
            ux.pFlushMenuThemes();
    }

    // ── 2. Per-window (call from WM_CREATE and on every NC paint) ───────────
    // Applies dark/light title bar, persistent SWCA lock, Mica, rounded corners.
    // All attributes follow m_isDark which tracks the actual system preference.
    void ApplyToWindow(HWND hwnd) const
    {
        BOOL dark = m_isDark ? TRUE : FALSE;

        // Dark/light title bar (DWMWA_USE_IMMERSIVE_DARK_MODE)
        if (FAILED(::DwmSetWindowAttribute(hwnd, 20, &dark, sizeof(dark))))
            ::DwmSetWindowAttribute(hwnd, 19, &dark, sizeof(dark));

        // Per-window non-client dark/light flag
        auto &ux = UxTheme();
        if (ux.pAllowDarkModeForWindow)
            ux.pAllowDarkModeForWindow(hwnd, m_isDark);

        // SetWindowCompositionAttribute(WCA_USEDARKMODECOLORS) — persistent lock.
        // DWM reads this on every composition pass so it cannot be reset by
        // minimize/restore animations.  Follows m_isDark so light mode clears it.
        WINDOWCOMPOSITIONATTRIBDATA wcad{static_cast<WINDOWCOMPISITIONATTRIB>(WCA_USEDARKMODECOLORS), &dark, sizeof(dark)};
        auto &swca = SwcaFn();
        if (swca.pSetWindowCompositionAttribute)
            swca.pSetWindowCompositionAttribute(hwnd, &wcad);

        // Mica backdrop (Win11 22H2+). Silently fails on Win10.
        constexpr DWORD kBackdrop = 38, kMica = 2;
        if (FAILED(::DwmSetWindowAttribute(hwnd, kBackdrop, &kMica, sizeof(kMica))))
        {
            BOOL b = TRUE;
            ::DwmSetWindowAttribute(hwnd, 1029, &b, sizeof(b));
        }
        // Rounded corners
        constexpr DWORD kCorner = 33, kRound = 2;
        ::DwmSetWindowAttribute(hwnd, kCorner, &kRound, sizeof(kRound));
    }

    // ── 3a. ListView + header subclass ────────────────────────────────────────
    // ApplyToListView installs HeaderSubclassProc on the header child so the
    // column headers match the dark menu bar exactly (no extra WM_NOTIFY wiring
    // required in the parent window).
    void ApplyToListView(HWND hwnd) const
    {
        // Theme strings follow system preference.
        // Light: reset to default explorer; Dark: DarkMode_Explorer.
        ::SetWindowTheme(hwnd, m_isDark ? L"DarkMode_Explorer" : L"Explorer", nullptr);
        ListView_SetBkColor(hwnd, m_pal.ListBg);
        ListView_SetTextBkColor(hwnd, m_pal.ListBg);
        ListView_SetTextColor(hwnd, m_pal.TextPrimary);

        // comctl32 sometimes re-applies WS_EX_CLIENTEDGE after creation.
        // Strip it explicitly and force a frame recalc.
        ::SetWindowLong(hwnd, GWL_EXSTYLE,
                        ::GetWindowLong(hwnd, GWL_EXSTYLE) & ~WS_EX_CLIENTEDGE);
        ::SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
                       SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

        // Subclass the ListView to suppress its internal NC border
        // without flicker using the screen-coordinate clipping region technique.
        ::SetWindowSubclass(hwnd, ListViewSubclassProc, 3,
                            reinterpret_cast<DWORD_PTR>(this));

        HWND hHdr = ListView_GetHeader(hwnd);
        if (hHdr)
        {
            // DarkMode_ItemsView gives the header the right scrollbar chrome
            // but still leaves the column bg light — subclass handles painting.
            ::SetWindowTheme(hHdr, m_isDark ? L"DarkMode_ItemsView" : L"ItemsView", nullptr);
            ::SetWindowSubclass(hHdr, HeaderSubclassProc, 1,
                                reinterpret_cast<DWORD_PTR>(this));
        }
    }

    // ── 3b. Status bar + grip subclass ────────────────────────────────────────
    // ApplyToStatusBar installs StatusBarSubclassProc so the grip is drawn with
    // AccentDark background and white diagonal lines.
    void ApplyToStatusBar(HWND hwnd) const
    {
        ::SetWindowTheme(hwnd, m_isDark ? L"DarkMode_Explorer" : nullptr, nullptr);
        // SB_SETBKCOLOR is not needed — StatusBarSubclassProc fully owns all
        // painting and fills every pixel itself.
        ::SetWindowSubclass(hwnd, StatusBarSubclassProc, 2,
                            reinterpret_cast<DWORD_PTR>(this));
    }

    void ApplyToEdit(HWND hwnd) const { ::SetWindowTheme(hwnd, m_isDark ? L"DarkMode_CFD" : nullptr, nullptr); }
    void ApplyToButton(HWND hwnd) const { ::SetWindowTheme(hwnd, m_isDark ? L"DarkMode_Explorer" : nullptr, nullptr); }

    // ── WM_NCPAINT — menu bar separator fix ──────────────────────────────────
    // Call this AFTER DefWindowProc(WM_NCPAINT) to erase the 1px bright line
    // Windows draws between the menu bar bottom and the client area top.
    void OnNcPaintMenuSeparator(HWND hwnd) const
    {
        MENUBARINFO mbi{sizeof(mbi)};
        if (!::GetMenuBarInfo(hwnd, OBJID_MENU, 0, &mbi))

            return;

        HDC hdc = ::GetWindowDC(hwnd);
        if (!hdc)
            return;

        RECT rcWin{};
        ::GetWindowRect(hwnd, &rcWin);

        // The separator is exactly 1px below mbi.rcBar — convert to window coords
        RECT rcSep = mbi.rcBar;
        ::OffsetRect(&rcSep, -rcWin.left, -rcWin.top);
        rcSep.top = rcSep.bottom;
        rcSep.bottom = rcSep.top + 4;

        ::FillRect(hdc, &rcSep, m_brBackground);
        ::ReleaseDC(hwnd, hdc);
    }

    // ── WM_ERASEBKGND ─────────────────────────────────────────────────────────
    BOOL OnEraseBkgnd(HDC hdc, HWND hwnd) const
    {
        RECT rc{};
        ::GetClientRect(hwnd, &rc);
        ::FillRect(hdc, &rc, m_brBackground);
        return TRUE;
    }

    // ── WM_CTLCOLOR* ─────────────────────────────────────────────────────────
    HBRUSH OnCtlColorStatic(HDC hdc, HWND) const
    {
        ::SetTextColor(hdc, m_pal.TextPrimary);
        ::SetBkColor(hdc, m_pal.Background);
        ::SetBkMode(hdc, OPAQUE);
        return m_brBackground;
    }
    HBRUSH OnCtlColorEdit(HDC hdc, HWND) const
    {
        ::SetTextColor(hdc, m_pal.TextPrimary);
        ::SetBkColor(hdc, m_pal.InputBg);
        return m_brInput;
    }
    HBRUSH OnCtlColorBtn(HDC hdc, HWND) const
    {
        ::SetTextColor(hdc, m_pal.TextPrimary);
        ::SetBkColor(hdc, m_pal.ControlFill);
        return m_brControl;
    }

    // ── UAH menu bar custom draw ──────────────────────────────────────────────
    LRESULT OnUahDrawMenu(HWND hwnd, LPARAM lParam) const
    {
        auto *pum = reinterpret_cast<UahMenu *>(lParam);
        MENUBARINFO mbi{sizeof(mbi)};
        if (!::GetMenuBarInfo(hwnd, OBJID_MENU, 0, &mbi))
            return 0;
        RECT rcWin{};
        ::GetWindowRect(hwnd, &rcWin);
        RECT rcBar = mbi.rcBar;
        ::OffsetRect(&rcBar, -rcWin.left, -rcWin.top);
        ::FillRect(pum->hdc, &rcBar, m_brBackground);
        return 0;
    }

    LRESULT OnUahDrawMenuItem(LPARAM lParam) const
    {
        auto *pudmi = reinterpret_cast<UahDrawMenuItem *>(lParam);
        DRAWITEMSTRUCT &dis = pudmi->dis;
        // dis.rcItem.bottom += 4;
        const bool isHot = (dis.itemState & ODS_HOTLIGHT) || (dis.itemState & ODS_SELECTED);
        const bool isDisabled = (dis.itemState & ODS_GRAYED) || (dis.itemState & ODS_DISABLED);
        ::FillRect(dis.hDC, &dis.rcItem, isHot ? m_brMenuHot : m_brBackground);
        TCHAR text[256]{};
        MENUITEMINFO mii{sizeof(mii)};
        mii.fMask = MIIM_STRING;
        mii.dwTypeData = text;
        mii.cch = static_cast<UINT>(_countof(text) - 1);
        ::GetMenuItemInfo(pudmi->um.hmenu, pudmi->umi.iPosition, TRUE, &mii);
        ::SetTextColor(dis.hDC, isDisabled ? m_pal.TextDisabled : m_pal.TextPrimary);
        ::SetBkMode(dis.hDC, TRANSPARENT);
        UINT dtFlags = DT_CENTER | DT_SINGLELINE | DT_VCENTER;
        if (dis.itemState & ODS_NOACCEL)
            dtFlags |= DT_HIDEPREFIX;
        ::DrawText(dis.hDC, text, -1, &dis.rcItem, dtFlags);
        return 0;
    }

    // ── WM_DRAWITEM — owner-draw push button ─────────────────────────────────
    void OnDrawButton(LPDRAWITEMSTRUCT dis) const
    {
        if (dis->CtlType != ODT_BUTTON)
            return;
        const bool isPressed = (dis->itemState & ODS_SELECTED) != 0;
        const bool isFocused = (dis->itemState & ODS_FOCUS) != 0;
        const bool isDisabled = (dis->itemState & ODS_DISABLED) != 0;
        const bool isHot = (dis->itemState & ODS_HOTLIGHT) != 0;

        COLORREF bg;
        if (isDisabled)
            bg = m_pal.ControlDisabled;
        else if (isPressed)
            bg = m_pal.ControlPressed;
        else if (isHot)
            bg = m_pal.ControlHot;
        else
            bg = m_pal.ControlFill;

        HBRUSH hbg = ::CreateSolidBrush(bg);
        ::FillRect(dis->hDC, &dis->rcItem, hbg);
        ::DeleteObject(hbg);

        HPEN hpen = ::CreatePen(PS_SOLID, 1, isFocused ? m_pal.BorderFocused : m_pal.Border);
        HPEN hOldPn = static_cast<HPEN>(::SelectObject(dis->hDC, hpen));
        HBRUSH hOldBr = static_cast<HBRUSH>(::SelectObject(dis->hDC, ::GetStockObject(NULL_BRUSH)));
        ::Rectangle(dis->hDC, dis->rcItem.left, dis->rcItem.top,
                    dis->rcItem.right, dis->rcItem.bottom);
        ::SelectObject(dis->hDC, hOldPn);
        ::SelectObject(dis->hDC, hOldBr);
        ::DeleteObject(hpen);

        TCHAR text[256]{};
        ::GetWindowText(dis->hwndItem, text, static_cast<int>(_countof(text)));
        ::SetTextColor(dis->hDC, isDisabled ? m_pal.TextDisabled : m_pal.TextPrimary);
        ::SetBkMode(dis->hDC, TRANSPARENT);
        RECT rcTxt = dis->rcItem;
        ::InflateRect(&rcTxt, -2, -2);
        ::DrawText(dis->hDC, text, -1, &rcTxt,
                   DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);

        if (isFocused)
        {
            RECT rcF = dis->rcItem;
            ::InflateRect(&rcF, -3, -3);
            ::DrawFocusRect(dis->hDC, &rcF);
        }
    }

    HBRUSH BrBackground() const { return m_brBackground; }
    HBRUSH BrSurface() const { return m_brSurface; }
    HBRUSH BrInput() const { return m_brInput; }
    HBRUSH BrControl() const { return m_brControl; }

private:
    // ── ListView subclass — border suppression without flicker ──────────────
    //
    // Root cause of the border:
    //   The ListView common control calls DrawEdge in its own WM_NCPAINT,
    //   painting a 1px grey frame regardless of WS_BORDER / WS_EX_CLIENTEDGE.
    //
    // Root cause of the position-dependent failure (previous implementation):
    //   WM_NCPAINT HRGNs are in SCREEN coordinates.  The old code created the
    //   clipping regions in window-relative (0,0-origin) space.  At screen x=0
    //   the two coordinate systems happen to match; anywhere else DefSubclassProc
    //   received a region pointing at the screen's top-left corner instead of
    //   the ListView, so it ignored the clip and painted the border anyway.
    //
    // Fix:
    //   Build all HRGNs in screen coordinates (from GetWindowRect).  Pass the
    //   inner screen-coord region to DefSubclassProc so scrollbars paint inside
    //   it but the border pixels are never touched.  Then offset hrgnBorder to
    //   window-relative space (GetWindowDC origin) before calling FillRgn.
    //   WM_WINDOWPOSCHANGED forces an immediate WM_NCPAINT after every resize
    //   or move so the border is repainted before Windows composites the frame.
    static LRESULT CALLBACK ListViewSubclassProc(HWND hwnd, UINT msg,
                                                 WPARAM wp, LPARAM lp,
                                                 UINT_PTR id, DWORD_PTR data)
    {
        if (msg == WM_NCPAINT)
        {
            // Screen coordinates — required by WM_NCPAINT.
            RECT rcWin{};
            ::GetWindowRect(hwnd, &rcWin);
            const LONG sl = rcWin.left, st = rcWin.top;
            const LONG sr = rcWin.right, sb = rcWin.bottom;

            // Inner = full window minus 1px border, in screen coords.
            HRGN hrgnFull = ::CreateRectRgn(sl, st, sr, sb);
            HRGN hrgnInner = ::CreateRectRgn(sl + 1, st + 1, sr - 1, sb - 1);
            HRGN hrgnBorder = ::CreateRectRgn(0, 0, 0, 0);
            ::CombineRgn(hrgnBorder, hrgnFull, hrgnInner, RGN_DIFF);

            // If caller gave a specific update HRGN, restrict hrgnInner to it.
            if (wp != 1)
                ::CombineRgn(hrgnInner, hrgnInner,
                             reinterpret_cast<HRGN>(wp), RGN_AND);

            // Scrollbars paint inside the inner region; border pixels untouched.
            ::DefSubclassProc(hwnd, WM_NCPAINT,
                              reinterpret_cast<WPARAM>(hrgnInner), lp);

            // GetWindowDC origin = window top-left (window-relative coords),
            // so offset hrgnBorder from screen space to window-relative space.
            HDC hdc = ::GetWindowDC(hwnd);
            if (hdc)
            {
                ::OffsetRgn(hrgnBorder, -sl, -st);
                const DarkTheme *t = reinterpret_cast<const DarkTheme *>(data);
                ::FillRgn(hdc, hrgnBorder, t->m_brBackground);
                ::ReleaseDC(hwnd, hdc);
            }

            ::DeleteObject(hrgnFull);
            ::DeleteObject(hrgnInner);
            ::DeleteObject(hrgnBorder);
            return 0;
        }

        // After every resize/move, force our themed WM_NCPAINT immediately so
        // the border strips are filled before the compositor presents the frame.
        if (msg == WM_WINDOWPOSCHANGED)
        {
            LRESULT lr = ::DefSubclassProc(hwnd, msg, wp, lp);
            ::SendMessage(hwnd, WM_NCPAINT, 1, 0);
            return lr;
        }

        if (msg == WM_NCDESTROY)
            ::RemoveWindowSubclass(hwnd, ListViewSubclassProc, id);

        return ::DefSubclassProc(hwnd, msg, wp, lp);
    }

    // ── Header subclass proc ─────────────────────────────────────────────────
    // Fully owner-draws the ListView header: dark surface background, white
    // column text, thin border dividers — identical visual weight to the menu bar.
    static LRESULT CALLBACK HeaderSubclassProc(HWND hwnd, UINT msg,
                                               WPARAM wp, LPARAM lp,
                                               UINT_PTR id, DWORD_PTR data)
    {

        // Suppress the header's own WM_NCPAINT — it draws a 1px bottom separator
        // via DrawEdge.  We own all header chrome in WM_PAINT, so NC painting
        // is not needed.
        if (msg == WM_NCPAINT)
            return 0;

        if (msg == WM_ERASEBKGND)
            return 1; // suppress default erase

        if (msg == WM_PAINT)
        {
            const DarkTheme *t = reinterpret_cast<const DarkTheme *>(data);
            PAINTSTRUCT ps{};
            HDC hdc = ::BeginPaint(hwnd, &ps);

            RECT rcClient{};
            ::GetClientRect(hwnd, &rcClient);

            // Fill entire header background with Surface colour
            ::FillRect(hdc, &rcClient, t->m_brSurface);

            const int colCount = Header_GetItemCount(hwnd);
            for (int i = 0; i < colCount; ++i)
            {
                RECT rcItem{};
                if (!Header_GetItemRect(hwnd, i, &rcItem))
                    continue;

                // Thin right-edge divider (skip on last column)
                if (i < colCount - 1)
                {
                    HPEN hpen = ::CreatePen(PS_SOLID, 1, t->m_pal.HeaderDivider);
                    HPEN hOldPn = static_cast<HPEN>(::SelectObject(hdc, hpen));
                    ::MoveToEx(hdc, rcItem.right - 1, rcItem.top + 3, nullptr);
                    ::LineTo(hdc, rcItem.right - 1, rcItem.bottom - 3);
                    ::SelectObject(hdc, hOldPn);
                    ::DeleteObject(hpen);
                }

                // Column text
                TCHAR text[256]{};
                HDITEM hdi{};
                hdi.mask = HDI_TEXT | HDI_FORMAT;
                hdi.pszText = text;
                hdi.cchTextMax = static_cast<int>(_countof(text) - 1);
                Header_GetItem(hwnd, i, &hdi);

                if (text[0] == L'\0')
                    continue;

                ::SetTextColor(hdc, t->m_pal.TextPrimary);
                ::SetBkMode(hdc, TRANSPARENT);

                RECT rcText = rcItem;
                ::InflateRect(&rcText, -6, 0); // left/right padding

                UINT dtFlags = DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;
                if (hdi.fmt & HDF_RIGHT)
                    dtFlags |= DT_RIGHT;
                else if (hdi.fmt & HDF_CENTER)
                    dtFlags |= DT_CENTER;
                else
                    dtFlags |= DT_LEFT;

                ::DrawText(hdc, text, -1, &rcText, dtFlags);
            }

            ::EndPaint(hwnd, &ps);
            return 0;
        }

        if (msg == WM_NCDESTROY)
            ::RemoveWindowSubclass(hwnd, HeaderSubclassProc, id);

        return ::DefSubclassProc(hwnd, msg, wp, lp);
    }

    // ── Status bar subclass proc ──────────────────────────────────────────────
    // Calls DefSubclassProc first (text areas draw normally), then repaints
    // the size grip: AccentDark background with 3 white diagonal lines.
    static LRESULT CALLBACK StatusBarSubclassProc(HWND hwnd, UINT msg,
                                                  WPARAM wp, LPARAM lp,
                                                  UINT_PTR id, DWORD_PTR data)
    {
        DarkTheme *t = reinterpret_cast<DarkTheme *>(data);
        // Suppress the common control's own NC border (1px line at the bottom
        // drawn by DrawEdge in WM_NCPAINT).  We fully own all visual output.
        if (msg == WM_NCPAINT)
            return 0;

        if (msg == WM_PAINT)
        {
            // Full paint ownership — DefSubclassProc draws system-colored panels
            // with highlight borders.  We own every pixel so no system drawing
            // leaks through.
            PAINTSTRUCT ps{};
            HDC hdc = ::BeginPaint(hwnd, &ps);

            RECT rcClient{};
            ::GetClientRect(hwnd, &rcClient);

            // 1. Fill entire bar with AccentDark so every pixel starts correct.
            HBRUSH hbrBar = ::CreateSolidBrush(t->m_pal.AccentDark);
            ::FillRect(hdc, &rcClient, hbrBar);
            ::DeleteObject(hbrBar);

            // 2. Draw text for each part.
            const int nParts = static_cast<int>(
                ::SendMessage(hwnd, SB_GETPARTS, 0, 0));
            ::SetTextColor(hdc, RGB(255, 255, 255));
            ::SetBkMode(hdc, TRANSPARENT);

            for (int i = 0; i < nParts; ++i)
            {
                RECT rcPart{};
                ::SendMessage(hwnd, SB_GETRECT, static_cast<WPARAM>(i),
                              reinterpret_cast<LPARAM>(&rcPart));

                const int textLen = LOWORD(::SendMessage(hwnd, SB_GETTEXTLENGTH,
                                                         static_cast<WPARAM>(i), 0));
                if (textLen > 0)
                {
                    // tstring owns the buffer — no stack array returned as pointer
                    std::wstring buf(static_cast<size_t>(textLen) + 1, L'\0');
                    ::SendMessage(hwnd, SB_GETTEXT, static_cast<WPARAM>(i),
                                  reinterpret_cast<LPARAM>(buf.data()));
                    buf.resize(static_cast<size_t>(textLen));

                    RECT rcText = rcPart;
                    ::InflateRect(&rcText, -4, 0);
                    ::DrawTextW(hdc, buf.c_str(), -1, &rcText,
                                DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
                }
            }

            // 3. Resize grip — same AccentDark bg (already filled), white diagonals.
            const bool hasGrip =
                (::GetWindowLong(hwnd, GWL_STYLE) & SBARS_SIZEGRIP) != 0;
            if (hasGrip)
            {
                const int gripW = ::GetSystemMetrics(SM_CXVSCROLL);
                const int gripH = ::GetSystemMetrics(SM_CYHSCROLL);
                RECT rcGrip{
                    rcClient.right - gripW - 2,
                    rcClient.bottom - gripH - 2,
                    rcClient.right,
                    rcClient.bottom};

                HPEN hpen = ::CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
                HPEN hOldPn = static_cast<HPEN>(::SelectObject(hdc, hpen));
                for (int i = 1; i <= 3; ++i)
                {
                    const int off = i * 4;
                    ::MoveToEx(hdc, rcGrip.right - off, rcGrip.bottom, nullptr);
                    ::LineTo(hdc, rcGrip.right, rcGrip.bottom - off);
                }
                ::SelectObject(hdc, hOldPn);
                ::DeleteObject(hpen);
            }

            ::EndPaint(hwnd, &ps);
            return 0;
        }

        if (msg == WM_NCDESTROY)
            ::RemoveWindowSubclass(hwnd, StatusBarSubclassProc, id);

        return ::DefSubclassProc(hwnd, msg, wp, lp);
    }

    // ── Undocumented uxtheme ordinals ─────────────────────────────────────────
    using fnAllowDarkModeForWindow = bool(WINAPI *)(HWND, bool);
    using fnSetPreferredAppMode = DWORD(WINAPI *)(DWORD);
    using fnFlushMenuThemes = void(WINAPI *)();

    struct UxThemeFunctions
    {
        fnAllowDarkModeForWindow pAllowDarkModeForWindow = nullptr;
        fnSetPreferredAppMode pSetPreferredAppMode = nullptr;
        fnFlushMenuThemes pFlushMenuThemes = nullptr;
        UxThemeFunctions()
        {
            HMODULE h = ::GetModuleHandleW(L"uxtheme.dll");
            if (!h)
                h = ::LoadLibraryExW(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
            if (!h)
                return;
            pAllowDarkModeForWindow = reinterpret_cast<fnAllowDarkModeForWindow>(::GetProcAddress(h, MAKEINTRESOURCEA(133)));
            pSetPreferredAppMode = reinterpret_cast<fnSetPreferredAppMode>(::GetProcAddress(h, MAKEINTRESOURCEA(135)));
            pFlushMenuThemes = reinterpret_cast<fnFlushMenuThemes>(::GetProcAddress(h, MAKEINTRESOURCEA(136)));
        }
    };
    static UxThemeFunctions &UxTheme()
    {
        static UxThemeFunctions s;
        return s;
    }

    struct SwcaFunctions
    {
        fnSetWindowCompositionAttribute pSetWindowCompositionAttribute = nullptr;
        SwcaFunctions()
        {
            HMODULE h = ::GetModuleHandleW(L"user32.dll");
            if (!h)
                return;
            pSetWindowCompositionAttribute =
                reinterpret_cast<fnSetWindowCompositionAttribute>(
                    ::GetProcAddress(h, "SetWindowCompositionAttribute"));
        }
    };
    static SwcaFunctions &SwcaFn()
    {
        static SwcaFunctions s;
        return s;
    }

    // ── Brush management ──────────────────────────────────────────────────────
    void CreateBrushes()
    {
        m_brBackground = ::CreateSolidBrush(m_pal.Background);
        m_brSurface = ::CreateSolidBrush(m_pal.Surface);
        m_brInput = ::CreateSolidBrush(m_pal.InputBg);
        m_brControl = ::CreateSolidBrush(m_pal.ControlFill);
        m_brMenuHot = ::CreateSolidBrush(m_pal.MenuItemHot);
    }
    void DestroyBrushes()
    {
        auto del = [](HBRUSH &b)
        { if (b) { ::DeleteObject(b); b = nullptr; } };
        del(m_brBackground);
        del(m_brSurface);
        del(m_brInput);
        del(m_brControl);
        del(m_brMenuHot);
    }

    bool m_isDark = true; // set by constructor and Refresh()
    PaletteP m_pal{};     // runtime color palette

    HBRUSH m_brBackground = nullptr;
    HBRUSH m_brSurface = nullptr;
    HBRUSH m_brInput = nullptr;
    HBRUSH m_brControl = nullptr;
    HBRUSH m_brMenuHot = nullptr;
};
