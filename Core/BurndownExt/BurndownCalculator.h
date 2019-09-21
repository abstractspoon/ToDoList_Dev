#if !defined(AFX_BURNDOWNCALCULATOR_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_)
#define AFX_BURNDOWNCALCULATOR_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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

	double GetStartDate() const { return m_dStartExtents; }
	double GetEndDate() const { return m_dEndExtents; }
	
	// Totals
	double GetDaysEstimated() const;
	double GetDaysSpent() const;
	double GetCostEstimate() const;
	double GetCostSpent() const;

	// Proportions
	double GetDaysSpent(const COleDateTime& date) const;
	double GetDaysEstimated(const COleDateTime& date) const;
	double GetCostSpent(const COleDateTime& date) const;
	double GetCostEstimated(const COleDateTime& date) const;

	int GetTotalDays() const;
	int GetTotalWeekdays() const;

	int GetIncompleteTaskCount(const COleDateTime& date, int nItemFrom, int& nNextItemFrom) const;
	BOOL GetStartedEndedTasks(const COleDateTime& date, int &nNumStarted, int &nNumDone) const;
	BOOL GetDaysEstimatedSpent(const COleDateTime& date, double &dEstDays, double &dSpentDays) const;
	BOOL GetCostEstimatedSpent(const COleDateTime& date, double &dEstCost, double &dSpentCost) const;

protected:
	const CStatsItemArray& m_data;

	double m_dStartExtents, m_dEndExtents;

	mutable int m_nTotalDays, m_nTotalWeekdays;

protected:
	double CalcProportionOfValue(const STATSITEM& si, double dValue, const COleDateTime& date) const;
	double GetIntersectionProportion(const STATSITEM& si, BOOL bWeekdays) const;

	enum ATTRIB { DAYS, COST };
	enum ATTRIBTYPE { ESTIMATE, SPENT };

	double GetTotalAttribValue(ATTRIB nAttrib, ATTRIBTYPE nType) const;
	double GetTotalAttribValue(ATTRIB nAttrib, ATTRIBTYPE nType, const COleDateTime& date) const;

	double GetAttribValue(const STATSITEM& si, ATTRIB nAttrib, ATTRIBTYPE nType) const;
	double GetAttribValue(const STATSITEM& si, ATTRIB nAttrib, ATTRIBTYPE nType, const COleDateTime& date) const;

	static double GetTimeInDays(double dTime, TDC_UNITS nUnits);
	static TH_UNITS MapUnitsToTHUnits(TDC_UNITS nUnits);
};

#endif // !defined(AFX_BURNDOWNCALCULATOR_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_)
