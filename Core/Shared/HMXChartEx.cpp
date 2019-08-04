// HMXChartEx.cpp : implementation file
//

#include "stdafx.h"
#include "HMXChartEx.h"
#include "GraphicsMisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const double MIN_SUBINTERVAL_HEIGHT = GraphicsMisc::ScaleByDPIFactor(5);

/////////////////////////////////////////////////////////////////////////////
// CHMXChartEx

CHMXChartEx::CHMXChartEx()
{
}

CHMXChartEx::~CHMXChartEx()
{
}


BEGIN_MESSAGE_MAP(CHMXChartEx, CHMXChart)
	//{{AFX_MSG_MAP(CHMXChartEx)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CHMXChartEx message handlers

BOOL CHMXChartEx::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CHMXChart::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID);
}

double CHMXChartEx::CalcMaxYAxisValue(double dDataMax, int nNumTicks)
{
	return (nNumTicks * CalcYAxisInterval(dDataMax, nNumTicks));
}

double CHMXChartEx::CalcYAxisInterval(double dDataMax, int nNumTicks)
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

bool CHMXChartEx::DrawHorzGridLines(CDC& dc)
{
	double dInterval = CalcYAxisInterval(m_nYMax, 10);
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

