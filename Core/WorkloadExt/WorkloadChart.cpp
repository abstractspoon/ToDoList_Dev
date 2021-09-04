// BurndownChart.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "WorkloadChart.h"
#include "WorkloadStruct.h"

#include "..\Shared\EnString.h"
#include "..\Shared\GraphicsMisc.h"

#include "..\3rdParty\GdiPlus.h"

/////////////////////////////////////////////////////////////////////////////

const COLORREF COLOR_YELLOW = RGB(204, 164,   0); 
const COLORREF COLOR_BLUE	= RGB(0,     0, 244); 
const COLORREF COLOR_PINK	= RGB(234,  28,  74); 
const COLORREF COLOR_ORANGE	= RGB(255,  91,  21); 

/////////////////////////////////////////////////////////////////////////////

// CWorkloadChart
CWorkloadChart::CWorkloadChart(const CStringArray& aAllocTo, const CMapAllocationTotals& mapPercentLoad) 
	: 
	m_mapPercentLoad(mapPercentLoad), 
	m_aAllocTo(aAllocTo),
	m_crOverload(CLR_NONE),
	m_dOverloadValue(100.0),
	m_crUnderload(CLR_NONE),
	m_dUnderloadValue(0.0)
{
	SetDatasetLineColor(0, COLOR_BLUE);
	EnableFixedLabelFontSize(); // don't scale down as available size decreases
}

CWorkloadChart::~CWorkloadChart()
{
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CWorkloadChart, CHMXChartEx)
	ON_WM_CREATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

// CWorkloadChart message handlers

BOOL CWorkloadChart::SaveToImage(CBitmap& bmImage)
{
	CClientDC dc(this);
	CDC dcImage;

	if (dcImage.CreateCompatibleDC(&dc))
	{
		CRect rClient;
		GetClientRect(rClient);

		if (bmImage.CreateCompatibleBitmap(&dc, rClient.Width(), rClient.Height()))
		{
			CBitmap* pOldImage = dcImage.SelectObject(&bmImage);

			SendMessage(WM_PRINTCLIENT, (WPARAM)dcImage.GetSafeHdc(), PRF_ERASEBKGND);
			Invalidate(TRUE);

			dcImage.SelectObject(pOldImage);
		}
	}

	return (bmImage.GetSafeHandle() != NULL);
}

int CWorkloadChart::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CHMXChartEx::OnCreate(lpCreateStruct) != 0)
		return -1;

	ModifyStyle(0, WS_BORDER);

	// Once only
	SetDatasetStyle(0, HMX_DATASET_STYLE_VBAR);
	SetDatasetMin(0, 0.0);
	SetDatasetMax(0, 100.0);
	SetDatasetSizeFactor(0, 5); // width factor for vertical bar

	SetYText(CEnString(IDS_PERCENTLOADPERPERSON));

	RebuildChart();
	VERIFY(InitTooltip(FALSE));

	return 0;
}

void CWorkloadChart::RebuildChart()
{
	// build the graph
	ClearData(0);

	for (int nAllocTo = 0; nAllocTo < m_aAllocTo.GetSize(); nAllocTo++)
	{
		AddData(0, m_mapPercentLoad.Get(m_aAllocTo[nAllocTo]));

		if (m_aAllocTo[nAllocTo].IsEmpty())
			SetXScaleLabel(nAllocTo, CEnString(IDS_UNALLOCATEDTO));
		else
			SetXScaleLabel(nAllocTo, m_aAllocTo[nAllocTo]);
	}

	// Set the maximum Y value to be something 'nice'
	double dMin, dMax;

	if (GetMinMax(dMin, dMax, true))
	{
		int nNumTicks = 10; // minimum default

		if (dMax > 100)
			nNumTicks = (int)((dMax / 10) + 1);

		SetNumYTicks(nNumTicks);
		SetDatasetMax(0, (nNumTicks * 10));
	}

	CalcDatas();
	Invalidate(FALSE);
}

void CWorkloadChart::EnableOverload(BOOL bEnable, double dOverloadValue, COLORREF crOverload)
{
	if (bEnable)
	{
		m_dOverloadValue = dOverloadValue;
		m_crOverload = crOverload;
	}
	else
	{
		m_dOverloadValue = 0.0;
		m_crOverload = CLR_NONE;
	}

	if (GetSafeHwnd())
		Invalidate();
}

void CWorkloadChart::EnableUnderload(BOOL bEnable, double dUnderloadValue, COLORREF crUnderload)
{
	if (bEnable)
	{
		m_dUnderloadValue = dUnderloadValue;
		m_crUnderload = crUnderload;
	}
	else
	{
		m_dUnderloadValue = 0.0;
		m_crUnderload = CLR_NONE;
	}

	if (GetSafeHwnd())
		Invalidate();
}

BOOL CWorkloadChart::SetNormalColor(COLORREF crNormal)
{
	if (crNormal == CLR_NONE)
	{
		ASSERT(0);
		return FALSE;
	}

	return (SetDatasetLineColor(0, crNormal) ? TRUE : FALSE);
}

COLORREF CWorkloadChart::GetLineColor(int nDatasetIndex, double dValue) const
{
	COLORREF crLine = GetValueColor(dValue);

	if (crLine == CLR_NONE)
		crLine = CHMXChartEx::GetLineColor(nDatasetIndex, dValue);

	return crLine;
}

COLORREF CWorkloadChart::GetFillColor(int /*nDatasetIndex*/, double dValue) const
{
	COLORREF crFill = GetValueColor(dValue);

	if (crFill != CLR_NONE)
		crFill = GraphicsMisc::Lighter(crFill, 0.25, FALSE);

	return crFill;
}

COLORREF CWorkloadChart::GetLineColor(double dValue) const
{
	return GetLineColor(0, dValue);
}

COLORREF CWorkloadChart::GetFillColor(double dValue) const
{
	return GetFillColor(0, dValue);
}

bool CWorkloadChart::DrawGrid( CDC& dc)
{
	if (!CHMXChartEx::DrawGrid(dc))
		return false;

	if ((HasOverload() || HasUnderload()) && HasData())
	{
		// Draw Over/underload cutoffs
		CGdiPlusGraphics graphics(dc);

		if (HasOverload())
		{
			CRect rOverload(m_rectData);
			rOverload.bottom -= (int)(rOverload.Height() * (m_dOverloadValue / m_nYMax));

			if (rOverload.bottom <= m_rectData.bottom)
			{
				CGdiPlusBrush brush(m_crOverload, 50);
				CGdiPlus::FillRect(graphics, brush, rOverload);
			}
		}

		if (HasUnderload())
		{
			CRect rUnderload(m_rectData);
			rUnderload.top = (rUnderload.bottom - (int)(rUnderload.Height() * (m_dUnderloadValue / m_nYMax)));

			if (rUnderload.top >= m_rectData.top)
			{
				CGdiPlusBrush brush(m_crUnderload, 50);
				CGdiPlus::FillRect(graphics, brush, rUnderload);
			}
		}
	}

	return true;
}

COLORREF CWorkloadChart::GetBkgndColor(double dValue) const
{
	if (IsOverloaded(dValue) || IsUnderloaded(dValue))
		return GraphicsMisc::Lighter(GetValueColor(dValue), 0.8, TRUE);

	// else
	return CLR_NONE;
}

COLORREF CWorkloadChart::GetValueColor(double dValue) const
{
	if (IsOverloaded(dValue))
		return m_crOverload;

	if (IsUnderloaded(dValue))
		return m_crUnderload;

	// else
	return GetNormalColor();
}

COLORREF CWorkloadChart::GetNormalColor() const
{
	COLORREF crNormal;

	if (!GetDatasetLineColor(0, crNormal))
		crNormal = 0;

	return crNormal;
}

BOOL CWorkloadChart::HasOverload() const
{
	return ((m_crOverload != CLR_NONE) && (m_dOverloadValue > 0.0));
}

BOOL CWorkloadChart::HasUnderload() const
{
	return ((m_crUnderload != CLR_NONE) && (m_dUnderloadValue > 0.0));
}

BOOL CWorkloadChart::IsOverloaded(double dValue) const
{
	return (HasOverload() && (dValue >= m_dOverloadValue));
}

BOOL CWorkloadChart::IsUnderloaded(double dValue) const
{
	return (HasUnderload() && (dValue <= m_dUnderloadValue));
}

CString CWorkloadChart::GetTooltip(int nHit) const
{
	ASSERT(nHit != -1);

	CString sAllocTo = m_aAllocTo[nHit];
	double dPercent = m_mapPercentLoad.Get(sAllocTo);

	if (sAllocTo.IsEmpty())
		sAllocTo = CEnString(IDS_UNALLOCATEDTO);

	CString sTooltip;
	sTooltip.Format(_T("%s: %.2f%%"), sAllocTo, dPercent);

	return sTooltip;
}

int CWorkloadChart::HitTest(const CPoint& ptClient) const
{
	if (!m_aAllocTo.GetSize())
		return -1;

	return CHMXChartEx::HitTest(ptClient);
}
