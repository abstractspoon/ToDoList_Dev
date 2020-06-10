// TaskListTxtExporter.cpp: implementation of the CTaskListTxtExporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TaskListTxtExporter.h"
#include "tdlrecurringtaskedit.h"

#include "..\shared\enstring.h"

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

CTaskListTxtExporter::CTaskListTxtExporter()
{
}

CTaskListTxtExporter::~CTaskListTxtExporter()
{
	
}

IIMPORTEXPORT_RESULT CTaskListTxtExporter::Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey)
{
	return CTaskListExporterBase::Export(pSrcTaskFile, szDestFilePath, bSilent, pPrefs, szKey);
}

IIMPORTEXPORT_RESULT CTaskListTxtExporter::Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey)
{
	return CTaskListExporterBase::Export(pSrcTaskFile, szDestFilePath, bSilent, pPrefs, szKey);
}

bool CTaskListTxtExporter::InitConsts(const ITASKLISTBASE* pTasks, LPCTSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey)
{
	// base consts
	if (!CTaskListExporterBase::InitConsts(pTasks, szDestFilePath, bSilent, pPrefs, szKey))
		return false;

	// then ours
	szKey = _T("Preferences");

	if (pPrefs->GetProfileInt(szKey, _T("UseSpaceIndents"), TRUE))
		INDENT = CString(' ', pPrefs->GetProfileInt(szKey, _T("TextIndent"), 2));
	else
		INDENT = '\t';
	
	if (pPrefs->GetProfileInt(szKey, _T("ExportSpaceForNotes"), FALSE))
	{
		TEXTNOTES.Empty();
		int nLine = pPrefs->GetProfileInt(szKey, _T("LineSpaces"), 8);
		
		if (nLine > 0)
		{
			while (nLine--)
				TEXTNOTES += "\n";
		}
	}

	return true;
}

CString CTaskListTxtExporter::FormatAttribute(TDC_ATTRIBUTE nAttrib, const CString& sAttribLabel, const CString& sValue) const 
{
	CString sFmtAttrib;

	if (!sValue.IsEmpty())
	{
		switch (nAttrib)
		{
		case TDCA_POSITION:
		case TDCA_TASKNAME:
			sFmtAttrib.Format(_T("%s "), sValue);
			break;

		// all else
		default:
			sFmtAttrib.Format(_T("(%s: %s) "), sAttribLabel, sValue);
			break;
		}
	}

	return sFmtAttrib;
}

CString CTaskListTxtExporter::FormatAttribute(const ITASKLISTBASE* pTasks, HTASKITEM hTask, int nDepth, 
											  TDC_ATTRIBUTE nAttrib, const CString& sAttribLabel) const
{
	// base processing
	CString sItem = CTaskListExporterBase::FormatAttribute(pTasks, hTask, nDepth, nAttrib, sAttribLabel);

	// extra processing
	switch (nAttrib)
	{
	case TDCA_COMMENTS:
	case TDCA_FILELINK:
		{
			// indent
			for (int nTab = 0; nTab < nDepth; nTab++)
				sItem = INDENT + sItem;
		}
		break;

	case TDCA_PARENTID:
		// ignore if not set
		if (pTasks->GetTaskParentID(hTask) == 0)
			sItem.Empty();
		break;
	}

    return sItem;
}

CString CTaskListTxtExporter::ExportTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, int nDepth) const
{
	CString sTask = CTaskListExporterBase::ExportTask(pTasks, hTask, nDepth);

	// indent
	if (nDepth > 0)
	{
		int nTab = (nDepth - 1);
		
		while (nTab--)
			sTask = INDENT + sTask;
	}

	return sTask;
}

CString CTaskListTxtExporter::FormatTitle(const ITASKLISTBASE* pTasks) const
{
	CString sTitle = pTasks->GetReportTitle();
	CString sDate = pTasks->GetReportDate();
	CString sHeader;

	if (!sTitle.IsEmpty())
	{
		sHeader.Format(_T("%s\n%s\n"), sTitle, sDate);
	}
	else if (!sDate.IsEmpty())
	{
		sHeader.Format(_T("%s\n"), sDate);
	}

	return sHeader;
}

