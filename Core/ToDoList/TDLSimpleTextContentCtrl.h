#if !defined(AFX_TDLSIMPLETEXTCONTENTCTRL_H__E9A5D982_4D40_43B2_A071_E3BE70D122B7__INCLUDED_)
#define AFX_TDLSIMPLETEXTCONTENTCTRL_H__E9A5D982_4D40_43B2_A071_E3BE70D122B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// tdlcommentsctrl.h : header file
//

#include "..\shared\urlricheditctrl.h"
#include "..\shared\menuiconmgr.h"

#include "..\Interfaces\richeditspellcheck.h"
#include "..\Interfaces\IContentControl.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLSimpleTextContentCtrl window

class CTDLSimpleTextContentCtrl : public CUrlRichEditCtrl, public IContentControl
{
// Construction
public:
	CTDLSimpleTextContentCtrl();
	virtual ~CTDLSimpleTextContentCtrl();
	
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	// IContentControl implementation
	int GetContent(unsigned char* pContent) const;
	bool SetContent(const unsigned char* pContent, int nLength, bool bResetSelection);
	LPCWSTR GetTypeID() const;
	int GetTextContent(LPTSTR szContent, int nLength = -1) const;
	bool SetTextContent(LPCTSTR szContent, bool bResetSelection);
	bool InsertTextContent(LPCWSTR szContent, bool bAtEnd);
	void SetReadOnly(bool bReadOnly);
	void Enable(bool bEnable);
	HWND GetHwnd() const;
	void Release();
	bool ProcessMessage(MSG* pMsg);
	bool DoIdleProcessing() { return false; }
	void FilterToolTipMessage(MSG* pMsg);
	ISpellCheck* GetSpellCheckInterface();
	bool Undo();
	bool Redo();
	void SetUITheme(const UITHEME* pTheme);
	void SetContentFont(HFONT hFont);
	void SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const;
	void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly);
	bool FindReplaceAll(LPCTSTR szFind, LPCTSTR szReplace, bool bCaseSensitive, bool bWholeWord);

	// global settings
	static void SetPasteSourceUrls(BOOL bPasteUrls) { s_bPasteSourceUrls = bPasteUrls; }
	static void EnableInlineSpellChecking(BOOL bEnable) { s_bInlineSpellChecking = bEnable; }
	static BOOL IsInlineSpellCheckingEnabled() { return s_bInlineSpellChecking; }

protected:
	BOOL m_bWordWrap;

	CRichEditSpellCheck m_reSpellCheck;
	CMenuIconMgr m_mgrMenuIcons;

	static BOOL s_bInlineSpellChecking;
	static BOOL s_bPasteSourceUrls;

protected:
	virtual void PreSubclassWindow();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnHelpInfo(HELPINFO* lpHelpInfo);
	afx_msg void OnCommentsMenuCmd(UINT nCmdID);
	afx_msg void OnUpdateCommentsMenuCmd(CCmdUI* pCmdUI);
	afx_msg BOOL OnChangeText();
	afx_msg BOOL OnKillFocus();
	afx_msg LRESULT OnSetWordWrap(WPARAM wp, LPARAM lp);
	afx_msg BOOL OnGetTooltip(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNcDestroy();
	afx_msg UINT OnGetDlgCode();

	DECLARE_MESSAGE_MAP()

	virtual LRESULT SendNotifyCustomUrl(LPCTSTR szUrl) const;
	virtual LRESULT SendNotifyFailedUrl(LPCTSTR szUrl) const;
	virtual CLIPFORMAT GetAcceptableClipFormat(LPDATAOBJECT lpDataOb, CLIPFORMAT format);
	virtual void OnSelectPopupListItem(const CString& sSelItem);

protected:
	BOOL IsTDLClipboardEmpty() const;
	void SetWordWrap(BOOL bWrap);
	BOOL Paste();
	BOOL CanPaste();
	void InitMenuIconManager();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLSIMPLETEXTCONTENTCTRL_H__E9A5D982_4D40_43B2_A071_E3BE70D122B7__INCLUDED_)
