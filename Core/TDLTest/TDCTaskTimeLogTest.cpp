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
#include "..\shared\FileRegister.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const CString TEMP_LOGFILE = _T("temp_Log.csv");
const CString TEMP_REFFILE = _T("temp.tdl");

const CString LIST_SEP = Misc::GetListSeparator();

const BOOL USEEXCELFORCSV = CFileRegister::IsRegisteredApp(_T("csv"), _T("EXCEL.EXE"), TRUE);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCTaskTimeLogTest::CTDCTaskTimeLogTest(const CTestUtils& utils) 
	: 
	CTDLTestBase(_T("CTDCTaskTimeLogTest"), utils)
{
#ifdef _DEBUG
	CString sCwd = FileMisc::GetCwd();
#endif
}

CTDCTaskTimeLogTest::~CTDCTaskTimeLogTest()
{
	FileMisc::DeleteFile(TEMP_LOGFILE);
}

TESTRESULT CTDCTaskTimeLogTest::Run()
{
	ClearTotals();

	TestLogTime();
	TestLoadSaveLogFile();

	return GetTotals();
}

void CTDCTaskTimeLogTest::TestLogTime()
{
	CTDCScopedTest test(*this, _T("CTDCTaskTimeLog::LogTime"));

	// NOT Logged separately
	CTaskTimeLogItemArray aItems;
	TASKTIMELOGITEM tli;
	CString sHeaderDelim;

	// New log file (uses latest version)
	{
		// Reusing log instance
		{
			CTDCTaskTimeLog log(TEMP_REFFILE);
			FileMisc::DeleteFile(TEMP_LOGFILE);

			// Invalid dates (from > to)
			{
				PopulateLogItem(tli, 11, _T("title"), _T("person"), _T("2022-11-13 16:23"), _T("2022-10-13 15:23"), 0.5);

				ExpectFalse(log.LogTime(tli, FALSE));
				ExpectFalse(FileMisc::FileExists(TEMP_LOGFILE));
			}

			// Minimum valid fields
			{
				PopulateLogItem(tli, 23, _T("title"), _T("person"), _T("2022-10-13 16:23"), _T("2022-10-13 17:23"), 0.5);

				ExpectTrue(log.LogTime(tli, FALSE));
				ExpectTrue(FileMisc::FileExists(TEMP_LOGFILE));

				ExpectTrue(CTDCTaskTimeLog::LoadLogFile(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
				ExpectEQ(1, aItems.GetSize());
				ExpectTrue((USEEXCELFORCSV && sHeaderDelim == _T("\t")) || (!USEEXCELFORCSV && (sHeaderDelim == LIST_SEP)));
				ExpectTrue(aItems[0] == tli);
			}

			// All fields
			{
				ExpectTrue(FileMisc::FileExists(TEMP_LOGFILE));

				PopulateLogItem(tli, 47, _T("title2"), _T("person2"), _T("2022-10-13 16:23"), _T("2022-10-13 17:23"), 0.5, _T("comment2"), _T("tracked2"), _T("path2"), 255);
				ExpectTrue(log.LogTime(tli, FALSE));

				ExpectTrue(CTDCTaskTimeLog::LoadLogFile(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
				ExpectEQ(2, aItems.GetSize());
				ExpectTrue((USEEXCELFORCSV && sHeaderDelim == _T("\t")) || (!USEEXCELFORCSV && (sHeaderDelim == LIST_SEP)));
				ExpectTrue(aItems[1] == tli);
			}

			// Field values containing embedded field delimiters, newlines and double quotes
			{
				ExpectTrue(FileMisc::FileExists(TEMP_LOGFILE));

				PopulateLogItem(tli, 47, _T("a lo\"ng\"er ti\tt,le"), _T("pe\tr,son2"), _T("2022-10-13 16:23"), _T("2022-10-13 17:23"), 0.5, _T("a\t\t lon\r\ng,er com\r\nment"), _T("tracked2"), _T("path2"), 255);
				ExpectTrue(log.LogTime(tli, FALSE));

				ExpectTrue(CTDCTaskTimeLog::LoadLogFile(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
				ExpectEQ(3, aItems.GetSize());
				ExpectTrue((USEEXCELFORCSV && sHeaderDelim == _T("\t")) || (!USEEXCELFORCSV && (sHeaderDelim == LIST_SEP)));
				ExpectTrue(aItems[2] == tli);
			}
		}

		// NOT Reusing log instance
		{
			FileMisc::DeleteFile(TEMP_LOGFILE);

			// Invalid dates (from > to)
			{
				CTDCTaskTimeLog log(TEMP_REFFILE);
				PopulateLogItem(tli, 11, _T("title"), _T("person"), _T("2022-11-13 16:23"), _T("2022-10-13 15:23"), 0.5);

				ExpectFalse(log.LogTime(tli, FALSE));
				ExpectFalse(FileMisc::FileExists(TEMP_LOGFILE));
			}

			// Minimum valid fields
			{
				CTDCTaskTimeLog log(TEMP_REFFILE);
				PopulateLogItem(tli, 23, _T("title"), _T("person"), _T("2022-10-13 16:23"), _T("2022-10-13 17:23"), 0.5);

				ExpectTrue(log.LogTime(tli, FALSE));
				ExpectTrue(FileMisc::FileExists(TEMP_LOGFILE));

				ExpectTrue(CTDCTaskTimeLog::LoadLogFile(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
				ExpectEQ(1, aItems.GetSize());
				ExpectTrue((USEEXCELFORCSV && sHeaderDelim == _T("\t")) || (!USEEXCELFORCSV && (sHeaderDelim == LIST_SEP)));
				ExpectTrue(aItems[0] == tli);
			}

			// All fields
			{
				CTDCTaskTimeLog log(TEMP_REFFILE);
				ExpectTrue(FileMisc::FileExists(TEMP_LOGFILE));

				PopulateLogItem(tli, 47, _T("title2"), _T("person2"), _T("2022-10-13 16:23"), _T("2022-10-13 17:23"), 0.5, _T("comment2"), _T("tracked2"), _T("path2"), 255);
				ExpectTrue(log.LogTime(tli, FALSE));

				ExpectTrue(CTDCTaskTimeLog::LoadLogFile(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
				ExpectEQ(2, aItems.GetSize());
				ExpectTrue((USEEXCELFORCSV && sHeaderDelim == _T("\t")) || (!USEEXCELFORCSV && (sHeaderDelim == LIST_SEP)));
				ExpectTrue(aItems[1] == tli);
			}
		}
	}

	// Existing VER_0 log file
	{
		ExpectTrue(FileMisc::SaveFile(TEMP_LOGFILE,
									  _T("Task ID,Title,Time Spent(Hrs),User ID,End Date/Time,Start Date/Time\n")
									  _T("23,title,0.5,person,2022-10-13 16:23,2022-10-13 15:23\n"),
									  SFEF_UTF16));
		ExpectTrue(FileMisc::FileExists(TEMP_LOGFILE));

		CTDCTaskTimeLog log(TEMP_REFFILE);

		// All fields
		{
			PopulateLogItem(tli, 47, _T("title2"), _T("person2"), _T("2022-10-13 16:23"), _T("2022-10-13 17:23"), 0.5, _T("comment2"), _T("tracked2"), _T("path2"), 255);
			ExpectTrue(log.LogTime(tli, FALSE));

			ExpectTrue(CTDCTaskTimeLog::LoadLogFile(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
			ExpectEQ(2, aItems.GetSize());
			ExpectTrue(sHeaderDelim == _T(","));

			const TASKTIMELOGITEM& tli1 = aItems[1];

			ExpectFalse(tli == tli1); // Extended fields not logged

			ExpectTrue(tli1.dwTaskID == tli.dwTaskID);
			ExpectTrue(tli1.sTaskTitle == tli.sTaskTitle);
			ExpectTrue(tli1.sPerson == tli.sPerson);
			ExpectTrue(tli1.dtFrom == tli.dtFrom);
			ExpectTrue(tli1.dtTo == tli.dtTo);
			ExpectTrue(tli1.dHours == tli.dHours);

			// Extended fields not logged
			ExpectTrue(tli1.sComment.IsEmpty());
			ExpectTrue(tli1.sType.IsEmpty());
			ExpectTrue(tli1.sPath.IsEmpty());
			ExpectTrue(tli1.crAltColor == CLR_NONE);
		}
	}
}

void CTDCTaskTimeLogTest::PopulateLogItem(TASKTIMELOGITEM& tli,
										  DWORD dwTaskID,
										  LPCTSTR szTaskTitle,
										  LPCTSTR szPerson,
										  LPCTSTR szDateFrom,
										  LPCTSTR szDateTo,
										  double dHours,
										  LPCTSTR szComment,
										  LPCTSTR szType,
										  LPCTSTR szPath,
										  COLORREF crAltColor)
{
	tli.dwTaskID = dwTaskID;
	tli.sTaskTitle = szTaskTitle;
	tli.sPerson = szPerson;
	tli.dtFrom.ParseDateTime(szDateFrom);
	tli.dtTo.ParseDateTime(szDateTo);
	tli.dHours = dHours;
	tli.sComment = szComment;
	tli.sType = szType;
	tli.sPath = szPath;
	tli.crAltColor = crAltColor;
}

void CTDCTaskTimeLogTest::TestLoadSaveLogFile()
{
	CTDCScopedTest test(*this, _T("CTDCTaskTimeLog::Load/SaveLogFile"));

	CTaskTimeLogItemArray aItems, aCheckItems;
	CString sHeaderDelim;

	// File version = 0 (no version row)
	{
		// Field count = 5 (invalid)
		{
			// Header delimiter = COMMA
			// Field delimiter = COMMA
			// Decimal delimiter = PERIOD
			// File format = UTF16
			{
				ExpectTrue(FileMisc::SaveFile(TEMP_LOGFILE,
											  _T("Task ID,Title,Time Spent(Hrs),User ID,End Date/Time\n")
											  _T("23,title,0.5,person,2022-10-13 16:23\n"),
											  SFEF_UTF16));

				ExpectTrue(CTDCTaskTimeLog::LoadLogFile(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
				ExpectEQ(0, aItems.GetSize());
				ExpectTrue(sHeaderDelim.IsEmpty());
			}
		}

		// Field count = 6
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

				ExpectTrue(CTDCTaskTimeLog::LoadLogFile(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
				ExpectEQ(1, aItems.GetSize());
				ExpectEQ(sHeaderDelim, _T(","));

				VerifyLogItemResult(aItems[0],
									   23,
									   _T("title"),
									   _T("person"),
									   _T("2022-10-13 15:23"),
									   _T("2022-10-13 16:23"),
									   0.5);

				TestSaveAndReload(aItems);
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

					ExpectTrue(CTDCTaskTimeLog::LoadLogFile(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
					ExpectEQ(1, aItems.GetSize());
					ExpectEQ(sHeaderDelim, _T("\t"));

					VerifyLogItemResult(aItems[0],
										   23,
										   _T("title"),
										   _T("person"),
										   _T("2022-10-13 15:23"),
										   _T("2022-10-13 16:23"),
										   0.5);

					TestSaveAndReload(aItems);
				}

				// Field delimiter = <varies>
				// Decimal delimiter = <varies> but never same as field delimiter
				{
					ExpectTrue(FileMisc::SaveFile(TEMP_LOGFILE,
												  _T("Task ID\tTitle\tTime Spent(Hrs)\tUser ID\tEnd Date/Time\tStart Date/Time\n")
												  _T("23;title;0,5;person;2022-10-13 16:23;2022-10-13 15:23\n")
												  _T("47,title2,0.75,person2,2022-10-13 16:23,2022-10-13 15:23\n"),
												  SFEF_UTF8));

					ExpectTrue(CTDCTaskTimeLog::LoadLogFile(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
					ExpectEQ(2, aItems.GetSize());
					ExpectEQ(sHeaderDelim, _T("\t"));

					VerifyLogItemResult(aItems[0],
										   23,
										   _T("title"),
										   _T("person"),
										   _T("2022-10-13 15:23"),
										   _T("2022-10-13 16:23"),
										   0.5);

					VerifyLogItemResult(aItems[1],
										   47,
										   _T("title2"),
										   _T("person2"),
										   _T("2022-10-13 15:23"),
										   _T("2022-10-13 16:23"),
										   0.75);

					TestSaveAndReload(aItems);
				}
			}
		}
	}

	// File version = 1
	{
		// Field count = 7 (invalid)
		{
			// Header delimiter = COMMA
			// Field delimiter = COMMA
			// Decimal delimiter = PERIOD
			// File format = UTF16
			{
				ExpectTrue(FileMisc::SaveFile(TEMP_LOGFILE,
											  _T("TODOTIMELOG VERSION 1\n")
											  _T("Task ID\tTitle\tUser ID\tStart Date\tStart Time\tEnd Date\tEnd Time\n")
											  _T("23,title,person,2022-10-13,15:45,2022-10-13,16:23\n"),
											  SFEF_UTF16));

				ExpectTrue(CTDCTaskTimeLog::LoadLogFile(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
				ExpectEQ(0, aItems.GetSize());
				ExpectTrue(sHeaderDelim.IsEmpty());
			}
		}

		// Field count = 8
		{
			// Header delimiter = TAB
			// Field delimiter = COMMA
			// Decimal delimiter = PERIOD
			// File format = UTF16
			{
				ExpectTrue(FileMisc::SaveFile(TEMP_LOGFILE,
											  _T("TODOTIMELOG VERSION 1\n")
											  _T("Task ID\tTitle\tUser ID\tStart Date\tStart Time\tEnd Date\tEnd Time\tTime Spent (Hrs)\n")
											  _T("23,title,person,2022-10-13,15:45,2022-10-13,16:23,0.5\n"),
											  SFEF_UTF16));

				ExpectTrue(CTDCTaskTimeLog::LoadLogFile(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
				ExpectEQ(1, aItems.GetSize());
				ExpectEQ(sHeaderDelim, _T("\t"));

				VerifyLogItemResult(aItems[0],
									   23,
									   _T("title"),
									   _T("person"),
									   _T("2022-10-13 15:45"),
									   _T("2022-10-13 16:23"),
									   0.5);

				TestSaveAndReload(aItems);
			}
		}

		// Field count = 9
		{
			// Header delimiter = TAB
			// Field delimiter = COMMA
			// Decimal delimiter = PERIOD
			// File format = UTF16
			{
				ExpectTrue(FileMisc::SaveFile(TEMP_LOGFILE,
											  _T("TODOTIMELOG VERSION 1\n")
											  _T("Task ID\tTitle\tUser ID\tStart Date\tStart Time\tEnd Date\tEnd Time\tTime Spent (Hrs)\tComment\n")
											  _T("23,title,person,2022-10-13,15:45,2022-10-13,16:23,0.5,comment\n"),
											  SFEF_UTF16));

				ExpectTrue(CTDCTaskTimeLog::LoadLogFile(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
				ExpectEQ(1, aItems.GetSize());
				ExpectEQ(sHeaderDelim, _T("\t"));

				VerifyLogItemResult(aItems[0],
									   23,
									   _T("title"),
									   _T("person"),
									   _T("2022-10-13 15:45"),
									   _T("2022-10-13 16:23"),
									   0.5,
									   _T("comment"));

				TestSaveAndReload(aItems);
			}
		}

		// Field count = 10
		{
			// Header delimiter = TAB
			// Field delimiter = COMMA
			// Decimal delimiter = PERIOD
			// File format = UTF16
			{
				ExpectTrue(FileMisc::SaveFile(TEMP_LOGFILE,
											  _T("TODOTIMELOG VERSION 1\n")
											  _T("Task ID\tTitle\tUser ID\tStart Date\tStart Time\tEnd Date\tEnd Time\tTime Spent (Hrs)\tComment\tType\n")
											  _T("23,title,person,2022-10-13,15:45,2022-10-13,16:23,0.5,comment,tracked\n"),
											  SFEF_UTF16));

				ExpectTrue(CTDCTaskTimeLog::LoadLogFile(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
				ExpectEQ(1, aItems.GetSize());
				ExpectEQ(sHeaderDelim, _T("\t"));

				VerifyLogItemResult(aItems[0],
									   23,
									   _T("title"),
									   _T("person"),
									   _T("2022-10-13 15:45"),
									   _T("2022-10-13 16:23"),
									   0.5,
									   _T("comment"),
									   _T("tracked"));

				TestSaveAndReload(aItems);
			}
		}

		// Field count = 11
		{
			// Header delimiter = TAB
			// Field delimiter = COMMA
			// Decimal delimiter = PERIOD
			// File format = UTF16
			{
				ExpectTrue(FileMisc::SaveFile(TEMP_LOGFILE,
											  _T("TODOTIMELOG VERSION 1\n")
											  _T("Task ID\tTitle\tUser ID\tStart Date\tStart Time\tEnd Date\tEnd Time\tTime Spent (Hrs)\tComment\tType\tPath\n")
											  _T("23,title,person,2022-10-13,15:45,2022-10-13,16:23,0.5,comment,tracked,path\n"),
											  SFEF_UTF16));

				ExpectTrue(CTDCTaskTimeLog::LoadLogFile(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
				ExpectEQ(1, aItems.GetSize());
				ExpectEQ(sHeaderDelim, _T("\t"));

				VerifyLogItemResult(aItems[0],
									   23,
									   _T("title"),
									   _T("person"),
									   _T("2022-10-13 15:45"),
									   _T("2022-10-13 16:23"),
									   0.5,
									   _T("comment"),
									   _T("tracked"),
									   _T("path"));

				TestSaveAndReload(aItems);
			}
		}

		// Field count = 12
		{
			// Header delimiter = TAB
			// Field delimiter = COMMA
			// Decimal delimiter = PERIOD
			// File format = UTF16
			{
				ExpectTrue(FileMisc::SaveFile(TEMP_LOGFILE,
											  _T("TODOTIMELOG VERSION 1\n")
											  _T("Task ID\tTitle\tUser ID\tStart Date\tStart Time\tEnd Date\tEnd Time\tTime Spent (Hrs),Comment,Type,Path,Colour\n")
											  _T("23,title,person,2022-10-13,15:45,2022-10-13,16:23,0.5,comment,tracked,path,255\n"),
											  SFEF_UTF16));

				ExpectTrue(CTDCTaskTimeLog::LoadLogFile(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
				ExpectEQ(1, aItems.GetSize());
				ExpectEQ(sHeaderDelim, _T("\t"));

				VerifyLogItemResult(aItems[0],
									   23,
									   _T("title"),
									   _T("person"),
									   _T("2022-10-13 15:45"),
									   _T("2022-10-13 16:23"),
									   0.5,
									   _T("comment"),
									   _T("tracked"),
									   _T("path"),
									   255);

				TestSaveAndReload(aItems);
			}

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

				ExpectTrue(CTDCTaskTimeLog::LoadLogFile(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
				ExpectEQ(1, aItems.GetSize());
				ExpectEQ(sHeaderDelim, _T(","));

				VerifyLogItemResult(aItems[0],
									   123,
									   _T("title"),
									   _T("person"),
									   _T("2022-10-13 15:45"),
									   _T("2022-10-13 16:23"),
									   0.5,
									   _T("comment"),
									   _T("tracked"),
									   _T("path"),
									   255);

				TestSaveAndReload(aItems);
			}

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

				ExpectTrue(CTDCTaskTimeLog::LoadLogFile(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
				ExpectEQ(1, aItems.GetSize());
				ExpectEQ(sHeaderDelim, _T(";"));

				VerifyLogItemResult(aItems[0],
									   123,
									   _T("title"),
									   _T("person"),
									   _T("2022-10-13 15:45"),
									   _T("2022-10-13 16:23"),
									   0.5,
									   _T("comment"),
									   _T("tracked"),
									   _T("path"),
									   255);

				TestSaveAndReload(aItems);
			}

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

				ExpectTrue(CTDCTaskTimeLog::LoadLogFile(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
				ExpectEQ(1, aItems.GetSize());
				ExpectTrue(sHeaderDelim.IsEmpty()); // pipe = not detectable

				VerifyLogItemResult(aItems[0],
									   123,
									   _T("title"),
									   _T("person"),
									   _T("2022-10-13 15:45"),
									   _T("2022-10-13 16:23"),
									   0.5,
									   _T("comment"),
									   _T("tracked"),
									   _T("path"),
									   255);

				TestSaveAndReload(aItems);
			}

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

				ExpectTrue(CTDCTaskTimeLog::LoadLogFile(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
				ExpectEQ(2, aItems.GetSize());
				ExpectEQ(sHeaderDelim, _T(","));

				VerifyLogItemResult(aItems[0],
									   123,
									   _T("title"),
									   _T("person"),
									   _T("2022-10-13 15:45"),
									   _T("2022-10-13 16:23"),
									   0.5,
									   _T("comment"),
									   _T("tracked"),
									   _T("path"),
									   255);

				VerifyLogItemResult(aItems[1],
									   47UL,
									   _T("title2"),
									   _T("person2"),
									   _T("2022-10-13 15:45"),
									   _T("2022-10-13 16:23"),
									   0.75,
									   _T("comment2"),
									   _T("tracked2"),
									   _T("path2"),
									   128);

				TestSaveAndReload(aItems);
			}

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

				ExpectTrue(CTDCTaskTimeLog::LoadLogFile(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
				ExpectEQ(2, aItems.GetSize());
				ExpectEQ(sHeaderDelim, _T(","));

				VerifyLogItemResult(aItems[0],
									   123,
									   _T("title"),
									   _T("person"),
									   _T("2022-10-13 15:45"),
									   _T("2022-10-13 16:23"),
									   0.5,
									   _T("comment"),
									   _T("tracked"),
									   _T("path"),
									   255);

				VerifyLogItemResult(aItems[1],
									   47,
									   _T("title2"),
									   _T("person2"),
									   _T("2022-10-13 15:45"),
									   _T("2022-10-13 16:23"),
									   0.75,
									   _T("comment2"),
									   _T("tracked2"),
									   _T("path2"),
									   128);

				TestSaveAndReload(aItems);
			}
		}

		// Field count = <varies>
		{
			// Header delimiter = TAB
			// Field delimiter = COMMA
			// Decimal delimiter = PERIOD
			// File format = UTF16
			{
				ExpectTrue(FileMisc::SaveFile(TEMP_LOGFILE,
											  _T("TODOTIMELOG VERSION 1\n")
											  _T("Task ID\tTitle\tUser ID\tStart Date\tStart Time\tEnd Date\tEnd Time\tTime Spent (Hrs)\tComment\tType\tPath\tColour\n")
											  _T("11,title,person,2022-10-13,15:45,2022-10-13,16:23,0.5,comment,tracked,path,255\n")	// 12 fields
											  _T("23,title2,person2,2022-10-13,15:45,2022-10-13,16:23,0.5\n")							//  8 fields
											  _T("31,title3,person3,2022-10-13,15:45,2022-10-13,16:23,0.5,comment3,tracked3,path3\n")	// 11 fields
											  _T("47,title4,person4,2022-10-13,15:45,2022-10-13,16:23\n")								//  7 fields
											  _T("52,title5,person5,2022-10-13,15:45,2022-10-13,16:23,0.5,comment5\n")					//  9 fields
											  _T("68,title6,person6,2022-10-13,15:45,2022-10-13,16:23,0.5,comment6,tracked6\n"),		// 10 fields
											  SFEF_UTF16));

				ExpectTrue(CTDCTaskTimeLog::LoadLogFile(TEMP_LOGFILE, aItems, FALSE, sHeaderDelim));
				ExpectEQ(5, aItems.GetSize());
				ExpectEQ(sHeaderDelim, _T("\t"));

				VerifyLogItemResult(aItems[0],
									   11,
									   _T("title"),
									   _T("person"),
									   _T("2022-10-13 15:45"),
									   _T("2022-10-13 16:23"),
									   0.5,
									   _T("comment"),
									   _T("tracked"),
									   _T("path"),
									   255);

				VerifyLogItemResult(aItems[1],
									   23,
									   _T("title2"),
									   _T("person2"),
									   _T("2022-10-13 15:45"),
									   _T("2022-10-13 16:23"),
									   0.5);

				VerifyLogItemResult(aItems[2],
									   31,
									   _T("title3"),
									   _T("person3"),
									   _T("2022-10-13 15:45"),
									   _T("2022-10-13 16:23"),
									   0.5,
									   _T("comment3"),
									   _T("tracked3"),
									   _T("path3"));

				VerifyLogItemResult(aItems[3],
									   52,
									   _T("title5"),
									   _T("person5"),
									   _T("2022-10-13 15:45"),
									   _T("2022-10-13 16:23"),
									   0.5,
									   _T("comment5"));

				VerifyLogItemResult(aItems[4],
									   68,
									   _T("title6"),
									   _T("person6"),
									   _T("2022-10-13 15:45"),
									   _T("2022-10-13 16:23"),
									   0.5,
									   _T("comment6"),
									   _T("tracked6"));

				TestSaveAndReload(aItems);
			}
		}
	}
}

void CTDCTaskTimeLogTest::VerifyLogItemResult(const TASKTIMELOGITEM& tli,
												 DWORD dwTaskID,
												 LPCTSTR szTaskTitle,
												 LPCTSTR szPerson,
												 LPCTSTR szDateFrom,
												 LPCTSTR szDateTo,
												 double dHours,
												 LPCTSTR szComment,
												 LPCTSTR szType,
												 LPCTSTR szPath,
												 COLORREF crAltColor)
{
	ExpectEQ(tli.dwTaskID, dwTaskID);
	ExpectEQ(tli.sTaskTitle, szTaskTitle);
	ExpectEQ(tli.dHours, dHours);
	ExpectEQ(tli.sPerson, szPerson);

	ExpectEQ(tli.sComment, szComment);
	ExpectEQ(tli.sType, szType);
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

void CTDCTaskTimeLogTest::TestSaveAndReload(const CTaskTimeLogItemArray& aItems)
{
	CTaskTimeLogItemArray aCheckItems;
	CString sHeaderDelim;

	// Save, reload and compare
	ExpectTrue(CTDCTaskTimeLog::SaveLogFile(TEMP_LOGFILE, aItems));
	ExpectTrue(CTDCTaskTimeLog::LoadLogFile(TEMP_LOGFILE, aCheckItems, FALSE, sHeaderDelim));

	ExpectTrue(Misc::MatchAllT(aItems, aCheckItems, TRUE));
}
