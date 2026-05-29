#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include <WinUtility/ComPtr.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <format>

class GdiInterop : BaseWindow<GdiInterop,Window,FrameWinTraits>
{
    ComPtr<ID2D1Factory> m_factory;
    ComPtr<ID2D1DCRenderTarget> m_renderTarget;
    ComPtr<ID2D1SolidColorBrush> m_brush;
    ComPtr<ID2D1SolidColorBrush> m_alphabrush;
    ComPtr<IDWriteFactory> m_dwfactory;
    ComPtr<IDWriteTextFormat> m_format;
    PushButtonControl m_btnSend;
    EditControl m_editBox;
    HRESULT CreateDeviceIndependantResources();
    HRESULT CreateDeviceResources();
    HRESULT OnRender(const PAINTSTRUCT& ps);
    void DiscardDeviceResources();
    void OnPaint(DC dc);
    int OnCreate(LPCREATESTRUCT lpCreateStruct);
    void OnDestroy();
    void OnSendClick(UINT uNotifyCode, int nID, Window wndCtl);
    POINT pntArray[2] = {
        {400,150},
        {static_cast<LONG>(400 + 100 * 0.15425),
         static_cast<LONG>(150 - 100 * 0.9885)}
    };
    POINT pntArray2[2] = {
        {400,150},
        {static_cast<LONG>(400 + 100 * 0.525),
         static_cast<LONG>(150 + 100 * 0.8509)}
    };
    POINT pntArray3[2] = {
        {400,150},
        {static_cast<LONG>(400 - 100 * 0.988),
         static_cast<LONG>(150 - 100 * 0.15425)}
    };
    D2D1_POINT_2F line1[2] = { D2D1::Point2F(150.0f,150.0f),
            D2D1::Point2F((150.0f + 100.0f * 0.15425f),
        (150.0f - 100.0f * 0.988f))};
    D2D1_POINT_2F line2[2] = {D2D1::Point2F(150.0f,150.0f),
            D2D1::Point2F((150.0f + 100.0f * 0.525f),
            (150.0f + 100.0f * 0.8509f))};
    
    D2D1_POINT_2F line3[2] = {D2D1::Point2F(150.0f,150.0f),
            D2D1::Point2F((150.0f - 100.0f * 0.988f),
            (150.0f - 100.0f * 0.15425f))};
public:

    DECLARE_WND_CLASS(TEXT("GdiInterop"));

    HRESULT Initalize();

    BEGIN_MSG_MAP()
    MSG_WM_PAINT(OnPaint)
    MSG_WM_CREATE(OnCreate)
    MSG_WM_DESTROY(OnDestroy)
    BEGIN_COMMAND_MAP()
        COMMAND_ID_HANDLER_EX(1,OnSendClick);
    END_COMMAND_MAP()
    END_MSG_MAP()

};