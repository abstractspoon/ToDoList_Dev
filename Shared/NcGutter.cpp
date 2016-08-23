// NcGutter.cpp: implementation of the CNcGutter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NcGutter.h"
#include "holdredraw.h"
#include "themed.h"
#include "dlgunits.h"
#include "graphicsmisc.h"
#include "misc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

UINT BORDER = 0; // set in WM_NCCALCSIZE so not const

//////////////////////////////////////////////////////////////////////

const UINT WM_NCG_FORCERESIZE = ::RegisterWindowMessage(_T("WM_NCG_FORCERESIZE"));
const UINT SORTWIDTH = 10;
const UINT MINCLIENTWIDTH = (GetSystemMetrics(SM_CXVSCROLL) * 8);
const UINT DEFTEXTFLAGS = (DT_END_ELLIPSIS | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNcGutter::CNcGutter(DWORD dwStyles) : 
	m_bSetRedraw(TRUE), 
	m_bFirstRecalc(TRUE),
	m_nHeaderColDown(-1),
	m_dwButtonDownItem(0),
	m_dwStyles(dwStyles),
	m_fAveCharWidth(0),
	m_nHeaderHeight(0),
	m_nMinClientWidth(MINCLIENTWIDTH)
{
	// add client header hot rect placeholder
	if (HasStyle(NCGS_SHOWHEADER) && CThemed().AreControlsThemed())
		m_hotTrack.AddRect();

	// client column is always last so we add it first
	m_aColumns.Add(new COLUMNDESC(NCG_CLIENTCOLUMNID));
}

CNcGutter::~CNcGutter()
{
	// delete column defs
	int nCol = m_aColumns.GetSize();

	while (nCol--)
		delete m_aColumns[nCol];
}

BOOL CNcGutter::Initialize(HWND hwnd)
{
	if (CSubclassWnd::HookWindow(hwnd))
	{
		// reset calculated widths
		int nCol = m_aColumns.GetSize() - 1; // omit last column == client

		while (nCol--)
		{
			if (m_aColumns[nCol]->bCalcWidth)
				m_aColumns[nCol]->nWidth = 0;
		}
		
		RecalcGutter();

		// hot tracking
		if (HasStyle(NCGS_SHOWHEADER) && CThemed().AreControlsThemed())
			m_hotTrack.Initialize(GetCWnd());

		return TRUE;
	}

	return FALSE;
}

void CNcGutter::EnableStyle(DWORD dwStyle, BOOL bEnable)
{
	if (bEnable)
		m_dwStyles |= dwStyle;
	else
		m_dwStyles &= ~dwStyle;

	if (!IsHooked() || m_bFirstRecalc)
		return;

	// special handling
	if (dwStyle & NCGS_SHOWHEADER)
	{
		if (CThemed().AreControlsThemed())
		{
			if (!bEnable)
				m_hotTrack.Reset();
			else
			{
				m_hotTrack.Initialize(GetCWnd());
				
				for (int nCol = 0; nCol < m_aColumns.GetSize(); nCol++)
					m_hotTrack.AddRect();
				
				UpdateHeaderHotRects();
			}
		}

		PostMessage(WM_NCG_FORCERESIZE);
	}
	else if (dwStyle & NCGS_RIGHTCOLUMNS)
		PostMessage(WM_NCG_FORCERESIZE);
}

void CNcGutter::RemoveColumn(UINT nColID)
{
	int nColIndex = GetColumnIndex(nColID);

	if (nColIndex != -1)
	{
		delete m_aColumns[nColIndex];
		m_aColumns.RemoveAt(nColIndex);

		// reset the first recalc flag to force a recalc
		m_bFirstRecalc = TRUE;
	}
}

BOOL CNcGutter::AddColumn(UINT nColID, LPCTSTR szTitle, UINT nWidth, UINT nTextAlign)
{
	return InsertColumn(-1, nColID, szTitle, nWidth, nTextAlign);
}

BOOL CNcGutter::InsertColumn(int nPos, UINT nColID, LPCTSTR szTitle, UINT nWidth, UINT nTextAlign)
{
	ASSERT (GetColumnIndex(nColID) == -1);

	if (GetColumnIndex(nColID) >= 0)
		return FALSE;

	COLUMNDESC* pCD = new COLUMNDESC(nColID);

	pCD->sTitle = szTitle;
	pCD->nWidth = nWidth;
	pCD->nTextAlign = nTextAlign;
	pCD->bCalcWidth = !nWidth;

	if (szTitle)
	{
		CWindowDC dc(GetCWnd());
		CFont* pOldFont = PrepareFont(&dc, TRUE);
		
		pCD->nTextWidth = dc.GetTextExtent(szTitle).cx;
		
		dc.SelectObject(pOldFont);
	}
	else
		pCD->nTextWidth = 0;
	
	if (nWidth)
		pCD->nWidth = max(nWidth, pCD->nTextWidth + 2 * NCG_COLPADDING);

	// client column is always last
	if (nPos < 0 || nPos > m_aColumns.GetSize() - 1)
		nPos = m_aColumns.GetSize() - 1;

	m_aColumns.InsertAt(nPos, pCD);

	// add a placeholder to the hot tracker
	if (CThemed().AreControlsThemed())
		m_hotTrack.AddRect();

	if (IsHooked())
		RecalcColumn(nColID);

	return TRUE;
}

void CNcGutter::PressHeader(UINT nColID, BOOL bPress)
{
	int nColumn = GetColumnIndex(nColID);

	UnpressAllColumnHeaders(nColumn);
	
	if (nColumn >= 0 && nColumn < m_aColumns.GetSize())
	{
		COLUMNDESC* pCD = m_aColumns[nColumn];

		if (pCD->bPressed != bPress)
		{
			pCD->bPressed = bPress;
			Redraw();
		}
	}
}

void CNcGutter::SetHeaderTitle(UINT nColID, LPCTSTR szTitle, LPCTSTR szFont, BOOL bSymbolFont)
{
	int nColumn = GetColumnIndex(nColID);

	if (nColumn >= 0 && nColumn < m_aColumns.GetSize())
	{
		BOOL bRedraw = FALSE;
		COLUMNDESC* pCD = m_aColumns[nColumn];

		if (!pCD->hFont && szFont)
		{
			LOGFONT lf;
			HFONT hDef = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

			if (GetObject(hDef, sizeof(lf), &lf))
			{
				lstrcpy(lf.lfFaceName, szFont);

				if (bSymbolFont)
				{
					lf.lfCharSet = SYMBOL_CHARSET;
					lf.lfHeight = MulDiv(lf.lfHeight, 12, 10);
				}
				
				pCD->hFont = CreateFontIndirect(&lf);

				if (pCD->hFont)
					pCD->bSymbolFont = bSymbolFont;

				bRedraw = (NULL != pCD->hFont);
			}
		}
		
		if (pCD->sTitle != szTitle)
		{
			pCD->sTitle = szTitle;

			if (szTitle)
			{
				CWindowDC dc(GetCWnd());
				CFont* pOldFont = PrepareFont(&dc, TRUE, pCD->hFont);
				
				pCD->nTextWidth = dc.GetTextExtent(szTitle).cx;
				dc.SelectObject(pOldFont);
			}
			else
				pCD->nTextWidth = 0;
			
			if (!pCD->bCalcWidth)
				pCD->nWidth = max(pCD->nWidth, pCD->nTextWidth + 2 * NCG_COLPADDING);

			bRedraw = TRUE;
		}

		if (bRedraw && IsHooked())
		{
			// do the redraw
			CWindowDC dc(GetCWnd());
			CRect rHeader;

			if (IsClientColumn(nColumn))
			{
				GetHeaderRect(rHeader, GHR_CLIENT, FALSE);
				NcDrawHeader(&dc, rHeader, CLIENT, NULL);
			}
			else
			{
				GetHeaderRect(rHeader, GHR_NONCLIENT, FALSE);
				NcDrawHeader(&dc, rHeader, NONCLIENT, NULL);
			}
		}
	}
}

BOOL CNcGutter::IsClientColumn(int nCol) const 
{ 
	if (Misc::HasFlag(m_dwStyles, NCGS_RIGHTCOLUMNS))
	{
		return (nCol == 0);
	}
	else
	{
		return (nCol == m_aColumns.GetSize() - 1); 
	}
}

void CNcGutter::SetColumnTextAlignment(UINT nColID, UINT nTextAlign, BOOL bRedraw)
{
	int nCol = GetColumnIndex(nColID);

	if (nCol >= 0)
	{
		COLUMNDESC* pCD = m_aColumns[nCol];
		pCD->nTextAlign = nTextAlign;

		if (bRedraw)
			Redraw();
	}
}

UINT CNcGutter::GetColumnTextAlignment(UINT nColID) const
{
	int nCol = GetColumnIndex(nColID);

	if (nCol >= 0)
		return m_aColumns[nCol]->nTextAlign;

	// else
	return 0;
}

void CNcGutter::SetColumnSort(UINT nColID, NCGSORT nSortDir, BOOL bExclusive)
{
	BOOL bNeedsRecalc = FALSE;

	int nCol = m_aColumns.GetSize();

	while (nCol--)
	{
		COLUMNDESC* pCD = m_aColumns[nCol];

		if (pCD->nColID != nColID) // other column
		{
			if (bExclusive)
			{
				BOOL bChange = (pCD->nSortDir != NCGSORT_NONE);

				// adjust column width if fixed size and was sorted
				if (bChange)
				{
					if (!pCD->bCalcWidth)
						pCD->nWidth -= SORTWIDTH;
					else
						bNeedsRecalc = TRUE;
				}

				pCD->nSortDir = NCGSORT_NONE;
				bNeedsRecalc |= bChange;
			}
		}
		else
		{
			BOOL bChange = (pCD->nSortDir != NCGSORT_NONE && nSortDir == NCGSORT_NONE) ||
							(pCD->nSortDir == NCGSORT_NONE && nSortDir != NCGSORT_NONE);

			// adjust column width if fixed size and was sorted
			if (bChange)
			{
				if (!pCD->bCalcWidth)
					pCD->nWidth += SORTWIDTH;
				else
					bNeedsRecalc = TRUE;
			}

			pCD->nSortDir = nSortDir;
		}
	}
	
	if (bNeedsRecalc)
		RecalcGutter();
	else
		Redraw();
}

NCGSORT CNcGutter::GetColumnSort(UINT nColID) const
{
	int nColumn = GetColumnIndex(nColID);

	if (nColumn >= 0 && nColumn < m_aColumns.GetSize())
	{
		return m_aColumns[nColumn]->nSortDir;
	}

	// else
	return NCGSORT_NONE;
}

void CNcGutter::EnableHeaderClicking(UINT nColID, BOOL bEnable)
{
	int nColumn = GetColumnIndex(nColID);

	if (nColumn >= 0 && nColumn < m_aColumns.GetSize())
		m_aColumns[nColumn]->bClickable = bEnable;
}

int CNcGutter::GetColumnIndex(UINT nColID) const
{
	int nCol = m_aColumns.GetSize();

	while (nCol--)
	{
		if (nColID == m_aColumns[nCol]->nColID)
			return nCol;
	}

	// else
	return -1;
}

void CNcGutter::UnpressAllColumnHeaders(int nExcludeCol)
{
	int nCol = m_aColumns.GetSize();

	while (nCol--)
	{
		if (nCol != nExcludeCol)
			m_aColumns[nCol]->bPressed = FALSE;
	}
}

void CNcGutter::Redraw()
{
	CNcRedraw hr(GetHwnd());
}

void CNcGutter::RecalcGutter(BOOL bForceRedraw)
{
	int nCurWidth = GetGutterWidth();
	int nNewWidth = RecalcGutterWidth();

	if (IsHooked())
	{
		if ((nNewWidth != nCurWidth) || m_bFirstRecalc)
		{
			m_bFirstRecalc = FALSE;
			PostMessage(WM_NCG_FORCERESIZE);

			// update header tracking rects
			UpdateHeaderHotRects();

			// notify parent then hook window
			UINT nID = GetDlgCtrlID();

			::SendMessage(GetParent(), WM_NCG_WIDTHCHANGE, nID, MAKELPARAM(nCurWidth, nNewWidth));
			SendMessage(WM_NCG_WIDTHCHANGE, nID, MAKELPARAM(nCurWidth, nNewWidth));
		}
		else if (bForceRedraw)
			Redraw();
	}
}

BOOL CNcGutter::RecalcColumn(UINT nColID)
{
	if (!IsHooked() || m_bFirstRecalc || !m_bSetRedraw)
		return FALSE;

	if (nColID == NCG_CLIENTCOLUMNID)
		return FALSE;

	int nCol = GetColumnIndex(nColID);

	if (nCol == -1)
		return FALSE;

	COLUMNDESC* pCD = m_aColumns[nCol];

	if (!pCD->bCalcWidth)
		return FALSE;

	// do the recalc
	int nCurWidth = GetGutterWidth(); // cache this

	CWindowDC dc(GetCWnd());
	CFont* pOldFont = PrepareFont(&dc, FALSE);

	NCGRECALCCOLUMN ncrc = { nColID, &dc, 0 };

	// try parent then hook window
	UINT nCtrlID = GetDlgCtrlID();

	if (!::SendMessage(GetParent(), WM_NCG_RECALCCOLWIDTH, nCtrlID, (LPARAM)&ncrc))
		SendMessage(WM_NCG_RECALCCOLWIDTH, nCtrlID, (LPARAM)&ncrc);

	if (ncrc.nWidth > 0)
	{
		pCD->nWidth = max(ncrc.nWidth, pCD->nTextWidth) + 2 * NCG_COLPADDING;
	 
		if (pCD->nSortDir != NCGSORT_NONE)
			pCD->nWidth += SORTWIDTH;
	}
	else
		pCD->nWidth = 0;

	if (pOldFont)
		dc.SelectObject(pOldFont);

	// check for a change
	int nNewWidth = GetGutterWidth();

	if (nNewWidth != nCurWidth) // there's been a change
	{
		PostMessage(WM_NCG_FORCERESIZE);
		
		// notify hooked wnd and parent
		SendMessage(WM_NCG_WIDTHCHANGE, nCtrlID, MAKELPARAM(nCurWidth, nNewWidth));
		::SendMessage(GetParent(), WM_NCG_WIDTHCHANGE, nCtrlID, MAKELPARAM(nCurWidth, nNewWidth));
		
		// update header tracking rects
		UpdateHeaderHotRects();

		return TRUE;
	}

	return FALSE;
}

BOOL CNcGutter::SetMinClientWidth(UINT nMinWidth)
{
	if (nMinWidth < MINCLIENTWIDTH)
		return FALSE;

	m_nMinClientWidth = nMinWidth;
	return TRUE;
}

int CNcGutter::GetGutterWidth() const
{
	int nWidth = 0;

	for (int nCol = 0; nCol < m_aColumns.GetSize() - 1; nCol++)
		nWidth += m_aColumns[nCol]->nWidth;

	return nWidth;
}

void CNcGutter::AddRecalcMessage(UINT nMessage, UINT nNotification)
{
	m_mapRecalcMessages.SetAt(MAKELONG((WORD)nMessage, (WORD)nNotification), 0);
}

void CNcGutter::AddRedrawMessage(UINT nMessage, UINT nNotification)
{
	m_mapRedrawMessages.SetAt(MAKELONG((WORD)nMessage, (WORD)nNotification), 0);
}

LRESULT CNcGutter::WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CAPTURECHANGED:
		if (!lp || (HWND)lp == GetHwnd())
			break;

		// else
		{
			m_nHeaderColDown = -1;
			CNcRedraw hr(hRealWnd);
			return Default();
		}
		break;

	case WM_SETFOCUS:
	case WM_KILLFOCUS:
		if (m_bSetRedraw)
		{
			CNcRedraw hr(hRealWnd);
			return Default();
		}
		break;

	case WM_VSCROLL:
	case WM_MOUSEWHEEL:
		if (m_bSetRedraw)
		{
			// send scroll notification
			::SendMessage(GetParent(), WM_NCG_NOTIFYSCROLL, GetDlgCtrlID(), msg);


			CNcRedraw hr(hRealWnd);
			return Default();
		}
		break;

	case WM_HSCROLL:
		// send scroll notification
		::SendMessage(GetParent(), WM_NCG_NOTIFYSCROLL, GetDlgCtrlID(), msg);
		break;

	case WM_KEYDOWN:
		switch (wp) // virtual key
		{
		// the standard scroll keypresses unless the parent is handling these
		case VK_DOWN:
		case VK_UP:
		case VK_LEFT:
		case VK_RIGHT:
		case VK_PRIOR: 
		case VK_NEXT:  
		case VK_END:   
		case VK_HOME:  
			if (!ParentWantRedraw())
				return Default();
			// else fall thru

		case VK_ESCAPE:  
			if (m_bSetRedraw)
			{
				CNcRedraw hr(hRealWnd);
				return Default();
			}
			break;

		// keys that might modify the cursor
		case VK_CONTROL:
		case VK_SHIFT:
		case VK_MENU:
			// but only on the first event (ie not repeats)
			if (!(lp & 0x40000000))
				OnSetCursor();
		}
		break;

	case WM_KEYUP:
		switch (wp) // virtual key
		{
		// keys that might modify the cursor
		case VK_CONTROL:
		case VK_SHIFT:
		case VK_MENU:
			if (!OnSetCursor())
			{
				// draw the default cursor
				SendMessage(WM_SETCURSOR, (WPARAM)GetHwnd(), MAKELPARAM(HTCLIENT, WM_MOUSEMOVE));
			}
		}
		break;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		OnButtonDown(msg, lp);
		break;

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		OnButtonUp(msg, lp);
		break;

	case WM_NCRBUTTONDOWN:
	case WM_NCLBUTTONDOWN:
	case WM_NCLBUTTONDBLCLK:
		if (wp == HTBORDER) // means gutter
			OnNcButtonDown(msg, lp);
		break;

	case WM_NCRBUTTONUP:
		if (wp == HTBORDER) // means gutter
		{
			OnNcButtonUp(msg, lp);
			return 0;
		}
		break;

	case WM_NCLBUTTONUP:
		if (wp == HTBORDER) // means gutter
			OnNcButtonUp(msg, lp);
		break;

	case WM_ERASEBKGND:
		// its not a neat solution but it does ensure that when expanding and collapsing
		// tree controls that any animations look correct
		{
			HDC hdc = (HDC)wp;
			UINT nFlags = PRF_CLIENT;
			OnPrint(hdc, nFlags);
		}
		return TRUE; // always

	case WM_NCHITTEST:
		return OnNcHitTest(lp);

 	case WM_PAINT:
 		OnPaint();
 		return 0;

	case WM_NCCALCSIZE:
		if (wp)
			return OnNcCalcSize((LPNCCALCSIZE_PARAMS)lp);
		break;

	case WM_NCPAINT:
		Default();

		if (m_bSetRedraw)
			OnNcPaint();
		return 0;

	case WM_PRINT:
		if (m_bSetRedraw)
			OnPrint((HDC)wp, (UINT&)lp);
		break;

	case WM_SETREDRAW:
		m_bSetRedraw = wp;
		
		if (m_bSetRedraw)
		{
			RecalcGutter();
			Redraw();

			LRESULT lr = Default();
			return lr;
		}
		break;

	case WM_SETFONT:
		Default();
		RecalcGutter();
		return 0;

	case WM_SETCURSOR:
		if (LOWORD(lp) == HTBORDER && OnSetCursor())
			return TRUE;
		break;

	case WM_SIZE:
		if (m_hotTrack.IsValid())
		{
			Default();
			UpdateHeaderHotRects();
			return 0;
		}
		break;
	}

	// registered messages must be handled explicitly
	if (msg == WM_NCG_FORCERESIZE)
	{
		LRESULT lr = SetWindowPos(hRealWnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER); 
		UpdateHeaderHotRects();

		return lr;
	}

	else if (msg == WM_HTHOTCHANGE)
		OnHotChange(wp, lp);

	// test for specific messages wanting recalcs/redraws
	LRESULT lr = 0;

	if (ProcessRecalcMsg(msg, wp, lp, lr) || ProcessRedrawMsg(msg, wp, lp, lr))
		return lr;

	return CSubclassWnd::WindowProc(hRealWnd, msg, wp, lp);
}

void CNcGutter::SetHeaderHeight(int nHeight)
{
	m_nHeaderHeight = nHeight;
	PostMessage(WM_NCG_FORCERESIZE);
}

LRESULT CNcGutter::OnNcCalcSize(LPNCCALCSIZE_PARAMS lpncsp)
{
	LRESULT lr = Default();
	
	if (BORDER == 0)
		BORDER = lpncsp->rgrc[2].left - lpncsp->rgrc[1].left; // diff between window and client

	// make sure the client width is _never_ reduced to zero
	CRect rWindow;
	GetWindowRect(rWindow);

	UINT nGutterWidth = GetGutterWidth();
	nGutterWidth = min(rWindow.Width() - m_nMinClientWidth - BORDER * 2, nGutterWidth);
	
	if (HasStyle(NCGS_RIGHTCOLUMNS))
		lpncsp->rgrc[0].right -= nGutterWidth;
	else
		lpncsp->rgrc[0].left += nGutterWidth;

	if (HasStyle(NCGS_SHOWHEADER))
    {
        if (m_nHeaderHeight == 0)
		{
			BOOL bThemed = CThemed().AreControlsThemed();
            m_nHeaderHeight = CDlgUnits(GetParent()).ToPixelsY(bThemed ? 12 : 10); // handles font sizes
		}

		lpncsp->rgrc[0].top += m_nHeaderHeight;
    }
	
	return lr;
}

BOOL CNcGutter::ParentWantRecalc()
{
	// ask parent/derived if they still want to recalc
	UINT nID = GetDlgCtrlID();
	const MSG* pMsg = GetCurrentMessage();
	
	BOOL bCancel = ::SendMessage(GetParent(), WM_NCG_WANTRECALC, nID, (LPARAM)pMsg);
	
	if (!bCancel)
		bCancel = SendMessage(WM_NCG_WANTRECALC, nID, (LPARAM)pMsg);
	
	return !bCancel;
}

BOOL CNcGutter::ParentWantRedraw()
{
	// ask parent/derived if they still want to redraw
	UINT nID = GetDlgCtrlID();
	const MSG* pMsg = GetCurrentMessage();
	
	BOOL bCancel = ::SendMessage(GetParent(), WM_NCG_WANTREDRAW, nID, (LPARAM)pMsg);
	
	if (!bCancel)
		bCancel = SendMessage(WM_NCG_WANTREDRAW, nID, (LPARAM)pMsg);
	
	return !bCancel;
}

BOOL CNcGutter::ProcessRecalcMsg(UINT nMsg, WPARAM wp, LPARAM lp, LRESULT& lr)
{
	if (!m_bSetRedraw)
		return FALSE;

	// convert message to lookup key
	WORD nNotification = 0;

	if (nMsg == WM_COMMAND)
		nNotification = HIWORD(wp);

	else if (nMsg == WM_NOTIFY)
	{
		NMHDR* pNMHDR = (NMHDR*)lp;
		nNotification = (WORD)pNMHDR->code;
	}

	DWORD dwKey = MAKELONG((WORD)nMsg, nNotification);
	char dummy;

	if (m_mapRecalcMessages.Lookup(dwKey, dummy))
	{
		lr = Default(); // always

		if (!ParentWantRecalc())
			return TRUE;

		RecalcGutter();

		return TRUE;
	}

	return FALSE;
}

BOOL CNcGutter::ProcessRedrawMsg(UINT nMsg, WPARAM wp, LPARAM lp, LRESULT& lr)
{
	if (!m_bSetRedraw)
		return FALSE;

	// convert message to lookup key
	WORD nNotification = 0;

	if (nMsg == WM_COMMAND)
		nNotification = HIWORD(wp);
	else if (nMsg == WM_NOTIFY)
	{
		NMHDR* pNMHDR = (NMHDR*)lp;
		nNotification = (WORD)pNMHDR->code;
	}

	DWORD dwKey = MAKELONG((WORD)nMsg, nNotification);
	char dummy;

	if (m_mapRedrawMessages.Lookup(dwKey, dummy))
	{
		lr = Default(); // always

		// ask parent/derived if they still want to redraw
		if (!ParentWantRedraw())
			return TRUE;

		CNcRedraw hr(GetHwnd());

		return TRUE;
	}

	return FALSE;
}

void CNcGutter::OnPaint()
{
	PAINTSTRUCT ps;
			
	HDC hdc = ::BeginPaint(GetHwnd(), &ps); // device context for painting

	if (m_bSetRedraw)
	{
		UINT nFlags = PRF_CLIENT;
		OnPrint(hdc, nFlags);
	}

	::EndPaint(GetHwnd(), &ps);
}

LRESULT CNcGutter::OnNcHitTest(CPoint point)
{
	LRESULT lHitTest = Default();
	
	switch (lHitTest)
	{
	case HTVSCROLL:
		// there seems to be a bug in windows that decides that if a window has a 
		// vertical scrollbar then everything to the right of the client area must
		// be the scrollbar so we need to double check if we have right side columns
		if (HasStyle(NCGS_RIGHTCOLUMNS) && (GetStyle() & WS_VSCROLL))
		{
			CRect rClient;
			GetWindowClientRect(rClient, TRUE);

			CRect rVScroll(rClient);
			rVScroll.left = rVScroll.right;
			rVScroll.right += ::GetSystemMetrics(SM_CXVSCROLL);

			if (!rVScroll.PtInRect(point))
				return HTBORDER;
		}
		break;

	case HTNOWHERE:
		return HTBORDER;
		
	case HTCAPTION:
		if (HitTest(point) == NCGHT_HEADER)
			return HTBORDER;
		break;
	}
		
	// everything else
	return lHitTest;
}

void CNcGutter::OnButtonDown(UINT nMsg, CPoint point)
{
	m_dwButtonDownItem = ItemHitTest(point);
	
	SetFocus(GetHwnd());

	// notify parent then hook window
	NCGITEMCLICK ngic = { m_dwButtonDownItem, 
						  0, 
						  NCG_CLIENTCOLUMNID, 
						  nMsg, { point.x, point.y } };
	
	UINT nID = GetDlgCtrlID();
		
	if (!::SendMessage(GetParent(), WM_NCG_NOTIFYITEMCLICK, nID, (LPARAM)&ngic))
		SendMessage(WM_NCG_NOTIFYITEMCLICK, nID, (LPARAM)&ngic);
	
	CNcRedraw hr(GetHwnd());
}

void CNcGutter::OnButtonUp(UINT nMsg, CPoint point)
{
	CPoint ptScreen(point);
	ClientToScreen(&ptScreen);
	
	// before we proceed, check whether we should process the message
	// as a non-client message. we check for any header button that is 
	// down as well as out of client area clicks  
	CRect rClient;
	GetClientRect(rClient);
	
	if (m_nHeaderColDown >= 0 || !rClient.PtInRect(point))
	{
		if (nMsg == WM_LBUTTONUP)
			OnNcButtonUp(WM_NCLBUTTONUP, ptScreen);
		else
			OnNcButtonUp(WM_NCRBUTTONUP, ptScreen);
		
		return;
	}

	DWORD dwItem;
	int nCol;
	NCG_HITTEST nHitTest = HitTest(ptScreen, dwItem, nCol);
	
	if (nMsg == WM_LBUTTONUP && nHitTest == NCGHT_ITEM)
	{
		// check its the same item
		if (dwItem == m_dwButtonDownItem)
		{
			// notify parent then hook window
			NCGITEMCLICK ngic = { dwItem, 
									m_dwButtonDownItem, 
									NCG_CLIENTCOLUMNID, 
									nMsg, { point.x, point.y } };
			
			UINT nID = GetDlgCtrlID();
			
			if (!::SendMessage(GetParent(), WM_NCG_NOTIFYITEMCLICK, nID, (LPARAM)&ngic))
				SendMessage(WM_NCG_NOTIFYITEMCLICK, nID, (LPARAM)&ngic);
		}
	}
}

void CNcGutter::OnNcButtonDown(UINT nMsg, CPoint point)
{
	SetFocus(GetHwnd());
	
	DWORD dwItem;
	int nCol;
	NCG_HITTEST nHitTest = HitTest(point, dwItem, nCol);

	if (nHitTest == NCGHT_NOWHERE)
		return;

	COLUMNDESC* pCD = m_aColumns[nCol];

	// reset
	m_dwButtonDownItem = 0;
	
	switch (nHitTest)
	{
	case NCGHT_HEADER:
		// check its a clickable column
		if (nMsg == WM_NCLBUTTONDOWN && nCol >= 0 && pCD->bClickable)
		{
			SetCapture(GetHwnd());
				
			pCD->bPressed = TRUE;
			m_nHeaderColDown = nCol;
		}
		break;

	case NCGHT_ITEM:
		{
			m_dwButtonDownItem = dwItem;
				
			NCGITEMCLICK ngic = { m_dwButtonDownItem, 0, pCD->nColID, nMsg, { point.x, point.y } };
				
			// try parent then hook window
			UINT nID = GetDlgCtrlID();
				
			if (!::SendMessage(GetParent(), WM_NCG_NOTIFYITEMCLICK, nID, (LPARAM)&ngic))
				SendMessage(WM_NCG_NOTIFYITEMCLICK, nID, (LPARAM)&ngic);
		}
		break;
	}
	
	// redraw for good measure
	Redraw();
}

void CNcGutter::OnNcButtonUp(UINT nMsg, CPoint point)
{
	if (nMsg == WM_NCRBUTTONUP)
	{
		PostMessage(WM_CONTEXTMENU, (WPARAM)GetHwnd(), MAKELPARAM(point.x, point.y));
	}
	else if (nMsg == WM_NCLBUTTONUP)
	{
		DWORD dwItem = 0;
		int nCol;
		
		NCG_HITTEST nHitTest = HitTest(point, dwItem, nCol);
		
		// if the header column is set it means that we're in the middle
		// of a column click. note: we don't check hittest here as the cursor
		// may not still be on the header
		if (m_nHeaderColDown >= 0)
		{
			UINT nColID = m_aColumns[m_nHeaderColDown]->nColID;
			
			// send notification if same column header currently pressed
			if (nHitTest == NCGHT_HEADER && nCol == m_nHeaderColDown)
			{
				NCGHDRCLICK nghc = { nColID, nMsg, FALSE };
				
				// try parent then hook window
				UINT nID = GetDlgCtrlID();
				
				if (!::SendMessage(GetParent(), WM_NCG_NOTIFYHEADERCLICK, nID, (LPARAM)&nghc))
					SendMessage(WM_NCG_NOTIFYHEADERCLICK, nID, (LPARAM)&nghc);
				
				m_nHeaderColDown = -1;
				PressHeader(nColID, nghc.bPressed);
			}
			else
			{
				m_nHeaderColDown = -1;
				PressHeader(nColID, FALSE);
			}
			
			ReleaseCapture();
		}
		// else item click, so check its the same item
		else if (nHitTest == NCGHT_ITEM && dwItem && dwItem == m_dwButtonDownItem)
		{
			// notify parent then hook window
			NCGITEMCLICK ngic = { dwItem, 
				m_dwButtonDownItem, 
				m_aColumns[nCol]->nColID, 
				nMsg, { point.x, point.y } };
			
			UINT nID = GetDlgCtrlID();
			
			if (!::SendMessage(GetParent(), WM_NCG_NOTIFYITEMCLICK, nID, (LPARAM)&ngic))
				SendMessage(WM_NCG_NOTIFYITEMCLICK, nID, (LPARAM)&ngic);
		}
	}
}

void CNcGutter::OnHotChange(int nPrevHot, int nHot)
{
	ASSERT (HasStyle(NCGS_SHOWHEADER));

	if (HasStyle(NCGS_SHOWHEADER))
	{
		CRect rWindow, rClient, rHeader;

		// get rects in window coords
		GetWindowRectEx(rWindow, FALSE);
		GetWindowClientRect(rClient, FALSE);

		CWindowDC dc(GetCWnd());
		CPoint ptCursor;
		GetCursorPos(ptCursor, FALSE);

		// nc portion
		int nClientCol = m_aColumns.GetSize() - 1;
		BOOL bDrawNonClient = FALSE;

		if (nPrevHot >= 0 && nPrevHot < nClientCol)
		{
			const COLUMNDESC* pCD = m_aColumns[nPrevHot];
			bDrawNonClient = (pCD->bClickable);
		}

		if (!bDrawNonClient && nHot >= 0 && nHot < nClientCol)
		{
			const COLUMNDESC* pCD = m_aColumns[nHot];
			bDrawNonClient = (pCD->bClickable);
		}

		CFont* pOldFont = PrepareFont(&dc, TRUE);

		if (bDrawNonClient)
		{
			GetHeaderRect(rHeader, GHR_NONCLIENT, FALSE);
			NcDrawHeader(&dc, rHeader, NONCLIENT, &ptCursor);
		}

		// client bit
		if (nPrevHot == nClientCol || nHot == nClientCol)
		{
			const COLUMNDESC* pCD = m_aColumns[nClientCol];

			if (pCD->bClickable)
			{
				GetHeaderRect(rHeader, GHR_CLIENT, FALSE);
				NcDrawHeader(&dc, rHeader, CLIENT, &ptCursor);
			}
		}

		dc.SelectObject(pOldFont);
	}
}

CFont* CNcGutter::PrepareFont(CDC* pDC, BOOL bHeader, HFONT hFont)
{
	CFont* pFont = CFont::FromHandle(hFont);
	HWND hWnd = NULL;
	
	if (IsHooked())
		hWnd = (bHeader ? ::GetParent(GetHwnd()) : GetHwnd());

	return GraphicsMisc::PrepareDCFont(pDC, hWnd, pFont);
}

DWORD CNcGutter::ItemHitTest(const CPoint& ptClient) const
{
	UINT nID = GetDlgCtrlID();
	LONG lPoint = MAKELONG(ptClient.x, ptClient.y);

	// try parent then hook window
	DWORD dwItem = (DWORD)::SendMessage(GetParent(), WM_NCG_HITTEST, nID, lPoint);

	if (!dwItem)
		dwItem = (DWORD)SendMessage(WM_NCG_HITTEST, nID, lPoint);

	// else
	return dwItem;
}

int CNcGutter::ColumnHitTest(const CPoint& ptScreen) const
{
	CRect rHeader;
	GetHeaderRect(rHeader, GHR_CLIENT, TRUE);

	if (rHeader.PtInRect(ptScreen))
		return m_aColumns.GetSize() - 1;  // last column == client

	// else must be a gutter column
	GetHeaderRect(rHeader, GHR_NONCLIENT, TRUE);

	// adjust ptScreen to be relative to start of gutter
	CPoint ptGutter(ptScreen);
	ptGutter.Offset(-rHeader.TopLeft());

	if (ptGutter.x <= 0)
		return 0;

	int nCol = m_aColumns.GetSize() - 1;  // omit last column == client
	int nGutter = GetGutterWidth();

	while (nCol--)
	{
		int nColWidth = m_aColumns[nCol]->nWidth;

		if (ptGutter.x >= (LONG)(nGutter - nColWidth) && ptGutter.x <= (LONG)nGutter + 1)
			return nCol;

		nGutter -= nColWidth;
	}

	return nCol; // == -1 ?? 
}

UINT CNcGutter::ColumnIDHitTest(const CPoint& ptScreen) const
{
	int nCol = ColumnHitTest(ptScreen);

	return (nCol != -1) ? GetColumnID(nCol) : NCG_CLIENTCOLUMNID;
}

CNcGutter::NCG_HITTEST CNcGutter::HitTest(const CPoint& ptScreen) const
{
	DWORD dwItem;
	int nColumn;

	return HitTest(ptScreen, dwItem, nColumn);
}

BOOL CNcGutter::PtInHeader(const CPoint& ptScreen) const
{
	return (HitTest(ptScreen) == NCGHT_HEADER);
}

void CNcGutter::GetCursorPos(CPoint& ptCursor, BOOL bScreen) const
{
	ptCursor = CPoint(::GetMessagePos());

	if (!bScreen)
	{
		CRect rWindow;
		::GetWindowRect(GetHwnd(), rWindow);
		
		ptCursor.Offset(-rWindow.TopLeft());
	}
}

void CNcGutter::GetHeaderRect(CRect& rHeader, GHR_WHAT nWhat, BOOL bScreen) const
{
	CRect rClient, rWindow;

	GetWindowRectEx(rWindow, bScreen);
	GetWindowClientRect(rClient, bScreen);

	rWindow.DeflateRect(BORDER, BORDER);

	rHeader.bottom = rClient.top;

	if (HasStyle(NCGS_SHOWHEADER))
		rHeader.top = rClient.top - m_nHeaderHeight; // HEADERHEIGHT will be zero if the header is hidden
	else
		rHeader.top = rClient.top;

	switch (nWhat)
	{
	case GHR_CLIENT:
		rHeader.left = rClient.left;
		rHeader.right = rClient.right;

		// check for vert scroll bar
		if (GetStyle() & WS_VSCROLL)
			rHeader.right += ::GetSystemMetrics(SM_CXVSCROLL);

		break;

	case GHR_NONCLIENT:
		if (HasStyle(NCGS_RIGHTCOLUMNS))
		{
			rHeader.left = rClient.right;
			rHeader.right = rWindow.right;

			// check for vert scroll bar
			if (GetStyle() & WS_VSCROLL)
				rHeader.left += ::GetSystemMetrics(SM_CXVSCROLL);
		}
		else
		{
			rHeader.left = rWindow.left;
			rHeader.right = rClient.left;
		}
		break;

	case GHR_ALL:
	default:
		rHeader.left = rWindow.left;
		rHeader.right = rWindow.right;
		break;
	}
}

void CNcGutter::GetWindowRectEx(CRect& rWindow, BOOL bScreen) const
{
	CWnd* pWnd = GetCWnd();
	ASSERT(pWnd);

	if (pWnd)
	{
		pWnd->GetWindowRect(rWindow);

		if (!bScreen)
			rWindow.OffsetRect(-rWindow.TopLeft());
	}
}

void CNcGutter::GetWindowClientRect(CRect& rClient, BOOL bScreen) const
{
	CWnd* pWnd = GetCWnd();
	ASSERT(pWnd);

	if (pWnd)
	{
		pWnd->GetClientRect(rClient);
		pWnd->ClientToScreen(rClient); // always
	
		if (!bScreen) // convert to window coords
		{
			CRect rWindow;
			pWnd->GetWindowRect(rWindow);

			rClient.OffsetRect(-rWindow.TopLeft());
		}
	}
}

void CNcGutter::GetGutterRect(CRect& rGutter, BOOL bScreen) const
{
	CRect rWindow, rClient;

	GetWindowRectEx(rWindow, bScreen);
	GetWindowClientRect(rClient, bScreen);

	rWindow.DeflateRect(BORDER, BORDER);
	
	rGutter = rWindow;

	if (HasStyle(NCGS_RIGHTCOLUMNS))
	{
		rGutter.left = rClient.right;

		// handle vertical scrollbar
		if (GetStyle() & WS_VSCROLL)
			rGutter.left += ::GetSystemMetrics(SM_CXVSCROLL);
	}
	else
		rGutter.right = rClient.left;

}

CNcGutter::NCG_HITTEST CNcGutter::HitTest(const CPoint& ptScreen, DWORD& dwItem, int& nColumn) const
{
	CRect rClient, rHeader, rWindow, rGutter;

	GetWindowClientRect(rClient, TRUE);
	GetGutterRect(rGutter, TRUE);
	GetHeaderRect(rHeader, GHR_ALL, TRUE);

	if (rHeader.PtInRect(ptScreen))
	{
		nColumn = ColumnHitTest(ptScreen);
		return NCGHT_HEADER;
	}
	else if (rGutter.PtInRect(ptScreen) || rClient.PtInRect(ptScreen))
	{
		nColumn = ColumnHitTest(ptScreen);

		// get item
		CPoint point(ptScreen);
		ScreenToClient(&point);

		// must ensure that point is within client rect
		if (HasStyle(NCGS_RIGHTCOLUMNS))
		{
			if (point.x > rClient.Width()) // in the gutter
				point.x = rClient.Width(); // client coords
		}
		else // columns left
		{
			if (point.x < 0) // in the gutter
				point.x = 0; // client coords
		}

		dwItem = ItemHitTest(point);
		return NCGHT_ITEM;
	}

	// else
	return NCGHT_NOWHERE;
}

BOOL CNcGutter::OnSetCursor()
{
	CPoint ptScreen(::GetMessagePos());
	DWORD dwItem;
	int nCol;

	NCG_HITTEST nHitTest = HitTest(ptScreen, dwItem, nCol);

	if (nHitTest == NCGHT_ITEM && dwItem && nCol >= 0)
	{
		NCGGETCURSOR ncgsc = { m_aColumns[nCol]->nColID, dwItem };
		UINT nID = GetDlgCtrlID();
		
		HCURSOR hCursor = (HCURSOR)::SendMessage(GetParent(), WM_NCG_GETCURSOR, nID, (LPARAM)&ncgsc);
		
		if (!hCursor)
			hCursor = (HCURSOR)SendMessage(WM_NCG_GETCURSOR, nID, (LPARAM)&ncgsc);
		
		if (hCursor)
		{
			::SetCursor(hCursor);
			return TRUE;
		}
	}

	return FALSE;
}

void CNcGutter::OnNcPaint() 
{
	// force client repaints to happen first because
	// of the time it takes to perform the non-client paint
	::UpdateWindow(GetHwnd());

	// see if we can avoid any unnecessary drawing
	int nGutterWidth = GetGutterWidth();

	if (nGutterWidth <= 0 && !HasStyle(NCGS_SHOWHEADER))
		return;

	CRect rWindow, rClient, rGutter, rHeader;

	GetWindowClientRect(rClient, FALSE);
	GetGutterRect(rGutter, FALSE);
	GetHeaderRect(rHeader, GHR_ALL, FALSE);

	// get cursor pos in window coords for header drawing
	CPoint ptCursor;

	if (HasStyle(NCGS_SHOWHEADER))
		GetCursorPos(ptCursor, FALSE);

	CWindowDC dc(GetCWnd());

	if (nGutterWidth > 0)
	{
		// to avoid creating excessively large bitmaps we'll render the client header straight
		// into the window dc ie only the non-client gutter & header is rendered on the bitmap
		//BOOL bNewBitmap = BitmapNeedsRecreate(&dc, &m_bmNonClient, rGutter);

		// if we can't create a memory dc or bitmap then we're stuffed
		// so don't bother proceeding
		if ((!m_dcMem.GetSafeHdc() && !m_dcMem.CreateCompatibleDC(&dc)) || 
			!PrepareBitmap(&dc, &m_bmNonClient, rGutter))
		{
			return;
		}

		// select bitmap into memory dc
		CBitmap* pBMOld = m_dcMem.SelectObject(&m_bmNonClient);

		// fill background
		m_dcMem.FillSolidRect(rGutter, ::GetSysColor(COLOR_WINDOW));
		
		// iterate the top level items
		CFont* pOldFont = PrepareFont(&m_dcMem, FALSE);
		
		int nItem = 1;
		CRect rItem;
		DWORD dwItem = GetFirstVisibleTopLevelItem(nItem);
		
		while (dwItem)
		{
			CString sPrevPos;
			
			NcDrawItem(&m_dcMem, dwItem, 0, 0, nItem, rGutter, rItem, TRUE);
			
			dwItem = GetNextItem(dwItem);
			nItem++;
			
			if (rItem.bottom >= rGutter.bottom)
				break;
		}
		
		// see if anyone wants to do any final drawing
		CRect rItems(rGutter);
		rItems.bottom = min(rGutter.bottom, rItem.bottom);
		
		PostNcDraw(&m_dcMem, rItems);
		
		// cleanup			
		m_dcMem.SelectObject(pOldFont);
		
		// non-client header
		if (HasStyle(NCGS_SHOWHEADER))
		{
			GetHeaderRect(rHeader, GHR_NONCLIENT, FALSE);
			NcDrawHeader(&m_dcMem, rHeader, NONCLIENT, &ptCursor);
		}
		
		// blt to window dc
		dc.BitBlt(rGutter.left, 
				rGutter.top, 
				rGutter.Width(), 
				rGutter.Height(),
				&m_dcMem, 
				rGutter.left, 
				rGutter.top, 
				SRCCOPY);
		
		// deselect bitmap
		m_dcMem.SelectObject(pBMOld); // V.V.IMPORTANT
	}

	// two items we render direct to the window dc
	
	// 1. the client header
	if (HasStyle(NCGS_SHOWHEADER))
	{
		CFont* pOldFont = PrepareFont(&dc, FALSE);

		GetHeaderRect(rHeader, GHR_CLIENT, FALSE);
		NcDrawHeader(&dc, rHeader, CLIENT, &ptCursor);

		// cleanup			
		dc.SelectObject(pOldFont);
	}
				
	// 2. if the window base does not match the client bottom then 
	// paint the extra bit gray (means there is a horz scrollbar)
	if (rGutter.bottom != rClient.bottom)
	{
		rGutter.top = rClient.bottom;

		// allow for vert scrollbar when columns are on right
		if (HasStyle(NCGS_RIGHTCOLUMNS) && (GetStyle() & WS_VSCROLL))
			rGutter.left -= ::GetSystemMetrics(SM_CXVSCROLL);

		if (CThemed().IsNonClientThemed())
		{
			CThemed th;

			if (th.Open(GetCWnd(), _T("SCROLLBAR")))
				th.DrawBackground(&dc, SBP_LOWERTRACKHORZ, SCRBS_NORMAL, rGutter);
			else
				dc.FillSolidRect(rGutter, ::GetSysColor(COLOR_SCROLLBAR));
		}
		else
			dc.FillSolidRect(rGutter, ::GetSysColor(COLOR_SCROLLBAR));
	}
}

void CNcGutter::NcDrawHeaderColumn(CDC* pDC, int nColumn, CRect rColumn, CThemed* pTheme, LPPOINT pCursor)
{
	COLUMNDESC* pCD = m_aColumns[nColumn];
	BOOL bSorted = (pCD->nSortDir != NCGSORT_NONE);
	
	if (!pTheme)
	{
		if (pCD->bPressed)
		{
			pDC->FillSolidRect(rColumn, ::GetSysColor(COLOR_3DHIGHLIGHT));
			pDC->Draw3dRect(rColumn, ::GetSysColor(COLOR_3DFACE), ::GetSysColor(COLOR_3DSHADOW));
		}
		else
			pDC->Draw3dRect(rColumn, ::GetSysColor(COLOR_3DHIGHLIGHT), ::GetSysColor(COLOR_3DSHADOW));
	}
	else
	{
		BOOL bHot = pCursor ? rColumn.PtInRect(*pCursor) : FALSE;
		bHot &= pCD->bClickable;

		pTheme->DrawBackground(pDC, HP_HEADERITEM, pCD->bPressed ? HIS_PRESSED : 
								(bHot ? HIS_HOT : HIS_NORMAL), rColumn);
	}
	
	// text
	if (!pCD->sTitle.IsEmpty())
	{
		CRect rText(rColumn);

		rText.bottom -= (pCD->bSymbolFont ? 1 : 2) + (pTheme ? 1 : 0);

		switch (pCD->nTextAlign)
		{
		case DT_LEFT:
			rText.left += NCG_COLPADDING;
			break;
			
		case DT_RIGHT:
			rText.right -= NCG_COLPADDING;
			break;
		}
		
		if (bSorted)
			rText.right -= (SORTWIDTH + 2);

		UINT nFlags = (DEFTEXTFLAGS | pCD->nTextAlign | GraphicsMisc::GetRTLDrawTextFlags(GetHwnd()));
		
		if (pCD->hFont)
		{
			CFont* pOldFont = PrepareFont(pDC, TRUE, pCD->hFont);

			if (pCD->bSymbolFont)
				GraphicsMisc::DrawAnsiSymbol(pDC, (char)pCD->sTitle[0], rText, nFlags, CFont::FromHandle(pCD->hFont));
			else
				pDC->DrawText(pCD->sTitle, rText, nFlags);

			pDC->SelectObject(pOldFont);
		}
		else
			pDC->DrawText(pCD->sTitle, rText, nFlags);

		// adjust for sort arrow
		if (bSorted)
		{
			if (pCD->nTextAlign & DT_CENTER)
				rColumn.left = ((rText.left + rText.right + pDC->GetTextExtent(pCD->sTitle).cx) / 2) + 2;

			else if (pCD->nTextAlign & DT_RIGHT)
				rColumn.left = rText.right + 2;
			else
				rColumn.left = rText.left + pDC->GetTextExtent(pCD->sTitle).cx + 2;
		}
	}

	// draw sort direction if required
	if (bSorted)
	{
		int nMidY = (rColumn.top + rColumn.bottom) / 2;
		POINT ptArrow[3] = { { 0, 0 }, { 3, (int)pCD->nSortDir * 3 }, { 7, -(int)pCD->nSortDir } };
		
		// translate the arrow to the appropriate location
		int nPoint = 3;
		
		while (nPoint--)
		{
			ptArrow[nPoint].x += rColumn.left + 3;
			ptArrow[nPoint].y += nMidY;
		}
		pDC->Polyline(ptArrow, 3);
	}
	
}

void CNcGutter::NcDrawHeader(CDC* pDC, const CRect& rHeader, HCHDRPART nPart, const LPPOINT pCursor)
{
	if (!HasStyle(NCGS_SHOWHEADER))
		return;

	CThemed th;
	BOOL bThemed = th.AreControlsThemed() && th.Open(GetCWnd(), _T("HEADER"));

	if (!bThemed)
		pDC->FillSolidRect(rHeader, ::GetSysColor(COLOR_3DFACE));

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));

	CFont* pOldFont = PrepareFont(pDC, TRUE); // default header font
	CRect rColumn(rHeader);

	if (nPart == NONCLIENT)
	{
		int nNumCols = m_aColumns.GetSize() - 1; // omit last column == client
		rColumn.right = rColumn.left;

		for (int nCol = 0; nCol < nNumCols; nCol++)
		{
			COLUMNDESC* pCD = m_aColumns[nCol];

			if (!pCD->nWidth)
				continue;

			rColumn.left = rColumn.right;
			rColumn.right = min(rHeader.right, rColumn.left + (int)pCD->nWidth);

			NcDrawHeaderColumn(pDC, nCol, rColumn, bThemed ? &th : NULL, bThemed ? pCursor : NULL);

			// stop if we've gone passed the end of the available space
			if (rColumn.right >= rHeader.right)
				break;
		}
	}
	else // the rest is over the client area
	{
		int nCol = GetColumnIndex(NCG_CLIENTCOLUMNID);
		ASSERT (nCol != -1);

		NcDrawHeaderColumn(pDC, nCol, rColumn, bThemed ? &th : NULL, bThemed ? pCursor : NULL);
	}

	// cleanup
	pDC->SelectObject(pOldFont);
}

void CNcGutter::OnPrint(HDC hdc, UINT& nFlags)
{
	if (nFlags & PRF_CLIENT)
	{
		// render the control to a bitmap for flicker free
		CRect rClient;
		GetClientRect(rClient);
		rClient.InflateRect(1, 1);

		CDC dcMem, dc;

		if (dcMem.CreateCompatibleDC(NULL))
		{
			dc.Attach(hdc);

			if (PrepareBitmap(&dc, &m_bmClient, rClient))
			{
				CBitmap* pBMOld = dcMem.SelectObject(&m_bmClient);
				dcMem.FillSolidRect(rClient, GetSysColor(COLOR_WINDOW));

				CSubclassWnd::WindowProc(GetHwnd(), WM_PRINT, (WPARAM)(HDC)dcMem, PRF_CLIENT);

				dc.BitBlt(0, 0, rClient.right, rClient.bottom, &dcMem, 0, 0, SRCCOPY);

				nFlags &= ~PRF_CLIENT; // we just drawn this bit

				// cleanup
				dcMem.SelectObject(pBMOld); // V.V.IMPORTANT
			}

			dc.Detach();
		}
	}
}

DWORD CNcGutter::GetFirstVisibleTopLevelItem(int& nItem) const
{
	// try hook window then parent
	UINT nID = GetDlgCtrlID();

	DWORD dwItem = (DWORD)::SendMessage(GetParent(), WM_NCG_GETFIRSTVISIBLETOPLEVELITEM, nID, (LPARAM)(LPINT)&nItem);

	if (!dwItem)
		dwItem = (DWORD)SendMessage(WM_NCG_GETFIRSTVISIBLETOPLEVELITEM, nID, (LPARAM)(LPINT)&nItem);

	// else
	return dwItem;
}

void CNcGutter::NcDrawItem(CDC* pDC, DWORD dwItem, DWORD dwParentItem, int nLevel, int nPos, 
						   const CRect& rGutter, CRect& rItem /* out */, BOOL bDrawChildren)
{
	rItem = GetWindowItemRect(dwItem);

	if (!rItem.IsRectEmpty()) // something to draw?
	{
		if (!bDrawChildren)
		{
			if (HasStyle(NCGS_RIGHTCOLUMNS))
				rItem.left = rGutter.left;
			else
				rItem.right = rGutter.right;

			pDC->FillSolidRect(rItem, GetSysColor(COLOR_WINDOW));
		}

		// draw the item
		NCGDRAWITEM ncgDI;
		
		ncgDI.pDC = pDC;
		ncgDI.dwItem = dwItem;
		ncgDI.dwParentItem = dwParentItem;
		ncgDI.nLevel = nLevel;
		ncgDI.nItemPos = nPos;
		ncgDI.rWindow = &rGutter;
		ncgDI.rItem = &rItem;

		// final check that this item wants to be drawn
		UINT nID = GetDlgCtrlID();
		BOOL bDraw = !::SendMessage(GetParent(), WM_NCG_PREDRAWITEM, nID, (LPARAM)&ncgDI);

		if (bDraw)
			bDraw = !SendMessage(WM_NCG_PREDRAWITEM, nID, (LPARAM)&ncgDI);
		
		if (bDraw)
		{
			// fill the item with background colour
			COLORREF crText, crBack;
			GetItemColors(dwItem, crText, crBack);
			
			if (crBack != GetSysColor(COLOR_WINDOW))
					pDC->FillSolidRect(rItem, crBack);
			
			// and set the text color
			pDC->SetTextColor(crText);
			pDC->SetBkMode(TRANSPARENT);
			
			// see if the caller wants to draw the entire row in one hit
			BOOL bDrawEntire = ::SendMessage(GetParent(), WM_NCG_DRAWITEM, nID, (LPARAM)&ncgDI);

			if (!bDrawEntire)
				bDrawEntire = SendMessage(WM_NCG_DRAWITEM, nID, (LPARAM)&ncgDI);

			// if not, draw each column in turn
			if (!bDrawEntire)
			{
				rItem.right = rItem.left;
				int nNumCols = m_aColumns.GetSize() - 1; // omit last column == client
				
				for (int nCol = 0; nCol < nNumCols; nCol++)
				{
					const COLUMNDESC* pCD = m_aColumns[nCol];
					
					ncgDI.nColID = pCD->nColID;
					ncgDI.nTextAlign = pCD->nTextAlign;
					
					rItem.left = rItem.right;
					
					// if we're out of the window rect then stop
					if (rItem.left > rGutter.right)
						break;
					
					// don't bother with zero width columns
					int nColWidth = (int)pCD->nWidth;
					
					if (!nColWidth)
						continue;
					
					rItem.right += nColWidth;
					int nSaveDC = pDC->SaveDC();
					
					// try parent then hook window
					if (!::SendMessage(GetParent(), WM_NCG_DRAWITEMCOLUMN, nID, (LPARAM)&ncgDI))
						SendMessage(WM_NCG_DRAWITEMCOLUMN, nID, (LPARAM)&ncgDI);
					
					pDC->RestoreDC(nSaveDC);

				}
			}
			
			// post draw for this item
			rItem.left = rGutter.left;
			rItem.right = rGutter.right;
			PostNcDrawItem(pDC, dwItem, rItem, nLevel, FALSE);
		}

		// draw children
		if (bDrawChildren)
		{
			int nChild = 1;
			DWORD dwChild = GetFirstChildItem(dwItem);
			CRect rChild;
			
			while (dwChild)
			{
				NcDrawItem(pDC, dwChild, dwItem, nLevel + 1, nChild, rGutter, rChild, TRUE);
				
				dwChild = GetNextItem(dwChild);
				nChild++;
				
				// accumulate child rects into rItem
				rItem |= rChild;
				
				// stop if we go off the bottom of the window
				if (rItem.bottom >= rGutter.bottom)
					break;
			}
			
			// post draw only if we reached the last item
			CRect rItemTotal(rItem);
			rItemTotal.left = rGutter.left;
			rItemTotal.right = rGutter.right;
			
			if (!dwChild)
				PostNcDrawItem(pDC, dwItem, rItemTotal, nLevel, TRUE);
			
			rItem = rItemTotal;
		}
	}
}

void CNcGutter::RedrawItem(DWORD dwItem)
{
	// for speed we draw straight to the window DC so _don't_ use
	// this internally.
	CRect rGutter, rItem;

	GetGutterRect(rGutter, FALSE);

	// prepare the DC
	CWindowDC dc(GetCWnd());
	CFont* pOldFont = PrepareFont(&dc, FALSE);

	NcDrawItem(&dc, dwItem, GetParentItem(dwItem), -1, -1, rGutter, rItem, FALSE);

	// cleanup
	if (pOldFont)
		dc.SelectObject(pOldFont);
}

void CNcGutter::PostNcDrawItem(CDC* pDC, DWORD dwItem, const CRect& rItem, int nLevel, BOOL bParent)
{
	NCGDRAWITEM ncgDI;
		
	ncgDI.pDC = pDC;
	ncgDI.rItem = &rItem;
	ncgDI.nLevel = nLevel;

	if (bParent)
		ncgDI.dwParentItem = dwItem;
	else
		ncgDI.dwItem = dwItem;

	UINT nID = GetDlgCtrlID();

	// try parent then hook window
	if (!::SendMessage(GetParent(), WM_NCG_POSTDRAWITEM, nID, (LPARAM)&ncgDI))
		SendMessage(WM_NCG_POSTDRAWITEM, nID, (LPARAM)&ncgDI);
}

DWORD CNcGutter::GetParentItem(DWORD dwItem) const
{
	UINT nID = GetDlgCtrlID();
		
	// try parent then hook window
	DWORD dwParentID = ::SendMessage(GetParent(), WM_NCG_GETPARENTITEM, nID, dwItem);

	if (!dwParentID)
		dwParentID = SendMessage(WM_NCG_GETPARENTITEM, nID, dwItem);

	return dwParentID;
}

void CNcGutter::PostNcDraw(CDC* pDC, const CRect& rWindow)
{
	NCGDRAWITEM ncgDI;
		
	ncgDI.pDC = pDC;
	ncgDI.rWindow = &rWindow;

	UINT nID = GetDlgCtrlID();
		
	// try parent then hook window
	if (!::SendMessage(GetParent(), WM_NCG_POSTNCDRAW, nID, (LPARAM)&ncgDI))
		SendMessage(WM_NCG_POSTNCDRAW, nID, (LPARAM)&ncgDI);
}

DWORD CNcGutter::GetNextItem(DWORD dwItem) const
{
	UINT nID = GetDlgCtrlID();

	// try parent then hook window
	DWORD dwNextItem = (DWORD)::SendMessage(GetParent(), WM_NCG_GETNEXTITEM, nID, dwItem);

	if (!dwNextItem)
		dwNextItem = (DWORD)SendMessage(WM_NCG_GETNEXTITEM, nID, dwItem);

	return dwNextItem;
}

DWORD CNcGutter::GetItemHeight() const
{
	// try parent then hook window
	DWORD dwHeight = (DWORD)::SendMessage(GetParent(), WM_NCG_GETITEMHEIGHT, 0, 0);

	if (!dwHeight)
		dwHeight = (DWORD)SendMessage(WM_NCG_GETITEMHEIGHT, 0, 0);

	return dwHeight;
}

void CNcGutter::GetItemColors(DWORD dwItem, COLORREF& crText, COLORREF& crBack) const
{
	UINT nID = GetDlgCtrlID();
	NCGITEMCOLORS colors = { dwItem, 0, 0, FALSE, FALSE };

	// try parent then hook window
	::SendMessage(GetParent(), WM_NCG_GETITEMCOLORS, nID, (LPARAM)&colors);

	if (!colors.bBackSet || !colors.bTextSet)
		SendMessage(WM_NCG_GETITEMCOLORS, nID, (LPARAM)&colors);

	if (colors.bBackSet)
		crBack = colors.crBack;
	else
		crBack = GetSysColor(COLOR_WINDOW); // fallback

	if (colors.bTextSet)
		crText = colors.crText;
	else
		crText = GetSysColor(COLOR_WINDOWTEXT); // fallback
}

int CNcGutter::RecalcGutterWidth()
{
	if (!IsHooked())
		return 0;

	// optimization
	if (!m_bSetRedraw)
		return GetGutterWidth();

	CWindowDC dc(GetCWnd());

	NCGRECALCCOLUMN ncrc;
	ncrc.pDC = &dc;

	// try hook window then parent
	UINT nID = GetDlgCtrlID();
	int nGutter = 0;
	int nCol = m_aColumns.GetSize() - 1; // omit last column == client

	while (nCol--)
	{
		// check if its a fixed size column
		COLUMNDESC* pCD = m_aColumns[nCol];

		if (pCD->bCalcWidth)
		{
			CFont* pOldFont = PrepareFont(&dc, FALSE);
			
			ncrc.nColID = pCD->nColID;
			ncrc.nWidth = 0;
			
			// try parent then hook window
			if (!::SendMessage(GetParent(), WM_NCG_RECALCCOLWIDTH, nID, (LPARAM)&ncrc))
				SendMessage(WM_NCG_RECALCCOLWIDTH, nID, (LPARAM)&ncrc);
			
			if (ncrc.nWidth > 0)
			{
				pCD->nWidth = max(ncrc.nWidth, pCD->nTextWidth) + 2 * NCG_COLPADDING;
				
				if (pCD->nSortDir != NCGSORT_NONE)
					pCD->nWidth += SORTWIDTH;
			}
			else
				pCD->nWidth = 0;
			
			if (pOldFont)
				dc.SelectObject(pOldFont);

		}
		nGutter += pCD->nWidth;
	}

	return nGutter;
}

int CNcGutter::GetColumnWidth(UINT nColID) const
{
	int nCol = GetColumnIndex(nColID);

	if (nCol >= 0)
	{
		const COLUMNDESC* pCD = m_aColumns[nCol];
		return pCD->nWidth;
	}

	// else
	return 0;
}

UINT CNcGutter::GetColumnID(int nCol) const
{
	if (IsClientColumn(nCol))
		return NCG_CLIENTCOLUMNID;

	else if (nCol >= 0 && nCol < GetColumnCount())
	{
		const COLUMNDESC* pCD = m_aColumns[nCol];
		return pCD->nColID;
	}

	// else
	return 0;
}

CRect CNcGutter::GetWindowItemRect(DWORD dwItem) const
{
	UINT nID = GetDlgCtrlID();
	NCGITEMRECT ncgGI;
	ncgGI.dwItem = dwItem;

	// try parent then hook window
	if (!::SendMessage(GetParent(), WM_NCG_GETITEMRECT, nID, (LPARAM)&ncgGI))
		SendMessage(WM_NCG_GETITEMRECT, nID, (LPARAM)&ncgGI);

	// convert from client to window coords
	CRect rWindow, rGutter, rItem(ncgGI.rItem);
	GetGutterRect(rGutter, FALSE);
	GetWindowRectEx(rWindow, TRUE);

	CWnd* pWnd = GetCWnd();
	pWnd->ClientToScreen(rItem);

	rItem.OffsetRect(0, -rWindow.top);
	rItem.right = rGutter.right;
	rItem.left = rGutter.left;

	return rItem;
}

DWORD CNcGutter::GetFirstChildItem(DWORD dwItem) const
{
	UINT nID = GetDlgCtrlID();

	// try parent then hook window
	DWORD dwChildItem = (DWORD)::SendMessage(GetParent(), WM_NCG_GETFIRSTCHILDITEM, nID, dwItem);

	if (!dwChildItem)
		dwChildItem = (DWORD)SendMessage(WM_NCG_GETFIRSTCHILDITEM, nID, dwItem);

	return dwChildItem;
}

BOOL CNcGutter::BitmapNeedsRecreate(CDC* pDC, CBitmap* pBitmap, const CRect& rect)
{
	BOOL bRecreate = !pBitmap->GetSafeHandle();

	if (!bRecreate)
	{
		BITMAP BM;

		bRecreate = (!pBitmap->GetBitmap(&BM));

		if (!bRecreate)
		{
			int nBitDepth = pDC->GetDeviceCaps(BITSPIXEL);

			bRecreate = (nBitDepth != BM.bmBitsPixel || 
						rect.right > BM.bmWidth ||
						rect.bottom > BM.bmHeight);
		}
	}

	return bRecreate;
}

BOOL CNcGutter::PrepareBitmap(CDC* pDC, CBitmap* pBitmap, const CRect& rect)
{
	if (BitmapNeedsRecreate(pDC, pBitmap, rect))
	{
		GraphicsMisc::VerifyDeleteObject(*pBitmap);
		pBitmap->CreateCompatibleBitmap(pDC, rect.right, rect.bottom);
	}

	return (NULL != pBitmap->GetSafeHandle());
}

void CNcGutter::UpdateHeaderHotRects()
{
	if (!HasStyle(NCGS_SHOWHEADER) || !CThemed().AreControlsThemed())
		return;

	CRect rWindow, rClient, rHeader;

	// get rects in client coords
	GetWindowRect(rWindow);
	ScreenToClient(rWindow);

	GetClientRect(rClient);

	// adjust top of window rect to be top of header (if showing)
	GetHeaderRect(rHeader, GHR_NONCLIENT, TRUE);
	ScreenToClient(rHeader);

	CRect rItem(rHeader);
	rItem.right = rItem.left;

	int nNumCols = m_aColumns.GetSize() - 1;  // omit last column == client

	for (int nCol = 0; nCol < nNumCols; nCol++)
	{
		rItem.left = rItem.right;
		rItem.right += m_aColumns[nCol]->nWidth;
		rItem.right = min(rItem.right, rWindow.right);
		
		m_hotTrack.UpdateRect(nCol, rItem);
		
		if (rItem.right == rWindow.right)
			break;
	}

	// client bit
	rItem.left = rClient.left;
	rItem.right = rClient.right;

	m_hotTrack.UpdateRect(nNumCols, rItem);
}

void CNcGutter::GetColumnHeaderClientRect(UINT nColID, CRect& rect)
{
	int nCol = GetColumnIndex(nColID);

	if (nCol >= 0)
		m_hotTrack.GetRect(nCol, rect);
}
