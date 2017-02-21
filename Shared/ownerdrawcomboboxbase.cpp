// ownerdrawcomboboxbase.cpp : implementation file
//

#include "stdafx.h"
#include "dlgunits.h"
#include "ownerdrawcomboboxbase.h"
#include "graphicsmisc.h"
#include "winclasses.h"
#include "dialoghelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COwnerdrawComboBoxBase

COwnerdrawComboBoxBase::COwnerdrawComboBoxBase(int nMinDLUHeight) 
	: 
	m_bItemHeightSet(FALSE), 
	m_nMinDLUHeight((nMinDLUHeight > 0) ? nMinDLUHeight : 9),
	m_nMaxTextWidth(-1)
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
	ON_WM_DESTROY()
	ON_MESSAGE(WM_SETFONT, OnSetFont)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COwnerdrawComboBoxBase message handlers

BOOL COwnerdrawComboBoxBase::SetMinDLUHeight(int nMinDLUHeight)
{
	ASSERT(GetSafeHwnd() == NULL);
	ASSERT(m_bItemHeightSet == FALSE);
	ASSERT(nMinDLUHeight > 0);

	if (!GetSafeHwnd() && !m_bItemHeightSet && (nMinDLUHeight > 0))
	{
		m_nMinDLUHeight = nMinDLUHeight;
		return TRUE;
	}

	return FALSE;
}

void COwnerdrawComboBoxBase::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC dc;
	
	if (!dc.Attach(lpDrawItemStruct->hDC))
		return;

	int nDC = dc.SaveDC();

	// initialise colours and fill bkgnd
	BOOL bSelected = (lpDrawItemStruct->itemState & ODS_SELECTED);

	COLORREF crBack = GetSysColor(IsWindowEnabled() ? (bSelected ? COLOR_HIGHLIGHT : COLOR_WINDOW) : COLOR_3DFACE);
	COLORREF crText = GetSysColor(IsWindowEnabled() ? (bSelected ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT) : COLOR_GRAYTEXT);

	// Special case
	if (IsType(CBS_SIMPLE) && !IsWindowEnabled())
		crBack = GetSysColor(COLOR_WINDOW);

	CRect rItem(lpDrawItemStruct->rcItem);

	dc.FillSolidRect(rItem, crBack);
	dc.SetTextColor(crText);

	// draw the item
	rItem.DeflateRect(2, 1);

	if (0 <= (int)lpDrawItemStruct->itemID)	// Any item selected?
	{
		// draw text
		CString sText;

		if (GetStyle() & CBS_HASSTRINGS)
		{
			GetLBText(lpDrawItemStruct->itemID, sText);
		}

		// virtual call
		DrawItemText(dc, rItem, 
					lpDrawItemStruct->itemID, 
					lpDrawItemStruct->itemState,
					lpDrawItemStruct->itemData, 
					sText, 
					TRUE);
	}
	else
	{
		CString sText;
		GetWindowText(sText);

		DrawItemText(dc, rItem, -1, 0, 0, sText, FALSE);
	}

	// Restore the DC state before focus rect
	dc.RestoreDC(nDC);

	// draw focus rect last of all
	rItem = lpDrawItemStruct->rcItem;

	if (lpDrawItemStruct->itemAction & ODA_FOCUS)
	{
		dc.DrawFocusRect(rItem);
	}
	else if (lpDrawItemStruct->itemAction & ODA_DRAWENTIRE)
	{
		if (lpDrawItemStruct->itemState & ODS_FOCUS)
			dc.DrawFocusRect(rItem);
	}

	dc.Detach();
}

void COwnerdrawComboBoxBase::DrawItemText(CDC& dc, const CRect& rect, int /*nItem*/, UINT /*nItemState*/,
										  DWORD /*dwItemData*/, const CString& sItem, BOOL /*bList*/)
{
	if (!sItem.IsEmpty())
	{
		UINT nFlags = (DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | GetDrawEllipsis() | GraphicsMisc::GetRTLDrawTextFlags(*this));

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
	
	m_bItemHeightSet = FALSE;
	InitItemHeight();
	
	return 0;
}

void COwnerdrawComboBoxBase::InitItemHeight()
{
	ASSERT(GetSafeHwnd());

	if (!m_bItemHeightSet) 
	{
		m_bItemHeightSet = TRUE;
		
		SetItemHeight(-1, CalcMinItemHeight(FALSE)); 
		SetItemHeight(0, CalcMinItemHeight(TRUE)); 
	}
}

int COwnerdrawComboBoxBase::CalcMinItemHeight(BOOL bList) const
{
	CDlgUnits dlu(this, TRUE);
	int nMinHeight = dlu.ToPixelsY(m_nMinDLUHeight);
	
	if (bList && HasIcon())
		nMinHeight = max(nMinHeight, 18);

	return nMinHeight;
}

void COwnerdrawComboBoxBase::PreSubclassWindow() 
{
	InitItemHeight();
	CComboBox::PreSubclassWindow();
}

void COwnerdrawComboBoxBase::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = CalcMinItemHeight(TRUE); 
}

int COwnerdrawComboBoxBase::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitItemHeight();
	return 0;
}

void COwnerdrawComboBoxBase::OnDestroy()
{
	// reset flag in case we get reused
	m_bItemHeightSet = FALSE;

	CComboBox::OnDestroy();
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
	return (HasIcon() ? 18 : 0);
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
		// because more than one item might exist if were doing a case-sensitive
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

