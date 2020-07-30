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
// CTDLFilterComboBox

CTDLFilterComboBox::CTDLFilterComboBox() : CTabbedComboBox(20), m_bShowDefaultFilters(TRUE)
{
}

CTDLFilterComboBox::~CTDLFilterComboBox()
{
}


BEGIN_MESSAGE_MAP(CTDLFilterComboBox, CTabbedComboBox)
	//{{AFX_MSG_MAP(CTDLFilterComboBox)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLFilterComboBox message handlers

void CTDLFilterComboBox::PreSubclassWindow() 
{
	CTabbedComboBox::PreSubclassWindow();

	FillCombo();
}

int CTDLFilterComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabbedComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	FillCombo();
	
	return 0;
}

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
	
	return CDialogHelper::AddString(*this, sItem, nFilter);
}

void CTDLFilterComboBox::FillCombo()
{
	ASSERT(GetSafeHwnd());
	
	if (GetCount())
		return; // already called

	CLocalizer::EnableTranslation(*this, FALSE);
	
	if (m_bShowDefaultFilters)
	{
		for (int nItem = 0; nItem < NUM_SHOWFILTER; nItem++)
			AddDefaultFilterItem(nItem);
	}
	else // always show 'All Tasks' filter
	{
		AddDefaultFilterItem(0);
	}
	
	// Advanced filters
	for (int nItem = 0; nItem < m_aAdvancedFilterNames.GetSize(); nItem++)
	{
		CString sFilter = FormatAdvancedFilterDisplayString(nItem, m_aAdvancedFilterNames[nItem]);	
		CDialogHelper::AddString(*this, sFilter, (DWORD)FS_ADVANCED);
	}

	// resize to fit widest item
	CDialogHelper::RefreshMaxDropWidth(*this, NULL, TABSTOPS);
}

void CTDLFilterComboBox::RefillCombo(LPCTSTR szAdvancedSel)
{
	if (GetSafeHwnd())
	{
		CHoldRedraw hr(*this);
		
		// save selection
		CString sAdvanced;
		FILTER_SHOW nSelFilter = GetSelectedFilter(sAdvanced);
		
		ResetContent();
		FillCombo();
		
		// restore selection
		RestoreSelection(nSelFilter, (szAdvancedSel ? szAdvancedSel : sAdvanced));
	}
}

FILTER_SHOW CTDLFilterComboBox::GetSelectedFilter() const
{
	return CDialogHelper::GetSelectedItemData(*this, FS_ALL);
}

FILTER_SHOW CTDLFilterComboBox::GetSelectedFilter(CString& sAdvanced) const
{
	FILTER_SHOW nSelFilter = GetSelectedFilter();

	if (nSelFilter == FS_ADVANCED)
	{
		CString sDisplay = CDialogHelper::GetSelectedItem(*this);
		VERIFY(ExtractAdvancedFilterName(sDisplay, sAdvanced));
	}
	else
	{
		sAdvanced.Empty();
	}

	return nSelFilter;
}

BOOL CTDLFilterComboBox::HasAdvancedFilter(const CString& sAdvanced) const
{
	return (Misc::Find(sAdvanced, m_aAdvancedFilterNames, FALSE, TRUE) != -1);
}

BOOL CTDLFilterComboBox::SelectFilter(FILTER_SHOW nFilter)
{
	return (CB_ERR != CDialogHelper::SelectItemByData(*this, (DWORD)nFilter));
}

BOOL CTDLFilterComboBox::SelectAdvancedFilter(const CString& sAdvFilter)
{
	for (int nItem = 0; nItem < GetCount(); nItem++)
	{
		if (FS_ADVANCED == GetItemData(nItem))
		{
			CString sFilter;
			GetLBText(nItem, sFilter);

			// exact test
			if (sFilter == sAdvFilter)
			{
				SetCurSel(nItem);
				return TRUE;
			}
			// partial test
			else if (sFilter.Find(sAdvFilter) != -1)
			{
				// then full test
				int nFilter = nItem - (m_bShowDefaultFilters ? NUM_SHOWFILTER : 0);
				CString sFull = FormatAdvancedFilterDisplayString(nFilter, sAdvFilter);	

				if (sFilter == sFull)
				{
					SetCurSel(nItem);
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

void CTDLFilterComboBox::AddAdvancedFilters(const CStringArray& aFilters, LPCTSTR szAdvancedSel)
{
	m_aAdvancedFilterNames.Copy(aFilters);

	if (GetSafeHwnd())
		RefillCombo(szAdvancedSel);
}

const CStringArray& CTDLFilterComboBox::GetAdvancedFilterNames() const
{
	return m_aAdvancedFilterNames;
}

void CTDLFilterComboBox::RemoveAdvancedFilters()
{
	m_aAdvancedFilterNames.RemoveAll();

	if (GetSafeHwnd())
		RefillCombo();
}

void CTDLFilterComboBox::ShowDefaultFilters(BOOL bShow)
{
	if (m_bShowDefaultFilters == bShow)
		return;

	m_bShowDefaultFilters = bShow;

	if (GetSafeHwnd())
		RefillCombo();
}

void CTDLFilterComboBox::RestoreSelection(FILTER_SHOW nFilter, LPCTSTR szAdvanced)
{
	if (nFilter == FS_ADVANCED)
	{
		if (SelectAdvancedFilter(szAdvanced))
			return;
	}
	else if (SelectFilter(nFilter))
	{
		return;
	}

	// else
	SetCurSel(0);

	// notify parent of selection change
	GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), CBN_SELCHANGE), (LPARAM)GetSafeHwnd());
}

CString CTDLFilterComboBox::FormatAdvancedFilterDisplayString(int nFilter, const CString& sFilter, BOOL bIncAdvancedLabel)
{
	// if it starts with a tab then it's already done
	if (!sFilter.IsEmpty() && sFilter[0] == '\t')
		return sFilter;

	CEnString sAdvanced(IDS_ADVANCEDFILTER);
	CString sDisplay, sNumeral(Misc::Format(nFilter));	

	if (sFilter.IsEmpty())
	{
		sDisplay.Format(_T("%s)\t%s"), sNumeral, sAdvanced);
	}
	else if (bIncAdvancedLabel)
	{
		sDisplay.Format(_T("%s)\t%s (%s)"), sNumeral, sFilter, sAdvanced);
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
	{
		sFilter = sDisplay;
		return TRUE;
	}

	sFilter.Empty();

	// check for Advanced filter string
	CEnString sAdvanced(IDS_ADVANCEDFILTER);
	int nEnd = sDisplay.Find(sAdvanced);
	
	if (nEnd == -1)
		return FALSE;

	nEnd = sDisplay.Left(nEnd).ReverseFind('(');

	if (nEnd == -1)
		return FALSE;

	// else
	sFilter = sDisplay.Mid(nTab + 1, nEnd - nTab - 1);
	Misc::Trim(sFilter);

	return !sFilter.IsEmpty();
}

void CTDLFilterComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
										DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
{
	CString sText(sItem);

	// if it's a Advanced filter and the non-list item
	// remove the (Find Tasks Filter) bit
	if ((FS_ADVANCED == dwItemData) && !bList)
	{
		CString sFilter;

		if (ExtractAdvancedFilterName(sItem, sFilter))
		{
			int nFilter = nItem;
		
			if (m_bShowDefaultFilters)
				nFilter -= NUM_SHOWFILTER;

			sText = FormatAdvancedFilterDisplayString(nFilter, sFilter, FALSE);
		}
	}

	// default drawing
	CTabbedComboBox::DrawItemText(dc, rect, nItem, nItemState, dwItemData, sText, bList, crText);
}
