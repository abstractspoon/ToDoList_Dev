// MenuIconMgr.cpp: implementation of the CTDCMenuIconMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDCMenuIconMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCMenuIconMgr::CTDCMenuIconMgr()
{
}

CTDCMenuIconMgr::~CTDCMenuIconMgr()
{
}

BOOL CTDCMenuIconMgr::Initialize(CWnd* pWnd/*, TODO */)
{
	if (!CMenuIconMgr::Initialize(pWnd))
		return FALSE;

	// TODO
	
	return TRUE;
}

void CTDCMenuIconMgr::Release()
{
	CMenuIconMgr::Release();

	// TODO
}

