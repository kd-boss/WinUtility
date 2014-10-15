#ifndef __BASEWINDOW_H__
#define __BASEWINDOW_H__
#ifndef STRICT
#define STRICT
#endif
#include <vector>
#include <utility>
#include <string>
#include <windows.h>
#include <windowsx.h>
#include <olectl.h>
#include <algorithm>
#include <memory>

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

#ifndef __COM_EXCEPTION__
#define __COM_EXCEPTION__
struct COM_EXCEPTION {
  const HRESULT res;
  COM_EXCEPTION(HRESULT hr) : res(hr) {}
  std::unique_ptr<PTSTR> what()
  {
      PTSTR errorText = NULL;

      FormatMessage(
         // use system message tables to retrieve error text
         FORMAT_MESSAGE_FROM_SYSTEM
         // allocate buffer on local heap for error text
         |FORMAT_MESSAGE_ALLOCATE_BUFFER
         // Important! will fail otherwise, since we're not
         // (and CANNOT) pass insertion parameters
         |FORMAT_MESSAGE_IGNORE_INSERTS,
         NULL,    // unused with FORMAT_MESSAGE_FROM_SYSTEM
         res,
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
         (LPTSTR)&errorText,  // output
         0, // minimum size for output buffer
         NULL);   // arguments - see note
      std::unique_ptr<PTSTR> buff;
      buff.reset(&errorText);
      return buff;
  }
};
#endif
#ifndef __HR__
#define __HR__
void HR(HRESULT hr) {
  if (hr != S_OK) throw COM_EXCEPTION(hr);
}
#endif


namespace std{
typedef std::basic_string<TCHAR> tstring;
#ifdef UNICODE
template<typename T>
tstring to_tstring(T t)
{
    return to_wstring(t);
}
#else
template<typename T>
tstring to_tstring(T t)
{
    return to_string(t);
}
#endif
}
enum ZOrderPlacement {
  Top = 0,          // HWND_TOP = ((HWND)0)
  Bottom = 1,       // HWND_BOTTOM = (HWND)1)
  TopMost = -1,     // HWND_TOPMOST = (HWND)-1)
  NonTopMost = -2,  // HWND_NOTOPMOST = ((HWND)-2)
};

struct Size {
  LONG cx;
  LONG cy;
  Size(LONG x, LONG y) : cx(x), cy(y) {}
  Size(LPARAM lParam) : cx(GET_X_LPARAM(lParam)), cy(GET_Y_LPARAM(lParam)) {}
};

struct Point {
  LONG x;
  LONG y;
  Point(LONG xx, LONG yy) : x(xx), y(yy) {}
  Point(LPARAM lParam) : x(GET_X_LPARAM(lParam)), y(GET_Y_LPARAM(lParam)) {}
};

struct Rect {
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
    Rect(RECT r) : left(r.left),top(r.top),right(r.right),bottom(r.bottom) {}
    Rect(RECT& r) : left(r.left),top(r.top),right(r.right),bottom(r.bottom) {}
    Rect() : left(0),top(0),right(0),bottom(0) {}
    LONG Width() { return right - left; }
    LONG Height() { return bottom - top; }
    Point Center() { return Point{left + (right - left) / 2,
                                    top + (bottom - top) /  2}; }
    operator RECT*() { return reinterpret_cast<RECT*>(this); } //for GetClientRect
};

Rect FixRect(const Rect rc){
    return RECT{std::min(rc.left,rc.right),
                std::min(rc.top,rc.bottom),
                std::max(rc.left,rc.right),
                std::max(rc.top,rc.bottom) };
}

bool HitTest(Rect r, Point pt)
{
    return (r.left <= pt.x) && (r.right >= pt.x) &&
           (r.top  <= pt.y) && (r.bottom >= pt.y);
}

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

template <DWORD m_dwStyle = 0, DWORD m_dwExStyle = 0>
class WinTraits {
 public:
  static DWORD GetStyle(DWORD dwStyle = 0) {
    return dwStyle ? dwStyle : m_dwStyle ;
  }
  static DWORD GetStyleEx(DWORD dwExStyle = 0) {
    return dwExStyle ?  dwExStyle :m_dwExStyle;
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
  DWORD m_mov;  // mov dword ptr [esp+0x4], pThis (esp+0x4 is hWnd)
  DWORD m_this; // replaces hWnd pointer with the 'this' pointer on the parameter stack
  BYTE m_jmp;       // jmp commmand
  DWORD m_relproc;  // relative jmp (address of EndProc)
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
};
#pragma pack(pop)
#endif

class WndProcThunk {
 public:
#ifdef __x86_64__
  _WndProcThunk *thunk;  // for x86_64 we need to VirtualAlloc the thunk code,
                         // and make the page executable
#else
  _WndProcThunk thunk;         // for x86 we need only to stack allocate it, which fails at runtime with msvc.
#endif
  void Init(WNDPROC proc, void *pThis) {
#ifdef __x86_64__
    thunk = (_WndProcThunk *)::VirtualAlloc(0, sizeof(_WndProcThunk),
                                            MEM_COMMIT | MEM_RESERVE,
                                            PAGE_EXECUTE_READWRITE);
    thunk->m_rcxMov = 0xb948;          // x86_64 op code : mov rcx,this
    thunk->m_rcxImm = (ULONG64)pThis;  // this pointer
    thunk->m_raxMov = 0xb848;          // x86_64 op code : mov rax,target
    thunk->m_raxImm = (ULONG64)proc;   // target pointer
    thunk->m_raxJmp = 0xe0ff;          // jmp rax
    ::FlushInstructionCache(::GetCurrentProcess(), thunk,
                            sizeof(_WndProcThunk));
#else
    thunk.m_mov = 0x042444C7;  // x86 op code : mov eax, DWORD_PTR this
    thunk.m_this = (ULONG_PTR)(ULONG) pThis;  // this
    thunk.m_jmp = 0xe9;  // x86 op code  : jmp relproc  (relproc is calulated to
                         // be the address at the end of this structure)
    thunk.m_relproc =
        DWORD((INT_PTR)proc -
              ((INT_PTR) this + sizeof(_WndProcThunk)));  // adjust address of
                                                          // Proc to compensate
                                                          // for size of the
                                                          // structure.
    ::FlushInstructionCache(::GetCurrentProcess(), &thunk,
                            sizeof(_WndProcThunk));
// actuall assembly generation is as follows due to __stdcall calling
// conventions on x86:
// mov eax,this
// call BaseWindow::WindowProc (who's address is replaced as the beginning of
// the jmp instruction in this struct)
// sub eax,al (from the arugment stack) (replacing HWND with the this ptr)
// jmp  BaseWindow::EndProc
#endif
  }
  ~WndProcThunk() {
#ifdef __x86_64__
    if (thunk) ::VirtualFree(thunk, 0, MEM_RELEASE);
#endif
  }
};

template <typename TBase, typename TWinTraits>
class BaseWindow {
 protected:
  WndProcThunk m_thunk;
  WNDPROC oldProc;
  bool bhandled;

  static LRESULT CALLBACK
      EndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_NCDESTROY) {
      LRESULT lres = ((TBase *)hwnd)->HandleMessage(msg, wParam, lParam);
#ifdef __x86_64__
      // unsubclass if necessairy
      if ( (WNDPROC)((TBase *)hwnd)->oldProc != (WNDPROC)((TBase *)hwnd)->m_thunk.thunk &&
          ((WNDPROC)::GetWindowLongPtr(((TBase *)hwnd)->m_hwnd, GWLP_WNDPROC)) ==
           ((WNDPROC)((TBase *)hwnd)->m_thunk.thunk) ){
        ::SetWindowLongPtr(((TBase *)hwnd)->m_hwnd, GWLP_WNDPROC,
                           (LONG_PTR)((TBase *)hwnd)->oldProc);
        return lres;
      }
#else
      if (((TBase *)hwnd)->oldProc != (WNDPROC) &
              ((TBase *)hwnd)->m_thunk.thunk &&
          ((WNDPROC)::GetWindowLongPtr(((TBase *)hwnd)->m_hwnd, GWLP_WNDPROC) ==
               (WNDPROC) &
           ((TBase *)hwnd)->m_thunk.thunk)) {
        ::SetWindowLongPtr(((TBase *)hwnd)->m_hwnd, GWLP_WNDPROC,
                           (LONG_PTR)((TBase *)hwnd)->oldProc);
        return lres;
      }
#endif
    }
    return ((TBase *)hwnd)->HandleMessage(
        msg, wParam, lParam);  // retrieve our class pointer and call the
                               // class's message handler, with compile time
                               // virtualization.
  }

  static LRESULT CALLBACK
      WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    // This function is only called ONCE, on the FIRST message a window recieves
    // during creation which is WM_GETMINMAXINFO. So the use of a the standard
    // library container to store the class pointer and then search for it is
    // extremely negligable in the performance of this library, as it only occurs
    // during window creation. This may slow down initalization of your application,
    // but if it does it will not be a measurable amount until you start adding more
    // than one thread in which tremendious amounts of windows are created. And in
    // that case performance degridation is to be expected because of the necessity
    // of allocating system resources. Most likely the critical section will be the
    // dominating factor if performance degridation is observed, as the search is
    // performed from the reverse end of the container (ie, last inserted).
    // The thunk applied to the window procedure completely replaces the window
    // procedure with EndProc. After the thunk is inserted into the windows memory,
    // subsequent calls to the windowprocedure by windows will invoke the EndProc
    // function with the HWND paramater replaced with the class pointer. This is
    // dynamic code execution, currently only supported on x86 and x86_64
    // archtecures for MinGW/MinGW64 with this library.

    CriticalSectionLock lock(_wndCS);
    auto ret =
        find_if(_wndData.rbegin(), _wndData.rend(), [&](CreateWndData &dat) {
          return dat.dwThreadId == ::GetCurrentThreadId();
        });
    if (ret != _wndData.rend()) {
      BaseWindow *pThis = (BaseWindow *)ret->pThis;
      pThis->m_hwnd = hwnd;
      pThis->m_thunk.Init(EndProc, pThis);
#ifdef __x86_64__
      WNDPROC pProc = (WNDPROC)(pThis->m_thunk.thunk);
      pThis->oldProc =
          (WNDPROC)::SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)pProc);
#else
      WNDPROC pProc = (WNDPROC) & (pThis->m_thunk.thunk);
      pThis->oldProc =
          (WNDPROC)::SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)pProc);
#endif

      return pProc(hwnd, uMsg, wParam, lParam);
    } else
      return 0;
  }

  LRESULT ForwardNotifications(UINT msg, WPARAM wParam, LPARAM lParam) {
    LRESULT res = 0;
    switch (msg) {
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
        res = ::SendMessage(::GetParent(m_hwnd), msg, wParam, lParam);
        break;
      default:
        bhandled = false;
        break;
    }
    return res;
  }

  LRESULT ReflectNotifications(UINT msg, WPARAM wParam, LPARAM lParam) {
    HWND hchild = nullptr;
    switch (msg) {
      case WM_COMMAND:
        if (lParam != 0)  // not from a menu
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
            hchild = GetDlgItem(m_hwnd, HIWORD(wParam));
            break;
        }
        break;
      case WM_DRAWITEM:
        if (wParam)  // not froma  menu
          hchild = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam)->hwndItem;
        break;
      case WM_MEASUREITEM:
        if (wParam)  // not froma  menu
          hchild = GetDlgItem(m_hwnd, ((LPMEASUREITEMSTRUCT)wParam)->CtlID);
        break;
      case WM_COMPAREITEM:
        if (wParam)  // not froma  menu
          hchild = reinterpret_cast<LPCOMPAREITEMSTRUCT>(wParam)->hwndItem;
        break;
      case WM_DELETEITEM:
        if (wParam)  // not froma  menu
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
      bhandled = false;
      return 1;
    }
    return ::SendMessage(hchild, OCM__BASE + msg, wParam, lParam);
  }

  BOOL DefaultReflectNotificationHandler(UINT msg, WPARAM wParam,
                                         LPARAM lParam) {
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
        lresult = ::DefWindowProc(m_hwnd, msg - OCM__BASE, wParam, lParam);
        return TRUE;
      default:
        break;
    }
    return FALSE;
  }

 public:
  BaseWindow() : m_hwnd(nullptr) {}

  HRESULT Create(PCTSTR lpWindowName, HWND hWndParent = 0, DWORD dwStyle = 0,
                 DWORD dwExStyle = 0, int x = CW_USEDEFAULT,
                 int y = CW_USEDEFAULT, int nWidth = CW_USEDEFAULT,
                 int nHeight = CW_USEDEFAULT, HMENU hMenu = 0,
                 void *lpCreateParams = nullptr) {
    bhandled = true;
    _wndCS.Lock();
    _wndData.push_back({this, ::GetCurrentThreadId()});
    _wndCS.UnLock();
    DWORD style = dwStyle;
    DWORD styleex = dwExStyle;
    WNDCLASS wc = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = ((TBase *)this)->ClassName();
    wc.hCursor = (HCURSOR)::LoadCursor(nullptr, IDC_ARROW);

    if (RegisterClass(&wc) == 0) {
      return E_FAIL;
    }
    if(dwStyle == 0)
        style = TWinTraits::GetStyle();
    if(dwExStyle == 0)
        styleex = TWinTraits::GetStyleEx();
    m_hwnd =
    CreateWindowEx(styleex, ((TBase *)this)->ClassName(), lpWindowName,
                       style, x, y, nWidth, nHeight, hWndParent, hMenu,
                       GetModuleHandle(nullptr), lpCreateParams);

    m_title = ((TBase *)this)->ClassName();
    if (m_hwnd == nullptr)
      return HRESULT_FROM_WIN32(GetLastError());
    else
      return S_OK;
  }

  HRESULT SubClassWindow(HWND hWnd) {
    if (!hWnd) return E_INVALIDARG;
    m_thunk.Init(EndProc, this);
#ifdef __x86_64__
    WNDPROC pProc = (WNDPROC)(m_thunk.thunk);
    oldProc = (WNDPROC)::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)pProc);
#else
    WNDPROC pProc = (WNDPROC) & (m_thunk.thunk);
    oldProc = (WNDPROC)::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)pProc);
#endif
    if (oldProc == nullptr) return E_FAIL;
    m_hwnd = hWnd;
    return S_OK;
  }

  HWND UnSubClassWindow(BOOL force = FALSE) {
    if (!IsWindow()) return nullptr;
#ifdef __x86_64__
    WNDPROC ourproc = (WNDPROC)(m_thunk.thunk);
#else
    WNDPROC ourproc = (WNDPROC) & (m_thunk.thunk);
#endif
    WNDPROC activeproc = (WNDPROC)::GetWindowLongPtr(m_hwnd, GWLP_WNDPROC);
    HWND hWnd = nullptr;
    if (force || ourproc == activeproc) {
      if (!::SetWindowLongPtr(m_hwnd, GWLP_WNDPROC, (LONG_PTR)oldProc))
        return nullptr;

      oldProc = ::DefWindowProc;
      hWnd = m_hwnd;
      m_hwnd = nullptr;
    }
    return hWnd;
  }

  HRESULT Invalidate() {
    if (!m_hwnd) return E_POINTER;
    if (InvalidateRect(m_hwnd, nullptr, FALSE) == 0)
      return HRESULT_FROM_WIN32(GetLastError());
    else
      return S_OK;
  }

  HRESULT Show(bool Visible) {
    if (!m_hwnd) return E_POINTER;
    ::ShowWindow(m_hwnd, Visible);
    return S_OK;
  }

  HRESULT ShowWindow(int nCmdShow) {
    if (!m_hwnd) return E_POINTER;
    ::ShowWindow(m_hwnd, nCmdShow);
    return S_OK;
  }

  HRESULT ShowWindowAsync(int nCmdShow) {
    if (!m_hwnd) return E_POINTER;
    if (!::ShowWindowAsync(m_hwnd, nCmdShow))
      return HRESULT_FROM_WIN32(::GetLastError());
    else
      return S_OK;
  }

  HRESULT GetSize(Size *sz) {
    if (!m_hwnd) return E_POINTER;
    RECT rect;
    if (::GetWindowRect(m_hwnd, &rect)) {
      if (nullptr != sz) {
        sz->cx = rect.right - rect.left;
        sz->cy = rect.bottom - rect.top;
      }
    } else {
      return HRESULT_FROM_WIN32(::GetLastError());
    }
    return S_OK;
  }

  HRESULT SetSize(const Size &sz) {
    if (!m_hwnd) return E_POINTER;
    if (!::SetWindowPos(m_hwnd, 0, 0, 0, sz.cx, sz.cy,
                        SWP_NOMOVE | SWP_NOREPOSITION | SWP_NOZORDER)) {
      return HRESULT_FROM_WIN32(::GetLastError());
    }
    return S_OK;
  }

  HRESULT SetPosition(const Point &pt) {
    if (!m_hwnd) return E_POINTER;
    if (!::SetWindowPos(m_hwnd, 0, pt.x, pt.y, 0, 0,
                        SWP_NOSIZE | SWP_NOZORDER | SWP_NOREPOSITION)) {
      return HRESULT_FROM_WIN32(::GetLastError());
    }
    return S_OK;
  }

  HRESULT GetRect(RECT *rc) {
    if (!m_hwnd) return E_POINTER;
    if (nullptr == rc) {
      return E_INVALIDARG;
    }
    if (!::GetWindowRect(m_hwnd, rc)) {
      return HRESULT_FROM_WIN32(::GetLastError());
    }
    return S_OK;
  }

  HRESULT GetParentWindowRect(RECT *rect) {
    if (!m_hwnd) return E_POINTER;
    if (nullptr == rect) {
      return E_INVALIDARG;
    }

    if (!::GetWindowRect(GetParent(m_hwnd), rect)) {
      return HRESULT_FROM_WIN32(::GetLastError());
    }

    return S_OK;
  }

  HRESULT SetRect(RECT rect) {
    if (!m_hwnd) return E_POINTER;
    if (!::SetWindowPos(m_hwnd, 0, rect.left, rect.top, rect.right - rect.left,
                        rect.bottom - rect.top, SWP_NOZORDER)) {
      return HRESULT_FROM_WIN32(::GetLastError());
    } else
      return S_OK;
  }

  HRESULT GetWindowHandle(HWND *hw) {
    if (m_hwnd) {
      *hw = m_hwnd;
      return S_OK;
    } else
      return E_POINTER;
  }

  HRESULT SetZOrder(BaseWindow *windowInsertAfter) {
    if (!m_hwnd) return E_POINTER;
    HWND hWndInsertAfter;

    HRESULT hr = windowInsertAfter->GetWindowHandle(&hWndInsertAfter);
    if (SUCCEEDED(hr)) {
      if (!::SetWindowPos(
              m_hwnd, hWndInsertAfter, 0, 0, 0, 0,
              SWP_NOSIZE | SWP_NOMOVE | SWP_NOSENDCHANGING | SWP_NOREDRAW)) {
        return HRESULT_FROM_WIN32(::GetLastError());
      } else
        hr = S_OK;
    }

    return hr;
  }

  HRESULT SetZOrder(ZOrderPlacement placement) {
    if (!m_hwnd) return E_POINTER;
    if (!::SetWindowPos(
            m_hwnd, reinterpret_cast<HWND>(placement), 0, 0, 0, 0,
            SWP_NOSIZE | SWP_NOMOVE | SWP_NOSENDCHANGING | SWP_NOREDRAW)) {
      return HRESULT_FROM_WIN32(::GetLastError());
    } else
      return S_OK;
  }

  HRESULT GetParentWindowHandle(HWND hWnd) {
    if (!m_hwnd) return E_POINTER;

    if ((hWnd = GetParent(m_hwnd)) == nullptr)
      return HRESULT_FROM_WIN32(::GetLastError());

    return S_OK;
  }

  HRESULT SetWindowHandle(HWND hWnd) {
    if (!::IsWindow(hWnd)) {
      return E_INVALIDARG;
    }
    if (m_hwnd) {
      DestroyWindow(m_hwnd);
      m_hwnd = nullptr;
    }
    m_hwnd = hWnd;
    return S_OK;
  }

  HRESULT GetClientRect(RECT *clientRect) {
    if (!::GetClientRect(m_hwnd, clientRect)) {
      return HRESULT_FROM_WIN32(::GetLastError());
    }

    return S_OK;
  }

  HRESULT SetTitle(const std::tstring &title) {
    if (!::SetWindowText(m_hwnd, title.c_str())) {
      return HRESULT_FROM_WIN32(::GetLastError());
    }

    m_title = title;
    return S_OK;
  }


  HRESULT GetTitle(std::tstring *title) {
    if (!title) return E_POINTER;

    *title = m_title;
    return S_OK;
  }


  HRESULT GetSmallIcon(HICON icon) {
    if (!icon) return E_INVALIDARG;

    icon = m_smallIcon;
    return S_OK;
  }

  HRESULT GetLargeIcon(HICON icon) {
    if (!icon) return E_INVALIDARG;

    icon = m_largeIcon;
    return S_OK;
  }

  HRESULT SetLargeIcon(HICON icon) {
    if (!icon) {
      return E_INVALIDARG;
    }
    if (!m_hwnd) {
      return E_POINTER;
    }
    m_largeIcon = icon;

    // Ignore the return value which is the handle to the prev. icon
    ::SendMessage(m_hwnd, WM_SETICON, ICON_BIG, MAKELPARAM(icon, 0));
    return S_OK;
  }

  HRESULT SetSmallIcon(HICON icon) {
    if (!icon) {
      return E_INVALIDARG;
    }
    if (!m_hwnd) {
      return E_POINTER;
    }
    m_smallIcon = icon;

    // Ignore the return value which is the handle to the prev. icon
    ::SendMessage(m_hwnd, WM_SETICON, ICON_SMALL, MAKELPARAM(icon, 0));
    return S_OK;
  }

  HRESULT RedrawWindow(bool eraseBackground) {
    if (!m_hwnd) {
      return E_POINTER;
    }
    ::InvalidateRect(m_hwnd, nullptr, eraseBackground ? TRUE : FALSE);
    return S_OK;
  }

  HRESULT RedrawWindow() {
    if (!m_hwnd) {
      return E_POINTER;
    }
    ::InvalidateRect(m_hwnd, nullptr, FALSE);
    return S_OK;
  }

  HRESULT UpdateWindow() {
    if (!m_hwnd) {
      return E_POINTER;
    }
    ::UpdateWindow(m_hwnd);
    return S_OK;
  }

  HRESULT SetFocus() {
    if (!m_hwnd) {
      return E_POINTER;
    }
    ::SetFocus(m_hwnd);
    return S_OK;
  }

  HRESULT SetCapture() {
    if (!m_hwnd) {
      return E_POINTER;
    }
    ::SetCapture(m_hwnd);
    return S_OK;
  }

  HRESULT IsMouseCaptured(bool *isMouseCaptured) {
    if (!m_hwnd) {
      return E_POINTER;
    }
    if (!isMouseCaptured) return E_INVALIDARG;

    *isMouseCaptured = ::GetCapture() == m_hwnd;
    return S_OK;
  }

  HRESULT IsChild(HWND hParent) {
    if (!m_hwnd) {
      return E_POINTER;
    }
    if (!::IsChild(hParent, m_hwnd))
      return HRESULT_FROM_WIN32(::GetLastError());
    else
      return S_OK;
  }

  HRESULT IsWindow() {
    if (::IsWindow(m_hwnd) == true)
      return S_OK;
    else
      return S_FALSE;
  }
  HRESULT AnimateWindow(DWORD dwTime, AnimateType type) {
    if (!m_hwnd) return E_POINTER;
    ::AnimateWindow(m_hwnd, dwTime, static_cast<DWORD>(type));
    return S_OK;
  }
  HRESULT DestroyWindow()
  {
      if(!m_hwnd) return E_POINTER;
      ::DestroyWindow(m_hwnd);
      return S_OK;
  }

 protected:
  virtual PCTSTR ClassName() const = 0;
  virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
  std::tstring m_title;
  HICON m_smallIcon;
  HICON m_largeIcon;
  HWND m_hwnd;
};

#define BEGIN_MSG_MAP()                                                     \
  inline void SetHandled(bool val = true) { bhandled = val; }               \
  LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override { \
    switch (uMsg) {
#define END_MSG_MAP()                                     \
  default:                                                \
    return ::DefWindowProc(m_hwnd, uMsg, wParam, lParam); \
    }                                                     \
    }

/// void OnActivateApp(bool activated,UINT ThreadID
#define MSG_WM_ACTIVATEAPP(thefunc) \
  case WM_ACTIVATEAPP:              \
    if (!bhandled) SetHandled();    \
    thefunc((bool)wParam, (UINT)lParam); return (bhandled == true) ? 0 : 1;

/// void OnCreate(LPCREATESTRUCT lpCreate)
#define MSG_WM_CREATE(thefunc)       \
  case WM_CREATE:                    \
    if (!bhandled) SetHandled();     \
    thefunc((LPCREATESTRUCT)lParam); \
    return (bhandled == true) ? 0 : 1;

/// void OnCancelMode()
#define MSG_WM_CANCELMODE(thefunc) \
  case WM_CANCELMODE:              \
    if (!bhandled) SetHandled();   \
    thefunc();                     \
    return (bhandled == true) ? 0 : 1;

/// void OnChildActivate()
#define MSG_WM_CHILDACTIVATE(thefunc) \
  case WM_CHILDACTIVATE:              \
    if (!bhandled) SetHandled();      \
    thefunc();                        \
    return (bhandled == true) ? 0 : 1;

/// void OnClose()
#define MSG_WM_CLOSE(thefunc)    \
  case WM_CLOSE:                 \
    if (!bhandled) SetHandled(); \
    thefunc();                   \
    return (bhandled == true) ? 0 : 1;

/// void OnCompacting(float CpuUsage)
/// Remarks: When an application receives this message, it should free as
/// much memory as possible, taking into account the current level of activity
/// of the application and the total number of applications running on the
/// system.
#define MSG_WM_COMPACTING(thefunc)            \
  case WM_COMPACTING:                         \
    if (!bhandled) SetHandled();              \
    thefunc((float)(0x10000 / (DWORD)wparm)); \
    return (bhandled == true) ? 0 : 1;

/// void OnDestroy()
#define MSG_WM_DESTROY(thefunc)  \
  case WM_DESTROY:               \
    if (!bhandled) SetHandled(); \
    thefunc();                   \
    return (bhandled == true) ? 0 : 1;

/// void OnEnable(bool EnabledState)
/// EnabledState determines if the window has been enabled or disabled
/// the parameter is TRUE if it's been enabled, FALSE otherwise
#define MSG_WM_ENABLED(thefunc)  \
  case WM_ENABLED:               \
    if (!bhandled) SetHandled(); \
    thefunc((bool)wParam);       \
    return (bhandled == true) ? 0 : 1;

/// void OnEnterSizeMove()
#define MSG_WM_ENTERSIZEMOVE(thefunc) \
  case WM_ENTERSIZEMOVE:              \
    if (!bhandled) SetHandled();      \
    thefunc();                        \
    return (bhandled == true) ? 0 : 1;

/// void OnExitSizeMove()
#define MSG_WM_EXITSIZEMOVE(thefunc) \
  case WM_EXITSIZEMOVE:              \
    if (!bhandled) SetHandled();     \
    thefunc();                       \
    return (bhandled == true) ? 0 : 1;

/// void OnGetMinMaxInfo(LPMINMAXINFO pMinMax)
#define MSG_WM_GETMINMAXINFO(thefunc) \
  case WM_GETMINMAXINFO:              \
    if (!bhandled) SetHandled();      \
    thefunc((LPMINMAXINFO)lParam);    \
    return (bhandled == true) ? 0 : 1;

// void OnInputLanguageChange(    
/// void OnMove(const Point& pt)
#define MSG_WM_MOVE(thefunc)     \
  case WM_MOVE:                  \
    if (!bhandled) SetHandled(); \
    thefunc(Point(lParam));      \
    return (bhandled == true) ? 0 : 1;

/// void OnMoving(LPRECT pRc)
#define MSG_WM_MOVING(thefunc) \
  case WM_MOVING:              \
    thefunc((LPRECT)lParam);   \
    return (bhandled == true) ? 0 : 1;

/// void OnSize(UINT nFlag,const Size& sz)
#define MSG_WM_SIZE(thefunc)             \
  case WM_SIZE:                          \
    if (!bhandled) SetHandled();         \
    thefunc((UINT)wParam, Size(lParam)); \
    return (bhandled == true) ? 0 : 1;

/// void OnUserChanged()
#define MSG_WM_USERCHANGED(thefunc) \
  case WM_USERCHANGED:              \
    if (!bhandled) SetHandled();    \
    thefunc();                      \
    return (bhandled == true) ? 0 : 1;

/// void OnPaint(LPPAINTSTRUCT lpPaint)
#define MSG_WM_PAINT(thefunc)    \
  case WM_PAINT:                 \
    if (!bhandled) SetHandled(); \
    PAINTSTRUCT ps;              \
    ::BeginPaint(m_hwnd, &ps);   \
    thefunc(&ps);                \
    ::EndPaint(m_hwnd, &ps);     \
    return (bhandled == true) ? 0 : 1;

/// void OnMouseMove(UINT nFlags,const Point& pt)
#define MSG_WM_MOUSEMOVE(thefunc)         \
  case WM_MOUSEMOVE:                      \
    if (!bhandled) SetHandled();          \
    thefunc((UINT)wParam, Point(lParam)); \
    return (bhandled == true) ? 0 : 1;

/// void OnLButtonDown(UINT nFlags,const Point& pt)
#define MSG_WM_LBUTTONDOWN(thefunc)       \
  case WM_LBUTTONDOWN:                    \
    if (!bhandled) SetHandled();          \
    thefunc((UINT)wParam, Point(lParam)); \
    return (bhandled == true) ? 0 : 1;

/// void OnLButtonUp(UINT nFlags,const Point& pt)
#define MSG_WM_LButtonUp(thefunc)         \
  case WM_LBUTTONUP:                      \
    if (!bhandled) SetHandled();          \
    thefunc((UINT)wParam, Point(lParam)); \
    return (bhandled == true) ? 0 : 1;

/// void OnLButtonDoubleClick(UINT nFlags,const Point& pt)
#define MSG_WM_LBUTTONCLK(thefunc)        \
  case WM_LBUTTONDBLCLK:                  \
    if (!bhandled) SetHandled();          \
    thefunc((UINT)wParam, Point(lParam)); \
    return (bhandled == true) ? 0 : 1;

/// void OnRButtonDown(UINT nFlags,const Point& pt)
#define MSG_WM_RBUTTONDOWN(thefunc)       \
  case WM_RBUTTONDOWN:                    \
    if (!bhandled) SetHandled();          \
    thefunc((UINT)wParam, Point(lParam)); \
    return (bhandled == true) ? 0 : 1;

/// void OnRButtonUp(UINT nFlags,const Point& pt)
#define MSG_WM_RBUTTONUP(thefunc)         \
  case WM_RBUTTONUP:                      \
    if (!bhandled) SetHandled();          \
    thefunc((UINT)wParam, Point(lParam)); \
    return (bhandled == true) ? 0 : 1;

/// void OnRButtonDoubleClick(UINT nFlags,const Point& pt)
#define MSG_WM_RBUTTONDBLCLK(thefunc)     \
  case WM_RBUTTONDBLCLK:                  \
    if (!bhandled) SetHandled();          \
    thefunc((UINT)wParam, Point(lParam)); \
    return (bhandled == true) ? 0 : 1;

/// void OnXButtonUp(UINT nFlags,const Point& pt)
#define MSG_WM_XBUTTONUP(thefunc)         \
  case WM_XBUTTONUP:                      \
    if (!bhandled) SetHandled();          \
    thefunc((UINT)wParam, Point(lParam)); \
    return (bhandled == true) ? 0 : 1;

/// void OnXButtonDown(UINT nFlags,const Point& pt)
#define MSG_WM_XBUTTONDOWN(thefunc)       \
  case WM_XBUTTONDOWN:                    \
    if (!bhandled) SetHandled();          \
    thefunc((UINT)wParam, Point(lParam)); \
    return (bhandled == true) ? 0 : 1;

/// void OnXButtonDoubleClick(UINT nFlags,const Point& pt)
#define MSG_WM_XBUTTONDBLCLK(thefunc)     \
  case WM_XBUTTONDBLCLK:                  \
    if (!bhandled) SetHandled();          \
    thefunc((UINT)wParam, Point(lParam)); \
    return (bhandled == true) ? 0 : 1;

/// void OnMButtonUp(UINT nFlags,const Point& pt)
#define MSG_WM_MBUTTONUP(thefunc)         \
  case WM_MBUTTONUP:                      \
    if (!bhandled) SetHandled();          \
    thefunc((UINT)wParam, Point(lParam)); \
    return (bhandled == true) ? 0 : 1;

/// void OnMButtonDown(UINT nFlags,const Point& pt)
#define MSG_WM_MBUTTONDOWN(thefunc)       \
  case WM_MBUTTONDOWN:                    \
    if (!bhandled) SetHandled();          \
    thefunc((UINT)wParam, Point(lParam)); \
    return (bhandled == true) ? 0 : 1;

/// void OnMButtonDoubleClick(UINT nFlags,const Point& pt)
#define MSG_WM_MBUTTONDBLCLK(thefunc)     \
  case WM_MBUTTONDBLCLK:                  \
    if (!bhandled) SetHandled();          \
    thefunc((UINT)wParam, Point(lParam)); \
    return (bhandled == true) ? 0 : 1;

#endif
