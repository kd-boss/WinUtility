#ifndef __BASEWINDOW_H__
#define __BASEWINDOW_H__
#ifndef STRICT
#define STRICT
#endif
#ifndef WINVER
#define WINVER 0x0601
#define NTDDI_VERSION 0x0601
#endif

// Windows includes
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <olectl.h>
#include <crtdbg.h>

// Drag in the standard library.
#include <type_traits>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <exception>
#include <stdlib.h>
#include <assert.h>
#include <comdef.h>
#ifndef __HRESULT_FROM_WIN32
#define __HRESULT_FROM_WIN32(x)                                                \
  ((HRESULT)(x) <= 0                                                           \
       ? ((HRESULT)(x))                                                        \
       : ((HRESULT)(((x)&0x0000FFFF) | (FACILITY_WIN32 << 16) | 0x80000000)))
#endif

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

#ifndef __HR__
#define __HR__
void HR(HRESULT hr) {
  if (hr != S_OK)
    throw _com_error(hr);
}
#endif

#ifndef _tcslen(x)
#if UNICODE
#define _tcslen(x) wcslen(x)
#else
#define _tcslen(x) strlen(x)
#endif
#endif

class URECT {
  LPRECT m_lpRect;

public:
  URECT() : m_lpRect(nullptr) {}
  URECT(std::initializer_list<int> ints) {

    auto a = ints.begin();
    if (m_lpRect == nullptr)
      m_lpRect = new RECT;

    m_lpRect->left = *a++;
    m_lpRect->top = *a++;
    m_lpRect->right = *a++;
    m_lpRect->bottom = *a++;
  }

  URECT(URECT &) = default;
  URECT(URECT &&) = default;
  URECT(LPRECT lpRect = nullptr) : m_lpRect(lpRect) {}
  URECT(RECT &rc) : m_lpRect(&rc) {}
  URECT &operator=(URECT &) = default;
  URECT &operator=(URECT &&) = default;
  URECT &operator=(LPRECT rc) {
    m_lpRect = rc;
    return *this;
  }
  URECT &operator=(RECT &rc) {
    m_lpRect = &rc;
    return *this;
  }
  LPRECT Get() { return m_lpRect; }
};

class UMenuOrID {
  HMENU m_hMenu;

public:
  UMenuOrID() : m_hMenu(nullptr) {}
  UMenuOrID(UMenuOrID &) = default;
  UMenuOrID(UMenuOrID &&) = default;
  UMenuOrID(HMENU hMenu) : m_hMenu(hMenu) {}
  UMenuOrID(UINT nID) : m_hMenu((HMENU)(UINT_PTR) nID) {}
  UMenuOrID &operator=(UMenuOrID &) = default;
  UMenuOrID &operator=(UMenuOrID &&) = default;
  UMenuOrID &operator=(UINT nID) {
    m_hMenu = (HMENU)(UINT_PTR) nID;
    return *this;
  }
  UMenuOrID &operator=(HMENU hMenu) {
    m_hMenu = hMenu;
    return *this;
  }
  HMENU Get() { return m_hMenu; }
};

class UStringOrID {
  LPCTSTR m_lpstr;

public:
  UStringOrID() : m_lpstr(nullptr) {}
  UStringOrID(UStringOrID &) = default;
  UStringOrID(UStringOrID &&) = default;
  UStringOrID(LPCTSTR lpString) : m_lpstr(lpString) {}
  UStringOrID(UINT nID) : m_lpstr(MAKEINTRESOURCE(nID)) {}
  UStringOrID &operator=(UStringOrID &) = default;
  UStringOrID &operator=(UStringOrID &&) = default;
  UStringOrID &operator=(LPCTSTR lpString) {
    m_lpstr = lpString;
    return *this;
  }
  UStringOrID &operator=(UINT nID) {
    m_lpstr = MAKEINTRESOURCE(nID);
    return *this;
  }
  LPCTSTR Get() { return m_lpstr; }
};

#ifdef SetWindowLongPtrA
#undef SetWindowLongPtrA
inline LONG_PTR SetWindowLongPtrA(_In_ HWND hWnd, _In_ int nIndex,
                                  _In_ LONG_PTR dwNewLong) {
  return (::SetWindowLongA(hWnd, nIndex, LONG(dwNewLong)));
}
#endif

#ifdef SetWindowLongPtrW
#undef SetWindowLongPtrW
inline LONG_PTR SetWindowLongPtrW(_In_ HWND hWnd, _In_ int nIndex,
                                  _In_ LONG_PTR dwNewLong) {
  return (::SetWindowLongW(hWnd, nIndex, LONG(dwNewLong)));
}
#endif

#ifdef GetWindowLongPtrA
#undef GetWindowLongPtrA
inline LONG_PTR GetWindowLongPtrA(_In_ HWND hWnd, _In_ int nIndex) {
  return (::GetWindowLongA(hWnd, nIndex));
}
#endif

#ifdef GetWindowLongPtrW
#undef GetWindowLongPtrW
inline LONG_PTR GetWindowLongPtrW(_In_ HWND hWnd, _In_ int nIndex) {
  return (::GetWindowLongW(hWnd, nIndex));
}
#endif

#ifdef SendMessageA
#undef SendMessageA
inline LRESULT SendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){
    return ::SendMessageA(hWnd,Msg,wParam, lParam);
}
#endif

#ifdef SendMessageW
#undef SendMessageW
inline LRESULT SendMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){
   return ::SendMessageW(hWnd,Msg,wParam,lParam);
}

#endif

namespace std {
typedef std::basic_string<TCHAR> tstring;
typedef std::basic_stringstream<TCHAR> tstringstream;
#ifdef UNICODE
template <typename T> tstring to_tstring(T t) { return to_wstring(t); }
#else
template <typename T> tstring to_tstring(T t) { return to_string(t); }
#endif
}

enum class ZOrderPlacement {
  Top = 0,         // HWND_TOP = ((HWND)0)
  Bottom = 1,      // HWND_BOTTOM = (HWND)1)
  TopMost = -1,    // HWND_TOPMOST = (HWND)-1)
  NonTopMost = -2, // HWND_NOTOPMOST = ((HWND)-2)
};

class Rect;
class Point;
class Size;

class Size : public tagSIZE {
public:
  Size();
  Size(Size &) = default;
  Size(Size &&) = default;
  Size(LONG x, LONG y);
  Size(LPARAM lParam);
  Size(SIZE size);
  BOOL operator==(SIZE other) const;
  BOOL operator!=(SIZE other) const;
  void operator+=(SIZE other);
  void operator-=(SIZE other);
  Size operator+(SIZE other) const;
  Size operator-(SIZE other) const;
  Size operator-() const;
  Point operator+(POINT other) const;
  Point operator-(POINT other) const;
  Rect operator+(const RECT *other) const;
  Rect operator-(const RECT *other) const;
  void SetSize(int CX, int CY);
};

class Point : public tagPOINT {
public:
  Point();
  Point(Point &) = default;
  Point(Point &&) = default;
  Point(LONG xx, LONG yy);
  Point(LPARAM lParam);
  void Offset(int OffsetX, int OffsetY);
  void Offset(POINT point);
  void SetPoint(int X, int Y);
  BOOL operator==(POINT other) const;
  BOOL operator!=(POINT other) const;
  void operator+=(SIZE size);
  void operator-=(SIZE size);
  void operator+=(POINT other);
  void operator-=(POINT other);
  Point operator+(SIZE size) const;
  Point operator-(SIZE size) const;
  Point operator+(POINT other) const;
  Point operator-(POINT other) const;
  Point operator-() const;
  Rect operator+(const RECT *pother) const;
  Rect operator-(const RECT *pother) const;
};

class Rect : public tagRECT {
public:
  Rect();
  Rect(Rect &) = default;
  Rect(Rect &&) = default;
  Rect(int l, int t, int r, int b);
  Rect(const RECT &r);
  Rect(LPCRECT lpr);
  Rect(POINT p, SIZE s);
  Rect(POINT topLeft, POINT bottomRight);
  int Width() const;
  int Height() const;
  Size Size() const;
  const Point &TopLeft() const;
  const Point &BottomRight() const;
  Point &TopLeft();
  Point &BottomRight();
  Point CenterPoint() const;
  void SwapLeftRight();
  static void WINAPI SwapLeftRight(LPRECT lpRect);
  operator LPRECT();
  operator LPCRECT();
  BOOL IsRectEmpty() const;
  BOOL IsRectnullptr() const;
  BOOL PtInRect(POINT point) const;
  void SetRect(int x1, int y1, int x2, int y2);
  void SetRect(POINT topLeft, POINT bottomRight);
  void SetRectEmpty();
  void CopyRect(LPCRECT lpRect);
  BOOL EqualRect(LPCRECT lpRect) const;
  void InflateRect(int x, int y);
  void InflateRect(SIZE size);
  void InflateRect(LPCRECT lpRect);
  void InflateRect(int l, int t, int r, int b);
  void DeflateRect(int x, int y);
  void DeflateRect(SIZE size);
  void DeflateRect(LPCRECT lpRect);
  void DeflateRect(int l, int t, int r, int b);
  void OffsetRect(int x, int y);
  void OffsetRect(SIZE size);
  void OffsetRect(POINT point);
  void NormalizeRect();
  void MoveToY(int y);
  void MoveToX(int x);
  void MoveToXY(int x, int y);
  void MoveToXY(POINT point);
  BOOL IntersectRect(LPCRECT lpRect1, LPCRECT lpRect2);
  BOOL UnionRect(LPCRECT lpRect1, LPCRECT lpRect2);
  BOOL SubtractRect(LPCRECT lpRect1, LPCRECT lpRect2);
  void operator=(const RECT &rc);
  BOOL operator==(const RECT &rc) const;
  BOOL operator!=(const RECT &rc) const;
  void operator+=(POINT point);
  void operator+=(SIZE size);
  void operator+=(LPCRECT lpRect);
  void operator-=(POINT point);
  void operator-=(SIZE size);
  void operator-=(LPCRECT lpRect);
  void operator&=(const RECT &rc);
  void operator|=(const RECT &rc);
  Rect operator+(POINT point) const;
  Rect operator-(POINT point) const;
  Rect operator+(LPCRECT lpRect) const;
  Rect operator+(SIZE size) const;
  Rect operator-(SIZE size) const;
  Rect operator-(LPCRECT lpRect) const;
  Rect operator&(const RECT &rc) const;
  Rect operator|(const RECT &rc) const;
  Rect MulDiv(int Multiplier, int Divisor) const;
};

Size::Size() {
  cx = 0;
  cy = 0;
}
Size::Size(LONG x, LONG y) {
  cx = x;
  cy = y;
}
Size::Size(LPARAM lParam) {
  cx = GET_X_LPARAM(lParam);
  cy = GET_Y_LPARAM(lParam);
}
Size::Size(SIZE size) {
  cx = size.cx;
  cy = size.cy;
}
inline BOOL Size::operator==(SIZE other) const {
  return (cx == other.cx && cy == other.cy);
}
inline BOOL Size::operator!=(SIZE other) const {
  return (cx != other.cx || cy != other.cy);
}
inline void Size::operator+=(SIZE other) {
  cx += other.cx;
  cy += other.cy;
}
inline void Size::operator-=(SIZE other) {
  cx -= other.cx;
  cy -= other.cy;
}
inline Size Size::operator+(SIZE other) const {
  return {cx + other.cx, cy + other.cy};
}
inline Size Size::operator-(SIZE other) const {
  return {cx - other.cx, cy - other.cy};
}
inline Size Size::operator-() const { return {-cx, -cy}; }
inline Point Size::operator+(POINT other) const {
  return Point{cx + other.x, cy + other.y};
}
inline Point Size::operator-(POINT other) const {
  return Point{cx - other.x, cy - other.y};
}
inline Rect Size::operator+(const RECT *other) const {
  return Rect(other) + *this;
}
inline Rect Size::operator-(const RECT *other) const {
  return Rect(other) - *this;
}

inline void Size::SetSize(int CX, int CY) {
  cx = CX;
  cy = CY;
}

inline Point::Point() {
  x = 0;
  y = 0;
}
inline Point::Point(LONG xx, LONG yy) {
  x = xx;
  y = yy;
}
inline Point::Point(LPARAM lParam) {
  x = GET_X_LPARAM(lParam);
  y = GET_Y_LPARAM(lParam);
}
inline void Point::Offset(int OffsetX, int OffsetY) {
  x += OffsetX;
  y += OffsetY;
}
inline void Point::Offset(POINT point) {
  x += point.x;
  y += point.y;
}
inline void Point::SetPoint(int X, int Y) {
  x = X;
  y = Y;
}
inline BOOL Point::operator==(POINT other) const {
  return (x == other.x && y == other.y);
}
inline BOOL Point::operator!=(POINT other) const {
  return (x != other.x || y != other.y);
}
inline void Point::operator+=(POINT other) {
  x += other.x;
  y += other.y;
}
inline void Point::operator-=(POINT other) {
  x -= other.x;
  y -= other.y;
}
inline Point Point::operator+(SIZE size) const {
  return Point{x + size.cx, y + size.cy};
}
inline Point Point::operator-(SIZE size) const {
  return Point{x - size.cx, y - size.cy};
}
inline Point Point::operator-() const { return {-x, -y}; }
inline Point Point::operator+(POINT other) const {
  return {x + other.x, y + other.y};
}
inline Point Point::operator-(POINT other) const {
  return {x - other.x, y - other.y};
}
inline Rect Point::operator+(const RECT *pother) const {
  return Rect{pother} + *this;
}
inline Rect Point::operator-(const RECT *pother) const {
  return Rect{pother} + *this;
}

inline Rect::Rect() {
  left = 0;
  top = 0;
  right = 0;
  bottom = 0;
}
inline Rect::Rect(int l, int t, int r, int b) {
  left = l;
  top = t;
  right = r;
  bottom = b;
}
inline Rect::Rect(const RECT &rc) { ::CopyRect(this, &rc); }
inline Rect::Rect(LPCRECT lpr) { ::CopyRect(this, lpr); }
inline Rect::Rect(POINT p, SIZE s) {
  right = (left = p.y) + s.cx;
  bottom = (top = p.y) + s.cy;
}
inline Rect::Rect(POINT topLeft, POINT bottomRight) {
  left = topLeft.x;
  top = topLeft.y;
  right = bottomRight.x;
  bottom = bottomRight.y;
}
inline int Rect::Width() const { return right - left; }
inline int Rect::Height() const { return bottom - top; }
inline Size Rect::Size() const { return {right - left, bottom - top}; }
inline const Point &Rect::TopLeft() const { return *((Point *)this); }
inline const Point &Rect::BottomRight() const { return *((Point *)this + 1); }
inline Point &Rect::TopLeft() { return *((Point *)this); }
inline Point &Rect::BottomRight() { return *((Point *)this + 1); }
inline Point Rect::CenterPoint() const {
  return {(left + right) / 2, (top + bottom) / 2};
}
inline void Rect::SwapLeftRight() { SwapLeftRight(LPRECT(this)); }
inline void WINAPI Rect::SwapLeftRight(LPRECT lpRect) {
  LONG temp = lpRect->left;
  lpRect->left = lpRect->right;
  lpRect->right = temp;
}
inline Rect::operator LPRECT() { return this; }
inline Rect::operator LPCRECT() { return this; }
inline BOOL Rect::IsRectEmpty() const { return ::IsRectEmpty(this); }
inline BOOL Rect::IsRectnullptr() const {
  return (left == 0 && right == 0 && top == 0 && bottom == 0);
}
inline BOOL Rect::PtInRect(POINT point) const {
  return ::PtInRect(this, point);
}
inline void Rect::SetRect(int x1, int y1, int x2, int y2) {
  ::SetRect(this, x1, y1, x2, y2);
}
inline void Rect::SetRect(POINT topLeft, POINT bottomRight) {
  ::SetRect(this, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
}
inline void Rect::SetRectEmpty() { ::SetRectEmpty(this); }
inline void Rect::CopyRect(LPCRECT lpRect) { ::CopyRect(this, lpRect); }
inline BOOL Rect::EqualRect(LPCRECT lpRect) const {
  return ::EqualRect(this, lpRect);
}
inline void Rect::InflateRect(int x, int y) { ::InflateRect(this, x, y); }
inline void Rect::InflateRect(SIZE size) {
  ::InflateRect(this, size.cx, size.cy);
}
inline void Rect::InflateRect(LPCRECT lpRect) {
  left -= lpRect->left;
  top -= lpRect->top;
  right += lpRect->right;
  bottom += lpRect->bottom;
}
inline void Rect::InflateRect(int l, int t, int r, int b) {
  left -= l;
  top -= t;
  right += r;
  bottom += b;
}
inline void Rect::DeflateRect(int x, int y) { ::InflateRect(this, -x, -y); }
inline void Rect::DeflateRect(SIZE size) {
  ::InflateRect(this, -size.cx, -size.cy);
}
inline void Rect::DeflateRect(LPCRECT lpRect) {
  left += lpRect->left;
  top += lpRect->top;
  right -= lpRect->right;
  bottom -= lpRect->bottom;
}
inline void Rect::DeflateRect(int l, int t, int r, int b) {
  left += l;
  top += t;
  right -= r;
  bottom -= b;
}
inline void Rect::OffsetRect(int x, int y) { ::OffsetRect(this, x, y); }
inline void Rect::OffsetRect(SIZE size) {
  ::OffsetRect(this, size.cx, size.cy);
}
inline void Rect::OffsetRect(POINT point) {
  ::OffsetRect(this, point.x, point.y);
}
inline void Rect::NormalizeRect() {
  int tmp;
  if (left > right) {
    tmp = left;
    left = right;
    right = tmp;
  }
  if (top > bottom) {
    tmp = top;
    top = bottom;
    bottom = tmp;
  }
}
inline void Rect::MoveToY(int y) {
  bottom = Height() + y;
  top = y;
}
inline void Rect::MoveToX(int x) {
  right = Width() + x;
  left = x;
}
inline void Rect::MoveToXY(int x, int y) {
  MoveToX(x);
  MoveToY(y);
}
inline void Rect::MoveToXY(POINT point) {
  MoveToX(point.x);
  MoveToY(point.y);
}
inline BOOL Rect::IntersectRect(LPCRECT lpRect1, LPCRECT lpRect2) {
  return ::IntersectRect(this, lpRect1, lpRect2);
}
inline BOOL Rect::UnionRect(LPCRECT lpRect1, LPCRECT lpRect2) {
  return ::UnionRect(this, lpRect1, lpRect2);
}
inline BOOL Rect::SubtractRect(LPCRECT lpRect1, LPCRECT lpRect2) {
  return ::SubtractRect(this, lpRect1, lpRect2);
}
inline void Rect::operator=(const RECT &rc) { ::CopyRect(this, &rc); }
inline BOOL Rect::operator==(const RECT &rc) const {
  return ::EqualRect(this, &rc);
}
inline BOOL Rect::operator!=(const RECT &rc) const {
  return !::EqualRect(this, &rc);
}
inline void Rect::operator+=(POINT point) {
  ::OffsetRect(this, point.x, point.y);
}
inline void Rect::operator+=(SIZE size) {
  ::OffsetRect(this, size.cx, size.cy);
}
inline void Rect::operator+=(LPCRECT lpRect) { InflateRect(lpRect); }
inline void Rect::operator-=(POINT point) {
  ::OffsetRect(this, -point.x, -point.y);
}
inline void Rect::operator-=(SIZE size) {
  ::OffsetRect(this, -size.cx, -size.cy);
}
inline void Rect::operator-=(LPCRECT lpRect) { DeflateRect(lpRect); }
inline void Rect::operator&=(const RECT &rc) {
  ::IntersectRect(this, this, &rc);
}
inline void Rect::operator|=(const RECT &rc) { ::UnionRect(this, this, &rc); }
inline Rect Rect::operator+(POINT point) const {
  Rect rc(*this);
  ::OffsetRect(&rc, point.x, point.y);
  return rc;
}
inline Rect Rect::operator-(POINT point) const {
  Rect rc(*this);
  ::OffsetRect(&rc, -point.x, -point.y);
  return rc;
}
inline Rect Rect::operator+(LPCRECT lpRect) const {
  Rect rc(this);
  rc.InflateRect(lpRect);
  return rc;
}
inline Rect Rect::operator+(SIZE size) const {
  Rect rc(*this);
  ::OffsetRect(&rc, size.cx, size.cy);
  return rc;
}
inline Rect Rect::operator-(SIZE size) const {
  Rect rc(*this);
  ::OffsetRect(&rc, -size.cx, -size.cy);
  return rc;
}
inline Rect Rect::operator-(LPCRECT lpRect) const {
  Rect rc(this);
  rc.DeflateRect(lpRect);
  return rc;
}
inline Rect Rect::operator&(const RECT &rc) const {
  Rect rcn;
  ::IntersectRect(&rcn, this, &rc);
  return rcn;
}
inline Rect Rect::operator|(const RECT &rc) const {
  Rect rcn;
  ::UnionRect(&rcn, this, &rc);
  return rcn;
}
inline Rect Rect::MulDiv(int Multiplier, int Divisor) const {
  return {::MulDiv(left, Multiplier, Divisor),
          ::MulDiv(top, Multiplier, Divisor),
          ::MulDiv(right, Multiplier, Divisor),
          ::MulDiv(bottom, Multiplier, Divisor)};
}

inline void WINCHECK(HWND hWnd) {
  if (!::IsWindow(hWnd)) {
    MessageBox(nullptr, TEXT("Function Called on an Improperly Created Window "
                             "Class. Ensure that Window::Create is called, and "
                             "succeeds to prevent this assertion."),
               TEXT("Runtime Assertion! Invalid Window Handle (HWND)"),
               MB_OK | MB_ICONERROR);
    ExitProcess(ERROR_INVALID_WINDOW_HANDLE);
  }
}

class WINASSERT {
public:
  WINASSERT(bool val) {
    if (!val) {
      MessageBox(nullptr, TEXT("DEBUG assertion Failed"), TEXT("BaseWindow"),
                 MB_OK | MB_ICONERROR);
      ExitProcess(ERROR_INVALID_WINDOW_HANDLE);
    }
  }
};

class WINTRACE {
public:
  WINTRACE(bool val, LPTSTR text) {
    if (!val) {
      ::MessageBox(nullptr, text, TEXT("Runtime Trace: "),
                   MB_OK | MB_ICONINFORMATION);
    }
  }
};

#ifndef GDISTUFF

#ifdef _INC_WINDOWSX
#undef CopyRgn
#undef CreateBrush
#undef CreatePen
#undef SelectBrush
#undef SelectPen
#undef SelectFont
#undef SelectBitmap
#endif

inline LPBITMAPINFOHEADER GetBitmapResourceInfo(HMODULE hModule,
                                                UStringOrID image) {

  HRSRC hResource = ::FindResource(hModule, image.Get(), RT_BITMAP);
  WINASSERT(hResource != nullptr);
  HGLOBAL hGlobal = ::LoadResource(hModule, hResource);
  WINASSERT(hGlobal != nullptr);
  LPBITMAPINFOHEADER lpBitmapInfoHeader =
      (LPBITMAPINFOHEADER)::LockResource(hGlobal);
  WINASSERT(lpBitmapInfoHeader != nullptr);
  return lpBitmapInfoHeader;
}

inline WORD GetBitmapResourceBitsPerPixel(HMODULE hModule, UStringOrID image) {
  LPBITMAPINFOHEADER lpBitmapInfoHeader = GetBitmapResourceInfo(hModule, image);
  return lpBitmapInfoHeader->biBitCount;
}

template <bool Managed> class PenT {
  HPEN m_hpen;
  PenT(HPEN hPen = nullptr) : m_hpen(hPen) {}

  ~PenT() {
    if (Managed && m_hpen != nullptr)
      DeleteObject();
  }

  PenT<Managed> &operator=(HPEN hPen) {
    Attach(hPen);
    return *this;
  }

  void Attach(HPEN hPen) {
    if (Managed && m_hpen != nullptr && m_hpen != hPen)
      ::DeleteObject(m_hpen);
    m_hpen = hPen;
  }

  HPEN Detach() {
    HPEN hPen = m_hpen;
    m_hpen = nullptr;
    return hPen;
  }

  HPEN CreatePen(int iPenStyle, int iWidth, COLORREF crColor) {
    WINASSERT(m_hpen == nullptr);
    m_hpen = ::CreatePen(iPenStyle, iWidth, crColor);
    return m_hpen;
  }

  HPEN CreatePen(int iPenStyle, const LOGBRUSH *pLogBrush, int iWidth,
                 int iStyleCount, const DWORD *lpStyle = nullptr) {
    WINASSERT(m_hpen == nullptr);
    m_hpen = ::ExtCreatePen(iPenStyle, iWidth, pLogBrush, iStyleCount, lpStyle);
    return m_hpen;
  }

  HPEN CreatePenIndirect(LPLOGPEN lpLogPen) {
    WINASSERT(m_hpen == nullptr);
    m_hpen = ::CreatePenIndirect(lpLogPen);
    return m_hpen;
  }

  BOOL DeleteObject() {
    WINASSERT(m_hpen != nullptr);
    BOOL bRet = ::DeleteObject(m_hpen);
    if (bRet)
      m_hpen = nullptr;
    return bRet;
  }

  int GetLogPen(LOGPEN *pLogPen) const {
    WINASSERT(m_hpen != nullptr);
    return ::GetObject(m_hpen, sizeof(LOGPEN), pLogPen);
  }

  bool GetLogPen(LOGPEN &LogPen) const {
    WINASSERT(m_hpen != nullptr);
    return (::GetObject(m_hpen, sizeof(LOGPEN), &LogPen) == sizeof(LOGPEN));
  }
};

typedef PenT<false> PenHandle;
typedef PenT<true> Pen;

template <bool Managed> class BrushT {
public:
  // Data members
  HBRUSH m_brush;

  // Constructor/destructor/operators
  BrushT(HBRUSH hBrush = nullptr) : m_brush(hBrush) {}

  ~BrushT() {
    if (Managed && m_brush != nullptr)
      DeleteObject();
  }

  BrushT<Managed> &operator=(HBRUSH hBrush) {
    Attach(hBrush);
    return *this;
  }

  void Attach(HBRUSH hBrush) {
    if (Managed && m_brush != nullptr && m_brush != hBrush)
      ::DeleteObject(m_brush);
    m_brush = hBrush;
  }

  HBRUSH Detach() {
    HBRUSH hBrush = m_brush;
    m_brush = nullptr;
    return hBrush;
  }

  operator HBRUSH() const { return m_brush; }

  bool IsNull() const { return (m_brush == nullptr); }

  // Create methods
  HBRUSH CreateSolidBrush(COLORREF crColor) {
    WINASSERT(m_brush == nullptr);
    m_brush = ::CreateSolidBrush(crColor);
    return m_brush;
  }

#ifndef _WIN32_WCE
  HBRUSH CreateHatchBrush(int nIndex, COLORREF crColor) {
    WINASSERT(m_brush == nullptr);
    m_brush = ::CreateHatchBrush(nIndex, crColor);
    return m_brush;
  }
#endif // !_WIN32_WCE

#if !defined(_WIN32_WCE) || (_ATL_VER >= 0x0800)
  HBRUSH CreateBrushIndirect(const LOGBRUSH *lpLogBrush) {
    WINASSERT(m_brush == nullptr);
#ifndef _WIN32_WCE
    m_brush = ::CreateBrushIndirect(lpLogBrush);
#else  // CE specific
    m_brush = ATL::CreateBrushIndirect(lpLogBrush);
#endif // _WIN32_WCE
    return m_brush;
  }
#endif // !defined(_WIN32_WCE) || (_ATL_VER >= 0x0800)

  HBRUSH CreatePatternBrush(HBITMAP hBitmap) {
    WINASSERT(m_brush == nullptr);
    m_brush = ::CreatePatternBrush(hBitmap);
    return m_brush;
  }

  HBRUSH CreateDIBPatternBrush(HGLOBAL hPackedDIB, UINT nUsage) {
    WINASSERT(hPackedDIB != nullptr);
    const void *lpPackedDIB = GlobalLock(hPackedDIB);
    WINASSERT(lpPackedDIB != nullptr);
    m_brush = ::CreateDIBPatternBrushPt(lpPackedDIB, nUsage);
    GlobalUnlock(hPackedDIB);
    return m_brush;
  }

  HBRUSH CreateDIBPatternBrush(const void *lpPackedDIB, UINT nUsage) {
    WINASSERT(m_brush == nullptr);
    m_brush = ::CreateDIBPatternBrushPt(lpPackedDIB, nUsage);
    return m_brush;
  }

  HBRUSH CreateSysColorBrush(int nIndex) {
    WINASSERT(m_brush == nullptr);
    m_brush = ::GetSysColorBrush(nIndex);
    return m_brush;
  }

  BOOL DeleteObject() {
    WINASSERT(m_brush != nullptr);
    BOOL bRet = ::DeleteObject(m_brush);
    if (bRet)
      m_brush = nullptr;
    return bRet;
  }

  // Attributes
  int GetLogBrush(LOGBRUSH *pLogBrush) const {
    WINASSERT(m_brush != nullptr);
    return ::GetObject(m_brush, sizeof(LOGBRUSH), pLogBrush);
  }

  bool GetLogBrush(LOGBRUSH &LogBrush) const {
    WINASSERT(m_brush != nullptr);
    return (::GetObject(m_brush, sizeof(LOGBRUSH), &LogBrush) ==
            sizeof(LOGBRUSH));
  }
};

typedef BrushT<false> BrushHandle;
typedef BrushT<true> Brush;

class LogFont : public LOGFONT {
public:
  LogFont() { memset(this, 0, sizeof(LogFont)); }

  LogFont(const LogFont &lf) { Copy(&lf); }

  LogFont(HFONT hFont) {
    WINASSERT(::GetObjectType(hFont) == OBJ_FONT);
    ::GetObject(hFont, sizeof(LogFont), (LogFont *)this);
  }

  HFONT CreateFontIndirect() { return ::CreateFontIndirect(this); }

  void SetBold() { lfWeight = FW_BOLD; }

  bool IsBold() const { return (lfWeight >= FW_BOLD); }

  void MakeBolder(int iScale = 1) { lfWeight += FW_BOLD * iScale; }

  void MakeLarger(int iScale) {
    if (lfHeight > 0)
      lfHeight += iScale;
    else
      lfHeight -= iScale;
  }

  void SetHeight(LONG nPointSize, HDC hDC = nullptr) {
    HDC hDC1 = (hDC != nullptr) ? hDC : ::GetDC(nullptr);
    // For MM_TEXT mapping mode
    lfHeight = -::MulDiv(nPointSize, ::GetDeviceCaps(hDC1, LOGPIXELSY), 72);
    if (hDC == nullptr)
      ::ReleaseDC(nullptr, hDC1);
  }

  LONG GetHeight(HDC hDC = nullptr) const {
    HDC hDC1 = (hDC != nullptr) ? hDC : ::GetDC(nullptr);
    // For MM_TEXT mapping mode
    LONG nPointSize =
        ::MulDiv(-lfHeight, 72, ::GetDeviceCaps(hDC1, LOGPIXELSY));
    if (hDC == nullptr)
      ::ReleaseDC(nullptr, hDC1);

    return nPointSize;
  }

  LONG GetDeciPointHeight(HDC hDC = nullptr) const {
    HDC hDC1 = (hDC != nullptr) ? hDC : ::GetDC(nullptr);
#ifndef _WIN32_WCE
    POINT ptOrg = {0, 0};
    ::DPtoLP(hDC1, &ptOrg, 1);
    POINT pt = {0, 0};
    pt.y = abs(lfHeight) + ptOrg.y;
    ::LPtoDP(hDC1, &pt, 1);
    LONG nDeciPoint =
        ::MulDiv(pt.y, 720,
                 ::GetDeviceCaps(
                     hDC1, LOGPIXELSY)); // 72 points/inch, 10 decipoints/point
#else                                    // CE specific
    // DP and LP are always the same on CE
    LONG nDeciPoint =
        ::MulDiv(abs(lfHeight), 720,
                 ::GetDeviceCaps(
                     hDC1, LOGPIXELSY)); // 72 points/inch, 10 decipoints/point
#endif                                   // _WIN32_WCE
    if (hDC == nullptr)
      ::ReleaseDC(nullptr, hDC1);

    return nDeciPoint;
  }

  void SetHeightFromDeciPoint(LONG nDeciPtHeight, HDC hDC = nullptr) {
    HDC hDC1 = (hDC != nullptr) ? hDC : ::GetDC(nullptr);
#ifndef _WIN32_WCE
    POINT pt = {0, 0};
    pt.y = ::MulDiv(::GetDeviceCaps(hDC1, LOGPIXELSY), nDeciPtHeight,
                    720); // 72 points/inch, 10 decipoints/point
    ::DPtoLP(hDC1, &pt, 1);
    POINT ptOrg = {0, 0};
    ::DPtoLP(hDC1, &ptOrg, 1);
    lfHeight = -abs(pt.y - ptOrg.y);
#else  // CE specific
    // DP and LP are always the same on CE
    lfHeight = -abs(::MulDiv(::GetDeviceCaps(hDC1, LOGPIXELSY), nDeciPtHeight,
                             720)); // 72 points/inch, 10 decipoints/point
#endif // _WIN32_WCE
    if (hDC == nullptr)
      ::ReleaseDC(nullptr, hDC1);
  }

  void SetCaptionFont() {
    NONCLIENTMETRICS ncm;
    memset(&ncm, 0, sizeof(ncm));
    WINASSERT(
        ::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0));
    Copy(&ncm.lfCaptionFont);
  }

  void SetMenuFont() {
    NONCLIENTMETRICS ncm;
    memset(&ncm, 0, sizeof(ncm));
    WINASSERT(
        ::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0));
    Copy(&ncm.lfMenuFont);
  }

  void SetStatusFont() {
    NONCLIENTMETRICS ncm;
    memset(&ncm, 0, sizeof(ncm));
    WINASSERT(
        ::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0));
    Copy(&ncm.lfStatusFont);
  }

  void SetMessageBoxFont() {
    NONCLIENTMETRICS ncm;
    memset(&ncm, 0, sizeof(ncm));
    WINASSERT(
        ::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0));
    Copy(&ncm.lfMessageFont);
  }

  void Copy(const LOGFONT *pGDILogFont) {
    WINASSERT(pGDILogFont != nullptr);
    *(LOGFONT *)this = *pGDILogFont;
  }

  LogFont &operator=(const LogFont &src) {
    Copy(&src);
    return *this;
  }

  LogFont &operator=(const LOGFONT &src) {
    Copy(&src);
    return *this;
  }

  LogFont &operator=(HFONT hFont) {
    WINASSERT(::GetObjectType(hFont) == OBJ_FONT);
    ::GetObject(hFont, sizeof(LogFont), (LogFont *)this);
    return *this;
  }

  bool operator==(const LOGFONT &logfont) const {
    return (logfont.lfHeight == lfHeight && logfont.lfWidth == lfWidth &&
            logfont.lfEscapement == lfEscapement &&
            logfont.lfOrientation == lfOrientation &&
            logfont.lfWeight == lfWeight && logfont.lfItalic == lfItalic &&
            logfont.lfUnderline == lfUnderline &&
            logfont.lfStrikeOut == lfStrikeOut &&
            logfont.lfCharSet == lfCharSet &&
            logfont.lfOutPrecision == lfOutPrecision &&
            logfont.lfClipPrecision == lfClipPrecision &&
            logfont.lfQuality == lfQuality &&
            logfont.lfPitchAndFamily == lfPitchAndFamily &&
            lstrcmp(logfont.lfFaceName, lfFaceName) == 0);
  }
};

template <bool Managed> class FontT {
public:
  HFONT m_font;

  FontT(HFONT hFont = nullptr) : m_font(hFont) {}

  ~FontT() {
    if (Managed && m_font != nullptr)
      DeleteObject();
  }

  FontT<Managed> &operator=(HFONT hFont) {
    Attach(hFont);
    return *this;
  }

  void Attach(HFONT hFont) {
    if (Managed && m_font != nullptr && m_font != hFont)
      ::DeleteObject(m_font);
    m_font = hFont;
  }

  HFONT Detach() {
    HFONT hFont = m_font;
    m_font = nullptr;
    return hFont;
  }

  operator HFONT() const { return m_font; }

  bool IsNull() const { return (m_font == nullptr); }

  HFONT CreateFontIndirect(const LogFont *lpGDILogFont) {
    WINASSERT(m_font == nullptr);
    m_font = ::CreateFontIndirect(lpGDILogFont);
    return m_font;
  }

  HFONT CreateFontIndirectEx(CONST ENUMLOGFONTEXDV *penumlfex) {
    WINASSERT(m_font == nullptr);
    m_font = ::CreateFontIndirectEx(penumlfex);
    return m_font;
  }

  HFONT CreateFont(int nHeight, int nWidth, int nEscapement, int nOrientation,
                   int nWeight, BYTE bItalic, BYTE bUnderline, BYTE cStrikeOut,
                   BYTE nCharSet, BYTE nOutPrecision, BYTE nClipPrecision,
                   BYTE nQuality, BYTE nPitchAndFamily, LPCTSTR lpszFacename) {
    WINASSERT(m_font == nullptr);

    m_font =
        ::CreateFont(nHeight, nWidth, nEscapement, nOrientation, nWeight,
                     bItalic, bUnderline, cStrikeOut, nCharSet, nOutPrecision,
                     nClipPrecision, nQuality, nPitchAndFamily, lpszFacename);
    return m_font;
  }

  HFONT CreatePointFont(int nPointSize, LPCTSTR lpszFaceName, HDC hDC = nullptr,
                        bool bBold = false, bool bItalic = false) {
    LOGFONT LFont = {0};
    LFont.lfCharSet = DEFAULT_CHARSET;
    LFont.lfHeight = nPointSize;

    if (_countof(LFont.lfFaceName) >= _tcslen(lpszFaceName)) {
#ifdef UNICODE
      wcsncpy_s(LFont.lfFaceName, _countof(LFont.lfFaceName), lpszFaceName,
                _tcslen(lpszFaceName));
#else
      strncpy_s(LFont.lfFaceName, _countof(LFont.lfFaceName), lpszFaceName,
                _tcslen(lpszFaceName));
#endif
    } else {
#ifdef UNICODE
      wcsncpy_s(LFont.lfFaceName, _countof(LFont.lfFaceName), lpszFaceName,
                _countof(LFont.lfFaceName));
#else
      strncpy_s(LFont.lfFaceName, _countof(LFont.lfFaceName), lpszFaceName,
                _countof(LFont.lfFaceName));
#endif
    }

    if (bBold)
      LFont.lfWeight = FW_BOLD;
    if (bItalic)
      LFont.lfItalic = (BYTE)TRUE;

    return CreatePointFontIndirect(&LFont, hDC);
  }

  HFONT CreatePointFontIndirect(const LOGFONT *lpGDILogFont,
                                HDC hDC = nullptr) {
    HDC hDC1 = (hDC != nullptr) ? hDC : ::GetDC(nullptr);
    LOGFONT font = *lpGDILogFont;
    POINT pt = {0, 0};
    pt.y = ::MulDiv(::GetDeviceCaps(hDC1, LOGPIXELSY), font.lfHeight,
                    720); // 72 points/inch, 10 decipoints/point
    ::DPtoLP(hDC1, &pt, 1);
    POINT ptOrg = {0, 0};
    ::DPtoLP(hDC1, &ptOrg, 1);
    font.lfHeight = -abs(pt.y - ptOrg.y);

    if (hDC == nullptr)
      ::ReleaseDC(nullptr, hDC1);

    return CreateFontIndirect(&font);
  }

  BOOL DeleteObject() {
    WINASSERT(m_font != nullptr);
    BOOL bRet = ::DeleteObject(m_font);
    if (bRet)
      m_font = nullptr;
    return bRet;
  }

  int GetLogFont(LOGFONT *pGDILogFont) const {
    WINASSERT(m_font != nullptr);
    return ::GetObject(m_font, sizeof(LogFont), pGDILogFont);
  }

  bool GetLogFont(LOGFONT &GDILogFont) const {
    WINASSERT(m_font != nullptr);
    return (::GetObject(m_font, sizeof(GDILogFont), &GDILogFont) ==
            sizeof(GDILogFont));
  }
};

typedef FontT<false> GDIFontHandle;
typedef FontT<true> GDIFont;

template <bool Managed> class BitmapT {
public:
  HBITMAP m_bitmap;

  BitmapT(HBITMAP hBitmap = nullptr) : m_bitmap(hBitmap) {}

  ~BitmapT() {
    if (Managed && m_bitmap != nullptr)
      DeleteObject();
  }

  BitmapT<Managed> &operator=(HBITMAP hBitmap) {
    Attach(hBitmap);
    return *this;
  }

  void Attach(HBITMAP hBitmap) {
    if (Managed && m_bitmap != nullptr && m_bitmap != hBitmap)
      ::DeleteObject(m_bitmap);
    m_bitmap = hBitmap;
  }

  HBITMAP Detach() {
    HBITMAP hBitmap = m_bitmap;
    m_bitmap = nullptr;
    return hBitmap;
  }

  operator HBITMAP() const { return m_bitmap; }

  bool IsNull() const { return (m_bitmap == nullptr); }

  HBITMAP LoadBitmap(HMODULE hResource, UStringOrID bitmap) {
    WINASSERT(m_bitmap == nullptr);
    m_bitmap = ::LoadBitmap(hResource, bitmap.Get());
    return m_bitmap;
  }

  HBITMAP LoadOEMBitmap(UINT nIDBitmap) {
    WINASSERT(m_bitmap == nullptr);
    m_bitmap = ::LoadBitmap(nullptr, MAKEINTRESOURCE(nIDBitmap));
    return m_bitmap;
  }

  HBITMAP LoadMappedBitmap(HMODULE hResource, UINT nIDBitmap, UINT nFlags = 0,
                           LPCOLORMAP lpColorMap = nullptr, int nMapSize = 0) {
    WINASSERT(m_bitmap == nullptr);
    m_bitmap = ::CreateMappedBitmap(hResource, nIDBitmap, (WORD)nFlags,
                                    lpColorMap, nMapSize);
    return m_bitmap;
  }

  HBITMAP CreateBitmap(int nWidth, int nHeight, UINT nPlanes,
                       UINT nBitsPerPixel, const void *lpBits) {
    WINASSERT(m_bitmap == nullptr);
    m_bitmap = ::CreateBitmap(nWidth, nHeight, nPlanes, nBitsPerPixel, lpBits);
    return m_bitmap;
  }

  HBITMAP CreateBitmapIndirect(LPBITMAP lpBitmap) {
    WINASSERT(m_bitmap == nullptr);
    m_bitmap = ::CreateBitmapIndirect(lpBitmap);
    return m_bitmap;
  }

  HBITMAP CreateCompatibleBitmap(HDC hDC, int nWidth, int nHeight) {
    WINASSERT(m_bitmap == nullptr);
    m_bitmap = ::CreateCompatibleBitmap(hDC, nWidth, nHeight);
    return m_bitmap;
  }

  HBITMAP CreateDiscardableBitmap(HDC hDC, int nWidth, int nHeight) {
    WINASSERT(m_bitmap == nullptr);
    m_bitmap = ::CreateDiscardableBitmap(hDC, nWidth, nHeight);
    return m_bitmap;
  }

  BOOL DeleteObject() {
    WINASSERT(m_bitmap != nullptr);
    BOOL bRet = ::DeleteObject(m_bitmap);
    if (bRet)
      m_bitmap = nullptr;
    return bRet;
  }

  int GetBitmap(BITMAP *pBitMap) const {
    WINASSERT(m_bitmap != nullptr);
    return ::GetObject(m_bitmap, sizeof(BITMAP), pBitMap);
  }

  bool GetBitmap(BITMAP &bm) const {
    WINASSERT(m_bitmap != nullptr);
    return (::GetObject(m_bitmap, sizeof(BITMAP), &bm) == sizeof(BITMAP));
  }

  bool GetSize(SIZE &size) const {
    WINASSERT(m_bitmap != nullptr);
    BITMAP bm = {0};
    if (!GetBitmap(&bm))
      return false;
    size.cx = bm.bmWidth;
    size.cy = bm.bmHeight;
    return true;
  }

  DWORD GetBitmapBits(DWORD dwCount, LPVOID lpBits) const {
    WINASSERT(m_bitmap != nullptr);
    return ::GetBitmapBits(m_bitmap, dwCount, lpBits);
  }

  DWORD SetBitmapBits(DWORD dwCount, const void *lpBits) {
    WINASSERT(m_bitmap != nullptr);
    return ::SetBitmapBits(m_bitmap, dwCount, lpBits);
  }

  BOOL GetBitmapDimension(LPSIZE lpSize) const {
    WINASSERT(m_bitmap != nullptr);
    return ::GetBitmapDimensionEx(m_bitmap, lpSize);
  }

  BOOL SetBitmapDimension(int nWidth, int nHeight, LPSIZE lpSize = nullptr) {
    WINASSERT(m_bitmap != nullptr);
    return ::SetBitmapDimensionEx(m_bitmap, nWidth, nHeight, lpSize);
  }

  HBITMAP CreateDIBitmap(HDC hDC, CONST BITMAPINFOHEADER *lpbmih, DWORD dwInit,
                         CONST VOID *lpbInit, CONST BITMAPINFO *lpbmi,
                         UINT uColorUse) {
    WINASSERT(m_bitmap == nullptr);
    m_bitmap = ::CreateDIBitmap(hDC, lpbmih, dwInit, lpbInit, lpbmi, uColorUse);
    return m_bitmap;
  }

  HBITMAP CreateDIBSection(HDC hDC, CONST BITMAPINFO *lpbmi, UINT uColorUse,
                           VOID **ppvBits, HANDLE hSection, DWORD dwOffset) {
    WINASSERT(m_bitmap == nullptr);
    m_bitmap =
        ::CreateDIBSection(hDC, lpbmi, uColorUse, ppvBits, hSection, dwOffset);
    return m_bitmap;
  }

  int GetDIBits(HDC hDC, UINT uStartScan, UINT cScanLines, LPVOID lpvBits,
                LPBITMAPINFO lpbmi, UINT uColorUse) const {
    WINASSERT(m_bitmap != nullptr);
    return ::GetDIBits(hDC, m_bitmap, uStartScan, cScanLines, lpvBits, lpbmi,
                       uColorUse);
  }

  int SetDIBits(HDC hDC, UINT uStartScan, UINT cScanLines, CONST VOID *lpvBits,
                CONST BITMAPINFO *lpbmi, UINT uColorUse) {
    WINASSERT(m_bitmap != nullptr);
    return ::SetDIBits(hDC, m_bitmap, uStartScan, cScanLines, lpvBits, lpbmi,
                       uColorUse);
  }
};

typedef BitmapT<false> BitmapHandle;
typedef BitmapT<true> Bitmap;

template <bool Managed> class PaletteT {
public:
  HPALETTE m_palette;

  PaletteT(HPALETTE hPalette = nullptr) : m_palette(hPalette) {}

  ~PaletteT() {
    if (Managed && m_palette != nullptr)
      DeleteObject();
  }

  PaletteT<Managed> &operator=(HPALETTE hPalette) {
    Attach(hPalette);
    return *this;
  }

  void Attach(HPALETTE hPalette) {
    if (Managed && m_palette != nullptr && m_palette != hPalette)
      ::DeleteObject(m_palette);
    m_palette = hPalette;
  }

  HPALETTE Detach() {
    HPALETTE hPalette = m_palette;
    m_palette = nullptr;
    return hPalette;
  }

  operator HPALETTE() const { return m_palette; }

  bool IsNull() const { return (m_palette == nullptr); }

  HPALETTE CreatePalette(LPLOGPALETTE lpLogPalette) {
    WINASSERT(m_palette == nullptr);
    m_palette = ::CreatePalette(lpLogPalette);
    return m_palette;
  }

  HPALETTE CreateHalftonePalette(HDC hDC) {
    WINASSERT(m_palette == nullptr);
    WINASSERT(hDC != nullptr);
    m_palette = ::CreateHalftonePalette(hDC);
    return m_palette;
  }

  BOOL DeleteObject() {
    WINASSERT(m_palette != nullptr);
    BOOL bRet = ::DeleteObject(m_palette);
    if (bRet)
      m_palette = nullptr;
    return bRet;
  }

  int GetEntryCount() const {
    WINASSERT(m_palette != nullptr);
    WORD nEntries = 0;
    ::GetObject(m_palette, sizeof(WORD), &nEntries);
    return (int)nEntries;
  }

  UINT GetPaletteEntries(UINT nStartIndex, UINT nNumEntries,
                         LPPALETTEENTRY lpPaletteColors) const {
    WINASSERT(m_palette != nullptr);
    return ::GetPaletteEntries(m_palette, nStartIndex, nNumEntries,
                               lpPaletteColors);
  }

  UINT SetPaletteEntries(UINT nStartIndex, UINT nNumEntries,
                         LPPALETTEENTRY lpPaletteColors) {
    WINASSERT(m_palette != nullptr);
    return ::SetPaletteEntries(m_palette, nStartIndex, nNumEntries,
                               lpPaletteColors);
  }

  void AnimatePalette(UINT nStartIndex, UINT nNumEntries,
                      LPPALETTEENTRY lpPaletteColors) {
    WINASSERT(m_palette != nullptr);
    ::AnimatePalette(m_palette, nStartIndex, nNumEntries, lpPaletteColors);
  }

  BOOL ResizePalette(UINT nNumEntries) {
    WINASSERT(m_palette != nullptr);
    return ::ResizePalette(m_palette, nNumEntries);
  }

  UINT GetNearestPaletteIndex(COLORREF crColor) const {
    WINASSERT(m_palette != nullptr);
    return ::GetNearestPaletteIndex(m_palette, crColor);
  }
};

typedef PaletteT<false> PaletteHandle;
typedef PaletteT<true> Palette;

template <bool Managed> class RgnT {
public:
  HRGN m_rgn;

  RgnT(HRGN hRgn = nullptr) : m_rgn(hRgn) {}

  ~RgnT() {
    if (Managed && m_rgn != nullptr)
      DeleteObject();
  }

  RgnT<Managed> &operator=(HRGN hRgn) {
    Attach(hRgn);
    return *this;
  }

  void Attach(HRGN hRgn) {
    if (Managed && m_rgn != nullptr && m_rgn != hRgn)
      ::DeleteObject(m_rgn);
    m_rgn = hRgn;
  }

  HRGN Detach() {
    HRGN hRgn = m_rgn;
    m_rgn = nullptr;
    return hRgn;
  }

  operator HRGN() const { return m_rgn; }

  bool IsNull() const { return (m_rgn == nullptr); }

  HRGN CreateRectRgn(int x1, int y1, int x2, int y2) {
    WINASSERT(m_rgn == nullptr);
    m_rgn = ::CreateRectRgn(x1, y1, x2, y2);
    return m_rgn;
  }

  HRGN CreateRectRgnIndirect(LPCRECT lpRect) {
    WINASSERT(m_rgn == nullptr);
    m_rgn = ::CreateRectRgnIndirect(lpRect);
    return m_rgn;
  }

  HRGN CreateEllipticRgn(int x1, int y1, int x2, int y2) {
    WINASSERT(m_rgn == nullptr);
    m_rgn = ::CreateEllipticRgn(x1, y1, x2, y2);
    return m_rgn;
  }

  HRGN CreateEllipticRgnIndirect(LPCRECT lpRect) {
    WINASSERT(m_rgn == nullptr);
    m_rgn = ::CreateEllipticRgnIndirect(lpRect);
    return m_rgn;
  }

  HRGN CreatePolygonRgn(LPPOINT lpPoints, int nCount, int nMode) {
    WINASSERT(m_rgn == nullptr);
    m_rgn = ::CreatePolygonRgn(lpPoints, nCount, nMode);
    return m_rgn;
  }

  HRGN CreatePolyPolygonRgn(LPPOINT lpPoints, LPINT lpPolyCounts, int nCount,
                            int nPolyFillMode) {
    WINASSERT(m_rgn == nullptr);
    m_rgn =
        ::CreatePolyPolygonRgn(lpPoints, lpPolyCounts, nCount, nPolyFillMode);
    return m_rgn;
  }

  HRGN CreateRoundRectRgn(int x1, int y1, int x2, int y2, int x3, int y3) {
    WINASSERT(m_rgn == nullptr);
    m_rgn = ::CreateRoundRectRgn(x1, y1, x2, y2, x3, y3);
    return m_rgn;
  }

  HRGN CreateFromPath(HDC hDC) {
    WINASSERT(m_rgn == nullptr);
    WINASSERT(hDC != nullptr);
    m_rgn = ::PathToRegion(hDC);
    return m_rgn;
  }

  HRGN CreateFromData(const XFORM *lpXForm, int nCount,
                      const RGNDATA *pRgnData) {
    WINASSERT(m_rgn == nullptr);
    m_rgn = ::ExtCreateRegion(lpXForm, nCount, pRgnData);
    return m_rgn;
  }

  BOOL DeleteObject() {
    WINASSERT(m_rgn != nullptr);
    BOOL bRet = ::DeleteObject(m_rgn);
    if (bRet)
      m_rgn = nullptr;
    return bRet;
  }

  void SetRectRgn(int x1, int y1, int x2, int y2) {
    WINASSERT(m_rgn != nullptr);
    ::SetRectRgn(m_rgn, x1, y1, x2, y2);
  }

  void SetRectRgn(LPCRECT lpRect) {
    WINASSERT(m_rgn != nullptr);
    ::SetRectRgn(m_rgn, lpRect->left, lpRect->top, lpRect->right,
                 lpRect->bottom);
  }

  int CombineRgn(HRGN hRgnSrc1, HRGN hRgnSrc2, int nCombineMode) {
    WINASSERT(m_rgn != nullptr);
    return ::CombineRgn(m_rgn, hRgnSrc1, hRgnSrc2, nCombineMode);
  }

  int CombineRgn(HRGN hRgnSrc, int nCombineMode) {
    WINASSERT(m_rgn != nullptr);
    return ::CombineRgn(m_rgn, m_rgn, hRgnSrc, nCombineMode);
  }

  int CopyRgn(HRGN hRgnSrc) {
    WINASSERT(m_rgn != nullptr);
    return ::CombineRgn(m_rgn, hRgnSrc, nullptr, RGN_COPY);
  }

  BOOL EqualRgn(HRGN hRgn) const {
    WINASSERT(m_rgn != nullptr);
    return ::EqualRgn(m_rgn, hRgn);
  }

  int OffsetRgn(int x, int y) {
    WINASSERT(m_rgn != nullptr);
    return ::OffsetRgn(m_rgn, x, y);
  }

  int OffsetRgn(POINT point) {
    WINASSERT(m_rgn != nullptr);
    return ::OffsetRgn(m_rgn, point.x, point.y);
  }

  int GetRgnBox(LPRECT lpRect) const {
    WINASSERT(m_rgn != nullptr);
    return ::GetRgnBox(m_rgn, lpRect);
  }

  BOOL PtInRegion(int x, int y) const {
    WINASSERT(m_rgn != nullptr);
    return ::PtInRegion(m_rgn, x, y);
  }

  BOOL PtInRegion(POINT point) const {
    WINASSERT(m_rgn != nullptr);
    return ::PtInRegion(m_rgn, point.x, point.y);
  }

  BOOL RectInRegion(LPCRECT lpRect) const {
    WINASSERT(m_rgn != nullptr);
    return ::RectInRegion(m_rgn, lpRect);
  }

  int GetRegionData(LPRGNDATA lpRgnData, int nDataSize) const {
    WINASSERT(m_rgn != nullptr);
    return (int)::GetRegionData(m_rgn, nDataSize, lpRgnData);
  }
};

typedef RgnT<false> RgnHandle;
typedef RgnT<true> Rgn;

template <bool Managed> class DCT {
public:
  HDC m_hdc;

  DCT(HDC hDC = nullptr) : m_hdc(hDC) {}

  ~DCT() {
    if (Managed && m_hdc != nullptr)
      ::DeleteDC(Detach());
  }

  DCT<Managed> &operator=(HDC hDC) {
    Attach(hDC);
    return *this;
  }

  void Attach(HDC hDC) {
    if (Managed && m_hdc != nullptr && m_hdc != hDC)
      ::DeleteDC(m_hdc);
    m_hdc = hDC;
  }

  HDC Detach() {
    HDC hDC = m_hdc;
    m_hdc = nullptr;
    return hDC;
  }

  operator HDC() const { return m_hdc; }

  bool IsNull() const { return (m_hdc == nullptr); }

  HWND WindowFromDC() const {
    WINASSERT(m_hdc != nullptr);
    return ::WindowFromDC(m_hdc);
  }

  PenHandle GetCurrentPen() const {
    WINASSERT(m_hdc != nullptr);
    return PenHandle((HPEN)::GetCurrentObject(m_hdc, OBJ_PEN));
  }

  BrushHandle GetCurrentBrush() const {
    WINASSERT(m_hdc != nullptr);
    return BrushHandle((HBRUSH)::GetCurrentObject(m_hdc, OBJ_BRUSH));
  }

  PaletteHandle GetCurrentPalette() const {
    WINASSERT(m_hdc != nullptr);
    return PaletteHandle((HPALETTE)::GetCurrentObject(m_hdc, OBJ_PAL));
  }

  GDIFontHandle GetCurrentFont() const {
    WINASSERT(m_hdc != nullptr);
    return GDIFontHandle((HFONT)::GetCurrentObject(m_hdc, OBJ_FONT));
  }

  BitmapHandle GetCurrentBitmap() const {
    WINASSERT(m_hdc != nullptr);
    return BitmapHandle((HBITMAP)::GetCurrentObject(m_hdc, OBJ_BITMAP));
  }

  HDC CreateDC(LPCTSTR lpszDriverName, LPCTSTR lpszDeviceName,
               LPCTSTR lpszOutput, const DEVMODE *lpInitData) {
    WINASSERT(m_hdc == nullptr);
    m_hdc = ::CreateDC(lpszDriverName, lpszDeviceName, lpszOutput, lpInitData);
    return m_hdc;
  }

  HDC CreateCompatibleDC(HDC hDC = nullptr) {
    WINASSERT(m_hdc == nullptr);
    m_hdc = ::CreateCompatibleDC(hDC);
    return m_hdc;
  }

  BOOL DeleteDC() {
    if (m_hdc == nullptr)
      return FALSE;
    BOOL bRet = ::DeleteDC(m_hdc);
    if (bRet)
      m_hdc = nullptr;
    return bRet;
  }

  int SaveDC() {
    WINASSERT(m_hdc != nullptr);
    return ::SaveDC(m_hdc);
  }

  BOOL RestoreDC(int nSavedDC) {
    WINASSERT(m_hdc != nullptr);
    return ::RestoreDC(m_hdc, nSavedDC);
  }

  int GetDeviceCaps(int nIndex) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetDeviceCaps(m_hdc, nIndex);
  }

  UINT SetBoundsRect(LPCRECT lpRectBounds, UINT flags) {
    WINASSERT(m_hdc != nullptr);
    return ::SetBoundsRect(m_hdc, lpRectBounds, flags);
  }

  UINT GetBoundsRect(LPRECT lpRectBounds, UINT flags) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetBoundsRect(m_hdc, lpRectBounds, flags);
  }

  BOOL ResetDC(const DEVMODE *lpDevMode) {
    WINASSERT(m_hdc != nullptr);
    return ::ResetDC(m_hdc, lpDevMode) != nullptr;
  }

  BOOL GetBrushOrg(LPPOINT lpPoint) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetBrushOrgEx(m_hdc, lpPoint);
  }

  BOOL SetBrushOrg(int x, int y, LPPOINT lpPoint = nullptr) {
    WINASSERT(m_hdc != nullptr);
    return ::SetBrushOrgEx(m_hdc, x, y, lpPoint);
  }

  BOOL SetBrushOrg(POINT point, LPPOINT lpPointRet = nullptr) {
    WINASSERT(m_hdc != nullptr);
    return ::SetBrushOrgEx(m_hdc, point.x, point.y, lpPointRet);
  }

  int EnumObjects(int nObjectType, int(CALLBACK *lpfn)(LPVOID, LPARAM),
                  LPARAM lpData) {
    WINASSERT(m_hdc != nullptr);
#ifdef STRICT
    return ::EnumObjects(m_hdc, nObjectType, (GOBJENUMPROC)lpfn, lpData);
#else
    return ::EnumObjects(m_hdc, nObjectType, (GOBJENUMPROC)lpfn,
                         (LPVOID)lpData);
#endif
  }

  HPEN SelectPen(HPEN hPen) {
    WINASSERT(m_hdc != nullptr);
    WINASSERT(hPen == nullptr || ::GetObjectType(hPen) == OBJ_PEN ||
              ::GetObjectType(hPen) == OBJ_EXTPEN);
    return (HPEN)::SelectObject(m_hdc, hPen);
  }

  HBRUSH SelectBrush(HBRUSH hBrush) {
    WINASSERT(m_hdc != nullptr);
    WINASSERT(hBrush == nullptr || ::GetObjectType(hBrush) == OBJ_BRUSH);
    return (HBRUSH)::SelectObject(m_hdc, hBrush);
  }

  HFONT SelectFont(HFONT hFont) {
    WINASSERT(m_hdc != nullptr);
    WINASSERT(hFont == nullptr || ::GetObjectType(hFont) == OBJ_FONT);
    return (HFONT)::SelectObject(m_hdc, hFont);
  }

  HBITMAP SelectBitmap(HBITMAP hBitmap) {
    WINASSERT(m_hdc != nullptr);
    WINASSERT(hBitmap == nullptr || ::GetObjectType(hBitmap) == OBJ_BITMAP);
    return (HBITMAP)::SelectObject(m_hdc, hBitmap);
  }

  int SelectRgn(HRGN hRgn) {
    WINASSERT(m_hdc != nullptr);
    WINASSERT(hRgn == nullptr || ::GetObjectType(hRgn) == OBJ_REGION);
    return PtrToInt(::SelectObject(m_hdc, hRgn));
  }

  HPEN SelectStockPen(int nPen) {
    WINASSERT(m_hdc != nullptr);
#if (_WIN32_WINNT >= 0x0500)
    WINASSERT(nPen == WHITE_PEN || nPen == BLACK_PEN || nPen == NULL_PEN ||
              nPen == DC_PEN);
#else
    WINASSERT(nPen == WHITE_PEN || nPen == BLACK_PEN || nPen == NULL_PEN);
#endif
    return SelectPen((HPEN)::GetStockObject(nPen));
  }

  HBRUSH SelectStockBrush(int nBrush) {
#if (_WIN32_WINNT >= 0x0500)
    WINASSERT((nBrush >= WHITE_BRUSH && nBrush <= HOLLOW_BRUSH) ||
              nBrush == DC_BRUSH);
#else
    WINASSERT(nBrush >= WHITE_BRUSH && nBrush <= HOLLOW_BRUSH);
#endif
    return SelectBrush((HBRUSH)::GetStockObject(nBrush));
  }

  HFONT SelectStockFont(int nFont) {
    WINASSERT((nFont >= OEM_FIXED_FONT && nFont <= SYSTEM_FIXED_FONT) ||
              nFont == DEFAULT_GUI_FONT);
    return SelectFont((HFONT)::GetStockObject(nFont));
  }

  HPALETTE SelectStockPalette(int nPalette, BOOL bForceBackground) {
    WINASSERT(nPalette == DEFAULT_PALETTE); // the only one supported
    return SelectPalette((HPALETTE)::GetStockObject(nPalette),
                         bForceBackground);
  }

  COLORREF GetNearestColor(COLORREF crColor) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetNearestColor(m_hdc, crColor);
  }

  HPALETTE SelectPalette(HPALETTE hPalette, BOOL bForceBackground) {
    WINASSERT(m_hdc != nullptr);

    return ::SelectPalette(m_hdc, hPalette, bForceBackground);
  }

  UINT RealizePalette() {
    WINASSERT(m_hdc != nullptr);
    return ::RealizePalette(m_hdc);
  }

  void UpdateColors() {
    WINASSERT(m_hdc != nullptr);
    ::UpdateColors(m_hdc);
  }

  COLORREF GetBkColor() const {
    WINASSERT(m_hdc != nullptr);
    return ::GetBkColor(m_hdc);
  }

  int GetBkMode() const {
    WINASSERT(m_hdc != nullptr);
    return ::GetBkMode(m_hdc);
  }

  int GetPolyFillMode() const {
    WINASSERT(m_hdc != nullptr);
    return ::GetPolyFillMode(m_hdc);
  }

  int GetROP2() const {
    WINASSERT(m_hdc != nullptr);
    return ::GetROP2(m_hdc);
  }

  int GetStretchBltMode() const {
    WINASSERT(m_hdc != nullptr);
    return ::GetStretchBltMode(m_hdc);
  }

  COLORREF GetTextColor() const {
    WINASSERT(m_hdc != nullptr);
    return ::GetTextColor(m_hdc);
  }

  COLORREF SetBkColor(COLORREF crColor) {
    WINASSERT(m_hdc != nullptr);
    return ::SetBkColor(m_hdc, crColor);
  }

  int SetBkMode(int nBkMode) {
    WINASSERT(m_hdc != nullptr);
    return ::SetBkMode(m_hdc, nBkMode);
  }

  int SetPolyFillMode(int nPolyFillMode) {
    WINASSERT(m_hdc != nullptr);
    return ::SetPolyFillMode(m_hdc, nPolyFillMode);
  }

  int SetROP2(int nDrawMode) {
    WINASSERT(m_hdc != nullptr);
    return ::SetROP2(m_hdc, nDrawMode);
  }

  int SetStretchBltMode(int nStretchMode) {
    WINASSERT(m_hdc != nullptr);
    return ::SetStretchBltMode(m_hdc, nStretchMode);
  }

  COLORREF SetTextColor(COLORREF crColor) {
    WINASSERT(m_hdc != nullptr);
    return ::SetTextColor(m_hdc, crColor);
  }

  BOOL GetColorAdjustment(LPCOLORADJUSTMENT lpColorAdjust) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetColorAdjustment(m_hdc, lpColorAdjust);
  }

  BOOL SetColorAdjustment(const COLORADJUSTMENT *lpColorAdjust) {
    WINASSERT(m_hdc != nullptr);
    return ::SetColorAdjustment(m_hdc, lpColorAdjust);
  }

  int GetMapMode() const {
    WINASSERT(m_hdc != nullptr);
    return ::GetMapMode(m_hdc);
  }

  BOOL GetViewportOrg(LPPOINT lpPoint) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetViewportOrgEx(m_hdc, lpPoint);
  }

  int SetMapMode(int nMapMode) {
    WINASSERT(m_hdc != nullptr);
    return ::SetMapMode(m_hdc, nMapMode);
  }

  BOOL SetViewportOrg(int x, int y, LPPOINT lpPoint = nullptr) {
    WINASSERT(m_hdc != nullptr);
    return ::SetViewportOrgEx(m_hdc, x, y, lpPoint);
  }

  BOOL SetViewportOrg(POINT point, LPPOINT lpPointRet = nullptr) {
    WINASSERT(m_hdc != nullptr);
    return SetViewportOrg(point.x, point.y, lpPointRet);
  }

  BOOL OffsetViewportOrg(int nWidth, int nHeight, LPPOINT lpPoint = nullptr) {
    WINASSERT(m_hdc != nullptr);
    return ::OffsetViewportOrgEx(m_hdc, nWidth, nHeight, lpPoint);
  }

  BOOL GetViewportExt(LPSIZE lpSize) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetViewportExtEx(m_hdc, lpSize);
  }

  BOOL SetViewportExt(int x, int y, LPSIZE lpSize = nullptr) {
    WINASSERT(m_hdc != nullptr);
    return ::SetViewportExtEx(m_hdc, x, y, lpSize);
  }

  BOOL SetViewportExt(SIZE size, LPSIZE lpSizeRet = nullptr) {
    WINASSERT(m_hdc != nullptr);
    return SetViewportExt(size.cx, size.cy, lpSizeRet);
  }

  BOOL ScaleViewportExt(int xNum, int xDenom, int yNum, int yDenom,
                        LPSIZE lpSize = nullptr) {
    WINASSERT(m_hdc != nullptr);
    return ::ScaleViewportExtEx(m_hdc, xNum, xDenom, yNum, yDenom, lpSize);
  }

  BOOL GetWindowOrg(LPPOINT lpPoint) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetWindowOrgEx(m_hdc, lpPoint);
  }

  BOOL SetWindowOrg(int x, int y, LPPOINT lpPoint = nullptr) {
    WINASSERT(m_hdc != nullptr);
    return ::SetWindowOrgEx(m_hdc, x, y, lpPoint);
  }

  BOOL SetWindowOrg(POINT point, LPPOINT lpPointRet = nullptr) {
    WINASSERT(m_hdc != nullptr);
    return SetWindowOrg(point.x, point.y, lpPointRet);
  }

  BOOL OffsetWindowOrg(int nWidth, int nHeight, LPPOINT lpPoint = nullptr) {
    WINASSERT(m_hdc != nullptr);
    return ::OffsetWindowOrgEx(m_hdc, nWidth, nHeight, lpPoint);
  }

  BOOL GetWindowExt(LPSIZE lpSize) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetWindowExtEx(m_hdc, lpSize);
  }

  BOOL SetWindowExt(int x, int y, LPSIZE lpSize = nullptr) {
    WINASSERT(m_hdc != nullptr);
    return ::SetWindowExtEx(m_hdc, x, y, lpSize);
  }

  BOOL SetWindowExt(SIZE size, LPSIZE lpSizeRet = nullptr) {
    WINASSERT(m_hdc != nullptr);
    return SetWindowExt(size.cx, size.cy, lpSizeRet);
  }

  BOOL ScaleWindowExt(int xNum, int xDenom, int yNum, int yDenom,
                      LPSIZE lpSize = nullptr) {
    WINASSERT(m_hdc != nullptr);
    return ::ScaleWindowExtEx(m_hdc, xNum, xDenom, yNum, yDenom, lpSize);
  }

  BOOL DPtoLP(LPPOINT lpPoints, int nCount = 1) const {
    WINASSERT(m_hdc != nullptr);
    return ::DPtoLP(m_hdc, lpPoints, nCount);
  }

  BOOL DPtoLP(LPRECT lpRect) const {
    WINASSERT(m_hdc != nullptr);
    return ::DPtoLP(m_hdc, (LPPOINT)lpRect, 2);
  }

  BOOL DPtoLP(LPSIZE lpSize) const {
    SIZE sizeWinExt = {0, 0};
    if (!GetWindowExt(&sizeWinExt))
      return FALSE;
    SIZE sizeVpExt = {0, 0};
    if (!GetViewportExt(&sizeVpExt))
      return FALSE;
    lpSize->cx = ::MulDiv(lpSize->cx, abs(sizeWinExt.cx), abs(sizeVpExt.cx));
    lpSize->cy = ::MulDiv(lpSize->cy, abs(sizeWinExt.cy), abs(sizeVpExt.cy));
    return TRUE;
  }

  BOOL LPtoDP(LPPOINT lpPoints, int nCount = 1) const {
    WINASSERT(m_hdc != nullptr);
    return ::LPtoDP(m_hdc, lpPoints, nCount);
  }

  BOOL LPtoDP(LPRECT lpRect) const {
    WINASSERT(m_hdc != nullptr);
    return ::LPtoDP(m_hdc, (LPPOINT)lpRect, 2);
  }

  BOOL LPtoDP(LPSIZE lpSize) const {
    SIZE sizeWinExt = {0, 0};
    if (!GetWindowExt(&sizeWinExt))
      return FALSE;
    SIZE sizeVpExt = {0, 0};
    if (!GetViewportExt(&sizeVpExt))
      return FALSE;
    lpSize->cx = ::MulDiv(lpSize->cx, abs(sizeVpExt.cx), abs(sizeWinExt.cx));
    lpSize->cy = ::MulDiv(lpSize->cy, abs(sizeVpExt.cy), abs(sizeWinExt.cy));
    return TRUE;
  }

#define HIMETRIC_INCH 2540 // HIMETRIC units per inch

  void DPtoHIMETRIC(LPSIZE lpSize) const {
    WINASSERT(m_hdc != nullptr);
    int nMapMode;
    if ((nMapMode = GetMapMode()) < MM_ISOTROPIC && nMapMode != MM_TEXT) {
      // when using a constrained map mode, map against physical inch
      ((DCT<true> *)this)->SetMapMode(MM_HIMETRIC);
      DPtoLP(lpSize);
      ((DCT<true> *)this)->SetMapMode(nMapMode);
    } else {
      // map against logical inch for non-constrained mapping modes
      int cxPerInch = GetDeviceCaps(LOGPIXELSX);
      int cyPerInch = GetDeviceCaps(LOGPIXELSY);
      WINASSERT(cxPerInch != 0 && cyPerInch != 0);
      lpSize->cx = ::MulDiv(lpSize->cx, HIMETRIC_INCH, cxPerInch);
      lpSize->cy = ::MulDiv(lpSize->cy, HIMETRIC_INCH, cyPerInch);
    }
  }

  void HIMETRICtoDP(LPSIZE lpSize) const {
    WINASSERT(m_hdc != nullptr);
    int nMapMode;
    if ((nMapMode = GetMapMode()) < MM_ISOTROPIC && nMapMode != MM_TEXT) {
      // when using a constrained map mode, map against physical inch
      ((DCT<true> *)this)->SetMapMode(MM_HIMETRIC);
      LPtoDP(lpSize);
      ((DCT<true> *)this)->SetMapMode(nMapMode);
    } else {
      // map against logical inch for non-constrained mapping modes
      int cxPerInch = GetDeviceCaps(LOGPIXELSX);
      int cyPerInch = GetDeviceCaps(LOGPIXELSY);
      WINASSERT(cxPerInch != 0 && cyPerInch != 0);
      lpSize->cx = ::MulDiv(lpSize->cx, cxPerInch, HIMETRIC_INCH);
      lpSize->cy = ::MulDiv(lpSize->cy, cyPerInch, HIMETRIC_INCH);
    }
  }

  void LPtoHIMETRIC(LPSIZE lpSize) const {
    LPtoDP(lpSize);
    DPtoHIMETRIC(lpSize);
  }

  void HIMETRICtoLP(LPSIZE lpSize) const {
    HIMETRICtoDP(lpSize);
    DPtoLP(lpSize);
  }

  BOOL FillRgn(HRGN hRgn, HBRUSH hBrush) {
    WINASSERT(m_hdc != nullptr);
    return ::FillRgn(m_hdc, hRgn, hBrush);
  }

  BOOL FrameRgn(HRGN hRgn, HBRUSH hBrush, int nWidth, int nHeight) {
    WINASSERT(m_hdc != nullptr);
    return ::FrameRgn(m_hdc, hRgn, hBrush, nWidth, nHeight);
  }

  BOOL InvertRgn(HRGN hRgn) {
    WINASSERT(m_hdc != nullptr);
    return ::InvertRgn(m_hdc, hRgn);
  }

  BOOL PaintRgn(HRGN hRgn) {
    WINASSERT(m_hdc != nullptr);
    return ::PaintRgn(m_hdc, hRgn);
  }

  int GetClipBox(LPRECT lpRect) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetClipBox(m_hdc, lpRect);
  }

  int GetClipRgn(Rgn &region) const {
    WINASSERT(m_hdc != nullptr);
    if (region.IsNull())
      region.CreateRectRgn(0, 0, 0, 0);

    int nRet = ::GetClipRgn(m_hdc, region);
    if (nRet != 1)
      region.DeleteObject();

    return nRet;
  }

  BOOL PtVisible(int x, int y) const {
    WINASSERT(m_hdc != nullptr);
    return ::PtVisible(m_hdc, x, y);
  }

  BOOL PtVisible(POINT point) const {
    WINASSERT(m_hdc != nullptr);
    return ::PtVisible(m_hdc, point.x, point.y);
  }

  BOOL RectVisible(LPCRECT lpRect) const {
    WINASSERT(m_hdc != nullptr);
    return ::RectVisible(m_hdc, lpRect);
  }

  int SelectClipRgn(HRGN hRgn) {
    WINASSERT(m_hdc != nullptr);
    return ::SelectClipRgn(m_hdc, (HRGN)hRgn);
  }

  int ExcludeClipRect(int x1, int y1, int x2, int y2) {
    WINASSERT(m_hdc != nullptr);
    return ::ExcludeClipRect(m_hdc, x1, y1, x2, y2);
  }

  int ExcludeClipRect(LPCRECT lpRect) {
    WINASSERT(m_hdc != nullptr);
    return ::ExcludeClipRect(m_hdc, lpRect->left, lpRect->top, lpRect->right,
                             lpRect->bottom);
  }

  int ExcludeUpdateRgn(HWND hWnd) {
    WINASSERT(m_hdc != nullptr);
    return ::ExcludeUpdateRgn(m_hdc, hWnd);
  }

  int IntersectClipRect(int x1, int y1, int x2, int y2) {
    WINASSERT(m_hdc != nullptr);
    return ::IntersectClipRect(m_hdc, x1, y1, x2, y2);
  }

  int IntersectClipRect(LPCRECT lpRect) {
    WINASSERT(m_hdc != nullptr);
    return ::IntersectClipRect(m_hdc, lpRect->left, lpRect->top, lpRect->right,
                               lpRect->bottom);
  }

  int OffsetClipRgn(int x, int y) {
    WINASSERT(m_hdc != nullptr);
    return ::OffsetClipRgn(m_hdc, x, y);
  }

  int OffsetClipRgn(SIZE size) {
    WINASSERT(m_hdc != nullptr);
    return ::OffsetClipRgn(m_hdc, size.cx, size.cy);
  }

  int SelectClipRgn(HRGN hRgn, int nMode) {
    WINASSERT(m_hdc != nullptr);
    return ::ExtSelectClipRgn(m_hdc, hRgn, nMode);
  }

  BOOL GetCurrentPosition(LPPOINT lpPoint) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetCurrentPositionEx(m_hdc, lpPoint);
  }

  BOOL MoveTo(int x, int y, LPPOINT lpPoint = nullptr) {
    WINASSERT(m_hdc != nullptr);
    return ::MoveToEx(m_hdc, x, y, lpPoint);
  }

  BOOL MoveTo(POINT point, LPPOINT lpPointRet = nullptr) {
    WINASSERT(m_hdc != nullptr);
    return MoveTo(point.x, point.y, lpPointRet);
  }

  BOOL LineTo(int x, int y) {
    WINASSERT(m_hdc != nullptr);
    return ::LineTo(m_hdc, x, y);
  }

  BOOL LineTo(POINT point) {
    WINASSERT(m_hdc != nullptr);
    return LineTo(point.x, point.y);
  }

  BOOL Arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    WINASSERT(m_hdc != nullptr);
    return ::Arc(m_hdc, x1, y1, x2, y2, x3, y3, x4, y4);
  }

  BOOL Arc(LPCRECT lpRect, POINT ptStart, POINT ptEnd) {
    WINASSERT(m_hdc != nullptr);
    return ::Arc(m_hdc, lpRect->left, lpRect->top, lpRect->right,
                 lpRect->bottom, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
  }

  BOOL Polyline(const POINT *lpPoints, int nCount) {
    WINASSERT(m_hdc != nullptr);
    return ::Polyline(m_hdc, lpPoints, nCount);
  }

  BOOL AngleArc(int x, int y, int nRadius, float fStartAngle,
                float fSweepAngle) {
    WINASSERT(m_hdc != nullptr);
    return ::AngleArc(m_hdc, x, y, nRadius, fStartAngle, fSweepAngle);
  }

  BOOL ArcTo(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    WINASSERT(m_hdc != nullptr);
    return ::ArcTo(m_hdc, x1, y1, x2, y2, x3, y3, x4, y4);
  }

  BOOL ArcTo(LPCRECT lpRect, POINT ptStart, POINT ptEnd) {
    WINASSERT(m_hdc != nullptr);
    return ArcTo(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom,
                 ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
  }

  int GetArcDirection() const {
    WINASSERT(m_hdc != nullptr);
    return ::GetArcDirection(m_hdc);
  }

  int SetArcDirection(int nArcDirection) {
    WINASSERT(m_hdc != nullptr);
    return ::SetArcDirection(m_hdc, nArcDirection);
  }

  BOOL PolyDraw(const POINT *lpPoints, const BYTE *lpTypes, int nCount) {
    WINASSERT(m_hdc != nullptr);
    return ::PolyDraw(m_hdc, lpPoints, lpTypes, nCount);
  }

  BOOL PolylineTo(const POINT *lpPoints, int nCount) {
    WINASSERT(m_hdc != nullptr);
    return ::PolylineTo(m_hdc, lpPoints, nCount);
  }

  BOOL PolyPolyline(const POINT *lpPoints, const DWORD *lpPolyPoints,
                    int nCount) {
    WINASSERT(m_hdc != nullptr);
    return ::PolyPolyline(m_hdc, lpPoints, lpPolyPoints, nCount);
  }

  BOOL PolyBezier(const POINT *lpPoints, int nCount) {
    WINASSERT(m_hdc != nullptr);
    return ::PolyBezier(m_hdc, lpPoints, nCount);
  }

  BOOL PolyBezierTo(const POINT *lpPoints, int nCount) {
    WINASSERT(m_hdc != nullptr);
    return ::PolyBezierTo(m_hdc, lpPoints, nCount);
  }

  BOOL FillRect(LPCRECT lpRect, HBRUSH hBrush) {
    WINASSERT(m_hdc != nullptr);
    return ::FillRect(m_hdc, lpRect, hBrush);
  }

  BOOL FillRect(LPCRECT lpRect, int nColorIndex) {
    WINASSERT(m_hdc != nullptr);
    return ::FillRect(m_hdc, lpRect, (HBRUSH)LongToPtr(nColorIndex + 1));
  }

  BOOL FrameRect(LPCRECT lpRect, HBRUSH hBrush) {
    WINASSERT(m_hdc != nullptr);
    return ::FrameRect(m_hdc, lpRect, hBrush);
  }

  BOOL InvertRect(LPCRECT lpRect) {
    WINASSERT(m_hdc != nullptr);
    return ::InvertRect(m_hdc, lpRect);
  }

  BOOL DrawIcon(int x, int y, HICON hIcon) {
    WINASSERT(m_hdc != nullptr);
    return ::DrawIcon(m_hdc, x, y, hIcon);
  }

  BOOL DrawIcon(POINT point, HICON hIcon) {
    WINASSERT(m_hdc != nullptr);
    return ::DrawIcon(m_hdc, point.x, point.y, hIcon);
  }

  BOOL DrawIconEx(int x, int y, HICON hIcon, int cxWidth, int cyWidth,
                  UINT uStepIfAniCur = 0, HBRUSH hbrFlickerFreeDraw = nullptr,
                  UINT uFlags = DI_NORMAL) {
    WINASSERT(m_hdc != nullptr);
    return ::DrawIconEx(m_hdc, x, y, hIcon, cxWidth, cyWidth, uStepIfAniCur,
                        hbrFlickerFreeDraw, uFlags);
  }

  BOOL DrawIconEx(POINT point, HICON hIcon, SIZE size, UINT uStepIfAniCur = 0,
                  HBRUSH hbrFlickerFreeDraw = nullptr,
                  UINT uFlags = DI_NORMAL) {
    WINASSERT(m_hdc != nullptr);
    return ::DrawIconEx(m_hdc, point.x, point.y, hIcon, size.cx, size.cy,
                        uStepIfAniCur, hbrFlickerFreeDraw, uFlags);
  }

  BOOL DrawState(POINT pt, SIZE size, HBITMAP hBitmap, UINT nFlags,
                 HBRUSH hBrush = nullptr) {
    WINASSERT(m_hdc != nullptr);
    return ::DrawState(m_hdc, hBrush, nullptr, (LPARAM)hBitmap, 0, pt.x, pt.y,
                       size.cx, size.cy, nFlags | DST_BITMAP);
  }

  BOOL DrawState(POINT pt, SIZE size, HICON hIcon, UINT nFlags,
                 HBRUSH hBrush = nullptr) {
    WINASSERT(m_hdc != nullptr);
    return ::DrawState(m_hdc, hBrush, nullptr, (LPARAM)hIcon, 0, pt.x, pt.y,
                       size.cx, size.cy, nFlags | DST_ICON);
  }

  BOOL DrawState(POINT pt, SIZE size, LPCTSTR lpszText, UINT nFlags,
                 BOOL bPrefixText = TRUE, int nTextLen = 0,
                 HBRUSH hBrush = nullptr) {
    WINASSERT(m_hdc != nullptr);
    return ::DrawState(m_hdc, hBrush, nullptr, (LPARAM)lpszText,
                       (WPARAM)nTextLen, pt.x, pt.y, size.cx, size.cy,
                       nFlags | (bPrefixText ? DST_PREFIXTEXT : DST_TEXT));
  }

  BOOL DrawState(POINT pt, SIZE size, DRAWSTATEPROC lpDrawProc, LPARAM lData,
                 UINT nFlags, HBRUSH hBrush = nullptr) {
    WINASSERT(m_hdc != nullptr);
    return ::DrawState(m_hdc, hBrush, lpDrawProc, lData, 0, pt.x, pt.y, size.cx,
                       size.cy, nFlags | DST_COMPLEX);
  }

  BOOL Chord(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    WINASSERT(m_hdc != nullptr);
    return ::Chord(m_hdc, x1, y1, x2, y2, x3, y3, x4, y4);
  }

  BOOL Chord(LPCRECT lpRect, POINT ptStart, POINT ptEnd) {
    WINASSERT(m_hdc != nullptr);
    return ::Chord(m_hdc, lpRect->left, lpRect->top, lpRect->right,
                   lpRect->bottom, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
  }

  void DrawFocusRect(LPCRECT lpRect) {
    WINASSERT(m_hdc != nullptr);
    ::DrawFocusRect(m_hdc, lpRect);
  }

  BOOL Ellipse(int x1, int y1, int x2, int y2) {
    WINASSERT(m_hdc != nullptr);
    return ::Ellipse(m_hdc, x1, y1, x2, y2);
  }

  BOOL Ellipse(LPCRECT lpRect) {
    WINASSERT(m_hdc != nullptr);
    return ::Ellipse(m_hdc, lpRect->left, lpRect->top, lpRect->right,
                     lpRect->bottom);
  }

  BOOL Pie(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    WINASSERT(m_hdc != nullptr);
    return ::Pie(m_hdc, x1, y1, x2, y2, x3, y3, x4, y4);
  }

  BOOL Pie(LPCRECT lpRect, POINT ptStart, POINT ptEnd) {
    WINASSERT(m_hdc != nullptr);
    return ::Pie(m_hdc, lpRect->left, lpRect->top, lpRect->right,
                 lpRect->bottom, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
  }

  BOOL Polygon(const POINT *lpPoints, int nCount) {
    WINASSERT(m_hdc != nullptr);
    return ::Polygon(m_hdc, lpPoints, nCount);
  }

  BOOL PolyPolygon(const POINT *lpPoints, const INT *lpPolyCounts, int nCount) {
    WINASSERT(m_hdc != nullptr);
    return ::PolyPolygon(m_hdc, lpPoints, lpPolyCounts, nCount);
  }

  BOOL Rectangle(int x1, int y1, int x2, int y2) {
    WINASSERT(m_hdc != nullptr);
    return ::Rectangle(m_hdc, x1, y1, x2, y2);
  }

  BOOL Rectangle(LPCRECT lpRect) {
    WINASSERT(m_hdc != nullptr);
    return ::Rectangle(m_hdc, lpRect->left, lpRect->top, lpRect->right,
                       lpRect->bottom);
  }

  BOOL RoundRect(int x1, int y1, int x2, int y2, int x3, int y3) {
    WINASSERT(m_hdc != nullptr);
    return ::RoundRect(m_hdc, x1, y1, x2, y2, x3, y3);
  }

  BOOL RoundRect(LPCRECT lpRect, POINT point) {
    WINASSERT(m_hdc != nullptr);
    return ::RoundRect(m_hdc, lpRect->left, lpRect->top, lpRect->right,
                       lpRect->bottom, point.x, point.y);
  }

  BOOL PatBlt(int x, int y, int nWidth, int nHeight, DWORD dwRop) {
    WINASSERT(m_hdc != nullptr);
    return ::PatBlt(m_hdc, x, y, nWidth, nHeight, dwRop);
  }

  BOOL BitBlt(int x, int y, int nWidth, int nHeight, HDC hSrcDC, int xSrc,
              int ySrc, DWORD dwRop) {
    WINASSERT(m_hdc != nullptr);
    return ::BitBlt(m_hdc, x, y, nWidth, nHeight, hSrcDC, xSrc, ySrc, dwRop);
  }

  BOOL StretchBlt(int x, int y, int nWidth, int nHeight, HDC hSrcDC, int xSrc,
                  int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop) {
    WINASSERT(m_hdc != nullptr);
    return ::StretchBlt(m_hdc, x, y, nWidth, nHeight, hSrcDC, xSrc, ySrc,
                        nSrcWidth, nSrcHeight, dwRop);
  }

  COLORREF GetPixel(int x, int y) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetPixel(m_hdc, x, y);
  }

  COLORREF GetPixel(POINT point) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetPixel(m_hdc, point.x, point.y);
  }

  COLORREF SetPixel(int x, int y, COLORREF crColor) {
    WINASSERT(m_hdc != nullptr);
    return ::SetPixel(m_hdc, x, y, crColor);
  }

  COLORREF SetPixel(POINT point, COLORREF crColor) {
    WINASSERT(m_hdc != nullptr);
    return ::SetPixel(m_hdc, point.x, point.y, crColor);
  }

  BOOL FloodFill(int x, int y, COLORREF crColor) {
    WINASSERT(m_hdc != nullptr);
    return ::FloodFill(m_hdc, x, y, crColor);
  }

  BOOL ExtFloodFill(int x, int y, COLORREF crColor, UINT nFillType) {
    WINASSERT(m_hdc != nullptr);
    return ::ExtFloodFill(m_hdc, x, y, crColor, nFillType);
  }

  BOOL MaskBlt(int x, int y, int nWidth, int nHeight, HDC hSrcDC, int xSrc,
               int ySrc, HBITMAP hMaskBitmap, int xMask, int yMask,
               DWORD dwRop) {
    WINASSERT(m_hdc != nullptr);
    return ::MaskBlt(m_hdc, x, y, nWidth, nHeight, hSrcDC, xSrc, ySrc,
                     hMaskBitmap, xMask, yMask, dwRop);
  }

  BOOL PlgBlt(LPPOINT lpPoint, HDC hSrcDC, int xSrc, int ySrc, int nWidth,
              int nHeight, HBITMAP hMaskBitmap, int xMask, int yMask) {
    WINASSERT(m_hdc != nullptr);
    return ::PlgBlt(m_hdc, lpPoint, hSrcDC, xSrc, ySrc, nWidth, nHeight,
                    hMaskBitmap, xMask, yMask);
  }

  BOOL SetPixelV(int x, int y, COLORREF crColor) {
    WINASSERT(m_hdc != nullptr);
    return ::SetPixelV(m_hdc, x, y, crColor);
  }

  BOOL SetPixelV(POINT point, COLORREF crColor) {
    WINASSERT(m_hdc != nullptr);
    return ::SetPixelV(m_hdc, point.x, point.y, crColor);
  }

  BOOL TransparentBlt(int x, int y, int nWidth, int nHeight, HDC hSrcDC,
                      int xSrc, int ySrc, int nSrcWidth, int nSrcHeight,
                      UINT crTransparent) {
    WINASSERT(m_hdc != nullptr);
    return ::TransparentBlt(m_hdc, x, y, nWidth, nHeight, hSrcDC, xSrc, ySrc,
                            nSrcWidth, nSrcHeight, crTransparent);
  }

  BOOL GradientFill(const PTRIVERTEX pVertices, DWORD nVertices,
                    void *pMeshElements, DWORD nMeshElements, DWORD dwMode) {
    WINASSERT(m_hdc != nullptr);
    return ::GradientFill(m_hdc, pVertices, nVertices, pMeshElements,
                          nMeshElements, dwMode);
  }

  BOOL GradientFillRect(RECT &rect, COLORREF clr1, COLORREF clr2,
                        bool bHorizontal) {
    WINASSERT(m_hdc != nullptr);
    TRIVERTEX arrTvx[2] = {{0}, {0}};
    arrTvx[0].x = rect.left;
    arrTvx[0].y = rect.top;
    arrTvx[0].Red = MAKEWORD(0, GetRValue(clr1));
    arrTvx[0].Green = MAKEWORD(0, GetGValue(clr1));
    arrTvx[0].Blue = MAKEWORD(0, GetBValue(clr1));
    arrTvx[0].Alpha = 0;
    arrTvx[1].x = rect.right;
    arrTvx[1].y = rect.bottom;
    arrTvx[1].Red = MAKEWORD(0, GetRValue(clr2));
    arrTvx[1].Green = MAKEWORD(0, GetGValue(clr2));
    arrTvx[1].Blue = MAKEWORD(0, GetBValue(clr2));
    arrTvx[1].Alpha = 0;
    GRADIENT_RECT gr = {0, 1};
    return ::GradientFill(m_hdc, arrTvx, 2, &gr, 1, bHorizontal
                                                        ? GRADIENT_FILL_RECT_H
                                                        : GRADIENT_FILL_RECT_V);
  }

  BOOL AlphaBlend(int x, int y, int nWidth, int nHeight, HDC hSrcDC, int xSrc,
                  int ySrc, int nSrcWidth, int nSrcHeight, BLENDFUNCTION bf) {
    WINASSERT(m_hdc != nullptr);
    return ::AlphaBlend(m_hdc, x, y, nWidth, nHeight, hSrcDC, xSrc, ySrc,
                        nSrcWidth, nSrcHeight, bf);
  }

  BOOL
  DitherBlt(int x, int y, int nWidth, int nHeight, HDC hSrcDC, HBITMAP hBitmap,
            int xSrc, int ySrc,
            HBRUSH hBrushBackground = ::GetSysColorBrush(COLOR_3DFACE),
            HBRUSH hBrush3DEffect = ::GetSysColorBrush(COLOR_3DHILIGHT),
            HBRUSH hBrushDisabledImage = ::GetSysColorBrush(COLOR_3DSHADOW)) {
    WINASSERT(m_hdc != nullptr || hBitmap != nullptr);
    WINASSERT(nWidth > 0 && nHeight > 0);

    // Create a generic DC for all BitBlts
    DCT<true> dc = (hSrcDC != nullptr) ? hSrcDC : ::CreateCompatibleDC(m_hdc);
    WINASSERT(dc.m_hdc != nullptr);
    if (dc.m_hdc == nullptr)
      return FALSE;

    // Create a DC for the monochrome DIB section
    DCT<false> dcBW = ::CreateCompatibleDC(m_hdc);
    WINASSERT(dcBW.m_hdc != nullptr);
    if (dcBW.m_hdc == nullptr) {
      if (hSrcDC == nullptr)
        dc.DeleteDC();
      return FALSE;
    }

    // Create the monochrome DIB section with a black and white palette
    struct RGBBWBITMAPINFO {
      BITMAPINFOHEADER bmiHeader;
      RGBQUAD bmiColors[2];
    };

    RGBBWBITMAPINFO rgbBWBitmapInfo = {
        {sizeof(BITMAPINFOHEADER), nWidth, nHeight, 1, 1, BI_RGB, 0, 0, 0, 0,
         0},
        {{0x00, 0x00, 0x00, 0x00}, {0xFF, 0xFF, 0xFF, 0x00}}};

    VOID *pbitsBW;
    Bitmap bmpBW = ::CreateDIBSection(dcBW, (LPBITMAPINFO)&rgbBWBitmapInfo,
                                      DIB_RGB_COLORS, &pbitsBW, nullptr, 0);
    WINASSERT(bmpBW.m_bitmap != nullptr);
    if (bmpBW.m_bitmap == nullptr) {
      if (hSrcDC == nullptr)
        dc.DeleteDC();
      return FALSE;
    }

    // Attach the monochrome DIB section and the bitmap to the DCs
    HBITMAP hbmOldBW = dcBW.SelectBitmap(bmpBW);
    HBITMAP hbmOldDC = nullptr;
    if (hBitmap != nullptr)
      hbmOldDC = dc.SelectBitmap(hBitmap);

    // Block: Dark gray removal: we want (128, 128, 128) pixels to become black
    // and not white
    {
      DCT<false> dcTemp1 = ::CreateCompatibleDC(m_hdc);
      DCT<false> dcTemp2 = ::CreateCompatibleDC(m_hdc);
      Bitmap bmpTemp1;
      bmpTemp1.CreateCompatibleBitmap(dc, nWidth, nHeight);
      Bitmap bmpTemp2;
      bmpTemp2.CreateBitmap(nWidth, nHeight, 1, 1, nullptr);
      HBITMAP hOldBmp1 = dcTemp1.SelectBitmap(bmpTemp1);
      HBITMAP hOldBmp2 = dcTemp2.SelectBitmap(bmpTemp2);
      // Let's copy our image, it will be altered
      dcTemp1.BitBlt(0, 0, nWidth, nHeight, dc, xSrc, ySrc, SRCCOPY);

      // All dark gray pixels will become white, the others black
      dcTemp1.SetBkColor(RGB(128, 128, 128));
      dcTemp2.BitBlt(0, 0, nWidth, nHeight, dcTemp1, 0, 0, SRCCOPY);
      // Do an XOR to set to black these white pixels
      dcTemp1.BitBlt(0, 0, nWidth, nHeight, dcTemp2, 0, 0, SRCINVERT);

      // BitBlt the bitmap into the monochrome DIB section
      // The DIB section will do a true monochrome conversion
      // The magenta background being closer to white will become white
      dcBW.BitBlt(0, 0, nWidth, nHeight, dcTemp1, 0, 0, SRCCOPY);

      // Cleanup
      dcTemp1.SelectBitmap(hOldBmp1);
      dcTemp2.SelectBitmap(hOldBmp2);
    }

    // Paint the destination rectangle using hBrushBackground
    if (hBrushBackground != nullptr) {
      RECT rc = {x, y, x + nWidth, y + nHeight};
      FillRect(&rc, hBrushBackground);
    }

    // BitBlt the black bits in the monochrome bitmap into hBrush3DEffect color
    // in the destination DC
    // The magic ROP comes from the Charles Petzold's book
    HBRUSH hOldBrush = SelectBrush(hBrush3DEffect);
    BitBlt(x + 1, y + 1, nWidth, nHeight, dcBW, 0, 0, 0xB8074A);

    // BitBlt the black bits in the monochrome bitmap into hBrushDisabledImage
    // color in the destination DC
    SelectBrush(hBrushDisabledImage);
    BitBlt(x, y, nWidth, nHeight, dcBW, 0, 0, 0xB8074A);

    SelectBrush(hOldBrush);
    dcBW.SelectBitmap(hbmOldBW);
    dc.SelectBitmap(hbmOldDC);

    if (hSrcDC == nullptr)
      dc.DeleteDC();

    return TRUE;
  }

  BOOL TextOut(int x, int y, LPCTSTR lpszString, int nCount = -1) {
    WINASSERT(m_hdc != nullptr);
    if (nCount == -1)
      nCount = lstrlen(lpszString);
    return ::TextOut(m_hdc, x, y, lpszString, nCount);
  }

  BOOL ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect,
                  LPCTSTR lpszString, UINT nCount = -1,
                  LPINT lpDxWidths = nullptr) {
    WINASSERT(m_hdc != nullptr);
    if (nCount == -1)
      nCount = lstrlen(lpszString);
    return ::ExtTextOut(m_hdc, x, y, nOptions, lpRect, lpszString, nCount,
                        lpDxWidths);
  }

  SIZE TabbedTextOut(int x, int y, LPCTSTR lpszString, int nCount = -1,
                     int nTabPositions = 0, LPINT lpnTabStopPositions = nullptr,
                     int nTabOrigin = 0) {
    WINASSERT(m_hdc != nullptr);
    if (nCount == -1)
      nCount = lstrlen(lpszString);
    LONG lRes = ::TabbedTextOut(m_hdc, x, y, lpszString, nCount, nTabPositions,
                                lpnTabStopPositions, nTabOrigin);
    SIZE size = {GET_X_LPARAM(lRes), GET_Y_LPARAM(lRes)};
    return size;
  }

  int DrawText(LPCTSTR lpstrText, int cchText, LPRECT lpRect, UINT uFormat) {
    WINASSERT(m_hdc != nullptr);
    WINASSERT((uFormat & DT_MODIFYSTRING) == 0);
    return ::DrawText(m_hdc, lpstrText, cchText, lpRect, uFormat);
  }

  int DrawText(LPTSTR lpstrText, int cchText, LPRECT lpRect, UINT uFormat) {
    WINASSERT(m_hdc != nullptr);
    return ::DrawText(m_hdc, lpstrText, cchText, lpRect, uFormat);
  }

  int DrawTextEx(LPTSTR lpstrText, int cchText, LPRECT lpRect, UINT uFormat,
                 LPDRAWTEXTPARAMS lpDTParams = nullptr) {
    WINASSERT(m_hdc != nullptr);
    return ::DrawTextEx(m_hdc, lpstrText, cchText, lpRect, uFormat, lpDTParams);
  }

  int DrawShadowText(LPCWSTR lpstrText, int cchText, LPRECT lpRect,
                     DWORD dwFlags, COLORREF clrText, COLORREF clrShadow,
                     int xOffset, int yOffset) {
    WINASSERT(m_hdc != nullptr);
    // This function is present only if comctl32.dll version 6 is loaded;
    // we use LoadLibrary/GetProcAddress to allow apps compiled with
    // _WIN32_WINNT >= 0x0501 to run on older Windows/CommCtrl
    int nRet = 0;
    HMODULE hCommCtrlDLL = ::LoadLibrary(TEXT("comctl32.dll"));
    WINASSERT(hCommCtrlDLL != nullptr);
    if (hCommCtrlDLL != nullptr) {
      typedef int(WINAPI * PFN_DrawShadowText)(
          HDC hDC, LPCWSTR lpstrText, UINT cchText, LPRECT lpRect,
          DWORD dwFlags, COLORREF clrText, COLORREF clrShadow, int xOffset,
          int yOffset);
      PFN_DrawShadowText pfnDrawShadowText =
          (PFN_DrawShadowText)::GetProcAddress(hCommCtrlDLL, "DrawShadowText");
      WINASSERT(pfnDrawShadowText !=
                nullptr); // this function requires CommCtrl6
      if (pfnDrawShadowText != nullptr)
        nRet = pfnDrawShadowText(m_hdc, lpstrText, cchText, lpRect, dwFlags,
                                 clrText, clrShadow, xOffset, yOffset);
      ::FreeLibrary(hCommCtrlDLL);
    }
    return nRet;
  }

  BOOL GetTextExtent(LPCTSTR lpszString, int nCount, LPSIZE lpSize) const {
    WINASSERT(m_hdc != nullptr);
    if (nCount == -1)
      nCount = lstrlen(lpszString);
    return ::GetTextExtentPoint32(m_hdc, lpszString, nCount, lpSize);
  }

  BOOL GetTextExtentExPoint(LPCTSTR lpszString, int cchString, LPSIZE lpSize,
                            int nMaxExtent, LPINT lpnFit = nullptr,
                            LPINT alpDx = nullptr) {
    WINASSERT(m_hdc != nullptr);
    return ::GetTextExtentExPoint(m_hdc, lpszString, cchString, nMaxExtent,
                                  lpnFit, alpDx, lpSize);
  }

  DWORD GetTabbedTextExtent(LPCTSTR lpszString, int nCount = -1,
                            int nTabPositions = 0,
                            LPINT lpnTabStopPositions = nullptr) const {
    WINASSERT(m_hdc != nullptr);
    if (nCount == -1)
      nCount = lstrlen(lpszString);
    return ::GetTabbedTextExtent(m_hdc, lpszString, nCount, nTabPositions,
                                 lpnTabStopPositions);
  }

  BOOL GrayString(HBRUSH hBrush, BOOL(CALLBACK *lpfnOutput)(HDC, LPARAM, int),
                  LPARAM lpData, int nCount, int x, int y, int nWidth,
                  int nHeight) {
    WINASSERT(m_hdc != nullptr);
    return ::GrayString(m_hdc, hBrush, (GRAYSTRINGPROC)lpfnOutput, lpData,
                        nCount, x, y, nWidth, nHeight);
  }

  UINT GetTextAlign() const {
    WINASSERT(m_hdc != nullptr);
    return ::GetTextAlign(m_hdc);
  }

  UINT SetTextAlign(UINT nFlags) {
    WINASSERT(m_hdc != nullptr);
    return ::SetTextAlign(m_hdc, nFlags);
  }

  int GetTextFace(LPTSTR lpszFacename, int nCount) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetTextFace(m_hdc, nCount, lpszFacename);
  }

  int GetTextFaceLen() const {
    WINASSERT(m_hdc != nullptr);
    return ::GetTextFace(m_hdc, 0, nullptr);
  }

  BOOL GetTextMetrics(LPTEXTMETRIC lpMetrics) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetTextMetrics(m_hdc, lpMetrics);
  }

  int SetTextJustification(int nBreakExtra, int nBreakCount) {
    WINASSERT(m_hdc != nullptr);
    return ::SetTextJustification(m_hdc, nBreakExtra, nBreakCount);
  }

  int GetTextCharacterExtra() const {
    WINASSERT(m_hdc != nullptr);
    return ::GetTextCharacterExtra(m_hdc);
  }

  int SetTextCharacterExtra(int nCharExtra) {
    WINASSERT(m_hdc != nullptr);
    return ::SetTextCharacterExtra(m_hdc, nCharExtra);
  }

  BOOL DrawEdge(LPRECT lpRect, UINT nEdge, UINT nFlags) {
    WINASSERT(m_hdc != nullptr);
    return ::DrawEdge(m_hdc, lpRect, nEdge, nFlags);
  }

  BOOL DrawFrameControl(LPRECT lpRect, UINT nType, UINT nState) {
    WINASSERT(m_hdc != nullptr);
    return ::DrawFrameControl(m_hdc, lpRect, nType, nState);
  }

  BOOL ScrollDC(int dx, int dy, LPCRECT lpRectScroll, LPCRECT lpRectClip,
                HRGN hRgnUpdate, LPRECT lpRectUpdate) {
    WINASSERT(m_hdc != nullptr);
    return ::ScrollDC(m_hdc, dx, dy, lpRectScroll, lpRectClip, hRgnUpdate,
                      lpRectUpdate);
  }

  BOOL GetCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetCharWidth(m_hdc, nFirstChar, nLastChar, lpBuffer);
  }

  BOOL GetCharWidth32(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetCharWidth32(m_hdc, nFirstChar, nLastChar, lpBuffer);
  }

  DWORD SetMapperFlags(DWORD dwFlag) {
    WINASSERT(m_hdc != nullptr);
    return ::SetMapperFlags(m_hdc, dwFlag);
  }

  BOOL GetAspectRatioFilter(LPSIZE lpSize) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetAspectRatioFilterEx(m_hdc, lpSize);
  }

  BOOL GetCharABCWidths(UINT nFirstChar, UINT nLastChar, LPABC lpabc) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetCharABCWidths(m_hdc, nFirstChar, nLastChar, lpabc);
  }

  DWORD GetFontData(DWORD dwTable, DWORD dwOffset, LPVOID lpData,
                    DWORD cbData) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetFontData(m_hdc, dwTable, dwOffset, lpData, cbData);
  }

  int GetKerningPairs(int nPairs, LPKERNINGPAIR lpkrnpair) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetKerningPairs(m_hdc, nPairs, lpkrnpair);
  }

  UINT GetOutlineTextMetrics(UINT cbData, LPOUTLINETEXTMETRIC lpotm) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetOutlineTextMetrics(m_hdc, cbData, lpotm);
  }

  DWORD GetGlyphOutline(UINT nChar, UINT nFormat, LPGLYPHMETRICS lpgm,
                        DWORD cbBuffer, LPVOID lpBuffer,
                        const MAT2 *lpmat2) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetGlyphOutline(m_hdc, nChar, nFormat, lpgm, cbBuffer, lpBuffer,
                             lpmat2);
  }

  BOOL GetCharABCWidths(UINT nFirstChar, UINT nLastChar,
                        LPABCFLOAT lpABCF) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetCharABCWidthsFloat(m_hdc, nFirstChar, nLastChar, lpABCF);
  }

  BOOL GetCharWidth(UINT nFirstChar, UINT nLastChar,
                    float *lpFloatBuffer) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetCharWidthFloat(m_hdc, nFirstChar, nLastChar, lpFloatBuffer);
  }

  int Escape(int nEscape, int nCount, LPCSTR lpszInData, LPVOID lpOutData) {
    WINASSERT(m_hdc != nullptr);
    return ::Escape(m_hdc, nEscape, nCount, lpszInData, lpOutData);
  }

  int Escape(int nEscape, int nInputSize, LPCSTR lpszInputData, int nOutputSize,
             LPSTR lpszOutputData) {
    WINASSERT(m_hdc != nullptr);
    return ::ExtEscape(m_hdc, nEscape, nInputSize, lpszInputData, nOutputSize,
                       lpszOutputData);
  }

  int DrawEscape(int nEscape, int nInputSize, LPCSTR lpszInputData) {
    WINASSERT(m_hdc != nullptr);
    return ::DrawEscape(m_hdc, nEscape, nInputSize, lpszInputData);
  }

  int StartDoc(LPCTSTR lpszDocName) {
    DOCINFO di = {0};
    di.cbSize = sizeof(DOCINFO);
    di.lpszDocName = lpszDocName;
    return StartDoc(&di);
  }

  int StartDoc(LPDOCINFO lpDocInfo) {
    WINASSERT(m_hdc != nullptr);
    return ::StartDoc(m_hdc, lpDocInfo);
  }

  int StartPage() {
    WINASSERT(m_hdc != nullptr);
    return ::StartPage(m_hdc);
  }

  int EndPage() {
    WINASSERT(m_hdc != nullptr);
    return ::EndPage(m_hdc);
  }

  int SetAbortProc(BOOL(CALLBACK *lpfn)(HDC, int)) {
    WINASSERT(m_hdc != nullptr);
    return ::SetAbortProc(m_hdc, (ABORTPROC)lpfn);
  }

  int AbortDoc() {
    WINASSERT(m_hdc != nullptr);
    return ::AbortDoc(m_hdc);
  }

  int EndDoc() {
    WINASSERT(m_hdc != nullptr);
    return ::EndDoc(m_hdc);
  }

  BOOL PlayMetaFile(HMETAFILE hMF) {
    WINASSERT(m_hdc != nullptr);
    if (::GetDeviceCaps(m_hdc, TECHNOLOGY) == DT_METAFILE) {
      // playing metafile in metafile, just use core windows API
      return ::PlayMetaFile(m_hdc, hMF);
    }
    return ::EnumMetaFile(m_hdc, hMF, EnumMetaFileProc, (LPARAM) this);
  }

  BOOL PlayMetaFile(HENHMETAFILE hEnhMetaFile, LPCRECT lpBounds) {
    WINASSERT(m_hdc != nullptr);
    return ::PlayEnhMetaFile(m_hdc, hEnhMetaFile, lpBounds);
  }

  BOOL AddMetaFileComment(UINT nDataSize, const BYTE *pCommentData) {
    WINASSERT(m_hdc != nullptr);
    return ::GdiComment(m_hdc, nDataSize, pCommentData);
  }

  static int CALLBACK EnumMetaFileProc(HDC hDC, HANDLETABLE *pHandleTable,
                                       METARECORD *pMetaRec, int nHandles,
                                       LPARAM lParam) {
    DCT<true> *pDC = (DCT<true> *)lParam;
    switch (pMetaRec->rdFunction) {
    case META_SETMAPMODE:
      pDC->SetMapMode((int)(short) pMetaRec->rdParm[0]);
      break;
    case META_SETWINDOWEXT:
      pDC->SetWindowExt((int)(short) pMetaRec->rdParm[1],
                        (int)(short) pMetaRec->rdParm[0]);
      break;
    case META_SETWINDOWORG:
      pDC->SetWindowOrg((int)(short) pMetaRec->rdParm[1],
                        (int)(short) pMetaRec->rdParm[0]);
      break;
    case META_SETVIEWPORTEXT:
      pDC->SetViewportExt((int)(short) pMetaRec->rdParm[1],
                          (int)(short) pMetaRec->rdParm[0]);
      break;
    case META_SETVIEWPORTORG:
      pDC->SetViewportOrg((int)(short) pMetaRec->rdParm[1],
                          (int)(short) pMetaRec->rdParm[0]);
      break;
    case META_SCALEWINDOWEXT:
      pDC->ScaleWindowExt(
          (int)(short) pMetaRec->rdParm[3], (int)(short) pMetaRec->rdParm[2],
          (int)(short) pMetaRec->rdParm[1], (int)(short) pMetaRec->rdParm[0]);
      break;
    case META_SCALEVIEWPORTEXT:
      pDC->ScaleViewportExt(
          (int)(short) pMetaRec->rdParm[3], (int)(short) pMetaRec->rdParm[2],
          (int)(short) pMetaRec->rdParm[1], (int)(short) pMetaRec->rdParm[0]);
      break;
    case META_OFFSETVIEWPORTORG:
      pDC->OffsetViewportOrg((int)(short) pMetaRec->rdParm[1],
                             (int)(short) pMetaRec->rdParm[0]);
      break;
    case META_SAVEDC:
      pDC->SaveDC();
      break;
    case META_RESTOREDC:
      pDC->RestoreDC((int)(short) pMetaRec->rdParm[0]);
      break;
    case META_SETBKCOLOR:
      pDC->SetBkColor(*(UNALIGNED COLORREF *)&pMetaRec->rdParm[0]);
      break;
    case META_SETTEXTCOLOR:
      pDC->SetTextColor(*(UNALIGNED COLORREF *)&pMetaRec->rdParm[0]);
      break;
    case META_SELECTOBJECT: {
      HGDIOBJ hObject = pHandleTable->objectHandle[pMetaRec->rdParm[0]];
      UINT nObjType = ::GetObjectType(hObject);
      if (nObjType == 0) {
        // object type is unknown, determine if it is a font
        HFONT hStockFont = (HFONT)::GetStockObject(SYSTEM_FONT);
        HFONT hFontOld = (HFONT)::SelectObject(pDC->m_hdc, hStockFont);
        HGDIOBJ hObjOld = ::SelectObject(pDC->m_hdc, hObject);
        if (hObjOld == hStockFont) {
          // got the stock object back, so must be selecting a font
          pDC->SelectFont((HFONT)hObject);
          break; // don't play the default record
        } else {
          // didn't get the stock object back, so restore everything
          ::SelectObject(pDC->m_hdc, hFontOld);
          ::SelectObject(pDC->m_hdc, hObjOld);
        }
        // and fall through to PlayMetaFileRecord...
      } else if (nObjType == OBJ_FONT) {
        // play back as DCT<true>::SelectFont(HFONT)
        pDC->SelectFont((HFONT)hObject);
        break; // don't play the default record
      }
    }
    default:
      ::PlayMetaFileRecord(hDC, pHandleTable, pMetaRec, nHandles);
      break;
    }

    return 1;
  }

  BOOL AbortPath() {
    WINASSERT(m_hdc != nullptr);
    return ::AbortPath(m_hdc);
  }

  BOOL BeginPath() {
    WINASSERT(m_hdc != nullptr);
    return ::BeginPath(m_hdc);
  }

  BOOL CloseFigure() {
    WINASSERT(m_hdc != nullptr);
    return ::CloseFigure(m_hdc);
  }

  BOOL EndPath() {
    WINASSERT(m_hdc != nullptr);
    return ::EndPath(m_hdc);
  }

  BOOL FillPath() {
    WINASSERT(m_hdc != nullptr);
    return ::FillPath(m_hdc);
  }

  BOOL FlattenPath() {
    WINASSERT(m_hdc != nullptr);
    return ::FlattenPath(m_hdc);
  }

  BOOL StrokeAndFillPath() {
    WINASSERT(m_hdc != nullptr);
    return ::StrokeAndFillPath(m_hdc);
  }

  BOOL StrokePath() {
    WINASSERT(m_hdc != nullptr);
    return ::StrokePath(m_hdc);
  }

  BOOL WidenPath() {
    WINASSERT(m_hdc != nullptr);
    return ::WidenPath(m_hdc);
  }

  BOOL GetMiterLimit(PFLOAT pfMiterLimit) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetMiterLimit(m_hdc, pfMiterLimit);
  }

  BOOL SetMiterLimit(float fMiterLimit) {
    WINASSERT(m_hdc != nullptr);
    return ::SetMiterLimit(m_hdc, fMiterLimit, nullptr);
  }

  int GetPath(LPPOINT lpPoints, LPBYTE lpTypes, int nCount) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetPath(m_hdc, lpPoints, lpTypes, nCount);
  }

  BOOL SelectClipPath(int nMode) {
    WINASSERT(m_hdc != nullptr);
    return ::SelectClipPath(m_hdc, nMode);
  }

  static BrushHandle PASCAL GetHalftoneBrush() {
    HBRUSH halftoneBrush = nullptr;
    WORD grayPattern[8] = {0};
    for (int i = 0; i < 8; i++)
      grayPattern[i] = (WORD)(0x5555 << (i & 1));
    HBITMAP grayBitmap = CreateBitmap(8, 8, 1, 1, &grayPattern);
    if (grayBitmap != nullptr) {
      halftoneBrush = ::CreatePatternBrush(grayBitmap);
      DeleteObject(grayBitmap);
    }
    return BrushHandle(halftoneBrush);
  }

  void DrawDragRect(LPCRECT lpRect, SIZE size, LPCRECT lpRectLast,
                    SIZE sizeLast, HBRUSH hBrush = nullptr,
                    HBRUSH hBrushLast = nullptr) {
    Rgn rgnOutside;
    rgnOutside.CreateRectRgnIndirect(lpRect);
    RECT rect = *lpRect;
    ::InflateRect(&rect, -size.cx, -size.cy);
    ::IntersectRect(&rect, &rect, lpRect);
    Rgn rgnInside;
    rgnInside.CreateRectRgnIndirect(&rect);
    Rgn rgnNew;
    rgnNew.CreateRectRgn(0, 0, 0, 0);
    rgnNew.CombineRgn(rgnOutside, rgnInside, RGN_XOR);

    HBRUSH hBrushOld = nullptr;
    Brush brushHalftone;
    if (hBrush == nullptr)
      brushHalftone = hBrush = DCT<true>::GetHalftoneBrush();
    if (hBrushLast == nullptr)
      hBrushLast = hBrush;

    Rgn rgnLast;
    Rgn rgnUpdate;
    if (lpRectLast != nullptr) {
      rgnLast.CreateRectRgn(0, 0, 0, 0);
      rgnOutside.SetRectRgn(lpRectLast->left, lpRectLast->top,
                            lpRectLast->right, lpRectLast->bottom);
      rect = *lpRectLast;
      ::InflateRect(&rect, -sizeLast.cx, -sizeLast.cy);
      ::IntersectRect(&rect, &rect, lpRectLast);
      rgnInside.SetRectRgn(rect.left, rect.top, rect.right, rect.bottom);
      rgnLast.CombineRgn(rgnOutside, rgnInside, RGN_XOR);

      if (hBrush == hBrushLast) {
        rgnUpdate.CreateRectRgn(0, 0, 0, 0);
        rgnUpdate.CombineRgn(rgnLast, rgnNew, RGN_XOR);
      }
    }
    if (hBrush != hBrushLast && lpRectLast != nullptr) {
      SelectClipRgn(rgnLast);
      GetClipBox(&rect);
      hBrushOld = SelectBrush(hBrushLast);
      PatBlt(rect.left, rect.top, rect.right - rect.left,
             rect.bottom - rect.top, PATINVERT);
      SelectBrush(hBrushOld);
      hBrushOld = nullptr;
    }
    SelectClipRgn(rgnUpdate.IsNull() ? rgnNew : rgnUpdate);
    GetClipBox(&rect);
    hBrushOld = SelectBrush(hBrush);
    PatBlt(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
           PATINVERT);
    if (hBrushOld != nullptr)
      SelectBrush(hBrushOld);
    SelectClipRgn(nullptr);
  }

  void FillSolidRect(LPCRECT lpRect, COLORREF clr) {
    WINASSERT(m_hdc != nullptr);
    COLORREF clrOld = ::SetBkColor(m_hdc, clr);
    WINASSERT(clrOld != CLR_INVALID);
    if (clrOld != CLR_INVALID) {
      ::ExtTextOut(m_hdc, 0, 0, ETO_OPAQUE, lpRect, nullptr, 0, nullptr);
      ::SetBkColor(m_hdc, clrOld);
    }
  }

  void FillSolidRect(int x, int y, int cx, int cy, COLORREF clr) {
    WINASSERT(m_hdc != nullptr);
    RECT rect = {x, y, x + cx, y + cy};
    FillSolidRect(&rect, clr);
  }

  void Draw3dRect(LPCRECT lpRect, COLORREF clrTopLeft,
                  COLORREF clrBottomRight) {
    Draw3dRect(lpRect->left, lpRect->top, lpRect->right - lpRect->left,
               lpRect->bottom - lpRect->top, clrTopLeft, clrBottomRight);
  }

  void Draw3dRect(int x, int y, int cx, int cy, COLORREF clrTopLeft,
                  COLORREF clrBottomRight) {
    FillSolidRect(x, y, cx - 1, 1, clrTopLeft);
    FillSolidRect(x, y, 1, cy - 1, clrTopLeft);
    FillSolidRect(x + cx, y, -1, cy, clrBottomRight);
    FillSolidRect(x, y + cy, cx, -1, clrBottomRight);
  }

  int SetDIBitsToDevice(int x, int y, DWORD dwWidth, DWORD dwHeight, int xSrc,
                        int ySrc, UINT uStartScan, UINT cScanLines,
                        CONST VOID *lpvBits, CONST BITMAPINFO *lpbmi,
                        UINT uColorUse) {
    WINASSERT(m_hdc != nullptr);
    return ::SetDIBitsToDevice(m_hdc, x, y, dwWidth, dwHeight, xSrc, ySrc,
                               uStartScan, cScanLines, lpvBits, lpbmi,
                               uColorUse);
  }

  int StretchDIBits(int x, int y, int nWidth, int nHeight, int xSrc, int ySrc,
                    int nSrcWidth, int nSrcHeight, CONST VOID *lpvBits,
                    CONST BITMAPINFO *lpbmi, UINT uColorUse, DWORD dwRop) {
    WINASSERT(m_hdc != nullptr);
    return ::StretchDIBits(m_hdc, x, y, nWidth, nHeight, xSrc, ySrc, nSrcWidth,
                           nSrcHeight, lpvBits, lpbmi, uColorUse, dwRop);
  }

  UINT GetDIBColorTable(UINT uStartIndex, UINT cEntries,
                        RGBQUAD *pColors) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetDIBColorTable(m_hdc, uStartIndex, cEntries, pColors);
  }

  UINT SetDIBColorTable(UINT uStartIndex, UINT cEntries,
                        CONST RGBQUAD *pColors) {
    WINASSERT(m_hdc != nullptr);
    return ::SetDIBColorTable(m_hdc, uStartIndex, cEntries, pColors);
  }

  int ChoosePixelFormat(CONST PIXELFORMATDESCRIPTOR *ppfd) {
    WINASSERT(m_hdc != nullptr);
    return ::ChoosePixelFormat(m_hdc, ppfd);
  }

  int DescribePixelFormat(int iPixelFormat, UINT nBytes,
                          LPPIXELFORMATDESCRIPTOR ppfd) {
    WINASSERT(m_hdc != nullptr);
    return ::DescribePixelFormat(m_hdc, iPixelFormat, nBytes, ppfd);
  }

  int GetPixelFormat() const {
    WINASSERT(m_hdc != nullptr);
    return ::GetPixelFormat(m_hdc);
  }

  BOOL SetPixelFormat(int iPixelFormat, CONST PIXELFORMATDESCRIPTOR *ppfd) {
    WINASSERT(m_hdc != nullptr);
    return ::SetPixelFormat(m_hdc, iPixelFormat, ppfd);
  }

  BOOL SwapBuffers() {
    WINASSERT(m_hdc != nullptr);
    return ::SwapBuffers(m_hdc);
  }

  HGLRC wglCreateContext() {
    WINASSERT(m_hdc != nullptr);
    return ::wglCreateContext(m_hdc);
  }

  HGLRC wglCreateLayerContext(int iLayerPlane) {
    WINASSERT(m_hdc != nullptr);
    return ::wglCreateLayerContext(m_hdc, iLayerPlane);
  }

  BOOL wglMakeCurrent(HGLRC hglrc) {
    WINASSERT(m_hdc != nullptr);
    return ::wglMakeCurrent(m_hdc, hglrc);
  }

  BOOL wglUseFontBitmaps(DWORD dwFirst, DWORD dwCount, DWORD listBase) {
    WINASSERT(m_hdc != nullptr);
    return ::wglUseFontBitmaps(m_hdc, dwFirst, dwCount, listBase);
  }

  BOOL wglUseFontOutlines(DWORD dwFirst, DWORD dwCount, DWORD listBase,
                          FLOAT deviation, FLOAT extrusion, int format,
                          LPGLYPHMETRICSFLOAT lpgmf) {
    WINASSERT(m_hdc != nullptr);
    return ::wglUseFontOutlines(m_hdc, dwFirst, dwCount, listBase, deviation,
                                extrusion, format, lpgmf);
  }

  BOOL wglDescribeLayerPlane(int iPixelFormat, int iLayerPlane, UINT nBytes,
                             LPLAYERPLANEDESCRIPTOR plpd) {
    WINASSERT(m_hdc != nullptr);
    return ::wglDescribeLayerPlane(m_hdc, iPixelFormat, iLayerPlane, nBytes,
                                   plpd);
  }

  int wglSetLayerPaletteEntries(int iLayerPlane, int iStart, int cEntries,
                                CONST COLORREF *pclr) {
    WINASSERT(m_hdc != nullptr);
    return ::wglSetLayerPaletteEntries(m_hdc, iLayerPlane, iStart, cEntries,
                                       pclr);
  }

  int wglGetLayerPaletteEntries(int iLayerPlane, int iStart, int cEntries,
                                COLORREF *pclr) {
    WINASSERT(m_hdc != nullptr);
    return ::wglGetLayerPaletteEntries(m_hdc, iLayerPlane, iStart, cEntries,
                                       pclr);
  }

  BOOL wglRealizeLayerPalette(int iLayerPlane, BOOL bRealize) {
    WINASSERT(m_hdc != nullptr);
    return ::wglRealizeLayerPalette(m_hdc, iLayerPlane, bRealize);
  }

  BOOL wglSwapLayerBuffers(UINT uPlanes) {
    WINASSERT(m_hdc != nullptr);
    return ::wglSwapLayerBuffers(m_hdc, uPlanes);
  }

  COLORREF GetDCPenColor() const {
    WINASSERT(m_hdc != nullptr);
    return ::GetDCPenColor(m_hdc);
  }

  COLORREF SetDCPenColor(COLORREF clr) {
    WINASSERT(m_hdc != nullptr);
    return ::SetDCPenColor(m_hdc, clr);
  }

  COLORREF GetDCBrushColor() const {
    WINASSERT(m_hdc != nullptr);
    return ::GetDCBrushColor(m_hdc);
  }

  COLORREF SetDCBrushColor(COLORREF clr) {
    WINASSERT(m_hdc != nullptr);
    return ::SetDCBrushColor(m_hdc, clr);
  }

  DWORD GetFontUnicodeRanges(LPGLYPHSET lpgs) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetFontUnicodeRanges(m_hdc, lpgs);
  }

  DWORD GetGlyphIndices(LPCTSTR lpstr, int cch, LPWORD pgi,
                        DWORD dwFlags) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetGlyphIndices(m_hdc, lpstr, cch, pgi, dwFlags);
  }

  BOOL GetTextExtentPointI(LPWORD pgiIn, int cgi, LPSIZE lpSize) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetTextExtentPointI(m_hdc, pgiIn, cgi, lpSize);
  }

  BOOL GetTextExtentExPointI(LPWORD pgiIn, int cgi, int nMaxExtent,
                             LPINT lpnFit, LPINT alpDx, LPSIZE lpSize) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetTextExtentExPointI(m_hdc, pgiIn, cgi, nMaxExtent, lpnFit, alpDx,
                                   lpSize);
  }

  BOOL GetCharWidthI(UINT giFirst, UINT cgi, LPWORD pgi, LPINT lpBuffer) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetCharWidthI(m_hdc, giFirst, cgi, pgi, lpBuffer);
  }

  BOOL GetCharABCWidthsI(UINT giFirst, UINT cgi, LPWORD pgi,
                         LPABC lpabc) const {
    WINASSERT(m_hdc != nullptr);
    return ::GetCharABCWidthsI(m_hdc, giFirst, cgi, pgi, lpabc);
  }

  BOOL ColorCorrectPalette(HPALETTE hPalette, DWORD dwFirstEntry,
                           DWORD dwNumOfEntries) {
    WINASSERT(m_hdc != nullptr);
    return ::ColorCorrectPalette(m_hdc, hPalette, dwFirstEntry, dwNumOfEntries);
  }
};

typedef DCT<false> DCHandle;
typedef DCT<true> DC;

class PaintDC : public DC {
public:
  HWND m_hwnd;
  PAINTSTRUCT m_ps;

  PaintDC(HWND hWnd) {
    WINASSERT(::IsWindow(hWnd));
    m_hwnd = hWnd;
    m_hdc = ::BeginPaint(hWnd, &m_ps);
  }

  ~PaintDC() {
    WINASSERT(m_hdc != nullptr);
    WINASSERT(::IsWindow(m_hwnd));
    ::EndPaint(m_hwnd, &m_ps);
    Detach();
  }
};

class ClientDC : public DC {
public:
  HWND m_hwnd;

  ClientDC(HWND hWnd) {
    WINASSERT(hWnd == nullptr || ::IsWindow(hWnd));
    m_hwnd = hWnd;
    m_hdc = ::GetDC(hWnd);
  }

  ~ClientDC() {
    WINASSERT(m_hdc != nullptr);
    ::ReleaseDC(m_hwnd, Detach());
  }
};

class WindowDC : public DC {
public:
  HWND m_hwnd;

  WindowDC(HWND hWnd) {
    WINASSERT(hWnd == nullptr || ::IsWindow(hWnd));
    m_hwnd = hWnd;
    m_hdc = ::GetWindowDC(hWnd);
  }

  ~WindowDC() {
    WINASSERT(m_hdc != nullptr);
    ::ReleaseDC(m_hwnd, Detach());
  }
};

class MemoryDC : public DC {
public:
  HDC m_hdcOriginal;
  RECT m_rcPaint;
  Bitmap m_bmp;
  HBITMAP m_hBmpOld;

  MemoryDC(HDC hDC, const RECT &rcPaint)
      : m_hdcOriginal(hDC), m_hBmpOld(nullptr) {
    m_rcPaint = rcPaint;
    CreateCompatibleDC(m_hdcOriginal);
    WINASSERT(m_hdc != nullptr);
    m_bmp.CreateCompatibleBitmap(m_hdcOriginal,
                                 m_rcPaint.right - m_rcPaint.left,
                                 m_rcPaint.bottom - m_rcPaint.top);
    WINASSERT(m_bmp.m_bitmap != nullptr);
    m_hBmpOld = SelectBitmap(m_bmp);
    SetViewportOrg(-m_rcPaint.left, -m_rcPaint.top);
  }

  ~MemoryDC() {
    ::BitBlt(m_hdcOriginal, m_rcPaint.left, m_rcPaint.top,
             m_rcPaint.right - m_rcPaint.left, m_rcPaint.bottom - m_rcPaint.top,
             m_hdc, m_rcPaint.left, m_rcPaint.top, SRCCOPY);
    SelectBitmap(m_hBmpOld);
  }
};

class EnhMetaFileInfo {
public:
  HENHMETAFILE m_emf;
  BYTE *m_bits;
  TCHAR *m_desc;
  ENHMETAHEADER m_header;
  PIXELFORMATDESCRIPTOR m_pfd;

  EnhMetaFileInfo(HENHMETAFILE hEMF)
      : m_bits(nullptr), m_desc(nullptr), m_emf(hEMF) {}

  ~EnhMetaFileInfo() {
    delete[] m_bits;
    delete[] m_desc;
  }

  BYTE *GetEnhMetaFileBits() {
    WINASSERT(m_emf != nullptr);
    UINT nBytes = ::GetEnhMetaFileBits(m_emf, 0, nullptr);
    delete[] m_bits;
    m_bits = nullptr;
    m_bits = new BYTE[nBytes];
    if (m_bits != nullptr)
      ::GetEnhMetaFileBits(m_emf, nBytes, m_bits);
    return m_bits;
  }

  LPTSTR GetEnhMetaFileDescription() {
    WINASSERT(m_emf != nullptr);
    UINT nLen = ::GetEnhMetaFileDescription(m_emf, 0, nullptr);
    delete[] m_desc;
    m_desc = nullptr;
    m_desc = new TCHAR[nLen];
    if (m_desc != nullptr)
      nLen = ::GetEnhMetaFileDescription(m_emf, nLen, m_desc);
    return m_desc;
  }

  ENHMETAHEADER *GetEnhMetaFileHeader() {
    WINASSERT(m_emf != nullptr);
    memset(&m_header, 0, sizeof(m_header));
    m_header.iType = EMR_HEADER;
    m_header.nSize = sizeof(ENHMETAHEADER);
    UINT n = ::GetEnhMetaFileHeader(m_emf, sizeof(ENHMETAHEADER), &m_header);
    return (n != 0) ? &m_header : nullptr;
  }

  PIXELFORMATDESCRIPTOR *GetEnhMetaFilePixelFormat() {
    WINASSERT(m_emf != nullptr);
    memset(&m_pfd, 0, sizeof(m_pfd));
    UINT n = ::GetEnhMetaFilePixelFormat(m_emf, sizeof(m_pfd), &m_pfd);
    return (n != 0) ? &m_pfd : nullptr;
  }
};

template <bool Managed> class EnhMetaFileT {
public:
  HENHMETAFILE m_emf;

  EnhMetaFileT(HENHMETAFILE hEMF = nullptr) : m_emf(hEMF) {}

  ~EnhMetaFileT() {
    if (Managed && m_emf != nullptr)
      DeleteObject();
  }

  EnhMetaFileT<Managed> &operator=(HENHMETAFILE hEMF) {
    Attach(hEMF);
    return *this;
  }

  void Attach(HENHMETAFILE hEMF) {
    if (Managed && m_emf != nullptr && m_emf != hEMF)
      DeleteObject();
    m_emf = hEMF;
  }

  HENHMETAFILE Detach() {
    HENHMETAFILE hEMF = m_emf;
    m_emf = nullptr;
    return hEMF;
  }

  operator HENHMETAFILE() const { return m_emf; }

  bool IsNull() const { return (m_emf == nullptr); }

  BOOL DeleteObject() {
    WINASSERT(m_emf != nullptr);
    BOOL bRet = ::DeleteEnhMetaFile(m_emf);
    m_emf = nullptr;
    return bRet;
  }

  UINT GetEnhMetaFileBits(UINT cbBuffer, LPBYTE lpbBuffer) const {
    WINASSERT(m_emf != nullptr);
    return ::GetEnhMetaFileBits(m_emf, cbBuffer, lpbBuffer);
  }

  UINT GetEnhMetaFileDescription(UINT cchBuffer, LPTSTR lpszDescription) const {
    WINASSERT(m_emf != nullptr);
    return ::GetEnhMetaFileDescription(m_emf, cchBuffer, lpszDescription);
  }

  UINT GetEnhMetaFileHeader(LPENHMETAHEADER lpemh) const {
    WINASSERT(m_emf != nullptr);
    lpemh->iType = EMR_HEADER;
    lpemh->nSize = sizeof(ENHMETAHEADER);
    return ::GetEnhMetaFileHeader(m_emf, sizeof(ENHMETAHEADER), lpemh);
  }

  UINT GetEnhMetaFilePaletteEntries(UINT cEntries, LPPALETTEENTRY lppe) const {
    WINASSERT(m_emf != nullptr);
    return ::GetEnhMetaFilePaletteEntries(m_emf, cEntries, lppe);
  }

  UINT GetEnhMetaFilePixelFormat(DWORD cbBuffer,
                                 PIXELFORMATDESCRIPTOR *ppfd) const {
    WINASSERT(m_emf != nullptr);
    return ::GetEnhMetaFilePixelFormat(m_emf, cbBuffer, ppfd);
  }
};

typedef EnhMetaFileT<false> EnhMetaFileHandle;
typedef EnhMetaFileT<true> EnhMetaFile;

class EnhMetaFileDC : public DC {
public:
  EnhMetaFileDC() {}

  EnhMetaFileDC(HDC hdc, LPCRECT lpRect) {
    Create(hdc, nullptr, lpRect, nullptr);
    WINASSERT(m_hdc != nullptr);
  }

  EnhMetaFileDC(HDC hdcRef, LPCTSTR lpFilename, LPCRECT lpRect,
                LPCTSTR lpDescription) {
    Create(hdcRef, lpFilename, lpRect, lpDescription);
    WINASSERT(m_hdc != nullptr);
  }

  ~EnhMetaFileDC() {
    HENHMETAFILE hEMF = Close();
    if (hEMF != nullptr)
      ::DeleteEnhMetaFile(hEMF);
  }

  void Create(HDC hdcRef, LPCTSTR lpFilename, LPCRECT lpRect,
              LPCTSTR lpDescription) {
    WINASSERT(m_hdc == nullptr);
    m_hdc = ::CreateEnhMetaFile(hdcRef, lpFilename, lpRect, lpDescription);
  }

  HENHMETAFILE Close() {
    HENHMETAFILE hEMF = nullptr;
    if (m_hdc != nullptr) {
      hEMF = ::CloseEnhMetaFile(m_hdc);
      m_hdc = nullptr;
    }
    return hEMF;
  }
};
#endif

#define DECLARE_WND_CLASS(WndClassName, style, bkgrnd)                         \
  static WNDCLASSEX GetWinClassInfo() {                                        \
    static WNDCLASSEX wc = {sizeof(WNDCLASSEX),                                \
                            style,                                             \
                            WindowProc,                                        \
                            0,                                                 \
                            0,                                                 \
                            HINST_THISCOMPONENT,                               \
                            (HICON)::LoadIcon(nullptr, IDI_WINLOGO),           \
                            (HCURSOR)::LoadCursor(nullptr, IDC_ARROW),         \
                            (HBRUSH)(bkgrnd + 1),                              \
                            nullptr,                                           \
                            WndClassName,                                      \
                            nullptr};                                          \
    return wc;                                                                 \
  }                                                                            \
  static LPTSTR GetWinClassName() { return WndClassName; }

#define DECLARE_WND_CLASS_1(WndClassName)                                      \
  static WNDCLASSEX GetWinClassInfo() {                                        \
    static WNDCLASSEX wc = {sizeof(WNDCLASSEX),                                \
                            CS_HREDRAW | CS_VREDRAW,                           \
                            WindowProc,                                        \
                            0,                                                 \
                            0,                                                 \
                            HINST_THISCOMPONENT,                               \
                            (HICON)::LoadIcon(nullptr, IDI_APPLICATION),       \
                            (HCURSOR)::LoadCursor(nullptr, IDC_ARROW),         \
                            (HBRUSH)(COLOR_WINDOW + 1),                        \
                            nullptr,                                           \
                            WndClassName,                                      \
                            nullptr};                                          \
    return wc;                                                                 \
  }                                                                            \
  static LPTSTR GetWinClassName() { return WndClassName; }

enum class AnimateType : DWORD {
  Activate = AW_ACTIVATE,
  ActivateBlend = AW_ACTIVATE | AW_BLEND,
  ActivateCenter = AW_ACTIVATE | AW_CENTER,
  Blend = AW_BLEND,
  Center = AW_CENTER,
  Hide = AW_HIDE,
  HideCenter = AW_HIDE | AW_CENTER,
  HideBlend = AW_HIDE | AW_BLEND,
  SlideRight = AW_SLIDE | AW_HOR_POSITIVE,
  SlideLeft = AW_SLIDE | AW_HOR_NEGATIVE,
  SlideUp = AW_SLIDE | AW_VER_POSITIVE,
  SlideDown = AW_SLIDE | AW_VER_NEGATIVE
};

enum class ShowWindowType : int {
  Hide,
  Normal,
  ShowMinimized,
  ShowMaximized,
  NoActivate,
  Show,
  Minimized,
  MinNoActivate,
  ShowNoActivate,
  Restore,
  Default,
  ForceMimize
};

enum class MessageBoxButtons : UINT {
    Ok,
    OkCancel,
    AbortRetryIgnore,
    YesNoCancel,
    YesNo,
    RetryCancel,
    CancelTryContinue,
    Help
};

enum class MessageBoxIcon : UINT {
    Hand = 0x00000010L,
    Error = 0x00000010L,
    Stop = 0x00000010L,
    Question = 0x00000020L,
    Warning = 0x00000030L,
    Exclamation = 0x00000030L,
    Information = 0x00000040L,
    Asterisk = 0x00000040L
};

enum class MessageBoxDefaultButton : UINT {
    Button1 = 0x00000000L,
    Button2 = 0x00000100L,
    Button3 = 0x00000200L,
    Button4 = 0x00000300L
};

enum class MessageBoxModalType : UINT {
    ApplicationModal,
    SystemModal = 0x00001000L,
    TaskModal = 0x00002000L
};

enum class MessageBoxOtherOptions : UINT {
    DefaultDesktopOnly = 0x00020000L,
    RightJustified = 0x00080000L,
    RightToLeftReading = 0x00100000L,
    SetForeGround = 0x00010000L,
    TopMost = 0x00040000L,
    ServiceNotification = 0x00200000L
};

enum class MessageBoxResult : UINT {
    Ok = 1,
    Cancel,
    Abort,
    Retry,
    Ignore,
    Yes,
    No,
    TryAgain = 10
};

enum class DCCreationFlags : DWORD {
    Window = DCX_WINDOW,
    Cache = DCX_CACHE,
    ParentClip = DCX_PARENTCLIP,
    ClipSiblings = DCX_CLIPSIBLINGS,
    ClipChildren = DCX_CLIPCHILDREN,
    NoResetAttributes = DCX_NORESETATTRS,
    LockWindowUpdate = DCX_LOCKWINDOWUPDATE,
    ExcludeRegion = DCX_EXCLUDERGN,
    Intersection = DCX_INTERSECTRGN,
    Validate = DCX_VALIDATE
};

enum class PrintFlag : DWORD {
    CheckVisible = PRF_CHECKVISIBLE,
    Children = PRF_CHILDREN,
    Client = PRF_CLIENT,
    EraseBackground = PRF_ERASEBKGND,
    NonClient = PRF_NONCLIENT,
    Owned = PRF_OWNED
};

class PrintFlags{
    DWORD m_flag;
public:
    PrintFlags(PrintFlag flag) : m_flag(0) {
        m_flag = static_cast<DWORD>(flag);
    }

    PrintFlags(std::initializer_list<PrintFlag> flags) : m_flag(0){
        for(auto i = flags.begin(); i != flags.end(); ++i){
            m_flag |= static_cast<DWORD>(*i);
        }
    }

    operator DWORD() { return m_flag; }
};

enum class GWLFlag : int {
    ExStyle = -20,
    HINSTANCE = -6,
    HwndParent = -8,
    ID = -12,
    Style = -16,
    UserData = -21,
    WndProc = -4,
    DialogProc = DWLP_MSGRESULT + sizeof(LRESULT),
    MsgResult = 0,
    User = DWLP_DLGPROC + sizeof(DLGPROC)
};

enum class WindowsMessage : unsigned int {
    Null = WM_NULL,
    Create = WM_CREATE,
    Destroy = WM_DESTROY,
    Move = WM_MOVE,
    Size = WM_SIZE,
    Activate = WM_ACTIVATE,
    SetFocus = WM_SETFOCUS,
    KillFocus = WM_KILLFOCUS,
    Enable = WM_ENABLE,
    SetReDraw = WM_SETREDRAW,
    SetText = WM_SETTEXT,
    GetText = WM_GETTEXT,
    GetTextLength = WM_GETTEXTLENGTH,
    Paint = WM_PAINT,
    Close = WM_CLOSE,
    QueryEndSession = WM_QUERYENDSESSION,
    Quit = WM_QUIT,
    EraseBackground = WM_ERASEBKGND,
    SysColorChange = WM_SYSCOLORCHANGE,
    EndSession = WM_ENDSESSION,
    SystemError = 0x17,
    ShowWindow = WM_SHOWWINDOW,
    ControlColor = WM_CTLCOLOR,
    WinInitChange = WM_WININICHANGE,
    SettingChange = WM_SETTINGCHANGE,
    DevModeChange = WM_DEVMODECHANGE,
    ActivateApp = WM_ACTIVATEAPP,
    FontChange = WM_FONTCHANGE,
    TimeChange = WM_TIMECHANGE,
    CancelMode = WM_CANCELMODE,
    SetCursor = WM_SETCURSOR,
    MouseActivate = WM_MOUSEACTIVATE,
    ChildActivate = WM_CHILDACTIVATE,
    QueUESync = WM_QUEUESYNC,
    GetMinMaxInfo = WM_GETMINMAXINFO,
    PaintIcon = WM_PAINTICON,
    IconEraseBackground = WM_ICONERASEBKGND,
    NextDialogControl = WM_NEXTDLGCTL,
    SpoolerStatus = WM_SPOOLERSTATUS,
    DrawItem = WM_DRAWITEM,
    MeasureItem = WM_MEASUREITEM,
    DeleteItem = WM_DELETEITEM,
    VKeyToItem = WM_VKEYTOITEM,
    CharToItem = WM_CHARTOITEM,
    SetFont = WM_SETFONT,
    GetFont= WM_GETFONT,
    SetHotKey = WM_SETHOTKEY,
    GetHotKey = WM_GETHOTKEY,
    QueryDragIcon = WM_QUERYDRAGICON,
    CompareItem = WM_COMPAREITEM,
    Compacting = WM_COMPACTING,
    WindowPosChanging = WM_WINDOWPOSCHANGING,
    WindowPosChanged = WM_WINDOWPOSCHANGED,
    Power = WM_POWER,
    CopyData = WM_COPYDATA,
    CancelJournal = WM_CANCELJOURNAL,
    Notify = WM_NOTIFY,
    InputLanguageChangeRequest = WM_INPUTLANGCHANGEREQUEST,
    InputLanguageChange = WM_INPUTLANGCHANGE,
    TCard = WM_TCARD,
    Help = WM_HELP,
    UserChanged = WM_USERCHANGED,
    NotifyFormat = WM_NOTIFYFORMAT ,
    ContextMenu = WM_CONTEXTMENU,
    StyleChanging = WM_STYLECHANGING,
    StyleChanged = WM_STYLECHANGED,
    DislayChange = WM_DISPLAYCHANGE,
    GetIcon = WM_GETICON,
    SetIcon = WM_SETICON,
    NcCreate = WM_NCCREATE,
    NcDestroy = WM_NCDESTROY,
    NcCalcSize = WM_NCCALCSIZE,
    NcHitTest = WM_NCHITTEST,
    NcPaint = WM_NCPAINT,
    NcActivate = WM_NCACTIVATE,
    GetDialogCode = WM_GETDLGCODE,
    NcMouseMove = WM_NCMOUSEMOVE,
    NcLButtonDown = WM_NCLBUTTONDOWN,
    NcLButtonUp = WM_NCLBUTTONUP,
    NcLButtonDoubleClick = WM_NCLBUTTONDBLCLK,
    NcRButtonDown = WM_NCRBUTTONDOWN,
    NcRButtonUp = WM_NCRBUTTONUP,
    NcRButtonDoubleClick = WM_NCRBUTTONDBLCLK,
    NcMButtonDown = WM_NCMBUTTONDOWN,
    NcMButtonUp = WM_NCMBUTTONUP,
    NcMButtonDoubleClick = WM_NCMBUTTONDBLCLK,
    KeyFirst = WM_KEYFIRST,
    KeyDown  = WM_KEYDOWN,
    KeyUp = WM_KEYUP,
    Char = WM_CHAR,
    DeadChar = WM_DEADCHAR,
    SysKeyDown = WM_SYSKEYDOWN,
    SysKeyUp = WM_SYSKEYUP,
    SysChar = WM_SYSCHAR,
    SysDeadChar = WM_SYSDEADCHAR,
    KeyLast = WM_KEYLAST,
    IME_StartComposition = WM_IME_STARTCOMPOSITION,
    IME_EndComposition = WM_IME_ENDCOMPOSITION,
    IME_Composition = WM_IME_COMPOSITION,
    IME_KeyLast = WM_IME_KEYLAST,
    InitDialog = WM_INITDIALOG,
    Command = WM_COMMAND,
    SysCommand = WM_SYSCOMMAND,
    Timer = WM_TIMER,
    HScroll= WM_HSCROLL,
    VScroll = WM_VSCROLL,
    InitMenu = WM_INITMENU,
    InitMenuPopUp = WM_INITMENUPOPUP,
    MenuSelect = WM_MENUSELECT,
    MenuChar = WM_MENUCHAR,
    EnterIdle = WM_ENTERIDLE,
    ControlColorMessageBox = WM_CTLCOLORMSGBOX,
    ControlColorEdit = WM_CTLCOLOREDIT,
    ControlColorListBox = WM_CTLCOLORLISTBOX,
    ControlColorButton = WM_CTLCOLORBTN,
    ControlColorDialog = WM_CTLCOLORDLG,
    ControlColorScrollBar = WM_CTLCOLORSCROLLBAR,
    ControlColorStatic = WM_CTLCOLORSTATIC,
    MouseFirst = WM_MOUSEFIRST,
    MouseMove = WM_MOUSEMOVE,
    LButtonDown = WM_LBUTTONDOWN,
    LButtonUp = WM_LBUTTONUP,
    LButtonDoubleClick = WM_LBUTTONDBLCLK,
    RButtonDown = WM_RBUTTONDOWN,
    RButtonUp = WM_RBUTTONUP,
    RButtonDoubleClick = WM_RBUTTONDBLCLK,
    MButtonDown = WM_MBUTTONDOWN,
    MButtonUp = WM_MBUTTONUP,
    MButtonDoulbeClick = WM_MBUTTONDBLCLK,
    MouseWheel = WM_MOUSEWHEEL,
    MouseHWheel = 0x20E,
    ParentNotify = WM_PARENTNOTIFY,
    EnterMenuLoop = WM_ENTERMENULOOP,
    ExitMenuLoop = WM_EXITMENULOOP,
    NextMenu = WM_NEXTMENU,
    Sizing = WM_SIZING,
    CaptureChanged = WM_CAPTURECHANGED ,
    Moving = WM_MOVING,
    PowerBroadcast = WM_POWERBROADCAST,
    DeviceChange = WM_DEVICECHANGE,
    MdiCreate = WM_MDICREATE,
    MdiDestroy = WM_MDIDESTROY,
    MdiActivate = WM_MDIACTIVATE,
    MdiRestore = WM_MDIRESTORE,
    MdiNext = WM_MDINEXT,
    MdiMaximize = WM_MDIMAXIMIZE,
    MdiTitle = WM_MDITILE,
    MdiCascade = WM_MDICASCADE,
    MdiIconArrange = WM_MDIICONARRANGE,
    MdiGetActive = WM_MDIGETACTIVE,
    MdiSetMenu = WM_MDISETMENU,
    EnterSizeMove = WM_ENTERSIZEMOVE,
    ExitSizeMove = WM_EXITSIZEMOVE,
    DropFiles = WM_DROPFILES,
    MdiRefreshMenu = WM_MDIREFRESHMENU,
    IME_SetContext = WM_IME_SETCONTEXT,
    IME_Notify = WM_IME_NOTIFY,
    IME_Control = WM_IME_CONTROL,
    IME_CompositionFull = WM_IME_COMPOSITIONFULL,
    IME_Select = WM_IME_SELECT,
    IME_Char = WM_IME_CHAR,
    IME_KeyDown = WM_IME_KEYDOWN,
    IME_KeyUp = WM_IME_KEYUP,
    MouseHover = WM_MOUSEHOVER,
    NcMouseLeave = WM_NCMOUSELEAVE,
    MouseLeave = WM_MOUSELEAVE,
    Cut = WM_CUT,
    Copy = WM_COPY,
    Paste = WM_PASTE,
    Clear = WM_CLEAR,
    Undo = WM_UNDO,
    RenderFormat = WM_RENDERFORMAT,
    RenderAllFormats = WM_RENDERALLFORMATS,
    DestroyClipBoard = WM_DESTROYCLIPBOARD,
    DrawClipBoard = WM_DRAWCLIPBOARD,
    PaintClipBoard = WM_PAINTCLIPBOARD,
    VScrollClipBoard = WM_VSCROLLCLIPBOARD,
    SizeClipBoard = WM_SIZECLIPBOARD,
    AskCbFormatName = WM_ASKCBFORMATNAME,
    ChangeCbChain = WM_CHANGECBCHAIN,
    HScrollClipBoard = WM_HSCROLLCLIPBOARD,
    QueryNewPlatte = WM_QUERYNEWPALETTE,
    PaletteIsChanging = WM_PALETTEISCHANGING,
    PaletteChanged = WM_PALETTECHANGED,
    HotKey = WM_HOTKEY,
    Print = WM_PRINT,
    PrintClient = WM_PRINTCLIENT,
    HandHeldFirst = WM_HANDHELDFIRST,
    HandHeldLast = WM_HANDHELDLAST,
    PenWinFirst = WM_PENWINFIRST,
    PenWInLast = WM_PENWINLAST ,
    CoalesceFirst = 0x390,
    CoalesceLast = 0x39F,
#ifndef WIN32_LEAN_AND_MEAN
    DDE_First = WM_DDE_FIRST,
    DDE_Initiate = WM_DDE_INITIATE,
    DDE_Terminate = WM_DDE_TERMINATE,
    DDE_Advise = WM_DDE_ADVISE,
    DDE_Unadvise = WM_DDE_UNADVISE,
    DDE_Ack = WM_DDE_ACK,
    DDE_Data = WM_DDE_DATA,
    DDE_Request = WM_DDE_REQUEST,
    DDE_Poke = WM_DDE_POKE,
    DDE_Execute = WM_DDE_EXECUTE,
    DDE_Last = WM_DDE_LAST,
#endif
    User = WM_USER,
    App = WM_APP
};

enum class SwpFlag : UINT {
    AsyncWindowPos = SWP_ASYNCWINDOWPOS,
    DeferErase = SWP_DEFERERASE,
    DrawFrame = SWP_DRAWFRAME,
    FrameChanged = SWP_FRAMECHANGED,
    HideWindow = SWP_HIDEWINDOW,
    NoActivate = SWP_NOACTIVATE,
    NoCopyBits = SWP_NOCOPYBITS,
    NoMove = SWP_NOMOVE,
    NoOwnerZOrder = SWP_NOOWNERZORDER,
    NoReDraw = SWP_NOREDRAW,
    NoReposition = SWP_NOREPOSITION,
    NoSendChanging = SWP_NOSENDCHANGING,
    NoSize = SWP_NOSIZE,
    NoZOrder = SWP_NOZORDER,
    ShowWindow = SWP_SHOWWINDOW
};

class SwpFlags {
    UINT m_uint;
public:
    SwpFlags(SwpFlag flag) : m_uint(static_cast<UINT>(flag)) {}
    SwpFlags(std::initializer_list<SwpFlag> flags){
        for(auto i = flags.begin(); i !=  flags.end(); ++i){
            m_uint |= static_cast<UINT>(*i);
        }
    }
    operator UINT() { return m_uint ;}
    operator |=(SwpFlag flag){
        m_uint |= static_cast<UINT>(flag);
    }
};

enum class RedrawFlag : UINT{
    Erase = RDW_ERASE,
    Frame = RDW_FRAME,
    InternalPaint = RDW_INTERNALPAINT,
    Invalidate = RDW_INVALIDATE,
    NoErase = RDW_NOERASE,
    NoFrame = RDW_NOFRAME,
    NoInternalPaint = RDW_NOINTERNALPAINT,
    Validate = RDW_VALIDATE,
    EraseNow = RDW_ERASENOW,
    UpdateNow = RDW_UPDATENOW,
    AllChildren = RDW_ALLCHILDREN,
    NoChildren = RDW_NOCHILDREN
};

class RedrawFlags {
    UINT m_flag;
public:
    RedrawFlags(RedrawFlag flag) : m_flag(static_cast<UINT>(flag)) {}
    RedrawFlags(std::initializer_list<RedrawFlag> flags) : m_flag(0){
        for(auto i = flags.begin(); i != flags.end();++i){
            m_flag |= static_cast<UINT>(*i);
        }
    }
    operator UINT() { return m_flag; }
};

class MessageBox {
public:
    static
    MessageBoxResult Show(LPCTSTR lpszText, LPCTSTR lpszCaption, MessageBoxButtons buttons){
         return static_cast<MessageBoxResult>(::MessageBox(nullptr, lpszText, lpszCaption,
                                                           static_cast<UINT>(buttons)));
    }

    static
    MessageBoxResult Show(HWND hwnd,LPCTSTR lpszText, LPCTSTR lpszCaption, MessageBoxButtons buttons){
         WINCHECK(hwnd);
         return static_cast<MessageBoxResult>(::MessageBox(hwnd, lpszText, lpszCaption, static_cast<UINT>(buttons)));
    }

    static
    MessageBoxResult Show(LPCTSTR lpszText, LPCTSTR lpszCaption,MessageBoxButtons buttons,
                                MessageBoxIcon icon){
         return static_cast<MessageBoxResult>(::MessageBox(nullptr, lpszText, lpszCaption,
                                                           static_cast<UINT>(buttons) |
                                                           static_cast<UINT>(icon)));
    }

    static
    MessageBoxResult Show(HWND hwnd,LPCTSTR lpszText, LPCTSTR lpszCaption,MessageBoxButtons buttons,
                                MessageBoxIcon icon){
         WINCHECK(hwnd);
         return static_cast<MessageBoxResult>(::MessageBox(hwnd, lpszText, lpszCaption,
                                                           static_cast<UINT>(buttons) |
                                                           static_cast<UINT>(icon)));
    }

    static
    MessageBoxResult Show(LPCTSTR lpszText, LPCTSTR lpszCaption,MessageBoxButtons buttons,
                  MessageBoxDefaultButton defa){
        return static_cast<MessageBoxResult>(::MessageBox(nullptr, lpszText, lpszCaption,
                                                          static_cast<UINT>(buttons) |
                                                          static_cast<UINT>(defa)));
    }

    static
    MessageBoxResult Show(HWND hwnd,LPCTSTR lpszText, LPCTSTR lpszCaption,MessageBoxButtons buttons,
                  MessageBoxDefaultButton defa){
        WINCHECK(hwnd);
        return static_cast<MessageBoxResult>(::MessageBox(hwnd, lpszText, lpszCaption,
                                                          static_cast<UINT>(buttons) |
                                                          static_cast<UINT>(defa)));
    }

    static
    MessageBoxResult Show(LPCTSTR lpszText, LPCTSTR lpszCaption,MessageBoxButtons buttons,
                                MessageBoxModalType mode) {
        return static_cast<MessageBoxResult>(::MessageBox(nullptr,lpszText,lpszCaption,
                                                          static_cast<UINT>(buttons) |
                                                          static_cast<UINT>(mode)));
    }

    static
    MessageBoxResult Show(HWND hwnd,LPCTSTR lpszText, LPCTSTR lpszCaption,MessageBoxButtons buttons,
                                MessageBoxModalType mode) {
        WINCHECK(hwnd);
        return static_cast<MessageBoxResult>(::MessageBox(hwnd,lpszText,lpszCaption,
                                                          static_cast<UINT>(buttons) |
                                                          static_cast<UINT>(mode)));
    }

    static
    MessageBoxResult Show(LPCTSTR lpszText, LPCTSTR lpszCaption,MessageBoxButtons buttons,
                                MessageBoxOtherOptions opt){
         return static_cast<MessageBoxResult>(::MessageBox(nullptr, lpszText, lpszCaption,
                                                           static_cast<UINT>(buttons) |
                                                           static_cast<UINT>(opt)));
    }

    static
    MessageBoxResult Show(HWND hwnd,LPCTSTR lpszText, LPCTSTR lpszCaption,MessageBoxButtons buttons,
                                MessageBoxOtherOptions opt){
         WINCHECK(hwnd);
         return static_cast<MessageBoxResult>(::MessageBox(hwnd, lpszText, lpszCaption,
                                                           static_cast<UINT>(buttons) |
                                                           static_cast<UINT>(opt)));
    }

    static
    MessageBoxResult Show(LPCTSTR lpszText, LPCTSTR lpszCaption,MessageBoxButtons buttons,
                                MessageBoxIcon icon, MessageBoxDefaultButton def) {
        return static_cast<MessageBoxResult>(::MessageBox(nullptr, lpszText, lpszCaption,
                                                          static_cast<UINT>(buttons) |
                                                          static_cast<UINT>(icon) |
                                                          static_cast<UINT>(def)));
    }

    static
    MessageBoxResult Show(HWND hwnd,LPCTSTR lpszText, LPCTSTR lpszCaption,MessageBoxButtons buttons,
                                MessageBoxIcon icon, MessageBoxDefaultButton def) {
        WINCHECK(hwnd);
        return static_cast<MessageBoxResult>(::MessageBox(hwnd, lpszText, lpszCaption,
                                                          static_cast<UINT>(buttons) |
                                                          static_cast<UINT>(icon) |
                                                          static_cast<UINT>(def)));
    }

    static
    MessageBoxResult Show(LPCTSTR lpszText, LPCTSTR lpszCaption,MessageBoxButtons buttons,
                                MessageBoxIcon icon, MessageBoxModalType type) {
        return static_cast<MessageBoxResult>(::MessageBox(nullptr, lpszText, lpszCaption,
                                                          static_cast<UINT>(buttons) |
                                                          static_cast<UINT>(icon) |
                                                          static_cast<UINT>(type)));
    }

    static
    MessageBoxResult Show(HWND hwnd,LPCTSTR lpszText, LPCTSTR lpszCaption,MessageBoxButtons buttons,
                                MessageBoxIcon icon, MessageBoxModalType type) {
        WINCHECK(hwnd);
        return static_cast<MessageBoxResult>(::MessageBox(hwnd, lpszText, lpszCaption,
                                                          static_cast<UINT>(buttons) |
                                                          static_cast<UINT>(icon) |
                                                          static_cast<UINT>(type)));
    }

    static
    MessageBoxResult Show(LPCTSTR lpszText, LPCTSTR lpszCaption,MessageBoxButtons buttons,
                                MessageBoxIcon icon, MessageBoxOtherOptions opt) {
        return static_cast<MessageBoxResult>(::MessageBox(nullptr, lpszText, lpszCaption,
                                                          static_cast<UINT>(buttons) |
                                                          static_cast<UINT>(icon) |
                                                          static_cast<UINT>(opt)));
    }

    static
    MessageBoxResult Show(HWND hwnd,LPCTSTR lpszText, LPCTSTR lpszCaption,MessageBoxButtons buttons,
                                MessageBoxIcon icon, MessageBoxOtherOptions opt) {
        WINCHECK(hwnd);
        return static_cast<MessageBoxResult>(::MessageBox(hwnd, lpszText, lpszCaption,
                                                          static_cast<UINT>(buttons) |
                                                          static_cast<UINT>(icon) |
                                                          static_cast<UINT>(opt)));
    }


    static
    MessageBoxResult Show(LPCTSTR lpszText, LPCTSTR lpszCaption,MessageBoxButtons buttons,
                                MessageBoxIcon icon, MessageBoxDefaultButton def,
                                MessageBoxModalType type) {
        return static_cast<MessageBoxResult>(::MessageBox(nullptr, lpszText, lpszCaption,
                                                          static_cast<UINT>(buttons) |
                                                          static_cast<UINT>(icon) |
                                                          static_cast<UINT>(def)) |
                                                          static_cast<UINT>(type));
    }

    static
    MessageBoxResult Show(HWND hwnd,LPCTSTR lpszText, LPCTSTR lpszCaption,MessageBoxButtons buttons,
                                MessageBoxIcon icon, MessageBoxDefaultButton def,
                                MessageBoxModalType type) {
        WINCHECK(hwnd);
        return static_cast<MessageBoxResult>(::MessageBox(hwnd, lpszText, lpszCaption,
                                                          static_cast<UINT>(buttons) |
                                                          static_cast<UINT>(icon) |
                                                          static_cast<UINT>(def)) |
                                                          static_cast<UINT>(type));
    }

    static
    MessageBoxResult Show(LPCTSTR lpszText, LPCTSTR lpszCaption,MessageBoxButtons buttons,
                                MessageBoxIcon icon, MessageBoxDefaultButton def,
                                MessageBoxOtherOptions opt) {
        return static_cast<MessageBoxResult>(::MessageBox(nullptr, lpszText, lpszCaption,
                                                          static_cast<UINT>(buttons) |
                                                          static_cast<UINT>(icon) |
                                                          static_cast<UINT>(def)) |
                                                          static_cast<UINT>(opt));
    }

    static
    MessageBoxResult Show(HWND hwnd,LPCTSTR lpszText, LPCTSTR lpszCaption,MessageBoxButtons buttons,
                                MessageBoxIcon icon, MessageBoxDefaultButton def,
                                MessageBoxOtherOptions opt) {
        WINCHECK(hwnd);
        return static_cast<MessageBoxResult>(::MessageBox(hwnd, lpszText, lpszCaption,
                                                          static_cast<UINT>(buttons) |
                                                          static_cast<UINT>(icon) |
                                                          static_cast<UINT>(def)) |
                                                          static_cast<UINT>(opt));
    }

    static
    MessageBoxResult Show(LPCTSTR lpszText, LPCTSTR lpszCaption,MessageBoxButtons buttons,
                                MessageBoxIcon icon, MessageBoxDefaultButton def,
                                MessageBoxModalType type,  MessageBoxOtherOptions opt) {
        return static_cast<MessageBoxResult>(::MessageBox(nullptr, lpszText, lpszCaption,
                                                          static_cast<UINT>(buttons) |
                                                          static_cast<UINT>(icon) |
                                                          static_cast<UINT>(def)) |
                                                          static_cast<UINT>(type) |
                                                          static_cast<UINT>(opt));
    }

    static
    MessageBoxResult Show(HWND hwnd,LPCTSTR lpszText, LPCTSTR lpszCaption,MessageBoxButtons buttons,
                                MessageBoxIcon icon, MessageBoxDefaultButton def,
                                MessageBoxModalType type,  MessageBoxOtherOptions opt) {
        WINCHECK(hwnd);
        return static_cast<MessageBoxResult>(::MessageBox(hwnd, lpszText, lpszCaption,
                                                          static_cast<UINT>(buttons) |
                                                          static_cast<UINT>(icon) |
                                                          static_cast<UINT>(def)) |
                                                          static_cast<UINT>(type) |
                                                          static_cast<UINT>(opt));
    }
};

class Window {
public:
  static RECT rcDefault;
  HWND m_hwnd;
  Window(HWND hWnd = nullptr) : m_hwnd(hWnd) {}
  Window(Window &other) = default;
  Window(Window &&other) = default;
  Window &operator=(HWND hWnd) {
    m_hwnd = hWnd;
    return *this;
  }

  Window &operator=(Window &) = default;
  Window &operator=(Window &&) = default;

  operator HWND() { return m_hwnd; }

  static LPCTSTR GetWndClassName() { return nullptr; }

  void Attach(HWND hWndNew = nullptr) { m_hwnd = hWndNew; }

  HWND Detach() {
    HWND hWnd = m_hwnd;
    m_hwnd = nullptr;
    return hWnd;
  }

  HWND Create(LPCTSTR lpstrWndClass = nullptr, HWND hWndParant = nullptr,
              Rect rect = nullptr, LPCTSTR szWindowName = nullptr,
              DWORD dwStyle = 0, DWORD dwExStyle = 0, HMENU menu = nullptr,
              LPVOID lpCreateParam = nullptr) {
    m_hwnd = ::CreateWindowEx(dwExStyle, lpstrWndClass, szWindowName, dwStyle,
                              rect.left,rect.top, rect.Width(),
                              rect.Height(), hWndParant, menu,
                              HINST_THISCOMPONENT, lpCreateParam);
    return m_hwnd;
  }

  BOOL DestroyWindow() {
    WINCHECK(m_hwnd);
    if (!::DestroyWindow(m_hwnd))
      return FALSE;

    m_hwnd = nullptr;
    return TRUE;
  }

  operator HWND() const { return m_hwnd; }

  DWORD GetStyle() const {
    WINCHECK(m_hwnd);
    return (DWORD)::GetWindowLong(m_hwnd, GWL_STYLE);
  }

  DWORD GetExStyle() const {
    WINCHECK(m_hwnd);
    return (DWORD)::GetWindowLong(m_hwnd, GWL_EXSTYLE);
  }

  LONG GetWindowLong(int nIndex) const {
    WINCHECK(m_hwnd);
    return ::GetWindowLong(m_hwnd, nIndex);
  }

  LONG GetWindowLong(GWLFlag flag) const {
    WINCHECK(m_hwnd);
    return ::GetWindowLong(m_hwnd, static_cast<int>(flag));
  }

  LONG_PTR GetWindowLongPtr(int nIndex) const {
    WINCHECK(m_hwnd);
    return ::GetWindowLongPtr(m_hwnd, nIndex);
  }

  LONG_PTR GetWindowLongPtr(GWLFlag flag) const {
    WINCHECK(m_hwnd);
    return ::GetWindowLongPtr(m_hwnd, static_cast<int>(flag));
  }

  LONG SetWindowLong(int nIndex, LONG dwNewLong) {
    WINCHECK(m_hwnd);
    return ::SetWindowLong(m_hwnd, nIndex, dwNewLong);
  }

  LONG SetWindowLong(GWLFlag flag, LONG dwNewLong) {
    WINCHECK(m_hwnd);
    return ::SetWindowLong(m_hwnd, static_cast<int>(flag), dwNewLong);
  }

  HRESULT SetWindowLong(int nIndex, LONG dwNewLong, LONG &oldLong) {
    WINCHECK(m_hwnd);
    oldLong = ::SetWindowLong(m_hwnd, nIndex, dwNewLong);
    if (oldLong == 0) {
      return __HRESULT_FROM_WIN32(GetLastError());
    } else {
      return S_OK;
    }
  }

  HRESULT SetWindowLong(GWLFlag flag, LONG dwNewLong, LONG &oldLong) {
    WINCHECK(m_hwnd);
    oldLong = ::SetWindowLong(m_hwnd,static_cast<int>(flag), dwNewLong);
    if (oldLong == 0) {
      return __HRESULT_FROM_WIN32(GetLastError());
    } else {
      return S_OK;
    }
  }

  LONG_PTR SetWindowLongPtr(int nIndex, LONG_PTR dwNewLong) {
    WINCHECK(m_hwnd);
    return ::SetWindowLongPtr(m_hwnd, nIndex, dwNewLong);
  }

  LONG_PTR SetWindowLongPtr(GWLFlag flag, LONG_PTR dwNewLong) {
    WINCHECK(m_hwnd);
    return ::SetWindowLongPtr(m_hwnd, static_cast<int>(flag), dwNewLong);
  }

  LRESULT SendMessage(WindowsMessage msg, WPARAM wParam = 0, LPARAM lParam = 0) const {
    WINCHECK(m_hwnd);
    return ::SendMessage(m_hwnd, static_cast<UINT>(msg), wParam, lParam);
  }

  BOOL PostMessage(UINT msg, WPARAM wParam = 0, LPARAM lParam = 0) {
    WINCHECK(m_hwnd);
    return ::PostMessage(m_hwnd, msg, wParam, lParam);
  }

  BOOL PostMessage(WindowsMessage msg, WPARAM wParam = 0, LPARAM lParam = 0) {
    WINCHECK(m_hwnd);
    return ::PostMessage(m_hwnd, static_cast<UINT>(msg), wParam, lParam);
  }

  BOOL SendNotifyMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
    WINCHECK(m_hwnd);
    return ::SendNotifyMessage(m_hwnd, msg, wParam, lParam);
  }

  BOOL SendNotifyMessage(WindowsMessage msg, WPARAM wParam, LPARAM lParam) {
    WINCHECK(m_hwnd);
    return ::SendNotifyMessage(m_hwnd, static_cast<UINT>(msg), wParam, lParam);
  }

  BOOL SetWindowText(LPCTSTR lpszString) {
    WINCHECK(m_hwnd);
    return ::SetWindowText(m_hwnd, lpszString);
  }

  int GetWindowTextLength() const {
    WINCHECK(m_hwnd);
    return ::GetWindowTextLength(m_hwnd);
  }

  int GetWindowText(LPTSTR lpszString, int BuffLen) {
    WINCHECK(m_hwnd);
    return ::GetWindowText(m_hwnd, lpszString, BuffLen);
  }

  HRESULT GetWindowText(std::tstring &str) {
    HRESULT hr = S_OK;
    int txtlen = GetWindowTextLength();
    try {
      if (txtlen + 1 <= str.max_size()) {
        str.resize(txtlen + 1);
      } else {
        str.resize(str.max_size());
      }
      int got = ::GetWindowText(m_hwnd, &str[0], str.length());
      if (got == 0) {
        int errval = GetLastError();
        if (errval == 0)
          return S_OK;
        else
          return __HRESULT_FROM_WIN32(errval);
      }
      return S_OK;
    } catch (std::bad_alloc e) {
      return __HRESULT_FROM_WIN32(ERROR_OUTOFMEMORY);
    }
    return S_OK;
  }

  void SetFont(HFONT hFont, BOOL bRedraw = TRUE) {
    WINCHECK(m_hwnd);
    SendMessage(WindowsMessage::SetFont, reinterpret_cast<WPARAM>(hFont),
                  MAKELPARAM(bRedraw, 0));
  }

  HFONT GetFont() const {
    WINCHECK(m_hwnd);
    return reinterpret_cast<HFONT>(this->SendMessage(WindowsMessage::GetFont, 0, 0));
  }

  HMENU GetMenu() const {
    WINCHECK(m_hwnd);
    return reinterpret_cast<HMENU>(::GetMenu(m_hwnd));
  }

  BOOL SetMenu(HMENU hMenu) {
    WINCHECK(m_hwnd);
    return ::SetMenu(m_hwnd, hMenu);
  }

  BOOL DrawMenuBar() {
    WINCHECK(m_hwnd);
    return ::DrawMenuBar(m_hwnd);
  }

  HMENU GetSystemMenu(BOOL bRevert) const {
    WINCHECK(m_hwnd);
    return reinterpret_cast<HMENU>(::GetSystemMenu(m_hwnd, bRevert));
  }

  BOOL HiliteMenuItem(HMENU hMenu, UINT uItemHilite, UINT uHilite) {
    WINCHECK(m_hwnd);
    return ::HiliteMenuItem(m_hwnd, hMenu, uItemHilite, uHilite);
  }

  BOOL IsIconic() const {
    WINCHECK(m_hwnd);
    return ::IsIconic(m_hwnd);
  }

  BOOL IsZoomed() const {
    WINCHECK(m_hwnd);
    return ::IsZoomed(m_hwnd);
  }

  BOOL MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE) {
    WINCHECK(m_hwnd);
    return ::MoveWindow(m_hwnd, x, y, nWidth, nHeight, bRepaint);
  }

  BOOL SetWindowPos(HWND hWndInsertAfter, int x, int y, int cx, int cy,
                    UINT nFlags) {
    WINCHECK(m_hwnd);
    return ::SetWindowPos(m_hwnd, hWndInsertAfter, x, y, cx, cy, nFlags);
  }

  BOOL SetWindowPos(HWND hWndInsertAfter, int x, int y, int cx, int cy,
                    SwpFlags nFlags) {
    WINCHECK(m_hwnd);
    return ::SetWindowPos(m_hwnd, hWndInsertAfter, x, y, cx, cy, nFlags);
  }

  BOOL SetWindowPos(HWND hWndInsertAfter, LPCRECT lpRect, UINT nFlags) {
    WINCHECK(m_hwnd);
    return ::SetWindowPos(m_hwnd, hWndInsertAfter, lpRect->left, lpRect->top,
                          lpRect->right - lpRect->left,
                          lpRect->bottom - lpRect->top, nFlags);
  }

  BOOL SetWindowPos(HWND hWndInsertAfter, LPCRECT lpRect, SwpFlags nFlags) {
    WINCHECK(m_hwnd);
    return ::SetWindowPos(m_hwnd, hWndInsertAfter, lpRect->left, lpRect->top,
                          lpRect->right - lpRect->left,
                          lpRect->bottom - lpRect->top, nFlags);
  }

  UINT ArrangeIconicWindows() {
    WINCHECK(m_hwnd);
    return ::ArrangeIconicWindows(m_hwnd);
  }

  BOOL BringWindowToTop() {
    WINCHECK(m_hwnd);
    return ::BringWindowToTop(m_hwnd);
  }

  BOOL GetWindowRect(LPRECT lpRect) const {
    WINCHECK(m_hwnd);
    return ::GetWindowRect(m_hwnd, lpRect);
  }

  BOOL GetClientRect(LPRECT lpRect) const {
    WINCHECK(m_hwnd);
    return ::GetClientRect(m_hwnd, lpRect);
  }

  BOOL GetWindowPlacement(WINDOWPLACEMENT *lpwndpl) {
    WINCHECK(m_hwnd);
    return ::GetWindowPlacement(m_hwnd, lpwndpl);
  }

  BOOL SetWindowPlacement(WINDOWPLACEMENT *lpwndpl) {
    WINCHECK(m_hwnd);
    return ::SetWindowPlacement(m_hwnd, lpwndpl);
  }

  BOOL ClientToScreen(LPPOINT lpPoint) const {
    WINCHECK(m_hwnd);
    return ::ClientToScreen(m_hwnd, lpPoint);
  }

  BOOL ClientToScreen(LPRECT lpRect) const {
    WINCHECK(m_hwnd);
    if (!::ClientToScreen(m_hwnd, (LPPOINT)lpRect))
      return FALSE;
    return ::ClientToScreen(m_hwnd, ((LPPOINT)lpRect) + 1);
  }

  BOOL ScreenToClient(LPPOINT lpPoint) const {
    WINCHECK(m_hwnd);
    return ::ScreenToClient(m_hwnd, lpPoint);
  }

  BOOL ScreenToClient(LPRECT lpRect) const {
    WINCHECK(m_hwnd);
    if (!::ScreenToClient(m_hwnd, (LPPOINT)lpRect))
      return FALSE;

    return ::ScreenToClient(m_hwnd, ((LPPOINT)lpRect) + 1);
  }

  int MapWindowPoints(HWND hWndTo, LPPOINT lpPoint, UINT nCount) const {
    WINCHECK(m_hwnd);
    return ::MapWindowPoints(m_hwnd, hWndTo, lpPoint, nCount);
  }

  int MapWindowPoints(HWND hWndTo, LPRECT lpRect) {
    WINCHECK(m_hwnd);
    return ::MapWindowPoints(m_hwnd, hWndTo, (LPPOINT)lpRect, 2);
  }

  HDC BeginPaint(LPPAINTSTRUCT lpPaint) {
    WINCHECK(m_hwnd);
    return ::BeginPaint(m_hwnd, lpPaint);
  }

  void EndPaint(LPPAINTSTRUCT lpPaint) {
    WINCHECK(m_hwnd);
    ::EndPaint(m_hwnd, lpPaint);
  }

  HDC GetDC() {
    WINCHECK(m_hwnd);
    return ::GetDC(m_hwnd);
  }

  HDC GetWindowDC() {
    WINCHECK(m_hwnd);
    return ::GetWindowDC(m_hwnd);
  }

  int ReleaseDC(HDC hDC) {
    WINCHECK(m_hwnd);
    return ::ReleaseDC(m_hwnd, hDC);
  }

  void Print(HDC hDC, DWORD dwFlags) const {
    this->SendMessage(WindowsMessage::Print, (WPARAM)hDC, dwFlags);
  }

  void Print(HDC hDC, PrintFlags dwFlags) const {
    this->SendMessage(WindowsMessage::Print, (WPARAM)hDC, dwFlags);
  }

  void PrintClient(HDC hDC, DWORD dwFlags) {  
    this->SendMessage(WindowsMessage::PrintClient, (WPARAM)hDC, dwFlags);
  }

  void PrintClient(HDC hDC, PrintFlags flags){
      this->SendMessage(WindowsMessage::PrintClient, (WPARAM)hDC, flags);
  }

  BOOL UpdateWindow() {
    WINCHECK(m_hwnd);
    return ::UpdateWindow(m_hwnd);
  }

  void SetRedraw(BOOL bRedraw = TRUE) {

    SendMessage(WindowsMessage::SetReDraw, (WPARAM)bRedraw, 0);
  }

  BOOL GetUpdateRect(LPRECT lpRect, BOOL bErase = FALSE) {
    WINCHECK(m_hwnd);
    return ::GetUpdateRect(m_hwnd, lpRect, bErase);
  }

  int GetUpdateRgn(HRGN hRgn, BOOL bErase = FALSE) {
    WINCHECK(m_hwnd);
    return ::GetUpdateRgn(m_hwnd, hRgn, bErase);
  }

  BOOL Invalidate(BOOL bErase = TRUE) {
    WINCHECK(m_hwnd);
    return ::InvalidateRect(m_hwnd, nullptr, bErase);
  }

  BOOL InvalidateRect(LPCRECT lpRect, BOOL bErase = TRUE) {
    WINCHECK(m_hwnd);
    return ::InvalidateRect(m_hwnd, lpRect, bErase);
  }

  BOOL ValidateRect(LPCRECT lpRect = nullptr) {
    WINCHECK(m_hwnd);
    return ::ValidateRect(m_hwnd, lpRect);
  }

  void InvalidateRgn(HRGN hRgn, BOOL bErase = TRUE) {
    WINCHECK(m_hwnd);
    ::InvalidateRgn(m_hwnd, hRgn, bErase);
  }

  BOOL ValidateRgn(HRGN hRgn = nullptr) {
    WINCHECK(m_hwnd);
    return ::ValidateRgn(m_hwnd, hRgn);
  }

  BOOL ShowWindow(int nCmdShow) {
    WINCHECK(m_hwnd);
    return ::ShowWindow(m_hwnd, nCmdShow);
  }

  BOOL ShowWindow(ShowWindowType type){
    WINCHECK(m_hwnd);
    return ::ShowWindow(m_hwnd,static_cast<int>(type));
  }

  BOOL IsWindowVisible() const {
    WINCHECK(m_hwnd);
    return ::IsWindowVisible(m_hwnd);
  }

  BOOL ShowOwnedPopups(BOOL bShow = TRUE) {
    WINCHECK(m_hwnd);
    return ::ShowOwnedPopups(m_hwnd, bShow);
  }

  HDC GetDCEx(HRGN hRgnClip, DWORD flags) {
    WINCHECK(m_hwnd);
    return ::GetDCEx(m_hwnd, hRgnClip, flags);
  }

  HDC GetDCEx(HRGN hRgnClip, DCCreationFlags flags) {
    WINCHECK(m_hwnd);
    return ::GetDCEx(m_hwnd, hRgnClip, static_cast<DWORD>(flags));
  }

  BOOL LockWindowUpdate(BOOL bLock = TRUE) {
    WINCHECK(m_hwnd);
    return ::LockWindowUpdate(bLock ? m_hwnd : nullptr);
  }

  BOOL RedrawWindow(LPCRECT lpRectUpdate = nullptr, HRGN hRgnUpdate = nullptr,
                    RedrawFlags flags = {RedrawFlag::Invalidate,
                                         RedrawFlag::UpdateNow,
                                         RedrawFlag::Erase}) {
    WINCHECK(m_hwnd);
    return ::RedrawWindow(m_hwnd, lpRectUpdate, hRgnUpdate, flags);
  }

  UINT_PTR SetTimer(UINT_PTR nIDEvent, UINT nElapse,
                    void(CALLBACK *lpfnTimer)(HWND, UINT, UINT_PTR,
                                              DWORD) = nullptr) {
    WINCHECK(m_hwnd);
    return ::SetTimer(m_hwnd, nIDEvent, nElapse, (TIMERPROC)lpfnTimer);
  }

  BOOL KillTimer(UINT_PTR nIDEvent) {
    WINCHECK(m_hwnd);
    return ::KillTimer(m_hwnd, nIDEvent);
  }

  BOOL IsWindowEnabled() const {
    WINCHECK(m_hwnd);
    return ::IsWindowEnabled(m_hwnd);
  }

  BOOL EnableWindow(BOOL Enable = TRUE) {
    WINCHECK(m_hwnd);
    return ::EnableWindow(m_hwnd, Enable);
  }

  HWND SetActiveWindow() {
    WINCHECK(m_hwnd);
    return ::SetActiveWindow(m_hwnd);
  }

  HWND SetCapture() {
    WINCHECK(m_hwnd);
    return ::SetCapture(m_hwnd);
  }

  HWND SetFocus() {
    WINCHECK(m_hwnd);
    return ::SetFocus(m_hwnd);
  }

  BOOL CheckDlgButton(int nIDButton, UINT nCheck) {
    WINCHECK(m_hwnd);
    return ::CheckDlgButton(m_hwnd, nIDButton, nCheck);
  }

  BOOL CheckRadioButton(int nIDFirstButton, int nIDLastButton,
                        int nIDCheckButton) {
    WINCHECK(m_hwnd);
    return ::CheckRadioButton(m_hwnd, nIDFirstButton, nIDLastButton,
                              nIDCheckButton);
  }

  int DlgDirList(LPTSTR lpPathSpec, int nIDListBox, int nIDStaticPath,
                 UINT nFileType) {
    WINCHECK(m_hwnd);
    return ::DlgDirList(m_hwnd, lpPathSpec, nIDListBox, nIDStaticPath,
                        nFileType);
  }

  int DlgDirListComboBox(LPTSTR lpPathSpec, int nIDComboBox, int nIDStaticPath,
                         UINT nFileType) {
    WINCHECK(m_hwnd);
    return ::DlgDirListComboBox(m_hwnd, lpPathSpec, nIDComboBox, nIDStaticPath,
                                nFileType);
  }

  UINT GetDlgItemInt(int nID, BOOL *lpTrans = nullptr,
                     BOOL bSigned = TRUE) const {
    WINCHECK(m_hwnd);
    return ::GetDlgItemInt(m_hwnd, nID, lpTrans, bSigned);
  }

  UINT GetDlgItemText(int nID, LPTSTR lpStr, int MaxCount) const throw() {
    WINCHECK(m_hwnd);
    return ::GetDlgItemText(m_hwnd, nID, lpStr, MaxCount) + 1;
  }

  UINT GetDlgItemText(int nID, std::tstring &str) {
    HWND hItem = GetDlgItem(nID);
    if (hItem != nullptr) {
      str.resize(::GetWindowTextLength(hItem) + 1);
      ::GetWindowText(hItem, &str[0], str.length());
      return str.length();
    } else {
      str = TEXT("");
      return 0;
    }
  }

  Window GetNextDlgGroupItem(HWND hWndCtl, BOOL bPrevious = FALSE) const {
    WINCHECK(m_hwnd);
    return Window(::GetNextDlgGroupItem(m_hwnd, hWndCtl, bPrevious));
  }

  Window GetNextDlgTabItem(HWND hWndCtrl, BOOL bPrevious = FALSE) const {
    WINCHECK(m_hwnd);
    return Window(::GetNextDlgTabItem(m_hwnd, hWndCtrl, bPrevious));
  }

  UINT IsDlgButtonChecked(int nIDButton) const {
    WINCHECK(m_hwnd);
    return ::IsDlgButtonChecked(m_hwnd, nIDButton);
  }

  LRESULT SendDlgItemMessage(int nID, UINT msg, WPARAM wParam = 0,
                             LPARAM lParam = 0) {
    WINCHECK(m_hwnd);
    return ::SendDlgItemMessage(m_hwnd, nID, msg, wParam, lParam);
  }

  int GetScrollPos(int nBar) const {
    WINCHECK(m_hwnd);
    return ::GetScrollPos(m_hwnd, nBar);
  }

  BOOL GetScrollRange(int nBar, LPINT lpMinPos, LPINT lpMaxPos) {
    WINCHECK(m_hwnd);
    return ::GetScrollRange(m_hwnd, nBar, lpMinPos, lpMaxPos);
  }

  BOOL ScrollWindow(int xAmount, int yAmount, LPCRECT lpRect = nullptr,
                    LPCRECT lpClipRect = nullptr) {
    WINCHECK(m_hwnd);
    return ::ScrollWindow(m_hwnd, xAmount, yAmount, lpRect, lpClipRect);
  }

  int ScrollWindowEx(int dx, int dy, LPCRECT lpRectScroll = nullptr,
                     LPCRECT lpRectClip = nullptr, HRGN hRgnUpdate = nullptr,
                     LPRECT lpRectUpdate = nullptr, UINT uFlags = 0) {
    WINCHECK(m_hwnd);
    return ::ScrollWindowEx(m_hwnd, dx, dy, lpRectScroll, lpRectClip,
                            hRgnUpdate, lpRectUpdate, uFlags);
  }

  int ScrollWindowEx(int dx, int dy, UINT uFlags,
                     LPCRECT lpRectScroll = nullptr,
                     LPCRECT lpRectClip = nullptr, HRGN hRgnUpdate = nullptr,
                     LPRECT lpRectUpdate = nullptr) {
    WINCHECK(m_hwnd);
    return ::ScrollWindowEx(m_hwnd, dx, dy, lpRectScroll, lpRectClip,
                            hRgnUpdate, lpRectUpdate, uFlags);
  }

  int SetScrollPos(int nBar, int nPos, BOOL bRedraw = TRUE) {
    WINCHECK(m_hwnd);
    return ::SetScrollPos(m_hwnd, nBar, nPos, bRedraw);
  }

  BOOL SetScrollRange(int nBar, int nMinPos, int nMaxPos, BOOL bRedraw = TRUE) {
    WINCHECK(m_hwnd);
    return ::SetScrollRange(m_hwnd, nBar, nMinPos, nMaxPos, bRedraw);
  }

  BOOL ShowScrollBar(UINT nBar, BOOL bShow = TRUE) {
    WINCHECK(m_hwnd);
    return ::ShowScrollBar(m_hwnd, nBar, bShow);
  }

  BOOL EnableScrollBar(UINT uSBFlags, UINT uArrowFlags = ESB_ENABLE_BOTH) {
    WINCHECK(m_hwnd);
    return ::EnableScrollBar(m_hwnd, uSBFlags, uArrowFlags);
  }

  Window ChildWindowFromPoint(POINT point) {
    WINCHECK(m_hwnd);
    return Window(::ChildWindowFromPoint(m_hwnd, point));
  }

  Window ChildWindowFromPointEx(POINT point, UINT uFlags) const {
    WINCHECK(m_hwnd);
    return Window(::ChildWindowFromPointEx(m_hwnd, point, uFlags));
  }

  Window GetTopWindow() const {
    WINCHECK(m_hwnd);
    return Window(::GetTopWindow(m_hwnd));
  }

  Window GetWindow(UINT nCmd) const {
    WINCHECK(m_hwnd);
    return Window(::GetWindow(m_hwnd, nCmd));
  }

  Window GetLastActivePopup() const {
    WINCHECK(m_hwnd);
    return Window(::GetLastActivePopup(m_hwnd));
  }

  BOOL IsChild(HWND hWnd) const {
    WINCHECK(m_hwnd);
    return ::IsChild(m_hwnd, hWnd);
  }

  Window GetParent() const {
    WINCHECK(m_hwnd);
    return Window(::GetParent(m_hwnd));
  }

  Window SetParent(HWND hWndNewParent) {
    WINCHECK(m_hwnd);
    return Window(::SetParent(m_hwnd, hWndNewParent));
  }

  int GetDlgCtrlID() const {
    WINCHECK(m_hwnd);
    return ::GetDlgCtrlID(m_hwnd);
  }

  int SetDlgCtrlID(int nID) {
    WINCHECK(m_hwnd);
    return (int)::SetWindowLong(m_hwnd, GWL_ID, nID);
  }

  Window GetDlgItem(int nID) const {
    WINCHECK(m_hwnd);
    return Window(::GetDlgItem(m_hwnd, nID));
  }

  BOOL FlashWindow(BOOL bInvert) {
    WINCHECK(m_hwnd);
    return ::FlashWindow(m_hwnd, bInvert);
  }

  int MessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = TEXT(""),
                 UINT nType = MB_OK) {
    WINCHECK(m_hwnd);
    return ::MessageBox(m_hwnd, lpszText, lpszCaption, nType);
  }

  BOOL ChangeClipboardChain(HWND hwndNext) {
    WINCHECK(m_hwnd);
    return ::ChangeClipboardChain(m_hwnd, hwndNext);
  }

  HWND SetClipboardViewer() {
    WINCHECK(m_hwnd);
    return ::SetClipboardViewer(m_hwnd);
  }

  BOOL OpenClipboard() {
    WINCHECK(m_hwnd);
    return ::OpenClipboard(m_hwnd);
  }

  BOOL CreateCaret(HBITMAP hBitmap) {
    WINCHECK(m_hwnd);
    return ::CreateCaret(m_hwnd, hBitmap, 0, 0);
  }

  BOOL CreateSolidCaret(int nWidth, int nHeight) {
    WINCHECK(m_hwnd);
    return ::CreateCaret(m_hwnd, (HBITMAP)0, nWidth, nHeight);
  }

  BOOL CreateGrayCaret(int nWidth, int nHeight) {
    WINCHECK(m_hwnd);
    return ::CreateCaret(m_hwnd, (HBITMAP)1, nWidth, nHeight);
  }

  BOOL HideCaret() {
    WINCHECK(m_hwnd);
    return ::HideCaret(m_hwnd);
  }

  BOOL ShowCaret() {
    WINCHECK(m_hwnd);
    return ::ShowCaret(m_hwnd);
  }

  HICON SetIcon(HICON hIcon, BOOL bBigIcon = TRUE) {
    return reinterpret_cast<HICON>(
        SendMessage(WindowsMessage::SetIcon, bBigIcon, (LPARAM)hIcon));
  }

  HICON GetIcon(BOOL bBigIcon = TRUE) const {
    return reinterpret_cast<HICON>(
        SendMessage(WindowsMessage::GetIcon, bBigIcon, 0));
  }

  BOOL WinHelp(LPCTSTR lpzHelp, UINT cmd = HELP_CONTEXT, DWORD dwData = 0) {
    WINCHECK(m_hwnd);
    return ::WinHelp(m_hwnd, lpzHelp, cmd, dwData);
  }

  BOOL SetWindowContextHelpId(DWORD dwContextId) {
    WINCHECK(m_hwnd);
    return ::SetWindowContextHelpId(m_hwnd, dwContextId);
  }

  DWORD GetWindowContextHelpId() {
    WINCHECK(m_hwnd);
    return ::GetWindowContextHelpId(m_hwnd);
  }

  int SetHotKey(WORD wVirtualKeyCode, WORD wModifiers) {
    WINCHECK(m_hwnd);
    return (int)SendMessage(WindowsMessage::SetHotKey,
                              MAKEWORD(wVirtualKeyCode, wModifiers), 0);
  }

  DWORD GetHotKey() const {
    return (DWORD)SendMessage(WindowsMessage::GetHotKey, 0, 0);
  }

  BOOL GetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo) {
    WINCHECK(m_hwnd);
    return ::GetScrollInfo(m_hwnd, nBar, lpScrollInfo);
  }

  int SetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, BOOL bRedraw = TRUE) {
    WINCHECK(m_hwnd);
    return ::SetScrollInfo(m_hwnd, nBar, lpScrollInfo, bRedraw);
  }

  BOOL IsDialogMessage(LPMSG lpMsg) {
    WINCHECK(m_hwnd);
    return ::IsDialogMessage(m_hwnd, lpMsg);
  }

  void NextDlgCtrl() const {
    SendMessage(WindowsMessage::NextDialogControl, 0, 0L);
  }

  void PrevDlgCtrl() const {

    SendMessage(WindowsMessage::NextDialogControl, 1, 0L);
  }

  void GotoDlgCtrl(HWND hWndCtrl) const {
    SendMessage(WindowsMessage::NextDialogControl, (WPARAM)hWndCtrl, 1L);
  }

  BOOL ResizeClient(int Width, int Height, BOOL redraw = TRUE) {
    WINCHECK(m_hwnd);
    RECT rcWnd;
    if (!GetClientRect(&rcWnd))
      return FALSE;

    if (Width != -1)
      ;
    rcWnd.right = Width;
    if (Height != -1)
      ;
    rcWnd.bottom = Height;

    if (!::AdjustWindowRectEx(
            &rcWnd, GetStyle(),
            (!(GetStyle() & WS_CHILD) && (GetMenu() != nullptr)), GetExStyle()))
      return FALSE;

    SwpFlags flags = { SwpFlag::NoZOrder,SwpFlag::NoMove, SwpFlag::NoActivate };

    if (!redraw)
        flags |= SwpFlag::NoReDraw;

    return SetWindowPos(nullptr, 0, 0, rcWnd.right - rcWnd.left,
                        rcWnd.bottom - rcWnd.top, flags);
  }

  BOOL SetWidth(int Width, BOOL redraw = TRUE) {
      Rect rc;
      GetClientRect(rc);
      return ResizeClient(Width,rc.Height(), redraw);
  }

  BOOL SetHeight(int Height, BOOL redraw = TRUE){
      Rect rc;
      GetClientRect(rc);
      return ResizeClient(rc.Width(),Height, redraw);
  }

  int GetWindowRgn(HRGN hRgn) {
    WINCHECK(m_hwnd);
    return ::GetWindowRgn(m_hwnd, hRgn);
  }

  int SetWindowRgn(HRGN hRgn = nullptr, BOOL redraw = FALSE) {
    WINCHECK(m_hwnd);
    return ::SetWindowRgn(m_hwnd, hRgn, redraw);
  }

  HDWP DeferWindowPos(HDWP hWinPosInfo, HWND hWndInsertAfter, int x, int y,
                      int cx, int cy, UINT uFlags) {
    WINCHECK(m_hwnd);
    return ::DeferWindowPos(hWinPosInfo, m_hwnd, hWndInsertAfter, x, y, cx, cy,
                            uFlags);
  }

  DWORD GetWindowThreadID() {
    WINCHECK(m_hwnd);
    return ::GetWindowThreadProcessId(m_hwnd, nullptr);
  }

  DWORD GetWindowProcessID() {
    WINCHECK(m_hwnd);
    DWORD dwProcessID;
    ::GetWindowThreadProcessId(m_hwnd, &dwProcessID);
    return dwProcessID;
  }

  BOOL IsWindow() const { return ::IsWindow(m_hwnd); }

  BOOL IsWindowUnicode() const {
    WINCHECK(m_hwnd);
    return ::IsWindowUnicode(m_hwnd);
  }

  BOOL IsParentDialog() {
    WINCHECK(m_hwnd);
    TCHAR szBuff[8];
    if (GetClassName(GetParent(), szBuff, sizeof(szBuff) / sizeof(szBuff[0])) ==
        0)
      return FALSE;

    return lstrcmp(szBuff, TEXT("#32770")) == 0;
  }

  BOOL ShowWindowAsync(int nCmdShow) {
    WINCHECK(m_hwnd);
    return ::ShowWindowAsync(m_hwnd, nCmdShow);
  }

  BOOL ShowWindowAsync(ShowWindowType type) {
    WINCHECK(m_hwnd);
    return ::ShowWindowAsync(m_hwnd, static_cast<int>(type));
  }

  int GetMappedRect(LPRECT rc){
      GetWindowRect(rc);
      return ::MapWindowPoints(HWND_DESKTOP,GetParent(),(LPPOINT)&rc,2);
  }

  Window GetDecendantWindow(int nID) const {
    WINCHECK(m_hwnd);
    // GetDlgItem recursive (return first found)
    // breadth-first for 1 level, then depth-first for successive levels

    // if it's a dialog, use the fast USER functions.
    HWND hWndChild, hWndTmp;
    if ((hWndChild = ::GetDlgItem(m_hwnd, nID)) != nullptr) {
      if (::GetTopWindow(hWndChild) != nullptr) {

        Window wnd(hWndChild);
        hWndTmp = wnd.GetDecendantWindow(nID);
        if (hWndTmp != nullptr)
          return Window(hWndTmp);
      }
      return Window(hWndChild);
    }

    // linear search for the ID.
    for (hWndChild = ::GetTopWindow(m_hwnd); hWndChild != nullptr;
         hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT)) {
      Window wnd(hWndChild);
      hWndTmp = wnd.GetDecendantWindow(nID);
      if (hWndTmp != nullptr)
        return Window(hWndTmp);
    }

    return Window(nullptr);
  }

  void SendMessageToDescendants(WindowsMessage msg, WPARAM wParam = 0, LPARAM  lParam = 0 , BOOL bDeep = TRUE){
      WINCHECK(m_hwnd);
      for(HWND hWndChild = ::GetTopWindow(m_hwnd); hWndChild != nullptr;
          hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT)){
          ::SendMessage(hWndChild,static_cast<UINT>(msg),wParam,lParam);

          if(bDeep && ::GetTopWindow(hWndChild) != nullptr){
              Window wnd(hWndChild);
              wnd.SendMessageToDescendants(msg,wParam,lParam,bDeep);
          }
      }
  }

  BOOL CenterWindow(HWND hWndCenter = nullptr) {
    WINCHECK(m_hwnd);
    DWORD dwStyle = GetStyle();
    if (hWndCenter == nullptr) {
      if (dwStyle & WS_CHILD)
        hWndCenter = ::GetParent(m_hwnd);
      else
        hWndCenter = ::GetWindow(m_hwnd, GW_OWNER);
    }

    RECT rcDlg;
    RECT rcArea;
    RECT rcCenter;
    HWND hWndParent;
    if (!(dwStyle & WS_CHILD)) {
      if (hWndCenter != nullptr) {
        DWORD dwStyleCenter = ::GetWindowLong(hWndCenter, GWL_STYLE);
        if (!(dwStyleCenter & WS_VISIBLE) || (dwStyleCenter & WS_MINIMIZE))
          hWndCenter = nullptr;
      }

      HMONITOR hMonitor = nullptr;
      if (hWndCenter != nullptr) {
        hMonitor = ::MonitorFromWindow(hWndCenter, MONITOR_DEFAULTTONEAREST);
      } else {
        hMonitor = ::MonitorFromWindow(m_hwnd, MONITOR_DEFAULTTONEAREST);
      }

      if (!hMonitor)
        return FALSE;

      MONITORINFO minfo;
      minfo.cbSize = sizeof(MONITORINFO);
      BOOL bResult = ::GetMonitorInfo(hMonitor, &minfo);
      if (!bResult)
        return bResult;

      rcArea = minfo.rcWork;

      if (hWndCenter == nullptr)
        rcCenter = rcArea;
      else
        ::GetWindowRect(hWndCenter, &rcCenter);
    } else {
      hWndParent = ::GetParent(m_hwnd);
      if (hWndParent == nullptr)
        return FALSE;

      ::GetClientRect(hWndParent, &rcArea);
      ::GetClientRect(hWndCenter, &rcCenter);
      ::MapWindowPoints(hWndCenter, hWndParent, (POINT *)&rcCenter, 2);
    }

    int DlgWidth = rcDlg.right - rcDlg.left;
    int DlgHeight = rcDlg.bottom - rcDlg.top;

    int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
    int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;

    if (xLeft + DlgWidth > rcArea.right)
      xLeft = rcArea.right - DlgWidth;
    if (xLeft < rcArea.left)
      xLeft = rcArea.left;

    if (yTop + DlgHeight > rcArea.bottom)
      yTop = rcArea.bottom - DlgHeight;
    if (yTop < rcArea.top)
      yTop = rcArea.top;

    return ::SetWindowPos(m_hwnd, nullptr, xLeft, yTop, -1, -1,
                          SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
  }
#if _WIN32_WINNT >= 0x0500
  BOOL AnimateWindow(DWORD time, DWORD flags){
      WINASSERT(::IsWindow(m_hwnd));
      return ::AnimateWindow(m_hwnd,time,flags);
  }

  BOOL AnimateWindow(DWORD time, AnimateType type){
      WINASSERT(::IsWindow(m_hwnd));
      return ::AnimateWindow(m_hwnd,time,static_cast<DWORD>(type));
  }

#endif
};


RECT Window::rcDefault = {CW_USEDEFAULT, CW_USEDEFAULT, 0, 0};


template <DWORD m_dwStyle = 0, DWORD m_dwExStyle = 0> class WinTraits {
public:
  static DWORD GetStyle(DWORD dwStyle = 0) {
    return dwStyle ? dwStyle : m_dwStyle;
  }

  static DWORD GetStyleEx(DWORD dwExStyle = 0) {
    return dwExStyle ? dwExStyle : m_dwExStyle;
  }
};


typedef WinTraits<WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0>
    ControlTraits;
typedef WinTraits<WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                  WS_EX_APPWINDOW | WS_EX_WINDOWEDGE> FrameWinTraits;
typedef WinTraits<WS_OVERLAPPEDWINDOW | WS_CHILD | WS_VISIBLE |
                      WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                  WS_EX_MDICHILD> MDIChildTriats;
typedef WinTraits<0, 0> nullptrTraits;

template <DWORD m_dwStyle = 0, DWORD m_dwExStyle = 0,
          class TWinTraits = ControlTraits>
class WinTraitsOR {
public:
  static DWORD GetWndStyle(DWORD dwStyle) {
    return dwStyle | m_dwStyle | TWinTraits::GetWndStyle(dwStyle);
  }

  static DWORD GetWndExStyle(DWORD dwStyle) {
    return dwStyle | m_dwStyle | TWinTraits::GetWndExStyle(dwStyle);
  }
};


struct CreateWndData {
  void *pThis;
  DWORD dwThreadId;
};


class CriticalSection {
public:
  CriticalSection() { ::InitializeCriticalSection(&m_Sec); }
  ~CriticalSection() { ::DeleteCriticalSection(&m_Sec); }

  HRESULT Lock() {
    EnterCriticalSection(&m_Sec);
    return S_OK;
  }
  HRESULT UnLock() {
    LeaveCriticalSection(&m_Sec);
    return S_OK;
  }

  CRITICAL_SECTION m_Sec;
};


struct CriticalSectionLock {
  CriticalSection *m_t;
  CriticalSectionLock(CriticalSection &t) : m_t(&t) { m_t->Lock(); }
  ~CriticalSectionLock() { m_t->UnLock(); }
};


CriticalSection _wndCS;
std::vector<CreateWndData> _wndData;

#ifdef _X86_
#pragma pack(push, 1)
struct _WndProcThunk {
  DWORD m_mov;     // mov dword ptr [esp+0x4], pThis (esp+0x4 is hWnd)
  DWORD m_this;    // replaces hWnd pointer with the 'this' pointer on the
                   // parameter stack
  BYTE m_jmp;      // jmp commmand
  DWORD m_relproc; // relative jmp (address of EndProc)
  void *GetCodeAddress() { return this; }
};
#pragma pack(pop)
#endif
#ifdef __x86_64__
#pragma pack(push, 2)
struct _WndProcThunk {
  USHORT m_raxMov;  // mov rcx,[pthis]
  ULONG64 m_raxImm; // [pthis]
  USHORT m_rcxMov;  // mov rax ,[proc]
  ULONG64 m_rcxImm; // [proc]
  USHORT m_raxJmp;  // jmp rax
  void *GetCodeAddress() { return this; }
};
#pragma pack(pop)
#endif

class WndProcThunk {
public:
#ifdef __x86_64__
  _WndProcThunk *thunk; // for x86_64 we need to VirtualAlloc the thunk code,
                        // and make the page executable
#else
  _WndProcThunk thunk; // for x86 we need only to stack allocate it, which fails
                       // at runtime with msvc.
#endif
  WndProcThunk(){
#ifdef __x86_64__
    thunk = nullptr;
#endif
  }

  void Init(WNDPROC proc, void *pThis) {
#ifdef __x86_64__
    thunk = (_WndProcThunk *)::VirtualAlloc(0, sizeof(_WndProcThunk),
                                            MEM_COMMIT | MEM_RESERVE,
                                            PAGE_EXECUTE_READWRITE);
    thunk->m_rcxMov = 0xb948;         // x86_64 op code : mov rcx,this
    thunk->m_rcxImm = (ULONG64)pThis; // this pointer
    thunk->m_raxMov = 0xb848;         // x86_64 op code : mov rax,target
    thunk->m_raxImm = (ULONG64)proc;  // target pointer
    thunk->m_raxJmp = 0xe0ff;         // jmp rax
    ::FlushInstructionCache(::GetCurrentProcess(), thunk,
                            sizeof(_WndProcThunk));
#else
    thunk.m_mov = 0x042444C7; // x86 op code : mov eax, DWORD_PTR this
    thunk.m_this = (ULONG_PTR)(ULONG) pThis; // this
    thunk.m_jmp = 0xe9; // x86 op code  : jmp relproc  (relproc is calculated to
                        // be the address at the end of this structure)
    thunk.m_relproc =
        DWORD((INT_PTR)proc -
              ((INT_PTR) this + sizeof(_WndProcThunk))); // adjust address of
                                                         // Proc to compensate
                                                         // for size of the
                                                         // structure.
    ::FlushInstructionCache(::GetCurrentProcess(), &thunk,
                            sizeof(_WndProcThunk));
// actual assembly generation is as follows due to __stdcall calling
// conventions on x86:
// mov eax,this
// call BaseWindow::WindowProc (who's address is replaced as the beginning of
// the jmp instruction in this struct)
// sub eax,al (from the argument stack) (replacing HWND with the this ptr)
// jmp  BaseWindow::EndProc
#endif
  }

  WNDPROC GetWNDPROC() {
#ifdef __x86_64__
    return (WNDPROC)thunk->GetCodeAddress();
#else
    return (WNDPROC)thunk.GetCodeAddress();
#endif
  }

  ~WndProcThunk() {
#ifdef __x86_64__
    if (thunk)
      if(!VirtualFree(thunk, sizeof(_WndProcThunk), MEM_DECOMMIT)){
         ::MessageBox(nullptr,TEXT("Failed To deallocate winproc thunk"),TEXT("Error"),MB_OK | MB_ICONERROR);
      }
#endif
  }
};

class MessageMap {
public:
  virtual BOOL HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                             LRESULT &lResult, DWORD dwMsgMapId) = 0;
};

template <typename TBase = Window>
class WindowImplRoot : public TBase, public MessageMap {
public:
  WndProcThunk m_thunk;
  DWORD m_state;
  enum { WINSTATE_DESTROYED = 0x00000001 };

  WindowImplRoot() : m_state(0) {}

  virtual ~WindowImplRoot() {}

  LRESULT ForwardNotifications(HWND hWnd, UINT msg, WPARAM wParam,
                               LPARAM lParam, BOOL &bHandled);
  LRESULT ReflectNotifications(HWND hWnd, UINT msg, WPARAM wParam,
                               LPARAM lParam, BOOL &bHandled);
  BOOL DefaultReflectNotificationHandler(HWND hWnd, UINT msg, WPARAM wParam,
                                         LPARAM lParam, BOOL &bHandled);
};

template <typename TBase>
LRESULT
WindowImplRoot<TBase>::ReflectNotifications(HWND hWnd, UINT msg, WPARAM wParam,
                                            LPARAM lParam, BOOL &bHandled) {

  HWND hchild = nullptr;
  switch (msg) {
  case WM_COMMAND:
    if (lParam != 0) // not from a menu
      hchild = reinterpret_cast<HWND>(lParam);
    break;
  case WM_NOTIFY:
    hchild = reinterpret_cast<LPNMHDR>(lParam)->hwndFrom;
    break;
  case WM_PARENTNOTIFY:
    switch (LOWORD(wParam)) {
    case WM_CREATE:
    case WM_DESTROY:
      hchild = reinterpret_cast<HWND>(lParam);
      break;
    default:
      hchild = TBase::GetDlgItem(HIWORD(wParam));
      break;
    }
    break;
  case WM_DRAWITEM:
    if (wParam) // not from a menu
      hchild = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam)->hwndItem;
    break;
  case WM_MEASUREITEM:
    if (wParam) // not from a menu
      hchild =
          TBase::GetDlgItem(((LPMEASUREITEMSTRUCT)wParam)->CtlID);
    break;
  case WM_COMPAREITEM:
    if (wParam) // not from a menu
      hchild = reinterpret_cast<LPCOMPAREITEMSTRUCT>(wParam)->hwndItem;
    break;
  case WM_DELETEITEM:
    if (wParam) // not from a menu
      hchild = reinterpret_cast<DELETEITEMSTRUCT *>(lParam)->hwndItem;
    break;
  case WM_VKEYTOITEM:
  case WM_CHARTOITEM:
  case WM_HSCROLL:
  case WM_VSCROLL:
    hchild = reinterpret_cast<HWND>(lParam);
    break;
  case WM_CTLCOLORBTN:
  case WM_CTLCOLORDLG:
  case WM_CTLCOLOREDIT:
  case WM_CTLCOLORLISTBOX:
  case WM_CTLCOLORMSGBOX:
  case WM_CTLCOLORSTATIC:
    hchild = reinterpret_cast<HWND>(lParam);
    break;
  }
  if (hchild == false) {
    bHandled = false;
    return 1;
  }
  return ::SendMessage(hchild, OCM__BASE + msg, wParam, lParam);
}


template <typename TBase>
LRESULT
WindowImplRoot<TBase>::ForwardNotifications(HWND hWnd, UINT uMsg, WPARAM wParam,
                                            LPARAM lParam, BOOL &bHandled) {

  LRESULT lResult = 0;
  switch (uMsg) {
  case WM_COMMAND:
  case WM_NOTIFY:
  case WM_PARENTNOTIFY:
  case WM_DRAWITEM:
  case WM_MEASUREITEM:
  case WM_COMPAREITEM:
  case WM_DELETEITEM:
  case WM_VKEYTOITEM:
  case WM_CHARTOITEM:
  case WM_HSCROLL:
  case WM_VSCROLL:
  case WM_CTLCOLORBTN:
  case WM_CTLCOLORDLG:
  case WM_CTLCOLOREDIT:
  case WM_CTLCOLORLISTBOX:
  case WM_CTLCOLORMSGBOX:
  case WM_CTLCOLORSCROLLBAR:
  case WM_CTLCOLORSTATIC:
    lResult = TBase::GetParent().SendMessage(uMsg, wParam, lParam);
    break;
  default:
    bHandled = FALSE;
    break;
  }
  return lResult;
}

template <typename TBase>
BOOL WindowImplRoot<TBase>::DefaultReflectNotificationHandler(
    HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) {

  LRESULT lresult = 0;
  switch (msg) {
  case OCM_COMMAND:
  case OCM_NOTIFY:
  case OCM_PARENTNOTIFY:
  case OCM_DRAWITEM:
  case OCM_MEASUREITEM:
  case OCM_COMPAREITEM:
  case OCM_DELETEITEM:
  case OCM_VKEYTOITEM:
  case OCM_CHARTOITEM:
  case OCM_HSCROLL:
  case OCM_VSCROLL:
  case OCM_CTLCOLORBTN:
  case OCM_CTLCOLORDLG:
  case OCM_CTLCOLOREDIT:
  case OCM_CTLCOLORLISTBOX:
  case OCM_CTLCOLORMSGBOX:
  case OCM_CTLCOLORSCROLLBAR:
  case OCM_CTLCOLORSTATIC:
    lresult = ::DefWindowProc(TBase::m_hwnd, msg - OCM__BASE, wParam,
                              lParam);
    return TRUE;
  default:
    break;
  }
  return FALSE;
}

template <typename TBase, class TWinTraits = ControlTraits>
class BaseWindowImplT : public WindowImplRoot<TBase> {

public:
  WNDPROC m_pSuperWindowProc;
  WndProcThunk m_thunk;
  WNDPROC oldProc;

  BaseWindowImplT() : m_pSuperWindowProc(::DefWindowProc) {}

  static DWORD GetWndStyle(DWORD dwStyle) {
    return TWinTraits::GetWndStyle(dwStyle);
  }

  static DWORD GetWndExStyle(DWORD dwExStyle) {
    return TWinTraits::GetWndExStyle(dwExStyle);
  }

  virtual WNDPROC GetWindowProc() { return EndProc; }

  static LRESULT CALLBACK
      EndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    BaseWindowImplT<TBase, TWinTraits> *pThis =
        (BaseWindowImplT<TBase, TWinTraits> *)hwnd;

    LRESULT lRes = 0;
    BOOL bRet = pThis->HandleMessage(hwnd, uMsg, wParam, lParam, lRes, 0);

    if (!bRet) {
      if (uMsg != WM_NCDESTROY) {
        lRes = pThis->DefWindowProc(uMsg, wParam, lParam);
      } else {
#ifdef __x86_64__
        // unsubclass if necessary
        if ((WNDPROC)pThis->oldProc != (WNDPROC)pThis->m_thunk.thunk &&
            ((WNDPROC)::GetWindowLongPtr(pThis->m_hwnd, GWLP_WNDPROC) ==
             (WNDPROC)(pThis->m_thunk.thunk))) {
          ::SetWindowLongPtr(pThis->m_hwnd, GWLP_WNDPROC,
                             (LONG_PTR)pThis->oldProc);
          return lRes;
        }
#else
        if (pThis->oldProc != (WNDPROC)&pThis->m_thunk.thunk &&
            ((WNDPROC)::GetWindowLongPtr(pThis->m_hwnd, GWLP_WNDPROC) ==
             (WNDPROC)&pThis->m_thunk.thunk)) {
          ::SetWindowLongPtr(pThis->m_hwnd, GWLP_WNDPROC,
                             (LONG_PTR)pThis->oldProc);
          return lRes;
        }
#endif
        pThis->m_state = WindowImplRoot<TBase>::WINSTATE_DESTROYED;
      }
    }
    if (pThis->m_state == WindowImplRoot<TBase>::WINSTATE_DESTROYED) {
      HWND hWndThis = pThis->m_hwnd;
      pThis->m_hwnd = nullptr;
      pThis->OnFinalMessage(hWndThis);
    }

    return lRes;
  }

  static LRESULT CALLBACK
      WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    /* This function is only called ONCE, on the FIRST message a window receives
    // during creation which is WM_GETMINMAXINFO. So the use of a the standard
    // library container to store the class pointer and then search for it is
    // extremely negligible in the performance of this library, as it only
    // occurs
    // during window creation. This may slow down initialization of your
    // application,
    // but if it does it will not be a measurable amount until you start adding
    // more
    // than one thread in which tremendous amounts of windows are created. And
    // in
    // that case performance degradation is to be expected because of the
    // necessity
    // of allocating system resources. Most likely the critical section will be
    // the
    // dominating factor if performance degradation is observed, as the search
    // is
    // performed from the reverse end of the container (ie, last inserted).
    // The thunk applied to the window procedure completely replaces the window
    // procedure with EndProc. After the thunk is inserted into the windows
    // memory,
    // subsequent calls to the windowprocedure by windows will invoke the
    // EndProc
    // function with the HWND parameter replaced with the class pointer. This is
    // dynamic code execution, currently only supported on x86 and x86_64
    // architectures for MinGW/MinGW64 with this library. */

    CriticalSectionLock lock(_wndCS);
    auto ret =
        find_if(_wndData.rbegin(), _wndData.rend(), [&](CreateWndData &dat) {
          return dat.dwThreadId == ::GetCurrentThreadId();
        });
    if (ret != _wndData.rend()) {
      BaseWindowImplT<TBase, TWinTraits> *pThis =
          (BaseWindowImplT<TBase, TWinTraits> *)ret->pThis;
      pThis->m_hwnd = hwnd;
      pThis->m_thunk.Init(pThis->GetWindowProc(), pThis);
#ifdef __x86_64__
      WNDPROC pProc = (WNDPROC)(pThis->m_thunk.thunk);
      pThis->oldProc =
          (WNDPROC)::SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)pProc);
#else
      WNDPROC pProc = (WNDPROC) & (pThis->m_thunk.thunk);
      pThis->oldProc =
          (WNDPROC)::SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)pProc);
#endif
      if (pThis->oldProc != WindowProc)
        WINTRACE(0, TEXT("Sub-classing through a discarded hook.\n"));

      return pProc(hwnd, uMsg, wParam, lParam);
    } else
      return 0;
  }

  HWND Create(HWND hWndParent, LPRECT rect, LPCTSTR szWindowName, DWORD dwStyle,
              DWORD dwExStyle, HMENU MenuOrID, ATOM atom,
              LPVOID lpCreateParam) {
    if (atom == 0)
      return nullptr;

    _wndCS.Lock();
    _wndData.push_back({this, ::GetCurrentThreadId()});
    _wndCS.UnLock();

    if (MenuOrID == nullptr && (dwStyle & WS_CHILD))
      MenuOrID = (HMENU)(UINT_PTR) this;
    if (rect == nullptr)
      rect = &TBase::rcDefault;

    if (dwStyle == 0)
      dwStyle = TWinTraits::GetStyle();
    if (dwExStyle == 0)
      dwExStyle = TWinTraits::GetStyleEx();

    HWND hWnd = ::CreateWindowEx(
        dwExStyle, MAKEINTATOM(atom), szWindowName, dwStyle, rect->left,
        rect->top, rect->right - rect->left, rect->bottom - rect->top,
        hWndParent, MenuOrID, HINST_THISCOMPONENT, lpCreateParam);

    return hWnd;
  }

  BOOL SubClassWindow(HWND hWnd) {
    WINCHECK(hWnd);
    m_thunk.Init(EndProc, this);
#ifdef __x86_64__
    WNDPROC pProc = (WNDPROC)(m_thunk.thunk);
    oldProc = (WNDPROC)::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)pProc);
#else
    WNDPROC pProc = (WNDPROC) & (m_thunk.thunk);
    oldProc = (WNDPROC)::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)pProc);
#endif
    if (oldProc == nullptr)
      false;

    ((BaseWindowImplT<TBase, TWinTraits> *)this)->m_hwnd = hWnd;
    return S_OK;
  }

  HWND UnSubClassWindow(BOOL force = FALSE) {
    HWND hWndo = ((BaseWindowImplT<TBase, TWinTraits> *)this)->m_hwnd;
    WINCHECK(hWndo);
#ifdef __x86_64__
    WNDPROC ourproc = (WNDPROC)(m_thunk.thunk);
#else
    WNDPROC ourproc = (WNDPROC) & (m_thunk.thunk);
#endif
    WNDPROC activeproc = (WNDPROC)::GetWindowLongPtr(hWndo, GWLP_WNDPROC);
    HWND hWnd = nullptr;
    if (force || ourproc == activeproc) {
      if (!::SetWindowLongPtr(hWndo, GWLP_WNDPROC, (LONG_PTR)oldProc))
        return nullptr;

      oldProc = ::DefWindowProc;
      hWnd = hWndo;
      hWndo = nullptr;
    }
    return hWnd;
  }

  BOOL DestroyWindow() {
    HWND hWnd = ((BaseWindowImplT<TBase, TWinTraits> *)this)->m_hwnd;
    WINASSERT(::IsWindow(hWnd));
    return ::DestroyWindow(hWnd);
  }

  LRESULT DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    return ::CallWindowProc(
        m_pSuperWindowProc,
        ((BaseWindowImplT<TBase, TWinTraits> *)this)->m_hwnd, uMsg, wParam,
        lParam);
  }

  virtual ATOM RegisterClass() = 0;
  virtual void OnFinalMessage(HWND) {}
};

template <typename T, typename TBaseWindow, typename TWindowTraits>
class BaseWindow : public BaseWindowImplT<TBaseWindow, TWindowTraits> {

public:
  HWND Create(HWND hWndParent = nullptr, LPRECT rect = nullptr,
              LPCTSTR szWindowName = nullptr, DWORD dwStyle = 0,
              DWORD dwExStyle = 0, HMENU MenuOrID = 0U,
              LPVOID lpCreateParam = nullptr) {
    if (rect == nullptr)
      *rect = Window::rcDefault;

    ATOM atom = this->RegisterClass();
    LPTSTR st = MAKEINTATOM(atom);
    return BaseWindowImplT<TBaseWindow, TWindowTraits>::Create(
        hWndParent, rect, szWindowName, dwStyle, dwExStyle, MenuOrID, atom,
        lpCreateParam);
  }

  HWND Create(LPCTSTR lpWindowName, HWND hWndParent = nullptr,
              DWORD dwStyle = 0, DWORD dwExStyle = 0, int x = CW_USEDEFAULT,
              int y = CW_USEDEFAULT, int nWidth = CW_USEDEFAULT,
              int nHeight = CW_USEDEFAULT, HMENU hMenu = 0,
              LPVOID lpCreateParam = nullptr) {

    RECT rect = {x, y, nWidth, nHeight};
    HWND hwnd = this->Create(
        (HWND)hWndParent, (LPRECT)&rect, (LPCTSTR)lpWindowName, (DWORD)dwStyle,
        (DWORD)dwExStyle, (HMENU)hMenu, (LPVOID)lpCreateParam);

    return hwnd;
  }

  HWND Create(HWND hWndParent = nullptr, LPCTSTR lpWindowName = nullptr,
              DWORD dwStyle = 0, DWORD dwExStyle = 0, int x = 0, int y = 0,
              int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT,
              HMENU hMenu = 0, LPVOID lpCreateParam = nullptr) {

    RECT rect = {x, y, nWidth, nHeight};
    HWND hwnd = this->Create(
        (HWND)hWndParent, (LPRECT)&rect, (LPCTSTR)lpWindowName, (DWORD)dwStyle,
        (DWORD)dwExStyle, (HMENU)hMenu, (LPVOID)lpCreateParam);

    return hwnd;
  }

  HWND Create(HWND hWndParent = nullptr, URECT rc = nullptr,
              UStringOrID lpWindowName = nullptr, DWORD dwStyle = 0,
              DWORD dwExStyle = 0, UMenuOrID hMenu = nullptr,
              LPVOID lpCreateParam = nullptr) {

    HWND hwnd = this->Create((HWND)hWndParent, (LPRECT)rc.Get(),
                             (LPCTSTR)lpWindowName.Get(), (DWORD)dwStyle,
                             (DWORD)dwExStyle, (HMENU)hMenu.Get(),
                             (LPVOID)lpCreateParam);

    return hwnd;
  }

  virtual ATOM RegisterClass() override {
    WNDCLASSEX wcx = T::GetWinClassInfo();
    return ::RegisterClassEx(&wcx);
  }
};

enum class StaticMessages : UINT {
    GetImage = STM_GETIMAGE,
    SetImage = STM_SETIMAGE,
    GetIcon = STM_GETICON,
    SetIcon = STM_SETICON
};

// Standard-Controls.
template <typename TBase> class StaticT : public TBase {

public:
  StaticT(HWND hWnd = nullptr) : TBase(hWnd) {}

  StaticT<TBase> &operator=(HWND hWnd) {
    TBase::m_hwnd = hWnd;
    return *this;
  }

  HWND Create(HWND hWndParent = nullptr, URECT rc = nullptr,
              UStringOrID windowName = nullptr, DWORD dwStyle = 0,
              DWORD dwExStyle = 0, UMenuOrID menu = 0U,
              LPVOID lpCreateParam = nullptr) {
    return TBase::Create(GetWndClassName(), hWndParent, rc.Get(),
                         windowName.Get(), dwStyle, dwExStyle, menu.Get(),
                         lpCreateParam);
  }

  static LPCTSTR GetWndClassName() { return TEXT("STATIC"); }

  HICON GetIcon() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (HICON)::SendMessage(TBase::m_hwnd, static_cast<UINT>(WindowsMessage::GetIcon), 0, 0L);
  }

  HICON SetIcon(HICON icon) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (HICON)::SendMessage(TBase::m_hwnd, static_cast<UINT>(WindowsMessage::SetIcon),
                                (WPARAM)icon, 0L);
  }

  HENHMETAFILE GetEnhMetaFile() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (HENHMETAFILE)::SendMessage(TBase::m_hwnd, static_cast<UINT>(StaticMessages::GetImage),
                                       IMAGE_ENHMETAFILE, 0L);
  }

  HENHMETAFILE SetEnhMetaFile(HENHMETAFILE file) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (HENHMETAFILE)::SendMessage(TBase::m_hwnd, static_cast<UINT>(StaticMessages::SetImage),
                                       IMAGE_ENHMETAFILE, (LPARAM)file);
  }

  BitmapHandle GetBitmap() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return BitmapHandle((HBITMAP)::SendMessage(TBase::m_hwnd,
                                               static_cast<UINT>(StaticMessages::GetImage), IMAGE_BITMAP, 0L));
  }

  BitmapHandle SetBitmap(HBITMAP bitmap) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return BitmapHandle((HBITMAP)::SendMessage(
        TBase::m_hwnd, static_cast<UINT>(StaticMessages::SetImage), IMAGE_BITMAP, (LPARAM)bitmap));
  }

  HCURSOR GetCursor() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (HCURSOR)::SendMessage(TBase::m_hwnd, static_cast<UINT>(StaticMessages::GetImage),
                                  IMAGE_CURSOR, 0L);
  }

  HCURSOR SetCursor(HCURSOR cursor) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (HCURSOR)::SendMessage(TBase::m_hwnd, static_cast<UINT>(StaticMessages::SetImage),
                                  IMAGE_CURSOR, (LPARAM)cursor);
  }
};

typedef StaticT<Window> Static;

enum class ButtonStyles : DWORD {
    TriState = BS_3STATE,
    AutoTriState = BS_AUTO3STATE,
    AutoCheckBox = BS_AUTOCHECKBOX,
    AutoRadioButton = BS_AUTORADIOBUTTON,
    CheckBox = BS_CHECKBOX,
    DefaultPushButton = BS_DEFPUSHBUTTON,
    GroupBox = BS_GROUPBOX,
    LeftText = BS_LEFTTEXT,
    OwnderDraw = BS_OWNERDRAW,
    PushButton = BS_PUSHBUTTON,
    RadioButton = BS_RADIOBUTTON,
    UserButton = BS_USERBUTTON,
    Bitmap = BS_BITMAP,
    Bottom = BS_BOTTOM,
    Center = BS_CENTER,
    Icon = BS_ICON,
    Flat = BS_FLAT,
    Left = BS_LEFT,
    MultiLine = BS_MULTILINE,
    Notify = BS_NOTIFY,
    PushLike = BS_PUSHLIKE,
    Right = BS_RIGHT,
    RightButton = BS_RIGHTBUTTON,
    Text = BS_TEXT,
    Top = BS_TOP,
    TypeMask = BS_TYPEMASK,
    VCenter = BS_VCENTER,
    SplitButton = BS_SPLITBUTTON,
    DefaultSplitButton = BS_DEFSPLITBUTTON,
    CommandLink = BS_COMMANDLINK,
    DefaultCommandLink = BS_DEFCOMMANDLINK
};

class ButtonType {
    DWORD m_type;
public:
    ButtonType(ButtonStyles style) : m_type(
                                            static_cast<DWORD>(style)) {
        m_type |= ControlTraits::GetStyle();
    }
    ButtonType(std::initializer_list<ButtonStyles> styles) : m_type(0){
        m_type |= ControlTraits::GetStyle();
        for(auto i = styles.begin(); i != styles.end(); i++){
            m_type |= static_cast<DWORD>(*i);
        }
    }
    operator DWORD() { return m_type; }
};

template <typename TBase> class ButtonT : public TBase {

public:
  ButtonT(HWND hwnd = nullptr) : TBase(hwnd) {}

  ButtonT<TBase> &operator=(HWND hwnd) {
    TBase::m_hwnd = hwnd;
    return *this;
  }

  HWND Create(HWND hWndParent = nullptr, URECT rc = nullptr,
              UStringOrID windowName = nullptr, DWORD dwStyle = 0,
              DWORD dwExStyle = 0, UMenuOrID menu = nullptr,
              LPVOID lpCreateParam = nullptr) {
    return TBase::Create(GetWndClassName(), hWndParent, rc.Get(),
                         windowName.Get(),dwStyle, dwExStyle, menu.Get(),
                         lpCreateParam);
  }

  static LPCTSTR GetWndClassName() { return TEXT("BUTTON"); }


  DWORD GetStyle() const {
    WINASSERT(TBase::m_hwnd);
    return (DWORD)::GetWindowLong(TBase::m_hwnd, GWL_STYLE);
  }

  UINT GetState() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (UINT)::SendMessage(TBase::m_hwnd, BM_GETSTATE, 0, 0L);
  }

  void SetState(BOOL Highlight) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, BM_SETSTATE, Highlight, 0L);
  }

  int GetCheck() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, BM_GETCHECK, 0, 0L);
  }

  void SetCheck(int Check) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, BM_SETCHECK, Check, 0L);
  }

  UINT GetButtonStyle() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (UINT)::GetWindowLong(TBase::m_hwnd, GWL_STYLE) & 0xFFFF;
  }

  void SetButtonStyle(UINT style, BOOL redraw = TRUE) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, BM_SETSTYLE, style, (LPARAM)redraw);
  }

  HICON GetIcon() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (HICON)::SendMessage(TBase::m_hwnd, BM_GETIMAGE,
                                IMAGE_ICON, 0L);
  }

  HICON SetIcon(HICON icon) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (HICON)::SendMessage(TBase::m_hwnd, BM_SETIMAGE,
                                IMAGE_ICON, (LPARAM)icon);
  }

#if (_WIN32_WINNT >= 0x0501)
  BOOL GetIdealSize(LPSIZE lpSize) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (BOOL)::SendMessage(TBase::m_hwnd, BCM_GETIDEALSIZE, 0,
                               (LPARAM)lpSize);
  }

  BOOL GetImageList(PBUTTON_IMAGELIST pButtonImagelist) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (BOOL)::SendMessage(TBase::m_hwnd, BCM_GETIMAGELIST, 0,
                               (LPARAM)pButtonImagelist);
  }

  BOOL SetImageList(PBUTTON_IMAGELIST pButtonImagelist) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (BOOL)::SendMessage(TBase::m_hwnd, BCM_SETIMAGELIST, 0,
                               (LPARAM)pButtonImagelist);
  }

  BOOL GetTextMargin(LPRECT lpRect) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (BOOL)::SendMessage(TBase::m_hwnd, BCM_GETTEXTMARGIN, 0,
                               (LPARAM)lpRect);
  }

  BOOL SetTextMargin(LPRECT lpRect) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (BOOL)::SendMessage(TBase::m_hwnd, BCM_SETTEXTMARGIN, 0,
                               (LPARAM)lpRect);
  }
#endif

#if (WINVER >= 0x0600)
  void SetDontClick(BOOL bDontClick) {
#ifndef BM_SETDONTCLICK
#define BM_SETDONTCLICK 0x00F8
#endif
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, BM_SETDONTCLICK, (WPARAM)bDontClick,
                  0L);
  }

  BOOL SetDropDownState(BOOL bDropDown) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    WINASSERT((GetStyle() & (BS_SPLITBUTTON | BS_DEFSPLITBUTTON)) != 0);
    return (BOOL)::SendMessage(TBase::m_hwnd, BCM_SETDROPDOWNSTATE,
                               (WPARAM)bDropDown, 0L);
  }

  BOOL GetSplitInfo(PBUTTON_SPLITINFO pSplitInfo) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    WINASSERT((GetStyle() & (BS_SPLITBUTTON | BS_DEFSPLITBUTTON)) != 0);
    return (BOOL)::SendMessage(TBase::m_hwnd, BCM_GETSPLITINFO, 0,
                               (LPARAM)pSplitInfo);
  }

  BOOL SetSplitInfo(PBUTTON_SPLITINFO pSplitInfo) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    WINASSERT((GetStyle() & (BS_SPLITBUTTON | BS_DEFSPLITBUTTON)) != 0);
    return (BOOL)::SendMessage(TBase::m_hwnd, BCM_SETSPLITINFO, 0,
                               (LPARAM)pSplitInfo);
  }

  int GetNoteLength() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    WINASSERT((GetStyle() & (BS_COMMANDLINK | BS_DEFCOMMANDLINK)) != 0);
    return (int)::SendMessage(TBase::m_hwnd, BCM_GETNOTELENGTH, 0,
                              0L);
  }

  BOOL GetNote(LPWSTR lpstrNoteText, int cchNoteText) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    WINASSERT((GetStyle() & (BS_COMMANDLINK | BS_DEFCOMMANDLINK)) != 0);
    return (BOOL)::SendMessage(TBase::m_hwnd, BCM_GETNOTE,
                               cchNoteText, (LPARAM)lpstrNoteText);
  }

  BOOL SetNote(LPCWSTR lpstrNoteText) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    WINASSERT((GetStyle() & (BS_COMMANDLINK | BS_DEFCOMMANDLINK)) != 0);
    return (BOOL)::SendMessage(TBase::m_hwnd, BCM_SETNOTE, 0,
                               (LPARAM)lpstrNoteText);
  }

  LRESULT SetElevationRequiredState(BOOL bSet) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return ::SendMessage(TBase::m_hwnd, BCM_SETSHIELD, 0,
                         (LPARAM)bSet);
  }
#endif
  void Click() {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, BM_CLICK, 0, 0L);
  }
};

typedef ButtonT<Window> Button;



typedef ButtonT<Window> Button;

template <typename TBase, typename Traits> class ButtonImpl : public TBase {
public:
    HWND Create(HWND hWndParent = nullptr,
           Rect rc = {0,0,0,0},LPCTSTR lpText = nullptr,UMenuOrID id = nullptr){
        DWORD style = Traits::GetStyle();
        DWORD styleEx = Traits::GetStyleEx();
        return TBase::Create(hWndParent,rc,lpText,style,styleEx,id);
    }
};


typedef ButtonImpl<Button,WinTraits<WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,0>> CheckBox;
typedef ButtonImpl<Button,WinTraits<WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTO3STATE,0>> TriCheckBox;
typedef ButtonImpl<Button,WinTraits<WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,0>> RadioButton;
typedef ButtonImpl<Button,WinTraits<WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_COMMANDLINK,0>> CommandLink;
typedef ButtonImpl<Button,WinTraits<WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,0>> PushButton;


template <typename TBase> class ListBoxT : public TBase {

public:
  ListBoxT(HWND hwnd = nullptr) : TBase(hwnd) {}

  ListBoxT<TBase> &operator=(HWND hwnd) {
    TBase::m_hwnd = hwnd;
    return *this;
  }

  HWND Create(HWND hwndParent, URECT rc = nullptr,
              UStringOrID windowName = nullptr, DWORD dwStyle = 0,
              DWORD dwExStyle = 0, UMenuOrID menu = nullptr,
              LPVOID lpCreateParam = nullptr) {
    return TBase::Create(TEXT("LISTBOX"), hwndParent, rc.Get(),
                         windowName.Get(), dwStyle, dwExStyle, menu.Get(),
                         lpCreateParam);
  }

  static LPCTSTR GetWndClassName() { return TEXT("LISTBOX"); }

  DWORD GetStyle() const {
    WINASSERT(TBase::m_hwnd);
    return (DWORD)::GetWindowLong(TBase::m_hwnd, GWL_STYLE);
  }

  int GetCount() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, LB_GETCOUNT, 0, 0L);
  }

  int SetCount(int cItems) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    WINASSERT(((GetStyle() & LBS_NODATA) != 0) &&
              ((GetStyle() & LBS_HASSTRINGS) == 0));
    return (int)::SendMessage(TBase::m_hwnd, LB_SETCOUNT, cItems, 0L);
  }

  int GetHorizontalExtent() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, LB_GETHORIZONTALEXTENT,
                              0, 0L);
  }

  void SetHorizontalExtent(int cxExtent) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, LB_SETHORIZONTALEXTENT, cxExtent,
                  0L);
  }

  int GetTopIndex() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, LB_GETTOPINDEX, 0, 0L);
  }

  int SetTopIndex(int nIndex) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, LB_SETTOPINDEX, nIndex,
                              0L);
  }

  LCID GetLocale() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (LCID)::SendMessage(TBase::m_hwnd, LB_GETLOCALE, 0, 0L);
  }

  LCID SetLocale(LCID nNewLocale) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (LCID)::SendMessage(TBase::m_hwnd, LB_SETLOCALE,
                               (WPARAM)nNewLocale, 0L);
  }

  DWORD GetListBoxInfo() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
#if (_WIN32_WINNT >= 0x0501)
    return (DWORD)::SendMessage(TBase::m_hwnd, LB_GETLISTBOXINFO, 0,
                                0L);
#else
    return ::GetListBoxInfo(m_hwnd);
#endif
  }

  int GetCurSel() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    WINASSERT((GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) == 0);
    return (int)::SendMessage(TBase::m_hwnd, LB_GETCURSEL, 0, 0L);
  }

  int SetCurSel(int nSelect) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    WINASSERT((GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) == 0);
    return (int)::SendMessage(TBase::m_hwnd, LB_SETCURSEL, nSelect,
                              0L);
  }

  int GetSel(int nIndex) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, LB_GETSEL, nIndex, 0L);
  }

  int SetSel(int nIndex, BOOL bSelect = TRUE) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    WINASSERT((GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) != 0);
    return (int)::SendMessage(TBase::m_hwnd, LB_SETSEL, bSelect,
                              nIndex);
  }

  int GetSelCount() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    WINASSERT((GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) != 0);
    return (int)::SendMessage(TBase::m_hwnd, LB_GETSELCOUNT, 0, 0L);
  }

  int GetSelItems(int nMaxItems, LPINT rgIndex) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    WINASSERT((GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) != 0);
    return (int)::SendMessage(TBase::m_hwnd, LB_GETSELITEMS,
                              nMaxItems, (LPARAM)rgIndex);
  }

  int GetAnchorIndex() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    WINASSERT((GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) != 0);
    return (int)::SendMessage(TBase::m_hwnd, LB_GETANCHORINDEX, 0,
                              0L);
  }

  void SetAnchorIndex(int nIndex) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    WINASSERT((GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) != 0);
    ::SendMessage(TBase::m_hwnd, LB_SETANCHORINDEX, nIndex, 0L);
  }

  int GetCaretIndex() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, LB_GETCARETINDEX, 0, 0);
  }

  int SetCaretIndex(int nIndex, BOOL bScroll = TRUE) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, LB_SETCARETINDEX, nIndex,
                              MAKELONG(bScroll, 0));
  }

  DWORD_PTR GetItemData(int nIndex) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (DWORD_PTR)::SendMessage(TBase::m_hwnd, LB_GETITEMDATA,
                                    nIndex, 0L);
  }

  int SetItemData(int nIndex, DWORD_PTR dwItemData) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, LB_SETITEMDATA, nIndex,
                              (LPARAM)dwItemData);
  }

  void *GetItemDataPtr(int nIndex) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (void *)::SendMessage(TBase::m_hwnd, LB_GETITEMDATA,
                                 nIndex, 0L);
  }

  int SetItemDataPtr(int nIndex, void *pData) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return SetItemData(nIndex, (DWORD_PTR)pData);
  }

  int GetItemRect(int nIndex, LPRECT lpRect) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, LB_GETITEMRECT, nIndex,
                              (LPARAM)lpRect);
  }

  int GetText(int nIndex, LPTSTR lpszBuffer) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, LB_GETTEXT, nIndex,
                              (LPARAM)lpszBuffer);
  }

  int GetTextLen(int nIndex) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, LB_GETTEXTLEN, nIndex,
                              0L);
  }

  int GetItemHeight(int nIndex) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, LB_GETITEMHEIGHT, nIndex,
                              0L);
  }

  int SetItemHeight(int nIndex, UINT cyItemHeight) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, LB_SETITEMHEIGHT, nIndex,
                              MAKELONG(cyItemHeight, 0));
  }

  void SetColumnWidth(int cxWidth) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, LB_SETCOLUMNWIDTH, cxWidth, 0L);
  }

  BOOL SetTabStops(int nTabStops, LPINT rgTabStops) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    WINASSERT((GetStyle() & LBS_USETABSTOPS) != 0);
    return (BOOL)::SendMessage(TBase::m_hwnd, LB_SETTABSTOPS,
                               nTabStops, (LPARAM)rgTabStops);
  }

  BOOL SetTabStops() {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    WINASSERT((GetStyle() & LBS_USETABSTOPS) != 0);
    return (BOOL)::SendMessage(TBase::m_hwnd, LB_SETTABSTOPS, 0, 0L);
  }

  BOOL SetTabStops(const int &cxEachStop) // takes an 'int'
  {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    WINASSERT((GetStyle() & LBS_USETABSTOPS) != 0);
    return (BOOL)::SendMessage(TBase::m_hwnd, LB_SETTABSTOPS, 1,
                               (LPARAM)(LPINT) & cxEachStop);
  }

  int InitStorage(int nItems, UINT nBytes) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, LB_INITSTORAGE,
                              (WPARAM)nItems, nBytes);
  }

  void ResetContent() {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, LB_RESETCONTENT, 0, 0L);
  }

  UINT ItemFromPoint(POINT pt, BOOL &bOutside) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    DWORD dw = (DWORD)::SendMessage(TBase::m_hwnd, LB_ITEMFROMPOINT,
                                    0, MAKELPARAM(pt.x, pt.y));
    bOutside = (BOOL)HIWORD(dw);
    return (UINT)LOWORD(dw);
  }

  int AddString(LPCTSTR lpszItem) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, LB_ADDSTRING, 0,
                              (LPARAM)lpszItem);
  }

  int DeleteString(UINT nIndex) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, LB_DELETESTRING, nIndex,
                              0L);
  }

  int InsertString(int nIndex, LPCTSTR lpszItem) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, LB_INSERTSTRING, nIndex,
                              (LPARAM)lpszItem);
  }

  int Dir(UINT attr, LPCTSTR lpszWildCard) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, LB_DIR, attr,
                              (LPARAM)lpszWildCard);
  }

  int AddFile(LPCTSTR lpstrFileName) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, LB_ADDFILE, 0,
                              (LPARAM)lpstrFileName);
  }

  int FindString(int nStartAfter, LPCTSTR lpszItem) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, LB_FINDSTRING,
                              nStartAfter, (LPARAM)lpszItem);
  }

  int FindStringExact(int nIndexStart, LPCTSTR lpszFind) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, LB_FINDSTRINGEXACT,
                              nIndexStart, (LPARAM)lpszFind);
  }

  int SelectString(int nStartAfter, LPCTSTR lpszItem) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, LB_SELECTSTRING,
                              nStartAfter, (LPARAM)lpszItem);
  }

  int SelItemRange(BOOL bSelect, int nFirstItem, int nLastItem) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    WINASSERT((GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) != 0);
    WINASSERT(nFirstItem <= nLastItem);
    return bSelect
               ? (int)::SendMessage(TBase::m_hwnd, LB_SELITEMRANGEEX,
                                    nFirstItem, nLastItem)
               : (int)::SendMessage(TBase::m_hwnd, LB_SELITEMRANGEEX,
                                    nLastItem, nFirstItem);
  }
};

typedef ListBoxT<Window> ListBox;


template <typename TBase> class ComboBoxT : public TBase {

  ComboBoxT(HWND hWnd = NULL) : TBase(hWnd) {}
  ComboBoxT<TBase> &operator=(HWND hWnd) {
    TBase::m_hwnd = hWnd;
    return *this;
  }

  HWND Create(HWND hwndParent, URECT rc = nullptr,
              UStringOrID windowName = nullptr, DWORD dwStyle = 0,
              DWORD dwExStyle = 0, UMenuOrID menu = nullptr,
              LPVOID lpCreateParam = nullptr) {
    return TBase::Create(GetWndClassName(), hwndParent, rc.Get(),
                         windowName.Get(), dwStyle, dwExStyle, menu.Get(),
                         lpCreateParam);
  }

  static LPCTSTR GetWndClassName() { return TEXT("COMBOBOX"); }

  int GetCount() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, CB_GETCOUNT, 0, 0L);
  }

  int GetCurSel() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, CB_GETCURSEL, 0, 0L);
  }

  int SetCurSel(int nSelect) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, CB_SETCURSEL, nSelect,
                              0L);
  }

  LCID GetLocale() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (LCID)::SendMessage(TBase::m_hwnd, CB_GETLOCALE, 0, 0L);
  }

  LCID SetLocale(LCID nNewLocale) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (LCID)::SendMessage(TBase::m_hwnd, CB_SETLOCALE,
                               (WPARAM)nNewLocale, 0L);
  }

  int GetTopIndex() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, CB_GETTOPINDEX, 0, 0L);
  }

  int SetTopIndex(int nIndex) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, CB_SETTOPINDEX, nIndex,
                              0L);
  }

  UINT GetHorizontalExtent() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (UINT)::SendMessage(TBase::m_hwnd, CB_GETHORIZONTALEXTENT,
                               0, 0L);
  }

  void SetHorizontalExtent(UINT nExtent) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, CB_SETHORIZONTALEXTENT, nExtent, 0L);
  }

  int GetDroppedWidth() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, CB_GETDROPPEDWIDTH, 0,
                              0L);
  }

  int SetDroppedWidth(UINT nWidth) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, CB_SETDROPPEDWIDTH,
                              nWidth, 0L);
  }

#if ((WINVER >= 0x0500))
  BOOL GetComboBoxInfo(PCOMBOBOXINFO pComboBoxInfo) const {
    return (BOOL)::SendMessage(TBase::m_hwnd, CB_GETCOMBOBOXINFO, 0,
                               (LPARAM)pComboBoxInfo);
  }
#endif

  DWORD GetEditSel() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (DWORD)::SendMessage(TBase::m_hwnd, CB_GETEDITSEL, 0, 0L);
  }

  BOOL SetEditSel(int nStartChar, int nEndChar) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (BOOL)::SendMessage(TBase::m_hwnd, CB_SETEDITSEL, 0,
                               MAKELONG(nStartChar, nEndChar));
  }

  DWORD_PTR GetItemData(int nIndex) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (DWORD_PTR)::SendMessage(TBase::m_hwnd, CB_GETITEMDATA,
                                    nIndex, 0L);
  }

  int SetItemData(int nIndex, DWORD_PTR dwItemData) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, CB_SETITEMDATA, nIndex,
                              (LPARAM)dwItemData);
  }

  void *GetItemDataPtr(int nIndex) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (void *)GetItemData(nIndex);
  }

  int SetItemDataPtr(int nIndex, void *pData) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return SetItemData(nIndex, (DWORD_PTR)pData);
  }

  int GetLBText(int nIndex, LPTSTR lpszText) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, CB_GETLBTEXT, nIndex,
                              (LPARAM)lpszText);
  }

  int GetLBTextLen(int nIndex) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, CB_GETLBTEXTLEN, nIndex,
                              0L);
  }

  int GetItemHeight(int nIndex) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, CB_GETITEMHEIGHT, nIndex,
                              0L);
  }

  int SetItemHeight(int nIndex, UINT cyItemHeight) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, CB_SETITEMHEIGHT, nIndex,
                              MAKELONG(cyItemHeight, 0));
  }

  BOOL GetExtendedUI() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (BOOL)::SendMessage(TBase::m_hwnd, CB_GETEXTENDEDUI, 0,
                               0L);
  }

  int SetExtendedUI(BOOL bExtended = TRUE) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, CB_SETEXTENDEDUI,
                              bExtended, 0L);
  }

  void GetDroppedControlRect(LPRECT lprect) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, CB_GETDROPPEDCONTROLRECT, 0,
                  (LPARAM)lprect);
  }

  BOOL GetDroppedState() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (BOOL)::SendMessage(TBase::m_hwnd, CB_GETDROPPEDSTATE, 0,
                               0L);
  }

#if (_WIN32_WINNT >= 0x0501)
  int GetMinVisible() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, CB_GETMINVISIBLE, 0, 0L);
  }

  BOOL SetMinVisible(int nMinVisible) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (BOOL)::SendMessage(TBase::m_hwnd, CB_SETMINVISIBLE,
                               nMinVisible, 0L);
  }

  BOOL GetCueBannerText(LPWSTR lpwText, int cchText) const {
#ifndef CB_GETCUEBANNER
    const UINT CB_GETCUEBANNER = (CBM_FIRST + 4);
#endif
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (BOOL)::SendMessage(TBase::m_hwnd, CB_GETCUEBANNER,
                               (WPARAM)lpwText, cchText);
  }

  // Vista only
  BOOL SetCueBannerText(LPCWSTR lpcwText) {
#ifndef CB_SETCUEBANNER
    const UINT CB_SETCUEBANNER = (CBM_FIRST + 3);
#endif
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (BOOL)::SendMessage(TBase::m_hwnd, CB_SETCUEBANNER, 0,
                               (LPARAM)lpcwText);
  }
#endif

  int InitStorage(int nItems, UINT nBytes) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, CB_INITSTORAGE,
                              (WPARAM)nItems, nBytes);
  }

  void ResetContent() {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, CB_RESETCONTENT, 0, 0L);
  }

  BOOL LimitText(int nMaxChars) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (BOOL)::SendMessage(TBase::m_hwnd, CB_LIMITTEXT, nMaxChars,
                               0L);
  }

  void ShowDropDown(BOOL bShowIt = TRUE) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, CB_SHOWDROPDOWN, bShowIt, 0L);
  }

  int AddString(LPCTSTR lpszString) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, CB_ADDSTRING, 0,
                              (LPARAM)lpszString);
  }

  int DeleteString(UINT nIndex) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, CB_DELETESTRING, nIndex,
                              0L);
  }

  int InsertString(int nIndex, LPCTSTR lpszString) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, CB_INSERTSTRING, nIndex,
                              (LPARAM)lpszString);
  }

  int Dir(UINT attr, LPCTSTR lpszWildCard) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, CB_DIR, attr,
                              (LPARAM)lpszWildCard);
  }

  int FindString(int nStartAfter, LPCTSTR lpszString) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, CB_FINDSTRING,
                              nStartAfter, (LPARAM)lpszString);
  }

  int FindStringExact(int nIndexStart, LPCTSTR lpszFind) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, CB_FINDSTRINGEXACT,
                              nIndexStart, (LPARAM)lpszFind);
  }

  int SelectString(int nStartAfter, LPCTSTR lpszString) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, CB_SELECTSTRING,
                              nStartAfter, (LPARAM)lpszString);
  }

  void Clear() {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, WM_CLEAR, 0, 0L);
  }

  void Copy() {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, WM_COPY, 0, 0L);
  }

  void Cut() {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, WM_CUT, 0, 0L);
  }

  void Paste() {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, WM_PASTE, 0, 0L);
  }
};

typedef ComboBoxT<Window> ComboBox;

template <typename TBase> class EditT : public TBase {

public:
  EditT(HWND hWnd = NULL) : TBase(hWnd) {}
  EditT<TBase> &operator=(HWND hWnd) {
    TBase::m_hwnd = hWnd;
    return *this;
  }

  HWND Create(HWND hwndParent, URECT rc = nullptr,
              UStringOrID windowName = nullptr, DWORD dwStyle = 0,
              DWORD dwExStyle = 0, UMenuOrID menu = nullptr,
              LPVOID lpCreateParam = nullptr) {
    return TBase::Create(GetWndClassName(), hwndParent, rc.Get(),
                         windowName.Get(), dwStyle, dwExStyle, menu.Get(),
                         lpCreateParam);
  }

  static LPCSTR GetWndClassName() { return TEXT("EDIT"); }


  DWORD GetStyle() const {
    WINASSERT(TBase::m_hwnd);
    return (DWORD)::GetWindowLong(TBase::m_hwnd, GWL_STYLE);
  }

  BOOL CanUndo() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (BOOL)::SendMessage(TBase::m_hwnd, EM_CANUNDO, 0, 0L);
  }

  int GetLineCount() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, EM_GETLINECOUNT, 0, 0L);
  }

  BOOL GetModify() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (BOOL)::SendMessage(TBase::m_hwnd, EM_GETMODIFY, 0, 0L);
  }

  void SetModify(BOOL bModified = TRUE) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, EM_SETMODIFY, bModified, 0L);
  }

  void GetRect(LPRECT lpRect) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, EM_GETRECT, 0, (LPARAM)lpRect);
  }

  DWORD GetSel() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (DWORD)::SendMessage(TBase::m_hwnd, EM_GETSEL, 0, 0L);
  }

  void GetSel(int &nStartChar, int &nEndChar) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, EM_GETSEL, (WPARAM)&nStartChar,
                  (LPARAM)&nEndChar);
  }

  HLOCAL GetHandle() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (HLOCAL)::SendMessage(TBase::m_hwnd, EM_GETHANDLE, 0, 0L);
  }

  void SetHandle(HLOCAL hBuffer) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, EM_SETHANDLE, (WPARAM)hBuffer, 0L);
  }

  DWORD GetMargins() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (DWORD)::SendMessage(TBase::m_hwnd, EM_GETMARGINS, 0, 0L);
  }

  void GetMargins(UINT &nLeft, UINT &nRight) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    DWORD dwRet =
        (DWORD)::SendMessage(TBase::m_hwnd, EM_GETMARGINS, 0, 0L);
    nLeft = LOWORD(dwRet);
    nRight = HIWORD(dwRet);
  }

  void SetMargins(UINT nLeft, UINT nRight,
                  WORD wFlags = EC_LEFTMARGIN | EC_RIGHTMARGIN) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, EM_SETMARGINS, wFlags,
                  MAKELONG(nLeft, nRight));
  }

  UINT GetLimitText() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (UINT)::SendMessage(TBase::m_hwnd, EM_GETLIMITTEXT, 0, 0L);
  }

  void SetLimitText(UINT nMax) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, EM_SETLIMITTEXT, nMax, 0L);
  }

  POINT PosFromChar(UINT nChar) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    DWORD dwRet =
        (DWORD)::SendMessage(TBase::m_hwnd, EM_POSFROMCHAR, nChar, 0);
    POINT point = {GET_X_LPARAM(dwRet), GET_Y_LPARAM(dwRet)};
    return point;
  }

  int CharFromPos(POINT pt, int *pLine = NULL) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    DWORD dwRet = (DWORD)::SendMessage(TBase::m_hwnd, EM_CHARFROMPOS,
                                       0, MAKELPARAM(pt.x, pt.y));
    if (pLine != NULL)
      *pLine = (int)(short) HIWORD(dwRet);
    return (int)(short) LOWORD(dwRet);
  }

  int GetLine(int nIndex, LPTSTR lpszBuffer) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, EM_GETLINE, nIndex,
                              (LPARAM)lpszBuffer);
  }

  int GetLine(int nIndex, LPTSTR lpszBuffer, int nMaxLength) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    *(LPWORD)lpszBuffer = (WORD)nMaxLength;
    return (int)::SendMessage(TBase::m_hwnd, EM_GETLINE, nIndex,
                              (LPARAM)lpszBuffer);
  }

  TCHAR GetPasswordChar() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (TCHAR)::SendMessage(TBase::m_hwnd, EM_GETPASSWORDCHAR, 0,
                                0L);
  }

  void SetPasswordChar(TCHAR ch) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, EM_SETPASSWORDCHAR, ch, 0L);
  }

  EDITWORDBREAKPROC GetWordBreakProc() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (EDITWORDBREAKPROC)::SendMessage(TBase::m_hwnd,
                                            EM_GETWORDBREAKPROC, 0, 0L);
  }

  void SetWordBreakProc(EDITWORDBREAKPROC ewbprc) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, EM_SETWORDBREAKPROC, 0,
                  (LPARAM)ewbprc);
  }

  int GetFirstVisibleLine() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, EM_GETFIRSTVISIBLELINE,
                              0, 0L);
  }

  int GetThumb() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    WINASSERT((GetStyle() & ES_MULTILINE) != 0);
    return (int)::SendMessage(TBase::m_hwnd, EM_GETTHUMB, 0, 0L);
  }

  BOOL SetReadOnly(BOOL bReadOnly = TRUE) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (BOOL)::SendMessage(TBase::m_hwnd, EM_SETREADONLY,
                               bReadOnly, 0L);
  }
#if (WINVER >= 0x0500)
  UINT GetImeStatus(UINT uStatus) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (UINT)::SendMessage(TBase::m_hwnd, EM_GETIMESTATUS,
                               uStatus, 0L);
  }

  UINT SetImeStatus(UINT uStatus, UINT uData) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (UINT)::SendMessage(TBase::m_hwnd, EM_SETIMESTATUS,
                               uStatus, uData);
  }
#endif
#if (_WIN32_WINNT >= 0x0501)
  BOOL GetCueBannerText(LPCWSTR lpstrText, int cchText) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (BOOL)::SendMessage(TBase::m_hwnd, EM_GETCUEBANNER,
                               (WPARAM)lpstrText, cchText);
  }

  BOOL SetCueBannerText(LPCWSTR lpstrText, BOOL bKeepWithFocus = FALSE) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (BOOL)::SendMessage(TBase::m_hwnd, EM_SETCUEBANNER,
                               (WPARAM)bKeepWithFocus, (LPARAM)(lpstrText));
  }
#endif
  void EmptyUndoBuffer() {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, EM_EMPTYUNDOBUFFER, 0, 0L);
  }

  BOOL FmtLines(BOOL bAddEOL) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (BOOL)::SendMessage(TBase::m_hwnd, EM_FMTLINES, bAddEOL,
                               0L);
  }

  void LimitText(int nChars = 0) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, EM_LIMITTEXT, nChars, 0L);
  }

  int LineFromChar(int nIndex = -1) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, EM_LINEFROMCHAR, nIndex,
                              0L);
  }

  int LineIndex(int nLine = -1) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, EM_LINEINDEX, nLine, 0L);
  }

  int LineLength(int nLine = -1) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (int)::SendMessage(TBase::m_hwnd, EM_LINELENGTH, nLine,
                              0L);
  }

  void LineScroll(int nLines, int nChars = 0) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, EM_LINESCROLL, nChars, nLines);
  }

  void ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo = FALSE) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, EM_REPLACESEL, (WPARAM)bCanUndo,
                  (LPARAM)lpszNewText);
  }

  void SetRect(LPCRECT lpRect) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, EM_SETRECT, 0, (LPARAM)lpRect);
  }

  void SetRectNP(LPCRECT lpRect) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, EM_SETRECTNP, 0, (LPARAM)lpRect);
  }

  void SetSel(DWORD dwSelection, BOOL bNoScroll = FALSE) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, EM_SETSEL, LOWORD(dwSelection),
                  HIWORD(dwSelection));
    if (!bNoScroll)
      ::SendMessage(TBase::m_hwnd, EM_SCROLLCARET, 0, 0L);
  }

  void SetSel(int nStartChar, int nEndChar, BOOL bNoScroll = FALSE) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, EM_SETSEL, nStartChar, nEndChar);
    if (!bNoScroll)
      ::SendMessage(TBase::m_hwnd, EM_SCROLLCARET, 0, 0L);
  }

  void SetSelAll(BOOL bNoScroll = FALSE) { SetSel(0, -1, bNoScroll); }

  void SetSelNone(BOOL bNoScroll = FALSE) { SetSel(-1, 0, bNoScroll); }

  BOOL SetTabStops(int nTabStops, LPINT rgTabStops) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (BOOL)::SendMessage(TBase::m_hwnd, EM_SETTABSTOPS,
                               nTabStops, (LPARAM)rgTabStops);
  }

  BOOL SetTabStops() {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (BOOL)::SendMessage(TBase::m_hwnd, EM_SETTABSTOPS, 0, 0L);
  }

  BOOL SetTabStops(const int &cxEachStop) // takes an 'int'
  {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (BOOL)::SendMessage(TBase::m_hwnd, EM_SETTABSTOPS, 1,
                               (LPARAM)(LPINT) & cxEachStop);
  }

  void ScrollCaret() {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, EM_SCROLLCARET, 0, 0L);
  }

  int Scroll(int nScrollAction) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    WINASSERT((GetStyle() & ES_MULTILINE) != 0);
    LRESULT lRet =
        ::SendMessage(TBase::m_hwnd, EM_SCROLL, nScrollAction, 0L);
    if (!(BOOL)HIWORD(lRet))
      return -1; // failed
    return (int)(short) LOWORD(lRet);
  }

  void InsertText(int nInsertAfterChar, LPCTSTR lpstrText,
                  BOOL bNoScroll = FALSE, BOOL bCanUndo = FALSE) {
    SetSel(nInsertAfterChar, nInsertAfterChar, bNoScroll);
    ReplaceSel(lpstrText, bCanUndo);
  }

  void AppendText(LPCTSTR lpstrText, BOOL bNoScroll = FALSE,
                  BOOL bCanUndo = FALSE) {
    InsertText(this->GetWindowTextLength(), lpstrText, bNoScroll, bCanUndo);
  }

#if (_WIN32_WINNT >= 0x0501)
  BOOL ShowBalloonTip(PEDITBALLOONTIP pEditBaloonTip) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (BOOL)::SendMessage(TBase::m_hwnd, EM_SHOWBALLOONTIP, 0,
                               (LPARAM)pEditBaloonTip);
  }

  BOOL HideBalloonTip() {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (BOOL)::SendMessage(TBase::m_hwnd, EM_HIDEBALLOONTIP, 0,
                               0L);
  }
#endif

#if (_WIN32_WINNT >= 0x0600)
  DWORD GetHilite() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (DWORD)::SendMessage(TBase::m_hwnd, EM_GETHILITE, 0, 0L);
  }

  void GetHilite(int &nStartChar, int &nEndChar) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    DWORD dwRet =
        (DWORD)::SendMessage(TBase::m_hwnd, EM_GETHILITE, 0, 0L);
    nStartChar = (int)(short) LOWORD(dwRet);
    nEndChar = (int)(short) HIWORD(dwRet);
  }

  void SetHilite(int nStartChar, int nEndChar) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, EM_SETHILITE, nStartChar, nEndChar);
  }
#endif

  BOOL Undo() {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return (BOOL)::SendMessage(TBase::m_hwnd, EM_UNDO, 0, 0L);
  }

  void Clear() {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, WM_CLEAR, 0, 0L);
  }

  void Copy() {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, WM_COPY, 0, 0L);
  }

  void Cut() {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, WM_CUT, 0, 0L);
  }

  void Paste() {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SendMessage(TBase::m_hwnd, WM_PASTE, 0, 0L);
  }
};

typedef EditT<Window> Edit;


template <typename TBase> class ScrollBarT : public TBase {

  ScrollBarT(HWND hWnd = NULL) : TBase(hWnd) {}
  ScrollBarT<TBase> &operator=(HWND hWnd) {
    TBase::m_hwnd = hWnd;
    return *this;
  }
  HWND Create(HWND hwndParent, URECT rc = nullptr,
              UStringOrID windowName = nullptr, DWORD dwStyle = 0,
              DWORD dwExStyle = 0, UMenuOrID menu = nullptr,
              LPVOID lpCreateParam = nullptr) {
    return TBase::Create(GetWndClassName(), hwndParent, rc.Get(),
                         windowName.Get(), dwStyle, dwExStyle, menu.Get(),
                         lpCreateParam);
  }

  static LPCTSTR GetWndClassName() { return TEXT("SCROLLBAR"); }

  int GetScrollPos() const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return ::GetScrollPos(TBase::m_hwnd, SB_CTL);
  }

  int SetScrollPos(int nPos, BOOL bRedraw = TRUE) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return ::SetScrollPos(TBase::m_hwnd, SB_CTL, nPos, bRedraw);
  }

  void GetScrollRange(LPINT lpMinPos, LPINT lpMaxPos) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::GetScrollRange(TBase::m_hwnd, SB_CTL, lpMinPos, lpMaxPos);
  }

  void SetScrollRange(int nMinPos, int nMaxPos, BOOL bRedraw = TRUE) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::SetScrollRange(TBase::m_hwnd, SB_CTL, nMinPos, nMaxPos,
                     bRedraw);
  }

  BOOL GetScrollInfo(LPSCROLLINFO lpScrollInfo) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return ::GetScrollInfo(TBase::m_hwnd, SB_CTL, lpScrollInfo);
  }

  int SetScrollInfo(LPSCROLLINFO lpScrollInfo, BOOL bRedraw = TRUE) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return ::SetScrollInfo(TBase::m_hwnd, SB_CTL, lpScrollInfo,
                           bRedraw);
  }
  int GetScrollLimit() const {
    int nMin = 0, nMax = 0;
    ::GetScrollRange(TBase::m_hwnd, SB_CTL, &nMin, &nMax);
    SCROLLINFO info = {sizeof(SCROLLINFO), SIF_PAGE};
    if (::GetScrollInfo(TBase::m_hwnd, SB_CTL, &info))
      nMax -= ((info.nPage - 1) > 0) ? (info.nPage - 1) : 0;

    return nMax;
  }

#if (WINVER >= 0x0500)
  BOOL GetScrollBarInfo(PSCROLLBARINFO pScrollBarInfo) const {
    WINASSERT(::IsWindow(TBase::m_hwnd));
#if (_WIN32_WINNT >= 0x0501)
    return (BOOL)::SendMessage(TBase::m_hwnd, SBM_GETSCROLLBARINFO, 0,
                               (LPARAM)pScrollBarInfo);
#else
    return ::GetScrollBarInfo(TBase::m_hwnd, OBJID_CLIENT,
                              pScrollBarInfo);
#endif
  }
#endif
  void ShowScrollBar(BOOL bShow = TRUE) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    ::ShowScrollBar(TBase::m_hwnd, SB_CTL, bShow);
  }

  BOOL EnableScrollBar(UINT nArrowFlags = ESB_ENABLE_BOTH) {
    WINASSERT(::IsWindow(TBase::m_hwnd));
    return ::EnableScrollBar(TBase::m_hwnd, SB_CTL, nArrowFlags);
  }
};

typedef ScrollBarT<Window> ScrollBar;
//Standard-Controls

enum class LayoutStyle : int {
    Left,
    Right,
    Bottom,
    Top,
    Center,
    BottomRight,
    BottomLeft,
    TopRight,
    TopLeft
};

class LayoutManager {
public:
    struct LayoutEntry {
        HWND win;
        Rect rc;
        LayoutStyle style;
    };
    LayoutEntry m_main;
    std::vector<LayoutEntry> m_entries;

    LayoutManager() {}

    void SetMain(HWND hwnd,Rect& rc){
        m_main.rc.CopyRect(rc);
        m_main.win = Window(hwnd);
        m_main.style = LayoutStyle::Center;
    }

    void AddEntry(HWND hwnd,Rect& rc, LayoutStyle style){
        m_entries.emplace_back(LayoutEntry{Window(hwnd),rc,style});
    }

    void UpdateLayout(Rect& rc)
    {
        auto def = ::BeginDeferWindowPos(m_entries.size());
        for(auto& e : m_entries){
           switch(e.style)
           {
           case LayoutStyle::Left:
              {
               int bottomOffset = m_main.rc.Height() -  e.rc.Height();
               def = ::DeferWindowPos(def,e.win,nullptr,e.rc.left,e.rc.top,
                                e.rc.Width(),
                                (int)rc.Height() - bottomOffset,
                                SWP_NOZORDER | SWP_SHOWWINDOW);
               if(def == nullptr) return;
           continue;
               }
           case LayoutStyle::Top:
             {
               float wratio = (float)m_main.rc.Width() / e.rc.Width();
               float hratio = (float)m_main.rc.Height() /  e.rc.Height();
               def = ::DeferWindowPos(def,e.win,nullptr,e.rc.left,e.rc.top,
                                (int)(rc.Width() * wratio),
                                (int)(rc.Height() * hratio),
                                SWP_NOZORDER | SWP_SHOWWINDOW);
               if(def == nullptr) return;
               continue;
            }
           case LayoutStyle::Right:
            {
               continue;
            }
           case LayoutStyle::Bottom:
            {
               continue;
            }
           case LayoutStyle::Center:
           {

               int xoffset = e.rc.left;
               int rightoffset = m_main.rc.right - e.rc.right;
               int newWidth = rc.right - rightoffset - xoffset;
               int yoffset = m_main.rc.bottom - e.rc.bottom;
               int newHeight = rc.bottom - yoffset - e.rc.top;
               def = ::DeferWindowPos(def,e.win,nullptr,e.rc.left,
                                                        e.rc.top,newWidth,newHeight,
                                      SWP_NOZORDER | SWP_SHOWWINDOW);
              if(def) continue;
              else break;
              continue;
           }
           case LayoutStyle::TopLeft:
           {

               int xoffset = m_main.rc.left - e.rc.left;
               int yoffset = m_main.rc.top - e.rc.top;

               def = ::DeferWindowPos(def,e.win,nullptr,rc.left + xoffset,
                                                        rc.top + yoffset,e.rc.Width(),e.rc.Height(),
                                      SWP_NOZORDER | SWP_SHOWWINDOW);
              if(def) continue;
              else break;
           }
           case LayoutStyle::TopRight:
           {
               int height = e.rc.Height() ;
               int xoffset = m_main.rc.right - e.rc.right;
               int yoffset = m_main.rc.top - e.rc.top;

               def = ::DeferWindowPos(def,e.win,nullptr,rc.right - xoffset - e.rc.Width(),
                                                        rc.bottom - yoffset - height,e.rc.Width(),height,
                                      SWP_NOZORDER | SWP_SHOWWINDOW);
              if(def) continue;
              else break;
              continue;
           }
           case LayoutStyle::BottomLeft:
           {
               int height = e.rc.Height() ;
               int xoffset = m_main.rc.left - e.rc.left;
               int yoffset = m_main.rc.bottom - e.rc.bottom;

               def = ::DeferWindowPos(def,e.win,nullptr,rc.left + xoffset,
                                                        rc.bottom - yoffset - height,e.rc.Width(),height,
                                      SWP_NOZORDER | SWP_SHOWWINDOW);
              if(def) continue;
              else break;
           }
           case LayoutStyle::BottomRight:
           {
              int width = e.rc.Width() ;
              int height = e.rc.Height() ;
              int xoffset = m_main.rc.right - e.rc.right;
              int yoffset = m_main.rc.bottom - e.rc.bottom;

              def = ::DeferWindowPos(def,e.win,nullptr,rc.right - width - xoffset,
                                                       rc.bottom - height - yoffset,width,height,
                                     SWP_NOZORDER | SWP_SHOWWINDOW);
              if(def) continue;
              else break;
           }
           }
           break;
        }
        ::EndDeferWindowPos(def);

    }
};

class VerticalStackedLayout {
std::vector<HWND> m_entries;
public:
    void AddEntry(HWND hwnd){
        m_entries.emplace_back(hwnd);
    }
    void UpdateLayout(Rect& rc){
        int NewHeight = rc.Height() / m_entries.size();
        int remainder = rc.Height() % m_entries.size();
        int curry = 0;
        auto def = ::BeginDeferWindowPos(m_entries.size());
        if(!def) return;
        for(int i = 0; i < m_entries.size(); i++){
            def = ::DeferWindowPos(def,m_entries[i],nullptr,0,curry + (i == 0? remainder : 0),rc.Width(),NewHeight,SWP_NOZORDER | SWP_SHOWWINDOW);
            curry += NewHeight;
            if(def) continue;
            else return;
        }
        ::EndDeferWindowPos(def);
    }
};

class HorizontalStackedLayout{
    std::vector<HWND> m_entries;
public:
    void AddEntry(HWND hwnd){
        m_entries.emplace_back(hwnd);
    }

    void UpdateLayout(Rect& rc){
        int newWidth = rc.Width() / m_entries.size();
        int currx  = 0;
        auto def = ::BeginDeferWindowPos(m_entries.size());
        for(int i = 0; i <  m_entries.size(); i++){
            def = ::DeferWindowPos(def,m_entries[i],nullptr,currx,0,newWidth,rc.Height(),SWP_NOZORDER | SWP_SHOWWINDOW);
        }
        ::EndDeferWindowPos(def);
    }
};

#define BEGIN_MSG_MAP()                                                        \
  BOOL bHandled = FALSE;                                                       \
  void SetHandled(BOOL Handled = TRUE) { bHandled = Handled; }                 \
                                                                               \
  BOOL HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,       \
                     LRESULT &lResult, DWORD dwMsgID = 0) override {           \
                                                                               \
    (m_hwnd);                                                                  \
    (uMsg);                                                                    \
    (wParam);                                                                  \
    (lParam);                                                                  \
    (lResult);                                                                 \
    (bHandled);                                                                \
    switch (dwMsgID) {                                                         \
    case 0:

#define END_MSG_MAP()                                                          \
  break;                                                                       \
  }                                                                            \
  return FALSE;                                                                \
  }

#define ALT_MSG_MAP(msgMapID)                                                  \
  break;                                                                       \
  case msgMapId:

// LRESULT OnHandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,BOOL& handled)
#define MESSAGE_HANDLER(msg, func)                                             \
  if (uMsg == msg) {                                                           \
    bHandled = TRUE;                                                           \
    lResult = func(uMsg, wParam, lParam, bHandled);                            \
    if(bHandled) return bHandled;                                                           \
  }

#ifndef COMMAND_HANDLERS
// void OnCommandHandlerEX(UINT uNotifyCode, int nID, Window wndCtl)
#define COMMAND_HANDLER_EX(id, code, func)                                     \
  if (uMsg == WM_COMMAND && code == HIWORD(wParam) && id == LOWORD(wParam)) {  \
    SetHandled();                                                              \
    func((UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam);             \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnCommandIDHandlerEX(UINT uNotifyCode, int nID, Window wndCtl)
#define COMMAND_ID_HANDLER_EX(id, func)                                        \
  if (uMsg == WM_COMMAND && id == LOWORD(wParam)) {                            \
    SetHandled();                                                              \
    func((UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam);             \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnCommandCodeHandlerEX(UINT uNotifyCode, int nID, Window wndCtl)
#define COMMAND_CODE_HANDLER_EX(code, func)                                    \
  if (uMsg == WM_COMMAND && code == HIWORD(wParam)) {                          \
    SetHandled();                                                              \
    func((UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam);             \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnCommandRangeHandlerEX(UINT uNotifyCode, int nID, Window wndCtl)
#define COMMAND_RANGE_HANDLER_EX(idFirst, idLast, func)                        \
  if (uMsg == WM_COMMAND && LOWORD(wParam) >= idFirst &&                       \
      LOWORD(wParam) <= idLast) {                                              \
    SetHandled();                                                              \
    func((UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam);             \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnCommandRangeCodeHandlerEX(UINT uNotifyCode, int nID, Window wndCtl)
#define COMMAND_RANGE_CODE_HANDLER_EX(idFirst, idLast, code, func)             \
  if (uMsg == WM_COMMAND && code == HIWORD(wParam) &&                          \
      LOWORD(wParam) >= idFirst && LOWORD(wParam) <= idLast) {                 \
    SetHandled();                                                              \
    func((UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam);             \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// LRESULT OnReflectedCommandHandlerEX(UINT uNotifyCode, int nID, Window wndCtl)
#define REFLECTED_COMMAND_HANDLER_EX(id, code, func)                           \
  if (uMsg == OCM_COMMAND && code == HIWORD(wParam) && id == LOWORD(wParam)) { \
    SetHandled();                                                              \
    func((UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam);             \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// LRESULT OnReflectedCommandIDHandlerEX(UINT uNotifyCode, int nID, Window
// wndCtl)
#define REFLECTED_COMMAND_ID_HANDLER_EX(id, func)                              \
  if (uMsg == OCM_COMMAND && id == LOWORD(wParam)) {                           \
    SetHandled();                                                              \
    func((UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam);             \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// LRESULT OnReflectedCommandCodeHandlerEX(UINT uNotifyCode, int nID, Window
// wndCtl)
#define REFLECTED_COMMAND_CODE_HANDLER_EX(code, func)                          \
  if (uMsg == OCM_COMMAND && code == HIWORD(wParam)) {                         \
    SetHandled();                                                              \
    func((UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam);             \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnReflectedCommandRangeHandlerEX(UINT uNotifyCode, int nID, Window
// wndCtl)
#define REFLECTED_COMMAND_RANGE_HANDLER_EX(idFirst, idLast, func)              \
  if (uMsg == OCM_COMMAND && LOWORD(wParam) >= idFirst &&                      \
      LOWORD(wParam) <= idLast) {                                              \
    SetHandled();                                                              \
    func((UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam);             \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnReflectedCommandRangeCodeHandlerEX(UINT uNotifyCode, int nID, Window
// wndCtl)
#define REFLECTED_COMMAND_RANGE_CODE_HANDLER_EX(idFirst, idLast, code, func)   \
  if (uMsg == OCM_COMMAND && code == HIWORD(wParam) &&                         \
      LOWORD(wParam) >= idFirst && LOWORD(wParam) <= idLast) {                 \
    SetHandled();                                                              \
    func((UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam);             \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }
#endif

#ifndef NOTIFY_HANDLERS

// LRESULT OnNotifyHandlerEX(LPNMHDR pnmh)
#define NOTIFY_HANDLER_EX(id, cd, func)                                        \
  if (uMsg == WM_NOTIFY && cd == ((LPNMHDR)lParam)->code &&                    \
      id == ((LPNMHDR)lParam)->idFrom) {                                       \
    SetHandled();                                                              \
    lResult = func((LPNMHDR)lParam);                                           \
    if(bHandled) return bHandled;                                                           \
  }

// LRESULT OnNotifyIDHandlerEX(LPNMHDR pnmh)
#define NOTIFY_ID_HANDLER_EX(id, func)                                         \
  if (uMsg == WM_NOTIFY && id == ((LPNMHDR)lParam)->idFrom) {                  \
    SetHandled();                                                              \
    lResult = func((LPNMHDR)lParam);                                           \
    if(bHandled) return bHandled;                                                           \
  }

// LRESULT OnNotifyCodeHandlerEX(LPNMHDR pnmh)
#define NOTIFY_CODE_HANDLER_EX(cd, func)                                       \
  if (uMsg == WM_NOTIFY && cd == ((LPNMHDR)lParam)->code) {                    \
    SetHandled();                                                              \
    lResult = func((LPNMHDR)lParam);                                           \
    if(bHandled) return bHandled;                                                           \
  }

// LRESULT OnNotifyRangeHandlerEX(LPNMHDR pnmh)
#define NOTIFY_RANGE_HANDLER_EX(idFirst, idLast, func)                         \
  if (uMsg == WM_NOTIFY && ((LPNMHDR)lParam)->idFrom >= idFirst &&             \
      ((LPNMHDR)lParam)->idFrom <= idLast) {                                   \
    SetHandled();                                                              \
    lResult = func((LPNMHDR)lParam);                                           \
    if(bHandled) return bHandled;                                                           \
  }

// LRESULT OnNotifyRangeCodeHandlerEX(LPNMHDR pnmh)
#define NOTIFY_RANGE_CODE_HANDLER_EX(idFirst, idLast, cd, func)                \
  if (uMsg == WM_NOTIFY && cd == ((LPNMHDR)lParam)->code &&                    \
      ((LPNMHDR)lParam)->idFrom >= idFirst &&                                  \
      ((LPNMHDR)lParam)->idFrom <= idLast) {                                   \
    SetHandled();                                                              \
    lResult = func((LPNMHDR)lParam);                                           \
    if(bHandled) return bHandled;                                                           \
  }

// LRESULT OnReflectedNotifyHandlerEX(LPNMHDR pnmh)
#define REFLECTED_NOTIFY_HANDLER_EX(id, cd, func)                              \
  if (uMsg == OCM_NOTIFY && cd == ((LPNMHDR)lParam)->code &&                   \
      id == ((LPNMHDR)lParam)->idFrom) {                                       \
    SetHandled();                                                              \
    lResult = func((LPNMHDR)lParam);                                           \
    if(bHandled) return bHandled;                                                           \
  }

// LRESULT OnReflectedNotifyIDHandlerEX(LPNMHDR pnmh)
#define REFLECTED_NOTIFY_ID_HANDLER_EX(id, func)                               \
  if (uMsg == OCM_NOTIFY && id == ((LPNMHDR)lParam)->idFrom) {                 \
    SetHandled();                                                              \
    lResult = func((LPNMHDR)lParam);                                           \
    if(bHandled) return bHandled;                                                           \
  }

// LRESULT OnReflectedNotifyCodeHandlerEX(LPNMHDR pnmh)
#define REFLECTED_NOTIFY_CODE_HANDLER_EX(cd, func)                             \
  if (uMsg == OCM_NOTIFY && cd == ((LPNMHDR)lParam)->code) {                   \
    SetHandled();                                                              \
    lResult = func((LPNMHDR)lParam);                                           \
    if(bHandled) return bHandled;                                                           \
  }

// LRESULT OnReflectedNotifyRangeHandlerEX(LPNMHDR pnmh)
#define REFLECTED_NOTIFY_RANGE_HANDLER_EX(idFirst, idLast, func)               \
  if (uMsg == OCM_NOTIFY && ((LPNMHDR)lParam)->idFrom >= idFirst &&            \
      ((LPNMHDR)lParam)->idFrom <= idLast) {                                   \
    SetHandled();                                                              \
    lResult = func((LPNMHDR)lParam);                                           \
    if(bHandled) return bHandled;                                                           \
  }

// LRESULT OnReflectedNotifyRangeCodeHandlerEX(LPNMHDR pnmh)
#define REFLECTED_NOTIFY_RANGE_CODE_HANDLER_EX(idFirst, idLast, cd, func)      \
  if (uMsg == OCM_NOTIFY && cd == ((LPNMHDR)lParam)->code &&                   \
      ((LPNMHDR)lParam)->idFrom >= idFirst &&                                  \
      ((LPNMHDR)lParam)->idFrom <= idLast) {                                   \
    SetHandled();                                                              \
    lResult = func((LPNMHDR)lParam);                                           \
    if(bHandled) return bHandled;                                                           \
  }

#define FORWARD_NOTIFICATIONS()                                                \
  {                                                                            \
    bHandled = TRUE;                                                           \
    lResult = ForwardNotifications(uMsg, wParam, lParam, bHandled);            \
    if(bHandled) return bHandled;                                                           \
  }

#define REFLECT_NOTIFICATIONS()                                                \
  {                                                                            \
    bHandled = TRUE;                                                           \
    lResult = ReflectNotifications(uMsg, wParam, lParam, bHandled);            \
    if(bHandled) return bHandled;                                                           \
  }

#endif

#ifndef CHAIN_HANDLERS

#define CHAIN_MSG_MAP(theChainClass)                                           \
  {                                                                            \
    if (theChainClass::HandleMessage(hWnd, uMsg, wParam, lParam, lResult))     \
      return TRUE;                                                             \
  }

#define CHAIN_MSG_MAP_MEMBER(theChainMember)                                   \
  {                                                                            \
    if (theChainMember.HandleMessage(hWnd, uMsg, wParam, lParam, lResult))     \
      return TRUE;                                                             \
  }

#define CHAIN_MSG_MAP_ALT(theChainClass, msgMapID)                             \
  {                                                                            \
    if (theChainClass::HandleMessage(hWnd, uMsg, wParam, lParam, lResult,      \
                                     msgMapID))                                \
      return TRUE;                                                             \
  }

#define CHAIN_MSG_MAP_ALT_MEMBER(theChainMember, msgMapID)                     \
  {                                                                            \
    if (theChainMember.HandleMessage(hWnd, uMsg, wParam, lParam, lResult,      \
                                     msgMapID))                                \
      return TRUE;                                                             \
  }
#endif

#if (_WIN32_WINNT >= 0x0400)

// int OnCreate(LPCREATESTRUCT lpCreateStruct)
#define MSG_WM_CREATE(func)                                                    \
  if (uMsg == WM_CREATE) {                                                     \
    SetHandled();                                                              \
    lResult = (LRESULT)func((LPCREATESTRUCT)lParam);                           \
    if(bHandled) return bHandled;                                                           \
  }

// BOOL OnInitDialog(Window wndFocus, LPARAM lInitParam)
#define MSG_WM_INITDIALOG(func)                                                \
  if (uMsg == WM_INITDIALOG) {                                                 \
    SetHandled();                                                              \
    lResult = (LRESULT)func((HWND)wParam, lParam);                             \
    if(bHandled) return bHandled;                                                           \
  }

// BOOL OnCopyData(Window wnd, PCOPYDATASTRUCT pCopyDataStruct)
#define MSG_WM_COPYDATA(func)                                                  \
  if (uMsg == WM_COPYDATA) {                                                   \
    SetHandled();                                                              \
    lResult = (LRESULT)func((HWND)wParam, (PCOPYDATASTRUCT)lParam);            \
    if(bHandled) return bHandled;                                                           \
  }

// void OnDestroy()
#define MSG_WM_DESTROY(func)                                                   \
  if (uMsg == WM_DESTROY) {                                                    \
    SetHandled();                                                              \
    func();                                                                    \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnMove(Point ptPos)
#define MSG_WM_MOVE(func)                                                      \
  if (uMsg == WM_MOVE) {                                                       \
    SetHandled();                                                              \
    func(Point(lParam));                                                       \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnSize(UINT nType, Size size)
#define MSG_WM_SIZE(func)                                                      \
  if (uMsg == WM_SIZE) {                                                       \
    SetHandled();                                                              \
    func((UINT)wParam, Size(lParam));                                          \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnActivate(UINT nState, BOOL bMinimized, Window wndOther)
#define MSG_WM_ACTIVATE(func)                                                  \
  if (uMsg == WM_ACTIVATE) {                                                   \
    SetHandled();                                                              \
    func((UINT)LOWORD(wParam), (BOOL)HIWORD(wParam), (HWND)lParam);            \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnSetFocus(Window wndOld)
#define MSG_WM_SETFOCUS(func)                                                  \
  if (uMsg == WM_SETFOCUS) {                                                   \
    SetHandled();                                                              \
    func((HWND)wParam);                                                        \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnKillFocus(Window wndFocus)
#define MSG_WM_KILLFOCUS(func)                                                 \
  if (uMsg == WM_KILLFOCUS) {                                                  \
    SetHandled();                                                              \
    func((HWND)wParam);                                                        \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnEnable(BOOL bEnable)
#define MSG_WM_ENABLE(func)                                                    \
  if (uMsg == WM_ENABLE) {                                                     \
    SetHandled();                                                              \
    func((BOOL)wParam);                                                        \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnPaint(DC dc)
#define MSG_WM_PAINT(func)                                                     \
  if (uMsg == WM_PAINT) {                                                      \
    SetHandled();                                                              \
    func((HDC)wParam);                                                         \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnClose()
#define MSG_WM_CLOSE(func)                                                     \
  if (uMsg == WM_CLOSE) {                                                      \
    SetHandled();                                                              \
    func();                                                                    \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// BOOL OnQueryEndSession(UINT nSource, UINT uLogOff)
#define MSG_WM_QUERYENDSESSION(func)                                           \
  if (uMsg == WM_QUERYENDSESSION) {                                            \
    SetHandled();                                                              \
    lResult = (LRESULT)func((UINT)wParam, (UINT)lParam);                       \
    if(bHandled) return bHandled;                                                           \
  }

// BOOL OnQueryOpen()
#define MSG_WM_QUERYOPEN(func)                                                 \
  if (uMsg == WM_QUERYOPEN) {                                                  \
    SetHandled();                                                              \
    lResult = (LRESULT)func();                                                 \
    if(bHandled) return bHandled;                                                           \
  }

// BOOL OnEraseBkgnd(DCT<true> dc)
#define MSG_WM_ERASEBKGND(func)                                                \
  if (uMsg == WM_ERASEBKGND) {                                                 \
    SetHandled();                                                              \
    lResult = (LRESULT)func((HDC)wParam);                                      \
    if(bHandled) return bHandled;                                                           \
  }

// void OnSysColorChange()
#define MSG_WM_SYSCOLORCHANGE(func)                                            \
  if (uMsg == WM_SYSCOLORCHANGE) {                                             \
    SetHandled();                                                              \
    func();                                                                    \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnEndSession(BOOL bEnding, UINT uLogOff)
#define MSG_WM_ENDSESSION(func)                                                \
  if (uMsg == WM_ENDSESSION) {                                                 \
    SetHandled();                                                              \
    func((BOOL)wParam, (UINT)lParam);                                          \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnShowWindow(BOOL bShow, UINT nStatus)
#define MSG_WM_SHOWWINDOW(func)                                                \
  if (uMsg == WM_SHOWWINDOW) {                                                 \
    SetHandled();                                                              \
    func((BOOL)wParam, (int)lParam);                                           \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// HBRUSH OnCtlColorEdit(DCT<true> dc, CEdit edit)
#define MSG_WM_CTLCOLOREDIT(func)                                              \
  if (uMsg == WM_CTLCOLOREDIT) {                                               \
    SetHandled();                                                              \
    lResult = (LRESULT)func((HDC)wParam, (HWND)lParam);                        \
    if(bHandled) return bHandled;                                                           \
  }

// HBRUSH OnCtlColorListBox(DCT<true> dc, CListBox listBox)
#define MSG_WM_CTLCOLORLISTBOX(func)                                           \
  if (uMsg == WM_CTLCOLORLISTBOX) {                                            \
    SetHandled();                                                              \
    lResult = (LRESULT)func((HDC)wParam, (HWND)lParam);                        \
    if(bHandled) return bHandled;                                                           \
  }

// HBRUSH OnCtlColorBtn(DCT<true> dc, CButton button)
#define MSG_WM_CTLCOLORBTN(func)                                               \
  if (uMsg == WM_CTLCOLORBTN) {                                                \
    SetHandled();                                                              \
    lResult = (LRESULT)func((HDC)wParam, (HWND)lParam);                        \
    if(bHandled) return bHandled;                                                           \
  }

// HBRUSH OnCtlColorDlg(DCT<true> dc, Window wnd)
#define MSG_WM_CTLCOLORDLG(func)                                               \
  if (uMsg == WM_CTLCOLORDLG) {                                                \
    SetHandled();                                                              \
    lResult = (LRESULT)func((HDC)wParam, (HWND)lParam);                        \
    if(bHandled) return bHandled;                                                           \
  }

// HBRUSH OnCtlColorScrollBar(DCT<true> dc, CScrollBar scrollBar)
#define MSG_WM_CTLCOLORSCROLLBAR(func)                                         \
  if (uMsg == WM_CTLCOLORSCROLLBAR) {                                          \
    SetHandled();                                                              \
    lResult = (LRESULT)func((HDC)wParam, (HWND)lParam);                        \
    if(bHandled) return bHandled;                                                           \
  }

// HBRUSH OnCtlColorStatic(DCT<true> dc, CStatic wndStatic)
#define MSG_WM_CTLCOLORSTATIC(func)                                            \
  if (uMsg == WM_CTLCOLORSTATIC) {                                             \
    SetHandled();                                                              \
    lResult = (LRESULT)func((HDC)wParam, (HWND)lParam);                        \
    if(bHandled) return bHandled;                                                           \
  }

// void OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
#define MSG_WM_SETTINGCHANGE(func)                                             \
  if (uMsg == WM_SETTINGCHANGE) {                                              \
    SetHandled();                                                              \
    func((UINT)wParam, (LPCTSTR)lParam);                                       \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnDevModeChange(LPCTSTR lpDeviceName)
#define MSG_WM_DEVMODECHANGE(func)                                             \
  if (uMsg == WM_DEVMODECHANGE) {                                              \
    SetHandled();                                                              \
    func((LPCTSTR)lParam);                                                     \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnActivateApp(BOOL bActive, DWORD dwThreadID)
#define MSG_WM_ACTIVATEAPP(func)                                               \
  if (uMsg == WM_ACTIVATEAPP) {                                                \
    SetHandled();                                                              \
    func((BOOL)wParam, (DWORD)lParam);                                         \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnFontChange()
#define MSG_WM_FONTCHANGE(func)                                                \
  if (uMsg == WM_FONTCHANGE) {                                                 \
    SetHandled();                                                              \
    func();                                                                    \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnTimeChange()
#define MSG_WM_TIMECHANGE(func)                                                \
  if (uMsg == WM_TIMECHANGE) {                                                 \
    SetHandled();                                                              \
    func();                                                                    \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnCancelMode()
#define MSG_WM_CANCELMODE(func)                                                \
  if (uMsg == WM_CANCELMODE) {                                                 \
    SetHandled();                                                              \
    func();                                                                    \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// BOOL OnSetCursor(Window wnd, UINT nHitTest, UINT message)
#define MSG_WM_SETCURSOR(func)                                                 \
  if (uMsg == WM_SETCURSOR) {                                                  \
    SetHandled();                                                              \
    lResult = (LRESULT)func((HWND)wParam, (UINT)LOWORD(lParam),                \
                            (UINT)HIWORD(lParam));                             \
    if(bHandled) return bHandled;                                                           \
  }

// int OnMouseActivate(Window wndTopLevel, UINT nHitTest, UINT message)
#define MSG_WM_MOUSEACTIVATE(func)                                             \
  if (uMsg == WM_MOUSEACTIVATE) {                                              \
    SetHandled();                                                              \
    lResult = (LRESULT)func((HWND)wParam, (UINT)LOWORD(lParam),                \
                            (UINT)HIWORD(lParam));                             \
    if(bHandled) return bHandled;                                                           \
  }

// void OnChildActivate()
#define MSG_WM_CHILDACTIVATE(func)                                             \
  if (uMsg == WM_CHILDACTIVATE) {                                              \
    SetHandled();                                                              \
    func();                                                                    \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnGetMinMaxInfo(LPMINMAXINFO lpMMI)
#define MSG_WM_GETMINMAXINFO(func)                                             \
  if (uMsg == WM_GETMINMAXINFO) {                                              \
    SetHandled();                                                              \
    func((LPMINMAXINFO)lParam);                                                \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnIconEraseBkgnd(DCT<true> dc)
#define MSG_WM_ICONERASEBKGND(func)                                            \
  if (uMsg == WM_ICONERASEBKGND) {                                             \
    SetHandled();                                                              \
    func((HDC)wParam);                                                         \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnSpoolerStatus(UINT nStatus, UINT nJobs)
#define MSG_WM_SPOOLERSTATUS(func)                                             \
  if (uMsg == WM_SPOOLERSTATUS) {                                              \
    SetHandled();                                                              \
    func((UINT)wParam, (UINT)LOWORD(lParam));                                  \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
#define MSG_WM_DRAWITEM(func)                                                  \
  if (uMsg == WM_DRAWITEM) {                                                   \
    SetHandled();                                                              \
    func((UINT)wParam, (LPDRAWITEMSTRUCT)lParam);                              \
    lResult = TRUE;                                                            \
    if(bHandled) return bHandled;                                                           \
  }

// void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
#define MSG_WM_MEASUREITEM(func)                                               \
  if (uMsg == WM_MEASUREITEM) {                                                \
    SetHandled();                                                              \
    func((UINT)wParam, (LPMEASUREITEMSTRUCT)lParam);                           \
    lResult = TRUE;                                                            \
    if(bHandled) return bHandled;                                                           \
  }

// void OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct)
#define MSG_WM_DELETEITEM(func)                                                \
  if (uMsg == WM_DELETEITEM) {                                                 \
    SetHandled();                                                              \
    func((UINT)wParam, (LPDELETEITEMSTRUCT)lParam);                            \
    lResult = TRUE;                                                            \
    if(bHandled) return bHandled;                                                           \
  }

// int OnCharToItem(UINT nChar, UINT nIndex, CListBox listBox)
#define MSG_WM_CHARTOITEM(func)                                                \
  if (uMsg == WM_CHARTOITEM) {                                                 \
    SetHandled();                                                              \
    lResult = (LRESULT)func((UINT)LOWORD(wParam), (UINT)HIWORD(wParam),        \
                            (HWND)lParam);                                     \
    if(bHandled) return bHandled;                                                           \
  }

// int OnVKeyToItem(UINT nKey, UINT nIndex, CListBox listBox)
#define MSG_WM_VKEYTOITEM(func)                                                \
  if (uMsg == WM_VKEYTOITEM) {                                                 \
    SetHandled();                                                              \
    lResult = (LRESULT)func((UINT)LOWORD(wParam), (UINT)HIWORD(wParam),        \
                            (HWND)lParam);                                     \
    if(bHandled) return bHandled;                                                           \
  }

// HCURSOR OnQueryDragIcon()
#define MSG_WM_QUERYDRAGICON(func)                                             \
  if (uMsg == WM_QUERYDRAGICON) {                                              \
    SetHandled();                                                              \
    lResult = (LRESULT)func();                                                 \
    if(bHandled) return bHandled;                                                           \
  }

// int OnCompareItem(int nIDCtl, LPCOMPAREITEMSTRUCT lpCompareItemStruct)
#define MSG_WM_COMPAREITEM(func)                                               \
  if (uMsg == WM_COMPAREITEM) {                                                \
    SetHandled();                                                              \
    lResult = (LRESULT)func((UINT)wParam, (LPCOMPAREITEMSTRUCT)lParam);        \
    if(bHandled) return bHandled;                                                           \
  }

// void OnCompacting(UINT nCpuTime)
#define MSG_WM_COMPACTING(func)                                                \
  if (uMsg == WM_COMPACTING) {                                                 \
    SetHandled();                                                              \
    func((UINT)wParam);                                                        \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct)
#define MSG_WM_NCCREATE(func)                                                  \
  if (uMsg == WM_NCCREATE) {                                                   \
    SetHandled();                                                              \
    lResult = (LRESULT)func((LPCREATESTRUCT)lParam);                           \
    if(bHandled) return bHandled;                                                           \
  }

// void OnNcDestroy()
#define MSG_WM_NCDESTROY(func)                                                 \
  if (uMsg == WM_NCDESTROY) {                                                  \
    SetHandled();                                                              \
    func();                                                                    \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
#define MSG_WM_NCCALCSIZE(func)                                                \
  if (uMsg == WM_NCCALCSIZE) {                                                 \
    SetHandled();                                                              \
    lResult = func((BOOL)wParam, lParam);                                      \
    if(bHandled) return bHandled;                                                           \
  }

// UINT OnNcHitTest(Point point)
#define MSG_WM_NCHITTEST(func)                                                 \
  if (uMsg == WM_NCHITTEST) {                                                  \
    SetHandled();                                                              \
    lResult = (LRESULT)func(Point(lParam));                                    \
    if(bHandled) return bHandled;                                                           \
  }

// void OnNcPaint(CRgnHandle rgn)
#define MSG_WM_NCPAINT(func)                                                   \
  if (uMsg == WM_NCPAINT) {                                                    \
    SetHandled();                                                              \
    func((HRGN)wParam);                                                        \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// BOOL OnNcActivate(BOOL bActive)
#define MSG_WM_NCACTIVATE(func)                                                \
  if (uMsg == WM_NCACTIVATE) {                                                 \
    SetHandled();                                                              \
    lResult = (LRESULT)func((BOOL)wParam);                                     \
    if(bHandled) return bHandled;                                                           \
  }

// UINT OnGetDlgCode(LPMSG lpMsg)
#define MSG_WM_GETDLGCODE(func)                                                \
  if (uMsg == WM_GETDLGCODE) {                                                 \
    SetHandled();                                                              \
    lResult = (LRESULT)func((LPMSG)lParam);                                    \
    if(bHandled) return bHandled;                                                           \
  }

// void OnNcMouseMove(UINT nHitTest, Point point)
#define MSG_WM_NCMOUSEMOVE(func)                                               \
  if (uMsg == WM_NCMOUSEMOVE) {                                                \
    SetHandled();                                                              \
    func((UINT)wParam, Point(lParam));                                         \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnNcLButtonDown(UINT nHitTest, Point point)
#define MSG_WM_NCLBUTTONDOWN(func)                                             \
  if (uMsg == WM_NCLBUTTONDOWN) {                                              \
    SetHandled();                                                              \
    func((UINT)wParam, Point(lParam));                                         \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnNcLButtonUp(UINT nHitTest, Point point)
#define MSG_WM_NCLBUTTONUP(func)                                               \
  if (uMsg == WM_NCLBUTTONUP) {                                                \
    SetHandled();                                                              \
    func((UINT)wParam, Point(lParam));                                         \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnNcLButtonDblClk(UINT nHitTest, Point point)
#define MSG_WM_NCLBUTTONDBLCLK(func)                                           \
  if (uMsg == WM_NCLBUTTONDBLCLK) {                                            \
    SetHandled();                                                              \
    func((UINT)wParam, Point(lParam));                                         \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnNcRButtonDown(UINT nHitTest, Point point)
#define MSG_WM_NCRBUTTONDOWN(func)                                             \
  if (uMsg == WM_NCRBUTTONDOWN) {                                              \
    SetHandled();                                                              \
    func((UINT)wParam, Point(lParam));                                         \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnNcRButtonUp(UINT nHitTest, Point point)
#define MSG_WM_NCRBUTTONUP(func)                                               \
  if (uMsg == WM_NCRBUTTONUP) {                                                \
    SetHandled();                                                              \
    func((UINT)wParam, Point(lParam));                                         \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnNcRButtonDblClk(UINT nHitTest, Point point)
#define MSG_WM_NCRBUTTONDBLCLK(func)                                           \
  if (uMsg == WM_NCRBUTTONDBLCLK) {                                            \
    SetHandled();                                                              \
    func((UINT)wParam, Point(lParam));                                         \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnNcMButtonDown(UINT nHitTest, Point point)
#define MSG_WM_NCMBUTTONDOWN(func)                                             \
  if (uMsg == WM_NCMBUTTONDOWN) {                                              \
    SetHandled();                                                              \
    func((UINT)wParam, Point(lParam));                                         \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnNcMButtonUp(UINT nHitTest, Point point)
#define MSG_WM_NCMBUTTONUP(func)                                               \
  if (uMsg == WM_NCMBUTTONUP) {                                                \
    SetHandled();                                                              \
    func((UINT)wParam, Point(lParam));                                         \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnNcMButtonDblClk(UINT nHitTest, Point point)
#define MSG_WM_NCMBUTTONDBLCLK(func)                                           \
  if (uMsg == WM_NCMBUTTONDBLCLK) {                                            \
    SetHandled();                                                              \
    func((UINT)wParam, Point(lParam));                                         \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
#define MSG_WM_KEYDOWN(func)                                                   \
  if (uMsg == WM_KEYDOWN) {                                                    \
    SetHandled();                                                              \
    func((TCHAR)wParam, (UINT)lParam & 0xFFFF,                                 \
         (UINT)((lParam & 0xFFFF0000) >> 16));                                 \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
#define MSG_WM_KEYUP(func)                                                     \
  if (uMsg == WM_KEYUP) {                                                      \
    SetHandled();                                                              \
    func((TCHAR)wParam, (UINT)lParam & 0xFFFF,                                 \
         (UINT)((lParam & 0xFFFF0000) >> 16));                                 \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
#define MSG_WM_CHAR(func)                                                      \
  if (uMsg == WM_CHAR) {                                                       \
    SetHandled();                                                              \
    func((TCHAR)wParam, (UINT)lParam & 0xFFFF,                                 \
         (UINT)((lParam & 0xFFFF0000) >> 16));                                 \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags)
#define MSG_WM_DEADCHAR(func)                                                  \
  if (uMsg == WM_DEADCHAR) {                                                   \
    SetHandled();                                                              \
    func((TCHAR)wParam, (UINT)lParam & 0xFFFF,                                 \
         (UINT)((lParam & 0xFFFF0000) >> 16));                                 \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
#define MSG_WM_SYSKEYDOWN(func)                                                \
  if (uMsg == WM_SYSKEYDOWN) {                                                 \
    SetHandled();                                                              \
    func((TCHAR)wParam, (UINT)lParam & 0xFFFF,                                 \
         (UINT)((lParam & 0xFFFF0000) >> 16));                                 \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
#define MSG_WM_SYSKEYUP(func)                                                  \
  if (uMsg == WM_SYSKEYUP) {                                                   \
    SetHandled();                                                              \
    func((TCHAR)wParam, (UINT)lParam & 0xFFFF,                                 \
         (UINT)((lParam & 0xFFFF0000) >> 16));                                 \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags)
#define MSG_WM_SYSCHAR(func)                                                   \
  if (uMsg == WM_SYSCHAR) {                                                    \
    SetHandled();                                                              \
    func((TCHAR)wParam, (UINT)lParam & 0xFFFF,                                 \
         (UINT)((lParam & 0xFFFF0000) >> 16));                                 \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnSysDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags)
#define MSG_WM_SYSDEADCHAR(func)                                               \
  if (uMsg == WM_SYSDEADCHAR) {                                                \
    SetHandled();                                                              \
    func((TCHAR)wParam, (UINT)lParam & 0xFFFF,                                 \
         (UINT)((lParam & 0xFFFF0000) >> 16));                                 \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnSysCommand(UINT nID, Point point)
#define MSG_WM_SYSCOMMAND(func)                                                \
  if (uMsg == WM_SYSCOMMAND) {                                                 \
    SetHandled();                                                              \
    func((UINT)wParam, Point(lParam));                                         \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnTCard(UINT idAction, DWORD dwActionData)
#define MSG_WM_TCARD(func)                                                     \
  if (uMsg == WM_TCARD) {                                                      \
    SetHandled();                                                              \
    func((UINT)wParam, (DWORD)lParam);                                         \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnTimer(UINT_PTR nIDEvent)
#define MSG_WM_TIMER(func)                                                     \
  if (uMsg == WM_TIMER) {                                                      \
    SetHandled();                                                              \
    func((UINT_PTR)wParam);                                                    \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar)
#define MSG_WM_HSCROLL(func)                                                   \
  if (uMsg == WM_HSCROLL) {                                                    \
    SetHandled();                                                              \
    func((int)LOWORD(wParam), (short)HIWORD(wParam), (HWND)lParam);            \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar)
#define MSG_WM_VSCROLL(func)                                                   \
  if (uMsg == WM_VSCROLL) {                                                    \
    SetHandled();                                                              \
    func((int)LOWORD(wParam), (short)HIWORD(wParam), (HWND)lParam);            \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnInitMenu(CMenuHandle menu)
#define MSG_WM_INITMENU(func)                                                  \
  if (uMsg == WM_INITMENU) {                                                   \
    SetHandled();                                                              \
    func((HMENU)wParam);                                                       \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnInitMenuPopup(CMenuHandle menuPopup, UINT nIndex, BOOL bSysMenu)
#define MSG_WM_INITMENUPOPUP(func)                                             \
  if (uMsg == WM_INITMENUPOPUP) {                                              \
    SetHandled();                                                              \
    func((HMENU)wParam, (UINT)LOWORD(lParam), (BOOL)HIWORD(lParam));           \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnMenuSelect(UINT nItemID, UINT nFlags, CMenuHandle menu)
#define MSG_WM_MENUSELECT(func)                                                \
  if (uMsg == WM_MENUSELECT) {                                                 \
    SetHandled();                                                              \
    func((UINT)LOWORD(wParam), (UINT)HIWORD(wParam), (HMENU)lParam);           \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenuHandle menu)
#define MSG_WM_MENUCHAR(func)                                                  \
  if (uMsg == WM_MENUCHAR) {                                                   \
    SetHandled();                                                              \
    lResult =                                                                  \
        func((TCHAR)LOWORD(wParam), (UINT)HIWORD(wParam), (HMENU)lParam);      \
    if(bHandled) return bHandled;                                                           \
  }

// LRESULT OnNotify(int idCtrl, LPNMHDR pnmh)
#define MSG_WM_NOTIFY(func)                                                    \
  if (uMsg == WM_NOTIFY) {                                                     \
    SetHandled();                                                              \
    lResult = func((int)wParam, (LPNMHDR)lParam);                              \
    if(bHandled) return bHandled;                                                           \
  }

// void OnEnterIdle(UINT nWhy, Window wndWho)
#define MSG_WM_ENTERIDLE(func)                                                 \
  if (uMsg == WM_ENTERIDLE) {                                                  \
    SetHandled();                                                              \
    func((UINT)wParam, (HWND)lParam);                                          \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnMouseMove(UINT nFlags, Point point)
#define MSG_WM_MOUSEMOVE(func)                                                 \
  if (uMsg == WM_MOUSEMOVE) {                                                  \
    SetHandled();                                                              \
    func((UINT)wParam, Point(lParam));                                         \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// BOOL OnMouseWheel(UINT nFlags, short zDelta, Point pt)
#define MSG_WM_MOUSEWHEEL(func)                                                \
  if (uMsg == WM_MOUSEWHEEL) {                                                 \
    SetHandled();                                                              \
    lResult = (LRESULT)func((UINT)LOWORD(wParam), (short)HIWORD(wParam),       \
                            Point(lParam));                                    \
    if(bHandled) return bHandled;                                                           \
  }

// void OnLButtonDown(UINT nFlags, Point point)
#define MSG_WM_LBUTTONDOWN(func)                                               \
  if (uMsg == WM_LBUTTONDOWN) {                                                \
    SetHandled();                                                              \
    func((UINT)wParam, Point(lParam));                                         \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnLButtonUp(UINT nFlags, Point point)
#define MSG_WM_LBUTTONUP(func)                                                 \
  if (uMsg == WM_LBUTTONUP) {                                                  \
    SetHandled();                                                              \
    func((UINT)wParam, Point(lParam));                                         \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnLButtonDblClk(UINT nFlags, Point point)
#define MSG_WM_LBUTTONDBLCLK(func)                                             \
  if (uMsg == WM_LBUTTONDBLCLK) {                                              \
    SetHandled();                                                              \
    func((UINT)wParam, Point(lParam));                                         \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnRButtonDown(UINT nFlags, Point point)
#define MSG_WM_RBUTTONDOWN(func)                                               \
  if (uMsg == WM_RBUTTONDOWN) {                                                \
    SetHandled();                                                              \
    func((UINT)wParam, Point(lParam));                                         \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnRButtonUp(UINT nFlags, Point point)
#define MSG_WM_RBUTTONUP(func)                                                 \
  if (uMsg == WM_RBUTTONUP) {                                                  \
    SetHandled();                                                              \
    func((UINT)wParam, Point(lParam));                                         \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnRButtonDblClk(UINT nFlags, Point point)
#define MSG_WM_RBUTTONDBLCLK(func)                                             \
  if (uMsg == WM_RBUTTONDBLCLK) {                                              \
    SetHandled();                                                              \
    func((UINT)wParam, Point(lParam));                                         \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnMButtonDown(UINT nFlags, Point point)
#define MSG_WM_MBUTTONDOWN(func)                                               \
  if (uMsg == WM_MBUTTONDOWN) {                                                \
    SetHandled();                                                              \
    func((UINT)wParam, Point(lParam));                                         \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnMButtonUp(UINT nFlags, Point point)
#define MSG_WM_MBUTTONUP(func)                                                 \
  if (uMsg == WM_MBUTTONUP) {                                                  \
    SetHandled();                                                              \
    func((UINT)wParam, Point(lParam));                                         \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnMButtonDblClk(UINT nFlags, Point point)
#define MSG_WM_MBUTTONDBLCLK(func)                                             \
  if (uMsg == WM_MBUTTONDBLCLK) {                                              \
    SetHandled();                                                              \
    func((UINT)wParam, Point(lParam));                                         \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnParentNotify(UINT message, UINT nChildID, LPARAM lParam)
#define MSG_WM_PARENTNOTIFY(func)                                              \
  if (uMsg == WM_PARENTNOTIFY) {                                               \
    SetHandled();                                                              \
    func((UINT)LOWORD(wParam), (UINT)HIWORD(wParam), lParam);                  \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnMDIActivate(Window wndActivate, Window wndDeactivate)
#define MSG_WM_MDIACTIVATE(func)                                               \
  if (uMsg == WM_MDIACTIVATE) {                                                \
    SetHandled();                                                              \
    func((HWND)wParam, (HWND)lParam);                                          \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnRenderFormat(UINT nFormat)
#define MSG_WM_RENDERFORMAT(func)                                              \
  if (uMsg == WM_RENDERFORMAT) {                                               \
    SetHandled();                                                              \
    func((UINT)wParam);                                                        \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnRenderAllFormats()
#define MSG_WM_RENDERALLFORMATS(func)                                          \
  if (uMsg == WM_RENDERALLFORMATS) {                                           \
    SetHandled();                                                              \
    func();                                                                    \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnDestroyClipboard()
#define MSG_WM_DESTROYCLIPBOARD(func)                                          \
  if (uMsg == WM_DESTROYCLIPBOARD) {                                           \
    SetHandled();                                                              \
    func();                                                                    \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnDrawClipboard()
#define MSG_WM_DRAWCLIPBOARD(func)                                             \
  if (uMsg == WM_DRAWCLIPBOARD) {                                              \
    SetHandled();                                                              \
    func();                                                                    \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnPaintClipboard(Window wndViewer, const LPPAINTSTRUCT lpPaintStruct)
#define MSG_WM_PAINTCLIPBOARD(func)                                            \
  if (uMsg == WM_PAINTCLIPBOARD) {                                             \
    SetHandled();                                                              \
    func((HWND)wParam, (const LPPAINTSTRUCT)::GlobalLock((HGLOBAL)lParam));    \
    ::GlobalUnlock((HGLOBAL)lParam);                                           \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnVScrollClipboard(Window wndViewer, UINT nSBCode, UINT nPos)
#define MSG_WM_VSCROLLCLIPBOARD(func)                                          \
  if (uMsg == WM_VSCROLLCLIPBOARD) {                                           \
    SetHandled();                                                              \
    func((HWND)wParam, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam));            \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnContextMenu(Window wnd, Point point)
#define MSG_WM_CONTEXTMENU(func)                                               \
  if (uMsg == WM_CONTEXTMENU) {                                                \
    SetHandled();                                                              \
    func((HWND)wParam, Point(lParam));                                         \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnSizeClipboard(Window wndViewer, const LPRECT lpRect)
#define MSG_WM_SIZECLIPBOARD(func)                                             \
  if (uMsg == WM_SIZECLIPBOARD) {                                              \
    SetHandled();                                                              \
    func((HWND)wParam, (const LPRECT)::GlobalLock((HGLOBAL)lParam));           \
    ::GlobalUnlock((HGLOBAL)lParam);                                           \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnAskCbFormatName(UINT nMaxCount, LPTSTR lpszString)
#define MSG_WM_ASKCBFORMATNAME(func)                                           \
  if (uMsg == WM_ASKCBFORMATNAME) {                                            \
    SetHandled();                                                              \
    func((UINT)wParam, (LPTSTR)lParam);                                        \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnChangeCbChain(Window wndRemove, Window wndAfter)
#define MSG_WM_CHANGECBCHAIN(func)                                             \
  if (uMsg == WM_CHANGECBCHAIN) {                                              \
    SetHandled();                                                              \
    func((HWND)wParam, (HWND)lParam);                                          \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnHScrollClipboard(Window wndViewer, UINT nSBCode, UINT nPos)
#define MSG_WM_HSCROLLCLIPBOARD(func)                                          \
  if (uMsg == WM_HSCROLLCLIPBOARD) {                                           \
    SetHandled();                                                              \
    func((HWND)wParam, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam));            \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// BOOL OnQueryNewPalette()
#define MSG_WM_QUERYNEWPALETTE(func)                                           \
  if (uMsg == WM_QUERYNEWPALETTE) {                                            \
    SetHandled();                                                              \
    lResult = (LRESULT)func();                                                 \
    if(bHandled) return bHandled;                                                           \
  }

// void OnPaletteChanged(Window wndFocus)
#define MSG_WM_PALETTECHANGED(func)                                            \
  if (uMsg == WM_PALETTECHANGED) {                                             \
    SetHandled();                                                              \
    func((HWND)wParam);                                                        \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnPaletteIsChanging(Window wndPalChg)
#define MSG_WM_PALETTEISCHANGING(func)                                         \
  if (uMsg == WM_PALETTEISCHANGING) {                                          \
    SetHandled();                                                              \
    func((HWND)wParam);                                                        \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnDropFiles(HDROP hDropInfo)
#define MSG_WM_DROPFILES(func)                                                 \
  if (uMsg == WM_DROPFILES) {                                                  \
    SetHandled();                                                              \
    func((HDROP)wParam);                                                       \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnWindowPosChanging(LPWINDOWPOS lpWndPos)
#define MSG_WM_WINDOWPOSCHANGING(func)                                         \
  if (uMsg == WM_WINDOWPOSCHANGING) {                                          \
    SetHandled();                                                              \
    func((LPWINDOWPOS)lParam);                                                 \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnWindowPosChanged(LPWINDOWPOS lpWndPos)
#define MSG_WM_WINDOWPOSCHANGED(func)                                          \
  if (uMsg == WM_WINDOWPOSCHANGED) {                                           \
    SetHandled();                                                              \
    func((LPWINDOWPOS)lParam);                                                 \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnExitMenuLoop(BOOL fIsTrackPopupMenu)
#define MSG_WM_EXITMENULOOP(func)                                              \
  if (uMsg == WM_EXITMENULOOP) {                                               \
    SetHandled();                                                              \
    func((BOOL)wParam);                                                        \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnEnterMenuLoop(BOOL fIsTrackPopupMenu)
#define MSG_WM_ENTERMENULOOP(func)                                             \
  if (uMsg == WM_ENTERMENULOOP) {                                              \
    SetHandled();                                                              \
    func((BOOL)wParam);                                                        \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
#define MSG_WM_STYLECHANGED(func)                                              \
  if (uMsg == WM_STYLECHANGED) {                                               \
    SetHandled();                                                              \
    func((UINT)wParam, (LPSTYLESTRUCT)lParam);                                 \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnStyleChanging(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
#define MSG_WM_STYLECHANGING(func)                                             \
  if (uMsg == WM_STYLECHANGING) {                                              \
    SetHandled();                                                              \
    func((UINT)wParam, (LPSTYLESTRUCT)lParam);                                 \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnSizing(UINT fwSide, LPRECT pRect)
#define MSG_WM_SIZING(func)                                                    \
  if (uMsg == WM_SIZING) {                                                     \
    SetHandled();                                                              \
    func((UINT)wParam, (LPRECT)lParam);                                        \
    lResult = TRUE;                                                            \
    if(bHandled) return bHandled;                                                           \
  }

// void OnMoving(UINT fwSide, LPRECT pRect)
#define MSG_WM_MOVING(func)                                                    \
  if (uMsg == WM_MOVING) {                                                     \
    SetHandled();                                                              \
    func((UINT)wParam, (LPRECT)lParam);                                        \
    lResult = TRUE;                                                            \
    if(bHandled) return bHandled;                                                           \
  }

// void OnCaptureChanged(Window wnd)
#define MSG_WM_CAPTURECHANGED(func)                                            \
  if (uMsg == WM_CAPTURECHANGED) {                                             \
    SetHandled();                                                              \
    func((HWND)lParam);                                                        \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// BOOL OnDeviceChange(UINT nEventType, DWORD_PTR dwData)
#define MSG_WM_DEVICECHANGE(func)                                              \
  if (uMsg == WM_DEVICECHANGE) {                                               \
    SetHandled();                                                              \
    lResult = (LRESULT)func((UINT)wParam, (DWORD_PTR)lParam);                  \
    if(bHandled) return bHandled;                                                           \
  }

// void OnCommand(UINT uNotifyCode, int nID, Window wndCtl)
#define MSG_WM_COMMAND(func)                                                   \
  if (uMsg == WM_COMMAND) {                                                    \
    SetHandled();                                                              \
    func((UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam);             \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnDisplayChange(UINT uBitsPerPixel, Size sizeScreen)
#define MSG_WM_DISPLAYCHANGE(func)                                             \
  if (uMsg == WM_DISPLAYCHANGE) {                                              \
    SetHandled();                                                              \
    func((UINT)wParam, Size(lParam));                                          \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnEnterSizeMove()
#define MSG_WM_ENTERSIZEMOVE(func)                                             \
  if (uMsg == WM_ENTERSIZEMOVE) {                                              \
    SetHandled();                                                              \
    func();                                                                    \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnExitSizeMove()
#define MSG_WM_EXITSIZEMOVE(func)                                              \
  if (uMsg == WM_EXITSIZEMOVE) {                                               \
    SetHandled();                                                              \
    func();                                                                    \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// HERSULT OnGetFont()
#define MSG_WM_GETFONT(func)                                                   \
  if (uMsg == WM_GETFONT) {                                                    \
    SetHandled();                                                              \
    lResult = (LRESULT)func();                                                 \
    if(bHandled) return bHandled;                                                           \
  }

// LRESULT OnGetHotKey()
#define MSG_WM_GETHOTKEY(func)                                                 \
  if (uMsg == WM_GETHOTKEY) {                                                  \
    SetHandled();                                                              \
    lResult = func();                                                          \
    if(bHandled) return bHandled;                                                           \
  }

// HICON OnGetIcon()
#define MSG_WM_GETICON(func)                                                   \
  if (uMsg == WM_GETICON) {                                                    \
    SetHandled();                                                              \
    lResult = (LRESULT)func((UINT)wParam);                                     \
    if(bHandled) return bHandled;                                                           \
  }

// int OnGetText(int cchTextMax, LPTSTR lpszText)
#define MSG_WM_GETTEXT(func)                                                   \
  if (uMsg == WM_GETTEXT) {                                                    \
    SetHandled();                                                              \
    lResult = (LRESULT)func((int)wParam, (LPTSTR)lParam);                      \
    if(bHandled) return bHandled;                                                           \
  }

// int OnGetTextLength()
#define MSG_WM_GETTEXTLENGTH(func)                                             \
  if (uMsg == WM_GETTEXTLENGTH) {                                              \
    SetHandled();                                                              \
    lResult = (LRESULT)func();                                                 \
    if(bHandled) return bHandled;                                                           \
  }

// void OnHelp(LPHELPINFO lpHelpInfo)
#define MSG_WM_HELP(func)                                                      \
  if (uMsg == WM_HELP) {                                                       \
    SetHandled();                                                              \
    func((LPHELPINFO)lParam);                                                  \
    lResult = TRUE;                                                            \
    if(bHandled) return bHandled;                                                           \
  }

// void OnHotKey(int nHotKeyID, UINT uModifiers, UINT uVirtKey)
#define MSG_WM_HOTKEY(func)                                                    \
  if (uMsg == WM_HOTKEY) {                                                     \
    SetHandled();                                                              \
    func((int)wParam, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam));             \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnInputLangChange(DWORD dwCharSet, HKL hKbdLayout)
#define MSG_WM_INPUTLANGCHANGE(func)                                           \
  if (uMsg == WM_INPUTLANGCHANGE) {                                            \
    SetHandled();                                                              \
    func((DWORD)wParam, (HKL)lParam);                                          \
    lResult = TRUE;                                                            \
    if(bHandled) return bHandled;                                                           \
  }

// void OnInputLangChangeRequest(BOOL bSysCharSet, HKL hKbdLayout)
#define MSG_WM_INPUTLANGCHANGEREQUEST(func)                                    \
  if (uMsg == WM_INPUTLANGCHANGEREQUEST) {                                     \
    SetHandled();                                                              \
    func((BOOL)wParam, (HKL)lParam);                                           \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnNextDlgCtl(BOOL bHandle, WPARAM wCtlFocus)
#define MSG_WM_NEXTDLGCTL(func)                                                \
  if (uMsg == WM_NEXTDLGCTL) {                                                 \
    SetHandled();                                                              \
    func((BOOL)LOWORD(lParam), wParam);                                        \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnNextMenu(int nVirtKey, LPMDINEXTMENU lpMdiNextMenu)
#define MSG_WM_NEXTMENU(func)                                                  \
  if (uMsg == WM_NEXTMENU) {                                                   \
    SetHandled();                                                              \
    func((int)wParam, (LPMDINEXTMENU)lParam);                                  \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// int OnNotifyFormat(Window wndFrom, int nCommand)
#define MSG_WM_NOTIFYFORMAT(func)                                              \
  if (uMsg == WM_NOTIFYFORMAT) {                                               \
    SetHandled();                                                              \
    lResult = (LRESULT)func((HWND)wParam, (int)lParam);                        \
    if(bHandled) return bHandled;                                                           \
  }

// BOOL OnPowerBroadcast(DWORD dwPowerEvent, DWORD_PTR dwData)
#define MSG_WM_POWERBROADCAST(func)                                            \
  if (uMsg == WM_POWERBROADCAST) {                                             \
    SetHandled();                                                              \
    lResult = (LRESULT)func((DWORD)wParam, (DWORD_PTR)lParam);                 \
    if(bHandled) return bHandled;                                                           \
  }

// void OnPrint(DCT<true> dc, UINT uFlags)
#define MSG_WM_PRINT(func)                                                     \
  if (uMsg == WM_PRINT) {                                                      \
    SetHandled();                                                              \
    func((HDC)wParam, (UINT)lParam);                                           \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnPrintClient(DCT<true> dc, UINT uFlags)
#define MSG_WM_PRINTCLIENT(func)                                               \
  if (uMsg == WM_PRINTCLIENT) {                                                \
    SetHandled();                                                              \
    func((HDC)wParam, (UINT)lParam);                                           \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnRasDialEvent(RASCONNSTATE rasconnstate, DWORD dwError)
#define MSG_WM_RASDIALEVENT(func)                                              \
  if (uMsg == WM_RASDIALEVENT) {                                               \
    SetHandled();                                                              \
    func((RASCONNSTATE)wParam, (DWORD)lParam);                                 \
    lResult = TRUE;                                                            \
    if(bHandled) return bHandled;                                                           \
  }

// void OnSetFont(GDIFontHandle font, BOOL bRedraw)
#define MSG_WM_SETFONT(func)                                                   \
  if (uMsg == WM_SETFONT) {                                                    \
    SetHandled();                                                              \
    func((HFONT)wParam, (BOOL)LOWORD(lParam));                                 \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// int OnSetHotKey(int nVirtKey, UINT uFlags)
#define MSG_WM_SETHOTKEY(func)                                                 \
  if (uMsg == WM_SETHOTKEY) {                                                  \
    SetHandled();                                                              \
    lResult = (LRESULT)func((int)LOBYTE(LOWORD(wParam)),                       \
                            (UINT)HIBYTE(LOWORD(wParam)));                     \
    if(bHandled) return bHandled;                                                           \
  }

// HICON OnSetIcon(UINT uType, HICON hIcon)
#define MSG_WM_SETICON(func)                                                   \
  if (uMsg == WM_SETICON) {                                                    \
    SetHandled();                                                              \
    lResult = (LRESULT)func((UINT)wParam, (HICON)lParam);                      \
    if(bHandled) return bHandled;                                                           \
  }

// void OnSetRedraw(BOOL bRedraw)
#define MSG_WM_SETREDRAW(func)                                                 \
  if (uMsg == WM_SETREDRAW) {                                                  \
    SetHandled();                                                              \
    func((BOOL)wParam);                                                        \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// int OnSetText(LPCTSTR lpstrText)
#define MSG_WM_SETTEXT(func)                                                   \
  if (uMsg == WM_SETTEXT) {                                                    \
    SetHandled();                                                              \
    lResult = (LRESULT)func((LPCTSTR)lParam);                                  \
    if(bHandled) return bHandled;                                                           \
  }

// void OnUserChanged()
#define MSG_WM_USERCHANGED(func)                                               \
  if (uMsg == WM_USERCHANGED) {                                                \
    SetHandled();                                                              \
    func();                                                                    \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnMouseHover(WPARAM wParam, Point ptPos)
#define MSG_WM_MOUSEHOVER(func)                                                \
  if (uMsg == WM_MOUSEHOVER) {                                                 \
    SetHandled();                                                              \
    func(wParam, Point(lParam));                                               \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnMouseLeave()
#define MSG_WM_MOUSELEAVE(func)                                                \
  if (uMsg == WM_MOUSELEAVE) {                                                 \
    SetHandled();                                                              \
    func();                                                                    \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

#endif

#if (WINVER >= 0x0500)

// void OnMenuRButtonUp(WPARAM wParam, CMenuHandle menu)
#define MSG_WM_MENURBUTTONUP(func)                                             \
  if (uMsg == WM_MENURBUTTONUP) {                                              \
    SetHandled();                                                              \
    func(wParam, (HMENU)lParam);                                               \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// LRESULT OnMenuDrag(WPARAM wParam, CMenuHandle menu)
#define MSG_WM_MENUDRAG(func)                                                  \
  if (uMsg == WM_MENUDRAG) {                                                   \
    SetHandled();                                                              \
    lResult = func(wParam, (HMENU)lParam);                                     \
    if(bHandled) return bHandled;                                                           \
  }

// LRESULT OnMenuGetObject(PMENUGETOBJECTINFO info)
#define MSG_WM_MENUGETOBJECT(func)                                             \
  if (uMsg == WM_MENUGETOBJECT) {                                              \
    SetHandled();                                                              \
    lResult = func((PMENUGETOBJECTINFO)lParam);                                \
    if(bHandled) return bHandled;                                                           \
  }

// void OnUnInitMenuPopup(UINT nID, CMenuHandle menu)
#define MSG_WM_UNINITMENUPOPUP(func)                                           \
  if (uMsg == WM_UNINITMENUPOPUP) {                                            \
    SetHandled();                                                              \
    func((UINT)HIWORD(lParam), (HMENU)wParam);                                 \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnMenuCommand(WPARAM nIndex, CMenuHandle menu)
#define MSG_WM_MENUCOMMAND(func)                                               \
  if (uMsg == WM_MENUCOMMAND) {                                                \
    SetHandled();                                                              \
    func(wParam, (HMENU)lParam);                                               \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

#endif

#if (_WIN32_WINNT >= 0x0500)

// BOOL OnAppCommand(Window wndFocus, short cmd, WORD uDevice, int dwKeys)
#define MSG_WM_APPCOMMAND(func)                                                \
  if (uMsg == WM_APPCOMMAND) {                                                 \
    SetHandled();                                                              \
    lResult =                                                                  \
        (LRESULT)func((HWND)wParam, GET_APPCOMMAND_LPARAM(lParam),             \
                      GET_DEVICE_LPARAM(lParam), GET_KEYSTATE_LPARAM(lParam)); \
    if(bHandled) return bHandled;                                                           \
  }

// void OnNCXButtonDown(int fwButton, short nHittest, Point ptPos)
#define MSG_WM_NCXBUTTONDOWN(func)                                             \
  if (uMsg == WM_NCXBUTTONDOWN) {                                              \
    SetHandled();                                                              \
    func(GET_XBUTTON_WPARAM(wParam), GET_NCHITTEST_WPARAM(wParam),             \
         Point(lParam));                                                       \
    lResult = 0;                                                               \
    return TRUE;                                                               \
  }

// void OnNCXButtonUp(int fwButton, short nHittest, Point ptPos)
#define MSG_WM_NCXBUTTONUP(func)                                               \
  if (uMsg == WM_NCXBUTTONUP) {                                                \
    SetHandled();                                                              \
    func(GET_XBUTTON_WPARAM(wParam), GET_NCHITTEST_WPARAM(wParam),             \
         Point(lParam));                                                       \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnNCXButtonDblClk(int fwButton, short nHittest, Point ptPos)
#define MSG_WM_NCXBUTTONDBLCLK(func)                                           \
  if (uMsg == WM_NCXBUTTONDBLCLK) {                                            \
    SetHandled();                                                              \
    func(GET_XBUTTON_WPARAM(wParam), GET_NCHITTEST_WPARAM(wParam),             \
         Point(lParam));                                                       \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnXButtonDown(int fwButton, int dwKeys, Point ptPos)
#define MSG_WM_XBUTTONDOWN(func)                                               \
  if (uMsg == WM_XBUTTONDOWN) {                                                \
    SetHandled();                                                              \
    func(GET_XBUTTON_WPARAM(wParam), GET_KEYSTATE_WPARAM(wParam),              \
         Point(lParam));                                                       \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnXButtonUp(int fwButton, int dwKeys, Point ptPos)
#define MSG_WM_XBUTTONUP(func)                                                 \
  if (uMsg == WM_XBUTTONUP) {                                                  \
    SetHandled();                                                              \
    func(GET_XBUTTON_WPARAM(wParam), GET_KEYSTATE_WPARAM(wParam),              \
         Point(lParam));                                                       \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnXButtonDblClk(int fwButton, int dwKeys, Point ptPos)
#define MSG_WM_XBUTTONDBLCLK(func)                                             \
  if (uMsg == WM_XBUTTONDBLCLK) {                                              \
    SetHandled();                                                              \
    func(GET_XBUTTON_WPARAM(wParam), GET_KEYSTATE_WPARAM(wParam),              \
         Point(lParam));                                                       \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnChangeUIState(WORD nAction, WORD nState)
#define MSG_WM_CHANGEUISTATE(func)                                             \
  if (uMsg == WM_CHANGEUISTATE) {                                              \
    SetHandled();                                                              \
    func(LOWORD(wParam), HIWORD(wParam));                                      \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnUpdateUIState(WORD nAction, WORD nState)
#define MSG_WM_UPDATEUISTATE(func)                                             \
  if (uMsg == WM_UPDATEUISTATE) {                                              \
    SetHandled();                                                              \
    func(LOWORD(wParam), HIWORD(wParam));                                      \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// LRESULT OnQueryUIState()
#define MSG_WM_QUERYUISTATE(func)                                              \
  if (uMsg == WM_QUERYUISTATE) {                                               \
    SetHandled();                                                              \
    lResult = func();                                                          \
    if(bHandled) return bHandled;                                                           \
  }

#endif

#if (_WIN32_WINNT >= 0x0501)

// void OnInput(WPARAM RawInputCode, HRAWINPUT hRawInput)
#define MSG_WM_INPUT(func)                                                     \
  if (uMsg == WM_INPUT) {                                                      \
    SetHandled();                                                              \
    func(GET_RAWINPUT_CODE_WPARAM(wParam), (HRAWINPUT)lParam);                 \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnUniChar(TCHAR nChar, UINT nRepCnt, UINT nFlags)
#define MSG_WM_UNICHAR(func)                                                   \
  if (uMsg == WM_UNICHAR) {                                                    \
    SetHandled();                                                              \
    func((TCHAR)wParam, (UINT)lParam & 0xFFFF,                                 \
         (UINT)((lParam & 0xFFFF0000) >> 16));                                 \
    if (bHandled) {                                                            \
      lResult = (wParam == UNICODE_NOCHAR) ? TRUE : FALSE;                     \
      if(bHandled) return bHandled;                                                         \
    }                                                                          \
  }

// void OnWTSSessionChange(WPARAM nStatusCode, PWTSSESSION_NOTIFICATION
// nSessionID)
#define MSG_WM_WTSSESSION_CHANGE(func)                                         \
  if (uMsg == WM_WTSSESSION_CHANGE) {                                          \
    SetHandled();                                                              \
    func(wParam, (PWTSSESSION_NOTIFICATION)lParam);                            \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

// void OnThemeChanged()
#define MSG_WM_THEMECHANGED(func)                                              \
  if (uMsg == WM_THEMECHANGED) {                                               \
    SetHandled();                                                              \
    func();                                                                    \
    lResult = 0;                                                               \
    if(bHandled) return bHandled;                                                           \
  }

#endif

#if (_WIN32_WINNT >= 0x0600)

// BOOL OnMouseHWheel(UINT nFlags, short zDelta, Point pt)
#define MSG_WM_MOUSEHWHEEL(func)                                               \
  if (uMsg == WM_MOUSEHWHEEL) {                                                \
    SetHandled();                                                              \
    lResult = (LRESULT)func((UINT)LOWORD(wParam), (short)HIWORD(wParam),       \
                            Point(lParam));                                    \
    if(bHandled) return bHandled;                                                           \
  }

#endif

#endif
