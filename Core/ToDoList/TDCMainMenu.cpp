// MenuEx.cpp: implementation of the CTDCMainMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "TDCMainMenu.h"
#include "FilteredToDoCtrl.h"
#include "PreferencesDlg.h"
#include "TDLFilterBar.h"
#include "TDLTasklistStorageMgr.h"
#include "TDCToolsHelper.h"

#include "..\Shared\Themed.h"
#include "..\Shared\GraphicsMisc.h"
#include "..\Shared\OSVersion.h"
#include "..\Shared\TabCtrlEx.h"
#include "..\Shared\MSOutlookHelper.h"
#include "..\Shared\Localizer.h"
#include "..\shared\fileicons.h"

#include "..\Interfaces\UIExtensionHelper.h"
#include "..\Interfaces\UIExtensionMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCMainMenu::CTDCMainMenu()
{

}

CTDCMainMenu::~CTDCMainMenu()
{
}

void CTDCMainMenu::SetUITheme(const UITHEME& theme)
{
	m_theme = theme;

	if (GetSafeHmenu())
		UpdateBackgroundColor();
}

BOOL CTDCMainMenu::LoadMenu(const CPreferencesDlg& prefs)
{
	CWnd* pMainWnd = AfxGetMainWnd();

	if (pMainWnd == NULL)
	{
		ASSERT(0);
		return FALSE;
	}

	if (!CEnMenu::LoadMenu(IDR_MAINFRAME, *pMainWnd, TRUE, FALSE))
		return FALSE;

	LoadMenuCommon();

	if (!prefs.GetShowTasklistTabCloseButton())
		AddMDIButton(MEB_CLOSE, ID_CLOSE);

#ifdef _DEBUG
	ModifyMenu(AM_DEBUG, MF_BYPOSITION | MFT_RIGHTJUSTIFY, 0, _T("&Debug"));

	// don't translate the debug menu
	CLocalizer::EnableTranslation(::GetSubMenu(GetSafeHmenu(), AM_DEBUG), FALSE);
#endif

	UpdateBackgroundColor();
	
	return TRUE;
}

BOOL CTDCMainMenu::LoadMenu()
{
	if (!CEnMenu::LoadMenu(IDR_MAINFRAME, NULL, TRUE, TRUE))
		return FALSE;

	LoadMenuCommon();

	TranslateDynamicMenuItems(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE16, _T("Recent Tasklist %d"));
	TranslateDynamicMenuItems(ID_WINDOW1, ID_WINDOW16, _T("Window %d"));
	TranslateDynamicMenuItems(ID_TOOLS_USERTOOL1, ID_TOOLS_USERTOOL50, _T("User Defined Tool %d"));
	TranslateDynamicMenuItems(ID_FILE_OPEN_USERSTORAGE1, ID_FILE_OPEN_USERSTORAGE16, _T("3rd Party Storage %d"));
	TranslateDynamicMenuItems(ID_FILE_SAVE_USERSTORAGE1, ID_FILE_SAVE_USERSTORAGE16, _T("3rd Party Storage %d"));
	TranslateDynamicMenuItems(ID_SHOWVIEW_UIEXTENSION1, ID_SHOWVIEW_UIEXTENSION16, _T("Task View %d"));
	TranslateDynamicMenuItems(ID_ACTIVATEVIEW_UIEXTENSION1, ID_ACTIVATEVIEW_UIEXTENSION16, _T("Task View %d"));

	return TRUE;
}

void CTDCMainMenu::LoadMenuCommon()
{
	ASSERT(GetSafeHmenu());

#ifndef _DEBUG
	DeleteMenu(AM_DEBUG, MF_BYPOSITION);
#endif

	// delete 'Record Bug Report' if below W7
	if (COSVersion() < OSV_WIN7)
	{
		CMenu* pSubMenu = GetSubMenu(AM_HELP);
		ASSERT(pSubMenu);

		int nPos = CEnMenu::GetMenuItemPos(*pSubMenu, ID_HELP_RECORDBUGREPORT);
		ASSERT(nPos != -1);

		pSubMenu->DeleteMenu(nPos, MF_BYPOSITION);

		// and the following separator
		if (pSubMenu->GetMenuItemID(nPos) == 0)
			pSubMenu->DeleteMenu(nPos, MF_BYPOSITION);
	}
}

void CTDCMainMenu::UpdateBackgroundColor()
{
	// set the menu background colour
	// Note: On XP and classic, the menu bar is not distinctive
	// so we set the menu color a little darker than 3DFACE
	if (!CThemed::IsAppThemed())
	{
		SetBackgroundColor(GraphicsMisc::Darker(GetSysColor(COLOR_3DFACE), 0.1));
	}
	else if (COSVersion() < OSV_VISTA)
	{
		if (m_theme.crMenuBack == m_theme.crAppBackLight)
		{
			if (m_theme.crAppBackDark != m_theme.crAppBackLight)
			{
				SetBackgroundColor(m_theme.crAppBackDark);
			}
			else
			{
				SetBackgroundColor(GraphicsMisc::Darker(m_theme.crAppBackLight, 0.1));
			}
		}
		else
		{
			SetBackgroundColor(m_theme.crMenuBack);
		}

	}
	else // Vista+ with themes
	{
		SetBackgroundColor(m_theme.crMenuBack);
	}

	AfxGetMainWnd()->DrawMenuBar();
}

// test for top-level menus
BOOL CTDCMainMenu::HandleInitMenuPopup(CMenu* pPopupMenu,  
									   const CFilteredToDoCtrl& tdc, 
									   const CPreferencesDlg& prefs,
									   const CTDLFilterBar& filterBar, 
									   const CTDLTasklistStorageMgr& mgrStorage,
									   const CUIExtensionMgr& mgrUIExt,
									   CMenuIconMgr& mgrMenuIcons)
{
	if (GetSubMenu(AM_FILE) == pPopupMenu)
	{
		return TRUE;
	}
	else if (GetSubMenu(AM_EDIT) == pPopupMenu)
	{
		PrepareEditMenu(pPopupMenu, tdc, prefs);
		return TRUE;
	}
	else if (GetSubMenu(AM_SORT) == pPopupMenu)
	{
		PrepareSortMenu(pPopupMenu, tdc, prefs);
		return TRUE;
	}
	else if (GetSubMenu(AM_TOOLS) == pPopupMenu)
	{
		PrepareToolsMenu(pPopupMenu, prefs, mgrMenuIcons);
		return TRUE;
	}
	else // all other sub-menus
	{
		switch (pPopupMenu->GetMenuItemID(0))
		{
			// test for 'Open From.../Save To...'
		case ID_FILE_OPEN_USERSTORAGE1:
		case ID_FILE_SAVE_USERSTORAGE1:
			AddUserStorageToMenu(pPopupMenu, mgrStorage, mgrMenuIcons);
			return TRUE;

		case ID_VIEW_ACTIVATEFILTER1:
			AddFiltersToMenu(pPopupMenu, filterBar);
			return TRUE;

		case ID_ACTIVATEVIEW_TASKTREE:
			AddTaskViewActivationToMenu(pPopupMenu, tdc, mgrUIExt);
			return TRUE;

		case ID_SHOWVIEW_TASKTREE:
			AddTaskViewVisibilityToMenu(pPopupMenu, tdc, mgrUIExt);
			return TRUE;
		}
	}

	return FALSE;
}

void CTDCMainMenu::PrepareFileMenu(CMenu* pMenu, const CPreferencesDlg& prefs)
{
	ASSERT(pMenu);

	if (!pMenu)
		return;

	// insert Min to sys tray if appropriate 
	BOOL bHasMinToTray = (::GetMenuString(*pMenu, ID_MINIMIZETOTRAY, NULL, 0, MF_BYCOMMAND) != 0);

	if (prefs.HasSysTrayOptions(STO_ONCLOSE, STO_ONMINCLOSE))
	{
		if (!bHasMinToTray)
			pMenu->InsertMenu(ID_EXIT, MF_BYCOMMAND, ID_MINIMIZETOTRAY, CEnString(ID_MINIMIZETOTRAY));
	}
	else if (bHasMinToTray) // then remove
	{
		pMenu->DeleteMenu(ID_MINIMIZETOTRAY, MF_BYCOMMAND);
	}

	// Remove 'Email Tasks' if Outlook is not installed
	if (!CMSOutlookHelper::IsOutlookInstalled())
	{
		int nPos = CEnMenu::GetMenuItemPos(*pMenu, ID_SENDTASKS);

		if (nPos != -1)
		{
			// Delete menu item
			pMenu->DeleteMenu(nPos, MF_BYPOSITION);

			// And then delete separator
			pMenu->DeleteMenu(nPos, MF_BYPOSITION);
		}
	}
}

void CTDCMainMenu::AddTaskViewVisibilityToMenu(CMenu* pMenu, const CFilteredToDoCtrl& tdc, const CUIExtensionMgr& mgrUIExt)
{
	CUIExtensionHelper helper(ID_SHOWVIEW_UIEXTENSION1, 16);
	helper.AddAllExtensionsToMenu(pMenu, mgrUIExt);
	
	// ListView visibility
	pMenu->CheckMenuItem(ID_SHOWVIEW_LISTVIEW, (tdc.IsListViewTabShowing() ? MF_CHECKED : 0));

	// UI Extension visibility
	CStringArray aTypeIDs;
	tdc.GetVisibleExtensionViews(aTypeIDs);

	helper.UpdateExtensionVisibilityState(pMenu, mgrUIExt, aTypeIDs);
}

void CTDCMainMenu::AddTaskViewActivationToMenu(CMenu* pMenu, const CFilteredToDoCtrl& tdc, const CUIExtensionMgr& mgrUIExt)
{
	CStringArray aTypeIDs;
	tdc.GetVisibleExtensionViews(aTypeIDs);

	CUIExtensionHelper helper(ID_ACTIVATEVIEW_UIEXTENSION1, 16);
	helper.AddExtensionsToMenu(pMenu, mgrUIExt, aTypeIDs);

	// Active view
	FTC_VIEW nView = tdc.GetTaskView();
	int nMenuID = 0;

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		nMenuID = ID_ACTIVATEVIEW_TASKTREE;
		break;

	case FTCV_TASKLIST:
		nMenuID = ID_ACTIVATEVIEW_LISTVIEW;
		break;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		nMenuID = (ID_ACTIVATEVIEW_UIEXTENSION1 + (nView - FTCV_UIEXTENSION1));
		break;

		// all else
	default:
		ASSERT(0);
		return;
	}

	pMenu->CheckMenuRadioItem(0, ID_ACTIVATEVIEW_UIEXTENSION16, nMenuID, MF_BYCOMMAND);
}

BOOL CTDCMainMenu::HandlePostTranslateMenu(HMENU hMenu)
{
	ASSERT(::IsMenu(hMenu));

	CMenu* pSortMenu = GetSubMenu(AM_SORT);

	if (pSortMenu && (hMenu == pSortMenu->GetSafeHmenu()))
	{
		CEnMenu::SortMenuStrings(hMenu, ID_SORTBY_ALLCOLUMNS_FIRST, ID_SORTBY_ALLCOLUMNS_LAST);
		return TRUE;
	}

	return FALSE;
}

BOOL CTDCMainMenu::HandleDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if ((nIDCtl == 0) && (lpDrawItemStruct->itemID == ID_CLOSE))
	{
		VERIFY(DrawMDIButton(lpDrawItemStruct));
		return TRUE;
	}

	return FALSE;
}

BOOL CTDCMainMenu::HandleMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (nIDCtl == 0 && lpMeasureItemStruct->itemID == ID_CLOSE)
	{
		VERIFY(MeasureMDIButton(lpMeasureItemStruct));
		return TRUE;
	}

	return FALSE;
}

void CTDCMainMenu::PrepareEditMenu(CMenu* pMenu, const CFilteredToDoCtrl& tdc, const CPreferencesDlg& prefs) const
{
	ASSERT(pMenu);

	if (!pMenu)
		return;

	if (!prefs.GetShowEditMenuAsColumns())
		return;

	int nCountLastSep = 0;

	for (int nItem = 0; nItem < (int)pMenu->GetMenuItemCount(); nItem++)
	{
		BOOL bDelete = FALSE;
		BOOL bIsSeparator = FALSE;

		UINT nMenuID = pMenu->GetMenuItemID(nItem);

		switch (nMenuID)
		{
		case -1: // its a popup so recursively handle it
			{
				CMenu* pPopup = pMenu->GetSubMenu(nItem);

				if (pPopup)
				{
					PrepareEditMenu(pPopup, tdc, prefs);

					// if the popup is now empty remove it too
					bDelete = !pPopup->GetMenuItemCount();
				}
			}
			break;

		case ID_EDIT_SETTASKLISTCOLOR:
		case ID_EDIT_CLEARTASKLISTCOLOR:
			bDelete = !CTabCtrlEx::IsSupportedFlag(TCE_TABCOLORS);
			break;

		case ID_EDIT_TASKCOLOR:
		case ID_EDIT_CLEARTASKCOLOR:
			bDelete = !((prefs.GetTextColorOption() == COLOROPT_DEFAULT) ||
						tdc.IsEditFieldShowing(TDCA_COLOR));
			break;

		case ID_EDIT_DECTASKPRIORITY:
		case ID_EDIT_INCTASKPRIORITY:
		case ID_EDIT_SETPRIORITYNONE:
		case ID_EDIT_SETPRIORITY0:
		case ID_EDIT_SETPRIORITY1:
		case ID_EDIT_SETPRIORITY2:
		case ID_EDIT_SETPRIORITY3:
		case ID_EDIT_SETPRIORITY4:
		case ID_EDIT_SETPRIORITY5:
		case ID_EDIT_SETPRIORITY6:
		case ID_EDIT_SETPRIORITY7:
		case ID_EDIT_SETPRIORITY8:
		case ID_EDIT_SETPRIORITY9:
		case ID_EDIT_SETPRIORITY10:
			bDelete = !tdc.IsColumnOrEditFieldShowing(TDCC_PRIORITY, TDCA_PRIORITY);
			break;

		case ID_EDIT_OFFSETDATES:
			bDelete = !(tdc.IsColumnOrEditFieldShowing(TDCC_STARTDATE, TDCA_STARTDATE) ||
						tdc.IsColumnOrEditFieldShowing(TDCC_DUEDATE, TDCA_DUEDATE) ||
						tdc.IsColumnOrEditFieldShowing(TDCC_DONEDATE, TDCA_DONEDATE));
			break;

		case ID_EDIT_CLOCK_TASK:
			bDelete = !(tdc.IsColumnShowing(TDCC_TRACKTIME) ||
						tdc.IsColumnOrEditFieldShowing(TDCC_TIMESPENT, TDCA_TIMESPENT));
			break;

		case ID_SHOWTIMELOGFILE:
		case ID_ADDTIMETOLOGFILE:
			bDelete = !((tdc.IsColumnShowing(TDCC_TRACKTIME) ||
						 tdc.IsColumnOrEditFieldShowing(TDCC_TIMESPENT, TDCA_TIMESPENT)) &&
						prefs.GetLogTimeTracking());
			break;

		case ID_EDIT_DECTASKPERCENTDONE:
		case ID_EDIT_INCTASKPERCENTDONE:
			bDelete = !tdc.IsColumnOrEditFieldShowing(TDCC_PERCENT, TDCA_PERCENT);
			break;

		case ID_EDIT_OPENFILEREF1:
		case ID_EDIT_SETFILEREF:
			bDelete = !tdc.IsColumnOrEditFieldShowing(TDCC_FILEREF, TDCA_FILEREF);
			break;

		case ID_EDIT_FLAGTASK:
			bDelete = !tdc.IsColumnShowing(TDCC_FLAG);
			break;

		case ID_EDIT_LOCKTASK:
			bDelete = FALSE;
			break;

		case ID_EDIT_RECURRENCE:
			bDelete = !tdc.IsColumnOrEditFieldShowing(TDCC_RECURRENCE, TDCA_RECURRENCE);
			break;

		case ID_EDIT_GOTODEPENDENCY:
			bDelete = !tdc.IsColumnOrEditFieldShowing(TDCC_DEPENDENCY, TDCA_DEPENDENCY);
			break;

		case ID_EDIT_SETTASKICON:
		case ID_EDIT_CLEARTASKICON:
			break;

		case ID_SEPARATOR:
			bIsSeparator = TRUE;
			bDelete = (nCountLastSep == 0);
			nCountLastSep = 0;
			break;

		default:
			break;
		}

		// delete the item else increment the count since the last separator
		if (bDelete)
		{
			pMenu->DeleteMenu(nItem, MF_BYPOSITION);
			nItem--;
		}
		else if (!bIsSeparator)
			nCountLastSep++;
	}

	// make sure last item is not a separator
	int nLastItem = (int)pMenu->GetMenuItemCount() - 1;

	if (pMenu->GetMenuItemID(nLastItem) == 0)
		pMenu->DeleteMenu(nLastItem, MF_BYPOSITION);
}

void CTDCMainMenu::PrepareSortMenu(CMenu* pMenu, const CFilteredToDoCtrl& tdc, const CPreferencesDlg& prefs) const
{
	ASSERT(pMenu);

	if (!pMenu)
		return;

	CTDCColumnIDMap mapColIDs;
	tdc.GetSortableColumns(mapColIDs);

	BOOL bVisibleColumnsOnly = prefs.GetShowSortMenuAsColumns();

	if (bVisibleColumnsOnly)
	{
		// Always rebuild from scratch
		{
			CTDCMainMenu menuBar;

			VERIFY(menuBar.LoadMenu());
			VERIFY(CEnMenu::CopyMenuContents(::GetSubMenu(menuBar, AM_SORT), *pMenu));
		}

		int nCountLastSep = 0;

		for (int nItem = 0; nItem < (int)pMenu->GetMenuItemCount(); nItem++)
		{
			BOOL bDelete = FALSE;
			BOOL bIsSeparator = FALSE;

			UINT nMenuID = pMenu->GetMenuItemID(nItem);

			if (nMenuID == ID_SEPARATOR)
			{
				bIsSeparator = TRUE;
				bDelete = (nCountLastSep == 0);
				nCountLastSep = 0;
			}
			else
			{
				TDC_COLUMN nColID = TDC::MapSortIDToColumn(nMenuID);

				if (nColID != TDCC_NONE)
					bDelete = !mapColIDs.Has(nColID);
			}

			// delete the item else increment the count since the last separator
			if (bDelete)
			{
				ASSERT((nMenuID == ID_SEPARATOR) || (TDC::MapSortIDToColumn(nMenuID) != TDCC_NONE));

				pMenu->DeleteMenu(nItem, MF_BYPOSITION);
				nItem--;
			}
			else if (!bIsSeparator)
			{
				nCountLastSep++;
			}
		}
	}

	// custom attributes

	// first delete all custom columns
	int nItem = (int)pMenu->GetMenuItemCount();

	while (nItem--)
	{
		UINT nMenuID = pMenu->GetMenuItemID(nItem);

		if (nMenuID >= ID_SORTBY_CUSTOMCOLUMN_FIRST && nMenuID <= ID_SORTBY_CUSTOMCOLUMN_LAST)
			pMenu->DeleteMenu(nItem, MF_BYPOSITION);
	}

	// then re-add just before the separator before 'unsorted entry'
	CTDCCustomAttribDefinitionArray aAttribDefs;

	if (tdc.GetCustomAttributeDefs(aAttribDefs))
	{
		int nInsert = CEnMenu::GetMenuItemPos(*pMenu, ID_SORTBY_NONE) - 1;
		ASSERT(nInsert >= 0);

		for (int nCol = 0; nCol < aAttribDefs.GetSize(); nCol++)
		{
			const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = aAttribDefs[nCol];
			TDC_COLUMN nColID = attribDef.GetColumnID();

			if (bVisibleColumnsOnly && !mapColIDs.Has(nColID))
				continue;

			ASSERT(attribDef.bEnabled && attribDef.SupportsFeature(TDCCAF_SORT));

			UINT nMenuID = ((nColID - TDCC_CUSTOMCOLUMN_FIRST) + ID_SORTBY_CUSTOMCOLUMN_FIRST);
			CEnString sColumn(IDS_CUSTOMCOLUMN, attribDef.sLabel);

			pMenu->InsertMenu(nInsert, MF_BYPOSITION, nMenuID, sColumn);
			nInsert++;
		}

		// Resort the menu if we're not translated else 
		// it'll get done in HandlePostTranslateMenu()
		if (!CLocalizer::IsInitialized())
			CEnMenu::SortMenuStrings(*pMenu, ID_SORTBY_DEFAULTCOLUMNS_FIRST, ID_SORTBY_CUSTOMCOLUMN_LAST);
	}
}

void CTDCMainMenu::AddFiltersToMenu(CMenu* pMenu, const CTDLFilterBar& filterBar)
{
	ASSERT(pMenu);

	if (!pMenu)
		return;

	const UINT MENUSTARTID = pMenu->GetMenuItemID(0);

	// delete existing entries
	int nFilter = 24;

	while (nFilter--)
		pMenu->DeleteMenu(nFilter, MF_BYPOSITION);

	CStringArray aFilters;
	int nNumFilters = min(filterBar.GetAllFilterNames(aFilters), 24);

	if (nNumFilters)
	{
		UINT nFlags = (MF_BYPOSITION | MF_STRING);
		CString sMenuItem;

		for (int nFilter = 0; nFilter < nNumFilters; nFilter++)
		{
			sMenuItem.Format(_T("&%s"), aFilters[nFilter]);
			sMenuItem.Replace('\t', ' ');

			pMenu->InsertMenu(nFilter, nFlags, (ID_VIEW_ACTIVATEFILTER1 + nFilter), sMenuItem);
		}

		int nSelFilter = filterBar.GetSelectedFilter();
		pMenu->CheckMenuRadioItem(0, nNumFilters, nSelFilter, MF_BYPOSITION);

	}
	else // if nothing to add just re-add placeholder
	{
		pMenu->InsertMenu(0, MF_BYPOSITION | MF_STRING | MF_GRAYED, MENUSTARTID, CEnString(IDS_FILTERPLACEHOLDER));
	}
}

void CTDCMainMenu::AddUserStorageToMenu(CMenu* pMenu, const CTDLTasklistStorageMgr& mgrStorage, CMenuIconMgr& mgrMenuIcons)
{
	ASSERT(pMenu);

	if (!pMenu)
		return;

	const UINT MENUSTARTID = pMenu->GetMenuItemID(0);

	// delete existing entries
	int nStore = 16;

	while (nStore--)
		pMenu->DeleteMenu(nStore, MF_BYPOSITION);

	// if we have any tools to add we do it here
	int nNumStorage = min(mgrStorage.GetNumStorage(), 16);

	if (nNumStorage)
	{
		UINT nFlags = (MF_BYPOSITION | MF_STRING);

		for (int nStore = 0; nStore < nNumStorage; nStore++)
		{
			CString sMenuItem, sText = mgrStorage.GetStorageMenuText(nStore);

			if (nStore < 9)
				sMenuItem.Format(_T("&%d %s"), nStore + 1, sText);
			else
				sMenuItem = sText;

			pMenu->InsertMenu(nStore, nFlags, MENUSTARTID + nStore, sMenuItem);

			// add icon if available
			HICON hIcon = mgrStorage.GetStorageIcon(nStore);

			if (hIcon)
				mgrMenuIcons.AddImage(MENUSTARTID + nStore, hIcon);
		}
	}
	else // if nothing to add just re-add placeholder
	{
		pMenu->InsertMenu(0, MF_BYPOSITION | MF_STRING | MF_GRAYED, MENUSTARTID, CEnString(IDS_3RDPARTYSTORAGE));
	}
}

void CTDCMainMenu::PrepareToolsMenu(CMenu* pMenu, const CPreferencesDlg& prefs, CMenuIconMgr& mgrMenuIcons)
{
	ASSERT(pMenu);

	if (!pMenu)
		return;

	const UINT MENUSTARTID = ID_TOOLS_USERTOOL1;
	const int MENUSTARTPOS = CEnMenu::GetMenuItemPos(*pMenu, ID_TOOLS_USERTOOL1);

	// delete existing tool entries and their icons first
	int nTool = MAX_NUM_TOOLS;

	while (nTool--)
	{
		DeleteMenu(MENUSTARTID + nTool, MF_BYCOMMAND);
		mgrMenuIcons.DeleteImage(MENUSTARTID + nTool);
	}

	// if we have any tools to add we do it here
	CUserToolArray aTools;
	prefs.GetUserTools(aTools);

	// Remove invalid tools
	nTool = aTools.GetSize();

	while (nTool--)
	{
		if (aTools[nTool].sToolName.IsEmpty() || aTools[nTool].sToolPath.IsEmpty())
			aTools.RemoveAt(nTool);
	}

	if (aTools.GetSize())
	{
		int nNumTools = min(aTools.GetSize(), MAX_NUM_TOOLS);

		for (nTool = 0; nTool < nNumTools; nTool++)
		{
			const USERTOOL& tool = aTools[nTool];
			CString sMenuItem;

			if (nTool < 9)
				sMenuItem.Format(_T("&%d %s"), nTool + 1, tool.sToolName);
			else
				sMenuItem = tool.sToolName;

			pMenu->InsertMenu(MENUSTARTPOS + nTool, MF_BYPOSITION | MF_STRING,
							  MENUSTARTID + nTool, sMenuItem);

			// Icon manager will free the icon
			mgrMenuIcons.SetImage(MENUSTARTID + nTool, CTDCToolsHelper::GetToolIcon(tool));
		}
	}
	else // if nothing to add just re-add placeholder
	{
		pMenu->InsertMenu(MENUSTARTPOS, MF_BYPOSITION | MF_STRING | MF_GRAYED,
						  MENUSTARTID, CEnString(IDS_USERDEFINEDTOOLS));
	}
}
