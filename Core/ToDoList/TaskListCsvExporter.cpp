// TaskListTxtExporter.cpp: implementation of the CTaskListCsvExporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TaskListcsvExporter.h"
#include "tdlrecurringtaskedit.h"
#include "tdcmapping.h"

#include "..\shared\enstring.h"
#include "..\shared\filemisc.h"
#include "..\Shared\DateHelper.h"

#include "..\3rdparty\stdiofileex.h"

#include "..\Interfaces\Preferences.h"
#include "..\Interfaces\TasklistSchemaDef.h"

#include <locale.h>

////////////////////////////////////////////////////////////////////// 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////// 
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaskListCsvExporter::CTaskListCsvExporter() 
	: 
	m_bExportingForExcel(FALSE), 
	m_bFirstHeader(TRUE), 
	m_nFirstAttribID(TDCA_NONE),
	ISODATES(FALSE)
{
}

CTaskListCsvExporter::~CTaskListCsvExporter()
{
	
}

IIMPORTEXPORT_RESULT CTaskListCsvExporter::Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey)
{
	m_bFirstHeader = TRUE;
	m_nFirstAttribID = TDCA_NONE;

	return CTaskListExporterBase::Export(pSrcTaskFile, szDestFilePath, dwFlags, pPrefs, szKey);
}

IIMPORTEXPORT_RESULT CTaskListCsvExporter::Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey)
{
	m_bFirstHeader = TRUE;
	m_nFirstAttribID = TDCA_NONE;

	return CTaskListExporterBase::Export(pSrcTaskFile, szDestFilePath, dwFlags, pPrefs, szKey);
}

IIMPORTEXPORT_RESULT CTaskListCsvExporter::ExportOutput(LPCTSTR szDestFilePath, const CString& sOutput) const
{
	if (!sOutput.IsEmpty() && m_bExportingForExcel)
	{
		if (!FileMisc::SaveFile(szDestFilePath, sOutput, SFEF_UTF16))
			return IIER_BADFILE;

		return IIER_SUCCESS;
	}

	// else default handling
	return CTaskListExporterBase::ExportOutput(szDestFilePath, sOutput);
}

CString CTaskListCsvExporter::ExportTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, int nDepth) const
{
	CString sTask = CTaskListExporterBase::ExportTask(pTasks, hTask, nDepth);

	// prevent double line feeds
	sTask.TrimRight(ENDL);

	return sTask;
}

bool CTaskListCsvExporter::InitConsts(const ITASKLISTBASE* pTasks, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey)
{
	// base consts
	if (!CTaskListExporterBase::InitConsts(pTasks, szDestFilePath, dwFlags, pPrefs, szKey))
		return false;

	ISODATES = pPrefs->GetProfileInt(_T("Preferences"), _T("DisplayDatesInISO"), FALSE);
	
	// we read direct from app preferences
	szKey = _T("Preferences");

	CTDLCsvImportExportDlg dialog(szDestFilePath, ARRATTRIBUTES, pPrefs, szKey);
	
	BOOL bSilent = ((dwFlags & IIEF_SILENT) != 0);

	if (!bSilent)
	{
		if (dialog.DoModal() != IDOK)
			return false;
	}
	
	VERIFY(dialog.GetColumnMapping(m_aColumnMapping));
	DELIM = dialog.GetDelimiter();

	if (pPrefs->GetProfileInt(szKey, _T("UseSpaceIndents"), TRUE))
		INDENT = CString(' ', pPrefs->GetProfileInt(szKey, _T("TextIndent"), 2));
	else
		INDENT = "  "; // don't use tabs - excel strips them off

	m_bExportingForExcel = dialog.IsExportingForExcel();

	CheckAddIDFields();

	return true;
}

void CTaskListCsvExporter::CheckAddIDFields()
{
	// if Task and Parent IDs are wanted, we may have to add them manually
	// Note: Do parent ID first so that task ID is inserted before it
	if ((m_aColumnMapping.Find(TDCA_PARENTID) != -1) && !WantAttribute(TDCA_PARENTID))
		ARRATTRIBUTES.InsertAt(0, TDCA_PARENTID);
	
	if ((m_aColumnMapping.Find(TDCA_ID) != -1) && !WantAttribute(TDCA_ID))
		ARRATTRIBUTES.InsertAt(0, TDCA_ID);
}

CString CTaskListCsvExporter::FormatHeader(const ITASKLISTBASE* pTasks) const
{
	if (!m_bFirstHeader)
		return _T("");

	// else
	m_bFirstHeader = FALSE;
	m_nFirstAttribID = TDCA_NONE;

	CString sHeader = CTaskListExporterBase::FormatHeader(pTasks);

	// remove trailing delimiter
	sHeader.TrimRight(DELIM);

	return sHeader;
}

CString CTaskListCsvExporter::FormatHeaderItem(TDC_ATTRIBUTE nAttribID, const CString& sAttribLabel) const
{
	CString sHeader;

	if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
	{
		sHeader = (sAttribLabel + DELIM);
	}
	else
	{
		int nMap = m_aColumnMapping.Find(nAttribID);
		ASSERT(nMap != -1);

		if (nMap == -1)
			sHeader = (sAttribLabel + DELIM);
		else
			sHeader = (m_aColumnMapping[nMap].sColumnName + DELIM);
	}
	
	// If we're a multi-file export we need to prefix 
	// the header with 'Tasklist' as the first column
	if (MULTIFILE && (m_nFirstAttribID == TDCA_NONE))
	{
		m_nFirstAttribID = nAttribID; // we'll need this for the 'value' later
		sHeader = (CEnString(IDS_TDLBC_PROJECTNAME) + DELIM + sHeader);
	}
	
	return sHeader;
}

CString CTaskListCsvExporter::FormatAttribute(TDC_ATTRIBUTE /*nAttribID*/, const CString& /*sAttribLabel*/, const CString& sValue) const
{
	// Note: We always export values even if they are empty
	CString sAttrib(sValue);

	// Quote if the value contains embedded quotes or embedded delimiters
	BOOL bNeedQuoting = ((sAttrib.Find('\"') != -1) || (sAttrib.Find(DELIM) != -1));

	if (bNeedQuoting)
		Misc::MakeQuoted(sAttrib, '\"'); // double-up embedded quotes
	
	// Always replace carriage returns
	sAttrib.Replace('\n', ' ');
	sAttrib += DELIM;

	return sAttrib;
}

CString CTaskListCsvExporter::FormatAttribute(const ITASKLISTBASE* pTasks, HTASKITEM hTask, int nDepth, 
											  TDC_ATTRIBUTE nAttribID, const CString& sAttribLabel) const
{
	// base processing
	CString sItem = CTaskListExporterBase::FormatAttribute(pTasks, hTask, nDepth, nAttribID, sAttribLabel);

	// extra processing
	if (!sItem.IsEmpty())
	{
		switch (nAttribID)
		{
		case TDCA_POSITION:
		case TDCA_TASKNAME:
			if ((nAttribID == TDCA_POSITION) || !pTasks->IsAttributeAvailable(TDCA_POSITION))
			{
				CString sIndent;

				// indent task title
				while (--nDepth)
					sIndent += INDENT;
				
				// if the first character is a double-quote, insert
				// the spaces after that else Excel gets confused
				if (sItem[0] == '\"')
					sItem.Insert(1, sIndent);
				else
					sItem.Insert(0, sIndent);
			}
			break;
			
		case TDCA_TIMEESTIMATE:
		case TDCA_TIMESPENT:
			{
				TDC_UNITS nUnits;
				double dTime = (nAttribID == TDCA_TIMEESTIMATE) ?
								pTasks->GetTaskTimeEstimate(hTask, nUnits, TRUE) :
								pTasks->GetTaskTimeSpent(hTask, nUnits, TRUE);
				
				CString sTime = CTimeHelper::FormatTime(dTime, ROUNDTIMEFRACTIONS ? 0 : 2);
				CString sUnits = CTimeHelper::GetUnits(TDC::MapUnitsToTHUnits(nUnits));

				sItem = FormatAttribute(nAttribID, sAttribLabel, (sTime + sUnits));
			}
			break;

		// The app omits seconds when exporting date-times but 
		// in case this exported file is re-imported we don't
		// want to lose the granularity
		case TDCA_CREATIONDATE:
			{
				time64_t timeT = 0;
			
				if (pTasks->GetTaskCreationDate64(hTask, timeT))
					sItem = FormatAttribute(nAttribID, sAttribLabel, FormatDateWithSeconds(timeT));
			}
			break;

		case TDCA_LASTMODDATE:
			{
				time64_t timeT = 0;
			
				if (pTasks->GetTaskLastModified64(hTask, timeT))
					sItem = FormatAttribute(nAttribID, sAttribLabel, FormatDateWithSeconds(timeT));
			}
			break;

		case TDCA_DONEDATE:
			{
				time64_t timeT = 0;
			
				if (pTasks->GetTaskDoneDate64(hTask, timeT))
					sItem = FormatAttribute(nAttribID, sAttribLabel, FormatDateWithSeconds(timeT));
			}
			break;
		}

		// If we're a multi-file export and this is the first attribute 
		// we need to prefix the value with 'Tasklist' as the first column
		if (MULTIFILE && (nAttribID == m_nFirstAttribID))
		{
			sItem = (CTaskListExporterBase::FormatTitle(pTasks, FALSE) + DELIM + sItem);
		}
	}

    return sItem;
}

CString CTaskListCsvExporter::FormatDateWithSeconds(time64_t timeT) const
{
	ASSERT(timeT != 0);

	COleDateTime date = CDateHelper::GetDate(timeT);
	DWORD dwFlags = (ISODATES ? DHFD_ISO : 0);
	
	if (CDateHelper::DateHasTime(date))
		dwFlags |= DHFD_TIME;
	
	return CDateHelper::FormatDate(date, dwFlags);
}
