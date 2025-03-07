// FileMiscTest.cpp: implementation of the CToDoCtrlMgrTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTest.h"
#include "ToDoCtrlMgrTest.h"

#include "..\ToDoList\ToDoCtrlMgr.h"

#include "..\shared\FileMisc.h"
#include "..\shared\Misc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToDoCtrlMgrTest::CToDoCtrlMgrTest(const CTestUtils& utils) : CTDLTestBase(utils)
{

}

CToDoCtrlMgrTest::~CToDoCtrlMgrTest()
{
}

TESTRESULT CToDoCtrlMgrTest::Run()
{
	ClearTotals();

	TestCullBackups();

	return GetTotals();
}

void CToDoCtrlMgrTest::TestCullBackups()
{
	CTDCScopedTest test(*this, _T("CToDoCtrlMgrTest::CullBackups"));
	CStringArray aBackups;

	// The file to backup
	ExpectTrue(FileMisc::SaveFile(_T("FileToBackup.csv"), "a"));

	// Existing backups with variable dates and versions but no specific order
	{
		ExpectFalse(FileMisc::FolderExists(_T("backup")));
		ExpectTrue(FileMisc::CreateFolder(_T("backup")));

		ExpectTrue(FileMisc::SaveFile(_T("backup\\FileToBackup.9_0_0_0.2025-02-07.csv"), "a"));
		ExpectTrue(FileMisc::SaveFile(_T("backup\\FileToBackup.9_0_0_0.2025-02-06.csv"), "a"));

		ExpectTrue(FileMisc::SaveFile(_T("backup\\FileToBackup.8_0_0_0.2025-02-07.csv"), "a"));
		ExpectTrue(FileMisc::SaveFile(_T("backup\\FileToBackup.8_0_0_0.2025-02-06.csv"), "a"));

		ExpectTrue(FileMisc::SaveFile(_T("backup\\FileToBackup.7_0_0_0.2025-02-05.csv"), "a"));

		ExpectEQ(5, FileMisc::FindFiles(_T("backup"), aBackups, FALSE, _T("*.csv")));
	}

	// 1st new backup
	{
		ExpectTrue(CToDoCtrlMgr::CreateBackup(_T("FileToBackup.csv"), _T("backup"), 3));
		ExpectEQ(5, FileMisc::FindFiles(_T("backup"), aBackups, FALSE, _T("*.csv")));

		// We expect none of v9 backups to have been removed
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.9_0_0_0.2025-02-07.csv")));
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.9_0_0_0.2025-02-06.csv")));

		// We expect only the later v8 backup to remain
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.8_0_0_0.2025-02-07.csv")));
		ExpectFalse(FileMisc::FileExists(_T("backup\\FileToBackup.8_0_0_0.2025-02-06.csv")));

		// We expect the only v7 backup to still remain
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.7_0_0_0.2025-02-05.csv")));
	}

	// 2nd new backup
	{
		// Ensure new date-stamp
		Sleep(1000);

		ExpectTrue(CToDoCtrlMgr::CreateBackup(_T("FileToBackup.csv"), _T("backup"), 3));
		ExpectEQ(5, FileMisc::FindFiles(_T("backup"), aBackups, FALSE, _T("*.csv")));

		// We expect only the later v9 backup to remain
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.9_0_0_0.2025-02-07.csv")));
		ExpectFalse(FileMisc::FileExists(_T("backup\\FileToBackup.9_0_0_0.2025-02-06.csv")));

		// We expect the later v8 backup to still remain
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.8_0_0_0.2025-02-07.csv")));
		ExpectFalse(FileMisc::FileExists(_T("backup\\FileToBackup.8_0_0_0.2025-02-06.csv")));

		// We expect the only v7 backup to still remain
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.7_0_0_0.2025-02-05.csv")));
	}

	// 3rd and further backups
	// It stabilises here with the newest 'new' backup simply replacing the oldest 'new' backup
	for (int i = 0; i < 3; i++)
	{
		// Ensure new date-stamp
		Sleep(1000);

		ExpectTrue(CToDoCtrlMgr::CreateBackup(_T("FileToBackup.csv"), _T("backup"), 3));
		ExpectEQ(6, FileMisc::FindFiles(_T("backup"), aBackups, FALSE, _T("*.csv")));

		// We expect the later of v9 backups to still remain
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.9_0_0_0.2025-02-07.csv")));
		ExpectFalse(FileMisc::FileExists(_T("backup\\FileToBackup.9_0_0_0.2025-02-06.csv")));

		// We expect the later of v8 backups to still remain
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.8_0_0_0.2025-02-07.csv")));
		ExpectFalse(FileMisc::FileExists(_T("backup\\FileToBackup.8_0_0_0.2025-02-06.csv")));

		// We expect the only v7 backup to still remain
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.7_0_0_0.2025-02-05.csv")));
	}

	// Final clean up
 	FileMisc::DeleteFile(_T("FileToBackup.csv"));
 	FileMisc::DeleteFolder(_T("backup"));
}
