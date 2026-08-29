#if !defined(AFX_TDLSELECTTASKDLG_H__135C804F_9469_4B97_AAE1_F05C79DE41DF__INCLUDED_)
#define AFX_TDLSELECTTASKDLG_H__135C804F_9469_4B97_AAE1_F05C79DE41DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLBrowseForTaskDlg.h : header file
//

#include "TDLDialog.h"
#include "TDLTaskComboBox.h"

/////////////////////////////////////////////////////////////////////////////

class CTaskFile;
class CTDCImageList;

/////////////////////////////////////////////////////////////////////////////
// CTDLSelectTaskDlg dialog

class CTDLSelectTaskDlg : public CTDLDialog
{
public:
	CTDLSelectTaskDlg(const CTaskFile& tasks, const CTDCImageList& ilTasks, LPCTSTR szPrefsKey = NULL, CWnd* pParent = NULL);

	DWORD GetSelectedTaskID() { return m_dwSelTaskID; }
	void SetSelectedTaskID(DWORD dwTaskID) { m_dwSelTaskID = dwTaskID; }

	void SetShowParentTasksAsFolders(BOOL bAsFolders = TRUE) { m_cbTasks.SetShowParentTasksAsFolders(bAsFolders); }
	void SetStrikethroughCompletedTasks(BOOL bStrikeThru = TRUE) { m_cbTasks.SetStrikethroughCompletedTasks(bStrikeThru); }
	void SetCompletedTaskColor(COLORREF crDone) { m_cbTasks.SetCompletedTaskColor(crDone); }

	int DoModal(HICON hIcon = NULL, UINT nTitleStrID = 0); // Caller owns icon

protected:
	CTDLTaskComboBox m_cbTasks;
	CDWordArray m_aRecentTaskIDs;

	const CTaskFile& m_tasks;
	const CTDCImageList& m_ilTasks;

	DWORD m_dwSelTaskID;
	UINT m_nTitleStrID;
	BOOL m_bShowDoneTasks;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual BOOL DoIdleProcessing();

protected:
	afx_msg void OnSelChangeTask();
	afx_msg void OnEditUpdateTask();
	afx_msg void OnDoubleClickTask();
	afx_msg void OnShowDoneTasks();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()

protected:
	void EnableDisableOK();

private:
	BOOL m_bDoIdleSelChange;

};

#endif // !defined(AFX_TDLSELECTTASKDLG_H__135C804F_9469_4B97_AAE1_F05C79DE41DF__INCLUDED_)
