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

void CEnCheckComboBox::PreSubclassWindow() 
{
	CCheckComboBox::PreSubclassWindow();
}

BOOL CEnCheckComboBox::GetCheck(int nIndex) const
{
	if (m_bMultiSel)
		return CCheckComboBox::GetCheck(nIndex);
	
	// else
	return ((nIndex != CB_ERR) && (CComboBox::GetCurSel() == nIndex));
}

int CEnCheckComboBox::GetChecked(CStringArray& aItems, CCB_CHECKSTATE nCheck) const
{
	if (m_bMultiSel)
		return CCheckComboBox::GetChecked(aItems, nCheck);
	
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

BOOL CEnCheckComboBox::SetChecked(const CStringArray& aChecked)
{
	CStringArray aUnused;

	return SetChecked(aChecked, aUnused);
}

BOOL CEnCheckComboBox::SetChecked(const CStringArray& aChecked, const CStringArray& aMixed)
{
	if (m_bMultiSel)
		return CCheckComboBox::SetChecked(aChecked, aMixed);

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
									DWORD dwItemData, const CString& sItem, BOOL bList)
{
	CString sEnText(sItem);
	
	if (m_bMultiSel)
	{
		// if drawing the comma-delimited list and it includes a blank
		// item, prepend <none> to the text
		if (nItem == -1)
		{
			sEnText = m_sText;

			int nBlank = FindStringExact(0, _T(""));

			if (nBlank != -1 && GetCheck(nBlank))
				sEnText = sEnText.Left(nBlank) + m_sNone + sEnText.Mid(nBlank);

			else if (sEnText.IsEmpty())
				sEnText = m_sAny;
		}
		else if (nItem != -1 && sEnText.IsEmpty()) 
			sEnText = m_sNone;
		
		CCheckComboBox::DrawItemText(dc, rect, nItem, nItemState, dwItemData, sEnText, bList);
	}
	else
	{
		if (nItem == -1 || sEnText.IsEmpty())
			sEnText = m_sAny;//None;
		
		CAutoComboBox::DrawItemText(dc, rect, nItem, nItemState, dwItemData, sEnText, bList);
	}
}

