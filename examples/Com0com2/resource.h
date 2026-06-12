#pragma once
// resource.h  —  Control and menu IDs for com0com Manager

// ── Menu IDs ────────────────────────────────────────────────────
#define IDM_FILE_EXIT           40001
#define IDM_PAIRS_ADD           40010
#define IDM_PAIRS_REMOVE        40011
#define IDM_PAIRS_REFRESH       40012
#define IDM_HELP_ABOUT          40020

// ── Main window control IDs ─────────────────────────────────────
#define IDC_LISTVIEW            1001
#define IDC_BTN_ADD             1002
#define IDC_BTN_REMOVE          1003
#define IDC_BTN_REFRESH         1004
#define IDC_BTN_BIND            1006
#define IDC_STATUSBAR           1005

// ── Add/Edit pair window control IDs ───────────────────────────
#define IDC_LBL_PORT_A          2001
#define IDC_EDIT_PORT_A         2002
#define IDC_LBL_PORT_B          2003
#define IDC_EDIT_PORT_B         2004
#define IDC_CHK_EMUBR_A         2005
#define IDC_CHK_EMUBR_B         2006
#define IDC_BTN_OK              2007
#define IDC_BTN_CANCEL          2008
#define IDC_LBL_HINT            2009
#define IDC_BTN_CHANGE_A        2010
#define IDC_BTN_CHANGE_B        2011
#define IDC_COM_COM             2012

// ── About dialog ────────────────────────────────────────────────
#define IDD_ABOUT               3001
#define IDC_ABOUT_ICON          3002
#define IDC_ABOUT_TITLE         3003
#define IDC_ABOUT_VERSION       3004
#define IDC_ABOUT_DESC          3005
#define IDC_ABOUT_LINK          3006   // SysLink control — clickable URL
#define IDC_ABOUT_DRIVER_LABEL  3007
#define IDC_ABOUT_CLOSE         3008
#define IDC_ABOUT_NOTE          3009

// ── Application icon ────────────────────────────────────────────
#define IDI_APP                 101
// ── Custom messages ─────────────────────────────────────────────
// Posted by AddPairWindow to its owner (MainWindow) when done.
// wParam = 1 (OK) or 0 (Cancel)
// lParam = pointer to heap-allocated PairResult (caller must delete)
#define WM_PAIRWINDOW_DONE      (WM_APP + 1)

// Posted by EditPortWindow when done.
#define WM_EDITWINDOW_DONE      (WM_APP + 2)
