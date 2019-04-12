// CFPSMiniCalendarListCtrl
// Author:	Matt Gullett
//			gullettm@yahoo.com
// Copyright (c) 2001
//
// This is a companion control for use by the CFPSMiniCalendarCtrl.
// It is probably useless for any other purpose.
//
// You may freely use this source code in personal, freeware, shareware
// or commercial applications provided that 1) my name is recognized in the
// code and if this code represents a substantial portion of the application
// that my name be included in the credits for the application (about box, etc)
//
// Use this code at your own risk.  This code is provided AS-IS.  No warranty
// is granted as to the correctness, usefulness or value of this code.
//
// Special thanks to Keith Rule for the CMemDC class
// http://www.codeproject.com/gdi/flickerfree.asp
//
// *******************************************************************
// TECHNICAL NOTES:
//
// See .cpp file for tech notes
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_FPSMINICALENDARLISTCTRL_H__B39BC25E_24F6_4DB1_A83B_B25CFA8D082C__INCLUDED_)
#define AFX_FPSMINICALENDARLISTCTRL_H__B39BC25E_24F6_4DB1_A83B_B25CFA8D082C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FPSMiniCalendarListCtrl.h : header file
//

#include "FPSMiniCalendarCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CFPSMiniCalendarListCtrl window

class CFPSMiniCalendarListCtrl : public CWnd
{
// Construction
public:
	CFPSMiniCalendarListCtrl();

// Attributes
public:

// Operations
public:
	int GetSelMonth() {return m_iSelMonth;}
	int GetSelYear() {return m_iSelYear;}

	void SetCalendar(CFPSMiniCalendarCtrl* pWnd);
	void SetMiddleMonthYear(int iMonth, int iYear);
	void SetItemsPerPage(int iValue);
	void SetBackColor(COLORREF cColor) {m_cBackColor = cColor;}
	void SetTextColor(COLORREF cColor) {m_cTextColor = cColor;}

	void SetFontInfo(const CFPSMiniCalendarCtrlFontInfo& font);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFPSMiniCalendarListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void ScrollDown(int iCount = 1);
	void ScrollUp(int iCount = 1);
	void AutoConfigure();
	void ForwardMessage(MSG* pMSG);
	virtual ~CFPSMiniCalendarListCtrl();

	// Generated message map functions
protected:
	BOOL IsSelected(int iX, CRect rectItem);
	CString CStr(long lValue);

	//{{AFX_MSG(CFPSMiniCalendarListCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	int			m_iMiddleMonth;
	int			m_iMiddleYear;
	int			m_iItemsPerPage;

	int			m_iSelMonth;
	int			m_iSelYear;

	COLORREF	m_cBackColor;
	COLORREF	m_cTextColor;
	CFPSMiniCalendarCtrlFontInfo	m_FontInfo;
	CFPSMiniCalendarCtrl*			m_pwndCalendar;

	int			m_iUpFactor;
	int			m_iDownFactor;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FPSMINICALENDARLISTCTRL_H__B39BC25E_24F6_4DB1_A83B_B25CFA8D082C__INCLUDED_)
