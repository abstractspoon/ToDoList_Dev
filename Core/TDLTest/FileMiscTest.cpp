// FileMiscTest.cpp: implementation of the CFileMiscTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTest.h"
#include "FileMiscTest.h"

#include "..\shared\FileMisc.h"
#include "..\shared\Misc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileMiscTest::CFileMiscTest(const CTestUtils& utils) : CTDLTestBase(utils)
{

}

CFileMiscTest::~CFileMiscTest()
{
}

void CFileMiscTest::Run()
{
	TestGetVirtualStorePath();
}

void CFileMiscTest::TestGetVirtualStorePath()
{
	BeginTest(_T("CFileMiscTest::GetVirtualStorePath"));

	LPCTSTR szTestPath1 = _T("C:\\Program Files\\Abstractspoon\\ToDoList\\ToDoList.ini"); // 64-bit app
	LPCTSTR szTestPath2 = _T("C:\\Program Files (x86)\\Abstractspoon\\ToDoList\\ToDoList.ini"); // 32-bit app
	LPCTSTR szTestPath3 = _T("C:\\Code\\ToDoList\\Debug\\ToDoList.ini");
	LPCTSTR szTestPath4 = _T("D:\\Code\\ToDoList\\Debug\\ToDoList.ini");

	CString sVirtualPath1, sVirtualPath2, sVirtualPath3, sVirtualPath4;

	// Only 32bit apps under 64bit Windows get virtualised
#ifdef _WIN64

	ExpectFalse(FileMisc::GetVirtualStorePath(szTestPath1, sVirtualPath1));
	ExpectEmpty(sVirtualPath1);
	
	ExpectFalse(FileMisc::GetVirtualStorePath(szTestPath2, sVirtualPath2));
	ExpectEmpty(sVirtualPath2);

#else // _WIN32

	if (Misc::Is64BitWindows())
	{
		ExpectFalse(FileMisc::GetVirtualStorePath(szTestPath1, sVirtualPath1));
		ExpectEmpty(sVirtualPath1);
		
		ExpectTrue(FileMisc::GetVirtualStorePath(szTestPath2, sVirtualPath2));
		ExpectNotEmpty(sVirtualPath2);
	}
	else // 32-bit
	{
		ExpectFalse(FileMisc::GetVirtualStorePath(szTestPath1, sVirtualPath1));
		ExpectEmpty(sVirtualPath1);
		
		ExpectFalse(FileMisc::GetVirtualStorePath(szTestPath2, sVirtualPath2));
		ExpectEmpty(sVirtualPath2);
	}

#endif

	ExpectTrue(FileMisc::GetVirtualStorePath(szTestPath2, sVirtualPath2));
	ExpectNotEmpty(sVirtualPath2);

	ExpectFalse(FileMisc::GetVirtualStorePath(szTestPath3, sVirtualPath3));
	ExpectEmpty(sVirtualPath3);
	ExpectFalse(FileMisc::GetVirtualStorePath(szTestPath4, sVirtualPath4));
	ExpectEmpty(sVirtualPath4);
	
	EndTest();
}
