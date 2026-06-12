// main.cpp  —  com0com Manager entry point
//
// Build (MSVC, Unicode, /std:c++23):
//   cl /std:c++23 /EHsc /O2 /DUNICODE /D_UNICODE
//      main.cpp MainWindow.cpp Com0ComDriver.cpp
//      /link setupapi.lib comctl32.lib shlwapi.lib user32.lib gdi32.lib
//            advapi32.lib shell32.lib
//
// The application requires elevation for any operation that creates, removes,
// or renames pairs (setupc.exe writes to HKLM and the driver device tree).
// Add an application manifest with requestedExecutionLevel = requireAdministrator
// for production use.  During development you can right-click and
// "Run as administrator".

#include "MainWindow.hpp"


int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/,
                     LPTSTR /*lpCmdLine*/, int nCmdShow)
{
    // Enable DPI awareness (call before any window creation)
    ::SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    // Initialise Common Controls
    INITCOMMONCONTROLSEX icex{};
    icex.dwSize = sizeof(icex);
    icex.dwICC  = ICC_LISTVIEW_CLASSES | ICC_BAR_CLASSES | ICC_STANDARD_CLASSES;
    ::InitCommonControlsEx(&icex);

    // Create and show the main window
    MainWindow wnd;

    RECT rc = { CW_USEDEFAULT, CW_USEDEFAULT,
                CW_USEDEFAULT + 640, CW_USEDEFAULT + 460 };

    HWND hWnd = wnd.Create(
        nullptr,            // no parent
        &rc,
        TEXT("com0com Manager"),
        WS_OVERLAPPEDWINDOW,
        WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

    if (!wnd.IsWindow()) {
        ::MessageBox(nullptr,
                     TEXT("Failed to create main window."),
                     TEXT("com0com Manager"),
                     MB_OK | MB_ICONERROR);
        return 1;
    }
    else
    {
        wnd.BuildMenu();
        wnd.BuildListView();
    }

    wnd.ShowWindow(nCmdShow);
    wnd.Invalidate();

    // Standard Win32 message loop
    MSG msg{};
    while (::GetMessage(&msg, nullptr, 0, 0)) {
        // IsDialogMessage allows keyboard navigation (Tab, Enter, etc.)
        // between controls even in our non-dialog frame window.
        if (!::IsDialogMessage(hWnd, &msg)) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }

    return static_cast<int>(msg.wParam);
}
