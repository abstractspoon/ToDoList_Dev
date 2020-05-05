#if !defined(AFX_TDCSIMPLETEXTCONTENT_H__E9A5D982_4D40_43B2_A071_E3BE70D122B7__INCLUDED_)
#define AFX_TDCSIMPLETEXTCONTENT_H__E9A5D982_4D40_43B2_A071_E3BE70D122B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// tdlcommentsctrl.h : header file
//

#include "..\shared\icon.h"
#include "..\shared\urlricheditctrl.h"

#include "..\Interfaces\richeditspellcheck.h"
#include "..\Interfaces\IContentControl.h"

/////////////////////////////////////////////////////////////////////////////
// CTDCSimpleTextContent

class CTDCSimpleTextContent : public IContent
{
public:
	CTDCSimpleTextContent();
	virtual ~CTDCSimpleTextContent();

	LPCTSTR GetTypeID() const;
	LPCTSTR GetTypeDescription() const;
	HICON GetTypeIcon() const;

	IContentControl* CreateCtrl(unsigned short nCtrlID, unsigned long nStyle, 
								long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent);

	void SetLocalizer(ITransText* pTT);
	void Release();

	int ConvertToHtml(const unsigned char* pContent, int nLength, LPCTSTR szCharset,
						LPTSTR& szHtml, LPCTSTR szImageDir);
	void FreeHtmlBuffer(LPTSTR& szHtml);

	void SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const;
	void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly);

protected:
	CIcon m_icon;
};

/////////////////////////////////////////////////////////////////////////////
// CTDLSimpleTextContentCtrl window

class CTDLSimpleTextContentCtrl : public CUrlRichEditCtrl, public IContentControl
{
// Construction
public:
	CTDLSimpleTextContentCtrl();
	
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
	bool ProcessMessage(MSG* pMsg);
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

// Attributes
protected:
	BOOL m_bWordWrap;
	CRichEditSpellCheck m_reSpellCheck;

	static BOOL s_bInlineSpellChecking;
	static BOOL s_bPasteSourceUrls;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLSimpleTextContentCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTDLSimpleTextContentCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDLSimpleTextContentCtrl)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
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

protected:
	BOOL IsTDLClipboardEmpty() const;
	void SetWordWrap(BOOL bWrap);
	BOOL Paste();
	BOOL CanPaste();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDCSIMPLETEXTCONTENT_H__E9A5D982_4D40_43B2_A071_E3BE70D122B7__INCLUDED_)
