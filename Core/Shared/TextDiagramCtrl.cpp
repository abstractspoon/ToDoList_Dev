// TextDiagramCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TextDiagramCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CHARSIZE_X 8
#define CHARSIZE_Y 12
#define ARROWSIZE 6
#define IDC_EDITTEXT 101

enum 
{
	ID_RECT_DELETE = 32771,
	ID_RECT_EDITTEXT,
	ID_RECT_RESIZETOFIT,
	ID_RECT_NEW,
	ID_CONN_NEW,
	ID_CONN_DELETE,
	ID_CONN_MODIFY,
	ID_CONN_FLIP,
	ID_RESET,
};

/////////////////////////////////////////////////////////////////////////////
// CTextDiagramCtrl

CTextDiagramCtrl::CTextDiagramCtrl(BOOL bWarnOnDeletion) 
	: m_nSelRect(-1), 
		m_ptScrollOffset(0, 0), 
		m_bMoving(FALSE), 
		m_nSizing(HTNOWHERE), 
		m_ptDragStart(0, 0), 
		m_ptDragPrev(0, 0), 
		m_editText("-|<>V^+", TRUE), 
		m_bDblClick(FALSE), 
		m_ptContextMenu(0, 0),
		m_bResizeOnEdit(FALSE),
		m_bWarn(bWarnOnDeletion),
		m_nPageWidth(-1)
{
}

CTextDiagramCtrl::~CTextDiagramCtrl()
{
}


BEGIN_MESSAGE_MAP(CTextDiagramCtrl, CWnd)
	//{{AFX_MSG_MAP(CTextDiagramCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CAPTURECHANGED()
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_RECT_DELETE, OnRectDelete)
	ON_COMMAND(ID_RECT_EDITTEXT, OnRectEditText)
	ON_COMMAND(ID_RECT_NEW, OnRectNew)
	ON_COMMAND(ID_CONN_NEW, OnConnNew)
	ON_COMMAND(ID_CONN_DELETE, OnConnDelete)
	ON_COMMAND(ID_CONN_MODIFY, OnConnModify)
	ON_COMMAND(ID_RESET, OnReset)
	ON_EN_KILLFOCUS(IDC_EDITTEXT, OnEndTextEdit)
	ON_EN_CHANGE(IDC_EDITTEXT, OnChangeText)
	ON_COMMAND(ID_CONN_FLIP, OnConnFlip)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTextDiagramCtrl message handlers

BOOL CTextDiagramCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParent, UINT nID)
{
	return CWnd::Create(NULL, NULL, dwStyle | WS_HSCROLL | WS_VSCROLL, rect, pParent, nID);
}

void CTextDiagramCtrl::SetDiagram(const CTextDiagram& diagram)
{
	CTextDiagram::SetDiagram(diagram);

	m_nSelRect = -1;

	if (GetSafeHwnd())
	{
		SendNotification(TDN_SELCHANGE);

		UpdateScrollbars();
		Invalidate();
	}
}

void CTextDiagramCtrl::SetDiagram(LPCTSTR szDiagram)
{
	CTextDiagram::SetDiagram(szDiagram);

	m_nSelRect = -1;

	if (GetSafeHwnd())
	{
		SendNotification(TDN_SELCHANGE);

		UpdateScrollbars();
		Invalidate();
	}
}

void CTextDiagramCtrl::SetDiagram(const CStringArray& diagram)
{
	CTextDiagram::SetDiagram(diagram);

	m_nSelRect = -1;

	if (GetSafeHwnd())
	{
		SendNotification(TDN_SELCHANGE);

		UpdateScrollbars();
		Invalidate();
	}
}

void CTextDiagramCtrl::SetPageWidth(int nPageWidth, BOOL bRedraw)
{
	m_nPageWidth = max(-1, nPageWidth);

	if (bRedraw && GetSafeHwnd())
		Invalidate();
}

BOOL CTextDiagramCtrl::ResetDiagram()
{
	if (m_bWarn && !IsEmpty() && 
		IDNO == MessageBox("Are you sure you want to clear the diagram", "Confirm Reset", MB_YESNO | MB_DEFBUTTON2))
		return FALSE;

	CTextDiagram::ResetDiagram();

	m_nSelRect = -1;
	SendNotification(TDN_SELCHANGE);

	UpdateScrollbars();
	Invalidate();

	return TRUE;
}

int CTextDiagramCtrl::AddRect(LPCRECT pRect, LPCTSTR szText)
{
	int nRect = CTextDiagram::AddRect(pRect, szText);

	if (nRect >= 0)
	{
		UpdateScrollbars();
		Invalidate();
	}

	return nRect;
}

int CTextDiagramCtrl::AddRect(LPCRECT pRect, LPCTSTR szText, BOOL bEditText)
{
	int nRect = AddRect(pRect, szText);

	if (nRect >= 0 && bEditText)
	{
		m_nSelRect = nRect;
		SendNotification(TDN_SELCHANGE);

		EditSelectedText();
	}

	return nRect;
}

int CTextDiagramCtrl::AddConnection(int nRectFrom, int nRectTo, int nSideFrom)
{
	int nConn = CTextDiagram::AddConnection(nRectFrom, nRectTo, nSideFrom);

	if (nConn >= 0)
	{
		UpdateScrollbars();
		Invalidate();
	}

	return nConn;
}

BOOL CTextDiagramCtrl::DeleteRect(int nRect)
{
	if (m_bWarn && IsValidRect(nRect) && 
		IDNO == MessageBox("Are you sure you want the delete the selected box", "Confirm Delete", MB_YESNO | MB_DEFBUTTON2))
		return FALSE;

	if (CTextDiagram::DeleteRect(nRect))
	{
		UpdateScrollbars();
		Invalidate();

		if (m_nSelRect == nRect)
		{
			m_nSelRect = -1;
			SendNotification(TDN_SELCHANGE);
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CTextDiagramCtrl::DeleteConnection(int nConn)
{
	if (m_bWarn && IsValidConnection(nConn) && 
		IDNO == MessageBox("Are you sure you want the delete the selected connection", "Confirm Delete", MB_YESNO | MB_DEFBUTTON2))
		return FALSE;

	if (CTextDiagram::DeleteConnection(nConn))
	{
		UpdateScrollbars();
		Invalidate();

		return TRUE;
	}

	return FALSE;
}

BOOL CTextDiagramCtrl::SetRect(int nRect, const CTDRect& rect)
{
	if (CTextDiagram::SetRect(nRect, rect))
	{
		UpdateScrollbars();

		Invalidate();
		return TRUE;
	}

	return FALSE;
}

BOOL CTextDiagramCtrl::SetText(int nRect, LPCTSTR szText)
{
	if (CTextDiagram::SetText(nRect, szText))
	{
		Invalidate();
		return TRUE;
	}

	return FALSE;
}

BOOL CTextDiagramCtrl::SetConnection(int nConn, const CTDConnection& conn)
{
	if (CTextDiagram::SetConnection(nConn, conn))
	{
		UpdateScrollbars();

		Invalidate();
		return TRUE;
	}

	return FALSE;
}

void CTextDiagramCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rClient;
	GetClientRect(rClient);
	dc.IntersectClipRect(rClient);

	// draw rects
	dc.SelectStockObject(ANSI_FIXED_FONT);

	int nRect = 0;
	CTDRect rect;

	while (GetRect(nRect, rect))
	{
		DrawRect(&dc, rect, nRect == m_nSelRect);
		nRect++;
	}

	// draw connections
	int nConn = 0;
	CTDConnection conn;

	while (GetConnection(nConn, conn))
	{
		DrawConn(&dc, conn);
		nConn++;
	}
}

void CTextDiagramCtrl::DrawRect(CDC* pDC, const CTDRect& rect, BOOL bSelected)
{
	CRect rDraw(rect);
	LogicalToClient(rDraw);

	if (!rDraw.IsRectEmpty())
	{
		if (bSelected)
			pDC->FillSolidRect(rDraw, GetSysColor(COLOR_HIGHLIGHT));
		
		pDC->Draw3dRect(rDraw, 0, 0);

		// text
		rDraw.DeflateRect(CHARSIZE_X / 2, CHARSIZE_Y / 2);

		pDC->SetTextColor(GetSysColor(bSelected ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT));
		pDC->SetBkMode(TRANSPARENT);

		RenderText(pDC, rDraw, rect.GetText());
	}
}

void CTextDiagramCtrl::RenderText(CDC* pDC, const CRect& rect, const CString& sText)
{
	int nLen = sText.GetLength(), nX = rect.left, nY = rect.top;

	for (int nChar = 0; nChar < nLen && (nY + CHARSIZE_Y) < rect.bottom; nChar++)
	{
		char c = sText[nChar];

		// new line?
		if (c == '\r')
		{
			nX = rect.left;
			nY += CHARSIZE_Y;
			nChar++; // to account for the trailing '\n'
		}
		else if ((nX + CHARSIZE_X) > rect.right)
		{
			nX = rect.left;
			nY += CHARSIZE_Y;
			nChar--; // try again
		}
		else
		{
			pDC->ExtTextOut(nX, nY, 0, NULL, CString(c), NULL);
			nX += CHARSIZE_X;
		}
	}
}

void CTextDiagramCtrl::DrawConn(CDC* pDC, const CTDConnection& conn)
{
	CPoint ptSegStart = conn.GetStartPos(), ptSegEnd;
	
	CPoint ptFrom(ptSegStart);
	ptFrom.x = ptFrom.x * CHARSIZE_X + CHARSIZE_X / 2;
	ptFrom.y = ptFrom.y * CHARSIZE_Y + CHARSIZE_Y / 2;
	
	CRect rStart(ptFrom.x - 1, ptFrom.y - 1, ptFrom.x + 2, ptFrom.y + 2);

	rStart.OffsetRect(-m_ptScrollOffset);
	ptFrom.Offset(-m_ptScrollOffset);

	pDC->FillSolidRect(rStart, 0);
	pDC->MoveTo(ptFrom);
	
	for (int nSeg = 0; nSeg < conn.NumSegments(); nSeg++)
	{
		CPoint ptTo, ptSegEnd = conn.GetSegmentPos(nSeg);
		
		ptTo.x = ptSegEnd.x * CHARSIZE_X + CHARSIZE_X / 2;
		ptTo.y = ptSegEnd.y * CHARSIZE_Y + CHARSIZE_Y / 2;
		
		ptTo.Offset(-m_ptScrollOffset);
		pDC->LineTo(ptTo);

		ptFrom = ptTo; // for drawing the arrow
	}
	
	DrawArrow(pDC, ptFrom, conn.GetEndDirection());
}

void CTextDiagramCtrl::DrawArrow(CDC* pDC, CPoint point, int nDirection)
{
	switch (nDirection)
	{
	case CONN_UP:
		pDC->MoveTo(point.x - ARROWSIZE, point.y + ARROWSIZE);
		pDC->LineTo(point);
		pDC->LineTo(point.x + (ARROWSIZE + 1), point.y + (ARROWSIZE + 1));
		break;

	case CONN_DOWN:
		pDC->MoveTo(point.x - ARROWSIZE, point.y - ARROWSIZE);
		pDC->LineTo(point);
		pDC->LineTo(point.x + (ARROWSIZE + 1), point.y - (ARROWSIZE + 1));
		break;

	case CONN_LEFT:
		pDC->MoveTo(point.x + ARROWSIZE, point.y - ARROWSIZE);
		pDC->LineTo(point);
		pDC->LineTo(point.x + (ARROWSIZE + 1), point.y + (ARROWSIZE + 1));
		break;

	case CONN_RIGHT:
		pDC->MoveTo(point.x - ARROWSIZE, point.y - ARROWSIZE);
		pDC->LineTo(point);
		pDC->LineTo(point.x - (ARROWSIZE + 1), point.y + (ARROWSIZE + 1));
		break;
	}
}

BOOL CTextDiagramCtrl::OnEraseBkgnd(CDC* pDC) 
{
	CRect rClient, rPage = GetVisiblePageRect();
	GetClientRect(rClient);

	// clip out the selected item
	int nSaveDC = pDC->SaveDC();

	if (m_nSelRect != -1)
	{
		CTDRect rect;
		GetRect(m_nSelRect, rect);

		CRect rDraw(rect);
		LogicalToClient(rDraw);

		pDC->ExcludeClipRect(rDraw);
	}

	pDC->FillSolidRect(rPage, GetSysColor(COLOR_WINDOW));
	
	// fill offpage rect with another color
	if (rPage.right < rClient.right)
	{
		rClient.left = rPage.right;
		pDC->FillSolidRect(rClient, GetSysColor(COLOR_APPWORKSPACE));
	}

	DrawGrid(pDC);

	pDC->RestoreDC(nSaveDC);
	
	return TRUE;
}

CRect CTextDiagramCtrl::GetVisiblePageRect()
{
	CRect rClient;
	GetClientRect(rClient);

	CRect rEdge(m_nPageWidth, 0, m_nPageWidth, m_size.cy);
	LogicalToClient(rEdge);
	rEdge.OffsetRect(-CHARSIZE_X / 2, 0);

	rClient.right = min(rClient.right, rEdge.left);

	return rClient;
}

void CTextDiagramCtrl::DrawGrid(CDC* pDC)
{
	CRect rClient;
	GetClientRect(rClient);
	rClient |= GetBoundingRect();

	for (int nY = CHARSIZE_Y; nY < rClient.bottom; nY += CHARSIZE_Y)
	{
		if ((nY / CHARSIZE_Y) % 10)
			pDC->FillSolidRect(0, nY - m_ptScrollOffset.y, rClient.right, 1, RGB(192, 192, 192));
		else
			pDC->FillSolidRect(0, nY - m_ptScrollOffset.y, rClient.right, 1, RGB(160, 160, 160));
	}

	for (int nX = CHARSIZE_X; nX < rClient.right; nX += CHARSIZE_X)
	{
		if ((nX / CHARSIZE_X) % 10)
			pDC->FillSolidRect(nX - m_ptScrollOffset.x, 0, 1, rClient.bottom, RGB(192, 192, 192));
		else
			pDC->FillSolidRect(nX - m_ptScrollOffset.x, 0, 1, rClient.bottom, RGB(160, 160, 160));
	}
}

void CTextDiagramCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDown(nFlags, point);

	// always make sure existing edit is finished
	EndTextEdit(TRUE);

	m_bDblClick = FALSE;
	SetFocus();

	// update selection
	point.Offset(m_ptScrollOffset);
	int nSel = IntersectRect(point.x / CHARSIZE_X, point.y / CHARSIZE_Y);

	if (nSel != m_nSelRect)
	{
		m_nSelRect = nSel;
		Invalidate();

		SendNotification(TDN_SELCHANGE);
	}

	if (nSel != -1)
	{
		// test for whether we are moving or sizing
		CTDRect rect;
		GetRect(m_nSelRect, rect);

		// deflate this and then check we're still inside
		CRect rDraw(rect);
		LogicalToClient(rDraw);

		rDraw.DeflateRect(2, 2);
		rDraw.OffsetRect(m_ptScrollOffset);

		if (!rDraw.PtInRect(point))
		{
			if (point.x <= rDraw.left)
				m_nSizing = HTLEFT;
			
			else if (point.x >= rDraw.right)
				m_nSizing = HTRIGHT;

			else if (point.y <= rDraw.top)
				m_nSizing = HTTOP;
			
			else if (point.y >= rDraw.bottom)
				m_nSizing = HTBOTTOM;

			else // ??
				ASSERT (0);
		}
		else
			m_bMoving = TRUE;
		
		m_ptDragStart = m_ptDragPrev = point;

		SetCapture();
	}
}

int CTextDiagramCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	UpdateScrollbars();

	return 0;
}

void CTextDiagramCtrl::UpdateScrollbars()
{
	CRect rClient, rDiagram(GetBoundingRect());
	GetClientRect(rClient);

	CPoint ptOffset(m_ptScrollOffset);

	ShowScrollBar(SB_VERT, rClient.bottom < rDiagram.bottom);
	ShowScrollBar(SB_HORZ, rClient.right < rDiagram.right);

	if (rClient.bottom >= rDiagram.bottom)
		m_ptScrollOffset.y = 0;
	else
	{
		m_ptScrollOffset.y = min(m_ptScrollOffset.y, (rDiagram.bottom - rClient.bottom));

		// set proportional scrollbar
		SCROLLINFO si;
		
		si.cbSize = sizeof(si); 
		si.fMask = SIF_ALL & ~SIF_TRACKPOS; 
		si.nMin = 0; 
		si.nMax = rDiagram.bottom; 
		si.nPage = rClient.bottom; 
		si.nPos = m_ptScrollOffset.y; 
		si.nTrackPos = 0; 

		SetScrollInfo(SB_VERT, &si);
	}

	if (rClient.right >= rDiagram.right)
		m_ptScrollOffset.x = 0;
	else
	{
		m_ptScrollOffset.x = min(m_ptScrollOffset.x, (rDiagram.right - rClient.right));

		// set proportional scrollbar
		SCROLLINFO si;
		
		si.cbSize = sizeof(si); 
		si.fMask = SIF_ALL & ~SIF_TRACKPOS; 
		si.nMin = 0; 
		si.nMax = rDiagram.right; 
		si.nPage = rClient.right; 
		si.nPos = m_ptScrollOffset.x; 
		si.nTrackPos = 0; 

		SetScrollInfo(SB_HORZ, &si);
	}

	if (m_ptScrollOffset != ptOffset)
		Invalidate();
}

CRect CTextDiagramCtrl::GetBoundingRect()
{
	return CRect(0, 0, (m_size.cx + 5) * CHARSIZE_X, (m_size.cy + 5) * CHARSIZE_Y);
}

void CTextDiagramCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (EditingText())
		SetFocus();

	CRect rClient, rDiagram(GetBoundingRect());
	GetClientRect(rClient);

	CPoint ptOffset(m_ptScrollOffset);
	
	switch (nSBCode)
	{
	case SB_LEFT:
		m_ptScrollOffset.x = 0;
		break;

	case SB_ENDSCROLL:
		break;

	case SB_LINELEFT:
		m_ptScrollOffset.x = max(m_ptScrollOffset.x - CHARSIZE_X, 0);
		break;

	case SB_LINERIGHT:
		m_ptScrollOffset.x = min(rDiagram.right - rClient.right, m_ptScrollOffset.x + CHARSIZE_X);
		break;

	case SB_PAGELEFT:
		m_ptScrollOffset.x = max(m_ptScrollOffset.x - CHARSIZE_X, 0);
		break;

	case SB_PAGERIGHT:
		m_ptScrollOffset.x = min(rDiagram.right - rClient.right, m_ptScrollOffset.x + CHARSIZE_X);
		break;

	case SB_RIGHT:
		m_ptScrollOffset.x = (rDiagram.right - rClient.right);
		break;

	case SB_THUMBPOSITION:
		m_ptScrollOffset.x = (nPos / CHARSIZE_X) * CHARSIZE_X;
		break;

	case SB_THUMBTRACK:
		m_ptScrollOffset.x = (nPos / CHARSIZE_X) * CHARSIZE_X;
		break;
	}

	if (m_ptScrollOffset != ptOffset)
	{
		Invalidate();
		UpdateScrollbars();
	}
	
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CTextDiagramCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (EditingText())
		SetFocus();

	CRect rClient, rDiagram(GetBoundingRect());
	GetClientRect(rClient);

	CPoint ptOffset(m_ptScrollOffset);
	
	switch (nSBCode)
	{
	case SB_TOP:
		m_ptScrollOffset.y = 0;
		break;

	case SB_ENDSCROLL:
		break;

	case SB_LINEUP:
		m_ptScrollOffset.y = max(m_ptScrollOffset.y - CHARSIZE_Y, 0);
		break;

	case SB_LINEDOWN:
		m_ptScrollOffset.y = min(rDiagram.bottom - rClient.bottom, m_ptScrollOffset.y + CHARSIZE_Y);
		break;

	case SB_PAGEUP:
		m_ptScrollOffset.y = max(m_ptScrollOffset.y - CHARSIZE_Y, 0);
		break;

	case SB_PAGEDOWN:
		m_ptScrollOffset.y = min(rDiagram.bottom - rClient.bottom, m_ptScrollOffset.y + CHARSIZE_Y);
		break;

	case SB_BOTTOM:
		m_ptScrollOffset.y = (rDiagram.bottom - rClient.bottom);
		break;

	case SB_THUMBPOSITION:
		m_ptScrollOffset.y = (nPos / CHARSIZE_Y) * CHARSIZE_Y;
		break;

	case SB_THUMBTRACK:
		m_ptScrollOffset.y = (nPos / CHARSIZE_Y) * CHARSIZE_Y;
		break;
	}

	if (m_ptScrollOffset != ptOffset)
	{
		Invalidate();
		UpdateScrollbars();
	}
	
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CTextDiagramCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	UpdateScrollbars();
}

BOOL CTextDiagramCtrl::RebuildConnections()
{
	if (!CTextDiagram::RebuildConnections())
		return FALSE;

	return TRUE;
}

void CTextDiagramCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bMoving || m_nSizing != HTNOWHERE)
	{
		ASSERT (GetCapture() == this);

		// get the original rect
		CTDRect rect;
		GetRect(m_nSelRect, rect);

		CRect rNew(rect);

		BOOL bStart = (m_ptDragPrev == m_ptDragStart);

		CPoint ptPrev(m_ptDragPrev); // cache
		CPoint ptOrg(point); // cache

		ClientToLogical(ptPrev);
		ClientToLogical(point);
		
		// get new pos
		if (m_bMoving)
		{
			rect.OffsetRect(ptPrev);
			rNew.OffsetRect(point);

			// prevent the rects preceding the origin
			rect.OffsetRect(max(0, 1 - rect.left), max(0, 1 - rect.top));
			rNew.OffsetRect(max(0, 1 - rNew.left), max(0, 1 - rNew.top));
		}
		else
		{
			switch (m_nSizing)
			{
			case HTLEFT:
				rect.left += ptPrev.x;
				rNew.left += point.x;

				rect.left = max(1, min(rect.left, rect.right - 2));
				rNew.left = max(1, min(rNew.left, rNew.right - 2));
				break;

			case HTRIGHT:
				rect.right += ptPrev.x;
				rNew.right += point.x;

				rect.right = max(rect.right, rect.left + 2);
				rNew.right = max(rNew.right, rNew.left + 2);
				break;

			case HTTOP:
				rect.top += ptPrev.y;
				rNew.top += point.y;

				rect.top = max(1, min(rect.top, rect.bottom - 2));
				rNew.top = max(1, min(rNew.top, rNew.bottom - 2));
				break;

			case HTBOTTOM:
				rect.bottom += ptPrev.y;
				rNew.bottom += point.y;

				rect.bottom = max(rect.bottom, rect.top + 2);
				rNew.bottom = max(rNew.bottom, rNew.top + 2);
				break;
			}
		}

		if (rNew != rect)
		{
			// set the cursor
			rNew.InflateRect(1, 1);

			if (IntersectRect(rNew, m_nSelRect) || rNew.left < 0 || rNew.top < 0)
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_NO));
			else
			{
				if (m_bMoving)
					SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
				else
				{
					switch (m_nSizing)
					{
					case HTLEFT:
					case HTRIGHT:
						SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
						break;

					case HTTOP:
					case HTBOTTOM:
						SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
						break;
					}
				}
			}

			// draw rect tracker
			rNew.DeflateRect(1, 1);

			LogicalToClient(rect);
			LogicalToClient(rNew);

			CDC* pDC = GetDC();
			pDC->DrawDragRect(rNew, CSize(2, 2), bStart ? NULL : &rect, CSize(2, 2));
			ReleaseDC(pDC);

			m_ptDragPrev = ptOrg;
		}
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

void CTextDiagramCtrl::SnapToGrid(CRect& rect)
{
	rect.left = (rect.left / CHARSIZE_X) * CHARSIZE_X + CHARSIZE_X / 2;
	rect.right = (rect.right / CHARSIZE_X) * CHARSIZE_X + CHARSIZE_X / 2;
	rect.top = (rect.top / CHARSIZE_Y) * CHARSIZE_Y + CHARSIZE_Y / 2;
	rect.bottom = (rect.bottom / CHARSIZE_Y) * CHARSIZE_Y + CHARSIZE_Y / 2;
}

void CTextDiagramCtrl::ClientToLogical(CPoint& point)
{
	point.Offset(m_ptScrollOffset);
	
	if (m_bMoving || m_nSizing != HTNOWHERE)
		point.Offset(-m_ptDragStart);

	// convert to logical coords
	point.x = point.x / CHARSIZE_X;
	point.y = point.y / CHARSIZE_Y;
}

void CTextDiagramCtrl::LogicalToClient(CRect& rect)
{
	rect.left *= CHARSIZE_X;
	rect.right *= CHARSIZE_X;
	rect.top *= CHARSIZE_Y;
	rect.bottom *= CHARSIZE_Y;

	rect.OffsetRect(CHARSIZE_X / 2, CHARSIZE_Y / 2);

	rect.right++;
	rect.bottom++;

	rect.OffsetRect(-m_ptScrollOffset);
}

void CTextDiagramCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonUp(nFlags, point);

	if (m_bMoving || m_nSizing != HTNOWHERE)
	{
		ClientToLogical(point);

		CTDRect rect;
		GetRect(m_nSelRect, rect);

		CRect rNew(rect);

		// get new pos
		if (m_bMoving)
		{
			rNew.OffsetRect(point);

			// prevent the rect preceding the origin
			rNew.OffsetRect(max(0, 1 - rNew.left), max(0, 1 - rNew.top));
		}
		else
		{
			switch (m_nSizing)
			{
			case HTLEFT:
				rNew.left += point.x;
				rNew.left = max(1, min(rNew.left, rNew.right - 2));
				break;

			case HTRIGHT:
				rNew.right += point.x;
				rNew.right = max(rNew.right, rNew.left + 2);
				break;

			case HTTOP:
				rNew.top += point.y;
				rNew.top = max(1, min(rNew.top, rNew.bottom - 2));
				break;

			case HTBOTTOM:
				rNew.bottom += point.y;
				rNew.bottom = max(rNew.bottom, rNew.top + 2);
				break;
			}
		}

		// test the new pos
		if (rNew != rect)
		{
			rect.CopyRect(rNew);

			if (SetRect(m_nSelRect, rect))
				SendNotification(TDN_SELRECTMOVE);
		}
		
		m_bMoving = FALSE;
		m_nSizing = HTNOWHERE;

		ReleaseCapture();
		Invalidate();
	}
	else if (m_bDblClick)
	{
		if (m_nSelRect != -1)
			EditSelectedText();
		else
			SendNotification(TDN_DBLCLK, point);
	}
}

void CTextDiagramCtrl::EditSelectedText(BOOL bResize)
{
	m_bResizeOnEdit = FALSE; // default
	ASSERT (m_nSelRect != -1);

	if (m_nSelRect != -1)
	{
		// make sure the selected rect is visible
		EnsureVisible(m_nSelRect);

		CTDRect rect;
		
		if (GetRect(m_nSelRect, rect))
		{
			CRect rPos(rect);
			LogicalToClient(rPos);
			
			if (!m_editText.GetSafeHwnd())
			{
				m_editText.Create(WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL, rPos, this, IDC_EDITTEXT);
				m_editText.SetFont(CFont::FromHandle((HFONT)GetStockObject(ANSI_FIXED_FONT)));
			}
			else
			{
				m_editText.ShowWindow(SW_SHOW);
				m_editText.MoveWindow(rPos);
			}
			
			rPos.OffsetRect(-rPos.TopLeft());
			rPos.left += CHARSIZE_X / 2 - 2;
			rPos.top += CHARSIZE_Y / 2 - 2;
			
			m_editText.SetRect(rPos);
			m_editText.SetWindowText(rect.GetText());
			m_editText.SetFocus();

			m_bResizeOnEdit = TRUE;
		}
	}
}

void CTextDiagramCtrl::EnsureVisible(int nRect)
{
	CRect rect;
		
	if (GetRect(nRect, rect))
	{
		LogicalToClient(rect);

		CRect rClient, rInt;
		GetClientRect(rClient);

		// horz
		if (rect.left < rClient.left)
			SendMessage(WM_HSCROLL, MAKEWPARAM(SB_THUMBPOSITION, m_ptScrollOffset.x + rect.left), 0);

		else if (rect.right > rClient.right)
			SendMessage(WM_HSCROLL, MAKEWPARAM(SB_THUMBPOSITION, m_ptScrollOffset.x + rect.right + CHARSIZE_X - rClient.Width()), 0);

		// vert
		if (rect.top < rClient.top)
			SendMessage(WM_VSCROLL, MAKEWPARAM(SB_THUMBPOSITION, m_ptScrollOffset.y + rect.top), 0);

		else if (rect.bottom > rClient.bottom)
			SendMessage(WM_VSCROLL, MAKEWPARAM(SB_THUMBPOSITION, m_ptScrollOffset.y + rect.bottom + CHARSIZE_Y - rClient.Height()), 0);
	}
}

void CTextDiagramCtrl::OnCaptureChanged(CWnd *pWnd) 
{
	if (m_bMoving && pWnd == this)
	{
		// just cancel
		m_bMoving = FALSE;
		Invalidate();
	}
	
	CWnd::OnCaptureChanged(pWnd);
}

UINT CTextDiagramCtrl::OnNcHitTest(CPoint point) 
{
	
	return CWnd::OnNcHitTest(point);
}

BOOL CTextDiagramCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (pWnd == this && nHitTest == HTCLIENT && m_nSelRect != -1)
	{
		CPoint point;

		GetCursorPos(&point);
		ScreenToClient(&point);
		point.Offset(m_ptScrollOffset);

		int nRect = IntersectRect(point.x / CHARSIZE_X, point.y / CHARSIZE_Y);

		if (nRect == m_nSelRect)
		{
			// see if we're on the rect border
			CTDRect rect;
			GetRect(m_nSelRect, rect);

			CRect rDraw(rect);
			LogicalToClient(rDraw);

			rDraw.OffsetRect(m_ptScrollOffset);

			// deflate this and then check we're still inside
			rDraw.DeflateRect(2, 2);

			if (!rDraw.PtInRect(point))
			{
				if (point.x < rDraw.left || point.x > rDraw.right)
					return (BOOL)SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
				else
					return (BOOL)SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
			}
		}
	}
	
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

int CTextDiagramCtrl::HitTestConn(CPoint point)
{
	// convert to logical coords
	ClientToLogical(point);

	int nConn = IntersectConn(point, TRUE);

	if (nConn == -1)
		nConn = IntersectConn(point, FALSE);
	
	return nConn;
}

int CTextDiagramCtrl::HitTestRect(CPoint point)
{
	// convert to logical coords
	ClientToLogical(point);

	return IntersectRect(point);
}

void CTextDiagramCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDblClk(nFlags, point);
	
	m_bDblClick = TRUE;
}

UINT CTextDiagramCtrl::OnGetDlgCode() 
{
	if (m_nSelRect != -1)
		return DLGC_WANTARROWS;
	
	return CWnd::OnGetDlgCode();
}

void CTextDiagramCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (m_nSelRect != -1)
	{
		CTDRect rect;
		GetRect(m_nSelRect, rect);

		switch (nChar)
		{
		case VK_DELETE:
			if (DeleteRect(m_nSelRect))
			{
				UpdateWindow();

				m_nSelRect = -1;
				SendNotification(TDN_SELRECTDELETE);
			}
			break;
			
		case VK_LEFT: 
			{
				rect.OffsetRect(-1, 0);

				if (SetRect(m_nSelRect, rect))
				{
					UpdateWindow();
					SendNotification(TDN_SELRECTMOVE);
				}
			}
			break; 
			
		case VK_RIGHT: 
			{
				rect.OffsetRect(1, 0);

				if (SetRect(m_nSelRect, rect))
				{
					UpdateWindow();
					SendNotification(TDN_SELRECTMOVE);
				}
			}
			break; 
			
		case VK_UP: 
			{
				rect.OffsetRect(0, -1);

				if (SetRect(m_nSelRect, rect))
				{
					UpdateWindow();
					SendNotification(TDN_SELRECTMOVE);
				}
			}
			break; 
			
		case VK_DOWN: 
			{
				rect.OffsetRect(0, 1);

				if (SetRect(m_nSelRect, rect))
				{
					UpdateWindow();
					SendNotification(TDN_SELRECTMOVE);
				}
			}
			break;

		case VK_F2:
			EditSelectedText();
			break;
		}
	}
	
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTextDiagramCtrl::OnEndTextEdit()
{
	if (EditingText())
	{
		if (m_nSelRect != -1 && m_editText.IsWindowVisible())
		{
			CString sItemText;

			if (GetText(m_nSelRect, sItemText))
			{
				CString sText;
				m_editText.GetWindowText(sText);

				if (sText != sItemText)
				{
					SetText(m_nSelRect, sText);

					if (m_bResizeOnEdit)
					{
						// TODO
					}

					SendNotification(TDN_SELRECTTEXTCHANGE);
				}
			}
		}
			
		m_editText.ShowWindow(SW_HIDE);
	}

	m_bResizeOnEdit = FALSE; // reset
}


BOOL CTextDiagramCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->hwnd == m_editText)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
			EndTextEdit(TRUE);
			return TRUE;

		case VK_RETURN:
			if (!(GetAsyncKeyState(VK_CONTROL) & 0x8000))
			{
				EndTextEdit(FALSE);
				return TRUE;
			}
			break;
		}
	}
	
	return CWnd::PreTranslateMessage(pMsg);
}

void CTextDiagramCtrl::EndTextEdit(BOOL bCancel)
{
	if (EditingText())
	{
		if (bCancel)
		{
			// hiding the window first causes the text changes to be ignored
			m_editText.ShowWindow(SW_HIDE);
			SetFocus(); 
		}
		else
		{
			SetFocus(); 
			m_editText.ShowWindow(SW_HIDE);
		}
	}
}

void CTextDiagramCtrl::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// always make sure existing edit is finished
	EndTextEdit(TRUE);

	CMenu menu;

	if (!menu.CreatePopupMenu())
		return;

	// what have we hit
	ScreenToClient(&point);
	m_ptContextMenu = point; // cache for command handlers

	int nRect = HitTestRect(point);

	if (nRect != -1)
	{
		// remove selection
		if (m_nSelRect != nRect)
		{
			m_nSelRect = nRect;
			Invalidate();

			SendNotification(TDN_SELCHANGE);
		}
		
		// build menu
		menu.AppendMenu(MF_STRING, ID_RECT_EDITTEXT, "Edit Box Text");
//		menu.AppendMenu(MF_STRING, ID_RECT_RESIZETOFIT, "Resize Box to Suit Text");
		menu.AppendMenu(MF_SEPARATOR);
		menu.AppendMenu(MF_STRING, ID_CONN_NEW, "New Connection To...");
		menu.AppendMenu(MF_SEPARATOR);
		menu.AppendMenu(MF_STRING, ID_RECT_DELETE, "Delete Box");
		menu.AppendMenu(MF_SEPARATOR);
		menu.AppendMenu(MF_STRING, IDCANCEL, "Cancel\tEsc");
	}
	else
	{
		// remove selection
		if (m_nSelRect != -1)
		{
			m_nSelRect = -1;
			Invalidate();

			SendNotification(TDN_SELCHANGE);
		}
		
		int nConn = HitTestConn(point);

		if (nConn != -1)
		{
			// build menu
			menu.AppendMenu(MF_STRING, ID_CONN_MODIFY, "Modify Connection...");
			menu.AppendMenu(MF_STRING, ID_CONN_FLIP, "Flip Connection");
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING, ID_CONN_DELETE, "Delete Connection");
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING, IDCANCEL, "Cancel\tEsc");

			// disable 'flip' if a conn already exists in the other dir
			CTDConnection conn;
			VERIFY(GetConnection(nConn, conn));

			BOOL bFound = (FindConnection(conn.RectTo(), conn.RectFrom()) != -1);
			menu.EnableMenuItem(ID_CONN_FLIP, bFound ? MF_GRAYED : MF_ENABLED);
		}
		else
		{
			menu.AppendMenu(MF_STRING, ID_RECT_NEW, "New Box");
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING, ID_RESET, "Reset Diagram");
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING, IDCANCEL, "Cancel\tEsc");

			// determine whether 'new box' is enabled or not
			CPoint ptCentre(point);
			ClientToLogical(ptCentre);

			CRect rNew(ptCentre.x - 3, ptCentre.y - 2, ptCentre.x + 2, ptCentre.y + 1);
			rNew.InflateRect(1, 1);

			menu.EnableMenuItem(ID_RECT_NEW, IntersectRect(rNew) ? MF_GRAYED : MF_ENABLED);
		}
	}

	ClientToScreen(&point);

	menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}

void CTextDiagramCtrl::OnRectNew()
{
	if (HitTestRect(m_ptContextMenu) == -1 && HitTestConn(m_ptContextMenu) == -1)
	{
		CPoint ptTopLeft(m_ptContextMenu);
		ClientToLogical(ptTopLeft);

		if (AddRect(CRect(ptTopLeft, CSize(5, 3))) != -1)
		{
			EditSelectedText();
		}
	}
}

void CTextDiagramCtrl::OnRectDelete()
{
	ASSERT (m_nSelRect == HitTestRect(m_ptContextMenu));
	
	if (m_nSelRect != -1)
		DeleteRect(m_nSelRect);
}

void CTextDiagramCtrl::OnRectEditText()
{
	ASSERT (m_nSelRect == HitTestRect(m_ptContextMenu));
	
	if (m_nSelRect != -1)
		EditSelectedText();
}

void CTextDiagramCtrl::OnConnNew()
{
	ASSERT (m_nSelRect == HitTestRect(m_ptContextMenu));
	
	if (m_nSelRect != -1)
	{
		// short term fudge sends a notification to the parent to provide this functionality
		SendNotification(TDN_IMPLCONNNEW, m_ptContextMenu);
	}
}

void CTextDiagramCtrl::OnConnModify()
{
	int nConn = HitTestConn(m_ptContextMenu);
	
	if (nConn != -1)
	{
		// short term fudge sends a notification to the parent to provide this functionality
		SendNotification(TDN_DBLCLK, m_ptContextMenu);
	}
}

void CTextDiagramCtrl::OnConnDelete()
{
	int nConn = HitTestConn(m_ptContextMenu);
	
	if (nConn != -1)
		DeleteConnection(nConn);
}

void CTextDiagramCtrl::OnReset()
{
	if (MessageBox("Are you sure you want to reset the diagram", "Code Plotter", MB_YESNO) == IDYES)
		ResetDiagram();
}

void CTextDiagramCtrl::SendNotification(UINT nCode, CPoint point)
{
	TDNHDR tdn = { { *this, GetDlgCtrlID(), nCode }, { point.x, point.y } };

	GetParent()->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&tdn);
}

void CTextDiagramCtrl::OnChangeText()
{
}

void CTextDiagramCtrl::OnConnFlip()
{
	int nConn = HitTestConn(m_ptContextMenu);
	
	if (nConn != -1)
	{
		CTDConnection conn;
		VERIFY(GetConnection(nConn, conn));

		// work out which side the existing conn hits the target rect
		CTDRect rect;
		VERIFY(GetRect(conn.RectTo(), rect));

		int nSideFrom = rect.GetSide(conn.GetEndPos());
		ASSERT (nSideFrom != NONE);

		SetConnection(nConn, CTDConnection(conn.RectTo(), conn.RectFrom(), nSideFrom));
		Invalidate();
	}
}
