// ownerdrawcomboboxbase.cpp : implementation file
//

#include "stdafx.h"
#include "dlgunits.h"
#include "ownerdrawcomboboxbase.h"
#include "graphicsmisc.h"
#include "winclasses.h"
#include "wclassdefines.h"
#include "dialoghelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef CB_GETMINVISIBLE
#	define CBM_FIRST         0x1700
#	define CB_GETMINVISIBLE  (CBM_FIRST + 2)
#endif

/////////////////////////////////////////////////////////////////////////////
// COwnerdrawComboBoxBase

COwnerdrawComboBoxBase::COwnerdrawComboBoxBase(int nDefMinVisible) 
	: 
	m_nMaxTextWidth(-1),
	m_nDefMinVisible(nDefMinVisible),
	m_bHasHeadings(FALSE)
{
	if (m_nDefMinVisible <= 0)
		m_nDefMinVisible = 30;
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
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COwnerdrawComboBoxBase message handlers

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
	BOOL bItemDisabled = (bDisabled || (nItemState & (ODS_GRAYED | ODS_DISABLED)) || ItemIsDisabled(nItem, dwItemData));
	BOOL bItemSelected = ((nItemState & ODS_SELECTED) && !bItemDisabled);

	crBack = GetSysColor(bDisabled ? COLOR_3DFACE : (bItemSelected ? COLOR_HIGHLIGHT : COLOR_WINDOW));
	crText = GetSysColor(bItemDisabled ? COLOR_GRAYTEXT : (bItemSelected ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT));

	// Special case
	if (IsType(CBS_SIMPLE) && bDisabled)
		crBack = GetSysColor(COLOR_WINDOW);

	if (ItemIsHeading(nItem, dwItemData))
	{
		crBack = GetSysColor(COLOR_3DLIGHT);
		crText = GetSysColor(COLOR_WINDOWTEXT);
	}
}

void COwnerdrawComboBoxBase::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC dc;
	
	if (!dc.Attach(lpDrawItemStruct->hDC))
		return;

	// Determine whether this is a list item
	HWND hwndDC = ::WindowFromDC(lpDrawItemStruct->hDC);
	BOOL bListItem = FALSE;
	
	if (hwndDC)
		bListItem = CWinClasses::IsClass(hwndDC, WC_COMBOLBOX);
	else
		bListItem = (dc.GetWindowOrg().y == -1);

	int nDC = dc.SaveDC(), nItem = (int)lpDrawItemStruct->itemID;

	// Draw item background
	COLORREF crText, crBack;
	GetItemColors(nItem, lpDrawItemStruct->itemState, lpDrawItemStruct->itemData, crText, crBack);

	CRect rItem(lpDrawItemStruct->rcItem);
	dc.FillSolidRect(rItem, crBack);

	// draw the item
	rItem.DeflateRect(2, 1);

	// Indent items below their heading
	if (bListItem && m_bHasHeadings && !ItemIsHeading(nItem, lpDrawItemStruct->itemData))
	{
		rItem.left += rItem.Height();
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

	//TRACE(_T("COwnerdrawComboBoxBase::DrawItem(%s, hwnd=%s, list = %d)\n"), sText, CWinClasses::GetClass(hwndDC), bListItem);

	// virtual call
	DrawItemText(dc, rItem, nItem, 
				lpDrawItemStruct->itemState,
				lpDrawItemStruct->itemData, 
				sText, bListItem, crText);

	// Restore the DC state before focus rect
	dc.RestoreDC(nDC);

	// draw focus rect last of all
	if (WantDrawFocusRect(lpDrawItemStruct))
	{
		dc.DrawFocusRect(&lpDrawItemStruct->rcItem);
	}

	dc.Detach();
}

BOOL COwnerdrawComboBoxBase::WantDrawFocusRect(LPDRAWITEMSTRUCT lpDrawItemStruct) const
{
	if (!ItemIsSelectable((int)lpDrawItemStruct->itemID))
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

		UINT nFlags = (DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_NOCLIP | GetDrawEllipsis() | GraphicsMisc::GetRTLDrawTextFlags(*this));

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
		int nMinDLUHeight = CDlgUnits(GetParent(), TRUE).ToPixelsY(10);
		nMinHeight = max((nMinHeight + 4), nMinDLUHeight);

		if (HasIcon())
		{
			int nMinDPIHeight = GraphicsMisc::ScaleByDPIFactor(18);
			nMinHeight = max(nMinHeight, nMinDPIHeight);
		}
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
	int nWidth = 0;
	
	if (bRecalc)
	{
		m_nMaxTextWidth = nWidth = CDialogHelper::CalcMaxTextWidth(*this, 0, TRUE);
	}
	else if (m_nMaxTextWidth > 0)
	{
		nWidth = m_nMaxTextWidth;
	}
	else
	{
		CRect rWindow;
		GetWindowRect(rWindow);
		
		nWidth = rWindow.Width();
	}
	
	int nMaxWidth = GetMaxDropWidth();
	
	if (nMaxWidth > 0)
		nWidth = min(nWidth, nMaxWidth);
	
	SetDroppedWidth(nWidth + GetExtraListboxWidth());
}

BOOL COwnerdrawComboBoxBase::IsType(UINT nComboType) const
{
	return ((GetStyle() & 0xf) == nComboType);
}

int COwnerdrawComboBoxBase::GetExtraListboxWidth() const
{
	// space for icon
	return (HasIcon() ? GraphicsMisc::ScaleByDPIFactor(18) : 0);
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
	// Prevent focus moving to a container item
	int nSel = GetCurSel();

	if (ValidateSelection(nSel, TRUE))
		SetCurSel(nSel);

	return FALSE;// continue routing
}

void COwnerdrawComboBoxBase::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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

	if ((nNewSel == CB_ERR) || ItemIsSelectable(nNewSel))
	{
		CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}

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

	if (nNewSel != nCurSel)
	{
		SetCurSel(nNewSel);

		int nMsgID = (GetDroppedState() ? CBN_SELCHANGE : CBN_SELENDOK);
		GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), nMsgID), (LPARAM)GetSafeHwnd());
	}
}

BOOL COwnerdrawComboBoxBase::ValidateSelection(int& nSel, BOOL bForward) const
{
	if (nSel != CB_ERR)
	{
		int nItem = nSel;

		while (!ItemIsSelectable(nItem))
		{
			nItem += (bForward ? 1 : -1);

			if (nItem < 0 || nItem >= GetCount())
				return FALSE;
		}

		nSel = nItem;
	}

	return TRUE;
}

BOOL COwnerdrawComboBoxBase::ItemIsSelectable(int nItem) const
{
	if (nItem < 0 || nItem >= GetCount())
		return FALSE;

	DWORD dwTaskID = GetItemData(nItem);

	return !(ItemIsHeading(nItem, dwTaskID) || ItemIsDisabled(nItem, dwTaskID));
}
