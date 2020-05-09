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

	// pseudo-message handlers
	BOOL HandleInitMenuPopup(CMenu* pPopupMenu, 
							 const CFilteredToDoCtrl& tdc, 
							 const CPreferencesDlg& prefs,
							 const CTDLFilterBar& filterBar,
							 const CTDLTasklistStorageMgr& mgrStorage,
							 const CUIExtensionMgr& mgrUIExt,
							 CMenuIconMgr& mgrMenuIcons); // not const

	BOOL HandleDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	BOOL HandleMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	BOOL HandlePostTranslateMenu(HMENU hMenu);

	void PrepareTaskContextMenu(CMenu* pMenu, 
								const CFilteredToDoCtrl& tdc, 
								const CPreferencesDlg& prefs) const;

	// Helpers
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

	void PrepareFileMenu(CMenu* pMenu, const CPreferencesDlg& prefs);
	void PrepareEditMenu(CMenu* pMenu, const CFilteredToDoCtrl& tdc, const CPreferencesDlg& prefs) const;
	void PrepareSortMenu(CMenu* pMenu, const CFilteredToDoCtrl& tdc, const CPreferencesDlg& prefs) const;
	void PrepareToolsMenu(CMenu* pMenu, const CPreferencesDlg& prefs, CMenuIconMgr& mgrMenuIcons);

	void AddFiltersToMenu(CMenu* pMenu, const CTDLFilterBar& filterBar);
	void AddFiltersToMenu(CMenu* pMenu, UINT nStart, UINT nEnd, const CStringArray& aFilters, UINT nPlaceholderStrID);
	void AddUserStorageToMenu(CMenu* pMenu, const CTDLTasklistStorageMgr& mgrStorage, CMenuIconMgr& mgrMenuIcons);
	void AddTaskViewVisibilityToMenu(CMenu* pMenu, const CFilteredToDoCtrl& tdc, const CUIExtensionMgr& mgrUIExt);
	void AddTaskViewActivationToMenu(CMenu* pMenu, const CFilteredToDoCtrl& tdc, const CUIExtensionMgr& mgrUIExt);

	static BOOL IsInRange(UINT nItem, UINT nStart, UINT nEnd);
};

#endif // !defined(AFX_TDCMAINMENU_H__5AB11CC8_CCF5_4D52_ADC7_27FDC151F3FE__INCLUDED_)
