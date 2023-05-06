// checkcombobox.cpp : implementation file
//

#include "stdafx.h"
#include "checkcombobox.h"
#include "misc.h"
#include "dlgunits.h"
#include "themed.h"
#include "autoflag.h"
#include "misc.h"
#include "graphicsmisc.h"
#include "dialoghelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

void DDX_CheckItemData(CDataExchange* pDX, CCheckComboBox& combo, DWORD& dwItems)
{
	if (pDX->m_bSaveAndValidate)
		dwItems = combo.GetCheckedItemData();
	else
		combo.SetCheckedByItemData(dwItems);
}

/////////////////////////////////////////////////////////////////////////////
// CCheckComboBox

const int CCheckComboBox::CHECKBOX_SIZE = GraphicsMisc::ScaleByDPIFactor(13);

/////////////////////////////////////////////////////////////////////////////

CCheckComboBox::CCheckComboBox(DWORD dwFlags) : CAutoComboBox(dwFlags)
{
	m_bDrawing = TRUE;
	m_bTextFits = TRUE;
	m_bChecking = FALSE;
	m_nClickedItem = LB_ERR;
}

CCheckComboBox::~CCheckComboBox()
{
}

IMPLEMENT_DYNAMIC(CCheckComboBox, CAutoComboBox)

BEGIN_MESSAGE_MAP(CCheckComboBox, CAutoComboBox)
	//{{AFX_MSG_MAP(CCheckComboBox)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_CHAR()
	ON_CONTROL_REFLECT_EX(CBN_EDITCHANGE, OnEditchange)
	ON_CONTROL_REFLECT_EX(CBN_DROPDOWN, OnDropdown)
	ON_CONTROL_REFLECT_EX(CBN_CLOSEUP, OnCloseUp)
	ON_CONTROL(LBN_SELCHANGE, 1000, OnLBSelChange)
	ON_MESSAGE(WM_GETTEXTLENGTH, OnGetTextLen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckComboBox message handlers

void CCheckComboBox::OnDestroy()
{
	m_tooltip.DestroyWindow();

	CAutoComboBox::OnDestroy();
}

void CCheckComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
								DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
{
	if (bList)
	{
		DrawCheckBox(dc, rect, nItem, nItemState, dwItemData, FALSE);

		CRect rText(rect);
		rText.left += CHECKBOX_SIZE + 2;
		
		CAutoComboBox::DrawItemText(dc, rText, nItem, nItemState, dwItemData, sItem, TRUE, crText);
	}
	else // static portion
	{
		CString sText(!sItem.IsEmpty() ? sItem : m_sText);

		if (!sText.IsEmpty())
		{
			CAutoComboBox::DrawItemText(dc, rect, nItem, nItemState, dwItemData, sText, FALSE, crText);

			CSize sizeText = dc.GetTextExtent(sText);
			m_bTextFits = (sizeText.cx <= rect.Width());
		}
	}
}

void CCheckComboBox::DrawCheckBox(CDC& dc, const CRect& rect, int nItem, UINT /*nItemState*/, DWORD dwItemData, BOOL bDisabled) const
{
	// Otherwise it is one of the items
	UINT nCheckState = (DFCS_BUTTONCHECK | (bDisabled ? DFCS_INACTIVE : 0));

	switch (GetCheck(nItem))
	{
	case CCBC_UNCHECKED:
		// no more to do
		break;

	case CCBC_CHECKED:
		nCheckState |= DFCS_CHECKED;
		break;

	case CCBC_MIXED:
		nCheckState |= DFCS_MIXED;
		break;
	}

	CRect rCheck(rect);
	rCheck.left = 1;
	rCheck.right = (rCheck.left + CHECKBOX_SIZE + 1);
	
	CThemed::DrawFrameControl(CWnd::FromHandle(GetListbox()), &dc, rCheck, DFC_BUTTON, nCheckState);
}

int CCheckComboBox::GetExtraListboxWidth() const
{
	return (CAutoComboBox::GetExtraListboxWidth() + CHECKBOX_SIZE + 2);
}

int CCheckComboBox::CalcMinItemHeight(BOOL bList) const
{
	BOOL nMinHeight = CAutoComboBox::CalcMinItemHeight(bList);

	if (bList)
		nMinHeight = max(nMinHeight, (CHECKBOX_SIZE + 2));

	return nMinHeight;
}

LRESULT CCheckComboBox::OnGetTextLen(WPARAM wParam, LPARAM lParam)
{
	if (!CDialogHelper::ComboHasEdit(*this))
		return m_sText.GetLength();

	return CComboBox::Default();
}

void CCheckComboBox::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const UINT CTRL_A = 0x01;

	// Windows converts Ctrl+A to special character
	if ((nChar == CTRL_A) && GetDroppedState())
	{
		if (IsAnyChecked(CCBC_UNCHECKED))
			CheckAll(CCBC_CHECKED);
		else
			CheckAll(CCBC_UNCHECKED);
	}

	CAutoComboBox::OnChar(nChar, nRepCnt, nFlags);
}

void CCheckComboBox::CheckAll(CCB_CHECKSTATE nCheck)
{
	CheckAll(nCheck, TRUE);
}

void CCheckComboBox::CheckAll(CCB_CHECKSTATE nCheck, BOOL bUpdate)
{
	int nItem = GetCount();

	while (nItem--)
		SetCheck(nItem, nCheck, FALSE);

	// derived classes
	OnCheckChange(-1);
	
	// update text
	RecalcText();
	
	// redraw listbox if dropped
	if (bUpdate && GetDroppedState())
	{
		::InvalidateRect(GetListbox(), NULL, FALSE);
		::UpdateWindow(GetListbox());
	}
}

BOOL CCheckComboBox::IsAnyChecked(CCB_CHECKSTATE nCheck) const
{
	int nItem = GetCount();

	while (nItem--)
	{
		if (GetCheck(nItem) == nCheck)
			return TRUE;
	}

	return FALSE;
}

int CCheckComboBox::GetCheckStates(CArray<CCB_CHECKSTATE, CCB_CHECKSTATE>& aStates) const
{
	int nItem = GetCount();
	aStates.SetSize(nItem);

	while (nItem--)
		aStates[nItem] = GetCheck(nItem);

	return aStates.GetSize();
}

void CCheckComboBox::RecalcText(BOOL bUpdate, BOOL bNotify)
{
	CString sPrevText = m_sText;
	m_sText = FormatCheckedItems();
	
	// update edit field if necessary
	if (CDialogHelper::ComboHasEdit(*this))
	{
		if (bUpdate)
		{
			SetDlgItemText(1001, m_sText);
				
			// notify parent
			if (bNotify && sPrevText != m_sText)
				ParentCBNotify(CBN_EDITCHANGE);
		}
	}
	else
	{
		Invalidate(FALSE);
	}
}

int CCheckComboBox::SelectString(int nStartAfter, LPCTSTR lpszString)
{
   CStringArray aItems;

   if (Misc::Split(lpszString, aItems)) 
   {
      SetChecked(aItems);
      return GetCurSel();
   }
   
   // else
   return CAutoComboBox::SelectString(nStartAfter, lpszString);
}

int CCheckComboBox::SetCheckByItemData(DWORD dwItemData, CCB_CHECKSTATE nCheck)
{
	int nIndex = CDialogHelper::FindItemByData(*this, dwItemData);

	return SetCheck(nIndex, nCheck, TRUE);
}

int CCheckComboBox::SetCheck(int nIndex, CCB_CHECKSTATE nCheck)
{
	return SetCheck(nIndex, nCheck, TRUE);
}

int CCheckComboBox::SetCheck(int nIndex, CCB_CHECKSTATE nCheck, BOOL bUpdate)
{
	if ((nIndex == -1) || (nIndex >= GetCount()))
		return CB_ERR;

	CCB_ITEMDATA* pItemData = (CCB_ITEMDATA*)GetAddExtItemData(nIndex);
	ASSERT(pItemData);

	if (pItemData == NULL)
		return CB_ERR;

	pItemData->nCheck = nCheck;

	if (bUpdate && !m_bChecking) // prevent re-entrancy
	{
		CAutoFlag af(m_bChecking, TRUE);

		// derived classes
		OnCheckChange(nIndex);
		
		// update text
		RecalcText();
		
		// Redraw the window
		Invalidate(FALSE);
	}
	
	return CB_OKAY;
}

CCB_CHECKSTATE CCheckComboBox::GetCheck(int nIndex) const
{
	if ((nIndex == -1) || !GetCount())
		return CCBC_UNCHECKED;

	CCB_ITEMDATA* pItemData = (CCB_ITEMDATA*)GetExtItemData(nIndex);

	if (pItemData == NULL)
		return CCBC_UNCHECKED; // default

	return pItemData->nCheck;
}

CCB_CHECKSTATE CCheckComboBox::GetCheckByData(DWORD dwItemData) const
{
	int nIndex = CDialogHelper::FindItemByData(*this, dwItemData);
	
	return GetCheck(nIndex);
}

BOOL CCheckComboBox::ParseText(BOOL bAutoAdd)
{
	if (IsType(CBS_DROPDOWNLIST))
		return FALSE;

	CString sEditText;
	GetDlgItemText(1001, sEditText);
	
	// Clear existing checks first but don't update window
	int nItem = GetCount();
	
	while (nItem--)
	{
		if (GetCheck(nItem) == CCBC_CHECKED)
			SetCheck(nItem, CCBC_UNCHECKED, FALSE);
	}
	
	// now parse the text and set the check states manually
	CStringArray aText;
	BOOL bAdded = FALSE;

	if (Misc::Split(sEditText, aText))
	{
		int nText = aText.GetSize();

		while (nText--)
		{
			const CString& sItem = Misc::GetItem(aText, nText);
			int nIndex = FindStringExact(-1, sItem);
			
			if ((nIndex == CB_ERR) && bAutoAdd)
			{
				nIndex = AddUniqueItem(sItem);
				ASSERT(nIndex != CB_ERR);

				bAdded = TRUE;
			}
			
			if (nIndex != CB_ERR)
				SetCheck(nIndex, CCBC_CHECKED, FALSE);
		}
	}

	if (GetDroppedState())
		::InvalidateRect(GetListbox(), NULL, TRUE);
	
	// Redraw the window
	Invalidate(FALSE);

	return bAdded;
}

// this handles messages destined for the dropped listbox
LRESULT CCheckComboBox::OnListboxMessage(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg) 
	{
	case LB_GETCURSEL: 
		// Make the combobox always return -1 as the current selection for
		// combos without an edit potion when drawing. This
		// causes the lpDrawItemStruct->itemID in DrawItem() to be -1
		// when the always-visible-portion of the combo is drawn
		if (IsType(CBS_DROPDOWNLIST) && m_bDrawing)
			return -1;
		break;
				
	case WM_CHAR: 
		// sent by the edit control (if present) or the list box
		if (wp == VK_SPACE && GetDroppedState()) 
		{
			// Get the current selection
			int nIndex = GetCurSel();
			
			CRect rcItem;
			::SendMessage(GetListbox(), LB_GETITEMRECT, nIndex, (LPARAM)(LPRECT)&rcItem);
			::InvalidateRect(GetListbox(), rcItem, FALSE);
			
			// Invert the check mark
			ToggleCheck(nIndex);
			m_bEditChange = TRUE;
			
			// Notify that selection has changed
			if (IsType(CBS_DROPDOWNLIST))
				ParentCBNotify(CBN_SELCHANGE);
			
			return 0;
		}
		break;

	case WM_LBUTTONDBLCLK:
		return 0L; // eat
	
	case WM_LBUTTONDOWN:
		m_nClickedItem = LB_ERR;

		// base class handling
		if (HitTestListDeleteBtn(lp) == LB_ERR)
		{
			m_nClickedItem = HitTestList(lp);

			if (m_nClickedItem != LB_ERR)
				return 1L;
		}
		// Do the default handling now (such as close the popup
		// window when clicked outside)
		break;
		
	case WM_LBUTTONUP: 
		// base class handling
		if (DoDeleteListItem(lp))
		{
			return 1L;
		}
		else if (m_nClickedItem != LB_ERR)
		{
			int nItem = m_nClickedItem;
			m_nClickedItem = LB_ERR;

			if (HitTestList(lp) == nItem)
			{
				// toggle check state
				::InvalidateRect(GetListbox(), NULL, FALSE);

				ToggleCheck(nItem);
				m_bEditChange = TRUE;
					
				// Notify that selection has changed
				if (IsType(CBS_DROPDOWNLIST))
					ParentCBNotify(CBN_SELCHANGE);

				return 1L;
			}
		}
		break;
	}

	return CAutoComboBox::OnListboxMessage(msg, wp, lp);
}

int CCheckComboBox::AddUniqueItem(const CString& sItem)
{
	if (sItem.Find(Misc::GetListSeparator()) != -1)
	{
		CStringArray aText;
		
		Misc::Split(sItem, aText);
		return CAutoComboBox::AddUniqueItems(aText);
	}
	
	// else add single item
	return CAutoComboBox::AddUniqueItem(sItem);;
}

BOOL CCheckComboBox::OnEditchange() 
{
	// if we generated this call as a result 
	// of an explicit call to SetCheck then eat it.
	if (m_bChecking)
		return TRUE; 

	// update check states
	if (GetDroppedState())
	{
		ParseText(FALSE);

		// update m_sText manually to point to
		// whatever has been input
		GetDlgItemText(1001, m_sText);
	}

	return CAutoComboBox::OnEditChange();
}

BOOL CCheckComboBox::OnDropdown() 
{
	CAutoComboBox::OnDropDown();

	// Snapshot the mixed items so that we can handle
	// them as tri-state checkboxes while dropped
	GetChecked(m_aMixedItems, CCBC_MIXED);

	ParseText();
	RecalcText(FALSE); // updates m_sText only
	
	return FALSE; // pass to parent
}

BOOL CCheckComboBox::OnCloseUp() 
{
	CAutoComboBox::OnCloseUp();

	// cleanup
	m_aMixedItems.RemoveAll();
	
	return FALSE; // pass to parent
}

// this handles messages destined for the embedded edit field
LRESULT CCheckComboBox::OnEditboxMessage(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CHAR: 
		{
			switch (wp)
			{
				// forward CTRL+Space to listbox
			case VK_SPACE:
				if (Misc::IsKeyPressed(VK_CONTROL))
				{
					if ((GetDroppedState() && IsType(CBS_DROPDOWN)) || IsType(CBS_SIMPLE)) 
					{
						OnListboxMessage(msg, wp, lp);
						return 0;
					}
				}
				break;
			}
		}
		break;

	case EM_SETSEL:
		// if this is a 'select all' and we're dropped then
		// assume it was in response to CTRL+A in the edit field
		if (GetDroppedState() && (wp == 0) && (lp == (LPARAM)-1))
		{ 
			if (IsAnyChecked(CCBC_UNCHECKED))
				CheckAll(CCBC_CHECKED);
			else
				CheckAll(CCBC_UNCHECKED);
			
			// Notify that selection has changed
			if (IsType(CBS_DROPDOWNLIST))
				ParentCBNotify(CBN_SELCHANGE);
			
			m_bEditChange = TRUE;
			return 0; // eat it
		}
		break;
	}

	// default handling
	return CAutoComboBox::OnEditboxMessage(msg, wp, lp);
}

void CCheckComboBox::HandleReturnKey()
{
	m_bEditChange = FALSE;

	if (!GetDroppedState())
	{
		BOOL bSomeAdded = ParseText();
		RecalcText(FALSE); // update m_sText only

		CAutoComboBox::HandleReturnKey();

		if (bSomeAdded)		
			ParentACNotify(WM_ACBN_ITEMADDED, 0xFFFF, NULL);
	}
	else
	{
		ShowDropDown(FALSE);

		// notify parent of (possible) selection change
		ParentCBNotify(CBN_SELCHANGE);
	}
}

CString CCheckComboBox::GetSelectedItemText() const
{
	return m_sText;
}

void CCheckComboBox::OnLBSelChange()
{
	// do nothing
}

BOOL CCheckComboBox::DeleteLBItem(int nItem)
{
	if (CAutoComboBox::DeleteLBItem(nItem))
	{
		if (GetCount() == 0)
			m_sText.Empty();

		RecalcText();
		return TRUE;
	}

	// else 
	return FALSE;
}

int CCheckComboBox::GetChecked(CStringArray& aItems, CCB_CHECKSTATE nCheck) const
{
	aItems.RemoveAll();

	// Maintain order
	int nCount = GetCount();

	for (int nItem = 0; nItem < nCount; nItem++)
	{
		if (GetCheck(nItem) == nCheck)
			aItems.Add(GetItemText(nItem));
	}	

	return aItems.GetSize();
}

CString CCheckComboBox::FormatCheckedItems(LPCTSTR szSep) const
{
	CStringArray aChecked;
	GetChecked(aChecked);

	return Misc::FormatArray(aChecked, szSep);
}

BOOL CCheckComboBox::SetChecked(const CStringArray& aItems)
{
	CStringArray aUnused;

	return SetChecked(aItems, aUnused);
}

BOOL CCheckComboBox::SetChecked(const CStringArray& aChecked, const CStringArray& aMixed)
{
	// Array should be mutually exclusive
	ASSERT(Misc::MatchAny(aChecked, aMixed, FALSE, TRUE) == FALSE);

	// make sure the items exist in the list
	int nAdded = CAutoComboBox::AddUniqueItems(aChecked);
	nAdded += CAutoComboBox::AddUniqueItems(aMixed);

	// if nothing was added
	if (!nAdded)
	{
		// see if anything has actually changed
		CStringArray aCBChecked, aCBMixed;

		GetChecked(aCBChecked, CCBC_CHECKED);
		GetChecked(aCBMixed, CCBC_MIXED);
		
		if (Misc::MatchAll(aCBChecked, aChecked, FALSE, TRUE) &&
			Misc::MatchAll(aCBMixed, aMixed, FALSE, TRUE))
		{
			// make sure window text matches checked items
			RecalcText(TRUE, FALSE);
			return TRUE; // nothing to do
		}
	}

	// clear existing checks first but don't update window
	int nItem = GetCount();
	
	while (nItem--)
		SetCheck(nItem, CCBC_UNCHECKED, FALSE);
	
	// now set the check states
	BOOL bModified = ModifyChecked(aChecked, CCBC_CHECKED, FALSE);
	bModified |= ModifyChecked(aMixed, CCBC_MIXED, FALSE);

	if (!bModified)
		return FALSE;

	RecalcText(TRUE, FALSE);

	if (GetDroppedState())
		::InvalidateRect(GetListbox(), NULL, TRUE);

	// Redraw the window
	Invalidate(FALSE);

	return TRUE;
}

BOOL CCheckComboBox::ModifyChecked(const CStringArray& aItems, CCB_CHECKSTATE nCheck, BOOL bUpdate)
{
	// Note: Do not clear existing check states
	int nItem = aItems.GetSize();
	
	while (nItem--)
	{
		const CString& sItem = Misc::GetItem(aItems, nItem);

		int nIndex = FindStringExact(-1, sItem);
		
		if (nIndex != CB_ERR)
		{
			SetCheck(nIndex, nCheck, bUpdate);
		}
		else // this ought not to happen
		{
			ASSERT(0);
			return FALSE;
		}
	}

	return TRUE;
}

void CCheckComboBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar)
	{
	case VK_DOWN:
		if (!GetDroppedState())
		{
			ShowDropDown();
			return; // eat
		}
		break;

	case VK_RETURN:
		// We only need to handle this when there's no edit box
		if (!m_scEdit.GetHwnd() && GetDroppedState())
			ShowDropDown(FALSE);
		break;

	case VK_ESCAPE:
		// We only need to handle this when there's no edit box
		if (!m_scEdit.GetHwnd() && GetDroppedState())
			m_bEditChange = FALSE;
		break;
	}
		
	CAutoComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

int CCheckComboBox::GetCheckedCount(CCB_CHECKSTATE nCheck) const
{
	int nCount = 0;
	int nItem = GetCount();

	while (nItem--)
	{
		if (GetCheck(nItem) == nCheck)
			nCount++;
	}

	return nCount;
}

void CCheckComboBox::EnableTooltip(BOOL bEnable)
{
	if (bEnable)
	{
		if (!m_tooltip.GetSafeHwnd())
		{
			VERIFY(m_tooltip.Create(this, (WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP)));

			// Set the multiline tooltip text
			m_tooltip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 300);
		}
	}
	else if (m_tooltip.GetSafeHwnd())
	{
		m_tooltip.DestroyWindow();
	}
}

CString CCheckComboBox::GetTooltip() const
{
	if (!m_bTextFits)
		return FormatCheckedItems(_T("\n"));

	// else
	return "";
}

BOOL CCheckComboBox::ToggleCheck(int nItem)
{
	CCB_CHECKSTATE nCheck = GetCheck(nItem);

	switch (nCheck)
	{
	case CCBC_UNCHECKED:
		// If this item was originally a mixed item when
		// the list was dropped down we treat it as a 
		// tri-state checkbox
		{
			CString sItem = GetItemText(nItem);
			BOOL bTriState = Misc::Contains(sItem, m_aMixedItems, FALSE, TRUE);

			if (bTriState)
				nCheck = CCBC_MIXED;
			else
				nCheck = CCBC_CHECKED;
		}
		break;

	case CCBC_MIXED:
		nCheck = CCBC_CHECKED;
		break;

	case CCBC_CHECKED:
		nCheck = CCBC_UNCHECKED;
		break;
	}

	return SetCheck(nItem, nCheck);
}

int CCheckComboBox::UpdateEditAutoComplete(const CString& sText, int nCaretPos)
{
	CStringArray aValues;
	int nMatch = CB_ERR;

	if (Misc::Split(sText, aValues))
	{
		int nSearchStart = 0;

		for (int nVal = 0; nVal < aValues.GetSize(); nVal++)
		{
			const CString& sValue = aValues[nVal];

			int nValStart = sText.Find(sValue, nSearchStart);
			ASSERT(nValStart != -1);

			int nValEnd = (nValStart + sValue.GetLength());

			if ((nValStart < nCaretPos) && (nCaretPos <= nValEnd))
			{
				CString sInput = sValue.Left(nCaretPos - nValStart);
				nMatch = FindString(-1, sInput);

				if (nMatch != CB_ERR)
				{
					CString sMatch;
					GetLBText(nMatch, sMatch);

					CString sNewText = sText.Left(nValStart) + sMatch + sText.Mid(nValEnd);
					::SetWindowText(GetEdit(), sNewText);

					m_scEdit.SendMessage(EM_SETSEL, (WPARAM)(nValStart + sInput.GetLength()), (LPARAM)nValStart + sMatch.GetLength());
				}

				break; // always
			}

			nSearchStart = (nValStart + sValue.GetLength());
		}
	}

	return nMatch;
}

int CCheckComboBox::SetCheckedByItemData(DWORD dwFlags)
{
	ASSERT(GetSafeHwnd());

	int nNumChecked = 0;

	for (int nItem = 0; nItem < GetCount(); nItem++)
	{
		// get flag for item
		UINT nFlag = GetItemData(nItem);

		// set state
		BOOL bChecked = Misc::HasFlag(dwFlags, nFlag);

		if (bChecked)
		{
			SetCheck(nItem, CCBC_CHECKED);
			nNumChecked++;
		}
		else
		{
			SetCheck(nItem, CCBC_UNCHECKED);
		}
	}

	ASSERT(nNumChecked || !dwFlags); // sanity check

	return nNumChecked;
}

DWORD CCheckComboBox::GetCheckedItemData() const
{
	DWORD dwChecked = 0;

	for (int nItem = 0; nItem < GetCount(); nItem++)
	{
		if (GetCheck(nItem) == CCBC_CHECKED)
			dwChecked |= GetItemData(nItem);
	}

	return dwChecked;
}

BOOL CCheckComboBox::PreTranslateMessage(MSG* pMsg)
{
	FilterToolTipMessage(pMsg);

	return CAutoComboBox::PreTranslateMessage(pMsg);
}

void CCheckComboBox::FilterToolTipMessage(MSG* pMsg)
{
	if (m_tooltip.GetSafeHwnd())
		m_tooltip.FilterToolTipMessage(pMsg);
}

int CCheckComboBox::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	if (m_bTextFits)
		return -1;

	return m_tooltip.SetToolInfo(*pTI, this, GetTooltip(), 1);
}
