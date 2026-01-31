// TDLTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TDLTest.h"

#include "DateHelperTest.h"
#include "FileMiscTest.h"
#include "ImportExportMgrTest.h"
#include "MiscTest.h"
#include "PreferencesTest.h"
#include "RemapMenuIDsTest.h"
#include "TaskFileTest.h"
#include "TDCCustomAttributeCalculationTest.h"
#include "TDCDependencyTest.h"
#include "TDCFileBackupTest.h"
#include "TDCRecurrenceTest.h"
#include "TDCTaskCalculatorTest.h"
#include "TDCTaskTimeLogTest.h"
#include "TDLTaskCtrlBaseTest.h"
#include "TDCTaskMatcherTest.h"
#include "TimeHelperTest.h"
#include "ToDoCtrlDataTest.h"
#include "ToDoCtrlTaskLinkTest.h"
#include "WorkingWeekTest.h"
#include "XmlFileTest.h"

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

#define DOTEST(test) test(utils).Run()

void DoTests(const CTestUtils& utils)
{
	TESTRESULT res;
	DWORD dwStartTick = GetTickCount();

	// Keep sorted for easy scanning
	res += DOTEST(CDateHelperTest);
	res += DOTEST(CFileMiscTest);
	res += DOTEST(CImportExportMgrTest);
	res += DOTEST(CMiscTest);
	res += DOTEST(CPreferencesTest);
	res += DOTEST(CRemapMenuIDsTest);
	res += DOTEST(CTaskFileTest);
	res += DOTEST(CTDCCUSTOMATTRIBUTECALCULATIONTest);
	res += DOTEST(CTDCDEPENDENCYTest);
	res += DOTEST(CTDCRECURRENCETest);
	res += DOTEST(CTDCTaskCalculatorTest);
	res += DOTEST(CTDCTaskTimeLogTest);
	res += DOTEST(CTDCTaskMatcherTest);
	res += DOTEST(CTDLTaskCtrlBaseTest);
	res += DOTEST(CTimeHelperTest);
	res += DOTEST(CToDoCtrlDataTest);
	res += DOTEST(CTDCFileBackupTest);
	res += DOTEST(CToDoCtrlTaskLinkTest);
	res += DOTEST(CWorkingDayTest);
	res += DOTEST(CWorkingWeekTest);
	res += DOTEST(CXmlFileTest);

	// More...

	res.ReportResults(_T("All"), FALSE);

	_tprintf(_T("\n  %d tests took %d seconds\n"), res.GetTotal(), (((GetTickCount() - dwStartTick) / 1000) + 1));

	// When we are NOT DEBUGGING we just assert once at the end
	// else we assert in the actual test that failed
	if (!::IsDebuggerPresent())
		ASSERT(res.nNumError == 0);
}
