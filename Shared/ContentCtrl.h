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
class CUIThemeFile;

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

	int GetContent(unsigned char* pContent) const;
	int GetContent(CBinaryData& sContent) const;
	int GetTextContent(CString& sContent) const;

	BOOL SetContent(const unsigned char* pContent, int nLength, BOOL bResetSelection);
	BOOL SetContent(const CBinaryData& sContent, BOOL bResetSelection);
	BOOL SetTextContent(LPCTSTR szContent, BOOL bResetSelection);
	BOOL IsSettingContent() const { return m_bSettingContent; }

	LPCTSTR GetTypeID() const;
	BOOL IsFormat(const CONTENTFORMAT& cf) const;
	CONTENTFORMAT GetContentFormat() const;

	BOOL SetReadOnly(BOOL bReadOnly);
	
	BOOL ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);
	BOOL ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);

	operator HWND() const;
	HWND GetSafeHwnd() const { return *this; }

	UINT GetDlgCtrlID() { return ::GetDlgCtrlID(*this); }

	LRESULT SendMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
	BOOL PostMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);

	BOOL PreTranslateMessage(MSG* pMsg);
	BOOL HasFocus() const;
	void SetFocus();

	ISpellCheck* GetSpellCheckInterface();
	
	BOOL Undo();
	BOOL Redo();

	void SetUITheme(const CUIThemeFile& theme);
	
	void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;
	void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);

protected:
	IContentControl* m_pContentCtrl;
	BOOL m_bSettingContent;
	CString m_sTypeID;
};

#endif // !defined(AFX_CONTENTCTRL_H__BE763781_9932_4582_B3D0_64DEE59B1A92__INCLUDED_)
