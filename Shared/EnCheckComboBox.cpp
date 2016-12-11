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


BEGIN_MESSAGE_MAP(CEnCheckComboBox, CCheckComboBox)
//{{AFX_MSG_MAP(CEnCheckComboBox)
//}}AFX_MSG_MAP
ON_CONTROL(LBN_SELCHANGE, 1000, OnLBSelChange)
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
				CheckAll(FALSE);
				m_sText.Empty();
			}
		}
		else 
		{
			// set the current selection to whatever was singly selected
			// provided it's not blank		
			CheckAll(FALSE);

			int nSel = GetCurSel();

			if (nSel != CB_ERR)
			{
				CString sItem;
				GetLBText(nSel, sItem);

				if (!sItem.IsEmpty())
					SetCheck(nSel, TRUE);
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

int CEnCheckComboBox::GetChecked(CStringArray& aItems) const
{
	if (m_bMultiSel)
		return CCheckComboBox::GetChecked(aItems);
	
	// else
	aItems.RemoveAll();
	
	if (CComboBox::GetCurSel() != CB_ERR)
	{
		CString sItem;
		GetLBText(CComboBox::GetCurSel(), sItem);

		// we don't add the blank item
		if (!sItem.IsEmpty())
			aItems.Add(sItem);
	}
	
	return aItems.GetSize();
}

BOOL CEnCheckComboBox::SetChecked(const CStringArray& aItems)
{
	if (m_bMultiSel)
		return CCheckComboBox::SetChecked(aItems);

	if (aItems.GetSize() == 0)
	{
		SetCurSel(-1);
		return TRUE;
	}

	// Call CComboBox::SelectString directly because the derived 
	// versions are all virtual AND CCheckComboBox::SelectString
	// calls back into here which results in infinite recursion
	return (CComboBox::SelectString(0, aItems[0]) != CB_ERR);
}

int CEnCheckComboBox::SetCheck(int nIndex, BOOL bCheck)
{
	if (m_bMultiSel)
		return CCheckComboBox::SetCheck(nIndex, bCheck);

	// else
	if (bCheck)
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

