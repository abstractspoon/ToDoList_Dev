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
	m_sScope = Format(szScope, szArg1, szArg2, szArg3);
	m_dwTickStart = m_dwIntermediateStart = ::GetTickCount();
}

CScopedTimer::~CScopedTimer()
{
}

CString CScopedTimer::FormatStart() const
{
	if (!m_sScope.IsEmpty())
		return Format(_T("%s - Start"), m_sScope, NULL, NULL);

	return _T("");
}

CString CScopedTimer::FormatEnd() const
{
	if (!m_sScope.IsEmpty())
		return Format(_T("%s - End"), m_sScope, NULL, NULL);

	return _T("");
}

CString CScopedTimer::FormatTimeElapsed(LPCTSTR szScope, DWORD& dwTickFrom)
{
	ASSERT(!Misc::IsEmpty(szScope));

	DWORD dwTickNow = GetTickCount();
	CString sElapsed = Misc::Format(dwTickNow - dwTickFrom);

	dwTickFrom = dwTickNow;

	return Format(_T("%s took %s ms"), szScope, sElapsed, NULL);
}

CString CScopedTimer::Format(LPCTSTR szScope, LPCTSTR szArg1, LPCTSTR szArg2, LPCTSTR szArg3)
{
	CString sScope;

	if (szScope)
	{
		if (szArg1 && szArg2 && szArg3)
		{
			sScope.Format(szScope, szArg1, szArg2, szArg3);
		}
		else if (szArg1 && szArg2)
		{
			sScope.Format(szScope, szArg1, szArg2);
		}
		else if (szArg1)
		{
			sScope.Format(szScope, szArg1);
		}
		else
		{
			sScope = szScope;
		}
	}

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
		CString sScope = Format(szSubScope, szArg1, szArg2, szArg3);
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

void CScopedLogTimer::LogEnd()
{
	if (FileMisc::IsLoggingEnabled() && !m_sScope.IsEmpty())
	{
		FileMisc::LogTextRaw(FormatEnd());
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

void CScopedTraceTimer::TraceEnd()
{
#ifdef _DEBUG
	if (!m_sScope.IsEmpty())
	{
		TRACE(FormatEnd());
		TRACE(_T("\n"));
	}
#endif
}

void CScopedTraceTimer::TraceTimeElapsed(LPCTSTR szSubScope, LPCTSTR szArg1, LPCTSTR szArg2, LPCTSTR szArg3)
{
#ifdef _DEBUG
	if (!Misc::IsEmpty(szSubScope))
	{
		CString sScope = Format(szSubScope, szArg1, szArg2, szArg3);
		CString sMessage = FormatTimeElapsed(sScope, m_dwIntermediateStart);

		TRACE(sMessage);
		TRACE(_T("\n"));
	}
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
