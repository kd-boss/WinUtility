/*
    D2D1Test, Small Demo application that creates a user interface window rendered with Direct2D.
    release build - compile with :
        g++ D2D1Test.cpp -std=c++11 -municode -mwindows -static -s -O3 -o d2d1test.exe -ld2d1
    debug build - compile with :
        g++ D2D1Test.cpp -std=c++11 -municode -mwindows  -static -g  d2d1test.exe -ld2d1
*/
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>


#include <utility/System.h>
#include <utility/BaseWindow.h>
#include <utility/ComPtr.h>
#include <utility/System.Utility.h>



class MyWindow : public BaseWindow<MyWindow, Window, FrameWinTraits> {
public:
  DECLARE_WND_CLASS(TEXT("D2D1Test"))
  ComPtr<ID2D1Factory> m_factory;
  ComPtr<ID2D1HwndRenderTarget> m_renderTarget;
  ComPtr<ID2D1SolidColorBrush> m_lightSlateGrayBrush;
  ComPtr<ID2D1SolidColorBrush> m_cornFlowerBlueBrush;
  
  HRESULT CreateDeviceIndependentResources();
  HRESULT CreateDeviceDependentResources();
  
  D2D1_SIZE_F rtSize;
  
  void OnClose();
  void OnSize(UINT nType, Size size);
  void OnDisplayChange(UINT uBitsPerPixel, Size sizeScreen);
  void OnPaint(DC dc);
  int OnCreate(LPCREATESTRUCT lpCreateStruct);
  
  BEGIN_MSG_MAP()
  MSG_WM_CREATE(OnCreate)
  MSG_WM_CLOSE(OnClose)
  MSG_WM_SIZE(OnSize)
  MSG_WM_DISPLAYCHANGE(OnDisplayChange)
  MSG_WM_PAINT(OnPaint)
  END_MSG_MAP()
};

void MyWindow::OnClose()
{
	m_lightSlateGrayBrush.Reset();
	m_cornFlowerBlueBrush.Reset();
	m_renderTarget.Reset();
	PostQuitMessage(0);
}

int MyWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	HR(CreateDeviceIndependentResources());
	return 0;
}


HRESULT MyWindow::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,m_factory.GetAddressOf());
	return hr;
}

HRESULT MyWindow::CreateDeviceDependentResources()
{
	
	HRESULT hr = S_OK;
	if(!m_renderTarget)
	{
		RECT rc;
		GetClientRect(&rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
		try{
			
			
		HR(m_factory->CreateHwndRenderTarget( D2D1::RenderTargetProperties(),
					D2D1::HwndRenderTargetProperties(m_hwnd, size),
					&m_renderTarget));
					
		HR(m_renderTarget->CreateSolidColorBrush(
						D2D1::ColorF(D2D1::ColorF::LightSlateGray),
						&m_lightSlateGrayBrush
						));
		
		HR(m_renderTarget->CreateSolidColorBrush(
						D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
						&m_cornFlowerBlueBrush
						));
		
		m_renderTarget->SetDpi(96.0f , 96.0f );
		}
		catch(std::exception ex)
		{
			TRACE(ex.what());
		}
	}
	return hr;
}

void MyWindow::OnSize(UINT nType, Size size)
{
	try
	{
	if (m_renderTarget)
	{
		// Note: This method can fail, but it's okay to ignore the
		// error here, because the error will be returned again
		// the next time EndDraw is called.
		auto sz = m_renderTarget->GetSize();
		HR(m_renderTarget->Resize(D2D1::SizeU(size.cx, size.cy)));
		
	}
	rtSize = D2D1::SizeF(size.cx, size.cy);
	}
	catch(std::exception ex)
	{
		::MessageBoxW(NULL,System::to_wstring(ex.what()).c_str(), L"D2D1Test", MB_OK);
	}
}

void MyWindow::OnDisplayChange(UINT uBitsPerPixel, Size sizeScreen)
{
	Invalidate(false);
}

void MyWindow::OnPaint(DC dc)
{
	try
	{
		HRESULT hr = S_OK;
		HR(CreateDeviceDependentResources());
		
		m_renderTarget->BeginDraw();
		m_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		m_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
			
		int width = static_cast<int>(rtSize.width);
		int height = static_cast<int>(rtSize.height);

		for (int x = 0; x < width; x += 10)
		{
			m_renderTarget->DrawLine(
				D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
				D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height),
				m_lightSlateGrayBrush.Get(),
				0.5f
				);
		}
		
		for (int y = 0; y < height; y += 10)
		{
			m_renderTarget->DrawLine(
				D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
				D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
				m_lightSlateGrayBrush.Get(),
				0.5f
				);
		}

		D2D1_RECT_F rectangle1 = D2D1::RectF(
					rtSize.width/2 - 50.0f,
					rtSize.height/2 - 50.0f,
					rtSize.width/2 + 50.0f,
					rtSize.height/2 + 50.0f
					);

		D2D1_RECT_F rectangle2 = D2D1::RectF(
					rtSize.width/2 - 100.0f,
					rtSize.height/2 - 100.0f,
					rtSize.width/2 + 100.0f,
					rtSize.height/2 + 100.0f
					);  
					
		m_renderTarget->FillRectangle(&rectangle1, m_lightSlateGrayBrush.Get());
		m_renderTarget->DrawRectangle(&rectangle2, m_cornFlowerBlueBrush.Get());
		
		hr = m_renderTarget->EndDraw();
		
		if(hr == D2DERR_RECREATE_TARGET)
		{
			m_lightSlateGrayBrush.Reset();
			m_cornFlowerBlueBrush.Reset();
			m_renderTarget.Reset();
		}
	}
	catch(std::exception ex)
	{
		TRACE(ex.what());
	}
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPTSTR, int nShow) {
  MyWindow win;
  win.Create(nullptr,&Window::rcDefault,TEXT("D2D1 Test"));
  win.ShowWindow(nShow);
  win.UpdateWindow();

  MSG msg;
  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}
