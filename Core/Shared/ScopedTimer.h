#ifndef AFX_SCOPEDTIME_H__155791A3_22AC_4083_B933_F39E9EBDADEF__INCLUDED_
#define AFX_SCOPEDTIME_H__155791A3_22AC_4083_B933_F39E9EBDADEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////////////////////////

class CScopedTimer
{
public:
	virtual ~CScopedTimer();

protected:
	DWORD m_dwTickStart, m_dwIntermediateStart;
	CString m_sScope;
	BOOL m_bStartLogged;

protected:
	CScopedTimer();
	CScopedTimer(LPCTSTR szScope, LPCTSTR szArg1, LPCTSTR szArg2, LPCTSTR szArg3);

	CString FormatStart() const;

protected:
	static CString FormatTimeElapsed(LPCTSTR szScope, DWORD& dwTickFrom, BOOL bLogEnd);
	static CString Format(LPCTSTR szScope, LPCTSTR szArg1, LPCTSTR szArg2, LPCTSTR szArg3);
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class CScopedLogTimer : public CScopedTimer
{
public:
	CScopedLogTimer();
	CScopedLogTimer(LPCTSTR szScope, LPCTSTR szArg1 = NULL, LPCTSTR szArg2 = NULL, LPCTSTR szArg3 = NULL);
	~CScopedLogTimer();

	void LogStart();
	void LogTimeElapsed(LPCTSTR szSubScope, LPCTSTR szArg1 = NULL, LPCTSTR szArg2 = NULL, LPCTSTR szArg3 = NULL);
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class CScopedTraceTimer : public CScopedTimer
{
public:
	CScopedTraceTimer();
	CScopedTraceTimer(LPCTSTR szScope, LPCTSTR szArg1 = NULL, LPCTSTR szArg2 = NULL, LPCTSTR szArg3 = NULL);
	~CScopedTraceTimer();

	void TraceStart();
	void TraceTimeElapsed(LPCTSTR szSubScope, LPCTSTR szArg1 = NULL, LPCTSTR szArg2 = NULL, LPCTSTR szArg3 = NULL);
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class CScopedConsoleTimer : public CScopedTimer
{
public:
	CScopedConsoleTimer();
	CScopedConsoleTimer(LPCTSTR szScope, LPCTSTR szArg1 = NULL, LPCTSTR szArg2 = NULL, LPCTSTR szArg3 = NULL);
	~CScopedConsoleTimer();

	void OutputStart();
	void OutputTimeElapsed(LPCTSTR szSubScope, LPCTSTR szArg1 = NULL, LPCTSTR szArg2 = NULL, LPCTSTR szArg3 = NULL);
};

///////////////////////////////////////////////////////////////////////////////////////////////////


#endif // AFX_SCOPEDTIME_H__155791A3_22AC_4083_B933_F39E9EBDADEF__INCLUDED_