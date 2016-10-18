#if !defined(AFX_TIMECOMBOBOX_H__E536C077_C228_4A44_89F5_D44F456A2EE7__INCLUDED_)
#define AFX_TIMECOMBOBOX_H__E536C077_C228_4A44_89F5_D44F456A2EE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// timecombobox.h : header file
//
#include "Subclass.h"

/////////////////////////////////////////////////////////////////////////////
// CTimeComboBox window

enum 
{
	TCB_NOTIME		= 0x01,
	TCB_HALFHOURS	= 0x02,
	TCB_ISO   		= 0x04,
};

class CTimeComboBox : public CComboBox, public CSubclasser
{
// Construction
public:
	CTimeComboBox(DWORD dwStyle = 0);
	
	double GetOleTime() const; // 0-1 suitable for COleDateTime, -1 for 'no time'
	BOOL SetOleTime(double dTime); // 0-1 suitable for COleDateTime, -1 for 'no time'
	
	double Get24HourTime() const; // -1 for 'no time'
	BOOL Set24HourTime(double dTime); // -1 for 'no time'
	
	DWORD GetStyle() { return m_dwStyle; }
	void SetStyle(DWORD dwStyle);
	
protected:
	DWORD m_dwStyle;
	CSubclassWnd m_scEdit;
	HWND m_hwndListBox;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimeComboBox)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTimeComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTimeComboBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnCaptureChanged(CWnd* pWnd);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()

	virtual LRESULT ScWindowProc(HWND hRealWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
	void BuildCombo(BOOL bReset = FALSE);
	BOOL Initialize();
	void ScrollListBox();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMECOMBOBOX_H__E536C077_C228_4A44_89F5_D44F456A2EE7__INCLUDED_)
