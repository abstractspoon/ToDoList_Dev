#if !defined(AFX_TDLPRINTDIALOG_H__1A62F94F_687F_421C_97D2_300BAC4A3E7C__INCLUDED_)
#define AFX_TDLPRINTDIALOG_H__1A62F94F_687F_421C_97D2_300BAC4A3E7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLPrintDialog.h : header file
//

#include "TaskSelectionDlg.h"
#include "TDLDialog.h"

#include "..\shared\fileedit.h"
#include "..\shared\historycombobox.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLPrintDialog dialog

enum TDLPD_STYLE
{
	TDLPDS_STYLESHEET = -1,

	TDLPDS_WRAP,
	TDLPDS_TABLE,
	TDLPDS_PARA,
	TDLPDS_IMAGE,
};

class CTDLPrintDialog : public CTDLDialog
{
// Construction
public:
	CTDLPrintDialog(LPCTSTR szTitle, BOOL bPreview, FTC_VIEW nView, LPCTSTR szStylesheet,
					const CTDCCustomAttribDefinitionArray& aAttribDefs, CWnd* pParent = NULL);

	BOOL GetStylesheet(CString& sStylesheet) const;
	CString GetTitle() const { return m_sTitle; }
	COleDateTime GetDate() const;
	TDLPD_STYLE GetExportStyle() const;

	const CTaskSelectionDlg& GetTaskSelection() const { return m_dlgTaskSel; }

protected:
// Dialog Data
	//{{AFX_DATA(CTDLPrintDialog)
	enum { IDD = IDD_PRINT_DIALOG };
	CFileEdit	m_eStylesheet;
	CString	m_sStylesheet;
    CTaskSelectionDlg m_dlgTaskSel;
	//}}AFX_DATA
	TDLPD_STYLE	m_nExportStyle;
	CString	m_sTitle;
	BOOL	m_bDate;
	BOOL	m_bUseStylesheet;
	BOOL	m_bPreview;
	CHistoryComboBox m_cbTitle;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLPrintDialog)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTDLPrintDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeStylesheet();
	afx_msg void OnChangeStyle();
	afx_msg void OnConfigureStylesheet();
	//}}AFX_MSG
    afx_msg void OnUsestylesheet();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

protected:
	void InitStylesheet(LPCTSTR szStylesheet);
	void EnableDisableControls();
	
	CString GetBaseStylesheetPath() const;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLPRINTDIALOG_H__1A62F94F_687F_421C_97D2_300BAC4A3E7C__INCLUDED_)
