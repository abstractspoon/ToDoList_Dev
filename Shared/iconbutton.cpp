// colorbutton.cpp : implementation file
//

#include "stdafx.h"
#include "iconbutton.h"
#include "misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIconButton

CIconButton::CIconButton(int nSize) : m_nSize(nSize)
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
	if (hIcon)
	{
		if (!m_ilIcon.GetSafeHandle())
		{
			m_ilIcon.Create(m_nSize, m_nSize, (ILC_COLOR32 | ILC_MASK), 1, 1);
			VERIFY(m_ilIcon.Add(hIcon) == 0);
		}
		else
		{
			ASSERT(m_ilIcon.GetImageCount() == 1);
			m_ilIcon.Replace(0, hIcon);
		}
		
		if (bCleanup)
			::DestroyIcon(hIcon);
	}
	else
	{
		m_ilIcon.DeleteImageList();
	}
	
	if (GetSafeHwnd())
		Invalidate();
}

void CIconButton::SetTooltip(LPCTSTR szTooltip)
{
	if (!Misc::IsEmpty(szTooltip))
	{
		if (!m_tooltip.GetSafeHwnd())
			VERIFY(m_tooltip.Create(this, (TTS_NOPREFIX | TTS_ALWAYSTIP)));
		else
			m_tooltip.DelTool(this);

		m_tooltip.AddTool(this, szTooltip);
	}
	else
	{
		m_tooltip.DestroyWindow();
	}
}


void CIconButton::CalcExtraSpace(const CRect& rClient, CRect& rExtra) const
{
	if (GetWindowTextLength() == 0)
	{
		rExtra = rClient;
		rExtra.DeflateRect(((rExtra.Width() - m_nSize) / 2), ((rExtra.Height() - m_nSize) / 2));
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
