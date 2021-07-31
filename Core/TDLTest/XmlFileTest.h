// TaskFileTest.h: interface for the CXmlFileTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLFILETEST_H__21479206_861C_4C47_9837_75F9B9171F90__INCLUDED_)
#define AFX_XMLFILETEST_H__21479206_861C_4C47_9837_75F9B9171F90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TDLTestBase.h"

#include "..\shared\xmlfile.h"

//////////////////////////////////////////////////////////////////////

class CXmlFileTest : public CTDLTestBase  
{
public:
	CXmlFileTest(const CTestUtils& utils);
	virtual ~CXmlFileTest();

	TESTRESULT Run();

	void TestAddPerformance();
	void TestAddItemPerformance(int nNumItems);
	void TestAddSiblingPerformance(int nNumItems);

};

#endif // !defined(AFX_XMLFILETEST_H__21479206_861C_4C47_9837_75F9B9171F90__INCLUDED_)
