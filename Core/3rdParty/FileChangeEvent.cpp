// FileChangeEvent.cpp: implementation of the CFileChangeEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileChangeEvent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FILECHANGEEVENTINFO::FILECHANGEEVENTINFO(const CString& sFilePath)
{
	TCHAR szPath[_MAX_PATH];
	TCHAR szDrive[_MAX_DRIVE];
	_tsplitpath(sFilePath, szDrive, szPath, NULL, NULL);

	m_sName = sFilePath;
	if (_tcslen(szDrive) == 0)
	{
		GetCurrentDirectory(_MAX_PATH, szPath);
		m_sPath = szPath;
	}
	else
	{
		m_sPath = szDrive;
		m_sPath += szPath;
	}

	HANDLE f = CreateFile(m_sName, 0, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
						  NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (f)
	{
		FILETIME FileModifyTime;
		GetFileTime(f, NULL, NULL, &FileModifyTime);
		m_dtFileDate = FileModifyTime;
		CloseHandle(f);
	}
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileChangeEvent::CFileChangeEvent(HWND hwndNotify)
{
	m_bEvent = false;
	m_hwndNotify = hwndNotify;
}

CFileChangeEvent::~CFileChangeEvent()
{
	if (m_bEvent)
		StopWatch();

	POSITION pos = m_FileInfoMap.GetStartPosition();

	while (pos)
	{
		CString sPath;
		FILECHANGEEVENTINFO* f = NULL;

		m_FileInfoMap.GetNextAssoc(pos, sPath, f);
		delete f;
	}
// 	for (FileInfoMap::iterator it = m_FileInfoMap.begin(); it != m_FileInfoMap.end(); it++)
// 		delete it->second;

	m_FileInfoMap.RemoveAll();

	int nPath = m_Paths.GetSize();

	while (nPath--)
		delete m_Paths[nPath];

// 	for (FilePathVector::iterator it2 = m_Paths.begin(); it2 != m_Paths.end(); it2++)
// 		delete *it2;

	m_Paths.RemoveAll();
}

void CFileChangeEvent::StartWatch(void)
{
	if (!m_bEvent)
	{
		m_evStopWatch.ResetEvent();
		m_bEvent = (AfxBeginThread(ThreadProc, (LPVOID)this) != NULL);
	}
}

void CFileChangeEvent::StopWatch(void)
{
	if (m_bEvent)
	{
		m_bEvent = false;
		m_evStopWatch.SetEvent();
	}
}

void CFileChangeEvent::AddFile(const CString& sFileName)
{
	FILECHANGEEVENTINFO* f = NULL;

	if (m_FileInfoMap.Lookup(sFileName, f))
		return;

// 	if (m_FileInfoMap.find(sFileName) != m_FileInfoMap.end())
// 		return;

	f = new FILECHANGEEVENTINFO(sFileName);
	m_FileInfoMap[sFileName] = f;

	int nPath = m_Paths.GetSize();

	while (nPath--)
	{
		if (m_Paths[nPath]->GetName() == f->GetPath())
		{
			m_Paths[nPath]->IncrementCount();
			return;
		}
	}

	// else
	m_Paths.Add(new CFilePath(f->GetPath()));

/*
	for (FilePathVector::iterator it = m_Paths.begin(); it != m_Paths.end(); it++)
	{
		if ((*it)->getName() == f->GetPath())
		{
			(**it)++;
			return;
		}
	}
	m_Paths.push_back(new CFilePath(f->GetPath()));
*/

	if (m_bEvent)
	{
		StopWatch();
		StartWatch();
	}
}

void CFileChangeEvent::RemoveFile(const CString& sFileName)
{
	FILECHANGEEVENTINFO f(sFileName);

	int nPath = m_Paths.GetSize();

	while (nPath--)
	{
		if (m_Paths[nPath]->GetName() == f.GetPath())
		{
			m_Paths[nPath]->DecrementCount();

			if (m_Paths[nPath]->GetCount() == 0)
			{
				delete m_Paths[nPath];
				m_Paths.RemoveAt(nPath);
			}

			return;
		}
	}

/*
	for (FilePathVector::iterator it = m_Paths.begin(); it != m_Paths.end(); it++)
	{
		if ((*it)->getName() == f.GetPath())
		{
			(**it)--;
			if ((*it)->getCount() == 0)
			{
				delete *it;
				m_Paths.erase(it);
			}
			return;
		}
	}
*/
}

UINT CFileChangeEvent::ThreadProc(LPVOID lpParam)
{
	CFileChangeEvent *pFileChangeEvent = (CFileChangeEvent *)lpParam;

	HANDLE *hChangeHandles = new HANDLE[pFileChangeEvent->m_Paths.GetSize() + 1];

	while (1)	//endless loop
	{
		hChangeHandles[0] = pFileChangeEvent->m_evStopWatch;

		for (int i = 0; i < pFileChangeEvent->m_Paths.GetSize(); i++)
		{
			hChangeHandles[i + 1] = FindFirstChangeNotification(pFileChangeEvent->m_Paths[i]->GetName(),
																FALSE,
																FILE_NOTIFY_CHANGE_LAST_WRITE
																| FILE_NOTIFY_CHANGE_SIZE
																| FILE_NOTIFY_CHANGE_FILE_NAME);
		}
		DWORD dwResult = WaitForMultipleObjects(pFileChangeEvent->m_Paths.GetSize() + 1, hChangeHandles, FALSE, INFINITE);

		if (dwResult == WAIT_OBJECT_0)	// Watch stopped 
		{
			break;
		}

		int nPathIdx = dwResult - WAIT_OBJECT_0 - 1;

		// Search for changed files
		POSITION pos = pFileChangeEvent->m_FileInfoMap.GetStartPosition();

		while (pos)
		{
			CString sPath;
			FILECHANGEEVENTINFO* f = NULL;

			pFileChangeEvent->m_FileInfoMap.GetNextAssoc(pos, sPath, f);

// 		for (CMapFileInfoMap::iterator it = pFileChangeEvent->m_FileInfoMap.begin(); it != pFileChangeEvent->m_FileInfoMap.end(); it++)
// 		{
// 			if (it->second->getPath() == pFileChangeEvent->m_Paths[nPathIdx]->GetName())

			if (f->GetPath() == pFileChangeEvent->m_Paths[nPathIdx]->GetName())
			{
				HANDLE file = CreateFile(f->GetName(),//it->second->getName(),
									  0,
									  FILE_SHARE_DELETE
									  | FILE_SHARE_READ
									  | FILE_SHARE_WRITE,
									  NULL,
									  OPEN_EXISTING,
									  FILE_ATTRIBUTE_NORMAL,
									  NULL);
				if (f)
				{
					FILETIME dtFileDate;
					FILETIME dtCreateDate;

					GetFileTime(file, &dtCreateDate, NULL, &dtFileDate);
					CloseHandle(file);

					if (f->GetDate().GetStatus() != COleDateTime::valid)
					{
						f->SetDate(dtCreateDate);
						pFileChangeEvent->SetChangeNotification(FA_CREATED, f->GetName());
					}
					else
					{
						COleDateTime dtDate = dtFileDate;
						if (f->GetDate() < dtDate)
						{
							f->SetDate(dtDate);
							pFileChangeEvent->SetChangeNotification(FA_CHANGED, f->GetName());
						}
					}
				}
				else
				{
					pFileChangeEvent->SetChangeNotification(FA_REMOVED, f->GetName());
					pFileChangeEvent->RemoveFile(f->GetName());
				}
			}
		}
	}

	delete[] hChangeHandles;
	return 0;
}

void CFileChangeEvent::SetChangeNotification(FileAlarm nAlarm, const CString& sFileName)
{
	if (m_hwndNotify)
	{
		// Take a copy because we are posting message
		static CString sFileNameEx;
		sFileNameEx = sFileName;

		PostMessage(m_hwndNotify, WM_FILECHANGEEVENT, nAlarm, (LPARAM)(LPCTSTR)sFileNameEx);
	}
	else
	{
		OnFileAlarm(nAlarm, sFileName);
	}
}
