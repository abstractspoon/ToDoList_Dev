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
	
	static int LoadLogItems(const CString& sLogPath, CTaskTimeLogItemArray& aLogItems, BOOL bAppend, CString& sDelim);

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
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_TASKTIMELOG_H__6C9F21CD_509E_4890_9B28_F8C6E52FF54B__INCLUDED_)
