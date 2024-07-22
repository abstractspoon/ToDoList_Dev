#if !defined(AFX_INSERTTABLEDLG_H__28F40381_2704_436A_B88B_6FABCE98AF52__INCLUDED_)
#define AFX_INSERTTABLEDLG_H__28F40381_2704_436A_B88B_6FABCE98AF52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InsertTableDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#include "..\shared\icon.h"

/////////////////////////////////////////////////////////////////////////////
// CInsertTableDlg dialog

class CInsertTableDlg : public CDialog
{
// Construction
public:
	CInsertTableDlg(CWnd* pParent = NULL);   // standard constructor

	int GetNumRows() const { return m_nNumRows; }
	int GetNumColumns() const { return m_nNumCols; }
	int GetColumnWidth() const { return m_nColWidth; }

protected:
// Dialog Data
	//{{AFX_DATA(CInsertTableDlg)
	enum { IDD = IDD_INSERTTABLE_DIALOG };
	CComboBox m_cbColWidth;
	CComboBox m_cbCols;
	CComboBox m_cbRows;
	CIcon m_icon;
	//}}AFX_DATA
	int		m_nNumRows;
	int		m_nNumCols;
	int		m_nColWidth;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInsertTableDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CInsertTableDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INSERTTABLEDLG_H__28F40381_2704_436A_B88B_6FABCE98AF52__INCLUDED_)
