#if !defined(AFX_TDCMSG_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_)
#define AFX_TDCMSG_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// tdcmsg.h : header file
//

// notification messages
const UINT WM_TDCN_MODIFY					= ::RegisterWindowMessage(_T("WM_TDCN_MODIFY"));				// lParam = TDCNOTIFYMOD*
const UINT WM_TDCN_SORT						= ::RegisterWindowMessage(_T("WM_TDCN_SORT")); 
const UINT WM_TDCN_TIMETRACK				= ::RegisterWindowMessage(_T("WM_TDCN_TIMETRACK"));				// wParam = hwnd, lParam = 0/TaskID => stop/start
const UINT WM_TDCN_VIEWPRECHANGE			= ::RegisterWindowMessage(_T("WM_TDCN_VIEWPRECHANGE"));
const UINT WM_TDCN_VIEWPOSTCHANGE			= ::RegisterWindowMessage(_T("WM_TDCN_VIEWPOSTCHANGE"));
const UINT WM_TDCN_SELECTIONCHANGE			= ::RegisterWindowMessage(_T("WM_TDCN_SELECTIONCHANGE"));
const UINT WM_TDCN_SCROLLCHANGE				= ::RegisterWindowMessage(_T("WM_TDCN_SCROLLCHANGE"));
const UINT WM_TDCN_RECREATERECURRINGTASK	= ::RegisterWindowMessage(_T("WM_TDCN_RECREATERECURRINGTASK"));
const UINT WM_TDCN_CLICKREMINDERCOL			= ::RegisterWindowMessage(_T("WM_TDCN_CLICKREMINDERCOL"));
const UINT WM_TDCN_COLUMNEDITCLICK			= ::RegisterWindowMessage(_T("WM_TDCN_COLUMNEDITCLICK"));		// lParam == <TDC_COLUMN>
const UINT WM_TDCN_TIMETRACKREMINDER		= ::RegisterWindowMessage(_T("WM_TDCN_TIMETRACKREMINDER"));		// wParam = hwnd, lParam = TaskID
const UINT WM_TDCN_LISTCHANGE				= ::RegisterWindowMessage(_T("WM_TDCN_LISTCHANGE"));			// lParam == <TDC_ATTRIBUTE>
const UINT WM_TDCN_SOURCECONTROLSAVE		= ::RegisterWindowMessage(_T("WM_TDCN_SOURCECONTROLSAVE"));	

// request messages
const UINT WM_TDCM_GETCLIPBOARD				= ::RegisterWindowMessage(_T("WM_TDCM_GETCLIPBOARD"));			// lParam == match hwnd
const UINT WM_TDCM_HASCLIPBOARD				= ::RegisterWindowMessage(_T("WM_TDCM_HASCLIPBOARD"));			// lParam == match hwnd
const UINT WM_TDCM_ISTASKDONE				= ::RegisterWindowMessage(_T("WM_TDCM_ISTASKDONE"));			// format as WM_TDCM_SELECTTASK
const UINT WM_TDCM_GETTASKREMINDER			= ::RegisterWindowMessage(_T("WM_TDCM_GETTASKREMINDER"));		// wParam = hwnd, lParam = TaskID
const UINT WM_TDCM_GETLINKTOOLTIP			= ::RegisterWindowMessage(_T("WM_TDCM_GETLINKTOOLTIP"));		// wParam = url, lParam = TOOLTIPTEXT*
const UINT WM_TDCM_SELECTTASK				= ::RegisterWindowMessage(_T("WM_TDCM_SELECTTASK"));			// wParam = taskID, lParam = taskfile path
const UINT WM_TDCM_FAILEDLINK				= ::RegisterWindowMessage(_T("WM_TDCM_FAILEDLINK"));			// wParam = hwnd, lParam = url
const UINT WM_TDCM_LENGTHYOPERATION			= ::RegisterWindowMessage(_T("WM_TDCM_LENGTHYOPERATION"));		// wParam = start/stop, lParam = text to display
const UINT WM_TDCM_IMPORTFROMDROP			= ::RegisterWindowMessage(_T("WM_TDCM_IMPORTFROMDROP"));		// wParam = hwnd, lParam = TDCDROPIMPORT*
const UINT WM_TDCM_CANIMPORTFROMDROP		= ::RegisterWindowMessage(_T("WM_TDCM_CANIMPORTFROMDROP"));		// wParam = hwnd, lParam = TDCDROPIMPORT*

// CTDLFilterBar
const UINT WM_FBN_FILTERCHNG				= ::RegisterWindowMessage(_T("WM_FBN_FILTERCHNG")); 

// CToDoCtrlReminders
const UINT WM_TDCN_DISMISSREMINDER			= ::RegisterWindowMessage(_T("WM_TDCN_DISMISSREMINDER"));		// wParam = hwnd, lParam = TaskID
const UINT WM_TDCN_SNOOZEREMINDER			= ::RegisterWindowMessage(_T("WM_TDCN_SNOOZEREMINDER"));		// wParam = hwnd, lParam = TaskID

// CTDLFindTasksDlg
const UINT WM_FTD_FIND						= ::RegisterWindowMessage(_T("WM_FTD_FIND"));
const UINT WM_FTD_SELECTRESULT				= ::RegisterWindowMessage(_T("WM_FTD_SELECTRESULT"));
const UINT WM_FTD_SELECTALL					= ::RegisterWindowMessage(_T("WM_FTD_SELECTALL"));
const UINT WM_FTD_CLOSE						= ::RegisterWindowMessage(_T("WM_FTD_CLOSE"));
const UINT WM_FTD_APPLYASFILTER				= ::RegisterWindowMessage(_T("WM_FTD_APPLYASFILTER"));			// lParam = LPCTSTR
const UINT WM_FTD_ADDSEARCH					= ::RegisterWindowMessage(_T("WM_FTD_ADDSEARCH"));				// lParam = LPCTSTR
const UINT WM_FTD_SAVESEARCH				= ::RegisterWindowMessage(_T("WM_FTD_SAVESEARCH"));				// lParam = LPCTSTR
const UINT WM_FTD_DELETESEARCH				= ::RegisterWindowMessage(_T("WM_FTD_DELETESEARCH"));			// lParam = LPCTSTR
const UINT WM_FTD_GETLISTITEMS				= ::RegisterWindowMessage(_T("WM_FTD_GETLISTITEMS"));			// wParam = <TDC_ATTRIBUTE>, lParam = CStringArray*

// CTDCTimeTrackerDlg
const UINT WM_TDLTTN_STARTTRACKING			= ::RegisterWindowMessage(_T("WM_TDLTTN_STARTTRACKING"));		// wParam = hwnd, lParam = TaskID
const UINT WM_TDLTTN_STOPTRACKING			= ::RegisterWindowMessage(_T("WM_TDLTTN_STOPTRACKING"));		// wParam = hwnd
const UINT WM_TDLTTN_RESETELAPSEDTIME		= ::RegisterWindowMessage(_T("WM_TDLTTN_RESETELAPSEDTIME"));	// wParam = hwnd
const UINT WM_TDLTTN_LOADDELAYEDTASKLIST	= ::RegisterWindowMessage(_T("WM_TDLTTN_LOADDELAYEDTASKLIST")); // wParam = hwnd

// CTDLThreadedExporterWnd
const UINT WM_TDLTE_EXPORTTHREADFINISHED	= ::RegisterWindowMessage(_T("WM_TDLTE_EXPORTTHREADFINISHED")); // wParam = success, lParam = TDCDUETASKNOTIFY*

// CTDLKeyboardShortcutDisplayDlg
const UINT WM_TDLKS_MODIFYSHORTCUTS			= ::RegisterWindowMessage(_T("WM_TDLKS_MODIFYSHORTCUTS")); 

/////////////////////////////////////////////////////////////////////////////


#endif // AFX_TDCMSG_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_