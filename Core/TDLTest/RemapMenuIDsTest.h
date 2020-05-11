// FileMiscTest.h: interface for the CFileMiscTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REMAPMENUIDSTEST_H__96F4BFF1_37C7_4AB6_95D6_FEE032D025DA__INCLUDED_)
#define AFX_REMAPMENUIDSTEST_H__96F4BFF1_37C7_4AB6_95D6_FEE032D025DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TDLTestBase.h"

#include "..\ToDoList\TDLCustomToolbar.h"

//////////////////////////////////////////////////////////////////////

class CRemapMenuIDsTest : public CTDLTestBase
{
public:
	CRemapMenuIDsTest(const CTestUtils& utils);
	virtual ~CRemapMenuIDsTest();
	
	TESTRESULT Run();

protected:
	void TestTDLCustomToolbar();
	void TestShortcutsMgr();
	void TestTDCFilter();

	static void InitButtonArray(CToolbarButtonArray& aButtons, UINT nIDFrom, UINT nIDTo);
};

#endif // !defined(AFX_REMAPMENUIDSTEST_H__96F4BFF1_37C7_4AB6_95D6_FEE032D025DA__INCLUDED_)
