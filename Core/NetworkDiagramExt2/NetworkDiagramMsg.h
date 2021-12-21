// NetworkDiagramMsg.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETWORKDIAGRAMMSG_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)
#define AFX_NETWORKDIAGRAMMSG_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////

// WPARAM = Hit test, LPARAM = Task ID
const UINT WM_NDC_DATECHANGE			= ::RegisterWindowMessage(_T("WM_NDC_DATECHANGE"));

// WPARAM = Drag Mode, LPARAM = Task ID
const UINT WM_NDC_DRAGCHANGE			= ::RegisterWindowMessage(_T("WM_NDC_DRAGCHANGE"));

// WPARAM = HWND, LPARAM = TRUE/FALSE
const UINT WM_NDC_COMPLETIONCHANGE		= ::RegisterWindowMessage(_T("WM_NDC_COMPLETIONCHANGE"));

// WPARAM = , LPARAM = 
const UINT WM_NDC_NOTIFYZOOM			= ::RegisterWindowMessage(_T("WM_NDC_NOTIFYZOOM"));

// WPARAM = , LPARAM = 
const UINT WM_NDC_NOTIFYSORT			= ::RegisterWindowMessage(_T("WM_NDC_NOTIFYSORT"));

// WPARAM = , LPARAM = 
const UINT WM_NDC_PREFSHELP				= ::RegisterWindowMessage(_T("WM_NDC_PREFSHELP"));

// WPARAM = LPCTSTR (Task Icon), LPARAM = int* (imageIndex), return HIMAGELIST
const UINT WM_NDC_GETTASKICON			= ::RegisterWindowMessage(_T("WM_NDC_GETTASKICON"));

// WPARAM = HWND, LPARAM = 
const UINT WM_NDC_EDITTASKICON			= ::RegisterWindowMessage(_T("WM_NDC_EDITTASKICON"));

// WPARAM = 0, LPARAM = IUITASKMOVE*
const UINT WM_NDC_MOVETASK				= ::RegisterWindowMessage(_T("WM_NDC_MOVETASK"));

// WPARAM = 0, LPARAM = Task ID
const UINT WM_NDC_EDITTASKTITLE			= ::RegisterWindowMessage(_T("WM_NDC_EDITTASKTITLE"));

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_NETWORKDIAGRAMMSG_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)
