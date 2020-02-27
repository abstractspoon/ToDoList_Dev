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
	m_nSize(GraphicsMisc::ScaleByDPIFactor(nSize)), 
	m_hIcon(NULL)
{
}

CIconButton::~CIconButton()
{
	::DestroyIcon(m_hIcon);
}


BEGIN_MESSAGE_MAP(CIconButton, CCustomButton)
	//{{AFX_MSG_MAP(CIconButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIconButton message handlers

void CIconButton::DoExtraPaint(CDC* pDC, const CRect& rExtra)
{
	if (m_hIcon)
		::DrawIconEx(*pDC, rExtra.left, rExtra.top, m_hIcon, m_nSize, m_nSize, 0, NULL, DI_NORMAL);
}

void CIconButton::SetIcon(HICON hIcon, BOOL bCleanup)
{ 
	::DestroyIcon(m_hIcon);

	if (hIcon)
	{
		if (bCleanup)
			m_hIcon = hIcon;
		else
			m_hIcon = ::CopyIcon(hIcon);
	}
	
	if (GetSafeHwnd())
		Invalidate();
}

void CIconButton::CalcExtraSpace(const CRect& rClient, CRect& rExtra) const
{
	if (GetWindowTextLength() == 0)
	{
		rExtra.SetRect(0, 0, m_nSize, m_nSize);

		GraphicsMisc::CentreRect(rExtra, rClient, TRUE, TRUE);
	}
	else
	{
		CCustomButton::CalcExtraSpace(rClient, rExtra);
	}
}

BOOL CIconButton::PreTranslateMessage(MSG* pMsg)
{
	if (m_tooltip.GetSafeHwnd())
		m_tooltip.RelayEvent(pMsg);

	return CCustomButton::PreTranslateMessage(pMsg);
}
