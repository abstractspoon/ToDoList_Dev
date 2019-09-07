// HMXChartEx.cpp : implementation file
//

#include "stdafx.h"
#include "HMXChartEx.h"
#include "GraphicsMisc.h"
#include "Mapex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const CString EMPTY_STR;

const int HILITEBOXSIZE = GraphicsMisc::ScaleByDPIFactor(3);

/////////////////////////////////////////////////////////////////////////////

bool HMXUtils::GetMinMax(const CHMXDataset datasets[], int nNumSets, double& nMin, double& nMax, bool bDataOnly)
{
	bool first = true;

	for (int i = 0; i < nNumSets; i++)
	{
		double dMinSet, dMaxSet;

		if (datasets[i].GetMinMax(dMinSet, dMaxSet, bDataOnly))
		{
			if (first)
			{
				nMin = dMinSet;
				nMax = dMaxSet;

				first = false;
			}
			else
			{
				nMin = min(nMin, dMinSet);
				nMax = min(nMax, dMaxSet);
			}
		}
	}

	return !first;
}

double HMXUtils::CalcMaxYAxisValue(double dDataMax, int nNumTicks)
{
	return (nNumTicks * CalcYAxisInterval(dDataMax, nNumTicks));
}

double HMXUtils::CalcYAxisInterval(double dDataMax, int nNumTicks)
{
	ASSERT(nNumTicks > 0);

	const double INTERVALS[] = { 1, 5, 10, 20, 25, 50, 100, 200, 250, 500, 1000, 2000, 5000 };
	const int NUM_INT = (sizeof(INTERVALS) / sizeof(INTERVALS[0]));

	// Find the first tick increment that gives us a range
	// greater than or equal to dDataMax
	for (int nInc = 0; nInc < NUM_INT; nInc++)
	{
		double dMaxYAxis = (nNumTicks * INTERVALS[nInc]);

		if (dDataMax <= dMaxYAxis)
			return INTERVALS[nInc];
	}

	return 10000;
}

/////////////////////////////////////////////////////////////////////////////

const double MIN_SUBINTERVAL_HEIGHT = GraphicsMisc::ScaleByDPIFactor(5);

/////////////////////////////////////////////////////////////////////////////
// CHMXChartEx

CHMXChartEx::CHMXChartEx() : m_nLastTooltipHit(-1)
{
}

CHMXChartEx::~CHMXChartEx()
{
}


BEGIN_MESSAGE_MAP(CHMXChartEx, CHMXChart)
	//{{AFX_MSG_MAP(CHMXChartEx)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_NOTIFY(TTN_SHOW, 0, OnShowTooltip)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CHMXChartEx message handlers

BOOL CHMXChartEx::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CHMXChart::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID);
}

int CHMXChartEx::GetYSubTicks(double dInterval) const
{
	if (dInterval != (int)dInterval)
	{
		ASSERT(0);
		return 1;
	}
	else if (dInterval == 1.0)
	{
		return 1;
	}

	const int SUB_TICKS[] = { 2, 4, 5 };
	const int NUM_SUBTICKS = (sizeof(SUB_TICKS) / sizeof(SUB_TICKS[0]));

	int nSubTick = NUM_SUBTICKS;
	int nNumTicks = GetYTicks();

	while (nSubTick--)
	{
		double dSubInterval = (dInterval / SUB_TICKS[nSubTick]);

		if (dSubInterval == (int)dSubInterval)
		{
			double dSubIntervalInPixels = (m_rectData.Height() / (nNumTicks * SUB_TICKS[nSubTick]));

			if (dSubIntervalInPixels >= MIN_SUBINTERVAL_HEIGHT)
				return SUB_TICKS[nSubTick];
		}
	}

	// else
	return 1;
}

void CHMXChartEx::FilterToolTipMessage(MSG* pMsg)
{
	m_tooltip.FilterToolTipMessage(pMsg);
}

BOOL CHMXChartEx::InitTooltip(BOOL bMultiline)
{
	if (!m_tooltip.Create(this))
		return FALSE;

	// else
	m_tooltip.ModifyStyleEx(0, WS_EX_TRANSPARENT);
	m_tooltip.SetDelayTime(TTDT_INITIAL, 0);
	m_tooltip.SetDelayTime(TTDT_AUTOPOP, 10000);
	m_tooltip.SetDelayTime(TTDT_RESHOW, 0);

	if (bMultiline)
		m_tooltip.SetMaxTipWidth(1024); // for '\n' support

	return TRUE;
}

void CHMXChartEx::OnShowTooltip(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	CRect rTooltip;
	m_tooltip.GetWindowRect(rTooltip);

	if (AdjustTooltipRect(rTooltip))
	{
		m_tooltip.MoveWindow(rTooltip);
		*pResult = TRUE; // handled
	}
}

bool CHMXChartEx::DrawHorzGridLines(CDC& dc)
{
	double dInterval = HMXUtils::CalcYAxisInterval(m_nYMax, 10);
	int nNumSubTicks = GetYSubTicks(dInterval);

	if (nNumSubTicks > 1)
	{
		CPen penSubGrid(PS_SOLID, 1, GraphicsMisc::Lighter(GetGridColor(), 0.6));
		CPen* pPenOld = dc.SelectObject(&penSubGrid);

		int nTotalTicks = (GetYTicks() * nNumSubTicks);
		double nY = ((m_nYMax - m_nYMin) / nTotalTicks);

		for (int f = 0; f <= nTotalTicks; f++)
		{
			if (f % nNumSubTicks)
			{
				double nTemp = m_rectData.bottom - (nY*f) * m_rectData.Height() / (m_nYMax - m_nYMin);

				dc.MoveTo(m_rectData.left, (int)nTemp);
				dc.LineTo(m_rectData.right, (int)nTemp);
			}
		}

		dc.SelectObject(pPenOld);
	}

	return CHMXChart::DrawHorzGridLines(dc);
}

BOOL CHMXChartEx::HighlightDataPoints(int nIndex)
{
	// Draw inverted boxes around data point(s)
	CDC* pDC = NULL;

	// Prevent drawing over the same X,Y more than once
	CDWordSet mapPoints;

	for (int nDataset = 0; nDataset < HMX_MAX_DATASET; nDataset++)
	{
		CPoint ptData;

		if (!GetPointXY(nDataset, nIndex, ptData))
			break;

		if (mapPoints.Has(MAKELONG(ptData.x, ptData.y)))
			continue;

		mapPoints.Add(MAKELONG(ptData.x, ptData.y));

		if (pDC == NULL)
		{
			pDC = GetDC();
			pDC->SetROP2(R2_NOT);
		}

		CRect rData(ptData, CSize(1, 1));
		rData.InflateRect(HILITEBOXSIZE, HILITEBOXSIZE);

		pDC->MoveTo(rData.left, rData.bottom);
		pDC->LineTo(rData.left, rData.top);
		pDC->LineTo(rData.right, rData.top);
		pDC->LineTo(rData.right, rData.bottom);
		pDC->LineTo(rData.left, rData.bottom);
	}

	if (pDC)
		ReleaseDC(pDC);

	return (pDC != NULL);
}

int CHMXChartEx::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	const_cast<CHMXChartEx*>(this)->HideLastHighlightedPoint(); // always
	int nHit = HitTest(point);

	if (nHit != -1)
	{
		CString sTooltip = GetTooltip(nHit);

		if (!sTooltip.IsEmpty())
		{
			if (const_cast<CHMXChartEx*>(this)->HighlightDataPoints(nHit))
				m_nLastTooltipHit = nHit;
			else
				ASSERT(0);

			return CToolTipCtrlEx::SetToolInfo(*pTI, this, sTooltip, MAKELONG(point.x, point.y), m_rectData);
		}
	}

	// else
	return CHMXChart::OnToolHitTest(point, pTI);
}

CString CHMXChartEx::GetTooltip(int nHit) const
{
	return EMPTY_STR;
}

BOOL CHMXChartEx::AdjustTooltipRect(CRect& rScreen)
{
	ASSERT(m_nLastTooltipHit != -1);

	// Offset to the right and place close to the relevant data point
	CPoint ptAveData;

	if (GetAveragePointXY(m_nLastTooltipHit, ptAveData))
	{
		ClientToScreen(&ptAveData);
		rScreen.OffsetRect(16, ptAveData.y - rScreen.CenterPoint().y);

		return TRUE;
	}

	// else
	return FALSE;

}

BOOL CHMXChartEx::GetAveragePointXY(int nIndex, CPoint& point) const
{
	int nNumSets = 0;
	CPoint ptTotal(0, 0);

	for (int nDataset = 0; nDataset < HMX_MAX_DATASET; nDataset++)
	{
		CPoint ptTemp;

		if (GetPointXY(nDataset, nIndex, ptTemp))
		{
			ptTotal += ptTemp;
			nNumSets++;
		}
	}

	if (nNumSets == 0)
		return FALSE;

	point.x = (ptTotal.x / nNumSets);
	point.y = (ptTotal.y / nNumSets);

	return TRUE;
}

int CHMXChartEx::HitTest(const CPoint& ptClient) const
{
	if (!m_rectData.Width())
		return -1;

	if (!m_rectData.PtInRect(ptClient))
		return -1;

	int nNumData = m_datasets[0].GetDatasetSize();
	int nXOffset = (ptClient.x - m_rectData.left);

	return ((nXOffset * nNumData) / m_rectData.Width());
}

void CHMXChartEx::OnMouseMove(UINT nFlags, CPoint point)
{
	// Tooltip is handled in OnToolHitTest but we still need
	// to check for when the mouse leaves the graph area
	if (!m_rectData.PtInRect(point))
		HideLastHighlightedPoint();
}

void CHMXChartEx::OnMouseLeave()
{
	HideLastHighlightedPoint();
}

void CHMXChartEx::DoPaint(CDC& dc, BOOL bPaintBkgnd)
{
	m_nLastTooltipHit = -1;

	CHMXChart::DoPaint(dc, bPaintBkgnd);
}

void CHMXChartEx::HideLastHighlightedPoint()
{
	if (m_nLastTooltipHit != -1)
	{
		HighlightDataPoints(m_nLastTooltipHit);
		m_nLastTooltipHit = -1;
	}
}
