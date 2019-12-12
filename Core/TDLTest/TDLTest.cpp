// TDLTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TDLTest.h"

#include "ToDoCtrlDataTest.h"
#include "ToDoCtrlTaskLinkTest.h"
#include "TaskFileTest.h"
#include "ImportExportMgrTest.h"
#include "TDCRecurrenceTest.h"
#include "FileMiscTest.h"
#include "DateHelperTest.h"
#include "TimeHelperTest.h"

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

	CCommandLineInfo rCmdInfo;
	theApp.ParseCommandLine(rCmdInfo);

	CString sOutputDir;// = rCmdInfo...;
	CString sControlDir;// = rCmdInfo...;

	CTestUtils utils;

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
	TESTRESULT resOverall;

	resOverall += CToDoCtrlDataTest(utils).Run();

	resOverall += CToDoCtrlTaskLinkTest(utils).Run();

	resOverall += CTaskFileTest(utils).Run();

	resOverall += CImportExportMgrTest(utils).Run();

	resOverall += CTDCRECURRENCETest(utils).Run();

	resOverall += CFileMiscTest(utils).Run();
	
	resOverall += CDateHelperTest(utils).Run();

	resOverall += CTimeHelperTest(utils).Run();

	// More...

	resOverall.ReportResults();
}
