#include "stdafx.h"
#include "ScopedTimer.h"
#include "misc.h"
#include "filemisc.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

CScopedTimer::CScopedTimer() 
	: 
	m_bStartLogged(FALSE)
{
	m_dwTickStart = m_dwIntermediateStart = ::GetTickCount();
}

CScopedTimer::CScopedTimer(LPCTSTR szScope, LPCTSTR szArg1, LPCTSTR szArg2, LPCTSTR szArg3)
	: 
	m_bStartLogged(FALSE)
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
		return Format(_T("\n%s - Start ----------------------"), m_sScope, NULL, NULL);

	return _T("");
}

CString CScopedTimer::FormatTimeElapsed(LPCTSTR szScope, DWORD& dwTickFrom, BOOL bLogEnd)
{
	ASSERT(!Misc::IsEmpty(szScope));

	DWORD dwTickNow = GetTickCount();
	CString sElapsed = Misc::Format(dwTickNow - dwTickFrom);

	dwTickFrom = dwTickNow;

	if (bLogEnd)
		return Format(_T("%s took %s ms ------------------\n"), szScope, sElapsed, NULL);

	// else
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
		FileMisc::LogTextRaw(FormatTimeElapsed(m_sScope, m_dwTickStart, m_bStartLogged));
	}
}

void CScopedLogTimer::LogTimeElapsed(LPCTSTR szSubScope, LPCTSTR szArg1, LPCTSTR szArg2, LPCTSTR szArg3)
{
	if (FileMisc::IsLoggingEnabled() && !Misc::IsEmpty(szSubScope))
	{
		CString sScope = Format(szSubScope, szArg1, szArg2, szArg3);
		CString sMessage = FormatTimeElapsed(sScope, m_dwIntermediateStart, FALSE);

		FileMisc::LogTextRaw(sMessage);
	}
}

void CScopedLogTimer::LogStart()
{
	if (FileMisc::IsLoggingEnabled() && !m_sScope.IsEmpty())
	{
		FileMisc::LogTextRaw(FormatStart());

		m_bStartLogged = TRUE;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

CScopedConsoleTimer::CScopedConsoleTimer()
	: 
	CScopedTimer()
{
}

CScopedConsoleTimer::CScopedConsoleTimer(LPCTSTR szScope, LPCTSTR szArg1, LPCTSTR szArg2, LPCTSTR szArg3)
	:
	CScopedTimer(szScope, szArg1, szArg2, szArg3)
{
}
	
CScopedConsoleTimer::~CScopedConsoleTimer()
{
	if (!m_sScope.IsEmpty())
		_tprintf(FormatTimeElapsed(m_sScope, m_dwTickStart, m_bStartLogged));
}

void CScopedConsoleTimer::OutputTimeElapsed(LPCTSTR szSubScope, LPCTSTR szArg1, LPCTSTR szArg2, LPCTSTR szArg3)
{
	if (!Misc::IsEmpty(szSubScope))
	{
		CString sScope = Format(szSubScope, szArg1, szArg2, szArg3);
		CString sMessage = FormatTimeElapsed(sScope, m_dwIntermediateStart, FALSE);

		_tprintf(sMessage);
	}
}

void CScopedConsoleTimer::OutputStart()
{
	if (!m_sScope.IsEmpty())
	{
		_tprintf(FormatStart());

		m_bStartLogged = TRUE;
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
		TRACE(_T("\n") + FormatTimeElapsed(m_sScope, m_dwTickStart, m_bStartLogged));
		TRACE(_T("\n"));
	}
#endif
}

void CScopedTraceTimer::TraceStart()
{
#ifdef _DEBUG
	if (!m_sScope.IsEmpty())
	{
		TRACE(_T("\n") + FormatStart());
		TRACE(_T("\n"));

		m_bStartLogged = TRUE;
	}
#endif
}

void CScopedTraceTimer::TraceTimeElapsed(LPCTSTR szSubScope, LPCTSTR szArg1, LPCTSTR szArg2, LPCTSTR szArg3)
{
#ifdef _DEBUG
	if (!Misc::IsEmpty(szSubScope))
	{
		CString sScope = Format(szSubScope, szArg1, szArg2, szArg3);
		CString sMessage = FormatTimeElapsed(sScope, m_dwIntermediateStart, FALSE);

		TRACE(_T("\n") + sMessage);
		TRACE(_T("\n"));
	}
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
