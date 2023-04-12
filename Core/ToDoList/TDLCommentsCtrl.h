#if !defined(AFX_TDLCOMMENTCTRL_H__852964E3_4ABD_4B66_88BA_F553177616F2__INCLUDED_)
#define AFX_TDLCOMMENTCTRL_H__852964E3_4ABD_4B66_88BA_F553177616F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "..\shared\runtimedlg.h"
#include "..\Shared\binarydata.h"
#include "..\shared\wndPrompt.h"
#include "..\shared\entoolbar.h"
#include "..\shared\toolbarhelper.h"

#include "..\Interfaces\UIThemeFile.h"
#include "..\Interfaces\contentctrl.h"
#include "..\Interfaces\contenttypecombobox.h"

/////////////////////////////////////////////////////////////////////////////
// PreferencesTaskDefPage.h : header file

class CTDLContentMgr;
class CShortcutManager;

/////////////////////////////////////////////////////////////////////////////

class CTDLCommentsCtrl : public CRuntimeDlg
{
	DECLARE_DYNAMIC(CTDLCommentsCtrl)

public:
	CTDLCommentsCtrl(BOOL bShowLabel, BOOL bShowToolbar, int nComboLenDLU, 
					 const CTDLContentMgr* pMgrContent = NULL, const CShortcutManager* pMgrShortcuts = NULL);
	virtual ~CTDLCommentsCtrl();

	BOOL Create(CWnd* pParent, UINT nID, const CRect& rPos = CRect(0, 0, 0, 0));
	BOOL IsCommentsEditable() const;

	void SetUITheme(const CUIThemeFile& theme);
	void SetContentFont(HFONT hFont);
	void SetCtrlStates(RT_CTRLSTATE nComboState, RT_CTRLSTATE nCommentsState);
	void SetPreferencesFilePath(LPCTSTR szFilePath);
	void SetWindowPrompts(LPCTSTR szComboPrompt, LPCTSTR szCommentsPrompt);

	BOOL GetSelectedFormat(CONTENTFORMAT& cf) const;
	BOOL SetSelectedFormat(const CONTENTFORMAT& cf);
	BOOL IsFormat(const CONTENTFORMAT& cf) const { return m_ctrlComments.IsFormat(cf); }
	BOOL IsUpdatingFormat() const { return m_bUpdatingFormat; }

	BOOL GetContent(CString& sTextContent, CBinaryData& customContent) const;
	BOOL SetContent(const CString& sTextContent, const CBinaryData& customContent, BOOL bResetSelection);
	BOOL InsertTextContent(LPCWSTR szContent, BOOL bAtEnd);
	void ClearContent();

	BOOL FindReplaceAll(LPCTSTR szFind, LPCTSTR szReplace, BOOL bCaseSensitive, BOOL bWholeWord);

	BOOL Undo() { return m_ctrlComments.Undo(); }
	BOOL Redo() { return m_ctrlComments.Redo(); }

	BOOL ProcessMessage(MSG* pMsg) { return m_ctrlComments.ProcessMessage(pMsg); }
	BOOL HasFocus() const { return m_ctrlComments.HasFocus(); }
	void UpdateAppPreferences();

	ISpellCheck* GetSpellCheckInterface() { return m_ctrlComments.GetSpellCheckInterface(); }

protected:
	const CTDLContentMgr* m_pMgrContent;
	const CShortcutManager* m_pMgrShortcuts;

	CContentTypeComboBox m_cbCommentsFmt;
	CContentCtrl m_ctrlComments;
	CUIThemeFile m_theme;
	CBrush m_brBack;
	HFONT m_hContentFont;
	CEnToolBar m_toolbar;

	CToolbarHelper m_tbHelper;
	CWndPromptManager m_mgrPrompts;
	CString m_sCommentsPrompt, m_sComboPrompt;

	CString m_sPrefsFilePath;
	BOOL m_bReadOnlyComments;
	BOOL m_bShowLabel, m_bShowToolbar;

	CONTENTFORMAT m_cfLastCustom;
	CBinaryData m_LastCustomComments;
	BOOL m_bUpdatingFormat;

protected:
	int DoModal() { ASSERT(0); return IDCANCEL; }

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSelchangeCommentsformat();
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnEnable(BOOL bEnable);

	afx_msg LRESULT OnCommentsChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsDoHelp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsKillFocus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsWantSpellCheck(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsGetClipboard(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsHasClipboard(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsTaskLink(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsFailedLink(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsGetTooltip(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsGetAttributeList(WPARAM wParam, LPARAM lParam);

	afx_msg void OnInsertDate();
	afx_msg void OnInsertTime();
	afx_msg void OnInsertDateAndTime();

protected:
	void CalcCommentsCtrlRect(CRect& rCtrl, int cx = 0, int cy = 0) const;
	BOOL UpdateControlFormat();
	BOOL UpdateControlFormat(const CONTENTFORMAT& cf);
	CString GetPreferencesKey() const;
	void SavePreferences() const;
	void LoadPreferences(BOOL bAppOnly);


};

#endif // AFX_TDLCOMMENTCTRL_H__852964E3_4ABD_4B66_88BA_F553177616F2__INCLUDED_
