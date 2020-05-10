// colorbutton.cpp : implementation file
//

#include "stdafx.h"
#include "colorbutton.h"
#include "encolordialog.h"
#include "graphicsmisc.h"

#include "..\3rdParty\ColorDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

CEnColorDialog CColourButton::s_dlgColor(CLR_NONE, CC_FULLOPEN | CC_RGBINIT);

/////////////////////////////////////////////////////////////////////////////
// CColorButton

CColourButton::CColourButton(BOOL bRoundRect) 
	: 
	m_color(CLR_NONE), 
	m_bRoundRect(bRoundRect)
{
}

CColourButton::~CColourButton()
{
}


BEGIN_MESSAGE_MAP(CColourButton, CCustomButton)
	//{{AFX_MSG_MAP(CColorButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorButton message handlers

void CColourButton::DDX(CDataExchange* pDX, COLORREF& value)
{
	if (pDX->m_bSaveAndValidate)
		value = GetColor();
	else
		SetColor(value);
}

void CColourButton::DoExtraPaint(CDC* pDC, const CRect& rExtra)
{
	int nCornerRadius = m_bRoundRect ? (rExtra.Width() / 4) : 0;

	COLORREF crFill, crBorder;
	GraphicsMisc::CalculateBoxColors(m_color, IsWindowEnabled(), crFill, crBorder);

	GraphicsMisc::DrawRect(pDC, rExtra, crFill, crBorder, nCornerRadius);
}

void CColourButton::SetColor(COLORREF color) 
{ 
	m_color = color; 

	if (GetSafeHwnd())
		Invalidate();
}

BOOL CColourButton::DoAction()
{
	s_dlgColor.SetCurrentColor(m_color);

	if (s_dlgColor.DoModal() == IDOK)
	{
		m_color = s_dlgColor.m_cc.rgbResult;
	
		Invalidate();
		UpdateWindow();
	}

	return FALSE; // always notify parent
}

void CColourButton::LoadPreferences(const IPreferences* pPrefs)
{
	s_dlgColor.LoadPreferences(pPrefs);
}

void CColourButton::SavePreferences(IPreferences* pPrefs)
{
	s_dlgColor.SavePreferences(pPrefs);
}

