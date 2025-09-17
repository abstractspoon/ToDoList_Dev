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
	void TestIndividualRules(const CToDoCtrlData& data);
	void TestMultipleRules(const CToDoCtrlData& data);

	void TestTextAttributeRules(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, const CString& sPrefix);
	void TestTextArrayAttributeRules(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, const CString& sPrefix);
	
	void TestIntegerAttributeRules(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, int nBase);
	void TestDoubleAttributeRules(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, double dBase);
	void TestTimePeriodAttributeRules(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, double dBase);
	void TestDateAttributeRules(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, double dBase);

	void PopulateDataModel(CToDoCtrlData& data) const;
	void InitialiseStyles();

	static BOOL ContainsAllTaskIDs(const CDWordArray& aTasks);
	static BOOL ContainsNoTaskIDs(const CDWordArray& aTasks);
	static BOOL ContainsAllTaskIDsButOne(const CDWordArray& aTasks, DWORD dwTaskID);
	static BOOL ContainsOneTaskID(const CDWordArray& aTasks, DWORD dwTaskID);

	template <class T>
	void TestSimpleNumberMatching(const CToDoCtrlData& data, SEARCHPARAM& rule, T tBase) const;

};

#endif // !defined(AFX_TDCTASKMATCHERTEST_H__DD08C7F5_76D6_4587_8325_41F964BFB927__INCLUDED_)

