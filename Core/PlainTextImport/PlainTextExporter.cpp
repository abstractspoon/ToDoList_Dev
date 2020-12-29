// PlainTextExporter.cpp: implementation of the CPlainTextExporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PlainTextImport.h"
#include "PlainTextExporter.h"
#include "optionsdlg.h"

//#include "..\shared\localizer.h"

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

const LPCTSTR ENDL = _T("\r\n");

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

bool CPlainTextExporter::InitConsts(DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey)
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
			return false;
		
		INDENT = dlg.GetIndent();
		WANTPROJECT = dlg.GetWantProject();

		pPrefs->WriteProfileInt(szKey, _T("IncludeProject"), WANTPROJECT);
		pPrefs->WriteProfileString(szKey, _T("Indent"), INDENT);
	}

	return true;
}

IIMPORTEXPORT_RESULT CPlainTextExporter::Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey)
{
	if (!InitConsts(dwFlags, pPrefs, szKey))
		return IIER_CANCELLED;

	CStdioFileEx fileOut;

	if (!fileOut.Open(szDestFilePath, CFile::modeCreate | CFile::modeWrite, SFEF_UTF8WITHOUTBOM))
		return IIER_BADFILE;

	// export report title as dummy task
	const ITaskList4* pTasks = GetITLInterface<ITaskList4>(pSrcTaskFile, IID_TASKLIST4);

	if (WANTPROJECT)
	{
		CString sTitle = pTasks->GetReportTitle();
		
		if (sTitle.IsEmpty())
			sTitle = pTasks->GetProjectName();
		
		// note: we export the title even if it's empty
		// to maintain consistency with the importer that the first line
		// is always the outline name
		sTitle += ENDL;
		fileOut.WriteString(sTitle);
	}
	
	// export first task
	ExportTask(pSrcTaskFile, pSrcTaskFile->GetFirstTask(), fileOut, 0, TRUE);
	
	return IIER_SUCCESS;
}

IIMPORTEXPORT_RESULT CPlainTextExporter::Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey)
{
	if (!InitConsts(dwFlags, pPrefs, szKey))
		return IIER_CANCELLED;

	CStdioFileEx fileOut;

	if (!fileOut.Open(szDestFilePath, (CFile::modeCreate | CFile::modeWrite), SFEF_UTF8WITHOUTBOM))
		return IIER_BADFILE;

	for (int nTaskList = 0; nTaskList < pSrcTaskFile->GetTaskListCount(); nTaskList++)
	{ 
		// minimum interface
		const ITaskList4* pTasks = GetITLInterface<ITaskList4>(pSrcTaskFile->GetTaskList(nTaskList), IID_TASKLIST4);
		
		if (pTasks == NULL)
		{
			ASSERT(0);
			return IIER_BADINTERFACE;
		}

		// always export report title as dummy task 
		CString sTitle = pTasks->GetReportTitle();

		if (sTitle.IsEmpty())
			sTitle = pTasks->GetProjectName();

		// note: we export the title even if it's empty
		// to maintain consistency with the importer that the first line
		// is always the outline name
		sTitle += ENDL;
		fileOut.WriteString(sTitle);

		// export first task at depth == 1 to indent 
		// it from the project name
		ExportTask(pTasks, pTasks->GetFirstTask(), fileOut, 1, TRUE);
		
		// add blank line before next tasklist
		fileOut.WriteString(ENDL);
	}
	
	return IIER_SUCCESS;
}

void CPlainTextExporter::ExportTask(const ITaskList* pSrcTaskFile, HTASKITEM hTask, 
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
		// remove all carriage returns
		sComments.Replace(_T("\r\n"), _T(""));
		sComments.Replace(_T("\n"), _T(""));

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

