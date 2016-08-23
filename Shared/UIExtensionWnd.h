// UIExtensionWnd.h: interface for the CUIExtensionWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UIEXTENSIONWND_H__9655FE6E_C8A5_4051_AAC6_EB3C8E566B31__INCLUDED_)
#define AFX_UIEXTENSIONWND_H__9655FE6E_C8A5_4051_AAC6_EB3C8E566B31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class IUIExtensionWindow;
class IPreferences;
struct UITHEME;

class CUIExtensionWnd  
{
public:
	CUIExtensionWnd(IUIExtensionWindow* pExtensionWnd = NULL);
	virtual ~CUIExtensionWnd();

public:
	BOOL Attach(IUIExtensionWindow* pExtensionWnd);

	LPCTSTR GetTypeID() const;

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
	void SetUITheme(const UITHEME* pTheme);
	
	void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;
	void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);

protected:
	IUIExtensionWindow* m_pExtensionWnd;
};

#endif // !defined(AFX_UIEXTENSIONWND_H__9655FE6E_C8A5_4051_AAC6_EB3C8E566B31__INCLUDED_)
