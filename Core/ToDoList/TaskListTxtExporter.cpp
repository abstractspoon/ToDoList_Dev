// TaskListTxtExporter.cpp: implementation of the CTaskListTxtExporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TaskListTxtExporter.h"
#include "tdlrecurringtaskedit.h"

#include "..\shared\enstring.h"
#include "..\shared\Misc.h"

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

IIMPORTEXPORT_RESULT CTaskListTxtExporter::Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey)
{
	return CTaskListExporterBase::Export(pSrcTaskFile, szDestFilePath, dwFlags, pPrefs, szKey);
}

IIMPORTEXPORT_RESULT CTaskListTxtExporter::Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey)
{
	return CTaskListExporterBase::Export(pSrcTaskFile, szDestFilePath, dwFlags, pPrefs, szKey);
}

bool CTaskListTxtExporter::InitConsts(const ITASKLISTBASE* pTasks, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey)
{
	// base consts
	if (!CTaskListExporterBase::InitConsts(pTasks, szDestFilePath, dwFlags, pPrefs, szKey))
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

CString CTaskListTxtExporter::FormatAttribute(TDC_ATTRIBUTE nAttribID, const CString& sAttribLabel, const CString& sValue) const 
{
	CString sFmtAttrib;

	if (!sValue.IsEmpty())
	{
		switch (nAttribID)
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
											  TDC_ATTRIBUTE nAttribID, const CString& sAttribLabel) const
{
	// base processing
	CString sItem = CTaskListExporterBase::FormatAttribute(pTasks, hTask, nDepth, nAttribID, sAttribLabel);

	// extra processing
	switch (nAttribID)
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

CString CTaskListTxtExporter::FormatTitle(const IMultiTaskList* pTasks) const
{
	CString sTitle = CTaskListExporterBase::FormatTitle(pTasks);
	CString sUnderline = CString('=', sTitle.GetLength());
	
	return (sTitle + '\n' + sUnderline + _T("\n\n"));
}

CString CTaskListTxtExporter::FormatTitle(const ITASKLISTBASE* pTasks, BOOL bWantDate) const
{
	CString sTitle = CTaskListExporterBase::FormatTitle(pTasks, bWantDate);
	CString sUnderline = CString('-', sTitle.GetLength());

	return (sTitle + '\n' + sUnderline + '\n');
}

