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

	BOOL operator==(const TASKTIMELOGITEM& other) const;

	BOOL IsValidToAnalyse() const;
	BOOL IsValidToLog() const;
	void Clear(BOOL bInitPerson = FALSE);

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
	CString sType;
	COLORREF crAltColor;

protected:
	static CString EncodeValue(const CString& sValue, const CString& sDelim, BOOL bEncodeNewLines = FALSE);
	static CString DecodeValue(const CString& sValue, const CString& sDelim, BOOL bDecodeNewLines = FALSE);
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

	CString GetLogFileFilter(BOOL bLogSeparately) const;
	
	static BOOL LoadLogFile(LPCTSTR szLogPath, CTaskTimeLogItemArray& aLogItems, BOOL bAppend, CString& sHeaderDelim);
	static BOOL SaveLogFile(LPCTSTR szLogPath, const CTaskTimeLogItemArray& aLogItems, BOOL bPreserveVersion = TRUE);

protected:
	CString m_sRefPath;
	SFE_FORMAT m_nFormat;
	int m_nVersion;
	BOOL m_bLogExists;
	BOOL m_bUseTabDelim;
	CString m_sHeaderDelim, m_sColumnHeader;

protected: 
	CTDCTaskTimeLog();

	BOOL Initialise(const CString& sLogPath);
	BOOL Initialise(const CStringArray& aLines);

	CString GetLatestColumnHeader() const;
	CString GetDelimiter(const CString& sLine = _T("")) const;
	int GetNumHeaderRows() const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_TASKTIMELOG_H__6C9F21CD_509E_4890_9B28_F8C6E52FF54B__INCLUDED_)
