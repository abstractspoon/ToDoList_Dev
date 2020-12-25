// ColourPicker.cpp : implementation file
//
// ColourPicker is a drop-in colour picker control. Check out the 
// header file or the accompanying HTML doc file for details.
//
// Written by Chris Maunder (chrismaunder@codeguru.com)
// Extended by Alexander Bischofberger (bischofb@informatik.tu-muenchen.de)
// Copyright (c) 1998.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. If 
// the source code in  this file is used in any commercial application 
// then a simple email would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to your
// computer, causes your pet cat to fall ill, increases baldness or
// makes you car start emitting strange noises when you start it up.
//
// Expect bugs.
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 
//
// Updated 16 May 1998
//         31 May 1998 - added Default text (CJM)
//          9 Jan 1999 - minor vis update

#include "stdafx.h"
#include "ColourPopup.h"
#include "ColourPicker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void AFXAPI DDX_ColourPicker(CDataExchange *pDX, int nIDC, COLORREF& crColour)
{
    HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
    ASSERT (hWndCtrl != NULL);                
    
    CColourPicker* pColourPicker = (CColourPicker*) CWnd::FromHandle(hWndCtrl);
    if (pDX->m_bSaveAndValidate)
    {
        crColour = pColourPicker->GetColour();
    }
    else // initializing
    {
        pColourPicker->SetColour(crColour);
    }
}

/////////////////////////////////////////////////////////////////////////////
// CColourPicker

CColourPicker::CColourPicker() : m_crColourBk(CLR_DEFAULT), m_crColourText(CLR_DEFAULT), m_pPopup(NULL)
{
    m_bTrackSelection = FALSE;
    m_nSelectionMode = CP_MODE_BK;
    m_bActive = FALSE;

    m_strDefaultText = _T("Automatic");
    m_strCustomText  = _T("More Colours...");
}

CColourPicker::~CColourPicker()
{
}

IMPLEMENT_DYNCREATE(CColourPicker, CButton)

BEGIN_MESSAGE_MAP(CColourPicker, CButton)
    //{{AFX_MSG_MAP(CColourPicker)
    ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
    ON_WM_CREATE()
	ON_WM_SYSKEYDOWN()
	//}}AFX_MSG_MAP
    ON_MESSAGE(CPN_SELENDOK,     OnSelEndOK)
    ON_MESSAGE(CPN_SELENDCANCEL, OnSelEndCancel)
    ON_MESSAGE(CPN_SELCHANGE,    OnSelChange)
	ON_WM_SIZE()
	ON_WM_GETDLGCODE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColourPicker message handlers

LONG CColourPicker::OnSelEndOK(UINT lParam, LONG /*wParam*/)
{
    m_bActive = FALSE;
	m_pPopup = NULL;
 
	COLORREF crNewColour = (COLORREF) lParam;
    SetColour(crNewColour);

    CWnd *pParent = GetParent();
    if (pParent) {
        pParent->SendMessage(CPN_CLOSEUP, lParam, (WPARAM) GetDlgCtrlID());
        pParent->SendMessage(CPN_SELENDOK, lParam, (WPARAM) GetDlgCtrlID());
    }

    if (crNewColour != GetColour())
        if (pParent) pParent->SendMessage(CPN_SELCHANGE, lParam, (WPARAM) GetDlgCtrlID());

    Invalidate();

    return TRUE;
}

LONG CColourPicker::OnSelEndCancel(UINT lParam, LONG /*wParam*/)
{
    m_bActive = FALSE;
	m_pPopup = NULL;
 
	SetColour((COLORREF) lParam);

    CWnd *pParent = GetParent();
    if (pParent) {
        pParent->SendMessage(CPN_CLOSEUP, lParam, (WPARAM) GetDlgCtrlID());
        pParent->SendMessage(CPN_SELENDCANCEL, lParam, (WPARAM) GetDlgCtrlID());
    }

    Invalidate();

    return TRUE;
}

LONG CColourPicker::OnSelChange(UINT lParam, LONG /*wParam*/)
{
    if (m_bTrackSelection) SetColour((COLORREF) lParam);

    CWnd *pParent = GetParent();
    if (pParent) pParent->SendMessage(CPN_SELCHANGE, lParam, (WPARAM) GetDlgCtrlID());

    return TRUE;
}

int CColourPicker::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CButton::OnCreate(lpCreateStruct) == -1)
        return -1;
    
    SetWindowSize(TRUE);    // resize appropriately
    return 0;
}

// On mouse click, create and show a CColourPopup window for colour selection
BOOL CColourPicker::OnClicked()
{
    DoColourPopup();

    return TRUE;
}

void CColourPicker::DoColourPopup()
{
	if (m_bActive)
		return;

    m_bActive = TRUE;
    CRect rect;
    GetWindowRect(rect);
    m_pPopup = NewColourPopup(CPoint(rect.left, rect.bottom));

    CWnd *pParent = GetParent();
    if (pParent)
        pParent->SendMessage(CPN_DROPDOWN, (LPARAM)GetColour(), (WPARAM) GetDlgCtrlID());
}

CColourPopup* CColourPicker::NewColourPopup(CPoint pt) 
{ 
	return new CColourPopup(pt,					// Point to display popup
							GetColour(),		// Selected colour
							this,				// parent
							0,					// nID
							m_strDefaultText,   // "Default" text area
							m_strCustomText); 
}

void CColourPicker::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	ASSERT(lpDrawItemStruct);

	CDC*    pDC     = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect   rect    = lpDrawItemStruct->rcItem;
	UINT    state   = lpDrawItemStruct->itemState;
	CString m_strText;

	CSize Margins(::GetSystemMetrics(SM_CXEDGE), ::GetSystemMetrics(SM_CYEDGE));

	// Draw arrow
	if (m_bActive) state |= ODS_SELECTED;
	pDC->DrawFrameControl(&m_ArrowRect, DFC_SCROLL, DFCS_SCROLLDOWN  | 
		((state & ODS_SELECTED) ? DFCS_PUSHED : 0) |
		((state & ODS_DISABLED) ? DFCS_INACTIVE : 0));

	pDC->DrawEdge(rect, EDGE_SUNKEN, BF_RECT);

	// Must reduce the size of the "client" area of the button due to edge thickness.
	rect.DeflateRect(Margins.cx, Margins.cy);

	// Fill remaining area with colour
	rect.right -= m_ArrowRect.Width();

	CBrush brush( ((state & ODS_DISABLED) || m_crColourBk == CLR_DEFAULT)? 
		::GetSysColor(COLOR_3DFACE) : m_crColourBk);
	CBrush* pOldBrush = (CBrush*) pDC->SelectObject(&brush);
	pDC->SelectStockObject(NULL_PEN);
	pDC->Rectangle(rect);
	pDC->SelectObject(pOldBrush);

	// Draw the window text (if any)
	GetWindowText(m_strText);
	if (m_strText.GetLength())
	{
		pDC->SetBkMode(TRANSPARENT);
		if (state & ODS_DISABLED)
		{
			rect.OffsetRect(1,1);
			pDC->SetTextColor(::GetSysColor(COLOR_3DHILIGHT));
			pDC->DrawText(m_strText, rect, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
			rect.OffsetRect(-1,-1);
			pDC->SetTextColor(::GetSysColor(COLOR_3DSHADOW));
			pDC->DrawText(m_strText, rect, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
		}
		else
		{
			pDC->SetTextColor((m_crColourText == CLR_DEFAULT)? 0 : m_crColourText);
			pDC->DrawText(m_strText, rect, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
		}
	}

	// Draw focus rect
	if (state & ODS_FOCUS) 
	{
		rect.DeflateRect(1,1);
		pDC->DrawFocusRect(rect);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CColourPicker overrides

void CColourPicker::PreSubclassWindow() 
{
    ModifyStyle(0, BS_OWNERDRAW);        // Make it owner drawn
    CButton::PreSubclassWindow();
    SetWindowSize(TRUE);                     // resize appropriately
}

/////////////////////////////////////////////////////////////////////////////
// CColourPicker attributes

COLORREF CColourPicker::GetColour() const
{ 
    return (m_nSelectionMode == CP_MODE_TEXT)? 
        GetTextColour(): GetBkColour(); 
}

void CColourPicker::SetColour(COLORREF crColour)
{ 
    (m_nSelectionMode == CP_MODE_TEXT)? 
        SetTextColour(crColour): SetBkColour(crColour); 
}

void CColourPicker::SetBkColour(COLORREF crColourBk)
{
	if (m_crColourBk != crColourBk)
	{
		m_crColourBk = crColourBk;
	
		if (IsWindow(m_hWnd))
			Invalidate();
	}
}

void CColourPicker::SetTextColour(COLORREF crColourText)
{
    if (m_crColourText != crColourText)
	{
		m_crColourText = crColourText;

		if (IsWindow(m_hWnd)) 
			Invalidate();
	}
}

void CColourPicker::SetDefaultText(LPCTSTR szDefaultText)
{
    m_strDefaultText = (szDefaultText)? szDefaultText : _T("");
}

void CColourPicker::SetCustomText(LPCTSTR szCustomText)
{
    m_strCustomText = (szCustomText)? szCustomText : _T("");
}

/////////////////////////////////////////////////////////////////////////////
// CColourPicker implementation

void CColourPicker::SetWindowSize(BOOL bDefault)
{
    // Get size dimensions of edges
    CSize MarginSize(::GetSystemMetrics(SM_CXEDGE), ::GetSystemMetrics(SM_CYEDGE));

    // Get size of dropdown arrow
    int nArrowWidth = max(::GetSystemMetrics(SM_CXHTHUMB), 5*MarginSize.cx);
    int nArrowHeight = max(::GetSystemMetrics(SM_CYVTHUMB), 5*MarginSize.cy);

    CSize ArrowSize(max(nArrowWidth, nArrowHeight), max(nArrowWidth, nArrowHeight));

    // Get window size
    CRect rect;
    GetWindowRect(rect);

    CWnd* pParent = GetParent();
    if (pParent)
        pParent->ScreenToClient(rect);

	if (bDefault)
	{
		// Set window size at least as wide as 2 arrows, and as high as arrow + margins
		int nWidth = max(rect.Width(), 2*ArrowSize.cx + 2*MarginSize.cx);
		int nHeight = max( rect.Height(), ArrowSize.cy+2*MarginSize.cy);

		MoveWindow(rect.left, rect.top, nWidth, nHeight, TRUE);
	}
	else
	{
		ArrowSize.cy = (rect.Height() - 2*MarginSize.cx);
		ArrowSize.cx = ArrowSize.cy;
	}

    // Get the new coords of this window
    GetWindowRect(rect);
    ScreenToClient(rect);

    // Get the rect where the arrow goes, and convert to client coords.
    m_ArrowRect.SetRect(rect.right - ArrowSize.cx - MarginSize.cx, 
                        rect.top + MarginSize.cy, rect.right - MarginSize.cx,
                        rect.bottom - MarginSize.cy);
}

void CColourPicker::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	SetWindowSize(FALSE);
}

UINT CColourPicker::OnGetDlgCode()
{
	UINT lCode = CButton::OnGetDlgCode();
	lCode &= ~DLGC_BUTTON;

	return lCode;
}

void CColourPicker::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// handle ALT + cursor keys like combo behaviour
	if (nChar == VK_DOWN && !m_bActive)
		DoColourPopup();
	
	else if (nChar == VK_UP && m_bActive)
	{
		ASSERT(m_pPopup);
		
		if (m_pPopup)
		{
			m_pPopup->Cancel();
			m_pPopup = NULL;
			m_bActive = FALSE;
		}
	}
	
	CButton::OnSysKeyDown(nChar, nRepCnt, nFlags);
}
