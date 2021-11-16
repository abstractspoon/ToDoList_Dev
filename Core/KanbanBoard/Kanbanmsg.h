// KanbanTreeList.h: interface for the CKanbanTreeList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANBANMSG_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)
#define AFX_KANBANMSG_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////

// WPARAM = int, LPARAM = LPDWORD
const UINT WM_KBC_VALUECHANGE		= ::RegisterWindowMessage(_T("WM_KBC_VALUECHANGE"));

// WPARAM = , LPARAM = 
const UINT WM_KBC_SELECTIONCHANGE	= ::RegisterWindowMessage(_T("WM_KBC_SELECTIONCHANGE"));

// WPARAM = , LPARAM = 
const UINT WM_KBC_PREFSHELP			= ::RegisterWindowMessage(_T("WM_KBC_PREFSHELP"));

// WPARAM = Task ID, LPARAM = int* (imageIndex), return HIMAGELIST
const UINT WM_KBC_GETTASKICON 		= ::RegisterWindowMessage(_T("WM_KBC_GETTASKICON"));

// WPARAM = , LPARAM = TRUE/FALSE
const UINT WM_KBC_EDITTASKDONE		= ::RegisterWindowMessage(_T("WM_KBC_EDITTASKDONE"));

// WPARAM = Task ID, LPARAM = 
const UINT WM_KBC_EDITTASKTITLE		= ::RegisterWindowMessage(_T("WM_KBC_EDITTASKTITLE"));

// WPARAM = , LPARAM = 
const UINT WM_KBC_EDITTASKICON		= ::RegisterWindowMessage(_T("WM_KBC_EDITTASKICON"));

// WPARAM = , LPARAM = TRUE/FALSE
const UINT WM_KBC_EDITTASKFLAG		= ::RegisterWindowMessage(_T("WM_KBC_EDITTASKFLAG"));

// WPARAM = Sort ascending, LPARAM = TDC_ATTRIBUTE
const UINT WM_KBC_SORTCHANGE		= ::RegisterWindowMessage(_T("WM_KBC_SORTCHANGE"));

// WPARAM = , LPARAM = LPCTSTR
const UINT WM_KBC_SHOWFILELINK		= ::RegisterWindowMessage(_T("WM_KBC_SHOWFILELINK"));

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_KANBANMSG_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)
