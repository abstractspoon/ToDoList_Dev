// ThreadedTaskFileSave.cpp: implementation of the CThreadedTaskFileSave class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDCThreadedTaskFileSave.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CThreadedTaskFileSave::CThreadedTaskFileSave()
{

}

CThreadedTaskFileSave::~CThreadedTaskFileSave()
{
	// wait until all threads have completed
	CWinThread* pUnused = NULL;
	CString sFilePath;

	while (m_mapThreads.GetCount())
	{
		TakeLock();

		POSITION pos = m_mapThreads.GetStartPosition();

		while (pos)
		{
			m_mapThreads.GetNextAssoc(pos, sFilePath, (void*&)pUnused);
			RemoveThread(sFilePath, FALSE);
		}

		ReleaseLock();
	}
}

void CThreadedTaskFileSave::SaveTaskFile(const CTaskFile& tasks, LPCTSTR szFilePath)
{
	Instance().AddThread(tasks, szFilePath);
}

void CThreadedTaskFileSave::AddThread(const CTaskFile& tasks, LPCTSTR szFilePath, BOOL bTakeLock)
{
	WaitOnExistingThread(szFilePath, bTakeLock);

	// create a new thread
	CWinThread* pThread = AfxBeginThread(ThreadProc, 
							(LPVOID)(new TTFCDATA(*this, tasks, szFilePath)), 
							THREAD_PRIORITY_NORMAL, 
							0, 
							CREATE_SUSPENDED);
	if (pThread)
	{
		// don't auto-delete because we have to _know_ that the thread object can always be accessed
		pThread->m_bAutoDelete = FALSE;

		if (bTakeLock)
			TakeLock();

		m_mapThreads[szFilePath] = pThread;

		if (bTakeLock)
			ReleaseLock();

		pThread->ResumeThread();
	}
}

UINT CThreadedTaskFileSave::ThreadProc(LPVOID pData)
{
	TTFCDATA* pTTFCData = (TTFCDATA*)pData;

	if (pTTFCData)
	{
		pTTFCData->m_tasks.Save(pTTFCData->sFilePath);
		delete pTTFCData; // we're done

		// remove from map
	}

	return 0;
}

void CThreadedTaskFileSave::WaitOnExistingThread(LPCTSTR szFilePath, BOOL bTakeLock)
{
	if (bTakeLock)
		TakeLock();
	
	CWinThread* pThread = GetThread(szFilePath, FALSE);

	if (pThread)
	{
		// and if it's running wait until it finishes
		::WaitForSingleObject(pThread->m_hThread, INFINITE);
	}

	if (bTakeLock)
		ReleaseLock();
}

void CThreadedTaskFileSave::RemoveThread(LPCTSTR szFilePath, BOOL bTakeLock)
{
	if (bTakeLock)
		TakeLock();
	
	WaitOnExistingThread(szFilePath, FALSE);

	delete GetThread(szFilePath, FALSE);
	m_mapThreads.RemoveKey(szFilePath);

	if (bTakeLock)
		ReleaseLock();
}

CWinThread* CThreadedTaskFileSave::GetThread(LPCTSTR szFilePath, BOOL bTakeLock)
{
	if (bTakeLock)
		TakeLock();
	
	CWinThread* pThread = NULL;
	m_mapThreads.Lookup(szFilePath, (void*&)pThread);

	if (bTakeLock)
		ReleaseLock();

	return pThread;
}
