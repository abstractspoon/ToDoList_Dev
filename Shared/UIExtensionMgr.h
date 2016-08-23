// ImportExportMgr.h: interface for the CUIExtensionMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UIEXTENSIONMGR_H__C258D849_69ED_46A7_A2F0_351C5C9FB3B3__INCLUDED_)
#define AFX_UIEXTENSIONMGR_H__C258D849_69ED_46A7_A2F0_351C5C9FB3B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Interfaces\IUIExtension.h"

#include <afxtempl.h>

class IUIExtension;

class CUIExtensionMgr  
{
public:
	CUIExtensionMgr();
	virtual ~CUIExtensionMgr();
	
	virtual void Release();
	void UpdateLocalizer();

	int GetNumUIExtensions() const;
	int FindUIExtension(LPCTSTR szTypeID) const;

	CString GetUIExtensionMenuText(int nExtension) const;
	HICON GetUIExtensionIcon(int nExtension) const;
	CString GetUIExtensionTypeID(int nExtension) const;
	IUIExtension* GetUIExtension(int nExtension) const;

	int GetExtensionTypeIDs(CStringArray& aTypeIDs) const;
	
	void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;
	void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);

protected:
	BOOL m_bInitialized;
	CArray<IUIExtension*, IUIExtension*> m_aUIExtensions;
	
protected:
	virtual void Initialize() const;

};

#endif // !defined(AFX_UIEXTENSIONMGR_H__C258D849_69ED_46A7_A2F0_351C5C9FB3B3__INCLUDED_)
