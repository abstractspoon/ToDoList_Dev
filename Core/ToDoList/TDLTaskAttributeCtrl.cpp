// TDLTaskAttributeListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLTaskAttributeCtrl.h"
#include "tdcstruct.h"

#include "..\shared\Localizer.h"

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

BEGIN_MESSAGE_MAP(CTDLTaskAttributeCtrl, CRuntimeDlg)
	//{{AFX_MSG_MAP(CTDLTaskAttributeDlgCtrl)
	ON_WM_SIZE()

	ON_REGISTERED_MESSAGE(WM_FE_DISPLAYFILE, OnFileLinkDisplay)
	ON_REGISTERED_MESSAGE(WM_FE_GETFILETOOLTIP, OnFileLinkWantTooltip)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskAttributeDlgCtrl message handlers

// Message handlers
BOOL CTDLTaskAttributeCtrl::OnInitDialog()
{
	// Create toolbar
	// TODO

	// Create List
	VERIFY(m_lcAttributes.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | LVS_NOCOLUMNHEADER | LVS_SHOWSELALWAYS, CRect(0, 0, 0, 0), this, IDC_TASKATTRIBUTES));

	return CDialog::OnInitDialog();
}

void CTDLTaskAttributeCtrl::OnSetFocus(CWnd* pOldWnd)
{
	CRuntimeDlg::OnSetFocus(pOldWnd);

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
		m_lcAttributes.MoveWindow(0, 0, cx, cy);

	CRuntimeDlg::OnSize(nType, cx, cy);
}

LRESULT CTDLTaskAttributeCtrl::OnFileLinkWantTooltip(WPARAM wParam, LPARAM lParam)
{
	return GetParent()->SendMessage(WM_TDCM_GETLINKTOOLTIP, wParam, lParam);
}

LRESULT CTDLTaskAttributeCtrl::OnFileLinkDisplay(WPARAM wParam, LPARAM lParam)
{
	return GetParent()->SendMessage(WM_TDCM_DISPLAYLINK, wParam, lParam);
}

// Public interface
BOOL CTDLTaskAttributeCtrl::Create(CWnd* pParent, UINT nID, const CRect& rPos)
{
	SetBordersDLU(0);

	return CRuntimeDlg::Create(NULL, (WS_CHILD | WS_VISIBLE | WS_TABSTOP | DS_SETFONT),
							   WS_EX_CONTROLPARENT, rPos, pParent, nID);
}

void CTDLTaskAttributeCtrl::RedrawValue(TDC_ATTRIBUTE nAttribID)
{
	m_lcAttributes.RedrawValue(nAttribID);
}

void CTDLTaskAttributeCtrl::SelectValue(TDC_ATTRIBUTE nAttribID)
{
	m_lcAttributes.SelectValue(nAttribID);
}

void CTDLTaskAttributeCtrl::RefreshDateTimeFormatting()
{
	m_lcAttributes.RefreshDateTimeFormatting();
}

void CTDLTaskAttributeCtrl::SaveState(CPreferences& prefs, LPCTSTR szKey) const
{
	m_lcAttributes.SaveState(prefs, szKey);
}

void CTDLTaskAttributeCtrl::LoadState(const CPreferences& prefs, LPCTSTR szKey)
{
	m_lcAttributes.LoadState(prefs, szKey);
}

void CTDLTaskAttributeCtrl::SetCompletionStatus(const CString& sStatus)
{
	m_lcAttributes.SetCompletionStatus(sStatus);
}

void CTDLTaskAttributeCtrl::SetPriorityColors(const CDWordArray& aColors)
{
	m_lcAttributes.SetPriorityColors(aColors);
}

void CTDLTaskAttributeCtrl::SetPercentDoneIncrement(int nAmount)
{
	m_lcAttributes.SetPercentDoneIncrement(nAmount);
}

TDC_ATTRIBUTE CTDLTaskAttributeCtrl::GetSelectedAttributeID() const
{
	return m_lcAttributes.GetSelectedAttributeID();
}

CString CTDLTaskAttributeCtrl::GetSelectedAttributeLabel() const
{
	return m_lcAttributes.GetSelectedAttributeLabel();
}

void CTDLTaskAttributeCtrl::SetDefaultAutoListData(const TDCAUTOLISTDATA& tldDefault) 
{ 
	m_lcAttributes.SetDefaultAutoListData(tldDefault);
}

void CTDLTaskAttributeCtrl::SetAutoListData(TDC_ATTRIBUTE nAttribID, const TDCAUTOLISTDATA& tld)
{
	m_lcAttributes.SetAutoListData(nAttribID, tld);
}

void CTDLTaskAttributeCtrl::GetAutoListData(TDC_ATTRIBUTE nAttribID, TDCAUTOLISTDATA& tld) const
{
	m_lcAttributes.GetAutoListData(nAttribID, tld);
}

void CTDLTaskAttributeCtrl::SetAutoListDataReadOnly(TDC_ATTRIBUTE nAttribID, BOOL bReadOnly)
{
	m_lcAttributes.SetAutoListDataReadOnly(nAttribID, bReadOnly);
}

BOOL CTDLTaskAttributeCtrl::SetSelectedTaskIDs(const CDWordArray& aTaskIDs)
{
	return m_lcAttributes.SetSelectedTaskIDs(aTaskIDs);
}

void CTDLTaskAttributeCtrl::RefreshSelectedTasksValues()
{
	m_lcAttributes.RefreshSelectedTasksValues();
}

void CTDLTaskAttributeCtrl::RefreshSelectedTasksValues(const CTDCAttributeMap& mapAttribIDs)
{
	m_lcAttributes.RefreshSelectedTasksValues(mapAttribIDs);
}

void CTDLTaskAttributeCtrl::RefreshSelectedTasksValue(TDC_ATTRIBUTE nAttribID)
{
	m_lcAttributes.RefreshSelectedTasksValue(nAttribID);
}

BOOL CTDLTaskAttributeCtrl::GetTimeEstimate(TDCTIMEPERIOD& timeEst) const
{
	return m_lcAttributes.GetTimeEstimate(timeEst);
}

BOOL CTDLTaskAttributeCtrl::GetTimeSpent(TDCTIMEPERIOD& timeSpent) const
{
	return m_lcAttributes.GetTimeSpent(timeSpent);
}

int CTDLTaskAttributeCtrl::GetAllocTo(CStringArray& aMatched, CStringArray& aMixed) const
{
	return m_lcAttributes.GetAllocTo(aMatched, aMixed);
}

CString CTDLTaskAttributeCtrl::GetTaskTitle() const
{
	return m_lcAttributes.GetTaskTitle();
}

CString CTDLTaskAttributeCtrl::GetAllocBy() const
{
	return m_lcAttributes.GetAllocBy();
}

CString CTDLTaskAttributeCtrl::GetStatus() const
{
	return m_lcAttributes.GetStatus();
}

int CTDLTaskAttributeCtrl::GetCategories(CStringArray& aMatched, CStringArray& aMixed) const
{
	return m_lcAttributes.GetCategories(aMatched, aMixed);
}

int CTDLTaskAttributeCtrl::GetDependencies(CTDCDependencyArray& aDepends) const
{
	return m_lcAttributes.GetDependencies(aDepends);
}

int CTDLTaskAttributeCtrl::GetTags(CStringArray& aMatched, CStringArray& aMixed) const
{
	return m_lcAttributes.GetTags(aMatched, aMixed);
}

int CTDLTaskAttributeCtrl::GetFileLinks(CStringArray& aFiles) const
{
	return m_lcAttributes.GetFileLinks(aFiles);
}

CString CTDLTaskAttributeCtrl::GetExternalID() const
{
	return m_lcAttributes.GetExternalID();
}

int CTDLTaskAttributeCtrl::GetPercentDone() const
{
	return m_lcAttributes.GetPercentDone();
}

int CTDLTaskAttributeCtrl::GetPriority() const
{
	return m_lcAttributes.GetPriority();
}

int CTDLTaskAttributeCtrl::GetRisk() const
{
	return m_lcAttributes.GetRisk();
}

BOOL CTDLTaskAttributeCtrl::GetCost(TDCCOST& cost) const
{
	return m_lcAttributes.GetCost(cost);
}

BOOL CTDLTaskAttributeCtrl::GetFlag() const
{
	return m_lcAttributes.GetFlag();
}

BOOL CTDLTaskAttributeCtrl::GetLock() const
{
	return m_lcAttributes.GetLock();
}

CString CTDLTaskAttributeCtrl::GetVersion() const
{
	return m_lcAttributes.GetVersion();
}

COleDateTime CTDLTaskAttributeCtrl::GetStartDate() const
{
	return m_lcAttributes.GetStartDate();
}

COleDateTime CTDLTaskAttributeCtrl::GetDueDate() const
{
	return m_lcAttributes.GetDueDate();
}

COleDateTime CTDLTaskAttributeCtrl::GetDoneDate() const
{
	return m_lcAttributes.GetDoneDate();
}

COleDateTime CTDLTaskAttributeCtrl::GetStartTime() const
{
	return m_lcAttributes.GetStartTime();
}

COleDateTime CTDLTaskAttributeCtrl::GetDueTime() const
{
	return m_lcAttributes.GetDueTime();
}

COleDateTime CTDLTaskAttributeCtrl::GetDoneTime() const
{
	return m_lcAttributes.GetDoneTime();
}

int CTDLTaskAttributeCtrl::GetCustomAttributeAutoListData(const CString& sAttribID, CStringArray& aItems) const
{
	return m_lcAttributes.GetCustomAttributeAutoListData(sAttribID, aItems);
}

BOOL CTDLTaskAttributeCtrl::GetCustomAttributeData(const CString& sAttribID, TDCCADATA& data, BOOL bFormatted) const
{
	return m_lcAttributes.GetCustomAttributeData(sAttribID, data, bFormatted);
}

////////////////////////////////////////////////////////////////////////////////////////////////
