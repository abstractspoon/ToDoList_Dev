// TDLTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TDLTest.h"

#include "DateHelperTest.h"
#include "FileMiscTest.h"
#include "ImportExportMgrTest.h"
#include "RemapMenuIDsTest.h"
#include "TaskFileTest.h"
#include "TDCRecurrenceTest.h"
#include "TimeHelperTest.h"
#include "ToDoCtrlDataTest.h"
#include "ToDoCtrlTaskLinkTest.h"

#include "..\shared\EnCommandLineInfo.h"

using namespace std; // for console output

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

/////////////////////////////////////////////////////////////////////////////

// local functions
void DoTests(const CTestUtils& utils);

/////////////////////////////////////////////////////////////////////////////

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		return 1;
	}

//	CTDLTestBase::SelfTest();

	CEnCommandLineInfo cmdInfo;
	theApp.ParseCommandLine(cmdInfo);

	CString sOutputDir;// = cmdInfo...;
	CString sControlDir;// = cmdInfo...;

	CTestUtils utils(cmdInfo);

// 	if (!utils.Initialise(sOutputDir, sControlDir))
// 	{
// 		cerr << _T("Fatal Error: Bad paths passed on command line") << endl;
// 		return 1;
// 	}

	DoTests(utils);

	return 0;
}

void DoTests(const CTestUtils& utils)
{
	TESTRESULT res;

	// Keep sorted for easy scanning
	res += CDateHelperTest(utils).Run();
	res += CFileMiscTest(utils).Run();
	res += CImportExportMgrTest(utils).Run();
	res += CRemapMenuIDsTest(utils).Run();
	res += CTaskFileTest(utils).Run();
	res += CTDCRECURRENCETest(utils).Run();
	res += CTimeHelperTest(utils).Run();
	res += CToDoCtrlDataTest(utils).Run();
	res += CToDoCtrlTaskLinkTest(utils).Run();

	// More...

	res.ReportResults();
}
