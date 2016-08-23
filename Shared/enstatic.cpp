// enstatic.cpp : implementation file
//

#include "stdafx.h"
#include "enstatic.h"
#include "themed.h"
#include "graphicsmisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnStatic

const int GRIPPERSIZE = 16;

CEnStatic::CEnStatic(BOOL bEnableGripper) 
	: m_bGripper(bEnableGripper ? 1 : 0), m_crBackFrom(CLR_NONE), m_crBackTo(CLR_NONE), m_crText(CLR_NONE)
{
}

CEnStatic::~CEnStatic()
{
}


BEGIN_MESSAGE_MAP(CEnStatic, CStatic)
	//{{AFX_MSG_MAP(CEnStatic)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_NCHITTEST()
	ON_WM_ERASEBKGND()
	ON_WM_WINDOWPOSCHANGING()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnStatic message handlers

BOOL CEnStatic::SetFontStyle(BOOL bBold, BOOL bItalic, BOOL bUnderline, BOOL bStrikeThru)
{
	DWORD dwStyle = 0;
	
	if (bBold)
		dwStyle |= GMFS_BOLD;
	
	if (bItalic)
		dwStyle |= GMFS_ITALIC;
	
	if (bUnderline)
		dwStyle |= GMFS_UNDERLINED;
	
	if (bStrikeThru)
		dwStyle |= GMFS_STRIKETHRU;

	HFONT hRefFont = (HFONT)::SendMessage(::GetParent(m_hWnd), WM_GETFONT, 0, 0);

	if (dwStyle)
	{
		if (m_font.GetSafeHandle())
			::SendMessage(m_hWnd, WM_SETFONT, (WPARAM)hRefFont, TRUE);

		if (!GraphicsMisc::CreateFont(m_font, hRefFont, dwStyle))
			return FALSE;

		SetFont(&m_font, TRUE);
		return TRUE;
	}

	// else
	if (m_font.GetSafeHandle())
	{
		::SendMessage(m_hWnd, WM_SETFONT, (WPARAM)hRefFont, TRUE);
		m_font.DeleteObject();
	}
	
	return TRUE;
}

void CEnStatic::EnableGripper(BOOL bEnable)
{
	bEnable = bEnable ? 1 : 0;

	if (GetSafeHwnd() && m_bGripper != bEnable)
		Invalidate();

	m_bGripper = bEnable;
}

void CEnStatic::OnPaint() 
{
	if (IsShowingGripper() || HasBackColor() || HasTextColor())
	{
		CPaintDC dc(this); // device context for painting

		if (DrawColorBkgnd(&dc))
		{
			// render text also
			CString sText;
			GetWindowText(sText);
			
			if (!sText.IsEmpty())
			{
				CRect rText;
				GetClientRect(rText);
				rText.DeflateRect(6, 0); // padding

				CFont* pOldFont = GraphicsMisc::PrepareDCFont(&dc, GetSafeHwnd());
				UINT nFlags = (DT_VCENTER | DT_SINGLELINE | GetHorzTextAlignment());

				if (HasTextColor())
					dc.SetTextColor(m_crText);

				dc.SetBkMode(TRANSPARENT);
				dc.DrawText(sText, rText, nFlags);
				dc.SelectObject(pOldFont);
			}
		}
		else // default
		{
			if (HasTextColor())
				dc.SetTextColor(m_crText);

			DefWindowProc(WM_PAINT, (WPARAM)(HDC)dc, 0);
		}

		// then the gripper
		if (IsShowingGripper())
			CThemed::DrawFrameControl(this, &dc, GetGripperRect(), DFC_SCROLL, DFCS_SCROLLSIZEGRIP);
	}
	else
	{
		Default();
	}
}

int CEnStatic::GetHorzTextAlignment() const
{
	DWORD dwStyle = GetStyle();
	
	if (dwStyle & SS_RIGHT)
		return DT_RIGHT;

	if (dwStyle & SS_CENTER)
		return DT_CENTER;

	// else
	return DT_LEFT;
}

BOOL CEnStatic::IsShowingGripper() const
{
	if (m_bGripper)
	{
		int nType = (GetStyle() & SS_TYPEMASK);

		switch (nType)
		{
		case SS_LEFT:          
		case SS_CENTER:
		case SS_RIGHT:
		case SS_SIMPLE:
		case SS_LEFTNOWORDWRAP:
			return TRUE;
		}
	}

	return FALSE;
}

void CEnStatic::OnSize(UINT nType, int cx, int cy) 
{
	if (IsShowingGripper())
		Invalidate();
	
	CStatic::OnSize(nType, cx, cy);
}

#if _MSC_VER >= 1400
LRESULT CEnStatic::OnNcHitTest(CPoint point)
#else
UINT CEnStatic::OnNcHitTest(CPoint point)
#endif
{
	if (IsShowingGripper())
	{
		CRect rClient;
		GetClientRect(rClient);

		rClient.left = rClient.right - GRIPPERSIZE;
		rClient.top = rClient.bottom - GRIPPERSIZE;

		ClientToScreen(rClient);

		if (rClient.PtInRect(point))
			return HTBOTTOMRIGHT;
	}
	
	return CStatic::OnNcHitTest(point);
}

BOOL CEnStatic::OnEraseBkgnd(CDC* pDC) 
{
	if (!HasBackColor())
	{
		CStatic::OnEraseBkgnd(pDC);

		// draw gripper here too
		if (IsShowingGripper())
			pDC->DrawFrameControl(GetGripperRect(), DFC_SCROLL, DFCS_SCROLLSIZEGRIP);
	}

	return TRUE;
}

BOOL CEnStatic::DrawColorBkgnd(CDC* pDC)
{
	if (HasBackColor())
	{
		CRect rClient;
		GetClientRect(rClient);
		
		if (HasVaryingBackColor())
		{
			GM_GRADIENT nType = GraphicsMisc::GetGradientType(m_bGlass, m_bGradient);

			return GraphicsMisc::DrawGradient(nType, pDC, rClient, m_crBackFrom, m_crBackTo, m_bHorz);
		}

		// else
		pDC->FillSolidRect(rClient, m_crBackFrom);
		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CEnStatic::HasTextColor() const
{
	return (m_crText != CLR_NONE);
}

BOOL CEnStatic::HasBackColor() const
{
	return (m_crBackFrom != CLR_NONE);
}

LRESULT CEnStatic::OnSetText(WPARAM /*wp*/, LPARAM /*lp*/)
{
	LRESULT lr = Default();

	if (HasBackColor() || HasTextColor())
	{
		Invalidate(TRUE);
		UpdateWindow();
	}

	return lr;
}

BOOL CEnStatic::HasVaryingBackColor() const
{
	return (HasBackColor() && (m_crBackTo != m_crBackFrom));
}

CRect CEnStatic::GetGripperRect() const
{
	if (IsShowingGripper())
	{
		CRect rClient;
		GetClientRect(rClient);

		rClient.left = rClient.right - GRIPPERSIZE;
		rClient.top = rClient.bottom - GRIPPERSIZE;

		return rClient;
	}
	else
		return CRect(0, 0, 0, 0);
}

void CEnStatic::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CStatic::OnWindowPosChanging(lpwndpos);
	
	InvalidateRect(GetGripperRect(), FALSE);
}

void CEnStatic::SetColors(COLORREF crText, COLORREF crBkFrom, COLORREF crBkTo, BOOL bGlass, BOOL bGradient, BOOL bHorz)
{
	m_bGlass = bGlass;
	m_bGradient = bGradient;
	m_bHorz = bHorz;

	m_crText = crText;
	m_crBackFrom = crBkFrom;

	if ((!bGlass && !bGradient) || (crBkTo == CLR_NONE))
		m_crBackTo = crBkFrom;
	else
		m_crBackTo = crBkTo;

	if (GetSafeHwnd())
		Invalidate(TRUE);
}
