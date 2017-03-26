// BurndownChart.cpp : implementation file
//

#include "stdafx.h"
#include "BurndownExt.h"
#include "BurndownChart.h"


// CBurndownChart
CBurndownChart::CBurndownChart()
{

}

CBurndownChart::~CBurndownChart()
{
}


BEGIN_MESSAGE_MAP(CBurndownChart, CHMXChart)
END_MESSAGE_MAP()



// CBurndownChart message handlers

BOOL CBurndownChart::SaveToImage(CBitmap& bmImage)
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
		}
	}

	return (bmImage.GetSafeHandle() != NULL);
}
