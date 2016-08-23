// FileProcessor.cpp: implementation of the EFileProcessor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eFileProcessor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define	LOCK_OBJECT	CSingleLock	slock((CCriticalSection*)&m_CritSec, TRUE)

EFileProcessor::EFileProcessor()
{
	m_nFileCount = 0;
	m_bContinue = FALSE;
}

EFileProcessor::~EFileProcessor()
{

}

// returns the number of files processed
int EFileProcessor::EProcessFiles(LPCTSTR szSrcFolder, LPCTSTR szSrcExt, BOOL bRecursive, 
					LPCTSTR szDestFolder, LPCTSTR szDestExt)
{
	m_nFileCount = 0;
	m_nTimeTaken = 0;

	{
		LOCK_OBJECT;
		m_bContinue = TRUE;
//		TRACE ("EFileProcessor::EProcessFiles::m_bContinue set to TRUE\n");
	}

	// validate source extension
	CString sSrcExt;
	int nLen = szSrcExt ? strlen(szSrcExt) : 0;

	for (int nChar = 0; nChar < nLen; nChar++)
	{
		char cChar = szSrcExt[nChar];

		switch (cChar)
		{
			case '*':
			case ' ':
				break;

			case '.':
				if (!sSrcExt.IsEmpty() && sSrcExt.Right(1) != ";")
					sSrcExt += ';';
				break;

			default:
				sSrcExt += cChar;
		}
		sSrcExt.MakeUpper();
	}

	// ensure callback for the root folder
	CFileFind ff;

	if (ff.FindFile(szSrcFolder))
	{
		ff.FindNextFile();

		CTime tModified;
		ff.GetLastWriteTime(tModified);

		if (EProcessFolder(szSrcFolder, tModified))
			EDoProcessFiles(szSrcFolder, sSrcExt, bRecursive, szDestFolder, szDestExt);
	}
	else // check if it was a drive or network share by appending "*.*"
	{
		CString sSrcFolder(szSrcFolder);
		sSrcFolder.TrimRight();

		if (sSrcFolder.ReverseFind('\\') != sSrcFolder.GetLength() - 1)
			sSrcFolder += '\\';

		if (ff.FindFile(sSrcFolder + "*.*"))
			EDoProcessFiles(szSrcFolder, sSrcExt, bRecursive, szDestFolder, szDestExt);
	}

	// done
	{
		LOCK_OBJECT;
		m_bContinue = FALSE;
//		TRACE ("EFileProcessor::EProcessFiles::m_bContinue set to TRUE\n");
	}

//	TRACE ("EFileProcessor::EProcessFiles::m_bContinue = %s\n", EContinue() ? "TRUE" : "FALSE");

	return m_nFileCount;
}

void EFileProcessor::EDoProcessFiles(LPCTSTR szSrcFolder, const CString& sSrcExt, BOOL bRecursive, 
					LPCTSTR szDestFolder, LPCTSTR szDestExt)
{
	CString sSearchPath(szSrcFolder);
	sSearchPath.TrimRight();
	sSearchPath.TrimLeft();

	if (sSearchPath.IsEmpty())
		return;

	if (sSearchPath.Right(1) != "\\")
		sSearchPath += "\\";

	sSearchPath += "*.*"; // unfortunately must do it like this to pick up folders

	CFileFind ff;

	BOOL bWorking = ff.FindFile(sSearchPath);

	while (bWorking && EContinue())
	{
		bWorking = ff.FindNextFile();

		if (ff.IsDots())
			continue;
		
		else if (ff.IsDirectory())
		{
			CTime tModified;
			
			ff.GetLastWriteTime(tModified);
			
			if (bRecursive && EProcessFolder(ff.GetFilePath(), tModified))
			{
				// note: really should pass in the corresponding sub folder in destination
				EDoProcessFiles(ff.GetFilePath(), sSrcExt, TRUE, szDestFolder, szDestExt);
			}
			continue;
		}
		
		static char szExt[30];
		
		_splitpath(ff.GetFileName(), NULL, NULL, NULL, szExt);
		
		CString sFileExt(szExt);
		sFileExt.MakeUpper();
		
		if (sSrcExt.IsEmpty() || sSrcExt.Find(sFileExt) != -1)
		{
			m_nFileCount++;
			CString sContents;
			CTime tModified;
			
			ff.GetLastWriteTime(tModified);
			
			DWORD dwTick = GetTickCount();
			BOOL bRes = EProcessFile(ff.GetFilePath(), tModified, ff.GetLength(), sContents);
			m_nTimeTaken += (int)(GetTickCount() - dwTick);
			
			if (bRes && EContinue())
			{
				CString sDestPath(szDestFolder);
				sDestPath.TrimRight();
				sDestPath.TrimLeft();
				
				if (!sDestPath.IsEmpty())
				{
					if (sDestPath.Right(1) != "\\")
						sDestPath += "\\";
					
					sDestPath += ff.GetFileTitle();
					
					CString sDestExt(szDestExt);
					sDestExt.TrimRight();
					sDestExt.TrimLeft();
					
					if (sDestExt.IsEmpty())
						sDestExt = sFileExt;
					
					else if (sDestExt.Left(1) != ".")
						sDestExt = "." + sDestExt;
					
					sDestPath += sDestExt;
					
					EWriteFile(sDestPath, sContents);
				}
			}
		}
	}
}
	
// return false if the contents are to be ignored
// the base class will load the file and fill sContents
BOOL EFileProcessor::EProcessFile(LPCTSTR szFilePath, CTime tLastModified, DWORD dwSize, CString& sContents)
{	
	return EContinue() ? EReadFile(szFilePath, sContents) : FALSE;
}

BOOL EFileProcessor::EReadFile(LPCTSTR szFilePath, CString& sContents, BOOL bPreserveCR)
{
	CStdioFile file;
	CString sLine;

	if (file.Open(szFilePath, CFile::modeRead | CFile::typeText | CFile::shareDenyNone))
	{
		sContents.Empty();

		while (file.ReadString(sLine))
		{
			if (!bPreserveCR)
				sContents.TrimRight();

			sContents += sLine;
		}

		file.Close();
		return TRUE;
	}

	return FALSE;
}

BOOL EFileProcessor::EWriteFile(LPCTSTR szFilePath, const CString& sContents)
{
	CStdioFile file;
	CString sLine;

	if (file.Open(szFilePath, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
	{
		file.WriteString(sContents);
		file.Close();
		return TRUE;
	}

	return FALSE;
}

BOOL EFileProcessor::EContinue()
{
	LOCK_OBJECT;

	return m_bContinue;
}

void EFileProcessor::ETerminate()
{
	LOCK_OBJECT;

	m_bContinue = FALSE;
//	TRACE ("EFileProcessor::ETerminate()::m_bContinue = %s\n", EContinue() ? "TRUE" : "FALSE");
}

