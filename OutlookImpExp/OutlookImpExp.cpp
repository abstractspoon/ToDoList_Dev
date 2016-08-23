// OutlookImpExp.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "OutlookImpExp.h"
#include "OutlookImporter.h"
#include "OutlookExporter.h"

#include "..\shared\MSOutlookHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static COutlookImpExpApp theApp;

COutlookImpExpApp::COutlookImpExpApp()
{
}

DLL_DECLSPEC IImportTasklist* CreateImportInterface()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (CMSOutlookHelper::IsOutlookInstalled())
		return new COutlookImporter;
	
	// else
	return NULL;
}

/*
DLL_DECLSPEC IExportTasklist* CreateExportInterface()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// not currently supported
	return NULL;
	
	//return new COutlookExporter;
}
*/

DLL_DECLSPEC int GetInterfaceVersion()
{
	return IIMPORTEXPORT_VERSION;
}
