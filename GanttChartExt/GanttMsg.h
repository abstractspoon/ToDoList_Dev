// GanttTreeList.h: interface for the CGanttTreeList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GANTTMSG_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)
#define AFX_GANTTMSG_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////

// WPARAM = Hit test, LPARAM = Task ID
const UINT WM_GTLC_DATECHANGE				= ::RegisterWindowMessage(_T("WM_GTLC_DATECHANGE"));

// WPARAM = Drag Mode, LPARAM = Task ID
const UINT WM_GTLC_DRAGCHANGE				= ::RegisterWindowMessage(_T("WM_GTLC_DRAGCHANGE"));

// WPARAM = , LPARAM = 
const UINT WM_GTLC_NOTIFYZOOM				= ::RegisterWindowMessage(_T("WM_GTLC_NOTIFYZOOM"));

// WPARAM = , LPARAM = 
const UINT WM_GTLC_NOTIFYSORT				= ::RegisterWindowMessage(_T("WM_GTLC_NOTIFYSORT"));

// WPARAM = , LPARAM = 
const UINT WM_GTLC_PREFSHELP				= ::RegisterWindowMessage(_T("WM_GTLC_PREFSHELP"));

/////////////////////////////////////////////////////////////////////////////

// WPARAM = GCDD_MODE, LPARAM = HWND
const UINT WM_GANTTDEPENDDLG_CLOSE			= ::RegisterWindowMessage(_T("WM_GANTTDEPENDDLG_CLOSE"));

/////////////////////////////////////////////////////////////////////////////

// WPARAM = width, LPARAM = HWND
const UINT WM_GTCN_TITLECOLUMNWIDTHCHANGE	= ::RegisterWindowMessage(_T("WM_GTCN_TITLECOLUMNWIDTHCHANGE"));

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_GANTTMSG_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)
