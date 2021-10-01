// TDLTaskCtrlBaseTest.h: interface for the CToDoCtrlDataTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKCTRLBASETEST_H__DD08C7F5_76D6_4587_8325_41F964BFB927__INCLUDED_)
#define AFX_TASKCTRLBASETEST_H__DD08C7F5_76D6_4587_8325_41F964BFB927__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TDLTestBase.h"

#include "..\todolist\todoctrldata.h"

class CToDoCtrlData;
class CTDCLongestItemMap;

class CTDLTaskCtrlBaseTest : public CTDLTestBase  
{
public:
	CTDLTaskCtrlBaseTest(const CTestUtils& utils);
	virtual ~CTDLTaskCtrlBaseTest();
	
	TESTRESULT Run();

protected:
	CTDCStyleMap m_aStyles;
	CTDCColumnIDMap m_mapColumns;
	CTDCCustomAttribDefinitionArray m_aCustAttribDefs;

protected:
	void TestColumnRecalculationPerformance();
	void TestTreeColumnRecalculationPerformance();
	void TestListColumnRecalculationPerformance();

	void PopulateLongestMap(const CToDoCtrlData& data, CTDCLongestItemMap& mapLongest) const;
};

#endif // !defined(AFX_TASKCTRLBASETEST_H__DD08C7F5_76D6_4587_8325_41F964BFB927__INCLUDED_)
