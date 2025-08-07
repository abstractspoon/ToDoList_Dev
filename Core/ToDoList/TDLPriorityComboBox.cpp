// TDLPriorityComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLPriorityComboBox.h"
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
// CTDLPriorityComboBox

CTDLPriorityComboBox::CTDLPriorityComboBox(BOOL bIncludeAny, BOOL bIncludeNone)
	: 
	m_bIncludeAny(bIncludeAny),
	m_bIncludeNone(bIncludeNone),
	m_nNumLevels(11)
{
	// 'Any' and NOT 'None' is unexpected though it will still work
	ASSERT(!bIncludeAny || bIncludeNone);
}

CTDLPriorityComboBox::~CTDLPriorityComboBox()
{
}

BEGIN_MESSAGE_MAP(CTDLPriorityComboBox, CColorComboBox)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLPriorityComboBox message handlers

int CTDLPriorityComboBox::IncrementPriority(int nAmount)
{
	ASSERT(GetCount());

	int nPrevPriority = GetCurSel();
	int nPriority = nPrevPriority + nAmount;
	nPriority = max(0, min(nPriority, m_nNumLevels));	

	if (nPriority != nPrevPriority)
		SetCurSel(nPriority);

	return GetSelectedPriority();
}

int CTDLPriorityComboBox::GetSelectedPriority() const
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

void CTDLPriorityComboBox::SetSelectedPriority(int nPriority) // -2 -> 10
{
	CheckBuildCombo();

	int nSel = CB_ERR;

	switch (nPriority)
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
		if ((nPriority >= 0) && (nPriority < m_nNumLevels))
		{
			if (m_bIncludeAny && m_bIncludeNone) // both
				nSel = (nPriority + 2);
			else
				nSel = (nPriority + 1); // one or other
		}
		break;
	}

	SetCurSel(nSel);
}

BOOL CTDLPriorityComboBox::SetColors(const CDWordArray& aColors)
{
	if (aColors.GetSize() < m_nNumLevels)
		return FALSE;

	if (!Misc::MatchAll(aColors, m_aColors, TRUE))
	{
		m_aColors.Copy(aColors);

		if (GetSafeHwnd())
		{
			CheckBuildCombo();

			// Update the colours in-place
			int nNumItems = GetCount();

			for (int nItem = (nNumItems - m_nNumLevels), nColor = 0; nItem < nNumItems; nItem++, nColor++)
				SetColor(nItem, aColors[nColor]);
		}
	}

	return TRUE;
}

void CTDLPriorityComboBox::BuildCombo()
{
	ASSERT(GetSafeHwnd());
	ASSERT(GetCount() == 0);

	ModifyStyle(CBS_SORT, 0); // Unsorted
	
	BOOL bHasColors = m_aColors.GetSize();

	// first item are 'Any' and  'None' which never have a colour
	if (m_bIncludeAny)
		AddColor(CLR_NONE, CEnString(IDS_TDC_ANY));
	
	if (m_bIncludeNone)
		AddColor(CLR_NONE, CEnString(IDS_TDC_NONE));

	UINT aStrResIDs[11];
	TDC::GetPriorityRiskLevelStringResourceIDs(m_nNumLevels, aStrResIDs);

	for (int nLevel = 0; nLevel < m_nNumLevels; nLevel++)
	{
		COLORREF color = bHasColors ? m_aColors[nLevel] : -1;
		int nPriority = nLevel;
		
		CString sPriority;
		sPriority.Format(_T("%d (%s)"), nPriority, CEnString(aStrResIDs[nLevel]));
		AddColor(color, sPriority);
	}
}

void CTDLPriorityComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState, 
										DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
{
	if (nItem == -1)
		return;

	// Draw <any> in window prompt color
	if (!(nItemState & ODS_SELECTED) && !bList && m_bIncludeAny && (nItem == 0))
		crText = CWndPrompt::GetTextColor();
	
	// Don't indent 'non-color' items
	if ((dwItemData == CLR_NONE) && !Misc::HasFlag(m_dwFlags, CCBS_DRAWNOCOLOR))
	{
		CAutoComboBox::DrawItemText(dc, rect, nItem, nItemState, dwItemData, sItem, bList, crText);
		return;
	}

	// all else
	CColorComboBox::DrawItemText(dc, rect, nItem, nItemState, dwItemData, sItem, bList, crText);
}	

void CTDLPriorityComboBox::DDX(CDataExchange* pDX, int& nPriority)
{
	if (pDX->m_bSaveAndValidate)
		nPriority = GetSelectedPriority();
	else
		SetSelectedPriority(nPriority);
}

void CTDLPriorityComboBox::SetNumLevels(int nNumLevels)
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

