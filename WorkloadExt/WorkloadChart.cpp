// BurndownChart.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "WorkloadChart.h"
#include "WorkloadStruct.h"

#include "..\Shared\EnString.h"

#include "..\3rdParty\ColorDef.h"

/////////////////////////////////////////////////////////////////////////////

const COLORREF COLOR_GREEN	= RGB(122, 204,   0); 
const COLORREF COLOR_RED	= RGB(204,   0,   0); 
const COLORREF COLOR_YELLOW = RGB(204, 164,   0); 
const COLORREF COLOR_BLUE	= RGB(0,     0, 244); 
const COLORREF COLOR_PINK	= RGB(234,  28,  74); 
const COLORREF COLOR_ORANGE	= RGB(255,  91,  21); 

/////////////////////////////////////////////////////////////////////////////

// CWorkloadChart
CWorkloadChart::CWorkloadChart(const CStringArray& aAllocTo, const CMapAllocations& mapPercentLoad) 
	: 
	m_mapPercentLoad(mapPercentLoad), 
	m_aAllocTo(aAllocTo),
	m_crOverload(COLOR_RED),
	m_dOverloadValue(80.0),
	m_crUnderload(COLOR_GREEN),
	m_dUnderloadValue(50.0)
{
	SetDatasetLineColor(0, COLOR_BLUE);
}

CWorkloadChart::~CWorkloadChart()
{
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CWorkloadChart, CHMXChart)
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
	if (CHMXChart::OnCreate(lpCreateStruct) != 0)
		return -1;

	ModifyStyle(0, WS_BORDER);

	// Once only
	SetDatasetStyle(0, HMX_DATASET_STYLE_VBAR);
	SetDatasetMin(0, 0.0);
	SetDatasetMax(0, 100.0);
	SetDatasetSizeFactor(0, 5);

	SetYText(CEnString(IDS_PERCENTLOADPERPERSON));
	SetDrawGridOnTop(false);

	OnChangeData();

	return 0;
}

void CWorkloadChart::OnChangeData()
{
	// build the graph
	ClearData(0);

	for (int nAllocTo = 0; nAllocTo < m_aAllocTo.GetSize(); nAllocTo++)
	{
		AddData(0, m_mapPercentLoad.Get(m_aAllocTo[nAllocTo]));
		SetXScaleLabel(nAllocTo, m_aAllocTo[nAllocTo]);
	}

	CalcDatas();
	Invalidate(FALSE);
}

void CWorkloadChart::SetOverloadColor(double dOverloadValue, COLORREF crOverload)
{
	m_dOverloadValue = dOverloadValue;
	m_crOverload = crOverload;
}

void CWorkloadChart::SetUnderloadColor(double dUnderloadValue, COLORREF crUnderload)
{
	m_dUnderloadValue = dUnderloadValue;
	m_crUnderload = crUnderload;
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
	if ((m_crOverload != CLR_NONE) && (dValue >= m_dOverloadValue))
		return m_crOverload;

	if ((m_crUnderload != CLR_NONE) && (dValue <= m_dUnderloadValue))
		return m_crUnderload;

	return CHMXChart::GetLineColor(nDatasetIndex, dValue);
}

COLORREF CWorkloadChart::GetFillColor(int nDatasetIndex, double dValue) const
{
	COLORREF crLine = GetLineColor(nDatasetIndex, dValue);

	return RGBX::AdjustLighting(crLine, 0.25, FALSE);
}

bool CWorkloadChart::DrawDataBkgnd( CDC& dc)
{
	if (!CHMXChart::DrawDataBkgnd(dc))
		return false;

	// Draw Over/underload cutoffs
	if (HasOverload() || HasUnderload())
	{
		// Normal
		COLORREF crFill = CLR_NONE;
		
		if (GetDatasetLineColor(0, crFill))
		{
			crFill = RGBX::AdjustLighting(crFill, 0.5, FALSE);
			dc.FillSolidRect(m_rectData, crFill);
		}
		
		if (HasOverload())
		{
			CRect rOverload(m_rectData);
			rOverload.bottom -= (rOverload.Height() * (m_dOverloadValue / m_nYMax));

			COLORREF crFill = RGBX::AdjustLighting(m_crOverload, 0.5, FALSE);
			dc.FillSolidRect(rOverload, crFill);
		}

		if (HasUnderload())
		{
			CRect rUnderload(m_rectData);
			rUnderload.top = (rUnderload.bottom - (rUnderload.Height() * (m_dUnderloadValue / m_nYMax)));

			COLORREF crFill = RGBX::AdjustLighting(m_crUnderload, 0.5, FALSE);
			dc.FillSolidRect(rUnderload, crFill);
		}
	}

	return true;
}

BOOL CWorkloadChart::HasOverload() const
{
	return ((m_crOverload != CLR_NONE) && (m_dOverloadValue > 0.0));
}

BOOL CWorkloadChart::HasUnderload() const
{
	return ((m_crUnderload != CLR_NONE) && (m_dUnderloadValue > 0.0));
}
