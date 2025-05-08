// MouseWheelMgr.h: interface for the CMouseWheelMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTLINPUTMGR_H__6738593F_D10B_45D9_ACA0_335DA7C0F630__INCLUDED_)
#define AFX_RTLINPUTMGR_H__6738593F_D10B_45D9_ACA0_335DA7C0F630__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HookMgr.h"

class CRTLInputMgr : public CHookMgr<CRTLInputMgr>  
{
   friend class CHookMgr<CRTLInputMgr>;

public:
	virtual ~CRTLInputMgr();

 	static BOOL Initialize();
	static void Release();

	static BOOL IsEnabled();

protected:
	CRTLInputMgr();
 	static CRTLInputMgr& Instance() { return CHookMgr<CRTLInputMgr>::GetInstance(); }

protected:
	virtual BOOL OnCallWndProc(const MSG& msg);
};

#endif // !defined(AFX_RTLINPUTMGR_H__6738593F_D10B_45D9_ACA0_335DA7C0F630__INCLUDED_)
