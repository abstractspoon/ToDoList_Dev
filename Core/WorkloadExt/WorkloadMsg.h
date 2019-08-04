// WorkloadTreeList.h: interface for the CWorkloadTreeList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKLOADMSG_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)
#define AFX_WORKLOADMSG_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////

// WPARAM = , LPARAM = 
const UINT WM_WLC_PREFSHELP				= ::RegisterWindowMessage(_T("WM_WLC_PREFSHELP"));

// WPARAM = LPCTSTR (Task Icon), LPARAM = int* (imageIndex), return HIMAGELIST
const UINT WM_WLC_GETTASKICON			= ::RegisterWindowMessage(_T("WM_WLC_GETTASKICON"));

// WPARAM = 0, LPARAM = IUITASKMOVE*
const UINT WM_WLC_MOVETASK				= ::RegisterWindowMessage(_T("WM_WLC_MOVETASK"));

// WPARAM = 0, LPARAM = Task ID
const UINT WM_WLC_EDITTASKTITLE			= ::RegisterWindowMessage(_T("WM_WLC_EDITTASKTITLE"));

// WPARAM = 0, LPARAM = Task ID
const UINT WM_WLC_EDITTASKICON			= ::RegisterWindowMessage(_T("WM_WLC_EDITTASKICON"));

// WPARAM = Alloc to name or NULL, LPARAM = Task ID
const UINT WM_WLC_EDITTASKALLOCATIONS	= ::RegisterWindowMessage(_T("WM_WLC_EDITTASKALLOCATIONS"));

/////////////////////////////////////////////////////////////////////////////

// WPARAM = , LPARAM = TRUE/FALSE
const UINT WM_WLCN_COMPLETIONCHANGE		= ::RegisterWindowMessage(_T("WM_WLCN_COMPLETIONCHANGE"));

// WPARAM = , LPARAM = 
const UINT WM_WLCN_SORTCHANGE			= ::RegisterWindowMessage(_T("WM_WLCN_SORTCHANGE"));

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_WORKLOADMSG_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)
