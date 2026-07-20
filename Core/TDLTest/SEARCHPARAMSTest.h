// TaskFileTest.h: interface for the CTaskFileTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEARCHPARAMTEST_H__21479206_861C_4C47_9837_75F9B9171F90__INCLUDED_)
#define AFX_SEARCHPARAMTEST_H__21479206_861C_4C47_9837_75F9B9171F90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TDLTestBase.h"

#include "..\Interfaces\IEnums.h"
#include "..\ToDoList\TDCEnum.h"

class CSEARCHPARAMSTest : public CTDLTestBase
{
public:
	CSEARCHPARAMSTest(const CTestUtils& utils);
	virtual ~CSEARCHPARAMSTest();

	TESTRESULT Run();

protected:
	void TestSEARCHPARAMS();
	void TestSearchParamArray();
	void TestSEARCHPARAM();

	void TestTextParam(TDC_ATTRIBUTE nAttribID, FIND_ATTRIBTYPE nType);
	void TestIntegerParam(TDC_ATTRIBUTE nAttribID, FIND_ATTRIBTYPE nType);
	void TestDecimalParam(TDC_ATTRIBUTE nAttribID, FIND_ATTRIBTYPE nType);
	void TestDateParam(TDC_ATTRIBUTE nAttribID, FIND_ATTRIBTYPE nType);
	void TestTimePeriodParam(TDC_ATTRIBUTE nAttribID);
	void TestGroupParam(TDC_ATTRIBUTE nAttribID);
};

#endif // !defined(AFX_SEARCHPARAMTEST_H__21479206_861C_4C47_9837_75F9B9171F90__INCLUDED_)
