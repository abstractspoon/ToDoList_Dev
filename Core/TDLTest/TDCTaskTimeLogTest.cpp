// DateHelperTest.cpp: implementation of the CTDCTaskTimeLogTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTest.h"
#include "TDCTaskTimeLogTest.h"

#include "..\ToDoList\TDCTaskTimeLog.h"

#include "..\shared\Misc.h"
#include "..\shared\FileMisc.h"
#include "..\shared\DateHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const CString TEMP_LOGFILE = _T("temp_Log.csv");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCTaskTimeLogTest::CTDCTaskTimeLogTest(const CTestUtils& utils) : CTDLTestBase(utils)
{

}

CTDCTaskTimeLogTest::~CTDCTaskTimeLogTest()
{

}

TESTRESULT CTDCTaskTimeLogTest::Run()
{
	ClearTotals();

	TestLogTime();
	TestLoadLogItems();

	return GetTotals();
}

void CTDCTaskTimeLogTest::TestLogTime()
{
	CTDCScopedTest test(*this, _T("CTDCTaskTimeLogTest::LogTime"));

	// -----------------------------------------------------------------------

	{
	
	}
}

void CTDCTaskTimeLogTest::TestLoadLogItems()
{
	CTDCScopedTest test(*this, _T("CTDCTaskTimeLogTest::LoadLogItems"));

	// -----------------------------------------------------------------------

	// File version = 0 (no header row)
	{
		{
			// 5 fields (invalid)
			// Header delimiter = COMMA
			// Field delimiter = COMMA
			// Decimal delimiter = PERIOD
			// File format = UTF16
			{
				ExpectTrue(FileMisc::SaveFile(TEMP_LOGFILE,
											  _T("Task ID,Title,Time Spent(Hrs),User ID,End Date/Time\n")
											  _T("23,title,0.5,person,2022-10-13 16:23\n"),
											  SFEF_UTF16));

				CTaskTimeLogItemArray aItems;
				CString sHeaderDelim;

				ExpectEQ(0, CTDCTaskTimeLog::LoadLogItems(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
				ExpectTrue(sHeaderDelim.IsEmpty());
			}


			// 6 fields (valid)
			{
				// Header delimiter = COMMA
				// Field delimiter = COMMA
				// Decimal delimiter = PERIOD
				// File format = UTF16
				{
					ExpectTrue(FileMisc::SaveFile(TEMP_LOGFILE,
												  _T("Task ID,Title,Time Spent(Hrs),User ID,End Date/Time,Start Date/Time\n")
												  _T("23,title,0.5,person,2022-10-13 16:23,2022-10-13 15:23\n"),
												  SFEF_UTF16));

					CTaskTimeLogItemArray aItems;
					CString sHeaderDelim;

					ExpectEQ(1, CTDCTaskTimeLog::LoadLogItems(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
					ExpectEQ(sHeaderDelim, _T(","));

					CheckLoadLogItemResult(aItems[0], 
										   23UL, 
										   0.5, 
										   _T("title"), 
										   _T("person"), 
										   _T("2022-10-13 15:23"),
										   _T("2022-10-13 16:23"));
				}

				// Header delimiter = TAB
				// File format = UTF8
				{
					// Field delimiter = SEMICOLON
					// Decimal delimiter = COMMA
					{
						ExpectTrue(FileMisc::SaveFile(TEMP_LOGFILE,
													  _T("Task ID\tTitle\tTime Spent(Hrs)\tUser ID\tEnd Date/Time\tStart Date/Time\n")
													  _T("23;title;0,5;person;2022-10-13 16:23;2022-10-13 15:23\n"),
													  SFEF_UTF8));

						CTaskTimeLogItemArray aItems;
						CString sHeaderDelim;

						ExpectEQ(1, CTDCTaskTimeLog::LoadLogItems(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
						ExpectEQ(sHeaderDelim, _T("\t"));

						CheckLoadLogItemResult(aItems[0],
											   23UL,
											   0.5,
											   _T("title"),
											   _T("person"),
											   _T("2022-10-13 15:23"),
											   _T("2022-10-13 16:23"));
					}

					// Field delimiter = <varies>
					// Decimal delimiter = <varies> but never same as field delimiter
					{
						ExpectTrue(FileMisc::SaveFile(TEMP_LOGFILE,
													  _T("Task ID\tTitle\tTime Spent(Hrs)\tUser ID\tEnd Date/Time\tStart Date/Time\n")
													  _T("23;title;0,5;person;2022-10-13 16:23;2022-10-13 15:23\n")
													  _T("47,title2,0.75,person2,2022-10-13 16:23,2022-10-13 15:23\n"),
													  SFEF_UTF8));

						CTaskTimeLogItemArray aItems;
						CString sHeaderDelim;

						ExpectEQ(2, CTDCTaskTimeLog::LoadLogItems(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
						ExpectEQ(sHeaderDelim, _T("\t"));

						CheckLoadLogItemResult(aItems[0],
											   23UL,
											   0.5,
											   _T("title"),
											   _T("person"),
											   _T("2022-10-13 15:23"),
											   _T("2022-10-13 16:23"));

						CheckLoadLogItemResult(aItems[1],
											   47UL,
											   0.75,
											   _T("title2"),
											   _T("person2"),
											   _T("2022-10-13 15:23"),
											   _T("2022-10-13 16:23"));
					}
				}
			}
		}
	}

	// File version = 1
	// Header delimiter = TAB
	// Field delimiter = COMMA
	// Decimal delimiter = PERIOD
	// File format = UTF16
	{
		ExpectTrue(FileMisc::SaveFile(TEMP_LOGFILE,
									  _T("TODOTIMELOG VERSION 1\n")
									  _T("Task ID\tTitle\tUser ID\tStart Date\tStart Time\tEnd Date\tEnd Time\tTime Spent (Hrs)\tComment\tType\tPath\tColour\n")
									  _T("23,title,person,2022-10-13,15:45,2022-10-13,16:23,0.5,comment,tracked,path,255\n"),
									  SFEF_UTF16));

		CTaskTimeLogItemArray aItems;
		CString sHeaderDelim;

		ExpectEQ(1, CTDCTaskTimeLog::LoadLogItems(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
		ExpectEQ(sHeaderDelim, _T("\t"));

		CheckLoadLogItemResult(aItems[0],
							   23UL,
							   0.5,
							   _T("title"),
							   _T("person"),
							   _T("2022-10-13 15:45"),
							   _T("2022-10-13 16:23"),
							   _T("comment"),
							   _T("path"),
							   _T("tracked"), 
							   255UL);
	}

	// File version = 1
	// Header delimiter = COMMA
	// Field delimiter = TAB
	// Decimal delimiter = COMMA
	// File format = UTF8
	{
		ExpectTrue(FileMisc::SaveFile(TEMP_LOGFILE,
									  _T("TODOTIMELOG VERSION 1\n")
									  _T("Task ID,Title,User ID,Start Date,Start Time,End Date,End Time,Time Spent (Hrs),Comment,Type,Path,Colour\n")
									  _T("123\ttitle\tperson\t2022-10-13\t15:45\t2022-10-13\t16:23\t0,5\tcomment\ttracked\tpath\t255\n"),
									  SFEF_UTF8));

		CTaskTimeLogItemArray aItems;
		CString sHeaderDelim;

		ExpectEQ(1, CTDCTaskTimeLog::LoadLogItems(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
		ExpectEQ(sHeaderDelim, _T(","));

		CheckLoadLogItemResult(aItems[0],
							   123UL,
							   0.5,
							   _T("title"),
							   _T("person"),
							   _T("2022-10-13 15:45"),
							   _T("2022-10-13 16:23"),
							   _T("comment"),
							   _T("path"),
							   _T("tracked"),
							   255UL);
	}

	// File version = 1
	// Header delimiter = SEMICOLON
	// Field delimiter = TAB
	// Decimal delimiter = COMMA
	// File format = UTF8
	{
		ExpectTrue(FileMisc::SaveFile(TEMP_LOGFILE,
									  _T("TODOTIMELOG VERSION 1\n")
									  _T("Task ID;Title;User ID;Start Date;Start Time;End Date;End Time;Time Spent (Hrs);Comment;Type;Path;Colour\n")
									  _T("123\ttitle\tperson\t2022-10-13\t15:45\t2022-10-13\t16:23\t0,5\tcomment\ttracked\tpath\t255\n"),
									  SFEF_UTF8));

		CTaskTimeLogItemArray aItems;
		CString sHeaderDelim;

		ExpectEQ(1, CTDCTaskTimeLog::LoadLogItems(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
		ExpectEQ(sHeaderDelim, _T(";"));

		CheckLoadLogItemResult(aItems[0],
							   123UL,
							   0.5,
							   _T("title"),
							   _T("person"),
							   _T("2022-10-13 15:45"),
							   _T("2022-10-13 16:23"),
							   _T("comment"),
							   _T("path"),
							   _T("tracked"),
							   255UL);
	}

	// File version = 1
	// Header delimiter = '|' (pipe)
	// Field delimiter = TAB
	// Decimal delimiter = COMMA
	// File format = UTF8
	{
		ExpectTrue(FileMisc::SaveFile(TEMP_LOGFILE,
									  _T("TODOTIMELOG VERSION 1\n")
									  _T("Task ID|Title|User ID|Start Date|Start Time|End Date|End Time|Time Spent (Hrs)|Comment|Type|Path|Colour\n")
									  _T("123\ttitle\tperson\t2022-10-13\t15:45\t2022-10-13\t16:23\t0,5\tcomment\ttracked\tpath\t255\n"),
									  SFEF_UTF8));

		CTaskTimeLogItemArray aItems;
		CString sHeaderDelim;

		ExpectEQ(1, CTDCTaskTimeLog::LoadLogItems(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
		ExpectTrue(sHeaderDelim.IsEmpty()); // pipe = not detectable

		CheckLoadLogItemResult(aItems[0],
							   123UL,
							   0.5,
							   _T("title"),
							   _T("person"),
							   _T("2022-10-13 15:45"),
							   _T("2022-10-13 16:23"),
							   _T("comment"),
							   _T("path"),
							   _T("tracked"),
							   255UL);
	}

	// File version = 1
	// Header delimiter = COMMA
	// Field delimiter <varies>
	// Decimal delimiter <varies>
	// File format = UTF8
	{
		ExpectTrue(FileMisc::SaveFile(TEMP_LOGFILE,
									  _T("TODOTIMELOG VERSION 1\n")
									  _T("Task ID,Title,User ID,Start Date,Start Time,End Date,End Time,Time Spent (Hrs),Comment,Type,Path,Colour\n")
									  _T("123\ttitle\tperson\t2022-10-13\t15:45\t2022-10-13\t16:23\t0,5\tcomment\ttracked\tpath\t255\n")
									  _T("47;title2;person2;2022-10-13;15:45;2022-10-13;16:23;0.75;comment2;tracked2;path2;128\n"),
									  SFEF_UTF8));

		CTaskTimeLogItemArray aItems;
		CString sHeaderDelim;

		ExpectEQ(2, CTDCTaskTimeLog::LoadLogItems(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
		ExpectEQ(sHeaderDelim, _T(","));

		CheckLoadLogItemResult(aItems[0],
							   123UL,
							   0.5,
							   _T("title"),
							   _T("person"),
							   _T("2022-10-13 15:45"),
							   _T("2022-10-13 16:23"),
							   _T("comment"),
							   _T("path"),
							   _T("tracked"),
							   255UL);

		CheckLoadLogItemResult(aItems[1],
							   47UL,
							   0.75,
							   _T("title2"),
							   _T("person2"),
							   _T("2022-10-13 15:45"),
							   _T("2022-10-13 16:23"),
							   _T("comment2"),
							   _T("path2"),
							   _T("tracked2"),
							   128UL);
	}
}

void CTDCTaskTimeLogTest::CheckLoadLogItemResult(const TASKTIMELOGITEM& tli,
												 DWORD dwTaskID,
												 double dHours,
												 LPCTSTR szTaskTitle,
												 LPCTSTR szPerson,
												 LPCTSTR szDateFrom,
												 LPCTSTR szDateTo,
												 LPCTSTR szComment,
												 LPCTSTR szPath,
												 LPCTSTR szTracked,
												 COLORREF crAltColor)
{
	ExpectEQ(tli.dwTaskID, dwTaskID);
	ExpectEQ(tli.sTaskTitle, szTaskTitle);
	ExpectEQ(tli.dHours, dHours);
	ExpectEQ(tli.sPerson, szPerson);

	ExpectEQ(tli.sComment, szComment);
	ExpectEQ(tli.sTracked, szTracked);
	ExpectEQ(tli.sPath, szPath);
	ExpectEQ(tli.crAltColor, crAltColor);

	COleDateTime date;

	if (!Misc::IsEmpty(szDateFrom))
	{
		ExpectTrue(date.ParseDateTime(szDateFrom));
		ExpectEQ(tli.dtFrom, date);
	}
	else
	{
		ExpectTrue(!CDateHelper::IsDateSet(tli.dtFrom));
	}

	if (!Misc::IsEmpty(szDateTo))
	{
		ExpectTrue(date.ParseDateTime(szDateTo));
		ExpectEQ(tli.dtTo, date);
	}
	else
	{
		ExpectTrue(!CDateHelper::IsDateSet(tli.dtTo));
	}
}
