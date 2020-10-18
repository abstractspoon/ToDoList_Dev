#if !defined(AFX_TDLTASKLISTCTRLOPTIONSCOMBOBOX_H__E788C983_B0C2_49F2_8959_7D07FB57802F__INCLUDED_)
#define AFX_TDLTASKLISTCTRLOPTIONSCOMBOBOX_H__E788C983_B0C2_49F2_8959_7D07FB57802F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLTaskListOptionsComboBox.h : header file
//

#include "..\shared\checkcombobox.h"

/////////////////////////////////////////////////////////////////////////////

class IPreferences;

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskListOptionsComboBox window

class CTDLTaskListCtrlOptionsComboBox : public CCheckComboBox
{
// Construction
public:
	CTDLTaskListCtrlOptionsComboBox();
	virtual ~CTDLTaskListCtrlOptionsComboBox();
	
	void BuildCombo();
	void RemoveOptions(DWORD dwOptions);

	static DWORD LoadOptions(const IPreferences* pPrefs, LPCTSTR szKey);
	static void SaveOptions(DWORD dwOptions, IPreferences* pPrefs, LPCTSTR szKey);

protected:
	DWORD m_dwRemovedOptions;

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLTaskListOptionsComboBox)
	//}}AFX_VIRTUAL
	virtual void PreSubclassWindow();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDLTaskListOptionsComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()

protected:
	void AddOption(DWORD dwOption, UINT nOptionStrID);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLTASKLISTCTRLOPTIONSCOMBOBOX_H__E788C983_B0C2_49F2_8959_7D07FB57802F__INCLUDED_)
