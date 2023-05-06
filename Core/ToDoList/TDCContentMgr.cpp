// TDLContentMgr.cpp: implementation of the CTDLContentMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDCContentMgr.h"
#include "TDCSimpleTextContent.h"

#include "..\Shared\Localizer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCContentMgr::CTDCContentMgr() 
{
}

CTDCContentMgr::~CTDCContentMgr()
{
}

void CTDCContentMgr::Initialize() const
{
	if (!m_bInitialized)
	{
		CContentMgr::Initialize();

		// we need a non-const pointer to update the array
		CTDCContentMgr* pMgr = const_cast<CTDCContentMgr*>(this);

		IContent* pSimpleText = new CTDCSimpleTextContent;

		pMgr->m_aContent.InsertAt(0, pSimpleText);
		pMgr->m_mapFormatToDescription[pSimpleText->GetTypeID()] = CLocalizer::TranslateText(pSimpleText->GetTypeDescription());
	}
}

CONTENTFORMAT CTDCContentMgr::GetSimpleTextContentFormat() const
{
	Initialize();

	return GetContentFormat(0);
}
