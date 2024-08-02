// ToDoCtrlDataTest.h: interface for the CToDoCtrlDataTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCTASKCALCULATORTEST_H__DD08C7F5_76D6_4587_8325_41F964BFB927__INCLUDED_)
#define AFX_TDCTASKCALCULATORTEST_H__DD08C7F5_76D6_4587_8325_41F964BFB927__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "TDLTestBase.h"

#include "..\todolist\tdcstruct.h"

//////////////////////////////////////////////////////////////////////

class CToDoCtrlData;

//////////////////////////////////////////////////////////////////////

class CTDCTaskCalculatorTest : public CTDLTestBase
{
public:
	CTDCTaskCalculatorTest(const CTestUtils& utils);
	virtual ~CTDCTaskCalculatorTest();
	
	TESTRESULT Run();

protected:
	CTDCStyleMap m_aStyles;
	CTDCCustomAttribDefinitionArray m_aCustomAttribDefs;

protected:
	void Test();

	void PopulateData(CToDoCtrlData& data) const;
};

#endif // !defined(AFX_TDCTASKCALCULATORTEST_H__DD08C7F5_76D6_4587_8325_41F964BFB927__INCLUDED_)

