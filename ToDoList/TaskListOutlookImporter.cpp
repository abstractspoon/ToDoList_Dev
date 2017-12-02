// OutlookImporter.cpp: implementation of the COutlookImporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "TasklistOutlookImporter.h"
#include "TDLImportOutlookDlg.h"

#include "..\shared\Localizer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaskListOutlookImporter::CTaskListOutlookImporter()
{
	m_icon.LoadIcon(IDI_OUTLOOK);
}

CTaskListOutlookImporter::~CTaskListOutlookImporter()
{
}

void CTaskListOutlookImporter::SetLocalizer(ITransText* pTT)
{
	CLocalizer::Initialize(pTT);
}

IIMPORTEXPORT_RESULT CTaskListOutlookImporter::Import(LPCTSTR /*szSrcFilePath*/, ITaskList* pDestTaskFile, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey)
{
	if (bSilent)
		return IIER_OTHER;
	
	CTDLImportOutlookDlg dlg;

	return dlg.ImportTasks(pDestTaskFile, pPrefs, szKey);
}
