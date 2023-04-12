// TaskTimeLog.cpp: implementation of the CTDCTaskTimeLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "TDCTaskTimeLog.h"

#include "..\shared\Filemisc.h"
#include "..\shared\misc.h"
#include "..\shared\datehelper.h"
#include "..\shared\timehelper.h"
#include "..\shared\enstring.h"
#include "..\shared\FileRegister.h"
#include "..\shared\xmlfile.h"

#include "..\3rdParty\stdiofileex.h"

#include <math.h>

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const LPCTSTR HEADER_LINE = _T("TODOTIMELOG VERSION");

static LPCTSTR TAB = _T("\t");

//////////////////////////////////////////////////////////////////////

enum CSVFMT_LOG_VERSION
{
	VER_NONE = -1,
	VER_0,
	// insert here
	VER_LATEST
};

//////////////////////////////////////////////////////////////////////

struct LOG_VERSION_INFO
{
	CSVFMT_LOG_VERSION nVersion;
	int nTimeField;
	int nNumFields;
};

const LOG_VERSION_INFO LOG_VERSIONS[] = 
{
	{ VER_0, 2, 6 },
	{ VER_LATEST, 7, 11 },
};
const int NUM_LOG_VERSIONS = sizeof(LOG_VERSIONS) / sizeof(LOG_VERSION_INFO);

//////////////////////////////////////////////////////////////////////

TASKTIMELOGITEM::TASKTIMELOGITEM()
{
	Reset();
}

BOOL TASKTIMELOGITEM::IsValidToLog() const
{
	if (dwTaskID && !sTaskTitle.IsEmpty())
	{
		// must have a comment or valid time
		return (!sComment.IsEmpty() || IsValidToAnalyse());
	}

	// else 
	return FALSE;
}

BOOL TASKTIMELOGITEM::IsValidToAnalyse() const
{
	if (dwTaskID && !sTaskTitle.IsEmpty())
	{
		return ((dHours != 0) && CDateHelper::IsDateSet(dtFrom) && (dtTo >= dtFrom));
	}

	// else 
	return FALSE;
}

void TASKTIMELOGITEM::Reset()
{
	dwTaskID = 0;
	dHours = 0.0;
	bTracked = TRUE;

	CDateHelper::ClearDate(dtFrom);
	CDateHelper::ClearDate(dtTo);

	sTaskTitle.Empty();
	sComment.Empty();
	sPerson.Empty();
	sPath.Empty();
}

CString TASKTIMELOGITEM::FormatRow(int nRowVer, const CString& sDelim) const
{
	ASSERT(IsValidToLog());

	CString sItem, sRowFormat(GetRowFormat(nRowVer, sDelim));
	
	switch (nRowVer)
	{
	case VER_0:
		sItem.Format(sRowFormat, 
					Misc::Format(dwTaskID),
					sTaskTitle,
					Misc::Format(dHours, 3), 
					Misc::GetUserName(),
					CDateHelper::FormatDate(dtTo, DHFD_TIME), 
					CDateHelper::FormatDate(dtFrom, DHFD_TIME));
		break;
		
	case VER_LATEST:
		sItem.Format(sRowFormat, 
					Misc::Format(dwTaskID), 
					sTaskTitle,
					Misc::GetUserName(),
					CDateHelper::FormatDate(dtFrom, DHFD_ISO),
					CTimeHelper::FormatClockTime(dtFrom, FALSE, TRUE), 
					CDateHelper::FormatDate(dtTo, DHFD_ISO), 
					CTimeHelper::FormatClockTime(dtTo, FALSE, TRUE),
					Misc::Format(dHours, 3),
					sComment,
					CEnString(bTracked ? IDS_LOG_TYPETRACKED : IDS_LOG_TYPEADJUSTED),
					sPath);		
		break;
		
	default:
		ASSERT(0);
	}

	// handle newline chars
	sItem.Replace(_T("\r"), _T(""));
	sItem.Replace('\n', '|');
	
	return sItem;
}



BOOL TASKTIMELOGITEM::ParseRow(const CString& sRow, const CString& sDelim)
{
	// NOTE: order of elements as FormatItemRow above
	CStringArray aFields;
	int nNumFields = Misc::Split(sRow, aFields, sDelim, TRUE);
	
	if (!nNumFields)
		return FALSE;
	
	// try to determine row version dynamically
	int nRowVer = GetRowVersion(nNumFields);
	ASSERT (nRowVer != -1);
	
	COleDateTime date;
	Reset();
	
	switch (nRowVer)
	{
	case VER_0:
		if (nNumFields == 6)
		{
			dwTaskID = _ttoi(aFields[0]);
			sTaskTitle = aFields[1];
			dHours = _ttof(aFields[2]);
			sPerson = aFields[3];
			
			// NOTE: 'To' precedes 'From' because 'To' was added later
			if (CDateHelper::DecodeDate(aFields[4], date, TRUE))
				dtTo = date;
			
			if (CDateHelper::DecodeDate(aFields[5], date, TRUE))
				dtFrom = date;
		}
		else
		{
			ASSERT(0);
		}
		break;
		
	case VER_LATEST:
		if (nNumFields >= 8)
		{
			dwTaskID = _ttoi(aFields[0]);
			sTaskTitle = aFields[1];
			sPerson = aFields[2];
			
			if (CDateHelper::DecodeDate(aFields[3], date, TRUE))
				dtFrom = date;
			
			if (CDateHelper::DecodeDate(aFields[5], date, TRUE))
				dtTo = date;
			
			dHours = _ttof(aFields[7]);
			
			// optional fields
			if (nNumFields > 8)
			{
				sComment = aFields[8];
				
				if (nNumFields > 9)
				{
					bTracked = _ttoi(aFields[9]);
					
					if (nNumFields > 10)
						sPath = aFields[10];
				}
			}
		}
		else
		{
			ASSERT(0);
		}
		break;
		
	default:
		ASSERT(0);
		break;
	}
	
	return IsValidToAnalyse();
}

BOOL TASKTIMELOGITEM::GetRowVersion(int nNumFields)
{
	int nRowVer = VER_NONE;
	int nVer = NUM_LOG_VERSIONS;
	
	while (nVer--)
	{
		if (LOG_VERSIONS[nVer].nNumFields == nNumFields)
		{
			nRowVer = LOG_VERSIONS[nVer].nVersion;
			ASSERT(nVer == nRowVer);

			return nRowVer;
		}
	}

	// else
	return VER_LATEST;
}

CString TASKTIMELOGITEM::GetRowFormat(int nRowVer, const CString& sDelim)
{
	// sanity checks
	ASSERT(VER_LATEST == NUM_LOG_VERSIONS - 1);
	ASSERT(nRowVer <= VER_LATEST);
	ASSERT(!sDelim.IsEmpty());
	
	const LOG_VERSION_INFO& vi = LOG_VERSIONS[nRowVer];
	ASSERT(vi.nVersion == nRowVer);
	
	// build row format from 
	CString sRowFormat;
	
	for (int nField = 0; nField < vi.nNumFields; nField++)
	{
		if (!sRowFormat.IsEmpty())
			sRowFormat += sDelim;
		
		sRowFormat += _T("%s");
	}
	
	return sRowFormat;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCTaskTimeLog::CTDCTaskTimeLog() 
	: 
	m_nFormat(SFEF_AUTODETECT),
	m_nVersion(VER_NONE),
	m_bLogExists(FALSE),
	m_sDelim(Misc::GetListSeparator())
{
}
	
CTDCTaskTimeLog::CTDCTaskTimeLog(LPCTSTR szRefPath, SFE_FORMAT nFormat) 
	: 
	m_sRefPath(szRefPath), 
	m_nFormat(nFormat),
	m_nVersion(VER_NONE),
	m_bLogExists(FALSE),
	m_sDelim(Misc::GetListSeparator())
{
}

CTDCTaskTimeLog::~CTDCTaskTimeLog()
{

}

BOOL CTDCTaskTimeLog::LogTime(DWORD dwTaskID, LPCTSTR szTaskTitle, LPCTSTR szTaskPath, 
							  double dHours, const COleDateTime& dtWhen, const CString& sComment, 
							  BOOL bTracked, BOOL bLogSeparately)
{
	TASKTIMELOGITEM li;

	li.dwTaskID = dwTaskID;
	li.sTaskTitle = szTaskTitle;
	li.sComment = sComment;
	li.sPath = szTaskPath;
	li.sPerson = Misc::GetUserName();
	li.bTracked = bTracked;
	li.dHours = dHours;
	li.dtTo = dtWhen;
	li.dtFrom = dtWhen;

	if (dHours > 0)
		li.dtFrom.m_dt -= (dHours / 24);

	return LogTime(li, bLogSeparately);
}

BOOL CTDCTaskTimeLog::LogTime(const TASKTIMELOGITEM& li, BOOL bLogSeparately)
{
	if (!li.IsValidToLog())
		return FALSE;

	CString sLogPath = GetLogPath(li.dwTaskID, bLogSeparately);

	// init state
	Initialise(sLogPath);
	ASSERT(m_nVersion != VER_NONE);

	// if the file doesn't exist then we insert the 
	// column headings as the first line
	if (!m_bLogExists)
	{
		CString sHeader;
		sHeader.Format(_T("%s %d\n%s\n"), HEADER_LINE, m_nVersion, GetLatestColumnHeader());

		// if Excel is installed we use UTF16 else UTF8
		SFE_FORMAT nFormat = SFEF_UTF8;

		if (CFileRegister::IsRegisteredApp(_T("csv"), _T("EXCEL.EXE"), TRUE))
			nFormat = SFEF_UTF16;

		VERIFY(FileMisc::CreateFolderFromFilePath(sLogPath));
		VERIFY(FileMisc::SaveFile(sLogPath, sHeader, nFormat));
	}

	return FileMisc::AppendLineToFile(sLogPath, li.FormatRow(m_nVersion, m_sDelim), SFEF_AUTODETECT);
}

CString CTDCTaskTimeLog::GetLogPath() const
{
	return GetLogPath(0, FALSE);
}

CString CTDCTaskTimeLog::GetLogPath(DWORD dwTaskID, BOOL bLogSeparately) const
{
	CString sLogPath;

	// use ref filename as the basis for the log filename
	if (!m_sRefPath.IsEmpty())
	{
		CString sLogPath, sDrive, sFolder, sFileName;

		FileMisc::SplitPath(m_sRefPath, &sDrive, &sFolder, &sFileName);

		if (bLogSeparately)
			sLogPath.Format(_T("%s%s%s\\%lu_Log.csv"), sDrive, sFolder, sFileName, dwTaskID);
		else
			sLogPath.Format(_T("%s%s%s_Log.csv"), sDrive, sFolder, sFileName);
	}

	return sLogPath;
}

CString CTDCTaskTimeLog::GetLatestColumnHeader() const // always the latest version
{
	// sanity check
	ASSERT(VER_LATEST == NUM_LOG_VERSIONS - 1);

	CString sRowFormat = TASKTIMELOGITEM::GetRowFormat(VER_LATEST, m_sDelim);
	CString sColumnHeader;

	sColumnHeader.Format(sRowFormat,
						CEnString(IDS_LOG_TASKID),
						CEnString(IDS_LOG_TASKTITLE),
						CEnString(IDS_LOG_USERID),
						CEnString(IDS_LOG_STARTDATE),
						CEnString(IDS_LOG_STARTTIME),
						CEnString(IDS_LOG_ENDDATE),
						CEnString(IDS_LOG_ENDTIME),
						CEnString(IDS_LOG_TIMESPENT),
						CEnString(IDS_LOG_COMMENT),
						CEnString(IDS_LOG_TYPE),
						CEnString(IDS_LOG_PATH));
	
	return sColumnHeader;
}

// public static helper
int CTDCTaskTimeLog::LoadLogItems(const CString& sLogPath, CTaskTimeLogItemArray& aLogItems, BOOL bAppend, CString& sDelim)
{
	CTDCTaskTimeLog log;
	log.Initialise(sLogPath);

	sDelim = log.GetDelimiter();

	if (FileMisc::FileExists(sLogPath))
	{
		CStringArray aLines;
		int nNumLines = FileMisc::LoadFile(sLogPath, aLines);

		if (nNumLines)
		{
			int nItem = 0;
			CTaskTimeLogItemArray aTempLogItems;

			// skip header and column titles lines
			aTempLogItems.SetSize(nNumLines - 2);

			for (int nLine = 2; nLine < nNumLines; nLine++)
			{
				const CString& sLine = Misc::GetItem(aLines, nLine);
				TASKTIMELOGITEM& li = aTempLogItems[nItem];

				if (li.ParseRow(sLine, sDelim))
					nItem++;
			}

			if (nItem)
			{
				// Remove unused items
				aTempLogItems.SetSize(nItem);

				if (bAppend && aLogItems.GetSize())
					aLogItems.Append(aTempLogItems);
				else
					aLogItems.Copy(aTempLogItems);
			}
		}
	}

	return aLogItems.GetSize();
}

void CTDCTaskTimeLog::Initialise(const CString& sLogPath)
{
	// Once only
	if (m_nVersion != VER_NONE)
		return; 

	m_bLogExists = FileMisc::FileExists(sLogPath);

	if (!m_bLogExists) // new log file
	{
		m_nVersion = VER_LATEST;
	
		// if Excel is installed we use a tab as delimiter
		if (CFileRegister::IsRegisteredApp(_T("csv"), _T("EXCEL.EXE"), TRUE))
			m_sDelim = TAB;
	}
	else
	{
		// get version and delimiter from file
		m_nVersion = VER_0;
		
		CStringArray aLines;
		
		if (FileMisc::LoadFile(sLogPath, aLines, 2))
		{
			CString sLine = aLines[0];
			
			// version
			if (sLine.Find(HEADER_LINE) != -1)
			{
				int nSpace = sLine.ReverseFind(' ');
				
				if (nSpace != -1)
				{
					m_nVersion = _ttoi(((LPCTSTR)sLine) + nSpace + 1);
					sLine = aLines[1];
				}
			}

			// check for tab char in column header
			if (sLine.Find(TAB) != -1)
				m_sDelim = TAB;
		}
	}
}

