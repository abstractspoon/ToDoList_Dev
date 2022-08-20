#if !defined(AFX_PREFERENCESTOOLPAGE_H__6B9BF0B2_D6DE_4868_B97E_8F6288C77778__INCLUDED_)
#define AFX_PREFERENCESTOOLPAGE_H__6B9BF0B2_D6DE_4868_B97E_8F6288C77778__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreferencesToolPage.h : header file
//

#include "tdcstruct.h"
#include "TDCToolsHelper.h"

#include "..\shared\fileedit.h"
#include "..\shared\preferencesbase.h"
#include "..\shared\menubutton.h"
#include "..\shared\ToolbarHelper.h"
#include "..\shared\entoolbar.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

const UINT WM_PTP_TESTTOOL = ::RegisterWindowMessage(_T("WM_PTP_TESTTOOL"));

/////////////////////////////////////////////////////////////////////////////

class CTDCCustomAttribDefinitionArray;

/////////////////////////////////////////////////////////////////////////////
// CPreferencesToolPage dialog

class CPreferencesToolPage : public CPreferencesPageBase
{
	DECLARE_DYNCREATE(CPreferencesToolPage)

// Construction
public:
	CPreferencesToolPage(int nMaxNumTools = 50);
	~CPreferencesToolPage();

	int GetUserTools(CUserToolArray& aTools) const; // returns the number of tools
	BOOL GetUserTool(int nTool, USERTOOL& tool) const; 
	BOOL GetDisplayUDTsInToolbar() const { return m_bDisplayUDTsInToolbar; }
	void SetCustomAttributeDefs(const CTDCCustomAttribDefinitionArray& aAttribDefs);

protected:

// Dialog Data
	//{{AFX_DATA(CPreferencesToolPage)
	enum { IDD = IDD_PREFTOOLS_PAGE };
	//}}AFX_DATA
	CMenuButton	m_btnArgMenu;
	CEnToolBar m_toolbar;
	CToolbarHelper m_tbHelper;
	CEdit	m_eCmdLine;
	CFileEdit	m_eToolPath;
	CListCtrl	m_lcTools;
	CFileEdit	m_eIconPath;

	int m_nMaxNumTools;
	BOOL m_bDisplayUDTsInToolbar;
	CString	m_sToolPath;
	CString	m_sCommandLine;
	BOOL	m_bRunMinimized;
	CString		m_sIconPath;
	
	CUserToolArray m_aTools;
	CStringArray m_aMenuCustomAttribIDs;
	CStringArray m_aMenuUserVariableIDs;
	CTDCCustomAttribDefinitionArray m_aCustomAttribDefs;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPreferencesToolPage)
	//}}AFX_VIRTUAL

protected:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnFirstShow();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPreferencesToolPage)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	//}}AFX_MSG
	afx_msg void OnNewExternalTool();
	afx_msg void OnNewTDLTool();
	afx_msg void OnDeleteTool();
	afx_msg void OnEditToolName();
	afx_msg void OnCopyTool();
	afx_msg void OnMoveToolUp();
	afx_msg void OnMoveToolDown();
	afx_msg void OnUpdateCmdUINewExternalTool(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCmdUINewTDLTool(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCmdUIDeleteTool(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCmdUIEditToolName(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCmdUICopyTool(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCmdUIMoveToolUp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCmdUIMoveToolDown(CCmdUI* pCmdUI);

	afx_msg void OnEndlabeleditToollist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedToollist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeToolpath();
	afx_msg void OnKeydownToollist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeCmdline();
	afx_msg void OnInsertPlaceholder(UINT nCmdID);
	afx_msg void OnInsertUserVariable(UINT nCmdID);
	afx_msg void OnInsertCustomAttribute(UINT nCmdID);
	afx_msg void OnRunminimized();
	afx_msg void OnTestTool();
	afx_msg void OnChangeIconPath();
	afx_msg void OnImportTools();
	afx_msg LRESULT OnGetFileIcon(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	virtual void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);
	virtual void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;

	int AddNewTool(BOOL bEditLabel);
	void EnableControls();
	int GetCurSel() const;
	BOOL SetCurSel(int nTool);
	void RebuildListCtrlImages();
	BOOL InitializeToolbar();
	BOOL GetListTool(int nTool, USERTOOL& tool) const;
	int AddListTool(const USERTOOL& tool, int nPos = -1, BOOL bRebuildImages = FALSE);
	
	CString MapCmdIDToPlaceholder(UINT nCmdID) const;
	CString GetNewUserVariableName(LPCTSTR szVarType) const;
	CString FormatUserPlaceHolder(LPCTSTR szPlaceHolder, LPCTSTR szVarType, UINT nIDTextPrompt) const;

	static CLA_TYPE MapCmdIDToType(UINT nCmdID);
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCESTOOLPAGE_H__6B9BF0B2_D6DE_4868_B97E_8F6288C77778__INCLUDED_)
