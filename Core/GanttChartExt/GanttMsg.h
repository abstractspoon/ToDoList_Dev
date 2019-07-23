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

// WPARAM = HWND, LPARAM = TRUE/FALSE
const UINT WM_GTLC_COMPLETIONCHANGE			= ::RegisterWindowMessage(_T("WM_GTLC_COMPLETIONCHANGE"));

// WPARAM = , LPARAM = 
const UINT WM_GTLC_NOTIFYZOOM				= ::RegisterWindowMessage(_T("WM_GTLC_NOTIFYZOOM"));

// WPARAM = , LPARAM = 
const UINT WM_GTLC_NOTIFYSORT				= ::RegisterWindowMessage(_T("WM_GTLC_NOTIFYSORT"));

// WPARAM = , LPARAM = 
const UINT WM_GTLC_PREFSHELP				= ::RegisterWindowMessage(_T("WM_GTLC_PREFSHELP"));

// WPARAM = LPCTSTR (Task Icon), LPARAM = int* (imageIndex), return HIMAGELIST
const UINT WM_GTLC_GETTASKICON				= ::RegisterWindowMessage(_T("WM_GTLC_GETTASKICON"));

// WPARAM = HWND, LPARAM = 
const UINT WM_GTLC_EDITTASKICON				= ::RegisterWindowMessage(_T("WM_GTLC_EDITTASKICON"));

// WPARAM = 0, LPARAM = IUITASKMOVE*
const UINT WM_GTLC_MOVETASK					= ::RegisterWindowMessage(_T("WM_GTLC_MOVETASK"));

// WPARAM = 0, LPARAM = Task ID
const UINT WM_GTLC_EDITTASKTITLE			= ::RegisterWindowMessage(_T("WM_GTLC_EDITTASKTITLE"));

/////////////////////////////////////////////////////////////////////////////

// WPARAM = GCDD_MODE, LPARAM = HWND
const UINT WM_GANTTDEPENDDLG_CLOSE			= ::RegisterWindowMessage(_T("WM_GANTTDEPENDDLG_CLOSE"));

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_GANTTMSG_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)
