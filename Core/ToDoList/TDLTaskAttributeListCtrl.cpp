// TDLTaskAttributeListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLTaskAttributeListCtrl.h"
#include "TDLTaskCtrlBase.h"
#include "TDCImageList.h"
#include "tdcstatic.h"
#include "tdcstruct.h"
#include "tdcmapping.h"
#include "TDLTaskIconDlg.h"

#include "..\shared\GraphicsMisc.h"
#include "..\shared\FileMisc.h"
#include "..\shared\HoldRedraw.h"
#include "..\shared\Localizer.h"
#include "..\shared\encolordialog.h"
#include "..\shared\FileIcons.h"
#include "..\shared\WndPrompt.h"
#include "..\shared\Themed.h"
#include "..\Shared\DateHelper.h"
#include "..\Shared\WinClasses.h"

#ifdef _DEBUG
#	include "..\shared\ScopedTimer.h"
#endif

#include "..\3rdParty\ColorDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

enum 
{
	LABEL_COL,
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
	ID_BTN_NONE = -1,
	ID_BTN_DEFAULT = 0,

	ID_BTN_TIMETRACK = 10,
	ID_BTN_ADDLOGGEDTIME,
	ID_BTN_VIEWDEPENDS,
	ID_BTN_EDITDEPENDS,
	ID_BTN_BROWSEFILE,
	ID_BTN_VIEWFILE,
	ID_BTN_CLEARREMINDER,
};

/////////////////////////////////////////////////////////////////////////////

const int CUSTOMTIMEATTRIBOFFSET = (TDCA_LAST_ATTRIBUTE + 1);

#define CUSTOMTIMEATTRIBID(dateID) ((TDC_ATTRIBUTE)(dateID + CUSTOMTIMEATTRIBOFFSET))

/////////////////////////////////////////////////////////////////////////////

const int COMBO_DROPHEIGHT	= GraphicsMisc::ScaleByDPIFactor(200);
const int ICON_SIZE			= GraphicsMisc::ScaleByDPIFactor(16);
const int SPLITTER_WIDTH	= GraphicsMisc::ScaleByDPIFactor(6);
const int MIN_EDIT_WIDTH	= GraphicsMisc::ScaleByDPIFactor(100);

const int MIN_COL_WIDTH		= (4 * EE_BTNWIDTH_DEFAULT);
const int MAX_EDIT_WIDTH	= (2 * MIN_EDIT_WIDTH);
const int MAX_TIP_LINELEN	= MAX_EDIT_WIDTH;

const int VALUE_VARIES = 1;
const int TIMEPERIOD_DECPLACES = 6; // Preserve full(ish) precision

// We use non-printable characters to avoid clashing with user data
const TCHAR ITEM_DELIM[2]  = { 14, 0 };
const TCHAR MIXED_DELIM[2] = { 15, 0 };
const TCHAR STATE_DELIM[2] = { 16, 0 };
const TCHAR TOOLTIP_DELIM  = '\n';

const LPCTSTR DATETIME_VARIES = _T("-1");

/////////////////////////////////////////////////////////////////////////////

enum 
{
	ICON_REMINDER		= IDI_REMINDER,
	ICON_TRACKTIME		= IDI_TIMETRACK,
	ICON_ADDTIME		= IDI_ADD_LOGGED_TIME,
	ICON_SHOWDEPENDS	= IDI_DEPENDS_LINK,
	ICON_BROWSEFILE		= IDI_FILEEDIT_BROWSE,
	ICON_SELECTICON		= IDI_ICON_SELECT,
	ICON_VIEWFILE		= IDI_FILEEDIT_GO,
	ICON_CLEARREMINDER	= IDI_RESET,
}; 

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskAttributeListCtrl

CTDLTaskAttributeListCtrl::CTDLTaskAttributeListCtrl(const CToDoCtrlData& data,
													 const CContentMgr& mgrContent,
													 const CTDCImageList& ilIcons,
													 const TDCCOLEDITVISIBILITY& vis,
													 const CTDCReminderHelper& rems,
													 const CTDCCustomAttribDefinitionArray& aCustAttribDefs)
	:
	m_data(data),
	m_ilIcons(ilIcons),
	m_vis(vis),
	m_aCustomAttribDefs(aCustAttribDefs),
	m_formatter(data, mgrContent),
	m_multitasker(data, mgrContent),
	m_calculator(data),
	m_cbTimeOfDay(TCB_HALFHOURS | TCB_NOTIME | TCB_HOURSINDAY),
	m_cbPriority(FALSE),
	m_cbRisk(FALSE),
	m_cbCustomIcons(ilIcons),
	m_dropFiles(this),
	m_bSplitting(FALSE),
	m_fAttribColProportion(0.5f),
	m_bGrouped(FALSE),
	m_aSortedGroupedItems(*this),
	m_aAttribState(aCustAttribDefs),
	m_reminders(rems),
	m_eSingleFileLink(FES_GOBUTTON | FES_RELATIVEPATHS),
	m_cbMultiFileLink(FES_GOBUTTON | FES_RELATIVEPATHS),
	m_iconCache(FALSE), // small icons
	m_dwTimeTrackingTask(0),
	m_bTaskIDChangeSinceLastEdit(FALSE)
{
	SetSortColumn(0, FALSE);

	// Fixed 'Dependency' buttons
	m_eDepends.EnableButtonPadding(FALSE);
	m_eDepends.SetDefaultButton(0);
	m_eDepends.AddButton(ID_BTN_VIEWDEPENDS, GetButtonIcon(ICON_SHOWDEPENDS), CEnString(IDS_VIEW));
	m_eDepends.AddButton(ID_BTN_EDITDEPENDS, _T("..."), CEnString(IDS_ATTRIBTIP_MODIFY));

	m_eTimePeriod.EnableButtonPadding(FALSE);
	m_eTimePeriod.SetDefaultButton(0);

	m_eSingleFileLink.EnableButtonPadding(FALSE);
	m_eSingleFileLink.SetDefaultButton(0);

	m_cbMultiFileLink.EnableButtonPadding(FALSE);
	m_cbMultiFileLink.SetDefaultButton(0);
}

CTDLTaskAttributeListCtrl::~CTDLTaskAttributeListCtrl()
{
}

BEGIN_MESSAGE_MAP(CTDLTaskAttributeListCtrl, CInputListCtrl)
	//{{AFX_MSG_MAP(CTDLTaskAttributeListCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CAPTURECHANGED()
	ON_WM_KEYDOWN()

	ON_NOTIFY(DTN_CLOSEUP, IDC_DATE_PICKER, OnDateCloseUp)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATE_PICKER, OnDateChange)
	ON_NOTIFY(NM_KILLFOCUS, IDC_DATE_PICKER, OnDateKillFocus)

	ON_EN_CHANGE(IDC_DEPENDS_EDIT, OnDependsChange)
	ON_EN_KILLFOCUS(IDC_DEPENDS_EDIT, OnDependsKillFocus)
	ON_EN_KILLFOCUS(IDC_TIMEPERIOD_EDIT, OnTimePeriodKillFocus)
	ON_EN_KILLFOCUS(IDC_FILELINK_EDIT, OnSingleFileLinkKillFocus)
	
	ON_CBN_SELENDOK(IDC_TIME_PICKER, OnTimeOfDaySelEndOK)

	ON_CONTROL_RANGE(CBN_KILLFOCUS, 0, 0xffff, OnComboKillFocus)
	ON_CONTROL_RANGE(CBN_CLOSEUP, 0, 0xffff, OnComboCloseUp)
	ON_CONTROL_RANGE(CBN_SELCHANGE, 0, 0xffff, OnComboSelChange)

	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnTextEditOK)

	ON_REGISTERED_MESSAGE(WM_ACBN_ITEMADDED, OnAutoComboAddDelete)
	ON_REGISTERED_MESSAGE(WM_ACBN_ITEMDELETED, OnAutoComboAddDelete)

	ON_REGISTERED_MESSAGE(WM_EE_BTNCLICK, OnEnEditButtonClick)
	ON_REGISTERED_MESSAGE(WM_FE_DISPLAYFILE, OnFileLinkDisplay)
	ON_REGISTERED_MESSAGE(WM_FE_GETFILEICON, OnFileLinkWantIcon)
	ON_REGISTERED_MESSAGE(WM_FE_GETFILETOOLTIP, OnFileLinkWantTooltip)
	ON_REGISTERED_MESSAGE(WM_FEN_BROWSECHANGE, OnSingleFileLinkNotifyBrowse)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskAttributeListCtrl message handlers

BOOL CTDLTaskAttributeListCtrl::Create(CWnd* pParent, UINT nID)
{
	DWORD dwFlags = (WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | LVS_NOCOLUMNHEADER | LVS_SHOWSELALWAYS | LVS_SINGLESEL);

	return CEnListCtrl::Create(dwFlags, CRect(0, 0, 0, 0), pParent, nID);
}

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
	CreateControl(m_datePicker, IDC_DATE_PICKER, DTS_SHORTDATEFORMAT | DTS_SHOWNONE);
	CreateControl(m_cbTimeOfDay, IDC_TIME_PICKER, (CBS_DROPDOWN | CBS_AUTOHSCROLL));
	CreateControl(m_cbPriority, IDC_PRIORITY_COMBO, CBS_DROPDOWNLIST);
	CreateControl(m_cbRisk, IDC_RISK_COMBO, CBS_DROPDOWNLIST);
	CreateControl(m_eDepends, IDC_DEPENDS_EDIT, ES_AUTOHSCROLL);
	CreateControl(m_eTimePeriod, IDC_TIMEPERIOD_EDIT, ES_AUTOHSCROLL);
	CreateControl(m_cbMultiFileLink, IDC_FILELINK_COMBO, (CBS_DROPDOWN | CBS_AUTOHSCROLL));
	CreateControl(m_eSingleFileLink, IDC_FILELINK_EDIT, ES_AUTOHSCROLL);
	CreateControl(m_cbCustomIcons, IDC_CUSTOMICON_COMBO, CBS_DROPDOWNLIST);

	VERIFY(m_spinPercent.Create(WS_CHILD | UDS_SETBUDDYINT | UDS_ARROWKEYS| UDS_ALIGNRIGHT, CRect(0, 0, 0, 0), this, IDC_PERCENT_SPIN));

	// Misc initialisation
	CLocalizer::EnableTranslation(m_cbTextAndNumbers, FALSE);
	CLocalizer::EnableTranslation(m_cbPriority, FALSE);
	CLocalizer::EnableTranslation(m_cbRisk, FALSE);
	CLocalizer::EnableTranslation(m_cbMultiFileLink, FALSE);

	VERIFY(m_dropFiles.Register(this));

	VERIFY(m_tooltip.Create(this));
	m_tooltip.EnableMultilineTips();

	TDC::GetPriorityRiskLevelStringResourceIDs(m_cbPriority.GetNumLevels(), m_aPriorityRiskStrResIDs);

	return 0;
}

void CTDLTaskAttributeListCtrl::ToggleSortDirection()
{
	m_bSortAscending = !m_bSortAscending;

	if (m_bGrouped)
		Populate(); // will also sort 
	else
		Sort();
}

BOOL CTDLTaskAttributeListCtrl::GetAttributeToMoveBelow(TDC_ATTRIBUTE nAttribID, BOOL bUp, TDC_ATTRIBUTE& nBelowAttribID) const
{
	// Reverse direction if sorted descending
	if (!m_bSortAscending)
		bUp = !bUp;

	nBelowAttribID = nAttribID;

	if (nAttribID == TDCA_NONE)
		return FALSE;

	// Time of day is not movable
	if (MapTimeToDate(nAttribID) != TDCA_NONE)
		return FALSE;

	if (IsAttributeMoveLimited(nAttribID, bUp))
		return FALSE;

	int nRowOffset = (bUp ? 2 : 1);

	for (int nStep = 0; nStep < nRowOffset; nStep++)
	{
		TDC_ATTRIBUTE nNextAttribID;
		
		if (!m_aAttribState.GetNextAttribute(nBelowAttribID, bUp, m_bGrouped, nNextAttribID))
		{
			ASSERT(bUp);
			ASSERT(nStep == 1);

			// if we are on the second 'step' shifting UP then it means
			// we're either below the absolute topmost attribute or we're 
			// below 'our' topmost group item
			if (m_bGrouped)
			{
				// Use whatever attribute is immediately above the topmost
				// group item regardless of visibility
				if (m_aAttribState.GetNextAttribute(nBelowAttribID, bUp, FALSE, nNextAttribID))
				{
					nBelowAttribID = nNextAttribID;
					break;
				}
			}

			// else there is no attribute above so we're at the top
			// so we return TDCA_NONE to insert above the topmost item
			nNextAttribID = TDCA_NONE;
		}

		nBelowAttribID = nNextAttribID;

		if (nBelowAttribID == TDCA_NONE)
			break;
		
		// Skip attributes not actually in the list
		if (GetRow(nBelowAttribID) == -1)
			nStep--;
	}

	return TRUE;
}

BOOL CTDLTaskAttributeListCtrl::IsAttributeMoveLimited(TDC_ATTRIBUTE nAttribID, BOOL bUp) const
{
	TDC_ATTRIBUTE nNextAttribID = nAttribID;

	while (m_aAttribState.GetNextAttribute(nNextAttribID, bUp, m_bGrouped, nNextAttribID))
	{
		if (GetRow(nNextAttribID) != -1)
			return FALSE;
	}

	return TRUE;
}

BOOL CTDLTaskAttributeListCtrl::MoveSelectedAttribute(BOOL bUp)
{
	TDC_ATTRIBUTE nAttribID = GetAttributeID(GetCurSel()), nBelowAttribID;

	if (!GetAttributeToMoveBelow(nAttribID, bUp, nBelowAttribID))
		return FALSE;

	VERIFY(m_aAttribState.MoveAttribute(nAttribID, nBelowAttribID));
	Sort();

	return TRUE;
}

BOOL CTDLTaskAttributeListCtrl::CanMoveSelectedAttribute(BOOL bUp) const
{
	TDC_ATTRIBUTE nAttribID = GetAttributeID(GetCurSel()), nUnused;

	return GetAttributeToMoveBelow(nAttribID, bUp, nUnused);
}

BOOL CTDLTaskAttributeListCtrl::ResetAttributeMoves()
{
	if (!m_aAttribState.ResetOrder())
		return FALSE;

	Sort();
	return TRUE;
}

BOOL CTDLTaskAttributeListCtrl::CanResetAttributeMoves() const
{
	return m_aAttribState.CanResetOrder();
}

int CTDLTaskAttributeListCtrl::CompareItems(DWORD dwItemData1, DWORD dwItemData2, int nSortColumn) const
{
	TDC_ATTRIBUTE nAttribID1 = (TDC_ATTRIBUTE)dwItemData1;
	TDC_ATTRIBUTE nAttribID2 = (TDC_ATTRIBUTE)dwItemData2;

	// If one or both values are time fields we need special handling
	// to ensure that the time fields always remain immediately below
	// their corresponding date fields
	TDC_ATTRIBUTE nDateAttribID1 = MapTimeToDate(nAttribID1);
	TDC_ATTRIBUTE nDateAttribID2 = MapTimeToDate(nAttribID2);

	BOOL bAttrib1IsTime = (nDateAttribID1 != TDCA_NONE);
	BOOL bAttrib2IsTime = (nDateAttribID2 != TDCA_NONE);

	if (bAttrib1IsTime || bAttrib2IsTime)
	{
		// If both values are time fields, they must have different date fields
		// and we just sort by their date fields
		if (bAttrib1IsTime && bAttrib2IsTime)
		{
			ASSERT(nDateAttribID1 != nDateAttribID2);

			nAttribID1 = nDateAttribID1;
			nAttribID2 = nDateAttribID2;
		}
		else if (bAttrib1IsTime)
		{
			// else if the first item is a time field
			ASSERT(!bAttrib2IsTime);

			// If the second attribute is the first attribute's  
			// date field we sort the time below the date
			if (nAttribID2 == nDateAttribID1)
			{
				ASSERT(bAttrib1IsTime);
				ASSERT(nAttribID2 != TDCA_NONE);

				return (m_bSortAscending ? 1 : -1);
			}

			// else we use the date field for the comparison
			// to keep the date and time adjacent to one another
			nAttribID1 = nDateAttribID1;
		}
		else // else second item must be a time field
		{
			ASSERT(bAttrib2IsTime);

			// If the first attribute is the second attribute's
			// date field we sort the time below the date
			if (nAttribID1 == nDateAttribID2)
			{
				ASSERT(bAttrib2IsTime);
				ASSERT(nAttribID1 != TDCA_NONE);

				return (m_bSortAscending ? -1 : 1);
			}

			// else we use the date field for the comparison
			// to keep the date and time adjacent to one another
			nAttribID2 = nDateAttribID2;
		}
	}

	return m_aAttribState.CompareItems(nAttribID1, nAttribID2);
}

void CTDLTaskAttributeListCtrl::ToggleGrouping()
{
	if (SupportsGrouping())
	{
		m_bGrouped = !m_bGrouped;
		
		if (GetSafeHwnd())
		{
			EnableGroupView(m_bGrouped);
			Populate();
		}
	}
}

void CTDLTaskAttributeListCtrl::SetNumPriorityRiskLevels(int nNumLevels)
{
	ASSERT(TDC::IsValidNumPriorityRiskLevels(nNumLevels));

	if (nNumLevels != m_cbPriority.GetNumLevels())
		TDC::GetPriorityRiskLevelStringResourceIDs(nNumLevels, m_aPriorityRiskStrResIDs);

	m_cbPriority.SetNumLevels(nNumLevels);
	m_cbPriority.SetColors(m_aPriorityColors);

	m_cbRisk.SetNumLevels(nNumLevels);
}

BOOL CTDLTaskAttributeListCtrl::HasFocus() const 
{ 
	return (IsEditing() || CDialogHelper::IsChildOrSame(this, GetFocus())); 
}

void CTDLTaskAttributeListCtrl::RedrawValue(TDC_ATTRIBUTE nAttribID)
{
	int nRow = GetRow(nAttribID);

	if (nRow != -1)
		RedrawCell(nRow, VALUE_COL, FALSE);
}

void CTDLTaskAttributeListCtrl::SelectValue(TDC_ATTRIBUTE nAttribID)
{
	int nRow = GetRow(nAttribID);

	if (nRow != -1)
	{
		SetCurSel(nRow, VALUE_COL);
		ScrollCellIntoView(nRow, VALUE_COL);
	}
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
			RefreshSelectedTasksValue(nRow);
			break;
		}
	}
}

void CTDLTaskAttributeListCtrl::SaveState(CPreferences& prefs, LPCTSTR szKey) const
{
	prefs.WriteProfileDouble(szKey, _T("AttribColProportion"), m_fAttribColProportion);
	prefs.WriteProfileInt(szKey, _T("AttribSortAscending"), m_bSortAscending);
	prefs.WriteProfileInt(szKey, _T("AttribGrouped"), m_bGrouped);
	prefs.WriteProfileInt(szKey, _T("SingleClickEditing"), HasSingleClickEditing());

	m_aAttribState.Save(prefs, szKey);
}

void CTDLTaskAttributeListCtrl::LoadState(const CPreferences& prefs, LPCTSTR szKey)
{
	m_fAttribColProportion = (float)prefs.GetProfileDouble(szKey, _T("AttribColProportion"), 0.5);
	m_bSortAscending = prefs.GetProfileInt(szKey, _T("AttribSortAscending"), TRUE);
	m_bGrouped = (SupportsGrouping() && prefs.GetProfileInt(szKey, _T("AttribGrouped"), FALSE));

	SetSingleClickEditing(prefs.GetProfileInt(szKey, _T("SingleClickEditing"), FALSE));

	m_aAttribState.Load(prefs, szKey);

	if (GetSafeHwnd())
	{
		EnableGroupView(m_bGrouped);
		Populate();
	}
}

void CTDLTaskAttributeListCtrl::SetCompletionStatus(const CString& sStatus)
{
	if (sStatus == m_sCompletionStatus)
		return;

	if (!sStatus.IsEmpty())
		Misc::AddUniqueItem(sStatus, m_tldDefault.aStatus);
	else
		Misc::RemoveItem(m_sCompletionStatus, m_tldDefault.aStatus);

	m_sCompletionStatus = sStatus;
}

void CTDLTaskAttributeListCtrl::SetPriorityColors(const CDWordArray& aColors)
{
	if (Misc::MatchAll(m_aPriorityColors, aColors, TRUE))
		return;

	m_aPriorityColors.Copy(aColors);
	m_cbPriority.SetColors(m_aPriorityColors);

	RedrawValue(TDCA_PRIORITY);
}

void CTDLTaskAttributeListCtrl::SetPercentDoneIncrement(int nAmount)
{
	ASSERT(m_spinPercent.GetSafeHwnd());
	
	UDACCEL uda = { 0, (UINT)nAmount };
	m_spinPercent.SetAccel(1, &uda);
}

void CTDLTaskAttributeListCtrl::SetTimeTrackTaskID(DWORD dwTaskID)
{
	BOOL bRedraw = ((m_dwTimeTrackingTask && !dwTaskID) ||
					m_multitasker.AnyTaskHasID(m_aSelectedTaskIDs, dwTaskID, FALSE));

	m_dwTimeTrackingTask = dwTaskID;

	if (bRedraw)
		RedrawValue(TDCA_TIMESPENT);
}

void CTDLTaskAttributeListCtrl::OnCustomAttributesChange()
{
	m_aAttribState.OnCustomAttributesChange();

	Populate();
}

void CTDLTaskAttributeListCtrl::OnAttributeVisibilityChange()
{
	Populate();
}

BOOL CTDLTaskAttributeListCtrl::WantAddAttribute(TDC_ATTRIBUTE nAttribID) const
{
	switch (nAttribID)
	{
	case TDCA_COMMENTS:	return FALSE;
	case TDCA_TASKNAME:	return TRUE;

	case TDCA_ICON:
		// Always show the icon field unless explicitly hidden
		return (m_vis.IsEditFieldVisible(TDCA_ICON) || (m_vis.GetShowFields() != TDLSA_ANY));
	}

	// All else
	return m_vis.IsEditFieldVisible(nAttribID);
}

int CTDLTaskAttributeListCtrl::CheckAddAttribute(TDC_ATTRIBUTE nAttribID, UINT nAttribResID)
{
	if (!WantAddAttribute(nAttribID))
		return -1;

	int nRow = AddRow(CEnString(nAttribResID));
	SetItemData(nRow, nAttribID);

	return nRow;
}

int CTDLTaskAttributeListCtrl::GetGroupAttributes(TDC_ATTRIBUTEGROUP nGroup, CMap<TDC_ATTRIBUTE, TDC_ATTRIBUTE, CString, LPCTSTR>& mapAttrib) const
{
	if (nGroup != TDCAG_CUSTOM)
	{
		for (int nAtt = 1; nAtt < ATTRIB_COUNT; nAtt++)
		{
			const TDCATTRIBUTE& attrib = TASKATTRIBUTES[nAtt];

			if ((attrib.nGroup == nGroup) && WantAddAttribute(attrib.nAttributeID))
				mapAttrib[attrib.nAttributeID] = CEnString(attrib.nLabelResID);
		}

		// Unhandled fields
		switch (nGroup)
		{
		case TDCAG_NUMERIC:
			{
				if (WantAddAttribute(TDCA_PARENTID))
					mapAttrib[TDCA_PARENTID] = CEnString(IDS_TDLBC_PARENTID);
			}
			break;

		case TDCAG_DATETIME:
			{
				if (WantAddAttribute(TDCA_STARTTIME))
					mapAttrib[TDCA_STARTTIME] = CEnString(IDS_TDLBC_STARTTIME);

				if (WantAddAttribute(TDCA_DUETIME))
					mapAttrib[TDCA_DUETIME] = CEnString(IDS_TDLBC_DUETIME);

				if (WantAddAttribute(TDCA_DONETIME))
					mapAttrib[TDCA_DONETIME] = CEnString(IDS_TDLBC_DONETIME);

				if (WantAddAttribute(TDCA_REMINDER))
					mapAttrib[TDCA_REMINDER] = CEnString(IDS_TDLBC_REMINDER);
			}
			break;
		}
	}
	else
	{
		// Custom attributes
		for (int nCust = 0; nCust < m_aCustomAttribDefs.GetSize(); nCust++)
		{
			const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = m_aCustomAttribDefs[nCust];

			if (attribDef.bEnabled)
			{
				TDC_ATTRIBUTE nAttribID = attribDef.GetAttributeID();
				mapAttrib[nAttribID] = attribDef.sLabel;

				if (attribDef.IsDataType(TDCCA_DATE) && attribDef.HasFeature(TDCCAF_SHOWTIME))
					mapAttrib[CUSTOMTIMEATTRIBID(nAttribID)] = attribDef.sLabel;
			}
		}
	}

	return mapAttrib.GetCount();
}

void CTDLTaskAttributeListCtrl::Populate()
{
	// Preserve current selection
	int nSelRow, nSelCol;
	TDC_ATTRIBUTE nSelAttribID = TDCA_NONE;

	if (GetCurSel(nSelRow, nSelCol))
		nSelAttribID = GetAttributeID(nSelRow);

	// Scoped to exclude the call to EnsureVisible at the end
	{
		CHoldRedraw hr(*this);
		DeleteAllItems();

		if (m_bGrouped)
		{
			CSortedGroupedHeaderArray aCategories(m_bSortAscending);

			for (int nCat = 0; nCat < aCategories.GetSize(); nCat++)
			{
				CMap<TDC_ATTRIBUTE, TDC_ATTRIBUTE, CString, LPCTSTR> mapAttribs;
				const ATTRIBGROUP& attribCat = aCategories[nCat];

				if (GetGroupAttributes(attribCat.nGroup, mapAttribs))
				{
					GetGrouping().InsertGroupHeader(nCat, attribCat.nGroup, attribCat.sName);

					POSITION pos = mapAttribs.GetStartPosition();
					TDC_ATTRIBUTE nAttribID = TDCA_NONE;
					CString sAttrib;

					while (pos)
					{
						mapAttribs.GetNextAssoc(pos, nAttribID, sAttrib);

						int nRow = AddRow(sAttrib);
						SetItemData(nRow, nAttribID);
						GetGrouping().SetItemGroupId(nRow, attribCat.nGroup);
					}
				}
			}

			m_aSortedGroupedItems.Clear();
		}
		else // simple list
		{
			// Built-in attributes
			for (int nAtt = 1; nAtt < ATTRIB_COUNT; nAtt++)
				CheckAddAttribute(TASKATTRIBUTES[nAtt].nAttributeID, TASKATTRIBUTES[nAtt].nLabelResID);

			// Associated time fields
			CheckAddAttribute(TDCA_STARTTIME, IDS_TDLBC_STARTTIME);
			CheckAddAttribute(TDCA_DUETIME, IDS_TDLBC_DUETIME);
			CheckAddAttribute(TDCA_DONETIME, IDS_TDLBC_DONETIME);

			// Reminder
			CheckAddAttribute(TDCA_REMINDER, IDS_TDLBC_REMINDER);

			// Custom attributes
			for (int nCust = 0; nCust < m_aCustomAttribDefs.GetSize(); nCust++)
			{
				const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = m_aCustomAttribDefs[nCust];

				if (attribDef.bEnabled)
				{
					CEnString sAttrib(IDS_CUSTOMCOLUMN, attribDef.sLabel);

					int nRow = AddRow(sAttrib);
					SetItemData(nRow, attribDef.GetAttributeID());

					if (attribDef.IsDataType(TDCCA_DATE) && attribDef.HasFeature(TDCCAF_SHOWTIME))
					{
						nRow = AddRow(sAttrib);
						SetItemData(nRow, CUSTOMTIMEATTRIBID(attribDef.GetAttributeID()));
					}
				}
			}
		}

		RefreshSelectedTasksValues();
		Sort();
	}

	// Restore previous selection
	if (nSelAttribID != TDCA_NONE)
	{
		SetCurSel(GetRow(nSelAttribID), nSelCol);
		EnsureVisible(nSelRow, FALSE);
	}
}

void CTDLTaskAttributeListCtrl::RecalcColumnWidths(int nAttribColWidth, int cx)
{
	if (cx == -1)
	{
		CRect rClient;
		GetClientRect(rClient);

		cx = rClient.Width();
	}
	
	if (cx == 0)
		return;

	if (nAttribColWidth == -1)
		nAttribColWidth = (int)(m_fAttribColProportion * cx);

	if (nAttribColWidth != GetColumnWidth(LABEL_COL))
	{
		CHoldRedraw hr(*this);

		SetColumnWidth(LABEL_COL, nAttribColWidth);
		SetColumnWidth(VALUE_COL, (cx - nAttribColWidth - 1));
	}
}

void CTDLTaskAttributeListCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CInputListCtrl::OnSize(nType, cx, cy);
	
	if (GetColumnCount())
		RecalcColumnWidths(-1, cx);
}

void CTDLTaskAttributeListCtrl::GetSplitterRect(CRect& rSplitBar) const
{
	GetClientRect(rSplitBar);

	rSplitBar.left += (int)(m_fAttribColProportion * rSplitBar.Width() - (SPLITTER_WIDTH / 2));
	rSplitBar.right = (rSplitBar.left + SPLITTER_WIDTH);
}

void CTDLTaskAttributeListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CRect rSplitBar;
	GetSplitterRect(rSplitBar);

	if (rSplitBar.PtInRect(point))
	{
		ASSERT(!m_bSplitting);

		m_fAttribColProportion = 0.5f;
		RecalcColumnWidths();

		return;
	}

	CInputListCtrl::OnLButtonDblClk(nFlags, point);
}

void CTDLTaskAttributeListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rSplitBar;
	GetSplitterRect(rSplitBar);

	if (rSplitBar.PtInRect(point) && ::DragDetect(*this, point))
	{
		OnCancelEdit();

		m_bSplitting = TRUE;
		SetCapture();

		return;
	}

	CInputListCtrl::OnLButtonDown(nFlags, point);
}

void CTDLTaskAttributeListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bSplitting)
	{
		m_bSplitting = FALSE;
		ReleaseCapture();

		CRect rClient;
		GetClientRect(rClient);

		m_fAttribColProportion = ((float)GetColumnWidth(LABEL_COL)) / (rClient.Width() - 1);
	}

	CInputListCtrl::OnLButtonUp(nFlags, point);
}

void CTDLTaskAttributeListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bSplitting)
	{
		// Prevent columns getting too narrow
		CRect rClient;
		GetClientRect(rClient);

		int nAttribColWidth = max(min(point.x, rClient.right - MIN_COL_WIDTH), MIN_COL_WIDTH);

		RecalcColumnWidths(nAttribColWidth, rClient.Width());
	}

	CInputListCtrl::OnMouseMove(nFlags, point);
}

void CTDLTaskAttributeListCtrl::OnCaptureChanged(CWnd* pWnd)
{
	if (m_bSplitting)
	{
		// Revert changes
		RecalcColumnWidths();
	}

	CInputListCtrl::OnCaptureChanged(pWnd);
}

BOOL CTDLTaskAttributeListCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// Check for mouse over a child editing control
	CPoint point(::GetMessagePos());

	if (IsEditing() && (::WindowFromPoint(point) != *this))
		return FALSE;

	// Check for mouse over splitter
	CRect rSplitBar;
	GetSplitterRect(rSplitBar);

	ScreenToClient(&point);

	if (rSplitBar.PtInRect(point))
		return GraphicsMisc::SetAfxCursor(AFX_IDC_HSPLITBAR);

	// Check for read-only or locked fields
	if (m_aSelectedTaskIDs.GetSize())
	{
		LVHITTESTINFO lvHit = { { point.x, point.y }, 0 };

		int nRow = SubItemHitTest(&lvHit);
		int nCol = lvHit.iSubItem;

		if ((nRow != -1) && (nCol == VALUE_COL))
		{
			BOOL bEditable = CanEditCell(nRow, nCol);

			if (!bEditable)
			{
				TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);
				CString sAppCursor(_T("NoDrag"));

				switch (nAttribID)
				{
				case TDCA_LOCK:
				case TDCA_REMINDER:
					break;

				default:
					if (!m_multitasker.AnyTaskIsUnlocked(m_aSelectedTaskIDs))
						sAppCursor = _T("Locked");
					break;
				}
				
				return GraphicsMisc::SetAppCursor(sAppCursor, _T("Resources\\Cursors"));
			}
		}
	}

	// all else
	return CInputListCtrl::OnSetCursor(pWnd, nHitTest, message);
}

TDC_ATTRIBUTE CTDLTaskAttributeListCtrl::GetAttributeID(int nRow, BOOL bResolveCustomTimeFields) const
{ 
	if (nRow == -1)
		return TDCA_NONE;

	TDC_ATTRIBUTE nAttribID = (TDC_ATTRIBUTE)GetItemData(nRow); 

	if (!IsCustomTime(nAttribID) || !bResolveCustomTimeFields)
		return nAttribID;

	// else
	TDC_ATTRIBUTE nDateAttribID = (TDC_ATTRIBUTE)GetItemData(nRow - 1);
	ASSERT(MapTimeToDate(nAttribID) == nDateAttribID);

	return nDateAttribID;
}

TDC_ATTRIBUTE CTDLTaskAttributeListCtrl::GetSelectedAttributeID() const
{
	return GetAttributeID(GetCurSel(), TRUE);
}

CString CTDLTaskAttributeListCtrl::GetSelectedAttributeLabel() const
{
	int nRow = GetCurSel();

	if (nRow == -1)
		return _T("");

	// else
	return GetItemText(nRow, LABEL_COL);
}

CString CTDLTaskAttributeListCtrl::GetAttributeLabel(TDC_ATTRIBUTE nAttribID) const
{
	int nRow = GetRow(nAttribID);

	if (nRow == -1)
		return _T("");

	// else
	return GetItemText(nRow, LABEL_COL);
}

void CTDLTaskAttributeListCtrl::SetSelectedAttributeLabelBackgroundColor(COLORREF crBkgnd)
{
	int nSel = GetCurSel();

	if (m_aAttribState.SetLabelBkgndColor(GetAttributeID(nSel), crBkgnd))
		RedrawCell(nSel, VALUE_COL);
}

void CTDLTaskAttributeListCtrl::ClearSelectedAttributeLabelBackgroundColor()
{
	int nSel = GetCurSel();

	if (m_aAttribState.ClearLabelBkgndColor(GetAttributeID(nSel)))
		RedrawCell(nSel, VALUE_COL);
}

COLORREF CTDLTaskAttributeListCtrl::GetSelectedAttributeLabelBackgroundColor() const
{
	return m_aAttribState.GetLabelBkgndColor(GetSelectedAttributeID());
}

BOOL CTDLTaskAttributeListCtrl::CanClearSelectedAttributeLabelBackgroundColor() const
{
	return (CanSetSelectedAttributeLabelBackgroundColor() &&
		   (CLR_NONE != GetSelectedAttributeLabelBackgroundColor()));
}

BOOL CTDLTaskAttributeListCtrl::CanSetSelectedAttributeLabelBackgroundColor() const
{
	return ((m_nCurCol == LABEL_COL) && (GetSelectedAttributeID() != TDCA_NONE));
}

IL_COLUMNTYPE CTDLTaskAttributeListCtrl::GetCellType(int nRow, int nCol) const
{
	if (nCol == LABEL_COL)
		return ILCT_TEXT;

	// else
	TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);
	IL_COLUMNTYPE nColType = ILCT_TEXT;

	switch (nAttribID)
	{
	case TDCA_TASKNAME:
	case TDCA_COST:
	case TDCA_EXTERNALID:
	case TDCA_PERCENT:
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
	case TDCA_TIMEREMAINING:
		break; // ILCT_TEXT;

	case TDCA_TIMEESTIMATE:
	case TDCA_TIMESPENT:
		nColType = ILCT_POPUPMENU;
		break;

	case TDCA_DONEDATE:
	case TDCA_DUEDATE:
	case TDCA_STARTDATE:
		nColType = ILCT_DATE;
		break;

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
		nColType = ILCT_COMBO;
		break;

	case TDCA_ICON:
	case TDCA_REMINDER:
		nColType = ILCT_ICON;
		break;

	case TDCA_RECURRENCE:
	case TDCA_DEPENDENCY:
	case TDCA_COLOR:
		nColType = ILCT_BROWSE;
		break;

	case TDCA_FLAG:
	case TDCA_LOCK:
		nColType = ILCT_CHECK;
		break;

	case TDCA_FILELINK:
		nColType = (GetItemText(nRow, nCol).IsEmpty() ? ILCT_ICON : ILCT_COMBO);
		break;

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		{
			const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
			GET_CUSTDEF_RET(m_aCustomAttribDefs, nAttribID, pDef, ILCT_TEXT);

			if (pDef->IsList())
			{
				nColType = ILCT_COMBO;
			}
			else
			{
				switch (pDef->GetDataType())
				{
				case TDCCA_STRING:
				case TDCCA_FRACTION:
				case TDCCA_INTEGER:
				case TDCCA_DOUBLE:
				case TDCCA_CALCULATION:
					break; // ILCT_TEXT;

				case TDCCA_TIMEPERIOD:	nColType = ILCT_POPUPMENU;	break;
				case TDCCA_DATE:		nColType = ILCT_DATE;		break;
				case TDCCA_BOOL:		nColType = ILCT_CHECK;		break;
				case TDCCA_ICON:		nColType = ILCT_ICON;		break;
				case TDCCA_FILELINK:	nColType = ILCT_ICON;		break;

				default:
					ASSERT(0);
					break;
				}
			}
		}
		else if (IsCustomTime(nAttribID))
		{
			nColType = ILCT_COMBO;
		}
		else
		{
			ASSERT(0);
		}
		break;
	}

	// Except checkboxes, hide buttons for non-editable cells
	if ((nColType != ILCT_CHECK) && !CanEditCell(nRow, nCol))
	{
		nColType = ILCT_TEXT;
	}

	return nColType;
}

BOOL CTDLTaskAttributeListCtrl::CanEditCell(int nRow, int nCol) const
{
	if (nCol != VALUE_COL)
		return FALSE;
	
	if (!CInputListCtrl::CanEditCell(nRow, nCol))
		return FALSE;

	TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);
	BOOL bCanEdit = m_multitasker.CanEditAnyTask(m_aSelectedTaskIDs, nAttribID);

	if (bCanEdit != -1) // Unhandled by multi-tasker
		return bCanEdit;

	if (IsCustomTime(nAttribID))
	{
		if (!m_multitasker.AnyTaskIsUnlocked(m_aSelectedTaskIDs))
			return FALSE;

		int nDateRow = (nRow - 1);
		ASSERT(GetDateRow(nAttribID) == nDateRow);

		return !GetItemText(nDateRow, VALUE_COL).IsEmpty();
	}

	ASSERT(0); // Unexpectedly unhandled
	return FALSE;
}

COLORREF CTDLTaskAttributeListCtrl::GetItemBackColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const
{
	TDC_ATTRIBUTE nAttribID = GetAttributeID(nItem);

	if ((nCol == LABEL_COL) && !bSelected)
	{
		COLORREF crBkgnd = m_aAttribState.GetLabelBkgndColor(nAttribID);

		if (crBkgnd != CLR_NONE)
			return crBkgnd;
	}
	else if (nCol == VALUE_COL)
	{
		if (!CanEditCell(nItem, nCol))
			return GetSysColor(COLOR_3DFACE);

		switch (nAttribID)
		{
		case TDCA_COLOR:
			if (!bSelected && m_data.HasStyle(TDCS_TASKCOLORISBACKGROUND))
			{
				COLORREF color = _ttoi(GetItemText(nItem, VALUE_COL));

				switch (color)
				{
				case CLR_NONE:
				case 0:
					break;

				default:
					return color;
				}
			}
			break;
		}
	}

	// All else
	return CInputListCtrl::GetItemBackColor(nItem, nCol, bSelected, bDropHighlighted, bWndFocus);
}

COLORREF CTDLTaskAttributeListCtrl::GetItemTextColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const
{
	if (nCol == LABEL_COL)
	{
		COLORREF crBkgnd = m_aAttribState.GetLabelBkgndColor(GetAttributeID(nItem));

		if (crBkgnd != CLR_NONE)
		{
			if (bSelected)
				return GraphicsMisc::GetExplorerItemSelectionTextColor(crBkgnd, GMIS_SELECTED, 0);
			
			// else
			return GraphicsMisc::GetBestTextColor(crBkgnd);
		}
	}
	else
	{
		ASSERT(nCol == VALUE_COL);

		if (!CanEditCell(nItem, nCol))
			return GetSysColor(COLOR_3DDKSHADOW);

		switch (GetAttributeID(nItem))
		{
		case TDCA_DEPENDENCY:
 			if (m_multitasker.AnyTaskHasLocalCircularDependencies(m_aSelectedTaskIDs))
 				return colorRed;
			break;

		case TDCA_COLOR:
			{
				COLORREF color = _ttoi(GetItemText(nItem, VALUE_COL));

				switch (color)
				{
				case CLR_NONE:
				case 0:
					break;

				default:
					{
						if (!bSelected && m_data.HasStyle(TDCS_TASKCOLORISBACKGROUND))
							return GraphicsMisc::GetBestTextColor(color);

						return color;
					}
					break;
				}
			}
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

void CTDLTaskAttributeListCtrl::SetAutoListData(TDC_ATTRIBUTE nAttribID, const TDCAUTOLISTDATA& tld)
{
	m_tldAll.Copy(tld, nAttribID);
	m_tldAll.AppendUnique(m_tldDefault, nAttribID);
}

void CTDLTaskAttributeListCtrl::GetAutoListData(TDC_ATTRIBUTE nAttribID, TDCAUTOLISTDATA& tld) const
{
	tld.Copy(m_tldAll, nAttribID);
}

void CTDLTaskAttributeListCtrl::SetAutoListDataReadOnly(TDC_ATTRIBUTE nAttribID, BOOL bReadOnly)
{
	switch (nAttribID)
	{
	case TDCA_ALLOCTO:
	case TDCA_ALLOCBY:
	case TDCA_STATUS:
	case TDCA_CATEGORY:
	case TDCA_TAGS:
	case TDCA_VERSION:
		if (bReadOnly)
			m_mapReadOnlyListData.Add(nAttribID);
		else
			m_mapReadOnlyListData.Remove(nAttribID);
		break;

	default:
		ASSERT(0);
		break;
	}
}

void CTDLTaskAttributeListCtrl::SetCurrentFolder(const CString& sFolder)
{
	m_sCurrentFolder = sFolder;
}

BOOL CTDLTaskAttributeListCtrl::SetSelectedTaskIDs(const CDWordArray& aTaskIDs)
{
	if (Misc::MatchAll(aTaskIDs, m_aSelectedTaskIDs))
		return FALSE;

	m_bTaskIDChangeSinceLastEdit = TRUE;
	m_aSelectedTaskIDs.Copy(aTaskIDs);

	RefreshSelectedTasksValues();

	return TRUE;
}

void CTDLTaskAttributeListCtrl::RefreshSelectedTasksValues()
{
	RefreshSelectedTasksValues(TDCA_ALL);
}

void CTDLTaskAttributeListCtrl::RefreshSelectedTasksValues(const CTDCAttributeMap& mapAttribIDs)
{
	CHoldRedraw hr(*this);
	HideAllControls();

	BOOL bRefreshAll = mapAttribIDs.Has(TDCA_ALL);
	BOOL bRefreshCustomCalcs = (bRefreshAll || m_aCustomAttribDefs.AnyCalculationUsesAnyAttribute(mapAttribIDs));

	int nRow = GetItemCount();

	while (nRow--)
	{
		BOOL bWantRefresh = bRefreshAll;

		if (!bWantRefresh)
		{
			TDC_ATTRIBUTE nRowAttribID = GetAttributeID(nRow, TRUE);
			bWantRefresh = mapAttribIDs.Has(nRowAttribID);

			if (!bWantRefresh && bRefreshCustomCalcs && TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nRowAttribID))
			{
				bWantRefresh = (m_aCustomAttribDefs.GetAttributeDataType(nRowAttribID, FALSE) == TDCCA_CALCULATION);
			}
		}

		if (bWantRefresh)
		{
			if (m_aSelectedTaskIDs.GetSize())
				RefreshSelectedTasksValue(nRow);
			else
				SetItemText(nRow, VALUE_COL, _T(""));
		}
	}

	EnableColumnEditing(VALUE_COL, m_aSelectedTaskIDs.GetSize());
}

void CTDLTaskAttributeListCtrl::RefreshSelectedTasksValue(TDC_ATTRIBUTE nAttribID)
{
	int nRow = GetRow(nAttribID);

	if (nRow != -1) // may be hidden
		RefreshSelectedTasksValue(nRow);
}

CString CTDLTaskAttributeListCtrl::FormatDate(const COleDateTime& date, BOOL bAndTime) const
{
	DWORD dwFlags = 0;

	Misc::SetFlag(dwFlags, DHFD_DOW, m_data.HasStyle(TDCS_SHOWWEEKDAYINDATES));
	Misc::SetFlag(dwFlags, DHFD_ISO, m_data.HasStyle(TDCS_SHOWDATESINISO));
	Misc::SetFlag(dwFlags, DHFD_TIME | DHFD_NOSEC, bAndTime);

	return CDateHelper::FormatDate(date, dwFlags);
}

CString CTDLTaskAttributeListCtrl::FormatTime(const COleDateTime& date, BOOL bNotSetIsEmpty) const
{
	if (bNotSetIsEmpty && !CDateHelper::DateHasTime(date))
		return _T("");

	return CTimeHelper::FormatClockTime(date, FALSE, m_data.HasStyle(TDCS_SHOWDATESINISO));
}

// -----------------------------------------------------------------------------------------

#define GETMULTIVALUE_STR(FUNCTION) \
{ bValueVaries = !m_multitasker.FUNCTION(m_aSelectedTaskIDs, sValue); }

// -----------------------------------------------------------------------------------------

#define GETMULTIVALUE_FMT(FUNCTION, TYPE, FMT)						\
{ TYPE value;														\
if (m_multitasker.FUNCTION(m_aSelectedTaskIDs, value)) sValue = FMT;\
else bValueVaries = TRUE; }

// -----------------------------------------------------------------------------------------

#define GETMULTIVALUE_DATETIME(DT)								\
{ COleDateTime value;											\
if (m_multitasker.GetTasksDate(m_aSelectedTaskIDs, DT, value))	\
{ if (CDateHelper::IsDateSet(value))							\
sValue = Misc::Format(value.m_dt); }							\
else { sValue = DATETIME_VARIES; bValueVaries = TRUE; } }

// -----------------------------------------------------------------------------------------

#define GETMULTIVALUE_LIST(FUNCTION)							\
{ CStringArray aMatched, aMixed;								\
m_multitasker.FUNCTION(m_aSelectedTaskIDs, aMatched, aMixed);	\
sValue = FormatMultiSelItems(aMatched, aMixed);					\
bValueVaries = aMixed.GetSize(); }

// -----------------------------------------------------------------------------------------

#define GETUNIQUEVALUE(FUNCTION)								\
{ if (dwSingleSelTaskID) sValue = FUNCTION(dwSingleSelTaskID);	\
else { ASSERT(m_aSelectedTaskIDs.GetSize() > 1); bValueVaries = TRUE; } }

// -----------------------------------------------------------------------------------------

#define GETMULTIVALUE_BOOL(FUNCTION)					\
{ BOOL value;											\
if (m_multitasker.FUNCTION(m_aSelectedTaskIDs, value))	\
sValue = (value ? _T("+") : _T(""));					\
else bValueVaries = TRUE; }

// -----------------------------------------------------------------------------------------

void CTDLTaskAttributeListCtrl::RefreshSelectedTasksValue(int nRow)
{
	CString sValue;
	BOOL bValueVaries = FALSE;

	if (m_aSelectedTaskIDs.GetSize())
	{
		DWORD dwSingleSelTaskID = GetSingleSelectedTaskID();
		TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);

		switch (nAttribID)
		{
		case TDCA_TASKNAME:			GETMULTIVALUE_STR(GetTasksTitle);			break;
		case TDCA_EXTERNALID:		GETMULTIVALUE_STR(GetTasksExternalID);		break;
		case TDCA_ALLOCBY:			GETMULTIVALUE_STR(GetTasksAllocatedBy);		break;
		case TDCA_STATUS:			GETMULTIVALUE_STR(GetTasksStatus);			break;
		case TDCA_VERSION:			GETMULTIVALUE_STR(GetTasksVersion);			break;
		case TDCA_ICON:				GETMULTIVALUE_STR(GetTasksIcon);			break;
		case TDCA_PATH:				GETMULTIVALUE_STR(GetTasksPath);			break;
		case TDCA_CREATEDBY:		GETMULTIVALUE_STR(GetTasksCreatedBy);		break;
		case TDCA_LASTMODBY:		GETMULTIVALUE_STR(GetTasksLastModifiedBy);	break;
		case TDCA_COMMENTSFORMAT:	GETMULTIVALUE_STR(GetTasksCommentsFormat);	break;
		case TDCA_SUBTASKDONE:		GETMULTIVALUE_STR(GetTasksSubtaskCompletion); break;

		case TDCA_ALLOCTO:			GETMULTIVALUE_LIST(GetTasksAllocatedTo);	break;
		case TDCA_CATEGORY:			GETMULTIVALUE_LIST(GetTasksCategories);		break;
		case TDCA_TAGS:				GETMULTIVALUE_LIST(GetTasksTags);			break;

		case TDCA_FLAG:				GETMULTIVALUE_BOOL(GetTasksFlagState);		break;
		case TDCA_LOCK:				GETMULTIVALUE_BOOL(GetTasksLockState);		break;

		case TDCA_PRIORITY:			GETMULTIVALUE_FMT(GetTasksPriority, int, Misc::Format(value));		break;
		case TDCA_RISK:				GETMULTIVALUE_FMT(GetTasksRisk, int, Misc::Format(value));			break;
		case TDCA_COLOR:			GETMULTIVALUE_FMT(GetTasksColor, COLORREF, Misc::Format(value));	break;
		case TDCA_PARENTID:			GETMULTIVALUE_FMT(GetTasksParentID, DWORD, Misc::Format(value));	break;
		case TDCA_COST:				GETMULTIVALUE_FMT(GetTasksCost, TDCCOST, value.Format(2));			break;

		case TDCA_RECURRENCE:		GETMULTIVALUE_FMT(GetTasksRecurrence, TDCRECURRENCE, value.GetRegularityText());	break;
		case TDCA_DEPENDENCY:		GETMULTIVALUE_FMT(GetTasksDependencies, CTDCDependencyArray, value.Format());		break;
		case TDCA_TIMEREMAINING:	GETMULTIVALUE_FMT(GetTasksTimeRemaining, TDCTIMEPERIOD, value.Format(2));			break;
		
		case TDCA_COMMENTSSIZE:		
			{
				GETMULTIVALUE_STR(GetTasksCommentsSize);

				if (!bValueVaries && !sValue.IsEmpty())
					sValue += _T(" KB");
			}
			break;

		case TDCA_FILELINK:
			{
				CStringArray aFileLinks;

				if (m_multitasker.GetTasksFileLinks(m_aSelectedTaskIDs, aFileLinks))
					sValue = FormatValueArray(aFileLinks);
				else
					bValueVaries = TRUE;
			}
			break;

		case TDCA_PERCENT:
			if (m_data.HasStyle(TDCS_AUTOCALCPERCENTDONE) ||
				(m_data.HasStyle(TDCS_AVERAGEPERCENTSUBCOMPLETION) && m_multitasker.AllTasksAreParents(m_aSelectedTaskIDs)))
			{
				int nPercent = m_calculator.GetTaskPercentDone(m_aSelectedTaskIDs[0]);

				for (int nID = 1; ((nID < m_aSelectedTaskIDs.GetSize()) && !bValueVaries); nID++)
				{
					bValueVaries = (m_calculator.GetTaskPercentDone(m_aSelectedTaskIDs[nID]) != nPercent);
				}

				if (!bValueVaries)
					sValue = Misc::Format(nPercent);
			}
			else
			{
				GETMULTIVALUE_FMT(GetTasksPercentDone, int, Misc::Format(value));
			}
			break;

		case TDCA_TIMEESTIMATE:
			if (m_data.HasStyle(TDCS_ALLOWPARENTTIMETRACKING) ||
				!m_multitasker.AllTasksAreParents(m_aSelectedTaskIDs))
			{
				GETMULTIVALUE_FMT(GetTasksTimeEstimate, TDCTIMEPERIOD, value.Format(6)); // Save full(ish) precision
			}
			else if (dwSingleSelTaskID)
			{
				sValue = m_formatter.GetTaskTimeEstimate(dwSingleSelTaskID);
			}
			break;

		case TDCA_TIMESPENT:
			if (m_data.HasStyle(TDCS_ALLOWPARENTTIMETRACKING) ||
				!m_multitasker.AllTasksAreParents(m_aSelectedTaskIDs))
			{
				GETMULTIVALUE_FMT(GetTasksTimeSpent, TDCTIMEPERIOD, value.Format(6)); // Save full(ish) precision
			}
			else if (dwSingleSelTaskID)
			{
				sValue = m_formatter.GetTaskTimeSpent(dwSingleSelTaskID);
			}
			break;

		case TDCA_DONEDATE:			GETMULTIVALUE_DATETIME(TDCD_DONEDATE);			break;
		case TDCA_DUEDATE:			GETMULTIVALUE_DATETIME(TDCD_DUEDATE);			break;
		case TDCA_STARTDATE:		GETMULTIVALUE_DATETIME(TDCD_STARTDATE);			break;
		case TDCA_LASTMODDATE:		GETMULTIVALUE_DATETIME(TDCD_LASTMOD);			break;
		case TDCA_CREATIONDATE:		GETMULTIVALUE_DATETIME(TDCD_CREATE);			break;

		case TDCA_DONETIME:			GETMULTIVALUE_DATETIME(TDCD_DONETIME);			break;
		case TDCA_DUETIME:			GETMULTIVALUE_DATETIME(TDCD_DUETIME);			break;
		case TDCA_STARTTIME:		GETMULTIVALUE_DATETIME(TDCD_STARTTIME);			break;

		case TDCA_POSITION:			GETUNIQUEVALUE(m_formatter.GetTaskPosition);	break;
		case TDCA_ID:				GETUNIQUEVALUE(Misc::Format);					break;

		case TDCA_REMINDER:
			{
				time_t tValue = 0;

				if (m_multitasker.GetTasksReminder(m_aSelectedTaskIDs, m_reminders, tValue))
				{
					switch (tValue)
					{
					case 0:  break;
					case -1: sValue = CEnString(IDS_REMINDER_DATENOTSET); break;
					default: sValue = FormatDate(tValue, TRUE); break;
					}
				}
				else
				{
					bValueVaries = TRUE;
				}
			}
			break;

		default:
			if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
			{
				const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
				GET_CUSTDEF_ALT(m_aCustomAttribDefs, nAttribID, pDef, break);

				CString sAttribID = pDef->sUniqueID;
				TDCCADATA data;

				if (m_multitasker.GetTasksCustomAttributeData(m_aSelectedTaskIDs, *pDef, data))
				{
					if (pDef->IsMultiList())
					{
						CStringArray aMatched, aMixed;
						data.AsArrays(aMatched, aMixed);

						sValue = FormatMultiSelItems(aMatched, aMixed);
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

						case TDCCA_DATE:
							if (!data.IsEmpty())
								sValue = Misc::Format(CDateHelper::GetDateOnly(data.AsDate()));
							break;

						case TDCCA_CALCULATION:
							sValue = m_formatter.GetTaskCustomAttributeData(m_aSelectedTaskIDs[0], *pDef);
							break;

						case TDCCA_TIMEPERIOD:
							sValue = pDef->FormatTimePeriod(data);
							break;

						case TDCCA_BOOL:
							sValue = (data.AsBool() ? _T("+") : _T(""));
							break;
						}
					}
				}
				else
				{
					bValueVaries = TRUE;
				}
			}
			else if (IsCustomTime(nAttribID))
			{
				const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
				GET_CUSTDEF_ALT(m_aCustomAttribDefs, MapTimeToDate(nAttribID), pDef, break);

				ASSERT(pDef->GetDataType() == TDCCA_DATE);
				TDCCADATA data;

				if (m_multitasker.GetTasksCustomAttributeData(m_aSelectedTaskIDs, *pDef, data))
				{
					if (!bValueVaries && !data.IsEmpty())
						sValue = Misc::Format(CDateHelper::GetTimeOnly(data.AsDate()));
				}
				else
				{
					bValueVaries = TRUE;
				}
			}
			break;
		}
	}

	// Use the item image index To distinguish between 
	// empty values and multiple differing values
	SetItemImage(nRow, (bValueVaries ? VALUE_VARIES : -1));
	SetItemText(nRow, VALUE_COL, sValue);
}

BOOL CTDLTaskAttributeListCtrl::GetButtonRect(int nRow, int nCol, CRect& rBtn) const
{
	if (!CInputListCtrl::GetButtonRect(nRow, nCol, rBtn))
		return FALSE;

	// Special handling
	TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);

	switch (nAttribID)
	{
	case TDCA_TIMESPENT:
		{
			rBtn.left -= EE_BTNWIDTH_ICON; // 'Track time' button
			rBtn.left -= EE_BTNWIDTH_ICON; // 'Add logged time' button
		}
		break;

	case TDCA_FILELINK:
		{
			if (!GetItemText(nRow, nCol).IsEmpty())
				rBtn.left -= EE_BTNWIDTH_ICON; // 'Browse' button

			rBtn.left -= EE_BTNWIDTH_ICON; // 'View File' button
		}
		break;

	case TDCA_DEPENDENCY:
		rBtn.left -= EE_BTNWIDTH_ICON; // 'Select Dependencies' button
		break;

	case TDCA_REMINDER:
		rBtn.left -= EE_BTNWIDTH_ICON; // 'Clear Reminder' button
		break;

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		{
			switch (m_aCustomAttribDefs.GetAttributeDataType(nAttribID))
			{
			case TDCCA_FILELINK:
				rBtn.left -= EE_BTNWIDTH_ICON; // 'View File' button
				break;
			}
		}
		break;
	}

	return TRUE;
}

BOOL CTDLTaskAttributeListCtrl::RowValueVaries(int nRow) const
{
	return (GetItemImage(nRow) == VALUE_VARIES);
}

BOOL CTDLTaskAttributeListCtrl::DrawIconButton(CDC* pDC, TDC_ATTRIBUTE nAttribID, int nBtnID, const CString& sText, DWORD dwBaseState, CRect& rBtn) const
{
	DWORD dwState = GetButtonState(nAttribID, nBtnID, sText, dwBaseState);
	HICON hIcon = GetButtonIcon(nAttribID, nBtnID, dwState);

	if (!hIcon)
	{
		ASSERT(0);
		return FALSE;
	}

	CRect rCellBtn(rBtn);

	if (nBtnID != ID_BTN_DEFAULT)
	{
		// We always get button zero because we advance the button rect each time LTR
		GetExtraButtonRect(rBtn, 0, rCellBtn);
	}

	CInputListCtrl::DrawIconButton(pDC, rCellBtn, hIcon, dwState);

	// Advance the button rect
	rBtn.SubtractRect(rBtn, rCellBtn);
	return TRUE;
}

BOOL CTDLTaskAttributeListCtrl::DrawButton(CDC* pDC, int nRow, int nCol, const CString& sText, BOOL bSelected, CRect& rBtn)
{
	if (!GetButtonRect(nRow, nCol, rBtn))
		return FALSE;

	// Some fields have multiple and/or icon buttons
	TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);
	IL_COLUMNTYPE nType = GetCellType(nRow, nCol);

	DWORD dwBaseState = CInputListCtrl::GetButtonState(nRow, nCol, bSelected);
	CRect rCellBtn(rBtn);

	if (nType == ILCT_CHECK)
	{
		if (RowValueVaries(nRow))
		{
			dwBaseState |= DFCS_MIXED;
		}
		else if (!sText.IsEmpty())
		{
			dwBaseState |= DFCS_CHECKED;
		}
	}
	else
	{
		switch (nAttribID)
		{
		case TDCA_DEPENDENCY:
			DrawIconButton(pDC, nAttribID, ID_BTN_VIEWDEPENDS, sText, dwBaseState, rCellBtn);
			break;		// Browse button drawn at bottom

		case TDCA_FILELINK:
			{
				DrawIconButton(pDC, nAttribID, ID_BTN_VIEWFILE, sText, dwBaseState, rCellBtn);

				if (sText.IsEmpty())
					return DrawIconButton(pDC, nAttribID, ID_BTN_DEFAULT, sText, dwBaseState, rCellBtn);
			
				// else
				DrawIconButton(pDC, nAttribID, ID_BTN_BROWSEFILE, sText, dwBaseState, rCellBtn);
			}
			break; // Combo button drawn at bottom

		case TDCA_TIMESPENT:
			{
				DrawIconButton(pDC, nAttribID, ID_BTN_TIMETRACK, sText, dwBaseState, rCellBtn);
				DrawIconButton(pDC, nAttribID, ID_BTN_ADDLOGGEDTIME, sText, dwBaseState, rCellBtn);

				// Time units button drawn at bottom
				dwBaseState = GetButtonState(nAttribID, ID_BTN_DEFAULT, sText, dwBaseState);
			}
			break;

		case TDCA_ICON:
			DrawIconButton(pDC, nAttribID, ID_BTN_DEFAULT, sText, dwBaseState, rCellBtn);
			return TRUE;

		case TDCA_REMINDER:
			DrawIconButton(pDC, nAttribID, ID_BTN_CLEARREMINDER, sText, dwBaseState, rCellBtn);
			DrawIconButton(pDC, nAttribID, ID_BTN_DEFAULT, sText, dwBaseState, rCellBtn);
			return TRUE;

		default:
			if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
			{
				const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
				GET_CUSTDEF_ALT(m_aCustomAttribDefs, nAttribID, pDef, break);

				switch (pDef->GetDataType())
				{
				case TDCCA_FILELINK:
					DrawIconButton(pDC, nAttribID, ID_BTN_VIEWFILE, sText, dwBaseState, rCellBtn);
					DrawIconButton(pDC, nAttribID, ID_BTN_BROWSEFILE, sText, dwBaseState, rCellBtn);
					return TRUE;

				case TDCCA_ICON:
					if (!pDef->IsList())
						return DrawIconButton(pDC, nAttribID, ID_BTN_DEFAULT, sText, dwBaseState, rCellBtn);
					break;
				}
			}
			break;
		}
	}

	// All else
	return CInputListCtrl::DrawButton(pDC, rCellBtn, nType, dwBaseState);
}

BOOL CTDLTaskAttributeListCtrl::GetCellPrompt(int nRow, const CString& sText, CString& sPrompt) const
{
	int nSelCount = m_aSelectedTaskIDs.GetSize();

	if (!nSelCount)
		return FALSE;

	BOOL bValueVaries = RowValueVaries(nRow);

	if (!bValueVaries)
	{
		TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);

		switch (nAttribID)
		{
		case TDCA_DUETIME:
			if (sText.IsEmpty())
			{
				// Only provide a prompt if the date value is set
				if (!GetItemText(nRow - 1, VALUE_COL).IsEmpty())
					sPrompt = CTimeHelper::FormatClockTime(23, 59, 0, FALSE, m_data.HasStyle(TDCS_SHOWDATESINISO));

				return TRUE; // Prevent default prompt
			}
			break;

		case TDCA_STARTTIME:
		case TDCA_DONETIME:
			if (sText.IsEmpty())
			{
				// Only provide a prompt if the date value is set
				if (!GetItemText(nRow - 1, VALUE_COL).IsEmpty())
					sPrompt = CTimeHelper::FormatClockTime(0, 0, 0, FALSE, m_data.HasStyle(TDCS_SHOWDATESINISO));

				return TRUE; // Prevent default prompt
			}
			break;

		case TDCA_REMINDER:
			if (sText == CEnString(IDS_REMINDER_DATENOTSET))
				sPrompt = sText;
			break;

		case TDCA_CATEGORY:
		case TDCA_TAGS:
		case TDCA_FILELINK:
		case TDCA_ALLOCTO:
		case TDCA_DEPENDENCY:
			bValueVaries = (sText.Find(MIXED_DELIM) != -1);
			break;

		default:
			if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
			{
				const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
				GET_CUSTDEF_RET(m_aCustomAttribDefs, nAttribID, pDef, FALSE);

				if (pDef->IsList())
					bValueVaries = (sText.Find(MIXED_DELIM) != -1);
			}
			else if (IsCustomTime(nAttribID) && sText.IsEmpty())
			{
				// Only provide a prompt if the date value is set
				if (!GetItemText(nRow - 1, VALUE_COL).IsEmpty())
					sPrompt = CTimeHelper::FormatClockTime(23, 59, 0, FALSE, m_data.HasStyle(TDCS_SHOWDATESINISO));
				
				return TRUE; // Prevent default prompt
			}
			break;
		}
	}

	if (bValueVaries)
		sPrompt = CEnString(IDS_EDIT_VALUEVARIES);

	return !sPrompt.IsEmpty();
}

UINT CTDLTaskAttributeListCtrl::GetTextDrawFlags(int nCol) const
{
	UINT nFlags = CInputListCtrl::GetTextDrawFlags(nCol);

	if (nCol == LABEL_COL)
		nFlags |= DT_END_ELLIPSIS;

	return nFlags;
}

void CTDLTaskAttributeListCtrl::DrawCellText(CDC* pDC, int nRow, int nCol, const CRect& rText, const CString& sText, COLORREF crText, UINT nDrawTextFlags)
{
	if ((nCol != VALUE_COL))
	{
		CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, sText, crText, nDrawTextFlags);
		return;
	}

	CString sPrompt;

	if (GetCellPrompt(nRow, sText, sPrompt))
	{
		if (!IsCellSelected(nRow, nCol))
			crText = CWndPrompt::GetTextColor();

		CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, sPrompt, crText, nDrawTextFlags);
		return;
	}

	// else attributes requiring custom rendering
	TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);

	switch (nAttribID)
	{
	case TDCA_STARTDATE:
	case TDCA_DUEDATE:
	case TDCA_DONEDATE:
		if (!sText.IsEmpty())
		{
			CString sDate(FormatDate(_ttof(sText), FALSE));

			if (CDateHelper::WantRTLDates())
				nDrawTextFlags |= DT_RTLREADING;

			CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, sDate, crText, nDrawTextFlags);
		}
		return;

	case TDCA_STARTTIME:
	case TDCA_DUETIME:
	case TDCA_DONETIME:
		{
			CString sDate(FormatTime(_ttof(sText), FALSE));
			CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, sDate, crText, nDrawTextFlags);
		}
		return;

	case TDCA_LASTMODDATE:
	case TDCA_CREATIONDATE:
		{
			CString sDate(FormatDate(_ttof(sText), TRUE));

			if (CDateHelper::WantRTLDates())
				nDrawTextFlags |= DT_RTLREADING;

			CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, sDate, crText, nDrawTextFlags);
		}
		return;

	case TDCA_TIMEESTIMATE:
	case TDCA_TIMESPENT:
		{
			TDCTIMEPERIOD tp;
			
			if (tp.Parse(sText))
			{
				if (tp.dAmount != 0.0)
					CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, tp.Format(2), crText, nDrawTextFlags);

				return;
			}
		}
		break;

	case TDCA_PRIORITY:
		if (!sText.IsEmpty())
		{
			int nPriority = _ttoi(sText);

			if ((nPriority >= 0) && m_aPriorityColors.GetSize())
			{
				nPriority = min(nPriority, (m_cbPriority.GetNumLevels() - 1));

				// Draw box
				CRect rBox(rText);
				rBox.DeflateRect(0, 3);
				rBox.right = rBox.left + rBox.Height();

				COLORREF crFill = m_aPriorityColors[nPriority];
				COLORREF crBorder = GraphicsMisc::Darker(crFill, 0.5);

				GraphicsMisc::DrawRect(pDC, rBox, crFill, crBorder);

				// Draw text
				CRect rLeft(rText);
				rLeft.left += rText.Height();

				CString sPriority = sText + Misc::Format(_T(" (%s)"), CEnString(m_aPriorityRiskStrResIDs[nPriority]));
				CInputListCtrl::DrawCellText(pDC, nRow, nCol, rLeft, sPriority, crText, nDrawTextFlags);
			}
		}
		return;

	case TDCA_RISK:
		if (!sText.IsEmpty())
		{
			int nRisk = _ttoi(sText);
			if (nRisk >= 0)
			{
				nRisk = min(nRisk, (m_cbRisk.GetNumLevels() - 1));

				CString sRisk = sText + Misc::Format(_T(" (%s)"), CEnString(m_aPriorityRiskStrResIDs[nRisk]));
				CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, sRisk, crText, nDrawTextFlags);
			}
		}
		return;

	case TDCA_ICON:
		{
			CRect rIcon(rText);

			if (DrawIcon(pDC, sText, rIcon, FALSE))
				CInputListCtrl::DrawCellText(pDC, nRow, nCol, rIcon, CTDLTaskIconDlg::GetUserIconName(sText), crText, nDrawTextFlags);
		}
		return;

	case TDCA_COST:
		CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, m_formatter.GetCost(Misc::Atof(sText)), crText, nDrawTextFlags);
		return;

	case TDCA_COLOR:
		CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, CEnString(IDS_COLOR_SAMPLETEXT), crText, nDrawTextFlags);
		return;

	case TDCA_ALLOCTO:
	case TDCA_CATEGORY:
	case TDCA_TAGS:
		{
			CString sChecked = Misc::SplitLeft(sText, MIXED_DELIM);

			if (!sChecked.IsEmpty())
			{
				sChecked.Replace(ITEM_DELIM, Misc::GetListSeparator());
				CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, sChecked, crText, nDrawTextFlags);

				return;
			}
		}
		break;

	case TDCA_FILELINK:
		{
			CStringArray aFiles;
			int nNumFiles = SplitValueArray(sText, aFiles);

			CRect rFile(rText);

			for (int nFile = 0; nFile < nNumFiles; nFile++)
			{
				CString sFile = aFiles[nFile];
				DrawIcon(pDC, sFile, rFile, TRUE);

				if (!TDCTASKLINK::IsTaskLink(sFile, TRUE))
					sFile = FileMisc::GetFileNameFromPath(sFile);

				if (nFile < (nNumFiles - 1))
					sFile += Misc::GetListSeparator() + ' ';

				CInputListCtrl::DrawCellText(pDC, nRow, nCol, rFile, sFile, crText, nDrawTextFlags);
				rFile.left += pDC->GetTextExtent(sFile).cx;

				if (rFile.left >= rText.right)
					break;
			}
		}
		return;

	case TDCA_DEPENDENCY:
		{
			CTDCDependencyArray aDepends;
			int nNumDepends = aDepends.Parse(sText);

			CRect rTitle(rText);

			for (int nDepend = 0; nDepend < nNumDepends; nDepend++)
			{
				DWORD dwDependsID = aDepends[nDepend].dwTaskID;

				CString sTitle = m_formatter.GetTaskTitlePath(dwDependsID, (TDCTF_TITLEONLY | TDCTF_TRAILINGID));
				CString sIcon = m_data.GetTaskIcon(dwDependsID);
	
				DrawIcon(pDC, m_data.GetTaskIcon(dwDependsID), rTitle, FALSE);

				if (nDepend < (nNumDepends - 1))
					sTitle += Misc::GetListSeparator() + ' ';

				CInputListCtrl::DrawCellText(pDC, nRow, nCol, rTitle, sTitle, crText, nDrawTextFlags);
				rTitle.left += pDC->GetTextExtent(sTitle).cx;
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
			case TDCCA_FILELINK:
				{
					CRect rRest(rText);
					DrawIcon(pDC, sText, rRest, TRUE);
	
					CInputListCtrl::DrawCellText(pDC, nRow, nCol, rRest, FileMisc::GetFileNameFromPath(sText), crText, nDrawTextFlags);
				}
				return;

			case TDCCA_ICON:
				{
					CString sIconName;
					CRect rIcon(rText);

					if (pDef->IsMultiList())
					{
						CString sMatched(sText), sUnused;
						Misc::Split(sMatched, sUnused, MIXED_DELIM);

						CStringArray aIcons;
						int nNumIcons = SplitValueArray(sMatched, aIcons);

						for (int nIcon = 0; nIcon < nNumIcons; nIcon++)
							DrawIcon(pDC, aIcons[nIcon], rIcon, FALSE);

						if (nNumIcons == 1)
						{
							if (!pDef->GetListIconName(sText, sIconName))
								sIconName = CTDLTaskIconDlg::GetUserIconName(sText);
						}
					}
					else
					{
						CString sImage;
						
						if (TDCCUSTOMATTRIBUTEDEFINITION::DecodeImageTag(sText, sImage, sIconName) &&
							DrawIcon(pDC, sImage, rIcon, FALSE))
						{
							if (sIconName.IsEmpty() && (!pDef->IsList() || !pDef->GetListIconName(sImage, sIconName)))
								sIconName = CTDLTaskIconDlg::GetUserIconName(sImage);
						}
					}

					CInputListCtrl::DrawCellText(pDC, nRow, nCol, rIcon, sIconName, crText, nDrawTextFlags);
				}
				return;

			case TDCCA_INTEGER:
			case TDCCA_DOUBLE:
				CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, pDef->FormatNumber(Misc::Atof(sText)), crText, nDrawTextFlags);
				return;

			case TDCCA_DATE:
				if (!sText.IsEmpty())
				{
					CString sDate(FormatDate(_ttof(sText), FALSE));

					if (CDateHelper::WantRTLDates())
						nDrawTextFlags |= DT_RTLREADING;

					CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, sDate, crText, nDrawTextFlags);
				}
				return;

			default:
				if (pDef->IsMultiList())
				{
					CString sChecked = Misc::SplitLeft(sText, MIXED_DELIM);

					if (!sChecked.IsEmpty())
					{
						sChecked.Replace(ITEM_DELIM, Misc::GetListSeparator());

						CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, sChecked, crText, nDrawTextFlags);
						return;
					}
				}
				break;
			}
		}
		else if (IsCustomTime(nAttribID))
		{
			CString sDate(FormatTime(_ttof(sText), FALSE));
			CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, sDate, crText, nDrawTextFlags);

			return;
		}
		break;
	}

	// All else
	CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, sText, crText, nDrawTextFlags);
}

CPoint CTDLTaskAttributeListCtrl::GetIconPos(const CRect& rText)
{
	return CPoint(rText.left - 1, rText.top + ((rText.Height() - ICON_SIZE) / 2));
}

BOOL CTDLTaskAttributeListCtrl::DrawIcon(CDC* pDC, const CString& sIcon, CRect& rIcon, BOOL bIconIsFile)
{
	if (sIcon.IsEmpty())
		return FALSE;

	CPoint ptIcon(GetIconPos(rIcon));
	BOOL bDrawn = FALSE;

	if (bIconIsFile)
	{
		bDrawn = CFileEdit::DrawFileIcon(pDC,
										 sIcon,
										 ptIcon,
										 m_iconCache,
										 this,
										 m_sCurrentFolder,
										 m_data.HasStyle(TDCS_SHOWFILELINKTHUMBNAILS));
	}
	else
	{
		bDrawn = m_ilIcons.Draw(pDC, sIcon, ptIcon, ILD_TRANSPARENT);
	}

	if (bDrawn)
		rIcon.left += (ICON_SIZE + 2);

	return bDrawn;
}

void CTDLTaskAttributeListCtrl::OnTextEditOK(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ((pDispInfo->item.iSubItem == VALUE_COL) &&
		(pDispInfo->item.iItem >= 0))
	{
		NotifyParentEdit(pDispInfo->item.iItem);
	}

	*pResult = 0;
}

LRESULT CTDLTaskAttributeListCtrl::OnSingleFileLinkNotifyBrowse(WPARAM wParam, LPARAM lParam)
{
	ASSERT(wParam && lParam);
	ASSERT(wParam == (WPARAM)m_eSingleFileLink.GetDlgCtrlID());

	int nRow = GetCurSel();
	ASSERT(nRow != CB_ERR);

	SetValueText(nRow, (LPCTSTR)lParam);

	return 0L;
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

CString CTDLTaskAttributeListCtrl::GetTaskTitle() const
{
	return GetValueText(TDCA_TASKNAME);
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
	return SplitValueArray(GetValueText(TDCA_FILELINK), aFiles);
}

CString CTDLTaskAttributeListCtrl::GetExternalID() const
{
	return GetValueText(TDCA_EXTERNALID);
}

int CTDLTaskAttributeListCtrl::GetPercentDone() const
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
	return GetDateTime(TDCA_STARTDATE);
}

COleDateTime CTDLTaskAttributeListCtrl::GetDueDate() const
{
	return GetDateTime(TDCA_DUEDATE);
}

COleDateTime CTDLTaskAttributeListCtrl::GetDoneDate() const
{
	return GetDateTime(TDCA_DONEDATE);
}

COleDateTime CTDLTaskAttributeListCtrl::GetStartTime() const
{
	return GetDateTime(TDCA_STARTTIME);
}

COleDateTime CTDLTaskAttributeListCtrl::GetDueTime() const
{
	return GetDateTime(TDCA_DUETIME);
}

COleDateTime CTDLTaskAttributeListCtrl::GetDoneTime() const
{
	return GetDateTime(TDCA_DONETIME);
}

COleDateTime CTDLTaskAttributeListCtrl::GetDateTime(TDC_ATTRIBUTE nAttribID) const
{
	CString sValue = GetValueText(nAttribID);

	return (sValue.IsEmpty() ? CDateHelper::NullDate() : _ttof(sValue));
}

int CTDLTaskAttributeListCtrl::GetCustomAttributeAutoListData(const CString& sAttribID, CStringArray& aItems) const
{
	const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
	GET_CUSTDEF_RET(m_aCustomAttribDefs, sAttribID, pDef, 0);

	ASSERT(pDef->IsAutoList());

	aItems.Copy(pDef->aAutoListData);
	return aItems.GetSize();
}

BOOL CTDLTaskAttributeListCtrl::GetCustomAttributeData(const CString& sAttribID, TDCCADATA& data, BOOL bFormatted) const
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
		COleDateTime date(_ttof(sValue));
		
		if (CDateHelper::IsDateSet(date))
		{
			if (pDef->HasFeature(TDCCAF_SHOWTIME))
			{
				TDC_ATTRIBUTE nTimeAttribID = CUSTOMTIMEATTRIBID(nAttribID);
				date.m_dt += _ttof(GetValueText(nTimeAttribID));
			}

			data.Set(date);
		}
	}
	else // Default handling
	{
		data.Set(sValue);
	}

	if (bFormatted && !data.IsEmpty())
		data.Set(pDef->FormatData(data, m_data.HasStyle(TDCS_SHOWDATESINISO)));

	return !data.IsEmpty();
}

TDC_ATTRIBUTE CTDLTaskAttributeListCtrl::MapTimeToDate(TDC_ATTRIBUTE nTimeAttribID) const
{
	switch (nTimeAttribID)
	{
	case TDCA_STARTTIME:	return TDCA_STARTDATE;
	case TDCA_DUETIME:		return TDCA_DUEDATE;
	case TDCA_DONETIME:		return TDCA_DONEDATE;

	default:
		if (IsCustomTime(nTimeAttribID))
		{
			TDC_ATTRIBUTE nDateAttribID = (TDC_ATTRIBUTE)(nTimeAttribID - CUSTOMTIMEATTRIBOFFSET);
#ifdef _DEBUG
			const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
			GET_CUSTDEF_ALT(m_aCustomAttribDefs, nDateAttribID, pDef, break);

			ASSERT(pDef->IsDataType(TDCCA_DATE) && pDef->HasFeature(TDCCAF_SHOWTIME));
#endif
			return nDateAttribID;
		}
		break;
	}

	return TDCA_NONE;
}

BOOL CTDLTaskAttributeListCtrl::IsCustomTime(TDC_ATTRIBUTE nAttribID)
{
	return (nAttribID > CUSTOMTIMEATTRIBOFFSET);
}

CString CTDLTaskAttributeListCtrl::FormatMultiSelItems(const CStringArray& aMatched, const CStringArray& aMixed)
{
	CString sValue = FormatValueArray(aMatched);
	
	if (aMixed.GetSize())
		sValue += (MIXED_DELIM + FormatValueArray(aMixed));

	return sValue;
}

int CTDLTaskAttributeListCtrl::ParseMultiSelValues(const CString& sValues, CStringArray& aMatched, CStringArray& aMixed)
{
	CString sMatched(sValues), sMixed;

	Misc::Split(sMatched, sMixed, MIXED_DELIM);
	SplitValueArray(sMatched, aMatched);
	SplitValueArray(sMixed, aMixed);

	return aMatched.GetSize();
}

BOOL CTDLTaskAttributeListCtrl::CheckRecreateCombo(int nRow, CEnCheckComboBox& combo, BOOL bWantSort)
{
	BOOL bIsReadOnly = !CDialogHelper::ComboHasEdit(combo), bWantReadOnly(bIsReadOnly);
	BOOL bIsSorted = CDialogHelper::HasStyle(combo, CBS_SORT);

	TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);

	switch (nAttribID)
	{
	case TDCA_ALLOCBY:
	case TDCA_STATUS:
	case TDCA_VERSION:
	case TDCA_ALLOCTO:
	case TDCA_CATEGORY:
	case TDCA_TAGS:
		bWantReadOnly = m_mapReadOnlyListData.Has(nAttribID);
		break;

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		{
			const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
			GET_CUSTDEF_RET(m_aCustomAttribDefs, nAttribID, pDef, NULL);

			bWantReadOnly = pDef->IsFixedList();
		}
		break;
	}

	if (Misc::StatesDiffer(bIsReadOnly, bWantReadOnly))
	{
		// Update the combo sort style for when it's recreated
		combo.ModifyStyle(bWantSort ? 0 : CBS_SORT, bWantSort ? CBS_SORT : 0);

		if (!CDialogHelper::SetAutoComboReadOnly(combo, TRUE, bWantReadOnly, COMBO_DROPHEIGHT))
		{
			ASSERT(0);
			return FALSE;
		}

		combo.ModifyFlags((bWantReadOnly ? (ACBS_ALLOWDELETE | ACBS_AUTOCOMPLETE) : 0),
							(bWantReadOnly ? 0 : (ACBS_ALLOWDELETE | ACBS_AUTOCOMPLETE)));
	}
	else if (Misc::StatesDiffer(bIsSorted, bWantSort))
	{
		int nCtrlID = combo.GetDlgCtrlID();

		DWORD dwStyle = combo.GetStyle();
		Misc::SetFlag(dwStyle, CBS_SORT, bWantSort);

		combo.DestroyWindow();
		
		if (!CreateControl(combo, nCtrlID, dwStyle))
		{
			ASSERT(0);
			return FALSE;
		}
	}
	return TRUE;
}

void CTDLTaskAttributeListCtrl::RebuildCombo(CEnCheckComboBox& combo, const CStringArray& aDefValues, const CStringArray& aUserValues, BOOL bMultiSel)
{
	combo.EnableMultiSelection(bMultiSel);

	CStringArray aAllValues;
	
	aAllValues.Copy(aDefValues);
	aAllValues.Append(aUserValues);
	Misc::RemoveDuplicates(aAllValues);

	CDialogHelper::SetComboBoxItems(combo, aAllValues);
}

void CTDLTaskAttributeListCtrl::PrepareMultiSelCombo(int nRow, const CStringArray& aDefValues, const CStringArray& aUserValues, CEnCheckComboBox& combo, BOOL bWantSort)
{
	// Populate
	CheckRecreateCombo(nRow, combo, bWantSort);
	RebuildCombo(combo, aDefValues, aUserValues, TRUE);

	// Set selection
	CStringArray aMatched, aMixed;
	ParseMultiSelValues(GetItemText(nRow, VALUE_COL), aMatched, aMixed);

	combo.SetChecked(aMatched, aMixed);
}

void CTDLTaskAttributeListCtrl::PrepareSingleSelCombo(int nRow, const CStringArray& aDefValues, const CStringArray& aUserValues, CEnCheckComboBox& combo, BOOL bWantSort)
{
	// Populate
	CheckRecreateCombo(nRow, combo, bWantSort);
	RebuildCombo(combo, aDefValues, aUserValues, FALSE);

	// Set selection
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
	m_editBox.ClearMask();

	BOOL bCheckWantMultilineEdit = FALSE;

	switch (nAttribID)
	{
	case TDCA_TASKNAME:
		bCheckWantMultilineEdit = TRUE;
		break;

	case TDCA_ALLOCBY:	PrepareSingleSelCombo(nRow, m_tldDefault.aAllocBy, m_tldAll.aAllocBy, m_cbTextAndNumbers);	break;
	case TDCA_STATUS: 	PrepareSingleSelCombo(nRow, m_tldDefault.aStatus, m_tldAll.aStatus, m_cbTextAndNumbers);	break;
	case TDCA_VERSION: 	PrepareSingleSelCombo(nRow, m_tldDefault.aVersion, m_tldAll.aVersion, m_cbTextAndNumbers);	break;

	case TDCA_ALLOCTO:	PrepareMultiSelCombo(nRow, m_tldDefault.aAllocTo, m_tldAll.aAllocTo, m_cbTextAndNumbers);	break;
	case TDCA_CATEGORY: PrepareMultiSelCombo(nRow, m_tldDefault.aCategory, m_tldAll.aCategory, m_cbTextAndNumbers);	break;
	case TDCA_TAGS:		PrepareMultiSelCombo(nRow, m_tldDefault.aTags, m_tldAll.aTags, m_cbTextAndNumbers);			break;

	case TDCA_FILELINK:
		{
			CString sValue = GetItemText(nRow, nCol);

			if (sValue.IsEmpty())
			{
				PrepareSingleFileEdit(nRow, sValue);
			}
			else
			{
				CStringArray aFiles;

				if (SplitValueArray(sValue, aFiles))
					m_cbMultiFileLink.SetFileList(aFiles);

				m_cbMultiFileLink.SetCurrentFolder(m_sCurrentFolder);
				m_cbMultiFileLink.EnableEditStyle(FES_DISPLAYIMAGETHUMBNAILS, m_data.HasStyle(TDCS_SHOWFILELINKTHUMBNAILS));
			}
		}
		break;

	case TDCA_DEPENDENCY:
		{
			CTDCDependencyArray aDepends;
			m_multitasker.GetTasksDependencies(m_aSelectedTaskIDs, aDepends);

			m_eDepends.SetDependencies(aDepends);
		}
		break;

	case TDCA_ICON: 
	case TDCA_FLAG: 
	case TDCA_LOCK: 
	case TDCA_COLOR:
	case TDCA_RECURRENCE:
	case TDCA_EXTERNALID: 
		break; // Nothing to do

	case TDCA_PERCENT:
		m_editBox.SetMask(_T("0123456789"));
		m_editBox.SetSpinBuddy(&m_spinPercent);
		m_spinPercent.SetRange32(0, 100);
		break;

	case TDCA_COST:
		m_editBox.SetMask(_T("-0123456789.@"), ME_LOCALIZEDECIMAL);
		break;

	case TDCA_PRIORITY:
		{
			if (RowValueVaries(nRow))
				m_cbPriority.SetCurSel(CB_ERR);
			else
				m_cbPriority.SetSelectedPriority(_ttoi(GetItemText(nRow, nCol)));
		}
		break;

	case TDCA_RISK:
		{
			if (RowValueVaries(nRow))
				m_cbRisk.SetCurSel(CB_ERR);
			else
				m_cbRisk.SetSelectedRisk(_ttoi(GetItemText(nRow, nCol)));
		}
		break;

	case TDCA_TIMEESTIMATE:
		PrepareTimePeriodEdit(nRow);
		break;

	case TDCA_TIMESPENT:
		{
			PrepareTimePeriodEdit(nRow);

			// Insert before default menu button
			m_eTimePeriod.InsertButton(0, ID_BTN_ADDLOGGEDTIME, GetButtonIcon(ICON_ADDTIME), CEnString(IDS_TDC_ADDLOGGEDTIME));

			// Insert before 'Add logged time button'
			m_eTimePeriod.InsertButton(0, ID_BTN_TIMETRACK, GetButtonIcon(ICON_TRACKTIME), CEnString(IDS_TDC_STARTSTOPCLOCK));
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
			m_multitasker.GetTasksCustomAttributeData(m_aSelectedTaskIDs, *pDef, data);

			if (pDef->IsList())
			{
				BOOL bWantSort = !pDef->IsFixedList();

				switch (pDef->GetDataType())
				{
				case TDCCA_STRING:
				case TDCCA_FRACTION:
				case TDCCA_INTEGER:
				case TDCCA_DOUBLE:
					if (pDef->IsMultiList())
						PrepareMultiSelCombo(nRow, pDef->aDefaultListData, pDef->aAutoListData, m_cbTextAndNumbers, bWantSort);
					else
						PrepareSingleSelCombo(nRow, pDef->aDefaultListData, pDef->aAutoListData, m_cbTextAndNumbers, bWantSort);
					break;

				case TDCCA_ICON:
					if (pDef->IsMultiList())
						PrepareMultiSelCombo(nRow, pDef->aDefaultListData, pDef->aAutoListData, m_cbCustomIcons, bWantSort);
					else
						PrepareSingleSelCombo(nRow, pDef->aDefaultListData, pDef->aAutoListData, m_cbCustomIcons, bWantSort);
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
				case TDCCA_FRACTION:
					m_editBox.SetMask(_T("-0123456789/"));
					break;

				case TDCCA_INTEGER:
					m_editBox.SetMask(_T("-0123456789"));
					m_editBox.SetSpinBuddy(&m_spinPercent);
					m_spinPercent.SetRange32(INT_MIN, INT_MAX);
					break;

				case TDCCA_DOUBLE:
					m_editBox.SetMask(_T("-0123456789."), ME_LOCALIZEDECIMAL);
					break;

				case TDCCA_STRING:
					bCheckWantMultilineEdit = TRUE;
					break;

				case TDCCA_ICON:
				case TDCCA_BOOL:
					break; // Handled by CInputListCtrl

				case TDCCA_FILELINK:
					PrepareSingleFileEdit(nRow, GetItemText(nRow, nCol));
					break;

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

	if (&ctrl == &m_editBox)
	{
		BOOL bMultiline = FALSE;

		if (bCheckWantMultilineEdit)
		{
			CRect rClient;
			GetClientRect(rClient);

			int nMaxWidth = rClient.Width();
			int nTextWidth = GraphicsMisc::GetTextWidth(GetItemText(nRow, nCol), *this);

			bMultiline = (nTextWidth >= min(nMaxWidth, MAX_EDIT_WIDTH));
		}

		VERIFY(CInputListCtrl::CheckRecreateEditControl(bMultiline));
	}
}

void CTDLTaskAttributeListCtrl::PrepareSingleFileEdit(int nRow, const CString& sValue)
{
	m_eSingleFileLink.SetWindowText(sValue);
	m_eSingleFileLink.SetCurrentFolder(m_sCurrentFolder);
	m_eSingleFileLink.EnableStyle(FES_DISPLAYIMAGETHUMBNAILS, m_data.HasStyle(TDCS_SHOWFILELINKTHUMBNAILS));
}

void CTDLTaskAttributeListCtrl::PrepareDatePicker(int nRow, TDC_ATTRIBUTE nFallbackDate)
{
	CString sValue = GetItemText(nRow, VALUE_COL);

	if ((sValue.IsEmpty() || (sValue == DATETIME_VARIES)) && 
		(nFallbackDate != TDCA_NONE) && 
		(TDC::MapAttributeToDate(nFallbackDate) != TDCD_NONE))
	{
		sValue = GetValueText(nFallbackDate);
	}

	if (sValue.IsEmpty() || (sValue == DATETIME_VARIES))
		m_datePicker.SendMessage(DTM_SETSYSTEMTIME, GDT_NONE, 0);
	else
		m_datePicker.SetTime(_ttof(sValue));

	m_datePicker.SetMonthCalStyle(MCS_WEEKNUMBERS);
	m_datePicker.SetISOFormat(m_data.HasStyle(TDCS_SHOWDATESINISO));
}

void CTDLTaskAttributeListCtrl::PrepareTimeOfDayCombo(int nRow)
{
	CString sValue = GetItemText(nRow, VALUE_COL);

	if (sValue.IsEmpty() || (sValue == DATETIME_VARIES))
		m_cbTimeOfDay.SetOleTime(-1);
	else
		m_cbTimeOfDay.SetOleTime(_ttof(sValue));

	m_cbTimeOfDay.SetISOFormat(m_data.HasStyle(TDCS_SHOWDATESINISO));
}

void CTDLTaskAttributeListCtrl::PrepareTimePeriodEdit(int nRow)
{
	CString sValue = GetItemText(nRow, VALUE_COL);

	TH_UNITS nUnits = THU_NULL;
	double dValue = 0.0;

	if (!RowValueVaries(nRow))
		CTimeHelper::DecodeOffset(sValue, dValue, nUnits, FALSE);

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
	case TDCA_TIMESPENT:
	case TDCA_REMINDER:
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
				case TDCCA_FILELINK:
					return NULL; // Handled in EditCell()

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
			GET_CUSTDEF_RET(m_aCustomAttribDefs, MapTimeToDate(nAttribID), pDef, NULL);

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

void CTDLTaskAttributeListCtrl::GetCellEditRect(int nRow, int nCol, CRect& rCell) const
{
	ASSERT(nCol == VALUE_COL);

	CInputListCtrl::GetCellEditRect(nRow, nCol, rCell);

	// Ensure a big-enough edit field for 'free-text'
	BOOL bCheckTextWidth = FALSE;

	TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);
	switch (nAttribID)
	{
	case TDCA_TASKNAME:
		bCheckTextWidth = TRUE;
		break;

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		{
			const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
			GET_CUSTDEF_ALT(m_aCustomAttribDefs, nAttribID, pDef, break);

			bCheckTextWidth = (!pDef->IsList() && (pDef->GetDataType() == TDCCA_STRING));
		}
		break;
	}

	if (bCheckTextWidth)
	{
		// Prevent child edit extending beyond visible client 
		// rect else those parts will not be visible
		CRect rScreen, rClient;

		GraphicsMisc::GetAvailableScreenSpace(*this, rScreen);
		ScreenToClient(rScreen);

		GetClientRect(rClient);
		rClient.IntersectRect(rClient, rScreen);

		// Progressively enlarge the edit box until the maximum is reached
		int nMaxWidth = rClient.Width();
		int nCellWidth = rCell.Width();

		CRect rEdit(rCell);
		rEdit.right = (rEdit.left + max(nCellWidth, min(nMaxWidth, MIN_EDIT_WIDTH)));

		CString sValue = GetItemText(nRow, VALUE_COL);
		int nTextWidth = GraphicsMisc::GetTextWidth(sValue, *this);

		if (nTextWidth >= rEdit.Width())
		{
			rEdit.right = (rEdit.left + max(nCellWidth, min(nMaxWidth, MAX_EDIT_WIDTH)));

			if (nTextWidth > MAX_EDIT_WIDTH)
			{
				// Make the height not a row-multiple so it's clearly 
				// distinguishable from the attributes behind
				rEdit.bottom += ((GetItemHeight() * 5) / 2);
			}
		}

		// Fit to visible client rect
		GraphicsMisc::FitRect(rEdit, rClient);
		rCell = rEdit;
	}
}

void CTDLTaskAttributeListCtrl::EditCell(int nRow, int nCol, BOOL bBtnClick)
{
	ASSERT(CanEditCell(nRow, nCol));

	m_bTaskIDChangeSinceLastEdit = FALSE;

	CWnd* pCtrl = GetEditControl(nRow, bBtnClick);

	if (pCtrl != NULL)
	{
		if (pCtrl == CInputListCtrl::GetEditControl())
		{
			PrepareControl(*pCtrl, nRow, nCol);
			CInputListCtrl::EditCell(nRow, nCol, bBtnClick);
		}
		else if (pCtrl == &m_eTimePeriod)
		{
			HandleTimePeriodEdit(nRow, bBtnClick);
		}
		else
		{
			ShowControl(*pCtrl, nRow, nCol, bBtnClick);
		}

		return;
	}

	// All other attributes not otherwise handled
	TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);
	CString sValue = GetItemText(nRow, VALUE_COL);

	switch (nAttribID)
	{
	case TDCA_FLAG:
	case TDCA_LOCK:
		// Toggle checkbox
		SetValueText(nRow, (sValue.IsEmpty() ? _T("+") : _T("")));
;		break;

	case TDCA_ICON:
	case TDCA_COLOR:
	case TDCA_RECURRENCE:
		if (GetParent()->SendMessage(WM_TDCM_EDITTASKATTRIBUTE, nAttribID))
			RefreshSelectedTasksValue(nRow);
		break;

	case TDCA_DEPENDENCY:
		if (bBtnClick)
		{
			int nBtnID = HitTestButtonID(nRow);
			ASSERT(nBtnID != ID_BTN_NONE);

			if (!CanClickButton(nAttribID, nBtnID, sValue))
				break;

			if (nBtnID == ID_BTN_VIEWDEPENDS)
			{
				OnEnEditButtonClick(0, nBtnID);
			}
			else
			{
				PrepareControl(m_eDepends, nRow, nCol);

				if (OnEnEditButtonClick(0, ID_BTN_EDITDEPENDS))
					RefreshSelectedTasksValue(nRow);
			}
		}
		else
		{
			CInputListCtrl::EditCell(nRow, nCol, bBtnClick);
		}
		break;

	case TDCA_REMINDER:
		if (bBtnClick)
		{
			int nBtnID = HitTestButtonID(nRow);
			ASSERT(nBtnID != ID_BTN_NONE);

			if (!CanClickButton(nAttribID, nBtnID, sValue))
				break;

			if (nBtnID == ID_BTN_CLEARREMINDER)
			{
				GetParent()->SendMessage(WM_TDCM_CLEARTASKREMINDER);
				break;
			}
		}
		// else
		GetParent()->SendMessage(WM_TDCM_EDITTASKREMINDER);
		break;

	case TDCA_TIMESPENT:
		if (bBtnClick)
		{
			// Check if one of the sub-buttons was clicked
			int nBtnID = HitTestButtonID(nRow);
			ASSERT(nBtnID != ID_BTN_NONE);

			if (!CanClickButton(nAttribID, nBtnID, sValue))
				break;

			if (nBtnID == ID_BTN_DEFAULT)
				HandleTimePeriodEdit(nRow, bBtnClick);
			else
				OnEnEditButtonClick(0, nBtnID);
		}
		else if (CanClickButton(nAttribID, ID_BTN_DEFAULT, sValue))
		{
			// If the time units button is disabled then also 
			// disallow editing of the time value
			HandleTimePeriodEdit(nRow, bBtnClick);
		}
		break;

	case TDCA_FILELINK:
		{
			// Check if one of the sub-buttons was clicked
			int nBtnID = (bBtnClick ? HitTestButtonID(nRow) : ID_BTN_NONE);
			ASSERT(!bBtnClick || (nBtnID != ID_BTN_NONE));

			if (bBtnClick && !CanClickButton(nAttribID, nBtnID, sValue))
				break;

			if (sValue.IsEmpty()) // no combo button
			{
				if (nBtnID == ID_BTN_DEFAULT) // Browse
				{
					m_eSingleFileLink.SetCurrentFolder(m_sCurrentFolder);
					m_eSingleFileLink.DoBrowse();
				}
				else
				{
					ShowControl(m_eSingleFileLink, nRow, VALUE_COL, bBtnClick);
				}
			}
			else
			{
				PrepareControl(m_cbMultiFileLink, nRow, nCol);

				switch (nBtnID)
				{
				case ID_BTN_BROWSEFILE:
					m_cbMultiFileLink.DoBrowse();
					return;

				case ID_BTN_VIEWFILE:
					GetParent()->SendMessage(WM_TDCM_DISPLAYLINK, (WPARAM)GetSafeHwnd(), (LPARAM)(LPCTSTR)m_cbMultiFileLink.GetFirstFile());
					return;
				}

				// else
				ShowControl(m_cbMultiFileLink, nRow, nCol, bBtnClick);
			}
		}
		break;

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		{
			const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
			GET_CUSTDEF_ALT(m_aCustomAttribDefs, nAttribID, pDef, break);

			// Custom attributes not handled by default
			if (!pDef->IsList())
			{
				switch (pDef->GetDataType())
				{
				case TDCCA_FILELINK:
					if (bBtnClick)
					{
						int nBtnID = HitTestButtonID(nRow);
						ASSERT(nBtnID != ID_BTN_NONE);

						switch (nBtnID)
						{
						case ID_BTN_DEFAULT:
							m_eSingleFileLink.SetCurrentFolder(m_sCurrentFolder);
							m_eSingleFileLink.DoBrowse(sValue);
							return;

						case ID_BTN_VIEWFILE:
							if (!sValue.IsEmpty())
								GetParent()->SendMessage(WM_TDCM_DISPLAYLINK, (WPARAM)GetSafeHwnd(), (LPARAM)(LPCTSTR)sValue);
							return;
						}
					}
					// else
					ShowControl(m_eSingleFileLink, nRow, VALUE_COL, FALSE);
					break;

				case TDCCA_ICON:
					if (GetParent()->SendMessage(WM_TDCM_EDITTASKATTRIBUTE, nAttribID))
						RefreshSelectedTasksValue(nRow);
					break;

				case TDCCA_BOOL:
					// Toggle checkbox
					SetValueText(nRow, (sValue.IsEmpty() ? _T("+") : _T("")));
					break;
				}
			}
		}
		break;
	}
}

int CTDLTaskAttributeListCtrl::HitTestButtonID(int nRow) const
{
	CRect rBtn;
	
	if (!GetButtonRect(nRow, VALUE_COL, rBtn))
		return ID_BTN_NONE;

	return HitTestButtonID(nRow, rBtn);
}

int CTDLTaskAttributeListCtrl::HitTestButtonID(int nRow, const CRect& rBtn) const
{
	CPoint ptMouse(GetMessagePos());
	ScreenToClient(&ptMouse);

	if (!rBtn.PtInRect(ptMouse))
		return ID_BTN_NONE;

	TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);

	switch (nAttribID)
	{
	case TDCA_TIMESPENT:
		{
			// Check if one of the sub-buttons was clicked
			switch (HitTestExtraButton(nRow, rBtn, ptMouse, 2))
			{
			case 0: return ID_BTN_TIMETRACK;
			case 1: return ID_BTN_ADDLOGGEDTIME;
			}
		}
		break;

	case TDCA_FILELINK:
		{
			int nNumBtns = (GetItemText(nRow, VALUE_COL).IsEmpty() ? 1 : 2);
			
			switch (HitTestExtraButton(nRow, rBtn, ptMouse, nNumBtns))
			{
			case 0: return ID_BTN_VIEWFILE;
			case 1: return ID_BTN_BROWSEFILE;
			}
		}
		break;

	case TDCA_DEPENDENCY:
		{
			switch (HitTestExtraButton(nRow, rBtn, ptMouse, 1))
			{
			case 0: return ID_BTN_VIEWDEPENDS;
			}
		}
		break;

	case TDCA_REMINDER:
		{
			switch (HitTestExtraButton(nRow, rBtn, ptMouse, 1))
			{
			case 0: return ID_BTN_CLEARREMINDER;
			}
		}
		break;

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID) &&
			m_aCustomAttribDefs.GetAttributeDataType(nAttribID) == TDCCA_FILELINK)
		{
			switch (HitTestExtraButton(nRow, rBtn, ptMouse, 1))
			{
			case 0: return ID_BTN_VIEWFILE;
			}
		}
		break;
	}

	// all else
	return ID_BTN_DEFAULT;
}

DWORD CTDLTaskAttributeListCtrl::GetButtonState(TDC_ATTRIBUTE nAttribID, int nBtnID, const CString& sCellText, DWORD dwBaseState) const
{
	if (Misc::HasFlag(dwBaseState, DFCS_INACTIVE))
		return dwBaseState;

	if (!CanClickButton(nAttribID, nBtnID, sCellText))
		return (dwBaseState | DFCS_INACTIVE);

	// else
	switch (nBtnID)
	{
	case ID_BTN_TIMETRACK:
		if ((m_aSelectedTaskIDs.GetSize() == 1) && (m_aSelectedTaskIDs[0] == m_dwTimeTrackingTask))
			return (dwBaseState | DFCS_PUSHED);
		break;
	}

	return dwBaseState;
}

HICON CTDLTaskAttributeListCtrl::GetButtonIcon(int nIconID, BOOL bDisabled) const
{
	CString sIcon = Misc::Format(nIconID);

	if (!m_iconCache.HasIcon(sIcon))
		m_iconCache.Add(sIcon, (UINT)nIconID);

	return m_iconCache.GetIcon(sIcon, bDisabled);
}

HICON CTDLTaskAttributeListCtrl::GetButtonIcon(TDC_ATTRIBUTE nAttribID, int nBtnID, DWORD dwState) const
{
	ASSERT(CanEditCell(GetRow(nAttribID), VALUE_COL));

	BOOL bDisabled = Misc::HasFlag(dwState, DFCS_INACTIVE);

	switch (nBtnID)
	{
	case ID_BTN_TIMETRACK:		return GetButtonIcon(ICON_TRACKTIME, bDisabled);
	case ID_BTN_ADDLOGGEDTIME:	return GetButtonIcon(ICON_ADDTIME, bDisabled);
	case ID_BTN_VIEWDEPENDS:	return GetButtonIcon(ICON_SHOWDEPENDS, bDisabled);
	case ID_BTN_VIEWFILE:		return GetButtonIcon(ICON_VIEWFILE, bDisabled);
	case ID_BTN_BROWSEFILE:		return GetButtonIcon(ICON_BROWSEFILE, bDisabled);
	case ID_BTN_CLEARREMINDER:	return GetButtonIcon(ICON_CLEARREMINDER, bDisabled);

	case ID_BTN_DEFAULT:
		switch (nAttribID)
		{
		case TDCA_ICON:			return GetButtonIcon(ICON_SELECTICON, bDisabled);
		case TDCA_REMINDER:		return GetButtonIcon(ICON_REMINDER, bDisabled);
		case TDCA_FILELINK:		return GetButtonIcon(ICON_BROWSEFILE, bDisabled);

		default:
			if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
			{
				switch (m_aCustomAttribDefs.GetAttributeDataType(nAttribID))
				{
				case TDCCA_ICON:
					return GetButtonIcon(ICON_SELECTICON, bDisabled);
				}
			}
		}
		break;

	default:
		ASSERT(0);
		break;
	}

	return NULL;
}

DWORD CTDLTaskAttributeListCtrl::GetSingleSelectedTaskID() const
{
	return ((m_aSelectedTaskIDs.GetSize() == 1) ? m_aSelectedTaskIDs[0] : 0);
}

BOOL CTDLTaskAttributeListCtrl::CanClickButton(TDC_ATTRIBUTE nAttribID, int nBtnID, const CString& sCellText) const
{
	ASSERT(CanEditCell(GetRow(nAttribID), VALUE_COL));

	DWORD dwSingleTaskID = GetSingleSelectedTaskID();

	switch (nBtnID)
	{
	case ID_BTN_TIMETRACK:
		return (dwSingleTaskID && !m_calculator.IsTaskDone(dwSingleTaskID)); // Include 'good as done'

	case ID_BTN_ADDLOGGEDTIME:
		return (dwSingleTaskID && (dwSingleTaskID != m_dwTimeTrackingTask));

	case ID_BTN_VIEWDEPENDS:
	case ID_BTN_VIEWFILE:
	case ID_BTN_CLEARREMINDER:
		return !sCellText.IsEmpty();

	case ID_BTN_EDITDEPENDS:
	case ID_BTN_BROWSEFILE:
		return TRUE;

	case ID_BTN_DEFAULT:
		switch (nAttribID)
		{
		case TDCA_TIMESPENT:
			return (!dwSingleTaskID || (dwSingleTaskID != m_dwTimeTrackingTask));
		}
		return TRUE;

	default:
		ASSERT(0);
		break;
	}

	return FALSE;
}

int CTDLTaskAttributeListCtrl::HitTestExtraButton(int nRow, const CRect& rBtn, const CPoint& ptMouse, int nNumExtraBtns)
{
	ASSERT(nNumExtraBtns > 0);
	CRect rExtraBtn;

	for (int nExtraBtn = 0; (nExtraBtn < nNumExtraBtns) && GetExtraButtonRect(rBtn, nExtraBtn, rExtraBtn); nExtraBtn++)
	{
		if (rExtraBtn.PtInRect(ptMouse))
			return nExtraBtn;
	}

	// else
	return -1;
}

BOOL CTDLTaskAttributeListCtrl::GetExtraButtonRect(const CRect& rBtn, int nSubBtn, CRect& rSubBtn)
{
	rSubBtn = rBtn;
	rSubBtn.left += (nSubBtn * EE_BTNWIDTH_ICON);
	rSubBtn.right = (rSubBtn.left + EE_BTNWIDTH_ICON);

	return rSubBtn.IntersectRect(rSubBtn, rBtn);
}

void CTDLTaskAttributeListCtrl::HandleTimePeriodEdit(int nRow, BOOL bBtnClick)
{
	ShowControl(m_eTimePeriod, nRow, VALUE_COL, FALSE);

	if (bBtnClick)
	{
		m_eTimePeriod.ShowUnitsPopupMenu(); // modal loop

		if (m_eTimePeriod.HasValidTime())
			HideControl(m_eTimePeriod);
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
	{
		m_editBox.SetSpinBuddy(NULL);
		HideControl(m_spinPercent, NULL);
	}
}

void CTDLTaskAttributeListCtrl::OnTimeOfDaySelEndOK()
{
	OnComboSelChange(IDC_TIME_PICKER);
}

void CTDLTaskAttributeListCtrl::OnComboCloseUp(UINT nCtrlID) 
{ 
	CWnd* pCombo = GetDlgItem(nCtrlID);

	// Note: our base class may already have hidden 
	// the combo so we have to check first
	if (pCombo->IsWindowVisible())
	{
		// If the combo has an edit field AND the user clicked inside 
		// the edit field to close the combo, DON'T hide the combo
		CWnd* pEdit = pCombo->GetDlgItem(1001);

		if (pEdit && CDialogHelper::IsMouseDownInWindow(*pEdit))
			return;

		// Else if this is the core File Link field and the user
		// clicked on one of the buttons (except for the drop button),
		// perform the required action before hiding the combo
		if ((pCombo == &m_cbMultiFileLink) && CDialogHelper::IsMouseDownInWindow(*pCombo))
		{
			int nRow = GetRow(TDCA_FILELINK);

			if (HitTestButtonID(nRow) != ID_BTN_DEFAULT)
				EditCell(nRow, VALUE_COL, TRUE);
		}
	}

	// All else
	HideControl(*pCombo);
}

void CTDLTaskAttributeListCtrl::OnComboKillFocus(UINT nCtrlID)
{
	HideControl(*GetDlgItem(nCtrlID));

	// Extra handling
	switch (nCtrlID)
	{
	case IDC_TIME_PICKER:
		OnComboSelChange(nCtrlID);
		break;
	}
}

void CTDLTaskAttributeListCtrl::OnComboSelChange(UINT nCtrlID)
{
	int nRow = GetCurSel();
	CString sNewValue;

	switch (nCtrlID)
	{
	case IDC_TEXTANDNUM_COMBO:
		if (m_cbTextAndNumbers.IsMultiSelectionEnabled())
		{
			CStringArray aMatched, aMixed;
			m_cbTextAndNumbers.GetChecked(aMatched, aMixed);

			sNewValue = FormatMultiSelItems(aMatched, aMixed);
		}
		else
		{
			sNewValue = CDialogHelper::GetSelectedItem(m_cbTextAndNumbers);
		}
		break;

	case IDC_TIME_PICKER:
		{
			double dTime = m_cbTimeOfDay.GetOleTime();

			if (dTime != 0.0)
				sNewValue = Misc::Format(dTime);

			// If the combo is visible, use the cell value as a 
			// scratch pad but WITHOUT notifying the parent
			if (m_cbTimeOfDay.IsWindowVisible())
			{
				SetItemText(nRow, VALUE_COL, sNewValue);
				return;
			}

			// If we've got multiple different values, DON'T notify
			// our parent if the cell text has not actually changed
			if (RowValueVaries(nRow) && (GetItemText(nRow, VALUE_COL) == DATETIME_VARIES))
			{
				return;
			}

			// Revert any scratch pad changes before default handling
			RefreshSelectedTasksValue(nRow);
		}
		break;

	case IDC_PRIORITY_COMBO:
		sNewValue = Misc::Format(m_cbPriority.GetSelectedPriority());
		break;

	case IDC_RISK_COMBO:
		sNewValue = Misc::Format(m_cbRisk.GetSelectedRisk());
		break;

	case IDC_FILELINK_COMBO:
		{
			CStringArray aFiles;
			
			if (m_cbMultiFileLink.GetFileList(aFiles))
				sNewValue = FormatValueArray(aFiles);
		}
		break;

	case IDC_CUSTOMICON_COMBO:
		if (m_cbCustomIcons.IsMultiSelectionEnabled())
		{
			CStringArray aMatched, aMixed;
			m_cbCustomIcons.GetChecked(aMatched, aMixed);

			sNewValue = FormatMultiSelItems(aMatched, aMixed);
		}
		else
		{
			sNewValue = CDialogHelper::GetSelectedItem(m_cbCustomIcons);
		}
		break;

	default:
		ASSERT(0);
		return;
	}

	HideControl(*GetDlgItem(nCtrlID));
	SetValueText(nRow, sNewValue);
}

void CTDLTaskAttributeListCtrl::NotifyParentEdit(int nRow, LPARAM bUnitsChange)
{
	if (m_bTaskIDChangeSinceLastEdit)
		return;

	UpdateWindow();

	// Refresh the cell text only if the edit failed
	if (!GetParent()->SendMessage(WM_TDCN_ATTRIBUTEEDITED, GetAttributeID(nRow, TRUE), bUnitsChange))
		RefreshSelectedTasksValue(nRow);
}

void CTDLTaskAttributeListCtrl::OnDependsChange()
{
	// Received after a manual edit of the task IDs
	HideControl(m_eDepends);

	int nRow = GetCurSel();
	SetValueText(nRow, m_eDepends.FormatDependencies());
}

void CTDLTaskAttributeListCtrl::OnDependsKillFocus()
{
	OnDependsChange();
}

void CTDLTaskAttributeListCtrl::OnSingleFileLinkKillFocus()
{
	// Received after a manual edit of the task IDs
	CString sFile;
	m_eSingleFileLink.GetWindowText(sFile);

	HideControl(m_eSingleFileLink);

	int nRow = GetCurSel();
	SetValueText(nRow, sFile);
}

BOOL CTDLTaskAttributeListCtrl::SetValueText(int nRow, const CString& sNewText, LPARAM bUnitsChange)
{
	if (m_bTaskIDChangeSinceLastEdit)
		return FALSE;

	if (sNewText == GetItemText(nRow, VALUE_COL))
		return FALSE;

	CLockUpdates hr(*this);

	VERIFY(SetItemText(nRow, VALUE_COL, sNewText));
	NotifyParentEdit(nRow, bUnitsChange);

	return TRUE;
}

void CTDLTaskAttributeListCtrl::OnTimePeriodKillFocus()
{
	HideControl(m_eTimePeriod);
	m_eTimePeriod.DeleteButton(ID_BTN_ADDLOGGEDTIME);
	m_eTimePeriod.DeleteButton(ID_BTN_TIMETRACK);
	
	if (!m_eTimePeriod.HasValidTime())
		return;

	TDCTIMEPERIOD tpNew(m_eTimePeriod.GetTime(), m_eTimePeriod.GetUnits());

	int nRow = GetCurSel();
	CString sCurTime = GetItemText(nRow, VALUE_COL);
	BOOL bUnitsChange = sCurTime.IsEmpty();

	if (!bUnitsChange)
	{
		TDCTIMEPERIOD tpCur(sCurTime);

		bUnitsChange = ((tpCur.dAmount > 0.0) &&
						(tpNew.nUnits != tpCur.nUnits) &&
						(Misc::Format(tpNew.dAmount, TIMEPERIOD_DECPLACES) == Misc::Format(tpCur.dAmount, TIMEPERIOD_DECPLACES)));
	}

	SetValueText(nRow, tpNew.Format(TIMEPERIOD_DECPLACES), bUnitsChange);
}

void CTDLTaskAttributeListCtrl::OnCancelEdit()
{
	int nRow = GetCurSel();

	if ((nRow != -1) && CanEditCell(nRow, VALUE_COL))
	{
		// Reinitialise the edit ctrl with its previous value to revert changes
		CWnd* pCtrl = GetEditControl(nRow, FALSE);

		if (pCtrl)
		{
			// If we've been using the cell text as a scratch pad we need 
			// revert any changes before calling PrepareControl
			switch (pCtrl->GetDlgCtrlID())
			{
			case IDC_DATE_PICKER:
			case IDC_TIME_PICKER:
				RefreshSelectedTasksValue(nRow);
				break;
			}

			PrepareControl(*pCtrl, nRow, VALUE_COL);
		}
	}

	CInputListCtrl::OnCancelEdit();
}

void CTDLTaskAttributeListCtrl::OnDateCloseUp(NMHDR* pNMHDR, LRESULT* pResult) 
{ 
	UNREFERENCED_PARAMETER(pNMHDR);
	ASSERT(pNMHDR->idFrom == IDC_DATE_PICKER);

	// Note: our base class may already have hidden 
	// the date picker so we have to check first
	if (m_datePicker.IsWindowVisible())
	{
		// If the use clicked in the date field DON'T hide the date picker
		// because we assume they want to perform a keyboard edit
		if (Misc::IsKeyPressed(VK_LBUTTON) &&
			(m_datePicker.HitTest(GetMessagePos(), TRUE) == DTCHT_DATETIME))
		{
			return;
		}

		// Note: HideControl will call HandleDateEditCompletion
		// via OnDateKillFocus so we don't need to
		HideControl(m_datePicker);
	}
	else
	{
		HandleDateEditCompletion();
	}

	*pResult = 0;
}

void CTDLTaskAttributeListCtrl::OnDateKillFocus(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (m_datePicker.IsWindowVisible())
		HideControl(m_datePicker);

	HandleDateEditCompletion();
}

void CTDLTaskAttributeListCtrl::OnDateChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	ASSERT(pNMHDR->idFrom == IDC_DATE_PICKER);

	// Only handle this if the calendar is closed ie. it's a manual edit
	// or the checkbox was clicked when the calendar was visible
	if (!m_datePicker.IsCalendarVisible())
	{
		NMDATETIMECHANGE* pNMDTC = (NMDATETIMECHANGE*)pNMHDR;

		if (pNMDTC->dwFlags == GDT_NONE)
		{
			// Clear the text and end the edit, triggering a parent notification
			VERIFY(SetItemText(GetCurSel(), VALUE_COL, _T("")));
			
			HandleDateEditCompletion();
		}
		else
		{
			// Use the cell text as a scratch-pad for storing intermediate
			// date edits but without notifying our parent
			COleDateTime date(pNMDTC->st);

			VERIFY(SetItemText(GetCurSel(), VALUE_COL, Misc::Format(date.m_dt)));
		}
	}

	*pResult = 0;
}

void CTDLTaskAttributeListCtrl::HandleDateEditCompletion()
{
	// If we've got multiple different values, DON'T notify
	// our parent if the cell text has not actually changed
	int nRow = GetCurSel();
	CString sValue = GetItemText(nRow, VALUE_COL);

	if (RowValueVaries(nRow) && (sValue == DATETIME_VARIES))
		return;

	TRACE(_T("\nCalling NotifyParentEdit(%d, \"%s\")\n"), nRow, sValue);

	NotifyParentEdit(nRow);
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

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		{
			ASSERT(m_aCustomAttribDefs.IsListType(nAttribID));

			const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
			GET_CUSTDEF_RET(m_aCustomAttribDefs, nAttribID, pDef, 0L);

			// Update the 'non-default' data only
			CDialogHelper::GetComboBoxItems(m_cbTextAndNumbers, pDef->aAutoListData);
			Misc::RemoveItems(pDef->aDefaultListData, pDef->aAutoListData);
		}
		break;
	}

	return GetParent()->SendMessage(WM_TDCN_AUTOITEMADDEDDELETED, nAttribID);
}

BOOL CTDLTaskAttributeListCtrl::DeleteSelectedCell()
{
	if (m_nCurCol == VALUE_COL)
	{
		TDC_ATTRIBUTE nAttribID = GetAttributeID(GetCurSel());

		switch (nAttribID)
		{
		case TDCA_REMINDER:
			GetParent()->SendMessage(WM_TDCM_CLEARTASKREMINDER);
			return TRUE;

		default:
			if (GetParent()->SendMessage(WM_TDCM_CLEARTASKATTRIBUTE, nAttribID))
			{
				SetItemText(GetCurSel(), m_nCurCol, _T(""));
				return TRUE;
			}
			break;
		}
	}

	// else
	return FALSE;
}

LRESULT CTDLTaskAttributeListCtrl::OnEnEditButtonClick(WPARAM /*wParam*/, LPARAM lParam)
{
#ifdef _DEBUG
	TDC_ATTRIBUTE nAttribID = GetAttributeID(GetCurSel());
#endif

	switch (lParam)
	{
	case ID_BTN_TIMETRACK:
		ASSERT(nAttribID == TDCA_TIMESPENT);
		return GetParent()->SendMessage(WM_TDCM_TOGGLETIMETRACKING);

	case ID_BTN_ADDLOGGEDTIME:
		ASSERT(nAttribID == TDCA_TIMESPENT);
		return GetParent()->SendMessage(WM_TDCM_ADDTIMETOLOGFILE);

	case ID_BTN_VIEWDEPENDS:
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
	return GetParent()->SendMessage(WM_FE_GETFILEICON, wParam, lParam);
}

LRESULT CTDLTaskAttributeListCtrl::OnFileLinkWantTooltip(WPARAM wParam, LPARAM lParam)
{
	return GetParent()->SendMessage(WM_TDCM_GETLINKTOOLTIP, wParam, lParam);
}

LRESULT CTDLTaskAttributeListCtrl::OnFileLinkDisplay(WPARAM wParam, LPARAM lParam)
{
	return GetParent()->SendMessage(WM_TDCM_DISPLAYLINK, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////////////////////////

CTDLTaskAttributeListCtrl::CFileDropTarget::CFileDropTarget(CTDLTaskAttributeListCtrl* pAtributeList) 
	: 
	m_pAttributeList(pAtributeList),
	m_nDropHighlightedRow(-1)
{
}

DROPEFFECT CTDLTaskAttributeListCtrl::CFileDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	m_nDropHighlightedRow = -1;

	return DROPEFFECT_COPY;
}

DROPEFFECT CTDLTaskAttributeListCtrl::CFileDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	CStringArray aUnused;
	int nRow = -1;

	BOOL bValidDrop = CanDropFiles(point, pDataObject, nRow, aUnused);
	
	if (bValidDrop)
	{
		if (nRow != m_nDropHighlightedRow)
			m_pAttributeList->SetItemState(m_nDropHighlightedRow, 0, LVIS_DROPHILITED);

		m_pAttributeList->SetItemState(nRow, LVIS_DROPHILITED, LVIS_DROPHILITED);
		m_nDropHighlightedRow = nRow;
	}
	else if (m_nDropHighlightedRow != -1)
	{
		m_pAttributeList->SetItemState(m_nDropHighlightedRow, 0, LVIS_DROPHILITED);
		m_nDropHighlightedRow = -1;
	}
	
	return (bValidDrop ? DROPEFFECT_COPY : DROPEFFECT_NONE);
}

BOOL CTDLTaskAttributeListCtrl::CFileDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	CStringArray aFiles;
	int nRow = -1;

	if (!CanDropFiles(point, pDataObject, nRow, aFiles))
		return FALSE;

	if ((aFiles.GetSize() == 1) && (m_pAttributeList->GetAttributeID(nRow, TRUE) != TDCA_FILELINK))
	{
		m_pAttributeList->SetValueText(nRow, aFiles[0]);
	}
	else
	{
		// Append unique file names to list
		CStringArray aExisting;
		SplitValueArray(m_pAttributeList->GetItemText(nRow, VALUE_COL), aExisting);

		if (Misc::AppendItems(aFiles, aExisting, TRUE))
			m_pAttributeList->SetValueText(nRow, FormatValueArray(aExisting));
	}

	return TRUE;
}

DROPEFFECT CTDLTaskAttributeListCtrl::CFileDropTarget::OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point)
{
	if (DoDragScroll(pWnd, dwKeyState, point))
		return DROPEFFECT_SCROLL;

	// else
	return COleDropTarget::OnDragScroll(pWnd, dwKeyState, point);
}

BOOL CTDLTaskAttributeListCtrl::CFileDropTarget::CanDropFiles(TDC_ATTRIBUTE nAttribID, const CStringArray& aFiles) const
{
	ASSERT(m_pAttributeList->m_aSelectedTaskIDs.GetSize());
	ASSERT(!m_pAttributeList->m_data.HasStyle(TDCS_READONLY));
	ASSERT(m_pAttributeList->m_multitasker.AnyTaskIsUnlocked(m_pAttributeList->m_aSelectedTaskIDs));

	switch (nAttribID)
	{
	case TDCA_FILELINK:
		return TRUE;

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		{
			const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
			GET_CUSTDEF_RET(m_pAttributeList->m_aCustomAttribDefs, nAttribID, pDef, 0L);

			switch (pDef->GetDataType())
			{
			case TDCCA_FILELINK:
				return (aFiles.GetSize() == 1);

			case TDCCA_STRING:
				return (pDef->IsAutoList() || (!pDef->IsList() && (aFiles.GetSize() == 1)));
			}
		}
		break;
	}

	// All else
	return FALSE;
}

BOOL CTDLTaskAttributeListCtrl::CFileDropTarget::CanDropFiles(const CPoint& point, COleDataObject* pDataObject, int& nRow, CStringArray& aFiles) const
{
	if (m_pAttributeList->m_aSelectedTaskIDs.GetSize() == 0)
		return FALSE;

	if (m_pAttributeList->m_data.HasStyle(TDCS_READONLY))
		return FALSE;

	if (!m_pAttributeList->m_multitasker.AnyTaskIsUnlocked(m_pAttributeList->m_aSelectedTaskIDs))
		return FALSE;

	nRow = m_pAttributeList->HitTest(point);

	// This check is essential to prevent the auto-scrolling 
	// and EnsureVisible from getting into a fight
	if (nRow == -1)
		return FALSE;

	int nNumFiles = FileMisc::GetDropFilePaths(pDataObject, aFiles);

	if (!nNumFiles)
		return FALSE;

	if (CanDropFiles(m_pAttributeList->GetAttributeID(nRow), aFiles))
		return TRUE;
	
	// else look for the closest valid attribute to the hit-test
	int nClosest = m_pAttributeList->FindItemFromData(TDCA_FILELINK); 

	int nFrom = m_pAttributeList->GetTopIndex();
	int nTo = (nFrom + m_pAttributeList->GetCountPerPage());
	nTo = min(nTo, (m_pAttributeList->GetItemCount() - 1));
	
	for (int nItem = nFrom; nItem <= nTo; nItem++)
	{
		TDC_ATTRIBUTE nAttribID = m_pAttributeList->GetAttributeID(nItem, TRUE);

		if (CanDropFiles(nAttribID, aFiles))
		{
			if ((nClosest == -1) || (abs(nItem - nRow) < abs(nClosest - nRow)))
				nClosest = nItem;
		}
	}

	if (nClosest == -1)
		return FALSE;

	// else
	nRow = nClosest;
	m_pAttributeList->EnsureVisible(nRow, FALSE);

	return TRUE;
}

void CTDLTaskAttributeListCtrl::CFileDropTarget::OnDragLeave(CWnd* pWnd)
{
	if (m_nDropHighlightedRow != -1)
	{
		m_pAttributeList->SetItemState(m_nDropHighlightedRow, 0, LVIS_DROPHILITED);
		m_nDropHighlightedRow = -1;
	}
}

BOOL CTDLTaskAttributeListCtrl::PreTranslateMessage(MSG* pMsg)
{
	m_tooltip.FilterToolTipMessage(pMsg);

	return CInputListCtrl::PreTranslateMessage(pMsg);
}

int CTDLTaskAttributeListCtrl::GetDateRow(TDC_ATTRIBUTE nTimeAttribID) const
{
	switch (nTimeAttribID)
	{
	case TDCA_DONETIME:		return GetRow(TDCA_DONEDATE);
	case TDCA_DUETIME:		return GetRow(TDCA_DUEDATE);
	case TDCA_STARTTIME:	return GetRow(TDCA_STARTDATE);

	default:
		if (IsCustomTime(nTimeAttribID))
			return GetRow(MapTimeToDate(nTimeAttribID));
		break;
	}

	ASSERT(0);
	return -1;
}

int CTDLTaskAttributeListCtrl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	// Check for mouse over a child editing control
	if (IsEditing())
	{
		CPoint ptScreen(point);
		ClientToScreen(&ptScreen);

		if (::WindowFromPoint(ptScreen) != *this)
			return 0;
	}

	LVHITTESTINFO lvHit = { { point.x, point.y }, 0 };

	// Get around constness
	int nRow = (int)::SendMessage(m_hWnd, LVM_SUBITEMHITTEST, 0, (LPARAM)&lvHit);
	int nCol = lvHit.iSubItem;

	if ((nRow == -1) || (nCol == -1))
		return 0;

	static CEnString sTooltip;
	sTooltip.Empty();
	
	// Button tooltips
	TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);
	int nToolID = MAKELONG(nRow, nCol);
	CRect rBounds;
	
	if (GetButtonRect(nRow, nCol, rBounds) && rBounds.PtInRect(point))
	{
		int nBtnID = HitTestButtonID(nRow, rBounds);
		nToolID += nBtnID;

		switch (nBtnID)
		{
		case ID_BTN_ADDLOGGEDTIME:
			sTooltip.LoadString(IDS_TDC_ADDLOGGEDTIME);
			break;

		case ID_BTN_TIMETRACK:
			sTooltip.LoadString(IDS_TDC_STARTSTOPCLOCK);
			break;

		case ID_BTN_VIEWDEPENDS:
		case ID_BTN_VIEWFILE:
			sTooltip.LoadString(IDS_VIEW);
			break;

		case ID_BTN_EDITDEPENDS:
			sTooltip.LoadString(IDS_ATTRIBTIP_MODIFY);
			break;

		case ID_BTN_BROWSEFILE:
			sTooltip.LoadString(IDS_BROWSE);
			break;

		case ID_BTN_CLEARREMINDER:
			sTooltip.LoadString(IDS_ATTRIBTIP_DISMISS);
			break;

		case ID_BTN_DEFAULT:
			switch (nAttribID)
			{
			case TDCA_DEPENDENCY:
			case TDCA_COLOR:
			case TDCA_ICON:
			case TDCA_RECURRENCE:
			case TDCA_REMINDER:
				sTooltip.LoadString(IDS_ATTRIBTIP_MODIFY);
				break;

			case TDCA_FILELINK:
				if (GetItemText(nRow, VALUE_COL).IsEmpty())
					sTooltip.LoadString(IDS_BROWSE);
				break;

			case TDCA_TIMEESTIMATE:
			case TDCA_TIMESPENT:
			case TDCA_TIMEREMAINING:
				sTooltip.LoadString(IDS_TIMEUNITS);
				break;

			default:
				if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
				{
					const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
					GET_CUSTDEF_RET(m_aCustomAttribDefs, nAttribID, pDef, NULL);

					switch (pDef->GetDataType())
					{
					case TDCCA_FILELINK:
						sTooltip.LoadString(IDS_BROWSE);
						break;

					case TDCCA_TIMEPERIOD:
						sTooltip.LoadString(IDS_TIMEUNITS);
						break;

					case TDCCA_ICON:
						if (!pDef->IsList())
							sTooltip.LoadString(IDS_ATTRIBTIP_MODIFY);
						break;
					}
				}
				break;
			}
			break;
		}
	}
	else // Label/Value tooltips
	{
		GetCellRect(nRow, nCol, rBounds);

		switch (nCol)
		{
		case LABEL_COL:
			if (IsCustomTime(nAttribID))
				sTooltip.LoadString(IDS_ATTRIBTIP_TIMEOFDAY);
			break;

		case VALUE_COL:
			if (!RowValueVaries(nRow))
			{
				BOOL bCheckTextNeedsTip = FALSE;

				switch (nAttribID)
				{
				case TDCA_TASKNAME:
					bCheckTextNeedsTip = TRUE;
					break;

				case TDCA_ALLOCTO:
				case TDCA_CATEGORY:
				case TDCA_TAGS:
				case TDCA_FILELINK:
					{
						CStringArray aValues;

						if (SplitValueArray(GetItemText(nRow, nCol), aValues) > 1)
							sTooltip = Misc::FormatArray(aValues, TOOLTIP_DELIM);
						else
							bCheckTextNeedsTip = (nAttribID == TDCA_FILELINK);
					}
					break;

				case TDCA_COLOR:
					{
						COLORREF color = _ttoi(GetItemText(nRow, nCol));

						if (!color || (color == CLR_NONE))
							sTooltip.LoadString(IDS_ATTRIBTIP_DEFCOLOR);
					}
					break;

				case TDCA_STARTDATE:
					if (!m_data.HasStyle(TDCS_READONLY) && 
						m_data.HasStyle(TDCS_AUTOADJUSTDEPENDENCYDATES) && 
						m_multitasker.AnyTaskIsUnlocked(m_aSelectedTaskIDs) &&
						m_multitasker.AllTasksHaveDependencies(m_aSelectedTaskIDs))
					{
						sTooltip.LoadString(IDS_ATTRIBTIP_DEPENDENTDATE);
					}
					break;

				case TDCA_DONETIME:
				case TDCA_DUETIME:
				case TDCA_STARTTIME:
					{
						int nDateRow = (nRow - 1); // Always
						ASSERT(GetDateRow(nAttribID) == nDateRow);

						if (CanEditCell(nDateRow, nCol) && !RowValueVaries(nDateRow) && GetItemText(nDateRow, nCol).IsEmpty())
							sTooltip.LoadString(IDS_ATTRIBTIP_NODATESET);
					}
					break;

				case TDCA_RECURRENCE:
					if (m_multitasker.AllTasksAreDone(m_aSelectedTaskIDs, FALSE)) // exclude 'good as done'
						sTooltip.LoadString(IDS_ATTRIBTIP_COMPLETEDTASK);
					break;

				case TDCA_REMINDER:
					if (m_multitasker.AllTasksAreDone(m_aSelectedTaskIDs, TRUE)) // include 'good as done'
						sTooltip.LoadString(IDS_ATTRIBTIP_COMPLETEDTASK);
					break;

				case TDCA_DEPENDENCY:
					{
						CTDCDependencyArray aDepends;
					
						if (aDepends.Parse(GetItemText(nRow, nCol)) > 1)
							sTooltip = m_formatter.GetDependencies(aDepends, TOOLTIP_DELIM);
					}
					break;

				case TDCA_PERCENT:
					if (m_data.HasStyle(TDCS_AUTOCALCPERCENTDONE) ||
						(m_data.HasStyle(TDCS_AVERAGEPERCENTSUBCOMPLETION) &&
						 m_multitasker.AnyTaskIsParent(m_aSelectedTaskIDs)))
					{
						sTooltip.LoadString(IDS_ATTRIBTIP_CALCULATEDVALUE);
					}
					break;

				default:
					if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
					{
						const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
						GET_CUSTDEF_RET(m_aCustomAttribDefs, nAttribID, pDef, NULL);

						if (pDef->IsCalculation())
						{
							sTooltip.LoadString(IDS_ATTRIBTIP_CALCULATEDVALUE);
						}
						else if (pDef->IsDataType(TDCCA_STRING))
						{
							if (pDef->IsMultiList())
							{
								CStringArray aValues;

								if (SplitValueArray(GetItemText(nRow, nCol), aValues) > 1)
									sTooltip = Misc::FormatArray(aValues, TOOLTIP_DELIM);
							}
							else
							{
								bCheckTextNeedsTip = !pDef->IsList();
							}
						}
					}
					else if (IsCustomTime(nAttribID))
					{
						int nDateRow = (nRow - 1);
						ASSERT(GetDateRow(nAttribID) == nDateRow);

						if (CanEditCell(nDateRow, nCol) && !RowValueVaries(nDateRow) && GetItemText(nDateRow, nCol).IsEmpty())
							sTooltip.LoadString(IDS_ATTRIBTIP_NODATESET);
					}
					break;
				}

				if (sTooltip.IsEmpty())
				{
					if (!CanEditCell(nRow, nCol) && m_multitasker.AnyTaskIsUnlocked(m_aSelectedTaskIDs))
					{
						sTooltip.LoadString(IDS_STATUSREADONLY);
					}
					else if (bCheckTextNeedsTip)
					{
						CString sText = GetItemText(nRow, nCol);
						int nTextWidth = GraphicsMisc::GetTextWidth(sText, *this);

						if (nTextWidth >= GetColumnWidth(VALUE_COL))
						{
							if (nTextWidth <= MAX_TIP_LINELEN)
							{
								sTooltip = sText;
							}
							else
							{
								CStringArray aLines;

								int nMaxCharLineLen = (int)(MAX_TIP_LINELEN / GraphicsMisc::GetAverageCharWidth(*this));
								int nNumLines = Misc::SplitLines(sText, aLines, nMaxCharLineLen);

								sTooltip = Misc::FormatArray(aLines, TOOLTIP_DELIM);
							}
						}
					}
				}
			}
			break;
		}
	}

	if (sTooltip.IsEmpty())
		return -1;

	return CToolTipCtrlEx::SetToolInfo(*pTI,
									   *this,
									   sTooltip,
									   nToolID,
									   rBounds,
									   TTF_TRANSPARENT | TTF_NOTBUTTON | TTF_EXCLUDEBOUNDS);
}

void CTDLTaskAttributeListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (GetGrouping().IsEnabled())
	{
		// The default behaviour when grouping is enabled and the 
		// selection is on the first item of a new group deviates
		// from what is expected by selecting all the new group's
		// items instead of moving the selection to the second item 
		// in the group (or the last item in the previous group).
		// And because this appears to the unwary as a bug I work 
		// around it by handling all such keyboard navigation.
		int nNextSel = m_aSortedGroupedItems.GetNextItem(nChar);

		if (nNextSel != -1)
		{
			if (nNextSel != GetCurSel())
			{
				CEnListCtrl::SetCurSel(nNextSel, TRUE);
				EnsureVisible(nNextSel, FALSE);
			}
			return;
		}
	}

	// All else
	CInputListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTDLTaskAttributeListCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	HideAllControls();

	LVHITTESTINFO lvHit = { { point.x, point.y }, 0 };

	int nRow = SubItemHitTest(&lvHit);
	int nCol = lvHit.iSubItem;

	SetCurSel(nRow, nCol);

	// Parent handles context menu
	CInputListCtrl::OnRButtonDown(nFlags, point);
}

CString CTDLTaskAttributeListCtrl::FormatValueArray(const CStringArray& aValues)
{
	return Misc::FormatArray(aValues, ITEM_DELIM);
}

int CTDLTaskAttributeListCtrl::SplitValueArray(const CString& sValues, CStringArray& aValues)
{
	return Misc::Split(sValues, aValues, ITEM_DELIM);
}

////////////////////////////////////////////////////////////////////////////////

int CTDLTaskAttributeListCtrl::CSortedGroupItemArray::CheckBuildArray()
{
	if (!GetSize())
	{
		int nVScrollPos = m_list.GetScrollPos(SB_VERT);
		int nNumItems = m_list.GetItemCount(), nItem = nNumItems;

		SetSize(nNumItems);

		while (nItem--)
		{
			GROUPITEM& sgi = GetAt(nItem);

			sgi.dwItemData = m_list.GetItemData(nItem);
			sgi.nGroupID = m_list.GetGrouping().GetItemGroupId(nItem);

			m_list.GetItemRect(nItem, sgi.rItem, LVIR_BOUNDS);
			sgi.rItem.OffsetRect(0, nVScrollPos);
		}

		Misc::SortArrayT<GROUPITEM>(*this, SortProc);
	}

	return GetSize();
}

int CTDLTaskAttributeListCtrl::CSortedGroupItemArray::GetNextItem(int nKeyPress)
{
	if (!CheckBuildArray())
		return -1;

	// Find out where we are
	int nCurSel = m_list.GetCurSel(), nFrom = FindItem(m_list.GetItemData(nCurSel));
	int nNumItems = GetSize(), nNext = -1;

	switch (nKeyPress)
	{
	case VK_DOWN:	
		nNext = (nFrom + 1);
		break;
	
	case VK_UP:		
		nNext = (nFrom - 1);
		break;

	case VK_NEXT:	
		nNext = (nFrom + GetPageSize(nFrom, TRUE));
		break;

	case VK_PRIOR:	
		nNext = (nFrom - GetPageSize(nFrom, FALSE));
		break;

	case VK_END:	
		nNext = (nNumItems - 1);
		break;
	
	case VK_HOME:	
		nNext = 0;
		break;

	default:
		return -1;
	}

	nNext = min(max(0, nNext), (nNumItems - 1));

	if (nNext == nFrom)
		return nCurSel;

	return m_list.FindItemFromData(GetAt(nNext).dwItemData);
}

int CTDLTaskAttributeListCtrl::CSortedGroupItemArray::GetPageSize(int nFrom, BOOL bDown) const
{
	CRect rClient;
	m_list.GetClientRect(rClient);

	int nNumItems = GetSize(), nPageSize = 0;

	if (bDown)
	{
		rClient.OffsetRect(0, GetAt(nFrom).rItem.top);

		int nTo = (nFrom + m_list.GetCountPerPage());
		nTo = min(nTo, nNumItems);

		for (int nItem = nFrom; nItem < nTo; nItem++)
		{
			if (GetAt(nItem).rItem.bottom > rClient.bottom)
				break;

			nPageSize++;
		}
	}
	else // Up
	{
		rClient.OffsetRect(0, (GetAt(nFrom).rItem.bottom - rClient.Height()));

		int nTo = (nFrom - m_list.GetCountPerPage());
		nTo = max(nTo, 0);

		for (int nItem = nFrom; nItem >= nTo; nItem--)
		{
			if (GetAt(nItem).rItem.top < rClient.top)
				break;

			nPageSize++;
		}
	}
	ASSERT(nPageSize > 0);

	return (nPageSize - 1);
}

int CTDLTaskAttributeListCtrl::CSortedGroupItemArray::FindItem(DWORD dwItemData) const
{
	int nItem = GetSize();

	while (nItem--)
	{
		if (GetAt(nItem).dwItemData == dwItemData)
			return nItem;
	}

	ASSERT(0);
	return -1;
}

int CTDLTaskAttributeListCtrl::CSortedGroupItemArray::SortProc(const void* item1, const void* item2)
{
	const GROUPITEM* pItem1 = (const GROUPITEM*)item1;
	const GROUPITEM* pItem2 = (const GROUPITEM*)item2;

	return (pItem1->rItem.top - pItem2->rItem.top);
}

////////////////////////////////////////////////////////////////////////////////

CTDLTaskAttributeListCtrl::CSortedGroupedHeaderArray::CSortedGroupedHeaderArray(BOOL bSortAscending)
{
	static const UINT ATTRIBGROUPS[][2] =
	{
		{ TDCAG_OTHER,			IDS_ATTRIBCAT_OTHER },
		{ TDCAG_CUSTOM,			IDS_ATTRIBCAT_CUSTOM },
		{ TDCAG_DATETIME,		IDS_ATTRIBCAT_DATETIME },
		{ TDCAG_SINGLETEXT,		IDS_ATTRIBCAT_SINGLETEXT },
		{ TDCAG_MULTITEXT,		IDS_ATTRIBCAT_MULTITEXT },
		{ TDCAG_NUMERIC,		IDS_ATTRIBCAT_NUMERIC },
		{ TDCAG_TIMEPERIOD,		IDS_ATTRIBCAT_TIMEPERIOD },
	};

	const int NUM_ATTRIBGROUPS = (sizeof(ATTRIBGROUPS) / (2 * sizeof(UINT)));
	SetSize(NUM_ATTRIBGROUPS);

	for (int nGroup = 0; nGroup < NUM_ATTRIBGROUPS; nGroup++)
	{
		ElementAt(nGroup).nGroup = (TDC_ATTRIBUTEGROUP)ATTRIBGROUPS[nGroup][0];
		ElementAt(nGroup).sName = CEnString(ATTRIBGROUPS[nGroup][1]);
	}

	Misc::SortArrayT<ATTRIBGROUP>(*this, (bSortAscending ? AscendingSortProc : DescendingSortProc));
}

int CTDLTaskAttributeListCtrl::CSortedGroupedHeaderArray::AscendingSortProc(const void* item1, const void* item2)
{
	const ATTRIBGROUP* pItem1 = (const ATTRIBGROUP*)item1;
	const ATTRIBGROUP* pItem2 = (const ATTRIBGROUP*)item2;

	return Misc::NaturalCompare(pItem1->sName, pItem2->sName);

}

int CTDLTaskAttributeListCtrl::CSortedGroupedHeaderArray::DescendingSortProc(const void* item1, const void* item2)
{
	return -AscendingSortProc(item1, item2);
}

////////////////////////////////////////////////////////////////////////////////

CTDLTaskAttributeListCtrl::ATTRIBSTATE::ATTRIBSTATE(UINT nLabelResID, TDC_ATTRIBUTE attribID, TDC_ATTRIBUTEGROUP group)
	:
	nAttribID(attribID),
	nGroup(group),
	nPos(0),
	crLabelBkgnd(CLR_NONE)
{
	if (nLabelResID)
		sLabel = CEnString(nLabelResID);
}

CTDLTaskAttributeListCtrl::ATTRIBSTATE::ATTRIBSTATE(const TDCATTRIBUTE& attrib)
	:
	sLabel(CEnString(attrib.nLabelResID)),
	nAttribID(attrib.nAttributeID),
	nGroup(attrib.nGroup),
	nPos(0),
	crLabelBkgnd(CLR_NONE)
{
}

CTDLTaskAttributeListCtrl::ATTRIBSTATE::ATTRIBSTATE(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef)
	:
	sLabel(attribDef.sLabel),
	nAttribID(attribDef.GetAttributeID()),
	sCustAttribID(attribDef.sUniqueID),
	nGroup(TDCAG_CUSTOM),
	nPos(0),
	crLabelBkgnd(CLR_NONE)
{
}

BOOL CTDLTaskAttributeListCtrl::ATTRIBSTATE::IsCustom() const
{
	return TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID);
}

// -----------------------------------------------------------------

CTDLTaskAttributeListCtrl::CAttributeStates::CAttributeStates(const CTDCCustomAttribDefinitionArray& aCustAttribDefs)
	:
	m_aCustomAttribDefs(aCustAttribDefs)
{
	Populate();
}

void CTDLTaskAttributeListCtrl::CAttributeStates::Populate()
{
	// Built-in attributes (excluding TDCA_NONE)
	for (int nAtt = 1; nAtt < ATTRIB_COUNT; nAtt++)
	{
		m_aAttributeItems.Add(ATTRIBSTATE(TASKATTRIBUTES[nAtt]));
	}

	// Custom attributes
	for (int nCust = 0; nCust < m_aCustomAttribDefs.GetSize(); nCust++)
	{
		m_aAttributeItems.Add(ATTRIBSTATE(m_aCustomAttribDefs[nCust]));
	}

	// Misc others
	TDCATTRIBUTE att = { TDCA_REMINDER, TDCAG_DATETIME, IDS_TDLBC_REMINDER };
	m_aAttributeItems.Add(ATTRIBSTATE(att));

	// Sort 
	Misc::SortArrayT<ATTRIBSTATE>(m_aAttributeItems, SortByNameProc);
	m_sDefaultOrder = GetItemsState(FALSE);

	RebuildItemPositions();
}

BOOL CTDLTaskAttributeListCtrl::CAttributeStates::MoveAttribute(TDC_ATTRIBUTE nAttribID, TDC_ATTRIBUTE nAttribIDBelow)
{
	ASSERT(nAttribID != TDCA_NONE);

	int nOldPos = GetAttribPos(nAttribID);
	int nNewPos = (GetAttribPos(nAttribIDBelow) + 1);

	if ((nOldPos < 0) || (nNewPos < 0))
	{
		ASSERT(0);
		return FALSE;
	}

	if (nNewPos == nOldPos)
		return FALSE;

	ATTRIBSTATE item = m_aAttributeItems.GetAt(nOldPos);
	m_aAttributeItems.InsertAt(nNewPos, item);

	if (nNewPos > nOldPos)
		m_aAttributeItems.RemoveAt(nOldPos);
	else
		m_aAttributeItems.RemoveAt(nOldPos + 1);
	
	RebuildItemPositions();
	return TRUE;
}

int CTDLTaskAttributeListCtrl::CAttributeStates::GetAttribPos(TDC_ATTRIBUTE nAttribID) const
{
	int nPos = -1;
	
	if ((nAttribID == TDCA_NONE) || m_mapPositions.Lookup(nAttribID, nPos))
		return nPos;

	// Time of day uses their corresponding dates
	switch (nAttribID)
	{
	case TDCA_STARTTIME:	return GetAttribPos(TDCA_STARTDATE); // RECURSIVE CALL
	case TDCA_DUETIME:		return GetAttribPos(TDCA_DUEDATE);   // RECURSIVE CALL
	case TDCA_DONETIME:		return GetAttribPos(TDCA_DONEDATE);  // RECURSIVE CALL

	default:
		if (CTDLTaskAttributeListCtrl::IsCustomTime(nAttribID))
		{
			TDC_ATTRIBUTE nDateAttribID = (TDC_ATTRIBUTE)(nAttribID - CUSTOMTIMEATTRIBOFFSET);
			return GetAttribPos(nDateAttribID); // RECURSIVE CALL
		}
		else // unknown custom attribute
		{
			ASSERT(TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID));
			nPos = (m_aAttributeItems.GetSize() - 1);
		}
	}

	return nPos;
}

BOOL CTDLTaskAttributeListCtrl::CAttributeStates::GetNextAttribute(TDC_ATTRIBUTE nAttribID, BOOL bUp, BOOL bSameGroup, TDC_ATTRIBUTE& nNextAttribID) const
{
	if (nAttribID == TDCA_NONE)
		return FALSE;

	int nPos = GetAttribPos(nAttribID), nNextPos = nPos;
	int nNumItems = m_aAttributeItems.GetSize();

	if ((bUp && (nPos == 0)) || (!bUp && (nPos >= (nNumItems - 1))))
		return FALSE;

	if (bSameGroup)
	{
		const ATTRIBSTATE& attrib = m_aAttributeItems[nPos];

		if (bUp)
		{
			for (--nNextPos; nNextPos >= 0; nNextPos--)
			{
				const ATTRIBSTATE& attribNext = m_aAttributeItems[nNextPos];

				if (attribNext.nGroup == attrib.nGroup)
				{
					nNextAttribID = attribNext.nAttribID;
					return TRUE;
				}
			}
		}
		else
		{
			for (++nNextPos; nNextPos < nNumItems; nNextPos++)
			{
				const ATTRIBSTATE& attribNext = m_aAttributeItems[nNextPos];

				if (attribNext.nGroup == attrib.nGroup)
				{
					nNextAttribID = attribNext.nAttribID;
					return TRUE;
				}
			}
		}

		return FALSE;
	}
	else
	{
		if (bUp)
			nNextPos--;
		else
			nNextPos++;
	}

	if (nNextPos >= nNumItems)
		return FALSE;

	if (nNextPos < 0)
		nNextAttribID = TDCA_NONE; // top of list
	else
		nNextAttribID = m_aAttributeItems[nNextPos].nAttribID;

	 return (nNextAttribID != nAttribID);
}

int CTDLTaskAttributeListCtrl::CAttributeStates::CompareItems(TDC_ATTRIBUTE nAttribID1, TDC_ATTRIBUTE nAttribID2) const
{
	ASSERT((nAttribID1 != TDCA_NONE) && (nAttribID2 != TDCA_NONE));

	int nPos1 = GetAttribPos(nAttribID1);
	int nPos2 = GetAttribPos(nAttribID2);

	int nCompare = (nPos1 - nPos2);

	// Sort by ID for a stable sort
	if (nCompare == 0)
		nCompare = (nAttribID1 - nAttribID2);

	return nCompare;
}

int CTDLTaskAttributeListCtrl::CAttributeStates::SortByNameProc(const void* item1, const void* item2)
{
	const ATTRIBSTATE* pItem1 = (const ATTRIBSTATE*)item1;
	const ATTRIBSTATE* pItem2 = (const ATTRIBSTATE*)item2;

	int nCompare = Misc::NaturalCompare(pItem1->sLabel, pItem2->sLabel);

	// Sort by ID for a stable sort
	if (nCompare == 0)
		nCompare = pItem1->sLabel.Compare(pItem2->sLabel);

	return nCompare;
}

int CTDLTaskAttributeListCtrl::CAttributeStates::SortByPosProc(const void* item1, const void* item2)
{
	const ATTRIBSTATE* pItem1 = (const ATTRIBSTATE*)item1;
	const ATTRIBSTATE* pItem2 = (const ATTRIBSTATE*)item2;

	int nCompare = (pItem1->nPos - pItem2->nPos);

	// Sort by ID for a stable sort
	if (nCompare == 0)
		nCompare = (pItem1->nAttribID - pItem2->nAttribID);

	return nCompare;
}

void CTDLTaskAttributeListCtrl::CAttributeStates::RebuildItemPositions()
{
	m_mapPositions.RemoveAll();

	int nItem = m_aAttributeItems.GetSize();

	while (nItem--)
	{
		ATTRIBSTATE& item = m_aAttributeItems[nItem];

		item.nPos = nItem;
		m_mapPositions[item.nAttribID] = nItem;
	}
}

void CTDLTaskAttributeListCtrl::CAttributeStates::OnCustomAttributesChange()
{
	CString sState;

	if (CanResetOrder())
	{
		sState = GetItemsState();
		ASSERT(!sState.IsEmpty());
	}

	m_aAttributeItems.RemoveAll();
	Populate();

	if (!sState.IsEmpty())
		SetItemsState(sState);
}

void CTDLTaskAttributeListCtrl::CAttributeStates::Save(CPreferences& prefs, LPCTSTR szKey) const
{
	prefs.WriteProfileString(szKey, _T("AttribState"), GetItemsState());

	// Remove this when 9.1 goes to Beta //////////////////////////
	prefs.DeleteProfileEntry(szKey, _T("AttribOrder"));
	///////////////////////////////////////////////////////////////
}

void CTDLTaskAttributeListCtrl::CAttributeStates::Load(const CPreferences& prefs, LPCTSTR szKey)
{
	CString sState = prefs.GetProfileString(szKey, _T("AttribState"));

	// Remove this when 9.1 goes to Beta //////////////////////////
	if (sState.IsEmpty())
		sState = prefs.GetProfileString(szKey, _T("AttribOrder"));
	///////////////////////////////////////////////////////////////

	SetItemsState(sState);
}

CString CTDLTaskAttributeListCtrl::CAttributeStates::GetItemsState(BOOL bIncBkgndColors) const
{
	CStringArray aStates;
	aStates.SetSize(m_aAttributeItems.GetSize());

	int nItem = m_aAttributeItems.GetSize();

	while (nItem--)
	{
		const ATTRIBSTATE& item = m_aAttributeItems[nItem];

		if (item.IsCustom())
			aStates[nItem] = item.sCustAttribID;
		else
			aStates[nItem] = Misc::Format(item.nAttribID);

		if (bIncBkgndColors && (item.crLabelBkgnd != CLR_NONE))
		{
			aStates[nItem] += STATE_DELIM;
			aStates[nItem] += Misc::Format(item.crLabelBkgnd);
		}
	}

	return Misc::FormatArray(aStates, ITEM_DELIM);
}

void CTDLTaskAttributeListCtrl::CAttributeStates::SetItemsState(const CString& sState, BOOL bIncBkgndColors)
{
	CStringArray aStates;
	int nNumItems = Misc::Split(sState, aStates, ITEM_DELIM);

	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		CString sAttribID = aStates[nItem], sBkgndColor;
		Misc::Split(sAttribID, sBkgndColor, STATE_DELIM);

		TDC_ATTRIBUTE nAttribID = TDCA_NONE;

		if (Misc::IsNumber(sAttribID))
			nAttribID = (TDC_ATTRIBUTE)_ttoi(sAttribID);
		else
			nAttribID = m_aCustomAttribDefs.GetAttributeID(sAttribID);

		int nOldPos = GetAttribPos(nAttribID);

		if (nOldPos != -1)
		{
			m_aAttributeItems[nOldPos].nPos = nItem;

			if (bIncBkgndColors)
			{
				if (!sBkgndColor.IsEmpty())
					m_aAttributeItems[nOldPos].crLabelBkgnd = _ttoi(sBkgndColor);
				else
					m_aAttributeItems[nOldPos].crLabelBkgnd = CLR_NONE;
			}
		}
	}

	Misc::SortArrayT<ATTRIBSTATE>(m_aAttributeItems, SortByPosProc);
	RebuildItemPositions();
}

BOOL CTDLTaskAttributeListCtrl::CAttributeStates::ResetOrder()
{
	if (!CanResetOrder())
		return FALSE;

	Misc::SortArrayT<ATTRIBSTATE>(m_aAttributeItems, SortByNameProc);
	RebuildItemPositions();

	return TRUE;
}

BOOL CTDLTaskAttributeListCtrl::CAttributeStates::CanResetOrder() const
{
	return (GetItemsState(FALSE) != m_sDefaultOrder);
}

BOOL CTDLTaskAttributeListCtrl::CAttributeStates::SetLabelBkgndColor(TDC_ATTRIBUTE nAttribID, COLORREF crBkgnd)
{
	int nPos = GetAttribPos(nAttribID);

	if (nPos == -1)
	{
		ASSERT(nAttribID == TDCA_NONE);
		return FALSE;
	}

	if (crBkgnd == m_aAttributeItems[nPos].crLabelBkgnd)
		return FALSE;

	m_aAttributeItems[nPos].crLabelBkgnd = crBkgnd;
	return TRUE;
}

BOOL CTDLTaskAttributeListCtrl::CAttributeStates::ClearLabelBkgndColor(TDC_ATTRIBUTE nAttribID)
{
	return SetLabelBkgndColor(nAttribID, CLR_NONE);
}

COLORREF CTDLTaskAttributeListCtrl::CAttributeStates::GetLabelBkgndColor(TDC_ATTRIBUTE nAttribID) const
{
	int nPos = GetAttribPos(nAttribID);

	if (nPos == -1)
	{
		ASSERT(nAttribID == TDCA_NONE);
		return CLR_NONE;
	}

	return m_aAttributeItems[nPos].crLabelBkgnd;
}

////////////////////////////////////////////////////////////////////////////////
