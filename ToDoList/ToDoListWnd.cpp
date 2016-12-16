// ToDoListWnd.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ToDoListWnd.h"
#include "tdcToolsCmdlineParser.h"
#include "tdlToolsUserInputDlg.h"
#include "tdcToolshelper.h"
#include "tdlexportDlg.h"
#include "tasklisthtmlexporter.h"
#include "tasklisttxtexporter.h"
#include "tdcmsg.h"
#include "tdcmapping.h"
#include "tdlschemadef.h"
#include "tdlprintdialog.h"
#include "tdltransformdialog.h"
#include "tdstringres.h"
#include "tdlcolumnselectiondlg.h"
#include "tdlfilterdlg.h"
#include "tdlOffsetDatesDlg.h"
#include "tdlKeyboardShortcutDisplayDlg.h"
#include "tdlimportdialog.h"
#include "tdlsetreminderdlg.h"
#include "tdlshowreminderdlg.h"
#include "tdlmultisortdlg.h"
#include "multitaskfile.h"
#include "tdcstatic.h"
#include "tdlcustomattributedlg.h"
#include "tdccustomattributehelper.h"
#include "tdlwelcomewizard.h"
#include "tdlpastefromclipboarddlg.h"
#include "tdcuiextensionhelper.h"
#include "tdlanalyseloggedtimedlg.h"
#include "tasktimelog.h"
#include "TDLAboutDlg.h"
#include "TDCWebUpdateScript.h"
#include "TDCToDoCtrlPreferenceHelper.h"

#include "..\shared\aboutdlg.h"
#include "..\shared\holdredraw.h"
#include "..\shared\autoflag.h"
#include "..\shared\enbitmap.h"
#include "..\shared\spellcheckdlg.h"
#include "..\shared\encolordialog.h"
#include "..\shared\winclasses.h"
#include "..\shared\wclassdefines.h"
#include "..\shared\datehelper.h"
#include "..\shared\osversion.h"
#include "..\shared\enfiledialog.h"
#include "..\shared\misc.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\filemisc.h"
#include "..\shared\webmisc.h"
#include "..\shared\themed.h"
#include "..\shared\enstring.h"
#include "..\shared\fileregister.h"
#include "..\shared\mousewheelMgr.h"
#include "..\shared\editshortcutMgr.h"
#include "..\shared\dlgunits.h"
#include "..\shared\passworddialog.h"
#include "..\shared\sysimagelist.h"
#include "..\shared\regkey.h"
#include "..\shared\uiextensionuihelper.h"
#include "..\shared\remotefile.h"
#include "..\shared\serverdlg.h"
#include "..\shared\focuswatcher.h"
#include "..\shared\localizer.h"
#include "..\shared\msoutlookhelper.h"
#include "..\shared\clipboard.h"
#include "..\shared\rtlstylemgr.h"
#include "..\shared\xslfile.h"

#include "..\3rdparty\gui.h"
#include "..\3rdparty\sendfileto.h"

#include <shlwapi.h>
#include <windowsx.h>
#include <direct.h>
#include <math.h>

#include <afxpriv.h>        // for WM_KICKIDLE

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const int TD_VERSION = 31000;
const int BEVEL = 3; // pixels
const int BORDER = 3; // pixels
const int MAX_NUM_TOOLS = 50;

#ifdef _DEBUG
const UINT ONE_MINUTE = 10000;
#else
const UINT ONE_MINUTE = 60000;
#endif

const LPCTSTR SETTINGS_KEY	= _T("Settings");
const LPCTSTR PREF_KEY		= _T("Preferences");
const LPCTSTR ENDL			= _T("\n");

/////////////////////////////////////////////////////////////////////////////

#define DOPROGRESS(stringID) \
	CWaitCursor cursor; \
	CStatusBarProgressProxy prog(&m_sbProgress, m_statusBar, CEnString(stringID))

/////////////////////////////////////////////////////////////////////////////

enum
{
	WM_POSTONCREATE = (WM_APP+1),
	WM_WEBUPDATEWIZARD,
	WM_ADDTOOLBARTOOLS,
	WM_APPRESTOREFOCUS,
	WM_DOINITIALDUETASKNOTIFY,
	WM_DUETASKTHREADFINISHED,
};

enum 
{
	TIMER_READONLYSTATUS = 1,
	TIMER_TIMESTAMPCHANGE,
	TIMER_AUTOSAVE,
	TIMER_CHECKOUTSTATUS,
	TIMER_DUEITEMS,
	TIMER_TIMETRACKING,
	TIMER_AUTOMINIMIZE,
	TIMER_TIMETRACKREMINDER,
};

enum 
{
	INTERVAL_READONLYSTATUS		= 1000,
	INTERVAL_TIMESTAMPCHANGE	= 10000,
	//	INTERVAL_AUTOSAVE		= dynamically determined
	INTERVAL_CHECKOUTSTATUS		= 5000,
	INTERVAL_DUEITEMS			= ONE_MINUTE,
	INTERVAL_TIMETRACKING		= 5000,
	//	INTERVAL_AUTOMINIMIZE	= dynamically determined
};

/////////////////////////////////////////////////////////////////////////////

#ifndef WM_WTSSESSION_CHANGE
#	define WM_WTSSESSION_CHANGE	0x02B1
#	define WTS_SESSION_LOCK		0x7
#	define WTS_SESSION_UNLOCK	0x8
#endif

/////////////////////////////////////////////////////////////////////////////
// CToDoListWnd 

BOOL CToDoListWnd::s_bRestoreExportSpaceForNotes = -1;

/////////////////////////////////////////////////////////////////////////////

CToDoListWnd::CToDoListWnd() 
	: 
	CFrameWnd(), 
	m_bVisible(-1), 
	m_mruList(0, _T("MRU"), _T("TaskList%d"), 16, AFX_ABBREV_FILENAME_LEN, CEnString(IDS_RECENTFILES)),
	m_nLastSelItem(-1), 
	m_nMaxState(TDCMS_NORMAL), 
	m_nPrevMaxState(TDCMS_NORMAL),
	m_bShowFilterBar(TRUE),
	m_bShowStatusBar(TRUE),
	m_bInNewTask(FALSE),
	m_bSaving(FALSE),
	m_mgrShortcuts(FALSE),
	m_pPrefs(NULL),
	m_bClosing(FALSE),
	m_tabCtrl(TCE_POSTDRAW | TCE_MBUTTONCLOSE | TCE_DRAGDROP | TCE_CLOSEBUTTON | TCE_BOLDSELTEXT),
	m_mgrToDoCtrls(m_tabCtrl),
	m_bFindShowing(FALSE),
	m_bShowProjectName(TRUE),
	m_bQueryOpenAllow(FALSE),
	m_bPasswordPrompting(TRUE),
	m_bShowToolbar(TRUE),
	m_bReloading(FALSE),
	m_hwndLastFocus(NULL),
	m_bStartHidden(FALSE),
	m_cbQuickFind(ACBS_ALLOWDELETE | ACBS_ADDTOSTART),
	m_bShowTasklistBar(TRUE), 
	m_bShowTreeListBar(TRUE),
	m_bEndingSession(FALSE),
	m_nContextColumnID(TDCC_NONE),
	m_nNumDueTaskThreads(0),
	m_bReshowTimeTrackerOnEnable(FALSE)
{
	// must do this before initializing any controls
	SetupUIStrings();

	// init preferences
	ResetPrefs();

	// RTL keyboard input
	if (Prefs().GetEnableRTLInput())
		CRTLStyleMgr::Initialize();

	CFilteredToDoCtrl::EnableExtendedSelection(FALSE, TRUE);

	m_bAutoMenuEnable = FALSE;

	m_cbQuickFind.SetMinDLUHeight(8);
}

CToDoListWnd::~CToDoListWnd()
{
	delete m_pPrefs;

	// cleanup temp files
	// Note: Due task notifications are removed by CToDoCtrlMgr
	FileMisc::DeleteFile(FileMisc::GetTempFilePath(_T("ToDoList.print"), _T("html")), TRUE);
	FileMisc::DeleteFile(FileMisc::GetTempFilePath(_T("ToDoList.import"), _T("txt")), TRUE);
	FileMisc::DeleteFile(FileMisc::GetTempFilePath(_T("tdt")), TRUE);
}

BEGIN_MESSAGE_MAP(CToDoListWnd, CFrameWnd)
//{{AFX_MSG_MAP(CToDoListWnd)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_VIEW_SHOWTIMETRACKER, OnViewShowTimeTracker)
	ON_WM_NCLBUTTONDBLCLK()
	ON_COMMAND(ID_TOOLS_SELECTINEXPLORER, OnToolsSelectinExplorer)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_SELECTINEXPLORER, OnUpdateToolsSelectinExplorer)
	ON_COMMAND(ID_VIEW_RESIZECOLSTOFIT, OnViewResizeColsToFit)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESIZECOLSTOFIT, OnUpdateViewResizeColsToFit)
	ON_CBN_EDITCHANGE(IDC_QUICKFIND, OnEditChangeQuickFind)
	ON_CBN_SELCHANGE(IDC_QUICKFIND, OnSelChangeQuickFind)
	ON_COMMAND(ID_ABOUT, OnAbout)
	ON_COMMAND(ID_ADDTIMETOLOGFILE, OnAddtimetologfile)
	ON_COMMAND(ID_ADDTIMETOLOGFILE, OnAddtimetologfile)
	ON_COMMAND(ID_ARCHIVE_COMPLETEDTASKS, OnArchiveCompletedtasks)
	ON_COMMAND(ID_ARCHIVE_SELECTEDTASKS, OnArchiveSelectedTasks)
	ON_COMMAND(ID_CLOSE, OnCloseTasklist)
	ON_COMMAND(ID_CLOSEALL, OnCloseall)
	ON_COMMAND(ID_CLOSEALLBUTTHIS, OnCloseallbutthis)
	ON_COMMAND(ID_DELETEALLTASKS, OnDeleteAllTasks)
	ON_COMMAND(ID_DELETETASK, OnDeleteTask)
	ON_COMMAND(ID_EDIT_CLEARFIELD, OnEditClearAttribute)
	ON_COMMAND(ID_EDIT_CLEARFOCUSEDFIELD, OnEditClearFocusedAttribute)
	ON_COMMAND(ID_EDIT_CLEARREMINDER, OnEditClearReminder)
	ON_COMMAND(ID_EDIT_CLEARTASKCOLOR, OnEditCleartaskcolor)
	ON_COMMAND(ID_EDIT_CLEARTASKICON, OnEditCleartaskicon)
	ON_COMMAND(ID_EDIT_CLOCK_TASK, OnEditTimeTrackTask)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_COPYAS_DEPEND, OnCopyTaskasDependency)
	ON_COMMAND(ID_EDIT_COPYAS_DEPENDFULL, OnCopyTaskasDependencyFull)
	ON_COMMAND(ID_EDIT_COPYAS_HTML, OnEditCopyashtml)
	ON_COMMAND(ID_EDIT_COPYAS_LINK, OnCopyTaskasLink)
	ON_COMMAND(ID_EDIT_COPYAS_LINKFULL, OnCopyTaskasLinkFull)
	ON_COMMAND(ID_EDIT_COPYAS_PATH, OnCopyTaskasPath)
	ON_COMMAND(ID_EDIT_COPYAS_TEXT, OnEditCopyastext)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_DECTASKPERCENTDONE, OnEditDectaskpercentdone)
	ON_COMMAND(ID_EDIT_DECTASKPRIORITY, OnEditDectaskpriority)
	ON_COMMAND(ID_EDIT_FINDTASKS, OnFindTasks)
	ON_COMMAND(ID_EDIT_FLAGTASK, OnEditFlagtask)
	ON_COMMAND(ID_EDIT_GOTODEPENDENCY, OnEditGotoDependency)
	ON_COMMAND(ID_EDIT_INCTASKPERCENTDONE, OnEditInctaskpercentdone)
	ON_COMMAND(ID_EDIT_INCTASKPRIORITY, OnEditInctaskpriority)
	ON_COMMAND(ID_EDIT_INSERTDATE, OnEditInsertdate)
	ON_COMMAND(ID_EDIT_INSERTDATETIME, OnEditInsertdatetime)
	ON_COMMAND(ID_EDIT_INSERTTIME, OnEditInserttime)
	ON_COMMAND(ID_EDIT_OFFSETDATES, OnEditOffsetdates)
	ON_COMMAND(ID_EDIT_PASTEAFTER, OnEditPasteAfter)
	ON_COMMAND(ID_EDIT_PASTEASREF, OnEditPasteAsRef)
	ON_COMMAND(ID_EDIT_PASTESUB, OnEditPasteSub)
	ON_COMMAND(ID_EDIT_QUICKFIND, OnQuickFind)
	ON_COMMAND(ID_EDIT_QUICKFINDNEXT, OnQuickFindNext)
	ON_COMMAND(ID_EDIT_QUICKFINDPREV, OnQuickFindPrev)
	ON_COMMAND(ID_EDIT_RECURRENCE, OnEditRecurrence)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_EDIT_SELECTALL, OnEditSelectall)
	ON_COMMAND(ID_EDIT_SELECTREFERENCETARGET, OnEditSelectReferenceTarget)
	ON_COMMAND(ID_EDIT_SELECTTASKREFERENCES, OnEditSelectTaskReferences)
	ON_COMMAND(ID_EDIT_SETFILEREF, OnEditAddFileLink)
	ON_COMMAND(ID_EDIT_SETREMINDER, OnEditSetReminder)
	ON_COMMAND(ID_EDIT_SETTASKICON, OnEditSettaskicon)
	ON_COMMAND(ID_EDIT_SPELLCHECKCOMMENTS, OnSpellcheckcomments)
	ON_COMMAND(ID_EDIT_SPELLCHECKTITLE, OnSpellchecktitle)
	ON_COMMAND(ID_EDIT_TASKCOLOR, OnEditTaskcolor)
	ON_COMMAND(ID_EDIT_TASKDONE, OnEditTaskdone)
	ON_COMMAND(ID_EDIT_TASKTEXT, OnEditTasktext)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EXIT, OnExit)
	ON_COMMAND(ID_FILE_CHANGEPASSWORD, OnFileChangePassword)
	ON_COMMAND(ID_FILE_ENCRYPT, OnFileEncrypt)
	ON_COMMAND(ID_FILE_OPENARCHIVE, OnFileOpenarchive)
	ON_COMMAND(ID_FILE_RESETVERSION, OnFileResetversion)
	ON_COMMAND(ID_HELP_KEYBOARDSHORTCUTS, OnShowKeyboardshortcuts)
	ON_COMMAND(ID_LOAD_NORMAL, OnLoad)
	ON_COMMAND(ID_MAXCOMMENTS, OnMaximizeComments)
	ON_COMMAND(ID_MAXTASKLIST, OnMaximizeTasklist)
	ON_COMMAND(ID_MINIMIZETOTRAY, OnMinimizeToTray)
	ON_COMMAND(ID_MOVETASKDOWN, OnMovetaskdown)
	ON_COMMAND(ID_MOVETASKLEFT, OnMovetaskleft)
	ON_COMMAND(ID_MOVETASKRIGHT, OnMovetaskright)
	ON_COMMAND(ID_MOVETASKUP, OnMovetaskup)
	ON_COMMAND(ID_NEW, OnNew)
	ON_COMMAND(ID_NEWSUBTASK, OnNewsubtask)
	ON_COMMAND(ID_NEWSUBTASK_ATBOTTOM, OnNewsubtaskAtbottom)
	ON_COMMAND(ID_NEWSUBTASK_ATTOP, OnNewsubtaskAttop)
	ON_COMMAND(ID_NEWTASK, OnNewtask)
	ON_COMMAND(ID_NEWTASK_AFTERSELECTEDTASK, OnNewtaskAfterselectedtask)
	ON_COMMAND(ID_NEWTASK_DEPENDENTAFTERSELECTEDTASK, OnNewDependenttaskAfterselectedtask)
	ON_COMMAND(ID_NEWTASK_ATBOTTOM, OnNewtaskAtbottom)
	ON_COMMAND(ID_NEWTASK_ATBOTTOMSELECTED, OnNewtaskAtbottomSelected)
	ON_COMMAND(ID_NEWTASK_ATTOP, OnNewtaskAttop)
	ON_COMMAND(ID_NEWTASK_ATTOPSELECTED, OnNewtaskAttopSelected)
	ON_COMMAND(ID_NEWTASK_BEFORESELECTEDTASK, OnNewtaskBeforeselectedtask)
	ON_COMMAND(ID_NEXTTASK, OnGotoNexttask)
	ON_COMMAND(ID_NEXTTOPLEVELTASK, OnNexttopleveltask)
	ON_COMMAND(ID_OPEN_RELOAD, OnReload)
	ON_COMMAND(ID_PREFERENCES, OnPreferences)
	ON_COMMAND(ID_PREVTASK, OnGotoPrevtask)
	ON_COMMAND(ID_PREVTOPLEVELTASK, OnPrevtopleveltask)
	ON_COMMAND(ID_PRINT, OnPrint)
	ON_COMMAND(ID_PRINTPREVIEW, OnPrintpreview)
	ON_COMMAND(ID_SAVEALL, OnSaveall)
	ON_COMMAND(ID_SAVEAS, OnSaveas)
	ON_COMMAND(ID_SAVE_NORMAL, OnSave)
	ON_COMMAND(ID_SENDTASKS, OnSendTasks)
	ON_COMMAND(ID_SEND_SELTASKS, OnSendSelectedTasks)
	ON_COMMAND(ID_SHOWTIMELOGFILE, OnShowTimelogfile)
	ON_COMMAND(ID_SORT, OnSort)
	ON_COMMAND(ID_SORT_BYMULTI, OnSortMulti)
	ON_COMMAND(ID_TABCTRL_PREFERENCES, OnTabctrlPreferences)
	ON_COMMAND(ID_TASKLIST_CUSTOMCOLUMNS, OnTasklistCustomColumns)
	ON_COMMAND(ID_TASKLIST_SELECTCOLUMNS, OnTasklistSelectColumns)
	ON_COMMAND(ID_TOOLS_ANALYSELOGGEDTIME, OnToolsAnalyseLoggedTime)
	ON_COMMAND(ID_TOOLS_CHECKIN, OnToolsCheckin)
	ON_COMMAND(ID_TOOLS_CHECKOUT, OnToolsCheckout)
	ON_COMMAND(ID_TOOLS_EXPORT, OnExport)
	ON_COMMAND(ID_TOOLS_IMPORT, OnImportTasklist)
	ON_COMMAND(ID_TOOLS_SPELLCHECKTASKLIST, OnSpellcheckTasklist)
	ON_COMMAND(ID_TOOLS_TOGGLECHECKIN, OnToolsToggleCheckin)
	ON_COMMAND(ID_TOOLS_TRANSFORM, OnToolsTransformactivetasklist)
	ON_COMMAND(ID_TRAYICON_CLOSE, OnTrayiconClose)
	ON_COMMAND(ID_TRAYICON_CREATETASK, OnTrayiconCreatetask)
	ON_COMMAND(ID_TRAYICON_SHOW, OnTrayiconShow)
	ON_COMMAND(ID_TRAYICON_SHOWTIMETRACKER, OnViewShowTimeTracker)
	ON_COMMAND(ID_VIEW_CLEARFILTER, OnViewClearfilter)
	ON_COMMAND(ID_VIEW_COLLAPSEALL, OnViewCollapseall)
	ON_COMMAND(ID_VIEW_COLLAPSEDUE, OnViewCollapseDuetasks)
	ON_COMMAND(ID_VIEW_COLLAPSESTARTED, OnViewCollapseStartedtasks)
	ON_COMMAND(ID_VIEW_COLLAPSETASK, OnViewCollapsetask)
	ON_COMMAND(ID_VIEW_CYCLETASKVIEWS, OnViewCycleTaskViews)
	ON_COMMAND(ID_VIEW_EXPANDALL, OnViewExpandall)
	ON_COMMAND(ID_VIEW_EXPANDDUE, OnViewExpandDuetasks)
	ON_COMMAND(ID_VIEW_EXPANDSTARTED, OnViewExpandStartedtasks)
	ON_COMMAND(ID_VIEW_EXPANDTASK, OnViewExpandtask)
	ON_COMMAND(ID_VIEW_FILTER, OnViewFilter)
	ON_COMMAND(ID_VIEW_MOVETASKLISTLEFT, OnViewMovetasklistleft)
	ON_COMMAND(ID_VIEW_MOVETASKLISTRIGHT, OnViewMovetasklistright)
	ON_COMMAND(ID_VIEW_NEXT, OnViewNext)
	ON_COMMAND(ID_VIEW_NEXT_SEL, OnViewNextSel)
	ON_COMMAND(ID_VIEW_PREV, OnViewPrev)
	ON_COMMAND(ID_VIEW_PREV_SEL, OnViewPrevSel)
	ON_COMMAND(ID_VIEW_PROJECTNAME, OnViewProjectname)
	ON_COMMAND(ID_VIEW_REFRESHFILTER, OnViewRefreshfilter)
	ON_COMMAND(ID_VIEW_SHOWFILTERBAR, OnViewShowfilterbar)
	ON_COMMAND(ID_VIEW_SHOWTASKLISTTABBAR, OnViewShowTasklistTabbar) 
	ON_COMMAND(ID_VIEW_SHOWTREELISTTABBAR, OnViewShowTreeListTabbar)
	ON_COMMAND(ID_VIEW_SORTTASKLISTTABS, OnViewSorttasklisttabs)
	ON_COMMAND(ID_VIEW_STATUS_BAR, OnViewStatusBar)
	ON_COMMAND(ID_VIEW_TOGGLEFILTER, OnViewTogglefilter)
	ON_COMMAND(ID_VIEW_TOGGLETASKEXPANDED, OnViewToggletaskexpanded)
	ON_COMMAND(ID_VIEW_TOGGLETASKSANDCOMMENTS, OnViewToggletasksandcomments)
	ON_COMMAND(ID_VIEW_TOGGLETREEANDLIST, OnViewToggleTreeandList)
	ON_COMMAND(ID_VIEW_TOOLBAR, OnViewToolbar)
	ON_COMMAND_EX_RANGE(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE16, OnOpenRecentFile)
	ON_COMMAND_RANGE(ID_EDIT_OPENFILEREF1, ID_EDIT_OPENFILEREF16, OnEditOpenfileref)
	ON_COMMAND_RANGE(ID_EDIT_SETPRIORITYNONE, ID_EDIT_SETPRIORITY10, OnSetPriority)
	ON_COMMAND_RANGE(ID_FILE_OPEN_USERSTORAGE1, ID_FILE_OPEN_USERSTORAGE16, OnFileOpenFromUserStorage)
	ON_COMMAND_RANGE(ID_FILE_SAVE_USERSTORAGE1, ID_FILE_SAVE_USERSTORAGE16, OnFileSaveToUserStorage)
	ON_COMMAND_RANGE(ID_NEWTASK_SPLITTASKINTO_TWO, ID_NEWTASK_SPLITTASKINTO_FIVE, OnSplitTaskIntoPieces)
	ON_COMMAND_RANGE(ID_SHOWVIEW_TASKTREE, ID_SHOWVIEW_UIEXTENSION16, OnShowTaskView)
	ON_COMMAND_RANGE(ID_SORT_BYFIRST, ID_SORT_BYLAST, OnSortBy)
	ON_COMMAND_RANGE(ID_TOOLS_SHOWTASKS_DUETODAY, ID_TOOLS_SHOWTASKS_DUEENDNEXTMONTH, OnToolsShowtasksDue)
	ON_COMMAND_RANGE(ID_TOOLS_USERTOOL1, ID_TOOLS_USERTOOL16, OnUserTool)
	ON_COMMAND_RANGE(ID_TRAYICON_SHOWDUETASKS1, ID_TRAYICON_SHOWDUETASKS20, OnTrayiconShowDueTasks)
	ON_COMMAND_RANGE(ID_WINDOW1, ID_WINDOW16, OnWindow)
	ON_MESSAGE(WM_ADDTOOLBARTOOLS, OnAddToolbarTools)
	ON_MESSAGE(WM_APPRESTOREFOCUS, OnAppRestoreFocus)
	ON_MESSAGE(WM_CLOSE, OnClose)
	ON_MESSAGE(WM_DUETASKTHREADFINISHED, OnDueTaskThreadFinished)
	ON_MESSAGE(WM_DOINITIALDUETASKNOTIFY, OnDoInitialDueTaskNotify)
	ON_MESSAGE(WM_GETFONT, OnGetFont)
	ON_MESSAGE(WM_GETICON, OnGetIcon)
	ON_MESSAGE(WM_HOTKEY, OnHotkey)
	ON_MESSAGE(WM_POSTONCREATE, OnPostOnCreate)
	ON_NOTIFY(NM_CLICK, IDC_TRAYICON, OnTrayIconClick) 
	ON_NOTIFY(NM_DBLCLK, IDC_TRAYICON, OnTrayIconDblClk)
	ON_NOTIFY(NM_RCLICK, IDC_TRAYICON, OnTrayIconRClick)
	ON_NOTIFY(TCN_CLOSETAB, IDC_TABCONTROL, OnTabCtrlCloseTab)
	ON_NOTIFY(TCN_ENDDRAG, IDC_TABCONTROL, OnTabCtrlEndDrag)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCONTROL, OnTabCtrlSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TABCONTROL, OnTabCtrlSelchanging)
	ON_NOTIFY(TTN_NEEDTEXT, 0, OnNeedTooltipText)
	ON_REGISTERED_MESSAGE(WM_ACBN_ITEMADDED, OnQuickFindItemAdded)
	ON_REGISTERED_MESSAGE(WM_FBN_FILTERCHNG, OnSelchangeFilter)
	ON_REGISTERED_MESSAGE(WM_FTD_ADDSEARCH, OnFindAddSearch)
	ON_REGISTERED_MESSAGE(WM_FTD_SAVESEARCH, OnFindSaveSearch)
	ON_REGISTERED_MESSAGE(WM_FTD_APPLYASFILTER, OnFindApplyAsFilter)
	ON_REGISTERED_MESSAGE(WM_FTD_CLOSE, OnFindDlgClose)
	ON_REGISTERED_MESSAGE(WM_FTD_DELETESEARCH, OnFindDeleteSearch)
	ON_REGISTERED_MESSAGE(WM_FTD_FIND, OnFindDlgFind)
	ON_REGISTERED_MESSAGE(WM_FTD_SELECTALL, OnFindSelectAll)
	ON_REGISTERED_MESSAGE(WM_FTD_SELECTRESULT, OnFindSelectResult)
	ON_REGISTERED_MESSAGE(WM_FW_FOCUSCHANGE, OnFocusChange)
	ON_REGISTERED_MESSAGE(WM_PGP_CLEARMRU, OnPreferencesClearMRU)
	ON_REGISTERED_MESSAGE(WM_PTP_TESTTOOL, OnPreferencesTestTool)
	ON_REGISTERED_MESSAGE(WM_TDCM_FAILEDLINK, OnTodoCtrlFailedLink)
	ON_REGISTERED_MESSAGE(WM_TDCM_LENGTHYOPERATION, OnToDoCtrlDoLengthyOperation)
	ON_REGISTERED_MESSAGE(WM_TDCM_GETTASKREMINDER, OnToDoCtrlGetTaskReminder)
	ON_REGISTERED_MESSAGE(WM_TDCM_ISTASKDONE, OnToDoCtrlIsTaskDone)
	ON_REGISTERED_MESSAGE(WM_TDCM_TASKLINK, OnToDoCtrlDoTaskLink)
	ON_REGISTERED_MESSAGE(WM_TDCN_CLICKREMINDERCOL, OnToDoCtrlNotifyClickReminderCol)
	ON_REGISTERED_MESSAGE(WM_TDCN_LISTCHANGE, OnToDoCtrlNotifyListChange)
	ON_REGISTERED_MESSAGE(WM_TDCN_MODIFY, OnToDoCtrlNotifyMod)
	ON_REGISTERED_MESSAGE(WM_TDCN_RECREATERECURRINGTASK, OnToDoCtrlNotifyRecreateRecurringTask)
	ON_REGISTERED_MESSAGE(WM_TDCN_TIMETRACK, OnToDoCtrlNotifyTimeTrack)
	ON_REGISTERED_MESSAGE(WM_TDCN_VIEWPOSTCHANGE, OnToDoCtrlNotifyViewChange)
	ON_REGISTERED_MESSAGE(WM_TDL_GETVERSION , OnToDoListGetVersion)
	ON_REGISTERED_MESSAGE(WM_TDL_ISCLOSING , OnToDoListIsClosing)
	ON_REGISTERED_MESSAGE(WM_TDL_REFRESHPREFS , OnToDoListRefreshPrefs)
	ON_REGISTERED_MESSAGE(WM_TDL_RESTORE , OnToDoListRestore)
	ON_REGISTERED_MESSAGE(WM_TDL_SHOWWINDOW , OnToDoListShowWindow)
	ON_REGISTERED_MESSAGE(WM_TDCN_DISMISSREMINDER , OnDismissReminder)
	ON_REGISTERED_MESSAGE(WM_TLDT_DROP, OnDropFile)
	ON_REGISTERED_MESSAGE(WM_TDLTTN_STARTTRACKING, OnTimeTrackerStartTracking)
	ON_REGISTERED_MESSAGE(WM_TDLTTN_STOPTRACKING, OnTimeTrackerStopTracking)
	ON_REGISTERED_MESSAGE(WM_TDLTTN_LOADDELAYEDTASKLIST, OnTimeTrackerLoadDelayedTasklist)
	ON_REGISTERED_MESSAGE(WM_SESSIONSTATUS_CHANGE, OnSessionStatusChange)
	ON_UPDATE_COMMAND_UI(ID_ADDTIMETOLOGFILE, OnUpdateAddtimetologfile)
	ON_UPDATE_COMMAND_UI(ID_ADDTIMETOLOGFILE, OnUpdateAddtimetologfile)
	ON_UPDATE_COMMAND_UI(ID_ARCHIVE_COMPLETEDTASKS, OnUpdateArchiveCompletedtasks)
	ON_UPDATE_COMMAND_UI(ID_ARCHIVE_SELECTEDTASKS, OnUpdateArchiveSelectedCompletedTasks)
	ON_UPDATE_COMMAND_UI(ID_CLOSEALL, OnUpdateCloseall)
	ON_UPDATE_COMMAND_UI(ID_CLOSEALLBUTTHIS, OnUpdateCloseallbutthis)
	ON_UPDATE_COMMAND_UI(ID_DELETEALLTASKS, OnUpdateDeletealltasks) 
	ON_UPDATE_COMMAND_UI(ID_DELETETASK, OnUpdateDeletetask)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEARFIELD, OnUpdateEditClearAttribute)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEARFOCUSEDFIELD, OnUpdateEditClearFocusedAttribute)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEARREMINDER, OnUpdateEditClearReminder)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEARTASKCOLOR, OnUpdateEditCleartaskcolor)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEARTASKICON, OnUpdateEditCleartaskicon)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLOCK_TASK, OnUpdateEditTimeTrackTask)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPYAS_DEPEND, OnUpdateCopyTaskasDependency)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPYAS_DEPENDFULL, OnUpdateCopyTaskasDependencyFull)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPYAS_HTML, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPYAS_LINK, OnUpdateCopyTaskasLink)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPYAS_LINKFULL, OnUpdateCopyTaskasLinkFull)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPYAS_PATH, OnUpdateCopyTaskasPath)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPYAS_TEXT, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DECTASKPERCENTDONE, OnUpdateEditDectaskpercentdone)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DECTASKPRIORITY, OnUpdateEditDectaskpriority)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FLAGTASK, OnUpdateEditFlagtask)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GOTODEPENDENCY, OnUpdateEditGotoDependency)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INCTASKPERCENTDONE, OnUpdateEditInctaskpercentdone)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INCTASKPRIORITY, OnUpdateEditInctaskpriority)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INSERTDATE, OnUpdateEditInsertdate)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INSERTDATETIME, OnUpdateEditInsertdatetime)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INSERTTIME, OnUpdateEditInserttime)
	ON_UPDATE_COMMAND_UI(ID_EDIT_OFFSETDATES, OnUpdateEditOffsetdates)
	ON_UPDATE_COMMAND_UI(ID_EDIT_OPENFILEREF1, OnUpdateEditOpenfileref)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTEAFTER, OnUpdateEditPasteAfter)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTEASREF, OnUpdateEditPasteAsRef)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTESUB, OnUpdateEditPasteSub)
	ON_UPDATE_COMMAND_UI(ID_EDIT_QUICKFIND, OnUpdateQuickFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_QUICKFINDNEXT, OnUpdateQuickFindNext)
	ON_UPDATE_COMMAND_UI(ID_EDIT_QUICKFINDPREV, OnUpdateQuickFindPrev)
	ON_UPDATE_COMMAND_UI(ID_EDIT_RECURRENCE, OnUpdateEditRecurrence)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECTALL, OnUpdateEditSelectall)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECTREFERENCETARGET, OnUpdateEditSelectReferenceTarget)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECTTASKREFERENCES, OnUpdateEditSelectTaskReferences)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SETFILEREF, OnUpdateEditAddFileLink)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SETREMINDER, OnUpdateEditSetReminder)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SETTASKICON, OnUpdateEditSettaskicon)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SPELLCHECKCOMMENTS, OnUpdateSpellcheckcomments)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SPELLCHECKTITLE, OnUpdateSpellchecktitle)
	ON_UPDATE_COMMAND_UI(ID_EDIT_TASKCOLOR, OnUpdateTaskcolor)
	ON_UPDATE_COMMAND_UI(ID_EDIT_TASKDONE, OnUpdateEditTaskdone)
	ON_UPDATE_COMMAND_UI(ID_EDIT_TASKTEXT, OnUpdateEditTasktext)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_FILE_CHANGEPASSWORD, OnUpdateFileChangePassword)
	ON_UPDATE_COMMAND_UI(ID_FILE_ENCRYPT, OnUpdateFileEncrypt)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE1, OnUpdateRecentFileMenu)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPENARCHIVE, OnUpdateFileOpenarchive)
	ON_UPDATE_COMMAND_UI(ID_FILE_RESETVERSION, OnUpdateFileResetversion)
	ON_UPDATE_COMMAND_UI(ID_MAXCOMMENTS, OnUpdateMaximizeComments)
	ON_UPDATE_COMMAND_UI(ID_MAXTASKLIST, OnUpdateMaximizeTasklist)
	ON_UPDATE_COMMAND_UI(ID_MOVETASKDOWN, OnUpdateMovetaskdown)
	ON_UPDATE_COMMAND_UI(ID_MOVETASKLEFT, OnUpdateMovetaskleft)
	ON_UPDATE_COMMAND_UI(ID_MOVETASKRIGHT, OnUpdateMovetaskright)
	ON_UPDATE_COMMAND_UI(ID_MOVETASKUP, OnUpdateMovetaskup)
	ON_UPDATE_COMMAND_UI(ID_NEW, OnUpdateNew)
	ON_UPDATE_COMMAND_UI(ID_NEWSUBTASK, OnUpdateNewsubtask)
	ON_UPDATE_COMMAND_UI(ID_NEWSUBTASK_ATBOTTOM, OnUpdateNewsubtaskAtBottom)
	ON_UPDATE_COMMAND_UI(ID_NEWSUBTASK_ATTOP, OnUpdateNewsubtaskAttop)
	ON_UPDATE_COMMAND_UI(ID_NEWTASK, OnUpdateNewtask)
	ON_UPDATE_COMMAND_UI(ID_NEWTASK_AFTERSELECTEDTASK, OnUpdateNewtaskAfterselectedtask)
	ON_UPDATE_COMMAND_UI(ID_NEWTASK_DEPENDENTAFTERSELECTEDTASK, OnUpdateNewDependenttaskAfterselectedtask)
	ON_UPDATE_COMMAND_UI(ID_NEWTASK_ATBOTTOM, OnUpdateNewtaskAtbottom)
	ON_UPDATE_COMMAND_UI(ID_NEWTASK_ATBOTTOMSELECTED, OnUpdateNewtaskAtbottomSelected)
	ON_UPDATE_COMMAND_UI(ID_NEWTASK_ATTOP, OnUpdateNewtaskAttop)
	ON_UPDATE_COMMAND_UI(ID_NEWTASK_ATTOPSELECTED, OnUpdateNewtaskAttopSelected)
	ON_UPDATE_COMMAND_UI(ID_NEWTASK_BEFORESELECTEDTASK, OnUpdateNewtaskBeforeselectedtask)
	ON_UPDATE_COMMAND_UI(ID_NEXTTASK, OnUpdateGotoNexttask)
	ON_UPDATE_COMMAND_UI(ID_NEXTTOPLEVELTASK, OnUpdateNexttopleveltask)
	ON_UPDATE_COMMAND_UI(ID_OPEN_RELOAD, OnUpdateReload)
	ON_UPDATE_COMMAND_UI(ID_PREVTASK, OnUpdateGotoPrevtask)
	ON_UPDATE_COMMAND_UI(ID_PREVTOPLEVELTASK, OnUpdatePrevtopleveltask)
	ON_UPDATE_COMMAND_UI(ID_PRINT, OnUpdatePrint)
	ON_UPDATE_COMMAND_UI(ID_SAVEALL, OnUpdateSaveall)
	ON_UPDATE_COMMAND_UI(ID_SAVEAS, OnUpdateSaveas)
	ON_UPDATE_COMMAND_UI(ID_SAVE_NORMAL, OnUpdateSave)
	ON_UPDATE_COMMAND_UI(ID_SB_SELCOUNT, OnUpdateSBSelectionCount)
	ON_UPDATE_COMMAND_UI(ID_SB_TASKCOUNT, OnUpdateSBTaskCount)
	ON_UPDATE_COMMAND_UI(ID_SENDTASKS, OnUpdateSendTasks)
	ON_UPDATE_COMMAND_UI(ID_SEND_SELTASKS, OnUpdateSendSelectedTasks)
	ON_UPDATE_COMMAND_UI(ID_SHOWTIMELOGFILE, OnUpdateShowTimelogfile)
	ON_UPDATE_COMMAND_UI(ID_SORT, OnUpdateSort)
	ON_UPDATE_COMMAND_UI(ID_TASKLIST_CUSTOMCOLUMNS, OnUpdateTasklistCustomcolumns)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ANALYSELOGGEDTIME, OnUpdateToolsAnalyseLoggedTime)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_CHECKIN, OnUpdateToolsCheckin)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_CHECKOUT, OnUpdateToolsCheckout)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_EXPORT, OnUpdateExport)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_IMPORT, OnUpdateImport)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_SPELLCHECKTASKLIST, OnUpdateSpellcheckTasklist)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_TOGGLECHECKIN, OnUpdateToolsToggleCheckin)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_TRANSFORM, OnUpdateExport) // use same text as export
	ON_UPDATE_COMMAND_UI(ID_VIEW_CLEARFILTER, OnUpdateViewClearfilter)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLLAPSEALL, OnUpdateViewCollapseall)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLLAPSEDUE, OnUpdateViewCollapseDuetasks)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLLAPSESTARTED, OnUpdateViewCollapseStartedtasks)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLLAPSETASK, OnUpdateViewCollapsetask)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CYCLETASKVIEWS, OnUpdateViewCycleTaskViews)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EXPANDALL, OnUpdateViewExpandall)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EXPANDDUE, OnUpdateViewExpandDuetasks)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EXPANDSTARTED, OnUpdateViewExpandStartedtasks)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EXPANDTASK, OnUpdateViewExpandtask)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FILTER, OnUpdateViewFilter)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOVETASKLISTLEFT, OnUpdateViewMovetasklistleft)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOVETASKLISTRIGHT, OnUpdateViewMovetasklistright)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NEXT, OnUpdateViewNext)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NEXT_SEL, OnUpdateViewNextSel)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PREV, OnUpdateViewPrev)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PREV_SEL, OnUpdateViewPrevSel)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROJECTNAME, OnUpdateViewProjectname)
	ON_UPDATE_COMMAND_UI(ID_VIEW_REFRESHFILTER, OnUpdateViewRefreshfilter)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWFILTERBAR, OnUpdateViewShowfilterbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWTASKLISTTABBAR, OnUpdateViewShowTasklistTabbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWTREELISTTABBAR, OnUpdateViewShowTreeListTabbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SORTTASKLISTTABS, OnUpdateViewSorttasklisttabs)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS_BAR, OnUpdateViewStatusBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOGGLEFILTER, OnUpdateViewTogglefilter)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOGGLETASKEXPANDED, OnUpdateViewToggletaskexpanded)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOGGLETASKSANDCOMMENTS, OnUpdateViewToggletasksandcomments)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOGGLETREEANDLIST, OnUpdateViewToggleTreeandList)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, OnUpdateViewToolbar)
	ON_UPDATE_COMMAND_UI(ID_WINDOW1, OnUpdateWindow)
	ON_UPDATE_COMMAND_UI_RANGE(ID_EDIT_SETPRIORITYNONE, ID_EDIT_SETPRIORITY10, OnUpdateSetPriority)	
	ON_UPDATE_COMMAND_UI_RANGE(ID_NEWTASK_SPLITTASKINTO_TWO, ID_NEWTASK_SPLITTASKINTO_FIVE, OnUpdateSplitTaskIntoPieces)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SHOWVIEW_TASKTREE, ID_SHOWVIEW_UIEXTENSION16, OnUpdateShowTaskView)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORT_BYFIRST, ID_SORT_BYLAST, OnUpdateSortBy)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TOOLS_USERTOOL1, ID_TOOLS_USERTOOL16, OnUpdateUserTool)
	ON_WM_ACTIVATEAPP()
	ON_WM_CONTEXTMENU()
	ON_WM_COPYDATA()
	ON_WM_CREATE()
	ON_WM_DRAWITEM()
	ON_WM_ENABLE()
	ON_WM_ENDSESSION()
	ON_WM_ERASEBKGND()
	ON_WM_ERASEBKGND()
	ON_WM_HELPINFO()
	ON_WM_INITMENUPOPUP()
	ON_WM_INITMENUPOPUP()
	ON_WM_MEASUREITEM()
	ON_WM_MOVE()
	ON_WM_QUERYENDSESSION()
	ON_WM_QUERYOPEN()
	ON_WM_SIZE()
	ON_WM_SETTINGCHANGE()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SYSCOMMAND()
	ON_WM_TIMER()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_WINDOWPOSCHANGING()

	ON_COMMAND(ID_TOOLS_ADDTOSOURCECONTROL, OnToolsAddtoSourceControl)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ADDTOSOURCECONTROL, OnUpdateToolsAddtoSourceControl)
 	ON_COMMAND(ID_TOOLS_REMOVEFROMSOURCECONTROL, OnToolsRemovefromsourcecontrol)
 	ON_UPDATE_COMMAND_UI(ID_TOOLS_REMOVEFROMSOURCECONTROL, OnUpdateToolsRemovefromsourcecontrol)
	
#ifdef _DEBUG
	ON_COMMAND(ID_DEBUGENDSESSION, OnDebugEndSession)
	ON_COMMAND(ID_DEBUGSHOWSETUPDLG, OnDebugShowSetupDlg)
#endif

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToDoListWnd message handlers

void CToDoListWnd::SetupUIStrings()
{
	// set up UI strings for helper classes
	CTimeEdit::SetUnits(THU_MINS,		CEnString(IDS_TE_MINS),		CEnString(IDS_MINS_ABBREV));
	CTimeEdit::SetUnits(THU_HOURS,		CEnString(IDS_TE_HOURS),	CEnString(IDS_HOURS_ABBREV));
	CTimeEdit::SetUnits(THU_WEEKDAYS,	CEnString(IDS_TE_WEEKDAYS), CEnString(IDS_WEEKDAYS_ABBREV));
	CTimeEdit::SetUnits(THU_DAYS,		CEnString(IDS_TE_DAYS),		CEnString(IDS_DAYS_ABBREV));
	CTimeEdit::SetUnits(THU_WEEKS,		CEnString(IDS_TE_WEEKS),	CEnString(IDS_WEEKS_ABBREV));
	CTimeEdit::SetUnits(THU_MONTHS,		CEnString(IDS_TE_MONTHS),	CEnString(IDS_MONTHS_ABBREV));
	CTimeEdit::SetUnits(THU_YEARS,		CEnString(IDS_TE_YEARS),	CEnString(IDS_YEARS_ABBREV));

	CTimeEdit::SetDefaultButtonTip(CEnString(IDS_TIMEUNITS));

	CFileEdit::SetDefaultButtonTips(CEnString(IDS_BROWSE), CEnString(IDS_VIEW));
	CFileEdit::SetDefaultBrowseTitles(CEnString(IDS_BROWSEFILE_TITLE), CEnString(IDS_BROWSEFOLDER_TITLE));

	// misc
	CTDLRecurringTaskEdit::SetDefaultButtonTip(CEnString(IDS_OPTIONS));
	CXmlFileEx::SetUIStrings(CEnString(IDS_ENCRYPTEDFILE), CEnString(IDS_DECRYPTFAILED));
	CSpellCheckDlg::SetItemText(DLG_SCD_BROWSETITLE, IDS_SCD_BROWSETITLE);
	CWinHelpButton::SetDefaultTooltip(CEnString(IDS_ONLINEHELP));
}

BOOL CToDoListWnd::OnHelpInfo(HELPINFO* /*pHelpInfo*/)
{
	// always eat this because we handle the F1 ourselves
	return FALSE;
}

void CToDoListWnd::SetUITheme(const CString& sThemeFile)
{
	// XP and above only
	if (COSVersion() < OSV_XP)
		return;

	// cache existing theme
	CUIThemeFile themeCur = m_theme;

	if (CThemed::IsAppThemed() && m_theme.LoadThemeFile(sThemeFile)) 
	{
		m_sThemeFile = sThemeFile;
	}
	else
	{
		m_sThemeFile.Empty();
		m_theme.Reset();
	}
	
	// update the UI
	if (themeCur != m_theme)
	{
		m_cbQuickFind.DestroyWindow();
		m_tbHelper.Release();
		m_toolbar.DestroyWindow();

		InitToolbar();

		// reinitialize the menu icon manager
		m_mgrMenuIcons.Release();
		InitMenuIconManager();
	}
	else
	{
		m_toolbar.SetBackgroundColors(m_theme.crToolbarLight, 
										m_theme.crToolbarDark, 
										m_theme.HasGradient(), 
										m_theme.HasGlass());
		m_toolbar.SetHotColor(m_theme.crToolbarHot);
	}

	m_statusBar.SetUIColors(m_theme.crStatusBarLight, 
							m_theme.crStatusBarDark, 
							m_theme.crStatusBarText, 
							m_theme.HasGradient(), 
							m_theme.HasGlass());



	m_filterBar.SetUITheme(m_theme);
	m_dlgTimeTracker.SetUITheme(m_theme);
	m_tabCtrl.SetBackgroundColor(m_theme.crAppBackDark);

	if (m_pPrefs)
		m_pPrefs->SetUITheme(m_theme);

	for (int nCtl = 0; nCtl < GetTDCCount(); nCtl++)
	{
		CFilteredToDoCtrl& tdc = GetToDoCtrl(nCtl);
		tdc.SetUITheme(m_theme);
	}

	if (m_findDlg.GetSafeHwnd())
		m_findDlg.SetUITheme(m_theme);

	UpdateMenuBackgroundColor();
	DrawMenuBar();
	Invalidate();
}

BOOL CToDoListWnd::Create(const CTDCStartupOptions& startup)
{
	m_startupOptions = startup;

	m_bVisible = startup.HasFlag(TLD_FORCEVISIBLE) ? 1 : -1;
	m_bSaveUIVisInTaskList = startup.HasFlag(TLD_SAVEUIVISINTASKLIST);

#ifdef _DEBUG
	m_bPasswordPrompting = FALSE;
#else
	m_bPasswordPrompting = startup.HasFlag(TLD_PASSWORDPROMPTING);
#endif

	if (startup.HasFlag(TLD_LOGGING) && !FileMisc::IsLoggingEnabled())
		FileMisc::EnableLogging(TRUE, _T("Abstractspoon"));
	
	return CFrameWnd::LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, NULL, NULL);
}

int CToDoListWnd::GetVersion()
{
	return TD_VERSION;
}

int CToDoListWnd::MessageBox(UINT nIDText, UINT nIDCaption, UINT nType, LPCTSTR szData)
{
	if (!Misc::IsEmpty(szData))
		return MessageBox(CEnString(nIDText, szData), nIDCaption, nType);
	else
		return MessageBox(CEnString(nIDText), nIDCaption, nType);
}

int CToDoListWnd::MessageBox(const CString& sText, UINT nIDCaption, UINT nType)
{
	return MessageBox(sText, CEnString(nIDCaption), nType);
}

int CToDoListWnd::MessageBox(const CString& sText, const CString& sCaption, UINT nType)
{
	CString sMessage;

	if (!sCaption.IsEmpty())
		sMessage.Format(_T("%s|%s"), sCaption, sText);
	else
		sMessage = sText;

	return AfxMessageBox(sMessage, nType);
}

int CToDoListWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOSIZE | SWP_NOZORDER | SWP_NOMOVE);

	// Various key UI elements
	if (!LoadMenubar())
		return -1;

	if (!m_dropTarget.Register(this, this))
		return -1;

	if (!InitTrayIcon())
		return -1;
	
	if (!InitToolbar())
		return -1;

	if (!InitStatusbar())
		return -1;

	if (!InitFilterbar())
		return -1;
	
	if (!InitTabCtrl())
		return -1;
	
	if (!InitTimeTrackDlg())
		return -1;

	UpdateWindowIcons();
	InitUIFont();
	LoadSettings();

	// Session notifications
	m_sessionWnd.Initialize(*this);

	// add a barebones tasklist while we're still hidden
	if (!CreateNewTaskList(FALSE))
		return -1;

	// notify users about dodgy content plugins
	if (m_mgrContent.SomePluginsHaveBadversions())
	{
		if (MessageBox(IDS_BADPLUGINVERSIONS, IDS_BADPLUGINTITLE, MB_OKCANCEL) == IDCANCEL)
			return -1;
	}

	// inherited parent task attributes for new tasks
	CTDCAttributeMap mapParentAttrib;
	BOOL bUpdateAttrib;

	Prefs().GetParentAttribsUsed(mapParentAttrib, bUpdateAttrib);
	CFilteredToDoCtrl::SetInheritedParentAttributes(mapParentAttrib, bUpdateAttrib);

	// theme
	SetUITheme(Prefs().GetUITheme());
				
	// late initialization
	PostMessage(WM_POSTONCREATE);
	
	return 0; // success
}

BOOL CToDoListWnd::InitTabCtrl()
{
	if (m_tabCtrl.Create(WS_CHILD | WS_VISIBLE | TCS_HOTTRACK | TCS_TABS | TCS_SINGLELINE | TCS_RIGHTJUSTIFY | TCS_TOOLTIPS, 
		CRect(0, 0, 10, 10), this, IDC_TABCONTROL))
	{
		m_tabCtrl.GetToolTips()->ModifyStyle(0, TTS_ALWAYSTIP);
		CLocalizer::EnableTranslation(m_tabCtrl, FALSE);
		
		BOOL bStackTabbar = Prefs().GetStackTabbarItems();
		
		m_tabCtrl.ModifyStyle(bStackTabbar ? 0 : TCS_MULTILINE, bStackTabbar ? TCS_MULTILINE : 0);
		
		if (m_ilTabCtrl.Create(16, 16, ILC_COLOR32 | ILC_MASK, 4, 1))
		{
			CBitmap bm;
			bm.LoadBitmap(IDB_SOURCECONTROL_STD);
			
			m_ilTabCtrl.Add(&bm, RGB(255, 0, 255));
			m_tabCtrl.SetImageList(&m_ilTabCtrl);
			
			return TRUE;
		}
	}
	
	// else
	return FALSE;
}

void CToDoListWnd::InitUIFont()
{
	GraphicsMisc::VerifyDeleteObject(m_fontMain);

	HFONT hFontUI = GraphicsMisc::CreateFont(_T("Tahoma"), 8);

	if (m_fontMain.Attach(hFontUI))
		CDialogHelper::SetFont(this, m_fontMain); // will update all child controls
}

void CToDoListWnd::InitShortcutManager()
{
	// setup defaults first
	m_mgrShortcuts.AddShortcut(ID_CLOSE, VK_F4, HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_DELETETASK, VK_DELETE, HOTKEYF_EXT);
	m_mgrShortcuts.AddShortcut(ID_EDIT_COPY, 'C', HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_EDIT_CUT, 'X', HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_EDIT_DECTASKPERCENTDONE, VK_SUBTRACT, HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_EDIT_FINDTASKS, 'F', HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_EDIT_INCTASKPERCENTDONE, VK_ADD, HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_EDIT_INSERTDATETIME, 'D', HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_EDIT_OPENFILEREF1, 'G', HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_EDIT_PASTEAFTER, 'V', HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_EDIT_PASTESUB, 'V', HOTKEYF_CONTROL | HOTKEYF_SHIFT);
	m_mgrShortcuts.AddShortcut(ID_EDIT_QUICKFIND, 'Q', HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_EDIT_QUICKFINDNEXT, VK_F3, 0);
	m_mgrShortcuts.AddShortcut(ID_EDIT_QUICKFINDPREV, VK_F3, HOTKEYF_SHIFT);
	m_mgrShortcuts.AddShortcut(ID_EDIT_REDO, 'Y', HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_EDIT_SELECTALL, 'A', HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_EDIT_TASKDONE, VK_SPACE, HOTKEYF_CONTROL | HOTKEYF_SHIFT);
	m_mgrShortcuts.AddShortcut(ID_EDIT_TASKTEXT, VK_F2, 0);
	m_mgrShortcuts.AddShortcut(ID_EDIT_UNDO, 'Z', HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_EXIT, VK_F4, HOTKEYF_ALT);
	m_mgrShortcuts.AddShortcut(ID_HELP, VK_F1, 0);
	m_mgrShortcuts.AddShortcut(ID_LOAD_NORMAL, 'O', HOTKEYF_CONTROL); 
	m_mgrShortcuts.AddShortcut(ID_MAXCOMMENTS, 'M', HOTKEYF_CONTROL | HOTKEYF_SHIFT);
	m_mgrShortcuts.AddShortcut(ID_MAXTASKLIST, 'M', HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_MOVETASKDOWN, VK_DOWN, HOTKEYF_CONTROL | HOTKEYF_EXT);
	m_mgrShortcuts.AddShortcut(ID_MOVETASKLEFT, VK_LEFT, HOTKEYF_CONTROL | HOTKEYF_EXT);
	m_mgrShortcuts.AddShortcut(ID_MOVETASKRIGHT, VK_RIGHT, HOTKEYF_CONTROL | HOTKEYF_EXT);
	m_mgrShortcuts.AddShortcut(ID_MOVETASKUP, VK_UP, HOTKEYF_CONTROL | HOTKEYF_EXT);
	m_mgrShortcuts.AddShortcut(ID_NEWSUBTASK_ATBOTTOM, 'N', HOTKEYF_CONTROL | HOTKEYF_SHIFT);
	m_mgrShortcuts.AddShortcut(ID_NEWTASK_AFTERSELECTEDTASK, 'N', HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_NEWTASK_BEFORESELECTEDTASK, 'N', HOTKEYF_CONTROL | HOTKEYF_ALT);
	m_mgrShortcuts.AddShortcut(ID_NEXTTASK, VK_F8, 0);
	m_mgrShortcuts.AddShortcut(ID_OPEN_RELOAD, 'R', HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_PREVTASK, VK_F7, 0);
	m_mgrShortcuts.AddShortcut(ID_PRINT, 'P', HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_SAVE_NORMAL, 'S', HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_TOOLS_EXPORT, 'E', HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_TOOLS_IMPORT, 'I', HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_TOOLS_TRANSFORM, 'T', HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_VIEW_CYCLETASKVIEWS, VK_F10, HOTKEYF_SHIFT);
	m_mgrShortcuts.AddShortcut(ID_VIEW_NEXT, VK_TAB, HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_VIEW_NEXT_SEL, VK_RIGHT, HOTKEYF_ALT | HOTKEYF_EXT);
	m_mgrShortcuts.AddShortcut(ID_VIEW_PREV, VK_TAB, HOTKEYF_CONTROL | HOTKEYF_SHIFT);
	m_mgrShortcuts.AddShortcut(ID_VIEW_PREV_SEL, VK_LEFT, HOTKEYF_ALT | HOTKEYF_EXT);
	m_mgrShortcuts.AddShortcut(ID_VIEW_REFRESHFILTER, VK_F5, HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_VIEW_RESIZECOLSTOFIT, VK_ADD, HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_VIEW_TOGGLEFILTER, VK_F12, 0);
	m_mgrShortcuts.AddShortcut(ID_VIEW_TOGGLETASKEXPANDED, VK_SPACE, HOTKEYF_CONTROL | HOTKEYF_ALT);
	m_mgrShortcuts.AddShortcut(ID_VIEW_TOGGLETASKSANDCOMMENTS, VK_F11, 0);
	m_mgrShortcuts.AddShortcut(ID_VIEW_TOGGLETREEANDLIST, VK_F10, 0);
	m_mgrShortcuts.AddShortcut(ID_WINDOW1, '1', HOTKEYF_ALT);
	m_mgrShortcuts.AddShortcut(ID_WINDOW2, '2', HOTKEYF_ALT);
	m_mgrShortcuts.AddShortcut(ID_WINDOW3, '3', HOTKEYF_ALT);
	m_mgrShortcuts.AddShortcut(ID_WINDOW4, '4', HOTKEYF_ALT);
	m_mgrShortcuts.AddShortcut(ID_WINDOW5, '5', HOTKEYF_ALT);
	m_mgrShortcuts.AddShortcut(ID_WINDOW6, '6', HOTKEYF_ALT);
	m_mgrShortcuts.AddShortcut(ID_WINDOW7, '7', HOTKEYF_ALT);
	m_mgrShortcuts.AddShortcut(ID_WINDOW8, '8', HOTKEYF_ALT);
	m_mgrShortcuts.AddShortcut(ID_WINDOW9, '9', HOTKEYF_ALT);

	
	// now init shortcut mgr which will override the defaults
	// with the users actual settings
	CPreferences prefs;

	if (m_mgrShortcuts.Initialize(this, prefs, _T("KeyboardShortcuts")))
	{
		// fix for previously adding escape key as a shortcut for IDCLOSE 
		// (big mistake)
		if (m_mgrShortcuts.GetShortcut(IDCLOSE) == VK_ESCAPE)
			m_mgrShortcuts.DeleteShortcut(IDCLOSE);

		// fix for paste being wrongly set up
		if (m_mgrShortcuts.GetShortcut(ID_EDIT_PASTE))
		{
			// delete existing
			m_mgrShortcuts.DeleteShortcut(ID_EDIT_PASTE);

			// if nothing already assigned use Ctrl+V
			if (!m_mgrShortcuts.GetShortcut(ID_EDIT_PASTESUB))
				m_mgrShortcuts.AddShortcut(ID_EDIT_PASTESUB, 'V', HOTKEYF_CONTROL);
		}
	}
}

void CToDoListWnd::InitMenuIconManager()
{
	if (!m_mgrMenuIcons.Initialize(this))
		return;
	
	m_mgrMenuIcons.ClearImages();

	// Toolbar images
	UINT nToolbarImageID = IDB_APP_TOOLBAR_STD;
	CUIntArray aCmdIDs;
	
	// toolbar
	aCmdIDs.Add(ID_LOAD_NORMAL);
	aCmdIDs.Add(ID_SAVE_NORMAL);
	aCmdIDs.Add(ID_SAVEALL);
	
	// new tasks
	aCmdIDs.Add(GetNewTaskCmdID());
	aCmdIDs.Add(GetNewSubtaskCmdID());
	
	aCmdIDs.Add(ID_EDIT_TASKTEXT);
	aCmdIDs.Add(ID_EDIT_SETTASKICON);
	aCmdIDs.Add(ID_EDIT_SETREMINDER);
	aCmdIDs.Add(ID_EDIT_UNDO);
	aCmdIDs.Add(ID_EDIT_REDO);
	aCmdIDs.Add(ID_MAXTASKLIST);
	aCmdIDs.Add(ID_VIEW_EXPANDTASK);
	aCmdIDs.Add(ID_VIEW_COLLAPSETASK);
	aCmdIDs.Add(ID_VIEW_PREV_SEL);
	aCmdIDs.Add(ID_VIEW_NEXT_SEL);
	aCmdIDs.Add(ID_EDIT_FINDTASKS);
	aCmdIDs.Add(ID_SORT);
	aCmdIDs.Add(ID_DELETETASK);
	aCmdIDs.Add(ID_TOOLS_TOGGLECHECKIN); // placeholder
	aCmdIDs.Add(ID_PREFERENCES);
	aCmdIDs.Add(ID_HELP_WIKI);

	if (m_theme.HasToolbarImageFile(_T("TODOLIST")))
	{
		COLORREF crMask = CLR_NONE;
		CString sImagePath = m_theme.GetToolbarImageFile(_T("TODOLIST"), crMask);

		VERIFY(m_mgrMenuIcons.AddImages(aCmdIDs, sImagePath, crMask));
	}
	else
		m_mgrMenuIcons.AddImages(aCmdIDs, nToolbarImageID, RGB(255, 0, 255));

	// extra
	aCmdIDs.RemoveAll();

  	aCmdIDs.Add(ID_HELP_DONATE);
	aCmdIDs.Add(ID_TOOLS_SELECTINEXPLORER);
	aCmdIDs.Add(ID_TOOLS_ADDTOSOURCECONTROL);
	aCmdIDs.Add(ID_TOOLS_REMOVEFROMSOURCECONTROL);

	if (m_theme.HasToolbarImageFile(_T("TODOLIST_EXTRA")))
	{
		COLORREF crMask = CLR_NONE;
		CString sImagePath = m_theme.GetToolbarImageFile(_T("TODOLIST_EXTRA"), crMask);

		VERIFY(m_mgrMenuIcons.AddImages(aCmdIDs, sImagePath, crMask));
	}
	else
	{
		m_mgrMenuIcons.AddImages(aCmdIDs, IDB_APP_EXTRA_STD, RGB(255, 0, 255));
	}

	// social images
	aCmdIDs.RemoveAll();
	
	aCmdIDs.Add(ID_HELP_WIKI);
	aCmdIDs.Add(ID_HELP_GOOGLEGROUP);
	aCmdIDs.Add(ID_HELP_FACEBOOK);
	aCmdIDs.Add(ID_HELP_TWITTER);
	aCmdIDs.Add(ID_HELP_GOOGLEPLUS);
	aCmdIDs.Add(ID_HELP_LINKEDIN);

	m_mgrMenuIcons.AddImages(aCmdIDs, IDB_SOCIAL_TOOLBAR, RGB(255, 0, 255));
}

void CToDoListWnd::OnShowKeyboardshortcuts() 
{
	CStringArray aMapping;

	if (m_mgrShortcuts.BuildMapping(IDR_MAINFRAME, aMapping, '|'))
	{
		// add a few misc items that don't appear in the menus
		CString sMisc;

		for (int nItem = 0; nItem < NUM_MISCSHORTCUTS; nItem++)
		{
			if (MISC_SHORTCUTS[nItem].dwShortcut)
			{
				const SHORTCUT& sc = MISC_SHORTCUTS[nItem];
				CString sShortcut(m_mgrShortcuts.GetShortcutText(sc.dwShortcut));

				sMisc.Format(_T("%s|%s"), sShortcut, CEnString(sc.nIDShortcut));
			}
			else
			{
				sMisc.Empty();
			}

			aMapping.Add(sMisc);
		}
	
		CTDLKeyboardShortcutDisplayDlg(aMapping, '|').DoModal();
	}
}

LRESULT CToDoListWnd::OnFocusChange(WPARAM wp, LPARAM /*lp*/)
{
	if (m_statusBar.GetSafeHwnd() && IsWindowEnabled() && GetTDCCount() && wp)
	{
		m_sCurrentFocus.Empty();

		// grab the previous window in the z-order and if its
		// static text then use that as the focus hint
		CWnd* pFocus = CWnd::FromHandle((HWND)wp);
		const CFilteredToDoCtrl& tdc = GetToDoCtrl();

		if (CDialogHelper::IsChildOrSame(tdc.GetSafeHwnd(), (HWND)wp))
		{
			m_sCurrentFocus.LoadString(IDS_FOCUS_TASKS);
			m_sCurrentFocus += ": ";
			m_sCurrentFocus += tdc.GetControlDescription(pFocus);
		}
		else if (pFocus == m_cbQuickFind.GetWindow(GW_CHILD))
		{
			m_sCurrentFocus.LoadString(IDS_QUICKFIND);
		}
		else
		{
			if (m_findDlg.GetSafeHwnd() && m_findDlg.IsChild(pFocus))
			{
				m_sCurrentFocus.LoadString(IDS_FINDTASKS);
			}
			else if (m_filterBar.GetSafeHwnd() && m_filterBar.IsChild(pFocus))
			{
				m_sCurrentFocus.LoadString(IDS_FOCUS_FILTERBAR);
			}
			
			if (!m_sCurrentFocus.IsEmpty())
				m_sCurrentFocus += ": ";
			
			m_sCurrentFocus += GetCtrlLabel(pFocus);
		}		

		// limit length of string
		if (m_sCurrentFocus.GetLength() > 22)
			m_sCurrentFocus = m_sCurrentFocus.Left(20) + _T("...");

		m_statusBar.SetPaneText(m_statusBar.CommandToIndex(ID_SB_FOCUS), m_sCurrentFocus);
		
		// if the status bar is hidden then add text to title bar
		if (!m_bShowStatusBar)
			UpdateCaption();
	}

	return 0L;
}

LRESULT CToDoListWnd::OnGetIcon(WPARAM bLargeIcon, LPARAM /*not used*/)
{
	return (LRESULT)(bLargeIcon ? m_icons.GetBigIcon() : m_icons.GetSmallIcon());
}

BOOL CToDoListWnd::InitStatusbar()
{
	static SBACTPANEINFO SB_PANES[] = 
	{
	  { ID_SB_FILEPATH,		MAKEINTRESOURCE(IDS_SB_FILEPATH_TIP), SBACTF_STRETCHY | SBACTF_RESOURCETIP }, 
	  { ID_SB_FILEVERSION,	MAKEINTRESOURCE(IDS_SB_FILEVERSION_TIP), SBACTF_AUTOFIT | SBACTF_RESOURCETIP }, 
	  { ID_SB_TASKCOUNT,	MAKEINTRESOURCE(IDS_SB_TASKCOUNT_TIP), SBACTF_AUTOFIT | SBACTF_RESOURCETIP }, 
	  //{ ID_SB_SPACER }, 
	  { ID_SB_SELCOUNT,		_T(""), SBACTF_AUTOFIT }, 
	  { ID_SB_SELTIMEEST,	MAKEINTRESOURCE(IDS_SB_SELTIMEEST_TIP), SBACTF_AUTOFIT | SBACTF_RESOURCETIP }, 
	  { ID_SB_SELTIMESPENT,	MAKEINTRESOURCE(IDS_SB_SELTIMESPENT_TIP), SBACTF_AUTOFIT | SBACTF_RESOURCETIP }, 
	  { ID_SB_SELCOST,		MAKEINTRESOURCE(IDS_SB_SELCOST_TIP), SBACTF_AUTOFIT | SBACTF_RESOURCETIP }, 
	  //{ ID_SB_SPACER }, 
	  { ID_SB_FOCUS,		MAKEINTRESOURCE(IDS_SB_FOCUS_TIP), SBACTF_AUTOFIT | SBACTF_RESOURCETIP }, 
	};

	static int SB_PANECOUNT = sizeof(SB_PANES) / sizeof(SBACTPANEINFO);

	if (!m_statusBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM, IDC_FILENAME))
		return FALSE;

	// prevent translation because we handle it manually
	CLocalizer::EnableTranslation(m_statusBar, FALSE);

	if (!m_statusBar.SetPanes(SB_PANES, SB_PANECOUNT))
		return FALSE;

	return TRUE;
}

BOOL CToDoListWnd::InitFilterbar()
{
	if (!m_filterBar.Create(this))
		return FALSE;

	m_filterBar.EnableMultiSelection(Prefs().GetMultiSelFilters());
	m_filterBar.ShowDefaultFilters(Prefs().GetShowDefaultFilters());

	RefreshFilterBarCustomFilters();

	return TRUE;
}

BOOL CToDoListWnd::InitTimeTrackDlg()
{
	// First time only
	if (m_dlgTimeTracker.GetSafeHwnd())
	{
		return TRUE;
	}
	else if (!m_dlgTimeTracker.Create())
	{
		return FALSE;
	}

	UpdateTimeTrackerPreferences();

	return TRUE;
}

void CToDoListWnd::UpdateTimeTrackerTasks(const CFilteredToDoCtrl& tdc, BOOL bAllTasks)
{
	CTaskFile tasks;

	if (bAllTasks)
		tdc.GetFilteredTasks(tasks);
	else
		tdc.GetSelectedTasks(tasks, TDCGT_ALL, TDCGSTF_ALLPARENTS);
	
	m_dlgTimeTracker.UpdateTasks(&tdc, tasks);
}

void CToDoListWnd::UpdateTimeTrackerPreferences()
{
	const CPreferencesDlg& prefs = Prefs();

	m_dlgTimeTracker.SetOption(TTDO_ALLOWPARENTTRACKING, prefs.GetAllowParentTimeTracking());
	m_dlgTimeTracker.SetOption(TTDO_FORMATTIMESASHMS, prefs.GetUseHMSTimeFormat());
	m_dlgTimeTracker.SetOption(TTDO_SHOWONBEGINTRACKING, prefs.GetShowTimeTracker());
	m_dlgTimeTracker.SetOption(TTDO_SHOWTASKPATH, TRUE/*prefs.GetShowFullTaskPathInTimeTracker()*/);
}

BOOL CToDoListWnd::InitTrayIcon()
{
	// we always create the trayicon (for simplicity) but we only
	// show it if required
	if (!m_trayIcon.Create(this, IDC_TRAYICON, IDR_MAINFRAME_STD, CEnString(IDS_COPYRIGHT)))
		return FALSE;

	if (Prefs().GetUseSysTray())
		m_trayIcon.ShowTrayIcon();

	// show the upgrade message as required
	if (m_startupOptions.HasFlag(TLD_UPGRADED))
		m_trayIcon.ShowBalloon(CEnString(IDS_SUCCESSFUL_UPGRADE), GetTitle(), NIIF_INFO, 10);

	return TRUE;
}

BOOL CToDoListWnd::InitToolbar()
{
	if (m_toolbar.GetSafeHwnd())
		return TRUE;

	UINT nStyle = (WS_CHILD | CBRS_ALIGN_TOP | WS_CLIPCHILDREN | CBRS_TOOLTIPS);

	if (m_bShowToolbar)
		nStyle |= WS_VISIBLE;

	if (!m_toolbar.CreateEx(this, (TBSTYLE_FLAT | TBSTYLE_WRAPABLE), nStyle))
		return FALSE;

	if (!m_toolbar.LoadToolBar(IDR_APP_TOOLBAR))
		return FALSE;
	
	m_toolbar.SetBorders(4, 2, 0, 0);

	// colors
	if (CThemed::IsAppThemed())
	{
		m_toolbar.SetBackgroundColors(m_theme.crToolbarLight, 
										m_theme.crToolbarDark, 
										m_theme.HasGradient(), 
										m_theme.HasGlass());
		m_toolbar.SetHotColor(m_theme.crToolbarHot);
	}
	
	// toolbar images
	if (m_theme.HasToolbarImageFile(_T("TODOLIST")))
	{
		COLORREF crMask = CLR_NONE;
		CString sImagePath = m_theme.GetToolbarImageFile(_T("TODOLIST"), crMask);

		VERIFY(m_toolbar.SetImage(sImagePath, crMask));
	}
	else 
	{
		const COLORREF MAGENTA = RGB(255, 0, 255);
		m_toolbar.SetImage(IDB_APP_TOOLBAR_STD, MAGENTA);
	}
	
	// resize the toolbar in one row so that our subsequent calculations work
	m_toolbar.MoveWindow(0, 2, 1000, 32); 
	
	// insert combobox for quick Find after Find Tasks button
	int nPos = m_toolbar.CommandToIndex(ID_EDIT_FINDTASKS) + 1;
	
	TBBUTTON tbbQuickFind = { 0, nPos, 0, TBSTYLE_SEP, 0, NULL };
	TBBUTTON tbbSep = { 0, nPos + 1, 0, TBSTYLE_SEP, 0, NULL };
	
	m_toolbar.GetToolBarCtrl().InsertButton(nPos, &tbbQuickFind);
	m_toolbar.GetToolBarCtrl().InsertButton(nPos + 1, &tbbSep);
	
	TBBUTTONINFO tbi;
	tbi.cbSize = sizeof( TBBUTTONINFO );
	tbi.cx = 150;
	tbi.dwMask = TBIF_SIZE;  // By index
	
	m_toolbar.GetToolBarCtrl().SetButtonInfo(nPos + 1, &tbi);
	
	CRect rect;
	m_toolbar.GetToolBarCtrl().GetItemRect(nPos + 1, &rect);
	rect.bottom += 200;
	
	if (!m_cbQuickFind.Create(WS_CHILD | WS_VSCROLL | WS_VISIBLE | CBS_AUTOHSCROLL | 
		CBS_DROPDOWN, rect, &m_toolbar, IDC_QUICKFIND))
	{
		return FALSE;
	}

	// previous quick find items
	CStringArray aItems;
	
	if (CPreferences().GetProfileArray(_T("QuickFind"), aItems))
		m_cbQuickFind.AddUniqueItems(aItems);

	m_cbQuickFind.SetFont(&m_fontMain);
	m_mgrPrompts.SetComboEditPrompt(m_cbQuickFind, IDS_QUICKFIND);
	
	m_tbHelper.Initialize(&m_toolbar, this, &m_mgrShortcuts);
	
	return TRUE;
}

void CToDoListWnd::OnEditChangeQuickFind()
{
	ProcessQuickFindTextChange(FALSE);
}

void CToDoListWnd::OnSelChangeQuickFind()
{
	// Only process changes
	if (m_sQuickFind != CDialogHelper::GetSelectedItem(m_cbQuickFind))
		ProcessQuickFindTextChange(TRUE);
}

void CToDoListWnd::ProcessQuickFindTextChange(BOOL bComboSelChange)
{
	if (bComboSelChange)
		m_sQuickFind = CDialogHelper::GetSelectedItem(m_cbQuickFind);
	else
		m_cbQuickFind.GetWindowText(m_sQuickFind);

	m_cbQuickFind.ClearEdit(); // we've handled it
	
	// make sure there's something worth looking for
	CStringArray aTemp;
	
	if (!Misc::ParseSearchString(m_sQuickFind, aTemp))
		return;
	
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	if (!tdc.SelectTask(m_sQuickFind, TDC_SELECTNEXTINCLCURRENT))
	{
		if (!tdc.SelectTask(m_sQuickFind, TDC_SELECTFIRST))
		{
			MessageBox(IDS_QUICKFIND_NOTFOUND, 0, MB_OK, m_sQuickFind);
			m_cbQuickFind.SetFocus();

			// Remove the just-unfound item
			m_cbQuickFind.DeleteString(m_sQuickFind);
		}
	}
}

BOOL CToDoListWnd::HandleReservedShortcut(DWORD dwShortcut)
{
	if (CFilteredToDoCtrl::IsReservedShortcut(dwShortcut))
	{
		int nRet = MessageBox(IDS_RESERVEDSHORTCUT_MSG, IDS_RESERVEDSHORTCUT_TITLE, MB_YESNOCANCEL);
		
		if (nRet == IDYES)
			DoPreferences(PREFPAGE_SHORTCUT);
		
		// and keep eating it until the user changes it
		return TRUE;
	}

	return FALSE;
}

BOOL CToDoListWnd::SendShortcutCommand(UINT nCmdID)
{
	BOOL bSendCommand = (nCmdID > 0); // default

	// focus checks
	switch (nCmdID)
	{
	case ID_EDIT_CUT:
	case ID_EDIT_COPY:
		// tree must have the focus
		if (!GetToDoCtrl().TasksHaveFocus())
		{
			bSendCommand = FALSE;
			GetToDoCtrl().ClearCopiedItem();
		}
		break;

		// tree must have the focus
	case ID_EDIT_SELECT_ALL: 
	case ID_EDIT_PASTE: 
	case ID_DELETEALLTASKS:
	case ID_DELETETASK:
		bSendCommand = GetToDoCtrl().TasksHaveFocus();
		break;
	}

	// send off
	if (bSendCommand)
		SendMessage(WM_COMMAND, nCmdID);

	return bSendCommand;
}

BOOL CToDoListWnd::HandleEscapeTabReturn(MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		{
			switch (pMsg->wParam)
			{
			case VK_ESCAPE:
				if (Prefs().GetEscapeMinimizes() && (GetCapture() == NULL))
				{
					// if the window with the target is either a combobox or
					// the child edit of a combobox and the combo is
					// dropped down then let it thru else if the target is
					// a child of ours then treat as a cancel
					BOOL bHandle = TRUE;
					
					if (CWinClasses::IsClass(pMsg->hwnd, WC_COMBOBOX))
					{
						bHandle = !ComboBox_GetDroppedState(pMsg->hwnd);
					}
					else if (CWinClasses::IsClass(::GetParent(pMsg->hwnd), WC_COMBOBOX))
					{
						bHandle = !ComboBox_GetDroppedState(::GetParent(pMsg->hwnd));
					}
					else if (GetTDCCount() && GetToDoCtrl().IsTaskLabelEditing())
					{
						bHandle = FALSE;
					}

					if (bHandle && ::IsChild(*this, pMsg->hwnd))
					{
						OnCancel();
						return TRUE;
					}
				}
				break;
				
			case VK_TAB: // tabbing away from Quick Find -> tasks
				if (::IsChild(m_cbQuickFind, pMsg->hwnd))
				{
					GetToDoCtrl().SetFocusToTasks();
					return TRUE;
				}
				break;
				
			case VK_RETURN: // hitting return in filter bar and quick find
				if (::IsChild(m_cbQuickFind, pMsg->hwnd) && !m_sQuickFind.IsEmpty())
				{
					OnQuickFindNext();
					return TRUE;
				}
				// likewise for filter bar
				else if (::IsChild(m_filterBar, pMsg->hwnd) && CWinClasses::IsEditControl(pMsg->hwnd))
				{
					 // fall thru to CFrameWnd::PreTranslateMessage(pMsg)
				}
				// and the Time Tracker
				else if (::IsChild(m_dlgTimeTracker, pMsg->hwnd) && CWinClasses::IsEditControl(pMsg->hwnd))
				{
					 // fall thru to CFrameWnd::PreTranslateMessage(pMsg)
				}
				else if (Prefs().GetFocusTreeOnEnter())
				{
					if (::IsChild(m_filterBar, pMsg->hwnd) && !CtrlWantsEnter(pMsg->hwnd))
					{
						GetToDoCtrl().SetFocusToTasks();
						return TRUE;
					}
				}
				break;
			}
		}
		break;
	}

	return FALSE;
}

BOOL CToDoListWnd::AppOverridesToDoCtrlProcessing(UINT nCmdID, DWORD dwShortcut) const
{
	if (GetTDCCount() == 0)
		return TRUE;

	// App handles undo globally
	if ((nCmdID == ID_EDIT_UNDO) || (nCmdID == ID_EDIT_REDO))
		return TRUE;

	// This is a workaround for the fact that 'Ctrl+Num+' is the default 
	// shortcut for ID_EDIT_INCTASKPERCENTDONE and this same shortcut
	// is handled internally by list-views to resize all columns.
	// Since the active ToDoCtrl gets first-dibs on the message this
	// overrides the expected behaviour is therefore considered to be a bug.
	if (dwShortcut == MAKELONG(VK_ADD, HOTKEYF_CONTROL))
		return TRUE;

	// all else is okay
	return FALSE;
}

BOOL CToDoListWnd::ProcessShortcut(MSG* pMsg)
{
	DWORD dwShortcut = 0;
	UINT nCmdID = m_mgrShortcuts.ProcessMessage(pMsg, &dwShortcut);
	
	// if it's a reserved shortcut let's notify the user to change it
	if (HandleReservedShortcut(dwShortcut))
		return TRUE;
	
	// Try active ToDoCtrl unless App overrides depending on context
	if (!AppOverridesToDoCtrlProcessing(nCmdID, dwShortcut))
	{
		if (GetToDoCtrl().PreTranslateMessage(pMsg))
			return TRUE;
	}
	
	// Send the command to ourselves
	if (SendShortcutCommand(nCmdID))
		return TRUE;

	// all else
	return FALSE;
}

BOOL CToDoListWnd::PreTranslateMessage(MSG* pMsg)
{
	// the only way we get a WM_CLOSE here is if it was sent from an external app
	// so we shut down as gracefully as possible
	if ((pMsg->message == WM_CLOSE) && IsWindowEnabled())
	{
		DoExit();
		return TRUE;
	}
	
	// Control accelerators
	if (ProcessDialogCtrlShortcut(pMsg))
		return TRUE;

	if (IsDroppedComboBox(pMsg->hwnd))
		return FALSE;

	// Keyboard shortcuts
	if (ProcessShortcut(pMsg))
		return TRUE;

	if (HandleEscapeTabReturn(pMsg))
		return TRUE;
	
	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CToDoListWnd::OnCancel()
{
	ASSERT (Prefs().GetEscapeMinimizes());

	// if the close button has been configured to Minimize to tray
	// then do that here else normal minimize 
	if (HasSysTrayOptions(STO_ONMINCLOSE, STO_ONCLOSE))
	{
		MinimizeToTray();
	}
	else
	{
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}
}

void CToDoListWnd::OnDeleteTask() 
{
	if (GetToDoCtrl().GetSelectedItem())
		GetToDoCtrl().DeleteSelectedTask();
}

void CToDoListWnd::OnDeleteAllTasks() 
{
	if (GetToDoCtrl().DeleteAllTasks())
		UpdateStatusbar();
}

void CToDoListWnd::OnSave() 
{
	if (SaveTaskList(GetSelToDoCtrl()) == TDCF_SUCCESS)
		UpdateCaption();
}

TDC_FILE CToDoListWnd::DoSaveWithBackupAndProgress(CFilteredToDoCtrl& tdc, int nIndex, CTaskFile& tasks, LPCTSTR szFilePath)
{
	DOPROGRESS(IDS_SAVINGPROGRESS);
		
	// back file up
	m_mgrToDoCtrls.DoBackup(nIndex);
	
	// do the save
	return tdc.Save(tasks, szFilePath);
}

BOOL CToDoListWnd::HandleSaveTasklistError(TDC_FILE& nErr, LPCTSTR szTasklist)
{
	CEnString sMessage;
	
	switch (nErr)
	{
	case TDCF_SUCCESS:
		break; // not an error
		
	case TDCF_NOSPACE:
	case TDCF_NOTALLOWED:
		{
			if (nErr == TDCF_NOTALLOWED)
			{
				if (CDriveInfo::IsReadonlyPath(szTasklist) > 0) // read-only file
				{
					sMessage.Format(IDS_SAVEREADONLY, szTasklist);
				}
				else // other
				{
					sMessage.Format(IDS_SAVEACCESSDENIED, szTasklist);
				}
			}
			else // run out of space
			{
				sMessage.Format(IDS_SAVENOSPACE, szTasklist);
			}
			
			if (IDOK == MessageBox(sMessage, IDS_SAVETASKLIST_TITLE, (MB_OKCANCEL | MB_ICONEXCLAMATION)))
			{
				// try again
				return TRUE;
			}
			else
			{
				nErr = TDCF_CANCELLED;
				sMessage.Empty(); // to prevent another message box
			}
		}
		break;
		
	case TDCF_CANCELLED:
		break;
		
	case TDCF_BADMSXML:
		sMessage.Format(IDS_SAVEBADXML, szTasklist);
		break;
		
	case TDCF_INUSE:
		sMessage.Format(IDS_SAVESHARINGVIOLATION, szTasklist);
		break;
		
	case TDCF_SSC_NOTCHECKEDOUT:
		sMessage.Format(IDS_SAVENOTCHECKEDOUT, szTasklist);
		break;
		
	case TDCF_SSC_NOTSRCCONTROLLED:
	case TDCF_SSC_DELAYLOADED:
		sMessage.Format(IDS_CHECKOUTFAILED, szTasklist);
		break;
		
	default:
		sMessage.Format(IDS_UNKNOWNSAVEERROR2, szTasklist, (nErr - (int)TDCF_OTHER));
		break;
	}
	
	if (!sMessage.IsEmpty())
	{
		FileMisc::LogText(_T("Saving of the file '%s' failed with the error code: %d"), szTasklist, nErr);
		MessageBox(sMessage, IDS_SAVETASKLIST_TITLE, MB_OK);
	}
	
	return FALSE; // not handled
}

TDC_FILE CToDoListWnd::SaveTaskList(int nIndex, LPCTSTR szFilePath, BOOL bAuto)
{
	CAutoFlag af(m_bSaving, TRUE);
	CPreferences prefs;
	const CPreferencesDlg& userPrefs = Prefs();

	// make sure we are up to date
	CFilteredToDoCtrl& tdc = GetToDoCtrl(nIndex);
	tdc.Flush();

	TDC_FILE nResult = TDCF_SUCCESS;
	CTaskFile tasks;

	// we can reduce the amount of work required if saving to storage
	TSM_TASKLISTINFO storageInfo;
	BOOL bUsesStorage = m_mgrToDoCtrls.GetStorageDetails(nIndex, storageInfo);

	if (bUsesStorage)
	{
		DOPROGRESS(IDS_SAVINGPROGRESS);

		// init storage local path to temp path
		storageInfo.SetLocalFilePath(FileMisc::GetTempFilePath());

		// save to file and then to storage
		if (tdc.Save(tasks, storageInfo.szLocalFileName) == TDCF_SUCCESS)
		{
			if (!m_mgrStorage.StoreTasklist(storageInfo, tasks, -1, prefs))
				nResult = TDCF_OTHER;
		}
	}
	else // we're file-based
	{
		CString sFilePath = (szFilePath ? szFilePath : m_mgrToDoCtrls.GetFilePath(nIndex));

		// conditions for saving
		// 1. Save As... ie szFilePath != NULL and not empty
		// 2. tasklist has been modified
		if ((szFilePath && !sFilePath.IsEmpty()) || tdc.IsModified())
		{
			// do this in a loop in case the save fails for _any_ reason
			while (TRUE)
			{
				if (sFilePath.IsEmpty()) // means first time save
				{
					// activate tasklist
					if (!SelectToDoCtrl(nIndex, (nIndex != GetSelToDoCtrl())))
						return TDCF_CANCELLED;

					// use 'friendly' name as user for user
					CFileSaveDialog dialog(IDS_SAVETASKLIST_TITLE, 
											GetDefaultFileExt(), 
											m_mgrToDoCtrls.GetFileName(nIndex, FALSE), 
											EOFN_DEFAULTSAVE, 
											GetFileFilter(), 
											this);
					
					dialog.m_ofn.nFilterIndex = 1; // .tdl

					if (dialog.DoModal(prefs) != IDOK)
						return TDCF_CANCELLED; // user elected not to proceed

					// else make sure the file is not readonly
					sFilePath = dialog.GetPathName();

				} // sFilePath.IsEmpty()

				// update source control status
				tdc.SetStyle(TDCS_CHECKOUTONLOAD, userPrefs.GetAutoCheckOut());

				// do the save
				nResult = DoSaveWithBackupAndProgress(tdc, nIndex, tasks, sFilePath);

				if (nResult != TDCF_SUCCESS)
				{
					// error handling if this is not an auto-save
					if (!bAuto && HandleSaveTasklistError(nResult, sFilePath))
					{
						// try again
						sFilePath.Empty();
						continue;
					}

					// we're done
					return nResult;
				}
				break; // success
			} // while (TRUE)
		}
	} // file-based
	ASSERT (nResult == TDCF_SUCCESS);

	// post-process success
	if (nResult == TDCF_SUCCESS)
	{
		m_mgrToDoCtrls.RefreshFileLastModified(nIndex);
		m_mgrToDoCtrls.RefreshReadOnlyStatus(nIndex);
		m_mgrToDoCtrls.RefreshPathType(nIndex);

		CString sFilePath = tdc.GetFilePath();

		if (userPrefs.GetAddFilesToMRU() && !bUsesStorage)
			m_mruList.Add(sFilePath);

		UpdateCaption();
		UpdateStatusbar();

		// auto-export after saving
		CString sExt;

		if (userPrefs.GetAutoExport() && GetAutoExportExtension(sExt))
		{
			ASSERT(tasks.GetTaskCount());

			DOPROGRESS(IDS_EXPORTPROGRESS);

			// construct output path
			CString sExportPath = userPrefs.GetAutoExportFolderPath();
			CString sDrive, sFolder, sFName;

			FileMisc::SplitPath(sFilePath, &sDrive, &sFolder, &sFName);

			if (!sExportPath.IsEmpty() && FileMisc::CreateFolder(sExportPath))
				FileMisc::MakePath(sFilePath, NULL, sExportPath, sFName, sExt);
			else
				FileMisc::MakePath(sFilePath, sDrive, sFolder, sFName, sExt);

			// The current contents of 'tasks' is 'All Tasks' and 
			// 'All Columns' but NOT 'Html Comments'.
			// So if user either wants 'Filtered Tasks' or 'Html Comments' or
			// only 'Visible Columns' we need to grab the tasks again.
			BOOL bFiltered = (userPrefs.GetExportFilteredOnly() && tdc.HasAnyFilter());
			BOOL bHtmlComments = userPrefs.GetExportToHTML();

			CString sStylesheet(userPrefs.GetSaveExportStylesheet());
			BOOL bTransform = GetStylesheetPath(tdc, sStylesheet);

			if (bFiltered || userPrefs.GetExportToHTML() || !userPrefs.GetExportAllAttributes())
			{
				TSD_TASKS nWhatTasks = bFiltered ? TSDT_FILTERED : TSDT_ALL;
				TDCGETTASKS filter;

				if (!userPrefs.GetExportAllAttributes())
				{
					// visible attributes
					filter.mapAttribs.CopyAttributes(tdc.GetVisibleEditFields());

					// add comments always
					filter.mapAttribs.AddAttribute(TDCA_COMMENTS);
				}

				// set the html image folder to be the output path with
				// an different extension
				CString sImgFolder;

				if (bHtmlComments)
				{
					sImgFolder = sFilePath;
					FileMisc::ReplaceExtension(sImgFolder, _T("html_images"));
				}

				GetTasks(tdc, bHtmlComments, bTransform, nWhatTasks, filter, 0, tasks, sImgFolder); 
			}

			// save intermediate tasklist to file as required
			LogIntermediateTaskList(tasks, tdc.GetFilePath());

			// want HTML 
			if (userPrefs.GetExportToHTML())
			{
				Export2Html(tasks, sFilePath, sStylesheet);
			}
			else if (userPrefs.GetOtherExporter() != -1)
			{
				int nExporter = userPrefs.GetOtherExporter();
				m_mgrImportExport.ExportTaskList(&tasks, sFilePath, nExporter, TRUE);
			}
		}
	}

	return nResult;
}

BOOL CToDoListWnd::GetAutoExportExtension(CString& sExt) const
{
	sExt.Empty();

	if (Prefs().GetExportToHTML())
		sExt = ".html";
	else
	{
		int nExporter = Prefs().GetOtherExporter();

		if (nExporter != -1)
			sExt = m_mgrImportExport.GetExporterFileExtension(nExporter);
	}

	return !sExt.IsEmpty();
}

LPCTSTR CToDoListWnd::GetFileFilter()
{
	static CEnString TDLFILEFILTER(IDS_TDLFILEFILTER);
	static CEnString XMLFILEFILTER(IDS_XMLFILEFILTER);
	
	if (Prefs().GetEnableTDLExtension() ||
		CFileRegister::IsRegisteredApp(_T("tdl"), _T("TODOLIST.EXE"), TRUE))
	{
		return TDLFILEFILTER;
	}
	
	// else
	return XMLFILEFILTER;
}

LPCTSTR CToDoListWnd::GetDefaultFileExt()
{
	static LPCTSTR TDLEXT = _T("tdl");
	static LPCTSTR XMLEXT = _T("xml");
	
	if (Prefs().GetEnableTDLExtension() ||
		CFileRegister::IsRegisteredApp(_T("tdl"), _T("TODOLIST.EXE"), TRUE))
	{
		return TDLEXT;
	}

	// else
	return XMLEXT;
}

void CToDoListWnd::UpdateStatusbar()
{
	if (!m_sbProgress.IsActive() && GetTDCCount())
	{
		// get display path
		int nTasklist = GetSelToDoCtrl();
		const CFilteredToDoCtrl& tdc = GetToDoCtrl(nTasklist);

		CEnString sText = m_mgrToDoCtrls.GetDisplayPath(nTasklist);

		if (sText.IsEmpty())
			sText.LoadString(ID_SB_FILEPATH);
		
		m_statusBar.SetPaneText(m_statusBar.CommandToIndex(ID_SB_FILEPATH), sText);

		// get file version
		sText.Format(ID_SB_FILEVERSION, tdc.GetFileVersion());
		m_statusBar.SetPaneText(m_statusBar.CommandToIndex(ID_SB_FILEVERSION), sText);
	}
}

void CToDoListWnd::OnLoad() 
{
	CPreferences prefs;
	CFileOpenDialog dialog(IDS_OPENTASKLIST_TITLE, 
							GetDefaultFileExt(), 
							NULL, 
							EOFN_DEFAULTOPEN | OFN_ALLOWMULTISELECT,
							GetFileFilter(), 
							this);
	
	const UINT BUFSIZE = 1024 * 5;
	static TCHAR FILEBUF[BUFSIZE] = { 0 };
	
	dialog.m_ofn.lpstrFile = FILEBUF;
	dialog.m_ofn.nMaxFile = BUFSIZE;
	
	if (dialog.DoModal(prefs) == IDOK)
	{
		CWaitCursor cursor;
		POSITION pos = dialog.GetStartPosition();
		
		while (pos)
		{
			CString sTaskList = dialog.GetNextPathName(pos);
			TDC_FILE nOpen = OpenTaskList(sTaskList);
			
			if (nOpen == TDCF_SUCCESS)
			{
				Resize();
				UpdateWindow();
			}
			else
			{
				HandleLoadTasklistError(nOpen, sTaskList);
			}
		}
		
		RefreshTabOrder();
	}
}

void CToDoListWnd::HandleLoadTasklistError(TDC_FILE& nErr, LPCTSTR szTaskList)
{
	CEnString sMessage;
	
	switch (nErr)
	{
	case TDCF_SUCCESS:
		break; // not an error!
		
	case TDCF_CANCELLED:
		break; 
		
	case TDCF_NOTEXIST:
		{
			DWORD dwError = GetLastError();

			if (dwError == ERROR_BAD_NETPATH)
			{
				nErr = TDCF_BADNETWORK;
				sMessage.Format(IDS_BADNETWORK, szTaskList);
			}
			else
			{
				sMessage.Format(IDS_TASKLISTNOTFOUND, szTaskList);
			}
		}
		break;
		
	case TDCF_NOTTASKLIST:
		sMessage.Format(IDS_INVALIDTASKLIST, szTaskList);
		break;
		
	case TDCF_NOTALLOWED:
		sMessage.Format(IDS_OPENACCESSDENIED, szTaskList);
		break;
		
	case TDCF_INUSE:
		sMessage.Format(IDS_OPENSHARINGVIOLATION, szTaskList);
		break;
		
	case TDCF_BADMSXML:
		sMessage.Format(IDS_BADXML, szTaskList);
		break;
		
	case TDCF_NOENCRYPTIONDLL:
		sMessage.Format(IDS_NOENCRYPTIONDLL, szTaskList);
		break;
		
	case TDCF_UNKNOWNENCRYPTION:
		sMessage.Format(IDS_UNKNOWNENCRYPTION, szTaskList);
		break;
		
	default: // all the other errors
		sMessage.Format(IDS_UNKNOWNOPENERROR, szTaskList, nErr - (int)TDCF_OTHER);
		break;
	}
	
	if (!sMessage.IsEmpty())
	{
		FileMisc::LogText(_T("Loading of the file '%s' failed with the error code: %d"), szTaskList, nErr);
		MessageBox(sMessage, CEnString(IDS_OPENTASKLIST_TITLE), MB_OK);
	}
	
}

void CToDoListWnd::SaveSettings()
{
	CPreferences prefs;

	// pos
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	
	prefs.WriteProfileInt(_T("Pos"), _T("Left"), wp.rcNormalPosition.left);
	prefs.WriteProfileInt(_T("Pos"), _T("Top"), wp.rcNormalPosition.top);
	prefs.WriteProfileInt(_T("Pos"), _T("Right"), wp.rcNormalPosition.right);
	prefs.WriteProfileInt(_T("Pos"), _T("Bottom"), wp.rcNormalPosition.bottom);

// 	FileMisc::LogText(_T("SavePosition: TopLeft=(%d,%d) BotRight=(%d,%d) MinPos=(%d,%d) MaxPos=(%d,%d)"), 
// 						wp.rcNormalPosition.left, wp.rcNormalPosition.top,
// 						wp.rcNormalPosition.right, wp.rcNormalPosition.bottom,
// 						wp.ptMaxPosition.x, wp.ptMaxPosition.y,
// 						wp.ptMinPosition.x, wp.ptMinPosition.y);

	prefs.WriteProfileInt(_T("Pos"), _T("Hidden"), !m_bVisible);
	prefs.WriteProfileInt(_T("Pos"), _T("Maximized"), IsZoomed());
	
	// version
	prefs.WriteProfileInt(_T("Version"), _T("Version"), GetVersion());
	
	// last open files
	int nCount = GetTDCCount();
	int nSel = GetSelToDoCtrl(); // and last active file
	
	if (nCount) // but don't overwrite files saved in OnQueryEndSession() or OnClose()
	{
		for (int nTDC = 0, nItem = 0; nTDC < nCount; nTDC++)
		{
			CString sFilePath = m_mgrToDoCtrls.GetFilePath(nTDC);

			TSM_TASKLISTINFO storageInfo;

			if (m_mgrToDoCtrls.GetStorageDetails(nTDC, storageInfo))
			{
				// always clear local path before saving
				storageInfo.ClearLocalFilePath();

				sFilePath = storageInfo.EncodeInfo(Prefs().GetSaveStoragePasswords());

#ifdef _DEBUG
				ASSERT(storageInfo.DecodeInfo(sFilePath));
				ASSERT(storageInfo.EncodeInfo(Prefs().GetSaveStoragePasswords()) == sFilePath);
#endif
			}
			else // make file paths relative
			{
				FileMisc::MakeRelativePath(sFilePath, FileMisc::GetAppFolder(), FALSE);
			}

			CString sKey = Misc::MakeKey(_T("LastFile%d"), nItem);
			prefs.WriteProfileString(SETTINGS_KEY, sKey, sFilePath);
			
			if (nSel == nTDC)
				prefs.WriteProfileString(SETTINGS_KEY, _T("LastActiveFile"), sFilePath);

			nItem++;
		}
		
		prefs.WriteProfileInt(SETTINGS_KEY, _T("NumLastFiles"), nCount);
	}

	// other settings
	prefs.WriteProfileInt(SETTINGS_KEY, _T("ViewState"), m_nMaxState);
	prefs.WriteProfileInt(SETTINGS_KEY, _T("ShowFilterBar"), m_bShowFilterBar);
	prefs.WriteProfileInt(SETTINGS_KEY, _T("ToolbarOption"), m_bShowToolbar);
	prefs.WriteProfileInt(SETTINGS_KEY, _T("ShowProjectName"), m_bShowProjectName);
	prefs.WriteProfileInt(SETTINGS_KEY, _T("ShowStatusBar"), m_bShowStatusBar);
	prefs.WriteProfileInt(SETTINGS_KEY, _T("ShowTasklistBar"), m_bShowTasklistBar);
	prefs.WriteProfileInt(SETTINGS_KEY, _T("ShowTreeListBar"), m_bShowTreeListBar);

	if (m_findDlg.GetSafeHwnd())
		prefs.WriteProfileInt(SETTINGS_KEY, _T("FindTasksVisible"), m_bFindShowing && m_findDlg.IsWindowVisible());
	
	if (Prefs().GetAddFilesToMRU())
		m_mruList.WriteList(prefs, TRUE);

	// quick find items
	CStringArray aItems;

	if (CDialogHelper::GetComboBoxItems(m_cbQuickFind, aItems))
		prefs.WriteProfileArray(_T("QuickFind"), aItems);

	m_mgrShortcuts.SaveSettings(prefs, _T("KeyboardShortcuts"));
	m_mgrUIExtensions.SavePreferences(prefs, _T("UIExtensions"));
	m_mgrContent.SavePreferences(prefs, _T("ContentControls"));
}

LRESULT CToDoListWnd::OnAddToolbarTools(WPARAM /*wp*/, LPARAM /*lp*/)
{
	Misc::ProcessMsgLoop();
	AppendTools2Toolbar();
	return 0L;
}

TDC_PREPAREPATH CToDoListWnd::PrepareFilePath(CString& sFilePath, TSM_TASKLISTINFO* pInfo)
{
	TDC_PREPAREPATH nType = TDCPP_NONE;
	TSM_TASKLISTINFO temp;

	if (pInfo == NULL)
		pInfo = &temp;

	// first test for storage
	if (pInfo->DecodeInfo(sFilePath, Prefs().GetSaveStoragePasswords()))
	{
		sFilePath = pInfo->szLocalFileName;

		// check for overflow and non-existence
		if (sFilePath.IsEmpty() || FileMisc::FileExists(sFilePath))
			nType = TDCPP_STORAGE;
		else
			sFilePath.Empty();
	}
	// else it's a file path.
	// if it starts with a colon then we need to find the removable drive it's stored on
	else if (!sFilePath.IsEmpty())
	{
		if (sFilePath[0] == ':')
		{
			for (int nDrive = 4; nDrive <= 26; nDrive++) // from D: upwards
			{
				if (CDriveInfo::GetType(nDrive) == DRIVE_REMOVABLE)
				{
					CString sTryPath = CDriveInfo::GetLetter(nDrive) + sFilePath;

					if (FileMisc::FileExists(sTryPath))
					{
						sFilePath = sTryPath;
						break; // finished
					}
				}
			}
		}
		else
		{
			FileMisc::MakeFullPath(sFilePath, FileMisc::GetAppFolder()); // handle relative paths
		}

		// check for existence
		if (FileMisc::FileExists(sFilePath))
			nType = TDCPP_FILE;
		else
			sFilePath.Empty();
	}
	
	return nType;
}

LRESULT CToDoListWnd::OnPostOnCreate(WPARAM /*wp*/, LPARAM /*lp*/)
{
	// late initialization
	CMouseWheelMgr::Initialize();
	CEditShortcutMgr::Initialize();
	CFocusWatcher::Initialize(this);

	InitShortcutManager();
	InitMenuIconManager();

	// reminders
	m_reminders.Initialize(this);

	// with or without Stickies Support
	const CPreferencesDlg& userPrefs = Prefs();
	CString sStickiesPath;
	
	if (userPrefs.GetUseStickies(sStickiesPath))
		VERIFY(m_reminders.UseStickies(TRUE, sStickiesPath));
	

	// add outstanding translated items to dictionary
	if (CLocalizer::GetTranslationOption() == ITTTO_ADD2DICTIONARY)
	{
		CDWordArray aDictVersion, aAppVersion;
		VERIFY (FileMisc::GetAppVersion(aAppVersion));

		BOOL bUpdateDict = !CLocalizer::GetDictionaryVersion(aDictVersion) ||
							aDictVersion.GetSize() < 2;

		if (!bUpdateDict)
		{
			// check if the major or minor version has increased
			bUpdateDict = (FileMisc::CompareVersions(aAppVersion, aDictVersion, 2) > 0);

			// check for pre-release build then update
			if (!bUpdateDict && aAppVersion[2] >= 297)
			{
				// compare entire version string
				bUpdateDict = (FileMisc::CompareVersions(aAppVersion, aDictVersion) > 0);
			}
		}

		if (bUpdateDict)
			TranslateUIElements();
	}

	RestoreVisibility();
	
	// load last open tasklists
	CAutoFlag af(m_bReloading, TRUE);
	CPreferences prefs;

	// initialize Progress first time
	m_sbProgress.BeginProgress(m_statusBar, CEnString(IDS_STARTUPPROGRESS));

	// open cmdline tasklist
	int nTDCCount = prefs.GetProfileInt(SETTINGS_KEY, _T("NumLastFiles"), 0);

	if (!m_startupOptions.HasFilePath() || nTDCCount)
	{
		// if we have a file on the commandline or any previous tasklists
		// set the prompt of the initial tasklist to something appropriate
		//	TODO
	}

	// cache empty flag for later
	BOOL bStartupEmpty = m_startupOptions.HasFlag(TLD_STARTEMPTY);

	// what to (re)load?
	BOOL bReloadTasklists = (!bStartupEmpty && userPrefs.GetReloadTasklists());
	
	// filepath overrides
#ifndef _DEBUG
	if (m_startupOptions.HasFilePath())
#endif
	{
		ProcessStartupOptions(m_startupOptions, TRUE);

		// don't reload previous if a tasklist was actually loaded
		if (!m_mgrToDoCtrls.IsPristine(0))
			bReloadTasklists = FALSE;
	}
	
	m_startupOptions.Reset(); // always
	
	// load last files
	if (bReloadTasklists)
	{
		UpdateWindow();

		// get the last active tasklist
		CString sLastActiveFile = prefs.GetProfileString(SETTINGS_KEY, _T("LastActiveFile")), sOrgLastActiveFile;
		BOOL bCanDelayLoad = userPrefs.GetEnableDelayedLoading();

		for (int nTDC = 0; nTDC < nTDCCount; nTDC++)
		{
			CString sKey = Misc::MakeKey(_T("LastFile%d"), nTDC);
			CString sLastFile = prefs.GetProfileString(SETTINGS_KEY, sKey);

			if (!sLastFile.IsEmpty())
			{
				// delay-open all but the non-active tasklist
				// unless the tasklist has reminders
				BOOL bActiveTDC = (sLastFile == sLastActiveFile);

				if (!bActiveTDC && bCanDelayLoad && !m_reminders.ToDoCtrlHasReminders(sLastFile))
				{
					DelayOpenTaskList(sLastFile);
				}
				else
				{
					TDC_FILE nResult = OpenTaskList(sLastFile, FALSE);

					// if the last active tasklist was cancelled then
					// delay load it and mark the last active todoctrl as not found
					if (bActiveTDC && (nResult != TDCF_SUCCESS))
					{
						sOrgLastActiveFile = sLastActiveFile;
						sLastActiveFile.Empty();

						if ((nResult == TDCF_CANCELLED) && bCanDelayLoad)
							DelayOpenTaskList(sLastFile);
					}
				}
			}
		}
		
		// process all pending messages
		Misc::ProcessMsgLoop();

		// if the last active tasklist could not be loaded then we need to find another
		if (GetTDCCount())
		{
			// make Last Active Files actual filepaths
			PrepareFilePath(sLastActiveFile);
			PrepareFilePath(sOrgLastActiveFile);

			if (sLastActiveFile.IsEmpty())
			{
				for (int nTDC = 0; nTDC < GetTDCCount() && sLastActiveFile.IsEmpty(); nTDC++)
				{
					CFilteredToDoCtrl& tdc = GetToDoCtrl(nTDC);

					// ignore original active tasklist
					if (tdc.GetFilePath() != sOrgLastActiveFile)
					{
						if (VerifyTaskListOpen(nTDC, FALSE))
							sLastActiveFile = tdc.GetFilePath();
					}
				}
			}

			// if nothing suitable found then create an empty tasklist
			if (sLastActiveFile.IsEmpty())
			{
				if (GetTDCCount() == 0)
					CreateNewTaskList(FALSE);
			}
			else if (!SelectToDoCtrl(sLastActiveFile, FALSE))
			{
				SelectToDoCtrl(0, FALSE); // the first one
			}

			Resize();
		}
	}

	// if there's only one tasklist open and it's pristine then 
	// it's the original one so add a sample task unless 
	// 'empty' flag is set
	if (GetTDCCount() == 1 && m_mgrToDoCtrls.IsPristine(0))
	{
		if (!bStartupEmpty)
		{
			CFilteredToDoCtrl& tdc = GetToDoCtrl();
			ASSERT (tdc.GetTaskCount() == 0);

			tdc.CreateNewTask(CEnString(IDS_SAMPLETASK), TDC_INSERTATTOP, FALSE);
			tdc.SetModified(FALSE);
			
			UpdateCaption();
		}
	}
	else // due task notifications
	{
		PostMessage(WM_DOINITIALDUETASKNOTIFY);
	}

	// refresh toolbar 'tools' buttons unless minimized because
	// we must handle it when we're first shown
	if (m_bShowToolbar && AfxGetApp()->m_nCmdShow != SW_SHOWMINIMIZED)
		AppendTools2Toolbar();

	// current focus
	PostMessage(WM_FW_FOCUSCHANGE, (WPARAM)::GetFocus(), 0L);
	
	// theme
	SetUITheme(userPrefs.GetUITheme());

	RefreshTabOrder();
	Invalidate(TRUE);

	// end progress before refreshing statusbar
	m_sbProgress.EndProgress();
	UpdateStatusbar();

	// find tasks dialog
	if (prefs.GetProfileInt(SETTINGS_KEY, _T("FindTasksVisible"), 0))
	{
		OnFindTasks();
		
		if (userPrefs.GetRefreshFindOnLoad())
			m_findDlg.RefreshSearch();
	}

	// log the app and its dlls for debugging
	FileMisc::LogAppModuleState(FBM_SORTBY_FILENAME);

	return 0L;
}

void CToDoListWnd::LoadSettings()
{
	// settings
	CPreferences prefs;

	BOOL bMaxTasklists = prefs.GetProfileInt(SETTINGS_KEY, _T("SimpleMode"), FALSE); // backward compatibility
	m_nMaxState = (TDC_MAXSTATE)prefs.GetProfileInt(SETTINGS_KEY, _T("ViewState"), bMaxTasklists ? TDCMS_MAXTASKLIST : TDCMS_NORMAL);

	m_bShowFilterBar = prefs.GetProfileInt(SETTINGS_KEY, _T("ShowFilterBar"), m_bShowFilterBar);
	m_bShowProjectName = prefs.GetProfileInt(SETTINGS_KEY, _T("ShowProjectName"), m_bShowProjectName);
	
	m_bShowStatusBar = prefs.GetProfileInt(SETTINGS_KEY, _T("ShowStatusBar"), m_bShowStatusBar);
	m_statusBar.ShowWindow(m_bShowStatusBar ? SW_SHOW : SW_HIDE);
	
	// toolbar
	m_bShowToolbar = prefs.GetProfileInt(SETTINGS_KEY, _T("ToolbarOption"), TRUE);
	m_toolbar.ShowWindow(m_bShowToolbar ? SW_SHOW : SW_HIDE);
	m_toolbar.EnableWindow(m_bShowToolbar);

	// tabbars
	m_bShowTasklistBar = prefs.GetProfileInt(SETTINGS_KEY, _T("ShowTasklistBar"), TRUE);
	m_bShowTreeListBar = prefs.GetProfileInt(SETTINGS_KEY, _T("ShowTreeListBar"), TRUE);

	// pos
	RestorePosition();

	// user preferences
	const CPreferencesDlg& userPrefs = Prefs();
	
	// MRU
	if (userPrefs.GetAddFilesToMRU())
		m_mruList.ReadList(prefs);
	
	// note: we do not restore visibility until OnPostOnCreate

	// default attributes
	userPrefs.GetDefaultTaskAttributes(m_tdiDefault);
	m_mgrImportExport.SetDefaultTaskAttributes(m_tdiDefault);
	
	// hotkey
	UpdateGlobalHotkey();
	UpdateTimeTrackerPreferences();
	
	// time periods
	CTimeHelper::SetHoursInWorkday(userPrefs.GetHoursInWorkday());
	CTimeHelper::SetWorkdaysInWeek(userPrefs.GetWorkdaysInWeek());

	// support for .tdl extension and tdl:// protocol
	EnableTDLExtension(userPrefs.GetEnableTDLExtension(), TRUE);
	EnableTDLProtocol(userPrefs.GetEnableTDLProtocol(), TRUE);

	// Recently modified period
	CFilteredToDoCtrl::SetRecentlyModifiedPeriod(userPrefs.GetRecentlyModifiedPeriod());

	RestoreTimers();
}

void CToDoListWnd::ProcessProtocolRegistrationFailure(BOOL bStartup, BOOL bExistingReg, UINT nMsgID, LPCTSTR szCheckPrefKey)
{
	// Don't display a message at startup
	if (bStartup)
		return;

	// Don't display a message if we've already told
	// them and we are non-admin
	CPreferences prefs;

	if (prefs.GetProfileInt(SETTINGS_KEY, szCheckPrefKey, FALSE) &&	!FileMisc::IsAdminProcess())
		return;

#ifdef _DEBUG
	// Also don't display an error message under debug
	// if another instance of TDL is installed
	if (bExistingReg)
		return;
#else
	UNREFERENCED_PARAMETER(bExistingReg);
#endif

	AfxMessageBox(nMsgID);

	// Record that we've told them so that we don't
	// tell them again unless they've switched to admin
	prefs.WriteProfileInt(SETTINGS_KEY, szCheckPrefKey, TRUE);
}

void CToDoListWnd::EnableTDLExtension(BOOL bEnable, BOOL bStartup)
{
	if (COSVersion() == OSV_LINUX)
		return;

	CFileRegister filereg(_T("tdl"), _T("tdl_Tasklist"));

	if (bEnable)
	{
		CString sAppPath(FileMisc::GetAppFilePath());

		// If we are a pre-release version then don't overwrite
		// an existing registration
		BOOL bPreRelease = CTDCWebUpdateScript::IsPreRelease(FileMisc::GetAppVersion());
		BOOL bExistingReg = filereg.IsRegisteredApp(_T("tdl"), sAppPath, TRUE);

		if (bPreRelease && bExistingReg)
			return;

		// Try to register app
		if (!filereg.RegisterFileType(_T("Tasklist"), 0))
		{
			ProcessProtocolRegistrationFailure(bStartup, bExistingReg, 
				IDS_ERRORINSTALLTDLEXTENSION, _T("NotifiedTDLExtensionError"));
		}
	}
	else
	{
		if (!filereg.UnRegisterFileType())
		{
			// TODO
		}

		// Reset the error notification
		CPreferences().DeleteProfileEntry(SETTINGS_KEY, _T("NotifiedTDLExtensionError"));
	}
}

void CToDoListWnd::EnableTDLProtocol(BOOL bEnable, BOOL bStartup)
{
	if (COSVersion() == OSV_LINUX)
		return;
	
	if (bEnable)
	{
		// If we are a pre-release version then don't overwrite
		// an existing registration
		BOOL bPreRelease = CTDCWebUpdateScript::IsPreRelease(FileMisc::GetAppVersion());
		BOOL bExistingReg = WebMisc::IsProtocolRegistered(_T("tdl"), _T("ToDoList"));

		if (bPreRelease && bExistingReg)
			return;

		CString sCommand = FileMisc::GetAppFilePath();
		sCommand += _T(" -l \"%1\"");
		
		if (!WebMisc::RegisterProtocol(_T("tdl"), _T("ToDoList"), sCommand))
		{
			ProcessProtocolRegistrationFailure(bStartup, bExistingReg, 
				IDS_ERRORINSTALLTDLPROTOCOL, _T("NotifiedTDLProtocolError"));
		}
	}
	else
	{
		if (!WebMisc::UnregisterProtocol(_T("tdl")))
		{
			// TODO
		}

		// Reset the error notification
		CPreferences().DeleteProfileEntry(SETTINGS_KEY, _T("NotifiedTDLProtocolError"));
	}
}

void CToDoListWnd::RestoreVisibility()
{
	const CPreferencesDlg& userPrefs = Prefs();
	CPreferences prefs;

	int nDefShowState = AfxGetApp()->m_nCmdShow;
	BOOL bShowOnStartup = userPrefs.GetShowOnStartup();

	BOOL bMaximized = (prefs.GetProfileInt(_T("Pos"), _T("Maximized"), FALSE) || (nDefShowState == SW_SHOWMAXIMIZED));
	BOOL bMinimized = (!bShowOnStartup && ((nDefShowState == SW_SHOWMINIMIZED) || (nDefShowState == SW_SHOWMINNOACTIVE)));
	
	if (bMinimized)
	{
		bMaximized = FALSE; // can't be max-ed and min-ed
		m_bStartHidden = TRUE;
	}
	
	if (m_bVisible == -1) // not yet set
	{
		m_bVisible = TRUE;
		
		// the only reason it can be hidden is if it uses the systray
		// and the user has elected to not have it show at startup
		// and it was hidden the last time it closed or it's set to run
		// minimized and that is the trigger to hide it
		if (!bShowOnStartup && userPrefs.GetUseSysTray())
		{
			if (prefs.GetProfileInt(_T("Pos"), _T("Hidden"), FALSE))
			{
				m_bVisible = FALSE;
			}
			// also if wp.showCmd == minimized and we would hide to sys
			// tray when minimized then hide here too
			else if ((nDefShowState == SW_SHOWMINIMIZED) || (nDefShowState == SW_SHOWMINNOACTIVE))
			{
				if (HasSysTrayOptions(STO_ONMINIMIZE, STO_ONMINCLOSE))
				{
					m_bVisible = FALSE;
				}
			}
		}
	}
	
	if (m_bVisible)
	{
		int nShowCmd = (bMaximized ? SW_SHOWMAXIMIZED : 
						(bMinimized ? SW_SHOWMINIMIZED : SW_SHOW));
		
 		ShowWindow(nShowCmd);
		SetForegroundWindow();

		// Startup redraw problem
		if (COSVersion() == OSV_LINUX)
		{
			m_toolbar.Invalidate(TRUE);
			m_filterBar.Invalidate(TRUE);
			m_cbQuickFind.Invalidate(TRUE);
		}

		Invalidate();
		UpdateWindow();
	}
	else
	{
		m_bStartHidden = TRUE;
	}

	// don't set topmost if maximized
#ifndef _DEBUG
	if (userPrefs.GetAlwaysOnTop() && !bMaximized)
		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
#endif
}

void CToDoListWnd::RestorePosition()
{
	CPreferences prefs;

	CRect rect;
	
	rect.left = prefs.GetProfileInt(_T("Pos"), _T("Left"), -1);
	rect.top = prefs.GetProfileInt(_T("Pos"), _T("Top"), -1);
	rect.right = prefs.GetProfileInt(_T("Pos"), _T("Right"), -1);
	rect.bottom = prefs.GetProfileInt(_T("Pos"), _T("Bottom"), -1);
	
	if (rect.Width() > 0 && rect.Height() > 0)
	{
		// ensure this intersects with the closest screen by a decent amount
		int BORDER = 200;
		rect.DeflateRect(BORDER, BORDER);

		CRect rUnused;

		if (GraphicsMisc::GetAvailableScreenSpace(rect, rUnused))
		{
			rect.InflateRect(BORDER, BORDER);

			// because the position was saved using the results of 
			// GetWindowPlacement we must use SetWindowPlacement
			// to restore the window
			WINDOWPLACEMENT wp = { 0 };

			wp.rcNormalPosition = rect;
			wp.ptMaxPosition.x = -1;
			wp.ptMaxPosition.y = -1;
			wp.ptMinPosition.x = -1;
			wp.ptMinPosition.y = -1;

// 			FileMisc::LogText(_T("RestorePosition: TopLeft=(%d,%d) BotRight=(%d,%d) MinPos=(%d,%d) MaxPos=(%d,%d)"), 
// 								wp.rcNormalPosition.left, wp.rcNormalPosition.top,
// 								wp.rcNormalPosition.right, wp.rcNormalPosition.bottom,
// 								wp.ptMaxPosition.x, wp.ptMaxPosition.y,
// 								wp.ptMinPosition.x, wp.ptMinPosition.y);

			SetWindowPlacement(&wp);
		}
		else
		{
			ASSERT(0);
			rect.SetRectEmpty();
		}
	}
	
	// first time or monitors changed?
	if (rect.IsRectEmpty())
	{
		rect.SetRect(0, 0, 1024, 730); // default

		// make sure it fits the screen
		CRect rScreen;
		VERIFY(GraphicsMisc::GetAvailableScreenSpace(rect, rScreen));

		if (rect.Height() > rScreen.Height())
			rect.bottom = rScreen.Height();

		MoveWindow(rect);
		CenterWindow();
	}
}

void CToDoListWnd::OnNew() 
{
	CreateNewTaskList(FALSE);
	RefreshTabOrder();
}

BOOL CToDoListWnd::CreateNewTaskList(BOOL bAddDefTask)
{
	CFilteredToDoCtrl* pNew = NewToDoCtrl();
	
	if (pNew)
	{
		m_dlgTimeTracker.AddTasklist(pNew);

		VERIFY(AddToDoCtrl(pNew) != -1);

		// insert a default task
		if (bAddDefTask)
		{
			if (pNew->GetTaskCount() == 0)
				VERIFY (CreateNewTask(CEnString(IDS_TASK), TDC_INSERTATTOP, FALSE));
		}
		else // ensure it is empty
		{
			pNew->DeleteAllTasks();
		}
		
		// clear modified flag
		pNew->SetModified(FALSE);
	}

	return (pNew != NULL);
}

void CToDoListWnd::OnUpdateDeletetask(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	pCmdUI->Enable(!tdc.IsReadOnly() && tdc.HasSelection());	
}

void CToDoListWnd::OnUpdateEditTasktext(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	int nSelCount = tdc.GetSelectedCount();
	
	pCmdUI->Enable(nSelCount == 1 && !tdc.IsReadOnly());	
}

void CToDoListWnd::OnUpdateTaskcolor(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	pCmdUI->Enable(!tdc.IsReadOnly() && tdc.HasSelection() && 
		Prefs().GetTextColorOption() == COLOROPT_DEFAULT);	
}

void CToDoListWnd::OnUpdateEditTaskdone(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	int nSelCount = tdc.GetSelectedCount();
	
	if (nSelCount == 1)
		pCmdUI->SetCheck(tdc.IsSelectedTaskDone() ? 1 : 0);
	
	pCmdUI->Enable(!tdc.IsReadOnly() && tdc.GetSelectedItem());	
}

void CToDoListWnd::OnUpdateDeletealltasks(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	pCmdUI->Enable(!tdc.IsReadOnly() && GetToDoCtrl().GetTaskCount());	
}

void CToDoListWnd::OnUpdateSave(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	pCmdUI->Enable(GetTDCCount() && tdc.IsModified() && !tdc.IsReadOnly());	
}

void CToDoListWnd::OnUpdateNew(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(TRUE);	
}

BOOL CToDoListWnd::OnEraseBkgnd(CDC* pDC) 
{
	if (!GetTDCCount())
		return CFrameWnd::OnEraseBkgnd(pDC);

	CDialogHelper::ExcludeChild(&m_toolbar, pDC);
	CDialogHelper::ExcludeChild(&m_statusBar, pDC);
	CDialogHelper::ExcludeChild(&m_tabCtrl, pDC);
	CDialogHelper::ExcludeChild(&m_filterBar, pDC);
	CDialogHelper::ExcludeChild(&GetToDoCtrl(), pDC);

	// Fill client background
	COLORREF crBkgnd(CThemed::IsAppThemed() ? m_theme.crAppBackLight : GetSysColor(COLOR_3DFACE));
	
	CRect rClient;
	GetClientRect(rClient);
	pDC->FillSolidRect(rClient, crBkgnd);

	// gap above tabbar
	if (m_tabCtrl.IsWindowVisible()) // can be hidden if only tab showing
	{
		int nVPos = (CDialogHelper::GetChildRect(&m_tabCtrl).top - BEVEL);

		pDC->FillSolidRect(0, nVPos, rClient.Width(), BEVEL, m_theme.crAppBackDark);
	}

	// Bevel below the toolbar
	if (m_bShowToolbar)
	{
 		int nVPos = m_toolbar.GetHeight();
		
		GraphicsMisc::DrawHorzLine(pDC, 0, rClient.Width(), nVPos, m_theme.crAppLinesDark);
		GraphicsMisc::DrawHorzLine(pDC, 0, rClient.Width(), (nVPos + 1), m_theme.crAppLinesLight);
	}	
	
	// bevel below filter-bar and filter-bar edges
	CRect rFilter;

	if (GetFilterBarRect(rFilter))
	{
		COLORREF crBkgnd = m_filterBar.CalcUIBkgndColor();

		if (crBkgnd != CLR_NONE)
			pDC->FillSolidRect(rFilter, crBkgnd);

		GraphicsMisc::DrawHorzLine(pDC, 0, rClient.Width(), rFilter.bottom, m_theme.crAppLinesDark);
		GraphicsMisc::DrawHorzLine(pDC, 0, rClient.Width(), (rFilter.bottom + 1), m_theme.crAppLinesLight);
	}

	// bevel above the statusbar if themed
	if (m_bShowStatusBar && CThemed::IsAppThemed())
	{
		int nVPos = (CDialogHelper::GetChildRect(&m_statusBar).top - BEVEL);

		GraphicsMisc::DrawHorzLine(pDC, 0, rClient.Width(), nVPos, m_theme.crAppLinesDark);
		GraphicsMisc::DrawHorzLine(pDC, 0, rClient.Width(), (nVPos + 1), m_theme.crAppLinesLight);
	}

	// this is definitely amongst the worst hacks I've ever had to implement.
	// It occurs because the CSysImageList class seems to not initialize 
	// properly unless the main window is visible. so in the case of starting hidden
	// or starting minimized we must wait until we become visible before
	// adding the tools to the toolbar.
	if (m_bStartHidden)
	{
		m_bStartHidden = FALSE;
		PostMessage(WM_ADDTOOLBARTOOLS);
	}

	return TRUE;
}

BOOL CToDoListWnd::GetFilterBarRect(CRect& rect) const
{
	if (!m_bShowFilterBar)
		return FALSE;

	CRect rClient;
	GetClientRect(rClient);

	rect = CDialogHelper::GetChildRect(&m_filterBar);

	rect.left = 0;
	rect.right = rClient.right;
	rect.top--;

	return TRUE;
}

void CToDoListWnd::OnSortBy(UINT nCmdID) 
{
	if (nCmdID == ID_SORT_BYMULTI)
		return;

	TDC_COLUMN nSortBy = TDC::MapSortIDToColumn(nCmdID);
	
	// update todoctrl
	GetToDoCtrl().Sort(nSortBy);
}

void CToDoListWnd::OnUpdateSortBy(CCmdUI* pCmdUI)
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	TDC_COLUMN nSortBy = TDC::MapSortIDToColumn(pCmdUI->m_nID);

	switch (pCmdUI->m_nID)
	{
	case ID_SORT_BYMULTI:
		pCmdUI->Enable(tdc.CanMultiSort());
		break;

	case ID_SORT_BYNONE:
		pCmdUI->Enable(TRUE);
		break;
		
	case ID_SORT_BYCOLOR:
		pCmdUI->Enable(Prefs().GetTextColorOption() == COLOROPT_DEFAULT);
		break;
		
	case ID_SORT_BYPATH:
		pCmdUI->Enable(tdc.GetView() != FTCV_TASKTREE);
		break;
		
	default:
		pCmdUI->Enable(tdc.CanSortBy(nSortBy));
		break;
	}

	// Radio button on current selection
	if (pCmdUI->m_pMenu)
	{
		if (tdc.IsMultiSorting())
		{
			pCmdUI->m_pMenu->CheckMenuRadioItem(ID_SORT_BYFIRST, ID_SORT_BYLAST, ID_SORT_BYMULTI, MF_BYCOMMAND);
		}
		else if (tdc.GetSortBy() == nSortBy)
		{
			pCmdUI->m_pMenu->CheckMenuRadioItem(ID_SORT_BYFIRST, ID_SORT_BYLAST, pCmdUI->m_nID, MF_BYCOMMAND);
		}
	}
}


void CToDoListWnd::OnNewtaskAttopSelected() 
{
	VERIFY (CreateNewTask(CEnString(IDS_TASK), TDC_INSERTATTOPOFSELTASKPARENT));
}

void CToDoListWnd::OnNewtaskAtbottomSelected() 
{
	VERIFY (CreateNewTask(CEnString(IDS_TASK), TDC_INSERTATBOTTOMOFSELTASKPARENT));
}

void CToDoListWnd::OnNewtaskAfterselectedtask() 
{
	VERIFY (CreateNewTask(CEnString(IDS_TASK), TDC_INSERTAFTERSELTASK));
}

void CToDoListWnd::OnNewDependenttaskAfterselectedtask() 
{
	VERIFY (CreateNewDependentTaskBelowSelectedTask(CEnString(IDS_TASK)));
}

void CToDoListWnd::OnNewtaskBeforeselectedtask() 
{
	VERIFY (CreateNewTask(CEnString(IDS_TASK), TDC_INSERTBEFORESELTASK));
}

void CToDoListWnd::OnNewsubtaskAtbottom() 
{
	VERIFY (CreateNewTask(CEnString(IDS_TASK), TDC_INSERTATBOTTOMOFSELTASK));
}

void CToDoListWnd::OnNewsubtaskAttop() 
{
	VERIFY (CreateNewTask(CEnString(IDS_TASK), TDC_INSERTATTOPOFSELTASK));
}

void CToDoListWnd::OnNewtaskAttop() 
{
	VERIFY (CreateNewTask(CEnString(IDS_TASK), TDC_INSERTATTOP));
}

void CToDoListWnd::OnNewtaskAtbottom() 
{
	VERIFY (CreateNewTask(CEnString(IDS_TASK), TDC_INSERTATBOTTOM));
}

BOOL CToDoListWnd::CreateNewTask(const CString& sTitle, TDC_INSERTWHERE nInsertWhere, BOOL bEdit, DWORD dwDependency)
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	if (!tdc.CreateNewTask(sTitle, nInsertWhere, bEdit, dwDependency))
		return FALSE;

	// else
	CheckCreateDefaultReminder(tdc, tdc.GetSelectedTaskID());

	return TRUE;
}

BOOL CToDoListWnd::CreateNewDependentTaskBelowSelectedTask(const CString& sTitle, BOOL bEdit)
{
	DWORD dwDependency = GetToDoCtrl().GetSelectedTaskID();
	
	return CreateNewTask(sTitle, TDC_INSERTAFTERSELTASK, bEdit, dwDependency);
}

void CToDoListWnd::CheckCreateDefaultReminder(const CFilteredToDoCtrl& tdc, DWORD dwTaskID)
{
	UINT nReminderLeadin = 0;
	BOOL bReminderBeforeDue = TRUE;

	if (Prefs().GetDefaultReminder(nReminderLeadin, bReminderBeforeDue))
	{
		ASSERT(tdc.HasTask(dwTaskID));

		TDCREMINDER rem;
		
		rem.dwTaskID = dwTaskID;
		rem.pTDC = &tdc;
		rem.bRelative = TRUE;
		rem.dRelativeDaysLeadIn = (nReminderLeadin / (24.0 * 60 * 60));
		rem.nRelativeFromWhen = (bReminderBeforeDue ? TDCR_DUEDATE : TDCR_STARTDATE);
		rem.bEnabled = TRUE;

		m_reminders.SetReminder(rem);
	}
}

void CToDoListWnd::OnUpdateSort(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	pCmdUI->Enable(tdc.IsSorting() && tdc.GetTaskCount());	
}

void CToDoListWnd::OnEditTaskcolor() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	if (!tdc.IsReadOnly() && tdc.HasSelection())
	{
		CEnColorDialog dialog(tdc.GetSelectedTaskColor(), CC_FULLOPEN | CC_RGBINIT);
		
		if (dialog.DoModal() == IDOK)
			tdc.SetSelectedTaskColor(dialog.GetColor());
	}
}

void CToDoListWnd::OnEditCleartaskcolor() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	if (!tdc.IsReadOnly() && tdc.HasSelection())
		tdc.ClearSelectedTaskColor();
}

void CToDoListWnd::OnUpdateEditCleartaskcolor(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	pCmdUI->Enable(!tdc.IsReadOnly() && tdc.HasSelection() && 
					Prefs().GetTextColorOption() == COLOROPT_DEFAULT &&
					tdc.GetSelectedTaskColor() != 0);	
}

void CToDoListWnd::OnEditTaskdone() 
{
	GetToDoCtrl().SetSelectedTaskDone(!GetToDoCtrl().IsSelectedTaskDone());
}

void CToDoListWnd::OnEditTasktext() 
{
	GetToDoCtrl().EditSelectedTask();
}

void CToDoListWnd::OnTrayIconClick(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	SetFocus();
	Show(Prefs().GetToggleTrayVisibility());
	*pResult = 0;
}

LRESULT CToDoListWnd::OnToDoListShowWindow(WPARAM /*wp*/, LPARAM /*lp*/)
{
	Show(FALSE);
	return 0;
}

LRESULT CToDoListWnd::OnToDoListGetVersion(WPARAM /*wp*/, LPARAM /*lp*/)
{
	return GetVersion();
}

LRESULT CToDoListWnd::OnToDoListRefreshPrefs(WPARAM /*wp*/, LPARAM /*lp*/)
{
	// sent by the app object if registry settings have changed
	ResetPrefs();

	// mark all tasklists as needing update
	m_mgrToDoCtrls.SetAllNeedPreferenceUpdate(TRUE);
	
	// then update active tasklist
	UpdateActiveToDoCtrlPreferences();

	return 0;
}

void CToDoListWnd::OnTrayIconDblClk(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	Show(FALSE);
	
	*pResult = 0;
}

void CToDoListWnd::OnTrayiconCreatetask() 
{
	Show(FALSE);
	
	// create a task at the top of the tree
	GetToDoCtrl().CreateNewTask(CEnString(IDS_TASK), TDC_INSERTATTOP);
}

void CToDoListWnd::OnTrayIconRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (IsWindowVisible() && !IsIconic())
		SetForegroundWindow();
	
	// show context menu
	CEnMenu menu;
	
	if (menu.LoadMenu(IDR_MISC, m_trayIcon.GetSafeHwnd(), TRUE))
	{
		CMenu* pSubMenu = menu.GetSubMenu(MM_TRAYICON);
		
		if (pSubMenu)
		{
			m_mgrToDoCtrls.PreparePopupMenu(*pSubMenu, ID_TRAYICON_SHOWDUETASKS1);
			pSubMenu->SetDefaultItem(ID_TRAYICON_SHOW);
			
			NM_TRAYICON* pNMTI = (NM_TRAYICON*)pNMHDR;

			// in order to ensure that multiple password dialogs cannot 
			// appear we must make sure that all the command handling is
			// done before we return from here
			UINT nCmdID = ::TrackPopupMenu(*pSubMenu, TPM_RETURNCMD | TPM_LEFTALIGN | TPM_LEFTBUTTON, 
											pNMTI->ptAction.x, pNMTI->ptAction.y, 0, *this, NULL);

			PostMessage(WM_NULL);

			if (nCmdID != (UINT)-1)
				SendMessage(WM_COMMAND, nCmdID);
		}
	}
	
	*pResult = 0;
}

LRESULT CToDoListWnd::OnClose(WPARAM /*wp*/, LPARAM bForUpdate) 
{
	if (!m_bEndingSession)
	{
		if (!bForUpdate && HasSysTrayOptions(STO_ONCLOSE, STO_ONMINCLOSE))
		{
			MinimizeToTray();
		}
		else // shutdown but user can cancel
		{
			DoExit();
		}
	}
	// else we've already shutdown

	return 0L;
}

void CToDoListWnd::MinimizeToTray()
{
	// end whatever the user is doing
	GetToDoCtrl().Flush();

	// save prev state so we can restore properly
	CPreferences().WriteProfileInt(_T("Pos"), _T("Maximized"), IsZoomed());
	
	if (Prefs().GetAutoSaveOnSwitchApp())
	{
		// save all
		SaveAll(TDLS_FLUSH | TDLS_AUTOSAVE);
	}
	
	// hide main window
	Gui::MinToTray(*this); // courtesy of floyd
	m_bVisible = FALSE;
	
	// hide find dialog
	ShowFindDialog(FALSE);
}

void CToDoListWnd::ShowFindDialog(BOOL bShow)
{
	if (bShow)
	{
		if (m_bVisible && m_findDlg.GetSafeHwnd() && IsWindowVisible())
			m_findDlg.Show(TRUE);
	}
	else // hide
	{
		if (m_findDlg.GetSafeHwnd())
		{
			m_bFindShowing = m_findDlg.IsWindowVisible();
			m_findDlg.Show(FALSE);
		}
		else
			m_bFindShowing = FALSE;
	}
}

void CToDoListWnd::OnTrayiconClose() 
{
	DoExit();
}

LRESULT CToDoListWnd::OnToDoCtrlNotifyListChange(WPARAM /*wp*/, LPARAM lp)
{
	// decide whether the filter controls need updating
	switch (lp)
	{
	case TDCA_ALLOCTO:
	case TDCA_ALLOCBY:
	case TDCA_STATUS:
	case TDCA_CATEGORY:
	case TDCA_VERSION:
	case TDCA_TAGS:
		RefreshFilterBarControls();
		RefreshFindTasksListData();
		break;
	}
	
	return 0L;
}

LRESULT CToDoListWnd::OnToDoCtrlNotifyViewChange(WPARAM wp, LPARAM lp)
{
	if (GetTDCCount())
	{
		if (lp != (LPARAM)wp)
		{
			CFocusWatcher::UpdateFocus();
			m_filterBar.RefreshFilterControls(GetToDoCtrl());
		}
		else
		{
			int breakpoint = 0;
		}
	}

	return 0L;
}

void CToDoListWnd::OnEditTimeTrackTask() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	BOOL bTrack = !tdc.IsSelectedTaskBeingTimeTracked();
	int nTDC = GetSelToDoCtrl();
	
	if (bTrack)
	{
		StartTimeTrackingTask(nTDC, tdc.GetSelectedTaskID(), FROM_APP);
	}
	else
	{
		StopTimeTrackingTask(nTDC, FROM_APP);
	}
}

void CToDoListWnd::OnUpdateEditTimeTrackTask(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	pCmdUI->Enable(tdc.CanTimeTrackSelectedTask());
	pCmdUI->SetCheck(tdc.IsSelectedTaskBeingTimeTracked() ? 1 : 0);
}

LRESULT CToDoListWnd::OnToDoCtrlNotifyTimeTrack(WPARAM wp, LPARAM lp)
{
	BOOL bTrack = lp;
	int nTDC = m_mgrToDoCtrls.FindToDoCtrl((HWND)wp);

	if (bTrack)
	{
		DWORD dwTaskID = GetToDoCtrl(nTDC).GetTimeTrackTaskID(FALSE);
		StartTimeTrackingTask(nTDC, dwTaskID, FROM_TASKLIST);
	}
	else
	{
		StopTimeTrackingTask(nTDC, FROM_TASKLIST);
	}

	return 0L;
}
	
void CToDoListWnd::StartTimeTrackingTask(int nTDC, DWORD dwTaskID, TIMETRACKSRC nFrom)
{
	ASSERT(nTDC != -1);

	// Handle time tracking on every OTHER tasklist first
	BOOL bExclusive = Prefs().GetExclusiveTimeTracking();

	for (int nCtrl = 0; nCtrl < GetTDCCount(); nCtrl++)
	{
		if ((nCtrl != nTDC) && m_mgrToDoCtrls.IsLoaded(nCtrl))
		{
			CFilteredToDoCtrl& tdc = GetToDoCtrl(nCtrl);

			if (tdc.GetTimeTrackTaskID(FALSE) != 0)
			{
				if (bExclusive)
					tdc.EndTimeTracking(FALSE);

				m_dlgTimeTracker.UpdateTracking(&tdc);
				m_mgrToDoCtrls.RefreshTimeTracking(nCtrl);
			}
		}
	}

	// Update Time track widget OR todoctrl depending on who notified
	int nSel = GetSelToDoCtrl();
	
	if (nFrom != FROM_TRACKER)
	{
		ASSERT(nTDC == nSel);

		m_dlgTimeTracker.UpdateTracking(&GetToDoCtrl());
	}
	
	if (nFrom != FROM_TASKLIST)
	{
		if ((nTDC == nSel) || SelectToDoCtrl(nTDC, TRUE))
			GetToDoCtrl().BeginTimeTracking(dwTaskID);
	}

	// always refresh the notifier tasklist
	m_mgrToDoCtrls.RefreshTimeTracking(nTDC);

	if (nTDC != nSel)
		m_mgrToDoCtrls.RefreshTimeTracking(nSel);

	// update tray icon
	OnTimerTimeTracking();
}

void CToDoListWnd::StopTimeTrackingTask(int nTDC, TIMETRACKSRC nFrom)
{
	if (!m_bClosing)
	{
		ASSERT(nTDC != -1);
		ASSERT((nFrom == FROM_TRACKER) || (nTDC == GetSelToDoCtrl()));

		CFilteredToDoCtrl& tdc = GetToDoCtrl(nTDC);
		
		// Update Time track widget
		if (nFrom != FROM_TRACKER)
		{
			m_dlgTimeTracker.UpdateTracking(&tdc);
		}

		// Update todoctrl
		if (nFrom != FROM_TASKLIST)
		{
			tdc.EndTimeTracking(TRUE);
		}

		// update tab image
		m_mgrToDoCtrls.RefreshTimeTracking(nTDC);
		
		// update tray icon
		OnTimerTimeTracking();
	}
}

LRESULT CToDoListWnd::OnTimeTrackerStartTracking(WPARAM wParam, LPARAM lParam)
{
	int nTDC = m_mgrToDoCtrls.FindToDoCtrl((CFilteredToDoCtrl*)lParam);
	ASSERT(nTDC != -1);
	
	DWORD dwTaskID = wParam;
	ASSERT(wParam);

	StartTimeTrackingTask(nTDC, dwTaskID, FROM_TRACKER);

	return 0L;
}

LRESULT CToDoListWnd::OnTimeTrackerStopTracking(WPARAM /*wParam*/, LPARAM lParam)
{
	int nTDC = m_mgrToDoCtrls.FindToDoCtrl((const CFilteredToDoCtrl*)lParam);
	ASSERT(nTDC != -1);
	
	StopTimeTrackingTask(nTDC, FROM_TRACKER);
	
	return 0L;
}

LRESULT CToDoListWnd::OnTimeTrackerLoadDelayedTasklist(WPARAM /*wParam*/, LPARAM lParam)
{
	int nTDC = m_mgrToDoCtrls.FindToDoCtrl((const CFilteredToDoCtrl*)lParam);
	ASSERT(nTDC != -1);

	return VerifyTaskListOpen(nTDC, FALSE);
}

LRESULT CToDoListWnd::OnToDoCtrlNotifyRecreateRecurringTask(WPARAM wp, LPARAM lp)
{
	DWORD dwTaskID = wp, dwNewTaskID = lp;

	// is there a reminder that we need to copy for the new task
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	TDCREMINDER rem;

	int nRem = m_reminders.FindReminder(dwTaskID, &tdc);

	if (nRem != -1)
	{
		// Transfer the original if the task id has changed
		if (dwNewTaskID != dwTaskID)		
		{
			if (m_reminders.TransferReminder(dwTaskID, dwNewTaskID, &tdc))
				tdc.RedrawReminders();
		}
		else // Update the existing reminder
		{
			// get the existing reminder
			m_reminders.GetReminder(nRem, rem);

			// init for new task
			rem.bEnabled = TRUE;
			rem.dDaysSnooze = 0.0;

			// Add
			m_reminders.SetReminder(rem);
		}
	}

	return 0L;
}

LRESULT CToDoListWnd::OnToDoCtrlNotifyMod(WPARAM wp, LPARAM lp)
{
	int nTDC = m_mgrToDoCtrls.FindToDoCtrl((HWND)wp);

	if (nTDC == -1)
	{
		// could be a notification from a TDC not yet added
		return 0L;
	}

	CFilteredToDoCtrl& tdc = GetToDoCtrl(nTDC);
	TDC_ATTRIBUTE nAttrib = (TDC_ATTRIBUTE)lp;

	switch (nAttrib)
	{
	case TDCA_PROJNAME:
		if (Prefs().GetKeepTabsOrdered())
			RefreshTabOrder();
		break;

	case TDCA_DUEDATE:
		OnTimerDueItems(nTDC);
		break;
		
	case TDCA_DONEDATE:
		OnTimerDueItems(nTDC);
		UpdateTimeTrackerTasks(tdc, FALSE);

		m_dlgTimeTracker.RemoveCompletedTasks(&tdc);

		if (m_reminders.RemoveCompletedTasks(&tdc))
			tdc.RedrawReminders();
		break;

	case TDCA_DELETE:
		m_dlgTimeTracker.RemoveDeletedTasks(&tdc);

		if (m_reminders.RemoveDeletedTasks(&tdc))
			tdc.RedrawReminders();
		break;

	case TDCA_TIMEEST:
	case TDCA_TIMESPENT:
		m_dlgTimeTracker.UpdateTaskTime(&tdc);
		break;

	case TDCA_TASKNAME:
	case TDCA_NEWTASK:
		UpdateTimeTrackerTasks(tdc, FALSE);
		break;
	}

	// Update UI
	UpdateStatusbar();
	UpdateCaption();

	// refresh toolbar states
	PostMessage(WM_IDLEUPDATECMDUI, TRUE);

	// do we need to update the current todoctrl's
	// custom attributes on the find dialog?
	if (m_findDlg.GetSafeHwnd() && (nAttrib == TDCA_CUSTOMATTRIBDEFS))
	{
		UpdateFindDialogActiveTasklist(&tdc);
	}

	return 0L;
}

void CToDoListWnd::UpdateCaption()
{
	int nSel = GetSelToDoCtrl();
	
	CString sProjectName = m_mgrToDoCtrls.UpdateTabItemText(nSel);
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	CString sCaption, sCopyright(MAKEINTRESOURCE(IDS_COPYRIGHT));
	CString sStatus = m_mgrToDoCtrls.GetTabItemTooltip(nSel);
	
	// add current focus text as required
	if (!sStatus.IsEmpty())
	{
		if (m_bShowStatusBar || m_sCurrentFocus.IsEmpty())
			sCaption.Format(_T("%s [%s] - %s"), sProjectName, sStatus, sCopyright);
		else
			sCaption.Format(_T("%s [%s][%s] - %s"), sProjectName, m_sCurrentFocus, sStatus, sCopyright);
	}
	else
	{
		if (m_bShowStatusBar || m_sCurrentFocus.IsEmpty())
			sCaption.Format(_T("%s - %s"), sProjectName, sCopyright);
		else
			sCaption.Format(_T("%s [%s] - %s"), sProjectName, m_sCurrentFocus, sCopyright);
	}
	
	// prepend task pathname if tasklist not visible
	if (m_nMaxState == TDCMS_MAXCOMMENTS)
	{
		// quote the path to help it stand-out
		CString sTaskPath;
		sTaskPath.Format(_T("\"%s\""), tdc.GetSelectedTaskPath(TRUE, 100));
		
		if (!sTaskPath.IsEmpty())
			sCaption = (sTaskPath + " - " + sCaption);
	}
	
	CLocalizer::IgnoreString(sCaption);
	SetWindowText(sCaption);

	// set tray tip too
	UpdateTooltip();
}

void CToDoListWnd::UpdateTooltip()
{
    // base the tooltip on our current caption
    CString sTooltip;
    GetWindowText(sTooltip);

    // move copyright to next line and remove '-'
    sTooltip.Replace(_T(" - "), ENDL);

    // prefix with selected task as first line
    CFilteredToDoCtrl& tdc = GetToDoCtrl();
    DWORD dwSelID = tdc.GetSelectedTaskID();

    if (dwSelID)
    {
        CString sSelItem = tdc.GetTaskTitle(dwSelID);

        // maximum length of tooltip is 128 including null
        if (sSelItem.GetLength() > (128 - sTooltip.GetLength() - 6))
        {
            sSelItem = sSelItem.Left(128 - sTooltip.GetLength() - 6);
            sSelItem += _T("...");
        }
        else if (tdc.GetSelectedCount() > 1)
            sSelItem += _T(", ...");

        sTooltip = (sSelItem + ENDL + sTooltip);
    }

	CLocalizer::IgnoreString(sTooltip);

    m_trayIcon.SetTip(sTooltip);
}

BOOL CToDoListWnd::Export2Html(const CTaskFile& tasks, const CString& sFilePath, const CString& sStylesheet) const
{
	CWaitCursor cursor;
	
	if (FileMisc::FileExists(sStylesheet))
	{
		return tasks.TransformToFile(sStylesheet, sFilePath, SFEF_UTF8);
	}
	
	// else default export
	return m_mgrImportExport.ExportTaskListToHtml(&tasks, sFilePath);
}

void CToDoListWnd::OnSaveas() 
{
	int nSel = GetSelToDoCtrl();
	CFilteredToDoCtrl& tdc = GetToDoCtrl(nSel);

	// use tab text as hint to user
	CString sFilePath = m_mgrToDoCtrls.GetFilePath(nSel, FALSE);
	CPreferences prefs;

	// display the dialog
	CFileSaveDialog dialog(IDS_SAVETASKLISTAS_TITLE,
							GetDefaultFileExt(), 
							sFilePath, 
							EOFN_DEFAULTSAVE,
							GetFileFilter(), 
							this);
	
	// always use .tdl for initializing the file dialog
	dialog.m_ofn.nFilterIndex = 1;
	
	int nRes = dialog.DoModal(prefs);
	
	if (nRes == IDOK)
	{
 		if (SaveTaskList(nSel, dialog.GetPathName()) == TDCF_SUCCESS)
		{
			m_mgrToDoCtrls.ClearStorageDetails(nSel);
			tdc.SetAlternatePreferencesKey(_T(""));
		}

		UpdateStatusbar();
	}
}

void CToDoListWnd::OnUpdateSaveas(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	pCmdUI->Enable(tdc.GetTaskCount() || tdc.IsModified());
}

void CToDoListWnd::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	static UINT nActiveMenuID = 0; // prevent reentrancy
	UINT nMenuID = 0;

	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	BOOL bKeyboard = (point.x == -1 && point.y == -1);
	
	if (pWnd == &m_tabCtrl)
	{
		// if point.x,y are both -1 then we just use the selection tab
		if (bKeyboard)
		{
			CRect rTab;
			
			if (m_tabCtrl.GetItemRect(m_tabCtrl.GetCurSel(), rTab))
			{
				point = rTab.CenterPoint();
				m_tabCtrl.ClientToScreen(&point);
				
				// load popup menu
				nMenuID = MM_TABCTRLCONTEXT;
			}
		}
		else
		{
			// activate clicked tab
			TCHITTESTINFO tcht = { { point.x, point.y }, TCHT_NOWHERE };
			m_tabCtrl.ScreenToClient(&tcht.pt);
			
			int nTab = m_tabCtrl.HitTest(&tcht);
			
			if (nTab != -1 && !(tcht.flags & TCHT_NOWHERE))
			{
				if (nTab != m_tabCtrl.GetCurSel())
				{
					if (!SelectToDoCtrl(nTab, TRUE))
						return; // user cancelled
				}
				
				m_tabCtrl.SetFocus(); // give user feedback
				
				// load popup menu
				nMenuID = MM_TABCTRLCONTEXT;
			}
		}
	}
	else if (pWnd == (CWnd*)&tdc) // try active todoctrl
	{
		// if point.x,y are both -1 then we just use the selected task
		if (bKeyboard && tdc.WantTaskContextMenu())
		{
			CRect rSelection;
			
			if (tdc.GetSelectionBoundingRect(rSelection))
			{
				point.x = min(rSelection.left + 50, rSelection.CenterPoint().x);
				point.y = rSelection.top + 8;

				tdc.ClientToScreen(&point);
			}
		}

		TDC_HITTEST nHit = tdc.HitTest(point);

		switch (nHit)
		{
		case TDCHT_NOWHERE:
			break;

		case TDCHT_TASKLIST:
		case TDCHT_TASK:
			if (tdc.WantTaskContextMenu())
				nMenuID = MM_TASKCONTEXT;
			break;

		case TDCHT_COLUMNHEADER:
			nMenuID = MM_HEADERCONTEXT;
			break;
		}
	}
	
	// show the menu
	if (nMenuID && nMenuID != nActiveMenuID)
	{
		CEnMenu menu;
		
		if (menu.LoadMenu(IDR_MISC, NULL, TRUE))
		{
			CMenu* pPopup = menu.GetSubMenu(nMenuID);
			
			if (pPopup)
			{
				// some special handling
				switch (nMenuID)
				{
				case MM_TASKCONTEXT:
					m_nContextColumnID = tdc.ColumnHitTest(point);
					PrepareEditMenu(pPopup);
					break;
				}
				
				CToolbarHelper::PrepareMenuItems(pPopup, this);
				
				nActiveMenuID = nMenuID;
				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);
				nActiveMenuID = 0;
			}
		}
	}
	else
		CFrameWnd::OnContextMenu(pWnd, point);
}

void CToDoListWnd::OnTrayiconShow() 
{
	Show(FALSE);
}

void CToDoListWnd::OnTrayiconShowDueTasks(UINT nCmdID) 
{
	int nTDC = nCmdID - ID_TRAYICON_SHOWDUETASKS1;
	int nSelTDC = GetSelToDoCtrl();

	// verify password if encrypted tasklist is active
	// unless app is already visible
	if (!m_bVisible || IsIconic() || (nTDC != nSelTDC))
	{
		if (!VerifyToDoCtrlPassword(nTDC))
			return;
	}

	CFilteredToDoCtrl& tdc = GetToDoCtrl(nTDC);

	if (!DoDueTaskNotification(nTDC, PFP_DUETODAY))
	{
		CEnString sMessage(IDS_NODUETODAY, m_mgrToDoCtrls.GetFriendlyProjectName(nTDC));
		MessageBox(sMessage);//, IDS_DUETASKS_TITLE);
	}
}

LRESULT CToDoListWnd::OnHotkey(WPARAM /*wp*/, LPARAM /*lp*/)
{
	Show(TRUE);
	return 0L;
}

BOOL CToDoListWnd::VerifyToDoCtrlPassword() const
{
	return VerifyToDoCtrlPassword(GetSelToDoCtrl());
}

BOOL CToDoListWnd::VerifyToDoCtrlPassword(int nIndex) const
{
	return (m_bPasswordPrompting ? m_mgrToDoCtrls.VerifyPassword(nIndex) : TRUE);
}

void CToDoListWnd::Show(BOOL bAllowToggle)
{
	if (GetSelToDoCtrl() == -1)
		return;
	
	if (!m_bVisible || !IsWindowVisible()) // restore from the tray
	{
		SetForegroundWindow();
		
		if (!VerifyToDoCtrlPassword())
			return;

		m_bVisible = TRUE;
		Gui::RestoreFromTray(*this, CPreferences().GetProfileInt(_T("Pos"), _T("Maximized"), FALSE));

		// restore find dialog
		if (m_bFindShowing)
			m_findDlg.Show();
	}
	else if (IsIconic())
	{
		SetForegroundWindow();
		ShowWindow(SW_RESTORE); // this will force a password check
	}
	// if we're already visible then either bring to the foreground 
	// or hide if we're right at the top of the z-order
	else if (!bAllowToggle || Gui::IsObscured(*this) || !Gui::HasFocus(*this, TRUE))
	{
		SetForegroundWindow();
	}
	else if (HasSysTrayOptions(STO_NONE))
	{
		ShowWindow(SW_MINIMIZE);
	}
	else // hide to system tray
	{
		MinimizeToTray();
	}

	// refresh all tasklists if we are visible
	if (m_bVisible && !IsIconic())
	{
		const CPreferencesDlg& userPrefs = Prefs();
		
		if (userPrefs.GetReadonlyReloadOption() != RO_NO)
			OnTimerReadOnlyStatus();
		
		if (userPrefs.GetTimestampReloadOption() != RO_NO)
			OnTimerTimestampChange();
		
		OnTimerCheckoutStatus();
	}	

	GetToDoCtrl().SetFocusToTasks();
}

#ifdef _DEBUG
void CToDoListWnd::OnDebugEndSession() 
{ 
	SendMessage(WM_QUERYENDSESSION); 
	SendMessage(WM_ENDSESSION, 1, 0); 
}

void CToDoListWnd::OnDebugShowSetupDlg() 
{ 
	CTDLWelcomeWizard dialog;
	dialog.DoModal();
}
#endif

void CToDoListWnd::TranslateUIElements() 
{ 
	// show progress bar
	DOPROGRESS(IDS_UPDATINGDICTIONARY);

	// disable translation of top-level menu names in
	// IDR_MISC, IDR_TASKVIEWVISIBILITY, IDR_TREEDRAGDROP
	CLocalizer::IgnoreString(_T("TrayIcon"));
	CLocalizer::IgnoreString(_T("TaskContext"));
	CLocalizer::IgnoreString(_T("TabCtrl"));
	CLocalizer::IgnoreString(_T("TasklistHeader"));
	CLocalizer::IgnoreString(_T("CommentsPopup"));
	CLocalizer::IgnoreString(_T("ToolsDialog"));
	CLocalizer::IgnoreString(_T("TreeDragDrop"));
	CLocalizer::IgnoreString(_T("TaskViews"));
	CLocalizer::IgnoreString(_T("Debug"));
	CLocalizer::IgnoreString(_T("Task Dialog"));

	CLocalizer::IgnoreString(IDS_ABOUTCONTRIBUTION);

	// disable translation of debug menu
	CLocalizer::EnableTranslation(ID_DEBUGENDSESSION, FALSE);
	CLocalizer::EnableTranslation(ID_DEBUGSHOWSCRIPTDLG, FALSE);
	CLocalizer::EnableTranslation(ID_DEBUGSHOWSETUPDLG, FALSE);
	CLocalizer::EnableTranslation(ID_DEBUGSHOWUPDATEDLG, FALSE);
	CLocalizer::EnableTranslation(ID_DEBUGTASKDIALOG_INFO, FALSE);
	CLocalizer::EnableTranslation(ID_DEBUGTASKDIALOG_WARNING, FALSE);
	CLocalizer::EnableTranslation(ID_DEBUGTASKDIALOG_ERROR, FALSE);

	CLocalizer::ForceTranslateAllUIElements(IDS_FIRSTSTRING, IDS_LASTSTRING);

	// force a redraw of whole UI
	if (IsWindowVisible())
	{
		ShowWindow(SW_HIDE);
		ShowWindow(SW_SHOW);
	}
	
	SetForegroundWindow();
}

void CToDoListWnd::OnUpdateRecentFileMenu(CCmdUI* pCmdUI) 
{
	// check that this is not occurring because our CFrameWnd
	// base class is routing this to the first item in a submenu
	if (pCmdUI->m_pMenu && 
		pCmdUI->m_pMenu->GetMenuItemID(pCmdUI->m_nIndex) == (UINT)-1)
		return;

	m_mruList.UpdateMenu(pCmdUI);	
}

BOOL CToDoListWnd::OnOpenRecentFile(UINT nID)
{
	ASSERT(nID >= ID_FILE_MRU_FILE1);
	ASSERT(nID < ID_FILE_MRU_FILE1 + (UINT)m_mruList.GetSize());
	
	int nIndex = nID - ID_FILE_MRU_FILE1;
	
	CString sTaskList = m_mruList[nIndex];
	TDC_FILE nOpen = OpenTaskList(sTaskList);
	
	if (nOpen == TDCF_SUCCESS)
	{
		Resize();
		UpdateWindow();
	}
	else
	{
		HandleLoadTasklistError(nOpen, sTaskList);

		switch (nOpen)
		{
		case TDCF_NOTEXIST:
		case TDCF_NOTTASKLIST:
			m_mruList.Remove(nIndex);
			break;
		}
	}

	RefreshTabOrder();
	
	// always return TRUE to say we handled it
	return TRUE;
}

void CToDoListWnd::RefreshTabOrder()
{
	if (Prefs().GetKeepTabsOrdered())
	{
		int nSelOrg = GetSelToDoCtrl();
		int nSel = m_mgrToDoCtrls.SortToDoCtrlsByName();
		
		if (nSel != nSelOrg)
			SelectToDoCtrl(nSel, FALSE);
	}
}

TDC_FILE CToDoListWnd::DelayOpenTaskList(LPCTSTR szFilePath)
{
	ASSERT (Prefs().GetEnableDelayedLoading()); // sanity check

	// decode/prepare filepath
	CString sFilePath(szFilePath);
	TSM_TASKLISTINFO storageInfo;

	TDC_PREPAREPATH nPathType = PrepareFilePath(sFilePath, &storageInfo);

	if (nPathType == TDCPP_NONE)
		return TDCF_NOTEXIST;

	// see if the tasklist is already open
	if (SelectToDoCtrl(sFilePath, TRUE))
		return TDCF_SUCCESS;

	// delay load the file, visible but disabled
	CFilteredToDoCtrl* pTDC = NewToDoCtrl(TRUE, FALSE);

	// if this is a 'special' temp file then assume TDL automatically
	// named it when handling WM_ENDSESSION. 
	BOOL bDelayLoad = !IsEndSessionFilePath(sFilePath);
	COleDateTime dtEarliest;

	if (bDelayLoad)
		bDelayLoad = pTDC->DelayLoad(sFilePath, dtEarliest);

	if (bDelayLoad)
	{
		// now we have to check for whether the tasklist has due tasks 
		// and the user wants notification
		int nNotifyDueBy = Prefs().GetNotifyDueByOnLoad();

		if (nNotifyDueBy != PFP_DONTNOTIFY && CDateHelper::IsDateSet(dtEarliest.m_dt))
		{
			// check the date against when the user wants notifying
			DH_DATE nDate = DHD_TODAY;
			
			switch (nNotifyDueBy)
			{
			case PFP_DUETODAY:		/*nDate = DHD_TODAY;*/		break;
			case PFP_DUETOMORROW:	nDate = DHD_TOMORROW;		break;
			case PFP_DUETHISWEEK:	nDate = DHD_ENDTHISWEEK;	break;
			case PFP_DUENEXTWEEK:	nDate = DHD_ENDNEXTWEEK;	break;
			case PFP_DUETHISMONTH:	nDate = DHD_ENDTHISMONTH;	break;
			case PFP_DUENEXTMONTH:	nDate = DHD_ENDNEXTMONTH;	break;
			default:				ASSERT (0);
			}
			
			COleDateTime dtDueWhen = CDateHelper::GetDate(nDate);
			
			bDelayLoad = (dtDueWhen < dtEarliest);
		}
	}

	// if the delay load failed for any reason we need to delete the tasklist
	// and fallback on the default load mechanism
	if (!bDelayLoad)
	{
		pTDC->DestroyWindow();
		delete pTDC;

		// note: we use the original filepath in case it is 
		// actually storage info
		return OpenTaskList(szFilePath, FALSE);
	}
	
	int nCtrl = m_mgrToDoCtrls.AddToDoCtrl(pTDC, &storageInfo, FALSE); // FALSE == not yet loaded
	
	// Update time tracking widget
	m_dlgTimeTracker.AddTasklist(pTDC);

	// update due item status
	if (CDateHelper::IsDateSet(dtEarliest))
	{
		TDCM_DUESTATUS nStatus = TDCM_FUTURE;
		COleDateTime dtToday = COleDateTime::GetCurrentTime();

		if (floor(dtEarliest) < floor(dtToday))
			nStatus = TDCM_PAST;

		else if (floor(dtEarliest) == floor(dtToday))
			nStatus = TDCM_TODAY;

		m_mgrToDoCtrls.SetDueItemStatus(nCtrl, nStatus);
	}
		
	return TDCF_SUCCESS;
}

CString CToDoListWnd::GetEndSessionFilePath()
{
	return FileMisc::GetTempFilePath(_T("tde"));
}

BOOL CToDoListWnd::IsEndSessionFilePath(const CString& sFilePath)
{
	if (!FileMisc::FileExists(sFilePath))
		return FALSE;

	if (!FileMisc::HasExtension(sFilePath, _T("tmp")))
		return FALSE;
	
	if (!FileMisc::IsTempFilePath(sFilePath))
		return FALSE;

	if (FileMisc::GetFileNameFromPath(sFilePath).Find(_T("tde")) != 0)
		return FALSE;

	// passed all the tests
	return TRUE;
}

BOOL CToDoListWnd::GetAutoArchiveOptions(TDC_ARCHIVE& nRemove, BOOL& bRemoveFlagged) const
{
	// defaults
	nRemove = TDC_REMOVENONE;
	bRemoveFlagged = FALSE;

	const CPreferencesDlg& userPrefs = Prefs();
	
	if (userPrefs.GetAutoArchive())
	{
		if (userPrefs.GetRemoveArchivedTasks())
		{
			if (userPrefs.GetRemoveOnlyOnAbsoluteCompletion())
				nRemove = TDC_REMOVEIFSIBLINGSANDSUBTASKSCOMPLETE;
			else
				nRemove = TDC_REMOVEALL;

			bRemoveFlagged = !userPrefs.GetDontRemoveFlagged();
		}
		
		return TRUE;
	}

	return FALSE;
}

TDC_FILE CToDoListWnd::OpenTaskList(LPCTSTR szFilePath, BOOL bNotifyDueTasks)
{
	CString sFilePath(szFilePath);

	TSM_TASKLISTINFO storageInfo;
	TDC_PREPAREPATH nType = PrepareFilePath(sFilePath, &storageInfo);
	
	if (nType == TDCPP_NONE)
		return TDCF_NOTEXIST;
	
	// see if the tasklist is already open
	int nExist = -1;
	
	if (nType == TDCPP_FILE)
		nExist = m_mgrToDoCtrls.FindToDoCtrl(sFilePath);
	else
		nExist = m_mgrToDoCtrls.FindToDoCtrl(storageInfo);
	
	// reload provided there are no existing changes
	// and the timestamp has changed
	if (nExist != -1)
	{
		if (!m_mgrToDoCtrls.IsModified(nExist) &&
			m_mgrToDoCtrls.RefreshFileLastModified(nExist))
		{
			ReloadTaskList(nExist, bNotifyDueTasks);
		}
		
		// then select
		if (SelectToDoCtrl(nExist, TRUE))
			return TDCF_SUCCESS;
	}
	
	// create a new todoltrl for this tasklist 
	const CPreferencesDlg& userPrefs = Prefs();
	CFilteredToDoCtrl* pTDC = NewToDoCtrl();
	CHoldRedraw hr(pTDC->GetSafeHwnd());
	
	// handles simple and storage tasklists
	// we use szFilePath because it may be storage Info not a true path
	TDC_FILE nOpen = OpenTaskList(pTDC, szFilePath, &storageInfo);
	
	if (nOpen == TDCF_SUCCESS)
	{
		int nTDC = AddToDoCtrl(pTDC, &storageInfo);

		// notify readonly
		m_mgrToDoCtrls.CheckNotifyReadonly(nTDC);

		// reload any reminders
		m_reminders.AddToDoCtrl(pTDC);
		
		// notify user of due tasks if req
		if (bNotifyDueTasks)
			DoDueTaskNotification(nTDC, userPrefs.GetNotifyDueByOnLoad());

		// check for automatic naming when handling WM_ENDSESSION. 
		// so we clear the filename and mark it as modified
		if (IsEndSessionFilePath(sFilePath))
		{
			pTDC->ClearFilePath();
			pTDC->SetModified();
		}
		
		UpdateCaption();
		UpdateStatusbar();
		OnTimerDueItems(nTDC);
		
		// update search
		if (userPrefs.GetRefreshFindOnLoad() && m_findDlg.GetSafeHwnd())
			m_findDlg.RefreshSearch();
	}
	else if (GetTDCCount() >= 1) // only delete if there's another ctrl existing
	{
		pTDC->DestroyWindow();
		delete pTDC;
	}
	else // re-add
	{
		AddToDoCtrl(pTDC);
	}
	
	return nOpen;
}

TDC_FILE CToDoListWnd::OpenTaskList(CFilteredToDoCtrl* pTDC, LPCTSTR szFilePath, TSM_TASKLISTINFO* pInfo)
{
	CString sFilePath(szFilePath);
	CTaskFile tasks;

	TSM_TASKLISTINFO storageInfo;
	TDC_PREPAREPATH nType = PrepareFilePath(sFilePath, &storageInfo);

	DOPROGRESS(IDS_LOADINGPROGRESS);

	switch (nType)
	{
	case TDCPP_STORAGE:
		{
			// storage may already have been loaded
			if (!pInfo || !pInfo->HasLocalFilePath())
			{
				CPreferences prefs;
				
				if (!m_mgrStorage.RetrieveTasklist(storageInfo, tasks, -1, prefs))
					return TDCF_CANCELLED;
			}
			else
				storageInfo = *pInfo;

			// else
			sFilePath = storageInfo.szLocalFileName;

			// must set this up before loading tasklist
			// so that pTDC can access correct prefs
			pTDC->SetAlternatePreferencesKey(storageInfo.szDisplayName);
		}
		break;
		
	case TDCPP_FILE:
		{
			// handle bad path
			if (sFilePath.IsEmpty())
			{
				if (!Misc::IsEmpty(szFilePath))
					return TDCF_NOTEXIST;
				else
					sFilePath = pTDC->GetFilePath(); // ie. reload
			}

			pTDC->SetStyle(TDCS_CHECKOUTONLOAD, Prefs().GetAutoCheckOut());
		}
		break;
		
	case TDCPP_NONE:
	default:
		ASSERT(0);
		break;
	}
	
	ASSERT(FileMisc::FileExists(sFilePath));

	BOOL bWasDelayed = pTDC->IsDelayLoaded();
	TDC_FILE nOpen = pTDC->Load(sFilePath, tasks);

	// cleanup temp storage file
	if (nType == TDCPP_STORAGE)
	{
		ASSERT(FileMisc::IsTempFilePath(sFilePath));
		FileMisc::DeleteFile(sFilePath, TRUE);
	}

	if (nOpen == TDCF_SUCCESS)
	{
		// update readonly status
		m_mgrToDoCtrls.UpdateToDoCtrlReadOnlyUIState(*pTDC);

		const CPreferencesDlg& userPrefs = Prefs();

		// certain operations cannot be performed on 'storage' tasklists
		if (nType == TDCPP_FILE)
		{
			// archive completed tasks?
			if (!pTDC->IsReadOnly())
			{
				BOOL bRemoveFlagged;
				TDC_ARCHIVE nRemove;
				
				if (GetAutoArchiveOptions(nRemove, bRemoveFlagged))
					pTDC->ArchiveDoneTasks(nRemove, bRemoveFlagged);
			}

			if (userPrefs.GetAddFilesToMRU())
				m_mruList.Add(sFilePath);
		}
		else // storage
		{
			ASSERT(nType == TDCPP_STORAGE);

			// return update storage info
			if (pInfo)
				*pInfo = storageInfo;
		}

		if (userPrefs.GetExpandTasksOnLoad())
			pTDC->ExpandTasks(TDCEC_ALL);
		
		// update find dialog with this ToDoCtrl's custom attributes
		UpdateFindDialogActiveTasklist(pTDC);

		// Update time tracking widget
		if (bWasDelayed)
			m_dlgTimeTracker.UpdateTasks(pTDC, tasks);
		else
			m_dlgTimeTracker.AddTasklist(pTDC, tasks);
	}
	else
	{
		pTDC->SetModified(FALSE);
	}

	return nOpen;
}

void CToDoListWnd::UpdateFindDialogActiveTasklist(const CFilteredToDoCtrl* pTDC)
{
	if (pTDC == NULL && GetTDCCount() == 0)
		return; // nothing to do

	CTDCCustomAttribDefinitionArray aTDCAttribDefs, aAllAttribDefs;

	// all tasklists
	int nTDC = GetTDCCount();

	while (nTDC--)
	{
		const CFilteredToDoCtrl& tdc = GetToDoCtrl(nTDC);
		tdc.GetCustomAttributeDefs(aTDCAttribDefs);

		CTDCCustomAttributeHelper::AppendUniqueAttributes(aTDCAttribDefs, aAllAttribDefs);
	}
	
	// active tasklist
	if (pTDC == NULL)
	{
		ASSERT(GetTDCCount() > 0);
		pTDC = &GetToDoCtrl();
	}

	ASSERT (pTDC);
	pTDC->GetCustomAttributeDefs(aTDCAttribDefs);

	// do the update
	m_findDlg.SetCustomAttributes(aTDCAttribDefs, aAllAttribDefs);
	m_findDlg.SetActiveTasklist(pTDC->GetFilePath(), Prefs().GetShowDefaultTaskIcons());
}

LRESULT CToDoListWnd::OnDoInitialDueTaskNotify(WPARAM /*wp*/, LPARAM /*lp*/)
{
	int nDueBy = Prefs().GetNotifyDueByOnLoad();
	
	if (nDueBy != PFP_DONTNOTIFY)
	{
		DOPROGRESS(IDS_DUETASKONLOADPROGRESS);
		
		UpdateWindow();
		m_tabCtrl.UpdateWindow();
		
		int nCtrls = GetTDCCount();
		
		for (int nCtrl = 0; nCtrl < nCtrls; nCtrl++)
		{
			if (m_mgrToDoCtrls.IsLoaded(nCtrl))
				DoDueTaskNotification(nCtrl, nDueBy);
		}
	}

	return 0L;
}

BOOL CToDoListWnd::DoDueTaskNotification(int nTDC, int nDueBy)
{
	if (m_bClosing)
		return FALSE;

	// check userPrefs
	if (nDueBy == -1)
		return TRUE; // nothing to do
	
	if (nTDC != -1 && !VerifyTaskListOpen(nTDC, FALSE))
		return TRUE; // no error. user cancelled

	DOPROGRESS(IDS_DUETASKONLOADPROGRESS);

	// prepare thread structure
	const CFilteredToDoCtrl& tdc = GetToDoCtrl(nTDC);
	const CPreferencesDlg& userPrefs = Prefs();
	
	// preferences
	BOOL bParentTitleCommentsOnly = userPrefs.GetExportParentTitleCommentsOnly();
	BOOL bDueTaskTitlesOnly = userPrefs.GetDueTaskTitlesOnly();
	BOOL bHtmlNotify = userPrefs.GetDisplayDueTasksInHtml();

	CString sStylesheet(userPrefs.GetDueTaskStylesheet());
	BOOL bTransform = GetStylesheetPath(tdc, sStylesheet);
	
	DWORD dwFlags = TDCGTF_FILENAME;
	
	if (userPrefs.GetDisplayDueCommentsInHtml())
		dwFlags |= TDCGTF_HTMLCOMMENTS;
	else
		dwFlags |= TDCGTF_TEXTCOMMENTS;

	if (bTransform)
		dwFlags |= TDCGTF_TRANSFORM;

	// due task notification preference overrides Export preference
	if (bDueTaskTitlesOnly)
		dwFlags |= TDCGTF_TITLESONLY;

	else if (bParentTitleCommentsOnly)
		dwFlags |= TDCGTF_PARENTTITLECOMMENTSONLY;

	TDC_GETTASKS nFilter = TDCGT_DUE;
	UINT nIDDueBy = IDS_DUETODAY;
	
	switch (nDueBy)
	{
	case PFP_DUETODAY:
		break; // done
		
	case PFP_DUETOMORROW:
		nIDDueBy = IDS_DUETOMORROW;
		nFilter = TDCGT_DUETOMORROW;
		break;
		
	case PFP_DUETHISWEEK:
		nIDDueBy = IDS_DUETHISWEEK;
		nFilter = TDCGT_DUETHISWEEK;
		break;
		
	case PFP_DUENEXTWEEK:
		nIDDueBy = IDS_DUENEXTWEEK;
		nFilter = TDCGT_DUENEXTWEEK;
		break;
		
	case PFP_DUETHISMONTH:
		nIDDueBy = IDS_DUETHISMONTH;
		nFilter = TDCGT_DUETHISMONTH;
		break;
		
	case PFP_DUENEXTMONTH:
		nIDDueBy = IDS_DUENEXTMONTH;
		nFilter = TDCGT_DUENEXTMONTH;
		break;
		
	default:
		ASSERT (0);
		return FALSE;
	}
	
	TDCGETTASKS filter(nFilter, dwFlags);
	filter.sAllocTo = userPrefs.GetDueTaskPerson();
	
	// visible attributes only
	TDC::MapColumnsToAttributes(tdc.GetVisibleColumns(), filter.mapAttribs);
			
	// prepare structure
	TDCDUETASKNOTIFY* pDDNotify = new TDCDUETASKNOTIFY(GetSafeHwnd(), tdc.GetFilePath(), bHtmlNotify);
	ASSERT(pDDNotify);
	
	// different file for each
	pDDNotify->sExportPath.Format(_T("ToDoList.due.%d"), nTDC);
	pDDNotify->sExportPath = FileMisc::GetTempFilePath(pDDNotify->sExportPath, (bHtmlNotify ? _T("html") : _T("txt")));

	if (!tdc.GetFilteredTasks(pDDNotify->tasks, filter))
	{
		// cleanup
		delete pDDNotify;
		return FALSE;
	}
	
	// set an appropriate title
	pDDNotify->tasks.SetReportAttributes(CEnString(nIDDueBy));

	if (bTransform)
		pDDNotify->sStylesheet = sStylesheet;

	// make sure import export manager is initialised 
	VERIFY(m_mgrImportExport.GetNumExporters());
	pDDNotify->pImpExpMgr = &m_mgrImportExport;

	// start thread paused
	ASSERT(pDDNotify && pDDNotify->IsValid());

	CWinThread* pThread = AfxBeginThread(DueTaskNotifyThreadProc, 
										(LPVOID)pDDNotify, 
										THREAD_PRIORITY_NORMAL, 
										0, 
										CREATE_SUSPENDED);
	ASSERT(pThread);

	if (pThread)
	{
		// Hack to prevent exporters adding space for notes
		if (s_bRestoreExportSpaceForNotes == -1)
		{
			s_bRestoreExportSpaceForNotes = userPrefs.GetExportSpaceForNotes();
			CPreferences().WriteProfileInt(PREF_KEY, _T("ExportSpaceForNotes"), FALSE);
		}

		if (pThread->ResumeThread() == 1)
		{
			m_nNumDueTaskThreads++;
			return TRUE;
		}

		// else
		pThread->Delete();
		// Fall thru
	}

	// else call thread proc directly
	return DueTaskNotifyThreadProc((LPVOID)pDDNotify);
}

LRESULT CToDoListWnd::OnDueTaskThreadFinished(WPARAM wp, LPARAM lp)
{
	ASSERT (s_bRestoreExportSpaceForNotes != -1);
	m_nNumDueTaskThreads--;

	// undo hack
	if (m_nNumDueTaskThreads == 0)
	{
		CPreferences().WriteProfileInt(PREF_KEY, _T("ExportSpaceForNotes"), s_bRestoreExportSpaceForNotes);
		s_bRestoreExportSpaceForNotes = -1;
	}

	TDCDUETASKNOTIFY* pDDNotify = (TDCDUETASKNOTIFY*)lp;
	ASSERT(pDDNotify && pDDNotify->IsValid());

	if (!m_bClosing)
	{
 		if (wp) // success
		{
			int nTDC = m_mgrToDoCtrls.FindToDoCtrl(pDDNotify->sTDCPath);

			if (nTDC != -1)
			{
				Show(FALSE);
				m_mgrToDoCtrls.ShowDueTaskNotification(nTDC, pDDNotify->sExportPath, pDDNotify->bHtml);
			}
		}
 	}

	// cleanup
	delete pDDNotify;

	return 0L;
}

UINT CToDoListWnd::DueTaskNotifyThreadProc(LPVOID pParam)
{
	TDCDUETASKNOTIFY* pDDNotify = (TDCDUETASKNOTIFY*)pParam;
	ASSERT(pDDNotify && pDDNotify->IsValid());
	
	// save intermediate tasklist to file as required
	LogIntermediateTaskList(pDDNotify->tasks, pDDNotify->sTDCPath);

	BOOL bSuccess = FALSE;

	if (pDDNotify->bHtml) // display in browser?
	{
		if (FileMisc::FileExists(pDDNotify->sStylesheet)) // bTransform
		{
			bSuccess = pDDNotify->tasks.TransformToFile(pDDNotify->sStylesheet, pDDNotify->sExportPath, SFEF_UTF8);
		}
		else
		{
			bSuccess = pDDNotify->pImpExpMgr->ExportTaskListToHtml(&pDDNotify->tasks, pDDNotify->sExportPath);
		}
	}
	else
	{
		bSuccess = pDDNotify->pImpExpMgr->ExportTaskListToText(&pDDNotify->tasks, pDDNotify->sExportPath);
	}
	
	// notify parent
	if (::IsWindow(pDDNotify->hWndNotify))
	{
		::PostMessage(pDDNotify->hWndNotify, WM_DUETASKTHREADFINISHED, bSuccess, (LPARAM)pDDNotify);
	}
	else
	{
		delete pDDNotify;
	}
	
	return bSuccess;
}

CString CToDoListWnd::GetTitle()
{
	static CString sTitle(_T("ToDoList 7.1.RC3 (Release Candidate)"));
	CLocalizer::IgnoreString(sTitle);

	return sTitle;
}

void CToDoListWnd::OnAbout() 
{
	CTDLAboutDlg dialog(GetTitle());
	
	dialog.DoModal();
}

void CToDoListWnd::OnPreferences() 
{
	DoPreferences();
}

void CToDoListWnd::DoPreferences(int nInitPage) 
{
	// take a copy of current userPrefs to check changes against
	const CPreferencesDlg oldPrefs; 
	
	// kill timers for the duration
	KillTimers();

	// restore translation of dynamic menu items shortcut prefs
	EnableDynamicMenuTranslation(TRUE);
	
	ASSERT(m_pPrefs);
	UINT nRet = m_pPrefs->DoModal(nInitPage);
	
	// re-disable dynamic menu translation
	EnableDynamicMenuTranslation(FALSE);
	
	// updates userPrefs
	RedrawWindow();
	ResetPrefs();

	const CPreferencesDlg& newPrefs = Prefs();
	
	if (nRet == IDOK)
	{
		// language changes may require restart so do that first
		if (UpdateLanguageTranslationAndCheckForRestart(oldPrefs))
		{
			DoExit(TRUE);
			return;
		}

		SetUITheme(newPrefs.GetUITheme());

		// mark all todoctrls as needing refreshing
		m_mgrToDoCtrls.SetAllNeedPreferenceUpdate(TRUE); 
		
		// delete fonts if they appear to have changed
		// and recreate in UpdateToDoCtrlPrefs
		CString sFaceName;
		int nFontSize;
		
		if (!newPrefs.GetTreeFont(sFaceName, nFontSize) || !GraphicsMisc::SameFont(m_fontTree, sFaceName, nFontSize))
			GraphicsMisc::VerifyDeleteObject(m_fontTree);
		
		if (!newPrefs.GetCommentsFont(sFaceName, nFontSize) || !GraphicsMisc::SameFont(m_fontComments, sFaceName, nFontSize))
			GraphicsMisc::VerifyDeleteObject(m_fontComments);
		
		// topmost
#ifndef _DEBUG
		BOOL bTopMost = (newPrefs.GetAlwaysOnTop() && !IsZoomed());
		SetWindowPos(bTopMost ? &wndTopMost : &wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
#endif
		
		// tray icon
		m_trayIcon.ShowTrayIcon(newPrefs.GetUseSysTray());
		
		// support for .tdl extension
		EnableTDLExtension(newPrefs.GetEnableTDLExtension(), FALSE);

		// support for tdl:// protocol
		EnableTDLProtocol(newPrefs.GetEnableTDLProtocol(), FALSE);

		// default task attributes
		newPrefs.GetDefaultTaskAttributes(m_tdiDefault);
		m_mgrImportExport.SetDefaultTaskAttributes(m_tdiDefault);

		// source control
		BOOL bAutoCheckOut = newPrefs.GetAutoCheckOut();
		BOOL bWantCheckOut = newPrefs.GetKeepTryingToCheckout();

		// update all open files to ensure they're in the right state
		int nCtrl = GetTDCCount();
			
		while (nCtrl--)
		{
			if (m_mgrToDoCtrls.IsSourceControlled(nCtrl))
			{
				if (bWantCheckOut && !m_mgrToDoCtrls.IsCheckedOut(nCtrl))
					m_mgrToDoCtrls.CheckOut(nCtrl);

				GetToDoCtrl(nCtrl).SetStyle(TDCS_CHECKOUTONLOAD, bAutoCheckOut);
			}
		}

		// menu icons
		UINT nPrevID = MapNewTaskPos(oldPrefs.GetNewTaskPos(), FALSE);
		m_mgrMenuIcons.ChangeImageID(nPrevID, GetNewTaskCmdID());

		nPrevID = MapNewTaskPos(oldPrefs.GetNewSubtaskPos(), TRUE);
		m_mgrMenuIcons.ChangeImageID(nPrevID, GetNewSubtaskCmdID());
		
		// reload menu 
		LoadMenubar();
		
		// tab bar
		if (newPrefs.GetShowTasklistTabCloseButton())
			m_tabCtrl.ModifyFlags(0, TCE_CLOSEBUTTON);
		else
			m_tabCtrl.ModifyFlags(TCE_CLOSEBUTTON, 0);

		BOOL bResizeDlg = (oldPrefs.GetAutoHideTabbar() != newPrefs.GetAutoHideTabbar());
		
		if (oldPrefs.GetStackTabbarItems() != newPrefs.GetStackTabbarItems())
		{
			BOOL bStackTabbar = newPrefs.GetStackTabbarItems();
			
			bResizeDlg = TRUE;
			m_tabCtrl.ModifyStyle(bStackTabbar ? 0 : TCS_MULTILINE, bStackTabbar ? TCS_MULTILINE : 0);
		}
		else
		{
			m_tabCtrl.Invalidate(); // handle priority colour changes
		}
			
		// visible filter controls
		if (m_bShowFilterBar)
			bResizeDlg = TRUE;

		BOOL bEnableMultiSel = newPrefs.GetMultiSelFilters();
		BOOL bPrevMultiSel = oldPrefs.GetMultiSelFilters();

		if (bPrevMultiSel != bEnableMultiSel)
		{
			m_filterBar.EnableMultiSelection(bEnableMultiSel);

			OnViewRefreshfilter();
		}

		m_filterBar.ShowDefaultFilters(newPrefs.GetShowDefaultFilters());

		// title filter option
		PUIP_MATCHTITLE nTitleOption = newPrefs.GetTitleFilterOption();
		PUIP_MATCHTITLE nPrevTitleOption = oldPrefs.GetTitleFilterOption();

		if (nPrevTitleOption != nTitleOption)
			OnViewRefreshfilter();

		// inherited parent task attributes for new tasks
		CTDCAttributeMap mapParentAttrib;
		BOOL bUpdateAttrib;

		newPrefs.GetParentAttribsUsed(mapParentAttrib, bUpdateAttrib);
		CFilteredToDoCtrl::SetInheritedParentAttributes(mapParentAttrib, bUpdateAttrib);
				
		// hotkey
		UpdateGlobalHotkey();
		
		// time periods
		CTimeHelper::SetHoursInWorkday(newPrefs.GetHoursInWorkday());
		CTimeHelper::SetWorkdaysInWeek(newPrefs.GetWorkdaysInWeek());
		CDateHelper::SetWeekendDays(newPrefs.GetWeekendDays());
		
		RefreshTabOrder();
		RefreshPauseTimeTracking();
		UpdateCaption();

		// colours
		if (m_findDlg.GetSafeHwnd())
			m_findDlg.RefreshUserPreferences();
		
		// active tasklist userPrefs
		UpdateActiveToDoCtrlPreferences();
		UpdateTimeTrackerPreferences();

		// then refresh filter bar for any new default cats, statuses, etc
		m_filterBar.RefreshFilterControls(GetToDoCtrl());
		
		if (bResizeDlg)
			Resize();

		// Stickies Support
		CString sStickiesPath;

		if (newPrefs.GetUseStickies(sStickiesPath))
			VERIFY(m_reminders.UseStickies(TRUE, sStickiesPath));
		else
			m_reminders.UseStickies(FALSE);

		// Content controls
		m_mgrContent.LoadPreferences(CPreferences(), _T("ContentControls"), TRUE);

		// Recently modified period
		CFilteredToDoCtrl::SetRecentlyModifiedPeriod(newPrefs.GetRecentlyModifiedPeriod());

		// don't ask me for the full details on this but it seems as
		// though the CSysImageList class is waiting to process a 
		// message before we can switch image sizes so we put it
		// right at the end after everything is done.
		Misc::ProcessMsgLoop();
		AppendTools2Toolbar(TRUE);
	}
	
	// finally set or terminate the various status check timers
	RestoreTimers();

}

BOOL CToDoListWnd::UpdateLanguageTranslationAndCheckForRestart(const CPreferencesDlg& oldPrefs)
{
	const CPreferencesDlg& prefs = Prefs();

	CString sLangFile = prefs.GetLanguageFile();
	BOOL bEnableRTL = prefs.GetEnableRTLInput();
	
	BOOL bDefLang = (CTDLLanguageComboBox::GetDefaultLanguage() == sLangFile);
		
	// language change requires a restart
	if (oldPrefs.GetLanguageFile() != sLangFile)
	{
		if (bDefLang || FileMisc::FileExists(sLangFile))
		{
			// if the language file exists and has changed then inform the user that they to restart
			// Note: restarting will also handle 'bAdd2Dict' and 'bEnableRTL'
			if (MessageBox(IDS_RESTARTTOCHANGELANGUAGE, 0, MB_YESNO) == IDYES)
			{
				return TRUE;
			}
		}
	}
	
	// RTL change requires a restart
	if (oldPrefs.GetEnableRTLInput() != bEnableRTL)
	{
		// if the language file exists and has changed then inform the user that they to restart
		// Note: restarting will also handle 'bAdd2Dict'
		if (MessageBox(IDS_RESTARTTOCHANGERTLINPUT, 0, MB_YESNO) == IDYES)
		{
			return TRUE;
		}
	}
	
	// no need to restart
	return FALSE;
}

void CToDoListWnd::UpdateMenuBackgroundColor()
{
	// set the menu background colour
	// Note: On XP and classic, the menu bar is not distinctive
	// so we set the menu color a little darker than 3DFACE
	if (!CThemed::IsAppThemed())
	{
		m_menubar.SetBackgroundColor(GraphicsMisc::Darker(GetSysColor(COLOR_3DFACE), 0.1));
	}
	else if (COSVersion() < OSV_VISTA)
	{
		if (m_theme.crMenuBack == m_theme.crAppBackLight)
		{
			if (m_theme.crAppBackDark != m_theme.crAppBackLight)
			{
				m_menubar.SetBackgroundColor(m_theme.crAppBackDark);
			}
			else
			{
				m_menubar.SetBackgroundColor(GraphicsMisc::Darker(m_theme.crAppBackLight, 0.1));
			}
		}
		else
		{
			m_menubar.SetBackgroundColor(m_theme.crMenuBack);
		}

	}
	else // Vista+ with themes
	{
		m_menubar.SetBackgroundColor(m_theme.crMenuBack);
	}
}

BOOL CToDoListWnd::LoadMenubar()
{
	m_menubar.DestroyMenu();
	
	if (!m_menubar.LoadMenu(IDR_MAINFRAME))
		return FALSE;

#ifdef _DEBUG
	m_menubar.ModifyMenu(AM_DEBUG, MF_BYPOSITION | MFT_RIGHTJUSTIFY, 0, _T("&Debug"));

	// don't translate the debug menu
	CLocalizer::EnableTranslation(::GetSubMenu(m_menubar, AM_DEBUG), FALSE);
#else
	m_menubar.DeleteMenu(AM_DEBUG, MF_BYPOSITION);
#endif

	SetMenu(&m_menubar);
	m_hMenuDefault = m_menubar;


	if (!Prefs().GetShowTasklistTabCloseButton()) 
		m_menubar.AddMDIButton(MEB_CLOSE, ID_CLOSE);
	
	UpdateMenuBackgroundColor();
	DrawMenuBar();

	// disable translation of dynamic menus
	EnableDynamicMenuTranslation(FALSE);

	// delete 'Record Bug Report' if below W7
	if (COSVersion() < OSV_WIN7)
	{
		CMenu* pSubMenu = m_menubar.GetSubMenu(AM_HELP);
		ASSERT(pSubMenu);
		
		int nPos = CEnMenu::GetMenuItemPos(*pSubMenu, ID_HELP_RECORDBUGREPORT);
		ASSERT(nPos != -1);
		
		pSubMenu->DeleteMenu(nPos, MF_BYPOSITION);
		
		// and the following separator
		if (pSubMenu->GetMenuItemID(nPos) == 0)
			pSubMenu->DeleteMenu(nPos, MF_BYPOSITION);
	}

	return TRUE;
}

void CToDoListWnd::EnableDynamicMenuTranslation(BOOL bEnable)
{
	CLocalizer::EnableTranslation(ID_FILE_MRU_FIRST, ID_FILE_MRU_LAST, bEnable);
	CLocalizer::EnableTranslation(ID_WINDOW1, ID_WINDOW16, bEnable);
	CLocalizer::EnableTranslation(ID_TOOLS_USERTOOL1, ID_TOOLS_USERTOOL16, bEnable);
	CLocalizer::EnableTranslation(ID_FILE_OPEN_USERSTORAGE1, ID_FILE_OPEN_USERSTORAGE16, bEnable);
	CLocalizer::EnableTranslation(ID_FILE_SAVE_USERSTORAGE1, ID_FILE_SAVE_USERSTORAGE16, bEnable);
	CLocalizer::EnableTranslation(ID_TRAYICON_SHOWDUETASKS1, ID_TRAYICON_SHOWDUETASKS20, bEnable);
}

void CToDoListWnd::UpdateGlobalHotkey()
{
	static DWORD dwPrevHotkey = 0;
	DWORD dwHotkey = Prefs().GetGlobalHotkey();
	
	if (dwPrevHotkey == dwHotkey)
		return;
	
	if (dwHotkey == 0) // disabled
	{
		::UnregisterHotKey(*this, 1);
	}
	else
	{
		// map modifiers to those wanted by RegisterHotKey
		DWORD dwPrefMods = HIWORD(dwHotkey);
		DWORD dwVKey = LOWORD(dwHotkey);
		DWORD dwMods = 0;
		
		dwMods |= ((dwPrefMods & HOTKEYF_ALT) ? MOD_ALT : 0);
		dwMods |= ((dwPrefMods & HOTKEYF_CONTROL) ? MOD_CONTROL : 0);
		dwMods |= ((dwPrefMods & HOTKEYF_SHIFT) ? MOD_SHIFT : 0);
		
		RegisterHotKey(*this, 1, dwMods, dwVKey);
	}

	dwPrevHotkey = dwHotkey;
}

void CToDoListWnd::RefreshPauseTimeTracking()
{
	BOOL bPauseAll = (((m_sessionWnd.IsLocked() || m_sessionWnd.IsScreenSaverActive()) && !Prefs().GetTrackOnScreenSaver()) || 
					  (m_sessionWnd.IsHibernated() && !Prefs().GetTrackHibernated()));

	BOOL bTrackActiveOnly = !Prefs().GetTrackNonActiveTasklists();
	int nCtrl = GetTDCCount();

	int nSel = GetSelToDoCtrl();
	ASSERT(nSel != -1);
	
	while (nCtrl--)
	{
		if (nCtrl == nSel)
			GetToDoCtrl(nCtrl).PauseTimeTracking(bPauseAll);
		else 
			GetToDoCtrl(nCtrl).PauseTimeTracking(bPauseAll || bTrackActiveOnly);
	}
}

BOOL CToDoListWnd::ProcessStartupOptions(const CTDCStartupOptions& startup, BOOL bStartup)
{
	// 1. check if we can handle a task link
	if (startup.HasFlag(TLD_TASKLINK))
	{
		CStringArray aFiles;

		if (startup.GetFilePaths(aFiles))
		{
			CString sPath;
			DWORD dwTaskID = 0;

			CFilteredToDoCtrl::ParseTaskLink(aFiles[0], _T(""), dwTaskID, sPath);
	
			if (sPath.IsEmpty() || ValidateTaskLinkFilePath(sPath))
			{
				return DoTaskLink(sPath, dwTaskID, bStartup);
			}
		}

		// else
		return FALSE;
	}

	// 2. execute a command
	if (startup.HasCommandID())
	{
		SendMessage(WM_COMMAND, MAKEWPARAM(startup.GetCommandID(), 0), 0);
		return TRUE;
	}

	// 3. try open/import file
	if (startup.HasFilePath())
	{
		int nFirstSel = -1;
		BOOL bSuccess = FALSE;

		CStringArray aFilePaths;
		int nNumFiles = startup.GetFilePaths(aFilePaths);

		for (int nFile = 0; nFile < nNumFiles; nFile++)
		{
			const CString& sFilePath = aFilePaths[nFile];
			
			if (startup.HasFlag(TLD_IMPORTFILE))
			{
				if (ImportFile(sFilePath, TRUE))
				{
					bSuccess = TRUE;
				}
			}
			else
			{
				BOOL bCanDelayLoad = Prefs().GetEnableDelayedLoading();

				// open the first tasklist fully and the rest delayed
				if (!bSuccess || !Prefs().GetEnableDelayedLoading())
				{
					if (OpenTaskList(sFilePath, FALSE) == TDCF_SUCCESS)
					{
						bSuccess = TRUE;
					}
				}
				else if (DelayOpenTaskList(sFilePath) == TDCF_SUCCESS)
				{
					bSuccess = TRUE;
				}
			}

			// snapshot the first success for subsequent selection
			if (bSuccess && (nFirstSel == -1))
				nFirstSel = GetSelToDoCtrl();
		}
		
		// exit on failure
		if (!bSuccess)
			return FALSE;

		// set selection to first tasklist loaded
		ASSERT((nFirstSel != -1) && (nFirstSel < GetTDCCount()));

		SelectToDoCtrl(nFirstSel, FALSE);
	}
	
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	BOOL bRes = FALSE;

	if (startup.HasFlag(TLD_SAVEINTERMEDIATE))
	{
		CString sOutputFile(GetIntermediateTaskListPath(GetToDoCtrl().GetFilePath()));

		CString sHtmlImgFolder(sOutputFile);
		FileMisc::ReplaceExtension(sHtmlImgFolder, _T("html_images"));

		CTaskFile tasks;

		if (startup.GetSaveIntermediateAll())
		{
			GetTasks(tdc, TRUE, TRUE, TSDT_ALL, TDCGETTASKS(), 0, tasks, sHtmlImgFolder);
		}
		else // use last state of transform dialog to determine what tasks to output
		{
			CTDLTransformDialog dialog(_T(""), tdc.GetView(), _T(""));
			GetTasks(tdc, TRUE, TRUE, dialog.GetTaskSelection(), tasks, sHtmlImgFolder);

			// add title and date 
			COleDateTime date;

			if (dialog.GetWantDate())
				date = COleDateTime::GetCurrentTime();

			tasks.SetReportAttributes(_T(""), date);
		}

		VERIFY(tasks.Save(sOutputFile, SFEF_UTF16));

		return TRUE;
	}
	else if (startup.HasFlag(TLD_NEWTASK))
	{
		CEnString sNewTask;
		BOOL bEditTask = FALSE;
		
		// we edit the task name if no name was supplied
		if (!startup.GetNewTaskName(sNewTask))
		{
			sNewTask.LoadString(IDS_TASK);
			bEditTask = TRUE;
		}
		
		// do we have a parent task ?
		if (tdc.SelectTask(startup.GetParentTaskID()))
		{
			bRes = CreateNewTask(sNewTask, TDC_INSERTATBOTTOMOFSELTASK, FALSE);
		}
		// or a sibling task ?
		else if (tdc.SelectTask(startup.GetSiblingTaskID()))
		{
			bRes = CreateNewTask(sNewTask, TDC_INSERTAFTERSELTASK, FALSE);
		}	
		else // default
		{
			bRes = CreateNewTask(sNewTask, TDC_INSERTATBOTTOM, FALSE);
		}
	
		// creation date
		COleDateTime date;

		if (startup.GetCreationDate(date))
			tdc.SetSelectedTaskDate(TDCD_CREATE, date);
		
		// edit task title?
		if (bRes && bEditTask)
			PostMessage(WM_COMMAND, ID_EDIT_TASKTEXT);
	}
	else if (startup.GetTaskID())
	{
		DWORD dwSelID = startup.GetTaskID();

		bRes = tdc.SelectTask(dwSelID);

		// perhaps the task is filtered out so we toggle 
		// the filter and try again
		if (!bRes && tdc.HasAnyFilter() && tdc.HasTask(dwSelID))
		{
			tdc.ToggleFilter();

			bRes = tdc.SelectTask(dwSelID);
			ASSERT(bRes);

			m_filterBar.RefreshFilterControls(tdc);
		}
	}
	else if (!startup.IsEmpty(TRUE))
	{
		// work on the currently selected item(s)
		bRes = (tdc.GetSelectedCount() > 0);
	}

	// rest of task attributes
	if (bRes)
	{
		CStringArray aItems;
		CString sItem;
		int nItem;
		double dItem;
		
		if (startup.GetComments(sItem))
		{
			// If it's a file, we first turn it into a URI
			WebMisc::FormatFileURI(sItem, sItem);

			// append comments
			CString sComments = tdc.GetSelectedTaskComments();

			if (!sComments.IsEmpty())
			{
				sComments += _T("\r\n");
				sComments += sItem;
				sItem = sComments;
			}
			tdc.SetSelectedTaskComments(sItem, _T(""));
		}

		if (startup.GetExternalID(sItem))
			tdc.SetSelectedTaskExtID(sItem);
		
		if (startup.GetVersion(sItem))
			tdc.SetSelectedTaskVersion(sItem);
		
		if (startup.GetAllocBy(sItem))
			tdc.SetSelectedTaskAllocBy(sItem);
		
		if (startup.GetStatus(sItem))
			tdc.SetSelectedTaskStatus(sItem);
		
		// Numeric items
		BOOL bOffset = FALSE;

		if (startup.GetPriority(nItem, bOffset))
		{
			if (bOffset)
				nItem += tdc.GetSelectedTaskPriority();

			tdc.SetSelectedTaskPriority(nItem);
		}

		if (startup.GetRisk(nItem, bOffset))
		{
			if (bOffset)
				nItem += tdc.GetSelectedTaskRisk();

			tdc.SetSelectedTaskRisk(nItem);
		}

		if (startup.GetPercentDone(nItem, bOffset))
		{
			if (bOffset)
				nItem += tdc.GetSelectedTaskPercent();

			tdc.SetSelectedTaskPercentDone(nItem);
		}

		if (startup.GetCost(dItem, bOffset))
		{
			if (bOffset)
				dItem += tdc.GetSelectedTaskCost();

			tdc.SetSelectedTaskCost(dItem);
		}

		// Times
		TDC_UNITS nUnits, nCurUnits;
		CTimeHelper th;

		if (startup.GetTimeEst(dItem, nUnits, bOffset))
		{
			double dTime = tdc.GetSelectedTaskTimeEstimate(nCurUnits);

			if (dTime != 0.0)
			{
				dItem = th.GetTime(dItem, TDC::MapUnitsToTHUnits(nUnits), TDC::MapUnitsToTHUnits(nCurUnits));
				nUnits = nCurUnits;
				
				if (bOffset)
					dItem += dTime;
			}

			tdc.SetSelectedTaskTimeEstimate(dItem, nUnits);
		}

		if (startup.GetTimeSpent(dItem, nUnits, bOffset))
		{
			double dTime = tdc.GetSelectedTaskTimeSpent(nCurUnits);

			if (dTime != 0.0)
			{
				dItem = th.GetTime(dItem, TDC::MapUnitsToTHUnits(nUnits), TDC::MapUnitsToTHUnits(nCurUnits));
				nUnits = nCurUnits;

				if (bOffset)
					dItem += dTime;
			}

			tdc.SetSelectedTaskTimeSpent(dItem, nUnits); 
		}

		// Multi-string items
		BOOL bAppend = FALSE;

		if (startup.GetAllocTo(aItems, bAppend) != -1)
		{
			if (bAppend)
				tdc.AppendSelectedTaskAllocTo(aItems);
			else
				tdc.SetSelectedTaskAllocTo(aItems);
		}
		
		if (startup.GetCategories(aItems, bAppend) != -1)
		{
			if (bAppend)
				tdc.AppendSelectedTaskCategories(aItems);
			else
				tdc.SetSelectedTaskCategories(aItems);
		}
		
		if (startup.GetDependencies(aItems, bAppend) != -1)
		{
			if (bAppend)
				tdc.AppendSelectedTaskDependencies(aItems);
			else
				tdc.SetSelectedTaskDependencies(aItems);
		}
		
		if (startup.GetTags(aItems, bAppend) != -1)
		{
			if (bAppend)
				tdc.AppendSelectedTaskTags(aItems);
			else
				tdc.SetSelectedTaskTags(aItems);
		}
		
		if (startup.GetFileRefs(aItems, bAppend) != -1)
		{
			if (bAppend)
				tdc.AppendSelectedTaskFileRefs(aItems);
			else
				tdc.SetSelectedTaskFileRefs(aItems);
		}
		
		// start date and time
		if (startup.GetStartDate(dItem, nUnits, bOffset))
		{
			if (bOffset)
			{
				TDC_OFFSET nOffset = TDC::MapUnitsToDateOffset(nUnits);
				tdc.OffsetSelectedTaskDate(TDCD_START, (int)dItem, nOffset, FALSE);
			}
			else
			{
				ASSERT(nUnits == DHU_DAYS);
				tdc.SetSelectedTaskDate(TDCD_START, dItem);
			}
		}
			
		// time overrides if present
		if (startup.GetStartTime(dItem, bOffset))
		{
			if (bOffset)
				dItem += tdc.GetSelectedTaskDate(TDCD_STARTTIME);

			tdc.SetSelectedTaskDate(TDCD_STARTTIME, dItem);
		}

		// due date and time
		if (startup.GetDueDate(dItem, nUnits, bOffset))
		{
			if (bOffset)
			{
				TDC_OFFSET nOffset = TDC::MapUnitsToDateOffset(nUnits);
				tdc.OffsetSelectedTaskDate(TDCD_DUE, (int)dItem, nOffset, FALSE);
			}
			else
			{
				ASSERT(nUnits == DHU_DAYS);
				tdc.SetSelectedTaskDate(TDCD_DUE, dItem);
			}
		}

		// time overrides if present
		if (startup.GetDueTime(dItem, bOffset))
		{
			if (bOffset)
				dItem += tdc.GetSelectedTaskDate(TDCD_DUETIME);

			tdc.SetSelectedTaskDate(TDCD_DUETIME, dItem);
		}

		// done date and time
		if (startup.GetDoneDate(dItem, nUnits, bOffset))
		{
			if (bOffset)
			{
				TDC_OFFSET nOffset = TDC::MapUnitsToDateOffset(nUnits);
				tdc.OffsetSelectedTaskDate(TDCD_DONE, (int)dItem, nOffset, FALSE);
			}
			else
			{
				ASSERT(nUnits == DHU_DAYS);
				tdc.SetSelectedTaskDate(TDCD_DONE, dItem);
			}
		}
			
		// time overrides if present
		if (startup.GetDoneTime(dItem, bOffset))
		{
			if (bOffset)
				dItem += tdc.GetSelectedTaskDate(TDCD_DONETIME);

			tdc.SetSelectedTaskDate(TDCD_DONETIME, dItem);
		}

		// Custom attribute
		CString sAttribID;

		if (startup.GetCustomAttribute(sAttribID, sItem))
			tdc.SetSelectedTaskCustomAttributeData(sAttribID, sItem);
	}

	return bRes;
}

BOOL CToDoListWnd::OnCopyData(CWnd* /*pWnd*/, COPYDATASTRUCT* pCopyDataStruct)
{
	BOOL bRes = FALSE;

	switch (pCopyDataStruct->dwData)
	{
	case TDLCD_FINDFILE:
		if (pCopyDataStruct->cbData != sizeof(CTDCStartupOptions))
		{
			ASSERT(0);
		}
		else
		{
			const CTDCStartupOptions* pStartup = (CTDCStartupOptions*)(pCopyDataStruct->lpData);
			
			if (pStartup)
			{
				CStringArray aFiles;
				
				// we only support finding a single filename
				if (pStartup->GetFilePaths(aFiles) == 1)
				{
					bRes = (m_mgrToDoCtrls.FindToDoCtrl(aFiles[0]) != -1);
				}
			}
		}
		break;

	case TDLCD_STARTUP:
		if (pCopyDataStruct->cbData != sizeof(CTDCStartupOptions))
		{
			ASSERT(0);
		}
		else
		{
			const CTDCStartupOptions* pStartup = (CTDCStartupOptions*)(pCopyDataStruct->lpData);

			if (pStartup)
				bRes = ProcessStartupOptions(*pStartup, FALSE);
		}
		break;
	}

	return bRes; 
}

BOOL CToDoListWnd::ImportFile(LPCTSTR szFilePath, BOOL bSilent)
{
	int nImporter = m_mgrImportExport.FindImporter(szFilePath);

	if (nImporter == -1)
		return FALSE;

	CTaskFile tasks;
	m_mgrImportExport.ImportTaskList(szFilePath, &tasks, nImporter, bSilent);
		
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
		
	if (tdc.InsertTasks(tasks, TDC_INSERTATTOP, FALSE))
		UpdateCaption();

	return TRUE;
}

void CToDoListWnd::OnEditCopy() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	tdc.Flush();
	tdc.CopySelectedTask();
}

void CToDoListWnd::OnEditCut() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	tdc.Flush();
	tdc.CutSelectedTask();
}

void CToDoListWnd::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	int nSelCount = tdc.GetSelectedCount();
	
	pCmdUI->Enable(!tdc.IsReadOnly() && nSelCount);	
}

void CToDoListWnd::OnEditPasteSub() 
{
	CWaitCursor wait;
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	if (tdc.CanPasteTasks(TDCP_ONSELTASK, FALSE))
	{
		tdc.PasteTasks(TDCP_ONSELTASK, FALSE);
	}
	else
	{
		DoPasteFromClipboard(TDIT_ONSELECTEDTASK);
	}

	UpdateTimeTrackerTasks(tdc, FALSE);
}

void CToDoListWnd::OnUpdateEditPasteSub(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	pCmdUI->Enable(tdc.CanPasteTasks(TDCP_ONSELTASK, FALSE) || 
					Misc::ClipboardHasText());	
}

BOOL CToDoListWnd::DoPasteFromClipboard(TDLID_IMPORTTO nWhere)
{
	if (!Misc::ClipboardHasText())
		return FALSE;

	CTDLPasteFromClipboardDlg dialog(m_mgrImportExport);

	if (dialog.DoModal() != IDOK)
		return FALSE;
	
	BOOL bFromClipboard = TRUE;
	int nImporter = dialog.GetImporterIndex();
	CString sImportFrom = dialog.GetClipboardText();

	return ImportTasks(bFromClipboard, sImportFrom, nImporter, nWhere);
}

void CToDoListWnd::OnEditPasteAfter() 
{
	CWaitCursor wait;

	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	int nSelCount = tdc.GetSelectedCount();

	TDC_PASTE nWhere = ((nSelCount == 0) ? TDCP_ATBOTTOM : TDCP_BELOWSELTASK);

	if (tdc.CanPasteTasks(nWhere, FALSE))
	{
		tdc.PasteTasks(nWhere, FALSE);
	}
	else if (Misc::ClipboardHasText())
	{
		DoPasteFromClipboard(TDIT_BELOWSELECTEDTASK);
	}

	UpdateTimeTrackerTasks(tdc, FALSE);
}

void CToDoListWnd::OnUpdateEditPasteAfter(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	int nSelCount = tdc.GetSelectedCount();
	
	// modify the text appropriately if the tasklist is empty
	if (nSelCount == 0)
		pCmdUI->SetText(CEnString(IDS_PASTETOPLEVELTASK));
	
	pCmdUI->Enable(tdc.CanPasteTasks(((nSelCount == 0) ? TDCP_ATBOTTOM : TDCP_BELOWSELTASK), FALSE) || 
					Misc::ClipboardHasText());	
}

void CToDoListWnd::OnEditPasteAsRef() 
{
	CWaitCursor wait;
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	tdc.PasteTasks(TDCP_ONSELTASK, TRUE);
}

void CToDoListWnd::OnUpdateEditPasteAsRef(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	pCmdUI->Enable(tdc.CanPasteTasks(TDCP_ONSELTASK, TRUE));
}

void CToDoListWnd::OnEditCopyastext() 
{
	CopySelectedTasksToClipboard(TDCTC_ASTEXT);
}

void CToDoListWnd::OnEditCopyashtml() 
{
	CopySelectedTasksToClipboard(TDCTC_ASHTML);
}

void CToDoListWnd::CopySelectedTasksToClipboard(TDC_TASKS2CLIPBOARD nAsFormat)
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	tdc.Flush();
	
	BOOL bParentTitleCommentsOnly = Prefs().GetExportParentTitleCommentsOnly();
	DWORD dwFlags = (bParentTitleCommentsOnly ? TDCGTF_PARENTTITLECOMMENTSONLY : 0);
	
	CTaskFile tasks;
	CString sTasks;
	tdc.GetSelectedTasks(tasks, TDCGETTASKS(TDCGT_ALL, dwFlags));
	
	switch (nAsFormat)
	{	
	case TDCTC_ASHTML:
		sTasks = m_mgrImportExport.ExportTaskListToHtml(&tasks);
		break;
		
	case TDCTC_ASTEXT:
		sTasks = m_mgrImportExport.ExportTaskListToText(&tasks);
		break;
		
	case TDCTC_ASLINK:
		sTasks.Format(_T("tdl://%lu"), tdc.GetSelectedTaskID());
		break;
		
	case TDCTC_ASDEPENDS:
		sTasks.Format(_T("%lu"), tdc.GetSelectedTaskID());
		break;
		
	case TDCTC_ASLINKFULL:
		sTasks.Format(_T("tdl://%s?%lu"), 
						tdc.GetFilePath(),
						tdc.GetSelectedTaskID());
		sTasks.Replace(_T(" "), _T("%20"));
		break;
		
	case TDCTC_ASDEPENDSFULL:
		sTasks.Format(_T("%s?%lu"), 
						tdc.GetFilePath(),
						tdc.GetSelectedTaskID());
		break;

	case TDCTC_ASPATH:
		sTasks = tdc.GetSelectedTaskPath(TRUE);
		break;
	}

	CClipboard(*this).SetText(sTasks);
}

void CToDoListWnd::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().GetSelectedCount() > 0);
}

BOOL CToDoListWnd::CanCreateNewTask(TDC_INSERTWHERE nInsertWhere) const
{
	return GetToDoCtrl().CanCreateNewTask(nInsertWhere);
}

void CToDoListWnd::OnUpdateNewtaskAttopSelected(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CanCreateNewTask(TDC_INSERTATTOPOFSELTASKPARENT));
}

void CToDoListWnd::OnUpdateNewtaskAtbottomSelected(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CanCreateNewTask(TDC_INSERTATBOTTOMOFSELTASKPARENT));
}

void CToDoListWnd::OnUpdateNewtaskAfterselectedtask(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CanCreateNewTask(TDC_INSERTAFTERSELTASK));
}

void CToDoListWnd::OnUpdateNewDependenttaskAfterselectedtask(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CanCreateNewTask(TDC_INSERTAFTERSELTASK));
}

void CToDoListWnd::OnUpdateNewtaskBeforeselectedtask(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CanCreateNewTask(TDC_INSERTBEFORESELTASK));
}

void CToDoListWnd::OnUpdateNewsubtaskAttop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CanCreateNewTask(TDC_INSERTATTOPOFSELTASK));
}

void CToDoListWnd::OnUpdateNewsubtaskAtBottom(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CanCreateNewTask(TDC_INSERTATBOTTOMOFSELTASK));
}

void CToDoListWnd::OnUpdateNewtaskAttop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CanCreateNewTask(TDC_INSERTATTOP));
}

void CToDoListWnd::OnUpdateNewtaskAtbottom(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CanCreateNewTask(TDC_INSERTATBOTTOM));
}

void CToDoListWnd::OnMaximizeTasklist() 
{
	// toggle max state on or off
	switch (m_nMaxState)
	{
	case TDCMS_MAXTASKLIST:
		// turn off maximize tasklist by restoring previous max state
		m_nMaxState = m_nPrevMaxState;
		m_nPrevMaxState = TDCMS_NORMAL; // reset
		break;

	case TDCMS_MAXCOMMENTS:
		// turn on maximize tasklist and save previous max state
		m_nMaxState = TDCMS_MAXTASKLIST;
		m_nPrevMaxState = TDCMS_MAXCOMMENTS;
		break;

	case TDCMS_NORMAL:
		// turn on maximize tasklist
		m_nMaxState = TDCMS_MAXTASKLIST;
		m_nPrevMaxState = TDCMS_NORMAL; // reset
		break;
	}
	
	// update active tasklist
	GetToDoCtrl().SetMaximizeState(m_nMaxState);
	Invalidate();

	// and caption
	UpdateCaption();
}

void CToDoListWnd::OnUpdateMaximizeTasklist(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nMaxState == TDCMS_MAXTASKLIST ? 1 : 0);
}

void CToDoListWnd::OnMaximizeComments() 
{
	// toggle max state on or off
	switch (m_nMaxState)
	{
	case TDCMS_MAXCOMMENTS:
		// toggle off maximize comments by restoring previous max state
		m_nMaxState = m_nPrevMaxState;
		m_nPrevMaxState = TDCMS_NORMAL; // reset
		break;

	case TDCMS_MAXTASKLIST:
		// turn on maximize comments and save previous max state
		m_nMaxState = TDCMS_MAXCOMMENTS;
		m_nPrevMaxState = TDCMS_MAXTASKLIST;
		break;

	case TDCMS_NORMAL:
		// turn on maximize comments
		m_nMaxState = TDCMS_MAXCOMMENTS;
		m_nPrevMaxState = TDCMS_NORMAL; // reset
		break;
	}
	
	// update active tasklist
	GetToDoCtrl().SetMaximizeState(m_nMaxState);
	Invalidate();

	// and caption
	UpdateCaption();
}

void CToDoListWnd::OnUpdateMaximizeComments(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nMaxState == TDCMS_MAXCOMMENTS ? 1 : 0);
}

void CToDoListWnd::OnReload() 
{
	int nSel = GetSelToDoCtrl();
	
	if (m_mgrToDoCtrls.IsModified(nSel))
	{ 
		if (IDYES != MessageBox(IDS_CONFIRMRELOAD, IDS_CONFIRMRELOAD_TITLE, MB_YESNOCANCEL | MB_DEFBUTTON2))
		{
			return;
		}
	}
	
	// else reload
	ReloadTaskList(nSel);
	RefreshTabOrder();
}

BOOL CToDoListWnd::ReloadTaskList(int nIndex, BOOL bNotifyDueTasks, BOOL bNotifyError)
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl(nIndex);
	CString sFilePath = tdc.GetFilePath();

	// Remove the tasklist from the time tracker because
	// OpenTasklist will correctly want to re-add it
	m_dlgTimeTracker.RemoveTasklist(&tdc);
	
	TDC_FILE nRes = OpenTaskList(&tdc, sFilePath);
	
	if (nRes == TDCF_SUCCESS)
	{
		const CPreferencesDlg& userPrefs = Prefs();
		
		// update file status
		m_mgrToDoCtrls.RefreshReadOnlyStatus(nIndex);
		m_mgrToDoCtrls.RefreshLastCheckoutStatus(nIndex);
		m_mgrToDoCtrls.RefreshFileLastModified(nIndex);
		m_mgrToDoCtrls.UpdateTabItemText(nIndex);
		
		// notify user of due tasks if req
		if (bNotifyDueTasks)
			DoDueTaskNotification(nIndex, userPrefs.GetNotifyDueByOnLoad());
		
		UpdateCaption();
		UpdateStatusbar();
	}
	else if (bNotifyError)
	{
		HandleLoadTasklistError(nRes, sFilePath);
	}

	return (nRes == TDCF_SUCCESS);
}

void CToDoListWnd::OnUpdateReload(CCmdUI* pCmdUI) 
{
	int nSel = GetSelToDoCtrl();
	
	pCmdUI->Enable(!m_mgrToDoCtrls.GetFilePath(nSel).IsEmpty());
}

void CToDoListWnd::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// ensure m_cbQuickFind is positioned correctly
	if (m_toolbar.GetSafeHwnd())
	{
		int nPos = m_toolbar.CommandToIndex(ID_EDIT_FINDTASKS) + 2;

		CRect rNewPos;
		m_toolbar.GetItemRect(nPos, rNewPos);
		m_toolbar.ClientToScreen(rNewPos);

		// check if it needs to be moved
		CRect rPrevPos;
		m_cbQuickFind.CWnd::GetWindowRect(rPrevPos);

		if (rNewPos.TopLeft() != rPrevPos.TopLeft())
		{
			m_toolbar.ScreenToClient(rNewPos);
			rNewPos.bottom = rNewPos.top + 200;
			m_cbQuickFind.MoveWindow(rNewPos);
		}

		// topmost?
		BOOL bMaximized = (nType == SIZE_MAXIMIZED);
		
		if (nType != SIZE_MINIMIZED)
			Resize(cx, cy, bMaximized);
		
		// if not maximized then set topmost if that's the preference
		// do nothing if no change
#ifndef _DEBUG
		BOOL bTopMost = (Prefs().GetAlwaysOnTop() && !bMaximized) ? 1 : 0;
		BOOL bIsTopMost = (GetExStyle() & WS_EX_TOPMOST) ? 1 : 0;
		
		if (bTopMost != bIsTopMost)
			SetWindowPos(bTopMost ? &wndTopMost : &wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
#endif
	}
}

BOOL CToDoListWnd::CalcToDoCtrlRect(CRect& rect, int cx, int cy, BOOL bMaximized)
{
	if (!cx && !cy)
	{
		CRect rClient;
		GetClientRect(rClient);
		
		cx = rClient.right;
		cy = rClient.bottom;
		bMaximized = IsZoomed();
		
		// check again 
		if (!cx && !cy)
			return FALSE;
	}
	
	CRect rTaskList(0, BEVEL, cx - BEVEL, cy);
	
	// toolbar
	if (m_bShowToolbar) 
 		rTaskList.top += m_toolbar.GetHeight();
	
	// resize tabctrl
	CDeferWndMove dwm(0); // dummy
	
	CPoint ptOrg(0, rTaskList.top);
	int nTabHeight = ReposTabBar(dwm, ptOrg, cx, TRUE);
	
	if (nTabHeight)
		rTaskList.top += nTabHeight + 1; // hide the bottom of the tab ctrl
	
	// filter controls
	int nInset = (CThemed().IsNonClientThemed() ? BORDER : BEVEL);
	int nFilterWidth = cx - 2 * nInset;
	int nFilterHeight = m_bShowFilterBar ? m_filterBar.CalcHeight(nFilterWidth) : 0;
	
	if (nFilterHeight)
		rTaskList.top += nFilterHeight;// + 4;
	
	// statusbar
	if (m_bShowStatusBar)
	{
		CRect rStatus;
		m_statusBar.GetWindowRect(rStatus);
		ScreenToClient(rStatus);
		rTaskList.bottom = rStatus.top - BORDER;
	}
	else
	{
		rTaskList.bottom = cy - BORDER;
	}
	
	// shrink slightly so that edit controls do not merge with window border
	rTaskList.DeflateRect(nInset, nInset, nInset, nInset);
	rect = rTaskList;
	
	return TRUE;
}

void CToDoListWnd::Resize(int cx, int cy, BOOL bMaximized)
{
	static int nLastCx = 0, nLastCy = 0;

	if (!cx && !cy)
	{
		CRect rClient;
		GetClientRect(rClient);
		
		cx = rClient.right;
		cy = rClient.bottom;
		bMaximized = IsZoomed();
		
		// check again 
		if (!cx && !cy)
			return;
	}

	// don't resize if there's no change and no tasklists
	if ((cx == nLastCx) && (cy == nLastCy) && !GetTDCCount())
		return;

	nLastCx = cx;
	nLastCy = cy;
	
	// resize in one go
	CDlgUnits dlu(this);
	CDeferWndMove dwm(6);
	CRect rTaskList(0, BEVEL, cx - BEVEL, cy);
	
	// toolbar
	if (m_bShowToolbar) // showing toolbar
		rTaskList.top += m_toolbar.Resize(cx);
	
	// resize tabctrl
	CPoint ptOrg(0, rTaskList.top);
	int nTabHeight = ReposTabBar(dwm, ptOrg, cx);
	
	if (nTabHeight)
		rTaskList.top += nTabHeight + 1; // hide the bottom of the tab ctrl
	
	// filter controls
	int nInset = (CThemed().IsNonClientThemed() ? BORDER : BEVEL);
	int nFilterWidth = cx - 2 * nInset;
	int nFilterHeight = m_bShowFilterBar ? m_filterBar.CalcHeight(nFilterWidth) : 0;
	
	dwm.MoveWindow(&m_filterBar, nInset, rTaskList.top, nFilterWidth, nFilterHeight);
	
	if (nFilterHeight)
		rTaskList.top += nFilterHeight;// + 4;
	
	// statusbar has already been automatically resized unless it's invisible
	CRect rStatus(0, cy, cx, cy);

	if (m_bShowStatusBar)
	{
		m_statusBar.GetWindowRect(rStatus);
		ScreenToClient(rStatus);
	}
	else
	{
		dwm.MoveWindow(&m_statusBar, rStatus, FALSE);
	}
	
	// finally the active todoctrl
	if (GetTDCCount())
	{
		if (m_bShowStatusBar)
			rTaskList.bottom = rStatus.top - BORDER;
		else
			rTaskList.bottom = rStatus.bottom - BORDER;
		
		// shrink slightly so that edit controls do not merge with window border
		rTaskList.DeflateRect(nInset, nInset, nInset, nInset);

		dwm.MoveWindow(&GetToDoCtrl(), rTaskList);

#ifdef _DEBUG
		CRect rect;
		CalcToDoCtrlRect(rect, cx, cy, IsZoomed());
		ASSERT(rect == rTaskList);
#endif

	}
}

BOOL CToDoListWnd::WantTasklistTabbarVisible() const 
{ 
	BOOL bWantTabbar = (GetTDCCount() > 1 || !Prefs().GetAutoHideTabbar()); 
	bWantTabbar &= m_bShowTasklistBar;

	return bWantTabbar;
}

int CToDoListWnd::ReposTabBar(CDeferWndMove& dwm, const CPoint& ptOrg, int nWidth, BOOL bCalcOnly)
{
	CRect rTabs(0, 0, nWidth, 0);
	m_tabCtrl.AdjustRect(TRUE, rTabs);
	int nTabHeight = rTabs.Height() - 4;
	
	rTabs = dwm.OffsetCtrl(this, IDC_TABCONTROL); // not actually a move
	rTabs.right = nWidth + 1;
	rTabs.bottom = rTabs.top + nTabHeight;
	rTabs.OffsetRect(0, ptOrg.y - rTabs.top); // add a pixel between tabbar and toolbar
	
	BOOL bNeedTabCtrl = WantTasklistTabbarVisible();
	
	if (!bCalcOnly)
	{
		dwm.MoveWindow(&m_tabCtrl, rTabs);
		
		// hide and disable tabctrl if not needed
		m_tabCtrl.ShowWindow(bNeedTabCtrl ? SW_SHOW : SW_HIDE);
		m_tabCtrl.EnableWindow(bNeedTabCtrl);
	}
	
	return bNeedTabCtrl ? rTabs.Height() : 0;
}

void CToDoListWnd::OnPrint() 
{
	DoPrint();
}

void CToDoListWnd::DoPrint(BOOL bPreview)
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	int nSelTDC = GetSelToDoCtrl();

	// pass the project name as the title field
	CString sTitle = m_mgrToDoCtrls.GetFriendlyProjectName(nSelTDC);

	// export to html and then print in IE
	CString sStylesheet(tdc.GetStylesheetPath());
	CTDLPrintDialog dialog(sTitle, bPreview, tdc.GetView(), sStylesheet);
	
	if (dialog.DoModal() != IDOK)
		return;

	RedrawWindow();
	
	// always use the same file
	CString sTempFile = FileMisc::GetTempFilePath(_T("ToDoList.print"), _T("html"));
	
	sTitle = dialog.GetTitle();
	BOOL bTransform = dialog.GetStylesheet(sStylesheet);
	
	// export
	DOPROGRESS(bPreview ? IDS_PPREVIEWPROGRESS : IDS_PRINTPROGRESS);

	CTaskFile tasks;
	GetTasks(tdc, TRUE, bTransform, dialog.GetTaskSelection(), tasks, NULL);

	// add title and date, and style 
	COleDateTime date;

	if (dialog.GetWantDate())
		date = COleDateTime::GetCurrentTime();

	tasks.SetReportAttributes(sTitle, date);

	// add export style
	if (!bTransform)
	{
		TDLPD_STYLE nStyle = dialog.GetExportStyle();
		tasks.SetMetaData(TDL_EXPORTSTYLE, Misc::Format(nStyle));
	}
	
	// save intermediate tasklist to file as required
	LogIntermediateTaskList(tasks, tdc.GetFilePath());

	if (!Export2Html(tasks, sTempFile, sStylesheet))
		return;
	
	// print from browser
	CRect rHidden(-20, -20, -10, -10); // create IE off screen
	
	if (m_IE.GetSafeHwnd() || m_IE.Create(NULL, WS_CHILD | WS_VISIBLE, rHidden, this, (UINT)IDC_STATIC))
	{
		double dFileSize = FileMisc::GetFileSize(sTempFile);
		BOOL bPrintBkgnd = Prefs().GetColorTaskBackground();

		if (bPreview)
			m_IE.PrintPreview(sTempFile, bPrintBkgnd);
		else
			m_IE.Print(sTempFile, bPrintBkgnd);
	}
	else // try sending to browser
	{
		DWORD dwRes = FileMisc::Run(*this, sTempFile, NULL, SW_HIDE, NULL, bPreview ? _T("print") : NULL);
								
		if (dwRes < 32)
			MessageBox(IDS_PRINTFAILED, IDS_PRINTFAILED_TITLE, MB_OK);
	}
}

void CToDoListWnd::OnUpdatePrint(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().GetTaskCount());
}

int CToDoListWnd::AddToDoCtrl(CFilteredToDoCtrl* pTDC, TSM_TASKLISTINFO* pInfo, BOOL bResizeDlg)
{
	// add tdc first to ensure tab controls has some
	// items before we query it for its size
	int nSel = m_mgrToDoCtrls.AddToDoCtrl(pTDC, pInfo);
	
	// make sure size is right
	CRect rTDC;
	
	if (CalcToDoCtrlRect(rTDC))
		pTDC->MoveWindow(rTDC);
	
	SelectToDoCtrl(nSel, FALSE);
	pTDC->SetFocusToTasks();
	
	// make sure the tab control is correctly sized
	if (bResizeDlg)
		Resize();

	// if this is the only control then set or terminate the various status 
	// check timers
	if (GetTDCCount() == 1)
		RestoreTimers();
	
	// make sure everything looks okay
	Invalidate();
	UpdateWindow();
	
	return nSel;
}

void CToDoListWnd::KillTimers()
{
	SetTimer(TIMER_READONLYSTATUS, FALSE);
	SetTimer(TIMER_TIMESTAMPCHANGE, FALSE);
	SetTimer(TIMER_CHECKOUTSTATUS, FALSE);
	SetTimer(TIMER_AUTOSAVE, FALSE);
	SetTimer(TIMER_TIMETRACKING, FALSE);
	SetTimer(TIMER_AUTOMINIMIZE, FALSE);
	SetTimer(TIMER_TIMETRACKREMINDER, FALSE);
}

void CToDoListWnd::RestoreTimers()
{
	const CPreferencesDlg& prefs = Prefs();

	SetTimer(TIMER_READONLYSTATUS, (prefs.GetReadonlyReloadOption() != RO_NO));
	SetTimer(TIMER_TIMESTAMPCHANGE, (prefs.GetTimestampReloadOption() != RO_NO));
	SetTimer(TIMER_AUTOSAVE, prefs.GetAutoSaveFrequency());
	SetTimer(TIMER_CHECKOUTSTATUS, (prefs.GetKeepTryingToCheckout() || prefs.GetAutoCheckinFrequency()));
	SetTimer(TIMER_TIMETRACKING, TRUE);
	SetTimer(TIMER_AUTOMINIMIZE, prefs.GetAutoMinimizeFrequency());
	SetTimer(TIMER_TIMETRACKREMINDER, prefs.GetTrackReminderFrequency());
}

void CToDoListWnd::SetTimer(UINT nTimerID, BOOL bOn)
{
	if (bOn)
	{
		UINT nPeriod = 0;
		
		switch (nTimerID)
		{
		case TIMER_READONLYSTATUS:
			nPeriod = INTERVAL_READONLYSTATUS;
			break;
			
		case TIMER_TIMESTAMPCHANGE:
			nPeriod = INTERVAL_TIMESTAMPCHANGE;
			break;
			
		case TIMER_AUTOSAVE:
			nPeriod = (Prefs().GetAutoSaveFrequency() * ONE_MINUTE);
			break;
			
		case TIMER_CHECKOUTSTATUS:
			nPeriod = INTERVAL_CHECKOUTSTATUS;
			break;
			
		case TIMER_DUEITEMS:
			nPeriod = INTERVAL_DUEITEMS;
			break;
			
		case TIMER_TIMETRACKING:
			nPeriod = INTERVAL_TIMETRACKING;
			break;
			
		case TIMER_TIMETRACKREMINDER:
			nPeriod = (Prefs().GetTrackReminderFrequency() * ONE_MINUTE);
			break;
			
		case TIMER_AUTOMINIMIZE:
			nPeriod = (Prefs().GetAutoMinimizeFrequency() * ONE_MINUTE);
			break;
		}
		
		if (nPeriod)
		{
			UINT nID = CFrameWnd::SetTimer(nTimerID, nPeriod, NULL);
			ASSERT (nID);
		}
	}
	else
	{
		KillTimer(nTimerID);
	}
}

void CToDoListWnd::OnTimer(UINT nIDEvent) 
{
	CFrameWnd::OnTimer(nIDEvent);
	
	// if we are disabled (== modal dialog visible) then do not respond
	if (!IsWindowEnabled())
		return;
	
	// don't check whilst in the middle of saving or closing
	if (m_bSaving || m_bClosing)
		return;
	
	// if no controls are active kill the timers
	if (!GetTDCCount())
	{
		KillTimers();
		return;
	}
	
	switch (nIDEvent)
	{
	case TIMER_READONLYSTATUS:
		OnTimerReadOnlyStatus();
		break;
		
	case TIMER_TIMESTAMPCHANGE:
		OnTimerTimestampChange();
		break;
		
	case TIMER_AUTOSAVE:
		OnTimerAutoSave();
		break;
		
	case TIMER_CHECKOUTSTATUS:
		OnTimerCheckoutStatus();
		break;
		
	case TIMER_DUEITEMS:
		OnTimerDueItems();
		break;
		
	case TIMER_TIMETRACKING:
		OnTimerTimeTracking();
		break;
		
	case TIMER_TIMETRACKREMINDER:
		OnTimerTimeTrackReminder();
		break;
		
	case TIMER_AUTOMINIMIZE:
		OnTimerAutoMinimize();
		break;
	}
}

BOOL CToDoListWnd::IsActivelyTimeTracking() const
{
	// cycle thru tasklists until we find a time tracker
	int nCtrl = GetTDCCount();
	
	while (nCtrl--)
	{
		if (GetToDoCtrl(nCtrl).IsActivelyTimeTracking())
			return TRUE;
	}

	// else
	return FALSE;
}

void CToDoListWnd::OnTimerTimeTracking()
{
	AF_NOREENTRANT // macro helper
		
	static BOOL bWasTimeTracking = FALSE;
	BOOL bNowTimeTracking = IsActivelyTimeTracking();
		
	if (bWasTimeTracking != bNowTimeTracking)
	{
		UpdateWindowIcons();
		bWasTimeTracking = bNowTimeTracking;
	}

	m_dlgTimeTracker.UpdateTaskTime(&GetToDoCtrl());
}

void CToDoListWnd::UpdateWindowIcons()
{
	BOOL bTimeTracking = IsActivelyTimeTracking();
	UINT nIDIcon = (bTimeTracking ? IDI_TIMETRACK_STD : IDR_MAINFRAME_STD);

	if (!m_icons.IsInitialised())
		VERIFY(m_icons.Initialise(*this, nIDIcon));
	else
		VERIFY(m_icons.ModifyIcon(nIDIcon));

	m_trayIcon.SetIcon(m_icons.GetSmallIcon(), FALSE);
	m_dlgTimeTracker.SetWindowIcons(m_icons.GetBigIcon(), m_icons.GetSmallIcon());
}

void CToDoListWnd::OnTimerTimeTrackReminder()
{
	AF_NOREENTRANT // macro helper
		
	// process files
	CString sTasksTracking;
	int nNumTDC = GetTDCCount();
	
	for (int nCtrl = 0; nCtrl < nNumTDC; nCtrl++)
	{
		const CFilteredToDoCtrl& tdc = GetToDoCtrl(nCtrl);

		DWORD dwTaskID = tdc.GetTimeTrackTaskID(FALSE);
		DWORD dwActiveID = tdc.GetTimeTrackTaskID(TRUE);

		if (!dwTaskID && !dwActiveID)
			continue;

		if (nNumTDC > 1)
		{
			sTasksTracking += m_mgrToDoCtrls.GetFriendlyProjectName(nCtrl);
			sTasksTracking += _T(": ");
		}
		
		sTasksTracking += tdc.GetTaskTitle(dwTaskID);

		if (dwActiveID == 0) // paused
			sTasksTracking += CEnString(IDS_TRACKREMINDERPAUSED);

		sTasksTracking += ENDL;
	}
	
	// do we need to notify the user?
	if (!sTasksTracking.IsEmpty())
	{
		CEnString sMessage(IDS_TIMETRACKREMINDER, sTasksTracking);
		m_trayIcon.ShowBalloon(sMessage, CEnString(IDS_TIMETRACKREMINDER_BALLOONTITLE), NIIF_INFO);
	}
}

void CToDoListWnd::OnTimerDueItems(int nCtrl)
{
	AF_NOREENTRANT // macro helper
		
	int nFrom = (nCtrl == -1) ? 0 : nCtrl;
	int nTo = (nCtrl == -1) ? GetTDCCount() - 1 : nCtrl;
	BOOL bRepaint = FALSE;
	
	for (nCtrl = nFrom; nCtrl <= nTo; nCtrl++)
	{
		// first we search for overdue items on each tasklist and if that
		// fails to find anything we then search for items due today
		// but only if the tasklist is fully loaded
		if (m_mgrToDoCtrls.IsLoaded(nCtrl))
		{
			CFilteredToDoCtrl& tdc = GetToDoCtrl(nCtrl);
			TDCM_DUESTATUS nStatus = TDCM_NONE;
			
			if (tdc.HasOverdueTasks()) // takes priority
				nStatus = TDCM_PAST;

			else if (tdc.HasDueTodayTasks())
				nStatus = TDCM_TODAY;
			
			if (nStatus != m_mgrToDoCtrls.GetDueItemStatus(nCtrl))
			{
				m_mgrToDoCtrls.SetDueItemStatus(nCtrl, nStatus);
				bRepaint = TRUE;
			}
		}
	}

	if (bRepaint)
		m_tabCtrl.Invalidate(FALSE);
}

void CToDoListWnd::OnTimerReadOnlyStatus(int nCtrl)
{
	AF_NOREENTRANT // macro helper
		
	const CPreferencesDlg& userPrefs = Prefs();
	
	// work out whether we should check remote files or not
	BOOL bCheckRemoteFiles = (nCtrl != -1);
	
	if (!bCheckRemoteFiles)
	{
		static int nElapsed = 0;
		UINT nRemoteFileCheckInterval = userPrefs.GetRemoteFileCheckFrequency() * 1000; // in ms
		
		nElapsed %= nRemoteFileCheckInterval;
		bCheckRemoteFiles = !nElapsed;
		
		nElapsed += INTERVAL_READONLYSTATUS;
	}
	
	int nReloadOption = userPrefs.GetReadonlyReloadOption();
	
	ASSERT (nReloadOption != RO_NO);
	
	// process files
	CString sFileList;
	int nFrom = (nCtrl == -1) ? 0 : nCtrl;
	int nTo = (nCtrl == -1) ? GetTDCCount() - 1 : nCtrl;
	
	for (nCtrl = nFrom; nCtrl <= nTo; nCtrl++)
	{
		// don't check delay-loaded tasklists
		if (!m_mgrToDoCtrls.IsLoaded(nCtrl))
			continue;
		
		// don't check removeable drives
		int nType = m_mgrToDoCtrls.GetFilePathType(nCtrl);
		
        if (nType == TDCM_UNDEF || nType == TDCM_REMOVABLE)
			continue;
		
		// check remote files?
		if (!bCheckRemoteFiles && nType == TDCM_REMOTE)
			continue;
				
		if (m_mgrToDoCtrls.RefreshReadOnlyStatus(nCtrl))
		{
			CFilteredToDoCtrl& tdc = GetToDoCtrl(nCtrl);
		
			BOOL bReadOnly = m_mgrToDoCtrls.GetReadOnlyStatus(nCtrl);
			BOOL bReload = FALSE;
			
			if (nReloadOption == RO_ASK)
			{
				CString sFilePath = tdc.GetFilePath();
				CEnString sMessage(bReadOnly ? IDS_WRITABLETOREADONLY : IDS_READONLYTOWRITABLE, sFilePath);
				
				if (!bReadOnly) // might been modified
					sMessage += CEnString(IDS_WANTRELOAD);

				UINT nRet = MessageBox(sMessage, IDS_STATUSCHANGE_TITLE, !bReadOnly ? MB_YESNOCANCEL : MB_OK);
				
				bReload = (nRet == IDYES || nRet == IDOK);
			}
			else
				bReload = !bReadOnly; // now writable
			
			if (bReload && ReloadTaskList(nCtrl, FALSE, (nReloadOption == RO_ASK)))
			{
				// notify the user if we haven't already
				if (nReloadOption == RO_NOTIFY)
				{
					sFileList += tdc.GetFriendlyProjectName();
					sFileList += ENDL;
				}
			}
			else // update the UI
			{
				if (nCtrl == m_tabCtrl.GetCurSel())
					UpdateCaption();
				
				m_mgrToDoCtrls.UpdateToDoCtrlReadOnlyUIState(nCtrl);
				m_mgrToDoCtrls.UpdateTabItemText(nCtrl);
			}
		}
	}
	
	// do we need to notify the user?
	if (!sFileList.IsEmpty())
	{
		CEnString sMessage(IDS_TASKLISTSRELOADED, sFileList);
		m_trayIcon.ShowBalloon(sMessage, CEnString(IDS_READONLYCHANGE_BALLOONTITLE), NIIF_INFO);
	}
}

void CToDoListWnd::OnTimerTimestampChange(int nCtrl)
{
	AF_NOREENTRANT // macro helper
		
	const CPreferencesDlg& userPrefs = Prefs();
	int nReloadOption = userPrefs.GetTimestampReloadOption();
	
	ASSERT (nReloadOption != RO_NO);
	
	// work out whether we should check remote files or not
	BOOL bCheckRemoteFiles = (nCtrl != -1);
	
	if (!bCheckRemoteFiles)
	{
		static int nElapsed = 0;
		UINT nRemoteFileCheckInterval = userPrefs.GetRemoteFileCheckFrequency() * 1000; // in ms
		
		nElapsed %= nRemoteFileCheckInterval;
		bCheckRemoteFiles = !nElapsed;
		
		nElapsed += INTERVAL_TIMESTAMPCHANGE;
	}
	
	// process files
	CString sFileList;
	int nFrom = (nCtrl == -1) ? 0 : nCtrl;
	int nTo = (nCtrl == -1) ? GetTDCCount() - 1 : nCtrl;
	
	for (nCtrl = nFrom; nCtrl <= nTo; nCtrl++)
	{
		// don't check delay-loaded tasklists
		if (!m_mgrToDoCtrls.IsLoaded(nCtrl))
			continue;

		// don't check removeable drives
		int nType = m_mgrToDoCtrls.GetFilePathType(nCtrl);
		
        if (nType == TDCM_UNDEF || nType == TDCM_REMOVABLE)
			continue;
		
		// check remote files?
		if (!bCheckRemoteFiles && nType == TDCM_REMOTE)
			continue;
		
		if (m_mgrToDoCtrls.RefreshFileLastModified(nCtrl))
		{
			CFilteredToDoCtrl& tdc = GetToDoCtrl(nCtrl);

			BOOL bReload = TRUE; // default
			
			if (nReloadOption == RO_ASK)
			{
				CString sFilePath = tdc.GetFilePath();
			
				CEnString sMessage(IDS_MODIFIEDELSEWHERE, sFilePath);
				sMessage += CEnString(IDS_WANTRELOAD);
				
				bReload = (MessageBox(sMessage, IDS_TIMESTAMPCHANGE_TITLE, MB_YESNOCANCEL) == IDYES);
			}
			
			if (bReload && ReloadTaskList(nCtrl, FALSE, (nReloadOption == RO_ASK)))
			{
				// notify the user if we haven't already
				if (nReloadOption == RO_NOTIFY)
				{
					sFileList += tdc.GetFriendlyProjectName();
					sFileList += ENDL;
				}
			}
			else
			{
				// update UI
				if (nCtrl == m_tabCtrl.GetCurSel())
					UpdateCaption();
				
				m_mgrToDoCtrls.UpdateToDoCtrlReadOnlyUIState(nCtrl);
				m_mgrToDoCtrls.UpdateTabItemText(nCtrl);
			}
		}
	}
	
	// do we need to notify the user?
	if (!sFileList.IsEmpty())
	{
		CEnString sMessage(IDS_TASKLISTSRELOADED, sFileList);
		m_trayIcon.ShowBalloon(sMessage, CEnString(IDS_TIMESTAMPCHANGE_BALLOONTITLE), NIIF_INFO);
	}
}

void CToDoListWnd::OnTimerAutoSave()
{
	AF_NOREENTRANT // macro helper
		
	// don't save if the user is editing a task label
	if (!GetToDoCtrl().IsTaskLabelEditing())
		SaveAll(TDLS_AUTOSAVE);
}

void CToDoListWnd::OnTimerAutoMinimize()
{
	AF_NOREENTRANT // macro helper

	if (!IsWindowVisible() || IsIconic())
		return;

	// check whether we should hide ourselves
	double dElapsed = (Misc::GetTicksSinceLastUserInput() / ONE_MINUTE);
		
	if (dElapsed > (double)Prefs().GetAutoMinimizeFrequency())
		ShowWindow(SW_MINIMIZE);
}

void CToDoListWnd::OnTimerCheckoutStatus(int nCtrl)
{
	AF_NOREENTRANT // macro helper
		
	const CPreferencesDlg& userPrefs = Prefs();
	
	// work out whether we should check remote files or not
	BOOL bCheckRemoteFiles = (nCtrl != -1);
	
	if (!bCheckRemoteFiles)
	{
		static int nElapsed = 0;
		UINT nRemoteFileCheckInterval = userPrefs.GetRemoteFileCheckFrequency() * 1000; // in ms
		
		nElapsed %= nRemoteFileCheckInterval;
		bCheckRemoteFiles = !nElapsed;
		
		nElapsed += INTERVAL_CHECKOUTSTATUS;
	}
	
	// process files
	int nFrom = (nCtrl == -1) ? 0 : nCtrl;
	int nTo = (nCtrl == -1) ? GetTDCCount() - 1 : nCtrl;

	CString sCheckedInFiles, sCheckedOutFiles;
	
	for (nCtrl = nFrom; nCtrl <= nTo; nCtrl++)
	{
		// avoid lengthy operations
		if (!m_mgrToDoCtrls.IsSourceControlled(nCtrl))
			continue;

		BOOL bRemote = (m_mgrToDoCtrls.GetFilePathType(nCtrl) == TDCM_REMOTE);

		if (bRemote && !bCheckRemoteFiles)
			continue;
		
		BOOL bCheckedOut = m_mgrToDoCtrls.IsCheckedOut(nCtrl);
		
		// try to check out only if the previous attempt failed
		if (!bCheckedOut && userPrefs.GetKeepTryingToCheckout())
		{
			// we only try to check if the previous checkout failed
			if (!m_mgrToDoCtrls.GetLastCheckoutSucceeded(nCtrl) && m_mgrToDoCtrls.CheckOut(nCtrl))
			{
				// notify the user
				sCheckedOutFiles += m_mgrToDoCtrls.GetFriendlyProjectName(nCtrl);
				sCheckedOutFiles += ENDL;
			}
		}
		// only checkin if sufficient time has elapsed since last mod
		// and there are no mods outstanding
		else if (bCheckedOut && (userPrefs.GetAutoCheckinFrequency() > 0))
		{
			int nMin = m_mgrToDoCtrls.GetElapsedMinutesSinceLastMod(nCtrl);
			
			if ((nMin >= userPrefs.GetAutoCheckinFrequency()) && m_mgrToDoCtrls.CheckIn(nCtrl))
			{
				// notify the user
				sCheckedInFiles += m_mgrToDoCtrls.GetFriendlyProjectName(nCtrl);
				sCheckedInFiles += ENDL;
			}
		}
	}

	// do we need to notify the user?
	if (!sCheckedInFiles.IsEmpty() || !sCheckedOutFiles.IsEmpty())
	{
		// update UI
		UpdateCaption();

		CString sMessage;

		if (!sCheckedInFiles.IsEmpty())
			sMessage += CEnString(IDS_TASKLISTSAUTOCHECKEDIN, sCheckedInFiles);
		
		if (!sCheckedOutFiles.IsEmpty())
		{
			if (!sCheckedInFiles.IsEmpty())
				sMessage += ENDL;

			sMessage += CEnString(IDS_TASKLISTSAUTOCHECKEDOUT, sCheckedOutFiles);
		}

		m_trayIcon.ShowBalloon(sMessage, CEnString(IDS_SOURCECONTROLCHANGE_BALLOONTITLE), NIIF_INFO);
	}
}

void CToDoListWnd::OnNeedTooltipText(NMHDR* pNMHDR, LRESULT* pResult)
{
	static CString sTipText;
	sTipText.Empty();

	switch (pNMHDR->idFrom)
	{
	case ID_TOOLS_USERTOOL1:
	case ID_TOOLS_USERTOOL2:
	case ID_TOOLS_USERTOOL3:
	case ID_TOOLS_USERTOOL4:
	case ID_TOOLS_USERTOOL5:
	case ID_TOOLS_USERTOOL6:
	case ID_TOOLS_USERTOOL7:
	case ID_TOOLS_USERTOOL8:
	case ID_TOOLS_USERTOOL9:
	case ID_TOOLS_USERTOOL10:
	case ID_TOOLS_USERTOOL11:
	case ID_TOOLS_USERTOOL12:
	case ID_TOOLS_USERTOOL13:
	case ID_TOOLS_USERTOOL14:
	case ID_TOOLS_USERTOOL15:
	case ID_TOOLS_USERTOOL16:
		{
			USERTOOL ut;

			if (Prefs().GetUserTool(pNMHDR->idFrom - ID_TOOLS_USERTOOL1, ut))
				sTipText = ut.sToolName;
		}
		break;

	default:
		// tab control popups
		if (pNMHDR->idFrom >= 0 && pNMHDR->idFrom < (UINT)m_mgrToDoCtrls.GetCount())
		{
			sTipText = m_mgrToDoCtrls.GetTabItemTooltip(pNMHDR->idFrom);
		}
		break;
	}

	if (!sTipText.IsEmpty())
	{
		TOOLTIPTEXT* pTTT = (TOOLTIPTEXT*)pNMHDR;
		pTTT->lpszText = (LPTSTR)(LPCTSTR)sTipText;
	}

	*pResult = 0;
}

void CToDoListWnd::OnUpdateUserTool(CCmdUI* pCmdUI) 
{
	if (pCmdUI->m_pMenu && pCmdUI->m_nID == ID_TOOLS_USERTOOL1) // only handle first item
	{
		CUserToolArray aTools;
		Prefs().GetUserTools(aTools);
		
		CTDCToolsHelper th(Prefs().GetEnableTDLExtension(), ID_TOOLS_USERTOOL1, MAX_NUM_TOOLS);
		th.UpdateMenu(pCmdUI, aTools, m_mgrMenuIcons);
	}
	else if (m_bShowToolbar) 
	{
		int nTool = pCmdUI->m_nID - ID_TOOLS_USERTOOL1;
		ASSERT (nTool >= 0 && nTool < MAX_NUM_TOOLS);

		USERTOOL ut;
		
		if (Prefs().GetUserTool(nTool, ut))
			pCmdUI->Enable(TRUE);
	}
}

void CToDoListWnd::OnUserTool(UINT nCmdID) 
{
	int nTool = nCmdID - ID_TOOLS_USERTOOL1;
	USERTOOL ut;
	
	ASSERT (nTool >= 0 && nTool < MAX_NUM_TOOLS);

	const CPreferencesDlg& prefs = Prefs();
	
	if (prefs.GetUserTool(nTool, ut))
	{
		// Save all tasklists before executing the user tool
		if (prefs.GetAutoSaveOnRunTools())
		{
			if (SaveAll(TDLS_FLUSH) == TDCF_CANCELLED)
				return;
		}

		USERTOOLARGS args;
		PopulateToolArgs(args);

		CTDCToolsHelper th(prefs.GetEnableTDLExtension(), ID_TOOLS_USERTOOL1, MAX_NUM_TOOLS);
		th.RunTool(ut, args, this);
	}
}

void CToDoListWnd::OnShowTaskView(UINT nCmdID) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	switch (nCmdID)
	{
	case ID_SHOWVIEW_TASKTREE:
		ASSERT(0); // this is not accessible
		break;

	case ID_SHOWVIEW_LISTVIEW:
		tdc.ShowListViewTab(!tdc.IsListViewTabShowing());
		break;

	case ID_SHOWVIEW_UIEXTENSION1:
	case ID_SHOWVIEW_UIEXTENSION2:
	case ID_SHOWVIEW_UIEXTENSION3:
	case ID_SHOWVIEW_UIEXTENSION4:
	case ID_SHOWVIEW_UIEXTENSION5:
	case ID_SHOWVIEW_UIEXTENSION6:
	case ID_SHOWVIEW_UIEXTENSION7:
	case ID_SHOWVIEW_UIEXTENSION8:
	case ID_SHOWVIEW_UIEXTENSION9:
	case ID_SHOWVIEW_UIEXTENSION10:
	case ID_SHOWVIEW_UIEXTENSION11:
	case ID_SHOWVIEW_UIEXTENSION12:
	case ID_SHOWVIEW_UIEXTENSION13:
	case ID_SHOWVIEW_UIEXTENSION14:
	case ID_SHOWVIEW_UIEXTENSION15:
	case ID_SHOWVIEW_UIEXTENSION16:
		{
			CStringArray aTypeIDs;
			
			if (tdc.GetVisibleExtensionViews(aTypeIDs))
			{
				if (CTDCUIExtensionHelper::ProcessViewVisibilityMenuCmd(nCmdID, m_mgrUIExtensions, aTypeIDs))
					tdc.SetVisibleExtensionViews(aTypeIDs);
			}
		}
		break;
	}
}

void CToDoListWnd::OnUpdateShowTaskView(CCmdUI* pCmdUI) 
{
	switch (pCmdUI->m_nID)
	{
	case ID_SHOWVIEW_TASKTREE:
		// ticked and disabled by default
		break;

	case ID_SHOWVIEW_LISTVIEW:
		pCmdUI->SetCheck(GetToDoCtrl().IsListViewTabShowing());
		break;

	case ID_SHOWVIEW_UIEXTENSION1:
		// handled in OnInitMenuPopup
		break;
	}
}

void CToDoListWnd::AddUserStorageToMenu(CMenu* pMenu) 
{
	if (pMenu)
	{
		const UINT MENUSTARTID = pMenu->GetMenuItemID(0);

		// delete existing entries
		int nStore = 16;

		while (nStore--)
		{
			pMenu->DeleteMenu(nStore, MF_BYPOSITION);
		}
		
		// if we have any tools to add we do it here
		int nNumStorage = min(m_mgrStorage.GetNumStorage(), 16);

		if (nNumStorage)
		{
			UINT nFlags = (MF_BYPOSITION | MF_STRING);

			for (int nStore = 0; nStore < nNumStorage; nStore++)
			{
				CString sMenuItem, sText = m_mgrStorage.GetStorageMenuText(nStore);
								
				if (nStore < 9)
					sMenuItem.Format(_T("&%d %s"), nStore + 1, sText);
				else
					sMenuItem = sText;
				
				pMenu->InsertMenu(nStore, nFlags, MENUSTARTID + nStore, sMenuItem);

				// add icon if available
				HICON hIcon = m_mgrStorage.GetStorageIcon(nStore);

				if (hIcon)
					m_mgrMenuIcons.AddImage(MENUSTARTID + nStore, hIcon);
			}
		}
		else // if nothing to add just re-add placeholder
		{
			pMenu->InsertMenu(0, MF_BYPOSITION | MF_STRING | MF_GRAYED, MENUSTARTID, _T("3rd Party Storage"));
		}
	}
}

void CToDoListWnd::OnFileOpenFromUserStorage(UINT nCmdID) 
{
	int nStorage = nCmdID - ID_FILE_OPEN_USERSTORAGE1;
	
	ASSERT (nStorage >= 0 && nStorage < 16);

	TSM_TASKLISTINFO storageInfo;
	CPreferences prefs;
	CTaskFile tasks;

	if (m_mgrStorage.RetrieveTasklist(storageInfo, tasks, nStorage, prefs))
	{
		CString sFilePath = storageInfo.EncodeInfo(Prefs().GetSaveStoragePasswords());
		TDC_FILE nOpen = OpenTaskList(sFilePath, TRUE);

		if (nOpen == TDCF_SUCCESS)
		{
			// nothing more to do
		}
		else
			HandleLoadTasklistError(nOpen, storageInfo.szDisplayName);
		
		// refresh UI
		UpdateCaption();
		UpdateStatusbar();
		Resize();
		UpdateWindow();
	}
}

void CToDoListWnd::OnFileSaveToUserStorage(UINT nCmdID) 
{
	int nStorage = (nCmdID - ID_FILE_SAVE_USERSTORAGE1);
	ASSERT (nStorage >= 0 && nStorage < 16);

	// retrieve any existing storage info for this tasklist
	int nTDC = GetSelToDoCtrl();
	TSM_TASKLISTINFO storageInfo;

	BOOL bUsesStorage = m_mgrToDoCtrls.GetStorageDetails(nTDC, storageInfo);

	if (bUsesStorage)
	{
		// clear the storage info ID if we are changing the destination
		if (nStorage != m_mgrStorage.FindStorage(storageInfo.sStorageID))
		{
			storageInfo.Reset();
		}
		else // clear the tasklist ID to force prompting
		{
			storageInfo.szTasklistID[0] = 0;
		}
	}

	// save the existing tasklist to temp path
	CString sTempPath = FileMisc::GetTempFilePath();
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	CTaskFile tasks;

	if (!bUsesStorage && (tdc.Save(tasks) == TDCF_SUCCESS))
	{
		// copy to temp location
		FileMisc::CopyFile(tdc.GetFilePath(), sTempPath, TRUE, TRUE);
	}
	else if (tdc.Save(tasks, sTempPath) != TDCF_SUCCESS)
	{
		// TODO
		return;
	}

	storageInfo.SetLocalFilePath(sTempPath);

		
	// prevent this save triggering a reload
	m_mgrToDoCtrls.RefreshFileLastModified(nTDC);
	
	// scope for progress bar
	{
		DOPROGRESS(IDS_SAVINGPROGRESS);
		CPreferences prefs;

		if (!m_mgrStorage.StoreTasklist(storageInfo, tasks, nStorage, prefs))
		{
			// assume storage plugin has handled error
			return;
		}
	}

	m_mgrToDoCtrls.SetStorageDetails(nTDC, storageInfo);
		
	UpdateCaption();
	UpdateStatusbar();
	Resize();
	UpdateWindow();
}

BOOL CToDoListWnd::HasSysTrayOptions(int nOption1, int nOption2) const
{
	int nSysOpt = Prefs().GetSysTrayOption();

	if (nSysOpt == nOption1)
	{
		return TRUE;
	}
	else if (nOption2 != STO_NONE) 
	{
		return (nSysOpt == nOption2);
	}

	// all else
	return FALSE;
}

void CToDoListWnd::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	// test for top-level menus
	if (bSysMenu)
		return;

	if (m_menubar.GetSubMenu(AM_FILE) == pPopupMenu)
	{
		// insert Min to sys tray if appropriate 
		BOOL bHasMinToTray = (::GetMenuString(*pPopupMenu, ID_MINIMIZETOTRAY, NULL, 0, MF_BYCOMMAND) != 0);
		
		if (HasSysTrayOptions(STO_ONCLOSE, STO_ONMINCLOSE))
		{
			if (!bHasMinToTray)
				pPopupMenu->InsertMenu(ID_EXIT, MF_BYCOMMAND, ID_MINIMIZETOTRAY, CEnString(ID_MINIMIZETOTRAY));
		}
		else if (bHasMinToTray) // then remove
		{
			pPopupMenu->DeleteMenu(ID_MINIMIZETOTRAY, MF_BYCOMMAND);
		}
	}
	else if (m_menubar.GetSubMenu(AM_EDIT) == pPopupMenu)
	{
		// remove relevant commands from the edit menu
		PrepareEditMenu(pPopupMenu);
	}
	else if (m_menubar.GetSubMenu(AM_SORT) == pPopupMenu)
	{
		// remove relevant commands from the sort menu
		PrepareSortMenu(pPopupMenu);
	}
	else if (m_menubar.GetSubMenu(AM_VIEW) == pPopupMenu)
	{
		// add UI extensions
		HMENU hMenuUIExt = CEnMenu::GetSubMenu(*pPopupMenu, ID_SHOWVIEW_TASKTREE);

		if (hMenuUIExt)
		{
			CStringArray aTypeIDs;
			GetToDoCtrl().GetVisibleExtensionViews(aTypeIDs);
			
			CTDCUIExtensionHelper::PrepareViewVisibilityMenu(CMenu::FromHandle(hMenuUIExt), m_mgrUIExtensions, aTypeIDs);
		} 
	}
	else // all other sub-menus
	{
		// test for 'Open From...'
		if (pPopupMenu->GetMenuItemID(0) == ID_FILE_OPEN_USERSTORAGE1)
		{
			AddUserStorageToMenu(pPopupMenu);
		}
		// test for 'save To...'
		else if (pPopupMenu->GetMenuItemID(0) == ID_FILE_SAVE_USERSTORAGE1)
		{
			AddUserStorageToMenu(pPopupMenu);
		}
	}

	CFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
}

void CToDoListWnd::OnViewToolbar() 
{
	m_bShowToolbar = !m_bShowToolbar;
	m_toolbar.ShowWindow(m_bShowToolbar ? SW_SHOW : SW_HIDE);
	m_toolbar.EnableWindow(m_bShowToolbar);

	Resize();
	Invalidate(TRUE);
}

void CToDoListWnd::OnUpdateViewToolbar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bShowToolbar);
}

void CToDoListWnd::AppendTools2Toolbar(BOOL bAppend)
{
	CTDCToolsHelper th(Prefs().GetEnableTDLExtension(), ID_TOOLS_USERTOOL1, MAX_NUM_TOOLS);
	
	if (bAppend)
	{
		// then re-add
		CUserToolArray aTools;
		Prefs().GetUserTools(aTools);
		
		th.AppendToolsToToolbar(aTools, m_toolbar, ID_PREFERENCES);

		// refresh tooltips
		m_tbHelper.Release();
		m_tbHelper.Initialize(&m_toolbar, this, &m_mgrShortcuts);
	}
	else // remove
	{
		th.RemoveToolsFromToolbar(m_toolbar, ID_PREFERENCES);
	}

	// resize toolbar to accept the additional buttons
	Resize();
}

void CToDoListWnd::OnSort() 
{
	GetToDoCtrl().Resort(TRUE);
}

void CToDoListWnd::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFrameWnd::OnWindowPosChanged(lpwndpos);

// 	// Keep time tracker in front of us 
// 	if (IsWindowVisible() && !IsIconic() && m_dlgTimeTracker.IsWindowVisible())
// 		m_dlgTimeTracker.SetWindowPos(this, 0, 0, 0, 0, (SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE));
}

void CToDoListWnd::OnArchiveCompletedtasks() 
{
	CWaitCursor cursor;
	int nSelTDC = GetSelToDoCtrl();
	
	if (m_mgrToDoCtrls.ArchiveDoneTasks(nSelTDC))
	{
		// auto-reload archive if it's loaded
		CString sArchivePath = m_mgrToDoCtrls.GetArchivePath(nSelTDC);
		int nArchiveTDC = m_mgrToDoCtrls.FindToDoCtrl(sArchivePath);

		if (nArchiveTDC != -1 && m_mgrToDoCtrls.IsLoaded(nArchiveTDC))
			ReloadTaskList(nArchiveTDC, FALSE, FALSE);
	
		UpdateCaption();
	}
	else
	{
		// TODO
	}
}

void CToDoListWnd::OnUpdateArchiveCompletedtasks(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	pCmdUI->Enable(!tdc.IsReadOnly() && !tdc.IsArchive());
}

void CToDoListWnd::OnArchiveSelectedTasks() 
{
	CWaitCursor cursor;
	int nSelTDC = GetSelToDoCtrl();
	
	if (m_mgrToDoCtrls.ArchiveSelectedTasks(nSelTDC))
	{
		// auto-reload archive if it's loaded
		CString sArchivePath = m_mgrToDoCtrls.GetArchivePath(nSelTDC);
		int nArchiveTDC = m_mgrToDoCtrls.FindToDoCtrl(sArchivePath);

		if (nArchiveTDC != -1 && m_mgrToDoCtrls.IsLoaded(nArchiveTDC))
			ReloadTaskList(nArchiveTDC, FALSE, FALSE);
	
		UpdateCaption();
	}
}

void CToDoListWnd::OnUpdateArchiveSelectedCompletedTasks(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	pCmdUI->Enable(!tdc.IsReadOnly() && !tdc.IsArchive() && !tdc.IsArchive());
}

void CToDoListWnd::OnMovetaskdown() 
{
	GetToDoCtrl().MoveSelectedTask(TDCM_DOWN);	
}

void CToDoListWnd::OnUpdateMovetaskdown(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanMoveSelectedTask(TDCM_DOWN));	
}

void CToDoListWnd::OnMovetaskup() 
{
	GetToDoCtrl().MoveSelectedTask(TDCM_UP);	
}

void CToDoListWnd::OnUpdateMovetaskup(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanMoveSelectedTask(TDCM_UP));	
}

void CToDoListWnd::OnMovetaskright() 
{
	GetToDoCtrl().MoveSelectedTask(TDCM_RIGHT);	
}

void CToDoListWnd::OnUpdateMovetaskright(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanMoveSelectedTask(TDCM_RIGHT));	
}

void CToDoListWnd::OnMovetaskleft() 
{
	GetToDoCtrl().MoveSelectedTask(TDCM_LEFT);	
}

void CToDoListWnd::OnUpdateMovetaskleft(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanMoveSelectedTask(TDCM_LEFT));	
}

CFilteredToDoCtrl& CToDoListWnd::GetToDoCtrl()
{
	return GetToDoCtrl(GetSelToDoCtrl());
}

CFilteredToDoCtrl& CToDoListWnd::GetToDoCtrl(int nIndex)
{
	return m_mgrToDoCtrls.GetToDoCtrl(nIndex);
}

const CFilteredToDoCtrl& CToDoListWnd::GetToDoCtrl() const
{
	return GetToDoCtrl(GetSelToDoCtrl());
}

const CFilteredToDoCtrl& CToDoListWnd::GetToDoCtrl(int nIndex) const
{
	return m_mgrToDoCtrls.GetToDoCtrl(nIndex);
}

CFilteredToDoCtrl* CToDoListWnd::NewToDoCtrl(BOOL bVisible, BOOL bEnabled)
{
	CHoldRedraw hr(*this);
	BOOL bFirstTDC = (GetTDCCount() == 0);
	
	// if the active tasklist is unsaved and unmodified then delete it
	if (!bFirstTDC)
	{
		// make sure that we don't accidentally delete a just edited tasklist
		CFilteredToDoCtrl& tdc = GetToDoCtrl();
		tdc.Flush(); 
		
		int nSel = GetSelToDoCtrl();
		
		if (m_mgrToDoCtrls.IsPristine(nSel))
		{
			m_dlgTimeTracker.RemoveTasklist(&tdc);
			m_mgrToDoCtrls.RemoveToDoCtrl(nSel, TRUE); // TRUE -> Delete
			
			// make sure we reset the selection to a valid index
			if (GetTDCCount())
			{
				// try leaving the selection as-is
				if (nSel >= GetTDCCount())
					nSel--; // try the preceding item
				
				SelectToDoCtrl(nSel, FALSE);
			}
		}
	}
	
	// else
	TDCCOLEDITFILTERVISIBILITY vis;
	Prefs().GetDefaultColumnEditFilterVisibility(vis);
	
	CFilteredToDoCtrl* pTDC = new CFilteredToDoCtrl(m_mgrUIExtensions, 
													m_mgrContent, 
													m_tdiDefault.sCommentsTypeID, 
													vis);
	
	// create somewhere out in space
	CRect rCtrl(-32010, -32010, -32000, -32000);
	
	if (pTDC && pTDC->Create(rCtrl, this, IDC_TODOLIST, bVisible, bEnabled))
	{
		// set font to our font
		CDialogHelper::SetFont(pTDC, m_fontMain, FALSE);
		
		// set the 'save ui to tasklist' flag once only
		// allowing the taskfile itself to override
		pTDC->SetStyle(TDCS_SAVEUIVISINTASKLIST, m_bSaveUIVisInTaskList);
		pTDC->SetUITheme(m_theme);
		
		// rest of runtime preferences
		UpdateToDoCtrlPreferences(pTDC);

		if (bFirstTDC)
		{
			// Extensions are 'lazy' loaded so this is the first chance
			// to allow them to load global preferences
			CPreferences prefs;
			
			m_mgrUIExtensions.LoadPreferences(prefs, _T("UIExtensions"));
			m_mgrContent.LoadPreferences(prefs, _T("ContentControls"), FALSE);
		}
		
		return pTDC;
	}
	
	// else
	delete pTDC;
	return NULL;
}

void CToDoListWnd::OnTabCtrlCloseTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	// don't close the tab if only one is open and it is pristine
	if ((GetTDCCount() == 1) && m_mgrToDoCtrls.IsPristine(0))
		return;

	NMTABCTRLEX* pNMTCE = (NMTABCTRLEX*)pNMHDR;
	
	// check valid tab
	if (pNMTCE->iTab >= 0)
	{
		CFilteredToDoCtrl& tdc = GetToDoCtrl(pNMTCE->iTab);
		tdc.Flush();
		
		CloseToDoCtrl(pNMTCE->iTab);
		
		if (!GetTDCCount())
			CreateNewTaskList(FALSE);
	}
	*pResult = 0;
}

void CToDoListWnd::OnTabCtrlEndDrag(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTABCTRLEX* pNMTCE = (NMTABCTRLEX*)pNMHDR;
	
	// check valid tab indices
	ASSERT((pNMTCE->iTab != -1) && (pNMTCE->nExtra != 0));

	if ((pNMTCE->iTab != -1) && (pNMTCE->nExtra != 0))
	{
		m_mgrToDoCtrls.MoveToDoCtrl(pNMTCE->iTab, pNMTCE->nExtra);
		
		// disable alpha-sorting on tabs
		if (Prefs().GetKeepTabsOrdered())
		{
			CPreferences().WriteProfileInt(PREF_KEY, _T("KeepTabsOrdered"), FALSE);
			ResetPrefs();
		}
	}

	*pResult = 0;
}

void CToDoListWnd::OnTabCtrlSelchanging(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	// cache the outgoing selection
	m_nLastSelItem = GetSelToDoCtrl();
	
	// and flush
	if (m_nLastSelItem != -1)
	{
		CFilteredToDoCtrl& tdc = GetToDoCtrl(m_nLastSelItem);
		tdc.Flush();

		// and save
		if (Prefs().GetAutoSaveOnSwitchTasklist() && !tdc.GetFilePath().IsEmpty() && tdc.IsModified())
		{
			CTaskFile dummy;

			if (DoSaveWithBackupAndProgress(tdc, m_nLastSelItem, dummy) == TDCF_SUCCESS)
			{
				// update status
				m_mgrToDoCtrls.RefreshFileLastModified(m_nLastSelItem); 
				m_mgrToDoCtrls.RefreshReadOnlyStatus(m_nLastSelItem); 
				m_mgrToDoCtrls.RefreshPathType(m_nLastSelItem); 
			}
		}
	}
	
	*pResult = 0;
}

void CToDoListWnd::OnTabCtrlSelchange(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	// show the incoming selection
	int nCurSel = GetSelToDoCtrl();

	// check password if necessary
	if (m_nLastSelItem != -1 && !VerifyToDoCtrlPassword())
	{
		m_tabCtrl.SetCurSel(m_nLastSelItem);
		return;
	}

	int nDueBy = Prefs().GetNotifyDueByOnSwitch();
	
	if (nCurSel != -1)
	{
		// make sure it's loaded
		if (!VerifyTaskListOpen(nCurSel, (nDueBy == -1)))
		{
			// restore the previous tab
			m_tabCtrl.SetCurSel(m_nLastSelItem);
			return;
		}

		CFilteredToDoCtrl& tdcShow = GetToDoCtrl(nCurSel);
		UpdateToDoCtrlPreferences(&tdcShow);

		// update the filter selection
 		RefreshFilterBarControls();
 		
		// update status bar
		UpdateStatusbar();
		UpdateCaption();

		// make sure size is right
		CRect rTDC;
		
		if (CalcToDoCtrlRect(rTDC))
			tdcShow.MoveWindow(rTDC, FALSE);

		// always come out of 'Maximise Comments'
		if (m_nMaxState == TDCMS_MAXCOMMENTS)
			m_nMaxState = TDCMS_NORMAL;
		
		tdcShow.SetMaximizeState(m_nMaxState);
		tdcShow.EnableWindow(TRUE);
		tdcShow.ShowWindow(SW_SHOW);
	}
	
	// hide the outgoing selection
	if (m_nLastSelItem != -1)
	{
		CFilteredToDoCtrl& tdcHide = GetToDoCtrl(m_nLastSelItem);

		tdcHide.ShowWindow(SW_HIDE);
		tdcHide.EnableWindow(FALSE);

		m_nLastSelItem = -1; // reset
	}
	
	if (nCurSel != -1)
	{
		CFilteredToDoCtrl& tdcShow = GetToDoCtrl(nCurSel);
		tdcShow.UpdateWindow();

		// update find dialog with this ToDoCtrl's custom attributes
		UpdateFindDialogActiveTasklist(&tdcShow);
		RefreshFindTasksListData();

		// leave focus setting till last else the 'old' tasklist flashes
		tdcShow.SetFocusToTasks();

		// check for external changes to file
		OnTimerTimestampChange(nCurSel);
		OnTimerReadOnlyStatus(nCurSel);

		// notify user of due tasks if req
		DoDueTaskNotification(nCurSel, nDueBy);

		UpdateAeroFeatures();
	}

	UpdateMenuIconMgrSourceControlStatus();
	RefreshPauseTimeTracking();
	
	*pResult = 0;
}

void CToDoListWnd::UpdateMenuIconMgrSourceControlStatus()
{
	// figure out current state
	BOOL bCurDisabled = m_mgrMenuIcons.HasImageID(ID_TOOLS_TOGGLECHECKIN);
	BOOL bCurCheckedOut = (!bCurDisabled && m_mgrMenuIcons.HasImageID(ID_TOOLS_CHECKIN));

	// figure out new state
	BOOL bNewDisabled = TRUE;
	BOOL bNewCheckedOut = FALSE;

	if (GetTDCCount())
	{
		const CFilteredToDoCtrl& tdc = GetToDoCtrl();

		if (tdc.IsSourceControlled())
		{
			bNewDisabled = FALSE;
			bNewCheckedOut = tdc.IsCheckedOut();
		}
	}

	if (bNewDisabled)
	{
		if (bCurDisabled)
		{
			return;
		}
		else if (bCurCheckedOut)
		{
			m_mgrMenuIcons.ChangeImageID(ID_TOOLS_CHECKIN, ID_TOOLS_TOGGLECHECKIN);
		}
		else // checkedin
		{
			m_mgrMenuIcons.ChangeImageID(ID_TOOLS_CHECKOUT, ID_TOOLS_TOGGLECHECKIN);
		}
	}
	else if (bNewCheckedOut)
	{
		if (bCurDisabled)
		{
			m_mgrMenuIcons.ChangeImageID(ID_TOOLS_TOGGLECHECKIN, ID_TOOLS_CHECKIN);
		}
		else if (bCurCheckedOut)
		{
			return;
		}
		else // checkedin
		{
			m_mgrMenuIcons.ChangeImageID(ID_TOOLS_CHECKOUT, ID_TOOLS_CHECKIN);
		}
	}
	else // new == checkedin
	{
		if (bCurDisabled)
		{
			m_mgrMenuIcons.ChangeImageID(ID_TOOLS_TOGGLECHECKIN, ID_TOOLS_CHECKOUT);
		}
		else if (bCurCheckedOut)
		{
			m_mgrMenuIcons.ChangeImageID(ID_TOOLS_CHECKIN, ID_TOOLS_CHECKOUT);
		}
		else // checkedin
		{
			return;
		}
	}
}

void CToDoListWnd::RefreshFilterBarControls()
{
	if (m_bShowFilterBar)
	{
		CFilteredToDoCtrl& tdc = GetToDoCtrl();

		// if this tasklist's filter is an unnamed custom one
		RefreshFilterBarCustomFilters();

		// get existing filter bar size so we can determine if a 
		// resize if required
		CRect rFilter;

		m_filterBar.GetClientRect(rFilter);
		m_filterBar.RefreshFilterControls(tdc);

		CRect rClient;
		GetClientRect(rClient);

		if (m_filterBar.CalcHeight(rClient.Width()) != rFilter.Height())
		{
			m_filterBar.Invalidate();
			Resize();
		}
		else if (GetFilterBarRect(rFilter))
		{
			InvalidateRect(rFilter, TRUE);
		}
	}
}

void CToDoListWnd::RefreshFindTasksListData()
{
	TDCAUTOLISTDATA tldActive, tldAll;

	m_mgrToDoCtrls.GetAutoListData(tldActive, tldAll);
	m_findDlg.SetAttributeListData(tldActive, tldAll);
}

TDC_FILE CToDoListWnd::ConfirmSaveTaskList(int nIndex, DWORD dwFlags)
{
	BOOL bClosingWindows = Misc::HasFlag(dwFlags, TDLS_CLOSINGWINDOWS);
	BOOL bClosingTaskList = Misc::HasFlag(dwFlags, TDLS_CLOSINGTASKLISTS) || bClosingWindows; // sanity check
	TDC_FILE nSave = TDCF_SUCCESS;
	
	// save changes
	CFilteredToDoCtrl& tdc = GetToDoCtrl(nIndex);
	
	if (tdc.IsModified())
	{
		BOOL bFirstTimeSave = (tdc.GetFilePath().IsEmpty() &&
								!m_mgrToDoCtrls.UsesStorage(nIndex));

		// if we are closing Windows, we don't bother asking
		// we just save and get out as fast as poss
		if (bClosingWindows)
		{
			// if it's a first time save we just save to a temp file
			if (bFirstTimeSave)
			{
				tdc.Save(GetEndSessionFilePath());
			}
			else
			{
				m_mgrToDoCtrls.DoBackup(nIndex);
				tdc.Save();
			}

			// always return success
			return TDCF_SUCCESS;
		}
		// else we obey the user's preferences
		else if (bClosingTaskList && (bFirstTimeSave || Prefs().GetConfirmSaveOnExit()))
		{
			// make sure app is visible
			Show(FALSE);

			// save tasklist
			CString sName(m_mgrToDoCtrls.FormatProjectNameWithFileName(nIndex));
			CEnString sMessage(IDS_SAVEBEFORECLOSE, sName);
			
			// don't allow user to cancel if closing down
			int nRet = MessageBox(sMessage, IDS_CONFIRMSAVE_TITLE, bClosingWindows ? MB_YESNO : MB_YESNOCANCEL);
			
			if (nRet == IDYES)
			{
				// note: we omit the auto save parameter here because we want the user to
				// be notified of any problems
				nSave = SaveTaskList(nIndex);

				// if the save failed (as opposed to cancelled) then we must
				// propagate this upwards
				if (nSave != TDCF_SUCCESS && nSave != TDCF_CANCELLED)
				{
					return nSave;
				}
				else if (nSave == TDCF_CANCELLED)
				{
					// user can still cancel save dialog even if closing down
					// but not if closing windows
					nRet = bClosingWindows ? IDNO : IDCANCEL;
				}
			}
			
			ASSERT (!(bClosingWindows && nRet == IDCANCEL)); // sanity check
			
			if (nRet == IDCANCEL)
			{
				return TDCF_CANCELLED;
			}
			else
			{
				tdc.SetModified(FALSE); // so we don't get prompted again
			}
		}
		else
		{
			nSave = SaveTaskList(nIndex, NULL, Misc::HasFlag(dwFlags, TDLS_AUTOSAVE));
		}
	}
	
	return nSave; // user did not cancel
}

BOOL CToDoListWnd::CloseToDoCtrl(int nIndex)
{
	ASSERT (nIndex >= 0);
	ASSERT (nIndex < GetTDCCount());

	CFilteredToDoCtrl& tdcSel = GetToDoCtrl();
	CFilteredToDoCtrl& tdc = GetToDoCtrl(nIndex);

	tdc.Flush(TRUE);
	
	if (ConfirmSaveTaskList(nIndex, TDLS_CLOSINGTASKLISTS) != TDCF_SUCCESS)
		return FALSE;
	
	// remove any find results/tracking associated with this tasklist
	m_findDlg.DeleteResults(&tdc);
	m_dlgTimeTracker.RemoveTasklist(&tdc);
	
	CWaitCursor cursor;

	// save off current reminders
	m_reminders.CloseToDoCtrl(&tdc);

	int nNewSel = m_mgrToDoCtrls.RemoveToDoCtrl(nIndex, TRUE);
	
	if (nNewSel != -1)
	{
		// if we're closing TDL then the main window will not
		// be visible at this point so we don't have to worry about
		// encrypted tasklists becoming visible. however if as a result
		// of this closure an encrypted tasklist would become visible
		// then we need to prompt for a password and if this fails
		// we must create another tasklist to hide the encrypted one.
		// unless the tasklist being closed was not active and the 
		// new selection hasn't actually changed
		BOOL bCheckPassword = (!m_bClosing && (&GetToDoCtrl(nNewSel) != &tdcSel));

		if (!SelectToDoCtrl(nNewSel, bCheckPassword))
		{
			CreateNewTaskList(FALSE);
			RefreshTabOrder();
		}

		if (!m_bClosing)
		{
			Resize();

			if (m_bShowFilterBar)
				m_filterBar.Invalidate();

			Invalidate();
			GetToDoCtrl().Invalidate();
		}
	}
	
	return TRUE;
}

void CToDoListWnd::OnCloseTasklist() 
{
	int nSel = GetSelToDoCtrl();
	CFilteredToDoCtrl& tdc = GetToDoCtrl(nSel);

	// make sure there are no edits pending
	tdc.Flush(TRUE); 
	
	// check if its a pristine tasklist and the last tasklist and 
	// if so only close it if the default comments type has changed
	if (m_mgrToDoCtrls.IsPristine(nSel) && GetTDCCount() == 1)
		return;
	
	CloseToDoCtrl(nSel);
	
	// if empty then create a new dummy item		
	if (!GetTDCCount())
		CreateNewTaskList(FALSE);
	else
		Resize();
}

BOOL CToDoListWnd::SelectToDoCtrl(LPCTSTR szFilePath, BOOL bCheckPassword, int nNotifyDueTasksBy)
{
	int nCtrl = m_mgrToDoCtrls.FindToDoCtrl(szFilePath);
	
	if (nCtrl != -1)
	{
		SelectToDoCtrl(nCtrl, bCheckPassword, nNotifyDueTasksBy);
		return TRUE;
	}
	
	return FALSE;
}

int CToDoListWnd::GetSelToDoCtrl() const 
{ 
	if (m_tabCtrl.GetSafeHwnd()) 
		return m_tabCtrl.GetCurSel(); 
	else
		return -1;
}

BOOL CToDoListWnd::VerifyTaskListOpen(int nIndex, BOOL bWantNotifyDueTasks)
{
	if (!m_mgrToDoCtrls.IsLoaded(nIndex))
	{
		CFilteredToDoCtrl& tdc = GetToDoCtrl(nIndex);
		CString sFilePath = tdc.GetFilePath();

		TSM_TASKLISTINFO storageInfo;

		if (m_mgrToDoCtrls.GetStorageDetails(nIndex, storageInfo))
		{
			sFilePath = storageInfo.EncodeInfo(Prefs().GetSaveStoragePasswords());
		}

		if (OpenTaskList(&tdc, sFilePath, &storageInfo) == TDCF_SUCCESS)
		{
			// make sure hidden windows stay hidden
			if (nIndex != GetSelToDoCtrl())
				tdc.ShowWindow(SW_HIDE);

			// notify readonly
			Resize();

			m_mgrToDoCtrls.CheckNotifyReadonly(nIndex);
			m_mgrToDoCtrls.SetStorageDetails(nIndex, storageInfo);
			m_mgrToDoCtrls.SetLoaded(nIndex);
			m_mgrToDoCtrls.UpdateTabItemText(nIndex);

			if (bWantNotifyDueTasks)
				DoDueTaskNotification(nIndex, Prefs().GetNotifyDueByOnLoad());

			return TRUE;
		}

		return FALSE;
	}

	return TRUE;
}

BOOL CToDoListWnd::SelectToDoCtrl(int nIndex, BOOL bCheckPassword, int nNotifyDueTasksBy)
{
	ASSERT (nIndex >= 0);
	ASSERT (nIndex < GetTDCCount());
	
	// load and show the chosen item
	// we don't need to do a 'open' due task notification if the caller
	// has asked for a notification anyway
	if (!m_bClosing)
	{
		// if the tasklist is not loaded and we verify its loading
		// then we know that the password (if there is one) has been 
		// verified and doesn't need checking again
		if (!m_mgrToDoCtrls.IsLoaded(nIndex) )
		{
			if (!VerifyTaskListOpen(nIndex, nNotifyDueTasksBy == -1))
			{
				// TODO
				return FALSE;
			}
			else
			{
				bCheckPassword = FALSE;
			}
		}
	}

	// validate password first if necessary
	if (bCheckPassword && !VerifyToDoCtrlPassword(nIndex))
		return FALSE;
	
	int nCurSel = GetSelToDoCtrl(); // cache this

	// resize tdc first
	CFilteredToDoCtrl& tdcShow = GetToDoCtrl(nIndex);
	CRect rTDC;
	
	if (CalcToDoCtrlRect(rTDC))
		tdcShow.MoveWindow(rTDC);
	
	m_tabCtrl.SetCurSel(nIndex); // this changes the selected CToDoCtrl
	m_tabCtrl.UpdateWindow();
	
	if (!m_bClosing)
		UpdateActiveToDoCtrlPreferences();
	
	const CPreferencesDlg& userPrefs = Prefs();

	// always come out of 'Maximise Comments'
	if (m_nMaxState == TDCMS_MAXCOMMENTS)
		m_nMaxState = TDCMS_NORMAL;
	
	tdcShow.SetMaximizeState(m_nMaxState);
	tdcShow.EnableWindow(TRUE);
	tdcShow.SetFocusToTasks();
	tdcShow.ShowWindow(SW_SHOW);

	// if the tasklist is encrypted and todolist always prompts for password
	// then disable Flip3D and Aero Peek
	UpdateAeroFeatures();

	// before hiding the previous selection
	if (nCurSel != -1 && nCurSel != nIndex)
	{
		CFilteredToDoCtrl& tdcHide = GetToDoCtrl(nCurSel);
		
		tdcHide.ShowWindow(SW_HIDE);
		tdcHide.EnableWindow(FALSE);
	}
	
	tdcShow.Invalidate(TRUE);
	tdcShow.UpdateWindow();
	
	if (!m_bClosing)
	{
		if (userPrefs.GetReadonlyReloadOption() != RO_NO)
			OnTimerReadOnlyStatus(nIndex);
		
		if (userPrefs.GetTimestampReloadOption() != RO_NO)
			OnTimerTimestampChange(nIndex);
		
		if (tdcShow.IsSourceControlled())
			OnTimerCheckoutStatus(nIndex);
		
		// update various dependencies
		UpdateCaption();
		UpdateStatusbar();
		UpdateMenuIconMgrSourceControlStatus();
		UpdateCwd();
		
		RefreshFilterBarControls();
		RefreshFindTasksListData();
		RefreshPauseTimeTracking();

		DoDueTaskNotification(GetSelToDoCtrl(), nNotifyDueTasksBy);
	}

	return TRUE;
}

void CToDoListWnd::UpdateAeroFeatures()
{
#ifdef _DEBUG
	BOOL bEnable = !GetToDoCtrl().IsEncrypted();
#else
	BOOL bEnable = (!m_bPasswordPrompting || !GetToDoCtrl().IsEncrypted());
#endif

	// Disable peek and other dynamic views if the active tasklist is encrypted
	GraphicsMisc::EnableFlip3D(*this, bEnable);

	if (!GraphicsMisc::EnableAeroPeek(*this, bEnable))
		GraphicsMisc::ForceIconicRepresentation(*this, !bEnable);
}

void CToDoListWnd::UpdateActiveToDoCtrlPreferences()
{
	// check if this has already been done since the last userPrefs change
	int nSel = GetSelToDoCtrl();
	
	if (m_mgrToDoCtrls.GetNeedsPreferenceUpdate(nSel))
	{
		CFilteredToDoCtrl& tdc = GetToDoCtrl(nSel);

		UpdateToDoCtrlPreferences(&tdc);

		// and filter bar relies on this tdc's visible columns
		m_filterBar.SetVisibleFilters(tdc.GetVisibleFilterFields());
		
		// reset flag
		m_mgrToDoCtrls.SetNeedsPreferenceUpdate(nSel, FALSE);
	}
}

void CToDoListWnd::UpdateToDoCtrlPreferences(CFilteredToDoCtrl* pTDC)
{
	const CPreferencesDlg& userPrefs = Prefs();
	CFilteredToDoCtrl& tdc = *pTDC;

	CTDCToDoCtrlPreferenceHelper::UpdateToDoCtrl(userPrefs, m_tdiDefault, 
												m_bShowProjectName, m_bShowTreeListBar, 
												m_fontTree, m_fontComments, tdc);
}

void CToDoListWnd::OnSaveall() 
{
	SaveAll(TDLS_INCLUDEUNSAVED | TDLS_FLUSH);
}

void CToDoListWnd::OnUpdateSaveall(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_mgrToDoCtrls.AnyIsModified());
}

void CToDoListWnd::OnCloseall() 
{
	// save first
	TDC_FILE nSaveAll = SaveAll(TDLS_INCLUDEUNSAVED | TDLS_CLOSINGTASKLISTS | TDLS_FLUSH);

	if (nSaveAll != TDCF_SUCCESS)
		return;

	// remove tasklists
	m_findDlg.DeleteAllResults();
	m_dlgTimeTracker.RemoveAllTasklists();

	int nCtrl = GetTDCCount();
	
	while (nCtrl--)
		m_mgrToDoCtrls.RemoveToDoCtrl(nCtrl, TRUE);

	// if empty then create a new dummy item		
	if (!GetTDCCount())
		CreateNewTaskList(FALSE);
	else
		Resize();

	m_filterBar.Invalidate();
}

void CToDoListWnd::OnUpdateCloseall(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetTDCCount());
}

BOOL CToDoListWnd::DoQueryEndSession(BOOL bQuery, BOOL bEnding)
{
	HWND hWnd = GetSafeHwnd();

	// what we do here depends on whether we're on Vista or not
	// we test for this by trying to load the new API functions
	if (bQuery)
	{
		CEnString sReason(IDS_SHUTDOWNBLOCKREASON);

		// if Vista and handling WM_QUERYENDSESSION
		// we register our reason and return TRUE to
		// get more time to clean up in WM_ENDSESSION
		if (Misc::ShutdownBlockReasonCreate(hWnd, sReason))
			return TRUE;

		// else we're XP so we return TRUE to let shutdown continue
		return TRUE;
	}

	// else do a proper shutdown
	m_bEndingSession = TRUE;

	DoExit(FALSE, bEnding);
		
	// cleanup our shutdown reason if not handled in DoExit
	Misc::ShutdownBlockReasonDestroy(hWnd);

	// and return anything because it's ignored
	return TRUE;
}

BOOL CToDoListWnd::OnQueryEndSession() 
{
	if (!CFrameWnd::OnQueryEndSession())
		return FALSE;
	
	return DoQueryEndSession(TRUE, FALSE);
}

void CToDoListWnd::OnEndSession(BOOL bEnding) 
{
	CFrameWnd::OnEndSession(bEnding);

	DoQueryEndSession(FALSE, bEnding);
}

void CToDoListWnd::OnExit()
{
	DoExit();
}

void CToDoListWnd::OnMinimizeToTray()
{
	MinimizeToTray();
}

BOOL CToDoListWnd::DoExit(BOOL bRestart, BOOL bClosingWindows) 
{
	ASSERT (!(bClosingWindows && bRestart));
	
    // save all first to ensure new tasklists get reloaded on startup
	DWORD dwSaveFlags = TDLS_INCLUDEUNSAVED | TDLS_CLOSINGTASKLISTS | TDLS_FLUSH;

	if (bClosingWindows)
		dwSaveFlags |= TDLS_CLOSINGWINDOWS;

	TDC_FILE nSaveAll = SaveAll(dwSaveFlags);

	if (nSaveAll != TDCF_SUCCESS)
        return FALSE; // user cancelled
	
	// save settings before we close the tasklists
	// to snapshot the currently open tasklists
	SaveSettings(); 

	m_bClosing = TRUE;
	
	// hide the window as soon as possible so users do not
	// see the machinations of closing down
	BOOL bWasVisible = IsWindowVisible();
	BOOL bWasMinimized = IsIconic();
	BOOL bWasMaximized = IsZoomed();

	if (m_dlgTimeTracker.IsAlwaysOnTop())
		m_dlgTimeTracker.ShowWindow(SW_HIDE);
	
	if (bWasVisible)
	{
		if (m_findDlg.GetSafeHwnd())
			m_findDlg.ShowWindow(SW_HIDE);

		ShowWindow(SW_HIDE);
	}
	
	// remove tasklists
	int nCtrl = GetTDCCount();
		
	while (nCtrl--)
		VERIFY(CloseToDoCtrl(nCtrl)); // shouldn't fail now
	
	// if there are any still open then the user must have cancelled else destroy the window
	ASSERT (GetTDCCount() == 0);
	
	if (GetTDCCount() == 0)
	{
		delete m_pPrefs;
		m_pPrefs = NULL;

		// wait here until all remaining due task threads are done
		while (m_nNumDueTaskThreads)
		{
			Misc::ProcessMsgLoop();
			Sleep(50);
		}

		m_mgrImportExport.Release();
		m_tbHelper.Release();
		m_mgrShortcuts.Release();
		m_mgrImportExport.Release();
		m_mgrUIExtensions.Release();
		m_mgrStorage.Release();
			
		CFocusWatcher::Release();
		CMouseWheelMgr::Release();
		CEditShortcutMgr::Release();

		m_dlgTimeTracker.DestroyWindow();
		m_reminders.DestroyWindow();

		// cleanup our shutdown reason
		Misc::ShutdownBlockReasonDestroy(*this);

		DestroyWindow();
		
		if (bRestart)
		{
			CString sParams = AfxGetApp()->m_lpCmdLine;
			sParams += CEnCommandLineInfo::FormatSwitch(SWITCH_RESTART, Misc::Format(::GetCurrentProcessId()));
			
			if (FileMisc::Run(NULL, FileMisc::GetModuleFilePath(), sParams) <= 32)
			{
				// TODO
			}
		}

		return TRUE;
	}

	// cancel
	m_bClosing = FALSE;

	if (bWasVisible)
	{
		if (bWasMinimized)
		{
			ShowWindow(SW_SHOWMINIMIZED);
		}
		else if (bWasMaximized)
		{
			ShowWindow(SW_SHOWMAXIMIZED);
		}
		else
		{
			ShowWindow(SW_SHOW);
		}
	}

	return FALSE;
}

void CToDoListWnd::OnImportTasklist() 
{
	BOOL bShowDlg = TRUE;
	
	while (bShowDlg)
	{
		CTDLImportDialog dialog(m_mgrImportExport, GetToDoCtrl().IsReadOnly());

		if (dialog.DoModal() == IDOK)
		{
			// check file can be opened
			TDLID_IMPORTTO nImportTo = dialog.GetImportTo();
			int nImporter = dialog.GetImporterIndex();
			BOOL bFromClipboard = dialog.GetImportFromClipboard();
			CString sImportFrom = (bFromClipboard ? dialog.GetImportClipboardText() : 
													dialog.GetImportFilePath());

			// Reshow dialog on error
			bShowDlg = !ImportTasks(bFromClipboard, sImportFrom, nImporter, nImportTo);
		}
		else
		{
			bShowDlg = FALSE; // cancelled
		}
	}
}

BOOL CToDoListWnd::ImportTasks(BOOL bFromClipboard, const CString& sImportFrom,
								int nImporter, TDLID_IMPORTTO nImportTo) 
{
	CString sImportPath;

	if (bFromClipboard)
	{
		sImportPath = FileMisc::GetTempFilePath(_T("ToDoList.import"), _T("txt"));
		FileMisc::SaveFile(sImportPath, sImportFrom, SFEF_UTF16);
	}
	else
	{
		sImportPath = sImportFrom;
	}
	ASSERT(!sImportPath.IsEmpty());

	// load/import tasks
	DOPROGRESS(IDS_IMPORTPROGRESS);

	// do the import
	CTaskFile tasks;
	UINT nIcon = 0, nMessageID = 0;

	switch (m_mgrImportExport.ImportTaskList(sImportPath, &tasks, nImporter))
	{
	case IIR_CANCELLED:
		break;

	case IIR_SUCCESS:
		if (tasks.GetTaskCount())
		{
			if (nImportTo == TDIT_NEWTASKLIST)
				VERIFY(CreateNewTaskList(FALSE));

			CFilteredToDoCtrl& tdc = GetToDoCtrl(); // newly created tasklist
			TDC_INSERTWHERE nWhere = TDC_INSERTATTOP;

			switch (nImportTo)
			{
			case TDIT_ONSELECTEDTASK:
				{
					switch (Prefs().GetNewSubtaskPos())
					{
					case PUIP_TOP:
						nWhere = TDC_INSERTATTOPOFSELTASK;
						break;

					case PUIP_BOTTOM:
						nWhere = TDC_INSERTATBOTTOMOFSELTASK;
						break;
					}
				}
				break;

			case TDIT_BELOWSELECTEDTASK:
				nWhere = TDC_INSERTAFTERSELTASK;
				break;

			case TDIT_NEWTASKLIST:
			case TDIT_TOPTASKLIST:
				// as-is
				break;
			}

			if (nImportTo == TDIT_NEWTASKLIST)
			{
				VERIFY(tdc.InsertTasks(tasks, nWhere, FALSE));
				tdc.SetProjectName(tasks.GetProjectName());
			}
			else
			{
				VERIFY(tdc.InsertTasks(tasks, nWhere, TRUE));
			}

			UpdateCaption();
			break;
		}
		// else fall thru

	case IIR_SOMEFAILED:
		if (bFromClipboard)
			nMessageID = (tasks.GetTaskCount() ? IDS_IMPORTFROMCB_SOMETASKSFAILED : IDS_IMPORTFROMCB_NOTASKS);
		else
			nMessageID = (tasks.GetTaskCount() ? IDS_IMPORTFILE_SOMETASKSFAILED : IDS_IMPORTFILE_NOTASKS);

		nIcon = MB_ICONWARNING;
		break;

	case IIR_BADFILE:
		if (!bFromClipboard)
		{
			nMessageID = IDS_IMPORTFILE_CANTOPEN;
			nIcon = MB_ICONERROR;
			break;
		}
		// else fall thru

	case IIR_BADFORMAT:
		nMessageID = (bFromClipboard ? IDS_IMPORTFROMCB_BADFORMAT : IDS_IMPORTFILE_BADFORMAT);
		nIcon = MB_ICONERROR;
		break;

	case IIR_BADINTERFACE:
		// TODO
		break;
	}

	if (nMessageID)
	{
		AfxMessageBox(CEnString(nMessageID, sImportFrom), (MB_OK | nIcon));
		return FALSE;
	}

	return TRUE;
}

void CToDoListWnd::OnSetPriority(UINT nCmdID) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	if (!tdc.IsReadOnly() && tdc.HasSelection())
	{
		if (nCmdID == ID_EDIT_SETPRIORITYNONE) 
			tdc.SetSelectedTaskPriority(-2);
		else
			tdc.SetSelectedTaskPriority(nCmdID - ID_EDIT_SETPRIORITY0);
	}
}

void CToDoListWnd::OnUpdateSetPriority(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	int nSelCount = tdc.GetSelectedCount();
	
	pCmdUI->Enable(!tdc.IsReadOnly() && nSelCount);
	
	int nPriority = pCmdUI->m_nID - ID_EDIT_SETPRIORITY0;
	
	if (pCmdUI->m_nID == ID_EDIT_SETPRIORITYNONE)
		nPriority = -2;
	
	pCmdUI->SetCheck(nPriority == tdc.GetSelectedTaskPriority());
}

void CToDoListWnd::OnEditAddFileLink() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	if (!tdc.IsReadOnly() && (tdc.GetSelectedCount() == 1))
	{
		int nNumFiles = tdc.GetSelectedTaskFileRefCount();
		
		CPreferences prefs;
		CFileOpenDialog dialog(IDS_SETFILEREF_TITLE, 
			NULL, 
			((nNumFiles == 1) ? tdc.GetSelectedTaskFileRef(0) : _T("")), 
			(EOFN_DEFAULTOPEN | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT), 
			CEnString(IDS_ALLFILEFILTER));
		
		if (dialog.DoModal(prefs) == IDOK)
		{
			CStringArray aFiles;
			
			if (dialog.GetPathNames(aFiles))
				tdc.AppendSelectedTaskFileRefs(aFiles);
		}
	}
}

void CToDoListWnd::OnUpdateEditAddFileLink(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	pCmdUI->Enable(!tdc.IsReadOnly() && (tdc.GetSelectedCount() == 1));
}

void CToDoListWnd::OnEditOpenfileref(UINT nCmdID) 
{
	int nFile = (nCmdID - ID_EDIT_OPENFILEREF1);
	ASSERT(nFile < 16);

	if (nFile < 16)
		GetToDoCtrl().GotoSelectedTaskFileRef(nFile);
}

void CToDoListWnd::OnUpdateEditOpenfileref(CCmdUI* pCmdUI) 
{
	// we do all the work from the first item
	if (pCmdUI->m_pMenu && (pCmdUI->m_nID == ID_EDIT_OPENFILEREF1))
	{
		// remove all existing items
		for (int nFile = 0; nFile < 16; nFile++)
		{
			if (pCmdUI->m_pMenu->DeleteMenu((ID_EDIT_OPENFILEREF1 + nFile), MF_BYCOMMAND))
				pCmdUI->m_nIndexMax--;
		}
	
		// re-add the selected task's file refs
		CStringArray aFileRefs;

		if (GetToDoCtrl().GetSelectedTaskFileRefs(aFileRefs))
		{
			// restrict file lengths to 250 pixels
			CClientDC dc(this);

			for (int nFile = 0; nFile < aFileRefs.GetSize(); nFile++)
			{
				CEnString sFileRef(aFileRefs[nFile]);
				ASSERT(!sFileRef.IsEmpty());

				sFileRef.FormatDC(&dc, 250, ES_PATH);

				pCmdUI->m_pMenu->InsertMenu(pCmdUI->m_nIndex++, 
											(MF_BYPOSITION | MF_STRING), 
											(ID_EDIT_OPENFILEREF1 + nFile), 
											sFileRef);
			}

			pCmdUI->m_nIndex--;
			pCmdUI->m_nIndexMax += aFileRefs.GetSize();
		}
		else // restore the generic text and disable
		{
			pCmdUI->m_pMenu->InsertMenu(pCmdUI->m_nIndex, 
										(MF_BYPOSITION | MF_STRING | MF_GRAYED), 
										ID_EDIT_OPENFILEREF1, 
										CEnString(IDS_EDIT_OPENFILEREF));
		}
	}
}

void CToDoListWnd::PopulateToolArgs(USERTOOLARGS& args) const
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
		
	args.sTasklist = tdc.GetFilePath();
	args.sTaskTitle = tdc.GetSelectedTaskTitle();
	args.sTaskExtID = tdc.GetSelectedTaskExtID();
	args.sTaskComments = tdc.GetSelectedTaskComments();
	args.sTaskFileLink = tdc.GetSelectedTaskFileRef(0);
	args.sTaskAllocBy = tdc.GetSelectedTaskAllocBy();
	
	CDWordArray aIDs;
	DWORD dwTemp;
	
	if (tdc.GetSelectedTaskIDs(aIDs, dwTemp, FALSE))
		args.sTaskIDs = Misc::FormatArray(aIDs, _T("|"));
	
	CStringArray aAllocTo;
	
	if (tdc.GetSelectedTaskAllocTo(aAllocTo))
		args.sTaskAllocTo = Misc::FormatArray(aAllocTo, _T("|"));

	tdc.GetSelectedTaskCustomAttributeData(args.mapTaskCustData, TRUE);
}

LRESULT CToDoListWnd::OnPreferencesTestTool(WPARAM /*wp*/, LPARAM lp)
{
	USERTOOL* pTool = (USERTOOL*)lp;
	
	if (pTool)
	{
		USERTOOLARGS args;
		PopulateToolArgs(args);

		CTDCToolsHelper th(Prefs().GetEnableTDLExtension(), ID_TOOLS_USERTOOL1, MAX_NUM_TOOLS);
		th.TestTool(*pTool, args, m_pPrefs);
	}
	
	return 0;
}

LRESULT CToDoListWnd::OnPreferencesClearMRU(WPARAM /*wp*/, LPARAM /*lp*/)
{
	m_mruList.RemoveAll();
	m_mruList.WriteList(CPreferences());
	
	return 0;
}

void CToDoListWnd::PrepareSortMenu(CMenu* pMenu)
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
		
	if (Prefs().GetShowEditMenuAsColumns())
	{
		int nCountLastSep = 0;
		
		for (int nItem = 0; nItem < (int)pMenu->GetMenuItemCount(); nItem++)
		{
			BOOL bDelete = FALSE;
			BOOL bIsSeparator = FALSE;

			UINT nMenuID = pMenu->GetMenuItemID(nItem);
			
			switch (nMenuID)
			{
			case ID_SORT_BYALLOCBY:		bDelete = !tdc.IsColumnShowing(TDCC_ALLOCBY);		break; 
			case ID_SORT_BYALLOCTO:		bDelete = !tdc.IsColumnShowing(TDCC_ALLOCTO);		break; 
			case ID_SORT_BYCATEGORY:	bDelete = !tdc.IsColumnShowing(TDCC_CATEGORY);		break; 
			case ID_SORT_BYCOST:		bDelete = !tdc.IsColumnShowing(TDCC_COST);			break;
			case ID_SORT_BYCREATEDBY:	bDelete = !tdc.IsColumnShowing(TDCC_CREATEDBY);		break; 
			case ID_SORT_BYCREATIONDATE:bDelete = !tdc.IsColumnShowing(TDCC_CREATIONDATE);	break; 
			case ID_SORT_BYDEPENDENCY:	bDelete = !tdc.IsColumnShowing(TDCC_DEPENDENCY);	break; 
			case ID_SORT_BYDONE:		bDelete = !tdc.IsColumnShowing(TDCC_DONE);			break; 
			case ID_SORT_BYDONEDATE:	bDelete = !tdc.IsColumnShowing(TDCC_DONEDATE);		break; 
			case ID_SORT_BYDUEDATE:		bDelete = !tdc.IsColumnShowing(TDCC_DUEDATE);		break; 
			case ID_SORT_BYEXTERNALID:	bDelete = !tdc.IsColumnShowing(TDCC_EXTERNALID);	break; 
			case ID_SORT_BYFILEREF:		bDelete = !tdc.IsColumnShowing(TDCC_FILEREF);		break; 
			case ID_SORT_BYFLAG:		bDelete = !tdc.IsColumnShowing(TDCC_FLAG);			break; 
			case ID_SORT_BYICON:		bDelete = !tdc.IsColumnShowing(TDCC_ICON);			break; 
			case ID_SORT_BYID:			bDelete = !tdc.IsColumnShowing(TDCC_ID);			break; 
			case ID_SORT_BYMODIFYDATE:	bDelete = !tdc.IsColumnShowing(TDCC_LASTMOD);		break; 
			case ID_SORT_BYPATH:		bDelete = !tdc.IsColumnShowing(TDCC_PATH);			break; 
			case ID_SORT_BYPERCENT:		bDelete = !tdc.IsColumnShowing(TDCC_PERCENT);		break; 
			case ID_SORT_BYPOSITION:	bDelete = !tdc.IsColumnShowing(TDCC_POSITION);		break; 
			case ID_SORT_BYPRIORITY:	bDelete = !tdc.IsColumnShowing(TDCC_PRIORITY);		break; 
			case ID_SORT_BYRECENTEDIT:	bDelete = !tdc.IsColumnShowing(TDCC_RECENTEDIT);	break; 
			case ID_SORT_BYRECURRENCE:	bDelete = !tdc.IsColumnShowing(TDCC_RECURRENCE);	break; 
			case ID_SORT_BYREMAINING:	bDelete = !tdc.IsColumnShowing(TDCC_REMAINING);		break; 
			case ID_SORT_BYRISK:		bDelete = !tdc.IsColumnShowing(TDCC_RISK);			break; 
			case ID_SORT_BYSTARTDATE:	bDelete = !tdc.IsColumnShowing(TDCC_STARTDATE);		break; 
			case ID_SORT_BYSTATUS:		bDelete = !tdc.IsColumnShowing(TDCC_STATUS);		break; 
			case ID_SORT_BYSUBTASKDONE:	bDelete = !tdc.IsColumnShowing(TDCC_SUBTASKDONE);	break; 
			case ID_SORT_BYTAG:			bDelete = !tdc.IsColumnShowing(TDCC_TAGS);			break; 
			case ID_SORT_BYTIMEEST:		bDelete = !tdc.IsColumnShowing(TDCC_TIMEEST);		break; 
			case ID_SORT_BYTIMESPENT:	bDelete = !tdc.IsColumnShowing(TDCC_TIMESPENT);		break; 
			case ID_SORT_BYTIMETRACKING:bDelete = !tdc.IsColumnShowing(TDCC_TRACKTIME);		break; 
			case ID_SORT_BYVERSION:		bDelete = !tdc.IsColumnShowing(TDCC_VERSION);		break; 
	//		case ID_SORT_BYCOLOR: bDelete = (Prefs().GetTextColorOption() != COLOROPT_DEFAULT); break; 


			case ID_SEPARATOR: 
				bIsSeparator = TRUE;
				bDelete = (nCountLastSep == 0);
				nCountLastSep = 0;
				break;

			default: bDelete = FALSE; break; 
			}

			// delete the item else increment the count since the last separator
			if (bDelete)
			{
				pMenu->DeleteMenu(nItem, MF_BYPOSITION);
				nItem--;
			}
			else if (!bIsSeparator)
				nCountLastSep++;
		}
	}

	// custom sort columns

	// first delete all custom columns and the related separator
	int nPosUnsorted = -1;

	for (int nItem = 0; nItem < (int)pMenu->GetMenuItemCount(); nItem++)
	{
		UINT nMenuID = pMenu->GetMenuItemID(nItem);

		if (nMenuID >= ID_SORT_BYCUSTOMCOLUMN_FIRST && nMenuID <= ID_SORT_BYCUSTOMCOLUMN_LAST)
		{
			pMenu->DeleteMenu(nItem, MF_BYPOSITION);
			nItem--;
		}
	}

	// separator is just before the separator before 'unsorted entry'
	int nInsert = CEnMenu::GetMenuItemPos(pMenu->GetSafeHmenu(), ID_SORT_BYNONE) - 1;
	ASSERT(nInsert >= 0);

	// delete separator if exist
	if (nInsert > 0 && pMenu->GetMenuItemID(nInsert - 1) == 0)
	{
		nInsert--;
		pMenu->DeleteMenu(nInsert, MF_BYPOSITION);
	}

	// then re-add
	CTDCCustomAttribDefinitionArray aAttribDefs;

	if (tdc.GetCustomAttributeDefs(aAttribDefs))
	{
		// re-add separator on demand
		BOOL bWantSep = TRUE;

		for (int nCol = 0; nCol < aAttribDefs.GetSize(); nCol++)
		{
			const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = aAttribDefs[nCol];

			if (attribDef.bEnabled && attribDef.SupportsFeature(TDCCAF_SORT))
			{
				if (bWantSep)
				{
					bWantSep = FALSE;
					pMenu->InsertMenu(nInsert, MF_BYPOSITION);
					nInsert++;
				}

				UINT nMenuID = (attribDef.GetColumnID() - TDCC_CUSTOMCOLUMN_FIRST) + ID_SORT_BYCUSTOMCOLUMN_FIRST;
				CEnString sColumn(IDS_CUSTOMCOLUMN, attribDef.sLabel);

				pMenu->InsertMenu(nInsert, MF_BYPOSITION, nMenuID, sColumn);
				nInsert++;
			}
		}
	}
}

void CToDoListWnd::PrepareEditMenu(CMenu* pMenu)
{
	if (!Prefs().GetShowEditMenuAsColumns())
		return;

	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	int nCountLastSep = 0;
	
	for (int nItem = 0; nItem < (int)pMenu->GetMenuItemCount(); nItem++)
	{
		BOOL bDelete = FALSE;
		BOOL bIsSeparator = FALSE;

		UINT nMenuID = pMenu->GetMenuItemID(nItem);
		
		switch (nMenuID)
		{
		case -1: // its a popup so recursively handle it
			{
				CMenu* pPopup = pMenu->GetSubMenu(nItem);

				if (pPopup)
				{
					PrepareEditMenu(pPopup);
				
					// if the popup is now empty remove it too
					bDelete = !pPopup->GetMenuItemCount();
				}
			}
			break;
			
		case ID_EDIT_TASKCOLOR:
		case ID_EDIT_CLEARTASKCOLOR:
			bDelete = !((Prefs().GetTextColorOption() == COLOROPT_DEFAULT) ||
						tdc.IsEditFieldShowing(TDCA_COLOR));
			break;
			
        case ID_EDIT_DECTASKPRIORITY:
        case ID_EDIT_INCTASKPRIORITY:
		case ID_EDIT_SETPRIORITYNONE: 
        case ID_EDIT_SETPRIORITY0:
        case ID_EDIT_SETPRIORITY1:
        case ID_EDIT_SETPRIORITY2:
        case ID_EDIT_SETPRIORITY3:
        case ID_EDIT_SETPRIORITY4:
        case ID_EDIT_SETPRIORITY5:
        case ID_EDIT_SETPRIORITY6:
        case ID_EDIT_SETPRIORITY7:
        case ID_EDIT_SETPRIORITY8:
        case ID_EDIT_SETPRIORITY9:
        case ID_EDIT_SETPRIORITY10:
			bDelete = !tdc.IsColumnOrEditFieldShowing(TDCC_PRIORITY, TDCA_PRIORITY);
			break;
			
		case ID_EDIT_OFFSETDATES:
			bDelete = !(tdc.IsColumnOrEditFieldShowing(TDCC_STARTDATE, TDCA_STARTDATE) ||
						tdc.IsColumnOrEditFieldShowing(TDCC_DUEDATE, TDCA_DUEDATE) || 
						tdc.IsColumnOrEditFieldShowing(TDCC_DONEDATE, TDCA_DONEDATE));
			break;
			
        case ID_EDIT_CLOCK_TASK:
			bDelete = !(tdc.IsColumnShowing(TDCC_TRACKTIME) ||
						tdc.IsColumnOrEditFieldShowing(TDCC_TIMESPENT, TDCA_TIMESPENT));
			break;

        case ID_SHOWTIMELOGFILE:
        case ID_ADDTIMETOLOGFILE:
			bDelete = !((tdc.IsColumnShowing(TDCC_TRACKTIME) ||
						tdc.IsColumnOrEditFieldShowing(TDCC_TIMESPENT, TDCA_TIMESPENT)) &&
						Prefs().GetLogTimeTracking());
			break;
			
        case ID_EDIT_DECTASKPERCENTDONE:	
        case ID_EDIT_INCTASKPERCENTDONE:	
			bDelete = !tdc.IsColumnOrEditFieldShowing(TDCC_PERCENT, TDCA_PERCENT); 
			break;

        case ID_EDIT_OPENFILEREF1:
		case ID_EDIT_SETFILEREF:			
			bDelete = !tdc.IsColumnOrEditFieldShowing(TDCC_FILEREF, TDCA_FILEREF); 
			break;

        case ID_EDIT_FLAGTASK:				
			bDelete = !tdc.IsColumnShowing(TDCC_FLAG); 
			break;

        case ID_EDIT_RECURRENCE:			
			bDelete = !tdc.IsColumnOrEditFieldShowing(TDCC_RECURRENCE, TDCA_RECURRENCE); 
			break;

        case ID_EDIT_GOTODEPENDENCY:		
			bDelete = !tdc.IsColumnOrEditFieldShowing(TDCC_DEPENDENCY, TDCA_DEPENDENCY); 
			break;

        case ID_EDIT_SETTASKICON:			
        case ID_EDIT_CLEARTASKICON:	
			break;

		case ID_SEPARATOR: 
			bIsSeparator = TRUE;
			bDelete = (nCountLastSep == 0);
			nCountLastSep = 0;
			break;

		default: 
			break; 
		}

		// delete the item else increment the count since the last separator
		if (bDelete)
		{
			pMenu->DeleteMenu(nItem, MF_BYPOSITION);
			nItem--;
		}
		else if (!bIsSeparator)
			nCountLastSep++;
	}

	// make sure last item is not a separator
	int nLastItem = (int)pMenu->GetMenuItemCount() - 1;

	if (pMenu->GetMenuItemID(nLastItem) == 0)
		pMenu->DeleteMenu(nLastItem, MF_BYPOSITION);
}

void CToDoListWnd::OnViewNext() 
{
	if (GetTDCCount() < 2)
		return;
	
	int nNext = GetSelToDoCtrl() + 1;
	
	if (nNext >= GetTDCCount())
		nNext = 0;
	
	SelectToDoCtrl(nNext, TRUE, Prefs().GetNotifyDueByOnSwitch());
}

void CToDoListWnd::OnUpdateViewNext(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetTDCCount() > 1);
}

void CToDoListWnd::OnViewPrev() 
{
	if (GetTDCCount() < 2)
		return;
	
	int nPrev = GetSelToDoCtrl() - 1;
	
	if (nPrev < 0)
		nPrev = GetTDCCount() - 1;
	
	SelectToDoCtrl(nPrev, TRUE, Prefs().GetNotifyDueByOnSwitch());
}

void CToDoListWnd::OnUpdateViewPrev(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetTDCCount() > 1);
}

void CToDoListWnd::OnSysCommand(UINT nID, LPARAM lParam) 
{
	switch (nID)
	{
	case SC_MINIMIZE:
		// we don't minimize if we're going to be hiding to the system tray
		{
			m_hwndLastFocus = ::GetFocus();

			if (HasSysTrayOptions(STO_ONMINIMIZE, STO_ONMINCLOSE))
			{
				MinimizeToTray();
			}
			else
			{
				// SPECIAL FIX: Apparently when Ultramon hooks the minimize
				// button it ends up sending us a close message!
				ShowWindow(SW_MINIMIZE);
			}
			return; // eat it
		}

	case SC_HOTKEY:
		Show(FALSE);
		return;
		
	case SC_CLOSE:
		// don't allow closing whilst reloading tasklists
		if (m_bReloading)
			return;
		break;

	case SC_RESTORE:
	case SC_MAXIMIZE:
		CFrameWnd::OnSysCommand(nID, lParam);

		Resize();
		PostMessage(WM_APPRESTOREFOCUS, 0L, (LPARAM)m_hwndLastFocus);
		return;
	}

	CFrameWnd::OnSysCommand(nID, lParam);
}


void CToDoListWnd::OnUpdateImport(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

UINT CToDoListWnd::MapNewTaskPos(int nPos, BOOL bSubtask)
{
	if (!bSubtask) // task
	{
		switch (nPos)
		{
		case PUIP_TOP:		return ID_NEWTASK_ATTOP;
		case PUIP_BOTTOM:	return ID_NEWTASK_ATBOTTOM;
		case PUIP_BELOW:	return ID_NEWTASK_AFTERSELECTEDTASK;
			
		case PUIP_ABOVE: 
		default:			return ID_NEWTASK_BEFORESELECTEDTASK;
		}
	}
	else // subtask
	{
		if (nPos == PUIP_BOTTOM)
			return ID_NEWSUBTASK_ATBOTTOM;
		else
			return ID_NEWSUBTASK_ATTOP;
	}
}

UINT CToDoListWnd::GetNewTaskCmdID()
{
	return MapNewTaskPos(Prefs().GetNewTaskPos(), FALSE);
}

UINT CToDoListWnd::GetNewSubtaskCmdID()
{
	return MapNewTaskPos(Prefs().GetNewSubtaskPos(), TRUE);
}

void CToDoListWnd::OnNewtask() 
{
	// convert to users choice
	SendMessage(WM_COMMAND, GetNewTaskCmdID());
}

void CToDoListWnd::OnUpdateNewtask(CCmdUI* pCmdUI) 
{
	switch (GetNewTaskCmdID())
	{
	case ID_NEWTASK_ATTOPSELECTED:
		OnUpdateNewtaskAttopSelected(pCmdUI);
		break;

	case ID_NEWTASK_ATBOTTOMSELECTED:	
		OnUpdateNewtaskAtbottomSelected(pCmdUI);
		break;
	
	case ID_NEWTASK_AFTERSELECTEDTASK:
		OnUpdateNewtaskAfterselectedtask(pCmdUI);
		break;

	case ID_NEWTASK_BEFORESELECTEDTASK:
		OnUpdateNewtaskBeforeselectedtask(pCmdUI);
		break;

	case ID_NEWTASK_ATTOP:
		OnUpdateNewtaskAttop(pCmdUI);
		break;

	case ID_NEWTASK_ATBOTTOM:
		OnUpdateNewtaskAtbottom(pCmdUI);
		break;
	}
}

void CToDoListWnd::OnNewsubtask() 
{
	// convert to users choice
	SendMessage(WM_COMMAND, GetNewSubtaskCmdID());
}

void CToDoListWnd::OnUpdateNewsubtask(CCmdUI* pCmdUI) 
{
	switch (GetNewSubtaskCmdID())
	{
	case ID_NEWSUBTASK_ATTOP:
		OnUpdateNewsubtaskAttop(pCmdUI);
		break;

	case ID_NEWSUBTASK_ATBOTTOM:
		OnUpdateNewsubtaskAtBottom(pCmdUI);
		break;
	}
}

void CToDoListWnd::OnToolsCheckout() 
{
	int nSel = GetSelToDoCtrl();
	
	// sanity check
	if (!m_mgrToDoCtrls.CanCheckOut(nSel))
		return;
	
	CAutoFlag af(m_bSaving, TRUE);
	CString sCheckedOutTo;
	
	TDC_FILE nFileRes = m_mgrToDoCtrls.CheckOut(nSel, sCheckedOutTo);
	
	if (nFileRes == TDCF_SUCCESS)
	{
		// update UI
		UpdateCaption();
		UpdateMenuIconMgrSourceControlStatus();
	}
	else // failed -> notify user
	{
		CEnString sMessage, sFilePath(m_mgrToDoCtrls.GetFilePath(nSel));
		
		if ((nFileRes == TDCF_OTHER) && !sCheckedOutTo.IsEmpty())
		{
			sMessage.Format(IDS_CHECKEDOUTBYOTHER, sFilePath, sCheckedOutTo);
			MessageBox(sMessage, IDS_CHECKOUT_TITLE, MB_OK | MB_ICONEXCLAMATION);
		}
		else
		{
			HandleSaveTasklistError(nFileRes, sFilePath);
		}
	}
}

void CToDoListWnd::OnUpdateToolsCheckout(CCmdUI* pCmdUI) 
{
	int nSel = GetSelToDoCtrl();
	
	pCmdUI->Enable(m_mgrToDoCtrls.CanCheckOut(nSel));
}

void CToDoListWnd::OnToolsToggleCheckin() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	if (tdc.IsCheckedOut())
		OnToolsCheckin();
	else
		OnToolsCheckout();
}

void CToDoListWnd::OnUpdateToolsToggleCheckin(CCmdUI* pCmdUI) 
{
	int nSel = GetSelToDoCtrl();
	BOOL bEnable = m_mgrToDoCtrls.CanCheckInOut(nSel);
	
	pCmdUI->Enable(bEnable);
	pCmdUI->SetCheck(bEnable && GetToDoCtrl().IsCheckedOut() ? 1 : 0);
}

void CToDoListWnd::OnToolsCheckin() 
{
	int nSel = GetSelToDoCtrl();

	// sanity check
	if (!m_mgrToDoCtrls.CanCheckIn(nSel))
		return;
	
	CAutoFlag af(m_bSaving, TRUE);
	CFilteredToDoCtrl& tdc = GetToDoCtrl(nSel);

	tdc.Flush();
	
	// save modifications
	TDC_FILE nSave = TDCF_SUCCESS;

	if (tdc.IsModified())
	{
		if (Prefs().GetConfirmSaveOnExit())
		{
			CString sName = m_mgrToDoCtrls.GetFriendlyProjectName(nSel);
			CEnString sMessage(IDS_SAVEBEFORECHECKIN, sName);
			
			int nRet = MessageBox(sMessage, IDS_CHECKIN_TITLE, MB_YESNOCANCEL);
			
			switch (nRet)
			{
			case IDYES:
				nSave = SaveTaskList(nSel);
				break;
				
			case IDNO:
				ReloadTaskList(nSel, FALSE, FALSE);
				break;
				
			case IDCANCEL:
				return;
			}
		}
		else
		{
			nSave = SaveTaskList(nSel); 
		}
	}
	
	if (nSave == TDCF_SUCCESS)
	{
		TDC_FILE nCheckin = m_mgrToDoCtrls.CheckIn(nSel);
		
		if (nCheckin == TDCF_SUCCESS)
		{
			// Update UI
			UpdateCaption();
			UpdateMenuIconMgrSourceControlStatus();
		}
		else
		{
			HandleSaveTasklistError(nCheckin, tdc.GetFilePath());
		}
	}
	else
	{
		HandleSaveTasklistError(nSave, tdc.GetFilePath());
	}
	
	m_mgrToDoCtrls.SetLastCheckoutSucceeded(nSel, TRUE); // so we won't try to immediately check it out again
}

void CToDoListWnd::OnUpdateToolsCheckin(CCmdUI* pCmdUI) 
{
	int nSel = GetSelToDoCtrl();
	
	pCmdUI->Enable(m_mgrToDoCtrls.CanCheckIn(nSel));
}

void CToDoListWnd::OnExport() 
{
	const CPreferencesDlg& userPrefs = Prefs();
	
	int nTDCCount = GetTDCCount(), nSelTDC = GetSelToDoCtrl();
	ASSERT (nTDCCount >= 1);

	CTDLExportDlg dialog(m_mgrImportExport, 
						nTDCCount == 1, 
						GetToDoCtrl().GetView(),
						userPrefs.GetExportVisibleColsOnly(), 
						m_mgrToDoCtrls.GetFilePath(nSelTDC, FALSE), 
						userPrefs.GetAutoExportFolderPath());
	
	// keep showing the dialog until either the user
	// selects a filename which does not match a tasklist
	// or they confirm that they want to overwrite the tasklist
	CString sExportPath;
	BOOL bOverWrite = FALSE;
	int nFormat = -1;

	while (!bOverWrite)
	{
		if (dialog.DoModal() != IDOK)
			return;

		sExportPath = dialog.GetExportPath();
		nFormat = dialog.GetExportFormat();

		// interested in overwriting single files
		if (nTDCCount == 1 || !dialog.GetExportAllTasklists() || dialog.GetExportOneFile())
		{
			// check with user if they are about to override a tasklist
			if (m_mgrToDoCtrls.FindToDoCtrl(sExportPath) != -1)
			{
				UINT nRet = MessageBox(IDS_CONFIRM_EXPORT_OVERWRITE, 0, MB_YESNOCANCEL, sExportPath);

				if (nRet == IDCANCEL)
					return;

				// else
				bOverWrite = (nRet == IDYES);
			}
			else
				bOverWrite = TRUE; // nothing to fear
		}
		else // check all open tasklists
		{
			CString sFilePath, sExt = m_mgrImportExport.GetExporterFileExtension(nFormat);
		
			for (int nCtrl = 0; nCtrl < nTDCCount; nCtrl++)
			{
				CString sPath = m_mgrToDoCtrls.GetFilePath(nCtrl);
				CString sFName;
						
				FileMisc::SplitPath(sPath, NULL, NULL, &sFName);
				FileMisc::MakePath(sFilePath, NULL, sExportPath, sFName, sExt);

				if (m_mgrToDoCtrls.FindToDoCtrl(sFilePath) != -1)
				{
					UINT nRet = MessageBox(IDS_CONFIRM_EXPORT_OVERWRITE, 0, MB_YESNOCANCEL, sFilePath);

					if (nRet == IDCANCEL)
						return;

					// else
					bOverWrite = (nRet == IDYES);
					break;
				}
			}

			// no matches?
			bOverWrite = TRUE; // nothing to fear
		}
	}

	UpdateWindow();

	// export
	DOPROGRESS(IDS_EXPORTPROGRESS);
	
	BOOL bHtmlComments = (nFormat == EXPTOHTML);

	if (nTDCCount == 1 || !dialog.GetExportAllTasklists())
	{
		// set the html image folder to be the output path with
		// an different extension
		CString sImgFolder;
		
		if (bHtmlComments)
		{
			sImgFolder = sExportPath;
			FileMisc::ReplaceExtension(sImgFolder, _T("html_images"));
			FileMisc::DeleteFolderContents(sImgFolder, FMDF_ALLOWDELETEONREBOOT | FMDF_HIDDENREADONLY);
		}
		
		CFilteredToDoCtrl& tdc = GetToDoCtrl(nSelTDC);
		CTaskFile tasks;

		// Note: don't need to verify password if encrypted tasklist is active
		GetTasks(tdc, bHtmlComments, FALSE, dialog.GetTaskSelection(), tasks, sImgFolder);

		// add project name as report title
		CString sTitle = m_mgrToDoCtrls.GetFriendlyProjectName(nSelTDC);
		tasks.SetReportAttributes(sTitle);
		
		// save intermediate tasklist to file as required
		LogIntermediateTaskList(tasks, tdc.GetFilePath());

		if (m_mgrImportExport.ExportTaskList(&tasks, sExportPath, nFormat, FALSE))
		{
			// and preview
			if (userPrefs.GetPreviewExport())
				FileMisc::Run(*this, sExportPath);
		}
	} 
	// multiple tasklists
	else if (dialog.GetExportOneFile())
	{
		CMultiTaskFile taskFiles;
		
		for (int nCtrl = 0; nCtrl < nTDCCount; nCtrl++)
		{
			// verify password
			if (nCtrl != nSelTDC && !VerifyToDoCtrlPassword(nCtrl))
				continue;
			
			CFilteredToDoCtrl& tdc = GetToDoCtrl(nCtrl);
			tdc.LockWindowUpdate();
			
			// make sure it's loaded
			if (VerifyTaskListOpen(nCtrl, FALSE))
			{
				CTaskFile& tasks = taskFiles.GetTaskFile(nCtrl);
				tasks.Reset();
				
				// set the html image folder to be the output path with
				// an different extension
				CString sImgFolder;
				
				if (bHtmlComments)
				{
					sImgFolder = sExportPath;
					FileMisc::ReplaceExtension(sImgFolder, _T("html_images"));
				}
				
				GetTasks(tdc, bHtmlComments, FALSE, dialog.GetTaskSelection(), tasks, sImgFolder);
				
				// add project name as report title
				CString sTitle = m_mgrToDoCtrls.GetFriendlyProjectName(nCtrl);
				tasks.SetReportAttributes(sTitle);

				// save intermediate tasklist to file as required
				LogIntermediateTaskList(tasks, tdc.GetFilePath());
			}
			
			tdc.UnlockWindowUpdate();
		}
		
		Resize();
		
		if (m_mgrImportExport.ExportTaskLists(&taskFiles, sExportPath, nFormat, FALSE))
		{
			// and preview
			if (userPrefs.GetPreviewExport())
				FileMisc::Run(*this, sExportPath);
		}
	}
	else // separate files
	{
		CString sExt = m_mgrImportExport.GetExporterFileExtension(nFormat);
		
		for (int nCtrl = 0; nCtrl < nTDCCount; nCtrl++)
		{
			// verify password
			if (nCtrl != nSelTDC && !VerifyToDoCtrlPassword(nCtrl))
				continue;
			
			CFilteredToDoCtrl& tdc = GetToDoCtrl(nCtrl);
			tdc.LockWindowUpdate();
			
			// make sure it's loaded
			if (VerifyTaskListOpen(nCtrl, FALSE))
			{
				// build filepath if required (if exporter has an extension)
				CString sFilePath;
				
				if (!sExt.IsEmpty())
				{
					CString sPath = m_mgrToDoCtrls.GetFilePath(nCtrl);
					
					// if the file has not been saved before we use the tab text
					// and prompt the user to confirm
					if (sPath.IsEmpty())
					{
						sPath = m_mgrToDoCtrls.GetFilePath(nCtrl, FALSE);
						FileMisc::MakePath(sFilePath, NULL, sExportPath, sPath, sExt);
						
						CFileSaveDialog dlgFile(IDS_EXPORTFILEAS_TITLE,
												sExt, 
												sFilePath, 
												EOFN_DEFAULTSAVE,
												m_mgrImportExport.GetExporterFileFilter(nFormat));
						
						CPreferences prefs;

						if (dlgFile.DoModal(prefs) == IDOK)
							sFilePath = dlgFile.GetPathName();
						else
							continue; // next tasklist
					}
					else
					{
						CString sFName;

						FileMisc::SplitPath(sPath, NULL, NULL, &sFName);
						FileMisc::MakePath(sFilePath, NULL, sExportPath, sFName, sExt);
					}
				}
				
				// set the html image folder to be the output path with
				// an different extension
				CString sImgFolder;
				
				if (bHtmlComments)
				{
					sImgFolder = sFilePath;
					FileMisc::ReplaceExtension(sImgFolder, _T("html_images"));
				}
				
				CTaskFile tasks;
				GetTasks(tdc, bHtmlComments, FALSE, dialog.GetTaskSelection(), tasks, sImgFolder);
				
				// add project name as report title
				CString sTitle = m_mgrToDoCtrls.GetFriendlyProjectName(nCtrl);
				tasks.SetReportAttributes(sTitle);

				// save intermediate tasklist to file as required
				LogIntermediateTaskList(tasks, tdc.GetFilePath());

				m_mgrImportExport.ExportTaskList(&tasks, sFilePath, nFormat, FALSE);
			}
			
			tdc.UnlockWindowUpdate();
		}
	}
}

int CToDoListWnd::GetTasks(CFilteredToDoCtrl& tdc, BOOL bHtmlComments, BOOL bTransform, 
						  TSD_TASKS nWhatTasks, TDCGETTASKS& filter, DWORD dwSelFlags, 
						  CTaskFile& tasks, LPCTSTR szHtmlImageDir) const
{
	// preferences
	const CPreferencesDlg& userPrefs = Prefs();

	tasks.Reset();	
	tasks.SetProjectName(tdc.GetFriendlyProjectName());
//	tasks.SetCharSet(userPrefs.GetExportEncoding());
	
	// export flags
	filter.dwFlags |= TDCGTF_FILENAME;

	if (userPrefs.GetExportParentTitleCommentsOnly())
		filter.dwFlags |= TDCGTF_PARENTTITLECOMMENTSONLY;

	if (bHtmlComments)
	{
		filter.dwFlags |= TDCGTF_HTMLCOMMENTS;
		tasks.SetHtmlImageFolder(szHtmlImageDir);

		// And delete all existing images in that folder
		if (!Misc::IsEmpty(szHtmlImageDir))
			FileMisc::DeleteFolderContents(szHtmlImageDir, FMDF_ALLOWDELETEONREBOOT | FMDF_HIDDENREADONLY);

		if (bTransform)
		{
			ASSERT(bHtmlComments);
			filter.dwFlags |= TDCGTF_TRANSFORM;
		}
	}

	// get the tasks
	tdc.Flush();
	
	switch (nWhatTasks)
	{
	case TSDT_ALL:
		{
			// if there's a filter present then we toggle it off 
			// grab the tasks and then toggle it back on
			BOOL bNeedToggle = tdc.HasAnyFilter();

			if (bNeedToggle)
			{
				::LockWindowUpdate(tdc.GetSafeHwnd());
				tdc.ToggleFilter();
			}

			tdc.GetFilteredTasks(tasks, filter);

			if (bNeedToggle)
			{
				tdc.ToggleFilter();
				::LockWindowUpdate(NULL);
			}
		}
		break;

	case TSDT_FILTERED:
		// if no filter is present then this just gets the whole lot
		tdc.GetFilteredTasks(tasks, filter);
		break;

	case TSDT_SELECTED:
		tdc.GetSelectedTasks(tasks, filter, dwSelFlags);
		break;
	}
	
	// delete the HTML image folder if it is empty
	// this will fail if it is not empty.
	if (bHtmlComments && !Misc::IsEmpty(szHtmlImageDir))
		RemoveDirectory(szHtmlImageDir);
	
	return tasks.GetTaskCount();
}

int CToDoListWnd::GetTasks(CFilteredToDoCtrl& tdc, BOOL bHtmlComments, BOOL bTransform, 
							const CTaskSelectionDlg& taskSel, CTaskFile& tasks, LPCTSTR szHtmlImageDir) const
{
	DWORD dwSelFlags = 0;
    TSD_TASKS nWhatTasks = taskSel.GetWantWhatTasks();
	
	if (taskSel.GetWantSelectedTasks())
	{
		if (!taskSel.GetWantSelectedSubtasks()) 
			dwSelFlags |= TDCGSTF_NOTSUBTASKS;

		if (taskSel.GetWantSelectedParentTask())
			dwSelFlags |= TDCGSTF_IMMEDIATEPARENT;
	}

	TDC_GETTASKS nFilter = TDCGT_ALL;
	
	// build filter
	if (taskSel.GetWantCompletedTasks() && !taskSel.GetWantInCompleteTasks())
		nFilter = TDCGT_DONE;
		
	else if (!taskSel.GetWantCompletedTasks() && taskSel.GetWantInCompleteTasks())
		nFilter = TDCGT_NOTDONE;
		
	TDCGETTASKS filter(nFilter);

	// attributes to export
	switch (taskSel.GetAttributeOption())
	{
	case TSDA_ALL:
		break;

	case TSDA_VISIBLE:
		{
			// visible columns
			TDC::MapColumnsToAttributes(tdc.GetVisibleColumns(), filter.mapAttribs);

			if (taskSel.GetWantCommentsWithVisible())
				filter.mapAttribs.AddAttribute(TDCA_COMMENTS);
		}
		break;

	case TSDA_USER:
		taskSel.GetUserAttributes(filter.mapAttribs);
		filter.dwFlags |= TDCGTF_USERCOLUMNS;
		break;
	}
	
	// get the tasks
   return GetTasks(tdc, bHtmlComments, bTransform, nWhatTasks, filter, dwSelFlags, tasks, szHtmlImageDir);
}

void CToDoListWnd::OnUpdateExport(CCmdUI* pCmdUI) 
{
	// make sure at least one control has items
	int nCtrl = GetTDCCount();
	
	while (nCtrl--)
	{
		if (GetToDoCtrl().GetTaskCount())
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	
	// else
	pCmdUI->Enable(FALSE);
}

void CToDoListWnd::OnToolsTransformactivetasklist() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	int nSelTDC = GetSelToDoCtrl();

	// pass the project name as the title field
	CString sTitle = m_mgrToDoCtrls.GetFriendlyProjectName(nSelTDC);

	CTDLTransformDialog dialog(sTitle, tdc.GetView(), tdc.GetStylesheetPath());
	
	if (dialog.DoModal() != IDOK)
		return;
	
	CString sStylesheet = dialog.GetStylesheet();
	sTitle = dialog.GetTitle();
	
	CXslFile xsl;
	VERIFY (xsl.Load(sStylesheet));
	
	// output path
	CString sOutputPath(tdc.GetFilePath()); 
	{
		// Output file extension may be specified in the stylesheet
		CString sExtOut = xsl.GetOutputFileExtension();

		if (!sOutputPath.IsEmpty())
			FileMisc::ReplaceExtension(sOutputPath, sExtOut);

		CPreferences prefs;
		CFileSaveDialog dialog(IDS_SAVETRANSFORM_TITLE,
								sExtOut, 
								sOutputPath, 
								OFN_OVERWRITEPROMPT, 
								CEnString(IDS_TRANSFORMFILEFILTER), 
								this);
		
		if (dialog.DoModal(prefs) != IDOK)
			return; // user elected not to proceed
		
		sOutputPath = dialog.GetPathName();
	}
	
	// export
	DOPROGRESS(IDS_TRANSFORMPROGRESS);

	// set the html image folder to be the same as the 
	// output path without the extension
	CString sHtmlImgFolder(sOutputPath);
	FileMisc::ReplaceExtension(sHtmlImgFolder, _T("html_images"));
	
	CTaskFile tasks;
	GetTasks(tdc, TRUE, TRUE, dialog.GetTaskSelection(), tasks, sHtmlImgFolder);

	// add title and date 
	COleDateTime date;

	if (dialog.GetWantDate())
		date = COleDateTime::GetCurrentTime();

	tasks.SetReportAttributes(sTitle, date);
	
	// save intermediate tasklist to file as required
	LogIntermediateTaskList(tasks, tdc.GetFilePath());
	
	if (tasks.TransformToFile(sStylesheet, sOutputPath, xsl.GetOutputFileEncoding()))
	{
		// preview
		if (Prefs().GetPreviewExport())
			FileMisc::Run(*this, sOutputPath);
	}
}

BOOL CToDoListWnd::LogIntermediateTaskList(CTaskFile& tasks, LPCTSTR szRefPath)
{
	if (FileMisc::IsLoggingEnabled())
		return tasks.Save(GetIntermediateTaskListPath(szRefPath), SFEF_UTF16);

	// else
	return TRUE;
}

CString CToDoListWnd::GetIntermediateTaskListPath(LPCTSTR szRefPath)
{
	ASSERT(szRefPath && FileMisc::IsPath(szRefPath));

	CString sRefName = FileMisc::RemoveExtension(FileMisc::GetFileNameFromPath(szRefPath));
	
	return FileMisc::GetTempFilePath(sRefName, _T("intermediate.txt")); 
}

BOOL CToDoListWnd::GetStylesheetPath(const CFilteredToDoCtrl& tdc, CString& sDlgStylesheet)
{
	CString sTDCStylesheet = tdc.GetStylesheetPath();

	if (FileMisc::FileExists(sTDCStylesheet))
	{
		sDlgStylesheet = sTDCStylesheet;
		return TRUE;
	}

	// else
	return FileMisc::FileExists(sDlgStylesheet);
}

void CToDoListWnd::OnNexttopleveltask() 
{
	GetToDoCtrl().GotoNextTopLevelTask(TDCG_NEXT);
}

void CToDoListWnd::OnPrevtopleveltask() 
{
	GetToDoCtrl().GotoNextTopLevelTask(TDCG_PREV);
}

void CToDoListWnd::OnUpdateNexttopleveltask(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanGotoNextTopLevelTask(TDCG_NEXT));
}

void CToDoListWnd::OnUpdatePrevtopleveltask(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanGotoNextTopLevelTask(TDCG_PREV));
}

void CToDoListWnd::OnGotoNexttask() 
{
	GetToDoCtrl().GotoNextTask(TDCG_NEXT);
}

void CToDoListWnd::OnGotoPrevtask() 
{
	GetToDoCtrl().GotoNextTask(TDCG_PREV);
}

void CToDoListWnd::OnUpdateGotoPrevtask(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanGotoNextTask(TDCG_PREV));
}

void CToDoListWnd::OnUpdateGotoNexttask(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanGotoNextTask(TDCG_NEXT));
}
//------------------------------------------------------------------------

BOOL CToDoListWnd::InitFindDialog(BOOL bShow)
{
	if (!m_findDlg.GetSafeHwnd())
	{
		UpdateFindDialogActiveTasklist();

		VERIFY(m_findDlg.Initialize(this));

		if (CThemed::IsAppThemed())
			m_findDlg.SetUITheme(m_theme);

		if (bShow)
			m_findDlg.Show(bShow);
	}

	return (m_findDlg.GetSafeHwnd() != NULL);
}

void CToDoListWnd::OnFindTasks() 
{
	InitFindDialog();

	if (IsWindowVisible())
	{
		// remove results from encrypted tasklists but not from the 
		// active tasklist
		if (!m_findDlg.IsWindowVisible())
		{
			int nSelTDC = GetSelToDoCtrl();
			int nTDC = GetTDCCount();

			while (nTDC--)
			{
				const CFilteredToDoCtrl& tdc = GetToDoCtrl(nTDC);

				if (nTDC != nSelTDC && tdc.IsEncrypted())
					m_findDlg.DeleteResults(&tdc);
			}
		}
		m_findDlg.Show();
	}
	
	m_bFindShowing = TRUE;
}

LRESULT CToDoListWnd::OnFindDlgClose(WPARAM /*wp*/, LPARAM /*lp*/)
{
	m_bFindShowing = FALSE;
	GetToDoCtrl().SetFocusToTasks();

	return 0L;
}

LRESULT CToDoListWnd::OnFindDlgFind(WPARAM /*wp*/, LPARAM /*lp*/)
{
	// set up the search
	BOOL bSearchAll = m_findDlg.GetSearchAllTasklists();
	int nSelTaskList = GetSelToDoCtrl();
	
	int nFrom = bSearchAll ? 0 : nSelTaskList;
	int nTo = bSearchAll ? GetTDCCount() - 1 : nSelTaskList;
	
	// search params
	SEARCHPARAMS params;

	if (m_findDlg.GetSearchParams(params))
	{
		int nSel = GetSelToDoCtrl();
		
		for (int nCtrl = nFrom; nCtrl <= nTo; nCtrl++)
		{
			// load or verify password unless tasklist is already active
			if (nCtrl != nSel)
			{
				// load if necessary (in which case the password will have been checked)
				if (!m_mgrToDoCtrls.IsLoaded(nCtrl))
				{
					if (!VerifyTaskListOpen(nCtrl, FALSE))
						continue;
				}
				else if (!VerifyToDoCtrlPassword(nCtrl))
					continue;
			}
			
			CFilteredToDoCtrl& tdc = GetToDoCtrl(nCtrl);
			CHoldRedraw hr(m_bFindShowing ? m_findDlg.GetSafeHwnd() : NULL);

			CResultArray aResults;
			
			if (tdc.FindTasks(params, aResults))
			{
				// use tasklist title from tabctrl
				CString sTitle = m_mgrToDoCtrls.GetTabItemText(nCtrl);
				
				m_findDlg.AddHeaderRow(sTitle);
				
				for (int nResult = 0; nResult < aResults.GetSize(); nResult++)
					AddFindResult(aResults[nResult], &tdc);
			}
		}
	}	
	
	m_findDlg.SetActiveWindow();
	
	return 0;
}

void CToDoListWnd::AddFindResult(const SEARCHRESULT& result, const CFilteredToDoCtrl* pTDC)
{
	CString sTitle = pTDC->GetTaskTitle(result.dwTaskID);
	CString sPath = pTDC->GetTaskPath(result.dwTaskID);
	
	m_findDlg.AddResult(result, sTitle, sPath, pTDC);
}

LRESULT CToDoListWnd::OnFindSelectResult(WPARAM /*wp*/, LPARAM lp)
{
	// extract Task ID
	FTDRESULT* pResult = (FTDRESULT*)lp;
	ASSERT (pResult->dwTaskID); 
	
	int nCtrl = m_mgrToDoCtrls.FindToDoCtrl(pResult->pTDC);
	ASSERT(nCtrl != -1);
	
	if (m_tabCtrl.GetCurSel() != nCtrl)
	{
		if (!SelectToDoCtrl(nCtrl, TRUE))
			return 0L;
	}
	
	// we can't use pResult->pTDC because it's const
	CFilteredToDoCtrl& tdc = GetToDoCtrl(nCtrl);
	tdc.SetFocusToTasks();
	
	if (tdc.GetSelectedTaskID() != pResult->dwTaskID)
	{
		if (!tdc.SelectTask(pResult->dwTaskID))
		{
			// perhaps the task is filtered out so we toggle the filter
			// and try again
			if (tdc.HasAnyFilter() && tdc.HasTask(pResult->dwTaskID))
			{
				tdc.ToggleFilter();
				VERIFY (tdc.SelectTask(pResult->dwTaskID));
			}
		}

		Invalidate();
		UpdateWindow();
	}
	
	return 1L;
}

LRESULT CToDoListWnd::OnFindSelectAll(WPARAM /*wp*/, LPARAM /*lp*/)
{
	if (!m_findDlg.GetResultCount())
		return 0;
	
	CWaitCursor cursor;
	
	for (int nTDC = 0; nTDC < GetTDCCount(); nTDC++)
	{
		CFilteredToDoCtrl& tdc = GetToDoCtrl(nTDC);
		CDWordArray aTaskIDs;
		
		if (m_findDlg.GetResultIDs(&tdc, aTaskIDs))
			tdc.SelectTasks(aTaskIDs);
	}

	// if find dialog is floating then hide it
	if (!m_findDlg.IsDocked())
	{
		m_findDlg.Show(FALSE);
		GetToDoCtrl().SetFocusToTasks();
	}
	
	return 0;
}

LRESULT CToDoListWnd::OnFindApplyAsFilter(WPARAM /*wp*/, LPARAM lp)
{
	FTDCCUSTOMFILTER filter((LPCTSTR)lp);
	m_findDlg.GetSearchParams(filter);

	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	tdc.SetCustomFilter(filter);
	tdc.SetFocusToTasks();
	
	RefreshFilterBarCustomFilters();
	m_filterBar.RefreshFilterControls(tdc);

	// if find dialog is floating then hide it
	if (!m_findDlg.IsDocked())
		m_findDlg.Show(FALSE);
	
	return 0;
}

LRESULT CToDoListWnd::OnFindAddSearch(WPARAM /*wp*/, LPARAM /*lp*/)
{
	RefreshFilterBarCustomFilters();
	return 0;
}

LRESULT CToDoListWnd::OnFindSaveSearch(WPARAM /*wp*/, LPARAM lp)
{
	ASSERT(lp);

	CString sActive;
	LPCTSTR szSearch = (LPCTSTR)lp;

	if ((m_filterBar.GetFilter(sActive) == FS_CUSTOM) && (sActive == szSearch))
	{
		FTDCCUSTOMFILTER filter;
		m_findDlg.GetSearchParams(szSearch, filter);

		GetToDoCtrl().SetCustomFilter(filter);
	}

	return 0;
}

LRESULT CToDoListWnd::OnFindDeleteSearch(WPARAM /*wp*/, LPARAM /*lp*/)
{
	RefreshFilterBarCustomFilters();
	return 0;
}

void CToDoListWnd::RefreshFilterBarCustomFilters()
{
	CStringArray aFilters;
	
	m_findDlg.GetSavedSearches(aFilters);

	// check for unnamed filter
	if (m_findDlg.GetSafeHwnd())
	{
		CEnString sUnNamed(IDS_UNNAMEDFILTER);

		if (m_findDlg.GetActiveSearch().IsEmpty() && Misc::Find(aFilters, sUnNamed, FALSE, FALSE) == -1)
			aFilters.Add(sUnNamed);
	}

	m_filterBar.AddCustomFilters(aFilters);

	CRect rFilter;

	if (GetFilterBarRect(rFilter))
		InvalidateRect(rFilter, TRUE);
}

//------------------------------------------------------------------------

LRESULT CToDoListWnd::OnDropFile(WPARAM wParam, LPARAM lParam)
{
	TLDT_DATA* pData = (TLDT_DATA*)wParam;
	CWnd* pTarget = (CWnd*)lParam;

	if (pTarget == this) // we're the target
	{
		CString sFile = pData->pFilePaths ? pData->pFilePaths->GetAt(0) : _T("");

		if (FileMisc::HasExtension(sFile, _T("tdl")) || FileMisc::HasExtension(sFile, _T("xml"))) // tasklist
		{
			TDC_FILE nRes = OpenTaskList(sFile);
			HandleLoadTasklistError(nRes, sFile);
		}
	}

	return 0L;
}

void CToDoListWnd::OnViewMovetasklistright() 
{
	m_mgrToDoCtrls.MoveToDoCtrl(GetSelToDoCtrl(), 1);
}

void CToDoListWnd::OnUpdateViewMovetasklistright(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!Prefs().GetKeepTabsOrdered() &&
					m_mgrToDoCtrls.CanMoveToDoCtrl(GetSelToDoCtrl(), 1));
}

void CToDoListWnd::OnViewMovetasklistleft() 
{
	m_mgrToDoCtrls.MoveToDoCtrl(GetSelToDoCtrl(), -1);
}

void CToDoListWnd::OnUpdateViewMovetasklistleft(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!Prefs().GetKeepTabsOrdered() &&
					m_mgrToDoCtrls.CanMoveToDoCtrl(GetSelToDoCtrl(), -1));
}

void CToDoListWnd::OnToolsShowtasksDue(UINT nCmdID) 
{
	int nDueBy = PFP_DUETODAY;
	UINT nIDDueBy = IDS_NODUETODAY;
	
	switch (nCmdID)
	{
	case ID_TOOLS_SHOWTASKS_DUETODAY:
		break; // done
		
	case ID_TOOLS_SHOWTASKS_DUETOMORROW:
		nIDDueBy = IDS_NODUETOMORROW;
		nDueBy = PFP_DUETOMORROW;
		break;
		
	case ID_TOOLS_SHOWTASKS_DUEENDTHISWEEK:
		nIDDueBy = IDS_NODUETHISWEEK;
		nDueBy = PFP_DUETHISWEEK;
		break;
		
	case ID_TOOLS_SHOWTASKS_DUEENDNEXTWEEK:
		nIDDueBy = IDS_NODUENEXTWEEK;
		nDueBy = PFP_DUENEXTWEEK;
		break;
		
	case ID_TOOLS_SHOWTASKS_DUEENDTHISMONTH:
		nIDDueBy = IDS_NODUETHISMONTH;
		nDueBy = PFP_DUETHISMONTH;
		break;
		
	case ID_TOOLS_SHOWTASKS_DUEENDNEXTMONTH:
		nIDDueBy = IDS_NODUENEXTMONTH;
		nDueBy = PFP_DUENEXTMONTH;
		break;
		
	default:
		ASSERT(0);
		return;
	}
	
	if (!DoDueTaskNotification(GetSelToDoCtrl(), nDueBy))
	{
		MessageBox(nIDDueBy, 0, MB_OK, m_mgrToDoCtrls.GetFriendlyProjectName(GetSelToDoCtrl()));
	}
}

void CToDoListWnd::ResetPrefs()
{
	delete m_pPrefs;
	m_pPrefs = new CPreferencesDlg(&m_mgrShortcuts, 
									&m_mgrContent, 
									&m_mgrImportExport,
									&m_mgrUIExtensions);
	
	// update
	m_pPrefs->SetUITheme(m_theme);
	m_mgrToDoCtrls.SetPrefs(m_pPrefs); 
	
	// Update Filter bar colours
	CDWordArray aPriorityColors;
	m_pPrefs->GetPriorityColors(aPriorityColors);

	m_filterBar.SetPriorityColors(aPriorityColors);
}

const CPreferencesDlg& CToDoListWnd::Prefs() const
{
	ASSERT (m_pPrefs);
	return *m_pPrefs;
}

void CToDoListWnd::OnSpellcheckcomments() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	tdc.SpellcheckSelectedTask(FALSE);
}

void CToDoListWnd::OnUpdateSpellcheckcomments(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	pCmdUI->Enable(tdc.CanSpellcheckSelectedTaskComments());
}

void CToDoListWnd::OnSpellchecktitle() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	tdc.SpellcheckSelectedTask(TRUE);
}

void CToDoListWnd::OnUpdateSpellchecktitle(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	pCmdUI->Enable(!tdc.GetSelectedTaskTitle().IsEmpty());
}

void CToDoListWnd::OnFileEncrypt() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	if (!tdc.IsReadOnly())
	{
		BOOL bWasEncrypted = tdc.IsEncrypted();
		CString sPassword = tdc.GetPassword();

		// if the tasklist is already encrypted then verify password
		// before allowing change
		if (!bWasEncrypted || VerifyToDoCtrlPassword())
			tdc.EnableEncryption(!tdc.IsEncrypted());

		// make sure we disable encryption on the archive too
		if (bWasEncrypted)
		{
			CString sArchivePath;
			
			if (tdc.GetArchivePath(sArchivePath) && FileMisc::FileExists(sArchivePath))
			{
				CTaskFile archive(sPassword);

				if (archive.Load(sArchivePath))
				{
					archive.SetPassword(NULL); // remove password
					archive.Save(sArchivePath, SFEF_UTF16);
				}
			}
		}
	}

	UpdateAeroFeatures();
}

void CToDoListWnd::OnUpdateFileEncrypt(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	pCmdUI->Enable(tdc.CanEncrypt() && !tdc.IsReadOnly());
	pCmdUI->SetCheck(tdc.IsEncrypted() ? 1 : 0);
}

void CToDoListWnd::OnFileResetversion() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	if (!tdc.IsReadOnly())
	{
		tdc.ResetFileVersion();
		tdc.SetModified();
		
		UpdateStatusbar();
		UpdateCaption();
	}
}

void CToDoListWnd::OnUpdateFileResetversion(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	pCmdUI->Enable(!tdc.IsReadOnly());
}

void CToDoListWnd::OnSpellcheckTasklist() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	tdc.Spellcheck();
}

void CToDoListWnd::OnUpdateSpellcheckTasklist(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	pCmdUI->Enable(tdc.GetTaskCount());
}

TDC_FILE CToDoListWnd::SaveAll(DWORD dwFlags)
{
	TDC_FILE nSaveAll = TDCF_SUCCESS;
	int nCtrl = GetTDCCount();

	BOOL bIncUnsaved = Misc::HasFlag(dwFlags, TDLS_INCLUDEUNSAVED);
	BOOL bClosingWindows = Misc::HasFlag(dwFlags, TDLS_CLOSINGWINDOWS);
	BOOL bClosingAll = Misc::HasFlag(dwFlags, TDLS_CLOSINGTASKLISTS);		

	// scoped to end status bar progress
	// before calling UpdateStatusbar
	{
		DOPROGRESS(IDS_SAVINGPROGRESS);

		while (nCtrl--)
		{
			CFilteredToDoCtrl& tdc = GetToDoCtrl(nCtrl);

			// bypass unsaved tasklists unless closing Windows
			if (!bClosingWindows && 
				!bIncUnsaved &&
				!m_mgrToDoCtrls.UsesStorage(nCtrl) && 
				tdc.GetFilePath().IsEmpty())
			{
				continue;
			}
			
			if (Misc::HasFlag(dwFlags, TDLS_FLUSH))
				tdc.Flush(bClosingAll);		

			TDC_FILE nSave = ConfirmSaveTaskList(nCtrl, dwFlags);

			if (nSave == TDCF_CANCELLED) // user cancelled
				return TDCF_CANCELLED;

			// else cache any failure w/o overwriting previous
			if (nSaveAll == TDCF_SUCCESS)
				nSaveAll = nSave;

			m_mgrToDoCtrls.UpdateTabItemText(nCtrl);
		}
	}
	
	if (!bClosingWindows)
	{
		UpdateCaption();
		UpdateStatusbar();
	}
	
    return nSaveAll;
}

void CToDoListWnd::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (nIDCtl == IDC_TABCONTROL)
	{
		TDCM_DUESTATUS nStatus = m_mgrToDoCtrls.GetDueItemStatus(lpDrawItemStruct->itemID);

		if (nStatus == TDCM_PAST || nStatus == TDCM_TODAY)
		{
			// determine appropriate due colour
			COLORREF crDue, crDueToday;

			GetToDoCtrl(lpDrawItemStruct->itemID).GetDueTaskColors(crDue, crDueToday);

			COLORREF crTag = (nStatus == TDCM_PAST) ? crDue : crDueToday;

			if (crTag != CLR_NONE)
			{
				CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
				const CRect& rect = lpDrawItemStruct->rcItem;

				// draw a little tag in the top left corner
				for (int nHPos = 0; nHPos < 6; nHPos++)
				{
					for (int nVPos = 0; nVPos < 6 - nHPos; nVPos++)
					{
						pDC->SetPixelV(rect.left + nHPos, rect.top + nVPos, crTag);
					}
				}

				// draw a black line between the two
				pDC->SelectStockObject(BLACK_PEN);
				pDC->MoveTo(rect.left, rect.top + 6);
				pDC->LineTo(rect.left + 7, rect.top - 1);
			}
		}
		return;
	}
	else if (nIDCtl == 0 && lpDrawItemStruct->itemID == ID_CLOSE)
	{
		if (m_menubar.DrawMDIButton(lpDrawItemStruct))
			return;
	}

	CFrameWnd::OnDrawItem(nIDCtl, lpDrawItemStruct);
} 

void CToDoListWnd::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	if (nIDCtl == 0 && lpMeasureItemStruct->itemID == ID_CLOSE)
	{
		if (m_menubar.MeasureMDIButton(lpMeasureItemStruct))
			return;
	}
	
	CFrameWnd::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CToDoListWnd::OnViewNextSel() 
{
	GetToDoCtrl().SelectTasksInHistory(TRUE);
}

void CToDoListWnd::OnUpdateViewNextSel(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanSelectTasksInHistory(TRUE));
}

void CToDoListWnd::OnViewPrevSel() 
{
	GetToDoCtrl().SelectTasksInHistory(FALSE);
}

void CToDoListWnd::OnUpdateViewPrevSel(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanSelectTasksInHistory(FALSE));
}

void CToDoListWnd::OnSplitTaskIntoPieces(UINT nCmdID) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	int nNumPieces = 2 + (nCmdID - ID_NEWTASK_SPLITTASKINTO_TWO);
	
	tdc.SplitSelectedTask(nNumPieces);
}

void CToDoListWnd::OnUpdateSplitTaskIntoPieces(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	pCmdUI->Enable(tdc.CanSplitSelectedTask());
}

void CToDoListWnd::OnViewExpandtask() 
{
	GetToDoCtrl().ExpandTasks(TDCEC_SELECTED, TRUE);
}

void CToDoListWnd::OnUpdateViewExpandtask(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanExpandTasks(TDCEC_SELECTED, TRUE));
}

void CToDoListWnd::OnViewCollapsetask() 
{
	GetToDoCtrl().ExpandTasks(TDCEC_SELECTED, FALSE);
}

void CToDoListWnd::OnUpdateViewCollapsetask(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanExpandTasks(TDCEC_SELECTED, FALSE));
}

void CToDoListWnd::OnViewExpandall() 
{
	GetToDoCtrl().ExpandTasks(TDCEC_ALL, TRUE);
}

void CToDoListWnd::OnUpdateViewExpandall(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanExpandTasks(TDCEC_ALL, TRUE));
}

void CToDoListWnd::OnViewCollapseall() 
{
	GetToDoCtrl().ExpandTasks(TDCEC_ALL, FALSE);
}

void CToDoListWnd::OnUpdateViewCollapseall(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanExpandTasks(TDCEC_ALL, FALSE));
}

void CToDoListWnd::OnViewExpandDuetasks() 
{
	GetToDoCtrl().ExpandTasks(TDCEC_DUE, TRUE);
}

void CToDoListWnd::OnUpdateViewExpandDuetasks(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanExpandTasks(TDCEC_DUE, TRUE));
}

void CToDoListWnd::OnViewCollapseDuetasks() 
{
	GetToDoCtrl().ExpandTasks(TDCEC_DUE, FALSE);
}

void CToDoListWnd::OnUpdateViewCollapseDuetasks(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanExpandTasks(TDCEC_DUE, FALSE));
}

void CToDoListWnd::OnViewExpandStartedtasks() 
{
	GetToDoCtrl().ExpandTasks(TDCEC_STARTED, TRUE);
}

void CToDoListWnd::OnUpdateViewExpandStartedtasks(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanExpandTasks(TDCEC_STARTED, TRUE));
}

void CToDoListWnd::OnViewCollapseStartedtasks() 
{
	GetToDoCtrl().ExpandTasks(TDCEC_STARTED, FALSE);
}

void CToDoListWnd::OnUpdateViewCollapseStartedtasks(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanExpandTasks(TDCEC_STARTED, FALSE));
}

void CToDoListWnd::OnViewToggletaskexpanded() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	tdc.ExpandTasks(TDCEC_SELECTED, tdc.CanExpandTasks(TDCEC_SELECTED, TRUE));
}

void CToDoListWnd::OnUpdateViewToggletaskexpanded(CCmdUI* pCmdUI) 
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();

	pCmdUI->Enable(tdc.CanExpandTasks(TDCEC_SELECTED, TRUE) || tdc.CanExpandTasks(TDCEC_SELECTED, FALSE));
}

void CToDoListWnd::OnWindow(UINT nCmdID) 
{
	int nTDC = (int)(nCmdID - ID_WINDOW1);
	
	if (nTDC < GetTDCCount())
		SelectToDoCtrl(nTDC, (nTDC != GetSelToDoCtrl()), Prefs().GetNotifyDueByOnSwitch());
}

void CToDoListWnd::OnUpdateWindow(CCmdUI* pCmdUI) 
{
	if (pCmdUI->m_pMenu)
	{
		ASSERT (ID_WINDOW1 == pCmdUI->m_nID);
		const UINT MAXWINDOWS = 16;
		
		// delete existing tool entries first
		for (int nWnd = 0; nWnd < MAXWINDOWS; nWnd++)
			pCmdUI->m_pMenu->DeleteMenu(ID_WINDOW1 + nWnd, MF_BYCOMMAND);
		
		int nSel = GetSelToDoCtrl();
		int nPos = 0, nTDCCount = GetTDCCount();
		ASSERT (nTDCCount);

		nTDCCount = min(nTDCCount, MAXWINDOWS);
		
		for (int nTDC = 0; nTDC < nTDCCount; nTDC++)
		{
			CString sMenuItem, sName = m_mgrToDoCtrls.FormatProjectNameWithFileName(nTDC);
			sMenuItem.Format(_T("&%d %s"), (nPos + 1) % 10, sName);
			
			UINT nFlags = (MF_BYPOSITION | MF_STRING | (nSel == nTDC ? MF_CHECKED : MF_UNCHECKED));
			pCmdUI->m_pMenu->InsertMenu(pCmdUI->m_nIndex++, nFlags, (ID_WINDOW1 + nTDC), sMenuItem);
			
			nPos++;
		}
		
		// update end menu count
		pCmdUI->m_nIndex--; // point to last menu added
		pCmdUI->m_nIndexMax = pCmdUI->m_pMenu->GetMenuItemCount();
		
		pCmdUI->m_bEnableChanged = TRUE;    // all the added items are enabled
	}
}

#if _MSC_VER >= 1400
void CToDoListWnd::OnActivateApp(BOOL bActive, DWORD dwThreadID)
#else
void CToDoListWnd::OnActivateApp(BOOL bActive, HTASK hTask) 
#endif
{
	// don't activate when in the middle of loading
	if (m_bReloading && !bActive)
		return;

#if _MSC_VER >= 1400
	CFrameWnd::OnActivateApp(bActive, dwThreadID);
#else
	CFrameWnd::OnActivateApp(bActive, hTask);
#endif
	
	// don't do any further processing if closing
    if (m_bClosing)
        return; 

	// Don't any further processing if the Reminder dialog is active
	// because the two windows get into a fight for activation!
	if (m_reminders.IsForegroundWindow())
		return;

	if (!bActive)
	{
		// save focus to restore when we next get activated
		HWND hFocus = ::GetFocus();

		if (hFocus)
			m_hwndLastFocus = hFocus;

		// save tasklists if required
		if (Prefs().GetAutoSaveOnSwitchApp())
			SaveAll(TDLS_FLUSH | TDLS_AUTOSAVE);
	}
	else
	{
		if (GetTDCCount() && (!m_hwndLastFocus || Prefs().GetAutoFocusTasklist()))
		{
			PostMessage(WM_APPRESTOREFOCUS, 0L, (LPARAM)GetToDoCtrl().GetSafeHwnd());
		}
		else if (m_hwndLastFocus)
		{
			// delay the restoration of focus else it gets lost
			PostMessage(WM_APPRESTOREFOCUS, 0L, (LPARAM)m_hwndLastFocus);
		}

		UpdateCwd();
	}
}

LRESULT CToDoListWnd::OnAppRestoreFocus(WPARAM /*wp*/, LPARAM lp)
{
	HWND hWnd = (HWND)lp;
	
#ifdef _DEBUG
	CString sClass = CWinClasses::GetClass(hWnd);
#endif

	if (GetTDCCount() && (hWnd == GetToDoCtrl().GetSafeHwnd()))
	{
		GetToDoCtrl().SetFocusToTasks();
	}
	else if (::IsWindowEnabled(hWnd))
	{
		::SetFocus(hWnd);
		::SendMessage(hWnd, WM_SETFOCUS, 0L, 0L);
	}

	return 0L;
}

void CToDoListWnd::UpdateCwd()
{
	// set cwd to active tasklist
	if (GetTDCCount())
	{
		CString sFolder	= FileMisc::GetFolderFromFilePath(m_mgrToDoCtrls.GetFilePath(GetSelToDoCtrl()));

		if (FileMisc::FolderExists(sFolder))
			SetCurrentDirectory(sFolder);
	}
}

BOOL CToDoListWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	UpdateWindow();

	return CFrameWnd::OnCommand(wParam, lParam);
}

void CToDoListWnd::OnEnable(BOOL bEnable) 
{
	CFrameWnd::OnEnable(bEnable);
	
	if (!bEnable)
	{
		// save current focus because modal window is being shown
		HWND hFocus = ::GetFocus();

		if (hFocus)
			m_hwndLastFocus = hFocus;

		// Save and hide time tracker if it is top-most
		ASSERT(!m_bReshowTimeTrackerOnEnable);

		if (m_dlgTimeTracker.IsWindowVisible() && m_dlgTimeTracker.IsAlwaysOnTop())
		{
			m_bReshowTimeTrackerOnEnable = TRUE;
			m_dlgTimeTracker.ShowWindow(SW_HIDE);
		}
	}
	else
	{
		UpdateWindow();

		if (m_bReshowTimeTrackerOnEnable)
		{
			m_bReshowTimeTrackerOnEnable = FALSE;
			m_dlgTimeTracker.ShowWindow(SW_SHOW);
		}

		// then restore it when we are enabled
		if (m_hwndLastFocus)
			PostMessage(WM_APPRESTOREFOCUS, 0L, (LPARAM)m_hwndLastFocus);
	}
}

void CToDoListWnd::OnViewSorttasklisttabs() 
{
	int nSel = m_mgrToDoCtrls.SortToDoCtrlsByName();
	SelectToDoCtrl(nSel, FALSE);
}

void CToDoListWnd::OnUpdateViewSorttasklisttabs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetTDCCount() > 1 && !Prefs().GetKeepTabsOrdered());
}

void CToDoListWnd::OnEditInctaskpercentdone() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	tdc.IncrementSelectedTaskPercentDone(TRUE);
}

void CToDoListWnd::OnUpdateEditInctaskpercentdone(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	int nSelCount = tdc.GetSelectedCount();
	
	pCmdUI->Enable(nSelCount && !tdc.IsReadOnly());	
}

void CToDoListWnd::OnEditDectaskpercentdone() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	tdc.IncrementSelectedTaskPercentDone(FALSE);
}

void CToDoListWnd::OnUpdateEditDectaskpercentdone(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	int nSelCount = tdc.GetSelectedCount();
	
	pCmdUI->Enable(nSelCount && !tdc.IsReadOnly());	
}

void CToDoListWnd::OnEditDectaskpriority() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	tdc.IncrementSelectedTaskPriority(FALSE);
}

void CToDoListWnd::OnUpdateEditDectaskpriority(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	int nSelCount = tdc.GetSelectedCount();
	
	pCmdUI->Enable(nSelCount && !tdc.IsReadOnly());	
}

void CToDoListWnd::OnEditInctaskpriority() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	tdc.IncrementSelectedTaskPriority(TRUE);
}

void CToDoListWnd::OnUpdateEditInctaskpriority(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	int nSelCount = tdc.GetSelectedCount();
	
	pCmdUI->Enable(nSelCount && !tdc.IsReadOnly());	
}

void CToDoListWnd::OnEditFlagtask() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	tdc.SetSelectedTaskFlag(!tdc.IsSelectedTaskFlagged());
}

void CToDoListWnd::OnUpdateEditFlagtask(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	int nSelCount = tdc.GetSelectedCount();
	
	pCmdUI->Enable(nSelCount && !tdc.IsReadOnly());	
	pCmdUI->SetCheck(tdc.IsSelectedTaskFlagged() ? 1 : 0);
}

void CToDoListWnd::OnEditGotoDependency() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	tdc.GotoSelectedTaskDependency();
}

void CToDoListWnd::OnUpdateEditGotoDependency(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	CStringArray aDepends;

	pCmdUI->Enable(tdc.GetSelectedTaskDependencies(aDepends) > 0);	
}

void CToDoListWnd::OnEditRecurrence() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	tdc.EditSelectedTaskRecurrence();	
}

void CToDoListWnd::OnUpdateEditRecurrence(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	pCmdUI->Enable(!tdc.IsReadOnly() && tdc.GetSelectedCount() && !tdc.IsSelectedTaskDone());	
}

void CToDoListWnd::OnFileOpenarchive() 
{
	CString sArchivePath;
	
	if (GetToDoCtrl().GetArchivePath(sArchivePath) && FileMisc::FileExists(sArchivePath))
		OpenTaskList(sArchivePath, FALSE);
}

void CToDoListWnd::OnUpdateFileOpenarchive(CCmdUI* pCmdUI) 
{
	CString sArchivePath;
	BOOL bArchiveExists = (GetToDoCtrl().GetArchivePath(sArchivePath) && FileMisc::FileExists(sArchivePath));
	
	pCmdUI->Enable(bArchiveExists);
}

void CToDoListWnd::PrepareFilter(FTDCFILTER& filter) const
{
	if (filter.nShow != FS_CUSTOM)
	{
		// handle title filter option
		switch (Prefs().GetTitleFilterOption())
		{
		case PUIP_MATCHONTITLECOMMENTS:
			filter.nTitleOption = FT_FILTERONTITLECOMMENTS;
			break;

		case PUIP_MATCHONANYTEXT:
			filter.nTitleOption = FT_FILTERONANYTEXT;
			break;

		case PUIP_MATCHONTITLE:
		default:
			filter.nTitleOption = FT_FILTERONTITLEONLY;
			break;
		}
	}
}

void CToDoListWnd::OnViewShowfilterbar() 
{
	m_bShowFilterBar = !m_bShowFilterBar;

	if (m_bShowFilterBar)
		m_filterBar.RefreshFilterControls(GetToDoCtrl());

	m_filterBar.ShowWindow(m_bShowFilterBar ? SW_SHOW : SW_HIDE);

	Resize();
	Invalidate();
	GetToDoCtrl().Invalidate();
}

void CToDoListWnd::OnUpdateViewShowfilterbar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bShowFilterBar ? 1 : 0);
}

void CToDoListWnd::OnViewClearfilter() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	if (tdc.HasAnyFilter() || tdc.CanToggleFilter())
	{
		tdc.ClearFilter();
	
		// re-enable the filter controls
		m_filterBar.RefreshFilterControls(tdc);
	
		RefreshFilterBarControls();
		UpdateStatusbar();
	}
}

void CToDoListWnd::OnUpdateViewClearfilter(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	pCmdUI->Enable(tdc.HasAnyFilter() || tdc.CanToggleFilter());
}

void CToDoListWnd::OnViewTogglefilter()
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	tdc.ToggleFilter();

	RefreshFilterBarControls();
	UpdateStatusbar();
}

void CToDoListWnd::OnUpdateViewTogglefilter(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetToDoCtrl().CanToggleFilter());
}

LRESULT CToDoListWnd::OnSelchangeFilter(WPARAM wp, LPARAM lp) 
{
	int nCtrlID = (int)wp;
	HWND hWnd = (HWND)lp;

	ASSERT((nCtrlID == m_filterBar.GetDlgCtrlID()) && (hWnd == m_filterBar));

	FTDCFILTER filter;
	CString sCustom;
	DWORD dwCustomFlags;

	m_filterBar.GetFilter(filter, sCustom, dwCustomFlags);
	OnChangeFilter(filter, sCustom, dwCustomFlags);

	return 0L;
}

void CToDoListWnd::OnChangeFilter(FTDCFILTER& filter, const CString& sCustom, DWORD dwCustomFlags)
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	if (!sCustom.IsEmpty())
	{
		FTDCCUSTOMFILTER filter(sCustom, dwCustomFlags);
		VERIFY(m_findDlg.GetSearchParams(sCustom, filter));

		tdc.SetCustomFilter(filter);
	}
	else
	{
		PrepareFilter(filter);
		tdc.SetFilter(filter);
	}

	m_filterBar.RefreshFilterControls(tdc);

	UpdateStatusbar();

	CRect rFilterBar;

	if (GetFilterBarRect(rFilterBar))
	{
		InvalidateRect(rFilterBar);
		UpdateWindow();
	}
}

void CToDoListWnd::OnViewFilter() 
{
	CStringArray aCustom;
	m_filterBar.GetCustomFilters(aCustom);
	
	CDWordArray aPriorityColors;
	Prefs().GetPriorityColors(aPriorityColors);
	
	CTDLFilterDlg dialog(Prefs().GetMultiSelFilters());

	if (dialog.DoModal(aCustom, GetToDoCtrl(), aPriorityColors) == IDOK)
	{
		FTDCFILTER filter;
		CString sCustom;
		DWORD dwCustomFlags = 0;
		
		dialog.GetFilter(filter, sCustom, dwCustomFlags);
		OnChangeFilter(filter, sCustom, dwCustomFlags);
	}
}

void CToDoListWnd::OnUpdateViewFilter(CCmdUI* pCmdUI) 
{
	UNREFERENCED_PARAMETER(pCmdUI);
	//	pCmdUI->Enable(!m_bShowFilterBar);
}

void CToDoListWnd::OnViewRefreshfilter() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	// if the filter has changed then set the new one else
	// refresh the current one
	FTDCFILTER filter;
	CString sCustom;
	DWORD dwCustomFlags;

	m_filterBar.GetFilter(filter, sCustom, dwCustomFlags);
	
	if (tdc.FilterMatches(filter, sCustom, dwCustomFlags))
	{
		tdc.RefreshFilter();
	}
	else
	{
		OnChangeFilter(filter, sCustom, dwCustomFlags);

		if (Prefs().GetExpandTasksOnLoad())
			tdc.ExpandTasks(TDCEC_ALL);
	}

	UpdateStatusbar();
}

void CToDoListWnd::OnUpdateViewRefreshfilter(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	pCmdUI->Enable(tdc.HasAnyFilter());
}

void CToDoListWnd::OnTabctrlPreferences() 
{
	DoPreferences(PREFPAGE_UI);
}

void CToDoListWnd::OnTasklistSelectColumns() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	int nSel = GetSelToDoCtrl();

	TDCCOLEDITFILTERVISIBILITY vis, visDefault;

	tdc.GetColumnEditFilterVisibility(vis);
	Prefs().GetDefaultColumnEditFilterVisibility(visDefault);

	CTDLColumnSelectionDlg dialog(vis, visDefault);

	if (dialog.DoModal() == IDOK)
	{
		dialog.GetColumnEditFilterVisibility(vis);

		// update preferences
		if (dialog.GetUpdatePreferences())
		{
			ASSERT(m_pPrefs);
			m_pPrefs->SetDefaultColumnEditFilterVisibility(vis);
		}

		m_filterBar.SetVisibleFilters(vis.GetVisibleFilterFields());

		if (dialog.GetApplyActiveTasklist())
		{
			tdc.SetColumnEditFilterVisibility(vis);
		}
		else
		{
			// and apply to all tasklists
			int nTDC = GetTDCCount();

			while (nTDC--)
			{
				CFilteredToDoCtrl& tdc = GetToDoCtrl(nTDC);
				tdc.SetColumnEditFilterVisibility(vis);
			}
		}

		// reload the menu if we dynamically alter it
		if (Prefs().GetShowEditMenuAsColumns())
			LoadMenubar();

		Resize();
	}
}

void CToDoListWnd::OnViewProjectname() 
{
	m_bShowProjectName = !m_bShowProjectName;
	
	// mark all tasklists as needing update
	m_mgrToDoCtrls.SetAllNeedPreferenceUpdate(TRUE);
	
	// then update active tasklist
	GetToDoCtrl().SetStyle(TDCS_SHOWPROJECTNAME, m_bShowProjectName);
	m_mgrToDoCtrls.SetNeedsPreferenceUpdate(GetSelToDoCtrl(), FALSE);
}

void CToDoListWnd::OnUpdateViewProjectname(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bShowProjectName ? 1 : 0);
}

void CToDoListWnd::OnEditOffsetdates() 
{
	CTDLOffsetDatesDlg dialog;
	
	if (dialog.DoModal() == IDOK)
	{
		TDC_OFFSET nUnits;
		int nAmount = dialog.GetOffsetAmount(nUnits);
		
		if (!nAmount)
			return;
		
		DWORD dwWhat = dialog.GetOffsetWhat();
		BOOL bSubtasks = dialog.GetOffsetSubtasks();
		
		// do the offsets
		CFilteredToDoCtrl& tdc = GetToDoCtrl();
		
		if (dwWhat & ODD_STARTDATE)
		{
			// special case: Moving due dates as well
			if (dwWhat & ODD_DUEDATE)
				tdc.OffsetSelectedTaskDates(nAmount, nUnits, bSubtasks);
			else
				tdc.OffsetSelectedTaskDate(TDCD_START, nAmount, nUnits, bSubtasks);
		}
		else if (dwWhat & ODD_DUEDATE)
		{
			tdc.OffsetSelectedTaskDate(TDCD_DUE, nAmount, nUnits, bSubtasks);
		}
		
		if (dwWhat & ODD_DONEDATE)
			tdc.OffsetSelectedTaskDate(TDCD_DONE, nAmount, nUnits, bSubtasks);
	}
}

void CToDoListWnd::OnUpdateEditOffsetdates(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	int nSelCount = tdc.GetSelectedCount();
	
	pCmdUI->Enable(nSelCount && !tdc.IsReadOnly());	
}

void CToDoListWnd::OnPrintpreview() 
{
	DoPrint(TRUE);
}

void CToDoListWnd::OnShowTimelogfile() 
{
	CString sLogPath = GetToDoCtrl().GetSelectedTaskTimeLogPath();
	
	if (FileMisc::FileExists(sLogPath))
	{
		// special handling for excel
		if (CFileRegister::IsRegisteredApp(_T("csv"), _T("EXCEL.EXE"), TRUE))
		{
			CString sParams;
			sParams.Format(_T("/r \"%s\""), sLogPath); // readonly 

			FileMisc::Run(*this, CFileRegister::GetRegisteredAppPath(_T("csv")), sParams);
		}
		else
		{
			FileMisc::Run(*this, sLogPath);
		}
	}
}

void CToDoListWnd::OnUpdateShowTimelogfile(CCmdUI* pCmdUI) 
{
	const CPreferencesDlg& userPrefs = Prefs();
	int nTasks = GetToDoCtrl().GetSelectedCount();
	BOOL bEnable = FALSE;

	if (userPrefs.GetLogTimeTracking() && 
		(nTasks == 1 || !userPrefs.GetLogTaskTimeSeparately()))
	{
		CString sLogPath = GetToDoCtrl().GetSelectedTaskTimeLogPath();
		bEnable = FileMisc::FileExists(sLogPath);
	}
	
	pCmdUI->Enable(bEnable);	
}

void CToDoListWnd::OnAddtimetologfile() 
{
	GetToDoCtrl().DoAddTimeToLogFile();
}

void CToDoListWnd::OnUpdateAddtimetologfile(CCmdUI* pCmdUI) 
{
	BOOL bEnable = (Prefs().GetLogTimeTracking() && GetToDoCtrl().GetSelectedCount() == 1);
	pCmdUI->Enable(bEnable);	
}

void CToDoListWnd::OnToolsAnalyseLoggedTime() 
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	const CPreferencesDlg& userPrefs = Prefs();

	CString sTaskFile(tdc.GetFilePath());
	CString sLogPath(CTDCTaskTimeLog(sTaskFile).GetLogPath());

	CTDLAnalyseLoggedTimeDlg dialog(sLogPath);
	BOOL bContinue = TRUE;

	while (bContinue)
	{
		bContinue = (dialog.DoModal() == IDOK);

		if (bContinue)
		{
			DWORD dwLastErr = 0;
			SetLastError(0);

			COleDateTime dtFrom, dtTo;

			if (dialog.GetDateRange(dtFrom, dtTo))
			{
				SetLastError(0);
				
				// else do the analysis
				CTDCTaskTimeLogAnalysis ttla(sTaskFile, userPrefs.GetLogTaskTimeSeparately());
				
				if (ttla.AnalyseTaskLog(dtFrom, dtTo, 
										dialog.GetBreakdown(), 
										dialog.GetOutputFormat(), 
										dialog.GetOutputFilepath()))
				{
					// query the user to view the results
					if (MessageBox(IDS_LOGANALYSIS_QUERYOPEN, 0, MB_YESNO) == IDYES)
						FileMisc::Run(*this, dialog.GetOutputFilepath());

					return;
				}
				else 
				{
					dwLastErr = GetLastError();
				}
			} 
			
			// error handling
			switch (dwLastErr)
			{
			case ERROR_ACCESS_DENIED:
			case ERROR_SHARING_VIOLATION:
				MessageBox(IDS_LOGANALYSIS_ERRNOACCESS, 0, MB_ICONWARNING);
				break;

			default: // no tasks matching
				bContinue = (MessageBox(IDS_LOGANALYSIS_ERRNOTASKS, 0, MB_YESNO) == IDYES);
				break;
			}
		}
	}
}

void CToDoListWnd::OnUpdateToolsAnalyseLoggedTime(CCmdUI* pCmdUI) 
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	const CPreferencesDlg& userPrefs = Prefs();

	BOOL bEnable = FALSE;

	if (userPrefs.GetLogTimeTracking())
	{
		CString sLogPath = tdc.GetSelectedTaskTimeLogPath();
		bEnable = FileMisc::FileExists(sLogPath);
	}
	
	pCmdUI->Enable(bEnable);
}

LRESULT CToDoListWnd::OnToDoCtrlDoLengthyOperation(WPARAM wParam, LPARAM lParam)
{
	if (wParam) // start op
	{
		m_sbProgress.BeginProgress(m_statusBar, (LPCTSTR)lParam);
	}
	else // end op
	{
		m_sbProgress.EndProgress();
	}
	
	return 0L;
}

BOOL CToDoListWnd::DoTaskLink(const CString& sPath, DWORD dwTaskID, BOOL bStartup)
{
	// handle no file path => active tasklist
	if (sPath.IsEmpty())
	{
		ASSERT(dwTaskID);
		GetToDoCtrl().SelectTask(dwTaskID);

		return TRUE; // handled regardless of result
	}
	else if (!PathIsRelative(sPath) && FileMisc::FileExists(sPath))
	{
		// do we have this tasklist ?
		int nTDC = m_mgrToDoCtrls.FindToDoCtrl(sPath);

		if (nTDC != -1)
		{
			int nSelTDC = GetSelToDoCtrl();
			
			if (SelectToDoCtrl(nTDC, (nTDC != nSelTDC)))
			{
				if (dwTaskID)
					GetToDoCtrl().SelectTask(dwTaskID);
			}
			else
			{
				ASSERT(0);
			}

			return TRUE; // handled regardless of result
		}
		else if (bStartup || !Prefs().GetMultiInstance())
		{
			TDC_FILE nRet = OpenTaskList(sPath);
			
			if (nRet == TDCF_SUCCESS)
			{
				if (dwTaskID)
					GetToDoCtrl().SelectTask(dwTaskID);
			}
			else
			{
				HandleLoadTasklistError(nRet, sPath);
			}

			return TRUE; // handled regardless of result
		}
	}
	else
	{
		ASSERT(0);
	}

	// not handled
	return FALSE;
}

BOOL CToDoListWnd::ValidateTaskLinkFilePath(CString& sPath) const
{
	if (sPath.IsEmpty())
		return FALSE;

	if (!FileMisc::HasExtension(sPath, _T("tdl")) &&
		!FileMisc::HasExtension(sPath, _T("xml")))
	{
		return FALSE;
	}

	if (::PathIsRelative(sPath))
	{
		if (GetTDCCount())
		{
			int nSelTDC = GetSelToDoCtrl();
			FileMisc::MakeFullPath(sPath, m_mgrToDoCtrls.GetFolderPath(nSelTDC));
		}
		else
		{
			FileMisc::MakeFullPath(sPath, FileMisc::GetModuleFolder());
		}
	}

	return FileMisc::FileExists(sPath);
}

LRESULT CToDoListWnd::OnToDoCtrlDoTaskLink(WPARAM wParam, LPARAM lParam)
{
	DWORD dwTaskID = wParam;
	CString sPath((LPCTSTR)lParam);

	// We should only receive this from CToDoCtrlReminders
	// or from CFilteredToDoCtrl because it couldn't handle it.
	// In both cases there should be a path 
	ASSERT(!sPath.IsEmpty());

	if (!ValidateTaskLinkFilePath(sPath))
		return FALSE;

	// can we handle it ?
	if (DoTaskLink(sPath, dwTaskID, FALSE))
		return TRUE;

	// Pass to our app startup code to look 
	// for another instance who can handle it
	CString sCommandline;
			
	sCommandline.Format(_T("%s -l \"%s?%lu\""),
						FileMisc::GetAppFilePath(),
						sPath,
						dwTaskID);

	return FileMisc::Run(*this, sCommandline);
}

LRESULT CToDoListWnd::OnTodoCtrlFailedLink(WPARAM wParam, LPARAM lParam)
{
	LPCTSTR szLink = (LPCTSTR)lParam;

	// if it's an Outlook link then run it directly
	if (CMSOutlookHelper::IsOutlookUrl(szLink))
	{
		if (!CMSOutlookHelper::IsOutlookInstalled())
		{
			AfxMessageBox(IDS_ERROROUTLOOKNOTINSTALLED);
			return TRUE; // we handled it
		}
		else if (CMSOutlookHelper::HandleUrl(*this, szLink))
		{
			return TRUE; // we handled it
		}
		// else fall thru for generic error message
	}
	else // see if it's a task link
	{
		CString sPath;
		DWORD dwTaskID = 0;

		int nTDC = m_mgrToDoCtrls.FindToDoCtrl((HWND)wParam);
		ASSERT(nTDC != -1);

		if (nTDC != -1)
		{
			const CFilteredToDoCtrl& tdc = GetToDoCtrl(nTDC);

			if (tdc.ParseTaskLink(szLink, dwTaskID, sPath) &&
				ValidateTaskLinkFilePath(sPath))
			{
				if (OnToDoCtrlDoTaskLink(dwTaskID, (LPARAM)(LPCTSTR)sPath))
					return TRUE; // we handled it
			}
		}
	}

	// all else
	MessageBox(IDS_COMMENTSGOTOERRMSG);
	return 0L;
}

LRESULT CToDoListWnd::OnToDoCtrlIsTaskDone(WPARAM wParam, LPARAM lParam)
{
	ASSERT (lParam);
	CString sFile((LPCTSTR)lParam);
	
	if (!sFile.IsEmpty())
	{
		// build the full path to the file
		if (::PathIsRelative(sFile))
		{
			// append it to the folder containing the active tasklist
			CString sPathName = m_mgrToDoCtrls.GetFilePath(GetSelToDoCtrl());
			CString sDrive, sFolder;

			FileMisc::SplitPath(sPathName, &sDrive, &sFolder);
			FileMisc::MakePath(sFile, sDrive, sFolder, sFile);
		}
		// else its a full path already
		
		int nTDC = m_mgrToDoCtrls.FindToDoCtrl(sFile);

		if (nTDC != -1) // already loaded
		{
			return GetToDoCtrl(nTDC).IsTaskDone(wParam);
		}
		else
		{
			// we must load the tasklist ourselves
			CTaskFile tasks;

			if (tasks.Load(sFile))
			{
				HTASKITEM ht = tasks.FindTask(wParam);
				return ht ? tasks.IsTaskDone(ht) : FALSE;
			}
		}
	}
	
	return 0L;
}

LRESULT CToDoListWnd::OnSessionStatusChange(WPARAM wp, LPARAM lp)
{
	switch (wp)
	{
	case SESSIONSTATUS_HIBERNATE:
		if (lp)
			KillTimers();
		else
			RestoreTimers();
		// fall thru

	case SESSIONSTATUS_LOCK:
	case SESSIONSTATUS_SCREENSAVER:
		RefreshPauseTimeTracking();
		break;
	}
	
	return 0L;
}

LRESULT CToDoListWnd::OnGetFont(WPARAM /*wp*/, LPARAM /*lp*/)
{
	return (LRESULT)m_fontMain.GetSafeHandle();
}

void CToDoListWnd::OnViewStatusBar() 
{
	m_bShowStatusBar = !m_bShowStatusBar;
	m_statusBar.ShowWindow(m_bShowStatusBar ? SW_SHOW : SW_HIDE);
	
	SendMessage(WM_SIZE, SIZE_RESTORED, 0L);

	if (m_bShowStatusBar)
		UpdateStatusbar();
	else
		UpdateCaption();
}

void CToDoListWnd::OnUpdateViewStatusBar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bShowStatusBar ? 1 : 0) ;
}

BOOL CToDoListWnd::OnQueryOpen() 
{
	if (CFrameWnd::OnQueryOpen())
	{
		// fail if the active tasklist is encrypted because we have to verify the password
		// and we're not allowed to display a dialog in this message handler
		if (!m_bQueryOpenAllow && GetToDoCtrl().IsEncrypted())
		{
			PostMessage(WM_TDL_RESTORE); 
			return FALSE;
		}
		
		// all others
		return TRUE;
	}
	
	return FALSE;
}

LRESULT CToDoListWnd::OnToDoListRestore(WPARAM /*wp*/, LPARAM /*lp*/)
{
    ASSERT (IsIconic() && GetToDoCtrl().IsEncrypted()); // sanity check
	
    if (IsIconic())
    {
        if (VerifyToDoCtrlPassword())
		{
			CAutoFlag af(m_bQueryOpenAllow, TRUE);
            ShowWindow(SW_RESTORE);
		}
    }

	return 0L;
}

void CToDoListWnd::OnCopyTaskasLink() 
{
	CopySelectedTasksToClipboard(TDCTC_ASLINK);
}

void CToDoListWnd::OnUpdateCopyTaskasLink(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().GetSelectedCount() == 1);
}

void CToDoListWnd::OnCopyTaskasDependency() 
{
	CopySelectedTasksToClipboard(TDCTC_ASDEPENDS);
}

void CToDoListWnd::OnUpdateCopyTaskasDependency(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().GetSelectedCount() == 1);
}

void CToDoListWnd::OnCopyTaskasLinkFull() 
{
	CopySelectedTasksToClipboard(TDCTC_ASLINKFULL);
}

void CToDoListWnd::OnUpdateCopyTaskasLinkFull(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().GetSelectedCount() == 1);
}

void CToDoListWnd::OnCopyTaskasDependencyFull() 
{
	CopySelectedTasksToClipboard(TDCTC_ASDEPENDSFULL);
}

void CToDoListWnd::OnUpdateCopyTaskasDependencyFull(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().GetSelectedCount() == 1);
}

void CToDoListWnd::OnCopyTaskasPath() 
{
	CopySelectedTasksToClipboard(TDCTC_ASPATH);
}

void CToDoListWnd::OnUpdateCopyTaskasPath(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().GetSelectedCount() == 1);
}

BOOL CToDoListWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (CFrameWnd::PreCreateWindow(cs))
	{
		cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

		// Check if our class is already defined
		LPCTSTR pszClassName = _T("ToDoListFrame");
		WNDCLASS wndcls;

		if (!::GetClassInfo(AfxGetInstanceHandle(), pszClassName, &wndcls))
		{
			// Get the current requested window class
			VERIFY(GetClassInfo(AfxGetInstanceHandle(), cs.lpszClass, &wndcls));

			// We want to register this info with our name
			wndcls.lpszClassName = pszClassName;

			// Need to preset the icon otherwise the function GetIconWndClass
			// calling us will overwrite our class.
			wndcls.hIcon = GraphicsMisc::LoadIcon(IDR_MAINFRAME);

			// Register our class now and check the outcome
			if (!::RegisterClass(&wndcls))
			{
				ASSERT(0);
				return FALSE;
			}
		}

		// Now use our class 
		cs.lpszClass = pszClassName;
		return TRUE;
	}
	
	// else
	return FALSE;
}

void CToDoListWnd::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CFrameWnd::OnWindowPosChanging(lpwndpos); 
}

void CToDoListWnd::OnEditInsertdatetime() 
{
	DoInsertDateAndTime(TRUE, TRUE);
}

void CToDoListWnd::OnEditInsertdate() 
{
	DoInsertDateAndTime(TRUE, FALSE);
}

void CToDoListWnd::OnEditInserttime() 
{
	DoInsertDateAndTime(FALSE, TRUE);
}

void CToDoListWnd::DoInsertDateAndTime(BOOL bDate, BOOL bTime) 
{
	COleDateTime date = COleDateTime::GetCurrentTime();
	const CPreferencesDlg& userPrefs = Prefs();

	CString sInsert;

	if (bDate) // date only or date and time
	{
		DWORD dwFmt = (bTime ? DHFD_TIME : 0);

		if (userPrefs.GetShowWeekdayInDates())
			dwFmt |= DHFD_DOW;
						
		if (userPrefs.GetDisplayDatesInISO())
			dwFmt |= DHFD_ISO;
						
		sInsert = CDateHelper::FormatDate(date, dwFmt);
	}
	else // time only
	{
		sInsert = CTimeHelper::FormatClockTime(date, TRUE, userPrefs.GetDisplayDatesInISO());
	}

	// and user name
	if (userPrefs.GetAppendUserToDateTimePaste())
		sInsert += (' ' + m_tdiDefault.sCreatedBy);

	// add trailing text
	sInsert += userPrefs.GetDateTimePasteTrailingText();

	GetToDoCtrl().PasteText(sInsert);
}

void CToDoListWnd::OnUpdateEditInsertdatetime(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanPasteText());
}

void CToDoListWnd::OnUpdateEditInserttime(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanPasteText());
}

void CToDoListWnd::OnUpdateEditInsertdate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanPasteText());
}

void CToDoListWnd::OnSysColorChange() 
{
	CFrameWnd::OnSysColorChange();
	
	InitMenuIconManager();

	SetUITheme(m_sThemeFile);
}

void CToDoListWnd::UpdateSBPaneAndTooltip(UINT nIDPane, UINT nIDTextFormat, const CString& sValue, UINT nIDTooltip, TDC_COLUMN nTDCC)
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	CEnString sText, sTooltip;

	if (tdc.IsColumnShowing(nTDCC))
	{
		sText.Format(nIDTextFormat, sValue);
		sTooltip.LoadString(nIDTooltip);
	}
	else
	{
		sText.Empty();
		sTooltip.Empty();
	}

	int nPane = m_statusBar.CommandToIndex(nIDPane);

	m_statusBar.SetPaneText(nPane, sText);
	m_statusBar.SetPaneTooltipIndex(nPane, sTooltip);
}

void CToDoListWnd::UpdateStatusBarInfo(const CFilteredToDoCtrl& tdc, TDCSTATUSBARINFO& sbi) const
{
	sbi.nSelCount = tdc.GetSelectedCount();
	sbi.dwSelTaskID = tdc.GetSelectedTaskID();
	sbi.dCost = tdc.CalcSelectedTaskCost();

	const CPreferencesDlg& userPrefs = Prefs();

	sbi.nTimeEstUnits = m_tdiDefault.nTimeEstUnits;
	sbi.dTimeEst = tdc.CalcSelectedTaskTimeEstimate(sbi.nTimeEstUnits);

	sbi.nTimeSpentUnits = m_tdiDefault.nTimeSpentUnits;
	sbi.dTimeSpent = tdc.CalcSelectedTaskTimeSpent(sbi.nTimeSpentUnits);
}

void CToDoListWnd::OnUpdateSBSelectionCount(CCmdUI* /*pCmdUI*/)
{
	if (GetTDCCount())
	{
		CFilteredToDoCtrl& tdc = GetToDoCtrl();

		// keep track of previous information to avoid unnecessary processing
		static TDCSTATUSBARINFO sbiPrev;

		TDCSTATUSBARINFO sbi;
		UpdateStatusBarInfo(tdc, sbi);

		if (sbi == sbiPrev)
			return;

		sbiPrev = sbi;

		// number of selected tasks
		CEnString sText;

		if (sbi.nSelCount == 1)
		{
			ASSERT(sbi.dwSelTaskID);
			sText.Format(ID_SB_SELCOUNTONE, sbi.dwSelTaskID);
		}
		else
			sText.Format(ID_SB_SELCOUNT, sbi.nSelCount);

		m_statusBar.SetPaneText(m_statusBar.CommandToIndex(ID_SB_SELCOUNT), sText);

		// times
		const CPreferencesDlg& userPrefs = Prefs();

		// estimate
		TH_UNITS nTHUnits = TDC::MapUnitsToTHUnits(sbi.nTimeEstUnits);

		if (userPrefs.GetUseHMSTimeFormat())
			sText = CTimeHelper().FormatTimeHMS(sbi.dTimeEst, nTHUnits);
		else
			sText = CTimeHelper().FormatTime(sbi.dTimeEst, nTHUnits, 2);

		UpdateSBPaneAndTooltip(ID_SB_SELTIMEEST, ID_SB_SELTIMEEST, sText, IDS_SB_SELTIMEEST_TIP, TDCC_TIMEEST);

		// spent
		nTHUnits = TDC::MapUnitsToTHUnits(sbi.nTimeSpentUnits);

		if (userPrefs.GetUseHMSTimeFormat())
			sText = CTimeHelper().FormatTimeHMS(sbi.dTimeSpent, nTHUnits);
		else
			sText = CTimeHelper().FormatTime(sbi.dTimeSpent, nTHUnits, 2);

		UpdateSBPaneAndTooltip(ID_SB_SELTIMESPENT, ID_SB_SELTIMESPENT, sText, IDS_SB_SELTIMESPENT_TIP, TDCC_TIMESPENT);

		// cost
		sText = Misc::Format(sbi.dCost, 2);
		UpdateSBPaneAndTooltip(ID_SB_SELCOST, ID_SB_SELCOST, sText, IDS_SB_SELCOST_TIP, TDCC_COST);

		// set tray tip too
		UpdateTooltip();
	}
}

void CToDoListWnd::OnUpdateSBTaskCount(CCmdUI* /*pCmdUI*/)
{
	if (GetTDCCount())
	{
		CFilteredToDoCtrl& tdc = GetToDoCtrl();

		UINT nVisibleTasks;
		UINT nTotalTasks = tdc.GetTaskCount(&nVisibleTasks);

		CEnString sText;
		sText.Format(IDS_SB_TASKCOUNT, nVisibleTasks, nTotalTasks);
		int nIndex = m_statusBar.CommandToIndex(ID_SB_TASKCOUNT);

		m_statusBar.SetPaneText(nIndex, sText);
	}
}

void CToDoListWnd::OnEditSelectall() 
{
	GetToDoCtrl().SelectAll();
}

void CToDoListWnd::OnUpdateEditSelectall(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().GetTaskCount());	
}

void CToDoListWnd::OnCloseallbutthis() 
{
	int nThis = GetSelToDoCtrl();
	int nCtrl = GetTDCCount();
	
	// remove tasklists
	while (nCtrl--)
	{
		if (nCtrl != nThis)
		{
			if (ConfirmSaveTaskList(nCtrl, TDLS_CLOSINGTASKLISTS) != TDCF_SUCCESS)
				continue; // user cancelled

			const CFilteredToDoCtrl& tdc = GetToDoCtrl(nCtrl);

			m_findDlg.DeleteResults(&tdc);
			m_dlgTimeTracker.RemoveTasklist(&tdc);

			m_mgrToDoCtrls.RemoveToDoCtrl(nCtrl, TRUE);
		}
	}

	Resize();
}

void CToDoListWnd::OnUpdateCloseallbutthis(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetTDCCount() > 1);
}

void CToDoListWnd::DoSendTasks(BOOL bSelected)
{
	CTDLSendTasksDlg dialog(bSelected, GetToDoCtrl().GetView());

	if (dialog.DoModal() == IDOK)
	{
		// get tasks
		CFilteredToDoCtrl& tdc = GetToDoCtrl();
		CTaskFile tasks;

		GetTasks(tdc, FALSE, FALSE, dialog.GetTaskSelection(), tasks, NULL);

		// package them up
		CString sAttachment, sBody;
		TD_SENDAS nSendAs = dialog.GetSendAs();

		switch (nSendAs)
		{
		case TDSA_TASKLIST:
			{
				CString sFileName, sExt;
				FileMisc::SplitPath(tdc.GetFilePath(), NULL, NULL, &sFileName, &sExt);
				
				sAttachment = FileMisc::GetTempFilePath(sFileName, sExt);
				
				if (!tasks.Save(sAttachment, SFEF_UTF16))
				{
					// TODO
					return;
				}
				
				sBody.LoadString(IDS_TASKLISTATTACHED);
			}
			break;
			
		case TDSA_BODYTEXT:
			sBody = m_mgrImportExport.ExportTaskListToText(&tasks);
			break;
		}
		
		// form subject
		CString sSubject = tdc.GetFriendlyProjectName();
		
		// recipients
		CStringArray aTo;
		tdc.GetSelectedTaskAllocTo(aTo);

		CString sTo = Misc::FormatArray(aTo, _T(";"));

		// prefix with task name if necessary
		if (dialog.GetTaskSelection().GetWantSelectedTasks() && tdc.GetSelectedCount() == 1)
		{
			sSubject = tdc.GetSelectedTaskTitle() + _T(" - ") + sSubject;
		}

		CSendFileTo().SendMail(*this, sTo, sSubject, sBody, sAttachment);
	}
}

void CToDoListWnd::OnSendTasks() 
{
	DoSendTasks(FALSE);
}

void CToDoListWnd::OnUpdateSendTasks(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	pCmdUI->Enable(tdc.GetTaskCount());
}

void CToDoListWnd::OnSendSelectedTasks() 
{
	DoSendTasks(TRUE);
}

void CToDoListWnd::OnUpdateSendSelectedTasks(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	pCmdUI->Enable(tdc.GetTaskCount());
}

void CToDoListWnd::OnEditUndo() 
{
	OnEditUndoRedo(TRUE);
}

void CToDoListWnd::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	OnUpdateEditUndoRedo(pCmdUI, TRUE);
}

void CToDoListWnd::OnEditRedo() 
{
	OnEditUndoRedo(FALSE);
}

void CToDoListWnd::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	OnUpdateEditUndoRedo(pCmdUI, FALSE);
}

void CToDoListWnd::OnEditUndoRedo(BOOL bUndo)
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	tdc.UndoLastAction(bUndo);
	
	UpdateStatusbar();
	UpdateTimeTrackerTasks(tdc, TRUE);
}

void CToDoListWnd::OnUpdateEditUndoRedo(CCmdUI* pCmdUI, BOOL bUndo)
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	pCmdUI->Enable(tdc.CanUndoLastAction(bUndo));
}

void CToDoListWnd::OnViewCycleTaskViews() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	tdc.SetNextView();
	tdc.SetFocusToTasks();
}

void CToDoListWnd::OnUpdateViewCycleTaskViews(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_nMaxState != TDCMS_MAXCOMMENTS);
}

void CToDoListWnd::OnViewToggleTreeandList() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	FTC_VIEW nView = tdc.GetView();

	switch (nView)
	{
	case FTCV_TASKTREE:
		nView = FTCV_TASKLIST;
		break;

	case FTCV_TASKLIST:
	default:
		nView = FTCV_TASKTREE;
		break;
	}

	tdc.SetView(nView);
	tdc.SetFocusToTasks();
}

void CToDoListWnd::OnUpdateViewToggleTreeandList(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_nMaxState != TDCMS_MAXCOMMENTS);
}

void CToDoListWnd::OnViewToggletasksandcomments() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	if (!tdc.TasksHaveFocus())
		tdc.SetFocusToTasks();
	else
		tdc.SetFocusToComments();
}

void CToDoListWnd::OnUpdateViewToggletasksandcomments(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_nMaxState == TDCMS_NORMAL || 
					(m_nMaxState == TDCMS_MAXTASKLIST && Prefs().GetShowCommentsAlways()));
}

void CToDoListWnd::OnUpdateQuickFind(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bShowToolbar);
}

void CToDoListWnd::OnQuickFind() 
{
	if (m_bShowToolbar)
		m_cbQuickFind.SetFocus();
}

void CToDoListWnd::OnQuickFindNext() 
{
	if (!m_sQuickFind.IsEmpty())
	{
		if (!GetToDoCtrl().SelectTask(m_sQuickFind, TDC_SELECTNEXT))
			GetToDoCtrl().SelectTask(m_sQuickFind, TDC_SELECTFIRST); // return to start
	}
}

void CToDoListWnd::OnUpdateQuickFindNext(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_sQuickFind.IsEmpty());
}

LRESULT CToDoListWnd::OnQuickFindItemAdded(WPARAM /*wp*/, LPARAM /*lp*/)
{
	// keep only the last 20 items
	int nItem = m_cbQuickFind.GetCount();

	while (nItem > 20)
	{
		nItem--;
		m_cbQuickFind.DeleteString(nItem);
	}

	return 0L;
}

void CToDoListWnd::OnQuickFindPrev() 
{
	if (!m_sQuickFind.IsEmpty())
	{
		if (!GetToDoCtrl().SelectTask(m_sQuickFind, TDC_SELECTPREV))
			GetToDoCtrl().SelectTask(m_sQuickFind, TDC_SELECTLAST); // return to end
	}
}

void CToDoListWnd::OnUpdateQuickFindPrev(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_sQuickFind.IsEmpty());
}

void CToDoListWnd::OnMove(int x, int y) 
{
	CFrameWnd::OnMove(x, y);
}

void CToDoListWnd::OnEditSettaskicon() 
{
	GetToDoCtrl().EditSelectedTaskIcon();
}

void CToDoListWnd::OnUpdateEditSettaskicon(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	int nSelCount = tdc.GetSelectedCount();
	
	pCmdUI->Enable(nSelCount && !tdc.IsReadOnly());	
}

LRESULT CToDoListWnd::OnToDoCtrlGetTaskReminder(WPARAM wParam, LPARAM lParam)
{
	int nRem = m_reminders.FindReminder(wParam, (CFilteredToDoCtrl*)lParam, FALSE);

	if (nRem == -1)
		return 0L;

	COleDateTime dtRem;

	if (!m_reminders.GetReminderDate(nRem, dtRem))
		return 0xFFFFFFFF; // means the task's start/due date has not yet been set

	time_t tRem = 0;

	if (!CDateHelper::GetTimeT(dtRem, tRem))
	{
		ASSERT(0);
		tRem = 0;
	}

	return (LRESULT)tRem;
}

LRESULT CToDoListWnd::OnToDoCtrlNotifyClickReminderCol(WPARAM /*wp*/, LPARAM /*lp*/)
{
	OnEditSetReminder();
	return 0L;
}

void CToDoListWnd::OnEditSetReminder() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	CTDLSetReminderDlg dialog;
	
	CString sTitle = tdc.GetSelectedTaskTitle();
	
	CDWordArray aTaskIDs;
	int nNumSel = tdc.GetSelectedTaskIDs(aTaskIDs, TRUE);
	
	if (nNumSel == 0)
		return;
	
	// get the first reminder as a reference
	BOOL bNewReminder = TRUE;
	TDCREMINDER rem;
	
	for (int nTask = 0; nTask < nNumSel; nTask++)
	{
		DWORD dwTaskID = aTaskIDs[nTask];
		int nRem = m_reminders.FindReminder(dwTaskID, &tdc);
		
		if (nRem != -1)
		{
			m_reminders.GetReminder(nRem, rem);
			bNewReminder = FALSE;
			break;
		}
	}
	
	// handle new task
	if (bNewReminder)
	{
		rem.dwTaskID = aTaskIDs[0];
		rem.pTDC = &tdc;
	}
	
	if (dialog.DoModal(rem, bNewReminder) == IDOK)
	{
		// apply reminder to selected tasks
		for (int nTask = 0; nTask < nNumSel; nTask++)
		{
			rem.dwTaskID = aTaskIDs[nTask];
			m_reminders.SetReminder(rem);
		}
		
		tdc.RedrawReminders();
	}
}

void CToDoListWnd::OnUpdateEditSetReminder(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	BOOL bEnable = (tdc.GetSelectedCount() > 0) && !tdc.SelectedTasksAreAllDone();
	pCmdUI->Enable(bEnable);
}

void CToDoListWnd::OnEditClearReminder() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	CDWordArray aTaskIDs;
	int nTask = tdc.GetSelectedTaskIDs(aTaskIDs, TRUE);
	
	while (nTask--)
	{
		DWORD dwTaskID = aTaskIDs[nTask];
		m_reminders.ClearReminder(dwTaskID, &tdc);
	}
	
	tdc.RedrawReminders();
}

void CToDoListWnd::OnUpdateEditClearReminder(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	BOOL bEnable = FALSE;
	
	// check at least one selected item has a reminder
	CDWordArray aTaskIDs;
	int nTask = tdc.GetSelectedTaskIDs(aTaskIDs, TRUE);
	
	while (nTask--)
	{
		DWORD dwTaskID = aTaskIDs[nTask];
		
		if (m_reminders.FindReminder(dwTaskID, &tdc) != -1)
		{
			bEnable = TRUE;
			break;
		}
	}
	
	pCmdUI->Enable(bEnable);
}

void CToDoListWnd::OnEditCleartaskicon() 
{
	GetToDoCtrl().ClearSelectedTaskIcon();
}

void CToDoListWnd::OnUpdateEditCleartaskicon(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	int nSelCount = tdc.GetSelectedCount();
	
	pCmdUI->Enable(nSelCount && !tdc.IsReadOnly() && tdc.SelectedTasksHaveIcons());	
}

void CToDoListWnd::OnSortMulti() 
{
	TDSORTCOLUMNS sort;
	CTDCCustomAttribDefinitionArray aAttribDefs;

	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	tdc.GetSortBy(sort);
	tdc.GetCustomAttributeDefs(aAttribDefs);

	// initialise multisort if first time
	if (!sort.IsSorting())
	{
		sort.col1 = tdc.GetSortBy();

		if (sort.col1.bAscending == -1)
			sort.col1.bAscending = TRUE;
	}

	CTDLMultiSortDlg dialog(sort, tdc.GetVisibleColumns(), aAttribDefs);

	if (dialog.DoModal() == IDOK)
	{
		dialog.GetSortBy(sort);
		tdc.MultiSort(sort);
	}
}

void CToDoListWnd::OnViewShowTasklistTabbar() 
{
	m_bShowTasklistBar = !m_bShowTasklistBar; 

	Resize();
	Invalidate(TRUE);
}

void CToDoListWnd::OnUpdateViewShowTasklistTabbar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bShowTasklistBar ? 1 : 0);
}

void CToDoListWnd::OnViewShowTreeListTabbar() 
{
	m_bShowTreeListBar = !m_bShowTreeListBar; 

	GetToDoCtrl().SetStyle(TDCS_SHOWTREELISTBAR, m_bShowTreeListBar);

	// refresh all the other tasklists
	m_mgrToDoCtrls.SetAllNeedPreferenceUpdate(TRUE, GetSelToDoCtrl());
}

void CToDoListWnd::OnUpdateViewShowTreeListTabbar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bShowTreeListBar ? 1 : 0);
}

void CToDoListWnd::OnFileChangePassword() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	if (!tdc.IsReadOnly() && tdc.IsEncrypted() && VerifyToDoCtrlPassword())
	{
		tdc.EnableEncryption(FALSE); // clears the password
		tdc.EnableEncryption(TRUE); // forces it to be re-got
	}
}

void CToDoListWnd::OnUpdateFileChangePassword(CCmdUI* pCmdUI) 
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();

	pCmdUI->Enable(!tdc.IsReadOnly() && tdc.IsEncrypted());
}

void CToDoListWnd::OnTasklistCustomColumns() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	if (!tdc.IsReadOnly())
	{
		CTDLCustomAttributeDlg dialog(tdc, m_theme);

		if (dialog.DoModal() == IDOK)
		{
			CTDCCustomAttribDefinitionArray aAttrib;

			dialog.GetAttributes(aAttrib);
			tdc.SetCustomAttributeDefs(aAttrib);
		}
	}
}

void CToDoListWnd::OnUpdateTasklistCustomcolumns(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetToDoCtrl().IsReadOnly());
}

void CToDoListWnd::OnEditClearAttribute() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	TDC_ATTRIBUTE nAttrib = TDC::MapColumnToAttribute(m_nContextColumnID);

	tdc.ClearSelectedTaskAttribute(nAttrib);
}

void CToDoListWnd::OnUpdateEditClearAttribute(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	TDC_ATTRIBUTE nAttrib = TDC::MapColumnToAttribute(m_nContextColumnID);

	pCmdUI->Enable(tdc.CanClearSelectedTaskAttribute(nAttrib));
}

void CToDoListWnd::OnEditClearFocusedAttribute() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	tdc.ClearSelectedTaskFocusedAttribute();
}

void CToDoListWnd::OnUpdateEditClearFocusedAttribute(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	pCmdUI->Enable(tdc.CanClearSelectedTaskFocusedAttribute());
}

void CToDoListWnd::OnEditSelectReferenceTarget() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	if (!tdc.GotoSelectedReferenceTaskTarget())	
		MessageBox(IDS_NOREFERENCETASKSINSELECTION);
}

void CToDoListWnd::OnUpdateEditSelectReferenceTarget(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	pCmdUI->Enable(tdc.GetSelectedCount() > 0);
}

void CToDoListWnd::OnEditSelectTaskReferences() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	if (!tdc.GotoSelectedTaskReferences())	
		MessageBox(IDS_SELECTIONHASNOREFERENCETASKS);
}

void CToDoListWnd::OnUpdateEditSelectTaskReferences(CCmdUI* pCmdUI) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	pCmdUI->Enable(tdc.GetSelectedCount() > 0);
}

void CToDoListWnd::OnToolsSelectinExplorer() 
{
	int nCtrl = GetSelToDoCtrl();
	
	FileMisc::SelectFileInExplorer(m_mgrToDoCtrls.GetFilePath(nCtrl));
}

void CToDoListWnd::OnUpdateToolsSelectinExplorer(CCmdUI* pCmdUI) 
{
	int nCtrl = GetSelToDoCtrl();
	
	pCmdUI->Enable(!m_mgrToDoCtrls.GetFilePath(nCtrl).IsEmpty());
}

void CToDoListWnd::OnToolsRemovefromsourcecontrol() 
{
	CWaitCursor cursor;
	int nCtrl = GetSelToDoCtrl();

	if (m_mgrToDoCtrls.AddToSourceControl(nCtrl, FALSE))
		UpdateMenuIconMgrSourceControlStatus();
}

void CToDoListWnd::OnUpdateToolsRemovefromsourcecontrol(CCmdUI* pCmdUI) 
{
	int nCtrl = GetSelToDoCtrl();

	pCmdUI->Enable(m_mgrToDoCtrls.IsSourceControlled(nCtrl));
}

void CToDoListWnd::OnToolsAddtoSourceControl() 
{
	CWaitCursor cursor;
	int nCtrl = GetSelToDoCtrl();
	
	if (m_mgrToDoCtrls.AddToSourceControl(nCtrl))
		UpdateMenuIconMgrSourceControlStatus();
}

void CToDoListWnd::OnUpdateToolsAddtoSourceControl(CCmdUI* pCmdUI) 
{
	int nCtrl = GetSelToDoCtrl();
	
	pCmdUI->Enable(!m_mgrToDoCtrls.IsSourceControlled(nCtrl));
}

void CToDoListWnd::OnViewResizeColsToFit() 
{
	GetToDoCtrl().ResizeAttributeColumnsToFit();
}

void CToDoListWnd::OnUpdateViewResizeColsToFit(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanResizeAttributeColumnsToFit());
}

LRESULT CToDoListWnd::OnDismissReminder(WPARAM /*wp*/, LPARAM lp)
{
// 	DWORD dwTaskID = wp; // zero for multiple tasks
	CFilteredToDoCtrl* pTDC = (CFilteredToDoCtrl*)lp;

	ASSERT(pTDC);

	if (pTDC && (*pTDC == GetToDoCtrl()))
	{
		Show(FALSE);
		pTDC->RedrawReminders();
	}

	return 0L;
}

void CToDoListWnd::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	CFrameWnd::OnNcLButtonDblClk(nHitTest, point);
	
	if (nHitTest == HTCAPTION)
	    Resize();
}

void CToDoListWnd::OnViewShowTimeTracker() 
{
	// Select the active tasklist and selected task before showing
	m_dlgTimeTracker.SelectTaskList(&GetToDoCtrl());
	m_dlgTimeTracker.ShowWindow(SW_SHOW);
	m_dlgTimeTracker.SetForegroundWindow();
}

void CToDoListWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	// Prompt to restart app whenever Regional settings change
	if (StrCmp(lpszSection, _T("intl")) == 0)
	{
		if (MessageBox(IDS_RESTARTTOUPDATESETTINGS, 0, MB_YESNO) == IDYES)
		{
			DoExit(TRUE);
			return;
		}
	}
	else if (StrCmp(lpszSection, _T("TraySettings")) == 0)
	{
		// Icons themselves will already have been updated
		// so we just need to handle dependencies
		m_trayIcon.SetIcon(m_icons.GetSmallIcon(), FALSE);
		m_dlgTimeTracker.SetWindowIcons(m_icons.GetBigIcon(), m_icons.GetSmallIcon());
	}
		
	CFrameWnd::OnSettingChange(uFlags, lpszSection);
}

#ifdef _DEBUG
void CToDoListWnd::ShowReminderDlg()
{
	TDCREMINDER rem;
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	rem.dwTaskID = tdc.GetSelectedTaskID();
	rem.pTDC = &tdc;
	rem.bRelative = TRUE;
	rem.dRelativeDaysLeadIn = 0;
	rem.nRelativeFromWhen = TDCR_STARTDATE;
	rem.bEnabled = TRUE;

	m_reminders.SetReminder(rem, TRUE);
}
#endif
