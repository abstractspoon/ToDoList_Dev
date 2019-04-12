// ImportExportMgrTest.h: interface for the CImportExportMgrTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMPORTEXPORTMGRTEST_H__C2A9F58D_DCE6_48BE_913E_A554CB9C10BC__INCLUDED_)
#define AFX_IMPORTEXPORTMGRTEST_H__C2A9F58D_DCE6_48BE_913E_A554CB9C10BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TDLTestBase.h"

class CImportExportMgrTest : public CTDLTestBase  
{
public:
	CImportExportMgrTest(const CTestUtils& utils);
	virtual ~CImportExportMgrTest();

	void Run();

};

#endif // !defined(AFX_IMPORTEXPORTMGRTEST_H__C2A9F58D_DCE6_48BE_913E_A554CB9C10BC__INCLUDED_)
