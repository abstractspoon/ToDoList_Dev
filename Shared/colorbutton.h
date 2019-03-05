#if !defined(AFX_COLORBUTTON_H__4FA393FA_20E7_4C07_A682_5AA7A8306DF4__INCLUDED_)
#define AFX_COLORBUTTON_H__4FA393FA_20E7_4C07_A682_5AA7A8306DF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// colorbutton.h : header file
//

#include "custombutton.h"
#include "encolordialog.h"

/////////////////////////////////////////////////////////////////////////////

class IPreferences;

/////////////////////////////////////////////////////////////////////////////
// CColorButton window

class CColorButton : public CCustomButton
{
// Construction
public:
	CColorButton(BOOL bRoundRect = FALSE);

	COLORREF GetColor() { return m_color; }
	void SetColor(COLORREF color);

	static void LoadPreferences(const IPreferences* pPrefs);
	static void SavePreferences(IPreferences* pPrefs);

protected:
	COLORREF m_color;
	BOOL m_bRoundRect;

	// Only one button can be actioned at one time
	static CEnColorDialog s_dlgColor;

protected:
	virtual BOOL DoAction();
	virtual void DoExtraPaint(CDC* pDC, const CRect& rExtra);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorButton)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORBUTTON_H__4FA393FA_20E7_4C07_A682_5AA7A8306DF4__INCLUDED_)
