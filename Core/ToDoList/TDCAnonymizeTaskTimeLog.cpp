// TDCAnonymizeTaskTimeLog.cpp: implementation of the CTDCAnonymizeTaskTimeLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDCAnonymizeTaskTimeLog.h"

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
	return CTDCAnonymizeTaskTimeLog().AnonymizeLog(szLogfile, sAnonFilePath);
}

BOOL CTDCAnonymizeTaskTimeLog::AnonymizeLog(LPCTSTR szLogfile, CString& sAnonFilePath)
{
	CTaskTimeLogItemArray aLogItems;
	CString sDelim;

	Initialise(szLogfile);

	if (!LoadLogFile(szLogfile, aLogItems, FALSE, sDelim))
		return FALSE;

	if (!AnonymizeLogItems(aLogItems))
		return FALSE;

	// Save to alternate filename
	sAnonFilePath = szLogfile;
	FileMisc::AddToFileName(sAnonFilePath, _T(".rnd"));
	
	return CTDCTaskTimeLog::SaveLogFile(sAnonFilePath, aLogItems);
}

BOOL CTDCAnonymizeTaskTimeLog::AnonymizeLogItems(CTaskTimeLogItemArray& aLogItems)
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

		AnonymizeItem(li.sTaskTitle, *m_mapSharedData.GetAddMapping(_T("TITLE")));
		AnonymizeItem(li.sPerson, *m_mapSharedData.GetAddMapping(_T("PERSON")));
		AnonymizeItem(li.sPath, *m_mapSharedData.GetAddMapping(_T("PATH")));
		AnonymizeItem(li.sType, *m_mapSharedData.GetAddMapping(_T("TYPE")));

		li.sComment = AnonymizeText(li.sComment);
	}

	return TRUE;
}

CString CTDCAnonymizeTaskTimeLog::GetContent(const CTaskTimeLogItemArray& aLogItems) const
{
	CString sContent;
	CStringSet mapUniqueValues;

	int nItem = aLogItems.GetSize();

	while (nItem--)
	{
		const TASKTIMELOGITEM& li = aLogItems[nItem];

		// Avoid duplicate title and comment entries
		if (!mapUniqueValues.Has(li.sTaskTitle))
		{
			sContent += li.sTaskTitle;
			mapUniqueValues.Add(li.sTaskTitle);
		}

		if (!mapUniqueValues.Has(li.sComment))
		{
			sContent += li.sComment;
			mapUniqueValues.Add(li.sComment);
		}
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

void CTDCAnonymizeTaskTimeLog::AnonymizeItem(CString& sItem, CMapStringToString& mapTLD) const
{
	if (!sItem.IsEmpty())
	{
		CString sRandom;

		if (!mapTLD.Lookup(sItem, sRandom))
		{
			// add to map
			sRandom = AnonymizeText(sItem);
			mapTLD[sItem] = sRandom;
		}

		sItem = sRandom;
		ASSERT(!sItem.IsEmpty());
	}
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


