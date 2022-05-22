// iCalExporter.cpp: implementation of the CiCalExporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "iCalImportExport.h"
#include "iCalExporter.h"
#include "iCalExporterOptionsDlg.h"

#include "..\Shared\DateHelper.h"
#include "..\Shared\misc.h"

#include "..\3rdParty\stdiofileex.h"

#include "..\Interfaces\IPreferences.h"

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CiCalExporter::CiCalExporter() : EXPORTFORMAT(ICEA_APPT), NODUEDATEISTODAYORSTART(FALSE)
{
	m_icon.Load(IDI_ICALENDAR);
}

CiCalExporter::~CiCalExporter()
{
}

void CiCalExporter::SetLocalizer(ITransText* /*pTT*/)
{
	//CLocalizer::Initialize(pTT);
}

void CiCalExporter::WriteHeader(CStdioFileEx& fileOut)
{
	WriteString(fileOut, _T("BEGIN:VCALENDAR"));
	WriteString(fileOut, _T("PRODID:iCalExporter (c) AbstractSpoon 2009-22"));
	WriteString(fileOut, _T("VERSION:2.0"));
}

IIMPORTEXPORT_RESULT CiCalExporter::Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey)
{
	const ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pSrcTaskFile, IID_TASKLISTBASE);

	if (pTasks == NULL)
	{
		ASSERT(0);
		return IIER_BADINTERFACE;
	}

	if (!InitConsts(dwFlags, pPrefs, szKey))
		return IIER_CANCELLED;

	CStdioFileEx fileOut;
	
	if (!fileOut.Open(szDestFilePath, CFile::modeCreate | CFile::modeWrite, SFEF_UTF8WITHOUTBOM))
		return IIER_BADFILE;

	// header
	WriteHeader(fileOut);
		
	// export first task only and the rest will follow
	int nNumExported = ExportTask(pTasks, pTasks->GetFirstTask(), _T(""), fileOut, TRUE);

	// footer
	WriteString(fileOut, _T("END:VCALENDAR"));

	if (nNumExported != pTasks->GetTaskCount())
		return IIER_SOMEFAILED;
		
	return IIER_SUCCESS;
}

IIMPORTEXPORT_RESULT CiCalExporter::Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey)
{
	if (!InitConsts(dwFlags, pPrefs, szKey))
		return IIER_CANCELLED;
	
	CStdioFileEx fileOut;
	
	if (!fileOut.Open(szDestFilePath, CFile::modeCreate | CFile::modeWrite, SFEF_UTF8WITHOUTBOM))
		return IIER_BADFILE;

	// header
	WriteHeader(fileOut);

	int nNumTasks = 0, nNumExported = 0;

	for (int nTaskList = 0; nTaskList < pSrcTaskFile->GetTaskListCount(); nTaskList++)
	{
		const ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pSrcTaskFile->GetTaskList(nTaskList), IID_TASKLISTBASE);

		// export first task only and the rest will follow
		if (pTasks == NULL)
		{
			ASSERT(0);
			return IIER_BADINTERFACE;
		}

		nNumTasks += pTasks->GetTaskCount();
		nNumExported += ExportTask(pTasks, pTasks->GetFirstTask(), _T(""), fileOut, TRUE);
	}

	// footer
	WriteString(fileOut, _T("END:VCALENDAR"));

	if (nNumExported != nNumTasks)
		return IIER_SOMEFAILED;

	return IIER_SUCCESS;
}

bool CiCalExporter::InitConsts(DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	NODUEDATEISTODAYORSTART = pPrefs->GetProfileInt(_T("Preferences"), _T("NoDueDateIsDueToday"), FALSE);

	CString sKey(szKey);
	sKey += _T("\\iCalExporter");

	EXPORTFORMAT = (ICALEXPORTAS)pPrefs->GetProfileInt(szKey, _T("ExportFormat"), ICEA_APPT);

	BOOL bSilent = ((dwFlags & IIEF_SILENT) != 0);

	if (!bSilent)
	{
		CiCalExporterOptionsDlg dlg(EXPORTFORMAT);

		if (dlg.DoModal() != IDOK)
			return false;

		EXPORTFORMAT = dlg.GetTaskExportFormat();

		pPrefs->WriteProfileInt(szKey, _T("ExportFormat"), EXPORTFORMAT);
	}

	return true;
}

CString CiCalExporter::FormatUID(LPCTSTR szFileName, DWORD dwTaskID)
{
	CString sUID, sFile(szFileName);
    
	sFile.Replace(_T("\\"), _T(""));
    sFile.Replace(_T(":"), _T(""));
    sFile.Replace(_T(" "), _T(""));

	sUID.Format(_T("%lu@%s.com"), dwTaskID, sFile);

	return sUID;
}

BOOL CiCalExporter::GetTaskDates(const ITASKLISTBASE* pTasks, HTASKITEM hTask, 
								COleDateTime& dtStart, COleDateTime& dtEnd, COleDateTime& dtDue) const
{
	// Neither Google Calendar not Outlook pay any attention to the 'DUE',
	// so we have to be a bit clever in using the 'END' tag both for 
	// due date and completion.
	time64_t tStart = 0, tDue = 0, tDone = 0;

	if (pTasks->GetTaskStartDate64(hTask, FALSE, tStart))
		dtStart = CDateHelper::GetDate(tStart);
	else
		CDateHelper::ClearDate(dtStart);

	if (pTasks->GetTaskDueDate64(hTask, FALSE, tDue))
		dtDue = CDateHelper::GetDate(tDue);
	else
		CDateHelper::ClearDate(dtDue);

	COleDateTime dtDone;

	if (pTasks->GetTaskDoneDate64(hTask, tDone))
		dtDone = CDateHelper::GetDate(tDone);
	else
		CDateHelper::ClearDate(dtDone);

	if (pTasks->GetTaskPercentDone(hTask, FALSE) < 100)
		dtEnd = dtDue;
	else
		dtEnd = dtDone;

	// if task only has a start date then make the end date the same as the start and vice versa
	BOOL bHasStart = CDateHelper::IsDateSet(dtStart), bHasEnd = CDateHelper::IsDateSet(dtEnd);

	if (bHasStart && !bHasEnd)
	{
		if (NODUEDATEISTODAYORSTART)
		{
			COleDateTime dtToday = CDateHelper::GetDate(DHD_TODAY);

			if (dtToday > dtStart)
				dtEnd = dtStart;
			else
				dtEnd = dtToday;
		}
	}
	else if (!bHasStart && bHasEnd)
	{
		dtStart = dtEnd;
	}

	return (bHasStart || bHasEnd);
}

int CiCalExporter::ExportTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, const CString& sParentUID, 
								CStdioFile& fileOut, BOOL bAndSiblings)
{
	if (!hTask)
		return 0;

	int nNumExported = 0;

	// construct a unique ID
	CString sUID = FormatUID(fileOut.GetFilePath(), pTasks->GetTaskID(hTask));
		
	// tasks must have a start date or a due date or both
	COleDateTime dtStart, dtDue, dtEnd;

	if (GetTaskDates(pTasks, hTask, dtStart, dtEnd, dtDue))
	{
		// header
		if (EXPORTFORMAT == ICEA_TODO)
			WriteString(fileOut, _T("BEGIN:VTODO"));
		else
			WriteString(fileOut, _T("BEGIN:VEVENT"));
		
		WriteString(fileOut, FormatDateTime(_T("DTSTART"), dtStart, TRUE));

		if (CDateHelper::IsDateSet(dtEnd))
			WriteString(fileOut, FormatDateTime(_T("DTEND"), dtEnd, FALSE));

		// write the due date always for later importing
		if (CDateHelper::IsDateSet(dtDue))
			WriteString(fileOut, FormatDateTime(_T("DUE"), dtDue, FALSE));
		
		WriteString(fileOut, _T("SUMMARY:%s"), pTasks->GetTaskTitle(hTask));
		WriteString(fileOut, _T("STATUS:%s"), pTasks->GetTaskStatus(hTask));
		WriteString(fileOut, _T("UID:%s"), sUID);
		WriteString(fileOut, _T("PERCENT:%lu"), pTasks->GetTaskPercentDone(hTask, FALSE));

		if (EXPORTFORMAT == ICEA_TODO)
		{
			WriteString(fileOut, _T("DELEGATED-TO:%s"), pTasks->GetTaskAllocatedTo(hTask, 0));
			WriteString(fileOut, _T("DELEGATED-FROM:%s"), pTasks->GetTaskAllocatedBy(hTask));
		}
		else if (EXPORTFORMAT == ICEA_EVENT)
		{
			WriteString(fileOut, _T("ATTENDEE:%s"), pTasks->GetTaskAllocatedTo(hTask, 0));
		}
		// else appointment

		// encode file link into ORGANIZER if it is an email address
		if (EXPORTFORMAT != ICEA_APPT)
		{
			CString sUrl(pTasks->GetTaskFileLinkPath(hTask));

			if (!sUrl.IsEmpty())
			{
				sUrl.MakeLower();

				if (!EXPORTFORMAT && (sUrl.Find(_T("mailto:")) == 0))
					WriteString(fileOut, _T("ORGANIZER;CN=%s:%s"), pTasks->GetTaskAllocatedBy(hTask), sUrl);
				else
					WriteString(fileOut, _T("URL:%s"), sUrl);
			}
		}

		// don't export our 'special' priorities
		int nPriority = pTasks->GetTaskPriority(hTask, TRUE);

		if (nPriority >= 0)
			WriteString(fileOut, _T("PRIORITY:%lu"), nPriority);

		// categories
		CStringArray aCats;
		int nCat = pTasks->GetTaskCategoryCount(hTask);

		while (nCat--)
			aCats.Add(pTasks->GetTaskCategory(hTask, nCat));

		WriteString(fileOut, _T("CATEGORIES:%s"), Misc::FormatArray(aCats, _T(",")));

		// comments
		CString sComments = pTasks->GetTaskComments(hTask);

		sComments.Replace(_T("\r\n"), _T(" "));
		sComments.Replace(_T("\n"), _T(" "));

		WriteString(fileOut, _T("DESCRIPTION:%s"), sComments);

		// recurrence
		TDC_REGULARITY nRegularity;
		int nUnused;
		TDC_RECURREUSEOPTION nUnused2;
		TDC_RECURFROMOPTION nUnused3;
		DWORD dwSpecific1, dwSpecific2;
		bool bUnused;

		if (pTasks->GetTaskRecurrence(hTask, nRegularity, dwSpecific1, dwSpecific2, 
										nUnused3, nUnused2, nUnused, nUnused, bUnused))
		{
			WriteString(fileOut, FormatRecurrence(nRegularity, dwSpecific1, dwSpecific2));
		}

		// parent child relationship
		WriteString(fileOut, _T("RELATED-TO;RELTYPE=PARENT:%s"), sParentUID);
		
		// footer
		if (EXPORTFORMAT == ICEA_TODO)
			WriteString(fileOut, _T("END:VTODO"));
		else
			WriteString(fileOut, _T("END:VEVENT"));

		nNumExported++;
	}
	
	// copy across first child
	nNumExported += ExportTask(pTasks, pTasks->GetFirstTask(hTask), sUID, fileOut, TRUE);
	
	// copy sibling tasks WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == don't recurse on siblings
			nNumExported += ExportTask(pTasks, hSibling, sParentUID, fileOut, FALSE);
			hSibling = pTasks->GetNextTask(hSibling);
		}
	}

	return nNumExported;
}

CString CiCalExporter::FormatRecurrence(int nRegularity, DWORD dwSpecific1, DWORD dwSpecific2)
{
	CString sRecurrence;

	switch (nRegularity)
	{
	case TDIR_DAY_EVERY_NDAYS: // TDIR_DAILY
		sRecurrence.Format(_T("RRULE:FREQ=DAILY;INTERVAL=%lu"), dwSpecific1);
		break;

	case TDIR_DAY_EVERY_NWEEKDAYS: // TDIR_DAILY
		sRecurrence.Format(_T("RRULE:FREQ=WEEKLY;BYDAY=MO,TU,WE,TH,FR;INTERVAL=%lu"));
		break;

	case TDIR_DAY_EVERY_WEEKDAY:
		sRecurrence.Format(_T("RRULE:FREQ=WEEKLY;BYDAY=MO,TU,WE,TH,FR"));
		break;

	// -----------------------------------------------
	
	case TDIR_WEEK_SPECIFIC_DOWS_NWEEKS: // TDIR_WEEKLY
		{
			sRecurrence.Format(_T("RRULE:FREQ=WEEKLY;INTERVAL=%lu;BYDAY="), dwSpecific1);

			if (dwSpecific2 & DHW_SUNDAY)
				sRecurrence += _T("SU,");
					
			if (dwSpecific2 & DHW_MONDAY)
				sRecurrence += _T("MO,");

			if (dwSpecific2 & DHW_TUESDAY)
				sRecurrence += _T("TU,");

			if (dwSpecific2 & DHW_WEDNESDAY)
				sRecurrence += _T("WE,");

			if (dwSpecific2 & DHW_THURSDAY)
				sRecurrence += _T("TH,");

			if (dwSpecific2 & DHW_FRIDAY)
				sRecurrence += _T("FR,");

			if (dwSpecific2 & DHW_SATURDAY)
				sRecurrence += _T("SA,");

			// strip off trailing comma
			sRecurrence.TrimRight(',');
		}
		break;

	case TDIR_WEEK_RECREATEAFTERNWEEKS_DEP:
		sRecurrence.Format(_T("RRULE:FREQ=WEEKLY;INTERVAL=%lu"), dwSpecific1);
		break;

	// -----------------------------------------------
	
	case TDIR_MONTH_SPECIFIC_DAY_NMONTHS: // TDIR_MONTHLY
		sRecurrence.Format(_T("RRULE:FREQ=MONTHLY;INTERVAL=%lu;BYMONTHDAY=%lu"), dwSpecific1, dwSpecific2);
		break;

	case TDIR_MONTH_RECREATEAFTERNMONTHS_DEP:
		sRecurrence.Format(_T("RRULE:FREQ=MONTHLY;INTERVAL=%lu"), dwSpecific1);
		break;
	
	case TDIR_MONTH_SPECIFIC_DOW_NMONTHS:
		sRecurrence.Format(_T("RRULE:FREQ=MONTHLY;INTERVAL=%lu;BYDAY=%s"), dwSpecific2, FormatDayOfMonth(dwSpecific1));
		break;
		
	case TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS:
		// TODO
		//sRecurrence.Format(_T("RRULE:FREQ=MONTHLY;INTERVAL=%lu;BYDAY=%s"), dwSpecific2, FormatDayOfMonth(dwSpecific1));
		break;

	// -----------------------------------------------
	
	case TDIR_YEAR_SPECIFIC_DAY_MONTH: // TDIR_YEARLY
		sRecurrence.Format(_T("RRULE:FREQ=YEARLY;BYMONTH=%lu"), dwSpecific1);
		break;
		
	case TDIR_YEAR_RECREATEAFTERNYEARS_DEP:
		sRecurrence.Format(_T("RRULE:FREQ=YEARLY;INTERVAL=%lu"), dwSpecific1);
		break;
	
	case TDIR_YEAR_SPECIFIC_DOW_MONTH:
		sRecurrence.Format(_T("RRULE:FREQ=YEARLY;BYMONTH=%lu;BYDAY=%s"), dwSpecific2, FormatDayOfMonth(dwSpecific1));
		break;
	}

	ASSERT(!sRecurrence.IsEmpty());
	return sRecurrence;
}

CString CiCalExporter::FormatDayOfMonth(DWORD dwDOM)
{
	CString sDOM;
	int nDOW = HIWORD(dwDOM);
	int nWhich = LOWORD(dwDOM);

	switch (nDOW)
	{
	case 1:
		sDOM.Format(_T("%luSU"), nWhich);
		break;
		
	case 2: 
		sDOM.Format(_T("%luMO"), nWhich);
		break;
		
	case 3: 
		sDOM.Format(_T("%luTU"), nWhich);
		break;
		
	case 4: 
		sDOM.Format(_T("%luWE"), nWhich);
		break;
		
	case 5: 
		sDOM.Format(_T("%luTH"), nWhich);
		break;
		
	case 6: 
		sDOM.Format(_T("%luFR"), nWhich);
		break;
		
	case 7: 
		sDOM.Format(_T("%luSA"), nWhich);
		break;

	default:
		ASSERT(0);
	}

	return sDOM;
}

CString CiCalExporter::FormatDateTime(LPCTSTR szType, const COleDateTime& date, BOOL bStartOfDay)
{
	CString sDateTime;

	if (CDateHelper::DateHasTime(date))
	{
		sDateTime.Format(_T("%s;VALUE=DATE-TIME:%04d%02d%02dT%02d%02d%02d"), szType, 
						date.GetYear(), date.GetMonth(), date.GetDay(),
						date.GetHour(), date.GetMinute(), date.GetSecond());
	}
	else // no time component
	{
		COleDateTime dtDay(date);
		
		if (!bStartOfDay)
			dtDay.m_dt += 1.0;
		
		sDateTime.Format(_T("%s;VALUE=DATE-TIME:%04d%02d%02dT000000"), szType, 
						dtDay.GetYear(), dtDay.GetMonth(), dtDay.GetDay());	
	}
	
	return sDateTime;
}

void __cdecl CiCalExporter::WriteString(CStdioFile& fileOut, LPCTSTR lpszFormat, ...)
{
	ASSERT(AfxIsValidString(lpszFormat));
	CString sLine;
	
	va_list argList;
	va_start(argList, lpszFormat);
	sLine.FormatV(lpszFormat, argList);
	va_end(argList);
	
	sLine.TrimRight();
	
	// write line out in pieces no longer than 75 bytes
	while (sLine.GetLength() > 75)
	{
		CString sTemp = sLine.Left(75);
		sLine = sLine.Mid(75);
		
		fileOut.WriteString(sTemp);
		fileOut.WriteString(_T("\r\n ")); // note space at beginning of next line
	}
	
	// write out whatever's left
	fileOut.WriteString(sLine);
	fileOut.WriteString(_T("\r\n"));
}
