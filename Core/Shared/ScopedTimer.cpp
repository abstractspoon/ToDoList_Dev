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
	if (!m_sScope.IsEmpty())
	{
		CString sMessage = FormatTimeElapsed(m_sScope, m_dwTickStart);
		OnScopeEnded(sMessage);
	}
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

void CScopedLogTimer::LogTimeElapsed(LPCTSTR szSubScope, LPCTSTR szArg1, LPCTSTR szArg2, LPCTSTR szArg3)
{
	if (FileMisc::IsLoggingEnabled() && !Misc::IsEmpty(szSubScope))
	{
		CString sScope = FormatScope(szSubScope, szArg1, szArg2, szArg3);
		CString sMessage = FormatTimeElapsed(sScope, m_dwIntermediateStart);

		FileMisc::LogTextRaw(sMessage);
	}
}

void CScopedLogTimer::OnScopeEnded(const CString& sMessage)
{
	if (FileMisc::IsLoggingEnabled())
	{
		FileMisc::LogTextRaw(sMessage);
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

void CScopedTraceTimer::TraceTimeElapsed(LPCTSTR szSubScope, LPCTSTR szArg1, LPCTSTR szArg2, LPCTSTR szArg3)
{
#ifdef _DEBUG
	if (!Misc::IsEmpty(szSubScope))
	{
		CString sScope = FormatScope(szSubScope, szArg1, szArg2, szArg3);
		CString sMessage = FormatTimeElapsed(sScope, m_dwIntermediateStart);

		TRACE(sMessage);
		TRACE('\n');
	}
#endif
}

void CScopedTraceTimer::OnScopeEnded(const CString& sMessage)
{
	TRACE(sMessage);
	TRACE('\n');
}

///////////////////////////////////////////////////////////////////////////////////////////////////
