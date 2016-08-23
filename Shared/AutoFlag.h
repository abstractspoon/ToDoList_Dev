// AutoFlag.h: interface for the CAutoFlag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOFLAG_H__EEBD725A_2C54_4554_96D8_E4CCDC940258__INCLUDED_)
#define AFX_AUTOFLAG_H__EEBD725A_2C54_4554_96D8_E4CCDC940258__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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

class CAutoFlag  
{
public:
	CAutoFlag(BOOL& bFlag, BOOL bState);
	virtual ~CAutoFlag();

protected:
	BOOL& m_bFlag;
	BOOL m_bInitialState;

};

#endif // !defined(AFX_AUTOFLAG_H__EEBD725A_2C54_4554_96D8_E4CCDC940258__INCLUDED_)
