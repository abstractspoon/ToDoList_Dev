// ToDoCtrlDataTest.h: interface for the CToDoCtrlDataTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TODOCTRLDATATEST_H__DD08C7F5_76D6_4587_8325_41F964BFB927__INCLUDED_)
#define AFX_TODOCTRLDATATEST_H__DD08C7F5_76D6_4587_8325_41F964BFB927__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TDLTestBase.h"

class CToDoCtrlDataTest : public CTDLTestBase  
{
public:
	CToDoCtrlDataTest(const CTestUtils& utils);
	virtual ~CToDoCtrlDataTest();
	
	void Run();

};

#endif // !defined(AFX_TODOCTRLDATATEST_H__DD08C7F5_76D6_4587_8325_41F964BFB927__INCLUDED_)
