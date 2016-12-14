// ToDoCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ToDoCtrl.h"
#include "tdlschemadef.h"
#include "taskfile.h"
#include "taskclipboard.h"
#include "tdcmsg.h"
#include "tdcmapping.h"
#include "todocommentsctrl.h"
#include "tdstringres.h"
#include "resource.h"
#include "tasktimelog.h"
#include "todoitem.h"
#include "tdcstatic.h"
#include "tdcenum.h"
#include "tdltaskicondlg.h"
#include "tdlreuserecurringtaskdlg.h"
#include "tdlimportoutlookobjectsdlg.h"
#include "tdccustomattributehelper.h"
#include "tdladdloggedtimedlg.h"
#include "tdcoutlookimporthelper.h"

#include "..\shared\holdredraw.h"
#include "..\shared\osversion.h"
#include "..\shared\deferWndMove.h"
#include "..\shared\dlgunits.h"
#include "..\shared\themed.h"
#include "..\shared\datehelper.h"
#include "..\shared\driveinfo.h"
#include "..\shared\toolbarhelper.h"
#include "..\shared\colordef.h"
#include "..\shared\spellcheckdlg.h"
#include "..\shared\passworddialog.h"
#include "..\shared\winclasses.h"
#include "..\shared\wclassdefines.h"
#include "..\shared\enfiledialog.h"
#include "..\shared\misc.h"
#include "..\shared\webmisc.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\enstring.h"
#include "..\shared\stringres.h"
#include "..\shared\treectrlhelper.h"
#include "..\shared\filemisc.h"
#include "..\shared\preferences.h"
#include "..\shared\autoflag.h"
#include "..\shared\enbitmap.h"
#include "..\shared\enmenu.h"
#include "..\shared\msoutlookhelper.h"
#include "..\shared\savefocus.h"
#include "..\shared\localizer.h"
#include "..\shared\clipboard.h"

#include "..\interfaces\IContentControl.h"

#include "..\3rdparty\msoutl.h"
#include "..\3rdparty\shellicons.h"

#include <Windowsx.h>
#include <float.h>
#include <Lmcons.h>
#include <math.h>
#include <afxpriv.h>
#include <shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const int CTRLHEIGHT		= 13; // dlu
const int CTRLVSPACING		= 5;  // dlu
const int CTRLHSPACING		= 4;  // dlu
const int CTRLLABELLEN		= 45; // dlu
const int CTRLCTRLLEN		= 65; // dlu
const int COMMENTSTYPELEN	= 85; // dlu

/////////////////////////////////////////////////////////////////////////////

const int TIMETRACKPERIOD	= 10000; // 10 secs
const int MIDNIGHTPERIOD	= 60000; // 1 minute

/////////////////////////////////////////////////////////////////////////////

const unsigned short SPLITSIZE				= 6; 
const unsigned short DEFCOMMENTSIZE			= 260;
const unsigned short MINNONCOMMENTHEIGHT	= 250; // what's above the comment section
const unsigned short MINNONCOMMENTWIDTH		= 350; // what's to the left of the comment section

/////////////////////////////////////////////////////////////////////////////

const COLORREF BLACK = RGB(0, 0, 0);
const COLORREF WHITE = RGB(240, 240, 240);

/////////////////////////////////////////////////////////////////////////////

const UINT  DAYINSECS		= 24 * 60 * 60;
const int   COMBODROPHEIGHT	= 200;

/////////////////////////////////////////////////////////////////////////////

const DWORD IDD_TODOCTRL_DIALOG = (DWORD)(LPCTSTR)_T("IDD_TODOCTRL_DIALOG");

/////////////////////////////////////////////////////////////////////////////

//const LPCTSTR DEFAULT_UTF8_HEADER = _T("version=\"1.0\" encoding=\"UTF-8\"");
const LPCTSTR DEFAULT_UNICODE_HEADER = _T("version=\"1.0\" encoding=\"UTF-16\"");
const LPCTSTR ARCHIVE_ID = _T(".done");
const LPCTSTR DICTIONARY_URL = _T("https://github.com/abstractspoon/ToDoList_Downloads/wiki/Dictionaries");

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
const double TICKS2HOURS = 10.0 / (1000 * 3600);
#else
const double TICKS2HOURS = 1.0 / (1000 * 3600);
#endif

const int DECIMALS		 = 4;

//////////////////////////////////////////////////////////////////////////////

enum 
{
	ID_TIME_TRACK = 0xfff0,
	ID_EXTERNALID_LINK,
	ID_DEPENDS_LINK,
};

// file formats changes
// as changes are made, insert a new value *BEFORE* FILEFORMAT_CURRENT
// which will effectively increment FILEFORMAT_CURRENT and be useful
// for conditional code
enum 
{
	FILEFORMAT_SORTBYID = 8,
	FILEFORMAT_CUSTATTRIB,
	FILEFORMAT_RECUR_MOD,
	// insert here when format changes
 
	FILEFORMAT_CURRENT
};

enum // flags for UpdateTask
{
	UTF_TIMEUNITSONLY	= 0x01,
	UTF_RECALCTIME		= 0x02,		
};

//////////////////////////////////////////////////////////////////////////////
// private class to help optimize xml parsing

class CXmlParseController : public IXmlParse
{
public:
	CXmlParseController(LPCTSTR szItem) : m_sItem(szItem) {}
	
	virtual BOOL Continue(LPCTSTR szItem, LPCTSTR /*szValue*/) const
	{
		return (m_sItem.CompareNoCase(szItem) != 0);
	}
	
protected:
	CString m_sItem;
};

//////////////////////////////////////////////////////////////////////////////
// private macros

#define HOLD_REDRAW(tdc, tree) 	CHoldRedraw hr(tdc); CHoldRedraw hr2(tree);

#define IMPLEMENT_UNDO(type) CUndoAction ua(m_data, type)
#define IMPLEMENT_UNDOEDIT() CUndoAction ua(m_data, TDCUAT_EDIT)
#define IMPLEMENT_UNDOEXT(type, extend) CUndoAction ua(m_data, type, extend)

//////////////////////////////////////////////////////////////////////////////
// static variables

int	CToDoCtrl::s_nCommentsSize = -1; // unset
 
//////////////////////////////////////////////////////////////////////////////
// private CToDoCtrl messages

UINT CToDoCtrl::WM_TDC_FIXUPPOSTDROPSELECTION		= (WM_APP + 1);
UINT CToDoCtrl::WM_TDC_REFRESHPERCENTSPINVISIBILITY	= (WM_APP + 2);
UINT CToDoCtrl::WM_TDC_RECREATERECURRINGTASK		= (WM_APP + 3);

//////////////////////////////////////////////////////////////////////////////

CToDoCtrl::CToDoCtrl(CContentMgr& mgr, const CONTENTFORMAT& cfDefault, const TDCCOLEDITFILTERVISIBILITY& visDefault) 
	: 
	m_bArchive(FALSE),
	m_bCheckedOut(FALSE),
	m_bDelayLoaded(FALSE),
	m_bDragDropSubtasksAtTop(TRUE),
	m_bFirstLoadCommentsPrefs(TRUE),
	m_bModified(FALSE), 
	m_bSourceControlled(FALSE),
	m_bSplitting(FALSE),
	m_bTimeTrackingPaused(FALSE),
	m_cbAllocBy(ACBS_ALLOWDELETE),
	m_cbAllocTo(ACBS_ALLOWDELETE),
	m_cbCategory(ACBS_ALLOWDELETE),
	m_cbCommentsType(&mgr),
	m_cbFileRef(FES_COMBOSTYLEBTN | FES_GOBUTTON | FES_ALLOWURL | FES_RELATIVEPATHS | FES_DISPLAYSIMAGES),
	m_cbStatus(ACBS_ALLOWDELETE),
	m_cbTags(ACBS_ALLOWDELETE),
	m_cbTimeDone(TCB_NOTIME),
	m_cbTimeDue(TCB_NOTIME),
	m_cbTimeStart(TCB_NOTIME),
	m_cbVersion(ACBS_ALLOWDELETE),
	m_cfDefault(cfDefault),
	m_ctrlComments(NULL),
	m_dCost(0),
	m_dLogTime(0),
	m_dTimeEstimate(0),
	m_dTimeSpent(0),
	m_data(m_aStyles),
	m_dwEditTitleTaskID(0),
	m_dwLastAddedID(0),
	m_dwNextUniqueID(1), 
	m_dwTimeTrackTaskID(0),
	m_eTaskName(PEC_AUTODESTROY),
	m_hFontComments(NULL),
	m_hFontTree(NULL),
	m_mgrContent(mgr),
	m_nCommentsPos(TDCUIL_RIGHT),
	m_nCommentsSize(DEFCOMMENTSIZE),
	m_nCommentsState(CS_CLEAN),
	m_nControlsPos(TDCUIL_BOTTOM),
	m_nDefRecurFrom(TDIRO_DUEDATE),
	m_nDefRecurReuse(TDIRO_REUSE),
	m_nFileFormat(FILEFORMAT_CURRENT),
	m_nFileVersion(0),
	m_nMaxState(TDCMS_NORMAL),
	m_nPriority(-1), 
	m_treeDragDrop(TSH(), m_taskTree.Tree()),
	m_visColAttrib(visDefault),
	m_hIconClock(NULL), 
	m_hIconLink(NULL),
	m_nTimeEstUnits(TDCU_HOURS),
	m_nTimeSpentUnits(TDCU_HOURS),
	m_sXmlHeader(DEFAULT_UNICODE_HEADER),
	m_taskTree(m_ilTaskIcons, m_data, m_aStyles, m_visColAttrib.GetVisibleColumns(), m_aCustomAttribDefs)
{
	SetBordersDLU(0);
	
	// add controls
	for (int nCtrl = 0; nCtrl < NUM_TDCCTRLS; nCtrl++)
	{
		const TDCCONTROL& ctrl = TDCCONTROLS[nCtrl];

		AddRCControl(_T("CONTROL"), ctrl.szClass, CString((LPCTSTR)ctrl.nIDCaption), 
					ctrl.dwStyle, ctrl.dwExStyle,
					ctrl.nX, ctrl.nY, ctrl.nCx, ctrl.nCy, ctrl.nID);
	}
	
    // init styles array
    // MFC sets all elements to zero
    m_aStyles.SetSize(TDCS_LAST);
	
	// set up number masks
	m_ePercentDone.SetMask(_T("0123456789"));
	m_eCost.SetMask(_T("-.0123456789"), ME_LOCALIZEDECIMAL);
	
	// add chess clock button to 'time spent'
	m_hIconClock = AfxGetApp()->LoadIcon(IDI_TIME_TRACK);
	m_eTimeSpent.InsertButton(0, ID_TIME_TRACK, m_hIconClock, CEnString(IDS_TDC_STARTSTOPCLOCK));

	// add link button to dependency
	m_hIconLink = AfxGetApp()->LoadIcon(IDI_DEPENDS_LINK);
	m_eDependency.AddButton(ID_DEPENDS_LINK, m_hIconLink, CEnString(IDS_TDC_DEPENDSLINK_TIP));

	// misc
	m_cpColour.SetSelectionMode(CP_MODE_TEXT);
	m_data.SetDefaultCommentsFormat(m_cfDefault);
}

CToDoCtrl::~CToDoCtrl()
{
}

void CToDoCtrl::DoDataExchange(CDataExchange* pDX)
{
	CRuntimeDlg::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_ALLOCBY, m_cbAllocBy);
	DDX_Control(pDX, IDC_ALLOCTO, m_cbAllocTo);
	DDX_Control(pDX, IDC_CATEGORY, m_cbCategory);
	DDX_Control(pDX, IDC_COLOUR, m_cpColour);
	DDX_Control(pDX, IDC_COMMENTSTYPE, m_cbCommentsType);
	DDX_Control(pDX, IDC_COST, m_eCost);
	DDX_Control(pDX, IDC_DEPENDS, m_eDependency);
	DDX_Control(pDX, IDC_DONEDATE, m_dtcDone);
	DDX_Control(pDX, IDC_DONETIME, m_cbTimeDone);
	DDX_Control(pDX, IDC_DUEDATE, m_dtcDue);
	DDX_Control(pDX, IDC_DUETIME, m_cbTimeDue);
	DDX_Control(pDX, IDC_EXTERNALID, m_eExternalID);
	DDX_Control(pDX, IDC_FILEPATH, m_cbFileRef);
	DDX_Control(pDX, IDC_PERCENT, m_ePercentDone);
	DDX_Control(pDX, IDC_PERCENTSPIN, m_spinPercent);
	DDX_Control(pDX, IDC_PRIORITY, m_cbPriority);
	DDX_Control(pDX, IDC_RECURRENCE, m_eRecurrence);
	DDX_Control(pDX, IDC_RISK, m_cbRisk);
	DDX_Control(pDX, IDC_STARTDATE, m_dtcStart);
	DDX_Control(pDX, IDC_STARTTIME, m_cbTimeStart);
	DDX_Control(pDX, IDC_STATUS, m_cbStatus);
	DDX_Control(pDX, IDC_TAGS, m_cbTags);
	DDX_Control(pDX, IDC_TIMEEST, m_eTimeEstimate);
	DDX_Control(pDX, IDC_TIMESPENT, m_eTimeSpent);
	DDX_Control(pDX, IDC_VERSION, m_cbVersion);

	DDX_Text(pDX, IDC_COST, m_dCost, DECIMALS);
	DDX_Text(pDX, IDC_DEPENDS, m_sDepends);
	DDX_Text(pDX, IDC_EXTERNALID, m_sExternalID);
	DDX_Text(pDX, IDC_PROJECTNAME, m_sProjectName);
	DDX_Text(pDX, IDC_TIMEEST, m_dTimeEstimate, DECIMALS);
	DDX_Text(pDX, IDC_TIMESPENT, m_dTimeSpent, DECIMALS);

	DDX_AutoCBString(pDX, IDC_ALLOCBY, m_sAllocBy);
	DDX_AutoCBString(pDX, IDC_STATUS, m_sStatus);
	DDX_AutoCBString(pDX, IDC_VERSION, m_sVersion);

	DDX_CBPriority(pDX, IDC_PRIORITY, m_nPriority);
	DDX_CBRisk(pDX, IDC_RISK, m_nRisk);
	DDX_ColourPicker(pDX, IDC_COLOUR, m_crColour);
	
	// custom
	if (pDX->m_bSaveAndValidate)
	{
		CString sPercent;
		m_ePercentDone.GetWindowText(sPercent);
		
		m_nPercentDone = max(0, _ttoi(sPercent));
		m_nPercentDone = min(100, m_nPercentDone);

		m_nTimeEstUnits = TDC::MapTHUnitsToUnits(m_eTimeEstimate.GetUnits());
		m_nTimeSpentUnits = TDC::MapTHUnitsToUnits(m_eTimeSpent.GetUnits());

		m_cbCategory.GetChecked(m_aCategory);
		m_cbAllocTo.GetChecked(m_aAllocTo);
		m_cbTags.GetChecked(m_aTags);
		m_cbFileRef.GetFileList(m_aFileRefs);
		m_eRecurrence.GetRecurrenceOptions(m_tRecurrence);

		if (m_crColour == CLR_DEFAULT)
			m_crColour = CLR_NONE; // unset

		CTDCCustomAttributeHelper::GetControlData(this, m_aCustomControls, m_aCustomAttribDefs, m_mapCustomCtrlData);
	}
	else
	{
		m_spinPercent.SetPos(m_nPercentDone);
		
		m_eTimeEstimate.SetUnits(TDC::MapUnitsToTHUnits(m_nTimeEstUnits));
		m_eTimeSpent.SetUnits(TDC::MapUnitsToTHUnits(m_nTimeSpentUnits));

		m_cbCategory.SetChecked(m_aCategory);
		m_cbAllocTo.SetChecked(m_aAllocTo);
		m_cbTags.SetChecked(m_aTags);
		m_cbFileRef.SetFileList(m_aFileRefs);

		m_eRecurrence.SetRecurrenceOptions(m_tRecurrence);

		if (m_mapCustomCtrlData.GetCount() == 0)
			CTDCCustomAttributeHelper::ClearCustomAttributeControls(this, m_aCustomControls, m_aCustomAttribDefs);
		else
			CTDCCustomAttributeHelper::UpdateCustomAttributeControls(this, m_aCustomControls, m_aCustomAttribDefs, m_mapCustomCtrlData);
	}
}

void CToDoCtrl::UpdateComments(BOOL bSaveAndValidate)
{
	if (bSaveAndValidate)
	{
		m_ctrlComments.GetTextContent(m_sTextComments);
		m_ctrlComments.GetContent(m_customComments);
	}
	else
	{
		// if we can't set the custom comments or
		// there are no custom comments then try setting the text comments
		BOOL bCommentsFocused = m_ctrlComments.HasFocus();

		if (m_customComments.IsEmpty() || !m_ctrlComments.SetContent(m_customComments, !bCommentsFocused))
			m_ctrlComments.SetTextContent(m_sTextComments, !bCommentsFocused);
	}
}

BEGIN_MESSAGE_MAP(CToDoCtrl, CRuntimeDlg)
//{{AFX_MSG_MAP(CToDoCtrl)
	//}}AFX_MSG_MAP
	ON_WM_SETCURSOR()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_CAPTURECHANGED()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_SETTINGCHANGE()
	ON_WM_HELPINFO()

	ON_NOTIFY(NM_CLICK, IDC_TASKTREELIST, OnTreeClick)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TASKTREELIST, OnTreeSelChange)

	ON_REGISTERED_MESSAGE(WM_ICC_COMMENTSCHANGE, OnCommentsChange)
	ON_REGISTERED_MESSAGE(WM_ICC_COMMENTSKILLFOCUS, OnCommentsKillFocus)
	ON_REGISTERED_MESSAGE(WM_ICC_WANTSPELLCHECK, OnCommentsWantSpellCheck)
	ON_REGISTERED_MESSAGE(WM_ICC_DOHELP, OnCommentsDoHelp)

	ON_REGISTERED_MESSAGE(WM_TDCN_COLUMNEDITCLICK, OnColumnEditClick)
	ON_REGISTERED_MESSAGE(WM_TDCM_GETTASKREMINDER, OnTDCGetTaskReminder)
	ON_REGISTERED_MESSAGE(WM_TDCM_HASCLIPBOARD, OnTDCHasClipboard)
	ON_REGISTERED_MESSAGE(WM_TDCM_GETCLIPBOARD, OnTDCGetClipboard)
	ON_REGISTERED_MESSAGE(WM_TDCM_TASKLINK, OnTDCDoTaskLink)
	ON_REGISTERED_MESSAGE(WM_TDCM_FAILEDLINK, OnTDCFailedLink)
	ON_REGISTERED_MESSAGE(WM_TDCM_ISTASKDONE, OnTDCTaskIsDone)

	ON_CBN_EDITCHANGE(IDC_DONETIME, OnSelChangeDoneTime)
	ON_CBN_EDITCHANGE(IDC_DUETIME, OnSelChangeDueTime)
	ON_CBN_EDITCHANGE(IDC_STARTTIME, OnSelChangeStartTime)
	ON_CBN_SELCHANGE(IDC_ALLOCBY, OnSelChangeAllocBy)
	ON_CBN_SELCHANGE(IDC_ALLOCTO, OnSelChangeAllocTo)
	ON_CBN_SELCHANGE(IDC_CATEGORY, OnSelChangeCategory)
	ON_CBN_SELCHANGE(IDC_DONETIME, OnSelChangeDoneTime)
	ON_CBN_SELCHANGE(IDC_DUETIME, OnSelChangeDueTime)
	ON_CBN_SELCHANGE(IDC_FILEPATH, OnSelChangeFileRefPath)
	ON_CBN_SELENDCANCEL(IDC_FILEPATH, OnCancelChangeFileRefPath)
	ON_CBN_SELCHANGE(IDC_PRIORITY, OnChangePriority)
	ON_CBN_SELCHANGE(IDC_RISK, OnChangeRisk)
	ON_CBN_SELCHANGE(IDC_STARTTIME, OnSelChangeStartTime)
	ON_CBN_SELCHANGE(IDC_STATUS, OnSelChangeStatus)
	ON_CBN_SELCHANGE(IDC_TAGS, OnSelChangeTag)
	ON_CBN_SELCHANGE(IDC_VERSION, OnSelChangeVersion)
	ON_CBN_SELENDCANCEL(IDC_ALLOCTO, OnSelCancelAllocTo)
	ON_CBN_SELENDCANCEL(IDC_CATEGORY, OnSelCancelCategory)
	ON_CBN_SELENDCANCEL(IDC_TAGS, OnSelCancelTag)
	ON_CBN_SELENDOK(IDC_COMMENTSTYPE, OnSelChangeCommentsType)
	ON_EN_CHANGE(IDC_COST, OnChangeCost)
	ON_EN_CHANGE(IDC_DEPENDS, OnChangeDependency)
	ON_EN_CHANGE(IDC_EXTERNALID, OnChangeExternalID)
	ON_EN_CHANGE(IDC_PERCENT, OnChangePercent)
	ON_EN_CHANGE(IDC_PROJECTNAME, OnChangeProjectName)
	ON_EN_CHANGE(IDC_RECURRENCE, OnChangeRecurrence)
	ON_EN_CHANGE(IDC_TIMEEST, OnChangeTimeEstimate)
	ON_EN_CHANGE(IDC_TIMESPENT, OnChangeTimeSpent)
	ON_MESSAGE(CPN_SELENDOK, OnChangeColour)
	ON_MESSAGE(WM_GETFONT, OnGetFont)
	ON_MESSAGE(WM_TDC_RECREATERECURRINGTASK, OnRecreateRecurringTask)
	ON_MESSAGE(WM_TDC_REFRESHPERCENTSPINVISIBILITY, OnRefreshPercentSpinVisibility)
	ON_MESSAGE(WM_TDC_FIXUPPOSTDROPSELECTION, OnFixupPostDropSelection)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DONEDATE, OnCompletionDatechange)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DUEDATE, OnDueDatechange)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_STARTDATE, OnStartDatechange)
	ON_REGISTERED_MESSAGE(WM_ACBN_ITEMADDED, OnAutoComboAddDelete)
	ON_REGISTERED_MESSAGE(WM_ACBN_ITEMDELETED, OnAutoComboAddDelete)
	ON_REGISTERED_MESSAGE(WM_DD_DRAGABORT, OnTreeDragDrop)
	ON_REGISTERED_MESSAGE(WM_DD_DRAGDROP, OnTreeDragDrop)
	ON_REGISTERED_MESSAGE(WM_DD_DRAGENTER, OnTreeDragDrop)
	ON_REGISTERED_MESSAGE(WM_DD_DRAGOVER, OnTreeDragOver)
	ON_REGISTERED_MESSAGE(WM_DD_PREDRAGMOVE, OnTreeDragDrop)
	ON_REGISTERED_MESSAGE(WM_FE_DISPLAYFILE, OnFileEditDisplayFile)
	ON_REGISTERED_MESSAGE(WM_FE_GETFILEICON, OnFileEditWantIcon)
	ON_REGISTERED_MESSAGE(WM_PCANCELEDIT, OnEditCancel)
	ON_REGISTERED_MESSAGE(WM_PENDEDIT, OnEditEnd)
	ON_REGISTERED_MESSAGE(WM_TDL_APPLYADDLOGGEDTIME, OnApplyAddLoggedTime)
	ON_REGISTERED_MESSAGE(WM_TEN_UNITSCHANGE, OnTimeUnitsChange)
	ON_REGISTERED_MESSAGE(WM_TLDT_DROP, OnDropObject)
 	ON_REGISTERED_MESSAGE(WM_EE_BTNCLICK, OnEEBtnClick)

	ON_NOTIFY_RANGE(DTN_DATETIMECHANGE, IDC_FIRST_CUSTOMDATAFIELD, IDC_LAST_CUSTOMDATAFIELD, OnCustomAttributeChange)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_FIRST_CUSTOMDATAFIELD, IDC_LAST_CUSTOMDATAFIELD, OnCustomAttributeChange)
	ON_CONTROL_RANGE(CBN_EDITCHANGE, IDC_FIRST_CUSTOMDATAFIELD, IDC_LAST_CUSTOMDATAFIELD, OnCustomAttributeChange)
	ON_CONTROL_RANGE(CBN_SELCHANGE, IDC_FIRST_CUSTOMDATAFIELD, IDC_LAST_CUSTOMDATAFIELD, OnCustomAttributeChange)
	ON_CONTROL_RANGE(CBN_SELENDCANCEL, IDC_FIRST_CUSTOMDATAFIELD, IDC_LAST_CUSTOMDATAFIELD, OnCustomAttributeChange)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_FIRST_CUSTOMDATAFIELD, IDC_LAST_CUSTOMDATAFIELD, OnCustomAttributeChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToDoCtrl message handlers

BOOL CToDoCtrl::ParseTaskLink(const CString& sLink, DWORD& dwTaskID, CString& sFile) const
{
	return m_taskTree.ParseTaskLink(sLink, dwTaskID, sFile);
}

BOOL CToDoCtrl::ParseTaskLink(const CString& sLink, const CString& sFolder, DWORD& dwTaskID, CString& sFile)
{
	return CTDLTaskCtrlBase::ParseTaskLink(sLink, sFolder, dwTaskID, sFile);
}

BOOL CToDoCtrl::IsReservedShortcut(DWORD dwShortcut)
{
	return CTDLTaskCtrlBase::IsReservedShortcut(dwShortcut);
}

void CToDoCtrl::EnableExtendedSelection(BOOL bCtrl, BOOL bShift)
{
	CTDLTaskCtrlBase::EnableExtendedSelection(bCtrl, bShift);
}

void CToDoCtrl::SetRecentlyModifiedPeriod(const COleDateTimeSpan& dtSpan)
{
	TODOITEM::SetRecentlyModifiedPeriod(dtSpan);
}

TDC_FILEFMT CToDoCtrl::CompareFileFormat() const
{
    if (m_nFileFormat < FILEFORMAT_CURRENT)
	{
		return TDCFF_OLDER;
	}
    else if (m_nFileFormat > FILEFORMAT_CURRENT)
	{
        return TDCFF_NEWER;
	}
    
	// else
	return TDCFF_SAME;
}

void CToDoCtrl::EnableEncryption(BOOL bEnable)
{
	if (!bEnable && !m_sPassword.IsEmpty())
	{
		m_sPassword.Empty();
		SetModified(TRUE, TDCA_ENCRYPT, 0);
	}
	else if (bEnable && m_sPassword.IsEmpty())
	{
		CEnString sExplanation(IDS_ENABLEENCRYPTION);

		if (CPasswordDialog::RetrievePassword(TRUE, m_sPassword, sExplanation))
			SetModified(TRUE, TDCA_ENCRYPT, 0);
	}
}

BOOL CToDoCtrl::VerifyPassword(const CString& sExplanation) const
{
	if (!IsEncrypted())
		return TRUE; // no password to check

	return CPasswordDialog::VerifyPassword(m_sPassword, sExplanation);
}

BOOL CToDoCtrl::CanEncrypt()
{
	return CTaskFile::CanEncrypt();
}

BOOL CToDoCtrl::Create(const RECT& rect, CWnd* pParentWnd, UINT nID, BOOL bVisible, BOOL bEnabled)
{
	DWORD dwStyle = (WS_CHILD | (bVisible ? WS_VISIBLE : 0) | (bEnabled ? 0 : WS_DISABLED) | WS_TABSTOP);
	CLocalizer::IgnoreString(_T("ToDoCtrl"));
	
	return CRuntimeDlg::Create(_T("ToDoCtrl"), dwStyle, WS_EX_CONTROLPARENT, rect, pParentWnd, nID);
}

BOOL CToDoCtrl::OnInitDialog() 
{
	// create the tree-list before anything else
	CRect rect(0, 0, 0, 0);
	VERIFY(m_taskTree.Create(this, rect, IDC_TASKTREELIST));

	// create rest of controls
	CRuntimeDlg::OnInitDialog();
	
	// host for comments
	VERIFY (CreateContentControl(FALSE));
	
	// disable translation of auto-combos
	CLocalizer::EnableTranslation(m_cbAllocBy, FALSE);
	CLocalizer::EnableTranslation(m_cbAllocTo, FALSE);
	CLocalizer::EnableTranslation(m_cbCategory, FALSE);
	CLocalizer::EnableTranslation(m_cbStatus, FALSE);
	CLocalizer::EnableTranslation(m_cbVersion, FALSE);
	CLocalizer::EnableTranslation(m_cbTags, FALSE);

	// percent spin
	m_spinPercent.SetRange(0, 100);
	m_spinPercent.SetBuddy(GetDlgItem(IDC_PERCENT));
	
	UDACCEL uda = { 0, m_nPercentIncrement };
	m_spinPercent.SetAccel(1, &uda);
	
	// init dates
	m_dtcStart.SendMessage(DTM_SETSYSTEMTIME, GDT_NONE, 0);
	m_dtcDue.SendMessage(DTM_SETSYSTEMTIME, GDT_NONE, 0);
	m_dtcDone.SendMessage(DTM_SETSYSTEMTIME, GDT_NONE, 0);
	
	m_dtTree.Register(&m_taskTree.Tree(), this);
	m_dtFileRef.Register(&m_cbFileRef, this); 

	// task icon image list
	LoadTaskIcons();
	
	// custom font
	if (m_hFontTree)
		m_taskTree.SetFont(m_hFontTree);

	InitEditPrompts();

	m_cpColour.SetWindowText(CString((LPCTSTR)IDS_SAMPLETEXT));
	
	// tree drag drop
	m_treeDragDrop.Initialize(this);
	
	// enabled states
	UpdateControls();
	SetFocusToTasks();

	// notify parent that we have been created
	CWnd* pParent = GetParent();

	if (pParent)
		pParent->SendMessage(WM_PARENTNOTIFY, MAKEWPARAM(WM_CREATE, GetDlgCtrlID()), (LPARAM)GetSafeHwnd());

	// and start the track timer
	// which runs persistently
	SetTimer(TIMER_TRACK, TIMETRACKPERIOD, NULL);

	// and the time which checks for midnight (day changeover)
	SetTimer(TIMER_MIDNIGHT, MIDNIGHTPERIOD, NULL);
	
	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CToDoCtrl::LoadTaskIcons()
{
	const COLORREF MAGENTA = RGB(255, 0, 255);

	VERIFY(m_ilTaskIcons.LoadImages(m_sLastSavePath, MAGENTA, HasStyle(TDCS_SHOWDEFAULTTASKICONS)));

	OnTaskIconsChanged();
}

BOOL CToDoCtrl::CreateContentControl(BOOL bResendComments)
{
	CPreferences prefs;
	CString sKey = GetPreferencesKey(); // no subkey

	// save outgoing content prefs provided they've already been loaded
	if (!m_bFirstLoadCommentsPrefs)
	{
		m_ctrlComments.SavePreferences(prefs, sKey);
	}

	// check to see if we really need to (re)create the control
	if (m_ctrlComments.IsFormat(m_cfComments))
	{
		// load preferences
		m_ctrlComments.LoadPreferences(prefs, sKey);

		// only reset this flag if we have a tasklist attached
		if (!GetFilePath().IsEmpty())
			m_bFirstLoadCommentsPrefs = FALSE;

		return TRUE;
	}

	// create new content control
	CRect rect(0, 211, 171, 238); // in DLU
	CDlgUnits(this).ToPixels(rect);

	if (m_ctrlComments.GetSafeHwnd())
	{
		::GetWindowRect(m_ctrlComments, &rect);
		ScreenToClient(rect);
	}

	DWORD dwVisStyle = (IsCommentsVisible(TRUE) ? WS_VISIBLE : 0);
	DWORD dwStyle = (dwVisStyle | WS_TABSTOP | WS_CHILD | WS_CLIPSIBLINGS); 

	BOOL bSuccess = m_mgrContent.CreateContentControl(m_cfComments, m_ctrlComments, 
													IDC_COMMENTS, dwStyle, WS_EX_CLIENTEDGE, rect, *this);

	if (!bSuccess)
	{
		// now that we are going to fallback on m_cfDefault
		// check to see if we really need to (re)create the control
		if (!m_ctrlComments.IsFormat(m_cfDefault))
		{
			bSuccess = m_mgrContent.CreateContentControl(m_cfDefault, m_ctrlComments, 
														IDC_COMMENTS, dwStyle, WS_EX_CLIENTEDGE, rect, *this);
		}
		else
			bSuccess = TRUE;

		// update comments type
		if (bSuccess)
			m_cfComments = m_cfDefault;
	}

	if (bSuccess)
	{
		// set the font
		UpdateCommentsFont(bResendComments);

		// make sure its in the right pos in the tab order
		::SetWindowPos(m_ctrlComments, ::GetDlgItem(*this, IDC_COMMENTSLABEL), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		// (re)set the edit prompt
		m_mgrPrompts.SetEditPrompt(m_ctrlComments, IDS_TDC_EDITPROMPT_COMMENTS);

		if (CThemed::IsAppThemed())
			m_ctrlComments.SetUITheme(m_theme);

		// load preferences
		m_ctrlComments.LoadPreferences(prefs, sKey);
	}

	return bSuccess; 
}

void CToDoCtrl::InitEditPrompts()
{
	m_mgrPrompts.SetEditPrompt(IDC_PROJECTNAME, *this, IDS_TDC_EDITPROMPT_PROJECT);
	m_mgrPrompts.SetEditPrompt(m_eExternalID, IDS_TDC_EDITPROMPT_EXTID);
	m_mgrPrompts.SetEditPrompt(m_eDependency, IDS_TDC_EDITPROMPT_DEPENDS); 

	m_mgrPrompts.SetComboPrompt(m_cbFileRef, IDS_TDC_EDITPROMPT_FILEREF);
	m_mgrPrompts.SetComboPrompt(m_cbAllocBy, IDS_TDC_EDITPROMPT_NAME);
	m_mgrPrompts.SetComboPrompt(m_cbAllocTo, IDS_TDC_EDITPROMPT_NAME);
	m_mgrPrompts.SetComboPrompt(m_cbCategory, IDS_TDC_EDITPROMPT_CATEGORY);
	m_mgrPrompts.SetComboPrompt(m_cbTags, IDS_TDC_EDITPROMPT_TAGS);
	m_mgrPrompts.SetComboPrompt(m_cbStatus, IDS_TDC_EDITPROMPT_STATUS);
	m_mgrPrompts.SetComboPrompt(m_cbVersion, IDS_TDC_EDITPROMPT_VER);
	
	m_mgrPrompts.SetComboPrompt(m_cbTimeDue.GetSafeHwnd(), CTimeHelper::FormatClockTime(23, 59));
	m_mgrPrompts.SetComboPrompt(m_cbTimeStart.GetSafeHwnd(), CTimeHelper::FormatClockTime(0, 0));

	m_mgrPrompts.SetEditPrompt(m_ctrlComments, IDS_TDC_EDITPROMPT_COMMENTS);

	// tree handles its own
	m_taskTree.SetWindowPrompt(CEnString(IDS_TDC_TASKLISTPROMPT));
}

BOOL CToDoCtrl::SetTreeFont(HFONT hFont)
{
	m_hFontTree = hFont;

	if (m_taskTree.GetSafeHwnd())
	{
		if (!hFont)
			hFont = CDialogHelper::GetFont(GetParent());

		UpdateCommentsFont(TRUE);
		return m_taskTree.SetFont(hFont);
	}

	// no change
	return FALSE;
}

BOOL CToDoCtrl::SetCommentsFont(HFONT hFont)
{
	m_hFontComments = hFont;

	return UpdateCommentsFont(TRUE);
}

BOOL CToDoCtrl::UpdateCommentsFont(BOOL bResendComments)
{
	if (m_ctrlComments.GetSafeHwnd())
	{
		HFONT hCurFont = CDialogHelper::GetFont(m_ctrlComments), hFont = NULL;

		if (HasStyle(TDCS_COMMENTSUSETREEFONT))
			hFont = m_hFontTree;
		else
			hFont = m_hFontComments;

		if (!hFont)
			hFont = CDialogHelper::GetFont(GetParent());

		if (GraphicsMisc::SameFontNameSize(hFont, hCurFont))
			return FALSE;

		m_ctrlComments.SendMessage(WM_SETFONT, (WPARAM)hFont, TRUE);

		// we've had some trouble with plugins using the
		// richedit control so after a font change we always
		// resend the content
		if (bResendComments)
		{
			if (m_customComments.GetLength())
			{
				if (!m_ctrlComments.SetContent(m_customComments, FALSE))
					m_ctrlComments.SetTextContent(m_sTextComments, FALSE);
			}
			else
			{
				m_ctrlComments.SetTextContent(m_sTextComments, FALSE);
			}
		}

		return TRUE;
	}

	return FALSE;
}

void CToDoCtrl::ResizeAttributeColumnsToFit()
{
	m_taskTree.RecalcAllColumnWidths();
}

void CToDoCtrl::SetMaximizeState(TDC_MAXSTATE nState)
{
	HandleUnsavedComments();

	if (m_nMaxState != nState)
	{
		m_nMaxState = nState;

		if (GetSafeHwnd())
		{
			Invalidate(FALSE);
			Resize();
			UpdateControls(FALSE); // don't update comments
			
			// make sure focus is set correctly
			switch (nState)
			{
			case TDCMS_NORMAL:
				m_taskTree.EnsureSelectionVisible();
				break;
				
			case TDCMS_MAXTASKLIST:
				if (!HasStyle(TDCS_SHOWCOMMENTSALWAYS) || !m_ctrlComments.HasFocus())
				{
					SetFocusToTasks();
				}
				break;
				
			case TDCMS_MAXCOMMENTS:
				m_ctrlComments.SetFocus();
				break;
			}
		}
	}
}

void CToDoCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CRuntimeDlg::OnSize(nType, cx, cy);
	
	EndLabelEdit(TRUE);
	ValidateCommentsSize();
	Resize(cx, cy);
}

void CToDoCtrl::Resize(int cx, int cy, BOOL bSplitting)
{
	if (m_taskTree.GetSafeHwnd())
	{
		if (!cx && !cy)
		{
			CRect rClient;
			GetClientRect(rClient);
			
			cx = rClient.right;
			cy = rClient.bottom;
		}
			
		// don't resize for unexpectedly small rects
		if ((cx < MINNONCOMMENTWIDTH) || (cy < MINNONCOMMENTHEIGHT))
		{
			return;
		}

		// hide unused controls
		ShowHideControls();

		// written to use DeferWindowPos()
		CRect rAvailable(0, 0, cx, cy);
		{
			CDeferWndMove dwm(100);

			if (GetStackCommentsAndControls())
			{
				ReposControls(&dwm, rAvailable, bSplitting);
				ReposComments(&dwm, rAvailable);
			}
			else
			{
				ReposComments(&dwm, rAvailable);
				ReposControls(&dwm, rAvailable, bSplitting);
			}

			ReposProjectName(&dwm, rAvailable);
			ReposTaskTree(&dwm, rAvailable);
		}

		UpdateWindow();
		UpdateSelectedTaskPath();
	}
}

void CToDoCtrl::ReposProjectName(CDeferWndMove* pDWM, CRect& rAvailable)
{
	// project name
	CDlgUnits dlu(this);
	CRect rLabel = GetCtrlRect(IDC_PROJECTLABEL); 
	CRect rProject = GetCtrlRect(IDC_PROJECTNAME); 

	int nOffset = rAvailable.left - rLabel.left;

	rLabel.OffsetRect(nOffset, 0);
	rProject.left += nOffset;
	rProject.right = rAvailable.right;

	pDWM->MoveWindow(GetDlgItem(IDC_PROJECTLABEL), rLabel);
	pDWM->MoveWindow(GetDlgItem(IDC_PROJECTNAME), rProject);

	if (m_nMaxState != TDCMS_MAXTASKLIST && HasStyle(TDCS_SHOWPROJECTNAME))
	{
		rAvailable.top = rProject.bottom + 5;
	}
	else
		rAvailable.top = rProject.top;
}

BOOL CToDoCtrl::CalcRequiredControlsRect(const CRect& rAvailable, CRect& rRequired, 
										 int& nCols, int& nRows, BOOL bPreserveSplitPos) const
{
	rRequired.SetRectEmpty();
	nCols = nRows = 0;

	if (m_nMaxState == TDCMS_MAXTASKLIST) // nothing to do
		return FALSE;

	// first count up the visible controls
	// so we can allocate the correct amount of space
	CTDCControlArray aControls;
	int nVisibleCtrls = GetControls(aControls, TRUE);
	
	if (nVisibleCtrls == 0) // nothing to do
		return FALSE;
	
	// figure out number of ctrl rows / columns
	nCols = nRows = 2;
	BOOL bStackedWithComments = GetStackCommentsAndControls();

	CDlgUnits dlu(this);
	
	int nCtrlHeight = dlu.ToPixelsY(2 * CTRLHEIGHT + CTRLVSPACING);
	int nCtrlWidth = dlu.ToPixelsX(CTRLCTRLLEN + CTRLHSPACING);
	
	if (HasStyle(TDCS_AUTOREPOSCTRLS))
	{
		int nAvailHeight = -1, nAvailWidth = -1;

		switch (m_nControlsPos)
		{
		case TDCUIL_RIGHT: // vertical
		case TDCUIL_LEFT: // vertical
			if (bStackedWithComments && bPreserveSplitPos)
			{
				nAvailWidth = m_nCommentsSize;
			}
			else
			{
				nAvailHeight = rAvailable.Height();
			}
			break;
			
		case TDCUIL_BOTTOM: // horizontal
			if (bStackedWithComments && bPreserveSplitPos)
			{
				nAvailHeight = m_nCommentsSize;
			}
			else
			{
				// To account of the 'extra' CTRLHSPACING that will occur
				// after the last column we add it into our calculations
				nAvailWidth = rAvailable.Width();
			}
			break;
		}

		if (nAvailHeight > 0)
		{
			// To account of the 'extra' CTRLVSPACING that will occur
			// after the last column we add it into our calculations
			nAvailHeight += dlu.ToPixelsY(CTRLVSPACING);

			nRows = max(2, nAvailHeight / nCtrlHeight);
			nCols = (nVisibleCtrls / nRows) + ((nVisibleCtrls % nRows) ? 1 : 0);

			// recalc actual rows used
			nRows = (nVisibleCtrls / nCols) + ((nVisibleCtrls % nCols) ? 1 : 0);
		}
		else if (nAvailWidth > 0)
		{
			// To account of the 'extra' CTRLHSPACING that will occur
			// after the last column we add it into our calculations
			nAvailWidth += dlu.ToPixelsX(CTRLHSPACING);

			nCols = max(2, nAvailWidth / nCtrlWidth);
			nRows = (nVisibleCtrls / nCols) + ((nVisibleCtrls % nCols) ? 1 : 0);
		}
	}
	else // fixed controls
	{
		switch (m_nControlsPos)
		{
		case TDCUIL_RIGHT: // vertical
		case TDCUIL_LEFT:
			{
				nRows = 12;
				nCols = (nVisibleCtrls / nRows) + ((nVisibleCtrls % nRows) ? 1 : 0);

				// recalc actual rows used
				nRows = (nVisibleCtrls / nCols) + ((nVisibleCtrls % nCols) ? 1 : 0);
			}
			break;
			
		case TDCUIL_BOTTOM: // horizontal
			{
				nCols = 2;
				nRows = (nVisibleCtrls / nCols) + ((nVisibleCtrls % nCols) ? 1 : 0);
			}
			break;
		}
	}
	
	// Calc ctrl rect
	// remembering to take account of the 'taking-account' 
	// of the 'extra' spacing above
	rRequired = rAvailable;
	
	int nRequiredWidth = ((nCols * nCtrlWidth) - dlu.ToPixelsX(CTRLHSPACING));
	int nRequiredHeight = ((nRows * nCtrlHeight) - dlu.ToPixelsY(CTRLVSPACING));

	switch (m_nControlsPos)
	{
	case TDCUIL_RIGHT: // vertical
		if (bStackedWithComments)
		{
			rRequired.top = (rRequired.bottom - nRequiredHeight);

			if (bPreserveSplitPos)
				nRequiredWidth = m_nCommentsSize;
		}
		else
		{
			rRequired.top += 2;
		}
		rRequired.left = (rRequired.right - nRequiredWidth);
		break;
		
	case TDCUIL_LEFT:
		if (bStackedWithComments)
		{
			rRequired.top = (rRequired.bottom - nRequiredHeight);
	
			if (bPreserveSplitPos)
				nRequiredWidth = m_nCommentsSize;
		}
		else
		{
			rRequired.top += 2;
		}
		rRequired.right = (rRequired.left + nRequiredWidth);
		break;
		
	case TDCUIL_BOTTOM: // horizontal
		if (bStackedWithComments)
		{
			rRequired.left = (rRequired.right - nRequiredWidth);

			// align controls at top
			rRequired.top = (rAvailable.bottom - m_nCommentsSize);
			rRequired.bottom = (rRequired.top + nRequiredHeight);
		}
		else
		{
			rRequired.top = (rRequired.bottom - nRequiredHeight);
		}
		break;
	}

	return TRUE;
}

void CToDoCtrl::ReposControls(CDeferWndMove* pDWM, CRect& rAvailable, BOOL bSplitting)
{
	// only required when controls are visible
	if (m_nMaxState != TDCMS_NORMAL)
		return;
	
	CRect rCtrls;
	int nCols, nRows;

	// First we try to preserve the current split size
	if (!CalcRequiredControlsRect(rAvailable, rCtrls, nCols, nRows, TRUE))
		return;
	
	// If stacking and not splitting, we then check whether 
	// that leaves enough space for the comments, and if not 
	// we resize again allowing the splitter pos to move
	BOOL bStackCommentsAndControls = GetStackCommentsAndControls();
	BOOL bCtrlsFit = TRUE;

	if (!bSplitting && bStackCommentsAndControls)
	{
		const int MIN_STACKED_COMMENT_SIZE = 60;
		CRect rStackedAvail(rAvailable);

		switch (m_nControlsPos)
		{
		case TDCUIL_RIGHT: // vertical
		case TDCUIL_LEFT:
			bCtrlsFit = ((rCtrls.top - rAvailable.top) >= MIN_STACKED_COMMENT_SIZE);

			if (!bCtrlsFit)
				rStackedAvail.top += MIN_STACKED_COMMENT_SIZE;
			break;
			
		case TDCUIL_BOTTOM: // horizontal
			bCtrlsFit = ((rCtrls.left - rAvailable.left) >= MIN_STACKED_COMMENT_SIZE);
			
			if (!bCtrlsFit)
				rStackedAvail.left += MIN_STACKED_COMMENT_SIZE;
			break;
		}

		if (!bCtrlsFit)
		{
			// Allow split pos to move
			if (!CalcRequiredControlsRect(rStackedAvail, rCtrls, nCols, nRows, FALSE))
				return;
		}
	}

	if (!bCtrlsFit)
	{
		// adjust splitter pos
		int nNewCommentsSize = m_nCommentsSize;

		switch (m_nControlsPos)
		{
		case TDCUIL_RIGHT: // vertical
			nNewCommentsSize = (rAvailable.right - rCtrls.left);
			break;
			
		case TDCUIL_LEFT:
			nNewCommentsSize = rCtrls.right;
			break;
			
		case TDCUIL_BOTTOM: // horizontal
			nNewCommentsSize = rCtrls.top;
			break;
		}

		if (nNewCommentsSize > 0)
			s_nCommentsSize = m_nCommentsSize = nNewCommentsSize;
	}
	else if (!bStackCommentsAndControls) // adjust available area
	{
		switch (m_nControlsPos)
		{
		case TDCUIL_RIGHT: // vertical
			rAvailable.right = rCtrls.left - SPLITSIZE;
			break;
			
		case TDCUIL_LEFT:
			rAvailable.left = rCtrls.right + SPLITSIZE;
			break;
			
		case TDCUIL_BOTTOM: // horizontal
			rAvailable.bottom = rCtrls.top - SPLITSIZE;
			break;
		}
	}

	// now iterate the visible controls settings their positions dynamically
	CTDCControlArray aControls;
	int nVisibleCtrls = GetControls(aControls, TRUE);
	
	ASSERT(nVisibleCtrls);
	
	CDlgUnits dlu(this);
	CRect rItem(rCtrls);

	rItem.bottom = rItem.top + dlu.ToPixelsY(2 * CTRLHEIGHT);
	rItem.right = rItem.left + dlu.ToPixelsX(CTRLCTRLLEN);
	
	int nCol = 0;
	int nRightEdge = rCtrls.right;
	
	for (int nCtrl = 0; nCtrl < aControls.GetSize(); nCtrl++)
	{
		const CTRLITEM& ctrl = aControls[nCtrl];
		ASSERT(IsCtrlShowing(ctrl));
		
		ReposControl(ctrl, pDWM, &dlu, rItem, nRightEdge);
		
		nCol++;
		
		// if we've passed the column count, reset to next row
		if (nCol >= nCols)
		{
			nCol = 0;
			rItem.OffsetRect(-(rItem.left - rCtrls.left), dlu.ToPixelsY(2 * CTRLHEIGHT + CTRLVSPACING));
		}
		else // offset to next column
		{
			rItem.OffsetRect(dlu.ToPixelsX(CTRLCTRLLEN + CTRLHSPACING), 0);
		}
	}
}

int CToDoCtrl::GetControls(CTDCControlArray& aControls, BOOL bVisible) const
{
	aControls.RemoveAll();

	// standard controls, except for file link which is always last
	int nCtrl;
	for (nCtrl = 0; nCtrl < NUM_CTRLITEMS - 1; nCtrl++)
	{
		CTRLITEM ctrl = CTRLITEMS[nCtrl];

		if (!bVisible || IsCtrlShowing(ctrl))
			aControls.Add(ctrl);
	}
	
	// custom attribs
	for (nCtrl = 0; nCtrl < m_aCustomControls.GetSize(); nCtrl++)
	{
		CUSTOMATTRIBCTRLITEM ctrl = m_aCustomControls[nCtrl];
		aControls.Add(ctrl);

		// Buddy Control
		CTRLITEM buddy;

		if (ctrl.GetBuddy(buddy))
			aControls.Add(buddy);
	}

	// finally file link
	CTRLITEM ctrlFileRef = CTRLITEMS[NUM_CTRLITEMS - 1];

	if (IsCtrlShowing(ctrlFileRef))
		aControls.Add(ctrlFileRef);

	return aControls.GetSize();
}

void CToDoCtrl::ReposControl(const CTRLITEM& ctrl, CDeferWndMove* pDWM, const CDlgUnits* pDLU, 
							 const CRect& rItem, int nClientRight)
{
	// move label
	CRect rLabel(rItem);
	rLabel.bottom = rLabel.top + pDLU->ToPixelsY(CTRLHEIGHT);

	pDWM->MoveWindow(GetDlgItem(ctrl.nLabelID), rLabel);
				
	// move control
	CRect rCtrl(rItem);
	rCtrl.top += pDLU->ToPixelsY(CTRLHEIGHT);
				
	// some special cases
	switch (ctrl.nCtrlID)
	{
	case IDC_PERCENT:
		{
			CRect rSpin = GetCtrlRect(IDC_PERCENTSPIN); // gets current pos
			rSpin.OffsetRect(rCtrl.right - rSpin.right, 0);
			rSpin.top = rCtrl.top;
			rSpin.bottom = rCtrl.bottom;
			pDWM->MoveWindow(&m_spinPercent, rSpin);
			
			rCtrl.right = rSpin.left;
		}
		break;
		
		// fall thru for 
		
	case IDC_ALLOCTO:
	case IDC_ALLOCBY:
	case IDC_STATUS:
	case IDC_CATEGORY:
	case IDC_TAGS:
	case IDC_PRIORITY:
	case IDC_VERSION:
	case IDC_RISK:
	case IDC_FILEPATH:
		{
			// file path control can take as much space as is left
			if (ctrl.nCtrlID == IDC_FILEPATH)
				rCtrl.right = nClientRight;

			// don't move it if it hasn't actually moved
			// to prevent flickering. Note we do this 
			// before we add 200 to the bottom of the rect
			CRect rPos;
			GetCtrlRect(ctrl.nCtrlID);

			if (rPos == rCtrl)
				return; 

			// else
			rCtrl.bottom += 200; // combo box drop height
		}
		break;

	// handle custom attributes
	default:
		if (CTDCCustomAttributeHelper::IsCustomControl(ctrl.nCtrlID))
		{
			TDCCUSTOMATTRIBUTEDEFINITION attribDef;

			if (CTDCCustomAttributeHelper::GetAttributeDef(ctrl.nAttrib, m_aCustomAttribDefs, attribDef))
			{
				if (attribDef.IsList())
				{
					// same as combos above
					CRect rPos;
					GetCtrlRect(ctrl.nCtrlID);
					
					if (rPos == rCtrl)
						return; 
					
					// else
					rCtrl.bottom += 200; // combo box drop height
				}
			}
		}
		break;
	}

	pDWM->MoveWindow(GetDlgItem(ctrl.nCtrlID), rCtrl);
}

void CToDoCtrl::ReposTaskTree(CDeferWndMove* pDWM, const CRect& rAvailable)
{
	pDWM->MoveWindow(&m_taskTree, rAvailable);
}

BOOL CToDoCtrl::IsSplitterVisible() const
{
	return ((m_nMaxState == TDCMS_NORMAL) || 
			((m_nMaxState == TDCMS_MAXTASKLIST) && HasStyle(TDCS_SHOWCOMMENTSALWAYS)));
}

BOOL CToDoCtrl::IsCommentsVisible(BOOL bActually) const
{
	if (m_nMaxState == TDCMS_MAXCOMMENTS)
		return TRUE; // always

	// comments are visible if splitter is
	BOOL bVisible = IsSplitterVisible();

	// check optionally for actual size
	if (bActually)
		bVisible &= (m_nCommentsSize > 0);

	return bVisible;
}

void CToDoCtrl::ReposComments(CDeferWndMove* pDWM, CRect& rAvailable /*in/out*/) 
{
	CRect rComments(rAvailable);

	BOOL bMaxTasklist = (m_nMaxState == TDCMS_MAXTASKLIST);
	BOOL bMaxComments = (m_nMaxState == TDCMS_MAXCOMMENTS);

	if (bMaxComments)
	{
		rAvailable.SetRectEmpty();
	}
	else if (bMaxTasklist && !HasStyle(TDCS_SHOWCOMMENTSALWAYS))
	{
		rComments.SetRectEmpty();
	}
	else
	{
		BOOL bStackCommentsAndControls = (GetStackCommentsAndControls() && !GetVisibleEditFields().IsEmpty());
		int nCommentSize = (HasStyle(TDCS_SHAREDCOMMENTSHEIGHT) ? s_nCommentsSize : m_nCommentsSize);
	
		if (bStackCommentsAndControls)
		{
			CRect rCtrls;
			int nCols, nRows;
			
			if (CalcRequiredControlsRect(rAvailable, rCtrls, nCols, nRows, TRUE))
			{
				switch (m_nCommentsPos)
				{
				case TDCUIL_RIGHT: // vertical
					rComments.top += 2;
					rComments.bottom = rCtrls.top;
					rComments.left = rAvailable.right - nCommentSize;
					rComments.right = rCtrls.right;

					rAvailable.right = rComments.left - SPLITSIZE;
					break;

				case TDCUIL_LEFT: // vertical
					rComments.top += 2;
					rComments.bottom = rCtrls.top;
					rComments.left = rAvailable.left;
					rComments.right = rAvailable.left + nCommentSize;

					rAvailable.left = rComments.right + SPLITSIZE;
					break;
					
				case TDCUIL_BOTTOM: // horizontal
					rComments.right = rCtrls.left - SPLITSIZE;				
					rComments.top = rAvailable.bottom - nCommentSize;				
					rComments.bottom = rAvailable.bottom;
					
					rAvailable.bottom = rComments.top - SPLITSIZE;
					break;
				}
			}
		}
		else
		{
			switch (m_nCommentsPos)
			{
			case TDCUIL_RIGHT: // vertical
				{
					rComments.top += 2;
					rComments.left = rAvailable.right - nCommentSize;

					rAvailable.right = rComments.left - SPLITSIZE;
				}
				break;
				
			case TDCUIL_LEFT:
				{
					rComments.top += 2;
					rComments.right = rAvailable.left + nCommentSize;

					rAvailable.left = rComments.right + SPLITSIZE;
				}
				break;
				
			case TDCUIL_BOTTOM: // horizontal
				{
					rComments.left = rAvailable.left;
					rComments.top = rAvailable.bottom - nCommentSize;

					rAvailable.bottom = rComments.top - SPLITSIZE;
				}
				break;
			}
		}
		
		// splitter control between comments and controls/task tree
		if (m_nCommentsPos == TDCUIL_BOTTOM)
		{
			CRect rDiv(rComments);
			
			switch (m_nCommentsPos)
			{
			case TDCUIL_RIGHT: // vertical
				{
					rDiv.right = rDiv.left - SPLITSIZE;
					rDiv.left = rDiv.right - 2;
					rDiv.bottom = rAvailable.bottom;
					
					if (!bMaxTasklist)
						rAvailable.right -= (SPLITSIZE + 2);
				}
				break;
				
			case TDCUIL_LEFT:
				{
					rDiv.left = rDiv.right + SPLITSIZE;
					rDiv.right = rDiv.left + 2;
					rDiv.bottom = rAvailable.bottom;
					
					if (!bMaxTasklist)
						rAvailable.left += (SPLITSIZE + 2);
				}
				break;
				
			case TDCUIL_BOTTOM: // horizontal
				{
					rDiv.bottom = rDiv.top - SPLITSIZE;
					rDiv.top = rDiv.bottom - 2;
					rDiv.right = rAvailable.right;

					if (!bMaxTasklist)
						rAvailable.bottom -= (SPLITSIZE + 2);
				}
				break;
			}
		}
	}

	if (!rComments.IsRectEmpty())
	{
		CDlgUnits dlu(this);

		// label
		// NOTE: label can be resized smaller so we must restore 
		// the actual size each time
		CRect rLabel = GetCtrlRect(IDC_COMMENTSLABEL);
		
		rLabel.right = rLabel.left + dlu.ToPixelsX(CTRLLABELLEN);
		rLabel.OffsetRect(rComments.left - rLabel.left, rComments.top - rLabel.top);
		
		// type combo
		// NOTE: combo can be resized smaller so we must restore 
		// the actual size each time
		CRect rCombo = GetCtrlRect(IDC_COMMENTSTYPE);
		
		rCombo.right = rCombo.left + dlu.ToPixelsX(COMMENTSTYPELEN);
		rCombo.OffsetRect(rLabel.right + 4 - rCombo.left, rComments.top - rCombo.top);
		
		if (m_nCommentsPos == TDCUIL_LEFT)
		{
			rLabel.right = min(rCombo.left, rComments.right);
			rCombo.right = min(rCombo.right, rComments.right);
		}
		rLabel.bottom = rCombo.bottom;
	
		pDWM->MoveWindow(GetDlgItem(IDC_COMMENTSLABEL), rLabel);
		pDWM->MoveWindow(&m_cbCommentsType, rCombo);
		
		// make sure drop list displays correctly
		CDialogHelper::RefreshMaxDropWidth(m_cbCommentsType);
		
		// comments below combo
		rComments.top = rCombo.bottom + 5;
		pDWM->MoveWindow(GetDlgItem(IDC_COMMENTS), rComments);
	}
}

void CToDoCtrl::ShowHideControl(const CTRLITEM& ctrl)
{
	CWnd* pCtrl = GetDlgItem(ctrl.nCtrlID);
	CWnd* pLabel = GetDlgItem(ctrl.nLabelID);
	
	if (!pCtrl || !pLabel)
		return;
	
	BOOL bCtrlShowing = IsCtrlShowing(ctrl);
	int nShowCtrl = bCtrlShowing ? SW_SHOW : SW_HIDE;
	
	pCtrl->ShowWindow(nShowCtrl);
	pLabel->ShowWindow(nShowCtrl);
	
	// some additions and modifications
	switch (ctrl.nCtrlID)
	{
	case IDC_PERCENT:
		m_spinPercent.ShowWindow(nShowCtrl);
		break;
	}
}

void CToDoCtrl::ShowHideControls()
{
	// now show/hide appropriate controls
	int nCtrl;
	for (nCtrl = 0; nCtrl < NUM_CTRLITEMS; nCtrl++)
	{
		const CTRLITEM& ctrl = CTRLITEMS[nCtrl];
		ShowHideControl(ctrl);
	}

	// always show custom controls
	for (nCtrl = 0; nCtrl < m_aCustomControls.GetSize(); nCtrl++)
	{
		const CUSTOMATTRIBCTRLITEM& ctrl = m_aCustomControls[nCtrl];
		ShowHideControl(ctrl);

		// Handle buddy control
		CTRLITEM buddy;

		if (ctrl.GetBuddy(buddy))
			ShowHideControl(buddy);
	}

	// hide comments if there is not enough space
	BOOL bCommentsVis = IsCommentsVisible(TRUE);
	ShowCtrls(IDC_COMMENTSLABEL, IDC_COMMENTS, bCommentsVis);

	// re-hide the comments type combo if necessary
	BOOL bHide = (!bCommentsVis || (m_mgrContent.GetNumContent() <= 1));
	m_cbCommentsType.ShowWindow(bHide ? SW_HIDE : SW_SHOW);

	// task tree
	UpdateTasklistVisibility();
	
	// project name
	BOOL bMaximize = (m_nMaxState != TDCMS_NORMAL);
	BOOL bShowProjectName = !bMaximize && HasStyle(TDCS_SHOWPROJECTNAME);
	ShowCtrls(IDC_PROJECTLABEL, IDC_PROJECTNAME, bShowProjectName);
}

void CToDoCtrl::EnableDisableCustomControl(const CUSTOMATTRIBCTRLITEM& ctrl, DWORD dwTaskID, BOOL bEnable, BOOL bReadOnly)
{
	// Main control first
	EnableDisableControl(ctrl, dwTaskID, bEnable, bReadOnly, FALSE, FALSE);

	// Buddy
	CTRLITEM buddy;

	if (ctrl.GetBuddy(buddy))
	{
		TDCCUSTOMATTRIBUTEDEFINITION attribDef;
		CTDCCustomAttributeHelper::GetAttributeDef(ctrl, m_aCustomAttribDefs, attribDef);

		switch (attribDef.GetDataType())
		{
		case TDCCA_STRING:
		case TDCCA_INTEGER:	
		case TDCCA_DOUBLE:	
		case TDCCA_BOOL:
		case TDCCA_ICON:
			break;
			
		case TDCCA_DATE:
			// Buddy is time
			if (bEnable && !bReadOnly)
			{
				TDCCADATA data(GetSelectedTaskCustomAttributeData(attribDef.sUniqueID, FALSE));
				
				if (!CDateHelper::IsDateSet(data.AsDate()))
				{
					bReadOnly = TRUE;

					// clear any existing value
					CTimeComboBox* pBuddy = (CTimeComboBox*)GetDlgItem(ctrl.nBuddyCtrlID);
					ASSERT(pBuddy);

					if (pBuddy)
						pBuddy->SetOleTime(-1);
				}
			}
		}
		
		EnableDisableControl(buddy, dwTaskID, bEnable, bReadOnly, FALSE, FALSE);
	}
}

void CToDoCtrl::EnableDisableControl(const CTRLITEM& ctrl, DWORD dwTaskID, BOOL bEnable, BOOL bReadOnly, BOOL bIsParent, BOOL bEditTime)
{
	CWnd* pCtrl = GetDlgItem(ctrl.nCtrlID);
	CWnd* pLabel = GetDlgItem(ctrl.nLabelID);
	
	if (!pCtrl || !pLabel)
		return;
	
	BOOL bCtrlShowing = IsCtrlShowing(ctrl);
	
	// control state
	RT_CTRLSTATE nCtrlState = (!bEnable || !bCtrlShowing) ? RTCS_DISABLED : 
								(bReadOnly ? RTCS_READONLY : RTCS_ENABLED);
	RT_CTRLSTATE nLabelState = (CThemed::IsAppThemed() && bCtrlShowing) ? RTCS_ENABLED : nCtrlState;
	
	// some additions and modifications
	switch (ctrl.nCtrlID)
	{
	case IDC_FILEPATH: // special case
		if (!bEnable)
		{
			m_cbFileRef.SetReadOnly(FALSE);
			m_cbFileRef.EnableWindow(FALSE);
		}
		else
		{
			m_cbFileRef.SetReadOnly(bReadOnly);
			m_cbFileRef.EnableWindow(TRUE);
		}
		return;

	case IDC_PERCENT:
		{
			BOOL bEditPercent = !HasStyle(TDCS_AUTOCALCPERCENTDONE);

			// restrictions on parents
			if (bEditPercent && bIsParent)
			{
				bEditPercent = (!HasStyle(TDCS_AVERAGEPERCENTSUBCOMPLETION) && 
								HasStyle(TDCS_ALLOWPARENTTIMETRACKING));
			}

			if (!bEditPercent && bEnable)
				nCtrlState = RTCS_READONLY;
			
			SetCtrlState(m_spinPercent, nCtrlState);
		}
		break;
		
	case IDC_TIMEEST:
		if (!bEditTime && bEnable)
			nCtrlState = RTCS_READONLY;
		break;
		
	case IDC_TIMESPENT:
		{
			BOOL bIsClocking = m_data.IsTaskTimeTrackable(dwTaskID) && (dwTaskID == m_dwTimeTrackTaskID);
			
			if ((!bEditTime || bIsClocking) && bEnable)
				nCtrlState = RTCS_READONLY;
		}
		break;
		
	case IDC_DUETIME:
		if ((nCtrlState == RTCS_ENABLED) && !SelectedTaskHasDate(TDCD_DUE))
			nCtrlState = RTCS_READONLY;
		break;
		
	case IDC_STARTTIME:
		if ((nCtrlState == RTCS_ENABLED) && !SelectedTaskHasDate(TDCD_START))
			nCtrlState = RTCS_READONLY;
		break;
		
	case IDC_DONETIME:
		if ((nCtrlState == RTCS_ENABLED) && !SelectedTaskHasDate(TDCD_DONE))
			nCtrlState = RTCS_READONLY;
		break;
	}
	
	SetCtrlState(*pLabel, nLabelState);
	SetCtrlState(*pCtrl, nCtrlState);
}

void CToDoCtrl::EnableDisableControls(HTREEITEM hti)
{
	DWORD dwTaskID = GetTaskID(hti);
	
	BOOL bMaximize = (m_nMaxState != TDCMS_NORMAL);
	BOOL bEnable = (hti && !bMaximize);
	BOOL bIsParent = TSH().ItemsAreAllParents();
	BOOL bReadOnly = IsReadOnly();
	BOOL bEditTime = (!bIsParent || HasStyle(TDCS_ALLOWPARENTTIMETRACKING));

	// now enable/disable appropriate controls
	int nCtrl;
	for (nCtrl = 0; nCtrl < NUM_CTRLITEMS; nCtrl++)
	{
		const CTRLITEM& ctrl = CTRLITEMS[nCtrl];
		EnableDisableControl(ctrl, dwTaskID, bEnable, bReadOnly, bIsParent, bEditTime);
	}

	// and custom controls
	for (nCtrl = 0; nCtrl < m_aCustomControls.GetSize(); nCtrl++)
	{
		const CUSTOMATTRIBCTRLITEM& ctrl = m_aCustomControls[nCtrl];
		EnableDisableCustomControl(ctrl, dwTaskID, bEnable, bReadOnly);
	}

	// comments
	CString sCommentsType;
	GetSelectedTaskCustomComments(sCommentsType);
	BOOL bEditComments = (m_mgrContent.FindContent(sCommentsType) != -1);
	
	BOOL bCommentsVis = IsCommentsVisible();
	RT_CTRLSTATE nCtrlState = (!bCommentsVis || !hti) ? RTCS_DISABLED : 
							((bReadOnly || !bEditComments) ? RTCS_READONLY : RTCS_ENABLED);

	switch (nCtrlState)
	{
	case RTCS_DISABLED:
		SetCtrlState(this, IDC_COMMENTS, nCtrlState);
		break;

	case RTCS_ENABLED:
		SetCtrlState(this, IDC_COMMENTS, nCtrlState);
		m_ctrlComments.SetReadOnly(FALSE);
		break;

	case RTCS_READONLY:
		SetCtrlState(this, IDC_COMMENTS, RTCS_ENABLED);
		m_ctrlComments.SetReadOnly(TRUE);
		break;
	}

	// if bEditComments is FALSE it means we have multiple comments types
	// selected. So we enable the selected comments type but keep the 
	// comments control readonly
	if (nCtrlState == RTCS_READONLY && !bReadOnly)
		SetCtrlState(this, IDC_COMMENTSTYPE, RTCS_ENABLED);
	else
		SetCtrlState(this, IDC_COMMENTSTYPE, nCtrlState);

	RT_CTRLSTATE nLabelState = (CThemed::IsAppThemed() && bCommentsVis) ? RTCS_ENABLED : nCtrlState;
	SetCtrlState(this, IDC_COMMENTSLABEL, nLabelState);

	// project name
	BOOL bShowProjectName = !bMaximize && HasStyle(TDCS_SHOWPROJECTNAME);
	nCtrlState = !bShowProjectName ? RTCS_DISABLED : (bReadOnly ? RTCS_READONLY : RTCS_ENABLED);
	SetCtrlState(this, IDC_PROJECTNAME, nCtrlState);

	nLabelState = (CThemed::IsAppThemed() && bShowProjectName) ? RTCS_ENABLED : nCtrlState;
	SetCtrlState(this, IDC_PROJECTLABEL, nCtrlState);
}

int CToDoCtrl::CalcMaxCommentSize() const
{
	CRect rClient;
	GetClientRect(rClient);

	switch (m_nCommentsPos)
	{
	case TDCUIL_LEFT:
	case TDCUIL_RIGHT:
		return (rClient.Width() - MINNONCOMMENTWIDTH);

	case TDCUIL_BOTTOM:
		return (rClient.Height() - MINNONCOMMENTHEIGHT);
	}

	// all else
	ASSERT(0);
	return -1;
}

BOOL CToDoCtrl::IsCtrlShowing(const CTRLITEM& ctrl) const
{
	if (m_nMaxState != TDCMS_NORMAL)
		return FALSE;

	// is this a custom control?
	if (CTDCCustomAttributeHelper::IsCustomControl(ctrl.nCtrlID))
		return TRUE;

	// other special cases
	switch (ctrl.nCtrlID)
	{
	case IDC_COLOUR:
		if (m_visColAttrib.GetShowEditsAndFilters() == TDLSA_ASCOLUMN)
		{
			return (!HasStyle(TDCS_COLORTEXTBYATTRIBUTE) &&
					!HasStyle(TDCS_COLORTEXTBYPRIORITY) &&
					!HasStyle(TDCS_COLORTEXTBYNONE));
		}
		break;
	}
	
	// all else
	return m_visColAttrib.IsEditFieldVisible(ctrl.nAttrib);
}

void CToDoCtrl::UpdateSelectedTaskPath()
{
	m_taskTree.UpdateSelectedTaskPath();
}

void CToDoCtrl::UpdateControls(BOOL bIncComments, HTREEITEM hti)
{
	if (!hti)
		hti = GetSelectedItem();
	
	BOOL bReadOnly = IsReadOnly();
	CString sCommentsType(m_cfDefault);
	
	if (hti)
	{
		int nSelCount = GetSelectedCount();
		DWORD dwTaskID = GetTrueTaskID(hti); 

		BOOL bMaximize = (m_nMaxState != TDCMS_NORMAL);
		BOOL bEnable = (hti && !bMaximize);
		BOOL bIsParent = TSH().ItemsAreAllParents();
		BOOL bAveSubTaskCompletion = HasStyle(TDCS_AVERAGEPERCENTSUBCOMPLETION) && bIsParent;
		BOOL bEditTime = !bIsParent || HasStyle(TDCS_ALLOWPARENTTIMETRACKING);
		BOOL bEditPercent = !HasStyle(TDCS_AUTOCALCPERCENTDONE) && (nSelCount > 1 || !bAveSubTaskCompletion);

		m_nPriority = (int)GetSelectedTaskPriority();
		m_nRisk = (int)GetSelectedTaskRisk();
		m_sAllocBy = GetSelectedTaskAllocBy();
		m_sStatus = GetSelectedTaskStatus();
		m_sExternalID = GetSelectedTaskExtID();
		m_sVersion = GetSelectedTaskVersion();
		m_crColour = GetSelectedTaskColor();
		m_dCost = GetSelectedTaskCost();

		if (m_crColour == 0)
			m_crColour = CLR_DEFAULT;

		// can only edit comments if the comments type for the task is available
		if (bIncComments)
		{
			m_sTextComments = GetSelectedTaskComments();
			m_customComments = GetSelectedTaskCustomComments(sCommentsType);
		}
		
		// special cases
		GetSelectedTaskCategories(m_aCategory);
		GetSelectedTaskAllocTo(m_aAllocTo);
		GetSelectedTaskTags(m_aTags);
		GetSelectedTaskFileRefs(m_aFileRefs, FALSE); // relative paths

		CStringArray aDepends;
		GetSelectedTaskDependencies(aDepends);
		m_sDepends = Misc::FormatArray(aDepends);

		if (bEditTime)
		{
			m_dTimeEstimate = GetSelectedTaskTimeEstimate(m_nTimeEstUnits);
			m_dTimeSpent = GetSelectedTaskTimeSpent(m_nTimeSpentUnits);
		}
		else
		{
			m_nTimeEstUnits = m_tdiDefault.nTimeEstUnits;
			m_dTimeEstimate = m_data.CalcTaskTimeEstimate(dwTaskID, m_nTimeEstUnits);

			m_nTimeSpentUnits = m_tdiDefault.nTimeSpentUnits;
			m_dTimeSpent = m_data.CalcTaskTimeSpent(dwTaskID, m_nTimeEstUnits);
		}

		// chess clock for time spent
		BOOL bCanTimeTrack = (bEditTime && 
								bEnable && 
								!bReadOnly && 
								(nSelCount == 1) && 
								m_data.IsTaskTimeTrackable(dwTaskID));

		m_eTimeSpent.CheckButton(ID_TIME_TRACK, (dwTaskID == m_dwTimeTrackTaskID));
		m_eTimeSpent.EnableButton(ID_TIME_TRACK, bCanTimeTrack);

		// dependency link button
		m_eDependency.EnableButton(ID_DEPENDS_LINK, bEnable && !m_sDepends.IsEmpty());

		// percent done
		if (IsSelectedTaskDone())
			m_nPercentDone = 100;
		
		else if (bEditPercent)
			m_nPercentDone = GetSelectedTaskPercent();
		else
			m_nPercentDone = m_data.CalcTaskPercentDone(dwTaskID);		
		
		// special handling for start date/time
		COleDateTime dateStart = GetSelectedTaskDate(TDCD_START);
		SetCtrlDate(m_dtcStart, dateStart);
		m_cbTimeStart.SetOleTime(dateStart.m_dt);

		// special handling for due date/time
		COleDateTime dateDue = GetSelectedTaskDate(TDCD_DUE);
		SetCtrlDate(m_dtcDue, dateDue, dateStart);
		m_cbTimeDue.SetOleTime(dateDue.m_dt);

		// special handling for done date/time
		COleDateTime dateDone = GetSelectedTaskDate(TDCD_DONE);
		SetCtrlDate(m_dtcDone, dateDone);
		m_cbTimeDone.SetOleTime(dateDone.m_dt);

		// recurrence
		GetSelectedTaskRecurrence(m_tRecurrence);

		// use due date if present else start date
		if (CDateHelper::IsDateSet(dateDue))
			m_eRecurrence.SetDefaultDate(dateDue);
		else
			m_eRecurrence.SetDefaultDate(dateStart);

		// custom attributes
		GetSelectedTaskCustomAttributeData(m_mapCustomCtrlData, FALSE);
	}
	else // clear controls
	{
		m_sTextComments.Empty();
		m_customComments.Empty();
		m_nPriority = 0;
		m_nRisk = 0;
		m_sAllocBy.Empty();
		m_sStatus.Empty();
		m_sExternalID.Empty();
		m_sDepends.Empty();
		m_nPercentDone = 0;
		m_dTimeEstimate = m_dTimeSpent = 0;
		m_dCost = 0;
		m_tRecurrence = TDCRECURRENCE();
		m_sVersion.Empty();
		m_crColour = CLR_DEFAULT;

		m_aCategory.RemoveAll();
		m_aAllocTo.RemoveAll();
		m_aTags.RemoveAll();
		m_aFileRefs.RemoveAll();

		COleDateTime date;
		SetCtrlDate(m_dtcDue, date);
		SetCtrlDate(m_dtcDone, date);
		SetCtrlDate(m_dtcStart, date);

		m_eTimeSpent.EnableButton(ID_TIME_TRACK, FALSE);
		m_eDependency.EnableButton(ID_DEPENDS_LINK, FALSE);

		m_mapCustomCtrlData.RemoveAll();
	}

	// update data controls excluding comments
	UpdateData(FALSE);

	// recreate comments control if necessary
	// note: if more than one comments type is selected then sCommentsType
	// will be empty which will put the comments type combo in an
	// indeterminate state which is the desired effect since this requires
	// the user to reset the type before they can edit
	if (bIncComments)
	{
		m_cfComments = sCommentsType;
		m_cbCommentsType.SetSelectedFormat(m_cfComments);
		
		if (!sCommentsType.IsEmpty())
			CreateContentControl(FALSE);
		
		// update control content
		UpdateComments(FALSE);
	}

	// and task header
	UpdateSelectedTaskPath();
	
	// show hide controls
	EnableDisableControls(hti);
	
	m_treeDragDrop.EnableDragDrop(!bReadOnly);
}

void CToDoCtrl::UpdateTasklistVisibility()
{
	BOOL bTasksVis = (m_nMaxState != TDCMS_MAXCOMMENTS);

	// if only the comments are visible then set the focus to the comments
	// before hiding the tasks, else Windows will select all the text
	// in the comments control
	if (!bTasksVis)
		m_ctrlComments.SetFocus();

	m_taskTree.Show(bTasksVis);
}

void CToDoCtrl::SetCtrlDate(CDateTimeCtrl& ctrl, const COleDateTime& date, const COleDateTime& dateMin)
{
	if (CDateHelper::IsDateSet(date))
	{
		ctrl.SetTime(date);
	}
	else
	{
		COleDateTime dtToday = COleDateTime::GetCurrentTime();
		
		if (CDateHelper::IsDateSet(dateMin))
			ctrl.SetTime(max(dateMin, dtToday));
		else
			ctrl.SetTime(dtToday);
		
		ctrl.SendMessage(DTM_SETSYSTEMTIME, GDT_NONE, 0);
	}
}

void CToDoCtrl::UpdateTask(TDC_ATTRIBUTE nAttrib, DWORD dwFlags)
{
	if (!m_taskTree.GetSafeHwnd())
		return;
	
	HTREEITEM hti = GetSelectedItem();
	
	if (!hti)
		return;
	
	// special case to circumvent CSaveFocus else it can mess up IME input
	if (nAttrib == TDCA_COMMENTS)
	{
		m_nCommentsState = CS_PENDING;
		SetModified(TRUE, TDCA_COMMENTS, GetTaskID(hti));
		return;
	}	
	
	// else
	CSaveFocus sf;
	UpdateData();
	
	switch (nAttrib)
	{
	case TDCA_DONEDATE:
		{
			COleDateTime date;
			m_dtcDone.GetTime(date);
			
			if (SetSelectedTaskDate(TDCD_DONE, date, TRUE))
			{
				// check if we need to modify percent done also
				if (!IsSelectedTaskDone())
				{
					int nPercentDone = GetSelectedTaskPercent();
					
					if (nPercentDone == 100)
						nPercentDone = 0;
					
					SetSelectedTaskPercentDone(nPercentDone);
					m_nPercentDone = nPercentDone;
					
					UpdateData(FALSE);
				}
				else if (m_nPercentDone != 100) // make the percent field look right
				{
					m_nPercentDone = 100;
					UpdateData(FALSE);
				}
			}
			else
			{
				UpdateControls(FALSE); // don't update comments
			}

			// enable done time field depending on whether the done date is valid or not
			m_cbTimeDone.EnableWindow(CDateHelper::IsDateSet(date));
		}
		break;
		
	case TDCA_DONETIME:
		SetSelectedTaskDate(TDCD_DONETIME, m_cbTimeDone.GetOleTime(), TRUE);
		break;
		
	case TDCA_STARTDATE:
		{
			COleDateTime date;
			m_dtcStart.GetTime(date);
			
			SetSelectedTaskDate(TDCD_STARTDATE, date, TRUE);

			// enable start time field depending on whether the start date is valid or not
			m_cbTimeStart.EnableWindow(CDateHelper::IsDateSet(date));
		}
		break;
		
	case TDCA_STARTTIME:
		SetSelectedTaskDate(TDCD_STARTTIME, m_cbTimeStart.GetOleTime(), TRUE);
		break;
		
	case TDCA_DUEDATE:
		{
			COleDateTime date;
			m_dtcDue.GetTime(date);
			
			SetSelectedTaskDate(TDCD_DUEDATE, date, TRUE);

			// enable due time field depending on whether the due date is valid or not
			m_cbTimeDue.EnableWindow(CDateHelper::IsDateSet(date));
		}
		break;
		
	case TDCA_DUETIME:
		SetSelectedTaskDate(TDCD_DUETIME, m_cbTimeDue.GetOleTime(), TRUE);
		break;
		
	case TDCA_COST:
		SetSelectedTaskCost(m_dCost);
		break;
		
	case TDCA_RECURRENCE:
		SetSelectedTaskRecurrence(m_tRecurrence);
		break;
		
	case TDCA_DEPENDENCY:
		{
			CStringArray aDepends;

			Misc::Split(m_sDepends, aDepends);
			SetSelectedTaskDependencies(aDepends);

			m_eDependency.EnableButton(ID_DEPENDS_LINK, !m_sDepends.IsEmpty());
		}
		break;
		
	case TDCA_PRIORITY:
		SetSelectedTaskPriority(m_nPriority);
		break;
		
	case TDCA_RISK:
		SetSelectedTaskRisk(m_nRisk);
		break;
		
	case TDCA_COLOR:
		SetSelectedTaskColor(m_crColour);
		break;
		
	case TDCA_EXTERNALID:
		SetSelectedTaskExtID(m_sExternalID);
		break;
		
	case TDCA_ALLOCTO:
		SetSelectedTaskAllocTo(m_aAllocTo);
		break;
		
	case TDCA_ALLOCBY:
		SetSelectedTaskAllocBy(m_sAllocBy);
		break;
		
	case TDCA_STATUS:
		SetSelectedTaskStatus(m_sStatus);
		break;
		
	case TDCA_VERSION:
		SetSelectedTaskVersion(m_sVersion);
		break;
		
	case TDCA_CATEGORY:
		SetSelectedTaskCategories(m_aCategory);
		break;
		
	case TDCA_TAGS:
		SetSelectedTaskTags(m_aTags);
		break;
		
	case TDCA_PERCENT:
		{
			// note: we need to take account of 'done' state too because
			// we maintain the task percent at its pre-done state even
			// if the UI says its '100%'
			BOOL bWasDone = IsSelectedTaskDone();
			SetSelectedTaskPercentDone(m_nPercentDone);
			
			// check if we need to update 'done' state
			BOOL bDoneChange = (bWasDone && m_nPercentDone < 100) || (!bWasDone && m_nPercentDone == 100);
			
			if (bDoneChange)
				SetSelectedTaskDone(m_nPercentDone == 100);
		}
		break;
		
	case TDCA_TIMEEST:
		if (dwFlags & UTF_TIMEUNITSONLY)
			SetSelectedTaskTimeEstimateUnits(m_nTimeEstUnits, Misc::HasFlag(dwFlags, UTF_RECALCTIME));
		else
			SetSelectedTaskTimeEstimate(m_dTimeEstimate, m_nTimeEstUnits);
		break;
		
	case TDCA_TIMESPENT:
		if (dwFlags & UTF_TIMEUNITSONLY)
			SetSelectedTaskTimeSpentUnits(m_nTimeSpentUnits, Misc::HasFlag(dwFlags, UTF_RECALCTIME));
		else
			SetSelectedTaskTimeSpent(m_dTimeSpent, m_nTimeSpentUnits);
		break;
		
	case TDCA_FILEREF:
		SetSelectedTaskFileRefs(m_aFileRefs);
		break;
		
	default:
		// handle custom attributes
		if (CTDCCustomAttributeHelper::IsCustomAttribute(nAttrib))
		{
			CString sData, sAttribID = CTDCCustomAttributeHelper::GetAttributeTypeID(nAttrib, m_aCustomAttribDefs);

			if (m_mapCustomCtrlData.Lookup(sAttribID, sData))
				SetSelectedTaskCustomAttributeData(sAttribID, sData, TRUE);
		}
	}
}

void CToDoCtrl::OnChangePriority()
{
	UpdateTask(TDCA_PRIORITY);
}

void CToDoCtrl::OnChangeRisk()
{
	UpdateTask(TDCA_RISK);
}

void CToDoCtrl::OnStartDatechange(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	// ignore this if the date selector is dropped down
	if (!m_dtcStart.IsCalendarVisible())
		UpdateTask(TDCA_STARTDATE); 
	
	*pResult = 0;
}

void CToDoCtrl::OnDueDatechange(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	// ignore this if the date selector is dropped down
	if (!m_dtcDue.IsCalendarVisible())
		UpdateTask(TDCA_DUEDATE); 	
	
	*pResult = 0;
}

void CToDoCtrl::OnCompletionDatechange(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	// ignore this if the date selector is dropped down
	if (!m_dtcDone.IsCalendarVisible())
		UpdateTask(TDCA_DONEDATE);	
	
	*pResult = 0;
}

void CToDoCtrl::OnSelChangeDueTime()
{
	UpdateTask(TDCA_DUETIME);
}

void CToDoCtrl::OnSelChangeStartTime()
{
	UpdateTask(TDCA_STARTTIME);
}

void CToDoCtrl::OnSelChangeDoneTime()
{
	UpdateTask(TDCA_DONETIME);
}

void CToDoCtrl::OnCustomAttributeChange(UINT nCtrlID, NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	OnCustomAttributeChange(nCtrlID);
	*pResult = 0;
}

void CToDoCtrl::OnCustomAttributeChange(UINT nCtrlID)
{
	ASSERT(CTDCCustomAttributeHelper::IsCustomControl(nCtrlID));

	CUSTOMATTRIBCTRLITEM ctrl;

	if (CTDCCustomAttributeHelper::GetControl(nCtrlID, m_aCustomControls, ctrl))
	{
		ASSERT(CTDCCustomAttributeHelper::IsCustomAttribute(ctrl.nAttrib));

		UpdateTask(ctrl.nAttrib);
	}
}

// external version
BOOL CToDoCtrl::SetSelectedTaskCustomAttributeData(const CString& sAttribID, const CString& sData)
{
	return SetSelectedTaskCustomAttributeData(sAttribID, sData, FALSE);
}

BOOL CToDoCtrl::SetSelectedTaskCustomAttributeData(const CString& sAttribID, const CString& sData, BOOL bCtrlEdited)
{
	if (IsReadOnly())
		return FALSE;

	if (sAttribID.IsEmpty())
		return FALSE;

	Flush();
	
	POSITION pos = TSH().GetFirstItemPos();
	int nRes = SET_NOCHANGE;
	DWORD dwModTaskID = 0;
	
	IMPLEMENT_UNDOEDIT();
		
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		int nItemRes = m_data.SetTaskCustomAttributeData(dwTaskID, sAttribID, sData);
			
		if (nItemRes == SET_CHANGE)
		{
			nRes = SET_CHANGE;
			dwModTaskID = dwTaskID;
		}
	}
	
	if (nRes == SET_CHANGE)
	{
 		SetModified(TRUE, TDCA_CUSTOMATTRIB, dwModTaskID);

		// update UI except if it's already up to date
		CUSTOMATTRIBCTRLITEM ctrl;
		
		if (CTDCCustomAttributeHelper::GetControl(sAttribID, m_aCustomControls, ctrl))
		{
			if (!bCtrlEdited)
				CTDCCustomAttributeHelper::UpdateCustomAttributeControl(this, ctrl, m_aCustomAttribDefs, sData);

			if (ctrl.HasBuddy())
				EnableDisableControls(GetSelectedItem());
		}
	}
	
	return (nRes != SET_FAILED);
}

void CToDoCtrl::DrawSplitter(CDC* pDC) 
{
	// draw splitter and clip out
	if ((m_nMaxState == TDCMS_NORMAL) &&
		m_ctrlComments.GetSafeHwnd() && 
		m_theme.IsSet() && 
		IsCommentsVisible())
	{
		CRect rSplitter = GetSplitterRect();
		GraphicsMisc::DrawSplitBar(pDC, rSplitter, m_theme.crAppBackDark, FALSE);

		pDC->ExcludeClipRect(rSplitter);
	}
}

BOOL CToDoCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// if the task tree has no size, we can treat this as spurious
	if (m_nMaxState == TDCMS_NORMAL)
	{
		CRect rTree;
		m_taskTree.GetWindowRect(rTree);
		
		if ((rTree.Width() == 0) && (rTree.Height() == 0))
			return TRUE;
	}

	// clip out all the child controls to reduce flicker
	if (!(GetStyle() & WS_CLIPCHILDREN) && m_taskTree.GetSafeHwnd())
	{
		ExcludeCtrls(pDC, IDC_FIRST + 1, IDC_LAST - 1, TRUE);
		ExcludeChild(&m_taskTree, pDC);
	}
	
	// fill background with theme brush
	int nDC = pDC->SaveDC();
	
	if (m_brUIBack.GetSafeHandle())
	{
		CRect rect;
		pDC->GetClipBox(rect);
		pDC->FillSolidRect(rect, m_theme.crAppBackLight);
	}
	else // default
	{
		CRuntimeDlg::OnEraseBkgnd(pDC);
	}

	// draw comments splitter
	DrawSplitter(pDC);
	
	pDC->RestoreDC(nDC);
	
	return TRUE;
}

int CToDoCtrl::GetAutoListData(TDCAUTOLISTDATA& tld) const
{
	m_cbAllocTo.GetItems(tld.aAllocTo);
	m_cbAllocBy.GetItems(tld.aAllocBy);
	m_cbCategory.GetItems(tld.aCategory);
	m_cbStatus.GetItems(tld.aStatus);
	m_cbTags.GetItems(tld.aTags);
	m_cbVersion.GetItems(tld.aVersion);

	return tld.GetSize();
}

void CToDoCtrl::SetDefaultAutoListData(const TDCAUTOLISTDATA& tld)
{
	// update the combos before copying over the current defaults

	// multi-selection
	SetDefaultListContent(m_cbAllocTo,	tld.aAllocTo,	m_tldDefault.aAllocTo);
	SetDefaultListContent(m_cbCategory,	tld.aCategory,	m_tldDefault.aCategory);
	SetDefaultListContent(m_cbTags,		tld.aTags,		m_tldDefault.aTags);

	// single selection
	SetDefaultListContent(m_cbStatus,	tld.aStatus,	m_tldDefault.aStatus, TRUE);
	SetDefaultListContent(m_cbVersion,	tld.aVersion,	m_tldDefault.aVersion, TRUE);
	SetDefaultListContent(m_cbAllocBy,	tld.aAllocBy,	m_tldDefault.aAllocBy, TRUE);

	// save
	m_tldDefault.Copy(tld);

	// restore selection
	OnSelCancelAllocTo();
	OnSelCancelCategory();
	OnSelCancelTag();

	m_cbAllocBy.SelectString(0, m_sAllocBy);
	m_cbStatus.SelectString(0, m_sStatus);
	m_cbVersion.SelectString(0, m_sVersion);
}

void CToDoCtrl::SetDefaultListContent(CAutoComboBox& combo, const CStringArray& aNewDefs, 
									 const CStringArray& aOldDefs, BOOL bAddEmpty)
{
	CHoldRedraw hr(combo);
	
	// if the combo contents are not writable then combo ought to
	// only contain the old defaults so we just replace with the new
	BOOL bIsReadOnly = !CDialogHelper::ComboHasEdit(combo);

	if (bIsReadOnly)
	{
		CDialogHelper::SetComboBoxItems(combo, aNewDefs);
	}
	else
	{
		CStringArray aComboItems;
		CDialogHelper::GetComboBoxItems(combo, aComboItems);

		// remove the old defaults
		Misc::RemoveEmptyItems(aComboItems);
		Misc::RemoveItems(aOldDefs, aComboItems);

		// add new defaults
		Misc::AddUniqueItems(aNewDefs, aComboItems);

		// update combo
		CDialogHelper::SetComboBoxItems(combo, aComboItems);
	}

	// add empty as required
	if (bAddEmpty)
		combo.AddEmptyString();

	// and recalc drop-width
	combo.RefreshDropWidth();
}

BOOL CToDoCtrl::SetAutoListContentReadOnly(TDC_ATTRIBUTE nListAttribID, BOOL bReadOnly)
{
	switch (nListAttribID)
	{
	// multi-selection
	case TDCA_CATEGORY: return SetComboReadOnly(m_cbCategory,	bReadOnly, m_tldDefault.aCategory, FALSE);
	case TDCA_ALLOCTO:	return SetComboReadOnly(m_cbAllocTo,	bReadOnly, m_tldDefault.aAllocTo, FALSE);
	case TDCA_TAGS:		return SetComboReadOnly(m_cbTags,		bReadOnly, m_tldDefault.aTags, FALSE);

	// single selection
	case TDCA_ALLOCBY:	return SetComboReadOnly(m_cbAllocBy,	bReadOnly, m_tldDefault.aAllocBy, TRUE);
	case TDCA_VERSION:	return SetComboReadOnly(m_cbVersion,	bReadOnly, m_tldDefault.aVersion, TRUE);
	case TDCA_STATUS:	return SetComboReadOnly(m_cbStatus,		bReadOnly, m_tldDefault.aStatus, TRUE);
	}

	// all else
	return FALSE;
}

BOOL CToDoCtrl::SetComboReadOnly(CAutoComboBox& combo, BOOL bReadOnly, const CStringArray& aDefContent, BOOL bAddEmpty)
{
	BOOL bWasReadOnly = !CDialogHelper::ComboHasEdit(combo);

	CStringArray aContent;
	CDialogHelper::GetComboBoxItems(combo, aContent);

	if (!CDialogHelper::SetComboReadOnly(combo, TRUE, bReadOnly, COMBODROPHEIGHT))
		return FALSE;

	if ((bReadOnly && !bWasReadOnly) || (!bReadOnly && bWasReadOnly))
	{
		CHoldRedraw hr(combo);

		// if switching TO readonly restore to default items
		if (bReadOnly)
			combo.AddUniqueItems(aDefContent);
		else
			combo.AddUniqueItems(aContent);
	
		// and recalc drop-width
		combo.RefreshDropWidth();

		if (bAddEmpty)
			combo.AddEmptyString();

		m_mgrPrompts.SetComboPrompt(combo, IDS_TDC_EDITPROMPT_NAME);

		// restore selected task items
		UpdateData(FALSE);
	}

	return TRUE;
}

void CToDoCtrl::NewList()
{
	Flush();

	BOOL bConfirmDelete = HasStyle(TDCS_CONFIRMDELETE);
	
	if (bConfirmDelete)
		SetStyle(TDCS_CONFIRMDELETE, FALSE);
	
	DeleteAllTasks();
	
	if (bConfirmDelete)
		SetStyle(TDCS_CONFIRMDELETE, TRUE);
	
	m_sProjectName.Empty();
	m_nFileVersion = 0;
	m_bModified = FALSE;
	m_sPassword.Empty();
	
	UpdateData(FALSE);
	UpdateComments(FALSE);
}

BOOL CToDoCtrl::SetSelectedTaskColor(COLORREF color)
{
	if (IsReadOnly())
		return FALSE;

	Flush();
	
	POSITION pos = TSH().GetFirstItemPos();
	int nRes = SET_NOCHANGE;
	DWORD dwModTaskID = 0;
	
	IMPLEMENT_UNDOEDIT();
		
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		int nItemRes = m_data.SetTaskColor(dwTaskID, color);
		
		if (nItemRes == SET_CHANGE)
		{
			nRes = SET_CHANGE;
			dwModTaskID = dwTaskID;
		}
	}
	
	if (nRes == SET_CHANGE)
	{
		if (color == CLR_NONE)
		{
			m_cpColour.SetBkColour(CLR_DEFAULT);
			m_cpColour.SetTextColour(CLR_DEFAULT);
		}
		else
		{
			m_cpColour.SetColour(color);
		}

		SetModified(TRUE, TDCA_COLOR, dwModTaskID);
	}
	
	return (nRes != SET_FAILED);
}

BOOL CToDoCtrl::EditSelectedTaskIcon()
{
	if (IsReadOnly())
		return FALSE;

	CTDLTaskIconDlg dialog(m_ilTaskIcons, GetSelectedTaskIcon());

	if (dialog.DoModal() != IDOK)
		return FALSE;

	return SetSelectedTaskIcon(dialog.GetIconName());
}

BOOL CToDoCtrl::ClearSelectedTaskIcon()
{
	if (IsReadOnly())
		return FALSE;

	return SetSelectedTaskIcon(_T(""));
}

BOOL CToDoCtrl::SetSelectedTaskIcon(const CString& sIcon)
{
	Flush();
	
	POSITION pos = TSH().GetFirstItemPos();
	int nRes = SET_NOCHANGE;
	DWORD dwModTaskID = 0;
	
	IMPLEMENT_UNDOEDIT();
		
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		int nItemRes = m_data.SetTaskIcon(dwTaskID, sIcon);
		
		if (nItemRes == SET_CHANGE)
		{
			nRes = SET_CHANGE;
			dwModTaskID = dwTaskID;
		}
	}
	
	if (nRes == SET_CHANGE)
		SetModified(TRUE, TDCA_ICON, dwModTaskID);
	
	return (nRes != SET_FAILED);
}

BOOL CToDoCtrl::CanPasteText()
{
	return (!IsReadOnly() && CWinClasses::IsEditControl(::GetFocus()));
}

BOOL CToDoCtrl::PasteText(const CString& sText)
{
	if (!CanPasteText() || sText.IsEmpty())
		return FALSE;

	HWND hFocus = ::GetFocus();
	::SendMessage(hFocus, EM_REPLACESEL, TRUE, (LPARAM)(LPCTSTR)sText);
	
	return TRUE;
}

BOOL CToDoCtrl::SetSelectedTaskComments(const CString& sComments, const CBinaryData& customComments)
{
	return SetSelectedTaskComments(sComments, customComments, FALSE); 
}

BOOL CToDoCtrl::SetSelectedTaskComments(const CString& sComments, const CBinaryData& customComments, BOOL bInternal)
{
	if (IsReadOnly())
		return FALSE;

	if (!bInternal)
		Flush();
	
	POSITION pos = TSH().GetFirstItemPos();
	int nRes = SET_NOCHANGE;
	DWORD dwModTaskID = 0;

	IMPLEMENT_UNDOEDIT();
		
	// do not change the custom comments unless the comments
	// type matches the current content control type
	BOOL bCanChangeCustom = (m_mgrContent.FindContent(m_cfComments) != -1);
	LPCTSTR szFormat = bCanChangeCustom ? m_cfComments : NULL;
	
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		int nItemRes = m_data.SetTaskComments(dwTaskID, sComments, customComments, szFormat);
		
		if (nItemRes == SET_CHANGE)
		{
			nRes = SET_CHANGE;
			dwModTaskID = dwTaskID;
		}
	}
	
	if (nRes == SET_CHANGE)
	{
		// refresh the comments of the active task if we were called externally
		// note: we don't use SetTextChange because that doesn't handle custom comments
		if (!bInternal)
		{
			// try custom comments if that's what they are
			if (!bCanChangeCustom || 
				!m_ctrlComments.SetContent(customComments, FALSE) || 
				customComments.IsEmpty())
			{
				// else text comments
				m_ctrlComments.SetTextContent(sComments, FALSE);
			}

			// update comments variables
			UpdateComments(TRUE);
		}

		TSH().InvalidateAll();

		SetModified(TRUE, TDCA_COMMENTS, dwModTaskID);
	}
	
	return (nRes != SET_FAILED);
}

BOOL CToDoCtrl::SetSelectedTaskTitle(const CString& sTitle)
{
	if (IsReadOnly() || GetSelectedCount() != 1)
		return FALSE;

	Flush();
	
	IMPLEMENT_UNDOEDIT();
		
	DWORD dwTaskID = GetSelectedTaskID();
	int nRes = m_data.SetTaskTitle(dwTaskID, sTitle);
	
	if (nRes == SET_CHANGE)
	{
		m_taskTree.Tree().SetItemText(GetSelectedItem(), sTitle);
		m_taskTree.InvalidateSelection();

		SetModified(TRUE, TDCA_TASKNAME, dwTaskID);
	}
	
	return (nRes != SET_FAILED);
}

BOOL CToDoCtrl::GetSelectionBoundingRect(CRect& rSelection) const
{
	if (m_taskTree.GetSelectionBoundingRect(rSelection))
	{
		m_taskTree.ClientToScreen(rSelection);
		ScreenToClient(rSelection);

		return TRUE;
	}

	return FALSE;
}

COleDateTime CToDoCtrl::GetTaskDate(DWORD dwTaskID, TDC_DATE nDate) const
{
	return m_data.GetTaskDate(dwTaskID, nDate);
}

BOOL CToDoCtrl::GetTaskTimes(DWORD dwTaskID, double& dTimeEst, TDC_UNITS& nEstUnits, double& dTimeSpent, TDC_UNITS& nSpentUnits) const
{
	if (!m_data.HasTask(dwTaskID))
		return FALSE;

	dTimeEst = m_data.GetTaskTimeEstimate(dwTaskID, nEstUnits);
	dTimeSpent = m_data.GetTaskTimeSpent(dwTaskID, nSpentUnits);

	return TRUE;
}

BOOL CToDoCtrl::SetSelectedTaskPriority(int nPriority)
{
	if (IsReadOnly())
		return FALSE;

	Flush();

	POSITION pos = TSH().GetFirstItemPos();
	int nRes = SET_FAILED;
	DWORD dwModTaskID = 0;

	IMPLEMENT_UNDOEDIT();
		
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		int nItemRes = m_data.SetTaskPriority(dwTaskID, nPriority);
		
		if (nItemRes == SET_CHANGE)
		{
			nRes = SET_CHANGE;
			dwModTaskID = dwTaskID;
		}
	}
	
	if (nRes == SET_CHANGE)
	{
		if (m_nPriority != nPriority)
		{
			m_nPriority = nPriority;
			m_cbPriority.SetSelectedPriority(m_nPriority);
		}
		
		SetModified(TRUE, TDCA_PRIORITY, dwModTaskID);
	}

	return (nRes != SET_FAILED);
}

BOOL CToDoCtrl::SetSelectedTaskRisk(int nRisk)
{
	if (IsReadOnly())
		return FALSE;

	Flush();
	
	POSITION pos = TSH().GetFirstItemPos();
	int nRes = SET_FAILED;
	DWORD dwModTaskID = 0;
	
	IMPLEMENT_UNDOEDIT();
		
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		int nItemRes = m_data.SetTaskRisk(dwTaskID, nRisk);
		
		if (nItemRes == SET_CHANGE)
		{
			nRes = SET_CHANGE;
			dwModTaskID = dwTaskID;
		}
	}
	
	if (nRes == SET_CHANGE)
	{
		if (m_nRisk != nRisk)
		{
			m_nRisk = nRisk;
			m_cbRisk.SetSelectedRisk(m_nRisk);
		}
		
		SetModified(TRUE, TDCA_RISK, dwModTaskID);
	}
	
	return (nRes != SET_FAILED);
}

BOOL CToDoCtrl::SetSelectedTaskFlag(BOOL bFlagged)
{
	if (IsReadOnly())
		return FALSE;

	Flush();
	
	POSITION pos = TSH().GetFirstItemPos();
	int nRes = SET_FAILED;
	DWORD dwModTaskID = 0;
	
	IMPLEMENT_UNDOEDIT();
		
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		int nItemRes = m_data.SetTaskFlag(dwTaskID, bFlagged);
		
		if (nItemRes == SET_CHANGE)
		{
			nRes = SET_CHANGE;
			dwModTaskID = dwTaskID;
		}
	}
	
	if (nRes == SET_CHANGE)
	{
		SetModified(TRUE, TDCA_FLAG, dwModTaskID);
	
		if (IsColumnShowing(TDCC_FLAG))
		{
			if (TSH().GetCount() > 1)
				m_taskTree.RedrawColumns();
			else
				m_taskTree.InvalidateTask(dwModTaskID);
		}
	}
	
	return (nRes != SET_FAILED);
}

BOOL CToDoCtrl::IncrementSelectedTaskPriority(BOOL bUp)
{
	if (IsReadOnly())
		return FALSE;

	Flush();
	
	POSITION pos = TSH().GetFirstItemPos();
	int nRes = SET_FAILED;
	DWORD dwModTaskID = 0;
	int nAmount = (bUp ? 1 : -1);

	IMPLEMENT_UNDOEDIT();
		
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		int nPriority = m_data.GetTaskPriority(dwTaskID) + nAmount;

		// need to jump over -1
		if (nPriority < 0)
		{
			if (nAmount < 0)
				nPriority = FM_NOPRIORITY;
			else
				nPriority = 0;
		}

		int nItemRes = m_data.SetTaskPriority(dwTaskID, nPriority);
		
		if (nItemRes == SET_CHANGE)
		{
			nRes = SET_CHANGE;
			dwModTaskID = dwTaskID;
		}
	}
	
	if (nRes == SET_CHANGE)
	{
		m_nPriority = m_cbPriority.IncrementPriority(nAmount);
		SetModified(TRUE, TDCA_PRIORITY, dwModTaskID);
	}
	
	return (nRes != SET_FAILED);
}

void CToDoCtrl::ShowTaskHasIncompleteDependenciesError(const CString& sIncomplete)
{
	if (GetSelectedCount() == 1)
	{
		int nRet = AfxMessageBox(CEnString(IDS_TDC_SELTASKHASDEPENDENCY), MB_YESNO | MB_ICONEXCLAMATION);
		
		if (nRet == IDYES)
			ShowTaskLink(sIncomplete);
	}
	else
	{
		AfxMessageBox(CEnString(IDS_TDC_SELTASKSHAVEDEPENDENCIES), MB_OK | MB_ICONEXCLAMATION);
	}
}

void CToDoCtrl::ShowTaskHasCircularDependenciesError(const CDWordArray& aTaskIDs) const
{
	if (GetSelectedCount() == 1)
	{
		AfxMessageBox(CEnString(IDS_TDC_SELTASKHASCIRCULARDEPENDENCY), MB_OK | MB_ICONEXCLAMATION);
	}
	else
	{
		CEnString sMessage(IDS_TDC_SELTASKSHAVECIRCULARDEPENDENCIES, Misc::FormatArray(aTaskIDs));
		AfxMessageBox(sMessage, MB_OK | MB_ICONEXCLAMATION);
	}
}

// external version
BOOL CToDoCtrl::SetSelectedTaskDate(TDC_DATE nDate, const COleDateTime& date)
{
	return SetSelectedTaskDate(nDate, date, FALSE);
}

// internal version
BOOL CToDoCtrl::SetSelectedTaskDate(TDC_DATE nDate, const COleDateTime& date, BOOL bDateEdited)
{
	// if this is a start/due edit then it must be a component 
	ASSERT (!bDateEdited || ((nDate != TDCD_DUE) && (nDate != TDCD_START)));

	if (bDateEdited && ((nDate == TDCD_DUE) || (nDate == TDCD_START)))
		return FALSE;

	// special case
	if (nDate == TDCD_DONE)
		return SetSelectedTaskDone(date, bDateEdited);

	if (IsReadOnly())
		return FALSE;

	POSITION pos = TSH().GetFirstItemPos();
	TDC_SET nRes = SET_NOCHANGE;
	DWORD dwModTaskID = 0;

	IMPLEMENT_UNDOEDIT();
		
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);

		// due, start, creation
		if (m_data.SetTaskDate(dwTaskID, nDate, date) == SET_CHANGE)
		{
			nRes = SET_CHANGE;
			dwModTaskID = dwTaskID;
		}
	}
	
	if (nRes == SET_CHANGE)
	{
		BOOL bUpdateTimeEst = FALSE;

		switch (nDate)
		{
		case TDCD_CREATE:	
			SetModified(TRUE, TDCA_CREATIONDATE, dwModTaskID); 
			break;

		case TDCD_STARTDATE:
			// update due date ctrl to be start date if 
			// no due date has been specified
			if (bDateEdited && CDateHelper::IsDateSet(date))
			{
				COleDateTime dtDue = GetSelectedTaskDate(TDCD_DUE);

				if (!CDateHelper::IsDateSet(dtDue))
				{
					SetCtrlDate(m_dtcDue, 0.0, date);
				}
			}
			// fall thru

		case TDCD_START:
		case TDCD_STARTTIME:
			bUpdateTimeEst = HasStyle(TDCS_SYNCTIMEESTIMATESANDDATES);

			SetModified(TRUE, TDCA_STARTDATE, dwModTaskID); 
			break;
			
		case TDCD_DUEDATE:	
			// update due date ctrl to be start date if 
			// no due date has been specified
			if (bDateEdited && !CDateHelper::IsDateSet(date))
			{
				COleDateTime dtStart = GetSelectedTaskDate(TDCD_START);

				SetCtrlDate(m_dtcDue, 0.0, dtStart);
			}
			m_eRecurrence.SetDefaultDate(date);
			// fall thru

		case TDCD_DUE:
		case TDCD_DUETIME:
			bUpdateTimeEst = HasStyle(TDCS_SYNCTIMEESTIMATESANDDATES);
			
			SetModified(TRUE, TDCA_DUEDATE, dwModTaskID); 
			break;
				
		case TDCD_DONETIME:	
			SetModified(TRUE, TDCA_DONEDATE, dwModTaskID); 
			break;

		//case TDCD_DONE:
		default:
			ASSERT(0);
			return FALSE;
		}

		// only update controls if the date was changed implicitly
		if (!bDateEdited)
		{
			UpdateControls(FALSE); // don't update comments
		}
		else if (bUpdateTimeEst && (GetSelectedCount() == 1))
		{
			TDC_UNITS nUnits;
			double dTimeEst = GetSelectedTaskTimeEstimate(nUnits);

			ASSERT(nUnits == TDC::MapTHUnitsToUnits(m_eTimeEstimate.GetUnits()));
			
			UpdateDataEx(this, IDC_TIMEEST, dTimeEst, FALSE, DECIMALS);
		}
	}
	
	return (nRes != SET_FAILED);
}

BOOL CToDoCtrl::MoveSelectedTaskDates(const COleDateTime& dtNewStart, BOOL bFailOnNoChange)
{
	if (GetSelectedCount() != 1)
	{
		ASSERT(0);
		return FALSE;
	}

	if (IsReadOnly())
		return FALSE;
	
	IMPLEMENT_UNDOEDIT();
	
	DWORD dwTaskID = TSH().GetFirstItemData();
	TDC_SET nRes = m_data.MoveTaskDates(dwTaskID, dtNewStart);
		
	// post-processing
	if (nRes == SET_CHANGE)
	{
		COleDateTime dtDue = GetSelectedTaskDate(TDCD_DUE);

		if (CDateHelper::IsDateSet(dtDue))
			m_eRecurrence.SetDefaultDate(dtDue);

		SetModified(TRUE, TDCA_STARTDATE, dwTaskID); 
		SetModified(TRUE, TDCA_DUEDATE, dwTaskID); 

		UpdateControls(FALSE); // don't update comments
	}
	
	if (bFailOnNoChange && (nRes == SET_NOCHANGE))
		return FALSE;

	// else
	return (nRes != SET_FAILED);
}

BOOL CToDoCtrl::OffsetSelectedTaskDate(TDC_DATE nDate, int nAmount, TDC_OFFSET nOffset, BOOL bAndSubtasks)
{
	if (IsReadOnly())
		return FALSE;

	Flush();

	// remove duplicate subtasks if we're going to be 
	// processing subtasks anyway
	CHTIList htiSel;
	TSH().CopySelection(htiSel, bAndSubtasks);

	POSITION pos = htiSel.GetHeadPosition();
	int nRes = SET_NOCHANGE;
	DWORD dwModTaskID = 0;
	TDC_UNITS nUnits = TDC::MapDateOffsetToUnits(nOffset);

	
	IMPLEMENT_UNDOEDIT();
		
	while (pos)
	{
		HTREEITEM hti = htiSel.GetNext(pos);
		DWORD dwTaskID = m_taskTree.GetTaskID(hti);

		int nItemRes = m_data.OffsetTaskDate(dwTaskID, nDate, nAmount, nUnits, bAndSubtasks);
		
		if (nItemRes == SET_CHANGE)
		{
			if (!dwModTaskID)
				dwModTaskID = dwTaskID;

			nRes = SET_CHANGE;
		}
	}
	
	if (nRes == SET_CHANGE)
	{
		switch (nDate)
		{
		case TDCD_CREATE:	
			SetModified(TRUE, TDCA_CREATIONDATE, dwModTaskID); 
			break;

		case TDCD_START:	
			SetModified(TRUE, TDCA_STARTDATE, dwModTaskID); 
			break;

		case TDCD_DUE:		
			SetModified(TRUE, TDCA_DUEDATE, dwModTaskID); 
			break;

		case TDCD_DONE:		
			SetModified(TRUE, TDCA_DONEDATE, dwModTaskID); 
			break;

		default:
			ASSERT(0);
			return FALSE;
		}

		UpdateControls(FALSE); // don't update comments
	}

	return (nRes != SET_FAILED);
}

BOOL CToDoCtrl::OffsetSelectedTaskDates(int nAmount, TDC_OFFSET nOffset, BOOL bAndSubtasks)
{
	if (IsReadOnly())
		return FALSE;
	
	Flush();
	
	// remove duplicate subtasks if we're going to be 
	// processing subtasks anyway
	CHTIList htiSel;
	TSH().CopySelection(htiSel, bAndSubtasks);
	
	POSITION pos = htiSel.GetHeadPosition();
	int nRes = SET_NOCHANGE;
	DWORD dwModTaskID = 0;
	DH_UNITS nDHUnits = TDC::MapDateOffsetToDHUnits(nOffset);
	
	IMPLEMENT_UNDOEDIT();
	
	while (pos)
	{
		HTREEITEM hti = htiSel.GetNext(pos);
		DWORD dwTaskID = m_taskTree.GetTaskID(hti);

		COleDateTime dtStart = m_data.GetTaskDate(dwTaskID, TDCD_START);
		ASSERT(CDateHelper::IsDateSet(dtStart));

		CDateHelper::OffsetDate(dtStart, nAmount, nDHUnits);
		
		int nItemRes = m_data.MoveTaskDates(dwTaskID, dtStart);
		
		if (nItemRes == SET_CHANGE)
		{
			if (!dwModTaskID)
				dwModTaskID = dwTaskID;
			
			nRes = SET_CHANGE;
		}
	}
	
	if (nRes == SET_CHANGE)
	{
		SetModified(TRUE, TDCA_STARTDATE, dwModTaskID); 
		SetModified(TRUE, TDCA_DUEDATE, dwModTaskID); 
		
		UpdateControls(FALSE); // don't update comments
	}
	
	return (nRes != SET_FAILED);
}

void CToDoCtrl::SetInheritedParentAttributes(const CTDCAttributeMap& mapAttribs, BOOL bUpdateAttrib)
{
	CToDoCtrlData::SetInheritedParentAttributes(mapAttribs, bUpdateAttrib);
}

int CToDoCtrl::CheckWantSubtasksCompleted()
{
	if (m_taskTree.SelectionHasIncompleteSubtasks(TRUE))
	{
		// ask the user what to do
		UINT nIDMessage = (GetSelectedCount() == 1) ? 
							IDS_TDC_SELTASKHASINCOMPLETE : IDS_TDC_TASKHASINCOMPLETE;
		
		int nRet = AfxMessageBox(CEnString(nIDMessage), MB_YESNOCANCEL | MB_ICONQUESTION);

		if (nRet == IDYES)
		{
			return 1;
		}
		else if (nRet == IDCANCEL)
		{
			return -1;
		}
	}

	return 0; // no incomplete subtasks or IDNO
}

BOOL CToDoCtrl::SetSelectedTaskDone(BOOL bDone)
{
	COleDateTime date;

	if (bDone)
		date = COleDateTime::GetCurrentTime();

	return SetSelectedTaskDone(date, FALSE);
}

void CToDoCtrl::AdjustNewRecurringTasksDates(DWORD dwPrevTaskID, DWORD dwNewTaskID, 
											 const COleDateTime& dtNext, BOOL bDueDate)
{
	// we need to move both the due date and the start date forward
	// so we first cache the old dates
	COleDateTime dtStart = m_data.GetTaskDate(dwPrevTaskID, TDCD_START);
	COleDateTime dtDue = m_data.GetTaskDate(dwPrevTaskID, TDCD_DUE);

	BOOL bHasStart = CDateHelper::IsDateSet(dtStart);
	BOOL bHasDue = CDateHelper::IsDateSet(dtDue);

	BOOL bWantInheritStart = m_data.WantUpdateInheritedAttibute(TDCA_STARTDATE);
	BOOL bWantInheritDue = m_data.WantUpdateInheritedAttibute(TDCA_DUEDATE);
	
	if (bDueDate) // dtNext is the new due date
	{
		int nOffsetDays = (bHasDue ? ((int)dtNext - (int)dtDue) : 0);
		
		if (bWantInheritDue)
		{
			m_data.SetTaskDate(dwNewTaskID, TDCD_DUE, dtNext);
			m_data.ApplyLastChangeToSubtasks(dwNewTaskID, TDCA_DUEDATE);
		}
		else // bump dates by required amount
		{
			if (bHasDue)
			{
				// Before we offset, make sure all subtasks have valid due dates
				m_data.InitMissingTaskDate(dwNewTaskID, TDCD_DUEDATE, dtDue, TRUE);
				m_data.OffsetTaskDate(dwNewTaskID, TDCD_DUEDATE, nOffsetDays, TDCU_DAYS, TRUE);
			}
			else
			{
				m_data.InitMissingTaskDate(dwNewTaskID, TDCD_DUE, dtNext, TRUE);
			}
		}

		// adjust start dates similarly
		if (bHasStart)
		{
			if (bWantInheritStart)
			{
				// don't offset children
				m_data.OffsetTaskDate(dwNewTaskID, TDCD_STARTDATE, nOffsetDays, TDCU_DAYS, FALSE);
				m_data.ApplyLastChangeToSubtasks(dwNewTaskID, TDCA_STARTDATE);
			}
			else // offset children
			{
				// Before we offset, make sure all subtasks have valid start dates
				m_data.InitMissingTaskDate(dwNewTaskID, TDCD_STARTDATE, dtStart, TRUE);
				m_data.OffsetTaskDate(dwNewTaskID, TDCD_STARTDATE, nOffsetDays, TDCU_DAYS, TRUE);
			}
		}
	}
	else // dtNext is the new start date
	{
		int nOffsetDays = (bHasStart ? ((int)dtNext - (int)dtStart) : 0);

		if (bWantInheritStart)
		{
			m_data.SetTaskDate(dwNewTaskID, TDCD_START, dtNext);
			m_data.ApplyLastChangeToSubtasks(dwNewTaskID, TDCA_STARTDATE);
		}
		else // bump dates by required amount
		{
			if (bHasStart)
			{
				// Before we offset, make sure all subtasks have valid start dates
				m_data.InitMissingTaskDate(dwNewTaskID, TDCD_STARTDATE, dtStart, TRUE);
				m_data.OffsetTaskDate(dwNewTaskID, TDCD_STARTDATE, nOffsetDays, TDCU_DAYS, TRUE);
			}
			else
			{
				m_data.InitMissingTaskDate(dwNewTaskID, TDCD_START, dtNext, TRUE);
			}
		}

		// adjust due dates similarly
		if (bHasDue)
		{
			if (bWantInheritDue)
			{
				// don't update children
				m_data.OffsetTaskDate(dwNewTaskID, TDCD_DUEDATE, nOffsetDays, TDCU_DAYS, FALSE);
				m_data.ApplyLastChangeToSubtasks(dwNewTaskID, TDCA_DUEDATE);
			}
			else // bump
			{
				// Before we offset, make sure all subtasks have valid due dates
				m_data.InitMissingTaskDate(dwNewTaskID, TDCD_DUEDATE, dtDue, TRUE);
				m_data.OffsetTaskDate(dwNewTaskID, TDCD_DUEDATE, nOffsetDays, TDCU_DAYS, TRUE);
			}
		}
	}
}

BOOL CToDoCtrl::SetSelectedTaskDone(const COleDateTime& date, BOOL bDateEdited)
{
	ASSERT(m_aRecreateTaskIDs.GetSize() == 0);

	if (IsReadOnly())
		return FALSE;

	Flush();

	// Check for incomplete or circular dependencies
	// and if the user also wants to complete any incomplete subtasks
	BOOL bDone = CDateHelper::IsDateSet(date);
	BOOL bAndSubtasks = FALSE;

	if (bDone && TSH().HasUncheckedItems())
	{
		// check for circular dependencies
		CDWordArray aTaskIDs;
		
		if (m_taskTree.SelectionHasCircularDependencies(aTaskIDs))
		{
			// reset ctrl date
			SetCtrlDate(m_dtcDone, 0.0);
			
			// show error msg
			ShowTaskHasCircularDependenciesError(aTaskIDs);
			
			return FALSE;
		}

		// check for incomplete dependents
		CString sIncomplete;

		if (m_taskTree.SelectionHasIncompleteDependencies(sIncomplete))
		{
			ShowTaskHasIncompleteDependenciesError(sIncomplete);
			return FALSE;
		}

		int nRet = CheckWantSubtasksCompleted();

		if (nRet == -1)
			return FALSE; // cancel

		bAndSubtasks = (nRet != 0);
	}

	// copy the selection and remove child duplicates
	// if setting state to 'done'
	CHTIList selection;
	TSH().CopySelection(selection, bDone);

	int nRes = SET_NOCHANGE;
	DWORD dwModTaskID = 0;
	BOOL bSomeRecurred = FALSE, bSomeDone = FALSE;
	
	IMPLEMENT_UNDOEDIT();
		
	POSITION pos = selection.GetHeadPosition();

	while (pos)
	{
		HTREEITEM hti = selection.GetNext(pos);

		DWORD dwTaskID = GetTaskID(hti);
		BOOL bWasDone = m_data.IsTaskDone(dwTaskID);

		// Handle recurring tasks, but only if changing state to completed
		COleDateTime dtNext;
		BOOL bRecurring = FALSE, bReuseTask = FALSE, bDueDate = TRUE;
		COleDateTime dtDone(date);
		
		if (bDone && !bWasDone)
		{
			TDCRECURRENCE tr;
			bRecurring = (m_data.CanTaskRecur(dwTaskID) && m_data.GetTaskRecurrence(dwTaskID, tr));

			if (bRecurring)
			{
				switch (tr.nReuse)
				{
				case TDIRO_REUSE:
					bReuseTask = TRUE;
					break;

				case TDIRO_CREATE:
					bReuseTask = FALSE;
					break;

				case TDIRO_ASK:
					{
						CTDLReuseRecurringTaskDlg dialog;

						if (dialog.DoModal() == IDCANCEL)
							return FALSE;

						bReuseTask = dialog.GetWantReuseTask();
					}
					break;
				}
			}
			else
			{
				bSomeDone = TRUE;
			}
		}
		else
		{
			bSomeDone = TRUE;
		}

		// FALSE == Don't update the dates of any already-completed subtasks
		int nItemRes = SetTaskDone(hti, dtDone, bAndSubtasks, FALSE);

		// handle recreation of recurring task
		if (bRecurring)
		{
			if (bReuseTask)
			{
				bSomeRecurred = TRUE;

				VERIFY (m_data.GetTaskNextOccurrence(dwTaskID, dtNext, bDueDate));

				InitialiseNewRecurringTask(dwTaskID, dwTaskID, hti, dtNext, bDueDate);

				// notify parent
				GetParent()->SendMessage(WM_TDCN_RECREATERECURRINGTASK, dwTaskID, dwTaskID);
			}
			else // save off task ID for later recreation
			{
				bSomeDone = TRUE;

				m_aRecreateTaskIDs.Add(dwTaskID);
			}

			nRes = SET_CHANGE;
			dwModTaskID = dwTaskID;
		}
		else if (nItemRes == SET_CHANGE)
		{
			nRes = SET_CHANGE;
			dwModTaskID = dwTaskID;
		}
	}
	
	// update related UI elements
	if (nRes == SET_CHANGE)
	{
		// only update controls if the date was changed implicitly
		if (!bDateEdited)
			UpdateControls(FALSE); // don't update comments

		m_taskTree.Tree().UpdateWindow();

		// clear done date control if reusing
		if (!bDone || bSomeRecurred)
		{
			SetCtrlDate(m_dtcDone, 0.0);
			m_cbTimeDone.SetOleTime(0.0);
		}
		else if (bDateEdited)
		{
			m_cbTimeDone.SetOleTime(date);
		}

		// if some tasks have recurred then we also need to report 
		// that the start/due dates have changed and selectively
		// update their state
		if (bSomeRecurred)
		{
			COleDateTime dtStart(GetSelectedTaskDate(TDCD_START));
			COleDateTime dtDue(GetSelectedTaskDate(TDCD_DUE));

			SetCtrlDate(m_dtcStart, dtStart);
			SetCtrlDate(m_dtcDue, dtDue);

			m_cbTimeStart.SetOleTime(dtStart.m_dt);
			m_cbTimeDue.SetOleTime(dtDue.m_dt);

			SetModified(TRUE, TDCA_STARTDATE, dwModTaskID);
			SetModified(TRUE, TDCA_DUEDATE, dwModTaskID);
		}
		
		// If some of the tasks were recurring and need to be created
		// we do that after this operation ends
		if (m_aRecreateTaskIDs.GetSize())
		{
			ASSERT(bSomeDone);
			PostMessage(WM_TDC_RECREATERECURRINGTASK, 0, m_aRecreateTaskIDs.GetSize());
		}

		// if some tasks have recurred or some tasks were completed
		// then we also need to report that the done date has changed
		if (bSomeDone || bSomeRecurred)
			SetModified(TRUE, TDCA_DONEDATE, dwModTaskID);

		// if some were completed and the status also changed
		// we may also need to update the UI
		if (bSomeDone && !m_sCompletionStatus.IsEmpty())
		{
			m_cbStatus.AddUniqueItem(m_sCompletionStatus);
			UpdateDataEx(this, IDC_STATUS, m_cbStatus, FALSE);

			SetModified(TRUE, TDCA_STATUS, dwModTaskID);
		}
	}
	
	return (nRes != SET_FAILED);
}

LRESULT CToDoCtrl::OnRecreateRecurringTask(WPARAM /*wParam*/, LPARAM lParam)
{
	int nNumTasks = lParam;

	// sanity checks
	if (!nNumTasks || (nNumTasks != m_aRecreateTaskIDs.GetSize()))
	{
		ASSERT(0);
		m_aRecreateTaskIDs.RemoveAll();
		return 0L;
	}

	// Always extend the previous undo action which completed
	// the recurring task(s) we are about to recreate
	IMPLEMENT_UNDOEXT(TDCUAT_ADD, TRUE);

	CDWordArray aTaskIDs, aNewTaskIDs;

	for (int nTask = 0; nTask < nNumTasks; nTask++)
	{
		DWORD dwTaskID = m_aRecreateTaskIDs[nTask];

		const TODOITEM* pTDI = NULL;
		const TODOSTRUCTURE* pTDS = NULL;

		if (!m_data.GetTask(dwTaskID, pTDI, pTDS))
		{
			ASSERT(0);
			return 0L;
		}

		// next occurrence can fail if we've run out of occurrences
		COleDateTime dtNext;
		BOOL bDueDate = TRUE;

		if (!m_data.GetTaskNextOccurrence(dwTaskID, dtNext, bDueDate))
			continue;

		// take copy of existing task
		CTaskFile task;
		VERIFY(AddTaskToTaskFile(pTDI, pTDS, task, NULL, FALSE));
	
		// insert below existing
		DWORD dwNewTaskID = m_dwNextUniqueID;

		HTREEITEM hti = m_taskTree.GetItem(dwTaskID);
		ASSERT(hti);

		HTREEITEM htiParent = m_taskTree.GetParentItem(hti);
		HTREEITEM htiNew = AddTaskToTreeItem(task, task.GetFirstTask(), htiParent, hti, TDCR_YES);

		InitialiseNewRecurringTask(dwTaskID, dwNewTaskID, htiNew, dtNext, bDueDate);

		// Save off taskIDs for the end
		aTaskIDs.Add(dwTaskID);
		aNewTaskIDs.Add(dwNewTaskID);
	}

	// mark as changed
	if (aNewTaskIDs.GetSize())
	{
		SelectTasks(aNewTaskIDs);
		SetModified(TRUE, TDCA_NEWTASK, ((aNewTaskIDs.GetSize() == 1) ? aNewTaskIDs[0] : 0));

		// notify parent of all new tasks
		int nTask = aTaskIDs.GetSize();

		while (nTask--)
			GetParent()->SendMessage(WM_TDCN_RECREATERECURRINGTASK, aTaskIDs[nTask], aNewTaskIDs[nTask]);
	}

	// always
	m_aRecreateTaskIDs.RemoveAll();

	return 0L;
}

void CToDoCtrl::InitialiseNewRecurringTask(DWORD dwPrevTaskID, DWORD dwNewTaskID, 
											HTREEITEM htiNew, const COleDateTime& dtNext, BOOL bDueDate)
{
	ASSERT(htiNew);
	ASSERT(GetTaskID(htiNew) == dwNewTaskID);

	// reset new task(s) state to 'undone' including all children
	SetTaskDone(htiNew, 0.0, TRUE, TRUE);

	// we need to move both the due date and the start date forward
	AdjustNewRecurringTasksDates(dwPrevTaskID, dwNewTaskID, dtNext, bDueDate);

	// Clear certain attributes
	m_data.ClearTaskAttribute(dwNewTaskID, TDCA_TIMESPENT, TRUE); 
	m_data.ClearTaskAttribute(dwNewTaskID, TDCA_PERCENT, TRUE);

	// Set some defaults
	m_data.SetTaskStatus(dwNewTaskID, m_tdiDefault.sStatus);

	// Reset number of occurrences
	m_data.ResetRecurringSubtaskOcurrences(dwNewTaskID);

	// Special handling for recreated tasks
	if (dwNewTaskID != dwPrevTaskID)
	{
		// the task ID has effectively changed so fix up those
		// tasks that previously had a dependency
		m_data.FixupTaskLocalDependentsIDs(dwNewTaskID, dwPrevTaskID);

		// Clear the comments
		m_data.ClearTaskAttribute(dwNewTaskID, TDCA_COMMENTS, TRUE);
	}
}

int CToDoCtrl::SetTaskDone(HTREEITEM hti, const COleDateTime& date, 
						   BOOL bAndSubtasks, BOOL bUpdateAllSubtaskDates)
{
	DWORD dwTaskID = GetTaskID(hti);
	int nRes = 0;

	// If bUpdateAllSubtaskDates == FALSE, we only update a task's
	// completion if its completion state has changed else we leave
	// the date as it was
	BOOL bDone = CDateHelper::IsDateSet(date);
	BOOL bWasDone = m_data.IsTaskDone(dwTaskID);
	BOOL bDoneChange = ((bDone && !bWasDone) || (!bDone && bWasDone));

	if (bDoneChange || bUpdateAllSubtaskDates)
	{
		if (m_data.SetTaskDate(dwTaskID, TDCD_DONE, date) == SET_CHANGE)
		{
			nRes = SET_CHANGE;

			// update status?
			if (!m_sCompletionStatus.IsEmpty())
			{
				if (bDone)
				{
					m_data.SetTaskStatus(dwTaskID, m_sCompletionStatus);
				}
				else if (m_data.GetTaskStatus(dwTaskID) == m_sCompletionStatus)
				{
					m_data.SetTaskStatus(dwTaskID, _T(""));
				}
			}
		}
	}
		
	if (m_dwTimeTrackTaskID == dwTaskID)
	{
		EndTimeTracking(TRUE, TRUE);
		UpdateControls(FALSE); // don't update comments
	}

	if (bAndSubtasks)
	{
		HTREEITEM htiChild = m_taskTree.GetChildItem(hti);
		
		while (htiChild)
		{
			if (SetTaskDone(htiChild, date, TRUE, bUpdateAllSubtaskDates) == SET_CHANGE)
				nRes = SET_CHANGE;
			
			htiChild = m_taskTree.GetNextItem(htiChild, TVGN_NEXT);
		}
	}

	return nRes;
}

BOOL CToDoCtrl::SetSelectedTaskPercentDone(int nPercent)
{
	if (IsReadOnly())
		return FALSE;

	Flush();
	
	POSITION pos = TSH().GetFirstItemPos();
	int nRes = SET_NOCHANGE;
	DWORD dwModTaskID = 0;
	
	IMPLEMENT_UNDOEDIT();
		
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		int nItemRes = m_data.SetTaskPercent(dwTaskID, nPercent);
		
		if (nItemRes == SET_CHANGE)
		{
			nRes = SET_CHANGE;
			dwModTaskID = dwTaskID;
		}
	}
	
	if (nRes == SET_CHANGE)
	{
		if (m_nPercentDone != nPercent)
		{
			m_nPercentDone = nPercent;
			UpdateDataEx(this, IDC_PERCENT, m_nPercentDone, FALSE);
		}
		
		SetModified(TRUE, TDCA_PERCENT, dwModTaskID);
	}
	
	return (nRes != SET_FAILED);
}

BOOL CToDoCtrl::SetSelectedTaskCost(double dCost)
{
	if (IsReadOnly())
		return FALSE;

	Flush();
	
	POSITION pos = TSH().GetFirstItemPos();
	int nRes = SET_NOCHANGE;
	DWORD dwModTaskID = 0;
	
	IMPLEMENT_UNDOEDIT();
		
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		int nItemRes = m_data.SetTaskCost(dwTaskID, dCost);
			
		if (nItemRes == SET_CHANGE)
		{
			nRes = SET_CHANGE;
			dwModTaskID = dwTaskID;
		}
	}
	
	if (nRes == SET_CHANGE)
	{
		if (m_dCost != dCost)
		{
			m_dCost = dCost;
			UpdateDataEx(this, IDC_COST, m_dCost, FALSE, DECIMALS);
		}
		
		SetModified(TRUE, TDCA_COST, dwModTaskID);
	}
	
	return (nRes != SET_FAILED);
}

BOOL CToDoCtrl::SetSelectedTaskRecurrence(const TDCRECURRENCE& tr)
{
	if (IsReadOnly())
		return FALSE;

	Flush();
	
	POSITION pos = TSH().GetFirstItemPos();
	int nRes = SET_NOCHANGE;
	DWORD dwModTaskID = 0;
	
	IMPLEMENT_UNDOEDIT();
	
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		int nItemRes = m_data.SetTaskRecurrence(dwTaskID, tr);
			
		if (nItemRes == SET_CHANGE)
		{
			nRes = SET_CHANGE;
			dwModTaskID = dwTaskID;
		}
	}
	
	if (nRes == SET_CHANGE)
	{
		if (m_tRecurrence != tr)
		{
			m_tRecurrence = tr;
			m_eRecurrence.SetRecurrenceOptions(tr);
		}

		// cache the last top-level options
		if (tr.IsRecurring())
		{
			m_nDefRecurFrom = tr.nRecalcFrom;
			m_nDefRecurReuse = tr.nReuse;
		}
		
		SetModified(TRUE, TDCA_RECURRENCE, dwModTaskID);
	}
	
	return (nRes != SET_FAILED);
}

void CToDoCtrl::SetPercentDoneIncrement(int nAmount)
{
	m_nPercentIncrement = abs(nAmount);
	m_nPercentIncrement = max(1, min(50, m_nPercentIncrement));

	if (m_spinPercent.GetSafeHwnd())
	{
		UDACCEL uda = { 0, m_nPercentIncrement };
		m_spinPercent.SetAccel(1, &uda);
	}
}

int CToDoCtrl::GetNextPercentDone(int nPercent, BOOL bUp)
{
	int nIncrement = (bUp ? m_nPercentIncrement : -m_nPercentIncrement);

	// we need to replicate the arithmetic performed by the 
	// spin button control, so that to the user the result
	// is the same as clicking the spin buttons
	if (m_nPercentIncrement > 1)
	{
		// bump the % to the next upper (if +ve) or
		// next lower (if -ve) whole increment
		// before adding the increment
		if (nPercent % m_nPercentIncrement)
		{
			if (bUp)
				nPercent = ((nPercent / m_nPercentIncrement) + 1) * m_nPercentIncrement;
			else
				nPercent = (nPercent / m_nPercentIncrement) * m_nPercentIncrement;
		}
	}

	return (nPercent + nIncrement);
}

BOOL CToDoCtrl::IncrementSelectedTaskPercentDone(BOOL bUp)
{
	if (IsReadOnly())
		return FALSE;

	Flush();
	
	POSITION pos = TSH().GetFirstItemPos();
	int nRes = SET_NOCHANGE;
	DWORD dwModTaskID = 0;
	
	IMPLEMENT_UNDOEDIT();
	
	while (pos)
	{
		HTREEITEM hti = TSH().GetNextItem(pos);
		DWORD dwTaskID = GetTaskID(hti);

		BOOL bDone = m_data.IsTaskDone(dwTaskID);

		int nPercent = m_data.GetTaskPercent(dwTaskID, TRUE);
		nPercent = GetNextPercentDone(nPercent, bUp);
				
		// need to handle transition to/from 100% as special case
		if (bDone && (nPercent < 100))
		{
			m_data.SetTaskDate(dwTaskID, TDCD_DONE, 0.0);
		}
		else if (!bDone && (nPercent >= 100))
		{
			m_data.SetTaskDate(dwTaskID, TDCD_DONE, COleDateTime::GetCurrentTime());
		}

		int nItemRes = m_data.SetTaskPercent(dwTaskID, nPercent);
		
		if (nItemRes == SET_CHANGE)
		{
			nRes = SET_CHANGE;
			dwModTaskID = dwTaskID;
		}
	}
	
	if (nRes == SET_CHANGE)
	{
		// don't update m_nPercentDone for multiple selection
		// else they all end up as the same value
		if (GetSelectedCount() == 1)
		{
			int nPercent = GetNextPercentDone(m_nPercentDone, bUp);

			if (m_nPercentDone != nPercent)
			{
				m_nPercentDone = nPercent;
				UpdateDataEx(this, IDC_PERCENT, m_nPercentDone, FALSE);
			}
		}
		
		SetModified(TRUE, TDCA_PERCENT, dwModTaskID);
	}
	
	return (nRes != SET_FAILED);
}

BOOL CToDoCtrl::SetSelectedTaskTimeEstimate(double dTime, TDC_UNITS nUnits)
{
	if (IsReadOnly())
		return FALSE;

	Flush();
	
	POSITION pos = TSH().GetFirstItemPos();
	TDC_SET nRes = SET_NOCHANGE;
	DWORD dwModTaskID = 0;
	
	IMPLEMENT_UNDOEDIT();
		
	while (pos)
	{
		HTREEITEM hti = TSH().GetNextItem(pos);

		// ignore parent tasks
		if (!m_taskTree.ItemHasChildren(hti) || HasStyle(TDCS_ALLOWPARENTTIMETRACKING))
		{
			DWORD dwTaskID = GetTaskID(hti);
			int nItemRes = m_data.SetTaskTimeEstimate(dwTaskID, dTime, nUnits);
			
			if (nItemRes == SET_CHANGE)
			{
				nRes = SET_CHANGE;
				dwModTaskID = dwTaskID;
			}
		}
	}
	
	if (nRes == SET_CHANGE)
	{
		if ((m_dTimeEstimate != dTime) || (m_nTimeEstUnits != nUnits))
		{
			// note: setting the time field changes m_nTimeEstUnits
			// so we have to do them separately
			m_nTimeEstUnits = nUnits;
			m_dTimeEstimate = dTime;

			m_eTimeEstimate.SetUnits(TDC::MapUnitsToTHUnits(m_nTimeEstUnits));
			UpdateDataEx(this, IDC_TIMEEST, m_dTimeEstimate, FALSE, DECIMALS);
		}
		
		// Recalc other attributes if only one item selected
		if (GetSelectedCount() == 1)
		{
			// update % complete?
			if (HasStyle(TDCS_AUTOCALCPERCENTDONE))
			{
				m_nPercentDone = m_data.CalcTaskPercentDone(GetSelectedTaskID());		
				UpdateDataEx(this, IDC_PERCENT, m_nPercentDone, FALSE);
			}
			
			// update start/due date?
			if (HasStyle(TDCS_SYNCTIMEESTIMATESANDDATES))
			{
				COleDateTime dtDue = GetSelectedTaskDate(TDCD_DUE);
				SetCtrlDate(m_dtcDue, dtDue, dtDue);

				COleDateTime dtStart = GetSelectedTaskDate(TDCD_START);
				SetCtrlDate(m_dtcStart, dtStart, dtStart);
			}
		}
		
		SetModified(TRUE, TDCA_TIMEEST, dwModTaskID);

		// may also need to report percent and/or date changes
		if (HasStyle(TDCS_AUTOCALCPERCENTDONE))
			SetModified(TRUE, TDCA_PERCENT, dwModTaskID);

		if (HasStyle(TDCS_SYNCTIMEESTIMATESANDDATES))
		{
			SetModified(TRUE, TDCA_STARTDATE, dwModTaskID);
			SetModified(TRUE, TDCA_DUEDATE, dwModTaskID);
		}
	}
	
	return (nRes != SET_FAILED);
}

BOOL CToDoCtrl::SetSelectedTaskTimeSpent(double dTime, TDC_UNITS nUnits)
{
	if (IsReadOnly())
		return FALSE;

	Flush();
	
	POSITION pos = TSH().GetFirstItemPos();
	int nRes = SET_NOCHANGE;
	DWORD dwModTaskID = 0;
	
	IMPLEMENT_UNDOEDIT();
		
	while (pos)
	{
		HTREEITEM hti = TSH().GetNextItem(pos);

		// ignore parent tasks
		if (!m_taskTree.ItemHasChildren(hti) || HasStyle(TDCS_ALLOWPARENTTIMETRACKING))
		{
			DWORD dwTaskID = GetTaskID(hti);
			int nItemRes = m_data.SetTaskTimeSpent(dwTaskID, dTime, nUnits);
			
			if (nItemRes == SET_CHANGE)
			{
				nRes = SET_CHANGE;
				dwModTaskID = dwTaskID;
			}
		}
	}
	
	if (nRes == SET_CHANGE)
	{
		if ((m_dTimeSpent != dTime) || (m_nTimeSpentUnits != nUnits))
		{
			// note: setting the time field changes m_nTimeSpentUnits
			// so we have to do them separately
			m_nTimeSpentUnits = nUnits;
			m_dTimeSpent = dTime;
			
			m_eTimeSpent.SetUnits(TDC::MapUnitsToTHUnits(m_nTimeSpentUnits));
			UpdateDataEx(this, IDC_TIMESPENT, m_dTimeSpent, FALSE, DECIMALS);
		}
		
		// update % complete?
		if (HasStyle(TDCS_AUTOCALCPERCENTDONE) && GetSelectedCount() == 1)
		{
			m_nPercentDone = m_data.CalcTaskPercentDone(GetSelectedTaskID());		
			UpdateDataEx(this, IDC_PERCENT, m_nPercentDone, FALSE);
		}
		
		SetModified(TRUE, TDCA_TIMESPENT, dwModTaskID);
	}
	
	return (nRes != SET_FAILED);
}

BOOL CToDoCtrl::SetSelectedTaskTimeEstimateUnits(TDC_UNITS nUnits, BOOL bRecalcTime)
{
	if (IsReadOnly())
		return FALSE;

	Flush();
	
	POSITION pos = TSH().GetFirstItemPos();
	int nRes = SET_NOCHANGE;
	DWORD dwModTaskID = 0;
	
	IMPLEMENT_UNDOEDIT();
		
	while (pos)
	{
		HTREEITEM hti = TSH().GetNextItem(pos);

		// ignore parent tasks
		if (!m_taskTree.ItemHasChildren(hti) || HasStyle(TDCS_ALLOWPARENTTIMETRACKING))
		{
			DWORD dwTaskID = GetTaskID(hti);

			TDC_UNITS nCurUnits;
			double dCurTime = m_data.GetTaskTimeEstimate(dwTaskID, nCurUnits);

			if (nCurUnits != nUnits)
			{
				double dTime = dCurTime;

				if (bRecalcTime)
					dTime = CTimeHelper().GetTime(dTime, TDC::MapUnitsToTHUnits(nCurUnits), TDC::MapUnitsToTHUnits(nUnits));

				int nItemRes = m_data.SetTaskTimeEstimate(dwTaskID, dTime, nUnits);
				
				if (nItemRes == SET_CHANGE)
				{
					nRes = SET_CHANGE;
					dwModTaskID = dwTaskID;
				}
			}
		}
	}
	
	if (nRes == SET_CHANGE)
	{
		if (m_nTimeEstUnits != nUnits)
		{
			m_nTimeEstUnits = nUnits;
			m_eTimeEstimate.SetUnits(TDC::MapUnitsToTHUnits(m_nTimeEstUnits));
		}

		// update other controls if only one item selected
		if (GetSelectedCount() == 1)
		{
			if (bRecalcTime)
			{
				m_dTimeEstimate = GetSelectedTaskTimeEstimate(nUnits);
				UpdateDataEx(this, IDC_TIMEEST, m_dTimeEstimate, FALSE, DECIMALS);
			}
			// update % complete?
			else if (HasStyle(TDCS_AUTOCALCPERCENTDONE))
			{
				m_nPercentDone = m_data.CalcTaskPercentDone(GetSelectedTaskID());		
				UpdateDataEx(this, IDC_PERCENT, m_nPercentDone, FALSE);
			}

			// update due date?
			if (HasStyle(TDCS_SYNCTIMEESTIMATESANDDATES))
			{
				COleDateTime dtDue = GetSelectedTaskDate(TDCD_DUE);
				SetCtrlDate(m_dtcDue, dtDue, dtDue);
			}
		}
		
		SetModified(TRUE, TDCA_TIMEEST, dwModTaskID);
		
		// may also need to report percent and/or date changes
		if (HasStyle(TDCS_AUTOCALCPERCENTDONE))
			SetModified(TRUE, TDCA_PERCENT, dwModTaskID);
		
		if (HasStyle(TDCS_SYNCTIMEESTIMATESANDDATES))
			SetModified(TRUE, TDCA_DUEDATE, dwModTaskID);
	}
	
	return (nRes != SET_FAILED);
}

BOOL CToDoCtrl::SetSelectedTaskTimeSpentUnits(TDC_UNITS nUnits, BOOL bRecalcTime)
{
	if (IsReadOnly())
		return FALSE;

	Flush();
	
	POSITION pos = TSH().GetFirstItemPos();
	int nRes = SET_NOCHANGE;
	DWORD dwModTaskID = 0;
	
	IMPLEMENT_UNDOEDIT();
		
	while (pos)
	{
		HTREEITEM hti = TSH().GetNextItem(pos);

		// ignore parent tasks
		if (!m_taskTree.ItemHasChildren(hti) || HasStyle(TDCS_ALLOWPARENTTIMETRACKING))
		{
			DWORD dwTaskID = GetTaskID(hti);

			TDC_UNITS nCurUnits;
			double dCurTime = m_data.GetTaskTimeSpent(dwTaskID, nCurUnits);

			if (nCurUnits != nUnits)
			{
				double dTime = dCurTime;

				if (bRecalcTime)
					dTime = CTimeHelper().GetTime(dTime, TDC::MapUnitsToTHUnits(nCurUnits), TDC::MapUnitsToTHUnits(nUnits));

				int nItemRes = m_data.SetTaskTimeSpent(dwTaskID, dTime, nUnits);
				
				if (nItemRes == SET_CHANGE)
				{
					nRes = SET_CHANGE;
					dwModTaskID = dwTaskID;
				}
			}
		}
	}
	
	// update UI
	if (nRes == SET_CHANGE)
	{
		if (m_nTimeSpentUnits != nUnits)
		{
			m_nTimeSpentUnits = nUnits;
			m_eTimeSpent.SetUnits(TDC::MapUnitsToTHUnits(m_nTimeSpentUnits));
		}

		// update controls if only one item selected
		if (GetSelectedCount() == 1)
		{
			if (bRecalcTime)
			{
				m_dTimeSpent = GetSelectedTaskTimeSpent(nUnits);
				UpdateDataEx(this, IDC_TIMESPENT, m_dTimeSpent, FALSE, DECIMALS);
			}
			// update % complete?
			else if (HasStyle(TDCS_AUTOCALCPERCENTDONE))
			{
				m_nPercentDone = m_data.CalcTaskPercentDone(GetSelectedTaskID());		
				UpdateDataEx(this, IDC_PERCENT, m_nPercentDone, FALSE);
			}
		}
		
		SetModified(TRUE, TDCA_TIMESPENT, dwModTaskID);
	}
	
	return (nRes != SET_FAILED);
}

BOOL CToDoCtrl::SetTextChange(int nChange, CString& sItem, const CString& sNewItem, TDC_ATTRIBUTE nAttrib, 
							  UINT nIDC, DWORD dwTaskID, CAutoComboBox* pCombo)
{
	ASSERT(!IsReadOnly());
	
	if (nChange == SET_CHANGE)
	{
		if (sItem != sNewItem)
		{
			// make sure it exists
			if (pCombo)
				pCombo->AddUniqueItem(sNewItem);

			sItem = sNewItem;
			UpdateDataEx(this, nIDC, sItem, FALSE);
		}
		
		SetModified(TRUE, nAttrib, dwTaskID);
	}
	
	return (nChange != SET_FAILED);
}

BOOL CToDoCtrl::SetSelectedTaskAllocTo(const CStringArray& aAllocTo)
{
	return SetSelectedTaskArray(TDCA_ALLOCTO, aAllocTo, FALSE, m_cbAllocTo);
}

BOOL CToDoCtrl::AppendSelectedTaskAllocTo(const CStringArray& aAllocTo)
{
	return SetSelectedTaskArray(TDCA_ALLOCTO, aAllocTo, TRUE, m_cbAllocTo);
}

BOOL CToDoCtrl::SetSelectedTaskAllocBy(const CString& sAllocBy)
{
	if (IsReadOnly())
		return FALSE;

	Flush();
	
	POSITION pos = TSH().GetFirstItemPos();
	TDC_SET nRes = SET_NOCHANGE;
	DWORD dwModTaskID = 0;
	
	IMPLEMENT_UNDOEDIT();
		
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		TDC_SET nItemRes = m_data.SetTaskAllocBy(dwTaskID, sAllocBy);
		
		if (nItemRes == SET_CHANGE)
		{
			nRes = SET_CHANGE;
			dwModTaskID = dwTaskID;
		}
	}
	
	return SetTextChange(nRes, m_sAllocBy, sAllocBy, TDCA_ALLOCBY, IDC_ALLOCBY, dwModTaskID, &m_cbAllocBy);
}

BOOL CToDoCtrl::SetSelectedTaskVersion(const CString& sVersion)
{
	if (IsReadOnly())
		return FALSE;

	Flush();
	
	POSITION pos = TSH().GetFirstItemPos();
	int nRes = SET_NOCHANGE;
	DWORD dwModTaskID = 0;
	
	IMPLEMENT_UNDOEDIT();
		
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		TDC_SET nItemRes = m_data.SetTaskVersion(dwTaskID, sVersion);
		
		if (nItemRes == SET_CHANGE)
		{
			nRes = SET_CHANGE;
			dwModTaskID = dwTaskID;
		}
	}
	
	return SetTextChange(nRes, m_sVersion, sVersion, TDCA_VERSION, IDC_VERSION, dwModTaskID, &m_cbVersion);
}

BOOL CToDoCtrl::SetSelectedTaskStatus(const CString& sStatus)
{
	if (IsReadOnly())
		return FALSE;

	Flush();
	
	POSITION pos = TSH().GetFirstItemPos();
	TDC_SET nRes = SET_NOCHANGE;
	DWORD dwModTaskID = 0;
	
	IMPLEMENT_UNDOEDIT();
		
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		TDC_SET nItemRes = m_data.SetTaskStatus(dwTaskID, sStatus);
		
		if (nItemRes == SET_CHANGE)
		{
			nRes = SET_CHANGE;
			dwModTaskID = dwTaskID;
		}
	}

	if (!SetTextChange(nRes, m_sStatus, sStatus, TDCA_STATUS, IDC_STATUS, dwModTaskID, &m_cbStatus))
		return FALSE;

	if (!m_sCompletionStatus.IsEmpty() && HasStyle(TDCS_SYNCCOMPLETIONTOSTATUS))
		SetSelectedTaskDone(sStatus == m_sCompletionStatus);

	return TRUE;
}

BOOL CToDoCtrl::SetSelectedTaskArray(TDC_ATTRIBUTE nAttrib, const CStringArray& aItems, 
									BOOL bAppend, CCheckComboBox& combo)
{
	DWORD dwRefTaskID = 0;
	TDC_SET nRes = SetSelectedTaskArray(nAttrib, aItems, bAppend, dwRefTaskID);
	
	if (nRes == SET_CHANGE)
	{
		ASSERT(dwRefTaskID);

		switch (nAttrib)
		{
		case TDCA_CATEGORY:	
		case TDCA_TAGS:			
		case TDCA_ALLOCTO:		
			if (bAppend)
			{
				CStringArray aTaskItems;
				m_data.GetTaskArray(dwRefTaskID, nAttrib, aTaskItems);

				combo.SetChecked(aTaskItems);
			}
			else
			{
				combo.SetChecked(aItems);
			}
			break;

		default:
			ASSERT(0);
			nRes = SET_FAILED;
			break;
		}
	}

	return (nRes != SET_FAILED);
}

TDC_SET CToDoCtrl::SetSelectedTaskArray(TDC_ATTRIBUTE nAttrib, const CStringArray& aItems, BOOL bAppend, DWORD& dwRefTaskID)
{
	if (IsReadOnly())
		return SET_FAILED;

	Flush();

	POSITION pos = TSH().GetFirstItemPos();
	TDC_SET nRes = SET_NOCHANGE;
	dwRefTaskID = 0;

	IMPLEMENT_UNDOEDIT();

	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		TDC_SET nItemRes = m_data.SetTaskArray(dwTaskID, nAttrib, aItems, bAppend);

		if (nItemRes == SET_CHANGE)
		{
			nRes = SET_CHANGE;
			dwRefTaskID = dwTaskID;
		}
	}

	if (nRes == SET_CHANGE)
		SetModified(TRUE, nAttrib, dwRefTaskID);

	return nRes;
}

BOOL CToDoCtrl::SetSelectedTaskCategories(const CStringArray& aCats)
{
	return SetSelectedTaskArray(TDCA_CATEGORY, aCats, FALSE, m_cbCategory);
}

BOOL CToDoCtrl::AppendSelectedTaskCategories(const CStringArray& aCats)
{
	return SetSelectedTaskArray(TDCA_CATEGORY, aCats, TRUE, m_cbCategory);
}

BOOL CToDoCtrl::SetSelectedTaskTags(const CStringArray& aTags)
{
	return SetSelectedTaskArray(TDCA_TAGS, aTags, FALSE, m_cbTags);
}

BOOL CToDoCtrl::AppendSelectedTaskTags(const CStringArray& aTags)
{
	return SetSelectedTaskArray(TDCA_TAGS, aTags, TRUE, m_cbTags);
}

BOOL CToDoCtrl::SetSelectedTaskFileRefs(const CStringArray& aFilePaths)
{
	return SetSelectedTaskFileRefs(aFilePaths, FALSE, FALSE);
}

BOOL CToDoCtrl::AppendSelectedTaskFileRefs(const CStringArray& aFilePaths)
{
	return SetSelectedTaskFileRefs(aFilePaths, TRUE, FALSE);
}

BOOL CToDoCtrl::SetSelectedTaskFileRefs(const CStringArray& aFilePaths, BOOL bAppend, BOOL bCtrlEdited)
{
	CStringArray aFileRefs;

	if (bAppend)
		GetSelectedTaskFileRefs(aFileRefs); // full paths

	Misc::AddUniqueItems(aFilePaths, aFileRefs);

	// convert to relative paths
	MakeRelativePaths(aFileRefs);

	DWORD dwRefTaskID = 0;
	TDC_SET nRes = SetSelectedTaskArray(TDCA_FILEREF, aFileRefs, bAppend, dwRefTaskID);
	
	if (nRes == SET_CHANGE)
	{
		ASSERT(dwRefTaskID);

		m_aFileRefs.Copy(aFileRefs);

		if (!bCtrlEdited)
			m_cbFileRef.SetFileList(aFileRefs);
	}

	return (nRes != SET_FAILED);
}

BOOL CToDoCtrl::SetSelectedTaskDependencies(const CStringArray& aDepends)
{
	return SetSelectedTaskDependencies(aDepends, FALSE);
}

BOOL CToDoCtrl::AppendSelectedTaskDependencies(const CStringArray& aDepends)
{
	return SetSelectedTaskDependencies(aDepends, TRUE);
}

BOOL CToDoCtrl::SetSelectedTaskDependencies(const CStringArray& aDepends, BOOL bAppend)
{
	DWORD dwRefTaskID = 0;
	TDC_SET nRes = SetSelectedTaskArray(TDCA_DEPENDENCY, aDepends, bAppend, dwRefTaskID);
	
	if (nRes == SET_CHANGE)
	{
		// Start and due dates might also have changed
		if (HasStyle(TDCS_AUTOADJUSTDEPENDENCYDATES))
		{
			UpdateControls(FALSE);
		}
		else
		{
			ASSERT(dwRefTaskID);
			m_sDepends = Misc::FormatArray(aDepends);
			UpdateDataEx(this, IDC_DEPENDS, m_sDepends, FALSE);
		}
	}

	return (nRes != SET_FAILED);
}

void CToDoCtrl::PauseTimeTracking(BOOL bPause) 
{ 
	if (bPause && !m_bTimeTrackingPaused)
	{
		m_bTimeTrackingPaused = TRUE;
	}
	else if (!bPause && m_bTimeTrackingPaused)
	{
		m_bTimeTrackingPaused = FALSE;
		ResetTimeTracking();
	}
}

BOOL CToDoCtrl::TimeTrackSelectedTask()
{
	if (!CanTimeTrackSelectedTask())
		return FALSE;
	
	TimeTrackTask(GetSelectedItem());
	
	return FALSE;
}

BOOL CToDoCtrl::CanTimeTrackSelectedTask() const
{
	if (IsReadOnly() || GetSelectedCount() != 1)
		return FALSE;
	
	return (m_data.IsTaskTimeTrackable(GetSelectedTaskID()));
}

BOOL CToDoCtrl::IsSelectedTaskBeingTimeTracked() const
{
	DWORD dwTaskID = GetSelectedTaskID();
	
	return (m_data.IsTaskTimeTrackable(dwTaskID) && m_dwTimeTrackTaskID == dwTaskID);
}

DWORD CToDoCtrl::GetTimeTrackTaskID(BOOL bActive) const
{
	if (!bActive || IsActivelyTimeTracking())
		return m_dwTimeTrackTaskID;

	// else
	return 0;
}

BOOL CToDoCtrl::IsActivelyTimeTracking() const
{
	if (!m_dwTimeTrackTaskID || m_bTimeTrackingPaused)
		return FALSE;

	DWORD dwSelTaskID = GetSelectedTaskID();
	BOOL bTrackingSelTask = (dwSelTaskID == m_dwTimeTrackTaskID);

	if (!bTrackingSelTask && HasStyle(TDCS_TRACKSELECTEDTASKONLY))
		return FALSE;
	
	// does it permit tracking
	DWORD dwTrackID = (bTrackingSelTask ? dwSelTaskID : m_dwTimeTrackTaskID);

	if (!m_data.IsTaskTimeTrackable(dwTrackID))
		return FALSE;

	// yeah!
	return TRUE;
}

BOOL CToDoCtrl::SetSelectedTaskFileRef(const CString& sFilePath)
{
	return SetSelectedTaskFileRef(sFilePath, FALSE);
}

BOOL CToDoCtrl::AppendSelectedTaskFileRef(const CString& sFilePath)
{
	return SetSelectedTaskFileRef(sFilePath, TRUE);
}
		
BOOL CToDoCtrl::SetSelectedTaskFileRef(const CString& sFilePath, BOOL bAppend)
{
	if (sFilePath.IsEmpty())
		return FALSE;
	
	CStringArray aFileRefs;

	if (bAppend)
		GetSelectedTaskFileRefs(aFileRefs); // full paths

	Misc::AddUniqueItem(sFilePath, aFileRefs);

	return SetSelectedTaskFileRefs(aFileRefs);
}
	
BOOL CToDoCtrl::SetSelectedTaskExtID(const CString& sID)
{
	if (IsReadOnly())
		return FALSE;

	Flush();
	
	POSITION pos = TSH().GetFirstItemPos();
	TDC_SET nRes = SET_NOCHANGE;
	DWORD dwModTaskID = 0;
	
	IMPLEMENT_UNDOEDIT();
		
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		TDC_SET nItemRes =	m_data.SetTaskExtID(dwTaskID, sID);
		
		if (nItemRes == SET_CHANGE)
		{
			nRes = SET_CHANGE;
 			dwModTaskID = dwTaskID;
		}
	}
	
	return SetTextChange(nRes, m_sExternalID, sID, TDCA_EXTERNALID, IDC_EXTERNALID, dwModTaskID);
}

BOOL CToDoCtrl::GetSelectedTaskRecurrence(TDCRECURRENCE& tr) const 
{ 
	if (!m_taskTree.GetSelectedTaskRecurrence(tr))
	{
		// initialise some options if regularity == once
		ASSERT(!tr.IsRecurring());

		tr.nReuse = m_nDefRecurReuse;
		tr.nRecalcFrom = m_nDefRecurFrom;

		return FALSE;
	}
	
	return TRUE;
}

int CToDoCtrl::GetSelectedTaskFileRefs(CStringArray& aFiles) const 
{  
	// external version always returns full paths
	return GetSelectedTaskFileRefs(aFiles, TRUE);
}

int CToDoCtrl::GetSelectedTaskFileRefs(CStringArray& aFiles, BOOL bFullPath) const 
{ 
	int nNumFiles = m_taskTree.GetSelectedTaskFileRefs(aFiles); 

	if (nNumFiles && bFullPath)
		MakeFullPaths(aFiles);

	return nNumFiles;
}

BOOL CToDoCtrl::GotoSelectedTaskFileRef(int nFile)
{
	return GotoFile(GetSelectedTaskFileRef(nFile, TRUE)); // full path
}

CString CToDoCtrl::GetSelectedTaskFileRef(int nFile, BOOL bFullPath) const 
{ 
	CString sFile = m_taskTree.GetSelectedTaskFileRef(nFile);

	if (!sFile.IsEmpty() && bFullPath && !WebMisc::IsURL(sFile))
	{
		CString sParentFolder = GetLastSaveFolder();
		FileMisc::MakeFullPath(sFile, sParentFolder); 
	}

	return sFile;
}

CString CToDoCtrl::GetSelectedTaskFileRef(int nFile) const
{
	// external version always returns full paths
	return GetSelectedTaskFileRef(nFile, TRUE);
}

BOOL CToDoCtrl::CreateNewTask(const CString& sText, TDC_INSERTWHERE nWhere, BOOL bEditText, DWORD dwDependency)
{
	if (!CanCreateNewTask(nWhere, sText))
		return FALSE;
	
	Flush();

	// handle special case when tasklist is empty
	if (GetTaskCount() == 0)
		nWhere = TDC_INSERTATBOTTOM;
	
	HTREEITEM htiParent = NULL, htiAfter = NULL;

	if (m_taskTree.GetInsertLocation(nWhere, htiParent, htiAfter))
	{
		HTREEITEM htiNew = InsertItem(sText, htiParent, htiAfter, bEditText, dwDependency);
		ASSERT(htiNew);

		DWORD dwTaskID = GetTaskID(htiNew);
		ASSERT(dwTaskID == (m_dwNextUniqueID - 1));

		return (htiNew != NULL);
	}

	// else
	ASSERT(0);
	return FALSE;
}

BOOL CToDoCtrl::CanCreateNewTask(TDC_INSERTWHERE nInsertWhere) const
{
	if (IsReadOnly())
		return FALSE;

	// can't handle multiple selection
	if (GetSelectedCount() > 1)
		return FALSE;

	HTREEITEM htiParent = NULL, htiAfter = NULL;
	VERIFY (m_taskTree.GetInsertLocation(nInsertWhere, htiParent, htiAfter));

	// inserting at root is always okay
	if (htiParent == TVI_ROOT || htiParent == NULL)
		return TRUE;

	switch (nInsertWhere)
	{
	case TDC_INSERTATTOP:
	case TDC_INSERTATBOTTOM:
		return TRUE;

	// parent cannot be reference
	case TDC_INSERTATTOPOFSELTASKPARENT:
	case TDC_INSERTATBOTTOMOFSELTASKPARENT:
	case TDC_INSERTAFTERSELTASK:
	case TDC_INSERTBEFORESELTASK:
	case TDC_INSERTATTOPOFSELTASK: 
	case TDC_INSERTATBOTTOMOFSELTASK:
		return (!m_data.IsTaskReference(GetTaskID(htiParent)));
	}

	ASSERT(0);
	return FALSE;
}

BOOL CToDoCtrl::CanCreateNewTask(TDC_INSERTWHERE nWhere, const CString& sText) const
{
	if (!CanCreateNewTask(nWhere))
		return FALSE;
	
	if (sText.IsEmpty())
		return FALSE;

	// are we an archive and should we warn user if we are
	if (m_bArchive && HasStyle(TDCS_WARNADDDELETEARCHIVE))
	{
		if (MessageBoxEx(this, IDS_TDC_WARNADDTOARCHIVE, IDS_TDC_CONFIRMADD_TITLE, 
						MB_YESNO | MB_ICONQUESTION) != IDYES) 
		{
			return FALSE;
		}
	}

	return TRUE;
}

TODOITEM* CToDoCtrl::CreateNewTask(HTREEITEM htiParent)
{
	// copy over parent attributes
	BOOL bRootParent = (!htiParent || htiParent == TVI_ROOT);
	DWORD dwParentID = bRootParent ? 0 : GetTaskID(htiParent);

	return m_data.NewTask(m_tdiDefault, dwParentID);
}

HTREEITEM CToDoCtrl::InsertItem(const CString& sText, HTREEITEM htiParent, HTREEITEM htiAfter, 
								BOOL bEdit, DWORD dwDependency = 0)
{
	m_dwLastAddedID = 0;
	
	if (IsReadOnly())
		return NULL;
	
	if (sText.IsEmpty())
		return NULL;
	
	IMPLEMENT_UNDO(TDCUAT_ADD);

	// create the new task item
	TODOITEM* pTDINew = CreateNewTask(htiParent);
	
	// and initialize
	pTDINew->sTitle = sText;

	// copy parent attribs
	DWORD dwParentID = GetTaskID(htiParent);
	DWORD dwAfterID = GetTaskID(htiAfter);

	// handle TVI_FIRST and TVI_LAST
	// Note: we don't modify the htiAfter directly because this is 
	// needed as-is in the call to CTreeCtrl::InsertItem
	if (htiAfter == TVI_FIRST)
	{
		dwAfterID = 0;
	}
	else if (htiAfter == TVI_LAST)
	{
		dwAfterID = GetTaskID(TCH().GetLastChildItem(htiParent));
	}

	// and map it
	m_data.AddTask(m_dwNextUniqueID, pTDINew, dwParentID, dwAfterID);
	
	// add corresponding tree item
	DWORD dwTaskID = m_dwNextUniqueID;

	HTREEITEM htiNew = m_taskTree.InsertItem(dwTaskID, htiParent, htiAfter);
	ASSERT(htiNew);
	
	if (htiNew)
	{
		// issues relating to non-root parent
		if (dwParentID)
		{
			// if the parent was being clocked then stop it now
			// since having children makes a task un-clockable
			if (!HasStyle(TDCS_ALLOWPARENTTIMETRACKING))
			{
				if (m_dwTimeTrackTaskID && (m_dwTimeTrackTaskID == dwParentID))
					EndTimeTracking(TRUE, TRUE);
			}

			// if the parent was marked as done and the new task 
			// is NOT cancellable, we mark the parent as incomplete.
			if (!bEdit && m_data.IsTaskDone(dwParentID))
				m_data.FixupParentCompletion(dwParentID);
		}
		
		m_dwNextUniqueID++;

		// Fixup dates for dependent tasks
		if (dwDependency)
		{
			CStringArray aDepends;
			aDepends.Add(Misc::Format(dwDependency));

			m_data.SetTaskDependencies(dwTaskID, aDepends, FALSE);
		}	

		SelectItem(htiNew);
		SetModified(TRUE, TDCA_NEWTASK, dwTaskID); 
		
		m_taskTree.InvalidateAll();

		if (bEdit)
			EditSelectedTask(TRUE);
		else
			SetFocusToTasks();
	}
	else // cleanup
	{
		m_data.DeleteTask(m_dwNextUniqueID);
	}
	
	return htiNew;
}

BOOL CToDoCtrl::SplitSelectedTask(int nNumSubtasks)
{
	if (nNumSubtasks < 2)
		return FALSE;
	
	if (!GetSelectedCount() || m_taskTree.SelectionHasReferences())
		return FALSE;

	Flush();
	
	IMPLEMENT_UNDO(TDCUAT_ADD);
	HOLD_REDRAW(NULL, m_taskTree.Tree());
	
	POSITION pos = TSH().GetFirstItemPos();
	int nNewTaskCount = 0;
	
	while (pos)
	{
		HTREEITEM hti = TSH().GetNextItem(pos);
				
		if (ItemHasChildren(hti))
			continue;
		
		DWORD dwTaskID = GetTaskID(hti);

		const TODOITEM* pTDI = GetTask(dwTaskID);
		ASSERT(pTDI);
		
		if (!pTDI || pTDI->IsDone())
			continue;
		
		// calculate how much time to allocate to each
		BOOL bHasDueTime = pTDI->HasDueTime();
		double dSubTime = 0, dSubEst = (pTDI->dTimeEstimate / nNumSubtasks);
		
		if (pTDI->HasStart() && bHasDueTime && (pTDI->dateDue > pTDI->dateStart))
			dSubTime = ((pTDI->dateDue - pTDI->dateStart) / nNumSubtasks);

		DWORD dwPrevSiblingID = 0;
		
		for (int nSubtask = 0; nSubtask < nNumSubtasks; nSubtask++)
		{
			TODOITEM* tdiSub = m_data.NewTask(*pTDI); // copy parent
			nNewTaskCount++;
			
			// allocate time slice and dates
			tdiSub->dTimeEstimate = dSubEst;
			
			if (dSubTime)
			{
				tdiSub->dateStart += (nSubtask * dSubTime);
				tdiSub->dateDue = tdiSub->dateStart + COleDateTime(dSubTime);

				// clear due time if parent didn't have any
				if (!bHasDueTime)
					tdiSub->dateDue = CDateHelper::GetDateOnly(tdiSub->dateDue);
			}
			else if (nSubtask) // not the first
			{
				tdiSub->ClearStart();
				tdiSub->ClearDue();
			}

			// clear time spent from all but first task
			if (nSubtask)
				tdiSub->dTimeSpent = 0;

			// map it
			DWORD dwChildID = m_dwNextUniqueID++;
			m_data.AddTask(dwChildID, tdiSub, dwTaskID, dwPrevSiblingID);
			
			// create tree item
			HTREEITEM htiSub = m_taskTree.InsertItem(dwChildID, hti, NULL);
			ASSERT(htiSub);
			
			dwPrevSiblingID = dwChildID;
		}

		// clear parent time spent/est
		m_data.ClearTaskAttribute(dwTaskID, TDCA_TIMEEST);
		m_data.ClearTaskAttribute(dwTaskID, TDCA_TIMESPENT);

		// show new subtasks
		m_taskTree.ExpandItem(hti, TRUE);
	}
	
	if (nNewTaskCount)
	{
		ASSERT(nNewTaskCount > 1);
		SetModified(TRUE, TDCA_NEWTASK, 0);

		return TRUE;
	}
	
	return FALSE;
}

BOOL CToDoCtrl::DeleteSelectedTask(BOOL bWarnUser, BOOL bResetSel)
{
	if (IsReadOnly())
		return FALSE;
	
	// check there's something to delete
	int nSelCount = GetSelectedCount();
	
	if (!nSelCount)
		return FALSE;

	// end time tracking as required
	if (m_dwTimeTrackTaskID && TSH().HasItem(m_dwTimeTrackTaskID))
		EndTimeTracking(FALSE, TRUE);

	// cache focus
	HWND hFocus = ::GetFocus();
	
	if (bWarnUser && !ConfirmDeleteAllTasks(TRUE))
		return FALSE;
	
	// Copy selected tasks without duplicate subtasks
	CHTIList selection;
	TSH().CopySelection(selection, TRUE);
	
	// reset anchor if we're going to delete it
	if (TSH().AnchorHasSelectedParent())
		TSH().SetAnchor(NULL);
	
	// remove real tree selection first if its going to be deleted
	// but before we do, lets figure out where the selection is
	// going to go _after_ the deletion
	HTREEITEM htiNextSel = NULL;
	
	if (bResetSel)
	{
		HTREEITEM hti = TSH().GetFirstItem();
		DWORD dwSelID = GetTaskID(hti);
		
		htiNextSel = TCH().GetNextVisibleItem(hti, FALSE);
		
		// don't include items we're about to delete!
		while (htiNextSel)
		{
			if (TSH().HasItem(htiNextSel) || 
				TSH().HasSelectedParent(htiNextSel) || 
				m_data.IsReferenceToTask(GetTaskID(htiNextSel), dwSelID))
			{
				htiNextSel = TCH().GetNextVisibleItem(htiNextSel);
			}
			else
			{
				break;
			}
		}
		
		// else try previous item
		if (!htiNextSel)
		{
			htiNextSel = TCH().GetPrevVisibleItem(hti, FALSE);
			
			// don't include items we're about to delete!
			while (htiNextSel)
			{
				if (TSH().HasItem(htiNextSel) || 
					TSH().HasSelectedParent(htiNextSel) ||
					m_data.IsReferenceToTask(GetTaskID(htiNextSel), dwSelID))
				{
					htiNextSel = TCH().GetPrevVisibleItem(htiNextSel);
				}
				else
				{
					break;
				}
			}
		}
	}
	
	if (TSH().HasItem(m_taskTree.GetSelectedItem()))
		TCH().SelectItem(NULL);
	
	// clear selection before deleting
	TSH().RemoveAll(TRUE);

	// Cache the task ID of a singly selected task
	// for later passing to SetModified
	DWORD dwDelTaskID = ((nSelCount == 1) ? GetTaskID(selection.GetHead()) : 0);

	{
		HOLD_REDRAW(NULL, m_taskTree.Tree());
		POSITION pos = selection.GetHeadPosition();

		while (pos)
		{
			HTREEITEM hti = selection.GetNext(pos);
			DWORD dwTaskID = m_taskTree.GetTaskID(hti);

			m_taskTree.DeleteItem(hti);
			m_data.DeleteTask(dwTaskID);
		}
		
		// Note: CToDoCtrlData ought to have already cleaned up the data
		m_taskTree.RemoveOrphanTreeItemReferences();
	}

	// refresh rest of UI
	SetModified(TRUE, TDCA_DELETE, dwDelTaskID);
	UpdateControls(FALSE); // don't update comments
	
	// set next selection
	if (htiNextSel)
		SelectItem(htiNextSel);

	// restore focus
	if (hFocus && ::IsWindowEnabled(hFocus))
		::SetFocus(hFocus);
	else
		SetFocusToTasks();

	return TRUE;
}

BOOL CToDoCtrl::DeleteSelectedTask()
{
	Flush();

	IMPLEMENT_UNDO(TDCUAT_DELETE);

	// else do the standard warning against deletion
	return DeleteSelectedTask(TRUE, TRUE);
}

BOOL CToDoCtrl::GetLabelEditRect(CRect& rScreen)
{
	if (m_taskTree.EnsureSelectionVisible())
		return m_taskTree.GetLabelEditRect(rScreen);

	// else
	return FALSE;
}

BOOL CToDoCtrl::EditSelectedTask(BOOL bTaskIsNew)
{
	if (IsReadOnly() || GetSelectedCount() != 1)
		return FALSE;

	if (IsTaskLabelEditing())
		return TRUE;
	
	// get edit rect
	CRect rPos;
	
	if (!GetLabelEditRect(rPos))
		return FALSE;

	// make sure it's at least 150 px long
	rPos.right = max(rPos.right, rPos.left + 150);

	// create edit if nec.
	if (!m_eTaskName.GetSafeHwnd() && !m_eTaskName.Create(this, IDC_FTC_EDIT, WS_POPUP | WS_CHILD | WS_BORDER))
		return FALSE;

	// start
	HTREEITEM htiSel = GetSelectedItem();
	ASSERT (htiSel);

	if (bTaskIsNew)
		m_dwLastAddedID = GetTaskID(htiSel);
	
	// save task id being edited
	SetEditTitleTaskID(GetSelectedTaskID());

	// set font
	CFont* pFontTree = m_taskTree.Tree().GetFont();

	if (pFontTree)
		m_eTaskName.SetFont(pFontTree);
	
	// set text
	m_eTaskName.SetWindowText(GetSelectedTaskTitle());
	
	// zero the left margin so that the text position remains the same
	m_eTaskName.SetMargins(0, HIWORD(m_eTaskName.GetMargins()));
	
	// move to the top of the Z-order
	m_eTaskName.SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	// and show
	m_eTaskName.Show(rPos);

	return TRUE;
}

BOOL CToDoCtrl::IsTaskLabelEditing() const
{ 
	return (!IsReadOnly() && (GetSelectedCount() == 1) && (m_dwEditTitleTaskID != 0));
}

LRESULT CToDoCtrl::OnEditEnd(WPARAM /*wParam*/, LPARAM lParam)
{
	ASSERT(m_dwEditTitleTaskID);

	CLockUpdates lu(m_taskTree);

	CString sText;
	m_eTaskName.GetWindowText(sText);

	if (GetSelectedCount() == 0) // user clicked into space
		SelectTask(m_dwEditTitleTaskID);

	// end can occurs either when the user selected return
	// or if the edit box loses the focus so we need to check
	// the lParam and only set the focus if the user chose return
	if (lParam)
		SetFocusToTasks();

	if (!sText.IsEmpty())
	{
		Flush();

		// Special handling: For new tasks we extend the previous undo
		BOOL bNewTask = (m_dwEditTitleTaskID == m_dwLastAddedID);
		TDCUNDOACTIONTYPE nAction = (bNewTask ? TDCUAT_ADD : TDCUAT_EDIT);

		IMPLEMENT_UNDOEXT(nAction, bNewTask);
		
		int nRes = m_data.SetTaskTitle(m_dwEditTitleTaskID, sText);
		
		if ((nRes == SET_CHANGE) || bNewTask)
		{
			m_taskTree.InvalidateSelection(TRUE);
			SetModified(TRUE, TDCA_TASKNAME, m_dwEditTitleTaskID);

			// If this was a new task and the parent was marked as done, 
			// now mark it as incomplete
			DWORD dwParentID = m_data.GetTaskParentID(m_dwEditTitleTaskID);

			if (bNewTask && dwParentID && m_data.IsTaskDone(dwParentID))
				m_data.FixupParentCompletion(dwParentID);
		}
	}

	m_dwLastAddedID = 0;
	SetEditTitleTaskID(0);

	return 0L;
}

void CToDoCtrl::SetEditTitleTaskID(DWORD dwTaskID)
{	
	m_dwEditTitleTaskID = dwTaskID;
	m_taskTree.SetEditTitleTaskID(dwTaskID);
}

LRESULT CToDoCtrl::OnEditCancel(WPARAM /*wParam*/, LPARAM lParam)
{
	if (GetSelectedCount() == 0) // user clicked into space
	{
		SelectTask(m_dwEditTitleTaskID);
	}
	// user hit escape key so we may need to delete this task if newly created
	else if (GetSelectedTaskID() == m_dwLastAddedID) 
	{
		ASSERT (lParam);
		UNREFERENCED_PARAMETER(lParam);

		// make sure this item is not selected
		HTREEITEM hti = GetSelectedItem();
		ASSERT(GetTaskID(hti) == m_dwLastAddedID);

		// set selection to previous task and if that fails then next task
		if (!GotoNextTask(TDCG_PREV) && !GotoNextTask(TDCG_NEXT))
			TSH().RemoveAll();
		
		// then delete and remove from undo
		{
			CHoldRedraw hr(m_taskTree);

			m_taskTree.DeleteItem(hti);
			m_data.DeleteTask(m_dwLastAddedID);
			m_data.DeleteLastUndoAction();

			SetModified(TRUE, TDCA_DELETE, m_dwLastAddedID);
		}

		// resync fields for selected task
		UpdateControls();
		
		// notify parent of deletion
		GetParent()->SendMessage(WM_TDCN_MODIFY, (WPARAM)GetSafeHwnd(), (LPARAM)TDCA_DELETE); 
	}

	SetFocusToTasks();

	m_dwEditTitleTaskID = m_dwLastAddedID = 0;
	return 0L;
}

BOOL CToDoCtrl::ConfirmDeleteAllTasks(BOOL bSelected) const
{
	if (!GetTaskCount() || IsReadOnly())
	{
		return FALSE;
	}
	// are we an archive and should we warn user if we are
	else if (m_bArchive && HasStyle(TDCS_WARNADDDELETEARCHIVE))
	{
		return (MessageBoxEx(this, IDS_TDC_WARNDELETEFROMARCHIVE, 
							IDS_TDC_CONFIRMDELETE_TITLE, 
							MB_YESNO | MB_ICONQUESTION) == IDYES);
	}
	else if (HasStyle(TDCS_CONFIRMDELETE))
	{
		CEnString sMessage(bSelected ? IDS_TDC_CONFIRMDELETESEL : IDS_TDC_CONFIRMDELETEALL);
		UINT nFlags = (MB_ICONEXCLAMATION | MB_YESNO);

		if (bSelected && m_taskTree.SelectionHasSubtasks())
		{
			if (GetSelectedCount() == 1)
				sMessage += CEnString(IDS_TDC_CONFIRMDELETEONEWARNSUBTASKS);
			else
				sMessage += CEnString(IDS_TDC_CONFIRMDELETEMOREWARNSUBTASKS);

			nFlags |= MB_DEFBUTTON2;
		}
		// else MB_DEFBUTTON1
			
		return (MessageBoxEx(this, sMessage, IDS_TDC_CONFIRMDELETE_TITLE, nFlags) == IDYES);
	}
		
	// all else
	return TRUE;
}

BOOL CToDoCtrl::DeleteAllTasks()
{
	if (!ConfirmDeleteAllTasks())
		return FALSE;

	// must do these first
	Flush();
	m_taskTree.DeselectAll();
	
	CWaitCursor cursor;
	
	IMPLEMENT_UNDO(TDCUAT_DELETE);
	HOLD_REDRAW(*this, 0);

	m_taskTree.DeleteAll();
	m_data.DeleteAllTasks();

	SetModified(TRUE, TDCA_DELETE, 0);
	UpdateControls();
	
	return TRUE;
}

BOOL CToDoCtrl::SetStyle(TDC_STYLE nStyle, BOOL bOn)
{
	HandleUnsavedComments();

	return SetStyle(nStyle, bOn, TRUE);
}

BOOL CToDoCtrl::SetStyle(TDC_STYLE nStyle, BOOL bOn, BOOL bWantUpdate)
{
	ASSERT (GetSafeHwnd());
	
	bOn = bOn ? TRUE : FALSE; // normalize
	
	if (bOn != HasStyle(nStyle))
	{
        m_aStyles[(int)nStyle] = (WORD)bOn;
		
		switch (nStyle)
		{
		case TDCS_SHOWDEFAULTTASKICONS:
			LoadTaskIcons();
			break;
			
		case TDCS_SHOWDATESINISO:
			{
				DWORD dwStyle = m_cbTimeDue.GetStyle();
				
				if (bOn)
					dwStyle |= TCB_ISO;
				else
					dwStyle &= ~(TCB_ISO);
				
				m_cbTimeStart.SetStyle(dwStyle);
				m_cbTimeDue.SetStyle(dwStyle);
				m_cbTimeDone.SetStyle(dwStyle);
			}
			break;

		case TDCS_NODUEDATEISDUETODAYORSTART:
		case TDCS_SHOWPATHINHEADER:
		case TDCS_SHOWINFOTIPS:
		case TDCS_SHOWCOMMENTSINLIST:
		case TDCS_SHOWFIRSTCOMMENTLINEINLIST:
		case TDCS_STRIKETHOUGHDONETASKS:
		case TDCS_RIGHTSIDECOLUMNS:
		case TDCS_DISPLAYHMSTIMEFORMAT:
		case TDCS_TREATSUBCOMPLETEDASDONE:
		case TDCS_CALCREMAININGTIMEBYDUEDATE:
		case TDCS_CALCREMAININGTIMEBYSPENT:
		case TDCS_CALCREMAININGTIMEBYPERCENT:
		case TDCS_USEHIGHESTPRIORITY:
		case TDCS_INCLUDEDONEINPRIORITYCALC:
		case TDCS_HIDEPRIORITYNUMBER:
		case TDCS_USEHIGHESTRISK:
		case TDCS_INCLUDEDONEINRISKCALC:
		case TDCS_SHOWNONFILEREFSASTEXT:
		case TDCS_USEPERCENTDONEINTIMEEST:
		case TDCS_HIDEZEROTIMECOST:
		case TDCS_ROUNDTIMEFRACTIONS:
		case TDCS_HIDEPERCENTFORDONETASKS:
		case TDCS_INCLUDEDONEINAVERAGECALC:
		case TDCS_WEIGHTPERCENTCALCBYNUMSUB:
		case TDCS_SHOWPERCENTASPROGRESSBAR:
		case TDCS_HIDEZEROPERCENTDONE:
		case TDCS_AVERAGEPERCENTSUBCOMPLETION:
		case TDCS_AUTOCALCPERCENTDONE:
		case TDCS_HIDESTARTDUEFORDONETASKS:
		case TDCS_SHOWWEEKDAYINDATES:
			// handled solely by tree-list
			break;
						
		case TDCS_USEEARLIESTDUEDATE:
			if (bOn)
				m_aStyles[TDCS_USELATESTDUEDATE] = FALSE; // mutually exclusive
			break;

		case TDCS_USELATESTDUEDATE:
			if (bOn)
				m_aStyles[TDCS_USEEARLIESTDUEDATE] = FALSE; // mutually exclusive
			break;

		case TDCS_USEEARLIESTSTARTDATE:
			if (bOn)
				m_aStyles[TDCS_USELATESTSTARTDATE] = FALSE; // mutually exclusive
			break;

		case TDCS_USELATESTSTARTDATE:
			if (bOn)
				m_aStyles[TDCS_USEEARLIESTSTARTDATE] = FALSE; // mutually exclusive
			break;
	
		case TDCS_TASKCOLORISBACKGROUND:
			if (bOn)
			{
				m_cpColour.SetSelectionMode(CP_MODE_BK);
				m_cpColour.SetBkColour(m_cpColour.GetTextColour());
				m_cpColour.SetTextColour(CLR_DEFAULT);
				
			}
			else
			{
				m_cpColour.SetSelectionMode(CP_MODE_TEXT);
				m_cpColour.SetTextColour(m_cpColour.GetBkColour());
				m_cpColour.SetBkColour(CLR_DEFAULT);
			}
			break;
			
		case TDCS_SORTDONETASKSATBOTTOM:
			if (bWantUpdate)
				Resort();
			break;
			
		case TDCS_DUEHAVEHIGHESTPRIORITY:
		case TDCS_DONEHAVELOWESTPRIORITY:
			if (bWantUpdate && (GetSortBy() == TDCC_PRIORITY))
				Resort();
			break;
			
		case TDCS_DONEHAVELOWESTRISK:
			if (bWantUpdate && (GetSortBy() == TDCC_RISK))
				Resort();
			break;

		case TDCS_SHOWPROJECTNAME:
			{
				// ensure focus is ok
				if (!bOn && (GetFocus() == GetDlgItem(IDC_PROJECTNAME)))
					SetFocusToTasks();
				
				if (bWantUpdate)
				{
					HOLD_REDRAW(*this, 0);
					Resize();
				}
			}
			break;
			
		case TDCS_SHAREDCOMMENTSHEIGHT:
			if (bOn)
				LoadSplitPos(CPreferences());
			else
				m_nCommentsSize = s_nCommentsSize;
			// fall thru
			
		case TDCS_SHOWCOMMENTSALWAYS:
		case TDCS_AUTOREPOSCTRLS:
		case TDCS_COLORTEXTBYPRIORITY:
		case TDCS_COLORTEXTBYATTRIBUTE:
		case TDCS_COLORTEXTBYNONE:
		case TDCS_ALLOWCOMMENTSSTACKING:
			if (bWantUpdate)
				Resize();
			break;
			
		case TDCS_READONLY:
		case TDCS_CONFIRMDELETE:
		case TDCS_CHECKOUTONLOAD:
		case TDCS_SORTVISIBLETASKSONLY:
		case TDCS_SYNCTIMEESTIMATESANDDATES:
		case TDCS_FOCUSTREEONENTER:
		case TDCS_TRACKSELECTEDTASKONLY:
		case TDCS_LOGTIMETRACKING:
		case TDCS_LOGTASKTIMESEPARATELY:
		case TDCS_WARNADDDELETEARCHIVE:
		case TDCS_ALLOWPARENTTIMETRACKING:
		case TDCS_USES3RDPARTYSOURCECONTROL:
		case TDCS_REFILTERONMODIFY:
		case TDCS_SHOWPARENTSASFOLDERS:
		case TDCS_INCLUDEUSERINCHECKOUT:
		case TDCS_DISPLAYLOGCONFIRM:
		case TDCS_SYNCCOMPLETIONTOSTATUS:
			// do nothing
			break;

		case TDCS_AUTOADJUSTDEPENDENCYDATES:
			if (bOn)
			{
				// :TODO: Auto adjust all dependents' dates
			}
			break;
			
		case TDCS_COMMENTSUSETREEFONT:
			UpdateCommentsFont(TRUE);
			break;
			
		default:
			//ASSERT(0); // just to help catch forgotten styles
			break;
		}
		
		if (bWantUpdate)
			UpdateControls();

		// notify tree-list to update itself
		m_taskTree.OnStyleUpdated(nStyle, bOn, bWantUpdate);
		
		return TRUE; // style was changed
	}
	
	return FALSE; // no change
}

BOOL CToDoCtrl::SetStyles(const CTDCStylesMap& styles)
{
	HandleUnsavedComments();

	// first evaluate whether there any changes or not
	// without changing anything
	BOOL bChange = FALSE;
	int nStyle;
	
	for (nStyle = TDCS_FIRST; nStyle < TDCS_LAST && !bChange; nStyle++)
	{
		BOOL bWantOn = -1, bIsOn = HasStyle((TDC_STYLE)nStyle); // undefined
		
		if (styles.Lookup((TDC_STYLE)nStyle, bWantOn))
		{
			bWantOn = bWantOn ? 1 : 0; // normalize
			bChange = (bWantOn != bIsOn);
		}
	}
	
	if (!bChange)
		return FALSE;
	
	// then update the styles
	CHoldRedraw hr(*this, (NCR_PAINT | NCR_ERASEBKGND));
	
	for (nStyle = TDCS_FIRST; nStyle < TDCS_LAST; nStyle++)
	{
		BOOL bOn = -1; // undefined
		
		if (styles.Lookup((TDC_STYLE)nStyle, bOn))
			SetStyle((TDC_STYLE)nStyle, bOn, FALSE); // FALSE => don't update
	}

	// notify tree-list we've finished changing styles
	OnStylesUpdated();
	
	// misc
	ValidateCommentsSize();
	Resize();
	Invalidate(TRUE);

	UpdateControls();
	Resort(); 

	return TRUE;
}

BOOL CToDoCtrl::HasStyle(TDC_STYLE nStyle) const 
{ 
	// some special handling for dependent styles
	switch (nStyle)
	{
	case TDCS_SHOWFIRSTCOMMENTLINEINLIST:
		if (!HasStyle(TDCS_SHOWCOMMENTSINLIST))
            return FALSE;
		break;

	case TDCS_LOGTASKTIMESEPARATELY:
		if (!HasStyle(TDCS_LOGTIMETRACKING))
            return FALSE;
		break;
	}
	
	return (m_aStyles[nStyle] ? TRUE : FALSE); 
}

void CToDoCtrl::SetCompletionStatus(const CString& sStatus) 
{ 
	if (sStatus != m_sCompletionStatus)
	{
		m_sCompletionStatus = sStatus; 
		m_taskTree.SetCompletionStatus(sStatus);
	}
}

void CToDoCtrl::SetLayoutPositions(TDC_UILOCATION nControlsPos, TDC_UILOCATION nCommentsPos, BOOL bResize)
{
	BOOL bChanged = (nControlsPos != m_nControlsPos || nCommentsPos != m_nCommentsPos);

	m_nControlsPos = nControlsPos;
	m_nCommentsPos = nCommentsPos;

	if (bChanged && bResize)
	{
		ValidateCommentsSize();
		Resize();
	}
}

int CToDoCtrl::GetCustomAttributeDefs(CTDCCustomAttribDefinitionArray& aAttrib) const
{
	aAttrib.Copy(m_aCustomAttribDefs);
	return aAttrib.GetSize();
}

void CToDoCtrl::SetCustomAttributeDefs(const CTDCCustomAttribDefinitionArray& aAttrib)
{
	ASSERT(!IsReadOnly());

	if (!Misc::MatchAllT(m_aCustomAttribDefs, aAttrib))
	{
		m_aCustomAttribDefs.Copy(aAttrib);
		RebuildCustomAttributeUI();

		// update interface
		SetModified(TRUE, TDCA_CUSTOMATTRIBDEFS, 0);
		UpdateControls(FALSE); // don't update comments
	}
}

void CToDoCtrl::UpdateVisibleColumns()
{
	m_taskTree.OnColumnVisibilityChange();
	
	// hide/show controls which may have been affected
	if (m_visColAttrib.GetShowEditsAndFilters() == TDLSA_ASCOLUMN)
	{
		UpdateControls(FALSE); // don't update comments
		
		// re-align controls
		Resize();
	}
}

void CToDoCtrl::SetColumnEditFilterVisibility(const TDCCOLEDITFILTERVISIBILITY& vis)
{
	BOOL bColumnChange, bEditChange, bUnused;
	BOOL bChange = m_visColAttrib.CheckForDiff(vis, bColumnChange, bEditChange, bUnused);

	m_visColAttrib = vis;

	if (bColumnChange)
		UpdateVisibleColumns();
		
	if (bEditChange)
	{		
		if (!bColumnChange)
			m_taskTree.RecalcColumnWidths();

		Resize();
		UpdateControls(FALSE); // don't update comments
	}

	if (bChange && HasStyle(TDCS_SAVEUIVISINTASKLIST))
		m_bModified = TRUE;
}

void CToDoCtrl::GetColumnEditFilterVisibility(TDCCOLEDITFILTERVISIBILITY& vis) const
{
	vis = m_visColAttrib;
}

const CTDCColumnIDMap& CToDoCtrl::GetVisibleColumns() const
{
	return m_visColAttrib.GetVisibleColumns();
}

const CTDCAttributeMap& CToDoCtrl::GetVisibleEditFields() const
{
	return m_visColAttrib.GetVisibleEditFields();
}

const CTDCAttributeMap& CToDoCtrl::GetVisibleFilterFields() const
{
	return m_visColAttrib.GetVisibleFilterFields();
}

BOOL CToDoCtrl::IsColumnShowing(TDC_COLUMN nColumn) const
{
	if (nColumn == TDCC_CLIENT || CTDCCustomAttributeHelper::IsCustomColumn(nColumn))
		return TRUE; // always visible

	return m_visColAttrib.IsColumnVisible(nColumn);
}

BOOL CToDoCtrl::IsEditFieldShowing(TDC_ATTRIBUTE nAttrib) const
{
	if (nAttrib == TDCA_TASKNAME || CTDCCustomAttributeHelper::IsCustomAttribute(nAttrib))
		return TRUE; // always visible

	return m_visColAttrib.IsEditFieldVisible(nAttrib);
}

BOOL CToDoCtrl::IsColumnOrEditFieldShowing(TDC_COLUMN nColumn, TDC_ATTRIBUTE nAttrib) const
{
	return (IsColumnShowing(nColumn) || IsEditFieldShowing(nAttrib));
}

TDC_FILE CToDoCtrl::Save(const CString& sFilePath)
{
	CTaskFile tasks;

	return Save(tasks, sFilePath);
}

TDC_FILE CToDoCtrl::Save(CTaskFile& tasks/*out*/, const CString& sFilePath)
{
	ASSERT (GetSafeHwnd());
	
	if (!GetSafeHwnd())
		return TDCF_OTHER;

	// can't save if file is readonly
	if (CDriveInfo::IsReadonlyPath(sFilePath) > 0)
	{
		return TDCF_NOTALLOWED;
	}
	
	// can't save if not checked-out
	// unless we're saving to another filename or this is our first save
	BOOL bFirstSave = (m_sLastSavePath.IsEmpty() || 
						!FileMisc::IsSamePath(m_sLastSavePath, sFilePath));
	
	if (m_bSourceControlled && !m_bCheckedOut && !bFirstSave)
	{
		return TDCF_SSC_NOTCHECKEDOUT;
	}

	CString sSavePath(sFilePath);
	
	if (sSavePath.IsEmpty())
	{
		if (m_sLastSavePath.IsEmpty())
		{
			return TDCF_OTHER;
		}
		else
		{
			sSavePath = m_sLastSavePath;
		}
	}

	// check for later changes if it's a network file
	BOOL bCheckforLaterChanges = (CDriveInfo::IsRemotePath(sFilePath) > 0);
	
	if (bCheckforLaterChanges && m_nFileVersion > 0) // else its newly created
	{
		if (FileMisc::FileExists(sSavePath)) // file exists (sanity check)
		{
			// i was going to use filetimes but these are too unreliable
			// instead we open the xml file and look at its internal version
			CTaskFile temp;
			CXmlParseController xpc(TDL_FILEVERSION);
			
			if (temp.Load(sSavePath, &xpc, FALSE)) // FALSE => don't decrypt
			{
				if (temp.GetFileVersion() > m_nFileVersion)
				{
					CEnString sMessage(IDS_TDC_CONFIRMOVERWRITE, sSavePath);
					
					if (MessageBoxEx(this, sMessage, IDS_TDC_CONFIRMOVERWRITE_TITLE, 
						MB_ICONWARNING | MB_YESNO) != IDYES)
					{
						return TDCF_CANCELLED;
					}
				}
			}
		}
	}
	
	// prepare task file
	BuildTasksForSave(tasks, bFirstSave);

	// backup the file if opening in read-write
	CTempFileBackup backup(sSavePath);
	
	// do the save
	if (tasks.Save(sSavePath, SFEF_UTF16))
	{
		m_sLastSavePath = sSavePath;
		m_bModified = FALSE;
		m_bCheckedOut = tasks.IsCheckedOutTo(GetSourceControlID());
		
		// update fileref with the todolist's folder
		m_cbFileRef.SetCurrentFolder(FileMisc::GetFolderFromFilePath(sSavePath));

		return TDCF_SUCCESS;
	}

	// restore the backup
	backup.RestoreBackup();
	
	return MapTaskfileError(tasks.GetLastFileError());
}

TDC_FILE CToDoCtrl::MapTaskfileError(int nFileErr)
{
	switch (nFileErr)
	{
	case XFL_BADMSXML:
		return TDCF_BADMSXML;
		
	default:
		// if nFileErr is greater than zero then it represents GetLastError
		// so we append this to TDCO_OTHER
		if (nFileErr > 0)
		{
			switch (nFileErr)
			{
			case ERROR_ACCESS_DENIED:		return TDCF_NOTALLOWED;
			case ERROR_SHARING_VIOLATION:	return TDCF_INUSE;
			case ERROR_DISK_FULL:			return TDCF_NOSPACE;
				
			default: return (TDC_FILE)(TDCF_OTHER + nFileErr);
			}
		}
	}
	
	// all else
	return TDCF_OTHER;
}

void CToDoCtrl::BuildTasksForSave(CTaskFile& tasks, BOOL bFirstSave)
{
	tasks.Reset();

	// get tasklist
	GetAllTasks(tasks);

	// file header info
	if (IsModified())
		m_nFileVersion++;
	
	// save globals
	SaveGlobals(tasks);

	// attrib visibility
 	if (HasStyle(TDCS_SAVEUIVISINTASKLIST))
 		tasks.SetAttributeVisibility(m_visColAttrib);

	// and meta data
	tasks.SetMetaData(m_mapMetaData);

	// encrypt prior to setting checkout status and file info (so these are visible without decryption)
	// this simply fails if password is empty
	tasks.Encrypt(m_sPassword, SFEF_UTF16);

	// then append header info
	AppendTaskFileHeader(tasks);
	
	// checkout status
	// if this is a first time save and source control is enabled
	// then check it out
	if (m_bSourceControlled)
	{
		if (m_bCheckedOut || bFirstSave)
			tasks.SetCheckedOutTo(GetSourceControlID());
		else
			tasks.SetCheckedOutTo(_T(""));
	}
	// else CHECKEDOUTTO not added to taskfile
}

void CToDoCtrl::SaveGlobals(CTaskFile& file) const
{
	// Note: we always remove the default items before we save
	TDCAUTOLISTDATA tld;
	
	if (GetAutoListData(tld))
	{
		tld.RemoveItems(m_tldDefault);
		file.SetAutoListData(tld);
	}
}

void CToDoCtrl::LoadGlobals(const CTaskFile& file)
{
	TDCAUTOLISTDATA tld;
	
	if (file.GetAutoListData(tld))
	{
		AddUserListContent(m_cbCategory, tld.aCategory);
		AddUserListContent(m_cbTags, tld.aTags);
		AddUserListContent(m_cbStatus, tld.aStatus);
		AddUserListContent(m_cbAllocTo, tld.aAllocTo);
		AddUserListContent(m_cbAllocBy, tld.aAllocBy);
		AddUserListContent(m_cbVersion, tld.aVersion);
	}
}

void CToDoCtrl::AddUserListContent(CAutoComboBox& combo, const CStringArray& aItems)
{
	// we only bother updating the combos if their content is 'writable'
	if (CDialogHelper::ComboHasEdit(combo))
		combo.AddUniqueItems(aItems);
}

void CToDoCtrl::SaveCustomAttributeDefinitions(CTaskFile& file) const
{
	// save auto combobox contents to definition first
	// just like we do with standard combos
	CTDCCustomAttributeHelper::SaveAutoListDataToDefs(this, m_aCustomControls, m_aCustomAttribDefs);

	file.SetCustomAttributeDefs(m_aCustomAttribDefs);
}

void CToDoCtrl::LoadCustomAttributeDefinitions(const CTaskFile& file)
{
	file.GetCustomAttributeDefs(m_aCustomAttribDefs);

	// Add Fields and columns to view
	RebuildCustomAttributeUI();
}

void CToDoCtrl::RebuildCustomAttributeUI()
{
	// and add fields after the 'version' control
 	CTDCCustomAttributeHelper::RebuildCustomAttributeUI(m_aCustomAttribDefs, m_aCustomControls, 
 														m_ilTaskIcons, this, IDC_VERSION);
	Resize();

	m_taskTree.OnCustomAttributeChange();
}

BOOL CToDoCtrl::CheckRestoreBackupFile(const CString& sFilePath)
{
	// check for the existence of a backup file
	CString sBackup = CFileBackup::BuildBackupPath(sFilePath);
	
	if (FileMisc::FileExists(sBackup))
	{
		double dBackupSize = FileMisc::GetFileSize(sBackup);
		
		// Check for valid backup
		if (dBackupSize == 0.0)
		{
			// Nothing we can do except delete it
			FileMisc::DeleteFile(sFilePath, TRUE);
			return TRUE; 
		}
		
		double dSize = FileMisc::GetFileSize(sFilePath);
		
		if (dSize == 0.0) // definitely a bad save -> copy over backup
		{
			FileMisc::DeleteFile(sFilePath, TRUE);
			FileMisc::MoveFile(sBackup, sFilePath);
		}
		else if (dSize == dBackupSize) // same size == same file
		{
			::DeleteFile(sBackup);
		}
		else // Different sizes -> prompt
		{
			CEnString sMessage(IDS_BACKUPFILEFOUND, sFilePath);
			int nRet = AfxMessageBox(sMessage, MB_YESNOCANCEL);
			
			switch (nRet)
			{
			case IDYES:
				FileMisc::DeleteFile(sFilePath, TRUE);
				FileMisc::MoveFile(sBackup, sFilePath);
				break;
				
			case IDNO: // keep the backup just in case
				{
					CString sRename = CFileBackup::BuildBackupPath(sBackup);
					::MoveFile(sBackup, sRename);
				}
				break;
				
			case IDCANCEL:
				return FALSE; // do nothing
			}
		}
	}
	
	// all else
	return TRUE;
}

// thin wrapper
TDC_FILE CToDoCtrl::Load(const CString& sFilePath)
{
	CTaskFile file;
	return Load(sFilePath, file);
}

TDC_FILE CToDoCtrl::Load(const CString& sFilePath, CTaskFile& tasks/*out*/)
{
	ASSERT (GetSafeHwnd());
	
	if (!GetSafeHwnd())
		return TDCF_OTHER;
	
	if (!CheckRestoreBackupFile(sFilePath))
		return TDCF_CANCELLED;

	if (!FileMisc::FileExists(sFilePath))
		return TDCF_NOTEXIST;

	// set password before opening taskfile
	tasks.SetPassword(m_sPassword);
	
	// work out whether we're going to write to the file
	// as part of the open -> checkout
	BOOL bReadOnly = (CDriveInfo::IsReadonlyPath(sFilePath) > 0);

	SetReadonly(bReadOnly); // so that other functions know we are read-only

	if (!tasks.Open(sFilePath, XF_READ, TRUE))
	{
		// do minor error handling
		switch (tasks.GetLastFileError())
		{
		case ERROR_ACCESS_DENIED:		return TDCF_NOTALLOWED;
		case ERROR_SHARING_VIOLATION:	return TDCF_INUSE;
			
		default:						return TDCF_OTHER;
		}
	}

	if (tasks.LoadEx())
	{
		tasks.Close();

		m_bSourceControlled = tasks.IsSourceControlled();

		CString sCheckedOutTo = tasks.GetCheckOutTo();
		m_bCheckedOut = (m_bSourceControlled && (sCheckedOutTo == GetSourceControlID()));
		
		if (tasks.Decrypt())
		{
			// save off password
			m_sPassword = tasks.GetPassword();
			
			// get comments type from header of older tasklists for the
			// setting of the enclosed tasks
			CONTENTFORMAT cf = tasks.GetCommentsType();

			if (!cf.IsEmpty())
				m_cfComments = cf;

			// meta data
			tasks.GetMetaData(m_mapMetaData);

			// and selection history unless reloading
			if (m_sLastSavePath != sFilePath)
			{
				TSH().RemoveAll();
				TSH().ClearHistory();
			}
					
			LoadTasks(tasks);
			LoadTaskIcons();
			
			// init fileref with the todolist's folder
			m_cbFileRef.SetCurrentFolder(FileMisc::GetFolderFromFilePath(sFilePath));

			if (m_bSourceControlled && !m_bCheckedOut && 
				HasStyle(TDCS_CHECKOUTONLOAD) && sCheckedOutTo.IsEmpty())
			{
				m_bCheckedOut = TRUE;
				
				// resave
				BuildTasksForSave(tasks, FALSE);
				
				m_bCheckedOut = tasks.Save(sFilePath, SFEF_UTF16);
			}
			
			SetModified(FALSE);
			
			return TDCF_SUCCESS;
		}
	}

	// else do error handling
	int nFileErr = tasks.GetLastFileError();
	
	switch (nFileErr)
	{
	case XFL_CANCELLED:			return TDCF_CANCELLED;
	case XFL_MISSINGROOT:		return TDCF_NOTTASKLIST;
	case XFL_BADMSXML:			return TDCF_BADMSXML;
	case XFL_NOENCRYPTIONDLL:	return TDCF_NOENCRYPTIONDLL;
	case XFL_UNKNOWNENCRYPTION:	return TDCF_UNKNOWNENCRYPTION;
	}

	// if nFileErr is greater than zero then it represents GetLastError
	// so we append this to TDCO_OTHER
	if (nFileErr > 0)
		return (TDC_FILE)(TDCF_OTHER + nFileErr);
	
	// all else
	return TDCF_OTHER;
}

BOOL CToDoCtrl::DelayLoad(const CString& sFilePath, COleDateTime& dtEarliestDue)
{
	ASSERT (m_bDelayLoaded || m_sLastSavePath.IsEmpty());

	m_bDelayLoaded = FALSE;
	CTaskFile temp;
	
	if (temp.LoadHeader(sFilePath) && (temp.IsArchive() || temp.GetEarliestDueDate(dtEarliestDue)))
	{
		m_bDelayLoaded = TRUE;

		// save off some of the header info
		SetFilePath(sFilePath);
		SetProjectName(temp.GetProjectName());
		SetModified(FALSE);

		temp.GetCustomAttributeDefs(m_aCustomAttribDefs);
	}

	return m_bDelayLoaded;
}

BOOL CToDoCtrl::LoadTasks(const CTaskFile& file)
{
	ASSERT (GetSafeHwnd());
	
	if (!GetSafeHwnd())
		return FALSE;

	// save visible state
	BOOL bHidden = !IsWindowVisible();

	// save current state if reloading same file
	TDCSELECTIONCACHE cache;
	CPreferences prefs;

	if (!m_bDelayLoaded && (m_sLastSavePath == file.GetFilePath()))
	{
		CacheTreeSelection(cache);

		SaveTasksState(prefs);
		SaveSplitPos(prefs);
		SaveAttributeVisibility(prefs);
	}	
	
	// Update XML headers if not already unicode
	m_sXslHeader = file.GetXslHeader();
	m_sXmlHeader = file.GetXmlHeader();

	if (!XMLHeaderIsUnicode(m_sXmlHeader))
		m_sXmlHeader = DEFAULT_UNICODE_HEADER;

	// file header info
	m_nFileFormat = file.GetFileFormat();
	m_nFileVersion = file.GetFileVersion();
	m_sProjectName = file.GetProjectName();
	m_bArchive = file.IsArchive();
	m_dtLastTaskMod = file.GetLastModifiedOle();
	
	// next unique ID
	m_dwNextUniqueID = file.GetNextUniqueID();
	
	// backwards compatibility
	if (!m_dwNextUniqueID)
		m_dwNextUniqueID = 1;
	
	m_bDelayLoaded = FALSE;
	m_sLastSavePath = file.GetFilePath();

	m_taskTree.SetTasklistFolder(FileMisc::GetFolderFromFilePath(m_sLastSavePath));
	
	LoadGlobals(file);
	LoadCustomAttributeDefinitions(file);
	LoadSplitPos(prefs);
	LoadDefaultRecurrence(prefs);
	
	// attrib visibility can be stored inside the file or the preferences
 	TDCCOLEDITFILTERVISIBILITY vis;
 
 	if (file.GetAttributeVisibility(vis))
 	{
 		SetColumnEditFilterVisibility(vis);
 
 		// update style to match
 		SetStyle(TDCS_SAVEUIVISINTASKLIST);
 	}
 	else
	{
		LoadAttributeVisibility(prefs);
	}

	if (file.GetTaskCount())
	{
		HOLD_REDRAW(*this, m_taskTree.Tree());
	
		HTREEITEM htiSel = NULL;
		DWORD dwFirstVis = GetTaskID(m_taskTree.Tree().GetFirstVisibleItem());
		HTREEITEM htiFirst = SetAllTasks(file);

		if (m_taskTree.GetItemCount())
		{
			// restore last tree state
			htiSel = LoadTasksState(prefs);
			
			// redo last sort
			if (IsSorting())
			{
				Resort(FALSE); // FALSE means 'Don't Toggle'
			}
		}

		// scroll to first visible item
		if (dwFirstVis)
		{
			HTREEITEM htiFirstVis = m_taskTree.GetItem(dwFirstVis);
			
			if (htiFirstVis)
				m_taskTree.Tree().SelectSetFirstVisible(htiFirstVis);
		}
		
		// restore last selected state
		if (cache.aSelTaskIDs.GetSize())
		{
			RestoreTreeSelection(cache);
		}
		else
		{
			if (!htiSel)
				htiSel = htiFirst;
			
			if (htiSel)
				SelectItem(htiSel);
		}
	}
	else
	{
		m_bModified = FALSE;
	}

	// reset last taskedit
	m_dtLastTaskMod = COleDateTime::GetCurrentTime();

	// restore previously visibility
	if (bHidden)
		ShowWindow(SW_HIDE);
	else
		Resize();

	return TRUE;
}

int CToDoCtrl::GetArchivableTasks(CTaskFile& tasks, BOOL bSelectedOnly) const
{
	if (bSelectedOnly)
		return GetSelectedTasks(tasks, TDCGT_ALL, TDCGSTF_ALLPARENTS);

	// else
	return GetTasks(tasks, TDCGT_DONE);
}

void CToDoCtrl::RemoveArchivedTasks(const CTaskFile& tasks, TDC_ARCHIVE nRemove, BOOL bRemoveFlagged)
{
	if (IsReadOnly() || nRemove == TDC_REMOVENONE || !tasks.GetTaskCount())
		return; // nothing to do

	CPreferences prefs;
	SaveTasksState(prefs);
	
	IMPLEMENT_UNDO(TDCUAT_ARCHIVE);
	
	// clear selection
	TSH().RemoveAll();
	TCH().SelectItem(NULL);
	
	CHoldRedraw hr(m_taskTree);
	HTASKITEM hTask = tasks.GetFirstTask();
	BOOL bRemoved = FALSE;

	while (hTask)
	{
		bRemoved |= RemoveArchivedTask(tasks, hTask, nRemove, bRemoveFlagged);
		hTask = tasks.GetNextTask(hTask);
	}

	if (bRemoved)
		SetModified(TRUE, TDCA_ARCHIVE, 0);
}

BOOL CToDoCtrl::RemoveArchivedTask(const CTaskFile& tasks, HTASKITEM hTask, TDC_ARCHIVE nRemove, BOOL bRemoveFlagged)
{
	DWORD dwTaskID = tasks.GetTaskID(hTask);
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;

	if (!m_data.GetTask(dwTaskID, pTDI, pTDS))
	{
		ASSERT(0);
		return FALSE;
	}

	BOOL bRemove = FALSE;
	BOOL bDone = m_data.IsTaskDone(pTDI, pTDS, TDCCHECKCHILDREN);

	// filter on completed tasks (and flagged tasks if requested)
	if (bDone)
	{
		// Note: it's not enough just to check this task's flagged state 
		// we need to do a deep search for flagged children and grandchildren, etc
		// otherwise they'll get removed when if we remove this task.
		if (!bRemoveFlagged)
		{
			CResultArray aResults;
			SEARCHPARAMS params;
			params.aRules.Add(SEARCHPARAM(TDCA_FLAG, FOP_SET));

			bRemove = !m_data.FindTasks(pTDI, pTDS, params, aResults);
		}
		else
			bRemove = TRUE;

		if (bRemove)
		{
			bRemove = FALSE; // reset

			switch (nRemove)
			{
			case TDC_REMOVEALL:
				bRemove = TRUE;
				break;
				
			case TDC_REMOVEIFSIBLINGSANDSUBTASKSCOMPLETE:
				{
					TODOSTRUCTURE* pTDSParent = pTDS->GetParentTask();

					if (pTDSParent->IsRoot() || !m_data.TaskHasIncompleteSubtasks(pTDSParent, FALSE))
						bRemove = TRUE;
				}
				break;
				
			default:
				ASSERT (0);
			}
			
			if (bRemove && !RemoveArchivedTask(dwTaskID))
				return FALSE;
		}
	}
	
	// process children if not deleted
	if (!bRemove)
	{
		HTASKITEM hChildTask = tasks.GetFirstTask(hTask);
		
		while (hChildTask)
		{
			bRemove |= RemoveArchivedTask(tasks, hChildTask, nRemove, bRemoveFlagged);
			hChildTask = tasks.GetNextTask(hChildTask);
		}
	}

	return bRemove;
}

BOOL CToDoCtrl::RemoveArchivedTask(DWORD dwTaskID)
{
	HTREEITEM hti = m_taskTree.GetItem(dwTaskID);
	ASSERT(hti);
	
	if (!hti)
		return FALSE;
	
	m_taskTree.DeleteItem(hti);
	m_data.DeleteTask(dwTaskID);

	return TRUE;
}

BOOL CToDoCtrl::ArchiveDoneTasks(TDC_ARCHIVE nRemove, BOOL bRemoveFlagged)
{
	CString sArchivePath;
	
	if (!GetTaskCount() || !GetArchivePath(sArchivePath))
		return FALSE;
	
	// first see if there's anything to archive
	CTaskFile xiDone;
	
	if (!GetArchivableTasks(xiDone, FALSE))
		return FALSE;

	if (!ArchiveTasks(sArchivePath, xiDone))
		return FALSE;

	// remove archived tasks from current list
	TDCSELECTIONCACHE cache;
	CacheTreeSelection(cache);
	
	RemoveArchivedTasks(xiDone, nRemove, bRemoveFlagged);

	RestoreTreeSelection(cache);

	return TRUE;
}

BOOL CToDoCtrl::ArchiveSelectedTasks(BOOL bRemove)
{
	CString sArchivePath;

	if (!GetTaskCount() || !GetArchivePath(sArchivePath))
		return FALSE;
	
	// first see if there's anything to archive
	CTaskFile xiSel;
	
	if (!GetArchivableTasks(xiSel, TRUE))
		return FALSE;
	
	if (!ArchiveTasks(sArchivePath, xiSel))
		return FALSE;

	// delete the selected tasks
	if (bRemove)
	{
		TDCSELECTIONCACHE cache;
		CacheTreeSelection(cache);
		
		BOOL bDeleted = DeleteSelectedTask();

		RestoreTreeSelection(cache);

		return bDeleted;
	}

	// else
	return TRUE;
}

BOOL CToDoCtrl::ArchiveTasks(const CString& sArchivePath, const CTaskFile& tasks)
{
	// can't archive archives
	ASSERT (!m_bArchive);
	
	// must be something to archive
	ASSERT(tasks.GetTaskCount());

	// Time how long it takes
	DWORD dwStartTick = GetTickCount();
	
	// Load existing archive if present
	CTaskFile file(m_sPassword);
	TASKFILE_HEADER tfh;
	
	if (file.Load(sArchivePath))
	{
		VERIFY(file.IsArchive());

		// cache header for writing back
		file.GetHeader(tfh);
		tfh.dwNextID = 0; // will be set by taskfile

		if (!XMLHeaderIsUnicode(tfh.sXmlHeader))
			tfh.sXmlHeader = m_sXmlHeader;

		// increment file version if not 3rd party source controlled
		if (!HasStyle(TDCS_USES3RDPARTYSOURCECONTROL))
		{
			int nFileVer = file.GetFileVersion() + 1;
			tfh.nFileVersion = max(nFileVer, 1);
		}
	}
	else // or initialize first time
	{
		tfh.bArchive = TRUE;
		tfh.nFileFormat = FILEFORMAT_CURRENT;
		tfh.sProjectName.Format(_T("%s (%s)"), m_sProjectName, CEnString(IDS_TDC_ARCHIVEPROJECT));
		tfh.nFileVersion = 1;
		tfh.sXmlHeader = m_sXmlHeader;
		tfh.sXslHeader = m_sXslHeader;
	}
	
	file.Merge(tasks, TRUE, TRUE);
	file.Encrypt();
	file.SetHeader(tfh);

	if (!file.Save(sArchivePath, SFEF_UTF16))
		return FALSE;

	// If the process took longer than 1 second we rename
	// the archive so that next time it'll start afresh
	if ((GetTickCount() - dwStartTick) > 1000)
	{
		int nSuffix = 1;
		CString sNewPath;

		while (true)
		{
			sNewPath = sArchivePath;
			VERIFY(FileMisc::AddToFileName(sNewPath, nSuffix++));

			if (!FileMisc::FileExists(sNewPath))
				break;

			// else keep trying
		} 
		ASSERT(!sNewPath.IsEmpty() && !FileMisc::FileExists(sNewPath));
		
		FileMisc::MoveFile(sArchivePath, sNewPath);
	}

	return TRUE;
}

HTREEITEM CToDoCtrl::AddTaskToTreeItem(const CTaskFile& file, HTASKITEM hTask, HTREEITEM htiParent, HTREEITEM htiAfter, TDC_RESETIDS nResetID)
{
	HTREEITEM hti = TVI_ROOT; // default for root item
	TODOITEM* pTDI = NULL;
	
	if (hTask)
	{
		// fixup htiAfter
		if (htiAfter == TVI_LAST)
			htiAfter = TCH().GetLastChildItem(htiParent);

		// add new task to map
		DWORD dwTaskID = file.GetTaskID(hTask);

		// make sure that m_dwNextUniqueID exceeds this ID
		if (dwTaskID && nResetID == TDCR_NO)
		{
			ASSERT (m_taskTree.GetItem(dwTaskID) == NULL); // sanity check
			m_dwNextUniqueID = max(m_dwNextUniqueID, dwTaskID + 1);
		}
		else if (dwTaskID && nResetID == TDCR_CHECK)
		{
			// see if it already exist
			if (HasTask(dwTaskID))
			{
				// provide a new unique ID
				dwTaskID = m_dwNextUniqueID++; 
			}
			else 
			{
				// make sure that m_dwNextUniqueID exceeds this ID
				m_dwNextUniqueID = max(m_dwNextUniqueID, dwTaskID + 1);
			}
		}
		else 
		{
			// always provide a new unique ID
			dwTaskID = m_dwNextUniqueID++;
		}
		
		// add items to map
		DWORD dwParentID = GetTaskID(htiParent);
		DWORD dwPrevTaskID = GetTaskID(htiAfter);
		
		pTDI = m_data.NewTask(file, hTask);
		ASSERT(pTDI);

		if (!pTDI)
			return NULL;
		
		// add this item to data structure
		m_data.AddTask(dwTaskID, pTDI, dwParentID, dwPrevTaskID);
		
		// add this item to tree
		hti = InsertTreeItem(pTDI, dwTaskID, htiParent, htiAfter);
	}
	
	// then children
	HTREEITEM htiFirstItem = (hti == TVI_ROOT) ? NULL : hti;
	HTASKITEM hChildTask = file.GetFirstTask(hTask);
	
	while (hChildTask)
	{
		HTREEITEM htiChild = AddTaskToTreeItem(file, hChildTask, hti, TVI_LAST, nResetID);
		
		if (!htiFirstItem)
			htiFirstItem = htiChild;
		
		hChildTask = file.GetNextTask(hChildTask); // next
	}
		
	return htiFirstItem;
}

HTREEITEM CToDoCtrl::InsertTreeItem(const TODOITEM* pTDI, DWORD dwTaskID, HTREEITEM htiParent, HTREEITEM htiAfter)
{
	HTREEITEM hti = m_taskTree.InsertItem(dwTaskID,	htiParent, htiAfter);
	ASSERT(hti);

	// add unique items to comboboxes
	m_cbAllocTo.AddUniqueItems(pTDI->aAllocTo);
	m_cbAllocBy.AddUniqueItem(pTDI->sAllocBy);
	m_cbStatus.AddUniqueItem(pTDI->sStatus);
	m_cbCategory.AddUniqueItems(pTDI->aCategories);
	m_cbTags.AddUniqueItems(pTDI->aTags);
	m_cbVersion.AddUniqueItem(pTDI->sVersion);

	return hti;
}

void CToDoCtrl::OnTreeChangeFocus(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	TSH().InvalidateAll(FALSE);
	*pResult = 0;
}

void CToDoCtrl::OnDestroy() 
{
	EndTimeTracking(FALSE, FALSE);
	
	// save state
	if (!m_bDelayLoaded)
	{
		CPreferences prefs;
		
		SaveTasksState(prefs);
		SaveSplitPos(prefs);
		SaveAttributeVisibility(prefs);
		SaveDefaultRecurrence(prefs);

		CString sKey = GetPreferencesKey(); // no subkey
		m_ctrlComments.SavePreferences(prefs, sKey);
	}
	
	// clean up custom controls
	CTDCCustomAttributeHelper::CleanupCustomAttributeUI(m_aCustomControls, this);
	
	CRuntimeDlg::OnDestroy();
}

void CToDoCtrl::SetModified(BOOL bMod)
{
	if (!IsReadOnly())
	{
		m_bModified = bMod;

		if (bMod)
		{
			m_dtLastTaskMod = COleDateTime::GetCurrentTime();
		}
		else if (m_nCommentsState == CS_PENDING)
		{
			m_nCommentsState = CS_CHANGED;
		}
	}
}

BOOL CToDoCtrl::IsModified() const 
{ 
	if (IsReadOnly())
		return FALSE;

	if (m_bModified)
		return TRUE;
	
	// else do we have outstanding unhandled comments
	return (m_nCommentsState == CS_PENDING);
}

// internal version
void CToDoCtrl::SetModified(BOOL bMod, TDC_ATTRIBUTE nAttrib, DWORD /*dwModTaskID*/)
{
	if (IsReadOnly())
		return;
	
	SetModified(bMod);
	
	if (bMod)
	{
		m_taskTree.SetModified(nAttrib);

		if (ModNeedsResort(nAttrib))
		{
			// if the mod was a task completion and the parent completed state 
			// is based on this then we need to resort the entire tree 
			// likewise for start dates and due dates
			if ((nAttrib == TDCA_DONEDATE && HasStyle(TDCS_TREATSUBCOMPLETEDASDONE)) ||
				(nAttrib == TDCA_DUEDATE && (HasStyle(TDCS_USEEARLIESTDUEDATE) || HasStyle(TDCS_USELATESTDUEDATE))) ||
				(nAttrib == TDCA_STARTDATE && (HasStyle(TDCS_USEEARLIESTSTARTDATE) || HasStyle(TDCS_USELATESTSTARTDATE))))
			{
				Resort();
			}
			else // attributes that only have a local effect
			{
				ResortSelectedTaskParents();
			}
		}

		GetParent()->SendMessage(WM_TDCN_MODIFY, (WPARAM)GetSafeHwnd(), (LPARAM)nAttrib);

		// special case: if this was the project name being edited make sure
		// the focus is set back to the name
		if (nAttrib == TDCA_PROJNAME)
			GetDlgItem(IDC_PROJECTNAME)->SetFocus();
	}
}

BOOL CToDoCtrl::ModNeedsResort(TDC_ATTRIBUTE nModType) const
{
	return m_taskTree.ModNeedsResort(nModType);
}

LRESULT CToDoCtrl::OnCommentsChange(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if (!m_ctrlComments.IsSettingContent())
		UpdateTask(TDCA_COMMENTS);

	return 0L;
}

LRESULT CToDoCtrl::OnCommentsKillFocus(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	HandleUnsavedComments();

	return 0L;
}

LRESULT CToDoCtrl::OnCommentsDoHelp(WPARAM /*wParam*/, LPARAM lParam)
{
	AfxGetApp()->WinHelp(lParam);

	return 0L;
}

BOOL CToDoCtrl::OnHelpInfo(HELPINFO* /*lpHelpInfo*/)
{
	AfxGetApp()->WinHelp(IDD_TODOCTRL_DIALOG);
	return TRUE;
}

void CToDoCtrl::OnChangeProjectName()
{
	UpdateData();
	SetModified(TRUE, TDCA_PROJNAME);
}

LRESULT CToDoCtrl::OnTreeDragOver(WPARAM /*wParam*/, LPARAM lParam)
{
	const MSG* pMsg = GetCurrentMessage();
	UINT nRes = m_treeDragDrop.ProcessMessage(pMsg);
	
	// handle WM_DD_DRAGOVER for creating task shortcuts
	if (nRes != DD_DROPEFFECT_NONE)
	{
		const DRAGDROPINFO* pDDI = (DRAGDROPINFO*)lParam;

		// only interested in left button drags with ctrl+shift pressed
		if (pDDI->bLeftDrag)
		{
			HTREEITEM htiOver = m_taskTree.HitTestItem(pDDI->pt);
			DWORD dwOverTaskID = GetTaskID(htiOver);

			if (m_data.IsTaskReference(dwOverTaskID) && m_taskTree.SelectionHasNonReferences())
			{
				nRes = DD_DROPEFFECT_LINK;
			}
			else if (Misc::ModKeysArePressed(MKS_CTRL | MKS_SHIFT) ||
					Misc::ModKeysArePressed(MKS_ALT)) 
			{
				nRes = DD_DROPEFFECT_LINK;
			}
		}
	}

	return nRes;
}

LRESULT CToDoCtrl::OnTreeDragDrop(WPARAM /*wParam*/, LPARAM lParam)
{
	const MSG* pMsg = GetCurrentMessage();
	UINT nRes = m_treeDragDrop.ProcessMessage(pMsg);
	
	// handle WM_DD_DRAGDROP
	if (nRes)
	{
		const DRAGDROPINFO* pDDI = (DRAGDROPINFO*)lParam;
		HTREEITEM htiDrop, htiAfter;
		
		if (pMsg->message == WM_DD_DRAGDROP)
		{
			if (!m_treeDragDrop.GetDropTarget(htiDrop, htiAfter))
				return 0;
			
			DWORD dwTargetID = GetTaskID(htiDrop);
			BOOL bTargetIsRef = m_data.IsTaskReference(dwTargetID);
			
			// make target ID point to 'true' task
			if (bTargetIsRef)
				dwTargetID = m_data.GetTrueTaskID(dwTargetID);
			
			BOOL bCopy = FALSE;
			BOOL bMove = FALSE;
			BOOL bRef = FALSE;
			
			m_taskTree.RedrawColumns();
			
			// if htiAfter is NULL then we are dropping 'on' an item
			// so we need to decide where
			BOOL bDropOn = (htiAfter == NULL);
			
			if (bDropOn)
				htiAfter = m_bDragDropSubtasksAtTop ? TVI_FIRST : TVI_LAST;
			
			if (pDDI->bLeftDrag) 
			{
				// Only allow non-refs to be dropped on references
				// as references
				if (bTargetIsRef && m_taskTree.SelectionHasNonReferences())
				{
					bRef = TRUE;
				}
				else
				{
					bMove  = Misc::ModKeysArePressed(MKS_NONE);
					bCopy = Misc::ModKeysArePressed(MKS_CTRL);
					bRef = (Misc::ModKeysArePressed(MKS_CTRL | MKS_SHIFT) ||
							Misc::ModKeysArePressed(MKS_ALT));
				}
			}
			else // right drag
			{
				CEnMenu menu;
				
				if (menu.LoadMenu(IDR_TREEDRAGDROP, *this, TRUE))
				{
					if (htiDrop && htiDrop != TVI_ROOT)
						m_taskTree.SelectDropTarget(htiDrop);
					
					CMenu* pSubMenu = menu.GetSubMenu(0);
					CPoint ptCursor(pMsg->pt);
					
					// disable task ref, dependency and file links
					// if dropping in-between tasks
					UINT nDisabled = (MF_BYCOMMAND | MF_GRAYED);
					
					if (!bDropOn)
					{
						pSubMenu->EnableMenuItem(ID_TDD_SETTASKDEPENDENCY, nDisabled);
						pSubMenu->EnableMenuItem(ID_TDD_ADDTASKDEPENDENCY, nDisabled);
						pSubMenu->EnableMenuItem(ID_TDD_SETFILELINK, nDisabled);
						pSubMenu->EnableMenuItem(ID_TDD_REFTASK, nDisabled);
					}
					// if the target is a reference disable copying and moving
					else if (bTargetIsRef) 
					{
						pSubMenu->EnableMenuItem(ID_TDD_COPYTASK, nDisabled);
						pSubMenu->EnableMenuItem(ID_TDD_MOVETASK, nDisabled);
						
						// cannot create dependency to self
						if (TSH().HasItem(dwTargetID))
						{
							pSubMenu->EnableMenuItem(ID_TDD_SETTASKDEPENDENCY, nDisabled);
							pSubMenu->EnableMenuItem(ID_TDD_ADDTASKDEPENDENCY, nDisabled);
						}
					}
					
					UINT nCmdID = ::TrackPopupMenu(*pSubMenu, TPM_RETURNCMD | TPM_LEFTALIGN | TPM_LEFTBUTTON, 
													ptCursor.x, ptCursor.y, 0, *this, NULL);

					switch (nCmdID)
					{
					case ID_TDD_REFTASK:
						bRef = TRUE;
						break;
						
					case ID_TDD_COPYTASK:
						bCopy = TRUE;
						break;
						
					case ID_TDD_MOVETASK:
						bMove = TRUE;
						break;
						
					case ID_TDD_SETTASKDEPENDENCY:
					case ID_TDD_ADDTASKDEPENDENCY:
						{
							IMPLEMENT_UNDOEDIT();

							// replace task dependencies with this one
							CStringArray aDepends;
							aDepends.Add(TODOITEM::FormatTaskDependency(GetSelectedTaskID()));

							m_data.SetTaskDependencies(dwTargetID, aDepends, (nCmdID == ID_TDD_ADDTASKDEPENDENCY));
							SetModified(TRUE, TDCA_DEPENDENCY, dwTargetID);
						}
						break;
						
					case ID_TDD_SETFILELINK:
						{
							IMPLEMENT_UNDOEDIT();

							CString sTaskRef;
							sTaskRef.Format(_T("tdl://%lu"), GetSelectedTaskID());

							CStringArray aFileRefs;
							aFileRefs.Add(sTaskRef);

							m_data.SetTaskFileRefs(dwTargetID, aFileRefs, TRUE);

							SetModified(TRUE, TDCA_FILEREF, dwTargetID);
						}
						break;
						
					default:
						break;
					}

					m_taskTree.SelectDropTarget(NULL);
					m_taskTree.RedrawColumns();
				}
			}
			
			if (bCopy || bRef)
			{
				// copy selection as xml
				CTaskFile tasks;
				TDCGETTASKS filter;
				
				// if pasting references then we don't want all subtasks just 
				// the ones actually selected
				if (GetSelectedTasks(tasks, filter, (bRef ? TDCGSTF_NOTSUBTASKS : 0)))
				{
					IMPLEMENT_UNDO(TDCUAT_COPY);
					HOLD_REDRAW(*this, m_taskTree);

					// fix up the dependencies of the copied tasks
					if (bCopy)
						PrepareTasksForPaste(tasks, TDCR_YES);
					else
						PrepareTaskIDsForPasteAsRef(tasks);
					
					// then add them with impunity!
					AddTasksToTree(tasks, htiDrop, htiAfter, TDCR_NO, TRUE, TDCA_PASTE);
					
					// if the parent was marked as done and this task is not
					// then mark the parent as incomplete too
					m_data.FixupParentCompletion(dwTargetID);
				}
			}
			else if (bMove)
			{
				IMPLEMENT_UNDO(TDCUAT_MOVE);
				
				DWORD dwDestParentID = m_taskTree.GetTaskID(htiDrop);
				DWORD dwDestPrevSiblingID = m_taskTree.GetTaskID(htiAfter);

				CDWordArray aSelTaskIDs;
				DWORD dwUnused = 0;
				
				m_taskTree.GetSelectedTaskIDs(aSelTaskIDs, dwUnused, TRUE);
				
				if (m_data.MoveTasks(aSelTaskIDs, dwDestParentID, dwDestPrevSiblingID))
				{
					m_taskTree.MoveSelection(htiDrop, htiAfter);
				
					// if the parent was marked as done and this task is not
					// then mark the parent as incomplete too
					m_data.FixupParentCompletion(dwDestParentID);

					SetModified(TRUE, TDCA_POSITION);
				}
			}
		}
	}

	return nRes;
}

void CToDoCtrl::RemoveNonSelectedTasks(CTaskFile& tasks, HTASKITEM hTask) const
{
	if (hTask)
	{
		LPCTSTR szSelected = tasks.GetTaskMetaData(hTask, _T("selected"));

		if (!szSelected || (szSelected[0] != '1'))
		{
			tasks.DeleteTask(hTask);
			return;
		}
	}

	// else test children
	hTask = tasks.GetFirstTask(hTask);

	while (hTask)
	{
		// get next child first in case hTask is deleted
		HTASKITEM hTaskNext = tasks.GetNextTask(hTask);
		
		RemoveNonSelectedTasks(tasks, hTask);

		hTask = hTaskNext;
	}
}

void CToDoCtrl::PrepareTasksForPaste(CTaskFile& tasks, TDC_RESETIDS nResetID) const
{
	if (nResetID == TDCR_NO || (tasks.GetTaskCount() == 0))
		return; // nothing to do

	// work out what new IDs we need and map them to the existing IDs
	CMapID2ID mapID;
	mapID.InitHashTable(tasks.GetTaskCount());

	DWORD dwNextID(max(tasks.GetNextUniqueID(), m_dwNextUniqueID));
	BuildTaskIDMapForPaste(tasks, tasks.GetFirstTask(NULL), dwNextID, mapID, nResetID, TRUE);

	// then fix them
	PrepareTasksForPaste(tasks, tasks.GetFirstTask(NULL), mapID, TRUE);
}

void CToDoCtrl::PrepareTaskIDsForPasteAsRef(CTaskFile& tasks) const
{
	if (tasks.GetTaskCount() == 0)
		return; // nothing to do

	CMapID2ID mapID;
	mapID.InitHashTable(tasks.GetTaskCount());

	// work out what new IDs we need and map them to the existing IDs
	DWORD dwNextID(max(tasks.GetNextUniqueID(), m_dwNextUniqueID));
	BuildTaskIDMapForPaste(tasks, tasks.GetFirstTask(NULL), dwNextID, mapID, TDCR_YES, TRUE);

	// work thru the 'new' tasks adding internal ref back to original
	POSITION pos = mapID.GetStartPosition();

	while (pos)
	{
		DWORD dwOrg, dwRef;
		mapID.GetNextAssoc(pos, dwOrg, dwRef);

		HTASKITEM hTask = tasks.FindTask(dwOrg);

		if (hTask)
		{
			// replace org ID with ref ID
			tasks.SetTaskID(hTask, dwRef);

			// don't create references to references
			if (tasks.GetTaskReferenceID(hTask) == 0)
			{
				// link ref task back to org
				tasks.SetTaskReferenceID(hTask, dwOrg);
			}
		}
	}
}

void CToDoCtrl::BuildTaskIDMapForPaste(CTaskFile& tasks, HTASKITEM hTask, DWORD& dwNextID, 
										CMapID2ID& mapID, TDC_RESETIDS nResetID, BOOL bAndSiblings) const
{
	if (nResetID == TDCR_NO) // sanity check
		return;

	if (!hTask)
		return;

	// we map this task only if it needs a new ID
	DWORD dwTaskID = tasks.GetTaskID(hTask);

	if ((dwTaskID <= 0) || (nResetID == TDCR_YES) || HasTask(dwTaskID))
		mapID[dwTaskID] = dwNextID++;

	// children
	BuildTaskIDMapForPaste(tasks, tasks.GetFirstTask(hTask), dwNextID, mapID, nResetID, TRUE);

	// handle sibling tasks WITHOUT RECURSION
	if (bAndSiblings)
	{
		hTask = tasks.GetNextTask(hTask);
		
		while (hTask)
		{
			// FALSE == don't recurse on siblings
			BuildTaskIDMapForPaste(tasks, hTask, dwNextID, mapID, nResetID, FALSE);
			
			hTask = tasks.GetNextTask(hTask);
		}
	}
}

void CToDoCtrl::PrepareTasksForPaste(CTaskFile& tasks, HTASKITEM hTask, const CMapID2ID& mapID, BOOL bAndSiblings) const
{
	if (!hTask)
		return;

	// change task's own ID
	DWORD dwTaskID = tasks.GetTaskID(hTask), dwNewID = 0;

	if (mapID.Lookup(dwTaskID, dwNewID))
	{
		ASSERT (dwNewID); // sanity check
		tasks.SetTaskID(hTask, dwNewID);

		// And give it a new creation date and creator
		tasks.SetTaskCreationDate(hTask, COleDateTime::GetCurrentTime());
		tasks.SetTaskCreatedBy(hTask, m_tdiDefault.sCreatedBy);
	}

	// dependencies first
	CStringArray aDepends;
	int nDepend = tasks.GetTaskDependencies(hTask, aDepends);

	BOOL bChanged = FALSE;

	while (nDepend--)
	{
		CString sDepends = aDepends[nDepend];

		if (PrepareTaskLinkForPaste(sDepends, mapID))
		{
			aDepends[nDepend] = sDepends;
			bChanged = TRUE;
		}
	}

	// update taskfile if any dependency was changed
	if (bChanged)
		tasks.SetTaskDependencies(hTask, aDepends);
	
	// file references
	CStringArray aFileRefs;
	int nFile = tasks.GetTaskFileLinks(hTask, aFileRefs);

	bChanged = FALSE;

	while (nFile--)
	{
		CString sFileRef = aFileRefs[nFile];

		if (PrepareTaskLinkForPaste(sFileRef, mapID))
		{
			aFileRefs[nFile] = sFileRef;
			bChanged = TRUE;
		}
	}

	// update taskfile if any file link was changed
	if (bChanged)
		tasks.SetTaskFileLinks(hTask, aFileRefs);

	// children
	PrepareTasksForPaste(tasks, tasks.GetFirstTask(hTask), mapID, TRUE);

	// siblings
	// handle sibling tasks WITHOUT RECURSION
	if (bAndSiblings)
	{
		hTask = tasks.GetNextTask(hTask);
		
		while (hTask)
		{
			// FALSE == don't recurse on siblings
			PrepareTasksForPaste(tasks, hTask, mapID, FALSE);
			
			hTask = tasks.GetNextTask(hTask);
		}
	}
}

BOOL CToDoCtrl::PrepareTaskLinkForPaste(CString& sLink, const CMapID2ID& mapID) const
{
	DWORD dwTaskID;
	CString sFile, sFilePrefix;

	if (sLink.Find(TDL_PROTOCOL) >= 0)
		sFilePrefix = TDL_PROTOCOL;

	ParseTaskLink(sLink, dwTaskID, sFile);
	
	if (!dwTaskID)
		return FALSE;

	// does the ID need changing
	DWORD dwNewID = 0;
	
	if (mapID.Lookup(dwTaskID, dwNewID))
	{
		ASSERT (dwNewID); // sanity check
		
		// make sure the file path matches us
		if (!sFile.IsEmpty() && !m_sLastSavePath.IsEmpty())
		{
			if (!FileMisc::IsSamePath(sFile, m_sLastSavePath))
				return FALSE;
		}
		
		// update dependency
		sLink = (sFilePrefix + TODOITEM::FormatTaskDependency(dwNewID, sFile));
		return TRUE;
	}

	return FALSE;

}

BOOL CToDoCtrl::PreTranslateMessage(MSG* pMsg) 
{
	BOOL bCtrl = Misc::IsKeyPressed(VK_CONTROL);
	BOOL bShift = Misc::IsKeyPressed(VK_SHIFT);
	BOOL bAlt = Misc::IsKeyPressed(VK_MENU);

	if (m_ctrlComments.PreTranslateMessage(pMsg))
		return TRUE;

	if (m_treeDragDrop.ProcessMessage(pMsg))
		return TRUE;
	
	if (m_taskTree.PreTranslateMessage(pMsg))
		return TRUE;
	
	switch (pMsg->message)
	{
	case  WM_CHAR:
		// eat any enter keys destined for non-multiline
		// edits else they'll beep
		if (pMsg->wParam == VK_RETURN && CWinClasses::IsEditControl(pMsg->hwnd))
		{
			DWORD dwStyle = ::GetWindowLong(pMsg->hwnd, GWL_STYLE);

			if (!(dwStyle & ES_MULTILINE))
				return TRUE;
		}
		break;

	case WM_KEYDOWN:
		{
			// handle <return> and <escape> for tree label editing
			if (pMsg->hwnd == (HWND)m_taskTree.Tree().SendMessage(TVM_GETEDITCONTROL))
			{
				// this should no longer need handling
				ASSERT(0);
			}
			// handle <enter> and TDCS_FOCUSTREEONENTER
			else if (HasStyle(TDCS_FOCUSTREEONENTER) && (pMsg->wParam == VK_RETURN))
			{
				// have to be careful here because dropped-down comboboxes should
				// take precedence. so we have to check explicitly for them
				// and make sure they're not in a dropped state
				CWnd* pFocus = GetFocus();
				
				if (pFocus && IsChild(pFocus))
				{
					if (!CtrlWantsEnter(*pFocus))
						SetFocusToTasks();

					return FALSE; // allow further routing
				}
			}
		}
		break;
	}
	
	return CRuntimeDlg::PreTranslateMessage(pMsg);
}

BOOL CToDoCtrl::MoveSelectedTask(TDC_MOVETASK nDirection)
{
	if (!CanMoveSelectedTask(nDirection))
		return FALSE;
	
	// else
	Flush(); // end any editing action
	SetFocusToTasks(); // else datetime controls get their focus screwed

	// do the move
	VERIFY(MoveSelection(nDirection));

	SetModified(TRUE, TDCA_POSITION, 0);

	return TRUE;
}

BOOL CToDoCtrl::MoveSelection(TDC_MOVETASK nDirection)
{
	if (CanMoveSelectedTask(nDirection))
	{
		// move the tasks
		IMPLEMENT_UNDO(TDCUAT_MOVE);

		DWORD dwDestParentID = 0, dwDestPrevSiblingID = 0;
		VERIFY(m_taskTree.GetInsertLocation(nDirection, dwDestParentID, dwDestPrevSiblingID));
				
		CDWordArray aSelTaskIDs;
		m_taskTree.GetSelectedTaskIDs(aSelTaskIDs);
		
		if (m_data.MoveTasks(aSelTaskIDs, dwDestParentID, dwDestPrevSiblingID))
		{
			m_taskTree.MoveSelection(nDirection);
				
			// refresh parent states if moving to the right (adding subtasks)
			if (nDirection == TDCM_RIGHT)
			{
				m_data.FixupParentCompletion(dwDestParentID);
			}

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CToDoCtrl::GotoNextTopLevelTask(TDC_GOTO nDirection)
{
	HTREEITEM htiNext = TCH().GetNextTopLevelItem(GetSelectedItem(), nDirection == TDCG_NEXT);
	
	if (htiNext)
		SelectItem(htiNext);

	return (htiNext != NULL);
}

BOOL CToDoCtrl::CanGotoNextTopLevelTask(TDC_GOTO nDirection) const
{
	return (NULL != TCH().GetNextTopLevelItem(GetSelectedItem(), nDirection == TDCG_NEXT));
}

BOOL CToDoCtrl::GotoNextTask(TDC_GOTO nDirection)
{
	HTREEITEM htiNext = NULL;

	if (nDirection == TDCG_NEXT)
		htiNext = TCH().GetNextVisibleItem(GetSelectedItem());
	else
		htiNext = TCH().GetPrevVisibleItem(GetSelectedItem());
	
	if (htiNext)
		SelectItem(htiNext);

	return (htiNext != NULL);
}

BOOL CToDoCtrl::CanGotoNextTask(TDC_GOTO nDirection) const
{
	HTREEITEM htiNext = NULL;

	if (nDirection == TDCG_NEXT)
		htiNext = TCH().GetNextVisibleItem(GetSelectedItem());
	else
		htiNext = TCH().GetPrevVisibleItem(GetSelectedItem());

	return (htiNext != NULL);
}

void CToDoCtrl::SetDueTaskColors(COLORREF crDue, COLORREF crDueToday)
{
	m_taskTree.SetDueTaskColors(crDue, crDueToday);
}

void CToDoCtrl::SetPriorityColors(const CDWordArray& aColors) 
{ 
	if (m_taskTree.SetPriorityColors(aColors))
		m_cbPriority.SetColors(aColors);
}

void CToDoCtrl::SetProjectName(const CString& sProjectName)
{
	if (m_sProjectName != sProjectName)
	{
		m_sProjectName = sProjectName;

		if (GetSafeHwnd())
			UpdateDataEx(this, IDC_PROJECTNAME, m_sProjectName, FALSE);
	}
}

BOOL CToDoCtrl::GetArchivePath(CString& sArchivePath) const
{
	// can't archive archives
	if (!m_bArchive && !m_sLastSavePath.IsEmpty())
	{
		sArchivePath = m_sLastSavePath;
		return FileMisc::AddToFileName(sArchivePath, ARCHIVE_ID);
	}

	// else
	sArchivePath.Empty();
	return FALSE;
}

CString CToDoCtrl::GetFriendlyProjectName(int nUntitledIndex) const
{
	CString sProjectName(m_sProjectName);
	sProjectName.TrimRight();
	
	if (sProjectName.IsEmpty())
	{
		if (!m_sLastSavePath.IsEmpty())
		{
			sProjectName = FileMisc::GetFileNameFromPath(m_sLastSavePath, FALSE);
		}
		else
		{
			sProjectName = CEnString(IDS_TDC_UNTITLEDFILE);

			if (nUntitledIndex > 0)
				sProjectName += Misc::Format(nUntitledIndex);
		}
	}
	
	return sProjectName;
}

CString CToDoCtrl::GetStylesheetPath() const
{
	if (!m_sXslHeader.IsEmpty())
	{
		CStringArray aLinks, aText;

		if (WebMisc::ExtractHtmlLinks(m_sXslHeader, aLinks, aText))
		{
			// look for .xsl
			int nLink = aLinks.GetSize();

			while (nLink--)
			{
				CString sFile(aLinks[nLink]);

				if (FileMisc::GetExtension(Misc::ToUpper(sFile)) == _T(".XSL"))
				{
					if (!m_sLastSavePath.IsEmpty())
						FileMisc::MakeFullPath(sFile, FileMisc::GetFolderFromFilePath(m_sLastSavePath));

					return sFile;
				}
			}
		}
	}

	// not found
	return _T("");
}

void CToDoCtrl::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	Flush();

	CRuntimeDlg::OnContextMenu(pWnd, point);
}

TDC_HITTEST CToDoCtrl::HitTest(const CPoint& ptScreen) const
{
	return m_taskTree.HitTest(ptScreen);
}

TDC_COLUMN CToDoCtrl::ColumnHitTest(const CPoint& ptScreen) const
{
	return m_taskTree.HitTestColumn(ptScreen);
}

LRESULT CToDoCtrl::OnAutoComboAddDelete(WPARAM wp, LPARAM /*lp*/)
{
	UINT nCtrlID = LOWORD(wp);

	switch (nCtrlID)
	{
	case IDC_FILEPATH:
		{
			CStringArray aFileRefs;
			m_cbFileRef.GetFileList(aFileRefs);

			SetSelectedTaskFileRefs(aFileRefs, FALSE, TRUE);
		}
		// Note: parent doesn't need to know about this
		return 0L;
		
	case IDC_CATEGORY:
	case IDC_TAGS:
	case IDC_STATUS:
	case IDC_ALLOCTO:
	case IDC_ALLOCBY:
	case IDC_VERSION:
		{
			TDC_ATTRIBUTE nAttribID = TDC::MapCtrlIDToAttribute(nCtrlID);
			GetParent()->SendMessage(WM_TDCN_LISTCHANGE, 0, nAttribID);
		}
		break;

	default:
		if (CTDCCustomAttributeHelper::IsCustomControl(nCtrlID))
		{
			GetParent()->SendMessage(WM_TDCN_LISTCHANGE, 0, TDCA_CUSTOMATTRIB);
		}
		else
		{
			ASSERT(0);
			return 0L;
		}
	}

	// mark ourselves as modified because we now save the lists to the taskfile
	SetModified(TRUE);

	return 0L;
}

void CToDoCtrl::OnSelChangeAllocTo()
{
	UpdateTask(TDCA_ALLOCTO);
}

void CToDoCtrl::OnSelChangeAllocBy()
{
	UpdateTask(TDCA_ALLOCBY);
}

void CToDoCtrl::OnSelChangeStatus()
{
	UpdateTask(TDCA_STATUS);
}

void CToDoCtrl::OnSelChangeVersion()
{
	UpdateTask(TDCA_VERSION);
}

void CToDoCtrl::OnSelChangeCategory()
{
	UpdateTask(TDCA_CATEGORY);
}

void CToDoCtrl::OnSelChangeFileRefPath()
{
	// Special case: When we are read-only we still allow the user 
	// to change the selection of the file reference combo but
	// without modifying the underlying task
	if (!IsReadOnly())
		UpdateTask(TDCA_FILEREF);
}

void CToDoCtrl::OnCancelChangeFileRefPath()
{
	// restore combo state
	m_cbFileRef.SetFileList(m_aFileRefs);
}

void CToDoCtrl::OnSelChangeTag()
{
	UpdateTask(TDCA_TAGS);
}

void CToDoCtrl::OnSelCancelAllocTo()
{
	// reset selection
	if (GetSelectedCount() > 1)
	{
		m_cbAllocTo.SetWindowText(_T(""));
	}
	else
	{
		HTREEITEM hti = TSH().GetFirstItem();

		if (hti)
		{
			CStringArray aTo;
			m_data.GetTaskAllocTo(GetTaskID(hti), aTo);

			m_cbAllocTo.SetChecked(aTo);
		}
	}
}

void CToDoCtrl::OnSelCancelCategory()
{
	// reset selection
	if (GetSelectedCount() > 1)
	{
		m_cbCategory.SetWindowText(_T(""));
	}
	else
	{
		HTREEITEM hti = TSH().GetFirstItem();

		if (hti)
		{
			CStringArray aCats;
			m_data.GetTaskCategories(GetTaskID(hti), aCats);

			m_cbCategory.SetChecked(aCats);
		}
	}
}

void CToDoCtrl::OnSelCancelTag()
{
	// reset selection
	if (GetSelectedCount() > 1)
	{
		m_cbTags.SetWindowText(_T(""));
	}
	else
	{
		HTREEITEM hti = TSH().GetFirstItem();

		if (hti)
		{
			CStringArray aTags;
			m_data.GetTaskTags(GetTaskID(hti), aTags);

			m_cbTags.SetChecked(aTags);
		}
	}
}

void CToDoCtrl::OnChangeTimeEstimate()
{
	UpdateTask(TDCA_TIMEEST); 
}

void CToDoCtrl::OnChangeTimeSpent()
{
	// ignore this if it came from time tracking
	if (IsSelectedTaskBeingTimeTracked())
	{
		ASSERT(m_eTimeSpent.GetStyle() & ES_READONLY);
		ASSERT(GetSelectedCount() == 1);
		
		return;
	}

	UpdateTask(TDCA_TIMESPENT); 
}

void CToDoCtrl::OnChangeExternalID()
{
	UpdateTask(TDCA_EXTERNALID); 
}

void CToDoCtrl::OnChangeCost()
{
	UpdateTask(TDCA_COST); 
}

void CToDoCtrl::OnChangeDependency()
{
	UpdateTask(TDCA_DEPENDENCY); 
}

void CToDoCtrl::OnChangeRecurrence()
{
	UpdateTask(TDCA_RECURRENCE); 
}

void CToDoCtrl::OnChangePercent()
{
	// don't handle if this was an 'auto' change
	if (!HasStyle(TDCS_AUTOCALCPERCENTDONE))
		UpdateTask(TDCA_PERCENT);
}

BOOL CToDoCtrl::IsClipboardEmpty(BOOL bCheckID) const
{
	if (CTaskClipboard::IsEmpty())
	{
		return TRUE;
	}
	else if (bCheckID)
	{
		return !CTaskClipboard::ClipIDMatches(GetClipboardID());
	}

	//else
	return FALSE;
}

BOOL CToDoCtrl::CopyCurrentSelection() const
{
	if (!GetSelectedCount())
		return FALSE;
	
	ClearCopiedItem();

	// copy selected tasks to clipboard
	TDCGETTASKS filter(TDCGT_ALL, TDCGTF_FILENAME);
	CTaskFile tasks;

	if (!GetSelectedTasks(tasks, filter))
		return FALSE;
	
	// and their titles
	CHTIList selection;
	CStringArray aTitles;

	VERIFY(TSH().CopySelection(selection, FALSE, TRUE));
	VERIFY(TSH().GetItemTitles(selection, aTitles));

	return CTaskClipboard::SetTasks(tasks, GetClipboardID(), Misc::FormatArray(aTitles, '\n'));
}

BOOL CToDoCtrl::CopySelectedTask() const
{
	return CopyCurrentSelection();
}

BOOL CToDoCtrl::CutSelectedTask()
{
	if (CopyCurrentSelection())
	{
		IMPLEMENT_UNDO(TDCUAT_DELETE);

		DeleteSelectedTask(FALSE, TRUE);
		return TRUE;
	}
	
	return FALSE;
}

BOOL CToDoCtrl::CanPasteTasks(TDC_PASTE nWhere, BOOL bAsRef) const
{
	if (IsReadOnly() || IsClipboardEmpty())
		return FALSE;

	// cannot paste references from other tasklists
	if (bAsRef && IsClipboardEmpty(TRUE))
		return FALSE;

	switch (nWhere)
	{
	case TDCP_ONSELTASK:
		// there must be exactly one task selected
		if (GetSelectedCount() != 1)
		{
			return FALSE;
		}
		// can only paste non-references on other non-references
		else if (!bAsRef && IsSelectedTaskReference())
		{
			return FALSE;
		}
		break;
		
	case TDCP_BELOWSELTASK:
		if (!GetSelectedCount())
		{
			return FALSE;
		}
		else
		{
			// parent must be acceptable
			DWORD dwParentID = GetSelectedTaskParentID();

			if (GetSelectedCount() == 1)
			{
				if (dwParentID) // non-root
				{
					if (!bAsRef && m_data.IsTaskReference(dwParentID))
						return FALSE;
				}
				// else => root => okay
			}
			else if (dwParentID) // same parent
			{
				if (!bAsRef && m_data.IsTaskReference(dwParentID))
					return FALSE;
			}
			else // multiple parents
			{
				return FALSE;
			}
		}
		break;
		
	case TDCP_ATBOTTOM:
		break; // always
	}

	return TRUE;
}

void CToDoCtrl::ClearCopiedItem() const
{
	CTaskClipboard::Reset();
}

BOOL CToDoCtrl::PasteTasks(TDC_PASTE nWhere, BOOL bAsRef)
{
	if (!CanPasteTasks(nWhere, bAsRef))
	{
		ASSERT(0);
		return FALSE;
	}

	CString sClipID = GetClipboardID(), sArchiveID;
	BOOL bCheckArchive = GetClipboardID(sArchiveID, TRUE);

	CTaskFile tasks;

	if (CTaskClipboard::GetTasks(tasks, sClipID))
	{
		HTREEITEM htiDest = NULL, htiDestAfter = NULL;

		switch (nWhere)
		{
		case TDCP_ONSELTASK:
			htiDest = m_taskTree.GetTreeSelectedItem();
			htiDestAfter = TVI_FIRST;
			break;

		case TDCP_BELOWSELTASK:
			htiDestAfter = m_taskTree.GetTreeSelectedItem();
			
			if (!htiDestAfter)
				htiDestAfter = TVI_LAST;
			else
				htiDest = m_taskTree.GetParentItem(htiDestAfter);
			break;

		case TDCP_ATBOTTOM:
			htiDestAfter = TVI_LAST;
			break;
		}
			
		if (bAsRef)
		{
			// remove tasks not originally selected
			RemoveNonSelectedTasks(tasks, NULL);

			// pre-process the tasks to add themselves
			// as a reference, and then to clear the task ID
			// so that it gets a newly allocated one
			PrepareTaskIDsForPasteAsRef(tasks);
		}
		else
		{
			// pre-process task IDs if the tasks did *not* originate 
			// from us (or our archive) and we're not empty
			TDC_RESETIDS nResetID = TDCR_YES;
			
			if (CTaskClipboard::ClipIDMatches(sClipID) ||
				(bCheckArchive && CTaskClipboard::ClipIDMatches(sArchiveID)))
			{
				nResetID = TDCR_CHECK;
			}
			else if (GetTaskCount() == 0)
			{
				nResetID = TDCR_NO;
			}

			// and fix up the dependencies of the tasks
			PrepareTasksForPaste(tasks, nResetID);
		}

		IMPLEMENT_UNDO(TDCUAT_PASTE);
		HOLD_REDRAW(*this, m_taskTree);

		// no need to re-check IDs as we've already done it
		if (AddTasksToTree(tasks, htiDest, htiDestAfter, TDCR_NO, TRUE, TDCA_PASTE))
		{
			m_data.FixupParentCompletion(GetTaskID(htiDest));

			return TRUE;
		}
	}

   // else
   return FALSE;
}

BOOL CToDoCtrl::AddTasksToTree(const CTaskFile& tasks, HTREEITEM htiDest, HTREEITEM htiDestAfter, 
							   TDC_RESETIDS nResetIDs, BOOL bSelectAll, TDC_ATTRIBUTE nModAttrib)
{
	if (!htiDest)
		htiDest = TVI_ROOT;
	
	HTASKITEM hTask = tasks.GetFirstTask();
	
	if (!hTask)
		return FALSE;
	
	// cache and clear current selection
	DWORD dwSelID = GetTaskID(TSH().GetFirstItem());

	TCH().SelectItem(NULL);
	TSH().RemoveAll();

	CDWordArray aTaskIDs;

	while (hTask)
	{
		htiDestAfter = AddTaskToTreeItem(tasks, hTask, htiDest, htiDestAfter, nResetIDs);

		// cache items for selection
		if (bSelectAll)
			aTaskIDs.Add(GetTaskID(htiDestAfter));
		
		// next task
		hTask = tasks.GetNextTask(hTask);
	}
	
	m_taskTree.ExpandItem(htiDest, TRUE);
	
	// restore selection
	if (bSelectAll && aTaskIDs.GetSize())
	{
		m_taskTree.SelectTasks(aTaskIDs);
	}
	else if (dwSelID)
	{
		SelectTask(dwSelID);
	}
	else // select first task
	{
		SelectItem(m_taskTree.GetChildItem());
	}
	
	SetModified(TRUE, nModAttrib, 0);
	UpdateControls();
	
	return TRUE;
}

void CToDoCtrl::OnTreeClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
		
	// special case
	if (Misc::IsKeyPressed(VK_MENU)) // alt key
	{
		NMITEMACTIVATE* pNMIA = (NMITEMACTIVATE*)pNMHDR;

		TDC_COLUMN nColID = (TDC_COLUMN)pNMIA->iSubItem;
		UINT nCtrlID = MapColumnToCtrlID(nColID);
		
		if (nCtrlID)
		{
			// make sure the edit controls are visible
			if (m_nMaxState != TDCMS_NORMAL)
				SetMaximizeState(TDCMS_NORMAL);
			
			GetDlgItem(nCtrlID)->SetFocus();
		}
	}
}

LRESULT CToDoCtrl::OnColumnEditClick(WPARAM wParam, LPARAM lParam)
{
	TDC_COLUMN nColID = (TDC_COLUMN)wParam;
	DWORD dwTaskID = lParam;

	switch (nColID)
	{
	case TDCC_CLIENT:
		ASSERT(!IsReadOnly());
		EditSelectedTask(FALSE);
		break;
		
	case TDCC_DONE:
		ASSERT(!IsReadOnly());
		SetSelectedTaskDone(!m_data.IsTaskDone(dwTaskID));
		break;
		
	case TDCC_TRACKTIME:
		{
			HTREEITEM hti = m_taskTree.GetTreeSelectedItem();

			ASSERT (!IsReadOnly() && 
					(GetSelectedCount() == 1) && 
					IsItemSelected(hti) && 
					m_data.IsTaskTimeTrackable(dwTaskID));
			
			TimeTrackTask(hti);
		}
		break;
		
	case TDCC_FLAG:
		ASSERT(!IsReadOnly());
		SetSelectedTaskFlag(!m_data.IsTaskFlagged(dwTaskID));
		break;
		
	case TDCC_ICON:
		ASSERT(!IsReadOnly());
		EditSelectedTaskIcon();
		break;
		
	case TDCC_FILEREF:
		GotoFile(m_data.GetTaskFileRef(dwTaskID, 0));
		break;
		
	case TDCC_DEPENDENCY:
		ShowTaskLink(m_data.GetTaskDependency(dwTaskID, 0));
		break;
		
	case TDCC_RECURRENCE:
		ASSERT(!IsReadOnly());
		m_eRecurrence.DoEdit();
		break;
		
	case TDCC_REMINDER:
		AfxGetMainWnd()->SendMessage(WM_TDCN_CLICKREMINDERCOL);
		break;
		
	default: // try custom columns
		HandleCustomColumnClick(nColID);
		break;
	}

	m_treeDragDrop.EnableDragDrop(!IsReadOnly() && GetSelectedCount());

	return 0L;
}

UINT CToDoCtrl::MapColumnToCtrlID(TDC_COLUMN nColID) const
{
	TDC_ATTRIBUTE nAttrib = TDC::MapColumnToAttribute(nColID);
	ASSERT(nAttrib != TDCA_NONE);

	int nCtrl;
	for (nCtrl = 0; nCtrl < NUM_CTRLITEMS; nCtrl++)
	{
		const CTRLITEM& ctrl = CTRLITEMS[nCtrl];

		if (ctrl.nAttrib == nAttrib)
			return ctrl.nCtrlID;
	}

	for (nCtrl = 0; nCtrl < m_aCustomControls.GetSize(); nCtrl++)
	{
		const CTRLITEM& ctrl = m_aCustomControls[nCtrl];

		if (ctrl.nAttrib == nAttrib)
			return ctrl.nCtrlID;
	}

	// pick up any stragglers
	switch (nColID)
	{
	case TDCC_DONE:
		return IDC_DONEDATE;

	case TDCC_TRACKTIME:
		return IDC_TIMESPENT;
	}

	ASSERT(0);
	return 0L;
}

TDC_COLUMN CToDoCtrl::MapCtrlIDToColumn(UINT nCtrlID) const
{
	int nCtrl;
	for (nCtrl = 0; nCtrl < NUM_CTRLITEMS; nCtrl++)
	{
		const CTRLITEM& ctrl = CTRLITEMS[nCtrl];

		if (ctrl.nCtrlID == nCtrlID)
			return TDC::MapAttributeToColumn(ctrl.nAttrib);
	}

	for (nCtrl = 0; nCtrl < m_aCustomControls.GetSize(); nCtrl++)
	{
		const CTRLITEM& ctrl = m_aCustomControls[nCtrl];

		if (ctrl.nCtrlID == nCtrlID)
			return TDC::MapAttributeToColumn(ctrl.nAttrib);
	}

	// pick up any stragglers
	switch (nCtrlID)
	{
	case IDC_DONEDATE:
		return TDCC_DONE;

	case IDC_TIMESPENT:
		return TDCC_TRACKTIME;
	}

//	ASSERT(0);
	return TDCC_NONE;
}

BOOL CToDoCtrl::HandleCustomColumnClick(TDC_COLUMN nColID)
{
	if (!CTDCCustomAttributeHelper::IsCustomColumn(nColID))
		return FALSE;

	TDCCUSTOMATTRIBUTEDEFINITION attribDef;
	VERIFY (CTDCCustomAttributeHelper::GetAttributeDef(nColID, m_aCustomAttribDefs, attribDef));
	
	TDCCADATA data(GetSelectedTaskCustomAttributeData(attribDef.sUniqueID, FALSE));
	
	switch (attribDef.GetDataType())
	{
	case TDCCA_BOOL:
		// toggle the flag state
		if (data.AsBool())
		{
			data.Set(false);
		}
		else
		{
			TCHAR nChar = 0;

			if (attribDef.aDefaultListData.GetSize())
				nChar = attribDef.aDefaultListData[0][0];
		
			data.Set(true, nChar);
		}
		SetSelectedTaskCustomAttributeData(attribDef.sUniqueID, data.AsString(), FALSE);
		return TRUE; // handled
		
	case TDCCA_ICON:
		switch (attribDef.GetListType())
		{
		case TDCCA_FIXEDLIST:
			{
				CString sImage, sDummy;
				BOOL bNext = (!Misc::IsKeyPressed(VK_SHIFT));
				CString sTag = attribDef.GetNextListItem(data.AsString(), bNext);
				
				if (sTag.IsEmpty() || attribDef.DecodeImageTag(sTag, sImage, sDummy))
				{
					SetSelectedTaskCustomAttributeData(attribDef.sUniqueID, sImage, FALSE);
					return TRUE; // handled
				}
			}
			break;

		case TDCCA_FIXEDMULTILIST:
			// do nothing
			break;

		case TDCCA_NOTALIST:
			{
				CTDLTaskIconDlg dialog(m_ilTaskIcons, data.AsString());
				
				if (dialog.DoModal() == IDOK)
				{
					SetSelectedTaskCustomAttributeData(attribDef.sUniqueID, dialog.GetIconName(), FALSE);
					return TRUE; // handled
				}
			}
			break;
		}
		break;
		
	default:
		// do item cycling for fixed lists unless they support calculation
		if (attribDef.GetListType() == TDCCA_FIXEDLIST)
		{
			BOOL bNext = (!Misc::IsKeyPressed(VK_SHIFT));
			CString sItem = attribDef.GetNextListItem(data.AsString(), bNext);
			
			SetSelectedTaskCustomAttributeData(attribDef.sUniqueID, sItem, FALSE);
			return TRUE; // handled
		}
		break;
	}

	return FALSE; // not handled
}

void CToDoCtrl::TimeTrackTask(HTREEITEM hti)
{
	ASSERT (GetSelectedCount() == 1); // sanity check
	
	DWORD dwTaskID = GetTrueTaskID(hti);

	if (dwTaskID == m_dwTimeTrackTaskID)
	{
		// toggle off
		EndTimeTracking(TRUE, TRUE); 
	}
	else
	{
		BeginTimeTracking(dwTaskID, TRUE);
	}
}

// External
void CToDoCtrl::BeginTimeTracking(DWORD dwTaskID)
{
	BeginTimeTracking(dwTaskID, FALSE); // don't notify
}

// Internal
void CToDoCtrl::BeginTimeTracking(DWORD dwTaskID, BOOL bNotify)
{
	if (m_data.IsTaskTimeTrackable(dwTaskID))
	{
		// if there's a current task being tracked then end it
		EndTimeTracking(TRUE, bNotify);
		
		const TODOITEM* pTDI = GetTask(dwTaskID);
		ASSERT (pTDI);
		
		if (pTDI)
		{
			m_dwTimeTrackTaskID = dwTaskID;
			ResetTimeTracking();
			
			// if the task's start date has not been set then set it now
			if (!pTDI->HasStart())
				m_data.SetTaskDate(dwTaskID, TDCD_STARTDATE, COleDateTime::GetCurrentTime());

			m_taskTree.SetTimeTrackTaskID(dwTaskID);
			
			// Update Time spent control
			m_eTimeSpent.CheckButton(ID_TIME_TRACK, TRUE);
			m_eTimeSpent.EnableButton(ID_TIME_TRACK, TRUE);
			SetCtrlState(m_eTimeSpent, RTCS_READONLY);
			
			// notify parent
			if (bNotify)
				GetParent()->SendMessage(WM_TDCN_TIMETRACK, (WPARAM)GetSafeHwnd(), TRUE);
		}
	}
}

// External
void CToDoCtrl::EndTimeTracking(BOOL bAllowConfirm)
{
	EndTimeTracking(bAllowConfirm, FALSE); // don't notify
}

// Internal
void CToDoCtrl::EndTimeTracking(BOOL bAllowConfirm, BOOL bNotify)
{
	// if there's a current task being tracked then log it
	if (m_dwTimeTrackTaskID)
	{
		// add whatever time is still waiting
		IncrementTrackedTime(TRUE);

		// reset taskID and time to prevent re-entrancy
		DWORD dwTaskID = m_dwTimeTrackTaskID;
		double dTime = m_dLogTime;

		m_dwTickLast = 0;
		m_dwTimeTrackTaskID = 0;
		m_dLogTime = 0;

		// log it
		if (HasStyle(TDCS_LOGTIMETRACKING))
		{
			BOOL bShowDialog = (bAllowConfirm && HasStyle(TDCS_DISPLAYLOGCONFIRM));

			DoAddTimeToLogFile(dwTaskID, dTime, bShowDialog);
		}

		// update tree
		m_taskTree.SetTimeTrackTaskID(0);

 		// Update Time spent control
		m_eTimeSpent.CheckButton(ID_TIME_TRACK, FALSE);
		m_eTimeSpent.EnableButton(ID_TIME_TRACK, m_data.IsTaskTimeTrackable(dwTaskID));
		SetCtrlState(m_eTimeSpent, RTCS_ENABLED);

		// notify parent
		if (bNotify)
			GetParent()->SendMessage(WM_TDCN_TIMETRACK, (WPARAM)GetSafeHwnd(), FALSE);
	}
}

BOOL CToDoCtrl::DoAddTimeToLogFile()
{
	return DoAddTimeToLogFile(GetSelectedTaskID(), 0.0, TRUE);
}

BOOL CToDoCtrl::DoAddTimeToLogFile(DWORD dwTaskID, double dHours, BOOL bShowDialog)
{
	// sanity check
	if (!HasStyle(TDCS_LOGTIMETRACKING))
		return FALSE;
	
	CString sTaskTitle = GetTaskTitle(dwTaskID), sComment;
	COleDateTime dtWhen = COleDateTime::GetCurrentTime();
	BOOL bAddToTimeSpent = FALSE;
	BOOL bTracked = (dHours != 0.0);

	if (bShowDialog)
	{
		CTDLAddLoggedTimeDlg dialog(dwTaskID, sTaskTitle, dHours, this);

		// if we are readonly, we need to prevent
		// the dialog showing 'Add time to time spent'
		if (dialog.DoModal(!IsReadOnly() && !bTracked) != IDOK)
			return FALSE;

		// else
		dHours = dialog.GetLoggedHours();
		dtWhen = dialog.GetWhen();
		sComment = dialog.GetComment();
		bAddToTimeSpent = dialog.GetAddToTimeSpent();
	}

	return AddTimeToTaskLogFile(dwTaskID, dHours, dtWhen, sComment, bTracked, bAddToTimeSpent);
}

LRESULT CToDoCtrl::OnApplyAddLoggedTime(WPARAM wParam, LPARAM lParam)
{
	ASSERT(wParam && lParam);

	if (!wParam || !lParam)
		return 0L;

	const CTDLAddLoggedTimeDlg* pDialog = (const CTDLAddLoggedTimeDlg*)lParam;
	DWORD dwTaskID = wParam;

	return AddTimeToTaskLogFile(dwTaskID, 
								pDialog->GetLoggedHours(), 
								pDialog->GetWhen(), 
								pDialog->GetComment(), 
								pDialog->IsTracked(), 
								pDialog->GetAddToTimeSpent());
}

BOOL CToDoCtrl::AddTimeToTaskLogFile(DWORD dwTaskID, double dHours, const COleDateTime& dtWhen, 
									 const CString& sComment, BOOL bTracked, BOOL bAddToTimeSpent)
{
	// sanity check
	if ((dHours == 0.0) && sComment.IsEmpty())
		return FALSE; // sanity check

	CTDCTaskTimeLog log(GetFilePath());
	
	if (!log.LogTime(dwTaskID, 
					GetTaskTitle(dwTaskID), 
					GetTaskPath(dwTaskID), 
					dHours,
					dtWhen, 
					sComment, 
					bTracked, 
					HasStyle(TDCS_LOGTASKTIMESEPARATELY)))
	{
		UpdateWindow();
		AfxMessageBox(CEnString(IDS_LOGFILELOCKED));

		return FALSE;
	}

	// else
	if (bAddToTimeSpent && (dHours != 0.0))
	{
		TDC_UNITS nUnits = TDCU_NULL;
		double dTime = m_data.GetTaskTimeSpent(dwTaskID, nUnits);

		dTime += CTimeHelper().GetTime(dHours, THU_HOURS, TDC::MapUnitsToTHUnits(nUnits));

		if (GetSelectedCount() == 1 && GetSelectedTaskID() == dwTaskID)
		{
			SetSelectedTaskTimeSpent(dTime, nUnits);
		}
		else
		{
			m_data.SetTaskTimeSpent(dwTaskID, dTime, nUnits);
			SetModified(TRUE, TDCA_TIMESPENT, dwTaskID);
		}
	}

	return TRUE;
}

CString CToDoCtrl::GetSelectedTaskTimeLogPath() const
{
	DWORD dwTaskID = GetSelectedTaskID();

	if (dwTaskID)
		return CTDCTaskTimeLog(GetFilePath()).GetLogPath(dwTaskID, HasStyle(TDCS_LOGTASKTIMESEPARATELY));

	// else
	return _T("");
}

LRESULT CToDoCtrl::OnGetFont(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return (LRESULT)::SendMessage(::GetParent(*this), WM_GETFONT, 0, 0);
}

BOOL CToDoCtrl::IsTaskRecurring(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = GetTask(dwTaskID);
	ASSERT(pTDI);

	return (pTDI && pTDI->IsRecurring());
}

BOOL CToDoCtrl::CanTaskRecur(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = GetTask(dwTaskID);
	ASSERT(pTDI);
	
	return (pTDI && pTDI->CanRecur());
}

void CToDoCtrl::OnTreeSelChange(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	*pResult = 0;

	UpdateControls(); 
	GetParent()->PostMessage(WM_TDCN_SELECTIONCHANGE);
	
	// There's a very subtle bug on Windows 8.1 and above:
	// The auto-spell checking of the comments fields sends
	// change notifications even when no edit has been made.
	// This bypasses the usual handling of comments edits
	// and spuriously leaves comments pending for the new
	// selected task(s). If two or more tasks are selected
	// when the next attribute edit is made then the comments
	// of all the selected tasks can end up being cleared!
	// Since I can find no way to detect that an edit is 
	// purely the consequence of spell-checking, I can only
	// work around the problem by migrating the pending flag
	// to effectively clear the changes
	if (m_nCommentsState == CS_PENDING)
	{
		ASSERT(COSVersion() >= OSV_WIN8);
		m_nCommentsState = CS_CHANGED;
	}
}

BOOL CToDoCtrl::SelectTask(DWORD dwTaskID, BOOL bTrue) 
{ 
	if (bTrue)
		dwTaskID = m_data.GetTrueTaskID(dwTaskID);

	HTREEITEM hti = m_taskTree.GetItem(dwTaskID);

	if (hti)
		SelectItem(hti);

	return (hti != NULL);
}

BOOL CToDoCtrl::SelectTasks(const CDWordArray& aTaskIDs, BOOL bTrue)
{
	// sanity check
	ASSERT(aTaskIDs.GetSize());

	if (!aTaskIDs.GetSize())
		return FALSE;

 	return m_taskTree.SelectTasks(aTaskIDs, bTrue);
}

void CToDoCtrl::SelectItem(HTREEITEM hti) 
{ 
	Flush();

	if (m_taskTree.GetSafeHwnd()) 
	{
		// This is a nasty hack until I can figure out a better method.
		// The problem is that the auto droplists (category, status, alloc to/by)
		// rely on focus changes to get them to update, and if any of the
		// Goto..Task() methods are called there is no change of focus.
		HWND hFocus = ::GetFocus();

		if (IsChildOrSame(GetSafeHwnd(), hFocus))
			::SendMessage(hFocus, WM_KILLFOCUS, 0, 0);

		if (!m_taskTree.SelectItem(hti))
			UpdateControls(); // disable controls
		
		UpdateSelectedTaskPath();
		
		m_treeDragDrop.EnableDragDrop(!IsReadOnly());

		if (IsChildOrSame(GetSafeHwnd(), hFocus))
			::SendMessage(hFocus, WM_SETFOCUS, 0, 0);

		// notify parent
		GetParent()->PostMessage(WM_TDCN_SELECTIONCHANGE);
	}
}

void CToDoCtrl::SelectAll() 
{ 
	HandleUnsavedComments();

	if (m_taskTree.SelectAll())
	{
		// load newly selected item
		UpdateControls();
	}
	
	// re-enable dragdrop
	m_treeDragDrop.EnableDragDrop(FALSE);
}

LRESULT CToDoCtrl::OnGutterNotifyScroll(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	m_eTaskName.EndEdit(FALSE);
	return 0L;
}

BOOL CToDoCtrl::GetColumnAttribAndCtrl(TDC_COLUMN nCol, TDC_ATTRIBUTE& nAttrib, CWnd*& pWnd) const
{
	nAttrib = TDC::MapColumnToAttribute(nCol);
	ASSERT(nAttrib != TDCA_NONE);

	pWnd = GetAttributeCtrl(nAttrib);
	
	return (pWnd != NULL);
}

CWnd* CToDoCtrl::GetAttributeCtrl(TDC_ATTRIBUTE nAttrib) const
{
	UINT nCtrID = TDC::MapAttributeToCtrlID(nAttrib);
	ASSERT(nCtrID != (UINT)-1);
	
	CWnd* pCtrl = GetDlgItem(nCtrID);
	ASSERT_VALID(pCtrl);
	
	return pCtrl;
}

int CToDoCtrl::GetTasks(CTaskFile& tasks, const TDCGETTASKS& filter) const
{
	PrepareTaskfileForTasks(tasks, filter);
	
	// and FINALLY the tasks
	AddTreeChildrenToTaskFile(NULL, tasks, NULL, filter);
	
	return tasks.GetTaskCount();
}

void CToDoCtrl::PrepareTaskfileForTasks(CTaskFile& tasks, const TDCGETTASKS& filter) const
{
	tasks.Reset();

	// ISO date strings
	// must be done first before any tasks are added
	tasks.EnableISODates(HasStyle(TDCS_SHOWDATESINISO));
	
	// custom attributes
	SaveCustomAttributeDefinitions(tasks);
	
	// filename
	if (filter.HasFlag(TDCGTF_FILENAME))
		tasks.SetFileName(m_sLastSavePath);
	
	// meta data	
	tasks.SetMetaData(m_mapMetaData);
	
	// file header
	AppendTaskFileHeader(tasks);
}

BOOL CToDoCtrl::XMLHeaderIsUnicode(LPCTSTR szXmlHeader)
{
	return (Misc::ToUpper(szXmlHeader).Find(_T("UTF-16")) != -1);
}

void CToDoCtrl::AppendTaskFileHeader(CTaskFile& tasks) const
{
	ASSERT(XMLHeaderIsUnicode(m_sXmlHeader));

	tasks.SetXmlHeader(m_sXmlHeader);
	tasks.SetXslHeader(m_sXslHeader);
	tasks.SetProjectName(m_sProjectName);
	tasks.SetArchive(m_bArchive);
	tasks.SetFileFormat(FILEFORMAT_CURRENT);
	tasks.SetEarliestDueDate(m_data.GetEarliestDueDate());
	tasks.SetLastModified(m_dtLastTaskMod);

	tasks.SetNextUniqueID(m_dwNextUniqueID);
	ASSERT (tasks.GetNextUniqueID() == m_dwNextUniqueID);

	tasks.SetFileName(FileMisc::GetFileNameFromPath(m_sLastSavePath));
	
	if (!HasStyle(TDCS_USES3RDPARTYSOURCECONTROL))
	{
		tasks.SetFileVersion(m_nFileVersion);
	}
}

int CToDoCtrl::GetAllTasks(CTaskFile& tasks) const
{
	ASSERT(tasks.GetTaskCount() == 0);

	// and custom attributes
	SaveCustomAttributeDefinitions(tasks);

	tasks.EnableISODates(HasStyle(TDCS_SHOWDATESINISO));

	// build task file from data structure not the tree
	if (AddSubTasksToTaskFile(m_data.GetStructure(), tasks, NULL, TRUE))
	{
		return tasks.GetTaskCount();
	}
	
	// else
	return 0;
}

BOOL CToDoCtrl::AddSubTasksToTaskFile(const TODOSTRUCTURE* pTDSParent, CTaskFile& tasks, 
									  HTASKITEM hParentTask, BOOL bIncDuplicateCompletedRecurringSubtasks) const
{
	const TODOITEM* pTDILastRecurringSubtask = NULL;

	for (int nSubTask = 0; nSubTask < pTDSParent->GetSubTaskCount(); nSubTask++)
	{
		const TODOSTRUCTURE* pTDS = pTDSParent->GetSubTask(nSubTask);
		ASSERT(pTDS);
		
		if (!pTDS)
			return FALSE;
				
		DWORD dwTaskID = pTDS->GetTaskID();
		ASSERT(dwTaskID);
		
		if (!dwTaskID)
			return FALSE;
		
		const TODOITEM* pTDI = m_data.GetTask(dwTaskID, FALSE);
		ASSERT(pTDI);

		if (!pTDI)
			return FALSE;
		
		// Ignore duplicate 
		if (!bIncDuplicateCompletedRecurringSubtasks)
		{
			if (pTDI->IsRecurring())
			{
				if (pTDILastRecurringSubtask && 
					pTDI->RecurrenceMatches(*pTDILastRecurringSubtask, FALSE))
				{
					continue; // skip over it
				}

				pTDILastRecurringSubtask = pTDI;
			}
			else
			{
				pTDILastRecurringSubtask = NULL;
			}
		}

		if (!AddTaskToTaskFile(pTDI, pTDS, tasks, hParentTask, bIncDuplicateCompletedRecurringSubtasks))
		{
			ASSERT(0);
			return FALSE;
		}
	}
	
	return TRUE;
}

BOOL CToDoCtrl::AddTaskToTaskFile(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CTaskFile& tasks, 
								  HTASKITEM hParentTask, BOOL bIncDuplicateCompletedRecurringSubtasks) const
{
	CString sTitle = pTDI->sTitle;
	DWORD dwTaskID = pTDS->GetTaskID();
	
	HTASKITEM hTask = tasks.NewTask(sTitle, hParentTask, dwTaskID);
	ASSERT(hTask);
	
	if (!hTask)
		return FALSE;
	
	// copy all other attributes
	SetAllTaskAttributes(pTDI, pTDS, tasks, hTask);
	
	// copy children
	return AddSubTasksToTaskFile(pTDS, tasks, hTask, bIncDuplicateCompletedRecurringSubtasks);
}

void CToDoCtrl::HandleUnsavedComments()
{
	if (m_nCommentsState == CS_PENDING)
	{
		UpdateComments(TRUE); // no longer handled by UpdateData
		SetSelectedTaskComments(m_sTextComments, m_customComments, TRUE); // TRUE == internal call

		m_nCommentsState = CS_CHANGED;
	}
}

HTREEITEM CToDoCtrl::SetAllTasks(const CTaskFile& tasks)
{
	// Clear existing tree items
	TSH().RemoveAll(FALSE);
	m_taskTree.DeleteAll();

	// Build data structure first 
	m_data.BuildDataModel(tasks);

	// Then tree structure
	return RebuildTree();
}

HTREEITEM CToDoCtrl::RebuildTree(const void* pContext)
{
	TCH().SelectItem(NULL);
	m_taskTree.DeleteAll();

	if (BuildTreeItem(NULL, m_data.GetStructure(), pContext))
	{
		m_taskTree.SetNextUniqueTaskID(m_dwNextUniqueID);

		return m_taskTree.GetChildItem();
	}

	// else
	return NULL;
}

BOOL CToDoCtrl::BuildTreeItem(HTREEITEM htiParent, const TODOSTRUCTURE* pTDS, const void* pContext)
{
	// sanity checks
#ifdef _DEBUG
	if (pTDS)
	{
		if (htiParent == NULL || htiParent == TVI_ROOT)
		{
			ASSERT (pTDS->GetTaskID() == 0);

			if (pTDS->GetTaskID() != 0)
				return FALSE;
		}
		else
		{
			ASSERT (m_taskTree.GetTaskID(htiParent) == pTDS->GetTaskID());
			
			if (m_taskTree.GetTaskID(htiParent) != pTDS->GetTaskID())
				return FALSE;
		}
	}
	else
	{
		ASSERT(htiParent);
		ASSERT(!m_taskTree.ItemHasChildren(htiParent));

		DWORD dwTaskID = m_taskTree.GetTaskID(htiParent);
		pTDS = m_data.LocateTask(dwTaskID);
	}
#endif

	// rebuild
	int nSubtask = pTDS->GetSubTaskCount();

	// reverse-order tree-insertion is quicker according to Raymond Chen
	while (nSubtask--)
	{
		const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubtask);
		DWORD dwTaskID = pTDSChild->GetTaskID();

		// Get the 'True' task for matching but also hang
		// on to the 'original' task ID because it may get
		// overwritten and we need it for when we add the item
		DWORD dwOrgID = dwTaskID;

		const TODOITEM* pTDIChild = GetTask(dwTaskID);
		ASSERT(pTDIChild);

		// is this task wanted?
		BOOL bHasChildren = pTDSChild->HasSubTasks();
		BOOL bAddTask = TRUE;

		if (!bHasChildren && pContext)
			bAddTask = WantAddTask(pTDIChild, pTDSChild, pContext);

		if (bAddTask)
		{
			// add this item to tree
			HTREEITEM htiChild = InsertTreeItem(pTDIChild, dwOrgID, htiParent, TVI_FIRST);
			ASSERT(htiChild);

			// update next unique ID
			m_dwNextUniqueID = max(m_dwNextUniqueID, dwOrgID + 1);

			// and its children
			if (bHasChildren)
			{
				BuildTreeItem(htiChild, pTDSChild, pContext);

				// if no subtasks got added to the tree then we may need to remove the parent too
				if (!m_taskTree.ItemHasChildren(htiChild))
				{
					BOOL bRemoveTask = (pContext != NULL && !WantAddTask(pTDIChild, pTDSChild, pContext));

					if (bRemoveTask)
					{
						m_taskTree.DeleteItem(htiChild);
					}
				}
			}
		}
	}

	return TRUE;
}

BOOL CToDoCtrl::WantAddTask(const TODOITEM* /*pTDI*/, const TODOSTRUCTURE* /*pTDS*/, const void* /*pContext*/) const
{
	return TRUE;
}

int CToDoCtrl::GetSelectedTasks(CTaskFile& tasks, const TDCGETTASKS& filter, DWORD dwFlags) const
{
	if (!GetSelectedCount())
		return 0;
	
	PrepareTaskfileForTasks(tasks, filter);

	BOOL bWantSubtasks = !(dwFlags & TDCGSTF_NOTSUBTASKS);
	BOOL bResolveReferences = (dwFlags & TDCGSTF_RESOLVEREFERENCES);
	BOOL bWantAllParents = (dwFlags & TDCGSTF_ALLPARENTS);
	BOOL bWantImmediateParent = (bWantAllParents ? FALSE : (dwFlags & TDCGSTF_IMMEDIATEPARENT));

	// get selected tasks ordered
	// remove duplicate subtasks if we will be processing
	// subtasks anyway
	CHTIList selection;
	TSH().CopySelection(selection, bWantSubtasks, TRUE);
	
	// get items
	POSITION pos = selection.GetHeadPosition();

	while (pos)
	{
		HTREEITEM hti = selection.GetNext(pos);
		DWORD dwTaskID = GetTaskID(hti);

		// do we need to resolve task references?
		if (bResolveReferences)
		{
			DWORD dwRefID = GetTrueTaskID(hti);
			
			if (dwRefID != dwTaskID)
				hti = m_taskTree.GetItem(dwRefID); // true task
		}
		
		// does the user want this task's parent(s) ?
		if (bWantAllParents || bWantImmediateParent)
		{
			VERIFY(AddItemAndParentToTaskFile(hti, tasks, filter, bWantAllParents, bWantSubtasks));
		}
		else
		{
			// find the parent task previously added (or not)
			DWORD dwParentID = m_taskTree.GetTaskParentID(hti);
			HTASKITEM hParent = tasks.FindTask(dwParentID);

			VERIFY(AddTreeItemToTaskFile(hti, tasks, hParent, filter, bWantSubtasks, dwParentID));
		}
	}

	// extra processing to identify the originally selected tasks
	// in case the user wants to paste as references.
	// Note: References are excluded of bResolveReferences is true
	pos = TSH().GetFirstItemPos();

	while (pos)
	{
		DWORD dwSelID = TSH().GetNextItemData(pos);
		ASSERT(dwSelID);

		if (!bResolveReferences || !m_data.IsTaskReference(dwSelID))
		{
			HTASKITEM hSelTask = tasks.FindTask(dwSelID);
			ASSERT(hSelTask);

			tasks.SetTaskMetaData(hSelTask, _T("selected"), _T("1"));
		}
	}

	return (tasks.GetTaskCount());
}

BOOL CToDoCtrl::AddItemAndParentToTaskFile(HTREEITEM hti, CTaskFile& tasks, const TDCGETTASKS& filter, 
											BOOL bAllParents, BOOL bWantSubtasks) const
{
	// add parents first, recursively if necessarily
	HTREEITEM htiParent = m_taskTree.GetParentItem(hti);
	HTASKITEM hParent = NULL;
	DWORD dwParentID = 0;

	if (htiParent)
	{
		// Note: we never want parent's subtasks, so we pass FALSE
		if (bAllParents)
		{
			VERIFY(AddItemAndParentToTaskFile(htiParent, tasks, filter, TRUE, FALSE));
		}
		else
		{
			DWORD dwGrandParentID = m_taskTree.GetTaskParentID(htiParent);
			HTASKITEM hGrandParent = tasks.FindTask(dwGrandParentID);

			VERIFY(AddTreeItemToTaskFile(htiParent, tasks, hGrandParent, filter, FALSE, dwGrandParentID));
		}

		// now find the parent we just added
		dwParentID = GetTaskID(htiParent);
		
		hParent = tasks.FindTask(dwParentID);
		ASSERT(hParent);
	}

	// now add item itself
	return AddTreeItemToTaskFile(hti, tasks, hParent, filter, bWantSubtasks, dwParentID);
}

int CToDoCtrl::CacheTreeSelection(TDCSELECTIONCACHE& cache, BOOL bIncBreadcrumbs) const
{
	return m_taskTree.CacheSelection(cache, bIncBreadcrumbs);
}

BOOL CToDoCtrl::RestoreTreeSelection(const TDCSELECTIONCACHE& cache)
{
	if (m_taskTree.RestoreSelection(cache))
	{
		// don't update controls if only one item is selected 
		// and it did not change as a result of the operation
		BOOL bSelChange = !((GetSelectedCount() == 1) && 
							(cache.aSelTaskIDs.GetSize() == 1) &&
							(GetSelectedTaskID() == cache.aSelTaskIDs[0]));
		
		if (bSelChange)
			UpdateControls();

		return TRUE;
	}

	return FALSE;
}

int CToDoCtrl::GetSelectedTaskIDs(CDWordArray& aTaskIDs, BOOL bTrue) const
{
	return m_taskTree.GetSelectedTaskIDs(aTaskIDs, bTrue);
}

int CToDoCtrl::GetSelectedTaskIDs(CDWordArray& aTaskIDs, DWORD& dwFocusedTaskID, BOOL bRemoveChildDupes) const
{
	return m_taskTree.GetSelectedTaskIDs(aTaskIDs, dwFocusedTaskID, bRemoveChildDupes);
}

BOOL CToDoCtrl::InsertTasks(const CTaskFile& tasks, TDC_INSERTWHERE nWhere, BOOL bSelectAll)
{
	if (IsReadOnly())
		return FALSE;
	
	HTREEITEM htiParent = NULL, htiAfter = NULL;

	if (!m_taskTree.GetInsertLocation(nWhere, htiParent, htiAfter))
		return FALSE;

	// merge in any new custom attribute definitions
	CTDCCustomAttribDefinitionArray aAttribDefs;
	
	if (tasks.GetCustomAttributeDefs(aAttribDefs))
	{
		if (CTDCCustomAttributeHelper::AppendUniqueAttributes(aAttribDefs, m_aCustomAttribDefs))
			RebuildCustomAttributeUI();
	}

	// add the tasks
	IMPLEMENT_UNDO(TDCUAT_ADD);
	HOLD_REDRAW(*this, m_taskTree);

	// Fix up dependencies if not inserting into new tasklist
	if (GetTaskCount())
	{
		CTaskFile copy(tasks);

		// and always assign new IDs
		PrepareTasksForPaste(copy, TDCR_YES);
		return AddTasksToTree(copy, htiParent, htiAfter, TDCR_NO, bSelectAll, TDCA_PASTE);
	}

	return AddTasksToTree(tasks, htiParent, htiAfter, TDCR_NO, bSelectAll, TDCA_PASTE);
}

BOOL CToDoCtrl::SetTaskAttributes(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CTaskFile& file, 
								  HTASKITEM hTask, const TDCGETTASKS& filter, BOOL bTitleCommentsOnly) const
{
	ASSERT(pTDI);
	
	if (!pTDI)
		return FALSE;
	
	BOOL bDone = pTDI->IsDone();
	BOOL bTitleOnly = filter.HasFlag(TDCGTF_TITLESONLY);
	BOOL bHtmlComments = filter.HasFlag(TDCGTF_HTMLCOMMENTS);
	BOOL bTextComments = filter.HasFlag(TDCGTF_TEXTCOMMENTS);
	BOOL bTransform = filter.HasFlag(TDCGTF_TRANSFORM);

	// attributes
	file.SetTaskReferenceID(hTask, pTDI->dwTaskRefID);

	if (pTDS->HasSubTasks())
		file.SetTaskIsParent(hTask);

	// if task is a reference we use a bit of sleight of hand
	// and write the 'true' task's title but nothing else
	if (pTDI->dwTaskRefID)
		file.SetTaskTitle(hTask, m_data.GetTaskTitle(pTDI->dwTaskRefID));
	else
		file.SetTaskTitle(hTask, pTDI->sTitle);
	
	// hide IDs if not wanted
	if (bTitleOnly || bTitleCommentsOnly || !filter.WantAttribute(TDCA_ID))
		file.HideAttribute(hTask, TDL_TASKID);

	if (bTitleOnly || bTitleCommentsOnly || !filter.WantAttribute(TDCA_PARENTID))
		file.HideAttribute(hTask, TDL_TASKPARENTID);

	// ignore everything else if we are a reference
	if (pTDI->dwTaskRefID)
		return TRUE;
	
	if (!bTransform)
		file.SetTaskIcon(hTask, pTDI->sIcon);
	
	// comments
	if (!bTitleOnly && filter.WantAttribute(TDCA_COMMENTS))
	{
		CString sHtml;

		if (bHtmlComments && !pTDI->customComments.IsEmpty())
		{
			m_mgrContent.ConvertContentToHtml(pTDI->customComments, 
												sHtml, 
												pTDI->sCommentsTypeID, 
												file.GetHtmlCharSet(), 
												file.GetHtmlImageFolder());
		}

		// to simplify stylesheet design we render all comments
		// as HTMLCOMMENTS even if they are plain text
		if (bTransform)
		{
			if (sHtml.IsEmpty())
				sHtml = pTDI->sComments;

			if (!sHtml.IsEmpty())
			{
				file.SetTaskHtmlComments(hTask, sHtml, bTransform);

				// add dummy COMMENTS entry as a temprary fix in 6.8.8
				// because stylesheets currently require its presence
				file.SetTaskComments(hTask, pTDI->sComments);
			}
		}
		else // render both HTML _and_ plain text
		{
			if (!sHtml.IsEmpty())
				file.SetTaskHtmlComments(hTask, sHtml, bTransform);

			if (!pTDI->sComments.IsEmpty())
				file.SetTaskComments(hTask, pTDI->sComments);
		}
	}
	
	// highest priority, because we need it further down
	int nHighestPriority = m_data.GetTaskHighestPriority(pTDI, pTDS, FALSE);
	
	if (!(bTitleOnly || bTitleCommentsOnly))
	{
		if (filter.WantAttribute(TDCA_POSITION))
		{
			file.SetTaskPosition(hTask, pTDS->GetPosition());
			file.SetTaskPosition(hTask, m_data.GetTaskPositionString(pTDI, pTDS));
		}
		
		if (pTDI->bFlagged && filter.WantAttribute(TDCA_FLAG))
			file.SetTaskFlag(hTask, pTDI->bFlagged != FALSE);
		
		if (pTDI->IsRecurring() && filter.WantAttribute(TDCA_RECURRENCE))
			file.SetTaskRecurrence(hTask, pTDI->trRecurrence);
		
		if (pTDI->aAllocTo.GetSize() && filter.WantAttribute(TDCA_ALLOCTO))
			file.SetTaskAllocatedTo(hTask, pTDI->aAllocTo);
		
		if (!pTDI->sAllocBy.IsEmpty() && filter.WantAttribute(TDCA_ALLOCBY))
			file.SetTaskAllocatedBy(hTask, pTDI->sAllocBy);
		
		if (!pTDI->sStatus.IsEmpty() && filter.WantAttribute(TDCA_STATUS))
			file.SetTaskStatus(hTask, pTDI->sStatus);
		
		if (!pTDI->sVersion.IsEmpty() && filter.WantAttribute(TDCA_VERSION))
			file.SetTaskVersion(hTask, pTDI->sVersion);
		
		if (pTDI->aCategories.GetSize() && filter.WantAttribute(TDCA_CATEGORY))
			file.SetTaskCategories(hTask, pTDI->aCategories);
		
		if (pTDI->aTags.GetSize() && filter.WantAttribute(TDCA_TAGS))
			file.SetTaskTags(hTask, pTDI->aTags);
		
		if (pTDI->aFileLinks.GetSize() && filter.WantAttribute(TDCA_FILEREF))
			file.SetTaskFileLinks(hTask, pTDI->aFileLinks);
		
		if (!pTDI->sCreatedBy.IsEmpty() && filter.WantAttribute(TDCA_CREATEDBY))
			file.SetTaskCreatedBy(hTask, pTDI->sCreatedBy);
		
		if (!pTDI->sExternalID.IsEmpty() && filter.WantAttribute(TDCA_EXTERNALID))
			file.SetTaskExternalID(hTask, pTDI->sExternalID);
		
		if (pTDI->aDependencies.GetSize() && filter.WantAttribute(TDCA_DEPENDENCY))
			file.SetTaskDependencies(hTask, pTDI->aDependencies);

 		if (filter.WantAttribute(TDCA_PATH))
		{
			CString sPath = m_data.GetTaskPath(pTDI, pTDS);

			if (!sPath.IsEmpty())
 				file.SetTaskAttribute(hTask, TDL_TASKPATH, sPath);
		}
		
		if (filter.WantAttribute(TDCA_PRIORITY))
		{
			file.SetTaskPriority(hTask, pTDI->nPriority);
			
			if (nHighestPriority > pTDI->nPriority)
				file.SetTaskHighestPriority(hTask, nHighestPriority);
		}
		
		if (filter.WantAttribute(TDCA_RISK))
		{
			file.SetTaskRisk(hTask, pTDI->nRisk);
			
			int nHighestRisk = m_data.GetTaskHighestRisk(pTDI, pTDS);
			
			if (nHighestRisk > pTDI->nRisk)
				file.SetTaskHighestRisk(hTask, nHighestRisk);
		}
		
		// percent done
		if (filter.WantAttribute(TDCA_PERCENT))
		{
			// don't allow incomplete tasks to be 100%
			int nPercent = pTDI->IsDone() ? 100 : min(99, pTDI->nPercentDone); 
			file.SetTaskPercentDone(hTask, (unsigned char)nPercent);
			
			// calculated percent
			nPercent = m_data.CalcTaskPercentDone(pTDI, pTDS);
			
			if (nPercent > 0)
				file.SetTaskCalcCompletion(hTask, nPercent);
		}
		
		// cost
		if (filter.WantAttribute(TDCA_COST))
		{
			//if (pTDI->dCost > 0)
			file.SetTaskCost(hTask, pTDI->dCost);
			
			double dCost = m_data.CalcTaskCost(pTDI, pTDS);
			
			//if (dCost > 0)
			file.SetTaskCalcCost(hTask, dCost);
		}
		
		// time estimate
		if (filter.WantAttribute(TDCA_TIMEEST))
		{
			if ((pTDI->dTimeEstimate > 0) || (pTDI->nTimeEstUnits != TDCU_HOURS))
				file.SetTaskTimeEstimate(hTask, pTDI->dTimeEstimate, pTDI->nTimeEstUnits);
			
			// for calc'ed estimate use this item's units if it
			// has a non-zero time estimate, else its first subtask's units
			TDC_UNITS nUnits = m_data.GetBestCalcTimeEstUnits(pTDI, pTDS);
			double dTime = m_data.CalcTaskTimeEstimate(pTDI, pTDS, nUnits);
			
			if (dTime > 0)
				file.SetTaskCalcTimeEstimate(hTask, dTime, nUnits);
		}
		
		// time spent
		if (filter.WantAttribute(TDCA_TIMESPENT))
		{
			if ((pTDI->dTimeSpent != 0) || (pTDI->nTimeSpentUnits != TDCU_HOURS))
				file.SetTaskTimeSpent(hTask, pTDI->dTimeSpent, pTDI->nTimeSpentUnits);
			
			// for calc'ed spent use this item's units if it
			// has a non-zero time estimate, else its first subtask's units
			TDC_UNITS nUnits = m_data.GetBestCalcTimeSpentUnits(pTDI, pTDS);
			double dTime = m_data.CalcTaskTimeSpent(pTDI, pTDS, nUnits);
			
			if (dTime != 0)
				file.SetTaskCalcTimeSpent(hTask, dTime, nUnits);
		}
		
		// done date
		if (bDone)
		{
			file.SetTaskDoneDate(hTask, pTDI->dateDone);
			file.SetTaskGoodAsDone(hTask, TRUE);
			
			// hide it if column not visible
			if (!filter.WantAttribute(TDCA_DONEDATE))
			{
				file.HideAttribute(hTask, TDL_TASKDONEDATE);
				file.HideAttribute(hTask, TDL_TASKDONEDATESTRING);
			}
		}
		else if (m_data.IsTaskDone(pTDI, pTDS, TDCCHECKALL))
		{
			file.SetTaskGoodAsDone(hTask, TRUE);
		}
		
		// add due date if we're filtering by due date
		if (CDateHelper::IsDateSet(filter.dateDueBy) || filter.WantAttribute(TDCA_DUEDATE))
		{
			if (pTDI->HasDue())
			{
				file.SetTaskDueDate(hTask, pTDI->dateDue);
			}
			else if (HasStyle(TDCS_NODUEDATEISDUETODAYORSTART))
			{
				COleDateTime dtDue(CDateHelper::GetDate(DHD_TODAY));
				
				if (CDateHelper::Max(dtDue, pTDI->dateStart))
					file.SetTaskDueDate(hTask, dtDue);
			}
		}

		if (filter.WantAttribute(TDCA_DUEDATE) && (HasStyle(TDCS_USEEARLIESTDUEDATE) || HasStyle(TDCS_USELATESTDUEDATE)))
		{
			double dDate = m_data.CalcTaskDueDate(pTDI, pTDS);
			
			if (dDate > 0)
				file.SetTaskCalcDueDate(hTask, dDate);
		}
		
		// start date
		if (filter.WantAttribute(TDCA_STARTDATE))
		{
			if (pTDI->HasStart())
				file.SetTaskStartDate(hTask, pTDI->dateStart);
		
			if (HasStyle(TDCS_USEEARLIESTDUEDATE) || HasStyle(TDCS_USELATESTDUEDATE))
			{
				double dDate = m_data.CalcTaskStartDate(pTDI, pTDS);
				
				if (dDate > 0)
					file.SetTaskCalcStartDate(hTask, dDate);
			}
		}
		
		// creation date
		if (pTDI->HasCreation() && filter.WantAttribute(TDCA_CREATIONDATE))
			file.SetTaskCreationDate(hTask, pTDI->dateCreated);
		
		// modify date
		if (pTDI->HasLastMod() && filter.WantAttribute(TDCA_LASTMOD))
			file.SetTaskLastModified(hTask, pTDI->dateLastMod);

		// subtask completion
		if (pTDS->HasSubTasks() && filter.WantAttribute(TDCA_SUBTASKDONE))
		{
			int nSubtaskCount = 0, nSubtaskDone = 0;
			VERIFY (m_data.GetTaskSubtaskTotals(pTDI, pTDS, nSubtaskCount, nSubtaskDone));

			CString sSubtasksDone;
			sSubtasksDone.Format(_T("%d/%d"), nSubtaskDone, nSubtaskCount);
			
			file.SetTaskSubtaskCompletion(hTask, sSubtasksDone);
		}
		
		// custom comments
		if (filter.WantAttribute(TDCA_COMMENTS) && !(bHtmlComments || bTextComments))
		{
			// Even if it's a text format we still need to write out the comments format
			// unless there were no comments or the comment type is the same as the default
			if (CONTENTFORMAT(pTDI->sCommentsTypeID).FormatIsText())
			{
				if (!pTDI->sComments.IsEmpty() || pTDI->sCommentsTypeID != m_cfDefault)
					file.SetTaskCustomComments(hTask, _T(""), pTDI->sCommentsTypeID);
			}
			// else we save the custom comments either if there are any comments or if the
			// comments type is different from the default
			else if (!pTDI->customComments.IsEmpty() || pTDI->sCommentsTypeID != m_cfDefault)
				file.SetTaskCustomComments(hTask, pTDI->customComments, pTDI->sCommentsTypeID);
		}

		// custom data 
		if (filter.WantAttribute(TDCA_CUSTOMATTRIB))
			file.SetTaskCustomAttributeData(hTask, pTDI->mapCustomData);
	}
	else if (bDone)
	{
		file.SetTaskDoneDate(hTask, pTDI->dateDone);
		file.SetTaskGoodAsDone(hTask, TRUE);
		file.HideAttribute(hTask, TDL_TASKDONEDATE);
		file.HideAttribute(hTask, TDL_TASKDONEDATESTRING);
	}
	else if (m_data.IsTaskDone(pTDI, pTDS, TDCCHECKALL))
	{
		file.SetTaskGoodAsDone(hTask, TRUE);
	}

	// assigned task color
	file.SetTaskColor(hTask, pTDI->color);

	// runtime text color
	file.SetTaskTextColor(hTask, GetTaskTextColor(pTDI, pTDS));

	// priority color
	file.SetTaskPriorityColor(hTask, GetPriorityColor(nHighestPriority));

	return TRUE;
}

COLORREF CToDoCtrl::GetTaskTextColor(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	COLORREF crText = 0, crBack = CLR_NONE;
	
	if (m_taskTree.GetTaskTextColors(pTDI, pTDS, crText, crBack))
	{
		if (HasStyle(TDCS_TASKCOLORISBACKGROUND) && (crBack != CLR_NONE))
		{
			return crBack;
		}
		else if (crText != CLR_NONE)
		{
			return crText;
		}
	}

	// all else
	return 0;
}

BOOL CToDoCtrl::SetAllTaskAttributes(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CTaskFile& file, HTASKITEM hTask) const
{
	ASSERT(pTDI);
	
	if (!pTDI)
		return FALSE;

	// SPECIAL CASE:
	// if task is a reference we use a bit of sleight of hand
	// and write the 'true' task's title but nothing else
	if (pTDI->dwTaskRefID)
	{
		file.SetTaskTitle(hTask, m_data.GetTaskTitle(pTDI->dwTaskRefID));
		file.SetTaskReferenceID(hTask, pTDI->dwTaskRefID);

		return TRUE;
	}

	// 'true' tasks
	file.SetTaskAttributes(hTask, pTDI);

	// dynamically calculated attributes
	int nHighestPriority = m_data.GetTaskHighestPriority(pTDI, pTDS, FALSE); 
	
	if (nHighestPriority > pTDI->nPriority)
		file.SetTaskHighestPriority(hTask, nHighestPriority);
	
	int nHighestRisk = m_data.GetTaskHighestRisk(pTDI, pTDS);
	
	if (nHighestRisk > pTDI->nRisk)
		file.SetTaskHighestRisk(hTask, nHighestRisk);
	
	// calculated percent
	int nPercent = m_data.CalcTaskPercentDone(pTDI, pTDS);
	
	if (nPercent > 0)
		file.SetTaskCalcCompletion(hTask, nPercent);
	
	// cost
	double dCost = m_data.CalcTaskCost(pTDI, pTDS);
	
	if (dCost != 0)
		file.SetTaskCalcCost(hTask, dCost);
	
	// for calc'ed estimate use this item's units if it
	// has a non-zero time estimate, else its first subtask's units
	TDC_UNITS nUnits = m_data.GetBestCalcTimeEstUnits(pTDI, pTDS);
	double dTime = m_data.CalcTaskTimeEstimate(pTDI, pTDS, nUnits);
	
	if (dTime > 0)
		file.SetTaskCalcTimeEstimate(hTask, dTime, nUnits);
	
	// for calc'ed spent use this item's units if it
	// has a non-zero time estimate, else its first subtask's units
	nUnits = m_data.GetBestCalcTimeEstUnits(pTDI, pTDS);
	dTime = m_data.CalcTaskTimeSpent(pTDI, pTDS, nUnits);
	
	if (dTime != 0)
		file.SetTaskCalcTimeSpent(hTask, dTime, nUnits);
	
	// due date
	if (HasStyle(TDCS_USEEARLIESTDUEDATE) || HasStyle(TDCS_USELATESTDUEDATE))
	{
		double dDate = m_data.CalcTaskDueDate(pTDI, pTDS);
		
		if (dDate > 0)
			file.SetTaskCalcDueDate(hTask, dDate);
	}
	
	// start date
	if (HasStyle(TDCS_USEEARLIESTSTARTDATE) || HasStyle(TDCS_USELATESTSTARTDATE))
	{
		double dDate = m_data.CalcTaskStartDate(pTDI, pTDS);
		
		if (dDate > 0)
			file.SetTaskCalcStartDate(hTask, dDate);
	}
	
	// runtime text color
	file.SetTaskTextColor(hTask, GetTaskTextColor(pTDI, pTDS));
	
	// priority color
	file.SetTaskPriorityColor(hTask, GetPriorityColor(nHighestPriority));

	// 'good as done'
	if (m_data.IsTaskDone(pTDI, pTDS, TDCCHECKALL))
		file.SetTaskGoodAsDone(hTask, TRUE);

	// subtask completion
	int nSubtaskCount = 0, nSubtaskDone = 0;

	if (m_data.GetTaskSubtaskTotals(pTDI, pTDS, nSubtaskCount, nSubtaskDone))
	{
		CString sSubtasksDone;
		sSubtasksDone.Format(_T("%d/%d"), nSubtaskDone, nSubtaskCount);

		file.SetTaskSubtaskCompletion(hTask, sSubtasksDone);
	}
	
	return TRUE;
}

int CToDoCtrl::AddTreeChildrenToTaskFile(HTREEITEM hti, CTaskFile& file, HTASKITEM hTask, const TDCGETTASKS& filter) const
{
	HTREEITEM htiChild = m_taskTree.GetChildItem(hti);
	int nChildren = 0;
	
	while (htiChild)
	{
		if (AddTreeItemToTaskFile(htiChild, file, hTask, filter, TRUE)) // TRUE = want subtasks
			nChildren++;
		
		// next
		htiChild = m_taskTree.GetNextItem(htiChild);
	}
	
	return nChildren;
}

BOOL CToDoCtrl::AddTreeItemToTaskFile(HTREEITEM hti, CTaskFile& file, HTASKITEM hParentTask, 
										const TDCGETTASKS& filter, BOOL bWantSubtasks, DWORD dwParentID) const
{
	// attributes
	DWORD dwTaskID = GetTaskID(hti);
	const TODOITEM* pTDI = m_data.GetTask(dwTaskID, FALSE);
	ASSERT (pTDI);
	
	if (pTDI)
	{
		// Note: task file may already have this task if we've 
		// been auto-adding parents elsewhere
		HTASKITEM hTask = file.FindTask(dwTaskID);
		
		if (hTask == NULL)
		{
			hTask = file.NewTask(pTDI->sTitle, hParentTask, dwTaskID, dwParentID);
			ASSERT(hTask);
			
			if (!hTask)
				return FALSE;

			const TODOSTRUCTURE* pTDS = m_data.LocateTask(dwTaskID);
			ASSERT (pTDS);
			
			BOOL bTitleCommentsOnly = (m_taskTree.ItemHasChildren(hti) &&
									filter.HasFlag(TDCGTF_PARENTTITLECOMMENTSONLY));

			SetTaskAttributes(pTDI, pTDS, file, hTask, filter, bTitleCommentsOnly);
		}

		// we return TRUE if we match the filter _or_ we have any matching children
		BOOL bMatch = FALSE;
		
		if (bWantSubtasks)
		{
			bMatch = AddTreeChildrenToTaskFile(hti, file, hTask, filter);
		}

		if (!bMatch) //  no children matched -> 'Check ourselves'
		{
			BOOL bDone = pTDI->IsDone();
			BOOL bGoodAsDone = bDone ? TRUE : m_data.IsTaskDone(GetTaskID(hti), TDCCHECKALL);
			
			switch (filter.nFilter)
			{
			case TDCGT_ALL:
				bMatch = TRUE; // always
				break;
				
			case TDCGT_DUE:
			case TDCGT_DUETOMORROW:
			case TDCGT_DUETHISWEEK:
			case TDCGT_DUENEXTWEEK:
			case TDCGT_DUETHISMONTH:
			case TDCGT_DUENEXTMONTH:
				// remember to check for 'Auto-Due-Today' tasks
				if (!bGoodAsDone)
				{
					if (pTDI->HasDue())
					{
						bMatch = pTDI->IsDue(filter.dateDueBy);
					}
					else if (HasStyle(TDCS_NODUEDATEISDUETODAYORSTART))
					{
						COleDateTime dtDue(CDateHelper::GetDate(DHD_TODAY));

						if (CDateHelper::Max(dtDue, pTDI->dateStart))
							bMatch = (dtDue <= filter.dateDueBy);
					}
				}
				break;
				
			case TDCGT_DONE:
				bMatch |= (bGoodAsDone || bDone);
				break;
				
			case TDCGT_NOTDONE:
				bMatch |= !bGoodAsDone; // 'good as' includes 'done'

				// check 'flagged' flag
				if (!bMatch && filter.HasFlag(TDCGTF_KEEPFLAGGED) && pTDI->bFlagged) 
					bMatch = TRUE;
				break;
				
			default:
				bMatch = FALSE;
			}

			// then check 'allocated to' if set
			if (bMatch && !filter.sAllocTo.IsEmpty())
				bMatch = (Misc::Find(pTDI->aAllocTo, filter.sAllocTo) != -1);
		}
		
		// if we don't match, we remove the item
		if (!bMatch)
			file.DeleteTask(hTask);
		
		return bMatch;
	}
	
	return FALSE;
}

void CToDoCtrl::OnGotoFileRef()
{
	if (m_aFileRefs.GetSize())
		GotoFile(m_aFileRefs[0]);
}

void CToDoCtrl::SetFocusToTasks()
{
	if (!m_taskTree.HasFocus())
	{
		// NOTE: if the comments was the last window focused
		// before we were disabled, and we revert the focus
		// to the tree, then the comments gets very confused
		// and will not want to take the focus even though it
		// contains the caret, so we force it to have the focus
		// before switching to the tree.
		SetFocusToComments();
		
		m_taskTree.SetFocus();
	}

	// ensure the selected tree item is visible
	if (!m_taskTree.EnsureSelectionVisible())
		SelectItem(m_taskTree.GetChildItem());
}

void CToDoCtrl::SetFocusToComments()
{
	// ignore if comments are not visible
	if (!::IsWindowVisible(m_ctrlComments))
		return;

	m_ctrlComments.SetFocus();
}

CString CToDoCtrl::GetControlDescription(const CWnd* pCtrl) const
{
	CEnString sText; 

	if (IsChild(pCtrl))
	{
		// comments field
		if (::IsChild(m_ctrlComments, *pCtrl))
		{
			sText.LoadString(IDS_TDC_FIELD_COMMENTS);
		}
		else if (::IsChild(m_taskTree, *pCtrl))
		{
			sText.LoadString(IDS_TASKTREE);
		}
		else if (pCtrl == &m_cbCommentsType)
		{
			sText.LoadString(IDS_COMMENTSTYPE);
		}
		else
		{
			sText = GetCtrlLabel(pCtrl);
		}

		sText.Replace(_T("&"), _T(""));
	}

	return sText;
}

CString CToDoCtrl::GetLastSaveFolder() const
{
	return FileMisc::GetFolderFromFilePath(m_sLastSavePath);
}

void CToDoCtrl::MakeRelativePaths(CStringArray& aFilePaths) const
{
	if (aFilePaths.GetSize())
	{
		CString sParentFolder = GetLastSaveFolder();
		
		for (int nFile = 0; nFile < aFilePaths.GetSize(); nFile++)
		{
			CString& sFilePath = aFilePaths[nFile];
			FileMisc::MakeRelativePath(sFilePath, sParentFolder, FileMisc::FolderExists(sFilePath));
		}
	}
}

void CToDoCtrl::MakeFullPaths(CStringArray& aFilePaths) const
{
	CString sParentFolder = GetLastSaveFolder();
	
	for (int nFile = 0; nFile < aFilePaths.GetSize(); nFile++)
	{
		CString& sFilePath = aFilePaths[nFile];

		if (!WebMisc::IsURL(sFilePath))
			FileMisc::MakeFullPath(sFilePath, sParentFolder);
	}
}

LRESULT CToDoCtrl::OnDropObject(WPARAM wParam, LPARAM lParam)
{
	if (IsReadOnly())
		return 0L;

	ASSERT(wParam && lParam);
	
	TLDT_DATA* pData = (TLDT_DATA*)wParam;
	CWnd* pTarget = (CWnd*)lParam;

	int nNumFiles = pData->GetFileCount();
	BOOL bSingleOutlookObj = (pData->pOutlookSelection && (pData->pOutlookSelection->GetCount() == 1));

	// common handling
	CStringArray aFiles;

	if (bSingleOutlookObj)
	{
		// format outlook link
		OutlookAPI::_Item obj(pData->pOutlookSelection->Item(COleVariant((short)1)));

		DWORD dwFlags = (Misc::ModKeysArePressed(MKS_SHIFT) ? 0 : OAFMT_NICE);
		aFiles.Add(CMSOutlookHelper::FormatItemAsUrl(obj, dwFlags));
	}
	else if (nNumFiles)
	{ 
		aFiles.Copy(*(pData->pFilePaths));
	}

	// specific handling
	if (pTarget == &m_taskTree.Tree())
	{
		// check for one or more file paths to add to file refs
		if (pData->hti && aFiles.GetSize())
		{
			IMPLEMENT_UNDOEDIT();
			
			DWORD dwTaskID = GetTaskID(pData->hti);
			m_data.SetTaskFileRefs(dwTaskID, aFiles, TRUE);

			SetModified(TRUE, TDCA_FILEREF, dwTaskID);
		}
		else if (CreateTasksFromOutlookObjects(pData) != 0)
		{
			// nothing else to do
		}
		else if (pData->pObject)
		{
			// TODO
		}

		SetFocusToTasks();
		PostMessage(WM_TDC_FIXUPPOSTDROPSELECTION, 0L, (LPARAM)pData->hti);
	}
	else if ((pTarget == &m_cbFileRef) && aFiles.GetSize())
	{
		SetSelectedTaskFileRefs(aFiles, TRUE, FALSE);
		m_cbFileRef.SetFocus();
	}
	
	return 0;
}

int CToDoCtrl::CreateTasksFromOutlookObjects(const TLDT_DATA* pData)
{
	CTaskFile tasks;

	if (CTDCOutlookImportHelper::ImportTasks(pData, IDS_CSV_MUSTMAPTASKTITLE, &tasks))
	{
		// add to current tasklist
		HTREEITEM htiInsert = pData->hti;
		
		if (!htiInsert)
			htiInsert = m_taskTree.HitTestItem(CPoint(1, pData->ptClient.y));
		
		if (htiInsert)
		{
			SelectItem(htiInsert);
			VERIFY(InsertTasks(tasks, TDC_INSERTATBOTTOMOFSELTASK));
		}
		else
		{
			VERIFY(InsertTasks(tasks, TDC_INSERTATBOTTOM));
		}
	}

	return tasks.GetTaskCount();
}

CString CToDoCtrl::GetPreferencesKey(const CString& sSubKey) const
{
	CString sKeyPath;
	
	if (!m_sAltPrefsKey.IsEmpty())
	{
		sKeyPath = CPreferences::KeyFromFile(m_sAltPrefsKey);
	}
	else if (!m_sLastSavePath.IsEmpty())
	{
		sKeyPath = CPreferences::KeyFromFile(m_sLastSavePath);
	}
	else
	{
		sKeyPath = _T("Default");
	}

	CString sKey;
	
	if (!sSubKey.IsEmpty())
		sKey.Format(_T("FileStates\\%s\\%s"), sKeyPath, sSubKey);
	else
		sKey.Format(_T("FileStates\\%s"), sKeyPath);
	
	ASSERT(!sKey.IsEmpty());
	return sKey;
}

void CToDoCtrl::SaveDefaultRecurrence(CPreferences& prefs) const
{
	ASSERT (GetSafeHwnd());
	
	CString sKey = GetPreferencesKey(); // no subkey
	
	if (!sKey.IsEmpty())
	{
		prefs.WriteProfileInt(_T("DefaultRecurrence"), _T("RecurFrom"), m_nDefRecurFrom);
		prefs.WriteProfileInt(_T("DefaultRecurrence"), _T("RecurReuse"), m_nDefRecurReuse);
	}
}

void CToDoCtrl::LoadDefaultRecurrence(const CPreferences& prefs)
{
	ASSERT (GetSafeHwnd());
	
	CString sKey = GetPreferencesKey(); // no subkey
	
	if (!sKey.IsEmpty())
	{
		m_nDefRecurFrom = (TDI_RECURFROMOPTION)prefs.GetProfileInt(_T("DefaultRecurrence"), _T("RecurFrom"), TDIRO_DUEDATE);
		m_nDefRecurReuse = (TDI_RECURREUSEOPTION)prefs.GetProfileInt(_T("DefaultRecurrence"), _T("RecurReuse"), TDIRO_REUSE);
	}
}

void CToDoCtrl::SaveSplitPos(CPreferences& prefs) const
{
	ASSERT (GetSafeHwnd());
	
	CString sKey = GetPreferencesKey(); // no subkey
	
	if (!sKey.IsEmpty())
		prefs.WriteProfileInt(sKey, _T("SplitPos"), m_nCommentsSize);

	if (HasStyle(TDCS_SHAREDCOMMENTSHEIGHT))
		prefs.WriteProfileInt(_T("FileStates"), _T("SharedSplitPos"), s_nCommentsSize);
}

void CToDoCtrl::LoadSplitPos(const CPreferences& prefs)
{
	s_nCommentsSize = prefs.GetProfileInt(_T("FileStates"), _T("SharedSplitPos"), DEFCOMMENTSIZE);

	CString sKey = GetPreferencesKey(); // no subkey
	
	if (!sKey.IsEmpty() && !HasStyle(TDCS_SHAREDCOMMENTSHEIGHT))
		m_nCommentsSize = prefs.GetProfileInt(sKey, _T("SplitPos"), DEFCOMMENTSIZE);
	else
		m_nCommentsSize = s_nCommentsSize;
}

void CToDoCtrl::SaveAttributeVisibility(CPreferences& prefs) const
{
	m_visColAttrib.Save(prefs, GetPreferencesKey());
}

void CToDoCtrl::LoadAttributeVisibility(const CPreferences& prefs)
{
	TDCCOLEDITFILTERVISIBILITY vis;

	if (!vis.Load(prefs, GetPreferencesKey()))
		vis = m_visColAttrib;

	SetColumnEditFilterVisibility(vis);
}

void CToDoCtrl::SaveTasksState(CPreferences& prefs, BOOL bRebuildingTree) const
{
	// If rebuilding the tree we only save expanded state 
	m_taskTree.SaveState(prefs, GetPreferencesKey(_T("TaskTree")), bRebuildingTree);
}

HTREEITEM CToDoCtrl::LoadTasksState(const CPreferences& prefs, BOOL bRebuildingTree)
{
	// Only restore expanded state if rebuilding the tree
	HTREEITEM hti = m_taskTree.LoadState(prefs, GetPreferencesKey(_T("TaskTree")), bRebuildingTree);

	if (!bRebuildingTree)
		m_taskTree.RecalcColumnWidths();

	return hti;
}

void CToDoCtrl::EndLabelEdit(BOOL bCancel)
{
	m_eTaskName.EndEdit(bCancel);
}

void CToDoCtrl::Flush(BOOL bEndTimeTracking) // called to end current editing actions
{
	CWnd* pFocus = GetFocus();

	if (pFocus)
	{
		if (pFocus == &m_eTaskName)
		{
			EndLabelEdit(FALSE);
		}
		else if (m_cbCategory.IsChild(pFocus))
		{
			m_cbCategory.Flush();
		}		
		else if (m_cbTags.IsChild(pFocus))
		{
			m_cbTags.Flush();
		}		
		else if (m_cbStatus.IsChild(pFocus))
		{
			m_cbStatus.Flush();
		}		
		else if (m_cbAllocBy.IsChild(pFocus))
		{
			m_cbAllocBy.Flush();
		}		
		else if (m_cbAllocTo.IsChild(pFocus))
		{
			m_cbAllocTo.Flush();
		}

		// flush custom attributes also
		// TODO
	}

	m_treeDragDrop.CancelDrag();

	HandleUnsavedComments();

	if (bEndTimeTracking)
		EndTimeTracking(TRUE, FALSE);
}

CString CToDoCtrl::GetSourceControlID(BOOL bAlternate) const
{
	if (HasStyle(TDCS_INCLUDEUSERINCHECKOUT))
	{
		if (!bAlternate)
			return Misc::FormatComputerNameAndUser();

		// else
		return Misc::GetComputerName();
	}

	// else
	if (!bAlternate)
		return Misc::GetComputerName();

	// else
	return Misc::FormatComputerNameAndUser();
}

BOOL CToDoCtrl::MatchesSourceControlID(const CString& sID) const
{
	if (sID.IsEmpty())
		return FALSE;

	return ((GetSourceControlID(FALSE) == sID) ||
			(GetSourceControlID(TRUE) == sID));
}

TDC_FILE CToDoCtrl::CheckIn()
{
	ASSERT(m_bSourceControlled && m_bCheckedOut);

	Flush(TRUE);
	
	if (!m_bSourceControlled)
	{
		return TDCF_SSC_NOTSRCCONTROLLED;
	}
	else if (!m_bCheckedOut)
	{
		return TDCF_SSC_NOTCHECKEDOUT;
	}
	else if (m_sLastSavePath.IsEmpty()) // not yet saved
	{
		m_bCheckedOut = FALSE;
		return TDCF_SUCCESS;
	}
	
	// backup the file
	CTempFileBackup backup(m_sLastSavePath);
	
	// scope the task file so it gets closed
	// before we try to restore the backup
	int nFileErr = 0;
	{
		CWaitCursor cursor;
		CTaskFile file(m_sPassword);
		
		// snap shot mod time so we can restore it
		FILETIME ftMod = { 0 };
		FileMisc::GetFileLastModified(m_sLastSavePath, ftMod);
		
		// change check-out state before resaving
		m_bCheckedOut = FALSE;
		
		// resave
		BuildTasksForSave(file, FALSE);
		
		if (file.Save(m_sLastSavePath, SFEF_UTF16))
		{
			FileMisc::SetFileLastModified(m_sLastSavePath, ftMod);
			return TDCF_SUCCESS;
		}
		
		// else
		nFileErr = file.GetLastFileError();
	}
	
	// always restore backup on failure
	VERIFY(backup.RestoreBackup());
	
	// else someone else or invalid file
	return MapTaskfileError(nFileErr);
}

TDC_FILE CToDoCtrl::CheckOut()
{
	CString sTemp;
	return CheckOut(sTemp);
}

BOOL CToDoCtrl::IsCheckedOut() const 
{ 
	return (IsSourceControlled() && m_bCheckedOut); 
}

BOOL CToDoCtrl::IsSourceControlled() const 
{ 
	return m_bSourceControlled; 
}

BOOL CToDoCtrl::AddToSourceControl(BOOL bAdd)
{
	if ((bAdd && m_bSourceControlled) || (!bAdd && !m_bSourceControlled))
		return TRUE;
	
	if (!bAdd && !m_bCheckedOut)
		return FALSE;
	
	// update source control
	m_bSourceControlled = m_bCheckedOut = bAdd;
				
	// resave
	if (!m_sLastSavePath.IsEmpty())
	{
		CTaskFile file;
		BuildTasksForSave(file, FALSE);
		
		if (file.Save(m_sLastSavePath, SFEF_UTF16))
			return TRUE;
	}
	else // not yet saved
	{
		return TRUE;
	}
	
	// else restore previous state
	m_bSourceControlled = m_bCheckedOut = bAdd;
	
	return FALSE;
}

TDC_FILE CToDoCtrl::CheckOut(CString& sCheckedOutTo)
{
	ASSERT(m_bSourceControlled);
	
	if (!m_bSourceControlled)
		return TDCF_SSC_NOTSRCCONTROLLED;
	
	if (m_bDelayLoaded)
		return TDCF_SSC_DELAYLOADED;
	
	if (m_bCheckedOut)
	{
		// caller must think we're not checked out
		ASSERT(0);
		
		sCheckedOutTo = GetSourceControlID();
		return TDCF_SUCCESS;
	}
	
	// backup the file
	CTempFileBackup backup(m_sLastSavePath);
	CWaitCursor cursor;
	
	// scope the task file so it gets closed
	// before we try to restore the backup
	int nFileErr = 0;
	{
		CTaskFile file(m_sPassword);
		
		if (file.Open(m_sLastSavePath, XF_READ, FALSE) && file.LoadEx())
		{
			file.Close();
			
			sCheckedOutTo = file.GetCheckOutTo();
			
			if (sCheckedOutTo.IsEmpty())
			{
				// load tasks
				file.Decrypt();
				LoadTasks(file); // load file
				
				// update source control
				m_bCheckedOut = TRUE;
				
				// resave
				BuildTasksForSave(file, FALSE);
				
				if (file.Save(m_sLastSavePath, SFEF_UTF16))
				{
					// update modified time
					m_dtLastTaskMod = COleDateTime::GetCurrentTime();
					return TDCF_SUCCESS;
				}
				
				// restore state
				m_bCheckedOut = FALSE;
			}
		}
		
		nFileErr = file.GetLastFileError();
	}
	
	// always restore backup on failure
	VERIFY(backup.RestoreBackup());
	
	return MapTaskfileError(nFileErr);
}

int CToDoCtrl::FindTasks(const SEARCHPARAMS& params, CResultArray& aResults) const
{
	return m_taskTree.Find().FindTasks(params, aResults);
}

BOOL CToDoCtrl::HasOverdueTasks() const
{
	return m_data.HasOverdueTasks();
}

BOOL CToDoCtrl::HasDueTodayTasks() const
{
	return m_data.HasDueTodayTasks();
}

BOOL CToDoCtrl::SelectTask(CString sPart, TDC_SELECTTASK nSelect)
{
	SEARCHRESULT result;
	DWORD dwFoundID = 0;

	// build a search query
	SEARCHPARAMS params;
	params.aRules.Add(SEARCHPARAM(TDCA_TASKNAMEORCOMMENTS, FOP_INCLUDES, sPart));

	switch (nSelect)
	{
	case TDC_SELECTFIRST:
		dwFoundID = m_taskTree.Find().FindFirstTask(params, result);
		break;

	case TDC_SELECTNEXT:
	case TDC_SELECTNEXTINCLCURRENT:
	case TDC_SELECTPREV:
		{
			HTREEITEM hti = GetSelectedItem(); // our start point

			if (!hti)
				return FALSE;

			// we want to start searching from the next item
			switch (nSelect)
			{
				case TDC_SELECTNEXT:
					hti = TCH().GetNextItem(hti);
					break;

 				case TDC_SELECTNEXTINCLCURRENT:
 					// nothing to do
 					break;

				case TDC_SELECTPREV:
					hti = TCH().GetPrevItem(hti);
					break;
			}

			if (!hti)
				return FALSE; // at the end/start

			BOOL bNext = (nSelect != TDC_SELECTPREV);
			dwFoundID = m_taskTree.Find().FindNextTask(GetTaskID(hti), params, result, bNext);
		}
		break;

	case TDC_SELECTLAST:
		{
			HTREEITEM hti = TCH().GetLastItem(); // our start point

			if (!hti)
				return FALSE;

			dwFoundID = m_taskTree.Find().FindNextTask(GetTaskID(hti), params, result, FALSE);
		}
		break;

	default:
		ASSERT(0);
		break;
	}

	if (dwFoundID)
		SelectTask(dwFoundID);

	return (dwFoundID != 0);
}

void CToDoCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// is it over the comments splitter
	ASSERT (!m_bSplitting);

	if (IsSplitterVisible() && GetSplitterRect().PtInRect(point))
	{
		m_bSplitting = TRUE;
		SetCapture();
	}
	
	CRuntimeDlg::OnLButtonDown(nFlags, point);
}

void CToDoCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bSplitting)
	{
		ReleaseCapture();
		m_bSplitting = FALSE;

        SaveSplitPos(CPreferences());
	}
	
	CRuntimeDlg::OnLButtonUp(nFlags, point);
}

BOOL CToDoCtrl::GetStackCommentsAndControls() const
{
	return ((m_nMaxState == TDCMS_NORMAL) &&
			(m_nCommentsPos == m_nControlsPos) && 
			HasStyle(TDCS_AUTOREPOSCTRLS) &&
			HasStyle(TDCS_ALLOWCOMMENTSSTACKING));
}

int CToDoCtrl::CalcMinCommentSize() const
{
	int nMinCommentsSize = 0; // no minimum

	if (GetStackCommentsAndControls())
	{
		CDlgUnits dlu(this);

		switch (m_nCommentsPos)
		{
		case TDCUIL_RIGHT: // vertical
		case TDCUIL_LEFT:
			// two column widths
			nMinCommentsSize = (dlu.ToPixelsX((2 * CTRLCTRLLEN) + CTRLHSPACING) + 1);
			break;
			
		case TDCUIL_BOTTOM: // horizontal
			// two row height
			nMinCommentsSize = dlu.ToPixelsY(2* ((2 * CTRLHEIGHT) + CTRLVSPACING));
			break;

		default:
			ASSERT(0);
			break;
		}
	}
	
	return nMinCommentsSize;
}

void CToDoCtrl::ValidateCommentsSize()
{
	CRect rClient;
	GetClientRect(rClient);

	// don't validate for unexpectedly small rects
	if ((rClient.Width() < MINNONCOMMENTWIDTH) || 
		(rClient.Height() < MINNONCOMMENTHEIGHT))
	{
		return;
	}

	int nMaxComments = CalcMaxCommentSize();
	int nMinComments = CalcMinCommentSize();

	int nCommentSize = ((HasStyle(TDCS_SHAREDCOMMENTSHEIGHT) ? s_nCommentsSize : m_nCommentsSize));
	int nValidCommentSize = max(nMinComments, min(nMaxComments, nCommentSize));

	s_nCommentsSize = m_nCommentsSize = nValidCommentSize;
}

void CToDoCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bSplitting)
	{
		int nNewSize = 0;
		int nCommentsSize = ((HasStyle(TDCS_SHAREDCOMMENTSHEIGHT) ? s_nCommentsSize : m_nCommentsSize));
		CRect rSplitter = GetSplitterRect();
		
		switch (m_nCommentsPos)
		{
		case TDCUIL_RIGHT: // vertical
			nNewSize = (nCommentsSize - (point.x - rSplitter.left));
			break;
			
		case TDCUIL_LEFT: // vertical
			nNewSize = (nCommentsSize + (point.x - rSplitter.right));
			break;
			
		case TDCUIL_BOTTOM: // horizontal
		default:
			nNewSize = (nCommentsSize - (point.y - rSplitter.top));
			break;
		}

		// calculate the minimum comments size 
		int nMaxCommentSize = CalcMaxCommentSize();
		int nMinCommentSize = CalcMinCommentSize();

		nNewSize = min(max(nNewSize, nMinCommentSize), nMaxCommentSize);

		if (nNewSize != nCommentsSize)
		{
			// calc minimum control rect and only resize if the 
			// controls require less space than we have available
			CRect rCtrls, rClient;
			int nCols, nRows;
			GetClientRect(rClient);

			// NOTE: we need to set the splitter before we 
			// call CalcRequiredControlsRect since it
			// depends on the splitter width to determine its
			// available width/height.
			// BUT if the drag fails we must remember to restore
			// the previous split pos
			m_nCommentsSize = s_nCommentsSize = nNewSize;

			CalcRequiredControlsRect(rClient, rCtrls, nCols, nRows, TRUE);
			CRect rIntersect;

			if (rCtrls.IsRectEmpty() || (rIntersect.IntersectRect(rClient, rCtrls) && (rIntersect == rCtrls)))
			{
				Resize(0, 0, TRUE);
				Invalidate(TRUE);
				UpdateWindow();

				return;
			}

			// restore previous drag pos
			m_nCommentsSize = s_nCommentsSize = nCommentsSize;
		}
	}
	
	CRuntimeDlg::OnMouseMove(nFlags, point);
}

CRect CToDoCtrl::GetSplitterRect() const
{
	BOOL bStackCommentsAndControls = GetStackCommentsAndControls();

	// calc splitter rect from comment pos/size
	CRect rSplitter;
	GetClientRect(rSplitter);

	CRect rTasks;
	m_taskTree.GetWindowRect(rTasks);
	ScreenToClient(rTasks);

	switch (m_nCommentsPos)
	{
	case TDCUIL_RIGHT: // vertical
		{
			rSplitter.right -= m_nCommentsSize;
			rSplitter.left = (rSplitter.right - SPLITSIZE);

			ASSERT(rSplitter.Width() == SPLITSIZE);
		}
		break;
		
	case TDCUIL_LEFT:
		{
			rSplitter.left = m_nCommentsSize;
			rSplitter.right = (rSplitter.left + SPLITSIZE);

			ASSERT(rSplitter.Width() == SPLITSIZE);
		}
		break;
		
	case TDCUIL_BOTTOM: // horizontal
		{
			rSplitter.bottom -= (m_nCommentsSize + 2);
			rSplitter.top = (rSplitter.bottom - SPLITSIZE);

			ASSERT(rSplitter.Height() == SPLITSIZE);
		}
		break;
	}
	
	return rSplitter;
}

BOOL CToDoCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (IsSplitterVisible())
	{
		CPoint ptClient(::GetMessagePos());
		ScreenToClient(&ptClient);
		
		if (GetSplitterRect().PtInRect(ptClient))
		{
			switch (m_nCommentsPos)
			{
			case TDCUIL_RIGHT: // vertical
			case TDCUIL_LEFT:
				::SetCursor(AfxGetApp()->LoadCursor(AFX_IDC_HSPLITBAR));
				break;
				
			case TDCUIL_BOTTOM: // horizontal
				::SetCursor(AfxGetApp()->LoadCursor(AFX_IDC_VSPLITBAR));
				break;
			}
			
			return TRUE;
		}
	}
	
	// else
	return CRuntimeDlg::OnSetCursor(pWnd, nHitTest, message);
}

void CToDoCtrl::OnCaptureChanged(CWnd *pWnd) 
{
	if (m_bSplitting)
	{
		// save latest split pos
		s_nCommentsSize = m_nCommentsSize;
		m_bSplitting = FALSE;
	}
	
	CRuntimeDlg::OnCaptureChanged(pWnd);
}

void CToDoCtrl::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CRuntimeDlg::OnShowWindow(bShow, nStatus);
	
	if (bShow)
	{
		// resize if we have shared splitter pos and its different
		if (HasStyle(TDCS_SHAREDCOMMENTSHEIGHT) && (m_nCommentsSize != s_nCommentsSize))
		{
			m_nCommentsSize = s_nCommentsSize;
			Resize();
		}
	}
}

LRESULT CToDoCtrl::OnTimeUnitsChange(WPARAM wParam, LPARAM /*lParam*/)
{
	int nRecalcTime = IDNO;
	BOOL bWantQueryRecalc = (GetSelectedCount() > 1);
	
	if (!bWantQueryRecalc) // one item selected
	{
		TDC_UNITS nDummy;

		// see if the time is non-zero and if so we prompt
		switch (wParam)
		{
		case IDC_TIMEEST:
			bWantQueryRecalc = (GetSelectedTaskTimeEstimate(nDummy) > 0.0);
			break;
		
		case IDC_TIMESPENT:
			bWantQueryRecalc = (GetSelectedTaskTimeSpent(nDummy) > 0.0);
			break;
		}
	}

	if (bWantQueryRecalc)
		nRecalcTime = MessageBox(CEnString(IDS_TDC_RECALCPROMPT), CEnString(IDS_TDC_RECALCTITLE), MB_ICONQUESTION | MB_YESNOCANCEL);

	if (nRecalcTime != IDCANCEL)
	{
		DWORD dwFlags = (UTF_TIMEUNITSONLY | (nRecalcTime == IDYES ? UTF_RECALCTIME : 0));

		switch (wParam)
		{
		case IDC_TIMEEST:
			UpdateTask(TDCA_TIMEEST, dwFlags); 
			break;
		
		case IDC_TIMESPENT:
			UpdateTask(TDCA_TIMESPENT, dwFlags); 
			break;
		}
	}
	
	return (nRecalcTime == IDCANCEL);
}

void CToDoCtrl::SpellcheckSelectedTask(BOOL bTitle) 
{
	Flush();

	IMPLEMENT_UNDO(TDCUAT_EDIT);

	// one off spell check
	CSpellCheckDlg dialog;
	dialog.SetDictionaryDownloadUrl(DICTIONARY_URL);
	
	SpellcheckItem(GetSelectedItem(), &dialog, bTitle, TRUE);
}

LRESULT CToDoCtrl::OnCommentsWantSpellCheck(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	SpellcheckSelectedTask(FALSE); 

	return 0L;
}

BOOL CToDoCtrl::CanSpellcheckSelectedTaskComments()
{
	if (!CanSpellcheckComments())
		return FALSE;

	return !GetSelectedTaskComments().IsEmpty();
}

BOOL CToDoCtrl::CanSpellcheckComments()
{
	return (NULL != m_ctrlComments.GetSpellCheckInterface());
}

void CToDoCtrl::Spellcheck()
{
	IMPLEMENT_UNDO(TDCUAT_EDIT);

	// top level items
	CSpellCheckDlg dialog;
	dialog.SetDictionaryDownloadUrl(DICTIONARY_URL);

	HTREEITEM hti = m_taskTree.GetChildItem();
	
	while (hti)
	{
		if (!SpellcheckItem(hti, &dialog))
			return;
		
		hti = m_taskTree.GetNextItem(hti);
	}
}

BOOL CToDoCtrl::SpellcheckItem(HTREEITEM hti, CSpellCheckDlg* pSpellChecker, BOOL bTitle, BOOL bNotifyNoErrors)
{
	ASSERT(pSpellChecker);
	
	if (!pSpellChecker)
		return FALSE;

	if (!bTitle && !CanSpellcheckComments())
		return TRUE;
	
	DWORD dwTaskID = GetTaskID(hti);
	const TODOITEM* pTDI = GetTask(dwTaskID);
	ASSERT (pTDI);
	
	if (pTDI)
	{
		if (bTitle)
		{
			CString sTitle = m_data.GetTaskTitle(dwTaskID);
			
			if (sTitle.IsEmpty())
				return TRUE;

			// else
			SelectItem(hti);
			pSpellChecker->SetText(sTitle);
		}
		else
		{
			SelectItem(hti);
			pSpellChecker->SetSpellCheck(m_ctrlComments.GetSpellCheckInterface());
		}
		
		int nRet = pSpellChecker->DoModal(TRUE);
		UpdateWindow();
			
		if (nRet == IDOK && !IsReadOnly())
		{
			int nChange = SET_NOCHANGE;
			
			if (bTitle)
			{
				CString sTitle = pSpellChecker->GetCorrectedText();
				nChange = m_data.SetTaskTitle(dwTaskID, sTitle);
				
				if (nChange == SET_CHANGE)
				{
					InvalidateItem(hti, TRUE);
				}
			}
			else if (pSpellChecker->MadeChanges()) 
			{
				UpdateTask(TDCA_COMMENTS);
				nChange = SET_CHANGE;
			}
			
			if (nChange == SET_CHANGE)
				SetModified(TRUE, TDCA_TASKNAME, dwTaskID);
		}
		else if (nRet == IDNOERRORS && bNotifyNoErrors)
		{
			MessageBoxEx(this, bTitle ? IDS_TDC_NOTITLESPELLERRORS : IDS_TDC_NOCOMMENTSPELLERRORS, 
				IDS_TDC_SPELLCHECK_TITLE);
		}
		else if (nRet == IDCANCEL)
		{
			return FALSE;
		}

		return TRUE;
	}
	
	// else
	ASSERT(0);
	return FALSE;
}

BOOL CToDoCtrl::SpellcheckItem(HTREEITEM hti, CSpellCheckDlg* pSpellChecker)
{
	if (!SpellcheckItem(hti, pSpellChecker, TRUE, FALSE) || 
		!SpellcheckItem(hti, pSpellChecker, FALSE, FALSE))
		return FALSE;
	
	// subtasks
	HTREEITEM htiSub = m_taskTree.GetChildItem(hti);
	
	while (htiSub)
	{
		if (!SpellcheckItem(htiSub, pSpellChecker))
			return FALSE;
		
		htiSub = m_taskTree.GetNextItem(htiSub, TVGN_NEXT);
	}
	
	return TRUE;
}

int CToDoCtrl::GetSelectedTaskCustomAttributeData(CMapStringToString& mapData, BOOL bFormatted) const
{
	mapData.RemoveAll();
	
	int nCtrl = m_aCustomControls.GetSize();
	
	while (nCtrl--)
	{
		const CUSTOMATTRIBCTRLITEM& ctrl = m_aCustomControls.GetData()[nCtrl];
		mapData[ctrl.sAttribID] = GetSelectedTaskCustomAttributeData(ctrl.sAttribID, bFormatted);
	}
	
	return mapData.GetCount();
}

void CToDoCtrl::SetDefaultTaskAttributes(const TODOITEM& tdi)
{
	m_tdiDefault = tdi;

	m_cfDefault = m_tdiDefault.sCommentsTypeID;
	m_data.SetDefaultCommentsFormat(m_cfDefault);

	// set default task creation date to zero so that new tasks
	// always get the current date
	CDateHelper::ClearDate(m_tdiDefault.dateCreated);

	// pass on default time units for calculations
	m_taskTree.SetDefaultTImeUnits(tdi.nTimeEstUnits, tdi.nTimeSpentUnits);
	m_data.SetDefaultTimeUnits(tdi.nTimeEstUnits, tdi.nTimeSpentUnits);
}

LRESULT CToDoCtrl::OnEEBtnClick(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case IDC_TIMESPENT:
		if ((lParam == ID_TIME_TRACK) && (GetSelectedCount() == 1))
		{
			HandleUnsavedComments();
			TimeTrackTask(GetSelectedItem());
		}
		break;

	case IDC_DEPENDS:
		GotoSelectedTaskDependency();
		break;
	}
	
	return 0L;
}

BOOL CToDoCtrl::GotoSelectedReferenceTaskTarget()
{
	// traverse the selected items adding any
	// reference task's target to a list
	CDWordArray aTaskIDs;

	POSITION pos = TSH().GetFirstItemPos();
			
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);

		if (m_data.IsTaskReference(dwTaskID))
			aTaskIDs.Add(dwTaskID);
	}

	// select any found items
	if (aTaskIDs.GetSize())
		return SelectTasks(aTaskIDs, TRUE); 

	// else
	return FALSE;
}

BOOL CToDoCtrl::GotoSelectedTaskReferences()
{
	// traverse the selected items adding any
	// reference task's target to a list
	CDWordArray aTaskRefIDs;

	POSITION pos = TSH().GetFirstItemPos();
			
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);

		if (!m_data.IsTaskReference(dwTaskID))
		{
			CDWordArray aRefIDs;

			if (m_data.GetReferencesToTask(dwTaskID, aRefIDs))
				aTaskRefIDs.Append(aRefIDs);
		}
	}

	// select any found items
	if (aTaskRefIDs.GetSize())
		return SelectTasks(aTaskRefIDs, FALSE);

	// else
	return FALSE;
}

BOOL CToDoCtrl::GotoSelectedTaskDependency()
{
	CStringArray aDepends;
	GetSelectedTaskDependencies(aDepends);
	
	if (aDepends.GetSize())
		return ShowTaskLink(aDepends[0]);

	// else
	return FALSE;
}

BOOL CToDoCtrl::EditSelectedTaskRecurrence()
{
	if (!IsReadOnly() && GetSelectedCount() && !IsSelectedTaskDone())
		return m_eRecurrence.DoEdit(TRUE); // TRUE forces an edit

	// else
	return FALSE;
}

void CToDoCtrl::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case TIMER_TRACK:
		IncrementTrackedTime(FALSE);
		break;

	case TIMER_MIDNIGHT:
		{
			// check if we've just passed midnight, in which case some tasks
			// may have just become due
			static time_t tPrev = time(NULL); // once only
			time_t tNow = time(NULL);

			if ((tNow / DAYINSECS) > (tPrev / DAYINSECS))
			{
				OnTimerMidnight();
			}

			tPrev = tNow;
		}
		break;
	}

	
	CRuntimeDlg::OnTimer(nIDEvent);
}

void CToDoCtrl::OnTimerMidnight()
{
	Invalidate();
}

void CToDoCtrl::IncrementTrackedTime(BOOL bEnding)
{
	// if we are editing the title of the task being tracked then 
	// leave immediately and wait until the editing has ended
	if (!bEnding && IsTaskLabelEditing() && (m_dwTimeTrackTaskID == GetSelectedTaskID()))
		return;

	DWORD dwTick = GetTickCount();
	
	if (IsActivelyTimeTracking())
	{
		TDC_UNITS nUnits = TDCU_NULL;
		double dTime = m_data.GetTaskTimeSpent(m_dwTimeTrackTaskID, nUnits);
		
		ASSERT (m_dwTickLast);
		double dInc = ((dwTick - m_dwTickLast) * TICKS2HOURS); // hours
		
		// logged time is always in hours
		m_dLogTime += dInc;

		// task time is in whatever units the user specified
		TH_UNITS nTHUnits = TDC::MapUnitsToTHUnits(nUnits);

		dTime = CTimeHelper().GetTime(dTime, nTHUnits, THU_HOURS);
		dTime += dInc;
		dTime = CTimeHelper().GetTime(dTime, THU_HOURS, nTHUnits);
		
		if ((m_dwTimeTrackTaskID == GetSelectedTaskID()) && (GetSelectedCount() == 1))
		{
			// this will update the Time Spent field
			SetSelectedTaskTimeSpent(dTime, nUnits);
		}
		else
		{
			m_data.SetTaskTimeSpent(m_dwTimeTrackTaskID, dTime, nUnits);
			SetModified(TRUE, TDCA_TIMESPENT, m_dwTimeTrackTaskID);
		}
	}
	
	m_dwTickLast = dwTick;
}

LRESULT CToDoCtrl::OnCustomUrl(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	ASSERT(wParam == (WPARAM)m_ctrlComments.GetDlgCtrlID());
	
	// FALSE says only handle "tdc://" links
	GotoFile((LPCTSTR)lParam, FALSE);
	
	return 0;
}

void CToDoCtrl::SelectTasksInHistory(BOOL bForward) 
{ 
	HandleUnsavedComments();

	if (m_taskTree.SelectTasksInHistory(bForward))
		UpdateControls();
}

LRESULT CToDoCtrl::OnFileEditWantIcon(WPARAM wParam, LPARAM lParam)
{
	if (wParam == IDC_FILEPATH)
	{
		const CString& sUrl = (LPCTSTR)lParam;
		
		if (sUrl.Find(TDL_PROTOCOL) != -1)
			return (LRESULT)GraphicsMisc::GetAppWindowIcon(FALSE);
	}
	
	return 0;
}

LRESULT CToDoCtrl::OnFileEditDisplayFile(WPARAM wParam, LPARAM lParam)
{
	if (wParam == IDC_FILEPATH)
		return GotoFile((LPCTSTR)lParam);
	
	return 0;
}

BOOL CToDoCtrl::GotoFile(const CString& sFile, BOOL bShellExecute)
{
	if (sFile.IsEmpty())
		return FALSE;
	
	// see if its a 'tdl://'
	int nFind = sFile.Find(TDL_PROTOCOL);
	
	if (nFind >= 0)
	{
		OnTDCDoTaskLink(0, (LPARAM)(LPCTSTR)sFile);
		return TRUE;
	}
	else if (bShellExecute)
	{
		// set the current working directory to that of the tasklist
		CString sTasklistFolder = GetLastSaveFolder();

		// try to run it but disable default error handling
		if (CFileEdit::GotoFile(sFile, sTasklistFolder, FALSE) >= 32)
			return TRUE;
	}

	// all else, pass to parent
	return GetParent()->SendMessage(WM_TDCM_FAILEDLINK, (WPARAM)GetSafeHwnd(), (LPARAM)(LPCTSTR)sFile);
}

void CToDoCtrl::ExpandTasks(TDC_EXPANDCOLLAPSE nWhat, BOOL bExpand)
{
	if (!CanExpandTasks(nWhat, bExpand))
		return;

	HOLD_REDRAW(*this, m_taskTree);

	switch (nWhat)
	{
	case TDCEC_ALL:
		m_taskTree.ExpandAll(bExpand);
		break;

	case TDCEC_SELECTED:
		{
			POSITION pos = TSH().GetFirstItemPos();
			
			while (pos)
			{
				HTREEITEM hti = TSH().GetNextItem(pos);
				m_taskTree.ExpandItem(hti, bExpand, TRUE);
			}
		}
		break;

	case TDCEC_DUE:
		{
			// build due date search
			SEARCHPARAMS params;
			params.aRules.Add(SEARCHPARAM(TDCA_DUEDATE, FOP_ON_OR_BEFORE, CDateHelper::GetDate(DHD_TODAY)));
			params.bIgnoreDone = true;

			SearchAndExpand(params, bExpand);
		}
		break;

	case TDCEC_STARTED:
		{
			// build due date search
			SEARCHPARAMS params;
			params.aRules.Add(SEARCHPARAM(TDCA_STARTDATE, FOP_ON_OR_BEFORE, CDateHelper::GetDate(DHD_TODAY)));
			params.bIgnoreDone = true;

			SearchAndExpand(params, bExpand);
		}
		break;
	}

	// if collapsing, move the selection to a visible parent item
	if (!bExpand)
	{
		HTREEITEM htiSel = TSH().GetFirstItem(), hti = htiSel;

		while (hti && !TCH().IsItemVisible(hti))
		{
			hti = m_taskTree.GetParentItem(hti);
		}

		if (hti != htiSel)
			SelectItem(hti);
		else
			UpdateSelectedTaskPath();
	}
}

void CToDoCtrl::SearchAndExpand(const SEARCHPARAMS& params, BOOL bExpand)
{
	// perform the search
	CResultArray aResults;
	int nNumRes = m_taskTree.Find().FindTasks(params, aResults);
	
	// do the expansion
	for (int nRes = 0; nRes < nNumRes; nRes++)
	{
		HTREEITEM hti = m_taskTree.GetItem(aResults[nRes].dwTaskID);
		
		if (hti)
			m_taskTree.ExpandItem(hti, bExpand, TRUE);
	}
}

BOOL CToDoCtrl::CanExpandTasks(TDC_EXPANDCOLLAPSE nWhat, BOOL bExpand) const
{
	if (m_taskTree.GetItemCount() == 0)
		return FALSE;
		
	switch (nWhat)
	{
	case TDCEC_ALL:
		break;

	case TDCEC_SELECTED:
		{
			int nFullyExpanded = TSH().IsSelectionExpanded(TRUE);
			
			if (nFullyExpanded == -1)	// selected items have no children
			{
				return FALSE; // can neither expand nor collapse
			}
			else if (bExpand)
			{
				return !nFullyExpanded;
			}
			
			// else
			return TSH().IsSelectionExpanded(FALSE);
		}
		break;

	case TDCEC_DUE:
		break;

	case TDCEC_STARTED:
		break;
	}

	// all else
	return TRUE;
}

CString CToDoCtrl::GetClipboardID() const
{
	CString sClipID = CPreferences::KeyFromFile(GetFilePath(), FALSE);

	if (sClipID.IsEmpty())
		sClipID = _T("New_Tasklist");

	return sClipID;
}

BOOL CToDoCtrl::GetClipboardID(CString& sClipID, BOOL bArchive) const
{
	CString sFilePath;

	if (bArchive && !GetArchivePath(sFilePath))
	{
		return FALSE;
	}
	else if (sFilePath.IsEmpty())
	{
		sFilePath = GetFilePath();
	}

	if (sFilePath.IsEmpty())
		sClipID = _T("New_Tasklist");
	else
		sClipID = CPreferences::KeyFromFile(sFilePath, FALSE);

	return TRUE;
}

LRESULT CToDoCtrl::OnTDCHasClipboard(WPARAM /*wParam*/, LPARAM lParam)
{
	BOOL bCheckID = (lParam != FALSE);
	return !IsClipboardEmpty(bCheckID);
}

LRESULT CToDoCtrl::OnTDCGetClipboard(WPARAM wParam, LPARAM lParam)
{
	if (OnTDCHasClipboard(wParam, lParam))
	{
		static CTaskFile tasks;

		if (CTaskClipboard::GetTasks(tasks, GetClipboardID()))
 			return (LRESULT)&tasks;
	}

	// else
	return NULL;
}

LRESULT CToDoCtrl::OnTDCDoTaskLink(WPARAM /*wParam*/, LPARAM lParam)
{
	return ShowTaskLink((LPCTSTR)lParam);
}

LRESULT CToDoCtrl::OnTDCFailedLink(WPARAM /*wParam*/, LPARAM lParam)
{
	// Handle relative file path links
	LPCTSTR szLink = (LPCTSTR)lParam;

	if (::PathIsRelative(szLink))
	{
		CString sLink = FileMisc::GetFullPath(szLink, m_taskTree.GetTasklistFolder());
		
		if (FileMisc::Run(*this, sLink) > 32)
			return 0L;
	}

	// else forward on to our parent
	return GetParent()->SendMessage(WM_TDCM_FAILEDLINK, (WPARAM)GetSafeHwnd(), lParam);
}

LRESULT CToDoCtrl::OnTDCTaskIsDone(WPARAM wParam, LPARAM lParam)
{
	// forward on to our parent
	return GetParent()->SendMessage(WM_TDCM_ISTASKDONE, wParam, lParam);
}

BOOL CToDoCtrl::ShowTaskLink(const CString& sLink)
{
	if (sLink.IsEmpty())
		return FALSE;

	CString sFile;
	DWORD dwTaskID = 0;

	ParseTaskLink(sLink, dwTaskID, sFile);

	// if there's a file attached then pass to parent because we can't handle it.
	if (!sFile.IsEmpty())
	{
		return GetParent()->SendMessage(WM_TDCM_TASKLINK, dwTaskID, (LPARAM)(LPCTSTR)sFile);
	}
	else if (dwTaskID)
	{
		if (SelectTask(dwTaskID))
		{
			SetFocusToTasks();
			return TRUE;
		}
		else
		{
			MessageBoxEx(this, CEnString(IDS_TDC_TASKIDNOTFOUND, dwTaskID), IDS_TDC_TASKIDNOTFOUND_TITLE);
		}
	}
	else
	{
		MessageBoxEx(this, CEnString(IDS_TDC_ZEROINVALIDTASKID), IDS_TDC_ZEROINVALIDTASKID_TITLE);
	}

	return FALSE;
}

void CToDoCtrl::OnSelChangeCommentsType()
{
	HandleUnsavedComments();

	BOOL bMixedSelection = (m_cfComments.IsEmpty());

	// recreate comments control if necessary
	m_cbCommentsType.GetSelectedFormat(m_cfComments);
	CreateContentControl(TRUE);

	IMPLEMENT_UNDOEDIT();

	// modify the comments type of the selected tasks
	// without clearing any custom comments
	BOOL bChange = FALSE;
	POSITION pos = TSH().GetFirstItemPos();
	DWORD dwModTaskID = 0;
		
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);

		if (m_data.SetTaskCommentsType(dwTaskID, m_cfComments) == SET_CHANGE)
		{
			bChange = TRUE;
			dwModTaskID = dwTaskID;
		}
	}

	if (bChange)
		SetModified(TRUE, TDCA_COMMENTS, dwModTaskID);

	// update comments control state if previously in a mixed state
	if (bMixedSelection)
		UpdateControls();
}

void CToDoCtrl::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    CRuntimeDlg::OnSettingChange(uFlags, lpszSection);

    // spin button visibility goes badly wrong after a settings change
    // this is my best current solution other than subclassing the
    // spin button. Simply calling ShowWindow(SW_HIDE) from here
    // does not work.
    PostMessage(WM_TDC_REFRESHPERCENTSPINVISIBILITY);
}

LRESULT CToDoCtrl::OnRefreshPercentSpinVisibility(WPARAM /*wp*/, LPARAM /*lp*/)
{
    if (!m_ePercentDone.IsWindowVisible())
        m_spinPercent.ShowWindow(SW_HIDE);

    return 0L;
}

LRESULT CToDoCtrl::OnFixupPostDropSelection(WPARAM /*wp*/, LPARAM lp)
{
	if (lp)
		SelectItem((HTREEITEM)lp);

    return 0L;
}

LRESULT CToDoCtrl::OnChangeColour(WPARAM /*wp*/, LPARAM /*lp*/)
{
	UpdateTask(TDCA_COLOR);

	return 0L;
}

BOOL CToDoCtrl::CanUndoLastAction(BOOL bUndo) const 
{ 
	// handle comments field
	if (m_ctrlComments.HasFocus())
	{
		return (m_nCommentsState != CS_CLEAN);
	}
	else if (bUndo)
	{
		// handle simple edit fields
		CWnd* pFocus = GetFocus();

		if (pFocus)
		{
			CEdit* pEdit = NULL;
			UINT nFocusID = pFocus ? pFocus->GetDlgCtrlID() : 0;

			if (IsTaskLabelEditing() || nFocusID == IDC_PROJECTNAME ||
				m_cbCategory.IsChild(pFocus) || m_cbAllocTo.IsChild(pFocus) || m_cbTags.IsChild(pFocus))
			{
				pEdit = (CEdit*)pFocus;
			}

			if (pEdit)
				return pEdit->CanUndo();
		}
	}

	return m_data.CanUndoLastAction(bUndo); 
}

BOOL CToDoCtrl::UndoLastAction(BOOL bUndo)
{
	// handle comments field
	if (m_ctrlComments.HasFocus())
		return bUndo ? m_ctrlComments.Undo() : m_ctrlComments.Redo();

	// else handle if we're editing a tree label or if the focus is on the 
	// project name, category or alloc to fields
	CWnd* pFocus = GetFocus();

	if (pFocus)
	{
		UINT nFocusID = pFocus ? pFocus->GetDlgCtrlID() : 0;

		if (IsTaskLabelEditing() || nFocusID == IDC_PROJECTNAME)
		{
			CEdit* pEdit = (CEdit*)pFocus;
			
			// try to undo and prevent further processing 
			pEdit->Undo();
			return FALSE; 
		}
		else if (m_cbCategory.IsChild(pFocus) || 
				m_cbAllocTo.IsChild(pFocus) || 
				m_cbTags.IsChild(pFocus))
		{
			CEdit* pEdit = (CEdit*)pFocus;

			// only process and prevent further processing if the 
			// edit has something to undo
			if (pEdit->CanUndo())
			{
				pEdit->Undo();
				return FALSE; // combo edit takes precedence
			}
		}
	}

	if (m_data.CanUndoLastAction(bUndo))
	{
		CWaitCursor cursor;
		HOLD_REDRAW(*this, m_taskTree);
		
		TDCSELECTIONCACHE cache;
		CacheTreeSelection(cache);

		// fix up selection first in case we are about to delete the selected item
		m_taskTree.DeselectAll();

		// get the list of the task IDs that will be undone/redone
		CDWordArray aTaskIDs;
		TDCUNDOACTIONTYPE nUndoType = m_data.GetLastUndoActionType(bUndo);

		// but not if the result is that the items in question were deleted
		if (!(nUndoType == TDCUAT_DELETE && !bUndo) && 
			!(nUndoType == TDCUAT_ADD && bUndo))
		{
			m_data.GetLastUndoActionTaskIDs(bUndo, aTaskIDs);
		}

		CArrayUndoElements aElms;

		if (m_data.UndoLastAction(bUndo, aElms))
		{
			VERIFY(UndoLastActionItems(aElms));

			// restore selection
			if (!aTaskIDs.GetSize() || !m_taskTree.SelectTasks(aTaskIDs))
			{
				if (!RestoreTreeSelection(cache))
					TSH().PrevSelection(FALSE);
			}
			
			m_taskTree.OnUndoRedo(bUndo);

			// update current selection
			UpdateControls();

 			SetModified(TRUE, TDCA_UNDO, 0);

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CToDoCtrl::UndoLastActionItems(const CArrayUndoElements& aElms)
{
	// restore elements
	int nNumElm = aElms.GetSize();
	
	// now undo
	for (int nElm = 0; nElm < nNumElm; nElm++)
	{
		TDCUNDOELEMENT elm = aElms[nElm];
		
		if (elm.nOp == TDCUEO_EDIT)
		{
			ASSERT(0); // we shouldn't ever have these
		}
		else if (elm.nOp == TDCUEO_DELETE)
		{
			// find tree item and delete it
			// note: DeleteTask on the Parent will already have disposed of the children
			// so we can expect hti to be NULL on occasion. ie don't ASSERT it
			HTREEITEM hti = m_taskTree.GetItem(elm.dwTaskID);
			
			if (hti)
			{
				if (m_dwTimeTrackTaskID == elm.dwTaskID)
					EndTimeTracking(FALSE, FALSE);

				m_taskTree.DeleteItem(hti);
			}
		}
		else if (elm.nOp == TDCUEO_ADD)
		{
			// find parent item and restore task
			HTREEITEM htiParent = m_taskTree.GetItem(elm.dwParentID);
			HTREEITEM htiPrevSibling = m_taskTree.GetItem(elm.dwPrevSiblingID);
			
			if ((elm.dwParentID && htiParent) || (!elm.dwParentID && !htiParent))
			{
				HTREEITEM htiNew = m_taskTree.InsertItem(elm.dwTaskID, htiParent, htiPrevSibling);
				ASSERT(htiNew);
				
				if (!htiNew)
					return FALSE;
			}
		}
		else if (elm.nOp == TDCUEO_MOVE)
		{
			// move the task back to it original location
			HTREEITEM hti = m_taskTree.GetItem(elm.dwTaskID); // current tree item
			HTREEITEM htiDestParent = m_taskTree.GetItem(elm.dwParentID); // original owner
			HTREEITEM htiDestPrevSibling = m_taskTree.GetItem(elm.dwPrevSiblingID); // original previous sibling

			if ((elm.dwParentID && htiDestParent) || (!elm.dwParentID && !htiDestParent))
			{
				// undo the move
				if (!m_taskTree.MoveItem(hti, htiDestParent, htiDestPrevSibling))
				{
					ASSERT(0);
					return FALSE;
				}
			}
		}
		else
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

LRESULT CToDoCtrl::OnTDCGetTaskReminder(WPARAM wp, LPARAM lp)
{
	UNREFERENCED_PARAMETER(lp);
	ASSERT(wp && ((HWND)lp == m_taskTree.GetSafeHwnd()));

	return GetParent()->SendMessage(WM_TDCM_GETTASKREMINDER, wp, (LPARAM)this);
}

void CToDoCtrl::SetUITheme(const CUIThemeFile& theme)
{
	ASSERT(GetSafeHwnd());
	
	m_theme = theme;
	
	GraphicsMisc::VerifyDeleteObject(m_brUIBack);
	
	if (m_theme.crAppBackLight != CLR_NONE)
		m_brUIBack.CreateSolidBrush(m_theme.crAppBackLight);
	
	m_ctrlComments.SetUITheme(m_theme);
	m_taskTree.SetSplitBarColor(m_theme.crAppBackDark);

	Invalidate();
}

HBRUSH CToDoCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CRuntimeDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (CThemed::IsAppThemed() && (nCtlColor == CTLCOLOR_STATIC) && (pWnd != &m_eRecurrence))
	{
		if (m_theme.crAppText != CLR_NONE)
			pDC->SetTextColor(m_theme.crAppText);
		
		if (m_brUIBack.GetSafeHandle())
		{
			pDC->SetBkMode(TRANSPARENT);
			hbr = m_brUIBack;
		}
	}
	
	return hbr;
}

void CToDoCtrl::RedrawReminders()
{ 
	// Called after the app has made a change to a task's reminder
	m_taskTree.OnReminderChange();
}

TDC_ATTRIBUTE CToDoCtrl::GetFocusedControlAttribute() const
{
	HWND hFocus = ::GetFocus();
	UINT nCtrlID = ::GetDlgCtrlID(hFocus);
	TDC_COLUMN nColID = MapCtrlIDToColumn(nCtrlID);

	// handle edit controls of combos
	if (nColID == TDCC_NONE && CWinClasses::IsEditControl(hFocus))
	{
		hFocus = ::GetParent(hFocus);

		if (CWinClasses::IsComboBox(hFocus))
		{
			nCtrlID = ::GetDlgCtrlID(hFocus);
			nColID = MapCtrlIDToColumn(nCtrlID);
		}
	}

	TDC_ATTRIBUTE nAttrib = TDC::MapColumnToAttribute(nColID);

	if (nAttrib == TDCA_NONE)
		nAttrib = CTDCCustomAttributeHelper::GetAttributeID(nColID, m_aCustomAttribDefs);

	return nAttrib;
}

BOOL CToDoCtrl::CanClearSelectedTaskFocusedAttribute() const
{
	if (IsReadOnly())
		return FALSE;

	TDC_ATTRIBUTE nAttrib = GetFocusedControlAttribute();

	return CanClearSelectedTaskAttribute(nAttrib);
}

BOOL CToDoCtrl::ClearSelectedTaskFocusedAttribute()
{
	if (!CanClearSelectedTaskFocusedAttribute())
		return FALSE;

	TDC_ATTRIBUTE nAttrib = GetFocusedControlAttribute();

	return ClearSelectedTaskAttribute(nAttrib);
}

BOOL CToDoCtrl::CanClearSelectedTaskAttribute(TDC_ATTRIBUTE nAttrib) const
{
	if (IsReadOnly())
		return FALSE;

	return ((nAttrib >= TDCA_FIRSTATTRIBUTE && 
			 nAttrib < TDCA_ATTRIBUTECOUNT &&
			 nAttrib != TDCA_TASKNAME && 
			 nAttrib != TDCA_PROJNAME) ||
			CTDCCustomAttributeHelper::IsCustomAttribute(nAttrib));
}

BOOL CToDoCtrl::ClearSelectedTaskAttribute(TDC_ATTRIBUTE nAttrib)
{
	switch (nAttrib)
	{
	case TDCA_DONEDATE:		return SetSelectedTaskDate(TDCD_DONE, 0.0);
	case TDCA_DUEDATE:		return SetSelectedTaskDate(TDCD_DUE, 0.0);
	case TDCA_STARTDATE:	return SetSelectedTaskDate(TDCD_START, 0.0);
		
	case TDCA_DONETIME:		return SetSelectedTaskDate(TDCD_DONETIME, 0.0);
	case TDCA_DUETIME:		return SetSelectedTaskDate(TDCD_DUETIME, 0.0);
	case TDCA_STARTTIME:	return SetSelectedTaskDate(TDCD_STARTTIME, 0.0);
		
	case TDCA_PRIORITY:		return SetSelectedTaskPriority(FM_NOPRIORITY);
	case TDCA_RISK:			return SetSelectedTaskRisk(FM_NORISK);
		
	case TDCA_ALLOCTO:		return SetSelectedTaskAllocTo(CStringArray());
	case TDCA_CATEGORY:		return SetSelectedTaskCategories(CStringArray());
	case TDCA_TAGS:			return SetSelectedTaskTags(CStringArray());
	case TDCA_DEPENDENCY:	return SetSelectedTaskDependencies(CStringArray());
	case TDCA_FILEREF:		return SetSelectedTaskFileRefs(CStringArray());
		
	case TDCA_ALLOCBY:		return SetSelectedTaskAllocBy(_T(""));
	case TDCA_STATUS:		return SetSelectedTaskStatus(_T(""));
	case TDCA_VERSION:		return SetSelectedTaskVersion(_T(""));
	case TDCA_EXTERNALID:	return SetSelectedTaskExtID(_T(""));
		
	case TDCA_PERCENT:		return SetSelectedTaskPercentDone(0);
	case TDCA_FLAG:			return SetSelectedTaskFlag(FALSE);
	case TDCA_COST:			return SetSelectedTaskCost(0.0);
	case TDCA_COLOR:		return SetSelectedTaskColor(0);
	case TDCA_RECURRENCE:	return SetSelectedTaskRecurrence(TDCRECURRENCE());
		
	case TDCA_TIMEEST:		
		{
			TDC_UNITS nUnits;
			GetSelectedTaskTimeEstimate(nUnits); // preserve existing units
			return SetSelectedTaskTimeEstimate(0.0, nUnits);
		}

	case TDCA_TIMESPENT:
		{
			TDC_UNITS nUnits;
			GetSelectedTaskTimeSpent(nUnits); // preserve existing units
			return SetSelectedTaskTimeSpent(0.0, nUnits);
		}

	// these have no field
	case TDCA_SUBTASKDONE:
	case TDCA_POSITION:
	case TDCA_CREATEDBY:
	case TDCA_CREATIONDATE:
	case TDCA_LASTMOD:
	case TDCA_ICON:
		ASSERT(0);
		return FALSE;

	// These cannot be cleared
	case TDCA_ID:
	case TDCA_PARENTID:
	case TDCA_PATH:
		ASSERT(0);
		return FALSE;
	}

	// fall thru to custom attributes
	CString sCustomAttribID = CTDCCustomAttributeHelper::GetAttributeTypeID(nAttrib, m_aCustomAttribDefs);

	if (!sCustomAttribID.IsEmpty())
		return SetSelectedTaskCustomAttributeData(sCustomAttribID, _T(""), FALSE);

	// else something we've missed
	ASSERT(0);
	return FALSE;
}
