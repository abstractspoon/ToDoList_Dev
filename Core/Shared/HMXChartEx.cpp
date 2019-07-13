// HMXChartEx.cpp : implementation file
//

#include "stdafx.h"
#include "HMXChartEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

double CHMXChartEx::CalcMaxYAxisValue(double dDataMax) const
{
	return (GetYTicks() * CalcYAxisInterval(dDataMax));
}

double CHMXChartEx::CalcYAxisInterval(double dDataMax) const
{
	const double INTERVALS[] = { 1, 5, 10, 20, 25, 50, 100, 200, 250, 500, 1000, 2000, 5000 };
	const int NUM_INT = (sizeof(INTERVALS) / sizeof(INTERVALS[0]));

	int nNumTicks = GetYTicks();

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