// TDLTestBase.cpp: implementation of the CTDLTestBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTestBase.h"

#include "..\shared\misc.h"
#include "..\shared\filemisc.h"

#include <math.h>

using namespace std; // for console output

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTestUtils::CTestUtils()
{
}

CTestUtils::CTestUtils(const CTestUtils& utils) 
{
	*this = utils;
}

BOOL CTestUtils::Initialise(const CString& sOutputDir, const CString& sControlDir)
{
	// one time only
	if (!m_sOutputDir.IsEmpty() && !m_sControlDir.IsEmpty())
		return FALSE;
	
	if (sOutputDir.IsEmpty() || sControlDir.IsEmpty())
		return FALSE;

	if (FileMisc::IsSamePath(sOutputDir, sControlDir))
		return FALSE;

	if (!FileMisc::CreateFolder(sOutputDir))
		return FALSE;
	
	if (!FileMisc::CreateFolder(sControlDir))
		return FALSE;

	m_sOutputDir = sOutputDir;
	m_sControlDir = sControlDir;
	
	return TRUE;
}

CString CTestUtils::GetOutputFilePath(const CString& sSubDir, const CString& sFilename, const CString& sExt) const
{
	return GetFilePath(m_sOutputDir, sSubDir, sFilename, sExt);
}

CString CTestUtils::GetControlFilePath(const CString& sSubDir, const CString& sFilename, const CString& sExt) const
{
	return GetFilePath(m_sControlDir, sSubDir, sFilename, sExt);
}

CString CTestUtils::GetFilePath(const CString& sRoot, const CString& sSubDir, 
								const CString& sFilename, const CString& sExt)
{
	ASSERT(FileMisc::IsPath(sRoot) && !sSubDir.IsEmpty() && !sFilename.IsEmpty() && !sExt.IsEmpty());
	
	
	if (FileMisc::IsPath(sRoot) && !sSubDir.IsEmpty() && !sFilename.IsEmpty() && !sExt.IsEmpty())
	{
		CString sFilePath, sTemp(FileMisc::PathConcat(sRoot, sSubDir));
		FileMisc::MakePath(sFilePath, NULL, sTemp, sFilename, sExt);
		
		return sFilePath;
	}
	
	return _T("");
}

int CTestUtils::Compare(float f1, float f2, float fTol) const
{
	if (fabs(f1 - f2) < fTol)
		return 0;

	return (f1 < f2) ? -1 : 1;
}

int CTestUtils::Compare(double d1, double d2, double dTol) const
{
	if (fabs(d1 - d2) < dTol)
		return 0;
	
	return (d1 < d2) ? -1 : 1;
}

int CTestUtils::Compare(const CString& s1, const CString& s2, BOOL bCaseSensitive) const
{
	return Compare((LPCTSTR)s1, (LPCTSTR)s2, bCaseSensitive);
}

int CTestUtils::Compare(LPCTSTR sz1, LPCTSTR sz2, BOOL bCaseSensitive) const
{
	return (bCaseSensitive ? _tcscmp(sz1, sz2) : _tcsicmp(sz1, sz2));
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

class CTDLTestSelfTest : public CTDLTestBase
{
public:
	CTDLTestSelfTest() : CTDLTestBase() {}

	void Run()
	{
		BeginTest(_T("CTDLTestBase::SelfTest"));

		// -------------------------------------------------
		
		ExpectTrue(true);
		ExpectFalse(false);
		ExpectTrue(!false);
		ExpectFalse(!true);

		ExpectTrue(TRUE);
		ExpectFalse(FALSE);
		ExpectTrue(!FALSE);
		ExpectFalse(!TRUE);

		// -------------------------------------------------

		ExpectEQ(1.5, (3.0 / 2));				// double
		ExpectNE(1.5, (3.0 / 2), 0.0);			// double
		ExpectEQ(1.5, (1.4 + 0.1));				// double
		ExpectNE(1.5, (1.4 + 0.1), 0.0);		// double
		ExpectEQ(1.5f, (3.0f / 2));				// float
		ExpectNE(1.5f, (3.0f / 2), 0.0f);		// float
		ExpectEQ(1.5f, (1.4f + 0.1f));			// float
		ExpectNE(1.5f, (1.4f + 0.1f), 0.0f);	// float

		ExpectEQ(true, true);					// bool
		ExpectNE(true, false);					// bool
		ExpectEQ(false, false);					// bool
		ExpectNE(false, true);					// bool
		ExpectEQ('c', 'c');						// char
		ExpectNE('c', 'd');						// char
		ExpectEQ(1, 1);							// int
		ExpectNE(1, 2);							// int
		ExpectEQ(1U, 1U);						// unsigned int
		ExpectNE(1U, 2U);						// unsigned int
		ExpectEQ(1L, 1L);						// long
		ExpectNE(1L, 2L);						// long
		ExpectEQ(1UL, 1UL);						// unsigned long
		ExpectNE(1UL, 2UL);						// unsigned long

		// -------------------------------------------------

		ExpectEQ(_T("Bob"), _T("BOB"), FALSE);
		ExpectNE(_T("Bob"), _T("BOB"));
		ExpectEQ(_T("Bob"), _T("Bob"));
		ExpectEQ(_T("Bob"), _T("Bob"), FALSE);

		// -------------------------------------------------

		CDWordArray aDW1, aDW2;

		aDW1.Add(1);
		aDW1.Add(3);
		aDW1.Add(5);
		aDW1.Add(7);
		
		aDW2.Add(3);
		aDW2.Add(5);
		aDW2.Add(7);
		aDW2.Add(1);

		ExpectEQ(aDW1, aDW2, FALSE);			// order unimportant
		ExpectNE(aDW1, aDW2, TRUE);				// order important

		aDW2.RemoveAt(3);
		aDW2.InsertAt(0, 1);

		ExpectEQ(aDW1, aDW2, FALSE);			// order unimportant
		ExpectEQ(aDW1, aDW2, TRUE);				// order important

		// -------------------------------------------------

		CStringArray aStr1, aStr2;

		aStr1.Add(_T("1"));
		aStr1.Add(_T("3"));
		aStr1.Add(_T("5"));
		aStr1.Add(_T("7"));
		
		aStr2.Add(_T("3"));
		aStr2.Add(_T("5"));
		aStr2.Add(_T("7"));
		aStr2.Add(_T("1"));

		ExpectEQ(aStr1, aStr2, FALSE);			// order unimportant
		ExpectNE(aStr1, aStr2, TRUE);			// order important

		aStr2.RemoveAt(3);
		aStr2.InsertAt(0, _T("1"));

		ExpectEQ(aStr1, aStr2, FALSE);			// order unimportant
		ExpectEQ(aStr1, aStr2, TRUE);			// order important

		// -----------

		aStr1.RemoveAll();
		aStr2.RemoveAll();

		aStr1.Add(_T("A"));
		aStr1.Add(_T("B"));
		aStr1.Add(_T("C"));
		aStr1.Add(_T("D"));
		
		aStr2.Add(_T("b"));
		aStr2.Add(_T("c"));
		aStr2.Add(_T("d"));
		aStr2.Add(_T("a"));

		ExpectEQ(aStr1, aStr2, FALSE, FALSE);	// order/case unimportant
		ExpectNE(aStr1, aStr2, FALSE, TRUE);	// order unimportant, case important

		aStr2.RemoveAt(3);
		aStr2.InsertAt(0, _T("a"));

		ExpectEQ(aStr1, aStr2, TRUE, FALSE);	// order important, case unimportant
		ExpectNE(aStr1, aStr2, TRUE, TRUE);		// order/case important

		// -------------------------------------------------

		ExpectFileContentsEQ(_T("C:\\Windows\\write.exe"), _T("C:\\Windows\\write.exe"));
		ExpectFileContentsNE(_T("C:\\Windows\\write.exe"), _T("C:\\Windows\\winhlp32.exe"));
		
		// -------------------------------------------------
		
		const CString TEST_PATH(_T("c:\\folder\\sub_folder\\file_name.ext"));
		CString sPath;
		
		sPath = CTestUtils::GetFilePath(_T("c:\\folder\\"), _T("\\sub_folder\\"), _T("file_name"), _T("ext"));
		ExpectEQ(sPath, TEST_PATH);

		sPath = CTestUtils::GetFilePath(_T("c:\\folder\\"), _T("\\sub_folder\\"), _T("file_name"), _T(".ext"));
		ExpectEQ(sPath, TEST_PATH);
		
		sPath = CTestUtils::GetFilePath(_T("c:\\folder"), _T("\\sub_folder\\"), _T("file_name"), _T("ext"));
		ExpectEQ(sPath, TEST_PATH);
		
		sPath = CTestUtils::GetFilePath(_T("c:\\folder"), _T("sub_folder\\"), _T("file_name"), _T("ext"));
		ExpectEQ(sPath, TEST_PATH);
		
		sPath = CTestUtils::GetFilePath(_T("c:\\folder"), _T("sub_folder"), _T("file_name"), _T("ext"));
		ExpectEQ(sPath, TEST_PATH);
		
		// -------------------------------------------------

		EndTest();
	}

};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDLTestBase::CTDLTestBase(const CTestUtils& utils) : m_utils(utils) 
{

}

CTDLTestBase::~CTDLTestBase()
{

}

BOOL CTDLTestBase::BeginTest(LPCTSTR szTest) 
{
	if (Misc::IsEmpty(szTest))
	{
		ASSERT(0);
		return FALSE;
	}

	ASSERT(m_sCurrentTest.IsEmpty());

	if (!m_sCurrentTest.IsEmpty())
		return FALSE;

	m_sCurrentTest = szTest;
	m_nTestError = m_nTestSuccess = m_nCurTest = 0;

	_tprintf(_T("\nTest '%s' started --------\n\n"), szTest);

	return TRUE;
}

BOOL CTDLTestBase::EndTest()
{
	ASSERT(!m_sCurrentTest.IsEmpty());
	
	if (m_sCurrentTest.IsEmpty())
		return FALSE;

	if (GetTestCount(FALSE))
	{
		if (GetErrorCount(FALSE))
			_tprintf(_T("\n  %2d/%2d tests FAILED\n"), GetErrorCount(FALSE), GetTestCount(FALSE));

		if (GetSuccessCount(FALSE))
			_tprintf(_T("\n  %2d/%2d tests SUCCEEDED\n"), GetSuccessCount(FALSE), GetTestCount(FALSE));
	}
	else
	{
		_tprintf(_T("  No tests were run\n")); 
	}
	
	_tprintf(_T("\nTest '%s' ended ----------\n"), m_sCurrentTest);
	
	m_sCurrentTest.Empty();
	return TRUE;
}

// -------------------------------------------------------------------------------------------------------

BOOL CTDLTestBase::ExpectTrue(bool b) const
{
	return ExpectEQ(b, true);
}

BOOL CTDLTestBase::ExpectFalse(bool b) const
{
	return ExpectEQ(b, false);
}

// -------------------------------------------------------------------------------------------------------

#define BOOL2STR(b) (b ? _T("TRUE") : _T("FALSE"))

BOOL CTDLTestBase::ExpectTrue(BOOL b) const
{
	return ExpectCompareT((b != FALSE), BOOL2STR(b), true, BOOL2STR(true), OP_EQ);
}

BOOL CTDLTestBase::ExpectFalse(BOOL b) const
{
	return ExpectCompareT((b != FALSE), BOOL2STR(b), false, BOOL2STR(false), OP_EQ);
}

#undef BOOL2STR

// -------------------------------------------------------------------------------------------------------

BOOL CTDLTestBase::ExpectEmpty(const CString& s1) const
{
	return ExpectEQ((BOOL)s1.IsEmpty(), TRUE);
}

BOOL CTDLTestBase::ExpectEmpty(LPCTSTR sz1) const
{
	return (ExpectEQ(sz1, NULL) || 
			ExpectEQ(sz1[0], (TCHAR)0));
}

BOOL CTDLTestBase::ExpectNotEmpty(const CString& s1) const
{
	return ExpectNE((BOOL)s1.IsEmpty(), TRUE);
}

BOOL CTDLTestBase::ExpectNotEmpty(LPCTSTR sz1) const
{
	return (ExpectNE(sz1, NULL) && 
			ExpectNE(sz1[0], (TCHAR)0));
}

// -------------------------------------------------------------------------------------------------------

#define BOOL2STR(b) (b ? _T("true") : _T("false"))

BOOL CTDLTestBase::ExpectEQ(bool b1, bool b2) const
{
	return ExpectCompareT(b1, BOOL2STR(b1), b2, BOOL2STR(b2), OP_EQ);
}

BOOL CTDLTestBase::ExpectNE(bool b1, bool b2) const
{
	return ExpectCompareT(b1, BOOL2STR(b1), b2, BOOL2STR(b2), OP_NE);
}

#undef BOOL2STR

// -------------------------------------------------------------------------------------------------------

BOOL CTDLTestBase::ExpectEQ(char c1, char c2) const
{
	return ExpectCompareT(c1, _T("%c"), c2, _T("%c"), OP_EQ);
}

BOOL CTDLTestBase::ExpectNE(char c1, char c2) const
{
	return ExpectCompareT(c1, _T("%c"), c2, _T("%c"), OP_NE);
}

// -------------------------------------------------------------------------------------------------------

BOOL CTDLTestBase::ExpectEQ(short n1, short n2) const
{
	return ExpectCompareT(n1, _T("%d"), n2, _T("%d"), OP_EQ);
}

BOOL CTDLTestBase::ExpectNE(short n1, short n2) const
{
	return ExpectCompareT(n1, _T("%d"), n2, _T("%d"), OP_NE);
}

// -------------------------------------------------------------------------------------------------------

BOOL CTDLTestBase::ExpectEQ(int n1, int n2) const 
{
	return ExpectCompareT(n1, _T("%d"), n2, _T("%d"), OP_EQ);
}

BOOL CTDLTestBase::ExpectNE(int n1, int n2) const
{
	return ExpectCompareT(n1, _T("%d"), n2, _T("%d"), OP_NE);
}

// -------------------------------------------------------------------------------------------------------

BOOL CTDLTestBase::ExpectEQ(long l1, long l2) const
{
	return ExpectCompareT(l1, _T("%ld"), l2, _T("%ld"), OP_EQ);
}

BOOL CTDLTestBase::ExpectNE(long l1, long l2) const
{
	return ExpectCompareT(l1, _T("%ld"), l2, _T("%ld"), OP_NE);
}

// -------------------------------------------------------------------------------------------------------

BOOL CTDLTestBase::ExpectEQ(__int64 l1, __int64 l2) const
{
	return ExpectCompareT(l1, _T("%lld"), l2, _T("%lld"), OP_EQ);
}

BOOL CTDLTestBase::ExpectNE(__int64 l1, __int64 l2) const
{
	return ExpectCompareT(l1, _T("%lld"), l2, _T("%lld"), OP_NE);
}

// -------------------------------------------------------------------------------------------------------

BOOL CTDLTestBase::ExpectEQ(unsigned char c1, unsigned char c2) const
{
	return ExpectCompareT(c1, _T("%uc"), c2, _T("%uc"), OP_EQ);
}

BOOL CTDLTestBase::ExpectNE(unsigned char c1, unsigned char c2) const
{
	return ExpectCompareT(c1, _T("%uc"), c2, _T("%uc"), OP_NE);
}

// -------------------------------------------------------------------------------------------------------

BOOL CTDLTestBase::ExpectEQ(unsigned short n1, unsigned short n2) const
{
	return ExpectCompareT(n1, _T("%ud"), n2, _T("%ud"), OP_EQ);
}

BOOL CTDLTestBase::ExpectNE(unsigned short n1, unsigned short n2) const
{
	return ExpectCompareT(n1, _T("%ud"), n2, _T("%ud"), OP_NE);
}

// -------------------------------------------------------------------------------------------------------

BOOL CTDLTestBase::ExpectEQ(unsigned int n1, unsigned int n2) const
{
	return ExpectCompareT(n1, _T("%u"), n2, _T("%u"), OP_EQ);
}

BOOL CTDLTestBase::ExpectNE(unsigned int n1, unsigned int n2) const
{
	return ExpectCompareT(n1, _T("%u"), n2, _T("%u"), OP_NE);
}

// -------------------------------------------------------------------------------------------------------

BOOL CTDLTestBase::ExpectEQ(unsigned long l1, unsigned long l2) const
{
	return ExpectCompareT(l1, _T("%ul"), l2, _T("%ul"), OP_EQ);
}

BOOL CTDLTestBase::ExpectNE(unsigned long l1, unsigned long l2) const
{
	return ExpectCompareT(l1, _T("%ul"), l2, _T("%ul"), OP_NE);
}

// -------------------------------------------------------------------------------------------------------

BOOL CTDLTestBase::ExpectEQ(unsigned __int64 l1, unsigned __int64 l2) const
{
	return ExpectCompareT(l1, _T("%ulld"), l2, _T("%ulld"), OP_EQ);
}

BOOL CTDLTestBase::ExpectNE(unsigned __int64 l1, unsigned __int64 l2) const
{
	return ExpectCompareT(l1, _T("%ulld"), l2, _T("%ulld"), OP_NE);
}

// -------------------------------------------------------------------------------------------------------

BOOL CTDLTestBase::ExpectEQ(float f1, float f2, float fTol) const
{
	return ExpectCompareT(f1, _T("%f"), f2, _T("%f"), fTol, OP_EQ);
}

BOOL CTDLTestBase::ExpectNE(float f1, float f2, float fTol) const
{
	return ExpectCompareT(f1, _T("%f"), f2, _T("%f"), fTol, OP_NE);
}

// -------------------------------------------------------------------------------------------------------

BOOL CTDLTestBase::ExpectEQ(double d1, double d2, double dTol) const
{
	return ExpectCompareT(d1, _T("%lf"), d2, _T("%lf"), dTol, OP_EQ);
}

BOOL CTDLTestBase::ExpectNE(double d1, double d2, double dTol) const
{
	return ExpectCompareT(d1, _T("%lf"), d2, _T("%lf"), dTol, OP_NE);
}

// -------------------------------------------------------------------------------------------------------

BOOL CTDLTestBase::ExpectEQ(const CString& s1, const CString& s2, BOOL bCaseSensitive) const
{
	return ExpectEQ((LPCTSTR)s1, (LPCTSTR)s2, bCaseSensitive);
}

BOOL CTDLTestBase::ExpectNE(const CString& s1, const CString& s2, BOOL bCaseSensitive) const
{
	return ExpectNE((LPCTSTR)s1, (LPCTSTR)s2, bCaseSensitive);
}

// -------------------------------------------------------------------------------------------------------

BOOL CTDLTestBase::ExpectEQ(LPCTSTR sz1, LPCTSTR sz2, BOOL bCaseSensitive) const
{
	return ExpectCompare(sz1, _T("%s"), sz2, _T("%s"), bCaseSensitive, OP_EQ);
}

BOOL CTDLTestBase::ExpectNE(LPCTSTR sz1, LPCTSTR sz2, BOOL bCaseSensitive) const
{
	return ExpectCompare(sz1, _T("%s"), sz2, _T("%s"), bCaseSensitive, OP_NE);
}

// -------------------------------------------------------------------------------------------------------

BOOL CTDLTestBase::ExpectEQ(const CStringArray& a1, const CStringArray& a2, BOOL bOrderSensitive, BOOL bCaseSensitive) const
{
	return ExpectCompare(a1, a2, bOrderSensitive, bCaseSensitive, OP_EQ);
}

BOOL CTDLTestBase::ExpectNE(const CStringArray& a1, const CStringArray& a2, BOOL bOrderSensitive, BOOL bCaseSensitive) const
{
	return ExpectCompare(a1, a2, bOrderSensitive, bCaseSensitive, OP_NE);
}

// -------------------------------------------------------------------------------------------------------

BOOL CTDLTestBase::ExpectEQ(const CDWordArray& a1, const CDWordArray& a2, BOOL bOrderSensitive) const
{
	return ExpectCompare(a1, a2, bOrderSensitive, OP_EQ);
}

BOOL CTDLTestBase::ExpectNE(const CDWordArray& a1, const CDWordArray& a2, BOOL bOrderSensitive) const
{
	return ExpectCompare(a1, a2, bOrderSensitive, OP_NE);
}

// -------------------------------------------------------------------------------------------------------

BOOL CTDLTestBase::ExpectFileContentsEQ(const CString& sFile1, const CString& sFile2) const
{
	return ExpectCompareFileContents(sFile1, sFile2, OP_EQ);
}

BOOL CTDLTestBase::ExpectFileContentsNE(const CString& sFile1, const CString& sFile2) const
{
	return ExpectCompareFileContents(sFile1, sFile2, OP_NE);
}

// -------------------------------------------------------------------------------------------------------

BOOL CTDLTestBase::ExpectCompare(LPCTSTR sz1, LPCTSTR szFmt1, LPCTSTR sz2, LPCTSTR szFmt2, BOOL bCaseSensitive, TEST_OP nOp) const
{
	LPCTSTR szTrail(bCaseSensitive ? _T("Case sensitive") : _T("Case insensitive"));

	return HandleCompareResult(sz1, szFmt1, sz2, szFmt2, nOp, m_utils.Compare(sz1, sz2, bCaseSensitive), szTrail);
}

BOOL CTDLTestBase::ExpectCompareFileContents(const CString& sFile, const CString& sControl, TEST_OP nOp) const
{
	ASSERT(FileMisc::FileExists(sFile) && FileMisc::IsPath(sControl));

	if (!FileMisc::FileExists(sFile))
		return FALSE;

	if (!FileMisc::IsPath(sControl))
		return FALSE;

	if (!FileMisc::FileExists(sControl)) // first time
	{
		if (!FileMisc::CopyFile(sFile, sControl))
			return FALSE;

		return TRUE;
	}

	return HandleCompareResult(sFile, _T("%s"), sControl, _T("%s"), nOp, FileMisc::CompareContents(sFile, sControl), _T("File contents"));
}

BOOL CTDLTestBase::ExpectCompare(const CStringArray& a1, const CStringArray& a2, BOOL bOrderSensitive, BOOL bCaseSensitive, TEST_OP nOp) const
{
	int nCmp = 99; // sentinel value

	switch (nOp)
	{
		case OP_EQ:
			if (Misc::MatchAll(a1, a2, bOrderSensitive, bCaseSensitive))
				nCmp = 0;
			break;

		case OP_NE: 
			if (!Misc::MatchAll(a1, a2, bOrderSensitive, bCaseSensitive))
				nCmp = 1;
			break;
			
		default: ASSERT(0);
			return FALSE;
	}

	ASSERT(nCmp != 99);

	CString sTrail;
	
	if (bOrderSensitive)
	{
		if (bCaseSensitive)
			sTrail = _T("Order & Case sensitive");
		else
			sTrail = _T("Order sensitive, Case insensitive");
	}
	else if (bCaseSensitive)
	{
		sTrail = _T("Order Insensitive, Case sensitive");
	}
	else
	{
		sTrail = _T("Order & Case insensitive");
	}

	return HandleCompareResult(Misc::FormatArray(a1), _T("%s"), Misc::FormatArray(a2), _T("%s"), nOp, nCmp, sTrail);
}

BOOL CTDLTestBase::ExpectCompare(const CDWordArray& a1, const CDWordArray& a2, BOOL bOrderSensitive, TEST_OP nOp) const
{
	int nCmp = 99; // sentinel value

	switch (nOp)
	{
		case OP_EQ:
			if (Misc::MatchAll(a1, a2, bOrderSensitive))
				nCmp = 0;
			break;

		case OP_NE: 
			if (!Misc::MatchAll(a1, a2, bOrderSensitive))
				nCmp = 1;
			break;
			
		default: ASSERT(0);
			return FALSE;
	}

	ASSERT(nCmp != 99);

	LPCTSTR szTrail(bOrderSensitive ? _T("Order sensitive") :  _T("Order insensitive"));

	return HandleCompareResult(Misc::FormatArray(a1), _T("%s"), Misc::FormatArray(a2), _T("%s"), nOp, nCmp, szTrail);
}

BOOL CTDLTestBase::SelfTest()
{
	CTDLTestSelfTest test;
	
	test.Run();

	return !test.GetErrorCount(TRUE);
}

// -------------------------------------------------------------------------------------------------------
