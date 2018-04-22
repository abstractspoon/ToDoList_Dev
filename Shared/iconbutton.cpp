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

CIconButton::CIconButton(int nSize) : m_nSize(GraphicsMisc::ScaleByDPIFactor(nSize))
{
}

CIconButton::~CIconButton()
{
	m_ilIcon.DeleteImageList();
}


BEGIN_MESSAGE_MAP(CIconButton, CCustomButton)
	//{{AFX_MSG_MAP(CIconButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIconButton message handlers

void CIconButton::DoExtraPaint(CDC* pDC, const CRect& rExtra)
{
	if (m_ilIcon.GetSafeHandle() && (m_ilIcon.GetImageCount() == 1))
		m_ilIcon.Draw(pDC, 0, rExtra.TopLeft(), ILD_TRANSPARENT);
}

void CIconButton::SetIcon(HICON hIcon, BOOL bCleanup)
{ 
	m_ilIcon.DeleteImageList();

	if (hIcon)
	{
		m_ilIcon.Create(m_nSize, m_nSize, (ILC_COLOR32 | ILC_MASK), 0, 1);

		VERIFY(m_ilIcon.Add(hIcon) == 0);

		if (bCleanup)
			::DestroyIcon(hIcon);
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
