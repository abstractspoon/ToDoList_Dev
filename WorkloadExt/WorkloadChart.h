#pragma once

#include "..\3rdParty\HMXChart.h"

/////////////////////////////////////////////////////////////////////////////

class CMapAllocations;

/////////////////////////////////////////////////////////////////////////////
// CWorkloadChart

class CWorkloadChart : public CHMXChart
{
public:
	CWorkloadChart(const CStringArray& aAllocTo, const CMapAllocations& mapPercentLoad);
	virtual ~CWorkloadChart();

	BOOL SaveToImage(CBitmap& bmImage);

	void OnEditData();

protected:
	const CMapAllocations& m_mapPercentLoad;
	const CStringArray& m_aAllocTo;
	
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////



