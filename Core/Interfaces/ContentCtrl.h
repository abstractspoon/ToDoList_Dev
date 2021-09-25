// ContentCtrl.h: interface for the CContentCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTENTCTRL_H__BE763781_9932_4582_B3D0_64DEE59B1A92__INCLUDED_)
#define AFX_CONTENTCTRL_H__BE763781_9932_4582_B3D0_64DEE59B1A92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class IContentControl;
class ISpellCheck;
class IPreferences;
class CBinaryData;

struct UITHEME;

class CONTENTFORMAT : public CString
{
public:
	CONTENTFORMAT(LPCTSTR szID = NULL) : CString(szID) {}
	CONTENTFORMAT(const CString& sID) : CString(sID) {}
	BOOL FormatIsText() const;
};

class CContentCtrl  
{
public:
	CContentCtrl(IContentControl* pContentCtrl = NULL);
	virtual ~CContentCtrl();

	BOOL Attach(IContentControl* pContentCtrl);
	void Release();

	int GetContent(unsigned char* pContent) const;
	int GetContent(CBinaryData& sContent) const;
	int GetTextContent(CString& sContent) const;

	BOOL SetContent(const unsigned char* pContent, int nLength, BOOL bResetSelection);
	BOOL SetContent(const CBinaryData& sContent, BOOL bResetSelection);
	BOOL SetTextContent(LPCTSTR szContent, BOOL bResetSelection);
	BOOL InsertTextContent(LPCTSTR szContent, BOOL bAtEnd = FALSE);

	BOOL IsSettingContent() const { return m_bSettingContent; }
	BOOL FindReplaceAll(LPCTSTR szFind, LPCTSTR szReplace, BOOL bCaseSensitive, BOOL bWholeWord);

	LPCTSTR GetTypeID() const;
	BOOL IsFormat(const CONTENTFORMAT& cf) const;
	CONTENTFORMAT GetContentFormat() const;

	BOOL EnableWindow(BOOL bEnable);
	BOOL SetReadOnly(BOOL bReadOnly);
	void MoveWindow(const CRect& rect, BOOL bRepaint);

	BOOL ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);
	BOOL ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);

	operator HWND() const;
	HWND GetSafeHwnd() const;

	UINT GetDlgCtrlID() { return ::GetDlgCtrlID(*this); }

	LRESULT SendMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
	BOOL PostMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);

	BOOL ProcessMessage(MSG* pMsg);
	BOOL HasFocus() const;
	void SetFocus();

	ISpellCheck* GetSpellCheckInterface();
	
	BOOL Undo();
	BOOL Redo();

	void SetUITheme(const UITHEME& theme);
	void SetContentFont(HFONT hFont);

	void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;
	void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey, BOOL bAppOnly);

protected:
	IContentControl* m_pContentCtrl;
	BOOL m_bSettingContent;
	CString m_sTypeID;
};

#endif // !defined(AFX_CONTENTCTRL_H__BE763781_9932_4582_B3D0_64DEE59B1A92__INCLUDED_)
