// TDLFilterComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "TDLFilterComboBox.h"
#include "tdcstatic.h"

#include "..\shared\enstring.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\misc.h"
#include "..\shared\holdredraw.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\localizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const int ITEM_HEADING = -100;

/////////////////////////////////////////////////////////////////////////////
// CTDLFilterComboBox

CTDLFilterComboBox::CTDLFilterComboBox() 
	: 
	CTabbedComboBox(20), 
	m_bShowDefaultFilters(TRUE)
{
	SetItemIndentBelowHeadings(0);
}

CTDLFilterComboBox::~CTDLFilterComboBox()
{
}


BEGIN_MESSAGE_MAP(CTDLFilterComboBox, CTabbedComboBox)
	//{{AFX_MSG_MAP(CTDLFilterComboBox)
// 	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLFilterComboBox message handlers

// void CTDLFilterComboBox::PreSubclassWindow() 
// {
// 	CTabbedComboBox::PreSubclassWindow();
// 
// 	FillCombo();
// }
// 
// int CTDLFilterComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
// {
// 	if (CTabbedComboBox::OnCreate(lpCreateStruct) == -1)
// 		return -1;
// 	
// 	FillCombo();
// 	
// 	return 0;
// }

int CTDLFilterComboBox::AddDefaultFilterItem(int nItem)
{
	ASSERT(nItem >= 0 && nItem < NUM_SHOWFILTER);
	
	CEnString sFilter(SHOW_FILTERS[nItem][0]);
	TCHAR cLetter = (TCHAR)('A' + nItem);
	
	CString sItem;

	if (cLetter > 'Z')
		sItem.Format(_T("\t%s"), sFilter); 
	else
		sItem.Format(_T("%c)\t%s"), cLetter, sFilter); 

	UINT nFilter = SHOW_FILTERS[nItem][1];
	
	return CDialogHelper::AddStringT(*this, sItem, nFilter);
}

void CTDLFilterComboBox::BuildCombo()
{
	ASSERT(GetSafeHwnd());
	ASSERT(GetCount() == 0);

	if (m_bShowDefaultFilters)
	{
		if (m_aAdvancedFilterNames.GetSize())
		{
			int nHeading = CDialogHelper::AddStringT(*this, CEnString(IDS_FILTERPLACEHOLDER), ITEM_HEADING);
			SetHeadingItem(nHeading);
		}

		for (int nItem = 0; nItem < NUM_SHOWFILTER; nItem++)
			AddDefaultFilterItem(nItem);
	}
	else // always show 'All Tasks' filter
	{
		AddDefaultFilterItem(0);
	}
	
	// Advanced filters
	if (m_aAdvancedFilterNames.GetSize())
	{
		if (m_bShowDefaultFilters)
		{
			int nHeading = CDialogHelper::AddStringT(*this, CEnString(IDS_ADVANCEDFILTERPLACEHOLDER), ITEM_HEADING);
			SetHeadingItem(nHeading);
		}

		for (int nItem = 0; nItem < m_aAdvancedFilterNames.GetSize(); nItem++)
		{
			CString sFilter = FormatAdvancedFilterDisplayString(nItem, m_aAdvancedFilterNames[nItem]);
			CDialogHelper::AddStringT(*this, sFilter, (DWORD)FS_ADVANCED);
		}
	}

	// resize to fit widest item
	CDialogHelper::RefreshMaxDropWidth(*this, NULL, TABSTOPS);

	CLocalizer::EnableTranslation(*this, FALSE);
}

void CTDLFilterComboBox::RebuildCombo(LPCTSTR szAdvancedSel)
{
	if (GetSafeHwnd())
	{
		CHoldRedraw hr(*this);
		
		// save selection
		CString sAdvFilter;
		FILTER_SHOW nSelFilter = GetSelectedFilter(sAdvFilter);
		
		CTabbedComboBox::RebuildCombo();

		// restore selection
		if (!SelectFilter(nSelFilter, (szAdvancedSel ? szAdvancedSel : sAdvFilter)))
		{
			SelectFilter(FS_ALL);

			// notify parent of selection change
			GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetSafeHwnd());
		}
	}
}

FILTER_SHOW CTDLFilterComboBox::GetSelectedFilter() const
{
	return CDialogHelper::GetSelectedItemDataT(*this, FS_ALL);
}

FILTER_SHOW CTDLFilterComboBox::GetSelectedFilter(CString& sAdvFilter) const
{
	FILTER_SHOW nSelFilter = GetSelectedFilter();

	if (nSelFilter == FS_ADVANCED)
	{
		CString sDisplay = CDialogHelper::GetSelectedItem(*this);
		VERIFY(ExtractAdvancedFilterName(sDisplay, sAdvFilter));
	}
	else
	{
		sAdvFilter.Empty();
	}

	return nSelFilter;
}

BOOL CTDLFilterComboBox::HasAdvancedFilter(const CString& sAdvFilter) const
{
	return (Misc::Find(sAdvFilter, m_aAdvancedFilterNames, FALSE, TRUE) != -1);
}

BOOL CTDLFilterComboBox::SelectFilter(FILTER_SHOW nFilter, LPCTSTR szAdvFilter)
{
	ASSERT((nFilter != FS_ADVANCED) || !Misc::IsEmpty(szAdvFilter));

	if (nFilter != FS_ADVANCED)
	{
		// This can fail if the default filters have been omitted
		// so we don't want to change the selection in such cases
		int nItem = CDialogHelper::FindItemByDataT(*this, (DWORD)nFilter);

		if (nItem == CB_ERR)
			return FALSE;

		SetCurSel(nItem);
		return TRUE;
	}

	// else lookup advanced filter by name
	int nAdvFilter = Misc::Find(szAdvFilter, m_aAdvancedFilterNames, TRUE, TRUE);

	if (nAdvFilter == -1)
		return FALSE;

#ifdef _DEBUG
	if (m_nNumHeadings == 0)
	{
		ASSERT(!m_bShowDefaultFilters || !m_aAdvancedFilterNames.GetSize());
	}
	else
	{
		ASSERT(GetDefaultFilterCount() == NUM_SHOWFILTER);
		ASSERT(m_nNumHeadings == 2);
		ASSERT(m_bShowDefaultFilters);
		ASSERT(m_aAdvancedFilterNames.GetSize());
	}
#endif

	int nIndex = (GetDefaultFilterCount() + nAdvFilter + m_nNumHeadings);

	ASSERT(GetItemData(nIndex) == FS_ADVANCED);

	return (SetCurSel(nIndex) != CB_ERR);
}

void CTDLFilterComboBox::SetAdvancedFilters(const CStringArray& aFilters, LPCTSTR szAdvancedSel)
{
	m_aAdvancedFilterNames.Copy(aFilters);

	if (GetSafeHwnd())
		RebuildCombo(szAdvancedSel);
}

const CStringArray& CTDLFilterComboBox::AdvancedFilterNames() const
{
	return m_aAdvancedFilterNames;
}

void CTDLFilterComboBox::ShowDefaultFilters(BOOL bShow)
{
	if (m_bShowDefaultFilters == bShow)
		return;

	m_bShowDefaultFilters = bShow;

	if (GetSafeHwnd())
		RebuildCombo();
}

int CTDLFilterComboBox::GetDefaultFilterCount() const
{
	return (m_bShowDefaultFilters ? NUM_SHOWFILTER : 1);
}

CString CTDLFilterComboBox::FormatAdvancedFilterDisplayString(int nFilter, const CString& sFilter)
{
	// if it starts with a tab then it's already done
	if (!sFilter.IsEmpty() && sFilter[0] == '\t')
		return sFilter;

	CString sDisplay, sNumeral(Misc::Format(nFilter));	

	if (sFilter.IsEmpty())
	{
		sDisplay.Format(_T("%s)\t%s"), sNumeral, CEnString(IDS_UNNAMEDFILTER));
	}
	else // filter only
	{
		sDisplay.Format(_T("%s)\t%s"), sNumeral, sFilter);
	}

	return sDisplay;
}

BOOL CTDLFilterComboBox::ExtractAdvancedFilterName(const CString& sDisplay, CString& sFilter)
{
	// if it doesn't have a tab then it's already done
	int nTab = sDisplay.Find('\t');

	if (nTab == -1)
		sFilter = sDisplay;
	else
		sFilter = sDisplay.Mid(nTab + 1);

	Misc::Trim(sFilter);

	return !sFilter.IsEmpty();
}

