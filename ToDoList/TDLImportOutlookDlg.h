#if !defined(AFX_OUTLOOKIMPORTDLG_H__E9D6C9F1_A424_4CB8_9AEF_0CE8641F1C53__INCLUDED_)
#define AFX_OUTLOOKIMPORTDLG_H__E9D6C9F1_A424_4CB8_9AEF_0CE8641F1C53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutlookImportDlg.h : header file
//

#include "..\Shared\wndprompt.h"

#include "..\Interfaces\IImportExport.h"

/////////////////////////////////////////////////////////////////////////////

// predecs
namespace OutlookAPI
{
	class _Application;
	class MAPIFolder;
	class _Item;
}

class ITaskList10;
class ITaskList;
class IPreferences;

typedef void* HTASKITEM;

/////////////////////////////////////////////////////////////////////////////
// COutlookImportDlg dialog

class CTDLImportOutlookDlg : public CDialog
{
// Construction
public:
	CTDLImportOutlookDlg(CWnd* pParent = NULL);   // standard constructor

	IIMPORT_RESULT ImportTasks(ITaskList* pDestTaskFile, IPreferences* pPrefs, LPCTSTR szKey);

// Dialog Data
	//{{AFX_DATA(COutlookImportDlg)
	BOOL	m_bRemoveOutlookTasks;
	CString	m_sCurFolder;
	BOOL	m_bHideUnflaggedEmails;
	//}}AFX_DATA
	CTreeCtrl	m_tcTasks;
	ITaskList10* m_pDestTaskFile;
	OutlookAPI::_Application* m_pOutlook;
	OutlookAPI::MAPIFolder* m_pFolder;
	CWndPromptManager m_wndPrompt;
	CImageList m_ilItemTypes;
	CMap<int, int, int, int&> m_mapItemTypes;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutlookImportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnOK();

// Implementation
protected:
	int DoModal() { return CDialog::DoModal(); }

	// Generated message map functions
	//{{AFX_MSG(COutlookImportDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnChoosefolder();
	afx_msg void OnClickTasklist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHideUnflaggedEmails();
	afx_msg void OnRefresh();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	static time_t ConvertDate(DATE date);

	void AddFolderItemsToTree(OutlookAPI::MAPIFolder* pFolder, HTREEITEM htiParent = NULL);
	BOOL DeleteItemFromFolder(OutlookAPI::_Item& obj, OutlookAPI::MAPIFolder* pFolder);
	void AddTreeItemsToTasks(HTREEITEM htiParent, HTASKITEM hTaskParent, OutlookAPI::MAPIFolder* pFolder);
	void SetChildItemsChecked(HTREEITEM hti, BOOL bChecked);
	BOOL BuildItemImageList();
	int GetItemImage(int nClass) const;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTLOOKIMPORTDLG_H__E9D6C9F1_A424_4CB8_9AEF_0CE8641F1C53__INCLUDED_)
