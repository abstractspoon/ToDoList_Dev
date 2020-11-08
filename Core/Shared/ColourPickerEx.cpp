// ColourPickerEx.cpp : implementation file
//

#include "stdafx.h"
#include "ColourPickerEx.h"
#include "ColourPopupEx.h"

#include "themed.h"
#include "GraphicsMisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColourPickerEx

CColourPickerEx::CColourPickerEx()
{
}

CColourPickerEx::~CColourPickerEx()
{
}


BEGIN_MESSAGE_MAP(CColourPickerEx, CColourPicker)
	//{{AFX_MSG_MAP(CColourPickerEx)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CColourPickerEx message handlers

void CColourPickerEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	ASSERT(lpDrawItemStruct);

	CDC*    pDC     = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect   rect    = lpDrawItemStruct->rcItem;
	UINT    state   = lpDrawItemStruct->itemState;
	CString strText;

	CSize Margins(::GetSystemMetrics(SM_CXEDGE), ::GetSystemMetrics(SM_CYEDGE));

	// border
	CThemed th(this, _T("EDIT"));

	if (th.AreControlsThemed())
		th.DrawBackground(pDC, EP_EDITTEXT, ETS_NORMAL, rect);
	else
		pDC->DrawEdge(rect, EDGE_SUNKEN, BF_RECT);

	// Draw arrow
	if (m_bActive) 
		state |= ODS_SELECTED;

	CThemed::DrawFrameControl(this, pDC, &m_ArrowRect, DFC_SCROLL, DFCS_SCROLLDOWN  | 
							((state & ODS_SELECTED) ? DFCS_PUSHED : 0) |
							((state & ODS_DISABLED) ? DFCS_INACTIVE : 0));

	// Must reduce the size of the "client" area of the button due to edge thickness.
	rect.DeflateRect(Margins.cx, Margins.cy);

	// figure out text and backgnd colours
	rect.right -= m_ArrowRect.Width() + 1;

	BOOL bEnabled = IsWindowEnabled();
	COLORREF crWindow = ::GetSysColor(bEnabled ? COLOR_WINDOW : COLOR_3DFACE);

	COLORREF crBack = (m_crColourBk == CLR_DEFAULT || !bEnabled) ? crWindow : m_crColourBk;
	COLORREF crText = m_crColourText;

	if (!bEnabled)
	{
		crText = GetSysColor(COLOR_GRAYTEXT); 
	}
	else if (crText == CLR_DEFAULT)
	{
		if (m_nSelectionMode == CP_MODE_BK)
			crText = GraphicsMisc::GetBestTextColor(crBack);
		else
			crText = GetSysColor(COLOR_WINDOWTEXT);
	}

	GetWindowText(strText);

	if (m_nSelectionMode == CP_MODE_BK)
		rect.DeflateRect(1, 1); // provide a border

	// draw required color
	pDC->FillSolidRect(rect, crBack);

	// draw text
	if (strText.GetLength())
	{
		// draw text in required color
		pDC->SetBkMode(TRANSPARENT);

		COLORREF crOld = pDC->SetTextColor(crText);
		pDC->DrawText(strText, rect, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
		pDC->SetTextColor(crOld);

		rect.DeflateRect(1, 1); // provide a border
	}

	// Draw focus rect
	if ((state & ODS_FOCUS) && !m_bActive) 
		pDC->DrawFocusRect(rect);
}

CColourPopup* CColourPickerEx::NewColourPopup(CPoint pt) 
{ 
	return new CColourPopupEx(pt,					// Point to display popup
								GetColour(),		// Selected colour
								this,				// parent
								0,					// nID
								m_strDefaultText,   // "Default" text area
								m_strCustomText); 
}
