// SizeGrip.cpp : implementation file
//

#include "stdafx.h"
#include "SizeGrip.h"

#include "imageprocessors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSizeGrip

CSizeGrip::CSizeGrip()
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
	if (!m_bm.GetSafeHandle())
	{
		Default();
	}
	else
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
}

BOOL CSizeGrip::Initialize(UINT nCtrlID, CWnd* pParent, UINT nBitmapID)
{
	if (!pParent || !pParent->GetSafeHwnd())
		return FALSE;

	CRect rCtrl;
	pParent->GetClientRect(rCtrl);
	rCtrl.left = rCtrl.right - 16;
	rCtrl.top = rCtrl.bottom - 16;

	if (!Create(WS_CHILD | WS_VISIBLE | SBS_SIZEBOX | SBS_SIZEBOXBOTTOMRIGHTALIGN, rCtrl, pParent, nCtrlID))
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
			m_bm.ProcessImage(&CImageSysColorMapper());
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

			CRect rCtrl(0, 0, LOWORD(lp), HIWORD(lp));
			rCtrl.left = rCtrl.right - 16;
			rCtrl.top = rCtrl.bottom - 16;

			MoveWindow(rCtrl);
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
