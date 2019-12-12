// TaskFileTest.h: interface for the CTaskFileTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TODOCTRLTASKLINKTEST_H__21479206_861C_4C47_9837_75F9B9171F90__INCLUDED_)
#define AFX_TODOCTRLTASKLINKTEST_H__21479206_861C_4C47_9837_75F9B9171F90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TDLTestBase.h"

#include "..\ToDoList\ToDoCtrl.h"

class CToDoCtrlTaskLinkTest : public CTDLTestBase
{
public:
	CToDoCtrlTaskLinkTest(const CTestUtils& utils);
	virtual ~CToDoCtrlTaskLinkTest();

	TESTRESULT Run();

protected:
	CToDoCtrl m_tdc;

protected:
	void TestFormatTaskLink();
	void TestFormatTaskDependency();
	void TestParseTaskLink();
};

#endif // !defined(AFX_TODOCTRLTASKLINKTEST_H__21479206_861C_4C47_9837_75F9B9171F90__INCLUDED_)
