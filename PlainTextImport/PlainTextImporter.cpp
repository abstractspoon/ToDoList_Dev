// PlainTextImporter.cpp: implementation of the CPlainTextImporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PlainTextImporter.h"
#include "optionsdlg.h"

#include "..\shared\misc.h"

#include "..\Interfaces\ITasklist.h"
#include "..\Interfaces\IPreferences.h"

#include "..\3rdParty\stdiofileex.h"

#include <time.h>
#include <unknwn.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlainTextImporter::CPlainTextImporter()
{

}

CPlainTextImporter::~CPlainTextImporter()
{

}

void CPlainTextImporter::SetLocalizer(ITransText* /*pTT*/)
{
	//CLocalizer::Initialize(pTT);
}

bool CPlainTextImporter::InitConsts(bool bSilent, IPreferences* pPrefs, LPCTSTR szKey)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString sKey(szKey);
	sKey += _T("\\PlainText");

	WANTPROJECT = pPrefs->GetProfileInt(szKey, _T("IncludeProject"), FALSE);
	INDENT = pPrefs->GetProfileString(szKey, _T("Indent"), _T("  "));

	if (!bSilent)
	{
		COptionsDlg dlg(TRUE, WANTPROJECT, INDENT);

		if (dlg.DoModal() != IDOK)
			return false;
		
		INDENT = dlg.GetIndent();
		WANTPROJECT = dlg.GetWantProject();

		pPrefs->WriteProfileInt(szKey, _T("IncludeProject"), WANTPROJECT);
		pPrefs->WriteProfileString(szKey, _T("Indent"), INDENT);
	}

	return true;
}

IIMPORT_RESULT CPlainTextImporter::Import(LPCTSTR szSrcFilePath, ITaskList* pDestTaskFile, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey)
{
	ITaskList8* pITL8 = GetITLInterface<ITaskList8>(pDestTaskFile, IID_TASKLIST8);
	
	if (!pITL8)
	{
		ASSERT(0);
		return IIR_BADINTERFACE;
	}
	
	if (!InitConsts(bSilent, pPrefs, szKey))
		return IIR_CANCELLED;

	CStdioFileEx file;

	if (!file.Open(szSrcFilePath, CFile::modeRead))
		return IIR_BADFILE;

	// the first line can be the project name
	if (WANTPROJECT)
	{
		CString sProjectName;
		file.ReadString(sProjectName);
		Misc::Trim(sProjectName);

		pITL8->SetProjectName(sProjectName);
	}

	// what follows are the tasks, indented to express subtasks
	int nLastDepth = 0;
	HTASKITEM hLastTask = NULL;

	ROOTDEPTH = -1; // gets set to the first task's depth

	CString sLine;
	
	while (file.ReadString(sLine)) 
	{
		CString sTitle, sComments;

		if (!GetTitleComments(sLine, sTitle, sComments))
			continue;

		// find the appropriate parent fro this task
		HTASKITEM hParent = NULL;
		int nDepth = GetDepth(sLine);

		if (nDepth == nLastDepth) // sibling
		{
			hParent = hLastTask ? pITL8->GetTaskParent(hLastTask) : NULL;
		}
		else if (nDepth > nLastDepth) // child
		{
			hParent = hLastTask;
		}
		else if (hLastTask) // we need to work up the tree
		{
			hParent = pITL8->GetTaskParent(hLastTask);

			while (hParent && nDepth < nLastDepth)
			{
				hParent = pITL8->GetTaskParent(hParent);
				nLastDepth--;
			}
		}
		
		HTASKITEM hTask = pITL8->NewTask(sTitle, hParent, 0);

		if (!sComments.IsEmpty())
			pITL8->SetTaskComments(hTask, sComments);

		// update state
		hLastTask = hTask;
		nLastDepth = nDepth;
	}

	return IIR_SUCCESS;
}

int CPlainTextImporter::GetDepth(const CString& sLine)
{
	if (INDENT.IsEmpty() || sLine.IsEmpty())
		return 0;

	// else
	int nDepth = 0;
	
	if (INDENT == "\t")
	{
		while (nDepth < sLine.GetLength())
		{
			if (sLine[nDepth] == '\t')
				nDepth++;
			else
				break;
		}
	}
	else // one or more spaces
	{
		int nPos = 0;

		while (nPos < sLine.GetLength())
		{
			if (sLine.Find(INDENT, nPos) == nPos)
				nDepth++;
			else
				break;

			// next
			nPos = nDepth * INDENT.GetLength();
		}
	}

	// set root depth if not set 
	if (ROOTDEPTH == -1)
		ROOTDEPTH = nDepth;

	// and take allowance for it
	nDepth -= ROOTDEPTH;

	return nDepth;
}

BOOL CPlainTextImporter::GetTitleComments(const CString& sLine, 
										  CString& sTitle, CString& sComments)
{
	int nDelim = sLine.Find(_T("|")); 
	
	if (nDelim != -1)
	{
		sTitle = sLine.Left(nDelim);
		sComments = sLine.Mid(nDelim + 1);

		// comments replace [\][n] with [\n]
		sComments.Replace(_T("\\n"), _T("\n"));
	}
	else
	{
		sTitle = sLine;
		sComments.Empty();
	}

	// cleanup
	Misc::Trim(sTitle);
	Misc::Trim(sComments);

	return sTitle.GetLength();
}
