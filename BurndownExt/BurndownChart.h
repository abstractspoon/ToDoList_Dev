#pragma once

#include "..\3rdParty\HMXChart.h"

/////////////////////////////////////////////////////////////////////////////
// CBurndownChart

class CBurndownChart : public CHMXChart
{
	DECLARE_DYNAMIC(CBurndownChart)

public:
	CBurndownChart();
	virtual ~CBurndownChart();

	BOOL SaveToImage(CBitmap& bmImage);

protected:
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////



