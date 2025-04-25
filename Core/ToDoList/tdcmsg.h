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
const UINT WM_TDCN_COLUMNEDITCLICK			= ::RegisterWindowMessage(_T("WM_TDCN_COLUMNEDITCLICK"));		// lParam == <TDC_COLUMN>
const UINT WM_TDCN_TIMETRACKREMINDER		= ::RegisterWindowMessage(_T("WM_TDCN_TIMETRACKREMINDER"));		// wParam = hwnd, lParam = TaskID
const UINT WM_TDCN_LISTCHANGE				= ::RegisterWindowMessage(_T("WM_TDCN_LISTCHANGE"));			// lParam == <TDC_ATTRIBUTE>
const UINT WM_TDCN_SOURCECONTROLSAVE		= ::RegisterWindowMessage(_T("WM_TDCN_SOURCECONTROLSAVE"));	
const UINT WM_TDCN_FILTERCHANGE				= ::RegisterWindowMessage(_T("WM_TDCN_FILTERCHANGE"));			// wParam = hwnd, 

// request messages
const UINT WM_TDCM_GETCLIPBOARD				= ::RegisterWindowMessage(_T("WM_TDCM_GETCLIPBOARD"));			// lParam == match hwnd
const UINT WM_TDCM_HASCLIPBOARD				= ::RegisterWindowMessage(_T("WM_TDCM_HASCLIPBOARD"));			// lParam == match hwnd
const UINT WM_TDCM_ISTASKDONE				= ::RegisterWindowMessage(_T("WM_TDCM_ISTASKDONE"));			// wParam = taskID, lParam = taskfile path
const UINT WM_TDCM_GETTASKREMINDER			= ::RegisterWindowMessage(_T("WM_TDCM_GETTASKREMINDER"));		// wParam = hwnd, lParam = TaskID
const UINT WM_TDCM_GETLINKTOOLTIP			= ::RegisterWindowMessage(_T("WM_TDCM_GETLINKTOOLTIP"));		// wParam = hwnd, lParam = url
const UINT WM_TDCM_SELECTTASK				= ::RegisterWindowMessage(_T("WM_TDCM_SELECTTASK"));			// wParam = taskID, lParam = taskfile path
const UINT WM_TDCM_COMPLETETASK				= ::RegisterWindowMessage(_T("WM_TDCM_COMPLETETASK"));			// wParam = taskID, lParam = taskfile path
const UINT WM_TDCM_FAILEDLINK				= ::RegisterWindowMessage(_T("WM_TDCM_FAILEDLINK"));			// wParam = hwnd, lParam = url
const UINT WM_TDCM_DISPLAYLINK				= ::RegisterWindowMessage(_T("WM_TDCM_DISPLAYLINK"));			// wParam = hwnd, lParam = url 
const UINT WM_TDCM_LENGTHYOPERATION			= ::RegisterWindowMessage(_T("WM_TDCM_LENGTHYOPERATION"));		// wParam = start/stop, lParam = text to display
const UINT WM_TDCM_IMPORTFROMDROP			= ::RegisterWindowMessage(_T("WM_TDCM_IMPORTFROMDROP"));		// wParam = hwnd, lParam = TDCDROPIMPORT*
const UINT WM_TDCM_CANIMPORTFROMDROP		= ::RegisterWindowMessage(_T("WM_TDCM_CANIMPORTFROMDROP"));		// wParam = hwnd, lParam = TDCDROPIMPORT*

// CTDLTaskAttributeListCtrl
const UINT WM_TDCN_ATTRIBUTEEDITED			= ::RegisterWindowMessage(_T("WM_TDCN_ATTRIBUTEEDITED"));		// wParam = <TDC_ATTRIBUTE>, lParam = TRUE for TDCA_TIMEESTIMATE/SPENT if only the time units changed
const UINT WM_TDCN_AUTOITEMADDEDDELETED		= ::RegisterWindowMessage(_T("WM_TDCN_AUTOITEMADDEDDELETED"));	// wParam = <TDC_ATTRIBUTE>

const UINT WM_TDCM_EDITTASKATTRIBUTE		= ::RegisterWindowMessage(_T("WM_TDCM_EDITTASKATTRIBUTE"));		// wParam = <TDC_ATTRIBUTE>
const UINT WM_TDCM_CLEARTASKATTRIBUTE		= ::RegisterWindowMessage(_T("WM_TDCM_CLEARTASKATTRIBUTE"));	// wParam = <TDC_ATTRIBUTE>
const UINT WM_TDCM_COPYTASKATTRIBUTE		= ::RegisterWindowMessage(_T("WM_TDCM_COPYTASKATTRIBUTE"));		// wParam = <TDC_ATTRIBUTE>
const UINT WM_TDCM_PASTETASKATTRIBUTE		= ::RegisterWindowMessage(_T("WM_TDCM_PASTETASKATTRIBUTE"));	// wParam = <TDC_ATTRIBUTE>
const UINT WM_TDCM_CANCOPYTASKATTRIBUTE		= ::RegisterWindowMessage(_T("WM_TDCM_CANCOPYTASKATTRIBUTE"));	// wParam = <TDC_ATTRIBUTE>
const UINT WM_TDCM_CANPASTETASKATTRIBUTE	= ::RegisterWindowMessage(_T("WM_TDCM_CANPASTETASKATTRIBUTE"));	// wParam = <TDC_ATTRIBUTE>, lParam = TDC_ATTRIBUTE* (from)
const UINT WM_TDCM_ADDTIMETOLOGFILE			= ::RegisterWindowMessage(_T("WM_TDCM_ADDTIMETOLOGFILE"));		
const UINT WM_TDCM_TOGGLETIMETRACKING		= ::RegisterWindowMessage(_T("WM_TDCM_TOGGLETIMETRACKING"));		
const UINT WM_TDCM_SELECTDEPENDENCIES		= ::RegisterWindowMessage(_T("WM_TDCM_SELECTDEPENDENCIES"));		
const UINT WM_TDCM_EDITTASKREMINDER			= ::RegisterWindowMessage(_T("WM_TDCM_EDITTASKREMINDER"));		// wParam = taskID OR zero for 'selected tasks', lParam = taskfile path or empty for selected tasklist
const UINT WM_TDCM_CLEARTASKREMINDER		= ::RegisterWindowMessage(_T("WM_TDCM_CLEARTASKREMINDER"));		// wParam = taskID OR zero for 'selected tasks', lParam = taskfile path or empty for selected tasklist

// CTDLFilterBar
const UINT WM_FBN_FILTERCHANGE				= ::RegisterWindowMessage(_T("WM_FBN_FILTERCHANGE")); 

// CToDoCtrlReminders
const UINT WM_TDCN_REMINDERDISMISSED		= ::RegisterWindowMessage(_T("WM_TDCN_REMINDERDISMISSED"));		// wParam = hwnd, lParam = TaskID
const UINT WM_TDCN_REMINDERSNOOZED			= ::RegisterWindowMessage(_T("WM_TDCN_REMINDERSNOOZED"));		// wParam = hwnd, lParam = TaskID

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
const UINT WM_FTD_DOCKCHANGE				= ::RegisterWindowMessage(_T("WM_FTD_DOCKCHANGE"));				// wparam = Old 'DM_POS', lParam = New 'DM_POS'

// CTDCTimeTrackerDlg
const UINT WM_TDLTTN_STARTTRACKING			= ::RegisterWindowMessage(_T("WM_TDLTTN_STARTTRACKING"));		// wParam = hwnd, lParam = TaskID
const UINT WM_TDLTTN_STOPTRACKING			= ::RegisterWindowMessage(_T("WM_TDLTTN_STOPTRACKING"));		// wParam = hwnd
const UINT WM_TDLTTN_RESETELAPSEDTIME		= ::RegisterWindowMessage(_T("WM_TDLTTN_RESETELAPSEDTIME"));	// wParam = hwnd
const UINT WM_TDLTTN_LOADDELAYEDTASKLIST	= ::RegisterWindowMessage(_T("WM_TDLTTN_LOADDELAYEDTASKLIST")); // wParam = hwnd
const UINT WM_TDLTTN_GOTOTASKLIST			= ::RegisterWindowMessage(_T("WM_TDLTTN_GOTOTASKLIST"));			// wParam = hwnd, lParam = TaskID

// CTDLThreadedExporterWnd
const UINT WM_TDLTE_EXPORTTHREADFINISHED	= ::RegisterWindowMessage(_T("WM_TDLTE_EXPORTTHREADFINISHED")); // wParam = success, lParam = TDCDUETASKNOTIFY*

// CTDLKeyboardShortcutDisplayDlg
const UINT WM_TDLKS_MODIFYSHORTCUTS			= ::RegisterWindowMessage(_T("WM_TDLKS_MODIFYSHORTCUTS")); 

/////////////////////////////////////////////////////////////////////////////


#endif // AFX_TDCMSG_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_