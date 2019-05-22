#if !defined(AFX_TIMECOMBOBOX_H__E536C077_C228_4A44_89F5_D44F456A2EE7__INCLUDED_)
#define AFX_TIMECOMBOBOX_H__E536C077_C228_4A44_89F5_D44F456A2EE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// timecombobox.h : header file
//
#include "Subclass.h"
#include "OwnerdrawComboBoxBase.h"

/////////////////////////////////////////////////////////////////////////////

enum 
{
	TCB_NOTIME		= 0x01,
	TCB_HALFHOURS	= 0x02,
	TCB_ISO   		= 0x04,
	TCB_HOURSINDAY	= 0x08,
};

/////////////////////////////////////////////////////////////////////////////
// CTimeComboBox window

class CTimeComboBox : public COwnerdrawComboBoxBase, public CSubclasser
{
// Construction
public:
	CTimeComboBox(DWORD dwStyle = 0);
	
	double GetOleTime() const;
	BOOL SetOleTime(double dTime); // 0-1 suitable for COleDateTime, -1 for 'no time'
	
	double Get24HourTime() const;
	BOOL Set24HourTime(double dTime); // -1 for 'no time'
	
	DWORD GetStyle() { return m_dwStyle; }
	void SetStyle(DWORD dwStyle);

	void DDX(CDataExchange* pDX, double& dHours);
	void DDX(CDataExchange* pDX, COleDateTime& dtTime);
	
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
	virtual void GetItemColors(int nItem, UINT nItemState, DWORD dwItemData, 
								COLORREF& crText, COLORREF& crBack) const;	

protected:
	void BuildCombo(BOOL bReset = FALSE);
	BOOL Initialize();
	void ScrollListBox();
	CString GetCurrentTime() const;
	double Get24HourTime(int nItem) const; // -1 for 'no time'
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMECOMBOBOX_H__E536C077_C228_4A44_89F5_D44F456A2EE7__INCLUDED_)
