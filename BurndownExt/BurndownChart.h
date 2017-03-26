#pragma once

#include "..\3rdParty\HMXChart.h"

/////////////////////////////////////////////////////////////////////////////
// CBurndownChart

class CBurndownChart : public CHMXChart
{
public:
	CBurndownChart();
	virtual ~CBurndownChart();

	BOOL SaveToImage(CBitmap& bmImage);

protected:
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////



