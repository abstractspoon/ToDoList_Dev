#if !defined(AFX_TDLSENDTASKSDLG_H__02DB12DD_A125_4754_AE1C_F8BB9EE45903__INCLUDED_)
#define AFX_TDLSENDTASKSDLG_H__02DB12DD_A125_4754_AE1C_F8BB9EE45903__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLSendTasksDlg.h : header file
//

#include "TaskSelectionDlg.h"
#include "TDLPrintDialog.h"
#include "TDCImportExportMgr.h"

#include "..\Interfaces\ImportExportComboBox.h"

/////////////////////////////////////////////////////////////////////////////

enum TD_SENDAS
{
	TDSA_TASKLIST,
	TDSA_BODYTEXT,	
};

/////////////////////////////////////////////////////////////////////////////
// CTDLSendTasksDlg dialog

class CTDLSendTasksDlg : public CTDLDialog
{
// Construction
public:
	CTDLSendTasksDlg(const CTDCImportExportMgr& mgr, 
					 BOOL bSelectedTasks, 
					 BOOL bEnableSubtaskSelection,
					 const CTDCCustomAttribDefinitionArray& aAttribDefs, 
					 CWnd* pParent = NULL);   // standard constructor

	TD_SENDAS GetSendAs() const { return (TD_SENDAS)m_nSendTasksAsOption; }
	CString GetFormatTypeID() const { return m_sFormatTypeID; }
	TDLPD_STYLE GetHtmlStyle() const { return m_nHtmlStyle; } 

	const CTaskSelectionDlg& GetTaskSelection() const { return m_dlgTaskSel; }

protected:
// Dialog Data
	//{{AFX_DATA(CTDLSendTasksDlg)
	enum { IDD = IDD_SENDTASKS_DIALOG };
	//}}AFX_DATA

	TDLPD_STYLE m_nHtmlStyle;
	int	m_nSendTasksAsOption;
	CString	m_sFormatTypeID;

	CImportExportComboBox m_cbFormat;
	CTaskSelectionDlg m_dlgTaskSel;
	CTDLHtmlStyleStatic m_stHtmlOptionIcon;
	CTDLHtmlStyleComboBox m_cbHtmlOptions;

	const CTDCImportExportMgr& m_mgrImportExport;

protected:
// Overrides
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLSendTasksDlg)
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTDLSendTasksDlg)
	//}}AFX_MSG
	afx_msg void OnSelChangeFormat();
	afx_msg void OnSelChangeHtmlStyle();
	DECLARE_MESSAGE_MAP()

protected:
	void UpdateHtmlOptionsVisibility();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLSENDTASKSDLG_H__02DB12DD_A125_4754_AE1C_F8BB9EE45903__INCLUDED_)
