// deferWndMove.cpp: implementation of the CDeferWndMove class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "deferWndMove.h"
#include "dialoghelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDeferWndMove::CDeferWndMove(int nNumWindows) : m_hdwp(NULL)
{
	if (nNumWindows)
		BeginMove(nNumWindows);
}

CDeferWndMove::~CDeferWndMove()
{
	if (m_hdwp)
 		EndMove();
}

BOOL CDeferWndMove::BeginMove(int nNumWindows)
{
	ASSERT (nNumWindows > 0);

	if (m_hdwp)
		EndMove();

	if (nNumWindows > 0)
		m_hdwp = ::BeginDeferWindowPos(nNumWindows);

	return (m_hdwp != NULL);
}

BOOL CDeferWndMove::EndMove()
{
	ASSERT (m_hdwp);

	BOOL bRes = ::EndDeferWindowPos(m_hdwp);
	m_hdwp = NULL;

	return bRes;
}

BOOL CDeferWndMove::MoveWindow(CWnd* pWnd, int x, int y, int nWidth, int nHeight, BOOL bRepaint)
{
	ASSERT (pWnd->GetParent());
	
	// figure out what flags we need
	int nFlags = SWP_NOACTIVATE | SWP_NOZORDER;
	CRect rWnd;
	
	pWnd->GetWindowRect(rWnd);
	pWnd->GetParent()->ScreenToClient(rWnd);
	
	if (x == rWnd.left && y == rWnd.top)
		nFlags |= SWP_NOMOVE;
	
	if ((nWidth == rWnd.Width()) && (nHeight == rWnd.Height()))
		nFlags |= SWP_NOSIZE;
	
	BOOL bNeedMove = !((nFlags & SWP_NOMOVE) && (nFlags & SWP_NOSIZE));
	
	if (bNeedMove)
	{
		if (!bRepaint)
			nFlags |= SWP_NOREDRAW;
		
		HWND hWnd = pWnd->GetSafeHwnd();
		
		m_hdwp = ::DeferWindowPos(m_hdwp, hWnd, NULL, x, y, nWidth, nHeight, nFlags);
		
		// fallback
		if (!m_hdwp)
		{
	//		ASSERT (0); // shouldn't fail but it might
			::SetWindowPos(hWnd, NULL, x, y, nWidth, nHeight, nFlags);
		}
	}
	
	return TRUE;
}

BOOL CDeferWndMove::MoveWindow(CWnd* pWnd, LPCRECT lpRect, BOOL bRepaint)
{
	return MoveWindow(pWnd, lpRect->left, lpRect->top, 
							      lpRect->right - lpRect->left,
							      lpRect->bottom - lpRect->top, bRepaint);
}

CRect CDeferWndMove::OffsetCtrl(const CWnd* pParent, UINT nCtrlID, int x, int y)
{
	return OffsetCtrl(pParent, pParent->GetDlgItem(nCtrlID), x, y);
}

CRect CDeferWndMove::MoveCtrl(const CWnd* pParent, UINT nCtrlID, int x, int y)
{
	return MoveCtrl(pParent, pParent->GetDlgItem(nCtrlID), x, y);
}

CRect CDeferWndMove::ResizeCtrl(const CWnd* pParent, UINT nCtrlID, int cx, int cy)
{
	return ResizeCtrl(pParent, pParent->GetDlgItem(nCtrlID), cx, cy);
}

CRect CDeferWndMove::OffsetChild(CWnd* pChild, int x, int y)
{
	return OffsetCtrl(pChild->GetParent(), pChild, x, y);
}

CRect CDeferWndMove::ResizeChild(CWnd* pChild, int cx, int cy)
{
	return ResizeCtrl(pChild->GetParent(), pChild, cx, cy);
}

CRect CDeferWndMove::MoveChild(CWnd* pChild, int x, int y)
{
	return MoveCtrl(pChild->GetParent(), pChild, x, y);
}

CRect CDeferWndMove::OffsetCtrl(const CWnd* pParent, CWnd* pCtrl, int x, int y)
{
	if (pParent && pCtrl)
	{
		CRect rChild = CDialogHelper::GetChildRect(pCtrl);

		if (x || y)
		{
			rChild.OffsetRect(x, y);
			MoveWindow(pCtrl, rChild); // our own version
		}

		return rChild;
	}

	ASSERT(0);
	return CRect(0, 0, 0, 0);
}

CRect CDeferWndMove::MoveCtrl(const CWnd* pParent, CWnd* pCtrl, int x, int y)
{
	if (pCtrl)
	{
		CRect rChild = CDialogHelper::GetChildRect(pCtrl);

		rChild.OffsetRect(x - rChild.left, y - rChild.top);
		MoveWindow(pCtrl, rChild); // our own version

		return rChild;
	}

	ASSERT(0);
	return CRect(0, 0, 0, 0);
}

CRect CDeferWndMove::ResizeCtrl(const CWnd* pParent, CWnd* pCtrl, int cx, int cy)
{
	if (pCtrl)
	{
		CRect rChild = CDialogHelper::GetChildRect(pCtrl);

		if (cx || cy)
		{
			rChild.right += cx;
			rChild.bottom += cy;

			// make sure it also intersects with parent
			CRect rParent;
			pParent->GetClientRect(rParent);

			if (rChild.IntersectRect(rChild, rParent))
				MoveWindow(pCtrl, rChild); // our own version
		}

		return rChild;
	}

	ASSERT(0);
	return CRect(0, 0, 0, 0);
}
