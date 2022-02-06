// StatusBarACTEx.cpp : implementation file
//

#include "stdafx.h"
#include "StatusBarACTEx.h"
#include "GraphicsMisc.h"
#include "enstring.h"
#include "themed.h"

#include "..\3rdParty\ColorDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatusBarACTEx

CStatusBarACTEx::CStatusBarACTEx()
{
}

CStatusBarACTEx::~CStatusBarACTEx()
{
}


BEGIN_MESSAGE_MAP(CStatusBarACTEx, CStatusBarACT)
	//{{AFX_MSG_MAP(CStatusBarACTEx)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatusBarACTEx message handlers

void CStatusBarACTEx::OnPaint()
{
	if ((m_crText == -1 && m_crFrom == -1 && m_crTo == -1) || !CThemed::IsAppThemed())
	{
		Default();
	}
	else
	{
		CPaintDC dc(this);
		CRect rClient;
		CFont* pOldFont = dc.SelectObject(GetFont());

		GetClientRect(rClient);
		DrawRectBkgnd(&dc, rClient);

		int nPane = GetStatusBarCtrl().GetParts(0, NULL);
		dc.SetTextColor(m_crText);
		dc.SetBkMode(TRANSPARENT);

		while (nPane--)
		{
			BOOL bTextDrawn = DrawPaneText(&dc, nPane, nPane ? 2 : 4);

			// prevent overwriting of next pane
			CRect rect;
			GetItemRect(nPane, &rect);

			dc.ExcludeClipRect(rect);

			// draw divider if the pane has any text or it's not autofit
			BOOL bAutoFit = (GetPaneFlagsIndex(nPane) & SBACTF_AUTOFIT);

			if (nPane && (bTextDrawn || !bAutoFit)) // ignore first pane
			{
				// pick appropriate color
				int nLum = RGBX(m_crFrom).Luminance();
				COLORREF color = (nLum < 128) ? GraphicsMisc::Lighter(m_crTo, 0.25) : GraphicsMisc::Darker(m_crTo, 0.4);

				GraphicsMisc::DrawVertLine(&dc, rect.top, rect.bottom, rect.left - 2, color);
			}
		}

		dc.SelectObject(pOldFont);

		// Draw size grip
		if (CThemed::IsAppThemed() && (GetStyle() & SBARS_SIZEGRIP))
		{
			CThemed th(this, _T("SCROLLBAR"));
			CRect rGrip(rClient);

			rGrip.left = rGrip.right - 24;
			rGrip.top = rGrip.bottom - 24;

			th.DrawBackground(&dc, SBP_SIZEBOX, SZB_RIGHTALIGN, rGrip);
		}
	}
}

BOOL CStatusBarACTEx::DrawPaneText(CDC* pDC, int nPane, int nOffset)
{
	const CString& sText = GetPaneText(nPane);

	if (sText.IsEmpty())
		return FALSE;

	CRect rect;
	GetItemRect(nPane, &rect);

	pDC->TextOut(rect.left + nOffset, rect.top, sText);
	return TRUE;
}

BOOL CStatusBarACTEx::OnEraseBkgnd(CDC* pDC)
{
	if ((m_crText == -1 && m_crFrom == -1 && m_crTo == -1) || !CThemed::IsAppThemed())
		return CStatusBar::OnEraseBkgnd(pDC);

	// else
	return TRUE;
}

void CStatusBarACTEx::DrawRectBkgnd(CDC* pDC, const CRect& rect)
{
	if (m_crTo == m_crFrom)
	{
		pDC->FillSolidRect(rect, m_crFrom);
	}
	else
	{
		GM_GRADIENT nType = GraphicsMisc::GetGradientType(m_bGlass, m_bGradient);

		GraphicsMisc::DrawGradient(nType, pDC, rect, m_crFrom, m_crTo, FALSE);
	}
}

void CStatusBarACTEx::SetUIColors(COLORREF crBackFrom, COLORREF crBackTo, COLORREF crText, BOOL bGradient, BOOL bGlass)
{
	m_crText = crText;
	m_crFrom = crBackFrom;

	if (crBackTo == CLR_NONE)
		m_crTo = m_crFrom;
	else
		m_crTo = crBackTo;

	m_bGradient = bGradient;
	m_bGlass = bGlass;

	if (GetSafeHwnd())
		Invalidate(TRUE);
}

BOOL CStatusBarACTEx::SetPaneTooltipIndex(int nIndex, UINT nTipID)
{
	return SetPaneTooltipIndex(nIndex, CEnString(nTipID));
}

BOOL CStatusBarACTEx::SetPaneTooltipIndex(int nIndex, const CString& sTip)
{
	if (sTip.GetLength() > 255)
		return CStatusBarACT::SetPaneTooltipIndex(nIndex, sTip.Left(255));

	return CStatusBarACT::SetPaneTooltipIndex(nIndex, sTip);
}

void CStatusBarACTEx::InvalidatePane(int nPane)
{
	CRect rect;
	GetItemRect(nPane, &rect);

	InvalidateRect(rect);
}
