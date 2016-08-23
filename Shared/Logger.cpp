// Logger.cpp: implementation of the CLogger class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Logger.h"
#include "filemisc.h"

#include <direct.h>
#include <afxtempl.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CLogger::s_bInitialized = FALSE;
HANDLE CLogger::s_hAccessible = NULL;

CLogger::CLogger(LPCTSTR szAppVer, int nLevel, int nNumBackups)
{
	ASSERT (!s_hAccessible);
	s_hAccessible = CreateEvent(NULL, FALSE, TRUE, NULL); // auto-reset

	m_nMaxLevel = nLevel;

	m_sLogFolder = FileMisc::GetAppFileName();

	int nFind = m_sLogFolder.ReverseFind('\\');
	m_sLogFolder = m_sLogFolder.Left(nFind) + "\\Logs";

	mkdir(m_sLogFolder);

	StartLogging(szAppVer);
	CullLogs(nNumBackups);
}

CLogger::~CLogger()
{
	EndLogging();

	CloseHandle(s_hAccessible);
}

CLogger& CLogger::GetInstance(LPCTSTR szAppVer, int nLevel, int nNumBackups) 
{ 
	static CLogger logger(szAppVer, nLevel, nNumBackups); 
	return logger; 
}

BOOL CLogger::Initialize(LPCTSTR szAppVer, int nLevel, int nNumBackups)
{
	if (s_bInitialized)
		return FALSE;

	// else
	nNumBackups = max(nNumBackups, 0);
	nNumBackups = min(nNumBackups, 50);

	GetInstance(szAppVer, nLevel, nNumBackups);
	s_bInitialized = TRUE;

	return TRUE;
}

BOOL CLogger::Log(int nLevel, const char *fmt, ...)
{
	if (!s_bInitialized)
		return FALSE;

	ASSERT (nLevel > 0);

	if (nLevel <= 0)
		return FALSE;

	if (nLevel > GetInstance().GetLoggingLevel())
		return TRUE;

	if (WaitForSingleObject(s_hAccessible, 50) == WAIT_OBJECT_0)
	{
		va_list marker;
		va_start(marker, fmt);
		
		static char gBuffer[1024];

		vsprintf(gBuffer, fmt, marker);
		va_end(marker);

		if (gBuffer[0] != 0)
		{
			CString sBuffer;
			sBuffer.Format("%s - ", GetTime());

			// add tabs corresponding to level
			CString sTabs;

			if (nLevel > 1)
			{
				nLevel--;

				while (nLevel--)
					sTabs += '\t';

				sBuffer += sTabs;
			}
			
			sBuffer += gBuffer;

			if (!sTabs.IsEmpty())
			{
				sTabs = "\n\t" + sTabs;
				sBuffer.Replace("\n", sTabs);
			}

			sBuffer += '\n';

			if (!sBuffer.IsEmpty())
				GetInstance().AppendToLog(sBuffer);
		}

		SetEvent(s_hAccessible); // reset
		return TRUE;
	}

	return FALSE;
}

void CLogger::StartLogging(LPCTSTR szAppVer)
{
	CString sText;

	if (szAppVer)
		sText.Format("\n%s *** Logging started (ver %s) ***\n\n", GetTime(), szAppVer);
	else
		sText.Format("\n%s *** Logging started ***\n\n", GetTime());

	AppendToLog(sText);
}

void CLogger::EndLogging()
{
	CString sText;
	sText.Format("\n%s *** Logging ended ***\n", GetTime());
	AppendToLog(sText);
}

void CLogger::AppendToLog(LPCTSTR szText)
{
	if (!szText || szText[0] == 0)
		return;

	// open the file and append this string to the end
	CString sFile;
	sFile.Format("%s\\%s.log", m_sLogFolder, GetDate());
	CStdioFile file;

	if (file.Open(sFile, CFile::modeCreate | CFile::modeNoTruncate | 
						CFile::modeReadWrite | CFile::shareDenyWrite))
	{
		file.SeekToEnd();
		file.WriteString(szText);

		file.Close();
	}
}

BOOL CLogger::EnableLogging(int nLevel)
{
	if (!s_bInitialized)
		return FALSE;

	GetInstance().SetLoggingLevel(nLevel);
	return TRUE;
}

void CLogger::CullLogs(int nNumBackups)
{
	WIN32_FIND_DATA finfo;
	CString sSearchSpec = m_sLogFolder + "\\\?\?-\?\?-\?\?.log";

	HANDLE hFind = FindFirstFile(sSearchSpec, &finfo);
				
	if (hFind != INVALID_HANDLE_VALUE) 
	{
		CStringArray aLogs;
		CMap<CString, LPCTSTR, time_t, time_t&> mapTimes;

		do 
		{
			if (finfo.cFileName[0] != '.') 
			{
				if (!(finfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					CString sFile;
					sFile.Format("%s\\%s", m_sLogFolder, finfo.cFileName);
					
					// insert in date order, oldest last
					time_t tFile = GetLastModified(sFile), tMapItem = 0;
					int nLogs = aLogs.GetSize();

					for (int nLog = 0; nLog < nLogs; nLog++)
					{
						ASSERT (mapTimes.Lookup(aLogs[nLog], tMapItem));
						
						if (tFile > tMapItem)
						{
							aLogs.InsertAt(nLog, sFile);
							break;
						}
					}
					
					if (nLog >= nLogs) // oldest
						aLogs.Add(sFile);
	
					mapTimes[sFile] = tFile;
				}
			}
		}
		while (FindNextFile(hFind, &finfo));
		
		FindClose(hFind);
		
		// cull the oldest
		while (aLogs.GetSize() > nNumBackups)
		{
			::DeleteFile(aLogs[aLogs.GetSize() - 1]);
			aLogs.RemoveAt(aLogs.GetSize() - 1);
		}
	} 

}

CString CLogger::GetDate()
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);

	CString sDate;
	sDate.Format("%02d-%02d-%02d", systime.wMonth, systime.wDay, systime.wYear % 100);

	return sDate;
}

CString CLogger::GetTime()
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);

	CString sDate;
	sDate.Format("%02d:%02d:%02d", systime.wHour, systime.wMinute, systime.wSecond);

	return sDate;
}
