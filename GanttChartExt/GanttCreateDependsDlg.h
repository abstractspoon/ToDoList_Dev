#if !defined(AFX_GANTTCREATEDEPENDSDLG_H__813AC0DB_65CE_4CD8_823B_814838F5E898__INCLUDED_)
#define AFX_GANTTCREATEDEPENDSDLG_H__813AC0DB_65CE_4CD8_823B_814838F5E898__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GanttCreateDependsDlg.h : header file
//

#include "GanttTreeListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CGanttCreateDependsDlg dialog

enum GCDD_MODE
{
	GCDDM_NONE = -1,
	GCDDM_ADD,
	GCDDM_EDIT,
	GCDDM_DELETE,
};

/////////////////////////////////////////////////////////////////////////////

class CGanttCreateDependsDlg : public CDialog, public CGanttDependencyEditor
{
// Construction
public:
	CGanttCreateDependsDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL Create(GCDD_MODE nMode, CWnd* pOwner);
	GCDD_MODE GetMode() const;

	// CGanttDependencyEditor interface
	BOOL SetFromTask(DWORD dwFromTaskID);
	BOOL SetFromDependency(DWORD dwFromTaskID, DWORD dwCurToTaskID);
	BOOL SetToTask(DWORD dwToTaskID);
	
	DWORD GetFromTask() const;
	DWORD GetFromDependency(DWORD& dwCurToTaskID) const;
	DWORD GetToTask() const;

	BOOL IsPicking() const;

	BOOL IsPickingFromTask() const			{ return (m_nStage == GCDDS_PICKINGFROMTASK); }
	BOOL IsPickingFromDependency() const	{ return (m_nStage == GCDDS_PICKINGFROMDEPEND); }
	BOOL IsPickingToTask() const			{ return (m_nStage == GCDDS_PICKINGTOTASK); }
	BOOL IsPickingCancelled() const			{ return (m_nStage == GCDDS_PICKINGCANCELLED); }
	BOOL IsPickingCompleted() const			{ return (m_nStage == GCDDS_PICKINGCOMPLETED); }

	BOOL IsAdding() const	{ return (m_nMode == GCDDM_ADD); }
	BOOL IsEditing() const	{ return (m_nMode == GCDDM_EDIT); }
	BOOL IsDeleting() const { return (m_nMode == GCDDM_DELETE); }

	void Cancel();

protected:
// Dialog Data
	//{{AFX_DATA(CGanttCreateDependsDlg)
	//}}AFX_DATA
	CString	m_sCurStage;
	DWORD m_dwFromTaskID, m_dwCurToTaskID, m_dwNewToTaskID;
	GCDD_MODE m_nMode;

	enum GCDD_STAGE
	{
		GCDDS_PICKINGCANCELLED = -1,
		GCDDS_PICKINGFROMTASK,
		GCDDS_PICKINGFROMDEPEND,
		GCDDS_PICKINGTOTASK,
		GCDDS_PICKINGCOMPLETED,
	};
	GCDD_STAGE m_nStage;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGanttCreateDependsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnOK() {}
	virtual void OnCancel();

// Implementation
protected:
	BOOL DoModal() { ASSERT(0); return TRUE; }
	void Cancel(BOOL bDestroyWindow);

	// Generated message map functions
	//{{AFX_MSG(CGanttCreateDependsDlg)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void UpdatePrompt();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GANTTCREATEDEPENDSDLG_H__813AC0DB_65CE_4CD8_823B_814838F5E898__INCLUDED_)
