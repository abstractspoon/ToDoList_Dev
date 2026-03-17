// HMXChartEx.cpp : implementation file
//

#include "stdafx.h"
#include "HMXChartEx.h"
#include "GraphicsMisc.h"
#include "DialogHelper.h"
#include "Mapex.h"
#include "Themed.h"
#include "Misc.h"

#include "..\3rdParty\OSVersion.h"

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

double HMXUtils::CalcMaxYAxisValue(double dDataMax, int nNumTicks)
{
	return (nNumTicks * CalcYAxisInterval(dDataMax, nNumTicks));
}

double HMXUtils::CalcYAxisInterval(double dDataMax, int nNumTicks)
{
	ASSERT(nNumTicks > 0);

	// Calculate the initial power of 10
	int nPower = (int)log10(dDataMax / nNumTicks);
	double dMultiplier = pow(10, nPower);

	// Find the first interval that gives us a range
	// greater than or equal to dDataMax
	const double INTERVAL[] = { 0.1, 0.2, 0.25, 0.5, 1.0, 2.0, 2.5, 5.0 };
	const int NUM_INT = (sizeof(INTERVAL) / sizeof(INTERVAL[0]));

	for (int nInt = 0; nInt < NUM_INT; nInt++)
	{
		double dInterval = (INTERVAL[nInt] * dMultiplier);
		double dMaxYAxis = (nNumTicks * dInterval);

		if (dDataMax <= dMaxYAxis)
			return dInterval;
	}

	return pow(10, nPower + 1);
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
	const int SUB_TICKS[] = { 2, 4, 5 };
	const int NUM_SUBTICKS = (sizeof(SUB_TICKS) / sizeof(SUB_TICKS[0]));

	int nSubTick = NUM_SUBTICKS;
	int nNumTicks = GetNumYTicks();
	int nAvailHeight = m_rectData.Height();

	while (nSubTick--)
	{
		double dSubIntervalInPixels = (nAvailHeight / (nNumTicks * SUB_TICKS[nSubTick]));

		// Must be an exact pixel height
		if (dSubIntervalInPixels == (int)dSubIntervalInPixels)
		{
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
	m_tooltip.SetDelayTime(TTDT_RESHOW, 0);
	m_tooltip.EnableTracking(TRUE, 16, 16);

	if (bMultiline)
		m_tooltip.EnableMultilineTips();

	return TRUE;
}

BOOL CHMXChartEx::DrawHorzGridLines(CDC& dc)
{
	double dMin, dMax;

	if (!GetMinMax(dMin, dMax, FALSE))
		return FALSE;

	double dRange = (dMax - dMin);
	double dInterval = HMXUtils::CalcYAxisInterval(dRange, 10);

	int nNumSubTicks = GetNumYSubTicks(dInterval);

	if (nNumSubTicks > 1)
	{
		CPen penSubGrid(PS_SOLID, 1, GraphicsMisc::Lighter(GetGridColor(), 0.6));
		CPen* pPenOld = dc.SelectObject(&penSubGrid);

		int nTotalTicks = (GetNumYTicks() * nNumSubTicks);
		double nY = (dRange / nTotalTicks);

		for (int f = 0; f <= nTotalTicks; f++)
		{
			if (f % nNumSubTicks)
			{
				int nVPos = Misc::Round(m_rectData.bottom - (((nY*f) * m_rectData.Height()) / dRange));

				dc.MoveTo(m_rectData.left, nVPos);
				dc.LineTo(m_rectData.right, nVPos);
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

			// Quick exit
			if (!rPie.PtInRect(ptClient))
				return -1;

			// Distance from the pie centre to the cursor
			CPoint ptDiff = (ptClient - rPie.CenterPoint());
			
			int nDistSquared = ((ptDiff.x * ptDiff.x) + (ptDiff.y * ptDiff.y));
			int nOuterRadius = (rPie.Width() / 2);

			if (nDistSquared > (nOuterRadius * nOuterRadius))
				return -1; // outside the pie chart

			if (nStyle == HMX_DATASET_STYLE_DONUT || 
				nStyle == HMX_DATASET_STYLE_DONUTLINE)
			{
				int nInnerRadius = (nOuterRadius / 2);

				if (nDistSquared < (nInnerRadius * nInnerRadius))
					return -1; // in the donut hole
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
