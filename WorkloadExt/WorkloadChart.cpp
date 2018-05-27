// BurndownChart.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "WorkloadChart.h"
#include "WorkloadStruct.h"

#include "..\Shared\EnString.h"

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
