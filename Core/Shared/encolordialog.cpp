// encolordialog.cpp : implementation file
//

#include "stdafx.h"
#include "encolordialog.h"

#include "..\Interfaces\IPreferences.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnColorDialog

IMPLEMENT_DYNAMIC(CEnColorDialog, CColorDialog)

CEnColorDialog::CEnColorDialog(COLORREF clrInit, DWORD dwFlags, CWnd* pParentWnd) 
	:
	CColorDialog(clrInit, dwFlags, pParentWnd)
{
	if (clrInit == CLR_NONE)
		m_cc.Flags &= ~CC_RGBINIT;
}

CEnColorDialog::~CEnColorDialog()
{
}

BEGIN_MESSAGE_MAP(CEnColorDialog, CColorDialog)
	//{{AFX_MSG_MAP(CEnColorDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

int CEnColorDialog::DoModal(IPreferences* pPrefs)
{
	if (pPrefs)
		LoadPreferences(pPrefs);

	int nRet = CColorDialog::DoModal();

	if ((nRet == IDOK) && pPrefs)
		SavePreferences(pPrefs);

	return nRet;
}

void CEnColorDialog::LoadPreferences(const IPreferences* pPrefs)
{
	// restore previously saved custom colors
	for (int nColor = 0; nColor < 16; nColor++)
	{
		CString sKey;
		sKey.Format(_T("CustomColor%d"), nColor);

		COLORREF color = (COLORREF)pPrefs->GetProfileInt(_T("ColorDialog"), sKey, (int)RGB(255, 255, 255));
		m_cc.lpCustColors[nColor] = color;
	}
}

void CEnColorDialog::SavePreferences(IPreferences* pPrefs) const
{
	// save any custom colors
	COLORREF* pColors = GetSavedCustomColors();

	for (int nColor = 0; nColor < 16; nColor++)
	{
		CString sKey;
		sKey.Format(_T("CustomColor%d"), nColor);

		int nColorVal = (int)pColors[nColor];
		pPrefs->WriteProfileInt(_T("ColorDialog"), sKey, nColorVal);
	}
}

void CEnColorDialog::SetCurrentColor(COLORREF clr)
{
	if (GetSafeHwnd())
	{
		CColorDialog::SetCurrentColor(clr);
	}
	else if (clr != CLR_NONE)
	{
		m_cc.rgbResult = clr;
		m_cc.Flags |= CC_RGBINIT;
	}
}
