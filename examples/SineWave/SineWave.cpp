/*
	WinTest, Small Demo application that creates a user interface window.
	release build - compile with :
		winres -i wintest.rc -o wintestrc.o
		g++ wintest.cpp wintestrc.o -municode -mwindows -s -O3 -o wintest.exe
	debug build - compile with :
		winres -i wintest.rc -o wintestrc.o
		g++ wintest.cpp wintestrc.o -municode -mwindows -g -o wintest.exe
*/

#include "SineWave.h"
#include <format>
#include <d2derr.h>
#include <cmath>
#include <numbers>
#include <WinUtility/Numbers.h>

void MyWindow::Render(const PAINTSTRUCT &ps)
{
	HRESULT hr = S_OK;
	if (!m_renderTarget)
	{
		hr = CreateDeviceResources();
	}
	Rect rc;
	GetClientRect(&rc);
	m_renderTarget->BindDC(ps.hdc, &rc);
	m_renderTarget->BeginDraw();
	m_renderTarget->DrawLine(
		D2D1::Point2F(0.0f, rc.Height() / 2.0f),
		D2D1::Point2F(convert_to<float>(rc.Width()), convert_to<float>(rc.Height() / 2.0f)),
		m_blackBrush.Get(),
		0.5f);
	m_renderTarget->DrawLine(
		D2D1::Point2F(rc.Width() / 2.0f, convert_to<float>(rc.top)),
		D2D1::Point2F(rc.Width() / 2.0f, convert_to<float>(rc.bottom)),
		m_blackBrush.Get(), 0.5f);

	m_renderTarget->DrawGeometry(m_path.Get(), m_sinewaveBrush.Get(), 1.0f);
	m_renderTarget->Flush();
	hr = m_renderTarget->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
	{
		auto target = m_renderTarget.ReleaseAndGetAddressOf();
		target = nullptr;
		auto brush = m_blackBrush.ReleaseAndGetAddressOf();
		brush = nullptr;
		auto sinbrush = m_sinewaveBrush.ReleaseAndGetAddressOf();
		sinbrush = nullptr;
		hr = CreateDeviceResources();
		if (!SUCCEEDED(hr))
			HR(hr);
	}
}

HRESULT MyWindow::CreateDeviceIndependantResources()
{
	HRESULT hr = S_OK;
	if (!m_factory)
	{
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_factory.GetAddressOf());
		if (SUCCEEDED(hr))
		{

			hr = m_factory->CreatePathGeometry(m_path.GetAddressOf());
		}
	}
	return hr;
}

HRESULT MyWindow::CreateDeviceResources()
{
	HRESULT hr = S_OK;
	if (!m_renderTarget)
	{
		D2D1_RENDER_TARGET_PROPERTIES properties = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(
				DXGI_FORMAT_B8G8R8A8_UNORM,
				D2D1_ALPHA_MODE_PREMULTIPLIED),
			0,
			0,
			D2D1_RENDER_TARGET_USAGE_NONE,
			D2D1_FEATURE_LEVEL_DEFAULT);

		hr = m_factory->CreateDCRenderTarget(&properties, m_renderTarget.GetAddressOf());
		if (SUCCEEDED(hr))
		{
			hr = m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black, 0.8f), m_blackBrush.GetAddressOf());
			if (SUCCEEDED(hr))
			{
				hr = m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), m_sinewaveBrush.GetAddressOf());
			}
		}
	}
	return hr;
}

HRESULT MyWindow::Initialize()
{
	HRESULT hr = S_OK;
	hr = CreateDeviceIndependantResources();
	if (SUCCEEDED(hr))
	{
		// get app title from resource file.
		std::tstring apptitle;
		apptitle.resize(256);
		LoadString(HINST_THISCOMPONENT, IDS_APP_TITLE, apptitle.data(), convert_to<int>(apptitle.length()));
		apptitle.shrink_to_fit();

		hr = ::IsWindow(Create(nullptr, &Window::rcDefault, apptitle.c_str())) ? S_OK : E_FAIL;
		if (SUCCEEDED(hr))
		{
			ShowWindow(ShowWindowType::Normal);
			UpdateWindow();
		}
	}
	return hr;
}

void MyWindow::OnPaint(DC dc)
{
	PAINTSTRUCT ps;
	BeginPaint(&ps);
	Render(ps);
	EndPaint(&ps);
}

void MyWindow::OnSize(UINT nType, Size size)
{
	Rect rc;
	GetClientRect(&rc);
	HRESULT hr = S_OK;

	if (!m_path)
	{
		hr = CreateDeviceIndependantResources();
	}
	else
	{
		auto path = m_path.ReleaseAndGetAddressOf();
		path = nullptr;
		hr = m_factory->CreatePathGeometry(m_path.GetAddressOf());
		if (m_sink)
		{
			auto sink = m_sink.ReleaseAndGetAddressOf();
			sink = nullptr;
		}
	}

	if (SUCCEEDED(hr))
	{
		hr = m_path->Open(m_sink.GetAddressOf());
		if (SUCCEEDED(hr))
		{
			m_sink->BeginFigure(D2D1::Point2F(convert_to<float>(rc.left), convert_to<float>(rc.Height() / 2)),
								D2D1_FIGURE_BEGIN_HOLLOW);
			auto numsegments = 10000;
			auto twopi = 2.0 * std::numbers::pi;
			if (!m_linepoints.empty())
			{
				for (int i = 0; i < numsegments; i++)
				{
					try
					{
						auto x = static_cast<float>(i * rc.Width() / numsegments);
						auto y = static_cast<float>(rc.Height() / 2 * (1 - std::sin(twopi * i / numsegments)));
						m_linepoints[i].x = x;
						m_linepoints[i].y = y;
					}
					catch(const Bad_value&)
					{
					}
				}
			}
			else
			{
				for (int i = 0; i < numsegments; i++)
				{
					try
					{
						auto x = static_cast<float>(i * rc.Width() / numsegments);
						auto y = static_cast<float>(rc.Height() / 2 * (1 - std::sin(twopi * i / numsegments)));
						m_linepoints.emplace_back(x, y);
					}
					catch(const Bad_value&)
					{
					}
				}
			}
			m_sink->AddLines(m_linepoints.data(), convert_to<int>(m_linepoints.size()));
			m_sink->EndFigure(D2D1_FIGURE_END_OPEN);
			hr = m_sink->Close();
		}
	}
	if (SUCCEEDED(hr))
	{
		Invalidate();
		UpdateWindow();
	}
}

void MyWindow::ReportError(HRESULT hr)
{
	switch (hr)
	{
	case D2DERR_WRONG_STATE:
		std::cout << "D2DERR_WRONG_STATE" << std::endl;
		break;
	case D2DERR_NOT_INITIALIZED:
		std::cout << "D2DERR_NOT_INITIALIZED" << std::endl;
		break;
	case D2DERR_UNSUPPORTED_OPERATION:
		std::cout << "D2DERR_UNSUPPORTED_OPERATION" << std::endl;
		break;
	case D2DERR_SCANNER_FAILED:
		std::cout << "D2DERR_SCANNER_FAILED" << std::endl;
		break;
	case D2DERR_SCREEN_ACCESS_DENIED:
		std::cout << "D2DERR_SCREEN_ACCESS_DENIED" << std::endl;
		break;
	case D2DERR_DISPLAY_STATE_INVALID:
		std::cout << "D2DERR_DISPLAY_STATE_INVALID" << std::endl;
		break;
	case D2DERR_ZERO_VECTOR:
		std::cout << "D2DERR_ZERO_VECTOR" << std::endl;
		break;
	case D2DERR_INTERNAL_ERROR:
		std::cout << "D2DERR_INTERNAL_ERROR" << std::endl;
		break;
	case D2DERR_DISPLAY_FORMAT_NOT_SUPPORTED:
		std::cout << "D2DERR_DISPLAY_FORMAT_NOT_SUPPORTED" << std::endl;
		break;
	case D2DERR_INVALID_CALL:
		std::cout << "D2DERR_INVALID_CALL" << std::endl;
		break;
	case D2DERR_NO_HARDWARE_DEVICE:
		std::cout << "D2DERR_NO_HARDWARE_DEVICE" << std::endl;
		break;
	case D2DERR_RECREATE_TARGET:
		std::cout << "D2DERR_RECREATE_TARGET" << std::endl;
		break;
	case D2DERR_TOO_MANY_SHADER_ELEMENTS:
		std::cout << "D2DERR_TOO_MANY_SHADER_ELEMENTS" << std::endl;
		break;
	case D2DERR_SHADER_COMPILE_FAILED:
		std::cout << "D2DERR_SHADER_COMPILE_FAILED" << std::endl;
		break;
	case D2DERR_MAX_TEXTURE_SIZE_EXCEEDED:
		std::cout << "D2DERR_MAX_TEXTURE_SIZE_EXCEEDED" << std::endl;
		break;
	case D2DERR_UNSUPPORTED_VERSION:
		std::cout << "D2DERR_UNSUPPORTED_VERSION" << std::endl;
		break;
	case D2DERR_BAD_NUMBER:
		std::cout << "D2DERR_BAD_NUMBER" << std::endl;
		break;
	case D2DERR_WRONG_FACTORY:
		std::cout << "D2DERR_WRONG_FACTORY" << std::endl;
		break;
	case D2DERR_LAYER_ALREADY_IN_USE:
		std::cout << "D2DERR_LAYER_ALREADY_IN_USE" << std::endl;
		break;
	case D2DERR_POP_CALL_DID_NOT_MATCH_PUSH:
		std::cout << "D2DERR_POP_CALL_DID_NOT_MATCH_PUSH" << std::endl;
		break;
	case D2DERR_WRONG_RESOURCE_DOMAIN:
		std::cout << "D2DERR_WRONG_RESOURCE_DOMAIN" << std::endl;
		break;
	case D2DERR_PUSH_POP_UNBALANCED:
		std::cout << "D2DERR_PUSH_POP_UNBALANCED" << std::endl;
		break;
	case D2DERR_RENDER_TARGET_HAS_LAYER_OR_CLIPRECT:
		std::cout << "D2DERR_RENDER_TARGET_HAS_LAYER_OR_CLIPRECT" << std::endl;
		break;
	case D2DERR_INCOMPATIBLE_BRUSH_TYPES:
		std::cout << "D2DERR_INCOMPATIBLE_BRUSH_TYPES" << std::endl;
		break;
	case D2DERR_WIN32_ERROR:
		std::cout << "D2DERR_WIN32_ERROR" << std::endl;
		break;
	case D2DERR_TARGET_NOT_GDI_COMPATIBLE:
		std::cout << "D2DERR_TARGET_NOT_GDI_COMPATIBLE" << std::endl;
		break;
	case D2DERR_TEXT_EFFECT_IS_WRONG_TYPE:
		std::cout << "D2DERR_TEXT_EFFECT_IS_WRONG_TYPE" << std::endl;
		break;
	case D2DERR_TEXT_RENDERER_NOT_RELEASED:
		std::cout << "D2DERR_TEXT_RENDERER_NOT_RELEASED" << std::endl;
		break;
	case D2DERR_EXCEEDS_MAX_BITMAP_SIZE:
		std::cout << "D2DERR_EXCEEDS_MAX_BITMAP_SIZE" << std::endl;
		break;
	case D2DERR_INVALID_GRAPH_CONFIGURATION:
		std::cout << "D2DERR_INVALID_GRAPH_CONFIGURATION" << std::endl;
		break;
	case D2DERR_INVALID_INTERNAL_GRAPH_CONFIGURATION:
		std::cout << "D2DERR_INVALID_INTERNAL_GRAPH_CONFIGURATION" << std::endl;
		break;
	case D2DERR_CYCLIC_GRAPH:
		std::cout << "D2DERR_CYCLIC_GRAPH" << std::endl;
		break;
	case D2DERR_BITMAP_CANNOT_DRAW:
		std::cout << "D2DERR_BITMAP_CANNOT_DRAW" << std::endl;
		break;
	case D2DERR_OUTSTANDING_BITMAP_REFERENCES:
		std::cout << "D2DERR_OUTSTANDING_BITMAP_REFERENCES" << std::endl;
		break;
	case D2DERR_ORIGINAL_TARGET_NOT_BOUND:
		std::cout << "D2DERR_ORIGINAL_TARGET_NOT_BOUND" << std::endl;
		break;
	case D2DERR_INVALID_TARGET:
		std::cout << "D2DERR_INVALID_TARGET" << std::endl;
		break;
	case D2DERR_BITMAP_BOUND_AS_TARGET:
		std::cout << "D2DERR_BITMAP_BOUND_AS_TARGET" << std::endl;
		break;
	case D2DERR_INSUFFICIENT_DEVICE_CAPABILITIES:
		std::cout << "D2DERR_INSUFFICIENT_DEVICE_CAPABILITIES" << std::endl;
		break;
	case D2DERR_INTERMEDIATE_TOO_LARGE:
		std::cout << "D2DERR_INTERMEDIATE_TOO_LARGE" << std::endl;
		break;
	case D2DERR_EFFECT_IS_NOT_REGISTERED:
		std::cout << "D2DERR_EFFECT_IS_NOT_REGISTERED" << std::endl;
		break;
	case D2DERR_INVALID_PROPERTY:
		std::cout << "D2DERR_INVALID_PROPERTY" << std::endl;
		break;
	case D2DERR_NO_SUBPROPERTIES:
		std::cout << "D2DERR_NO_SUBPROPERTIES" << std::endl;
		break;
	case D2DERR_PRINT_JOB_CLOSED:
		std::cout << "D2DERR_PRINT_JOB_CLOSED" << std::endl;
		break;
	case D2DERR_PRINT_FORMAT_NOT_SUPPORTED:
		std::cout << "D2DERR_PRINT_FORMAT_NOT_SUPPORTED" << std::endl;
		break;
	case D2DERR_TOO_MANY_TRANSFORM_INPUTS:
		std::cout << "D2DERR_TOO_MANY_TRANSFORM_INPUTS" << std::endl;
		break;
	case D2DERR_INVALID_GLYPH_IMAGE:
		std::cout << "D2DERR_INVALID_GLYPH_IMAGE" << std::endl;
		break;
	default:
		std::cout << std::format("Not Knwon {:X}", hr) << std::endl;
		break;
	};
}

void MyWindow::OnExit(UINT uNotifyCode, int nID, Window wndCtl)
{
	OnClose();
}

void MyWindow::OnAbout(UINT uNotifyCode, int nID, Window wndCtl)
{
	if (!about)
	{
		about.DoModal();
	}
}

void MyWindow::OnClose()
{
	PostQuitMessage(0);
}

int MyWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// additional window initalization would go here.
	return 0;
}
