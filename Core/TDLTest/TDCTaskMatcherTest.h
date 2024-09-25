// ToDoCtrlDataTest.h: interface for the CToDoCtrlDataTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCTASKMATCHERTEST_H__DD08C7F5_76D6_4587_8325_41F964BFB927__INCLUDED_)
#define AFX_TDCTASKMATCHERTEST_H__DD08C7F5_76D6_4587_8325_41F964BFB927__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "TDLTestBase.h"

#include "..\todolist\tdcstruct.h"

//////////////////////////////////////////////////////////////////////

class CToDoCtrlData;

//////////////////////////////////////////////////////////////////////

class CTDCTaskMatcherTest : public CTDLTestBase
{
public:
	CTDCTaskMatcherTest(const CTestUtils& utils);
	virtual ~CTDCTaskMatcherTest();
	
	TESTRESULT Run();

protected:
	CTDCStyleMap m_aStyles;
	CTDCCustomAttribDefinitionArray m_aCustomAttribDefs;

protected:
	void TestSimpleMatching(const CToDoCtrlData& data);
	void TestComplexMatching(const CToDoCtrlData& data);

	void TestSimpleTextMatch(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, const CString& sPrefix);
	void TestSimpleTextArrayMatch(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, const CString& sPrefix);

	void PopulateDataModel(CToDoCtrlData& data) const;
	void InitialiseStyles();
};

#endif // !defined(AFX_TDCTASKMATCHERTEST_H__DD08C7F5_76D6_4587_8325_41F964BFB927__INCLUDED_)

