// autocombobox.cpp : implementation file
//

#include "stdafx.h"
#include "autocombobox.h"
#include "holdredraw.h"
#include "autoflag.h"
#include "misc.h"
#include "enstring.h"
#include "graphicsmisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const LPCTSTR STR_CLOSEBTN	= _T("r");
const int SIZE_CLOSEBTN		= 8;
const COLORREF RED			= RGB(200, 90, 90);
const COLORREF WHITE		= RGB(240, 240, 240);

/////////////////////////////////////////////////////////////////////////////

void AFXAPI DDX_AutoCBString(CDataExchange* pDX, int nIDC, CString& value)
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);

	if (pDX->m_bSaveAndValidate)
	{
		::DDX_CBString(pDX, nIDC, value);
	}
	else
	{
		// try exact first
		int nIndex = (int)::SendMessage(hWndCtrl, CB_FINDSTRINGEXACT, (WPARAM)-1,
										(LPARAM)(LPCTSTR)value);

		// then partial
		if (nIndex == CB_ERR)
		{
			nIndex = ::SendMessage(hWndCtrl, CB_SELECTSTRING, (WPARAM)-1, (LPARAM)(LPCTSTR)value);

			// if there's a match, check its not partial
			if (nIndex != CB_ERR)
			{
				CString sItem;
				int nLen = ::SendMessage(hWndCtrl, CB_GETLBTEXTLEN, nIndex, 0);
				::SendMessage(hWndCtrl, CB_GETLBTEXT, nIndex, (LPARAM)(LPCTSTR)sItem.GetBuffer(nLen + 1));
				sItem.ReleaseBuffer();

				if (value.CompareNoCase(sItem) != 0)
					nIndex = CB_ERR;
			}
		}

		if (nIndex == CB_ERR)
		{
			// add it and then select it if not empty
			if (!value.IsEmpty())
				nIndex = ::SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)value);

		}

		::SendMessage(hWndCtrl, CB_SETCURSEL, nIndex, 0L);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAutoComboBox

CAutoComboBox::CAutoComboBox(DWORD dwFlags) 
	: 
	m_dwFlags(dwFlags), 
	m_bClosingUp(FALSE), 
	m_bEditChange(FALSE), 
	m_nDeleteItem(LB_ERR)
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
	ON_CONTROL_REFLECT_EX(CBN_SELENDCANCEL, OnSelEndCancel)
	ON_CONTROL_REFLECT_EX(CBN_SELENDOK, OnSelEndOK)
	ON_CONTROL_REFLECT_EX(CBN_SELCHANGE, OnSelChange)
	ON_CONTROL_REFLECT_EX(CBN_CLOSEUP, OnCloseUp)
	ON_CONTROL_REFLECT_EX(CBN_DROPDOWN, OnDropDown)
	ON_CONTROL_REFLECT_EX(CBN_EDITCHANGE, OnEditChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoComboBox message handlers

void CAutoComboBox::ModifyFlags(DWORD dwRemove, DWORD dwAdd)
{
	Misc::ModifyFlags(m_dwFlags, dwRemove, dwAdd);
}

int CAutoComboBox::SetStrings(const CStringArray& aItems) 
{ 
	CComboBox::SetRedraw(FALSE);
	ResetContent(); 
	CComboBox::SetRedraw(TRUE);

	return AddUniqueItems(aItems); 
}

BOOL CAutoComboBox::OnCloseUp()
{
	CAutoFlag af(m_bClosingUp, TRUE);

	// notify parent of (possible) selection change
	if (m_bEditChange && GetCount())
		ParentCBNotify(CBN_SELCHANGE);
	else
		ParentCBNotify(CBN_SELENDCANCEL);
	
	return FALSE; // pass to parent
}

BOOL CAutoComboBox::OnDropDown()
{
// 	if (ScIsHooked())
// 		FixupListBoxPosition();

	return FALSE; // pass to parent
}

BOOL CAutoComboBox::OnEditChange()
{
	m_bEditChange = TRUE;

	return FALSE; // pass to parent
}

BOOL CAutoComboBox::OnSelEndOK()
{
	OnSelChange();
	HandleReturnKey();

	return FALSE; // continue routing
}

BOOL CAutoComboBox::OnSelEndCancel()
{
	// eat this unless we sent it explicitly
	return (!m_bClosingUp);
}

BOOL CAutoComboBox::OnSelChange()
{
	// make sure the edit control is up to date
	if (m_scEdit.IsValid())
	{
		int nSel = GetCurSel();

		if (nSel < GetCount() )
			SetWindowText(GetSelectedItem());
	}

	// eat notification if dropped down
	// ie we don't send selection notifications until the dropdown closes
	return GetDroppedState() && !IsType(CBS_SIMPLE);
}

CString CAutoComboBox::GetSelectedItem() const
{
	CString sSel;
	int nSel = GetCurSel();

	if (nSel != CB_ERR)
		GetLBText(nSel, sSel);
	
	return sSel;
}

int CAutoComboBox::FindStringExact(int nIndexStart, LPCTSTR lpszFind) const
{ 
	return FindStringExact(nIndexStart, lpszFind, FALSE); 
}

int CAutoComboBox::FindStringExact(int nIndexStart, const CString& sItem, BOOL bCaseSensitive) const
{
	if (GetCount() == 0)
		return CB_ERR;

	int nFind = nIndexStart; // default
	
	if (!sItem.IsEmpty())
	{
		// because more than one item might exist if were doing a case-sensitive
		// search we can't just stop if the first find doesn't exactly match
		// because there still may be further matches
		BOOL bContinue = TRUE;
		
		while (bContinue)
        {
			int nPrevFind = nFind;
			nFind = COwnerdrawComboBoxBase::FindStringExact(nFind, sItem);
			
			// if no match then definitely done
			if (nFind <= nPrevFind && nFind != nIndexStart)
			{
				nFind = CB_ERR;
				bContinue = FALSE;
			}
			else if (!bCaseSensitive)
			{
				bContinue = FALSE;
			}
			else
			{
				// else if (bCaseSensitive)
				ASSERT (nFind != CB_ERR);
				ASSERT (bCaseSensitive);
				
				// test for real exactness because FindStringExact is not case sensitive
				CString sFind;
				GetLBText(nFind, sFind);
				
				bContinue = !(sItem == sFind); // differ in case
			}
        }
	}
	else // special case: look for empty item
	{
		nFind = GetCount();

		while (nFind--)
		{
			CString sLBItem;
			GetLBText(nFind, sLBItem);

			if (sLBItem.IsEmpty())
				break;
		}
	}
	
	return nFind;
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
			CString sLBItem;
			GetLBText(nFind, sLBItem);

			if (nIndex == -1)
				nIndex = nFind; // leave it in it's current position
			
			if (nIndex != nFind || sItem != sLBItem)
			{
				// save selection so we can restore it
				int nSel = GetCurSel();
				CString sSelItem;
				
				if (nSel != CB_ERR)
					GetLBText(nSel, sSelItem);

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
				if (nSel != CB_ERR)
					SelectString(-1, sSelItem);
				
				return nIndex;
			}
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

void CAutoComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
								DWORD dwItemData, const CString& sItem, BOOL bList)
{
	COwnerdrawComboBoxBase::DrawItemText(dc, rect, nItem, nItemState, dwItemData, sItem, bList);

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
		
		// set the color to white-on-red if selected
		if (nItemState & ODS_SELECTED)
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
        CString sItem;
		GetLBText(nItem, sItem);

        aItems.SetAt(nItem, sItem); // maintain order
    }

    return aItems.GetSize();
}

// this handles messages destined for the dropped listbox
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

	case WM_WINDOWPOSCHANGED:
		if (lp)
		{
			WINDOWPOS* pWPos = (WINDOWPOS*)lp;
			
			if (FixupListBoxPosition(*pWPos))
				return 0;
		}
		break;
	}
	
	return CSubclasser::ScDefault(m_scList);
}

LRESULT CAutoComboBox::OnEditboxMessage(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		// <Ctrl> + <Delete>
		if (AllowDelete() && (wp == VK_DELETE) && GetDroppedState() && Misc::IsKeyPressed(VK_CONTROL))
		{
			if (DeleteLBItem(m_scList.SendMessage(LB_GETCURSEL)))
			{
				// eat message else it'll go to the edit window
				return 0L;
			}
		}
		// <Return>
		else if (wp == VK_RETURN)
		{
			if (GetDroppedState() && !IsType(CBS_SIMPLE))
				ShowDropDown(FALSE);

			HandleReturnKey();
			
			return 0L;
		}
		else if (wp == VK_DOWN && !GetDroppedState() && !IsType(CBS_SIMPLE) && GetCount())
		{
			ShowDropDown();
			return 0L; // eat
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

int CAutoComboBox::HitTestListDeleteBtn(const CPoint& ptList) const
{
	if (!IsType(CBS_DROPDOWNLIST))
	{
		int nItem = HitTestList(ptList);
		
		if ((nItem != LB_ERR) && ::SendMessage(GetListbox(), LB_GETTEXTLEN, nItem, 0))
		{
			CRect rItem, rBtn;
			
			if (LB_ERR != ::SendMessage(GetListbox(), LB_GETITEMRECT, nItem, (LPARAM)(LPRECT)rItem))
			{
				rItem.left = rItem.right - SIZE_CLOSEBTN;

				if (rItem.PtInRect(ptList))
					return nItem;
			}
		}
	}

	// else
	return LB_ERR;
}

int CAutoComboBox::HitTestList(const CPoint& ptList) const
{
	DWORD dwHit = ::SendMessage(GetListbox(), LB_ITEMFROMPOINT, 0, MAKELPARAM(ptList.x, ptList.y));

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

void CAutoComboBox::HandleReturnKey()
{
	m_bEditChange = FALSE;

	if (m_scEdit.IsValid())
	{
		CString sEdit;
		m_scEdit.GetCWnd()->GetWindowText(sEdit);
		
		int nAdd = AddUniqueItem(sEdit);
		
		if (nAdd != CB_ERR)
		{
			ParentACNotify(WM_ACBN_ITEMADDED, nAdd, sEdit);
		}

		if (!sEdit.IsEmpty())
			SelectString(-1, sEdit);
			
		// send a possible selection change
		ParentCBNotify(CBN_SELCHANGE);
	}
}

void CAutoComboBox::ParentACNotify(UINT nMsgNotify, int nIndex, LPCTSTR szItem)
{
	ASSERT((nIndex >= 0) && (!szItem || szItem[0]));

	if ((nIndex >= 0) && (!szItem || szItem[0]))
	{
		GetParent()->SendMessage(nMsgNotify, MAKEWPARAM(GetDlgCtrlID(), nIndex), (LPARAM)szItem);
	}
}

void CAutoComboBox::ParentCBNotify(UINT nIDNotify)
{
	CWnd* pParent = GetParent();

	if (pParent)
	{
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
			return ((GetStyle() & 0xf) != CBS_DROPDOWNLIST);
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
		CString sCurItem, sItem;
		int nCurSel = GetCurSel();
		
		// save existing selection
		if (nCurSel != nItem)
			GetWindowText(sCurItem);
		
		GetLBText(nItem, sItem); // need this for notifying parent
		::SendMessage(GetSafeHwnd(), CB_DELETESTRING, nItem, 0);
		
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
		if (!IsType(CBS_SIMPLE))
		{
			if (GetCount())
			{
				CRect rList;
				m_scList.GetWindowRect(rList);
				rList.bottom -= GetItemHeight(0);
				
				m_scList.GetCWnd()->MoveWindow(rList, TRUE);
			}
			else
			{
				ShowDropDown(FALSE);
			}
		}

		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CAutoComboBox::FixupListBoxPosition(const WINDOWPOS& wpos)
{
	// If the right edge of the droplist is off the edge
	// of the screen such that the close button is hidden
	// reposition the listbox aligned to the right of the
	// combobox
	if (AllowDelete())
	{
		if (!m_scList.IsValid())
		{
			ASSERT(0);
			return FALSE;
		}

		CRect rListBox(wpos.x, wpos.y, (wpos.x + wpos.cx), (wpos.y + wpos.cy));
		CRect rComboBox;
		GetWindowRect(rComboBox);

		if (rListBox.right > rComboBox.right)
		{
			CRect rDesktop;
			GraphicsMisc::GetTotalAvailableScreenSpace(rDesktop);

			if (rListBox.right > rDesktop.right)
			{
				rListBox.OffsetRect((rComboBox.right - rListBox.right), 0);
				m_scList.GetCWnd()->MoveWindow(rListBox, TRUE);
				return TRUE;
			}
		}
	}

	// not modified
	return FALSE;
}
