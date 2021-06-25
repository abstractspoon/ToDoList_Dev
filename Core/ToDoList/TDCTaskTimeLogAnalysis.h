// TaskTimeLog.h: interface for the CTDCTaskTimeLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKTIMELOGANALYSIS_H__6C9F21CD_509E_4890_9B28_F8C6E52FF54B__INCLUDED_)
#define AFX_TASKTIMELOGANALYSIS_H__6C9F21CD_509E_4890_9B28_F8C6E52FF54B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable: 4786)

#include "tdcstruct.h"
#include "TDCTaskTimeLog.h"

/////////////////////////////////////////////////////////////////////////////

class CTDCTaskTimeLogAnalysis
{
public:
	CTDCTaskTimeLogAnalysis(const CString& sTaskList, const CTDCCustomAttribDefinitionArray& aCustomAttribDefs);
	~CTDCTaskTimeLogAnalysis();

	BOOL AnalyseTaskLog(const COleDateTime& dtFrom, 
						const COleDateTime& dtTo,
						TDCTTL_BREAKDOWN nBreakdown, 
						TDC_ATTRIBUTE nGroupBy,
						TDCTTL_FORMAT nFormat,
						LPCTSTR szOutputFile);

	int GetLogFilePaths(CStringArray& aFilePaths) const;

protected:
	CString m_sTaskFile;
	CString m_sCsvDelim;
	TDC_ATTRIBUTE m_nGroupBy;

	CTaskTimeLogItemArray m_aLogItems;
	const CTDCCustomAttribDefinitionArray& m_aCustomAttribDefs;

	CMap<DWORD, DWORD, int, int&> m_mapIDtoRefLogItem;
	CMap<DWORD, DWORD, CString, LPCTSTR> m_mapIDtoGroupBy;

protected:
	typedef CMap<DWORD, DWORD, double, double&> CMapIDToTime;
	
	// ------------------------------------------------------------------------------

public:
	class CMapIDToTimeAndPeriod : public CMapIDToTime
	{
	public:
		CMapIDToTimeAndPeriod(double dPeriod = 0.0);

		BOOL IsValid() const;

	public:
		COleDateTime dtPeriod;
	};

	class CMapIDToTimeAndPeriodArray : public CArray<CMapIDToTimeAndPeriod*, CMapIDToTimeAndPeriod*&>
	{
	public:
		~CMapIDToTimeAndPeriodArray();

		CMapIDToTimeAndPeriod* Get(double dPeriod);
		const CMapIDToTimeAndPeriod* Get(double dPeriod) const;

		int Find(double dPeriod) const;
		void Sort();

	protected:
		static int SortProc(const void* ppPeriod1, const void* ppPeriod2);
	};

	// ------------------------------------------------------------------------------

protected:
	int BuildLogItemArray();
	int BuildGroupByMapping();
	int BuildSortedIDList(const CMapIDToTime& mapDates, CDWordArray& aIDs) const;

	BOOL AnalyseByTask(const COleDateTime& dtFrom, 
						const COleDateTime& dtTo,
						CMapIDToTime& mapIDs) const;

	BOOL AnalyseByDate(const COleDateTime& dtFrom, 
						const COleDateTime& dtTo,
						CMapIDToTimeAndPeriodArray& aPeriods) const;

	BOOL BreakdownDateAnalysis(CMapIDToTimeAndPeriodArray& aPeriods, TDCTTL_BREAKDOWN nBreakdown) const;

	BOOL OutputAnalysis(const CMapIDToTime& mapIDs,
						TDCTTL_FORMAT nFormat,
						LPCTSTR szOutputFile) const;

	BOOL OutputAnalysis(const CMapIDToTimeAndPeriodArray& aPeriods,
						TDCTTL_BREAKDOWN nBreakdown,
						TDCTTL_FORMAT nFormat,
						LPCTSTR szOutputFile) const;

	BOOL WantGroupBy() const;
	CString GetTaskGroupBy(DWORD dwTaskID) const;
	CString GetGroupByHeader() const;
	CString BuildCsvHeader(BOOL bBreakdownByPeriod) const;
	CString FormatCsvRow(DWORD dwTaskID, const CString& sTaskTitle, double dTime, 
						 const CString& sPath, const CString& sPeriod, const CString& sGroupBy) const;

	const TASKTIMELOGITEM& GetReferenceLogItem(DWORD dwTaskID) const;
	const TASKTIMELOGITEM& GetLogItem(int nIndex) const;

	static void Merge(const CMapIDToTime& mapFrom, CMapIDToTime& mapTo); 
	static BOOL GetMinMaxDates(const CMapIDToTimeAndPeriodArray& aPeriods, double& dStart, double& dEnd);
	static BOOL IsSamePeriod(const COleDateTime& date1, const COleDateTime& date2, TDCTTL_BREAKDOWN nBreakdown);
	static void AppendLogItemToMap(const TASKTIMELOGITEM& li, CMapIDToTime& mapIDs);
	static CString FormatPeriod(double dDay, TDCTTL_BREAKDOWN nBreakdown);
	static int BuildGroupByMapping(const CTaskFile& tasks, HTASKITEM hTask, TDC_ATTRIBUTE nGroupBy, const CString& sCustGroupByAttrib, CMap<DWORD, DWORD, CString, LPCTSTR>& mapGroupBy);
};


/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_TASKTIMELOGANALYSIS_H__6C9F21CD_509E_4890_9B28_F8C6E52FF54B__INCLUDED_)
