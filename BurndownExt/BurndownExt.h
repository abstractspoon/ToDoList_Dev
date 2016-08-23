// BurndownExt.h: interface for the CBurndownExtApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BurndownEXT_H__DEE73DE1_C6EC_4648_9151_0FC2C75A806D__INCLUDED_)
#define AFX_BurndownEXT_H__DEE73DE1_C6EC_4648_9151_0FC2C75A806D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "..\Interfaces\IUIExtension.h"

//////////////////////////////////////////////////////////////////////

const LPCTSTR STATS_TYPEID = _T("0AA35779-4BB4-4151-BA8E-D471281B6A08");

//////////////////////////////////////////////////////////////////////

class CBurndownExtApp : public CWinApp, public IUIExtension  
{
public:
	CBurndownExtApp();
	virtual ~CBurndownExtApp();

    void Release(); // releases the interface
	void SetLocalizer(ITransText* pTT);

	LPCTSTR GetMenuText() const { return _T("Burndown"); }
	HICON GetIcon() const { return m_hIcon; }
	LPCTSTR GetTypeID() const { return STATS_TYPEID; }

	IUIExtensionWindow* CreateExtWindow(UINT nCtrlID, DWORD nStyle, 
										long nLeft, long nTop, long nWidth, long nHeight, 
										HWND hwndParent);
	
	void SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const;
	void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey);

protected:
	HICON m_hIcon;

protected:
	BOOL InitInstance();
};

#endif // !defined(AFX_BurndownEXT_H__DEE73DE1_C6EC_4648_9151_0FC2C75A806D__INCLUDED_)
