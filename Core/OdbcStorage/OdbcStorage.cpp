// OdbcStorage.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "OdbcStorage.h"
#include "OdbctasklistStorage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////

static COdbcStorageApp theApp;

/////////////////////////////////////////////////////////////////////

DLL_DECLSPEC ITasklistStorage* CreateTasklistStorageInterface()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

#ifdef _DEBUG
	return new COdbcTasklistStorage;
#else
	return NULL;
#endif
}
