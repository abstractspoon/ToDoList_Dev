#if !defined(AFX_OUTLOOKIMPORTITEMSDLG_H__E9D6C9F1_A424_4CB8_9AEF_0CE8641F1C53__INCLUDED_)
#define AFX_OUTLOOKIMPORTITEMSDLG_H__E9D6C9F1_A424_4CB8_9AEF_0CE8641F1C53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutlookImportDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////

#include "TDLDialog.h"

#include "..\Shared\wndprompt.h"
#include "..\Shared\enimagelist.h"

#include "..\Interfaces\ITasklist.h"
#include "..\Interfaces\IImportExport.h"

/////////////////////////////////////////////////////////////////////////////

namespace OutlookAPI
{
	class _Application;
	class MAPIFolder;
	class _Item;
}

class ITASKLISTBASE;
class ITaskList;
class IPreferences;

typedef void* HTASKITEM;

/////////////////////////////////////////////////////////////////////////////
// COutlookImportDlg dialog

class CTDLOutlookImportItemsDlg : public CTDLDialog
{
// Construction
public:
	CTDLOutlookImportItemsDlg(CWnd* pParent = NULL);

	IIMPORTEXPORT_RESULT ImportTasks(ITaskList* pDestTaskFile, IPreferences* pPrefs, LPCTSTR szKey);

// Dialog Data
	BOOL	m_bRemoveOutlookTasks;
	CString	m_sCurFolder;
	BOOL	m_bHideUnflaggedEmails;

	CTreeCtrl	m_tcTasks;
	ITASKLISTBASE* m_pDestTaskFile;
	OutlookAPI::_Application* m_pOutlook;
	OutlookAPI::MAPIFolder* m_pFolder;
	CWndPromptManager m_wndPrompt;
	CEnImageList m_ilItemTypes;
	CMap<int, int, int, int&> m_mapItemTypes;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnChoosefolder();
	afx_msg void OnClickTasklist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHideUnflaggedEmails();
	afx_msg void OnRefresh();
	DECLARE_MESSAGE_MAP()

protected:
	void AddFolderItemsToTree(OutlookAPI::MAPIFolder* pFolder, HTREEITEM htiParent = NULL);
	BOOL DeleteItemFromFolder(OutlookAPI::_Item& obj, OutlookAPI::MAPIFolder* pFolder);
	void AddTreeItemsToTasks(HTREEITEM htiParent, HTASKITEM hTaskParent, OutlookAPI::MAPIFolder* pFolder);
	void SetChildItemsChecked(HTREEITEM hti, BOOL bChecked);
	BOOL BuildItemImageList();
	int GetItemImage(int nClass) const;

};

#endif // !defined(AFX_OUTLOOKIMPORTITEMSDLG_H__E9D6C9F1_A424_4CB8_9AEF_0CE8641F1C53__INCLUDED_)
