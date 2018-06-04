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
	void SetOverloadColor(double dOverloadValue, COLORREF crOverload);
	void SetUnderloadColor(double dUnderloadValue, COLORREF crUnderload);
	BOOL SetNormalColor(COLORREF crNormal);

	BOOL IsOverloaded(double dValue) const;
	BOOL IsUnderloaded(double dValue) const;

	COLORREF GetLineColor(double dValue) const;
	COLORREF GetFillColor(double dValue) const;
	COLORREF GetBkgndColor(double dValue) const;

	void OnChangeData();

protected:
	const CMapAllocations& m_mapPercentLoad;
	const CStringArray& m_aAllocTo;

	COLORREF m_crOverload, m_crUnderload;
	double m_dOverloadValue, m_dUnderloadValue;
	
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

protected:
	COLORREF GetLineColor(int nDatasetIndex, double dValue) const;
	COLORREF GetFillColor(int nDatasetIndex, double dValue) const;

	COLORREF GetNormalColor() const;
	COLORREF GetValueColor(double dValue) const;
	bool DrawDataBkgnd( CDC& dc);

	BOOL HasOverload() const;
	BOOL HasUnderload() const;

};

/////////////////////////////////////////////////////////////////////////////



