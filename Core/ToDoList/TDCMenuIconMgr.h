// TDCMenuIconMgr.h: interface for the CTDCMenuIconMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCMENUICONMGR_H__0FF0228C_515C_4E93_A957_1952AFD0F3A1__INCLUDED_)
#define AFX_TDCMENUICONMGR_H__0FF0228C_515C_4E93_A957_1952AFD0F3A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\shared\menuiconmgr.h"

class CTDCMenuIconMgr : public CMenuIconMgr
{
public:
	CTDCMenuIconMgr();
	virtual ~CTDCMenuIconMgr();
	
	BOOL Initialize(CWnd* pWnd/*, TODO */);
	void Release();

};

#endif // !defined(AFX_TDCMENUICONMGR_H__0FF0228C_515C_4E93_A957_1952AFD0F3A1__INCLUDED_)
