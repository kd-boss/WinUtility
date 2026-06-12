// main.cpp  —  com0com Manager entry point

#include "MainWindow.hpp"

#ifdef _MSC_VER
#pragma comment(linker,                                        \
                "\"/manifestdependency:type='win32' "          \
                "name='Microsoft.Windows.Common-Controls' "    \
                "version='6.0.0.0' processorArchitecture='*' " \
                "publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/,
                    LPTSTR /*lpCmdLine*/, int nCmdShow)
{
    // ── Dark mode process registration ───────────────────────────────────────
    // MUST be the first call — before InitCommonControlsEx, before any window
    // is created, and before any menu exists.
    // Calls uxtheme.dll ordinal 135 (SetPreferredAppMode = AllowDark) so
    // that Win32 menus render dark when the system theme is dark.
    DarkTheme::InitProcess();

    // ── DPI awareness ────────────────────────────────────────────────────────
    ::SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    // ── Common controls — called ONCE here, never in any OnCreate ────────────
    INITCOMMONCONTROLSEX icex{};
    icex.dwSize = sizeof(icex);
    icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_BAR_CLASSES | ICC_STANDARD_CLASSES | ICC_LINK_CLASS | ICC_WIN95_CLASSES;
    ::InitCommonControlsEx(&icex);

    MainWindow wnd;

    RECT rc{CW_USEDEFAULT, CW_USEDEFAULT,
            CW_USEDEFAULT + 640, CW_USEDEFAULT + 460};

    HWND hWnd = wnd.Create(
        nullptr, &rc,
        TEXT("com0com Manager"),
        WS_OVERLAPPEDWINDOW,
        WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

    if (!hWnd)
    {
        ::MessageBox(nullptr, TEXT("Failed to create main window."),
                     TEXT("com0com Manager"), MB_OK | MB_ICONERROR);
        return 1;
    }

    wnd.ShowWindow(nCmdShow);
    wnd.UpdateWindow();

    MSG msg{};
    while (::GetMessage(&msg, nullptr, 0, 0))
    {
        if (!::IsDialogMessage(hWnd, &msg))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }

    return static_cast<int>(msg.wParam);
}
