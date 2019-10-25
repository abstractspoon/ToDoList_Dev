// colorcombobox.cpp : implementation file
//

#include "stdafx.h"
#include "colorcombobox.h"
#include "dlgunits.h"
#include "graphicsmisc.h"
#include "misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorComboBox

CColorComboBox::CColorComboBox(BOOL dwFlags, DWORD dwAutoComboFlags) 
	: CAutoComboBox(dwAutoComboFlags), m_dwFlags(dwFlags)
{
}

CColorComboBox::~CColorComboBox()
{
}


BEGIN_MESSAGE_MAP(CColorComboBox, CAutoComboBox)
	//{{AFX_MSG_MAP(CColorComboBox)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorComboBox message handlers

void CColorComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState, 
								  DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
{
	if (nItem == -1)
		return;

	CDlgUnits dlu(this, TRUE);
	CRect rColor(rect), rText(rect);
	
	// draw text
	if (!sItem.IsEmpty())
	{
		rColor.right = (rColor.left + rColor.Height());
		rText.left = rColor.right + 2;

		CAutoComboBox::DrawItemText(dc, rText, nItem, nItemState, dwItemData, sItem, bList, crText);
	}

	// draw color
	BOOL bHasColor = (dwItemData != CLR_NONE);

	if (bHasColor || Misc::HasFlag(m_dwFlags, CCBS_DRAWNOCOLOR))
	{
		COLORREF crFill, crBorder;
		GraphicsMisc::CalculateBoxColors(dwItemData, IsWindowEnabled(), crFill, crBorder);

		int nCornerRadius = ((m_dwFlags & CCBS_ROUNDRECT) ? (rColor.Width() / 4) : 0);
		
		GraphicsMisc::DrawRect(&dc, rColor, crFill, crBorder, nCornerRadius);
	}
}	

int CColorComboBox::AddColor(COLORREF color, LPCTSTR szDescription)
{
	ASSERT (GetStyle() & (CBS_OWNERDRAWFIXED | CBS_OWNERDRAWVARIABLE));

	if (szDescription)
		ASSERT (GetStyle() & CBS_HASSTRINGS);
	else
		szDescription = _T("");

	int nIndex = AddString(szDescription);

	if (nIndex != CB_ERR)
		SetItemData(nIndex, (DWORD)color);

	return nIndex;
}

int CColorComboBox::InsertColor(int nIndex, COLORREF color, LPCTSTR szDescription)
{
	ASSERT (GetStyle() & (CBS_OWNERDRAWFIXED | CBS_OWNERDRAWVARIABLE));

	if (szDescription)
		ASSERT (GetStyle() & CBS_HASSTRINGS);
	else
		szDescription = _T("");

	nIndex = InsertString(nIndex, szDescription);

	if (nIndex != CB_ERR)
		SetItemData(nIndex, (DWORD)color);

	return nIndex;
}

COLORREF CColorComboBox::SetColor(int nIndex, COLORREF color)
{
	ASSERT (GetStyle() & (CBS_OWNERDRAWFIXED | CBS_OWNERDRAWVARIABLE));
	ASSERT (nIndex >= 0 && nIndex < GetCount());

	COLORREF crOld = (COLORREF)GetItemData(nIndex);
	SetItemData(nIndex, (DWORD)color);

	if ((nIndex == GetCurSel()) || IsType(CBS_SIMPLE))
		Invalidate(FALSE);

	return crOld;
}

COLORREF CColorComboBox::SetColor(LPCTSTR szDescription, COLORREF color)
{
	int nFind = FindStringExact(0, szDescription);
	ASSERT(nFind != CB_ERR);

	return SetColor(nFind, color);
}
