// TDCRECURRENCETest.h: interface for the CTDCRECURRENCETest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCRECURRENCETEST_H__C3A4504B_65B5_43B1_8B97_8E6053066724__INCLUDED_)
#define AFX_TDCRECURRENCETEST_H__C3A4504B_65B5_43B1_8B97_8E6053066724__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TDLTestBase.h"

class CTDCRECURRENCETest : public CTDLTestBase  
{
public:
	CTDCRECURRENCETest(const CTestUtils& utils);
	virtual ~CTDCRECURRENCETest();
	
	void Run();

protected:
	void TestSetRegularity();
};

#endif // !defined(AFX_TDCRECURRENCETEST_H__C3A4504B_65B5_43B1_8B97_8E6053066724__INCLUDED_)
