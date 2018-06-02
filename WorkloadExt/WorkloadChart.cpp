// BurndownChart.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "WorkloadChart.h"
#include "WorkloadStruct.h"

#include "..\Shared\EnString.h"

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
	m_aAllocTo(aAllocTo)
{

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
	SetDatasetSizeFactor(0, 3);
	SetDatasetLineColor(0, COLOR_ORANGE);

	SetYText(CEnString(IDS_PERCENTLOADPERPERSON));
	SetDrawGridOnTop(false);

	OnEditData();

	return 0;
}

void CWorkloadChart::OnEditData()
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
