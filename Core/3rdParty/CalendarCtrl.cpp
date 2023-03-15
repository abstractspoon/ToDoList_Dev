/* PERF__FREZ/PUB
 * ====================================================
 * CalendarCtrl.cpp : implementation file
 * Frederic Rezeau
 * 
 * Perf'Control Personal Edition calendar 
 *
 * If you like it, feel free to use it. 
 *
 * www.performlabs.com 
 * ==================================================== 
 */

#include "stdafx.h"
#include "CalendarCtrl.h"

#include <math.h>

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

COleDateTime NullDate()
{
	static COleDateTime null;

	null.m_dt = 0;
	null.m_status = COleDateTime::null;

	return null;
}

/////////////////////////////////////////////////////////////////////////////
// CCalendarCtrl

CCalendarCtrl::CCalendarCtrl() 
	: 
	m_nVisibleWeeks(6), 
	m_bEnableMultiSel(FALSE),
	m_bDrawGridOverCells(TRUE),
	m_crGrid(RGB(125,175,255)),
	m_nHeaderHeight(CALENDAR_MIN_HEADER_HEIGHT),
	m_nDayHeaderHeight(CALENDAR_MIN_DAY_HEADER_HEIGHT)
{
	LOGFONT lf;
	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT),sizeof(lf),&lf);
	lstrcpy(lf.lfFaceName, _T("Arial"));
	lf.lfHeight = 13;
	m_DefaultFont.CreateFontIndirect(&lf);

	m_bMonthIsOdd		= false;
	m_bSelectionStarted = false;
	m_SelectionRange[0]	= 0;
	m_SelectionRange[1]	= 0;
	m_SelectionRange[2]	= 0;
	m_nVscrollMax		= 110;
	m_nVscrollPos		= m_nVscrollMax/2;
	m_BoundDown			= COleDateTime(1999,12,31,0,0,0); 
	m_BoundUp			= COleDateTime(2036,1,1,0,0,0);
	m_nMaxSel			= 42;
	m_nFirstWeekDay		= 1;
	m_pfnDataCallback	= NULL;
	m_crTheme			= GetSysColor(COLOR_3DFACE);
}

CCalendarCtrl::~CCalendarCtrl()
{
}

BEGIN_MESSAGE_MAP(CCalendarCtrl, CWnd)
	//{{AFX_MSG_MAP(CCalendarCtrl)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_GETDLGCODE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL CCalendarCtrl::Create(DWORD dwStyle, const CRect &wndRect, CWnd *pParent, UINT nID, LPFN_CALENDAR_DATA_CALLBACK pfnDataCallback)
{
	BOOL bResult = CWnd::Create(NULL, NULL, dwStyle, wndRect, pParent, nID);
	m_pfnDataCallback = pfnDataCallback;

    SetScrollRange(SB_VERT, 0, m_nVscrollMax, FALSE);
	Reset();

	return bResult;
}

BOOL CCalendarCtrl::SetVisibleWeeks(int nWeeks)
{
	ASSERT(nWeeks > 0 && nWeeks <= CALENDAR_MAX_ROWS);

	if (nWeeks < 1 || nWeeks > CALENDAR_MAX_ROWS)
		return FALSE;

	if (nWeeks != m_nVisibleWeeks)
	{
		m_nVisibleWeeks = nWeeks;

		Invalidate();
		OnVisibleDateRangeChanged();
	}
	
	return TRUE;
}

void CCalendarCtrl::Reset()
{
	COleDateTime today(time(NULL));
	COleDateTime dtFirstDay(today.GetYear(), today.GetMonth(), 1,0,0,0); 
	
	if (dtFirstDay > m_BoundUp) 
		dtFirstDay = m_BoundUp;

	else if (dtFirstDay < m_BoundDown)	
		dtFirstDay = m_BoundDown;

	if (dtFirstDay.GetMonth()%2) 
		m_bMonthIsOdd = true;

	Goto(dtFirstDay);

	// Reset selection
	m_SelectionRange[1] = DateToSeconds(COleDateTime(today.GetYear(), today.GetMonth(), today.GetDay(),0,0,0));
	m_SelectionRange[0] = m_SelectionRange[1];

	// Reset scroll pos
	m_nVscrollPos = m_nVscrollMax/2;
	SetScrollPos(SB_VERT, m_nVscrollPos, TRUE);
}

void CCalendarCtrl::GotoToday(bool bSelect) 
{ 
	Goto(COleDateTime::GetCurrentTime(), bSelect); 
}

void CCalendarCtrl::Goto(const COleDateTime& dtDate, bool bSelect)
{
	if ((dtDate < m_BoundUp) && 
		(dtDate > m_BoundDown))
	{
		m_DateCurrent = WholeDays(dtDate);	
		
		if (m_nFirstWeekDay<1 || m_nFirstWeekDay>7)
			m_nFirstWeekDay = 1;

		COleDateTime dtIte = m_DateCurrent;
		int narr[7];

		for (int d=0; d<7; d++)	
			narr[((m_nFirstWeekDay-1)+d)%7] = d;

		int nCellStart = narr[dtIte.GetDayOfWeek()-1], i;

		for(i=0; i<nCellStart; i++)	
			dtIte -= COleDateTimeSpan(1,0,0,0);

		for(i=0; i<CALENDAR_MAX_ROWS ; i++)
		{
			for(int u=0; u<CALENDAR_NUM_COLUMNS; u++)
			{
				// Init the cell
				m_dayCells[i][u].date = dtIte;
				m_dayCells[i][u].bPartial = false;
				m_dayCells[i][u].bMark = false;
				m_dayCells[i][u].csaLines.RemoveAll();

				if (m_pfnDataCallback)
					m_pfnDataCallback(this, DateToSeconds(m_dayCells[i][u].date));

				dtIte += COleDateTimeSpan(1,0,0,0);
			}
		}

		if (bSelect)
		{
			m_SingleSelection.RemoveAll();

			m_SelectionRange[2] = 0;
			m_SelectionRange[1] = DateToSeconds(m_DateCurrent);
			m_SelectionRange[0] = m_SelectionRange[1];

			// Scrolling pos
			COleDateTime today = WholeDays(COleDateTime::GetCurrentTime());
			m_nVscrollPos = (m_nVscrollMax/2) + (m_DateCurrent-today).GetDays()/7;

			SetScrollPos(SB_VERT, m_nVscrollPos, TRUE);
			NotifyParentClick();
		}

		OnVisibleDateRangeChanged(); // for derived classes
	}

	Invalidate(true);
}

void CCalendarCtrl::UpdateCells()
{
	if (m_pfnDataCallback)
		Goto(m_DateCurrent, false);
}

void CCalendarCtrl::SetThemeColour(COLORREF crTheme)
{
	if (crTheme == CLR_NONE)
		m_crTheme = GetSysColor(COLOR_3DFACE);
	else
		m_crTheme = crTheme;
	
	if (GetSafeHwnd())
		Invalidate();
}

void CCalendarCtrl::SetHeaderHeight(int nHeight)
{
	nHeight = max(nHeight, CALENDAR_MIN_HEADER_HEIGHT);

	if (m_nHeaderHeight != nHeight)
	{
		m_nHeaderHeight = nHeight;
		Invalidate(FALSE);
	}
}

void CCalendarCtrl::SetDayHeaderHeight(int nHeight)
{
	nHeight = max(nHeight, CALENDAR_MIN_DAY_HEADER_HEIGHT);

	if (m_nDayHeaderHeight != nHeight)
	{
		m_nDayHeaderHeight = nHeight;
		Invalidate(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCalendarCtrl message handlers

void CCalendarCtrl::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(&rc);

	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	CBitmap MemBitmap;
	MemBitmap.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
	CBitmap *pOldBitmap = MemDC.SelectObject(&MemBitmap);

	MemDC.FillSolidRect(&rc, GetSysColor(COLOR_WINDOW));
	MemDC.SetBkMode(TRANSPARENT);
	
	CFont* pOldFont = MemDC.SelectObject(&m_DefaultFont);

	// Draw calendar elements
	DrawHeader(&MemDC);			

	if (m_bDrawGridOverCells)
	{
		DrawCells(&MemDC);
		DrawGrid(&MemDC);
	}
	else
	{
		DrawGrid(&MemDC);
		DrawCells(&MemDC);
	}

	MemDC.SelectObject(pOldFont);

	// Render
	dc.BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(pOldBitmap);
}

const CCalendarCell* CCalendarCtrl::GetCell(int nRow, int nCol) const
{
	ASSERT(nRow >= 0 && nRow < CALENDAR_MAX_ROWS);
	ASSERT(nCol >= 0 && nCol < CALENDAR_NUM_COLUMNS);

	return &(m_dayCells[nRow][nCol]);
}

CCalendarCell* CCalendarCtrl::GetCell(int nRow, int nCol)
{
	ASSERT(nRow >= 0 && nRow < CALENDAR_MAX_ROWS);
	ASSERT(nCol >= 0 && nCol < CALENDAR_NUM_COLUMNS);

	return &(m_dayCells[nRow][nCol]);
}

BOOL CCalendarCtrl::GetGridCell(const COleDateTime& date, int &nRow, int &nCol) const
{
	int nMinDate = (int)m_dayCells[0][0].date.m_dt, nDate = (int)date.m_dt;
	int nCell = (nDate - nMinDate);

	if (nCell < 0 || (nCell >= (m_nVisibleWeeks * CALENDAR_NUM_COLUMNS)))
		return false;

	nRow = (nCell / m_nVisibleWeeks);
	nCol = (nCell % CALENDAR_NUM_COLUMNS);

	return true;
}

BOOL CCalendarCtrl::PtInHeader(const CPoint& ptScreen) const
{
	CPoint ptHeader(ptScreen);
	ScreenToClient(&ptHeader);

	CRect rHeader;
	GetClientRect(rHeader);
	rHeader.bottom = rHeader.top + m_nHeaderHeight;

	return rHeader.PtInRect(ptHeader);
}

int CCalendarCtrl::GetDayOfWeek(int nColumn) const
{
	return ((((m_nFirstWeekDay - 1) + nColumn) % 7) + 1);
}

void CCalendarCtrl::DrawHeader(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	rc.bottom = rc.top + m_nHeaderHeight;

	CRect rcLine(rc);
	rcLine.top = rcLine.bottom-1;
	int i;
	for(i=0; i<m_nHeaderHeight; i++)
	{
		pDC->FillSolidRect(rcLine, GetFadedThemeColour(i*4));
		rcLine.bottom--;
		rcLine.top = rcLine.bottom-1;
	}

	int nWidth = rc.Width()/CALENDAR_NUM_COLUMNS;
	bool bShort = false;
	for(i=0; i<CALENDAR_NUM_COLUMNS; i++)
	{
		CRect txtRect(i*nWidth, 0, (i+1)*nWidth, m_nHeaderHeight);
		CSize TitleSize(pDC->GetTextExtent(GetDayOfWeekName(m_dayCells[0][i].date)));

		if(TitleSize.cx	> txtRect.Width())
		{
			bShort = true;
			break;
		}
	}

	for(i=0 ; i<CALENDAR_NUM_COLUMNS; i++)
	{
		CString csTitle(GetDayOfWeekName(m_dayCells[0][i].date, bShort));
		CRect txtRect(i*nWidth + 2, 2, (i+1)*nWidth, m_nHeaderHeight);
		pDC->DrawText(csTitle, txtRect, DT_CENTER|DT_VCENTER);
	}
}

void CCalendarCtrl::DrawGrid(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	int nHeight = (rc.Height()-m_nHeaderHeight)/GetVisibleWeeks();
	int nWidth = rc.Width()/CALENDAR_NUM_COLUMNS;

	CPen thinPen(PS_SOLID, 1, m_crGrid);
	CPen* pOldPen = pDC->SelectObject(&thinPen);

	int i;

	for(i=1; i<CALENDAR_NUM_COLUMNS; i++)
	{
		pDC->MoveTo(i*nWidth, m_nHeaderHeight);
		pDC->LineTo(i*nWidth, rc.Height());
	}
	
	for(i=1; i<GetVisibleWeeks(); i++)
	{
		pDC->MoveTo(0,			(i*nHeight)+m_nHeaderHeight);
		pDC->LineTo(rc.Width(),	(i*nHeight)+m_nHeaderHeight);
	}
	pDC->SelectObject(pOldPen);
}

void CCalendarCtrl::DrawCells(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);

	for(int i=0; i<GetVisibleWeeks() ; i++)
	{
		for(int u=0; u<CALENDAR_NUM_COLUMNS ; u++)
		{
			CRect rCell;

			if (GetGridCellRect(i, u, rCell))
			{
				const CCalendarCell* pCell = GetCell(i, u);

				if (u == CALENDAR_NUM_COLUMNS-1) 
					rCell.right = rc.right;

				if (i == GetVisibleWeeks()-1) 
					rCell.bottom = rc.bottom;

				// test for showing month
				BOOL bShowMonth = ((pCell->date.GetDay() == 1) || (i==0 && u==0));

				// test for today
				COleDateTime today(time(NULL));
				
				bool bToday = (today.GetDay() == pCell->date.GetDay() && 
								today.GetMonth() == pCell->date.GetMonth() && 
								today.GetYear() == pCell->date.GetYear());

				// test for selected
				bool bSelected = false;
				time_t tcur = DateToSeconds(pCell->date);	
				
				if (m_SingleSelection.GetCount())
				{	
					m_SingleSelection.Lookup(tcur, bSelected);
				}
				else
				{
					time_t tmax = max(m_SelectionRange[0], m_SelectionRange[1]);
					time_t tmin = min(m_SelectionRange[0], m_SelectionRange[1]);		
					
					bSelected = ((tmax >= tcur) && (tcur >= tmin));
				}

				int nSaveDC = pDC->SaveDC();
				pDC->IntersectClipRect(rCell);

				DrawCellBkgnd(pDC, pCell, rCell, bSelected, bToday);

				// Don't overdraw grid
				if (!m_bDrawGridOverCells && (m_crGrid != CLR_NONE) && (u > 0))
					rCell.left++;

				DrawCell(pDC, pCell, rCell, bSelected, bToday, bShowMonth);

				// focus rect
				if (m_SelectionRange[2] == tcur)
					DrawCellFocus(pDC, pCell, rCell);

				pDC->RestoreDC(nSaveDC);
			}
		}
	}
}

void CCalendarCtrl::DrawCellFocus(CDC* pDC, const CCalendarCell* /*pCell*/, const CRect& rCell)
{
	CRect rFocus(rCell);
	rFocus.DeflateRect(1, 1, 0, 0);
	
	pDC->DrawFocusRect(rFocus);
}

CString CCalendarCtrl::GetDayOfWeekName(const COleDateTime& date, BOOL bShort) const
{
	CString csTitle(date.Format(_T("%A")));
	return (bShort ? csTitle.Left(3) : csTitle);
}

CString CCalendarCtrl::GetMonthName(const COleDateTime& date, BOOL bShort) const
{
	return (bShort ? date.Format(_T("%B")) : date.Format(_T("%b")));
}

void CCalendarCtrl::DrawCellBkgnd(CDC* pDC, const CCalendarCell* pCell, const CRect& rCell, BOOL bSelected, BOOL bToday)
{
	// don't draw over gridline
	CRect rBack(rCell);

	if ((m_crGrid != CLR_NONE) && !m_bDrawGridOverCells)
	{
		rBack.top++;

		if (rBack.left > 0)
			rBack.left++;
	}

	if ((m_bMonthIsOdd && !(pCell->date.GetMonth()%2)) ||
		(!m_bMonthIsOdd && (pCell->date.GetMonth()%2)))
	{
		pDC->FillSolidRect(rCell, CALENDAR_LIGHTGREY);
	}
	
	if (bToday)
	{
		CRect selRect(rCell);
		selRect.bottom = selRect.top + m_nDayHeaderHeight - 1;

		pDC->FillSolidRect(rCell, m_crTheme);
	}
	
	// Draw the selection
	if (bSelected)
	{	
		CRect selRect(rCell);

		if (bToday)	
			selRect.top += m_nDayHeaderHeight;

		pDC->FillSolidRect(rCell, GetFadedThemeColour(40));
	}
	
	// Out of range
	if ((pCell->date >= m_BoundUp) || 
		(pCell->date <= m_BoundDown))	
	{
		pDC->FillSolidRect(rCell, RGB(255,225,225));
	}
}

void CCalendarCtrl::DrawCell(CDC* pDC, const CCalendarCell* pCell, const CRect& rCell, 
							 BOOL bSelected, BOOL bToday, BOOL bShowMonth)
{
	if (pCell->bMark)
	{
		CRect rcMark(rCell);
		rcMark.DeflateRect(3,3);
		rcMark.right = rcMark.left +6;
		rcMark.bottom = rcMark.top +6;

		pDC->FillSolidRect(rcMark, RGB(255,104,4));
	}

	CRect rHeader(rCell);
	rHeader.bottom = rHeader.top + m_nDayHeaderHeight;

	DrawCellHeader(pDC, pCell, rHeader, bShowMonth);

	// Draw contents
	CRect rContent(rCell);
	rContent.top += m_nDayHeaderHeight;		
	
	DrawCellContent(pDC, pCell, rContent, bSelected, bToday);
}

void CCalendarCtrl::DrawCellHeader(CDC* pDC, const CCalendarCell* pCell, const CRect& rHeader, BOOL bShowMonth)
{
	// draw day/month numbers
	CString csDay;
	int nDay = pCell->date.GetDay();

	// Draw the first of any month in bold
	CFont fontBold, *pOldFont = NULL;

	if (nDay == 1)
	{
		LOGFONT lf;
		m_DefaultFont.GetLogFont(&lf);

		lf.lfWeight = FW_BOLD;
		fontBold.CreateFontIndirect(&lf);

		pOldFont = pDC->SelectObject(&fontBold);
	}
	
	if (bShowMonth)
	{
		csDay.Format(_T("%d %s"), nDay, GetMonthName(pCell->date, FALSE));
		CSize dtSize(pDC->GetTextExtent(csDay));

		if (dtSize.cx>rHeader.Width())
			csDay.Format(_T("%d %s"), nDay, GetMonthName(pCell->date, TRUE));
	}
	else
	{
		csDay.Format(_T("%d"), nDay);
	}
	
	CRect rText(rHeader);
	rText.DeflateRect(2, 3, 3, 0); // padding
	
	pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(csDay, &rText, DT_LEFT|DT_VCENTER);

	// cleanup
	if (fontBold.GetSafeHandle())
		pDC->SelectObject(pOldFont);
}

void CCalendarCtrl::DrawCellContent(CDC* pDC, const CCalendarCell* pCell, const CRect& rCell, 
									BOOL /*bSelected*/, BOOL /*bToday*/)
{
	// Draw the cell content if possible
	if (rCell.Width() >= 15)
	{
		for (int j=0; j<pCell->csaLines.GetSize(); j++)
		{
			CRect txtRect(rCell);
			CRect dotRect(rCell);
			
			txtRect.left += 9; 
			txtRect.right-= 2;
			txtRect.top += (j+1)*CALENDAR_LINE_HEIGHT; 
			
			dotRect.top += (j+1)*CALENDAR_LINE_HEIGHT+(CALENDAR_LINE_HEIGHT/2-1); 
			dotRect.bottom = dotRect.top + 3;
			dotRect.left += 3;
			dotRect.right = dotRect.left +3;
			
			pCell->bPartial = false;
			
			if (!pCell->csaLines[j].IsEmpty() && txtRect.Height() > CALENDAR_LINE_HEIGHT)
			{
				pDC->SetTextColor(RGB(0,0,0));
				pDC->DrawText(pCell->csaLines[j], txtRect, DT_LEFT|DT_TOP);								
				CBrush br;
				br.CreateSolidBrush(RGB(125,175,255));
				pDC->FillRect(&dotRect, &br);
			}
			else if (!pCell->csaLines[j].IsEmpty())
			{
				CPen dotPen(PS_SOLID, 1, RGB(170,170,170));
				pDC->SelectObject(&dotPen);
				
				// Draw a little arrow
				static int t[2][7] = {5,5,8,8,8,5,5,4,3,2,1,2,3,4};
				int n = 0;
				
				for(int r=7; r>0; r--)
				{
					pDC->MoveTo(rCell.right-9+r, rCell.bottom-t[0][n]);
					pDC->LineTo(rCell.right-9+r, rCell.bottom-t[1][n]);
					n++;
				}
				
				pCell->bPartial = true;
				break;
			}
		}
	}
}

bool CCalendarCtrl::GetGridCellRect(int nRow, int nCol, CRect& rect, BOOL bOmitHeader) const
{
	if (nRow >=0 && nRow<GetVisibleWeeks() && 
		nCol>=0 && nCol<CALENDAR_NUM_COLUMNS)
	{
		CRect rc;
		GetClientRect(&rc);

		int nHeight = (rc.Height()-m_nHeaderHeight)/GetVisibleWeeks();
		int nWidth = rc.Width()/CALENDAR_NUM_COLUMNS;

		if ((nHeight <= 0) || (bOmitHeader && (nHeight <= m_nDayHeaderHeight)))
			return false;

		rect.left = nWidth*nCol;
		rect.right = rect.left + nWidth;
		rect.top = m_nHeaderHeight + nRow*nHeight;
		rect.bottom = rect.top + nHeight;

		// right-most column extends to client extent
		if (nCol == (CALENDAR_NUM_COLUMNS-1))
			rect.right = rc.right;

		if (bOmitHeader)
			rect.top += m_nDayHeaderHeight;

		return true;
	}
	return false;
}

CCalendarCell* CCalendarCtrl::HitTestGridCell(const CPoint& point)
{
	int nRow, nCol;

	if (HitTestGridCell(point, nRow, nCol))
		return GetCell(nRow, nCol);

	return NULL;
}

const CCalendarCell* CCalendarCtrl::HitTestGridCell(const CPoint& point) const
{
	int nRow, nCol;
	
	if (HitTestGridCell(point, nRow, nCol))
		return GetCell(nRow, nCol);
	
	return NULL;
}

bool CCalendarCtrl::HitTestGridCell(const CPoint& point, int &nRow, int &nCol) const
{
	if (point.y > m_nHeaderHeight) 
	{
		CRect rc;
		GetClientRect(&rc);

		if (point.x < 0 || point.x > rc.right || point.y < 0 || point.y > rc.bottom)
			return false;

		int nHeight = (rc.Height()-m_nHeaderHeight)/GetVisibleWeeks();
		int nWidth = rc.Width()/CALENDAR_NUM_COLUMNS;

		if (nHeight && nWidth)
		{
			nRow = (point.y-m_nHeaderHeight)/nHeight;
			nCol = point.x/nWidth;
			
			// correct for last row and column
			nRow = min(nRow, GetVisibleWeeks()-1);
			nCol = min(nCol, CALENDAR_NUM_COLUMNS-1);

			return true;
		}
	}
	return false;
}

int CCalendarCtrl::HitTestGridRow(const CPoint& point) const
{
	int nRow, nCol;

	if (HitTestGridCell(point, nRow, nCol))
		return nRow;

	// else
	return -1;
}

int CCalendarCtrl::GetGridCellColumn(const COleDateTime& date) const
{
	// Supports any date visible or not
	int nOffset = ((int)(date.m_dt) - (int)GetMinDate().m_dt);
	int nCol = (nOffset % 7);

	if (nCol < 0)
		nCol += 7;

	return nCol;
}

COleDateTime CCalendarCtrl::GetMinDate() const
{ 
	return GetMinDate(0);
}

COleDateTime CCalendarCtrl::GetMaxDate() const 
{ 
	return GetMaxDate(m_nVisibleWeeks - 1); 
}

COleDateTime CCalendarCtrl::GetMinDate(int nRow) const
{
	if (nRow < 0 || nRow >= m_nVisibleWeeks)
	{
		ASSERT(0);
		return NullDate();
	}

	return m_dayCells[nRow][0].date; 
}

COleDateTime CCalendarCtrl::GetMaxDate(int nRow) const
{
	if (nRow < 0 || nRow >= m_nVisibleWeeks)
	{
		ASSERT(0);
		return NullDate();
	}

	// else
	return m_dayCells[nRow][CALENDAR_NUM_COLUMNS-1].date; 
}

BOOL CCalendarCtrl::IsDateVisible(const COleDateTime& date) const
{
	return ((date >= GetMinDate()) && (date.m_dt < (GetMaxDate().m_dt + 1)));
}

BOOL CCalendarCtrl::SelectDate(const COleDateTime& dtDate, BOOL bAutoScroll)
{
	if (!IsDateVisible(dtDate))
	{
		if (!bAutoScroll)
			return FALSE;

		Goto(dtDate, true);
	}
	else // just select
	{
//		m_DateCurrent = WholeDays(dtDate);
		m_SingleSelection.RemoveAll();
		
		m_SelectionRange[2] = 0;
		m_SelectionRange[1] = DateToSeconds(WholeDays(dtDate));
		m_SelectionRange[0] = m_SelectionRange[1];
		
		// Scrolling pos
// 		COleDateTime today = WholeDays(COleDateTime::GetCurrentTime());
// 		m_nVscrollPos = (m_nVscrollMax/2) + (m_DateCurrent -today).GetDays()/7;
// 		
// 		SetScrollPos(SB_VERT, m_nVscrollPos, TRUE);
// 		Invalidate(FALSE);
	}

	return TRUE;
}

bool CCalendarCtrl::GetLastSelectedGridCell(int &nRow, int &nCol) const
{
	nRow = 0; nCol = 0;

	for(int i=0; i<CALENDAR_MAX_ROWS ; i++)
	{
		for(int u=0; u<CALENDAR_NUM_COLUMNS ; u++)
		{
			if (m_SelectionRange[1] == DateToSeconds(GetCell(i, u)->date))
			{
				nRow = i; 
				nCol = u;
				return true;
			}
		}
	}

	return false;
}

bool CCalendarCtrl::IsGridCellSelected(const CCalendarCell* pCell) const
{
	ASSERT(pCell);
	
	if (!pCell)
		return FALSE;

	time_t tDate = DateToSeconds(pCell->date);
	ASSERT(tDate);

	if (m_SingleSelection.GetCount())
	{
		bool bSel;
		return (m_SingleSelection.Lookup(tDate, bSel) && bSel);
	}

	// else selection range
	return (tDate >= m_SelectionRange[0] && tDate <= m_SelectionRange[1]);
}
	
bool CCalendarCtrl::IsGridCellSelected(int nRow, int nCol) const
{
	const CCalendarCell* pCell = GetCell(nRow, nCol);
	ASSERT(pCell);

	return IsGridCellSelected(pCell);
}

bool CCalendarCtrl::SelectGridCell(int nRow, int nCol)
{ 
	if ((nRow < 0) || (nRow >= m_nVisibleWeeks) || (nCol < 0) || (nCol >= CALENDAR_NUM_COLUMNS))
	{
		ASSERT(0);
		return false;
	}

	return (SelectDate(m_dayCells[nRow][nCol].date, FALSE) != FALSE);
}

void CCalendarCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	int nRow, nCol;

	if (HitTestGridCell(point, nRow, nCol))
	{
		// select if not already
		if (!IsGridCellSelected(nRow, nCol))
		{
			const CCalendarCell* pCell = GetCell(nRow, nCol);
			ASSERT(pCell);

			m_SingleSelection.RemoveAll();

			time_t tDate = DateToSeconds(pCell->date);

			m_SelectionRange[2] = 0;
			m_SelectionRange[0] = tDate;
			m_SelectionRange[1] = m_SelectionRange[0];

			Invalidate(FALSE);
			UpdateWindow();
		}
	}	
	
	SetFocus();
	CWnd::OnRButtonDown(nFlags, point);
}

void CCalendarCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	bool bShift = (GetKeyState(VK_SHIFT)>>8 == -1);
	bool bCtrl = (GetKeyState(VK_CONTROL)>>8 == -1);

	if (bShift || !m_bEnableMultiSel) 
		bCtrl = false; // allow one combination

	int nRow, nCol;

	if (HitTestGridCell(point, nRow, nCol) && !bCtrl)
	{
		if ((m_dayCells[nRow][nCol].date < m_BoundUp) && 
			(m_dayCells[nRow][nCol].date > m_BoundDown))
		{
			m_SingleSelection.RemoveAll();

			m_SelectionRange[2] = 0;
			m_SelectionRange[0] = DateToSeconds(GetCell(nRow, nCol)->date);
			m_SelectionRange[1] = m_SelectionRange[0];

			if (m_bEnableMultiSel)
			{
				m_bSelectionStarted = true;
				SetCapture();
			}

			Invalidate(true);
		}
	}	
	
	SetFocus();
	CWnd::OnLButtonDown(nFlags, point);
}

void CCalendarCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	bool bShift = (GetKeyState(VK_SHIFT)>>8 == -1);
	bool bCtrl = (GetKeyState(VK_CONTROL)>>8 == -1);

	if (bShift || !m_bEnableMultiSel) 
		bCtrl = false; // allow one combination

	int nRow, nCol;

	if (HitTestGridCell(point, nRow, nCol))
	{
		if (bCtrl)
		{
			if ((m_dayCells[nRow][nCol].date < m_BoundUp) && 
				(m_dayCells[nRow][nCol].date > m_BoundDown))	
			{
				if (m_SelectionRange[2])
				{				
					m_SelectionRange[1] = DateToSeconds(GetCell(nRow, nCol)->date);
					m_SelectionRange[0] = m_SelectionRange[1];
					
					bool bSelected;
					
					if (m_SingleSelection.Lookup(m_SelectionRange[1], bSelected))
					{
						m_SingleSelection.RemoveKey(m_SelectionRange[1]);
					}
					else if (m_SingleSelection.GetCount() < m_nMaxSel)
					{
						m_SingleSelection.SetAt(m_SelectionRange[1], true);
					}
					m_SelectionRange[2] = m_SelectionRange[1];
				}
				else
				{
					if (m_SingleSelection.GetCount() < m_nMaxSel)
						m_SingleSelection.SetAt(m_SelectionRange[1], true);
					
					m_SelectionRange[1] = DateToSeconds(GetCell(nRow, nCol)->date);
					m_SelectionRange[0] = m_SelectionRange[1];
					
					if (m_SingleSelection.GetCount() < m_nMaxSel)
						m_SingleSelection.SetAt(m_SelectionRange[1], true);
					
					m_SelectionRange[2] = m_SelectionRange[1];
				}			
			}
		}
		else if (m_bSelectionStarted)
		{
			ASSERT(m_bEnableMultiSel);

			m_SingleSelection.RemoveAll();
			m_bSelectionStarted = false;

			if ((m_dayCells[nRow][nCol].date < m_BoundUp) &&
				(m_dayCells[nRow][nCol].date > m_BoundDown))
			{
				m_SelectionRange[1] = DateToSeconds(GetCell(nRow, nCol)->date);
			}
			else
				m_SelectionRange[1] = m_SelectionRange[0];
			
			AdjustSelection();
		}
		else // single selection
		{
			if ((m_dayCells[nRow][nCol].date < m_BoundUp) && 
				(m_dayCells[nRow][nCol].date > m_BoundDown))
			{
				m_SingleSelection.RemoveAll();

				m_SelectionRange[2] = 0;
				m_SelectionRange[0] = DateToSeconds(GetCell(nRow, nCol)->date);
				m_SelectionRange[1] = m_SelectionRange[0];
			}
		}

		// always release capture
		ReleaseCapture();
		Invalidate(true);

		// notify parent
		NotifyParentClick();
	}
	else // remove selection
	{
		m_SingleSelection.RemoveAll();
		m_SelectionRange[2] = 0;

		Invalidate(true);
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CCalendarCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bSelectionStarted)
	{
		int nRow, nCol;

		if (HitTestGridCell(point, nRow, nCol))
		{
			if ((m_dayCells[nRow][nCol].date < m_BoundUp) && 
				(m_dayCells[nRow][nCol].date > m_BoundDown))	
			{
				m_SingleSelection.RemoveAll();
				m_SelectionRange[1] = DateToSeconds(GetCell(nRow, nCol)->date);

				AdjustSelection();
				Invalidate(true);
			}
		}
	}	
	CWnd::OnMouseMove(nFlags, point);
}

void CCalendarCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int nInc = 0;
    switch (nSBCode)
    {
        case SB_TOP:
        case SB_LINEUP:
        case SB_PAGEUP:     
			nInc = -1;
			break;

		case SB_BOTTOM:
		case SB_LINEDOWN:
        case SB_PAGEDOWN:
			nInc = 1;
			break;

        case SB_THUMBTRACK: 
			nInc = nPos - m_nVscrollPos;
			break;

		case SB_ENDSCROLL:	
        default:
			nInc = 0;
    }

	if (nInc)
	{		
		m_nVscrollPos += nInc;	

		COleDateTime newDate = m_DateCurrent;
		newDate += COleDateTimeSpan(7*nInc, 0, 0, 0); 

		if ((m_BoundUp > newDate) && (newDate > m_BoundDown))
			Goto(newDate);
		else
			m_nVscrollPos -= nInc;

		SetScrollPos(SB_VERT, m_nVscrollPos, TRUE);
	}
	
	SetFocus();
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CCalendarCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{	
	int nInc = ((zDelta < 0) ? 1 : -1);

	if (GetKeyState(VK_CONTROL) & 0x8000)
	{
		SetVisibleWeeks(min(m_nVisibleWeeks + nInc, CALENDAR_MAX_ROWS));
		GetParent()->SendMessage(WM_CALENDAR_VISIBLEWEEKCHANGE);
	}
	else
	{
		int nPos = GetScrollPos(SB_VERT);
			
		if (nInc > 0)
		{
			SendMessage(WM_VSCROLL,SB_LINEDOWN);

			// If we're already at the bottom of the scrollbar, 
			// we have to notify the parent explicitly
			if (nPos == m_nVscrollMax)
			{
				GetParent()->SendMessage(WM_VSCROLL, SB_LINEDOWN, (LPARAM)GetSafeHwnd());
			}
		}
		else if (nInc < 0)
		{
			SendMessage(WM_VSCROLL,SB_LINEUP);

			// If we're already at the top of the scrollbar, 
			// we have to notify the parent explicitly
			if (nPos == 0)
			{
				GetParent()->SendMessage(WM_VSCROLL, SB_LINEUP, (LPARAM)GetSafeHwnd());
			}
		}
	}
	
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

time_t CCalendarCtrl::DateToSeconds(const COleDateTime& date)
{
	// FIX : We need to find the gmt bias for the zone	
	time_t t = (time_t)(date - COleDateTime(1970, 1, 1, 0, 0, 0)).GetTotalSeconds();
	
	COleDateTime diffdate(t);
	int gmtbias = 0;
	
	if (diffdate > date) 
		gmtbias = -((int)(diffdate-date).GetTotalSeconds());
	else 
		gmtbias =  ((int)(date-diffdate).GetTotalSeconds());
	
	// The next check is to handle the case when system is adjusting the clock 
	// and we meet a time change
	// Note: for the missing hour case, bias is left untouched so the date may have a time > 0
	COleDateTime v((time_t)(t/86400*86400 + gmtbias));

	if (v < date) 
		gmtbias += 3600;
	
	return (time_t)(t/86400*86400 + gmtbias);
}

UINT CCalendarCtrl::OnGetDlgCode()
{
	return DLGC_WANTARROWS;
}

void CCalendarCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{		
	int nRow, nCol;
	GetLastSelectedGridCell(nRow, nCol);
	
	if ((m_dayCells[nRow][nCol].date >= m_BoundUp) || 
		(m_dayCells[nRow][nCol].date <= m_BoundDown))
	{
		return;
	}

	COleDateTime newDate = m_DateCurrent;
	
	bool bHold = (GetKeyState(VK_SHIFT)>>8 == -1)?true:false;
	bool bCtrl = (GetKeyState(VK_CONTROL)>>8 == -1)?true:false;
	
	if (bHold) 
		bCtrl = false; // allow one combination
	
	switch(nChar)
	{
	case VK_SPACE:
		if (bCtrl)
		{
			bool bSelected; 
			time_t t = DateToSeconds(GetCell(nRow, nCol)->date);
			
			if (m_SingleSelection.Lookup(t, bSelected))
			{
				m_SingleSelection.RemoveKey(t);
			}
			else if (m_SingleSelection.GetCount() < m_nMaxSel)
			{
				m_SingleSelection.SetAt(t, true);
			}
			
			NotifyParentClick();
			Invalidate(true);
		}
		break;
		
	case VK_UP:
		{
			if (!bCtrl)
			{
				m_SingleSelection.RemoveAll();
				m_SelectionRange[2] = 0;
			}
			else if (m_SingleSelection.GetCount() == 0)
			{
				m_SingleSelection.SetAt(m_SelectionRange[0], true);
			}
			
			if (nRow == 0)
			{
				newDate -= COleDateTimeSpan(7,0,0,0);
				
				if ((m_BoundUp > newDate) && (newDate > m_BoundDown))
				{
					m_nVscrollPos--;
					SetScrollPos(SB_VERT, m_nVscrollPos, TRUE);
					Goto(newDate);
				}
			}
			else
				nRow--;	
			
			if ((m_dayCells[nRow][nCol].date < m_BoundUp) && 
				(m_dayCells[nRow][nCol].date > m_BoundDown))	
			{
				m_SelectionRange[1] = DateToSeconds(GetCell(nRow, nCol)->date);
				
				if (bCtrl)
					m_SelectionRange[2] = m_SelectionRange[1];
				
				if (!bHold)
					m_SelectionRange[0] = m_SelectionRange[1];
				
				AdjustSelection();
				Invalidate(true);
			}
			
			NotifyParentClick();
		}
		break;
		
	case VK_DOWN:
		{
			if (!bCtrl)
			{
				m_SingleSelection.RemoveAll();
				m_SelectionRange[2] = 0;
			}
			else if (m_SingleSelection.GetCount() == 0)
			{
				m_SingleSelection.SetAt(m_SelectionRange[0], true);
			}
			
			if (nRow == GetVisibleWeeks()-1)
			{
				newDate += COleDateTimeSpan(7,0,0,0);
				
				if ((m_BoundUp > newDate) && (newDate > m_BoundDown))
				{
					m_nVscrollPos++;
					SetScrollPos(SB_VERT, m_nVscrollPos, TRUE);
					Goto(newDate);
				}
			}
			else
				nRow++;
			
			if ((m_dayCells[nRow][nCol].date < m_BoundUp) && 
				(m_dayCells[nRow][nCol].date > m_BoundDown))	
			{	
				m_SelectionRange[1] = DateToSeconds(GetCell(nRow, nCol)->date);
				
				if (bCtrl)
					m_SelectionRange[2] = m_SelectionRange[1];
				
				if (!bHold)
					m_SelectionRange[0] = m_SelectionRange[1];
				
				AdjustSelection();
				Invalidate(true);
			}
			
			NotifyParentClick();
		}
		break;
		
	case VK_LEFT:
		{
			if (!bCtrl)
			{
				m_SingleSelection.RemoveAll();
				m_SelectionRange[2] = 0;
			}
			else if (m_SingleSelection.GetCount() == 0)
			{
				m_SingleSelection.SetAt(m_SelectionRange[0], true);
			}
			
			if (nCol > 0) 
			{
				nCol--;
			}
			else
			{
				nCol = CALENDAR_NUM_COLUMNS-1;
				
				if (nRow == 0)
				{
					newDate -= COleDateTimeSpan(7,0,0,0);
					
					if ((m_BoundUp > newDate) && (newDate > m_BoundDown))
					{
						m_nVscrollPos--;
						SetScrollPos(SB_VERT, m_nVscrollPos, TRUE);
						Goto(newDate);
					}
					else
						nCol = 0;
				}
				else
					nRow--;
			}
			
			if ((m_dayCells[nRow][nCol].date < m_BoundUp) && 
				(m_dayCells[nRow][nCol].date > m_BoundDown))	
			{					
				m_SelectionRange[1] = DateToSeconds(GetCell(nRow, nCol)->date);
				
				if (bCtrl)
					m_SelectionRange[2] = m_SelectionRange[1];
				
				if (!bHold)
					m_SelectionRange[0] = m_SelectionRange[1];
				
				AdjustSelection();
				Invalidate(true);
			}
			
			NotifyParentClick();
		}
		break;
		
	case VK_RIGHT:
		{
			if (!bCtrl)
			{
				m_SingleSelection.RemoveAll();
				m_SelectionRange[2] = 0;
			}
			else if (m_SingleSelection.GetCount() == 0)
			{
				m_SingleSelection.SetAt(m_SelectionRange[0], true);
			}
			
			if (nCol < CALENDAR_NUM_COLUMNS-1)	
			{
				nCol++;
			}
			else
			{
				nCol = 0;
				if (nRow == GetVisibleWeeks()-1)
				{
					newDate += COleDateTimeSpan(7,0,0,0);
					
					if ((m_BoundUp > newDate) && (newDate > m_BoundDown))
					{
						m_nVscrollPos++;
						SetScrollPos(SB_VERT, m_nVscrollPos, TRUE);
						Goto(newDate);
					}
					else
						nCol = CALENDAR_NUM_COLUMNS-1;
				}
				else
					nRow++;
			}
			
			if ((m_dayCells[nRow][nCol].date < m_BoundUp) && 
				(m_dayCells[nRow][nCol].date > m_BoundDown))	
			{
				m_SelectionRange[1] = DateToSeconds(GetCell(nRow, nCol)->date);
				
				if (bCtrl)
					m_SelectionRange[2] = m_SelectionRange[1];
				
				if (!bHold)
					m_SelectionRange[0] = m_SelectionRange[1];
				
				AdjustSelection();
				Invalidate(true);
			}
			
			NotifyParentClick();
		}
		break;
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

time_t CCalendarCtrl::GetFirstSelectedItem() const
{
	if (m_SingleSelection.GetCount())
	{
		POSITION pos = m_SingleSelection.GetStartPosition();
		ASSERT(pos);

		bool bSelected;
		time_t date;

		m_SingleSelection.GetNextAssoc(pos, date, bSelected);
		ASSERT(bSelected);

		return date;
	}
	else if (m_SelectionRange[0] && m_SelectionRange[1])
	{
		return min(m_SelectionRange[0], m_SelectionRange[1]);
	}

	// all else
	return 0;
}


int CCalendarCtrl::GetSelectedItems(CDWordArray& dwaSelection) const
{
	if (m_SingleSelection.GetCount())
	{
		POSITION pos = m_SingleSelection.GetStartPosition();
		bool bSelected;
		time_t date;

		while (pos)
		{
			m_SingleSelection.GetNextAssoc(pos, date, bSelected);
			ASSERT(bSelected);

			dwaSelection.Add((DWORD)date);
		}
	}
	else
	{
		if (m_SelectionRange[0] && m_SelectionRange[1])
		{
			COleDateTime dmax(max(m_SelectionRange[0], m_SelectionRange[1]));
			COleDateTime dmin(min(m_SelectionRange[0], m_SelectionRange[1]));			
			COleDateTime dcur = dmin;

			// FIX : Remove added hour if any
			if (dmin.GetHour())	
				dmin -= COleDateTimeSpan(0,1,0,0);

			int nDays = 1;

			while(dcur <= dmax)
			{	
				dwaSelection.Add((DWORD)DateToSeconds(dcur));
				dcur = dmin + COleDateTimeSpan(nDays,0,0,0);				
				dcur = COleDateTime(DateToSeconds(dcur));

				nDays++;
			}
		}
	}

	return dwaSelection.GetSize();
}

void CCalendarCtrl::AdjustSelection()
{
	int nSelCount = 0;

	COleDateTime dmax(max(m_SelectionRange[0], m_SelectionRange[1]));
	COleDateTime dmin(min(m_SelectionRange[0], m_SelectionRange[1]));
	COleDateTime dcur = dmin;

	while(dcur <= dmax)
	{
		nSelCount++;
		dcur += COleDateTimeSpan(1,0,0,0);			
	}

	if (nSelCount > m_nMaxSel)
	{
		if (m_SelectionRange[1] > m_SelectionRange[0])
		{
			COleDateTime dMin((time_t)m_SelectionRange[1]);
			dMin -= COleDateTimeSpan(m_nMaxSel-1,0,0,0);
			m_SelectionRange[0] = DateToSeconds(dMin);
		}
		else if (m_SelectionRange[1] < m_SelectionRange[0])
		{
			COleDateTime dMax((time_t)m_SelectionRange[1]);
			dMax += COleDateTimeSpan(m_nMaxSel-1,0,0,0);
			m_SelectionRange[0] = DateToSeconds(dMax);
		}
	}
}

COLORREF CCalendarCtrl::GetFadedThemeColour(int percent) const
{	
	unsigned char r = GetRValue(m_crTheme), g = GetGValue(m_crTheme), b = GetBValue(m_crTheme);
	int al = percent*75/100;

	return RGB(	(unsigned char)(r + ((255-r)/(float)75) * al), 
				(unsigned char)(g + ((255-g)/(float)75) * al), 
				(unsigned char)(b + ((255-b)/(float)75) * al));
}

void CCalendarCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	int nRow, nCol;

	if (HitTestGridCell(point, nRow, nCol))
	{
		CCalendarCell* pCell = GetCell(nRow, nCol);
		ASSERT(pCell);

		if (pCell)
			NotifyParentDblClick();
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}

void CCalendarCtrl::NotifyParentClick()
{
	NMHDR	NotifyArea;

	NotifyArea.code = NM_CLICK;
	NotifyArea.hwndFrom = m_hWnd;
	NotifyArea.idFrom = ::GetDlgCtrlID(m_hWnd);

	GetParent()->SendMessage(WM_NOTIFY, ::GetDlgCtrlID(m_hWnd), (WPARAM)&NotifyArea);
}

void CCalendarCtrl::NotifyParentDblClick()
{
	NMHDR	NotifyArea;

	NotifyArea.code = NM_DBLCLK;
	NotifyArea.hwndFrom = m_hWnd;
	NotifyArea.idFrom = ::GetDlgCtrlID(m_hWnd);

	GetParent()->SendMessage(WM_NOTIFY, ::GetDlgCtrlID(m_hWnd), (WPARAM)&NotifyArea);
}

