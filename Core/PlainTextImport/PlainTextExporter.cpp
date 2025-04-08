// PlainTextExporter.cpp: implementation of the CPlainTextExporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PlainTextImport.h"
#include "PlainTextExporter.h"
#include "optionsdlg.h"

#include "..\shared\Misc.h"

#include "..\Interfaces\IPreferences.h"
#include "..\Interfaces\ITranstext.h"

#include "..\3rdParty\stdiofileex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const CString ENDL(_T("\r\n"));

CPlainTextExporter::CPlainTextExporter() : INDENT(_T("  ")), WANTPROJECT(FALSE)
{
}

CPlainTextExporter::~CPlainTextExporter()
{

}

void CPlainTextExporter::SetLocalizer(ITransText* /*pTT*/)
{
	//CLocalizer::Initialize(pTT);
}

BOOL CPlainTextExporter::InitConsts(DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString sKey(szKey);
	sKey += _T("\\PlainText");

	WANTPROJECT = pPrefs->GetProfileInt(szKey, _T("IncludeProject"), FALSE);
	INDENT = pPrefs->GetProfileString(szKey, _T("Indent"));

	if (INDENT.IsEmpty()) // first time
	{
		BOOL bSpaceIndent = pPrefs->GetProfileInt(_T("Preferences"), _T("UseSpaceIndents"), TRUE);
		int nSpaces = pPrefs->GetProfileInt(_T("Preferences"), _T("TextIndent"), 2);

		if (bSpaceIndent)
			INDENT = CString(' ', nSpaces);
		else
			INDENT = '\t';
	}

	BOOL bSilent = ((dwFlags & IIEF_SILENT) != 0);

	if (!bSilent)
	{
		COptionsDlg dlg(FALSE, WANTPROJECT, INDENT);

		if (dlg.DoModal() != IDOK)
			return FALSE;
		
		INDENT = dlg.GetIndent();
		WANTPROJECT = dlg.GetWantProject();

		pPrefs->WriteProfileInt(szKey, _T("IncludeProject"), WANTPROJECT);
		pPrefs->WriteProfileString(szKey, _T("Indent"), INDENT);
	}

	return TRUE;
}

IIMPORTEXPORT_RESULT CPlainTextExporter::Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey)
{
	if (!InitConsts(dwFlags, pPrefs, szKey))
		return IIER_CANCELLED;

	CStdioFileEx fileOut;

	if (!fileOut.Open(szDestFilePath, CFile::modeCreate | CFile::modeWrite, SFEF_UTF8WITHOUTBOM))
		return IIER_BADFILE;

	if (!ExportTasklist(pSrcTaskFile, fileOut, 0))
		return IIER_SOMEFAILED;
	
	return IIER_SUCCESS;
}

IIMPORTEXPORT_RESULT CPlainTextExporter::Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey)
{
	if (!InitConsts(dwFlags, pPrefs, szKey))
		return IIER_CANCELLED;

	CStdioFileEx fileOut;
	BOOL bSomeFailed = FALSE;

	if (!fileOut.Open(szDestFilePath, (CFile::modeCreate | CFile::modeWrite), SFEF_UTF8WITHOUTBOM))
		return IIER_BADFILE;

	if (WANTPROJECT)
	{
		fileOut.WriteString(FormatTitle(pSrcTaskFile->GetReportTitle(), pSrcTaskFile->GetReportDate()));
		fileOut.WriteString(ENDL);
	}

	for (int nTaskList = 0; nTaskList < pSrcTaskFile->GetTaskListCount(); nTaskList++)
	{ 
		if (!ExportTasklist(pSrcTaskFile->GetTaskList(nTaskList), fileOut, 1))
			bSomeFailed = TRUE;
	}
	
	return (bSomeFailed ? IIER_SOMEFAILED : IIER_SUCCESS);
}

BOOL CPlainTextExporter::ExportTasklist(const ITaskList* pSrcTaskFile, CStdioFile& fileOut, int nDepth)
{
	const ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pSrcTaskFile, IID_TASKLISTBASE);

	if (!pTasks)
		return FALSE;

	if (WANTPROJECT)
	{
		// note: we export the title even if it's empty to maintain consistency 
		// with the importer that the first line is always the outline name
		// Note: We don't export the date if we're part of a multi-file export
		if (nDepth == 0)
			fileOut.WriteString(FormatTitle(pTasks->GetReportTitle(), pTasks->GetReportDate()));
		else
			fileOut.WriteString(INDENT + FormatTitle(pTasks->GetReportTitle(), pTasks->GetReportDate(), FALSE));

		fileOut.WriteString(ENDL);
	}

	// export first task indented from the project name
	ExportTask(pTasks, pTasks->GetFirstTask(), fileOut, (nDepth + 1), TRUE);

	// add blank line before next tasklist
	fileOut.WriteString(ENDL);
	return TRUE;
}

CString CPlainTextExporter::FormatTitle(LPCTSTR szReportTitle, LPCTSTR szReportDate, BOOL bWantDate)
{
	if (!bWantDate || Misc::IsEmpty(szReportDate))
		return szReportTitle;

	if (Misc::IsEmpty(szReportTitle))
		return szReportDate;

	return Misc::Format(_T("%s (%s)"), szReportTitle, szReportDate);
}

void CPlainTextExporter::ExportTask(const ITASKLISTBASE* pSrcTaskFile, HTASKITEM hTask,
									CStdioFile& fileOut, int nDepth, BOOL bAndSiblings)
{
	if (!hTask)
		return;

	// export each task as '[indent]title|comments' on a single line
	CString sTask;

	// indent
	for (int nTab = 0; nTab < nDepth; nTab++)
		sTask += INDENT;

	// title
	sTask += pSrcTaskFile->GetTaskTitle(hTask);

	// comments
	CString sComments = pSrcTaskFile->GetTaskComments(hTask);

	if (!sComments.IsEmpty())
	{
		// Remove carriage-returns
		sComments.Replace(_T("\r"), _T(""));

		// Escape line-feeds for compatibility with the importer
		sComments.Replace(_T("\n"), _T("\\n"));

		sTask += '|';
		sTask += sComments;
	}

	// add carriage return
	sTask += ENDL;

	// save write to file
	fileOut.WriteString(sTask);

	// copy across first child
	ExportTask(pSrcTaskFile, pSrcTaskFile->GetFirstTask(hTask), fileOut, nDepth + 1, TRUE);

	// handle sibling tasks WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pSrcTaskFile->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == don't recurse on siblings
			ExportTask(pSrcTaskFile, hSibling, fileOut, nDepth, FALSE);
			hSibling = pSrcTaskFile->GetNextTask(hSibling);
		}
	}
}

