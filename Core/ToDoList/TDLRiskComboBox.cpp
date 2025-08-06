// TDLRiskComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "TDLRiskComboBox.h"
#include "resource.h"
#include "tdcenum.h"
#include "tdcstatic.h"

#include "..\shared\holdredraw.h"
#include "..\shared\EnString.h"
#include "..\shared\Wndprompt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLRiskComboBox

CTDLRiskComboBox::CTDLRiskComboBox(BOOL bIncludeAny, BOOL bIncludeNone)
	: 
	m_bIncludeAny(bIncludeAny),
	m_bIncludeNone(bIncludeNone),
	m_nNumLevels(11)
{
	// 'Any' and NOT 'None' is unexpected though it will still work
	ASSERT(!bIncludeAny || bIncludeNone);
}

CTDLRiskComboBox::~CTDLRiskComboBox()
{
}


BEGIN_MESSAGE_MAP(CTDLRiskComboBox, COwnerdrawComboBoxBase)
	//{{AFX_MSG_MAP(CTDLRiskComboBox)
// 	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLRiskComboBox message handlers

// int CTDLRiskComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
// {
// 	if (COwnerdrawComboBoxBase::OnCreate(lpCreateStruct) == -1)
// 		return -1;
// 	
// 	BuildCombo();
// 	
// 	return 0;
// }
// 
// void CTDLRiskComboBox::PreSubclassWindow() 
// {
// 	COwnerdrawComboBoxBase::PreSubclassWindow();
// 
//  	BuildCombo();
// }

int CTDLRiskComboBox::GetSelectedRisk() const
{
	int nSel = GetCurSel();

	if ((nSel == CB_ERR) || (!m_bIncludeAny && !m_bIncludeNone))
	{
		// index is priority
		return nSel;
	}

	// Both
	if (m_bIncludeAny && m_bIncludeNone)
	{
		switch (nSel)
		{
		case 0:		return FM_ANYPRIORITY;
		case 1:		return FM_NOPRIORITY;
		default:	return (nSel - 2);
		}
	}

	// Only 'Any'
	if (m_bIncludeAny)
	{
		switch (nSel)
		{
		case 0:		return FM_ANYPRIORITY;
		default:	return (nSel - 1);
		}
	}

	// Only 'None'
	switch (nSel)
	{
	case 0:		return FM_NOPRIORITY;
	default:	return (nSel - 1);
	}
}

void CTDLRiskComboBox::SetSelectedRisk(int nRisk) // -2 -> m_nNumLevels
{
	CheckBuildCombo();

	int nSel = CB_ERR;

	switch (nRisk)
	{
	case FM_ANYPRIORITY:
		if (m_bIncludeAny)
			nSel = 0;
		else
			ASSERT(0);
		break;

	case FM_NOPRIORITY:
		if (m_bIncludeNone)
			nSel = (m_bIncludeAny ? 1 : 0);
		else
			ASSERT(0);
		break;

	default:
		if ((nRisk >= 0) && (nRisk < m_nNumLevels))
		{
			if (m_bIncludeAny && m_bIncludeNone) // both
				nSel = (nRisk + 2);
			else
				nSel = (nRisk + 1); // one or other
		}
		break;
	}

	SetCurSel(nSel);
}

void CTDLRiskComboBox::BuildCombo()
{
	ASSERT(GetSafeHwnd());

	if (GetCount() == 0)
		return;

//	CHoldRedraw hr(*this);
	
	// first items are 'Any' and 'None'
	if (m_bIncludeAny)
		AddString(CEnString(IDS_TDC_ANY));
	
	AddString(CEnString(IDS_TDC_NONE));
	
	UINT aStrResIDs[11];
	TDC::GetPriorityRiskLevelStringResourceIDs(m_nNumLevels, aStrResIDs);

	for (int nLevel = 0; nLevel < m_nNumLevels; nLevel++)
	{
		CString sRisk;
		sRisk.Format(_T("%d (%s)"), nLevel, CEnString(aStrResIDs[nLevel]));
		AddString(sRisk);
	}
}

void CTDLRiskComboBox::DDX(CDataExchange* pDX, int& nRisk)
{
	if (pDX->m_bSaveAndValidate)
		nRisk = GetSelectedRisk();
	else
		SetSelectedRisk(nRisk);
}

void CTDLRiskComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
	DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
{
	// Draw <any> or <none> in window prompt color
	if (!(nItemState & ODS_SELECTED) && !bList && (nItem == 0))
	{
		crText = CWndPrompt::GetTextColor();
	}

	// all else
	COwnerdrawComboBoxBase::DrawItemText(dc, rect, nItem, nItemState, dwItemData, sItem, bList, crText);
}

void CTDLRiskComboBox::SetNumLevels(int nNumLevels)
{
	ASSERT(TDC::IsValidNumPriorityRiskLevels(nNumLevels));

	if (nNumLevels != m_nNumLevels)
	{
		m_nNumLevels = nNumLevels;

		if (GetSafeHwnd())
		{
			int nSel = GetCurSel(); // save
			RebuildCombo();

			SetCurSel(nSel); // restore
		}
	}
}

