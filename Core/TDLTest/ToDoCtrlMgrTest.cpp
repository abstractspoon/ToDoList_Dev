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

CToDoCtrlMgrTest::CToDoCtrlMgrTest(const CTestUtils& utils) 
	: 
	CTDLTestBase(_T("CToDoCtrlMgrTest"), utils)
{

}

CToDoCtrlMgrTest::~CToDoCtrlMgrTest()
{
}

TESTRESULT CToDoCtrlMgrTest::Run()
{
	ClearTotals();

	//TestCreateBackup();

	return GetTotals();
}

void CToDoCtrlMgrTest::TestCreateBackup()
{
	CTDCScopedTest test(*this, _T("CToDoCtrlMgr::CreateBackup"));
	CStringArray aBackups;

	// The file to backup
	ExpectTrue(FileMisc::SaveFile(_T("FileToBackup.csv"), "a"));

	// Create existing backups with variable dates and versions but no specific order
	{
		FileMisc::DeleteFolder(_T("backup"));
		ExpectTrue(FileMisc::CreateFolder(_T("backup")));

		ExpectTrue(FileMisc::SaveFile(_T("backup\\FileToBackup.9_0_0_0.2025-02-07.csv"), "a"));
		ExpectTrue(FileMisc::SaveFile(_T("backup\\FileToBackup.9_0_0_0.2025-02-06.csv"), "a"));
		ExpectTrue(FileMisc::SaveFile(_T("backup\\FileToBackup.9_0_0_0.2025-02-05.csv"), "a"));

		ExpectTrue(FileMisc::SaveFile(_T("backup\\FileToBackup.8_0_0_0.2025-02-07.csv"), "a"));
		ExpectTrue(FileMisc::SaveFile(_T("backup\\FileToBackup.8_0_0_0.2025-02-06.csv"), "a"));

		ExpectTrue(FileMisc::SaveFile(_T("backup\\FileToBackup.7_0_1_0.2025-02-05.csv"), "a"));
		ExpectTrue(FileMisc::SaveFile(_T("backup\\FileToBackup.7_0_1_0.2025-02-04.csv"), "a"));

		ExpectTrue(FileMisc::SaveFile(_T("backup\\FileToBackup.6_0_5_0.2025-02-03.csv"), "a"));

		ExpectTrue(FileMisc::SaveFile(_T("backup\\FileToBackup.5_0_0_0.2025-02-05.csv"), "a"));
		ExpectTrue(FileMisc::SaveFile(_T("backup\\FileToBackup.5_0_0_0.2025-02-04.csv"), "a"));
		ExpectTrue(FileMisc::SaveFile(_T("backup\\FileToBackup.5_0_0_0.2025-02-03.csv"), "a"));

		ExpectEQ(11, FileMisc::FindFiles(_T("backup"), aBackups, FALSE, _T("*.csv")));
	}

	// Perform 1st new backup
	{
		ExpectTrue(CToDoCtrlMgr::CreateBackup(_T("FileToBackup.csv"), _T("backup"), 3));
		ExpectEQ(9, FileMisc::FindFiles(_T("backup"), aBackups, FALSE, _T("*.csv")));

		// We expect the oldest of the v9.0.0.0 backups to have been removed
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.9_0_0_0.2025-02-07.csv")));
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.9_0_0_0.2025-02-06.csv")));
		ExpectFalse(FileMisc::FileExists(_T("backup\\FileToBackup.9_0_0_0.2025-02-05.csv")));

		// We expect older of the v8.0.0.0 backup to have been removed
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.8_0_0_0.2025-02-07.csv")));
		ExpectFalse(FileMisc::FileExists(_T("backup\\FileToBackup.8_0_0_0.2025-02-06.csv")));

		// We expect older of the v7.0.1.0 backup to have been removed
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.7_0_1_0.2025-02-05.csv")));
		ExpectFalse(FileMisc::FileExists(_T("backup\\FileToBackup.7_0_1_0.2025-02-04.csv")));

		// We expect the only v6.0.5.0 backup to still remain
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.6_0_5_0.2025-02-03.csv")));

		// We expect all the v5.0.0.0 backups to remain because the 
		// single 6.0.5.0 backup terminates the culling process
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.5_0_0_0.2025-02-05.csv")));
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.5_0_0_0.2025-02-04.csv")));
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.5_0_0_0.2025-02-03.csv")));
	}

	// Perform 2nd new backup
	{
		// Ensure new date-stamp
		Sleep(1000);

		ExpectTrue(CToDoCtrlMgr::CreateBackup(_T("FileToBackup.csv"), _T("backup"), 3));
		ExpectEQ(9, FileMisc::FindFiles(_T("backup"), aBackups, FALSE, _T("*.csv")));

		// We expect the 2nd oldest of the v9.0.0.0 backups to have been removed
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.9_0_0_0.2025-02-07.csv")));
		ExpectFalse(FileMisc::FileExists(_T("backup\\FileToBackup.9_0_0_0.2025-02-06.csv")));
		ExpectFalse(FileMisc::FileExists(_T("backup\\FileToBackup.9_0_0_0.2025-02-05.csv")));

		// We expect the later v8.0.0.0 backup to still remain
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.8_0_0_0.2025-02-07.csv")));
		ExpectFalse(FileMisc::FileExists(_T("backup\\FileToBackup.8_0_0_0.2025-02-06.csv")));

		// We expect the later v7.0.1.0 backup to still remain
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.7_0_1_0.2025-02-05.csv")));
		ExpectFalse(FileMisc::FileExists(_T("backup\\FileToBackup.7_0_1_0.2025-02-04.csv")));

		// We expect the only v6.0.5.0 backup to still remain
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.6_0_5_0.2025-02-03.csv")));

		// We expect all the v5.0.0.0 backups to remain because the 
		// single 6.0.5.0 backup terminates the culling process
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.5_0_0_0.2025-02-05.csv")));
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.5_0_0_0.2025-02-04.csv")));
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.5_0_0_0.2025-02-03.csv")));
	}

	// Perform 3rd and further backups
	// It stabilises here with the newest 'new' backup simply replacing the oldest 'new' backup
	for (int i = 0; i < 3; i++)
	{
		// Ensure new date-stamp
		Sleep(1000);

		// We expect the latest v9.0.0.0 backup to still remain
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.9_0_0_0.2025-02-07.csv")));
		ExpectFalse(FileMisc::FileExists(_T("backup\\FileToBackup.9_0_0_0.2025-02-06.csv")));
		ExpectFalse(FileMisc::FileExists(_T("backup\\FileToBackup.9_0_0_0.2025-02-05.csv")));

		// We expect the later v8.0.0.0 backup to still remain
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.8_0_0_0.2025-02-07.csv")));
		ExpectFalse(FileMisc::FileExists(_T("backup\\FileToBackup.8_0_0_0.2025-02-06.csv")));

		// We expect the later v7.0.1.0 backup to still remain
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.7_0_1_0.2025-02-05.csv")));
		ExpectFalse(FileMisc::FileExists(_T("backup\\FileToBackup.7_0_1_0.2025-02-04.csv")));

		// We expect the only v6.0.5.0 backup to still remain
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.6_0_5_0.2025-02-03.csv")));

		// We expect all the v5.0.0.0 backups to remain because the 
		// single 6.0.5.0 backup terminates the culling process
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.5_0_0_0.2025-02-05.csv")));
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.5_0_0_0.2025-02-04.csv")));
		ExpectTrue(FileMisc::FileExists(_T("backup\\FileToBackup.5_0_0_0.2025-02-03.csv")));
	}

	// Final clean up
 	FileMisc::DeleteFile(_T("FileToBackup.csv"));
 	FileMisc::DeleteFolder(_T("backup"));
}
