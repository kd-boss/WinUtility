/*
    WinTest, Small Demo application that creates a user interface window.
    release build - compile with :
        g++ wintest.cpp -std=c++11 -municode -mwindows -static -s -O3 -o wintest.exe
    debug build - compile with :
        g++ wintest.cpp -std=c++11 -municode -mwindows  -static -g  wintest.exe
*/


#include <utility/BaseWindow.h>
#include <utility/ComPtr.h>
#include <d2d1.h>

class MyWindow : public BaseWindow<MyWindow,FrameWinTraits>{

void OnClose();

public:
BEGIN_MSG_MAP()
    MSG_WM_CLOSE(OnClose)
END_MSG_MAP()

PCTSTR ClassName() const;
};

PCTSTR MyWindow::ClassName() const{
    return const_cast<WCHAR*>(L"TestWindow");
}

void MyWindow::OnClose()
{
    PostQuitMessage(0);
}

int WINAPI wWinMain(HINSTANCE,HINSTANCE, LPTSTR,int nShow){
    MyWindow win;
    win.Create(L"Test Win");
    win.Show(nShow);
    win.UpdateWindow();
    
    MSG msg;
    while(GetMessage(&msg,nullptr,0,0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

