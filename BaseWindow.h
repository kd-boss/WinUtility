#ifndef __BASEWINDOW_H__
#define __BASEWINDOW_H__
#ifndef STRICT
#define STRICT
#endif
#ifndef WINVER
#define WINVER 0x0601
#define NTDDI_VERSION 0x0601
#endif

//Windows includes
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <olectl.h>


//Drag in the standard library. All of it.
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <exception>
#include <comdef.h>

#ifndef __HRESULT_FROM_WIN32
#define __HRESULT_FROM_WIN32(x) ((HRESULT)(x) <= 0 ? ((HRESULT)(x)) : ((HRESULT) (((x) & 0x0000FFFF) | (FACILITY_WIN32 << 16) | 0x80000000)))
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

class URECT
{
	LPRECT m_lpRect;
public:
	URECT() : m_lpRect(nullptr) {}
	URECT(URECT&) = default;
	URECT(URECT&&) = default;
	URECT(LPRECT lpRect = nullptr) : m_lpRect(lpRect) {}
	URECT(RECT& rc) : m_lpRect(&rc) {}
	URECT& operator =(URECT&) = default;
	URECT& operator =(URECT&&) = default;
	URECT& operator =(LPRECT rc) { m_lpRect = rc; return *this;}
	URECT& operator =(RECT& rc) { m_lpRect = &rc; return *this; }
	LPRECT Get() { return m_lpRect; }
};

class UMenuOrID
{
	HMENU m_hMenu;
public:
	UMenuOrID() : m_hMenu(nullptr) {}
	UMenuOrID(UMenuOrID&) = default;
	UMenuOrID(UMenuOrID&&) = default;
	UMenuOrID(HMENU hMenu) : m_hMenu(hMenu) {}
	UMenuOrID(UINT nID) : m_hMenu((HMENU)(UINT_PTR)nID) {}
	UMenuOrID& operator =(UMenuOrID&) = default;
	UMenuOrID& operator =(UMenuOrID&&) = default;
	UMenuOrID& operator =(UINT nID){ m_hMenu = (HMENU)(UINT_PTR)nID; return *this; }
	UMenuOrID& operator =(HMENU hMenu) { m_hMenu = hMenu; return *this; }
	HMENU Get() { return m_hMenu; }
};

class UStringOrId
{
	LPCTSTR m_lpstr;
public:
	UStringOrId() : m_lpstr(nullptr) {}
	UStringOrId(UStringOrId&) = default;
	UStringOrId(UStringOrId&&) = default;
	UStringOrId(LPCTSTR lpString) : m_lpstr(lpString) {}
	UStringOrId(UINT nID) : m_lpstr(MAKEINTRESOURCE(nID)) {}
	UStringOrId& operator =(UStringOrId&) = default;
	UStringOrId& operator =(UStringOrId&&) = default;
	UStringOrId& operator =(LPCTSTR lpString) { m_lpstr = lpString; return *this; }
	UStringOrId& operator =(UINT nID) { m_lpstr = MAKEINTRESOURCE(nID); return *this; }
	LPCTSTR Get() { return m_lpstr; }
};


#ifdef SetWindowLongPtrA
#undef SetWindowLongPtrA
inline LONG_PTR SetWindowLongPtrA(
	_In_ HWND hWnd,
	_In_ int nIndex,
	_In_ LONG_PTR dwNewLong)
{
	return( ::SetWindowLongA( hWnd, nIndex, LONG( dwNewLong ) ) );
}
#endif

#ifdef SetWindowLongPtrW
#undef SetWindowLongPtrW
inline LONG_PTR SetWindowLongPtrW(
	_In_ HWND hWnd,
	_In_ int nIndex,
	_In_ LONG_PTR dwNewLong)
{
	return( ::SetWindowLongW( hWnd, nIndex, LONG( dwNewLong ) ) );
}
#endif

#ifdef GetWindowLongPtrA
#undef GetWindowLongPtrA
inline LONG_PTR GetWindowLongPtrA(
	_In_ HWND hWnd,
	_In_ int nIndex)
{
	return( ::GetWindowLongA( hWnd, nIndex ) );
}
#endif

#ifdef GetWindowLongPtrW
#undef GetWindowLongPtrW
inline LONG_PTR GetWindowLongPtrW(
	_In_ HWND hWnd,
	_In_ int nIndex)
{
	return( ::GetWindowLongW( hWnd, nIndex ) );
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
  Size(Size&) = default;
  Size(Size&&) = default;
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
  Rect operator+(const RECT* other) const;
  Rect operator-(const RECT* other) const;
  void SetSize(int CX, int CY);
};



class Point : public tagPOINT {
public:
  Point();
  Point(Point&) = default;
  Point(Point&&) = default;
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
  Rect operator+(const RECT* pother) const;
  Rect operator-(const RECT* pother) const;
};

class Rect : public tagRECT {
public:
  Rect();
  Rect(Rect&) = default;
  Rect(Rect&&) = default;
  Rect(int l, int t, int r, int b);
  Rect(const RECT &r);
  Rect(LPCRECT lpr);
  Rect(POINT p, SIZE s);
  Rect(POINT topLeft, POINT bottomRight );
  int Width() const;
  int Height() const;
  Size Size() const;
  const Point& TopLeft() const;
  const Point& BottomRight() const;
  Point& TopLeft();
  Point& BottomRight();
  Point CenterPoint() const;
  void SwapLeftRight();
  static void WINAPI SwapLeftRight(LPRECT lpRect);
  operator LPRECT();
  operator LPCRECT();
  BOOL IsRectEmpty() const;
  BOOL IsRectNull() const;
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
  void operator=(const RECT& rc);
  BOOL operator==(const RECT& rc) const;
  BOOL operator!=(const RECT& rc) const;
  void operator+=(POINT point);
  void operator+=(SIZE size);
  void operator+=(LPCRECT lpRect);
  void operator-=(POINT point);
  void operator-=(SIZE size);
  void operator-=(LPCRECT lpRect);
  void operator&=(const RECT& rc);
  void operator|=(const RECT& rc);
  Rect operator+(POINT point) const;
  Rect operator-(POINT point) const;
  Rect operator+(LPCRECT lpRect) const;
  Rect operator+(SIZE size) const;
  Rect operator-(SIZE size) const;
  Rect operator-(LPCRECT lpRect) const;
  Rect operator&(const RECT& rc) const;
  Rect operator|(const RECT& rc) const;
  Rect MulDiv(int Multiplier, int Divisor) const;
  
};

Size::Size() { cx = 0; cy = 0; }
Size::Size(LONG x, LONG y) { cx = x; cy = y; }
Size::Size(LPARAM lParam) { cx = GET_X_LPARAM(lParam); cy = GET_Y_LPARAM(lParam); }
Size::Size(SIZE size) { cx = size.cx; cy = size.cy; }
inline BOOL Size::operator==(SIZE other) const { return (cx == other.cx && cy == other.cy); }
inline BOOL Size::operator!=(SIZE other) const { return (cx != other.cx || cy != other.cy); }
inline void Size::operator+=(SIZE other) { cx += other.cx; cy += other.cy; }
inline void Size::operator-=(SIZE other) { cx -= other.cx; cy -= other.cy; }
inline Size Size::operator+(SIZE other)  const { return { cx + other.cx, cy + other.cy}; }
inline Size Size::operator-(SIZE other)  const { return { cx - other.cx, cy - other.cy}; }
inline Size Size::operator-() const { return {-cx, -cy}; }
inline Point Size::operator+(POINT other) const { return Point{ cx + other.x, cy + other.y }; }
inline Point Size::operator-(POINT other) const { return Point{ cx - other.x, cy - other.y }; }
inline Rect Size::operator+(const RECT* other) const { return Rect(other) + *this; }
inline Rect Size::operator-(const RECT* other) const { return Rect(other) - *this; }
inline void Size::SetSize(int CX, int CY) { cx = CX; cy = CY; }

inline Point::Point() { x = 0;  y = 0; }
inline Point::Point(LONG xx, LONG yy){ x = xx; y = yy; }
inline Point::Point(LPARAM lParam){ x = GET_X_LPARAM(lParam); y = GET_Y_LPARAM(lParam); }
inline void Point::Offset(int OffsetX, int OffsetY) { x += OffsetX; y += OffsetY; }
inline void Point::Offset(POINT point) { x += point.x; y += point.y; }
inline void Point::SetPoint(int X, int Y) { x = X; y = Y; }
inline BOOL Point::operator==(POINT other) const { return (x == other.x && y == other.y); }
inline BOOL Point::operator!=(POINT other) const { return (x != other.x || y != other.y); }
inline void Point::operator+=(POINT other) { x += other.x; y += other.y; }
inline void Point::operator-=(POINT other) { x -= other.x; y -= other.y; }
inline Point Point::operator+(SIZE size) const { return Point{x + size.cx, y + size.cy}; }
inline Point Point::operator-(SIZE size) const { return Point{x - size.cx, y - size.cy}; }
inline Point Point::operator-() const { return {-x, -y}; }
inline Point Point::operator+(POINT other) const { return { x + other.x, y + other.y}; }
inline Point Point::operator-(POINT other) const { return {x - other.x, y - other.y}; }
inline Rect Point::operator+(const RECT* pother) const { return Rect{pother} + *this; }
inline Rect Point::operator-(const RECT* pother) const { return Rect{pother} + *this; }

inline Rect::Rect() { left = 0; top =0 ; right = 0; bottom = 0; }

inline Rect::Rect(int l, int t, int r, int b) 
{ left = l; top = t; right = r; bottom = b; }

inline Rect::Rect(const RECT &rc) { ::CopyRect(this,&rc); }

inline Rect::Rect(LPCRECT lpr) { ::CopyRect(this,lpr); }

inline Rect::Rect(POINT p, SIZE s) {
	right = (left = p.y) + s.cx;
	bottom = (top = p.y) + s.cy;
}

inline Rect::Rect(POINT topLeft, POINT bottomRight ){
	left = topLeft.x;
	top = topLeft.y;
	right = bottomRight.x;
	bottom = bottomRight.y;
}

inline int Rect::Width() const{
	return right - left;
}

inline int Rect::Height() const{
	return bottom - top;
}

inline Size Rect::Size() const{
	return {right - left, bottom - top};
}

inline const Point& Rect::TopLeft() const{
	return *((Point*)this);
}

inline const Point& Rect::BottomRight() const{
	return *((Point*)this + 1);
}

inline Point& Rect::TopLeft(){
	return *((Point*)this);
}

inline Point& Rect::BottomRight(){
	return *((Point*)this + 1);
}

inline Point Rect::CenterPoint() const{
	return {(left + right) / 2, (top + bottom)/2 };
}

inline void Rect::SwapLeftRight(){
	SwapLeftRight(LPRECT(this));
}

inline void WINAPI Rect::SwapLeftRight(LPRECT lpRect){
	LONG temp = lpRect->left;
	lpRect->left = lpRect->right;
	lpRect->right = temp;
}

inline Rect::operator LPRECT(){
	return this;
}

inline Rect::operator LPCRECT(){
	return this;
}

inline BOOL Rect::IsRectEmpty() const{
	return ::IsRectEmpty(this);
}

inline BOOL Rect::IsRectNull() const{
	return (left ==0 && right == 0 && top == 0 && bottom == 0);
}

inline BOOL Rect::PtInRect(POINT point) const{
	return ::PtInRect(this,point);
}

inline void Rect::SetRect(int x1, int y1, int x2, int y2){
	::SetRect(this,x1,y1,x2,y2);
}

inline void Rect::SetRect(POINT topLeft, POINT bottomRight){
	::SetRect(this, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
}

inline void Rect::SetRectEmpty(){
	::SetRectEmpty(this);
}

inline void Rect::CopyRect(LPCRECT lpRect){
	::CopyRect(this, lpRect);
}

inline BOOL Rect::EqualRect(LPCRECT lpRect) const {
	return ::EqualRect(this,lpRect);
}

inline void Rect::InflateRect(int x, int y){
	::InflateRect(this,x,y);
}

inline void Rect::InflateRect(SIZE size){
	 ::InflateRect(this,size.cx, size.cy);
}

inline void Rect::InflateRect(LPCRECT lpRect){
	left -= lpRect->left;
	top -= lpRect->top;
	right += lpRect->right;
	bottom += lpRect->bottom;
}

inline void Rect::InflateRect(int l, int t, int r, int b){
    left -= l;
	top -= t;
	right += r;
	bottom += b;
}

inline void Rect::DeflateRect(int x, int y){
	::InflateRect(this, -x, -y);
}

inline void Rect::DeflateRect(SIZE size){
	::InflateRect(this, -size.cx, -size.cy);
}

inline void Rect::DeflateRect(LPCRECT lpRect){
	left += lpRect->left;
	top += lpRect->top;
	right -= lpRect->right;
	bottom -= lpRect->bottom;
}

inline void Rect::DeflateRect(int l, int t, int r, int b){
	left += l;
	top += t;
	right -= r;
	bottom -= b;
}

inline void Rect::OffsetRect(int x, int y){
	::OffsetRect(this, x, y);
}

inline void Rect::OffsetRect(SIZE size){
	::OffsetRect(this, size.cx, size.cy);
}

inline void Rect::OffsetRect(POINT point){
	::OffsetRect(this,point.x, point.y);
}

inline void Rect::NormalizeRect(){
	int tmp;
	if(left > right){
		tmp = left;
		left = right;
		right = tmp;
	}
	if(top > bottom){
		tmp = top;
		top = bottom;
		bottom = tmp;
	}
}

inline void Rect::MoveToY(int y){
	bottom = Height() + y;
	top = y;
}

inline void Rect::MoveToX(int x){
	right = Width() + x;
	left = x;
}

inline void Rect::MoveToXY(int x, int y){
	MoveToX(x);
	MoveToY(y);
}

inline void Rect::MoveToXY(POINT point){
	MoveToX(point.x);
	MoveToY(point.y);
}

inline BOOL Rect::IntersectRect(LPCRECT lpRect1, LPCRECT lpRect2){
	return ::IntersectRect(this,lpRect1, lpRect2);
}

inline BOOL Rect::UnionRect(LPCRECT lpRect1, LPCRECT lpRect2){
	return ::UnionRect(this, lpRect1, lpRect2);
}

inline BOOL Rect::SubtractRect(LPCRECT lpRect1, LPCRECT lpRect2){
	return ::SubtractRect(this,lpRect1, lpRect2);
}

inline void Rect::operator=(const RECT& rc){
		::CopyRect(this,&rc);
}

inline BOOL Rect::operator==(const RECT& rc) const{
	return ::EqualRect(this,&rc);
}

inline BOOL Rect::operator!=(const RECT& rc) const{
	return !::EqualRect(this,&rc);
}

inline void Rect::operator+=(POINT point){
	::OffsetRect(this, point.x,point.y);
}

inline void Rect::operator+=(SIZE size){
	::OffsetRect(this, size.cx, size.cy);
}

inline void Rect::operator+=(LPCRECT lpRect){
	InflateRect(lpRect);
}

inline void Rect::operator-=(POINT point){
	::OffsetRect(this,-point.x, -point.y);
}

inline void Rect::operator-=(SIZE size){
	::OffsetRect(this,-size.cx, -size.cy);
}

inline void Rect::operator-=(LPCRECT lpRect){
	DeflateRect(lpRect);
}

inline void Rect::operator&=(const RECT& rc){
	::IntersectRect(this,this,&rc);
}

inline void Rect::operator|=(const RECT& rc){
	::UnionRect(this,this,&rc);
}

inline Rect Rect::operator+(POINT point) const{
	Rect rc(*this);
	::OffsetRect(&rc,point.x, point.y);
	return rc;
}

inline Rect Rect::operator-(POINT point) const{
	Rect rc(*this);
	::OffsetRect(&rc,-point.x, -point.y);
	return rc;
}

inline Rect Rect::operator+(LPCRECT lpRect) const{
	Rect rc(this);
	rc.InflateRect(lpRect);
	return rc;
}

inline Rect Rect::operator+(SIZE size) const{
	Rect rc(*this);
	::OffsetRect(&rc, size.cx, size.cy);
	return rc;
}

inline Rect Rect::operator-(SIZE size) const{
	Rect rc(*this);
	::OffsetRect(&rc, -size.cx, -size.cy);
	return rc;
}

inline Rect Rect::operator-(LPCRECT lpRect) const{
	Rect rc(this);
	rc.DeflateRect(lpRect);
	return rc;
}

inline Rect Rect::operator&(const RECT& rc) const{
	Rect rcn;
	::IntersectRect(&rcn,this,&rc);
	return rcn;
}

inline Rect Rect::operator|(const RECT& rc) const{
	Rect rcn;
	::UnionRect(&rcn, this,&rc);
	return rcn;
}

inline Rect Rect::MulDiv(int Multiplier, int Divisor) const{
	return { ::MulDiv(left, Multiplier, Divisor),
			 ::MulDiv(top , Multiplier, Divisor),
			 ::MulDiv(right, Multiplier,Divisor),
			 ::MulDiv(bottom, Multiplier, Divisor)};
}


inline void WINCHECK(HWND hWnd)
{
	if(!::IsWindow(hWnd))
	{
		MessageBox(nullptr,
		TEXT("Function Called on an Improperly Created Window Class. Ensure that Window::Create is called, and succeeds to prevent this assertion."),
		TEXT("Runtime Assertion! Invalid Window Handle (HWND)"),MB_OK | MB_ICONERROR);
		ExitProcess(ERROR_INVALID_WINDOW_HANDLE);
	}
}

inline void WINASSERT(bool val)
{
	if(!val)
	{
		MessageBox(nullptr,
		TEXT("Debugging Assertion FAILED!"),
		TEXT("Runtime Assertion!"),MB_OK | MB_ICONERROR);
		ExitProcess(ERROR_INVALID_WINDOW_HANDLE);
	}
}

inline void WINTRACE(bool val,LPTSTR text)
{
	if(!val)
	{
		MessageBox(nullptr,
		text,
		TEXT("Runtime Trace: "),MB_OK | MB_ICONINFORMATION);
	}
}

#define DECLARE_WND_CLASS(WndClassName, style, bkgrnd) \
static WNDCLASSEX GetWinClassInfo() \
{ \
	static WNDCLASSEX wc = \
	{ \
		sizeof(WNDCLASSEX),\
		style,\
		WindowProc,\
		0,\
		0, \
		HINST_THISCOMPONENT,\
		(HICON)::LoadIcon(nullptr,IDI_WINLOGO),\
		(HCURSOR)::LoadCursor(nullptr,IDC_ARROW),\
		(HBRUSH)(bkgrnd + 1), \
		nullptr,\
		WndClassName,\
		nullptr \
	}; \
	return wc; \
} \
static LPTSTR GetWinClassName() { return WndClassName;}

#define DECLARE_WND_CLASS_1(WndClassName) \
static WNDCLASSEX GetWinClassInfo() \
{ \
	static WNDCLASSEX wc = \
	{ \
		sizeof(WNDCLASSEX),\
		CS_HREDRAW | CS_VREDRAW,\
		WindowProc,\
		0,\
		0, \
		HINST_THISCOMPONENT,\
		(HICON)::LoadIcon(nullptr,IDI_APPLICATION),\
		(HCURSOR)::LoadCursor(nullptr,IDC_ARROW),\
		(HBRUSH)(COLOR_WINDOW + 1), \
		nullptr,\
		WndClassName,\
		nullptr \
	}; \
	return wc; \
}\
static LPTSTR GetWinClassName() { return WndClassName; }

class Window
{


public:
	static RECT rcDefault;
	HWND m_hwnd;
	Window(HWND hWnd = nullptr) : m_hwnd(hWnd){}
	
	Window(Window& other) = default;
	Window(Window&& other) = default;
	
	Window& operator=(HWND hWnd)
	{
		m_hwnd = hWnd;
		return *this;
	}
	
	Window& operator=(Window&) = default;
	Window& operator=(Window&&) = default;
	
	static LPCTSTR GetWndClassName()
	{
		return nullptr;
	}
	
	void Attach(HWND hWndNew = nullptr)
	{
		m_hwnd = hWndNew;
	}
	
	HWND Detach()
	{
		HWND hWnd = m_hwnd;
		m_hwnd = nullptr;
		return hWnd;
	}
	
	HWND Create(
	LPCTSTR lpstrWndClass = nullptr,
	HWND hWndParant = nullptr,
	RECT rect = {0,0,0,0},
	LPCTSTR szWindowName = nullptr,
	DWORD dwStyle = 0,
	DWORD dwExStyle = 0,
	HMENU menu = nullptr,
	LPVOID lpCreateParam = nullptr)
	{
		
		m_hwnd = ::CreateWindowEx(dwExStyle,lpstrWndClass,szWindowName,
		dwStyle,rect.left,rect.top,rect.right - rect.left, rect.bottom - rect.top,
		hWndParant,menu,HINST_THISCOMPONENT,lpCreateParam);
		return m_hwnd;
	}
	
	BOOL DestroyWindow()
	{	
		WINCHECK(m_hwnd);
		if(!::DestroyWindow(m_hwnd))
			return FALSE;
			
		m_hwnd = nullptr;
		return S_OK;
	}
	
	operator HWND() const{ return m_hwnd; }
	
	DWORD GetStyle() const
	{
		WINCHECK(m_hwnd);
		return (DWORD) ::GetWindowLong(m_hwnd,GWL_STYLE);
	}
	
	DWORD GetExStyle() const
	{
		WINCHECK(m_hwnd);
		return (DWORD) ::GetWindowLong(m_hwnd, GWL_EXSTYLE);
	}
	
	LONG GetWindowLong(int nIndex) const
	{
		WINCHECK(m_hwnd);
		return ::GetWindowLong(m_hwnd, nIndex);
	}
	
	LONG_PTR GetWindowLongPtr(int nIndex) const
	{
		WINCHECK(m_hwnd);
		return ::GetWindowLongPtr(m_hwnd,nIndex);
	}

	LONG SetWindowLong(int nIndex, LONG dwNewLong)
	{
		WINCHECK(m_hwnd);
		return ::SetWindowLong(m_hwnd,nIndex,dwNewLong);
	}
		
	HRESULT SetWindowLong(int nIndex, LONG dwNewLong, LONG& oldLong)
	{
		WINCHECK(m_hwnd);
		oldLong = ::SetWindowLong(m_hwnd,nIndex,dwNewLong);
		if(oldLong == 0){
			return __HRESULT_FROM_WIN32(GetLastError());
		}else{
			return S_OK;
		}
	}

	LONG_PTR SetWindowLongPtr(int nIndex, LONG_PTR dwNewLong)
	{
		WINCHECK(m_hwnd);
		return ::SetWindowLongPtr(m_hwnd,nIndex,dwNewLong);
	}
	
	LRESULT SendMessage(UINT msg, WPARAM wParam = 0, LPARAM lParam = 0)
	{		
		WINCHECK(m_hwnd);
		return ::SendMessage(m_hwnd,msg,wParam,lParam);
	}
	
	BOOL PostMessage(UINT msg, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		WINCHECK(m_hwnd);
		return ::PostMessage(m_hwnd,msg, wParam,lParam);
	}
	
	BOOL SendNotifyMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		 WINCHECK(m_hwnd);
		 return ::SendNotifyMessage(m_hwnd,msg,wParam,lParam);
	}
	
	BOOL SetWindowText(LPCTSTR lpszString)
	{	
		WINCHECK(m_hwnd);
		return ::SetWindowText(m_hwnd,lpszString);
	}
	
	int GetWindowTextLength() const
	{
		WINCHECK(m_hwnd);
		return ::GetWindowTextLength(m_hwnd);
	}
	
	int GetWindowText(LPTSTR lpszString, int BuffLen)
	{
		WINCHECK(m_hwnd);
		return ::GetWindowText(m_hwnd,lpszString,BuffLen);
	}
	
	HRESULT GetWindowText(std::tstring& str)
	{
		HRESULT hr = S_OK;
		int txtlen = GetWindowTextLength();
			try
			{
				if(txtlen + 1 <= str.max_size())
				{
					str.resize(txtlen + 1);
				}
				else
				{
					str.resize(str.max_size());
				}
				
				int got = ::GetWindowText(m_hwnd,&str[0],str.length());
				if(got == 0)
				{
					int errval = GetLastError();
					if(errval == 0)
						return S_OK;
					else
						return __HRESULT_FROM_WIN32(errval);
				}
				return S_OK;
			}
			catch(std::bad_alloc e)
			{
				return __HRESULT_FROM_WIN32(ERROR_OUTOFMEMORY);
			}
			return S_OK;
	}
	
	void SetFont(HFONT hFont, BOOL bRedraw = TRUE)
	{
		WINCHECK(m_hwnd);
		::SendMessage(m_hwnd,WM_SETFONT,reinterpret_cast<WPARAM>(hFont),
		MAKELPARAM(bRedraw,0)); 
	}
	
	HFONT GetFont() const
	{
		WINCHECK(m_hwnd);
		return reinterpret_cast<HFONT>(::SendMessage(m_hwnd,WM_GETFONT,0,0));
	}
	
	HMENU GetMenu() const
	{
		WINCHECK(m_hwnd);
		return reinterpret_cast<HMENU>(::GetMenu(m_hwnd));
	}
	
	BOOL SetMenu(HMENU hMenu)
	{
		WINCHECK(m_hwnd);
		return ::SetMenu(m_hwnd, hMenu);
	}
	
	BOOL DrawMenuBar()
	{
		WINCHECK(m_hwnd);
		return ::DrawMenuBar(m_hwnd);
	}
	
	HMENU GetSystemMenu(BOOL bRevert) const
	{
		WINCHECK(m_hwnd);
		return reinterpret_cast<HMENU>(::GetSystemMenu(m_hwnd,bRevert));
	}
	
	BOOL HiliteMenuItem(HMENU hMenu,UINT uItemHilite,UINT uHilite)
	{
		WINCHECK(m_hwnd);
		return ::HiliteMenuItem(m_hwnd,hMenu,uItemHilite,uHilite);
	}
	
	BOOL IsIconic() const
	{
		WINCHECK(m_hwnd);
		return ::IsIconic(m_hwnd);
	}
	
	BOOL IsZoomed() const
	{
		WINCHECK(m_hwnd);
		return ::IsZoomed(m_hwnd);
	}
	
	BOOL MoveWindow( 
	int x,
	int y,
	int nWidth,
	int nHeight,
	BOOL bRepaint = TRUE)
	{
		WINCHECK(m_hwnd);
		return ::MoveWindow(m_hwnd,x,y,nWidth,nHeight,bRepaint);
	}
	
	BOOL SetWindowPos( 
	HWND hWndInsertAfter,
	int x,
	int y,
	int cx,
	int cy,
	UINT nFlags)
	{
		WINCHECK(m_hwnd);
		return ::SetWindowPos(m_hwnd,hWndInsertAfter,x,y,cx,cy,nFlags);
	}
	
	BOOL SetWindowPos(
	HWND hWndInsertAfter,
	LPCRECT lpRect,
	UINT nFlags)
	{
		WINCHECK(m_hwnd);
		return ::SetWindowPos(m_hwnd,hWndInsertAfter,lpRect->left,lpRect->top,
		lpRect->right - lpRect->left, lpRect->bottom - lpRect->top,nFlags);
	}
	
	UINT ArrangeIconicWindows()
	{
		WINCHECK(m_hwnd);
		return ::ArrangeIconicWindows(m_hwnd);
	}
	
	BOOL BringWindowToTop()
	{
		WINCHECK(m_hwnd);
		return ::BringWindowToTop(m_hwnd);
	}
	
	BOOL GetWindowRect(LPRECT lpRect) const
	{
		WINCHECK(m_hwnd);
		return ::GetWindowRect(m_hwnd, lpRect);
	}
	
	BOOL GetClientRect(LPRECT lpRect) const
	{
		WINCHECK(m_hwnd);
		return ::GetClientRect(m_hwnd, lpRect);
	}
	
	BOOL GetWindowPlacement(WINDOWPLACEMENT *lpwndpl)
	{
		WINCHECK(m_hwnd);
		return ::GetWindowPlacement(m_hwnd,lpwndpl);
	}
	
	BOOL SetWindowPlacement(WINDOWPLACEMENT *lpwndpl)
	{
		WINCHECK(m_hwnd);
		return ::SetWindowPlacement(m_hwnd,lpwndpl);
	}
	
	BOOL ClientToScreen(LPPOINT lpPoint) const
	{
		WINCHECK(m_hwnd);
		return ::ClientToScreen(m_hwnd,lpPoint);
	}
	
	BOOL ClientToScreen(LPRECT lpRect) const
	{
		WINCHECK(m_hwnd);
		if(!::ClientToScreen(m_hwnd,(LPPOINT)lpRect))
			return FALSE;
		return ::ClientToScreen(m_hwnd,((LPPOINT)lpRect) + 1);
	}
	
	BOOL ScreenToClient(LPPOINT lpPoint) const
	{
		WINCHECK(m_hwnd);
		return ::ScreenToClient(m_hwnd, lpPoint);
	}
	
	BOOL ScreenToClient(LPRECT lpRect) const
	{
		WINCHECK(m_hwnd);
		if(!::ScreenToClient(m_hwnd,(LPPOINT)lpRect))
			return FALSE;
			
		return ::ScreenToClient(m_hwnd,((LPPOINT)lpRect) + 1);
	}
	
	int MapWindowPoints( HWND hWndTo, LPPOINT lpPoint, UINT nCount) const
	{
		WINCHECK(m_hwnd);
		return ::MapWindowPoints(m_hwnd,hWndTo,lpPoint,nCount);
	}
	
	int MapWindowPoints( HWND hWndTo, LPRECT lpRect)
	{
		WINCHECK(m_hwnd);
		return ::MapWindowPoints(m_hwnd,hWndTo,(LPPOINT)lpRect,2);
	}
	
	HDC BeginPaint(LPPAINTSTRUCT lpPaint)
	{
		WINCHECK(m_hwnd);
		return ::BeginPaint(m_hwnd,lpPaint);
	}
	
	void EndPaint(LPPAINTSTRUCT lpPaint)
	{
		WINCHECK(m_hwnd);
		::EndPaint(m_hwnd, lpPaint);
	}
	
	HDC GetDC()
	{
		WINCHECK(m_hwnd);
		return ::GetDC(m_hwnd);
	}
	
	HDC GetWindowDC()
	{
		WINCHECK(m_hwnd);
		return ::GetWindowDC(m_hwnd);
	}
	
	int ReleaseDC(HDC hDC)
	{
		WINCHECK(m_hwnd);
		return ::ReleaseDC(m_hwnd,hDC);
	}
	
	void Print(HDC hDC, DWORD dwFlags) const
	{
		WINCHECK(m_hwnd);
		::SendMessage(m_hwnd, WM_PRINT, (WPARAM)hDC, dwFlags);
	}
	
	void PrintClient(HDC hDC, DWORD dwFlags)
	{
		WINCHECK(m_hwnd);
		::SendMessage(m_hwnd, WM_PRINTCLIENT, (WPARAM)hDC, dwFlags);
	}
	
	BOOL UpdateWindow()
	{
		WINCHECK(m_hwnd);
		return ::UpdateWindow(m_hwnd);
	}
	
	void SetRedraw(BOOL bRedraw = TRUE)
	{
		WINCHECK(m_hwnd);
		::SendMessage(m_hwnd, WM_SETREDRAW, (WPARAM)bRedraw, 0);
	}
	
	BOOL GetUpdateRect(LPRECT lpRect, BOOL bErase = FALSE)
	{
		WINCHECK(m_hwnd);
		return ::GetUpdateRect(m_hwnd, lpRect, bErase);
	}
	
	int GetUpdateRgn(HRGN hRgn, BOOL bErase = FALSE)
	{
		WINCHECK(m_hwnd);
		return ::GetUpdateRgn(m_hwnd, hRgn, bErase);
	}
	
	BOOL Invalidate(BOOL bErase = TRUE)
	{
		WINCHECK(m_hwnd);
		return ::InvalidateRect(m_hwnd, nullptr, bErase);
	}
	
	BOOL InvalidateRect(LPCRECT lpRect, BOOL bErase = TRUE)
	{
		WINCHECK(m_hwnd);
		return ::InvalidateRect(m_hwnd, lpRect, bErase);
	}
	
	BOOL ValidateRect(LPCRECT lpRect = nullptr)
	{
		WINCHECK(m_hwnd);
		return ::ValidateRect(m_hwnd, lpRect);
	}
	
	void InvalidateRgn(HRGN hRgn, BOOL bErase = TRUE)
	{
		WINCHECK(m_hwnd);
		::InvalidateRgn(m_hwnd, hRgn, bErase);
	}
	
	BOOL ValidateRgn(HRGN hRgn = nullptr)
	{
		WINCHECK(m_hwnd);
		return ::ValidateRgn(m_hwnd, hRgn);
	}
	
	BOOL ShowWindow(int nCmdShow)
	{
		WINCHECK(m_hwnd);
		return ::ShowWindow(m_hwnd, nCmdShow);
	}
	
	BOOL IsWindowVisible() const
	{
		WINCHECK(m_hwnd);
		return ::IsWindowVisible(m_hwnd);
	}
	
	BOOL ShowOwnedPopups(BOOL bShow = TRUE)
	{
		WINCHECK(m_hwnd);
		return ::ShowOwnedPopups(m_hwnd, bShow);
	}
	
	HDC GetDCEx(HRGN hRgnClip, DWORD flags)
	{
		WINCHECK(m_hwnd);
		return ::GetDCEx(m_hwnd, hRgnClip, flags);
	}
	
	BOOL LockWindowUpdate(BOOL bLock = TRUE)
	{
		WINCHECK(m_hwnd);
		return ::LockWindowUpdate(bLock ? m_hwnd : nullptr);
	}
	
	BOOL RedrawWindow(LPCRECT lpRectUpdate = nullptr, 
		HRGN hRgnUpdate = nullptr, 
		UINT flags = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE)
		{
			WINCHECK(m_hwnd);
			return ::RedrawWindow(m_hwnd, lpRectUpdate, hRgnUpdate, flags);
		}
	
	UINT_PTR SetTimer(
	UINT_PTR nIDEvent,
	UINT nElapse,
	void (CALLBACK* lpfnTimer)(HWND, UINT, UINT_PTR,DWORD) = nullptr)
	{
		WINCHECK(m_hwnd);
		return ::SetTimer(m_hwnd, nIDEvent, nElapse, (TIMERPROC)lpfnTimer);
	}
	
	BOOL KillTimer(UINT_PTR nIDEvent)
	{
		WINCHECK(m_hwnd);
		return ::KillTimer(m_hwnd, nIDEvent);
	}
	
	BOOL IsWindowEnabled() const
	{
		WINCHECK(m_hwnd);
		return ::IsWindowEnabled(m_hwnd);
	}
	
	BOOL EnableWindow(BOOL Enable = TRUE)
	{
		WINCHECK(m_hwnd);
		return ::EnableWindow(m_hwnd, Enable);
	}
	
	HWND SetActiveWindow()
	{
		WINCHECK(m_hwnd);
		return ::SetActiveWindow(m_hwnd);
	}
	
	HWND SetCapture()
	{
		WINCHECK(m_hwnd);
		return ::SetCapture(m_hwnd);
	}
	
	HWND SetFocus()
	{
		WINCHECK(m_hwnd);
		return ::SetFocus(m_hwnd);
	}
	
	BOOL CheckDlgButton(int nIDButton, UINT nCheck)
	{
		WINCHECK(m_hwnd);
		return ::CheckDlgButton(m_hwnd, nIDButton, nCheck);
	}
	
	BOOL CheckRadioButton(
	int nIDFirstButton, 
	int nIDLastButton,
	int nIDCheckButton)
	{
		WINCHECK(m_hwnd);
		return ::CheckRadioButton(m_hwnd, nIDFirstButton, nIDLastButton, 
		nIDCheckButton);
	}
	
	int DlgDirList(
	LPTSTR lpPathSpec,
	int nIDListBox,
	int nIDStaticPath,
	UINT nFileType)
	{
		WINCHECK(m_hwnd);
		return ::DlgDirList(m_hwnd,lpPathSpec, nIDListBox, nIDStaticPath, nFileType);
	}
	
	int DlgDirListComboBox(
	LPTSTR lpPathSpec,
	int nIDComboBox,
	int nIDStaticPath,
	UINT nFileType)
	{
		WINCHECK(m_hwnd);
		return ::DlgDirListComboBox(m_hwnd,lpPathSpec,nIDComboBox, nIDStaticPath, nFileType);
	}
	
	UINT GetDlgItemInt(
	int nID,
	BOOL* lpTrans = nullptr,
	BOOL bSigned = TRUE) const
	{
		WINCHECK(m_hwnd);
		return ::GetDlgItemInt(m_hwnd, nID, lpTrans, bSigned);
	}
	
	UINT GetDlgItemText(
	int nID,
	LPTSTR lpStr,
	int MaxCount) const throw()
	{
		WINCHECK(m_hwnd);
		return ::GetDlgItemText(m_hwnd, nID, lpStr, MaxCount) + 1;
	}
	
	UINT GetDlgItemText( int nID, std::tstring& str)
	{
		HWND hItem = GetDlgItem(nID);
		if(hItem != nullptr)
		{
			str.resize(::GetWindowTextLength(hItem) + 1);
			::GetWindowText(hItem, &str[0], str.length());
			return str.length();
		}
		else
		{
			str = TEXT("");
			return 0;
		}
	}
	
	Window GetNextDlgGroupItem( HWND hWndCtl, BOOL bPrevious = FALSE) const
	{
		WINCHECK(m_hwnd);
		return Window(::GetNextDlgGroupItem(m_hwnd, hWndCtl, bPrevious));
	}
	
	Window GetNextDlgTabItem(
	HWND hWndCtrl,
	BOOL bPrevious = FALSE) const
	{
		WINCHECK(m_hwnd);
		return Window(::GetNextDlgTabItem(m_hwnd, hWndCtrl, bPrevious));
	}
	
	UINT IsDlgButtonChecked(int nIDButton) const
	{
		WINCHECK(m_hwnd);
		return ::IsDlgButtonChecked(m_hwnd, nIDButton);
	}
	
	LRESULT SendDlgItemMessage(
	int nID,
	UINT msg, 
	WPARAM wParam = 0,
	LPARAM lParam = 0)
	{
		WINCHECK(m_hwnd);
		return ::SendDlgItemMessage(m_hwnd, nID, msg, wParam, lParam);
	}
	
	int GetScrollPos(int nBar) const
	{
		WINCHECK(m_hwnd);
		return ::GetScrollPos(m_hwnd,nBar);
	}
	
	BOOL GetScrollRange( int nBar, LPINT lpMinPos, LPINT lpMaxPos)
	{
		WINCHECK(m_hwnd);
		return ::GetScrollRange(m_hwnd, nBar, lpMinPos, lpMaxPos);
	}
	
	BOOL ScrollWindow( int xAmount, int yAmount, LPCRECT lpRect = nullptr,
	LPCRECT lpClipRect = nullptr)
	{
		WINCHECK(m_hwnd);
		return ::ScrollWindow(m_hwnd, xAmount, yAmount, lpRect, lpClipRect);
	}
	
	int ScrollWindowEx(
	int dx,
	int dy,
	LPCRECT lpRectScroll = nullptr,
	LPCRECT lpRectClip = nullptr,
	HRGN hRgnUpdate = nullptr,
	LPRECT lpRectUpdate = nullptr,
	UINT uFlags = 0)
	{
		WINCHECK(m_hwnd);
		return ::ScrollWindowEx(m_hwnd,dx,dy,lpRectScroll,lpRectClip,hRgnUpdate, lpRectUpdate, uFlags);
	}
	
	int ScrollWindowEx(
	int dx,
	int dy,
	UINT uFlags,
	LPCRECT lpRectScroll = nullptr,
	LPCRECT lpRectClip = nullptr,
	HRGN hRgnUpdate = nullptr,
	LPRECT lpRectUpdate = nullptr)
	{	
		WINCHECK(m_hwnd);
		return ::ScrollWindowEx(m_hwnd, dx, dy, lpRectScroll, lpRectClip, hRgnUpdate, lpRectUpdate, uFlags);
	}
	
	int SetScrollPos(int nBar, int nPos, BOOL bRedraw = TRUE)
	{
		WINCHECK(m_hwnd);
		return ::SetScrollPos(m_hwnd, nBar, nPos, bRedraw);
	}
	
	BOOL SetScrollRange( int nBar, int nMinPos, int nMaxPos, BOOL bRedraw = TRUE)
	{
		WINCHECK(m_hwnd);
		return ::SetScrollRange(m_hwnd, nBar, nMinPos, nMaxPos,bRedraw );
	}
	
	BOOL ShowScrollBar(UINT nBar, BOOL bShow = TRUE)
	{
		WINCHECK(m_hwnd);
		return ::ShowScrollBar(m_hwnd, nBar, bShow);
	}
	
	BOOL EnableScrollBar(
	UINT uSBFlags, 
	UINT uArrowFlags = ESB_ENABLE_BOTH)
	{
		WINCHECK(m_hwnd);
		return ::EnableScrollBar(m_hwnd, uSBFlags, uArrowFlags);
	}
	
	Window ChildWindowFromPoint(POINT point)
	{
		WINCHECK(m_hwnd);
		return Window(::ChildWindowFromPoint(m_hwnd,point));
	}
	
	Window ChildWindowFromPointEx(POINT point, UINT uFlags) const
	{
		WINCHECK(m_hwnd);
		return Window(::ChildWindowFromPointEx(m_hwnd, point,uFlags));
	}
	
	Window GetTopWindow() const
	{
		WINCHECK(m_hwnd);
		return Window(::GetTopWindow(m_hwnd));
	}
	
	Window GetWindow(UINT nCmd) const
	{
		WINCHECK(m_hwnd);
		return Window(::GetWindow(m_hwnd,nCmd));
	}
	
	Window GetLastActivePopup() const
	{
		WINCHECK(m_hwnd);
		return Window(::GetLastActivePopup(m_hwnd));
	}
	
	BOOL IsChild(HWND hWnd) const
	{
		WINCHECK(m_hwnd);
		return ::IsChild(m_hwnd, hWnd);
	}
	
	Window GetParent() const
	{
		WINCHECK(m_hwnd);
		return Window(::GetParent(m_hwnd));
	}
	
	Window SetParent(HWND hWndNewParent)
	{
		WINCHECK(m_hwnd);
		return Window(::SetParent(m_hwnd, hWndNewParent));
	}
	
	int GetDlgCtrlID() const
	{
		WINCHECK(m_hwnd);
		return ::GetDlgCtrlID(m_hwnd);
	}
	
	int SetDlgCtrlID(int nID)
	{
		WINCHECK(m_hwnd);
		return (int)::SetWindowLong(m_hwnd,GWL_ID,nID);
	}
	
	Window GetDlgItem(int nID) const
	{
		WINCHECK(m_hwnd);
		return Window(::GetDlgItem(m_hwnd, nID));
	}
	
	BOOL FlashWindow(BOOL bInvert)
	{
		WINCHECK(m_hwnd);
		return ::FlashWindow(m_hwnd, bInvert);
	}
	
	int MessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = TEXT(""),
	UINT nType = MB_OK)
	{
		WINCHECK(m_hwnd);
		return ::MessageBox(m_hwnd, lpszText, lpszCaption, nType);
	}
	
	BOOL ChangeClipboardChain(HWND hwndNext)
	{
		WINCHECK(m_hwnd);
		return ::ChangeClipboardChain(m_hwnd, hwndNext);
	}
	
	HWND SetClipboardViewer()
	{
		WINCHECK(m_hwnd);
		return ::SetClipboardViewer(m_hwnd);
	}
	
	BOOL OpenClipboard()
	{
		WINCHECK(m_hwnd);
		return ::OpenClipboard(m_hwnd);
	}
	
	BOOL CreateCaret(HBITMAP hBitmap)
	{
		WINCHECK(m_hwnd);
		return ::CreateCaret(m_hwnd, hBitmap, 0,0);
	}
	
	BOOL CreateSolidCaret(int nWidth , int nHeight)
	{
		WINCHECK(m_hwnd);
		return ::CreateCaret(m_hwnd, (HBITMAP)0, nWidth, nHeight);
	}
	
	BOOL CreateGrayCaret(int nWidth, int nHeight)
	{
		WINCHECK(m_hwnd);
		return ::CreateCaret(m_hwnd, (HBITMAP)1, nWidth,nHeight);
	}
	
	BOOL HideCaret()
	{
		WINCHECK(m_hwnd);
		return ::HideCaret(m_hwnd);
	}
	
	BOOL ShowCaret()
	{
		WINCHECK(m_hwnd);
		return ::ShowCaret(m_hwnd);
	}
	
	HICON SetIcon( HICON hIcon, BOOL bBigIcon = TRUE)
	{
		WINCHECK(m_hwnd);
		return reinterpret_cast<HICON>(::SendMessage(m_hwnd, WM_SETICON,
		bBigIcon, (LPARAM)hIcon));
	}
	
	HICON GetIcon(BOOL bBigIcon = TRUE) const
	{
		WINCHECK(m_hwnd);
		return reinterpret_cast<HICON>(::SendMessage(m_hwnd, WM_GETICON,bBigIcon, 0 ));
	}
	
	BOOL WinHelp(LPCTSTR lpzHelp, UINT cmd = HELP_CONTEXT, DWORD dwData = 0)
	{
		WINCHECK(m_hwnd);
		return ::WinHelp(m_hwnd, lpzHelp, cmd, dwData);
	}
	
	BOOL SetWindowContextHelpId(DWORD dwContextId)
	{
		WINCHECK(m_hwnd);
		return ::SetWindowContextHelpId(m_hwnd, dwContextId);
	}
	
	DWORD GetWindowContextHelpId()
	{
		WINCHECK(m_hwnd);
		return ::GetWindowContextHelpId(m_hwnd);
	}
	
	int SetHotKey(WORD wVirtualKeyCode, WORD wModifiers)
	{
		WINCHECK(m_hwnd);
		return (int)::SendMessage(m_hwnd, WM_SETHOTKEY,MAKEWORD(wVirtualKeyCode,wModifiers), 0);
	}
	
	DWORD GetHotKey() const
	{
		WINCHECK(m_hwnd);
		return (DWORD)::SendMessage(m_hwnd, WM_GETHOTKEY, 0, 0);
	}
	
	BOOL GetScrollInfo(int nBar,LPSCROLLINFO lpScrollInfo)
	{
		WINCHECK(m_hwnd);
		return ::GetScrollInfo(m_hwnd, nBar, lpScrollInfo);
	}
	
	int SetScrollInfo(int nBar,LPSCROLLINFO lpScrollInfo,BOOL bRedraw = TRUE)
	{
		WINCHECK(m_hwnd);
		return ::SetScrollInfo(m_hwnd, nBar, lpScrollInfo, bRedraw);
	}
	
	BOOL IsDialogMessage(LPMSG lpMsg)
	{
		WINCHECK(m_hwnd);
		return ::IsDialogMessage(m_hwnd, lpMsg);
	}
	
	void NextDlgCtrl() const
	{
		WINCHECK(m_hwnd);
		::SendMessage(m_hwnd, WM_NEXTDLGCTL, 0, 0L);
	}
	
	void PrevDlgCtrl() const
	{
		WINCHECK(m_hwnd);
		::SendMessage(m_hwnd, WM_NEXTDLGCTL, 1, 0L);
	}
	
	void GotoDlgCtrl(HWND hWndCtrl) const
	{
		WINCHECK(m_hwnd);
		::SendMessage(m_hwnd, WM_NEXTDLGCTL, (WPARAM)hWndCtrl, 1L );
	}
	
	BOOL ResizeClient( int Width, int Height, BOOL redraw = TRUE)
	{
		WINCHECK(m_hwnd);
		RECT rcWnd;
		if(!GetClientRect(&rcWnd))
			return FALSE;
			
		if(Width != -1);
			rcWnd.right = Width;
		if(Height != -1);
			rcWnd.bottom = Height;
			
		if(!::AdjustWindowRectEx(&rcWnd, GetStyle(), (!(GetStyle() & WS_CHILD) && (GetMenu() != NULL)), GetExStyle()))
			return FALSE;
		
		UINT uFlags = SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE;
		if(!redraw)
			uFlags |= SWP_NOREDRAW;
	
		return SetWindowPos(nullptr, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top, uFlags);
	}
	
	int GetWindowRgn(HRGN hRgn)
	{
		WINCHECK(m_hwnd);
		return ::GetWindowRgn(m_hwnd, hRgn);
	}
	
	int SetWindowRgn( HRGN hRgn = nullptr, BOOL redraw = FALSE)
	{
		WINCHECK(m_hwnd);
		return ::SetWindowRgn(m_hwnd, hRgn, redraw);
	}
	
	HDWP DeferWindowPos(
		HDWP hWinPosInfo,
		HWND hWndInsertAfter,
		int x, 
		int y, 
		int cx, 
		int cy, 
		UINT uFlags)
	{
		WINCHECK(m_hwnd);
		return ::DeferWindowPos(hWinPosInfo, m_hwnd, hWndInsertAfter, x, y, cx, cy, uFlags);
	}
	
	DWORD GetWindowThreadID()
	{
		WINCHECK(m_hwnd);
		return ::GetWindowThreadProcessId(m_hwnd, nullptr);
	}
	
	DWORD GetWindowProcessID()
	{
		WINCHECK(m_hwnd);
		DWORD dwProcessID;
		::GetWindowThreadProcessId(m_hwnd, &dwProcessID);
		return dwProcessID;
	}
	
	BOOL IsWindow() const
	{
		return ::IsWindow(m_hwnd);
	}
	
	BOOL IsWindowUnicode() const
	{
		WINCHECK(m_hwnd);
		return ::IsWindowUnicode(m_hwnd);
	}
	
	BOOL IsParentDialog()
	{
		WINCHECK(m_hwnd);
		TCHAR szBuff[8];
		if(GetClassName(GetParent(), szBuff, sizeof(szBuff)/sizeof(szBuff[0])) == 0)
			return FALSE;
		
		return lstrcmp(szBuff, TEXT("#32770")) == 0;
	}
	
	BOOL ShowWindowAsync(int nCmdShow)
	{
		WINCHECK(m_hwnd);
		return ::ShowWindowAsync(m_hwnd, nCmdShow);
	}
	
	Window GetDecendantWindow(int nID) const
	{
		WINCHECK(m_hwnd);
		// GetDlgItem recursive (return first found)
		// breadth-first for 1 level, then depth-first for successive levels
		
		//if it's a dialog, use the fast USER functions.
		HWND hWndChild, hWndTmp;
		if((hWndChild = ::GetDlgItem(m_hwnd,nID)) != nullptr)
		{
			if(::GetTopWindow(hWndChild) != NULL)
			{
			
				Window wnd(hWndChild);
				hWndTmp = wnd.GetDecendantWindow(nID);
				if(hWndTmp != nullptr)
					return Window(hWndTmp);
			}
			return Window(hWndChild);
		}
		
		//linear search for the ID.
		for(hWndChild = ::GetTopWindow(m_hwnd); hWndChild != nullptr;
		    hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
			{
				Window wnd(hWndChild);
				hWndTmp = wnd.GetDecendantWindow(nID);
				if(hWndTmp != nullptr)
					return Window(hWndTmp);
			}
			
		return Window(nullptr);
	}
	
	void SendMessageToDescendants(
			UINT message, 
			WPARAM wParam  = 0,
			LPARAM lParam = 0,
			BOOL bDeep = TRUE)
	{
		WINCHECK(m_hwnd);
		for(HWND hWndChild = ::GetTopWindow(m_hwnd); hWndChild != nullptr;
			hWndChild = ::GetNextWindow(hWndChild,  GW_HWNDNEXT))
			{
				::SendMessage(hWndChild, message, wParam, lParam);
				
				if(bDeep && ::GetTopWindow(hWndChild) != nullptr)
				{
					Window wnd(hWndChild);
					wnd.SendMessageToDescendants(message, wParam, lParam, bDeep);
				}
			}
	}
	
	BOOL CenterWindow(HWND hWndCenter = nullptr)
	{
		WINCHECK(m_hwnd);
		DWORD dwStyle = GetStyle();
		if(hWndCenter == nullptr)
		{
			if(dwStyle & WS_CHILD)
				hWndCenter = ::GetParent(m_hwnd);
			else	
				hWndCenter = ::GetWindow(m_hwnd, GW_OWNER);
		}
		
		RECT rcDlg;
		RECT rcArea;
		RECT rcCenter;
		HWND hWndParent;
		if(!(dwStyle & WS_CHILD))
		{
			if(hWndCenter != nullptr)
			{
				DWORD dwStyleCenter = ::GetWindowLong(hWndCenter, GWL_STYLE);
				if(!(dwStyleCenter & WS_VISIBLE) || (dwStyleCenter & WS_MINIMIZE))
					hWndCenter = nullptr;
			}
			
			HMONITOR hMonitor = nullptr;
			if(hWndCenter != nullptr)
			{
				hMonitor = ::MonitorFromWindow(hWndCenter, MONITOR_DEFAULTTONEAREST);
			}
			else
			{
				hMonitor = ::MonitorFromWindow(m_hwnd, MONITOR_DEFAULTTONEAREST);
			}
			
			if(!hMonitor) return FALSE;
			
			MONITORINFO minfo;
			minfo.cbSize = sizeof(MONITORINFO);
			BOOL bResult = ::GetMonitorInfo(hMonitor, &minfo);
			if(!bResult) return bResult;
			
			rcArea = minfo.rcWork;
			
			if(hWndCenter == NULL)
				rcCenter = rcArea;
			else
				::GetWindowRect(hWndCenter, &rcCenter);
		}
		else
		{
			hWndParent = ::GetParent(m_hwnd);
			if(hWndParent == nullptr) return FALSE;
			
			::GetClientRect(hWndParent, &rcArea);
			::GetClientRect(hWndCenter, &rcCenter);
			::MapWindowPoints(hWndCenter, hWndParent, (POINT*)&rcCenter, 2);
		}
		
		int DlgWidth = rcDlg.right - rcDlg.left;
		int DlgHeight = rcDlg.bottom - rcDlg.top;
		
		int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
		int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;
		
		if(xLeft + DlgWidth > rcArea.right)
			xLeft = rcArea.right - DlgWidth;
		if(xLeft < rcArea.left)
			xLeft = rcArea.left;
			
		if(yTop + DlgHeight > rcArea.bottom)
			yTop = rcArea.bottom - DlgHeight;
		if(yTop < rcArea.top)
			yTop = rcArea.top;
				
		return ::SetWindowPos(m_hwnd, NULL, xLeft, yTop, -1, -1,
			SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);		
		
	}
	
};

RECT Window::rcDefault = { CW_USEDEFAULT, CW_USEDEFAULT, 0, 0 };

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
typedef WinTraits<0, 0> NullTraits;

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
  void* GetCodeAddress()
  {
	return this;
  }
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
  void* GetCodeAddress()
  {
	return this;
  }
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
  
  WNDPROC GetWNDPROC(){
#ifdef __x86_64__
	return (WNDPROC)thunk->GetCodeAddress();
#else
	return (WNDPROC)thunk.GetCodeAddress();
#endif 
  }
  
  ~WndProcThunk() {
#ifdef __x86_64__
    if (thunk)
      ::VirtualFree(thunk, 0, MEM_RELEASE);
#endif
  }
};

class MessageMap
{
public:
  virtual BOOL HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,LRESULT& lResult, DWORD dwMsgMapId) = 0;
};

struct WNDMSG :
	 public MSG
{
public:
	int Size;
	BOOL bHandled;
	
	WNDMSG() : Size(sizeof(WNDMSG)), bHandled(true)
	{
		hwnd = nullptr;
		message = 0;
		wParam = 0;
		lParam = 0;
		time = 0;
		pt.x = pt.y = 0;
	}

	WNDMSG(HWND hWndIn, UINT uMsg,WPARAM wParamIn, LPARAM lParamIn,
		   DWORD dwTime, POINT ptIn, BOOL bHandledIn = TRUE ) : Size(sizeof(WNDMSG)),
		   bHandled(bHandledIn)
	{
		hwnd = hWndIn;
		message = uMsg;
		wParam = wParamIn;
		lParam = lParamIn;
		time = dwTime;
		pt = ptIn;
	}
	
	WNDMSG(MSG& msg, BOOL bHandledIn = TRUE) :
		Size(sizeof(WNDMSG)),bHandled(bHandledIn)
	{
		hwnd = msg.hwnd;
		message = msg.message;
		wParam = msg.wParam;
		lParam = msg.lParam;
		time = msg.time;
		pt = msg.pt;
	}
	WNDMSG(HWND hWndIn, UINT uMsg, WPARAM wParamIn, LPARAM lParamIn, BOOL bHandledIn = TRUE)
	{
		hwnd = hWndIn;
		message = uMsg;
		wParam = wParamIn;
		lParam = lParamIn;
		time = 0;
		pt.x = pt.y = 0;
	}
};

template <typename TBase = Window>
class WindowImplRoot:
	  public TBase,
	  public MessageMap
{
public:
	WndProcThunk m_thunk;
	const WNDMSG* m_currMsg;
	DWORD m_state;
	enum { WINSTATE_DESTROYED = 0x00000001 };
	
	WindowImplRoot() : m_currMsg(nullptr),m_state(0) {}
	
	virtual ~WindowImplRoot()
	{
	 
	}
	
	WNDMSG* GetCurrentMessage() const { return m_currMsg; }
	
	BOOL IsMsgHandled() const
	{
		const WNDMSG* pMsg = GetCurrentMessage();
		WINASSERT(pMsg != nullptr);
		WINASSERT(pMsg->Size >= sizeof(WNDMSG));
		return pMsg->bHandled;
	}
	
	void SetMsgHandled(BOOL bHandled)
	{
		WNDMSG* pMsg = GetCurrentMessage();
		WINASSERT(pMsg != nullptr);
		WINASSERT(pMsg->Size >= sizeof(WNDMSG));
		pMsg->bHandled = bHandled;
	}
	
LRESULT ForwardNotifications(HWND hWnd,UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
LRESULT ReflectNotifications(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
BOOL DefaultReflectNotificationHandler(HWND hWnd, UINT msg, WPARAM wParam,LPARAM lParam, BOOL& bHandled);

};

template<typename TBase>
LRESULT WindowImplRoot<TBase>::ReflectNotifications(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
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
        hchild = ((TBase*)this)->GetDlgItem( HIWORD(wParam));
        break;
      }
      break;
    case WM_DRAWITEM:
      if (wParam) // not from a menu
        hchild = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam)->hwndItem;
      break;
    case WM_MEASUREITEM:
      if (wParam) // not from a menu
        hchild = ((TBase*)this)->GetDlgItem(((LPMEASUREITEMSTRUCT)wParam)->CtlID);
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
LRESULT WindowImplRoot<TBase>::ForwardNotifications(
	 HWND hWnd,
	 UINT uMsg,
	 WPARAM wParam,
	 LPARAM lParam,
	 BOOL& bHandled)
{
	LRESULT lResult = 0;
	switch(uMsg)
	{
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
		lResult = ((TBase*)this)->GetParent().SendMessage(uMsg, wParam, lParam);
		break;
	default:
		bHandled = FALSE;
		break;
	}
	return lResult;
}


template<typename TBase>
BOOL WindowImplRoot<TBase>::DefaultReflectNotificationHandler(HWND hWnd, UINT msg, WPARAM wParam,
                                         LPARAM lParam, BOOL& bHandled) {
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
      lresult = ::DefWindowProc(((TBase*)this)->m_hwnd, msg - OCM__BASE, wParam, lParam);
      return TRUE;
    default:
      break;
    }
    return FALSE;
  }


template <typename TBase, class TWinTraits = ControlTraits>
class BaseWindowImplT : public WindowImplRoot<TBase>
{
public:
	WNDPROC m_pSuperWindowProc;
	WndProcThunk m_thunk;
	WNDPROC oldProc;
	
	BaseWindowImplT() : m_pSuperWindowProc(::DefWindowProc)
	{}
	
	static DWORD GetWndStyle(DWORD dwStyle)
	{
		return TWinTraits::GetWndStyle(dwStyle);
	}
	
	static DWORD GetWndExStyle(DWORD dwExStyle)
	{
		return TWinTraits::GetWndExStyle(dwExStyle);
	}
	
	virtual WNDPROC GetWindowProc()
	{
		return EndProc;
	}
	
static LRESULT CALLBACK
EndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	
	BaseWindowImplT<TBase, TWinTraits>* pThis = (BaseWindowImplT<TBase, TWinTraits>*)hwnd;

	LRESULT lRes = 0;
	BOOL bRet = pThis->HandleMessage(hwnd,uMsg,wParam,lParam,lRes,0);
	
	WINASSERT(pThis->m_currMsg == &msg);

	if(!bRet)
	{
	if (uMsg != WM_NCDESTROY) {
		lRes = pThis->DefWindowProc(uMsg,wParam,lParam);
	}
	else
	{
#ifdef __x86_64__
      // unsubclass if necessary
      if ((WNDPROC)pThis->oldProc !=
              (WNDPROC)pThis->m_thunk.thunk &&
          ((WNDPROC)::GetWindowLongPtr(pThis->m_hwnd,
                                       GWLP_WNDPROC) ==
              (WNDPROC)(pThis->m_thunk.thunk))) {
        ::SetWindowLongPtr(pThis->m_hwnd, GWLP_WNDPROC,
                           (LONG_PTR)pThis->oldProc);
        return lRes;
      }
#else
      if (pThis->oldProc != (WNDPROC) &
              pThis->m_thunk.thunk &&
          ((WNDPROC)::GetWindowLongPtr(pThis->m_hwnd, GWLP_WNDPROC) ==
               (WNDPROC) &
           pThis->m_thunk.thunk)) {
        ::SetWindowLongPtr(pThis->m_hwnd, GWLP_WNDPROC,
                           (LONG_PTR)pThis->oldProc);
        return lRes;
      }
#endif
		pThis->m_state |= WindowImplRoot<TBase>::WINSTATE_DESTROYED;
    }
	}
	if(pThis->m_state & WindowImplRoot<TBase>::WINSTATE_DESTROYED )
	{
		HWND hWndThis = pThis->m_hwnd;
		pThis->m_hwnd = nullptr;
		pThis->m_state &= ~WindowImplRoot<TBase>::WINSTATE_DESTROYED;
		pThis->OnFinalMessage(hWndThis);
	}
	
	return lRes;
  }

static LRESULT CALLBACK
WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    // This function is only called ONCE, on the FIRST message a window receives
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
    // architectures for MinGW/MinGW64 with this library.

    CriticalSectionLock lock(_wndCS);
    auto ret =
        find_if(_wndData.rbegin(), _wndData.rend(), [&](CreateWndData &dat) {
          return dat.dwThreadId == ::GetCurrentThreadId();
        });
    if (ret != _wndData.rend()) {
      BaseWindowImplT<TBase, TWinTraits> *pThis = (BaseWindowImplT<TBase, TWinTraits>  *)ret->pThis;
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
	  if(pThis->oldProc != WindowProc)
			WINTRACE(0,TEXT("Sub-classing through a discarded hook.\n"));
			
      return pProc(hwnd, uMsg, wParam, lParam);
    } else
      return 0;
  }
	

	HWND Create(
		HWND hWndParent,
		LPRECT rect,
		LPCTSTR szWindowName,
		DWORD dwStyle,
		DWORD dwExStyle,
		HMENU MenuOrID,
		ATOM atom,
		LPVOID lpCreateParam)
{
			if(atom == 0) return nullptr;
			
			_wndCS.Lock();
			_wndData.push_back({this, ::GetCurrentThreadId()});
			_wndCS.UnLock();
			
			if(MenuOrID == nullptr && (dwStyle & WS_CHILD))
				MenuOrID = (HMENU)(UINT_PTR)this;
			if(rect == nullptr)
				rect = &TBase::rcDefault;
			
			if(dwStyle == 0) dwStyle = TWinTraits::GetStyle();
			if(dwExStyle == 0) dwExStyle = TWinTraits::GetStyleEx();
				
			HWND hWnd = ::CreateWindowEx(dwExStyle,MAKEINTATOM(atom),szWindowName,
			dwStyle, rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top,
			hWndParent,MenuOrID, HINST_THISCOMPONENT, lpCreateParam);
			
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
	  
    ((BaseWindowImplT<TBase,TWinTraits>*)this)->m_hwnd = hWnd;
    return S_OK;
  }

  HWND UnSubClassWindow(BOOL force = FALSE) {
		HWND hWndo = ((BaseWindowImplT<TBase,TWinTraits>*)this)->m_hwnd;
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
  
  BOOL DestroyWindow()
  {
	HWND hWnd = ((BaseWindowImplT<TBase,TWinTraits>*)this)->m_hwnd;
	WINASSERT(::IsWindow(hWnd));
	return ::DestroyWindow(hWnd);
  }
  
  
  LRESULT DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
		return ::CallWindowProc(m_pSuperWindowProc,((BaseWindowImplT<TBase,TWinTraits>*)this)->m_hwnd,uMsg, wParam, lParam);
  }
  
  virtual ATOM RegisterClass() = 0;
  virtual void OnFinalMessage(HWND) {}
  
};

template <typename T, typename TBaseWindow, typename TWindowTraits> 
class BaseWindow : 
	public BaseWindowImplT<TBaseWindow,TWindowTraits>
{
public:
  	HWND Create(
		HWND hWndParent = nullptr,
		LPRECT rect = nullptr,
		LPCTSTR szWindowName = nullptr,
		DWORD dwStyle = 0,
		DWORD dwExStyle = 0,
		HMENU MenuOrID = 0U,
		LPVOID lpCreateParam = nullptr)
	{
		if(rect == nullptr) *rect = Window::rcDefault;
		
		ATOM atom = this->RegisterClass();
		LPTSTR st = MAKEINTATOM(atom);
		return BaseWindowImplT< TBaseWindow, TWindowTraits >::Create(hWndParent, rect, szWindowName,
			dwStyle, dwExStyle, MenuOrID, atom, lpCreateParam);
	}
	
	HWND Create(LPCTSTR lpWindowName, HWND hWndParent = nullptr, DWORD dwStyle = 0,
                 DWORD dwExStyle = 0, int x = CW_USEDEFAULT,
                 int y = CW_USEDEFAULT, int nWidth = CW_USEDEFAULT,
                 int nHeight = CW_USEDEFAULT, HMENU hMenu = 0,
                 LPVOID lpCreateParam = nullptr) {
     
	RECT rect = { x, y, nWidth, nHeight };
    HWND hwnd = this->Create((HWND)hWndParent,(LPRECT) &rect,(LPCTSTR) lpWindowName,
			(DWORD)dwStyle, (DWORD)dwExStyle,(HMENU)hMenu,(LPVOID)lpCreateParam);
	
    return hwnd;
  }
  
  HWND Create(HWND hWndParent = nullptr, LPCTSTR lpWindowName = nullptr, DWORD dwStyle = 0,
                 DWORD dwExStyle = 0, int x = CW_USEDEFAULT,
                 int y = CW_USEDEFAULT, int nWidth = CW_USEDEFAULT,
                 int nHeight = CW_USEDEFAULT, HMENU hMenu = 0,
                 LPVOID lpCreateParam = nullptr) {
     
	RECT rect = { x, y, nWidth, nHeight };
    HWND hwnd = this->Create((HWND)hWndParent,(LPRECT) &rect,(LPCTSTR) lpWindowName,
			(DWORD)dwStyle, (DWORD)dwExStyle,(HMENU)hMenu,(LPVOID)lpCreateParam);
	
    return hwnd;
  }
  virtual ATOM  RegisterClass() override
  {
	WNDCLASSEX wcx = T::GetWinClassInfo();
	return ::RegisterClassEx(&wcx);
  }
  
};


#define BEGIN_MSG_MAP()  																						 \
  BOOL bHandled = FALSE; 																						 \
  void SetHandled(BOOL Handled = TRUE) { bHandled = Handled; } 													 \
																												\
  BOOL HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgID = 0) override     \
  	{ 																											 \
																									 \
		(m_hwnd); 																								 \
		(uMsg); 																								 \
		(wParam); 																								 \
		(lParam); 																								\
		(lResult); 																									\
		(bHandled); 																								\
		switch(dwMsgID) 																							\
		{ 																											\
		case 0:

#define END_MSG_MAP() \
			break; \
		} \
		return FALSE; \
		} \

#define ALT_MSG_MAP(msgMapID) \
		break; \
		case msgMapId:
		

#define MESSAGE_HANDLER(msg, func) \
	if(uMsg == msg) \
	{ \
		bHandled = TRUE; \
		lResult = func(uMsg, wParam, lParam, bHandled); \
		return bHandled; \
	}

#ifndef COMMAND_HANDLERS
#define COMMAND_HANDLERS
// void OnCommandHandlerEX(UINT uNotifyCode, int nID, Window wndCtl)
#define COMMAND_HANDLER_EX(id, code, func) \
	if (uMsg == WM_COMMAND && code == HIWORD(wParam) && id == LOWORD(wParam)) \
	{ \
		SetHandled(); \
		func((UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnCommandIDHandlerEX(UINT uNotifyCode, int nID, Window wndCtl)
#define COMMAND_ID_HANDLER_EX(id, func) \
	if (uMsg == WM_COMMAND && id == LOWORD(wParam)) \
	{ \
		SetHandled(); \
		func((UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnCommandCodeHandlerEX(UINT uNotifyCode, int nID, Window wndCtl)
#define COMMAND_CODE_HANDLER_EX(code, func) \
	if (uMsg == WM_COMMAND && code == HIWORD(wParam)) \
	{ \
		SetHandled(); \
		func((UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam); \
		lResult = 0; \
			return bHandled; \
	}


// void OnCommandRangeHandlerEX(UINT uNotifyCode, int nID, Window wndCtl)
#define COMMAND_RANGE_HANDLER_EX(idFirst, idLast, func) \
	if(uMsg == WM_COMMAND && LOWORD(wParam) >= idFirst && LOWORD(wParam) <= idLast) \
	{ \
		SetHandled(); \
		func((UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnCommandRangeCodeHandlerEX(UINT uNotifyCode, int nID, Window wndCtl)
#define COMMAND_RANGE_CODE_HANDLER_EX(idFirst, idLast, code, func) \
	if(uMsg == WM_COMMAND && code == HIWORD(wParam) && LOWORD(wParam) >= idFirst && LOWORD(wParam) <= idLast) \
	{ \
		SetHandled(); \
		func((UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// LRESULT OnReflectedCommandHandlerEX(UINT uNotifyCode, int nID, Window wndCtl)
#define REFLECTED_COMMAND_HANDLER_EX(id, code, func) \
	if (uMsg == OCM_COMMAND && code == HIWORD(wParam) && id == LOWORD(wParam)) \
	{ \
		SetHandled(); \
		func((UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// LRESULT OnReflectedCommandIDHandlerEX(UINT uNotifyCode, int nID, Window wndCtl)
#define REFLECTED_COMMAND_ID_HANDLER_EX(id, func) \
	if (uMsg == OCM_COMMAND && id == LOWORD(wParam)) \
	{ \
		SetHandled(); \
		func((UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// LRESULT OnReflectedCommandCodeHandlerEX(UINT uNotifyCode, int nID, Window wndCtl)
#define REFLECTED_COMMAND_CODE_HANDLER_EX(code, func) \
	if (uMsg == OCM_COMMAND && code == HIWORD(wParam)) \
	{ \
		SetHandled(); \
		func((UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnReflectedCommandRangeHandlerEX(UINT uNotifyCode, int nID, Window wndCtl)
#define REFLECTED_COMMAND_RANGE_HANDLER_EX(idFirst, idLast, func) \
	if(uMsg == OCM_COMMAND && LOWORD(wParam) >= idFirst && LOWORD(wParam) <= idLast) \
	{ \
		SetHandled(); \
		func((UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnReflectedCommandRangeCodeHandlerEX(UINT uNotifyCode, int nID, Window wndCtl)
#define REFLECTED_COMMAND_RANGE_CODE_HANDLER_EX(idFirst, idLast, code, func) \
	if(uMsg == OCM_COMMAND && code == HIWORD(wParam) && LOWORD(wParam) >= idFirst && LOWORD(wParam) <= idLast) \
	{ \
		SetHandled(); \
		func((UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam); \
		lResult = 0; \
			return bHandled; \
	}
#endif

#ifndef NOTIFY_HANDLERS
#define NOTIFY_HANDLERS
	// LRESULT OnNotifyHandlerEX(LPNMHDR pnmh)
#define NOTIFY_HANDLER_EX(id, cd, func) \
	if (uMsg == WM_NOTIFY && cd == ((LPNMHDR)lParam)->code && id == ((LPNMHDR)lParam)->idFrom) \
	{ \
		SetHandled(); \
		lResult = func((LPNMHDR)lParam); \
			return bHandled; \
	}

// LRESULT OnNotifyIDHandlerEX(LPNMHDR pnmh)
#define NOTIFY_ID_HANDLER_EX(id, func) \
	if (uMsg == WM_NOTIFY && id == ((LPNMHDR)lParam)->idFrom) \
	{ \
		SetHandled(); \
		lResult = func((LPNMHDR)lParam); \
			return bHandled; \
	}

// LRESULT OnNotifyCodeHandlerEX(LPNMHDR pnmh)
#define NOTIFY_CODE_HANDLER_EX(cd, func) \
	if (uMsg == WM_NOTIFY && cd == ((LPNMHDR)lParam)->code) \
	{ \
		SetHandled(); \
		lResult = func((LPNMHDR)lParam); \
			return bHandled; \
	}

// LRESULT OnNotifyRangeHandlerEX(LPNMHDR pnmh)
#define NOTIFY_RANGE_HANDLER_EX(idFirst, idLast, func) \
	if(uMsg == WM_NOTIFY && ((LPNMHDR)lParam)->idFrom >= idFirst && ((LPNMHDR)lParam)->idFrom <= idLast) \
	{ \
		SetHandled(); \
		lResult = func((LPNMHDR)lParam); \
			return bHandled; \
	}

// LRESULT OnNotifyRangeCodeHandlerEX(LPNMHDR pnmh)
#define NOTIFY_RANGE_CODE_HANDLER_EX(idFirst, idLast, cd, func) \
	if(uMsg == WM_NOTIFY && cd == ((LPNMHDR)lParam)->code && ((LPNMHDR)lParam)->idFrom >= idFirst && ((LPNMHDR)lParam)->idFrom <= idLast) \
	{ \
		SetHandled(); \
		lResult = func((LPNMHDR)lParam); \
			return bHandled; \
	}

// LRESULT OnReflectedNotifyHandlerEX(LPNMHDR pnmh)
#define REFLECTED_NOTIFY_HANDLER_EX(id, cd, func) \
	if (uMsg == OCM_NOTIFY && cd == ((LPNMHDR)lParam)->code && id == ((LPNMHDR)lParam)->idFrom) \
	{ \
		SetHandled(); \
		lResult = func((LPNMHDR)lParam); \
			return bHandled; \
	}

// LRESULT OnReflectedNotifyIDHandlerEX(LPNMHDR pnmh)
#define REFLECTED_NOTIFY_ID_HANDLER_EX(id, func) \
	if (uMsg == OCM_NOTIFY && id == ((LPNMHDR)lParam)->idFrom) \
	{ \
		SetHandled(); \
		lResult = func((LPNMHDR)lParam); \
			return bHandled; \
	}

// LRESULT OnReflectedNotifyCodeHandlerEX(LPNMHDR pnmh)
#define REFLECTED_NOTIFY_CODE_HANDLER_EX(cd, func) \
	if (uMsg == OCM_NOTIFY && cd == ((LPNMHDR)lParam)->code) \
	{ \
		SetHandled(); \
		lResult = func((LPNMHDR)lParam); \
			return bHandled; \
	}

// LRESULT OnReflectedNotifyRangeHandlerEX(LPNMHDR pnmh)
#define REFLECTED_NOTIFY_RANGE_HANDLER_EX(idFirst, idLast, func) \
	if(uMsg == OCM_NOTIFY && ((LPNMHDR)lParam)->idFrom >= idFirst && ((LPNMHDR)lParam)->idFrom <= idLast) \
	{ \
		SetHandled(); \
		lResult = func((LPNMHDR)lParam); \
			return bHandled; \
	}

// LRESULT OnReflectedNotifyRangeCodeHandlerEX(LPNMHDR pnmh)
#define REFLECTED_NOTIFY_RANGE_CODE_HANDLER_EX(idFirst, idLast, cd, func) \
	if(uMsg == OCM_NOTIFY && cd == ((LPNMHDR)lParam)->code && ((LPNMHDR)lParam)->idFrom >= idFirst && ((LPNMHDR)lParam)->idFrom <= idLast) \
	{ \
		SetHandled(); \
		lResult = func((LPNMHDR)lParam); \
			return bHandled; \
	}

#define FORWARD_NOTIFICATIONS() \
	{ \
		bHandled = TRUE; \
		lResult = ForwardNotifications(uMsg, wParam, lParam, bHandled); \
		return bHandled;\
	}

#define REFLECT_NOTIFICATIONS() \
	{ \
		bHandled = TRUE; \
		lResult = ReflectNotifications(uMsg, wParam, lParam, bHandled); \
		return bHandled;\
	}

#endif

#ifndef CHAIN_HANDLERS
#define CHAIN_HANDLERS

#define CHAIN_MSG_MAP(theChainClass) \
	{ \
		if(theChainClass::HandleMessage(hWnd, uMsg, wParam, lParam, lResult)) \
			return TRUE; \
	}

#define CHAIN_MSG_MAP_MEMBER(theChainMember) \
	{ \
		if(theChainMember.HandleMessage(hWnd, uMsg, wParam, lParam, lResult)) \
			return TRUE; \
	}

#define CHAIN_MSG_MAP_ALT(theChainClass, msgMapID) \
	{ \
		if(theChainClass::HandleMessage(hWnd, uMsg, wParam, lParam, lResult, msgMapID)) \
			return TRUE; \
	}

#define CHAIN_MSG_MAP_ALT_MEMBER(theChainMember, msgMapID) \
	{ \
		if(theChainMember.HandleMessage(hWnd, uMsg, wParam, lParam, lResult, msgMapID)) \
			return TRUE; \
	}
#endif
	
#if (_WIN32_WINNT >= 0x0400)
	
	
// int OnCreate(LPCREATESTRUCT lpCreateStruct)
#define MSG_WM_CREATE(func) \
	if (uMsg == WM_CREATE) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((LPCREATESTRUCT)lParam); \
			return bHandled; \
	}

// BOOL OnInitDialog(Window wndFocus, LPARAM lInitParam)
#define MSG_WM_INITDIALOG(func) \
	if (uMsg == WM_INITDIALOG) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((HWND)wParam, lParam); \
			return bHandled; \
	}

// BOOL OnCopyData(Window wnd, PCOPYDATASTRUCT pCopyDataStruct)
#define MSG_WM_COPYDATA(func) \
	if (uMsg == WM_COPYDATA) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((HWND)wParam, (PCOPYDATASTRUCT)lParam); \
			return bHandled; \
	}

// void OnDestroy()
#define MSG_WM_DESTROY(func) \
	if (uMsg == WM_DESTROY) \
	{ \
		SetHandled(); \
		func(); \
		lResult = 0; \
			return bHandled; \
	}

// void OnMove(Point ptPos)
#define MSG_WM_MOVE(func) \
	if (uMsg == WM_MOVE) \
	{ \
		SetHandled(); \
		func(Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnSize(UINT nType, Size size)
#define MSG_WM_SIZE(func) \
	if (uMsg == WM_SIZE) \
	{ \
		SetHandled(); \
		func((UINT)wParam, Size(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnActivate(UINT nState, BOOL bMinimized, Window wndOther)
#define MSG_WM_ACTIVATE(func) \
	if (uMsg == WM_ACTIVATE) \
	{ \
		SetHandled(); \
		func((UINT)LOWORD(wParam), (BOOL)HIWORD(wParam), (HWND)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnSetFocus(Window wndOld)
#define MSG_WM_SETFOCUS(func) \
	if (uMsg == WM_SETFOCUS) \
	{ \
		SetHandled(); \
		func((HWND)wParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnKillFocus(Window wndFocus)
#define MSG_WM_KILLFOCUS(func) \
	if (uMsg == WM_KILLFOCUS) \
	{ \
		SetHandled(); \
		func((HWND)wParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnEnable(BOOL bEnable)
#define MSG_WM_ENABLE(func) \
	if (uMsg == WM_ENABLE) \
	{ \
		SetHandled(); \
		func((BOOL)wParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnPaint(CDCHandle dc)
#define MSG_WM_PAINT(func) \
	if (uMsg == WM_PAINT) \
	{ \
		SetHandled(); \
		func((HDC)wParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnClose()
#define MSG_WM_CLOSE(func) \
	if (uMsg == WM_CLOSE) \
	{ \
		SetHandled(); \
		func(); \
		lResult = 0; \
		return bHandled; \
	}

// BOOL OnQueryEndSession(UINT nSource, UINT uLogOff)
#define MSG_WM_QUERYENDSESSION(func) \
	if (uMsg == WM_QUERYENDSESSION) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((UINT)wParam, (UINT)lParam); \
			return bHandled; \
	}

// BOOL OnQueryOpen()
#define MSG_WM_QUERYOPEN(func) \
	if (uMsg == WM_QUERYOPEN) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func(); \
			return bHandled; \
	}

// BOOL OnEraseBkgnd(CDCHandle dc)
#define MSG_WM_ERASEBKGND(func) \
	if (uMsg == WM_ERASEBKGND) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((HDC)wParam); \
			return bHandled; \
	}

// void OnSysColorChange()
#define MSG_WM_SYSCOLORCHANGE(func) \
	if (uMsg == WM_SYSCOLORCHANGE) \
	{ \
		SetHandled(); \
		func(); \
		lResult = 0; \
			return bHandled; \
	}

// void OnEndSession(BOOL bEnding, UINT uLogOff)
#define MSG_WM_ENDSESSION(func) \
	if (uMsg == WM_ENDSESSION) \
	{ \
		SetHandled(); \
		func((BOOL)wParam, (UINT)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnShowWindow(BOOL bShow, UINT nStatus)
#define MSG_WM_SHOWWINDOW(func) \
	if (uMsg == WM_SHOWWINDOW) \
	{ \
		SetHandled(); \
		func((BOOL)wParam, (int)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// HBRUSH OnCtlColorEdit(CDCHandle dc, CEdit edit)
#define MSG_WM_CTLCOLOREDIT(func) \
	if (uMsg == WM_CTLCOLOREDIT) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((HDC)wParam, (HWND)lParam); \
			return bHandled; \
	}

// HBRUSH OnCtlColorListBox(CDCHandle dc, CListBox listBox)
#define MSG_WM_CTLCOLORLISTBOX(func) \
	if (uMsg == WM_CTLCOLORLISTBOX) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((HDC)wParam, (HWND)lParam); \
			return bHandled; \
	}

// HBRUSH OnCtlColorBtn(CDCHandle dc, CButton button)
#define MSG_WM_CTLCOLORBTN(func) \
	if (uMsg == WM_CTLCOLORBTN) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((HDC)wParam, (HWND)lParam); \
			return bHandled; \
	}

// HBRUSH OnCtlColorDlg(CDCHandle dc, Window wnd)
#define MSG_WM_CTLCOLORDLG(func) \
	if (uMsg == WM_CTLCOLORDLG) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((HDC)wParam, (HWND)lParam); \
			return bHandled; \
	}

// HBRUSH OnCtlColorScrollBar(CDCHandle dc, CScrollBar scrollBar)
#define MSG_WM_CTLCOLORSCROLLBAR(func) \
	if (uMsg == WM_CTLCOLORSCROLLBAR) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((HDC)wParam, (HWND)lParam); \
			return bHandled; \
	}

// HBRUSH OnCtlColorStatic(CDCHandle dc, CStatic wndStatic)
#define MSG_WM_CTLCOLORSTATIC(func) \
	if (uMsg == WM_CTLCOLORSTATIC) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((HDC)wParam, (HWND)lParam); \
			return bHandled; \
	}

// void OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
#define MSG_WM_SETTINGCHANGE(func) \
	if (uMsg == WM_SETTINGCHANGE) \
	{ \
		SetHandled(); \
		func((UINT)wParam, (LPCTSTR)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnDevModeChange(LPCTSTR lpDeviceName)
#define MSG_WM_DEVMODECHANGE(func) \
	if (uMsg == WM_DEVMODECHANGE) \
	{ \
		SetHandled(); \
		func((LPCTSTR)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnActivateApp(BOOL bActive, DWORD dwThreadID)
#define MSG_WM_ACTIVATEAPP(func) \
	if (uMsg == WM_ACTIVATEAPP) \
	{ \
		SetHandled(); \
		func((BOOL)wParam, (DWORD)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnFontChange()
#define MSG_WM_FONTCHANGE(func) \
	if (uMsg == WM_FONTCHANGE) \
	{ \
		SetHandled(); \
		func(); \
		lResult = 0; \
			return bHandled; \
	}

// void OnTimeChange()
#define MSG_WM_TIMECHANGE(func) \
	if (uMsg == WM_TIMECHANGE) \
	{ \
		SetHandled(); \
		func(); \
		lResult = 0; \
			return bHandled; \
	}

// void OnCancelMode()
#define MSG_WM_CANCELMODE(func) \
	if (uMsg == WM_CANCELMODE) \
	{ \
		SetHandled(); \
		func(); \
		lResult = 0; \
			return bHandled; \
	}

// BOOL OnSetCursor(Window wnd, UINT nHitTest, UINT message)
#define MSG_WM_SETCURSOR(func) \
	if (uMsg == WM_SETCURSOR) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((HWND)wParam, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam)); \
			return bHandled; \
	}

// int OnMouseActivate(Window wndTopLevel, UINT nHitTest, UINT message)
#define MSG_WM_MOUSEACTIVATE(func) \
	if (uMsg == WM_MOUSEACTIVATE) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((HWND)wParam, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam)); \
			return bHandled; \
	}

// void OnChildActivate()
#define MSG_WM_CHILDACTIVATE(func) \
	if (uMsg == WM_CHILDACTIVATE) \
	{ \
		SetHandled(); \
		func(); \
		lResult = 0; \
			return bHandled; \
	}

// void OnGetMinMaxInfo(LPMINMAXINFO lpMMI)
#define MSG_WM_GETMINMAXINFO(func) \
	if (uMsg == WM_GETMINMAXINFO) \
	{ \
		SetHandled(); \
		func((LPMINMAXINFO)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnIconEraseBkgnd(CDCHandle dc)
#define MSG_WM_ICONERASEBKGND(func) \
	if (uMsg == WM_ICONERASEBKGND) \
	{ \
		SetHandled(); \
		func((HDC)wParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnSpoolerStatus(UINT nStatus, UINT nJobs)
#define MSG_WM_SPOOLERSTATUS(func) \
	if (uMsg == WM_SPOOLERSTATUS) \
	{ \
		SetHandled(); \
		func((UINT)wParam, (UINT)LOWORD(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
#define MSG_WM_DRAWITEM(func) \
	if (uMsg == WM_DRAWITEM) \
	{ \
		SetHandled(); \
		func((UINT)wParam, (LPDRAWITEMSTRUCT)lParam); \
		lResult = TRUE; \
			return bHandled; \
	}

// void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
#define MSG_WM_MEASUREITEM(func) \
	if (uMsg == WM_MEASUREITEM) \
	{ \
		SetHandled(); \
		func((UINT)wParam, (LPMEASUREITEMSTRUCT)lParam); \
		lResult = TRUE; \
			return bHandled; \
	}

// void OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct)
#define MSG_WM_DELETEITEM(func) \
	if (uMsg == WM_DELETEITEM) \
	{ \
		SetHandled(); \
		func((UINT)wParam, (LPDELETEITEMSTRUCT)lParam); \
		lResult = TRUE; \
			return bHandled; \
	}

// int OnCharToItem(UINT nChar, UINT nIndex, CListBox listBox)
#define MSG_WM_CHARTOITEM(func) \
	if (uMsg == WM_CHARTOITEM) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((UINT)LOWORD(wParam), (UINT)HIWORD(wParam), (HWND)lParam); \
			return bHandled; \
	}

// int OnVKeyToItem(UINT nKey, UINT nIndex, CListBox listBox)
#define MSG_WM_VKEYTOITEM(func) \
	if (uMsg == WM_VKEYTOITEM) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((UINT)LOWORD(wParam), (UINT)HIWORD(wParam), (HWND)lParam); \
			return bHandled; \
	}

// HCURSOR OnQueryDragIcon()
#define MSG_WM_QUERYDRAGICON(func) \
	if (uMsg == WM_QUERYDRAGICON) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func(); \
			return bHandled; \
	}

// int OnCompareItem(int nIDCtl, LPCOMPAREITEMSTRUCT lpCompareItemStruct)
#define MSG_WM_COMPAREITEM(func) \
	if (uMsg == WM_COMPAREITEM) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((UINT)wParam, (LPCOMPAREITEMSTRUCT)lParam); \
			return bHandled; \
	}

// void OnCompacting(UINT nCpuTime)
#define MSG_WM_COMPACTING(func) \
	if (uMsg == WM_COMPACTING) \
	{ \
		SetHandled(); \
		func((UINT)wParam); \
		lResult = 0; \
			return bHandled; \
	}

// BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct)
#define MSG_WM_NCCREATE(func) \
	if (uMsg == WM_NCCREATE) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((LPCREATESTRUCT)lParam); \
			return bHandled; \
	}

// void OnNcDestroy()
#define MSG_WM_NCDESTROY(func) \
	if (uMsg == WM_NCDESTROY) \
	{ \
		SetHandled(); \
		func(); \
		lResult = 0; \
			return bHandled; \
	}

// LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
#define MSG_WM_NCCALCSIZE(func) \
	if (uMsg == WM_NCCALCSIZE) \
	{ \
		SetHandled(); \
		lResult = func((BOOL)wParam, lParam); \
			return bHandled; \
	}

// UINT OnNcHitTest(Point point)
#define MSG_WM_NCHITTEST(func) \
	if (uMsg == WM_NCHITTEST) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func(Point(lParam)); \
			return bHandled; \
	}

// void OnNcPaint(CRgnHandle rgn)
#define MSG_WM_NCPAINT(func) \
	if (uMsg == WM_NCPAINT) \
	{ \
		SetHandled(); \
		func((HRGN)wParam); \
		lResult = 0; \
			return bHandled; \
	}

// BOOL OnNcActivate(BOOL bActive)
#define MSG_WM_NCACTIVATE(func) \
	if (uMsg == WM_NCACTIVATE) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((BOOL)wParam); \
			return bHandled; \
	}

// UINT OnGetDlgCode(LPMSG lpMsg)
#define MSG_WM_GETDLGCODE(func) \
	if (uMsg == WM_GETDLGCODE) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((LPMSG)lParam); \
			return bHandled; \
	}

// void OnNcMouseMove(UINT nHitTest, Point point)
#define MSG_WM_NCMOUSEMOVE(func) \
	if (uMsg == WM_NCMOUSEMOVE) \
	{ \
		SetHandled(); \
		func((UINT)wParam, Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnNcLButtonDown(UINT nHitTest, Point point)
#define MSG_WM_NCLBUTTONDOWN(func) \
	if (uMsg == WM_NCLBUTTONDOWN) \
	{ \
		SetHandled(); \
		func((UINT)wParam, Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnNcLButtonUp(UINT nHitTest, Point point)
#define MSG_WM_NCLBUTTONUP(func) \
	if (uMsg == WM_NCLBUTTONUP) \
	{ \
		SetHandled(); \
		func((UINT)wParam, Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnNcLButtonDblClk(UINT nHitTest, Point point)
#define MSG_WM_NCLBUTTONDBLCLK(func) \
	if (uMsg == WM_NCLBUTTONDBLCLK) \
	{ \
		SetHandled(); \
		func((UINT)wParam, Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnNcRButtonDown(UINT nHitTest, Point point)
#define MSG_WM_NCRBUTTONDOWN(func) \
	if (uMsg == WM_NCRBUTTONDOWN) \
	{ \
		SetHandled(); \
		func((UINT)wParam, Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnNcRButtonUp(UINT nHitTest, Point point)
#define MSG_WM_NCRBUTTONUP(func) \
	if (uMsg == WM_NCRBUTTONUP) \
	{ \
		SetHandled(); \
		func((UINT)wParam, Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnNcRButtonDblClk(UINT nHitTest, Point point)
#define MSG_WM_NCRBUTTONDBLCLK(func) \
	if (uMsg == WM_NCRBUTTONDBLCLK) \
	{ \
		SetHandled(); \
		func((UINT)wParam, Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnNcMButtonDown(UINT nHitTest, Point point)
#define MSG_WM_NCMBUTTONDOWN(func) \
	if (uMsg == WM_NCMBUTTONDOWN) \
	{ \
		SetHandled(); \
		func((UINT)wParam, Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnNcMButtonUp(UINT nHitTest, Point point)
#define MSG_WM_NCMBUTTONUP(func) \
	if (uMsg == WM_NCMBUTTONUP) \
	{ \
		SetHandled(); \
		func((UINT)wParam, Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnNcMButtonDblClk(UINT nHitTest, Point point)
#define MSG_WM_NCMBUTTONDBLCLK(func) \
	if (uMsg == WM_NCMBUTTONDBLCLK) \
	{ \
		SetHandled(); \
		func((UINT)wParam, Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
#define MSG_WM_KEYDOWN(func) \
	if (uMsg == WM_KEYDOWN) \
	{ \
		SetHandled(); \
		func((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
#define MSG_WM_KEYUP(func) \
	if (uMsg == WM_KEYUP) \
	{ \
		SetHandled(); \
		func((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
#define MSG_WM_CHAR(func) \
	if (uMsg == WM_CHAR) \
	{ \
		SetHandled(); \
		func((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags)
#define MSG_WM_DEADCHAR(func) \
	if (uMsg == WM_DEADCHAR) \
	{ \
		SetHandled(); \
		func((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
#define MSG_WM_SYSKEYDOWN(func) \
	if (uMsg == WM_SYSKEYDOWN) \
	{ \
		SetHandled(); \
		func((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
#define MSG_WM_SYSKEYUP(func) \
	if (uMsg == WM_SYSKEYUP) \
	{ \
		SetHandled(); \
		func((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags)
#define MSG_WM_SYSCHAR(func) \
	if (uMsg == WM_SYSCHAR) \
	{ \
		SetHandled(); \
		func((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnSysDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags)
#define MSG_WM_SYSDEADCHAR(func) \
	if (uMsg == WM_SYSDEADCHAR) \
	{ \
		SetHandled(); \
		func((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnSysCommand(UINT nID, Point point)
#define MSG_WM_SYSCOMMAND(func) \
	if (uMsg == WM_SYSCOMMAND) \
	{ \
		SetHandled(); \
		func((UINT)wParam, Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnTCard(UINT idAction, DWORD dwActionData)
#define MSG_WM_TCARD(func) \
	if (uMsg == WM_TCARD) \
	{ \
		SetHandled(); \
		func((UINT)wParam, (DWORD)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnTimer(UINT_PTR nIDEvent)
#define MSG_WM_TIMER(func) \
	if (uMsg == WM_TIMER) \
	{ \
		SetHandled(); \
		func((UINT_PTR)wParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar)
#define MSG_WM_HSCROLL(func) \
	if (uMsg == WM_HSCROLL) \
	{ \
		SetHandled(); \
		func((int)LOWORD(wParam), (short)HIWORD(wParam), (HWND)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar)
#define MSG_WM_VSCROLL(func) \
	if (uMsg == WM_VSCROLL) \
	{ \
		SetHandled(); \
		func((int)LOWORD(wParam), (short)HIWORD(wParam), (HWND)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnInitMenu(CMenuHandle menu)
#define MSG_WM_INITMENU(func) \
	if (uMsg == WM_INITMENU) \
	{ \
		SetHandled(); \
		func((HMENU)wParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnInitMenuPopup(CMenuHandle menuPopup, UINT nIndex, BOOL bSysMenu)
#define MSG_WM_INITMENUPOPUP(func) \
	if (uMsg == WM_INITMENUPOPUP) \
	{ \
		SetHandled(); \
		func((HMENU)wParam, (UINT)LOWORD(lParam), (BOOL)HIWORD(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnMenuSelect(UINT nItemID, UINT nFlags, CMenuHandle menu)
#define MSG_WM_MENUSELECT(func) \
	if (uMsg == WM_MENUSELECT) \
	{ \
		SetHandled(); \
		func((UINT)LOWORD(wParam), (UINT)HIWORD(wParam), (HMENU)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenuHandle menu)
#define MSG_WM_MENUCHAR(func) \
	if (uMsg == WM_MENUCHAR) \
	{ \
		SetHandled(); \
		lResult = func((TCHAR)LOWORD(wParam), (UINT)HIWORD(wParam), (HMENU)lParam); \
			return bHandled; \
	}

// LRESULT OnNotify(int idCtrl, LPNMHDR pnmh)
#define MSG_WM_NOTIFY(func) \
	if (uMsg == WM_NOTIFY) \
	{ \
		SetHandled(); \
		lResult = func((int)wParam, (LPNMHDR)lParam); \
			return bHandled; \
	}

// void OnEnterIdle(UINT nWhy, Window wndWho)
#define MSG_WM_ENTERIDLE(func) \
	if (uMsg == WM_ENTERIDLE) \
	{ \
		SetHandled(); \
		func((UINT)wParam, (HWND)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnMouseMove(UINT nFlags, Point point)
#define MSG_WM_MOUSEMOVE(func) \
	if (uMsg == WM_MOUSEMOVE) \
	{ \
		SetHandled(); \
		func((UINT)wParam, Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// BOOL OnMouseWheel(UINT nFlags, short zDelta, Point pt)
#define MSG_WM_MOUSEWHEEL(func) \
	if (uMsg == WM_MOUSEWHEEL) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((UINT)LOWORD(wParam), (short)HIWORD(wParam), Point(lParam)); \
			return bHandled; \
	}

// void OnLButtonDown(UINT nFlags, Point point)
#define MSG_WM_LBUTTONDOWN(func) \
	if (uMsg == WM_LBUTTONDOWN) \
	{ \
		SetHandled(); \
		func((UINT)wParam, Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnLButtonUp(UINT nFlags, Point point)
#define MSG_WM_LBUTTONUP(func) \
	if (uMsg == WM_LBUTTONUP) \
	{ \
		SetHandled(); \
		func((UINT)wParam, Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnLButtonDblClk(UINT nFlags, Point point)
#define MSG_WM_LBUTTONDBLCLK(func) \
	if (uMsg == WM_LBUTTONDBLCLK) \
	{ \
		SetHandled(); \
		func((UINT)wParam, Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnRButtonDown(UINT nFlags, Point point)
#define MSG_WM_RBUTTONDOWN(func) \
	if (uMsg == WM_RBUTTONDOWN) \
	{ \
		SetHandled(); \
		func((UINT)wParam, Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnRButtonUp(UINT nFlags, Point point)
#define MSG_WM_RBUTTONUP(func) \
	if (uMsg == WM_RBUTTONUP) \
	{ \
		SetHandled(); \
		func((UINT)wParam, Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnRButtonDblClk(UINT nFlags, Point point)
#define MSG_WM_RBUTTONDBLCLK(func) \
	if (uMsg == WM_RBUTTONDBLCLK) \
	{ \
		SetHandled(); \
		func((UINT)wParam, Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnMButtonDown(UINT nFlags, Point point)
#define MSG_WM_MBUTTONDOWN(func) \
	if (uMsg == WM_MBUTTONDOWN) \
	{ \
		SetHandled(); \
		func((UINT)wParam, Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnMButtonUp(UINT nFlags, Point point)
#define MSG_WM_MBUTTONUP(func) \
	if (uMsg == WM_MBUTTONUP) \
	{ \
		SetHandled(); \
		func((UINT)wParam, Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnMButtonDblClk(UINT nFlags, Point point)
#define MSG_WM_MBUTTONDBLCLK(func) \
	if (uMsg == WM_MBUTTONDBLCLK) \
	{ \
		SetHandled(); \
		func((UINT)wParam, Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnParentNotify(UINT message, UINT nChildID, LPARAM lParam)
#define MSG_WM_PARENTNOTIFY(func) \
	if (uMsg == WM_PARENTNOTIFY) \
	{ \
		SetHandled(); \
		func((UINT)LOWORD(wParam), (UINT)HIWORD(wParam), lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnMDIActivate(Window wndActivate, Window wndDeactivate)
#define MSG_WM_MDIACTIVATE(func) \
	if (uMsg == WM_MDIACTIVATE) \
	{ \
		SetHandled(); \
		func((HWND)wParam, (HWND)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnRenderFormat(UINT nFormat)
#define MSG_WM_RENDERFORMAT(func) \
	if (uMsg == WM_RENDERFORMAT) \
	{ \
		SetHandled(); \
		func((UINT)wParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnRenderAllFormats()
#define MSG_WM_RENDERALLFORMATS(func) \
	if (uMsg == WM_RENDERALLFORMATS) \
	{ \
		SetHandled(); \
		func(); \
		lResult = 0; \
		return bHandled; \
	}

// void OnDestroyClipboard()
#define MSG_WM_DESTROYCLIPBOARD(func) \
	if (uMsg == WM_DESTROYCLIPBOARD) \
	{ \
		SetHandled(); \
		func(); \
		lResult = 0; \
			return bHandled; \
	}

// void OnDrawClipboard()
#define MSG_WM_DRAWCLIPBOARD(func) \
	if (uMsg == WM_DRAWCLIPBOARD) \
	{ \
		SetHandled(); \
		func(); \
		lResult = 0; \
			return bHandled; \
	}

// void OnPaintClipboard(Window wndViewer, const LPPAINTSTRUCT lpPaintStruct)
#define MSG_WM_PAINTCLIPBOARD(func) \
	if (uMsg == WM_PAINTCLIPBOARD) \
	{ \
		SetHandled(); \
		func((HWND)wParam, (const LPPAINTSTRUCT)::GlobalLock((HGLOBAL)lParam)); \
		::GlobalUnlock((HGLOBAL)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnVScrollClipboard(Window wndViewer, UINT nSBCode, UINT nPos)
#define MSG_WM_VSCROLLCLIPBOARD(func) \
	if (uMsg == WM_VSCROLLCLIPBOARD) \
	{ \
		SetHandled(); \
		func((HWND)wParam, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnContextMenu(Window wnd, Point point)
#define MSG_WM_CONTEXTMENU(func) \
	if (uMsg == WM_CONTEXTMENU) \
	{ \
		SetHandled(); \
		func((HWND)wParam, Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnSizeClipboard(Window wndViewer, const LPRECT lpRect)
#define MSG_WM_SIZECLIPBOARD(func) \
	if (uMsg == WM_SIZECLIPBOARD) \
	{ \
		SetHandled(); \
		func((HWND)wParam, (const LPRECT)::GlobalLock((HGLOBAL)lParam)); \
		::GlobalUnlock((HGLOBAL)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnAskCbFormatName(UINT nMaxCount, LPTSTR lpszString)
#define MSG_WM_ASKCBFORMATNAME(func) \
	if (uMsg == WM_ASKCBFORMATNAME) \
	{ \
		SetHandled(); \
		func((UINT)wParam, (LPTSTR)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnChangeCbChain(Window wndRemove, Window wndAfter)
#define MSG_WM_CHANGECBCHAIN(func) \
	if (uMsg == WM_CHANGECBCHAIN) \
	{ \
		SetHandled(); \
		func((HWND)wParam, (HWND)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnHScrollClipboard(Window wndViewer, UINT nSBCode, UINT nPos)
#define MSG_WM_HSCROLLCLIPBOARD(func) \
	if (uMsg == WM_HSCROLLCLIPBOARD) \
	{ \
		SetHandled(); \
		func((HWND)wParam, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// BOOL OnQueryNewPalette()
#define MSG_WM_QUERYNEWPALETTE(func) \
	if (uMsg == WM_QUERYNEWPALETTE) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func(); \
			return bHandled; \
	}

// void OnPaletteChanged(Window wndFocus)
#define MSG_WM_PALETTECHANGED(func) \
	if (uMsg == WM_PALETTECHANGED) \
	{ \
		SetHandled(); \
		func((HWND)wParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnPaletteIsChanging(Window wndPalChg)
#define MSG_WM_PALETTEISCHANGING(func) \
	if (uMsg == WM_PALETTEISCHANGING) \
	{ \
		SetHandled(); \
		func((HWND)wParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnDropFiles(HDROP hDropInfo)
#define MSG_WM_DROPFILES(func) \
	if (uMsg == WM_DROPFILES) \
	{ \
		SetHandled(); \
		func((HDROP)wParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnWindowPosChanging(LPWINDOWPOS lpWndPos)
#define MSG_WM_WINDOWPOSCHANGING(func) \
	if (uMsg == WM_WINDOWPOSCHANGING) \
	{ \
		SetHandled(); \
		func((LPWINDOWPOS)lParam); \
		lResult = 0; \
		return bHandled; \
	}

// void OnWindowPosChanged(LPWINDOWPOS lpWndPos)
#define MSG_WM_WINDOWPOSCHANGED(func) \
	if (uMsg == WM_WINDOWPOSCHANGED) \
	{ \
		SetHandled(); \
		func((LPWINDOWPOS)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnExitMenuLoop(BOOL fIsTrackPopupMenu)
#define MSG_WM_EXITMENULOOP(func) \
	if (uMsg == WM_EXITMENULOOP) \
	{ \
		SetHandled(); \
		func((BOOL)wParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnEnterMenuLoop(BOOL fIsTrackPopupMenu)
#define MSG_WM_ENTERMENULOOP(func) \
	if (uMsg == WM_ENTERMENULOOP) \
	{ \
		SetHandled(); \
		func((BOOL)wParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
#define MSG_WM_STYLECHANGED(func) \
	if (uMsg == WM_STYLECHANGED) \
	{ \
		SetHandled(); \
		func((UINT)wParam, (LPSTYLESTRUCT)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnStyleChanging(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
#define MSG_WM_STYLECHANGING(func) \
	if (uMsg == WM_STYLECHANGING) \
	{ \
		SetHandled(); \
		func((UINT)wParam, (LPSTYLESTRUCT)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnSizing(UINT fwSide, LPRECT pRect)
#define MSG_WM_SIZING(func) \
	if (uMsg == WM_SIZING) \
	{ \
		SetHandled(); \
		func((UINT)wParam, (LPRECT)lParam); \
		lResult = TRUE; \
			return bHandled; \
	}

// void OnMoving(UINT fwSide, LPRECT pRect)
#define MSG_WM_MOVING(func) \
	if (uMsg == WM_MOVING) \
	{ \
		SetHandled(); \
		func((UINT)wParam, (LPRECT)lParam); \
		lResult = TRUE; \
			return bHandled; \
	}

// void OnCaptureChanged(Window wnd)
#define MSG_WM_CAPTURECHANGED(func) \
	if (uMsg == WM_CAPTURECHANGED) \
	{ \
		SetHandled(); \
		func((HWND)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// BOOL OnDeviceChange(UINT nEventType, DWORD_PTR dwData)
#define MSG_WM_DEVICECHANGE(func) \
	if (uMsg == WM_DEVICECHANGE) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((UINT)wParam, (DWORD_PTR)lParam); \
			return bHandled; \
	}

// void OnCommand(UINT uNotifyCode, int nID, Window wndCtl)
#define MSG_WM_COMMAND(func) \
	if (uMsg == WM_COMMAND) \
	{ \
		SetHandled(); \
		func((UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnDisplayChange(UINT uBitsPerPixel, Size sizeScreen)
#define MSG_WM_DISPLAYCHANGE(func) \
	if (uMsg == WM_DISPLAYCHANGE) \
	{ \
		SetHandled(); \
		func((UINT)wParam, Size(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnEnterSizeMove()
#define MSG_WM_ENTERSIZEMOVE(func) \
	if (uMsg == WM_ENTERSIZEMOVE) \
	{ \
		SetHandled(); \
		func(); \
		lResult = 0; \
			return bHandled; \
	}

// void OnExitSizeMove()
#define MSG_WM_EXITSIZEMOVE(func) \
	if (uMsg == WM_EXITSIZEMOVE) \
	{ \
		SetHandled(); \
		func(); \
		lResult = 0; \
			return bHandled; \
	}

// HFONT OnGetFont()
#define MSG_WM_GETFONT(func) \
	if (uMsg == WM_GETFONT) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func(); \
		return bHandled; \
	}

// LRESULT OnGetHotKey()
#define MSG_WM_GETHOTKEY(func) \
	if (uMsg == WM_GETHOTKEY) \
	{ \
		SetHandled(); \
		lResult = func(); \
			return bHandled; \
	}

// HICON OnGetIcon()
#define MSG_WM_GETICON(func) \
	if (uMsg == WM_GETICON) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((UINT)wParam); \
			return bHandled; \
	}

// int OnGetText(int cchTextMax, LPTSTR lpszText)
#define MSG_WM_GETTEXT(func) \
	if (uMsg == WM_GETTEXT) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((int)wParam, (LPTSTR)lParam); \
			return bHandled; \
	}

// int OnGetTextLength()
#define MSG_WM_GETTEXTLENGTH(func) \
	if (uMsg == WM_GETTEXTLENGTH) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func(); \
			return bHandled; \
	}

// void OnHelp(LPHELPINFO lpHelpInfo)
#define MSG_WM_HELP(func) \
	if (uMsg == WM_HELP) \
	{ \
		SetHandled(); \
		func((LPHELPINFO)lParam); \
		lResult = TRUE; \
			return bHandled; \
	}

// void OnHotKey(int nHotKeyID, UINT uModifiers, UINT uVirtKey)
#define MSG_WM_HOTKEY(func) \
	if (uMsg == WM_HOTKEY) \
	{ \
		SetHandled(); \
		func((int)wParam, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnInputLangChange(DWORD dwCharSet, HKL hKbdLayout)
#define MSG_WM_INPUTLANGCHANGE(func) \
	if (uMsg == WM_INPUTLANGCHANGE) \
	{ \
		SetHandled(); \
		func((DWORD)wParam, (HKL)lParam); \
		lResult = TRUE; \
			return bHandled; \
	}

// void OnInputLangChangeRequest(BOOL bSysCharSet, HKL hKbdLayout)
#define MSG_WM_INPUTLANGCHANGEREQUEST(func) \
	if (uMsg == WM_INPUTLANGCHANGEREQUEST) \
	{ \
		SetHandled(); \
		func((BOOL)wParam, (HKL)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnNextDlgCtl(BOOL bHandle, WPARAM wCtlFocus)
#define MSG_WM_NEXTDLGCTL(func) \
	if (uMsg == WM_NEXTDLGCTL) \
	{ \
		SetHandled(); \
		func((BOOL)LOWORD(lParam), wParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnNextMenu(int nVirtKey, LPMDINEXTMENU lpMdiNextMenu)
#define MSG_WM_NEXTMENU(func) \
	if (uMsg == WM_NEXTMENU) \
	{ \
		SetHandled(); \
		func((int)wParam, (LPMDINEXTMENU)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// int OnNotifyFormat(Window wndFrom, int nCommand)
#define MSG_WM_NOTIFYFORMAT(func) \
	if (uMsg == WM_NOTIFYFORMAT) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((HWND)wParam, (int)lParam); \
			return bHandled; \
	}

// BOOL OnPowerBroadcast(DWORD dwPowerEvent, DWORD_PTR dwData)
#define MSG_WM_POWERBROADCAST(func) \
	if (uMsg == WM_POWERBROADCAST) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((DWORD)wParam, (DWORD_PTR)lParam); \
			return bHandled; \
	}

// void OnPrint(CDCHandle dc, UINT uFlags)
#define MSG_WM_PRINT(func) \
	if (uMsg == WM_PRINT) \
	{ \
		SetHandled(); \
		func((HDC)wParam, (UINT)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnPrintClient(CDCHandle dc, UINT uFlags)
#define MSG_WM_PRINTCLIENT(func) \
	if (uMsg == WM_PRINTCLIENT) \
	{ \
		SetHandled(); \
		func((HDC)wParam, (UINT)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnRasDialEvent(RASCONNSTATE rasconnstate, DWORD dwError)
#define MSG_WM_RASDIALEVENT(func) \
	if (uMsg == WM_RASDIALEVENT) \
	{ \
		SetHandled(); \
		func((RASCONNSTATE)wParam, (DWORD)lParam); \
		lResult = TRUE; \
			return bHandled; \
	}

// void OnSetFont(CFontHandle font, BOOL bRedraw)
#define MSG_WM_SETFONT(func) \
	if (uMsg == WM_SETFONT) \
	{ \
		SetHandled(); \
		func((HFONT)wParam, (BOOL)LOWORD(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// int OnSetHotKey(int nVirtKey, UINT uFlags)
#define MSG_WM_SETHOTKEY(func) \
	if (uMsg == WM_SETHOTKEY) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((int)LOBYTE(LOWORD(wParam)), (UINT)HIBYTE(LOWORD(wParam))); \
			return bHandled; \
	}

// HICON OnSetIcon(UINT uType, HICON hIcon)
#define MSG_WM_SETICON(func) \
	if (uMsg == WM_SETICON) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((UINT)wParam, (HICON)lParam); \
			return bHandled; \
	}

// void OnSetRedraw(BOOL bRedraw)
#define MSG_WM_SETREDRAW(func) \
	if (uMsg == WM_SETREDRAW) \
	{ \
		SetHandled(); \
		func((BOOL)wParam); \
		lResult = 0; \
			return bHandled; \
	}

// int OnSetText(LPCTSTR lpstrText)
#define MSG_WM_SETTEXT(func) \
	if (uMsg == WM_SETTEXT) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((LPCTSTR)lParam); \
			return bHandled; \
	}

// void OnUserChanged()
#define MSG_WM_USERCHANGED(func) \
	if (uMsg == WM_USERCHANGED) \
	{ \
		SetHandled(); \
		func(); \
		lResult = 0; \
			return bHandled; \
	}

// void OnMouseHover(WPARAM wParam, Point ptPos)
#define MSG_WM_MOUSEHOVER(func) \
	if (uMsg == WM_MOUSEHOVER) \
	{ \
		SetHandled(); \
		func(wParam, Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnMouseLeave()
#define MSG_WM_MOUSELEAVE(func) \
	if (uMsg == WM_MOUSELEAVE) \
	{ \
		SetHandled(); \
		func(); \
		lResult = 0; \
			return bHandled; \
	}

#endif 

#if (WINVER >= 0x0500)

// void OnMenuRButtonUp(WPARAM wParam, CMenuHandle menu)
#define MSG_WM_MENURBUTTONUP(func) \
	if (uMsg == WM_MENURBUTTONUP) \
	{ \
		SetHandled(); \
		func(wParam, (HMENU)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// LRESULT OnMenuDrag(WPARAM wParam, CMenuHandle menu)
#define MSG_WM_MENUDRAG(func) \
	if (uMsg == WM_MENUDRAG) \
	{ \
		SetHandled(); \
		lResult = func(wParam, (HMENU)lParam); \
			return bHandled; \
	}

// LRESULT OnMenuGetObject(PMENUGETOBJECTINFO info)
#define MSG_WM_MENUGETOBJECT(func) \
	if (uMsg == WM_MENUGETOBJECT) \
	{ \
		SetHandled(); \
		lResult = func((PMENUGETOBJECTINFO)lParam); \
			return bHandled; \
	}

// void OnUnInitMenuPopup(UINT nID, CMenuHandle menu)
#define MSG_WM_UNINITMENUPOPUP(func) \
	if (uMsg == WM_UNINITMENUPOPUP) \
	{ \
		SetHandled(); \
		func((UINT)HIWORD(lParam), (HMENU)wParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnMenuCommand(WPARAM nIndex, CMenuHandle menu)
#define MSG_WM_MENUCOMMAND(func) \
	if (uMsg == WM_MENUCOMMAND) \
	{ \
		SetHandled(); \
		func(wParam, (HMENU)lParam); \
		lResult = 0; \
			return bHandled; \
	}

#endif 

#if (_WIN32_WINNT >= 0x0500)

// BOOL OnAppCommand(Window wndFocus, short cmd, WORD uDevice, int dwKeys)
#define MSG_WM_APPCOMMAND(func) \
	if (uMsg == WM_APPCOMMAND) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((HWND)wParam, GET_APPCOMMAND_LPARAM(lParam), GET_DEVICE_LPARAM(lParam), GET_KEYSTATE_LPARAM(lParam)); \
			return bHandled; \
	}

// void OnNCXButtonDown(int fwButton, short nHittest, Point ptPos)
#define MSG_WM_NCXBUTTONDOWN(func) \
	if (uMsg == WM_NCXBUTTONDOWN) \
	{ \
		SetHandled(); \
		func(GET_XBUTTON_WPARAM(wParam), GET_NCHITTEST_WPARAM(wParam), Point(lParam)); \
		lResult = 0; \
			return TRUE; \
	}

// void OnNCXButtonUp(int fwButton, short nHittest, Point ptPos)
#define MSG_WM_NCXBUTTONUP(func) \
	if (uMsg == WM_NCXBUTTONUP) \
	{ \
		SetHandled(); \
		func(GET_XBUTTON_WPARAM(wParam), GET_NCHITTEST_WPARAM(wParam), Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnNCXButtonDblClk(int fwButton, short nHittest, Point ptPos)
#define MSG_WM_NCXBUTTONDBLCLK(func) \
	if (uMsg == WM_NCXBUTTONDBLCLK) \
	{ \
		SetHandled(); \
		func(GET_XBUTTON_WPARAM(wParam), GET_NCHITTEST_WPARAM(wParam), Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnXButtonDown(int fwButton, int dwKeys, Point ptPos)
#define MSG_WM_XBUTTONDOWN(func) \
	if (uMsg == WM_XBUTTONDOWN) \
	{ \
		SetHandled(); \
		func(GET_XBUTTON_WPARAM(wParam), GET_KEYSTATE_WPARAM(wParam), Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnXButtonUp(int fwButton, int dwKeys, Point ptPos)
#define MSG_WM_XBUTTONUP(func) \
	if (uMsg == WM_XBUTTONUP) \
	{ \
		SetHandled(); \
		func(GET_XBUTTON_WPARAM(wParam), GET_KEYSTATE_WPARAM(wParam), Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnXButtonDblClk(int fwButton, int dwKeys, Point ptPos)
#define MSG_WM_XBUTTONDBLCLK(func) \
	if (uMsg == WM_XBUTTONDBLCLK) \
	{ \
		SetHandled(); \
		func(GET_XBUTTON_WPARAM(wParam), GET_KEYSTATE_WPARAM(wParam), Point(lParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnChangeUIState(WORD nAction, WORD nState)
#define MSG_WM_CHANGEUISTATE(func) \
	if (uMsg == WM_CHANGEUISTATE) \
	{ \
		SetHandled(); \
		func(LOWORD(wParam), HIWORD(wParam)); \
		lResult = 0; \
			return bHandled; \
	}

// void OnUpdateUIState(WORD nAction, WORD nState)
#define MSG_WM_UPDATEUISTATE(func) \
	if (uMsg == WM_UPDATEUISTATE) \
	{ \
		SetHandled(); \
		func(LOWORD(wParam), HIWORD(wParam)); \
		lResult = 0; \
			return bHandled; \
	}

// LRESULT OnQueryUIState()
#define MSG_WM_QUERYUISTATE(func) \
	if (uMsg == WM_QUERYUISTATE) \
	{ \
		SetHandled(); \
		lResult = func(); \
			return bHandled; \
	}

#endif

#if(_WIN32_WINNT >= 0x0501)

// void OnInput(WPARAM RawInputCode, HRAWINPUT hRawInput)
#define MSG_WM_INPUT(func) \
	if (uMsg == WM_INPUT) \
	{ \
		SetHandled(); \
		func(GET_RAWINPUT_CODE_WPARAM(wParam), (HRAWINPUT)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnUniChar(TCHAR nChar, UINT nRepCnt, UINT nFlags)
#define MSG_WM_UNICHAR(func) \
	if (uMsg == WM_UNICHAR) \
	{ \
		SetHandled(); \
		func((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		if(bHandled)\
		{ \
			lResult = (wParam == UNICODE_NOCHAR) ? TRUE : FALSE; \
			return bHandled; \
		} \
	}

// void OnWTSSessionChange(WPARAM nStatusCode, PWTSSESSION_NOTIFICATION nSessionID)
#define MSG_WM_WTSSESSION_CHANGE(func) \
	if (uMsg == WM_WTSSESSION_CHANGE) \
	{ \
		SetHandled(); \
		func(wParam, (PWTSSESSION_NOTIFICATION)lParam); \
		lResult = 0; \
			return bHandled; \
	}

// void OnThemeChanged()
#define MSG_WM_THEMECHANGED(func) \
	if (uMsg == WM_THEMECHANGED) \
	{ \
		SetHandled(); \
		func(); \
		lResult = 0; \
			return bHandled; \
	}

#endif 

#if (_WIN32_WINNT >= 0x0600)

// BOOL OnMouseHWheel(UINT nFlags, short zDelta, Point pt)
#define MSG_WM_MOUSEHWHEEL(func) \
	if (uMsg == WM_MOUSEHWHEEL) \
	{ \
		SetHandled(); \
		lResult = (LRESULT)func((UINT)LOWORD(wParam), (short)HIWORD(wParam), Point(lParam)); \
			return bHandled; \
	}

#endif 

#endif