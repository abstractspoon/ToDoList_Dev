#if !defined(AFX_TDLMULTISORTDLG_H__95734250_E60B_48CF_B356_1394C11317DA__INCLUDED_)
#define AFX_TDLMULTISORTDLG_H__95734250_E60B_48CF_B356_1394C11317DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLMultiSortDlg.h : header file
//

#include "tdcenum.h"
#include "tdcstruct.h"
#include "tdldialog.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLMultiSortDlg dialog

class CTDLMultiSortDlg : public CTDLDialog
{
// Construction
public:
	CTDLMultiSortDlg(const TDSORTCOLUMNS& sort, const CTDCColumnIDMap& mapVisibleColumns, 
					const CTDCCustomAttribDefinitionArray& aAttribDefs, CWnd* pParent = NULL);   // standard constructor

	void GetSortBy(TDSORTCOLUMNS& sort) const;

protected:
// Dialog Data
	//{{AFX_DATA(CTDLMultiSortDlg)
	enum { IDD = IDD_MULTISORT_DIALOG };
	CComboBox	m_cbSortBy3;
	CComboBox	m_cbSortBy2;
	CComboBox	m_cbSortBy1;
	//}}AFX_DATA
	TDSORTCOLUMNS m_sort;
	const CTDCColumnIDMap& m_mapVisibleColumns;
	const CTDCCustomAttribDefinitionArray& m_aAttribDefs;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLMultiSortDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTDLMultiSortDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeSortby1();
	afx_msg void OnSelchangeSortby2();
	afx_msg void OnSelchangeSortby3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void BuildCombos();
	BOOL IsColumnVisible(TDC_COLUMN col) const;
	void EnableControls();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLMULTISORTDLG_H__95734250_E60B_48CF_B356_1394C11317DA__INCLUDED_)
