// TaskTimeLog.h: interface for the CTDCTaskTimeLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKTIMELOG_H__6C9F21CD_509E_4890_9B28_F8C6E52FF54B__INCLUDED_)
#define AFX_TASKTIMELOG_H__6C9F21CD_509E_4890_9B28_F8C6E52FF54B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable: 4786)

#include "tdcenum.h"

#include "..\3rdParty\stdiofileex.h"

/////////////////////////////////////////////////////////////////////////////

struct TASKTIMELOGITEM
{
	TASKTIMELOGITEM();
	
	BOOL IsValidToAnalyse() const;
	BOOL IsValidToLog() const;
	void Reset();

	CString FormatRow(int nRowVer, const CString& sDelim) const;
	BOOL ParseRow(const CString& sRow, const CString& sDelim);

	static BOOL GetRowVersion(int nNumFields);
	static CString GetRowFormat(int nRowVer, const CString& sDelim);
	
	DWORD dwTaskID;
	COleDateTime dtFrom, dtTo;
	double dHours;
	CString sTaskTitle;
	CString sComment;
	CString sPerson;
	CString sPath;
	BOOL bTracked;
};
typedef CArray<TASKTIMELOGITEM, TASKTIMELOGITEM&> CTaskTimeLogItemArray;

/////////////////////////////////////////////////////////////////////////////

class CTDCTaskTimeLog  
{
public:
	CTDCTaskTimeLog(LPCTSTR szRefPath, SFE_FORMAT nFormat = SFEF_AUTODETECT);
	virtual ~CTDCTaskTimeLog();

	BOOL LogTime(const TASKTIMELOGITEM& li, BOOL bLogSeparately);
 	BOOL LogTime(DWORD dwTaskID, LPCTSTR szTaskTitle, LPCTSTR szTaskPath, double dHours, const COleDateTime& dtWhen, 
 				const CString& sComment, BOOL bTracked, BOOL bLogSeparately); 

	CString GetLogPath() const;
	CString GetLogPath(DWORD dwTaskID, BOOL bLogSeparately) const;
	const CString& GetDelimiter() const  { return m_sDelim; }
	
	// static helper to reduce copying when building
	// a single list from multiple log files
	static int ReadLogItems(const CString& sLogPath, CTaskTimeLogItemArray& aLogItems, BOOL bAppend, CString& sDelim);

protected:
	CString m_sRefPath;
	SFE_FORMAT m_nFormat;
	int m_nVersion;
	BOOL m_bLogExists;
	CString m_sDelim;

protected: 
	CTDCTaskTimeLog();
	void Initialise(const CString& sLogPath);

	CString GetLatestColumnHeader() const;
	double CalcAccumulatedTime(DWORD dwTaskID, BOOL bLogSeparately); // returns time in hours
};

/////////////////////////////////////////////////////////////////////////////

class CTDCTaskTimeLogAnalysis
{
public:
	CTDCTaskTimeLogAnalysis(const CString& sTaskList, BOOL bLogTaskTimeSeparately);
	~CTDCTaskTimeLogAnalysis();

	BOOL AnalyseTaskLog(const COleDateTime& dtFrom, 
						const COleDateTime& dtTo,
						TDCTTL_BREAKDOWN nBreakdown,
						TDCTTL_FORMAT nFormat,
						LPCTSTR szOutputFile);

	typedef CMap<DWORD, DWORD, double, double&> CMapIDToTime;
	
	class CMapIDToTimeAndPeriod : public CMapIDToTime
	{
	public:
		CMapIDToTimeAndPeriod(double dPeriod = 0.0);

		BOOL IsValid() const;
		
	public:
		COleDateTime dtPeriod;
	};
	
protected:
	CString m_sTasklist;
	BOOL m_bLogTaskTimeSeparately;
	CTaskTimeLogItemArray m_aLogItems;
	CMap<DWORD, DWORD, int, int&> m_mapIDtoRefLogItem;
	CString m_sCsvDelim;

protected:
	int BuildLogItemArray();
	const TASKTIMELOGITEM& GetReferenceLogItem(DWORD dwTaskID) const;
	const TASKTIMELOGITEM& GetLogItem(int nIndex) const;

	BOOL AnalyseByTask(const COleDateTime& dtFrom, 
						const COleDateTime& dtTo,
						CMapIDToTime& mapIDs) const;

	BOOL OutputAnalysis(const CMapIDToTime& mapIDs,
						TDCTTL_FORMAT nFormat,
						LPCTSTR szOutputFile) const;

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

	BOOL AnalyseByDate(const COleDateTime& dtFrom, 
						const COleDateTime& dtTo,
						CMapIDToTimeAndPeriodArray& aPeriods) const;

	BOOL BreakdownDateAnalysis(CMapIDToTimeAndPeriodArray& aPeriods, TDCTTL_BREAKDOWN nBreakdown) const;

	BOOL OutputAnalysis(const CMapIDToTimeAndPeriodArray& aPeriods,
						TDCTTL_BREAKDOWN nBreakdown,
						TDCTTL_FORMAT nFormat,
						LPCTSTR szOutputFile) const;

	CString BuildCsvHeader(BOOL bBreakdownByPeriod) const;
	CString FormatCsvRow(DWORD dwTaskID, const CString& sTaskTitle, double dTime, 
						const CString& sPath, const CString& sPeriod = _T("")) const;

	static void Merge(const CMapIDToTime& mapFrom, CMapIDToTime& mapTo); 
	static BOOL GetMinMaxDates(const CMapIDToTimeAndPeriodArray& aPeriods, double& dStart, double& dEnd);
	static BOOL IsSamePeriod(const COleDateTime& date1, const COleDateTime& date2, TDCTTL_BREAKDOWN nBreakdown);
	static void AppendLogItemToMap(const TASKTIMELOGITEM& li, CMapIDToTime& mapIDs);
	static CString FormatPeriod(double dDay, TDCTTL_BREAKDOWN nBreakdown);
	static int BuildSortedIDList(const CMapIDToTime& mapDates, CDWordArray& aIDs);
};


/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_TASKTIMELOG_H__6C9F21CD_509E_4890_9B28_F8C6E52FF54B__INCLUDED_)
