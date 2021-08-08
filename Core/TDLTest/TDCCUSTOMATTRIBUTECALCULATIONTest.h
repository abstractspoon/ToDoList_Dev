// TaskFileTest.h: interface for the CTaskFileTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCCUSTOMATTRIBUTECALCULATIONTEST_H__21479206_861C_4C47_9837_75F9B9171F90__INCLUDED_)
#define AFX_TDCCUSTOMATTRIBUTECALCULATIONTEST_H__21479206_861C_4C47_9837_75F9B9171F90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TDLTestBase.h"

class CTDCStyleMap;
class CToDoCtrlData;
class CTDCCustomAttribDefinitionArray;

class CTDCCUSTOMATTRIBUTECALCULATIONTest : public CTDLTestBase
{
public:
	CTDCCUSTOMATTRIBUTECALCULATIONTest(const CTestUtils& utils);
	virtual ~CTDCCUSTOMATTRIBUTECALCULATIONTest();

	TESTRESULT Run();

protected:
	void TestTDCCUSTOMATTRIBUTECALCULATION();
	void TestTDCCUSTOMATTRIBUTECALCULATIONSimple();

	void InitCustomAttributes(CTDCCustomAttribDefinitionArray& aAttribDef);
	void InitDataModel(CToDoCtrlData& data);
	void InitStyles(CTDCStyleMap& styles);
};

#endif // !defined(AFX_TDCCUSTOMATTRIBUTECALCULATIONTEST_H__21479206_861C_4C47_9837_75F9B9171F90__INCLUDED_)
