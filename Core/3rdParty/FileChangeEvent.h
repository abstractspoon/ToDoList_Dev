// FileChangeEvent.h: interface for the CFileChangeEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILECHANGEEVENT_H__1B890621_FEC8_11D2_B9E0_704B7C000000__INCLUDED_)
#define AFX_FILECHANGEEVENT_H__1B890621_FEC8_11D2_B9E0_704B7C000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//
// Author   : Franky Braem
// Created  : 30/04/1999
// Modified : 17/05/1999 - Use the drive of the path instead of current drive
//			  18/05/1999 - Use the current path if no path specified

// Modified : 10/-2/2023 - Converted to MFC by AbstractSpoon

//////////////////////////////////////////////////////////////////////

#pragma warning(disable:4786)

#include <afxmt.h>
#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

class FILECHANGEEVENTINFO
{
public:
	FILECHANGEEVENTINFO(const CString &sFilePath);

	CString GetName() const { return m_sName; }
	CString GetPath() const { return m_sPath; }
	COleDateTime GetDate() const { return m_dtFileDate; }

	void SetDate(const COleDateTime &dtDate) { m_dtFileDate = dtDate; }

protected:
	CString m_sName;
	CString m_sPath;
	COleDateTime m_dtFileDate;
};

//////////////////////////////////////////////////////////////////////

class CFilePath
{
public:
	CFilePath(const CString &sPath) { m_sName = sPath; m_nCount = 0; }

	inline CString GetName() const { return m_sName; }
	inline int GetCount() const { return m_nCount; }

	inline void IncrementCount() { m_nCount++; }
	inline void DecrementCount() { m_nCount--; }

protected :
	CString m_sName;
	int m_nCount;
};

//////////////////////////////////////////////////////////////////////

typedef CMap<CString, LPCTSTR, FILECHANGEEVENTINFO*, FILECHANGEEVENTINFO*> CMapFileInfo;
typedef CArray<CFilePath*, CFilePath*> CFilePathArray;

//////////////////////////////////////////////////////////////////////

class CFileChangeEvent  
{
public:
	enum FileAlarm { FA_CHANGED, FA_REMOVED, FA_CREATED };

	CFileChangeEvent();
	virtual ~CFileChangeEvent();

	void AddFile(const CString &sFileName);
	void RemoveFile(const CString &sFileName);

	void StartWatch();
	void StopWatch();
	inline bool IsWatching() const { return m_bEvent; }

protected:
	CEvent m_evStopWatch;
	CMapFileInfo m_FileInfoMap;
	CFilePathArray m_Paths;
	bool m_bEvent;

protected:
	virtual void OnFileAlarm(FileAlarm nAlarm, const CString &sFileName) {}	
	
	static UINT ThreadProc(LPVOID lpParam);
};

#endif // !defined(AFX_FILECHANGEEVENT_H__1B890621_FEC8_11D2_B9E0_704B7C000000__INCLUDED_)
