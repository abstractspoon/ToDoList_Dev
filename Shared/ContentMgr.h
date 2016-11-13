// ContentMgr.h: interface for the CContentMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTENTMGR_H__C258D849_69ED_46A7_A2F0_351C5C9FB3B3__INCLUDED_)
#define AFX_CONTENTMGR_H__C258D849_69ED_46A7_A2F0_351C5C9FB3B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "contentctrl.h"
#include <afxtempl.h>

class IContent;
class CBinaryData;

class CContentMgr  
{
public:
	CContentMgr();
	virtual ~CContentMgr();
	
	virtual void Release();
	void UpdateLocalizer();

	int GetNumContent() const;
	int FindContent(LPCTSTR szID) const;
	BOOL SomePluginsHaveBadversions() const;
	
	CString GetContentID(int nContent) const;
	CString GetContentDescription(int nContent) const;
	CONTENTFORMAT GetContentFormat(int nContent) const;
	HICON GetContentIcon(int nContent) const;

	BOOL ContentFormatIsText(int nContent) const;
	BOOL ContentFormatIsText(const CString& sTypeID) const;

	BOOL CreateContentControl(int nContent, CContentCtrl& ctrl, UINT nCtrlID, DWORD nStyle, 
							 DWORD dwExStyle, const CRect& rect, HWND hwndParent);
	BOOL CreateContentControl(const CONTENTFORMAT& cf, CContentCtrl& ctrl, UINT nCtrlID, DWORD nStyle, 
							 DWORD dwExStyle, const CRect& rect, HWND hwndParent);

	BOOL ConvertContentToHtml(const CBinaryData& content, CString& sHtml, LPCTSTR szID, LPCTSTR szCharSet, LPCTSTR szImageDir = NULL);
	
	void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;
	void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey, BOOL bAppOnly);
	
protected:
	BOOL m_bInitialized;
	BOOL m_bSomeBadVersions;
	CArray<IContent*, IContent*> m_aContent;
	
protected:
	virtual void Initialize() const;
};

#endif // !defined(AFX_CONTENTMGR_H__C258D849_69ED_46A7_A2F0_351C5C9FB3B3__INCLUDED_)
