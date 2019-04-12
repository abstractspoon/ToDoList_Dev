// ThreadedTaskFileSave.h: interface for the CThreadedTaskFileSave class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADEDTASKFILESAVE_H__ABB4341E_B854_4DBB_8B6C_20BDD5F4D286__INCLUDED_)
#define AFX_THREADEDTASKFILESAVE_H__ABB4341E_B854_4DBB_8B6C_20BDD5F4D286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "taskfile.h"

#include <afxmt.h>

class CThreadedTaskFileSave  
{
public:
	static void SaveTaskFile(const CTaskFile& tasks, LPCTSTR szFilePath);
	virtual ~CThreadedTaskFileSave();

protected:
	CThreadedTaskFileSave();

	static CThreadedTaskFileSave& Instance()
	{
		static CThreadedTaskFileSave ttfs;
		return ttfs;
	}

	void WaitOnExistingThread(LPCTSTR szFilePath, BOOL bTakeLock = TRUE);
	void RemoveThread(LPCTSTR szFilePath, BOOL bTakeLock = TRUE);
	void AddThread(const CTaskFile& tasks, LPCTSTR szFilePath, BOOL bTakeLock = TRUE);
	CWinThread* GetThread(LPCTSTR szFilePath, BOOL bTakeLock = TRUE);

	void TakeLock() { m_cs.Lock(); }
	void ReleaseLock() { m_cs.Unlock(); }

protected:

	struct TTFCDATA
	{
		TTFCDATA(CThreadedTaskFileSave& ttfs, const CTaskFile& tasks, LPCTSTR szFilePath) 
			: m_ttfs(ttfs), m_tasks(tasks), sFilePath(szFilePath)
		{
		}

		virtual ~TTFCDATA()
		{
		}

		CThreadedTaskFileSave& m_ttfs;
		CTaskFile m_tasks;
		CString sFilePath;
	};

	CMapStringToPtr m_mapThreads;
	CCriticalSection m_cs;

protected:
	static UINT ThreadProc(LPVOID pData);

};

#endif // !defined(AFX_THREADEDTASKFILESAVE_H__ABB4341E_B854_4DBB_8B6C_20BDD5F4D286__INCLUDED_)
