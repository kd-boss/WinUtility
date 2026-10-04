#include "ContainedEditTest.h"

int ContainedEditTestWindow::OnCreate([[maybe_unused]] LPCREATESTRUCT lpCreateStruct)
{
    Rect rect{0, 0, 0, 0};

    if (!m_edit.Create(*this, rect, nullptr,
                       WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_LEFT | ES_AUTOHSCROLL,
                       WS_EX_CLIENTEDGE, 1001, nullptr))
    {
        return -1;
    }

    m_edit.SetFocus();
    m_super.SetOwner(*this);
    m_super.SubClassWindow(m_edit);
    return 0;
}

void ContainedEditTestWindow::OnSize([[maybe_unused]] UINT nType, Size size)
{
    if (m_edit.IsWindow())
        m_edit.MoveWindow(0, 0, size.cx, size.cy);
}

void ContainedEditTestWindow::OnDestroy()
{
    PostQuitMessage(0);
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int nShow)
{
    ContainedEditTestWindow window;

    if (!window.Create(nullptr, &Window::rcDefault, TEXT("WinUtility Contained Edit Test")))
        return 1;

    window.ShowWindow(nShow);
    window.UpdateWindow();

    MSG message{};
    while (GetMessage(&message, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return static_cast<int>(message.wParam);
}
