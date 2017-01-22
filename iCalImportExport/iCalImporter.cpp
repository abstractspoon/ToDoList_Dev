// iCalImporter.cpp: implementation of the CiCalImporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "iCalImportExport.h"
#include "iCalImporter.h"

#include "..\shared\misc.h"
#include "..\shared\filemisc.h"
#include "..\shared\datehelper.h"

#include "..\todolist\tdcenum.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

struct LINETYPE
{
	ICI_LINETYPE nType;
	LPCTSTR szKey;
};

static LINETYPE LINETYPES[] = 
{
	{ ICILT_BEGIN_VCAL,		_T("BEGIN:VCALENDAR") },

	{ ICILT_PRODID,			_T("PRODID") },
	{ ICILT_VERSION,		_T("VERSION") },
			
	{ ICILT_BEGIN_VTIMEZONE,_T("BEGIN:VTIMEZONE") },
	{ ICILT_BEGIN_STANDARD,	_T("BEGIN:STANDARD") },
	{ ICILT_BEGIN_DAYLIGHT,	_T("BEGIN:DAYLIGHT") },
	{ ICILT_BEGIN_VEVENT,	_T("BEGIN:VEVENT") },
	{ ICILT_BEGIN_VTODO,	_T("BEGIN:VTODO") },
	{ ICILT_BEGIN_VALARM,	_T("BEGIN:VALARM") },

	{ ICILT_DTSTART,		_T("DTSTART;VALUE=DATE-TIME:") },
	{ ICILT_DTDUE,			_T("DUE;VALUE=DATE-TIME:") },
	{ ICILT_DTEND,			_T("DTEND;VALUE=DATE-TIME:") },
	{ ICILT_DTSTART,		_T("DTSTART;VALUE=DATE:") },
	{ ICILT_DTDUE,			_T("DUE;VALUE=DATE:") },
	{ ICILT_DTEND,			_T("DTEND;VALUE=DATE:") },
	{ ICILT_SUMMARY,		_T("SUMMARY") },
	{ ICILT_STATUS,			_T("STATUS") },
	{ ICILT_CATEGORIES,		_T("CATEGORIES") },
	{ ICILT_URL,			_T("URL") },
	{ ICILT_ORGANIZER,		_T("ORGANIZER") },
	{ ICILT_ORGANIZERCN,	_T("ORGANIZER;CN=") },
	{ ICILT_ATTENDEE,		_T("ATTENDEE") },
	{ ICILT_UID,			_T("UID") },
	{ ICILT_DESCRIPTION,	_T("DESCRIPTION") },
	{ ICILT_RELATEDTOPARENT,_T("RELATED-TO;RELTYPE=PARENT") },
	{ ICILT_PERCENT,		_T("PERCENT") },
	{ ICILT_PRIORITY,		_T("PRIORITY") },
	{ ICILT_RECURRENCE,		_T("RRULE") },

	{ ICILT_END_VTIMEZONE,	_T("END:VTIMEZONE") },
	{ ICILT_END_STANDARD,	_T("END:STANDARD") },
	{ ICILT_END_DAYLIGHT,	_T("END:DAYLIGHT") },
	{ ICILT_END_VEVENT,		_T("END:VEVENT") },
	{ ICILT_END_VTODO,		_T("END:VTODO") },
	{ ICILT_END_VALARM,		_T("END:VALARM") },

	{ ICILT_END_VCAL,		_T("END:VCALENDAR") }
};
const UINT NUM_LINETYPES = (sizeof(LINETYPES) / sizeof(LINETYPE));

//////////////////////////////////////////////////////////////////////

struct ICALITEM
{
	ICALITEM() 
		: nPriority(5), nPercent(0), nRegularity(TDIR_ONCE), dwRegSpecific1(0), dwRegSpecific2(0) 
	{
	}

	CString sSummary;
	CString sDescription;
	CString sOrganizer;
	CString sAttendee;
	CString sStatus;
	CString sCategories;
	CString sUrl;
	CString sUID, sUIDParent;
	COleDateTime dtStart, dtDue, dtEnd;
	unsigned char nPercent;
	int nPriority;
	int nRegularity;
	DWORD dwRegSpecific1, dwRegSpecific2;
};

CiCalEventArray::~CiCalEventArray()
{
	int nEvent = GetSize();
	
	while (nEvent--)
		delete GetAt(nEvent);
	
	RemoveAll();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CiCalImporter::CiCalImporter() : m_hIcon(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICALENDAR);
}

CiCalImporter::~CiCalImporter()
{
	::DestroyIcon(m_hIcon);
}

void CiCalImporter::SetLocalizer(ITransText* /*pTT*/)
{
	//CLocalizer::Initialize(pTT);
}

#define CHECK_STATE(cur_state, new_state) {	if (nState == cur_state	nState = new_state;	else nState = ICIS_ERROR; }

IIMPORT_RESULT CiCalImporter::Import(LPCTSTR szSrcFilePath, ITaskList* pDestTaskFile, bool /*bSilent*/, IPreferences* /*pPrefs*/, LPCTSTR /*szKey*/)
{
	ITaskList12* pTasks = GetITLInterface<ITaskList12>(pDestTaskFile, IID_TASKLIST12);

	if (!pTasks)
	{
		ASSERT(0);
		return IIR_BADINTERFACE;
	}
		
	CStringArray aLines;

	if (!FileMisc::LoadFile(szSrcFilePath, aLines))
		return IIR_BADFILE;

	if (PreProcessFileLines(aLines))
	{
#ifdef _DEBUG
		FileMisc::SaveFile(_T("test_ics.txt"), Misc::FormatArray(aLines, _T("\r\n")), SFEF_UTF8WITHOUTBOM);
#endif
		
		// import to temp array
		CiCalEventArray aTasks;
		
		if (!ImportTasks(aLines, aTasks))
			return IIR_BADFORMAT;
		
		// import temp array to tasklist
		ImportTasks(aTasks, pTasks);
	}

	return IIR_SUCCESS;
}

int CiCalImporter::ImportTasks(const CStringArray& aLines, CiCalEventArray& aTasks)
{
	// process the lines
	int nNumLines = aLines.GetSize();

	ICI_STATE nState = ICIS_BEGIN;
	ICALITEM* pTask = NULL;
	
	for (int nLine = 0; nLine < nNumLines; nLine++)
	{
		const CString& sLine = aLines[nLine];
		CString sValue;

		ICI_LINETYPE nType = ExtractInfo(sLine, sValue, nState);

		if (nType == ICIS_ERROR)
			continue; // unrecognised

		ICI_STATE nPrevState = nState;
		nState = UpdateState(nType, nPrevState);

		// state-related handing
		switch (nState)
		{
		case ICIS_END:
			nLine = nNumLines; // this will break the outer loop
			continue;

		case ICIS_VTIMEZONE:
		case ICIS_STANDARD:
		case ICIS_DAYLIGHT:
		case ICIS_VALARM:
			// not handled for now
			continue;
		
		case ICIS_VTODO: 
		case ICIS_VEVENT: 
			if (nPrevState == ICIS_VCAL)
			{
				pTask = new ICALITEM;
				ASSERT(pTask);

				aTasks.Add(pTask);

				// keep track of attributes we don't know about
				m_aUnhandledTaskAttrib.RemoveAll();
				break;
			}

			// data-related handling
			switch (nType)
			{
			case ICILT_PRODID:
				// not used
				break;
				
			case ICILT_VERSION:
				// not used
				break;
				
			case ICILT_DTSTART:
				ExtractDate(sValue, pTask->dtStart, FALSE);
				break;
				
			case ICILT_DTDUE:
				ExtractDate(sValue, pTask->dtDue, TRUE);
				break;
				
			case ICILT_DTEND:
				ExtractDate(sValue, pTask->dtEnd, TRUE);
				break;
				
			case ICILT_SUMMARY:
				pTask->sSummary = sValue;
				break;
				
			case ICILT_STATUS:
				pTask->sStatus = sValue;
				break;
				
			case ICILT_CATEGORIES:
				pTask->sCategories = sValue;
				break;
				
			case ICILT_URL:
				pTask->sUrl = sValue;
				break;
				
			case ICILT_ORGANIZER:
				pTask->sOrganizer = sValue;
				break;
				
			case ICILT_ORGANIZERCN:
				pTask->sOrganizer = sValue;

				if (pTask->sUrl.IsEmpty())
					Misc::Split(pTask->sOrganizer, pTask->sUrl, ':');
				break;
				
			case ICILT_ATTENDEE:
				pTask->sAttendee = sValue;
				break;
				
			case ICILT_DESCRIPTION:
				pTask->sDescription = sValue;
				pTask->sDescription.Replace(_T("\\N"), _T("\n"));
				break;
				
			case ICILT_UID:
				pTask->sUID = sValue;
				break;
				
			case ICILT_PERCENT:
				pTask->nPercent = (unsigned char)_ttoi(sValue);
				break;
				
			case ICILT_PRIORITY:
				pTask->nPriority = _ttoi(sValue);
				break;
				
			case ICILT_RECURRENCE:
				DecodeRecurrence(sValue, pTask->nRegularity, pTask->dwRegSpecific1, pTask->dwRegSpecific2);
				break;

			case ICILT_RELATEDTOPARENT:
				pTask->sUIDParent = sValue;
				break;

			default:
				if (nPrevState != ICIS_VALARM)
					m_aUnhandledTaskAttrib.Add(sLine);
			}
			break;
		}
	}

	// sort tasks to ensure parents appear before children
	qsort(aTasks.GetData(), aTasks.GetSize(), sizeof(ICALITEM*), SortProc);

	// sanity check
	VERIFY (nState == ICIS_END);

	return aTasks.GetSize();
}

int CiCalImporter::SortProc(const void* v1, const void* v2)
{
	const ICALITEM* pEvent1 = *((ICALITEM**)v1);
	const ICALITEM* pEvent2 = *((ICALITEM**)v2);

	// sort parents before children
	if (pEvent1->sUID == pEvent2->sUIDParent)
		return -1;

	if (pEvent1->sUIDParent == pEvent2->sUID)
		return 1;
	
	// all else
	return 0;
}

bool CiCalImporter::ImportTasks(const CiCalEventArray& aTasks, ITaskList12* pTasks)
{
	// map tasks as we go so we can lookup parents when we need
	CMapStringToPtr mapTasks;

	// tasks should now be in parent->task order
	int nNumTasks = aTasks.GetSize();

	for (int nTask = 0; nTask < nNumTasks; nTask++) 
	{
		const ICALITEM* pEvent = aTasks[nTask];
		ASSERT(pEvent);

		// find its parent
		HTASKITEM hParent = NULL;
		mapTasks.Lookup(pEvent->sUIDParent, hParent);

		// create and map tasks
		HTASKITEM hTask = pTasks->NewTask(pEvent->sSummary, hParent, 0);
		ASSERT(hTask);

		mapTasks[pEvent->sUID] = hTask;

		// set task dates
		time64_t tDate;

		if (CDateHelper::GetTimeT64(pEvent->dtStart, tDate))
			pTasks->SetTaskStartDate64(hTask, tDate);

		if (CDateHelper::GetTimeT64(pEvent->dtDue, tDate))
			pTasks->SetTaskDueDate64(hTask, tDate);
		
		// for TODOs, dtEnd could be due date or completed date
		// depending on the value of nPercent
		if (CDateHelper::GetTimeT64(pEvent->dtEnd, tDate))
		{
			if (pEvent->nPercent < 100)
				pTasks->SetTaskDueDate64(hTask, tDate);
			else
				pTasks->SetTaskDoneDate64(hTask, tDate);
		}
		
		// rest of attributes
		pTasks->SetTaskStatus(hTask, pEvent->sStatus);
		pTasks->SetTaskFileLinkPath(hTask, pEvent->sUrl);
		pTasks->SetTaskAllocatedBy(hTask, pEvent->sOrganizer);
		pTasks->SetTaskAllocatedTo(hTask, pEvent->sAttendee);
		pTasks->SetTaskExternalID(hTask, pEvent->sUID);
		pTasks->SetTaskPercentDone(hTask, (unsigned char)min(100, max(0, pEvent->nPercent)));
		pTasks->SetTaskPriority(hTask, min(10, max(-2, pEvent->nPriority)));

		// recurrence
		if (pEvent->nRegularity != TDIR_ONCE)
		{
			pTasks->SetTaskRecurrence(hTask, pEvent->nRegularity, pEvent->dwRegSpecific1, pEvent->dwRegSpecific2, TRUE, TDIRO_REUSE);
		}
		
		CStringArray aCats;
		int nCat = Misc::Split(pEvent->sCategories, aCats, ',');
		
		while (nCat--)
			pTasks->AddTaskCategory(hTask, aCats[nCat]);

		// prepend unknown attributes to comments
		if (m_aUnhandledTaskAttrib.GetSize())
		{
			CString sAttrib = Misc::FormatArray(m_aUnhandledTaskAttrib, '\n') + _T("\n\n");
			pTasks->SetTaskComments(hTask, (sAttrib + pEvent->sDescription));
		}
		else
		{
			pTasks->SetTaskComments(hTask, pEvent->sDescription);
		}

		// meta data in case there is a return trip
		CString sMetaUID;

		sMetaUID.Format(_T("%s_UID"), GetMetaID());
		pTasks->SetTaskMetaData(hTask, sMetaUID, pEvent->sUID);
			
		sMetaUID.Format(_T("%s_PUID"), GetMetaID());
		pTasks->SetTaskMetaData(hTask, sMetaUID, pEvent->sUIDParent);
	}
	
	return (pTasks->GetTaskCount() > 0);
}

BOOL CiCalImporter::ExtractDate(const CString& sValue, COleDateTime& date, BOOL bEndDate)
{
	CString sDate(sValue), sTime;
	Misc::Split(sDate, sTime, 'T');

	// sanity checks
	ASSERT(sDate.GetLength() == 8); // YYYYMMDD
	ASSERT(sTime.IsEmpty() || (sTime.GetLength() == 6)); // HHMMSS
	
	if ((sDate.GetLength() != 8) || !(sTime.IsEmpty() || (sTime.GetLength() == 6)))
		return FALSE;

	int nYear, nMonth, nDay, nHour = 0, nMin = 0, nSec = 0;

#if _MSC_VER >= 1400
	int nRes = _stscanf_s(sDate, _T("%4d%2d%2d"), &nYear, &nMonth, &nDay);
#else
	int nRes = _stscanf(sDate, _T("%4d%2d%2d"), &nYear, &nMonth, &nDay);
#endif
	ASSERT(nRes == 3);

	if (nRes != 3)
		return FALSE;

	if (!sTime.IsEmpty())
	{
#if _MSC_VER >= 1400
		int nRes = _stscanf_s(sTime, _T("%2d%2d%2d"), &nHour, &nMin, &nSec);
#else
		int nRes = _stscanf(sTime, _T("%2d%2d%2d"), &nHour, &nMin, &nSec);
#endif
		ASSERT(nRes == 3);
		
		if (nRes != 3)
			return FALSE;
	}

	date.SetDateTime(nYear, nMonth, nDay, nHour, nMin, nSec);

	// if there was no time component and this is an 'end' date
	// then we subtract a day, because that's how ics represents 'end-of-day'
	if (bEndDate && CDateHelper::IsDateSet(date) && (nHour == 0) && (nMin == 0) && (nSec == 0))
		date.m_dt--;

	return CDateHelper::IsDateSet(date);
}

int CiCalImporter::PreProcessFileLines(CStringArray& aLines)
{
	int nNumLines = aLines.GetSize();

	for (int nLine = (nNumLines - 1); nLine > 0; nLine--)
	{
		// if the line begins with a space,
		// concatenate it to the previous line
		const CString& sLine = aLines[nLine];

		if (sLine.IsEmpty())
		{
			aLines.RemoveAt(nLine);
		}
		else if (sLine[0] == ' ')
		{
			aLines[nLine - 1] += sLine.Mid(1);
			aLines.RemoveAt(nLine);
		}
	}

	return aLines.GetSize();
}

BOOL CiCalImporter::DecodeRecurrence(const CString& sRecur, int& nRegularity, DWORD& dwSpecific1, DWORD& dwSpecific2)
{
	nRegularity = TDIR_ONCE;
	dwSpecific1 = dwSpecific2 = 0;

	CStringArray aParts;
	int nNumParts = Misc::Split(sRecur, aParts, ';');

	if (nNumParts < 2)
		return FALSE;

	// extract the frequency and params
	CString sFreq, sSpecKey1, sSpecVal1, sSpecKey2, sSpecVal2;

	for (int nPart = 0; nPart < nNumParts; nPart++)
	{
		CString sKey(aParts[nPart]), sVal;

		if (!Misc::Split(sKey, sVal, '=') || sVal.IsEmpty())
			return FALSE;

		switch (nPart)
		{
		case 0:	
			if (sKey.CompareNoCase(_T("FREQ")) != 0)
				return FALSE;

			sFreq = sVal;
			break;
			
		case 1:	
			sSpecKey1 = sKey;	
			sSpecVal1 = sVal;	
			break;

		case 2:	
			sSpecKey2 = sKey;	
			sSpecVal2 = sVal;	
			break;
		}
	}

	// handle each recurrence type
	if (sFreq.CompareNoCase(_T("DAILY")) == 0)
	{
		// case TDIR_DAY_EVERY: // TDIR_DAILY
		// case TDIR_DAY_RECREATEAFTERNDAYS:
		//	  sRecurrence.Format(_T("RRULE:FREQ=DAILY;INTERVAL=%lu"), dwSpecific1);

		nRegularity = TDIR_DAY_EVERY_NDAYS;
		dwSpecific1 = 1; // default = every day

		// optional second item is interval
		if (sSpecKey1.CompareNoCase(_T("INTERVAL")) == 0)
			dwSpecific1 = _ttoi(sSpecVal1);
	}
	else if (sFreq.CompareNoCase(_T("WEEKLY")) == 0)
	{
		// case TDIR_WEEK_RECREATEAFTERNWEEKS:
		//   sRecurrence.Format(_T("RRULE:FREQ=WEEKLY;INTERVAL=%lu"), dwSpecific1);
		// case TDIR_WEEK_EVERY: // TDIR_WEEKLY
		//   sRecurrence.Format(_T("RRULE:FREQ=WEEKLY;INTERVAL=%lu;BYDAY="), dwSpecific1);
		// 
		// 	 if (dwSpecific2 & DHW_SUNDAY)
		// 		sRecurrence += _T("SU,");
		// 
		// 	 if (dwSpecific2 & DHW_MONDAY)
		// 		sRecurrence += _T("MO,");
		// 
		// 	 if (dwSpecific2 & DHW_TUESDAY)
		// 		sRecurrence += _T("TU,");
		// 
		// 	 if (dwSpecific2 & DHW_WEDNESDAY)
		// 		sRecurrence += _T("WE,");
		// 
		// 	 if (dwSpecific2 & DHW_THURSDAY)
		// 		sRecurrence += _T("TH,");
		// 
		// 	 if (dwSpecific2 & DHW_FRIDAY)
		// 		sRecurrence += _T("FR,");
		// 
		// 	 if (dwSpecific2 & DHW_SATURDAY)
		// 		sRecurrence += _T("SA,");

		BOOL bHasInterval = (sSpecKey1.CompareNoCase(_T("INTERVAL")) == 0);

		CString sByDayKey = (bHasInterval ? sSpecKey2 : sSpecKey1);
		CString sByDayVal = (bHasInterval ? sSpecVal2 : sSpecVal1);

		if (sByDayKey.CompareNoCase(_T("BYDAY")) == 0)
		{
			// decode abbreviated days
			CStringArray aDays;

			if (Misc::Split(sByDayVal, aDays, ','))
			{
				if (Misc::Find(aDays, _T("SU"), FALSE, FALSE) != -1)
					dwSpecific2 |= DHW_SUNDAY;

				if (Misc::Find(aDays, _T("MO"), FALSE, FALSE) != -1)
					dwSpecific2 |= DHW_MONDAY;

				if (Misc::Find(aDays, _T("TU"), FALSE, FALSE) != -1)
					dwSpecific2 |= DHW_TUESDAY;

				if (Misc::Find(aDays, _T("WE"), FALSE, FALSE) != -1)
					dwSpecific2 |= DHW_WEDNESDAY;

				if (Misc::Find(aDays, _T("TH"), FALSE, FALSE) != -1)
					dwSpecific2 |= DHW_THURSDAY;

				if (Misc::Find(aDays, _T("FR"), FALSE, FALSE) != -1)
					dwSpecific2 |= DHW_FRIDAY;

				if (Misc::Find(aDays, _T("SA"), FALSE, FALSE) != -1)
					dwSpecific2 |= DHW_SATURDAY;
			}
		}

		if (bHasInterval && dwSpecific2)
		{
			nRegularity = TDIR_WEEK_SPECIFIC_DOWS_NWEEKS;
			dwSpecific1 = _ttoi(sSpecVal1);
		}
		else
		{
			nRegularity = TDIR_WEEK_RECREATEAFTERNWEEKS_DEP;
			dwSpecific1 = _ttoi(sSpecVal1);
		}
	}
	else if (sFreq.CompareNoCase(_T("MONTHLY")) == 0)
	{
		// case TDIR_MONTH_EVERY: // TDIR_MONTHLY
		//    sRecurrence.Format(_T("RRULE:FREQ=MONTHLY;INTERVAL=%lu;BYMONTHDAY=%lu"), dwSpecific1, dwSpecific2);
		// case TDIR_MONTH_RECREATEAFTERNMONTHS:
		//    sRecurrence.Format(_T("RRULE:FREQ=MONTHLY;INTERVAL=%lu"), dwSpecific1);
		// case TDIR_MONTH_SPECIFICDAY:
		//    sRecurrence.Format(_T("RRULE:FREQ=MONTHLY;INTERVAL=%lu;BYDAY=%s"), dwSpecific2, FormatDayOfMonth(dwSpecific1));

		if (sSpecKey1.CompareNoCase(_T("INTERVAL")) == 0)
		{
			if (sSpecKey2.CompareNoCase(_T("BYMONTHDAY")) == 0)
			{
				nRegularity = TDIR_MONTH_SPECIFIC_DAY_NMONTHS;

				dwSpecific1 = _ttoi(sSpecVal1);
				dwSpecific2 = _ttoi(sSpecVal2);
			}
			else if (sSpecKey2.CompareNoCase(_T("BYDAY")) == 0)
			{
				nRegularity = TDIR_MONTH_SPECIFIC_DOW_NMONTHS;

				dwSpecific2 = _ttoi(sSpecVal1);
				dwSpecific1 = DecodeDayOfMonth(sSpecVal2);
			}
			else
			{
				nRegularity = TDIR_MONTH_RECREATEAFTERNMONTHS_DEP;
				dwSpecific1 = _ttoi(sSpecVal1);
			}
		}
		else
		{
			ASSERT(0);
			return FALSE;
		}
	}
	else if (sFreq.CompareNoCase(_T("YEARLY")) == 0)
	{
		// case TDIR_YEAR_EVERY: // TDIR_YEARLY
		//    sRecurrence.Format(_T("RRULE:FREQ=YEARLY;BYMONTH=%lu"), dwSpecific1);
		// case TDIR_YEAR_RECREATEAFTERNYEARS:
		//    sRecurrence.Format(_T("RRULE:FREQ=YEARLY;INTERVAL=%lu"), dwSpecific1);
		// case TDIR_YEAR_SPECIFICDAYMONTH:
		//    sRecurrence.Format(_T("RRULE:FREQ=YEARLY;BYMONTH=%lu;BYDAY=%s"), dwSpecific2, FormatDayOfMonth(dwSpecific1));


		if (sSpecKey1.CompareNoCase(_T("BYMONTH")) == 0)
		{
			// default
			nRegularity = TDIR_YEAR_SPECIFIC_DAY_MONTH;
			dwSpecific1 = _ttoi(sSpecVal1);

			if (sSpecKey2.CompareNoCase(_T("BYDAY")) == 0)
			{
				nRegularity = TDIR_YEAR_SPECIFIC_DOW_MONTH;

				dwSpecific2 = _ttoi(sSpecVal1);
				dwSpecific1 = DecodeDayOfMonth(sSpecVal2);
			}
		}
		else
		{
			nRegularity = TDIR_YEAR_RECREATEAFTERNYEARS_DEP;
			dwSpecific1 = 1;

			if (sSpecKey1.CompareNoCase(_T("INTERVAL")) == 0)
				dwSpecific1 = _ttoi(sSpecVal1);
		}
	}
	else // unknown
	{
		ASSERT(0);
		return FALSE;
	}

	return TRUE;
}

DWORD CiCalImporter::DecodeDayOfMonth(const CString& sDOM)
{
	if (sDOM.GetLength() == 3)
	{
		int nWhich = _ttoi(sDOM.Left(1)), nDOW = 0;
		CString sDOW = sDOM.Mid(1);
		
		if (sDOW.CompareNoCase(_T("SU")) == 0)
		{
			nDOW = 1;
		}
		else if (sDOW.CompareNoCase(_T("MO")) == 0)
		{
			nDOW = 2;
		}
		else if (sDOW.CompareNoCase(_T("TU")) == 0)
		{
			nDOW = 3;
		}
		else if (sDOW.CompareNoCase(_T("WE")) == 0)
		{
			nDOW = 4;
		}
		else if (sDOW.CompareNoCase(_T("TH")) == 0)
		{
			nDOW = 5;
		}
		else if (sDOW.CompareNoCase(_T("FR")) == 0)
		{
			nDOW = 6;
		}
		else if (sDOW.CompareNoCase(_T("SA")) == 0)
		{
			nDOW = 7;
		}
		
		ASSERT(nDOW);
		return (nDOW ? MAKELONG(nWhich, nDOW) : 0);
	}

	return 0;
}

ICI_LINETYPE CiCalImporter::ExtractInfo(const CString& sLine, CString& sValue, ICI_STATE nState)
{
	CString sKey(sLine);
	sKey.TrimLeft();
	sKey.TrimRight();

	int nItem = NUM_LINETYPES;
		
	while (nItem--)
	{
		if (sLine.Find(LINETYPES[nItem].szKey, 0) == 0)
		{
			ICI_LINETYPE nType = LINETYPES[nItem].nType;

			switch (nType)
			{
				case ICILT_BEGIN_VCAL:
				case ICILT_END_VCAL:
				case ICILT_BEGIN_VTIMEZONE:
				case ICILT_END_VTIMEZONE:
				case ICILT_BEGIN_STANDARD:
				case ICILT_END_STANDARD:
				case ICILT_BEGIN_DAYLIGHT:
				case ICILT_END_DAYLIGHT:
				case ICILT_BEGIN_VEVENT:
				case ICILT_END_VEVENT:
				case ICILT_BEGIN_VTODO:
				case ICILT_END_VTODO:
				case ICILT_BEGIN_VALARM:
				case ICILT_END_VALARM:
					return nType;
					
				case ICILT_PRODID:
				case ICILT_VERSION:
					if (nState == ICIS_VCAL)
					{
						if (Misc::Split(sKey, sValue, ':')) // must have a value
							return nType;
					}
					break;

				case ICILT_DTSTART:
				case ICILT_DTDUE:
				case ICILT_DTEND:
				case ICILT_SUMMARY:
				case ICILT_STATUS:
				case ICILT_CATEGORIES:
				case ICILT_URL:
				case ICILT_ORGANIZER:
				case ICILT_ATTENDEE:
				case ICILT_UID:
				case ICILT_DESCRIPTION:
				case ICILT_RELATEDTOPARENT:
				case ICILT_PERCENT:
				case ICILT_PRIORITY:
				case ICILT_RECURRENCE:
					if ((nState == ICIS_VEVENT) || (nState == ICIS_VTODO))
					{
						if (Misc::Split(sKey, sValue, ':')) // must have a value
							return nType;
					}
					break;

				case ICILT_ORGANIZERCN:
					if ((nState == ICIS_VEVENT) || (nState == ICIS_VTODO))
					{
						if (Misc::Split(sKey, sValue, '=')) // must have a value
							return nType;
					}
					break;
			}
		}
	}

	// not found
//	ASSERT(0);
	return ICILT_NONE;
}

CiCalImporter::ICI_STATE CiCalImporter::UpdateState(ICI_LINETYPE nType, ICI_STATE nState)
{
	static BOOL bInVEvent = FALSE;

	switch (nType)
	{
	case ICILT_BEGIN_VCAL:
		if (nState == ICIS_BEGIN)
			return ICIS_VCAL;
		break;
		
	case ICILT_END_VCAL:
		if (nState == ICIS_VCAL) 
			return ICIS_END;
		break;
		
	case ICILT_BEGIN_VTIMEZONE:
		if (nState == ICIS_VCAL) 
			return ICIS_VTIMEZONE;
		break;
		
	case ICILT_END_VTIMEZONE:
		if (nState == ICIS_VTIMEZONE) 
			return ICIS_VCAL;
		break;
		
	case ICILT_BEGIN_STANDARD:
		if (nState == ICIS_VTIMEZONE) 
			return ICIS_STANDARD;
		break;
		
	case ICILT_END_STANDARD:
		if (nState == ICIS_STANDARD) 
			return ICIS_VTIMEZONE;
		break;
		
	case ICILT_BEGIN_DAYLIGHT:
		if (nState == ICIS_VTIMEZONE) 
			return ICIS_DAYLIGHT;
		break;
		
	case ICILT_END_DAYLIGHT:
		if (nState == ICIS_DAYLIGHT) 
			return ICIS_VTIMEZONE;
		break;
		
	case ICILT_BEGIN_VEVENT:
		if (nState == ICIS_VCAL) 
		{
			bInVEvent = TRUE;
			return ICIS_VEVENT;
		}
		break;
		
	case ICILT_END_VEVENT:
		if (nState == ICIS_VEVENT) 
		{
			bInVEvent = FALSE;
			return ICIS_VCAL;
		}
		break;
		
	case ICILT_BEGIN_VTODO:
		if (nState == ICIS_VCAL) 
		{
			bInVEvent = FALSE;
			return ICIS_VTODO;
		}
		break;
		
	case ICILT_END_VTODO:
		if (nState == ICIS_VTODO) 
			return ICIS_VCAL;
		break;
		
	case ICILT_BEGIN_VALARM:
		if (nState == ICIS_VEVENT || nState == ICIS_VTODO) 
			return ICIS_VALARM;
		break;
		
	case ICILT_END_VALARM:
		if (nState == ICIS_VALARM) 
			return (bInVEvent ? ICIS_VEVENT : ICIS_VTODO);
		break;
		
	case ICILT_PRODID:
	case ICILT_VERSION:
		if (nState == ICIS_VCAL)  
			return nState; // no change
		break;

	case ICILT_PERCENT:
	case ICILT_PRIORITY:
	case ICILT_DTDUE:
		if (nState == ICIS_VTODO)
			return nState; // no change
		// fall thru
		
	case ICILT_DTEND:
	case ICILT_DTSTART:
	case ICILT_SUMMARY:
	case ICILT_STATUS:
	case ICILT_CATEGORIES:
	case ICILT_URL:
	case ICILT_ORGANIZER:
	case ICILT_ORGANIZERCN:
	case ICILT_ATTENDEE:
	case ICILT_UID:
	case ICILT_DESCRIPTION:
	case ICILT_RELATEDTOPARENT:
	case ICILT_RECURRENCE:
		if ((nState == ICIS_VEVENT) || (nState == ICIS_VTODO))
			return nState; // no change
		break;
	}
	
	// all else
	ASSERT(0);
	return ICIS_ERROR;
}
