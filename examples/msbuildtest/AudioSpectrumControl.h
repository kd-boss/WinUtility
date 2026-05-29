#pragma once
// =============================================================================
//  AudioSpectrumControl.h  –  see AudioSpectrumControl.cpp for implementation
//
//  A BaseWindow<T,Window,ControlTraits> CRTP child window that:
//    * Enumerates WASAPI capture (input) devices via GetAudioInputDevices()
//    * Captures audio on a background thread, computes a real-time FFT
//    * Renders the spectrum via Direct2D at ~30 fps
//    * Overlays a notch-filter band   (FTDX101MP IF-notch model)
//    * Overlays a contour-filter curve (FTDX101MP parametric contour model)
//
//  Link: d2d1  dwrite  ole32  avrt
//
//  Quick-start:
//    auto devs = AudioSpectrumControl::GetAudioInputDevices(); // devs[0]="Mic..."
//    RECT rc{ 0,0,600,300 };
//    m_spectrum.Create(m_hWnd, &rc, nullptr, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN);
//    m_spectrum.SetDevice(0);
//
//    m_spectrum.EnableNotch(true);
//    m_spectrum.SetNotchFrequency(1350.f);   // Hz  centre of rejected band
//    m_spectrum.SetNotchWidth(80.f);          // Hz  total width
//
//    m_spectrum.EnableContour(true);
//    m_spectrum.SetContourFrequency(800.f);  // Hz  bell-curve centre
//    m_spectrum.SetContourWidth(400.f);       // Hz  -3 dB bandwidth
//    m_spectrum.SetContourDepth(-18.f);       // dB  negative=cut, positive=boost
// =============================================================================

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <avrt.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wrl/client.h>
#include <atomic>
#include <complex>
#include <cmath>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>

#ifdef _MSC_VER
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
#pragma comment(lib,"ole32.lib")
#pragma comment(lib,"avrt.lib")
#endif
// =============================================================================
class AudioSpectrumControl
    : public BaseWindow<AudioSpectrumControl, Window, ControlTraits>
{
public:
    // -------------------------------------------------------------------------
    //  DSP / display constants
    // -------------------------------------------------------------------------
    static constexpr int   FFT_SIZE     = 2048;    // must be power-of-2
    static constexpr float DB_FLOOR     = -90.0f;  // dBFS display floor
    static constexpr float DB_CEILING   =   0.0f;  // dBFS display ceiling
    static constexpr float PEAK_DECAY   =   0.93f; // per-frame peak-hold decay

    // Pixel margins around the spectrum plot area
    static constexpr float ML = 45.0f; // left  (dB axis labels)
    static constexpr float MR =  8.0f; // right
    static constexpr float MT =  8.0f; // top
    static constexpr float MB = 22.0f; // bottom (Hz axis labels)

    // =========================================================================
    //  BaseWindow CRTP requirements
    // =========================================================================
    static LPCTSTR GetWinClassName() { return TEXT("AudioSpectrumControl"); }

    static WNDCLASSEX GetWinClassInfo()
    {
        WNDCLASSEX wcx{};
        wcx.cbSize        = sizeof(wcx);
        wcx.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
        wcx.lpfnWndProc   = WindowProc;          // patched by BaseWindow
        wcx.hInstance     = HINST_THISCOMPONENT;
        wcx.hCursor       = ::LoadCursor(nullptr, IDC_ARROW);
        wcx.hbrBackground = nullptr;             // fully owner-drawn
        wcx.lpszClassName = GetWinClassName();
        return wcx;
    }

    // =========================================================================
    //  Audio device enumeration
    // =========================================================================

    /// Returns friendly names of all active WASAPI capture endpoints.
    /// The vector index is the deviceIndex argument accepted by SetDevice().
    static std::vector<std::wstring> GetAudioInputDevices();

    // =========================================================================
    //  Device selection
    // =========================================================================

    /// Begin capturing from the device at deviceIndex.  Pass -1 to stop.
    void SetDevice(int deviceIndex);

    // =========================================================================
    //  Notch filter  (FTDX101MP IF-notch model)
    //
    //  Paints a semi-transparent vertical band to show the rejected range.
    // =========================================================================
    void EnableNotch(bool enable);
    void SetNotchFrequency(float frequencyHz); // centre of the rejected band
    void SetNotchWidth(float widthHz);          // total bandwidth of rejection

    // =========================================================================
    //  Contour filter  (FTDX101MP parametric contour model)
    //
    //  The FTDX101MP contour is a variable-frequency parametric EQ stage that
    //  applies a Lorentzian (bell-shaped) boost or cut inside the passband.
    //
    //    frequency  – centre Hz (typical range 100–4000 Hz)
    //    width      – the -3 dB bandwidth of the bell curve in Hz;
    //                 larger = broader/gentler, smaller = sharper
    //    depth      – gain at the centre in dB;
    //                 negative = cut/notch (suppress hum, hiss, QRM),
    //                 positive = boost/peak (bring out speech fundamentals).
    //                 Typical FTDX101MP range: -40 … +20 dB
    //
    //  The gain curve is drawn overlaid on the live spectrum.
    // =========================================================================
    void EnableContour(bool enable);
    void SetContourFrequency(float frequencyHz); // centre of the peak/dip
    void SetContourWidth(float widthHz);          // -3 dB bandwidth in Hz
    void SetContourDepth(float depthDb);          // dB; negative=cut, positive=boost
    void SetFactory(ID2D1Factory* pft);
    // =========================================================================
    //  Message map
    // =========================================================================
    BEGIN_MSG_MAP()
        MSG_WM_CREATE(OnCreate)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_PAINT(OnPaint)
        MSG_WM_SIZE(OnSize)
        MSG_WM_TIMER(OnTimer)
        MSG_WM_ERASEBKGND(OnEraseBkgnd)
    END_MSG_MAP()

private:
    // -------------------------------------------------------------------------
    //  Message handlers
    // -------------------------------------------------------------------------
    int  OnCreate(LPCREATESTRUCT lpcs);
    void OnDestroy();
    void OnPaint(HDC hdc);
    void OnSize(UINT nType, Size sz);      // uses framework's Size class
    void OnTimer(UINT_PTR nIDEvent);
    BOOL OnEraseBkgnd(HDC hdc);

    // -------------------------------------------------------------------------
    //  Direct2D helpers
    // -------------------------------------------------------------------------
    HRESULT CreateDeviceResources();
    void    DiscardDeviceResources();
    void    Render();

    // Coordinate helpers (results are in spectrum-area pixel space)
    float FreqToX(float hz) const;   // logarithmic Hz -> X pixel (within spec area)
    float DbToY  (float db) const;   // dBFS value  -> Y pixel
    float XToFreq(float x)  const;   // inverse of FreqToX

    // -------------------------------------------------------------------------
    //  DSP helpers
    // -------------------------------------------------------------------------
    // In-place Cooley-Tukey radix-2 DIT FFT; buf.size() must be a power of 2
    static void FFT(std::vector<std::complex<float>>& buf);
    // Apply a Hann window to a float PCM buffer (in-place)
    static void ApplyHannWindow(std::vector<float>& s);
    // Lorentzian gain curve: returns contour filter gain at hz in dB
    float ContourGainDb(float hz) const;

    // -------------------------------------------------------------------------
    //  WASAPI capture
    // -------------------------------------------------------------------------
    void StartCapture(int deviceIndex);
    void StopCapture();
    void CaptureThread(); // entry point for m_captureThread

    // -------------------------------------------------------------------------
    //  Direct2D device-dependent resources
    // -------------------------------------------------------------------------
    Microsoft::WRL::ComPtr<ID2D1Factory>            m_d2dFactory;
    Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget>   m_renderTarget;
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_brBg;
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_brGrid;
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_brLabel;
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_brSpecFill;
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_brSpecLine;
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_brPeak;
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_brNotchFill;
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_brNotchEdge;
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_brContour;
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_brContourRef;
    Microsoft::WRL::ComPtr<IDWriteFactory>           m_dwFactory;
    Microsoft::WRL::ComPtr<IDWriteTextFormat>        m_textFmt;

    // -------------------------------------------------------------------------
    //  Spectrum data
    //  m_bufMutex guards m_ringBuffer/m_ringWrite.
    //  m_magnitudeDb and m_peakHold are only touched on the UI thread after
    //  being copied from the ring buffer under the mutex.
    // -------------------------------------------------------------------------
    std::mutex         m_bufMutex;
    std::vector<float> m_ringBuffer;      // raw mono float PCM ring buffer
    size_t             m_ringWrite{ 0 };  // next write position (wraps)
    size_t             m_ringSize{ 0 };   // == FFT_SIZE * 4

    std::vector<float> m_magnitudeDb;     // current FFT frame  (FFT_SIZE/2 bins)
    std::vector<float> m_peakHold;        // per-bin peak-hold trace

    // -------------------------------------------------------------------------
    //  Capture thread
    // -------------------------------------------------------------------------
    std::thread        m_captureThread;
    std::atomic<bool>  m_captureRunning{ false };
    std::atomic<UINT32>m_sampleRate{ 48000 };
    int                m_currentDevice{ -1 };

    // -------------------------------------------------------------------------
    //  Filter parameters (atomics so public setters are thread-safe)
    // -------------------------------------------------------------------------
    std::atomic<bool>  m_notchEnabled{ false };
    std::atomic<float> m_notchFreqHz { 1000.0f };
    std::atomic<float> m_notchWidthHz{  100.0f };

    std::atomic<bool>  m_contourEnabled{ false };
    std::atomic<float> m_contourFreqHz {  800.0f };
    std::atomic<float> m_contourWidthHz{  300.0f };
    std::atomic<float> m_contourDepthDb{  -20.0f };

    // -------------------------------------------------------------------------
    //  Layout cache (updated in OnSize)
    // -------------------------------------------------------------------------
    float m_clientW{ 0.f }, m_clientH{ 0.f };
    float m_specLeft{ 0.f }, m_specTop{ 0.f };
    float m_specW{ 0.f },    m_specH{ 0.f };

    static constexpr UINT_PTR REDRAW_TIMER_ID    = 1;
    static constexpr UINT     REDRAW_INTERVAL_MS = 33;  // ~30 fps
};