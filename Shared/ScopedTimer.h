// ScopedTimer.h: interface for the CScopedTimer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCOPEDTIMER_H__602734CB_5DB8_4001_856E_83043DBFE960__INCLUDED_)
#define AFX_SCOPEDTIMER_H__602734CB_5DB8_4001_856E_83043DBFE960__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CScopedTimer  
{
public:
	CScopedTimer(LPCTSTR szScope);
	virtual ~CScopedTimer();

protected:
	CString m_sScope;
	DWORD m_dwTickStart;

};

#endif // !defined(AFX_SCOPEDTIMER_H__602734CB_5DB8_4001_856E_83043DBFE960__INCLUDED_)
