// custombutton.cpp : implementation file
//

#include "stdafx.h"
#include "custombutton.h"
#include "misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef WM_SHOWACCELERATORS
#define WM_SHOWACCELERATORS 0x0128
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomButton

CCustomButton::CCustomButton()
{
}

CCustomButton::~CCustomButton()
{
}


BEGIN_MESSAGE_MAP(CCustomButton, CButton)
	//{{AFX_MSG_MAP(CCustomButton)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_ENABLE()
	ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
	ON_MESSAGE(WM_SHOWACCELERATORS, OnShowAccelerators)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomButton message handlers

void CCustomButton::SetWindowText(LPCTSTR lpszString)
{
	CButton::SetWindowText(lpszString);
}

void CCustomButton::SetWindowText(TCHAR nChar)
{
	TCHAR szString[2] = { nChar, 0 };
	SetWindowText(szString);
}

void CCustomButton::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// create a temp dc to paint on
	CDC dcTemp;

	if (dcTemp.CreateCompatibleDC(&dc))
	{
		CBitmap bmTemp;
		CRect rClient;

		GetClientRect(rClient);

		if (bmTemp.CreateCompatibleBitmap(&dc, rClient.right, rClient.bottom))
		{
			// draw extra on top
			CRect rExtra;
			CalcExtraSpace(rClient, rExtra);

			CBitmap* pOld = dcTemp.SelectObject(&bmTemp);
			
			// default draw to temp dc
			DefWindowProc(WM_PAINT, (WPARAM)(HDC)dcTemp, 0);
			
			// extra draw
			DoExtraPaint(&dcTemp, rExtra);
			
			// blit to screen
			dc.BitBlt(0, 0, rClient.right, rClient.bottom, &dcTemp, 0, 0, SRCCOPY);
			
			// cleanup
			dcTemp.SelectObject(pOld);
			
			return;
		}
	}
	
	// else draw to default dc
	DefWindowProc(WM_PAINT, (WPARAM)(HDC)dc, 0);
}

void CCustomButton::CalcExtraSpace(const CRect& rClient, CRect& rExtra) const
{
	rExtra = rClient;
	rExtra.DeflateRect(5, 4, 5, 5);

	DWORD dwStyle = GetStyle();

	if (dwStyle & BS_RIGHT)
	{
		rExtra.right = rExtra.left + max(12, rExtra.Height());
	}
	else // left/centre aligned
	{
		rExtra.left = rExtra.right - max(12, rExtra.Height());
	}
}

void CCustomButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	Invalidate();
	UpdateWindow();

	CButton::OnLButtonUp(nFlags, point);
}

void CCustomButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CButton::OnLButtonDown(nFlags, point);

	Invalidate();
	UpdateWindow();
}

void CCustomButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	CButton::OnMouseMove(nFlags, point);

	Invalidate();
	UpdateWindow();
}

void CCustomButton::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CButton::OnLButtonDblClk(nFlags, point);

	Invalidate();
	UpdateWindow();
}

void CCustomButton::OnKillFocus(CWnd* pNewWnd) 
{
	CButton::OnKillFocus(pNewWnd);
	
	Invalidate();
	UpdateWindow();
}

void CCustomButton::OnSetFocus(CWnd* pOldWnd) 
{
	CButton::OnSetFocus(pOldWnd);
	
	Invalidate();
	UpdateWindow();
}

void CCustomButton::OnEnable(BOOL bEnable) 
{
	CButton::OnEnable(bEnable);
	
	Invalidate();
	UpdateWindow();
}

BOOL CCustomButton::OnClicked() 
{
	Invalidate();
	UpdateWindow();

	if (DoAction())
	{
		Invalidate();
		UpdateWindow();

		return TRUE; // no need to bother parent
	}

	return FALSE; // pass to parent
}

LRESULT CCustomButton::OnShowAccelerators(WPARAM /*wp*/, LPARAM /*lp*/)
{
	LRESULT lr = Default();
	RedrawWindow();

	return lr;
}

void CCustomButton::SetTooltip(LPCTSTR lpszString)
{
	if (!Misc::IsEmpty(lpszString))
	{
		if (!m_tooltip.GetSafeHwnd())
			VERIFY(m_tooltip.Create(this, (TTS_NOPREFIX | TTS_ALWAYSTIP)));
		else
			m_tooltip.DelTool(this);
		
		m_tooltip.AddTool(this, lpszString);
	}
	else
	{
		m_tooltip.DestroyWindow();
	}
}

BOOL CCustomButton::PreTranslateMessage(MSG* pMsg)
{
	if (m_tooltip.GetSafeHwnd())
		m_tooltip.RelayEvent(pMsg);
	
	return CButton::PreTranslateMessage(pMsg);
}
