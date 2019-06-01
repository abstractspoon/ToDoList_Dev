// TDLContentMgr.cpp: implementation of the CTDLContentMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLContentMgr.h"
#include "TDCSimpleTextContent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDLContentMgr::CTDLContentMgr() 
{
}

CTDLContentMgr::~CTDLContentMgr()
{
}

void CTDLContentMgr::Initialize() const
{
	if (!m_bInitialized)
	{
		CContentMgr::Initialize();

		// we need a non-const pointer to update the array
		CTDLContentMgr* pMgr = const_cast<CTDLContentMgr*>(this);

		pMgr->m_aContent.InsertAt(0, new CTDCSimpleTextContent);
	}
}

CONTENTFORMAT CTDLContentMgr::GetSimpleTextContentFormat() const
{
	Initialize();

	return GetContentFormat(0);
}
