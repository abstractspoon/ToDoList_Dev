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

	TDC_FILE LoadTasklist(const CString& sFilePath);
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
public:
	CTDLTasklistImportDlg(const CString& sFilePath, CWnd* pParent = NULL);

	IIMPORTEXPORT_RESULT GetSelectedTasks(ITaskList* pTasks);

protected:
	BOOL m_bResetCreationDate;
	CString m_sFilePath;
	BOOL m_bImportSubtasks;
	BOOL m_bFirstShow;

	CFileEdit	m_eFilePath;
	CTDLTaskTreeImportCtrl m_taskTree;
	CTaskFile m_tasksSelected;
	TDC_FILE m_nLoadRes;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnRepositionControls(int dx, int dy);
	virtual BOOL DoIdleProcessing();

	afx_msg void OnSelectall();
	afx_msg void OnSelectnone();
	afx_msg LRESULT OnTDCNotifySelectionChange(WPARAM wp, LPARAM lp);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

	void ResetSelectedTaskCreationDate(HTASKITEM hTask, BOOL bAndSiblings);
};

#endif // !defined(AFX_TDLTASKLISTIMPORTDLG_H__F9A9753D_D022_4FC7_B781_DB11A4B4A6E4__INCLUDED_)
