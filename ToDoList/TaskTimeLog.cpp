// TaskTimeLog.cpp: implementation of the CTDCTaskTimeLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "TaskTimeLog.h"

#include "..\shared\Filemisc.h"
#include "..\shared\misc.h"
#include "..\shared\datehelper.h"
#include "..\shared\timehelper.h"
#include "..\shared\enstring.h"
#include "..\shared\FileRegister.h"
#include "..\shared\xmlfile.h"

#include "..\3rdParty\stdiofileex.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const LPCTSTR HEADER_LINE = _T("TODOTIMELOG VERSION");

#define MAGIC_TASKID	    999999999
#define MAGIC_TASKIDSTR _T("999999999")

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

SFE_FORMAT GetCsvFileFormat()
{
	// if Excel is installed we use UTF16 else UTF8
	SFE_FORMAT nFormat = SFEF_UTF8;
	
	if (CFileRegister::IsRegisteredApp(_T("csv"), _T("EXCEL.EXE"), TRUE))
		nFormat = SFEF_UTF16;

	return nFormat;
}

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
	CString sLogPath, sDrive, sFolder, sFileName;

	// use ref filename as the basis for the log filename
	FileMisc::SplitPath(m_sRefPath, &sDrive, &sFolder, &sFileName);
	
	if (bLogSeparately)
		sLogPath.Format(_T("%s%s%s\\%lu_Log.csv"), sDrive, sFolder, sFileName, dwTaskID);
	else
		sLogPath.Format(_T("%s%s%s_Log.csv"), sDrive, sFolder, sFileName);

	return sLogPath;
}

double CTDCTaskTimeLog::CalcAccumulatedTime(DWORD dwTaskID, BOOL bLogSeparately)
{
	CString sLogPath = GetLogPath(dwTaskID, bLogSeparately);
	double dTotalTime = 0;

	if (FileMisc::FileExists(sLogPath))
	{
		CStdioFileEx file;

		const LOG_VERSION_INFO& vi = LOG_VERSIONS[m_nVersion];
		ASSERT(vi.nVersion == m_nVersion);

		if (file.Open(sLogPath, CFile::modeRead | CFile::typeText))
		{
			CString sLine;

			while (file.ReadString(sLine))
			{
				// decode it
				CStringArray aParts;

				if (Misc::Split(sLine, aParts, _T(""), TRUE) > vi.nTimeField)
				{
					long nLogID = _ttol(aParts[0]); // always the first item

					if (nLogID > 0 && (DWORD)nLogID == dwTaskID)
					{
						dTotalTime += _ttof(aParts[vi.nTimeField]);
					}
				}
			}
		}
	}

	return dTotalTime;
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
int CTDCTaskTimeLog::ReadLogItems(const CString& sLogPath, CTaskTimeLogItemArray& aLogItems, BOOL bAppend, CString& sDelim)
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

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCTaskTimeLogAnalysis::CMapIDToTimeAndPeriod::CMapIDToTimeAndPeriod(double dPeriod)
	: dtPeriod(floor(dPeriod)) 
{
}

BOOL CTDCTaskTimeLogAnalysis::CMapIDToTimeAndPeriod::IsValid() const
{
	return ((dtPeriod.m_dt != 0.0) && GetCount());
}

//////////////////////////////////////////////////////////////////////

// cleanup map resources
CTDCTaskTimeLogAnalysis::CMapIDToTimeAndPeriodArray::~CMapIDToTimeAndPeriodArray()
{
	int nPeriod = GetSize();

	while (nPeriod--)
		delete GetAt(nPeriod);

	RemoveAll();
}

int CTDCTaskTimeLogAnalysis::CMapIDToTimeAndPeriodArray::Find(double dPeriod) const
{
	int nPeriod = GetSize();
	
	while (nPeriod--)
	{
		if (floor(dPeriod) == floor(GetAt(nPeriod)->dtPeriod))
			return nPeriod;
	}
	
	// else 
	return -1;
}

CTDCTaskTimeLogAnalysis::CMapIDToTimeAndPeriod* CTDCTaskTimeLogAnalysis::CMapIDToTimeAndPeriodArray::Get(double dPeriod)
{
	int nPeriod = Find(dPeriod);
	return ((nPeriod == -1) ? NULL : GetAt(nPeriod));
}

const CTDCTaskTimeLogAnalysis::CMapIDToTimeAndPeriod* CTDCTaskTimeLogAnalysis::CMapIDToTimeAndPeriodArray::Get(double dPeriod) const
{
	int nPeriod = Find(dPeriod);
	return ((nPeriod == -1) ? NULL : GetAt(nPeriod));
}

void CTDCTaskTimeLogAnalysis::CMapIDToTimeAndPeriodArray::Sort()
{
	if (GetSize() > 1)
		qsort(GetData(), GetSize(), sizeof(CMapIDToTimeAndPeriod*), SortProc);
}

int CTDCTaskTimeLogAnalysis::CMapIDToTimeAndPeriodArray::SortProc(const void* ppPeriod1, const void* ppPeriod2)
{
	const CMapIDToTimeAndPeriod* pPeriod1 = *((const CMapIDToTimeAndPeriod**)ppPeriod1);
	const CMapIDToTimeAndPeriod* pPeriod2 = *((const CMapIDToTimeAndPeriod**)ppPeriod2);

	ASSERT(pPeriod1->IsValid() && pPeriod2->IsValid());

	if (pPeriod1->dtPeriod > pPeriod2->dtPeriod)
		return 1;

	if (pPeriod1->dtPeriod < pPeriod2->dtPeriod)
		return -1;
	
	ASSERT(0);
	return 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCTaskTimeLogAnalysis::CTDCTaskTimeLogAnalysis(const CString& sTaskList, BOOL bLogTaskTimeSeparately) 
	:
	m_sTasklist(sTaskList), 
	m_bLogTaskTimeSeparately(bLogTaskTimeSeparately)
{
	ASSERT(FileMisc::FileExists(m_sTasklist));

	// if Excel is installed we always use a tab as delimiter
	if (CFileRegister::IsRegisteredApp(_T("csv"), _T("EXCEL.EXE"), TRUE))
		m_sCsvDelim = TAB;
}

CTDCTaskTimeLogAnalysis::~CTDCTaskTimeLogAnalysis()
{

}

BOOL CTDCTaskTimeLogAnalysis::AnalyseTaskLog(const COleDateTime& dtFrom, 
											const COleDateTime& dtTo,
											TDCTTL_BREAKDOWN nBreakdown,
											TDCTTL_FORMAT nFormat,
											LPCTSTR szOutputFile)
{
	if (!BuildLogItemArray())
		return FALSE;

	switch (nBreakdown)
	{
	case TTLB_BYTASK:
		{
			CMapIDToTime mapIDs;

			if (AnalyseByTask(dtFrom, dtTo, mapIDs))
				return OutputAnalysis(mapIDs, nFormat, szOutputFile);
		}
		break;

	case TTLB_BYDAY:
	case TTLB_BYWEEK:
	case TTLB_BYMONTH:
		{
			CMapIDToTimeAndPeriodArray aPeriods;

			if (AnalyseByDate(dtFrom, dtTo, aPeriods))
			{
				if (BreakdownDateAnalysis(aPeriods, nBreakdown))
					return OutputAnalysis(aPeriods, nBreakdown, nFormat, szOutputFile);
			}
		}
		break;
	}

	// all else
	return FALSE;
}

int CTDCTaskTimeLogAnalysis::BuildLogItemArray()
{
	m_aLogItems.RemoveAll();
	m_mapIDtoRefLogItem.RemoveAll();

	// Build a list of log files needing processing
	CTDCTaskTimeLog log(m_sTasklist);
	CStringArray aLogFiles;

	if (m_bLogTaskTimeSeparately)
	{
		// get a file filter
		CString sFilter = log.GetLogPath(MAGIC_TASKID, TRUE);
		sFilter.Replace(MAGIC_TASKIDSTR, _T("*"));

		FileMisc::FindFiles(FileMisc::GetFolderFromFilePath(sFilter),
							aLogFiles, 
							TRUE, 
							FileMisc::GetFileNameFromPath(sFilter));
	}
	else
	{
		aLogFiles.Add(log.GetLogPath());
	}

	int nNumLogFiles = aLogFiles.GetSize();

	if (nNumLogFiles == 0)
		return FALSE;

	// iterate the list of files building a master list of log items
	for (int nFile = 0; nFile < nNumLogFiles; nFile++)
	{
		CString sDelim;
		
		if (CTDCTaskTimeLog::ReadLogItems(aLogFiles[nFile], m_aLogItems, TRUE, sDelim))
		{
			if (m_sCsvDelim.IsEmpty())
			{
				ASSERT(!sDelim.IsEmpty());
				m_sCsvDelim = sDelim;
			}
		}
	}

	// build the map linking taskID to Log item index
	int nItem = m_aLogItems.GetSize();

	while (nItem--)
	{
		const TASKTIMELOGITEM& li = GetLogItem(nItem);
		int nLogItem = -1;

		if (!m_mapIDtoRefLogItem.Lookup(li.dwTaskID, nLogItem))
			m_mapIDtoRefLogItem[li.dwTaskID] = nItem;
	}

	return m_aLogItems.GetSize();
}

const TASKTIMELOGITEM& CTDCTaskTimeLogAnalysis::GetLogItem(int nIndex) const
{
	ASSERT(nIndex >= 0 && nIndex < m_aLogItems.GetSize());
	
	return (m_aLogItems.GetData()[nIndex]);
}

const TASKTIMELOGITEM& CTDCTaskTimeLogAnalysis::GetReferenceLogItem(DWORD dwTaskID) const
{
	// look up a Log item for reference
	int nLogIndex = -1;
	
	VERIFY(m_mapIDtoRefLogItem.Lookup(dwTaskID, nLogIndex));
	ASSERT(nLogIndex != -1);
	
	const TASKTIMELOGITEM& li = GetLogItem(nLogIndex);
	ASSERT(dwTaskID == li.dwTaskID);

	return li;
}

BOOL CTDCTaskTimeLogAnalysis::AnalyseByTask(const COleDateTime& dtFrom, 
											const COleDateTime& dtTo,
											CMapIDToTime& mapIDs) const
{
	int nNumItems = m_aLogItems.GetSize();

	if (nNumItems == 0)
		return FALSE;

	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		const TASKTIMELOGITEM& li = GetLogItem(nItem);

		// does this item fall into the date range
		if ((li.dtFrom >= dtFrom) && (li.dtFrom < dtTo))
		{
			AppendLogItemToMap(li, mapIDs);
		}
	}

	return (mapIDs.GetCount() > 0);
}

CString CTDCTaskTimeLogAnalysis::FormatCsvRow(DWORD dwTaskID, const CString& sTaskTitle, 
											  double dTime, const CString& sPath, const CString& sPeriod) const
{
	CString sRow;

	if (sPeriod.IsEmpty())
		sRow.Format(_T("%lu\t%s\t%0.3f\t%s\n"), dwTaskID, sTaskTitle, dTime, sPath);
	else
		sRow.Format(_T("%s\t%lu\t%s\t%0.3f\t%s\n"), sPeriod, dwTaskID, sTaskTitle, dTime, sPath);

	sRow.Replace(TAB, m_sCsvDelim);

	return sRow;
}

BOOL CTDCTaskTimeLogAnalysis::OutputAnalysis(const CMapIDToTime& mapIDs,
											TDCTTL_FORMAT nFormat,
											LPCTSTR szOutputFile) const
{
	// sanity checks
	ASSERT(mapIDs.GetCount());

	if (mapIDs.GetCount() == 0)
		return FALSE;

	switch (nFormat)
	{
	case TTLF_CSV:
		{
			CStdioFileEx file;

			if (file.Open(szOutputFile, CFile::modeCreate | CFile::modeWrite | CFile::typeText, GetCsvFileFormat()))
			{
				// header row
				file.WriteString(BuildCsvHeader(FALSE));

				// build list if tasks sorted by ID
				CDWordArray aSortedIDs;
				int nNumItems = BuildSortedIDList(mapIDs, aSortedIDs);

				for (int nItem = 0; nItem < nNumItems; nItem++)
				{
					DWORD dwTaskID = aSortedIDs[nItem];
					double dTime = 0;

					// get the time for the task
					VERIFY(mapIDs.Lookup(dwTaskID, dTime));
					ASSERT(dwTaskID > 0);
					
					// get the title and path of tha task
					const TASKTIMELOGITEM& liRef = GetReferenceLogItem(dwTaskID);
					
					// write to file
					file.WriteString(FormatCsvRow(dwTaskID, liRef.sTaskTitle, dTime, liRef.sPath));
				}

				file.Close();
				return TRUE;
			}
		}
		break;

	case TTLF_XML:
		{
			CXmlFile file(_T("TODOLIST_LOG_REPORT"));

			// NOTE: no need to sort the taskIDs first because
			// XML is inherently unsorted and so there's no point
			POSITION pos = mapIDs.GetStartPosition();
			
			while (pos)
			{
				DWORD dwTaskID = 0;
				double dTime = 0;
				
				mapIDs.GetNextAssoc(pos, dwTaskID, dTime);
				ASSERT(dwTaskID > 0);
				
				// look up a Log item for reference
				const TASKTIMELOGITEM& liRef = GetReferenceLogItem(dwTaskID);
				
				// write to file
				CXmlItem* pXITask = file.AddItem(_T("TASK"), _T(""), XIT_ELEMENT);
				
				pXITask->SetItemValue(_T("ID"), (int)liRef.dwTaskID);
				pXITask->SetItemValue(_T("TITLE"), liRef.sTaskTitle);
				pXITask->SetItemValue(_T("HOURS"), dTime);
				pXITask->SetItemValue(_T("PERSON"), liRef.sPerson);
				pXITask->SetItemValue(_T("PATH"), liRef.sPath);
			}
			
			return file.Save(szOutputFile, SFEF_UTF8WITHOUTBOM);
		}
		break;

	case TTLF_TXT:
		ASSERT(0);
		break;

	case TTLF_HTM:
		ASSERT(0);
		break;

	default:
		ASSERT(0);
		break;
	}

	return FALSE;
}

BOOL CTDCTaskTimeLogAnalysis::AnalyseByDate(const COleDateTime& dtFrom, 
											const COleDateTime& dtTo, 
											CMapIDToTimeAndPeriodArray& aPeriods) const
{
	// sanity check
	ASSERT(m_aLogItems.GetSize());

	int nNumItems = m_aLogItems.GetSize();

	if (nNumItems == 0)
		return FALSE;

	// Build a map of days since this is our 
	// smallest unit of breakdown
	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		const TASKTIMELOGITEM& li = m_aLogItems.GetData()[nItem];

		// does this item fall into the date range
		if ((li.dtFrom >= dtFrom) && (li.dtFrom < dtTo))
		{
			// look up existing map for the day
			CMapIDToTimeAndPeriod* pDayMap = aPeriods.Get(li.dtFrom);

			if (!pDayMap)
			{
				pDayMap = new CMapIDToTimeAndPeriod(li.dtFrom);
				aPeriods.Add(pDayMap);
			}
			else
				ASSERT(pDayMap->IsValid());

			AppendLogItemToMap(li, *pDayMap);
		}
	}

	aPeriods.Sort();

	return (aPeriods.GetSize() > 0);
}

BOOL CTDCTaskTimeLogAnalysis::BreakdownDateAnalysis(CMapIDToTimeAndPeriodArray& aPeriods,
													TDCTTL_BREAKDOWN nBreakdown) const
{
	// sanity checks
	ASSERT(nBreakdown != TTLB_BYTASK);
	ASSERT(aPeriods.GetSize());

	if (!aPeriods.GetSize() || (nBreakdown == TTLB_BYTASK))
		return FALSE;

	switch (nBreakdown)
	{
	case TTLB_BYDAY:
		// nothing more to do
		break;

	case TTLB_BYWEEK:
	case TTLB_BYMONTH:
		{
			// determine the start/end dates
			double dStart = 0, dEnd = 0;

			VERIFY(GetMinMaxDates(aPeriods, dStart, dEnd));

			// work our way thru the date-sorted array
			// merging ID maps having the same time period
			COleDateTime dtPeriod(dEnd);
			CMapIDToTimeAndPeriod* pPeriodMap = NULL;

			int nPeriod = aPeriods.GetSize();

			while (nPeriod--)
			{
				CMapIDToTimeAndPeriod* pDayMap = aPeriods[nPeriod];
				ASSERT(pDayMap && pDayMap->IsValid());

				if (pPeriodMap == NULL)
				{
					// first item
					pPeriodMap = pDayMap;
					ASSERT(pDayMap->dtPeriod.GetMonth() == dtPeriod.GetMonth());
				}
				else if (IsSamePeriod(pDayMap->dtPeriod, dtPeriod, nBreakdown))
				{
					Merge(*pDayMap, *pPeriodMap);

					// delete and remove
					delete pDayMap;
					aPeriods.RemoveAt(nPeriod);
				}
				else // new period
				{
					pPeriodMap = pDayMap;
					dtPeriod = pPeriodMap->dtPeriod;
				}
			}
		}
		break;
	}

	return TRUE;
}

CString CTDCTaskTimeLogAnalysis::BuildCsvHeader(BOOL bBreakdownByPeriod) const
{
	CString sHeader;

	if (bBreakdownByPeriod)
	{
		sHeader.Format(_T("%s\t%s\t%s\t%s\t%s\n"),
			CEnString(IDS_LOG_PERIOD),
			CEnString(IDS_LOG_TASKID),
			CEnString(IDS_LOG_TASKTITLE),
			CEnString(IDS_LOG_TIMESPENT),
			CEnString(IDS_LOG_PATH));
	}
	else // by task
	{
		sHeader.Format(_T("%s\t%s\t%s\t%s\n"),
			CEnString(IDS_LOG_TASKID),
			CEnString(IDS_LOG_TASKTITLE),
			CEnString(IDS_LOG_TIMESPENT),
			CEnString(IDS_LOG_PATH));
	}
	sHeader.Replace(TAB, m_sCsvDelim);

	return sHeader;
}


BOOL CTDCTaskTimeLogAnalysis::OutputAnalysis(const CMapIDToTimeAndPeriodArray& aPeriods,
											TDCTTL_BREAKDOWN nBreakdown,
											TDCTTL_FORMAT nFormat,
											LPCTSTR szOutputFile) const
{
	ASSERT(nBreakdown != TTLB_BYTASK);

	if (nBreakdown == TTLB_BYTASK)
		return FALSE;

	switch (nFormat)
	{
	case TTLF_CSV:
		{
			CStdioFileEx file;

			if (file.Open(szOutputFile, CFile::modeCreate | CFile::modeWrite | CFile::typeText, GetCsvFileFormat()))
			{
				// header row
				file.WriteString(BuildCsvHeader(TRUE));

				int nNumPeriods = aPeriods.GetSize();

				for (int nPeriod = 0; nPeriod < nNumPeriods; nPeriod++)
				{
					const CMapIDToTimeAndPeriod* pPeriod = aPeriods[nPeriod];
					ASSERT(pPeriod);

					// format string for this period
					CString sPeriod = FormatPeriod(pPeriod->dtPeriod, nBreakdown);

					// build list if tasks sorted by ID
					CDWordArray aSortedIDs;
					int nNumItems = BuildSortedIDList(*pPeriod, aSortedIDs);
					
					for (int nItem = 0; nItem < nNumItems; nItem++)
					{
						DWORD dwTaskID = aSortedIDs[nItem];
						double dTime = 0;
						
						// get the time for the task
						VERIFY(pPeriod->Lookup(dwTaskID, dTime));
						ASSERT(dwTaskID > 0);
						
						// get the title and path of the task
						const TASKTIMELOGITEM& liRef = GetReferenceLogItem(dwTaskID);
						
						// write to file
						file.WriteString(FormatCsvRow(dwTaskID, liRef.sTaskTitle, dTime, liRef.sPath, sPeriod));
					}

					// spacer between periods
					file.WriteString(_T("\n"));
				}

				file.Close();
				return TRUE;
			}
		}
		break;

	case TTLF_XML:
		{
			CXmlFile file(_T("TODOLIST_LOG_REPORT"));

			int nNumPeriods = aPeriods.GetSize();
			
			for (int nPeriod = 0; nPeriod < nNumPeriods; nPeriod++)
			{
				const CMapIDToTimeAndPeriod* pPeriod = aPeriods[nPeriod];
				ASSERT(pPeriod && pPeriod->IsValid());
				
				// new period
				CXmlItem* pXIPeriod = file.AddItem(_T("PERIOD"), _T(""), XIT_ELEMENT);

				// format string for this period
				CString sPeriod = FormatPeriod(pPeriod->dtPeriod, nBreakdown);
				pXIPeriod->SetItemValue(_T("DATE_RANGE"), sPeriod);
				
				// iterate the items in this period
				POSITION posID = pPeriod->GetStartPosition();
				
				while (posID)
				{
					DWORD dwTaskID = 0;
					double dTime = 0;
					
					pPeriod->GetNextAssoc(posID, dwTaskID, dTime);
					ASSERT(dwTaskID > 0);
					
					// look up a Log item for reference
					const TASKTIMELOGITEM& liRef = GetReferenceLogItem(dwTaskID);
			
					// write to file
					CXmlItem* pXITask = pXIPeriod->AddItem(_T("TASK"), _T(""), XIT_ELEMENT);

					pXITask->SetItemValue(_T("ID"), (int)liRef.dwTaskID);
					pXITask->SetItemValue(_T("TITLE"), liRef.sTaskTitle);
					pXITask->SetItemValue(_T("HOURS"), dTime);
					pXITask->SetItemValue(_T("PERSON"), liRef.sPerson);
					pXITask->SetItemValue(_T("PATH"), liRef.sPath);
				}
			}
			
			return file.Save(szOutputFile, SFEF_UTF8WITHOUTBOM);
		}
		break;

	case TTLF_TXT:
		ASSERT(0);
		break;

	case TTLF_HTM:
		ASSERT(0);
		break;

	default:
		ASSERT(0);
		break;
	}

	return FALSE;
}

CString CTDCTaskTimeLogAnalysis::FormatPeriod(double dDay, TDCTTL_BREAKDOWN nBreakdown)
{
	CString sPeriod;
	COleDateTime date(dDay);
	
	switch (nBreakdown)
	{
	case TTLB_BYDAY:
		sPeriod = date.Format(VAR_DATEVALUEONLY);
		break;
		
	case TTLB_BYWEEK:
		{
			// have to get the start of the week

			int nFirstDOW = CDateHelper::GetFirstDayOfWeek();
			int nDOW = date.GetDayOfWeek();

			COleDateTime dtStart = (date.m_dt - (nDOW - nFirstDOW));
			COleDateTime dtEnd = (dtStart.m_dt + 6);

			sPeriod.Format(_T("%s - %s"), dtStart.Format(VAR_DATEVALUEONLY), dtEnd.Format(VAR_DATEVALUEONLY));
		}
		break;
		
	case TTLB_BYMONTH:
		{
			// have to get start of this month
			int nYear = date.GetYear(), nMonth = date.GetMonth();

			COleDateTime dtStart(nYear, nMonth, 1, 0, 0, 0);
			COleDateTime dtEnd(nYear, nMonth, CDateHelper::GetDaysInMonth(nMonth, nYear), 0, 0, 0);

			sPeriod.Format(_T("%s - %s"), dtStart.Format(VAR_DATEVALUEONLY), dtEnd.Format(VAR_DATEVALUEONLY));
		}
		break;
		
	default:
		ASSERT(0);
	}

	return sPeriod;
}

void CTDCTaskTimeLogAnalysis::Merge(const CMapIDToTime& mapFrom, CMapIDToTime& mapTo)
{
	ASSERT(mapFrom.GetCount());
	ASSERT(mapTo.GetCount());

	POSITION pos = mapFrom.GetStartPosition();
	double dTimeFrom = 0, dTimeTo = 0;
	DWORD dwTaskID = 0;

	while (pos)	
	{
		mapFrom.GetNextAssoc(pos, dwTaskID, dTimeFrom);
		ASSERT(dwTaskID > 0);

		// look up ID in 'mapTo'
		if (mapTo.Lookup(dwTaskID, dTimeTo))
			dTimeTo += dTimeFrom;
		else
			dTimeTo = dTimeFrom;

		mapTo[dwTaskID] = dTimeTo;
	}
}

BOOL CTDCTaskTimeLogAnalysis::GetMinMaxDates(const CMapIDToTimeAndPeriodArray& aPeriods, double& dStart, double& dEnd)
{
	dStart = dEnd = 0;

	int nPeriod = aPeriods.GetSize();

	while (nPeriod--)
	{
		double dDay = aPeriods[nPeriod]->dtPeriod;

		dStart = ((dStart == 0) ? dDay : min(dStart, dDay));
		dEnd   = ((dEnd == 0)   ? dDay : max(dEnd, dDay));
	}

	return ((dStart != 0) && (dEnd != 0));
}

BOOL CTDCTaskTimeLogAnalysis::IsSamePeriod(const COleDateTime& date1, const COleDateTime& date2, TDCTTL_BREAKDOWN nBreakdown)
{
	ASSERT(!CDateHelper::DateHasTime(date1) && !CDateHelper::DateHasTime(date2));

	switch (nBreakdown)
	{
	case TTLB_BYDAY:
		return (date1 == date2);

	case TTLB_BYWEEK:
		return (CDateHelper::GetISOWeekofYear(date1) == CDateHelper::GetISOWeekofYear(date2));

	case TTLB_BYMONTH:
		return ((date1.GetMonth() == date2.GetMonth()) &&
				(date1.GetYear()  == date2.GetYear()));
	}

	ASSERT(0);
	return FALSE;
}

void CTDCTaskTimeLogAnalysis::AppendLogItemToMap(const TASKTIMELOGITEM& li, CMapIDToTime& mapIDs)
{
	// lookup existing time for this task
	double dTime = 0;
	mapIDs.Lookup(li.dwTaskID, dTime);

	// update map
	mapIDs[li.dwTaskID] = (dTime + li.dHours);
}

int CTDCTaskTimeLogAnalysis::BuildSortedIDList(const CMapIDToTime& mapIDs, CDWordArray& aIDs)
{
	aIDs.RemoveAll();

	if (mapIDs.GetCount())
	{
		aIDs.SetSize(mapIDs.GetCount());

		POSITION pos = mapIDs.GetStartPosition();
		int nItem = 0;
		
		while (pos)
		{
			DWORD dwTaskID = 0;
			double dTime = 0;
			
			mapIDs.GetNextAssoc(pos, dwTaskID, dTime);
			ASSERT(dwTaskID > 0);
			
			aIDs[nItem++] = dwTaskID;
		}

		Misc::SortArray(aIDs);
	}

	return aIDs.GetSize();
}
