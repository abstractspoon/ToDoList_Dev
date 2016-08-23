// AutoFlag.cpp: implementation of the CAutoFlag class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoFlag.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAutoFlag::CAutoFlag(BOOL& bFlag, BOOL bState) : m_bFlag(bFlag)
{
	m_bInitialState = bFlag;
	m_bFlag = bState;
}

CAutoFlag::~CAutoFlag()
{
	m_bFlag = m_bInitialState;
}
