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

#include "..\shared\EnMenu.h"
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
	ID_BTN_NONE = -1,
	ID_BTN_DEFAULT = 0,

	ID_BTN_TIMETRACK = 10,
	ID_BTN_ADDLOGGEDTIME,
	ID_BTN_VIEWDEPENDS,
	ID_BTN_EDITDEPENDS,
	ID_BTN_BROWSEFILE,
	ID_BTN_VIEWFILE,
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

#define CUSTOMTIMEATTRIBID(dateID) ((TDC_ATTRIBUTE)(dateID + CUSTOMTIMEATTRIBOFFSET))

/////////////////////////////////////////////////////////////////////////////

const int COMBO_DROPHEIGHT	= GraphicsMisc::ScaleByDPIFactor(200);
const int ICON_SIZE			= GraphicsMisc::ScaleByDPIFactor(16);
const int SPLITTER_WIDTH	= GraphicsMisc::ScaleByDPIFactor(6);
const int MIN_COL_WIDTH		= (4 * EE_BTNWIDTH_DEFAULT);

const int VALUE_VARIES = 1;
const int TIMEPERIOD_DECPLACES = 6; // Preserve full(ish) precision

const TCHAR NEWLINE = '\n';

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
	m_reminders(rems),
	m_eSingleFileLink(FES_GOBUTTON),
	m_cbMultiFileLink(FES_GOBUTTON),
	m_iconCache(FALSE), // small icons
	m_dwTimeTrackingTask(0)
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
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CAPTURECHANGED()
	ON_WM_KEYDOWN()
	ON_WM_CONTEXTMENU()

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
	ON_CONTROL_RANGE(CBN_SELCHANGE, 0, 0xffff, OnComboEditChange)

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

	CLocalizer::EnableTranslation(m_cbTextAndNumbers, FALSE);
	CLocalizer::EnableTranslation(m_cbPriority, FALSE);
	CLocalizer::EnableTranslation(m_cbRisk, FALSE);
	CLocalizer::EnableTranslation(m_cbMultiFileLink, FALSE);

	VERIFY(m_dropFiles.Register(this));

	VERIFY(m_tooltip.Create(this));
	m_tooltip.EnableMultilineTips();

	return 0;
}

void CTDLTaskAttributeListCtrl::ToggleSortDirection()
{
	m_bSortAscending = !m_bSortAscending;

	if (m_bGrouped)
		Populate();
	else
		Sort();
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
			return CInputListCtrl::CompareItems(nDateAttribID1, nDateAttribID2, nSortColumn);
		}

		// else if the first item is a time field
		if (bAttrib1IsTime)
		{
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
			return CInputListCtrl::CompareItems(nDateAttribID1, dwItemData2, nSortColumn);
		}

		// else second item must be a time field
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
		return CInputListCtrl::CompareItems(dwItemData1, nDateAttribID2, nSortColumn);
	}

	return CInputListCtrl::CompareItems(dwItemData1, dwItemData2, nSortColumn);
}

void CTDLTaskAttributeListCtrl::ToggleGrouping()
{
	m_bGrouped = !m_bGrouped;

	if (GetSafeHwnd())
	{
		EnableGroupView(m_bGrouped);
		Populate();
	}
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
}

void CTDLTaskAttributeListCtrl::LoadState(const CPreferences& prefs, LPCTSTR szKey)
{
	m_fAttribColProportion = (float)prefs.GetProfileDouble(szKey, _T("AttribColProportion"), 0.5);
	m_bSortAscending = prefs.GetProfileInt(szKey, _T("AttribSortAscending"), TRUE);
	m_bGrouped = prefs.GetProfileInt(szKey, _T("AttribGrouped"), FALSE);

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
	m_dwTimeTrackingTask = dwTaskID;

	if (m_multitasker.AnyTaskHasID(m_aSelectedTaskIDs, dwTaskID, FALSE))
		RedrawValue(TDCA_TIMESPENT);
}

void CTDLTaskAttributeListCtrl::OnCustomAttributesChange()
{
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
	case TDCA_PROJECTNAME:
	case TDCA_COMMENTS:
		return FALSE;

	case TDCA_TASKNAME:
		return TRUE;

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
			const TDCATTRIBUTE& attrib = ATTRIBUTES[nAtt];

			if ((attrib.nGroup == nGroup) && WantAddAttribute(attrib.nAttributeID))
				mapAttrib[attrib.nAttributeID] = CEnString(attrib.nAttribResID);
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
				CEnString sAttrib(IDS_CUSTOMCOLUMN, attribDef.sLabel);

				mapAttrib[nAttribID] = sAttrib;

				if (attribDef.IsDataType(TDCCA_DATE) && attribDef.HasFeature(TDCCAF_SHOWTIME))
					mapAttrib[CUSTOMTIMEATTRIBID(nAttribID)] = sAttrib;
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
				CheckAddAttribute(ATTRIBUTES[nAtt].nAttributeID, ATTRIBUTES[nAtt].nAttribResID);

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

	if (nAttribColWidth != GetColumnWidth(ATTRIB_COL))
	{
		CHoldRedraw hr(*this);

		SetColumnWidth(ATTRIB_COL, nAttribColWidth);
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

		m_fAttribColProportion = ((float)GetColumnWidth(ATTRIB_COL)) / (rClient.Width() - 1);
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
	CRect rSplitBar;
	GetSplitterRect(rSplitBar);

	CPoint ptClient(::GetMessagePos());
	ScreenToClient(&ptClient);

	if (rSplitBar.PtInRect(ptClient))
		return GraphicsMisc::SetAfxCursor(AFX_IDC_HSPLITBAR);

	if (m_aSelectedTaskIDs.GetSize())
	{
		LVHITTESTINFO lvHit = { { ptClient.x, ptClient.y }, 0 };

		int nRow = SubItemHitTest(&lvHit);
		int nCol = lvHit.iSubItem;

		if (nCol == VALUE_COL)
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

	if (bResolveCustomTimeFields && IsCustomTime(nAttribID))
		nAttribID = MapTimeToDate(nAttribID);

	return nAttribID;
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
	return GetItemText(nRow, ATTRIB_COL);
}

IL_COLUMNTYPE CTDLTaskAttributeListCtrl::GetCellType(int nRow, int nCol) const
{
	if (nCol == ATTRIB_COL)
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
	
	if (m_data.HasStyle(TDCS_READONLY))
		return FALSE;

	if (!CInputListCtrl::CanEditCell(nRow, nCol))
		return FALSE;

	TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);

	if (!m_multitasker.AnyTaskIsUnlocked(m_aSelectedTaskIDs))
	{
		switch (nAttribID)
		{
		case TDCA_LOCK:
		case TDCA_REMINDER:
			break;

		default:
			return FALSE;
		}
	}

	// else
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
	case TDCA_TIMEREMAINING:
		// Permanently read-only fields
		return FALSE;

	case TDCA_RECURRENCE:
	case TDCA_REMINDER:
		return !m_multitasker.AllTasksAreDone(m_aSelectedTaskIDs);

	case TDCA_PERCENT:
		if (m_data.HasStyle(TDCS_AUTOCALCPERCENTDONE))
		{
			return FALSE;
		}
		else if (m_data.HasStyle(TDCS_AVERAGEPERCENTSUBCOMPLETION) && 
				 m_multitasker.AnyTaskIsParent(m_aSelectedTaskIDs))
		{
			return FALSE;
		}
		break;

	case TDCA_LOCK:
		return TRUE;

	case TDCA_STARTTIME:
		if (!m_multitasker.AnyTaskHasDate(m_aSelectedTaskIDs, TDCD_STARTDATE))
			return FALSE;
		// else fall through to TDCA_STARTDATE

	case TDCA_STARTDATE:
		return (!m_data.HasStyle(TDCS_AUTOADJUSTDEPENDENCYDATES) || !m_multitasker.AllTasksHaveDependencies(m_aSelectedTaskIDs));

	case TDCA_DUETIME:		
		return m_multitasker.AnyTaskHasDate(m_aSelectedTaskIDs, TDCD_DUEDATE);

	case TDCA_DONETIME:		
		return m_multitasker.AnyTaskHasDate(m_aSelectedTaskIDs, TDCD_DONEDATE);

	case TDCA_TIMEESTIMATE:
	case TDCA_TIMESPENT:
		return (m_data.HasStyle(TDCS_ALLOWPARENTTIMETRACKING) || !m_multitasker.AllTasksAreParents(m_aSelectedTaskIDs));

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
	if (!CanEditCell(nItem, VALUE_COL))
		return GetSysColor(COLOR_3DDKSHADOW);

	if (nCol == VALUE_COL)
	{
		switch (GetAttributeID(nItem))
		{
		case TDCA_DEPENDENCY:
 			if (m_multitasker.AnyTaskHasLocalCircularDependencies(m_aSelectedTaskIDs))
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
	
	if (m_data.HasStyle(TDCS_SHOWDATESINISO))
		dwFlags |= DHFD_ISO;
	
	if (bAndTime)
		dwFlags |= DHFD_TIME | DHFD_NOSEC;

	return CDateHelper::FormatDate(date,  dwFlags);
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

#define GETMULTIVALUE_DATE(DT, ANDTIME)							\
{ COleDateTime value;											\
if (m_multitasker.GetTasksDate(m_aSelectedTaskIDs, DT, value))	\
{ sValue = FormatDate(value, ANDTIME); }						\
else { sValue = DATETIME_VARIES; bValueVaries = TRUE; } }

// -----------------------------------------------------------------------------------------

#define GETMULTIVALUE_TIME(DT)									\
{ COleDateTime value;											\
if (m_multitasker.GetTasksDate(m_aSelectedTaskIDs, DT, value))	\
{ sValue = FormatTime(value, TRUE); }							\
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
else if (nSelCount > 1) bValueVaries = TRUE; }

// -----------------------------------------------------------------------------------------

#define GETMULTIVALUE_BOOL(FUNCTION)					\
{ BOOL value;											\
if (m_multitasker.FUNCTION(m_aSelectedTaskIDs, value))	\
sValue = (value ? _T("+") : _T(""));					\
else bValueVaries = TRUE; }

// -----------------------------------------------------------------------------------------

void CTDLTaskAttributeListCtrl::RefreshSelectedTasksValue(int nRow)
{
	int nSelCount = m_aSelectedTaskIDs.GetSize();
	DWORD dwSingleSelTaskID = ((nSelCount == 1) ? m_aSelectedTaskIDs[0] : 0);
	TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow);

	CString sValue;
	BOOL bValueVaries = FALSE;

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
	case TDCA_COMMENTSSIZE:		GETMULTIVALUE_STR(GetTasksCommentsSize);	break;
 	case TDCA_SUBTASKDONE:		GETMULTIVALUE_STR(GetTasksSubtaskCompletion); break;

	case TDCA_ALLOCTO:			GETMULTIVALUE_LIST(GetTasksAllocatedTo);	break;
	case TDCA_CATEGORY:			GETMULTIVALUE_LIST(GetTasksCategories);		break;
	case TDCA_TAGS:				GETMULTIVALUE_LIST(GetTasksTags);			break;

	case TDCA_FLAG:				GETMULTIVALUE_BOOL(GetTasksFlagState);		break;
	case TDCA_LOCK:				GETMULTIVALUE_BOOL(GetTasksLockState);		break;

	case TDCA_PRIORITY:			GETMULTIVALUE_FMT(GetTasksPriority,			int,		Misc::Format(value));	break;
	case TDCA_RISK:				GETMULTIVALUE_FMT(GetTasksRisk,				int,		Misc::Format(value));	break;
	case TDCA_COLOR:			GETMULTIVALUE_FMT(GetTasksColor,			COLORREF,	Misc::Format(value));	break;
	case TDCA_PARENTID:			GETMULTIVALUE_FMT(GetTasksParentID,			DWORD,		Misc::Format(value));	break;

	case TDCA_COST:				GETMULTIVALUE_FMT(GetTasksCost,				TDCCOST,				value.Format(2));			break;
	case TDCA_RECURRENCE:		GETMULTIVALUE_FMT(GetTasksRecurrence,		TDCRECURRENCE,			value.GetRegularityText());	break;
	case TDCA_DEPENDENCY:		GETMULTIVALUE_FMT(GetTasksDependencies,		CTDCDependencyArray,	value.Format());			break;
	case TDCA_TIMEREMAINING:	GETMULTIVALUE_FMT(GetTasksTimeRemaining,	TDCTIMEPERIOD,			value.Format(2));			break;
	
	case TDCA_FILELINK:	
		{
			CStringArray aFileLinks;
			
			if (m_multitasker.GetTasksFileLinks(m_aSelectedTaskIDs, aFileLinks))
				sValue = Misc::FormatArray(aFileLinks);
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

	case TDCA_DONEDATE:			GETMULTIVALUE_DATE(TDCD_DONEDATE, FALSE);		break;
	case TDCA_DUEDATE:			GETMULTIVALUE_DATE(TDCD_DUEDATE, FALSE);		break;
	case TDCA_STARTDATE:		GETMULTIVALUE_DATE(TDCD_STARTDATE, FALSE);		break;
	case TDCA_LASTMODDATE:		GETMULTIVALUE_DATE(TDCD_LASTMOD, TRUE);			break;
	case TDCA_CREATIONDATE:		GETMULTIVALUE_DATE(TDCD_CREATE, TRUE);			break;

	case TDCA_DONETIME:			GETMULTIVALUE_TIME(TDCD_DONETIME);				break;
	case TDCA_DUETIME:			GETMULTIVALUE_TIME(TDCD_DUETIME);				break;
	case TDCA_STARTTIME:		GETMULTIVALUE_TIME(TDCD_STARTTIME);				break;

	case TDCA_POSITION:			GETUNIQUEVALUE(m_formatter.GetTaskPosition);	break;
	case TDCA_ID:				GETUNIQUEVALUE(Misc::Format);					break;

	case TDCA_REMINDER:
		{
			time_t tValue = 0;
			
			if (m_multitasker.GetTasksReminder(m_aSelectedTaskIDs, m_reminders, tValue))
			{
				if (tValue != 0)
					sValue = FormatDate(tValue, TRUE);
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

					case TDCCA_CALCULATION:
						sValue = m_formatter.GetTaskCustomAttributeData(m_aSelectedTaskIDs[0], *pDef);
						break;

					case TDCCA_TIMEPERIOD:
						sValue = pDef->FormatTimePeriod(data);
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
				sValue = CTimeHelper::FormatClockTime(data.AsDate(), FALSE, m_data.HasStyle(TDCS_SHOWDATESINISO));
			else
				bValueVaries = TRUE;
		}
		break;
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
		// We always get button zero because we advance the button rect each time
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
		case TDCA_REMINDER:
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
				sPrompt = CTimeHelper::FormatClockTime(23, 59, 0, FALSE, m_data.HasStyle(TDCS_SHOWDATESINISO));
			break;

		case TDCA_STARTTIME:
			if (sText.IsEmpty())
				sPrompt = CTimeHelper::FormatClockTime(0, 0, 0, FALSE, m_data.HasStyle(TDCS_SHOWDATESINISO));
			break;

		case TDCA_CATEGORY:
		case TDCA_TAGS:
		case TDCA_FILELINK:
		case TDCA_ALLOCTO:
		case TDCA_DEPENDENCY:
			bValueVaries = (sText.Find('|') != -1);
			break;

		default:
			if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
			{
				const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
				GET_CUSTDEF_RET(m_aCustomAttribDefs, nAttribID, pDef, FALSE);

				if (pDef->IsList())
					bValueVaries = (sText.Find('|') != -1);
			}
			else if (IsCustomTime(nAttribID))
			{
				if (sText.IsEmpty())
					sPrompt = CTimeHelper::FormatClockTime(23, 59, 0, FALSE, m_data.HasStyle(TDCS_SHOWDATESINISO));
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

	if (nCol == ATTRIB_COL)
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

				CString sPriority = sText + Misc::Format(_T(" (%s)"), CEnString(IDS_PRIORITYRISK_SCALE[nPriority]));
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
				CString sPriority = sText + Misc::Format(_T(" (%s)"), CEnString(IDS_PRIORITYRISK_SCALE[nRisk]));
				CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, sPriority, crText, nDrawTextFlags);
			}
		}
		return;

	case TDCA_ICON:
		DrawIcon(pDC, sText, rText, FALSE);
		return;

	case TDCA_COST:
		CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, m_formatter.GetCost(Misc::Atof(sText)), crText, nDrawTextFlags);
		return;

	case TDCA_COLOR:
		{
			COLORREF color = _ttoi(sText);

			switch (color)
			{
			case CLR_NONE:
			case 0:
				break;

			default:
				if (m_data.HasStyle(TDCS_TASKCOLORISBACKGROUND))
				{
					// Use the entire cell rect for the background colour
					CRect rCell;
					GetCellRect(nRow, nCol, rCell);

					pDC->FillSolidRect(rCell, color);
					crText = GraphicsMisc::GetBestTextColor(color);
				}
				else
				{
					crText = color;
				}
				break;
			}

			CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, CEnString(IDS_COLOR_SAMPLETEXT), crText, nDrawTextFlags);
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
		}
		break;

	case TDCA_FILELINK:
		{
			CStringArray aFiles;
			int nNumFiles = Misc::Split(sText, aFiles);

			CRect rFile(rText);

			for (int nFile = 0; nFile < nNumFiles; nFile++)
			{
				CString sFile = aFiles[nFile];

				if (DrawIcon(pDC, sFile, rFile, TRUE))
					rFile.left += (ICON_SIZE + 2);

				if (!TDCTASKLINK::IsTaskLink(aFiles[0], TRUE))
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
	
				if (DrawIcon(pDC, m_data.GetTaskIcon(dwDependsID), rTitle, FALSE))
					rTitle.left += (ICON_SIZE + 2);

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

					if (DrawIcon(pDC, sText, rText, TRUE))
						rRest.left += (ICON_SIZE + 2);
	
					CInputListCtrl::DrawCellText(pDC, nRow, nCol, rRest, FileMisc::GetFileNameFromPath(sText), crText, nDrawTextFlags);
				}
				return;

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

			case TDCCA_INTEGER:
			case TDCCA_DOUBLE:
				CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, pDef->FormatNumber(Misc::Atof(sText)), crText, nDrawTextFlags);
				return;

			default:
				if (pDef->IsMultiList())
				{
					CString sMatched(sText), sUnused;
					Misc::Split(sMatched, sUnused, '|');

					CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, sMatched, crText, nDrawTextFlags);
					return;
				}
				break;
			}
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

BOOL CTDLTaskAttributeListCtrl::DrawIcon(CDC* pDC, const CString& sIcon, const CRect& rText, BOOL bIconIsFile)
{
	if (sIcon.IsEmpty())
		return FALSE;

	CPoint ptIcon(GetIconPos(rText));

	if (bIconIsFile)
	{
		return CFileEdit::DrawFileIcon(pDC, 
									   sIcon,
									   ptIcon, 
									   m_iconCache,
									   this,
									   m_sCurrentFolder,
									   m_data.HasStyle(TDCS_SHOWFILELINKTHUMBNAILS));
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
	return Misc::Split(GetValueText(TDCA_FILELINK), aFiles);
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
		COleDateTime date;
		
		if (CDateHelper::DecodeDate(sValue, date, FALSE))
		{
			if (pDef->HasFeature(TDCCAF_SHOWTIME))
			{
				TDC_ATTRIBUTE nTimeAttribID = CUSTOMTIMEATTRIBID(nAttribID);
				date.m_dt += (CTimeHelper::DecodeClockTime(GetValueText(nTimeAttribID)) / 24);
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

BOOL CTDLTaskAttributeListCtrl::CheckRecreateCombo(int nRow, CEnCheckComboBox& combo)
{
	BOOL bIsReadOnly = !CDialogHelper::ComboHasEdit(combo), bWantReadOnly(bIsReadOnly);
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

	if (Misc::StateChanged(bIsReadOnly, bWantReadOnly))
	{
		if (!CDialogHelper::SetAutoComboReadOnly(combo, TRUE, bWantReadOnly, COMBO_DROPHEIGHT))
		{
			ASSERT(0);
			return FALSE;
		}
	}

	combo.ModifyFlags((bWantReadOnly ? (ACBS_ALLOWDELETE | ACBS_AUTOCOMPLETE) : 0),
						(bWantReadOnly ? 0 : (ACBS_ALLOWDELETE | ACBS_AUTOCOMPLETE)));
	return TRUE;
}

void CTDLTaskAttributeListCtrl::PrepareMultiSelCombo(int nRow, const CStringArray& aDefValues, const CStringArray& aUserValues, CEnCheckComboBox& combo, BOOL bWantSort)
{
	CheckRecreateCombo(nRow, combo);

	combo.ModifyStyle(bWantSort ? 0 : CBS_SORT, bWantSort ? CBS_SORT : 0);
	combo.EnableMultiSelection(TRUE);
	combo.ResetContent();
	combo.AddStrings(aDefValues);
	combo.AddUniqueItems(aUserValues);

	CStringArray aMatched, aMixed;
	ParseMultiSelValues(GetItemText(nRow, VALUE_COL), aMatched, aMixed);

	combo.SetChecked(aMatched, aMixed);
}

void CTDLTaskAttributeListCtrl::PrepareSingleSelCombo(int nRow, const CStringArray& aDefValues, const CStringArray& aUserValues, CEnCheckComboBox& combo, BOOL bWantSort)
{
	CheckRecreateCombo(nRow, combo);

	combo.ModifyStyle(bWantSort ? 0 : CBS_SORT, bWantSort ? CBS_SORT : 0);
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
	m_editBox.ClearMask();

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
			CString sValue = GetItemText(nRow, nCol);

			if (sValue.IsEmpty())
			{
				PrepareSingleFileEdit(nRow, sValue);
			}
			else
			{
				CStringArray aFiles;

				if (Misc::Split(sValue, aFiles))
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
	case TDCA_TASKNAME:
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
			m_cbPriority.SetColors(m_aPriorityColors);

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

	COleDateTime date;

	if (CDateHelper::DecodeDate(sValue, date, FALSE))
		m_datePicker.SetTime(date);
	else
		m_datePicker.SendMessage(DTM_SETSYSTEMTIME, GDT_NONE, 0);

	m_datePicker.SetMonthCalStyle(MCS_WEEKNUMBERS);
}

void CTDLTaskAttributeListCtrl::PrepareTimeOfDayCombo(int nRow)
{
	CString sValue = GetItemText(nRow, VALUE_COL);

	if (sValue.IsEmpty() || (sValue == DATETIME_VARIES))
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

void CTDLTaskAttributeListCtrl::EditCell(int nRow, int nCol, BOOL bBtnClick)
{
	ASSERT(CanEditCell(nRow, nCol));

	CWnd* pCtrl = GetEditControl(nRow, bBtnClick);

	if (pCtrl != NULL)
	{
		if (pCtrl == CInputListCtrl::GetEditControl())
		{
			PrepareControl(m_editBox, nRow, nCol);
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

	case TDCA_REMINDER:
		GetParent()->SendMessage(WM_TDCM_EDITTASKREMINDER);
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
	GetButtonRect(nRow, VALUE_COL, rBtn);

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

	BOOL bDisabled = (dwState & DFCS_INACTIVE);

	switch (nBtnID)
	{
	case ID_BTN_TIMETRACK:		return GetButtonIcon(ICON_TRACKTIME, bDisabled);
	case ID_BTN_ADDLOGGEDTIME:	return GetButtonIcon(ICON_ADDTIME, bDisabled);
	case ID_BTN_VIEWDEPENDS:	return GetButtonIcon(ICON_SHOWDEPENDS, bDisabled);
	case ID_BTN_VIEWFILE:		return GetButtonIcon(ICON_VIEWFILE, bDisabled);
	case ID_BTN_BROWSEFILE:		return GetButtonIcon(ICON_BROWSEFILE, bDisabled);

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
	}

	return NULL;
}

BOOL CTDLTaskAttributeListCtrl::CanClickButton(TDC_ATTRIBUTE nAttribID, int nBtnID, const CString& sCellText) const
{
	ASSERT(CanEditCell(GetRow(nAttribID), VALUE_COL));

	switch (nBtnID)
	{
	case ID_BTN_TIMETRACK:
		return (m_aSelectedTaskIDs.GetSize() == 1);

	case ID_BTN_ADDLOGGEDTIME:
		return ((m_aSelectedTaskIDs.GetSize() == 1) && (m_aSelectedTaskIDs[0] != m_dwTimeTrackingTask));

	case ID_BTN_VIEWDEPENDS:
	case ID_BTN_VIEWFILE:
		return !sCellText.IsEmpty();

	case ID_BTN_EDITDEPENDS:
	case ID_BTN_BROWSEFILE:
		return TRUE;

	case ID_BTN_DEFAULT:
		switch (nAttribID)
		{
		case TDCA_TIMESPENT:
			if ((m_aSelectedTaskIDs.GetSize() == 1) && (m_aSelectedTaskIDs[0] == m_dwTimeTrackingTask))
				return FALSE;
			break;
		}
		return TRUE;

	default:
		ASSERT(0);
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
	OnComboEditChange(IDC_TIME_PICKER);
}

void CTDLTaskAttributeListCtrl::OnComboCloseUp(UINT nCtrlID) 
{ 
	CWnd* pCombo = GetDlgItem(nCtrlID);

	// If the combo has already been hidden by the base class
	// we can ignore this
	if (!pCombo->IsWindowVisible())
		return;

	// If the combo has an edit field AND the user clicked inside 
	// the edit field to close the combo, DON'T hide the combo
	CWnd* pEdit = pCombo->GetDlgItem(1001);

	if (pEdit && CDialogHelper::IsMouseDownInWindow(*pEdit))
		return;

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
		OnComboEditChange(nCtrlID);
		break;
	}
}

void CTDLTaskAttributeListCtrl::OnComboEditChange(UINT nCtrlID)
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
			double dTime = m_cbTimeOfDay.Get24HourTime();

			if (dTime != 0.0)
				sNewValue = CTimeHelper::FormatClockTime(dTime / 24);

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
				sNewValue = Misc::FormatArray(aFiles);
		}
		break;

	case IDC_CUSTOMICON_COMBO:
		{
			CStringArray aMatched, aMixed;
			m_cbCustomIcons.GetChecked(aMatched, aMixed);

			sNewValue = FormatMultiSelItems(aMatched, aMixed);
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

	// If the field has already been hidden by the base class
	// we can ignore this
	if (!m_datePicker.IsWindowVisible())
		return;

	// If the use clicked in the date field DON'T hide it
	if (CDialogHelper::IsMouseDownInWindow(m_datePicker))
	{
		DATETIMEPICKERINFO dtpi = { 0 };

		if (m_datePicker.GetPickerInfo(dtpi))
		{
			CPoint ptMsg(::GetMessagePos());
			m_datePicker.ScreenToClient(&ptMsg);

			if (!::PtInRect(&dtpi.rcButton, ptMsg) &&
				!::PtInRect(&dtpi.rcCheck, ptMsg))
			{
				return;
			}
		}
	}

	HideControl(m_datePicker); 

	// If we've got multiple different values, DON'T notify
	// our parent if the cell text has not actually changed
	int nRow = GetCurSel();

	if (RowValueVaries(nRow) && (GetItemText(nRow, VALUE_COL) == DATETIME_VARIES))
		return;
	
	NotifyParentEdit(nRow);

	*pResult = 0;
}

void CTDLTaskAttributeListCtrl::OnDateKillFocus(NMHDR* pNMHDR, LRESULT* pResult)
{
	OnDateCloseUp(pNMHDR, pResult);
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
			
			OnDateCloseUp(pNMHDR, pResult);
		}
		else
		{
			// Use the cell text as a scratch-pad for storing intermediate
			// date edits but without notifying our parent
			VERIFY(SetItemText(GetCurSel(), VALUE_COL, m_formatter.GetDateOnly(pNMDTC->st, TRUE)));
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
		Misc::Split(m_pAttributeList->GetItemText(nRow, VALUE_COL), aExisting);

		if (Misc::AddUniqueItems(aFiles, aExisting))
		{
			m_pAttributeList->SetValueText(nRow, Misc::FormatArray(aExisting));
		}
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
	CRect rBtn;
	
	if (GetButtonRect(nRow, nCol, rBtn) && rBtn.PtInRect(point))
	{
		int nBtnID = HitTestButtonID(nRow, rBtn);

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

			case TDCA_TIMEESTIMATE:
			case TDCA_TIMESPENT:
			case TDCA_TIMEREMAINING:
				sTooltip.LoadString(IDS_TIMEUNITS);
				break;

			case TDCA_FILELINK:
				sTooltip.LoadString(IDS_BROWSE);
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

		if (!sTooltip.IsEmpty())
			return CToolTipCtrlEx::SetToolInfo(*pTI, *this, sTooltip, MAKELONG(nRow, nCol) + nBtnID);
	}
	
	// Value tooltips
	switch (nCol)
	{
	case ATTRIB_COL:
		if (IsCustomTime(nAttribID))
			sTooltip.LoadString(IDS_ATTRIBTIP_TIMEOFDAY);
		break;

	case VALUE_COL:
		if (!RowValueVaries(nRow))
		{
			switch (nAttribID)
			{
			case TDCA_ALLOCTO:
			case TDCA_CATEGORY:
			case TDCA_TAGS:
			case TDCA_FILELINK:
				{
					CStringArray aValues;

					if (Misc::Split(GetItemText(nRow, nCol), aValues) > 1)
						sTooltip = Misc::FormatArray(aValues, NEWLINE);
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
					int nDateRow = GetDateRow(nAttribID);

					if (CanEditCell(nDateRow, nCol) && !RowValueVaries(nDateRow) && GetItemText(nDateRow, nCol).IsEmpty())
						sTooltip.LoadString(IDS_ATTRIBTIP_NODATESET);
				}
				break;

			case TDCA_RECURRENCE:
			case TDCA_REMINDER:
				if (m_multitasker.AllTasksAreDone(m_aSelectedTaskIDs))
					sTooltip.LoadString(IDS_ATTRIBTIP_COMPLETEDTASK);
				break;

			case TDCA_DEPENDENCY:
				{
					CTDCDependencyArray aDepends;
					
					if (aDepends.Parse(GetItemText(nRow, nCol)) > 1)
						sTooltip = m_formatter.GetDependencies(aDepends, NEWLINE);
				}
				break;

			default:
				if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
				{
					const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
					GET_CUSTDEF_RET(m_aCustomAttribDefs, nAttribID, pDef, NULL);

					if (pDef->IsMultiList())
					{
						switch (pDef->GetDataType())
						{
						case TDCCA_STRING:
							{
								CStringArray aValues;

								if (Misc::Split(GetItemText(nRow, nCol), aValues) > 1)
									sTooltip = Misc::FormatArray(aValues, NEWLINE);
							}
							break;
						}
					}
				}
				else if (IsCustomTime(nAttribID))
				{
					int nDateRow = GetDateRow(nAttribID);

					if (CanEditCell(nDateRow, nCol) && !RowValueVaries(nDateRow) && GetItemText(nDateRow, nCol).IsEmpty())
						sTooltip.LoadString(IDS_ATTRIBTIP_NODATESET);
				}
				break;
			}
		}
		break;
	}

	if (!sTooltip.IsEmpty())
		return CToolTipCtrlEx::SetToolInfo(*pTI, *this, sTooltip, MAKELONG(nRow, nCol));

	return -1;  // not found
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

void CTDLTaskAttributeListCtrl::OnContextMenu(CWnd* pWnd, CPoint pos)
{
	HideAllControls();

	LVHITTESTINFO lvHit = { { pos.x, pos.y }, 0 };
	ScreenToClient(&lvHit.pt);

	int nRow = SubItemHitTest(&lvHit);
	int nCol = lvHit.iSubItem;

	if (nCol != VALUE_COL)
		return; // eat

	SetCurSel(nRow, nCol);

	// Decide whether we should show a menu at all
	TDC_ATTRIBUTE nAttribID = GetAttributeID(nRow, TRUE);

	if (!GetParent()->SendMessage(WM_TDCM_CANCOPYTASKATTRIBUTE, nAttribID))
		return; // eat

	CMenu menu;

	if (menu.LoadMenu(IDR_MISC))
	{
		CMenu* pPopup = menu.GetSubMenu(MM_ATTRIBUTECTRL);

		// Prepare menu items
		BOOL bMultiSel = (m_aSelectedTaskIDs.GetSize() > 1);
		CString sAttrib = GetItemText(nRow, ATTRIB_COL);

		// Copy command
		CEnString sMenuText;

		if (GetParent()->SendMessage(WM_TDCM_CANCOPYTASKATTRIBUTE, nAttribID))
		{
			sMenuText.Format((bMultiSel ? IDS_ATTRIBCTRL_COPYATTRIBVALUES : IDS_ATTRIBCTRL_COPYATTRIBVALUE), sAttrib);

			CEnMenu::SetMenuString(*pPopup, ID_ATTRIBLIST_COPYATTRIBVALUES, sMenuText, MF_BYCOMMAND);
			pPopup->EnableMenuItem(ID_ATTRIBLIST_COPYATTRIBVALUES, MF_BYCOMMAND | MF_ENABLED);
		}
		else
		{
			pPopup->EnableMenuItem(ID_ATTRIBLIST_COPYATTRIBVALUES, MF_BYCOMMAND | MF_DISABLED);
		}

		// Paste command
		TDC_ATTRIBUTE nFromAttribID = TDCA_NONE;

		if (GetParent()->SendMessage(WM_TDCM_CANPASTETASKATTRIBUTE, nAttribID, (LPARAM)&nFromAttribID))
		{
			CString sFromAttrib = GetItemText(GetRow(nFromAttribID), ATTRIB_COL);
			sMenuText.Format((bMultiSel ? IDS_ATTRIBCTRL_PASTEATTRIBVALUES : IDS_ATTRIBCTRL_PASTEATTRIBVALUE), sFromAttrib, sAttrib);

			CEnMenu::SetMenuString(*pPopup, ID_ATTRIBLIST_PASTEATTRIBVALUES, sMenuText, MF_BYCOMMAND);
			pPopup->EnableMenuItem(ID_ATTRIBLIST_PASTEATTRIBVALUES, MF_BYCOMMAND | MF_ENABLED);
		}
		else
		{
			pPopup->EnableMenuItem(ID_ATTRIBLIST_PASTEATTRIBVALUES, MF_BYCOMMAND | MF_DISABLED);
		}

		// Clear command
		if (CanEditCell(nRow, nCol))
		{
			sMenuText.Format((bMultiSel ? IDS_ATTRIBCTRL_CLEARATTRIBVALUES : IDS_ATTRIBCTRL_CLEARATTRIBVALUE), sAttrib);

			CEnMenu::SetMenuString(*pPopup, ID_ATTRIBLIST_CLEARATTRIBVALUES, sMenuText, MF_BYCOMMAND);
			pPopup->EnableMenuItem(ID_ATTRIBLIST_CLEARATTRIBVALUES, MF_BYCOMMAND | MF_ENABLED);
		}
		else
		{
			pPopup->EnableMenuItem(ID_ATTRIBLIST_CLEARATTRIBVALUES, MF_BYCOMMAND | MF_DISABLED);
		}

		UINT nCmdID = ::TrackPopupMenu(*pPopup, TPM_RETURNCMD | TPM_LEFTALIGN | TPM_LEFTBUTTON,
									   pos.x, pos.y, 0, GetSafeHwnd(), NULL);

		switch (nCmdID)
		{
		case ID_ATTRIBLIST_COPYATTRIBVALUES:
			GetParent()->SendMessage(WM_TDCM_COPYTASKATTRIBUTE, nAttribID);
			break;

		case ID_ATTRIBLIST_PASTEATTRIBVALUES:
			GetParent()->SendMessage(WM_TDCM_PASTETASKATTRIBUTE, nAttribID);
			break;

		case ID_ATTRIBLIST_CLEARATTRIBVALUES:
			GetParent()->SendMessage(WM_TDCM_CLEARTASKATTRIBUTE, nAttribID);
			break;
		}
	}
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
