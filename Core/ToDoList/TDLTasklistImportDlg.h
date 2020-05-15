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

class CTDLTaskTreeImportCtrl : public CTDLTaskTreeCtrl
{
public:
	CTDLTaskTreeImportCtrl();

	BOOL BuildTree(const CString& sFilePath);
	int GetSelectedTasks(CTaskFile& tasks, BOOL bWantAllSubtasks);

protected:
	CToDoCtrlData m_data;
	CTDCImageList m_ilIcons;
	TDCAUTOLISTDATA m_tld;
	CTDCStyleMap m_styles;
	TDCCOLEDITVISIBILITY m_visibleCols;
	CTDCCustomAttribDefinitionArray m_aCustAttribDefs;
	CContentMgr m_mgrContent;
	CTDCTaskExporter m_exporter;
	CFont m_font;

protected:
	void AddTasksToTree(const TODOSTRUCTURE* pTDSParent, HTREEITEM htiParent);
	void AddTreeItemToTasks(HTREEITEM htiParent, CTaskFile& tasks, BOOL bWantAllSubtasks);
};

/////////////////////////////////////////////////////////////////////////////
// CTDLTasklistImportDlg dialog

class CTDLTasklistImportDlg : public CTDLDialog
{
// Construction
public:
	CTDLTasklistImportDlg(const CString& sFilePath, CWnd* pParent = NULL);   // standard constructor

	IIMPORTEXPORT_RESULT GetSelectedTasks(ITaskList* pTasks);

protected:
// Dialog Data
	//{{AFX_DATA(CTDLTasklistImportDlg)
	enum { IDD = IDD_TDLIMPORTEXPORT_DIALOG };
	BOOL	m_bResetCreationDate;
	CString	m_sFilePath;
	BOOL	m_bImportSubtasks;
	//}}AFX_DATA

	CFileEdit	m_eFilePath;
	CTDLTaskTreeImportCtrl m_taskTree;
	CTaskFile m_tasksSelected;
	TDC_FILE m_nLoadRes;
	BOOL m_bFirstShow;

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
	virtual void OnRepositionControls(int dx, int dy);

	// Generated message map functions
	//{{AFX_MSG(CTDLTasklistImportDlg)
	afx_msg void OnSelectall();
	afx_msg void OnSelectnone();
	//}}AFX_MSG
	afx_msg LRESULT OnTDCNotifySelectionChange(WPARAM wp, LPARAM lp);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

	void ResetSelectedTaskCreationDate(HTASKITEM hTask, BOOL bAndSiblings);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLTASKLISTIMPORTDLG_H__F9A9753D_D022_4FC7_B781_DB11A4B4A6E4__INCLUDED_)
