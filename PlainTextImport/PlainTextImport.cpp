// PlainTextImport.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PlainTextImport.h"
#include "PlainTextImporter.h"
#include "PlainTextExporter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////

static CPlainTextImportApp theApp;

/////////////////////////////////////////////////////////////////////

DLL_DECLSPEC IImportTasklist* CreateImportInterface()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return new CPlainTextImporter;
}

DLL_DECLSPEC IExportTasklist* CreateExportInterface()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return new CPlainTextExporter;
}

DLL_DECLSPEC int GetInterfaceVersion()
{
	return IIMPORTEXPORT_VERSION;
}
