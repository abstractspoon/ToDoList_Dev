// TaskFileTest.cpp: implementation of the CTaskFileTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTest.h"
#include "ToDoCtrlTaskLinkTest.h"

#include "..\ToDoList\ToDoCtrl.h"

#include "..\Shared\ShortcutManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

// Stubs
CTDLContentMgr mgrContent;
CShortcutManager mgrShortcuts;
CONTENTFORMAT cfDefault;
TDCCOLEDITFILTERVISIBILITY visDefault;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToDoCtrlTaskLinkTest::CToDoCtrlTaskLinkTest(const CTestUtils& utils) 
	: 
	CTDLTestBase(utils),
	m_tdc(mgrContent, mgrShortcuts, cfDefault, visDefault)
{

}

CToDoCtrlTaskLinkTest::~CToDoCtrlTaskLinkTest()
{

}

TESTRESULT CToDoCtrlTaskLinkTest::Run()
{
	ClearTotals();

	TestFormatTaskLink();
	TestFormatTaskDependency();
	TestParseTaskLink();

	return GetTotals();
}

void CToDoCtrlTaskLinkTest::TestFormatTaskLink()
{
	BeginTest(_T("CToDoCtrl::FormatTaskLink"));

	//  ---------------------------------------

	m_tdc.SetFilePath(_T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

#ifndef _DEBUG
	// These assert so we only run them in release
	ExpectEQ(m_tdc.FormatTaskLink(0, FALSE), _T("")); // Must provide task ID
	ExpectEQ(m_tdc.FormatTaskLink(0, TRUE), _T("")); // Must provide task ID
#endif

	ExpectEQ(m_tdc.FormatTaskLink(10, FALSE), _T("tdl://10"));
	ExpectEQ(m_tdc.FormatTaskLink(10, TRUE), _T("tdl://C:/Users/Daniel%20Godson/AppData/Local/A%20Tasklist.tdl?10"));

	//  ---------------------------------------

	m_tdc.SetFilePath(_T(""));

#ifndef _DEBUG
	// These assert so we only run them in release
	ExpectEQ(m_tdc.FormatTaskLink(10, TRUE), _T("")); // Must have been saved for 'full' task link
#endif

	//  ---------------------------------------

	EndTest();
}

void CToDoCtrlTaskLinkTest::TestFormatTaskDependency()
{
	BeginTest(_T("CToDoCtrl::FormatTaskDependency"));

	//  ---------------------------------------

	m_tdc.SetFilePath(_T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

#ifndef _DEBUG
	// These assert so we only run them in release
	ExpectEQ(m_tdc.FormatTaskDependency(0, FALSE), _T("")); // Must provide task ID
	ExpectEQ(m_tdc.FormatTaskDependency(0, TRUE), _T("")); // Must provide task ID
#endif

	ExpectEQ(m_tdc.FormatTaskDependency(10, FALSE), _T("10"));
	ExpectEQ(m_tdc.FormatTaskDependency(10, TRUE), _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl?10"));
	
	//  ---------------------------------------

	m_tdc.SetFilePath(_T(""));

#ifndef _DEBUG
	ExpectEQ(m_tdc.FormatTaskDependency(10, TRUE), _T("")); // Must have been saved for 'full' task link
#endif

	//  ---------------------------------------

	EndTest();
}

void CToDoCtrlTaskLinkTest::TestParseTaskLink()
{
	BeginTest(_T("CToDoCtrl::ParseTaskLink"));

	//  ---------------------------------------

	m_tdc.SetFilePath(_T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	DWORD dwTaskID = 0;
	CString sFilePath;

	//  ---------------------------------------

	// Task links (bURL = TRUE)
	// Note: Handles ONLY URLs

	// Protocol only
	ExpectFalse(m_tdc.ParseTaskLink(_T("tdl://"), TRUE, dwTaskID, sFilePath));

	// Task ID only
	ExpectTrue(m_tdc.ParseTaskLink(_T("tdl://10"), TRUE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 10UL);
	ExpectEQ(sFilePath, _T(""));

	// Task ID and full path
	ExpectTrue(m_tdc.ParseTaskLink(_T("tdl://C:/Users/Daniel%20Godson/AppData/Local/A%20Tasklist.tdl?10"), TRUE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 10UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	// Task ID and relative path
	ExpectTrue(m_tdc.ParseTaskLink(_T("tdl://A%20Tasklist.tdl?10"), TRUE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 10UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	// Task full path only
	ExpectTrue(m_tdc.ParseTaskLink(_T("tdl://C:/Users/Daniel%20Godson/AppData/Local/A%20Tasklist.tdl"), TRUE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 0UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	// Task relative path only
	ExpectTrue(m_tdc.ParseTaskLink(_T("tdl://A%20Tasklist.tdl"), TRUE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 0UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	// Missing taskID is okay
	ExpectTrue(m_tdc.ParseTaskLink(_T("tdl://C:/Users/Daniel%20Godson/AppData/Local/A%20Tasklist.tdl?"), TRUE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 0UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	// Non-numeric taskID is NOT okay
	ExpectFalse(m_tdc.ParseTaskLink(_T("tdl://C:/Users/Daniel%20Godson/AppData/Local/A%20Tasklist.tdl?20B"), TRUE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 0UL);
	ExpectEmpty(sFilePath);

	// Missing tdl:// is NOT okay
	ExpectFalse(m_tdc.ParseTaskLink(_T(""), TRUE, dwTaskID, sFilePath));
	ExpectFalse(m_tdc.ParseTaskLink(_T("10"), TRUE, dwTaskID, sFilePath));
	ExpectFalse(m_tdc.ParseTaskLink(_T("C:/Users/Daniel%20Godson/AppData/Local/A%20Tasklist.tdl?10"), TRUE, dwTaskID, sFilePath));
	ExpectFalse(m_tdc.ParseTaskLink(_T("A%20Tasklist.tdl?10"), TRUE, dwTaskID, sFilePath));
	ExpectFalse(m_tdc.ParseTaskLink(_T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl?10"), TRUE, dwTaskID, sFilePath));
	ExpectFalse(m_tdc.ParseTaskLink(_T("A Tasklist.tdl?10"), TRUE, dwTaskID, sFilePath));

	//  ---------------------------------------

	// Task Dependencies (bURL = FALSE)
	// Note: Handles both URLs and non_URLs

	// Task ID only
	ExpectTrue(m_tdc.ParseTaskLink(_T("tdl://10"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 10UL);
	ExpectEQ(sFilePath, _T(""));

	ExpectTrue(m_tdc.ParseTaskLink(_T("10"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 10UL);
	ExpectEQ(sFilePath, _T(""));

	// Task ID and full path
	ExpectTrue(m_tdc.ParseTaskLink(_T("tdl://C:/Users/Daniel%20Godson/AppData/Local/A%20Tasklist.tdl?10"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 10UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	ExpectTrue(m_tdc.ParseTaskLink(_T("C:/Users/Daniel%20Godson/AppData/Local/A%20Tasklist.tdl?10"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 10UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	// Task ID and relative path
	ExpectTrue(m_tdc.ParseTaskLink(_T("tdl://A%20Tasklist.tdl?10"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 10UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	ExpectTrue(m_tdc.ParseTaskLink(_T("A%20Tasklist.tdl?10"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 10UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	// Task full path only
	ExpectTrue(m_tdc.ParseTaskLink(_T("tdl://C:/Users/Daniel%20Godson/AppData/Local/A%20Tasklist.tdl"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 0UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	ExpectTrue(m_tdc.ParseTaskLink(_T("C:/Users/Daniel%20Godson/AppData/Local/A%20Tasklist.tdl"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 0UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	ExpectTrue(m_tdc.ParseTaskLink(_T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 0UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	// Task relative path only
	ExpectTrue(m_tdc.ParseTaskLink(_T("tdl://A%20Tasklist.tdl"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 0UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	ExpectTrue(m_tdc.ParseTaskLink(_T("A%20Tasklist.tdl"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 0UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	ExpectTrue(m_tdc.ParseTaskLink(_T("A Tasklist.tdl"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 0UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	// Missing taskID is okay
	ExpectTrue(m_tdc.ParseTaskLink(_T("tdl://C:/Users/Daniel%20Godson/AppData/Local/A%20Tasklist.tdl?"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 0UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	ExpectTrue(m_tdc.ParseTaskLink(_T("C:/Users/Daniel%20Godson/AppData/Local/A%20Tasklist.tdl?"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 0UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	ExpectTrue(m_tdc.ParseTaskLink(_T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl?"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 0UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	ExpectTrue(m_tdc.ParseTaskLink(_T("tdl://A%20Tasklist.tdl?"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 0UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	ExpectTrue(m_tdc.ParseTaskLink(_T("A%20Tasklist.tdl?"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 0UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	ExpectTrue(m_tdc.ParseTaskLink(_T("A Tasklist.tdl?"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 0UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	//  ---------------------------------------

	EndTest();
}
