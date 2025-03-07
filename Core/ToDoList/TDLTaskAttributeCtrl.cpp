// TDLTaskAttributeListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLTaskAttributeCtrl.h"
#include "tdcstruct.h"

#include "..\shared\Localizer.h"
#include "..\shared\EnMenu.h"

#include "..\3rdparty\XNamedColors.h"

#include "..\interfaces\UIThemeFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskAttributeDlgCtrl

CTDLTaskAttributeCtrl::CTDLTaskAttributeCtrl(const CToDoCtrlData& data,
											 const CContentMgr& mgrContent,
											 const CTDCImageList& ilIcons,
											 const TDCCOLEDITVISIBILITY& vis,
											 const CTDCReminderHelper& rems,
											 const CTDCCustomAttribDefinitionArray& aCustAttribDefs)
	:
	m_lcAttributes(data, 
				   mgrContent, 
				   ilIcons, 
				   vis, 
				   rems, 
				   aCustAttribDefs)
{
	m_toolbar.SetBorders(0, 0, 0, 1);
}

CTDLTaskAttributeCtrl::~CTDLTaskAttributeCtrl()
{
}

BEGIN_MESSAGE_MAP(CTDLTaskAttributeCtrl, CWnd)
	//{{AFX_MSG_MAP(CTDLTaskAttributeDlgCtrl)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()

	ON_COMMAND(ID_ATTRIBCTRL_TOGGLEGROUP, OnToggleGrouping)
	ON_COMMAND(ID_ATTRIBCTRL_TOGGLESORT, OnToggleSorting)
	ON_COMMAND(ID_ATTRIBCTRL_MOVEATTRIBUP, OnMoveAttributeUp)
	ON_COMMAND(ID_ATTRIBCTRL_MOVEATTRIBDOWN, OnMoveAttributeDown)
	ON_COMMAND(ID_ATTRIBCTRL_RESETMOVES, OnResetAttributeMoves)

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TASKATTRIBUTES, OnItemChanged)

	// These we just forward to our parent
	ON_REGISTERED_MESSAGE(WM_TDCM_EDITTASKATTRIBUTE, OnEditTaskAttribute)
	ON_REGISTERED_MESSAGE(WM_TDCM_EDITTASKREMINDER, OnEditTaskReminder)
	ON_REGISTERED_MESSAGE(WM_TDCN_ATTRIBUTEEDITED, OnAttributeEdited)
	ON_REGISTERED_MESSAGE(WM_TDCN_AUTOITEMADDEDDELETED, OnAutoItemAddedOrDeleted)
	ON_REGISTERED_MESSAGE(WM_TDCM_TOGGLETIMETRACKING, OnToggleTimeTracking)
	ON_REGISTERED_MESSAGE(WM_TDCM_ADDTIMETOLOGFILE, OnAddTimeToLogFile)
	ON_REGISTERED_MESSAGE(WM_TDCM_SELECTDEPENDENCIES, OnSelectDependencies)
	ON_REGISTERED_MESSAGE(WM_TDCM_GETLINKTOOLTIP, OnGetLinkTooltip)
	ON_REGISTERED_MESSAGE(WM_TDCM_DISPLAYLINK, OnDisplayLink)

	ON_REGISTERED_MESSAGE(WM_FE_GETFILEICON, OnGetFileIcon)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// -----------------------------------------------------------------------
// Pass-through message handlers

#define FWD_MSG(msg, name) LRESULT CTDLTaskAttributeCtrl::name(WPARAM wp, LPARAM lp) { return GetParent()->SendMessage(msg, wp, lp); }

FWD_MSG(WM_TDCM_EDITTASKATTRIBUTE,		OnEditTaskAttribute)
FWD_MSG(WM_TDCM_EDITTASKREMINDER,		OnEditTaskReminder)
FWD_MSG(WM_TDCN_ATTRIBUTEEDITED,		OnAttributeEdited)
FWD_MSG(WM_TDCN_AUTOITEMADDEDDELETED,	OnAutoItemAddedOrDeleted)
FWD_MSG(WM_TDCM_TOGGLETIMETRACKING,		OnToggleTimeTracking)
FWD_MSG(WM_TDCM_ADDTIMETOLOGFILE,		OnAddTimeToLogFile)
FWD_MSG(WM_TDCM_SELECTDEPENDENCIES,		OnSelectDependencies)
FWD_MSG(WM_TDCM_GETLINKTOOLTIP,			OnGetLinkTooltip)
FWD_MSG(WM_TDCM_DISPLAYLINK,			OnDisplayLink)
FWD_MSG(WM_FE_GETFILEICON,				OnGetFileIcon)

/////////////////////////////////////////////////////////////////////////////

// Public interface
BOOL CTDLTaskAttributeCtrl::Create(CWnd* pParent, UINT nID, const CRect& rPos)
{
	return CWnd::CreateEx(WS_EX_CONTROLPARENT,
						  NULL, 
						  NULL, 
						  (WS_CHILD | WS_VISIBLE | WS_TABSTOP), 
						  rPos, 
						  pParent, 
						  nID);
}

void CTDLTaskAttributeCtrl::SetUITheme(const CUIThemeFile& theme)
{
	m_toolbar.SetBackgroundColor(theme.crToolbarLight);
	m_toolbar.SetHotColor(theme.crToolbarHot);
}

// Message handlers
int CTDLTaskAttributeCtrl::OnCreate(LPCREATESTRUCT pCreateStruct)
{
	if (CWnd::OnCreate(pCreateStruct) != 0)
		return -1;

	// Create toolbar
	if (!m_toolbar.CreateEx(this, (TBSTYLE_FLAT | TBSTYLE_WRAPABLE), WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP))
		return -1;
		
	if (!m_toolbar.LoadToolBar(IDR_TASK_ATTRIBUTE_TOOLBAR, IDB_TASKATTRIB_TOOLBAR, colorMagenta))
		return -1;

	if (!m_tbHelper.Initialize(&m_toolbar))
		return -1;

	// Create List
	if (!m_lcAttributes.Create(this, IDC_TASKATTRIBUTES))
		return -1;

	return 0;
}

void CTDLTaskAttributeCtrl::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

	m_lcAttributes.SetFocus();
}

void CTDLTaskAttributeCtrl::OnCustomAttributesChange()
{
	m_lcAttributes.OnCustomAttributesChange();
}

void CTDLTaskAttributeCtrl::OnAttributeVisibilityChange()
{
	m_lcAttributes.OnAttributeVisibilityChange();
}

void CTDLTaskAttributeCtrl::OnSize(UINT nType, int cx, int cy)
{
	if (m_lcAttributes.GetSafeHwnd())
	{
		int nTBHeight = m_toolbar.Resize(cx);
		m_lcAttributes.MoveWindow(0, nTBHeight, cx, cy - nTBHeight);
	}

	CWnd::OnSize(nType, cx, cy);
}

void CTDLTaskAttributeCtrl::OnToggleGrouping()
{
	m_lcAttributes.ToggleGrouping();
	UpdateToolbarButtons();
}

void CTDLTaskAttributeCtrl::OnToggleSorting()
{
	m_lcAttributes.ToggleSortDirection();
	UpdateToolbarButtons();
}

void CTDLTaskAttributeCtrl::OnItemChanged(NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
	if (m_lcAttributes.IsSelectionChange((NMLISTVIEW*)pNMHDR))
		UpdateToolbarButtons();
}

void CTDLTaskAttributeCtrl::OnMoveAttributeUp()
{
	if (m_lcAttributes.MoveSelectedAttribute(TRUE))
		UpdateToolbarButtons();
}

void CTDLTaskAttributeCtrl::OnMoveAttributeDown()
{
	if (m_lcAttributes.MoveSelectedAttribute(FALSE))
		UpdateToolbarButtons();
}

void CTDLTaskAttributeCtrl::OnResetAttributeMoves()
{
	if (m_lcAttributes.ResetAttributeMoves())
		UpdateToolbarButtons();
}

void CTDLTaskAttributeCtrl::LoadState(const CPreferences& prefs, LPCTSTR szKey)
{
	m_lcAttributes.LoadState(prefs, szKey);
	UpdateToolbarButtons();
}

void CTDLTaskAttributeCtrl::SaveState(CPreferences& prefs, LPCTSTR szKey) const
{
	m_lcAttributes.SaveState(prefs, szKey);
}

void CTDLTaskAttributeCtrl::UpdateToolbarButtons()
{
	CToolBarCtrl& tb = m_toolbar.GetToolBarCtrl();
	
	tb.PressButton(ID_ATTRIBCTRL_TOGGLEGROUP, m_lcAttributes.IsGrouped());
	tb.EnableButton(ID_ATTRIBCTRL_MOVEATTRIBUP, m_lcAttributes.CanMoveSelectedAttribute(TRUE));
	tb.EnableButton(ID_ATTRIBCTRL_MOVEATTRIBDOWN, m_lcAttributes.CanMoveSelectedAttribute(FALSE));
	tb.EnableButton(ID_ATTRIBCTRL_RESETMOVES, m_lcAttributes.CanResetAttributeMoves());
}

void CTDLTaskAttributeCtrl::OnContextMenu(CWnd* pWnd, CPoint pos)
{
	int nRow, nCol;

	if (!m_lcAttributes.GetCurSel(nRow, nCol) || (nCol == 0))
		return; // eat

	// Decide whether we should show a menu at all
	TDC_ATTRIBUTE nAttribID = m_lcAttributes.GetSelectedAttributeID();

	if (!GetParent()->SendMessage(WM_TDCM_CANCOPYTASKATTRIBUTE, nAttribID))
		return; // eat

	CMenu menu;

	if (menu.LoadMenu(IDR_MISC))
	{
		CMenu* pPopup = menu.GetSubMenu(MM_ATTRIBUTECTRL);

		// Prepare menu items
		BOOL bMultiSel = m_lcAttributes.HasMultiSelection();
		CString sAttrib = m_lcAttributes.GetSelectedAttributeLabel();

		// Copy command
		CEnString sMenuText;

		if (GetParent()->SendMessage(WM_TDCM_CANCOPYTASKATTRIBUTE, nAttribID))
		{
			sMenuText.Format((bMultiSel ? IDS_ATTRIBCTRL_COPYATTRIBVALUES : IDS_ATTRIBCTRL_COPYATTRIBVALUE), sAttrib);

			CEnMenu::SetMenuString(*pPopup, ID_ATTRIBCTRL_COPYATTRIBVALUES, sMenuText, MF_BYCOMMAND);
			pPopup->EnableMenuItem(ID_ATTRIBCTRL_COPYATTRIBVALUES, MF_BYCOMMAND | MF_ENABLED);
		}
		else
		{
			pPopup->EnableMenuItem(ID_ATTRIBCTRL_COPYATTRIBVALUES, MF_BYCOMMAND | MF_DISABLED);
		}

		// Paste command
		TDC_ATTRIBUTE nFromAttribID = TDCA_NONE;

		if (GetParent()->SendMessage(WM_TDCM_CANPASTETASKATTRIBUTE, nAttribID, (LPARAM)&nFromAttribID))
		{
			CString sFromAttrib = m_lcAttributes.GetAttributeLabel(nFromAttribID);
			sMenuText.Format((bMultiSel ? IDS_ATTRIBCTRL_PASTEATTRIBVALUES : IDS_ATTRIBCTRL_PASTEATTRIBVALUE), sFromAttrib, sAttrib);

			CEnMenu::SetMenuString(*pPopup, ID_ATTRIBCTRL_PASTEATTRIBVALUES, sMenuText, MF_BYCOMMAND);
			pPopup->EnableMenuItem(ID_ATTRIBCTRL_PASTEATTRIBVALUES, MF_BYCOMMAND | MF_ENABLED);
		}
		else
		{
			pPopup->EnableMenuItem(ID_ATTRIBCTRL_PASTEATTRIBVALUES, MF_BYCOMMAND | MF_DISABLED);
		}

		// Clear command
		if (m_lcAttributes.CanEditSelectedAttribute())
		{
			sMenuText.Format((bMultiSel ? IDS_ATTRIBCTRL_CLEARATTRIBVALUES : IDS_ATTRIBCTRL_CLEARATTRIBVALUE), sAttrib);

			CEnMenu::SetMenuString(*pPopup, ID_ATTRIBCTRL_CLEARATTRIBVALUES, sMenuText, MF_BYCOMMAND);
			pPopup->EnableMenuItem(ID_ATTRIBCTRL_CLEARATTRIBVALUES, MF_BYCOMMAND | MF_ENABLED);
		}
		else
		{
			pPopup->EnableMenuItem(ID_ATTRIBCTRL_CLEARATTRIBVALUES, MF_BYCOMMAND | MF_DISABLED);
		}

		UINT nCmdID = ::TrackPopupMenu(*pPopup, TPM_RETURNCMD | TPM_LEFTALIGN | TPM_LEFTBUTTON,
									   pos.x, pos.y, 0, GetSafeHwnd(), NULL);

		switch (nCmdID)
		{
		case ID_ATTRIBCTRL_COPYATTRIBVALUES:
			GetParent()->SendMessage(WM_TDCM_COPYTASKATTRIBUTE, nAttribID);
			break;

		case ID_ATTRIBCTRL_PASTEATTRIBVALUES:
			GetParent()->SendMessage(WM_TDCM_PASTETASKATTRIBUTE, nAttribID);
			break;

		case ID_ATTRIBCTRL_CLEARATTRIBVALUES:
			GetParent()->SendMessage(WM_TDCM_CLEARTASKATTRIBUTE, nAttribID);
			break;
		}
	}
}

// -----------------------------------------------------------------------
// Pass-through function handlers

#define FWD_FUNC_VOID_0ARG(name) void CTDLTaskAttributeCtrl::name() { m_lcAttributes.name(); }

FWD_FUNC_VOID_0ARG(RefreshDateTimeFormatting)

// -----------------------------------------------------------------------

#define CONST_FWD_FUNC_RET_0ARG(ret, name) ret CTDLTaskAttributeCtrl::name() const { return m_lcAttributes.name(); }

CONST_FWD_FUNC_RET_0ARG(TDC_ATTRIBUTE, GetSelectedAttributeID)
CONST_FWD_FUNC_RET_0ARG(BOOL, GetFlag)
CONST_FWD_FUNC_RET_0ARG(BOOL, GetLock)
CONST_FWD_FUNC_RET_0ARG(CString, GetTaskTitle)
CONST_FWD_FUNC_RET_0ARG(CString, GetAllocBy)
CONST_FWD_FUNC_RET_0ARG(CString, GetStatus)
CONST_FWD_FUNC_RET_0ARG(CString, GetSelectedAttributeLabel)
CONST_FWD_FUNC_RET_0ARG(CString, GetExternalID)
CONST_FWD_FUNC_RET_0ARG(CString, GetVersion)
CONST_FWD_FUNC_RET_0ARG(int, GetPercentDone)
CONST_FWD_FUNC_RET_0ARG(int, GetPriority)
CONST_FWD_FUNC_RET_0ARG(int, GetRisk)
CONST_FWD_FUNC_RET_0ARG(COleDateTime, GetStartDate)
CONST_FWD_FUNC_RET_0ARG(COleDateTime, GetDueDate)
CONST_FWD_FUNC_RET_0ARG(COleDateTime, GetDoneDate)
CONST_FWD_FUNC_RET_0ARG(COleDateTime, GetStartTime)
CONST_FWD_FUNC_RET_0ARG(COleDateTime, GetDueTime)
CONST_FWD_FUNC_RET_0ARG(COleDateTime, GetDoneTime)

// -----------------------------------------------------------------------

#define FWD_FUNC_VOID_1ARG(name, argType) void CTDLTaskAttributeCtrl::name(argType arg) { m_lcAttributes.name(arg); }

FWD_FUNC_VOID_1ARG(RedrawValue,	TDC_ATTRIBUTE)
FWD_FUNC_VOID_1ARG(SelectValue,	TDC_ATTRIBUTE)
FWD_FUNC_VOID_1ARG(SetCurrentFolder, const CString&)
FWD_FUNC_VOID_1ARG(SetCompletionStatus, const CString&)
FWD_FUNC_VOID_1ARG(SetPriorityColors, const CDWordArray&)
FWD_FUNC_VOID_1ARG(SetNumPriorityRiskLevels, int)
FWD_FUNC_VOID_1ARG(SetPercentDoneIncrement,	int)
FWD_FUNC_VOID_1ARG(SetTimeTrackTaskID, DWORD)
FWD_FUNC_VOID_1ARG(SetDefaultAutoListData, const TDCAUTOLISTDATA&)
FWD_FUNC_VOID_1ARG(RefreshSelectedTasksValues, const CTDCAttributeMap&)
FWD_FUNC_VOID_1ARG(RefreshSelectedTasksValue, TDC_ATTRIBUTE)

// -----------------------------------------------------------------------

#define CONST_FWD_FUNC_RET_1ARG(ret, name, argType) ret CTDLTaskAttributeCtrl::name(argType arg) const { return m_lcAttributes.name(arg); }

CONST_FWD_FUNC_RET_1ARG(BOOL, GetTimeEstimate, TDCTIMEPERIOD&)
CONST_FWD_FUNC_RET_1ARG(BOOL, GetTimeSpent, TDCTIMEPERIOD&)
CONST_FWD_FUNC_RET_1ARG(int, GetDependencies, CTDCDependencyArray&)
CONST_FWD_FUNC_RET_1ARG(int, GetFileLinks, CStringArray&)
CONST_FWD_FUNC_RET_1ARG(BOOL, GetCost, TDCCOST&)

// -----------------------------------------------------------------------

#define FWD_FUNC_VOID_2ARG(name, arg1Type, arg2Type) void CTDLTaskAttributeCtrl::name(arg1Type arg1, arg2Type arg2) { m_lcAttributes.name(arg1, arg2); }

FWD_FUNC_VOID_2ARG(SetAutoListData, TDC_ATTRIBUTE, const TDCAUTOLISTDATA&)
FWD_FUNC_VOID_2ARG(SetAutoListDataReadOnly, TDC_ATTRIBUTE, BOOL)

// -----------------------------------------------------------------------

#define CONST_FWD_FUNC_RET_2ARG(ret, name, arg1Type, arg2Type) ret CTDLTaskAttributeCtrl::name(arg1Type arg1, arg2Type arg2) const { return m_lcAttributes.name(arg1, arg2); }

CONST_FWD_FUNC_RET_2ARG(int, GetAllocTo, CStringArray&, CStringArray&)
CONST_FWD_FUNC_RET_2ARG(int, GetCategories, CStringArray&, CStringArray&)
CONST_FWD_FUNC_RET_2ARG(int, GetTags, CStringArray&, CStringArray&)
CONST_FWD_FUNC_RET_2ARG(int, GetCustomAttributeAutoListData, const CString&, CStringArray&)

// -----------------------------------------------------------------------

// Others having unique patterns
void CTDLTaskAttributeCtrl::GetAutoListData(TDC_ATTRIBUTE nAttribID, TDCAUTOLISTDATA& tld) const
{
	m_lcAttributes.GetAutoListData(nAttribID, tld);
}

BOOL CTDLTaskAttributeCtrl::SetSelectedTaskIDs(const CDWordArray& aTaskIDs)
{
	return m_lcAttributes.SetSelectedTaskIDs(aTaskIDs);
}

BOOL CTDLTaskAttributeCtrl::GetCustomAttributeData(const CString& sAttribID, TDCCADATA& data, BOOL bFormatted) const
{
	return m_lcAttributes.GetCustomAttributeData(sAttribID, data, bFormatted);
}

////////////////////////////////////////////////////////////////////////////////////////////////
