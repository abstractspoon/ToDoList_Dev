#pragma once

#include "..\3rdParty\HMXChart.h"

/////////////////////////////////////////////////////////////////////////////

class CWorkloadItemMap;

/////////////////////////////////////////////////////////////////////////////
// CWorkloadChart

class CWorkloadChart : public CHMXChart
{
public:
	CWorkloadChart(const CWorkloadItemMap& data);
	virtual ~CWorkloadChart();

	BOOL SaveToImage(CBitmap& bmImage);

protected:
	const CWorkloadItemMap& m_data;
	
protected:
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////



