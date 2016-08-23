// OutlookImporter.cpp: implementation of the COutlookImporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OutlookImpExp.h"
#include "OutlookImporter.h"
#include "OutlookImportDlg.h"

#include "..\shared\Localizer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COutlookImporter::COutlookImporter() : m_hIcon(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_OUTLOOK);
}

COutlookImporter::~COutlookImporter()
{
	::DestroyIcon(m_hIcon);
}

void COutlookImporter::SetLocalizer(ITransText* pTT)
{
	CLocalizer::Initialize(pTT);
}

IIMPORT_RESULT COutlookImporter::Import(LPCTSTR /*szSrcFilePath*/, ITaskList* pDestTaskFile, BOOL bSilent, IPreferences* pPrefs, LPCTSTR szKey)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (bSilent)
		return IIR_OTHER;
	
	COutlookImportDlg dlg;

	return dlg.ImportTasks(pDestTaskFile, pPrefs, szKey);
}
