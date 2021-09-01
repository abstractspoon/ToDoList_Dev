// MenuEx.h: interface for the CTDCMainMenu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCMAINMENU_H__5AB11CC8_CCF5_4D52_ADC7_27FDC151F3FE__INCLUDED_)
#define AFX_TDCMAINMENU_H__5AB11CC8_CCF5_4D52_ADC7_27FDC151F3FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "..\Shared\EnMenu.h"
#include "..\Interfaces\UITheme.h"

//////////////////////////////////////////////////////////////////////

class CRecentFileList;

class CToDoCtrlMgr;
class CFilteredToDoCtrl;
class CPreferencesDlg;
class CTDLFilterBar;
class CTDLTasklistStorageMgr;
class CUIExtensionMgr;
class CMenuIconMgr;

//////////////////////////////////////////////////////////////////////

class CTDCMainMenu : public CEnMenu  
{
public:
	CTDCMainMenu();
	virtual ~CTDCMainMenu();

	BOOL LoadMenu();
	BOOL LoadMenu(const CPreferencesDlg& prefs);
	void UpdateBackgroundColor();
	void SetUITheme(const UITHEME& theme);

	BOOL HandleInitMenuPopup(CMenu* pPopupMenu, 
							 const CFilteredToDoCtrl& tdc, 
							 const CPreferencesDlg& prefs,
							 const CTDLFilterBar& filterBar,
							 const CTDLTasklistStorageMgr& mgrStorage,
							 const CUIExtensionMgr& mgrUIExt,
							 CMenuIconMgr& mgrMenuIcons) const;

	BOOL HandleDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) const;
	BOOL HandleMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) const;
	BOOL HandlePostTranslateMenu(HMENU hMenu) const;

	void PrepareTaskContextMenu(CMenu* pMenu, 
								const CFilteredToDoCtrl& tdc, 
								const CPreferencesDlg& prefs) const;

	void PrepareTabCtrlContextMenu(CMenu* pMenu, 
									const CFilteredToDoCtrl& tdc, 
									const CPreferencesDlg& prefs) const;
	
	CString GetDynamicItemTooltip(UINT nMenuID,
								  const CRecentFileList& mru,
								  const CToDoCtrlMgr& mgrToDoCtrl,
								  const CPreferencesDlg& prefs,
								  const CTDLFilterBar& filterBar,
								  const CTDLTasklistStorageMgr& mgrStorage,
								  const CUIExtensionMgr& mgrUIExt) const;

	static BOOL IsDynamicItem(UINT nMenuID);

protected:
	UITHEME m_theme;

protected:
	void LoadMenuCommon();

	static void PrepareFileMenu(CMenu* pMenu, const CPreferencesDlg& prefs);
	static void PrepareEditMenu(CMenu* pMenu, const CFilteredToDoCtrl& tdc, const CPreferencesDlg& prefs);
	static void PrepareSortMenu(CMenu* pMenu, const CFilteredToDoCtrl& tdc, const CPreferencesDlg& prefs);
	static void PrepareToolsMenu(CMenu* pMenu, const CPreferencesDlg& prefs, CMenuIconMgr& mgrMenuIcons);

	static void PrepareFiltersActivationMenu(CMenu* pMenu, const CTDLFilterBar& filterBar);
	static void PrepareUserStorageMenu(CMenu* pMenu, const CTDLTasklistStorageMgr& mgrStorage, CMenuIconMgr& mgrMenuIcons);
	static void PrepareTaskViewVisibilityMenu(CMenu* pMenu, const CFilteredToDoCtrl& tdc, const CUIExtensionMgr& mgrUIExt);
	static void PrepareTaskViewActivationMenu(CMenu* pMenu, const CFilteredToDoCtrl& tdc, const CUIExtensionMgr& mgrUIExt);

	static void AddFiltersToMenu(CMenu* pMenu, UINT nStart, UINT nEnd, const CStringArray& aFilters, UINT nPlaceholderStrID);
	static BOOL IsInRange(UINT nItem, UINT nStart, UINT nEnd);
};

#endif // !defined(AFX_TDCMAINMENU_H__5AB11CC8_CCF5_4D52_ADC7_27FDC151F3FE__INCLUDED_)
