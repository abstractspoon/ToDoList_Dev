// TaskTimeLog.cpp: implementation of the CTDCTaskTimeLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "TDCTaskTimeLogAnalysis.h"
#include "TaskFile.h"
#include "TDCStatic.h"

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

#define MAGIC_TASKID	    999999999
#define MAGIC_TASKIDSTR _T("999999999")

//////////////////////////////////////////////////////////////////////

static LPCTSTR TAB = _T("\t");

static LPCTSTR HEADERFMT_PERIOD_GROUPBY = _T("%s\t%s\t%s\t%s\t%s\t%s\n");
static LPCTSTR HEADERFMT_PERIOD_NOGROUPBY = _T("%s\t%s\t%s\t%s\t%s\n");
static LPCTSTR HEADERFMT_NOPERIOD_GROUPBY = _T("%s\t%s\t%s\t%s\t%s\n");
static LPCTSTR HEADERFMT_NOPERIOD_NOGROUPBY = _T("%s\t%s\t%s\t%s\n");

static LPCTSTR ROWFMT_PERIOD_GROUPBY = _T("%s\t%s\t%lu\t%s\t%0.3f\t%s\n");
static LPCTSTR ROWFMT_PERIOD_NOGROUPBY = _T("%s\t%lu\t%s\t%0.3f\t%s\n");
static LPCTSTR ROWFMT_NOPERIOD_GROUPBY = _T("%s\t%lu\t%s\t%0.3f\t%s\n");
static LPCTSTR ROWFMT_NOPERIOD_NOGROUPBY = _T("%lu\t%s\t%0.3f\t%s\n");

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

struct LOGSORTITEM
{
	DWORD dwTaskID;
	CString sGroupBy;
};

int LogSortProc(const void* pV1, const void* pV2)
{
	const LOGSORTITEM* pLI1 = (LOGSORTITEM*)pV1;
	const LOGSORTITEM* pLI2 = (LOGSORTITEM*)pV2;

	// Sort by group then by ID
	int nCompare = Misc::NaturalCompare(pLI1->sGroupBy, pLI2->sGroupBy, TRUE);

	if (nCompare != 0)
		return nCompare;

	// else
	if (pLI1->dwTaskID < pLI2->dwTaskID)
		return -1;

	if (pLI1->dwTaskID > pLI2->dwTaskID)
		return 1;

	// else
	return 0;
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

CTDCTaskTimeLogAnalysis::CTDCTaskTimeLogAnalysis(const CString& sTaskList, 
												 const CTDCCustomAttribDefinitionArray& aCustomAttribDefs)
	:
	m_sTaskFile(sTaskList), 
	m_aCustomAttribDefs(aCustomAttribDefs),
	m_nGroupBy(TDCA_NONE)
{
	ASSERT(FileMisc::FileExists(m_sTaskFile));

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
											 TDC_ATTRIBUTE nGroupBy,
											 TDCTTL_FORMAT nFormat,
											 LPCTSTR szOutputFile)
{
	if (!BuildLogItemArray())
		return FALSE;

	m_nGroupBy = nGroupBy;
	ASSERT((nGroupBy == TDCA_NONE) || !GetGroupByHeader().IsEmpty());

	BuildGroupByMapping();
	
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

int CTDCTaskTimeLogAnalysis::GetLogFilePaths(CStringArray& aFilePaths) const
{
	CTDCTaskTimeLog log(m_sTaskFile);

	// Get the individual task log files if they exist
	CString sFilter = log.GetLogPath(MAGIC_TASKID, TRUE);
	sFilter.Replace(MAGIC_TASKIDSTR, _T("*"));

	FileMisc::FindFiles(FileMisc::GetFolderFromFilePath(sFilter),
						aFilePaths, 
						TRUE, 
						FileMisc::GetFileNameFromPath(sFilter));

	// Prepend the 'global' log file if it exists
	CString sLogPath(log.GetLogPath());

	if (FileMisc::FileExists(sLogPath))
		aFilePaths.InsertAt(0, sLogPath);

	return aFilePaths.GetSize();
}

int CTDCTaskTimeLogAnalysis::BuildLogItemArray()
{
	m_aLogItems.RemoveAll();
	m_mapIDtoRefLogItem.RemoveAll();

	// Get a list of log files needing processing
	CStringArray aLogFiles;

	int nNumLogFiles = GetLogFilePaths(aLogFiles);

	if (nNumLogFiles == 0)
		return FALSE;

	// iterate the list of files building a master list of log items
	for (int nFile = 0; nFile < nNumLogFiles; nFile++)
	{
		CString sDelim;
		
		if (CTDCTaskTimeLog::LoadLogItems(aLogFiles[nFile], m_aLogItems, TRUE, sDelim))
		{
			if (m_sCsvDelim.IsEmpty())
			{
				ASSERT(!sDelim.IsEmpty());
				m_sCsvDelim = sDelim;
			}
		}
	}

	// Build reverse lookup
	int nNumItems = m_aLogItems.GetSize(), nItem = nNumItems;

	while (nItem--)
	{
		const TASKTIMELOGITEM& li = GetLogItem(nItem);
		int nLogItem = -1;

		if (!m_mapIDtoRefLogItem.Lookup(li.dwTaskID, nLogItem))
			m_mapIDtoRefLogItem[li.dwTaskID] = nItem;
	}
		
	return m_aLogItems.GetSize();
}

int CTDCTaskTimeLogAnalysis::BuildGroupByMapping()
{
	m_mapIDtoGroupBy.RemoveAll();

	if (WantGroupBy())
	{
		CTaskFile tasks;
		VERIFY(tasks.Load(m_sTaskFile));

		CString sCustAttribID;
		int nCust = m_aCustomAttribDefs.Find(m_nGroupBy);

		if (nCust != -1)
			sCustAttribID = m_aCustomAttribDefs[nCust].sUniqueID;

		BuildGroupByMapping(tasks, tasks.GetFirstTask(), m_nGroupBy, sCustAttribID, m_mapIDtoGroupBy);
	}

	return m_mapIDtoGroupBy.GetCount();
}

// static helper
int CTDCTaskTimeLogAnalysis::BuildGroupByMapping(const CTaskFile& tasks, HTASKITEM hTask, TDC_ATTRIBUTE nGroupBy, const CString& sCustGroupByAttrib, CMap<DWORD, DWORD, CString, LPCTSTR>& mapGroupBy)
{
	ASSERT(nGroupBy != TDCA_NONE);

	if (hTask == NULL)
		return 0;

	CString sGroupBy;

	if (!sCustGroupByAttrib.IsEmpty())
	{
		sGroupBy = tasks.GetTaskCustomAttributeData(hTask, sCustGroupByAttrib);
	}
	else
	{
		switch (nGroupBy)
		{
		case TDCA_ALLOCBY:		sGroupBy = tasks.GetTaskAllocatedBy(hTask);		break;
		case TDCA_ALLOCTO:		sGroupBy = tasks.GetTaskAllocatedTo(hTask, 0);	break;
		case TDCA_CATEGORY:		sGroupBy = tasks.GetTaskCategory(hTask, 0);		break;
		case TDCA_CREATEDBY:	sGroupBy = tasks.GetTaskCreatedBy(hTask);		break;
		case TDCA_EXTERNALID:	sGroupBy = tasks.GetTaskExternalID(hTask);		break;
		case TDCA_LASTMODBY:	sGroupBy = tasks.GetTaskLastModifiedBy(hTask);	break;
		case TDCA_STATUS:		sGroupBy = tasks.GetTaskStatus(hTask);			break;
		case TDCA_TAGS:			sGroupBy = tasks.GetTaskTag(hTask, 0);			break;
		case TDCA_VERSION:		sGroupBy = tasks.GetTaskVersion(hTask);			break;
	
		case TDCA_PRIORITY:		
		{
			int nPriority = tasks.GetTaskPriority(hTask, false);

			if (nPriority >= 0)
				sGroupBy = Misc::Format(nPriority);
		}
		break;

		case TDCA_RISK:
		{
			int nRisk = tasks.GetTaskRisk(hTask, false);

			if (nRisk >= 0)
				sGroupBy = Misc::Format(nRisk);
		}
		break;

		default:
			ASSERT(0);
		}

		mapGroupBy[tasks.GetTaskID(hTask)] = sGroupBy;
	}

	// First child
	BuildGroupByMapping(tasks, tasks.GetFirstTask(hTask), nGroupBy, sCustGroupByAttrib, mapGroupBy);

	// Next sibling
	BuildGroupByMapping(tasks, tasks.GetNextTask(hTask), nGroupBy, sCustGroupByAttrib, mapGroupBy);

	return mapGroupBy.GetCount();
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

				CString sLastGroupBy;

				for (int nItem = 0; nItem < nNumItems; nItem++)
				{
					DWORD dwTaskID = aSortedIDs[nItem];
					double dTime = 0;

					// get the time for the task
					VERIFY(mapIDs.Lookup(dwTaskID, dTime));
					ASSERT(dwTaskID > 0);
					
					// get the title and path of the task
					const TASKTIMELOGITEM& liRef = GetReferenceLogItem(dwTaskID);
					CString sGroupBy = GetTaskGroupBy(dwTaskID);

					// Add spacer between groups
					if (WantGroupBy() && (sLastGroupBy != sGroupBy))
					{
						sLastGroupBy = sGroupBy;

						if (nItem > 0)
							file.WriteString(_T("\n"));
					}

					// write to file
					file.WriteString(FormatCsvRow(dwTaskID, liRef.sTaskTitle, dTime, liRef.sPath, _T(""), sGroupBy));
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
				
				pXITask->SetItemValue(_T("ID"), (int)dwTaskID);
				pXITask->SetItemValue(_T("TITLE"), liRef.sTaskTitle);
				pXITask->SetItemValue(_T("HOURS"), dTime);
				pXITask->SetItemValue(_T("PERSON"), liRef.sPerson);
				pXITask->SetItemValue(_T("PATH"), liRef.sPath);

				if (WantGroupBy())
					pXITask->SetItemValue(_T("GROUPBY"), GetTaskGroupBy(dwTaskID));

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
		if (WantGroupBy())
		{
			sHeader.Format(HEADERFMT_PERIOD_GROUPBY,
						   CEnString(IDS_LOG_PERIOD),
						   GetGroupByHeader(),
						   CEnString(IDS_LOG_TASKID),
						   CEnString(IDS_LOG_TASKTITLE),
						   CEnString(IDS_LOG_TIMESPENT),
						   CEnString(IDS_LOG_PATH));
		}
		else
		{
			sHeader.Format(HEADERFMT_PERIOD_NOGROUPBY,
						   CEnString(IDS_LOG_PERIOD),
						   CEnString(IDS_LOG_TASKID),
						   CEnString(IDS_LOG_TASKTITLE),
						   CEnString(IDS_LOG_TIMESPENT),
						   CEnString(IDS_LOG_PATH));
		}
	}
	else // by task
	{
		if (WantGroupBy())
		{
			sHeader.Format(HEADERFMT_NOPERIOD_GROUPBY,
						   GetGroupByHeader(),
						   CEnString(IDS_LOG_TASKID),
						   CEnString(IDS_LOG_TASKTITLE),
						   CEnString(IDS_LOG_TIMESPENT),
						   CEnString(IDS_LOG_PATH));
		}
		else
		{
			sHeader.Format(HEADERFMT_NOPERIOD_NOGROUPBY,
						   CEnString(IDS_LOG_TASKID),
						   CEnString(IDS_LOG_TASKTITLE),
						   CEnString(IDS_LOG_TIMESPENT),
						   CEnString(IDS_LOG_PATH));
		}
	}
	sHeader.Replace(TAB, m_sCsvDelim);

	return sHeader;
}

CString CTDCTaskTimeLogAnalysis::FormatCsvRow(DWORD dwTaskID, const CString& sTaskTitle, double dTime, 
											  const CString& sPath, const CString& sPeriod, const CString& sGroupBy) const
{
	CString sRow;
	
	if (!sPeriod.IsEmpty())
	{
		if (WantGroupBy())
			sRow.Format(ROWFMT_PERIOD_GROUPBY, sPeriod, sGroupBy, dwTaskID, sTaskTitle, dTime, sPath);
		else
			sRow.Format(ROWFMT_PERIOD_NOGROUPBY, sPeriod, dwTaskID, sTaskTitle, dTime, sPath);
	}
	else
	{
		if (WantGroupBy())
			sRow.Format(ROWFMT_NOPERIOD_GROUPBY, sGroupBy, dwTaskID, sTaskTitle, dTime, sPath);
		else
			sRow.Format(ROWFMT_NOPERIOD_NOGROUPBY, dwTaskID, sTaskTitle, dTime, sPath);
	}
	sRow.Replace(TAB, m_sCsvDelim);

	return sRow;
}

BOOL CTDCTaskTimeLogAnalysis::WantGroupBy() const
{
	return (m_nGroupBy != TDCA_NONE);
}

CString CTDCTaskTimeLogAnalysis::GetTaskGroupBy(DWORD dwTaskID) const
{
	CString sGroupBy;

	if (WantGroupBy())
		m_mapIDtoGroupBy.Lookup(dwTaskID, sGroupBy);

	return sGroupBy;
}

CString CTDCTaskTimeLogAnalysis::GetGroupByHeader() const
{
	if (WantGroupBy())
	{
		switch (m_nGroupBy)
		{
		case TDCA_PRIORITY:		return CEnString(IDS_TDLBC_PRIORITY);
		case TDCA_ALLOCTO:		return CEnString(IDS_TDLBC_ALLOCTO);
		case TDCA_ALLOCBY:		return CEnString(IDS_TDLBC_ALLOCBY);
		case TDCA_STATUS:		return CEnString(IDS_TDLBC_STATUS);
		case TDCA_CATEGORY:		return CEnString(IDS_TDLBC_CATEGORY);
		case TDCA_TAGS:			return CEnString(IDS_TDLBC_TAGS);
		case TDCA_CREATEDBY:	return CEnString(IDS_TDLBC_CREATEDBY);
		case TDCA_RISK:			return CEnString(IDS_TDLBC_RISK);
		case TDCA_EXTERNALID:	return CEnString(IDS_TDLBC_EXTERNALID);
		case TDCA_VERSION:		return CEnString(IDS_TDLBC_VERSION);
		case TDCA_LASTMODBY:	return CEnString(IDS_TDLBC_LASTMODBY);
		}

		// else
		int nCust = m_aCustomAttribDefs.Find(m_nGroupBy);

		if (nCust != -1)
			return m_aCustomAttribDefs[nCust].sLabel;

		// else
		ASSERT(0);
	}

	return _T("");
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

					CString sLastGroupBy;
					
					for (int nItem = 0; nItem < nNumItems; nItem++)
					{
						DWORD dwTaskID = aSortedIDs[nItem];
						double dTime = 0;
						
						// get the time for the task
						VERIFY(pPeriod->Lookup(dwTaskID, dTime));
						ASSERT(dwTaskID > 0);
						
						// get the title and path of the task
						const TASKTIMELOGITEM& liRef = GetReferenceLogItem(dwTaskID);
						CString sGroupBy = GetTaskGroupBy(dwTaskID);

						// Add spacer between groups
						if (WantGroupBy() && (sLastGroupBy != sGroupBy))
						{
							sLastGroupBy = sGroupBy;

							if (nItem > 0)
								file.WriteString(_T("\n"));
						}

						// write to file
						file.WriteString(FormatCsvRow(dwTaskID, liRef.sTaskTitle, dTime, liRef.sPath, sPeriod, sGroupBy));
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

					pXITask->SetItemValue(_T("ID"), (int)dwTaskID);
					pXITask->SetItemValue(_T("TITLE"), liRef.sTaskTitle);
					pXITask->SetItemValue(_T("HOURS"), dTime);
					pXITask->SetItemValue(_T("PERSON"), liRef.sPerson);
					pXITask->SetItemValue(_T("PATH"), liRef.sPath);

					if (WantGroupBy())
						pXITask->SetItemValue(_T("GROUPBY"), GetTaskGroupBy(dwTaskID));
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
		return (CDateHelper::GetWeekofYear(date1) == CDateHelper::GetWeekofYear(date2));

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

int CTDCTaskTimeLogAnalysis::BuildSortedIDList(const CMapIDToTime& mapIDs, CDWordArray& aIDs) const
{
	aIDs.RemoveAll();

	if (mapIDs.GetCount())
	{
		// Build intermediate array of items for sorting
		CArray<LOGSORTITEM, LOGSORTITEM&> aSortItems;
		aSortItems.SetSize(mapIDs.GetCount());

		POSITION pos = mapIDs.GetStartPosition();
		int nItem = 0;

		DWORD dwTaskID = 0;
		double dUnused = 0;
		
		while (pos)
		{
			mapIDs.GetNextAssoc(pos, dwTaskID, dUnused);
			ASSERT(dwTaskID > 0);
			
			LOGSORTITEM& li = aSortItems[nItem++];

			li.dwTaskID = dwTaskID;

			if (WantGroupBy())
				m_mapIDtoGroupBy.Lookup(dwTaskID, li.sGroupBy);
		}

		// Sort and convert to simple ID array
		Misc::SortArrayT<LOGSORTITEM>(aSortItems, LogSortProc);

		aIDs.SetSize(mapIDs.GetCount());
		nItem = aIDs.GetSize();

		while (nItem--)
			aIDs[nItem] = aSortItems.GetData()[nItem].dwTaskID;
	}

	return aIDs.GetSize();
}
