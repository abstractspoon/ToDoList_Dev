////////////////////////////////////////////////////////////////////////////
// File:	StatusBarACT.h
// Version:	3.1
// Created:	24 Jun 2004
//
// Author:	Paul S. Vickery
// E-mail:	paul@vickeryhome.freeserve.co.uk
//
// CStatusBar derived control to add auto-fit, tooltips and command handling
//
// You are free to use or modify this code, with no restrictions, other than
// you continue to acknowledge me as the original author in this source code,
// or any code derived from it.
//
// If you use this code, or use it as a base for your own code, it would be 
// nice to hear from you simply so I know it's not been a waste of time!
//
// Copyright (c) 2003-2004 Paul S. Vickery
//
////////////////////////////////////////////////////////////////////////////
// Version History:
//
// Version 3.1 - 24 Jun 2004
// =========================
// - Updated to support Unicode builds
// - fixed bugs:
//   - using ON_COMMAND macros causes assertion failure
//   - updating tip text re-adds the tool, and causes old one to overlap
//   - crash in GetPaneCursorIndex() if no pane cursors set and also in
//     GetPaneFlagsIndex() if no pane flags set (reported by Tom Mason)
//
// Version 3.0 - 28 May 2003
// =========================
// - Can now specify a custom cursor for a pane
// - Added ability to specify a hand cursor to use instead of the default
//   (the default is only available on Windows 98/ME and Windows 2000/XP)
//
// Version 2.0 - 15 Apr 2003
// =========================
// - Extended control to allow pane tool-tips to be specified as part of the 
//   pane's text, separated by a new line ('\n') character.
// - Added ability to show multi-line tool-tips by including carriage returns 
//   ('\r') and/or line breaks ('\n') in the tip text.
// 
// Version 1.0 - 18 Feb 2003
// =========================
// Initial version
// 
////////////////////////////////////////////////////////////////////////////
// PLEASE LEAVE THIS HEADER INTACT
////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATUSBARACT_H__18717507_36AA_4071_9911_F678515483B0__INCLUDED_)
#define AFX_STATUSBARACT_H__18717507_36AA_4071_9911_F678515483B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatusBarACT window

enum
{
  SBACTF_NORMAL			= 0x0000, 
  SBACTF_AUTOFIT		= 0x0001, // autosize as the text changes
  SBACTF_COMMAND		= 0x0002, // send a WM_COMMAND message to the parent frame
  SBACTF_HANDCURSOR		= 0x0004, // show a hand cursor when the mouse is over the pane
  SBACTF_STRETCHY		= 0x0008, // takes up space left by every other panes

  SBACTF_RESOURCETIP	= 0x0080, // lpszTip is really a UINT

  // which clicks will send commands (only valid if SBACTF_COMMAND specified)
  SBACTF_DOUBLECLICK	= 0x0100, // default if none specified
  SBACTF_SINGLECLICK	= 0x0200, 

  // which buttons will send commands (only valid if SBACTF_COMMAND specified)
  SBACTF_LEFTBUTTON		= 0x1000, // default if none specified
  SBACTF_RIGHTBUTTON	= 0x2000, 
  SBACTF_MIDDLEBUTTON	= 0x4000, 

  SBACTF_STYLEFLAGMASK	= 0x00FF,   // mask for 'style' flags
  SBACTF_CLICKFLAGMASK	= 0x0F00,   // mask for click flags
  SBACTF_BUTTONFLAGMASK	= 0xF000,   // mask for button flags
};

typedef struct SBACTPANEINFO
{
  UINT nID;		// pane command ID
  LPCTSTR lpszTip;	// text for pane tooltip (see SBACTF_RESOURCETIP above)
  DWORD dwFlags;	// pane flags (see above)
  LPCTSTR lpszCursor;	// custom cursor, if specified
}*LPSBACTPANEINFO;

class CStatusBarACT : public CStatusBar
{
	DECLARE_DYNAMIC(CStatusBarACT)

// Construction
public:
	CStatusBarACT();

// Attributes
public:
	void SetUIColors(COLORREF crBackFrom, COLORREF crBackTo, COLORREF crText, BOOL bGradient, BOOL bGlass);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatusBarACT)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL SetPanes(LPSBACTPANEINFO lpsbactpi, UINT nPaneInfoCount);
	BOOL SetPane(LPSBACTPANEINFO lpsbactpi);
	BOOL SetPaneTooltipIndex(int nIndex, LPCTSTR lpszText = NULL);
	BOOL SetPaneTooltip(UINT nID, LPCTSTR lpszText = NULL);
	BOOL SetPaneTooltipIndex(int nIndex, UINT nTipID);
	BOOL SetPaneTooltip(UINT nID, UINT nTipID);
	BOOL SetPaneFlagsIndex(int nIndex, DWORD dwFlags = SBACTF_NORMAL);
	BOOL SetPaneFlags(UINT nID, DWORD dwFlags = SBACTF_NORMAL);
	DWORD GetPaneFlagsIndex(int nIndex);
	DWORD GetPaneFlags(UINT nID);
	BOOL SetPaneCursorIndex(int nIndex, UINT nCursorID);
	BOOL SetPaneCursor(UINT nID, UINT nCursorID);
	BOOL SetPaneCursorIndex(int nIndex, LPCTSTR lpszCursor);
	BOOL SetPaneCursor(UINT nID, LPCTSTR lpszCursor);
	BOOL SetPaneCursorIndex(int nIndex, HCURSOR hCursor);
	BOOL SetPaneCursor(UINT nID, HCURSOR hCursor);
	HCURSOR GetPaneCursorIndex(int nIndex);
	HCURSOR GetPaneCursor(UINT nID);
	void SetHandCursor(HCURSOR hCursor, BOOL bTryDefault = TRUE);
	BOOL SetHandCursor(LPCTSTR lpszCursorID, BOOL bTryDefault = TRUE);
	BOOL SetHandCursor(UINT nCursorID, BOOL bTryDefault = TRUE);
	void AutoFitPane(int nIndex);
	int GetPaneCount() const { return m_adwFlags.GetSize(); }

	virtual ~CStatusBarACT();

	// Generated message map functions
protected:
	void SendPaneCommand(CPoint point, UINT message);
	int HitTest(CPoint point);
	void UpdateTooltipRects();

	CToolTipCtrl m_tooltip;
	CArray<DWORD, DWORD> m_adwFlags;
	CArray<HCURSOR, HCURSOR> m_adwCursors;
	HCURSOR m_hCursorHand;
	BOOL m_bTryDefaultHandCursor;
	COLORREF m_crFrom, m_crTo, m_crText;
	BOOL m_bGradient, m_bGlass;

	//{{AFX_MSG(CStatusBarACT)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();

	virtual void DrawRectBkgnd(CDC* pDC, const CRect& rect);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATUSBARACT_H__18717507_36AA_4071_9911_F678515483B0__INCLUDED_)
