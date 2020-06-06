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

	TestLookupPerformance();

	return GetTotals();
}

void CPreferencesTest::TestLookupPerformance()
{
	if (!m_utils.HasCommandlineFlag('p'))
	{
		_tprintf(_T("Add '-p' to run CPreferencesTest::LookupPerformance\n"));
		return;
	}

	BeginTest(_T("CPreferencesTest::LookupPerformance"));

	CString sIniPath = FileMisc::GetTempFilePath(_T("Test"), _T(".ini"));

	FileMisc::DeleteFile(sIniPath);
	CPreferences::Initialise(sIniPath, TRUE);

	CPreferences prefs;
	CDWordArray aItems;

	int nTestSize = 100;

	for (int i = 0; i < 4; i++)
	{
		// Write test
		{
			InitArray(aItems, nTestSize); // not part of test

			// Separate items
			{
				DWORD dwTickStart = GetTickCount();

				for (int k = 0; k < 10; k++)
					prefs.WriteProfileArray(_T("Preferences\\SeparateItems"), aItems);

				DWORD dwDuration = (GetTickCount() - dwTickStart);
				_tprintf(_T("Writing %d separate items took %ld ms\n"), nTestSize, dwDuration / 10);
			}

			// Single concatenated item
			{
				DWORD dwTickStart = GetTickCount();

				for (int k = 0; k < 10; k++)
					prefs.WriteProfileString(_T("Preferences"), _T("SingleItem"), Misc::FormatArray(aItems, ','));

				DWORD dwDuration = (GetTickCount() - dwTickStart);
				_tprintf(_T("Writing single %d item took %ld ms\n"), nTestSize, dwDuration / 10);
			}
		}

		// Read test
		{
			// Reading separate items
			{
				DWORD dwTickStart = GetTickCount();

				for (int k = 0; k < 10; k++)
					prefs.GetProfileArray(_T("Preferences\\SeparateItems"), aItems);

				DWORD dwDuration = (GetTickCount() - dwTickStart);
				_tprintf(_T("Reading %d separate items took %ld ms\n"), nTestSize, dwDuration / 10);
			}

			// Reading/parsing single concatenated item
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
		}

		nTestSize *= 10;
	}

	// Clear preferences to prevent save
	CPreferences::Release();


	EndTest();
}

void CPreferencesTest::InitArray(CDWordArray& aItems, int nSize)
{
	aItems.RemoveAll();

	for (int i = 0; i < nSize; i++)
		aItems.Add(i);
}
