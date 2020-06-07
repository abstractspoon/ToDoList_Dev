// TDCRECURRENCETest.h: interface for the CTDCRECURRENCETest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PREFERENCESTEST_H__C3A4504B_65B5_43B1_8B97_8E6053066724__INCLUDED_)
#define AFX_PREFERENCESTEST_H__C3A4504B_65B5_43B1_8B97_8E6053066724__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TDLTestBase.h"

//////////////////////////////////////////////////////////////////////

class CPreferences;

//////////////////////////////////////////////////////////////////////

class CPreferencesTest : public CTDLTestBase  
{
public:
	CPreferencesTest(const CTestUtils& utils);
	virtual ~CPreferencesTest();
	
	TESTRESULT Run();

protected:
	void TestArrayPerformance();
	void TestSectionPerformance();

protected:
	static void InitArray(CDWordArray& aItems, int nSize);

};

#endif // !defined(AFX_PREFERENCESTEST_H__C3A4504B_65B5_43B1_8B97_8E6053066724__INCLUDED_)
