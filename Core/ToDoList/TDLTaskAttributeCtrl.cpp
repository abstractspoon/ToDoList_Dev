// TDLTaskAttributeListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLTaskAttributeCtrl.h"
#include "tdcstruct.h"

#include "..\shared\Localizer.h"

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
													 const CTDCCustomAttribDefinitionArray& aCustAttribDefs)
	:
	m_lcAttributes(data, mgrContent, ilIcons, vis, aCustAttribDefs)
{
}

CTDLTaskAttributeCtrl::~CTDLTaskAttributeCtrl()
{
}

BEGIN_MESSAGE_MAP(CTDLTaskAttributeCtrl, CWnd)
	//{{AFX_MSG_MAP(CTDLTaskAttributeDlgCtrl)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()

	ON_COMMAND(ID_CATEGORIZE_ATTRIB, OnCategorizeAttributes)
	ON_COMMAND(ID_TOGGLE_SORT, OnToggleSorting)

	// These we just forward to our parent
	ON_REGISTERED_MESSAGE(WM_TDCM_EDITTASKATTRIBUTE, OnEditTaskAttribute)
	ON_REGISTERED_MESSAGE(WM_TDCN_ATTRIBUTEEDITED, OnAttributeEdited)
	ON_REGISTERED_MESSAGE(WM_TDCN_AUTOITEMADDEDDELETED, OnAutoItemAddedOrDeleted)
	ON_REGISTERED_MESSAGE(WM_TDCM_CLEARTASKATTRIBUTE, OnClearTaskAttribute)
	ON_REGISTERED_MESSAGE(WM_TDCM_TOGGLETIMETRACKING, OnToggleTimeTracking)
	ON_REGISTERED_MESSAGE(WM_TDCM_ADDTIMETOLOGFILE, OnAddTimeToLogFile)
	ON_REGISTERED_MESSAGE(WM_TDCM_SELECTDEPENDENCIES, OnSelectDependencies)
	ON_REGISTERED_MESSAGE(WM_TDCM_GETLINKTOOLTIP, OnGetLinkTooltip)
	ON_REGISTERED_MESSAGE(WM_TDCM_DISPLAYLINK, OnDisplayLink)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// -----------------------------------------------------------------------

#define FWD_MSG(msg, name) LRESULT CTDLTaskAttributeCtrl::name(WPARAM wp, LPARAM lp) { return GetParent()->SendMessage(msg, wp, lp); }

// ---------------------------------------------------

FWD_MSG(WM_TDCM_EDITTASKATTRIBUTE, OnEditTaskAttribute)
FWD_MSG(WM_TDCN_ATTRIBUTEEDITED, OnAttributeEdited)
FWD_MSG(WM_TDCN_AUTOITEMADDEDDELETED, OnAutoItemAddedOrDeleted)
FWD_MSG(WM_TDCM_CLEARTASKATTRIBUTE, OnClearTaskAttribute)
FWD_MSG(WM_TDCM_TOGGLETIMETRACKING, OnToggleTimeTracking)
FWD_MSG(WM_TDCM_ADDTIMETOLOGFILE, OnAddTimeToLogFile)
FWD_MSG(WM_TDCM_SELECTDEPENDENCIES, OnSelectDependencies)
FWD_MSG(WM_TDCM_GETLINKTOOLTIP, OnGetLinkTooltip)
FWD_MSG(WM_TDCM_DISPLAYLINK, OnDisplayLink)

// ---------------------------------------------------

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
	if (!m_toolbar.Create(this) || !m_toolbar.LoadToolBar(IDR_TASK_ATTRIBUTE_TOOLBAR, IDB_TASKATTRIB_TOOLBAR, colorMagenta))
		return -1;

	if (!m_tbHelper.Initialize(&m_toolbar))
		return -1;

	// Create List
	VERIFY(m_lcAttributes.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | LVS_NOCOLUMNHEADER | LVS_SHOWSELALWAYS, CRect(0, 0, 0, 0), this, IDC_TASKATTRIBUTES));
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

void CTDLTaskAttributeCtrl::OnCategorizeAttributes()
{
	int breakpoint = 0;
}

void CTDLTaskAttributeCtrl::OnToggleSorting()
{
	int breakpoint = 0;
}

// Call/message forwarding functions
// -----------------------------------------------------------------------

#define FWD_FUNC_VOID_0ARG(name) void CTDLTaskAttributeCtrl::name() { m_lcAttributes.name(); }

// -----------------------------------------------------------------------

FWD_FUNC_VOID_0ARG(RefreshDateTimeFormatting)
FWD_FUNC_VOID_0ARG(RefreshSelectedTasksValues)

// -----------------------------------------------------------------------

#define CONST_FWD_FUNC_RET_0ARG(ret, name) ret CTDLTaskAttributeCtrl::name() const { return m_lcAttributes.name(); }

// -----------------------------------------------------------------------

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

// -----------------------------------------------------------------------

FWD_FUNC_VOID_1ARG(RedrawValue,	TDC_ATTRIBUTE)
FWD_FUNC_VOID_1ARG(SelectValue,	TDC_ATTRIBUTE)
FWD_FUNC_VOID_1ARG(SetCompletionStatus,	const CString&)
FWD_FUNC_VOID_1ARG(SetPriorityColors, const CDWordArray&)
FWD_FUNC_VOID_1ARG(SetPercentDoneIncrement,	int)
FWD_FUNC_VOID_1ARG(SetDefaultAutoListData, const TDCAUTOLISTDATA&)
FWD_FUNC_VOID_1ARG(RefreshSelectedTasksValues, const CTDCAttributeMap&)
FWD_FUNC_VOID_1ARG(RefreshSelectedTasksValue, TDC_ATTRIBUTE)

// -----------------------------------------------------------------------

#define CONST_FWD_FUNC_RET_1ARG(ret, name, argType) ret CTDLTaskAttributeCtrl::name(argType arg) const { return m_lcAttributes.name(arg); }

// -----------------------------------------------------------------------

CONST_FWD_FUNC_RET_1ARG(BOOL, GetTimeEstimate, TDCTIMEPERIOD&)
CONST_FWD_FUNC_RET_1ARG(BOOL, GetTimeSpent, TDCTIMEPERIOD&)
CONST_FWD_FUNC_RET_1ARG(int, GetDependencies, CTDCDependencyArray&)
CONST_FWD_FUNC_RET_1ARG(int, GetFileLinks, CStringArray&)
CONST_FWD_FUNC_RET_1ARG(BOOL, GetCost, TDCCOST&)

// -----------------------------------------------------------------------

#define FWD_FUNC_VOID_2ARG(name, arg1Type, arg2Type) void CTDLTaskAttributeCtrl::name(arg1Type arg1, arg2Type arg2) { m_lcAttributes.name(arg1, arg2); }

// -----------------------------------------------------------------------

FWD_FUNC_VOID_2ARG(LoadState, const CPreferences&, LPCTSTR)
FWD_FUNC_VOID_2ARG(SetAutoListData, TDC_ATTRIBUTE, const TDCAUTOLISTDATA&)
FWD_FUNC_VOID_2ARG(SetAutoListDataReadOnly, TDC_ATTRIBUTE, BOOL)

// -----------------------------------------------------------------------

#define CONST_FWD_FUNC_RET_2ARG(ret, name, arg1Type, arg2Type) ret CTDLTaskAttributeCtrl::name(arg1Type arg1, arg2Type arg2) const { return m_lcAttributes.name(arg1, arg2); }

// -----------------------------------------------------------------------

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

void CTDLTaskAttributeCtrl::SaveState(CPreferences& prefs, LPCTSTR szKey) const
{
	m_lcAttributes.SaveState(prefs, szKey);
}

BOOL CTDLTaskAttributeCtrl::GetCustomAttributeData(const CString& sAttribID, TDCCADATA& data, BOOL bFormatted) const
{
	return m_lcAttributes.GetCustomAttributeData(sAttribID, data, bFormatted);
}

////////////////////////////////////////////////////////////////////////////////////////////////
