// TDLContentMgr.h: interface for the CTDLContentMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDLCONTENTMGR_H__44EE386C_9797_4472_B8D6_8E3E2504D293__INCLUDED_)
#define AFX_TDCCONTENTMGR_H__44EE386C_9797_4472_B8D6_8E3E2504D293__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Interfaces\contentmgr.h"

class CTDCContentMgr : public CContentMgr  
{
public:
	CTDCContentMgr();
	virtual ~CTDCContentMgr();

	CONTENTFORMAT GetSimpleTextContentFormat() const;

protected:
	virtual void Initialize() const;

};

#endif // !defined(AFX_TDLCONTENTMGR_H__44EE386C_9797_4472_B8D6_8E3E2504D293__INCLUDED_)
