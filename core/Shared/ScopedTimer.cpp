// ScopedTimer.cpp: implementation of the CScopedTimer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScopedTimer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScopedTimer::CScopedTimer(LPCTSTR szScope) 
	: 
	m_sScope(szScope), 
	m_dwTickStart(GetTickCount())
{
}

CScopedTimer::~CScopedTimer()
{
	TRACE(_T("%s took %d ms)\n"), m_sScope, (GetTickCount() - m_dwTickStart));
}
