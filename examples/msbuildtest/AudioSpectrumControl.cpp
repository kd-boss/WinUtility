// =============================================================================
//  AudioSpectrumControl.cpp
// =============================================================================
#include "AudioSpectrumControl.h"
#include <functiondiscoverykeys_devpkey.h>
#include <cstring>
#include <algorithm>
#include <cassert>
#include <format>

// ---------------------------------------------------------------------------
//  Helpers
// ---------------------------------------------------------------------------
static inline float clampf(float v, float lo, float hi)
{
    return v < lo ? lo : (v > hi ? hi : v);
}

// Safe WASAPI release helper
template<class T>
static void SafeRelease(T*& p) { if (p) { p->Release(); p = nullptr; } }

void AudioSpectrumControl::SetFactory(ID2D1Factory* pft)
{
    if(!m_d2dFactory)
        m_d2dFactory.Attach(pft);
}
// ===========================================================================
//  Static: enumerate WASAPI capture devices
// ===========================================================================
std::vector<std::wstring> AudioSpectrumControl::GetAudioInputDevices()
{
    std::vector<std::wstring> names;

    IMMDeviceEnumerator* pEnum = nullptr;
    HRESULT hr = ::CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr,
                                    CLSCTX_ALL, IID_PPV_ARGS(&pEnum));
    if (FAILED(hr) || !pEnum) return names;

    IMMDeviceCollection* pColl = nullptr;
    hr = pEnum->EnumAudioEndpoints(eCapture, DEVICE_STATE_ACTIVE, &pColl);
    pEnum->Release();
    if (FAILED(hr) || !pColl) return names;

    UINT count = 0;
    pColl->GetCount(&count);

    for (UINT i = 0; i < count; ++i)
    {
        IMMDevice* pDev = nullptr;
        if (FAILED(pColl->Item(i, &pDev)) || !pDev) continue;

        IPropertyStore* pStore = nullptr;
        if (SUCCEEDED(pDev->OpenPropertyStore(STGM_READ, &pStore)) && pStore)
        {
            PROPVARIANT pv;
            PropVariantInit(&pv);
            if (SUCCEEDED(pStore->GetValue(PKEY_Device_FriendlyName, &pv))
                && pv.vt == VT_LPWSTR && pv.pwszVal)
            {
                names.push_back(pv.pwszVal);
            }
            PropVariantClear(&pv);
            pStore->Release();
        }
        pDev->Release();
    }
    pColl->Release();
    return names;
}

// ===========================================================================
//  Public filter setters  (all just store atomics and invalidate)
// ===========================================================================
void AudioSpectrumControl::EnableNotch(bool e)        { m_notchEnabled = e;  ::InvalidateRect(m_hwnd,nullptr,FALSE); }
void AudioSpectrumControl::SetNotchFrequency(float f) { m_notchFreqHz  = f;  ::InvalidateRect(m_hwnd,nullptr,FALSE); }
void AudioSpectrumControl::SetNotchWidth(float w)     { m_notchWidthHz = w;  ::InvalidateRect(m_hwnd,nullptr,FALSE); }

void AudioSpectrumControl::EnableContour(bool e)        { m_contourEnabled  = e; ::InvalidateRect(m_hwnd,nullptr,FALSE); }
void AudioSpectrumControl::SetContourFrequency(float f) { m_contourFreqHz   = f; ::InvalidateRect(m_hwnd,nullptr,FALSE); }
void AudioSpectrumControl::SetContourWidth(float w)     { m_contourWidthHz  = w; ::InvalidateRect(m_hwnd,nullptr,FALSE); }
void AudioSpectrumControl::SetContourDepth(float d)     { m_contourDepthDb  = d; ::InvalidateRect(m_hwnd,nullptr,FALSE); }

// ===========================================================================
//  SetDevice
// ===========================================================================
void AudioSpectrumControl::SetDevice(int deviceIndex)
{
    StopCapture();
    if (deviceIndex >= 0)
        StartCapture(deviceIndex);
}

// ===========================================================================
//  Message handlers
// ===========================================================================

int AudioSpectrumControl::OnCreate(LPCREATESTRUCT /*lpcs*/)
{

    HRESULT hr = S_OK;
    // Create the D2D factory (device-independent)
    if(!m_d2dFactory) { hr = ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
                                     m_d2dFactory.GetAddressOf());}
    if (FAILED(hr)) return -1;

    // Create the DWrite factory
    hr = ::DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
                               __uuidof(IDWriteFactory),
                               reinterpret_cast<IUnknown**>(m_dwFactory.GetAddressOf()));
    if (FAILED(hr)) return -1;

    // Text format for axis labels (8 pt Segoe UI)
    hr = m_dwFactory->CreateTextFormat(
        L"Segoe UI", nullptr,
        DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
        8.0f * 96.0f / 72.0f, L"en-US",
        m_textFmt.GetAddressOf());
    if (FAILED(hr)) return -1;

    // Initialise spectrum / peak-hold buffers
    m_magnitudeDb.assign(FFT_SIZE / 2, DB_FLOOR);
    m_peakHold   .assign(FFT_SIZE / 2, DB_FLOOR);

    // Ring buffer (4× FFT_SIZE gives a comfortable capture margin)
    m_ringSize = static_cast<size_t>(FFT_SIZE) * 4;
    m_ringBuffer.assign(m_ringSize, 0.f);
    m_ringWrite = 0;

    // Start the redraw timer
    ::SetTimer(m_hwnd, REDRAW_TIMER_ID, REDRAW_INTERVAL_MS, nullptr);

    return 0; // 0 = success
}

void AudioSpectrumControl::OnDestroy()
{
    ::KillTimer(m_hwnd, REDRAW_TIMER_ID);
    StopCapture();
    DiscardDeviceResources();
    m_d2dFactory.Detach();
    m_dwFactory.Reset();
    m_textFmt.Reset();
}

void AudioSpectrumControl::OnPaint(HDC /*hdc*/)
{
    PAINTSTRUCT ps;
    ::BeginPaint(m_hwnd, &ps);
    if (FAILED(CreateDeviceResources()))
    {
        ::EndPaint(m_hwnd, &ps);
        return;
    }
    Render();
    if (m_renderTarget->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED)
        DiscardDeviceResources();
    ::EndPaint(m_hwnd, &ps);
}

void AudioSpectrumControl::OnSize(UINT /*nType*/, Size sz)
{
    m_clientW = static_cast<float>(sz.cx);
    m_clientH = static_cast<float>(sz.cy);
    m_specLeft = ML;
    m_specTop  = MT;
    m_specW    = m_clientW - ML - MR;
    m_specH    = m_clientH - MT - MB;

    if (m_renderTarget)
    {
        D2D1_SIZE_U s{ static_cast<UINT32>(sz.cx), static_cast<UINT32>(sz.cy) };
        m_renderTarget->Resize(s);
    }
}

void AudioSpectrumControl::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent != REDRAW_TIMER_ID) return;

    // ---- Grab the latest audio from the ring buffer ----
    {
        std::lock_guard<std::mutex> lk(m_bufMutex);
        if (m_ringBuffer.empty()) return;

        // Copy FFT_SIZE samples ending at the current write head
        std::vector<float> frame(FFT_SIZE, 0.f);
        size_t src = (m_ringWrite + m_ringSize - FFT_SIZE) % m_ringSize;
        for (int i = 0; i < FFT_SIZE; ++i)
        {
            frame[i] = m_ringBuffer[src];
            src = (src + 1) % m_ringSize;
        }

        // Window + FFT
        ApplyHannWindow(frame);
        std::vector<std::complex<float>> fftBuf(FFT_SIZE);
        for (int i = 0; i < FFT_SIZE; ++i)
            fftBuf[i] = frame[i];
        FFT(fftBuf);

        // Convert to dBFS magnitude, update peak-hold
        const float norm = 2.f / static_cast<float>(FFT_SIZE);
        const int N2 = FFT_SIZE / 2;
        for (int k = 0; k < N2; ++k)
        {
            float mag = std::abs(fftBuf[k]) * norm;
            float db  = (mag > 0.f) ? 20.f * std::log10f(mag) : DB_FLOOR;
            db = clampf(db, DB_FLOOR, DB_CEILING);
            m_magnitudeDb[k] = db;

            // Peak-hold: rise instantly, decay slowly
            if (db > m_peakHold[k])
                m_peakHold[k] = db;
            else
                m_peakHold[k] = m_peakHold[k] * PEAK_DECAY
                               + db * (1.f - PEAK_DECAY);
        }
    }

    ::InvalidateRect(m_hwnd, nullptr, FALSE);
}

BOOL AudioSpectrumControl::OnEraseBkgnd(HDC /*hdc*/)
{
    return TRUE; // suppress GDI erase; D2D handles the background
}

// ===========================================================================
//  Direct2D resource management
// ===========================================================================

HRESULT AudioSpectrumControl::CreateDeviceResources()
{
    if (m_renderTarget) return S_OK; // already created

    RECT rc;
    ::GetClientRect(m_hwnd, &rc);
    D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

    HRESULT hr = m_d2dFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(m_hwnd, size),
        m_renderTarget.GetAddressOf());
    if (FAILED(hr)) return hr;

    // Colours ----------------------------------------------------------------
    //   Background: very dark blue-grey (like a spectrum analyser display)
    //   Grid:       dim green lines
    //   Labels:     medium grey
    //   Spectrum fill: semi-transparent green gradient base colour
    //   Spectrum line: bright green
    //   Peak-hold:  amber
    //   Notch fill: semi-transparent red
    //   Notch edge: bright red
    //   Contour:    bright cyan
    //   Contour ref (0 dB line): dim cyan

    auto mkBrush = [&](D2D1_COLOR_F col,
                        Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>& br) -> HRESULT {
        return m_renderTarget->CreateSolidColorBrush(col, br.GetAddressOf());
    };

    if (FAILED(hr = mkBrush(D2D1::ColorF(0.07f, 0.10f, 0.15f, 1.f),  m_brBg)))        return hr;
    if (FAILED(hr = mkBrush(D2D1::ColorF(0.15f, 0.30f, 0.20f, 0.8f), m_brGrid)))      return hr;
    if (FAILED(hr = mkBrush(D2D1::ColorF(0.55f, 0.65f, 0.55f, 1.f),  m_brLabel)))     return hr;
    if (FAILED(hr = mkBrush(D2D1::ColorF(0.00f, 0.60f, 0.20f, 0.4f), m_brSpecFill)))  return hr;
    if (FAILED(hr = mkBrush(D2D1::ColorF(0.10f, 0.90f, 0.30f, 1.f),  m_brSpecLine)))  return hr;
    if (FAILED(hr = mkBrush(D2D1::ColorF(1.00f, 0.75f, 0.00f, 1.f),  m_brPeak)))      return hr;
    if (FAILED(hr = mkBrush(D2D1::ColorF(0.90f, 0.10f, 0.10f, 0.25f),m_brNotchFill))) return hr;
    if (FAILED(hr = mkBrush(D2D1::ColorF(1.00f, 0.20f, 0.20f, 0.85f),m_brNotchEdge))) return hr;
    if (FAILED(hr = mkBrush(D2D1::ColorF(0.00f, 0.90f, 1.00f, 0.90f),m_brContour)))   return hr;
    if (FAILED(hr = mkBrush(D2D1::ColorF(0.00f, 0.50f, 0.60f, 0.50f),m_brContourRef)))return hr;

    return S_OK;
}

void AudioSpectrumControl::DiscardDeviceResources()
{
    m_renderTarget.Reset();
    m_brBg.Reset();   m_brGrid.Reset();     m_brLabel.Reset();
    m_brSpecFill.Reset(); m_brSpecLine.Reset(); m_brPeak.Reset();
    m_brNotchFill.Reset();m_brNotchEdge.Reset();
    m_brContour.Reset();  m_brContourRef.Reset();
}

// ===========================================================================
//  Coordinate helpers
// ===========================================================================

// Logarithmic frequency axis: maps [100 Hz … Nyquist] to [0 … m_specW]
float AudioSpectrumControl::FreqToX(float hz) const
{
    const float nyq   = static_cast<float>(m_sampleRate.load()) * 0.5f;
    const float fLow  = 100.f;
    const float fHigh = nyq;
    if (hz <= fLow)  return m_specLeft;
    if (hz >= fHigh) return m_specLeft + m_specW;
    return m_specLeft + m_specW * (std::log10f(hz / fLow) / std::log10f(fHigh / fLow));
}

float AudioSpectrumControl::DbToY(float db) const
{
    float t = (db - DB_CEILING) / (DB_FLOOR - DB_CEILING); // 0 (top) … 1 (bottom)
    t = clampf(t, 0.f, 1.f);
    return m_specTop + t * m_specH;
}

float AudioSpectrumControl::XToFreq(float x) const
{
    const float nyq  = static_cast<float>(m_sampleRate.load()) * 0.5f;
    const float fLow = 100.f;
    float t = clampf((x - m_specLeft) / m_specW, 0.f, 1.f);
    return fLow * std::powf(nyq / fLow, t);
}

// ===========================================================================
//  Render
// ===========================================================================

void AudioSpectrumControl::Render()
{
    if (!m_renderTarget || m_specW < 1.f || m_specH < 1.f) return;

    // Snapshot filter params so they don't change mid-frame
    const bool  notchOn  = m_notchEnabled.load();
    const float nFreq    = m_notchFreqHz .load();
    const float nWidth   = m_notchWidthHz.load();
    const bool  contOn   = m_contourEnabled.load();
    const float cFreq    = m_contourFreqHz .load();
    const float cWidth   = m_contourWidthHz.load();
    const float cDepth   = m_contourDepthDb.load();
    const float nyq      = static_cast<float>(m_sampleRate.load()) * 0.5f;

    m_renderTarget->BeginDraw();
    m_renderTarget->Clear(D2D1::ColorF(0.07f, 0.10f, 0.15f));

    // ---- Clip to spectrum area for most drawing ----
    D2D1_RECT_F specRect = D2D1::RectF(m_specLeft, m_specTop,
                                        m_specLeft + m_specW,
                                        m_specTop  + m_specH);
    m_renderTarget->PushAxisAlignedClip(specRect, D2D1_ANTIALIAS_MODE_ALIASED);

    // =========================================================================
    //  1. Grid lines (dB verticle, Hz horizontal)
    // =========================================================================
    // verticle dB grid every 10 dB
    for (float db = DB_FLOOR; db <= DB_CEILING; db += 10.f)
    {
        float y = DbToY(db);
        m_renderTarget->DrawLine(
            D2D1::Point2F(m_specLeft, y),
            D2D1::Point2F(m_specLeft + m_specW, y),
            m_brGrid.Get(), 0.5f);
    }
    // Horizontal Hz grid (1/2/5 kHz decades)
    const float gridHz[] = { 200.f,500.f,1000.f,2000.f,5000.f,10000.f,20000.f };
    for (float f : gridHz)
    {
        if (f > nyq) break;
        float x = FreqToX(f);
        m_renderTarget->DrawLine(
            D2D1::Point2F(x, m_specTop),
            D2D1::Point2F(x, m_specTop + m_specH),
            m_brGrid.Get(), 0.5f);
    }

    // =========================================================================
    //  2. Notch-filter overlay  (shaded vertical band)
    // =========================================================================
    if (notchOn)
    {
        float xL = FreqToX(nFreq - nWidth * 0.5f);
        float xR = FreqToX(nFreq + nWidth * 0.5f);
        xL = clampf(xL, m_specLeft, m_specLeft + m_specW);
        xR = clampf(xR, m_specLeft, m_specLeft + m_specW);
        D2D1_RECT_F notchRect = D2D1::RectF(xL, m_specTop, xR, m_specTop + m_specH);
        m_renderTarget->FillRectangle(notchRect, m_brNotchFill.Get());
        // Draw the two vertical edge lines
        m_renderTarget->DrawLine(D2D1::Point2F(xL, m_specTop),
                                  D2D1::Point2F(xL, m_specTop + m_specH),
                                  m_brNotchEdge.Get(), 1.5f);
        m_renderTarget->DrawLine(D2D1::Point2F(xR, m_specTop),
                                  D2D1::Point2F(xR, m_specTop + m_specH),
                                  m_brNotchEdge.Get(), 1.5f);
        // Centre tick
        float xC = FreqToX(nFreq);
        m_renderTarget->DrawLine(D2D1::Point2F(xC, m_specTop),
                                  D2D1::Point2F(xC, m_specTop + m_specH),
                                  m_brNotchEdge.Get(), 0.75f);
    }

    // =========================================================================
    //  3. Spectrum fill  (filled polygon under the magnitude curve)
    // =========================================================================
    {
        // Build a path geometry for the filled area
        Microsoft::WRL::ComPtr<ID2D1PathGeometry> path;
        Microsoft::WRL::ComPtr<ID2D1GeometrySink> sink;
        if (SUCCEEDED(m_d2dFactory->CreatePathGeometry(path.GetAddressOf())) &&
            SUCCEEDED(path->Open(sink.GetAddressOf())))
        {
            const int N2 = FFT_SIZE / 2;
            const float binHz = nyq / static_cast<float>(N2);
            float yBase = m_specTop + m_specH;

            bool started = false;
            for (int k = 0; k < N2; ++k)
            {
                float hz = (static_cast<float>(k) + 0.5f) * binHz;
                if (hz < 100.f || hz > nyq) continue;
                float x = FreqToX(hz);
                float y = DbToY(m_magnitudeDb[k]);

                if (!started)
                {
                    sink->BeginFigure(D2D1::Point2F(x, yBase), D2D1_FIGURE_BEGIN_FILLED);
                    sink->AddLine(D2D1::Point2F(x, y));
                    started = true;
                }
                else
                {
                    sink->AddLine(D2D1::Point2F(x, y));
                }
            }
            if (started)
            {
                // Close back down to the baseline
                float xEnd = m_specLeft + m_specW;
                sink->AddLine(D2D1::Point2F(xEnd, yBase));
                sink->EndFigure(D2D1_FIGURE_END_CLOSED);
            }
            sink->Close();
            m_renderTarget->FillGeometry(path.Get(), m_brSpecFill.Get());
        }
    }

    // =========================================================================
    //  4. Spectrum line  (outline of the magnitude curve)
    // =========================================================================
    {
        Microsoft::WRL::ComPtr<ID2D1PathGeometry> path;
        Microsoft::WRL::ComPtr<ID2D1GeometrySink> sink;
        if (SUCCEEDED(m_d2dFactory->CreatePathGeometry(path.GetAddressOf())) &&
            SUCCEEDED(path->Open(sink.GetAddressOf())))
        {
            const int N2 = FFT_SIZE / 2;
            const float binHz = nyq / static_cast<float>(N2);
            bool started = false;
            for (int k = 0; k < N2; ++k)
            {
                float hz = (static_cast<float>(k) + 0.5f) * binHz;
                if (hz < 100.f || hz > nyq) continue;
                float x = FreqToX(hz);
                float y = DbToY(m_magnitudeDb[k]);
                if (!started)
                {
                    sink->BeginFigure(D2D1::Point2F(x, y), D2D1_FIGURE_BEGIN_HOLLOW);
                    started = true;
                }
                else sink->AddLine(D2D1::Point2F(x, y));
            }
            if (started) sink->EndFigure(D2D1_FIGURE_END_OPEN);
            sink->Close();
            m_renderTarget->DrawGeometry(path.Get(), m_brSpecLine.Get(), 1.25f);
        }
    }

    // =========================================================================
    //  5. Peak-hold trace
    // =========================================================================
    {
        Microsoft::WRL::ComPtr<ID2D1PathGeometry> path;
        Microsoft::WRL::ComPtr<ID2D1GeometrySink> sink;
        if (SUCCEEDED(m_d2dFactory->CreatePathGeometry(path.GetAddressOf())) &&
            SUCCEEDED(path->Open(sink.GetAddressOf())))
        {
            const int N2 = FFT_SIZE / 2;
            const float binHz = nyq / static_cast<float>(N2);
            bool started = false;
            for (int k = 0; k < N2; ++k)
            {
                float hz = (static_cast<float>(k) + 0.5f) * binHz;
                if (hz < 100.f || hz > nyq) continue;
                float x = FreqToX(hz);
                float y = DbToY(m_peakHold[k]);
                if (!started)
                {
                    sink->BeginFigure(D2D1::Point2F(x, y), D2D1_FIGURE_BEGIN_HOLLOW);
                    started = true;
                }
                else sink->AddLine(D2D1::Point2F(x, y));
            }
            if (started) sink->EndFigure(D2D1_FIGURE_END_OPEN);
            sink->Close();
            m_renderTarget->DrawGeometry(path.Get(), m_brPeak.Get(), 1.0f);
        }
    }

    // =========================================================================
    //  6. Contour filter curve
    //     We draw the gain curve as a line whose Y position is:
    //       DbToY(ContourGainDb(hz)) -- but offset from the 0-dB reference line
    //     so the operator sees the shape of the response relative to unity.
    //     A horizontal "0 dB" reference line is drawn first in a dim colour.
    // =========================================================================
    if (contOn)
    {
        // Draw 0 dB reference line
        float yZero = DbToY(0.f);
        m_renderTarget->DrawLine(
            D2D1::Point2F(m_specLeft, yZero),
            D2D1::Point2F(m_specLeft + m_specW, yZero),
            m_brContourRef.Get(), 0.75f);

        // Build contour curve path
        Microsoft::WRL::ComPtr<ID2D1PathGeometry> path;
        Microsoft::WRL::ComPtr<ID2D1GeometrySink> sink;
        if (SUCCEEDED(m_d2dFactory->CreatePathGeometry(path.GetAddressOf())) &&
            SUCCEEDED(path->Open(sink.GetAddressOf())))
        {
            // Sample the curve at ~1 pixel intervals
            const int nSteps = static_cast<int>(m_specW);
            bool started = false;
            for (int i = 0; i <= nSteps; ++i)
            {
                float x   = m_specLeft + static_cast<float>(i) / static_cast<float>(nSteps) * m_specW;
                float hz  = XToFreq(x);
                float gDb = ContourGainDb(hz);
                // The visual Y is relative to the 0 dB line but clamped to the area
                float y   = DbToY(clampf(gDb, DB_FLOOR, DB_CEILING));
                if (!started)
                {
                    sink->BeginFigure(D2D1::Point2F(x, y), D2D1_FIGURE_BEGIN_HOLLOW);
                    started = true;
                }
                else sink->AddLine(D2D1::Point2F(x, y));
            }
            if (started) sink->EndFigure(D2D1_FIGURE_END_OPEN);
            sink->Close();
            m_renderTarget->DrawGeometry(path.Get(), m_brContour.Get(), 1.75f);
        }

        // Draw centre-frequency marker
        float xC = FreqToX(cFreq);
        float yC = DbToY(clampf(cDepth, DB_FLOOR, DB_CEILING));
        const float MK = 5.f;
        m_renderTarget->DrawLine(D2D1::Point2F(xC - MK, yC),
                                  D2D1::Point2F(xC + MK, yC),
                                  m_brContour.Get(), 1.5f);
        m_renderTarget->DrawLine(D2D1::Point2F(xC, yC - MK),
                                  D2D1::Point2F(xC, yC + MK),
                                  m_brContour.Get(), 1.5f);
    }

    m_renderTarget->PopAxisAlignedClip();

    // =========================================================================
    //  7. Axis labels
    // =========================================================================
    if (m_textFmt)
    {
        // dB labels on the left
        for (float db = DB_FLOOR; db <= DB_CEILING; db += 20.f)
        {
            float y = DbToY(db);
            auto str = std::format(L"{:.0f}", db);
            D2D1_RECT_F lr = D2D1::RectF(0.f, y - 8.f, m_specLeft - 2.f, y + 8.f);
            m_renderTarget->DrawText(str.c_str(), static_cast<UINT32>(str.size()),
                                     m_textFmt.Get(), lr, m_brLabel.Get());
        }
        // Hz labels on the bottom
        struct { float hz; const wchar_t* lbl; } hzLabels[] = {
            {200.f,L"200"},{500.f,L"500"},{1000.f,L"1k"},
            {2000.f,L"2k"},{5000.f,L"5k"},{10000.f,L"10k"},{20000.f,L"20k"}
        };
        for (auto& e : hzLabels)
        {
            if (e.hz > nyq) break;
            float x = FreqToX(e.hz);
            D2D1_RECT_F lr = D2D1::RectF(x - 18.f, m_specTop + m_specH + 2.f,
                                          x + 18.f, m_specTop + m_specH + MB);
            m_renderTarget->DrawText(e.lbl, static_cast<UINT32>(wcslen(e.lbl)),
                                     m_textFmt.Get(), lr, m_brLabel.Get());
        }
    }

    // =========================================================================
    //  8. Border around the spectrum area
    // =========================================================================
    m_renderTarget->DrawRectangle(specRect, m_brGrid.Get(), 1.0f);

    HRESULT hr = m_renderTarget->EndDraw();
    if (hr == D2DERR_RECREATE_TARGET)
        DiscardDeviceResources();
}

// ===========================================================================
//  DSP helpers
// ===========================================================================

void AudioSpectrumControl::ApplyHannWindow(std::vector<float>& s)
{
    const int N = static_cast<int>(s.size());
    for (int i = 0; i < N; ++i)
    {
        float w = 0.5f * (1.f - std::cosf(2.f * 3.14159265358979323846f * i / (N - 1)));
        s[i] *= w;
    }
}

// Cooley-Tukey iterative radix-2 DIT FFT
void AudioSpectrumControl::FFT(std::vector<std::complex<float>>& buf)
{
    const size_t N = buf.size();
    assert((N & (N - 1)) == 0); // must be power of 2

    // Bit-reversal permutation
    for (size_t i = 1, j = 0; i < N; ++i)
    {
        size_t bit = N >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) std::swap(buf[i], buf[j]);
    }

    // Butterfly stages
    for (size_t len = 2; len <= N; len <<= 1)
    {
        const float ang = -2.f * 3.14159265358979323846f / static_cast<float>(len);
        std::complex<float> wlen(std::cosf(ang), std::sinf(ang));
        for (size_t i = 0; i < N; i += len)
        {
            std::complex<float> w(1.f, 0.f);
            for (size_t j = 0; j < len / 2; ++j)
            {
                std::complex<float> u = buf[i + j];
                std::complex<float> v = buf[i + j + len / 2] * w;
                buf[i + j]            = u + v;
                buf[i + j + len / 2]  = u - v;
                w *= wlen;
            }
        }
    }
}

// Lorentzian (bell) gain curve for the contour filter
float AudioSpectrumControl::ContourGainDb(float hz) const
{
    const float f0    = m_contourFreqHz .load();
    const float bw    = m_contourWidthHz.load();
    const float depth = m_contourDepthDb.load();
    if (bw <= 0.f) return 0.f;
    // Lorentzian: G(f) = depth / (1 + ((f - f0) / (bw/2))^2)
    float ratio = (hz - f0) / (bw * 0.5f);
    return depth / (1.f + ratio * ratio);
}

// ===========================================================================
//  WASAPI capture
// ===========================================================================

void AudioSpectrumControl::StartCapture(int deviceIndex)
{
    m_currentDevice = deviceIndex;
    m_captureRunning.store(true);
    m_captureThread = std::thread([this]{ CaptureThread(); });
}

void AudioSpectrumControl::StopCapture()
{
    m_captureRunning.store(false);
    if (m_captureThread.joinable())
        m_captureThread.join();
    m_currentDevice = -1;
}

void AudioSpectrumControl::CaptureThread()
{
    if(SUCCEEDED(CoInitializeEx(NULL, tagCOINIT::COINIT_APARTMENTTHREADED | tagCOINIT::COINIT_SPEED_OVER_MEMORY)))
    {
    // Raise thread priority to audio (avrt)
    DWORD taskIndex = 0;
    HANDLE hTask = ::AvSetMmThreadCharacteristicsW(L"Audio", &taskIndex);

    // Open the device
    IMMDeviceEnumerator* pEnum  = nullptr;
    IMMDeviceCollection* pColl  = nullptr;
    IMMDevice*           pDev   = nullptr;
    IAudioClient*        pAC    = nullptr;
    IAudioCaptureClient* pCC    = nullptr;

    auto cleanup = [&]{
        SafeRelease(pCC);
        if (pAC) { pAC->Stop(); SafeRelease(pAC); }
        SafeRelease(pDev);
        SafeRelease(pColl);
        SafeRelease(pEnum);
        if (hTask) ::AvRevertMmThreadCharacteristics(hTask);
    };

    HRESULT hr = ::CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr,
                                    CLSCTX_ALL, IID_PPV_ARGS(&pEnum));
    if (FAILED(hr)) { cleanup(); return; }

    hr = pEnum->EnumAudioEndpoints(eCapture, DEVICE_STATE_ACTIVE, &pColl);
    if (FAILED(hr)) { cleanup(); return; }

    hr = pColl->Item(static_cast<UINT>(m_currentDevice), &pDev);
    if (FAILED(hr)) { cleanup(); return; }

    hr = pDev->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr,
                         reinterpret_cast<void**>(&pAC));
    if (FAILED(hr)) { cleanup(); return; }

    // Query the mix format
    WAVEFORMATEX* pFmt = nullptr;
    hr = pAC->GetMixFormat(&pFmt);
    if (FAILED(hr)) { cleanup(); return; }

    // Store sample rate for coordinate mapping
    m_sampleRate.store(pFmt->nSamplesPerSec);

    // Initialise the audio client (100 ms buffer)
    hr = pAC->Initialize(AUDCLNT_SHAREMODE_SHARED, 0,
                          10000000 / 10,  // 100 ms in 100-ns units
                          0, pFmt, nullptr);
    ::CoTaskMemFree(pFmt);
    if (FAILED(hr)) { cleanup(); return; }

    hr = pAC->GetService(__uuidof(IAudioCaptureClient), reinterpret_cast<void**>(&pCC));
    if (FAILED(hr)) { cleanup(); return; }

    hr = pAC->Start();
    if (FAILED(hr)) { cleanup(); return; }

    // ---- Capture loop ----
    while (m_captureRunning.load())
    {
        UINT32 packetSize = 0;
        if (FAILED(pCC->GetNextPacketSize(&packetSize))) break;

        while (packetSize)
        {
            BYTE*  pData   = nullptr;
            UINT32 frames  = 0;
            DWORD  flags   = 0;
            if (FAILED(pCC->GetBuffer(&pData, &frames, &flags, nullptr, nullptr))) break;

            if (!(flags & AUDCLNT_BUFFERFLAGS_SILENT) && pData && frames)
            {
                // We only want mono float samples. The mix format is typically
                // WAVE_FORMAT_IEEE_FLOAT. If multi-channel, we average channels.
                WAVEFORMATEX* fmt2 = nullptr;
                pAC->GetMixFormat(&fmt2);
                const UINT32 ch    = fmt2 ? fmt2->nChannels     : 2;
                const bool   isFloat = fmt2 && (fmt2->wFormatTag == WAVE_FORMAT_IEEE_FLOAT
                                          || (fmt2->wFormatTag == WAVE_FORMAT_EXTENSIBLE));
                if (fmt2) ::CoTaskMemFree(fmt2);

                std::lock_guard<std::mutex> lk(m_bufMutex);
                const auto* pFloat = reinterpret_cast<const float*>(pData);

                for (UINT32 i = 0; i < frames; ++i)
                {
                    float mono = 0.f;
                    if (isFloat)
                    {
                        for (UINT32 c = 0; c < ch; ++c)
                            mono += pFloat[i * ch + c];
                        mono /= static_cast<float>(ch);
                    }
                    else
                    {
                        // 16-bit integer fallback
                        const auto* p16 = reinterpret_cast<const short*>(pData);
                        for (UINT32 c = 0; c < ch; ++c)
                            mono += p16[i * ch + c] / 32768.f;
                        mono /= static_cast<float>(ch);
                    }
                    m_ringBuffer[m_ringWrite] = mono;
                    m_ringWrite = (m_ringWrite + 1) % m_ringSize;
                }
            }

            pCC->ReleaseBuffer(frames);
            if (FAILED(pCC->GetNextPacketSize(&packetSize))) break;
        }

        ::Sleep(10); // yield ~10 ms
    }

    cleanup();
    }
    CoUninitialize();
}