// ownerdrawcomboboxbase.cpp : implementation file
//

#include "stdafx.h"
#include "dlgunits.h"
#include "ownerdrawcomboboxbase.h"
#include "graphicsmisc.h"
#include "winclasses.h"
#include "wclassdefines.h"
#include "dialoghelper.h"
#include "misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

static HFONT s_hFontHeadings = NULL;

/////////////////////////////////////////////////////////////////////////////

#ifndef CB_GETMINVISIBLE
#	define CBM_FIRST         0x1700
#	define CB_GETMINVISIBLE  (CBM_FIRST + 2)
#endif

/////////////////////////////////////////////////////////////////////////////
// COwnerdrawComboBoxBase

COwnerdrawComboBoxBase::COwnerdrawComboBoxBase(int nDefMinVisible, int nItemIndentBelowHeadings) 
	: 
	m_nMaxTextWidth(-1),
	m_nDefMinVisible(nDefMinVisible <= 0 ? 30 : nDefMinVisible),
	m_nNumHeadings(0),
	m_bHasExtItemData(FALSE),
	m_nItemIndentBelowHeadings(nItemIndentBelowHeadings)
{
}

COwnerdrawComboBoxBase::~COwnerdrawComboBoxBase()
{
}

IMPLEMENT_DYNAMIC(COwnerdrawComboBoxBase, CComboBox)

BEGIN_MESSAGE_MAP(COwnerdrawComboBoxBase, CComboBox)
	//{{AFX_MSG_MAP(COwnerdrawComboBoxBase)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_CONTROL_REFLECT_EX(CBN_SELENDOK, OnSelEndOK)
	ON_WM_KEYDOWN()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()

	ON_MESSAGE(CB_GETITEMDATA, OnCBGetItemData)
	ON_MESSAGE(CB_SETITEMDATA, OnCBSetItemData)
	ON_MESSAGE(CB_DELETESTRING, OnCBDeleteString)
	ON_MESSAGE(CB_RESETCONTENT, OnCBResetContent)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COwnerdrawComboBoxBase message handlers

int COwnerdrawComboBoxBase::SetDisabledItem(int nItem, BOOL bDisabled)
{
	ODCB_ITEMDATA* pItemData = GetAddExtItemData(nItem);

	if (pItemData == NULL)
	{
		ASSERT(0);
		return CB_ERR;
	}

	pItemData->bDisabled = bDisabled;
	return CB_OKAY;
}

int COwnerdrawComboBoxBase::SetHeadingItem(int nItem, BOOL bHeading)
{
	ODCB_ITEMDATA* pItemData = GetAddExtItemData(nItem);

	if (pItemData == NULL)
	{
		ASSERT(0);
		return CB_ERR;
	}

	if (bHeading && !pItemData->bHeading)
	{
		m_nNumHeadings++;
	}
	else if (!bHeading && pItemData->bHeading)
	{
		m_nNumHeadings--;
	}

	pItemData->bHeading = bHeading;
	return CB_OKAY;
}

void COwnerdrawComboBoxBase::SetItemIndentBelowHeadings(int nIndent)
{
	m_nItemIndentBelowHeadings = nIndent;
}

BOOL COwnerdrawComboBoxBase::IsHeadingItem(int nItem) const
{
	const ODCB_ITEMDATA* pItemData = GetExtItemData(nItem);

	return (pItemData ? pItemData->bHeading : FALSE);
}

BOOL COwnerdrawComboBoxBase::IsDisabledItem(int nItem) const
{
	const ODCB_ITEMDATA* pItemData = GetExtItemData(nItem);

	return (pItemData ? pItemData->bDisabled : FALSE);
}

BOOL COwnerdrawComboBoxBase::IsSelectableItem(int nItem) const
{
	const ODCB_ITEMDATA* pItemData = GetExtItemData(nItem);

	if (!pItemData)
		return TRUE;

	// else
	return !(pItemData->bHeading || pItemData->bDisabled);
}

int COwnerdrawComboBoxBase::SetCurSel(int nSel, BOOL bValidate)
{
	if (bValidate)
	{
		// Step over container/disabled items
		if (!ValidateSelection(nSel, TRUE))
			ValidateSelection(nSel, FALSE); // move back one place
	}

	return CComboBox::SetCurSel(nSel);
}

void COwnerdrawComboBoxBase::GetItemColors(int nItem, UINT nItemState, DWORD dwItemData, 
											COLORREF& crText, COLORREF& crBack) const
{
	BOOL bDisabled = !IsWindowEnabled();
	BOOL bItemDisabled = (bDisabled || (nItemState & (ODS_GRAYED | ODS_DISABLED)) || !IsSelectableItem(nItem));
	BOOL bItemSelected = ((nItemState & ODS_SELECTED) && !bItemDisabled);

	crBack = GetSysColor(bDisabled ? COLOR_3DFACE : (bItemSelected ? COLOR_HIGHLIGHT : COLOR_WINDOW));
	crText = GetSysColor(bItemDisabled ? COLOR_GRAYTEXT : (bItemSelected ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT));

	// Special case to match Windows
	if (IsType(CBS_SIMPLE) && bDisabled)
		crBack = GetSysColor(COLOR_WINDOW);

	if (IsHeadingItem(nItem))
	{
		crBack = GetSysColor(COLOR_3DFACE);
		crText = GetSysColor(COLOR_WINDOWTEXT);
	}
}

void COwnerdrawComboBoxBase::OnPaint()
{
	CPaintDC dc(this);

	// default drawing
	DefWindowProc(WM_PAINT, (WPARAM)dc.m_hDC, 0);
}

void COwnerdrawComboBoxBase::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC dc;
	
	if (!dc.Attach(lpDrawItemStruct->hDC))
		return;

	int nDC = dc.SaveDC(), nItem = (int)lpDrawItemStruct->itemID;
	BOOL bListItem = !(lpDrawItemStruct->itemState & ODS_COMBOBOXEDIT);

	// Fixup item data because Windows will have passed us the 'raw' item data
	DWORD dwItemData = (m_bHasExtItemData ? GetItemData(nItem) : lpDrawItemStruct->itemData);

	// Draw item background
	COLORREF crText, crBack;
	GetItemColors(nItem, lpDrawItemStruct->itemState, dwItemData, crText, crBack);

	if (bListItem)
		FillListItemBkgnd(dc, lpDrawItemStruct->rcItem, nItem, lpDrawItemStruct->itemState, dwItemData, crBack);

	// draw the item
	CRect rItem(lpDrawItemStruct->rcItem);
	rItem.DeflateRect(2, 1);

	// Indent items below their heading
	BOOL bHeading = (bListItem && IsHeadingItem(nItem));

	if (bListItem && m_nNumHeadings && !bHeading)
	{
		rItem.left += m_nItemIndentBelowHeadings;
	}

	CString sText;

	if (nItem != CB_ERR) // Any item selected?
	{
		// draw text
		if (GetStyle() & CBS_HASSTRINGS)
			GetLBText(nItem, sText);
	}
	else
	{
		GetWindowText(sText);
	}

	// Show headings in bold
	if (bHeading)
	{
		// We share a single font across the entire app
		// and if ever the font changes for a given window
		// we just recreate the font
		HFONT hFont = GraphicsMisc::GetFont(lpDrawItemStruct->hwndItem);

		if (hFont && !GraphicsMisc::IsSameFontNameAndSize(hFont, s_hFontHeadings))
		{
			GraphicsMisc::VerifyDeleteObject(s_hFontHeadings);

			s_hFontHeadings = GraphicsMisc::CreateFont(hFont, GMFS_BOLD);
			ASSERT(s_hFontHeadings);
		}
		dc.SelectObject(s_hFontHeadings);

		// Note: No need to manually de-select the font from the dc
		// because this will be handled by the call to RestoreDC below
	}

	// virtual call
	DrawItemText(dc, rItem, nItem, lpDrawItemStruct->itemState, dwItemData,	sText, bListItem, crText);

	// Restore the DC state before focus rect
	dc.RestoreDC(nDC);

	// draw focus rect last of all
	if (WantDrawFocusRect(lpDrawItemStruct))
	{
		dc.DrawFocusRect(&lpDrawItemStruct->rcItem);
	}

	dc.Detach();
}

void COwnerdrawComboBoxBase::FillListItemBkgnd(CDC& dc, const CRect& rect, int /*nItem*/, UINT /*nItemState*/, DWORD /*dwItemData*/, COLORREF crBack)
{
	dc.FillSolidRect(rect, crBack);
}

BOOL COwnerdrawComboBoxBase::WantDrawFocusRect(LPDRAWITEMSTRUCT lpDrawItemStruct) const
{
	if (!IsSelectableItem((int)lpDrawItemStruct->itemID))
		return FALSE;
	
	if (lpDrawItemStruct->itemState & (ODS_DISABLED | ODS_GRAYED))
		return FALSE;

	if (lpDrawItemStruct->itemAction & ODA_FOCUS)
		return TRUE;

	if ((lpDrawItemStruct->itemAction & ODA_DRAWENTIRE) && 
		(lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		return TRUE;
	}

	return FALSE;
}

void COwnerdrawComboBoxBase::DrawItemText(CDC& dc, const CRect& rect, int /*nItem*/, UINT /*nItemState*/,
										  DWORD /*dwItemData*/, const CString& sItem, BOOL /*bList*/, COLORREF crText)
{
	if (!sItem.IsEmpty())
	{
		if (crText != CLR_NONE)
			dc.SetTextColor(crText);

		UINT nFlags = (DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_NOCLIP | GetDrawEllipsis());
		dc.DrawText(sItem, (LPRECT)(LPCRECT)rect, nFlags);
	}
}

BOOL COwnerdrawComboBoxBase::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= (CBS_OWNERDRAWFIXED | CBS_HASSTRINGS);

	return CComboBox::PreCreateWindow(cs);
}

LRESULT COwnerdrawComboBoxBase::OnSetFont(WPARAM , LPARAM)
{
	Default();
	InitItemHeight();
	
	return 0;
}

void COwnerdrawComboBoxBase::InitItemHeight()
{
	ASSERT(GetSafeHwnd());

	SetItemHeight(-1, CalcMinItemHeight(FALSE)); 
	SetItemHeight(0, CalcMinItemHeight(TRUE)); 
}

int COwnerdrawComboBoxBase::CalcMinItemHeight(BOOL bList) const
{
	int nMinHeight = GraphicsMisc::GetFontPixelSize(*this);
	
	if (bList)
	{
		int nMinDLUHeight = CDlgUnits(GetParent(), TRUE).ToPixelsY(9);
		nMinHeight = max((nMinHeight + 4), nMinDLUHeight);
	}
	else
	{
		int nFudge = 0;

		if (GraphicsMisc::WantDPIScaling())
			nFudge = GraphicsMisc::ScaleByDPIFactor(2);

		nMinHeight += (nFudge + (2*GetSystemMetrics(SM_CYEDGE)));
	}

	return nMinHeight;
}

void COwnerdrawComboBoxBase::PreSubclassWindow() 
{
	ASSERT(GetStyle() & (CBS_OWNERDRAWFIXED | CBS_OWNERDRAWVARIABLE));

	InitItemHeight();

	CComboBox::PreSubclassWindow();
}

void COwnerdrawComboBoxBase::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	UINT nMinHeight = CalcMinItemHeight(TRUE);

	lpMeasureItemStruct->itemHeight = max(lpMeasureItemStruct->itemHeight, nMinHeight); 
}

int COwnerdrawComboBoxBase::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitItemHeight();
	return 0;
}

void COwnerdrawComboBoxBase::RefreshDropWidth()
{
	RefreshDropWidth(TRUE);
}

void COwnerdrawComboBoxBase::RefreshDropWidth(BOOL bRecalc)
{
	if (bRecalc)
		m_nMaxTextWidth = CDialogHelper::CalcMaxTextWidth(*this, 0, TRUE);

	int nDefaultWidth = CDialogHelper::GetChildWidth(this);
	int nReqWidth = 0;

	if (m_nMaxTextWidth > 0)
	{
		// Derived classes can request extra space for drawing
		nReqWidth = (m_nMaxTextWidth + GetExtraListboxWidth());
	
		// And can set a maximum width
		int nMaxWidth = GetMaxDropWidth();
	
		if (nMaxWidth > 0)
			nReqWidth = min(nReqWidth, nMaxWidth);
	}

	SetDroppedWidth(max(nDefaultWidth, nReqWidth));
}

void COwnerdrawComboBoxBase::OnSize(UINT nType, int cx, int cy)
{
	CComboBox::OnSize(nType, cx, cy);

	RefreshDropWidth(FALSE);
}

BOOL COwnerdrawComboBoxBase::IsType(UINT nComboType) const
{
	return ((GetStyle() & 0xf) == nComboType);
}

int COwnerdrawComboBoxBase::FindStringExact(int nIndexStart, LPCTSTR lpszFind) const
{ 
	return FindStringExact(nIndexStart, lpszFind, FALSE); 
}

int COwnerdrawComboBoxBase::FindStringExact(int nIndexStart, const CString& sItem, BOOL bCaseSensitive) const
{
	if (GetCount() == 0)
		return CB_ERR;
	
	int nFind = nIndexStart; // default
	
	if (!sItem.IsEmpty())
	{
		// because more than one item might exist if we're doing a case-sensitive
		// search we can't just stop if the first find doesn't exactly match
		// because there still may be further matches
		BOOL bContinue = TRUE;
		
		while (bContinue)
        {
			int nPrevFind = nFind;
			nFind = CComboBox::FindStringExact(nFind, sItem);
			
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

int COwnerdrawComboBoxBase::GetMinVisible() const
{
	int nMinVis = ::SendMessage(m_hWnd, CB_GETMINVISIBLE, 0, 0);

	if (nMinVis == 0)
		return m_nDefMinVisible;

	return nMinVis;
}

BOOL COwnerdrawComboBoxBase::OnSelEndOK()
{
	// Prevent focus moving to a container/disabled item
	int nSel = GetCurSel();

	// We don't receive WM_KEYDOWN for simple combos, so
	// we need to decide if we are validating up or down
	BOOL bValidateDown = TRUE;

	if (IsType(CBS_SIMPLE))
		bValidateDown = ((nSel == 0) || !(Misc::IsKeyPressed(VK_UP) || Misc::IsKeyPressed(VK_PRIOR)));

	if (ValidateSelection(nSel, bValidateDown))
		SetCurSel(nSel);

	return FALSE;// continue routing
}

BOOL COwnerdrawComboBoxBase::HandleCursorKey(UINT nChar)
{
	// Step over container and disabled items
	int nCurSel = GetCurSel(), nNewSel = CB_ERR;

	switch (nChar)
	{
	case VK_UP:
		nNewSel = (nCurSel - 1);
		nNewSel = max(0, nNewSel);
		break;

	case VK_PRIOR:
		nNewSel = (nCurSel - (GetMinVisible() - 1));
		nNewSel = max(0, nNewSel);
		break;

	case VK_DOWN:
		nNewSel = (nCurSel + 1);
		nNewSel = min((GetCount() - 1), nNewSel);
		break;

	case VK_NEXT:
		nNewSel = (nCurSel + (GetMinVisible() - 1));
		nNewSel = min((GetCount() - 1), nNewSel);
		break;

	case VK_HOME:
		nNewSel = 0;
		break;

	case VK_END:
		nNewSel = (GetCount() - 1);
		break;
	}

	if ((nNewSel == CB_ERR) || IsSelectableItem(nNewSel))
		return FALSE;

	if (nNewSel > nCurSel)
	{
		// Step over container/disabled items
		if (!ValidateSelection(nNewSel, TRUE))
			ValidateSelection(nNewSel, FALSE); // move back one place
	}
	else
	{
		if (!ValidateSelection(nNewSel, FALSE))
			ValidateSelection(nNewSel, TRUE); // move back one place
	}

	if (nNewSel == nCurSel)
		return FALSE;

	// else
	SetCurSel(nNewSel);

	int nMsgID = (GetDroppedState() ? CBN_SELCHANGE : CBN_SELENDOK);
	GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), nMsgID), (LPARAM)GetSafeHwnd());

	return TRUE;
}

void COwnerdrawComboBoxBase::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (HandleCursorKey(nChar))
		return;

	CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL COwnerdrawComboBoxBase::ValidateSelection(int& nSel, BOOL bForward) const
{
	if (nSel != CB_ERR)
	{
		int nItem = nSel;

		while (!IsSelectableItem(nItem))
		{
			nItem += (bForward ? 1 : -1);

			if (nItem < 0 || nItem >= GetCount())
				return FALSE;
		}

		nSel = nItem;
	}

	return TRUE;
}

LRESULT COwnerdrawComboBoxBase::OnCBSetItemData(WPARAM wParam, LPARAM lParam)
{
	if (!IsValidIndex((int)wParam))
	{
		ASSERT(0);
		return CB_ERR;
	}

	ODCB_ITEMDATA* pItemData = GetAddExtItemData(wParam);

	if (pItemData == NULL)
	{
		ASSERT(0);
		return CB_ERR;
	}

	pItemData->dwItemData = lParam;
	return CB_OKAY;
}

LRESULT COwnerdrawComboBoxBase::OnCBGetItemData(WPARAM wParam, LPARAM /*lParam*/)
{
	LRESULT lResult = GetRawItemData((int)wParam);

	if (!m_bHasExtItemData || !lResult)
		return lResult;

	ODCB_ITEMDATA* pItemData = (ODCB_ITEMDATA*)lResult;

	return pItemData->dwItemData;
}

LRESULT COwnerdrawComboBoxBase::OnCBDeleteString(WPARAM wParam, LPARAM lParam)
{
	ODCB_ITEMDATA* pItemData = GetExtItemData(wParam);

	LRESULT lResult = DefWindowProc(CB_DELETESTRING, wParam, lParam);

	if (lResult != CB_ERR)
		delete pItemData;

	return lResult;
}

void COwnerdrawComboBoxBase::OnDestroy()
{
	DeleteAllExtItemData();

	CComboBox::OnDestroy();
}

void COwnerdrawComboBoxBase::DeleteAllExtItemData()
{
	if (m_bHasExtItemData)
	{
		int nItem = GetCount();

		while (nItem--)
			delete GetExtItemData(nItem);

		m_bHasExtItemData = FALSE;
	}

	m_nNumHeadings = 0;
}

LRESULT COwnerdrawComboBoxBase::OnCBResetContent(WPARAM wParam, LPARAM lParam)
{
	// delete check states first
	DeleteAllExtItemData();

	return DefWindowProc(CB_RESETCONTENT, wParam, lParam);
}

LRESULT COwnerdrawComboBoxBase::GetRawItemData(int nItem) const
{
	if (!IsValidIndex(nItem))
		return 0;

	COwnerdrawComboBoxBase* pThis = const_cast<COwnerdrawComboBoxBase*>(this);

	return pThis->DefWindowProc(CB_GETITEMDATA, nItem, 0);
}

LRESULT COwnerdrawComboBoxBase::SetRawItemData(int nItem, ODCB_ITEMDATA*& pItemData)
{
	if (!IsValidIndex(nItem) || !pItemData)
	{
		ASSERT(0);
		return CB_ERR;
	}

	LRESULT lResult = DefWindowProc(CB_SETITEMDATA, nItem, (LPARAM)pItemData);
	
	if (lResult == CB_ERR)
	{
		delete pItemData;
		pItemData = NULL;
	}

	return lResult;
}

ODCB_ITEMDATA* COwnerdrawComboBoxBase::GetExtItemData(int nItem) const
{
	if (!IsValidIndex(nItem) || !m_bHasExtItemData)
	{
		return NULL;
	}

	return (ODCB_ITEMDATA*)GetRawItemData(nItem);
}

ODCB_ITEMDATA* COwnerdrawComboBoxBase::GetAddExtItemData(int nItem)
{
	if (!IsValidIndex(nItem))
	{
		ASSERT(0);
		return NULL;
	}

	if (!m_bHasExtItemData && GetCount())
	{
		// Convert existing item data to extended format
		int nExist = GetCount();

		while (nExist--)
		{
			ODCB_ITEMDATA* pItemData = NewExtItemData();
			ASSERT(pItemData);

			pItemData->dwItemData = GetRawItemData(nExist);
			SetRawItemData(nExist, pItemData);
		}

		m_bHasExtItemData = TRUE;
	}

	ODCB_ITEMDATA* pItemData = GetExtItemData(nItem);

	if (pItemData == NULL)
	{
		pItemData = NewExtItemData();
		SetRawItemData(nItem, pItemData);
	}

	return pItemData;
}

BOOL COwnerdrawComboBoxBase::SelectNextItem(const CString& sText, BOOL bForward)
{
	if (sText.IsEmpty())
	{
		SetCurSel(-1, FALSE);
		return CB_ERR;
	}

	// else
	int nSel = GetCurSel();
	int nFrom = (bForward ? (nSel + 1) : (nSel - 1));

	int nNext = FindNextItem(sText, nFrom, bForward);

	if (nNext == nSel)
		return FALSE;

	SetCurSel(nNext);
	return TRUE;
}

int COwnerdrawComboBoxBase::FindNextItem(const CString& sText, int nFrom, BOOL bForward) const
{
	int nNumItems = GetCount();

	if (nNumItems == 0)
		return CB_ERR;

	int nNext = CB_ERR;

	if (bForward)
	{
		if ((nFrom < 0) || (nFrom >= nNumItems))
			nFrom = 0;

		// From nFrom to end of combo
		if (FindNextItem(sText, nFrom, (nNumItems - 1), 1, nNext))
			return nNext;

		// From start of combo to nFrom
		if ((nFrom > 0) && FindNextItem(sText, 0, (nFrom - 1), 1, nNext))
			return nNext;
	}
	else // backwards
	{
		if ((nFrom < 0) || (nFrom >= nNumItems))
			nFrom = (nNumItems - 1);

		// From nFrom to start of combo
		if (FindNextItem(sText, nFrom, 0, -1, nNext))
			return nNext;

		// From end of combo to nFrom
		if (FindNextItem(sText, (nNumItems - 1), (nFrom + 1), -1, nNext))
			return nNext;
	}

	// else
	return CB_ERR;
}

BOOL COwnerdrawComboBoxBase::FindNextItem(const CString& sText, int nFrom, int nTo, int nIncrement, int& nNext) const
{
	// Sanity checks
	if (!(((nIncrement == 1) && (nTo >= nFrom)) || ((nIncrement == -1) && (nTo <= nFrom))))
	{
		ASSERT(0);
		return FALSE;
	}

	nNext = CB_ERR;
	nTo += nIncrement; // so the != will work to stop the loop

	for (int nItem = nFrom; nItem != nTo; nItem += nIncrement)
	{
		if (!IsSelectableItem(nItem))
			continue;

		CString sItem(CDialogHelper::GetItem(*this, nItem));

		if (Misc::Find(sText, sItem) != -1)
		{
			nNext = nItem;
			break;
		}
	}

	return (nNext != CB_ERR);
}

