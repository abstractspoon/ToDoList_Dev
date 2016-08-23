// TDLFilterComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "TDLFilterDateComboBox.h"
#include "tdcstatic.h"

#include "..\shared\enstring.h"
#include "..\shared\localizer.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\autoflag.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLFilterDateComboBox

CTDLFilterDateComboBox::CTDLFilterDateComboBox(int nNextNDays) 
	: 
	m_nNextNDays(nNextNDays),
	m_bRebuildingCombo(FALSE)
{
}

CTDLFilterDateComboBox::~CTDLFilterDateComboBox()
{
}


BEGIN_MESSAGE_MAP(CTDLFilterDateComboBox, CTabbedComboBox)
	//{{AFX_MSG_MAP(CTDLFilterDateComboBox)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT_EX(CBN_SELCHANGE, OnReflectSelChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLFilterDateComboBox message handlers

void CTDLFilterDateComboBox::PreSubclassWindow() 
{
	CTabbedComboBox::PreSubclassWindow();

	FillCombo();
}

int CTDLFilterDateComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabbedComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	FillCombo();
	
	return 0;
}

void CTDLFilterDateComboBox::SetNextNDays(int nDays)
{
	if (m_nNextNDays != nDays)
	{
		m_nNextNDays = nDays;

		// Selectively replace '7' with 'N' without loss of selection
		CAutoFlag af(m_bRebuildingCombo, TRUE);
		
		FILTER_DATE nSelFilter = GetSelectedFilter();

		ResetContent();
		FillCombo();

		SelectFilter(nSelFilter);
	}
}

BOOL CTDLFilterDateComboBox::OnReflectSelChange()
{
	// prevent forwarding of selection changes
	// whilst rebuilding combo
	return m_bRebuildingCombo;
}

void CTDLFilterDateComboBox::FillCombo()
{
	ASSERT(GetSafeHwnd());

	if (GetCount())
		return; // already called

	CLocalizer::EnableTranslation(*this, FALSE);

	TCHAR nLetter = 'A';

	for (int nItem = 0; nItem < NUM_DATEFILTER; nItem++)
	{
		CEnString sFilter(DATE_FILTERS[nItem][0]);
		UINT nFilter = DATE_FILTERS[nItem][1];

		CString sText;

		switch (nFilter)
		{
		case FD_ANY:
		case FD_NONE:
		case FD_USER:
			sText = sFilter;
			break;

		case FD_NEXTNDAYS:
			if (m_nNextNDays != 7)
				sFilter.Replace(_T("7"), Misc::Format(m_nNextNDays));
			// fall thru

		default:
			sText.Format(_T("%c)\t%s"), nLetter, sFilter); 
			nLetter++;
			break;
		}

		VERIFY(CDialogHelper::AddString(*this, sText, nFilter) != CB_ERR);
	}

	CDialogHelper::RefreshMaxDropWidth(*this);
}

FILTER_DATE CTDLFilterDateComboBox::GetSelectedFilter() const
{
	int nSel = GetCurSel();

	if (nSel == CB_ERR)
		return FD_ANY;

	// else
	return (FILTER_DATE)GetItemData(nSel);
}

BOOL CTDLFilterDateComboBox::SelectFilter(FILTER_DATE nFilter)
{
	return CDialogHelper::SelectItemByData(*this, (DWORD)nFilter);
}
