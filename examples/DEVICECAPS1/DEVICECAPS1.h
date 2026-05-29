
#ifndef WINVER
#define WINVER 0x0A00
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif

#include <WinUtility/System.h>
#include <WinUtility/BaseWindow.h>
#include <WinUtility/ComPtr.h>
#include "Resource.h"
#include "About.h"
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

struct
{
    int iIndex;
    std::tstring szLabel;
    std::tstring szDesc;
    std::tstring szVal;
}
devicecaps[] = {
    HORZSIZE, TEXT("HORZSIZE"), TEXT("Width in millimeters:"),TEXT(""),
    VERTSIZE, TEXT("VERTSIZE"), TEXT("Height in millimeters:"),TEXT(""),
    HORZRES, TEXT("HORZRES"), TEXT("Width inpixels:"),TEXT(""),
    VERTRES, TEXT("VERTRES"), TEXT("Height in raster lines:"),TEXT(""),
    BITSPIXEL, TEXT("BITSPIXEL"), TEXT("Color bits per pixel:"),TEXT(""),
    PLANES, TEXT("PLANES"), TEXT("Number of color planes"),TEXT(""),
    NUMBRUSHES, TEXT("NUMBRUSHES"), TEXT("Number of device brushes:"),TEXT(""),
    NUMPENS, TEXT("NUMPENS"), TEXT("Number of device pens:"),TEXT(""),
    NUMMARKERS, TEXT("NUMMARKERS"),TEXT("Number of device markers:"),TEXT(""),
    NUMFONTS, TEXT("NUMFONTS"),TEXT("Number of device fonts:"),TEXT(""),
    NUMCOLORS, TEXT("NUMCOLORS"),TEXT("Number of device colors:"),TEXT(""),
    PDEVICESIZE,TEXT("PDEVICESIZE"),TEXT("Size of device structure:"),TEXT(""),
    ASPECTX, TEXT("ASPECTX"), TEXT("Relative width of pixel:"),TEXT(""),
    ASPECTY, TEXT("ASPECTY"), TEXT("Relative height of pixel:"),TEXT(""),
    LOGPIXELSX, TEXT("LOGPIXELSX"), TEXT("Horizontal dots per inch:"),TEXT(""),
    LOGPIXELSY, TEXT("LOGPIXELSY"), TEXT("Vertical dots per inch:"),TEXT(""),
    SIZEPALETTE, TEXT("SIZEPLATTE"), TEXT("Number of platte entries:"),TEXT(""),
    NUMRESERVED, TEXT("NUMRESERVED"),TEXT("Reserved platte entries:"),TEXT(""),
    COLORRES, TEXT("COLORRES"), TEXT("Actual color resolution:"), TEXT("")
};

class DEVICECAPS1 : public BaseWindow<DEVICECAPS1,Window,FrameWinTraits>
{
MyAbout about;
ComPtr<ID2D1DCRenderTarget> m_renderTarget;
ComPtr<ID2D1SolidColorBrush> m_blackbrush;
ComPtr<ID2D1Factory> m_factory;
ComPtr<IDWriteFactory> m_dwriteFactory;
ComPtr<IDWriteTextFormat> m_format;
int OnCreate(LPCREATESTRUCT lpCreateStruct);
void OnPaint(DC dc);
void OnAbout(UINT uNotifyCode, int nID, Window wndCtl);
void OnExit(UINT uNotifyCode, int nID, Window wndCtl);
void OnDestroy();
void OnClose();
HRESULT CreateDeviceIndependantResources();
HRESULT CreateDeviceResources();
public:
    HRESULT Initialize();
    DECLARE_WND_CLASS_1(IDC_DEVICECAPS1, IDI_SMALL)

     BEGIN_MSG_MAP()
                MSG_WM_CREATE(OnCreate)
                MSG_WM_CLOSE(OnClose)
                MSG_WM_PAINT(OnPaint)
                MSG_WM_DESTROY(OnDestroy)
        BEGIN_COMMAND_MAP()
                COMMAND_ID_HANDLER_EX(IDM_ABOUT, OnAbout)
                COMMAND_ID_HANDLER_EX(IDOK, OnAbout)
                COMMAND_ID_HANDLER_EX(IDM_EXIT, OnExit)
        END_COMMAND_MAP()
    END_MSG_MAP()
};