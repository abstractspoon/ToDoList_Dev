#if !defined(AFX_TDLTASKLISTIMPORTDLG_H__F9A9753D_D022_4FC7_B781_DB11A4B4A6E4__INCLUDED_)
#define AFX_TDLTASKLISTIMPORTDLG_H__F9A9753D_D022_4FC7_B781_DB11A4B4A6E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLTasklistImportDlg.h : header file
//

#include "FilteredToDoCtrl.h"
#include "TaskFile.h"

#include "..\shared\fileedit.h"

#include "..\Interfaces\IImportExport.h"

/////////////////////////////////////////////////////////////////////////////

class CTDLTasklistImportCtrl : public CFilteredToDoCtrl
{
public:
	CTDLTasklistImportCtrl();
	void DeselectAll();
	void ShowAllColumns();
};

/////////////////////////////////////////////////////////////////////////////
// CTDLTasklistImportDlg dialog

class CTDLTasklistImportDlg : public CDialog
{
// Construction
public:
	CTDLTasklistImportDlg(const CString& sFilePath, CWnd* pParent = NULL);   // standard constructor

	IIMPORT_RESULT GetSelectedTasks(ITaskList8* pTasks);

protected:
// Dialog Data
	//{{AFX_DATA(CTDLTasklistImportDlg)
	BOOL	m_bResetCreationDate;
	CString	m_sFilePath;
	BOOL	m_bImportSubtasks;
	//}}AFX_DATA

	CFileEdit	m_eFilePath;
	CTDLTasklistImportCtrl m_tdc;
	CTaskFile m_tasksSelected;
	TDC_FILE m_nLoadRes;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLTasklistImportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	// Generated message map functions
	//{{AFX_MSG(CTDLTasklistImportDlg)
	afx_msg void OnSelectall();
	afx_msg void OnSelectnone();
	//}}AFX_MSG
	afx_msg LRESULT OnTDCNotifySelectionChange(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

	void ResetSelectedTaskCreationDate(HTASKITEM hTask, BOOL bAndSiblings);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLTASKLISTIMPORTDLG_H__F9A9753D_D022_4FC7_B781_DB11A4B4A6E4__INCLUDED_)
