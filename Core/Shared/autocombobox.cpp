// autocombobox.cpp : implementation file
//

#include "stdafx.h"
#include "autocombobox.h"
#include "holdredraw.h"
#include "autoflag.h"
#include "misc.h"
#include "enstring.h"
#include "graphicsmisc.h"
#include "dialoghelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const LPCTSTR STR_CLOSEBTN	= _T("r");
const int SIZE_CLOSEBTN		= GraphicsMisc::ScaleByDPIFactor(8);
const COLORREF RED			= RGB(200, 90, 90);
const COLORREF WHITE		= RGB(240, 240, 240);

/////////////////////////////////////////////////////////////////////////////
// CAutoComboBox

CAutoComboBox::CAutoComboBox(DWORD dwFlags) 
	: 
	m_dwFlags(dwFlags), 
	m_bNotifyingParent(FALSE), 
	m_bEditChange(FALSE), 
	m_nDeleteItem(LB_ERR),
	m_bSkipAutoComplete(FALSE),
	m_nHotSimpleListItem(LB_ERR),
	m_nAutoCompleteMatch(LB_ERR)
{
}

CAutoComboBox::~CAutoComboBox()
{
}

IMPLEMENT_DYNAMIC(CAutoComboBox, COwnerdrawComboBoxBase)

BEGIN_MESSAGE_MAP(CAutoComboBox, COwnerdrawComboBoxBase)
	//{{AFX_MSG_MAP(CAutoComboBox)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_WM_KEYDOWN()
	ON_CONTROL_REFLECT_EX(CBN_SELENDCANCEL, OnSelEndCancel)
	ON_CONTROL_REFLECT_EX(CBN_SELENDOK, OnSelEndOK)
	ON_CONTROL_REFLECT_EX(CBN_SELCHANGE, OnSelChange)
	ON_CONTROL_REFLECT_EX(CBN_CLOSEUP, OnCloseUp)
	ON_CONTROL_REFLECT_EX(CBN_DROPDOWN, OnDropDown)
	ON_CONTROL_REFLECT_EX(CBN_EDITCHANGE, OnEditChange)
	ON_CONTROL_REFLECT_EX(CBN_EDITUPDATE, OnEditUpdate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoComboBox message handlers

void CAutoComboBox::ModifyFlags(DWORD dwRemove, DWORD dwAdd)
{
	Misc::ModifyFlags(m_dwFlags, dwRemove, dwAdd);
}

int CAutoComboBox::SetStrings(const CStringArray& aItems) 
{ 
	CHoldRedraw hr(*this);

	ResetContent(); 

	return AddUniqueItems(aItems); 
}

BOOL CAutoComboBox::OnCloseUp()
{
	m_nAutoCompleteMatch = LB_ERR;

	// notify parent of (possible) selection change
	if (m_bEditChange)
	{
		// Clear edit change flag to protect against
		// re-entrancy during parent notifications
		m_bEditChange = FALSE;

		ParentCBNotify(CBN_SELCHANGE);
	}
	else
	{
		ParentCBNotify(CBN_SELENDCANCEL);
	}
	
	return FALSE; // pass to parent
}

BOOL CAutoComboBox::OnDropDown()
{
	return FALSE; // pass to parent
}

BOOL CAutoComboBox::OnEditUpdate()
{
	m_nAutoCompleteMatch = LB_ERR;

	if (Misc::HasFlag(m_dwFlags, ACBS_AUTOCOMPLETE) && !m_bSkipAutoComplete)
	{
		CString sText = GetEditText();
		int nCaretPos = LOWORD(m_scEdit.SendMessage(EM_GETSEL));

		m_nAutoCompleteMatch = UpdateEditAutoComplete(GetEditText(), nCaretPos);

		if ((m_nAutoCompleteMatch != LB_ERR) && GetDroppedState())
			m_scList.Invalidate();
	}

	return FALSE; // pass to parent
}

int CAutoComboBox::UpdateEditAutoComplete(const CString& sText, int nCaretPos)
{
	ASSERT(Misc::HasFlag(m_dwFlags, ACBS_AUTOCOMPLETE));
	ASSERT(!m_bSkipAutoComplete);

	CString sInput = sText.Left(nCaretPos);
	int nMatch = FindString(-1, sInput);

	if (nMatch != CB_ERR)
	{
		CString sMatch;
		GetLBText(nMatch, sMatch);

		::SetWindowText(GetEdit(), sMatch);

		int nStart = sText.Find(sInput);
		ASSERT(nStart >= 0);

		m_scEdit.SendMessage(EM_SETSEL, (WPARAM)(nStart + sInput.GetLength()), (LPARAM)-1);
	}

	return nMatch;
}

BOOL CAutoComboBox::OnEditChange()
{
	m_bEditChange = TRUE;
	
	return FALSE; // pass to parent
}

BOOL CAutoComboBox::OnSelEndOK()
{
	m_nAutoCompleteMatch = LB_ERR;

	OnSelChange();
	HandleReturnKey();

	return FALSE; // continue routing
}

BOOL CAutoComboBox::OnSelEndCancel()
{
	m_nAutoCompleteMatch = LB_ERR;

	// eat this unless we sent it explicitly
	return (!m_bNotifyingParent);
}

BOOL CAutoComboBox::OnSelChange()
{
	if (!m_bNotifyingParent)
	{
		// make sure the edit control is up to date
		// unless we sent this notification
		if (m_scEdit.IsValid())
		{
			int nSel = GetCurSel();

			if (nSel < GetCount() )
				SetWindowText(GetSelectedItemText());
		}

		m_bEditChange = TRUE;
	}

	// eat notification if dropped down
	// ie we don't send selection notifications until the dropdown closes
	return GetDroppedState() && !IsType(CBS_SIMPLE);
}

CString CAutoComboBox::GetSelectedItemText() const
{
	return GetItemText(GetCurSel());
}

CString CAutoComboBox::GetItemText(int nItem) const
{
	CString sItem;

	if (nItem != CB_ERR)
		GetLBText(nItem, sItem);

	return sItem;
}

int CAutoComboBox::DeleteString(LPCTSTR szItem, BOOL bCaseSensitive)
{
	int nItem = FindStringExact(-1, szItem, bCaseSensitive);

	if (nItem != CB_ERR)
		return DeleteString(nItem);

	// else
	return CB_ERR;
}

int CAutoComboBox::DeleteItems(const CStringArray& aItems, BOOL bCaseSensitive)
{
	int nDeleted = 0;
	int nItem = aItems.GetSize();

	while (nItem--)
	{
		const CString& sItem = Misc::GetItem(aItems, nItem);

		if (DeleteString(sItem, bCaseSensitive) != CB_ERR)
			nDeleted++;
	}

	return nDeleted;
}

BOOL CAutoComboBox::AddEmptyString()
{
	int nItem = FindStringExact(-1, _T(""));

	if (nItem == CB_ERR)
	{
		if (AddToStart())
			nItem = COwnerdrawComboBoxBase::InsertString(0, _T(""));
		else
			nItem = COwnerdrawComboBoxBase::AddString(_T(""));
	}

	return (nItem != CB_ERR);
}

int CAutoComboBox::AddUniqueItem(const CString& sItem)
{
	return AddUniqueItem(sItem, AddToStart());
}

int CAutoComboBox::AddUniqueItem(const CString& sItem, BOOL bAddToStart)
{
	return InsertUniqueItem(bAddToStart ? 0 : -1, sItem);
}

int CAutoComboBox::InsertUniqueItem(int nIndex, const CString& sNewItem)
{
	if (nIndex < 0 || nIndex > GetCount())
		nIndex = -1; // insert at end if not exists

	CString sItem(sNewItem);
	Misc::Trim(sItem);
	
	if (!sItem.IsEmpty())
	{
		int nFind = FindStringExact(-1, sItem, CaseSensitive());
		
		if (nFind != CB_ERR) // items already exists
		{
			CString sLBItem = GetItemText(nFind);

			if (nIndex == -1)
				nIndex = nFind; // leave it in it's current position
			
			if (nIndex != nFind || sItem != sLBItem)
			{
				// save selection so we can restore it
				CString sSelItem = GetItemText(GetCurSel());
				
				// be sure to transfer item data
				DWORD dwItemData = GetItemData(nFind);
				
				DeleteString(nFind); // remove original

				if (GetStyle() & CBS_SORT)
					nIndex = COwnerdrawComboBoxBase::AddString(sItem); // re-insert
				else
					nIndex = COwnerdrawComboBoxBase::InsertString(nIndex, sItem); // re-insert

				SetItemData(nIndex, dwItemData);
				
				if (nIndex != CB_ERR)
					RefreshDropWidth();
				
				// restore selection
				if (!sSelItem.IsEmpty())
					SelectString(-1, sSelItem);
			}
			
			return nIndex;
		}
		else
		{
			if (GetStyle() & CBS_SORT)
				nIndex = COwnerdrawComboBoxBase::AddString(sItem); // re-insert
			else
				nIndex = COwnerdrawComboBoxBase::InsertString(nIndex, sItem); // re-insert
			
			if (nIndex != CB_ERR)
			{
				SetItemData(nIndex, GetNewItemData());
				RefreshDropWidth();
			}
			
			return nIndex;
		}
	}
	
	return CB_ERR; // invalid item
}

void CAutoComboBox::Flush() 
{ 
	if (m_bEditChange) 
		HandleReturnKey(); 
}

int CAutoComboBox::GetExtraListboxWidth() const
{
	// space for close button
	int nWidth = COwnerdrawComboBoxBase::GetExtraListboxWidth();

	if (!IsType(CBS_DROPDOWNLIST))
		nWidth += SIZE_CLOSEBTN;

	return nWidth;
}

BOOL CAutoComboBox::GetListDeleteButtonRect(const CRect& rItem, CRect& rBtn) const
{
	if (IsType(CBS_DROPDOWNLIST))
		return FALSE;

	rBtn = rItem;

	rBtn.left = rBtn.right - SIZE_CLOSEBTN;
	rBtn.DeflateRect(0, ((rBtn.Height() - SIZE_CLOSEBTN) / 2));

	return TRUE;
}

void CAutoComboBox::GetItemColors(int nItem, UINT nItemState, DWORD dwItemData,
								  COLORREF& crText, COLORREF& crBack) const
{
	if (IsAutoCompleteMatch(nItem))
		nItemState |= ODS_SELECTED;

	COwnerdrawComboBoxBase::GetItemColors(nItem, nItemState, dwItemData, crText, crBack);
}

BOOL CAutoComboBox::IsAutoCompleteMatch(int nItem) const
{
	return (Misc::HasFlag(m_dwFlags, ACBS_AUTOCOMPLETE) &&
			(nItem != -1) &&
			(m_nAutoCompleteMatch == nItem) &&
			(GetCurSel() == CB_ERR));
}

void CAutoComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
								DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
{
	if (bList && IsAutoCompleteMatch(nItem))
		nItemState |= ODS_SELECTED;

	COwnerdrawComboBoxBase::DrawItemText(dc, rect, nItem, nItemState, dwItemData, sItem, bList, crText);

	// draw 'delete' mark for non-empty deletable list items
	// but not if we are a disabled simple combo
	if (bList && !sItem.IsEmpty() && AllowDelete())
	{
		// create font first time
		if (m_fontClose.GetSafeHandle() == NULL)
			GraphicsMisc::CreateFont(m_fontClose, _T("Marlett"), 6);
		
		CFont* pOldFont = dc.SelectObject(&m_fontClose);
		
		CRect rBtn;
		VERIFY(GetListDeleteButtonRect(rect, rBtn));

		BOOL bHighlightDeleteBtn = (nItemState & ODS_SELECTED);

		if (!bHighlightDeleteBtn && IsType(CBS_SIMPLE))
			bHighlightDeleteBtn = (nItem == m_nHotSimpleListItem);
		
		// set the color to white-on-red if highlighted
		if (bHighlightDeleteBtn)
		{
			dc.SetTextColor(WHITE);
			dc.FillSolidRect(rBtn, RED);
		}
		else
		{
			dc.SetTextColor(GetSysColor(COLOR_3DDKSHADOW));
		}

		dc.SetTextAlign(TA_TOP | TA_LEFT);
		dc.SetBkMode(TRANSPARENT);
		dc.TextOut(rBtn.left + 1, rBtn.top + 1, STR_CLOSEBTN);
		dc.SelectObject(pOldFont);
	}
}

void CAutoComboBox::OnSize(UINT nType, int cx, int cy) 
{
	CWnd* pEdit = GetDlgItem(1001);

	// if the edit control does not have the focus then hide the selection
	if (pEdit && pEdit != GetFocus())
	{
		CHoldRedraw hr(*pEdit);
		COwnerdrawComboBoxBase::OnSize(nType, cx, cy);
	
		pEdit->SendMessage(EM_SETSEL, (UINT)-1, 0);
	}
	else
	{
		COwnerdrawComboBoxBase::OnSize(nType, cx, cy);
	}

	RefreshDropWidth(FALSE);
}

int CAutoComboBox::AddUniqueItems(const CStringArray& aItems)
{
    int nItemCount = aItems.GetSize(), nCount = 0;

	// maintain order
    for (int nItem = 0; nItem < nItemCount; nItem++)
    {
		const CString& sItem = Misc::GetItem(aItems, nItem);

        if (AddUniqueItem(sItem, FALSE) != CB_ERR)
            nCount++;
    }

	if (nCount)
		RefreshDropWidth();

    return nCount;
}

int CAutoComboBox::AddUniqueItems(const CAutoComboBox& cbItems)
{
    CStringArray aItems;

    if (cbItems.GetItems(aItems))
        return AddUniqueItems(aItems);

    // else
    return 0;
}

int CAutoComboBox::GetItems(CStringArray& aItems) const
{
    int nItem = GetCount();

    aItems.RemoveAll();
    aItems.SetSize(nItem);

    while (nItem--)
    {
        CString sItem = GetItemText(nItem);
        aItems.SetAt(nItem, sItem); // maintain order
    }

    return aItems.GetSize();
}

LRESULT CAutoComboBox::ScWindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	if (m_scEdit.GetHwnd() == hRealWnd)
		return OnEditboxMessage(msg, wp, lp);

	// else
	ASSERT(m_scList.GetHwnd() == hRealWnd);

	return OnListboxMessage(msg, wp, lp);
}

BOOL CAutoComboBox::DoDeleteListItem(const CPoint& ptClient)
{
	ASSERT(GetDroppedState());

	if (m_nDeleteItem != LB_ERR)
	{
		int nItem = m_nDeleteItem;
		m_nDeleteItem = LB_ERR;
		
		if (HitTestListDeleteBtn(ptClient) == nItem)
		{
			return DeleteLBItem(nItem);
		}
	}

	// else
	return FALSE;
}

LRESULT CAutoComboBox::OnListboxMessage(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg) 
	{
	case WM_LBUTTONDOWN:
		m_nDeleteItem = HitTestListDeleteBtn(lp);

		if (m_nDeleteItem != -1)
			return 1L;
		break;
		
	case WM_LBUTTONUP:
		if (DoDeleteListItem(lp))
		{
			return 1L; // prevent combo closing
		}
		break;

	case WM_MOUSEMOVE:
		if (IsType(CBS_SIMPLE))
		{
			int nHit = HitTestList(lp);

			if (nHit != m_nHotSimpleListItem)
			{
				int nPrev = m_nHotSimpleListItem;
				m_nHotSimpleListItem = nHit;

				int nSel = m_scList.SendMessage(LB_GETCURSEL);

				if ((nPrev != LB_ERR) && (nPrev != nSel))
					RedrawListItem(nPrev);

				if ((nHit != LB_ERR) && (nHit != nSel))
				{
					RedrawListItem(nHit);
					CDialogHelper::TrackMouseLeave(m_scList.GetHwnd());
				}
				else
				{
					CDialogHelper::TrackMouseLeave(m_scList.GetHwnd(), FALSE);
				}
			}
		}
		break;

	case WM_MOUSELEAVE:
		if (IsType(CBS_SIMPLE))
		{
			int nSel = m_scList.SendMessage(LB_GETCURSEL);

			if (m_nHotSimpleListItem != LB_ERR)
			{
				int nPrev = m_nHotSimpleListItem;
				m_nHotSimpleListItem = LB_ERR;

				if (nPrev != nSel)
					RedrawListItem(nPrev);
			}

			CDialogHelper::TrackMouseLeave(m_scList.GetHwnd(), FALSE);
		}
		break;
	}
	
	return CSubclasser::ScDefault(m_scList);
}

void CAutoComboBox::RedrawListItem(int nItem) const
{
	CRect rItem;
	m_scList.SendMessage(LB_GETITEMRECT, nItem, (LPARAM)(LPRECT)&rItem);

	TRACE(_T("CAutoComboBox::OnListboxMessage(hot = %d)\n"), m_nHotSimpleListItem);

	::InvalidateRect(m_scList.GetHwnd(), rItem, FALSE);
	::UpdateWindow(m_scList.GetHwnd());
}

LRESULT CAutoComboBox::OnEditboxMessage(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		{
			m_bSkipAutoComplete = FALSE;

			switch (wp)
			{
			case VK_BACK:
				// else the just deleted selection will be restored
				m_bSkipAutoComplete = TRUE;
				break;

			case VK_DELETE:
				{
					// else the just deleted selection will be restored
					m_bSkipAutoComplete = TRUE;

					if (AllowDelete() && GetDroppedState() && Misc::IsKeyPressed(VK_CONTROL))
					{
						if (DeleteLBItem(m_scList.SendMessage(LB_GETCURSEL)))
						{
							// eat message else it'll go to the edit window
							return 0L;
						}
					}
				}
				break;

			case VK_RETURN:
				if (GetDroppedState() && !IsType(CBS_SIMPLE))
					ShowDropDown(FALSE);

				HandleReturnKey();
				return 0L;

			case VK_UP:
			case VK_DOWN:
				if (!GetDroppedState() && !IsType(CBS_SIMPLE) && GetCount())
				{
					ShowDropDown();
					return 0L; // eat
				}
				break;

			case VK_ESCAPE:
				m_bEditChange = FALSE;
				ShowDropDown(FALSE);
				break;
			}
		}
		break;
		
	case WM_GETDLGCODE:
		// special handling of the return key
		if (lp)
		{
			LPMSG pMsg = (LPMSG)lp;

			if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
			{
				// can be simple or not dropped
				if (IsType(CBS_SIMPLE) || !GetDroppedState())
					return DLGC_WANTALLKEYS;
			}
		}
		break;

	case WM_KILLFOCUS:
		if (m_bEditChange)
			HandleReturnKey();
		break;
	}

	return CSubclasser::ScDefault(m_scEdit);
}

void CAutoComboBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ((nChar == VK_ESCAPE) && GetDroppedState() && !m_scEdit.IsValid())
	{
		m_bEditChange = FALSE;
		ShowDropDown(FALSE);

		return;
	}

	// else
	COwnerdrawComboBoxBase::OnKeyDown(nChar, nRepCnt, nFlags);
}

int CAutoComboBox::HitTestListDeleteBtn(const CPoint& ptList) const
{
	if (!IsType(CBS_DROPDOWNLIST))
	{
		int nItem = HitTestList(ptList);
		
		if ((nItem != LB_ERR) && m_scList.SendMessage(LB_GETTEXTLEN, nItem, 0))
		{
			CRect rItem;
			
			if (LB_ERR != m_scList.SendMessage(LB_GETITEMRECT, nItem, (LPARAM)(LPRECT)rItem))
			{
				CRect rBtn;
				GetListDeleteButtonRect(rItem, rBtn);
				rBtn.InflateRect(2, 2);
				
				if (rBtn.PtInRect(ptList))
					return nItem;
			}
		}
	}

	// else
	return LB_ERR;
}

int CAutoComboBox::HitTestList(const CPoint& ptList) const
{
	DWORD dwHit = m_scList.SendMessage(LB_ITEMFROMPOINT, 0, MAKELPARAM(ptList.x, ptList.y));

	if (HIWORD(dwHit))
		return LB_ERR;

	// else
	return LOWORD(dwHit);
}

int CAutoComboBox::GetCurSel() const
{
	CAutoFlag af(m_bDrawing, FALSE);
	
	return CComboBox::GetCurSel();
}

CString CAutoComboBox::GetEditText() const
{
	if (!m_scEdit.IsValid())
		return _T("");

	CString sEdit;
	m_scEdit.GetCWnd()->GetWindowText(sEdit);

	return sEdit;
}

void CAutoComboBox::HandleReturnKey()
{
	// Prevent re-entrancy
	if (m_bNotifyingParent)
		return;

	// Only clear the change flag if we are not
	// dropped else it will be handled in OnCloseUp
	BOOL bDroppedDown = GetDroppedState();

	if (!bDroppedDown)
		m_bEditChange = FALSE;

	if (m_scEdit.IsValid())
	{
		CString sEdit;
		m_scEdit.GetCWnd()->GetWindowText(sEdit);
		
		int nAdd = AddUniqueItem(sEdit);
		
		if (nAdd != CB_ERR)
			ParentACNotify(WM_ACBN_ITEMADDED, nAdd, sEdit);

		if (!sEdit.IsEmpty())
			SelectString(-1, sEdit);
			
		// Send a possible selection change if we are not
		// dropped else it will be handled in OnCloseUp
		if (!bDroppedDown)
			ParentCBNotify(CBN_SELCHANGE);
	}
}

void CAutoComboBox::ParentACNotify(UINT nMsgNotify, int nIndex, LPCTSTR szItem)
{
	ASSERT((nIndex >= 0) && (!szItem || szItem[0]));

	if ((nIndex >= 0) && (!szItem || szItem[0]))
	{
		CAutoFlag af(m_bNotifyingParent, TRUE);
		
		GetParent()->SendMessage(nMsgNotify, MAKEWPARAM(GetDlgCtrlID(), nIndex), (LPARAM)szItem);
	}
}

void CAutoComboBox::ParentCBNotify(UINT nIDNotify)
{
	CWnd* pParent = GetParent();

	if (pParent)
	{
		CAutoFlag af(m_bNotifyingParent, TRUE);
		
		UINT nID = GetDlgCtrlID();
		pParent->SendMessage(WM_COMMAND, MAKEWPARAM(nID, nIDNotify), (LPARAM)m_hWnd);
	}
}

BOOL CAutoComboBox::AllowDelete() const 
{ 
	if (IsWindowEnabled())
	{
		if (Misc::HasFlag(m_dwFlags, ACBS_ALLOWDELETE))
		{
			// Edit field must be present
			return !IsType(CBS_DROPDOWNLIST);
		}
	}

	// all else
	return FALSE;
}

void CAutoComboBox::SetEditMask(LPCTSTR szMask, DWORD dwMaskFlags)
{
	m_eMask.SetMask(szMask, dwMaskFlags);
}

HBRUSH CAutoComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = COwnerdrawComboBoxBase::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// hook list box before base class subclasses it
	if ((nCtlColor == CTLCOLOR_LISTBOX) && !m_scList.IsValid())
	{
		m_scList.HookWindow(*pWnd, this);
	}
	// and hook edit box
	else if ((nCtlColor == CTLCOLOR_EDIT) && !m_scEdit.IsValid())
	{
		m_scEdit.HookWindow(*pWnd, this);

		// mask
		if (m_eMask.IsMasked())
			m_eMask.SubclassWindow(*pWnd);
	}
	
	return hbr;
}

BOOL CAutoComboBox::DeleteLBItem(int nItem)
{
	ASSERT(AllowDelete());
	ASSERT((nItem >= 0) && (nItem < GetCount()));

	if ((nItem >= 0) && (nItem < GetCount()))
	{
		CString sCurItem, sItem = GetItemText(nItem); // for notifying parent
		int nCurSel = GetCurSel();
		
		// save existing selection
		if (nCurSel != nItem)
			GetWindowText(sCurItem);

		// Do the delete
		DeleteString(nItem);
		
		// restore combo selection
		if (!sCurItem.IsEmpty())
		{
			nCurSel = FindStringExact(-1, sCurItem);
		}
		else if (GetCount() == 0)
		{
			nCurSel = CB_ERR; // we've deleted the last item
			SetWindowText(_T("")); // clear edit
			m_bEditChange = TRUE;
		}
		m_scList.SendMessage(LB_SETCURSEL, nCurSel);
		
		// notify parent that we've been fiddling
		ParentACNotify(WM_ACBN_ITEMDELETED, nItem, sItem);

		// resize our window
		if (!IsType(CBS_SIMPLE) && GetCount())
		{
			CRect rList;
			m_scList.GetWindowRect(rList);

			rList.bottom -= GetItemHeight(0);
			m_scList.GetCWnd()->MoveWindow(rList, TRUE);
		}

		return TRUE;
	}

	// else
	return FALSE;
}

void CAutoComboBox::DDX(CDataExchange* pDX, CString& value)
{
	if (pDX->m_bSaveAndValidate)
	{
		::DDX_CBString(pDX, GetDlgCtrlID(), value);
	}
	else
	{
		// try exact first
		int nIndex = FindStringExact(-1, value);

		// then partial
		if (nIndex == CB_ERR)
		{
			nIndex = SelectString(-1, value);

			// if there's a match, check its not partial
			if (nIndex != CB_ERR)
			{
				CString sItem;
				GetLBText(nIndex, sItem);

				if (value.CompareNoCase(sItem) != 0)
					nIndex = CB_ERR;
			}
		}

		// add it and then select it if not empty
		if ((nIndex == CB_ERR) && !value.IsEmpty())
			nIndex = AddString(value);

		SetCurSel(nIndex);
	}
}

