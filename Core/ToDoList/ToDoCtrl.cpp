// ToDoCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ToDoCtrl.h"
#include "taskfile.h"
#include "taskclipboard.h"
#include "tdcmsg.h"
#include "tdcmapping.h"
#include "tdcstringres.h"
#include "resource.h"
#include "TDCtasktimelog.h"
#include "todoitem.h"
#include "tdcstatic.h"
#include "tdcenum.h"
#include "tdltaskicondlg.h"
#include "tdlreuserecurringtaskdlg.h"
#include "tdlimportoutlookobjectsdlg.h"
#include "tdladdloggedtimedlg.h"
#include "tdcoutlookimporthelper.h"
#include "ToDoCtrlDataDefines.h"
#include "TDCDialogHelper.h"
#include "TDCTaskCompletion.h"
#include "tdccontentmgr.h"
#include "TDLRecurringTaskEdit.h"
#include "ToDoCtrlDataUtils.h"
#include "TDLSelectTaskDlg.h"

#include "..\shared\autoflag.h"
#include "..\shared\clipboard.h"
#include "..\shared\datehelper.h"
#include "..\shared\deferWndMove.h"
#include "..\shared\dlgunits.h"
#include "..\shared\driveinfo.h"
#include "..\shared\enbitmap.h"
#include "..\shared\encolordialog.h"
#include "..\shared\enfiledialog.h"
#include "..\shared\enmenu.h"
#include "..\shared\enstring.h"
#include "..\shared\filemisc.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\holdredraw.h"
#include "..\shared\localizer.h"
#include "..\shared\mapex.h"
#include "..\shared\messagebox.h"
#include "..\shared\misc.h"
#include "..\shared\msoutlookhelper.h"
#include "..\shared\osversion.h"
#include "..\shared\passworddialog.h"
#include "..\shared\savefocus.h"
#include "..\shared\ScopedTimer.h"
#include "..\shared\stringres.h"
#include "..\shared\themed.h"
#include "..\shared\toolbarhelper.h"
#include "..\shared\treectrlhelper.h"
#include "..\shared\wclassdefines.h"
#include "..\shared\webmisc.h"
#include "..\shared\winclasses.h"

#include "..\3rdparty\msoutl.h"
#include "..\3rdparty\shellicons.h"
#include "..\3rdparty\colordef.h"
#include "..\3rdparty\dibdata.h"
#include "..\3rdparty\gdiplus.h"
#include "..\3rdparty\XNamedColors.h"

#include "..\Interfaces\Preferences.h"
#include "..\interfaces\spellcheckdlg.h"
#include "..\interfaces\IContentControl.h"
#include "..\Interfaces\TasklistSchemaDef.h"

#include <Windowsx.h>
#include <float.h>
#include <Lmcons.h>
#include <math.h>
#include <afxpriv.h>
#include <shlwapi.h>

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

// In DLU
const int CTRLHEIGHT		= 13;
const int LABELHEIGHT		= 9; 
const int CTRLHSPACING		= 6; 
const int CTRLVSPACING		= 4; 
const int CTRLLEN			= 75;
const int COMMENTSTYPELEN	= 85;

/////////////////////////////////////////////////////////////////////////////

const int TIMETRACKPERIOD	= 10000; // 10 secs

/////////////////////////////////////////////////////////////////////////////

const int SPLITSIZE				= GraphicsMisc::ScaleByDPIFactor(6); 
const int DEFCOMMENTSIZE		= GraphicsMisc::ScaleByDPIFactor(260);
const int MINNONCOMMENTHEIGHT	= GraphicsMisc::ScaleByDPIFactor(250); // what's above the comment section
const int MINNONCOMMENTWIDTH	= GraphicsMisc::ScaleByDPIFactor(350); // what's to the left of the comment section
const int COMBODROPHEIGHT		= GraphicsMisc::ScaleByDPIFactor(200);
const int MINSTACKEDCOMMENTSIZE = GraphicsMisc::ScaleByDPIFactor(60);
const int INFOTIPOFFSET			= GraphicsMisc::ScaleByDPIFactor(16);

/////////////////////////////////////////////////////////////////////////////

const DWORD IDD_TODOCTRL_DIALOG = (DWORD)(LPCTSTR)_T("IDD_TODOCTRL_DIALOG");

/////////////////////////////////////////////////////////////////////////////

const LPCTSTR ARCHIVE_ID		= _T(".done");
const LPCTSTR DICTIONARY_URL	= _T("https://github.com/abstractspoon/ToDoList_Downloads/wiki/Dictionaries");

//////////////////////////////////////////////////////////////////////////////

enum 
{
	ID_TIME_TRACK = 0xfff0,
	ID_ADD_TIME,
	ID_EXTERNALID_LINK,
	ID_DEPENDS_LINK,
	ID_DEPENDS_EDIT,
};

enum // flags for UpdateTask
{
	UTF_RECALCTIME		= 0x01,	
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

//////////////////////////////////////////////////////////////////////////////
// private CToDoCtrl messages

UINT CToDoCtrl::WM_TDC_FIXUPPOSTDROPSELECTION		= (WM_APP + 1);
UINT CToDoCtrl::WM_TDC_RECREATERECURRINGTASK		= (WM_APP + 2);

//////////////////////////////////////////////////////////////////////////////
// Statics

HICON CToDoCtrl::s_hIconIconDlg = NULL;
HICON CToDoCtrl::s_hIconDependsDlg = NULL; 
HICON CToDoCtrl::s_hIconRecurDlg = NULL;
HICON CToDoCtrl::s_hIconAddLogDlg = NULL;

//////////////////////////////////////////////////////////////////////////////

CToDoCtrl::IDLETASKS::IDLETASKS(CToDoCtrl& tdc) : m_tdc(tdc)
{
}

void CToDoCtrl::IDLETASKS::RefreshAttributeValues(const CTDCAttributeMap& mapAttribIDs)
{
	ASSERT(!mapAttribIDs.IsEmpty());

	if (mapAttribIDs.Has(TDCA_ALL))
	{
		m_mapRefreshAttribIDs.Set(TDCA_ALL);
	}
	else if (!m_mapRefreshAttribIDs.Has(TDCA_ALL))
	{
		m_mapRefreshAttribIDs.Append(mapAttribIDs);
	}
}

BOOL CToDoCtrl::IDLETASKS::Process()
{
	if (!m_mapRefreshAttribIDs.IsEmpty())
	{
		m_tdc.m_ctrlAttributes.RefreshSelectedTasksValues(m_mapRefreshAttribIDs);

		m_mapRefreshAttribIDs.RemoveAll();
	}

	return HasTasks();
}

BOOL CToDoCtrl::IDLETASKS::HasTasks() const
{
	return !m_mapRefreshAttribIDs.IsEmpty();
}

//////////////////////////////////////////////////////////////////////////////

CToDoCtrl::CToDoCtrl(const CTDCContentMgr& mgrContent, 
					 const CShortcutManager& mgrShortcuts, 
					 const CONTENTFORMAT& cfDefault, 
					 const TDCCOLEDITFILTERVISIBILITY& visDefault) 
	: 
	m_bArchive(FALSE),
	m_bDelayLoaded(FALSE),
	m_bDeletingTasks(FALSE),
	m_bDragDropSubtasksAtTop(TRUE),
	m_bInSelectedTaskEdit(FALSE),
	m_bModified(FALSE), 
	m_calculator(m_data),
	m_cfDefault(cfDefault),
	m_dTrackedTimeElapsedHours(0),
	m_dwEditTitleTaskID(0),
	m_dwLastAddedID(0),
	m_dwNextUniqueID(1), 
	m_eTaskName(PEC_AUTODESTROY),
	m_hFontComments(NULL),
	m_hFontTree(NULL),
	m_mgrContent(mgrContent),
	m_nCommentsSize(DEFCOMMENTSIZE),
	m_nCommentsState(CS_CLEAN),
	m_nDefRecurFrom(TDIRO_DUEDATE),
	m_nDefRecurReuse(TDIRO_REUSE),
	m_nFileFormat(TDL_FILEFORMAT_CURRENT),
	m_bPendingUpdateControls(FALSE),
	m_nFileVersion(0),
	m_nPercentIncrement(5),
	m_nMaxInfotipCommentsLength(-1),
	m_visColEdit(visDefault),
	m_sXmlHeader(DEFAULT_UNICODE_HEADER),

	m_sourceControl(*this),
	m_findReplace(*this),
	m_reminders(*this),
	m_idleTasks(*this),

	m_data(m_styles, m_aCustomAttribDefs),
	m_timeTracking(m_data, m_taskTree.TSH()),
	m_formatter(m_data, mgrContent),
	m_attribCopier(m_data, mgrContent),
	m_exporter(m_data, m_taskTree, mgrContent),
	m_matcher(m_data, m_reminders, mgrContent),
	m_multitasker(m_data, mgrContent),

	m_ctrlComments(TRUE,
				   TRUE,
				   85,
				   &mgrContent,
				   &mgrShortcuts),

	m_treeDragDrop(m_taskTree.TSH(),
				   m_taskTree.Tree(),
				   &m_taskTree),

	m_infoTip(m_data,
			  m_aCustomAttribDefs,
			  mgrContent),

	m_taskTree(m_ilTaskIcons, 
			   m_data, 
			   m_styles, 
			   m_tldAll, 
			   m_visColEdit.GetVisibleColumns(), 
			   m_aCustomAttribDefs,
			   mgrContent),

	m_layout(this, 
			 &m_ctrlAttributes, 
			 &m_ctrlComments),

	m_ctrlAttributes(m_data,
					 mgrContent,
					 m_ilTaskIcons,
					 m_visColEdit,
					 m_reminders,
					 m_aCustomAttribDefs)
{
	SetBordersDLU(0);
	
	for (int nCtrl = 0; nCtrl < NUM_TDCCTRLS; nCtrl++)
	{
		const TDCCONTROL& ctrl = TDCCONTROLS[nCtrl];

		AddRCControl(_T("CONTROL"), ctrl.szClass, CString((LPCTSTR)ctrl.nIDCaption), 
					ctrl.dwStyle, ctrl.dwExStyle, ctrl.nX, ctrl.nY, ctrl.nCx, ctrl.nCy, ctrl.nID);
	}
	
	m_data.SetDefaultCommentsFormat(m_cfDefault);
}

CToDoCtrl::~CToDoCtrl()
{
}

void CToDoCtrl::DoDataExchange(CDataExchange* pDX)
{
	CRuntimeDlg::DoDataExchange(pDX);
	
	DDX_Text(pDX, IDC_PROJECTNAME, m_sProjectName);
}

void CToDoCtrl::UpdateComments(const CString& sTextComments, const CBinaryData& customComments)
{
	int nSelCount = GetSelectedTaskCount();

	if (sTextComments.IsEmpty() && (nSelCount > 1))
	{
		m_ctrlComments.ClearContent();
	}
	else
	{
		BOOL bCommentsFocused = m_ctrlComments.HasFocus();
		
		m_ctrlComments.SetContent(sTextComments, customComments, !bCommentsFocused);
	}

	CEnString sComboPrompt((nSelCount > 1) ? IDS_TDC_EDITPROMPT_MULTIPLEFORMATS : IDS_TDC_EDITPROMPT_UNKNOWNFORMAT);
	CEnString sCommentsPrompt((nSelCount > 1) ? IDS_TDC_EDITPROMPT_MULTIPLETASKS : IDS_TDC_EDITPROMPT_COMMENTS);

	m_ctrlComments.SetWindowPrompts(sComboPrompt, sCommentsPrompt);
}

BEGIN_MESSAGE_MAP(CToDoCtrl, CRuntimeDlg)
//{{AFX_MSG_MAP(CToDoCtrl)
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_SETTINGCHANGE()
	ON_WM_HELPINFO()

	ON_NOTIFY(NM_CLICK, IDC_TASKTREECTRL, OnTreeClick)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TASKTREECTRL, OnTreeSelChange)

	ON_MESSAGE(WM_GETFONT, OnGetFont)
	ON_MESSAGE(WM_TDC_RECREATERECURRINGTASK, OnRecreateRecurringTask)
	ON_MESSAGE(WM_TDC_FIXUPPOSTDROPSELECTION, OnFixupPostDropSelection)

	ON_REGISTERED_MESSAGE(WM_ICC_CONTENTCHANGE, OnCommentsChange)
	ON_REGISTERED_MESSAGE(WM_ICC_KILLFOCUS, OnCommentsKillFocus)
	ON_REGISTERED_MESSAGE(WM_ICC_WANTSPELLCHECK, OnCommentsWantSpellCheck)
	ON_REGISTERED_MESSAGE(WM_ICC_DOHELP, OnCommentsDoHelp)
	ON_REGISTERED_MESSAGE(WM_ICC_HASCLIPBOARD, OnTDCHasClipboard)
	ON_REGISTERED_MESSAGE(WM_ICC_GETCLIPBOARD, OnTDCGetClipboard)
	ON_REGISTERED_MESSAGE(WM_ICC_TASKLINK, OnTDCDoTaskLink)
	ON_REGISTERED_MESSAGE(WM_ICC_FAILEDLINK, OnTDCFailedLink)
	ON_REGISTERED_MESSAGE(WM_ICC_GETLINKTOOLTIP, OnCommentsGetTooltip)
	ON_REGISTERED_MESSAGE(WM_ICC_GETATTRIBUTELIST, OnCommentsGetAttributeList)

	ON_REGISTERED_MESSAGE(WM_TDCM_GETTASKREMINDER, OnTDCGetTaskReminder)
	ON_REGISTERED_MESSAGE(WM_TDCM_GETLINKTOOLTIP, OnTDCGetLinkTooltip)
	ON_REGISTERED_MESSAGE(WM_TDCM_FAILEDLINK, OnTDCFailedLink)
	ON_REGISTERED_MESSAGE(WM_TDCM_DISPLAYLINK, OnTDCDisplayLink)
	ON_REGISTERED_MESSAGE(WM_TDCM_EDITTASKATTRIBUTE, OnTDCEditTaskAttribute)
	ON_REGISTERED_MESSAGE(WM_TDCM_EDITTASKREMINDER, OnTDCEditTaskReminder)
	ON_REGISTERED_MESSAGE(WM_TDCM_CLEARTASKREMINDER, OnTDCClearTaskReminder)
	ON_REGISTERED_MESSAGE(WM_TDCM_CLEARTASKATTRIBUTE, OnTDCClearTaskAttribute)
	ON_REGISTERED_MESSAGE(WM_TDCM_TOGGLETIMETRACKING, OnTDCToggleTimeTracking)
	ON_REGISTERED_MESSAGE(WM_TDCM_ADDTIMETOLOGFILE, OnTDCAddTimeToLogFile)
	ON_REGISTERED_MESSAGE(WM_TDCM_SELECTDEPENDENCIES, OnTDCSelectDependencies)
	ON_REGISTERED_MESSAGE(WM_TDCM_COPYTASKATTRIBUTE, OnTDCCopyAttributeValue)
	ON_REGISTERED_MESSAGE(WM_TDCM_CANCOPYTASKATTRIBUTE, OnTDCCanCopyAttributeValue)
	ON_REGISTERED_MESSAGE(WM_TDCM_PASTETASKATTRIBUTE, OnTDCPasteAttributeValue)
	ON_REGISTERED_MESSAGE(WM_TDCM_CANPASTETASKATTRIBUTE, OnTDCCanPasteAttributeValue)

	ON_REGISTERED_MESSAGE(WM_TDCN_COLUMNEDITCLICK, OnTDCNotifyColumnEditClick)
	ON_REGISTERED_MESSAGE(WM_TDCN_ATTRIBUTEEDITED, OnTDCNotifyTaskAttributeEdited)
	ON_REGISTERED_MESSAGE(WM_TDCN_AUTOITEMADDEDDELETED, OnTDCNotifyAutoComboAddDelete)

	ON_CBN_SELENDOK(IDC_COMMENTS, OnSelChangeCommentsType)
	ON_EN_CHANGE(IDC_PROJECTNAME, OnChangeProjectName)

	ON_REGISTERED_MESSAGE(WM_FE_GETFILEICON, OnGetFileIcon)
	ON_REGISTERED_MESSAGE(WM_DD_DRAGABORT, OnTreeDragAbort)
	ON_REGISTERED_MESSAGE(WM_DD_DRAGDROP, OnTreeDragDrop)
	ON_REGISTERED_MESSAGE(WM_DD_DRAGENTER, OnTreeDragEnter)
	ON_REGISTERED_MESSAGE(WM_DD_DRAGOVER, OnTreeDragOver)
	ON_REGISTERED_MESSAGE(WM_DD_PREDRAGMOVE, OnTreeDragPreMove)
	ON_REGISTERED_MESSAGE(WM_PCANCELEDIT, OnLabelEditCancel)
	ON_REGISTERED_MESSAGE(WM_PENDEDIT, OnLabelEditEnd)
	ON_REGISTERED_MESSAGE(WM_TDL_APPLYADDLOGGEDTIME, OnApplyAddLoggedTime)
	ON_REGISTERED_MESSAGE(WM_TLDT_DROP, OnDropObject)
	ON_REGISTERED_MESSAGE(WM_TLDT_CANDROP, OnCanDropObject)
	ON_REGISTERED_MESSAGE(WM_FINDREPLACE, OnFindReplaceMsg)
	ON_REGISTERED_MESSAGE(WM_SS_NOTIFYSPLITCHANGE, OnSplitChange)
	ON_REGISTERED_MESSAGE(WM_SS_DRAWSPLITBAR, OnDrawSplitBar)
	ON_REGISTERED_MESSAGE(WM_TDCFR_GETEXCLUSIONRECT, OnFindReplaceGetExclusionRect)
	ON_REGISTERED_MESSAGE(WM_TDCFR_REPLACESELTASK, OnFindReplaceSelectedTask)
	ON_REGISTERED_MESSAGE(WM_TDCFR_REPLACEALLTASKS, OnFindReplaceAllTasks)
	ON_REGISTERED_MESSAGE(WM_TDCFR_SELECTNEXTTASK, OnFindReplaceSelectNextTask)
	ON_REGISTERED_MESSAGE(WM_TDCTI_RELOADICONS, OnTaskIconDlgReloadIcons)
	ON_REGISTERED_MESSAGE(WM_MIDNIGHT, OnMidnight)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToDoCtrl message handlers

BOOL CToDoCtrl::ParseTaskLink(const CString& sLink, BOOL bURL, DWORD& dwTaskID, CString& sFile) const
{
	return ParseTaskLink(sLink, bURL, GetLastSaveFolder(), dwTaskID, sFile);
}

BOOL CToDoCtrl::ParseTaskLink(const CString& sLink, BOOL bURL, const CString& sFolder, DWORD& dwTaskID, CString& sFile)
{
	return TDCTASKLINK::Parse(sLink, bURL, sFolder, dwTaskID, sFile);
}

CString CToDoCtrl::FormatTaskLink(DWORD dwTaskID, BOOL bFull) const
{
	if (!dwTaskID || (bFull && !HasFilePath()))
	{
		ASSERT(0);
		return _T("");
	}

	return TDCTASKLINK::Format(dwTaskID, TRUE, (bFull ? m_sLastSavePath : _T("")));
}

LRESULT CToDoCtrl::OnGetFileIcon(WPARAM /*wParam*/, LPARAM lParam)
{
	if (TDCTASKLINK::IsTaskLink((LPCTSTR)lParam, TRUE))
		return (LRESULT)GraphicsMisc::GetAppWindowIcon(FALSE);

	return 0L;
}

CString CToDoCtrl::FormatTaskDependency(DWORD dwTaskID, BOOL bFull) const
{
	if (!dwTaskID || (bFull && !HasFilePath()))
	{
		ASSERT(0);
		return _T("");
	}

	return TDCTASKLINK::Format(dwTaskID, FALSE, (bFull ? m_sLastSavePath : _T("")));
}

BOOL CToDoCtrl::IsReservedShortcut(DWORD dwShortcut)
{
	return CTDLTaskCtrlBase::IsReservedShortcut(dwShortcut);
}

void CToDoCtrl::EnableExtendedSelection(BOOL bCtrl, BOOL bShift)
{
	CTDLTaskCtrlBase::EnableExtendedSelection(bCtrl, bShift);
}

void CToDoCtrl::SetDialogIcons(HICON hIconIconDlg, HICON hIconDependsDlg, HICON hIconRecurDlg, HICON hIconAddLogDlg)
{
	s_hIconIconDlg = hIconIconDlg;
	s_hIconDependsDlg = hIconDependsDlg;
	s_hIconRecurDlg = hIconRecurDlg;
	s_hIconAddLogDlg = hIconAddLogDlg;
}

void CToDoCtrl::SetRecentlyModifiedPeriod(const COleDateTimeSpan& dtSpan)
{
	TODOITEM::SetRecentlyModifiedPeriod(dtSpan);
}

TDC_FILEFMT CToDoCtrl::CompareFileFormat() const
{
    if (m_nFileFormat < TDL_FILEFORMAT_CURRENT)
	{
		return TDCFF_OLDER;
	}
    else if (m_nFileFormat > TDL_FILEFORMAT_CURRENT)
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
		SetModified(TDCA_ENCRYPT);
	}
	else if (bEnable && m_sPassword.IsEmpty())
	{
		CEnString sExplanation(IDS_ENABLEENCRYPTION);

		if (CPasswordDialog::RetrievePassword(TRUE, m_sPassword, sExplanation))
			SetModified(TDCA_ENCRYPT);
	}
}

BOOL CToDoCtrl::WantPasswordReprompting() const
{
	return !HasStyle(TDCS_DISABLEPASSWORDPROMPTING);
}

BOOL CToDoCtrl::VerifyPassword(const CString& sExplanation) const
{
	if (!IsEncrypted())
		return TRUE; // no password to check

	return CPasswordDialog::VerifyPassword(m_sPassword, sExplanation);
}

BOOL CToDoCtrl::ModifyPassword()
{
	// Sanity checks
	if (!IsEncrypted() || IsReadOnly())
	{
		ASSERT(0);
		return FALSE;
	}

	CEnString sExplanation(IDS_ENABLEENCRYPTION);

	if (!CPasswordDialog::RetrievePassword(TRUE, m_sPassword, sExplanation))
		return FALSE;

	SetModified(TDCA_ENCRYPT);
	return TRUE;
}

BOOL CToDoCtrl::CanEncrypt()
{
	return CTaskFile::CanEncrypt();
}

BOOL CToDoCtrl::Create(const CRect& rect, CWnd* pParentWnd, UINT nID, BOOL bVisible, BOOL bEnabled)
{
	DWORD dwStyle = (WS_CHILD | (bVisible ? WS_VISIBLE : 0) | (bEnabled ? 0 : WS_DISABLED) | WS_TABSTOP);
	CLocalizer::IgnoreString(_T("ToDoCtrl"));
	
	return CRuntimeDlg::Create(_T("ToDoCtrl"), dwStyle | DS_SETFONT, WS_EX_CONTROLPARENT, rect, pParentWnd, nID);
}

BOOL CToDoCtrl::DoIdleProcessing()
{
	if (m_ctrlComments.HasFocus() && m_ctrlComments.DoIdleProcessing())
		return TRUE;

	// else
	if (m_taskTree.DoIdleProcessing())
		return TRUE;

	return m_idleTasks.Process();
}

BOOL CToDoCtrl::OnInitDialog() 
{
	CRuntimeDlg::OnInitDialog();

	// create the tree-list before anything else
	CRect rCtrl;
	GraphicsMisc::GetAvailableScreenSpace(*this, rCtrl);

	VERIFY(m_taskTree.Create(this, rCtrl, IDC_TASKTREECTRL));

	m_treeDropTarget.Register(&m_taskTree.Tree(), this);

	// comments
	VERIFY(m_ctrlComments.Create(this, IDC_COMMENTS));

	// Attributes
	VERIFY(m_ctrlAttributes.Create(this, IDC_ATTRIBUTELIST));
	m_ctrlAttributes.SetPercentDoneIncrement(m_nPercentIncrement);
	
	// custom font
	if (m_hFontTree)
		m_taskTree.SetFont(m_hFontTree);

	// Window prompts
	m_mgrPrompts.SetEditPrompt(IDC_PROJECTNAME, *this, IDS_TDC_EDITPROMPT_PROJECT);
	m_taskTree.SetWindowPrompt(CEnString(IDS_TDC_TASKLISTPROMPT));

	// tree drag drop
	m_treeDragDrop.Initialize(this);
	
	// Initial state
	UpdateControls();
	SetFocus(TDCSF_TASKVIEW);

	// notify parent that we have been created
	GetParent()->SendMessage(WM_PARENTNOTIFY, MAKEWPARAM(WM_CREATE, GetDlgCtrlID()), (LPARAM)GetSafeHwnd());

	// Start the timer which checks for midnight (day changeover)
	// which runs persistently
	m_timerMidnight.Enable(*this);
	
	return FALSE;  // return TRUE unless you set the focus to a control
}

void CToDoCtrl::LoadTaskIcons()
{
	VERIFY(m_ilTaskIcons.LoadImages(m_sLastSavePath, colorMagenta, HasStyle(TDCS_SHOWDEFAULTTASKICONS)));

	OnTaskIconsChanged();
}

const CTDCImageList& CToDoCtrl::GetTaskIconImageList() const 
{ 
	if (!m_ilTaskIcons.GetSafeHandle())
	{
		CToDoCtrl* pThis = const_cast<CToDoCtrl*>(this);
		pThis->LoadTaskIcons();
	}

	return m_ilTaskIcons; 
}

BOOL CToDoCtrl::SetTreeFont(HFONT hFont)
{
	ASSERT(hFont);

	if (hFont && !GraphicsMisc::IsSameFontNameAndSize(hFont, m_hFontTree))
	{
		m_hFontTree = hFont;

		if (m_infoTip.GetSafeHwnd())
			m_infoTip.SetFont(CFont::FromHandle(hFont));

		if (m_taskTree.GetSafeHwnd())
			return m_taskTree.SetFont(hFont);
	}

	// no change
	return FALSE;
}

BOOL CToDoCtrl::SetCommentsFont(HFONT hFont)
{
	ASSERT(hFont);

	if (hFont && !GraphicsMisc::IsSameFontNameAndSize(hFont, m_hFontComments))
	{
		m_hFontComments = hFont;

		// we've had some trouble with plugins using the richedit control 
		// so after a font change we always resend the content
		CString sTextComments;
		CBinaryData customComments;

		m_ctrlComments.GetContent(sTextComments, customComments);
		m_ctrlComments.SetContentFont(m_hFontComments);
		m_ctrlComments.SetContent(sTextComments, customComments, FALSE);

		return TRUE;
	}

	// no change
	return FALSE;
}

void CToDoCtrl::ResizeAttributeColumnsToFit()
{
	m_taskTree.RecalcAllColumnWidths();
}

void CToDoCtrl::SetMaximizeState(TDC_MAXSTATE nState)
{
	ASSERT(GetSafeHwnd());

	if (!HandleUnsavedComments())
		return;

	// Save split states
	CPreferences prefs;
	m_layout.SaveState(prefs, GetPreferencesKey());

	// Change state WITHOUT recalculating layout
	if (m_layout.SetMaximised(nState, HasStyle(TDCS_SHOWCOMMENTSALWAYS), FALSE))
	{
		ShowHideControls();

		// make sure focus is set correctly
		switch (nState)
		{
		case TDCMS_NORMAL:
			m_taskTree.EnsureSelectionVisible(TRUE);
			break;

		case TDCMS_MAXTASKLIST:
			if (!HasStyle(TDCS_SHOWCOMMENTSALWAYS) || !m_ctrlComments.HasFocus())
				SetFocus(TDCSF_TASKVIEW);
			break;

		case TDCMS_MAXCOMMENTS:
			m_ctrlComments.SetFocus();
			break;
		}

		// Restore split states WITHOUT recalculating layout
		m_layout.LoadState(prefs, GetPreferencesKey(), FALSE);
 
		// Recalculate layout
		Resize();
	}
}

void CToDoCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CRuntimeDlg::OnSize(nType, cx, cy);
	
	EndLabelEdit(TRUE);
	Resize(cx, cy);
	UpdateWindow();
}

void CToDoCtrl::SetLayoutPositions(TDC_UILOCATION nAttribsPos, TDC_UILOCATION nCommentsPos)
{
	if (m_layout.ModifyLayout(nAttribsPos,
							  nCommentsPos,
							  HasStyle(TDCS_ALLOWCOMMENTSSTACKING),
							  HasStyle(TDCS_STACKCOMMENTSABOVEEDITS)))
	{
		Resize();
	}
}

BOOL CToDoCtrl::OnEraseBkgnd(CDC* pDC)
{
	ExcludeChild(&m_taskTree, pDC);
	ExcludeChild(&m_ctrlAttributes, pDC);
	ExcludeChild(&m_ctrlComments, pDC);

	ExcludeCtrl(this, IDC_PROJECTLABEL, pDC);
	ExcludeCtrl(this, IDC_PROJECTNAME, pDC);

	m_layout.ExcludeSplitBars(pDC);

	// fill background with theme brush
	CSaveDC sdc(pDC);

	if (m_brUIBack.GetSafeHandle())
	{
		CRect rect;
		pDC->GetClipBox(rect);
		pDC->FillSolidRect(rect, m_theme.crAppBackLight);

		return TRUE;
	}

	// else
	return CRuntimeDlg::OnEraseBkgnd(pDC);
}

LRESULT CToDoCtrl::OnDrawSplitBar(WPARAM wp, LPARAM lp)
{
	ASSERT(wp && lp);

	CDC* pDC = CDC::FromHandle((HDC)wp);
	CRect rBar = (LPCRECT)lp;

	if (!rBar.IsRectEmpty())
	{
		GraphicsMisc::DrawSplitBar(pDC, rBar, m_theme.crAppBackDark, FALSE);
		pDC->ExcludeClipRect(rBar);

		return 1L; // we handled it
	}

	return 0L;
}

LRESULT CToDoCtrl::OnSplitChange(WPARAM wp, LPARAM lp)
{
 	if (!m_layout.IsRebuildingLayout())
	{
		CRect rPane = (LPCRECT)lp;

		if (!rPane.IsRectEmpty())
		{
			ReposProjectName(rPane);
			ReposTaskCtrl(rPane);
		}
	}

	return 0L;
}

void CToDoCtrl::Resize(int cx, int cy)
{
	if (m_taskTree.GetSafeHwnd())
	{
		if (!cx && !cy)
		{
			// Ignore resizes until we receive the first proper one
			if (!HasInitialSize())
			{
				CRect rClient;
				GetClientRect(rClient);

				cx = rClient.right;
				cy = rClient.bottom;
			}
		}
		else
		{
			ClearInitialSize();
		}

		m_layout.Resize(cx, cy);
		ShowHideControls();

		UpdateSelectedTaskPath();
	}
}

void CToDoCtrl::ReposProjectName(CRect& rAvailable)
{
	// project name
	CRect rLabel = GetCtrlRect(IDC_PROJECTLABEL); 
	CRect rProject = GetCtrlRect(IDC_PROJECTNAME); 

	int nOffset = (rAvailable.left - rLabel.left);

	rProject.left += nOffset;
	rProject.right = rAvailable.right;
	
	rLabel.OffsetRect(nOffset, 0);
	rLabel.top = rProject.top;
	rLabel.bottom = rProject.bottom;

	GetDlgItem(IDC_PROJECTLABEL)->MoveWindow(rLabel);
	GetDlgItem(IDC_PROJECTNAME)->MoveWindow(rProject);

	if (!m_layout.HasMaximiseState(TDCMS_MAXTASKLIST) && HasStyle(TDCS_SHOWPROJECTNAME))
		rAvailable.top = rProject.bottom + CDlgUnits(this).ToPixelsY(2);
	else
		rAvailable.top = rProject.top;
}

void CToDoCtrl::ReposTaskCtrl(const CRect& rTasks)
{
	m_taskTree.MoveWindow(rTasks);
}

void CToDoCtrl::ShowHideControls()
{
	switch (m_layout.GetMaximiseState())
	{
	case TDCMS_NORMAL:
		ShowCtrls(this, IDC_PROJECTLABEL, IDC_PROJECTNAME, HasStyle(TDCS_SHOWPROJECTNAME));
		ShowTaskCtrl(TRUE);
		m_ctrlAttributes.ShowWindow(SW_SHOW);
		m_ctrlComments.ShowWindow(SW_SHOW);
		break;

	case TDCMS_MAXTASKLIST:
		ShowCtrls(this, IDC_PROJECTLABEL, IDC_PROJECTNAME, FALSE);
		ShowTaskCtrl(TRUE);
		m_ctrlAttributes.ShowWindow(SW_HIDE);
		m_ctrlComments.ShowWindow(HasStyle(TDCS_SHOWCOMMENTSALWAYS) ? SW_SHOW : SW_HIDE);
		break;

	case TDCMS_MAXCOMMENTS:
		ShowCtrls(this, IDC_PROJECTLABEL, IDC_PROJECTNAME, FALSE);
		ShowTaskCtrl(FALSE);
		m_ctrlAttributes.ShowWindow(SW_HIDE);
		m_ctrlComments.ShowWindow(SW_SHOW);
		break;
	}

	EnableDisableControls();
}

void CToDoCtrl::EnableDisableControls(BOOL bHasSelection)
{
	if (bHasSelection == -1)
		bHasSelection = (GetUpdateControlsItem() && HasSelection());

	EnableDisableComments(bHasSelection);

	if (m_layout.HasMaximiseState(TDCMS_NORMAL) && HasStyle(TDCS_SHOWPROJECTNAME))
		SetCtrlState(this, IDC_PROJECTNAME, (IsReadOnly() ? RTCS_READONLY : RTCS_ENABLED));
	else
		SetCtrlState(this, IDC_PROJECTNAME, RTCS_DISABLED);
}

void CToDoCtrl::EnableDisableComments(BOOL bHasSelection)
{
	if (bHasSelection == -1)
		bHasSelection = (GetUpdateControlsItem() && HasSelection());

	CONTENTFORMAT cfComments;
	GetSelectedTaskCustomComments(cfComments);
	BOOL bEditComments = (m_mgrContent.FindContent(cfComments) != -1);

	BOOL bCommentsVis = m_layout.IsVisible(TDCSF_COMMENTS);
	RT_CTRLSTATE nCommentsState = RTCS_ENABLED, nComboState = RTCS_ENABLED;

	if (!bCommentsVis || !bHasSelection)
	{
		nComboState = nCommentsState = RTCS_DISABLED;
	}
	else if ((IsReadOnly() || m_taskTree.SelectionHasLocked()))
	{
		nComboState = nCommentsState = RTCS_READONLY;
	}
	else if (!bEditComments)
	{
		nCommentsState = RTCS_READONLY;
	}

	m_ctrlComments.SetCtrlStates(nComboState, nCommentsState);
}

void CToDoCtrl::UpdateSelectedTaskPath()
{
	m_taskTree.UpdateSelectedTaskPath();
}

void CToDoCtrl::UpdateControls(BOOL bIncComments)
{
	if (m_bDeletingTasks)
		return;

	// Something about the creation of the HTML comments plugin allows
	// subsequent selection changes from the task tree to be processed
	// before a previous call to this function has completed. 
	// And this re-entrancy causes mayhem because the HTML plugin can't
	// be terminated whilst it's in the process of initialising the web
	// browser within it.
	// So. We detect that the comments control has not yet completed and
	// set a flag so that when the comments control does complete we can
	// immediately re-call this function to get us up to date.
	// Fortunately, everything happens inside this function.
	if (m_bPendingUpdateControls || (bIncComments && m_ctrlComments.IsUpdatingFormat()))
	{
		m_bPendingUpdateControls = TRUE;
		return;
	}
	ASSERT(!m_bPendingUpdateControls);

	CDWordArray aSelTaskIDs;
	BOOL bHasSelection = (NULL != GetUpdateControlsItem());

	if (bHasSelection)
		bHasSelection = GetSelectedTaskIDs(aSelTaskIDs, TRUE);

	m_ctrlAttributes.SetSelectedTaskIDs(aSelTaskIDs);

	// and task header
	UpdateSelectedTaskPath();
	
	// Do the control enabling before updating the comments
	// to prevent unwanted intermediate comments states
	EnableDisableControls(bHasSelection);

	// Finally update comments
	if (bIncComments)
	{
		ASSERT(!m_ctrlComments.IsUpdatingFormat());

		// We need to take care to avoid unnecessary copying 
		// of a task's comments which can be huge
		static CString sEmptyComments;
		static CBinaryData emptyComments;

		CONTENTFORMAT cfPrev;
		m_ctrlComments.GetSelectedFormat(cfPrev);

		CONTENTFORMAT cfComments;
		const CBinaryData& customComments = (bHasSelection ? m_taskTree.GetSelectedTaskCustomComments(cfComments) : emptyComments);
		
		CString sTextComments = (bHasSelection ? m_taskTree.GetSelectedTaskComments() : sEmptyComments);
		
		// if more than one comments type is selected then sCommentsType
		// will be empty which will put the comments type combo in an
		// indeterminate state which is the desired effect since this requires
		// the user to reset the type before they can edit
		if (m_ctrlComments.SetSelectedFormat(cfComments))
			m_cfComments = cfComments;
		else
			m_cfComments.Empty();

		// See re-entrancy comment at start of function for explanation
		if (m_bPendingUpdateControls)
		{
			ASSERT(!m_ctrlComments.IsUpdatingFormat());

			m_bPendingUpdateControls = FALSE;
			UpdateControls(); // RECURSIVE CALL

			return;
		}
		
		UpdateComments(sTextComments, customComments);

		// Update the enable state again if the comments 
		// format changed because the new control will have 
		// been created enabled and we may not want that
		if (m_cfComments.IsEmpty() && (m_cfComments != cfPrev))
			EnableDisableComments(bHasSelection);
	}
}

void CToDoCtrl::ShowTaskCtrl(BOOL bShow)
{
	// if only the comments are visible then set the focus to the comments
	// before hiding the tasks, else Windows will select all the text
	// in the comments control
	if (!bShow)
		m_ctrlComments.SetFocus();

	m_taskTree.ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	m_taskTree.EnableWindow(bShow);
}

BOOL CToDoCtrl::UpdateTask(TDC_ATTRIBUTE nAttribID, DWORD dwFlags)
{
	if (!m_taskTree.GetSafeHwnd())
		return FALSE;
	
	if (!CanEditSelectedTask(nAttribID))
		return FALSE;
	
	// special case to circumvent CSaveFocus else it can mess up IME input
	if (nAttribID == TDCA_COMMENTS)
	{
		CDWordArray aModTaskIDs;
		GetSelectedTaskIDs(aModTaskIDs, FALSE);

		m_nCommentsState = CS_PENDING;
		SetModified(TDCA_COMMENTS, aModTaskIDs);

		return aModTaskIDs.GetSize();
	}	
	
	// else
	CSaveFocus sf;
	BOOL bChange = FALSE;
	
	switch (nAttribID)
	{
	case TDCA_TASKNAME:
		bChange = SetSelectedTaskTitle(m_ctrlAttributes.GetTaskTitle(), TRUE);
		break;

	case TDCA_DONEDATE:
 		bChange = SetSelectedTaskDate(TDCD_DONE, m_ctrlAttributes.GetDoneDate()); // NOT TDCD_DONEDATE
		break;
		
	case TDCA_DONETIME:
 		bChange = SetSelectedTaskDate(TDCD_DONETIME, m_ctrlAttributes.GetDoneTime());
		break;
		
	case TDCA_STARTDATE:
		bChange = SetSelectedTaskDate(TDCD_STARTDATE, m_ctrlAttributes.GetStartDate());
		break;
		
	case TDCA_STARTTIME:
		bChange = SetSelectedTaskDate(TDCD_STARTTIME, m_ctrlAttributes.GetStartTime());
		break;
		
	case TDCA_DUEDATE:
		bChange = SetSelectedTaskDate(TDCD_DUEDATE, m_ctrlAttributes.GetDueDate());
		break;
		
	case TDCA_DUETIME:
		bChange = SetSelectedTaskDate(TDCD_DUETIME, m_ctrlAttributes.GetDueTime());
		break;
		
	case TDCA_FLAG:
		bChange = SetSelectedTaskFlag(m_ctrlAttributes.GetFlag());
		break;

	case TDCA_LOCK:
		bChange = SetSelectedTaskLock(m_ctrlAttributes.GetLock());
		break;
		
	case TDCA_PRIORITY:
		bChange = SetSelectedTaskPriority(m_ctrlAttributes.GetPriority());
		break;

	case TDCA_RISK:
		bChange = SetSelectedTaskRisk(m_ctrlAttributes.GetRisk());
		break;

	case TDCA_EXTERNALID:
		bChange = SetSelectedTaskExternalID(m_ctrlAttributes.GetExternalID());
		break;

	case TDCA_ALLOCBY:
		bChange = SetSelectedTaskAllocBy(m_ctrlAttributes.GetAllocBy());
		break;

	case TDCA_STATUS:
		bChange = SetSelectedTaskStatus(m_ctrlAttributes.GetStatus());
		break;

	case TDCA_VERSION:
		bChange = SetSelectedTaskVersion(m_ctrlAttributes.GetVersion());
		break;

	case TDCA_PERCENT:
		bChange = SetSelectedTaskPercentDone(m_ctrlAttributes.GetPercentDone());
		break;

	case TDCA_COST:
		{
			TDCCOST cost;
			
			if (m_ctrlAttributes.GetCost(cost))
				bChange = SetSelectedTaskCost(cost);
		}
		break;
		
	case TDCA_DEPENDENCY:
		{
			CTDCDependencyArray aDepends;
			m_ctrlAttributes.GetDependencies(aDepends);

			bChange = SetSelectedTaskDependencies(aDepends);
		}
		break;
		
	case TDCA_ALLOCTO:
		{
			CStringArray aMatched, aMixed;
			m_ctrlAttributes.GetAllocTo(aMatched, aMixed);

			bChange = SetSelectedTaskArray(TDCA_ALLOCTO, m_tldAll.aAllocTo, aMatched, aMixed);
		}
		break;
		
	case TDCA_CATEGORY:
		{
			CStringArray aMatched, aMixed;
			m_ctrlAttributes.GetCategories(aMatched, aMixed);

			bChange = SetSelectedTaskArray(TDCA_CATEGORY, m_tldAll.aCategory, aMatched, aMixed);
		}
		break;
		
	case TDCA_TAGS:
		{
			CStringArray aMatched, aMixed;
			m_ctrlAttributes.GetTags(aMatched, aMixed);

			bChange = SetSelectedTaskArray(TDCA_TAGS, m_tldAll.aTags, aMatched, aMixed);
		}
		break;
		
	case TDCA_TIMEESTIMATE:
		{
			TDCTIMEPERIOD tp;
			m_ctrlAttributes.GetTimeEstimate(tp);

			bChange = SetSelectedTaskTimeEstimate(tp, FALSE, Misc::HasFlag(dwFlags, UTF_RECALCTIME));
		}
		break;
		
	case TDCA_TIMESPENT:
		{
			TDCTIMEPERIOD tp;
			m_ctrlAttributes.GetTimeSpent(tp);

			bChange = SetSelectedTaskTimeSpent(tp, FALSE, Misc::HasFlag(dwFlags, UTF_RECALCTIME));
		}
		break;
		
	case TDCA_FILELINK:
		{
			CStringArray aFiles;
			m_ctrlAttributes.GetFileLinks(aFiles);

			BOOL bAppend = (GetSelectedTaskCount() > 1);

			bChange = SetSelectedTaskFileLinks(aFiles, bAppend);
		}
		break;
		
	case TDCA_RECURRENCE:
	case TDCA_COLOR:
		ASSERT(0);
		break;

	default:
		// handle custom attributes
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		{
			CString sAttribID = m_aCustomAttribDefs.GetAttributeTypeID(nAttribID);
			TDCCADATA data;

			if (m_ctrlAttributes.GetCustomAttributeData(sAttribID, data))
				bChange = SetSelectedTaskCustomAttributeData(sAttribID, data);
			else
				bChange = SetSelectedTaskCustomAttributeData(sAttribID, TDCCADATA());
		}
	}

	return bChange;
}

BOOL CToDoCtrl::SetSelectedTaskCustomAttributeData(const CString& sAttribID, const TDCCADATA& data)
{
	if (!CanEditSelectedTask(TDCA_CUSTOMATTRIB))
		return FALSE;

	if (sAttribID.IsEmpty())
		return FALSE;

	Flush();
	
	IMPLEMENT_DATA_UNDO_EDIT(m_data);
		
	CDWordArray aModTaskIDs;
	POSITION pos = TSH().GetFirstItemPos();
	
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		HandleModResult(dwTaskID, m_data.SetTaskCustomAttributeData(dwTaskID, sAttribID, data), aModTaskIDs);
	}
	
	if (!aModTaskIDs.GetSize())
		return FALSE;

	SetModified(m_aCustomAttribDefs.GetAttributeID(sAttribID), aModTaskIDs);
	return TRUE;
}

BOOL CToDoCtrl::SetSelectedTaskMetaData(const CString& sKey, const CString& sMetaData)
{
	if (!CanEditSelectedTask(TDCA_METADATA))
		return FALSE;

	if (sKey.IsEmpty())
		return FALSE;

	Flush();
	
	IMPLEMENT_DATA_UNDO_EDIT(m_data);
		
	CDWordArray aModTaskIDs;
	POSITION pos = TSH().GetFirstItemPos();
	
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		HandleModResult(dwTaskID, m_data.SetTaskMetaData(dwTaskID, sKey, sMetaData), aModTaskIDs);
	}
	
	if (!aModTaskIDs.GetSize())
		return FALSE;

 	SetModified(TDCA_METADATA, aModTaskIDs);
	return TRUE;
}

int CToDoCtrl::GetAutoListData(TDC_ATTRIBUTE nAttribID, TDCAUTOLISTDATA& tld) const
{
	return tld.Copy(m_tldAll, nAttribID);
}

void CToDoCtrl::UpdateAutoListData(TDC_ATTRIBUTE nAttribID)
{
	m_ctrlAttributes.GetAutoListData(nAttribID, m_tldAll);
}

void CToDoCtrl::SetDefaultAutoListData(const TDCAUTOLISTDATA& tld)
{
	// update the combos before copying over the current defaults
	m_ctrlAttributes.SetDefaultAutoListData(tld);
	m_ctrlAttributes.GetAutoListData(TDCA_ALL, m_tldAll);

	m_tldDefault.Copy(tld, TDCA_ALL);
}

void CToDoCtrl::SetAutoListContentReadOnly(TDC_ATTRIBUTE nListAttribID, BOOL bReadOnly)
{
	m_ctrlAttributes.SetAutoListDataReadOnly(nListAttribID, bReadOnly);
}

BOOL CToDoCtrl::RenameTaskAttributeValues(TDC_ATTRIBUTE nListAttribID, const CString& sFrom, const CString& sTo, BOOL bCaseSensitive, BOOL bWholeWord)
{
	return (m_data.RenameTasksAttributeValue(nListAttribID, sFrom, sTo, bCaseSensitive, bWholeWord) == SET_CHANGE);
}

void CToDoCtrl::NewList()
{
	Flush();

	// Temporarily disable delete confirmation
	BOOL bConfirmDelete = HasStyle(TDCS_CONFIRMDELETE);
	
	if (bConfirmDelete)
		m_styles[TDCS_CONFIRMDELETE] = FALSE;
	
	DeleteAllTasks();
	
	if (bConfirmDelete)
		m_styles[TDCS_CONFIRMDELETE] = TRUE;

	m_sProjectName.Empty();
	m_nFileVersion = 0;
	m_bModified = FALSE;
	m_sPassword.Empty();
	
	UpdateData(FALSE);
	UpdateComments(CString(), CBinaryData());
}

BOOL CToDoCtrl::EditSelectedTaskColor()
{
	if (!CanEditSelectedTask(TDCA_COLOR))
		return FALSE;

	CEnColorDialog dialog(GetSelectedTaskColor());

	if (dialog.DoModal(CPreferences()) != IDOK)
		return FALSE;

	return SetSelectedTaskColor(dialog.GetColor());
}

BOOL CToDoCtrl::SetSelectedTaskColor(COLORREF color)
{
	if (!CanEditSelectedTask(TDCA_COLOR))
		return FALSE;

	Flush();
	
	IMPLEMENT_DATA_UNDO_EDIT(m_data);
		
	CDWordArray aModTaskIDs;
	POSITION pos = TSH().GetFirstItemPos();
	
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		HandleModResult(dwTaskID, m_data.SetTaskColor(dwTaskID, color), aModTaskIDs);
	}
	
	if (!aModTaskIDs.GetSize())
		return FALSE;

	SetModified(TDCA_COLOR, aModTaskIDs);
	return FALSE;
}

BOOL CToDoCtrl::EditSelectedTaskIcon()
{
	if (!CanEditSelectedTask(TDCA_ICON))
		return FALSE;

	CTDLTaskIconDlg dialog(m_ilTaskIcons, GetSelectedTaskIcon(), TRUE, -1, this);

	if (dialog.DoModal(s_hIconIconDlg) != IDOK)
		return FALSE;

	return SetSelectedTaskIcon(dialog.GetIconName());
}

LRESULT CToDoCtrl::OnTaskIconDlgReloadIcons(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if (m_ilTaskIcons.LoadImages(m_sLastSavePath, colorMagenta, HasStyle(TDCS_SHOWDEFAULTTASKICONS)))
	{
		OnTaskIconsChanged();
		return TRUE;
	}

	// else 
	return FALSE;
}

BOOL CToDoCtrl::ClearSelectedTaskIcon()
{
	if (!CanEditSelectedTask(TDCA_ICON))
		return FALSE;

	return SetSelectedTaskIcon(_T(""));
}

BOOL CToDoCtrl::SetSelectedTaskIcon(const CString& sIcon)
{
	if (!CanEditSelectedTask(TDCA_ICON))
		return FALSE;

	Flush();
	
	IMPLEMENT_DATA_UNDO_EDIT(m_data);
		
	CDWordArray aModTaskIDs;
	POSITION pos = TSH().GetFirstItemPos();
	
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		HandleModResult(dwTaskID, m_data.SetTaskIcon(dwTaskID, sIcon), aModTaskIDs);
	}
	
	if (!aModTaskIDs.GetSize())
		return FALSE;

	SetModified(TDCA_ICON, aModTaskIDs);
	return TRUE;
}

BOOL CToDoCtrl::CanPasteText() const
{
	TDC_ATTRIBUTE nAttribID = GetFocusedControlAttribute();
	
	if (!CanEditSelectedTask(nAttribID))
		return FALSE;

	if (CWinClasses::IsEditControl(::GetFocus()))
		return TRUE;

	// Special case
	if (nAttribID == TDCA_COMMENTS)
		return HasFocus(TDCSF_COMMENTS);

	return FALSE;
}

BOOL CToDoCtrl::CanPasteDateTime() const
{
	if (!CanPasteText())
		return FALSE;

	// Exclude some numeric fields
	TDC_ATTRIBUTE nAttribID = GetFocusedControlAttribute();

	switch (nAttribID)
	{
	case TDCA_COST:
	case TDCA_TIMEESTIMATE:
	case TDCA_TIMESPENT:
	case TDCA_PERCENT:
		return FALSE;

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
			return (m_aCustomAttribDefs.GetAttributeDataType(nAttribID) == TDCCA_STRING);
		break;
	}

	// All else
	return TRUE;
}

BOOL CToDoCtrl::PasteText(const CString& sText)
{
	if (!CanPasteText() || sText.IsEmpty())
		return FALSE;

	if (GetFocusedControlAttribute() == TDCA_COMMENTS)
		return m_ctrlComments.InsertTextContent(sText, FALSE);

	// else
	HWND hFocus = ::GetFocus();
	::SendMessage(hFocus, EM_REPLACESEL, TRUE, (LPARAM)(LPCTSTR)sText);
	
	return TRUE;
}

void CToDoCtrl::BeginSelectedTaskEdit()
{
	ASSERT(!m_bInSelectedTaskEdit);

	m_bInSelectedTaskEdit = TRUE;

	// Aggregate all mods in a single undo operation
	VERIFY(m_data.BeginNewUndoAction(TDCUAT_EDIT));
}

void CToDoCtrl::EndSelectedTaskEdit()
{
	ASSERT(m_bInSelectedTaskEdit);

	m_bInSelectedTaskEdit = FALSE;

	VERIFY(m_data.EndCurrentUndoAction());
}

BOOL CToDoCtrl::SetSelectedTaskComments(const CString& sComments, const CBinaryData& customComments)
{
	return SetSelectedTaskComments(sComments, customComments, FALSE); 
}

BOOL CToDoCtrl::SetSelectedTaskComments(const CString& sComments, const CBinaryData& customComments, BOOL bInternal)
{
	if (!CanEditSelectedTask(TDCA_COMMENTS))
		return FALSE;

	if (!bInternal)
		Flush();
	
	IMPLEMENT_DATA_UNDO_EDIT(m_data);
		
	CDWordArray aModTaskIDs;
	POSITION pos = TSH().GetFirstItemPos();

	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		HandleModResult(dwTaskID, m_data.SetTaskComments(dwTaskID, sComments, customComments), aModTaskIDs);
	}
	
	if (!aModTaskIDs.GetSize())
		return FALSE;

	// refresh the comments of the active task if we were called externally
	// note: we don't use SetTextChange because that doesn't handle custom comments
	if (!bInternal && (TSH().GetCount() == 1))
		UpdateComments(GetSelectedTaskComments(), GetSelectedTaskCustomComments(m_cfComments));

	TSH().InvalidateAll();

	SetModified(TDCA_COMMENTS, aModTaskIDs);
	return TRUE;
}

BOOL CToDoCtrl::SetSelectedTaskTitle(const CString& sTitle, BOOL bAllowMultiple)
{
	if (sTitle.IsEmpty()) // sanity check
		return FALSE;

	if (!CanEditSelectedTask(TDCA_TASKNAME))
		return FALSE;

	if (!bAllowMultiple && (GetSelectedTaskCount() > 1))
		return FALSE;

	Flush();
	
	IMPLEMENT_DATA_UNDO_EDIT(m_data);
		
	CDWordArray aModTaskIDs;
	POSITION pos = TSH().GetFirstItemPos();

	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		HandleModResult(dwTaskID, m_data.SetTaskTitle(dwTaskID, sTitle), aModTaskIDs);
	}

	if (!aModTaskIDs.GetSize())
		return FALSE;

	m_taskTree.Tree().SetItemText(GetSelectedItem(), sTitle);
	m_taskTree.InvalidateSelection();

	SetModified(TDCA_TASKNAME, aModTaskIDs);
	return TRUE;
}

BOOL CToDoCtrl::GetSelectionBoundingRect(CRect& rSelection) const
{
	if (!m_taskTree.GetSelectionBoundingRect(rSelection))
		return FALSE;

	m_taskTree.MapWindowPoints((CWnd*)this, rSelection);
	return TRUE;
}

COleDateTime CToDoCtrl::GetTaskDate(DWORD dwTaskID, TDC_DATE nDate) const
{
	return m_data.GetTaskDate(dwTaskID, nDate);
}

BOOL CToDoCtrl::GetTaskTimes(DWORD dwTaskID, TDCTIMEPERIOD& timeEst, TDCTIMEPERIOD& timeSpent) const
{
	if (!HasTask(dwTaskID))
		return FALSE;

	m_data.GetTaskTimeEstimate(dwTaskID, timeEst);
	m_data.GetTaskTimeSpent(dwTaskID, timeSpent);

	return TRUE;
}

BOOL CToDoCtrl::GetTaskTextColors(DWORD dwTaskID, COLORREF& crText, COLORREF& crBack, BOOL bSelected) const 
{ 
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;

	if (!m_data.GetTask(dwTaskID, pTDI, pTDS))
	{
		ASSERT(0);
		return FALSE;
	}

	return m_taskTree.GetTaskTextColors(pTDI, pTDS, crText, crBack, -1, bSelected);
}

BOOL CToDoCtrl::SetSelectedTaskPriority(int nPriority, BOOL bOffset)
{
	if (!CanEditSelectedTask(TDCA_PRIORITY))
		return FALSE;

	Flush();

	IMPLEMENT_DATA_UNDO_EDIT(m_data);
		
	CDWordArray aModTaskIDs;
	POSITION pos = TSH().GetFirstItemPos();

	// Keep track of what we've processed to avoid offsetting
	// the same task multiple times via references
	CDWordSet mapProcessed;

	while (pos)
	{
		DWORD dwTaskID = GetTrueTaskID(TSH().GetNextItem(pos));

		if (mapProcessed.Has(dwTaskID))
			continue;

		HandleModResult(dwTaskID, m_data.SetTaskPriority(dwTaskID, nPriority, bOffset), aModTaskIDs);
		mapProcessed.Add(dwTaskID);
	}
	
	if (!aModTaskIDs.GetSize())
		return FALSE;
		
	SetModified(TDCA_PRIORITY, aModTaskIDs);
	return TRUE;
}

BOOL CToDoCtrl::SetSelectedTaskRisk(int nRisk, BOOL bOffset)
{
	if (!CanEditSelectedTask(TDCA_RISK))
		return FALSE;

	Flush();
	
	IMPLEMENT_DATA_UNDO_EDIT(m_data);
		
	CDWordArray aModTaskIDs;
	POSITION pos = TSH().GetFirstItemPos();
	
	// Keep track of what we've processed to avoid offsetting
	// the same task multiple times via references
	CDWordSet mapProcessed;

	while (pos)
	{
		DWORD dwTaskID = GetTrueTaskID(TSH().GetNextItem(pos));

		if (mapProcessed.Has(dwTaskID))
			continue;

		HandleModResult(dwTaskID, m_data.SetTaskRisk(dwTaskID, nRisk, bOffset), aModTaskIDs);
		mapProcessed.Add(dwTaskID);
	}
	
	if (!aModTaskIDs.GetSize())
		return FALSE;
		
	SetModified(TDCA_RISK, aModTaskIDs);
	return TRUE;
}

BOOL CToDoCtrl::SetSelectedTaskFlag(BOOL bFlagged)
{
	if (!CanEditSelectedTask(TDCA_FLAG))
		return FALSE;

	Flush();
	
	IMPLEMENT_DATA_UNDO_EDIT(m_data);
		
	CDWordArray aModTaskIDs;
	POSITION pos = TSH().GetFirstItemPos();

	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		HandleModResult(dwTaskID, m_data.SetTaskFlag(dwTaskID, bFlagged), aModTaskIDs);
	}
	
	if (!aModTaskIDs.GetSize())
		return FALSE;

	SetModified(TDCA_FLAG, aModTaskIDs);
	return TRUE;
}

BOOL CToDoCtrl::SetSelectedTaskLock(BOOL bLocked)
{
	if (!CanEditSelectedTask(TDCA_LOCK))
		return FALSE;

	Flush();
	
	IMPLEMENT_DATA_UNDO_EDIT(m_data);
		
	CDWordArray aModTaskIDs;
	POSITION pos = TSH().GetFirstItemPos();

	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		HandleModResult(dwTaskID, m_data.SetTaskLock(dwTaskID, bLocked), aModTaskIDs);
	}
	
	if (!aModTaskIDs.GetSize())
		return FALSE;

	SetModified(TDCA_LOCK, aModTaskIDs);
	return TRUE;
}

BOOL CToDoCtrl::IncrementSelectedTaskPriority(BOOL bUp)
{
	return SetSelectedTaskPriority((bUp ? 1 : -1), TRUE); // offset
}

// external version
BOOL CToDoCtrl::SetSelectedTaskDate(TDC_DATE nDate, const COleDateTime& date)
{
	// special case
	if (nDate == TDCD_DONE)
		return SetSelectedTaskCompletion(date);

	TDC_ATTRIBUTE nAttribID = TDC::MapDateToAttribute(nDate);

	if (!CanEditSelectedTask(nAttribID))
		return FALSE;

	Flush();

	IMPLEMENT_DATA_UNDO_EDIT(m_data);
		
	CDWordArray aModTaskIDs;
	POSITION pos = TSH().GetFirstItemPos();

	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		HandleModResult(dwTaskID, m_data.SetTaskDate(dwTaskID, nDate, date), aModTaskIDs);
	}
	
	if (!aModTaskIDs.GetSize())
		return FALSE;

	switch (nDate)
	{
	case TDCD_CREATE:
		SetModified(TDCA_CREATIONDATE, aModTaskIDs);
		break;

	case TDCD_START:
	case TDCD_STARTDATE:
	case TDCD_STARTTIME:
		SetModified(TDCA_STARTDATE, aModTaskIDs);
		break;

	case TDCD_DUE:
	case TDCD_DUEDATE:
	case TDCD_DUETIME:
		SetModified(TDCA_DUEDATE, aModTaskIDs);
		break;

	case TDCD_DONETIME:
		SetModified(TDCA_DONEDATE, aModTaskIDs);
		break;

	//case TDCD_DONE:
	default:
		ASSERT(0);
		return FALSE;
	}

	return TRUE;
}

BOOL CToDoCtrl::CanOffsetSelectedTaskDates(const CTDCDateSet& mapDates) const
{
	if (mapDates.IsEmpty())
	{
		ASSERT(0);
		return FALSE;
	}

	BOOL bCanAdjustDependDates = (!m_taskTree.SelectionHasDependencies() || !HasStyle(TDCS_AUTOADJUSTDEPENDENCYDATES));
	POSITION pos = mapDates.GetStartPosition();

	while (pos)
	{
		TDC_DATE nDate = mapDates.GetNext(pos);

		if (!CanEditSelectedTask(TDC::MapDateToAttribute(nDate)))
			return FALSE;

		switch (nDate)
		{
		case TDCD_CREATE:
		case TDCD_DONE:
		case TDCD_DONEDATE:
		case TDCD_DONETIME:
			break;

		case TDCD_START:
		case TDCD_DUE:
		case TDCD_STARTDATE:
		case TDCD_DUEDATE:
		case TDCD_STARTTIME:
		case TDCD_DUETIME:
			if (!bCanAdjustDependDates)
				return FALSE;
			break;

		default:
			ASSERT(0);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CToDoCtrl::OffsetSelectedTaskDates(const CTDCDateSet& mapDates, int nAmount, TDC_UNITS nUnits, DWORD dwFlags)
{
	if (!CanOffsetSelectedTaskDates(mapDates))
		return FALSE;

	Flush();

	IMPLEMENT_DATA_UNDO_EDIT(m_data);

	// remove duplicate subtasks if we're going to be processing subtasks anyway
	CHTIList htiSel;
	TSH().CopySelection(htiSel, (dwFlags & TDCOTD_OFFSETSUBTASKS));

	CDWordArray aModTaskIDs;
	CTDCAttributeMap mapAttribs;

	POSITION posDate = mapDates.GetStartPosition();

	while (posDate)
	{
		CDWordArray aDateModTaskIDs;
		TDC_DATE nDate = mapDates.GetNext(posDate);

		POSITION posTask = htiSel.GetHeadPosition();

		while (posTask)
		{
			DWORD dwTaskID = GetTrueTaskID(htiSel.GetNext(posTask));
			CDWordArray aSelModTaskIDs;

			TDC_SET nRes = m_data.OffsetTaskDate(dwTaskID,
												 nDate,
												 nAmount,
												 nUnits,
												 dwFlags,
												 aSelModTaskIDs);

			aDateModTaskIDs.Append(aSelModTaskIDs);
		}

		if (aDateModTaskIDs.GetSize())
		{
			mapAttribs.Add(TDC::MapDateToAttribute(nDate));
			Misc::AppendItems(aDateModTaskIDs, aModTaskIDs, TRUE);
		}
	}

	if (!aModTaskIDs.GetSize())
		return FALSE;

	SetModified(mapAttribs, aModTaskIDs, TRUE);
	UpdateControls(FALSE); // don't update comments

	return TRUE;
}

void CToDoCtrl::SetInheritedParentAttributes(const CTDCAttributeMap& mapAttribs, BOOL bUpdateAttrib)
{
	m_data.SetInheritedParentAttributes(mapAttribs, bUpdateAttrib);
}

BOOL CToDoCtrl::CanSetSelectedTasksDone(const CTDCTaskCompletionArray& aTasks, BOOL& bAndSubtasks) /*const*/
{
	// Sanity checks
	if (aTasks.GetSize() == 0)
	{
		ASSERT(0);
		return FALSE;
	}
	
	if (!CanEditSelectedTask(TDCA_DONEDATE))
		return FALSE;

	bAndSubtasks = FALSE;

	// If there are no items to be completed then no further restrictions
	CDWordArray aToDoIDs;

	if (aTasks.GetTaskIDsForCompletion(aToDoIDs) == 0)
		return TRUE;

	// check for circular dependencies
	CDWordArray aCircularIDs;
	int nID;

	for (nID = 0; nID < aToDoIDs.GetSize(); nID++)
	{
		DWORD dwTaskID = aToDoIDs[nID];

		if (m_data.TaskHasLocalCircularDependencies(dwTaskID))
			aCircularIDs.Add(dwTaskID);
	}

	if (aCircularIDs.GetSize() > 0)
	{
		CEnString sMessage(IDS_TASKCOMPLETION);
		sMessage += '|';

		if (aCircularIDs.GetSize() == 1)
			sMessage += CEnString(IDS_TDC_SELTASKHASCIRCULARDEPENDENCY);
		else
			sMessage += CEnString(IDS_TDC_SELTASKSHAVECIRCULARDEPENDENCIES, Misc::FormatArray(aToDoIDs));
		
		AfxMessageBox(sMessage, MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// check for incomplete dependents
	CString sIncomplete;

	for (nID = 0; nID < aToDoIDs.GetSize(); nID++)
	{
		DWORD dwTaskID = aToDoIDs[nID];

		if (TaskHasIncompleteDependencies(dwTaskID, sIncomplete))
		{
			CEnString sMessage(IDS_TASKCOMPLETION);
			sMessage += '|';
			sMessage += CEnString(IDS_TDC_SELTASKHASDEPENDENCY);

			if (IDYES == AfxMessageBox(sMessage, MB_YESNO | MB_ICONERROR))
				ShowTaskLink(sIncomplete, FALSE);

			return FALSE;
		}
	}

	bAndSubtasks = (!m_data.WantUpdateInheritedAttibute(TDCA_DONEDATE) &&
					CheckWantTaskSubtasksCompleted(aToDoIDs));

	return TRUE;
}

BOOL CToDoCtrl::TaskHasIncompleteDependencies(DWORD dwTaskID, CString& sIncomplete) const
{
	CTDCDependencyArray aDepends;
	int nNumDepends = m_data.GetTaskDependencies(dwTaskID, aDepends);

	for (int nDepend = 0; nDepend < nNumDepends; nDepend++)
	{
		const TDCDEPENDENCY& depend = aDepends[nDepend];

		// see if dependent is one of 'our' tasks
		if (depend.IsLocal())
		{
			if (m_data.HasTask(depend.dwTaskID) && !m_data.IsTaskDone(depend.dwTaskID))
			{
				sIncomplete = depend.Format();
				return TRUE;
			}
		}
		else if (!depend.sTasklist.IsEmpty()) // pass to parent if we can't handle
		{
			BOOL bDependentIsDone = GetParent()->SendMessage(WM_TDCM_ISTASKDONE, depend.dwTaskID, (LPARAM)(LPCTSTR)depend.sTasklist);

			if (!bDependentIsDone)
			{
				sIncomplete = depend.Format();
				return TRUE;
			}
		}
	}

	// check this tasks subtasks
	const TODOSTRUCTURE* pTDS = m_data.LocateTask(dwTaskID);
	ASSERT(pTDS);

	if (pTDS && pTDS->HasSubTasks())
	{
		int nPos = pTDS->GetSubTaskCount();

		while (nPos--)
		{
			if (TaskHasIncompleteDependencies(pTDS->GetSubTaskID(nPos), sIncomplete))
				return TRUE;
		}
	}

	return FALSE;
}

BOOL CToDoCtrl::CheckWantTaskSubtasksCompleted(const CDWordArray& aTaskIDs) const
{
	// If the specified tasks are both recurring AND reusable
	// AND none of the specified task's subtasks are recurring
	// then we don't need to ask because we are going to reset
	// the subtasks' completion states anyway
	BOOL bNeedAsk = FALSE;
	int nTask = aTaskIDs.GetSize();

	while (nTask-- && !bNeedAsk)
	{
		DWORD dwTaskID = aTaskIDs[nTask];

		if (m_data.IsTaskDone(aTaskIDs[nTask]))
			continue;

		if (m_data.TaskHasIncompleteSubtasks(dwTaskID, TRUE))
		{
			bNeedAsk = (!m_data.IsTaskReusableRecurring(dwTaskID) ||
						m_data.TaskHasRecurringSubtasks(dwTaskID));
		}
	}

	if (!bNeedAsk)
		return 0;

	// Do the asking
	CEnString sMessage(IDS_TASKCOMPLETION);
	sMessage += '|';

	if (aTaskIDs.GetSize() == 1)
		sMessage += CEnString(IDS_TDC_SELTASKHASINCOMPLETE);
	else
		sMessage += CEnString(IDS_TDC_TASKHASINCOMPLETE);

	return (IDYES == AfxMessageBox(sMessage, MB_YESNO | MB_ICONQUESTION));
}

BOOL CToDoCtrl::SetSelectedTaskCompletion(TDC_TASKCOMPLETION nCompletion)
{
	if (nCompletion == TDCTC_UNDONE)
		return SetSelectedTaskCompletion(CDateHelper::NullDate());

	CDWordArray aTaskIDs;
	DWORD dwUnused;

	if (!GetSelectedTaskIDs(aTaskIDs, dwUnused, FALSE))
		return FALSE;

	CTDCTaskCompletionArray aTasks(m_data, m_sCompletionStatus);

	if (nCompletion == TDCTC_TOGGLE)
	{
		if (!aTasks.Toggle(aTaskIDs))
			return FALSE;
	}
	else
	{
		ASSERT(nCompletion == TDCTC_DONE);

		// Weed out already completed tasks else their dates
		// will get changed which we don't want
		for (int nID = 0; nID < aTaskIDs.GetSize(); nID++)
		{
			if (!m_data.IsTaskDone(aTaskIDs[nID]))
				aTasks.Add(aTaskIDs[nID], COleDateTime::GetCurrentTime());
		}

		// There should be at least one undone task
		ASSERT(aTaskIDs.GetSize());
	}

	return SetSelectedTaskCompletion(aTasks);
}

BOOL CToDoCtrl::SetSelectedTaskCompletion(const COleDateTime& date)
{
	Flush();

	CDWordArray aTaskIDs;
	DWORD dwUnused;

	if (!GetSelectedTaskIDs(aTaskIDs, dwUnused, CDateHelper::IsDateSet(date)))
		return FALSE;

	CTDCTaskCompletionArray aTasks(m_data, m_sCompletionStatus);

	if (!aTasks.Add(aTaskIDs, date))
		return FALSE;

	return SetSelectedTaskCompletion(aTasks);
}

BOOL CToDoCtrl::SetSelectedTaskCompletion(const CTDCTaskCompletionArray& aTasks)
{
	BOOL bAndSubtasks = FALSE;

	if (!CanSetSelectedTasksDone(aTasks, bAndSubtasks))
		return FALSE;

	if (m_aRecreateTaskIDs.GetSize())
	{
		// Should have been cleared last time it was used
		ASSERT(0);
		m_aRecreateTaskIDs.RemoveAll();
	}

	IMPLEMENT_DATA_UNDO_EDIT(m_data);

	CDWordArray aModTaskIDs;

	for (int nSel = 0; nSel < aTasks.GetSize(); nSel++)
	{
		if (SetSelectedTaskCompletion(aTasks[nSel], bAndSubtasks))
			aModTaskIDs.Add(aTasks[nSel].dwID);
	}

	if (!aModTaskIDs.GetSize())
	{
		ASSERT(!m_aRecreateTaskIDs.GetSize());
		return FALSE;
	}

	// If some of the tasks were recurring and need to be created
	// we do that after this operation ends
	if (m_aRecreateTaskIDs.GetSize())
	{
		// Don't recalc column widths until after the new tasks are created
		PostMessage(WM_TDC_RECREATERECURRINGTASK, 0, m_aRecreateTaskIDs.GetSize());
	}

	SetModified(TDCA_DONEDATE, aModTaskIDs);
	return TRUE;
}

BOOL CToDoCtrl::SetSelectedTaskCompletion(const TDCTASKCOMPLETION& task, BOOL bAndSubtasks)
{
	ASSERT(!bAndSubtasks || task.IsDone());

	DWORD dwTaskID = task.dwID;
	BOOL bReuse = FALSE, bRecreate = FALSE, bChange = FALSE;

	if (task.bStateChange && task.IsDone())
	{
		if (m_timeTracking.IsTrackingTaskOrSubtask(dwTaskID, FALSE))
			EndTimeTracking(TRUE, TRUE);

		// Handle recurring tasks if changing state to completed
		TDCRECURRENCE tr;
		BOOL bRecurring = (m_data.CanTaskRecur(dwTaskID) && m_data.GetTaskRecurrence(dwTaskID, tr));

		if (bRecurring)
		{
			switch (tr.nReuse)
			{
			case TDIRO_REUSE:
				bReuse = TRUE;
				break;

			case TDIRO_CREATE:
				bRecreate = TRUE;
				break;

			case TDIRO_ASK:
				{
					CTDLReuseRecurringTaskDlg dialog(tr.bPreserveComments);

					if (dialog.DoModal(s_hIconRecurDlg) == IDOK)
					{
						if (dialog.GetWantReuseTask())
							bReuse = TRUE;
						else
							bRecreate = TRUE;

						// Update the task's 'preserve comments' flag
						// for later initialisation
						tr.bPreserveComments = dialog.GetWantPreserveComments();
						m_data.SetTaskRecurrence(dwTaskID, tr);
					}
					else
					{
						return FALSE;
					}
				}
				break;
			}
		}
	}

	// FALSE == Don't update the dates of any already-completed subtasks
	if (m_data.SetTaskDone(dwTaskID, task.dtDone, bAndSubtasks, FALSE) == SET_CHANGE)
	{
		bChange = TRUE;

		// Post-processing for recurring tasks
		if (bReuse)
		{
			COleDateTime dtNext;
			BOOL bDueDate = TRUE;

			VERIFY(m_data.GetNextTaskOccurrence(dwTaskID, dtNext, bDueDate));
			InitialiseNewRecurringTask(dwTaskID, dwTaskID, dtNext, bDueDate);

			// notify parent
			GetParent()->SendMessage(WM_TDCN_RECREATERECURRINGTASK, dwTaskID, dwTaskID);
		}
		else if (bRecreate)
		{
			m_aRecreateTaskIDs.Add(dwTaskID);
		}

		// We only need update the state of the task if not reusing
		// ie. Recreating or NOT recurring
		if (task.bStateChange && !bReuse)
		{
			// Update status and status of subtasks as required
			if ((m_data.SetTaskStatus(dwTaskID, task.sStatus) == SET_CHANGE) && bAndSubtasks)
			{
				m_data.ApplyLastChangeToSubtasks(dwTaskID, TDCA_STATUS);
			}

			// Don't update subtask percent so that if the task is
			// later undone, the previous value is preserved
			m_data.SetTaskPercent(dwTaskID, task.nPercent);
		}
	}

	ASSERT(bChange || !task.bStateChange);

	return bChange;
}

LRESULT CToDoCtrl::OnRecreateRecurringTask(WPARAM /*wParam*/, LPARAM lParam)
{
	int nNumTasks = lParam;

	// sanity checks
	if (!nNumTasks || (nNumTasks != m_aRecreateTaskIDs.GetSize()))
	{
		ASSERT(0);
	}
	else
	{
		// Always extend the previous undo action which completed
		// the recurring task(s) we are about to recreate
		IMPLEMENT_DATA_UNDO_EXTEND(m_data, TDCUAT_ADD, TRUE);

		CDWordArray aTaskIDs, aNewTaskIDs;

		for (int nTask = 0; nTask < nNumTasks; nTask++)
		{
			DWORD dwTaskID = m_aRecreateTaskIDs[nTask];

			// next occurrence can fail if we've run out of occurrences
			COleDateTime dtNext;
			BOOL bDueDate = TRUE;

			if (!m_data.GetNextTaskOccurrence(dwTaskID, dtNext, bDueDate))
				continue;

			CTaskFile task;
			PrepareTaskfileForTasks(task, TDCGT_ALL);
		
			VERIFY(m_exporter.ExportTask(dwTaskID, task, NULL, FALSE));

			DWORD dwNewTaskID = RecreateRecurringTaskInTree(task, dtNext, bDueDate);

			if (dwNewTaskID)
			{
				// Save off taskIDs for the end
				aTaskIDs.Add(dwTaskID);
				aNewTaskIDs.Add(dwNewTaskID);
			}
		}

		// mark as changed
		if (aNewTaskIDs.GetSize())
		{
			SelectTasks(aNewTaskIDs);
			SetModified(TDCA_NEWTASK, aNewTaskIDs);

			// notify parent of all new tasks
			int nTask = aTaskIDs.GetSize();

			while (nTask--)
				GetParent()->SendMessage(WM_TDCN_RECREATERECURRINGTASK, aTaskIDs[nTask], aNewTaskIDs[nTask]);
		}
	}

	// always
	m_aRecreateTaskIDs.RemoveAll();

	return 0L;
}

DWORD CToDoCtrl::RecreateRecurringTaskInTree(const CTaskFile& task, const COleDateTime& dtNext, BOOL bDueDate)
{
	HTASKITEM hTask = task.GetFirstTask();
	DWORD dwTaskID = task.GetTaskID(hTask); // existing task ID

	// insert below existing item
	HTREEITEM hti = m_taskTree.GetItem(dwTaskID);
	ASSERT(hti);

	HTREEITEM htiParent = m_taskTree.GetParentItem(hti);
	HTREEITEM htiNew = PasteTaskToTree(task, hTask, htiParent, hti, TDCR_YES, TRUE);

	DWORD dwNewTaskID = GetTaskID(htiNew);
	InitialiseNewRecurringTask(dwTaskID, dwNewTaskID, dtNext, bDueDate);

	return dwNewTaskID;
}

void CToDoCtrl::InitialiseNewRecurringTask(DWORD dwPrevTaskID, DWORD dwNewTaskID, const COleDateTime& dtNext, BOOL bDueDate)
{
	VERIFY(m_data.InitialiseNewRecurringTask(dwPrevTaskID, dwNewTaskID, dtNext, bDueDate));

	// optionally clear the comments field
	TDCRECURRENCE tr;

	if (!m_data.GetTaskRecurrence(dwNewTaskID, tr) || !tr.bPreserveComments)
	{
		UpdateComments(CString(), CBinaryData());
	}
}

BOOL CToDoCtrl::CanSetSelectedTaskPercentDone(BOOL bToToday) const
{
	if (!CanEditSelectedTask(TDCA_PERCENT))
		return FALSE;

	// Extra date processing
	if (bToToday)
	{
		if (HasStyle(TDCS_AUTOCALCPERCENTDONE))
			return FALSE;

		if (m_taskTree.SelectionHasParents() && 
			HasStyle(TDCS_AVERAGEPERCENTSUBCOMPLETION))
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CToDoCtrl::CanSetSelectedTaskPercentDoneToToday() const
{
	return CanSetSelectedTaskPercentDone(TRUE);
}

BOOL CToDoCtrl::SetSelectedTaskPercentDoneToToday()
{
	return SetSelectedTaskPercentDone(-1, FALSE, CDateHelper::GetEndOfDay(COleDateTime::GetCurrentTime()));
}

// External
BOOL CToDoCtrl::SetSelectedTaskPercentDone(int nPercent, BOOL bOffset)
{
	return SetSelectedTaskPercentDone(nPercent, bOffset, CDateHelper::NullDate());
}

// Internal
BOOL CToDoCtrl::SetSelectedTaskPercentDone(int nPercent, BOOL bOffset, const COleDateTime& date)
{
	// Sanity check
	BOOL bDateIsvalid = CDateHelper::IsDateSet(date);

	ASSERT((bDateIsvalid && (nPercent == -1) && !bOffset) || 
			(!bDateIsvalid && (bOffset || (nPercent >= 0))));

	if (!CanSetSelectedTaskPercentDone(bDateIsvalid))
		return FALSE;

	Flush();

	IMPLEMENT_DATA_UNDO_EDIT(m_data);
	
	// Keep track of what we've processed to avoid offsetting
	// the same task multiple times via references
	CDWordSet mapProcessed;

	// Percent edits can cause completion changes
	CTDCTaskCompletionArray aTasksForCompletion(m_data, m_sCompletionStatus);

	CDWordArray aModTaskIDs;
	POSITION pos = TSH().GetFirstItemPos();

	while (pos)
	{
		DWORD dwTaskID = GetTrueTaskID(TSH().GetNextItem(pos));

		if (mapProcessed.Has(dwTaskID))
			continue;

		int nTaskPercent = nPercent;

		if (bDateIsvalid)
		{
			ASSERT(!bOffset);
			COleDateTimeRange dtStartDue;

			if (!m_data.GetTaskStartDueDates(dwTaskID, dtStartDue))
				continue;

			nTaskPercent = (int)(dtStartDue.CalcProportion(date) * 100);
		}
		else if (bOffset)
		{
			nTaskPercent = Misc::GetNextValue(m_data.GetTaskPercent(dwTaskID), nPercent);
		}

		nTaskPercent = min(nTaskPercent, 100);
		nTaskPercent = max(nTaskPercent, 0);

		if (!aTasksForCompletion.Add(dwTaskID, nTaskPercent))
			HandleModResult(dwTaskID, m_data.SetTaskPercent(dwTaskID, nTaskPercent), aModTaskIDs);

		mapProcessed.Add(dwTaskID);
	}

	if (aTasksForCompletion.GetSize())
	{
		if (!SetSelectedTaskCompletion(aTasksForCompletion))
			return FALSE;

		aTasksForCompletion.GetTaskIDs(aModTaskIDs, TRUE);
		SetModified(TDCA_DONEDATE, aModTaskIDs);

		return TRUE;
	}

	if (!aModTaskIDs.GetSize())
		return FALSE;

	SetModified(TDCA_PERCENT, aModTaskIDs);
	return TRUE;
}

BOOL CToDoCtrl::SetSelectedTaskCost(const TDCCOST& cost, BOOL bOffset)
{
	if (!CanEditSelectedTask(TDCA_COST))
		return FALSE;

	Flush();
	
	IMPLEMENT_DATA_UNDO_EDIT(m_data);
		
	CDWordArray aModTaskIDs;
	POSITION pos = TSH().GetFirstItemPos();
	
	// Keep track of what we've processed to avoid offsetting
	// the same task multiple times via references
	CDWordSet mapProcessed;

	while (pos)
	{
		DWORD dwTaskID = GetTrueTaskID(TSH().GetNextItem(pos));

		if (mapProcessed.Has(dwTaskID))
			continue;

		HandleModResult(dwTaskID, m_data.SetTaskCost(dwTaskID, cost, bOffset), aModTaskIDs);
		mapProcessed.Add(dwTaskID);
	}

	if (!aModTaskIDs.GetSize())
		return FALSE;

	SetModified(TDCA_COST, aModTaskIDs);
	return TRUE;
}

BOOL CToDoCtrl::SetSelectedTaskRecurrence(const TDCRECURRENCE& tr)
{
	if (!CanEditSelectedTask(TDCA_RECURRENCE))
		return FALSE;

	Flush();
	
	IMPLEMENT_DATA_UNDO_EDIT(m_data);

	CDWordArray aModTaskIDs;
	POSITION pos = TSH().GetFirstItemPos();
	
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		HandleModResult(dwTaskID, m_data.SetTaskRecurrence(dwTaskID, tr), aModTaskIDs);
	}
	
	if (!aModTaskIDs.GetSize())
		return FALSE;

	// cache the last top-level options
	if (tr.IsRecurring())
	{
		m_nDefRecurFrom = tr.nRecalcFrom;
		m_nDefRecurReuse = tr.nReuse;
	}
		
	SetModified(TDCA_RECURRENCE, aModTaskIDs);
	return TRUE;
}

void CToDoCtrl::SetPercentDoneIncrement(int nAmount)
{
	m_nPercentIncrement = abs(nAmount);
	m_nPercentIncrement = max(1, min(50, m_nPercentIncrement));

	if (m_ctrlAttributes.GetSafeHwnd())
		m_ctrlAttributes.SetPercentDoneIncrement(m_nPercentIncrement);
}

BOOL CToDoCtrl::IncrementSelectedTaskPercentDone(BOOL bUp)
{
	return SetSelectedTaskPercentDone((bUp ? m_nPercentIncrement : -m_nPercentIncrement), TRUE);
}

// External
BOOL CToDoCtrl::SetSelectedTaskTimeEstimate(const TDCTIMEPERIOD& timeEst, BOOL bOffset)
{
	return SetSelectedTaskTimeEstimate(timeEst, bOffset, FALSE);
}

// Internal
BOOL CToDoCtrl::SetSelectedTaskTimeEstimate(const TDCTIMEPERIOD& timeEst, BOOL bOffset, BOOL bRecalcTime)
{
	if (!CanEditSelectedTask(TDCA_TIMEESTIMATE))
		return FALSE;

	Flush();
	
	IMPLEMENT_DATA_UNDO_EDIT(m_data);
		
	CDWordArray aModTaskIDs;
	POSITION pos = TSH().GetFirstItemPos();
	
	// Keep track of what we've processed to avoid offsetting
	// the same task multiple times via references
	CDWordSet mapProcessed;

	while (pos)
	{
		DWORD dwTaskID = GetTrueTaskID(TSH().GetNextItem(pos));

		if (mapProcessed.Has(dwTaskID))
			continue;

		// ignore parent tasks
		if (m_data.IsTaskParent(dwTaskID) && !HasStyle(TDCS_ALLOWPARENTTIMETRACKING))
			continue;

		if (bRecalcTime) // Just changing the units
 		{
			TDCTIMEPERIOD tpCur;
			m_data.GetTaskTimeEstimate(dwTaskID, tpCur);

			ASSERT(tpCur.nUnits != timeEst.nUnits);

			if (tpCur.SetUnits(timeEst.nUnits, TRUE))
				HandleModResult(dwTaskID, m_data.SetTaskTimeEstimate(dwTaskID, tpCur), aModTaskIDs);
		}
		else // Set or Offset
		{
			HandleModResult(dwTaskID, m_data.SetTaskTimeEstimate(dwTaskID, timeEst, bOffset), aModTaskIDs);
		}

		mapProcessed.Add(dwTaskID);
	}

	if (!aModTaskIDs.GetSize())
		return FALSE;

	SetModified(TDCA_TIMEESTIMATE, aModTaskIDs);
	return TRUE;
}

// External
BOOL CToDoCtrl::SetSelectedTaskTimeSpent(const TDCTIMEPERIOD& timeSpent, BOOL bOffset)
{
	return SetSelectedTaskTimeSpent(timeSpent, bOffset, FALSE);
}

// Internal
BOOL CToDoCtrl::SetSelectedTaskTimeSpent(const TDCTIMEPERIOD& timeSpent, BOOL bOffset, BOOL bRecalcTime)
{
	if (!CanEditSelectedTask(TDCA_TIMESPENT))
		return FALSE;

	Flush();
	
	IMPLEMENT_DATA_UNDO_EDIT(m_data);
		
	CDWordArray aModTaskIDs;
	POSITION pos = TSH().GetFirstItemPos();
	
	// Keep track of what we've processed to avoid offsetting
	// the same task multiple times via references
	CDWordSet mapProcessed;

	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);

		if (mapProcessed.Has(dwTaskID))
			continue;

		// ignore parent tasks
		if (m_data.IsTaskParent(dwTaskID) && !HasStyle(TDCS_ALLOWPARENTTIMETRACKING))
			continue;

		if (bRecalcTime) // Just changing the units
 		{
			TDCTIMEPERIOD tpCur;
			m_data.GetTaskTimeSpent(dwTaskID, tpCur);

			ASSERT(tpCur.nUnits != timeSpent.nUnits);

			if (tpCur.SetUnits(timeSpent.nUnits, TRUE))
				HandleModResult(dwTaskID, m_data.SetTaskTimeSpent(dwTaskID, tpCur), aModTaskIDs);
		}
		else // Set or offset
		{
			HandleModResult(dwTaskID, m_data.SetTaskTimeSpent(dwTaskID, timeSpent, bOffset), aModTaskIDs);
		}

		mapProcessed.Add(dwTaskID);
	}
	
	if (!aModTaskIDs.GetSize())
		return FALSE;
	
	SetModified(TDCA_TIMESPENT, aModTaskIDs);
	return TRUE;
}

BOOL CToDoCtrl::HandleModResult(DWORD dwTaskID, TDC_SET nRes, CDWordArray& aModTaskIDs)
{
	switch (nRes)
	{
	case SET_FAILED:
		return FALSE;

	case SET_CHANGE:
		aModTaskIDs.Add(dwTaskID);
		break;
	}

	return TRUE;
}

BOOL CToDoCtrl::SetSelectedTaskAllocTo(const CStringArray& aAllocTo, BOOL bAppend)
{
	return SetSelectedTaskArray(TDCA_ALLOCTO, aAllocTo, bAppend);
}

BOOL CToDoCtrl::SetSelectedTaskAllocBy(const CString& sAllocBy)
{
	if (!CanEditSelectedTask(TDCA_ALLOCBY))
		return FALSE;

	Flush();
	
	IMPLEMENT_DATA_UNDO_EDIT(m_data);

	CDWordArray aModTaskIDs;
	POSITION pos = TSH().GetFirstItemPos();
	
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		HandleModResult(dwTaskID, m_data.SetTaskAllocBy(dwTaskID, sAllocBy), aModTaskIDs);
	}
	
	if (!aModTaskIDs.GetSize())
		return FALSE;

	SetModified(TDCA_ALLOCBY, aModTaskIDs);
	return TRUE;
}

BOOL CToDoCtrl::SetSelectedTaskVersion(const CString& sVersion)
{
	if (!CanEditSelectedTask(TDCA_VERSION))
		return FALSE;

	Flush();
	
	IMPLEMENT_DATA_UNDO_EDIT(m_data);
		
	CDWordArray aModTaskIDs;
	POSITION pos = TSH().GetFirstItemPos();

	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		HandleModResult(dwTaskID, m_data.SetTaskVersion(dwTaskID, sVersion), aModTaskIDs);
	}
	
	if (!aModTaskIDs.GetSize())
		return FALSE;

	SetModified(TDCA_VERSION, aModTaskIDs);
	return TRUE;
}

BOOL CToDoCtrl::SetSelectedTaskStatus(const CString& sStatus)
{
	if (!CanEditSelectedTask(TDCA_STATUS))
		return FALSE;

	Flush();
	
	IMPLEMENT_DATA_UNDO_EDIT(m_data);
		
	POSITION pos = TSH().GetFirstItemPos();
	CDWordArray aModTaskIDs;
	
	// Status edits can cause completion changes
	CTDCTaskCompletionArray aTasksForCompletion(m_data, m_sCompletionStatus);
	
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);

		if (!aTasksForCompletion.Add(dwTaskID, sStatus))
		{
			// regular status change
			HandleModResult(dwTaskID, m_data.SetTaskStatus(dwTaskID, sStatus), aModTaskIDs);
		}
	}

	if (aTasksForCompletion.GetSize())
		return SetSelectedTaskCompletion(aTasksForCompletion);

	// else regular status change
	if (!aModTaskIDs.GetSize())
		return FALSE;

	SetModified(TDCA_STATUS, aModTaskIDs);
	return TRUE;
}

BOOL CToDoCtrl::SetSelectedTaskArray(TDC_ATTRIBUTE nAttribID, const CStringArray& aItems, BOOL bAppend)
{
	if (!CanEditSelectedTask(nAttribID))
		return SET_FAILED;

	Flush();

	IMPLEMENT_DATA_UNDO_EDIT(m_data);

	CDWordArray aModTaskIDs;
	POSITION pos = TSH().GetFirstItemPos();

	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		HandleModResult(dwTaskID, m_data.SetTaskArray(dwTaskID, nAttribID, aItems, bAppend), aModTaskIDs);
	}

	if (!aModTaskIDs.GetSize())
		return FALSE;

	SetModified(nAttribID, aModTaskIDs);
	return TRUE;
}

BOOL CToDoCtrl::SetSelectedTaskArray(TDC_ATTRIBUTE nAttribID, const CStringArray& aAll, 
									 const CStringArray& aChecked, const CStringArray& aMixed)
{
	Flush();

	IMPLEMENT_DATA_UNDO_EDIT(m_data);

	CStringArray aUnchecked, aTaskItems;
	BOOL bMergeItems = FALSE;

	if (aMixed.GetSize())
	{
		aUnchecked.Copy(aAll);
		Misc::RemoveItems(aChecked, aUnchecked);
		Misc::RemoveItems(aMixed, aUnchecked);

		bMergeItems = TRUE;
	}
	
	CDWordArray aModTaskIDs;
	POSITION pos = TSH().GetFirstItemPos();

	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);

		// We only need to be careful if the combo has any mixed items
		// and if the task itself has any current array items
		if (bMergeItems && m_data.GetTaskArray(dwTaskID, nAttribID, aTaskItems))
		{
			Misc::AppendItems(aChecked, aTaskItems, TRUE);
			Misc::RemoveItems(aUnchecked, aTaskItems);
		}
		else
		{
			aTaskItems.Copy(aChecked);
		}
		
		HandleModResult(dwTaskID, m_data.SetTaskArray(dwTaskID, nAttribID, aTaskItems, FALSE), aModTaskIDs);
	}

	if (!aModTaskIDs.GetSize())
		return FALSE;

	SetModified(nAttribID, aModTaskIDs);
	return TRUE;
}

BOOL CToDoCtrl::SetSelectedTaskCategories(const CStringArray& aCats, BOOL bAppend)
{
	return SetSelectedTaskArray(TDCA_CATEGORY, aCats, bAppend);
}

BOOL CToDoCtrl::SetSelectedTaskTags(const CStringArray& aTags, BOOL bAppend)
{
	return SetSelectedTaskArray(TDCA_TAGS, aTags, bAppend);
}

BOOL CToDoCtrl::SetSelectedTaskFileLinks(const CStringArray& aFilePaths, BOOL bAppend)
{
	CStringArray aFileLinks;

	if (bAppend)
		GetSelectedTaskFileLinks(aFileLinks); // full paths

	Misc::AppendItems(aFilePaths, aFileLinks, TRUE);
	MakeRelativePaths(aFileLinks);

	return SetSelectedTaskArray(TDCA_FILELINK, aFileLinks, bAppend);
}

BOOL CToDoCtrl::SetSelectedTaskDependencies(const CTDCDependencyArray& aDepends, BOOL bAppend)
{
	if (!CanEditSelectedTask(TDCA_DEPENDENCY))
		return SET_FAILED;

	Flush();

	IMPLEMENT_DATA_UNDO_EDIT(m_data);

	CDWordArray aModTaskIDs;
	POSITION pos = TSH().GetFirstItemPos();

	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		HandleModResult(dwTaskID, m_data.SetTaskDependencies(dwTaskID, aDepends, bAppend), aModTaskIDs);
	}

	if (!aModTaskIDs.GetSize())
		return FALSE;

	SetModified(TDCA_DEPENDENCY, aModTaskIDs);
	return TRUE;
}

void CToDoCtrl::PauseTimeTracking(BOOL bPause) 
{ 
	if (m_timeTracking.PauseTracking(bPause))
	{
		if (bPause)
			KillTimer(TIMER_TRACK);
		else
			SetTimer(TIMER_TRACK, TIMETRACKPERIOD, NULL);
	}
}

CString CToDoCtrl::FormatTimeTrackingElapsedTime() const
{
	return m_timeTracking.FormatElapsedTime();
}

void CToDoCtrl::ResetTimeTrackingElapsedMinutes()
{
	m_timeTracking.ResetReminderIsDue();
}

BOOL CToDoCtrl::TimeTrackSelectedTask()
{
	if (!CanTimeTrackSelectedTask())
		return FALSE;
	
	ToggleTimeTracking(GetSelectedItem());
	
	return FALSE;
}

BOOL CToDoCtrl::CanTimeTrackSelectedTask() const
{
	if (!CanEditSelectedTask(TDCA_TIMESPENT))
		return FALSE;

	return m_timeTracking.CanTrackSelectedTask();
}

BOOL CToDoCtrl::IsSelectedTaskBeingTimeTracked() const
{
	return m_timeTracking.IsTrackingSelectedTask();
}

DWORD CToDoCtrl::GetTimeTrackTaskID(BOOL bActive) const
{
	return m_timeTracking.GetTrackedTaskID(bActive);
}

BOOL CToDoCtrl::IsActivelyTimeTracking() const
{
	DWORD dwTrackedTaskID = m_timeTracking.GetTrackedTaskID(TRUE);

	return (dwTrackedTaskID != 0);
}

BOOL CToDoCtrl::SetSelectedTaskExternalID(const CString& sExtID)
{
	if (!CanEditSelectedTask(TDCA_EXTERNALID))
		return FALSE;

	Flush();
	
	IMPLEMENT_DATA_UNDO_EDIT(m_data);
		
	CDWordArray aModTaskIDs;
	POSITION pos = TSH().GetFirstItemPos();
	
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		HandleModResult(dwTaskID, m_data.SetTaskExternalID(dwTaskID, sExtID), aModTaskIDs);
	}
	
	if (!aModTaskIDs.GetSize())
		return FALSE;

	SetModified(TDCA_EXTERNALID, aModTaskIDs);
	return TRUE;
}

BOOL CToDoCtrl::GotoSelectedTaskFileLink(int nFile)
{
	if (nFile < 0)
	{
		ASSERT(0);
		return FALSE;
	}

	CStringArray aFiles;
	int nNumFiles = m_ctrlAttributes.GetFileLinks(aFiles);

	if (nFile < nNumFiles)
		return GotoFile(aFiles[nFile]);

	return FALSE;
}

BOOL CToDoCtrl::CreateNewTask(const CString& sText, TDC_INSERTWHERE nWhere, BOOL bEditLabel, DWORD dwDependency)
{
	if (sText.IsEmpty() || !CanCreateNewTask(nWhere))
	{
		ASSERT(0);
		return FALSE;
	}

	// Are we an archive and should we warn user if we are
	if (m_bArchive && 
		HasStyle(TDCS_WARNADDDELETEARCHIVE) && 
		(IDNO == CMessageBox::AfxShow(IDS_TDC_CONFIRMADD_TITLE, IDS_TDC_WARNADDTOARCHIVE, MB_YESNO | MB_ICONQUESTION)))
	{
		return FALSE;
	}
	
	Flush();

	HTREEITEM htiParent = NULL, htiAfter = NULL;

	if (!m_taskTree.GetInsertLocation(nWhere, htiParent, htiAfter))
	{
		ASSERT(0);
		return FALSE;
	}

	HTREEITEM htiNew = InsertNewTask(sText, htiParent, htiAfter, bEditLabel, dwDependency);
	ASSERT(htiNew);

	DWORD dwTaskID = GetTaskID(htiNew);
	ASSERT(dwTaskID == (m_dwNextUniqueID - 1));

	return (htiNew != NULL);
}

BOOL CToDoCtrl::CanCreateNewTask(TDC_INSERTWHERE nInsertWhere) const
{
	switch (nInsertWhere)
	{
	case TDC_INSERTATTOP:
	case TDC_INSERTATBOTTOM:
		return !IsReadOnly();

	case TDC_INSERTATTOPOFSELTASKPARENT:
	case TDC_INSERTATBOTTOMOFSELTASKPARENT:
	case TDC_INSERTAFTERSELTASK:
	case TDC_INSERTBEFORESELTASK:
	case TDC_INSERTATTOPOFSELTASK: 
	case TDC_INSERTATBOTTOMOFSELTASK:
		if (CanEditSelectedTask(TDCA_NEWTASK))
		{
			HTREEITEM htiParent = NULL, htiAfter = NULL;

			switch (GetSelectedTaskCount())
			{
			case 0:
				break; // handled below

			case 1:
				VERIFY(m_taskTree.GetInsertLocation(nInsertWhere, htiParent, htiAfter));
				break;

			default:
				if (!m_taskTree.SelectionHasSameParent())
					return FALSE;

				VERIFY(m_taskTree.GetInsertLocation(nInsertWhere, htiParent, htiAfter));
				break;
			}

			// Selected item cannot be NULL and parent cannot be reference
 			if (GetSelectedItem() == NULL)
 				return FALSE;
		
			return !m_data.IsTaskReference(GetTaskID(htiParent));
		}
		break;

	default:
		ASSERT(0);
		break;
	}

	return FALSE;
}

BOOL CToDoCtrl::CreateNewSubtaskInTask(const CString& sText, BOOL bTop)
{
	// Get all editable tasks 
	TDCGETTASKS filter(TDCGT_ALL, TDCGTF_NOTLOCKED);

	filter.mapAttribs.Add(TDCA_TASKNAME);
	filter.mapAttribs.Add(TDCA_ICON);

	CTaskFile tasks;
	GetTasks(tasks, filter);

	// Prepare the dialog
	CTDLSelectTaskDlg dialog(tasks,
							 GetTaskIconImageList(),
							 GetPreferencesKey(_T("NewSubtaskInTask")));

	dialog.SetStrikethroughCompletedTasks(HasStyle(TDCS_STRIKETHOUGHDONETASKS));
	dialog.SetShowParentTasksAsFolders(HasStyle(TDCS_SHOWPARENTSASFOLDERS));
	dialog.SetSelectedTaskID(GetSelectedTaskID());
	dialog.SetCompletedTaskColor(m_taskTree.GetCompletedTaskColor());

	if (dialog.DoModal(NULL, IDS_SELECTSUBTASKPARENT_TITLE) != IDOK)
		return FALSE;

	// Select the chosen task
	if (!SelectTask(dialog.GetSelectedTaskID(), FALSE))
	{
		ASSERT(0);
		return FALSE;
	}

	// Create the task
	TDC_INSERTWHERE nInsert = (bTop ? TDC_INSERTATTOPOFSELTASK : TDC_INSERTATBOTTOMOFSELTASK);

	if (!CreateNewTask(sText, nInsert, TRUE))
	{
		ASSERT(0);
		return FALSE;
	}

	return TRUE;
}

BOOL CToDoCtrl::CanCreateNewSubtaskInTask() const
{
	return m_taskTree.GetItemCount();
}

TODOITEM* CToDoCtrl::CreateNewTask(HTREEITEM htiParent)
{
	// copy over parent attributes
	BOOL bRootParent = (!htiParent || htiParent == TVI_ROOT);
	DWORD dwParentID = bRootParent ? 0 : GetTaskID(htiParent);

	return m_data.NewTask(m_tdiDefault, dwParentID);
}

HTREEITEM CToDoCtrl::InsertNewTask(const CString& sText, HTREEITEM htiParent, HTREEITEM htiAfter, 
									BOOL bEditLabel, DWORD dwDependency)
{
	ASSERT((htiParent == TVI_ROOT) || CanEditTask(GetTaskID(htiParent), TDCA_NEWTASK));
	ASSERT(!sText.IsEmpty());

	m_dwLastAddedID = 0;
	
	IMPLEMENT_DATA_UNDO(m_data, TDCUAT_ADD);

	// create the new task item
	TODOITEM* pTDINew = CreateNewTask(htiParent);

	if (!pTDINew)
	{
		ASSERT(0);
		return NULL;
	}
	
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
				if (m_timeTracking.IsTrackingTask(dwParentID))
					EndTimeTracking(TRUE, TRUE);
			}

			// if the parent was marked as done and the new task 
			// is NOT cancellable, we mark the parent as incomplete.
			if (!bEditLabel && m_data.IsTaskDone(dwParentID))
				FixupParentCompletion(dwParentID);
		}
		
		m_dwNextUniqueID++;

		// Insert task into dependency chain
		if (dwDependency)
			m_data.InsertTaskIntoDependencyChain(dwTaskID, dwDependency);

		CDWordArray aModTaskIDs;
		aModTaskIDs.Add(dwTaskID);

		SelectItem(htiNew);
		SetModified(TDCA_NEWTASK, aModTaskIDs); 

		m_taskTree.InvalidateAll();

		if (bEditLabel)
			EditSelectedTaskTitle(TRUE);
		else
			SetFocus(TDCSF_TASKVIEW);
	}
	else // cleanup
	{
		m_data.DeleteTask(m_dwNextUniqueID, FALSE); // FALSE == no undo
	}
	
	return htiNew;
}

BOOL CToDoCtrl::CanSplitSelectedTask() const 
{ 
	if (!CanEditSelectedTask(TDCA_NEWTASK))
		return FALSE; 

	switch (m_taskTree.GetSelectedCount())
	{
	case 0:
		return FALSE;

	case 1:
		return (!m_taskTree.SelectionHasDone(FALSE) && 
				!m_taskTree.SelectionHasSubtasks() &&
				!m_taskTree.SelectionHasReferences());
	}

	// For the rest we filter during the actual splitting
	return TRUE;
}

BOOL CToDoCtrl::SplitSelectedTask(int nNumSubtasks)
{
	if ((nNumSubtasks < 2) || !CanSplitSelectedTask())
		return FALSE;
	
	Flush();
	
	IMPLEMENT_DATA_UNDO(m_data, TDCUAT_ADD);
	HOLD_REDRAW(NULL, m_taskTree.Tree());
	
	POSITION pos = TSH().GetFirstItemPos();
	CDWordArray aNewTaskIDs;
	
	while (pos)
	{
		HTREEITEM hti = TSH().GetNextItem(pos);
				
		if (ItemHasChildren(hti))
			continue;
		
		DWORD dwTaskID = GetTaskID(hti);

		if (m_calculator.IsTaskLocked(dwTaskID))
			continue;

		const TODOITEM* pTDI = GetTask(dwTaskID);
		ASSERT(pTDI);
		
		if (!pTDI || pTDI->IsDone() || pTDI->IsReference())
			continue;
		
		// Calculate how to apportion time to subtasks
		BOOL bWantInheritTimeEst = m_data.WantUpdateInheritedAttibute(TDCA_TIMEESTIMATE);
		BOOL bWantInheritStartDate = m_data.WantUpdateInheritedAttibute(TDCA_STARTDATE);
		BOOL bWantInheritDueDate = m_data.WantUpdateInheritedAttibute(TDCA_DUEDATE);
		
		double dSubDuration = 0;
		
		if (!bWantInheritStartDate && !bWantInheritDueDate)
		{
			if (pTDI->HasStart() && (pTDI->dateDue > pTDI->dateStart))
			{
				double dDuration = (pTDI->dateDue.m_dt - pTDI->dateStart.m_dt);

				if (!CDateHelper::DateHasTime(pTDI->dateDue))
					dDuration++;

				dSubDuration = (dDuration / nNumSubtasks);
			}
		}

		DWORD dwPrevSiblingID = 0;
		HTREEITEM htiPrevSibling = NULL;
		
		for (int nSubtask = 0; nSubtask < nNumSubtasks; nSubtask++)
		{
			TODOITEM* pTDISub = m_data.NewTask(*pTDI); // copy parent by default
			
			// Allocate time estimate if not inherited
			if (!bWantInheritTimeEst)
				pTDISub->timeEstimate.dAmount = (pTDI->timeEstimate.dAmount / nNumSubtasks);
			
			// Allocate all time spent to first task (clear the rest)
			if (nSubtask > 0)
				pTDISub->timeSpent.dAmount = 0;

			// Allocate duration if not inherited
			if (!bWantInheritStartDate && !bWantInheritDueDate)
			{
				if (dSubDuration)
				{
					pTDISub->dateStart += (nSubtask * dSubDuration);
					pTDISub->dateDue = pTDISub->dateStart + COleDateTime(dSubDuration);

					if (!CDateHelper::DateHasTime(pTDISub->dateDue))
					{
						pTDISub->dateDue.m_dt--;
					}
					else if (!pTDI->HasDueTime())
					{
						// clear due time if parent didn't have any
						pTDISub->dateDue = CDateHelper::GetDateOnly(pTDISub->dateDue);
					}
				}
				else if (nSubtask) // not the first
				{
					pTDISub->ClearStart();
					pTDISub->ClearDue();
				}
			}
			else
			{
				if (!bWantInheritStartDate)
					pTDISub->ClearStart();

				if (!bWantInheritDueDate)
					pTDISub->ClearDue();
			}

			// map it
			DWORD dwChildID = m_dwNextUniqueID++;
			m_data.AddTask(dwChildID, pTDISub, dwTaskID, dwPrevSiblingID);
			
			// create tree item
			htiPrevSibling = m_taskTree.InsertItem(dwChildID, hti, htiPrevSibling);
			ASSERT(htiPrevSibling);
			
			dwPrevSiblingID = dwChildID;
			aNewTaskIDs.Add(dwChildID);
		}

		// clear parent time est if not inherited
		if (!bWantInheritTimeEst)
			m_data.ClearTaskAttribute(dwTaskID, TDCA_TIMEESTIMATE);

		m_data.ClearTaskAttribute(dwTaskID, TDCA_TIMESPENT);

		// show new subtasks
		m_taskTree.ExpandItem(hti, TRUE);
	}
	
	if (aNewTaskIDs.GetSize())
	{
		ASSERT(aNewTaskIDs.GetSize() > 1);
		SetModified(TDCA_NEWTASK, aNewTaskIDs);

		return TRUE;
	}
	
	return FALSE;
}

BOOL CToDoCtrl::DeleteSelectedTask()
{
	Flush();
	
	IMPLEMENT_DATA_UNDO(m_data, TDCUAT_DELETE);
	
	// else do the standard warning against deletion
	return DeleteSelectedTask(TRUE, TRUE);
}

BOOL CToDoCtrl::DeleteSelectedTask(BOOL bWarnUser, BOOL bResetSel)
{
	if (!CanEditSelectedTask(TDCA_DELETE))
		return FALSE;
	
	// check there's something to delete
	int nSelCount = GetSelectedTaskCount();
	
	if (!nSelCount)
		return FALSE;

	// end time tracking as required
	DWORD dwTrackedTaskID = m_timeTracking.GetTrackedTaskID(FALSE);

	if (dwTrackedTaskID && TSH().HasItem(dwTrackedTaskID))
		EndTimeTracking(FALSE, TRUE);

	// cache focus
	CSaveFocus focus;
	
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
	DWORD dwNextID = 0;

	if (bResetSel)
		dwNextID = GetNextNonSelectedTaskID();
	
	if (TSH().HasItem(m_taskTree.GetSelectedItem()))
		TCH().SelectItem(NULL);
	
	// clear selection before deleting
	TSH().RemoveAll(TRUE, FALSE);

	CDWordArray aDelTaskIDs;
	TSH().GetItemData(selection, aDelTaskIDs);

	{
		BOOL bSingleTask = (nSelCount == 1);

		CAutoFlag af(m_bDeletingTasks, TRUE);
		CHoldRedraw hr(bSingleTask ? NULL : m_taskTree.GetSafeHwnd());
		POSITION pos = selection.GetHeadPosition();

		while (pos)
		{
			HTREEITEM hti = selection.GetNext(pos);
			DWORD dwTaskID = m_taskTree.GetTaskID(hti);

			// Delete the tree item first so there's no chance 
			// of a tree item referencing a non-existent task
			m_taskTree.DeleteItem(hti);

			// Delete data object last
			m_data.DeleteTask(dwTaskID, TRUE); // TRUE == with undo
		}
	}
	m_taskTree.UpdateAll();

	// Update other views before changing selection
	SetModified(TDCA_DELETE, aDelTaskIDs);
	
	// set next selection
	if (dwNextID)
		SelectTask(dwNextID, FALSE);
	else
		UpdateControls(); // update comments

	// restore focus
	if (!focus.RestoreFocus())
		SetFocus(TDCSF_TASKVIEW);

	return TRUE;
}

DWORD CToDoCtrl::GetNextNonSelectedTaskID() const
{
	HTREEITEM hti = TSH().GetFirstItem();
	HTREEITEM htiNextSel = TCH().GetNextVisibleItem(hti, FALSE);

	// don't include items we're about to delete!
	DWORD dwSelID = GetTaskID(hti);

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

	return GetTaskID(htiNextSel);
}

BOOL CToDoCtrl::GetLabelEditRect(CRect& rScreen)
{
	if (m_taskTree.EnsureSelectionVisible(FALSE))
		return m_taskTree.GetLabelEditRect(rScreen);

	// else
	return FALSE;
}

BOOL CToDoCtrl::EditSelectedTaskTitle(BOOL bTaskIsNew)
{
	if (HasStyle(TDCS_SHOWINFOTIPS) && m_infoTip.GetSafeHwnd())
		m_infoTip.Pop();

	if (!CanEditSelectedTask(TDCA_TASKNAME))
		return FALSE;

	if (IsTaskLabelEditing())
		return TRUE;
	
	// get edit rect
	CRect rPos;
	
	if (!GetLabelEditRect(rPos))
		return FALSE;

	// make sure it's at least 200 px long
	int nMinLen = GraphicsMisc::ScaleByDPIFactor(200);
	rPos.right = max(rPos.right, rPos.left + nMinLen);

	// create edit on request
	if (!m_eTaskName.GetSafeHwnd() && !m_eTaskName.Create(this, IDC_TASKLABELEDIT, (WS_POPUP | WS_BORDER | ES_AUTOHSCROLL)))
		return FALSE;

	// start
	DWORD dwSelTaskID = GetSelectedTaskID();
	ASSERT (dwSelTaskID);

	if (bTaskIsNew)
		m_dwLastAddedID = dwSelTaskID;
	
	// save task id being edited
	SetEditTitleTaskID(dwSelTaskID);

	// set font
	CFont* pFontTree = m_taskTree.Tree().GetFont();

	if (pFontTree)
		m_eTaskName.SetFont(pFontTree);
	
	// set text
	m_eTaskName.SetWindowText(GetTaskTitle(dwSelTaskID));
	
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
	return (CanEditSelectedTask(TDCA_TASKNAME) && (m_dwEditTitleTaskID != 0));
}

LRESULT CToDoCtrl::OnLabelEditEnd(WPARAM /*wParam*/, LPARAM lParam)
{
	ASSERT(m_dwEditTitleTaskID);

	CString sText;
	m_eTaskName.GetWindowText(sText);

	BOOL bResort = FALSE;

	// Scope locking to end before resorting which also wants
	// to take a lock
	{
		CLockUpdates lu(m_taskTree);

		if (GetSelectedTaskCount() == 0) // user clicked into space
			SelectTask(m_dwEditTitleTaskID, FALSE);

		// end can occurs either when the user selected return
		// or if the edit box loses the focus so we need to check
		// the lParam and only set the focus if the user chose return
		if (lParam)
			SetFocus(TDCSF_TASKVIEW);

		if (!sText.IsEmpty())
		{
			Flush();

			// Special handling: For new tasks we extend the previous undo
			BOOL bNewTask = (m_dwEditTitleTaskID == m_dwLastAddedID);
			TDC_UNDOACTIONTYPE nAction = (bNewTask ? TDCUAT_ADD : TDCUAT_EDIT);

			IMPLEMENT_DATA_UNDO_EXTEND(m_data, nAction, bNewTask);
		
			int nRes = m_data.SetTaskTitle(m_dwEditTitleTaskID, sText);
		
			if ((nRes == SET_CHANGE) || bNewTask)
			{
				CDWordArray aModTaskIDs;
				aModTaskIDs.Add(m_dwEditTitleTaskID);

				m_taskTree.InvalidateSelection(TRUE);
				SetModified(TDCA_TASKNAME, aModTaskIDs);

				if (bNewTask)
				{
					// If this was a new task and the parent was marked as done, 
					// now mark it as incomplete
					DWORD dwParentID = m_data.GetTaskParentID(m_dwEditTitleTaskID);
				
					if (dwParentID && m_data.IsTaskDone(dwParentID))
						FixupParentCompletion(dwParentID);

					// For new tasks we did not sort so we may need to do so
					// now if sorting by anything other than 'title' because 
					// will already have been handled by SetModified
					bResort = (m_taskTree.IsMultiSorting() || (m_taskTree.IsSorting() && !m_taskTree.IsSortingBy(TDCC_CLIENT)));
				}
			}
		}
	}

	if (bResort)
		m_taskTree.Resort();

	SetEditTitleTaskID(0);

	return 0L;
}

void CToDoCtrl::SetEditTitleTaskID(DWORD dwTaskID)
{	
	m_dwEditTitleTaskID = dwTaskID;
	m_taskTree.SetEditTitleTaskID(dwTaskID);

	if (dwTaskID == 0)
		m_dwLastAddedID = 0;
}

LRESULT CToDoCtrl::OnLabelEditCancel(WPARAM /*wParam*/, LPARAM lParam)
{
	if (GetSelectedTaskCount() == 0) // user clicked into space
	{
		SelectTask(m_dwEditTitleTaskID, FALSE);
	}
	// user hit escape key so we may need to delete this task if newly created
	else if (GetSelectedTaskID() == m_dwLastAddedID) 
	{
		ASSERT (lParam);
		UNREFERENCED_PARAMETER(lParam);

		// Revert selection to previous task and if that fails then next task
		if (!m_taskTree.RestorePreviousSelection(FALSE) &&
			!GotoNextTask(TDCG_PREV) &&
			!GotoNextTask(TDCG_NEXT))
		{
			TSH().RemoveAll();
		}

		UndoLastAction(TRUE);
		m_data.ClearRedoStack(); // Not undoable
	}

	SetFocus(TDCSF_TASKVIEW);
	SetEditTitleTaskID(0);

	return 0L;
}

BOOL CToDoCtrl::ConfirmDeleteAllTasks(BOOL bSelected) const
{
	if (!GetTaskCount() || !CanEditSelectedTask(TDCA_DELETE))
	{
		return FALSE;
	}
	// are we an archive and should we warn user if we are
	else if (m_bArchive && HasStyle(TDCS_WARNADDDELETEARCHIVE))
	{
		return (CMessageBox::AfxShow(IDS_TDC_CONFIRMDELETE_TITLE, IDS_TDC_WARNDELETEFROMARCHIVE, MB_YESNO | MB_ICONQUESTION) == IDYES);
	}
	else if (HasStyle(TDCS_CONFIRMDELETE))
	{
		CEnString sMessage(bSelected ? IDS_TDC_CONFIRMDELETESEL : IDS_TDC_CONFIRMDELETEALL);
		UINT nFlags = (MB_ICONEXCLAMATION | MB_YESNO);

		if (bSelected && m_taskTree.SelectionHasSubtasks())
		{
			if (GetSelectedTaskCount() == 1)
				sMessage += CEnString(IDS_TDC_CONFIRMDELETEONEWARNSUBTASKS);
			else
				sMessage += CEnString(IDS_TDC_CONFIRMDELETEMOREWARNSUBTASKS);

			nFlags |= MB_DEFBUTTON2;
		}
		// else MB_DEFBUTTON1
			
		return (CMessageBox::AfxShow(IDS_TDC_CONFIRMDELETE_TITLE, sMessage, nFlags) == IDYES);
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
	DeselectAll();
	
	CWaitCursor cursor;
	
	IMPLEMENT_DATA_UNDO(m_data, TDCUAT_DELETE);
	HOLD_REDRAW(*this, 0);

	m_taskTree.DeleteAll();
	m_data.DeleteAllTasks();

	SetModified(TDCA_DELETE);
	UpdateControls();
	
	return TRUE;
}

DWORD CToDoCtrl::SetStyle(TDC_STYLE nStyle, BOOL bEnable)
{
	ASSERT (GetSafeHwnd());
	
	DWORD dwResult = 0;
	
	m_styles[nStyle] = bEnable;

	switch (nStyle)
	{
	case TDCS_SHOWDEFAULTTASKICONS:
		// Reload only if they've already been initialised
		if (m_ilTaskIcons.GetSafeHandle())
			LoadTaskIcons();
		break;

	case TDCS_SHOWDATESINISO:
		m_ctrlAttributes.RefreshDateTimeFormatting();
		break;

	case TDCS_NODUEDATEISDUETODAYORSTART:
	case TDCS_SHOWPATHINHEADER:
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
	case TDCS_INCLUDEREFERENCESINCALCS:
	case TDCS_HIDEPRIORITYNUMBER:
	case TDCS_USEHIGHESTRISK:
	case TDCS_INCLUDEDONEINRISKCALC:
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
	case TDCS_SORTDONETASKSATBOTTOM:
	case TDCS_DUEHAVEHIGHESTPRIORITY:
	case TDCS_DONEHAVELOWESTPRIORITY:
	case TDCS_DONEHAVELOWESTRISK:
		// handled solely by tree-list
		break;

	case TDCS_SHOWFILELINKTHUMBNAILS:
		m_ctrlAttributes.Invalidate(FALSE);
		break;

	case TDCS_SHOWINFOTIPS:
		if (bEnable)
		{
			ASSERT(!m_infoTip.GetSafeHwnd());

			if (!m_infoTip.Create(this))
				return FALSE;

			// else
			m_infoTip.EnableMultilineTips();
			m_infoTip.EnableTracking(TRUE, INFOTIPOFFSET, INFOTIPOFFSET);
			m_infoTip.SetFont(CFont::FromHandle(m_hFontTree));
		}
		else if (m_infoTip.GetSafeHwnd())
		{
			m_infoTip.DestroyWindow();
		}
		break;

	case TDCS_USEEARLIESTDUEDATE:
		if (bEnable)
			m_styles[TDCS_USELATESTDUEDATE] = FALSE; // mutually exclusive
		break;

	case TDCS_USELATESTDUEDATE:
		if (bEnable)
			m_styles[TDCS_USEEARLIESTDUEDATE] = FALSE; // mutually exclusive
		break;

	case TDCS_USEEARLIESTSTARTDATE:
		if (bEnable)
			m_styles[TDCS_USELATESTSTARTDATE] = FALSE; // mutually exclusive
		break;

	case TDCS_USELATESTSTARTDATE:
		if (bEnable)
			m_styles[TDCS_USEEARLIESTSTARTDATE] = FALSE; // mutually exclusive
		break;

	case TDCS_TASKCOLORISBACKGROUND:
		m_ctrlAttributes.RedrawValue(TDCA_COLOR);
		break;

	case TDCS_SHOWPROJECTNAME:
		// Fix up focus
		if (!bEnable && (GetFocus() == GetDlgItem(IDC_PROJECTNAME)))
		{
			SetFocus(TDCSF_TASKVIEW);
		}
		dwResult = TDCSS_WANTRESIZE;
		break;

	case TDCS_SHOWCOMMENTSALWAYS:
		if (m_layout.GetMaximiseState() == TDCMS_MAXTASKLIST)
		{
			m_layout.SetMaximised(TDCMS_MAXTASKLIST, bEnable);
			dwResult = TDCSS_WANTRESIZE;
		}
		break;

	case TDCS_COLORTEXTBYPRIORITY:
	case TDCS_COLORTEXTBYATTRIBUTE:
	case TDCS_COLORTEXTBYNONE:
	case TDCS_ALLOWCOMMENTSSTACKING:
	case TDCS_STACKCOMMENTSABOVEEDITS:
		dwResult = TDCSS_WANTRESIZE;
		break;

	case TDCS_READONLY:
		m_treeDragDrop.EnableDragDrop(!bEnable);
		break;

	case TDCS_CONFIRMDELETE:
	case TDCS_CHECKOUTONLOAD:
	case TDCS_SYNCTIMEESTIMATESANDDATES:
	case TDCS_FOCUSTREEONENTER:
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
	case TDCS_DISABLEPASSWORDPROMPTING:
	case TDCS_SAVEUIVISINTASKLIST:
	case TDCS_AUTOADJUSTDEPENDENCYDATES:
	case TDCS_TRACKSELECTEDTASKONLY:
	case TDCS_COMMENTSUSETREEFONT:
		// do nothing
		break;

	default:
		//ASSERT(0); // just to help catch forgotten styles
		break;
	}
	
	return dwResult;
}

int CToDoCtrl::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
{
	// Don't yet know why but updating the info tip text while
	// multiple selecting causes the mouse-down message to get
	// mislaid/eaten and the multiple-selection fails so if the
	// ctrl or shift keys are down we don't return a tooltip
	TDC_MAXSTATE nMaxState = m_layout.GetMaximiseState();

	if (!IsTaskLabelEditing() &&
		HasStyle(TDCS_SHOWINFOTIPS) &&
		m_infoTip.GetSafeHwnd() &&
		(nMaxState != TDCMS_MAXCOMMENTS) && 
		!Misc::IsKeyPressed(VK_CONTROL) &&
		!Misc::IsKeyPressed(VK_SHIFT))
	{
		CWnd::ClientToScreen(&point);

		DWORD dwTaskID = HitTestTask(point, TDCHTR_INFOTIP);

		if (dwTaskID)
		{
			CTDCAttributeMap mapAttrib;
			TDC::MapColumnsToAttributes(m_visColEdit.GetVisibleColumns(), mapAttrib);

			// Always add path for context
			mapAttrib.Add(TDCA_PATH);

			if (nMaxState == TDCMS_NORMAL)
				mapAttrib.Add(TDCA_COMMENTS);

			CString sInfoTip = m_infoTip.FormatTip(dwTaskID, 
												   mapAttrib, 
												   m_nMaxInfotipCommentsLength,
												   m_sCompletionStatus);
			ASSERT(!sInfoTip.IsEmpty());

			HWND hwndHit = CDialogHelper::GetWindowFromPoint(GetSafeHwnd(), point);
			ASSERT(hwndHit);

			return CToolTipCtrlEx::SetToolInfo(*pTI, hwndHit, sInfoTip, dwTaskID);
		}
	}

	// Don't want default behaviour
	return 0;
}

void CToDoCtrl::SetReadonly(BOOL bReadOnly)
{
	BOOL bIsReadOnly = HasStyle(TDCS_READONLY);

	if ((bIsReadOnly && !bReadOnly) || (!bIsReadOnly && bReadOnly))
		ModifyStyles(CTDCStyleMap(TDCS_READONLY, bReadOnly));
}

BOOL CToDoCtrl::ModifyStyles(const CTDCStyleMap& modStyles)
{
	if (modStyles.GetCount() == 0)
		return FALSE;

	HandleUnsavedComments();

	CHoldRedraw hr(*this, (NCR_PAINT | NCR_ERASEBKGND));
	
	POSITION pos = modStyles.GetStartPosition();
	DWORD dwResult = 0;

	while (pos)
	{
		TDC_STYLE nStyle;
		BOOL bWantOn;
		modStyles.GetNextAssoc(pos, nStyle, bWantOn);

		dwResult |= SetStyle(nStyle, bWantOn);
	}

	// notify tree-list we've finished changing styles
	OnStylesUpdated(modStyles);

	if (Misc::HasFlag(dwResult, TDCSS_WANTRESIZE)) 
		Resize();

	Invalidate(TRUE);
	UpdateControls();

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
	
	return m_styles.IsStyleEnabled(nStyle); 
}

void CToDoCtrl::SetCompletionStatus(const CString& sStatus) 
{ 
	if (sStatus != m_sCompletionStatus)
	{
		m_sCompletionStatus = sStatus; 
		m_taskTree.SetCompletionStatus(sStatus);

		if (!m_sCompletionStatus.IsEmpty())
			Misc::AddUniqueItem(m_sCompletionStatus, m_tldDefault.aStatus);
		else
			Misc::RemoveItem(m_sCompletionStatus, m_tldDefault.aStatus);

		m_ctrlAttributes.SetDefaultAutoListData(m_tldDefault);
	}
}

int CToDoCtrl::GetCustomAttributeDefs(CTDCCustomAttribDefinitionArray& aAttrib) const
{
	aAttrib.Copy(m_aCustomAttribDefs);
	return aAttrib.GetSize();
}

BOOL CToDoCtrl::SetCustomAttributeDefs(const CTDCCustomAttribDefinitionArray& aAttrib)
{
	ASSERT(CanEditSelectedTask(TDCA_CUSTOMATTRIB_DEFS));

	if (!Misc::MatchAllT(m_aCustomAttribDefs, aAttrib, TRUE))
	{
		m_aCustomAttribDefs.Copy(aAttrib);
		OnCustomAttributesChanged();

		// update interface
		SetModified(TDCA_CUSTOMATTRIB_DEFS);
		UpdateControls(FALSE); // don't update comments

		return TRUE;
	}

	// else
	return FALSE;
}

void CToDoCtrl::NotifyEndPreferencesUpdate() 
{ 
	Invalidate(); 

	// Update active comments
	m_ctrlComments.UpdateAppPreferences();
}

void CToDoCtrl::UpdateVisibleColumns(const CTDCColumnIDMap& mapChanges)
{
	m_taskTree.OnColumnVisibilityChange(mapChanges);
}

void CToDoCtrl::SetColumnFieldVisibility(const TDCCOLEDITVISIBILITY& vis)
{
	BOOL bColumnChange, bEditChange;
	BOOL bChange = m_visColEdit.HasDifferences(vis, bColumnChange, bEditChange);

	if (!bChange)
		return;

	TDCCOLEDITVISIBILITY visPrev = m_visColEdit;
	m_visColEdit = vis;

	if (GetSafeHwnd() && !IsDelayLoaded())
	{
		if (bColumnChange)
		{
			CTDCColumnIDMap mapChanges;
			VERIFY(visPrev.GetVisibleColumns().GetDifferences(vis.GetVisibleColumns(), mapChanges));

			UpdateVisibleColumns(mapChanges);
		}

 		if (bEditChange || (bColumnChange && (vis.GetShowFields() == TDLSA_ASCOLUMN)))
			m_ctrlAttributes.OnAttributeVisibilityChange();

		if (bChange && HasStyle(TDCS_SAVEUIVISINTASKLIST))
			m_bModified = TRUE;
	}
}

void CToDoCtrl::GetColumnFieldVisibility(TDCCOLEDITVISIBILITY& vis) const
{
	vis = m_visColEdit;
}

const CTDCColumnIDMap& CToDoCtrl::GetVisibleColumns() const
{
	return m_visColEdit.GetVisibleColumns();
}

const CTDCAttributeMap& CToDoCtrl::GetVisibleEditFields() const
{
	return m_visColEdit.GetVisibleEditFields();
}

int CToDoCtrl::GetSortableColumns(CTDCColumnIDMap& mapColIDs) const
{
	mapColIDs.Copy(m_visColEdit.GetVisibleColumns());
	mapColIDs.Add(TDCC_CLIENT); // always

	for (int nAtt = 0; nAtt < m_aCustomAttribDefs.GetSize(); nAtt++)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = m_aCustomAttribDefs[nAtt];

		if (attribDef.bEnabled && attribDef.SupportsFeature(TDCCAF_SORT))
			mapColIDs.Add(attribDef.GetColumnID());
	}

	return mapColIDs.GetCount();
}

BOOL CToDoCtrl::IsColumnShowing(TDC_COLUMN nColumn) const
{
	if ((nColumn == TDCC_CLIENT) || TDCCUSTOMATTRIBUTEDEFINITION::IsCustomColumn(nColumn))
		return TRUE;

	return m_visColEdit.IsColumnVisible(nColumn);
}

BOOL CToDoCtrl::IsEditFieldShowing(TDC_ATTRIBUTE nAttribID) const
{
	if ((nAttribID == TDCA_TASKNAME) || TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		return TRUE; // always visible

	return m_visColEdit.IsEditFieldVisible(nAttribID);
}

BOOL CToDoCtrl::IsColumnOrEditFieldShowing(TDC_COLUMN nColumn, TDC_ATTRIBUTE nAttribID) const
{
	return (IsColumnShowing(nColumn) || IsEditFieldShowing(nAttribID));
}

TDC_FILE CToDoCtrl::Save(const CString& sFilePath, BOOL bFlush)
{
	CTaskFile unused;
	return Save(unused, sFilePath, bFlush);
}

TDC_FILE CToDoCtrl::Save(CTaskFile& tasks/*out*/, const CString& sFilePath, BOOL bFlush)
{
	// PERMANENT LOGGING //////////////////////////////////////////////
	CScopedLogTimer log(_T("CToDoCtrl::Save(%s)"), FileMisc::GetFileNameFromPath(sFilePath));
	///////////////////////////////////////////////////////////////////////
	
	ASSERT (GetSafeHwnd());
	
	if (!GetSafeHwnd())
		return TDCF_OTHER;

	// can't save if file is readonly or delay-loaded
	if (IsDelayLoaded() || (CDriveInfo::IsReadonlyPath(sFilePath) > 0))
	{
		return TDCF_NOTALLOWED;
	}
	
	BOOL bFirstSave = (!HasFilePath() || !FileMisc::IsSamePath(m_sLastSavePath, sFilePath));

	// can't save if not checked-out
	// unless we're saving to another filename or this is our first save
	if (m_sourceControl.IsSourceControlled() && !m_sourceControl.IsCheckedOut())
	{
		if (!bFirstSave)
			return TDCF_SSC_NOTCHECKEDOUT;
	}

	CString sSavePath(sFilePath);
	
	if (sSavePath.IsEmpty())
	{
		if (!HasFilePath())
			return TDCF_OTHER;
		else
			sSavePath = m_sLastSavePath;
	}

	if (bFlush)
		Flush();

	// check for later changes if it's a network file
	BOOL bCheckforLaterChanges = (CDriveInfo::IsRemotePath(sFilePath) > 0);
	
	if (bCheckforLaterChanges && (m_nFileVersion > 0)) // else its newly created
	{
		if (FileMisc::FileExists(sSavePath)) // file exists (sanity check)
		{
			CTaskFile temp;

			if (temp.LoadHeader(sSavePath) && (temp.GetFileVersion() > m_nFileVersion))
			{
				CEnString sMessage(IDS_TDC_CONFIRMOVERWRITE, sSavePath);

				if (CMessageBox::AfxShow(IDS_TDC_CONFIRMOVERWRITE_TITLE, sMessage, MB_ICONWARNING | MB_YESNO) != IDYES)
				{
					return TDCF_CANCELLED;
				}
			}
		}
	}

	// prepare task file
	if (IsModified())
		m_nFileVersion++;

	BuildTasksForSave(tasks);

	TDC_FILE nResult = SaveTaskfile(tasks, sSavePath);

	if (nResult == TDCF_SUCCESS)
	{
		SetFilePath(sSavePath);
		m_bModified = FALSE;

		if (bFirstSave)
			OnFirstSave(tasks);
	}

	return nResult;
}

// static helper
TDC_FILE CToDoCtrl::SaveTaskfile(CTaskFile& tasks, const CString& sSavePath)
{
	// PERMANENT LOGGING //////////////////////////////////////////////
	CScopedLogTimer log(_T("CToDoCtrl::SaveTaskfile(%s)"), FileMisc::GetFileNameFromPath(sSavePath));
	///////////////////////////////////////////////////////////////////

	CWaitCursor cursor;

	// Always backup before overwriting
	CTempFileBackup backup(sSavePath, FBS_RENAME);
	
	// do the save
	if (tasks.Save(sSavePath, SFEF_UTF16))
		return TDCF_SUCCESS;

	// Error
	VERIFY(backup.RestoreBackup());
	
	return TDC::MapTaskfileError(tasks.GetLastFileError());
}

void CToDoCtrl::BuildTasksForSave(CTaskFile& tasks) const
{
	// PERMANENT LOGGING //////////////////////////////////////////////
	CScopedLogTimer log(_T("CToDoCtrl::BuildTasksForSave()"));
	///////////////////////////////////////////////////////////////////

	tasks.Reset();

	// get tasklist
	GetAllTasks(tasks);

	// attrib visibility
	SaveAttributeVisibility(tasks);

	// global data
	tasks.SetMetaData(m_mapMetaData);
	tasks.SetAutoListData(m_tldAll);

	// And password prompting disability
	if (IsEncrypted() && HasStyle(TDCS_DISABLEPASSWORDPROMPTING))
		tasks.SetDisablePasswordPrompting();
	
	// encrypt prior to setting checkout status and file info 
	// so these are visible without decryption
	// this quietly  fails if password is empty
	tasks.Encrypt(m_sPassword, SFEF_UTF16);

	// then append header info
	AppendTaskFileHeader(tasks);
	
	// checkout status
	m_sourceControl.PrepareTasksForSave(tasks);
}

void CToDoCtrl::LoadGlobals(const CTaskFile& tasks)
{
	if (tasks.GetAutoListData(m_tldAll))
	{
		m_ctrlAttributes.SetAutoListData(TDCA_ALL, m_tldAll);
		UpdateAutoListData();
	}
}

void CToDoCtrl::SaveCustomAttributeDefinitions(CTaskFile& tasks, const TDCGETTASKS& filter) const
{
	if (filter.mapAttribs.HasOnly(TDCA_ALL) ||
		filter.mapAttribs.Has(TDCA_CUSTOMATTRIB_ALL))
	{
		tasks.SetCustomAttributeDefs(m_aCustomAttribDefs);
	}
	else
	{
		// Disable all BUT those attributes that exist in the filter 
		CTDCCustomAttribDefinitionArray aAttribDefs;
		aAttribDefs.Copy(m_aCustomAttribDefs);

		int nDef = aAttribDefs.GetSize();

		while (nDef--)
		{
			TDCCUSTOMATTRIBUTEDEFINITION& attribDef = aAttribDefs[nDef];
			attribDef.bEnabled &= filter.WantAttribute(attribDef.GetAttributeID());
		}

		tasks.SetCustomAttributeDefs(aAttribDefs);
	}
}

void CToDoCtrl::LoadCustomAttributeDefinitions(const CTaskFile& tasks)
{
	tasks.GetCustomAttributeDefs(m_aCustomAttribDefs);

	// Add Fields and columns to view
	OnCustomAttributesChanged();
}

void CToDoCtrl::OnCustomAttributesChanged()
{
	m_taskTree.OnCustomAttributesChange();
	m_ctrlAttributes.OnCustomAttributesChange();
}

BOOL CToDoCtrl::CheckRestoreBackupFile(const CString& sFilePath)
{
	// check for the existence of a backup file
	CString sBackupPath = CFileBackup::BuildBackupPath(sFilePath, FBS_OVERWRITE);
	
	if (FileMisc::FileExists(sBackupPath))
	{
		/////////////////////////////////////////////////////////////
		// Permanent logging
		FileMisc::LogText(_T("Backup file found: %s"), sBackupPath);
		/////////////////////////////////////////////////////////////

		double dBackupSize = FileMisc::GetFileSize(sBackupPath);

		// Check for valid backup
		if (dBackupSize > 0.0)
		{
			/////////////////////////////////////////////////////////////
			// Permanent logging
			FileMisc::LogText(_T("  Backup file has non-zero length"));
			/////////////////////////////////////////////////////////////

			double dSize = FileMisc::GetFileSize(sFilePath);

			if (dSize == 0.0) // definitely a bad save -> copy over backup
			{
				/////////////////////////////////////////////////////////////
				// Permanent logging
				FileMisc::LogText(_T("    Taskfile '%s' has zero length, replacing with backup"));
				/////////////////////////////////////////////////////////////

				FileMisc::DeleteFile(sFilePath, TRUE);
				FileMisc::MoveFile(sBackupPath, sFilePath);
			}
			else
			{
				/////////////////////////////////////////////////////////////
				// Permanent logging
				FileMisc::LogText(_T("    Taskfile has non-zero length"));
				/////////////////////////////////////////////////////////////

				time64_t tMod = FileMisc::GetFileLastModified(sFilePath);
				time64_t tBackupMod = FileMisc::GetFileLastModified(sBackupPath);

				if (tMod >= tBackupMod) // file is newer than backup
				{
					/////////////////////////////////////////////////////////////
					// Permanent logging
					FileMisc::LogText(_T("      Taskfile is newer than backup, deleting backup"));
					/////////////////////////////////////////////////////////////

					FileMisc::DeleteFile(sBackupPath, TRUE);
				}
				else // Different sizes and dates -> prompt
				{
					CEnString sMessage(IDS_BACKUPFILEFOUND, sFilePath);
					int nRet = AfxMessageBox(sMessage, MB_YESNOCANCEL);

					switch (nRet)
					{
					case IDYES:
						/////////////////////////////////////////////////////////////
						// Permanent logging
						FileMisc::LogText(_T("      User confirmed to replace taskfile with backup"));
						/////////////////////////////////////////////////////////////

						FileMisc::DeleteFile(sFilePath, TRUE);
						FileMisc::MoveFile(sBackupPath, sFilePath, TRUE, TRUE);
						break;

					case IDNO: // keep the backup just in case
						{
							/////////////////////////////////////////////////////////////
							// Permanent logging
							FileMisc::LogText(_T("      User confirmed to keep taskfile, renaming backup"));
							/////////////////////////////////////////////////////////////

							CString sRename = CFileBackup::BuildBackupPath(sBackupPath);
							FileMisc::MoveFile(sBackupPath, sRename, TRUE, TRUE);
						}
						break;

					case IDCANCEL:
						return FALSE; // do nothing
					}
				}
			}
		}
		else
		{
			/////////////////////////////////////////////////////////////
			// Permanent logging
			FileMisc::LogText(_T("  Backup file has zero length, deleting backup"));
			/////////////////////////////////////////////////////////////

			FileMisc::DeleteFile(sBackupPath, TRUE);
		}
	}
	
	return TRUE;
}

// thin wrapper
TDC_FILE CToDoCtrl::Load(const CString& sFilePath, LPCTSTR szDefaultPassword)
{
	CTaskFile unused;
	return Load(sFilePath, unused, szDefaultPassword);
}

TDC_FILE CToDoCtrl::Load(const CString& sFilePath, CTaskFile& tasks/*out*/, LPCTSTR szDefaultPassword)
{
	ASSERT (GetSafeHwnd());
	
	if (!GetSafeHwnd())
		return TDCF_OTHER;
	
	if (!CheckRestoreBackupFile(sFilePath))
		return TDCF_CANCELLED;

	if (!FileMisc::FileExists(sFilePath))
		return TDCF_NOTEXIST;

	SetReadonly(CDriveInfo::IsReadonlyPath(sFilePath) > 0);

	// PERMANENT LOGGING //////////////////////////////////////////////
	CScopedLogTimer log(_T("CToDoCtrl::Load(%s)"), FileMisc::GetFileNameFromPath(sFilePath));
	log.LogStart();
	///////////////////////////////////////////////////////////////////
	
	if (tasks.Load(sFilePath, NULL, FALSE)) // don't decrypt
	{
		m_sourceControl.Initialise(tasks);

		BOOL bWantCheckout = (HasStyle(TDCS_CHECKOUTONLOAD) &&
								m_sourceControl.IsSourceControlled() && 
								!m_sourceControl.IsCheckedOut());

		if (bWantCheckout)
		{
			// Checkout has to happen before the tasks are decrypted
			// but it will fail if we are currently delay loaded.
			// And m_bDelayLoaded is not cleared until LoadTasks(...)
			// which happens only after decryption.
			// So we temporarily clear m_bDelayLoaded just for this case.
			CAutoFlag af(m_bDelayLoaded, FALSE);

			// Clear flag if check-out failed
			bWantCheckout = (m_sourceControl.CheckOut(tasks, CString(), FALSE, sFilePath) == TDCF_SUCCESS);
		}

		if (m_sPassword.IsEmpty())
			m_sPassword = szDefaultPassword;

		if (tasks.Decrypt(m_sPassword))
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

			// PERMANENT LOGGING //////////////////////////////////////////////
			log.LogTimeElapsed(_T("   CToDoCtrl::Load(before LoadTasks"));
			///////////////////////////////////////////////////////////////////

			LoadTasks(tasks);
					
			// PERMANENT LOGGING //////////////////////////////////////////////
			log.LogTimeElapsed(_T("   CToDoCtrl::Load(LoadTasks"));
			///////////////////////////////////////////////////////////////////

			// Reload only if they've already been initialised
			if (m_ilTaskIcons.GetSafeHandle())
				LoadTaskIcons();
	
			SetModified(FALSE);
					
			// PERMANENT LOGGING //////////////////////////////////////////////
			log.LogTimeElapsed(_T("   CToDoCtrl::Load(rest)"));
			///////////////////////////////////////////////////////////////////

			return TDCF_SUCCESS;
		} 
		// user cancelled so revert any check-out
		else if (bWantCheckout)
		{
			VERIFY(m_sourceControl.CheckIn(tasks) == TDCF_SUCCESS);
		}
	}

	// else do error handling
	return TDC::MapTaskfileError(tasks.GetLastFileError());
}

BOOL CToDoCtrl::DelayLoad(const CString& sFilePath, COleDateTime& dtEarliestDue)
{
	// Can only delay load an unloaded tasklist
	if (!HasFilePath())
	{
		CTaskFile temp;
	
		if (temp.LoadHeader(sFilePath) && (temp.IsArchive() || temp.GetEarliestTaskDueDate(dtEarliestDue)))
		{
			SetProjectName(temp.GetProjectName());
			temp.GetCustomAttributeDefs(m_aCustomAttribDefs);
		}

		SetFilePath(sFilePath);
		SetModified(FALSE);

		m_bDelayLoaded = TRUE;
	}

	return m_bDelayLoaded;
}

BOOL CToDoCtrl::LoadTasks(const CTaskFile& tasks)
{
	ASSERT (GetSafeHwnd());
	
	if (!GetSafeHwnd())
		return FALSE;

	// PERMANENT LOGGING //////////////////////////////////////////////
	CScopedLogTimer log;
	///////////////////////////////////////////////////////////////////

	// save visible state
	BOOL bHidden = !IsWindowVisible();

	// save current state if reloading same file
	TDCSELECTIONCACHE cache;
	CPreferences prefs;

	if (!m_bDelayLoaded && (m_sLastSavePath == tasks.GetFilePath()))
	{
		CacheTreeSelection(cache);

		SaveTasksState(prefs);
		SaveSplitPos(prefs);
		SaveAttributeVisibility(prefs);
		m_findReplace.SaveState(prefs);

		// PERMANENT LOGGING //////////////////////////////////////////////
		log.LogTimeElapsed(_T("      CToDoCtrl::LoadTasks(Save state)"));
		///////////////////////////////////////////////////////////////////
	}	
	
	// Update XML headers if not already unicode
	m_sXslHeader = tasks.GetXslHeader();
	m_sXmlHeader = tasks.GetXmlHeader();

	if (!XMLHeaderIsUnicode(m_sXmlHeader))
		m_sXmlHeader = DEFAULT_UNICODE_HEADER;

	// file header info
	m_nFileFormat = tasks.GetFileFormat();
	m_nFileVersion = tasks.GetFileVersion();
	m_sProjectName = tasks.GetProjectName();
	m_bArchive = tasks.IsArchive();
	m_dtLastTaskMod = tasks.GetLastModifiedOle();
	
	// next unique ID
	m_dwNextUniqueID = tasks.GetNextUniqueID();
	
	// backwards compatibility
	if (!m_dwNextUniqueID)
		m_dwNextUniqueID = 1;
	
	m_bDelayLoaded = FALSE;

	SetFilePath(tasks.GetFilePath());
	LoadGlobals(tasks);

	// PERMANENT LOGGING //////////////////////////////////////////////
	log.LogTimeElapsed(_T("      CToDoCtrl::LoadTasks(globals)"));
	///////////////////////////////////////////////////////////////////

	LoadCustomAttributeDefinitions(tasks);

	// PERMANENT LOGGING //////////////////////////////////////////////
	log.LogTimeElapsed(_T("      CToDoCtrl::LoadTasks(custom attrib)"));
	///////////////////////////////////////////////////////////////////

	LoadSplitPos(prefs);
	// PERMANENT LOGGING //////////////////////////////////////////////
	log.LogTimeElapsed(_T("      CToDoCtrl::LoadTasks(split pos)"));
	///////////////////////////////////////////////////////////////////

	LoadDefaultRecurrence(prefs);
	// PERMANENT LOGGING //////////////////////////////////////////////
	log.LogTimeElapsed(_T("      CToDoCtrl::LoadTasks(recurrence)"));
	///////////////////////////////////////////////////////////////////

	LoadAttributeVisibility(tasks, prefs);
	// PERMANENT LOGGING //////////////////////////////////////////////
	log.LogTimeElapsed(_T("      CToDoCtrl::LoadTasks(attrib vis)"));
	///////////////////////////////////////////////////////////////////

	m_findReplace.LoadState(prefs);

	if (tasks.IsPasswordPromptingDisabled())
		m_styles[TDCS_DISABLEPASSWORDPROMPTING] = TRUE;
	
	// PERMANENT LOGGING //////////////////////////////////////////////
	log.LogTimeElapsed(_T("      CToDoCtrl::LoadTasks(Process header)"));
	///////////////////////////////////////////////////////////////////

	if (tasks.GetTaskCount())
	{
		HOLD_REDRAW(*this, m_taskTree.Tree());
	
		HTREEITEM htiSel = NULL;
		DWORD dwFirstVis = GetTaskID(m_taskTree.Tree().GetFirstVisibleItem());
		HTREEITEM htiFirst = SetAllTasks(tasks);

		// PERMANENT LOGGING //////////////////////////////////////////////
		log.LogTimeElapsed(_T("      CToDoCtrl::LoadTasks(Build tree)"));
		///////////////////////////////////////////////////////////////////

		if (m_taskTree.GetItemCount())
		{
			// restore last tree state
			htiSel = LoadTasksState(prefs);

			// PERMANENT LOGGING //////////////////////////////////////////////
			log.LogTimeElapsed(_T("      CToDoCtrl::LoadTasks(Restore state)"));
			///////////////////////////////////////////////////////////////////
			
			// redo last sort
			if (IsSorting())
				Resort();
		}

		// scroll to first visible item
		if (dwFirstVis)
		{
			HTREEITEM htiFirstVis = m_taskTree.GetItem(dwFirstVis);
			
			if (htiFirstVis)
				m_taskTree.Tree().SelectSetFirstVisible(htiFirstVis);
		}
		
		// restore last selected state
		if (!cache.IsEmpty())
		{
			RestoreTreeSelection(cache);
		}
		else
		{
			if (!htiSel)
				htiSel = htiFirst;
			
			if (htiSel)
			{
				SelectItem(htiSel);
				m_taskTree.EnsureSelectionVisible(FALSE);
			}
		}
	}
	else
	{
		m_bModified = FALSE;
	}

	// reset last taskedit
	m_dtLastTaskMod = COleDateTime::GetCurrentTime();

	Resize();
	UpdateData(FALSE);

	// restore previously visibility
	if (bHidden)
		ShowWindow(SW_HIDE);
	
	// PERMANENT LOGGING //////////////////////////////////////////////
	log.LogTimeElapsed(_T("      CToDoCtrl::LoadTasks(Rest)"));
	///////////////////////////////////////////////////////////////////
	
	return TRUE;
}

int CToDoCtrl::GetArchivableTasks(CTaskFile& tasks, BOOL bSelectedOnly) const
{
	if (bSelectedOnly)
		return GetSelectedTasks(tasks, TDCGETTASKS(TDCGT_ALL, TDCGSTF_ALLPARENTS));

	// else
	return GetTasks(tasks, TDCGT_DONE);
}

void CToDoCtrl::RemoveArchivedTasks(const CTaskFile& tasks, TDC_ARCHIVE nRemove, BOOL bRemoveFlagged)
{
	if (IsReadOnly() || (nRemove == TDC_REMOVENONE) || !tasks.GetTaskCount())
		return; // nothing to do

	CPreferences prefs;
	SaveTasksState(prefs);
	
	IMPLEMENT_DATA_UNDO(m_data, TDCUAT_ARCHIVE);
	
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
		SetModified(TDCA_ARCHIVE);
}

BOOL CToDoCtrl::RemoveArchivedTask(const CTaskFile& tasks, HTASKITEM hTask, TDC_ARCHIVE nRemove, BOOL bRemoveFlagged)
{
	DWORD dwTaskID = tasks.GetTaskID(hTask);
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;

	if (!m_data.GetTrueTask(dwTaskID, pTDI, pTDS))
	{
		ASSERT(0);
		return FALSE;
	}

	BOOL bRemove = FALSE;
	BOOL bDone = m_calculator.IsTaskDone(pTDI, pTDS, TDCCHECKCHILDREN);

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

			bRemove = !m_matcher.FindTasks(pTDI, pTDS, params, FALSE, aResults);
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
	m_data.DeleteTask(dwTaskID, TRUE); // TRUE == with undo

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
		IMPLEMENT_DATA_UNDO(m_data, TDCUAT_ARCHIVE);

		return DeleteSelectedTask(FALSE, TRUE);
	}

	// else
	return TRUE;
}

BOOL CToDoCtrl::ArchiveTasks(const CString& sArchivePath, const CTaskFile& tasks)
{
	// Can't archive if not checked out
	ASSERT(!IsReadOnly());

	// can't archive archives
	ASSERT(!m_bArchive);
	
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
			int nFileVer = (file.GetFileVersion() + 1);
			tfh.nFileVersion = max(nFileVer, 1);
		}
	}
	else // or initialize first time
	{
		tfh.bArchive = TRUE;
		tfh.sProjectName.Format(_T("%s (%s)"), m_sProjectName, CEnString(IDS_TDC_ARCHIVEPROJECT));
		tfh.sXmlHeader = m_sXmlHeader;
		tfh.sXslHeader = m_sXslHeader;
	}

	SaveCustomAttributeDefinitions(file);
	
	file.Merge(tasks, TRUE, TRUE);
	file.Encrypt(m_sPassword, SFEF_UTF16);
	file.SetHeader(tfh);

	if (SaveTaskfile(file, sArchivePath) != TDCF_SUCCESS)
		return FALSE;

	// If the process took longer than 1 second we rename
	// the archive so that next time it'll start afresh
#ifndef _DEBUG
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
#endif

	return TRUE;
}

HTREEITEM CToDoCtrl::PasteTaskToTree(const CTaskFile& tasks, HTASKITEM hTask, HTREEITEM htiParent, 
										HTREEITEM htiAfter, TDC_RESETIDS nResetID, BOOL bAndSubtasks)
{
	HTREEITEM hti = TVI_ROOT; // default for root item
	
	if (hTask)
	{
		// fixup htiAfter
		if (htiAfter == TVI_LAST)
			htiAfter = TCH().GetLastChildItem(htiParent);

		// add new task to map
		DWORD dwTaskID = tasks.GetTaskID(hTask);

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

		TODOITEM* pTDI = m_data.NewTask(tasks, hTask, &m_tdiDefault);

		if (!pTDI)
		{
			ASSERT(0);
			return NULL;
		}
		
		// add this item to data structure
		m_data.AddTask(dwTaskID, pTDI, dwParentID, dwPrevTaskID);

		// And update inherited attributes
		m_data.ApplyLastInheritedChangeFromParent(dwTaskID, TDCA_ALL);
				
		// add this item to tree
		hti = InsertTreeItem(pTDI, dwTaskID, htiParent, htiAfter, TRUE);
	}

	HTREEITEM htiFirstItem = (hti == TVI_ROOT) ? NULL : hti;
	
	// then children
	if (bAndSubtasks)
	{
		HTASKITEM hChildTask = tasks.GetFirstTask(hTask);
	
		while (hChildTask)
		{
			// RECURSIVE CALL
			HTREEITEM htiChild = PasteTaskToTree(tasks, hChildTask, hti, TVI_LAST, nResetID, bAndSubtasks);
		
			if (!htiFirstItem)
				htiFirstItem = htiChild;
		
			hChildTask = tasks.GetNextTask(hChildTask); // next
		}
	}
		
	return htiFirstItem;
}

HTREEITEM CToDoCtrl::InsertTreeItem(const TODOITEM* pTDI, DWORD dwTaskID, HTREEITEM htiParent, HTREEITEM htiAfter, BOOL bAddToCombos)
{
	HTREEITEM hti = m_taskTree.InsertItem(dwTaskID,	htiParent, htiAfter);
	ASSERT(hti);

	// add unique items to combo-boxes
	if (bAddToCombos)
	{
		Misc::AppendItems(pTDI->aAllocTo, m_tldAll.aAllocTo, TRUE);
		Misc::AppendItems(pTDI->aCategories, m_tldAll.aCategory, TRUE);
		Misc::AppendItems(pTDI->aTags, m_tldAll.aTags, TRUE);

		Misc::AddUniqueItem(pTDI->sAllocBy, m_tldAll.aAllocBy);
		Misc::AddUniqueItem(pTDI->sStatus, m_tldAll.aStatus);
		Misc::AddUniqueItem(pTDI->sVersion, m_tldAll.aVersion);
	}

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
		
		m_findReplace.SaveState(prefs);
		m_findReplace.DestroyDialog();
	}
	
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
	if (IsReadOnly() || IsDelayLoaded())
		return FALSE;

	if (m_bModified)
		return TRUE;
	
	// else do we have outstanding unhandled comments
	return (m_nCommentsState == CS_PENDING);
}

BOOL CToDoCtrl::IsPristine() const 
{ 
	return (!HasFilePath() && !IsModified());
}

void CToDoCtrl::SetModified(TDC_ATTRIBUTE nAttribID, const CDWordArray& aModTaskIDs)
{
	CTDCAttributeMap mapAttribIDs;
	m_taskTree.GetAttributesAffectedByMod(nAttribID, mapAttribIDs);
	
	// Don't allow sorting during find/replace operation because
	// it messes up the order of the items	
	SetModified(mapAttribIDs, aModTaskIDs, !m_findReplace.IsReplacing());
}

BOOL CToDoCtrl::IsNewTaskMod(const CTDCAttributeMap& mapAttribIDs, const CDWordArray& aModTaskIDs) const
{
	return (mapAttribIDs.HasOnly(TDCA_NEWTASK) && (aModTaskIDs.GetSize() == 1));
}

BOOL CToDoCtrl::IsNewTaskTitleEditMod(const CTDCAttributeMap& mapAttribIDs, const CDWordArray& aModTaskIDs) const
{
	return (mapAttribIDs.HasOnly(TDCA_TASKNAME) &&
			(aModTaskIDs.GetSize() == 1) &&
			(aModTaskIDs[0] == m_dwLastAddedID));
}

void CToDoCtrl::SetModified(const CTDCAttributeMap& mapAttribIDs, const CDWordArray& aModTaskIDs, BOOL bAllowResort)
{
	ASSERT(aModTaskIDs.GetSize() || 
		   mapAttribIDs.HasOnly(TDCA_CUSTOMATTRIB) ||
		   mapAttribIDs.HasOnly(TDCA_PASTE) ||
		   mapAttribIDs.HasOnly(TDCA_PROJECTNAME) || 
		   mapAttribIDs.HasOnly(TDCA_UNDO) ||
		   mapAttribIDs.HasOnly(TDCA_POSITION_SAMEPARENT) ||
		   mapAttribIDs.HasOnly(TDCA_POSITION_DIFFERENTPARENT));

	if (IsReadOnly())
		return;
	
	SetModified(TRUE);

	if (mapAttribIDs.Has(TDCA_PASTE))
		UpdateAutoListData();
	
	// For new tasks we want to do as little processing as possible 
	// so as not to delay the appearance of the title edit field.
	BOOL bNewTask = IsNewTaskMod(mapAttribIDs, aModTaskIDs);
	BOOL bNewTaskTitleEdit = IsNewTaskTitleEditMod(mapAttribIDs, aModTaskIDs);

	if (bNewTaskTitleEdit || mapAttribIDs.Has(TDCA_PASTE))
		m_taskTree.SetLargestTaskID(m_dwNextUniqueID);

	if (!bNewTask)
		m_taskTree.SetModified(mapAttribIDs, bAllowResort);

	TDCNOTIFYMOD mod(mapAttribIDs, aModTaskIDs);
	GetParent()->SendMessage(WM_TDCN_MODIFY, (WPARAM)GetSafeHwnd(), (LPARAM)&mod);

	// if this was the project name being edited make sure
	// the focus is set back to the name
	if (mapAttribIDs.Has(TDCA_PROJECTNAME))
		GetDlgItem(IDC_PROJECTNAME)->SetFocus();
	else
		m_idleTasks.RefreshAttributeValues(mapAttribIDs);

	if (mapAttribIDs.Has(TDCA_LOCK))
		EnableDisableComments();
}

LRESULT CToDoCtrl::OnCommentsChange(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	UpdateTask(TDCA_COMMENTS);
	return 0L;
}

LRESULT CToDoCtrl::OnCommentsGetTooltip(WPARAM wp, LPARAM lp)
{
	return OnGetLinkTooltip(wp, lp);
}

LRESULT CToDoCtrl::OnTDCGetLinkTooltip(WPARAM wp, LPARAM lp)
{
	return OnGetLinkTooltip(wp, lp);
}

LRESULT CToDoCtrl::OnGetLinkTooltip(WPARAM /*wp*/, LPARAM lp)
{
	ASSERT(lp);

	LPCTSTR szTooltip = (LPCTSTR)GetParent()->SendMessage(WM_TDCM_GETLINKTOOLTIP, (WPARAM)GetSafeHwnd(), lp);

	if (!Misc::IsEmpty(szTooltip))
		return (LRESULT)szTooltip;
	
	static CString sTooltip;
	LPCTSTR szURL = (LPCTSTR)lp;

	if (WebMisc::DecodeFileURI(szURL, sTooltip))
	{
		FileMisc::ExpandPathEnvironmentVariables(sTooltip);
		sTooltip = FileMisc::GetFullPath(sTooltip, m_taskTree.GetTasklistFolder());

		if (sTooltip != szURL)
			return (LRESULT)(LPCTSTR)sTooltip;
	}

	return 0;
}

LRESULT CToDoCtrl::OnCommentsGetAttributeList(WPARAM wParam, LPARAM lParam)
{
	TDC_ATTRIBUTE nAttribID = (TDC_ATTRIBUTE)wParam;
	TCHAR cSep = (TCHAR)lParam;

	static CString sList;
	sList = m_tldAll.Format(nAttribID, cSep);

	return (LRESULT)(LPCTSTR)sList;
}

LRESULT CToDoCtrl::OnCommentsKillFocus(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	TDCSELECTIONCACHE cache;
	CacheTreeSelection(cache, FALSE);

	if (!HandleUnsavedComments())
	{
		m_ctrlComments.SetFocus();
		RestoreTreeSelection(cache);
	}

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
	SetModified(TDCA_PROJECTNAME);
}

LRESULT CToDoCtrl::OnTreeDragAbort(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return m_treeDragDrop.ProcessMessage(GetCurrentMessage());
}

LRESULT CToDoCtrl::OnTreeDragEnter(WPARAM /*wParam*/, LPARAM lParam)
{
	ASSERT(!IsReadOnly());

	if (!m_treeDragDrop.ProcessMessage(GetCurrentMessage()))
		return FALSE;

	const DRAGDROPINFO* pDDI = (DRAGDROPINFO*)lParam;

	return IsSelectedTaskMoveEnabled(pDDI->bLeftDrag ? TDCM_LEFTDRAG : TDCM_RIGHTDRAG);
}

LRESULT CToDoCtrl::OnTreeDragPreMove(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return m_treeDragDrop.ProcessMessage(GetCurrentMessage());
}

LRESULT CToDoCtrl::OnTreeDragOver(WPARAM /*wParam*/, LPARAM lParam)
{
	ASSERT(!IsReadOnly());

	if (m_treeDragDrop.ProcessMessage(GetCurrentMessage()) == DD_DROPEFFECT_NONE)
		return DD_DROPEFFECT_NONE;

	HTREEITEM htiOver, htiAfter;
		
	if (!m_treeDragDrop.GetDropTarget(htiOver, htiAfter, m_bDragDropSubtasksAtTop))
		return DD_DROPEFFECT_NONE;

	const DRAGDROPINFO* pDDI = (DRAGDROPINFO*)lParam;
	DWORD dwTargetID = GetTaskID(htiOver);

	return GetSelectedTasksDropEffect(dwTargetID, pDDI->bLeftDrag);
}

LRESULT CToDoCtrl::OnTreeDragDrop(WPARAM /*wParam*/, LPARAM lParam)
{
	ASSERT(!IsReadOnly());

	if (!m_treeDragDrop.ProcessMessage(GetCurrentMessage()))
		return FALSE;

	HTREEITEM htiDropOn, htiDropAfter;
		
	if (!m_treeDragDrop.GetDropTarget(htiDropOn, htiDropAfter, m_bDragDropSubtasksAtTop))
		return FALSE;

	const DRAGDROPINFO* pDDI = (DRAGDROPINFO*)lParam;
	DWORD dwTargetID = GetTaskID(htiDropOn);

	DD_DROPEFFECT nDrop = GetSelectedTasksDropEffect(dwTargetID, pDDI->bLeftDrag);

	if (nDrop == DD_DROPEFFECT_NONE)
		return FALSE;
			
	// Handle right-click drop context menu
	if (!pDDI->bLeftDrag)
	{
		ASSERT(nDrop == DD_DROPEFFECT_MOVE);

		CEnMenu menu;

		if (menu.LoadMenu(IDR_TREEDRAGDROP, *this, TRUE))
		{
			if (m_treeDragDrop.IsDropOn() && dwTargetID)
				m_taskTree.SelectDropTarget(htiDropOn);

			// Menu items are enabled by default so we just need
			// to disable as required
			CMenu* pSubMenu = menu.GetSubMenu(0);
			UINT nDisabled = (MF_BYCOMMAND | MF_GRAYED);

			if (!CanDropSelectedTasks(DD_DROPEFFECT_COPY, htiDropOn))
				pSubMenu->EnableMenuItem(ID_TDD_COPYTASK, nDisabled);

			if (!CanDropSelectedTasks(DD_DROPEFFECT_MOVE, htiDropOn))
				pSubMenu->EnableMenuItem(ID_TDD_MOVETASK, nDisabled);

			if (!CanDropSelectedTasks(DD_DROPEFFECT_LINK, htiDropOn))
				pSubMenu->EnableMenuItem(ID_TDD_REFTASK, nDisabled);

			// disable dependency and file links if dropping in-between tasks
			// or dropping on locked tasks
			BOOL bTargetIsLocked = m_calculator.IsTaskLocked(dwTargetID);

			if (bTargetIsLocked || !m_treeDragDrop.IsDropOn())
			{
				pSubMenu->EnableMenuItem(ID_TDD_SETTASKDEPENDENCY, nDisabled);
				pSubMenu->EnableMenuItem(ID_TDD_ADDTASKDEPENDENCY, nDisabled);
				pSubMenu->EnableMenuItem(ID_TDD_ADDFILELINK, nDisabled);
			}
			else if (m_taskTree.SelectionHasTask(dwTargetID, TRUE)) // prevent dependency on self
			{
				pSubMenu->EnableMenuItem(ID_TDD_SETTASKDEPENDENCY, nDisabled);
				pSubMenu->EnableMenuItem(ID_TDD_ADDTASKDEPENDENCY, nDisabled);
			}

			// Display the menu, returning the selected command
			CPoint ptCursor(pDDI->pt);
			::ClientToScreen(pDDI->hwndTarget, &ptCursor);

			UINT nCmdID = ::TrackPopupMenu(*pSubMenu, TPM_RETURNCMD | TPM_LEFTALIGN | TPM_LEFTBUTTON,
										   ptCursor.x, ptCursor.y, 0, *this, NULL);

			nDrop = DD_DROPEFFECT_NONE;

			switch (nCmdID)
			{
				case ID_TDD_REFTASK:
					nDrop = DD_DROPEFFECT_LINK;
					break;

				case ID_TDD_COPYTASK:
					nDrop = DD_DROPEFFECT_COPY;
					break;

				case ID_TDD_MOVETASK:
					nDrop = DD_DROPEFFECT_MOVE;
					break;

				case ID_TDD_SETTASKDEPENDENCY:
				case ID_TDD_ADDTASKDEPENDENCY:
					{
						IMPLEMENT_DATA_UNDO_EDIT(m_data);

						CDWordArray aTaskIDs;
						int nTask = GetSelectedTaskIDs(aTaskIDs, TRUE);

						CTDCDependencyArray aDepends;
						aDepends.Set(aTaskIDs);

						m_data.SetTaskDependencies(dwTargetID, aDepends, (nCmdID == ID_TDD_ADDTASKDEPENDENCY));

						CDWordArray aModTaskIDs;
						aModTaskIDs.Add(dwTargetID);

						SetModified(TDCA_DEPENDENCY, aModTaskIDs);
					}
					break;

				case ID_TDD_ADDFILELINK:
					{
						IMPLEMENT_DATA_UNDO_EDIT(m_data);

						CDWordArray aTaskIDs;
						int nTask = GetSelectedTaskIDs(aTaskIDs, TRUE);

						CStringArray aFileLinks;
						aFileLinks.SetSize(nTask);

						while (nTask--)
							aFileLinks[nTask] = TDCTASKLINK::Format(aTaskIDs[nTask], TRUE); // as URL

						CDWordArray aModTaskIDs;
						aModTaskIDs.Add(dwTargetID);

						m_data.SetTaskFileLinks(dwTargetID, aFileLinks, TRUE);
						SetModified(TDCA_FILELINK, aModTaskIDs);
					}
					break;

				default:
					break;
			}

			m_taskTree.SelectDropTarget(NULL);
			m_taskTree.RedrawColumns();
		}
	}

	return DropSelectedTasks(nDrop, htiDropOn, htiDropAfter);
}

BOOL CToDoCtrl::CanDropSelectedTasks(DD_DROPEFFECT nDrop, HTREEITEM htiDropTarget) const
{
	DWORD dwTargetID = GetTaskID(htiDropTarget);

	if (!IsValidSelectedTaskMoveTarget(dwTargetID, nDrop))
		return FALSE;

	switch (nDrop)
	{
		case DD_DROPEFFECT_COPY:
			return TRUE;

		case DD_DROPEFFECT_LINK:
			return TRUE;

		case DD_DROPEFFECT_MOVE:
			return m_taskTree.IsSelectedTaskMoveEnabled(TDCM_NONDRAG);

		case DD_DROPEFFECT_NONE:
			return FALSE;
	}

	// all else
	ASSERT(0);
	return FALSE;
}

BOOL CToDoCtrl::DropSelectedTasks(DD_DROPEFFECT nDrop, HTREEITEM htiDropTarget, HTREEITEM htiDropAfter)
{
	if (!CanDropSelectedTasks(nDrop, htiDropTarget))
		return FALSE;

	CLockUpdates lu(*this);

	switch (nDrop)
	{
	case DD_DROPEFFECT_COPY:
	case DD_DROPEFFECT_LINK:
		{
			// copy selection as xml
			CTaskFile tasks;
			TDCGETTASKS filter;

			// if pasting references then we don't want all subtasks just 
			// the ones actually selected
			BOOL bDropRefs = (nDrop == DD_DROPEFFECT_LINK);

			if (bDropRefs)
				filter.dwFlags = TDCGSTF_NOTSUBTASKS;

			if (GetSelectedTasks(tasks, filter))
			{
				IMPLEMENT_DATA_UNDO(m_data, TDCUAT_COPY);
				HOLD_REDRAW(NULL, m_taskTree);

				// fix up the dependencies of the copied tasks
				if (bDropRefs)
					PrepareTasksForPasteAsRef(tasks);
				else
					PrepareTasksForPaste(tasks, TDCR_YES, TRUE);

				// then add them with impunity!
				PasteTasksToTree(tasks, htiDropTarget, htiDropAfter, TDCR_NO, TRUE);

				// if the parent was marked as done and this task is not
				// then mark the parent as incomplete too
				DWORD dwDestParentID = m_taskTree.GetTaskID(htiDropTarget);
				FixupParentCompletion(dwDestParentID);
			}
		}
		break;

	case DD_DROPEFFECT_MOVE:
		{
			IMPLEMENT_DATA_UNDO(m_data, TDCUAT_MOVE);
			HOLD_REDRAW(NULL, m_taskTree);

			DWORD dwSrcParentID = GetSelectedTaskParentID(); // zero for multiple parents
			
			DWORD dwDestParentID = m_taskTree.GetTaskID(htiDropTarget);
			DWORD dwDestPrevSiblingID = m_taskTree.GetTaskID(htiDropAfter);

			CDWordArray aSelTaskIDs;
			DWORD dwUnused = 0;

			m_taskTree.GetSelectedTaskIDs(aSelTaskIDs, dwUnused, TRUE, TRUE); // ordered

			if (m_data.MoveTasks(aSelTaskIDs, dwDestParentID, dwDestPrevSiblingID))
			{
				m_taskTree.MoveSelection(htiDropTarget, htiDropAfter);

				// if the parent was marked as done and this task is not
				// then mark the parent as incomplete too
				FixupParentCompletion(dwDestParentID);

				if (m_taskTree.SelectionHasSameParent() && (dwSrcParentID == dwDestParentID))
					SetModified(TDCA_POSITION_SAMEPARENT);
				else
					SetModified(TDCA_POSITION_DIFFERENTPARENT);
			}
		}
		break;

	case DD_DROPEFFECT_NONE:
	default:
		ASSERT(0);
		return FALSE;
	}

	return TRUE;
}

void CToDoCtrl::FixupParentCompletion(DWORD dwParentID)
{
	VERIFY(m_data.FixupParentCompletion(dwParentID, !m_sCompletionStatus.IsEmpty()));
}

void CToDoCtrl::PrepareTasksForPaste(CTaskFile& tasks, TDC_RESETIDS nResetID, BOOL bResetCreation) const
{
	if (nResetID == TDCR_NO || (tasks.GetTaskCount() == 0))
		return; // nothing to do

	// work out what new IDs we need and map them to the existing IDs
	CMapID2ID mapID;
	mapID.InitHashTable(tasks.GetTaskCount());

	DWORD dwNextID(max(tasks.GetNextUniqueID(), m_dwNextUniqueID));
	BuildTaskIDMapForPaste(tasks, tasks.GetFirstTask(NULL), dwNextID, mapID, nResetID, TRUE);

	// then fix them
	PrepareTasksForPaste(tasks, tasks.GetFirstTask(NULL), bResetCreation, mapID, TRUE);
}

void CToDoCtrl::PrepareTasksForPasteAsRef(CTaskFile& tasks, const CDWordArray& aSelTaskIDs) const
{
	if (tasks.GetTaskCount() == 0)
		return; // nothing to do

	// remove tasks not originally selected
	if (aSelTaskIDs.GetSize())
	{
		RemoveNonSelectedTasks(aSelTaskIDs, tasks, tasks.GetFirstTask());

		if (tasks.GetTaskCount() == 0)
			return; // nothing further to do
	}

	// pre-process the tasks to add themselves
	// as a reference, and then to clear the task ID
	// so that it gets a newly allocated one
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

void CToDoCtrl::PrepareTasksForPaste(CTaskFile& tasks, HTASKITEM hTask, BOOL bResetCreation, const CMapID2ID& mapID, BOOL bAndSiblings) const
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
		if (bResetCreation)
		{
			tasks.SetTaskCreationDate(hTask, COleDateTime::GetCurrentTime());
			tasks.SetTaskCreatedBy(hTask, m_tdiDefault.sCreatedBy);
		}
	}

	// and it's reference ID
	DWORD dwRefTaskID = tasks.GetTaskReferenceID(hTask);

	if (mapID.Lookup(dwRefTaskID, dwNewID))
	{
		ASSERT (dwNewID); // sanity check
		tasks.SetTaskReferenceID(hTask, dwNewID);
	}

	// dependencies first
	CTDCDependencyArray aDepends;
	int nDepend = tasks.GetTaskDependencies(hTask, aDepends);

	BOOL bChanged = FALSE;

	while (nDepend--)
	{
		TDCDEPENDENCY& depend = aDepends[nDepend];

		if (PrepareTaskLinkForPaste(depend, mapID))
			bChanged = TRUE;
	}

	// update taskfile if any dependency was changed
	if (bChanged)
		tasks.SetTaskDependencies(hTask, aDepends);
	
	// file references
	CStringArray aFileLinks;
	int nFile = tasks.GetTaskFileLinks(hTask, aFileLinks);

	bChanged = FALSE;

	while (nFile--)
	{
		CString sFileLink = aFileLinks[nFile];

		if (PrepareTaskLinkForPaste(sFileLink, mapID))
		{
			aFileLinks[nFile] = sFileLink;
			bChanged = TRUE;
		}
	}

	// update taskfile if any file link was changed
	if (bChanged)
		tasks.SetTaskFileLinks(hTask, aFileLinks);

	// children
	PrepareTasksForPaste(tasks, tasks.GetFirstTask(hTask), bResetCreation, mapID, TRUE);

	// siblings
	// handle sibling tasks WITHOUT RECURSION
	if (bAndSiblings)
	{
		hTask = tasks.GetNextTask(hTask);
		
		while (hTask)
		{
			// FALSE == don't recurse on siblings
			PrepareTasksForPaste(tasks, hTask, bResetCreation, mapID, FALSE);
			
			hTask = tasks.GetNextTask(hTask);
		}
	}
}

BOOL CToDoCtrl::PrepareTaskLinkForPaste(CString& sLink, const CMapID2ID& mapID) const
{
	DWORD dwTaskID;
	CString sFile;

	BOOL bURL = TDCTASKLINK::IsTaskLink(sLink, TRUE);

	VERIFY(ParseTaskLink(sLink, bURL, dwTaskID, sFile));
	
	if (!dwTaskID)
		return FALSE;

	// does the ID need changing
	DWORD dwNewID = 0;
	
	if (mapID.Lookup(dwTaskID, dwNewID))
	{
		ASSERT (dwNewID); // sanity check
		
		// make sure the file path matches us
		if (!sFile.IsEmpty() && HasFilePath())
		{
			if (!FileMisc::IsSamePath(sFile, m_sLastSavePath))
				return FALSE;
		}
		
		// update link
		sLink = TDCTASKLINK::Format(dwNewID, bURL, sFile);
		return TRUE;
	}

	return FALSE;
}

BOOL CToDoCtrl::PrepareTaskLinkForPaste(TDCDEPENDENCY& depend, const CMapID2ID& mapID) const
{
	if (!depend.dwTaskID)
		return FALSE;

	// does the ID need changing
	DWORD dwNewID = 0;

	if (mapID.Lookup(depend.dwTaskID, dwNewID))
	{
		ASSERT(dwNewID); // sanity check

		// make sure the file path matches us
		if (!depend.IsLocal() && HasFilePath())
		{
			CString sDependPath = depend.GetFullPath(GetLastSaveFolder());

			if (!FileMisc::IsSamePath(sDependPath, m_sLastSavePath))
				return FALSE;
		}

		// update link
		depend.dwTaskID = dwNewID;
		return TRUE;
	}

	return FALSE;
}

BOOL CToDoCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if (m_infoTip.GetSafeHwnd())
		m_infoTip.FilterToolTipMessage(pMsg);

	if (m_ctrlComments.ProcessMessage(pMsg))
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
						SetFocus(TDCSF_TASKVIEW);

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

	Flush(); // end any editing action
	SetFocus(TDCSF_TASKVIEW); // else datetime controls get their focus screwed

	IMPLEMENT_DATA_UNDO(m_data, TDCUAT_MOVE);

	DWORD dwDestParentID = 0, dwDestPrevSiblingID = 0;
	VERIFY(m_taskTree.GetInsertLocation(nDirection, dwDestParentID, dwDestPrevSiblingID));

	// Get selected tasks without duplicate subtasks
	// because the subtasks will be moved with their parents
	CDWordArray aSelTaskIDs;
	DWORD dwUnused;
	m_taskTree.GetSelectedTaskIDs(aSelTaskIDs, dwUnused, TRUE, TRUE); // ordered

	// Move the associated tree items
	CLockUpdates lu(*this);
	HOLD_REDRAW(*this, m_taskTree);

	if (!m_taskTree.MoveSelection(nDirection))
	{
		ASSERT(0);
		return FALSE;
	}

	// move the tasks
	if (!m_data.MoveTasks(aSelTaskIDs, dwDestParentID, dwDestPrevSiblingID))
	{
		ASSERT(0);
		return FALSE;
	}

	// refresh parent states if moving to the right (adding subtasks)
	if (nDirection == TDCM_RIGHT)
		FixupParentCompletion(dwDestParentID);

	if ((nDirection == TDCM_DOWN) || (nDirection == TDCM_UP))
		SetModified(TDCA_POSITION_SAMEPARENT);
	else
		SetModified(TDCA_POSITION_DIFFERENTPARENT);

	return TRUE;
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
	m_taskTree.SetPriorityColors(aColors);
	m_ctrlAttributes.SetPriorityColors(aColors);
}

void CToDoCtrl::SetProjectName(const CString& sProjectName)
{
	if (m_sProjectName != sProjectName)
	{
		m_sProjectName = sProjectName;
		m_bModified = TRUE;

		if (GetSafeHwnd())
			UpdateData(FALSE);
	}
}

BOOL CToDoCtrl::GetArchivePath(CString& sArchivePath) const
{
	// can't archive archives
	if (!m_bArchive && HasFilePath())
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
		if (HasFilePath())
		{
			sProjectName = FileMisc::GetFileNameFromPath(m_sLastSavePath, FALSE);
		}
		else
		{
			sProjectName = CEnString(IDS_TDC_UNTITLEDFILE);
			Misc::Trim(sProjectName);

			if (nUntitledIndex > 0)
				sProjectName += Misc::Format(nUntitledIndex);
		}
	}
	
	return sProjectName;
}

void CToDoCtrl::SetFilePath(const CString& sPath) 
{ 
	m_sLastSavePath = sPath; 
	m_ctrlComments.SetPreferencesFilePath(sPath);
	
	CString sFolder(FileMisc::GetFolderFromFilePath(sPath));

	m_taskTree.SetTasklistFolder(sFolder);
	m_ctrlAttributes.SetCurrentFolder(sFolder);
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

				if (FileMisc::HasExtension(sFile, _T("xsl")))
					return GetFullPath(sFile);
			}
		}
	}

	// not found
	return _T("");
}

void CToDoCtrl::SetCommentsSize(int nSize)
{
	if (nSize != m_nCommentsSize)
	{
		m_nCommentsSize = nSize;
		Resize();
	}
}

void CToDoCtrl::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	Flush();

	CRuntimeDlg::OnContextMenu(pWnd, point);
}

TDC_HITTEST CToDoCtrl::HitTest(const CPoint& ptScreen, TDC_HITTESTREASON /*nReason*/) const
{
	return m_taskTree.HitTest(ptScreen);
}

DWORD CToDoCtrl::HitTestTask(const CPoint& ptScreen, TDC_HITTESTREASON nReason) const
{
	return m_taskTree.HitTestTask(ptScreen, (nReason == TDCHTR_INFOTIP));
}

TDC_COLUMN CToDoCtrl::HitTestColumn(const CPoint& ptScreen) const
{
	return m_taskTree.HitTestColumn(ptScreen);
}

LRESULT CToDoCtrl::OnTDCNotifyAutoComboAddDelete(WPARAM wp, LPARAM /*lp*/)
{
	TDC_ATTRIBUTE nAttribID = (TDC_ATTRIBUTE)LOWORD(wp);

	switch (nAttribID)
	{
	case TDCA_CATEGORY: 
	case TDCA_TAGS:
	case TDCA_STATUS:
	case TDCA_ALLOCTO:
	case TDCA_ALLOCBY:
	case TDCA_VERSION:
		UpdateAutoListData(nAttribID);
		GetParent()->SendMessage(WM_TDCN_LISTCHANGE, 0, nAttribID);
		break;

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		{
			const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
			GET_CUSTDEF_RET(m_aCustomAttribDefs, nAttribID, pDef, 0L);

			if (pDef->IsAutoList())
			{
				m_ctrlAttributes.GetCustomAttributeAutoListData(pDef->sUniqueID, pDef->aAutoListData);

				GetParent()->SendMessage(WM_TDCN_LISTCHANGE, 0, nAttribID);
				break;
			}
		}
		// all else
		ASSERT(0);
		return 0L;
	}

	// mark ourselves as modified because we now save the lists to the taskfile
	if (HasStyle(TDCS_SAVEUIVISINTASKLIST))
		SetModified(TRUE);

	return 0L;
}

BOOL CToDoCtrl::IsClipboardEmpty(BOOL bCheckID) const
{
	if (CTaskClipboard::IsEmpty())
		return TRUE;

	if (bCheckID)
		return !CTaskClipboard::TasklistIDMatches(GetClipboardID());

	//else
	return FALSE;
}

LRESULT CToDoCtrl::OnTDCCanCopyAttributeValue(WPARAM wParam, LPARAM lParam)
{
	TDC_COLUMN nColID = TDC::MapAttributeToColumn((TDC_ATTRIBUTE)wParam);

	return CanCopyAttributeColumnValues(nColID, TRUE);
}

LRESULT CToDoCtrl::OnTDCCanPasteAttributeValue(WPARAM wParam, LPARAM lParam)
{
	if (!lParam)
	{
		ASSERT(0);
		return FALSE;
	}

	TDC_COLUMN nFromColID = TDCC_NONE;
	TDC_COLUMN nToColID = TDC::MapAttributeToColumn((TDC_ATTRIBUTE)wParam);
	int nUnused;

	if (!CanPasteAttributeColumnValues(nToColID, TRUE, nFromColID, nUnused))
		return 0L;

	TDC_ATTRIBUTE* pFromAttribID = (TDC_ATTRIBUTE*)lParam;
	*pFromAttribID = TDC::MapColumnToAttribute(nFromColID);

	return TRUE;
}

LRESULT CToDoCtrl::OnTDCCopyAttributeValue(WPARAM wParam, LPARAM lParam)
{
	TDC_COLUMN nFromColID = TDC::MapAttributeToColumn((TDC_ATTRIBUTE)wParam);

	return CopyAttributeColumnValues(nFromColID, TRUE);
}

LRESULT CToDoCtrl::OnTDCPasteAttributeValue(WPARAM wParam, LPARAM lParam)
{
	TDC_COLUMN nToColID = TDC::MapAttributeToColumn((TDC_ATTRIBUTE)wParam);

	return PasteAttributeColumnValues(nToColID, TRUE);
}

BOOL CToDoCtrl::CanCopyAttributeColumnValues(TDC_COLUMN nColID, BOOL bSelectedTasksOnly) const
{
	if (!GetUpdateControlsItem())
		return FALSE;

	if (bSelectedTasksOnly && !GetSelectedTaskCount())
		return FALSE;

	return m_attribCopier.CanCopyColumnValues(nColID);
}

BOOL CToDoCtrl::CopyAttributeColumnValues(TDC_COLUMN nColID, BOOL bSelectedTasksOnly) const
{
	if (!CanCopyAttributeColumnValues(nColID, bSelectedTasksOnly))
		return FALSE;

	// Build a task file with sequential IDs of the values
	CDWordArray aTaskIDs;
	int nNumIDs = 0;

	if (bSelectedTasksOnly)
		nNumIDs = GetSelectedTaskIDs(aTaskIDs, TRUE, TRUE); // ordered
	else
		nNumIDs = GetColumnTaskIDs(aTaskIDs);

	if (!nNumIDs)
	{
		ASSERT(0);
		return FALSE;
	}

	TDC_ATTRIBUTE nAttribID = TDC::MapColumnToAttribute(nColID);

	TDCGETTASKS filter;
	filter.mapAttribs.Add(nAttribID);

	CTaskFile tasks;
	PrepareTaskfileForTasks(tasks, filter);

	CStringArray aValues;
	aValues.SetSize(nNumIDs);

	for (int nID = 0; nID < nNumIDs; nID++)
	{
		DWORD dwTaskID = aTaskIDs[nID], dwParentID = m_data.GetTaskParentID(dwTaskID);
		HTASKITEM hTask = tasks.NewTask(_T(""), NULL, dwTaskID, dwParentID);

		m_exporter.ExportMatchingTaskAttributes(dwTaskID, tasks, hTask, filter);
		aValues[nID] = tasks.GetTaskAttribute(hTask, nAttribID, true, true);
	}

	return CTaskClipboard::SetTasks(tasks, 
									GetClipboardID(), 
									aTaskIDs,
									Misc::FormatArray(aValues, '\n', TRUE),
									nColID);
}

BOOL CToDoCtrl::CanPasteAttributeColumnValues(TDC_COLUMN nToColID, BOOL bSelectedTasksOnly, TDC_COLUMN& nFromColID, int& nNumFrom) const
{
	if (IsReadOnly())
		return FALSE;

	// Check we have something to copy
	CWaitCursor cursor;
	CTaskFile tasks;

	nFromColID = CTaskClipboard::GetColumnTasks(tasks);
	nNumFrom = tasks.GetTaskCount();

	if ((nFromColID == TDCC_NONE) || (nNumFrom == 0))
		return FALSE;

	// Check column compatibility
	BOOL bSameTasklist = CTaskClipboard::TasklistIDMatches(GetClipboardID());

	if (!m_attribCopier.CanCopyColumnValues(nFromColID, nToColID, bSameTasklist))
		return FALSE;

	// For 'Selected' check that every task is editable
	TDC_ATTRIBUTE nToAttribID = TDC::MapColumnToAttribute(nToColID);

	if (bSelectedTasksOnly)
		return CanEditSelectedTask(nToAttribID);

	// Check we have at least one editable task
	CDWordArray aToTaskIDs;

	if (nNumFrom == 1)
		GetColumnTaskIDs(aToTaskIDs); // All
	else
		GetColumnTaskIDs(aToTaskIDs, 0, (nNumFrom - 1));

	int nID = aToTaskIDs.GetSize();

	while (nID--)
	{
		if (CanEditTask(aToTaskIDs[nID], nToAttribID))
			return TRUE;
	}
	
	// else
	nNumFrom = 0;
	nFromColID = TDCC_NONE;

	return FALSE;
}

int CToDoCtrl::GetColumnTaskIDs(CDWordArray& aTaskIDs, int nFrom, int nTo) const
{ 
	return m_taskTree.GetColumnTaskIDs(aTaskIDs, nFrom, nTo); 
}

BOOL CToDoCtrl::PasteAttributeColumnValues(TDC_COLUMN nToColID, BOOL bSelectedTasksOnly)
{
	CWaitCursor cursor;

	CTaskFile tasks;
	TDC_COLUMN nFromColID = CTaskClipboard::GetColumnTasks(tasks);

	if (nFromColID == TDCC_NONE)
	{
		ASSERT(0); // should have been caught before this
		return FALSE;
	}

	IMPLEMENT_DATA_UNDO_EDIT(m_data);

	// If only a single task was copied then we paste that task's value
	// to ALL target tasks else we copy each task's value to just one
	// target task
	int nNumFrom = tasks.GetTaskCount();
	CDWordArray aToTaskIDs;

	if (bSelectedTasksOnly)
	{
		GetSelectedTaskIDs(aToTaskIDs, FALSE, TRUE); // ordered
	}
	else if (nNumFrom == 1)
	{
		// Get all target task IDs
		GetColumnTaskIDs(aToTaskIDs);
	}
	else
	{
		// Get only as many target task IDs as there are copied task IDs
		GetColumnTaskIDs(aToTaskIDs, 0, (nNumFrom - 1));
	}

	// Do the merge
	CDWordArray aModTaskIDs;
	TODOITEM tdiFrom, tdiTo;

	int nToNumIDs = aToTaskIDs.GetSize();
	HTASKITEM hTask = tasks.GetFirstTask();

	if (nNumFrom == 1)
	{
		// Paste calculated attributes
		if (tasks.GetTaskAttributes(hTask, tdiFrom, TRUE))
		{
			for (int nID = 0; nID < nToNumIDs; nID++)
			{
				DWORD dwToTaskID = aToTaskIDs[nID];

				if (CopyColumnValue(tdiFrom, nFromColID, dwToTaskID, nToColID))
				{
					aModTaskIDs.Add(dwToTaskID);
				}
			}
		}
	}
	else
	{
		int nID = 0;

		while (hTask && (nID < nToNumIDs))
		{
			DWORD dwToTaskID = aToTaskIDs[nID];

			// Paste calculated attributes
			if (tasks.GetTaskAttributes(hTask, tdiFrom, TRUE) && CopyColumnValue(tdiFrom, nFromColID, dwToTaskID, nToColID))
			{
				aModTaskIDs.Add(dwToTaskID);
			}

			hTask = tasks.GetNextTask(hTask);
			nID++;
		}
	}

	if (aModTaskIDs.GetSize())
		SetModified(TDC::MapColumnToAttribute(nToColID), aModTaskIDs, TRUE);

	return TRUE;
}

BOOL CToDoCtrl::CopyColumnValue(const TODOITEM& tdiFrom, TDC_COLUMN nFromColID, DWORD dwToTaskID, TDC_COLUMN nToColID)
{
	TODOITEM tdiTo;

	if (!m_data.GetTaskAttributes(dwToTaskID, tdiTo))
		return FALSE;

	if (!m_attribCopier.CopyColumnValue(tdiFrom, nFromColID, tdiTo, nToColID))
		return FALSE;

	return (SET_CHANGE == m_data.SetTaskAttributes(dwToTaskID, tdiTo));
}

BOOL CToDoCtrl::CopySelectedTasks() const
{
	if (!GetSelectedTaskCount())
		return FALSE;
	
	ClearCopiedItem();

	// copy selected tasks to clipboard
	CTaskFile tasks;
	TDCGETTASKS filter;

	if (!GetSelectedTasks(tasks, filter))
		return FALSE;
	
	// and their titles
	CHTIList selection;
	CStringArray aTitles;

	VERIFY(TSH().CopySelection(selection, FALSE, TRUE));
	VERIFY(TSH().GetItemTitles(selection, aTitles));

	CDWordArray aSelTaskIDs;
	GetSelectedTaskIDs(aSelTaskIDs, FALSE);

	return CTaskClipboard::SetTasks(tasks, GetClipboardID(), aSelTaskIDs, Misc::FormatArray(aTitles, '\n'));
}

BOOL CToDoCtrl::CopySelectedTask() const
{
	return CopySelectedTasks();
}

BOOL CToDoCtrl::CutSelectedTask()
{
	if (CanEditSelectedTask(TDCA_DELETE) && CopySelectedTasks())
	{
		IMPLEMENT_DATA_UNDO(m_data, TDCUAT_DELETE);
			
		DeleteSelectedTask(FALSE, TRUE);
		return TRUE;
	}
	
	return FALSE;
}

BOOL CToDoCtrl::CanPasteTasks(TDC_PASTE nWhere, BOOL bAsRef) const
{
	if (!CanEditSelectedTask(TDCA_PASTE) || IsClipboardEmpty())
		return FALSE;

	// cannot paste references from other tasklists
	if (bAsRef && IsClipboardEmpty(TRUE))
		return FALSE;

	switch (nWhere)
	{
	case TDCP_ONSELTASK:
		// there must be exactly one task selected
		if (GetSelectedTaskCount() != 1)
		{
			return FALSE;
		}
		// can only paste non-references on other non-references
		else if (!bAsRef && m_data.IsTaskReference(m_taskTree.GetSelectedTaskID()))
		{
			return FALSE;
		}
		break;
		
	case TDCP_BELOWSELTASK:
		if (!GetSelectedTaskCount())
		{
			return FALSE;
		}
		else
		{
			// parent must be acceptable
			DWORD dwParentID = GetSelectedTaskParentID();

			if (GetSelectedTaskCount() == 1)
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

	// Check we've got valid tasks to copy from
	CString sClipID = GetClipboardID();
	CTaskFile tasks;
	CDWordArray aSelTaskIDs;

	if (!CTaskClipboard::GetTasks(sClipID, tasks, aSelTaskIDs))
		return FALSE;

	// Figure out where to paste to
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
		PrepareTasksForPasteAsRef(tasks, aSelTaskIDs);
	}
	else
	{
		// pre-process task IDs if the tasks did *not* originate 
		// from us (or our archive) and we're not empty
		CString sArchiveID;
		BOOL bCheckArchive = GetClipboardID(sArchiveID, TRUE);

		TDC_RESETIDS nResetID = TDCR_YES;

		if (CTaskClipboard::TasklistIDMatches(sClipID))
		{
			nResetID = TDCR_CHECK;
		}
		else if (bCheckArchive && CTaskClipboard::TasklistIDMatches(sArchiveID))
		{
			nResetID = TDCR_CHECK;
		}
		else if (GetTaskCount() == 0)
		{
			nResetID = TDCR_NO;
		}
		
		// and fix up the dependencies of the tasks
		// and the creation date
		PrepareTasksForPaste(tasks, nResetID, TRUE);
	}
		
	// Merge in any custom attributes we don't already have
	CTDCCustomAttribDefinitionArray aOrgAttribDefs, aPasteAttribDefs;
	aOrgAttribDefs.Copy(m_aCustomAttribDefs);
	
	BOOL bCustomAttribDefsChanged = (tasks.GetCustomAttributeDefs(aPasteAttribDefs) &&
									m_aCustomAttribDefs.Append(aPasteAttribDefs));
	DWORD dwDestTaskID = GetTaskID(htiDest);
	
	IMPLEMENT_DATA_UNDO(m_data, TDCUAT_PASTE);
	{
		HOLD_REDRAW(*this, m_taskTree);
			
		// no need to re-check IDs as we've already done it
		if (!PasteTasksToTree(tasks, htiDest, htiDestAfter, TDCR_NO, TRUE))
		{
			// Revert custom attribute changes
			if (bCustomAttribDefsChanged)
				m_aCustomAttribDefs.Copy(aOrgAttribDefs);
			
			return FALSE;
		}
	}

	OnCustomAttributesChanged();
	FixupParentCompletion(dwDestTaskID);
	
	return TRUE;
}

BOOL CToDoCtrl::RemoveNonSelectedTasks(const CDWordSet& mapSelTaskIDs, CTaskFile& tasks, HTASKITEM hTask)
{
	if (hTask)
	{
		// siblings first ie. before we might delete it
		RemoveNonSelectedTasks(mapSelTaskIDs, tasks, tasks.GetNextTask(hTask)); // RECURSIVE CALL

		if (!mapSelTaskIDs.Has(tasks.GetTaskID(hTask)))
		{
			tasks.DeleteTask(hTask); // will delete children
			return TRUE;
		}

		// check children
		RemoveNonSelectedTasks(mapSelTaskIDs, tasks, tasks.GetFirstTask(hTask)); // RECURSIVE CALL
	}

	return FALSE;
}

BOOL CToDoCtrl::PasteTasksToTree(const CTaskFile& tasks, HTREEITEM htiDestParent, HTREEITEM htiDestAfter,
								 TDC_RESETIDS nResetIDs, BOOL bSelectAll)
{
	if (!htiDestParent)
		htiDestParent = TVI_ROOT;
	
	HTASKITEM hTask = tasks.GetFirstTask();
	
	if (!hTask)
		return FALSE;
	
	// cache and clear current selection
	DWORD dwSelID = GetTaskID(TSH().GetFirstItem());

	TCH().SelectItem(NULL);
	TSH().RemoveAll();

	while (hTask)
	{
		htiDestAfter = PasteTaskToTree(tasks, hTask, htiDestParent, htiDestAfter, nResetIDs, TRUE);

		// next task
		hTask = tasks.GetNextTask(hTask);
	}

	m_taskTree.ExpandItem(htiDestParent);
	
	// restore selection
	CDWordArray aSelTaskIDs;

	if (bSelectAll && tasks.GetTaskIDs(aSelTaskIDs))
	{
		m_taskTree.SelectTasks(aSelTaskIDs);
	}
	else if (dwSelID)
	{
		SelectTask(dwSelID, FALSE);
	}
	else // select first task
	{
		SelectItem(m_taskTree.GetChildItem());
	}
	
	SetModified(TDCA_PASTE);
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

		TDC_ATTRIBUTE nAttribID = TDC::MapColumnToAttribute((TDC_COLUMN)pNMIA->iSubItem);
		m_ctrlAttributes.SelectValue(nAttribID);
	}
}

LRESULT CToDoCtrl::OnTDCNotifyColumnEditClick(WPARAM wParam, LPARAM lParam)
{
	TDC_COLUMN nColID = (TDC_COLUMN)wParam;
	ASSERT(nColID != TDCC_NONE);

	DWORD dwTaskID = lParam;
	ASSERT(dwTaskID);

	// Note: We only assert that the entry conditions are met
	// because this should all have been dealt with by the caller
	BOOL bSelTask = m_taskTree.IsTaskSelected(dwTaskID);

	switch (nColID)
	{
	case TDCC_CLIENT:
		ASSERT(bSelTask && CanEditTask(dwTaskID, TDCA_TASKNAME));
		EditSelectedTaskTitle();
		break;
		
	case TDCC_DONE:
		ASSERT(bSelTask && CanEditTask(dwTaskID, TDCA_DONEDATE));
		SetSelectedTaskCompletion(m_data.IsTaskDone(dwTaskID) ? TDCTC_UNDONE : TDCTC_DONE);
		break;
		
	case TDCC_TRACKTIME:
		ASSERT(bSelTask && CanEditTask(dwTaskID, TDCA_TIMESPENT));
		{
			HTREEITEM hti = m_taskTree.GetTreeSelectedItem();

			ASSERT ((GetSelectedTaskCount() == 1) && 
					IsItemSelected(hti) && 
					m_timeTracking.CanTrackTask(dwTaskID));
			
			ToggleTimeTracking(hti);
		}
		break;
		
	case TDCC_FLAG:
		ASSERT(bSelTask && CanEditTask(dwTaskID, TDCA_FLAG));
		SetSelectedTaskFlag(!m_data.IsTaskFlagged(dwTaskID));
		break;
		
	case TDCC_LOCK:
		ASSERT(bSelTask && CanEditTask(dwTaskID, TDCA_LOCK));
		SetSelectedTaskLock(!m_data.IsTaskLocked(dwTaskID));
		break;

	case TDCC_ICON:
		ASSERT(bSelTask && CanEditTask(dwTaskID, TDCA_ICON));

		// Cancel any drag started by clicking on the tree item icon
		m_treeDragDrop.CancelDrag();

		EditSelectedTaskIcon();
		break;
		
	case TDCC_FILELINK:
		GotoFile(m_data.GetTaskFileLink(dwTaskID, 0));
		break;
		
	case TDCC_DEPENDENCY:
		GotoSelectedTaskDependency();
		break;
		
	case TDCC_RECURRENCE:
		EditSelectedTaskRecurrence();
		break;
		
	case TDCC_REMINDER:
		GetParent()->SendMessage(WM_TDCM_EDITTASKREMINDER);
		break;
		
	default: // try custom columns
		HandleCustomColumnClick(nColID);
		break;
	}

	return 0L;
}

BOOL CToDoCtrl::GetSelectedTaskTimePeriod(TDC_ATTRIBUTE nAttribID, TDCTIMEPERIOD& time) const 
{ 
	switch (nAttribID)
	{
	case TDCA_TIMESPENT:
		return m_ctrlAttributes.GetTimeSpent(time);

	case TDCA_TIMEESTIMATE:
		return m_ctrlAttributes.GetTimeEstimate(time);
	}

	ASSERT(0);
	return FALSE;
}

LRESULT CToDoCtrl::OnTDCNotifyTaskAttributeEdited(WPARAM wParam, LPARAM lParam)
{
	TDC_ATTRIBUTE nAttribID = (TDC_ATTRIBUTE)wParam;

	// Special handling
	switch (nAttribID)
	{
	case TDCA_TIMEESTIMATE:
	case TDCA_TIMESPENT:
		if (lParam && (GetSelectedTaskCount() == 1)) // Time units change only
		{
			TDCTIMEPERIOD time;

			// see if the current time is non-zero and if so we prompt
			if (GetSelectedTaskTimePeriod(nAttribID, time) && (time.dAmount > 0.0))
			{
				switch (CMessageBox::AfxShow(IDS_TDC_RECALCTITLE, IDS_TDC_RECALCPROMPT, MB_ICONQUESTION | MB_YESNOCANCEL))
				{
				case IDCANCEL:
					return 0L;

				case IDYES:
					return UpdateTask(nAttribID, UTF_RECALCTIME);

				case IDNO:
					// Default handling
					break;
				}
			}
		}
		break;

	default:
		break;
	}

	// Default handling
	return UpdateTask(nAttribID);
}

LRESULT CToDoCtrl::OnTDCEditTaskReminder(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return GetParent()->SendMessage(WM_TDCM_EDITTASKREMINDER);
}

LRESULT CToDoCtrl::OnTDCClearTaskReminder(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return GetParent()->SendMessage(WM_TDCM_CLEARTASKREMINDER);
}

LRESULT CToDoCtrl::OnTDCEditTaskAttribute(WPARAM wParam, LPARAM lParam)
{
	TDC_ATTRIBUTE nAttribID = (TDC_ATTRIBUTE)wParam;

	switch (nAttribID)
	{
	case TDCA_COLOR:
		return EditSelectedTaskColor();

	case TDCA_DEPENDENCY:
		return EditSelectedTaskDependency();

	case TDCA_ICON:
		return EditSelectedTaskIcon();

	case TDCA_RECURRENCE:
		return EditSelectedTaskRecurrence();

	case TDCA_REMINDER:
		// Use WM_TDCM_EDITTASKREMINDER instead
		break;

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		{
			const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
			GET_CUSTDEF_RET(m_aCustomAttribDefs, nAttribID, pDef, 0L);

			switch (pDef->GetDataType())
			{
			case TDCCA_ICON:
				if (!pDef->IsList())
					return HandleCustomColumnClick(pDef->GetColumnID());
				break;
			}
		}
		break;
	}

	// All else
	ASSERT(0);
	return 0L;
}

LRESULT CToDoCtrl::OnTDCClearTaskAttribute(WPARAM wParam, LPARAM lParam)
{
	return ClearSelectedTaskAttribute((TDC_ATTRIBUTE)wParam);
}

int CToDoCtrl::GetAllSelectedTaskDependencies(CDWordArray& aLocalDepends, CStringArray& aOtherDepends) const
{
	CStringSet aOther;
	CDWordSet aLocal;
	CDWordArray aTaskIDs;

	int nID = GetSelectedTaskIDs(aTaskIDs, TRUE);
	CDWordArray aTaskLocal;
	CStringArray aTaskOther;

	while (nID--)
	{
		if (m_data.GetTaskDependencies(aTaskIDs[nID], aTaskLocal, aTaskOther))
		{
			aLocal.Add(aTaskLocal);
			aOther.Add(aTaskOther);
		}
	}

	aOther.CopyTo(aOtherDepends);
	aLocal.CopyTo(aLocalDepends);

	return (aLocalDepends.GetSize() + aOtherDepends.GetSize());
}

BOOL CToDoCtrl::HandleCustomColumnClick(TDC_COLUMN nColID)
{
	if (!TDCCUSTOMATTRIBUTEDEFINITION::IsCustomColumn(nColID))
		return FALSE;

	const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
	GET_CUSTDEF_RET(m_aCustomAttribDefs, nColID, pDef, FALSE);
		
	TDCCADATA data;
	GetSelectedTaskCustomAttributeData(pDef->sUniqueID, data);

	BOOL bHandled = FALSE;
	
	switch (pDef->GetDataType())
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

			if (pDef->aDefaultListData.GetSize())
				nChar = pDef->aDefaultListData[0][0];
		
			data.Set(true, nChar);
		}
		bHandled = SetSelectedTaskCustomAttributeData(pDef->sUniqueID, data.AsString());
		break;
		
	case TDCCA_ICON:
		switch (pDef->GetListType())
		{
		case TDCCA_FIXEDLIST:
			{
				CString sImage, sDummy;
				BOOL bNext = (!Misc::IsKeyPressed(VK_SHIFT));
				CString sTag = pDef->GetNextListItem(data.AsString(), bNext);
				
				if (sTag.IsEmpty() || pDef->DecodeImageTag(sTag, sImage, sDummy))
				{
					bHandled = SetSelectedTaskCustomAttributeData(pDef->sUniqueID, sImage);
				}
			}
			break;

		case TDCCA_FIXEDMULTILIST:
			// do nothing
			break;

		case TDCCA_NOTALIST:
			{
				CTDLTaskIconDlg dialog(m_ilTaskIcons, data.AsString(), TRUE, -1, this);
				
				if (dialog.DoModal(IDS_CUSTATTRIBICONDLG_TITLE) == IDOK)
				{
					bHandled = SetSelectedTaskCustomAttributeData(pDef->sUniqueID, dialog.GetIconName());
				}
			}
			break;
		}
		break;
		
	default:
		// do item cycling for fixed lists unless they support calculation
		if (pDef->GetListType() == TDCCA_FIXEDLIST)
		{
			BOOL bNext = (!Misc::IsKeyPressed(VK_SHIFT));
			CString sItem = pDef->GetNextListItem(data.AsString(), bNext);
			
			bHandled = SetSelectedTaskCustomAttributeData(pDef->sUniqueID, sItem);
		}
		break;
	}

	if (bHandled)
		m_ctrlAttributes.RefreshSelectedTasksValue(pDef->GetAttributeID());

	return bHandled;
}

void CToDoCtrl::ToggleTimeTracking(HTREEITEM hti)
{
	ASSERT (GetSelectedTaskCount() == 1); // sanity check
	
	DWORD dwTaskID = GetTrueTaskID(hti);

	if (m_timeTracking.IsTrackingTask(dwTaskID, FALSE))
		EndTimeTracking(TRUE, TRUE); 
	else
		BeginTimeTracking(dwTaskID, TRUE);
}

// External
BOOL CToDoCtrl::BeginTimeTracking(DWORD dwTaskID)
{
	return BeginTimeTracking(dwTaskID, FALSE); // don't notify
}

// Internal
BOOL CToDoCtrl::BeginTimeTracking(DWORD dwTaskID, BOOL bNotify)
{
	if (!m_timeTracking.CanTrackTask(dwTaskID))
		return FALSE;

	// Verify that we have been saved
	if (!HasFilePath())
	{
		CMessageBox::AfxShow(CEnString(IDS_TITLE_TIMETRACKING), 
							 CEnString(IDS_MESSAGE_SAVETASKLISTTOENABLEFEATURE), 
							 (MB_OK | MB_ICONEXCLAMATION));
		return FALSE;
	}

	// if there's a current task being tracked then end it
	EndTimeTracking(TRUE, bNotify);

	// Select the task if this is a requirement
	BOOL bTaskIsSelected = m_taskTree.IsTaskSelected(dwTaskID, TRUE);

	if (!bTaskIsSelected && HasStyle(TDCS_TRACKSELECTEDTASKONLY))
	{
		bTaskIsSelected = TRUE;
		VERIFY(SelectTask(dwTaskID, FALSE));
	}

	VERIFY(m_timeTracking.BeginTracking(dwTaskID));

	// if the task's start date has not been set then set it now
	if (!m_data.TaskHasDate(dwTaskID, TDCD_START))
		m_data.SetTaskDate(dwTaskID, TDCD_STARTDATE, COleDateTime::GetCurrentTime());

	m_taskTree.SetTimeTrackTaskID(dwTaskID);
	m_ctrlAttributes.SetTimeTrackTaskID(dwTaskID);

	SetTimer(TIMER_TRACK, TIMETRACKPERIOD, NULL);

	// notify parent
	if (bNotify)
		GetParent()->SendMessage(WM_TDCN_TIMETRACK, (WPARAM)GetSafeHwnd(), dwTaskID);

	return TRUE;
}

void CToDoCtrl::SetTimeTrackingReminderInterval(int nMinutes)
{
	m_timeTracking.SetTrackingReminderInterval(nMinutes);
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
	DWORD dwTaskID = m_timeTracking.GetTrackedTaskID(FALSE);

	if (!dwTaskID)
		return;

	KillTimer(TIMER_TRACK);

	// add whatever time is still waiting
	IncrementTrackedTime(TRUE);

	VERIFY(m_timeTracking.EndTracking());

	// prevent re-entrancy
	double dTime = m_dTrackedTimeElapsedHours;
	m_dTrackedTimeElapsedHours = 0.0;

	// log it
	if (HasStyle(TDCS_LOGTIMETRACKING))
	{
		BOOL bShowDialog = (bAllowConfirm && HasStyle(TDCS_DISPLAYLOGCONFIRM));

		DoAddTimeToLogFile(dwTaskID, dTime, bShowDialog);
	}

	// update tree
	m_taskTree.SetTimeTrackTaskID(0);
	m_ctrlAttributes.SetTimeTrackTaskID(0);

	// notify parent
	if (bNotify)
		GetParent()->SendMessage(WM_TDCN_TIMETRACK, (WPARAM)GetSafeHwnd(), 0);
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
	double dOrgHours = dHours;

	if (bShowDialog)
	{
		// if we are readonly, we need to prevent
		// the dialog showing 'Add time to time spent'
		BOOL bShowAddToTimeSpent = (!bTracked && CanEditSelectedTask(TDCA_TIMESPENT));

		CTDLAddLoggedTimeDlg dialog(dwTaskID, bShowAddToTimeSpent, HasStyle(TDCS_SHOWDATESINISO), dHours, this);

		if (dialog.DoModal(s_hIconAddLogDlg) != IDOK)
			return FALSE;

		// else
		dHours = dialog.GetLoggedHours();
		dtWhen = dialog.GetWhen();
		sComment = dialog.GetComment();

		if (!bTracked)
			bAddToTimeSpent = dialog.GetAddToTimeSpent();
	}

	if (!AddTimeToTaskLogFile(dwTaskID, dHours, dtWhen, sComment, bTracked))
		return FALSE;

	// If the user changed the tracked hours then manually adjust the task's
	// time because it will have already been updated during time-tracking
	if (bTracked && (dHours != dOrgHours))
	{
		return AdjustTaskTimeSpent(dwTaskID, (dHours - dOrgHours));
	}
	else if (bAddToTimeSpent)
	{
		return AdjustTaskTimeSpent(dwTaskID, dHours);
	}

	// else
	return TRUE;
}

LRESULT CToDoCtrl::OnApplyAddLoggedTime(WPARAM wParam, LPARAM lParam)
{
	ASSERT(wParam && lParam);

	if (!wParam || !lParam)
		return 0L;

	const CTDLAddLoggedTimeDlg* pDialog = (const CTDLAddLoggedTimeDlg*)lParam;
	DWORD dwTaskID = wParam;

	if (AddTimeToTaskLogFile(dwTaskID, 
							pDialog->GetLoggedHours(), 
							pDialog->GetWhen(), 
							pDialog->GetComment(), 
							pDialog->IsTracked()))
	{
		if (pDialog->GetAddToTimeSpent())
			return AdjustTaskTimeSpent(dwTaskID, pDialog->GetLoggedHours());
	}

	// else
	return 0L;
}

BOOL CToDoCtrl::AddTimeToTaskLogFile(DWORD dwTaskID, double dHours, const COleDateTime& dtWhen, 
									const CString& sComment, BOOL bTracked)
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
		AfxMessageBox(Misc::Format(_T("%s\n\n%s"), CEnString(IDS_LOGFILELOCKED), CEnString(IDS_LOGFILELOCKED_EXTRA)));

		return FALSE;
	}

	// else
	return TRUE;
}

BOOL CToDoCtrl::AdjustTaskTimeSpent(DWORD dwTaskID, double dHours)
{
	ASSERT(dwTaskID && (dHours != 0.0));

 	TDCTIMEPERIOD time(dHours, TDCU_HOURS);

	if ((GetSelectedTaskCount() == 1) && (GetSelectedTaskID() == dwTaskID))
		return SetSelectedTaskTimeSpent(time, TRUE); // offset

	// else
	CDWordArray aModTaskIDs;
	aModTaskIDs.Add(dwTaskID);

	m_data.SetTaskTimeSpent(dwTaskID, time, TRUE); // offset
	SetModified(TDCA_TIMESPENT, aModTaskIDs);

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

void CToDoCtrl::NotifyParentSelectionChange() const
{
	GetParent()->PostMessage(WM_TDCN_SELECTIONCHANGE);
}

void CToDoCtrl::OnTreeSelChange(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	*pResult = 0;

	UpdateControls(); 
	NotifyParentSelectionChange();
	
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

BOOL CToDoCtrl::SelectTask(DWORD dwTaskID, BOOL /*bTaskLink*/)
{ 
	HTREEITEM hti = m_taskTree.GetItem(dwTaskID);

	if (hti)
		SelectItem(hti);

	return (hti != NULL);
}

BOOL CToDoCtrl::SelectTasks(const CDWordArray& aTaskIDs)
{
	// sanity check
	ASSERT(aTaskIDs.GetSize());

	if (!aTaskIDs.GetSize())
		return FALSE;

 	return m_taskTree.SelectTasks(aTaskIDs);
}

void CToDoCtrl::SelectItem(HTREEITEM hti) 
{ 
	// PERMANENT LOGGING //////////////////////////////////////////////
	CScopedLogTimer log(_T("CToDoCtrl::SelectItem()"));
	///////////////////////////////////////////////////////////////////

	if (m_taskTree.GetSafeHwnd()) 
	{
		// Do a flush if the item is changing
		if (!m_taskTree.IsItemSelected(hti, TRUE))
			Flush();

		if (!m_taskTree.SelectItem(hti))
			UpdateControls(); // disable controls

		UpdateSelectedTaskPath();
		NotifyParentSelectionChange();
	}
}

void CToDoCtrl::SelectAll(BOOL bVisibleOnly)
{ 
	if (!HandleUnsavedComments())
		return;

	// Note: No need to call UpdateControls because that will happen
	//       as a consequence of a selection change notification
	m_taskTree.SelectAll(bVisibleOnly);
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
	tasks.SetAvailableAttributes(filter.mapAttribs);

	// ISO date strings
	// must be done first before any tasks are added
	tasks.EnableISODates(HasStyle(TDCS_SHOWDATESINISO));
	
	// custom attributes
	SaveCustomAttributeDefinitions(tasks, filter);
	
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
	tasks.SetEarliestTaskDueDate(m_calculator.GetEarliestDueDate());
	tasks.SetLastModified(m_dtLastTaskMod);
	tasks.SetFilePath(m_sLastSavePath);

	tasks.SetNextUniqueID(m_dwNextUniqueID);
	ASSERT (tasks.GetNextUniqueID() == m_dwNextUniqueID);
	
	if (!HasStyle(TDCS_USES3RDPARTYSOURCECONTROL))
	{
		tasks.SetFileVersion(m_nFileVersion);
	}
}

int CToDoCtrl::GetAllTasks(CTaskFile& tasks) const
{
	return m_exporter.ExportAllTasks(tasks);
}

BOOL CToDoCtrl::HandleUnsavedComments()
{
	if (m_nCommentsState == CS_PENDING)
	{
		CString sTextComments;
		CBinaryData customComments;
		
		if (m_ctrlComments.GetContent(sTextComments, customComments) == -1)
		{
			// Notify user
			AfxMessageBox(IDS_COMMENTSMEMORYERROR, MB_ICONERROR | MB_OK);
			return FALSE;
		}

		// else
		SetSelectedTaskComments(sTextComments, customComments, TRUE); // TRUE == internal call

		m_nCommentsState = CS_CHANGED;

		// Update sort if required
		if ((m_visColEdit.IsColumnVisible(TDCC_COMMENTSSIZE) && IsSortingBy(TDCC_COMMENTSSIZE)) ||
			(m_visColEdit.IsColumnVisible(TDCC_COMMENTSFORMAT) && IsSortingBy(TDCC_COMMENTSFORMAT)))
		{
			Resort();
		}
	}

	return TRUE;
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
	// cache current selection and task breadcrumbs before clearing selection
	TDCSELECTIONCACHE cache;
	CacheTreeSelection(cache);
		
	CHoldRedraw hr(*this);
	CWaitCursor cursor;
	CDWordArray aExpanded;
	
	// Allow task tree to prepare for a rebuild
	m_taskTree.OnBeginRebuild();

	if (m_taskTree.GetItemCount())
	{
		m_taskTree.GetExpandedTasks(aExpanded);
		
		TSH().RemoveAll();
		TCH().SelectItem(NULL);
		m_taskTree.DeleteAll();
	}

	HTREEITEM hti = NULL;
	
	if (BuildTreeItem(NULL, m_data.GetStructure(), pContext))
	{
		m_taskTree.SetLargestTaskID(m_dwNextUniqueID);

		hti = m_taskTree.GetChildItem();
	}
	
	// Notify tree that the rebuild is over
	m_taskTree.OnEndRebuild();

	// Then restore previous state
	m_taskTree.ExpandTasks(aExpanded);

	if (!RestoreTreeSelection(cache))
	{
		HTREEITEM hti = m_taskTree.GetChildItem(NULL); // select first item
		
		if (hti)
			SelectItem(hti);
		else
			UpdateControls();
	}

	return hti;
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
		ASSERT(pTDS);
	}
#endif

	if (!pTDS)
		return FALSE;

	// rebuild
	int nSubtask = pTDS->GetSubTaskCount();

	// reverse-order tree-insertion is quicker according to Raymond Chen
	while (nSubtask--)
	{
		const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubtask);
		DWORD dwTaskID = pTDSChild->GetTaskID();

		const TODOITEM* pTDIChild = m_data.GetTask(dwTaskID);
		ASSERT(pTDIChild);

		// is this task wanted?
		BOOL bHasChildren = pTDSChild->HasSubTasks();
		BOOL bAddTask = TRUE;

		if (!bHasChildren && pContext)
			bAddTask = WantAddTaskToTree(pTDIChild, pTDSChild, pContext);

		if (bAddTask)
		{
			// add this item to tree
			HTREEITEM htiChild = InsertTreeItem(pTDIChild, dwTaskID, htiParent, TVI_FIRST, FALSE);
			ASSERT(htiChild);

			// update next unique ID
			m_dwNextUniqueID = max(m_dwNextUniqueID, dwTaskID + 1);

			// and its children
			if (bHasChildren)
			{
				BuildTreeItem(htiChild, pTDSChild, pContext); // RECURSIVE call

				// if no subtasks got added to the tree then we may need to remove the parent too
				if (!m_taskTree.ItemHasChildren(htiChild))
				{
					BOOL bRemoveTask = (pContext != NULL && !WantAddTaskToTree(pTDIChild, pTDSChild, pContext));

					if (bRemoveTask)
						m_taskTree.DeleteItem(htiChild);
				}
			}
		}
	}

	return TRUE;
}

BOOL CToDoCtrl::WantAddTaskToTree(const TODOITEM* /*pTDI*/, const TODOSTRUCTURE* /*pTDS*/, const void* /*pContext*/) const
{
	return TRUE;
}

int CToDoCtrl::GetSelectedTasks(CTaskFile& tasks, const TDCGETTASKS& filter) const
{
	if (!HasSelection())
		return 0;
	
	PrepareTaskfileForTasks(tasks, filter);

	// Add selected tasks in an display-ordered list, 
	// removing duplicate subtasks if we will be adding them anyway
	BOOL bRemoveDupeSubtasks = !filter.HasFlag(TDCGSTF_NOTSUBTASKS);

	CHTIList selection;
	TSH().CopySelection(selection, bRemoveDupeSubtasks, TRUE);

	// Note: this call can fail if, for instance, the filter is asking
	// for incomplete tasks and the selected tasks have just been 
	// marked completed
	if (AddTasksToTaskFile(selection, filter, tasks)) // Mark tasks as selected
	{
		AddSelectedTaskReferencesToTaskFile(filter, tasks);
		AddSelectedTaskDependentsToTaskFile(filter, tasks);
	}
	
	return (tasks.GetTaskCount());
}

void CToDoCtrl::AddSelectedTaskReferencesToTaskFile(const TDCGETTASKS& filter, CTaskFile& tasks) const
{
	if (filter.HasFlag(TDCGSTF_APPENDREFERENCES) && m_taskTree.HasReferenceTasks())
	{
		CHTIList lstReferences;

		if (m_taskTree.GetReferencesToSelectedTask(lstReferences))
		{
			// We want just the references and nothing else
			TDCGETTASKS filterRefs(filter);
			filterRefs.dwFlags = TDCGSTF_NOTSUBTASKS;

			VERIFY(AddTasksToTaskFile(lstReferences, filterRefs, tasks));
		}
	}
}

void CToDoCtrl::AddSelectedTaskDependentsToTaskFile(const TDCGETTASKS& filter, CTaskFile& tasks) const
{
	if (filter.HasFlag(TDCGSTF_LOCALDEPENDENTS))
	{
		CHTIList lstDependents;

		if (m_taskTree.GetSelectedTaskLocalDependents(FALSE, lstDependents))
		{
			// We want just the dependents and nothing else
			TDCGETTASKS filterDeps(filter);
			filterDeps.dwFlags = TDCGSTF_NOTSUBTASKS;

			VERIFY(AddTasksToTaskFile(lstDependents, filterDeps, tasks));
		}
	}
}

int CToDoCtrl::AddTasksToTaskFile(const CHTIList& listHTI, const TDCGETTASKS& filter, CTaskFile& tasks) const
{
	BOOL bWantSubtasks = !filter.HasFlag(TDCGSTF_NOTSUBTASKS);
	BOOL bWantAllParents = filter.HasFlag(TDCGSTF_ALLPARENTS);
	BOOL bWantImmediateParent = filter.HasFlag(TDCGSTF_IMMEDIATEPARENT);
	BOOL bResolveReferences = filter.HasFlag(TDCGSTF_RESOLVEREFERENCES);

	POSITION pos = listHTI.GetHeadPosition();

	while (pos)
	{
		HTREEITEM hti = listHTI.GetNext(pos);
		DWORD dwTaskID = GetTaskID(hti);
		BOOL bHasParent = m_taskTree.ItemHasParent(hti);

		// do we need to resolve task references?
		if (bResolveReferences)
		{
			DWORD dwRefID = GetTrueTaskID(hti);

			if (dwRefID != dwTaskID)
			{
				dwTaskID = dwRefID;
				hti = m_taskTree.GetItem(dwRefID); // true task
			}
		}

		// does the user want this task's parent(s) ?
		if (bHasParent && (bWantAllParents || bWantImmediateParent))
		{
			if (AddTreeItemAndParentToTaskFile(hti, tasks, filter, bWantAllParents, bWantSubtasks))
				ASSERT(dwTaskID);
		}
		else
		{
			// find the parent task previously added (or not)
			DWORD dwParentID = 0;
			HTASKITEM hParent = NULL;

			if (bHasParent)
			{
				dwParentID = m_taskTree.GetTaskParentID(hti);
				hParent = tasks.FindTask(dwParentID);
			}

			if (AddTreeItemToTaskFile(hti, dwTaskID, tasks, hParent, filter, bWantSubtasks, dwParentID))
				ASSERT(dwTaskID);
		}
	}

	return tasks.GetTaskCount();
}

BOOL CToDoCtrl::AddTreeItemAndParentToTaskFile(HTREEITEM hti, CTaskFile& tasks, const TDCGETTASKS& filter,
											   BOOL bAllParents, BOOL bWantSubtasks) const
{
	// add parents first, recursively if necessarily
	HTREEITEM htiParent = m_taskTree.GetParentItem(hti);
	HTASKITEM hParent = NULL;
	DWORD dwParentID = 0;

	if (htiParent)
	{
		BOOL bParentAdded = FALSE;

		// Note: we never want parent's subtasks, so we pass FALSE
		if (bAllParents)
		{
			bParentAdded = AddTreeItemAndParentToTaskFile(htiParent, tasks, filter, TRUE, FALSE); // RECURSIVE CALL
		}
		else
		{
			DWORD dwGrandParentID = m_taskTree.GetTaskParentID(htiParent);
			HTASKITEM hGrandParent = tasks.FindTask(dwGrandParentID);

			bParentAdded = AddTreeItemToTaskFile(htiParent, 0, tasks, hGrandParent, filter, FALSE, dwGrandParentID);
		}

		// If we didn't add the parent then we won't be adding adding subtasks
		if (!bParentAdded)
			return FALSE;

		// else find the just added parent
		dwParentID = GetTaskID(htiParent);
		
		hParent = tasks.FindTask(dwParentID);
		ASSERT(hParent);
	}

	// now add item itself
	return AddTreeItemToTaskFile(hti, 0, tasks, hParent, filter, bWantSubtasks, dwParentID);
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
		BOOL bSelChange = !((GetSelectedTaskCount() == 1) && 
							(cache.aSelTaskIDs.GetSize() == 1) &&
							(GetSelectedTaskID() == cache.aSelTaskIDs[0]));
		if (bSelChange)
			UpdateControls();

		m_idleTasks.RefreshAttributeValues();
		return TRUE;
	}

	return FALSE;
}

int CToDoCtrl::GetSelectedTaskIDs(CDWordArray& aTaskIDs, BOOL bTrue, BOOL bOrdered) const
{
	return m_taskTree.GetSelectedTaskIDs(aTaskIDs, bTrue, bOrdered);
}

int CToDoCtrl::GetSelectedTaskIDs(CDWordArray& aTaskIDs, DWORD& dwFocusedTaskID, BOOL bRemoveChildDupes, BOOL bOrdered) const
{
	return m_taskTree.GetSelectedTaskIDs(aTaskIDs, dwFocusedTaskID, bRemoveChildDupes, bOrdered);
}

int CToDoCtrl::GetSubTaskIDs(DWORD dwTaskID, CDWordArray& aSubtaskIDs) const
{
	aSubtaskIDs.RemoveAll();

	const TODOSTRUCTURE* pTDS = NULL;
	GET_DATA_TDS(m_data, dwTaskID, pTDS, 0);

	for (int nPos = 0; nPos < pTDS->GetSubTaskCount(); nPos++)
		aSubtaskIDs.Add(pTDS->GetSubTaskID(nPos));

	return aSubtaskIDs.GetSize();
}

int CToDoCtrl::GetAllTaskIDs(CDWordArray& aTaskIDs, BOOL bIncParents, BOOL bIncCollapsedChildren) const
{
	if (bIncParents && !bIncCollapsedChildren)
		return m_taskTree.GetColumnTaskIDs(aTaskIDs);

	return TCH().GetItemData(aTaskIDs, bIncParents, bIncCollapsedChildren);
}

BOOL CToDoCtrl::PasteTaskAttributeValues(const CTaskFile& tasks, HTASKITEM hTask, const CTDCAttributeMap& mapAttribs, DWORD dwFlags)
{
	IMPLEMENT_DATA_UNDO_EDIT(m_data);

	CDWordArray aModTaskIDs;
	POSITION posSel = TSH().GetFirstItemPos();

	while (posSel)
	{
		DWORD dwTaskID = TSH().GetNextItemData(posSel);

		if (m_calculator.IsTaskLocked(dwTaskID))
			continue;

		// For each task, build a set of editable attributes only
		CTDCAttributeMap mapTaskAttribs;
		POSITION posAttrib = mapAttribs.GetStartPosition();

		while (posAttrib)
		{
			TDC_ATTRIBUTE nAttribID = mapAttribs.GetNext(posAttrib);

			if (CanEditTask(dwTaskID, nAttribID))
				mapTaskAttribs.Add(nAttribID);
		}

		if (!mapTaskAttribs.IsEmpty())
		{
			const TODOITEM* pTDI = GetTask(dwTaskID);

			if (pTDI)
			{
				TODOITEM tdiCopy = *pTDI;

				if (tasks.MergeTaskAttributes(hTask, tdiCopy, mapTaskAttribs, m_aCustomAttribDefs, dwFlags))
				{
					if (m_data.SetTaskAttributes(dwTaskID, tdiCopy) == SET_CHANGE)
						aModTaskIDs.Add(dwTaskID);
				}
			}
		}
	}

	if (aModTaskIDs.GetSize())
	{
		TDC_ATTRIBUTE nAttribID = ((mapAttribs.GetCount() == 1) ? mapAttribs.GetFirst() : TDCA_ALL);
		SetModified(nAttribID, aModTaskIDs);
	}

	return aModTaskIDs.GetSize();
}

BOOL CToDoCtrl::PasteTasks(const CTaskFile& tasks, TDC_INSERTWHERE nWhere, BOOL bSelectAll)
{
	if (!CanEditSelectedTask(TDCA_PASTE))
		return FALSE;

	HTREEITEM htiParent = NULL, htiAfter = NULL;

	if (!m_taskTree.GetInsertLocation(nWhere, htiParent, htiAfter))
		return FALSE;

	// merge in any new custom attribute definitions
	CTDCCustomAttribDefinitionArray aAttribDefs;
	
	if (tasks.GetCustomAttributeDefs(aAttribDefs))
	{
		if (m_aCustomAttribDefs.Append(aAttribDefs))
			OnCustomAttributesChanged();
	}

	// add the tasks
	IMPLEMENT_DATA_UNDO(m_data, TDCUAT_ADD);
	HOLD_REDRAW(*this, m_taskTree);

	// Fix up dependencies if not inserting into new tasklist
	if (GetTaskCount())
	{
		CTaskFile copy(tasks);

		// and always assign new IDs
		PrepareTasksForPaste(copy, TDCR_YES, FALSE);
		return PasteTasksToTree(copy, htiParent, htiAfter, TDCR_NO, bSelectAll);
	}

	// else
	return PasteTasksToTree(tasks, htiParent, htiAfter, TDCR_NO, bSelectAll);
}

BOOL CToDoCtrl::MergeTasks(const CTaskFile& tasks, BOOL bMergeByID)
{
	if (!GetTaskCount())
		return PasteTasks(tasks, TDC_INSERTATTOP, FALSE);

	HTASKITEM hTask = tasks.GetFirstTask();

	if (!hTask)
		return FALSE;

	// cache and clear current selection
	DWORD dwSelID = GetTaskID(TSH().GetFirstItem());

	TCH().SelectItem(NULL);
	TSH().RemoveAll();

	CDWordArray aTaskIDs;

	// add the tasks
	IMPLEMENT_DATA_UNDO(m_data, TDCUAT_ADD);
	HOLD_REDRAW(*this, m_taskTree);
	
	while (hTask)
	{
		VERIFY(MergeTaskWithTree(tasks, hTask, tasks.GetTaskParentID(hTask), bMergeByID, aTaskIDs));

		// next task
		hTask = tasks.GetNextTask(hTask);
	}

	// Merge in any additional custom attributes definitions
	CTDCCustomAttribDefinitionArray aImportedDefs;
				
	if (tasks.GetCustomAttributeDefs(aImportedDefs))
	{
		if (m_aCustomAttribDefs.Append(aImportedDefs))
			OnCustomAttributesChanged();
	}

	SetModified(TDCA_PASTE, aTaskIDs);
	return TRUE;
}

BOOL CToDoCtrl::MergeTaskWithTree(const CTaskFile& tasks, HTASKITEM hTask, DWORD dwParentTaskID, BOOL bMergeByID, CDWordArray& aNewTaskIDs)
{
	DWORD dwTaskID = 0;

	if (bMergeByID)
	{
		dwTaskID = tasks.GetTaskID(hTask);

		// If this task does not exist then treat it as a new task
		if (!HasTask(dwTaskID))
			dwTaskID = 0;
	}
	else
	{
		CDWordArray aTaskIDs;

		// Task name must be unique else treat it as a new task
		if (m_matcher.FindTasks(TDCA_TASKNAME, FOP_EQUALS, tasks.GetTaskTitle(hTask), FALSE, aTaskIDs) == 1)
			dwTaskID = aTaskIDs[0];
	}

	if (HasTask(dwTaskID))
	{
		TODOITEM tdi;
		VERIFY(m_data.GetTaskAttributes(dwTaskID, tdi));

		if (tasks.MergeTaskAttributes(hTask, tdi, TDLMTA_EXCLUDEEMPTYSOURCEVALUES))
			m_data.SetTaskAttributes(dwTaskID, tdi);
	}
	else 
	{
		dwTaskID = MergeNewTaskIntoTree(tasks, hTask, dwParentTaskID, FALSE); // not subtasks

		if (!dwTaskID)
			return FALSE;

		aNewTaskIDs.Add(dwTaskID);
	}
	ASSERT(dwTaskID);

	// Subtasks
	HTASKITEM hChildTask = tasks.GetFirstTask(hTask);

	while (hChildTask)
	{
		VERIFY(MergeTaskWithTree(tasks, hChildTask, dwTaskID, bMergeByID, aNewTaskIDs));
		hChildTask = tasks.GetNextTask(hChildTask); // next
	}

	return TRUE;
}

DWORD CToDoCtrl::MergeNewTaskIntoTree(const CTaskFile& tasks, HTASKITEM hTask, DWORD dwParentTaskID, BOOL bAndSubtasks)
{
	// Add task to existing parent or root
	HTREEITEM htiParent = NULL;
		
	if (dwParentTaskID)
		htiParent = m_taskTree.GetItem(dwParentTaskID);

	HTREEITEM htiNew = PasteTaskToTree(tasks, hTask, htiParent, TVI_FIRST, TDCR_YES, bAndSubtasks);

	if (!htiNew)
		return 0;

	// else
	return GetTaskID(htiNew);
}

int CToDoCtrl::AddTreeChildrenToTaskFile(HTREEITEM hti, CTaskFile& tasks, HTASKITEM hTask, const TDCGETTASKS& filter) const
{
	HTREEITEM htiChild = m_taskTree.GetChildItem(hti);
	int nChildren = 0;
	
	while (htiChild)
	{
		if (AddTreeItemToTaskFile(htiChild, 0, tasks, hTask, filter, TRUE)) // TRUE = want subtasks
			nChildren++;
		
		// next
		htiChild = m_taskTree.GetNextItem(htiChild);
	}
	
	return nChildren;
}

BOOL CToDoCtrl::AddTreeItemToTaskFile(HTREEITEM hti, DWORD dwTaskID, CTaskFile& tasks, HTASKITEM hParentTask, 
										const TDCGETTASKS& filter, BOOL bWantSubtasks, DWORD dwParentID) const
{
	// Sanity checks
	if (hti == NULL)
	{
		if (bWantSubtasks || (dwTaskID == 0))
		{
			ASSERT(0);
			return FALSE;
		}
	}
	else if (dwTaskID == 0)
	{	
		dwTaskID = GetTaskID(hti);
		ASSERT(dwTaskID);
	}
	else
	{
		ASSERT (GetTaskID(hti) == dwTaskID);
	}
	
	const TODOITEM* pTDI = m_data.GetTask(dwTaskID);
	ASSERT (pTDI);
	
	if (pTDI)
	{
		// Note: task file may already have this task if we've 
		// been auto-adding parents elsewhere
		HTASKITEM hTask = tasks.FindTask(dwTaskID);
		
		if (hTask == NULL)
		{
			hTask = tasks.NewTask(pTDI->sTitle, hParentTask, dwTaskID, dwParentID);
			ASSERT(hTask);
			
			if (!hTask)
				return FALSE;
		}

		// we return TRUE if we match the filter _or_ we have any matching children
		BOOL bMatch = FALSE;
		
		if (bWantSubtasks)
		{
			bMatch = AddTreeChildrenToTaskFile(hti, tasks, hTask, filter);
		}

		if (!bMatch) //  no children matched -> 'Check ourselves'
		{
			if (filter.HasFlag(TDCGTF_NOTLOCKED) && m_calculator.IsTaskLocked(dwTaskID))
			{
				// no match
			}
			else if (filter.nFilter == TDCGT_ALL)
			{
				bMatch = TRUE; // always
			}
			else
			{
				BOOL bDone = pTDI->IsDone();
				BOOL bGoodAsDone = (bDone ? TRUE : m_calculator.IsTaskDone(dwTaskID));

				switch (filter.nFilter)
				{
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
			}

			// then check 'allocated to' if set
			if (bMatch && !filter.sAllocTo.IsEmpty())
				bMatch = Misc::Contains(filter.sAllocTo, pTDI->aAllocTo, FALSE, TRUE);
		}
		
		if (bMatch)
		{
			const TODOSTRUCTURE* pTDS;
			GET_DATA_TDS(m_data, dwTaskID, pTDS, FALSE);

			m_exporter.ExportMatchingTaskAttributes(pTDI, pTDS, tasks, hTask, filter);
		}
		else
		{
			tasks.DeleteTask(hTask);
		}
		
		return bMatch;
	}
	
	return FALSE;
}

void CToDoCtrl::SetFocus(TDC_SETFOCUSTO nLocation)
{
	switch (nLocation)
	{
	case TDCSF_TASKVIEW:
		if (!m_taskTree.HasFocus())
		{
			if (!m_layout.IsVisible(TDCSF_TASKVIEW))
			{
				ASSERT(m_layout.GetMaximiseState() == TDCMS_MAXCOMMENTS);
				SetMaximizeState(TDCMS_MAXTASKLIST);
			}
			else
			{
				// NOTE: if the comments was the last window focused
				// before we were disabled, and we revert the focus
				// to the tree, then the comments gets very confused
				// and will not want to take the focus even though it
				// contains the caret, so we force it to have the focus
				// before switching to the tree.
				if (m_layout.IsVisible(TDCSF_COMMENTS))
					m_ctrlComments.SetFocus();

				m_taskTree.SetFocus();
			}

			// ensure the selected tree item is visible
			if (!m_taskTree.EnsureSelectionVisible(TRUE))
				SelectItem(m_taskTree.GetChildItem());
		}
		break;

	case TDCSF_COMMENTS:
		{
			if (!m_layout.IsVisible(TDCSF_COMMENTS))
			{
				ASSERT(m_layout.GetMaximiseState() == TDCMS_MAXTASKLIST);
				SetMaximizeState(TDCMS_MAXCOMMENTS);
			}
			else
			{
				m_ctrlComments.SetFocus();
			}
		}
		break;

	case TDCSF_ATTRIBUTES:
		{
			if (!m_layout.IsVisible(nLocation))
			{
				ASSERT(m_layout.GetMaximiseState() != TDCMS_NORMAL);
				SetMaximizeState(TDCMS_NORMAL);
			}

			m_ctrlAttributes.SetFocus();
		}
		break;

	case TDCSF_PROJECTNAME:
		{
			if (!m_layout.IsVisible(nLocation))
			{
				ASSERT(m_layout.GetMaximiseState() != TDCMS_NORMAL);
				SetMaximizeState(TDCMS_NORMAL);
			}

			GetDlgItem(IDC_PROJECTNAME)->SetFocus();
		}
		break;

	default:
		ASSERT(0);
		return;
	}

	InvalidateAllCtrls(this, FALSE);
}

BOOL CToDoCtrl::HasFocus(TDC_SETFOCUSTO nLocation) const
{
	if (!m_layout.IsVisible(nLocation))
		return FALSE;

	switch (nLocation)
	{
	case TDCSF_TASKVIEW:
		return m_taskTree.HasFocus();

	case TDCSF_COMMENTS:
		return m_ctrlComments.HasFocus();

	case TDCSF_ATTRIBUTES:
		return m_ctrlAttributes.HasFocus();

	case TDCSF_PROJECTNAME:
		return (GetFocus() == GetDlgItem(IDC_PROJECTNAME));
	}

	ASSERT(0);
	return FALSE;
}

CString CToDoCtrl::GetControlDescription(const CWnd* pCtrl) const
{
	CEnString sText; 

	if (IsChild(pCtrl))
	{
		// comments field
		if (IsChildOrSame(m_ctrlComments, *pCtrl))
		{
			sText.LoadString(IDS_COMMENTSTYPE);
		}
		else if (IsChildOrSame(m_taskTree, *pCtrl))
		{
			sText.LoadString(IDS_TASKTREE);
		}
		else if (IsChildOrSame(m_ctrlAttributes, *pCtrl))
		{
			sText.LoadString(IDS_ATTRIBUTES);
		}
		else if (pCtrl == GetDlgItem(IDC_PROJECTNAME))
		{
			sText.LoadString(IDS_TDC_FIELD_PROJECT);
		}

		sText.Remove('&');
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
		if (!WebMisc::IsURL(aFilePaths[nFile]))
			FileMisc::MakeFullPath(aFilePaths[nFile], sParentFolder);
	}
}

CString CToDoCtrl::GetFullPath(const CString& sFilePath) const
{
	if (WebMisc::IsURL(sFilePath))
		return sFilePath;

	return FileMisc::GetFullPath(sFilePath, GetLastSaveFolder());
}

LRESULT CToDoCtrl::OnCanDropObject(WPARAM wParam, LPARAM lParam)
{
	if (!wParam || !lParam || IsReadOnly())
	{
		ASSERT(0);
		return FALSE;
	}

	TLDT_DATA* pData = (TLDT_DATA*)wParam;
	CWnd* pTarget = (CWnd*)lParam;

	if (pTarget == &m_taskTree.Tree())
	{
		if (pData->HasFiles())
		{
			if (pData->dwTaskID)
			{
				if (m_data.IsTaskLocked(pData->dwTaskID))
					return FALSE;

				if (!pData->bImportTasks)
					return CanEditTask(pData->dwTaskID, TDCA_FILELINK);
			}

			// Check with parent
			TDCDROPIMPORT data(pData->dwTaskID, *pData->pFilePaths);
			return GetParent()->SendMessage(WM_TDCM_CANIMPORTFROMDROP, (WPARAM)GetSafeHwnd(), (LPARAM)&data);
		}

		if (pData->pOutlookSelection || CMSOutlookHelper::IsOutlookObject(pData->pObject))
		{
			if (pData->dwTaskID)
			{
				if (m_data.IsTaskLocked(pData->dwTaskID))
					return FALSE;

				if (!pData->bImportTasks)
					return CanEditTask(pData->dwTaskID, TDCA_FILELINK);
			}

			// else 
			return CanEditSelectedTask(TDCA_NEWTASK);
		}

		if (pData->HasText())
		{
			// Check with parent
			TDCDROPIMPORT data(pData->dwTaskID, pData->GetText());
			return GetParent()->SendMessage(WM_TDCM_CANIMPORTFROMDROP, (WPARAM)GetSafeHwnd(), (LPARAM)&data);
		}

		// else
		ASSERT(0);
		return FALSE;
	}

	// else
	return TRUE;
}

LRESULT CToDoCtrl::OnDropObject(WPARAM wParam, LPARAM lParam)
{
	if (!OnCanDropObject(wParam, lParam))
		return 0L;

	ASSERT(wParam && lParam);
	
	TLDT_DATA* pData = (TLDT_DATA*)wParam;
	CWnd* pTarget = (CWnd*)lParam;

	// Handle dropping files on to a tree item or the 'File Link' field
	CStringArray aFiles;

	if (pData->pOutlookSelection && pData->pOutlookSelection->GetCount())
	{
		int nNumItems = pData->pOutlookSelection->GetCount();

		for (int nItem = 0; nItem < nNumItems; nItem++)
		{
			// format outlook link
			OutlookAPI::_Item obj(pData->pOutlookSelection->Item(COleVariant((short)(nItem + 1))));

			DWORD dwFlags = (Misc::ModKeysArePressed(MKS_SHIFT) ? 0 : OAFMT_NICE);
			aFiles.Add(CMSOutlookHelper::FormatItemAsUrl(obj, dwFlags));
		}
	}
	else if (pData->pFilePaths && pData->pFilePaths->GetSize())
	{
		aFiles.Copy(*(pData->pFilePaths));
	}

	// specific handling
	if (pTarget == &m_taskTree.Tree())
	{
		if (pData->dwTaskID)
			SelectTask(pData->dwTaskID, FALSE);

		if (aFiles.GetSize())
		{
			if (pData->bImportTasks)
			{
				switch (CreateTasksFromOutlookObjects(pData))
				{
				case -1: // user cancelled
					return 0L;

				case 0:  // failed => not an outlook object
					{
						TDCDROPIMPORT data(pData->dwTaskID, aFiles);
						return GetParent()->SendMessage(WM_TDCM_IMPORTFROMDROP, (WPARAM)GetSafeHwnd(), (LPARAM)&data);
					}

				default:
					break; // all good
				}
			}
			else
			{
				SetSelectedTaskFileLinks(aFiles, TRUE);
			}
		}
		else if (pData->HasText())
		{
			TDCDROPIMPORT data(pData->dwTaskID, pData->GetText());
			return GetParent()->SendMessage(WM_TDCM_IMPORTFROMDROP, (WPARAM)GetSafeHwnd(), (LPARAM)&data);
		}
		else
		{
			switch (CreateTasksFromOutlookObjects(pData))
			{
				case -1: // user cancelled
					return 0L;

				case 0:  // failed => not an outlook object
					// TODO
					return 0L;

				default:
					break; // all good
			}
		}

		SetFocus(TDCSF_TASKVIEW);
		PostMessage(WM_TDC_FIXUPPOSTDROPSELECTION, 0L, (LPARAM)pData->dwTaskID);
	}

	return 0L;
}

int CToDoCtrl::CreateTasksFromOutlookObjects(const TLDT_DATA* pData)
{
	CTaskFile tasks;

	int nRet = CTDCOutlookImportHelper::ImportTasks(pData, &tasks);

	switch (nRet)
	{
	case -1: // cancelled
	case  0: // failed ??
		return nRet;
	}

	// add to current tasklist
	HTREEITEM htiInsert = m_taskTree.GetItem(pData->dwTaskID);
	
	if (!htiInsert)
		htiInsert = m_taskTree.HitTestItem(CPoint(1, pData->ptClient.y));
	
	if (htiInsert)
	{
		SelectItem(htiInsert);
		VERIFY(PasteTasks(tasks, TDC_INSERTATBOTTOMOFSELTASK));
	}
	else
	{
		VERIFY(PasteTasks(tasks, TDC_INSERTATBOTTOM));
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
	else if (HasFilePath())
	{
		sKeyPath = CPreferences::KeyFromFile(m_sLastSavePath);
	}
	else
	{
		sKeyPath = _T("Default");
	}
	ASSERT(!sKeyPath.IsEmpty());

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
	
	prefs.WriteProfileInt(_T("DefaultRecurrence"), _T("RecurFrom"), m_nDefRecurFrom);
	prefs.WriteProfileInt(_T("DefaultRecurrence"), _T("RecurReuse"), m_nDefRecurReuse);
}

void CToDoCtrl::LoadDefaultRecurrence(const CPreferences& prefs)
{
	ASSERT (GetSafeHwnd());
	
	CString sKey = GetPreferencesKey(); // no subkey
	
	m_nDefRecurFrom = prefs.GetProfileEnum(_T("DefaultRecurrence"), _T("RecurFrom"), TDIRO_DUEDATE);
	m_nDefRecurReuse = prefs.GetProfileEnum(_T("DefaultRecurrence"), _T("RecurReuse"), TDIRO_REUSE);
}

void CToDoCtrl::SaveSplitPos(CPreferences& prefs) const
{
	ASSERT (GetSafeHwnd());

	CString sPrefsKey(GetPreferencesKey());
	
	m_layout.SaveState(prefs, sPrefsKey);
	m_ctrlAttributes.SaveState(prefs, sPrefsKey);
}

void CToDoCtrl::LoadSplitPos(const CPreferences& prefs)
{
	ASSERT(GetSafeHwnd());

	CString sPrefsKey(GetPreferencesKey());

	m_layout.LoadState(prefs, sPrefsKey);
	m_ctrlAttributes.LoadState(prefs, sPrefsKey);
}

void CToDoCtrl::SaveAttributeVisibility(CTaskFile& tasks) const
{
	if (HasStyle(TDCS_SAVEUIVISINTASKLIST))
		tasks.SetAttributeVisibility(m_visColEdit);
}

void CToDoCtrl::SaveAttributeVisibility(CPreferences& prefs) const
{
	m_visColEdit.Save(prefs, GetPreferencesKey());
}

void CToDoCtrl::LoadAttributeVisibility(const CTaskFile& tasks, const CPreferences& prefs)
{
	// attrib visibility can be stored inside the file or the preferences
	TDCCOLEDITVISIBILITY vis;

	if (tasks.GetAttributeVisibility(vis))
	{
		m_styles[TDCS_SAVEUIVISINTASKLIST] = TRUE;
	}
	else if (!vis.Load(prefs, GetPreferencesKey()))
	{
		vis = m_visColEdit;
	}

	// Preserve edit field visibility
	vis.ShowColorEditIfAsColumns(m_visColEdit.IsEditFieldVisible(TDCA_COLOR));

	SetColumnFieldVisibility(vis);
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
		m_taskTree.RecalcUntrackedColumnWidths();

	return hti;
}

void CToDoCtrl::EndLabelEdit(BOOL bCancel)
{
	m_eTaskName.EndEdit(bCancel);
}

BOOL CToDoCtrl::Flush() 
{
	CWnd* pFocus = GetFocus();

	if (pFocus)
	{
		if (pFocus == &m_eTaskName)
		{
			EndLabelEdit(FALSE);
		}
		else if (CDialogHelper::IsChildOrSame(&m_ctrlAttributes, pFocus))
		{
			m_ctrlAttributes.Flush();
		}
	}

	m_treeDragDrop.CancelDrag();

	return HandleUnsavedComments();
}

TDC_FILE CToDoCtrl::CheckIn()
{
	Flush();
	EndTimeTracking(TRUE, FALSE);

	return m_sourceControl.CheckIn();
}

BOOL CToDoCtrl::IsCheckedOut() const 
{ 
	return m_sourceControl.IsCheckedOut();
}

BOOL CToDoCtrl::IsSourceControlled() const 
{ 
	return m_sourceControl.IsSourceControlled();
}

BOOL CToDoCtrl::CanAddToSourceControl(BOOL bAdd) const
{
	return m_sourceControl.CanAddToSourceControl(bAdd);
}

BOOL CToDoCtrl::AddToSourceControl(BOOL bAdd)
{
	return (m_sourceControl.AddToSourceControl(bAdd) == TDCF_SUCCESS);
}

TDC_FILE CToDoCtrl::CheckOut(CString& sCheckedOutTo, BOOL bForce)
{
	CWaitCursor cursor;
	CTaskFile tasks(m_sPassword);

	TDC_FILE nResult = m_sourceControl.CheckOut(tasks, sCheckedOutTo, bForce);

	if (nResult == TDCF_SUCCESS)
	{
		// load tasks
		tasks.Decrypt();
		VERIFY(LoadTasks(tasks));
	}

	return nResult;
}

int CToDoCtrl::FindTasks(const SEARCHPARAMS& params, CResultArray& aResults) const
{
	return TCF().FindTasks(params, aResults, HasDueTodayColor());
}

BOOL CToDoCtrl::HasOverdueTasks() const
{
	return m_calculator.HasOverdueTasks();
}

BOOL CToDoCtrl::HasDueTodayTasks() const
{
	return m_calculator.HasDueTodayTasks();
}

BOOL CToDoCtrl::HasLockedTasks() const
{
	return m_calculator.HasLockedTasks();
}

// External
BOOL CToDoCtrl::SelectNextTask(const CString& sPart, TDC_SELECTNEXTTASK nSelect)
{
	return SelectNextTask(sPart, nSelect, TDCA_ANYTEXTATTRIBUTE, FALSE, FALSE, FALSE);
}

// Internal
BOOL CToDoCtrl::SelectNextTask(const CString& sPart, TDC_SELECTNEXTTASK nSelect, TDC_ATTRIBUTE nAttribID, 
							BOOL bCaseSensitive, BOOL bWholeWord, BOOL /*bFindReplace*/)
{
	if (!SEARCHPARAM::GetAttribType(nAttribID, FALSE) == FT_STRING)
	{
		ASSERT(0);
		return FALSE;
	}

	SEARCHPARAM rule(nAttribID, FOP_INCLUDES, sPart);
	rule.SetMatchWholeWord(bWholeWord);

	SEARCHPARAMS params;
	params.aRules.Add(rule);
	params.bCaseSensitive = bCaseSensitive;

	HTREEITEM htiStart = NULL; // first item
	BOOL bForwards = TRUE;
	
	switch (nSelect)
	{
	case TDC_SELECTFIRST:
		htiStart = TCH().GetFirstItem();
		break;

	case TDC_SELECTNEXT:
		htiStart = TCH().GetNextItem(GetSelectedItem());

		if (htiStart == NULL)
			htiStart = TCH().GetFirstItem();
		break;

	case TDC_SELECTNEXTINCLCURRENT:
		htiStart = GetSelectedItem();
		break;

	case TDC_SELECTPREV:
		htiStart = TCH().GetPrevItem(GetSelectedItem());

		if (htiStart == NULL)
			htiStart = TCH().GetLastItem();

		bForwards = FALSE;
		break;

	case TDC_SELECTLAST:
		htiStart = TCH().GetLastItem();
		bForwards = FALSE;
		break;

	default:
		ASSERT(0);
		break;
	}

	if (!htiStart)
		return FALSE;

	SEARCHRESULT result;
	HTREEITEM htiMatch = TCF().FindNextTask(htiStart, params, result, bForwards, FALSE);

	if (!htiMatch)
		return FALSE;

	return SelectTask(GetTaskID(htiMatch), FALSE);
}

void CToDoCtrl::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CRuntimeDlg::OnShowWindow(bShow, nStatus);
	
	if (!bShow)
	{
		m_findReplace.DestroyDialog();
	}
	else if (!m_ilTaskIcons.GetSafeHandle())
	{
		// Delay-loading of resources else image lists seem to result 
		// in memory leaks inside of system dlls
		LoadTaskIcons();
	}
}

void CToDoCtrl::SpellcheckSelectedTask(BOOL bTitle) 
{
	Flush();

	IMPLEMENT_DATA_UNDO(m_data, TDCUAT_EDIT);

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

	return (CanEditSelectedTask(TDCA_COMMENTS) && !GetSelectedTaskComments().IsEmpty());
}

BOOL CToDoCtrl::CanSpellcheckComments()
{
	return (NULL != m_ctrlComments.GetSpellCheckInterface());
}

void CToDoCtrl::Spellcheck()
{
	IMPLEMENT_DATA_UNDO_EDIT(m_data);

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

BOOL CToDoCtrl::SpellcheckItem(HTREEITEM hti, CSpellCheckDlg* pSpellChecker, BOOL bCheckTitle, BOOL bNotifyNoErrors)
{
	ASSERT(pSpellChecker);
	
	if (!pSpellChecker)
		return FALSE;

	if (!bCheckTitle && !CanSpellcheckComments())
		return TRUE;
	
	DWORD dwTaskID = GetTaskID(hti);
	const TODOITEM* pTDI = GetTask(dwTaskID);
	ASSERT (pTDI);
	
	if (pTDI)
	{
		if (bCheckTitle)
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
		
		int nRet = pSpellChecker->DoModal(CPreferences(), TRUE);
		UpdateWindow();
			
		if ((nRet == IDOK) && CanEditSelectedTask(TDCA_TASKNAMEORCOMMENTS))
		{
			int nChange = SET_NOCHANGE;
			
			if (bCheckTitle)
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
			{
				CDWordArray aModTaskIDs;
				aModTaskIDs.Add(dwTaskID);

				SetModified(TDCA_TASKNAME, aModTaskIDs);
			}
		}
		else if (nRet == IDNOERRORS && bNotifyNoErrors)
		{
			if (bCheckTitle)
				CMessageBox::AfxShow(IDS_TDC_SPELLCHECK_TITLE, IDS_TDC_NOTITLESPELLERRORS, MB_OK);
			else
				CMessageBox::AfxShow(IDS_TDC_SPELLCHECK_TITLE, IDS_TDC_NOCOMMENTSPELLERRORS, MB_OK);
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

BOOL CToDoCtrl::DoFindReplace(TDC_ATTRIBUTE nAttribID)
{
	return m_findReplace.DoFindReplace(nAttribID);
}

BOOL CToDoCtrl::CanDoFindReplace(TDC_ATTRIBUTE nAttribID) const
{
	return m_findReplace.CanDoFindReplace(nAttribID);
}

LRESULT CToDoCtrl::OnFindReplaceSelectNextTask(WPARAM wParam, LPARAM /*lParam*/)
{
	return SelectNextTask(m_findReplace.GetSearchFor(), 
						(TDC_SELECTNEXTTASK)wParam,
						m_findReplace.GetAttribute(),
						m_findReplace.WantCaseSensitive(), 
						m_findReplace.WantWholeWord(), TRUE);
}

LRESULT CToDoCtrl::OnFindReplaceSelectedTask(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return FindReplaceSelectedTaskAttribute(FALSE);
}

LRESULT CToDoCtrl::OnFindReplaceAllTasks(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// Treat as a single edit
	IMPLEMENT_DATA_UNDO_EDIT(m_data);

	// Start at the beginning
	if (SelectNextTask(m_findReplace.GetSearchFor(), 
					TDC_SELECTFIRST, 
					m_findReplace.GetAttribute(),
					m_findReplace.WantCaseSensitive(), 
					m_findReplace.WantWholeWord(), 
					TRUE))
	{
		do 
		{
			ASSERT(GetSelectedTaskCount() == 1);

			FindReplaceSelectedTaskAttribute(TRUE);
		} 
		while (SelectNextTask(m_findReplace.GetSearchFor(), 
							TDC_SELECTNEXT, 
							m_findReplace.GetAttribute(),
							m_findReplace.WantCaseSensitive(), 
							m_findReplace.WantWholeWord(), 
							TRUE));

		return TRUE;
	}

	MessageBeep(MB_ICONHAND);
	return FALSE;
}

LRESULT CToDoCtrl::OnFindReplaceGetExclusionRect(WPARAM wParam, LPARAM lParam)
{
	ASSERT(lParam);

	BOOL bFirstTime = wParam, bUpdown = bFirstTime;
	CRect rExclude;

	switch (m_findReplace.GetAttribute())
	{
	case TDCA_TASKNAME:
		if (bFirstTime)
			m_taskTree.Tree().GetWindowRect(rExclude);
		else
			GetLabelEditRect(rExclude);
		break;

	case TDCA_COMMENTS:
		m_ctrlComments.GetWindowRect(rExclude);

		bUpdown = (m_layout.GetCommentsPos() == TDCUIL_BOTTOM);
		break;

	default:
		ASSERT(0);
		break;
	}
	
	*((LPRECT)lParam) = rExclude;
	return bUpdown;
}

LRESULT CToDoCtrl::OnFindReplaceMsg(WPARAM wParam, LPARAM lParam)
{
	m_findReplace.HandleCmd(wParam, lParam);
	return 0;
}

BOOL CToDoCtrl::FindReplaceSelectedTaskAttribute(BOOL bReplacingAllTasks)
{
	TDC_ATTRIBUTE nAttribID = m_findReplace.GetAttribute();

	// Sanity checks
	if (!m_findReplace.IsReplacing())
	{
		ASSERT(0);
		return FALSE;
	}
	else if (!CanEditSelectedTask(nAttribID))
	{
		ASSERT(bReplacingAllTasks);
		return FALSE;
	}
	
	CString sSelAttrib;

	switch (nAttribID)
	{
	case TDCA_TASKNAME:
		sSelAttrib = GetTaskTitle(m_taskTree.GetSelectedTaskID());
		break;

	case TDCA_COMMENTS:
		sSelAttrib = GetSelectedTaskComments();
		break;
	}

	if (Misc::Replace(m_findReplace.GetSearchFor(), 
						m_findReplace.GetReplaceWith(), 
						sSelAttrib, 
						m_findReplace.WantCaseSensitive(), 
						m_findReplace.WantWholeWord()))
	{
		switch (nAttribID)
		{
		case TDCA_TASKNAME:
			if (SetSelectedTaskTitle(sSelAttrib, FALSE))
				return TRUE;
			break;

		case TDCA_COMMENTS:
			if (m_ctrlComments.FindReplaceAll(m_findReplace.GetSearchFor(), 
											m_findReplace.GetReplaceWith(), 
											m_findReplace.WantCaseSensitive(), 
											m_findReplace.WantWholeWord()))
			{
				CString sTextComments;
				CBinaryData customComments;
				
				if (m_ctrlComments.GetContent(sTextComments, customComments) == -1)
				{
					// TODO
					m_nCommentsState = CS_PENDING;
				}
				else if (SetSelectedTaskComments(sTextComments, customComments, TRUE))
				{
					return TRUE;
				}
			}
			break;
		}
	}

	MessageBeep(MB_ICONHAND);
	return FALSE;
}

const CBinaryData& CToDoCtrl::GetSelectedTaskCustomComments(CONTENTFORMAT& cfComments) const 
{ 
	if (GetSelectedTaskCount() == 0)
	{
		static CBinaryData data;

		cfComments.Empty();
		return data;
	}

	return m_taskTree.GetSelectedTaskCustomComments(cfComments); 
}

int CToDoCtrl::GetSelectedTaskCustomAttributeData(CTDCCustomAttributeDataMap& mapData, BOOL bFormatted) const
{
	mapData.RemoveAll();
	
	int nDef = m_aCustomAttribDefs.GetSize();
	TDCCADATA data;
	
	while (nDef--)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& def = m_aCustomAttribDefs.GetData()[nDef];
		GetSelectedTaskCustomAttributeData(def.sUniqueID, data, bFormatted);

		mapData[def.sUniqueID] = data;
	}
	
	return mapData.GetCount();
}

void CToDoCtrl::SetDefaultTaskAttributeValues(const TODOITEM& tdi)
{
	m_tdiDefault = tdi;
	m_cfDefault = m_tdiDefault.cfComments;

	m_data.SetDefaultCommentsFormat(m_cfDefault);
	m_data.SetDefaultTimeUnits(tdi.timeEstimate.nUnits, tdi.timeSpent.nUnits);
	m_data.SetDefaultStatus(tdi.sStatus);
	
	TODOITEM::SetModifierName(tdi.sCreatedBy); // 'this' user

	// set default task creation date to zero so that new tasks
	// always get the current date
	CDateHelper::ClearDate(m_tdiDefault.dateCreated);

	UpdateDefaultTaskCustomAttributeValues();
}

void CToDoCtrl::UpdateDefaultTaskCustomAttributeValues()
{
	CTDCCustomAttributeDataMap dataDef;
	int nCust = m_aCustomAttribDefs.GetSize();

	while (nCust--)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = m_aCustomAttribDefs[nCust];

		// The only default custom attribute value we currently support
		// is single selection lists with default data but without a 'blank' item
		// ie. A blank default item is not appropriate
		if (attribDef.bEnabled && 
			!attribDef.IsMultiList() && 
			attribDef.aDefaultListData.GetSize() && 
			attribDef.HasFeature(TDCCAF_EXCLUDEBLANKITEM))
		{
			dataDef.SetAt(attribDef.sUniqueID, attribDef.aDefaultListData[0]);
		}
	}

	m_tdiDefault.SetCustomAttributeValues(dataDef);
}

LRESULT CToDoCtrl::OnTDCToggleTimeTracking(WPARAM wParam, LPARAM lParam)
{
	ToggleTimeTracking(GetSelectedItem());
	return 0L;
}

LRESULT CToDoCtrl::OnTDCAddTimeToLogFile(WPARAM wParam, LPARAM lParam)
{
	DoAddTimeToLogFile();
	return 0L;
}

LRESULT CToDoCtrl::OnTDCSelectDependencies(WPARAM wParam, LPARAM lParam)
{
	GotoSelectedTaskDependency();
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
		DWORD dwTaskID = m_data.GetTaskReferenceID(TSH().GetNextItemData(pos));

		if (dwTaskID)
			aTaskIDs.Add(dwTaskID);
	}

	// select any found items
	if (aTaskIDs.GetSize())
		return SelectTasks(aTaskIDs); 

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
		return SelectTasks(aTaskRefIDs);

	// else
	return FALSE;
}

BOOL CToDoCtrl::GotoSelectedTaskLocalDependencies()
{
	// traverse the selected items adding any dependencies to a list
	CDWordArray aDependIDs;
	POSITION pos = TSH().GetFirstItemPos();
			
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		CDWordArray aLocalDepends;

		if (m_data.GetTaskLocalDependencies(dwTaskID, aLocalDepends))
			aDependIDs.Append(aLocalDepends);
	}

	// select any found items
	if (aDependIDs.GetSize())
		return SelectTasks(aDependIDs);

	// else
	return FALSE;
}

BOOL CToDoCtrl::GotoSelectedTaskLocalDependents()
{
	// traverse the selected items adding any dependents to a list
	CDWordArray aDependentIDs;
	POSITION pos = TSH().GetFirstItemPos();
			
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		CDWordArray aLocalDependents;

		if (m_data.GetTaskLocalDependents(dwTaskID, aLocalDependents, FALSE))
			aDependentIDs.Append(aLocalDependents);
	}

	// select any found items
	if (aDependentIDs.GetSize())
		return SelectTasks(aDependentIDs);

	// else
	return FALSE;
}

BOOL CToDoCtrl::GotoSelectedTaskDependency()
{
	CDWordArray aLocalDepends;
	CStringArray aOtherDepends;

	if (GetAllSelectedTaskDependencies(aLocalDepends, aOtherDepends))
	{
		if (aLocalDepends.GetSize() == 0)
		{
			if (aOtherDepends.GetSize() > 0)
				return ShowTaskLink(aOtherDepends[0], FALSE);
		}
		else
		{
			return SelectTasks(aLocalDepends);
		}
	}

	// else
	return FALSE;
}

BOOL CToDoCtrl::EditSelectedTaskDependency()
{
	if (CanEditSelectedTask(TDCA_DEPENDENCY))
	{
		TDCGETTASKS filter(TDCGT_NOTDONE);

		filter.mapAttribs.Add(TDCA_TASKNAME);
		filter.mapAttribs.Add(TDCA_ICON);

		CTaskFile tasks;
		GetTasks(tasks, filter);

		CDWordArray aSelTaskIDs;
		m_taskTree.GetSelectedTaskIDs(aSelTaskIDs, TRUE);

		CTDCDependencyArray aDepends;
		GetSelectedTaskDependencies(aDepends);

		CTDLTaskDependencyEditDlg dialog(aSelTaskIDs,
										 tasks, 
										 m_ilTaskIcons, 
										 aDepends, 
										 HasStyle(TDCS_SHOWPARENTSASFOLDERS),
										 HasStyle(TDCS_AUTOADJUSTDEPENDENCYDATES));

		if (dialog.DoModal(s_hIconDependsDlg) == IDOK)
		{
			CTDCDependencyArray aDepends;
			dialog.GetDependencies(aDepends);

			return SetSelectedTaskDependencies(aDepends);
		}
	}

	// else
	return FALSE;
}


BOOL CToDoCtrl::EditSelectedTaskRecurrence()
{
	if (CanEditSelectedTask(TDCA_RECURRENCE))
	{
		TDCRECURRENCE tr;
		GetSelectedTaskRecurrence(tr);

		// use due date if present else start date
		COleDateTime dtDefault = m_taskTree.GetSelectedTaskDate(TDCD_DUE);

		if (!CDateHelper::IsDateSet(dtDefault))
			dtDefault = m_taskTree.GetSelectedTaskDate(TDCD_START);

		CTDLRecurringTaskOptionDlg dialog(tr, dtDefault);

		if (dialog.DoModal(s_hIconRecurDlg) == IDOK)
		{
			TDCRECURRENCE trNew;
			dialog.GetRecurrenceOptions(trNew);

			return SetSelectedTaskRecurrence(trNew);
		}
	}

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
	}
	
	CRuntimeDlg::OnTimer(nIDEvent);
}

LRESULT CToDoCtrl::OnMidnight(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	FileMisc::LogText(_T("CToDoCtrl::OnMidnight"));
	Invalidate();

	return 0L;
}

void CToDoCtrl::IncrementTrackedTime(BOOL bEnding)
{
	AF_NOREENTRANT;

	// if we are editing the title of the task being tracked then 
	// leave immediately and wait until the editing has ended
	if (!bEnding && IsTaskLabelEditing() && m_timeTracking.IsTracking(GetSelectedTaskID()))
		return;

	double dIncrement = m_timeTracking.IncrementTrackedTime(); // hours
	
	if (dIncrement > 0.0)
	{
		DWORD dwTaskID = m_timeTracking.GetTrackedTaskID(TRUE);
		ASSERT(dwTaskID);

		// Tracked/logged time is always in hours
		m_dTrackedTimeElapsedHours += dIncrement;

		TDCTIMEPERIOD time(dIncrement, TDCU_HOURS);
		
		if ((dwTaskID == GetSelectedTaskID()) && (GetSelectedTaskCount() == 1))
		{
			// this will also update the Time Spent field
			SetSelectedTaskTimeSpent(time, TRUE); // offset
		}
		else
		{
			CDWordArray aModTaskIDs;
			aModTaskIDs.Add(dwTaskID);

			m_data.SetTaskTimeSpent(dwTaskID, time, TRUE); // offset
			SetModified(TDCA_TIMESPENT, aModTaskIDs);
		}

		// Is a reminder due?
		if (m_timeTracking.IsReminderDue())
		{
			TRACE(_T("Sending time track reminder notification as %.2f seconds\n"), 
						(m_dTrackedTimeElapsedHours / TICKS2HOURS) / 1000);

			m_timeTracking.ResetReminderIsDue();
			GetParent()->SendMessage(WM_TDCN_TIMETRACKREMINDER, (WPARAM)GetSafeHwnd(), dwTaskID);
		}
	}
}

LRESULT CToDoCtrl::OnCustomUrl(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	ASSERT(wParam == (WPARAM)m_ctrlComments.GetDlgCtrlID());
	
	// FALSE says only handle "tdl://" links
	GotoFile((LPCTSTR)lParam, FALSE);
	
	return 0;
}

BOOL CToDoCtrl::SelectTasksInHistory(BOOL bForward) 
{ 
	if (!CanSelectTasksInHistory(bForward))
		return FALSE;

	if (!HandleUnsavedComments())
		return FALSE;

	m_taskTree.SelectTasksInHistory(bForward);
	UpdateControls();

	return TRUE;
}

LRESULT CToDoCtrl::OnTDCDisplayLink(WPARAM wParam, LPARAM lParam)
{
	return GotoFile((LPCTSTR)lParam);
}

BOOL CToDoCtrl::GotoFile(const CString& sFile, BOOL bShellExecute)
{
	if (sFile.IsEmpty())
		return FALSE;
	
	if (TDCTASKLINK::IsTaskLink(sFile, TRUE))
	{
		ShowTaskLink(sFile, TRUE);
		return TRUE;
	}
	else if (bShellExecute)
	{
		// set the current working directory to that of the tasklist
		CString sTasklistFolder = GetLastSaveFolder();

		// try to run it but disable default error handling
		if (CFileEdit::GotoFile(*this, sFile, sTasklistFolder, FALSE) >= SE_ERR_SUCCESS)
			return TRUE;
	}

	// all else, pass to parent
	GetParent()->SendMessage(WM_TDCM_FAILEDLINK, (WPARAM)GetSafeHwnd(), (LPARAM)(LPCTSTR)sFile);

	return TRUE; // parent will handle errors
}

void CToDoCtrl::ExpandTasks(TDC_EXPANDCOLLAPSE nWhat, BOOL bExpand)
{
	if (!CanExpandTasks(nWhat, bExpand))
		return;

	// PERMANENT LOGGING ///////////////////////////////////////////////
	CScopedLogTimer timer(_T("ExpandTasks(%s)"), Misc::Format(bExpand));
	////////////////////////////////////////////////////////////////////

	CHTIList prevSel;
	TSH().CopySelection(prevSel);

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

	if (!bExpand)
	{
		// if any previously selected tasks has a collapsed parent,
		// move the selection to the first visible parent item
		// and refresh the edit controls
		POSITION pos = prevSel.GetHeadPosition();
		BOOL bContinue = TRUE;

		while (pos && bContinue)
		{
			HTREEITEM hti = prevSel.GetNext(pos);
			
			while (hti && !TCH().IsParentItemExpanded(hti))
			{
				hti = m_taskTree.GetParentItem(hti);
				bContinue = FALSE;
			}

			if (!bContinue)
			{
				SelectItem(hti);
				UpdateControls();
			}
		}
	}
}

void CToDoCtrl::SearchAndExpand(const SEARCHPARAMS& params, BOOL bExpand)
{
	// perform the search
	CResultArray aResults;
	int nNumRes = FindTasks(params, aResults);
	
	if (bExpand)
	{
		// Get the unique IDs of the found tasks together with their parents
		CDWordArray aTaskIDs, aParentIDs;
	
		for (int nRes = 0; nRes < nNumRes; nRes++)
		{
			DWORD dwTaskID = aResults[nRes].dwTaskID;

			// Task must be a parent tree item
			if (m_taskTree.ItemHasChildren(m_taskTree.GetItem(dwTaskID)))
			{
				Misc::AddUniqueItemT(dwTaskID, aTaskIDs);

				// Its parents
				if (m_data.GetTaskParentIDs(dwTaskID, aParentIDs))
					Misc::AppendItems(aParentIDs, aTaskIDs, TRUE);
			}
		}

		m_taskTree.ExpandTasks(aTaskIDs);
	}
	else // collapsing
	{
		for (int nRes = 0; nRes < nNumRes; nRes++)
		{
			HTREEITEM hti = m_taskTree.GetItem(aResults[nRes].dwTaskID);
			ASSERT(hti);

			if (hti && m_taskTree.ItemHasChildren(hti))
				m_taskTree.ExpandItem(hti, bExpand);
		}
	}
}

BOOL CToDoCtrl::CanExpandTasks(TDC_EXPANDCOLLAPSE nWhat, BOOL bExpand) const
{
	if (m_taskTree.GetItemCount() == 0)
		return FALSE;
		
	switch (nWhat)
	{
	case TDCEC_ALL:
		if (bExpand)
			return m_taskTree.TCH().IsAnyItemCollapsed();
		else
			return m_taskTree.TCH().IsAnyItemExpanded();
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
	CString sClipID;
	GetClipboardID(sClipID, FALSE);

	return sClipID;
}

BOOL CToDoCtrl::GetClipboardID(CString& sClipID, BOOL bArchive) const
{
	CString sFilePath;

	if (bArchive && !GetArchivePath(sFilePath))
		return FALSE;

	if (sFilePath.IsEmpty())
		sFilePath = GetFilePath();

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

		if (CTaskClipboard::GetTasks(GetClipboardID(), tasks))
 			return (LRESULT)&tasks;
	}

	// else
	return NULL;
}

LRESULT CToDoCtrl::OnTDCDoTaskLink(WPARAM /*wParam*/, LPARAM lParam)
{
	return ShowTaskLink((LPCTSTR)lParam, TRUE);
}

LRESULT CToDoCtrl::OnTDCFailedLink(WPARAM /*wParam*/, LPARAM lParam)
{
	// Handle relative file path links
	CString sLink((LPCTSTR)lParam);

	if (TDCTASKLINK::IsTaskLink(sLink, TRUE))
	{
		return ShowTaskLink(sLink, TRUE);
	}
	else if (!CMSOutlookHelper::IsOutlookUrl(sLink) && ::PathIsRelative(sLink))
	{
		sLink = GetFullPath(sLink);
		
		if (FileMisc::Run(*this, sLink) >= SE_ERR_SUCCESS)
			return 0L;
	}

	// else forward on to our parent
	return GetParent()->SendMessage(WM_TDCM_FAILEDLINK, (WPARAM)GetSafeHwnd(), lParam);
}

BOOL CToDoCtrl::ShowTaskLink(const CString& sLink, BOOL bURL)
{
	if (sLink.IsEmpty())
		return FALSE;

	CString sFile;
	DWORD dwTaskID = 0;

	if (!ParseTaskLink(sLink, bURL, dwTaskID, sFile))
		return FALSE;

	// if there's a file attached then pass to parent because we can't handle it.
	if (!sFile.IsEmpty())
	{
		return GetParent()->SendMessage(WM_TDCM_SELECTTASK, dwTaskID, (LPARAM)(LPCTSTR)sFile);
	}
	else if (dwTaskID)
	{
		if (SelectTask(dwTaskID, TRUE))
		{
			SetFocus(TDCSF_TASKVIEW);
			return TRUE;
		}

		// else
		CMessageBox::AfxShow(IDS_TDC_TASKIDNOTFOUND_TITLE, IDS_TDC_TASKIDNOTFOUND);
	}
	else
	{
		CMessageBox::AfxShow(IDS_TDC_ZEROINVALIDTASKID_TITLE, IDS_TDC_ZEROINVALIDTASKID);
	}

	return FALSE;
}

void CToDoCtrl::OnSelChangeCommentsType()
{
	if (!HandleUnsavedComments())
	{
		m_ctrlComments.SetSelectedFormat(m_cfComments);
		return;
	}

	BOOL bMixedSelection = (m_cfComments.IsEmpty());
	m_ctrlComments.GetSelectedFormat(m_cfComments);

	IMPLEMENT_DATA_UNDO_EDIT(m_data);

	// modify the comments type of the selected tasks
	// without clearing any custom comments
	BOOL bChange = FALSE;
	POSITION pos = TSH().GetFirstItemPos();
	CDWordArray aModTaskIDs;
		
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		HandleModResult(dwTaskID, m_data.SetTaskCommentsType(dwTaskID, m_cfComments), aModTaskIDs);
	}

	if (!aModTaskIDs.GetSize())
		return;

	SetModified(TDCA_COMMENTS, aModTaskIDs);

	// update comments control state if previously in a mixed state
	if (bMixedSelection)
		UpdateControls();
}

void CToDoCtrl::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    CRuntimeDlg::OnSettingChange(uFlags, lpszSection);

	m_ctrlAttributes.RefreshDateTimeFormatting();
}

LRESULT CToDoCtrl::OnFixupPostDropSelection(WPARAM /*wp*/, LPARAM lp)
{
	if (lp)
		SelectTask(lp, FALSE);

    return 0L;
}

BOOL CToDoCtrl::CanUndoLastAction(BOOL bUndo) const 
{ 
	if (IsReadOnly())
		return FALSE;

	// handle comments field
	if (m_ctrlComments.HasFocus())
		return (m_nCommentsState != CS_CLEAN);

	// Handle edit fields as below
	CWnd* pFocus = GetFocus();

	if (pFocus && CWinClasses::IsClass(*pFocus, WC_EDIT))
	{
		CEdit* pEdit = (CEdit*)pFocus;
		
		if (pEdit->CanUndo() || IsTaskLabelEditing() || (pEdit->GetDlgCtrlID() == IDC_PROJECTNAME))
			return TRUE;
	}

	return m_data.CanUndoLastAction(bUndo); 
}

BOOL CToDoCtrl::UndoLastAction(BOOL bUndo)
{
	if (IsReadOnly())
		return FALSE;

	// handle comments field
	if (m_ctrlComments.HasFocus())
		return bUndo ? m_ctrlComments.Undo() : m_ctrlComments.Redo();

	// We pass the undo request to the focused edit if:
	// 1. Tree label editing
	// OR
	// 2. Project description editing
	// OR
	// 3. There is something to be undone
	CWnd* pFocus = GetFocus();

	if (pFocus && CWinClasses::IsClass(*pFocus, WC_EDIT))
	{
		CEdit* pEdit = (CEdit*)pFocus;

		if (pEdit->CanUndo() || IsTaskLabelEditing() || (pEdit->GetDlgCtrlID() == IDC_PROJECTNAME))
		{
			pEdit->Undo();
			return FALSE; 
		}
	}

	if (m_data.CanUndoLastAction(bUndo))
	{
 		CWaitCursor cursor;
		CLockUpdates lu(*this);
		HOLD_REDRAW(*this, m_taskTree);

		TDCSELECTIONCACHE cache;
		CDWordArray aTaskIDs;

		// get the list of the task IDs that will be undone/redone
		// and clear selection if we will be removing the selection
		TDC_UNDOACTIONTYPE nUndoType = m_data.GetLastUndoActionType(bUndo);
		BOOL bClearSelection = ((nUndoType == TDCUAT_DELETE && !bUndo) || 
								(nUndoType == TDCUAT_ADD && bUndo));

		if (bClearSelection)
		{
			CacheTreeSelection(cache);
			DeselectAll();
		}
		else
		{
			m_data.GetLastUndoActionTaskIDs(bUndo, aTaskIDs);
		}

		CArrayUndoElements aElms;

		if (m_data.UndoLastAction(bUndo, aElms))
		{
			VERIFY(UndoLastActionItems(aElms));

			m_taskTree.OnUndoRedo(bUndo);

			// restore selection
			if (bClearSelection || !SelectTasks(aTaskIDs))
			{
				if (!RestoreTreeSelection(cache))
					SelectTasksInHistory(FALSE);
			}
			
			// update current selection
			UpdateControls();

			// If the operation just un/redone was an edit then we treat it as such
			if (nUndoType == TDCUAT_EDIT)
 				SetModified(TDCA_ALL, aTaskIDs);
			else
				SetModified(TDCA_UNDO, aTaskIDs);

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
			CAutoFlag af(m_bDeletingTasks, TRUE);

			// note: DeleteTask on the Parent will already have disposed of the children
			// so we can expect hti to be NULL on occasion. ie don't ASSERT it
			HTREEITEM hti = m_taskTree.GetItem(elm.dwTaskID);
			ASSERT(hti);
			
			if (hti)
			{
				if (m_timeTracking.IsTrackingTask(elm.dwTaskID, FALSE))
					EndTimeTracking(FALSE, FALSE);

				// Don't cleanup references because we handle that ourselves
				m_taskTree.DeleteItem(hti, FALSE); 
			}
		}
		else if (elm.nOp == TDCUEO_ADD)
		{
			// find parent item and restore task
			HTREEITEM htiParent = m_taskTree.GetItem(elm.dwParentID);
			ASSERT(htiParent || !elm.dwParentID);

			HTREEITEM htiPrevSibling = m_taskTree.GetItem(elm.dwPrevSiblingID);
			ASSERT(htiPrevSibling || !elm.dwPrevSiblingID);
			
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
			ASSERT(hti);

			HTREEITEM htiDestParent = m_taskTree.GetItem(elm.dwParentID); // original owner
			ASSERT(htiDestParent || !elm.dwParentID);

			HTREEITEM htiDestPrevSibling = m_taskTree.GetItem(elm.dwPrevSiblingID); // original previous sibling
			ASSERT(htiDestPrevSibling || !elm.dwPrevSiblingID);

			if (htiDestPrevSibling == NULL)
				htiDestPrevSibling = TVI_FIRST;
			
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
	ASSERT(lp);
	ASSERT(((HWND)wp == m_taskTree.GetSafeHwnd()) ||
		   ((HWND)wp == m_ctrlAttributes.GetSafeHwnd()) ||
		   ((HWND)wp == m_infoTip.GetSafeHwnd()));

	return (LRESULT)m_reminders.GetTaskReminder(lp);
}

void CToDoCtrl::SetUITheme(const CUIThemeFile& theme)
{
	ASSERT(GetSafeHwnd());
	
	m_theme = theme;
	
	GraphicsMisc::VerifyDeleteObject(m_brUIBack);
	
	if (m_theme.crAppBackLight != CLR_NONE)
		m_brUIBack.CreateSolidBrush(m_theme.crAppBackLight);
	
	m_ctrlComments.SetUITheme(m_theme);
	m_ctrlAttributes.SetUITheme(m_theme);

	m_taskTree.SetSplitBarColor(m_theme.crAppBackDark);
	m_layout.SetSplitBarColor(m_theme.crAppBackDark);

	Invalidate();
}

void CToDoCtrl::SetNumPriorityRiskLevels(int nNumLevels)
{
	m_ctrlAttributes.SetNumPriorityRiskLevels(nNumLevels);
}

HBRUSH CToDoCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CRuntimeDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
		if (CThemed::IsAppThemed())
		{
			if (m_theme.crAppText != CLR_NONE)
				pDC->SetTextColor(m_theme.crAppText);

			if (m_brUIBack.GetSafeHandle())
			{
				pDC->SetBkMode(TRANSPARENT);
				hbr = m_brUIBack;
			}
		}
		break;
	}
	
	return hbr;
}

void CToDoCtrl::RefreshReminders()
{ 
	// Called after the app has made a change to a task's reminder
	m_taskTree.OnReminderChange();
	m_ctrlAttributes.RefreshSelectedTasksValue(TDCA_REMINDER);
}

TDC_ATTRIBUTE CToDoCtrl::GetFocusedControlAttribute() const
{
	HWND hFocus = ::GetFocus();

	if (hFocus == NULL)
		return TDCA_NONE;

	if (hFocus == m_eTaskName)
		return TDCA_TASKNAME;

	if (IsChildOrSame(m_ctrlComments, hFocus))
		return TDCA_COMMENTS;

	if (IsChildOrSame(m_ctrlAttributes, hFocus))
		return m_ctrlAttributes.GetSelectedAttributeID();

	if (hFocus == ::GetDlgItem(*this, IDC_PROJECTNAME))
		return TDCA_PROJECTNAME;

	return TDCA_NONE;
}

BOOL CToDoCtrl::CanClearSelectedTaskFocusedAttribute() const
{
	TDC_ATTRIBUTE nAttribID = GetFocusedControlAttribute();

	return CanClearSelectedTaskAttribute(nAttribID);
}

BOOL CToDoCtrl::ClearSelectedTaskFocusedAttribute()
{
	TDC_ATTRIBUTE nAttribID = GetFocusedControlAttribute();

	return ClearSelectedTaskAttribute(nAttribID);
}

BOOL CToDoCtrl::CanClearSelectedTaskAttribute(TDC_ATTRIBUTE nAttribID) const
{
	if (!CanEditSelectedTask(nAttribID))
		return FALSE;

	switch (nAttribID)
	{
	case TDCA_LOCK:			
		return TRUE;

	case TDCA_TASKNAME:
	case TDCA_PROJECTNAME:
	case TDCA_COMMENTS:		
		return FALSE;
	}

	if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		return TRUE;

	// else
	return ((nAttribID >= TDCA_FIRST_ATTRIBUTE) && (nAttribID <= TDCA_LAST_REALATTRIBUTE));
}

BOOL CToDoCtrl::ClearSelectedTaskAttribute(TDC_ATTRIBUTE nAttribID)
{
	if (!CanClearSelectedTaskAttribute(nAttribID))
		return FALSE;

	switch (nAttribID)
	{
	case TDCA_DONEDATE:		return SetSelectedTaskDate(TDCD_DONE, 0.0);
	case TDCA_DUEDATE:		return SetSelectedTaskDate(TDCD_DUE, 0.0);
	case TDCA_STARTDATE:	return SetSelectedTaskDate(TDCD_START, 0.0);
		
	case TDCA_DONETIME:		return SetSelectedTaskDate(TDCD_DONETIME, 0.0);
	case TDCA_DUETIME:		return SetSelectedTaskDate(TDCD_DUETIME, 0.0);
	case TDCA_STARTTIME:	return SetSelectedTaskDate(TDCD_STARTTIME, 0.0);
		
	case TDCA_PRIORITY:		return SetSelectedTaskPriority(FM_NOPRIORITYORRISK);
	case TDCA_RISK:			return SetSelectedTaskRisk(FM_NOPRIORITYORRISK);
		
	case TDCA_ALLOCTO:		return SetSelectedTaskAllocTo(CStringArray());
	case TDCA_CATEGORY:		return SetSelectedTaskCategories(CStringArray());
	case TDCA_TAGS:			return SetSelectedTaskTags(CStringArray());
	case TDCA_DEPENDENCY:	return SetSelectedTaskDependencies(CTDCDependencyArray());
	case TDCA_FILELINK:		return SetSelectedTaskFileLinks(CStringArray());
		
	case TDCA_ALLOCBY:		return SetSelectedTaskAllocBy(_T(""));
	case TDCA_STATUS:		return SetSelectedTaskStatus(_T(""));
	case TDCA_VERSION:		return SetSelectedTaskVersion(_T(""));
	case TDCA_EXTERNALID:	return SetSelectedTaskExternalID(_T(""));
		
	case TDCA_PERCENT:		return SetSelectedTaskPercentDone(0);
	case TDCA_FLAG:			return SetSelectedTaskFlag(FALSE);
	case TDCA_LOCK:			return SetSelectedTaskLock(FALSE);
	case TDCA_COLOR:		return SetSelectedTaskColor(CLR_NONE);
	case TDCA_RECURRENCE:	return SetSelectedTaskRecurrence(TDCRECURRENCE());
	case TDCA_ICON:			return ClearSelectedTaskIcon();
		
	case TDCA_TIMEESTIMATE:		
		{
			// preserve existing units
			TDCTIMEPERIOD time;
			VERIFY(GetSelectedTaskTimeEstimate(time)); 

			time.dAmount = 0.0;
			return SetSelectedTaskTimeEstimate(time);
		}
		break;

	case TDCA_TIMESPENT:
		{
			// preserve existing units
			TDCTIMEPERIOD time;
			VERIFY(GetSelectedTaskTimeSpent(time));

			time.dAmount = 0.0;
			return SetSelectedTaskTimeSpent(time);
		}
		break;

	case TDCA_COST:
		{ 
			// preserve 'IsRate'
			TDCCOST cost;
			VERIFY(m_ctrlAttributes.GetCost(cost));

			cost.dAmount = 0.0;
			return SetSelectedTaskCost(cost);
		}
		break;


	// These cannot be cleared
	case TDCA_SUBTASKDONE:
	case TDCA_POSITION:
	case TDCA_POSITION_SAMEPARENT:
	case TDCA_POSITION_DIFFERENTPARENT:
	case TDCA_CREATEDBY:
	case TDCA_CREATIONDATE:
	case TDCA_LASTMODDATE:
	case TDCA_LASTMODBY:
	case TDCA_ID:
	case TDCA_PARENTID:
	case TDCA_PATH:
	case TDCA_TASKNAME:
		ASSERT(0);
		return FALSE;

	case TDCA_REMINDER:		
		// Handled by WM_TDCM_CLEARTASKREMINDER
		break;

	default:
		{
			CString sCustomAttribID = m_aCustomAttribDefs.GetAttributeTypeID(nAttribID);

			if (!sCustomAttribID.IsEmpty())
				return SetSelectedTaskCustomAttributeData(sCustomAttribID, TDCCADATA());
		}
		break;
	}

	// else something we've missed
	ASSERT(0);
	return FALSE;
}

CString CToDoCtrl::FormatSelectedTaskTitles(BOOL bFullPath, TCHAR cSep, int nMaxTasks) const 
{ 
	return m_taskTree.FormatSelectedTaskTitles(bFullPath, cSep, nMaxTasks); 
}

BOOL CToDoCtrl::CanEditSelectedTask(TDC_ATTRIBUTE nAttribID) const
{
	CDWordArray aTaskIDs;
	
	// Special case: Nothing selected
	if (!GetSelectedTaskIDs(aTaskIDs, TRUE))
		return CanEditTask(0, nAttribID);
	
	// Look for first editable task
	for (int nID = 0; nID < aTaskIDs.GetSize(); nID++)
	{
		if (CanEditTask(aTaskIDs[nID], nAttribID))
			return TRUE;
	}

	return FALSE;
}

BOOL CToDoCtrl::CanEditTask(DWORD dwTaskID, TDC_ATTRIBUTE nAttribID) const
{
	if (IsReadOnly())
		return FALSE;

	// These do not depend on a specific task
	switch (nAttribID)
	{
	case TDCA_NEWTASK:
	case TDCA_PASTE:
		if (dwTaskID == 0)
			return TRUE;
		break;

	case TDCA_UNDO:
	case TDCA_CUSTOMATTRIB_DEFS:
	case TDCA_ENCRYPT:
	case TDCA_PROJECTNAME:
		return TRUE;
	}

	// Task specific editing
	BOOL bCanEdit = m_multitasker.CanEditTask(dwTaskID, nAttribID);

	if (bCanEdit != -1)
		return bCanEdit; // Handled by multi-tasker

	switch (nAttribID)
	{
	case TDCA_NEWTASK:
	case TDCA_PASTE:
		return !m_calculator.IsTaskLocked(dwTaskID);

	case TDCA_DELETE:
		// Can only delete tasks if their immediate parent is UNLOCKED
		if (!m_data.IsTaskLocked(m_data.GetTaskParentID(dwTaskID)))
		{
			// AND the task is UNLOCKED 
			if (!m_calculator.IsTaskLocked(dwTaskID))
				return TRUE;

			// OR the task is a REFERENCE to the locked task
			return m_data.IsTaskReference(dwTaskID);
		}
		break;

	case TDCA_OFFSETTASK:
		return (CanEditTask(dwTaskID, TDCA_STARTDATE) &&  // RECURSIVE CALL
				CanEditTask(dwTaskID, TDCA_DUEDATE));     // RECURSIVE CALL

	default:
		ASSERT(0); // Unexpectedly unhandled
		break;
	}

	return FALSE;
}

BOOL CToDoCtrl::CopySelectedTaskAttributeValue(TDC_ATTRIBUTE nFromAttribID, TDC_ATTRIBUTE nToAttribID)
{
	if (!m_attribCopier.CanCopyAttributeValues(nFromAttribID, nToAttribID, TRUE))
		return FALSE;

	Flush();

	IMPLEMENT_DATA_UNDO_EDIT(m_data);

	// Some attribute edits can cause completion changes
	CTDCTaskCompletionArray aTasksForCompletion(m_data, m_sCompletionStatus);

	POSITION pos = TSH().GetFirstItemPos();
	CDWordArray aModTaskIDs;

	while (pos)
	{
		DWORD dwTaskID = GetTrueTaskID(TSH().GetNextItem(pos));
		TDCCADATA dataFrom;

		if (m_data.GetTaskAttributeValue(dwTaskID, nFromAttribID, dataFrom) && 
			aTasksForCompletion.Add(dwTaskID, nToAttribID, dataFrom))
		{
			//int breakpoint = 0;
		}
		else if (!dataFrom.IsEmpty())
		{
			TODOITEM tdiFromTo;
			m_data.GetTaskAttributes(dwTaskID, tdiFromTo);
			
			if (m_attribCopier.CopyAttributeValue(tdiFromTo, nFromAttribID, tdiFromTo, nToAttribID))
				HandleModResult(dwTaskID, m_data.SetTaskAttributes(dwTaskID, tdiFromTo), aModTaskIDs);
		}
	}

	UpdateControls(FALSE); // Don't update comments

	if (aTasksForCompletion.GetSize() && SetSelectedTaskCompletion(aTasksForCompletion))
	{
		aTasksForCompletion.GetTaskIDs(aModTaskIDs, TRUE);
		SetModified(TDCA_DONEDATE, aModTaskIDs);
	}

	if (!aModTaskIDs.GetSize())
		return FALSE;

	SetModified(nToAttribID, aModTaskIDs);
	return TRUE;
}

BOOL CToDoCtrl::CopySelectedTaskAttributeValue(TDC_ATTRIBUTE nFromAttribID, const CString& sToCustomAttribID)
{
	const TDCCUSTOMATTRIBUTEDEFINITION* pToDef = NULL;
	GET_CUSTDEF_RET(m_aCustomAttribDefs, sToCustomAttribID, pToDef, FALSE);

	return CopySelectedTaskAttributeValue(nFromAttribID, pToDef->GetAttributeID());
}

BOOL CToDoCtrl::CopySelectedTaskAttributeValue(const CString& sFromCustomAttribID, TDC_ATTRIBUTE nToAttribID)
{
	const TDCCUSTOMATTRIBUTEDEFINITION* pFromDef = NULL;
	GET_CUSTDEF_RET(m_aCustomAttribDefs, sFromCustomAttribID, pFromDef, FALSE);

	return CopySelectedTaskAttributeValue(pFromDef->GetAttributeID(), nToAttribID);
}

BOOL CToDoCtrl::CopySelectedTaskAttributeValue(const CString& sFromCustomAttribID, const CString& sToCustomAttribID)
{
	// Doesn't make sense to copy to self
	if (sToCustomAttribID == sFromCustomAttribID)
	{
		ASSERT(0);
		return FALSE;
	}

	const TDCCUSTOMATTRIBUTEDEFINITION* pFromDef = NULL;
	GET_CUSTDEF_RET(m_aCustomAttribDefs, sFromCustomAttribID, pFromDef, FALSE);

	const TDCCUSTOMATTRIBUTEDEFINITION* pToDef = NULL;
	GET_CUSTDEF_RET(m_aCustomAttribDefs, sToCustomAttribID, pToDef, FALSE);

	return CopySelectedTaskAttributeValue(pFromDef->GetAttributeID(), pToDef->GetAttributeID());
}

BOOL CToDoCtrl::SaveTaskViewToImage(const CString& sFilePath) 
{ 
	CBitmap bmImage;

	if (m_taskTree.SaveToImage(bmImage))
		return (CGdiPlusBitmap(bmImage).SaveAsFile(sFilePath) != FALSE);

	// else
	return FALSE;
}

