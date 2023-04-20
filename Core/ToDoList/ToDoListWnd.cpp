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
#include "tdlwelcomewizard.h"
#include "tdlanalyseloggedtimedlg.h"
#include "TDCTaskTimeLogAnalysis.h"
#include "TDLAboutDlg.h"
#include "TDCWebUpdateScript.h"
#include "TDCToDoCtrlPreferenceHelper.h"
#include "TaskClipboard.h"
#include "TDLGoToTaskDlg.h"
#include "TDLCleanupIniPreferencesDlg.h"
#include "TDLTasklistSaveAsDlg.h"
#include "TDCAnonymizeTasklist.h"

#include "..\shared\aboutdlg.h"
#include "..\shared\holdredraw.h"
#include "..\shared\autoflag.h"
#include "..\shared\enbitmap.h"
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
#include "..\shared\regkey.h"
#include "..\shared\remotefile.h"
#include "..\shared\serverdlg.h"
#include "..\shared\focuswatcher.h"
#include "..\shared\localizer.h"
#include "..\shared\msoutlookhelper.h"
#include "..\shared\clipboard.h"
#include "..\shared\rtlstylemgr.h"
#include "..\shared\xslfile.h"
#include "..\shared\soundedit.h"
#include "..\shared\messagebox.h"
#include "..\shared\ComboListboxPositioner.h"
#include "..\shared\sendfiletoEx.h"
#include "..\shared\ScopedTimer.h"
#include "..\shared\WorkingWeek.h"
#include "..\shared\FileIcons.h"
#include "..\shared\AcceleratorString.h"

#include "..\3rdparty\gui.h"
#include "..\3rdparty\ShellIcons.h"
#include "..\3rdparty\XNamedColors.h"

#include "..\Interfaces\spellcheckdlg.h"
#include "..\Interfaces\uiextensionhelper.h"
#include "..\Interfaces\TasklistSchemaDef.h"

#include <shlwapi.h>
#include <windowsx.h>
#include <direct.h>
#include <math.h>
#include <afxpriv.h>        // for WM_KICKIDLE

#pragma warning(disable: 4201)
#include <Mmsystem.h> 

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const int BEVEL = 3; // DON'T SCALE
const int MRU_MAX_ITEM_LEN = 128;

const int BORDER = GraphicsMisc::ScaleByDPIFactor(3);
const int SPLITTER_WIDTH = GraphicsMisc::ScaleByDPIFactor(6);

const int QUICKFIND_HEIGHT = GraphicsMisc::ScaleByDPIFactor(200);
const int QUICKFIND_VOFFSET = GraphicsMisc::ScaleByDPIFactor(1);
const int QUICKFIND_HOFFSET = -GraphicsMisc::ScaleByDPIFactor(2);

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
const UINT ONE_MINUTE = 10000;
#else
const UINT ONE_MINUTE = 60000;
#endif

/////////////////////////////////////////////////////////////////////////////

const LPCTSTR SETTINGS_KEY	= _T("Settings");
const LPCTSTR PREF_KEY		= _T("Preferences");
const LPCTSTR ENDL			= _T("\n");
const LPCTSTR TDL_EXT		= _T("tdl");
const LPCTSTR XML_EXT		= _T("xml");
const LPCTSTR HTMLIMG_EXT	= _T("html_images");

static CEnString TDL_FILEFILTER;

/////////////////////////////////////////////////////////////////////////////

const CString TEMP_CLIPBOARD_FILEPATH	= FileMisc::GetTempFilePath(_T("tdl.clipboard"), _T(""));
const CString TEMP_PRINT_FILEPATH		= FileMisc::GetTempFilePath(_T("tdl.print"), _T("html"));
const CString TEMP_TASKVIEW_FILEPATH	= FileMisc::GetTempFilePath(_T("tdl.view"), _T("png"));

/////////////////////////////////////////////////////////////////////////////

#define DOPROGRESS(stringID) \
	CWaitCursor cursor; \
	CTDLStatusBarProgressProxy prog(m_statusBar, CEnString(stringID))

/////////////////////////////////////////////////////////////////////////////

enum
{
	WM_POSTONCREATE				= (WM_APP+1),
	WM_UPDATEUDTSINTOOLBAR,
	WM_APPRESTOREFOCUS,
	WM_DOINITIALDUETASKNOTIFY,
};

/////////////////////////////////////////////////////////////////////////////

enum 
{
	TIMER_READONLYSTATUS = 1,
	TIMER_TIMESTAMPCHANGE,
	TIMER_AUTOSAVE,
	TIMER_CHECKOUTSTATUS,
	TIMER_DUEITEMS,
	TIMER_TIMETRACKING,
	TIMER_AUTOMINIMIZE,
};

/////////////////////////////////////////////////////////////////////////////

enum 
{
	INTERVAL_READONLYSTATUS		= 1000,
	INTERVAL_TIMESTAMPCHANGE	= 10000,
	// INTERVAL_AUTOSAVE		= calculated
	INTERVAL_CHECKOUTSTATUS		= 5000,
	INTERVAL_DUEITEMS			= ONE_MINUTE,
	INTERVAL_TIMETRACKING		= 5000,
	// INTERVAL_AUTOMINIMIZE	= calculated
};

/////////////////////////////////////////////////////////////////////////////
// CToDoListWnd 

CToDoListWnd::CToDoListWnd() 
	: 
	CFrameWnd(), 
	m_bVisible(-1), 
	m_mruList(0, _T("MRU"), _T("TaskList%d"), 16, MRU_MAX_ITEM_LEN, CEnString(IDS_RECENTFILES)),
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
	m_tabCtrl(TCE_POSTDRAW | TCE_MBUTTONCLOSE | TCE_DRAGDROP | TCE_CLOSEBUTTON | TCE_BOLDSELTEXT | TCE_TABCOLORS),
	m_mgrToDoCtrls(m_tabCtrl),
	m_bFindShowing(FALSE),
	m_bShowProjectName(TRUE),
	m_bQueryOpenAllow(FALSE),
	m_bPasswordPrompting(TRUE),
	m_bShowingMainToolbar(TRUE),
	m_bShowingCustomToolbar(TRUE),
	m_bReloading(FALSE),
	m_hwndLastFocus(NULL),
	m_bStartHidden(FALSE),
	m_cbQuickFind(ACBS_ALLOWDELETE | ACBS_ADDTOSTART | ACBS_AUTOCOMPLETE),
	m_bShowTasklistBar(TRUE), 
	m_bShowTreeListBar(TRUE),
	m_bEndingSession(FALSE),
	m_bSettingAttribDefs(FALSE),
	m_bReshowTimeTrackerOnEnable(FALSE),
	m_bPromptLanguageChangeRestartOnActivate(FALSE),
	m_bIgnoreResize(FALSE),
	m_bAllowForcedCheckOut(FALSE),
	m_nContextColumnID(TDCC_NONE),
	m_nContextMenuID(0),
	m_bFirstEraseBkgnd(TRUE),
	m_bLogCommands(FALSE),
	m_bSplitting(FALSE),
	m_statusBar(m_tdiDefault),
	m_findFilterHelper(m_dlgFindTasks, m_filterBar)
{
	TDL_FILEFILTER.LoadString(IDS_TDLFILEFILTER);
	
	// must do this before initializing any controls
	SetupUIStrings();
	
	// init preferences
	ResetPrefs();
	InitUITheme();

	// RTL keyboard input
	if (Prefs().GetEnableRTLInput())
		CRTLStyleMgr::Initialize();

	CFilteredToDoCtrl::EnableExtendedSelection(FALSE, TRUE);

	m_bAutoMenuEnable = FALSE;
	m_nFlags |= WF_STAYACTIVE;
}

CToDoListWnd::~CToDoListWnd()
{
	delete m_pPrefs;

	m_fontComments.DeleteObject();
	m_fontMain.DeleteObject();
	m_fontTree.DeleteObject();

	ShellIcons::Release();

	// cleanup temp files
	// Note: Due task notifications are removed by CToDoCtrlMgr
	FileMisc::DeleteFile(TEMP_CLIPBOARD_FILEPATH, TRUE);
	FileMisc::DeleteFile(TEMP_PRINT_FILEPATH, TRUE);
	FileMisc::DeleteFile(TEMP_TASKVIEW_FILEPATH, TRUE);

	CString sHtmlImgFolder = GetHtmlImageFolder(TRUE, TEMP_PRINT_FILEPATH);
	FileMisc::DeleteFolder(sHtmlImgFolder, FMDF_SUBFOLDERS | FMDF_ALLOWDELETEONREBOOT | FMDF_HIDDENREADONLY);
}

BEGIN_MESSAGE_MAP(CToDoListWnd, CFrameWnd)
//{{AFX_MSG_MAP(CToDoListWnd)
	//}}AFX_MSG_MAP

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
	ON_COMMAND(ID_EDIT_CLEARTASKLISTCOLOR, OnEditClearTasklistTabColor)
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
	ON_COMMAND(ID_EDIT_DEPENDENCY, OnEditDependency)
	ON_COMMAND(ID_EDIT_FINDREPLACEINTASKCOMMENTS, OnEditFindReplaceInTaskComments)
	ON_COMMAND(ID_EDIT_FINDREPLACEINTASKTITLES, OnEditFindReplaceInTaskTitles)
	ON_COMMAND(ID_EDIT_FINDTASKS, OnFindTasks)
	ON_COMMAND(ID_EDIT_FLAGTASK, OnEditFlagtask)
	ON_COMMAND(ID_EDIT_GOTODEPENDENCY, OnEditGotoDependency)
	ON_COMMAND(ID_EDIT_INCTASKPERCENTDONE, OnEditInctaskpercentdone)
	ON_COMMAND(ID_EDIT_INCTASKPRIORITY, OnEditInctaskpriority)
	ON_COMMAND(ID_EDIT_INSERTDATE, OnEditInsertdate)
	ON_COMMAND(ID_EDIT_INSERTDATETIME, OnEditInsertdatetime)
	ON_COMMAND(ID_EDIT_INSERTTIME, OnEditInserttime)
	ON_COMMAND(ID_EDIT_LOCKTASK, OnEditLocktask)
	ON_COMMAND(ID_EDIT_OFFSETDATES, OnEditOffsetDates)
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
	ON_COMMAND(ID_EDIT_SETFILELINK, OnEditAddFileLink)
	ON_COMMAND(ID_EDIT_SETPERCENTTOTODAY, OnEditSetPercentToToday)
	ON_COMMAND(ID_EDIT_SETREMINDER, OnEditSetReminder)
	ON_COMMAND(ID_EDIT_SETTASKICON, OnEditSettaskicon)
	ON_COMMAND(ID_EDIT_SETTASKLISTCOLOR, OnEditSetTasklistTabColor)
	ON_COMMAND(ID_EDIT_SPELLCHECKCOMMENTS, OnSpellcheckcomments)
	ON_COMMAND(ID_EDIT_SPELLCHECKTITLE, OnSpellchecktitle)
	ON_COMMAND(ID_EDIT_TASKCOLOR, OnEditTaskcolor)
	ON_COMMAND(ID_EDIT_TOGGLETASKDONE, OnEditToggleTaskDone)
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
	ON_COMMAND(ID_MOVE_GOTOTASK, OnMoveGoToTask)
	ON_COMMAND(ID_MOVE_SELECTTASKDEPENDENCIES, OnMoveSelectTaskDependencies)
	ON_COMMAND(ID_MOVE_SELECTTASKDEPENDENTS, OnMoveSelectTaskDependents)
	ON_COMMAND(ID_NEW, OnNewTasklist)
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
	ON_COMMAND(ID_SORTBY_MULTI, OnSortMulti)
	ON_COMMAND(ID_TABCTRL_PREFERENCES, OnTabctrlPreferences)
	ON_COMMAND(ID_TASKLIST_COPYCOLUMNVALUES, OnTasklistCopyColumnValues)
	ON_COMMAND(ID_TASKLIST_COPYSELTASKSCOLUMNVALUES, OnTasklistCopySelectedTaskColumnValues)
	ON_COMMAND(ID_TASKLIST_CUSTOMCOLUMNS, OnTasklistCustomColumns)
	ON_COMMAND(ID_TASKLIST_SELECTCOLUMNS, OnTasklistSelectColumns)
	ON_COMMAND(ID_TOOLS_ADDTOSOURCECONTROL, OnToolsAddtoSourceControl)
	ON_COMMAND(ID_TOOLS_ANALYSELOGGEDTIME, OnToolsAnalyseLoggedTime)
	ON_COMMAND(ID_TOOLS_ANONYMIZE_TASKLIST, OnToolsAnonymizeTasklist)
	ON_COMMAND(ID_TOOLS_CHECKIN, OnToolsCheckin)
	ON_COMMAND(ID_TOOLS_CHECKOUT, OnToolsCheckout)
	ON_COMMAND(ID_TOOLS_CLEANUPINIPREFERENCES, OnToolsCleanupIniPreferences)
	ON_COMMAND(ID_TOOLS_COPYTASKLISTPATH, OnToolsCopyTasklistPath)
	ON_COMMAND(ID_TOOLS_EXPORT, OnExport)
	ON_COMMAND(ID_TOOLS_IMPORT, OnImportTasklist)
	ON_COMMAND(ID_TOOLS_REMOVEFROMSOURCECONTROL, OnToolsRemovefromsourcecontrol)
	ON_COMMAND(ID_TOOLS_SELECTINEXPLORER, OnToolsSelectinExplorer)
	ON_COMMAND(ID_TOOLS_SPELLCHECKTASKLIST, OnSpellcheckTasklist)
	ON_COMMAND(ID_TOOLS_TOGGLECHECKIN, OnToolsToggleCheckin)
	ON_COMMAND(ID_TOOLS_TOGGLE_LOGGING, OnToolsToggleLogging)
	ON_COMMAND(ID_TOOLS_TRANSFORM, OnToolsTransformactivetasklist)
	ON_COMMAND(ID_TOOLS_VIEWLOG, OnToolsViewLogFile)
	ON_COMMAND(ID_TRAYICON_CLOSE, OnTrayiconClose)
	ON_COMMAND(ID_TRAYICON_CREATETASK, OnTrayiconCreatetask)
	ON_COMMAND(ID_TRAYICON_SHOW, OnTrayiconShow)
	ON_COMMAND(ID_TRAYICON_SHOWTIMETRACKER, OnViewShowTimeTracker)
	ON_COMMAND(ID_VIEW_CLEARFILTER, OnViewClearfilter)
	ON_COMMAND(ID_VIEW_CUSTOMTOOLBAR, OnViewCustomToolbar)
	ON_COMMAND(ID_VIEW_CYCLETASKVIEWS, OnViewCycleTaskViews)
	ON_COMMAND(ID_VIEW_DECREMENTTASKVIEWFONTSIZE, OnViewDecrementTaskViewFontSize)
	ON_COMMAND(ID_VIEW_FILTER, OnViewFilter)
	ON_COMMAND(ID_VIEW_HIDEALLBARS, OnViewHideAllBars)
	ON_COMMAND(ID_VIEW_INCREMENTTASKVIEWFONTSIZE, OnViewIncrementTaskViewFontSize)
	ON_COMMAND(ID_VIEW_MAINTOOLBAR, OnViewMainToolbar)
	ON_COMMAND(ID_VIEW_MOVETASKLISTLEFT, OnViewMovetasklistleft)
	ON_COMMAND(ID_VIEW_MOVETASKLISTRIGHT, OnViewMovetasklistright)
	ON_COMMAND(ID_VIEW_NEXT, OnViewNextTasklist)
	ON_COMMAND(ID_VIEW_NEXT_SEL, OnViewNextSelectedTask)
	ON_COMMAND(ID_VIEW_PREV, OnViewPrevTasklist)
	ON_COMMAND(ID_VIEW_PREV_SEL, OnViewPrevSelectedTask)
	ON_COMMAND(ID_VIEW_PROJECTNAME, OnViewProjectname)
	ON_COMMAND(ID_VIEW_REFRESHFILTER, OnViewRefreshfilter)
	ON_COMMAND(ID_VIEW_RESIZECOLSTOFIT, OnViewResizeColsToFit)
	ON_COMMAND(ID_VIEW_RESTOREDEFAULTTASKVIEWFONTSIZE, OnViewRestoreDefaultTaskViewFontSize)
	ON_COMMAND(ID_VIEW_SAVETOIMAGE, OnViewSaveToImage)
	ON_COMMAND(ID_VIEW_SHOWALLBARS, OnViewShowAllBars)
	ON_COMMAND(ID_VIEW_SHOWFILTERBAR, OnViewShowfilterbar)
	ON_COMMAND(ID_VIEW_SHOWREMINDERS, OnViewShowRemindersWindow)
	ON_COMMAND(ID_VIEW_SHOWTASKLISTTABBAR, OnViewShowTasklistTabbar)
	ON_COMMAND(ID_VIEW_SHOWTIMETRACKER, OnViewShowTimeTracker)
	ON_COMMAND(ID_VIEW_SHOWTREELISTTABBAR, OnViewShowTreeListTabbar)
	ON_COMMAND(ID_VIEW_SORTTASKLISTTABS, OnViewSorttasklisttabs)
	ON_COMMAND(ID_VIEW_STATUS_BAR, OnViewStatusBar)
	ON_COMMAND(ID_VIEW_TOGGLEFILTER, OnViewTogglefilter)
	ON_COMMAND(ID_VIEW_TOGGLETASKSANDCOMMENTS, OnViewToggletasksandcomments)
	ON_COMMAND(ID_VIEW_TOGGLETREEANDLIST, OnViewToggleTreeandList)

	ON_COMMAND_EX_RANGE(ID_FILE_MRU1, ID_FILE_MRU16, OnOpenRecentFile)

	ON_COMMAND_RANGE(ID_ACTIVATEVIEW_TASKTREE, ID_ACTIVATEVIEW_UIEXTENSION16, OnActivateTaskView)
	ON_COMMAND_RANGE(ID_EDIT_OPENFILELINK1, ID_EDIT_OPENFILELINK16, OnEditOpenFileLink)
	ON_COMMAND_RANGE(ID_EDIT_SETPRIORITYNONE, ID_EDIT_SETPRIORITY10, OnSetPriority)
	ON_COMMAND_RANGE(ID_FILE_OPEN_USERSTORAGE1, ID_FILE_OPEN_USERSTORAGE16, OnFileOpenFromUserStorage)
	ON_COMMAND_RANGE(ID_FILE_SAVE_USERSTORAGE1, ID_FILE_SAVE_USERSTORAGE16, OnFileSaveToUserStorage)
	ON_COMMAND_RANGE(ID_NEWTASK, ID_NEWSUBTASK, OnNewTask)
	ON_COMMAND_RANGE(ID_NEWTASK_ATTOP, ID_NEWSUBTASK_ATBOTTOM, OnNewTask)
	ON_COMMAND_RANGE(ID_NEWTASK_ATTOPSELECTED, ID_NEWTASK_ATBOTTOMSELECTED, OnNewTask)
	ON_COMMAND_RANGE(ID_NEWTASK_DEPENDENTAFTERSELECTEDTASK, ID_NEWTASK_DEPENDENTBEFORESELECTEDTASK, OnNewTask)
	ON_COMMAND_RANGE(ID_OFFSETDATES_BACKWARDSBY_ONEDAY, ID_OFFSETDATES_BACKWARDSBY_ONEYEAR, OnEditOffsetStartDueDatesBackwards)
	ON_COMMAND_RANGE(ID_OFFSETDATES_FORWARDSBY_ONEDAY, ID_OFFSETDATES_FORWARDSBY_ONEYEAR, OnEditOffsetStartDueDatesForwards)
	ON_COMMAND_RANGE(ID_SHOWVIEW_TASKTREE, ID_SHOWVIEW_UIEXTENSION16, OnShowTaskView)
	ON_COMMAND_RANGE(ID_SORTBY_ALLCOLUMNS_FIRST, ID_SORTBY_ALLCOLUMNS_LAST, OnSortBy)
	ON_COMMAND_RANGE(ID_SPLITTASKINTO_TWO, ID_SPLITTASKINTO_FIVE, OnSplitTask)
	ON_COMMAND_RANGE(ID_TOOLS_SHOWTASKS_DUETODAY, ID_TOOLS_SHOWTASKS_DUEENDNEXTMONTH, OnToolsShowtasksDue)
	ON_COMMAND_RANGE(ID_TOOLS_USERTOOL1, ID_TOOLS_USERTOOL50, OnUserTool)
	ON_COMMAND_RANGE(ID_TRAYICON_SHOWDUETASKS1, ID_TRAYICON_SHOWDUETASKS20, OnTrayiconShowDueTasks)
	ON_COMMAND_RANGE(ID_VIEW_ACTIVATEADVANCEDFILTER1, ID_VIEW_ACTIVATEADVANCEDFILTER24, OnViewActivateAdvancedFilter)
	ON_COMMAND_RANGE(ID_VIEW_ACTIVATEFILTER1, ID_VIEW_ACTIVATEFILTER24, OnViewActivateFilter)
	ON_COMMAND_RANGE(ID_VIEW_EXPANDDUE, ID_VIEW_COLLAPSESTARTED, OnViewExpandTasks)
	ON_COMMAND_RANGE(ID_VIEW_EXPANDTASK, ID_VIEW_COLLAPSEALL, OnViewExpandTasks)
	ON_COMMAND_RANGE(ID_VIEW_TOGGLEALLTASKEXPANDED, ID_VIEW_TOGGLEALLTASKEXPANDED, OnViewExpandTasks)
	ON_COMMAND_RANGE(ID_VIEW_TOGGLETASKEXPANDED, ID_VIEW_TOGGLETASKEXPANDED, OnViewExpandTasks)
	ON_COMMAND_RANGE(ID_WINDOW1, ID_WINDOW16, OnWindow)

	ON_MESSAGE(WM_APPRESTOREFOCUS, OnAppRestoreFocus)
	ON_MESSAGE(WM_CLOSE, OnClose)
	ON_MESSAGE(WM_DOINITIALDUETASKNOTIFY, OnDoInitialDueTaskNotify)
	ON_MESSAGE(WM_GETFONT, OnGetFont)
	ON_MESSAGE(WM_GETICON, OnGetIcon)
	ON_MESSAGE(WM_HOTKEY, OnHotkey)
	ON_MESSAGE(WM_POSTONCREATE, OnPostOnCreate)
	ON_MESSAGE(WM_UPDATEUDTSINTOOLBAR, OnUpdateUDTsInToolbar)

	ON_NOTIFY(NM_CLICK, IDC_TRAYICON, OnTrayIconClick)
	ON_NOTIFY(NM_DBLCLK, IDC_TRAYICON, OnTrayIconDblClk)
	ON_NOTIFY(NM_RCLICK, IDC_TRAYICON, OnTrayIconRClick)
	ON_NOTIFY(TCN_CLOSETAB, IDC_TABCONTROL, OnTabCtrlCloseTab)
	ON_NOTIFY(TCN_ENDDRAG, IDC_TABCONTROL, OnTabCtrlEndDrag)
	ON_NOTIFY(TCN_GETBACKCOLOR, IDC_TABCONTROL, OnTabCtrlGetBackColor)
	ON_NOTIFY(TCN_POSTDRAW, IDC_TABCONTROL, OnTabCtrlPostDrawTab)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCONTROL, OnTabCtrlSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TABCONTROL, OnTabCtrlSelchanging)
	ON_NOTIFY(TTN_NEEDTEXT, 0, OnNeedTooltipText)

	ON_REGISTERED_MESSAGE(WM_FBN_FILTERCHANGE, OnSelchangeFilter)
	ON_REGISTERED_MESSAGE(WM_FTD_ADDSEARCH, OnFindAddSearch)
	ON_REGISTERED_MESSAGE(WM_FTD_APPLYASFILTER, OnFindApplyAsFilter)
	ON_REGISTERED_MESSAGE(WM_FTD_CLOSE, OnFindDlgClose)
	ON_REGISTERED_MESSAGE(WM_FTD_DELETESEARCH, OnFindDeleteSearch)
	ON_REGISTERED_MESSAGE(WM_FTD_DOCKCHANGE, OnFindDlgDockChange)
	ON_REGISTERED_MESSAGE(WM_FTD_FIND, OnFindDlgFind)
	ON_REGISTERED_MESSAGE(WM_FTD_SAVESEARCH, OnFindSaveSearch)
	ON_REGISTERED_MESSAGE(WM_FTD_SELECTALL, OnFindSelectAll)
	ON_REGISTERED_MESSAGE(WM_FTD_SELECTRESULT, OnFindSelectResult)
	ON_REGISTERED_MESSAGE(WM_FW_FOCUSCHANGE, OnFocusChange)
	ON_REGISTERED_MESSAGE(WM_ITT_POSTTRANSLATEMENU, OnPostTranslateMenu)
	ON_REGISTERED_MESSAGE(WM_PGP_CLEARMRU, OnPreferencesClearMRU)
	ON_REGISTERED_MESSAGE(WM_PGP_EDITLANGFILE, OnPreferencesEditLanguageFile)
	ON_REGISTERED_MESSAGE(WM_PTP_TESTTOOL, OnPreferencesTestTool)
	ON_REGISTERED_MESSAGE(WM_SESSIONSTATUS_CHANGE, OnSessionStatusChange)
	ON_REGISTERED_MESSAGE(WM_TDCM_CANIMPORTFROMDROP, OnToDoCtrlCanImportFromDrop)
	ON_REGISTERED_MESSAGE(WM_TDCM_COMPLETETASK, OnReminderCompleteTask)
	ON_REGISTERED_MESSAGE(WM_TDCM_FAILEDLINK, OnToDoCtrlFailedLink)
	ON_REGISTERED_MESSAGE(WM_TDCM_GETLINKTOOLTIP, OnToDoCtrlGetLinkTooltip)
	ON_REGISTERED_MESSAGE(WM_TDCM_GETTASKREMINDER, OnToDoCtrlGetTaskReminder)
	ON_REGISTERED_MESSAGE(WM_TDCM_IMPORTFROMDROP, OnToDoCtrlImportFromDrop)
	ON_REGISTERED_MESSAGE(WM_TDCM_ISTASKDONE, OnToDoCtrlIsTaskDone)
	ON_REGISTERED_MESSAGE(WM_TDCM_LENGTHYOPERATION, OnToDoCtrlDoLengthyOperation)
	ON_REGISTERED_MESSAGE(WM_TDCM_SELECTTASK, OnToDoCtrlSelectTask)
	ON_REGISTERED_MESSAGE(WM_TDCN_CLICKREMINDERCOL, OnToDoCtrlNotifyClickReminderCol)
	ON_REGISTERED_MESSAGE(WM_TDCN_LISTCHANGE, OnToDoCtrlNotifyListChange)
	ON_REGISTERED_MESSAGE(WM_TDCN_MODIFY, OnToDoCtrlNotifyMod)
	ON_REGISTERED_MESSAGE(WM_TDCN_FILTERCHANGE, OnToDoCtrlNotifyFilterChange)
	ON_REGISTERED_MESSAGE(WM_TDCN_RECREATERECURRINGTASK, OnToDoCtrlNotifyRecreateRecurringTask)
	ON_REGISTERED_MESSAGE(WM_TDCN_REMINDERDISMISS, OnNotifyReminderModified)
	ON_REGISTERED_MESSAGE(WM_TDCN_REMINDERSNOOZE, OnNotifyReminderModified)
	ON_REGISTERED_MESSAGE(WM_TDCN_SELECTIONCHANGE, OnToDoCtrlNotifySelChange)
	ON_REGISTERED_MESSAGE(WM_TDCN_SOURCECONTROLSAVE, OnToDoCtrlNotifySourceControlSave)
	ON_REGISTERED_MESSAGE(WM_TDCN_TIMETRACK, OnToDoCtrlNotifyTimeTrack)
	ON_REGISTERED_MESSAGE(WM_TDCN_TIMETRACKREMINDER, OnToDoCtrlNotifyTimeTrackReminder)
	ON_REGISTERED_MESSAGE(WM_TDCN_VIEWPOSTCHANGE, OnToDoCtrlNotifyViewChange)
	ON_REGISTERED_MESSAGE(WM_TDLKS_MODIFYSHORTCUTS, OnModifyKeyboardShortcuts)
	ON_REGISTERED_MESSAGE(WM_TDLTE_EXPORTTHREADFINISHED, OnExportThreadFinished)
	ON_REGISTERED_MESSAGE(WM_TDLTTN_LOADDELAYEDTASKLIST, OnTimeTrackerLoadDelayedTasklist)
	ON_REGISTERED_MESSAGE(WM_TDLTTN_RESETELAPSEDTIME, OnTimeTrackerResetElapsedTime)
	ON_REGISTERED_MESSAGE(WM_TDLTTN_STARTTRACKING, OnTimeTrackerStartTracking)
	ON_REGISTERED_MESSAGE(WM_TDLTTN_STOPTRACKING, OnTimeTrackerStopTracking)
	ON_REGISTERED_MESSAGE(WM_TDL_ISCLOSING, OnToDoListIsClosing)
	ON_REGISTERED_MESSAGE(WM_TDL_REFRESHPREFS, OnToDoListRefreshPrefs)
	ON_REGISTERED_MESSAGE(WM_TDL_PASSWORDRESTORE, OnToDoListPasswordRestore)
	ON_REGISTERED_MESSAGE(WM_TDL_SHOWWINDOW, OnToDoListShowWindow)
	ON_REGISTERED_MESSAGE(WM_TLDT_CANDROP, OnCanDropFile)
	ON_REGISTERED_MESSAGE(WM_TLDT_DROP, OnDropFile)

	ON_UPDATE_COMMAND_UI(ID_ADDTIMETOLOGFILE, OnUpdateAddtimetologfile)
	ON_UPDATE_COMMAND_UI(ID_ADDTIMETOLOGFILE, OnUpdateAddtimetologfile)
	ON_UPDATE_COMMAND_UI(ID_ARCHIVE_COMPLETEDTASKS, OnUpdateArchiveCompletedtasks)
	ON_UPDATE_COMMAND_UI(ID_ARCHIVE_SELECTEDTASKS, OnUpdateArchiveSelectedCompletedTasks)
	ON_UPDATE_COMMAND_UI(ID_CLOSEALL, OnUpdateCloseall)
	ON_UPDATE_COMMAND_UI(ID_CLOSEALLBUTTHIS, OnUpdateCloseallbutthis)
	ON_UPDATE_COMMAND_UI(ID_COMMENTS_INSERTDATE, OnUpdateCommentsInsertDateAndOrTime)
	ON_UPDATE_COMMAND_UI(ID_COMMENTS_INSERTDATETIME, OnUpdateCommentsInsertDateAndOrTime)
	ON_UPDATE_COMMAND_UI(ID_COMMENTS_INSERTTIME, OnUpdateCommentsInsertDateAndOrTime)
	ON_UPDATE_COMMAND_UI(ID_DELETEALLTASKS, OnUpdateDeletealltasks)
	ON_UPDATE_COMMAND_UI(ID_DELETETASK, OnUpdateDeletetask)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEARFIELD, OnUpdateEditClearAttribute)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEARFOCUSEDFIELD, OnUpdateEditClearFocusedAttribute)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEARREMINDER, OnUpdateEditClearReminder)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEARTASKCOLOR, OnUpdateEditCleartaskcolor)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEARTASKICON, OnUpdateEditCleartaskicon)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEARTASKLISTCOLOR, OnUpdateEditClearTasklistTabColor)
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
	ON_UPDATE_COMMAND_UI(ID_EDIT_DEPENDENCY, OnUpdateEditDependency)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FINDREPLACEINTASKCOMMENTS, OnUpdateEditFindReplaceInTaskComments)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FINDREPLACEINTASKTITLES, OnUpdateEditFindReplaceInTaskTitles)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FLAGTASK, OnUpdateEditFlagtask)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GOTODEPENDENCY, OnUpdateEditGotoDependency)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INCTASKPERCENTDONE, OnUpdateEditInctaskpercentdone)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INCTASKPRIORITY, OnUpdateEditInctaskpriority)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INSERTDATE, OnUpdateEditInsertDateAndOrTime)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INSERTDATETIME, OnUpdateEditInsertDateAndOrTime)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INSERTTIME, OnUpdateEditInsertDateAndOrTime)
	ON_UPDATE_COMMAND_UI(ID_EDIT_LOCKTASK, OnUpdateEditLocktask)
	ON_UPDATE_COMMAND_UI(ID_EDIT_OFFSETDATES, OnUpdateEditOffsetDates)
	ON_UPDATE_COMMAND_UI(ID_EDIT_OPENFILELINK1, OnUpdateEditOpenFileLink)
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
	ON_UPDATE_COMMAND_UI(ID_EDIT_SETFILELINK, OnUpdateEditAddFileLink)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SETPERCENTTOTODAY, OnUpdateEditSetPercentToToday)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SETREMINDER, OnUpdateEditSetReminder)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SETTASKICON, OnUpdateEditSettaskicon)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SPELLCHECKCOMMENTS, OnUpdateSpellcheckcomments)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SPELLCHECKTITLE, OnUpdateSpellchecktitle)
	ON_UPDATE_COMMAND_UI(ID_EDIT_TASKCOLOR, OnUpdateTaskcolor)
	ON_UPDATE_COMMAND_UI(ID_EDIT_TOGGLETASKDONE, OnUpdateEditToggleTaskDone)
	ON_UPDATE_COMMAND_UI(ID_EDIT_TASKTEXT, OnUpdateEditTasktext)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_FILE_CHANGEPASSWORD, OnUpdateFileChangePassword)
	ON_UPDATE_COMMAND_UI(ID_FILE_ENCRYPT, OnUpdateFileEncrypt)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU1, OnUpdateRecentFileMenu)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPENARCHIVE, OnUpdateFileOpenarchive)
	ON_UPDATE_COMMAND_UI(ID_FILE_RESETVERSION, OnUpdateFileResetversion)
	ON_UPDATE_COMMAND_UI(ID_MAXCOMMENTS, OnUpdateMaximizeComments)
	ON_UPDATE_COMMAND_UI(ID_MAXTASKLIST, OnUpdateMaximizeTasklist)
	ON_UPDATE_COMMAND_UI(ID_MOVETASKDOWN, OnUpdateMovetaskdown)
	ON_UPDATE_COMMAND_UI(ID_MOVETASKLEFT, OnUpdateMovetaskleft)
	ON_UPDATE_COMMAND_UI(ID_MOVETASKRIGHT, OnUpdateMovetaskright)
	ON_UPDATE_COMMAND_UI(ID_MOVETASKUP, OnUpdateMovetaskup)
	ON_UPDATE_COMMAND_UI(ID_MOVE_GOTOTASK, OnUpdateMoveGoToTask)
	ON_UPDATE_COMMAND_UI(ID_MOVE_SELECTTASKDEPENDENCIES, OnUpdateMoveSelectTaskDependencies)
	ON_UPDATE_COMMAND_UI(ID_MOVE_SELECTTASKDEPENDENTS, OnUpdateMoveSelectTaskDependents)
	ON_UPDATE_COMMAND_UI(ID_NEW, OnUpdateNewTasklist)
	ON_UPDATE_COMMAND_UI(ID_NEXTTASK, OnUpdateGotoNexttask)
	ON_UPDATE_COMMAND_UI(ID_NEXTTOPLEVELTASK, OnUpdateNexttopleveltask)
	ON_UPDATE_COMMAND_UI(ID_OPEN_RELOAD, OnUpdateReload)
	ON_UPDATE_COMMAND_UI(ID_PREVTASK, OnUpdateGotoPrevtask)
	ON_UPDATE_COMMAND_UI(ID_PREVTOPLEVELTASK, OnUpdatePrevtopleveltask)
	ON_UPDATE_COMMAND_UI(ID_PRINT, OnUpdatePrint)
	ON_UPDATE_COMMAND_UI(ID_SAVEALL, OnUpdateSaveall)
	ON_UPDATE_COMMAND_UI(ID_SAVEAS, OnUpdateSaveas)
	ON_UPDATE_COMMAND_UI(ID_SAVE_NORMAL, OnUpdateSave)
	ON_UPDATE_COMMAND_UI(ID_SENDTASKS, OnUpdateSendTasks)
	ON_UPDATE_COMMAND_UI(ID_SEND_SELTASKS, OnUpdateSendSelectedTasks)
	ON_UPDATE_COMMAND_UI(ID_SHOWTIMELOGFILE, OnUpdateShowTimelogfile)
	ON_UPDATE_COMMAND_UI(ID_SORT, OnUpdateSort)
	ON_UPDATE_COMMAND_UI(ID_TASKLIST_COPYCOLUMNVALUES, OnUpdateTasklistCopyColumnValues)
	ON_UPDATE_COMMAND_UI(ID_TASKLIST_COPYSELTASKSCOLUMNVALUES, OnUpdateTasklistCopySelectedTaskColumnValues)
	ON_UPDATE_COMMAND_UI(ID_TASKLIST_CUSTOMCOLUMNS, OnUpdateTasklistCustomcolumns)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ADDTOSOURCECONTROL, OnUpdateToolsAddtoSourceControl)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ANALYSELOGGEDTIME, OnUpdateToolsAnalyseLoggedTime)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ANONYMIZE_TASKLIST, OnUpdateToolsAnonymizeTasklist)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_CHECKIN, OnUpdateToolsCheckin)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_CHECKOUT, OnUpdateToolsCheckout)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_CLEANUPINIPREFERENCES, OnUpdateToolsCleanupIniPreferences)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_COPYTASKLISTPATH, OnUpdateToolsCopyTasklistPath)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_EXPORT, OnUpdateExport)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_IMPORT, OnUpdateImport)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_REMOVEFROMSOURCECONTROL, OnUpdateToolsRemovefromsourcecontrol)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_SELECTINEXPLORER, OnUpdateToolsSelectinExplorer)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_SPELLCHECKTASKLIST, OnUpdateSpellcheckTasklist)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_TOGGLECHECKIN, OnUpdateToolsToggleCheckin)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_TOGGLE_LOGGING, OnUpdateToolsToggleLogging)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_TRANSFORM, OnUpdateExport) // use same text as export
	ON_UPDATE_COMMAND_UI(ID_TOOLS_VIEWLOG, OnUpdateToolsViewLogFile)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CLEARFILTER, OnUpdateViewClearfilter)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CUSTOMTOOLBAR, OnUpdateViewCustomToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CYCLETASKVIEWS, OnUpdateViewCycleTaskViews)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DECREMENTTASKVIEWFONTSIZE, OnUpdateViewDecrementTaskViewFontSize)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FILTER, OnUpdateViewFilter)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HIDEALLBARS, OnUpdateViewHideAllBars)
	ON_UPDATE_COMMAND_UI(ID_VIEW_INCREMENTTASKVIEWFONTSIZE, OnUpdateViewIncrementTaskViewFontSize)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAINTOOLBAR, OnUpdateViewMainToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOVETASKLISTLEFT, OnUpdateViewMovetasklistleft)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOVETASKLISTRIGHT, OnUpdateViewMovetasklistright)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NEXT, OnUpdateViewNext)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NEXT_SEL, OnUpdateViewNextSel)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PREV, OnUpdateViewPrev)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PREV_SEL, OnUpdateViewPrevSel)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROJECTNAME, OnUpdateViewProjectname)
	ON_UPDATE_COMMAND_UI(ID_VIEW_REFRESHFILTER, OnUpdateViewRefreshfilter)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESIZECOLSTOFIT, OnUpdateViewResizeColsToFit)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESTOREDEFAULTTASKVIEWFONTSIZE, OnUpdateViewRestoreDefaultTaskViewFontSize)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SAVETOIMAGE, OnUpdateViewSaveToImage)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWALLBARS, OnUpdateViewShowAllBars)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWFILTERBAR, OnUpdateViewShowfilterbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWREMINDERS, OnUpdateViewShowRemindersWindow)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWTASKLISTTABBAR, OnUpdateViewShowTasklistTabbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWTREELISTTABBAR, OnUpdateViewShowTreeListTabbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SORTTASKLISTTABS, OnUpdateViewSorttasklisttabs)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS_BAR, OnUpdateViewStatusBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOGGLEALLTASKEXPANDED, OnUpdateViewExpandTasks)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOGGLEFILTER, OnUpdateViewTogglefilter)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOGGLETASKEXPANDED, OnUpdateViewExpandTasks)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOGGLETASKSANDCOMMENTS, OnUpdateViewToggletasksandcomments)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOGGLETREEANDLIST, OnUpdateViewToggleTreeandList)
	ON_UPDATE_COMMAND_UI(ID_WINDOW1, OnUpdateWindow)

	ON_UPDATE_COMMAND_UI_RANGE(ID_EDIT_SETPRIORITYNONE, ID_EDIT_SETPRIORITY10, OnUpdateSetPriority)
	ON_UPDATE_COMMAND_UI_RANGE(ID_NEWTASK, ID_NEWSUBTASK, OnUpdateNewTask)
	ON_UPDATE_COMMAND_UI_RANGE(ID_NEWTASK_ATTOP, ID_NEWSUBTASK_ATBOTTOM, OnUpdateNewTask)
	ON_UPDATE_COMMAND_UI_RANGE(ID_NEWTASK_ATTOPSELECTED, ID_NEWTASK_ATBOTTOMSELECTED, OnUpdateNewTask)
	ON_UPDATE_COMMAND_UI_RANGE(ID_NEWTASK_DEPENDENTAFTERSELECTEDTASK, ID_NEWTASK_DEPENDENTBEFORESELECTEDTASK, OnUpdateNewTask)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OFFSETDATES_BACKWARDSBY_ONEDAY, ID_OFFSETDATES_BACKWARDSBY_ONEYEAR, OnUpdateEditOffsetDatesBackwards)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OFFSETDATES_FORWARDSBY_ONEDAY, ID_OFFSETDATES_FORWARDSBY_ONEYEAR, OnUpdateEditOffsetDatesForwards)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SHOWVIEW_TASKTREE, ID_SHOWVIEW_UIEXTENSION16, OnUpdateShowTaskView)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTBY_ALLCOLUMNS_FIRST, ID_SORTBY_ALLCOLUMNS_LAST, OnUpdateSortBy)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SPLITTASKINTO_TWO, ID_SPLITTASKINTO_FIVE, OnUpdateSplitTask)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TOOLS_USERTOOL1, ID_TOOLS_USERTOOL50, OnUpdateUserTool)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_EXPANDDUE, ID_VIEW_COLLAPSESTARTED, OnUpdateViewExpandTasks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_EXPANDTASK, ID_VIEW_COLLAPSEALL, OnUpdateViewExpandTasks)

	ON_WM_ACTIVATE()
	ON_WM_ACTIVATEAPP()
	ON_WM_CAPTURECHANGED()
	ON_WM_CONTEXTMENU()
	ON_WM_COPYDATA()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_DRAWITEM()
	ON_WM_ENABLE()
	ON_WM_ENDSESSION()
	ON_WM_ERASEBKGND()
	ON_WM_HELPINFO()
	ON_WM_INITMENUPOPUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MEASUREITEM()
	ON_WM_MOUSEMOVE()
	ON_WM_MOVE()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_QUERYENDSESSION()
	ON_WM_QUERYOPEN()
	ON_WM_SETCURSOR()
	ON_WM_SETTINGCHANGE()
	ON_WM_SIZE()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SYSCOMMAND()
	ON_WM_TIMER()
	ON_WM_WINDOWPOSCHANGING()

#ifdef _DEBUG
	ON_COMMAND(ID_DEBUG_ENDSESSION, OnDebugEndSession)
	ON_COMMAND(ID_DEBUG_SHOWSETUPDLG, OnDebugShowSetupDlg)
	ON_COMMAND(ID_DEBUG_SHOWREMINDERDLG, OnDebugShowReminderDlg)
	ON_COMMAND(ID_DEBUG_UPDATETRANSLATION, OnDebugUpdateTranslation)
	ON_COMMAND(ID_DEBUG_CLEANDICTIONARIES, OnDebugCleanDictionaries)
	ON_COMMAND(ID_DEBUG_RESTARTAPP, OnDebugRestartApp)
	ON_COMMAND(ID_DEBUG_RESTARTAPPFROMEXPLORER, OnDebugRestartAppFromExplorer)
	ON_UPDATE_COMMAND_UI(ID_DEBUG_RESTARTAPPFROMEXPLORER, OnUpdateDebugRestartAppFromExplorer)
#endif

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToDoListWnd message handlers

void CToDoListWnd::SetupUIStrings()
{
	// set up UI strings for helper classes
	CEnString sSeconds(IDS_SECS_ABBREV); 

	if (!sSeconds.IsEmpty())
		CTimeHelper::SetUnits(THU_SECONDS, sSeconds[0]);

	CTimeEdit::SetUnits(THU_MINS,		CEnString(IDS_TE_MINS),		CEnString(IDS_MINS_ABBREV));
	CTimeEdit::SetUnits(THU_HOURS,		CEnString(IDS_TE_HOURS),	CEnString(IDS_HOURS_ABBREV));
	CTimeEdit::SetUnits(THU_WEEKDAYS,	CEnString(IDS_TE_WEEKDAYS), CEnString(IDS_WEEKDAYS_ABBREV));
	CTimeEdit::SetUnits(THU_DAYS,		CEnString(IDS_TE_DAYS),		CEnString(IDS_DAYS_ABBREV));
	CTimeEdit::SetUnits(THU_WEEKS,		CEnString(IDS_TE_WEEKS),	CEnString(IDS_WEEKS_ABBREV));
	CTimeEdit::SetUnits(THU_MONTHS,		CEnString(IDS_TE_MONTHS),	CEnString(IDS_MONTHS_ABBREV));
	CTimeEdit::SetUnits(THU_YEARS,		CEnString(IDS_TE_YEARS),	CEnString(IDS_YEARS_ABBREV));
	CTimeEdit::SetDefaultButtonTip(		CEnString(IDS_TIMEUNITS));

	CFileEdit::SetDefaultButtonTips(CEnString(IDS_BROWSE), CEnString(IDS_VIEW));
	CFileEdit::SetDefaultBrowseTitles(CEnString(IDS_BROWSEFILE_TITLE), CEnString(IDS_BROWSEFOLDER_TITLE));

	CSoundEdit::SetDefaultFilter(CEnString(IDS_SOUNDFILEFILTER));
	CSoundEdit::SetDefaultPlayButtonTip(CEnString(IDS_PLAYSOUNDBTNTIP));
	
	CTDLRecurringTaskEdit::SetDefaultButtonTip(CEnString(IDS_OPTIONS));
	CXmlFileEx::SetUIStrings(CEnString(IDS_ENCRYPTEDFILE), CEnString(IDS_DECRYPTFAILED));
	CSpellCheckDlg::SetItemText(DLG_SCD_BROWSETITLE, IDS_SCD_BROWSETITLE);
	CWinHelpButton::SetDefaultTooltip(CEnString(IDS_ONLINEHELP));
	CPasswordDialog::SetItemText(PD_TITLE, IDS_PD_TITLE);
	CServerDlg::SetItemText(SD_TITLE, IDS_SD_TITLE);
}

BOOL CToDoListWnd::OnHelpInfo(HELPINFO* /*pHelpInfo*/)
{
	// always eat this because we handle the F1 ourselves
	return FALSE;
}

BOOL CToDoListWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (FileMisc::IsLoggingEnabled() && m_bLogCommands)
	{
		CString sMenuItem = m_menubar.GetFullItemPath(wParam, _T(" > "));

		if (sMenuItem.IsEmpty())
			sMenuItem = Misc::Format(_T("%ld"), wParam);
		else
			while(CAcceleratorString::RemoveAccelerator(sMenuItem));

		FileMisc::LogText(_T("CToDoListWnd::OnCommand(%s)"), sMenuItem);
	}
	
	return CFrameWnd::OnCommand(wParam, lParam);
}


void CToDoListWnd::InitUITheme()
{
	// XP and above only ie. Not Linux
	if (COSVersion() < OSV_XP)
		return;

	if (!m_pPrefs)
	{
		ASSERT(0);
		return;
	}

	CString sThemeFile = m_pPrefs->GetUIThemeFile();

	if (!CThemed::IsAppThemed() || !m_theme.LoadThemeFile(sThemeFile)) 
		m_theme.Reset();

	m_pPrefs->SetUITheme(m_theme);
}
	
void CToDoListWnd::UpdateUITheme()
{
	// XP and above only ie. Not Linux
	if (COSVersion() < OSV_XP)
		return;

	// cache existing theme and update
	CUIThemeFile themeCur = m_theme;
	InitUITheme();

	// update the UI if the theme has changed
	if (themeCur != m_theme)
	{
		// We only need to recreate the toolbar if we are in high-DPI mode
		// so that the images get rescaled with the new background colour
		// Likewise for the menu icons.
		if (GraphicsMisc::WantDPIScaling())
		{
			m_cbQuickFind.DestroyWindow();
			m_tbHelperMain.Release();
			m_toolbarMain.DestroyWindow();

			InitMainToolbar();

			// Only reload if already initialised
			if (m_toolbarCustom.GetSafeHwnd())
			{
				m_toolbarCustom.DestroyWindow();
				InitCustomToolbar();
			}

			// Repopulate the menu icon manager
			m_mgrMenuIcons.ClearImages();
		}
		else
		{
			UpdateToolbarColors(m_toolbarMain, m_theme);
			UpdateToolbarColors(m_toolbarCustom, m_theme);
		}

		// Rest of UI
		m_statusBar.SetUIColors(m_theme.crStatusBarLight, 
								m_theme.crStatusBarDark, 
								m_theme.crStatusBarText, 
								m_theme.HasGradient(), 
								m_theme.HasGlass());

		m_filterBar.SetUITheme(m_theme);
		m_dlgTimeTracker.SetUITheme(m_theme);
		m_tabCtrl.SetBackgroundColor(m_theme.crAppBackDark);

		for (int nCtl = 0; nCtl < GetTDCCount(); nCtl++)
		{
			CFilteredToDoCtrl& tdc = GetToDoCtrl(nCtl);
			tdc.SetUITheme(m_theme);
		}

		if (m_dlgFindTasks.GetSafeHwnd())
			m_dlgFindTasks.SetUITheme(m_theme);

		m_menubar.SetUITheme(m_theme);

		Invalidate();
	}
}

void CToDoListWnd::UpdateToolbarColors(CEnToolBar& toolbar, const CUIThemeFile& theme)
{
	if (CThemed::IsAppThemed() && toolbar.GetSafeHwnd())
	{
		toolbar.SetBackgroundColors(theme.crToolbarLight,
									theme.crToolbarDark,
									theme.HasGradient(),
									theme.HasGlass());

		toolbar.SetHotColor(theme.crToolbarHot);
	}
}

BOOL CToDoListWnd::Create(const CTDCStartupOptions& startup)
{
	m_startupOptions = startup;

	m_bVisible = startup.HasFlag(TLD_FORCEVISIBLE) ? 1 : -1;
	m_bSaveUIVisInTaskList = startup.HasFlag(TLD_SAVEUIVISINTASKLIST);
	m_bAllowForcedCheckOut = startup.HasFlag(TLD_ALLOWFORCEDCHECKOUT);
	m_bPasswordPrompting = startup.HasFlag(TLD_PASSWORDPROMPTING);
	m_bMasterPasswordEnabled = startup.HasFlag(TLD_MASTERPASSWORDENABLED);

	// Sanity check - Master passwords only work with password prompting disabled
	ASSERT(!m_bMasterPasswordEnabled || !m_bPasswordPrompting);

	if (startup.HasFlag(TLD_LOGGING) && EnableLogging())
		m_bLogCommands = startup.HasFlag(TLD_LOGCOMMANDS);

	return CFrameWnd::LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, NULL, NULL);
}

BOOL CToDoListWnd::EnableLogging(BOOL bEnable)
{
	// Quick exit if state hasn't changed
	if (!Misc::StateChanged(bEnable, FileMisc::IsLoggingEnabled()))
		return TRUE;

	BOOL bRes = FileMisc::EnableLogging(bEnable, _T("Abstractspoon"));

	if (bRes && bEnable)
		FileMisc::LogAppModuleState(FBM_SORTBY_FILENAME);

	return bRes;
}

int CToDoListWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOSIZE | SWP_NOZORDER | SWP_NOMOVE);

	// Various key UI elements
	if (!InitMenubar())
		return -1;

	if (!m_dropTarget.Register(this, this))
		return -1;

	if (!InitTrayIcon())
		return -1;
	
	if (!InitMainToolbar())
		return -1;

	// Custom toolbar is delay loaded to avoid memory leaks
	
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
	m_wndSessionStatus.Initialize(*this);

	// add a barebones tasklist while we're still hidden
	if (!CreateNewTaskList(FALSE, FALSE))
		return -1;

	// notify users about dodgy content plugins
	if (m_mgrContent.SomePluginsHaveBadversions())
	{
		if (CMessageBox::AfxShow(IDS_BADPLUGINTITLE, IDS_BADPLUGINVERSIONS, MB_OKCANCEL) == IDCANCEL)
			return -1;
	}

	// late initialization
	PostMessage(WM_POSTONCREATE);
	
	return 0; // success
}

void CToDoListWnd::OnDestroy()
{
	m_wndSessionStatus.DestroyWindow();

	CFrameWnd::OnDestroy();
}

BOOL CToDoListWnd::InitTabCtrl()
{
	UINT nFlags = (WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | TCS_HOTTRACK | TCS_TABS | 
					TCS_SINGLELINE | TCS_RIGHTJUSTIFY | TCS_TOOLTIPS);

	if (Prefs().GetStackTabbarItems())
		nFlags |= TCS_MULTILINE;

	if (!m_tabCtrl.Create(nFlags, CRect(0, 0, 10, 10), this, IDC_TABCONTROL))
		return FALSE;

	m_tabCtrl.SetBackgroundColor(m_theme.crAppBackDark);
	m_tabCtrl.GetToolTips()->ModifyStyle(0, (TTS_ALWAYSTIP | TTS_NOPREFIX));

	CLocalizer::EnableTranslation(m_tabCtrl, FALSE);
		
	// Delay image list creation to avoid resource leaks
	return TRUE;
}

void CToDoListWnd::InitUIFont()
{
	GraphicsMisc::VerifyDeleteObject(m_fontMain);

	HFONT hFontUI = GraphicsMisc::CreateFont(_T("Tahoma"), 8);

	if (m_fontMain.Attach(hFontUI))
		CDialogHelper::SetFont(this, m_fontMain); // will update all child controls
	else
		GraphicsMisc::VerifyDeleteObject(hFontUI);
}

void CToDoListWnd::InitShortcutManager()
{
	// setup defaults first
	m_mgrShortcuts.AddShortcut(ID_CLOSE,							VK_F4,			HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_DELETETASK,						VK_DELETE,		HOTKEYF_EXT);
	m_mgrShortcuts.AddShortcut(ID_EDIT_CLOCK_TASK,					'T',			HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_EDIT_COPY,						'C',			HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_EDIT_CUT,							'X',			HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_EDIT_DECTASKPERCENTDONE,			VK_SUBTRACT,	HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_EDIT_FINDTASKS,					'F',			HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_EDIT_FINDREPLACEINTASKTITLES,		'H',			HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_EDIT_FINDREPLACEINTASKCOMMENTS,	'H',			HOTKEYF_CONTROL | HOTKEYF_SHIFT);
	m_mgrShortcuts.AddShortcut(ID_EDIT_INCTASKPERCENTDONE,			VK_ADD,			HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_EDIT_INSERTDATETIME,				'D',			HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_EDIT_LOCKTASK,					'K',			HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_EDIT_OPENFILELINK1,				'G',			HOTKEYF_CONTROL | HOTKEYF_SHIFT);
	m_mgrShortcuts.AddShortcut(ID_EDIT_PASTEAFTER,					'V',			HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_EDIT_PASTESUB,					'V',			HOTKEYF_CONTROL | HOTKEYF_SHIFT);
	m_mgrShortcuts.AddShortcut(ID_EDIT_QUICKFIND,					'Q',			HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_EDIT_QUICKFINDNEXT,				VK_F3,			0);
	m_mgrShortcuts.AddShortcut(ID_EDIT_QUICKFINDPREV,				VK_F3,			HOTKEYF_SHIFT);
	m_mgrShortcuts.AddShortcut(ID_EDIT_REDO,						'Y',			HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_EDIT_SELECTALL,					'A',			HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_EDIT_TOGGLETASKDONE,				VK_SPACE,		HOTKEYF_CONTROL | HOTKEYF_SHIFT);
	m_mgrShortcuts.AddShortcut(ID_EDIT_TASKTEXT,					VK_F2,			0);
	m_mgrShortcuts.AddShortcut(ID_EDIT_UNDO,						'Z',			HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_EXIT,								VK_F4,			HOTKEYF_ALT);
	m_mgrShortcuts.AddShortcut(ID_HELP_WIKI,						VK_F1,			0);
	m_mgrShortcuts.AddShortcut(ID_LOAD_NORMAL,						'O',			HOTKEYF_CONTROL); 
	m_mgrShortcuts.AddShortcut(ID_MAXCOMMENTS,						'M',			HOTKEYF_CONTROL | HOTKEYF_SHIFT);
	m_mgrShortcuts.AddShortcut(ID_MAXTASKLIST,						'M',			HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_MOVE_GOTOTASK,					'G',			HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_MOVETASKDOWN,						VK_DOWN,		HOTKEYF_CONTROL | HOTKEYF_EXT);
	m_mgrShortcuts.AddShortcut(ID_MOVETASKLEFT,						VK_LEFT,		HOTKEYF_CONTROL | HOTKEYF_EXT);
	m_mgrShortcuts.AddShortcut(ID_MOVETASKRIGHT,					VK_RIGHT,		HOTKEYF_CONTROL | HOTKEYF_EXT);
	m_mgrShortcuts.AddShortcut(ID_MOVETASKUP,						VK_UP,			HOTKEYF_CONTROL | HOTKEYF_EXT);
	m_mgrShortcuts.AddShortcut(ID_NEWSUBTASK_ATBOTTOM,				'N',			HOTKEYF_CONTROL | HOTKEYF_SHIFT);
	m_mgrShortcuts.AddShortcut(ID_NEWTASK_AFTERSELECTEDTASK,		'N',			HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_NEWTASK_BEFORESELECTEDTASK,		'N',			HOTKEYF_CONTROL | HOTKEYF_ALT);
	m_mgrShortcuts.AddShortcut(ID_NEXTTASK,							VK_F8,			0);
	m_mgrShortcuts.AddShortcut(ID_OPEN_RELOAD,						'R',			HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_PREVTASK,							VK_F7, 0);
	m_mgrShortcuts.AddShortcut(ID_PRINT,							'P',			HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_SAVE_NORMAL,						'S',			HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_TOOLS_EXPORT,						'E',			HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_TOOLS_IMPORT,						'I',			HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_TOOLS_TRANSFORM,					'T',			HOTKEYF_CONTROL | HOTKEYF_SHIFT);
	m_mgrShortcuts.AddShortcut(ID_VIEW_CYCLETASKVIEWS,				VK_F10,			HOTKEYF_SHIFT);
	m_mgrShortcuts.AddShortcut(ID_VIEW_DECREMENTTASKVIEWFONTSIZE,	VK_DOWN,		HOTKEYF_SHIFT | HOTKEYF_ALT | HOTKEYF_EXT);
	m_mgrShortcuts.AddShortcut(ID_VIEW_INCREMENTTASKVIEWFONTSIZE,	VK_UP,			HOTKEYF_SHIFT | HOTKEYF_ALT | HOTKEYF_EXT);
	m_mgrShortcuts.AddShortcut(ID_VIEW_NEXT,						VK_TAB,			HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_VIEW_NEXT_SEL,					VK_RIGHT,		HOTKEYF_ALT | HOTKEYF_EXT);
	m_mgrShortcuts.AddShortcut(ID_VIEW_PREV,						VK_TAB,			HOTKEYF_CONTROL | HOTKEYF_SHIFT);
	m_mgrShortcuts.AddShortcut(ID_VIEW_PREV_SEL,					VK_LEFT,		HOTKEYF_ALT | HOTKEYF_EXT);
	m_mgrShortcuts.AddShortcut(ID_VIEW_REFRESHFILTER,				VK_F5,			HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_VIEW_RESIZECOLSTOFIT,				VK_ADD,			HOTKEYF_CONTROL);
	m_mgrShortcuts.AddShortcut(ID_VIEW_TOGGLEFILTER,				VK_F12,			0);
	m_mgrShortcuts.AddShortcut(ID_VIEW_TOGGLETASKEXPANDED,			VK_SPACE,		HOTKEYF_CONTROL | HOTKEYF_ALT);
	m_mgrShortcuts.AddShortcut(ID_VIEW_TOGGLEALLTASKEXPANDED,		VK_SPACE,		HOTKEYF_SHIFT | HOTKEYF_ALT);
	m_mgrShortcuts.AddShortcut(ID_VIEW_TOGGLETASKSANDCOMMENTS,		VK_F11,			0);
	m_mgrShortcuts.AddShortcut(ID_VIEW_TOGGLETREEANDLIST,			VK_F10,			0);
	m_mgrShortcuts.AddShortcut(ID_WINDOW1,							'1',			HOTKEYF_ALT);
	m_mgrShortcuts.AddShortcut(ID_WINDOW2,							'2',			HOTKEYF_ALT);
	m_mgrShortcuts.AddShortcut(ID_WINDOW3,							'3',			HOTKEYF_ALT);
	m_mgrShortcuts.AddShortcut(ID_WINDOW4,							'4',			HOTKEYF_ALT);
	m_mgrShortcuts.AddShortcut(ID_WINDOW5,							'5',			HOTKEYF_ALT);
	m_mgrShortcuts.AddShortcut(ID_WINDOW6,							'6',			HOTKEYF_ALT);
	m_mgrShortcuts.AddShortcut(ID_WINDOW7,							'7',			HOTKEYF_ALT);
	m_mgrShortcuts.AddShortcut(ID_WINDOW8,							'8',			HOTKEYF_ALT);
	m_mgrShortcuts.AddShortcut(ID_WINDOW9,							'9',			HOTKEYF_ALT);
	
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

	m_dlgTimeTracker.SetStartStopShortcut(m_mgrShortcuts.GetShortcut(ID_EDIT_CLOCK_TASK));
}

void CToDoListWnd::PopulateMenuIconManager()
{
	if (!m_mgrMenuIcons.IsInitialized())
	{
		ASSERT(0);
		return;
	}
	else if (m_mgrMenuIcons.HasImages())
	{
		return;
	}

	// Toolbar images
	CUIntArray aCmdIDs;
	
	// Main toolbar
	aCmdIDs.Add(ID_LOAD_NORMAL);
	aCmdIDs.Add(ID_SAVE_NORMAL);
	aCmdIDs.Add(ID_SAVEALL);
	
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

	m_mgrMenuIcons.AddImages(aCmdIDs, IDB_APP_TOOLBAR_STD, colorMagenta);

	// extra
	aCmdIDs.RemoveAll();

  	aCmdIDs.Add(ID_HELP_DONATE);
	aCmdIDs.Add(ID_TOOLS_SELECTINEXPLORER);
	aCmdIDs.Add(ID_TOOLS_ADDTOSOURCECONTROL);
	aCmdIDs.Add(ID_TOOLS_REMOVEFROMSOURCECONTROL);
	aCmdIDs.Add(ID_VIEW_INCREMENTTASKVIEWFONTSIZE);
	aCmdIDs.Add(ID_VIEW_DECREMENTTASKVIEWFONTSIZE);
	aCmdIDs.Add(ID_EXIT);
	aCmdIDs.Add(ID_FILE_ENCRYPT);

	m_mgrMenuIcons.AddImages(aCmdIDs, IDB_APP_EXTRA_STD, colorMagenta);

	// social images
	aCmdIDs.RemoveAll();
	
	aCmdIDs.Add(ID_HELP_WIKI);
	aCmdIDs.Add(ID_HELP_FORUM);

	m_mgrMenuIcons.AddImages(aCmdIDs, IDB_SOCIAL_TOOLBAR, colorMagenta);

	// Tray icon
	m_mgrMenuIcons.AddImage(ID_TRAYICON_CREATETASK, GetNewTaskCmdID());
	m_mgrMenuIcons.AddImage(ID_TRAYICON_CLOSE, ID_EXIT);

	// Custom toolbar
	if (m_toolbarCustom.GetSafeHwnd())
		m_mgrMenuIcons.AddImages(m_toolbarCustom);

	UpdateMenuIconMgrSourceControlStatus();
}

void CToDoListWnd::OnShowKeyboardshortcuts() 
{
	CTDCMainMenu menu;

	if (menu.LoadMenu())
	{
		CStringArray aMapping;

		if (m_mgrShortcuts.BuildMapping(menu, aMapping, '|'))
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
}

void CToDoListWnd::SaveCurrentFocus(HWND hwndFocus)
{
	if (!IsWindowEnabled())
		return;

	if (IsIconic())
		return;

	if (!hwndFocus)
		hwndFocus = ::GetFocus();

	if (CDialogHelper::IsChildOrSame(m_dlgReminders, hwndFocus))
		return;

	if (CDialogHelper::IsChildOrSame(m_dlgTimeTracker, hwndFocus))
		return;

	if (hwndFocus != *this)
		m_hwndLastFocus = hwndFocus;
}

void CToDoListWnd::PostAppRestoreFocus(BOOL bActivate)
{
	HWND hwndFocus = m_hwndLastFocus;

	if (bActivate && GetTDCCount() && (!hwndFocus || Prefs().GetAutoFocusTasklist()))
		hwndFocus = GetToDoCtrl();
	
	if (!hwndFocus || (hwndFocus == *this))
		return;
	
	PostMessage(WM_APPRESTOREFOCUS, 0L, (LPARAM)hwndFocus);
}

LRESULT CToDoListWnd::OnFocusChange(WPARAM wp, LPARAM /*lp*/)
{
	if (IsWindowEnabled() && GetTDCCount() && wp)
	{
		// Store latest 'good' focus for restoration
		// whenever we reactivate the application
		SaveCurrentFocus((HWND)wp);

		if (m_statusBar.GetSafeHwnd())
		{
			m_sCurrentFocus.Empty();

			CWnd* pFocus = CWnd::FromHandle((HWND)wp);

// #ifdef _DEBUG
// 			if (pFocus)
// 			{
// 				CString sFocus;
// 				pFocus->GetWindowText(sFocus);
// 				TRACE(_T("OnFocusChange(%s = %s)\n"), CWinClasses::GetClassEx(*pFocus), sFocus.Left(100));
// 			}
// 			else
// 			{
// 				TRACE(_T("OnFocusChange(NULL)\n"));
// 			}
// #endif
			const CFilteredToDoCtrl& tdc = GetToDoCtrl();
			CEnString sFocus;

			if (CDialogHelper::IsChildOrSame(tdc.GetSafeHwnd(), (HWND)wp))
			{
				sFocus.LoadString(IDS_FOCUS_TASKS);
				sFocus += ": ";
				sFocus += tdc.GetControlDescription(pFocus);
			}
			else if (m_cbQuickFind.GetSafeHwnd() && m_cbQuickFind.IsChild(pFocus))
			{
				sFocus.LoadString(IDS_QUICKFIND);
			}
			else if (m_dlgFindTasks.GetSafeHwnd() && m_dlgFindTasks.IsChild(pFocus))
			{
				sFocus.LoadString(IDS_FINDTASKS);
			}
			else if (m_filterBar.GetSafeHwnd() && m_filterBar.IsChild(pFocus))
			{
				sFocus.LoadString(IDS_FOCUS_FILTERBAR);
			}
			else if (m_dlgTimeTracker.GetSafeHwnd() && m_dlgTimeTracker.IsChild(pFocus))
			{
				sFocus.LoadString(IDS_FOCUS_TIMETRACKER);
			}
			else if (m_dlgReminders.GetSafeHwnd() && m_dlgReminders.IsChild(pFocus))
			{
				sFocus.LoadString(IDS_FOCUS_REMINDERS);
			}		

			// limit length of string
			if (sFocus.GetLength() > 22)
				m_sCurrentFocus = sFocus.Left(20) + _T("...");
			else
				m_sCurrentFocus = sFocus;

			m_statusBar.UpdateFocusedControl(m_sCurrentFocus);
		
			// if the status bar is hidden then add text to title bar
			if (!m_bShowStatusBar)
				UpdateCaption();
		}
	}

	return 0L;
}

LRESULT CToDoListWnd::OnGetIcon(WPARAM bLargeIcon, LPARAM /*not used*/)
{
	return (LRESULT)(bLargeIcon ? m_icons.GetBigIcon() : m_icons.GetSmallIcon());
}

BOOL CToDoListWnd::InitStatusbar()
{
	if (!m_statusBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM, IDC_FILENAME))
		return FALSE;

	m_statusBar.SetUITheme(m_theme);

	return TRUE;
}

BOOL CToDoListWnd::InitFilterbar()
{
	if (!m_filterBar.Create(this))
		return FALSE;

	m_filterBar.EnableMultiSelection(Prefs().GetMultiSelFilters());
	m_filterBar.ShowDefaultFilters(Prefs().GetShowDefaultFiltersInFilterBar());
	m_filterBar.SetTitleFilterOption(Prefs().GetTitleFilterOption());
	m_filterBar.SetUITheme(m_theme);

	RefreshFilterBarAdvancedFilters();

	return TRUE;
}

BOOL CToDoListWnd::InitTimeTrackDlg()
{
	// First time only
	if (m_dlgTimeTracker.GetSafeHwnd())
		return TRUE;

	if (!m_dlgTimeTracker.Create(this))
		return FALSE;

	UpdateTimeTrackerPreferences();
	return TRUE;
}

void CToDoListWnd::UpdateTimeTrackerTasks(BOOL bAllTasks, const CTDCAttributeMap& mapAttrib)
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();

	if (bAllTasks)
		m_dlgTimeTracker.UpdateAllTasks(&tdc);
	else
		m_dlgTimeTracker.UpdateSelectedTasks(&tdc, mapAttrib);
}

void CToDoListWnd::UpdateTimeTrackerPreferences()
{
	const CPreferencesDlg& prefs = Prefs();

	m_dlgTimeTracker.SetOption(TTDO_ALLOWPARENTTRACKING, prefs.GetAllowParentTimeTracking());
	m_dlgTimeTracker.SetOption(TTDO_SHOWPARENTSASFOLDERS, prefs.GetShowParentsAsFolders());
	m_dlgTimeTracker.SetOption(TTDO_FORMATTIMESASHMS, prefs.GetUseHMSTimeFormat());
	m_dlgTimeTracker.SetOption(TTDO_SHOWONBEGINTRACKING, prefs.GetShowTimeTracker());

	m_dlgTimeTracker.SetUITheme(m_theme);
	m_dlgTimeTracker.SetStartStopShortcut(m_mgrShortcuts.GetShortcut(ID_EDIT_CLOCK_TASK));
}

BOOL CToDoListWnd::InitTrayIcon()
{
	if (!m_trayIcon.Create(this, IDC_TRAYICON, IDR_MAINFRAME_STD, CEnString(IDS_COPYRIGHT)))
		return FALSE;

	UpdateTrayIcon();

	// we always create the tray icon (for simplicity) but only show it if required
	if (Prefs().GetUseSysTray())
		m_trayIcon.ShowTrayIcon();

	// show the upgrade message as required
	if (m_startupOptions.HasFlag(TLD_UPGRADED))
		m_trayIcon.ShowBalloon(CEnString(IDS_SUCCESSFUL_UPGRADE), GetTitle(TRUE), NIIF_INFO, 10);

	return TRUE;
}

BOOL CToDoListWnd::InitMainToolbar()
{
	if (m_toolbarMain.GetSafeHwnd())
		return TRUE;

	UINT nStyle = (WS_CHILD | CBRS_ALIGN_TOP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CBRS_TOOLTIPS);

	if (m_bShowingMainToolbar)
		nStyle |= WS_VISIBLE;

	if (!m_toolbarMain.CreateEx(this, (TBSTYLE_FLAT | TBSTYLE_WRAPABLE), nStyle))
		return FALSE;

	if (!m_toolbarMain.LoadToolBar(IDR_APP_TOOLBAR))
		return FALSE;
	
	m_toolbarMain.SetBorders(4, 2, 0, 0);

	// initialise colors before setting image because the
	// background colour may be needed for image scaling
	UpdateToolbarColors(m_toolbarMain, m_theme);
	
	m_toolbarMain.SetImage(IDB_APP_TOOLBAR_STD, colorMagenta);
	
	// resize the toolbar in one row so that our subsequent calculations work
	m_toolbarMain.Resize(1000, CPoint(0, 2)); 
	
	// insert combobox for quick Find after Find Tasks button
	int nPos = m_toolbarMain.CommandToIndex(ID_EDIT_FINDTASKS) + 1;
	ASSERT(m_toolbarMain.IsItemSeparator(nPos));

	// A separator at the end is wider than one in the middle (??)
	m_nToolbarEndSepWidth = (m_toolbarMain.GetItemWidth(nPos) + 2);
	
	m_toolbarMain.InsertSeparator(nPos);
	m_toolbarMain.InsertSeparator(nPos + 1);
	
	CRect rect;
	m_toolbarMain.SetItemWidth(nPos + 1, GraphicsMisc::ScaleByDPIFactor(150), rect);
	
	rect.top += QUICKFIND_VOFFSET;
	rect.bottom += QUICKFIND_HEIGHT;
	
	// Note: not sorted
	if (!m_cbQuickFind.Create(WS_CHILD | WS_VSCROLL | WS_VISIBLE | CBS_AUTOHSCROLL | 
		CBS_DROPDOWN | CBS_OWNERDRAWFIXED, rect, &m_toolbarMain, IDC_QUICKFIND))
	{
		return FALSE;
	}
	m_cbQuickFind.SetFont(&m_fontMain);

	// previous quick find items
	CStringArray aItems;
	
	if (CPreferences().GetProfileArray(_T("QuickFind"), aItems))
		m_cbQuickFind.AddUniqueItems(aItems);

	m_mgrPrompts.SetComboEditPrompt(m_cbQuickFind, IDS_QUICKFIND);
	
	m_tbHelperMain.Initialize(&m_toolbarMain, this, &m_mgrShortcuts);
	
	return TRUE;
}

BOOL CToDoListWnd::InitCustomToolbar()
{
	if (!m_bShowingCustomToolbar || m_toolbarCustom.GetSafeHwnd())
		return TRUE;

	CToolbarButtonArray aTBButtons;

	if (!Prefs().GetCustomToolbarButtons(aTBButtons))
		return TRUE; // not an error

	UINT nStyle = (WS_CHILD | CBRS_ALIGN_TOP | WS_CLIPCHILDREN | WS_VISIBLE);

	if (!m_toolbarCustom.CreateEx(this, (TBSTYLE_FLAT | TBSTYLE_WRAPABLE), nStyle))
		return FALSE;

	m_toolbarCustom.SetBorders(4, 2, 0, 0);

	// initialise colors before initialising buttons because the
	// background colour may be needed for image scaling
	UpdateToolbarColors(m_toolbarCustom, m_theme);

	if (!m_toolbarCustom.InitialiseButtons(aTBButtons, m_menubar, m_mgrShortcuts))
	{
		ASSERT(0);
		m_toolbarCustom.DestroyWindow();
		return FALSE;
	}
	
	return TRUE;
}

void CToDoListWnd::OnUpdateQuickFind(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bShowingMainToolbar);
}

void CToDoListWnd::OnQuickFind() 
{
	if (m_bShowingMainToolbar)
		m_cbQuickFind.SetFocus();
}

void CToDoListWnd::OnQuickFindNext() 
{
	if (!m_sQuickFind.IsEmpty())
	{
		CFilteredToDoCtrl& tdc = GetToDoCtrl();
		
		if (!tdc.SelectNextTask(m_sQuickFind, TDC_SELECTNEXT))
		{
			// return to start
			if (!tdc.SelectNextTask(m_sQuickFind, TDC_SELECTFIRST))
				CMessageBox::AfxShow(CEnString(IDS_QUICKFIND_NOTFOUND, m_sQuickFind));
		}
	}
}

void CToDoListWnd::OnUpdateQuickFindNext(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_sQuickFind.IsEmpty());
}

void CToDoListWnd::OnQuickFindPrev() 
{
	if (!m_sQuickFind.IsEmpty())
	{
		CFilteredToDoCtrl& tdc = GetToDoCtrl();
		
		if (!tdc.SelectNextTask(m_sQuickFind, TDC_SELECTPREV))
		{
			// return to end
			if (!tdc.SelectNextTask(m_sQuickFind, TDC_SELECTLAST))
				CMessageBox::AfxShow(CEnString(IDS_QUICKFIND_NOTFOUND, m_sQuickFind));
		}
	}
}

void CToDoListWnd::OnUpdateQuickFindPrev(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_sQuickFind.IsEmpty());
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
	
	if (!tdc.SelectNextTask(m_sQuickFind, TDC_SELECTNEXTINCLCURRENT))
	{
		if (!tdc.SelectNextTask(m_sQuickFind, TDC_SELECTFIRST))
		{
			if (bComboSelChange)
			{
				CMessageBox::AfxShow(CEnString(IDS_QUICKFIND_NOTFOUND, m_sQuickFind), MB_OK);
				m_cbQuickFind.SetFocus();
			}

			// Remove the just-unfound item
			m_cbQuickFind.DeleteString(m_sQuickFind);
		}
	}
}

BOOL CToDoListWnd::HandleReservedShortcut(DWORD dwShortcut)
{
	if (CFilteredToDoCtrl::IsReservedShortcut(dwShortcut))
	{
		int nRet = CMessageBox::AfxShow(IDS_RESERVEDSHORTCUT_TITLE, IDS_RESERVEDSHORTCUT_MSG, MB_YESNOCANCEL);
		
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
					else if (GetTDCCount() && GetToDoCtrl().PreTranslateMessage(pMsg))
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
					// Add the item
					if (m_cbQuickFind.FindStringExact(0, m_sQuickFind) == CB_ERR)
					{
						int nSel = m_cbQuickFind.AddUniqueItem(m_sQuickFind);
						m_cbQuickFind.SetCurSel(nSel);

						// keep only the last 20 items
						int nItem = m_cbQuickFind.GetCount();

						while (nItem > 20)
						{
							nItem--;
							m_cbQuickFind.DeleteString(nItem);
						}
					}
					else
					{
						OnQuickFindNext();
					}
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
				// and the Find Tasks dialog
				else if (::IsChild(m_dlgFindTasks, pMsg->hwnd))
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
	ASSERT(IsWindowEnabled());

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
	if (IsWindowEnabled())
	{
		// the only way we get a WM_CLOSE here is if it was sent from 
		// an external app so we shut down as gracefully as possible
		if ((pMsg->message == WM_CLOSE) && 
			(pMsg->hwnd == GetSafeHwnd()))
		{
			DoExit();
			return TRUE;
		}

		BOOL bKeyPress = ((pMsg->message == WM_KEYDOWN) || 
						  (pMsg->message == WM_SYSKEYDOWN));
	
		if (bKeyPress && !IsDroppedComboBox(pMsg->hwnd))
		{
			if (HandleEscapeTabReturn(pMsg))
			{
				TRACE(_T("CToDoListWnd::PreTranslateMessage(Keypress -> HandleEscapeTabReturn)\n"));
				return TRUE;
			}

			if (ProcessShortcut(pMsg))
			{
				TRACE(_T("CToDoListWnd::PreTranslateMessage(Keypress -> ProcessShortcut)\n"));
				return TRUE;
			}

			if (ProcessDialogCtrlAccelerator(pMsg))
			{
				TRACE(_T("CToDoListWnd::PreTranslateMessage(Keypress -> ProcessDialogCtrlShortcut)\n"));
				return TRUE;
			}

			//TRACE(_T("CToDoListWnd::PreTranslateMessage(Keypress -> Default handling)\n"));
		}
	}
	
	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CToDoListWnd::OnCancel()
{
	ASSERT (Prefs().GetEscapeMinimizes());

	// if the close button has been configured to Minimize to tray
	// then do that here else normal minimize 
	if (Prefs().HasSysTrayOptions(STO_ONMINCLOSE, STO_ONCLOSE))
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
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	if (!tdc.IsReadOnly() && tdc.HasSelection())
	{
		tdc.DeleteSelectedTask();
	
		UpdateStatusBar();
	}
}

void CToDoListWnd::OnDeleteAllTasks() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	if (!tdc.IsReadOnly() && tdc.DeleteAllTasks())
	{
		UpdateStatusBar();
	}
}

void CToDoListWnd::OnSave() 
{
	///////////////////////////////////////////////////////////////////
	// PERMANENT LOGGING
	CScopedLogTimer log(_T("CToDoListWnd::OnSave()"));
	log.LogStart();
	///////////////////////////////////////////////////////////////////

	SaveTaskList(GetSelToDoCtrl());
}

TDC_FILE CToDoListWnd::DoSaveWithBackupAndProgress(CFilteredToDoCtrl& tdc, int nIndex, CTaskFile& tasks, LPCTSTR szFilePath, BOOL bFlush)
{
	DOPROGRESS(IDS_SAVINGPROGRESS);
		
	// back file up
	m_mgrToDoCtrls.DoBackup(nIndex);
	
	// do the save
	return tdc.Save(tasks, szFilePath, bFlush);
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
			
			if (IDOK == CMessageBox::AfxShow(IDS_SAVETASKLIST_TITLE, sMessage, (MB_OKCANCEL | MB_ICONEXCLAMATION)))
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
		ASSERT(nErr >= TDCF_OTHER);
		sMessage.Format(IDS_UNKNOWNSAVEERROR2, szTasklist, (nErr - (int)TDCF_OTHER));
		break;
	}
	
	if (!sMessage.IsEmpty())
	{
		///////////////////////////////////////////////////////////////////////
		// PERMANENT LOGGING
		FileMisc::LogText(_T("Saving of the file '%s' failed with the error code: %d"), szTasklist, nErr);
		///////////////////////////////////////////////////////////////////////

		CMessageBox::AfxShow(IDS_SAVETASKLIST_TITLE, sMessage, MB_OK | MB_ICONEXCLAMATION);
	}
	
	return FALSE; // not handled
}

TDC_FILE CToDoListWnd::SaveTaskList(int nTDC, LPCTSTR szFilePath, DWORD dwFlags)
{
	CAutoFlag af(m_bSaving, TRUE);
	CPreferences prefs;
	const CPreferencesDlg& userPrefs = Prefs();

	// make sure we are up to date
	CFilteredToDoCtrl& tdc = GetToDoCtrl(nTDC);

	TDC_FILE nResult = TDCF_SUCCESS;
	CTaskFile tasks;

	// we can reduce the amount of work required if saving to storage
	TSM_TASKLISTINFO storageInfo;
	BOOL bUsesStorage = m_mgrToDoCtrls.GetStorageDetails(nTDC, storageInfo);
	BOOL bFlush = !Misc::HasFlag(dwFlags, TDLS_NOFLUSH);

	if (bUsesStorage)
	{
		DOPROGRESS(IDS_SAVINGPROGRESS);

		// init storage local path to temp path
		storageInfo.SetLocalFilePath(FileMisc::GetTempFilePath());

		// save to file and then to storage
		if (tdc.Save(tasks, storageInfo.szLocalFileName, bFlush) == TDCF_SUCCESS)
		{
			if (!m_mgrStorage.StoreTasklist(storageInfo, tasks, -1, prefs))
				nResult = TDCF_OTHER;
		}
	}
	else // we're file-based
	{
		CString sFilePath = (szFilePath ? szFilePath : m_mgrToDoCtrls.GetFilePath(nTDC));

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
					if (!SelectToDoCtrl(nTDC, (nTDC != GetSelToDoCtrl())))
						return TDCF_CANCELLED;

					// use 'friendly' name as user for user
					CFileSaveDialog dialog(IDS_SAVETASKLIST_TITLE, 
											TDL_EXT, 
											m_mgrToDoCtrls.GetFileName(nTDC, FALSE), 
											EOFN_DEFAULTSAVE, 
											TDL_FILEFILTER, 
											this);
					
					dialog.m_ofn.nFilterIndex = 1; // .tdl

					if (dialog.DoModal(prefs) != IDOK)
						return TDCF_CANCELLED; // user elected not to proceed

					// else make sure the file is not readonly
					sFilePath = dialog.GetPathName();

				}

				// do the save
				nResult = DoSaveWithBackupAndProgress(tdc, nTDC, tasks, sFilePath, bFlush);

				if (nResult != TDCF_SUCCESS)
				{
					// error handling if this is not an auto-save
					if (!Misc::HasFlag(dwFlags, TDLS_AUTOSAVE) && HandleSaveTasklistError(nResult, sFilePath))
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
	m_mgrToDoCtrls.RefreshFileLastModified(nTDC);
	m_mgrToDoCtrls.RefreshReadOnlyStatus(nTDC);
	m_mgrToDoCtrls.RefreshPathType(nTDC);
	m_mgrToDoCtrls.SetLoaded(nTDC); // In case we were previously a 'new' tasklist

	CString sFilePath = tdc.GetFilePath();

	if (userPrefs.GetAddFilesToMRU() && !bUsesStorage)
		m_mruList.Add(sFilePath);

	UpdateCaption();

	// auto-export after saving
	TDCEXPORTTASKLIST* pExport = PrepareNewExportAfterSave(nTDC, tasks);

	if (pExport)
		m_wndExport.ExportTasks(pExport);

	return nResult;
}

TDCEXPORTTASKLIST* CToDoListWnd::PrepareNewExportAfterSave(int nTDC, const CTaskFile& tasks)
{
	const CPreferencesDlg& userPrefs = Prefs();
	CString sExt;

	if (!userPrefs.GetSaveExport() || !userPrefs.GetSaveExportExtension(sExt))
		return NULL;

	if (!tasks.GetTaskCount())
	{
		ASSERT(0);
		return NULL;
	}

	DOPROGRESS(IDS_EXPORTPROGRESS);
	
	CFilteredToDoCtrl& tdc = GetToDoCtrl(nTDC);
	CString sTDCPath = tdc.GetFilePath();
	int nExporter = m_mgrImportExport.FindExporterByType(userPrefs.GetSaveExportTypeID());

	TDCEXPORTTASKLIST* pExport = new TDCEXPORTTASKLIST(GetSafeHwnd(), sTDCPath, nExporter);
	ASSERT(pExport);

	pExport->nPurpose = TDCTEP_EXPORTAFTERSAVE;
	pExport->pImpExpMgr = &m_mgrImportExport;
	
	// save intermediate tasklist to file as required
	if (FileMisc::IsLoggingEnabled())
		pExport->sSaveIntermediatePath = GetIntermediateTaskListPath(sTDCPath);

	// construct output path
	CString sExportFolder = userPrefs.GetSaveExportFolderPath();
	CString sDrive, sFolder, sFName;

	FileMisc::SplitPath(sTDCPath, &sDrive, &sFolder, &sFName);

	if (!sExportFolder.IsEmpty() && FileMisc::CreateFolder(sExportFolder))
		FileMisc::MakePath(pExport->sExportPath, NULL, sExportFolder, sFName, sExt);
	else
		FileMisc::MakePath(pExport->sExportPath, sDrive, sFolder, sFName, sExt);

	// The current contents of 'tasks' is 'All Tasks' and 
	// 'All Columns' but NOT 'Html Comments'.
	// So if user either wants 'Filtered Tasks' or 'Html Comments' or
	// only 'Visible Columns' we need to grab the tasks again.
	BOOL bFiltered = (userPrefs.GetSaveExportFilteredOnly() && tdc.HasAnyFilter());

	pExport->sStylesheet = userPrefs.GetSaveExportStylesheet();
	BOOL bTransform = GetStylesheetPath(tdc, pExport->sStylesheet);
	BOOL bHtmlComments = (bTransform || m_mgrImportExport.ExporterSupportsHtmlComments(nExporter));

	if (bFiltered || bHtmlComments || !userPrefs.GetExportAllAttributes())
	{
		TSD_TASKS nWhatTasks = bFiltered ? TSDT_FILTERED : TSDT_ALL;
		TDCGETTASKS filter;

		if (userPrefs.GetExportAllAttributes())
		{
			filter.mapAttribs.Add(TDCA_ALL);
		}
		else // visible attributes
		{
			filter.mapAttribs.Copy(tdc.GetVisibleEditFields());
			filter.mapAttribs.Add(TDCA_COMMENTS); // always
		}

		// set the html image folder to be the output path with
		// an different extension
		CString sHtmlImgFolder = GetHtmlImageFolder(bHtmlComments, pExport->sExportPath);

		GetTasks(tdc, bHtmlComments, sHtmlImgFolder, bTransform, nWhatTasks, filter, pExport->tasks); 
	}
	else
	{
		pExport->tasks.CopyFrom(tasks);
	}
	
	return pExport;
}

void CToDoListWnd::OnLoad() 
{
	ASSERT(IsWindowVisible());

	CPreferences prefs;
	CFileOpenDialog dialog(IDS_OPENTASKLIST_TITLE,
							TDL_EXT,
							NULL,
							EOFN_DEFAULTOPEN | OFN_ALLOWMULTISELECT,
							TDL_FILEFILTER,
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
		sMessage.Format(IDS_TASKLISTNOTFOUND, szTaskList);
		break;
		
	case TDCF_BADNETWORK:
		sMessage.Format(IDS_BADNETWORK, szTaskList);
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
		///////////////////////////////////////////////////////////////////////
		// PERMANENT LOGGING
		FileMisc::LogText(_T("Loading of the file '%s' failed with the error code: %d"), szTaskList, nErr);
		///////////////////////////////////////////////////////////////////////

		CMessageBox::AfxShow(IDS_OPENTASKLIST_TITLE, sMessage, MB_OK | MB_ICONWARNING);
	}
}

void CToDoListWnd::SaveSettings()
{
	CPreferences prefs;

	// pos
	WINDOWPLACEMENT wp = { 0 };
	VERIFY(GetWindowPlacement(&wp));
	
	prefs.WriteProfileInt(_T("Pos"), _T("Left"), wp.rcNormalPosition.left);
	prefs.WriteProfileInt(_T("Pos"), _T("Top"), wp.rcNormalPosition.top);
	prefs.WriteProfileInt(_T("Pos"), _T("Right"), wp.rcNormalPosition.right);
	prefs.WriteProfileInt(_T("Pos"), _T("Bottom"), wp.rcNormalPosition.bottom);
	prefs.WriteProfileInt(_T("Pos"), _T("Hidden"), !m_bVisible);
	prefs.WriteProfileInt(_T("Pos"), _T("Maximized"), IsZoomed());
	
	// version
	prefs.WriteProfileString(_T("Version"), _T("Version"), GetVersion(FALSE));
	
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
	prefs.WriteProfileInt(SETTINGS_KEY, _T("ToolbarOption"), m_bShowingMainToolbar);
	prefs.WriteProfileInt(SETTINGS_KEY, _T("ShowCustomToolbar"), m_bShowingCustomToolbar);
	prefs.WriteProfileInt(SETTINGS_KEY, _T("ShowProjectName"), m_bShowProjectName);
	prefs.WriteProfileInt(SETTINGS_KEY, _T("ShowStatusBar"), m_bShowStatusBar);
	prefs.WriteProfileInt(SETTINGS_KEY, _T("ShowTasklistBar"), m_bShowTasklistBar);
	prefs.WriteProfileInt(SETTINGS_KEY, _T("ShowTreeListBar"), m_bShowTreeListBar);
	prefs.WriteProfileInt(SETTINGS_KEY, _T("FindTasksVisible"), m_bFindShowing && m_dlgFindTasks.GetSafeHwnd() && m_dlgFindTasks.IsWindowVisible());

	prefs.WriteProfileString(SETTINGS_KEY, _T("TaskViewImageExt"), m_sTaskViewImageExt);
	
	if (Prefs().GetAddFilesToMRU())
		m_mruList.WriteList(prefs, TRUE);

	// quick find items
	CStringArray aItems;

	if (CDialogHelper::GetComboBoxItems(m_cbQuickFind, aItems))
		prefs.WriteProfileArray(_T("QuickFind"), aItems);
	else
		prefs.DeleteProfileSection(_T("QuickFind"));

	m_mgrShortcuts.SaveSettings(prefs, _T("KeyboardShortcuts"));
	m_mgrUIExtensions.SavePreferences(prefs, _T("UIExtensions"));
	m_mgrContent.SavePreferences(prefs, _T("ContentControls"));
}

LRESULT CToDoListWnd::OnUpdateUDTsInToolbar(WPARAM /*wp*/, LPARAM /*lp*/)
{
	UpdateUDTsInToolbar(UDT_INIT);

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
		FileMisc::MakeFullPath(sFilePath, FileMisc::GetAppFolder());

		// Don't check for existence
		nType = TDCPP_FILE;
	}
	
	return nType;
}

LRESULT CToDoListWnd::OnPostOnCreate(WPARAM /*wp*/, LPARAM /*lp*/)
{
	const CPreferencesDlg& userPrefs = Prefs();
	
	// late initialization
	CMouseWheelMgr::Initialize();
	CEditShortcutMgr::Initialize();
	CFocusWatcher::Initialize(this);
	CComboListboxPositioner::Initialize();

	CLocalizer::SetMenuPostTranslationCallback(*this);

	InitShortcutManager();
	m_mgrMenuIcons.Initialize(this); // Populated on demand

	// reminders
	m_dlgReminders.Initialize(this);

	// with or without Stickies Support
	CString sStickiesPath;
	BOOL bShowFullTaskPathInSticky = FALSE;
	
	if (userPrefs.GetUseStickies(sStickiesPath, bShowFullTaskPathInSticky))
		VERIFY(m_dlgReminders.UseStickies(TRUE, sStickiesPath, bShowFullTaskPathInSticky, TRUE));
	
	RestoreVisibility();
	
	// initialize Progress first time
	m_statusBar.BeginProgress(CEnString(IDS_STARTUPPROGRESS));

	// open cmdline tasklist
	CPreferences prefs;
	int nTDCCount = prefs.GetProfileInt(SETTINGS_KEY, _T("NumLastFiles"), 0);

	if (!m_startupOptions.HasFilePath() || nTDCCount)
	{
		// if we have a file on the commandline or any previous tasklists
		// set the prompt of the initial tasklist to something appropriate
		//	TODO
	}

	// cache flags for later
	BOOL bStartupEmpty = m_startupOptions.HasFlag(TLD_STARTEMPTY);
	BOOL bLogModules = m_startupOptions.HasFlag(TLD_LOGMODULES);

	// what to (re)load?
	BOOL bReloadTasklists = (!bStartupEmpty && userPrefs.GetReloadTasklists());
	
	// filepath overrides
	if (m_startupOptions.HasFilePath())
	{
		ProcessStartupOptions(m_startupOptions, TRUE);

		// don't reload previous if a tasklist was actually loaded
		if (!m_mgrToDoCtrls.IsPristine())
			bReloadTasklists = FALSE;
	}
	else
	{
		m_sTitlePrefix = m_startupOptions.GetTitlePrefix();
	}

	if (!m_sTitlePrefix.IsEmpty())
		UpdateCaption();
	
	m_startupOptions.Reset(); // always
	
	// load last files
	if (bReloadTasklists)
	{
		CAutoFlag af(m_bReloading, TRUE);

		// get the last active tasklist
		CString sLastActiveFile = prefs.GetProfileString(SETTINGS_KEY, _T("LastActiveFile"));
		CString sOrgLastActiveFile(sLastActiveFile);

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

				if (bCanDelayLoad && !bActiveTDC && !m_dlgReminders.ToDoCtrlHasReminders(sLastFile))
				{
					DelayOpenTaskList(sLastFile);
				}
				else
				{
					TDC_FILE nResult = OpenTaskList(sLastFile, FALSE);

					// if the last active tasklist couldn't be loaded for any reason
					// then delay load it and mark the last active todoctrl as not found
					if (bActiveTDC && (nResult != TDCF_SUCCESS))
					{
						sLastActiveFile.Empty();

						if (bCanDelayLoad)
							DelayOpenTaskList(sLastFile);
					}
				}
			}
		}
		
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
						if (VerifyTaskListOpen(nTDC, FALSE, FALSE))
							sLastActiveFile = tdc.GetFilePath();
					}
				}
			}

			// if nothing suitable found then select the 'pristine' first tasklist
			if (sLastActiveFile.IsEmpty() || !SelectToDoCtrl(sLastActiveFile, FALSE))
				SelectToDoCtrl(0, FALSE);
			else
				CheckRemovePristineTasklist();
		}
	}

	Resize();
	UpdateWindow();

	// if there's only one tasklist open and it's pristine then it's
	// the original one so add a sample task unless 'empty' flag is set
	if (m_mgrToDoCtrls.IsPristine())
	{
		if (!bStartupEmpty)
		{
			CFilteredToDoCtrl& tdc = GetToDoCtrl();
			ASSERT (tdc.GetTaskCount() == 0);

			tdc.CreateNewTask(CEnString(IDS_SAMPLETASK), TDC_INSERTATTOP, FALSE);
			tdc.SetModified(FALSE);
		}
	}
	else // due task notifications
	{
		PostMessage(WM_DOINITIALDUETASKNOTIFY);
	}

	// current focus
	PostMessage(WM_FW_FOCUSCHANGE, (WPARAM)::GetFocus(), 0L);
	
	RefreshTabOrder();
	Invalidate(TRUE);

	// End progress before updating statusbar
	m_statusBar.EndProgress();

	UpdateStatusBar();

	// find tasks dialog
	if (prefs.GetProfileInt(SETTINGS_KEY, _T("FindTasksVisible"), 0))
	{
		// If the find dialog was previously visible then the 
		// app window will already be the right size so we must
		// make sure it's not enlarged further
		ShowFindDialog(FALSE);
		
		if (userPrefs.GetRefreshFindOnLoad())
			m_dlgFindTasks.RefreshSearch();

		// Find dialog will have stolen focus
		GetToDoCtrl().SetFocusToTasks();
	}

	// log the app and its dlls for debugging
	if (bLogModules)
		FileMisc::LogAppModuleState(FBM_SORTBY_FILENAME);

	return 0L;
}

void CToDoListWnd::LoadSettings()
{
	// settings
	CPreferences prefs;

	BOOL bMaxTasklists = prefs.GetProfileInt(SETTINGS_KEY, _T("SimpleMode"), FALSE); // backward compatibility
	m_nMaxState = prefs.GetProfileEnum(SETTINGS_KEY, _T("ViewState"), bMaxTasklists ? TDCMS_MAXTASKLIST : TDCMS_NORMAL);

	m_bShowFilterBar = prefs.GetProfileInt(SETTINGS_KEY, _T("ShowFilterBar"), m_bShowFilterBar);
	m_bShowProjectName = prefs.GetProfileInt(SETTINGS_KEY, _T("ShowProjectName"), m_bShowProjectName);
	
	m_bShowStatusBar = prefs.GetProfileInt(SETTINGS_KEY, _T("ShowStatusBar"), m_bShowStatusBar);
	m_statusBar.ShowWindow(m_bShowStatusBar ? SW_SHOW : SW_HIDE);
	
	m_sTaskViewImageExt = prefs.GetProfileString(SETTINGS_KEY, _T("TaskViewImageExt"), _T("png"));

	// toolbars
	m_bShowingMainToolbar = prefs.GetProfileInt(SETTINGS_KEY, _T("ToolbarOption"), TRUE);
	m_toolbarMain.ShowWindow(m_bShowingMainToolbar ? SW_SHOW : SW_HIDE);
	m_toolbarMain.EnableWindow(m_bShowingMainToolbar);

	m_bShowingCustomToolbar = prefs.GetProfileInt(SETTINGS_KEY, _T("ShowCustomToolbar"), TRUE);

	if (m_toolbarCustom.GetSafeHwnd())
	{
		m_toolbarCustom.ShowWindow(m_bShowingCustomToolbar ? SW_SHOW : SW_HIDE);
		m_toolbarCustom.EnableWindow(m_bShowingCustomToolbar);
	}

	// tabbars
	m_bShowTasklistBar = prefs.GetProfileInt(SETTINGS_KEY, _T("ShowTasklistBar"), TRUE);
	m_bShowTreeListBar = prefs.GetProfileInt(SETTINGS_KEY, _T("ShowTreeListBar"), TRUE);

	// user preferences
	const CPreferencesDlg& userPrefs = Prefs();
	
	if (userPrefs.GetShowTabCloseButtons())
		m_tabCtrl.ModifyFlags(0, TCE_CLOSEBUTTON);
	else
		m_tabCtrl.ModifyFlags(TCE_CLOSEBUTTON, 0);

	// MRU
	if (userPrefs.GetAddFilesToMRU())
		m_mruList.ReadList(prefs);
	
	// note: we do not restore visibility until OnPostOnCreate

	// default attributes
	userPrefs.GetDefaultTaskAttributes(m_tdiDefault);
	
	// hotkey
	UpdateGlobalHotkey();
	UpdateTimeTrackerPreferences();
	
	// Recently modified period
	CFilteredToDoCtrl::SetRecentlyModifiedPeriod(userPrefs.GetRecentlyModifiedPeriod());

	RestoreTimers();
}

void CToDoListWnd::EnableTDLExtension(BOOL bEnable)
{
	if (COSVersion() == OSV_LINUX)
		return;

	ASSERT((COSVersion() < OSV_VISTA) || FileMisc::IsAdminProcess());

	CFileRegister filereg(_T("tdl"), _T("tdl_Tasklist"));
	BOOL bExistingReg = filereg.IsRegisteredApp(_T("tdl"), FileMisc::GetAppFilePath(), TRUE);

	if (bEnable)
	{
		// If we are a pre-release version then don't overwrite
		// an existing registration unless it's also pre-release
		BOOL bPreRelease = CTDCWebUpdateScript::IsPreRelease(FileMisc::GetAppVersion());

		if (bPreRelease && bExistingReg)
		{
			CString sRegPath = filereg.GetRegisteredAppPath(_T("tdl"));

			if (!CTDCWebUpdateScript::IsPreRelease(FileMisc::GetModuleVersion(sRegPath)))
				return;
		}

		VERIFY(filereg.RegisterFileType(_T("Tasklist"), 0));
	}
	else if (bExistingReg)
	{
		VERIFY(filereg.UnRegisterFileType());
	}
}

void CToDoListWnd::EnableTDLProtocol(BOOL bEnable)
{
	if (COSVersion() == OSV_LINUX)
		return;
	
	ASSERT((COSVersion() < OSV_VISTA) || FileMisc::IsAdminProcess());

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
		
		VERIFY(WebMisc::RegisterProtocol(_T("tdl"), _T("ToDoList"), sCommand));
	}
	else
	{
		VERIFY(WebMisc::UnregisterProtocol(_T("tdl")));
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
				if (Prefs().HasSysTrayOptions(STO_ONMINIMIZE, STO_ONMINCLOSE))
				{
					m_bVisible = FALSE;
				}
			}
		}
	}
	
	if (m_bVisible)
	{
		{
			CAutoFlag af(m_bIgnoreResize, bMaximized);
			RestorePosition();
		}
		
		int nCmdShow = (bMaximized ? SW_SHOWMAXIMIZED : (bMinimized ? SW_SHOWMINIMIZED : SW_SHOW));
 		ShowWindow(nCmdShow);

		if (!bMinimized)
		{
			SetForegroundWindow();

			// Startup redraw problem
			if (COSVersion() == OSV_LINUX)
			{
				m_toolbarMain.Invalidate(TRUE);
				m_filterBar.Invalidate(TRUE);
				m_cbQuickFind.Invalidate(TRUE);
			}

			Invalidate();
			UpdateWindow();
		}
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
	m_bStartHidden = FALSE;

	CPreferences prefs;
	CRect rect;
	
	rect.left = prefs.GetProfileInt(_T("Pos"), _T("Left"), -1);
	rect.top = prefs.GetProfileInt(_T("Pos"), _T("Top"), -1);
	rect.right = prefs.GetProfileInt(_T("Pos"), _T("Right"), -1);
	rect.bottom = prefs.GetProfileInt(_T("Pos"), _T("Bottom"), -1);
	
	if (!rect.IsRectEmpty())
	{
		CRect rScreen;

		if (GraphicsMisc::GetAvailableScreenSpace(rect, rScreen))
		{
			// because the position was saved using the results of 
			// GetWindowPlacement we must use SetWindowPlacement
			// to restore the window
			WINDOWPLACEMENT wp = { 0 };

			wp.rcNormalPosition = rect;
			wp.ptMaxPosition.x = -1;
			wp.ptMaxPosition.y = -1;
			wp.ptMinPosition.x = -1;
			wp.ptMinPosition.y = -1;
			
			//TRACE(_T("CToDoListWnd::SetWindowPlacement(%d, %d)\n"), rect.Width(), rect.Height());
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
		GraphicsMisc::ScaleByDPIFactor(rect);

		// make sure it fits the screen
		CRect rScreen;
		VERIFY(GraphicsMisc::GetAvailableScreenSpace(rect, rScreen, MONITOR_DEFAULTTOPRIMARY));

		if (rect.Height() > rScreen.Height())
			rect.bottom = rScreen.Height();

		MoveWindow(rect);
		CenterWindow();
	}
}

void CToDoListWnd::OnNewTasklist() 
{
	CreateNewTaskList(FALSE, TRUE);
	RefreshTabOrder();

	GetToDoCtrl().SetFocusToProjectName();
}

BOOL CToDoListWnd::CreateNewTaskList(BOOL bAddDefTask, BOOL bByUser)
{
	CFilteredToDoCtrl* pNew = NewToDoCtrl();
	
	if (pNew)
	{
		m_dlgTimeTracker.AddTasklist(pNew, CTaskFile());

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
		
		// We identify the user's explicitly created tasklists
		// by marking them as modified so we don't delete them 
		// when we are cleaning up
		pNew->SetModified(bByUser);

		VERIFY(AddToDoCtrl(pNew) != -1);

		if (bByUser)
			CheckRemovePristineTasklist();

		Resize();
		Invalidate();
	}

	return (pNew != NULL);
}

void CToDoListWnd::OnUpdateDeletetask(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanEditSelectedTask(TDCA_DELETE));
}

void CToDoListWnd::OnUpdateEditTasktext(CCmdUI* pCmdUI) 
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	int nSelCount = tdc.GetSelectedTaskCount();
	
	pCmdUI->Enable(tdc.CanEditSelectedTask(TDCA_TASKNAME) && (nSelCount == 1));	
}

void CToDoListWnd::OnUpdateTaskcolor(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanEditSelectedTask(TDCA_COLOR) && (Prefs().GetTextColorOption() == COLOROPT_DEFAULT));	
}

void CToDoListWnd::OnUpdateEditToggleTaskDone(CCmdUI* pCmdUI) 
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	int nSelCount = tdc.GetSelectedTaskCount();
	
	if (nSelCount == 1)
		pCmdUI->SetCheck(tdc.IsSelectedTaskDone() ? 1 : 0);
	
	pCmdUI->Enable(tdc.CanEditSelectedTask(TDCA_DONEDATE));	
}

void CToDoListWnd::OnUpdateDeletealltasks(CCmdUI* pCmdUI) 
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	pCmdUI->Enable(!tdc.IsReadOnly() && tdc.GetTaskCount() && !tdc.HasLockedTasks());	
}

void CToDoListWnd::OnUpdateSave(CCmdUI* pCmdUI) 
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	pCmdUI->Enable(GetTDCCount() && tdc.IsModified() && !tdc.IsReadOnly());	
}

void CToDoListWnd::OnUpdateNewTasklist(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(TRUE);	
}

BOOL CToDoListWnd::OnEraseBkgnd(CDC* pDC) 
{
	if (!GetTDCCount())
		return CFrameWnd::OnEraseBkgnd(pDC);

	CDialogHelper::ExcludeChild(&m_toolbarMain, pDC);
	CDialogHelper::ExcludeChild(&m_toolbarCustom, pDC);
	CDialogHelper::ExcludeChild(&m_statusBar, pDC);
	CDialogHelper::ExcludeChild(&m_tabCtrl, pDC);
	CDialogHelper::ExcludeChild(&m_filterBar, pDC);
	CDialogHelper::ExcludeChild(&GetToDoCtrl(), pDC);

	if (m_dlgFindTasks.IsDocked())
		CDialogHelper::ExcludeChild(&m_dlgFindTasks, pDC);

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

	// Bevel below the toolbars
	int nVPos = 0;

	if (m_toolbarCustom.GetSafeHwnd() && m_bShowingCustomToolbar)
	{
		nVPos = CDialogHelper::GetChildRect(&m_toolbarCustom).bottom;
	}
	else if (m_bShowingMainToolbar)
	{
		nVPos = m_toolbarMain.GetHeight();
	}

	if (nVPos > 0)
	{
		// Draw a bevel if the luminance of the toolbar and app color are similar
		COLORREF crAppBack = (WantTasklistTabbarVisible() ? m_theme.crAppBackDark : m_theme.crAppBackLight);

		if (RGBX::CalcLuminanceDifference(m_theme.crToolbarDark, crAppBack) < 0.3f)
		{
			GraphicsMisc::DrawHorzLine(pDC, 0, rClient.Width(), ++nVPos, m_theme.crAppLinesDark);
			GraphicsMisc::DrawHorzLine(pDC, 0, rClient.Width(), ++nVPos, m_theme.crAppLinesLight);
		}
	}	
	
	// filter-bar edges
	CRect rFilter;

	if (GetFilterBarRect(rFilter))
	{
		COLORREF crBkgnd = m_filterBar.CalcUIBkgndColor();

		if (crBkgnd != CLR_NONE)
			pDC->FillSolidRect(rFilter, crBkgnd);
	}

	// bevel above the statusbar if themed because classic theme provides it's own bevel
	if (m_bShowStatusBar && CThemed::IsAppThemed())
	{
		if (RGBX::CalcLuminanceDifference(m_theme.crStatusBarLight, m_theme.crAppBackLight) < 0.3f)
		{
			int nVPos = (CDialogHelper::GetChildRect(&m_statusBar).top - BEVEL);

			GraphicsMisc::DrawHorzLine(pDC, 0, rClient.Width(), nVPos++, m_theme.crAppLinesDark);
			GraphicsMisc::DrawHorzLine(pDC, 0, rClient.Width(), nVPos, m_theme.crAppLinesLight);
		}
	}

	// Find Tasks dialog splitter if docked
	CRect rSplitter;

	if (GetFindTasksDialogSplitterRect(rSplitter))
	{
		GraphicsMisc::DrawSplitBar(pDC, rSplitter, m_theme.crAppBackDark, FALSE);
	}

	// The CSysImageList class seems to not initialize properly unless the 
	// main window is visible. so in the case of starting hidden
	// or starting minimized we must wait until we become visible before
	// adding the tools to the toolbar.
	if (m_bFirstEraseBkgnd)
	{
		m_bFirstEraseBkgnd = FALSE;
		PostMessage(WM_UPDATEUDTSINTOOLBAR);
	}

	return TRUE;
}

BOOL CToDoListWnd::GetFindTasksDialogSplitterRect(CRect& rSplitter) const
{
	if (!m_bFindShowing || !m_dlgFindTasks.IsDocked())
		return FALSE;

	rSplitter = GetChildRect(&m_dlgFindTasks);

	switch (m_dlgFindTasks.GetDockPosition())
	{
	case DMP_LEFT:
		rSplitter.left = rSplitter.right;
		rSplitter.right += SPLITTER_WIDTH;
		break;

	case DMP_RIGHT:
		rSplitter.right = rSplitter.left;
		rSplitter.left -= SPLITTER_WIDTH;
		break;

	case DMP_BELOW:
		rSplitter.bottom = rSplitter.top;
		rSplitter.top -= SPLITTER_WIDTH;
		break;
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
	if (nCmdID == ID_SORTBY_MULTI)
		return;

	TDC_COLUMN nSortBy = TDC::MapSortIDToColumn(nCmdID);
	
	// update todoctrl
	GetToDoCtrl().Sort(nSortBy);
}

void CToDoListWnd::OnUpdateSortBy(CCmdUI* pCmdUI)
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	TDC_COLUMN nSortBy = TDC::MapSortIDToColumn(pCmdUI->m_nID);

	switch (pCmdUI->m_nID)
	{
	case ID_SORTBY_MULTI:
		pCmdUI->Enable(tdc.CanMultiSort());
		break;

	case ID_SORTBY_NONE:
		pCmdUI->Enable(TRUE);
		break;
		
	case ID_SORTBY_COLOR:
		pCmdUI->Enable(Prefs().GetTextColorOption() == COLOROPT_DEFAULT);
		break;
		
	case ID_SORTBY_PATH:
		pCmdUI->Enable(tdc.GetTaskView() == FTCV_TASKLIST);
		break;
		
	default:
		pCmdUI->Enable(tdc.CanSortBy(nSortBy));
		break;
	}

	// Radio button on current selection
	if (pCmdUI->m_pMenu)
	{
		UINT nSelCmdID = 0;

		if (tdc.IsMultiSorting())
		{
			nSelCmdID = ID_SORTBY_MULTI;
		}
		else if (tdc.GetSortBy() == nSortBy)
		{
			nSelCmdID = pCmdUI->m_nID;
		}

		if (nSelCmdID != 0)
			pCmdUI->m_pMenu->CheckMenuRadioItem(ID_SORTBY_ALLCOLUMNS_FIRST, ID_SORTBY_ALLCOLUMNS_LAST, nSelCmdID, MF_BYCOMMAND);
	}
}

BOOL CToDoListWnd::CreateNewTask(const CString& sTitle, TDC_INSERTWHERE nInsertWhere, BOOL bEdit, DWORD dwDependency)
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	// Special case
	if (!tdc.CanCreateNewTask(nInsertWhere) && (tdc.GetTaskCount() == 0))
	{
		// This location always works
		nInsertWhere = TDC::MapInsertIDToInsertWhere(ID_NEWTASK_ATTOP);
	}

	if (!tdc.CreateNewTask(sTitle, nInsertWhere, bEdit, dwDependency))
		return FALSE;

	// else
	CheckCreateDefaultReminder(tdc, tdc.GetSelectedTaskID());

	return TRUE;
}

void CToDoListWnd::CheckCreateDefaultReminder(const CFilteredToDoCtrl& tdc, DWORD dwTaskID)
{
	TDCREMINDER rem;

	if (Prefs().GetDefaultReminder(rem))
	{
		ASSERT(tdc.HasTask(dwTaskID));
		
		rem.dwTaskID = dwTaskID;
		rem.pTDC = &tdc;
		rem.bEnabled = TRUE;

		m_dlgReminders.SetReminder(rem);
	}
}

void CToDoListWnd::OnUpdateSort(CCmdUI* pCmdUI) 
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();

	pCmdUI->Enable(tdc.IsSorting() && tdc.GetTaskCount());	
}

void CToDoListWnd::OnEditTaskcolor() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	if (!tdc.IsReadOnly() && tdc.HasSelection())
	{
		CEnColorDialog dialog(tdc.GetSelectedTaskColor());

		CPreferences prefs;
		dialog.LoadPreferences(prefs);
		
		if (dialog.DoModal() == IDOK)
		{
			dialog.SavePreferences(prefs);
			tdc.SetSelectedTaskColor(dialog.GetColor());
		}
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
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	pCmdUI->Enable(tdc.CanEditSelectedTask(TDCA_COLOR) && 
					(Prefs().GetTextColorOption() == COLOROPT_DEFAULT) &&
					(tdc.GetSelectedTaskColor() != 0));	
}

void CToDoListWnd::OnEditToggleTaskDone() 
{
	GetToDoCtrl().SetSelectedTaskCompletion(TDCTC_TOGGLE);
}

void CToDoListWnd::OnEditTasktext() 
{
	GetToDoCtrl().EditSelectedTaskTitle();
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

LRESULT CToDoListWnd::OnToDoListRefreshPrefs(WPARAM /*wp*/, LPARAM /*lp*/)
{
	// sent by the app object if registry settings have changed
	ResetPrefs();

	// mark all tasklists as needing update
	m_mgrToDoCtrls.SetAllNeedPreferenceUpdate(TRUE);
	
	// then update active tasklist
	CheckUpdateActiveToDoCtrlPreferences();

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
		if (!bForUpdate && Prefs().HasSysTrayOptions(STO_ONCLOSE, STO_ONMINCLOSE))
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
		SaveAll(TDLS_AUTOSAVE);
	
	// hide main window
	Gui::MinToTray(*this); // courtesy of floyd
	m_bVisible = FALSE;
}

void CToDoListWnd::OnTrayiconClose() 
{
	DoExit();
}

LRESULT CToDoListWnd::OnToDoCtrlNotifyListChange(WPARAM /*wp*/, LPARAM lp)
{
	// decide whether the filter controls need updating
	TDC_ATTRIBUTE nAttribID = (TDC_ATTRIBUTE)lp;

	switch (nAttribID)
	{
	case TDCA_ALLOCTO:
	case TDCA_ALLOCBY:
	case TDCA_STATUS:
	case TDCA_CATEGORY:
	case TDCA_VERSION:
	case TDCA_TAGS:
		break;

	default:
		if (!TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		{
			// Invalid attribute
			ASSERT(0);
			return 0L;
		}
		break;
	}

	UpdateFilterBarListData(nAttribID);
	RefreshFindTasksListData(nAttribID);
	
	return 0L;
}

LRESULT CToDoListWnd::OnToDoCtrlNotifySelChange(WPARAM /*wp*/, LPARAM /*lp*/)
{
	UpdateStatusBar();

	return 0L;
}

LRESULT CToDoListWnd::OnToDoCtrlNotifyViewChange(WPARAM wp, LPARAM lp)
{
	if (GetTDCCount())
	{
		if (lp != (LPARAM)wp)
		{
			CFocusWatcher::UpdateFocus();

			RefreshFilterBarControls(TDCA_ALL);
			UpdateStatusBar();
		}
		else
		{
			int breakpoint = 0;
		}
	}

	return 0L;
}

LRESULT CToDoListWnd::OnToDoCtrlNotifySourceControlSave(WPARAM /*wp*/, LPARAM /*lp*/)
{
	m_mgrToDoCtrls.RefreshFileLastModified(GetSelToDoCtrl());

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
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	pCmdUI->Enable(tdc.CanTimeTrackSelectedTask());
	pCmdUI->SetCheck(tdc.IsSelectedTaskBeingTimeTracked() ? 1 : 0);
}

LRESULT CToDoListWnd::OnToDoCtrlNotifyTimeTrack(WPARAM wp, LPARAM lp)
{
	DWORD dwTrackID = lp;
	int nTDC = m_mgrToDoCtrls.FindToDoCtrl((HWND)wp);

	if (nTDC == -1)
	{
		ASSERT(0);
	}
	else 
	{
		if (dwTrackID)
		{
			StartTimeTrackingTask(nTDC, dwTrackID, FROM_TASKLIST);
		}
		else
		{
			StopTimeTrackingTask(nTDC, FROM_TASKLIST);
		}
	}

	return 0L;
}
	
void CToDoListWnd::StartTimeTrackingTask(int nTDC, DWORD dwTaskID, TIMETRACKSRC nFrom)
{
	ASSERT(nTDC != -1);
	ASSERT(dwTaskID);

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
					tdc.EndTimeTracking(TRUE);

				if (&tdc == m_dlgTimeTracker.GetSelectedTasklist())
					m_dlgTimeTracker.UpdateTracking(&tdc);

				m_mgrToDoCtrls.RefreshTimeTracking(nCtrl);
			}
		}
	}

	// Update Time track widget AND/OR todoctrl depending on who notified
	int nSel = GetSelToDoCtrl();
	
	// Must update TDC before the time-tracker
	if (nFrom != FROM_TASKLIST)
	{
		if ((nTDC == nSel) || SelectToDoCtrl(nTDC, TRUE))
			GetToDoCtrl().BeginTimeTracking(dwTaskID);
	}

	if (nFrom != FROM_TRACKER)
	{
		ASSERT(nTDC == nSel);

		m_dlgTimeTracker.UpdateTracking(&GetToDoCtrl());
	}
	
	// always refresh the notifier tasklist
	m_mgrToDoCtrls.RefreshTimeTracking(nTDC);

	// And the one potentially being paused
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
		ASSERT((nFrom != FROM_TASKLIST) || (nTDC == GetSelToDoCtrl()));

		// Update Time track widget AND/OR todoctrl depending on who notified
		CFilteredToDoCtrl& tdc = GetToDoCtrl(nTDC);
		
		// Must update TDC before the time-tracker
		if (nFrom != FROM_TASKLIST)
		{
			tdc.EndTimeTracking(TRUE);
		}

		if (nFrom != FROM_TRACKER)
		{
			m_dlgTimeTracker.UpdateTracking(&tdc);
		}

		// update tab image
		m_mgrToDoCtrls.RefreshTimeTracking(nTDC);
		
		// update tray icon
		OnTimerTimeTracking();
	}
}

LRESULT CToDoListWnd::OnTimeTrackerStartTracking(WPARAM wParam, LPARAM lParam)
{
	DWORD dwTaskID = lParam;
	int nTDC = m_mgrToDoCtrls.FindToDoCtrl((HWND)wParam);
	
	if (!dwTaskID || (nTDC == -1))
		ASSERT(0);
	else
		StartTimeTrackingTask(nTDC, dwTaskID, FROM_TRACKER);

	return 0L;
}

LRESULT CToDoListWnd::OnTimeTrackerStopTracking(WPARAM wParam, LPARAM /*lParam*/)
{
	int nTDC = m_mgrToDoCtrls.FindToDoCtrl((HWND)wParam);

	if (nTDC == -1)
		ASSERT(0);
	else
		StopTimeTrackingTask(nTDC, FROM_TRACKER);
	
	return 0L;
}

LRESULT CToDoListWnd::OnTimeTrackerResetElapsedTime(WPARAM wParam, LPARAM /*lParam*/)
{
	int nTDC = m_mgrToDoCtrls.FindToDoCtrl((HWND)wParam);

	if (nTDC == -1)
		ASSERT(0);
	else
		GetToDoCtrl(nTDC).ResetTimeTrackingElapsedMinutes();
	
	return 0L;
}

LRESULT CToDoListWnd::OnTimeTrackerLoadDelayedTasklist(WPARAM wParam, LPARAM /*lParam*/)
{
	int nTDC = m_mgrToDoCtrls.FindToDoCtrl((HWND)wParam);
	
	if (nTDC != -1)
		return VerifyTaskListOpen(nTDC, FALSE);

	// else
	ASSERT(0);
	return 0L;
}

LRESULT CToDoListWnd::OnToDoCtrlNotifyRecreateRecurringTask(WPARAM wp, LPARAM lp)
{
	DWORD dwTaskID = wp, dwNewTaskID = lp;

	// is there a reminder that we need to copy for the new task
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	TDCREMINDER rem;

	int nRem = m_dlgReminders.FindReminder(dwTaskID, &tdc);

	if (nRem != -1)
	{
		// Transfer the original if the task id has changed
		if (dwNewTaskID != dwTaskID)		
		{
			if (m_dlgReminders.TransferReminder(dwTaskID, dwNewTaskID, &tdc))
				tdc.RedrawReminders();
		}
		else // Update the existing reminder
		{
			// get the existing reminder
			m_dlgReminders.GetReminder(nRem, rem);

			// init for new task
			rem.bEnabled = TRUE;
			rem.dDaysSnooze = 0.0;

			// Add
			m_dlgReminders.SetReminder(rem);
		}
	}

	return 0L;
}

LRESULT CToDoListWnd::OnToDoCtrlNotifyFilterChange(WPARAM wp, LPARAM /*lp*/)
{
	int nTDC = m_mgrToDoCtrls.FindToDoCtrl((HWND)wp);

	if (nTDC == -1)
	{
		ASSERT(0);
	}
	else
	{
		m_filterBar.RefreshFilterControls(GetToDoCtrl(nTDC));

		CRect rFilter;
		
		if (GetFilterBarRect(rFilter))
			InvalidateRect(rFilter);
	}

	return 0L;
}

LRESULT CToDoListWnd::OnToDoCtrlNotifyMod(WPARAM wp, LPARAM lp)
{
	ASSERT(wp && lp);

	const TDCNOTIFYMOD* pMod = (TDCNOTIFYMOD*)lp;

	int nTDC = m_mgrToDoCtrls.FindToDoCtrl((HWND)wp);

	if (nTDC == -1)
	{
		// could be a notification from a TDC not yet added
		return 0L;
	}

	CFilteredToDoCtrl& tdc = GetToDoCtrl(nTDC);
	int nSelCount = tdc.GetSelectedTaskCount();

	if (pMod->mapAttrib.Has(TDCA_PROJECTNAME))
	{
		if (Prefs().GetKeepTabsOrdered())
			RefreshTabOrder();
	}

	if (pMod->mapAttrib.Has(TDCA_DUEDATE) || 
		pMod->mapAttrib.Has(TDCA_DONEDATE))
	{
		OnTimerDueItems(nTDC);
	}

	if (pMod->mapAttrib.Has(TDCA_CUSTOMATTRIBDEFS))
	{
		// Ignore modification callback if it came from us
		if (m_bSettingAttribDefs)
		{
			return 0L;
		}
		else if (m_dlgFindTasks.GetSafeHwnd())
		{
			UpdateFindDialogCustomAttributes(&tdc);
		}
	}

	if (pMod->mapAttrib.Has(TDCA_PASTE))
	{
		UpdateFilterBarListData(TDCA_ALL);
		RefreshFindTasksListData(TDCA_ALL);
	}

	UpdateTimeTrackerTasks(FALSE, pMod->mapAttrib);
	UpdateStatusBar(pMod->mapAttrib);

	if (m_dlgReminders.UpdateModifiedTasks(&tdc, pMod->aTaskIDs, pMod->mapAttrib))
		tdc.RedrawReminders();

	if (nTDC == GetSelToDoCtrl())
		UpdateCaption();
	else
		m_mgrToDoCtrls.UpdateTabItemText(nTDC);

	// refresh toolbar states
	PostMessage(WM_IDLEUPDATECMDUI, TRUE);

	return 0L;
}

void CToDoListWnd::UpdateCaption()
{
	int nSel = GetSelToDoCtrl();
	
	CString sProjectName = m_mgrToDoCtrls.UpdateTabItemText(nSel);
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
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
	
	// Prefix with task pathname if tasklist not visible
	// and status bar not visible
	if (!m_bShowStatusBar && (m_nMaxState == TDCMS_MAXCOMMENTS))
	{
		// quote the path to help it stand-out
		CString sTaskPath;
		sTaskPath.Format(_T("\"%s\""), tdc.GetSelectedTaskPath(TRUE, 100));
		
		if (!sTaskPath.IsEmpty())
			sCaption = (sTaskPath + " - " + sCaption);
	}

	// Prefix with 'title prefix'
	if (!m_sTitlePrefix.IsEmpty())
		sCaption = (m_sTitlePrefix.Left(20) + " - " + sCaption);
		
	CLocalizer::IgnoreString(sCaption);
	SetWindowText(sCaption);

	// set tray tip too
	UpdateTrayTooltip();
}

void CToDoListWnd::UpdateTrayTooltip()
{
    // base the tooltip on our current caption
    CString sTooltip;
    GetWindowText(sTooltip);

    // move copyright to next line and remove '-'
    sTooltip.Replace(_T(" - "), ENDL);

    // prefix with selected task as first line
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
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
        else if (tdc.GetSelectedTaskCount() > 1)
            sSelItem += _T(", ...");

        sTooltip = (sSelItem + ENDL + sTooltip);
    }

	CLocalizer::IgnoreString(sTooltip);

    m_trayIcon.SetTip(sTooltip);
}

void CToDoListWnd::OnSaveas() 
{
	int nSel = GetSelToDoCtrl();
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	CString sCurFilePath = m_mgrToDoCtrls.GetFilePath(nSel), sNewFilePath(sCurFilePath);
	CString sCurProjName = tdc.GetProjectName();
	
	// If the tasklist has already been saved and has a project name
	// then we show a custom dialog allowing both to be modified
	if (!sCurFilePath.IsEmpty() && !sCurProjName.IsEmpty())
	{
		CTDLTasklistSaveAsDlg dialog(sCurFilePath, 
									 sCurProjName,
									 TDL_EXT,
									 TDL_FILEFILTER);

		if (IDOK != dialog.DoModal())
			return;

		sNewFilePath = dialog.GetFilePath();

		tdc.SetProjectName(dialog.GetProjectName());
	}
	else // simple file dialog
	{
		sNewFilePath = m_mgrToDoCtrls.GetFilePath(nSel, FALSE);

		// Preserve existing file extension
		if (FileMisc::HasExtension(tdc.GetFilePath(), XML_EXT))
			FileMisc::ReplaceExtension(sNewFilePath, XML_EXT);
		else
			FileMisc::ReplaceExtension(sNewFilePath, TDL_EXT);
	
		CFileSaveDialog dialog(IDS_SAVETASKLISTAS_TITLE,
							   TDL_EXT,
							   sNewFilePath,
							   EOFN_DEFAULTSAVE,
							   TDL_FILEFILTER,
							   this);
	
		// always use .tdl for initializing the file dialog
		dialog.m_ofn.nFilterIndex = 1;
	
		if (IDOK != dialog.DoModal(CPreferences()))
			return;

		sNewFilePath = dialog.GetPathName();
	}
	
	if (!FileMisc::IsSamePath(sCurFilePath, sNewFilePath))
	{
 		if (SaveTaskList(nSel, sNewFilePath) == TDCF_SUCCESS)
		{
			m_mgrToDoCtrls.ClearStorageDetails(nSel);
			tdc.SetAlternatePreferencesKey(_T(""));
		}
		else
		{
			tdc.SetProjectName(sCurProjName); // revert change
		}
	}
}

void CToDoListWnd::OnUpdateSaveas(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CToDoListWnd::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	UINT nMenuID = 0;
	TDC_COLUMN nColID = TDCC_NONE;

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
				
				nMenuID = MM_TABCTRLCONTEXT;
			}
		}
	}
	else if (pWnd == &m_statusBar)
	{
		m_statusBar.ScreenToClient(&point);

		if (tdc.GetSelectedTaskCount() && (m_statusBar.HitTest(point) == 0))
		{
			m_statusBar.ClientToScreen(&point);
			nMenuID = MM_TASKCONTEXT;
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
			{
				if (tdc.GetSelectedTaskCount())
				{
					nMenuID = MM_TASKCONTEXT;
					nColID = tdc.HitTestColumn(point);
				}
				else
				{
					nMenuID = MM_TASKCONTEXTNOSEL;
				}
			}
			break;

		case TDCHT_COLUMNHEADER:
			nMenuID = MM_HEADERCONTEXT;
			nColID = tdc.HitTestColumn(point);
			break;
		}
	}
	
	// show the menu
	if (nMenuID && (nMenuID != m_nContextMenuID))
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
					m_menubar.PrepareTaskContextMenu(pPopup, tdc, Prefs());
					break;

				case MM_TABCTRLCONTEXT:
					m_menubar.PrepareTabCtrlContextMenu(pPopup, tdc, Prefs());
					break;
				}
				
				// prevent re-entrancy
				CAutoFlagT<UINT> af(m_nContextMenuID, nMenuID);
				CAutoFlagT<TDC_COLUMN> af2(m_nContextColumnID, nColID);

				CToolbarHelper::PrepareMenuItems(pPopup, this);

				// Ensure that the command is handled before the
				// auto-flags are restored to their original values
				UINT nCmdID = pPopup->TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);

				if (nCmdID)
					SendMessage(WM_COMMAND, nCmdID, 0);
			}
		}
	}
	else
	{
		CFrameWnd::OnContextMenu(pWnd, point);
	}
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
		CMessageBox::AfxShow(sMessage);//, IDS_DUETASKS_TITLE);
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
	return m_mgrToDoCtrls.VerifyPassword(nIndex);
}

void CToDoListWnd::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	// Keep the reminder window in from of the app
	if (m_dlgReminders.GetSafeHwnd() && 
		m_dlgReminders.IsWindowVisible() &&
		!m_dlgReminders.IsIconic() &&
		!Gui::IsObscured(m_dlgReminders))
	{
		lpwndpos->hwndInsertAfter = m_dlgReminders;
	}

	CFrameWnd::OnWindowPosChanging(lpwndpos);
}

void CToDoListWnd::Show(BOOL bAllowToggle)
{
	if (GetSelToDoCtrl() == -1)
		return;

	if (m_bStartHidden)
		RestorePosition();

	if (!m_bVisible || !IsWindowVisible()) // restore from the tray
	{
		SetForegroundWindow();
		
		if (!VerifyToDoCtrlPassword())
			return;

		m_bVisible = TRUE;
		Gui::RestoreFromTray(*this, CPreferences().GetProfileInt(_T("Pos"), _T("Maximized"), FALSE));

		// restore find dialog
		if (m_bFindShowing)
			m_dlgFindTasks.Show();
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
	else if (Prefs().HasSysTrayOptions(STO_NONE))
	{
		ShowWindow(SW_MINIMIZE);
	}
	else // hide to system tray
	{
		MinimizeToTray();
	}

	if (m_bVisible && !IsIconic())
	{
		// refresh all tasklists if we are visible
		OnTimerCheckReloadTasklists(-1, TRUE);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// DEBUG FUNCTIONS

#ifdef _DEBUG

void CToDoListWnd::OnDebugEndSession() 
{ 
	SendMessage(WM_QUERYENDSESSION); 
	SendMessage(WM_ENDSESSION, 1, 0); 
}

void CToDoListWnd::OnDebugShowSetupDlg() 
{ 
	CTDLWelcomeWizard dialog(CToDoListWnd::GetVersion(FALSE));
	dialog.DoModal();
}

void CToDoListWnd::OnDebugShowReminderDlg() 
{
	TDCREMINDER rem;
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	rem.dwTaskID = tdc.GetSelectedTaskID();
	rem.pTDC = &tdc;
	rem.bRelative = FALSE;
	rem.dtAbsolute = CDateHelper::GetDate(DHD_TODAY);
	rem.bEnabled = TRUE;
	
	m_dlgReminders.SetReminder(rem, TRUE);
}

void CToDoListWnd::OnDebugUpdateTranslation() 
{
	if (!CLocalizer::IsInitialized())
	{
		ASSERT(0);
		return;
	}
	
	// Reinitialise with 'add to dictionary'
	if (CLocalizer::GetTranslationOption() == ITTTO_TRANSLATEONLY)
	{
		CString sDictFile = CLocalizer::GetDictionaryPath();
		
		CLocalizer::Release();
		CLocalizer::Initialize(sDictFile, ITTTO_ADD2DICTIONARY);
		
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
		CLocalizer::EnableTranslation(ID_DEBUG_ENDSESSION, FALSE);
		CLocalizer::EnableTranslation(ID_DEBUG_SHOWSCRIPTDLG, FALSE);
		CLocalizer::EnableTranslation(ID_DEBUG_SHOWSETUPDLG, FALSE);
		CLocalizer::EnableTranslation(ID_DEBUG_SHOWUPDATEDLG, FALSE);
		CLocalizer::EnableTranslation(ID_DEBUG_TASKDIALOG_INFO, FALSE);
		CLocalizer::EnableTranslation(ID_DEBUG_TASKDIALOG_WARNING, FALSE);
		CLocalizer::EnableTranslation(ID_DEBUG_TASKDIALOG_ERROR, FALSE);
		CLocalizer::EnableTranslation(ID_DEBUG_UPDATETRANSLATION, FALSE);
		CLocalizer::EnableTranslation(ID_DEBUG_RESTARTAPPFROMEXPLORER, FALSE);
		CLocalizer::EnableTranslation(ID_DEBUG_RESTARTAPPFROMEXPLORER, FALSE);
		
		CLocalizer::ForceTranslateAllUIElements(IDS_FIRSTSTRING, IDS_LASTSTRING);
		
		// force a redraw of whole UI
		if (IsWindowVisible())
		{
			ShowWindow(SW_HIDE);
			ShowWindow(SW_SHOW);
		}
			
		SetForegroundWindow();
	}
	
	// Leave as 'add to dictionary' for duration of session
}

void CToDoListWnd::OnDebugCleanDictionaries() 
{
	DOPROGRESS(IDS_UPDATINGDICTIONARY);

	BOOL bIsInit = (CLocalizer::IsInitialized() && (CLocalizer::GetTranslationOption() == ITTTO_ADD2DICTIONARY));
	
	if (!bIsInit)
	{
		CString sDictFile = CLocalizer::GetDictionaryPath();

		CLocalizer::Release();
		CLocalizer::Initialize(sDictFile, ITTTO_ADD2DICTIONARY);
	}
	
	CString sDictPath = CLocalizer::GetDictionaryPath();
	CString sTransFolder = FileMisc::GetAppResourceFolder(_T("Resources\\Translations"));
	
	if (sDictPath.IsEmpty())
		sDictPath = sTransFolder;
	
	CString sMasterDict = (sTransFolder + _T("\\YourLanguage.csv"));
	
	VERIFY(CLocalizer::CleanupDictionary(sMasterDict, sDictPath));
}

void CToDoListWnd::OnDebugRestartApp()
{
	DoExit(TRUE);
}

void CToDoListWnd::OnDebugRestartAppFromExplorer()
{
	DoExit(FALSE); // don't restart

	CString sParams = AfxGetApp()->m_lpCmdLine;
	sParams += CEnCommandLineInfo::FormatSwitch(SWITCH_RESTART, Misc::Format(::GetCurrentProcessId()));

	if (!FileMisc::RunFromExplorer(FileMisc::GetModuleFilePath(), sParams))
	{
		AfxMessageBox(_T("Failed to restart app from Explorer"));
	}
}

void CToDoListWnd::OnUpdateDebugRestartAppFromExplorer(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FileMisc::IsAdminProcess());
}

#endif // DEBUG FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////

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
	ASSERT(nID >= ID_FILE_MRU1);
	ASSERT(nID < ID_FILE_MRU1 + (UINT)m_mruList.GetSize());
	
	int nIndex = nID - ID_FILE_MRU1;
	
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
	CFilteredToDoCtrl* pTDC = NewToDoCtrl(FALSE);

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

		if ((nNotifyDueBy != PFP_DONTNOTIFY) && CDateHelper::IsDateSet(dtEarliest.m_dt))
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
	
	int nCtrl = AddToDoCtrl(pTDC, &storageInfo);
	
	// Update time tracking widget
	m_dlgTimeTracker.AddTasklist(pTDC, CTaskFile());

	// update due item status
	if (CDateHelper::IsDateSet(dtEarliest))
	{
		TDCM_DUESTATUS nStatus = TDCM_FUTURE;
		COleDateTime dtToday = COleDateTime::GetCurrentTime();

		if (floor(dtEarliest) < floor(dtToday))
		{
			nStatus = TDCM_PAST;
		}
		else if (floor(dtEarliest) == floor(dtToday))
		{
			nStatus = TDCM_TODAY;
		}

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
	{
		if (GetLastError() == ERROR_BAD_NETPATH)
			return TDCF_BADNETWORK;

		// else
		return TDCF_NOTEXIST;
	}
	
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
	CHoldRedraw hr(*pTDC);
	
	// handles simple and storage tasklists
	// we use szFilePath because it may be storage Info not a true path
	TDC_FILE nOpen = OpenTaskList(pTDC, szFilePath, &storageInfo);
	
	if (nOpen == TDCF_SUCCESS)
	{
		int nTDC = AddToDoCtrl(pTDC, &storageInfo);

		// reload any reminders
		m_dlgReminders.AddToDoCtrl(pTDC);

		// And notify the tasklist if it's sorting by reminder
		if (pTDC->IsSortingBy(TDCC_REMINDER))
			pTDC->Resort();
		
		// notify user of due tasks if req
		if (bNotifyDueTasks)
			DoDueTaskNotification(nTDC, userPrefs.GetNotifyDueByOnLoad());
		
		UpdateCaption();
		UpdateStatusBar();
		OnTimerDueItems(nTDC);
		
		// update search
		if (userPrefs.GetRefreshFindOnLoad() && m_dlgFindTasks.GetSafeHwnd())
			m_dlgFindTasks.RefreshSearch();

		CheckRemovePristineTasklist();
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

	Resize();
	Invalidate();
	
	return nOpen;
}

TDC_FILE CToDoListWnd::OpenTaskList(CFilteredToDoCtrl* pTDC, LPCTSTR szFilePath, TSM_TASKLISTINFO* pInfo)
{
	ASSERT(!Misc::IsEmpty(szFilePath));

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
			{
				storageInfo = *pInfo;
			}

			// else
			sFilePath = storageInfo.szLocalFileName;

			// must set this up before loading tasklist
			// so that pTDC can access correct prefs
			pTDC->SetAlternatePreferencesKey(storageInfo.szDisplayName);
		}
		break;
		
	case TDCPP_FILE:
		if (!FileMisc::FileExists(sFilePath))
			return TDCF_NOTEXIST;
		break;
		
	case TDCPP_NONE:
		ASSERT(0);
		return TDCF_OTHER;
	}
	
	ASSERT(FileMisc::FileExists(sFilePath));

	BOOL bWasDelayed = pTDC->IsDelayLoaded();
	TDC_FILE nOpen = pTDC->Load(sFilePath, tasks, m_sMasterPassword);

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

			// check for automatic naming when handling WM_ENDSESSION. 
			// so we clear the filename and mark it as modified
			if (IsEndSessionFilePath(sFilePath))
			{
				pTDC->ClearFilePath();
				pTDC->SetModified();
			}
			else if (userPrefs.GetAddFilesToMRU())
			{
				m_mruList.Add(sFilePath);
			}
		}
		else // storage
		{
			ASSERT(nType == TDCPP_STORAGE);

			// return update storage info
			if (pInfo)
				*pInfo = storageInfo;
		}

		// Set master password once only
		if (m_bMasterPasswordEnabled && m_sMasterPassword.IsEmpty() && pTDC->IsEncrypted())
			m_sMasterPassword = pTDC->GetPassword();

		// Delay task expansion until we're finished
		if (userPrefs.GetExpandTasksOnLoad())
			PostMessage(WM_COMMAND, ID_VIEW_EXPANDALL);
		
		// update find dialog with this ToDoCtrl's custom attributes
		UpdateFindDialogActiveTasklist(pTDC);

		// Update time tracking widget
		if (bWasDelayed)
			m_dlgTimeTracker.SetTasks(pTDC, tasks);
		else
			m_dlgTimeTracker.AddTasklist(pTDC, tasks);
	}
	else
	{
		pTDC->SetModified(FALSE);
	}

	return nOpen;
}

void CToDoListWnd::UpdateFindDialogCustomAttributes(const CFilteredToDoCtrl* pTDC)
{
	ASSERT(pTDC);

	CTDCCustomAttribDefinitionArray aAllAttribDefs;
	m_mgrToDoCtrls.GetAllCustomAttributeDefinitions(aAllAttribDefs);
	
	CTDCCustomAttribDefinitionArray aTDCAttribDefs;
	pTDC->GetCustomAttributeDefs(aTDCAttribDefs);

	m_dlgFindTasks.SetCustomAttributes(aTDCAttribDefs, aAllAttribDefs);
}

void CToDoListWnd::UpdateFindDialogActiveTasklist(const CFilteredToDoCtrl* pTDC)
{
	if ((pTDC == NULL) && (GetTDCCount() == 0))
		return; // nothing to do

	// active tasklist
	if (pTDC == NULL)
		pTDC = &GetToDoCtrl();

	ASSERT (pTDC);
	m_dlgFindTasks.SetActiveTasklist(pTDC->GetFilePath(), Prefs().GetShowDefaultTaskIcons());

	UpdateFindDialogCustomAttributes(pTDC);
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

	TDCEXPORTTASKLIST* pExport = PrepareNewDueTaskNotification(nTDC, nDueBy);
	
	if (!pExport)
		return FALSE;

	return m_wndExport.ExportTasks(pExport);
}

TDCEXPORTTASKLIST* CToDoListWnd::PrepareNewDueTaskNotification(int nTDC, int nDueBy)
{
	// prepare thread structure
	const CFilteredToDoCtrl& tdc = GetToDoCtrl(nTDC);
	const CPreferencesDlg& userPrefs = Prefs();
	
	CString sStylesheet(userPrefs.GetDueTaskStylesheet());
	BOOL bTransform = GetStylesheetPath(tdc, sStylesheet);
	
	TDC_GETTASKS nFilter = TDCGT_DUE;
	UINT nIDDueBy = IDS_DUETODAY;
	
	switch (nDueBy)
	{
	case PFP_DUETODAY:
		break;
		
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
		return NULL;
	}

	TDCGETTASKS filter(nFilter);
	filter.sAllocTo = userPrefs.GetDueTaskPerson();

	if (bTransform)
		filter.dwFlags |= TDCGTF_TRANSFORM;

	// due task notification preference overrides Export preference
	if (!userPrefs.GetDueTaskTitlesOnly())
	{
		// visible attributes only
		TDC::MapColumnsToAttributes(tdc.GetVisibleColumns(), filter.mapAttribs);

		if (userPrefs.GetExportParentTitleCommentsOnly())
			filter.dwFlags |= TDCGTF_PARENTTITLECOMMENTSONLY;

		if (userPrefs.GetDisplayDueCommentsInHtml())
			filter.mapAttribs.Add(TDCA_HTMLCOMMENTS);
		else
			filter.mapAttribs.Add(TDCA_COMMENTS);
	}

	filter.mapAttribs.Add(TDCA_TASKNAME); // always
			
	// prepare structure
	int nExporter = -1;
	
	if (userPrefs.GetDisplayDueTasksInHtml())
		nExporter = m_mgrImportExport.GetExporter(TDCET_HTML);
	else
		nExporter = m_mgrImportExport.GetExporter(TDCET_TXT);

	CString sFileExt = m_mgrImportExport.GetExporterFileExtension(nExporter, FALSE);

	TDCEXPORTTASKLIST* pExport = new TDCEXPORTTASKLIST(GetSafeHwnd(), tdc.GetFilePath(), nExporter);
	ASSERT(pExport);

	pExport->nPurpose = TDCTEP_DUETASKNOTIFY;
	
	// different file for each
	pExport->sExportPath.Format(_T("tdl.due.%d"), nTDC);
	pExport->sExportPath = FileMisc::GetTempFilePath(pExport->sExportPath, sFileExt);

	if (FileMisc::IsLoggingEnabled())
		pExport->sSaveIntermediatePath = GetIntermediateTaskListPath(tdc.GetFilePath());

	if (!tdc.GetFilteredTasks(pExport->tasks, filter))
	{
		// cleanup
		delete pExport;
		return NULL;
	}
	
	// set an appropriate title
	pExport->tasks.SetReportDetails(CEnString(nIDDueBy));

	if (bTransform)
		pExport->sStylesheet = sStylesheet;

	// make sure import export manager is initialised 
	VERIFY(m_mgrImportExport.GetNumExporters());
	pExport->pImpExpMgr = &m_mgrImportExport;

	return pExport;
}

LRESULT CToDoListWnd::OnExportThreadFinished(WPARAM wp, LPARAM lp)
{
	///////////////////////////////////////////////////////////////////////
	// PERMANENT LOGGING
	FileMisc::LogTextRaw(_T("CToDoListWnd::OnExportThreadFinished(Received parent-notification)"));
	///////////////////////////////////////////////////////////////////////

	TDCEXPORTTASKLIST* pExport = (TDCEXPORTTASKLIST*)lp;
	ASSERT(pExport && pExport->IsValid());

	BOOL bSuccess = wp;

	if (!m_bClosing && bSuccess)
	{
		int nTDC = m_mgrToDoCtrls.FindToDoCtrl(pExport->sTDCPath);
		ASSERT(nTDC != -1);

		if (nTDC != -1)
		{
			switch (pExport->nPurpose)
			{
			case TDCTEP_DUETASKNOTIFY:
				Show(FALSE);
				m_mgrToDoCtrls.ShowDueTaskNotification(nTDC, pExport->sExportPath, Prefs().GetDisplayDueTasksInHtml());
				break;

			case TDCTEP_EXPORTAFTERSAVE:
				// Nothing to do
#ifdef _DEBUG
				{
					int breakpoint = 0;
				}
#endif
				break;
			}
		}
 	}

	// cleanup
	delete pExport;

	///////////////////////////////////////////////////////////////////////
	// PERMANENT LOGGING
	FileMisc::LogTextRaw(_T("CToDoListWnd::OnExportThreadFinished(Deleted allocated memory)\n"));
	///////////////////////////////////////////////////////////////////////

	return 0L;
}

CString CToDoListWnd::GetTitle(BOOL bExtended)
{
	CString sTitle;

	sTitle.Format(_T("ToDoList %s"), GetVersion(bExtended));
	CLocalizer::IgnoreString(sTitle);

	return sTitle;
}

CString CToDoListWnd::GetVersion(BOOL bExtended)
{
	static CString sVersion, sExtVersion;
	
	// One-time initialization
	if (sVersion.IsEmpty())
	{
		sVersion = sExtVersion = FileMisc::GetAppVersion();

		CDWordArray aVer;
		
		if (FileMisc::SplitVersionNumber(sVersion, aVer) == 4)
		{
			UINT nExtendedStr = IDS_GEN_RELEASE;
			CString sShortStr;

			// Pre-release version semantics
			switch (aVer[2])
			{
			case DEV_PREVIEW_VER:
				sShortStr = _T("DP");
				nExtendedStr = IDS_DEV_RELEASE;
				break;

			case ALPHA_RELEASE_VER:
				sShortStr = _T("A");
				nExtendedStr = IDS_ALPHA_RELEASE;
				break;

			case BETA_RELEASE_VER:
				sShortStr = _T("B");
				nExtendedStr = IDS_BETA_RELEASE;
				break;

			case RELEASE_CANDIDATE_VER:
				sShortStr = _T("RC");
				nExtendedStr = IDS_RC_RELEASE;
				break;
			}
			
			if (!sShortStr.IsEmpty())
			{
				const int LAST_MINOR_VER = 999;

				int nMajorVer = aVer[0];
				int nMinorVer = aVer[1];

				if (nMinorVer == LAST_MINOR_VER)
				{
					nMajorVer++;
					nMinorVer = 0;
				}
				else
				{
					nMinorVer++;
				}

				sVersion.Format(_T("%d.%d.%s%d"), nMajorVer, nMinorVer, sShortStr, aVer[3]);
			}

			sExtVersion.Format(_T("%s (%s)"), sVersion, CEnString(nExtendedStr));
		}

		CLocalizer::IgnoreString(sVersion);
		CLocalizer::IgnoreString(sExtVersion);
	}

	return (bExtended ? sExtVersion : sVersion);
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

BOOL CToDoListWnd::DoPreferences(int nInitPage, UINT nInitCtrlID) 
{
	// take a copy of current userPrefs to check changes against
	const CPreferencesDlg oldPrefs; 
	
	// kill timers for the duration
	KillTimers();

	ASSERT(m_pPrefs);

	// Pass in the selected tasklist's list data
	TDCAUTOLISTDATA autoListData;
	GetToDoCtrl().GetAutoListData(autoListData, TDCA_ALL);
	m_pPrefs->SetAutoListData(autoListData);

	// And all the custom attributes definitionsa
	CTDCCustomAttribDefinitionArray aAttribDefs;
	m_mgrToDoCtrls.GetAllCustomAttributeDefinitions(aAttribDefs);
	m_pPrefs->SetCustomAttributeDefs(aAttribDefs);

	UINT nRet = m_pPrefs->DoModal(nInitPage, nInitCtrlID);
	
	// updates userPrefs
	RedrawWindow();
	ResetPrefs();

	const CPreferencesDlg& newPrefs = Prefs();
	BOOL bModified = (nRet == IDOK);

	if (bModified)
	{
		// language changes may require restart so do that first
		if (UpdateLanguageTranslationAndCheckForRestart(oldPrefs))
		{
			DoExit(TRUE);
			return FALSE;
		}

		UpdateUITheme();

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
		BOOL bTopMost = (newPrefs.GetAlwaysOnTop() && !IsZoomed());
		SetWindowPos(bTopMost ? &wndTopMost : &wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		
		// tray icon
		m_trayIcon.ShowTrayIcon(newPrefs.GetUseSysTray());
		
		// support for .tdl extension/protocol
		if ((COSVersion() < OSV_VISTA) || FileMisc::IsAdminProcess())
		{
			EnableTDLExtension(newPrefs.GetEnableTDLExtension());
			EnableTDLProtocol(newPrefs.GetEnableTDLProtocol());
		}

		// default task attributes
		newPrefs.GetDefaultTaskAttributes(m_tdiDefault);

		// menu icons
		UINT nPrevID = MapNewTaskPos(oldPrefs.GetNewTaskPos(), FALSE);
		m_mgrMenuIcons.ChangeImageID(nPrevID, GetNewTaskCmdID());

		nPrevID = MapNewTaskPos(oldPrefs.GetNewSubtaskPos(), TRUE);
		m_mgrMenuIcons.ChangeImageID(nPrevID, GetNewSubtaskCmdID());
		
		// reload menu 
		InitMenubar();
		
		// tab bar
		if (newPrefs.GetShowTabCloseButtons())
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
		BOOL bRefreshFilter = FALSE;

		if (m_bShowFilterBar)
			bResizeDlg = TRUE;

		m_filterBar.ShowDefaultFilters(newPrefs.GetShowDefaultFiltersInFilterBar());

		BOOL bEnableMultiSel = newPrefs.GetMultiSelFilters();
		BOOL bPrevMultiSel = oldPrefs.GetMultiSelFilters();

		if (bPrevMultiSel != bEnableMultiSel)
		{
			m_filterBar.EnableMultiSelection(bEnableMultiSel);
			bRefreshFilter = TRUE;
		}

		if (m_filterBar.SetTitleFilterOption(newPrefs.GetTitleFilterOption()))
			bRefreshFilter = TRUE;

		if (newPrefs.GetRecentlyModifiedPeriod() != oldPrefs.GetRecentlyModifiedPeriod())
		{
			CFilteredToDoCtrl::SetRecentlyModifiedPeriod(newPrefs.GetRecentlyModifiedPeriod());
			bRefreshFilter = (m_filterBar.GetFilter() == FS_RECENTMOD);
		}
		
		if (bRefreshFilter)
			OnViewRefreshfilter();

		// hotkey
		UpdateGlobalHotkey();
		
		RefreshTabOrder();
		RefreshPauseTimeTracking();
		UpdateCaption();

		// colours
		if (m_dlgFindTasks.GetSafeHwnd())
			m_dlgFindTasks.RefreshUserPreferences();
		
		// active tasklist userPrefs
		CheckUpdateActiveToDoCtrlPreferences();
		UpdateTimeTrackerPreferences();

		// then refresh filter bar for any new default cats, statuses, etc
		RefreshFilterBarControls(TDCA_ALL);

		// Recreate custom toolbar as required before any resize
		CToolbarButtonArray aOldButtons, aNewButtons;

		oldPrefs.GetCustomToolbarButtons(aOldButtons);
		newPrefs.GetCustomToolbarButtons(aNewButtons);
		
		BOOL bRebuildCustomTB = !Misc::MatchAllT(aOldButtons, aNewButtons, TRUE);

		if (bRebuildCustomTB)
		{
			if (m_mgrMenuIcons.HasImages())
				m_mgrMenuIcons.RemoveImages(m_toolbarCustom);

			m_toolbarCustom.DestroyWindow();
			InitCustomToolbar();

			if (m_mgrMenuIcons.HasImages())
				m_mgrMenuIcons.AddImages(m_toolbarCustom);
		}
		
		if (bResizeDlg)
			Resize();

		Invalidate();
		UpdateWindow();

		// Stickies Support
		CString sStickiesPath;
		BOOL bShowFullTaskPathInSticky = FALSE;
		BOOL bUseStickies = newPrefs.GetUseStickies(sStickiesPath, bShowFullTaskPathInSticky);

		VERIFY(m_dlgReminders.UseStickies(bUseStickies, sStickiesPath, bShowFullTaskPathInSticky, FALSE));

		// Content controls
		m_mgrContent.LoadPreferences(CPreferences(), _T("ContentControls"), TRUE);

		// UDTs in toolbar
		BOOL bUpdateUDTs = (bRebuildCustomTB || (oldPrefs.GetUIThemeFile() != newPrefs.GetUIThemeFile()));

		if (!bUpdateUDTs)
		{
			CUserToolArray aOldTools, aNewTools;

			if (oldPrefs.GetDisplayUDTsInToolbar())
				oldPrefs.GetUserTools(aOldTools);

			if (newPrefs.GetDisplayUDTsInToolbar())
				newPrefs.GetUserTools(aNewTools);

			bUpdateUDTs = !Misc::MatchAllT(aOldTools, aNewTools, TRUE);
		}

		if (bUpdateUDTs)
			UpdateUDTsInToolbar(UDT_PREFERENCES);
	}
	
	// finally set or terminate the various status check timers
	RestoreTimers();

	return bModified;
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
			if (CMessageBox::AfxShow(IDS_RESTARTTOCHANGELANGUAGE, MB_YESNO) == IDYES)
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
		if (CMessageBox::AfxShow(IDS_RESTARTTOCHANGERTLINPUT, MB_YESNO) == IDYES)
		{
			return TRUE;
		}
	}
	
	// no need to restart
	return FALSE;
}


BOOL CToDoListWnd::InitMenubar()
{
	if (!m_menubar.LoadMenu(Prefs()))
		return FALSE;

	SetMenu(&m_menubar);
	m_hMenuDefault = m_menubar;

	return TRUE;
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
	BOOL bPauseAll = (((m_wndSessionStatus.IsLocked() || m_wndSessionStatus.IsScreenSaverActive()) && !Prefs().GetTrackOnScreenSaver()) || 
					  (m_wndSessionStatus.IsHibernated() && !Prefs().GetTrackHibernated()));

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

BOOL CToDoListWnd::HasTaskFile(const CTDCStartupOptions& startup) const
{
	CStringArray aFiles;

	// we only support finding a single filename
	if (startup.GetFilePaths(aFiles) == 1)
	{
		CString sFile(aFiles[0]);

		if (startup.HasFlag(TLD_TASKLINK))
		{
			CString sPath;
			DWORD dwTaskID = 0;

			if (!CFilteredToDoCtrl::ParseTaskLink(sFile, FALSE, _T(""), dwTaskID, sPath))
				return FALSE;

			sFile = sPath;
		}

		return (m_mgrToDoCtrls.FindToDoCtrl(sFile) != -1);
	}

	return FALSE;
}

BOOL CToDoListWnd::ProcessStartupOptions(const CTDCStartupOptions& startup, BOOL bStartup)
{
	// Miscellaneous
	m_sTitlePrefix = startup.GetTitlePrefix();

	// 1. Select the tasklist ------------------------------------
	if (startup.HasFilePath())
	{
		int nFirstSel = -1;

		CStringArray aFilePaths;
		int nNumFiles = startup.GetFilePaths(aFilePaths);

		for (int nFile = 0; nFile < nNumFiles; nFile++)
		{
			const CString& sFilePath = aFilePaths[nFile];
			
			if (startup.HasFlag(TLD_IMPORTFILE))
			{
				if (!ImportFile(sFilePath, TRUE))
					return FALSE;
			}
			else
			{
				// open the first tasklist fully and the rest delayed
				BOOL bCanDelayLoad = Prefs().GetEnableDelayedLoading();

				if ((nFirstSel == -1) || !Prefs().GetEnableDelayedLoading())
				{
					if (OpenTaskList(sFilePath, FALSE) != TDCF_SUCCESS)
						return FALSE;
				}
				else if (DelayOpenTaskList(sFilePath) != TDCF_SUCCESS)
				{
					return FALSE;
				}
			}

			// snapshot the first success for subsequent selection
			if (nFirstSel == -1)
				nFirstSel = GetSelToDoCtrl();
		}
		
		// set selection to first tasklist loaded
		ASSERT((nFirstSel != -1) && (nFirstSel < GetTDCCount()));

		SelectToDoCtrl(nFirstSel, FALSE);
	}

	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	// 2. Source control ------------------------------------------
	// 
	// A) If WantCheckOut AND WantCheckIn are set then '-ssc' was used 
	//    without an argument
	// B) Otherwise '-ssc' was followed by 'out' or 'in but not both
	// c) If both are set (no argument used) but the tasklist was
	//    already checked out then we don't checkin at the end
	BOOL bWantCheckIn = FALSE;

	if (tdc.IsSourceControlled())
	{
		bWantCheckIn = startup.WantCheckIn();
		
		if (startup.WantCheckOut())
		{
			if (!tdc.IsCheckedOut())
			{
				OnToolsCheckout();

				if (!tdc.IsCheckedOut())
					return FALSE;

				m_toolbarMain.RefreshButtonStates();
			}
			else
			{
				bWantCheckIn = FALSE;
			}
		}
	}
	
	// 3. Select/Create the task ----------------------------------
	if (startup.HasFlag(TLD_TASKLINK))
	{
		CStringArray aFiles;

		if (!startup.GetFilePaths(aFiles))
			return FALSE;

		CString sPath;
		DWORD dwTaskID = 0;

		CFilteredToDoCtrl::ParseTaskLink(aFiles[0], FALSE, _T(""), dwTaskID, sPath);
	
		if (!sPath.IsEmpty() && !ValidateTaskLinkFilePath(sPath))
			return FALSE;

		if (!DoTaskLink(sPath, dwTaskID, bStartup))
			return FALSE;
	}
	else if (startup.HasFlag(TLD_NEWTASK))
	{
		CEnString sNewTask;
		BOOL bEditTask = FALSE;

		// we edit the task name if no name was supplied
		if (!startup.GetNewTaskTitle(sNewTask))
		{
			sNewTask.LoadString(IDS_TASK);
			bEditTask = TRUE;
		}

		TDC_INSERTWHERE nWhere = TDC::MapInsertIDToInsertWhere(GetNewTaskCmdID()); // default

		// do we have a parent task ?
		if (tdc.SelectTask(startup.GetParentTaskID(), TRUE))
		{
			nWhere = TDC_INSERTATBOTTOMOFSELTASK;
		}
		// or a sibling task ?
		else if (tdc.SelectTask(startup.GetSiblingTaskID(), TRUE))
		{
			nWhere = TDC_INSERTAFTERSELTASK;
		}
		else if (!tdc.CanCreateNewTask(nWhere))
		{
			nWhere = TDC_INSERTATBOTTOM;
		}

		ASSERT(tdc.CanCreateNewTask(nWhere));

		if (!CreateNewTask(sNewTask, nWhere, FALSE)) // don't edit label immediately
			return FALSE;

		// creation date
		COleDateTime date;

		if (startup.GetTaskCreationDate(date))
			tdc.SetSelectedTaskDate(TDCD_CREATE, date);

		// edit task title?
		if (bEditTask)
		{	
			PostMessage(WM_COMMAND, ID_EDIT_TASKTEXT);
			return TRUE;
		}
	}
	else if (startup.GetTaskID())
	{
		if (!tdc.SelectTask(startup.GetTaskID(), TRUE))
			return FALSE;
	}
	else if (!startup.IsEmpty(TRUE))
	{
		// work on the currently selected item(s)
		if (tdc.GetSelectedTaskCount() == 0)
			return FALSE;
	}
		
	// 4. Modify the task --------------------------------------
	tdc.BeginSelectedTaskEdit();

	CStringArray aItems;
	CString sItem;
	int nItem;
	double dItem;

	if (startup.GetTaskTitle(sItem))
		tdc.SetSelectedTaskTitle(sItem, TRUE);

	if (startup.GetTaskComments(sItem))
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
		tdc.SetSelectedTaskComments(sItem);
	}

	if (startup.GetTaskExternalID(sItem))
		tdc.SetSelectedTaskExternalID(sItem);

	if (startup.GetTaskVersion(sItem))
		tdc.SetSelectedTaskVersion(sItem);

	if (startup.GetTaskAllocBy(sItem))
		tdc.SetSelectedTaskAllocBy(sItem);

	if (startup.GetTaskStatus(sItem))
		tdc.SetSelectedTaskStatus(sItem);

	if (startup.GetTaskIcon(sItem))
		tdc.SetSelectedTaskIcon(sItem);

	// Numeric items
	BOOL bOffset = FALSE;

	if (startup.GetTaskPriority(nItem, bOffset))
		tdc.SetSelectedTaskPriority(nItem, bOffset);

	if (startup.GetTaskRisk(nItem, bOffset))
		tdc.SetSelectedTaskRisk(nItem, bOffset);

	if (startup.GetTaskPercentDone(nItem, bOffset))
		tdc.SetSelectedTaskPercentDone(nItem, bOffset);

	if (startup.GetTaskCost(dItem, bOffset))
		tdc.SetSelectedTaskCost(TDCCOST(dItem), bOffset);

	// Times
	TDCTIMEPERIOD time;

	if (startup.GetTaskTimeEst(time.dAmount, time.nUnits, bOffset))
		tdc.SetSelectedTaskTimeEstimate(time, bOffset);

	if (startup.GetTaskTimeSpent(time.dAmount, time.nUnits, bOffset))
		tdc.SetSelectedTaskTimeSpent(time, bOffset);

	// Multi-string items
	BOOL bAppend = FALSE;

	if (startup.GetTaskAllocTo(aItems, bAppend) != -1)
		tdc.SetSelectedTaskAllocTo(aItems, bAppend);

	if (startup.GetTaskCategories(aItems, bAppend) != -1)
		tdc.SetSelectedTaskCategories(aItems, bAppend);

	if (startup.GetTaskDependencies(aItems, bAppend) != -1)
	{
		CTDCDependencyArray aDepends;

		if (aDepends.Set(aItems))
			tdc.SetSelectedTaskDependencies(aDepends, bAppend);
	}

	if (startup.GetTaskTags(aItems, bAppend) != -1)
		tdc.SetSelectedTaskTags(aItems, bAppend);

	if (startup.GetTaskFileLinks(aItems, bAppend) != -1)
		tdc.SetSelectedTaskFileLinks(aItems, bAppend);

	// start date and time
	TDC_UNITS nUnits;

	if (startup.GetTaskStartDate(dItem, nUnits, bOffset))
	{
		ASSERT(bOffset || (nUnits == DHU_DAYS));

		if (bOffset)
			tdc.OffsetSelectedTaskDate(TDCD_START, (int)dItem, nUnits, FALSE, FALSE);
		else
			tdc.SetSelectedTaskDate(TDCD_START, dItem);
	}

	// time overrides if present
	if (startup.GetTaskStartTime(dItem, nUnits, bOffset))
	{
		ASSERT(bOffset || (nUnits == DHU_DAYS));

		if (bOffset)
			tdc.OffsetSelectedTaskDate(TDCD_STARTTIME, (int)dItem, nUnits, FALSE, FALSE);
		else
			tdc.SetSelectedTaskDate(TDCD_STARTTIME, dItem);
	}

	// due date and time
	if (startup.GetTaskDueDate(dItem, nUnits, bOffset))
	{
		ASSERT(bOffset || (nUnits == DHU_DAYS));

		if (bOffset)
			tdc.OffsetSelectedTaskDate(TDCD_DUE, (int)dItem, nUnits, FALSE, FALSE);
		else
			tdc.SetSelectedTaskDate(TDCD_DUE, dItem);
	}

	// time overrides if present
	if (startup.GetTaskDueTime(dItem, nUnits, bOffset))
	{
		ASSERT(bOffset || (nUnits == DHU_DAYS));

		if (bOffset)
			tdc.OffsetSelectedTaskDate(TDCD_DUETIME, (int)dItem, nUnits, FALSE, FALSE);
		else
			tdc.SetSelectedTaskDate(TDCD_DUETIME, dItem);
	}

	// done date and time
	if (startup.GetTaskDoneDate(dItem, nUnits, bOffset))
	{
		ASSERT(bOffset || (nUnits == DHU_DAYS));

		if (bOffset)
			tdc.OffsetSelectedTaskDate(TDCD_DONE, (int)dItem, nUnits, FALSE, FALSE);
		else
			tdc.SetSelectedTaskDate(TDCD_DONE, dItem);
	}

	// time overrides if present
	if (startup.GetTaskDoneTime(dItem, nUnits, bOffset))
	{
		ASSERT(bOffset || (nUnits == DHU_DAYS));

		if (bOffset)
			tdc.OffsetSelectedTaskDate(TDCD_DONETIME, (int)dItem, nUnits, FALSE, FALSE);
		else
			tdc.SetSelectedTaskDate(TDCD_DONETIME, dItem);
	}

	// Custom attribute
	CString sAttribID;

	if (startup.GetTaskCustomAttribute(sAttribID, sItem))
		tdc.SetSelectedTaskCustomAttributeData(sAttribID, sItem);

	// Copying attributes
	CString sFrom, sTo;
	TDC_ATTRIBUTE nFrom, nTo;

	if (startup.GetCopyTaskAttribute(nFrom, nTo))
	{
		tdc.CopySelectedTaskAttributeData(nFrom, nTo);
	}
	else if (startup.GetCopyTaskAttribute(nFrom, sTo))
	{
		tdc.CopySelectedTaskAttributeData(nFrom, sTo);
	}
	else if (startup.GetCopyTaskAttribute(sFrom, nTo))
	{
		tdc.CopySelectedTaskAttributeData(sFrom, nTo);
	}
	else if (startup.GetCopyTaskAttribute(sFrom, sTo))
	{
		tdc.CopySelectedTaskAttributeData(sFrom, sTo);
	}

	tdc.EndSelectedTaskEdit();
	
	// 5. Execute any commands -------------------------------
	if (startup.HasCommandID())
	{
		CUIntArray aCmdIDs;
		int nNumCmd = startup.GetCommandIDs(aCmdIDs);

		for (int nCmd = 0; nCmd < nNumCmd; nCmd++)
			SendMessage(WM_COMMAND, MAKEWPARAM(aCmdIDs[nCmd], 0), 0);
	}

	// 6. Source Control --------------------------------------
	if (bWantCheckIn)
	{
		OnToolsCheckin();
	}

	// 7. Save tasklist intermediate state ------------------
	if (startup.HasFlag(TLD_SAVEINTERMEDIATE))
	{
		CString sOutputFile(GetIntermediateTaskListPath(GetToDoCtrl().GetFilePath()));
		CString sHtmlImgFolder = GetHtmlImageFolder(TRUE, sOutputFile);

		CTaskFile tasks;

		if (startup.GetSaveIntermediateAll())
		{
			GetTasks(tdc, TRUE, sHtmlImgFolder, TRUE, TSDT_ALL, TDCGETTASKS(), tasks);
		}
		else // use last state of transform dialog to determine what tasks to output
		{
			CTDLTransformDialog dialog(_T(""), 
										(tdc.GetTaskView() != FTCV_TASKLIST),
									   _T(""),
									   tdc.GetCustomAttributeDefs());

			GetTasks(tdc, TRUE, sHtmlImgFolder, TRUE, dialog.GetTaskSelection(), tasks);

			tasks.SetReportDetails(_T(""), dialog.GetDate());
		}

		if (!tasks.Save(sOutputFile, SFEF_UTF16))
			return FALSE;
	}

	return TRUE;
}

BOOL CToDoListWnd::OnCopyData(CWnd* /*pWnd*/, COPYDATASTRUCT* pCopyDataStruct)
{
	if (!pCopyDataStruct->lpData)
	{
		ASSERT(0);
		return FALSE;
	}

	switch (pCopyDataStruct->dwData)
	{
	case TDLCD_HASTASKFILE:
		if (pCopyDataStruct->cbData != sizeof(CTDCStartupOptions))
		{
			ASSERT(0);
		}
		else
		{
			const CTDCStartupOptions* pStartup = (CTDCStartupOptions*)(pCopyDataStruct->lpData);
			return HasTaskFile(*pStartup);
		}
		break;

	case TDLCD_PROCESSSTARTUP:
		if (pCopyDataStruct->cbData != sizeof(CTDCStartupOptions))
		{
			ASSERT(0);
		}
		else
		{
			const CTDCStartupOptions* pStartup = (CTDCStartupOptions*)(pCopyDataStruct->lpData);
			CString sOldPrefix = m_sTitlePrefix;

			if (ProcessStartupOptions(*pStartup, FALSE))
			{
				if (m_sTitlePrefix != sOldPrefix)
					UpdateCaption();

				return TRUE;
			}
		}
		break;

	case TDLCD_LANGFILECHANGE:
		{
			LPCTSTR szLangFile = (LPCTSTR)(pCopyDataStruct->lpData);

			if (!AfxIsValidString(szLangFile, pCopyDataStruct->cbData) || Misc::IsEmpty(szLangFile))
			{
				ASSERT(0);
			}
			else if (CLocalizer::GetDictionaryPath().CompareNoCase(szLangFile) == 0)
			{
				m_bPromptLanguageChangeRestartOnActivate = TRUE;
				return TRUE;
			}
		}
		break;
	}

	return FALSE; 
}

BOOL CToDoListWnd::ImportFile(LPCTSTR szFilePath, BOOL bSilent)
{
	int nImporter = m_mgrImportExport.FindImporterByPath(szFilePath);

	if (nImporter == -1)
		return FALSE;

	CTaskFile tasks;

	if (m_mgrImportExport.ImportTaskList(szFilePath, tasks, nImporter, bSilent) != IIER_SUCCESS)
		return FALSE;

	CFilteredToDoCtrl& tdc = GetToDoCtrl();
		
	if (!tdc.PasteTasks(tasks, TDC_INSERTATTOP, TRUE))
		return FALSE;

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
	pCmdUI->Enable(GetToDoCtrl().CanEditSelectedTask(TDCA_DELETE));	
}

void CToDoListWnd::OnEditPasteSub() 
{
	OnEditPaste(TDCP_ONSELTASK, TDIT_ADDTOSELECTEDTASK);
}

void CToDoListWnd::OnUpdateEditPasteSub(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CanPasteTasks(TDCP_ONSELTASK, FALSE));
}

void CToDoListWnd::OnEditPasteAfter() 
{
	int nSelCount = GetToDoCtrl().GetSelectedTaskCount();
	
	TDC_PASTE nPasteWhere = ((nSelCount == 0) ? TDCP_ATBOTTOM : TDCP_BELOWSELTASK);
	TDLID_IMPORTTO nImportWhere = TDIT_ADDBELOWSELECTEDTASK;
	
	OnEditPaste(nPasteWhere, nImportWhere);
}

void CToDoListWnd::OnUpdateEditPasteAfter(CCmdUI* pCmdUI) 
{
	TDC_PASTE nWhere = TDCP_BELOWSELTASK;
	
	// modify the text appropriately if the selection is empty
	if (GetToDoCtrl().GetSelectedTaskCount() == 0)
	{
		pCmdUI->SetText(CEnString(IDS_PASTETOPLEVELTASK));
		nWhere = TDCP_ATBOTTOM;
	}

	pCmdUI->Enable(CanPasteTasks(nWhere, FALSE));	
}

void CToDoListWnd::OnEditPaste(TDC_PASTE nPasteWhere, TDLID_IMPORTTO nImportWhere)
{
	CWaitCursor wait;
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	if (tdc.CanPasteTasks(nPasteWhere, FALSE))
	{
		tdc.PasteTasks(nPasteWhere, FALSE);
	}
	else if (CanImportPasteFromClipboard())
	{
		DoImportPasteFromClipboard(nImportWhere);

		RefreshFilterBarControls(TDCA_ALL, FALSE);
		UpdateTimeTrackerTasks(FALSE, TDCA_PASTE);
	}
}

void CToDoListWnd::OnEditPasteAsRef() 
{
	CWaitCursor wait;
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	tdc.PasteTasks(TDCP_ONSELTASK, TRUE);
}

void CToDoListWnd::OnUpdateEditPasteAsRef(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CanPasteTasks(TDCP_ONSELTASK, TRUE));
}

BOOL CToDoListWnd::CanPasteTasks(TDC_PASTE nWhere, BOOL bAsRef) const
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();

	if (bAsRef)
		return tdc.CanPasteTasks(nWhere, TRUE);

	if (tdc.CanPasteTasks(nWhere, FALSE))
		return TRUE;

	switch (nWhere)
	{
	case TDCP_ONSELTASK:
	case TDCP_BELOWSELTASK:
		if (!tdc.GetSelectedTaskCount())
			return FALSE;
		break;
	}

	// else try clipboard
	return CanImportPasteFromClipboard();
}

BOOL CToDoListWnd::CanImportPasteFromClipboard() const
{
	return (!GetToDoCtrl().IsReadOnly() && CTaskClipboard::IsEmpty() && Misc::ClipboardHasText());
}

BOOL CToDoListWnd::CanImportDropText(const CString& sText) const
{
	return (!GetToDoCtrl().IsReadOnly() && !sText.IsEmpty());
}

BOOL CToDoListWnd::DoImportPasteFromClipboard(TDLID_IMPORTTO nWhere)
{
	if (!CanImportPasteFromClipboard())
	{
		ASSERT(0);
		return FALSE;
	}

	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	CTDLImportDialog dialog(m_mgrImportExport, tdc.IsReadOnly(), tdc.HasSelection());

	dialog.SetUseClipboard();
	dialog.SetImportTo(nWhere);

	if (dialog.DoModal() != IDOK)
		return FALSE;

	TDLID_IMPORTTO nImportTo = dialog.GetImportTo();
	int nImporter = m_mgrImportExport.FindImporterByType(dialog.GetFormatTypeID());

	return ImportTasks(TRUE, dialog.GetImportText(), nImporter, nImportTo);
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
		sTasks = m_mgrImportExport.ExportTaskList(&tasks, TDCET_HTML);
		break;
		
	case TDCTC_ASTEXT:
		sTasks = m_mgrImportExport.ExportTaskList(&tasks, TDCET_TXT);
		break;
		
	case TDCTC_ASLINK:
		sTasks = tdc.FormatTaskLink(tdc.GetSelectedTaskID(), FALSE);
		break;
		
	case TDCTC_ASDEPENDS:
		sTasks = tdc.FormatTaskDependency(tdc.GetSelectedTaskID(), FALSE);
		break;
		
	case TDCTC_ASLINKFULL:
		sTasks = tdc.FormatTaskLink(tdc.GetSelectedTaskID(), TRUE);
		break;
		
	case TDCTC_ASDEPENDSFULL:
		sTasks = tdc.FormatTaskDependency(tdc.GetSelectedTaskID(), TRUE);
		break;

	case TDCTC_ASPATH:
		sTasks = tdc.GetSelectedTaskPath(TRUE);
		break;
	}

	CClipboard(*this).SetText(sTasks);
}

void CToDoListWnd::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().GetSelectedTaskCount() > 0);
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
	pCmdUI->SetCheck((m_nMaxState == TDCMS_MAXTASKLIST) ? 1 : 0);
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
	pCmdUI->SetCheck((m_nMaxState == TDCMS_MAXCOMMENTS) ? 1 : 0);
}

void CToDoListWnd::OnReload() 
{
	int nSel = GetSelToDoCtrl();
	
	if (m_mgrToDoCtrls.IsModified(nSel))
	{ 
		if (IDYES != CMessageBox::AfxShow(IDS_CONFIRMRELOAD_TITLE, IDS_CONFIRMRELOAD, MB_YESNOCANCEL | MB_DEFBUTTON2))
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
	// OpenTasklist will (correctly) want to re-add it
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
		UpdateStatusBar();

		RefreshFilterBarControls(TDCA_ALL);
		UpdateTimeTrackerTasks(TRUE);
	}
	else if (bNotifyError)
	{
		HandleLoadTasklistError(nRes, sFilePath);
	}

	return (nRes == TDCF_SUCCESS);
}

void CToDoListWnd::OnUpdateReload(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_mgrToDoCtrls.HasFilePath(GetSelToDoCtrl()));
}

void CToDoListWnd::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);

	if (m_bIgnoreResize)
	{
		ASSERT(nType == SIZE_RESTORED);
		//TRACE(_T("CToDoListWnd::OnSize(Ignoring Resize)\n"));

		return;
	}
	
	BOOL bVisible = ((m_bVisible > 0) && (nType != SIZE_MINIMIZED) && !m_bStartHidden);

	if (bVisible && m_toolbarMain.GetSafeHwnd())
	{
		// Delay-loading of resources else image lists seem to result 
		// in memory leaks inside of system dlls
		InitCustomToolbar();

		if (!m_ilTabCtrl.GetSafeHandle() && 
			m_ilTabCtrl.Create(16, 16, ILC_COLOR32 | ILC_MASK, 4, 1))
		{
			CBitmap bm;
			bm.LoadBitmap(IDB_SOURCECONTROL_STD);

			m_ilTabCtrl.Add(&bm, colorMagenta);
			m_ilTabCtrl.ScaleByDPIFactor();

			m_tabCtrl.SetImageList(&m_ilTabCtrl);
		}

		Resize(cx, cy, (nType == SIZE_MAXIMIZED));
		
		// if not maximized then set topmost if that's the preference
		// do nothing if no change
		BOOL bTopMost = (Prefs().GetAlwaysOnTop() && (nType != SIZE_MAXIMIZED)) ? 1 : 0;
		BOOL bIsTopMost = (GetExStyle() & WS_EX_TOPMOST) ? 1 : 0;
		
		if (bTopMost != bIsTopMost)
			SetWindowPos(bTopMost ? &wndTopMost : &wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
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
	
	CRect rTaskList(0, 0, cx, cy);
	
	// toolbar
	if (m_bShowingMainToolbar) 
 		rTaskList.top += m_toolbarMain.GetHeight();
	
	if (m_toolbarCustom.GetSafeHwnd() && m_bShowingCustomToolbar) 
	{
		BOOL bSeparateLine = !m_bShowingMainToolbar;
		
		if (!bSeparateLine)
		{
			int nMainLen = m_toolbarMain.GetMinReqLength();
			int nCustLen = m_toolbarCustom.GetMinReqLength();

			if (!m_toolbarMain.LastItemIsSeparator())
				nMainLen += m_nToolbarEndSepWidth;

			bSeparateLine = ((nMainLen + nCustLen) > cx);
		}

		if (bSeparateLine)
			rTaskList.top += m_toolbarCustom.GetHeight();
	}

	// Bevel below toolbars
	if (rTaskList.top > 0)
		rTaskList.top += BEVEL;
	
	ReposStatusBar(NULL, rTaskList);
	ReposFindTasksDialog(NULL, rTaskList);
	ReposTabBar(NULL, rTaskList);
	ReposFilterBar(NULL, rTaskList);
	ReposTaskList(NULL, rTaskList);

	rect = rTaskList;
	return TRUE;
}

void CToDoListWnd::Resize(int cx, int cy, BOOL bMaximized)
{
	if (m_bIgnoreResize)
		return;

	// Don't resize if hidden in any way
	if ((m_bVisible <= 0) || IsIconic())
		return;

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
	CDeferWndMove dwm(10);
	CRect rTaskList(0, 0, cx, cy);
	
	ReposToolbars(&dwm, rTaskList);
	ReposStatusBar(&dwm, rTaskList);	
	ReposFindTasksDialog(&dwm, rTaskList);
	ReposTabBar(&dwm, rTaskList);
	ReposFilterBar(&dwm, rTaskList);
	ReposTaskList(&dwm, rTaskList);
	
#ifdef _DEBUG
	CRect rect;
	CalcToDoCtrlRect(rect, cx, cy, IsZoomed());

	if (rect != rTaskList)
		ASSERT(0);
#endif

	Invalidate(TRUE);
}

void CToDoListWnd::ReposToolbars(CDeferWndMove* pDwm, CRect& rAvailable)
{
	// toolbar
	if (m_bShowingMainToolbar) // showing toolbar
		rAvailable.top += m_toolbarMain.Resize(rAvailable.Width(), rAvailable.TopLeft());

	// ensure m_cbQuickFind is positioned correctly
	int nPos = m_toolbarMain.CommandToIndex(ID_EDIT_FINDTASKS) + 2;

	CRect rNewPos;
	m_toolbarMain.GetItemRect(nPos, rNewPos);

	// check if it needs to be moved
	CRect rPrevPos = GetChildRect(&m_cbQuickFind);

	if (rNewPos.TopLeft() != rPrevPos.TopLeft())
	{
		rNewPos.top += QUICKFIND_VOFFSET;
		rNewPos.bottom = rNewPos.top + QUICKFIND_HEIGHT;
		rNewPos.OffsetRect(QUICKFIND_HOFFSET, 0);

		m_cbQuickFind.MoveWindow(rNewPos);
	}

	// Attempt to put the custom toolbar on the same line
	if (m_toolbarCustom.GetSafeHwnd() && m_bShowingCustomToolbar)
	{
		BOOL bSeparateLine = !m_bShowingMainToolbar;

		if (!bSeparateLine)
		{
			int nMainLen = m_toolbarMain.GetMinReqLength();
			int nCustLen = m_toolbarCustom.GetMinReqLength();

			if (!m_toolbarMain.LastItemIsSeparator())
				nMainLen += m_nToolbarEndSepWidth;

			bSeparateLine = ((nMainLen + nCustLen) > rAvailable.Width());
		}

		if (bSeparateLine)
		{
			if (m_toolbarMain.LastItemIsSeparator())
				m_toolbarMain.DeleteLastItem();

			rAvailable.top += m_toolbarCustom.Resize(rAvailable.Width(), CPoint(rAvailable.left, rAvailable.top));
		}
		else
		{
			if (!m_toolbarMain.LastItemIsSeparator())
				m_toolbarMain.AppendSeparator();

			int nMainLen = m_toolbarMain.GetMinReqLength();

			m_toolbarMain.Resize(nMainLen);
			m_toolbarCustom.Resize(rAvailable.Width() - nMainLen, CPoint(nMainLen, 0));
		}
	}

	// Bevel below toolbars
	if (rAvailable.top > 0)
		rAvailable.top += BEVEL;
}

void CToDoListWnd::ReposFindTasksDialog(CDeferWndMove* pDwm, CRect& rAvailable)
{
	if (m_bFindShowing && m_dlgFindTasks.IsDocked())
	{
		CRect rFindTasks(rAvailable);

		DM_POS nPos = m_dlgFindTasks.GetDockPosition();
		int nDim = m_dlgFindTasks.GetDockedDimension(nPos, IsZoomed());

		switch (nPos)
		{
		case DMP_LEFT:
			rFindTasks.right = rFindTasks.left + nDim;
			rAvailable.left += nDim;
			rAvailable.left += SPLITTER_WIDTH;
			break;

		case DMP_RIGHT:
			rFindTasks.left = rFindTasks.right - nDim;
			rAvailable.right -= nDim;
			rAvailable.right -= SPLITTER_WIDTH;
			break;

		case DMP_BELOW:
			rFindTasks.top = rFindTasks.bottom - nDim;
			rAvailable.bottom -= nDim;
			rAvailable.bottom -= SPLITTER_WIDTH;
			break;
		}

		if (pDwm)
			pDwm->MoveWindow(&m_dlgFindTasks, rFindTasks);
	}
}

void CToDoListWnd::ReposFilterBar(CDeferWndMove* pDwm, CRect& rAvailable)
{
	CRect rFilter(rAvailable);

	int nInset = CalcEditFieldInset();
	int nFilterWidth = rAvailable.Width() - 2 * nInset;
	int nFilterHeight = m_bShowFilterBar ? m_filterBar.CalcHeight(nFilterWidth) : 0;

	rAvailable.top += nFilterHeight;
	rFilter.bottom = rAvailable.top;
	rFilter.left += nInset;

	if (pDwm)
	{
		pDwm->MoveWindow(&m_filterBar, rFilter);
		m_filterBar.Invalidate();
	}
}

int CToDoListWnd::CalcEditFieldInset() const
{
	if (m_bFindShowing && (m_dlgFindTasks.GetDockPosition() == DMP_LEFT))
		return 0;

	return (CThemed::IsNonClientThemed() ? BORDER : BEVEL);
}

void CToDoListWnd::ReposStatusBar(CDeferWndMove* pDwm, CRect& rAvailable)
{
	// If visible, the statusbar has already been automatically resized
	CRect rStatus(rAvailable);

	if (m_bShowStatusBar)
	{
		m_statusBar.GetWindowRect(rStatus);
		ScreenToClient(rStatus);

		rAvailable.bottom = rStatus.top - BORDER;
	}
	else
	{
		if (pDwm)
			pDwm->MoveWindow(&m_statusBar, rStatus, FALSE);

		rAvailable.bottom = rStatus.bottom - BORDER;
	}
}

void CToDoListWnd::ReposTabBar(CDeferWndMove* pDwm, CRect& rAvailable)
{
	CRect rTabs(rAvailable.left, 0, rAvailable.Width(), 0);
	m_tabCtrl.AdjustRect(TRUE, rTabs);

	int nTabHeight = rTabs.Height() - 4;

	rTabs = rAvailable;
	rTabs.right++;
	rTabs.bottom = rTabs.top + nTabHeight;
//	rTabs.OffsetRect(0, rAvailable.y - rTabs.top); // add a pixel between tabbar and toolbar

	BOOL bNeedTabCtrl = WantTasklistTabbarVisible();

	if (bNeedTabCtrl)
		rAvailable.top += rTabs.Height();

	if (pDwm)
	{
		pDwm->MoveWindow(&m_tabCtrl, rTabs);

		// hide and disable tabctrl if not needed
		m_tabCtrl.ShowWindow(bNeedTabCtrl ? SW_SHOW : SW_HIDE);
		m_tabCtrl.EnableWindow(bNeedTabCtrl);
	}
}

void CToDoListWnd::ReposTaskList(CDeferWndMove* pDwm, CRect& rAvailable)
{
	// finally the active todoctrl
	if (GetTDCCount())
	{
		// shrink slightly so that edit controls do not merge with window border
		int nIndent = (CThemed::IsNonClientThemed() ? BORDER : BEVEL);

		if (!m_bFindShowing || !m_dlgFindTasks.IsDocked())
		{
			rAvailable.DeflateRect(nIndent, nIndent, nIndent, nIndent);
		}
		else
		{
			switch (m_dlgFindTasks.GetDockPosition())
			{
			case DMP_LEFT:
				rAvailable.DeflateRect(0, nIndent, nIndent, nIndent);
				break;

			case DMP_RIGHT:
				rAvailable.DeflateRect(nIndent, nIndent, 0, nIndent);
				break;

			case DMP_BELOW:
				rAvailable.DeflateRect(nIndent, nIndent, nIndent, 0);
				break;
			}
		}

		if (pDwm)
		{
			CFilteredToDoCtrl& tdc = GetToDoCtrl();

			pDwm->MoveWindow(&tdc, rAvailable, FALSE);
			tdc.Invalidate();
		}
	}
}

BOOL CToDoListWnd::WantTasklistTabbarVisible() const 
{ 
	if (m_bReloading)
		return FALSE;

	if (!m_bShowTasklistBar)
		return FALSE;

	return ((GetTDCCount() > 1) || !Prefs().GetAutoHideTabbar()); 
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
	CTDLPrintDialog dialog(sTitle,
						   bPreview,
						   m_mgrImportExport,
						   (tdc.GetTaskView() != FTCV_TASKLIST),
						   tdc.GetStylesheetPath(),
						   tdc.GetCustomAttributeDefs(),
						   (tdc.CanSaveTaskViewToImage() ? tdc.GetTaskViewName() : _T("")));
	
	if (dialog.DoModal() != IDOK)
		return;

	RedrawWindow();
	
	DOPROGRESS(bPreview ? IDS_PPREVIEWPROGRESS : IDS_PRINTPROGRESS);
	
	// always use the same file
	if (!CreateTempPrintFile(dialog))
	{
		// Cancelled or error handled
		return;
	}

	// print from browser
	CRect rHidden(-20, -20, -10, -10); // create IE off screen
	
	if (m_IE.GetSafeHwnd() || m_IE.Create(NULL, WS_CHILD | WS_VISIBLE, rHidden, this, (UINT)IDC_STATIC))
	{
		if (bPreview)
			m_IE.PrintPreview(TEMP_PRINT_FILEPATH, TRUE); // TRUE = Print background colours
		else
			m_IE.Print(TEMP_PRINT_FILEPATH, TRUE); // TRUE = Print background colours
	}
	else // try sending to browser
	{
		DWORD dwRes = FileMisc::Run(*this, TEMP_PRINT_FILEPATH, NULL, SW_HIDE, NULL, bPreview ? _T("print") : NULL);
								
		if (dwRes < 32)
			CMessageBox::AfxShow(IDS_PRINTFAILED_TITLE, IDS_PRINTFAILED, MB_OK);
	}
}

BOOL CToDoListWnd::CreateTempPrintFile(const CTDLPrintDialog& dlg)
{
	CWaitCursor cursor;

	TDLPD_STYLE nStyle = dlg.GetExportStyle();
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	CString sHtmlImgFolder = GetHtmlImageFolder(TRUE, TEMP_PRINT_FILEPATH);


	switch (nStyle)
	{
	case TDLPDS_IMAGE:
		{
			CString sTempImg = TEMP_TASKVIEW_FILEPATH;

			// export
			if (tdc.SaveTaskViewToImage(sTempImg))
			{
				// Make a simple web page container
				CString sHtmlOutput(_T("<!DOCTYPE html>\n"));

				sHtmlOutput += _T("<html>\n<head>\n");
				sHtmlOutput += _T("<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-16\">\n");
				sHtmlOutput += _T("</head>\n<style>\n");
				sHtmlOutput += _T("img { max-width: 100%; } \n");
				sHtmlOutput += _T("</style>\n<body>\n");

				CString sTitle = dlg.GetTitle(), sDate = CDateHelper::FormatDate(dlg.GetDate());

				if (!sDate.IsEmpty() || !sTitle.IsEmpty())
				{
					CString sTitleTag;
					sTitleTag.Format(_T("<font face='%s' size='%d'>\n<h2>%s</h2>%s<p/>"),
									 Prefs().GetHtmlFont(), Prefs().GetHtmlFontSize(),
									 sTitle, sDate);

					sHtmlOutput += sTitleTag;
				}

				CString sImage;
				sImage.Format(_T("<img src=\"%s\">"), sTempImg);

				sHtmlOutput += sImage;
				sHtmlOutput += _T("</body>\n</html>\n");

				return FileMisc::SaveFile(TEMP_PRINT_FILEPATH, sHtmlOutput, SFEF_UTF8WITHOUTBOM);
			}
			else
			{
				CMessageBox::AfxShow(IDS_SAVETOIMAGEERROR);
				return FALSE;
			}
		}
		break;

	case TDLPDS_STYLESHEET:
		{
			CString sStylesheet;
			VERIFY(dlg.GetStylesheet(sStylesheet));

			CTaskFile tasks;
			GetTasks(tdc, TRUE, sHtmlImgFolder, TRUE, dlg.GetTaskSelection(), tasks);

			tasks.SetReportDetails(dlg.GetTitle(), dlg.GetDate());

			// save intermediate tasklist to file as required
			LogIntermediateTaskList(tasks);

			// export
			return tasks.TransformToFile(sStylesheet, TEMP_PRINT_FILEPATH);
		}
		break;

	case TDLPDS_OTHERHTMLEXPORTER:
		{
			CString sExporterTypeID;
			VERIFY(dlg.GetOtherExporterTypeID(sExporterTypeID));

			CTaskFile tasks;
			GetTasks(tdc, TRUE, sHtmlImgFolder, FALSE, dlg.GetTaskSelection(), tasks);

			tasks.SetReportDetails(dlg.GetTitle(), dlg.GetDate());

			// save intermediate tasklist to file as required
			LogIntermediateTaskList(tasks);

			// export
			return (m_mgrImportExport.ExportTaskList(tasks, TEMP_PRINT_FILEPATH, sExporterTypeID, IIEF_PRINTING) == IIER_SUCCESS);
		}
		break;

	case TDLPDS_WRAP:
	case TDLPDS_TABLE:
	case TDLPDS_PARA:
		// simple web page
		{
			CTaskFile tasks;
			GetTasks(tdc, TRUE, sHtmlImgFolder, FALSE, dlg.GetTaskSelection(), tasks);

			tasks.SetReportDetails(dlg.GetTitle(), dlg.GetDate());

			// save intermediate tasklist to file as required
			LogIntermediateTaskList(tasks);

			// export
			DWORD dwFlags = (IIEF_PRINTING | TDC::MapPrintToExportStyle(nStyle));

			return (m_mgrImportExport.ExportTaskList(tasks, TEMP_PRINT_FILEPATH, TDCET_HTML, dwFlags) == IIER_SUCCESS);
		}
		break;
	}

	ASSERT(0);
	return FALSE;
}

void CToDoListWnd::OnUpdatePrint(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().GetTaskCount());
}

int CToDoListWnd::AddToDoCtrl(CFilteredToDoCtrl* pTDC, TSM_TASKLISTINFO* pInfo)
{
	// Add tdc first to ensure tab controls has some
	// items before we query it for its size
	int nSel = m_mgrToDoCtrls.AddToDoCtrl(pTDC, pInfo);

	// make sure size is right
	CRect rTDC;
	
	if (CalcToDoCtrlRect(rTDC))
		pTDC->MoveWindow(rTDC);

	if (!pTDC->IsDelayLoaded() && SelectToDoCtrl(nSel, FALSE))
	{
		pTDC->SetFocusToTasks();
	
		// if this is the only control then set or terminate the various status 
		// check timers
		if (GetTDCCount() == 1)
			RestoreTimers();

		Invalidate();
		UpdateWindow();
	}
	else
	{
		pTDC->ShowWindow(SW_HIDE);
	}

	// Show the tab-bar as required
	if (WantTasklistTabbarVisible() && !m_tabCtrl.IsWindowVisible())
		Resize();

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

	const CToDoCtrl* pTDC = m_dlgTimeTracker.GetSelectedTasklist();

	if (pTDC && pTDC->IsActivelyTimeTracking())
		m_dlgTimeTracker.UpdateTaskTime(pTDC);
}

void CToDoListWnd::UpdateWindowIcons()
{
	BOOL bTimeTracking = IsActivelyTimeTracking();
	UINT nIDIcon = (bTimeTracking ? IDI_TIMETRACK_STD : IDR_MAINFRAME_STD);

	if (!m_icons.IsInitialised())
		VERIFY(m_icons.Initialise(*this, nIDIcon));
	else
		VERIFY(m_icons.ModifyIcon(nIDIcon));

	m_dlgTimeTracker.SetWindowIcons(m_icons.GetBigIcon(), m_icons.GetSmallIcon());

	UpdateTrayIcon();
}

void CToDoListWnd::UpdateTrayIcon()
{
	BOOL bTimeTracking = IsActivelyTimeTracking();
	UINT nIDIcon = 0;

	if (COSVersion() >= OSV_WIN8)
	{
		DWORD dwTaskbarUsesLightTheme = FALSE;

		CRegKey2 reg;

		if (reg.Open(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize"), TRUE) == ERROR_SUCCESS)
			reg.Read(_T("SystemUsesLightTheme"), dwTaskbarUsesLightTheme);

		if (dwTaskbarUsesLightTheme)
			nIDIcon = (bTimeTracking ? IDI_TRAY_TIMETRACK_DARK : IDI_TRAY_DARK);
		else
			nIDIcon = (bTimeTracking ? IDI_TRAY_TIMETRACK_LIGHT : IDI_TRAY_LIGHT);
	}
	else
	{
		nIDIcon = (bTimeTracking ? IDI_TIMETRACK_STD : IDR_MAINFRAME_STD);
	}

	m_trayIcon.SetIcon(nIDIcon);
}

LPARAM CToDoListWnd::OnToDoCtrlNotifyTimeTrackReminder(WPARAM wParam, LPARAM lParam)
{
	DWORD dwTrackID = lParam;
	int nTDC = m_mgrToDoCtrls.FindToDoCtrl((HWND)wParam);

	if (!dwTrackID || (nTDC == -1))
	{
		ASSERT(0);
		return 0L;
	}

	const CFilteredToDoCtrl& tdc = GetToDoCtrl(nTDC);

	CEnString sMessage;
	sMessage.Format(IDS_TIMETRACKREMINDER, 
					Prefs().GetTrackReminderFrequency(),
					tdc.GetFriendlyProjectName(),
					tdc.GetTaskTitle(dwTrackID));

	m_trayIcon.ShowBalloon(sMessage, CEnString(IDS_TIMETRACKREMINDER_BALLOONTITLE), NIIF_INFO);

	// Option extras
	const CPreferencesDlg& prefs = Prefs();
	CString sSoundFile = prefs.GetTrackReminderSoundFile();

	if (!sSoundFile.IsEmpty())
		PlaySound(sSoundFile, NULL, (SND_FILENAME | SND_ASYNC));

	// And optionally end tracking
	if (prefs.GetEndTrackingOnReminder())
		StopTimeTrackingTask(nTDC, FROM_APP);

	return TRUE;
}

void CToDoListWnd::OnTimerDueItems(int nCtrl)
{
	AF_NOREENTRANT // macro helper

	BOOL bRepaintTabs = FALSE;

	if (nCtrl == -1)
		bRepaintTabs = m_mgrToDoCtrls.RefreshDueItemStatus();
	else
		bRepaintTabs = m_mgrToDoCtrls.RefreshDueItemStatus(nCtrl);

	if (bRepaintTabs)
		m_tabCtrl.Invalidate(FALSE);
}

void CToDoListWnd::OnTimerReadOnlyStatus(int nCtrl, BOOL bForceCheckRemote)
{
	AF_NOREENTRANT // macro helper

	// Don't distract the user unnecessarily
	const CPreferencesDlg& userPrefs = Prefs();
	RELOAD_OPTION nReloadOption = userPrefs.GetReadonlyReloadOption();

	if (!WantCheckReloadFiles(nReloadOption))
		return;
			
	// work out whether we should check remote files or not
	BOOL bCheckRemoteFiles = bForceCheckRemote;

	if (!bCheckRemoteFiles)
	{
		static int nElapsed = 0;
		bCheckRemoteFiles = WantCheckRemoteFiles(nCtrl, INTERVAL_READONLYSTATUS, nElapsed);
	}
	
	// process files
	CString sFileList;
	int nFrom = ((nCtrl == -1) ? 0 : nCtrl);
	int nTo = ((nCtrl == -1) ? (GetTDCCount() - 1) : nCtrl);
	
	for (nCtrl = nFrom; nCtrl <= nTo; nCtrl++)
	{
		// don't check delay-loaded tasklists
		if (!m_mgrToDoCtrls.IsLoaded(nCtrl))
			continue;
		
		// don't check removable drives
		int nType = m_mgrToDoCtrls.GetFilePathType(nCtrl);
		
        if ((nType == TDCM_UNDEF) || (nType == TDCM_REMOVABLE))
			continue;
		
		// check remote files?
		if (!bCheckRemoteFiles && (nType == TDCM_REMOTE))
			continue;
				
		if (m_mgrToDoCtrls.RefreshReadOnlyStatus(nCtrl))
		{
			CFilteredToDoCtrl& tdc = GetToDoCtrl(nCtrl);
		
			BOOL bReadOnly = m_mgrToDoCtrls.GetReadOnlyStatus(nCtrl);
			BOOL bReload = !bReadOnly; // now writable
			
			if (nReloadOption == RO_ASK)
			{
				CString sFilePath = tdc.GetFilePath();
				CEnString sMessage(bReadOnly ? IDS_WRITABLETOREADONLY : IDS_READONLYTOWRITABLE, sFilePath);
				
				if (!bReadOnly) // might been modified
					sMessage += CEnString(IDS_WANTRELOAD);

				UINT nRet = CMessageBox::AfxShow(IDS_STATUSCHANGE_TITLE, sMessage, !bReadOnly ? MB_YESNOCANCEL : MB_OK);
				
				bReload = (nRet == IDYES || nRet == IDOK);
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

BOOL CToDoListWnd::WantCheckReloadFiles(RELOAD_OPTION nOption) const
{
	switch (nOption)
	{
	case RO_NO:	
		return FALSE;

	case RO_ASK:
		return (IsWindowVisible() && !IsIconic());

	case RO_AUTO:   // Means reload
	case RO_NOTIFY: // Means reload and notify
		return TRUE;
	}

	ASSERT(0);
	return FALSE;
}

void CToDoListWnd::OnTimerTimestampChange(int nCtrl, BOOL bForceCheckRemote)
{
	AF_NOREENTRANT // macro helper
		
	// Don't distract the user unnecessarily
	const CPreferencesDlg& userPrefs = Prefs();
	RELOAD_OPTION nReloadOption = userPrefs.GetTimestampReloadOption();

	if (!WantCheckReloadFiles(nReloadOption))
		return;

	// work out whether we should check remote files or not
	BOOL bCheckRemoteFiles = bForceCheckRemote;

	if (!bCheckRemoteFiles)
	{
		static int nElapsed = 0;
		bCheckRemoteFiles = WantCheckRemoteFiles(nCtrl, INTERVAL_TIMESTAMPCHANGE, nElapsed);
	}
	
	// process files
	CString sFileList;
	int nFrom = ((nCtrl == -1) ? 0 : nCtrl);
	int nTo = ((nCtrl == -1) ? (GetTDCCount() - 1) : nCtrl);
	
	for (nCtrl = nFrom; nCtrl <= nTo; nCtrl++)
	{
		// don't check delay-loaded tasklists
		if (!m_mgrToDoCtrls.IsLoaded(nCtrl))
			continue;

		// don't check removable drives
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
				
				bReload = (CMessageBox::AfxShow(IDS_TIMESTAMPCHANGE_TITLE, sMessage, MB_YESNOCANCEL) == IDYES);
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
		
	// don't flush because that can mess with focus
	SaveAll(TDLS_AUTOSAVE | TDLS_NOFLUSH);
}

void CToDoListWnd::OnTimerAutoMinimize()
{
	AF_NOREENTRANT // macro helper

	if (!IsWindowVisible() || IsIconic())
		return;

	// check whether we should hide ourselves
	double dElapsed = (Misc::GetTicksSinceLastUserInput() / ONE_MINUTE);
		
	if (dElapsed > Prefs().GetAutoMinimizeFrequency())
		ShowWindow(SW_MINIMIZE);
}

BOOL CToDoListWnd::WantCheckRemoteFiles(int nCtrl, int nInterval, int& nElapsed) const
{
	// work out whether we should check remote files or not
	BOOL bCheckRemoteFiles = (nCtrl != -1);
	
	if (!bCheckRemoteFiles)
	{
		UINT nRemoteFileCheckInterval = Prefs().GetRemoteFileCheckFrequency() * 1000; // in ms
		
		nElapsed %= nRemoteFileCheckInterval;
		bCheckRemoteFiles = !nElapsed;
		
		nElapsed += nInterval;
	}

	return bCheckRemoteFiles;
}

void CToDoListWnd::OnTimerCheckoutStatus(int nCtrl, BOOL bForceCheckRemote)
{
	if (!m_mgrToDoCtrls.AnyIsSourceControlled())
		return;

	AF_NOREENTRANT // macro helper
		
	// work out whether we should check remote files or not
	BOOL bCheckRemoteFiles = bForceCheckRemote;
	
	if (!bCheckRemoteFiles)
	{
		static int nElapsed = 0;
		bCheckRemoteFiles = WantCheckRemoteFiles(nCtrl, INTERVAL_CHECKOUTSTATUS, nElapsed);
	}
	
	// process files
	const CPreferencesDlg& userPrefs = Prefs();
	
	int nFrom = ((nCtrl == -1) ? 0 : nCtrl);
	int nTo = ((nCtrl == -1) ? (GetTDCCount() - 1) : nCtrl);

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
			if (!m_mgrToDoCtrls.GetLastCheckoutSucceeded(nCtrl) && 
				(m_mgrToDoCtrls.CheckOut(nCtrl) == TDCF_SUCCESS))
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
			
			if (nMin >= userPrefs.GetAutoCheckinFrequency())
			{
				StopTimeTrackingTask(nCtrl, FROM_APP);

				if (m_mgrToDoCtrls.CheckIn(nCtrl) == TDCF_SUCCESS)
				{
					// notify the user
					sCheckedInFiles += m_mgrToDoCtrls.GetFriendlyProjectName(nCtrl);
					sCheckedInFiles += ENDL;
				}
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

	UINT nCmdID = pNMHDR->idFrom;

	if (pNMHDR->hwndFrom == m_tabCtrl.GetToolTips()->GetSafeHwnd())
	{
		sTipText = m_mgrToDoCtrls.GetTabItemTooltip((int)nCmdID);
	}
	else
	{
		// Handle items having dynamic menu text
		sTipText = m_menubar.GetDynamicItemTooltip(nCmdID,
												   m_mruList,
												   m_mgrToDoCtrls,
												   Prefs(),
												   m_filterBar,
												   m_mgrStorage,
												   m_mgrUIExtensions);
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
	if (m_bShowingMainToolbar && (pCmdUI->m_pMenu == NULL)) 
	{
		ASSERT(CTDCToolsHelper::IsToolCmdID(pCmdUI->m_nID));
		int nTool = (pCmdUI->m_nID - ID_TOOLS_USERTOOL1);

		USERTOOL tool;
		pCmdUI->Enable(Prefs().GetUserTool(nTool, tool));
	}
}

void CToDoListWnd::OnUserTool(UINT nCmdID) 
{
	ASSERT(CTDCToolsHelper::IsToolCmdID(nCmdID));
	int nTool = (nCmdID - ID_TOOLS_USERTOOL1);

	const CPreferencesDlg& prefs = Prefs();
	USERTOOL tool;
	
	if (prefs.GetUserTool(nTool, tool))
	{
		// Save all tasklists before executing the user tool
		if (prefs.GetAutoSaveOnRunTools())
		{
			if (SaveAll(TDLS_AUTOSAVE) == TDCF_CANCELLED)
				return;
		}

		CTDCToolsHelper th(prefs.GetEnableTDLExtension(), 
						   prefs.GetDisplayDatesInISO());
		USERTOOLARGS args;
		PopulateToolArgs(args);

		// If the tool points to 'us' we try to avoid running another instance
		if (FileMisc::IsSamePath(FileMisc::GetAppFilePath(), th.GetToolPath(tool)))
		{
			CString sCmdLine;
			
			if (th.PrepareCmdline(tool, args, GetToDoCtrl().GetCustomAttributeDefs(), sCmdLine))
			{
				CTDCStartupOptions startup(sCmdLine);

				// We can handle it if there is no file path (apply to active tasklist)
				// OR We have the tasklist (delay) loaded
				if (!startup.HasFilePath() || HasTaskFile(startup))
				{
					VERIFY(ProcessStartupOptions(startup, FALSE));
				}
			}

			// else user cancelled
			return;
		}

		// All else
		th.RunTool(tool, args, GetToDoCtrl().GetCustomAttributeDefs());
	}
}

void CToDoListWnd::OnViewActivateFilter(UINT nCmdID)
{
	int nFilter = (nCmdID - ID_VIEW_ACTIVATEFILTER1);

	if ((nFilter < 0) || (nFilter >= 24))
	{
		ASSERT(0);
		return;
	}

	VERIFY(m_filterBar.SelectFilter(nFilter));
}

void CToDoListWnd::OnViewActivateAdvancedFilter(UINT nCmdID)
{
	int nCustomFilter = (nCmdID - ID_VIEW_ACTIVATEADVANCEDFILTER1);

	if ((nCustomFilter < 0) || (nCustomFilter >= 24))
	{
		ASSERT(0);
		return;
	}

	int nNumDefaultFilters = (Prefs().GetShowDefaultFiltersInFilterBar() ? NUM_SHOWFILTER : 1);

	VERIFY(m_filterBar.SelectFilter(nNumDefaultFilters + nCustomFilter));
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
			
			if (tdc.GetVisibleTaskViews(aTypeIDs))
			{
				CUIExtensionHelper helper(ID_SHOWVIEW_UIEXTENSION1, 16);

				if (helper.ProcessExtensionVisibilityMenuCmd(nCmdID, m_mgrUIExtensions, aTypeIDs))
					tdc.SetVisibleTaskViews(aTypeIDs);
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
		pCmdUI->SetCheck(GetToDoCtrl().IsListViewTabShowing() ? 1 : 0);
		break;

	case ID_SHOWVIEW_UIEXTENSION1:
		// handled in OnInitMenuPopup
		break;
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
		UpdateStatusBar();
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
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	// Use the existing tasklist's file name
	CString sTDCFile = FileMisc::GetFileNameFromPath(tdc.GetFilePath(), FALSE);
	CString sTDCExt = FileMisc::GetExtension(tdc.GetFilePath(), FALSE);

	if (sTDCFile.IsEmpty())
	{
		sTDCFile = CEnString(IDS_TDC_UNTITLEDFILE);
		Misc::Trim(sTDCFile);

		sTDCExt = TDL_EXT;
	}

	CString sTempPath = FileMisc::GetTempFilePath(sTDCFile, sTDCExt);
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
	Resize();
	UpdateWindow();
}

void CToDoListWnd::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	if (!bSysMenu)
	{
		// On-demand initialisation
		PopulateMenuIconManager();

		m_menubar.HandleInitMenuPopup(pPopupMenu,
									  GetToDoCtrl(),
									  Prefs(),
									  m_filterBar,
									  m_mgrStorage,
									  m_mgrUIExtensions,
									  m_mgrMenuIcons);
	}

	CFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
}

LRESULT CToDoListWnd::OnPostTranslateMenu(WPARAM /*wp*/, LPARAM lp)
{
	return m_menubar.HandlePostTranslateMenu((HMENU)lp);
}

void CToDoListWnd::OnViewCustomToolbar() 
{
	m_bShowingCustomToolbar = !m_bShowingCustomToolbar;

	if (m_bShowingCustomToolbar && !m_toolbarCustom.GetSafeHwnd())
		InitCustomToolbar();

	if (Prefs().GetDisplayUDTsInToolbar())
		UpdateUDTsInToolbar(UDT_CUSTOMTOOLBAR);
	
	m_toolbarCustom.ShowWindow(m_bShowingCustomToolbar ? SW_SHOW : SW_HIDE);
	m_toolbarCustom.EnableWindow(m_bShowingCustomToolbar);

	Resize();
	Invalidate(TRUE);
}

void CToDoListWnd::OnViewMainToolbar() 
{
	m_bShowingMainToolbar = !m_bShowingMainToolbar;

	if (Prefs().GetDisplayUDTsInToolbar())
		UpdateUDTsInToolbar(UDT_MAINTOOLBAR);
	
	m_toolbarMain.ShowWindow(m_bShowingMainToolbar ? SW_SHOW : SW_HIDE);
	m_toolbarMain.EnableWindow(m_bShowingMainToolbar);
	
	Resize();
	Invalidate(TRUE);
}

void CToDoListWnd::OnUpdateViewMainToolbar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bShowingMainToolbar ? 1 : 0);
}

void CToDoListWnd::OnUpdateViewCustomToolbar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bShowingCustomToolbar ? 1 : 0);
	pCmdUI->Enable(Prefs().HasCustomToolbar());
}

void CToDoListWnd::UpdateUDTsInToolbar(UDTCHANGETYPE nChange)
{
	const CPreferencesDlg& prefs = Prefs();

	BOOL bRemoveFromMainToolbar = FALSE, bRemoveFromCustomToolbar = FALSE;
	BOOL bAddToMainToolbar = FALSE, bAddToCustomToolbar = FALSE;

	switch (nChange)
	{
	case UDT_INIT:
		if (m_bShowingMainToolbar)
		{
			bAddToMainToolbar = TRUE;
		}
		else // main toolbar hidden
		{
			bAddToCustomToolbar = m_bShowingCustomToolbar;
		}
		break;

	case UDT_MAINTOOLBAR:
		if (m_bShowingMainToolbar)
		{
			bAddToMainToolbar = TRUE;

			// If custom toolbar was previously showing then
			// it will have had the UDTs so we must remove them
			bRemoveFromCustomToolbar = m_bShowingCustomToolbar;
		}
		else // hide main toolbar
		{
			bAddToCustomToolbar = m_bShowingCustomToolbar;

			// Main toolbar previously had the UDTs so we must remove them
			bRemoveFromMainToolbar = TRUE;
		}
		break;

	case UDT_CUSTOMTOOLBAR:
		if (m_bShowingMainToolbar)
		{
			// If main toolbar was already visible then
			// it will already be showing the UDTs so nothing to be done
		}
		else if (m_bShowingCustomToolbar)
		{
			// Custom toolbar was previously hidden so we just need to add
			bAddToCustomToolbar = TRUE;
		}
		else // hide custom toolbar
		{
			// Custom toolbar was previously visible so we just need to remove
			bRemoveFromCustomToolbar = TRUE;
		}
		break;

	case UDT_PREFERENCES:
		if (m_bShowingMainToolbar)
		{
			bAddToMainToolbar = TRUE;

			// Main toolbar previously had the UDTs so we must remove them
			bRemoveFromMainToolbar = TRUE;
		}
		else // main toolbar hidden
		{
			bAddToCustomToolbar = m_bShowingCustomToolbar;

			// Custom toolbar previously had the UDTs so we must remove them
			bRemoveFromCustomToolbar = m_bShowingCustomToolbar;
		}
		break;
	}

	BOOL bWantInToolbar = prefs.GetDisplayUDTsInToolbar();
	CTDCToolsHelper th(FALSE, FALSE);

	if (!bWantInToolbar || bRemoveFromMainToolbar)
		th.RemoveToolsFromToolbar(m_toolbarMain, ID_PREFERENCES);

	if (!bWantInToolbar || bRemoveFromCustomToolbar)
		m_toolbarCustom.RemoveTools();

	if (bWantInToolbar && (bAddToMainToolbar || bAddToCustomToolbar))
	{
		CUserToolArray aTools;
		prefs.GetUserTools(aTools);

		if (bAddToMainToolbar)
		{
			th.AddToolsToToolbar(aTools, m_toolbarMain, ID_PREFERENCES);

			// refresh tooltips
			m_tbHelperMain.Release();
			m_tbHelperMain.Initialize(&m_toolbarMain, this, &m_mgrShortcuts);
		}
		else
		{
			m_toolbarCustom.AppendTools(aTools);
		}
	}

	// resize toolbar to accept the additional buttons
	if (bRemoveFromMainToolbar || bRemoveFromCustomToolbar || bAddToMainToolbar || bAddToCustomToolbar)
		Resize();
}

void CToDoListWnd::OnSort() 
{
	GetToDoCtrl().Resort(TRUE);
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
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
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
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	pCmdUI->Enable(!tdc.IsReadOnly() && !tdc.IsArchive() && tdc.GetSelectedTaskCount());
}

void CToDoListWnd::DoMoveTask(TDC_MOVETASK nDirection)
{
	// DEBUGGING /////////////////////////////////////////////////////////
// 	static DWORD dwLastTick = GetTickCount();
// 	DWORD dwTick = GetTickCount();
// 
// #ifdef _DEBUG
// 	TRACE(_T("Time since last CToDoCtrl::MoveSelectedTask() = %ld ms\n"), dwTick - dwLastTick);
// #else
// 	FileMisc::LogTextRaw(Misc::Format(_T("Time since last CToDoCtrl::MoveSelectedTask() = %ld ms\n"), dwTick - dwLastTick));
// #endif
// 
// 	dwLastTick = GetTickCount();
// 
// 	CScopedLogTimer log(_T("CToDoCtrl::MoveSelectedTask()"));
	//////////////////////////////////////////////////////////////////////

	GetToDoCtrl().MoveSelectedTask(nDirection);
}

void CToDoListWnd::OnMovetaskdown()
{
	DoMoveTask(TDCM_DOWN);
}

void CToDoListWnd::OnUpdateMovetaskdown(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetToDoCtrl().CanMoveSelectedTask(TDCM_DOWN));
}

void CToDoListWnd::OnMovetaskup() 
{
	DoMoveTask(TDCM_UP);	
}

void CToDoListWnd::OnUpdateMovetaskup(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanMoveSelectedTask(TDCM_UP));	
}

void CToDoListWnd::OnMovetaskright() 
{
	DoMoveTask(TDCM_RIGHT);
}

void CToDoListWnd::OnUpdateMovetaskright(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanMoveSelectedTask(TDCM_RIGHT));	
}

void CToDoListWnd::OnMovetaskleft() 
{
	DoMoveTask(TDCM_LEFT);
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

CFilteredToDoCtrl* CToDoListWnd::NewToDoCtrl(BOOL bEnabled)
{
	BOOL bWantHoldRedraw = ((m_bVisible > 0) && !IsIconic());
	CHoldRedraw hr(bWantHoldRedraw ? GetSafeHwnd() : NULL);
	
	TDCCOLEDITFILTERVISIBILITY vis;

	Prefs().GetDefaultColumnEditFilterVisibility(vis);
	
	CFilteredToDoCtrl* pTDC = new CFilteredToDoCtrl(m_mgrUIExtensions, 
													m_mgrContent, 
													m_mgrShortcuts,
													m_tdiDefault.cfComments, 
													vis);
	// Give it a meaningful maximum size
	CPreferences prefs;
	CRect rCtrl;
	BOOL bMaximized = IsZoomed();

	if (m_bReloading)
	{
		int nDefShowState = AfxGetApp()->m_nCmdShow;
		bMaximized = ((nDefShowState == SW_SHOWMAXIMIZED) || prefs.GetProfileInt(_T("Pos"), _T("Maximized"), FALSE));

		rCtrl.left = prefs.GetProfileInt(_T("Pos"), _T("Left"), -1);
		rCtrl.top = prefs.GetProfileInt(_T("Pos"), _T("Top"), -1);
		rCtrl.right = prefs.GetProfileInt(_T("Pos"), _T("Right"), -1);
		rCtrl.bottom = prefs.GetProfileInt(_T("Pos"), _T("Bottom"), -1);

		if (bMaximized)
			GraphicsMisc::GetAvailableScreenSpace(CRect(rCtrl), rCtrl);
	}
	else
	{
		GetClientRect(rCtrl);
	}

	if (rCtrl.IsRectEmpty() || !CalcToDoCtrlRect(rCtrl, rCtrl.Width(), rCtrl.Height(), bMaximized))
		VERIFY(GraphicsMisc::GetPrimaryMonitorScreenSpace(rCtrl));

	// and somewhere out in space
	rCtrl.OffsetRect(-30000, -30000);
	
	if (pTDC && pTDC->Create(rCtrl, this, IDC_TODOLIST, FALSE, bEnabled))
	{
		// set font to our font
		CDialogHelper::SetFont(pTDC, m_fontMain, FALSE);
		
		// set global styles once only allowing the taskfile 
		// itself to override from this point on
		InitGlobalStyles(*pTDC);

		// Set initial theme before it becomes visible
		pTDC->SetUITheme(m_theme);
		
		// rest of runtime preferences
		UpdateToDoCtrlPreferences(pTDC);

		if (GetTDCCount() == 0)
		{
			// Extensions are 'lazy' loaded so this is the first chance
			// to allow them to load global preferences
			m_mgrUIExtensions.LoadPreferences(prefs, _T("UIExtensions"));
			m_mgrContent.LoadPreferences(prefs, _T("ContentControls"), FALSE);
		}
		
		return pTDC;
	}
	
	// else
	delete pTDC;
	return NULL;
}

void CToDoListWnd::InitGlobalStyles(CFilteredToDoCtrl& tdc)
{
	CTDCStyleMap styles;

	if (m_bSaveUIVisInTaskList)
		styles[TDCS_SAVEUIVISINTASKLIST] = TRUE;

	if (!m_bPasswordPrompting)
		styles[TDCS_DISABLEPASSWORDPROMPTING] = TRUE;

	tdc.ModifyStyles(styles);
}

void CToDoListWnd::OnTabCtrlGetBackColor(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTABCTRLEX* pNMTCE = (NMTABCTRLEX*)pNMHDR;
	*pResult = 0;

	COLORREF crTab = m_mgrToDoCtrls.GetTabColor(pNMTCE->iTab);

	if (crTab != CLR_NONE)
	{
		if (crTab == 0)
			*pResult = 1;
		else
			*pResult = crTab;
	}
} 

void CToDoListWnd::OnTabCtrlPostDrawTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTABCTRLEX* pNMTCE = (NMTABCTRLEX*)pNMHDR;
	DRAWITEMSTRUCT* pDIS = (DRAWITEMSTRUCT*)pNMTCE->dwExtra;

	TDCM_DUESTATUS nStatus = m_mgrToDoCtrls.GetDueItemStatus(pDIS->itemID);

	if (nStatus == TDCM_PAST || nStatus == TDCM_TODAY)
	{
		// determine appropriate due colour
		COLORREF crDue, crDueToday;
		GetToDoCtrl(pDIS->itemID).GetDueTaskColors(crDue, crDueToday);

		COLORREF crTag = (nStatus == TDCM_PAST) ? crDue : crDueToday;

		if (crTag != CLR_NONE)
		{
			// draw a little tag in the top left corner
			CRect rect;
			m_tabCtrl.GetTabContentRect(&pDIS->rcItem, pNMTCE->iTab, rect);

			CDC* pDC = CDC::FromHandle(pDIS->hDC);

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

	*pResult = 0;
} 

void CToDoListWnd::OnTabCtrlCloseTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMTABCTRLEX* pNMTCE = (NMTABCTRLEX*)pNMHDR;

	if (pNMTCE->iTab >= 0)
		CheckCloseTasklist(pNMTCE->iTab);

	*pResult = 0;
}

void CToDoListWnd::OnTabCtrlEndDrag(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTABCTRLEX* pNMTCE = (NMTABCTRLEX*)pNMHDR;
	
	// check valid tab indices
	ASSERT((pNMTCE->iTab != -1) && (pNMTCE->dwExtra != 0));

	if ((pNMTCE->iTab != -1) && (pNMTCE->dwExtra != 0))
	{
		m_mgrToDoCtrls.MoveToDoCtrl(pNMTCE->iTab, pNMTCE->dwExtra);
		
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
	///////////////////////////////////////////////////////////////////////
	// PERMANENT LOGGING
	FileMisc::LogText(_T("CToDoListWnd::OnTabCtrlSelchanging(start)"));
	///////////////////////////////////////////////////////////////////////

	// cache the outgoing selection
	m_nLastSelItem = GetSelToDoCtrl();
	
	// and flush
	if (m_nLastSelItem != -1)
	{
		CFilteredToDoCtrl& tdc = GetToDoCtrl(m_nLastSelItem);
		tdc.Flush();

		// and save
		if (Prefs().GetAutoSaveOnSwitchTasklist() && tdc.HasFilePath() && tdc.IsModified())
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
	if ((m_nLastSelItem != -1) && !VerifyToDoCtrlPassword())
	{
		m_tabCtrl.SetCurSel(m_nLastSelItem);
		return;
	}

	int nDueBy = Prefs().GetNotifyDueByOnSwitch();
	
	if (nCurSel != -1)
	{
		// make sure it's loaded and notify due tasks
		// only if the user has not configured due task
		// notifications on switching
		if (!VerifyTaskListOpen(nCurSel, (nDueBy == -1)))
		{
			// restore the previous tab
			m_tabCtrl.SetCurSel(m_nLastSelItem);
			return;
		}

		CFilteredToDoCtrl& tdcShow = GetToDoCtrl(nCurSel);
		UpdateToDoCtrlPreferences(&tdcShow);

		// update the filter selection
 		RefreshFilterBarControls(TDCA_ALL);
 		
 		UpdateStatusBar();
		UpdateCaption();

		if (Prefs().GetShareCommentsSize() && (m_nLastSelItem != -1))
		{
			int nCommentsSize = GetToDoCtrl(m_nLastSelItem).GetCommentsSize();
			tdcShow.SetCommentsSize(nCommentsSize);
		}

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
		UpdateWindow();

		CFilteredToDoCtrl& tdcShow = GetToDoCtrl(nCurSel);
		tdcShow.UpdateWindow();

		// update find dialog with this ToDoCtrl's custom attributes
		UpdateFindDialogActiveTasklist(&tdcShow);
		RefreshFindTasksListData(TDCA_ALL);

		// leave focus setting till last else the 'old' tasklist flashes
		tdcShow.SetFocusToTasks();

		// check for external changes to file
		OnTimerCheckReloadTasklists(nCurSel, TRUE);

		// notify user of due tasks if req
		DoDueTaskNotification(nCurSel, nDueBy);

		UpdateAeroFeatures();
	}

	UpdateMenuIconMgrSourceControlStatus();
	RefreshPauseTimeTracking();

	///////////////////////////////////////////////////////////////////////
	// PERMANENT LOGGING
	FileMisc::LogText(_T("CToDoListWnd::OnTabCtrlSelchange(end)"));
	///////////////////////////////////////////////////////////////////////
	
	*pResult = 0;
}

void CToDoListWnd::OnTimerCheckReloadTasklists(int nCtrl, BOOL bForceCheckRemote)
{
	OnTimerTimestampChange(nCtrl, bForceCheckRemote);
	OnTimerReadOnlyStatus(nCtrl, bForceCheckRemote);
	OnTimerCheckoutStatus(nCtrl, bForceCheckRemote);
}

void CToDoListWnd::UpdateMenuIconMgrSourceControlStatus()
{
	if (!m_mgrMenuIcons.HasImages())
		return;

	// figure out current state
	BOOL bWasDisabled = m_mgrMenuIcons.HasImageID(ID_TOOLS_TOGGLECHECKIN);
	BOOL bWasCheckedOut = (!bWasDisabled && m_mgrMenuIcons.HasImageID(ID_TOOLS_CHECKIN));

	// figure out new state
	BOOL bIsDisabled = TRUE;
	BOOL bIsCheckedOut = FALSE;

	if (GetTDCCount())
	{
		const CFilteredToDoCtrl& tdc = GetToDoCtrl();

		if (tdc.IsSourceControlled())
		{
			bIsDisabled = FALSE;
			bIsCheckedOut = tdc.IsCheckedOut();
		}
	}

	if (bIsDisabled)
	{
		if (bWasDisabled)
		{
			return;
		}
		else if (bWasCheckedOut)
		{
			m_mgrMenuIcons.ChangeImageID(ID_TOOLS_CHECKIN, ID_TOOLS_TOGGLECHECKIN);
		}
		else // checked in
		{
			m_mgrMenuIcons.ChangeImageID(ID_TOOLS_CHECKOUT, ID_TOOLS_TOGGLECHECKIN);
		}
	}
	else if (bIsCheckedOut)
	{
		if (bWasDisabled)
		{
			m_mgrMenuIcons.ChangeImageID(ID_TOOLS_TOGGLECHECKIN, ID_TOOLS_CHECKIN);
		}
		else if (bWasCheckedOut)
		{
			return;
		}
		else // checked in
		{
			m_mgrMenuIcons.ChangeImageID(ID_TOOLS_CHECKOUT, ID_TOOLS_CHECKIN);
		}
	}
	else // new == checked in
	{
		if (bWasDisabled)
		{
			m_mgrMenuIcons.ChangeImageID(ID_TOOLS_TOGGLECHECKIN, ID_TOOLS_CHECKOUT);
		}
		else if (bWasCheckedOut)
		{
			m_mgrMenuIcons.ChangeImageID(ID_TOOLS_CHECKIN, ID_TOOLS_CHECKOUT);
		}
		else // checkedin
		{
			return;
		}
	}
}

void CToDoListWnd::UpdateFilterBarListData(TDC_ATTRIBUTE nAttribID)
{
	if (m_bShowFilterBar)
		m_filterBar.UpdateDropListData(GetToDoCtrl(), nAttribID);
}

void CToDoListWnd::RefreshFilterBarControls(TDC_ATTRIBUTE nAttribID, BOOL bClearCheckboxHistory)
{
	if (m_bShowFilterBar)
	{
		if (nAttribID == TDCA_ALL)
		{
			RefreshFilterBarAdvancedFilters();

			m_filterBar.SetTitleFilterOption(Prefs().GetTitleFilterOption());
			m_filterBar.RefreshFilterControls(GetToDoCtrl(), nAttribID);

			if (bClearCheckboxHistory)
				m_filterBar.ClearCheckboxHistory();
		}
		else
		{
			m_filterBar.RefreshFilterControls(GetToDoCtrl(), nAttribID);
		}

		CheckResizeFilterBar();
	}
}

void CToDoListWnd::CheckResizeFilterBar()
{
	// Determine if a resize if required
	CRect rFilter, rClient;

	GetFilterBarRect(rFilter);
	GetClientRect(rClient);

	int nReqHeight = m_filterBar.CalcHeight(rClient.Width());

	// GetFilterBarRect() adds 1 pixel to the top
	if (rFilter.Height() != (nReqHeight + 1))
		Resize();

	// Always redraw
	rFilter.bottom = (rFilter.top + nReqHeight);
	InvalidateRect(rFilter, TRUE);
}

void CToDoListWnd::RefreshFindTasksListData(TDC_ATTRIBUTE nAttribID)
{
	BOOL bRefreshAll = (nAttribID == TDCA_ALL);
	BOOL bCustAttrib = TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID);

	if (bRefreshAll || bCustAttrib)
	{
		UpdateFindDialogCustomAttributes(&GetToDoCtrl());
	}

	if (bRefreshAll || !bCustAttrib)
	{
		TDCAUTOLISTDATA tldActive, tldAll;

		m_mgrToDoCtrls.GetAutoListData(tldActive, tldAll, nAttribID);
		m_dlgFindTasks.SetAttributeListData(tldActive, tldAll, nAttribID);
	}
}

void CToDoListWnd::RemapAdvancedFilterMenuItemIDs(const CStringArray& aOldFilters, const CStringArray& aNewFilters)
{
	// A side-effect of the current alphabetic sorting of advanced
	// filters is that any custom toolbar buttons or keyboard shortcuts
	// which reference advanced filter menu items are at risk of being 
	// invalidated by the insertion or deletion of filters earlier in 
	// the sort order
	CMap<UINT, UINT, UINT, UINT&> mapMenuIDs;

	if (!CTDCFilter::BuildAdvancedFilterMenuItemMapping(aOldFilters, aNewFilters, mapMenuIDs))
		return;

	if (m_pPrefs->RemapMenuItemIDs(mapMenuIDs) & PREFS_REMAPPEDTOOLBAR)
	{
		// Remove 'old' toolbar icons
		m_mgrMenuIcons.RemoveImages(m_toolbarCustom);
		
		CToolbarButtonArray aTBButtons;
		VERIFY(Prefs().GetCustomToolbarButtons(aTBButtons));

		VERIFY(m_toolbarCustom.ModifyButtonAttributes(aTBButtons, m_menubar));

		// re-add modified toolbar icons
		m_mgrMenuIcons.AddImages(m_toolbarCustom);
	}
}

void CToDoListWnd::RefreshFilterBarAdvancedFilters()
{
	m_findFilterHelper.RefreshFilterBarAdvancedFilters();

	CRect rFilter;

	if (GetFilterBarRect(rFilter))
		InvalidateRect(rFilter, TRUE);
}

TDC_FILE CToDoListWnd::ConfirmSaveTaskList(int nIndex, DWORD dwFlags)
{
	BOOL bClosingWindows = Misc::HasFlag(dwFlags, TDLS_CLOSINGWINDOWS);
	BOOL bClosingTaskList = Misc::HasFlag(dwFlags, TDLS_CLOSINGTASKLISTS) || bClosingWindows; // sanity check
	BOOL bFlush = !Misc::HasFlag(dwFlags, TDLS_NOFLUSH);

	TDC_FILE nSave = TDCF_SUCCESS;
	
	// save changes
	CFilteredToDoCtrl& tdc = GetToDoCtrl(nIndex);
	
	if (bClosingTaskList && tdc.IsActivelyTimeTracking())
	{
		if (bClosingWindows)
			tdc.EndTimeTracking(FALSE);
		else
			StopTimeTrackingTask(nIndex, FROM_APP);
	}

	if (tdc.IsModified())
	{
		BOOL bFirstTimeSave = (!tdc.HasFilePath() && !m_mgrToDoCtrls.UsesStorage(nIndex));

		// if we are closing Windows, we don't bother asking
		// we just save and get out as fast as poss
		if (bClosingWindows)
		{
			// if it's a first time save we just save to a temp file
			if (bFirstTimeSave)
			{
				tdc.Save(GetEndSessionFilePath(), bFlush);
			}
			else
			{
				m_mgrToDoCtrls.DoBackup(nIndex);
				tdc.Save(_T(""), bFlush);
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
			int nRet = CMessageBox::AfxShow(IDS_CONFIRMSAVE_TITLE, sMessage, bClosingWindows ? MB_YESNO : MB_YESNOCANCEL);
			
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
			nSave = SaveTaskList(nIndex, NULL, dwFlags);
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

	if (ConfirmSaveTaskList(nIndex, TDLS_CLOSINGTASKLISTS) != TDCF_SUCCESS)
		return FALSE;
	
	// remove any references to this tasklist
	m_dlgFindTasks.DeleteResults(&tdc);
	m_dlgTimeTracker.RemoveTasklist(&tdc);
	m_dlgReminders.RemoveToDoCtrl(&tdc);
	
	CWaitCursor cursor;

	int nNewSel = m_mgrToDoCtrls.DeleteToDoCtrl(nIndex);
	
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
			CreateNewTaskList(FALSE, FALSE);
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
	CheckCloseTasklist(GetSelToDoCtrl());
}

void CToDoListWnd::CheckCloseTasklist(int nIndex)
{
	if (nIndex < 0 || nIndex >= GetTDCCount())
	{
		ASSERT(0);
		return;
	}

	// make sure there are no edits pending
	GetToDoCtrl(nIndex).Flush(); 
	
	// Don't close the tab if it's the only pristine tasklist
	if (m_mgrToDoCtrls.IsPristine())
		return;

	// Don't close the tab if it's pristine and there are
	// no other selectable tasklists
	if (m_mgrToDoCtrls.IsPristine(nIndex))
	{
		if (m_mgrToDoCtrls.GetNextMostSelectableToDoCtrl(nIndex) == -1)
			return;
	}
		
	CloseToDoCtrl(nIndex);
	
	// if empty then create a new dummy item		
	if (!GetTDCCount())
		CreateNewTaskList(FALSE, FALSE);
	else
		Resize();
}

void CToDoListWnd::CheckRemovePristineTasklist()
{
	ASSERT(GetTDCCount());

	if (m_mgrToDoCtrls.IsPristine())
		return;

	int nIndex = m_mgrToDoCtrls.FindPristineToDoCtrl();

	if ((nIndex == -1))
		return;

	// We can remove the pristine tasklist if there 
	// is another tasklist that can be selected
	int nNextSel = m_mgrToDoCtrls.GetNextMostSelectableToDoCtrl(nIndex);

	if (nNextSel == -1)
		return;

	CloseToDoCtrl(nIndex);
}

BOOL CToDoListWnd::SelectToDoCtrl(LPCTSTR szFilePath, BOOL bCheckPassword, int nNotifyDueTasksBy)
{
	int nCtrl = m_mgrToDoCtrls.FindToDoCtrl(szFilePath);
	
	if (nCtrl == -1)
		return FALSE;

	VERIFY(SelectToDoCtrl(nCtrl, bCheckPassword, nNotifyDueTasksBy));
	return TRUE;
}

int CToDoListWnd::GetSelToDoCtrl() const 
{ 
	if (m_tabCtrl.GetSafeHwnd()) 
		return m_tabCtrl.GetCurSel(); 
	else
		return -1;
}

BOOL CToDoListWnd::VerifyTaskListOpen(int nIndex, BOOL bWantNotifyDueTasks, BOOL bNotifyError)
{
	if (m_mgrToDoCtrls.IsLoaded(nIndex))
		return TRUE;

	if (!m_mgrToDoCtrls.HasFilePath(nIndex))
		return TRUE;

	// else
	CFilteredToDoCtrl& tdc = GetToDoCtrl(nIndex);
	CString sFilePath = tdc.GetFilePath();

	TSM_TASKLISTINFO storageInfo;

	if (m_mgrToDoCtrls.GetStorageDetails(nIndex, storageInfo))
		sFilePath = storageInfo.EncodeInfo(Prefs().GetSaveStoragePasswords());

	TDC_FILE nRes = OpenTaskList(&tdc, sFilePath, &storageInfo);

	if (nRes == TDCF_SUCCESS)
	{
		// make sure hidden windows stay hidden
		if (nIndex != GetSelToDoCtrl())
			tdc.ShowWindow(SW_HIDE);

		Resize();

		m_mgrToDoCtrls.CheckNotifyReadonly(nIndex);
		m_mgrToDoCtrls.SetStorageDetails(nIndex, storageInfo);
		m_mgrToDoCtrls.SetLoaded(nIndex);

		if (bWantNotifyDueTasks)
			DoDueTaskNotification(nIndex, Prefs().GetNotifyDueByOnLoad());

		return TRUE;
	}
	else if (bNotifyError)
	{
		HandleLoadTasklistError(nRes, sFilePath);
	}

	// else
	return FALSE;
}

BOOL CToDoListWnd::SelectToDoCtrl(int nIndex, BOOL bCheckPassword, int nNotifyDueTasksBy, BOOL bNotifyError)
{
	ASSERT (nIndex >= 0);
	ASSERT (nIndex < GetTDCCount());
	
	// load and show the chosen item
	if (!m_bClosing)
	{
		// if the tasklist is not loaded and we verify its loading
		// then we know that the password (if there is one) has been 
		// verified and doesn't need checking again
		if (!m_mgrToDoCtrls.IsLoaded(nIndex))
		{
			// We only need to do a 'open' due task notification 
			// if the caller has NOT otherwise requested a notification
			// else we do it at the end
			BOOL bWantNotifyDueTasks = (nNotifyDueTasksBy == -1);

			if (!VerifyTaskListOpen(nIndex, bWantNotifyDueTasks, bNotifyError))
				return FALSE;

			bCheckPassword = FALSE;
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
		CheckUpdateActiveToDoCtrlPreferences();
	
	const CPreferencesDlg& userPrefs = Prefs();

	// always come out of 'Maximise Comments'
	if (m_nMaxState == TDCMS_MAXCOMMENTS)
		m_nMaxState = TDCMS_NORMAL;
	
	tdcShow.SetMaximizeState(m_nMaxState);
	tdcShow.EnableWindow(TRUE);
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
	
	tdcShow.SetFocusToTasks();
	tdcShow.Invalidate(TRUE);
	tdcShow.UpdateWindow();
	
	if (!m_bClosing)
	{
		// Reload as required
		OnTimerCheckReloadTasklists(nIndex, TRUE);

		// update various dependencies
		UpdateCaption();
		UpdateStatusBar();
		UpdateMenuIconMgrSourceControlStatus();
		UpdateCwd();
		
		RefreshFilterBarControls(TDCA_ALL);
		RefreshFindTasksListData(TDCA_ALL);
		RefreshPauseTimeTracking();

		DoDueTaskNotification(GetSelToDoCtrl(), nNotifyDueTasksBy);
	}

	return TRUE;
}

void CToDoListWnd::UpdateAeroFeatures()
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();

#ifdef _DEBUG
	BOOL bEnable = !tdc.IsEncrypted();
#else
	BOOL bEnable = (!tdc.IsEncrypted() || !tdc.WantPasswordReprompting());
#endif

	// Disable peek and other dynamic views if the active tasklist is encrypted
	GraphicsMisc::EnableFlip3D(*this, bEnable);

	if (!GraphicsMisc::EnableAeroPeek(*this, bEnable))
		GraphicsMisc::ForceIconicRepresentation(*this, !bEnable);
}

void CToDoListWnd::CheckUpdateActiveToDoCtrlPreferences()
{
	// check if this has already been done since the last userPrefs change
	int nSel = GetSelToDoCtrl();
	
	if (m_mgrToDoCtrls.GetNeedsPreferenceUpdate(nSel))
	{
		CFilteredToDoCtrl& tdc = GetToDoCtrl(nSel);

		UpdateToDoCtrlPreferences(&tdc);

		// and filter bar relies on this tdc's visible columns
		RefreshFilterBarControls(TDCA_ALL);
	}
}

void CToDoListWnd::UpdateToDoCtrlPreferences(CFilteredToDoCtrl* pTDC)
{
	const CPreferencesDlg& userPrefs = Prefs();
	CFilteredToDoCtrl& tdc = *pTDC;

	CTDCToDoCtrlPreferenceHelper::UpdateToDoCtrl(tdc, userPrefs, m_tdiDefault, 
												m_bShowProjectName, m_bShowTreeListBar, 
												m_fontMain, m_fontTree, m_fontComments);

}

void CToDoListWnd::OnSaveall() 
{
	SaveAll(TDLS_INCLUDEUNSAVED);
}

void CToDoListWnd::OnUpdateSaveall(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_mgrToDoCtrls.AnyIsModified());
}

void CToDoListWnd::OnCloseall() 
{
	// save first
	TDC_FILE nSaveAll = SaveAll(TDLS_INCLUDEUNSAVED | TDLS_CLOSINGTASKLISTS);

	if (nSaveAll != TDCF_SUCCESS)
		return;

	// remove tasklists
	m_dlgFindTasks.DeleteAllResults();
	m_dlgTimeTracker.RemoveAllTasklists();

	int nCtrl = GetTDCCount();
	
	while (nCtrl--)
		m_mgrToDoCtrls.DeleteToDoCtrl(nCtrl);

	// if empty then create a new dummy tasklist		
	if (!GetTDCCount())
		CreateNewTaskList(FALSE, FALSE);
	else
		Resize();

	m_filterBar.Invalidate();
}

void CToDoListWnd::OnUpdateCloseall(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetTDCCount());
}

BOOL CToDoListWnd::OnQueryEndSession() 
{
	if (!CFrameWnd::OnQueryEndSession())
		return FALSE;
		
	// if Vista we register our reason and return TRUE to
	// get more time to clean up in WM_ENDSESSION
	HWND hWndThis = GetSafeHwnd();
	CEnString sReason(IDS_SHUTDOWNBLOCKREASON);
	
	if (Misc::ShutdownBlockReasonCreate(hWndThis, sReason))
		return TRUE;
	
	// else we're XP so we return TRUE to let shutdown continue
	return TRUE;
}

void CToDoListWnd::OnEndSession(BOOL bEnding) 
{
	if (bEnding)
	{
		///////////////////////////////////////////////////////////////////////
		// PERMANENT LOGGING
		CScopedLogTimer log(_T("CToDoListWnd::OnEndSession()"));
		///////////////////////////////////////////////////////////////////////
		
		m_bEndingSession = TRUE;
		DoExit(FALSE, TRUE);
	}
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
	DWORD dwSaveFlags = TDLS_INCLUDEUNSAVED | TDLS_CLOSINGTASKLISTS;

	if (bClosingWindows)
		dwSaveFlags |= TDLS_CLOSINGWINDOWS;

	TDC_FILE nSaveAll = SaveAll(dwSaveFlags);

	if (nSaveAll != TDCF_SUCCESS)
        return FALSE; // user cancelled

	// Hold a reference to the preferences so that
	// changes only get written once right at the end
	CPreferences hold;
	
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
		if (m_dlgFindTasks.GetSafeHwnd() && !m_dlgFindTasks.IsDocked())
			m_dlgFindTasks.ShowWindow(SW_HIDE);

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

		// wait here until all remaining export threads are done
		while (!m_wndExport.IsFinished())
		{
			Misc::ProcessMsgLoop();
			Sleep(50);
		}

		// Force components to save their state before we
		// signal Windows that it's alright to shutdown
		m_mgrImportExport.Release();
		m_tbHelperMain.Release();
		m_mgrShortcuts.Release();
		m_mgrImportExport.Release();
		m_mgrUIExtensions.Release();
		m_mgrStorage.Release();
			
		CFocusWatcher::Release();
		CMouseWheelMgr::Release();
		CEditShortcutMgr::Release();

		m_dlgTimeTracker.DestroyWindow();
		m_dlgReminders.DestroyWindow();
		m_filterBar.DestroyWindow();
		m_dlgFindTasks.DestroyWindow();
		m_toolbarMain.DestroyWindow();
		m_toolbarCustom.DestroyWindow();
			
		// Only need to destroy windows if NOT closing Windows
#ifdef _DEBUG
		DestroyWindow();
#else
		if (!bClosingWindows)
			DestroyWindow();
#endif

		if (hold.UsesIni())
			hold.Save();

		// cleanup the shutdown reason created in OnQueryEndSession.
		// This allows Windows to forcibly close the app hence no need
		// to call DestroyWindow
		if (bClosingWindows)
		{
			Misc::ShutdownBlockReasonDestroy(GetSafeHwnd());
		}
		else if (bRestart)
		{
			// By the time we get here 'this' has been destroyed
			// so we must NOT attempt to call any non-static functions
			CString sParams = AfxGetApp()->m_lpCmdLine;
			sParams += CEnCommandLineInfo::FormatSwitch(SWITCH_RESTART, Misc::Format(::GetCurrentProcessId()));
			
			if (FileMisc::Run(NULL, FileMisc::GetModuleFilePath(), sParams) < SE_ERR_SUCCESS)
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
	
	// cleanup our shutdown reason
	Misc::ShutdownBlockReasonDestroy(GetSafeHwnd());

	return FALSE;
}

LRESULT CToDoListWnd::OnToDoCtrlImportFromDrop(WPARAM wp, LPARAM lp)
{
	if (!OnToDoCtrlCanImportFromDrop(wp, lp))
	{
		ASSERT(0);
		return FALSE;
	}

	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	const TDCDROPIMPORT* pData = (const TDCDROPIMPORT*)lp;

	int nNumFiles = pData->aFiles.GetSize();

	if (nNumFiles)
	{
		CTDLImportDialog dialog(m_mgrImportExport, tdc.IsReadOnly(), tdc.HasSelection());

		dialog.SetImportTo(pData->dwTaskID ? TDIT_ADDTOSELECTEDTASK : TDIT_ADDTOBOTTOMOFTASKLIST);

		for (int nFile = 0; nFile < nNumFiles; nFile++)
		{
			dialog.SetUseFile(pData->aFiles[nFile]);

			if (dialog.DoModal() == IDOK)
			{
				// check file can be opened
				TDLID_IMPORTTO nImportTo = dialog.GetImportTo();
				int nImporter = m_mgrImportExport.FindImporterByType(dialog.GetFormatTypeID());

				ImportTasks(FALSE, dialog.GetImportFilePath(), nImporter, nImportTo);
			}
		}
	}
	else if (!pData->sText.IsEmpty())
	{
		CTDLImportDialog dialog(m_mgrImportExport, tdc.IsReadOnly(), tdc.HasSelection());

		dialog.SetUseText(pData->sText);
		dialog.SetImportTo(pData->dwTaskID ? TDIT_ADDTOSELECTEDTASK : TDIT_ADDTOBOTTOMOFTASKLIST);

		if (dialog.DoModal() == IDOK)
		{
			// check file can be opened
			TDLID_IMPORTTO nImportTo = dialog.GetImportTo();
			int nImporter = m_mgrImportExport.FindImporterByType(dialog.GetFormatTypeID());

			ImportTasks(TRUE, dialog.GetImportText(), nImporter, nImportTo);
		}
	}
	
	return 0L;
}

LRESULT CToDoListWnd::OnToDoCtrlCanImportFromDrop(WPARAM wp, LPARAM lp)
{
	if (!lp || (HWND(wp) != GetToDoCtrl().GetSafeHwnd()))
	{
		ASSERT(0);
		return FALSE;
	}

	const TDCDROPIMPORT* pData = (const TDCDROPIMPORT*)lp;
	int nNumFiles = pData->aFiles.GetSize();

	if (nNumFiles)
	{
		for (int nFile = 0; nFile < nNumFiles; nFile++)
		{
			if (m_mgrImportExport.FindImporterByPath(pData->aFiles[nFile]) != -1)
				return TRUE;
		}

		return FALSE;
	}

	return !pData->sText.IsEmpty();
}

void CToDoListWnd::OnImportTasklist() 
{
	BOOL bShowDlg = TRUE;
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	while (bShowDlg)
	{
		// Put the dialog instance inside the loop
		// to make sure it's reinitialised properly
		CTDLImportDialog dialog(m_mgrImportExport, tdc.IsReadOnly(), tdc.HasSelection());

		if (dialog.DoModal() == IDOK)
		{
			// check file can be opened
			TDLID_IMPORTTO nImportTo = dialog.GetImportTo();
			int nImporter = m_mgrImportExport.FindImporterByType(dialog.GetFormatTypeID());

			BOOL bFromText = dialog.GetImportFromText();
			CString sImportFrom = (bFromText ? dialog.GetImportText() : dialog.GetImportFilePath());

			// Reshow dialog on error
			bShowDlg = !ImportTasks(bFromText, sImportFrom, nImporter, nImportTo);
		}
		else
		{
			bShowDlg = FALSE; // cancelled
		}
	}
}

BOOL CToDoListWnd::ImportTasks(BOOL bFromText, const CString& sImportFrom,
								int nImporter, TDLID_IMPORTTO nImportTo) 
{
	CString sImportPath;

	if (bFromText)
	{
		sImportPath = TEMP_CLIPBOARD_FILEPATH;
		VERIFY(FileMisc::SaveFile(sImportPath, sImportFrom, SFEF_UTF16));
	}
	else 
	{
		if (sImportFrom.IsEmpty())
		{
			ASSERT(!m_mgrImportExport.ImporterHasFileExtension(nImporter));

			// Use path as placeholder for error handling
			sImportPath = m_mgrImportExport.GetImporterMenuText(nImporter);
		}
		else
		{
			sImportPath = sImportFrom;
		}
	}

	// If the importing into an existing tasklist
	// write its custom attributes to the tasklist
	CTaskFile tasks;

	if (nImportTo != TDIT_CREATENEWTASKLIST)
		tasks.SetCustomAttributeDefs(GetToDoCtrl().GetCustomAttributeDefs());
	
	// do the import (scoped else updating the status bar afterwards won't work)
	BOOL bSomeSuceeded = FALSE;
	{
		DOPROGRESS(IDS_IMPORTPROGRESS);

		IIMPORTEXPORT_RESULT nRes = m_mgrImportExport.ImportTaskList(sImportPath, tasks, nImporter, FALSE);

		if (tasks.GetTaskCount())
		{
			switch (nRes)
			{
			case IIER_SOMEFAILED:
			case IIER_SUCCESS:
				{
					bSomeSuceeded = TRUE;

					if (nImportTo == TDIT_CREATENEWTASKLIST)
					{
						// If the imported tasks contain any custom attributes
						// and those custom attributes match those of any open 
						// tasklist then overwrite the imported custom attributes 
						// with those from the matching tasklist
						CTDCCustomAttribDefinitionArray aImportedDefs, aTDCAttribDefs;

						if (tasks.GetCustomAttributeDefs(aImportedDefs))
						{
							int nTDC = GetTDCCount();

							while (nTDC--)
							{
								if (GetToDoCtrl(nTDC).GetCustomAttributeDefs(aTDCAttribDefs) &&
									aImportedDefs.MatchAny(aTDCAttribDefs))
								{
									aTDCAttribDefs.Append(aImportedDefs);

									tasks.SetCustomAttributeDefs(aTDCAttribDefs);
									break;
								}
							}
						}

						VERIFY(CreateNewTaskList(FALSE, TRUE));
					}

					CFilteredToDoCtrl& tdc = GetToDoCtrl(); // newly created tasklist
					TDC_INSERTWHERE nWhere = TDC_INSERTATTOP;
					BOOL bSelectAll = TRUE;

					if (nImportTo == TDIT_MERGETOTASKLISTBYID)
					{
						VERIFY(tdc.MergeTasks(tasks, TRUE));
					}
					else if (nImportTo == TDIT_MERGETOTASKLISTBYTITLE)
					{
						VERIFY(tdc.MergeTasks(tasks, FALSE));
					}
					else // Paste
					{
						switch (nImportTo)
						{
						case TDIT_ADDTOSELECTEDTASK:
							{
								if (Prefs().GetNewSubtaskPos() == PUIP_TOP)
									nWhere = TDC_INSERTATTOPOFSELTASK;
								else
									nWhere = TDC_INSERTATBOTTOMOFSELTASK;
							}
							break;

						case TDIT_ADDBELOWSELECTEDTASK:
							nWhere = TDC_INSERTAFTERSELTASK;
							break;

						case TDIT_ADDTOBOTTOMOFTASKLIST:
							nWhere = TDC_INSERTATBOTTOM;
							break;

						case TDIT_CREATENEWTASKLIST:
							bSelectAll = FALSE;
							tdc.SetProjectName(tasks.GetProjectName());
							break;

						case TDIT_ADDTOTOPOFTASKLIST:
							break;
						}

						VERIFY(tdc.PasteTasks(tasks, nWhere, bSelectAll));
					}

					UpdateCaption();
				}
			}
		}
	
		HandleImportTasklistError(nRes, sImportPath, bFromText, bSomeSuceeded);
	}

	UpdateStatusBar(TDCA_PASTE);

	return bSomeSuceeded;
}

void CToDoListWnd::HandleImportTasklistError(IIMPORTEXPORT_RESULT nErr, const CString& sImportPath, BOOL bFromClipboard, BOOL bAnyTasksSucceeded)
{
	UINT nIcon = 0, nMessageID = 0;

	switch (nErr)
	{
	case IIER_CANCELLED:
		break;

	case IIER_SOMEFAILED:
		if (bAnyTasksSucceeded)
		{
			if (bFromClipboard)
				nMessageID = IDS_IMPORTFROMCB_SOMETASKSFAILED;
			else
				nMessageID = IDS_IMPORTFILE_SOMETASKSFAILED;
			
			nIcon = MB_ICONWARNING;
		}
		else // All failed
		{
			if (bFromClipboard)
				nMessageID = IDS_IMPORTFROMCB_NOTASKS;
			else
				nMessageID = IDS_IMPORTFILE_NOTASKS;

			nIcon = MB_ICONERROR;
		}
		break;

	case IIER_SUCCESS:
		break;

	case IIER_BADFILE:
		if (!bFromClipboard)
		{
			nMessageID = IDS_IMPORTFILE_CANTOPEN;
			nIcon = MB_ICONERROR;
			break;
		}
		// else fall thru

	case IIER_BADFORMAT:
		if (bFromClipboard)
			nMessageID = IDS_IMPORTFROMCB_BADFORMAT;
		else
			nMessageID = IDS_IMPORTFILE_BADFORMAT;

		nIcon = MB_ICONERROR;
		break;

	case IIER_BADINTERFACE:
		// TODO
		break;
	}

	if (nMessageID)
		CMessageBox::AfxShow(CEnString(nMessageID, sImportPath), (MB_OK | nIcon));
}

void CToDoListWnd::HandleExportTasklistResult(IIMPORTEXPORT_RESULT nRes, const CString& sExportPath, BOOL bToClipboard, BOOL bPreview)
{
	// Post export operation
	switch (nRes)
	{
	case IIER_SOMEFAILED:
	case IIER_SUCCESS:
		if (bToClipboard)
		{
			CString sText;
			VERIFY(FileMisc::LoadFile(sExportPath, sText));

			// Hack to get ourselves in a static function
			CWnd* pWnd = AfxGetMainWnd();

			CClipboard(*pWnd).SetText(sText);
		}
		else if (bPreview)
		{
			// Hack to get ourselves in a static function
			CWnd* pWnd = AfxGetMainWnd();

			FileMisc::Run(*pWnd, sExportPath);
		}
		break;
	}

	// Error message
	UINT nIcon = 0, nMessageID = 0;

	switch (nRes)
	{
	case IIER_CANCELLED:
	case IIER_SUCCESS:
		break;

	case IIER_SOMEFAILED:
		nMessageID = IDS_EXPORT_SOMEFAILED;
		nIcon = MB_ICONERROR;
		break;

	case IIER_BADFILE:
		nMessageID = IDS_EXPORT_CANTSAVE;
		nIcon = MB_ICONERROR;
		break;

// 	case IIER_BADFORMAT:
// 		nMessageID = IDS_EXPORTFILE_BADFORMAT;
// 		nIcon = MB_ICONERROR;
// 		break;

	case IIER_BADINTERFACE:
		// TODO
		break;
	}

	if (nMessageID)
		CMessageBox::AfxShow(nMessageID, (MB_OK | nIcon));
}

void CToDoListWnd::OnSetPriority(UINT nCmdID) 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	if (!tdc.IsReadOnly() && tdc.HasSelection())
	{
		if (nCmdID == ID_EDIT_SETPRIORITYNONE) 
			tdc.SetSelectedTaskPriority(-2, FALSE);
		else
			tdc.SetSelectedTaskPriority((nCmdID - ID_EDIT_SETPRIORITY0), FALSE);
	}
}

void CToDoListWnd::OnUpdateSetPriority(CCmdUI* pCmdUI) 
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	pCmdUI->Enable(tdc.CanEditSelectedTask(TDCA_PRIORITY));
	
	int nPriority = (pCmdUI->m_nID - ID_EDIT_SETPRIORITY0);
	
	if (pCmdUI->m_nID == ID_EDIT_SETPRIORITYNONE)
		nPriority = -2;
	
	pCmdUI->SetCheck((nPriority == tdc.GetSelectedTaskPriority()) ? 1 : 0);
}

void CToDoListWnd::OnEditAddFileLink() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	int nNumFiles = tdc.GetSelectedTaskFileLinkCount();
		
	CPreferences prefs;
	CFileOpenDialog dialog(IDS_SETFILELINK_TITLE, 
							NULL, 
							((nNumFiles == 1) ? tdc.GetSelectedTaskFileLink(0) : _T("")), 
							(EOFN_DEFAULTOPEN | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT), 
							CEnString(IDS_ALLFILEFILTER));
	
	if (dialog.DoModal(prefs) == IDOK)
	{
		CStringArray aFiles;
		
		if (dialog.GetPathNames(aFiles))
			tdc.SetSelectedTaskFileLinks(aFiles, TRUE); // append
	}
}

void CToDoListWnd::OnUpdateEditAddFileLink(CCmdUI* pCmdUI) 
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	pCmdUI->Enable(tdc.CanEditSelectedTask(TDCA_FILELINK) && (tdc.GetSelectedTaskCount() == 1));
}

void CToDoListWnd::OnEditOpenFileLink(UINT nCmdID) 
{
	int nFile = (nCmdID - ID_EDIT_OPENFILELINK1);
	ASSERT(nFile < 16);

	if (nFile < 16)
		GetToDoCtrl().GotoSelectedTaskFileLink(nFile);
}

void CToDoListWnd::OnUpdateEditOpenFileLink(CCmdUI* pCmdUI) 
{
	// we do all the work from the first item
	if (pCmdUI->m_pMenu && (pCmdUI->m_nID == ID_EDIT_OPENFILELINK1))
	{
		// remove all existing items
		for (int nFile = 0; nFile < 16; nFile++)
		{
			if (pCmdUI->m_pMenu->DeleteMenu((ID_EDIT_OPENFILELINK1 + nFile), MF_BYCOMMAND))
				pCmdUI->m_nIndexMax--;
		}
	
		// re-add the selected task's file refs
		CStringArray aFileLinks;

		if (GetToDoCtrl().GetSelectedTaskFileLinks(aFileLinks))
		{
			// restrict file lengths to 250 pixels
			CClientDC dc(this);

			for (int nFile = 0; nFile < aFileLinks.GetSize(); nFile++)
			{
				CEnString sFileLink(aFileLinks[nFile]);
				ASSERT(!sFileLink.IsEmpty());

				sFileLink.FormatDC(&dc, 250, ES_PATH);

				pCmdUI->m_pMenu->InsertMenu(pCmdUI->m_nIndex++, 
											(MF_BYPOSITION | MF_STRING), 
											(ID_EDIT_OPENFILELINK1 + nFile), 
											sFileLink);
			}

			pCmdUI->m_nIndex--;
			pCmdUI->m_nIndexMax += aFileLinks.GetSize();
		}
		else // restore the generic text and disable
		{
			pCmdUI->m_pMenu->InsertMenu(pCmdUI->m_nIndex, 
										(MF_BYPOSITION | MF_STRING | MF_GRAYED), 
										ID_EDIT_OPENFILELINK1, 
										CEnString(IDS_EDIT_OPENFILELINK));
		}
	}
}

void CToDoListWnd::PopulateToolArgs(USERTOOLARGS& args) const
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
		
	args.sTasklist = tdc.GetFilePath();
	args.sTaskTitle = tdc.FormatSelectedTaskTitles(FALSE);
	args.sTaskExtID = tdc.GetSelectedTaskExtID();
	args.sTaskComments = tdc.GetSelectedTaskComments();
	args.sTaskFileLink = tdc.GetSelectedTaskFileLink(0);
	args.sTaskAllocBy = tdc.GetSelectedTaskAllocBy();
	args.sTaskPath = tdc.GetSelectedTaskPath(FALSE);
	
	CDWordArray aIDs;
	DWORD dwTemp;
	
	if (tdc.GetSelectedTaskIDs(aIDs, dwTemp, FALSE))
		args.sTaskIDs = Misc::FormatArray(aIDs, _T("|"));
	
	CStringArray aAllocTo;
	
	if (tdc.GetSelectedTaskAllocTo(aAllocTo))
		args.sTaskAllocTo = Misc::FormatArray(aAllocTo, _T("|"));

	tdc.GetSelectedTaskCustomAttributeData(args.mapTaskCustData, TRUE);
	tdc.GetAutoListData(args.tdlListData, TDCA_ALL);
}

LRESULT CToDoListWnd::OnPreferencesTestTool(WPARAM /*wp*/, LPARAM lp)
{
	USERTOOL* pTool = (USERTOOL*)lp;
	
	if (pTool)
	{
		const CPreferencesDlg& prefs = Prefs();
		const CFilteredToDoCtrl& tdc = GetToDoCtrl();

		CTDCToolsHelper th(prefs.GetEnableTDLExtension(), prefs.GetDisplayDatesInISO());

		USERTOOLARGS args;
		PopulateToolArgs(args);

		th.TestTool(*pTool, args, tdc.GetCustomAttributeDefs());
	}
	
	return 0;
}

LRESULT CToDoListWnd::OnPreferencesClearMRU(WPARAM /*wp*/, LPARAM /*lp*/)
{
	m_mruList.RemoveAll();
	m_mruList.WriteList(CPreferences());
	
	return 0;
}

LRESULT CToDoListWnd::OnPreferencesEditLanguageFile(WPARAM /*wp*/, LPARAM /*lp*/)
{
	CString sLangFilePath = Misc::GetQuoted(Prefs().GetLanguageFile(), '\\');

	return FileMisc::Run(*this, _T("TDLTransEdit.exe"), sLangFilePath, SW_SHOWNORMAL, FileMisc::GetModuleFolder());
}

void CToDoListWnd::OnViewNextTasklist() 
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

void CToDoListWnd::OnViewPrevTasklist() 
{
	if (GetTDCCount() < 2)
		return;
	
	int nPrev = (GetSelToDoCtrl() - 1);
	
	if (nPrev < 0)
		nPrev = (GetTDCCount() - 1);
	
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
		if (Prefs().HasSysTrayOptions(STO_ONMINIMIZE, STO_ONMINCLOSE))
		{
			MinimizeToTray();
		}
		else
		{
			// SPECIAL FIX: Apparently when Ultramon hooks the minimize
			// button it ends up sending us a close message!
			ShowWindow(SW_MINIMIZE);
		}
		return;

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
		{
			BOOL bWasMinimised = IsIconic();
			CFrameWnd::OnSysCommand(nID, lParam);
			
			Resize();
			PostAppRestoreFocus();

			if (bWasMinimised)
				OnTimerCheckReloadTasklists(-1, TRUE);
		}
		return;
	}

	CFrameWnd::OnSysCommand(nID, lParam);
}


void CToDoListWnd::OnUpdateImport(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

UINT CToDoListWnd::MapNewTaskPos(PUIP_NEWTASKPOS nPos, BOOL bSubtask)
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

UINT CToDoListWnd::GetNewTaskCmdID() const
{
	return MapNewTaskPos(Prefs().GetNewTaskPos(), FALSE);
}

UINT CToDoListWnd::GetNewSubtaskCmdID() const
{
	return MapNewTaskPos(Prefs().GetNewSubtaskPos(), TRUE);
}

void CToDoListWnd::OnNewTask(UINT nCmdID) 
{
	DWORD dwDependencyID = 0;

	switch (nCmdID)
	{
	case ID_NEWTASK: 
		nCmdID = GetNewTaskCmdID();
		break;

	case ID_NEWSUBTASK:
		nCmdID = GetNewSubtaskCmdID();
		break;

	case ID_NEWTASK_ATTOP:
	case ID_NEWTASK_ATBOTTOM:
	case ID_NEWTASK_AFTERSELECTEDTASK:
	case ID_NEWTASK_BEFORESELECTEDTASK:
	case ID_NEWTASK_ATTOPSELECTED:
	case ID_NEWTASK_ATBOTTOMSELECTED:
	case ID_NEWSUBTASK_ATTOP:
	case ID_NEWSUBTASK_ATBOTTOM:
		break;

	case ID_NEWTASK_DEPENDENTAFTERSELECTEDTASK:
	case ID_NEWTASK_DEPENDENTBEFORESELECTEDTASK:
		dwDependencyID = GetToDoCtrl().GetSelectedTaskID();
		break;

	default:
		ASSERT(0);
		return;
	}

	TDC_INSERTWHERE nInsert = TDC::MapInsertIDToInsertWhere(nCmdID);

	VERIFY (CreateNewTask(CEnString(IDS_TASK), nInsert, TRUE, dwDependencyID));
}

void CToDoListWnd::OnUpdateNewTask(CCmdUI* pCmdUI) 
{
	UINT nCmdID = pCmdUI->m_nID;
	BOOL bDependent = FALSE;

	switch (nCmdID)
	{
	case ID_NEWTASK: 
		nCmdID = GetNewTaskCmdID();
		break;

	case ID_NEWSUBTASK:
		nCmdID = GetNewSubtaskCmdID();
		break;

	case ID_NEWTASK_ATTOP:
	case ID_NEWTASK_ATBOTTOM:
	case ID_NEWTASK_AFTERSELECTEDTASK:
	case ID_NEWTASK_BEFORESELECTEDTASK:
	case ID_NEWTASK_ATTOPSELECTED:
	case ID_NEWTASK_ATBOTTOMSELECTED:
	case ID_NEWSUBTASK_ATTOP:
	case ID_NEWSUBTASK_ATBOTTOM:
		break;

	case ID_NEWTASK_DEPENDENTAFTERSELECTEDTASK:
	case ID_NEWTASK_DEPENDENTBEFORESELECTEDTASK:
		bDependent = TRUE;
		break;

	default:
		ASSERT(0);
		return;
	}

	TDC_INSERTWHERE nInsert = TDC::MapInsertIDToInsertWhere(nCmdID);

	pCmdUI->Enable(CanCreateNewTask(nInsert, bDependent));
}

BOOL CToDoListWnd::CanCreateNewTask(TDC_INSERTWHERE nInsertWhere, BOOL bDependent) const
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();

	if (tdc.CanCreateNewTask(nInsertWhere))
		return TRUE;

	// Special case: Map to the default position
	if ((tdc.GetTaskCount() == 0) && !bDependent)
	{
		UINT nNewTaskID = GetNewTaskCmdID();
		
		if (TDC::MapInsertIDToInsertWhere(nNewTaskID) == nInsertWhere)
			return TRUE;
	}

	return FALSE;
}

void CToDoListWnd::OnSplitTask(UINT nCmdID) 
{
	switch (nCmdID)
	{
	case ID_SPLITTASKINTO_TWO:
	case ID_SPLITTASKINTO_THREE:
	case ID_SPLITTASKINTO_FOUR:
	case ID_SPLITTASKINTO_FIVE:
		{
			int nNumPieces = (2 + (nCmdID - ID_SPLITTASKINTO_TWO));

			VERIFY(GetToDoCtrl().SplitSelectedTask(nNumPieces));
		}
		break;

	default:
		ASSERT(0);
	}
}

void CToDoListWnd::OnUpdateSplitTask(CCmdUI* pCmdUI) 
{
	switch (pCmdUI->m_nID)
	{
	case ID_SPLITTASKINTO_TWO:
	case ID_SPLITTASKINTO_THREE:
	case ID_SPLITTASKINTO_FOUR:
	case ID_SPLITTASKINTO_FIVE:
		pCmdUI->Enable(GetToDoCtrl().CanSplitSelectedTask());
		break;

	default:
		ASSERT(0);
	}
}

void CToDoListWnd::OnToolsCheckout() 
{
	int nSel = GetSelToDoCtrl();
	
	// sanity check
	if (!m_mgrToDoCtrls.CanCheckOut(nSel))
		return;
	
	CAutoFlag af(m_bSaving, TRUE);
	TDC_FILE nFileRes = TDCF_UNSET;
	BOOL bAllowForcedCheckout = FALSE; // always FALSE first try

	do 
	{
		CString sCheckedOutTo;
		nFileRes = m_mgrToDoCtrls.CheckOut(nSel, sCheckedOutTo, bAllowForcedCheckout);

		if (nFileRes == TDCF_SUCCESS)
		{
			UpdateCaption();
			UpdateMenuIconMgrSourceControlStatus();
			break;
		}

		// Handle error
		CString sFilePath(m_mgrToDoCtrls.GetFilePath(nSel));

		if (nFileRes != TDCF_SSC_CHECKEDOUTBYOTHER)
		{
			ASSERT(sCheckedOutTo.IsEmpty());

			HandleSaveTasklistError(nFileRes, sFilePath);
			break;
		}

		// Handle 'checked out to other' error
		ASSERT(!sCheckedOutTo.IsEmpty());
		
		SYSTEMTIME stLastMod;
		FileMisc::GetFileLastModified(sFilePath, stLastMod);

		CEnString sMessage;
		sMessage.Format(IDS_CHECKEDOUTBYOTHER, sFilePath, sCheckedOutTo, 
						COleDateTime(stLastMod).Format(VAR_DATEVALUEONLY));

		UINT nFlags = (MB_OK | MB_ICONEXCLAMATION);
		
		if (m_bAllowForcedCheckOut)
		{
			sMessage += CEnString(IDS_QUERYFORCEDCHECKOUT, sCheckedOutTo);
			nFlags |= MB_YESNO;
		}

		if (CMessageBox::AfxShow(IDS_CHECKOUT_TITLE, sMessage, nFlags) != IDYES)
			break;

		// else try again with a forced checkout
		bAllowForcedCheckout = TRUE;
	} 
	while (nFileRes != TDCF_SUCCESS);
}

void CToDoListWnd::OnUpdateToolsCheckout(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_mgrToDoCtrls.CanCheckOut(GetSelToDoCtrl()));
}

void CToDoListWnd::OnToolsToggleCheckin() 
{
	if (GetToDoCtrl().IsCheckedOut())
		OnToolsCheckin();
	else
		OnToolsCheckout();
}

void CToDoListWnd::OnUpdateToolsToggleCheckin(CCmdUI* pCmdUI) 
{
	BOOL bEnable = m_mgrToDoCtrls.CanCheckInOut(GetSelToDoCtrl());
	
	pCmdUI->Enable(bEnable);
	pCmdUI->SetCheck((bEnable && GetToDoCtrl().IsCheckedOut()) ? 1 : 0);
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
			
			int nRet = CMessageBox::AfxShow(IDS_CHECKIN_TITLE, sMessage, MB_YESNOCANCEL);
			
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
	pCmdUI->Enable(m_mgrToDoCtrls.CanCheckIn(GetSelToDoCtrl()));
}

int CToDoListWnd::GetExportFilePaths(const CTDLExportDlg& dlg, CStringArray& aExportPaths) const
{
	ASSERT(!dlg.GetExportToClipboard());

	aExportPaths.RemoveAll();

	CString sExportPath = dlg.GetExportPath();
	int nTDCCount = GetTDCCount();

	// When exporting a single file
	if ((nTDCCount == 1) || !dlg.GetExportAllTasklists() || dlg.GetExportOneFile())
	{
		aExportPaths.Add(sExportPath);
	}
	else // all open tasklists
	{
		int nExporter = m_mgrImportExport.FindExporterByType(dlg.GetFormatTypeID());
		ASSERT(nExporter != -1);

		CString sExt = m_mgrImportExport.GetExporterFileExtension(nExporter, TRUE);

		for (int nTDC = 0; nTDC < nTDCCount; nTDC++)
		{
			CString sFName;
			FileMisc::SplitPath(m_mgrToDoCtrls.GetFilePath(nTDC), NULL, NULL, &sFName);

			CString sFilePath;
			FileMisc::MakePath(sFilePath, NULL, sExportPath, sFName, sExt);

			aExportPaths.Add(sFilePath);
		}
	}
	ASSERT(aExportPaths.GetSize());

	return aExportPaths.GetSize();
}

BOOL CToDoListWnd::CanOverwriteExportFiles(const CStringArray& aExportPaths) const
{
	// NEVER allow a tasklist to get overwritten
	int nPath = aExportPaths.GetSize();

	while (nPath--)
	{
		if (m_mgrToDoCtrls.FindToDoCtrl(aExportPaths[nPath]) != -1)
		{
			CMessageBox::AfxShow(IDS_ERROR_EXPORT_OVERWRITETASKLIST, MB_ICONERROR | MB_OK);
			return FALSE;
		}
	}

	// Check for file existence
	CStringArray aExistPaths;

	if (FileMisc::GetExistingFiles(aExportPaths, aExistPaths) == 0)
		return TRUE;

	if (IDCANCEL == CMessageBox::AfxShow(IDS_CONFIRM_EXPORT_OVERWRITE, Misc::FormatArray(aExistPaths, _T("\n\n")), MB_OKCANCEL | MB_ICONQUESTION))
		return FALSE;

	// Check if any of the existing files are locked
	CStringArray aLockedPaths;

	while (FileMisc::GetNonWritableFiles(aExistPaths, aLockedPaths))
	{
		if (IDCANCEL == CMessageBox::AfxShow(IDS_ERROR_LOCKEDFILES, Misc::FormatArray(aLockedPaths, _T("\n\n")), MB_ICONERROR | MB_RETRYCANCEL))
			return FALSE;
	}

	return TRUE;
}

void CToDoListWnd::OnExport() 
{
	int nTDCCount = GetTDCCount(), nSelTDC = GetSelToDoCtrl();
	ASSERT (nTDCCount >= 1);

	const CPreferencesDlg& userPrefs = Prefs();
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	const CString sTasklistPath = m_mgrToDoCtrls.GetFilePath(nSelTDC, FALSE);

	CTDLExportDlg dialog(m_mgrToDoCtrls.GetFriendlyProjectName(nSelTDC),
						 m_mgrImportExport,
						 (nTDCCount == 1),
						 (tdc.GetTaskView() != FTCV_TASKLIST),
						 userPrefs.GetExportVisibleColsOnly(),
						 sTasklistPath,
						 userPrefs.GetSaveExportFolderPath(),
						 tdc.GetCustomAttributeDefs());
	
	// keep showing the dialog until the user selects a non-existing 
	// filename OR they confirm that they want to overwrite the file(s)
	BOOL bOverWrite = FALSE, bExportToClipboard = FALSE;

	while (!bOverWrite)
	{
		if (dialog.DoModal() != IDOK)
			return;

		if (dialog.GetExportToClipboard())
		{
			bOverWrite = TRUE;
			break;
		}

		// else
		CStringArray aExportPaths;
		VERIFY(GetExportFilePaths(dialog, aExportPaths));
			
		bOverWrite = CanOverwriteExportFiles(aExportPaths);
	}
	ASSERT(bOverWrite);
	
	UpdateWindow();

	// export
	DOPROGRESS(IDS_EXPORTPROGRESS);
	
	int nExporter = m_mgrImportExport.FindExporterByType(dialog.GetFormatTypeID());
	ASSERT(nExporter != -1);

	BOOL bHtmlComments = m_mgrImportExport.ExporterSupportsHtmlComments(nExporter);
	CString sExportPath = (bExportToClipboard ? TEMP_CLIPBOARD_FILEPATH : dialog.GetExportPath());
	
	// The only OR active tasklist -----------------------------------------------------
	if ((nTDCCount == 1) || !dialog.GetExportAllTasklists())
	{
		tdc.SaveAllTaskViewPreferences();

		// set the html image folder to be the output path with an different extension
		CString sHtmlImgFolder = GetHtmlImageFolder(bHtmlComments, sExportPath);
		
		// Note: don't need to verify password if encrypted tasklist is active
		CTaskFile tasks;
		GetTasks(tdc, bHtmlComments, sHtmlImgFolder, FALSE, dialog.GetTaskSelection(), tasks);

		// add report details
		tasks.SetReportDetails(dialog.GetExportTitle(), dialog.GetExportDate());
		
		// save intermediate tasklist to file as required
		LogIntermediateTaskList(tasks);

		// Do the export
		DWORD dwFlags = TDC::MapPrintToExportStyle(dialog.GetHtmlStyle());

		IIMPORTEXPORT_RESULT nRes = m_mgrImportExport.ExportTaskList(tasks, sExportPath, nExporter, dwFlags);

		HandleExportTasklistResult(nRes, sExportPath, bExportToClipboard, userPrefs.GetPreviewExport());
	} 
	else if (dialog.GetExportOneFile())	// Multiple tasklists to one file --------------------------
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
				
				// set the html image folder to be the output path with an different extension
				CString sHtmlImgFolder = GetHtmlImageFolder(bHtmlComments, sExportPath);
				
				GetTasks(tdc, bHtmlComments, sHtmlImgFolder, FALSE, dialog.GetTaskSelection(), tasks);
				
				// add report details
				tasks.SetReportDetails(m_mgrToDoCtrls.GetFriendlyProjectName(nCtrl), dialog.GetExportDate());

				// save intermediate tasklist to file as required
				LogIntermediateTaskList(tasks);
			}
			
			tdc.UnlockWindowUpdate();
		}
		
		Resize();

		// Do the export
		DWORD dwFlags = TDC::MapPrintToExportStyle(dialog.GetHtmlStyle());

		IIMPORTEXPORT_RESULT nRes = m_mgrImportExport.ExportTaskLists(taskFiles, sExportPath, nExporter, dwFlags);

		HandleExportTasklistResult(nRes, sExportPath, bExportToClipboard, userPrefs.GetPreviewExport());
	}
	else // More than one separate files ----------------------------------------------------------
	{
		ASSERT(!bExportToClipboard);

		CString sExt = m_mgrImportExport.GetExporterFileExtension(nExporter, TRUE);
		
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
												m_mgrImportExport.GetExporterFileFilter(nExporter));
						
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
				
				// set the html image folder to be the output path with an different extension
				CString sHtmlImgFolder = GetHtmlImageFolder(bHtmlComments, sFilePath);
				
				CTaskFile tasks;
				GetTasks(tdc, bHtmlComments, sHtmlImgFolder, FALSE, dialog.GetTaskSelection(), tasks);
				
				// add report details
				tasks.SetReportDetails(m_mgrToDoCtrls.GetFriendlyProjectName(nCtrl), dialog.GetExportDate());

				// save intermediate tasklist to file as required
				LogIntermediateTaskList(tasks);

				// Do the export
				DWORD dwFlags = TDC::MapPrintToExportStyle(dialog.GetHtmlStyle());

				IIMPORTEXPORT_RESULT nRes = m_mgrImportExport.ExportTaskList(tasks, sFilePath, nExporter, dwFlags);

				// Show error messages but that's all
				HandleExportTasklistResult(nRes, _T(""), FALSE, FALSE);
			}
			
			tdc.UnlockWindowUpdate();
		}
	}
}

int CToDoListWnd::GetTasks(CFilteredToDoCtrl& tdc, BOOL bHtmlComments, 
						   const CString& sHtmlImageDir, BOOL bTransform, 
						   TSD_TASKS nWhatTasks, TDCGETTASKS& filter, CTaskFile& tasks) const
{
	// preferences
	const CPreferencesDlg& userPrefs = Prefs();

	tasks.Reset();	
	tasks.SetProjectName(tdc.GetFriendlyProjectName());

	// export flags
	if (userPrefs.GetExportParentTitleCommentsOnly())
		filter.dwFlags |= TDCGTF_PARENTTITLECOMMENTSONLY;

	if (bTransform)
	{
		ASSERT(bHtmlComments);
		filter.dwFlags |= TDCGTF_TRANSFORM;
	}

	if (bHtmlComments)
	{
		if (filter.WantAttribute(TDCA_COMMENTS))
		{
			ASSERT(!filter.mapAttribs.IsEmpty());
			filter.mapAttribs.Add(TDCA_HTMLCOMMENTS);
		}

		ASSERT(!sHtmlImageDir.IsEmpty());
		tasks.SetHtmlImageFolder(sHtmlImageDir);

		// And delete all existing images in that folder
		FileMisc::DeleteFolderContents(sHtmlImageDir, FMDF_SUBFOLDERS | FMDF_ALLOWDELETEONREBOOT | FMDF_HIDDENREADONLY);
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
		tdc.GetSelectedTasks(tasks, filter);
		break;
	}
	
	// delete the HTML image folder if it is empty
	// this will fail if it is not empty.
	if (bHtmlComments && !sHtmlImageDir)
		::RemoveDirectory(sHtmlImageDir);
	
	return tasks.GetTaskCount();
}

int CToDoListWnd::GetTasks(CFilteredToDoCtrl& tdc, BOOL bHtmlComments, 
						   const CString& sHtmlImageDir, BOOL bTransform,
						   const CTaskSelectionDlg& taskSel, CTaskFile& tasks) const
{
	TDCGETTASKS filter(TDCGT_ALL);
	taskSel.GetSelectedAttributes(tdc, filter.mapAttribs);

	TSD_TASKS nWhatTasks = taskSel.GetWantWhatTasks();

	switch (nWhatTasks)
	{
	case TSDT_SELECTED:
		if (!taskSel.GetWantSelectedSubtasks())
		{
			filter.dwFlags |= TDCGSTF_NOTSUBTASKS;
		}

		if (taskSel.GetWantSelectedParentTask())
		{
			filter.dwFlags |= TDCGSTF_IMMEDIATEPARENT;
		}
		break;

	default:
		if (taskSel.GetWantCompletedTasksOnly())
		{
			filter.nFilter = TDCGT_DONE;
		}
		else if (taskSel.GetWantIncompleteTasksOnly())
		{
			filter.nFilter = TDCGT_NOTDONE;
		}
		break;
	}
	
	return GetTasks(tdc, bHtmlComments, sHtmlImageDir, bTransform, nWhatTasks, filter, tasks);
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

	CTDLTransformDialog dialog(sTitle, 
								(tdc.GetTaskView() != FTCV_TASKLIST),
								tdc.GetStylesheetPath(),
								tdc.GetCustomAttributeDefs());
	
	if (dialog.DoModal() != IDOK)
		return;
	
	CString sStylesheet = dialog.GetStylesheet();
	sTitle = dialog.GetTitle();
	
	// output path
	CString sOutputPath(tdc.GetFilePath()); 
	{
		// Output file extension may be specified in the stylesheet
		CXslFile xsl;
		VERIFY (xsl.Load(sStylesheet));
		
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
	CString sHtmlImgFolder = GetHtmlImageFolder(TRUE, sOutputPath);
	
	CTaskFile tasks;
	GetTasks(tdc, TRUE, sHtmlImgFolder, TRUE, dialog.GetTaskSelection(), tasks);

	// add report details
	tasks.SetReportDetails(m_mgrToDoCtrls.GetFriendlyProjectName(nSelTDC), dialog.GetDate());
	
	// save intermediate tasklist to file as required
	LogIntermediateTaskList(tasks);
	
	if (tasks.TransformToFile(sStylesheet, sOutputPath))
	{
		// preview
		if (Prefs().GetPreviewExport())
			FileMisc::Run(*this, sOutputPath);
	}
}

CString CToDoListWnd::GetHtmlImageFolder(BOOL bHtmlComments, const CString& sTasklistPath)
{
	CString sHtmlImgFolder;

	if (bHtmlComments)
	{
		ASSERT(!sTasklistPath.IsEmpty());

		sHtmlImgFolder = sTasklistPath;
		FileMisc::ReplaceExtension(sHtmlImgFolder, HTMLIMG_EXT);
	}

	return sHtmlImgFolder;
}

BOOL CToDoListWnd::LogIntermediateTaskList(CTaskFile& tasks)
{
	BOOL bSuccess = TRUE;

	if (FileMisc::IsLoggingEnabled())
	{
		// Cache path so it can be restored
		CString sOrgPath = tasks.GetFileName(true);

		bSuccess = tasks.Save(GetIntermediateTaskListPath(sOrgPath), SFEF_UTF16);

		// Restore original path
		tasks.SetFilePath(sOrgPath);
	}

	return bSuccess;
}

CString CToDoListWnd::GetIntermediateTaskListPath(LPCTSTR szRefPath)
{
	CEnString sRefName(IDS_TDC_UNTITLEDFILE);
	Misc::Trim(sRefName);

	if (szRefPath && FileMisc::IsPath(szRefPath))
		sRefName = FileMisc::RemoveExtension(FileMisc::GetFileNameFromPath(szRefPath));
	
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

BOOL CToDoListWnd::InitFindDialog()
{
	if (!m_dlgFindTasks.GetSafeHwnd())
	{
		UpdateFindDialogActiveTasklist();
		
		if (!m_dlgFindTasks.Create())
			return FALSE;

		if (CThemed::IsAppThemed())
			m_dlgFindTasks.SetUITheme(m_theme);
	}

	return TRUE;
}

LRESULT CToDoListWnd::OnFindDlgDockChange(WPARAM wp, LPARAM lp)
{
	// Modify our size to accept or release the 
	// space needed to dock the find tasks dialog
	if (!IsZoomed())
	{
		DM_POS nOldPos = (DM_POS)wp;
		DM_POS nNewPos = (DM_POS)lp;

		// Switching from left to right and vice versa is a
		// special case because it requires no size change
		if (((nOldPos == DMP_LEFT) && (nNewPos == DMP_RIGHT)) ||
			((nOldPos == DMP_RIGHT) && (nNewPos == DMP_LEFT)))
		{
			// No change
		}
		else
		{
			CRect rOldWindow;
			GetWindowRect(rOldWindow);

			CRect rNewWindow(rOldWindow);

			// Release old space
			if (nOldPos != DMP_UNDOCKED)
			{
				int nOldDim = m_dlgFindTasks.GetDockedDimension(nOldPos, FALSE);

				switch (nOldPos)
				{
				case DMP_LEFT:	rNewWindow.left		+= nOldDim;	break;
				case DMP_RIGHT:	rNewWindow.right	-= nOldDim;	break;
				case DMP_BELOW:	rNewWindow.bottom	-= nOldDim;	break;
				}
			}

			// Add new space
			if (nNewPos != DMP_UNDOCKED)
			{
				int nNewDim = m_dlgFindTasks.GetDockedDimension(nNewPos, FALSE);

				switch (nNewPos)
				{
				case DMP_LEFT:	rNewWindow.left		-= nNewDim;	break;
				case DMP_RIGHT:	rNewWindow.right	+= nNewDim;	break;
				case DMP_BELOW:	rNewWindow.bottom	+= nNewDim;	break;
				}
			}

			if (rNewWindow != rOldWindow)
				MoveWindow(rNewWindow);
		}
	}

	Resize(); // Always

	return 0L;
}

void CToDoListWnd::OnFindTasks() 
{
	ShowFindDialog(TRUE);
}

void CToDoListWnd::ShowFindDialog(BOOL bAllowResizeApp)
{
	// Create the find dialog first time only
	InitFindDialog();

	if (IsWindowVisible())
	{
		// remove results from encrypted tasklists but not from the 
		// active tasklist
		if (!m_dlgFindTasks.IsWindowVisible())
		{
			int nSelTDC = GetSelToDoCtrl();
			int nTDC = GetTDCCount();

			while (nTDC--)
			{
				const CFilteredToDoCtrl& tdc = GetToDoCtrl(nTDC);

				if (nTDC != nSelTDC && tdc.IsEncrypted())
					m_dlgFindTasks.DeleteResults(&tdc);
			}

			m_dlgFindTasks.Show();
		}
		else if (m_dlgFindTasks.IsDocked())
		{
			if (!IsChildOrSame(m_dlgFindTasks, ::GetFocus()))
				m_dlgFindTasks.SetFocus();
		}
		else
		{
			m_dlgFindTasks.SetForegroundWindow();
		}

		// If the find dialog was docked, fake a re-docking
		// event to enlarge the app window appropriately
		if (!m_bFindShowing && bAllowResizeApp && m_dlgFindTasks.IsDocked())
			OnFindDlgDockChange(DMP_UNDOCKED, m_dlgFindTasks.GetDockPosition());
	}
	
	m_bFindShowing = TRUE;

	Resize();
	InvalidateAllCtrls(this);
}

LRESULT CToDoListWnd::OnFindDlgClose(WPARAM /*wp*/, LPARAM /*lp*/)
{
	m_bFindShowing = FALSE;

	// If the find dialog is still docked, fake an un-docking
	// to remove the find dialog from the app's borders
	if (m_dlgFindTasks.IsDocked())
		OnFindDlgDockChange(m_dlgFindTasks.GetDockPosition(), DMP_UNDOCKED);
	
	GetToDoCtrl().SetFocusToTasks();

	return 0L;
}

LRESULT CToDoListWnd::OnFindDlgFind(WPARAM /*wp*/, LPARAM /*lp*/)
{
	// set up the search
	BOOL bSearchAll = m_dlgFindTasks.GetSearchAllTasklists();
	int nSelTaskList = GetSelToDoCtrl();
	
	int nFrom = bSearchAll ? 0 : nSelTaskList;
	int nTo = bSearchAll ? GetTDCCount() - 1 : nSelTaskList;
	
	// search params
	SEARCHPARAMS params;

	if (m_dlgFindTasks.GetSearchParams(params))
	{
		CWaitCursor cursor;

		int nSel = GetSelToDoCtrl();
		BOOL bMultiAttribFind = params.HasMultipleAttributes();
		
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
			
			const CFilteredToDoCtrl& tdc = GetToDoCtrl(nCtrl);
			CResultArray aResults;
			
			if (tdc.FindTasks(params, aResults))
			{
				// Eliminate flicker
				HWND hwndHold = NULL;

				if (IsWindowVisible() && !IsIconic() && m_dlgFindTasks.IsWindowVisible())
					hwndHold = m_dlgFindTasks.GetSafeHwnd();

				CHoldRedraw hr(hwndHold);

				m_dlgFindTasks.AddResults(&tdc, aResults, !bMultiAttribFind, m_mgrToDoCtrls.GetTabItemText(nCtrl));
			}
		}
	}	
	
	m_dlgFindTasks.SetActiveWindow();
	
	return 0;
}

LRESULT CToDoListWnd::OnFindSelectResult(WPARAM /*wp*/, LPARAM lp)
{
	// extract Task ID
	FTDRESULT* pResult = (FTDRESULT*)lp;
	ASSERT (pResult->dwTaskID); 
	
	int nCtrl = m_mgrToDoCtrls.FindToDoCtrl(pResult->pTDC);

	if (nCtrl == -1)
	{
		ASSERT(0);
		return 0L;
	}
	
	if (m_tabCtrl.GetCurSel() != nCtrl)
	{
		if (!SelectToDoCtrl(nCtrl, TRUE))
			return 0L;
	}
	
	// we can't use pResult->pTDC because it's const
	CFilteredToDoCtrl& tdc = GetToDoCtrl(nCtrl);

	if (tdc.SelectTask(pResult->dwTaskID, TRUE))
	{
		tdc.SetFocusToTasks();

		Invalidate();
		UpdateWindow();
	}
	
	return 1L;
}

LRESULT CToDoListWnd::OnFindSelectAll(WPARAM /*wp*/, LPARAM /*lp*/)
{
	if (!m_dlgFindTasks.GetResultCount())
		return 0;
	
	// if find dialog is floating then hide it
	if (!m_dlgFindTasks.IsDocked())
		m_dlgFindTasks.Show(FALSE);
	
	CFilteredToDoCtrl& tdcSel = GetToDoCtrl();
	CHoldRedraw hr(tdcSel);
	CWaitCursor cursor;
	
	for (int nTDC = 0; nTDC < GetTDCCount(); nTDC++)
	{
		CFilteredToDoCtrl& tdc = GetToDoCtrl(nTDC);
		CDWordArray aTaskIDs;
		
		if (m_dlgFindTasks.GetResultIDs(&tdc, aTaskIDs))
			tdc.SelectTasks(aTaskIDs);
	}

	tdcSel.SetFocusToTasks();

	return 0;
}

LRESULT CToDoListWnd::OnFindApplyAsFilter(WPARAM /*wp*/, LPARAM lp)
{
	// if find dialog is floating then hide it
	if (!m_dlgFindTasks.IsDocked())
		m_dlgFindTasks.Show(FALSE);
	
	TDCADVANCEDFILTER filter((LPCTSTR)lp);
	m_dlgFindTasks.GetSearchParams(filter);

	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	tdc.SetAdvancedFilter(filter);
	
	RefreshFilterBarControls(TDCA_ALL);
	UpdateTimeTrackerTasks(TRUE);

	tdc.SetFocusToTasks();

	return 0;
}

LRESULT CToDoListWnd::OnFindAddSearch(WPARAM /*wp*/, LPARAM lp)
{
	ASSERT(lp);
	LPCTSTR szFilter = (LPCTSTR)lp;

	// Notify the filter bar
	RefreshFilterBarAdvancedFilters();

	// See RemapAdvancedFilterMenuItemIDs for more detail
	CStringArray aPrevFilters;
	aPrevFilters.Copy(m_filterBar.GetAdvancedFilterNames());

	VERIFY(Misc::RemoveItem(szFilter, aPrevFilters));

	RemapAdvancedFilterMenuItemIDs(aPrevFilters, m_filterBar.GetAdvancedFilterNames());

	return 0;
}

LRESULT CToDoListWnd::OnFindDeleteSearch(WPARAM /*wp*/, LPARAM lp)
{
	ASSERT(lp);
	LPCTSTR szFilter = (LPCTSTR)lp;

	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	if (tdc.GetAdvancedFilterName().CompareNoCase(szFilter) == 0)
		OnViewClearfilter();

	// Notify the filter bar
	RefreshFilterBarAdvancedFilters();

	// See RemapAdvancedFilterMenuItemIDs for more detail
	CStringArray aPrevFilters;
	aPrevFilters.Copy(m_filterBar.GetAdvancedFilterNames());

	VERIFY(Misc::AddUniqueItem(szFilter, aPrevFilters));
	Misc::SortArray(aPrevFilters);

	RemapAdvancedFilterMenuItemIDs(aPrevFilters, m_filterBar.GetAdvancedFilterNames());

	return 0;
}

LRESULT CToDoListWnd::OnFindSaveSearch(WPARAM /*wp*/, LPARAM lp)
{
	ASSERT(lp);
	LPCTSTR szSearch = (LPCTSTR)lp;

	// Notify the filter bar
	VERIFY(m_findFilterHelper.UpdateFilterBarAdvancedFilter(szSearch));

	TDCADVANCEDFILTER filter;
	m_dlgFindTasks.GetSearchParams(szSearch, filter);

	// Update Filter
	CString sActive;

	if ((m_filterBar.GetFilter(sActive) == FS_ADVANCED) && (sActive == szSearch))
	{
		GetToDoCtrl().SetAdvancedFilter(filter);
	}

	return 0;
}

//------------------------------------------------------------------------

LRESULT CToDoListWnd::OnDropFile(WPARAM wParam, LPARAM lParam)
{
	// Sanity check
	if (!OnCanDropFile(wParam, lParam))
	{
		ASSERT(0);
		return 0L;
	}

	// else
	TLDT_DATA* pData = (TLDT_DATA*)wParam;
	CString sFile = pData->pFilePaths->GetAt(0);

	TDC_FILE nRes = OpenTaskList(sFile);
	HandleLoadTasklistError(nRes, sFile);

	return (nRes == TDCF_SUCCESS);
}

LRESULT CToDoListWnd::OnCanDropFile(WPARAM wParam, LPARAM lParam)
{
	CWnd* pTarget = (CWnd*)lParam;

	if (pTarget != this)
		return FALSE;
	
	// One file at a time
	TLDT_DATA* pData = (TLDT_DATA*)wParam;

	if (!pData->pFilePaths || (pData->pFilePaths->GetSize() != 1))
		return FALSE;

	// Check it's a possible tasklist
	CString sFile = pData->pFilePaths ? pData->pFilePaths->GetAt(0) : _T("");

	return (FileMisc::HasExtension(sFile, _T("tdl")) || 
			FileMisc::HasExtension(sFile, _T("xml")));
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
		CMessageBox::AfxShow(CEnString(nIDDueBy, m_mgrToDoCtrls.GetFriendlyProjectName(GetSelToDoCtrl())), MB_OK);
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

	CStringArray aDefTaskViews;

	m_pPrefs->GetDefaultTaskViews(aDefTaskViews);
	CFilteredToDoCtrl::SetDefaultTaskViews(aDefTaskViews);
}

const CPreferencesDlg& CToDoListWnd::Prefs() const
{
	ASSERT (m_pPrefs);
	return *m_pPrefs;
}

void CToDoListWnd::OnSpellcheckcomments() 
{
	GetToDoCtrl().SpellcheckSelectedTask(FALSE);
}

void CToDoListWnd::OnUpdateSpellcheckcomments(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanSpellcheckSelectedTaskComments());
}

void CToDoListWnd::OnSpellchecktitle() 
{
	GetToDoCtrl().SpellcheckSelectedTask(TRUE);
}

void CToDoListWnd::OnUpdateSpellchecktitle(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().GetSelectedTaskCount());
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
		if (!bWasEncrypted || m_mgrToDoCtrls.VerifyPassword(GetSelToDoCtrl(), TRUE))
		{
			tdc.EnableEncryption(!bWasEncrypted);
		}

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
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
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
		
		UpdateCaption();
	}
}

void CToDoListWnd::OnUpdateFileResetversion(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetToDoCtrl().IsReadOnly());
}

void CToDoListWnd::OnSpellcheckTasklist() 
{
	GetToDoCtrl().Spellcheck();
}

void CToDoListWnd::OnUpdateSpellcheckTasklist(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().GetTaskCount());
}

TDC_FILE CToDoListWnd::SaveAll(DWORD dwFlags)
{
	if (!m_mgrToDoCtrls.AnyIsModified())
		return TDCF_SUCCESS;

	TDC_FILE nSaveAll = TDCF_SUCCESS;

	BOOL bClosingWindows = Misc::HasFlag(dwFlags, TDLS_CLOSINGWINDOWS);
	BOOL bClosingTasklists = Misc::HasFlag(dwFlags, TDLS_CLOSINGTASKLISTS);

	DOPROGRESS(IDS_SAVINGPROGRESS);

	int nCtrl = GetTDCCount();

	while (nCtrl--)
	{
		const CFilteredToDoCtrl& tdc = GetToDoCtrl(nCtrl);

		// bypass unsaved or 'busy' tasklists unless 
		// closing Windows or tasklists
		if (!bClosingWindows && !bClosingTasklists)
		{
			BOOL bWantFlush = !Misc::HasFlag(dwFlags, TDLS_NOFLUSH);

			if (!bWantFlush && tdc.IsTaskLabelEditing())
			{
				TRACE(_T("Tasklist not auto-saved because it is 'busy'\n"));
				continue;
			}

			BOOL bWantUnsaved = Misc::HasFlag(dwFlags, TDLS_INCLUDEUNSAVED);

			if (!bWantUnsaved && !m_mgrToDoCtrls.UsesStorage(nCtrl) && !tdc.HasFilePath())
			{
				TRACE(_T("Tasklist not auto-saved because it is 'unsaved'\n"));
				continue;
			}
		}

		TDC_FILE nSave = ConfirmSaveTaskList(nCtrl, dwFlags);

		if (nSave == TDCF_CANCELLED) // user cancelled
			return TDCF_CANCELLED;

		// else cache any failure w/o overwriting previous
		if (nSaveAll == TDCF_SUCCESS)
			nSaveAll = nSave;

		if (!bClosingWindows && !bClosingTasklists)
			m_mgrToDoCtrls.UpdateTabItemText(nCtrl);
	}

	if (!bClosingWindows)
		UpdateCaption();
	
    return nSaveAll;
}

void CToDoListWnd::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (m_menubar.HandleDrawItem(nIDCtl, lpDrawItemStruct))
		return;

	CFrameWnd::OnDrawItem(nIDCtl, lpDrawItemStruct);
} 

void CToDoListWnd::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	if (m_menubar.HandleMeasureItem(nIDCtl, lpMeasureItemStruct))
		return;
	
	CFrameWnd::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CToDoListWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rSplitter;

	if (GetFindTasksDialogSplitterRect(rSplitter) && rSplitter.PtInRect(point))
	{
		m_bSplitting = TRUE;
		SetCapture();
	}

	CFrameWnd::OnLButtonDown(nFlags, point);
}

void CToDoListWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	CFrameWnd::OnLButtonUp(nFlags, point);
}

void CToDoListWnd::OnCaptureChanged(CWnd* pWnd)
{
	m_bSplitting = FALSE;

	CFrameWnd::OnCaptureChanged(pWnd);
}

void CToDoListWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bSplitting)
	{
		ASSERT(m_dlgFindTasks.IsDocked());

		CRect rOld = GetChildRect(&m_dlgFindTasks), rNew(rOld);

		switch (m_dlgFindTasks.GetDockPosition())
		{
		case DMP_LEFT:
			rNew.right = (point.x - (SPLITTER_WIDTH / 2));
			break;

		case DMP_RIGHT:
			rNew.left = (point.x + (SPLITTER_WIDTH / 2));
			break;

		case DMP_BELOW:
			rNew.top = (point.y + (SPLITTER_WIDTH / 2));
			break;
		}

		if (rNew != rOld)
		{
			m_dlgFindTasks.MoveWindow(rNew);
			m_dlgFindTasks.UpdateWindow();

			Resize();
		}
	}

	CFrameWnd::OnMouseMove(nFlags, point);
}

BOOL CToDoListWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CRect rSplitter;

	if (GetFindTasksDialogSplitterRect(rSplitter))
	{
		CPoint ptCursor(GetCurrentMessage()->pt);
		ScreenToClient(&ptCursor);

		if (rSplitter.PtInRect(ptCursor))
		{
			UINT nIDCursor = ((m_dlgFindTasks.GetDockPosition() == DMP_BELOW) ? AFX_IDC_VSPLITBAR : AFX_IDC_HSPLITBAR);
			::SetCursor(AfxGetApp()->LoadCursor(nIDCursor));

			return TRUE;
		}
	}

	// else
	return CFrameWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CToDoListWnd::OnViewSelectedTask(BOOL bNext)
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	BOOL bWasFiltered = tdc.HasAnyFilter();

	if (tdc.SelectTasksInHistory(bNext))
	{
		UpdateStatusBar();

		// Update the filter bar if the filter auto-toggled 
		// because the required tasks were hidden
		if (bWasFiltered && !tdc.HasAnyFilter())
		{
			RefreshFilterBarControls(TDCA_ALL);
			UpdateTimeTrackerTasks(TRUE);
		}
	}
}

void CToDoListWnd::OnViewNextSelectedTask() 
{
	OnViewSelectedTask(TRUE);
}

void CToDoListWnd::OnUpdateViewNextSel(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanSelectTasksInHistory(TRUE));
}

void CToDoListWnd::OnViewPrevSelectedTask() 
{
	OnViewSelectedTask(FALSE);
}

void CToDoListWnd::OnUpdateViewPrevSel(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanSelectTasksInHistory(FALSE));
}

void  CToDoListWnd::OnViewExpandTasks(UINT nCmdID)
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	switch (nCmdID)
	{
	case ID_VIEW_EXPANDTASK:
		tdc.ExpandTasks(TDCEC_SELECTED, TRUE);
		break;

	case ID_VIEW_COLLAPSETASK:
		tdc.ExpandTasks(TDCEC_SELECTED, FALSE);
		break;

	case ID_VIEW_TOGGLETASKEXPANDED:
		tdc.ExpandTasks(TDCEC_SELECTED, tdc.CanExpandTasks(TDCEC_SELECTED, TRUE));
		break;

	case ID_VIEW_TOGGLEALLTASKEXPANDED:
		tdc.ExpandTasks(TDCEC_ALL, tdc.CanExpandTasks(TDCEC_ALL, TRUE));
		break;

	case ID_VIEW_EXPANDALL:
		tdc.ExpandTasks(TDCEC_ALL, TRUE);
		break;

	case ID_VIEW_COLLAPSEALL:
		tdc.ExpandTasks(TDCEC_ALL, FALSE);
		break;

	case ID_VIEW_EXPANDDUE:
		tdc.ExpandTasks(TDCEC_DUE, TRUE);
		break;

	case ID_VIEW_COLLAPSEDUE:
		tdc.ExpandTasks(TDCEC_DUE, FALSE);
		break;

	case ID_VIEW_EXPANDSTARTED:
		tdc.ExpandTasks(TDCEC_STARTED, TRUE);
		break;

	case ID_VIEW_COLLAPSESTARTED:
		tdc.ExpandTasks(TDCEC_STARTED, FALSE);
		break;

	default:
		ASSERT(0);
	}
}

void  CToDoListWnd::OnUpdateViewExpandTasks(CCmdUI* pCmdUI)
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	BOOL bEnable = FALSE;

	switch (pCmdUI->m_nID)
	{
	case ID_VIEW_EXPANDTASK:
		bEnable = tdc.CanExpandTasks(TDCEC_SELECTED, TRUE);
		break;

	case ID_VIEW_COLLAPSETASK:
		bEnable = tdc.CanExpandTasks(TDCEC_SELECTED, FALSE);
		break;

	case ID_VIEW_TOGGLETASKEXPANDED:
		bEnable = (tdc.CanExpandTasks(TDCEC_SELECTED, TRUE) || 
					tdc.CanExpandTasks(TDCEC_SELECTED, FALSE));
		break;

	case ID_VIEW_TOGGLEALLTASKEXPANDED:
		bEnable = (tdc.CanExpandTasks(TDCEC_ALL, TRUE) || 
					tdc.CanExpandTasks(TDCEC_ALL, FALSE));
		break;

	case ID_VIEW_EXPANDALL:
		bEnable = tdc.CanExpandTasks(TDCEC_ALL, TRUE);
		break;

	case ID_VIEW_COLLAPSEALL:
		bEnable = tdc.CanExpandTasks(TDCEC_ALL, FALSE);
		break;

	case ID_VIEW_EXPANDDUE:
		bEnable = tdc.CanExpandTasks(TDCEC_DUE, TRUE);
		break;

	case ID_VIEW_COLLAPSEDUE:
		bEnable = tdc.CanExpandTasks(TDCEC_DUE, FALSE);
		break;

	case ID_VIEW_EXPANDSTARTED:
		bEnable = tdc.CanExpandTasks(TDCEC_STARTED, TRUE);
		break;

	case ID_VIEW_COLLAPSESTARTED:
		bEnable = tdc.CanExpandTasks(TDCEC_STARTED, FALSE);
		break;

	default:
		ASSERT(0);
	}

	pCmdUI->Enable(bEnable);
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
// 	TRACE(_T("OnActivateApp(%s, last focus = %s)\n"),
// 		  bActive ? _T("TRUE") : _T("FALSE"),
// 		  CWinClasses::GetClassEx(m_hwndLastFocus));

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

	// Reload tasklists as required
	if (bActive)
		OnTimerCheckReloadTasklists(-1, TRUE);

	// Don't do any further processing if the Reminder window or Time tracker
	// is active because the two windows get into a fight for activation!
	if (m_dlgReminders.IsForegroundWindow() || m_dlgTimeTracker.IsForegroundWindow())
		return;

	if (!bActive)
	{
		// save tasklists if required
		if (Prefs().GetAutoSaveOnSwitchApp())
			SaveAll(TDLS_AUTOSAVE);
	}
	else
	{
		if (m_bPromptLanguageChangeRestartOnActivate)
		{
			m_bPromptLanguageChangeRestartOnActivate = FALSE;

			if (CMessageBox::AfxShow(IDS_RESTARTTOCHANGELANGUAGE, MB_YESNO) == IDYES)
			{
				DoExit(TRUE);
				return;
			}
		}
		
		PostAppRestoreFocus(TRUE);
		UpdateCwd();
	}
}

void CToDoListWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	if (GetTDCCount() == 0)
		return;

	switch (nState)
	{
	case WA_ACTIVE:
		if ((pWndOther == &m_dlgReminders) || (pWndOther == &m_dlgTimeTracker))
		{
			// Restore the focus if we are being reactivated as a consequence 
			// of closing the time tracker or the reminder window 
			PostAppRestoreFocus(TRUE);
		}
		break;
	}
}

LRESULT CToDoListWnd::OnAppRestoreFocus(WPARAM /*wp*/, LPARAM lp)
{
	HWND hWnd = (HWND)lp;
	
	if (!hWnd || (hWnd == *this))
	{
		// We should have prevented this
		ASSERT(0);
	}
	else if (GetTDCCount() && (hWnd == GetToDoCtrl().GetSafeHwnd()))
	{
		GetToDoCtrl().SetFocusToTasks();
	}
	else if (::IsWindowEnabled(hWnd) && ::IsWindowVisible(hWnd) && (::GetFocus() != hWnd))
	{
#ifdef _DEBUG
// 		CString sFocus, sClass = CWinClasses::GetClassEx(hWnd);
// 		CWnd::FromHandle(hWnd)->GetWindowText(sFocus);
// 		TRACE(_T("OnAppRestoreFocus(%s = %s)\n"), sClass, sFocus.Left(100));
#endif

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

void CToDoListWnd::OnEnable(BOOL bEnable) 
{
	CFrameWnd::OnEnable(bEnable);
	
	if (!bEnable)
	{
		// Save and hide time tracker if it is top-most
		if (m_dlgTimeTracker.GetSafeHwnd())
		{
			ASSERT(!m_bReshowTimeTrackerOnEnable);

			if (m_dlgTimeTracker.IsWindowVisible() && m_dlgTimeTracker.IsAlwaysOnTop())
			{
				m_bReshowTimeTrackerOnEnable = TRUE;
				m_dlgTimeTracker.ShowWindow(SW_HIDE);
			}
		}
	}
	else
	{
		// clear any pressed state
		m_toolbarMain.Invalidate(FALSE);

		if (m_toolbarCustom.GetSafeHwnd())
			m_toolbarCustom.Invalidate(FALSE);

		UpdateWindow();

		if (m_bReshowTimeTrackerOnEnable)
		{
			m_bReshowTimeTrackerOnEnable = FALSE;
			m_dlgTimeTracker.ShowWindow(SW_SHOW);
		}

		// then restore it when we are enabled
		PostAppRestoreFocus();
	}
}

void CToDoListWnd::OnViewSorttasklisttabs() 
{
	int nSel = m_mgrToDoCtrls.SortToDoCtrlsByName();
	SelectToDoCtrl(nSel, FALSE);
}

void CToDoListWnd::OnUpdateViewSorttasklisttabs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((GetTDCCount() > 1) && !Prefs().GetKeepTabsOrdered());
}

void CToDoListWnd::OnEditInctaskpercentdone() 
{
	GetToDoCtrl().IncrementSelectedTaskPercentDone(TRUE);
}

void CToDoListWnd::OnUpdateEditInctaskpercentdone(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanEditSelectedTask(TDCA_PERCENT));	
}

void CToDoListWnd::OnEditDectaskpercentdone() 
{
	GetToDoCtrl().IncrementSelectedTaskPercentDone(FALSE);
}

void CToDoListWnd::OnUpdateEditDectaskpercentdone(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanEditSelectedTask(TDCA_PERCENT));	
}

void CToDoListWnd::OnEditDectaskpriority() 
{
	GetToDoCtrl().IncrementSelectedTaskPriority(FALSE);
}

void CToDoListWnd::OnUpdateEditDectaskpriority(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanEditSelectedTask(TDCA_PRIORITY));	
}

void CToDoListWnd::OnEditInctaskpriority() 
{
	GetToDoCtrl().IncrementSelectedTaskPriority(TRUE);
}

void CToDoListWnd::OnUpdateEditInctaskpriority(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanEditSelectedTask(TDCA_PRIORITY));	
}

void CToDoListWnd::OnEditFlagtask() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	tdc.SetSelectedTaskFlag(!tdc.IsSelectedTaskFlagged());
}

void CToDoListWnd::OnUpdateEditFlagtask(CCmdUI* pCmdUI) 
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	pCmdUI->Enable(tdc.CanEditSelectedTask(TDCA_FLAG));	
	pCmdUI->SetCheck(tdc.IsSelectedTaskFlagged() ? 1 : 0);
}

void CToDoListWnd::OnEditLocktask() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	tdc.SetSelectedTaskLock(!tdc.IsSelectedTaskLocked());
}

void CToDoListWnd::OnUpdateEditLocktask(CCmdUI* pCmdUI) 
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();

	pCmdUI->Enable(tdc.CanEditSelectedTask(TDCA_LOCK));	
	pCmdUI->SetCheck(tdc.IsSelectedTaskLocked() ? 1 : 0);
}

void CToDoListWnd::OnEditGotoDependency() 
{
	GetToDoCtrl().GotoSelectedTaskDependency();
}

void CToDoListWnd::OnUpdateEditGotoDependency(CCmdUI* pCmdUI) 
{
	CTDCDependencyArray aDepends;

	pCmdUI->Enable(GetToDoCtrl().GetSelectedTaskDependencies(aDepends) > 0);	
}

void CToDoListWnd::OnEditDependency()
{
	GetToDoCtrl().EditSelectedTaskDependency();
}

void CToDoListWnd::OnUpdateEditDependency(CCmdUI* pCmdUI)
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();

	pCmdUI->Enable(tdc.CanEditSelectedTask(TDCA_DEPENDENCY));
}

void CToDoListWnd::OnEditRecurrence() 
{
	GetToDoCtrl().EditSelectedTaskRecurrence();	
}

void CToDoListWnd::OnUpdateEditRecurrence(CCmdUI* pCmdUI) 
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();

	pCmdUI->Enable(tdc.CanEditSelectedTask(TDCA_RECURRENCE) && !tdc.IsSelectedTaskDone());	
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

void CToDoListWnd::OnViewShowfilterbar() 
{
	m_bShowFilterBar = !m_bShowFilterBar;

	if (m_bShowFilterBar)
		RefreshFilterBarControls(TDCA_ALL);

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
	
		RefreshFilterBarControls(TDCA_ALL, TRUE); // clear checkbox history

		UpdateStatusBar();
		UpdateTimeTrackerTasks(TRUE);
	}
}

void CToDoListWnd::OnUpdateViewClearfilter(CCmdUI* pCmdUI) 
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	pCmdUI->Enable(tdc.HasAnyFilter() || tdc.CanToggleFilter());
}

void CToDoListWnd::OnViewTogglefilter()
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	tdc.ToggleFilter();

	RefreshFilterBarControls(TDCA_ALL);

	UpdateStatusBar();
	UpdateTimeTrackerTasks(TRUE);
}

void CToDoListWnd::OnUpdateViewTogglefilter(CCmdUI* pCmdUI)
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	BOOL bCanToggle = tdc.CanToggleFilter();

	pCmdUI->Enable(bCanToggle);

	// Select toolbar button if filter is currently toggled
	if (pCmdUI->m_pMenu == NULL)
		pCmdUI->SetCheck((bCanToggle && tdc.HasAnyFilter()) ? 0 : 1);
}

LRESULT CToDoListWnd::OnSelchangeFilter(WPARAM wp, LPARAM lp) 
{
	int nCtrlID = (int)wp;
	HWND hWnd = (HWND)lp;

	ASSERT((nCtrlID == m_filterBar.GetDlgCtrlID()) && (hWnd == m_filterBar));

	TDCFILTER filter;
	CString sCustom;
	DWORD dwCustomFlags;

	m_filterBar.GetFilter(filter, sCustom, dwCustomFlags);

	// Refresh filter controls if we've switched 
	// from a custom to default filter or vice versa
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	BOOL bUpdateFilterCtrls = Misc::StateChanged(sCustom.IsEmpty(), !tdc.HasAdvancedFilter());

	OnChangeFilter(filter, sCustom, dwCustomFlags, bUpdateFilterCtrls);

	return 0L;
}

void CToDoListWnd::OnChangeFilter(TDCFILTER& filter, const CString& sCustom, DWORD dwCustomFlags, BOOL bUpdateFilterCtrls)
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	if (!sCustom.IsEmpty())
	{
		// Synchronise 'Find Tasks' dialog flags
		VERIFY(m_findFilterHelper.UpdateFindDlgAdvancedFilter(sCustom, dwCustomFlags));

		// Update filter
		TDCADVANCEDFILTER filter(sCustom, dwCustomFlags);
		VERIFY(m_dlgFindTasks.GetSearchParams(sCustom, filter));

		tdc.SetAdvancedFilter(filter);
	}
	else if (filter.IsSet())
	{
		tdc.SetFilter(filter);
	}
	else
	{
		tdc.ClearFilter();
	}

	if (bUpdateFilterCtrls)
		RefreshFilterBarControls(TDCA_ALL);
	else
		CheckResizeFilterBar();

	UpdateStatusBar();
	UpdateTimeTrackerTasks(TRUE);
}

void CToDoListWnd::OnViewFilter() 
{
	const CPreferencesDlg& prefs = Prefs();

	CDWordArray aPriorityColors;
	prefs.GetPriorityColors(aPriorityColors);
	
	CTDLFilterDlg dialog(prefs.GetTitleFilterOption(),
						 prefs.GetMultiSelFilters(),
						 m_filterBar.GetAdvancedFilterNames(),
						 GetToDoCtrl(),
						 aPriorityColors);

	if (dialog.DoModal() == IDOK)
	{
		TDCFILTER filter;
		CString sCustom;
		DWORD dwCustomFlags = 0;
		
		dialog.GetFilter(filter, sCustom, dwCustomFlags);
		OnChangeFilter(filter, sCustom, dwCustomFlags, TRUE);
	}
}

void CToDoListWnd::OnUpdateViewFilter(CCmdUI* pCmdUI) 
{
	UNREFERENCED_PARAMETER(pCmdUI);
}

void CToDoListWnd::OnViewRefreshfilter() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	// if the filter has changed then set the new one else
	// refresh the current one
	TDCFILTER filter;
	CString sCustom;
	DWORD dwCustomFlags;

	m_filterBar.GetFilter(filter, sCustom, dwCustomFlags);
	
	if (tdc.FilterMatches(filter, sCustom, dwCustomFlags))
	{
		tdc.RefreshFilter();
	}
	else
	{
		OnChangeFilter(filter, sCustom, dwCustomFlags, FALSE);

		if (Prefs().GetExpandTasksOnLoad())
			tdc.ExpandTasks(TDCEC_ALL);
	}

	UpdateStatusBar();
}

void CToDoListWnd::OnUpdateViewRefreshfilter(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().HasAnyFilter());
}

void CToDoListWnd::OnTabctrlPreferences() 
{
	DoPreferences(PREFPAGE_UI, IDC_TABBARGROUP);
}

void CToDoListWnd::OnTasklistSelectColumns() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	int nSel = GetSelToDoCtrl();

	TDCCOLEDITFILTERVISIBILITY vis, visDefault;

	tdc.GetColumnFieldVisibility(vis);
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

		if (dialog.GetApplyActiveTasklist())
		{
			tdc.SetColumnFieldVisibility(vis);
		}
		else
		{
			// and apply to all tasklists
			int nTDC = GetTDCCount();

			while (nTDC--)
				GetToDoCtrl(nTDC).SetColumnFieldVisibility(vis);
		}

		RefreshFilterBarControls(TDCA_ALL);
		UpdateStatusBar(); // Time Est/Spent, Cost visibility may have changed

		// reload the menu if we dynamically alter it
		if (Prefs().GetShowEditMenuAsColumns())
			InitMenubar();

		Resize();
	}
}

void CToDoListWnd::OnViewProjectname() 
{
	ToggleActiveTasklistStyle(TDCS_SHOWPROJECTNAME, m_bShowProjectName);
}

void CToDoListWnd::OnUpdateViewProjectname(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bShowProjectName ? 1 : 0);
}

void CToDoListWnd::OnEditOffsetDates() 
{
	CTDLOffsetDatesDlg dialog;
	
	if (dialog.DoModal() == IDOK)
	{
		DWORD dwWhat = dialog.GetOffsetWhat();
		BOOL bSubtasks = dialog.GetOffsetSubtasks();
		BOOL bFromToday = dialog.GetOffsetFromToday();

		TDC_UNITS nUnits = TDCU_NULL;
		int nAmount = dialog.GetOffsetAmount(nUnits);
		
		if (!nAmount && !bFromToday)
			return;

		ASSERT(nUnits != TDCU_NULL);
		
		// do the offsets
		CFilteredToDoCtrl& tdc = GetToDoCtrl();
		
		if (dwWhat & ODD_STARTDATE)
		{
			// special case: Moving due dates as well
			if (dwWhat & ODD_DUEDATE)
				tdc.OffsetSelectedTaskStartAndDueDates(nAmount, nUnits, bSubtasks, bFromToday);
			else
				tdc.OffsetSelectedTaskDate(TDCD_START, nAmount, nUnits, bSubtasks, bFromToday);
		}
		else if (dwWhat & ODD_DUEDATE)
		{
			tdc.OffsetSelectedTaskDate(TDCD_DUE, nAmount, nUnits, bSubtasks, bFromToday);
		}
		
		if (dwWhat & ODD_DONEDATE)
		{
			tdc.OffsetSelectedTaskDate(TDCD_DONE, nAmount, nUnits, bSubtasks, bFromToday);
		}
		
		if (dwWhat & ODD_REMINDER)
		{
			CDWordArray aTaskIDs;
			DWORD dwUnused;

			int nTask = tdc.GetSelectedTaskIDs(aTaskIDs, dwUnused, bSubtasks);

			while (nTask--)
				m_dlgReminders.OffsetReminder(aTaskIDs[nTask], nAmount, nUnits, &tdc, bSubtasks, bFromToday);
		}
	}
}

void CToDoListWnd::OnEditOffsetStartDueDatesForwards(UINT nCmdID)
{
	TDC_UNITS nUnits = TDCU_NULL;

	switch (nCmdID)
	{
	case ID_OFFSETDATES_FORWARDSBY_ONEDAY:		nUnits = TDCU_DAYS;		break;
	case ID_OFFSETDATES_FORWARDSBY_ONEWEEKDAY:	nUnits = TDCU_WEEKDAYS; break;
	case ID_OFFSETDATES_FORWARDSBY_ONEWEEK:		nUnits = TDCU_WEEKS;	break;
	case ID_OFFSETDATES_FORWARDSBY_ONEMONTH:	nUnits = TDCU_MONTHS;	break;
	case ID_OFFSETDATES_FORWARDSBY_ONEYEAR:		nUnits = TDCU_YEARS;	break;

	default:
		ASSERT(0);
		return;
	}

	GetToDoCtrl().OffsetSelectedTaskStartAndDueDates(1, nUnits, FALSE, FALSE);
}

void CToDoListWnd::OnEditOffsetStartDueDatesBackwards(UINT nCmdID)
{
	TDC_UNITS nUnits = TDCU_NULL;

	switch (nCmdID)
	{
	case ID_OFFSETDATES_BACKWARDSBY_ONEDAY:		nUnits = TDCU_DAYS;		break;
	case ID_OFFSETDATES_BACKWARDSBY_ONEWEEKDAY:	nUnits = TDCU_WEEKDAYS; break;
	case ID_OFFSETDATES_BACKWARDSBY_ONEWEEK:	nUnits = TDCU_WEEKS;	break;
	case ID_OFFSETDATES_BACKWARDSBY_ONEMONTH:	nUnits = TDCU_MONTHS;	break;
	case ID_OFFSETDATES_BACKWARDSBY_ONEYEAR:	nUnits = TDCU_YEARS;	break;

	default:
		ASSERT(0);
		return;
	}

	GetToDoCtrl().OffsetSelectedTaskStartAndDueDates(-1, nUnits, FALSE, FALSE);
}

void CToDoListWnd::OnUpdateEditOffsetDates(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanEditSelectedTask(TDCA_STARTDATE));	
}

void CToDoListWnd::OnUpdateEditOffsetDatesForwards(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanOffsetSelectedTaskStartAndDueDates());	
}

void CToDoListWnd::OnUpdateEditOffsetDatesBackwards(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanOffsetSelectedTaskStartAndDueDates());	
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
	int nTasks = GetToDoCtrl().GetSelectedTaskCount();
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
	BOOL bEnable = (Prefs().GetLogTimeTracking() && (GetToDoCtrl().GetSelectedTaskCount() == 1));
	pCmdUI->Enable(bEnable);	
}

void CToDoListWnd::OnToolsAnalyseLoggedTime() 
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	const CPreferencesDlg& userPrefs = Prefs();
	const CTDCCustomAttribDefinitionArray& aCustAttribDefs = tdc.GetCustomAttributeDefs();

	CString sTaskFile(tdc.GetFilePath());
	CTDLAnalyseLoggedTimeDlg dialog(sTaskFile, aCustAttribDefs);

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
				
				CTDCTaskTimeLogAnalysis ttla(sTaskFile, aCustAttribDefs);

				// Check all the files are accessible
				CStringArray aLogFiles, aLockedPaths;

				VERIFY(ttla.GetLogFilePaths(aLogFiles));
				aLogFiles.Add(dialog.GetOutputFilepath());

				while (FileMisc::GetNonWritableFiles(aLogFiles, aLockedPaths))
				{
					if (IDCANCEL == CMessageBox::AfxShow(IDS_ERROR_LOCKEDFILES, Misc::FormatArray(aLockedPaths, _T("\n\n")), MB_ICONERROR | MB_RETRYCANCEL))
						return;
				}

				// Do the analysis
				if (ttla.AnalyseTaskLog(dtFrom, dtTo, 
										dialog.GetBreakdown(), 
										dialog.GetGroupBy(),
										dialog.GetOutputFormat(), 
										dialog.GetOutputFilepath()))
				{
					// Show the report
					if (dialog.GetAutoDisplayReport() ||
						CMessageBox::AfxShow(IDS_LOGANALYSIS_QUERYOPEN, MB_YESNO) == IDYES)
					{
						FileMisc::Run(*this, dialog.GetOutputFilepath());
					}

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
				CMessageBox::AfxShow(IDS_LOGANALYSIS_ERRNOACCESS, MB_ICONWARNING);
				break;

			default: // no tasks matching
				bContinue = (CMessageBox::AfxShow(IDS_LOGANALYSIS_ERRNOTASKS, MB_YESNO) == IDYES);
				break;
			}
		}
	}
}

void CToDoListWnd::OnUpdateToolsAnalyseLoggedTime(CCmdUI* pCmdUI) 
{
	BOOL bEnable = FALSE;

	if (Prefs().GetLogTimeTracking())
	{
		CString sLogPath = GetToDoCtrl().GetSelectedTaskTimeLogPath();
		bEnable = FileMisc::FileExists(sLogPath);
	}
	
	pCmdUI->Enable(bEnable);
}

LRESULT CToDoListWnd::OnToDoCtrlDoLengthyOperation(WPARAM wParam, LPARAM lParam)
{
	ASSERT(!wParam || lParam);

	if (wParam)
		m_statusBar.BeginProgress((LPCTSTR)lParam);
	else
		m_statusBar.EndProgress();
	
	return 0L;
}

BOOL CToDoListWnd::DoTaskLink(const CString& sPath, DWORD dwTaskID, BOOL bStartup)
{
	BOOL bHandled = FALSE, bSelected = FALSE;

	// handle no file path => active tasklist
	if (sPath.IsEmpty())
	{
		ASSERT(dwTaskID);

		bSelected = GetToDoCtrl().SelectTask(dwTaskID, TRUE);
		bHandled = TRUE; // handled regardless of result
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
				bSelected = TRUE;

				if (dwTaskID)
					GetToDoCtrl().SelectTask(dwTaskID, TRUE);
			}
			else
			{
				ASSERT(0);
			}

			bHandled = TRUE; // handled regardless of result
		}
		else if (bStartup || !Prefs().GetMultiInstance())
		{
			TDC_FILE nRet = OpenTaskList(sPath);
			
			if (nRet == TDCF_SUCCESS)
			{
				bSelected = TRUE;

				if (dwTaskID)
					GetToDoCtrl().SelectTask(dwTaskID, TRUE);
			}
			else
			{
				HandleLoadTasklistError(nRet, sPath);
			}

			bHandled = TRUE; // handled regardless of result
		}
	}
	else
	{
		ASSERT(0);
	}

	if (!bStartup && bSelected)
		Show(FALSE);

	return bHandled;
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
			FileMisc::MakeFullPath(sPath, m_mgrToDoCtrls.GetFolderPath(GetSelToDoCtrl()));
		}
		else
		{
			FileMisc::MakeFullPath(sPath, FileMisc::GetModuleFolder());
		}
	}

	return FileMisc::FileExists(sPath);
}

LRESULT CToDoListWnd::OnToDoCtrlSelectTask(WPARAM wParam, LPARAM lParam)
{
	DWORD dwTaskID = wParam;
	CString sPath((LPCTSTR)lParam);

	// We should only receive this from CToDoCtrlReminders
	// or from CFilteredToDoCtrl because it couldn't handle it.
	// In both cases there should be a path 
	ASSERT(!sPath.IsEmpty());

	if (!ValidateTaskLinkFilePath(sPath))
	{
		CMessageBox::AfxShow(CEnString(IDS_TASKLISTNOTFOUND, sPath));
		return FALSE;
	}

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

LRESULT CToDoListWnd::OnReminderCompleteTask(WPARAM wParam, LPARAM lParam)
{
	DWORD dwTaskID = wParam;
	CString sPath((LPCTSTR)lParam);

	// We should only receive this from CToDoCtrlReminders so there should be a path 
	ASSERT(!sPath.IsEmpty());

	if (ValidateTaskLinkFilePath(sPath) && DoTaskLink(sPath, dwTaskID, FALSE))
	{
		GetToDoCtrl().SetSelectedTaskCompletion(TDCTC_DONE);
	}

	return 0L;
}

LRESULT CToDoListWnd::OnToDoCtrlGetLinkTooltip(WPARAM wParam, LPARAM lParam)
{
	LPCTSTR szLink = (LPCTSTR)lParam;

	static CEnString sTooltip;
	sTooltip.Empty();

	if (CMSOutlookHelper::IsOutlookUrl(szLink))
	{
		CString sItemID(szLink);
		sItemID.Replace(_T("outlook:"), _T(""));

		if (!Misc::Trim(sItemID).IsEmpty() && (sItemID[0] != '/'))
		{
			// Outlook pops up an error message which is a pain for
			// the user if the issue is that they archive old messages
			CMessageBox::DisableSimpleErrorMessages(TRUE);

			CMSOutlookHelper outlook;
			OutlookAPI::_Item* pItem = outlook.GetItemByID(sItemID);

			if (pItem)
				sTooltip = outlook.GetItemData(*pItem, OA_TITLE);

			CMessageBox::DisableSimpleErrorMessages(FALSE);
		}
	}
	else // see if it's a task link
	{
		CString sPath, sCwd(m_mgrToDoCtrls.GetFolderPath(GetSelToDoCtrl()));
		DWORD dwTaskID = 0;

		if (CFilteredToDoCtrl::ParseTaskLink(szLink, TRUE, sCwd, dwTaskID, sPath))
		{
			if (sPath.IsEmpty())
			{
				if (GetToDoCtrl().HasTask(dwTaskID))
					sTooltip = GetToDoCtrl().GetTaskTitle(dwTaskID);
				else 
					sTooltip.LoadString(IDS_GOTOTASK_NOSUCHTASK);
			}
			else
			{
				int nTDC = m_mgrToDoCtrls.FindToDoCtrl(sPath);

				if (nTDC != -1)
				{
					if (m_mgrToDoCtrls.IsLoaded(nTDC))
					{
						if (GetToDoCtrl(nTDC).HasTask(dwTaskID))
							sTooltip = GetToDoCtrl(nTDC).GetTaskTitle(dwTaskID);
						else
							sTooltip.LoadString(IDS_GOTOTASK_NOSUCHTASK);
					}
					else
					{
						sTooltip.Format(IDS_TABTIP_NOTLOADED, sPath);
					}
				}
			}
		}
	}

	if (!sTooltip.IsEmpty())
		return (LRESULT)(LPCTSTR)sTooltip;

	// all else
	return 0;
}

LRESULT CToDoListWnd::OnToDoCtrlFailedLink(WPARAM wParam, LPARAM lParam)
{
	LPCTSTR szLink = (LPCTSTR)lParam;

	// if it's an Outlook link then run it directly
	if (CMSOutlookHelper::IsOutlookUrl(szLink))
	{
		if (!CMSOutlookHelper::IsOutlookInstalled())
		{
			CMessageBox::AfxShow(IDS_ERROROUTLOOKNOTINSTALLED);
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

			if (tdc.ParseTaskLink(szLink, TRUE, dwTaskID, sPath))
			{
				return OnToDoCtrlSelectTask(dwTaskID, (LPARAM)(LPCTSTR)sPath);
			}
		}
		// else fall thru for generic error message
	}

	// all else
	CMessageBox::AfxShow(IDS_COMMENTSGOTOERRMSG);
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
		UpdateStatusBar();
	else
		UpdateCaption();
}

void CToDoListWnd::OnUpdateViewStatusBar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bShowStatusBar ? 1 : 0);
}

BOOL CToDoListWnd::OnQueryOpen() 
{
	if (CFrameWnd::OnQueryOpen())
	{
		// fail if the active tasklist is encrypted because we have to verify the password
		// and we're not allowed to display a dialog in this message handler
		const CFilteredToDoCtrl& tdc = GetToDoCtrl();

		if (!m_bQueryOpenAllow && tdc.IsEncrypted() && tdc.WantPasswordReprompting())
		{
			PostMessage(WM_TDL_PASSWORDRESTORE); 
			return FALSE;
		}
		
		// all others
		return TRUE;
	}
	
	return FALSE;
}

LRESULT CToDoListWnd::OnToDoListPasswordRestore(WPARAM /*wp*/, LPARAM /*lp*/)
{ 
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();

    if (IsIconic() && tdc.IsEncrypted() && tdc.WantPasswordReprompting())
	{
		if (VerifyToDoCtrlPassword())
		{
			CAutoFlag af(m_bQueryOpenAllow, TRUE);
			ShowWindow(SW_RESTORE);

			// Repost 'focus-restore' because the call to ShowWindow
			// above will have overwritten its previous effect
			PostAppRestoreFocus();
		}
	}
	else
	{
		ASSERT(0); // sanity check
	}

	return 0L;
}

void CToDoListWnd::OnCopyTaskasLink() 
{
	CopySelectedTasksToClipboard(TDCTC_ASLINK);
}

void CToDoListWnd::OnUpdateCopyTaskasLink(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().GetSelectedTaskCount() == 1);
}

void CToDoListWnd::OnCopyTaskasDependency() 
{
	CopySelectedTasksToClipboard(TDCTC_ASDEPENDS);
}

void CToDoListWnd::OnUpdateCopyTaskasDependency(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().GetSelectedTaskCount() == 1);
}

void CToDoListWnd::OnCopyTaskasLinkFull() 
{
	CopySelectedTasksToClipboard(TDCTC_ASLINKFULL);
}

void CToDoListWnd::OnUpdateCopyTaskasLinkFull(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().GetSelectedTaskCount() == 1);
}

void CToDoListWnd::OnCopyTaskasDependencyFull() 
{
	CopySelectedTasksToClipboard(TDCTC_ASDEPENDSFULL);
}

void CToDoListWnd::OnUpdateCopyTaskasDependencyFull(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().GetSelectedTaskCount() == 1);
}

void CToDoListWnd::OnCopyTaskasPath() 
{
	CopySelectedTasksToClipboard(TDCTC_ASPATH);
}

void CToDoListWnd::OnUpdateCopyTaskasPath(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().GetSelectedTaskCount() == 1);
}

BOOL CToDoListWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (CFrameWnd::PreCreateWindow(cs))
	{
		cs.style |= CS_OWNDC;
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
			//VERIFY(m_iconClass.Load(IDR_MAINFRAME));
			//wndcls.hIcon = m_iconClass;
			wndcls.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

			// Register our class now and check the outcome
			if (!::RegisterClass(&wndcls))
			{
				ASSERT(0);

				//m_iconClass.Destroy();
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

void CToDoListWnd::OnUpdateEditInsertDateAndOrTime(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CanInsertDateAndTime());
}

void CToDoListWnd::OnUpdateCommentsInsertDateAndOrTime(CCmdUI* pCmdUI)
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();

	pCmdUI->Enable(tdc.CommentsHaveFocus() && tdc.CanPasteDateTime());
}

BOOL CToDoListWnd::CanInsertDateAndTime() const
{
	if (GetToDoCtrl().CanPasteDateTime())
		return TRUE;

	// Check other controls outside of CToDoCtrl
	if (m_bShowFilterBar && m_filterBar.CanPasteText())
		return TRUE;

	if (m_bShowingMainToolbar && CDialogHelper::IsChildOrSame(m_cbQuickFind, ::GetFocus()))
		return TRUE;

	return FALSE;
}

void CToDoListWnd::OnSysColorChange() 
{
	CFrameWnd::OnSysColorChange();
	
	UpdateUITheme();
}

void CToDoListWnd::UpdateStatusBar(const CTDCAttributeMap& mapAttrib)
{
	if (m_bShowStatusBar && m_statusBar.GetSafeHwnd() && GetTDCCount())
	{
		m_statusBar.UpdateTasks(GetToDoCtrl(), mapAttrib);
	}
}

void CToDoListWnd::OnEditSelectall() 
{
	GetToDoCtrl().SelectAll();
}

void CToDoListWnd::OnUpdateEditSelectall(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanSelectAll());	
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
				break; // user cancelled

			const CFilteredToDoCtrl& tdc = GetToDoCtrl(nCtrl);

			m_dlgFindTasks.DeleteResults(&tdc);
			m_dlgTimeTracker.RemoveTasklist(&tdc);
			m_dlgReminders.RemoveToDoCtrl(&tdc);

			m_mgrToDoCtrls.DeleteToDoCtrl(nCtrl);
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
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	CTDLSendTasksDlg dialog(m_mgrImportExport, 
							bSelected, 
							(tdc.GetTaskView() != FTCV_TASKLIST), 
							tdc.GetCustomAttributeDefs());

	if (dialog.DoModal() == IDOK)
	{
		int nFormat = m_mgrImportExport.FindExporterByType(dialog.GetFormatTypeID());
		ASSERT(nFormat != -1);

		CString sExt = Misc::ToLower(m_mgrImportExport.GetExporterFileExtension(nFormat, TRUE));
		CString sFilePath = FileMisc::GetTempFilePath(_T("tdl.email"), sExt);

		// get tasks
		BOOL bHtmlComments = m_mgrImportExport.ExporterSupportsHtmlComments(nFormat);
		CString sHtmlImgFolder = GetHtmlImageFolder(bHtmlComments, sFilePath);

		const CTaskSelectionDlg& taskSel = dialog.GetTaskSelection();

		CTaskFile tasks;
		GetTasks(tdc, bHtmlComments, sHtmlImgFolder, FALSE, taskSel, tasks);

		// Export them
		DWORD dwFlags = TDC::MapPrintToExportStyle(dialog.GetHtmlStyle());

		if (m_mgrImportExport.ExportTaskList(tasks, sFilePath, nFormat, dwFlags) != IIER_SUCCESS)
		{
			// Display error message
			// TODO
			return;
		}

		// package them up
		CString sSubject = tdc.GetFriendlyProjectName();
		CString sAttachment, sBody;

		switch (dialog.GetSendAs())
		{
		case TDSA_TASKLIST:
			sAttachment = sFilePath;
			sBody.LoadString(IDS_TASKLISTATTACHED);
			break;
			
		case TDSA_BODYTEXT:
			if ((sExt == _T(".html")) || (sExt == _T(".htm")))
			{
				sAttachment = sFilePath;
				// empty body instructs email client to render the 
				// attached file in the body
			}
			else if (!FileMisc::LoadFile(sFilePath, sBody))
			{
				// Display error message
				// TODO
				return;
			}
			break;
		}
		
		// recipients
		CStringArray aTo;
		tdc.GetSelectedTaskAllocTo(aTo);

		CString sTo = Misc::FormatArray(aTo, _T(";"));

		// prefix with task name if necessary
		if (taskSel.GetWantSelectedTasks() && (tdc.GetSelectedTaskCount() == 1))
		{
			sSubject = tdc.FormatSelectedTaskTitles(FALSE) + _T(" - ") + sSubject;
		}

		VERIFY(CSendFileToEx::SendMail(*this, sTo, sSubject, sBody, sAttachment));

		FileMisc::DeleteFile(sFilePath, TRUE);
	}
}

void CToDoListWnd::OnSendTasks() 
{
	DoSendTasks(FALSE);
}

void CToDoListWnd::OnUpdateSendTasks(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().GetTaskCount());
}

void CToDoListWnd::OnSendSelectedTasks() 
{
	DoSendTasks(TRUE);
}

void CToDoListWnd::OnUpdateSendSelectedTasks(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().GetSelectedTaskCount());
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
	
	UpdateStatusBar();
	UpdateTimeTrackerTasks(TRUE);
}

void CToDoListWnd::OnUpdateEditUndoRedo(CCmdUI* pCmdUI, BOOL bUndo)
{
	pCmdUI->Enable(GetToDoCtrl().CanUndoLastAction(bUndo));
}

void CToDoListWnd::OnViewCycleTaskViews() 
{
	GetToDoCtrl().SetNextTaskView();
}

void CToDoListWnd::OnUpdateViewCycleTaskViews(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_nMaxState != TDCMS_MAXCOMMENTS);
}

void CToDoListWnd::OnViewToggleTreeandList() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	switch (tdc.GetTaskView())
	{
	case FTCV_TASKTREE:
		tdc.SetTaskView(FTCV_TASKLIST);
		break;

	case FTCV_TASKLIST:
	default:
		tdc.SetTaskView(FTCV_TASKTREE);
		break;
	}
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
	pCmdUI->Enable(GetToDoCtrl().CanEditSelectedTask(TDCA_ICON));	
}

LRESULT CToDoListWnd::OnToDoCtrlGetTaskReminder(WPARAM wParam, LPARAM lParam)
{
	DWORD dwTaskID = lParam;

	if (!dwTaskID)
	{
		ASSERT(0);
		return 0L;
	}

	int nTDC = m_mgrToDoCtrls.FindToDoCtrl((HWND)wParam);

	if (nTDC == -1)
	{
		// Currently there is no way to detect whether the 
		// requester is bogus or just a tasklist not yet added 
		// ASSERT(m_bReloading);
		return 0L;
	}

	const CFilteredToDoCtrl& tdc = GetToDoCtrl(nTDC);
	int nRem = m_dlgReminders.FindReminder(dwTaskID, &tdc, FALSE);

	if (nRem == -1)
		return 0;

	COleDateTime dtRem;

	if (!m_dlgReminders.GetReminderDate(nRem, dtRem))
		return -1; // means the task's start/due date has not yet been set

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

BOOL CToDoListWnd::GetFirstTaskReminder(const CFilteredToDoCtrl& tdc, const CDWordArray& aTaskIDs, TDCREMINDER& rem) const
{
	int nNumSel = aTaskIDs.GetSize();

	for (int nTask = 0; nTask < nNumSel; nTask++)
	{
		DWORD dwTaskID = aTaskIDs[nTask];
		int nRem = m_dlgReminders.FindReminder(dwTaskID, &tdc);

		if (nRem != -1)
		{
			m_dlgReminders.GetReminder(nRem, rem);
			return TRUE;
		}
	}

	// no task has a reminder
	return FALSE;
}

void CToDoListWnd::OnEditSetReminder() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	if (!tdc.HasSelection())
	{
		ASSERT(0);
		return;
	}
	
	CDWordArray aTaskIDs;
	int nNumSel = tdc.GetSelectedTaskIDs(aTaskIDs, TRUE);

	DWORD dwFlags = (nNumSel == 1) ? 0 : TDCREM_MULTIPLETASKS;
	
	// get the first reminder as a reference
	TDCREMINDER rem;
	
	if (!GetFirstTaskReminder(tdc, aTaskIDs, rem))
	{
		// handle new reminder
		rem.dwTaskID = aTaskIDs[0];
		rem.pTDC = &tdc;

		dwFlags |= TDCREM_NEWREMINDER;
	}

	int nRet = CTDLSetReminderDlg().DoModal(rem, dwFlags);

	switch (nRet)
	{
	case IDOK:
		{
			// apply reminder to selected tasks
			for (int nTask = 0; nTask < nNumSel; nTask++)
			{
				rem.dwTaskID = aTaskIDs[nTask];
				m_dlgReminders.SetReminder(rem);
			}
			m_dlgReminders.CheckReminders();
		}
		break;
		
	case IDDISMISS:
		{
			// clear reminder for selected tasks
			for (int nTask = 0; nTask < nNumSel; nTask++)
			{
				m_dlgReminders.ClearReminder(aTaskIDs[nTask], &tdc);
			}
		}
		break;

	default:
		return;
	}
	
	tdc.RedrawReminders();
}

void CToDoListWnd::OnUpdateEditSetReminder(CCmdUI* pCmdUI) 
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	BOOL bEnable = (tdc.HasSelection() && !tdc.SelectedTasksAreAllDone());
	pCmdUI->Enable(bEnable);

	if (bEnable && pCmdUI->m_pMenu)
	{
		CDWordArray aTaskIDs;
		VERIFY(tdc.GetSelectedTaskIDs(aTaskIDs, TRUE));
	
		// get the first reminder as a reference
		TDCREMINDER rem;
		BOOL bNewReminder = !GetFirstTaskReminder(tdc, aTaskIDs, rem);

		pCmdUI->SetText(CEnString(bNewReminder ? IDS_SETREMINDER : IDS_MODIFYREMINDER));
	}
}

void CToDoListWnd::OnEditClearReminder() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	CDWordArray aTaskIDs;
	int nTask = tdc.GetSelectedTaskIDs(aTaskIDs, TRUE);
	
	while (nTask--)
	{
		DWORD dwTaskID = aTaskIDs[nTask];
		m_dlgReminders.ClearReminder(dwTaskID, &tdc);
	}
	
	tdc.RedrawReminders();
}

void CToDoListWnd::OnUpdateEditClearReminder(CCmdUI* pCmdUI) 
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	// check at least one selected item has a reminder
	CDWordArray aTaskIDs;
	tdc.GetSelectedTaskIDs(aTaskIDs, TRUE);
	
	TDCREMINDER rem;
	pCmdUI->Enable(GetFirstTaskReminder(tdc, aTaskIDs, rem));
}

void CToDoListWnd::OnEditCleartaskicon() 
{
	GetToDoCtrl().ClearSelectedTaskIcon();
}

void CToDoListWnd::OnUpdateEditCleartaskicon(CCmdUI* pCmdUI) 
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	pCmdUI->Enable(tdc.CanEditSelectedTask(TDCA_ICON) && tdc.SelectedTasksHaveIcons());	
}

void CToDoListWnd::OnSortMulti() 
{
	TDSORTCOLUMNS sort;
	CTDCCustomAttribDefinitionArray aAttribDefs;

	CFilteredToDoCtrl& tdc = GetToDoCtrl();

	tdc.GetSortBy(sort);
	tdc.GetCustomAttributeDefs(aAttribDefs);

	CTDCColumnIDMap mapColIDs;
	VERIFY(tdc.GetSortableColumns(mapColIDs));

	CTDLMultiSortDlg dialog(sort, mapColIDs, aAttribDefs);

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
	ToggleActiveTasklistStyle(TDCS_SHOWTREELISTBAR, m_bShowTreeListBar);
}

void CToDoListWnd::ToggleActiveTasklistStyle(TDC_STYLE nStyle, BOOL& bWantStyle)
{
	bWantStyle = !bWantStyle;

	int nSelTDC = GetSelToDoCtrl();
	BOOL bHasStyle = GetToDoCtrl().HasStyle(nStyle);

	// Sanity checks
	ASSERT(!m_mgrToDoCtrls.GetNeedsPreferenceUpdate(nSelTDC));
	ASSERT((bWantStyle && !bHasStyle) || (!bWantStyle && bHasStyle));

	// Update active tasklist
	GetToDoCtrl().ModifyStyles(CTDCStyleMap(nStyle, bWantStyle));

	// mark all other tasklists as needing update
	m_mgrToDoCtrls.SetAllNeedPreferenceUpdate(TRUE, nSelTDC);
}

void CToDoListWnd::OnUpdateViewShowTreeListTabbar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bShowTreeListBar ? 1 : 0);
}

void CToDoListWnd::OnFileChangePassword() 
{
	int nSelTDC = GetSelToDoCtrl();
	CFilteredToDoCtrl& tdc = GetToDoCtrl(nSelTDC);

	// Force the user to re-enter the current password
	if (!tdc.IsReadOnly() && tdc.IsEncrypted() && m_mgrToDoCtrls.VerifyPassword(nSelTDC, TRUE))
	{
		tdc.ModifyPassword();
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
		CTDLCustomAttributeDlg dialog(tdc.GetFilePath(),
									  tdc.GetCustomAttributeDefs(),
									  tdc.GetTaskIconImageList(),
									  tdc.GetCheckImageList());

		if (dialog.DoModal() == IDOK)
		{
			BOOL bAnyHasInheritance = tdc.GetCustomAttributeDefs().AnyHasFeature(TDCCAF_INHERITPARENTCHANGES);

			// Ignore modification callback if it came from us
			CAutoFlag af(m_bSettingAttribDefs, TRUE);

			CTDCCustomAttribDefinitionArray aAttrib;
			dialog.GetAttributeDefinitions(aAttrib);

			if (tdc.SetCustomAttributeDefs(aAttrib))
			{
				RefreshFilterBarControls(TDCA_ALL);
				UpdateFindDialogActiveTasklist();

				// Auto-enable attribute inheritance first time only
				if (!bAnyHasInheritance && aAttrib.AnyHasFeature(TDCCAF_INHERITPARENTCHANGES))
				{
					if (m_pPrefs->EnableCustomAttributeInheritance())
					{
						// update inherited attributes
						CTDCAttributeMap mapParentAttrib;
						BOOL bUpdateAttrib;

						Prefs().GetInheritParentAttributes(mapParentAttrib, bUpdateAttrib);
						tdc.SetInheritedParentAttributes(mapParentAttrib, bUpdateAttrib);

						m_mgrToDoCtrls.SetAllNeedPreferenceUpdate(TRUE, GetSelToDoCtrl());
					}
				}
			}
		}
	}
}

void CToDoListWnd::OnUpdateTasklistCustomcolumns(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetToDoCtrl().IsReadOnly());
}

void CToDoListWnd::OnEditClearAttribute() 
{
	TDC_ATTRIBUTE nAttrib = TDC::MapColumnToAttribute(m_nContextColumnID);

	GetToDoCtrl().ClearSelectedTaskAttribute(nAttrib);
}

void CToDoListWnd::OnUpdateEditClearAttribute(CCmdUI* pCmdUI) 
{
	TDC_ATTRIBUTE nAttrib = TDC::MapColumnToAttribute(m_nContextColumnID);

	pCmdUI->Enable(GetToDoCtrl().CanClearSelectedTaskAttribute(nAttrib));
}

void CToDoListWnd::OnEditClearFocusedAttribute() 
{
	GetToDoCtrl().ClearSelectedTaskFocusedAttribute();
}

void CToDoListWnd::OnUpdateEditClearFocusedAttribute(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanClearSelectedTaskFocusedAttribute());
}

void CToDoListWnd::OnEditSelectReferenceTarget() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	
	if (!tdc.GotoSelectedReferenceTaskTarget())	
		CMessageBox::AfxShow(IDS_NOREFERENCETASKSINSELECTION);
}

void CToDoListWnd::OnUpdateEditSelectReferenceTarget(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().GetSelectedTaskCount() > 0);
}

void CToDoListWnd::OnEditSelectTaskReferences() 
{
	if (!GetToDoCtrl().GotoSelectedTaskReferences())	
		CMessageBox::AfxShow(IDS_SELECTIONHASNOREFERENCETASKS);
}

void CToDoListWnd::OnUpdateEditSelectTaskReferences(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().GetSelectedTaskCount() > 0);
}

void CToDoListWnd::OnToolsSelectinExplorer() 
{
	FileMisc::SelectFileInExplorer(m_mgrToDoCtrls.GetFilePath(GetSelToDoCtrl()));
}

void CToDoListWnd::OnUpdateToolsSelectinExplorer(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_mgrToDoCtrls.HasFilePath(GetSelToDoCtrl()));
}

void CToDoListWnd::OnToolsRemovefromsourcecontrol() 
{
	CWaitCursor cursor;

	if (m_mgrToDoCtrls.AddToSourceControl(GetSelToDoCtrl(), FALSE))
		UpdateMenuIconMgrSourceControlStatus();
}

void CToDoListWnd::OnUpdateToolsRemovefromsourcecontrol(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_mgrToDoCtrls.CanAddToSourceControl(GetSelToDoCtrl(), FALSE));
}

void CToDoListWnd::OnToolsAddtoSourceControl() 
{
	CWaitCursor cursor;
	
	if (m_mgrToDoCtrls.AddToSourceControl(GetSelToDoCtrl()))
		UpdateMenuIconMgrSourceControlStatus();
}

void CToDoListWnd::OnUpdateToolsAddtoSourceControl(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_mgrToDoCtrls.CanAddToSourceControl(GetSelToDoCtrl()));
}

void CToDoListWnd::OnViewResizeColsToFit() 
{
	GetToDoCtrl().ResizeAttributeColumnsToFit();
}

void CToDoListWnd::OnUpdateViewResizeColsToFit(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanResizeAttributeColumnsToFit());
}

LRESULT CToDoListWnd::OnNotifyReminderModified(WPARAM wp, LPARAM lp)
{
	int nTDC = m_mgrToDoCtrls.FindToDoCtrl((HWND)wp);
	ASSERT(nTDC != -1);

	ASSERT(lp);
	UNREFERENCED_PARAMETER(lp);

	if ((nTDC != -1) && (nTDC == GetSelToDoCtrl()) && IsWindowVisible() && !IsIconic())
		GetToDoCtrl().RedrawReminders();

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
	///////////////////////////////////////////////////////////////////////
	// PERMANENT LOGGING
	FileMisc::LogText(_T("WM_SETTINGCHANGE(%04x, %s)"), uFlags, lpszSection);
	///////////////////////////////////////////////////////////////////////

	// Prompt to restart app whenever Regional settings change
	if ((uFlags == 0) && (StrCmp(lpszSection, _T("intl")) == 0))
	{
		Invalidate();
	}
	else if (StrCmp(lpszSection, _T("TraySettings")) == 0)
	{
		// Icons themselves will already have been updated
		// so we just need to handle dependencies
		UpdateTrayIcon();
		m_dlgTimeTracker.SetWindowIcons(m_icons.GetBigIcon(), m_icons.GetSmallIcon());
	}
	else if (StrCmp(lpszSection, _T("ImmersiveColorSet")) == 0)
	{
		// Windows 10 theme has changed from "light" to "dark" or vice versa
		UpdateTrayIcon();
	}
	else if (uFlags == SPI_SETNONCLIENTMETRICS)
	{
		CFileIcons::Reinitialise();
	}
		
	CFrameWnd::OnSettingChange(uFlags, lpszSection);
}

void CToDoListWnd::OnViewSaveToImage() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	CPreferences prefs;

	// Get filename first as this allows the user a chance to cancel
	CString sFilePath(tdc.GetFilePath());

	if (sFilePath.IsEmpty())
	{
		sFilePath = CEnString(IDS_TDC_UNTITLEDFILE);
		Misc::Trim(sFilePath);
	}
	else
	{
		FileMisc::RemoveExtension(sFilePath);
	}

	sFilePath += '.';
	sFilePath += tdc.GetTaskViewName();
	sFilePath += '.';
	sFilePath += m_sTaskViewImageExt;

	CFileSaveDialog dialog(IDS_SAVETASKVIEWTOIMAGE_TITLE,
							m_sTaskViewImageExt, 
							sFilePath, 
							EOFN_DEFAULTSAVE,
							CEnString(IDS_TASKVIEWIMAGEFILTER), 
							this);

	if (dialog.DoModal(prefs) == IDCANCEL)
		return;
	
	// else
	sFilePath = dialog.GetPathName();
	m_sTaskViewImageExt = FileMisc::GetExtension(sFilePath, FALSE);

	DOPROGRESS(IDS_SAVETOIMAGEPROGRESS);
	
	if (tdc.SaveTaskViewToImage(sFilePath))
	{
		FileMisc::Run(*this, sFilePath);
	}
	else
	{
		CMessageBox::AfxShow(IDS_SAVETOIMAGEERROR);
	}
}

void CToDoListWnd::OnUpdateViewSaveToImage(CCmdUI* pCmdUI) 
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();
	CEnString sMenuItem(ID_VIEW_SAVETOIMAGE);

	if (tdc.CanSaveTaskViewToImage())
	{
		pCmdUI->Enable(TRUE);
		sMenuItem.Replace(_T("%s"), tdc.GetTaskViewName());
	}
	else
	{
		pCmdUI->Enable(FALSE);
		sMenuItem.Replace(_T("(%s)"), _T(""));
	}

	pCmdUI->SetText(sMenuItem);
}

void CToDoListWnd::OnToolsCopyTasklistPath()
{
	CClipboard(*this).SetText(GetToDoCtrl().GetFilePath());
}

void CToDoListWnd::OnUpdateToolsCopyTasklistPath(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_mgrToDoCtrls.HasFilePath(GetSelToDoCtrl()));
}

void CToDoListWnd::OnMoveSelectTaskDependencies()
{
	GetToDoCtrl().GotoSelectedTaskLocalDependencies();
}

void CToDoListWnd::OnMoveSelectTaskDependents()
{
	if (!GetToDoCtrl().GotoSelectedTaskLocalDependents())
		CMessageBox::AfxShow(IDS_NOTASKSDEPENDENTONSELECTION);
}

void CToDoListWnd::OnUpdateMoveSelectTaskDependencies(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetToDoCtrl().SelectedTasksHaveDependencies());
}

void CToDoListWnd::OnUpdateMoveSelectTaskDependents(CCmdUI* pCmdUI)
{
	// It can be an expensive call to get dependents so we don't do it
	pCmdUI->Enable(GetToDoCtrl().GetSelectedTaskCount());
}

void CToDoListWnd::OnEditSetTasklistTabColor() 
{
	int nSelTDC = GetSelToDoCtrl();

	CEnColorDialog dialog(m_mgrToDoCtrls.GetTabColor(nSelTDC));

	if (dialog.DoModal(CPreferences()) == IDOK)
	{
		m_mgrToDoCtrls.SetTabColor(nSelTDC, dialog.GetColor());

		// Make sure the tasklist tab bar is visible
		if (!m_bShowTasklistBar || Prefs().GetAutoHideTabbar())
		{
			m_bShowTasklistBar = TRUE;
			m_pPrefs->SetAutoHideTabbar(FALSE);

			if (!m_tabCtrl.IsWindowVisible())
				Resize();
		}
	}
}

void CToDoListWnd::OnEditClearTasklistTabColor() 
{
	m_mgrToDoCtrls.SetTabColor(GetSelToDoCtrl(), CLR_NONE);
}

void CToDoListWnd::OnUpdateEditClearTasklistTabColor(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_mgrToDoCtrls.GetTabColor(GetSelToDoCtrl()) != CLR_NONE);
}

void CToDoListWnd::OnViewIncrementTaskViewFontSize() 
{
	OnViewIncrementTaskViewFontSize(TRUE);
}

void CToDoListWnd::OnUpdateViewIncrementTaskViewFontSize(CCmdUI* pCmdUI) 
{
	OnUpdateViewIncrementTaskViewFontSize(pCmdUI, TRUE);
}

void CToDoListWnd::OnViewDecrementTaskViewFontSize() 
{
	OnViewIncrementTaskViewFontSize(FALSE);
}

void CToDoListWnd::OnUpdateViewDecrementTaskViewFontSize(CCmdUI* pCmdUI) 
{
	OnUpdateViewIncrementTaskViewFontSize(pCmdUI, FALSE);
}

void CToDoListWnd::OnViewIncrementTaskViewFontSize(BOOL bLarger) 
{
	if (m_pPrefs->IncrementTreeFontSize(bLarger, m_fontMain))
	{
		UpdateTreeAndCommentsFonts();
	}
}

void CToDoListWnd::OnUpdateViewIncrementTaskViewFontSize(CCmdUI* pCmdUI, BOOL bLarger) 
{
	pCmdUI->Enable(m_pPrefs->CanIncrementTreeFontSize(bLarger, m_fontMain));
}

void CToDoListWnd::OnViewRestoreDefaultTaskViewFontSize() 
{
	if (m_pPrefs->RestoreTreeFontSize(m_fontMain))
	{
		UpdateTreeAndCommentsFonts();
	}
}

void CToDoListWnd::UpdateTreeAndCommentsFonts()
{
	CTDCToDoCtrlPreferenceHelper::UpdateToDoCtrl(GetToDoCtrl(), Prefs(), m_fontMain, m_fontTree, m_fontComments);
}

void CToDoListWnd::OnUpdateViewRestoreDefaultTaskViewFontSize(CCmdUI* pCmdUI) 
{
	CString sUnused;
	int nUnused;

	pCmdUI->Enable(Prefs().GetTreeFont(sUnused, nUnused));
}

void CToDoListWnd::OnMoveGoToTask() 
{
	CFilteredToDoCtrl& tdc = GetToDoCtrl();
	CTDLGoToTaskDlg dialog(tdc);

	if (dialog.DoModal() == IDOK)
		tdc.SelectTask(dialog.GetTaskID(), TRUE);
}

void CToDoListWnd::OnUpdateMoveGoToTask(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().GetTaskCount());
}

void CToDoListWnd::OnToolsCleanupIniPreferences() 
{
	if (!CPreferences::UsesIni())
	{
		ASSERT(0);
		return;
	}

	// Exclude open tasklists because they'll just resave their
	// preferences when we close
	CStringArray aExclusions;
	m_mgrToDoCtrls.GetFileNames(aExclusions, TRUE);

	CTDLCleanupIniPreferencesDlg dialog(aExclusions);
		
	if (dialog.DoModal() == IDOK)
	{
		m_mruList.ReadList(CPreferences());
		ResetPrefs();
	}
}

void CToDoListWnd::OnUpdateToolsCleanupIniPreferences(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CPreferences::UsesIni());
}

void CToDoListWnd::OnToolsToggleLogging() 
{
	EnableLogging(!FileMisc::IsLoggingEnabled());
}

void CToDoListWnd::OnUpdateToolsToggleLogging(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(FileMisc::IsLoggingEnabled());
}

void CToDoListWnd::OnEditFindReplaceInTaskTitles() 
{
	GetToDoCtrl().DoFindReplace(TDCA_TASKNAME);
}

void CToDoListWnd::OnUpdateEditFindReplaceInTaskTitles(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanDoFindReplace(TDCA_TASKNAME));
}

void CToDoListWnd::OnEditFindReplaceInTaskComments() 
{
	GetToDoCtrl().DoFindReplace(TDCA_COMMENTS);
}

void CToDoListWnd::OnUpdateEditFindReplaceInTaskComments(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanDoFindReplace(TDCA_COMMENTS));
}

void CToDoListWnd::OnViewShowRemindersWindow() 
{
	m_dlgReminders.ShowWindow();
}

void CToDoListWnd::OnUpdateViewShowRemindersWindow(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CToDoListWnd::OnActivateTaskView(UINT nCmdID)
{
	switch (nCmdID)
	{
	case ID_ACTIVATEVIEW_TASKTREE:
		GetToDoCtrl().SetTaskView(FTCV_TASKTREE);
		break;

	case ID_ACTIVATEVIEW_LISTVIEW:
		GetToDoCtrl().SetTaskView(FTCV_TASKLIST);
		break;

	default:
		ASSERT ((nCmdID >= ID_ACTIVATEVIEW_UIEXTENSION1) && (nCmdID <= ID_ACTIVATEVIEW_UIEXTENSION16));
		GetToDoCtrl().SetTaskView((FTC_VIEW)(FTCV_UIEXTENSION1 + (nCmdID - ID_ACTIVATEVIEW_UIEXTENSION1)));
		break;
	}
}

LRESULT CToDoListWnd::OnModifyKeyboardShortcuts(WPARAM /*wp*/, LPARAM /*lp*/)
{
	return DoPreferences(PREFPAGE_SHORTCUT);
}

void CToDoListWnd::OnTasklistCopyColumnValues() 
{
	OnTasklistCopyColumnValues(FALSE);
}

void CToDoListWnd::OnUpdateTasklistCopyColumnValues(CCmdUI* pCmdUI) 
{
	OnUpdateTasklistCopyColumnValues(pCmdUI, FALSE);
}

void CToDoListWnd::OnTasklistCopySelectedTaskColumnValues() 
{
	OnTasklistCopyColumnValues(TRUE);
}

void CToDoListWnd::OnUpdateTasklistCopySelectedTaskColumnValues(CCmdUI* pCmdUI) 
{
	OnUpdateTasklistCopyColumnValues(pCmdUI, TRUE);
}

void CToDoListWnd::OnTasklistCopyColumnValues(BOOL bSelectedTasks)
{
	ASSERT(m_nContextColumnID != TDCC_NONE);

	VERIFY(GetToDoCtrl().CopyTaskColumnValues(m_nContextColumnID, bSelectedTasks));
}

void CToDoListWnd::OnUpdateTasklistCopyColumnValues(CCmdUI* pCmdUI, BOOL bSelectedTasks)
{
	const CFilteredToDoCtrl& tdc = GetToDoCtrl();

	BOOL bEnable = tdc.CanCopyTaskColumnValues(m_nContextColumnID, bSelectedTasks);
	pCmdUI->Enable(bEnable);

	if (bEnable)
	{
		UINT nMenuStrID = (bSelectedTasks ? IDS_TASKLIST_COPYSELECTEDCOLUMNVALUES : IDS_TASKLIST_COPYCOLUMNVALUES);
		CString sColLabel = tdc.GetColumnName(m_nContextColumnID);

		pCmdUI->SetText(CEnString(nMenuStrID, sColLabel));
	}
}

void CToDoListWnd::OnEditSetPercentToToday() 
{
	GetToDoCtrl().SetSelectedTaskPercentDoneToToday();
}

void CToDoListWnd::OnUpdateEditSetPercentToToday(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().CanSetSelectedTaskPercentDoneToToday());	
}

int CToDoListWnd::FindToDoListWnds(TDCFINDWND& find)
{
	ASSERT(find.hWndIgnore == NULL || ::IsWindow(find.hWndIgnore));

	find.aResults.RemoveAll();
	EnumWindows(FindOtherInstance, (LPARAM)&find);

	return find.aResults.GetSize();
}

BOOL CALLBACK CToDoListWnd::FindOtherInstance(HWND hWnd, LPARAM lParam)
{
	static CString COPYRIGHT(MAKEINTRESOURCE(IDS_COPYRIGHT));

	CString sCaption;
	CWnd::FromHandle(hWnd)->GetWindowText(sCaption);

	if (Misc::HasSuffix(sCaption, COPYRIGHT))
	{
		TDCFINDWND* pFind = (TDCFINDWND*)lParam;
		ASSERT(pFind);

		// check window to ignore
		if ((pFind->hWndIgnore == NULL) || (pFind->hWndIgnore == hWnd))
		{
			// check if it's closing
			DWORD bClosing = FALSE;
			BOOL bSendSucceeded = ::SendMessageTimeout(hWnd,
													   WM_TDL_ISCLOSING,
													   0,
													   0,
													   SMTO_ABORTIFHUNG | SMTO_BLOCK,
													   1000,
													   &bClosing);

			if (bSendSucceeded && (pFind->bIncClosing || !bClosing))
			{
				if (pFind->dwProcessID)
				{
					DWORD dwOtherProcID = 0;
					GetWindowThreadProcessId(hWnd, &dwOtherProcID);
					ASSERT(dwOtherProcID);

					if (dwOtherProcID == pFind->dwProcessID)
					{
						pFind->aResults.Add(hWnd);
						return FALSE; // we can stop now
					}
				}
				else
				{
					pFind->aResults.Add(hWnd);
				}
			}
		}
	}

	return TRUE; // keep going to the end
}

void CToDoListWnd::OnToolsViewLogFile()
{
	FileMisc::Run(*this, FileMisc::GetLogFilePath());
}

void CToDoListWnd::OnUpdateToolsViewLogFile(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FileMisc::FileExists(FileMisc::GetLogFilePath()));
}

void CToDoListWnd::OnToolsAnonymizeTasklist() 
{
	CString sAnonFilePath;

	if (CTDCAnonymizeTasklist::Anonymize(GetToDoCtrl().GetFilePath(), sAnonFilePath))
	{
		if (IDYES == CMessageBox::AfxShow(IDS_ANONYMIZE_SUCCESS, MB_YESNO))
			OpenTaskList(sAnonFilePath, FALSE);
	}
	else
	{
		CMessageBox::AfxShow(IDS_ANONYMIZE_FAIL);
	}
}

void CToDoListWnd::OnUpdateToolsAnonymizeTasklist(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetToDoCtrl().HasFilePath());
}

void CToDoListWnd::OnViewHideAllBars() 
{
	OnViewShowHideAllBars(FALSE);
}

void CToDoListWnd::OnUpdateViewHideAllBars(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bShowFilterBar ||
				   m_bShowStatusBar || 
				   m_bShowingMainToolbar || 
				   m_bShowingCustomToolbar || 
				   m_bShowTasklistBar || 
				   m_bShowTreeListBar);
}

void CToDoListWnd::OnViewShowAllBars() 
{
	OnViewShowHideAllBars(TRUE);
}

void CToDoListWnd::OnUpdateViewShowAllBars(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_bShowFilterBar ||
				   !m_bShowStatusBar ||
				   !m_bShowingMainToolbar ||
				   !m_bShowingCustomToolbar ||
				   !m_bShowTasklistBar ||
				   !m_bShowTreeListBar);
}

void CToDoListWnd::OnViewShowHideAllBars(BOOL bShow)
{
	{
		CLockUpdates lu(*this);
		CAutoFlag af(m_bIgnoreResize, TRUE);

		if ((!m_bShowFilterBar && bShow) || (m_bShowFilterBar && !bShow))
			OnViewShowfilterbar();

		if ((!m_bShowStatusBar && bShow) || (m_bShowStatusBar && !bShow))
			OnViewStatusBar();

		if ((!m_bShowingMainToolbar && bShow) || (m_bShowingMainToolbar && !bShow))
			OnViewMainToolbar();

		if ((!m_bShowingCustomToolbar && bShow) || (m_bShowingCustomToolbar && !bShow))
			OnViewCustomToolbar();

		if ((!m_bShowTasklistBar && bShow) || (m_bShowTasklistBar && !bShow))
			OnViewShowTasklistTabbar();

		if ((!m_bShowTreeListBar && bShow) || (m_bShowTreeListBar && !bShow))
			OnViewShowTreeListTabbar();
	}

	Resize();
}
