// LimitSingleInstance.h: interface for the CLimitSingleInstance class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIMITSINGLEINSTANCE_H__14CD9485_CDA3_4146_9AB9_B3CDC0C2568B__INCLUDED_)
#define AFX_LIMITSINGLEINSTANCE_H__14CD9485_CDA3_4146_9AB9_B3CDC0C2568B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <windows.h> 

//this code is from Q243953 in case you lose the article and wonder
//where this code came from...
class CLimitSingleInstance
{
protected:
	CString m_strMutexName;
	DWORD  m_dwLastError;
	HANDLE m_hMutex;
	
public:
	CLimitSingleInstance(TCHAR *strMutexName) 
		: 
		m_strMutexName(strMutexName), 
		m_hMutex(NULL), 
		m_dwLastError(0)
	{
		m_hMutex = CreateMutex(NULL, FALSE, m_strMutexName); //do early
		m_dwLastError = GetLastError(); //save for use later...
	}
	
	~CLimitSingleInstance() 
	{
		if (m_hMutex)  //don't forget to close handles...
		{
			CloseHandle(m_hMutex); //do as late as possible
			m_hMutex = NULL; //good habit to be in
		}
	}
	
	BOOL IsAnotherInstanceRunning(BOOL bTryAgain = FALSE) 
	{
		if (bTryAgain && (m_hMutex == NULL))
		{
			m_hMutex = CreateMutex(NULL, FALSE, m_strMutexName);
			m_dwLastError = GetLastError();
		}

		return (ERROR_ALREADY_EXISTS == m_dwLastError);
	}
};
#endif // !defined(AFX_LIMITSINGLEINSTANCE_H__14CD9485_CDA3_4146_9AB9_B3CDC0C2568B__INCLUDED_)
