#if !defined(AFX_RTFCONTENTCONTROL_H__4F1A93A0_7829_4DBB_AA0B_A2F62E4E7F50__INCLUDED_)
#define AFX_RTFCONTENTCONTROL_H__4F1A93A0_7829_4DBB_AA0B_A2F62E4E7F50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RTFContentControl.h : header file
//

#include "rulerricheditctrl.h"
#include "rtfpreferencesdlg.h"

#include "..\shared\toolbarhelper.h"
#include "..\shared\richeditspellcheck.h"
#include "..\shared\subclass.h"
#include "..\shared\menuiconmgr.h"
#include "..\shared\ShortcutManager.h"

#include "..\Interfaces\IContentControl.h"

/////////////////////////////////////////////////////////////////////////////

class CRtfHtmlConverter;

/////////////////////////////////////////////////////////////////////////////
// CRTFContentControl window
class CRTFContentControl : public CRulerRichEditCtrl, public IContentControl
{
// Construction
public:
	CRTFContentControl(CRtfHtmlConverter& rtfHtml);

	// ICustomControl implementation
	int GetContent(unsigned char* pContent) const;
	bool SetContent(const unsigned char* pContent, int nLength, bool bResetSelection);
	int GetTextContent(LPTSTR szContent, int nLength = -1) const;
	bool SetTextContent(LPCTSTR szContent, bool bResetSelection);
	void SetReadOnly(bool bReadOnly);
	HWND GetHwnd() const;
	LPCTSTR GetTypeID() const { return RTF_TYPEID; }
	void Release();
	bool ProcessMessage(MSG* pMsg);
	ISpellCheck* GetSpellCheckInterface() { return &m_reSpellCheck; }
	bool Undo();
	bool Redo();
	void SetUITheme(const UITHEME* pTheme);
	void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;
	void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);

	// global settings
	static void SetPasteSourceUrls(BOOL bPasteUrls) { s_bPasteSourceUrls = bPasteUrls; }
	static void SetConvertWithMSWord(BOOL bUseMSWord) { s_bConvertWithMSWord = bUseMSWord; }
	static BOOL GetConvertWithMSWord() { return s_bConvertWithMSWord; }
	static void EnableInlineSpellChecking(BOOL bEnable) { s_bInlineSpellChecking = bEnable; }
	static BOOL IsInlineSpellCheckingEnabled() { return s_bInlineSpellChecking; }

// Attributes
protected:
	BOOL m_bAllowNotify;
	CToolbarHelper m_tbHelper;
	CRichEditSpellCheck m_reSpellCheck;
	CMenuIconMgr m_mgrMenuIcons;
	CharFormat m_cfCopiedFormat;
	CShortcutManager m_mgrShortcuts;
	CRtfHtmlConverter& m_rtfHtml;
	LOGFONT m_lfCurrent;
	CRTFPreferencesDlg m_dlgPrefs;

	static BOOL s_bPasteSourceUrls;
	static BOOL s_bConvertWithMSWord;
	static BOOL s_bInlineSpellChecking;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRTFContentControl)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRTFContentControl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRTFContentControl)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_
	afx_msg void OnEditCopy();
	afx_msg void OnEditCopyAsHtml();
	afx_msg void OnEditCopyFormatting();
	afx_msg void OnEditCut();
	afx_msg void OnEditDelete();
	afx_msg void OnEditFileBrowse();
	afx_msg void OnEditFind();
	afx_msg void OnEditFindReplace();
	afx_msg void OnEditHorzRule();
	afx_msg void OnEditOpenUrl();
	afx_msg void OnEditCopyUrl();
	afx_msg void OnEditPaste();
	afx_msg void OnEditPasteFormatting();
	afx_msg void OnEditPasteSimple();
	afx_msg void OnEditPasteasRef();
	afx_msg void OnEditSelectAll();
	afx_msg void OnEditShowRuler();
	afx_msg void OnEditShowToolbar();
	afx_msg void OnEditSpellcheck();
	afx_msg void OnEditSubscript();
	afx_msg void OnEditSuperscript();
	afx_msg void OnEditInlineSpellcheck();
	afx_msg void OnPreferences();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopyFormatting(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditFileBrowse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditFind(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditFindReplace(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditHorzRule(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditOpenCopyUrl(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPasteFormatting(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPasteSimple(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPasteasRef(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditSelectAll(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditShowRuler(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditShowToolbar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditSpellcheck(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditSubscript(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditSuperscript(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditInlineSpellcheck(CCmdUI* pCmdUI);
	afx_msg void OnChangeText();
	afx_msg void OnKillFocus();
	afx_msg LRESULT OnSetFont(WPARAM wp, LPARAM lp);
	afx_msg void OnStyleChanging(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	afx_msg LRESULT OnCustomUrl(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnFailedUrl(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnPrefsHelp(WPARAM wp, LPARAM lp);
	afx_msg void OnHelp();
	afx_msg BOOL OnHelpInfo(HELPINFO* lpHelpInfo);

	DECLARE_MESSAGE_MAP()

	BOOL CanPaste();
	BOOL Paste(BOOL bSimple);
	void PrepareForPaste(BOOL bSimple);
	BOOL IsTDLClipboardEmpty() const;
	void InitMenuIconManager();
	void InitShortcutManager();
	BOOL CopyRtfToClipboardAsHtml(const CString& sRTF, BOOL bAppend = TRUE);
	void RemoveAdvancedFeatures(CMenu* pMenu) const;

	static void EnableMenuItem(CMenu* pMenu, UINT nCmdID, BOOL bEnable);
	static void CheckMenuItem(CMenu* pMenu, UINT nCmdID, BOOL bCheck);
	static BOOL GetClipboardHtmlForPasting(CString& sHtml, CString& sSourceUrl);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RTFCONTENTCONTROL_H__4F1A93A0_7829_4DBB_AA0B_A2F62E4E7F50__INCLUDED_)
