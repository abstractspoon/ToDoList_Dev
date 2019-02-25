// TaskListTxtExporter.cpp: implementation of the CTaskListCsvExporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TaskListcsvExporter.h"
#include "tdlrecurringtaskedit.h"

#include "..\shared\enstring.h"
#include "..\shared\filemisc.h"

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

static LPCTSTR ONEDBLQUOTE = _T("\"");
static LPCTSTR TWODBLQUOTE = _T("\"\"");

CTaskListCsvExporter::CTaskListCsvExporter() : m_bExportingForExcel(FALSE), m_bFirstHeader(TRUE)
{
}

CTaskListCsvExporter::~CTaskListCsvExporter()
{
	
}

IIMPORTEXPORT_RESULT CTaskListCsvExporter::Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey)
{
	m_bFirstHeader = TRUE;

	return CTaskListExporterBase::Export(pSrcTaskFile, szDestFilePath, bSilent, pPrefs, szKey);
}

IIMPORTEXPORT_RESULT CTaskListCsvExporter::Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey)
{
	m_bFirstHeader = TRUE;

	return CTaskListExporterBase::Export(pSrcTaskFile, szDestFilePath, bSilent, pPrefs, szKey);
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

bool CTaskListCsvExporter::InitConsts(const ITASKLISTBASE* pTasks, LPCTSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey)
{
	// base consts
	if (!CTaskListExporterBase::InitConsts(pTasks, szDestFilePath, bSilent, pPrefs, szKey))
		return false;
	
	// Add project identifier if exporting multiple files
	if (MULTIFILE)
	{
		ARRATTRIBUTES.InsertAt(0, TDCA_PROJECTNAME);
		ARRLABELS.Add(GetAttribLabel(TDCA_PROJECTNAME));
	}
	
	// we read direct from app preferences
	szKey = _T("Preferences");

	CTDLCsvImportExportDlg dialog(szDestFilePath, ARRATTRIBUTES, pPrefs, szKey);
	
	if (!bSilent && dialog.DoModal() != IDOK)
		return false;
	
	VERIFY(dialog.GetColumnMapping(m_aColumnMapping));
	DELIM = dialog.GetDelimiter();

	if (pPrefs->GetProfileInt(szKey, _T("UseSpaceIndents"), TRUE))
		INDENT = CString(' ', pPrefs->GetProfileInt(szKey, _T("TextIndent"), 2));
	else
		INDENT = "  "; // don't use tabs - excel strips them off

	m_bExportingForExcel = dialog.IsExportingForExcel();

	// if Task and Parent IDs are wanted, we may have to add them manually
	// Note: Do parent ID first so that task ID is inserted before it
	CheckAddIDField(TDCA_PARENTID);
	CheckAddIDField(TDCA_ID);
	
	return true;
}

void CTaskListCsvExporter::CheckAddIDField(TDC_ATTRIBUTE nAttrib)
{
	// sanity check
	switch (nAttrib)
	{
	case TDCA_PARENTID:
	case TDCA_ID:
	case TDCA_PROJECTNAME:
		break;

	default:
		ASSERT(0);
		return;
	}

	if ((m_aColumnMapping.Find(nAttrib) != -1) && (FindAttribute(nAttrib) == -1))
	{
		ARRATTRIBUTES.InsertAt(0, nAttrib);

		// translate label once only
		CEnString sLabel(GetAttribLabel(nAttrib));
		sLabel.Translate();
		
		ARRLABELS.InsertAt(0, sLabel);
	}
}

CString CTaskListCsvExporter::FormatHeader(const ITASKLISTBASE* pTasks) const
{
	if (!m_bFirstHeader)
		return _T("");

	// else
	m_bFirstHeader = FALSE;
	CString sHeader = CTaskListExporterBase::FormatHeader(pTasks);

	// remove trailing delimiter
	sHeader.TrimRight(DELIM);

	return sHeader;
}

CString CTaskListCsvExporter::FormatHeaderItem(TDC_ATTRIBUTE nAttrib, const CString& sAttribLabel) const
{
	CString sHeader;
	
	if (IsCustomAttribute(nAttrib))
	{
		sHeader = (sAttribLabel + DELIM);
	}
	else
	{
		int nMap = m_aColumnMapping.Find(nAttrib);
		ASSERT(nMap != -1);

		if (nMap == -1)
			sHeader = (sAttribLabel + DELIM);
		else
			sHeader = (m_aColumnMapping[nMap].sColumnName + DELIM);
	}
	
	return sHeader;
}

CString CTaskListCsvExporter::FormatAttribute(TDC_ATTRIBUTE nAttrib, const CString& /*sAttribLabel*/, const CString& sValue) const
{
	// Note: We always export values even if they are empty

	// Note: We always quote comments to avoid unnecessary search/replace
	BOOL bNeedQuoting = (nAttrib == TDCA_COMMENTS);
	CString sAttrib(sValue);
	
	// double up quotes
	if (sAttrib.Find(ONEDBLQUOTE) != -1)
	{
		sAttrib.Replace(ONEDBLQUOTE, TWODBLQUOTE);
		bNeedQuoting = TRUE;
	}
	
	// look for commas or whatever is the list delimiter
	if (sAttrib.Find(DELIM) != -1)
		bNeedQuoting = TRUE;
	
	if (bNeedQuoting)
		sAttrib = ONEDBLQUOTE + sAttrib + ONEDBLQUOTE;
	
	// replace carriage returns
	sAttrib.Replace('\n', ' ');
	sAttrib += DELIM;

	return sAttrib;
}

CString CTaskListCsvExporter::FormatAttribute(const ITASKLISTBASE* pTasks, HTASKITEM hTask, int nDepth, 
											  TDC_ATTRIBUTE nAttrib, const CString& sAttribLabel) const
{
	// base processing
	CString sItem = CTaskListExporterBase::FormatAttribute(pTasks, hTask, nDepth, nAttrib, sAttribLabel);

	// extra processing
	if (!sItem.IsEmpty())
	{
		switch (nAttrib)
		{
		case TDCA_POSITION:
		case TDCA_TASKNAME:
			if ((nAttrib == TDCA_POSITION) || !WANTPOS)
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
			
		case TDCA_TIMEEST:
		case TDCA_TIMESPENT:
			{
				TDC_UNITS nUnits;
				double dTime = (nAttrib == TDCA_TIMEEST) ?
								pTasks->GetTaskTimeEstimate(hTask, nUnits, TRUE) :
								pTasks->GetTaskTimeSpent(hTask, nUnits, TRUE);
				
				CString sTime = CTimeHelper().FormatTime(dTime, ROUNDTIMEFRACTIONS ? 0 : 2);
				CString sUnits = CTimeHelper().GetUnits(TDC::MapUnitsToTHUnits(nUnits));

				sItem = FormatAttribute(nAttrib, sAttribLabel, (sTime + sUnits));
			}
			break;
		}
	}

    return sItem;
}

