// MouseWheelMgr.h: interface for the CMouseWheelMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTLEDITMGR_H__6738593F_D10B_45D9_ACA0_335DA7C0F630__INCLUDED_)
#define AFX_RTLEDITMGR_H__6738593F_D10B_45D9_ACA0_335DA7C0F630__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HookMgr.h"

class CRTLStyleMgr : public CHookMgr<CRTLStyleMgr>  
{
   friend class CHookMgr<CRTLStyleMgr>;

public:
	virtual ~CRTLStyleMgr();

 	static BOOL Initialize();
	static void Release();

	static BOOL IsRTL();
	static BOOL IsSystemFontRTL();

protected:
	CRTLStyleMgr();
 	static CRTLStyleMgr& Instance() { return CHookMgr<CRTLStyleMgr>::GetInstance(); }

protected:
	virtual BOOL OnCallWndProc(const MSG& msg);

	static BOOL IsClass(HWND hWnd, LPCTSTR szClass);
};

#endif // !defined(AFX_RTLEDITMGR_H__6738593F_D10B_45D9_ACA0_335DA7C0F630__INCLUDED_)
