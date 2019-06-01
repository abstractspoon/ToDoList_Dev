// TDLTestBase.h: interface for the CTDLTestBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDLTESTBASE_H__58880D2A_3570_4D67_8955_7B656A0B3A45__INCLUDED_)
#define AFX_TDLTESTBASE_H__58880D2A_3570_4D67_8955_7B656A0B3A45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

class CTestUtils
{
	friend class CTDLTestSelfTest;

public:
	CTestUtils();
	CTestUtils(const CTestUtils& utils);
	
	BOOL Initialise(const CString& sOutputDir, const CString& sControlDir);
	
	CString GetOutputFilePath(const CString& sSubDir, const CString& sFilename, const CString& sExt) const;
	CString GetControlFilePath(const CString& sSubDir, const CString& sFilename, const CString& sExt) const;

	template <class T> 
	int CompareT(T t1, T t2) const
	{
		return ((t1 == t2) ? 0 : ((t1 < t2) ? -1 : 1));
	}

	int Compare(float f1, float f2, float fTol = 1e-6) const;
	int Compare(double d1, double d2, double dTol = 1e-12) const;
	
	int Compare(const CString& s1, const CString& s2, BOOL bCaseSensitive = TRUE) const;
	int Compare(LPCTSTR sz1, LPCTSTR sz2, BOOL bCaseSensitive = TRUE) const;

protected:
	CString m_sOutputDir;
	CString m_sControlDir;

	static CString GetFilePath(const CString& sRoot, const CString& sSubDir, 
								const CString& sFilename, const CString& sExt);

};

//////////////////////////////////////////////////////////////////////

class CTDLTestBase
{
	friend class CTDLTestSelfTest;

public:
	virtual ~CTDLTestBase();
	
	virtual void Run() = 0; // derived must override

	static BOOL SelfTest();
	
protected:
	CTDLTestBase(const CTestUtils& utils);

	BOOL BeginTest(LPCTSTR szTest);
	BOOL EndTest();

	UINT GetSuccessCount(BOOL bTotal) const { return (bTotal ? m_nTotalSuccess : m_nTestSuccess); }
	UINT GetErrorCount(BOOL bTotal) const { return (bTotal ? m_nTotalError : m_nTestError); }
	UINT GetTestCount(BOOL bTotal) const { return (bTotal ? (m_nTotalError + m_nTotalSuccess) : (m_nTestError + m_nTestSuccess)); }

	BOOL ExpectTrue(bool b) const;
	BOOL ExpectTrue(BOOL b) const;
	
	BOOL ExpectFalse(bool b) const;
	BOOL ExpectFalse(BOOL b) const;

	BOOL ExpectEmpty(const CString& s1) const;
	BOOL ExpectEmpty(LPCTSTR sz1) const;

	BOOL ExpectNotEmpty(const CString& s1) const;
	BOOL ExpectNotEmpty(LPCTSTR sz1) const;
	
	BOOL ExpectEQ(char c1, char c2) const;
	BOOL ExpectEQ(short n1, short n2) const;
	BOOL ExpectEQ(int n1, int n2) const;
	BOOL ExpectEQ(long l1, long l2) const;
	BOOL ExpectEQ(__int64 l1, __int64 l2) const;
	
	BOOL ExpectEQ(unsigned char c1, unsigned char c2) const;
	BOOL ExpectEQ(unsigned short n1, unsigned short n2) const;
	BOOL ExpectEQ(unsigned int n1, unsigned int n2) const;
	BOOL ExpectEQ(unsigned long l1, unsigned long l2) const;
	BOOL ExpectEQ(unsigned __int64 l1, unsigned __int64 l2) const;
	
	BOOL ExpectEQ(float f1, float f2, float fTol = 1e-6) const;
	BOOL ExpectEQ(double d1, double d2, double dTol = 1e-12) const;
	
	BOOL ExpectEQ(bool b1, bool b2) const;
	
	BOOL ExpectEQ(const CString& s1, const CString& s2, BOOL bCaseSensitive = TRUE) const;
	BOOL ExpectEQ(LPCTSTR sz1, LPCTSTR sz2, BOOL bCaseSensitive = TRUE) const;

	BOOL ExpectEQ(const CStringArray& a1, const CStringArray& a2, BOOL bOrderSensitive = FALSE, BOOL bCaseSensitive = TRUE) const;
	BOOL ExpectEQ(const CDWordArray& a1, const CDWordArray& a2, BOOL bOrderSensitive = FALSE) const;

	BOOL ExpectFileContentsEQ(const CString& sFile1, const CString& sFile2) const;
	
	BOOL ExpectNE(char c1, char c2) const;
	BOOL ExpectNE(short n1, short n2) const;
	BOOL ExpectNE(int n1, int n2) const;
	BOOL ExpectNE(long l1, long l2) const;
	BOOL ExpectNE(__int64 l1, __int64 l2) const;
	
	BOOL ExpectNE(unsigned char c1, unsigned char c2) const;
	BOOL ExpectNE(unsigned short n1, unsigned short n2) const;
	BOOL ExpectNE(unsigned int n1, unsigned int n2) const;
	BOOL ExpectNE(unsigned long l1, unsigned long l2) const;
	BOOL ExpectNE(unsigned __int64 l1, unsigned __int64 l2) const;
	
	BOOL ExpectNE(float f1, float f2, float fTol = 1e-6) const;
	BOOL ExpectNE(double d1, double d2, double dTol = 1e-12) const;
	
	BOOL ExpectNE(bool b1, bool b2) const;
	
	BOOL ExpectNE(const CString& s1, const CString& s2, BOOL bCaseSensitive = TRUE) const;
	BOOL ExpectNE(LPCTSTR sz1, LPCTSTR sz2, BOOL bCaseSensitive = TRUE) const;
	
	BOOL ExpectNE(const CStringArray& a1, const CStringArray& a2, BOOL bOrderSensitive = FALSE, BOOL bCaseSensitive = TRUE) const;
	BOOL ExpectNE(const CDWordArray& a1, const CDWordArray& a2, BOOL bOrderSensitive = FALSE) const;
	
	BOOL ExpectFileContentsNE(const CString& sFile1, const CString& sFile2) const;
	
protected:
	const CTestUtils m_utils;

private:
	mutable UINT m_nTestError, m_nTestSuccess, m_nCurTest;
	UINT m_nTotalError, m_nTotalSuccess;
	CString m_sCurrentTest;

private:
	CTDLTestBase() {} // for self test

	enum TEST_OP
	{
		OP_EQ,
		OP_NE,
		OP_LT,
		OP_GT,
		OP_LE,
		OP_GE,
	};

	BOOL ExpectCompare(LPCTSTR sz1, LPCTSTR szFmt1, LPCTSTR sz2, LPCTSTR szFmt2, BOOL bCaseSensitive, TEST_OP nOp) const;
	BOOL ExpectCompare(const CStringArray& a1, const CStringArray& a2, BOOL bOrderSensitive, BOOL bCaseSensitive, TEST_OP nOp) const;
	BOOL ExpectCompare(const CDWordArray& a1, const CDWordArray& a2, BOOL bOrderSensitive, TEST_OP nOp) const;

	BOOL ExpectCompareFileContents(const CString& sFile1, const CString& sFile2, TEST_OP nOp) const;

	template <class T> 
	BOOL ExpectCompareT(T t1, LPCTSTR szFmt1, T t2, LPCTSTR szFmt2, TEST_OP nOp) const
	{
		return HandleCompareResult(t1, szFmt1, t2, szFmt2, nOp, m_utils.CompareT(t1, t2));
	}

	// specifically for float and double
	template <class T> 
	BOOL ExpectCompareT(T t1, LPCTSTR szFmt1, T t2, LPCTSTR szFmt2, T tol, TEST_OP nOp) const
	{
		CString sTrail;

		if (tol != 0.0)
			sTrail.Format(_T("Tolerance = %e"), tol);
		else
			sTrail = _T("No tolerance");

		return HandleCompareResult(t1, szFmt1, t2, szFmt2, nOp, m_utils.Compare(t1, t2, tol), sTrail);
	}

	template <class T> 
	BOOL HandleCompareResult(T t1, LPCTSTR szFmt1, T t2, LPCTSTR szFmt2, TEST_OP nOp, int nCmp, LPCTSTR szTrail = _T("")) const
	{
		m_nCurTest++;
		BOOL bSuccess = FALSE;

		switch (nOp)
		{
		case OP_EQ: bSuccess = (nCmp == 0); break;
		case OP_NE: bSuccess = (nCmp != 0); break;
		case OP_LT: bSuccess = (nCmp <  0); break;
		case OP_GT: bSuccess = (nCmp >  0); break;
		case OP_LE: bSuccess = (nCmp <= 0); break;
		case OP_GE: bSuccess = (nCmp >= 0); break;

		default: ASSERT(0);
		}

		if (bSuccess)
			m_nTestSuccess++;
		else
			m_nTestError++;
		
		return OutputExpectResult(t1, szFmt1, t2, szFmt2, nOp, bSuccess, szTrail);
	}

	template <class T> 
	BOOL OutputExpectResult(T t1, LPCTSTR szFmt1, T t2, LPCTSTR szFmt2, TEST_OP nOp, BOOL bSuccess, LPCTSTR szTrail) const 
	{
		CString sOp, sOutput;

		switch (nOp)
		{
		case OP_EQ: sOp = _T("=="); break;
		case OP_NE: sOp = _T("!="); break;
		case OP_LT: sOp = _T("< "); break;
		case OP_GT: sOp = _T("> "); break;
		case OP_LE: sOp = _T("<="); break;
		case OP_GE: sOp = _T(">="); break;
		}

		ASSERT(!sOp.IsEmpty());

		if (bSuccess)
			sOutput.Format(_T("  Test [%2d] succeeded:          \"%s\" %s \"%s\""), m_nCurTest, szFmt1, sOp, szFmt2);
		else
			sOutput.Format(_T("  Test [%2d] failed:    Expected \"%s\" %s \"%s\""), m_nCurTest, szFmt1, sOp, szFmt2);

		if (!Misc::IsEmpty(szTrail))
		{
			CString sTrail;
			sTrail.Format(_T(" (%s)\n"), szTrail);

			sOutput += sTrail;
		}
		else
		{
			sOutput += '\n';
		}

		_tprintf(sOutput, t1, t2);

		return bSuccess;
	}
	
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_TDLTESTBASE_H__58880D2A_3570_4D67_8955_7B656A0B3A45__INCLUDED_)
