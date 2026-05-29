// =============================================================================
//  DialControl.h
//
//  A ham-radio-style rotatable dial control rendered with Direct2D / GDI
//  DC interop, built on the BaseWindow.h CRTP framework.
//
//  Crash-safety / factory sharing – see CustomScrollBar.h header comment.
//  The same rules apply: pass a shared factory via SetSharedFactory() before
//  calling Create(), call CoInitializeEx before creating anything, and
//  CoUninitialize after releasing the factory.
//
//  Public API:
//    SetRange(min, max)          SetPos(pos, redraw)        GetPos()
//    SetPage(page)               GetPage()
//    SetWrapping(bool)           IsWrapping()
//    ShowMarker(bool)            IsMarkerShown()
//    SetMarkerTicks(int)         GetMarkerTicks()
//    SetTracking(bool)           IsTracking()
//    SetSensitivity(float)       GetSensitivity()
//    SetSharedFactory(ID2D1Factory*)
//    ReleaseResources()          [public]
//
//  Notifications (WM_VSCROLL to parent):
//    SB_THUMBTRACK    – continuous (when tracking == true)
//    SB_THUMBPOSITION – final position on release
//
//  Dependencies: BaseWindow.h (include first), d2d1.lib, dwmapi.lib
// =============================================================================
#pragma once
#ifndef __DIAL_CONTROL_H__
#define __DIAL_CONTROL_H__

#ifndef __BASEWINDOW_H__
#error "DialControl.h requires BaseWindow.h to be included first."
#endif

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwmapi.h>
#include <cmath>
#include <algorithm>

#ifdef _MSC_VER
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwmapi.lib")
#endif

static constexpr float DIAL_PI = 3.14159265358979f;
static constexpr float DIAL_2PI = DIAL_PI * 2.0f;
static constexpr float DIAL_D2R = DIAL_PI / 180.0f;
static constexpr float DIAL_START = -135.0f;
static constexpr float DIAL_SWEEP = 270.0f;

static constexpr float DIAL_RF_KNOB = 0.72f;
static constexpr float DIAL_RF_BEZEL_O = 0.80f;
static constexpr float DIAL_RF_ARC = 0.855f;
static constexpr float DIAL_RF_TICK_IN = 0.875f;
static constexpr float DIAL_RF_TICK_MIN = 0.920f;
static constexpr float DIAL_RF_TICK_MAJ = 0.965f;
static constexpr float DIAL_RF_FOCUS = 0.990f;
static constexpr float DIAL_NEEDLE_END = 0.78f;
static constexpr float DIAL_NEEDLE_BASE = 0.10f;
static constexpr float DIAL_DIMPLE_POS = 0.68f;
static constexpr float DIAL_DIMPLE_RF = 0.060f;
static constexpr float DIAL_HUB_RF = 0.052f;

// ── Colour schemes ────────────────────────────────────────────────────────────
struct DialColorScheme
{
    D2D1_COLOR_F background, bezelOuter, bezelLine;
    D2D1_COLOR_F knobLight, knobMid, knobDark, grooves;
    D2D1_COLOR_F arcTrack, arcActive;
    D2D1_COLOR_F tickMajor, tickMinor;
    D2D1_COLOR_F needle, dimpleRing, dimpleFill, hub;
    D2D1_COLOR_F rimHighlight, focusRing;
};

inline DialColorScheme DialDarkScheme() noexcept
{
    return {
        D2D1::ColorF(0.110f, 0.110f, 0.110f),
        D2D1::ColorF(0.039f, 0.039f, 0.047f),
        D2D1::ColorF(0.235f, 0.239f, 0.278f),
        D2D1::ColorF(0.267f, 0.275f, 0.345f),
        D2D1::ColorF(0.133f, 0.137f, 0.180f),
        D2D1::ColorF(0.055f, 0.059f, 0.086f),
        D2D1::ColorF(0.059f, 0.063f, 0.090f, 0.70f),
        D2D1::ColorF(0.160f, 0.165f, 0.204f),
        D2D1::ColorF(0.000f, 0.471f, 0.831f),
        D2D1::ColorF(0.518f, 0.525f, 0.600f),
        D2D1::ColorF(0.259f, 0.263f, 0.314f),
        D2D1::ColorF(0.953f, 0.953f, 0.953f),
        D2D1::ColorF(0.024f, 0.024f, 0.035f),
        D2D1::ColorF(0.133f, 0.137f, 0.184f),
        D2D1::ColorF(0.200f, 0.208f, 0.263f),
        D2D1::ColorF(1.000f, 1.000f, 1.000f, 0.13f),
        D2D1::ColorF(0.000f, 0.471f, 0.831f, 0.90f),
    };
}

inline DialColorScheme DialLightScheme() noexcept
{
    return {
        D2D1::ColorF(0.953f, 0.953f, 0.953f),
        D2D1::ColorF(0.376f, 0.380f, 0.408f),
        D2D1::ColorF(0.816f, 0.820f, 0.847f),
        D2D1::ColorF(0.949f, 0.949f, 0.957f),
        D2D1::ColorF(0.745f, 0.749f, 0.769f),
        D2D1::ColorF(0.506f, 0.510f, 0.537f),
        D2D1::ColorF(0.450f, 0.454f, 0.475f, 0.50f),
        D2D1::ColorF(0.753f, 0.757f, 0.780f),
        D2D1::ColorF(0.000f, 0.471f, 0.831f),
        D2D1::ColorF(0.259f, 0.263f, 0.302f),
        D2D1::ColorF(0.557f, 0.561f, 0.592f),
        D2D1::ColorF(0.094f, 0.094f, 0.125f),
        D2D1::ColorF(0.306f, 0.310f, 0.349f),
        D2D1::ColorF(0.631f, 0.635f, 0.663f),
        D2D1::ColorF(0.412f, 0.416f, 0.451f),
        D2D1::ColorF(1.000f, 1.000f, 1.000f, 0.75f),
        D2D1::ColorF(0.000f, 0.471f, 0.831f, 0.90f),
    };
}

enum DialBrushIndex
{
    DBR_BG = 0,
    DBR_BEZEL_OUTER,
    DBR_BEZEL_LINE,
    DBR_ARC_TRACK,
    DBR_ARC_ACTIVE,
    DBR_TICK_MAJOR,
    DBR_TICK_MINOR,
    DBR_NEEDLE,
    DBR_DIMPLE_RING,
    DBR_DIMPLE_FILL,
    DBR_HUB,
    DBR_RIM_HIGHLIGHT,
    DBR_FOCUS,
    DBR_GROOVES,
    DBR_COUNT
};

// =============================================================================
class DialControlImpl
    : public BaseWindow<DialControlImpl, Window,
                        WinTraits<WS_CHILD | WS_VISIBLE | WS_TABSTOP>>
{
public:
    DECLARE_WND_CLASS(TEXT("DialControl"))

    DialControlImpl()
        : m_nMin(0), m_nMax(100), m_nPos(0), m_nPage(10), m_bWrapping(false), m_bShowMarker(false), m_nMarkerTicks(0), m_bTracking(false), m_fSensitivity(1.0f), m_bDarkMode(false), m_bFocused(false), m_bMouseIn(false), m_bDragging(false), m_fDragAngle(0.0f), m_fDragAccum(0.0f), m_bOwnsFactory(true), m_pD2DFactory(nullptr), m_pRenderTarget(nullptr), m_pBrushKnobGrad(nullptr), m_pSSRound(nullptr)
    {
        for (int i = 0; i < DBR_COUNT; ++i)
            m_aBrushes[i] = nullptr;
    }

    ~DialControlImpl() { ReleaseResources(); }

    void SetRange(int nMin, int nMax)
    {
        m_nMin = nMin;
        m_nMax = nMax >= nMin ? nMax : nMin;
        m_nPos = Clamp(m_nPos, m_nMin, m_nMax);
        Invalidate();
    }

    void GetRange(int &nMin, int &nMax) const
    {
        nMin = m_nMin;
        nMax = m_nMax;
    }

    int SetPos(int nPos, BOOL bRedraw = TRUE)
    {
        int old = m_nPos;
        m_nPos = Clamp(nPos, m_nMin, m_nMax);
        if (bRedraw)
            Invalidate();
        return old;
    }

    int GetPos() const { return m_nPos; }
    void SetPage(int n) { m_nPage = n > 0 ? n : 1; }
    int GetPage() const { return m_nPage; }
    
    void SetWrapping(bool b)
    {
        m_bWrapping = b;
        Invalidate();
    }

    bool IsWrapping() const { return m_bWrapping; }
    
    void ShowMarker(bool b)
    {
        m_bShowMarker = b;
        Invalidate();
    }

    bool IsMarkerShown() const { return m_bShowMarker; }
    
    void SetMarkerTicks(int n)
    {
        m_nMarkerTicks = n >= 0 ? n : 0;
        Invalidate();
    }

    int GetMarkerTicks() const { return m_nMarkerTicks; }
    void SetTracking(bool b) { m_bTracking = b; }
    bool IsTracking() const { return m_bTracking; }
    void SetSensitivity(float f) { m_fSensitivity = f > 0.001f ? f : 0.001f; }
    float GetSensitivity() const { return m_fSensitivity; }

    void SetSharedFactory(ID2D1Factory *pFactory)
    {
        if (m_bOwnsFactory)
            SafeRelease(m_pD2DFactory);
        m_pD2DFactory = pFactory;
        m_bOwnsFactory = false;
    }

    void ReleaseResources()
    {
        ReleaseBrushes();
        SafeRelease(m_pSSRound);
        SafeRelease(m_pRenderTarget);
        if (m_bOwnsFactory)
            SafeRelease(m_pD2DFactory);
        else
            m_pD2DFactory = nullptr;
    }

    BEGIN_MSG_MAP()
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    MESSAGE_HANDLER(WM_PAINT, OnPaint)
    MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
    MESSAGE_HANDLER(WM_SIZE, OnSize)
    MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
    MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
    MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
    MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
    MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
    MESSAGE_HANDLER(WM_CAPTURECHANGED, OnCaptureChanged)
    MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
    MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
    MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
    MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
    MESSAGE_HANDLER(WM_THEMECHANGED, OnThemeChanged)
    MESSAGE_HANDLER(WM_SETTINGCHANGE, OnSettingChange)
    MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
    END_MSG_MAP()

private:
    int m_nMin, m_nMax, m_nPos, m_nPage;
    bool m_bWrapping, m_bShowMarker;
    int m_nMarkerTicks;
    bool m_bTracking;
    float m_fSensitivity;
    bool m_bDarkMode, m_bFocused, m_bMouseIn, m_bDragging;
    float m_fDragAngle, m_fDragAccum;
    bool m_bOwnsFactory;

    ID2D1Factory *m_pD2DFactory;
    ID2D1DCRenderTarget *m_pRenderTarget;
    ID2D1SolidColorBrush *m_aBrushes[DBR_COUNT];
    ID2D1RadialGradientBrush *m_pBrushKnobGrad;
    ID2D1StrokeStyle *m_pSSRound;

    static int Clamp(int v, int lo, int hi) noexcept { return v < lo ? lo : v > hi ? hi
                                                                                   : v; }
    static float Clampf(float v, float lo, float hi) noexcept { return v < lo ? lo : v > hi ? hi
                                                                                            : v; }
    template <class T>
    static void SafeRelease(T *&p) noexcept
    {
        if (p)
        {
            p->Release();
            p = nullptr;
        }
    }
    static bool DetectDarkMode() noexcept
    {
        DWORD v = 1, sz = sizeof(v);
        ::RegGetValueW(HKEY_CURRENT_USER,
                       L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                       L"AppsUseLightTheme", RRF_RT_REG_DWORD, nullptr, &v, &sz);
        return v == 0;
    }
    void Invalidate() const
    {
        if (IsWindow())
            ::InvalidateRect(m_hwnd,nullptr,FALSE);
    }
    void NotifyParent(int nCode) const
    {
        HWND hP = GetParent();
        if (!hP)
            return;
        ::SendMessage(hP, WM_VSCROLL,
                      MAKEWPARAM(static_cast<WORD>(nCode), static_cast<WORD>(m_nPos)),
                      reinterpret_cast<LPARAM>(m_hwnd));
    }

    static D2D1_POINT_2F ClockPt(float cx, float cy, float R, float rad) noexcept
    {
        return D2D1::Point2F(cx + R * sinf(rad), cy - R * cosf(rad));
    }

    float PosToAngle(int pos) const noexcept
    {
        float range = static_cast<float>(m_nMax - m_nMin);
        float t = range > 0.0f ? Clampf(static_cast<float>(pos - m_nMin) / range, 0.0f, 1.0f) : 0.0f;
        float sweepDeg = m_bWrapping ? 360.0f : DIAL_SWEEP;
        return (DIAL_START + t * sweepDeg) * DIAL_D2R;
    }

    static float ScreenAngle(POINT pt, float cx, float cy) noexcept
    {
        return atan2f(static_cast<float>(pt.y) - cy, static_cast<float>(pt.x) - cx);
    }

    bool ApplyAngleDelta(float delta, int &newPosOut)
    {
        while (delta > DIAL_PI)
            delta -= DIAL_2PI;
        while (delta < -DIAL_PI)
            delta += DIAL_2PI;
        float range = static_cast<float>(m_nMax - m_nMin);
        m_fDragAccum += (delta / DIAL_2PI) * range * m_fSensitivity;
        int newPos;
        if (m_bWrapping && range > 0.0f)
        {
            float posRange = range + 1.0f;
            float w = fmodf(m_fDragAccum - static_cast<float>(m_nMin), posRange);
            if (w < 0.0f)
                w += posRange;
            newPos = Clamp(m_nMin + static_cast<int>(w), m_nMin, m_nMax);
        }
        else
        {
            newPos = Clamp(static_cast<int>(m_fDragAccum + 0.5f), m_nMin, m_nMax);
        }
        newPosOut = newPos;
        return newPos != m_nPos;
    }

    HRESULT CreateD2DFactory()
    {
        if (m_pD2DFactory)
            return S_OK;
        m_bOwnsFactory = true;
        HRESULT hr = ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
        if (SUCCEEDED(hr) && !m_pSSRound)
        {
            D2D1_STROKE_STYLE_PROPERTIES ssp = {};
            ssp.startCap = D2D1_CAP_STYLE_ROUND;
            ssp.endCap = D2D1_CAP_STYLE_ROUND;
            ssp.lineJoin = D2D1_LINE_JOIN_ROUND;
            ssp.dashStyle = D2D1_DASH_STYLE_SOLID;
            ssp.miterLimit = 10.0f;
            m_pD2DFactory->CreateStrokeStyle(ssp, nullptr, 0, &m_pSSRound);
        }
        return hr;
    }

    HRESULT CreateRenderTarget()
    {
        if (m_pRenderTarget)
            return S_OK;
        if (!m_pD2DFactory)
            return E_FAIL;
        D2D1_RENDER_TARGET_PROPERTIES p = D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE));
        return m_pD2DFactory->CreateDCRenderTarget(&p, &m_pRenderTarget);
    }

    HRESULT CreateBrushes()
    {
        if (!m_pRenderTarget)
            return E_FAIL;
        DialColorScheme c = m_bDarkMode ? DialDarkScheme() : DialLightScheme();
        const D2D1_COLOR_F clrMap[DBR_COUNT] = {
            c.background, c.bezelOuter, c.bezelLine,
            c.arcTrack, c.arcActive,
            c.tickMajor, c.tickMinor,
            c.needle, c.dimpleRing, c.dimpleFill,
            c.hub, c.rimHighlight, c.focusRing, c.grooves};
        HRESULT hr = S_OK;
        for (int i = 0; i < DBR_COUNT && SUCCEEDED(hr); ++i)
            hr = m_pRenderTarget->CreateSolidColorBrush(clrMap[i], &m_aBrushes[i]);
        if (SUCCEEDED(hr))
        {
            D2D1_GRADIENT_STOP stops[3] = {{0.0f, c.knobLight}, {0.5f, c.knobMid}, {1.0f, c.knobDark}};
            ID2D1GradientStopCollection *pStops = nullptr;
            hr = m_pRenderTarget->CreateGradientStopCollection(stops, 3, D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &pStops);
            if (SUCCEEDED(hr))
            {
                D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES rp;
                rp.center = D2D1::Point2F(0, 0);
                rp.gradientOriginOffset = D2D1::Point2F(0, 0);
                rp.radiusX = rp.radiusY = 50.f;
                hr = m_pRenderTarget->CreateRadialGradientBrush(rp, pStops, &m_pBrushKnobGrad);
                pStops->Release();
            }
        }
        return hr;
    }

    void ReleaseBrushes()
    {
        for (int i = 0; i < DBR_COUNT; ++i)
            SafeRelease(m_aBrushes[i]);
        SafeRelease(m_pBrushKnobGrad);
    }

    void RecoverDeviceLoss()
    {
        ReleaseBrushes();
        SafeRelease(m_pRenderTarget);
        if (SUCCEEDED(CreateRenderTarget()))
            CreateBrushes();
    }

    void DrawArc(float cx, float cy, float radius, float startRad, float endRad,
                 ID2D1Brush *pBrush, float strokeW, ID2D1StrokeStyle *pSS = nullptr) const
    {
        if (!m_pD2DFactory || !m_pRenderTarget || !pBrush)
            return;
        float span = endRad - startRad;
        while (span < 0.0f)
            span += DIAL_2PI;
        while (span >= DIAL_2PI)
            span -= DIAL_2PI;
        if (span < 0.0005f)
            return;
        if (span > DIAL_2PI - 0.0005f)
        {
            m_pRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(cx, cy), radius, radius), pBrush, strokeW, pSS);
            return;
        }
        ID2D1PathGeometry *pGeom = nullptr;
        if (FAILED(m_pD2DFactory->CreatePathGeometry(&pGeom)))
            return;
        ID2D1GeometrySink *pSink = nullptr;
        if (SUCCEEDED(pGeom->Open(&pSink)))
        {
            pSink->BeginFigure(ClockPt(cx, cy, radius, startRad), D2D1_FIGURE_BEGIN_HOLLOW);
            D2D1_ARC_SEGMENT arc = {};
            arc.point = ClockPt(cx, cy, radius, endRad);
            arc.size = D2D1::SizeF(radius, radius);
            arc.rotationAngle = 0.0f;
            arc.sweepDirection = D2D1_SWEEP_DIRECTION_CLOCKWISE;
            arc.arcSize = (span > DIAL_PI) ? D2D1_ARC_SIZE_LARGE : D2D1_ARC_SIZE_SMALL;
            pSink->AddArc(arc);
            pSink->EndFigure(D2D1_FIGURE_END_OPEN);
            pSink->Close();
            pSink->Release();
            m_pRenderTarget->DrawGeometry(pGeom, pBrush, strokeW, pSS);
        }
        pGeom->Release();
    }

    void DrawDial(float w, float h)
    {
        if (!m_pRenderTarget)
            return;
        const float cx = w * 0.5f, cy = h * 0.5f, hmin = std::min(w, h) * 0.5f;
        const float rKnob = hmin * DIAL_RF_KNOB, rBezelO = hmin * DIAL_RF_BEZEL_O;
        const float rArc = hmin * DIAL_RF_ARC;
        const float rTickIn = hmin * DIAL_RF_TICK_IN, rTickMin = hmin * DIAL_RF_TICK_MIN, rTickMaj = hmin * DIAL_RF_TICK_MAJ;
        const float rFocus = hmin * DIAL_RF_FOCUS;
        const float rDimple = hmin * DIAL_DIMPLE_RF, rHub = hmin * DIAL_HUB_RF;
        const float sweepDeg = m_bWrapping ? 360.0f : DIAL_SWEEP;
        const float trackStart = DIAL_START * DIAL_D2R;
        const float trackEnd = (DIAL_START + sweepDeg) * DIAL_D2R;
        const float valueAngle = PosToAngle(m_nPos);

        m_pRenderTarget->FillRectangle(D2D1::RectF(0, 0, w, h), m_aBrushes[DBR_BG]);

        if (m_bShowMarker && m_nMarkerTicks > 0)
            for (int i = 0; i <= m_nMarkerTicks; ++i)
            {
                float t = (float)i / (float)m_nMarkerTicks;
                float a = (DIAL_START + t * sweepDeg) * DIAL_D2R;
                bool maj = ((i % 5) == 0);
                m_pRenderTarget->DrawLine(
                    ClockPt(cx, cy, rTickIn, a), ClockPt(cx, cy, maj ? rTickMaj : rTickMin, a),
                    m_aBrushes[maj ? DBR_TICK_MAJOR : DBR_TICK_MINOR], maj ? 1.5f : 1.0f);
            }

        DrawArc(cx, cy, rArc, trackStart, trackEnd, m_aBrushes[DBR_ARC_TRACK], hmin * 0.038f);
        if (m_nMax > m_nMin && m_nPos > m_nMin)
            DrawArc(cx, cy, rArc, trackStart, valueAngle, m_aBrushes[DBR_ARC_ACTIVE], hmin * 0.050f);
        else if (m_bWrapping && m_nPos == m_nMax && m_nMax > m_nMin)
            m_pRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(cx, cy), rArc, rArc), m_aBrushes[DBR_ARC_ACTIVE], hmin * 0.050f);

        m_pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(cx, cy), rBezelO, rBezelO), m_aBrushes[DBR_BEZEL_OUTER]);
        m_pRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(cx, cy), rKnob * 1.025f, rKnob * 1.025f), m_aBrushes[DBR_BEZEL_LINE], 1.2f);

        m_pBrushKnobGrad->SetCenter(D2D1::Point2F(cx, cy));
        m_pBrushKnobGrad->SetGradientOriginOffset(D2D1::Point2F(-rKnob * 0.28f, -rKnob * 0.28f));
        m_pBrushKnobGrad->SetRadiusX(rKnob);
        m_pBrushKnobGrad->SetRadiusY(rKnob);
        m_pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(cx, cy), rKnob, rKnob), m_pBrushKnobGrad);

        for (float fr : {0.30f, 0.57f, 0.84f})
            m_pRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(cx, cy), rKnob * fr, rKnob * fr), m_aBrushes[DBR_GROOVES], 1.0f);

        m_pRenderTarget->DrawLine(
            ClockPt(cx, cy, rKnob * DIAL_NEEDLE_BASE, valueAngle),
            ClockPt(cx, cy, rKnob * DIAL_NEEDLE_END, valueAngle),
            m_aBrushes[DBR_NEEDLE], hmin * 0.028f, m_pSSRound);

        D2D1_POINT_2F dc = ClockPt(cx, cy, rKnob * DIAL_DIMPLE_POS, valueAngle);
        m_pRenderTarget->FillEllipse(D2D1::Ellipse(dc, rDimple, rDimple), m_aBrushes[DBR_DIMPLE_RING]);
        m_pRenderTarget->FillEllipse(D2D1::Ellipse(dc, rDimple * 0.60f, rDimple * 0.60f), m_aBrushes[DBR_DIMPLE_FILL]);

        m_pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(cx, cy), rHub, rHub), m_aBrushes[DBR_HUB]);
        m_pRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(cx, cy), rHub, rHub), m_aBrushes[DBR_GROOVES], 1.0f);

        DrawArc(cx, cy, rKnob * 0.975f, -DIAL_PI * 0.5f, 0.0f, m_aBrushes[DBR_RIM_HIGHLIGHT], hmin * 0.022f, m_pSSRound);

        if (m_bFocused)
            m_pRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(cx, cy), rFocus, rFocus), m_aBrushes[DBR_FOCUS], hmin * 0.022f);
    }

    // Message handlers
    LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        m_bDarkMode = DetectDarkMode();
        if (!m_pD2DFactory)
            CreateD2DFactory();
        if (SUCCEEDED(CreateRenderTarget()))
            CreateBrushes();
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        ReleaseResources();
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        bHandled = TRUE;
        return 1;
    }

    LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint( &ps);
        if (!m_pRenderTarget)
        {
            EndPaint( &ps);
            bHandled = TRUE;
            return 0;
        }
        if (!m_aBrushes[0])
            CreateBrushes();
        RECT rc;
        GetClientRect( &rc);
        HRESULT hr = m_pRenderTarget->BindDC(hdc, &rc);
        if (hr == D2DERR_RECREATE_TARGET)
        {
            RecoverDeviceLoss();
            hr = m_pRenderTarget ? m_pRenderTarget->BindDC(hdc, &rc) : E_FAIL;
        }
        if (SUCCEEDED(hr))
        {
            m_pRenderTarget->BeginDraw();
            DrawDial((float)(rc.right - rc.left), (float)(rc.bottom - rc.top));
            hr = m_pRenderTarget->EndDraw();
            if (hr == D2DERR_RECREATE_TARGET)
                RecoverDeviceLoss();
        }
        EndPaint( &ps);
        bHandled = TRUE;
        return 0;
    }

    LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        Invalidate();
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnSetCursor(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        ::SetCursor(::LoadCursor(nullptr, IDC_HAND));
        bHandled = TRUE;
        return TRUE;
    }

    LRESULT OnLButtonDown(UINT, WPARAM, LPARAM lParam, BOOL &bHandled)
    {
        SetFocus();
        SetCapture();
        Rect rc;
        GetClientRect(&rc);
        float cx = (rc.right - rc.left) * 0.5f, cy = (rc.bottom - rc.top) * 0.5f;
        POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        m_fDragAngle = ScreenAngle(pt, cx, cy);
        m_fDragAccum = static_cast<float>(m_nPos);
        m_bDragging = true;
        bHandled = TRUE;
        return 0;
    }

    LRESULT OnLButtonUp(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        if (m_bDragging)
        {
            m_bDragging = false;
            NotifyParent(SB_THUMBPOSITION);
            Invalidate();
        }
        ::ReleaseCapture();
        bHandled = TRUE;
        return 0;
    }

    LRESULT OnMouseMove(UINT, WPARAM, LPARAM lParam, BOOL &bHandled)
    {
        POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        if (m_bDragging)
        {
            Rect rc;
            GetClientRect( &rc);
            float cx = (rc.right - rc.left) * 0.5f, cy = (rc.bottom - rc.top) * 0.5f;
            float newAngle = ScreenAngle(pt, cx, cy);
            float delta = newAngle - m_fDragAngle;
            int newPos;
            if (ApplyAngleDelta(delta, newPos))
            {
                m_nPos = newPos;
                if (m_bTracking)
                    NotifyParent(SB_THUMBTRACK);
                Invalidate();
            }
            m_fDragAngle = newAngle;
        }
        if (!m_bMouseIn)
        {
            m_bMouseIn = true;
            SetFocus();
            TRACKMOUSEEVENT tme = {sizeof(tme), TME_LEAVE, m_hwnd, 0};
            ::TrackMouseEvent(&tme);
        }
        bHandled = TRUE;
        return 0;
    }

    LRESULT OnMouseLeave(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        m_bMouseIn = false;
        // HWND hP = ::GetParent(m_hwnd);
        // if (hP)
        //     ::SetFocus(hP);
        bHandled = TRUE;
        return 0;
    }

    LRESULT OnMouseWheel(UINT, WPARAM wParam, LPARAM, BOOL &bHandled)
    {
        int d = GET_WHEEL_DELTA_WPARAM(wParam), step = d > 0 ? 1 : -1;
        int newPos = m_nPos + step;
        if (m_bWrapping && m_nMax > m_nMin)
        {
            if (newPos < m_nMin)
                newPos = m_nMax;
            else if (newPos > m_nMax)
                newPos = m_nMin;
        }
        else
            newPos = Clamp(newPos, m_nMin, m_nMax);
        if (newPos != m_nPos)
        {
            m_nPos = newPos;
            NotifyParent(SB_THUMBPOSITION);
            Invalidate();
        }
        bHandled = TRUE;
        return 0;
    }

    LRESULT OnCaptureChanged(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        if (m_bDragging)
        {
            m_bDragging = false;
            NotifyParent(SB_THUMBPOSITION);
            Invalidate();
        }
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnKeyDown(UINT, WPARAM vk, LPARAM, BOOL &bHandled)
    {
        bool sh = (::GetKeyState(VK_SHIFT) & 0x8000) != 0;
        int newPos = m_nPos, code = -1;
        switch (vk)
        {
        case VK_LEFT:
        case VK_UP:
            newPos += sh ? 5 : 1;
            code = SB_LINEDOWN;
            break;
        case VK_RIGHT:
        case VK_DOWN:
            newPos -= sh ? 5 : 1;
            code = SB_LINEUP;
            break;
        case VK_PRIOR:
            newPos += m_nPage;
            code = SB_PAGEDOWN;
            break;
        case VK_NEXT:
            newPos -= m_nPage;
            code = SB_PAGEUP;
            break;
        case VK_HOME:
            newPos = m_nMin;
            code = SB_TOP;
            break;
        case VK_END:
            newPos = m_nMax;
            code = SB_BOTTOM;
            break;
        default:
            bHandled = FALSE;
            return 0;
        }
        if (m_bWrapping && m_nMax > m_nMin)
        {
            int span = m_nMax - m_nMin + 1;
            while (newPos < m_nMin)
                newPos += span;
            while (newPos > m_nMax)
                newPos -= span;
        }
        else
            newPos = Clamp(newPos, m_nMin, m_nMax);
        if (newPos != m_nPos)
        {
            m_nPos = newPos;
            NotifyParent(code);
            Invalidate();
        }
        bHandled = TRUE;
        return 0;
    }

    LRESULT OnSetFocus(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        m_bFocused = true;
        Invalidate();
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnKillFocus(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        m_bFocused = false;
        Invalidate();
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnGetDlgCode(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        bHandled = TRUE;
        return DLGC_WANTARROWS | DLGC_WANTCHARS;
    }

    LRESULT OnThemeChanged(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        m_bDarkMode = DetectDarkMode();
        ReleaseBrushes();
        Invalidate();
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnSettingChange(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        m_bDarkMode = DetectDarkMode();
        ReleaseBrushes();
        Invalidate();
        bHandled = FALSE;
        return 0;
    }
};

typedef DialControlImpl DialControl;
#endif // __DIAL_CONTROL_H__
