#if !defined(AFX_TIMECOMBOBOX_H__E536C077_C228_4A44_89F5_D44F456A2EE7__INCLUDED_)
#define AFX_TIMECOMBOBOX_H__E536C077_C228_4A44_89F5_D44F456A2EE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// timecombobox.h : header file
//
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

class CTimeComboBox : public COwnerdrawComboBoxBase
{
// Construction
public:
	CTimeComboBox(DWORD dwStyles = 0);
	virtual ~CTimeComboBox();
	
	double GetOleTime() const;
	BOOL SetOleTime(double dTime); // 0-1 suitable for COleDateTime, -1 for 'no time'
	
	double Get24HourTime() const;
	BOOL Set24HourTime(double dTime); // -1 for 'no time'
	
	void SetISOFormat(BOOL bISO);

	void DDX(CDataExchange* pDX, double& dHours);
	void DDX(CDataExchange* pDX, COleDateTime& dtTime);
	
protected:
	DWORD m_dwStyle;
	
// protected:
// 	virtual void PreSubclassWindow();

protected:
	// Generated message map functions
// 	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCaptureChanged(CWnd* pWnd);

	DECLARE_MESSAGE_MAP()

	virtual LRESULT OnEditboxMessage(UINT msg, WPARAM wp, LPARAM lp);
	virtual void GetItemColors(int nItem, UINT nItemState, DWORD dwItemData, 
								COLORREF& crText, COLORREF& crBack) const;	
	virtual void OnPopulate();

protected:
	void ScrollListBox();
	CString GetCurrentTime() const;
	double Get24HourTime(int nItem) const; // -1 for 'no time'
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMECOMBOBOX_H__E536C077_C228_4A44_89F5_D44F456A2EE7__INCLUDED_)
