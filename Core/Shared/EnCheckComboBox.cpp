// TDLCategoryComboBox.cpp : implementation file
//

#include "stdafx.h"

#include "enCheckComboBox.h"
#include "misc.h"
#include "enstring.h"
#include "holdredraw.h"
#include "WndPrompt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnCheckComboBox

CEnCheckComboBox::CEnCheckComboBox(BOOL bMulti, UINT nIDNoneString, UINT nIDAnyString) : 
	CCheckComboBox(ACBS_ALLOWDELETE), m_bMultiSel(bMulti)
{
	m_sNone.LoadString(nIDNoneString);
	m_sAny.LoadString(nIDAnyString);
}

CEnCheckComboBox::~CEnCheckComboBox()
{
}

IMPLEMENT_DYNAMIC(CEnCheckComboBox, CCheckComboBox)

BEGIN_MESSAGE_MAP(CEnCheckComboBox, CCheckComboBox)
	//{{AFX_MSG_MAP(CEnCheckComboBox)
	//}}AFX_MSG_MAP
	ON_CONTROL(LBN_SELCHANGE, 1000, OnLBSelChange)
	ON_CONTROL_REFLECT_EX(CBN_SELENDOK, OnSelEndOK)
	ON_MESSAGE(WM_GETTEXTLENGTH, OnGetTextLen)
	ON_WM_KEYDOWN()

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnCheckComboBox message handlers

void CEnCheckComboBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// Bypass base-class opening of droplist if NOT in check-box mode
	if (!m_bMultiSel && (nChar == VK_DOWN) && !GetDroppedState())
	{
		CAutoComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
	}

	CCheckComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CEnCheckComboBox::EnableMultiSelection(BOOL bEnable)
{
	if (bEnable == m_bMultiSel)
		return TRUE;
	
	m_bMultiSel = bEnable;

	if (GetSafeHwnd())
	{
		FixupEmptyStringsAtStart();

		// if changing from multi selection and only one item was 
		// selected then set that as the single selection else clear all
		if (!m_bMultiSel)
		{
			CStringArray aItems;

			if (CCheckComboBox::GetChecked(aItems) == 1)
			{
				SelectString(0, aItems[0]);
				m_sText == aItems[0];
			}
			else
			{
				CheckAll(CCBC_UNCHECKED);
				m_sText.Empty();
			}
		}
		else 
		{
			// set the current selection to whatever was singly selected
			// provided it's not blank		
			CheckAll(CCBC_UNCHECKED);

			int nSel = GetCurSel();

			if (nSel != CB_ERR)
			{
				CString sItem = CCheckComboBox::GetItemText(nSel);

				if (!sItem.IsEmpty())
					SetCheck(nSel, CCBC_CHECKED);
			}
		}
		
		Invalidate();
		InitItemHeight();
	}
	
	return TRUE;
}

void CEnCheckComboBox::ClearMultiSelectionHistory()
{
	if (m_bMultiSel && IsItemAnyChecked())
		CheckAll(CCBC_UNCHECKED, FALSE);
}

int CEnCheckComboBox::SetStrings(const CStringArray& aItems)
{
	// Save and restore check states
	CStringArray aChecked, aMixed;

	if (m_bMultiSel)
		GetChecked(aChecked, aMixed);
	
	int nRes = CCheckComboBox::SetStrings(aItems);

	FixupEmptyStringsAtStart();

	if (m_bMultiSel)
		SetChecked(aChecked, aMixed);

	return nRes;
}

void CEnCheckComboBox::FixupEmptyStringsAtStart()
{
	CHoldRedraw hr(*this);

	while (GetCount() && CAutoComboBox::GetItemText(0).IsEmpty())
		CComboBox::DeleteString(0);
	
	int nEmpty = CalcNumRequiredEmptyStrings();

	while (nEmpty--)
		int nItem = CComboBox::InsertString(0, _T("")); // bypass checks for uniqueness
}

int CEnCheckComboBox::CalcNumRequiredEmptyStrings() const
{
	int nNumReqd = 0;

	nNumReqd += (m_sAny.IsEmpty() ? 0 : 1);
	nNumReqd += (m_sNone.IsEmpty() ? 0 : 1);

	return nNumReqd;
}

int CEnCheckComboBox::GetItemNoneIndex() const
{
	if (m_sNone.IsEmpty())
		return -1;

	return (CalcNumRequiredEmptyStrings() - 1);
}

int CEnCheckComboBox::GetItemAnyIndex() const
{
	if (m_sAny.IsEmpty())
		return -1;

	return 0;
}

BOOL CEnCheckComboBox::IsItemNoneIndex(int nItem) const
{
	if (nItem == -1)
		return FALSE;

	return (nItem == GetItemNoneIndex());
}

BOOL CEnCheckComboBox::IsItemAnyIndex(int nItem) const
{
	if (nItem == -1)
		return FALSE;

	return (nItem == GetItemAnyIndex());
}

BOOL CEnCheckComboBox::IsItemAnyChecked() const
{
	int nAny = GetItemAnyIndex();

	return ((nAny == -1) ? FALSE : GetCheck(nAny));
}

BOOL CEnCheckComboBox::IsItemNoneChecked() const
{
	int nNone = GetItemNoneIndex();

	return ((nNone == -1) ? FALSE : GetCheck(nNone));
}

CString CEnCheckComboBox::FormatCheckedItems(LPCTSTR szSep) const
{
	if (IsItemAnyChecked())
		return m_sAny;

	CStringArray aChecked;
	
	if (!GetChecked(aChecked))
		return _T("");

	if (aChecked[0].IsEmpty())
	{
		ASSERT(IsItemNoneChecked());
		aChecked[0] = m_sNone;
	}

	return Misc::FormatArray(aChecked, szSep);
}

void CEnCheckComboBox::OnCheckChange(int nIndex)
{
	if (m_bMultiSel)
	{
		int nAny = GetItemAnyIndex();

		if (nIndex == nAny)
		{
			m_scList.Invalidate();
		}
		else if (GetCheck(nAny))
		{
			SetCheck(nAny, CCBC_UNCHECKED);
			m_scList.Invalidate();
		}
	}
}

BOOL CEnCheckComboBox::GetCheck(int nIndex) const
{
	if (m_bMultiSel)
		return (CCheckComboBox::GetCheck(nIndex) == CCBC_CHECKED);
	
	// else
	return ((nIndex != CB_ERR) && (CComboBox::GetCurSel() == nIndex));
}


int CEnCheckComboBox::GetChecked(CStringArray& aItems, CCB_CHECKSTATE nCheck) const
{
	if (m_bMultiSel)
	{
		if (IsItemAnyChecked())
		{
			if (nCheck == CCBC_UNCHECKED)
				GetItems(aItems);
			else
				aItems.RemoveAll();

			return aItems.GetSize();
		}

		// else
		return CCheckComboBox::GetChecked(aItems, nCheck);
	}
	
	// else
	aItems.RemoveAll();

	if (nCheck == CCBC_CHECKED)
	{
		int nSel = GetCurSel();

		if ((nSel != -1) && !IsItemAnyIndex(nSel))
			aItems.Add(CCheckComboBox::GetItemText(nSel));
	}
	
	return aItems.GetSize();
}

void CEnCheckComboBox::GetChecked(CStringArray& aChecked, CStringArray& aMixed) const
{
	CCheckComboBox::GetChecked(aChecked, CCBC_CHECKED);
	CCheckComboBox::GetChecked(aMixed, CCBC_MIXED);

	// Remove 'Any'
	if (m_bMultiSel && IsItemAnyChecked())
	{
		ASSERT(aChecked.GetSize());
		aChecked.RemoveAt(0);
	}
}

#ifdef _DEBUG
void CEnCheckComboBox::TraceCheckStates() const
{
	int nNumItems = GetCount();

	if (nNumItems)
	{
		TRACE(_T("CEnCheckComboBox::TraceCheckStates(begin)\n"));

		for (int nItem = 0; nItem < nNumItems; nItem++)
		{
			CString sItem = GetItemText(nItem);
			TRACE(_T("\t %s: "), sItem);

			switch (CCheckComboBox::GetCheck(nItem))
			{
			case CCBC_UNCHECKED:
				TRACE(_T("unchecked\n"));
				break;

			case CCBC_CHECKED:
				TRACE(_T("checked\n"));
				break;

			case CCBC_MIXED:
				TRACE(_T("mixed\n"));
				break;
			}
		}

		TRACE(_T("CEnCheckComboBox::TraceCheckStates(end)\n"));
	}
}
#endif

CString CEnCheckComboBox::GetItemText(int nItem, const CString& sHint) const
{
	CString sItem(sHint);

	if (sHint.IsEmpty())
	{
		if (nItem != -1)
			sItem = CCheckComboBox::GetItemText(nItem);
		else
			sItem = m_sText;
	}

	if (m_bMultiSel)
	{
		if (sItem.IsEmpty()) 
		{
			if (nItem == -1)
			{
				sItem = m_sAny;
			}
			else if (IsItemAnyIndex(nItem))
			{
				sItem = m_sAny;
			}
			else if (IsItemNoneIndex(nItem))
			{
				sItem = m_sNone;
			}
			else
			{
				// Text shouldn't be empty
				ASSERT(0);
			}
		}
	}
	else // single sel
	{
		if (nItem == -1)
			nItem = GetCurSel();

		if (IsItemAnyIndex(nItem))
		{
			sItem = m_sAny;
		}
		else if (IsItemNoneIndex(nItem))
		{
			sItem = m_sNone;
		}
		else if (sItem.IsEmpty())
		{
			sItem = m_sAny;
		}
	}

	return sItem;
}

BOOL CEnCheckComboBox::SetChecked(const CStringArray& aChecked)
{
	CStringArray aUnused;

	return SetChecked(aChecked, aUnused);
}

BOOL CEnCheckComboBox::SetChecked(const CStringArray& aChecked, const CStringArray& aMixed)
{
	if (m_bMultiSel)
	{
		int nAny = GetItemAnyIndex();
		BOOL bAnyIsChecked = GetCheck(nAny);

		// Check for changes
		CStringArray aCurChecked, aCurMixed;
		GetChecked(aCurChecked, aCurMixed);

		if (Misc::MatchAll(aChecked, aCurChecked) && 
			Misc::MatchAll(aMixed, aCurMixed))
		{
			// Clear 'any'
			if (bAnyIsChecked)
				CCheckComboBox::SetCheck(nAny, CCBC_UNCHECKED, TRUE);
			
			return TRUE;
		}

		// If we're clearing the selection, and we have the 'Any' item,
		// we just set the 'Any' item to 'checked' so as not to lose the
		// existing selection.
		// Caller can call 'ClearMultiSelectionHistory' as required afterwards.
		if ((nAny != CB_ERR) && !aChecked.GetSize() && aCurChecked.GetSize())
		{
			ASSERT(!bAnyIsChecked);

			if (CCheckComboBox::SetCheck(nAny, CCBC_CHECKED, FALSE) == CB_ERR)
				return FALSE;

			m_sText.Empty();
			Invalidate(FALSE);

			return TRUE;
		}

		return CCheckComboBox::SetChecked(aChecked, aMixed);
	}

	// single selection
	ASSERT(aMixed.GetSize() == 0);
	ASSERT(aChecked.GetSize() <= 1);

	if (aChecked.GetSize() == 0)
	{
		SetCurSel(GetItemAnyIndex());
		return TRUE;
	}
	else if (aChecked[0].IsEmpty())
	{
		SetCurSel(GetItemNoneIndex());
		return TRUE;
	}

	// Call CComboBox::SelectString directly because the derived 
	// versions are all virtual AND CCheckComboBox::SelectString
	// calls back into here which results in infinite recursion
	return (CComboBox::SelectString(0, aChecked[0]) != CB_ERR);
}

int CEnCheckComboBox::SetCheck(int nIndex, CCB_CHECKSTATE nCheck)
{
	if (m_bMultiSel)
		return CCheckComboBox::SetCheck(nIndex, nCheck);

	// else
	if (nCheck == CCBC_CHECKED)
	{
		SetCurSel(nIndex);
	}
	else if (nIndex == GetCurSel())
	{
		SetCurSel(-1);
	}

	return nIndex;
}

LRESULT CEnCheckComboBox::OnListboxMessage(UINT msg, WPARAM wp, LPARAM lp)
{
	// we don't prevent the base class from hooking the droplist
	// if single selecting, we just bypass it
	if (m_bMultiSel) 
		return CCheckComboBox::OnListboxMessage(msg, wp, lp);
	
	// else
	return CAutoComboBox::OnListboxMessage(msg, wp, lp);
}

LRESULT CEnCheckComboBox::OnEditboxMessage(UINT msg, WPARAM wp, LPARAM lp)
{
	if (m_bMultiSel)
		return CCheckComboBox::OnEditboxMessage(msg, wp, lp);
	
	// else
	return CAutoComboBox::OnEditboxMessage(msg, wp, lp);
}

void CEnCheckComboBox::OnLBSelChange()
{
	if (m_bMultiSel)
		CCheckComboBox::OnLBSelChange();
	
	// else
	CAutoComboBox::Default();
}

BOOL CEnCheckComboBox::OnSelEndOK()
{
	if (m_bMultiSel)
		return CCheckComboBox::OnSelEndOK();
	
	// else
	m_bEditChange = TRUE;
	return FALSE; // Route to parent
}

LRESULT CEnCheckComboBox::OnGetTextLen(WPARAM wParam, LPARAM lParam)
{
	if (m_bMultiSel)
		return CCheckComboBox::OnGetTextLen(wParam, lParam);

	return CAutoComboBox::Default();
}

void CEnCheckComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
									DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
{
	CString sEnText = GetItemText(nItem, sItem);

	if (m_bMultiSel)
	{
		if ((nItemState & ODS_SELECTED) == 0)
		{
			if (bList)
			{
				if (nItem != -1)
				{
					// Gray-out all items apart from <any> is <any> is checked
					if (!IsItemAnyIndex(nItem) && IsItemAnyChecked())
					{
						crText = GetSysColor(COLOR_GRAYTEXT);
					}
				}
			}
			else if (sEnText == m_sAny)
			{
				// Display <any> in window prompt colour if nothing is checked
				crText = CWndPrompt::GetTextColor(*this);
			}
		}

		CCheckComboBox::DrawItemText(dc, rect, nItem, nItemState, dwItemData, sEnText, bList, crText);
	}
	else
	{
		CAutoComboBox::DrawItemText(dc, rect, nItem, nItemState, dwItemData, sEnText, bList, crText);
	}
}

BOOL CEnCheckComboBox::DrawCheckBox(CDC& dc, const CRect& rect, int nItem, UINT nItemState, DWORD dwItemData, BOOL bDisabled) const
{
	ASSERT(m_bMultiSel);

	if (!(nItemState & ODS_SELECTED) && !IsItemAnyIndex(nItem) && IsItemAnyChecked())
	{
		bDisabled = TRUE;
	}

	return CCheckComboBox::DrawCheckBox(dc, rect, nItem, nItemState, dwItemData, bDisabled);
}

