#if !defined(AFX_DATETIMECTRLEX_H__A8DB7BE7_A89F_4427_AD13_4743B5397314__INCLUDED_)
#define AFX_DATETIMECTRLEX_H__A8DB7BE7_A89F_4427_AD13_4743B5397314__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DateTimeCtrlEx.h : header file
//

#include "MonthCalCtrlEx.h"

/////////////////////////////////////////////////////////////////////////////

#if _MSC_VER < 1500
struct DATETIMEPICKERINFO;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDateTimeCtrlEx window

class CDateTimeCtrlEx : public CDateTimeCtrl
{
// Construction
public:
	CDateTimeCtrlEx(DWORD dwMonthCalStyle = MCS_WEEKNUMBERS);

	DWORD SetMonthCalStyle(DWORD dwStyle);
	DWORD GetMonthCalStyle() const;
	BOOL IsCalendarVisible() const;
	void EnableInlineEditing(BOOL bEnable = TRUE) { m_bEnableInlineEditing = bEnable; }
	void ShowCalendarOnCompleting(BOOL bShow = TRUE) { m_bShowCalendarOnCompleting = bShow; }
	BOOL ShowSeconds(BOOL bShow);

// Attributes
protected:
	BOOL m_bLButtonDown, m_bDropped, m_bWasSet, m_bNotifyingParent;
	NMDATETIMECHANGE m_nmdtcFirst, m_nmdtcLast;
	DWORD m_dwMonthCalStyle;
	BOOL m_bEnableInlineEditing;
	BOOL m_bShowCalendarOnCompleting;

	CMonthCalCtrlEx m_monthCal;

// Overrides
protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDateTimeCtrlEx)
	//}}AFX_VIRTUAL
	void PreSubclassWindow();

// Implementation
public:
	virtual ~CDateTimeCtrlEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDateTimeCtrlEx)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnDateTimeChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnCloseUp(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnDropDown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	afx_msg int OnCreate(LPCREATESTRUCT pCreate);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);

	afx_msg LRESULT OnGetMonthCal(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()

protected:
	BOOL IsCheckboxFocused() const;
	BOOL IsDateSet() const;
	CRect GetDropButtonRect() const;
	CRect GetCheckboxRect() const;
	void ResetCalendarHandling();
	BOOL GetPickerInfo(DATETIMEPICKERINFO& dtpi) const;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATETIMECTRLEX_H__A8DB7BE7_A89F_4427_AD13_4743B5397314__INCLUDED_)
