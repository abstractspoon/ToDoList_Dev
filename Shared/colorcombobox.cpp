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
								  DWORD dwItemData, const CString& sItem, BOOL bList)
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

		CAutoComboBox::DrawItemText(dc, rText, nItem, nItemState, dwItemData, sItem, bList);
	}

	// draw color
	BOOL bHasColor = (dwItemData != CLR_NONE);

	if (bHasColor || Misc::HasFlag(m_dwFlags, CCBS_DRAWNOCOLOR))
	{
		COLORREF crFill = CLR_NONE, crBorder = GetSysColor(COLOR_3DDKSHADOW);
		
		if (!IsWindowEnabled() || (nItemState & ODS_GRAYED))
		{
			if (bHasColor)
				crFill = GetSysColor(COLOR_3DFACE);

			crBorder = GetSysColor(COLOR_3DSHADOW);
		}
		else if (dwItemData != CLR_NONE)
		{
			crFill = (COLORREF)dwItemData;
			crBorder = GraphicsMisc::Darker(crFill, 0.5);
		}
		
		int nCornerRadius = 0;
		
		if (m_dwFlags & CCBS_ROUNDRECT)
			nCornerRadius = (rColor.Width() / 4);
		
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
