#if !defined(AFX_TDLCOMMENTCTRL_H__852964E3_4ABD_4B66_88BA_F553177616F2__INCLUDED_)
#define AFX_TDLCOMMENTCTRL_H__852964E3_4ABD_4B66_88BA_F553177616F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tdlcontenttypecombobox.h"

#include "..\shared\runtimedlg.h"
#include "..\shared\contentctrl.h"
#include "..\Shared\binarydata.h"
#include "..\Shared\UIThemeFile.h"
#include "..\shared\wndPrompt.h"

/////////////////////////////////////////////////////////////////////////////
// PreferencesTaskDefPage.h : header file

class CContentMgr;

/////////////////////////////////////////////////////////////////////////////

class CTDLCommentsCtrl : public CRuntimeDlg
{
	DECLARE_DYNAMIC(CTDLCommentsCtrl)

public:
	CTDLCommentsCtrl(BOOL bLabel, int nComboLenDLU, const CContentMgr* pMgrContent = NULL);
	virtual ~CTDLCommentsCtrl();

	BOOL Create(CWnd* pParent, UINT nID, const CRect& rPos = CRect(0, 0, 0, 0));
	void SetUITheme(const CUIThemeFile& theme);
	void SetDefaultCommentsFont(HFONT hFont);
	void SetCtrlStates(RT_CTRLSTATE nComboState, RT_CTRLSTATE nCommentsState);
	void SetPreferencesFilePath(LPCTSTR szFilePath);
	void SetWindowPrompts(LPCTSTR szComboPrompt, LPCTSTR szCommentsPrompt);

	int GetSelectedFormat(CONTENTFORMAT& cf) const;
	int SetSelectedFormat(const CONTENTFORMAT& cf);
	BOOL IsFormat(const CONTENTFORMAT& cf) const { return m_ctrlComments.IsFormat(cf); }

	BOOL GetContent(CString& sTextContent, CBinaryData& customContent) const;
	BOOL SetContent(const CString& sTextContent, const CBinaryData& customContent, BOOL bResetSelection);
	void ClearContent();
	
	BOOL Undo() { return m_ctrlComments.Undo(); }
	BOOL Redo() { return m_ctrlComments.Redo(); }

	BOOL ProcessMessage(MSG* pMsg) { return m_ctrlComments.ProcessMessage(pMsg); }
	BOOL HasFocus() const { return m_ctrlComments.HasFocus(); }

	ISpellCheck* GetSpellCheckInterface() { return m_ctrlComments.GetSpellCheckInterface(); }

protected:
	const CContentMgr* m_pMgrContent;

	CTDLContentTypeComboBox	m_cbCommentsFmt;
	CContentCtrl m_ctrlComments;
	CUIThemeFile m_theme;
	CBrush m_brBack;
	HFONT m_hDefaultFont;

	CWndPromptManager m_mgrPrompts;
	CString m_sCommentsPrompt, m_sComboPrompt;

	BOOL m_bFirstLoadCommentsPrefs;
	CString m_sPrefsFilePath;
	BOOL m_bReadOnly;

	CONTENTFORMAT m_cfLastCustom;
	CBinaryData m_LastCustomComments;

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

	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsDoHelp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsKillFocus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsWantSpellCheck(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsGetClipboard(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsHasClipboard(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsTaskLink(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsFailedLink(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsGetTooltip(WPARAM wParam, LPARAM lParam);

protected:
	void CalcCommentsCtrlRect(CRect& rCtrl, int cx = 0, int cy = 0) const;
	BOOL UpdateControlFormat();
	CString GetPreferencesKey() const;
	void SavePreferences() const;
	void LoadPreferences();

public:
	afx_msg void OnEnable(BOOL bEnable);
};

#endif // AFX_TDLCOMMENTCTRL_H__852964E3_4ABD_4B66_88BA_F553177616F2__INCLUDED_
