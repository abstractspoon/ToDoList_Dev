// HMXChartEx.cpp : implementation file
//

#include "stdafx.h"
#include "HMXChartEx.h"
#include "GraphicsMisc.h"
#include "DialogHelper.h"
#include "Mapex.h"
#include "Themed.h"
#include "OSVersion.h"

#include <math.h>

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

#define RAD2DEG(d) ((d) * 180 / 3.141592654f)

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

	const double INTERVALS[] = { 1, 2, 5, 10, 20, 25, 50, 100, 200, 250, 500, 1000, 2000, 5000 };
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

CHMXChartEx::CHMXChartEx() : m_nLastTooltipHit(-1), m_bFixedLabelFontSize(FALSE)
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

int CHMXChartEx::GetNumYSubTicks(double dInterval) const
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
	int nNumTicks = GetNumYTicks();

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
	int nNumSubTicks = GetNumYSubTicks(dInterval);

	if (nNumSubTicks > 1)
	{
		CPen penSubGrid(PS_SOLID, 1, GraphicsMisc::Lighter(GetGridColor(), 0.6));
		CPen* pPenOld = dc.SelectObject(&penSubGrid);

		int nTotalTicks = (GetNumYTicks() * nNumSubTicks);
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
	CHMXChartEx* pThis = const_cast<CHMXChartEx*>(this);
	int nHit = HitTest(point);

	if (nHit != -1)
	{
		CString sTooltip = GetTooltip(nHit);

		if (!sTooltip.IsEmpty())
		{
			if (nHit != m_nLastTooltipHit)
			{
				pThis->HideLastHighlightedPoint();

				if (pThis->HighlightDataPoint(nHit))
					m_nLastTooltipHit = nHit;
			}

			return CToolTipCtrlEx::SetToolInfo(*pTI, this, sTooltip, (nHit + 1), m_rectData);
		}
	}

	pThis->HideLastHighlightedPoint();

	return CHMXChart::OnToolHitTest(point, pTI);
}

CString CHMXChartEx::GetTooltip(int nHit) const
{
	return EMPTY_STR;
}

int CHMXChartEx::HitTest(const CPoint& ptClient, int nDataset) const
{
	if (!m_rectData.Width())
		return -1;

	HMX_DATASET_STYLE nStyle;
	VERIFY(GetDatasetStyle(nDataset, nStyle));

	switch (nStyle)
	{
	case HMX_DATASET_STYLE_DONUT:
	case HMX_DATASET_STYLE_DONUTLINE:
	case HMX_DATASET_STYLE_PIE:
	case HMX_DATASET_STYLE_PIELINE:
		{
			CArray<PIESEGMENT, PIESEGMENT&> aSegments;
			int nNumSeg = CalcPieSegments(m_datasets[nDataset], aSegments);

			if (nNumSeg == 0)
				return -1;
			
			CRect rPie, rDonut;
			VERIFY(CalcPieRects(rPie, rDonut) > 0);

			if (!rPie.PtInRect(ptClient))
				return false;

			// Calculate the distance from the pie centre to the cursor
			CPoint ptDiff = (ptClient - rPie.CenterPoint());
			double dist = sqrt((double)(ptDiff.x * ptDiff.x) + (ptDiff.y * ptDiff.y));

			if (dist > (rPie.Width() / 2))
				return -1;

			if (nStyle == HMX_DATASET_STYLE_DONUT || nStyle == HMX_DATASET_STYLE_DONUTLINE)
			{
				if (dist < (rPie.Width() / 4))
					return -1;
			}

			// Determine which segment the cursor falls in
			float fAngle = NormaliseAngle(RAD2DEG(atan2f((float)ptDiff.y, (float)ptDiff.x)));

			for (int nSeg = 0; nSeg < nNumSeg; nSeg++)
			{
				const PIESEGMENT& seg = aSegments[nSeg];
				
				if (seg.fSweepDegrees > 0.0f)
				{
					float fFrom = seg.fStartDegrees;
					float fTo = NormaliseAngle(seg.fStartDegrees + seg.fSweepDegrees);

					if (fFrom == fTo)
					{
						// segment is full 0-360
						return nSeg;
					}
					else if (fTo < fFrom) // Segment straddles the 0-360 mark
					{
						if (((fAngle >= fFrom) && (fAngle < 360.f)) ||
							 ((fAngle >= 0.0f) && (fAngle < fTo)))
						{
							return nSeg;
						}
					}
					else
					{
						if ((fAngle >= fFrom) && (fAngle < fTo))
							return nSeg;
					}
				}
			}
			
			ASSERT(0);
		}
		break;

	default:
		{
			if (!m_rectData.PtInRect(ptClient))
				return -1;

			int nNumData = m_datasets[nDataset].GetDatasetSize();

			if (nNumData == 0)
				return -1;

			int nXOffset = (ptClient.x - m_rectData.left);

			return ((nXOffset * nNumData) / m_rectData.Width());
		}
		break;
	}

	ASSERT(0);
	return -1;
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
	// Because the overlapping tooltips can cause drawing artifacts 
	// below Windows 8 we selectively disable them:
	if (COSVersion() < OSV_WIN8)
	{
		// 1. Windows XP
		if (COSVersion() < OSV_VISTA)
			return TRUE;

		// 2. Classic theme (Windows 7 and Vista)
		if (!CThemed::AreControlsThemed())
			return TRUE;

		// 3. Disabled Desktop Composition (Windows 7 and Vista)
		if (!GraphicsMisc::DwmIsCompositionEnabled())
			return TRUE;
	}
	
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

void CHMXChartEx::EnableFixedLabelFontSize(BOOL bFixed)
{
	if (bFixed != m_bFixedLabelFontSize)
	{
		m_bFixedLabelFontSize = bFixed;

		if (GetSafeHwnd())
		{
			CalcDatas();
			Invalidate();
		}
	}
}

int CHMXChartEx::CalcYScaleFontSize(BOOL bTitle) const
{
	if (m_bFixedLabelFontSize)
		return (bTitle ? (m_nFontPixelSize + 8) : m_nFontPixelSize);

	// else
	return CHMXChart::CalcYScaleFontSize(bTitle);
}

int CHMXChartEx::CalcXScaleFontSize(BOOL bTitle) const
{
	if (m_bFixedLabelFontSize)
		return CalcYScaleFontSize(bTitle);

	// else
	return CHMXChart::CalcXScaleFontSize(bTitle);
}

CString CHMXChartEx::GetYTickText(int nTick, double dValue) const
{
	// Because we don't scale the label font we may need to drop-out
	// labels when they don't fit but only in a controlled way
	const int SPACING[] = { 10, 5, 2, 1 };
	const int NUM_SPACING = (sizeof(SPACING) / sizeof(SPACING[0]));

	int iSpacing = NUM_SPACING;
	int nSingleTickHeight = (m_rectData.Height() / GetNumYTicks());

	while (iSpacing--)
	{
		if ((nSingleTickHeight * SPACING[iSpacing]) > m_nFontPixelSize)
			break;
	}

	// Omit zero if there is still not enough space
	if (iSpacing < 0)
	{
		if (dValue == 0)
			return _T("");

		iSpacing = 0; // widest spacing
	}

	if ((nTick % SPACING[iSpacing]) != 0)
		return _T("");

	// else
	return CHMXChart::GetYTickText(nTick, dValue);
}

