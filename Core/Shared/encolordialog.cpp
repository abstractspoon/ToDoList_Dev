// encolordialog.cpp : implementation file
//

#include "stdafx.h"
#include "encolordialog.h"

#include "..\Interfaces\IPreferences.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const UINT ID_BASICCOLORS	= 0x02D0;
const UINT ID_CUSTOMCOLORS	= 0x02D1;

const int NUM_COLS			= 8;
const int NUM_BASICROWS		= 6;
const int NUM_CUSTOMROWS	= 2;

/////////////////////////////////////////////////////////////////////////////
// CEnColorDialog

IMPLEMENT_DYNAMIC(CEnColorDialog, CColorDialog)

CEnColorDialog::CEnColorDialog(COLORREF clrInit, DWORD dwFlags, CWnd* pParentWnd) 
	:
	CColorDialog(clrInit, dwFlags, pParentWnd)
{
	if (clrInit == CLR_NONE)
		m_cc.Flags &= ~CC_RGBINIT;
}

CEnColorDialog::~CEnColorDialog()
{
}

BEGIN_MESSAGE_MAP(CEnColorDialog, CColorDialog)
	//{{AFX_MSG_MAP(CEnColorDialog)
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

int CEnColorDialog::DoModal(IPreferences* pPrefs)
{
	if (pPrefs)
		LoadPreferences(pPrefs);

	int nRet = CColorDialog::DoModal();

	if ((nRet == IDOK) && pPrefs)
		SavePreferences(pPrefs);

	return nRet;
}

void CEnColorDialog::LoadPreferences(const IPreferences* pPrefs)
{
	// restore previously saved custom colors
	for (int nColor = 0; nColor < 16; nColor++)
	{
		CString sKey;
		sKey.Format(_T("CustomColor%d"), nColor);

		COLORREF color = (COLORREF)pPrefs->GetProfileInt(_T("ColorDialog"), sKey, (int)RGB(255, 255, 255));
		m_cc.lpCustColors[nColor] = color;
	}
}

void CEnColorDialog::SavePreferences(IPreferences* pPrefs) const
{
	// save any custom colors
	COLORREF* pColors = GetSavedCustomColors();

	for (int nColor = 0; nColor < 16; nColor++)
	{
		CString sKey;
		sKey.Format(_T("CustomColor%d"), nColor);

		int nColorVal = (int)pColors[nColor];
		pPrefs->WriteProfileInt(_T("ColorDialog"), sKey, nColorVal);
	}
}

void CEnColorDialog::SetCurrentColor(COLORREF clr)
{
	if (GetSafeHwnd())
	{
		CColorDialog::SetCurrentColor(clr);
	}
	else if (clr != CLR_NONE)
	{
		m_cc.rgbResult = clr;
		m_cc.Flags |= CC_RGBINIT;
	}
}

void CEnColorDialog::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CColorDialog::OnLButtonDblClk(nFlags, point);

	if (!HandleColorGridDblClk(point, ID_BASICCOLORS, NUM_BASICROWS))
		HandleColorGridDblClk(point, ID_CUSTOMCOLORS, NUM_CUSTOMROWS);
}

BOOL CEnColorDialog::HandleColorGridDblClk(CPoint point, UINT nCtrlID, int nNumRows)
{
	CWnd *pGrid = GetDlgItem(nCtrlID);
	ASSERT(pGrid);
	
	if (!pGrid)
		return FALSE;

	CRect rGrid;
	pGrid->GetClientRect(rGrid);

	ClientToScreen(&point);
	pGrid->ScreenToClient(&point);

	if (!rGrid.PtInRect(point))
		return FALSE;

	int nCXCell = (rGrid.Width() / NUM_COLS);
	int nCYCell = (rGrid.Height() / nNumRows);
	
	int nXCell = (point.x / nCXCell);
	int nYCell = (point.y / nCYCell);

	CRect rCell(CPoint(nXCell * nCXCell, nYCell * nCYCell), CSize(nCXCell, nCYCell));
	rCell.DeflateRect(4, 3);

	if (rCell.PtInRect(point))
		SendMessage(WM_COMMAND, MAKEWPARAM(IDOK, BN_CLICKED), (LPARAM)GetSafeHwnd());

	return TRUE;
}
