// TDCAnonymizeTaskTimeLog.cpp: implementation of the CTDCAnonymizeTaskTimeLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDCAnonymizeTaskTimeLog.h"
#include "todoitem.h"

#include "..\Shared\Misc.h"
#include "..\Shared\fileMisc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCAnonymizeTaskTimeLog::CTDCAnonymizeTaskTimeLog()
{
}

BOOL CTDCAnonymizeTaskTimeLog::Anonymize(LPCTSTR szLogfile)
{
	CString sUnused;
	return Anonymize(szLogfile, sUnused);
}

BOOL CTDCAnonymizeTaskTimeLog::Anonymize(LPCTSTR szLogfile, CString& sAnonFilePath)
{
	CTaskTimeLogItemArray aLogItems;
	CString sDelim;

	if (!LoadLogItems(szLogfile, aLogItems, FALSE, sDelim))
		return FALSE;

	CTDCAnonymizeTaskTimeLog anon;

	if (!anon.AnonymizeLog(aLogItems))
		return FALSE;

	// Save to alternate filename
	sAnonFilePath = szLogfile;
	FileMisc::AddToFileName(sAnonFilePath, _T(".rnd"));
	
	return anon.SaveAnonymisedLogFile(sAnonFilePath, aLogItems, sDelim);
}

BOOL CTDCAnonymizeTaskTimeLog::SaveAnonymisedLogFile(const CString& sLogPath, const CTaskTimeLogItemArray& aLogItems, const CString sDelim)
{
	const CString ENDL(_T("\n"));
	const CString HEADER_LINE(_T("TODOTIMELOG VERSION 1"));

	CStdioFileEx file;

	if (!file.Open(sLogPath, CFile::modeCreate | CFile::modeWrite | CFile::typeText, SFEF_UTF16))
		return FALSE;

	file.WriteString(HEADER_LINE + ENDL);
	file.WriteString(GetLatestColumnHeader() + ENDL);

	int nNumItems = aLogItems.GetSize();

	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		file.WriteString(aLogItems[nItem].FormatRow(1, sDelim));
		file.WriteString(ENDL);
	}

	file.Close();
	return TRUE;
}

BOOL CTDCAnonymizeTaskTimeLog::AnonymizeLog(CTaskTimeLogItemArray& aLogItems)
{
	// Use a different seed value so that we don't get same
	// result each time we run this method
	srand((unsigned int)time(NULL));

	if (!BuildContent(aLogItems))
		return FALSE;

	// Walk the entries randomising their text columns
	int nItem = aLogItems.GetSize();

	while (nItem--)
	{
		TASKTIMELOGITEM& li = aLogItems[nItem];

		li.sTaskTitle = AnonymizeText(li.sTaskTitle);
		li.sComment = AnonymizeText(li.sComment);
		li.sPerson = AnonymizeText(li.sPerson);
		li.sPath = AnonymizeText(li.sPath);
	}

	return TRUE;
}

CString CTDCAnonymizeTaskTimeLog::GetContent(const CTaskTimeLogItemArray& aLogItems) const
{
	CString sContent;
	int nItem = aLogItems.GetSize();

	while (nItem--)
	{
		const TASKTIMELOGITEM& li = aLogItems[nItem];

		sContent += li.sTaskTitle;
		sContent += li.sComment;
		sContent += li.sPerson;
		sContent += li.sPath;
	}

	return sContent;
}

BOOL CTDCAnonymizeTaskTimeLog::BuildContent(const CTaskTimeLogItemArray& aLogItems)
{
	// Get all task titles and comments
	m_sContent = GetContent(aLogItems);

	// Convert all whitespace and punctuation into spaces
	int nChar = m_sContent.GetLength();

	while (nChar--)
	{
		if (IsWhiteSpaceOrNumberOrPunctuation(m_sContent[nChar]))
			m_sContent.SetAt(nChar, 32);
	}

	// Remove all spaces
	m_sContent.Remove(32);

	return (m_sContent.GetLength() > 0);
}

BOOL CTDCAnonymizeTaskTimeLog::IsWhiteSpaceOrNumberOrPunctuation(TCHAR c)
{
	if (c > 127)
		return FALSE;

	if ((c >= 'A') && (c <= 'Z'))
		return FALSE;

	if ((c >= 'a') && (c <= 'z'))
		return FALSE;

	return TRUE;
}

CString CTDCAnonymizeTaskTimeLog::AnonymizeText(const CString& sItem) const
{
	CString sRandom(sItem);
	int nChar = sRandom.GetLength();

	while (nChar--)
	{
		TCHAR c = sRandom[nChar], cRnd = c;

		if (!IsWhiteSpaceOrNumberOrPunctuation(c))
		{
			while (cRnd == c)
			{
				int nRnd = (rand() % (m_sContent.GetLength() + 1));
				cRnd = m_sContent[nRnd];
			}
		}

		sRandom.SetAt(nChar, cRnd);
	}

	return sRandom;
}


