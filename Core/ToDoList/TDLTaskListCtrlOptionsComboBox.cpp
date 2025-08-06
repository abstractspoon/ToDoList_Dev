// TDLTaskListOptionsComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "tdcenum.h"
#include "TDLTaskListCtrlOptionsComboBox.h"

#include "..\shared\dialoghelper.h"

#include "..\interfaces\IPreferences.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskListOptionsComboBox

CTDLTaskListCtrlOptionsComboBox::CTDLTaskListCtrlOptionsComboBox()
	:
	m_dwRemovedOptions(0)
{
}

CTDLTaskListCtrlOptionsComboBox::~CTDLTaskListCtrlOptionsComboBox()
{
}

BEGIN_MESSAGE_MAP(CTDLTaskListCtrlOptionsComboBox, CCheckComboBox)
	//{{AFX_MSG_MAP(CTDLTaskListOptionsComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
// 	ON_WM_CREATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskListOptionsComboBox message handlers

// int CTDLTaskListCtrlOptionsComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
// {
// 	if (CCheckComboBox::OnCreate(lpCreateStruct) == -1)
// 		return -1;
// 
// 	//BuildCombo();
// 
// 	return 0;
// }
// 
// void CTDLTaskListCtrlOptionsComboBox::PreSubclassWindow()
// {
// 	CCheckComboBox::PreSubclassWindow();
// 
// 	//BuildCombo();
// }

void CTDLTaskListCtrlOptionsComboBox::BuildCombo()
{
	// once only
	if (!GetCount())
	{
		AddOption(LVO_HIDEPARENTS,			IDS_LISTVIEW_HIDEPARENTS);
		AddOption(LVO_HIDECOLLAPSED,		IDS_LISTVIEW_HIDECOLLAPSED);
		AddOption(LVO_HIDENOGROUPVALUE,		IDS_LISTVIEW_HIDENOGROUPVALUE);
		AddOption(LVO_SORTGROUPSASCENDING,	IDS_LISTVIEW_SORTGROUPSASCENDING);
		AddOption(LVO_SORTNONEGROUPBELOW,	IDS_LISTVIEW_SORTNONEGROUPBELOW);

		EnableTooltip();
	}
}

void CTDLTaskListCtrlOptionsComboBox::AddOption(DWORD dwOption, UINT nOptionStrID)
{
	if (!Misc::HasFlag(m_dwRemovedOptions, dwOption))
	{
		CDialogHelper::AddStringT(*this, nOptionStrID, dwOption);
	}
}

void CTDLTaskListCtrlOptionsComboBox::RemoveOptions(DWORD dwOptions)
{
	if (dwOptions != m_dwRemovedOptions)
	{
		m_dwRemovedOptions = dwOptions;
		RebuildCombo();
// 		if (GetSafeHwnd())
// 			Repopulate();
	}
}

DWORD CTDLTaskListCtrlOptionsComboBox::LoadOptions(const IPreferences* pPrefs, LPCTSTR szKey)
{
	DWORD dwOptions = 0;

	// Backwards compatibility (misspelt option)
	BOOL bSortGroupsAscending = pPrefs->GetProfileInt(szKey, _T("ListViewSortGroupsAscending"), -1);

	if (bSortGroupsAscending == -1)
		bSortGroupsAscending = pPrefs->GetProfileInt(szKey, _T("ListViewSortGroupsAsecnding"), TRUE);

	Misc::SetFlag(dwOptions, LVO_SORTGROUPSASCENDING,	bSortGroupsAscending);
	Misc::SetFlag(dwOptions, LVO_HIDEPARENTS,			pPrefs->GetProfileInt(szKey, _T("ListViewHideParents"),			FALSE));
	Misc::SetFlag(dwOptions, LVO_HIDECOLLAPSED,			pPrefs->GetProfileInt(szKey, _T("ListViewHideCollapsed"),		FALSE));
	Misc::SetFlag(dwOptions, LVO_SORTNONEGROUPBELOW,	pPrefs->GetProfileInt(szKey, _T("ListViewSortNoneGroupBelow"),	FALSE));
	Misc::SetFlag(dwOptions, LVO_HIDENOGROUPVALUE,		pPrefs->GetProfileInt(szKey, _T("ListViewHideNoGroupValue"),	FALSE));

	return dwOptions;
}

void CTDLTaskListCtrlOptionsComboBox::SaveOptions(DWORD dwOptions, IPreferences* pPrefs, LPCTSTR szKey)
{
	pPrefs->WriteProfileInt(szKey, _T("ListViewHideParents"),			(dwOptions & LVO_HIDEPARENTS));
	pPrefs->WriteProfileInt(szKey, _T("ListViewHideCollapsed"),			(dwOptions & LVO_HIDECOLLAPSED));
	pPrefs->WriteProfileInt(szKey, _T("ListViewSortNoneGroupBelow"),	(dwOptions & LVO_SORTNONEGROUPBELOW));
	pPrefs->WriteProfileInt(szKey, _T("ListViewHideNoGroupValue"),		(dwOptions & LVO_HIDENOGROUPVALUE));
	pPrefs->WriteProfileInt(szKey, _T("ListViewSortGroupsAscending"),	(dwOptions & LVO_SORTGROUPSASCENDING));

	// Delete previously misspelt option
	pPrefs->DeleteProfileEntry(szKey, _T("ListViewSortGroupsAsecnding"));
}
