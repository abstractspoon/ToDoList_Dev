// InputListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "InputListCtrl.h"
#include "themed.h"
#include "enstring.h"
#include "osversion.h"
#include "graphicsmisc.h"
#include "misc.h"
#include "enimagelist.h"
#include "dlgunits.h"

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
#define WM_SHOWPOPUPMENU (WM_APP+1001)

/////////////////////////////////////////////////////////////////////////////

const int BTN_WIDTH  = GetSystemMetrics(SM_CXVSCROLL);

/////////////////////////////////////////////////////////////////////////////

static DWORD PROMPT = 0xfefefefe;

/////////////////////////////////////////////////////////////////////////////
// CInputListCtrl

CInputListCtrl::CInputListCtrl()
{
	CInputListCtrl::InitState();
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
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
	ON_REGISTERED_MESSAGE(WM_PENDEDIT, OnEditEnd)
	ON_REGISTERED_MESSAGE(WM_PCANCELEDIT, OnEditCancel)
	ON_REGISTERED_MESSAGE(WM_HTHOTCHANGE, OnHotChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputListCtrl message handlers

void CInputListCtrl::InitState()
{
	m_nItemLastSelected = -1;
	m_nColLastSelected = -1;
	m_nEditItem = -1;
	m_nEditCol = -1;
	m_bAutoAddRows = FALSE;
	m_bAutoAddCols = FALSE;
	m_nAutoColWidth = MINCOLWIDTH;
	m_sAutoRowPrompt = "(new row)";
	m_sAutoColPrompt = "(new col)";
	m_bAllowDuplication = TRUE;
	m_bNotifyDuplicates = FALSE;
	m_bBaseClassEdit = FALSE;
	m_nCurCol = -1;
	m_nLastEditCol = m_nLastEditRow = -1;

	if (GetSafeHwnd() && CThemed::AreControlsThemed())
		m_hotTrack.Initialize(this, FALSE);
}

const CColumnData2* CInputListCtrl::GetColumnData(int nCol) const
{
	return static_cast<const CColumnData2*>(CEnListCtrl::GetColumnData(nCol)); 
}

void CInputListCtrl::AllowDuplicates(BOOL bAllow, BOOL bNotify)
{ 
	m_bAllowDuplication = bAllow; 
	m_bNotifyDuplicates = bNotify;
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
			EditCell(nItem, nCol, FALSE);
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
	if (bWasEditing && nItem == GetCurSel() && nCol == m_nCurCol)
		return;
	
	// if this is the second click or the user clicked on the column button
	// then edit else update clicked pos unless we did not have the focus
	if (nItem != -1)
	{
		CRect rButton;

		if (CanEditCell(nItem, nCol) && GetButtonRect(nItem, nCol, rButton) && rButton.PtInRect(point))
		{
			SetCurSel(nItem, nCol, TRUE); // notifies parent
			SetItemFocus(nItem, TRUE);

			EditCell(nItem, nCol, TRUE);
		}
		else if (CanEditSelectedCell() && bHadFocus && nItem == nSelItem && nCol == nSelCol)
		{
			EditCell(nItem, nCol, FALSE);
		}
		else
		{
			m_nItemLastSelected = nItem;
			m_nColLastSelected = nCol;

			SetCurSel(nItem, nCol, TRUE); // notifies parent
			SetItemFocus(nItem, TRUE);

			// scroll cell into view
			ScrollCellIntoView(nItem, nCol);
		}

		m_nCurCol = nCol;
	}
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
		if (IsColumnEditingDisabled(nCol))
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
			
			GetEditControl()->Show(rEdit);

			// this says that we are handling the edit
			// not a derived class.
			m_bBaseClassEdit = TRUE;
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

void CInputListCtrl::DisableColumnEditing(int nCol, BOOL bDisable)
{
	CColumnData2* pData = (CColumnData2*)CreateColumnData(nCol);

	// add a new item if necessary
	if (pData)
		pData->bEditEnabled = !bDisable;
}

BOOL CInputListCtrl::IsColumnEditingDisabled(int nCol) const
{
	const CColumnData2* pData = GetColumnData(nCol);

	// add a new item if necessary
	if (pData)
		return !pData->bEditEnabled;

	// else
	return FALSE;
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
		// delete last column
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

	// DO NOT DO DEFAULT HANDLING 
	// because it causes flicker in combination with our owner draw
	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
	
	nItem = GetFocusedItem();
	m_nItemLastSelected = nItem;
	
	// if its the right or left cursor keys then update column pos
	if (nChar == VK_LEFT && nCol > 0)
	{
		nCol--;
		
		// scroll cell into view
		ScrollCellIntoView(nItem, nCol);
	}
	else if (nChar == VK_RIGHT && nCol < GetColumnCount() - 1)
	{
		nCol++;
		
		// scroll cell into view
		ScrollCellIntoView(nItem, nCol);
	}
	else if (nChar == VK_DELETE && CanDeleteSelectedCell())
	{
		// if the delete key is pressed and we're in col0 or row0 
		// then delete correseponding row or column
		// unless its the prompt row or column 
		DeleteSelectedCell();
	}
	else if ((nChar == VK_F2 || nChar == VK_SPACE || nChar == VK_RETURN) && CanEditSelectedCell())
	{
		// if its the space bar then edit the current cell
		EditCell(nItem, nCol, FALSE);
	}

	// update the list selection if its changed
	SetCurSel(nItem, nCol, TRUE); // notifies parent
}

void CInputListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// this function differs from the base class in as much as it 
	// highlights only the currently selected cell, not the whole line
	CDC* pDC;
	CRect rHeader, rFocus;
	BOOL bRes;
	int nWidth;
	CSize sizeText;
	BOOL bIsPrompt;
	CSize sizeState, sizeImage;

	// get and prepare devide context
	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	pDC->SelectObject(GetFont());
	pDC->SetROP2(R2_COPYPEN);

	// init helper variables
	int nItem = lpDrawItemStruct->itemID;
	UINT uStyle = GetStyle();
	UINT uState = GetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED);
	int nImage = GetImageIndex(nItem, 0); 

	// init helper variables
	CRect rItem(lpDrawItemStruct->rcItem), rText(rItem), rClient;
	GetClientRect(&rClient);

	// some problems with drophiliting items during drag and drop
	// so we need to make sure drawing is clipped to client area
	// this fixes it admirably!
	if (GetHeader())
	{
		GetHeader()->GetWindowRect(rHeader);
		ScreenToClient(rHeader);
		rClient.top = max(0, rHeader.bottom);
		pDC->IntersectClipRect(rClient);
	}
	
	BOOL bListFocused = (GetFocus() == this);
	BOOL bSelAlways = ((uStyle & LVS_SHOWSELALWAYS) == LVS_SHOWSELALWAYS);
	BOOL bSelected = (IsWindowEnabled() && (nItem == GetCurSel()) && (bListFocused || bSelAlways));
	BOOL bItemFocused = (bListFocused && bSelected);
	BOOL bWantCellFocus = (bListFocused && !IsSelectionThemed(TRUE) && !CThemed::AreControlsThemed());
	
	// images 
	// DO NOT SUPPORT INDENTATION
	CImageList* pImageList = GetImageList(LVSIL_SMALL);

	if (pImageList)
	{
		nImage = GetImageIndex(nItem, 0); 
		CEnImageList::GetImageSize(*pImageList, sizeImage);
	}

	// state
	CImageList* pStateList = GetImageList(LVSIL_STATE);

	if (pStateList)
		CEnImageList::GetImageSize(*pStateList, sizeState);

	if (lpDrawItemStruct->itemAction & (ODA_DRAWENTIRE | ODA_SELECT))
	{
		int nSaveDC = pDC->SaveDC();

		LV_COLUMN lvc = { 0 };
		lvc.mask = LVCF_WIDTH | LVCF_FMT;
		int nCol = 0;

		// draw state image if required
		int nImageWidth = 0;
		bRes = GetColumn(nCol, &lvc);

		if (pStateList && bRes)
		{
			int nState = (GetItemState(nItem, LVIS_STATEIMAGEMASK) & LVIS_STATEIMAGEMASK);
			nImage = nState >> 12;
			pStateList->Draw(pDC, nImage, CPoint(rText.left + 1, rText.top), ILD_TRANSPARENT); 

			if (lvc.cx > sizeState.cx)
				pStateList->Draw(pDC, nState, CPoint(rText.left + 1, rText.top), ILD_TRANSPARENT); 

			nImageWidth = sizeState.cx + 2; // 1 pixel border either side
		}

		// draw item image
		if (pImageList && bRes)
		{
			int nImageStyle;

			if (bSelected && (nCol == m_nCurCol/* && nItem == GetCurSel()*/))
			{
				if (bListFocused && !IsEditing())
					nImageStyle = ILD_BLEND50;
				else
					nImageStyle = ILD_TRANSPARENT;
			}
			else
			{
				nImageStyle = ILD_TRANSPARENT;
			}

			if (lvc.cx > nImageWidth + sizeImage.cx)
				pImageList->Draw(pDC, nImage, CPoint(rText.left + 1 + nImageWidth, rText.top), nImageStyle); 

			nImageWidth += sizeImage.cx + 2; // 1 pixel border either side
		}
		
		// draw horz grid lines if required
		if (m_bHorzGrid)
		{
			if (!m_bVertGrid)
				GraphicsMisc::DrawHorzLine(pDC, rClient.left, rClient.right, rItem.bottom - 1, GetSysColor(COLOR_3DSHADOW));
			else
				GraphicsMisc::DrawHorzLine(pDC, rClient.left, (rItem.right/* - 2*/), rItem.bottom - 1, GetSysColor(COLOR_3DSHADOW));
		}

		// cycle thru the columns drawing each one
		rText.left += nImageWidth;

		while (bRes)
		{
			bIsPrompt = IsPrompt(nItem, nCol);
			BOOL bSel = (bSelected && (nCol == m_nCurCol));

			// save width and format because GetItem overwrites
			// if first column deduct width of image if exists
			nWidth = (nCol == 0) ? lvc.cx - nImageWidth : lvc.cx;

			// get next item
			bRes = GetColumn(nCol + 1, &lvc);

			CRect rCell;
			GetCellRect(nItem, nCol, rCell);

			// adjust for button
			CRect rButton(0, 0, 0, 0), rBack(rCell);
			BOOL bHasBtn = GetButtonRect(nItem, nCol, rButton);

			if (bHasBtn && (GetColumnType(nCol) != ILCT_CHECK))
				rBack.right = rButton.left;
			
			// fill cell
			if (bSel && IsSelectionThemed(FALSE))
			{
				DWORD dwFlags = ((IsSelectionThemed(TRUE) ? GMIB_THEMECLASSIC : 0) | (bHasBtn ? GMIB_CLIPRIGHT : 0));
				GM_ITEMSTATE nState = (bListFocused ? GMIS_SELECTED : GMIS_SELECTEDNOTFOCUSED);

				GraphicsMisc::DrawExplorerItemSelection(pDC, *this, nState, rBack, dwFlags, rBack);
			}
			else
			{
				if (m_bHorzGrid)
					rBack.bottom--;

				COLORREF crBack = GetItemBackColor(nItem, nCol, (bSel && !IsEditing()), FALSE, bListFocused);
				pDC->FillSolidRect(rBack, crBack);
			}

			if (m_bVertGrid && (bHasBtn || !(bSel && IsSelectionThemed(FALSE))))
			{
				// if we're not tight up against the client edge then draw the vertical 
				if (rCell.right < rClient.right)
				{
					GraphicsMisc::DrawVertLine(pDC, rCell.bottom, rCell.top, (rCell.right - 1), GetSysColor(COLOR_3DSHADOW));
					rBack.right--;
				}
			}

			// get item text
			CEnString sText;
			
			if (bIsPrompt && (IsReadOnly() || !IsWindowEnabled()))
			{
				sText.Empty(); // hides the prompt if readonly
			}
			else
			{
				CString sTemp(GetItemText(nItem, nCol));
				
				sText = sTemp;
				sizeText = sText.FormatDC(pDC, nWidth, GetColumnFormat(nCol));
			}

			// adjust text rect for button
			CRect rText(rCell);

			if (bHasBtn)
			{
				if (rButton.left <= rCell.left)
				{
					rText.left = rButton.right;
				}
				else if (rButton.right >= rCell.right)
				{
					rText.right = rButton.left;
				}
				else // button is centred => no text
				{
					rText.right = rText.left = (rText.CenterPoint().x - (BTN_WIDTH / 2));
				}
			}
			
			// setup focus rect (only for classic)
			if (bWantCellFocus && bSel)
			{
				if (rText.IsRectEmpty())
				{
					rFocus = rCell;
				}
				else
				{
					rFocus = rText;
					
					if (nCol == 0)
						rFocus.left += 2;
				}
			}
			
			// draw text
			COLORREF crText = GetItemTextColor(nItem, nCol, (bSel && !IsEditing()), FALSE, bListFocused);
	
			if (bSel && IsSelectionThemed(FALSE))
			{
				DWORD dwFlags = (IsSelectionThemed(TRUE) ? GMIB_THEMECLASSIC : 0);
				GM_ITEMSTATE nState = (bListFocused ? GMIS_SELECTED : GMIS_SELECTEDNOTFOCUSED);
				
				crText = GraphicsMisc::GetExplorerItemSelectionTextColor(crText, nState, dwFlags);
			}

			UINT nFlags = (DT_END_ELLIPSIS | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | GraphicsMisc::GetRTLDrawTextFlags(*this));

			switch ((lvc.fmt & LVCFMT_JUSTIFYMASK))
			{
			case LVCFMT_CENTER: 
				nFlags |= DT_CENTER;	
				break;

			case LVCFMT_RIGHT:	
				nFlags |= DT_RIGHT;		
				rText.right -= 4;
				break;

			case LVCFMT_LEFT:	
				nFlags |= DT_LEFT;		
				rText.left += 4;
				break;
			}

			DrawCellText(pDC, nItem, nCol, rText, sText, crText, nFlags);
		
			if (bHasBtn)
				DrawButton(pDC, nItem, nCol, rButton, !sText.IsEmpty(), bSel);

			// next column
			nCol++;
		}

		pDC->RestoreDC(nSaveDC);

		// then draw focus rect
		if (bWantCellFocus)
			pDC->DrawFocusRect(rFocus);
	}
}

void CInputListCtrl::DrawCellText(CDC* pDC, int /*nRow*/, int /*nCol*/, 
									const CRect& rText, const CString& sText, 
									COLORREF crText, UINT nDrawTextFlags)
{
	if (!sText.IsEmpty())
	{
		pDC->SetTextColor(crText);
		pDC->DrawText(sText, (LPRECT)(LPCRECT)rText, nDrawTextFlags);
	}
}

IL_COLUMNTYPE CInputListCtrl::GetCellType(int /*nRow*/, int nCol) const
{
	return GetColumnType(nCol);
}

DWORD CInputListCtrl::GetButtonState(int nRow, int nCol, BOOL bSelected) const
{
	DWORD dwState = 0;
	
	if (!IsButtonEnabled(nRow, nCol))
	{
		dwState = DFCS_INACTIVE;
	}
	else if (bSelected || IsButtonHot(nRow, nCol))
	{
		dwState = DFCS_HOT;
	}

	return dwState;
}

BOOL CInputListCtrl::DrawButton(CDC* pDC, int nRow, int nCol, CRect& rButton, BOOL bHasText, BOOL bSelected)
{
	IL_COLUMNTYPE nType = GetCellType(nRow, nCol);

	if (nType == ILCT_TEXT || !CanEditCell(nRow, nCol))
		return FALSE;

	if (!GetButtonRect(nRow, nCol, rButton))
		return FALSE;

	DWORD dwState = GetButtonState(nRow, nCol, bSelected);
	BOOL bEnabled = ((dwState & DFCS_INACTIVE) == 0);

	switch (nType)
	{
		case ILCT_DROPLIST:
		case ILCT_DATE:
			CThemed::DrawFrameControl(this, pDC, rButton, DFC_SCROLL, (DFCS_SCROLLCOMBOBOX | dwState));
			break;
					
		case ILCT_POPUPMENU:
			{
				CThemed::DrawFrameControl(this, pDC, rButton, DFC_BUTTON, (DFCS_BUTTONPUSH | dwState));

				if (!bEnabled)
					pDC->SetTextColor(GetSysColor(COLOR_3DSHADOW));

				UINT nFlags = DT_END_ELLIPSIS | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_CENTER;
				GraphicsMisc::DrawAnsiSymbol(pDC, '6', rButton, nFlags, &GraphicsMisc::Marlett());
			}
			break;

		case ILCT_BROWSE:
			{
				CThemed::DrawFrameControl(this, pDC, rButton, DFC_BUTTON, (DFCS_BUTTONPUSH | dwState));

				if (!bEnabled)
					pDC->SetTextColor(GetSysColor(COLOR_3DSHADOW));

				// Make rect sides even for better centering of ellipsis
				rButton.left += (rButton.Width() % 2);
				rButton.top += (rButton.Height() % 2);

				pDC->DrawText("...", rButton, DT_CENTER | DT_VCENTER);
			}
			break;
			
		case ILCT_CHECK:
			{
				if (bHasText)
					dwState |= DFCS_CHECKED;

				CThemed::DrawFrameControl(this, pDC, rButton, DFC_BUTTON, (DFCS_BUTTONCHECK | dwState));
			}
			break;

		default:
			return FALSE;
	}

	return TRUE;
}

BOOL CInputListCtrl::GetButtonRect(int nRow, int nCol, CRect& rButton) const
{
	rButton.SetRectEmpty();

	IL_COLUMNTYPE nType = GetCellType(nRow, nCol);

	if (nType == ILCT_TEXT || !CanEditCell(nRow, nCol))
		return FALSE;
	
	GetCellRect(nRow, nCol, rButton);

	// tweaks
	switch (nType)
	{
		case ILCT_BROWSE:
			rButton.left = (rButton.right - BTN_WIDTH);

			// Windows 10 (maybe Windows 8/8.1) shrinks buttons
			// by a pixel all round which looks inconsistent
			// with all other controls so we experiment with
			// enlarging the button appropriately
			if (COSVersion() >= OSV_WIN8)
				rButton.InflateRect(1, 1);
			break;

		case ILCT_DROPLIST:
		case ILCT_DATE:
			rButton.left = (rButton.right - BTN_WIDTH);
			break;

		case ILCT_POPUPMENU:
			rButton.right++;
			rButton.left = (rButton.right - BTN_WIDTH - 2);
			rButton.top--;
			break;

		case ILCT_CHECK:
			rButton.left += ((rButton.Width() - BTN_WIDTH) / 2);
			rButton.right = (rButton.left + BTN_WIDTH);
			break;

		default:
			rButton.SetRectEmpty();
			return FALSE;
	}

	return TRUE;
}

BOOL CInputListCtrl::CanDeleteSelectedCell() const
{
	return CanDeleteCell(GetCurSel(), m_nCurCol);
}

BOOL CInputListCtrl::CanDeleteCell(int nRow, int nCol) const
{
	// if readonly or disabled then no
	if (IsReadOnly() || !IsWindowEnabled())
		return FALSE;

	if (nRow == -1)
		return FALSE;

	// don't delete it if its the topleft item
	if (m_bAutoAddCols && nRow == 0 && m_bAutoAddRows && nCol == 0)
		return FALSE;

	// else can delete it if its not the row prompt
	else if (m_bAutoAddRows && nCol == 0)
	{
		if (nRow < GetItemCount() - 1)
			return TRUE;
	}
	// else can delete it if its not the col prompt
	else if (m_bAutoAddCols && nRow == 0)
	{
		if (nCol < GetColumnCount() - 1)
			return TRUE;
	}

	// else can't delete
	return FALSE;
}

BOOL CInputListCtrl::DeleteSelectedCell()
{
	if (GetCurSel() != - 1)
	{
		// don't delete it if its the topleft item
		if (m_bAutoAddCols && GetCurSel() == 0 && m_bAutoAddRows && m_nCurCol == 0)
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
	// NOT if readonly or disabled
	return (!IsReadOnly() && IsWindowEnabled());
}

BOOL CInputListCtrl::CanEditCell(int nRow, int nCol) const
{

	if (nRow == -1 || nCol == -1)
		return FALSE;

	// don't edit it:
	//
	// if editing is disabled in the current column
	if (IsColumnEditingDisabled(nCol))
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
	return CanEditCell(GetCurSel(), m_nCurCol);
}

void CInputListCtrl::EditSelectedCell()
{
	EditCell(GetCurSel(), m_nCurCol, FALSE);
}

void CInputListCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	// ignore if WE are pNewWnd
	if (pNewWnd == this)
		return;

	// if we're not editing then clear the current selection
	if (!IsEditing())
	{
		m_nItemLastSelected = -1;
		m_nColLastSelected = -1;
	}

	CRect rItem;
	GetItemRect(GetCurSel(), rItem, LVIR_BOUNDS);
	InvalidateRect(rItem, FALSE);

	CEnListCtrl::OnKillFocus(pNewWnd);
}

BOOL CInputListCtrl::SetCellText(int nRow, int nCol, const CString& sText)
{
	ASSERT (m_hWnd);
	ASSERT ((m_bAutoAddRows && nRow >=0 && nRow < GetItemCount() - 1) || 
			(!m_bAutoAddRows && nRow >=0 && nRow < GetItemCount()));
	ASSERT ((m_bAutoAddCols && nCol >=0 && nCol < GetColumnCount() - 1) ||
			(!m_bAutoAddCols && nCol >=0 && nCol < GetColumnCount()));

	// only allow text setting if within valid range and if user is
	// not trying to change prompt string if auto adding
	if (((m_bAutoAddRows && nRow >=0 && nRow < GetItemCount() - 1) || 
			(!m_bAutoAddRows && nRow >=0 && nRow < GetItemCount())) &&
		 ((m_bAutoAddCols && nCol >=0 && nCol < GetColumnCount() - 1) ||
			(!m_bAutoAddCols && nCol >=0 && nCol < GetColumnCount())))
	{
		SetItemText(nRow, nCol, sText);
		return TRUE;
	}

	return FALSE;
}

BOOL CInputListCtrl::DeleteAllItems(BOOL bIncludeCols)
{
	CString sText;
	BOOL bRes;
	int nNumCols;

	// delete all 
	bRes = CEnListCtrl::DeleteAllItems();

	nNumCols = GetColumnCount();

	// delete columns if necessary
	if (bIncludeCols)
	{
		while (DeleteColumn(0));
	}

	if (!bRes || (!m_bAutoAddRows && !m_bAutoAddCols))
		return bRes;

	// if auto adding re-add prompts as appropriate
	nNumCols = GetColumnCount();

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

void CInputListCtrl::SetCurSel(int nRow, int nCol, BOOL bNotifyParent)
{
	ASSERT (m_hWnd);
	ASSERT (nRow >= -1 && nRow < GetItemCount() && 
			nCol >= -1 && nCol < GetColumnCount());

	// only allow selection setting if within valid range 
	if (nRow < 0 || nRow >= GetItemCount() || nCol < 0 || nCol >= GetColumnCount())
		return;

	// don't update if nothing's changed
	if (nCol == m_nCurCol && nRow == CEnListCtrl::GetCurSel())
		return;

	CRect rItem;
	int nSel = GetCurSel();
	RedrawItems(nSel, nSel);

	m_nCurCol = nCol;
	CEnListCtrl::SetCurSel(nRow, bNotifyParent == TRUE);

	m_nColLastSelected = nCol;
	m_nItemLastSelected = nRow;
	RedrawItems(nRow, nRow);
}

int CInputListCtrl::GetLastEdit(int* pRow, int* pCol)
{
	if (pRow)
		*pRow = m_nLastEditRow;

	if (pCol)
		*pCol = m_nLastEditCol;

	return m_nLastEdit;
}

BOOL CInputListCtrl::GetCurSel(int& nRow, int& nCol) const
{
	nRow = GetCurSel();
	nCol = m_nCurCol;

	return ((nRow != -1) && (nCol != -1));
}

int CInputListCtrl::InsertRow(CString sRowText, int nRow, int nImage)
{
	if (m_bAutoAddRows && nRow == GetItemCount())
		nRow--; // add before prompt

	if (nImage != -1)
		nRow = InsertItem(nRow, sRowText, nImage);
	else
		nRow = InsertItem(nRow, sRowText);

	RecalcHotButtonRects();

	return nRow;
}

BOOL CInputListCtrl::HasNonTextColumns() const
{
	int nCol = GetColumnCount();

	while (nCol--)
	{
		if (GetColumnType(nCol) != ILCT_TEXT)
			return TRUE;
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
	// if we're editing then quit editing
	OnCancelEdit();

	CEnListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);

	RecalcHotButtonRects();
}

void CInputListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// if we're editing then quit editing
	OnCancelEdit();

	CEnListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);

	RecalcHotButtonRects();
}

#if _MSC_VER >= 1400
BOOL CInputListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
#else
void CInputListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
#endif
{
	OnCancelEdit();

	LRESULT lr = Default();

	RecalcHotButtonRects();
	
#if _MSC_VER >= 1400
	return lr;
#endif
}

void CInputListCtrl::OnSize(UINT nType, int cx, int cy)
{
	// if we're editing then quit editing
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

void CInputListCtrl::GetCellEditRect(int nRow, int nCol, CRect& rCell)
{
	CEnListCtrl::GetCellRect(nRow, nCol, rCell);

	switch (GetCellType(nRow, nCol))
	{
	case ILCT_TEXT:
	case ILCT_BROWSE:
	case ILCT_DROPLIST:
		//rCell.right++;
		
		// move top edge up one pixel so that it looks right
		// but not of the first row else it gets clipped 
		// by the window border
		if (nRow > 0)
			rCell.top--;
		break;

	case ILCT_DATE:
	case ILCT_CHECK:
		//rCell.OffsetRect(1, -1);
		//rCell.left--;
		break;
	}
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
	RefreshItemHeight();
}

int CInputListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEnListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	InitState();
	RefreshItemHeight();
	
	return 0;
}

void CInputListCtrl::HideControl(CWnd& ctrl)
{
	if (ctrl.IsWindowVisible())
	{
		ctrl.ShowWindow(SW_HIDE);
		ctrl.EnableWindow(FALSE);
	}
}

void CInputListCtrl::ShowControl(CWnd& ctrl, int nRow, int nCol)
{
	PrepareControl(ctrl, nRow, nCol);
	ScrollCellIntoView(nRow, nCol);

	CRect rCell;
	GetCellEditRect(nRow, nCol, rCell);

	if (ctrl.IsKindOf(RUNTIME_CLASS(CComboBox)))
		rCell.bottom += 200;

	ctrl.MoveWindow(rCell);
	ctrl.EnableWindow(TRUE);
	ctrl.ShowWindow(SW_SHOW);
	ctrl.SetFocus();

	if (ctrl.IsKindOf(RUNTIME_CLASS(CComboBox)))
	{
		CComboBox* pCombo = (CComboBox*)&ctrl;
		pCombo->ShowDropDown(TRUE);
	}
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

void CInputListCtrl::CreateControl(CWnd& ctrl, UINT nID, BOOL bSort)
{
	DWORD dwStyle = WS_CHILD;

	if (ctrl.IsKindOf(RUNTIME_CLASS(CComboBox)))
	{
		dwStyle |= CBS_DROPDOWNLIST | WS_VSCROLL | CBS_AUTOHSCROLL;

		if (bSort)
			dwStyle |= CBS_SORT;

		CComboBox* pCombo = (CComboBox*)&ctrl;
		VERIFY (pCombo->Create(dwStyle, CRect(0, 0, 0, 0), this, nID));
	}
	else if (ctrl.IsKindOf(RUNTIME_CLASS(CEdit)))
	{
		CEdit* pEdit = (CEdit*)&ctrl;
		VERIFY (pEdit->Create(dwStyle, CRect(0, 0, 0, 0), this, nID));

		pEdit->ModifyStyleEx(0, WS_EX_CLIENTEDGE, 0);
	}
	else// if (ctrl.IsKindOf(RUNTIME_CLASS(CDateTimeCtrl)))
	{
		CDateTimeCtrl* pDateTime = (CDateTimeCtrl*)&ctrl;
		VERIFY (pDateTime->Create(dwStyle, CRect(0, 0, 0, 0), this, nID));
	}

	ctrl.SetFont(GetFont()); // set font to parents
	ctrl.ShowWindow(SW_HIDE);

	CRect rWnd;
	ctrl.GetClientRect(rWnd);

	SetMinItemHeight(max(GetMinItemHeight(), rWnd.Height()));
}

CPopupEditCtrl* CInputListCtrl::GetEditControl()
{
	ASSERT (m_hWnd);

	if (!m_hWnd)
		return NULL;

	if (!m_editBox.m_hWnd)
		m_editBox.Create(this, IDC_EDITBOX, (WS_CHILD | WS_BORDER));

	return &m_editBox;
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
		m_nLastEdit = *pResult;
	}
	else
	{
		m_nLastEditRow = -1;
		m_nLastEditCol = -1;
		m_nLastEdit = NOTVALID;
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
	if ((m_nEditItem != -1) && (m_nEditCol != -1))
	{
		GetEditControl()->ShowWindow(SW_HIDE);

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
	CWnd* pFocus;

	// note: if we are currently editing then don't pass on
	// else the edit box loses the focus
	pFocus = GetFocus();

	if (m_editBox.m_hWnd && (pFocus == &m_editBox || m_editBox.IsWindowVisible()))
		return;

	CEnListCtrl::OnLButtonUp(nFlags, point);
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
	if (!bThemedSel && (nCol == m_nCurCol) && (nItem == GetCurSel()))
	{
		// if focused then draw item in focused colors 
		if (bWndFocus)
		{
			return ::GetSysColor(COLOR_HIGHLIGHTTEXT);
		}
		else if (bSelected)
		{
			// else if not focused then draw selection else
			// draw in column colors as below unless we're the prompt and 
			// readonly - then draw in back color (ie hide it)
			return ::GetSysColor(COLOR_WINDOWTEXT);
		}
	}

	// else 
	if (bIsPrompt || !IsWindowEnabled())
		return ::GetSysColor(COLOR_3DDKSHADOW);
	
	// else
	return GetColumnTextColor(nCol);
}

COLORREF CInputListCtrl::GetItemBackColor(int nItem, int /*nCol*/, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const
{
	BOOL bIsPrompt = IsPrompt(nItem, 0);

	if (bIsPrompt)
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
		int nNewMinHeight = 0;

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

int CInputListCtrl::CompareItems(DWORD dwItemData1, DWORD dwItemData2, int nSortColumn)
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
		// Prevent Windows own column resizing
		if ((pMsg->hwnd == GetSafeHwnd()) &&
			(pMsg->wParam == VK_ADD) && 
			Misc::ModKeysArePressed(MKS_CTRL))
		{
			return TRUE;
		}
		break;
	}
	
	// all else
	return CEnListCtrl::PreTranslateMessage(pMsg);
}

void CInputListCtrl::RecalcHotButtonRects()
{
	m_hotTrack.DeleteAllRects();

	if (m_hotTrack.IsInitialized() && HasNonTextColumns())
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
	CRect rButton;

	if (!GetButtonRect(nRow, nCol, rButton))
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
