// SizeGrip.cpp : implementation file
//

#include "stdafx.h"
#include "SizeGrip.h"

#include "imageprocessors.h"
#include "Themed.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSizeGrip

CSizeGrip::CSizeGrip() : m_crBack(CLR_NONE)
{
}

CSizeGrip::~CSizeGrip()
{
}

BEGIN_MESSAGE_MAP(CSizeGrip, CScrollBar)
	//{{AFX_MSG_MAP(CSizeGrip)
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSizeGrip message handlers

void CSizeGrip::OnPaint() 
{
	if (m_bm.GetSafeHandle())
	{
		CPaintDC dc(this); // device context for painting

		BITMAP BM;
		m_bm.GetBitmap(&BM);

		CDC dcMem;

		if (dcMem.CreateCompatibleDC(&dc))
		{
			CBitmap* pOld = dcMem.SelectObject(&m_bm);

			dc.BitBlt(0, 0, BM.bmWidth, BM.bmHeight, &dcMem, 0, 0, SRCCOPY);

			dcMem.SelectObject(pOld);
		}
	}
	else if (CThemed::IsAppThemed() && (m_crBack != CLR_NONE))
	{
		CPaintDC dc(this); // device context for painting
		CThemed th(this, _T("SCROLLBAR"));

		CRect rGrip;
		GetClientRect(rGrip);

		dc.FillSolidRect(rGrip, m_crBack);
		th.DrawBackground(&dc, SBP_SIZEBOX, SZB_RIGHTALIGN, rGrip);
	}	
	else
	{
		Default();
	}
}

void CSizeGrip::SetBackgroundColor(COLORREF crBack)
{
	m_crBack = crBack;

	if (CThemed::IsAppThemed() && GetSafeHwnd())
		Invalidate(TRUE);
}

BOOL CSizeGrip::Initialize(CWnd* pParent, UINT nCtrlID, UINT nBitmapID)
{
	if (!pParent || !pParent->GetSafeHwnd())
		return FALSE;

	CRect rParent;
	pParent->GetClientRect(rParent);

	CRect rGrip;
	CalcRect(rParent.Width(), rParent.Height(), rGrip);

	if (!Create(WS_CHILD | WS_VISIBLE | SBS_SIZEBOX | SBS_SIZEBOXBOTTOMRIGHTALIGN | WS_CLIPSIBLINGS, rGrip, pParent, nCtrlID))
		return FALSE;

	if (!m_scParent.HookWindow(*pParent, this))
	{
		DestroyWindow();
		return FALSE;
	}

	ShowScrollBar();

	if (nBitmapID)
	{
		if (m_bm.LoadBitmap(nBitmapID))
			m_bm.RemapSysColors();
	}

	return TRUE;
}

LRESULT CSizeGrip::ScWindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_SIZE:
		{
			ShowScrollBar(!(wp & SIZE_MAXIMIZED));

			CRect rGrip;
			CalcRect(LOWORD(lp), HIWORD(lp), rGrip);

			MoveWindow(rGrip);
		}
		break;
	}

	return CSubclasser::ScDefault(m_scParent);
}

BOOL CSizeGrip::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	HCURSOR hCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE);

	if (hCursor)
	{
		::SetCursor(hCursor);	
		return TRUE;
	}

	return CScrollBar::OnSetCursor(pWnd, nHitTest, message);
}

void CSizeGrip::CalcRect(int nParentWidth, int nParentHeight, CRect& rGrip) const
{
	rGrip.left = (nParentWidth - GetSystemMetrics(SM_CXVSCROLL));
	rGrip.top = (nParentHeight - GetSystemMetrics(SM_CYVSCROLL));
	rGrip.right = nParentWidth;
	rGrip.bottom = nParentHeight;
}
