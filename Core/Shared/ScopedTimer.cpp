#include "stdafx.h"
#include "ScopedTimer.h"
#include "misc.h"
#include "filemisc.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

CScopedTimer::CScopedTimer()
{
	m_dwTickStart = m_dwIntermediateStart = ::GetTickCount();
}

CScopedTimer::CScopedTimer(LPCTSTR szScope, LPCTSTR szArg1, LPCTSTR szArg2, LPCTSTR szArg3)
{
	m_sScope = FormatScope(szScope, szArg1, szArg2, szArg3);
	m_dwTickStart = m_dwIntermediateStart = ::GetTickCount();
}

CScopedTimer::~CScopedTimer()
{
}

CString CScopedTimer::FormatStart() const
{
	CString sMessage;
	sMessage.Format(_T("%s - Start"), m_sScope);

	return sMessage;
}

CString CScopedTimer::FormatTimeElapsed(LPCTSTR szScope, DWORD& dwTickFrom)
{
	ASSERT(!Misc::IsEmpty(szScope));

	DWORD dwTickNow = GetTickCount();

	CString sMessage;
	sMessage.Format(_T("%s took %lu ms"), szScope, (dwTickNow - dwTickFrom));

	dwTickFrom = dwTickNow;

	return sMessage;
}

CString CScopedTimer::FormatScope(LPCTSTR szScope, LPCTSTR szArg1, LPCTSTR szArg2, LPCTSTR szArg3)
{
	ASSERT(!Misc::IsEmpty(szScope));

	CString sScope;
	sScope.Format(szScope, szArg1, szArg2, szArg3);

	return sScope;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

CScopedLogTimer::CScopedLogTimer() 
	: 
	CScopedTimer()
{
}

CScopedLogTimer::CScopedLogTimer(LPCTSTR szScope, LPCTSTR szArg1, LPCTSTR szArg2, LPCTSTR szArg3)
	:
	CScopedTimer(szScope, szArg1, szArg2, szArg3)
{
}
	
CScopedLogTimer::~CScopedLogTimer()
{
	if (FileMisc::IsLoggingEnabled() && !m_sScope.IsEmpty())
	{
		FileMisc::LogTextRaw(FormatTimeElapsed(m_sScope, m_dwTickStart));
	}
}

void CScopedLogTimer::LogTimeElapsed(LPCTSTR szSubScope, LPCTSTR szArg1, LPCTSTR szArg2, LPCTSTR szArg3)
{
	if (FileMisc::IsLoggingEnabled() && !Misc::IsEmpty(szSubScope))
	{
		CString sScope = FormatScope(szSubScope, szArg1, szArg2, szArg3);
		CString sMessage = FormatTimeElapsed(sScope, m_dwIntermediateStart);

		FileMisc::LogTextRaw(sMessage);
	}
}

void CScopedLogTimer::LogStart()
{
	if (FileMisc::IsLoggingEnabled() && !m_sScope.IsEmpty())
	{
		FileMisc::LogTextRaw(FormatStart());
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

CScopedTraceTimer::CScopedTraceTimer()
	: 
	CScopedTimer()
{
}

CScopedTraceTimer::CScopedTraceTimer(LPCTSTR szScope, LPCTSTR szArg1, LPCTSTR szArg2, LPCTSTR szArg3)
	:
	CScopedTimer(szScope, szArg1, szArg2, szArg3)
{
}
	
CScopedTraceTimer::~CScopedTraceTimer()
{
#ifdef _DEBUG
	if (!m_sScope.IsEmpty())
	{
		TRACE(FormatTimeElapsed(m_sScope, m_dwTickStart));
		TRACE(_T("\n"));
	}
#endif
}

void CScopedTraceTimer::TraceStart()
{
#ifdef _DEBUG
	if (!m_sScope.IsEmpty())
	{
		TRACE(FormatStart());
		TRACE(_T("\n"));
	}
#endif
}

void CScopedTraceTimer::TraceTimeElapsed(LPCTSTR szSubScope, LPCTSTR szArg1, LPCTSTR szArg2, LPCTSTR szArg3)
{
#ifdef _DEBUG
	if (!Misc::IsEmpty(szSubScope))
	{
		CString sScope = FormatScope(szSubScope, szArg1, szArg2, szArg3);
		CString sMessage = FormatTimeElapsed(sScope, m_dwIntermediateStart);

		TRACE(sMessage);
		TRACE(_T("\n"));
	}
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
