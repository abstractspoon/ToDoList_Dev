// PreferencesTest.cpp: implementation of the CPreferencesTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTest.h"
#include "PreferencesTest.h"

#include "..\shared\misc.h"
#include "..\shared\filemisc.h"

#include "..\interfaces\preferences.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPreferencesTest::CPreferencesTest(const CTestUtils& utils) : CTDLTestBase(utils)
{

}

CPreferencesTest::~CPreferencesTest()
{

}

TESTRESULT CPreferencesTest::Run()
{
	ClearTotals();

	TestArrayPerformance();
	TestSectionPerformance();

	return GetTotals();
}

void CPreferencesTest::TestSectionPerformance()
{
	if (!m_utils.HasCommandlineFlag('p'))
	{
		_tprintf(_T("Add '-p' to run CPreferencesTest::SectionPerformance\n"));
		return;
	}

	BeginTest(_T("CPreferencesTest::SectionPerformance"));

	CString sIniPath = FileMisc::GetTempFilePath(_T("Test"), _T(".ini"));

	FileMisc::DeleteFile(sIniPath);
	CPreferences::Initialise(sIniPath, TRUE);

	CPreferences prefs;
	int nNumSection = 1000, nNumEntry = 50;

	// Writing
	{
		DWORD dwTickStart = GetTickCount();

		for (int k = 0; k < 10; k++)
		{
			for (int nEntry = 0; nEntry < nNumEntry; nEntry++)
			{
				CString sEntryKey = Misc::MakeKey(_T("Entry%d"), nEntry);

				for (int nSection = 0; nSection < nNumSection; nSection++)
				{
					CString sSectionKey = Misc::MakeKey(_T("Section%d"), nSection);

					prefs.WriteProfileInt(sSectionKey, sEntryKey, nSection * nEntry);
				}
			}
		}

		DWORD dwDuration = (GetTickCount() - dwTickStart);
		_tprintf(_T("Writing %d sections x %d entries took %ld ms\n"), nNumSection, nNumEntry, dwDuration / 10);
	}

	// Reading
	{
		DWORD dwTickStart = GetTickCount();
		int nSum = 0;

		for (int k = 0; k < 10; k++)
		{
			for (int nEntry = 0; nEntry < nNumEntry; nEntry++)
			{
				CString sEntryKey = Misc::MakeKey(_T("Entry%d"), nEntry);

				for (int nSection = 0; nSection < nNumSection; nSection++)
				{
					CString sSectionKey = Misc::MakeKey(_T("Section%d"), nSection);

					nSum += prefs.GetProfileInt(sSectionKey, sEntryKey, nSection * nEntry);
				}
			}
		}

		DWORD dwDuration = (GetTickCount() - dwTickStart);
		_tprintf(_T("Reading %d sections x %d entries took %ld ms\n"), nNumSection, nNumEntry, dwDuration / 10);
	}

	// Clear preferences to prevent save
	CPreferences::Release();
	
	EndTest();
}

void CPreferencesTest::TestArrayPerformance()
{
	if (!m_utils.HasCommandlineFlag('p'))
	{
		_tprintf(_T("Add '-p' to run CPreferencesTest::TestArrayPerformance\n"));
		return;
	}

	BeginTest(_T("CPreferencesTest::TestArrayPerformance"));

	// Separate items
	{
		CString sIniPath = FileMisc::GetTempFilePath(_T("TestSeparateArrayItems"), _T(".ini"));
		int nTestSize = 100;

		for (int i = 0; i < 3; i++)
		{
			FileMisc::DeleteFile(sIniPath);
			CPreferences::Initialise(sIniPath, TRUE);

			// scoped so that preferences goes out of scope before release
			{
				CPreferences prefs;

				CDWordArray aItems;
				InitArray(aItems, nTestSize);

				// Write test
				{

					DWORD dwTickStart = GetTickCount();

					for (int k = 0; k < 10; k++)
					{
						for (int i = 0; i < nTestSize; i++)
						{
							prefs.WriteProfileString(_T("Preferences\\SeparateItems"), Misc::MakeKey(_T("Item%d"), i), Misc::Format(aItems[i]));
						}
					}

					DWORD dwDuration = (GetTickCount() - dwTickStart);
					_tprintf(_T("Writing %d separate items took %ld ms\n"), nTestSize, dwDuration / 10);
				}

				// Read test
				{
					DWORD dwTickStart = GetTickCount();

					for (int k = 0; k < 10; k++)
					{
						for (int i = 0; i < nTestSize; i++)
						{
							CString sItem = prefs.GetProfileString(_T("Preferences\\SeparateItems"), Misc::MakeKey(_T("Item%d"), i));
						}
					}

					DWORD dwDuration = (GetTickCount() - dwTickStart);
					_tprintf(_T("Reading %d separate items took %ld ms\n"), nTestSize, dwDuration / 10);
				}

				// Saving
				{
					DWORD dwTickStart = GetTickCount();

					for (int k = 0; k < 10; k++)
					{
						// Have to change something to cause it to save again
						prefs.WriteProfileInt(_T("Preferences"), _T("Test"), k);

						prefs.Save();
					}

					DWORD dwDuration = (GetTickCount() - dwTickStart);
					_tprintf(_T("Saving %d separate items took %ld ms\n"), nTestSize, dwDuration / 10);
				}
			}

			CPreferences::Release();

			nTestSize *= 10;
		}

	}

	// Single concatenated item
	{
		CString sIniPath = FileMisc::GetTempFilePath(_T("TestSingleArrayItem"), _T(".ini"));
		int nTestSize = 100;

		for (int i = 0; i < 3; i++)
		{
			FileMisc::DeleteFile(sIniPath);
			CPreferences::Initialise(sIniPath, TRUE);

			// scoped so that preferences goes out of scope before release
			{
				CPreferences prefs;

				CDWordArray aItems;
				InitArray(aItems, nTestSize);

				// Write test
				{
					DWORD dwTickStart = GetTickCount();

					for (int k = 0; k < 10; k++)
						prefs.WriteProfileString(_T("Preferences"), _T("SingleItem"), Misc::FormatArray(aItems, ','));

					DWORD dwDuration = (GetTickCount() - dwTickStart);
					_tprintf(_T("Writing single %d item took %ld ms\n"), nTestSize, dwDuration / 10);
				}

				// Read test
				{
					DWORD dwTickStart = GetTickCount();

					for (int k = 0; k < 10; k++)
					{
						CString sItem = prefs.GetProfileString(_T("Preferences"), _T("SingleItem"));
						Misc::Split(sItem, aItems, ',');
					}

					DWORD dwDuration = (GetTickCount() - dwTickStart);
					_tprintf(_T("Reading single %d item took %ld ms\n"), nTestSize, dwDuration / 10);
				}

				// Saving
				{
					DWORD dwTickStart = GetTickCount();

					for (int k = 0; k < 10; k++)
					{
						// Have to change something to cause it to save again
						prefs.WriteProfileInt(_T("Preferences"), _T("Test"), k);

						prefs.Save();
					}

					DWORD dwDuration = (GetTickCount() - dwTickStart);
					_tprintf(_T("Saving single %d item took %ld ms\n"), nTestSize, dwDuration / 10);
				}

			}

			CPreferences::Release();

			nTestSize *= 10;
		}
	}
	
	EndTest();
}

void CPreferencesTest::InitArray(CDWordArray& aItems, int nSize)
{
	aItems.RemoveAll();

	for (int i = 0; i < nSize; i++)
		aItems.Add(i);
}
