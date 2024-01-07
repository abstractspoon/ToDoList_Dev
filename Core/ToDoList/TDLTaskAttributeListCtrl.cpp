// TDLTaskAttributeListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLTaskAttributeListCtrl.h"
#include "TDLTaskCtrlBase.h"
#include "TDCImageList.h"
#include "tdcstatic.h"
#include "tdcstruct.h"

#include "..\shared\GraphicsMisc.h"
#include "..\shared\FileMisc.h"
#include "..\shared\HoldRedraw.h"
#include "..\shared\Localizer.h"
#include "..\shared\encolordialog.h"
#include "..\shared\FileIcons.h"

#include "..\3rdParty\ColorDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

enum 
{
	ATTRIB_COL,
	VALUE_COL
};

/////////////////////////////////////////////////////////////////////////////

enum 
{
	IDC_TEXTANDNUM_COMBO = 1000,
	IDC_DATE_PICKER,
	IDC_TIME_PICKER,
	IDC_PRIORITY_COMBO,
	IDC_RISK_COMBO,
	IDC_DEPENDS_EDIT,
	IDC_PERCENT_SPIN,
	IDC_TIMEPERIOD_EDIT,
	IDC_FILELINK_COMBO,
	IDC_FILELINK_EDIT,
	IDC_CUSTOMICON_COMBO,
};

/////////////////////////////////////////////////////////////////////////////

enum 
{
	ID_BTN_TIMETRACK = 10,
	ID_BTN_ADDLOGGEDTIME,
	ID_BTN_SELECTDEPENDS,
	ID_BTN_EDITDEPENDS,
};

/////////////////////////////////////////////////////////////////////////////

const UINT IDS_PRIORITYRISK_SCALE[] = 
{ 
	IDS_TDC_SCALE0,
	IDS_TDC_SCALE1,
	IDS_TDC_SCALE2,
	IDS_TDC_SCALE3,
	IDS_TDC_SCALE4,
	IDS_TDC_SCALE5,
	IDS_TDC_SCALE6,
	IDS_TDC_SCALE7,
	IDS_TDC_SCALE8,
	IDS_TDC_SCALE9,
	IDS_TDC_SCALE10
};

/////////////////////////////////////////////////////////////////////////////

const int CUSTOMTIMEATTRIBOFFSET = (TDCA_LAST_ATTRIBUTE + 1);
const int ICON_SIZE = GraphicsMisc::ScaleByDPIFactor(16);

/////////////////////////////////////////////////////////////////////////////

static CContentMgr s_mgrContent;

/////////////////////////////////////////////////////////////////////////////

CIcon CTDLTaskAttributeListCtrl::s_iconApp;
CIcon CTDLTaskAttributeListCtrl::s_iconTrackTime;
CIcon CTDLTaskAttributeListCtrl::s_iconAddTime;
CIcon CTDLTaskAttributeListCtrl::s_iconLink;
CIcon CTDLTaskAttributeListCtrl::s_iconBrowse;

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskAttributeListCtrl

CTDLTaskAttributeListCtrl::CTDLTaskAttributeListCtrl(const CTDLTaskCtrlBase& taskCtrl,
													 const CToDoCtrlData& data,
													 const CTDCImageList& ilIcons,
													 const TDCCOLEDITVISIBILITY& defaultVis)
	:
	m_taskCtrl(taskCtrl),
	m_data(data),
	m_ilIcons(ilIcons),
	m_formatter(data, s_mgrContent),
	m_vis(defaultVis),
	m_cbTimeOfDay(TCB_HALFHOURS | TCB_NOTIME | TCB_HOURSINDAY),
	m_cbPriority(FALSE),
	m_cbRisk(FALSE),
	m_cbCustomIcons(ilIcons)
{
	// Fixed 'Dependency' buttons
	m_eDepends.SetBorderWidth(0);
	m_eDepends.SetDefaultButton(0);
	m_eDepends.AddButton(ID_BTN_SELECTDEPENDS, s_iconLink, CEnString(IDS_TDC_DEPENDSLINK_TIP));
	m_eDepends.AddButton(ID_BTN_EDITDEPENDS, _T("..."), CEnString(IDS_OPTIONS));

	m_eTimePeriod.SetBorderWidth(0);
	m_eTimePeriod.SetDefaultButton(0);

	m_eSingleFileLink.SetBorderWidth(0);
	m_eSingleFileLink.SetDefaultButton(0);

	m_cbMultiFileLink.SetButtonBorderWidth(0);
	m_cbMultiFileLink.SetDefaultButton(0);

	// static icons
	if (!s_iconTrackTime.IsValid())
	{
		s_iconTrackTime.Load(IDI_TIMETRACK, 16, FALSE);
		s_iconAddTime.Load(IDI_ADD_LOGGED_TIME, 16, FALSE);
		s_iconLink.Load(IDI_DEPENDS_LINK, 16, FALSE);
		s_iconBrowse.Load(IDI_FILEEDIT_BROWSE, 16, TRUE);
	}
}

CTDLTaskAttributeListCtrl::~CTDLTaskAttributeListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTDLTaskAttributeListCtrl, CInputListCtrl)
	//{{AFX_MSG_MAP(CTDLTaskAttributeListCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DROPFILES()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()

	ON_NOTIFY(DTN_CLOSEUP, IDC_DATE_PICKER, OnDateCloseUp)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATE_PICKER, OnDateChange)

	ON_EN_CHANGE(IDC_DEPENDS_EDIT, OnDependsChange)
	ON_EN_KILLFOCUS(IDC_TIMEPERIOD_EDIT, OnTimePeriodChange)
	ON_EN_KILLFOCUS(IDC_FILELINK_EDIT, OnSingleFileLinkChange)

	ON_CONTROL_RANGE(CBN_KILLFOCUS, 0, 0xffff, OnComboKillFocus)
	ON_CONTROL_RANGE(CBN_CLOSEUP, 0, 0xffff, OnComboCloseUp)
	ON_CONTROL_RANGE(CBN_SELCHANGE, 0, 0xffff, OnComboEditChange)

	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnTextEditOK)

	ON_REGISTERED_MESSAGE(WM_ACBN_ITEMADDED, OnAutoComboAddDelete)
	ON_REGISTERED_MESSAGE(WM_ACBN_ITEMDELETED, OnAutoComboAddDelete)

	ON_REGISTERED_MESSAGE(WM_EE_BTNCLICK, OnEnEditButtonClick)
	ON_REGISTERED_MESSAGE(WM_FE_DISPLAYFILE, OnFileLinkDisplay)
	ON_REGISTERED_MESSAGE(WM_FE_GETFILEICON, OnFileLinkWantIcon)
	ON_REGISTERED_MESSAGE(WM_FE_GETFILETOOLTIP, OnFileLinkWantTooltip)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskAttributeListCtrl message handlers

int CTDLTaskAttributeListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CInputListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetView(LVS_REPORT);
	EnableSorting(TRUE);
	ShowGrid(TRUE, TRUE);
	
	// Add columns
	AddCol(_T("Attribute"));
	AddCol(_T("Value"));

	// Prevent ellipses
	SetColumnFormat(VALUE_COL, ES_NONE);

	// Add attributes
	Populate();

	// Create our edit fields
	CreateControl(m_cbTextAndNumbers, IDC_TEXTANDNUM_COMBO, (CBS_DROPDOWN | CBS_SORT | CBS_AUTOHSCROLL));
	m_cbTextAndNumbers.ModifyFlags(0, ACBS_ALLOWDELETE | ACBS_AUTOCOMPLETE);

	CreateControl(m_datePicker, IDC_DATE_PICKER);
	CreateControl(m_cbTimeOfDay, IDC_TIME_PICKER, (CBS_DROPDOWN | CBS_AUTOHSCROLL));
	CreateControl(m_cbPriority, IDC_PRIORITY_COMBO, CBS_DROPDOWNLIST);
	CreateControl(m_cbRisk, IDC_RISK_COMBO, CBS_DROPDOWNLIST);
	CreateControl(m_eDepends, IDC_DEPENDS_EDIT, ES_AUTOHSCROLL);
	CreateControl(m_eTimePeriod, IDC_TIMEPERIOD_EDIT, ES_AUTOHSCROLL);
	CreateControl(m_cbMultiFileLink, IDC_FILELINK_COMBO, (CBS_DROPDOWN | CBS_AUTOHSCROLL));
	CreateControl(m_eSingleFileLink, IDC_FILELINK_EDIT, ES_AUTOHSCROLL);
	CreateControl(m_cbCustomIcons, IDC_CUSTOMICON_COMBO, CBS_DROPDOWNLIST);

	VERIFY(m_spinPercent.Create(WS_CHILD | UDS_SETBUDDYINT | UDS_ARROWKEYS| UDS_ALIGNRIGHT, CRect(0, 0, 0, 0), this, IDC_PERCENT_SPIN));
	m_spinPercent.SetRange(0, 100);

	CLocalizer::EnableTranslation(m_cbTextAndNumbers, FALSE);
	CLocalizer::EnableTranslation(m_cbPriority, FALSE);
	CLocalizer::EnableTranslation(m_cbRisk, FALSE);
	CLocalizer::EnableTranslation(m_cbMultiFileLink, FALSE);

	return 0;
}

void CTDLTaskAttributeListCtrl::RedrawValue(TDC_ATTRIBUTE nAttribID)
{
	int nRow = GetRow(nAttribID);

	if (nRow != -1)
		RedrawCell(nRow, VALUE_COL, FALSE);
}

void CTDLTaskAttributeListCtrl::RefreshCompletionStatus()
{
	CString sStatus = m_taskCtrl.GetCompletionStatus();

	if (!sStatus.IsEmpty())
		Misc::AddUniqueItem(sStatus, m_tldDefault.aStatus);
	else
		Misc::RemoveItem(sStatus, m_tldDefault.aStatus);
}

void CTDLTaskAttributeListCtrl::RefreshDateTimeFormatting()
{
	int nRow = GetItemCount();

	while (nRow--)
	{
		switch (GetAttributeID(nRow))
		{
		case TDCA_DONEDATE:
		case TDCA_DUEDATE:
		case TDCA_STARTDATE:
		case TDCA_DONETIME:
		case TDCA_DUETIME:
		case TDCA_STARTTIME:
			RefreshSelectedTaskValue(nRow);
			break;
		}
	}
}

void CTDLTaskAttributeListCtrl::SetPercentDoneIncrement(int nAmount)
{
	ASSERT(m_spinPercent.GetSafeHwnd());
	
	UDACCEL uda = { 0, (UINT)nAmount };
	m_spinPercent.SetAccel(1, &uda);
}

void CTDLTaskAttributeListCtrl::SetCustomAttributeDefinitions(const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	if (Misc::MatchAllT(m_aCustomAttribDefs, aAttribDefs, FALSE))
		return;

	m_aCustomAttribDefs.Copy(aAttribDefs);
	Populate();
}

void CTDLTaskAttributeListCtrl::SetAttributeVisibility(const TDCCOLEDITVISIBILITY& vis)
{
	BOOL bColChange = FALSE, bEditChange = FALSE;
	BOOL bChange = m_vis.HasDifferences(vis, bColChange, bEditChange);

	if (!bChange)
		return;

	m_vis = vis;

	if (bEditChange || (bColChange && (vis.GetShowFields() == TDLSA_ASCOLUMN)))
		Populate();
}

void CTDLTaskAttributeListCtrl::CheckAddAttribute(TDC_ATTRIBUTE nAttribID, UINT nAttribResID)
{
	BOOL bAdd = (m_vis.IsEditFieldVisible(nAttribID) && (nAttribID != TDCA_PROJECTNAME));

	if (!bAdd)
	{
		bAdd = ((m_vis.GetShowFields() == TDLSA_ASCOLUMN) &&
				m_vis.IsColumnVisible(TDC::MapAttributeToColumn(nAttribID)));

		if (!bAdd)
		{
			bAdd = ((nAttribID == TDCA_ICON) && 
					m_data.HasStyle(TDCS_ALLOWTREEITEMCHECKBOX));
		}
	}

	if (bAdd)
	{
		int nItem = AddRow(CEnString(nAttribResID));
		SetItemData(nItem, nAttribID);
	}
}

void CTDLTaskAttributeListCtrl::Populate()
{
	CHoldRedraw hr(*this);

	// Preserve current selection
	int nSelRow, nSelCol;
	TDC_ATTRIBUTE nSelAttribID = TDCA_NONE;
	
	if (GetCurSel(nSelRow, nSelCol))
		nSelAttribID = GetAttributeID(nSelRow);

	DeleteAllItems();

	for (int nAttrib = 1; nAttrib < ATTRIB_COUNT; nAttrib++)
		CheckAddAttribute(ATTRIBUTES[nAttrib].nAttribID, ATTRIBUTES[nAttrib].nAttribResID);

	// Dependent time fields
	CheckAddAttribute(TDCA_STARTTIME, IDS_TDLBC_STARTTIME);
	CheckAddAttribute(TDCA_DUETIME, IDS_TDLBC_DUETIME);
	CheckAddAttribute(TDCA_DONETIME, IDS_TDLBC_DONETIME);

	// Custom attributes
	for (int nCust = 0; nCust < m_aCustomAttribDefs.GetSize(); nCust++)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = m_aCustomAttribDefs[nCust];

		if (attribDef.bEnabled)
		{
			int nItem = AddRow(CEnString(IDS_CUSTOMCOLUMN, attribDef.sLabel));
			SetItemData(nItem, attribDef.GetAttributeID());

			if (attribDef.IsDataType(TDCCA_DATE) && attribDef.HasFeature(TDCCAF_SHOWTIME))
			{
				int nItem = AddRow(CEnString(IDS_CUSTOMCOLUMN, attribDef.sLabel));
				SetItemData(nItem, MapCustomDateToTime(attribDef.GetAttributeID()));
			}
		}
	}

	RefreshSelectedTaskValues();
	Sort();

	// Restore previous selection
	if (nSelAttribID != TDCA_NONE)
		SetCurSel(GetRow(nSelAttribID), nSelCol);
}

void CTDLTaskAttributeListCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CInputListCtrl::OnSize(nType, cx, cy);
	
	if (GetColumnCount())
	{
		SetColumnWidth(ATTRIB_COL, (cx / 2));
		SetColumnWidth(VALUE_COL, (cx / 2) - 1);
	}
}

void CTDLTaskAttributeListCtrl::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	
	CInputListCtrl::OnDropFiles(hDropInfo);
}

BOOL CTDLTaskAttributeListCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CInputListCtrl::OnEraseBkgnd(pDC);
}

BOOL CTDLTaskAttributeListCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CInputListCtrl::OnSetCursor(pWnd, nHitTest, message);
}


TDC_ATTRIBUTE CTDLTaskAttributeListCtrl::GetAttributeID(int nRow, BOOL bResolveCustomTimeFields) const
{ 
	if (nRow == -1)
		return TDCA_NONE;

	TDC_ATTRIBUTE nAttribID = (TDC_ATTRIBUTE)GetItemData(nRow); 

	if (bResolveCustomTimeFields && IsCustomTime(nAttribID))
		nAttribID = MapCustomTimeToDate(nAttribID);

	return nAttribID;
}

IL_COLUMNTYPE CTDLTaskAttributeListCtrl::GetCellType(int nRow, int nCol) const
{
	if (nCol == ATTRIB_COL)
		return ILCT_TEXT;

	// else
	TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);

	switch (nAttribID)
	{
	case TDCA_TASKNAME:
	case TDCA_COST:
	case TDCA_EXTERNALID:
	case TDCA_PERCENT:
		return ILCT_TEXT;

	case TDCA_TIMEESTIMATE:
	case TDCA_TIMESPENT:
		return ILCT_POPUPMENU;

	case TDCA_DONEDATE:
	case TDCA_DUEDATE:
	case TDCA_STARTDATE:
		return ILCT_DATE;

	case TDCA_PRIORITY:
	case TDCA_ALLOCTO:
	case TDCA_ALLOCBY:
	case TDCA_STATUS:
	case TDCA_CATEGORY:
	case TDCA_TAGS:
	case TDCA_RISK:
	case TDCA_VERSION:
	case TDCA_DONETIME:
	case TDCA_DUETIME:
	case TDCA_STARTTIME:
		return ILCT_DROPLIST;

	case TDCA_FILELINK:
		return (GetItemText(nRow, nCol).IsEmpty() ? ILCT_CUSTOMBTN : ILCT_DROPLIST);

	case TDCA_ICON:
	case TDCA_RECURRENCE:
	case TDCA_DEPENDENCY:
	case TDCA_COLOR:
		return ILCT_BROWSE;

	case TDCA_FLAG:
	case TDCA_LOCK:
		return ILCT_CHECK;

	case TDCA_CREATEDBY:
	case TDCA_PATH:
	case TDCA_POSITION:
	case TDCA_CREATIONDATE:
	case TDCA_LASTMODDATE:
	case TDCA_COMMENTSSIZE:
	case TDCA_COMMENTSFORMAT:
	case TDCA_SUBTASKDONE:
	case TDCA_LASTMODBY:
	case TDCA_ID:
	case TDCA_PARENTID:
		return ILCT_TEXT;

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		{
			const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
			GET_CUSTDEF_RET(m_aCustomAttribDefs, nAttribID, pDef, ILCT_TEXT);

			if (pDef->IsList())
				return ILCT_DROPLIST;

			// else
			switch (pDef->GetDataType())
			{
			case TDCCA_STRING:
			case TDCCA_FRACTION:
			case TDCCA_INTEGER:
			case TDCCA_DOUBLE:
			case TDCCA_CALCULATION:
			case TDCCA_TIMEPERIOD:
				return ILCT_TEXT;

			case TDCCA_DATE:
				return ILCT_DATE;

			case TDCCA_BOOL:
				return ILCT_CHECK;

			case TDCCA_ICON:
			case TDCCA_FILELINK:
				return ILCT_BROWSE;
			}
		}
		else if (IsCustomTime(nAttribID))
		{
			return ILCT_DROPLIST;
		}
		break;
	}

	// All else
	ASSERT(0);
	return ILCT_TEXT;
}

BOOL CTDLTaskAttributeListCtrl::CanEditCell(int nRow, int nCol) const
{
	if (!IsWindowEnabled() || (nCol == ATTRIB_COL) || m_data.HasStyle(TDCS_READONLY))
		return FALSE;

	if (m_taskCtrl.SelectionHasLocked())
		return FALSE;

	// else
	TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);

	switch (nAttribID)
	{
	case TDCA_CREATEDBY:
	case TDCA_PATH:
	case TDCA_POSITION:
	case TDCA_CREATIONDATE:
	case TDCA_LASTMODDATE:
	case TDCA_COMMENTSSIZE:
	case TDCA_COMMENTSFORMAT:
	case TDCA_SUBTASKDONE:
	case TDCA_LASTMODBY:
	case TDCA_ID:
	case TDCA_PARENTID:
		// Permanently read-only fields
		return FALSE;

	case TDCA_PERCENT:
		{
			if (m_data.HasStyle(TDCS_AUTOCALCPERCENTDONE))
				return FALSE;

			if (m_taskCtrl.GetSelectedCount() > 1)
			{
				if (m_data.HasStyle(TDCS_AVERAGEPERCENTSUBCOMPLETION) && m_taskCtrl.SelectionHasParents())
					return FALSE;
			}
		}
		break;

	case TDCA_LOCK:
		return TRUE;

	case TDCA_STARTTIME:
		return m_taskCtrl.SelectedTaskHasDate(TDCD_STARTDATE);

	case TDCA_DUETIME:
		return m_taskCtrl.SelectedTaskHasDate(TDCD_DUEDATE);

	case TDCA_DONETIME:
		return m_taskCtrl.SelectedTaskHasDate(TDCD_DONEDATE);

	case TDCA_TIMEESTIMATE:
	case TDCA_TIMESPENT:
		return (m_data.HasStyle(TDCS_ALLOWPARENTTIMETRACKING) || !m_taskCtrl.SelectionHasParents());

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		{
			const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
			GET_CUSTDEF_RET(m_aCustomAttribDefs, nAttribID, pDef, FALSE);

			return (pDef->IsList() || !pDef->IsDataType(TDCCA_CALCULATION));
		}
		else if (IsCustomTime(nAttribID))
		{
			TDC_ATTRIBUTE nDateAttribID = GetAttributeID(nRow, TRUE);

			return !GetValueText(nDateAttribID).IsEmpty();
		}
		break;
	}
	
	return TRUE;
}

COLORREF CTDLTaskAttributeListCtrl::GetItemBackColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const
{
	if (nCol == VALUE_COL)
	{
		if (!CanEditCell(nItem, nCol))
			return GetSysColor(COLOR_3DFACE);
	}

	// All else
	return CInputListCtrl::GetItemBackColor(nItem, nCol, bSelected, bDropHighlighted, bWndFocus);
}

COLORREF CTDLTaskAttributeListCtrl::GetItemTextColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const
{
	if (nCol == VALUE_COL)
	{
		if (!CanEditCell(nItem, nCol))
			return GetSysColor(COLOR_GRAYTEXT);

		switch (GetAttributeID(nItem))
		{
		case TDCA_DEPENDENCY:
			if (m_taskCtrl.SelectionHasCircularDependencies())
				return colorRed;
			break;
		}
	}

	// All else
	return CInputListCtrl::GetItemTextColor(nItem, nCol, bSelected, bDropHighlighted, bWndFocus);
}

void CTDLTaskAttributeListCtrl::SetDefaultAutoListData(const TDCAUTOLISTDATA& tldDefault) 
{ 
	m_tldAll.RemoveItems(m_tldDefault, TDCA_ALL);
	m_tldAll.AppendUnique(tldDefault, TDCA_ALL);

	m_tldDefault.Copy(tldDefault, TDCA_ALL);
}

void CTDLTaskAttributeListCtrl::SetAutoListData(const TDCAUTOLISTDATA& tld, TDC_ATTRIBUTE nAttribID)
{
	m_tldAll.Copy(tld, nAttribID);
}

void CTDLTaskAttributeListCtrl::GetAutoListData(TDCAUTOLISTDATA& tld, TDC_ATTRIBUTE nAttribID) const
{
	tld.Copy(m_tldAll, nAttribID);
}

void CTDLTaskAttributeListCtrl::RefreshSelectedTaskValues(BOOL bForceClear)
{
	CHoldRedraw hr(*this);
	HideAllControls();

	int nRow = GetItemCount();

	while (nRow--)
	{
		if (bForceClear)
			SetItemText(nRow, VALUE_COL, _T(""));
		else
			RefreshSelectedTaskValue(nRow);
	}
}

void CTDLTaskAttributeListCtrl::RefreshSelectedTaskValue(TDC_ATTRIBUTE nAttribID)
{
	int nRow = GetRow(nAttribID);
	ASSERT(nRow != -1);

	RefreshSelectedTaskValue(nRow);
}

void CTDLTaskAttributeListCtrl::RefreshSelectedTaskValue(int nRow)
{
	CString sValue;
	CStringArray aMatched, aMixed;

	DWORD dwSingleSelTaskID = ((m_taskCtrl.GetSelectedCount() == 1) ? m_taskCtrl.GetSelectedTaskID() : 0);
	TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);

	switch (nAttribID)
	{
	case TDCA_EXTERNALID:	sValue = m_taskCtrl.GetSelectedTaskExtID();		break;
	case TDCA_ALLOCBY:		sValue = m_taskCtrl.GetSelectedTaskAllocBy();	break;
	case TDCA_STATUS:		sValue = m_taskCtrl.GetSelectedTaskStatus();	break;
	case TDCA_VERSION:		sValue = m_taskCtrl.GetSelectedTaskVersion();	break;
	case TDCA_ICON:			sValue = m_taskCtrl.GetSelectedTaskIcon();		break;

	case TDCA_FLAG:			sValue = m_taskCtrl.IsSelectedTaskFlagged() ? _T("+") : _T(""); break;
	case TDCA_LOCK:			sValue = m_taskCtrl.IsSelectedTaskLocked() ? _T("+") : _T("");	break;

	case TDCA_ALLOCTO:		m_taskCtrl.GetSelectedTaskAllocTo(aMatched, aMixed);	break;
	case TDCA_CATEGORY:		m_taskCtrl.GetSelectedTaskCategories(aMatched, aMixed); break;
	case TDCA_TAGS:			m_taskCtrl.GetSelectedTaskTags(aMatched, aMixed);		break;
	case TDCA_FILELINK:		m_taskCtrl.GetSelectedTaskFileLinks(aMatched, TRUE);	break;

	case TDCA_COST:
		{
			TDCCOST cost;

			if (m_taskCtrl.GetSelectedTaskCost(cost))
				sValue = cost.Format(2);
		}
		break;

	case TDCA_PERCENT:
		if (m_taskCtrl.IsSelectedTaskDone())
		{
			sValue = _T("100");
		}
		else if (dwSingleSelTaskID && m_data.HasStyle(TDCS_AUTOCALCPERCENTDONE))
		{
			sValue = m_formatter.GetTaskPercentDone(dwSingleSelTaskID);
		}
		else
		{
			int nValue = m_taskCtrl.GetSelectedTaskPercent();

			if (nValue != -1)
				sValue = Misc::Format(nValue);
		}
		break;

	case TDCA_PRIORITY:
		sValue = Misc::Format(m_taskCtrl.GetSelectedTaskPriority());
		break;

	case TDCA_RISK:
		sValue = Misc::Format(m_taskCtrl.GetSelectedTaskRisk());
		break;

	case TDCA_TIMEESTIMATE:
		if (m_data.HasStyle(TDCS_ALLOWPARENTTIMETRACKING) || !m_taskCtrl.SelectionHasParents())
		{
			TDCTIMEPERIOD tp;

			if (m_taskCtrl.GetSelectedTaskTimeEstimate(tp))
				sValue = tp.Format(2);
		}
		else if (dwSingleSelTaskID)
		{
			sValue = m_formatter.GetTaskTimeEstimate(dwSingleSelTaskID);
		}
		break;

	case TDCA_TIMESPENT:
		if (m_data.HasStyle(TDCS_ALLOWPARENTTIMETRACKING) || !m_taskCtrl.SelectionHasParents())
		{
			TDCTIMEPERIOD tp;

			if (m_taskCtrl.GetSelectedTaskTimeSpent(tp))
				sValue = tp.Format(2);
		}
		else if (dwSingleSelTaskID)
		{
			sValue = m_formatter.GetTaskTimeEstimate(dwSingleSelTaskID);
		}
		break;

	case TDCA_DONEDATE:
	case TDCA_DUEDATE:
	case TDCA_STARTDATE:
		{
			COleDateTime date = m_taskCtrl.GetSelectedTaskDate(TDC::MapAttributeToDate(nAttribID));
			sValue = CDateHelper::FormatDate(date, (m_data.HasStyle(TDCS_SHOWDATESINISO) ? DHFD_ISO : 0));
		}
		break;

	case TDCA_DONETIME:
	case TDCA_DUETIME:
	case TDCA_STARTTIME:
		{
			COleDateTime date = m_taskCtrl.GetSelectedTaskDate(TDC::MapAttributeToDate(nAttribID));

			if (CDateHelper::DateHasTime(date))
				sValue = CTimeHelper::FormatClockTime(date, FALSE, m_data.HasStyle(TDCS_SHOWDATESINISO));
		}
		break;

	case TDCA_LASTMODDATE:
	case TDCA_CREATIONDATE:
		{
			COleDateTime date = m_taskCtrl.GetSelectedTaskDate(TDC::MapAttributeToDate(nAttribID));
			sValue = CDateHelper::FormatDate(date, DHFD_TIME);
		}
		break;

	case TDCA_COLOR:
		sValue = Misc::Format(m_taskCtrl.GetSelectedTaskColor());
		break;

	case TDCA_RECURRENCE:
		{
			TDCRECURRENCE recurs;

			if (m_taskCtrl.GetSelectedTaskRecurrence(recurs))
				sValue = recurs.GetRegularityText();
		}
		break;

	case TDCA_DEPENDENCY:
		{
			CTDCDependencyArray aDepends;

			if (m_taskCtrl.GetSelectedTaskDependencies(aDepends))
				sValue = aDepends.Format();
		}
		break;

	case TDCA_POSITION:
		if (dwSingleSelTaskID)
			sValue = m_formatter.GetTaskPosition(dwSingleSelTaskID);
		break;


	case TDCA_ID:
		if (dwSingleSelTaskID)
			sValue = Misc::Format(dwSingleSelTaskID);
		break;

	case TDCA_PARENTID:
		{
			DWORD dwID = m_taskCtrl.GetSelectedTaskParentID();

			if (dwID)
				sValue = Misc::Format(dwID);
		}
		break;

	case TDCA_PATH:
		if (dwSingleSelTaskID)
		{
			sValue = m_formatter.GetTaskPath(dwSingleSelTaskID);
		}
		else
		{
			DWORD dwID = m_taskCtrl.GetSelectedTaskParentID();

			if (dwID)
				sValue = m_formatter.GetTaskPath(m_taskCtrl.GetSelectedTaskID());
		}
		break;

	case TDCA_TASKNAME:			break; // TODO
	case TDCA_CREATEDBY:		break; // TODO
	case TDCA_COMMENTSSIZE:		break; // TODO
	case TDCA_COMMENTSFORMAT:	break; // TODO
	case TDCA_SUBTASKDONE:		break; // TODO
	case TDCA_LASTMODBY:		break; // TODO

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		{
			const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
			GET_CUSTDEF_ALT(m_aCustomAttribDefs, nAttribID, pDef, break);

			CString sAttribID = pDef->sUniqueID;
			TDCCADATA data;

			if (m_taskCtrl.GetSelectedTaskCustomAttributeData(sAttribID, data))
			{
				if (pDef->IsMultiList())
				{
					data.AsArrays(aMatched, aMixed);
				}
				else if (pDef->IsList())
				{
					sValue = data.FormatAsArray();
				}
				else
				{
					switch (pDef->GetDataType())
					{
					case TDCCA_STRING:
					case TDCCA_FRACTION:
					case TDCCA_INTEGER:
					case TDCCA_DOUBLE:
					case TDCCA_ICON:
					case TDCCA_FILELINK:
						sValue = data.AsString();
						break;

					case TDCCA_CALCULATION:
						// TODO
						break;

					case TDCCA_TIMEPERIOD:
						sValue = data.FormatAsTimePeriod();
						break;

					case TDCCA_DATE:
						sValue = data.FormatAsDate(m_data.HasStyle(TDCS_SHOWDATESINISO), FALSE);
						break;

					case TDCCA_BOOL:
						sValue = (data.AsBool() ? _T("+") : _T(""));
						break;
					}
				}
			}
		}
		else if (IsCustomTime(nAttribID))
		{
			CString sAttribID = m_aCustomAttribDefs.GetAttributeTypeID(MapCustomTimeToDate(nAttribID));
			TDCCADATA data;

			if (m_taskCtrl.GetSelectedTaskCustomAttributeData(sAttribID, data))
			{
				ASSERT(m_aCustomAttribDefs.GetAttributeDataType(sAttribID) == TDCCA_DATE);

				sValue = CTimeHelper::FormatClockTime(data.AsDate(), FALSE, m_data.HasStyle(TDCS_SHOWDATESINISO));
			}
		}
		break;
	}

	if (aMatched.GetSize() || aMixed.GetSize())
		sValue = FormatMultiSelItems(aMatched, aMixed);

	SetItemText(nRow, VALUE_COL, sValue);
}

BOOL CTDLTaskAttributeListCtrl::GetButtonRect(int nRow, int nCol, CRect& rButton) const
{
	if (!CInputListCtrl::GetButtonRect(nRow, nCol, rButton))
		return FALSE;

	// 'File Link' browse icon button
	TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);

	switch (nAttribID)
	{
	case TDCA_FILELINK:
		if (GetItemText(nRow, nCol).IsEmpty())
			rButton.left -= 2; // To compensate for the inflation when drawing
		break;

	default:
		// Custom File Link Attributes
		// TODO
		break;
	}

	return TRUE;

}

BOOL CTDLTaskAttributeListCtrl::DrawButton(CDC* pDC, int nRow, int nCol, CRect& rButton, BOOL bHasText, BOOL bSelected)
{
	if (!CInputListCtrl::DrawButton(pDC, nRow, nCol, rButton, bHasText, bSelected))
		return FALSE;

	// Draw 'File Link' browse icon
	TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);

	switch (nAttribID)
	{
	case TDCA_FILELINK:
		if (!bHasText)
		{
			CRect rIcon(0, 0, ICON_SIZE, ICON_SIZE);
			GraphicsMisc::CentreRect(rIcon, rButton);

			s_iconBrowse.Draw(pDC, rIcon.TopLeft());
		}
		break;

	default:
		// Custom File Link Attributes
		// TODO
		break;
	}

	return TRUE;
}

void CTDLTaskAttributeListCtrl::DrawCellText(CDC* pDC, int nRow, int nCol, const CRect& rText, const CString& sText, COLORREF crText, UINT nDrawTextFlags)
{
	// Only draw what we really need to
	if (nCol == VALUE_COL)
	{
		TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);

		switch (nAttribID)
		{
		case TDCA_PRIORITY:
			{
				int nPriority = _ttoi(sText);
				
				if (nPriority >= 0)
				{
					// Draw box
					CRect rBox(rText);
					rBox.DeflateRect(0, 3);
					rBox.right = rBox.left + rBox.Height();

					COLORREF crFill = m_taskCtrl.GetPriorityColor(nPriority);
					COLORREF crBorder = GraphicsMisc::Darker(crFill, 0.5);

					GraphicsMisc::DrawRect(pDC, rBox, crFill, crBorder);

					// Draw text
					CRect rLeft(rText);
					rLeft.left += rText.Height();

					CString sPriority = sText + Misc::Format(_T(" (%s)"), CEnString(IDS_PRIORITYRISK_SCALE[nPriority]));
					CInputListCtrl::DrawCellText(pDC, nRow, nCol, rLeft, sPriority, crText, nDrawTextFlags);
				}
			}
			return;

		case TDCA_RISK:
			{
				int nRisk = _ttoi(sText);
				
				if (nRisk >= 0)
				{
					CString sPriority = sText + Misc::Format(_T(" (%s)"), CEnString(IDS_PRIORITYRISK_SCALE[nRisk]));
					CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, sPriority, crText, nDrawTextFlags);
				}
			}
			return;

		case TDCA_ICON:
			DrawIcon(pDC, sText, rText, FALSE);
			return;

		case TDCA_COLOR:
			{
				crText = _ttoi(sText);

				if (crText != CLR_NONE)
				{
					if (m_data.HasStyle(TDCS_TASKCOLORISBACKGROUND))
					{
						// Use the entire cell rect for the background colour
						CRect rCell;
						GetCellRect(nRow, nCol, rCell);

						pDC->FillSolidRect(rCell, crText);
						crText = GraphicsMisc::GetBestTextColor(crText);
					}

					CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, CEnString(IDS_COLOR_SAMPLETEXT), crText, nDrawTextFlags);
				}
			}
			return;

		case TDCA_ALLOCTO:
		case TDCA_CATEGORY:
		case TDCA_TAGS:
			{
				int nMixed = sText.Find('|');

				if (nMixed != -1)
				{
					CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, sText.Left(nMixed), crText, nDrawTextFlags);
					return;
				}
				// else do default
			}
			break;

		case TDCA_FILELINK:
			if (!sText.IsEmpty())
			{
				CStringArray aFiles;
				int nNumFiles = Misc::Split(sText, aFiles);

				CRect rFile(rText);

				for (int nFile = 0; nFile < nNumFiles; nFile++)
				{
					if (DrawIcon(pDC, aFiles[nFile], rFile, TRUE))
						rFile.left += (ICON_SIZE + 2);
				}

				// Only render the link text if there is a single link
				if (nNumFiles == 1)
				{
					CString sFile(aFiles[0]);

					if (!TDCTASKLINK::IsTaskLink(aFiles[0], TRUE))
						sFile = FileMisc::GetFileNameFromPath(sFile);

					CInputListCtrl::DrawCellText(pDC, nRow, nCol, rFile, sFile, crText, nDrawTextFlags);
				}
			}
			return;

		default:
			if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
			{
				const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
				GET_CUSTDEF_ALT(m_aCustomAttribDefs, nAttribID, pDef, break);

				switch (pDef->GetDataType())
				{
				case TDCCA_ICON:
					if (pDef->IsMultiList())
					{
						CString sMatched(sText), sUnused;
						Misc::Split(sMatched, sUnused, '|');

						CStringArray aIcons;
						int nNumIcons = Misc::Split(sMatched, aIcons);

						CRect rIcon(rText);

						for (int nIcon = 0; nIcon < nNumIcons; nIcon++)
						{
							if (DrawIcon(pDC, aIcons[nIcon], rIcon, FALSE))
								rIcon.left += (ICON_SIZE + 2);
						}
					}
					else
					{
						DrawIcon(pDC, sText, rText, FALSE);
					}
					return;

				default:
					if (pDef->IsMultiList())
					{
						CString sMatched(sText), sUnused;
						Misc::Split(sMatched, sUnused, '|');

						CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, sText, crText, nDrawTextFlags);
						return;
					}
					break;
				}
			}
			break;
		}
	}

	// All else
	CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, sText, crText, nDrawTextFlags);
}

CPoint CTDLTaskAttributeListCtrl::GetIconPos(const CRect& rText)
{
	return CPoint(rText.left - 1, rText.top + ((rText.Height() - ICON_SIZE) / 2));
}

BOOL CTDLTaskAttributeListCtrl::DrawIcon(CDC* pDC, const CString& sIcon, const CRect& rText, BOOL bIconIsFile)
{
	if (sIcon.IsEmpty())
		return FALSE;

	CPoint ptIcon(GetIconPos(rText));

	if (bIconIsFile)
	{
		HICON hIcon = (HICON)OnFileLinkWantIcon(0, (LPARAM)(LPCTSTR)sIcon);

		if (hIcon == NULL)
			return CFileIcons::Draw(pDC, FileMisc::GetExtension(sIcon), ptIcon);

		// else
		return ::DrawIconEx(*pDC, ptIcon.x, ptIcon.y, hIcon, ICON_SIZE, ICON_SIZE, 0, NULL, DI_NORMAL);
	}

	// else
	return m_ilIcons.Draw(pDC, sIcon, GetIconPos(rText), ILD_TRANSPARENT);
}

void CTDLTaskAttributeListCtrl::OnTextEditOK(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ((pDispInfo->item.iSubItem == VALUE_COL) &&
		(pDispInfo->item.iItem >= 0))
	{
		TDC_ATTRIBUTE nAttribID = GetAttributeID(pDispInfo->item.iItem, TRUE);
		
		NotifyParentEdit(nAttribID);
		RefreshSelectedTaskValue(nAttribID);
	}

	*pResult = 0;
}

BOOL CTDLTaskAttributeListCtrl::GetTimeEstimate(TDCTIMEPERIOD& timeEst) const
{
	return timeEst.Parse(GetValueText(TDCA_TIMEESTIMATE));
}

BOOL CTDLTaskAttributeListCtrl::GetTimeSpent(TDCTIMEPERIOD& timeSpent) const
{
	return timeSpent.Parse(GetValueText(TDCA_TIMESPENT));
}

int CTDLTaskAttributeListCtrl::GetAllocTo(CStringArray& aMatched, CStringArray& aMixed) const
{
	return ParseMultiSelValues(GetValueText(TDCA_ALLOCTO), aMatched, aMixed);
}

CString CTDLTaskAttributeListCtrl::GetAllocBy() const
{
	return GetValueText(TDCA_ALLOCBY);
}

CString CTDLTaskAttributeListCtrl::GetStatus() const
{
	return GetValueText(TDCA_STATUS);
}

int CTDLTaskAttributeListCtrl::GetCategories(CStringArray& aMatched, CStringArray& aMixed) const
{
	return ParseMultiSelValues(GetValueText(TDCA_CATEGORY), aMatched, aMixed);
}

int CTDLTaskAttributeListCtrl::GetDependencies(CTDCDependencyArray& aDepends) const
{
	return m_eDepends.GetDependencies(aDepends);
}

int CTDLTaskAttributeListCtrl::GetTags(CStringArray& aMatched, CStringArray& aMixed) const
{
	return ParseMultiSelValues(GetValueText(TDCA_TAGS), aMatched, aMixed);
}

int CTDLTaskAttributeListCtrl::GetFileLinks(CStringArray& aFiles) const
{
	return Misc::Split(GetValueText(TDCA_FILELINK), aFiles);
}

CString CTDLTaskAttributeListCtrl::GetExternalID() const
{
	return GetValueText(TDCA_EXTERNALID);
}

int CTDLTaskAttributeListCtrl::GetPercent() const
{
	return _ttoi(GetValueText(TDCA_PERCENT));
}

int CTDLTaskAttributeListCtrl::GetPriority() const
{
	CString sValue = GetValueText(TDCA_PRIORITY);

	return (sValue.IsEmpty() ? TDC_NOPRIORITYORISK : _ttoi(sValue));
}

int CTDLTaskAttributeListCtrl::GetRisk() const
{
	CString sValue = GetValueText(TDCA_RISK);

	return (sValue.IsEmpty() ? TDC_NOPRIORITYORISK : _ttoi(sValue));
}

BOOL CTDLTaskAttributeListCtrl::GetCost(TDCCOST& cost) const
{
	return cost.Parse(GetValueText(TDCA_COST));
}

BOOL CTDLTaskAttributeListCtrl::GetFlag() const
{
	return !GetValueText(TDCA_FLAG).IsEmpty();
}

BOOL CTDLTaskAttributeListCtrl::GetLock() const
{
	return !GetValueText(TDCA_LOCK).IsEmpty();
}

CString CTDLTaskAttributeListCtrl::GetVersion() const
{
	return GetValueText(TDCA_VERSION);
}

COleDateTime CTDLTaskAttributeListCtrl::GetStartDate() const
{
	COleDateTime date;
	CDateHelper::DecodeDate(GetValueText(TDCA_STARTDATE), date, FALSE);

	return date;
}

COleDateTime CTDLTaskAttributeListCtrl::GetDueDate() const
{
	COleDateTime date;
	CDateHelper::DecodeDate(GetValueText(TDCA_DUEDATE), date, FALSE);

	return date;
}

COleDateTime CTDLTaskAttributeListCtrl::GetDoneDate() const
{
	COleDateTime date;
	CDateHelper::DecodeDate(GetValueText(TDCA_DONEDATE), date, FALSE);

	return date;
}

COleDateTime CTDLTaskAttributeListCtrl::GetStartTime() const
{
	return (CTimeHelper::DecodeClockTime(GetValueText(TDCA_STARTTIME)) / 24);
}

COleDateTime CTDLTaskAttributeListCtrl::GetDueTime() const
{
	return (CTimeHelper::DecodeClockTime(GetValueText(TDCA_DUETIME)) / 24);
}

COleDateTime CTDLTaskAttributeListCtrl::GetDoneTime() const
{
	return (CTimeHelper::DecodeClockTime(GetValueText(TDCA_DONETIME)) / 24);
}

BOOL CTDLTaskAttributeListCtrl::GetCustomAttributeData(const CString& sAttribID, TDCCADATA& data) const
{
	const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
	GET_CUSTDEF_RET(m_aCustomAttribDefs, sAttribID, pDef, FALSE);

	TDC_ATTRIBUTE nAttribID = pDef->GetAttributeID();
	CString sValue = GetValueText(nAttribID);

	// Attributes needing special handling
	if (pDef->IsMultiList())
	{
		CStringArray aMatched, aMixed;
		ParseMultiSelValues(sValue, aMatched, aMixed);

		data.Set(aMatched, aMixed, FALSE);
	}
	else if (pDef->IsDataType(TDCCA_DATE))
	{
		COleDateTime date;
		
		if (CDateHelper::DecodeDate(sValue, date, FALSE))
		{
			if (pDef->HasFeature(TDCCAF_SHOWTIME))
			{
				TDC_ATTRIBUTE nTimeAttribID = MapCustomDateToTime(nAttribID);
				date.m_dt += (CTimeHelper::DecodeClockTime(GetValueText(nTimeAttribID)) / 24);
			}

			data.Set(date);
		}
	}
	else // Default handling
	{
		data.Set(sValue);
	}

	return !data.IsEmpty();
}

TDC_ATTRIBUTE CTDLTaskAttributeListCtrl::MapCustomDateToTime(TDC_ATTRIBUTE nDateAttribID) const
{
#ifdef _DEBUG
	ASSERT(TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nDateAttribID));

	int nCust = m_aCustomAttribDefs.Find(nDateAttribID);
	ASSERT(nCust != -1);
	ASSERT(m_aCustomAttribDefs[nCust].IsDataType(TDCCA_DATE));
#endif

	return (TDC_ATTRIBUTE)(nDateAttribID + CUSTOMTIMEATTRIBOFFSET);
}

TDC_ATTRIBUTE CTDLTaskAttributeListCtrl::MapCustomTimeToDate(TDC_ATTRIBUTE nTimeAttribID) const
{
	TDC_ATTRIBUTE nDateAttribID = (TDC_ATTRIBUTE)(nTimeAttribID - CUSTOMTIMEATTRIBOFFSET);

#ifdef _DEBUG
	ASSERT(IsCustomTime(nTimeAttribID));
	ASSERT(TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nDateAttribID));

	int nCust = m_aCustomAttribDefs.Find(nDateAttribID);
	ASSERT(nCust != -1);
	ASSERT(m_aCustomAttribDefs[nCust].IsDataType(TDCCA_DATE));
#endif

	return nDateAttribID;
}

BOOL CTDLTaskAttributeListCtrl::IsCustomTime(TDC_ATTRIBUTE nAttribID)
{
	return (nAttribID > CUSTOMTIMEATTRIBOFFSET);
}

CString CTDLTaskAttributeListCtrl::FormatMultiSelItems(const CStringArray& aMatched, const CStringArray& aMixed)
{
	CString sValue = Misc::FormatArray(aMatched);
	
	if (aMixed.GetSize())
		sValue += ('|' + Misc::FormatArray(aMixed));

	return sValue;
}

int CTDLTaskAttributeListCtrl::ParseMultiSelValues(const CString& sValues, CStringArray& aMatched, CStringArray& aMixed)
{
	CString sMatched(sValues), sMixed;

	Misc::Split(sMatched, sMixed, '|');
	Misc::Split(sMatched, aMatched);
	Misc::Split(sMixed, aMixed);

	return aMatched.GetSize();
}

void CTDLTaskAttributeListCtrl::PrepareMultiSelCombo(int nRow, const CStringArray& aDefValues, const CStringArray& aUserValues, CEnCheckComboBox& combo)
{
	combo.EnableMultiSelection(TRUE);
	combo.ResetContent();
	combo.AddStrings(aDefValues);
	combo.AddUniqueItems(aUserValues);

	CStringArray aMatched, aMixed;
	ParseMultiSelValues(GetItemText(nRow, VALUE_COL), aMatched, aMixed);

	combo.SetChecked(aMatched, aMixed);
}

void CTDLTaskAttributeListCtrl::PrepareSingleSelCombo(int nRow, const CStringArray& aDefValues, const CStringArray& aUserValues, CEnCheckComboBox& combo)
{
	combo.EnableMultiSelection(FALSE);
	combo.ResetContent();
	combo.AddStrings(aDefValues);
	combo.AddUniqueItems(aUserValues);

	CString sValue = GetItemText(nRow, VALUE_COL);
	combo.AddUniqueItem(sValue);

	combo.SelectString(-1, sValue);
}

void CTDLTaskAttributeListCtrl::PrepareControl(CWnd& ctrl, int nRow, int nCol)
{
	if (nCol != VALUE_COL)
	{
		ASSERT(0);
		return;
	}

	TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);

	switch (nAttribID)
	{
	case TDCA_ALLOCBY:	PrepareSingleSelCombo(nRow, m_tldDefault.aAllocBy, m_tldAll.aAllocBy, m_cbTextAndNumbers);	break;
	case TDCA_STATUS: 	PrepareSingleSelCombo(nRow, m_tldDefault.aStatus, m_tldAll.aStatus, m_cbTextAndNumbers);	break;
	case TDCA_VERSION: 	PrepareSingleSelCombo(nRow, m_tldDefault.aVersion, m_tldAll.aVersion, m_cbTextAndNumbers);	break;
		break;

	case TDCA_ALLOCTO:	PrepareMultiSelCombo(nRow, m_tldDefault.aAllocTo, m_tldAll.aAllocTo, m_cbTextAndNumbers);	break;
	case TDCA_CATEGORY: PrepareMultiSelCombo(nRow, m_tldDefault.aCategory, m_tldAll.aCategory, m_cbTextAndNumbers);	break;
	case TDCA_TAGS:		PrepareMultiSelCombo(nRow, m_tldDefault.aTags, m_tldAll.aTags, m_cbTextAndNumbers);			break;

	case TDCA_FILELINK:
		{
			CStringArray aFiles;
			
			if (m_taskCtrl.GetSelectedTaskFileLinks(aFiles, TRUE))
				m_cbMultiFileLink.SetFileList(aFiles);
		}
		break;

	case TDCA_ICON: 
	case TDCA_FLAG: 
	case TDCA_LOCK: 
	case TDCA_COLOR:
	case TDCA_RECURRENCE:
	case TDCA_TASKNAME:
	case TDCA_EXTERNALID: 
		break; // Nothing to do

	case TDCA_PERCENT:
		m_editBox.SetMask(_T("0123456789"));
		m_editBox.SetSpinBuddy(&m_spinPercent);
		break;

	case TDCA_DEPENDENCY:
		{
			CTDCDependencyArray aDepends;
			m_taskCtrl.GetSelectedTaskDependencies(aDepends);

			m_eDepends.SetDependencies(aDepends);
		}
		break;

	case TDCA_COST:
		m_editBox.SetMask(_T("-0123456789.@"), ME_LOCALIZEDECIMAL);
		break;

	case TDCA_PRIORITY:
		{
			CDWordArray aColors;

			m_taskCtrl.GetPriorityColors(aColors);
			m_cbPriority.SetColors(aColors);

			m_cbPriority.SetSelectedPriority(_ttoi(GetItemText(nRow, nCol)));
		}
		break;

	case TDCA_RISK:
		m_cbRisk.SetSelectedRisk(_ttoi(GetItemText(nRow, nCol)));
		break;

	case TDCA_TIMEESTIMATE:
		PrepareTimePeriodEdit(nRow);
		break;

	case TDCA_TIMESPENT:
		{
			PrepareTimePeriodEdit(nRow);

			m_eTimePeriod.InsertButton(0, ID_BTN_TIMETRACK, s_iconTrackTime, CEnString(IDS_TDC_STARTSTOPCLOCK), 15);
			m_eTimePeriod.InsertButton(1, ID_BTN_ADDLOGGEDTIME, s_iconAddTime, CEnString(IDS_TDC_ADDLOGGEDTIME), 15);
		}
		break;

	case TDCA_DONEDATE:
		PrepareDatePicker(nRow, TDCA_NONE);
		break;

	case TDCA_DUEDATE:
		PrepareDatePicker(nRow, TDCA_STARTDATE);
		break;

	case TDCA_STARTDATE:
		PrepareDatePicker(nRow, TDCA_DUEDATE);
		break;

	case TDCA_DONETIME:
	case TDCA_DUETIME:
	case TDCA_STARTTIME:
		PrepareTimeOfDayCombo(nRow);
		break;

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		{
			const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
			GET_CUSTDEF_ALT(m_aCustomAttribDefs, nAttribID, pDef, break);

			TDCCADATA data;
			m_taskCtrl.GetSelectedTaskCustomAttributeData(pDef->sUniqueID, data);

			if (pDef->IsList())
			{
				CStringArray aDefValues;
				pDef->GetUniqueListData(aDefValues);

				switch (pDef->GetDataType())
				{
				case TDCCA_STRING:
				case TDCCA_FRACTION:
				case TDCCA_INTEGER:
				case TDCCA_DOUBLE:
					if (pDef->IsMultiList())
						PrepareMultiSelCombo(nRow, aDefValues, CStringArray(), m_cbTextAndNumbers);
					else
						PrepareSingleSelCombo(nRow, aDefValues, CStringArray(), m_cbTextAndNumbers);
					break;

				case TDCCA_ICON:
					if (pDef->IsMultiList())
						PrepareMultiSelCombo(nRow, aDefValues, CStringArray(), m_cbCustomIcons);
					else
						PrepareSingleSelCombo(nRow, aDefValues, CStringArray(), m_cbCustomIcons);
					break;

				case TDCCA_BOOL:
				case TDCCA_FILELINK:
				case TDCCA_CALCULATION:
				case TDCCA_TIMEPERIOD:
				case TDCCA_DATE:
					break; // Not supported
				}
			}
			else
			{
				switch (pDef->GetDataType())
				{
				case TDCCA_STRING:
				case TDCCA_FRACTION:
				case TDCCA_INTEGER:
				case TDCCA_DOUBLE:
				case TDCCA_ICON:
				case TDCCA_FILELINK:
				case TDCCA_BOOL:
					break; // Handled by base class

				case TDCCA_CALCULATION:
					break; // Not editable

				case TDCCA_TIMEPERIOD:
					PrepareTimePeriodEdit(nRow);
					break;

				case TDCCA_DATE:
					PrepareDatePicker(nRow, TDCA_NONE);
					break;
				}
			}
		}
		else if (IsCustomTime(nAttribID))
		{
			PrepareTimeOfDayCombo(nRow);
		}
		break;
	}
}

void CTDLTaskAttributeListCtrl::PrepareDatePicker(int nRow, TDC_ATTRIBUTE nFallbackDate)
{
	CString sValue = GetItemText(nRow, VALUE_COL);

	if (sValue.IsEmpty() && 
		(nFallbackDate != TDCA_NONE) && 
		(TDC::MapAttributeToDate(nFallbackDate) != TDCD_NONE))
	{
		sValue = GetValueText(nFallbackDate);
	}

	COleDateTime date;

	if (CDateHelper::DecodeDate(sValue, date, FALSE))
		m_datePicker.SetTime(date);
	else
		m_datePicker.SendMessage(DTM_SETSYSTEMTIME, GDT_NONE, 0);
}

void CTDLTaskAttributeListCtrl::PrepareTimeOfDayCombo(int nRow)
{
	CString sValue = GetItemText(nRow, VALUE_COL);

	if (sValue.IsEmpty())
		m_cbTimeOfDay.Set24HourTime(-1);
	else
		m_cbTimeOfDay.Set24HourTime(CTimeHelper::DecodeClockTime(sValue));

	DWORD dwStyle = m_cbTimeOfDay.GetStyle();
	Misc::SetFlag(dwStyle, TCB_ISO, m_data.HasStyle(TDCS_SHOWDATESINISO));

	m_cbTimeOfDay.SetStyle(dwStyle);
}

void CTDLTaskAttributeListCtrl::PrepareTimePeriodEdit(int nRow)
{
	CString sValue = GetItemText(nRow, VALUE_COL);
	TH_UNITS nUnits = THU_NULL;
	double dValue = 0.0;
		
	if (CTimeHelper::DecodeOffset(sValue, dValue, nUnits, FALSE))
		m_eTimePeriod.SetTime(dValue, nUnits);
}

CString CTDLTaskAttributeListCtrl::GetValueText(TDC_ATTRIBUTE nAttribID) const 
{ 
	return GetItemText(GetRow(nAttribID), VALUE_COL); 
}

CWnd* CTDLTaskAttributeListCtrl::GetEditControl(int nRow, BOOL bBtnClick)
{
	// Sanity check
	if ((nRow < 0) || (nRow > GetItemCount()))
		return NULL;

	TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);

	switch (nAttribID)
	{
	case TDCA_ALLOCBY:
	case TDCA_STATUS:
	case TDCA_VERSION:
	case TDCA_ALLOCTO:
	case TDCA_CATEGORY:
	case TDCA_TAGS:
		return &m_cbTextAndNumbers;

	case TDCA_FLAG:
	case TDCA_ICON:
	case TDCA_LOCK:
	case TDCA_COLOR:
	case TDCA_RECURRENCE:
	case TDCA_FILELINK:
		return NULL; // Handled in EditCell()

	case TDCA_PERCENT:
	case TDCA_EXTERNALID:
	case TDCA_COST:
	case TDCA_TASKNAME:
		return CInputListCtrl::GetEditControl();

	case TDCA_PRIORITY:
		return &m_cbPriority;

	case TDCA_RISK:
		return &m_cbRisk;

	case TDCA_TIMEESTIMATE:
	case TDCA_TIMESPENT:
		return &m_eTimePeriod;

	case TDCA_DONEDATE:
	case TDCA_DUEDATE:
	case TDCA_STARTDATE:
		return &m_datePicker;

	case TDCA_DONETIME:
	case TDCA_DUETIME:
	case TDCA_STARTTIME:
		return &m_cbTimeOfDay;

	case TDCA_DEPENDENCY:
		return (bBtnClick ? NULL : &m_eDepends);

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		{
			const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
			GET_CUSTDEF_RET(m_aCustomAttribDefs, nAttribID, pDef, NULL);

			if (pDef->IsList())
			{
				switch (pDef->GetDataType())
				{
				case TDCCA_STRING:
				case TDCCA_FRACTION:
				case TDCCA_INTEGER:
				case TDCCA_DOUBLE:
					return &m_cbTextAndNumbers;

				case TDCCA_ICON:
					return &m_cbCustomIcons;

				case TDCCA_FILELINK:
				case TDCCA_BOOL:
				case TDCCA_TIMEPERIOD:
				case TDCCA_DATE:
				case TDCCA_CALCULATION:
					break; // Not supported
				}
			}
			else
			{
				switch (pDef->GetDataType())
				{
				case TDCCA_STRING:
				case TDCCA_FRACTION:
				case TDCCA_INTEGER:
				case TDCCA_DOUBLE:
					return CInputListCtrl::GetEditControl();

				case TDCCA_CALCULATION:
					return NULL; // Not editable

				case TDCCA_BOOL:
				case TDCCA_ICON:
					return NULL; // Handled in EditCell()

				case TDCCA_FILELINK:
					return &m_eSingleFileLink;

				case TDCCA_TIMEPERIOD:
					return &m_eTimePeriod;

				case TDCCA_DATE:
					return &m_datePicker;
				}
			}
		}
		else if (IsCustomTime(nAttribID))
		{
			const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
			GET_CUSTDEF_RET(m_aCustomAttribDefs, MapCustomTimeToDate(nAttribID), pDef, NULL);

			if (pDef->HasFeature(TDCCAF_SHOWTIME))
			{
				return &m_cbTimeOfDay;
			}
		}
		break;
	}

	// all else
	ASSERT(0);
	return NULL;
}

void CTDLTaskAttributeListCtrl::EditCell(int nRow, int nCol, BOOL bBtnClick)
{
	if (!CanEditCell(nRow, nCol))
		return;

	CWnd* pCtrl = GetEditControl(nRow, bBtnClick);
	TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);

	if (pCtrl != NULL)
	{
		if (pCtrl == CInputListCtrl::GetEditControl())
		{
			PrepareControl(m_editBox, nRow, nCol);
			CInputListCtrl::EditCell(nRow, nCol, bBtnClick);
		}
		else if (pCtrl == &m_eTimePeriod)
		{
			ShowControl(*pCtrl, nRow, nCol, bBtnClick);

			if (bBtnClick)
			{
				m_eTimePeriod.ShowUnitsPopupMenu();
				HideControl(*pCtrl);
			}
		}
		else
		{
			ShowControl(*pCtrl, nRow, nCol, bBtnClick);
		}

		return;
	}

	// All other attributes not otherwise handled
	CString sValue = GetItemText(nRow, VALUE_COL);

	switch (nAttribID)
	{
	case TDCA_FLAG:
	case TDCA_LOCK:
		// Toggle checkbox
		SetItemText(nRow, VALUE_COL, (sValue.IsEmpty() ? _T("+") : _T(""))); 
		NotifyParentEdit(nAttribID);
		break;

	case TDCA_ICON:
	case TDCA_COLOR:
	case TDCA_RECURRENCE:
		if (GetParent()->SendMessage(WM_TDCM_EDITTASKATTRIBUTE, nAttribID))
			RefreshSelectedTaskValue(nRow);
		break;

	case TDCA_DEPENDENCY:
		if (bBtnClick)
		{
			if (GetParent()->SendMessage(WM_TDCM_EDITTASKATTRIBUTE, nAttribID))
				RefreshSelectedTaskValue(nRow);
		}
		else
		{
			CInputListCtrl::EditCell(nRow, nCol, bBtnClick);
		}
		break;

	case TDCA_FILELINK:
		if (GetItemText(nRow, nCol).IsEmpty())
		{
			if (bBtnClick)
			{
				if (m_eSingleFileLink.DoBrowse())
				{
					CString sFile;
					m_eSingleFileLink.GetWindowText(sFile);

					SetItemText(nRow, nCol, sFile);
					NotifyParentEdit(nAttribID);
				}
			}
			else
			{
				ShowControl(m_eSingleFileLink, nRow, nCol, bBtnClick);
			}
		}
		else
		{
			PrepareControl(m_cbMultiFileLink, nRow, nCol);
			ShowControl(m_cbMultiFileLink, nRow, nCol, bBtnClick);
		}
		break;

	case TDCA_TASKNAME:
		// TODO
		break;

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		{
			const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
			GET_CUSTDEF_ALT(m_aCustomAttribDefs, nAttribID, pDef, break);

			if (pDef->IsList())
			{
				// TODO
			}
			else
			{
				// Custom attributes not handled by the base class
				switch (pDef->GetDataType())
				{
				case TDCCA_STRING:
				case TDCCA_FRACTION:
				case TDCCA_INTEGER:
				case TDCCA_DOUBLE:
				case TDCCA_FILELINK:
					// TODO
					break;

				case TDCCA_ICON:
					if (GetParent()->SendMessage(WM_TDCM_EDITTASKATTRIBUTE, nAttribID))
						RefreshSelectedTaskValue(nRow);
					break;

				case TDCCA_CALCULATION:
					// TODO
					break;

				case TDCCA_TIMEPERIOD:
					// TODO
					break;

				case TDCCA_DATE:
					// TODO
					break;

				case TDCCA_BOOL:
					// Toggle checkbox
					SetItemText(nRow, VALUE_COL, (sValue.IsEmpty() ? _T("+") : _T("")));
					NotifyParentEdit(nAttribID);
					break;
				}
			}
		}
		break;
	}
}

void CTDLTaskAttributeListCtrl::HideAllControls(const CWnd* pWndIgnore)
{
	CInputListCtrl::HideAllControls(pWndIgnore);

	HideControl(m_datePicker, pWndIgnore);
	HideControl(m_cbTextAndNumbers, pWndIgnore);
	HideControl(m_cbTimeOfDay, pWndIgnore);
	HideControl(m_cbPriority, pWndIgnore);
	HideControl(m_cbRisk, pWndIgnore);
	HideControl(m_eDepends, pWndIgnore);
	HideControl(m_eTimePeriod, pWndIgnore);
	HideControl(m_cbMultiFileLink, pWndIgnore);
	HideControl(m_eSingleFileLink, pWndIgnore);

	m_eTimePeriod.DeleteButton(ID_BTN_ADDLOGGEDTIME);
	m_eTimePeriod.DeleteButton(ID_BTN_TIMETRACK);
	
	if (pWndIgnore != &m_editBox)
		m_editBox.SetSpinBuddy(NULL);
}

void CTDLTaskAttributeListCtrl::OnComboCloseUp(UINT nCtrlID) 
{ 
	int nCount = GetDlgItem(nCtrlID)->SendMessage(CB_GETCOUNT);

	if (GetDlgItem(nCtrlID)->GetDlgItem(1001) == NULL)
		HideControl(*GetDlgItem(nCtrlID));
}

void CTDLTaskAttributeListCtrl::OnComboKillFocus(UINT nCtrlID)
{
	HideControl(*GetDlgItem(nCtrlID));
}

void CTDLTaskAttributeListCtrl::OnComboEditChange(UINT nCtrlID)
{
	HideControl(*GetDlgItem(nCtrlID));

	int nRow = GetCurSel();
	TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);

	CString sNewItemText;

	switch (nCtrlID)
	{
	case IDC_TEXTANDNUM_COMBO:
		if (m_cbTextAndNumbers.IsMultiSelectionEnabled())
		{
			CStringArray aMatched, aMixed;
			m_cbTextAndNumbers.GetChecked(aMatched, aMixed);

			sNewItemText = FormatMultiSelItems(aMatched, aMixed);
		}
		else
		{
			sNewItemText = CDialogHelper::GetSelectedItem(m_cbTextAndNumbers);
		}
		break;

	case IDC_TIME_PICKER:
		sNewItemText = CTimeHelper::FormatClockTime(m_cbTimeOfDay.Get24HourTime() / 24);
		break;

	case IDC_PRIORITY_COMBO:
		sNewItemText = Misc::Format(m_cbPriority.GetSelectedPriority());
		break;

	case IDC_RISK_COMBO:
		sNewItemText = Misc::Format(m_cbRisk.GetSelectedRisk());
		break;

	case IDC_FILELINK_COMBO:
		{
			CStringArray aFiles;
			
			if (m_cbMultiFileLink.GetFileList(aFiles))
				sNewItemText = Misc::FormatArray(aFiles);
		}
		break;

	case IDC_CUSTOMICON_COMBO:
		{
			CStringArray aMatched, aMixed;
			m_cbCustomIcons.GetChecked(aMatched, aMixed);

			sNewItemText = FormatMultiSelItems(aMatched, aMixed);
		}
		break;

	default:
		ASSERT(0);
		return;
	}

	if (sNewItemText != GetItemText(nRow, VALUE_COL))
	{
		SetItemText(nRow, VALUE_COL, sNewItemText);
		NotifyParentEdit(nAttribID);
	}
}

LRESULT CTDLTaskAttributeListCtrl::NotifyParentEdit(TDC_ATTRIBUTE nAttribID)
{
	UpdateWindow();

	if (IsCustomTime(nAttribID))
		nAttribID = MapCustomTimeToDate(nAttribID);

	return GetParent()->SendMessage(WM_TDCN_ATTRIBUTEEDITED, nAttribID);
}

void CTDLTaskAttributeListCtrl::OnDependsChange()
{
	// Received after a manual edit of the task IDs
	int nRow = GetCurSel();
	ASSERT(GetAttributeID(nRow) == TDCA_DEPENDENCY);

	HideControl(m_eDepends);
	SetItemText(nRow, VALUE_COL, m_eDepends.FormatDependencies());
	NotifyParentEdit(TDCA_DEPENDENCY);
}

void CTDLTaskAttributeListCtrl::OnSingleFileLinkChange()
{
	// Received after a manual edit of the task IDs
	int nRow = GetCurSel();
	TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);

	CString sFile;
	m_eSingleFileLink.GetWindowText(sFile);

	HideControl(m_eSingleFileLink);
	SetItemText(nRow, VALUE_COL, sFile);
	NotifyParentEdit(nAttribID);
}

void CTDLTaskAttributeListCtrl::OnTimePeriodChange()
{
	// Received after a manual edit of the task IDs
	int nRow = GetCurSel();
	TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);

	TDCTIMEPERIOD tp(m_eTimePeriod.GetTime(), m_eTimePeriod.GetUnits());

	HideControl(m_eTimePeriod);
	m_eTimePeriod.DeleteButton(ID_BTN_ADDLOGGEDTIME);
	m_eTimePeriod.DeleteButton(ID_BTN_TIMETRACK);

	SetItemText(nRow, VALUE_COL, tp.Format(2));
	NotifyParentEdit(nAttribID);
}

void CTDLTaskAttributeListCtrl::OnCancelEdit()
{
	int nRow = GetCurSel();
	TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);

	// Revert changes before default handling
	switch (nAttribID)
	{
	case TDCA_DEPENDENCY:
		m_eDepends.SetWindowText(m_eDepends.FormatDependencies());
		break;

	case TDCA_TIMEESTIMATE:
	case TDCA_TIMESPENT:
		PrepareTimePeriodEdit(nRow);
		break;
	}

	CInputListCtrl::OnCancelEdit();
}

void CTDLTaskAttributeListCtrl::OnDateCloseUp(NMHDR* pNMHDR, LRESULT* pResult) 
{ 
	UNREFERENCED_PARAMETER(pNMHDR);
	ASSERT(pNMHDR->idFrom == IDC_DATE_PICKER);

	HideControl(m_datePicker); 
	*pResult = 0;
}

void CTDLTaskAttributeListCtrl::OnDateChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);
	ASSERT(pNMHDR->idFrom == IDC_DATE_PICKER);

	// Only handle this if the calendar is closed
	if (!m_datePicker.IsCalendarVisible())
	{
		// Note: Don't hide the date picker because the user 
		// may be editing the date components manually

		int nRow = GetCurSel();
		TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);

		CString sNewItemText;
		COleDateTime date;

		if (m_datePicker.GetTime(date))
			sNewItemText = m_formatter.GetDateOnly(date, TRUE);
		
		if (sNewItemText != GetItemText(nRow, VALUE_COL))
		{
			SetItemText(nRow, VALUE_COL, sNewItemText);
			NotifyParentEdit(nAttribID);
		}
	}

	*pResult = 0;
}

LRESULT CTDLTaskAttributeListCtrl::OnAutoComboAddDelete(WPARAM wp, LPARAM lp)
{
	int nRow = GetCurSel();
	TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);

	switch (nAttribID)
	{
	case TDCA_ALLOCBY:	CDialogHelper::GetComboBoxItems(m_cbTextAndNumbers, m_tldAll.aAllocBy);	break;
	case TDCA_STATUS:	CDialogHelper::GetComboBoxItems(m_cbTextAndNumbers, m_tldAll.aStatus);	break;
	case TDCA_VERSION:	CDialogHelper::GetComboBoxItems(m_cbTextAndNumbers, m_tldAll.aVersion);	break;
	case TDCA_ALLOCTO:	CDialogHelper::GetComboBoxItems(m_cbTextAndNumbers, m_tldAll.aAllocTo);	break;
	case TDCA_CATEGORY: CDialogHelper::GetComboBoxItems(m_cbTextAndNumbers, m_tldAll.aCategory);break;
	case TDCA_TAGS:		CDialogHelper::GetComboBoxItems(m_cbTextAndNumbers, m_tldAll.aTags);	break;

	case TDCA_FILELINK:
		return 0L;
	}

	return GetParent()->SendMessage(WM_TDCN_AUTOITEMADDEDDELETED, nAttribID);
}

BOOL CTDLTaskAttributeListCtrl::DeleteSelectedCell()
{
	if (m_nCurCol == VALUE_COL)
	{
		TDC_ATTRIBUTE nAttribID = GetAttributeID(GetCurSel());
		
		if (GetParent()->SendMessage(WM_TDCM_CLEARTASKATTRIBUTE, nAttribID))
		{
			SetItemText(GetCurSel(), m_nCurCol, _T(""));
			return TRUE;
		}
	}

	// else
	return FALSE;
}

LRESULT CTDLTaskAttributeListCtrl::OnEnEditButtonClick(WPARAM wParam, LPARAM lParam)
{
	TDC_ATTRIBUTE nAttribID = GetAttributeID(GetCurSel());

	switch (lParam)
	{
	case ID_BTN_TIMETRACK:
		ASSERT(nAttribID == TDCA_TIMESPENT);
		return GetParent()->SendMessage(WM_TDCM_TOGGLETIMETRACKING);

	case ID_BTN_ADDLOGGEDTIME:
		ASSERT(nAttribID == TDCA_TIMESPENT);
		return GetParent()->SendMessage(WM_TDCM_ADDTIMETOLOGFILE);

	case ID_BTN_SELECTDEPENDS:
		ASSERT(nAttribID == TDCA_DEPENDENCY);
		return GetParent()->SendMessage(WM_TDCM_SELECTDEPENDENCIES);

	case ID_BTN_EDITDEPENDS:
		ASSERT(nAttribID == TDCA_DEPENDENCY);
		return GetParent()->SendMessage(WM_TDCM_EDITTASKATTRIBUTE, TDCA_DEPENDENCY);
	}

	return 0L;
}

LRESULT CTDLTaskAttributeListCtrl::OnFileLinkWantIcon(WPARAM wParam, LPARAM lParam)
{
	if (TDCTASKLINK::IsTaskLink((LPCTSTR)lParam, TRUE))
	{
		if (!s_iconApp.IsValid())
			VERIFY(s_iconApp.SetIcon(GraphicsMisc::GetAppWindowIcon(FALSE)));

		return (LRESULT)(HICON)s_iconApp;
	}

	return 0L;
}

LRESULT CTDLTaskAttributeListCtrl::OnFileLinkWantTooltip(WPARAM wParam, LPARAM lParam)
{
	return GetParent()->SendMessage(WM_TDCM_GETLINKTOOLTIP, wParam, lParam);
}

LRESULT CTDLTaskAttributeListCtrl::OnFileLinkDisplay(WPARAM wParam, LPARAM lParam)
{
	return GetParent()->SendMessage(WM_TDCM_DISPLAYLINK, wParam, lParam);
}
