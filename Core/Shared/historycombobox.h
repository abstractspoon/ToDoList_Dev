#if !defined(AFX_HISTORYCOMBOBOX_H__B67234FA_504A_48CF_A187_3E5CCA2D05DD__INCLUDED_)
#define AFX_HISTORYCOMBOBOX_H__B67234FA_504A_48CF_A187_3E5CCA2D05DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// historycombobox.h : header file
//

class IPreferences;

/////////////////////////////////////////////////////////////////////////////
// CHistoryComboBox window

class CHistoryComboBox : public CComboBox
{
// Construction
public:
	CHistoryComboBox();

// Operations
public:
	void Load(const IPreferences* pPrefs, LPCTSTR szKey);
	void Save(IPreferences* pPrefs, LPCTSTR szKey) const;

protected:
	CStringArray m_aHistory;

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHistoryComboBox)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHistoryComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHistoryComboBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void BuildCombo();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTORYCOMBOBOX_H__B67234FA_504A_48CF_A187_3E5CCA2D05DD__INCLUDED_)
