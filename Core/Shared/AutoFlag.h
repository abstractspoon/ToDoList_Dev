// AutoFlag.h: interface for the CAutoFlag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOFLAG_H__EEBD725A_2C54_4554_96D8_E4CCDC940258__INCLUDED_)
#define AFX_AUTOFLAG_H__EEBD725A_2C54_4554_96D8_E4CCDC940258__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

template <class T>
class CAutoFlagT  
{
public:
	CAutoFlagT(T& tFlag, T tState) : m_tFlag(tFlag), m_tInitialState(tFlag)
	{
		m_tFlag = tState;
	}

	virtual ~CAutoFlagT()
	{
		m_tFlag = m_tInitialState;
	}

protected:
	T& m_tFlag;
	T m_tInitialState;

};

typedef CAutoFlagT<BOOL> CAutoFlag;

//////////////////////////////////////////////////////////////////////

// macro to help prevent re-entrancy in functions
#define AF_NOREENTRANT  \
	static BOOL bAFInHere = FALSE; \
	if (bAFInHere) \
	return; \
CAutoFlag af(bAFInHere, TRUE);

#define AF_NOREENTRANT_RET(ret)  \
	static BOOL bAFInHere = FALSE; \
	if (bAFInHere) \
	return ret; \
CAutoFlag af(bAFInHere, TRUE);

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_AUTOFLAG_H__EEBD725A_2C54_4554_96D8_E4CCDC940258__INCLUDED_)
