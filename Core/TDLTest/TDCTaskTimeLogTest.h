// DateHelperTest.h: interface for the CTimeHelperTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCTASKTIMELOGTEST_H__6A11464D_D4DA_4ABA_8FDB_310C9A5252E5__INCLUDED_)
#define AFX_TDCTASKTIMELOGTEST_H__6A11464D_D4DA_4ABA_8FDB_310C9A5252E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "TDLTestBase.h"

#include "..\ToDoList\TDCTaskTimeLog.h"

//////////////////////////////////////////////////////////////////////

class CTDCTaskTimeLogTest : public CTDLTestBase
{
public:
	CTDCTaskTimeLogTest(const CTestUtils& utils);
	virtual ~CTDCTaskTimeLogTest();

	TESTRESULT Run();

protected:
	void TestLogTime();
	void TestLoadSaveLogFile();

	void VerifyLogItemResult(const TASKTIMELOGITEM& tli,
								DWORD dwTaskID,
								LPCTSTR szTaskTitle,
								LPCTSTR szPerson,
								LPCTSTR szDateFrom,
								LPCTSTR szDateTo,
								double dHours,
								LPCTSTR szComment = _T(""),
								LPCTSTR szType = _T(""),
								LPCTSTR szPath = _T(""),
								COLORREF crAltColor = CLR_NONE);

	void PopulateLogItem(TASKTIMELOGITEM& tli,
						 DWORD dwTaskID,
						 LPCTSTR szTaskTitle,
						 LPCTSTR szPerson,
						 LPCTSTR szDateFrom,
						 LPCTSTR szDateTo,
						 double dHours,
						 LPCTSTR szComment = _T(""),
						 LPCTSTR szType = _T(""),
						 LPCTSTR szPath = _T(""),
						 COLORREF crAltColor = CLR_NONE);

	void TestSaveAndReload(const CTaskTimeLogItemArray& aItems);
};

#endif // !defined(AFX_TDCTASKTIMELOGTEST_H__6A11464D_D4DA_4ABA_8FDB_310C9A5252E5__INCLUDED_)
