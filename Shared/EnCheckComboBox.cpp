// TDLCategoryComboBox.cpp : implementation file
//

#include "stdafx.h"

#include "enCheckComboBox.h"
#include "misc.h"
#include "enstring.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnCheckComboBox

CEnCheckComboBox::CEnCheckComboBox(BOOL bMulti, UINT nIDNoneString, UINT nIDAnyString) : 
	CCheckComboBox(ACBS_ALLOWDELETE), m_bMultiSel(bMulti), m_sNone("<none>"), m_sAny("<any>")
{
	if (nIDNoneString)
		m_sNone.LoadString(nIDNoneString);

	if (nIDAnyString)
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
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnCheckComboBox message handlers

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
				CString sItem = GetItemText(nSel);

				if (!sItem.IsEmpty())
					SetCheck(nSel, CCBC_CHECKED);
			}
		}
		
		Invalidate();
	}
	
	return TRUE;
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
	while (GetCount() && GetItemText(0).IsEmpty())
		CComboBox::DeleteString(0);
	
	int nEmpty = CalcNumRequiredEmptyStrings();

	while (nEmpty--)
		int nItem = CComboBox::InsertString(0, _T("")); // bypass checks for uniqueness
}

int CEnCheckComboBox::CalcNumRequiredEmptyStrings() const
{
	int nNumReqd = 1; // None

	if (m_bMultiSel)
		nNumReqd++; // Any

	return nNumReqd;
}

int CEnCheckComboBox::GetNoneIndex() const
{
	if (m_sNone.IsEmpty())
		return -1;

	return (CalcNumRequiredEmptyStrings() - 1);
}

int CEnCheckComboBox::GetAnyIndex() const
{
	if (!m_bMultiSel || m_sAny.IsEmpty())
		return -1;

	return 0;
}

BOOL CEnCheckComboBox::IsAnyChecked() const
{
	int nAny = GetAnyIndex();

	return GetCheck(nAny);
}

void CEnCheckComboBox::OnCheckChange(int nIndex)
{
	if (m_bMultiSel)
	{
		int nAny = GetAnyIndex();

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

void CEnCheckComboBox::PreSubclassWindow() 
{
	CCheckComboBox::PreSubclassWindow();
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
		if (IsAnyChecked())
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
		CString sSelItem = GetItemText(CComboBox::GetCurSel());

		// we don't add the blank item
		if (!sSelItem.IsEmpty())
			aItems.Add(sSelItem);
	}
	
	return aItems.GetSize();
}

void CEnCheckComboBox::GetChecked(CStringArray& aChecked, CStringArray& aMixed) const
{
	CCheckComboBox::GetChecked(aChecked, CCBC_CHECKED);
	CCheckComboBox::GetChecked(aMixed, CCBC_MIXED);

	// Remove 'Any'
	if (IsAnyChecked())
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
		if (nItem == -1)
		{
			int nNone = GetNoneIndex();

			if (IsAnyChecked())
			{
				sItem = m_sAny;
			}
			else if (GetCheck(nNone))
			{
				// if drawing the comma-delimited list and it includes
				// a blank item, prefix the text with <none>
				sItem = (m_sNone + m_sText);
			}
			else if (sItem.IsEmpty())
			{
				sItem = m_sAny;
			}
		}
		else
		{
			if (sItem.IsEmpty()) 
			{
				switch (nItem)
				{
				case 0:
					sItem = m_sAny;
					break;

				case 1:
					sItem = m_sNone;
					break;

				default:
					ASSERT(0);
				}
			}
		}
	}
	else if (nItem == -1 || sItem.IsEmpty())
	{
		sItem = m_sAny;
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
		BOOL bRes = FALSE;

// #ifdef _DEBUG
// 		TraceCheckStates();
// #endif
		int nAny = GetAnyIndex();

		if (!aChecked.GetSize() || GetCheck(nAny))
			bRes = (CCheckComboBox::SetCheck(nAny, CCBC_CHECKED, FALSE) != CB_ERR);
		else
			bRes = CCheckComboBox::SetChecked(aChecked, aMixed);

// #ifdef _DEBUG
// 		TraceCheckStates();
// #endif

		return bRes;
	}

	ASSERT(aMixed.GetSize() == 0);

	if (aChecked.GetSize() == 0)
	{
		SetCurSel(-1);
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
	CComboBox::Default();
}

BOOL CEnCheckComboBox::OnSelEndOK()
{
	if (m_bMultiSel)
		return CCheckComboBox::OnSelEndOK();
	
	// else
	m_bEditChange = TRUE;
	return FALSE; // Route to parent
}

void CEnCheckComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
									DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
{
	CString sEnText = GetItemText(nItem, sItem);

	if (m_bMultiSel)
	{
		if (!(nItemState & ODS_SELECTED) && (nItem != -1) && (nItem != GetAnyIndex()) && IsAnyChecked())
		{
			crText = GetSysColor(COLOR_GRAYTEXT);
		}
	}

	CCheckComboBox::DrawItemText(dc, rect, nItem, nItemState, dwItemData, sEnText, bList, crText);
}

BOOL CEnCheckComboBox::DrawCheckBox(CDC& dc, const CRect& rect, int nItem, UINT nItemState, DWORD dwItemData, BOOL bDisabled) const
{
	ASSERT(m_bMultiSel);

	if (!(nItemState & ODS_SELECTED) && (nItem != -1) && (nItem != GetAnyIndex()) && IsAnyChecked())
	{
		bDisabled = TRUE;
	}

	return CCheckComboBox::DrawCheckBox(dc, rect, nItem, nItemState, dwItemData, bDisabled);
}

