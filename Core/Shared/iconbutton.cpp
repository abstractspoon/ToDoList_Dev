// colorbutton.cpp : implementation file
//

#include "stdafx.h"
#include "iconbutton.h"
#include "GraphicsMisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIconButton

CIconButton::CIconButton(int nSize) 
	: 
	m_nSize(GraphicsMisc::ScaleByDPIFactor(nSize)) 
{
}

CIconButton::~CIconButton()
{
}


BEGIN_MESSAGE_MAP(CIconButton, CCustomButton)
	//{{AFX_MSG_MAP(CIconButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIconButton message handlers

void CIconButton::DoExtraPaint(CDC* pDC, const CRect& rExtra)
{
	if (m_icon.IsValid())
		::DrawIconEx(*pDC, rExtra.left, rExtra.top, m_icon, m_nSize, m_nSize, 0, NULL, DI_NORMAL);
}

void CIconButton::SetIcon(HICON hIcon, BOOL bCleanup)
{ 
	m_icon.Destroy();
	m_icon.Attach(hIcon, bCleanup);
	
	if (GetSafeHwnd())
		Invalidate();
}

void CIconButton::CalcExtraSpace(const CRect& rClient, CRect& rExtra) const
{
	if (GetWindowTextLength() == 0)
		rExtra = GraphicsMisc::CalcCentredRect(m_nSize, rClient, TRUE, TRUE);
	else
		CCustomButton::CalcExtraSpace(rClient, rExtra);
}

BOOL CIconButton::PreTranslateMessage(MSG* pMsg)
{
	if (m_tooltip.GetSafeHwnd())
		m_tooltip.RelayEvent(pMsg);

	return CCustomButton::PreTranslateMessage(pMsg);
}
