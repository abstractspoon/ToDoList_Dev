#if !defined(AFX_BURNDOWNCALCULATOR_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_)
#define AFX_BURNDOWNCALCULATOR_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BurndownStruct.h"

#include "..\Shared\TimeHelper.h"

#include "..\Interfaces\ITaskList.h"

/////////////////////////////////////////////////////////////////////////////

class CStatsItemArray;
class COleDateTimeRange;

struct STATSITEM;

/////////////////////////////////////////////////////////////////////////////

class CStatsItemCalculator
{
public:
	CStatsItemCalculator(const CStatsItemArray& data);
	virtual ~CStatsItemCalculator();

	BOOL SetDateRange(const COleDateTimeRange& dtExtents);
	BOOL SetShowEmptyFrequencyValues(BOOL bShowEmpty);

	double GetStartDate() const { return m_dStartExtents; }
	double GetEndDate() const { return m_dEndExtents; }

	int GetTotalDays() const;
	int GetTotalWeekdays() const;

	// Time Series ---------------------------------------------------
	
	double GetDaysEstimated() const;
	double GetDaysEstimated(const COleDateTime& date) const;

	double GetDaysSpent() const;
	double GetDaysSpent(const COleDateTime& date) const;

	double GetCostEstimate() const;
	double GetCostEstimated(const COleDateTime& date) const;

	double GetCostSpent() const;
	double GetCostSpent(const COleDateTime& date) const;

	int GetIncompleteTaskCount(const COleDateTime& date, int nItemFrom, int& nNextItemFrom) const;
	BOOL GetStartedEndedTasks(const COleDateTime& date, int &nNumStarted, int &nNumDone) const;
	BOOL GetDaysEstimatedSpent(const COleDateTime& date, double &dEstDays, double &dSpentDays) const;
	BOOL GetCostEstimatedSpent(const COleDateTime& date, double &dEstCost, double &dSpentCost) const;

	int HitTest(const COleDateTime& date) const;

	// Frequency Distributions ---------------------------------------

	int GetCategoryFrequencies(CArray<FREQUENCYITEM, FREQUENCYITEM&>& aFrequencies) const;
	int GetStatusFrequencies(CArray<FREQUENCYITEM, FREQUENCYITEM&>& aFrequencies) const;
	int GetAllocatedToFrequencies(CArray<FREQUENCYITEM, FREQUENCYITEM&>& aFrequencies) const;
	int GetAllocatedByFrequencies(CArray<FREQUENCYITEM, FREQUENCYITEM&>& aFrequencies) const;
	int GetPriorityFrequencies(CArray<FREQUENCYITEM, FREQUENCYITEM&>& aFrequencies) const;
	int GetRiskFrequencies(CArray<FREQUENCYITEM, FREQUENCYITEM&>& aFrequencies) const;
	int GetTagFrequencies(CArray<FREQUENCYITEM, FREQUENCYITEM&>& aFrequencies) const;
	int GetVersionFrequencies(CArray<FREQUENCYITEM, FREQUENCYITEM&>& aFrequencies) const;

protected:
	const CStatsItemArray& m_data;

	double m_dStartExtents, m_dEndExtents;
	BOOL m_bShowEmptyFrequencyValues;

	mutable int m_nTotalWeekdays;

protected:
	double CalcProportionOfValue(const STATSITEM& si, double dValue, const COleDateTime& date) const;
	double GetIntersectionProportion(const STATSITEM& si, BOOL bWeekdays) const;

	enum TIMESERIES_ATTRIB		{ DAYS, COST };
	enum TIMESERIES_ATTRIBTYPE	{ ESTIMATE, SPENT };

	double GetTotalAttribValue(TIMESERIES_ATTRIB nAttrib, TIMESERIES_ATTRIBTYPE nType) const;
	double GetTotalAttribValue(TIMESERIES_ATTRIB nAttrib, TIMESERIES_ATTRIBTYPE nType, const COleDateTime& date) const;

	double GetAttribValue(const STATSITEM& si, TIMESERIES_ATTRIB nAttrib, TIMESERIES_ATTRIBTYPE nType) const;
	double GetAttribValue(const STATSITEM& si, TIMESERIES_ATTRIB nAttrib, TIMESERIES_ATTRIBTYPE nType, const COleDateTime& date) const;

	enum FREQUENCY_ATTRIB		{ CATEGORY, STATUS, ALLOCTO, ALLOCBY, PRIORITY, RISK, TAGS, VERSION };

	int GetAttribFrequencies(FREQUENCY_ATTRIB nAttrib, CMap<CString, LPCTSTR, int, int&>& mapFrequencies) const;
	void AppendFrequencyAttrib(const CString& sAttrib, CMap<CString, LPCTSTR, int, int&>& mapFrequencies) const;
	void AppendFrequencyAttribs(const CStringArray& aAttrib, CMap<CString, LPCTSTR, int, int&>& mapFrequencies) const;

	static double GetTimeInDays(double dTime, TDC_UNITS nUnits);
	static TH_UNITS MapUnitsToTHUnits(TDC_UNITS nUnits);
	
	static int AsSortedArray(const CMap<CString, LPCTSTR, int, int&>& mapFrequencies, CArray<FREQUENCYITEM, FREQUENCYITEM&>& aFrequencies);
	static int CompareFrequencyItems(const void* pV1, const void* pV2);
};

#endif // !defined(AFX_BURNDOWNCALCULATOR_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_)
