#if !defined(AFX_TDCENUM_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_)
#define AFX_TDCENUM_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// tdcenum.h : header file
//

/////////////////////////////////////////////////////////////////////////////

#include "..\Interfaces\IEnums.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// WM_COPYDATA options

enum TDL_COPYDATA
{
	TDLCD_PROCESSSTARTUP,	// cbData = sizeof(CTDCStartupOptions)
	TDLCD_HASTASKFILE,		// cbData = sizeof(CTDCStartupOptions)
	TDLCD_LANGFILECHANGE,	// cbData = sizeof(TCHAR[])
};

/////////////////////////////////////////////////////////////////////////////
// CToDoListWnd creation flags

enum
{
	TLD_FORCEVISIBLE			= 0x0001,
    TLD_PASSWORDPROMPTING		= 0x0002,
    TLD_LOGGING					= 0x0004,
	TLD_IMPORTFILE				= 0x0008,
	TLD_NEWTASK					= 0x0010,
	TLD_STARTEMPTY				= 0x0020,
	TLD_TASKLINK				= 0x0040,
	TLD_UPGRADED				= 0x0080,
	TLD_SAVEUIVISINTASKLIST		= 0x0100,
	TLD_SAVEINTERMEDIATE		= 0x0200,
    TLD_LOGMODULES				= 0x0400,
    TLD_LOGCOMMANDS				= 0x0800,
	TLD_ALLOWFORCEDCHECKOUT		= 0x1000,
	TLD_MASTERPASSWORDENABLED	= 0x2000,
};

/////////////////////////////////////////////////////////////////////////////

enum MISC_MENU
{ 
	MM_TRAYICON, 
	MM_TASKCONTEXT, 
	MM_TASKCONTEXTNOSEL,
	MM_TABCTRLCONTEXT, 
	MM_HEADERCONTEXT, 
	MM_COMMENTS, 
	MM_TOOLARGS 
};

enum APP_MENU
{ 
	AM_FILE, 
	AM_NEWTASK, 
	AM_EDIT, 
	AM_VIEW, 
	AM_MOVE, 
	AM_SORT, 
	AM_SRCCONTROL, 
	AM_TOOLS, 
	AM_WINDOW, 
	AM_HELP, 
	AM_DEBUG 
};

/////////////////////////////////////////////////////////////////////////////
// Print styles

enum TDLPD_STYLE
{
	TDLPDS_NONE = -1,

	TDLPDS_STYLESHEET,
	TDLPDS_WRAP,
	TDLPDS_TABLE,
	TDLPDS_PARA,
	TDLPDS_IMAGE,
	TDLPDS_OTHERHTMLEXPORTER,
};

/////////////////////////////////////////////////////////////////////////////

// Help IDs for Runtime Dialogs not having resource-based templates
enum
{
	IDD_USERTOOL_DIALOG = 1001,
};

/////////////////////////////////////////////////////////////////////////////

enum TDCC_IMAGE
{
	// Order must match the image list
	TDCCI_NONE = -1,
	TDCCI_RISK,
	TDCCI_TRACKTIME,
	TDCCI_REMAINING,
	TDCCI_DEPENDENCY,
	TDCCI_MULTIDEPENDENCY,
	TDCCI_FILELINK,
	TDCCI_FLAG,
	TDCCI_REMINDER,
	TDCCI_ICON,
	TDCCI_DONE,
	TDCCI_PRIORITY,
	TDCCI_LOCK,
	TDCCI_LOCKCALC,
	TDCCI_FLAGCALC,
	TDCCI_REMINDERDATEUNSET,
};

/////////////////////////////////////////////////////////////////////////////

enum TDC_THREADEDEXPORTPURPOSE
{
	TDCTEP_NONE = -1,
	TDCTEP_DUETASKNOTIFY,
	TDCTEP_EXPORTAFTERSAVE,
};

/////////////////////////////////////////////////////////////////////////////

enum TDC_LABELRECT 
{ 
	TDCTR_TEXT, 
	TDCTR_EDIT,
	TDCTR_BKGND,
};

/////////////////////////////////////////////////////////////////////////////

enum TDC_INSERTWHERE
{
	TDC_INSERTATTOP,
	TDC_INSERTATBOTTOM,
	TDC_INSERTATTOPOFSELTASKPARENT,
	TDC_INSERTATBOTTOMOFSELTASKPARENT,
	TDC_INSERTAFTERSELTASK,
	TDC_INSERTBEFORESELTASK,
	TDC_INSERTATTOPOFSELTASK, // subtask
	TDC_INSERTATBOTTOMOFSELTASK, // subtask
};

/////////////////////////////////////////////////////////////////////////////

enum TDC_SORTDIR
{
	TDC_SORTUP   = -1,
	TDC_SORTNONE =  0,
	TDC_SORTDOWN =  1,
};

/////////////////////////////////////////////////////////////////////////////

enum TDC_ARCHIVE
{
	TDC_REMOVENONE,
	TDC_REMOVEALL,
	TDC_REMOVEIFSIBLINGSANDSUBTASKSCOMPLETE,
};

/////////////////////////////////////////////////////////////////////////////

enum TDC_STYLE
{
	TDCS_FIRST,

	TDCS_SHOWINFOTIPS = TDCS_FIRST,					
	TDCS_COLORTEXTBYPRIORITY,			
	TDCS_COLORTEXTBYATTRIBUTE,	
	TDCS_COLORTEXTBYNONE,
	TDCS_SHOWCOMMENTSINLIST,				
	TDCS_SHOWFIRSTCOMMENTLINEINLIST,				
	TDCS_TREATSUBCOMPLETEDASDONE,		
	TDCS_HIDEPERCENTFORDONETASKS,		
	TDCS_CONFIRMDELETE,					
	TDCS_AVERAGEPERCENTSUBCOMPLETION,	
	TDCS_INCLUDEDONEINAVERAGECALC,		
	TDCS_USEEARLIESTDUEDATE,
	TDCS_USELATESTDUEDATE,			
	TDCS_USEEARLIESTSTARTDATE,
	TDCS_USELATESTSTARTDATE,			
	TDCS_USEPERCENTDONEINTIMEEST,		
	TDCS_SHOWCOMMENTSALWAYS,				
	TDCS_HIDEZEROTIMECOST,				
	TDCS_HIDESTARTDUEFORDONETASKS,		
	TDCS_SHOWPERCENTASPROGRESSBAR,		
	TDCS_READONLY,						
	TDCS_CHECKOUTONLOAD,					 
	TDCS_SHOWPATHINHEADER,				
	TDCS_STRIKETHOUGHDONETASKS,			
	TDCS_TASKCOLORISBACKGROUND,			
	TDCS_COMMENTSUSETREEFONT,
	TDCS_SHOWDATESINISO,
	TDCS_USEHIGHESTPRIORITY,
	TDCS_SYNCTIMEESTIMATESANDDATES,
	TDCS_SHOWWEEKDAYINDATES,
	TDCS_ROUNDTIMEFRACTIONS,
	TDCS_INCLUDEDONEINPRIORITYCALC,		
	TDCS_WEIGHTPERCENTCALCBYNUMSUB,
	TDCS_SHOWPARENTSASFOLDERS,
	TDCS_FOCUSTREEONENTER,
	TDCS_AUTOCALCPERCENTDONE,
	TDCS_TRACKSELECTEDTASKONLY,
	TDCS_HIDEPRIORITYNUMBER,
	TDCS_DISPLAYHMSTIMEFORMAT,
	TDCS_SORTDONETASKSATBOTTOM,
	TDCS_USEHIGHESTRISK,
	TDCS_INCLUDEDONEINRISKCALC,		
	TDCS_SHOWPROJECTNAME,
	TDCS_LOGTIMETRACKING,
	TDCS_LOGTASKTIMESEPARATELY,
	TDCS_WARNADDDELETEARCHIVE,
	TDCS_ALLOWPARENTTIMETRACKING,
	TDCS_REFILTERONMODIFY,
	TDCS_AUTOADJUSTDEPENDENCYDATES,
	TDCS_RIGHTSIDECOLUMNS,
	TDCS_USES3RDPARTYSOURCECONTROL,
	TDCS_DUEHAVEHIGHESTPRIORITY,
	TDCS_DONEHAVELOWESTPRIORITY,
	TDCS_DONEHAVELOWESTRISK,
	TDCS_NODUEDATEISDUETODAYORSTART,
	TDCS_CALCREMAININGTIMEBYDUEDATE,
	TDCS_CALCREMAININGTIMEBYSPENT,
	TDCS_CALCREMAININGTIMEBYPERCENT,
	TDCS_SHOWTREELISTBAR,
	TDCS_INCLUDEUSERINCHECKOUT,
	TDCS_SHOWDEFAULTTASKICONS,
	TDCS_ALWAYSHIDELISTPARENTS,
	TDCS_RESTOREFILTERS,
	TDCS_RESORTONMODIFY,
	TDCS_CATEGORYLISTREADONLY,
	TDCS_STATUSLISTREADONLY,
	TDCS_ALLOCTOLISTREADONLY,
	TDCS_ALLOCBYLISTREADONLY,
	TDCS_HIDEZEROPERCENTDONE,
	TDCS_COLUMNHEADERSORTING,
	TDCS_DISPLAYLOGCONFIRM,
	TDCS_ALLOWCOMMENTSSTACKING,
	TDCS_STACKCOMMENTSABOVEEDITS,
	TDCS_SAVEUIVISINTASKLIST,
	TDCS_ALLOWTREEITEMCHECKBOX,
	TDCS_HIDEPANESPLITBAR,
	TDCS_SHOWREMINDERSASDATEANDTIME,
	TDCS_SYNCCOMPLETIONTOSTATUS,
	TDCS_DISABLEPASSWORDPROMPTING,
	TDCS_SUBTASKSINHERITLOCK,
	TDCS_TASKINHERITSSUBTASKFLAGS,
	TDCS_USELATESTLASTMODIFIED,
	TDCS_SHOWTASKVIEWTABCLOSEBUTTON,
	TDCS_SHOWFILELINKTHUMBNAILS,
	TDCS_SETCOMPLETIONSTATUS,

//	TDCS_,
    TDCS_LAST
};

/////////////////////////////////////////////////////////////////////////////

enum TDC_UILOCATION
{
	TDCUIL_BOTTOM,
	TDCUIL_RIGHT,
	TDCUIL_LEFT,
};

/////////////////////////////////////////////////////////////////////////////

enum TDC_COLUMN
{
	TDCC_FIRST = 0,

	TDCC_PRIORITY = TDCC_FIRST,
	TDCC_PERCENT,
	TDCC_TIMEESTIMATE,
	TDCC_TIMESPENT,
	TDCC_STARTDATE,
	TDCC_DUEDATE,
	TDCC_DONEDATE,
	TDCC_ALLOCTO,
	TDCC_ALLOCBY,
	TDCC_STATUS,
	TDCC_CATEGORY,
	TDCC_FILELINK,
	TDCC_POSITION,
	TDCC_ID,
	TDCC_DONE,
	TDCC_TRACKTIME,
	TDCC_FLAG,
	TDCC_CREATIONDATE,
	TDCC_CREATEDBY,
	TDCC_LASTMODDATE,
	TDCC_RISK,
	TDCC_EXTERNALID,
	TDCC_COST,
	TDCC_DEPENDENCY,
	TDCC_RECURRENCE,
	TDCC_VERSION,
	TDCC_RECENTEDIT,
	TDCC_TIMEREMAINING,
	TDCC_ICON,
	TDCC_REMINDER,
	TDCC_PARENTID,
	TDCC_PATH,
	TDCC_TAGS,
	TDCC_COLOR,			// pseudo
	TDCC_SUBTASKDONE,
	TDCC_STARTTIME,		// pseudo
	TDCC_DUETIME,		// pseudo
	TDCC_DONETIME,		// pseudo

	TDCC_CUSTOMCOLUMN_FIRST,
	TDCC_CUSTOMCOLUMN_LAST = TDCC_CUSTOMCOLUMN_FIRST + 63,

	// NEW COLUMNS HERE
	TDCC_CREATIONTIME,	// pseudo
	TDCC_LOCK,
	TDCC_LASTMODBY,
	TDCC_COMMENTSSIZE,
	TDCC_COMMENTSFORMAT,
//	TDCC_,

	TDCC_COUNT,

	TDCC_NONE		= 0xFFFE,
	TDCC_CLIENT		= 0xFFFF, 
}; 

const int NUM_CUST_COLUMNS = (TDCC_CUSTOMCOLUMN_LAST - TDCC_CUSTOMCOLUMN_FIRST + 1);

/////////////////////////////////////////////////////////////////////////////

enum TDC_GOTO
{
	TDCG_NEXT,
	TDCG_PREV,
};

/////////////////////////////////////////////////////////////////////////////

enum TDC_FILEFMT
{
    TDCFF_OLDER,
    TDCFF_SAME,
    TDCFF_NEWER
};

/////////////////////////////////////////////////////////////////////////////

enum TDC_FILE
{
	TDCF_UNSET = -1,
	TDCF_SUCCESS,
	TDCF_NOTEXIST,
	TDCF_NOTTASKLIST,
	TDCF_NOTALLOWED,
	TDCF_CANCELLED,
	TDCF_BADMSXML,
	TDCF_BADNETWORK,
	TDCF_NOENCRYPTIONDLL,
	TDCF_UNKNOWNENCRYPTION,
	TDCF_INUSE,
  	TDCF_NOSPACE,
	TDCF_SSC_NOTCHECKEDOUT,
	TDCF_SSC_NOTSRCCONTROLLED,
	TDCF_SSC_DELAYLOADED,
	TDCF_SSC_ARCHIVE,
	TDCF_SSC_CHECKEDOUTBYOTHER,
	TDCF_OTHER, // always the last value
};

/////////////////////////////////////////////////////////////////////////////

enum // TDL_SAVETASKLIST
{
	TDLS_INCLUDEUNSAVED		= 0x01, 
	TDLS_CLOSINGTASKLISTS	= 0x02, 
	TDLS_CLOSINGWINDOWS		= 0x04, 
	TDLS_NOFLUSH			= 0x08,
	TDLS_AUTOSAVE			= 0x10,
	TDLS_INCLUDEPREFERENCES	= 0x20, 
};
 
/////////////////////////////////////////////////////////////////////////////

enum TDC_GETTASKS
{
	TDCGT_NONE,			// not very useful except to assist coding
	TDCGT_ALL,
	TDCGT_DUE,			// due today
	TDCGT_DUETOMORROW,
	TDCGT_DUETHISWEEK,
	TDCGT_DUENEXTWEEK,
	TDCGT_DUETHISMONTH,
	TDCGT_DUENEXTMONTH,
	TDCGT_DONE,
	TDCGT_NOTDONE,
};
 
/////////////////////////////////////////////////////////////////////////////

enum TDC_RESETIDS
{
	TDCR_YES,
	TDCR_NO,
	TDCR_CHECK
};
 
/////////////////////////////////////////////////////////////////////////////

enum TDC_PASTE
{
	TDCP_ONSELTASK,
	TDCP_BELOWSELTASK,
	TDCP_ATBOTTOM,
};
 
/////////////////////////////////////////////////////////////////////////////

enum TDC_MOVETASK	
{
	TDCM_DOWN,
	TDCM_UP,
	TDCM_LEFT,
	TDCM_RIGHT,
};

/////////////////////////////////////////////////////////////////////////////

enum TDC_MOVEMETHOD
{
	TDCM_NONDRAG,
	TDCM_LEFTDRAG,
	TDCM_RIGHTDRAG,
};

/////////////////////////////////////////////////////////////////////////////

enum // CToDoCtrl::GetTasks()
{
	TDCGTF_PARENTTITLECOMMENTSONLY	= 0x0001,
	TDCGTF_KEEPFLAGGED				= 0x0002,
	TDCGTF_TRANSFORM				= 0x0004,
};

enum // CToDoCtrl::GetSelectedTasks()
{
	TDCGSTF_IMMEDIATEPARENT			= 0x0100,
	TDCGSTF_ALLPARENTS				= 0x0200,
	TDCGSTF_NOTSUBTASKS				= 0x0400,
	TDCGSTF_RESOLVEREFERENCES		= 0x0800,
	TDCGSTF_APPENDREFERENCES		= 0x1000 | TDCGSTF_RESOLVEREFERENCES,
	TDCGSTF_LOCALDEPENDENTS			= 0x2000,
};
 
/////////////////////////////////////////////////////////////////////////////

enum TDC_DATE
{
	TDCD_NONE = -1,
	TDCD_CREATE,
	TDCD_START,		// getting only (date+time)
	TDCD_STARTDATE,	// setting or getting (date only)
	TDCD_STARTTIME,	// setting or getting (time only)
	TDCD_DUE,		// getting only (date+time)
	TDCD_DUEDATE,	// setting or getting (date only)
	TDCD_DUETIME,	// setting or getting (time only)
	TDCD_DONE,		// getting only (date+time)
	TDCD_DONEDATE,	// setting or getting (date only)
	TDCD_DONETIME,	// setting or getting (time only)
	TDCD_LASTMOD,
	TDCD_CUSTOM,
	TDCD_REMINDER,
};
 
/////////////////////////////////////////////////////////////////////////////

// control ids
enum 
{
	IDC_FIRST			= 100,
		
	IDC_TASKTREECTRL,		
	IDC_PROJECTLABEL,	
	IDC_PROJECTNAME,	
	IDC_COMMENTSLABEL,	
	IDC_COMMENTSTYPE,	
	IDC_COMMENTS,	
	
	IDC_FIRST_CUSTOMEDITFIELD,
	IDC_LAST_CUSTOMEDITFIELD = IDC_FIRST_CUSTOMEDITFIELD + 256,
	
	IDC_TASKLISTTABCTRL,
	IDC_TASKLISTCTRL,
	IDC_TASKLABELEDIT,
	IDC_LISTVIEWGROUPBYLABEL,
	IDC_LISTVIEWGROUPBYATTRIB,
	IDC_LISTVIEWOPTIONSLABEL,
	IDC_LISTVIEWOPTIONS,

	IDC_EXTENSIONWINDOW1,
	IDC_EXTENSIONWINDOW2,
	IDC_EXTENSIONWINDOW3,
	IDC_EXTENSIONWINDOW4,
	IDC_EXTENSIONWINDOW5,
	IDC_EXTENSIONWINDOW6,
	IDC_EXTENSIONWINDOW7,
	IDC_EXTENSIONWINDOW8,
	IDC_EXTENSIONWINDOW9,
	IDC_EXTENSIONWINDOW10,
	IDC_EXTENSIONWINDOW11,
	IDC_EXTENSIONWINDOW12,
	IDC_EXTENSIONWINDOW13,
	IDC_EXTENSIONWINDOW14,
	IDC_EXTENSIONWINDOW15,
	IDC_EXTENSIONWINDOW16,

	IDC_TASKATTRIBUTES,
	IDC_HORZSPLITTER,
	IDC_VERTSPLITTER,
		
	IDC_LAST,
};
 
/////////////////////////////////////////////////////////////////////////////

enum // Filter Bar
{
	IDC_FIRST_CUSTOMFILTERFIELD = 4000,
	IDC_LAST_CUSTOMFILTERFIELD = IDC_FIRST_CUSTOMFILTERFIELD + 256,
};
 
/////////////////////////////////////////////////////////////////////////////

enum FIND_OPERATOR
{
	FOP_NONE = -1,
	FOP_EQUALS,
	FOP_NOT_EQUALS,
	FOP_INCLUDES,
	FOP_NOT_INCLUDES,
	FOP_ON_OR_BEFORE,
	FOP_BEFORE,
	FOP_ON_OR_AFTER,
	FOP_AFTER,
	FOP_GREATER_OR_EQUAL,
	FOP_GREATER,
	FOP_LESS_OR_EQUAL,
	FOP_LESS,
	FOP_SET,
	FOP_NOT_SET,
	FOP_IS_COMPLETE, // Applies only to TDCA_DEPENDENCY
};

enum FIND_ATTRIBTYPE
{
	FT_NONE = -1,
	FT_STRING,
	FT_INTEGER,
	FT_DOUBLE,
	FT_DATE,
	FT_BOOL,
	FT_TIMEPERIOD,
	FT_DATERELATIVE,
	FT_ICON,
	FT_RECURRENCE,
	FT_DEPENDENCY
};
 
/////////////////////////////////////////////////////////////////////////////

enum 
{
	TDCCHECKNONE		= 0x0,
	TDCCHECKPARENT		= 0x1,
	TDCCHECKCHILDREN	= 0x2,
	TDCCHECKALL			= 0x3,
};
 
/////////////////////////////////////////////////////////////////////////////

enum TDC_SET
{ 
	SET_NOCHANGE	= -1, 
	SET_FAILED		= 0, 
	SET_CHANGE		= 1 
};
 
/////////////////////////////////////////////////////////////////////////////

enum TDC_TASKCOMPLETION
{ 
	TDCTC_DONE	= 0, 
	TDCTC_UNDONE,
	TDCTC_TOGGLE
};
 
/////////////////////////////////////////////////////////////////////////////

enum
{
	MAX_TDPRIORITY = 10,
	MIN_TDPRIORITY = 0,
	MAX_TDRISK = 10,
	MIN_TDRISK = 0,
};
 
/////////////////////////////////////////////////////////////////////////////

enum TDC_EXPANDCOLLAPSE
{
	TDCEC_ALL,
	TDCEC_SELECTED,
	TDCEC_DUE,
	TDCEC_STARTED,
};
 
/////////////////////////////////////////////////////////////////////////////

enum FILTER_SHOW
{
	FS_ADVANCED = -1,
	FS_ALL = 100, // to not coincide with previous enum
	FS_NOTDONE,
	FS_DONE, 
	FS_FLAGGED, 
	FS_SELECTED,
	FS_RECENTMOD,
	FS_LOCKED,
};

enum FILTER_DATE
{
	FD_ANY,
	FD_NONE,
	FD_TODAY,
	FD_TOMORROW,
	FD_ENDTHISWEEK, 
	FD_ENDNEXTWEEK, 
	FD_ENDTHISMONTH,
	FD_ENDNEXTMONTH,
	FD_ENDTHISYEAR,
	FD_ENDNEXTYEAR,
	FD_NEXTNDAYS,
	FD_NOW,
	FD_USER,
	FD_YESTERDAY,
};

enum // FILTER_OPTIONS
{
	FO_ANYCATEGORY	= 0x0001,
	FO_ANYPERSON	= 0x0002,
	// FO_HIDEPARENTS = 0x0004,  // -> LVO_HIDEPARENTS
	FO_HIDEOVERDUE	= 0x0008,
	FO_HIDEDONE		= 0x0010,
	// FO_HIDECOLLAPSED= 0x0020, // -> LVO_HIDECOLLAPSED
	FO_SHOWALLSUB	= 0x0040,
	FO_ANYTAG		= 0x0080,
	FO_ANYSTATUS	= 0x0100,
	FO_ANYVERSION	= 0x0200,
	// New items here

	FO_GLOBALS		= (FO_HIDEOVERDUE | FO_HIDEDONE | FO_SHOWALLSUB),
	FO_ATTRIBUTES	= (FO_ANYCATEGORY | FO_ANYPERSON | FO_ANYTAG | FO_ANYSTATUS | FO_ANYVERSION)
};

enum // LISTVIEW_OPTIONS
{
	LVO_HIDEPARENTS			= 0x0001,
	LVO_HIDECOLLAPSED		= 0x0002,
	LVO_HIDENOGROUPVALUE	= 0x0004,
	LVO_SORTGROUPSASCENDING	= 0x0008,
};

enum // FILTER_MISC
{
	FM_ANYPRIORITY	= -1,
	FM_ANYRISK		= -1,
	FM_NOPRIORITY	= -2,
	FM_NORISK		= -2,
};

enum FILTER_TITLE
{
	FT_FILTERONTITLEONLY,
	FT_FILTERONTITLECOMMENTS,
	FT_FILTERONANYTEXT,
};

/////////////////////////////////////////////////////////////////////////////

enum // RESULT_FLAGS
{
	RF_DONE			= 0x0001,
	RF_PARENT		= 0x0002,
	RF_REFERENCE	= 0x0004,
	RF_TOPMOST		= 0x0008, // no parent
	RF_GOODASDONE	= 0x0010,
};

/////////////////////////////////////////////////////////////////////////////

enum 
{ 
	FB_MULTISELCAT		= 0x01, 
	FB_MULTISELALLOCTO	= 0x02, 
	FB_MULTISELSTAT		= 0x04, 
	FB_MULTISELALLOCBY	= 0x08,
	FB_MULTISELVER		= 0x10,
	FB_MULTISELTAGS		= 0x20,
};

/////////////////////////////////////////////////////////////////////////////

enum TDC_HITTEST
{
	TDCHT_NOWHERE,
	TDCHT_TASKLIST,
	TDCHT_COLUMNHEADER,
	TDCHT_TASK,
};

/////////////////////////////////////////////////////////////////////////////

enum TDC_HITTESTREASON
{
	TDCHTR_NONE,
	TDCHTR_INFOTIP,
	TDCHTR_CONTEXTMENU,
};

/////////////////////////////////////////////////////////////////////////////

enum TDC_SELECTNEXTTASK
{
	TDC_SELECTFIRST,
	TDC_SELECTNEXT,
	TDC_SELECTNEXTINCLCURRENT,
	TDC_SELECTPREV,
	TDC_SELECTLAST,
};

/////////////////////////////////////////////////////////////////////////////

enum FTC_VIEW
{
	FTCV_UNSET = -1,
	FTCV_TASKTREE,
	FTCV_TASKLIST,

	FTCV_FIRSTUIEXTENSION = 256,
	FTCV_UIEXTENSION1 = FTCV_FIRSTUIEXTENSION,
	FTCV_UIEXTENSION2,
	FTCV_UIEXTENSION3,
	FTCV_UIEXTENSION4,
	FTCV_UIEXTENSION5,
	FTCV_UIEXTENSION6,
	FTCV_UIEXTENSION7,
	FTCV_UIEXTENSION8,
	FTCV_UIEXTENSION9,
	FTCV_UIEXTENSION10,
	FTCV_UIEXTENSION11,
	FTCV_UIEXTENSION12,
	FTCV_UIEXTENSION13,
	FTCV_UIEXTENSION14,
	FTCV_UIEXTENSION15,
	FTCV_UIEXTENSION16,
	FTCV_LASTUIEXTENSION = FTCV_UIEXTENSION16,
};

/////////////////////////////////////////////////////////////////////////////

enum TDC_REMINDER
{
	TDCR_STARTDATE,
	TDCR_DUEDATE,
};

/////////////////////////////////////////////////////////////////////////////

enum TDC_UNDOELMOP
{
	TDCUEO_EDIT,
	TDCUEO_ADD,
	TDCUEO_DELETE,
	TDCUEO_MOVE,
};

enum TDC_UNDOACTIONTYPE
{
	TDCUAT_NONE = -1,
	TDCUAT_EDIT,
	TDCUAT_ADD,
	TDCUAT_DELETE,
	TDCUAT_PASTE,
	TDCUAT_COPY,
	TDCUAT_MOVE,
	TDCUAT_ARCHIVE,
};

/////////////////////////////////////////////////////////////////////////////

enum TDC_MAXSTATE
{
	TDCMS_NORMAL,
	TDCMS_MAXTASKLIST,
	TDCMS_MAXCOMMENTS,
};

/////////////////////////////////////////////////////////////////////////////

enum TDCCA_CALC_OPERATOR
{
	TDCCAC_ADD		= '+',
	TDCCAC_SUBTRACT	= '-',
	TDCCAC_MULTIPLY = '*',
	TDCCAC_DIVIDE	= '/',
};

/////////////////////////////////////////////////////////////////////////////

enum TDC_PREPAREPATH 
{ 
	TDCPP_NONE,
	TDCPP_FILE, 
	TDCPP_STORAGE, 
};

/////////////////////////////////////////////////////////////////////////////

enum TDC_TASKS2CLIPBOARD
{ 
	TDCTC_ASHTML, 
	TDCTC_ASTEXT, 
	TDCTC_ASLINK, 
	TDCTC_ASDEPENDS, 
	TDCTC_ASLINKFULL, 
	TDCTC_ASDEPENDSFULL, 
	TDCTC_ASPATH, 
};

/////////////////////////////////////////////////////////////////////////////
// time long analysis

enum TDCTTL_FORMAT
{
	TTLF_CSV,
	TTLF_XML,
	TTLF_TXT,
	TTLF_HTM,
};

enum TDCTTL_PERIOD
{
	TTLP_TODAY,
	TTLP_THISWEEK,
	TTLP_THISMONTH,
	TTLP_THISYEAR,
	TTLP_FROMTO,
	TTLP_YESTERDAY,
	TTLP_LASTWEEK,
	TTLP_LASTMONTH,
	TTLP_LASTYEAR,
};

enum TDCTTL_BREAKDOWN
{
	TTLB_BYTASK,
	TTLB_BYDAY,
	TTLB_BYWEEK,
	TTLB_BYMONTH,
};

/////////////////////////////////////////////////////////////////////////////
// web update 

enum TDL_WEBUPDATE_CHECK
{
	TDLWUC_WANTUPDATE,
	TDLWUC_WANTPRERELEASEUPDATE,
	TDLWUC_CANCELLED,
	TDLWUC_NOTCONNECTED,
	TDLWUC_NOUPDATES,
	TDLWUC_FAILED
};

/////////////////////////////////////////////////////////////////////////////
// attribute visibility

enum TDL_SHOWATTRIB
{
	TDLSA_ALL,
	TDLSA_ASCOLUMN,
	TDLSA_ANY
};

/////////////////////////////////////////////////////////////////////////////

enum TDLID_IMPORTTO
{
	TDIT_CREATENEWTASKLIST,
	TDIT_ADDTOTOPOFTASKLIST,
	TDIT_ADDTOSELECTEDTASK,
	TDIT_ADDBELOWSELECTEDTASK,
	TDIT_ADDTOBOTTOMOFTASKLIST,
	TDIT_MERGETOTASKLISTBYTITLE,
	TDIT_MERGETOTASKLISTBYID,
};

/////////////////////////////////////////////////////////////////////////////

enum TDC_OFFSETTASKDATES
{
	TDCOTD_OFFSETSUBTASKS		= 0x01,
	TDCOTD_OFFSETSUBTASKREFS	= 0x02,
	TDCOTD_OFFSETFROMTODAY		= 0x04,
	TDCOTD_PRESERVEENDOFMONTH	= 0x08,
};

/////////////////////////////////////////////////////////////////////////////

enum TDC_ATTRIBUTECATEGORY
{
	TDCAC_NONE = -1,
	TDCAC_CUSTOM,
	TDCAC_DATETIME,
	TDCAC_TEXT,
	TDCAC_NUMERIC,
	TDCAC_TIMEPERIOD,
	TDCAC_OTHER,
};

/////////////////////////////////////////////////////////////////////////////

#endif // AFX_TDCENUM_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_