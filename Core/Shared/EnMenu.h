// MenuEx.h: interface for the CEnMenu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENMENU_H__5AB11CC8_CCF5_4D52_ADC7_27FDC151F3FE__INCLUDED_)
#define AFX_ENMENU_H__5AB11CC8_CCF5_4D52_ADC7_27FDC151F3FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "EnBitmapEx.h"

enum MENUEX_BTN
{
	MEB_MINIMIZE,
	MEB_RESTORE,
	MEB_CLOSE,
};

class ITransText;

class CEnMenu : public CMenu  
{
public:
	CEnMenu();
	virtual ~CEnMenu();

	BOOL LoadMenu(UINT nMenuResID, HWND hWndRef = NULL, BOOL bTranslate = FALSE, BOOL bRecursiveTranslate = FALSE);
	void SetBackgroundColor(COLORREF color);
	
	// pass -1 as nThemeBMID is you want ownerdraw
	BOOL AddMDIButton(MENUEX_BTN nBtn, UINT nCmdID, BOOL bRightJustify = TRUE);
	BOOL DeleteMDIMenu(UINT nCmdID);

	// for themed buttons only
	BOOL DrawMDIButton(LPDRAWITEMSTRUCT lpDrawItemStruct) const; 
	BOOL MeasureMDIButton(LPMEASUREITEMSTRUCT lpMeasureItemStruct) const; 

	int FindMenuItem(UINT nCmdID) const;
	int FindMenuItem(HMENU hSubMenu) const;
	int FindFirstMenuItem(UINT nCmdIDStart, UINT nCmdIDEnd) const;

	int GetMenuString(UINT nIDItem,	CString& sItem, UINT nFlags) const;
	CString GetMenuString(UINT nIDItem,	UINT nFlags) const;
	CString GetFullItemPath(UINT nIDItem, LPCTSTR szDelim = _T("\\")) const; // MF_BYCOMMAND
	BOOL SetMenuString(UINT nIDItem, const CString& sItem, UINT nFlags);
	int GetMenuStrings(CStringArray& aItems) const;

	// have to copy base class because we provide a static overload
	CMenu* GetSubMenu(int nPos) const { return CMenu::GetSubMenu(nPos); }
	HMENU GetParentMenu(HMENU hMenu) const;

	BOOL CopyMenuContents(const CMenu* pMenu);

	BOOL IsSeparator(int nPos) const;
	BOOL IsPopop(int nPos) const;

	BOOL DeleteSubMenu(HMENU hSubMenu, BOOL bAutoCleanUp = FALSE);
	BOOL DeleteMenu(UINT nPosition, UINT nFlags, BOOL bAutoCleanUp = FALSE); // equivalent to base class
	BOOL DeleteMenuContents();
	void RemoveDuplicateSeparators(int nStartPos = 0);

	BOOL SortMenuStrings(UINT nCmdIDStart, UINT nCmdIDEnd);
	BOOL TranslateDynamicMenuItems(UINT nCmdIDStart, UINT nCmdIDEnd, LPCTSTR szFormat);
	BOOL EnsureUniqueAccelerators();

	static void SetLocalizer(ITransText* pTT);

	static int GetMenuAccelerators(HMENU hMenu, CString& sAccelerators);
	static TCHAR EnsureUniqueAccelerator(CString& sText, HMENU hMenu);
	static BOOL EnsureUniqueAccelerators(HMENU hMenu);

	static int FindMenuItem(HMENU hMenu, UINT nCmdID);
	static int FindMenuItem(HMENU hMenu, UINT nCmdID, HMENU& hParentMenu);
	static int FindMenuItem(HMENU hMenu, HMENU hSubMenu);
	static int FindMenuItem(HMENU hMenu, HMENU hSubMenu, HMENU& hParentMenu);
	static int FindFirstMenuItem(HMENU hMenu, UINT nCmdIDStart, UINT nCmdIDEnd);
	static int FindFirstMenuItem(HMENU hMenu, UINT nCmdIDStart, UINT nCmdIDEnd, HMENU& hParentMenu);

	static HMENU GetSubMenu(HMENU hMenu, UINT nCmdID);
	static HMENU GetParentMenu(HMENU hMenu, HMENU hSubMenu);

	static BOOL CopyMenuContents(HMENU hMenuFrom, HMENU hMenuTo);

	static BOOL IsSeparator(HMENU hMenu, int nPos);
	static BOOL IsPopop(HMENU hMenu, int nPos);

	static BOOL DeleteSubMenu(HMENU hMenu, HMENU hSubMenu, BOOL bAutoCleanUp);
	static BOOL DeleteMenu(HMENU hMenu, UINT nPosition, UINT nFlags, BOOL bAutoCleanUp);
	static BOOL DeleteMenuContents(HMENU hMenu);
	static void RemoveDuplicateSeparators(HMENU hMenu, int nStartPos = 0);

	static CString GetMenuString(HMENU hMenu, UINT nIDItem,	UINT nFlags);
	static BOOL SetMenuString(HMENU hMenu, UINT nIDItem, const CString& sItem, UINT nFlags);
	static DWORD GetMenuItemData(HMENU hMenu, UINT nIDItem, UINT nFlags);
	static BOOL SetMenuItemData(HMENU hMenu, UINT nIDItem, DWORD dwItemData, UINT nFlags);
	static int GetMenuStrings(HMENU hMenu, CStringArray& aItems);
	static BOOL SetMenuStrings(HMENU hMenu, const CStringArray& aItems);
	static BOOL SortMenuStrings(HMENU hMenu, UINT nCmdIDStart, UINT nCmdIDEnd);
	static int GetMenuItemInfo(HMENU hMenu, CArray<MENUITEMINFO, MENUITEMINFO&>& aMenuItems);
	static BOOL GetMenuItemInfo(HMENU hMenu, CArray<MENUITEMINFO, MENUITEMINFO&>& aMenuItems, CStringArray& aItemText);

	static BOOL RebaseMenuID(UINT nFromCmdIDStart, UINT nFromCmdIDEnd, UINT nToCmdIDStart, UINT& nCmdID);

protected:
	static ITransText* s_pTT;

protected:
	CMap<UINT, UINT, int, int> m_mapCmd2ID;
	CBrush m_brBkgnd;

protected:
	static BOOL IsThemed();
	static int FindMenuItem(HMENU hMenu, DWORD dwItem, HMENU& hParentMenu, BOOL bItemIsMenu);
	static void DoCleanUp(HMENU hMenu, HMENU hCmdMenu, int nCmdPos);
	static int MenuSortProc(const void* v1, const void* v2);

};

#endif // !defined(AFX_ENMENU_H__5AB11CC8_CCF5_4D52_ADC7_27FDC151F3FE__INCLUDED_)
