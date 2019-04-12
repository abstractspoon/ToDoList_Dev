// orderededit.cpp : implementation file
//

#include "stdafx.h"
#include "..\NcGutterTest\NcGutterTest.h"
#include "orderededit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COrderedEdit

COrderedEdit::COrderedEdit()
{
	m_gutter.AddColumn(); // for the pos string
}

COrderedEdit::~COrderedEdit()
{
}


BEGIN_MESSAGE_MAP(COrderedEdit, CEdit)
	//{{AFX_MSG_MAP(COrderedEdit)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_NCG_GETFIRSTVISIBLETOPLEVELITEM, OnGutterGetFirstVisibleTopLevelItem)
	ON_REGISTERED_MESSAGE(WM_NCG_GETNEXTITEM, OnGutterGetNextItem)
	ON_REGISTERED_MESSAGE(WM_NCG_DRAWITEM, OnGutterDrawItem)
	ON_REGISTERED_MESSAGE(WM_NCG_RECALCCOLWIDTH, OnGutterRecalcColWidth)
	ON_REGISTERED_MESSAGE(WM_NCG_POSTNCDRAW, OnGutterPostNcDraw)
	ON_REGISTERED_MESSAGE(WM_NCG_GETITEMRECT, OnGutterGetItemRect)
	ON_REGISTERED_MESSAGE(WM_NCG_GETFIRSTCHILDITEM, OnGutterGetFirstChildItem)
	ON_REGISTERED_MESSAGE(WM_NCG_POSTDRAWITEM, OnGutterPostDrawItem)
	ON_REGISTERED_MESSAGE(WM_NCG_GETSELECTEDITEM, OnGutterGetSelectedItem)
	ON_REGISTERED_MESSAGE(WM_NCG_SETSELECTEDITEM, OnGutterSetSelectedItem)
	ON_REGISTERED_MESSAGE(WM_NCG_HITTEST, OnGutterHitTest)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrderedEdit message handlers

void COrderedEdit::PreSubclassWindow() 
{
	m_gutter.AddRecalcMessage(WM_CHAR);
//	m_gutter.AddRecalcMessage(TVM_INSERTITEM);
//	m_gutter.AddRecalcMessage(TVM_DELETEITEM);

//	m_gutter.AddRedrawMessage(TVM_SELECTITEM);
//	m_gutter.AddRedrawMessage(TVM_SORTCHILDREN);
//	m_gutter.AddRedrawMessage(TVM_SORTCHILDRENCB);

	m_gutter.Initialize(GetSafeHwnd());
	
	CEdit::PreSubclassWindow();
}

LRESULT COrderedEdit::OnGutterGetFirstVisibleTopLevelItem(WPARAM wParam, LPARAM lParam)
{
	int nFirstLine = GetFirstVisibleLine() + 1; // one based
	*((LPINT)lParam) = nFirstLine;

	return (LRESULT)nFirstLine;
}

LRESULT COrderedEdit::OnGutterGetNextItem(WPARAM wParam, LPARAM lParam)
{
	return min(GetLineCount(), lParam) + 1;
}

LRESULT COrderedEdit::OnGutterGetFirstChildItem(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT COrderedEdit::OnGutterDrawItem(WPARAM wParam, LPARAM lParam)
{
	NCGDRAWITEM* pNCGDI = (NCGDRAWITEM*)lParam;

	if (pNCGDI->nCol == 0)
	{
//		NcDrawItem(pNCGDI->pDC, pNCGDI->dwItem, pNCGDI->nCol, pNCGDI->rItem, pNCGDI->nLevel, 
//					pNCGDI->nPos, pNCGDI->sPrevPos, pNCGDI->rWindow);

		return TRUE; // we handled it
	}

	// else
	return FALSE;
}

LRESULT COrderedEdit::OnGutterPostDrawItem(WPARAM wParam, LPARAM lParam)
{
	NCGDRAWITEM* pNCGDI = (NCGDRAWITEM*)lParam;

//	PostNcDrawItem(pNCGDI->pDC, pNCGDI->dwItem, pNCGDI->rItem, pNCGDI->nLevel);

	return TRUE; // we handled it
}

LRESULT COrderedEdit::OnGutterPostNcDraw(WPARAM wParam, LPARAM lParam)
{
	NCGDRAWITEM* pNCGDI = (NCGDRAWITEM*)lParam;

//	PostNcDraw(pNCGDI->pDC, pNCGDI->rWindow);

	return TRUE; // we handled it
}

LRESULT COrderedEdit::OnGutterRecalcColWidth(WPARAM wParam, LPARAM lParam)
{
	return 15;//RecalcColumnWidth(lParam);
}

LRESULT COrderedEdit::OnGutterGetItemRect(WPARAM wParam, LPARAM lParam)
{
	NCGITEMRECT* pNCGGI = (NCGITEMRECT*)lParam;

	int nLine = (int)pNCGGI->dwItem - 1;
	int nChar = LineIndex(nLine);

	CPoint ptTopLeft = PosFromChar(nChar);

	pNCGGI->rItem.left = 0;
	pNCGGI->rItem.top = ptTopLeft.y;
	pNCGGI->rItem.right = 100; // not important
	pNCGGI->rItem.bottom = pNCGGI->rItem.top + 15;

	return TRUE;
}

LRESULT COrderedEdit::OnGutterGetSelectedItem(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT COrderedEdit::OnGutterSetSelectedItem(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT COrderedEdit::OnGutterHitTest(WPARAM wParam, LPARAM lParam)
{
	CPoint point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	
	int nChar = CharFromPos(point);
	int nLine = LineFromChar(nChar);

	return (LRESULT)nLine + 1;
}

