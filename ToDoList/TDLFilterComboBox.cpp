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

CTDLFilterComboBox::CTDLFilterComboBox() : m_bShowDefaultFilters(TRUE)
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
	
	// custom filters
	for (int nItem = 0; nItem < m_aCustomFilters.GetSize(); nItem++)
	{
		CString sFilter = FormatCustomFilterDisplayString(nItem, m_aCustomFilters[nItem]);	
		CDialogHelper::AddString(*this, sFilter, (DWORD)FS_CUSTOM);
	}

	// resize to fit widest item
	CDialogHelper::RefreshMaxDropWidth(*this, NULL, TABSTOPS);
}

void CTDLFilterComboBox::RefillCombo(LPCTSTR szCustomSel)
{
	if (GetSafeHwnd())
	{
		CHoldRedraw hr(*this);
		
		// save selection
		CString sCustom;
		FILTER_SHOW nSelFilter = GetSelectedFilter(sCustom);
		
		ResetContent();
		FillCombo();
		
		// restore selection
		RestoreSelection(nSelFilter, (szCustomSel ? szCustomSel : sCustom));
	}
}

FILTER_SHOW CTDLFilterComboBox::GetSelectedFilter() const
{
	return (FILTER_SHOW)CDialogHelper::GetSelectedItemData(*this);
}

FILTER_SHOW CTDLFilterComboBox::GetSelectedFilter(CString& sCustom) const
{
	FILTER_SHOW nSelFilter = GetSelectedFilter();

	if (nSelFilter == FS_CUSTOM)
	{
		CString sFilter = CDialogHelper::GetSelectedItem(*this);
		sCustom = ExtractCustomFilterName(sFilter);
	}
	else
		sCustom.Empty();

	return nSelFilter;
}

BOOL CTDLFilterComboBox::SelectFilter(FILTER_SHOW nFilter)
{
	return CDialogHelper::SelectItemByData(*this, (DWORD)nFilter);
}

BOOL CTDLFilterComboBox::SelectFilter(const CString& sCustomFilter)
{
	for (int nItem = 0; nItem < GetCount(); nItem++)
	{
		if (FS_CUSTOM == GetItemData(nItem))
		{
			CString sFilter;
			GetLBText(nItem, sFilter);

			// exact test
			if (sFilter == sCustomFilter)
			{
				SetCurSel(nItem);
				return TRUE;
			}
			// partial test
			else if (sFilter.Find(sCustomFilter) != -1)
			{
				// then full test
				int nFilter = nItem - (m_bShowDefaultFilters ? NUM_SHOWFILTER : 0);
				CString sFull = FormatCustomFilterDisplayString(nFilter, sCustomFilter);	

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

void CTDLFilterComboBox::AddCustomFilters(const CStringArray& aFilters, LPCTSTR szCustomSel)
{
	m_aCustomFilters.Copy(aFilters);

	if (GetSafeHwnd())
		RefillCombo(szCustomSel);
}

int CTDLFilterComboBox::GetCustomFilters(CStringArray& aFilters) const
{
	aFilters.Copy(m_aCustomFilters);
	return aFilters.GetSize();
}

void CTDLFilterComboBox::RemoveCustomFilters()
{
	m_aCustomFilters.RemoveAll();

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

void CTDLFilterComboBox::RestoreSelection(FILTER_SHOW nFilter, LPCTSTR szCustom)
{
	if (nFilter == FS_CUSTOM)
	{
		if (SelectFilter(szCustom))
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

CString CTDLFilterComboBox::FormatCustomFilterDisplayString(int nFilter, const CString& sFilter, BOOL bIncCustomLabel)
{
	// if it starts with a tab then it's already done
	if (!sFilter.IsEmpty() && sFilter[0] == '\t')
		return sFilter;

	CEnString sCustom(IDS_CUSTOMFILTER);
	CString sDisplay, sNumeral(Misc::Format(nFilter));	

	if (sFilter.IsEmpty())
	{
		sDisplay.Format(_T("%s)\t%s"), sNumeral, sCustom);
	}
	else if (bIncCustomLabel)
	{
		sDisplay.Format(_T("%s)\t%s (%s)"), sNumeral, sFilter, sCustom);
	}
	else // filter only
	{
		sDisplay.Format(_T("%s)\t%s"), sNumeral, sFilter);
	}

	return sDisplay;
}

CString CTDLFilterComboBox::ExtractCustomFilterName(const CString& sDisplay)
{
	// if it doesn't have a tab then it's already done
	int nTab = sDisplay.Find('\t');

	if (nTab == -1)
		return sDisplay;

	// check for custom filter string
	CEnString sCustom(IDS_CUSTOMFILTER);

	ASSERT(sDisplay.Find(sCustom) != -1);

	int nEnd = sDisplay.Find(_T(" ("));

	if (nEnd == -1)
		return _T("");

	// else
	sCustom = sDisplay.Mid(nTab + 1, nEnd - nTab - 1);
	return sCustom;
}

void CTDLFilterComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
										DWORD dwItemData, const CString& sItem, BOOL bList)
{
	CString sText(sItem);

	// if it's a custom filter and the non-list item
	// remove the (Find Tasks Filter) bit
	if ((FS_CUSTOM == dwItemData) && !bList)
	{
		CString sFilter = ExtractCustomFilterName(sItem);

		if (!sFilter.IsEmpty())
		{
			int nFilter = nItem;
		
			if (m_bShowDefaultFilters)
				nFilter -= NUM_SHOWFILTER;

			sText = FormatCustomFilterDisplayString(nFilter, sFilter, FALSE);
		}
	}

	// default drawing
	CTabbedComboBox::DrawItemText(dc, rect, nItem, nItemState, dwItemData, sText, bList);
}
