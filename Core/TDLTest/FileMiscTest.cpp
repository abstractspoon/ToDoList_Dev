// FileMiscTest.cpp: implementation of the CFileMiscTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTest.h"
#include "FileMiscTest.h"

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

CFileMiscTest::CFileMiscTest(const CTestUtils& utils) 
	: 
	CTDLTestBase(_T("CFileMiscTest"), utils)
{

}

CFileMiscTest::~CFileMiscTest()
{
}

TESTRESULT CFileMiscTest::Run()
{
	ClearTotals();

	TestGetVirtualStorePath();
	TestGetExtension();
	TestSplitPath();
	TestFileBackupCullBackups();

	return GetTotals();
}

void CFileMiscTest::TestGetVirtualStorePath()
{
	CTDCScopedTest test(*this, _T("FileMisc::GetVirtualStorePath"));

	LPCTSTR szTestPath1 = _T("C:\\Program Files\\Abstractspoon\\ToDoList\\ToDoList.ini"); // 64-bit app
	LPCTSTR szTestPath2 = _T("C:\\Program Files (x86)\\Abstractspoon\\ToDoList\\ToDoList.ini"); // 32-bit app
	LPCTSTR szTestPath3 = _T("C:\\Code\\ToDoList\\Debug\\ToDoList.ini");
	LPCTSTR szTestPath4 = _T("D:\\Code\\ToDoList\\Debug\\ToDoList.ini");

	CString sVirtualPath1, sVirtualPath2, sVirtualPath3, sVirtualPath4;

	// Only 32bit apps under 64bit Windows get virtualised
#ifdef _WIN64

	ExpectFalse(FileMisc::GetVirtualStorePath(szTestPath1, sVirtualPath1));
	ExpectEmpty(sVirtualPath1);
	
	ExpectFalse(FileMisc::GetVirtualStorePath(szTestPath2, sVirtualPath2));
	ExpectEmpty(sVirtualPath2);

#else // _WIN32

	if (Misc::Is64BitWindows())
	{
		ExpectFalse(FileMisc::GetVirtualStorePath(szTestPath1, sVirtualPath1));
		ExpectEmpty(sVirtualPath1);
		
		ExpectTrue(FileMisc::GetVirtualStorePath(szTestPath2, sVirtualPath2));
		ExpectNotEmpty(sVirtualPath2);
	}
	else // 32-bit
	{
		ExpectFalse(FileMisc::GetVirtualStorePath(szTestPath1, sVirtualPath1));
		ExpectEmpty(sVirtualPath1);
		
		ExpectFalse(FileMisc::GetVirtualStorePath(szTestPath2, sVirtualPath2));
		ExpectEmpty(sVirtualPath2);
	}

#endif

	ExpectTrue(FileMisc::GetVirtualStorePath(szTestPath2, sVirtualPath2));
	ExpectNotEmpty(sVirtualPath2);

	ExpectFalse(FileMisc::GetVirtualStorePath(szTestPath3, sVirtualPath3));
	ExpectEmpty(sVirtualPath3);
	ExpectFalse(FileMisc::GetVirtualStorePath(szTestPath4, sVirtualPath4));
	ExpectEmpty(sVirtualPath4);
}

void CFileMiscTest::TestGetExtension()
{
	CTDCScopedTest test(*this, _T("FileMisc::GetExtension"));

	CString FILEPATHS[] = 
	{
		_T("\\\\NetworkPath\\Fol.der1\\Folder2\\File."),
		_T("C:\\Fol.der1\\Folder2\\File."),
		_T("..\\Fol.der1\\Folder2\\File."),
		_T(".\\Fol.der1\\Folder2\\File."),
		_T("Fol.der1\\Folder2\\File."),
		_T("File.")
	};
	const int NUM_FILES = (sizeof(FILEPATHS) / sizeof(FILEPATHS[0]));

	// single-part extension
	int nFile = NUM_FILES;

	while (nFile--)
	{
		CString sPath = FILEPATHS[nFile] + _T("ext");

		ExpectEQ(_T(".ext"), FileMisc::GetExtension(sPath, TRUE));
		ExpectEQ(_T("ext"), FileMisc::GetExtension(sPath, FALSE));
	}

	// multi-part extensions not supported
	nFile = NUM_FILES;

	while (nFile--)
	{
		CString sPath = FILEPATHS[nFile] + _T("ext1.ext2");

		ExpectEQ(_T(".ext2"), FileMisc::GetExtension(sPath, TRUE));
		ExpectEQ(_T("ext2"), FileMisc::GetExtension(sPath, FALSE));
	}
}

void CFileMiscTest::TestSplitPath()
{
	CTDCScopedTest test(*this, _T("FileMisc::SplitPath"));

	CString sDrive, sFolder, sFileName, sExt;

	{
		CString sFilePath = _T("\\\\NetworkPath\\Fol.der1\\Folder2\\File.");

		// Single-part extension
		FileMisc::SplitPath(sFilePath + _T("ext"), &sDrive, &sFolder, &sFileName, &sExt);

		ExpectEQ(_T(""), sDrive);
		ExpectEQ(_T("\\\\NetworkPath\\Fol.der1\\Folder2\\"), sFolder);
		ExpectEQ(_T("File"), sFileName);
		ExpectEQ(_T(".ext"), sExt);

		// Multi-part extensions not supported
		FileMisc::SplitPath(sFilePath + _T("ext1.ext2"), &sDrive, &sFolder, &sFileName, &sExt);

		ExpectEQ(_T("File.ext1"), sFileName);
		ExpectEQ(_T(".ext2"), sExt);
	}

	{
		CString sFilePath = _T("C:\\Fol.der1\\Folder2\\File.");

		// Single-part extension
		FileMisc::SplitPath(sFilePath + _T("ext"), &sDrive, &sFolder, &sFileName, &sExt);

		ExpectEQ(_T("C:"), sDrive);
		ExpectEQ(_T("\\Fol.der1\\Folder2\\"), sFolder);
		ExpectEQ(_T("File"), sFileName);
		ExpectEQ(_T(".ext"), sExt);

		// Multi-part extensions not supported
		FileMisc::SplitPath(sFilePath + _T("ext1.ext2"), &sDrive, &sFolder, &sFileName, &sExt);

		ExpectEQ(_T("File.ext1"), sFileName);
		ExpectEQ(_T(".ext2"), sExt);
	}

	{
		CString sFilePath = _T("..\\Fol.der1\\Folder2\\File.");

		// Single-part extension
		FileMisc::SplitPath(sFilePath + _T("ext"), &sDrive, &sFolder, &sFileName, &sExt);

		ExpectEQ(_T(""), sDrive);
		ExpectEQ(_T("..\\Fol.der1\\Folder2\\"), sFolder);
		ExpectEQ(_T("File"), sFileName);
		ExpectEQ(_T(".ext"), sExt);

		// Multi-part extensions not supported
		FileMisc::SplitPath(sFilePath + _T("ext1.ext2"), &sDrive, &sFolder, &sFileName, &sExt);

		ExpectEQ(_T("File.ext1"), sFileName);
		ExpectEQ(_T(".ext2"), sExt);
	}

	{
		CString sFilePath = _T(".\\Fol.der1\\Folder2\\File.");

		// Single-part extension
		FileMisc::SplitPath(sFilePath + _T("ext"), &sDrive, &sFolder, &sFileName, &sExt);

		ExpectEQ(_T(""), sDrive);
		ExpectEQ(_T(".\\Fol.der1\\Folder2\\"), sFolder);
		ExpectEQ(_T("File"), sFileName);
		ExpectEQ(_T(".ext"), sExt);

		// Multi-part extensions not supported
		FileMisc::SplitPath(sFilePath + _T("ext1.ext2"), &sDrive, &sFolder, &sFileName, &sExt);

		ExpectEQ(_T("File.ext1"), sFileName);
		ExpectEQ(_T(".ext2"), sExt);
	}

	{
		CString sFilePath = _T("Fol.der1\\Folder2\\File.");

		// Single-part extension
		FileMisc::SplitPath(sFilePath + _T("ext"), &sDrive, &sFolder, &sFileName, &sExt);

		ExpectEQ(_T(""), sDrive);
		ExpectEQ(_T("Fol.der1\\Folder2\\"), sFolder);
		ExpectEQ(_T("File"), sFileName);
		ExpectEQ(_T(".ext"), sExt);

		// Multi-part extensions not supported
		FileMisc::SplitPath(sFilePath + _T("ext1.ext2"), &sDrive, &sFolder, &sFileName, &sExt);

		ExpectEQ(_T("File.ext1"), sFileName);
		ExpectEQ(_T(".ext2"), sExt);
	}

	{
		CString sFilePath = _T("File.");

		// Single-part extension
		FileMisc::SplitPath(sFilePath + _T("ext"), &sDrive, &sFolder, &sFileName, &sExt);

		ExpectEQ(_T(""), sDrive);
		ExpectEQ(_T(""), sFolder);
		ExpectEQ(_T("File"), sFileName);
		ExpectEQ(_T(".ext"), sExt);

		// Multi-part extensions not supported
		FileMisc::SplitPath(sFilePath + _T("ext1.ext2"), &sDrive, &sFolder, &sFileName, &sExt);

		ExpectEQ(_T("File.ext1"), sFileName);
		ExpectEQ(_T(".ext2"), sExt);
	}
}

void CFileMiscTest::TestFileBackupCullBackups()
{
	CTDCScopedTest test(*this, _T("CFileBackup::CullBackups"));

	int nNumFound = 0;

	// Keep NO backups
	// Recreate backups each time
	{
		const int NUMTOKEEP = 0;

		{
			CreateBackupFiles();

			ExpectEQ(11, CFileBackup::CullBackups(_T("backup\\*.*"), FBS_DATESTAMP, NUMTOKEEP, nNumFound));
			ExpectEQ(11, nNumFound);
		}

		{
			CreateBackupFiles();

			ExpectEQ(0, CFileBackup::CullBackups(_T("backup\\*.bob"), FBS_DATESTAMP, NUMTOKEEP, nNumFound));
			ExpectEQ(0, nNumFound);
		}

		{
			CreateBackupFiles();

			ExpectEQ(11, CFileBackup::CullBackups(_T("backup\\*.csv"), FBS_DATESTAMP, NUMTOKEEP, nNumFound));
			ExpectEQ(11, nNumFound);
		}

		{
			CreateBackupFiles();

			ExpectEQ(3, CFileBackup::CullBackups(_T("backup\\FileToBackup.9_0_0_0*.csv"), FBS_DATESTAMP, NUMTOKEEP, nNumFound));
			ExpectEQ(3, nNumFound);
		}

		{
			CreateBackupFiles();

			ExpectEQ(8, CFileBackup::CullBackups(_T("backup\\*_0_0_0.*.csv"), FBS_DATESTAMP, NUMTOKEEP, nNumFound));
			ExpectEQ(8, nNumFound);
		}
	}

	// Keep 2 backups
	{ 
		const int NUMTOKEEP = 2;

		// Recreate backups each time
		{
			{
				CreateBackupFiles();

				ExpectEQ(9, CFileBackup::CullBackups(_T("backup\\*.*"), FBS_DATESTAMP, NUMTOKEEP, nNumFound));
				ExpectEQ(11, nNumFound);
			}

			{
				CreateBackupFiles();

				ExpectEQ(0, CFileBackup::CullBackups(_T("backup\\*.bob"), FBS_DATESTAMP, NUMTOKEEP, nNumFound));
				ExpectEQ(0, nNumFound);
			}

			{
				CreateBackupFiles();

				ExpectEQ(9, CFileBackup::CullBackups(_T("backup\\*.csv"), FBS_DATESTAMP, NUMTOKEEP, nNumFound));
				ExpectEQ(11, nNumFound);
			}

			{
				CreateBackupFiles();

				ExpectEQ(1, CFileBackup::CullBackups(_T("backup\\FileToBackup.9_0_0_0*.csv"), FBS_DATESTAMP, 2, nNumFound));
				ExpectEQ(3, nNumFound);
			}

			{
				CreateBackupFiles();

				ExpectEQ(6, CFileBackup::CullBackups(_T("backup\\*_0_0_0.*.csv"), FBS_DATESTAMP, NUMTOKEEP, nNumFound));
				ExpectEQ(8, nNumFound);
			}
		}

		// DON'T recreate backups each time
		{
			CreateBackupFiles();

			{
				ExpectEQ(1, CFileBackup::CullBackups(_T("backup\\FileToBackup.9_0_0_0*.csv"), FBS_DATESTAMP, NUMTOKEEP, nNumFound));
				ExpectEQ(3, nNumFound);
			}

			{
				ExpectEQ(5, CFileBackup::CullBackups(_T("backup\\*_0_0_0.*.csv"), FBS_DATESTAMP, NUMTOKEEP, nNumFound));
				ExpectEQ(7, nNumFound);
			}

			{
				ExpectEQ(0, CFileBackup::CullBackups(_T("backup\\*.2025-02-05.csv"), FBS_DATESTAMP, NUMTOKEEP, nNumFound));
				ExpectEQ(1, nNumFound);
			}

			{
				ExpectEQ(3, CFileBackup::CullBackups(_T("backup\\*.*"), FBS_DATESTAMP, NUMTOKEEP, nNumFound));
				ExpectEQ(5, nNumFound);
			}
		}
	}
}

void CFileMiscTest::CreateBackupFiles()
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

	CStringArray aBackups;
	ExpectEQ(11, FileMisc::FindFiles(_T("backup"), aBackups, FALSE, _T("*.csv")));
}
