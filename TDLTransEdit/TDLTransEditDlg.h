// TDLTransEditDlg.h : header file
//

#if !defined(AFX_TDLTRANSEDITDLG_H__0F67B66D_B692_4CF0_B237_D0698B2E44ED__INCLUDED_)
#define AFX_TDLTRANSEDITDLG_H__0F67B66D_B692_4CF0_B237_D0698B2E44ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////

#include "TDLTransEditListCtrl.h"

#include "..\transtext\transdictionary.h"

#include "..\shared\WindowIcons.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\shortcutmanager.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLTransEditDlg dialog

class CTDLTransEditDlg : public CDialog, public CDialogHelper
{
// Construction
public:
	CTDLTransEditDlg(LPCTSTR szAppVer, CWnd* pParent = NULL);	// standard constructor

protected:
// Dialog Data
	//{{AFX_DATA(CTDLTransEditDlg)
	enum { IDD = IDD_TDLTRANSEDIT_DIALOG };
	CString	m_sFilter;
	//}}AFX_DATA
	CTDLTransEditListCtrl m_lcDictItems;
	CTransDictionary m_dictionary;
	CWindowIcons m_icons;
	CShortcutManager m_mgrShortcuts;

	BOOL m_bEdited;
	CString m_sBaseTitle;
	CString m_sEnglish;
	CString m_sTranslation;
	CString m_sYourLanguagePath;
	CString m_sLastBrowsePath;
	CString m_sAppVer;
	BOOL m_bShowAlternatives;
	BOOL m_bShowTooltips;
	BOOL m_bSortUntranslatedAtTop;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLTransEditDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();

	// Generated message map functions
	//{{AFX_MSG(CTDLTransEditDlg)
	afx_msg void OnOptionsShowTooltips();
	//}}AFX_MSG
	afx_msg void OnToolsCleanUp();
	afx_msg void OnFileNewTranslation();
	afx_msg void OnFileOpenTranslation();
	afx_msg void OnFileSaveTranslation();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEndlabeleditDictionary(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFileExit();
	afx_msg void OnClose();
	afx_msg void OnShowAlternatives();
	afx_msg void OnUpdateFilter();
	afx_msg void OnClearFilter();
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnListItemChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnChangeTranslation();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnOptionsSortUntranslatedAtTop();
	DECLARE_MESSAGE_MAP()

protected:
	void Resize(int cx = 0, int cy = 0);
	void UpdateCaption();
	BOOL PromptAndSave();
	void LoadState();
	void SaveState();
	BOOL LoadDictionary(LPCTSTR szDictPath, BOOL bAllowPrompt = TRUE);
	BOOL RecheckYourLanguagePath(LPCTSTR szDictPath, BOOL bAllowPrompt = TRUE);
	BOOL ModifyDictionaryItem(int nItem, const CString& sTrans);
	CString GetTranslationVersion(const CString& sTransPath) const;
	BOOL IsReadOnly() const;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLTRANSEDITDLG_H__0F67B66D_B692_4CF0_B237_D0698B2E44ED__INCLUDED_)
