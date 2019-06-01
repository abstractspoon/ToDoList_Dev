// symbolbutton.cpp : implementation file
//

#include "stdafx.h"
#include "symbolbutton.h"

#include "GraphicsMisc.h"
#include "localizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSymbolButton

CSymbolButton::CSymbolButton(LPCTSTR szFaceName, int nPoint) : m_hFont(NULL)
{
	m_hFont = GraphicsMisc::CreateFont(szFaceName, nPoint);
}

CSymbolButton::~CSymbolButton()
{
	GraphicsMisc::VerifyDeleteObject(m_hFont);
}


BEGIN_MESSAGE_MAP(CSymbolButton, CCustomButton)
	//{{AFX_MSG_MAP(CSymbolButton)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSymbolButton message handlers

void CSymbolButton::PreSubclassWindow() 
{
	CCustomButton::PreSubclassWindow();

	// symbols do not need translation
	CLocalizer::EnableTranslation(GetSafeHwnd(), FALSE);

	SendMessage(WM_SETFONT, (WPARAM)m_hFont);
}
