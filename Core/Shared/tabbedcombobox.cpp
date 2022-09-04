// tabbedcombobox.cpp : implementation file
//

#include "stdafx.h"
#include "tabbedcombobox.h"
#include "dlgunits.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabbedComboBox

CTabbedComboBox::CTabbedComboBox(int nTabStops) : TABSTOPS(nTabStops)
{
}

CTabbedComboBox::~CTabbedComboBox()
{
}


BEGIN_MESSAGE_MAP(CTabbedComboBox, COwnerdrawComboBoxBase)
	//{{AFX_MSG_MAP(CTabbedComboBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabbedComboBox message handlers

void CTabbedComboBox::DrawItemText(CDC& dc, const CRect& rect, int /*nItem*/, UINT /*nItemState*/,
									DWORD /*dwItemData*/, const CString& sItem, BOOL bList, COLORREF crText)
{
	if (crText != CLR_NONE)
		dc.SetTextColor(crText);

	// replace [\][t] with [\t]
	CString sText(sItem);
	sText.Replace(_T("\\t"), _T("\t"));

	if (TABSTOPS < 0)
	{
		dc.DrawText(sText, (LPRECT)(LPCRECT)rect, DT_EXPANDTABS);
	}
	else if (TABSTOPS == 0)
	{
		dc.TabbedTextOut(rect.left, rect.top, sText, 0, NULL, rect.left);
	}
	else
	{
		dc.TabbedTextOut(rect.left, rect.top, sText, 1, (int*)&TABSTOPS, rect.left);
	}
}

