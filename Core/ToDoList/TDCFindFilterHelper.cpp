// ToDoCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TDCFindFilterHelper.h"
#include "TDLFindTasksDlg.h"
#include "TDLFilterBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

CTDCFindFilterHelper::CTDCFindFilterHelper(CTDLFindTasksDlg& findDlg, CTDLFilterBar& filterBar)
	:
	m_filterBar(filterBar),
	m_findDlg(findDlg)
{
}

CTDCFindFilterHelper::~CTDCFindFilterHelper()
{
}


void CTDCFindFilterHelper::RefreshFilterBarAdvancedFilters()
{
	CStringArray aFilters;
	m_findDlg.GetSavedSearches(aFilters);

	// check/add unnamed filter
	if (m_findDlg.GetSafeHwnd())
	{
		CEnString sUnNamed(IDS_UNNAMEDFILTER);

		if (m_findDlg.GetActiveSearch().IsEmpty() && !Misc::Contains(sUnNamed, aFilters, FALSE, TRUE))
			aFilters.Add(sUnNamed);
	}

	m_filterBar.AddAdvancedFilters(aFilters);

	int nFilter = aFilters.GetSize();

	while (nFilter--)
		UpdateFilterBarAdvancedFilter(aFilters[nFilter]);
}

BOOL CTDCFindFilterHelper::UpdateFilterBarAdvancedFilter(LPCTSTR szFilter)
{
	BOOL bIncDone = m_findDlg.GetSearchIncludesCompletedTasks(szFilter);

	return m_filterBar.SetAdvancedFilterIncludesDoneTasks(szFilter, bIncDone);
}


BOOL CTDCFindFilterHelper::UpdateFindDlgAdvancedFilter(LPCTSTR szFilter, DWORD dwFilterFlags)
{
	BOOL bIncDone = !Misc::HasFlag(dwFilterFlags, FO_HIDEDONE);

	return m_findDlg.SetSearchIncludesCompletedTasks(szFilter, bIncDone);
}
