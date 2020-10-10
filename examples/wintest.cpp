/*
    WinTest, Small Demo application that creates a user interface window.
    release build - compile with :
        g++ wintest.cpp -std=c++11 -municode -mwindows -static -s -O3 -o
   wintest.exe
    debug build - compile with :
        g++ wintest.cpp -std=c++11 -municode -mwindows  -static -g  wintest.exe
*/
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <utility/BaseWindow.h>
#include <utility/ComPtr.h>


class MyWindow : public BaseWindow<MyWindow, Window, FrameWinTraits> {
public:
  DECLARE_WND_CLASS(TEXT("TestWindow"))

  void OnClose() ;
  void OnLButtonDown(UINT nFlags,const Point& pt);
  
public:
  BEGIN_MSG_MAP()
  MSG_WM_CLOSE(OnClose)
  MSG_WM_LBUTTONDOWN(OnLButtonDown)
  END_MSG_MAP()
};

void MyWindow::OnClose(){
	PostQuitMessage(0);
}

void MyWindow::OnLButtonDown(UINT nFlags,const Point& pt){
	std::tstringstream str;
	str << TEXT("Clicked At: ") << pt.x << TEXT(",") << pt.y;
	MessageBox(str.str().c_str(),TEXT("Mouse Position"), MB_OK);	
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPTSTR, int nShow) {
  MyWindow win;
  win.Create(nullptr,&Window::rcDefault,TEXT("Test Win"));
  win.ShowWindow(nShow);
  win.UpdateWindow();

  MSG msg;
  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}
