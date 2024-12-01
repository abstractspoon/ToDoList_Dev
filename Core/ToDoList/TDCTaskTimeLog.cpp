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

const CString VERSION_LINE	= _T("TODOTIMELOG VERSION");

static CString TAB			= _T("\t");
static CString COMMA		= _T(",");
static CString SEMICOLON	= _T(";");

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
	int nNumHeaderRows;
	int nTimeField;
	int nMinFields;
	int nMaxFields;
};

const LOG_VERSION_INFO LOG_VERSIONS[] = 
{
	{ VER_0,		1, 2, 6, 6 },
	{ VER_LATEST,	2, 7, 8, 12 },
};
const int NUM_LOG_VERSIONS = sizeof(LOG_VERSIONS) / sizeof(LOG_VERSION_INFO);

//////////////////////////////////////////////////////////////////////

TASKTIMELOGITEM::TASKTIMELOGITEM()
{
	Clear(TRUE);
}

BOOL TASKTIMELOGITEM::operator==(const TASKTIMELOGITEM& other) const
{
	return ((other.dwTaskID == dwTaskID) &&
			(other.sTaskTitle == sTaskTitle) &&
			(other.sPerson == sPerson) &&
			(other.dtFrom == dtFrom) &&
			(other.dtTo == dtTo) &&
			(other.dHours == dHours) &&
			(other.sComment == sComment) &&
			(other.sType == sType) &&
			(other.sPath == sPath) &&
			(other.crAltColor == crAltColor));
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

void TASKTIMELOGITEM::Clear(BOOL bInitPerson)
{
	dwTaskID = 0;
	dHours = 0.0;
	crAltColor = CLR_NONE;

	CDateHelper::ClearDate(dtFrom);
	CDateHelper::ClearDate(dtTo);

	sTaskTitle.Empty();
	sComment.Empty();
	sPath.Empty();
	sType = CEnString(IDS_LOG_TYPETRACKED);

	if (bInitPerson)
		sPerson = Misc::GetUserName();
	else
		sPerson.Empty();
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
					 sPerson,
					 CDateHelper::FormatDate(dtTo, DHFD_TIME),
					 CDateHelper::FormatDate(dtFrom, DHFD_TIME));
		break;
		
	case VER_LATEST:
		sItem.Format(sRowFormat,
					 Misc::Format(dwTaskID),
					 sTaskTitle,
					 sPerson,
					 CDateHelper::FormatDate(dtFrom, DHFD_ISO),
					 CTimeHelper::FormatClockTime(dtFrom, FALSE, TRUE),
					 CDateHelper::FormatDate(dtTo, DHFD_ISO),
					 CTimeHelper::FormatClockTime(dtTo, FALSE, TRUE),
					 Misc::Format(dHours, 3),
					 sComment,
					 sType,
					 sPath,
					 ((crAltColor == CLR_NONE) ? _T("") : Misc::Format((int)crAltColor)));
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
	
	Clear();

	// try to determine row version dynamically
	int nRowVer = GetRowVersion(nNumFields);

	if (nRowVer == VER_NONE)
		return FALSE;
	
	COleDateTime date;

	switch (nRowVer)
	{
	case VER_0:
		if (nNumFields == 6)
		{
			dwTaskID = _ttoi(aFields[0]);
			sTaskTitle = aFields[1];
			dHours = ParseTimeSpent(aFields[2]);
			sPerson = aFields[3];
			
			// NOTE: 'To' precedes 'From' because 'From' was added later
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
			
			if (CDateHelper::DecodeDate((aFields[3] + ' ' + aFields[4]), date, TRUE))
				dtFrom = date;
			
			if (CDateHelper::DecodeDate((aFields[5] + ' ' + aFields[6]), date, TRUE))
				dtTo = date;
			
			dHours = ParseTimeSpent(aFields[7]);
			
			// optional fields
			if (nNumFields > 8)
			{
				switch (nNumFields)
				{
					// ADDITIONAL FIELDS ADDED HERE

				case 12:
					if (!aFields[11].IsEmpty())
						crAltColor = _ttoi(aFields[11]);

				case 11: sPath		= aFields[10];
				case 10: sType		= aFields[9];
				case 9:  sComment	= aFields[8];
					break;

				default:
					ASSERT(0);
					break;
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

double TASKTIMELOGITEM::ParseTimeSpent(CString sValue)
{
	// There are essentially only two decimal separators 
	// that the world uses: '.' and ','
	// Plus we know that these entries will not contain thousands separators
	CString sNativeSep = Misc::GetDecimalSeparator();
	CString sAltSep = ((sNativeSep == ".") ? "," : ".");

	sValue.Replace(sAltSep, sNativeSep);

	return _ttof(sValue);
}

BOOL TASKTIMELOGITEM::GetRowVersion(int nNumFields)
{
	int nRowVer = VER_NONE;
	int nVer = NUM_LOG_VERSIONS;
	
	while (nVer--)
	{
		if ((nNumFields >= LOG_VERSIONS[nVer].nMinFields) &&
			(nNumFields <= LOG_VERSIONS[nVer].nMaxFields))
		{
			nRowVer = LOG_VERSIONS[nVer].nVersion;
			ASSERT(nVer == nRowVer);

			return nRowVer;
		}
	}

	// else
	return VER_NONE;
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
	
	for (int nField = 0; nField < vi.nMaxFields; nField++)
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
	m_bUseTabDelim(FALSE)
{
}
	
CTDCTaskTimeLog::CTDCTaskTimeLog(LPCTSTR szRefPath, SFE_FORMAT nFormat) 
	: 
	m_sRefPath(szRefPath), 
	m_nFormat(nFormat),
	m_nVersion(VER_NONE),
	m_bLogExists(FALSE),
	m_bUseTabDelim(FALSE)
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
	li.sType = CEnString(bTracked ? IDS_LOG_TYPETRACKED : IDS_LOG_TYPEADJUSTED);
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

	// init state
	CString sLogPath = GetLogPath(li.dwTaskID, bLogSeparately);

	Initialise(sLogPath);
	ASSERT(m_nVersion != VER_NONE);

	// if the file doesn't exist then we insert the 
	// column headings as the first line
	if (!m_bLogExists)
	{
		CString sHeader;
		sHeader.Format(_T("%s %d\n%s\n"), VERSION_LINE, m_nVersion, GetLatestColumnHeader());

		// if Excel is installed we use UTF16 else UTF8
		SFE_FORMAT nFormat = SFEF_UTF8;

		if (CFileRegister::IsRegisteredApp(_T("csv"), _T("EXCEL.EXE"), TRUE))
			nFormat = SFEF_UTF16;

		VERIFY(FileMisc::CreateFolderFromFilePath(sLogPath));
		VERIFY(m_bLogExists = FileMisc::SaveFile(sLogPath, sHeader, nFormat));
	}

	return FileMisc::AppendLineToFile(sLogPath, li.FormatRow(m_nVersion, GetDelimiter()), SFEF_AUTODETECT);
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
		CString sDrive, sFolder, sFileName;

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

	CString sRowFormat = TASKTIMELOGITEM::GetRowFormat(VER_LATEST, GetDelimiter());
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
						 CEnString(IDS_LOG_PATH),
						 CEnString(IDS_LOG_ALTCOLOR));
	
	return sColumnHeader;
}

// public static helper
int CTDCTaskTimeLog::LoadLogFile(const CString& sLogPath, CTaskTimeLogItemArray& aLogItems, BOOL bAppend, CString& sHeaderDelim)
{
	if (!FileMisc::FileExists(sLogPath))
		return 0;

	CTDCTaskTimeLog log;
	log.Initialise(sLogPath);

	sHeaderDelim = log.m_sHeaderDelim;

	CStringArray aLines;
	int nNumLines = FileMisc::LoadFile(sLogPath, aLines), nItem = 0;

	if (nNumLines)
	{
		CTaskTimeLogItemArray aTempLogItems;

		// skip header and column titles lines
		int nNumHeaderRows = log.GetNumHeaderRows();
		aTempLogItems.SetSize(nNumLines - nNumHeaderRows);

		for (int nLine = nNumHeaderRows; nLine < nNumLines; nLine++)
		{
			const CString& sLine = Misc::GetItem(aLines, nLine);
			CString sDelim = log.GetDelimiter(sLine);

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

	return nItem;
}

BOOL CTDCTaskTimeLog::SaveLogFile(const CString& sLogPath, const CTaskTimeLogItemArray& aLogItems, BOOL bPreserveVersion)
{
	// sanity check
	if (aLogItems.GetSize() == 0)
	{
		ASSERT(0);
		return FALSE;
	}

	CString sTempFile;

	if (!bPreserveVersion)
	{
		// Rename the log file so it won't be found by 'Initialise'
		sTempFile = FileMisc::GetTempFilePath(sLogPath, _T("bak"));
		FileMisc::MoveFile(sLogPath, sTempFile);
	}

	CTDCTaskTimeLog log;
	log.Initialise(sLogPath);

	CString sDelim = log.GetDelimiter();
	int nVersion = log.m_nVersion;
	int nNumHeaderRows = log.GetNumHeaderRows();
	SFE_FORMAT nFormat = log.m_nFormat;

	CStringArray aLines;
	int nNumItems = aLogItems.GetSize(), nLine = 0;
	aLines.SetSize(nNumHeaderRows + nNumItems);

	if (nVersion != VER_0)
		aLines[nLine++].Format(_T("%s %n"), VERSION_LINE, nVersion);

	aLines[nLine++] = log.m_sColumnHeader;

	for (int nItem = 0; nItem < nNumItems; nItem++, nLine++)
		aLines[nLine] = aLogItems[nItem].FormatRow(nVersion, sDelim);

	CString sFileContents = Misc::FormatArray(aLines, '\n');

	if (!FileMisc::SaveFile(sLogPath, sFileContents, nFormat)) 
	{
		VERIFY(FileMisc::MoveFile(sTempFile, sLogPath));
		return FALSE;
	}

	return TRUE;
}

void CTDCTaskTimeLog::Initialise(const CString& sLogPath)
{
	// Once only
	if (m_nVersion != VER_NONE)
		return; 

	m_bLogExists = FileMisc::FileExists(sLogPath);
	m_bUseTabDelim = CFileRegister::IsRegisteredApp(_T("csv"), _T("EXCEL.EXE"), TRUE);

	m_nVersion = VER_LATEST; // default
	m_sHeaderDelim = GetDelimiter(); // default
	m_sColumnHeader = GetLatestColumnHeader(); // default

	if (m_bLogExists)
	{
		CStringArray aLines;

		if (FileMisc::LoadFile(sLogPath, aLines, 2))
		{
			CString sLine = aLines[0];
			
			// version
			if (sLine.Find(VERSION_LINE) != -1)
			{
				CString sVer;
				Misc::Split(sLine, sVer, VERSION_LINE);

				m_nVersion = _ttoi(sVer);
				sLine = aLines[1];
			}
			else
			{
				m_nVersion = VER_0;
			}

			// Test for column header delimiter
			m_sColumnHeader = sLine;
			m_sHeaderDelim = TAB;

			CStringArray aUnused;
			int nNumFields = Misc::Split(m_sColumnHeader, aUnused, m_sHeaderDelim);

			if (TASKTIMELOGITEM::GetRowVersion(nNumFields) != VER_NONE)
			{
				m_bUseTabDelim = TRUE;
			}
			else
			{
				m_bUseTabDelim = FALSE;
				
				// Try some others
				m_sHeaderDelim = COMMA;

				nNumFields = Misc::Split(m_sColumnHeader, aUnused, m_sHeaderDelim);

				if (TASKTIMELOGITEM::GetRowVersion(nNumFields) == VER_NONE)
				{
					m_sHeaderDelim = SEMICOLON;
					nNumFields = Misc::Split(m_sColumnHeader, aUnused, m_sHeaderDelim);

					if (TASKTIMELOGITEM::GetRowVersion(nNumFields) == VER_NONE)
						m_sHeaderDelim.Empty(); // unknown
				}
			}
		}
	}
}

CString CTDCTaskTimeLog::GetDelimiter(const CString& sLine) const
{
	if (!sLine.IsEmpty())
	{
		ASSERT(isdigit(sLine[0]));

		int nLen = sLine.GetLength(), nPos = 0;

		while (++nPos < nLen)
		{
			TCHAR nVal = sLine[nPos];

			if (!isdigit(nVal))
				return CString(nVal);
		}
	}

	// else
	return (m_bUseTabDelim ? TAB : Misc::GetListSeparator());
}

int CTDCTaskTimeLog::GetNumHeaderRows() const
{
	if ((m_nVersion < 0) || (m_nVersion >= NUM_LOG_VERSIONS))
	{
		ASSERT(0);
		return 0;
	}

	return LOG_VERSIONS[m_nVersion].nNumHeaderRows;
}
