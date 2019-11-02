// HMXChartEx.cpp : implementation file
//

#include "stdafx.h"
#include "HMXChartEx.h"
#include "GraphicsMisc.h"
#include "DialogHelper.h"
#include "Mapex.h"
#include "Themed.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const CString EMPTY_STR;
const CPoint  NULL_POINT(0, 0);

const int HILITEBOXSIZE = GraphicsMisc::ScaleByDPIFactor(3);
const int TOOLTIPOFFSET = GraphicsMisc::ScaleByDPIFactor(20);

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
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
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
	m_tooltip.SetDelayTime(TTDT_AUTOPOP, 100000);
	m_tooltip.SetDelayTime(TTDT_RESHOW, 0);
	m_tooltip.EnableTracking(TRUE, 16, 16);

	if (bMultiline)
		m_tooltip.SetMaxTipWidth(SHRT_MAX); // for '\n' support

	return TRUE;
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

int CHMXChartEx::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	// Don't remove and redraw the same point if it hasn't changed
	int nHit = HitTest(point);

	if (nHit != -1)
	{
		CString sTooltip = GetTooltip(nHit);

		if (!sTooltip.IsEmpty())
		{
			if (nHit != m_nLastTooltipHit)
			{
				const_cast<CHMXChartEx*>(this)->HideLastHighlightedPoint();

				if (const_cast<CHMXChartEx*>(this)->HighlightDataPoint(nHit))
				{
					m_nLastTooltipHit = nHit;
				}
				else
				{
					ASSERT(0);
				}
			}

			return CToolTipCtrlEx::SetToolInfo(*pTI, this, sTooltip, (nHit + 1), m_rectData);
		}
	}

	const_cast<CHMXChartEx*>(this)->HideLastHighlightedPoint();

	return CHMXChart::OnToolHitTest(point, pTI);
}

CString CHMXChartEx::GetTooltip(int nHit) const
{
	return EMPTY_STR;
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

LRESULT CHMXChartEx::OnMouseLeave(WPARAM /*wp*/, LPARAM /*lp*/)
{
	HideLastHighlightedPoint();

	return 0L;
}

void CHMXChartEx::DoPaint(CDC& dc, BOOL bPaintBkgnd)
{
	m_nLastTooltipHit = -1;

	CHMXChart::DoPaint(dc, bPaintBkgnd);
}

BOOL CHMXChartEx::HighlightDataPoint(int nIndex)
{
	// Disable highlighting under Classic theme because the overlapping
	// tooltips cause drawing artifacts
	if (CThemed::AreControlsThemed())
	{
		CPoint ptData;

		if (!GetPointXY(0, nIndex, ptData))
			return FALSE;

		CDC* pDC = GetDC();

		pDC->SetROP2(R2_NOT);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->SelectStockObject(BLACK_PEN);

		pDC->MoveTo(ptData.x, m_rectData.top);
		pDC->LineTo(ptData.x, m_rectData.bottom);

		ReleaseDC(pDC);
	}
	
	return TRUE;
}

void CHMXChartEx::HideLastHighlightedPoint()
{
	if (m_nLastTooltipHit != -1)
	{
		HighlightDataPoint(m_nLastTooltipHit);
		m_nLastTooltipHit = -1;
	}
}
