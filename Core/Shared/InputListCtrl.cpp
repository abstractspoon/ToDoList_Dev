// InputListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "InputListCtrl.h"
#include "themed.h"
#include "enstring.h"
#include "graphicsmisc.h"
#include "misc.h"
#include "enimagelist.h"
#include "dlgunits.h"
#include "osversion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CInputListCtrl, CEnListCtrl)

/////////////////////////////////////////////////////////////////////////////

#define MAXCOLWIDTH 600
#define MINCOLWIDTH 80
#define IDC_EDITBOX 101

/////////////////////////////////////////////////////////////////////////////

const DWORD PROMPT = 0xfefefefe;

/////////////////////////////////////////////////////////////////////////////

CInputListCtrl::CColumnData2::CColumnData2() 
	: 
	CColumnData(), 
	bEditEnabled(TRUE), 
	nType(ILCT_TEXT) 
{
}

/////////////////////////////////////////////////////////////////////////////
// CInputListCtrl

CInputListCtrl::CInputListCtrl()
{
	InitState();
}

CInputListCtrl::~CInputListCtrl()
{
}

BEGIN_MESSAGE_MAP(CInputListCtrl, CEnListCtrl)
	//{{AFX_MSG_MAP(CInputListCtrl)
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()

	ON_NOTIFY_REFLECT_EX(LVN_ITEMCHANGED, OnSelItemChanged)
	ON_NOTIFY_RANGE(NM_KILLFOCUS, 0, 0xFFFF, OnNotifyKillFocus)

	ON_REGISTERED_MESSAGE(WM_PENDEDIT, OnEditEnd)
	ON_REGISTERED_MESSAGE(WM_PCANCELEDIT, OnEditCancel)
	ON_REGISTERED_MESSAGE(WM_HTHOTCHANGE, OnHotChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputListCtrl message handlers

void CInputListCtrl::InitState()
{
	m_nEditItem = -1;
	m_nEditCol = -1;
	m_bAutoAddRows = FALSE;
	m_bAutoAddCols = FALSE;
	m_nAutoColWidth = MINCOLWIDTH;
	m_sAutoRowPrompt = "(new row)";
	m_sAutoColPrompt = "(new col)";
	m_bAllowDuplication = TRUE;
	m_bNotifyDuplicates = FALSE;
	m_nCurCol = -1;
	m_nLastEditCol = m_nLastEditRow = -1;

	// Hot tracker might have been initialised in PreSubclassWindow
	if (GetSafeHwnd() && CThemed::AreControlsThemed() && !m_hotTrack.IsInitialized())
		m_hotTrack.Initialize(this, FALSE);
}

const CInputListCtrl::CColumnData2* CInputListCtrl::GetColumnData(int nCol) const
{
	return static_cast<const CColumnData2*>(CEnListCtrl::GetColumnData(nCol)); 
}

void CInputListCtrl::HideAllControls(const CWnd* pWndIgnore) 
{ 
	HideControl(m_editBox, pWndIgnore); 
}

void CInputListCtrl::AllowDuplicates(BOOL bAllow, BOOL bNotify)
{ 
	m_bAllowDuplication = bAllow; 
	m_bNotifyDuplicates = bNotify;
}

void CInputListCtrl::SetSingleClickEditing(BOOL bEnable)
{
	m_bSingleClickEditing = bEnable;
}

void CInputListCtrl::OnLButtonDblClk(UINT /*nFlags*/, CPoint point) 
{
	// find out which column the user clicked on
	LVHITTESTINFO lvHit = { 0 };
	lvHit.pt = point;
	
	int nItem = SubItemHitTest(&lvHit);
	int nCol = lvHit.iSubItem;

	// DO NOT DO DEFAULT HANDLING 
	// because it causes flicker in combination with our owner draw
	//CEnListCtrl::OnLButtonDblClk(nFlags, point);
	if (nItem != -1)
	{
		// if we cant edit then pass on to parent but not otherwise since this would
		// cause the focus to be removed from the edit control
		if (!CanEditSelectedCell())
		{
			// dg 4/2/99
			// this produced counter-intuitive results so i'm disabling it for now
			// instead you must right-click and select 'properties'
		//	m_nmhdr.hwndFrom = m_hWnd;
		//	m_nmhdr.idFrom = GetDlgCtrlID();
		//	m_nmhdr.code = LVN_USERSELCHANGEDBLCLK;
		//	GetParent()->SendMessage(WM_NOTIFY, (WPARAM)GetDlgCtrlID(), (LPARAM)&m_nmhdr);
		}
		else if (!(IsEditing() && nItem == m_nEditItem && nCol == m_nEditCol))
		{
			// the above check deals with the situation than occurs when the user
			// double clicks on an already selected item. 
			// ie the WM_LBUTTONDOWN that preceeds WM_LBUTTONDBLCLK will already
			// have initiated an edit 
			CRect rBtn;

			EditCell(nItem, nCol, (GetButtonRect(nItem, nCol, rBtn) && rBtn.PtInRect(point)));
		}
	}
}

void CInputListCtrl::OnLButtonDown(UINT /*nFlags*/, CPoint point) 
{
	// DO NOT DO DEFAULT HANDLING 
	// because it causes flicker in combination with our owner draw
//	CEnListCtrl::OnLButtonDown(nFlags, point);

	int nSelItem, nSelCol;
	GetCurSel(nSelItem, nSelCol);

	BOOL bHadFocus = (GetFocus() == this);
	BOOL bWasEditing = IsEditing();

	LVHITTESTINFO lvHit = { 0 };
	lvHit.pt = point;
	
	int nItem = SubItemHitTest(&lvHit);
	int nCol = lvHit.iSubItem;
	
	SetFocus();

	// if we're were editing the same cell we've clicked on then
	// we may be cancelling a listbox edit so don't trigger it again
	if (bWasEditing && IsCellSelected(nItem, nCol))
		return;
	
	if (nItem != -1)
	{
		// If we are single-click editing OR a button was clicked
		// we can edit immediately regardless if the item was already selected
		CRect rBtn;
		BOOL bBtnClick = (GetButtonRect(nItem, nCol, rBtn) && rBtn.PtInRect(point));

		if (CanEditCell(nItem, nCol) && (m_bSingleClickEditing || bBtnClick))
		{
			SetCurSel(nItem, nCol, TRUE); // notifies parent
			SetItemFocus(nItem, TRUE);

			if (bBtnClick)
			{
				// Draw button pressed (unpressing handled in OnLButtonUp)
				InvalidateRect(rBtn);
				UpdateWindow();
			}

			EditCell(nItem, nCol, bBtnClick);
		}
		// else if we already had the focus and the user clicked 
		// on the selected item then we can also edit
		else if (CanEditSelectedCell() && bHadFocus && (nItem == nSelItem) && (nCol == nSelCol))
		{
			EditCell(nItem, nCol, FALSE);
		}
		else // we just scroll the clicked item into view
		{
			SetCurSel(nItem, nCol, TRUE); // notifies parent
			SetItemFocus(nItem, TRUE);
		}

		m_nCurCol = nCol;
		ScrollCellIntoView(nItem, nCol);
	}
	else
	{
		HideAllControls();
	}
}

void CInputListCtrl::OnDestroy()
{
	OnCancelEdit();

	CEnListCtrl::OnDestroy();
}

void CInputListCtrl::EditCell(int nItem, int nCol, BOOL bBtnClick)
{
	CString sText;
	CRect rEdit;
	int nLastRow, nLastCol;

	// if this is the right column, scroll item fully into view
	// begin editing and move the 
	// edit control to the correct column and fill edit control
	// with the right text
	if (nCol != -1 && nItem != -1 && !IsReadOnly())
	{
		// dont edit if editing is disabled
		if (!IsColumnEditingEnabled(nCol))
			return;

		// dont edit if both autorowadding and autocoladding are enabled and we're in
		// pos (0, 0)
		if (m_bAutoAddRows && m_bAutoAddCols && nItem == 0 && nCol == 0)
			return;

		// dont edit if both autorowadding and autocoladding are enabled and we're in
		// other than col0 for row adding or row0 for col adding
		nLastRow = GetItemCount() - 1;
		nLastCol = GetColumnCount() - 1;

		if ((m_bAutoAddRows && nItem == nLastRow && nCol != 0) ||
			(m_bAutoAddCols && nCol == nLastCol && nItem != 0))
			return;

		if (GetColumnType(nCol) == ILCT_CHECK)
		{
			// just invert the current state
			sText = GetItemText(nItem, nCol);

			SetItemText(nItem, nCol, (sText.IsEmpty() ? _T("+") : _T("")));
			Invalidate(FALSE);
		}
		else
		{
			m_nEditItem = nItem;
			m_nEditCol = nCol;
			GetEditControl()->Reset();

			// position edit box
			ScrollCellIntoView(nItem, nCol);
			GetCellEditRect(nItem, nCol, rEdit);

			// set text and show
			// if its a new row or column then clear the prompt text
			if (IsPrompt(nItem, nCol))
				sText.Empty();
			else
				sText = GetItemText(nItem, nCol);

			GetEditControl()->SetWindowText(sText);

			if (GetEditControl()->GetStyle() & WS_POPUP)
				ClientToScreen(rEdit);
			
			GetEditControl()->Show(rEdit, FALSE);
		}
	}
}

LRESULT CInputListCtrl::OnEditEnd(WPARAM wParam, LPARAM lParam)
{
	int nResult;
	BOOL bReturnSelected = (BOOL)lParam;

	// wParam holds the id of the ctrl which sent the message
	OnEndEdit((UINT)wParam, &nResult);

	// end can occurs either when the user selected return
	// or if the edit box loses the focus so we need to check
	// the lParam and only set the focus if the user chose return
	if (bReturnSelected)
		SetFocus();	

	return 0L;
}

BOOL CInputListCtrl::IsDuplicateRow(CString sRow, int nRowToIgnore) const
{
	int nNumRows;

	nNumRows = GetItemCount();

	while (nNumRows--)
	{
		if (sRow.CompareNoCase(GetItemText(nNumRows, 0)) == 0 && nNumRows != nRowToIgnore)
			return TRUE;
	}

	return FALSE;
}

BOOL CInputListCtrl::IsDuplicateCol(CString sCol, int nColToIgnore) const
{
	int nNumCols;

	nNumCols = GetColumnCount();

	while (nNumCols--)
	{
		if (sCol.CompareNoCase(GetItemText(0, nNumCols)) == 0 && nNumCols != nColToIgnore)
			return TRUE;
	}

	return FALSE;
}

LRESULT CInputListCtrl::OnEditCancel(WPARAM /*wParam*/, LPARAM lParam)
{
	BOOL bEscapeSelected = (BOOL)lParam;

	OnCancelEdit();

	// cancel can occurs either when the user selected escape
	// or if the edit box loses the focus so we need to check
	// the lParam and only set the focus if the user chose escape
	if (bEscapeSelected)
		SetFocus();	

	return 0L;
}

void CInputListCtrl::EnableColumnEditing(int nCol, BOOL bEnable)
{
	// Adds a new item if necessary
	CColumnData2* pData = (CColumnData2*)CreateColumnData(nCol);

	if (pData)
		pData->bEditEnabled = bEnable;
}

BOOL CInputListCtrl::IsColumnEditingEnabled(int nCol) const
{
	const CColumnData2* pData = GetColumnData(nCol);

	return (pData ? pData->bEditEnabled : TRUE);
}

void CInputListCtrl::AutoAdd(BOOL bRows, BOOL bCols)
{
	ASSERT (m_hWnd);

	if (!m_hWnd)
		return;

	// rows
	if (m_bAutoAddRows && !bRows)
	{
		m_bAutoAddRows = FALSE;

		// delete last row
		DeleteItem(GetItemCount() - 1);
	}
	else if (!m_bAutoAddRows && bRows)
	{
		m_bAutoAddRows = TRUE;

		// ensure there is at least one column
		if (GetColumnCount() == 0)
		{
			InsertColumn(0, _T(""), LVCFMT_LEFT, m_nAutoColWidth);
		}
		else if (bCols)
		{
			SetColumnWidth(0, m_nAutoColWidth);
		}

		int nIndex = InsertItem(GetItemCount(), m_sAutoRowPrompt, -1);
		SetItemData(nIndex, PROMPT);
	}

	// cols
	if (m_bAutoAddCols && !bCols)
	{
		m_bAutoAddCols = FALSE;
	}
	else if (!m_bAutoAddCols && bCols)
	{
		m_bAutoAddCols = TRUE;

		InsertColumn(GetColumnCount(), _T(""), LVCFMT_LEFT, m_nAutoColWidth);
		InsertItem(0, _T(""));
		SetItemText(0, GetColumnCount() - 1, m_sAutoColPrompt);
	}

	m_nCurCol = 0;
	Invalidate();
}

void CInputListCtrl::SetAutoColumnWidth(int nWidth)
{
	// don't allow outside than min/max sizes
	m_nAutoColWidth = min(MAXCOLWIDTH, max(nWidth, MINCOLWIDTH));
}

void CInputListCtrl::SetAutoRowPrompt(const CString& sPrompt)
{
	// can't set to empty string
	if (!sPrompt.IsEmpty())
		m_sAutoRowPrompt = sPrompt;
}

void CInputListCtrl::SetAutoColPrompt(const CString& sPrompt)
{
	// can't set to empty string
	if (!sPrompt.IsEmpty())
		m_sAutoColPrompt = sPrompt;
}

void CInputListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (IsEditing())
		return;
	
	int nOldCol, nOldItem, nCol = m_nCurCol;
	CRect rItem;
	int nItem;

	// save current selection
	GetCurSel(nOldItem, nOldCol);

	CEnListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
	
	nItem = GetFocusedItem();
	
	switch (nChar)
	{
	case VK_LEFT:
		if (nCol > 0)
			ScrollCellIntoView(nItem, --nCol);
		break;

	case VK_RIGHT:
		if (nCol < GetColumnCount() - 1)
			ScrollCellIntoView(nItem, ++nCol);
		break;

	case VK_DELETE:
		if (CanDeleteSelectedCell())
			DeleteSelectedCell();
		break;

	case VK_F2:
	case VK_SPACE:
	case VK_RETURN:
		if (CanEditSelectedCell())
			EditCell(nItem, nCol, FALSE);
		break;
	}

	// update list selection notify parent
	SetCurSel(nItem, nCol, TRUE);
}

void CInputListCtrl::DrawItemBackground(CDC* /*pDC*/, int /*nItem*/, const CRect& /*rItem*/, COLORREF /*crBack*/, 
										BOOL /*bSelected*/, BOOL /*bDropHighlighted*/, BOOL /*bFocused*/)
{
	// Do nothing - handled in DrawCellBackground
}

BOOL CInputListCtrl::IsCellSelected(int nRow, int nCol, BOOL bVisually) const
{
	if (nRow != GetCurSel())
		return FALSE;

	if (nCol != m_nCurCol)
		return FALSE;

	if (bVisually && (IsEditing() || IsChild(GetFocus())))
		return FALSE;

	return TRUE;
}

void CInputListCtrl::DrawCellBackground(CDC* pDC, int nItem, int nCol, const CRect& rCell, 
										BOOL bSelected, BOOL bDropHighlighted, BOOL bFocused)
{
	bSelected &= IsCellSelected(nItem, nCol, TRUE);
	COLORREF crBack = GetItemBackColor(nItem, nCol, bSelected, FALSE, bFocused);
	
	CEnListCtrl::DrawItemBackground(pDC, nItem, rCell, crBack, bSelected, bDropHighlighted, bFocused);
}

void CInputListCtrl::DrawCell(CDC* pDC, int nItem, int nCol, 
							  const CRect& rCell, const CString& sText, 
							  BOOL bSelected, BOOL bDropHighlighted, BOOL bFocused)
{
	bSelected &= IsCellSelected(nItem, nCol, TRUE);

	CRect rText(rCell);

	if (CellHasButton(nItem, nCol))
	{
		IL_COLUMNTYPE nBtnType = GetCellType(nItem, nCol);
		CRect rBtn(0, 0, 0, 0);

		if (nBtnType == ILCT_CHECK) // Special case
		{
			// Draw checkbox over background
			CEnListCtrl::DrawCell(pDC, nItem, nCol, rCell, sText, bSelected, bDropHighlighted, bFocused);

			VERIFY(DrawButton(pDC, nItem, nCol, sText, bSelected, rBtn));
			return;
		}

		// all other button types
		VERIFY(DrawButton(pDC, nItem, nCol, sText, bSelected, rBtn));

		// Exclude button from text rect
		if (rBtn.left <= rCell.left)
		{
			rText.left = rBtn.right;
		}
		else if (rBtn.right >= rCell.right)
		{
			rText.right = rBtn.left;
		}
	}
			
	CEnListCtrl::DrawCell(pDC, nItem, nCol, rText, sText, bSelected, bDropHighlighted, bFocused);
}

void CInputListCtrl::DrawCellText(CDC* pDC, int nRow, int nCol, const CRect& rText, 
								  const CString& sText, COLORREF crText, UINT nDrawTextFlags)
{
	if (sText.IsEmpty())
		return;

	if (!IsWindowEnabled() && IsPrompt(nRow))
		return;

	if (GetCellType(nRow, nCol) == ILCT_CHECK)
		return;

	// else
	CEnListCtrl::DrawCellText(pDC, nRow, nCol, rText, sText, crText, nDrawTextFlags);
}

UINT CInputListCtrl::GetTextDrawFlags(int nCol) const
{
	return (CEnListCtrl::GetTextDrawFlags(nCol) & ~DT_END_ELLIPSIS);
}

IL_COLUMNTYPE CInputListCtrl::GetCellType(int /*nRow*/, int nCol) const
{
	return GetColumnType(nCol);
}

DWORD CInputListCtrl::GetButtonState(int nRow, int nCol, BOOL bSelected) const
{
	ASSERT(CellHasButton(nRow, nCol));

	if (!IsButtonEnabled(nRow, nCol))
		return DFCS_INACTIVE;

	if ((bSelected && (::GetFocus() == *this)) || IsButtonHot(nRow, nCol))
		return DFCS_HOT;

	return 0;
}

BOOL CInputListCtrl::DrawButton(CDC* pDC, int nRow, int nCol, const CString& sText, BOOL bSelected, CRect& rBtn)
{
	if (rBtn.IsRectEmpty() && !GetButtonRect(nRow, nCol, rBtn))
		return FALSE;

	IL_COLUMNTYPE nType = GetCellType(nRow, nCol);
	DWORD dwState = GetButtonState(nRow, nCol, bSelected);

	if ((nType == ILCT_CHECK) && !sText.IsEmpty())
		dwState |= DFCS_CHECKED;

	return DrawButton(pDC, rBtn, nType, dwState);
}

BOOL CInputListCtrl::DrawButton(CDC* pDC, const CRect& rBtn, IL_COLUMNTYPE nType, DWORD dwState) const
{
	switch (nType)
	{
	case ILCT_DATE:			DrawDateButton(pDC, rBtn, dwState);		return TRUE;
	case ILCT_COMBO:		DrawComboButton(pDC, rBtn, dwState);	return TRUE;
	case ILCT_ICON:			DrawBlankButton(pDC, rBtn, dwState);	return TRUE;
	case ILCT_POPUPMENU:	DrawMenuButton(pDC, rBtn, dwState);		return TRUE;
	case ILCT_BROWSE:		DrawBrowseButton(pDC, rBtn, dwState);	return TRUE;
	case ILCT_CHECK:		DrawCheckBoxButton(pDC, rBtn, dwState);	return TRUE;
	}

	ASSERT(0);
	return FALSE;
}

void CInputListCtrl::CheckApplyPushedState(const CRect& rBtn, DWORD& dwState) const
{
	if (!Misc::HasFlag(dwState, DFCS_PUSHED) && Misc::IsKeyPressed(VK_LBUTTON))
	{
		CPoint pt(GetMessagePos());
		ScreenToClient(&pt);

		if (rBtn.PtInRect(pt))
		{
			// Replace 'hot' with 'pushed'
			Misc::SetFlag(dwState, DFCS_PUSHED, TRUE);
			Misc::SetFlag(dwState, DFCS_HOT, FALSE);
		}
	}
}

void CInputListCtrl::DrawBlankButton(CDC* pDC, const CRect& rBtn, DWORD dwState) const
{
	CheckApplyPushedState(rBtn, dwState);

	CThemed::DrawFrameControl(this, pDC, rBtn, DFC_COMBONOARROW, dwState);
}

void CInputListCtrl::DrawIconButton(CDC* pDC, const CRect& rBtn, HICON hIcon, DWORD dwState) const
{
	DrawBlankButton(pDC, rBtn, dwState);

	GraphicsMisc::DrawCentred(pDC, hIcon, rBtn);
}

void CInputListCtrl::DrawDateButton(CDC* pDC, const CRect& rBtn, DWORD dwState) const
{
	if (CThemed::AreControlsThemed() && (COSVersion() >= OSV_WIN7))
	{
		// Draw underlying button
		DrawBlankButton(pDC, rBtn, dwState);

		// Draw date drop arrow
		CThemed th;
		th.Open(this, _T("DATEPICKER"));

		BOOL bEnabled = !Misc::HasFlag(dwState, DFCS_INACTIVE);
		th.DrawBackground(pDC, DP_SHOWCALENDARBUTTONRIGHT, (bEnabled ? DPSCBR_NORMAL : DPSCBR_DISABLED), rBtn);
	}
	else
	{
		CheckApplyPushedState(rBtn, dwState);

		CThemed::DrawFrameControl(this, pDC, rBtn, DFC_COMBO, dwState);
	}
}

void CInputListCtrl::DrawMenuButton(CDC* pDC, const CRect& rBtn, DWORD dwState) const
{
	DrawBlankButton(pDC, rBtn, dwState);

	// Draw arrow
	BOOL bEnabled = !Misc::HasFlag(dwState, DFCS_INACTIVE);
	pDC->SetTextColor(GetSysColor(bEnabled ? COLOR_BTNTEXT : COLOR_GRAYTEXT));

	UINT nFlags = DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_CENTER;
	GraphicsMisc::DrawAnsiSymbol(pDC, MARLETT_MENUDOWN, rBtn, nFlags, &GraphicsMisc::Marlett());
}

void CInputListCtrl::DrawBrowseButton(CDC* pDC, const CRect& rBtn, DWORD dwState) const
{
	DrawBlankButton(pDC, rBtn, dwState);

	// Draw ellipsis
	CRect rText(rBtn);

	rText.left += (rText.Width() % 2);
	rText.top += (rText.Height() % 2);

	BOOL bEnabled = !Misc::HasFlag(dwState, DFCS_INACTIVE);

	pDC->SetTextColor(GetSysColor(bEnabled ? COLOR_BTNTEXT : COLOR_GRAYTEXT));
	pDC->DrawText(_T("..."), rText, DT_CENTER | DT_VCENTER);
}

void CInputListCtrl::DrawComboButton(CDC* pDC, const CRect& rBtn, DWORD dwState) const
{
	CheckApplyPushedState(rBtn, dwState);

	CThemed::DrawFrameControl(this, pDC, rBtn, DFC_COMBO, dwState);
}

void CInputListCtrl::DrawCheckBoxButton(CDC* pDC, const CRect& rBtn, DWORD dwState) const
{
	CheckApplyPushedState(rBtn, dwState);

	CThemed::DrawFrameControl(this, pDC, rBtn, DFC_BUTTON, (DFCS_BUTTONCHECK | dwState));
}

BOOL CInputListCtrl::CellHasButton(int nRow, int nCol) const
{
	IL_COLUMNTYPE nType = GetCellType(nRow, nCol);
	
	switch (nType)
	{
	case ILCT_TEXT:
		return FALSE;

	case ILCT_CHECK:
		return TRUE;
	}

	// All else
	return CanEditCell(nRow, nCol);
}

BOOL CInputListCtrl::GetButtonRect(int nRow, int nCol, CRect& rBtn) const
{
	rBtn.SetRectEmpty();

	if (!CellHasButton(nRow, nCol))
		return FALSE;

	IL_COLUMNTYPE nType = GetCellType(nRow, nCol);
	GetCellRect(nRow, nCol, rBtn);

	switch (nType)
	{
	case ILCT_BROWSE:
	case ILCT_POPUPMENU:
	case ILCT_COMBO:
		rBtn.left = (rBtn.right - EE_BTNWIDTH_DEFAULT);
		break;

	case ILCT_ICON:
		rBtn.left = (rBtn.right - EE_BTNWIDTH_ICON);
		break;

	case ILCT_DATE:
		rBtn.left = (rBtn.right - EE_BTNWIDTH_DEFAULT);

		// Only Win7 and above supports the fancy new date dropdown icon
		if ((COSVersion() >= OSV_WIN7) && CThemed::AreControlsThemed())
			rBtn.left -= EE_BTNWIDTH_DEFAULT;
		break;

	case ILCT_CHECK:
		rBtn.left += ((rBtn.Width() - EE_BTNWIDTH_DEFAULT) / 2);
		rBtn.right = (rBtn.left + EE_BTNWIDTH_DEFAULT);
		break;

	default:
		ASSERT(0);
		return FALSE;
	}

	return TRUE;
}

BOOL CInputListCtrl::CanDeleteSelectedCell() const
{
	return (CanEditSelectedCell() && !IsPrompt(GetCurSel(), m_nCurCol));
}

BOOL CInputListCtrl::CanDeleteCell(int nRow, int nCol) const
{
	return (CanEditCell(nRow, nCol) && !IsPrompt(nRow, nCol));
}

BOOL CInputListCtrl::DeleteSelectedCell()
{
	if (GetCurSel() != - 1)
	{
		// don't delete it if its the topleft item
		if (m_bAutoAddCols && m_bAutoAddRows && IsCellSelected(0, 0))
		{
			return FALSE;
		}
		// else delete the cell if its not the 'add row' prompt
		else if (m_bAutoAddRows && m_nCurCol == 0)
		{
			if (GetCurSel() < GetItemCount() - 1)
			{
				DeleteItem(GetCurSel());
				RecalcHotButtonRects();
			}
			else
			{
				return FALSE;
			}
		}
		// else delete the cell if its not the 'add col' prompt
		else if (m_bAutoAddCols && GetCurSel() == 0)
		{
			if (m_nCurCol < GetColumnCount() - 1)
			{
				DeleteColumn(m_nCurCol);
				RecalcHotButtonRects();

				m_nCurCol = -1;
			}
			else
			{
				return FALSE;
			}
		}
		else // clear the field
		{
			SetItemText(GetCurSel(), m_nCurCol, _T(""));
		}

		return TRUE;
	}
	// else do nothing
	return FALSE;
}

BOOL CInputListCtrl::IsEditing() const 
{ 
	return m_editBox.GetSafeHwnd() && m_editBox.IsWindowVisible(); 
}

BOOL CInputListCtrl::IsButtonEnabled(int nRow, int nCol) const
{
	return CanEditCell(nRow, nCol);
}

BOOL CInputListCtrl::CanEditCell(int nRow, int nCol) const
{
	if (nRow == -1 || nCol == -1)
		return FALSE;

	// if readonly or disabled then no
	if (IsReadOnly() || !IsWindowEnabled())
		return FALSE;

	// don't edit it:
	//
	// if editing is disabled in the current column
	if (!IsColumnEditingEnabled(nCol))
		return FALSE;
	
	// or if its the last row and not the first column when autoadding rows
	int nNumRows = GetItemCount();
	int nNumCols = GetColumnCount();
	
	if (m_bAutoAddRows && (nRow == nNumRows - 1) && nCol != 0)
		return FALSE;
	
	// or if its the last col and not the first row when autoadding cols
	if (m_bAutoAddCols && (nCol == nNumCols - 1) && nRow != 0)
		return FALSE;
	
	// or if its the topleft item when auto adding rows AND cols
	if (m_bAutoAddCols && m_bAutoAddRows && nRow == 0 && nCol == 0)
		return FALSE;
	
	// of if its the bottom right item when auto adding rows AND cols
	if (m_bAutoAddCols && m_bAutoAddRows && (nRow == nNumRows - 1) && (nCol == nNumCols - 1))
		return FALSE;
	
	// go ahead
	return TRUE;
}

BOOL CInputListCtrl::CanEditSelectedCell() const
{
	int nSelRow, nSelCol;

	if (!GetCurSel(nSelRow, nSelCol))
		return FALSE;

	return CanEditCell(nSelRow, nSelCol);
}

void CInputListCtrl::EditSelectedCell()
{
	if (CanEditSelectedCell())
		EditCell(GetCurSel(), m_nCurCol, FALSE);
}

void CInputListCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	// ignore if WE are pNewWnd
	if (pNewWnd == this)
		return;

	if (IsEditing())
		HideAllControls(pNewWnd);

	CRect rItem;
	GetItemRect(GetCurSel(), rItem, LVIR_BOUNDS);
	InvalidateRect(rItem, FALSE);

	CEnListCtrl::OnKillFocus(pNewWnd);
}

BOOL CInputListCtrl::OnSelItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (IsSelectionChange((NMLISTVIEW*)pNMHDR))
		HideAllControls();

	return FALSE; // continue routing
}

void CInputListCtrl::OnNotifyKillFocus(UINT nCtrlID, NMHDR* pNMHDR, LRESULT* pResult)
{
	CWnd* pCtrl = GetDlgItem(nCtrlID);

	if (pCtrl)
		HideControl(*pCtrl);
}

BOOL CInputListCtrl::SetCellText(int nRow, int nCol, const CString& sText)
{
	ASSERT (m_hWnd);

	// only allow text setting if within valid range and if user is
	// not trying to change prompt string if auto adding
	if (((m_bAutoAddRows && (nRow >= 0) && (nRow < GetItemCount() - 1)) ||
		(!m_bAutoAddRows && (nRow >= 0) && (nRow < GetItemCount()))) &&
		((m_bAutoAddCols && (nCol >= 0) && (nCol < GetColumnCount() - 1)) ||
		(!m_bAutoAddCols && nCol >= 0 && (nCol < GetColumnCount()))))
	{
		SetItemText(nRow, nCol, sText);
		return TRUE;
	}

	ASSERT(0);
	return FALSE;
}

BOOL CInputListCtrl::DeleteAllItems(BOOL bIncludeCols)
{
	// delete all 
	m_hotTrack.DeleteAllRects();

	BOOL bRes = CEnListCtrl::DeleteAllItems();

	// delete columns if necessary
	if (bIncludeCols)
	{
		while (DeleteColumn(0));
	}

	if (!bRes || (!m_bAutoAddRows && !m_bAutoAddCols))
		return bRes;

	// if auto adding re-add prompts as appropriate
	int nNumCols = GetColumnCount();

	// rows
	if (m_bAutoAddRows)
	{
		// ensure there is at least one column
		if (nNumCols == 0)
			InsertColumn(0, _T(""), LVCFMT_LEFT, m_nAutoColWidth);
		
		int nIndex = InsertItem(0, m_sAutoRowPrompt, -1);
		SetItemData(nIndex, PROMPT); 
	}

	// cols 
	nNumCols = GetColumnCount();

	if (m_bAutoAddCols)
	{
		// however, ensure there is at least one column
		if (nNumCols == 0 || (m_bAutoAddRows && nNumCols == 1))
		{
			InsertColumn(nNumCols, _T(""), LVCFMT_LEFT, m_nAutoColWidth);
			nNumCols++;
		}
		
		InsertItem(0, _T(""));
		SetItemText(0, nNumCols - 1, m_sAutoColPrompt);
	}

	m_nCurCol = 0;
	Invalidate();

	// cols 
	nNumCols = GetColumnCount();

	return TRUE;
}

BOOL CInputListCtrl::DeleteItem(int nItem)
{
	if (CEnListCtrl::DeleteItem(nItem))
	{
		RecalcHotButtonRects();
		return TRUE;
	}

	return FALSE;
}

void CInputListCtrl::SetCurSel(int nRow, int nCol, BOOL bNotifyParent)
{
	ASSERT (m_hWnd);
	ASSERT (nRow >= -1 && nRow < GetItemCount() && 
			nCol >= -1 && nCol < GetColumnCount());

	// only allow selection setting if within valid range 
	if (nRow < 0 || nRow >= GetItemCount() || nCol < 0 || nCol >= GetColumnCount())
		return;

	// don't update if nothing has changed
	if (IsCellSelected(nRow, nCol))
		return;

	// Redraw current selection
	RedrawItems(GetCurSel(), m_nCurCol);

	m_nCurCol = nCol;
	CEnListCtrl::SetCurSel(nRow, bNotifyParent == TRUE);

	// Redraw new selection
	RedrawItems(GetCurSel(), m_nCurCol);
}

int CInputListCtrl::GetLastEdit(int* pRow, int* pCol)
{
	if (pRow)
		*pRow = m_nLastEditRow;

	if (pCol)
		*pCol = m_nLastEditCol;

	return m_nLastEditResult;
}

BOOL CInputListCtrl::GetCurSel(int& nRow, int& nCol) const
{
	nRow = GetCurSel();
	nCol = m_nCurCol;

	return ((nRow != -1) && (nCol != -1));
}

int CInputListCtrl::InsertRow(const CString& sRowText, int nRow, int nImage)
{
	if (m_bAutoAddRows && nRow == GetItemCount())
		nRow--; // add before prompt

	nRow = InsertItem(nRow, sRowText, nImage);

	RecalcHotButtonRects();

	return nRow;
}

BOOL CInputListCtrl::HasNonTextCells() const
{
	int nCol = GetColumnCount();

	while (nCol--)
	{
		if (GetColumnType(nCol) != ILCT_TEXT)
			return TRUE;

		// Test all cells in this column
		int nRow = GetItemCount();

		while (nRow--)
		{
			if (GetCellType(nRow, nCol) != ILCT_TEXT)
				return TRUE;
		}
	}

	return FALSE;
}

int CInputListCtrl::AddRow(const CString& sRowText, int nImage)
{
	return InsertRow(sRowText, GetItemCount(), nImage);
}

int CInputListCtrl::AddCol(const CString& sColText, int nWidth, IL_COLUMNTYPE nColType)
{
	int nCol = GetColumnCount();

	if (m_bAutoAddCols)
		nCol--; // add before prompt

	nCol = InsertColumn(nCol, sColText, LVCFMT_LEFT, nWidth == -1 ? m_nAutoColWidth : nWidth);

	if (!sColText.IsEmpty())
		SetItemText(0, nCol, sColText);

	SetColumnType(nCol, nColType);

	if (m_nCurCol == -1)
		m_nCurCol = nCol;

	return nCol;
}

void CInputListCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CEnListCtrl::OnSetFocus(pOldWnd);

	if (GetCurSel() == -1)
	{
		if (GetItemCount())
		{
			SetCurSel(0, 0);
			SetItemFocus(0, TRUE); // set focus
		}
	}
	else
		SetItemFocus(GetCurSel(), TRUE); // reset focus
}

void CInputListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	OnCancelEdit();

	CEnListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);

	RecalcHotButtonRects();
}

void CInputListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (!pScrollBar)
		OnCancelEdit();

	CEnListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);

	if (!pScrollBar)
		RecalcHotButtonRects();
}

BOOL CInputListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	OnCancelEdit();

	BOOL bRes = CEnListCtrl::OnMouseWheel(nFlags, zDelta, pt);

	RecalcHotButtonRects();
	
	return bRes;
}

void CInputListCtrl::OnSize(UINT nType, int cx, int cy)
{
	OnCancelEdit();

	CEnListCtrl::OnSize(nType, cx, cy);

	RecalcHotButtonRects();
}

CRect CInputListCtrl::ScrollCellIntoView(int nRow, int nCol)
{
	CRect rClient, rIntersect, rCell;
	CPoint ptItem;
	CSize sizeScroll;
//	int nColStart, nColEnd;

	// do nothing if invalid coords
	if (nRow == -1 || nCol == -1)
		return CRect(0, 0, 0, 0);

	// note: amount to scroll is relative to current position
	// so we need to determine whether the entire cell is visible and
	// if not, how much to scroll to bring it into view.
	GetClientRect(rClient);
	GetCellRect(nRow, nCol, rCell);

	// if the cell is completely contained within the client area
	// we don't need to scroll
	rIntersect.IntersectRect(rClient, rCell);

	if (rIntersect != rCell)
	{
		// calc how much to scroll
		if (rCell.left <= 0)
		{
			sizeScroll.cx = rCell.left;
		}
		else if (rCell.right >= rClient.right)
		{
			sizeScroll.cx = min(rCell.left, rCell.right - rClient.right);
		}
		else 
			sizeScroll.cx = 0;

		if (rCell.top < 0)
		{
			sizeScroll.cy = rCell.top;
		}
		else if (rCell.bottom > rClient.bottom)
		{
			sizeScroll.cy = rCell.bottom - rClient.bottom;
		}
		else 
			sizeScroll.cy = 0;

		// now, listctrls only scroll by whole lines so..
		if (sizeScroll.cy > 0 && sizeScroll.cy % rCell.Height() != 0)
			sizeScroll.cy += rCell.Height();

		// do the scroll
		if (sizeScroll != CSize(0, 0))
			Scroll(sizeScroll);

		// calc final cell position
		GetCellRect(nRow, nCol, rCell);
	}

	return rCell;
}

void CInputListCtrl::GetCellEditRect(int nRow, int nCol, CRect& rCell) const
{
	CEnListCtrl::GetCellEditRect(nRow, nCol, rCell);
}

void CInputListCtrl::SetView(int nView)
{
	// this control only supports LVS_REPORT view
	ASSERT (nView == LVS_REPORT);
	UNREFERENCED_PARAMETER(nView);

	CEnListCtrl::SetView(LVS_REPORT);
}

void CInputListCtrl::PreSubclassWindow()
{
	CEnListCtrl::PreSubclassWindow();
	
	// make sure we clip out the edit field
	ModifyStyle(0, WS_CLIPCHILDREN);

	InitState();
}

void CInputListCtrl::HideControl(CWnd& ctrl, const CWnd* pWndIgnore)
{
	if (pWndIgnore == this)
		return;

	if (pWndIgnore == &ctrl)
		return;

	if (!ctrl.GetSafeHwnd())
		return;
	
	if (pWndIgnore && ctrl.IsKindOf(RUNTIME_CLASS(CDateTimeCtrl)))
	{
		if (pWndIgnore->GetSafeHwnd() == (HWND)ctrl.SendMessage(DTM_GETMONTHCAL))
			return;
	}

	ctrl.ShowWindow(SW_HIDE);
	ctrl.EnableWindow(FALSE);
}

void CInputListCtrl::ShowControl(CWnd& ctrl, int nRow, int nCol, BOOL bBtnClick)
{
	PrepareControl(ctrl, nRow, nCol);
	ScrollCellIntoView(nRow, nCol);

	CRect rCell;
	GetCellEditRect(nRow, nCol, rCell);

	if (ctrl.IsKindOf(RUNTIME_CLASS(CComboBox)))
	{
		if (nRow == 0)
			rCell.top--;

		rCell.bottom += GraphicsMisc::ScaleByDPIFactor(200);
	}

	ctrl.MoveWindow(rCell);
	ctrl.EnableWindow(TRUE);
	ctrl.ShowWindow(SW_SHOW);
	ctrl.SetFocus();

	if (ctrl.IsKindOf(RUNTIME_CLASS(CComboBox)))
	{
		CEdit* pEdit = (CEdit*)ctrl.GetDlgItem(1001);

		if (pEdit)
		{
			pEdit->ShowWindow(SW_SHOW);
			pEdit->SetSel(0, -1);
		}

		if (bBtnClick || !pEdit)
			ctrl.SendMessage(CB_SHOWDROPDOWN, TRUE);
	}
	else if (ctrl.IsKindOf(RUNTIME_CLASS(CEdit)))
	{
		((CEdit&)ctrl).SetSel(0, -1);
	}
	else if (ctrl.IsKindOf(RUNTIME_CLASS(CDateTimeCtrl)))
	{
		if (bBtnClick)
			ctrl.PostMessage(WM_SYSKEYDOWN, VK_DOWN, 0);
	}
}

void CInputListCtrl::RedrawCell(int nRow, int nCol, BOOL bErase)
{
	CRect rCell;
	GetCellRect(nRow, nCol, rCell);

	InvalidateRect(rCell, bErase);
}

void CInputListCtrl::EndEdit()
{
	int nRow, nCol;
	GetCurSel(nRow, nCol);

	if (nRow == -1 || nCol == -1)
		return;

	// if any editing control is visible, just shift the
	// focus back to the list to end the edit
	if (IsEditing())
		SetFocus();
}

BOOL CInputListCtrl::CreateControl(CComboBox& ctrl, UINT nID, DWORD dwComboStyles)
{
	dwComboStyles |= (WS_CHILD | WS_VSCROLL);

	if ((dwComboStyles & 0xf) == 0)
		dwComboStyles |= CBS_DROPDOWNLIST;

	if (!ctrl.Create(dwComboStyles, CRect(0, 0, 0, 0), this, nID))
	{
		ASSERT(0);
		return FALSE;
	}

	// else
	PostCreateControl(ctrl);
	return TRUE;
}

BOOL CInputListCtrl::CreateControl(CEdit& ctrl, UINT nID, DWORD dwEditStyles)
{
	dwEditStyles |= WS_CHILD;

	if (!ctrl.Create(dwEditStyles, CRect(0, 0, 0, 0), this, nID))
	{
		ASSERT(0);
		return FALSE;
	}

	ctrl.ModifyStyleEx(0, WS_EX_CLIENTEDGE, 0);
	PostCreateControl(ctrl);

	return TRUE;
}

BOOL CInputListCtrl::CreateControl(CDateTimeCtrl& ctrl, UINT nID, DWORD dwDateTimeStyles)
{
	dwDateTimeStyles |= WS_CHILD;

	if (!ctrl.Create(dwDateTimeStyles, CRect(0, 0, 0, 0), this, nID))
	{
		ASSERT(0);
		return FALSE;
	}

	// else
	PostCreateControl(ctrl);
	return TRUE;
}

void CInputListCtrl::PostCreateControl(CWnd& ctrl)
{
	ctrl.SetFont(GetFont()); // set font to parent's
	ctrl.ShowWindow(SW_HIDE);

	CRect rWnd;
	ctrl.GetWindowRect(rWnd);

	if (ctrl.IsKindOf(RUNTIME_CLASS(CComboBox)) ||
		ctrl.IsKindOf(RUNTIME_CLASS(CDateTimeCtrl)))
	{
		// Reduce a bit else the row height can seem excessive
 		rWnd.bottom -= 2;
	}

	SetMinItemHeight(max(GetMinItemHeight(), rWnd.Height()));
}

CPopupEditCtrl* CInputListCtrl::GetEditControl()
{
	ASSERT (m_hWnd);

	if (!m_hWnd)
		return NULL;

	if (!m_editBox.m_hWnd)
		VERIFY(CheckRecreateEditControl(FALSE));

	return &m_editBox;
}

BOOL CInputListCtrl::CheckRecreateEditControl(BOOL bMultiline)
{
	ASSERT (m_hWnd);

	if (m_hWnd)
	{
		// Seems the only way to change the ES_MULTILINE style
		// is to recreate the edit box
		if (m_editBox.m_hWnd)
		{
			BOOL bIsMultiline = (m_editBox.GetStyle() & ES_MULTILINE);

			if (Misc::StatesDiffer(bMultiline, bIsMultiline))
				m_editBox.DestroyWindow();
		}

		if (!m_editBox.m_hWnd)
		{
			// Note: The default client border (WS_BORDER) is generally
			// preferred because it is more distinctive but in the case
			// of multiline we use a non-client border (WS_EX_CLIENTEDGE)
			// because otherwise the vertical scrollbar appears detached
			UINT dwStyle = (WS_CHILD | ES_LEFT);

			if (bMultiline)
			{
				m_editBox.Create(this, IDC_EDITBOX, (dwStyle | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL));
				m_editBox.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
			}
			else
			{
				m_editBox.Create(this, IDC_EDITBOX, (dwStyle | WS_BORDER | ES_AUTOHSCROLL));
			}
		}
	}

	return (!m_editBox.GetSafeHwnd() == NULL);
}

void CInputListCtrl::OnEndEdit(UINT /*uIDCtrl*/, int* pResult)
{
	CString sText, sMessage;
	int nLastRow, nLastCol, nIndex, nNumCols, nNumRows;
	BOOL bNotifyParent = FALSE;
	BOOL bItemDeleted = FALSE;

	nLastRow = GetItemCount() - 1;
	nLastCol = GetColumnCount() - 1;
	*pResult = NOTVALID;

	// update item text that was edited
	// if we're auto adding and it was the last row don't allow empty strings
	// if autoaddrows is enabled and we've just edited the last row 
	// re-add the prompt item
	GetEditControl()->GetWindowText(sText);

	if (m_bAutoAddRows && m_nEditItem == nLastRow && m_nEditCol == 0)
	{
		// if its a duplicate and notifying, so notify user
		if (!sText.IsEmpty())
		{
			if (IsDuplicateRow(sText, nLastRow) && !m_bAllowDuplication)
			{
				if (m_bNotifyDuplicates)
				{
					sMessage.Format(_T("The item '%s' already exists in the list"), sText);
					AfxMessageBox(sMessage, MB_OK | MB_ICONEXCLAMATION);
					SetFocus();
				}

				EditSelectedCell();
				return;
			}
			else
			{
				SetItemText(m_nEditItem, m_nEditCol, sText);
				SetItemData(m_nEditItem, 0);

				// reinsert prompt
				nIndex = InsertItem(nLastRow + 1, m_sAutoRowPrompt, -1);
				SetItemData(nIndex, PROMPT); // for sorting

				// reset text in other cols
				nNumCols = nLastCol;
				while (nNumCols > 0)
				{
					SetItemText(nIndex, nNumCols, _T(""));
					nNumCols--;
				}

				bNotifyParent = TRUE;
				*pResult = ADDITEM;
			}
		}
	}
	else if (m_bAutoAddCols && m_nEditItem == 0 && m_nEditCol == nLastCol)
	{
		if (!sText.IsEmpty())
		{
			if (IsDuplicateCol(sText, nLastCol) && !m_bAllowDuplication)
			{
				if (m_bNotifyDuplicates)
				{
					sMessage.Format(_T("The item '%s' already exists in the list"), sText);
					AfxMessageBox(sMessage, MB_OK | MB_ICONEXCLAMATION);
					SetFocus();
				}

				EditSelectedCell();
				return;
			}
			else
			{
				nIndex = InsertColumn(GetColumnCount(), _T(""), LVCFMT_LEFT, m_nAutoColWidth);
				SetItemText(m_nEditItem, m_nEditCol + 1, m_sAutoColPrompt);
				SetItemText(m_nEditItem, m_nEditCol, sText);

				// reset text in other rows
				nNumRows = nLastRow;
				while (nNumRows > 0)
				{
					SetItemText(nNumRows, nIndex, _T(""));
					nNumRows--;
				}

				bNotifyParent = TRUE;
				*pResult = ADDITEM;
			}
		}
	}
	else // just a normal column and row but we still need to check for duplication
	{
		if (!sText.IsEmpty())
		{
			if ((m_bAutoAddRows && m_nEditCol == 0 && IsDuplicateRow(sText, m_nEditItem) && !m_bAllowDuplication) ||
				(m_bAutoAddCols && m_nEditItem == 0 && IsDuplicateCol(sText, m_nEditCol) && !m_bAllowDuplication))
			{
				if (m_bNotifyDuplicates)
				{
					sMessage.Format(_T("The item '%s' already exists in the list"), sText);
					AfxMessageBox(sMessage, MB_OK | MB_ICONEXCLAMATION);
					SetFocus();
				}
				
				EditSelectedCell();
				return;
			}
			else
			{
				SetItemText(m_nEditItem, m_nEditCol, sText);
				bNotifyParent = TRUE;
				*pResult = EDITITEM;
			}
		}
		else // remove item
		{
			SetItemText(m_nEditItem, m_nEditCol, _T(""));
			DeleteSelectedCell();

			// Only treat it as a delete if the row count has changed
			if (nLastRow == (GetItemCount() - 1))
			{
				bNotifyParent = TRUE;
				*pResult = EDITITEM;
			}
			else
			{
				bItemDeleted = TRUE;
				bNotifyParent = FALSE;
			}
		}
	}

	if (!bItemDeleted)
	{
		int nRow = (*pResult == ADDITEM) ? m_nEditItem + 1 : m_nEditItem; 
		m_nCurCol = m_nEditCol;
		SetCurSel(nRow, m_nCurCol);

		m_nLastEditRow = m_nEditItem;
		m_nLastEditCol = m_nEditCol;
		m_nLastEditResult = *pResult;
	}
	else
	{
		m_nLastEditRow = -1;
		m_nLastEditCol = -1;
		m_nLastEditResult = NOTVALID;
	}

	m_nEditCol = m_nEditItem = -1;

	// reset redrawing
	RecalcHotButtonRects();
	SetRedraw(TRUE);
	Invalidate(FALSE);
	UpdateWindow();

	// notify parent
	if (bNotifyParent)
		NotifyParentEditCell(sText);
}

void CInputListCtrl::NotifyParentEditCell(const CString& sText, int nRow, int nCol) const
{
	LV_DISPINFO lvd;

	if (nRow == -1)
		nRow = m_nLastEditRow;

	if (nCol == -1)
		nCol = m_nLastEditCol;
	
	lvd.item.iItem = nRow;
	lvd.item.iSubItem = nCol;
	lvd.item.mask = LVIF_TEXT;
	lvd.item.pszText = (LPTSTR)(LPCTSTR)sText;
	lvd.item.cchTextMax = sText.GetLength();
	
	lvd.hdr.hwndFrom = m_hWnd;
	lvd.hdr.idFrom = GetDlgCtrlID();
	lvd.hdr.code = LVN_ENDLABELEDIT;
	
	GetParent()->SendMessage(WM_NOTIFY, (WPARAM)GetDlgCtrlID(), (LPARAM)&lvd);
}

void CInputListCtrl::OnCancelEdit()
{
	HideAllControls();

	if ((m_nEditItem != -1) && (m_nEditCol != -1))
	{
		m_nCurCol = m_nEditCol;
		SetCurSel(m_nEditItem, m_nEditCol);
		m_nEditItem = m_nEditCol = -1;

		// reset redrawing
		SetRedraw(TRUE);
		Invalidate();
	}
}

void CInputListCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// note: if we are currently editing then don't pass on
	// else the edit box loses the focus
	CWnd* pFocus = GetFocus();

	if (m_editBox.m_hWnd && (pFocus == &m_editBox || m_editBox.IsWindowVisible()))
		return;

	CEnListCtrl::OnLButtonUp(nFlags, point);

	// Redraw previously pressed button
	CRect rBtn;

	if (GetButtonRect(GetCurSel(), m_nCurCol, rBtn))
		InvalidateRect(rBtn);
}

BOOL CInputListCtrl::IsPrompt(int nItem, int nCol) const
{
	if ((nItem >= 0) || (nCol >= 0))
	{
		if (m_bAutoAddRows)
		{
			if (nCol == -1)
				return (nItem == (GetItemCount() - 1));
			
			// else
			return ((nItem == (GetItemCount() - 1)) && (nCol == 0));
		}
		else if (m_bAutoAddCols)
		{
			if (nItem == -1)
				return (nCol == (GetColumnCount() - 1));

			// else
			return ((nItem == 0) && (nCol == (GetColumnCount() - 1)));
		}
	}

	// else
	return FALSE;
}

COLORREF CInputListCtrl::GetItemTextColor(int nItem, int nCol, BOOL bSelected, BOOL /*bDropHighlighted*/, BOOL bWndFocus) const
{
	BOOL bIsPrompt = IsPrompt(nItem, nCol);
	BOOL bThemedSel = (bSelected && (CThemed::AreControlsThemed() || IsSelectionThemed(TRUE)));

	// setup colors
	if (!bThemedSel && IsCellSelected(nItem, nCol))
	{
		if (bWndFocus)
			return ::GetSysColor(COLOR_HIGHLIGHTTEXT);

		if (bSelected)
			return ::GetSysColor(COLOR_WINDOWTEXT);
	}

	// else 
	if (bIsPrompt || !IsWindowEnabled())
		return ::GetSysColor(COLOR_3DDKSHADOW);
	
	// else
	return GetColumnTextColor(nCol);
}

COLORREF CInputListCtrl::GetItemBackColor(int nItem, int /*nCol*/, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const
{
	if (IsPrompt(nItem, 0))
		return CEnListCtrl::GetItemBackColor(0, bSelected, bDropHighlighted, bWndFocus);

	// else
	return CEnListCtrl::GetItemBackColor(nItem, bSelected, bDropHighlighted, bWndFocus);
}

void CInputListCtrl::SetColumnType(int nCol, IL_COLUMNTYPE nType)
{
	CColumnData2* pData = (CColumnData2*)CreateColumnData(nCol);

	if (pData)
	{
		pData->nType = nType;
	
		// Recalc the minimum height
		int nCol = GetColumnCount();
		int nNewMinHeight = GetMinItemHeight();

		while (nCol--)
		{
			int nMinColHeight = 0;

			switch (GetColumnType(nCol))
			{
			case ILCT_TEXT:
				nMinColHeight = CDlgUnits(this, TRUE).ToPixelsY(10); // default edit height
				break;

			case ILCT_CHECK:
				nMinColHeight = GraphicsMisc::ScaleByDPIFactor(18);
				break;

			default:
				nMinColHeight = GetSystemMetrics(SM_CYVTHUMB);
				break;
			}

			nNewMinHeight = max(nNewMinHeight, nMinColHeight);
		}

		SetMinItemHeight(nNewMinHeight);
	}
}

IL_COLUMNTYPE CInputListCtrl::GetColumnType(int nCol) const
{
	const CColumnData2* pData = GetColumnData(nCol);

	return pData ? pData->nType : ILCT_TEXT;
}

int CInputListCtrl::CompareItems(DWORD dwItemData1, DWORD dwItemData2, int nSortColumn) const
{
	// if one of the items is the prompt then this is always last
	if (dwItemData1 == PROMPT || dwItemData2 == PROMPT)
		return 0; // don't move item

	// else
	return CEnListCtrl::CompareItems(dwItemData1, dwItemData2, nSortColumn);
}

// base class overrides
int CInputListCtrl::SetCurSel(int nIndex, bool bNotifyParent)
{
	SetCurSel(nIndex, m_nCurCol, (BOOL)bNotifyParent);
	
	return GetCurSel();
}

int CInputListCtrl::GetCurSel() const
{
	return CEnListCtrl::GetCurSel();
}

void CInputListCtrl::SetEditMask(LPCTSTR szMask, DWORD dwFlags)
{
	m_editBox.SetMask(szMask, dwFlags);
}

void CInputListCtrl::SetReadOnly(BOOL bReadOnly)
{
	if (m_bReadOnly != bReadOnly && (m_bAutoAddRows || m_bAutoAddCols))
		Invalidate(FALSE); // to redraw the prompt string

	CEnListCtrl::SetReadOnly(bReadOnly);
}

BOOL CInputListCtrl::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_ADD:
			// Prevent Windows own column resizing
			{
				if ((pMsg->hwnd == GetSafeHwnd()) && Misc::ModKeysArePressed(MKS_CTRL))
					return TRUE;
			}
			break;

		case VK_RETURN:
			{
				CWnd* pWnd = CWnd::FromHandle(pMsg->hwnd);

				if ((pWnd != &m_editBox) && IsChild(pWnd))
				{
					HideControl(*pWnd);
					return TRUE;
				}
			}
			break;

		case VK_ESCAPE:
			{
				CWnd* pWnd = CWnd::FromHandle(pMsg->hwnd);

				if ((pWnd != &m_editBox) && IsChild(pWnd))
				{
					OnEditCancel(0, TRUE);
					return TRUE;
				}
			}
			break;
		}
		break;
	}
	
	// all else
	return CEnListCtrl::PreTranslateMessage(pMsg);
}

void CInputListCtrl::RecalcHotButtonRects()
{
	m_hotTrack.DeleteAllRects();

	if (m_hotTrack.IsInitialized() && HasNonTextCells())
	{
		int nNumRows = GetItemCount();
		int nNumCols = GetColumnCount();

		for (int nRow = 0; nRow < nNumRows; nRow++)
		{
			for (int nCol = 0; nCol < nNumCols; nCol++)
			{
				// Add rects even for cells without buttons
				// so that the button index can always be 
				// dereferenced into row and column
				CRect rBtn(0, 0, 0, 0);
				GetCellRect(nRow, nCol, rBtn);

				m_hotTrack.AddRect(rBtn);
			}
		}
	}
}

BOOL CInputListCtrl::IsButtonHot(int nRow, int nCol) const
{
	if (!CellHasButton(nRow, nCol))
		return FALSE;

	int nButton = ((nRow * GetColumnCount()) + nCol);

	return m_hotTrack.IsRectHot(nButton);
}

LRESULT CInputListCtrl::OnHotChange(WPARAM wParam, LPARAM lParam)
{
	InvalidateHotButton(wParam);
	InvalidateHotButton(lParam);

	return 0L;
}

void CInputListCtrl::InvalidateHotButton(int nBtn)
{
	if (nBtn != -1)
	{
		int nNumCols = GetColumnCount();
		int nRow = (nBtn / nNumCols);
		int nCol = (nBtn % nNumCols);

		ASSERT(nRow < GetItemCount());

		CRect rBtn;

		if (GetButtonRect(nRow, nCol, rBtn) && !rBtn.IsRectEmpty())
			InvalidateRect(rBtn, FALSE);
	}
}
