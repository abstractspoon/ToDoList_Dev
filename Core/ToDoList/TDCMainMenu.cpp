// MenuEx.cpp: implementation of the CTDCMainMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "TDCMainMenu.h"
#include "TDCStatic.h"
#include "FilteredToDoCtrl.h"
#include "ToDoCtrlMgr.h"
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

#include <afxadv.h> // for CRecentFileList

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

	if (!prefs.GetShowTabCloseButtons())
		AddMDIButton(MEB_CLOSE, ID_CLOSE);

#ifdef _DEBUG
	ModifyMenu(AM_DEBUG, MF_BYPOSITION | MFT_RIGHTJUSTIFY, 0, _T("&Debug"));

	// don't translate the debug menu
	CLocalizer::EnableTranslation(::GetSubMenu(GetSafeHmenu(), AM_DEBUG), FALSE);

	// 'Run from Explorer' not supported below Vista
	if (COSVersion() < OSV_VISTA)
	{
		CMenu* pSubMenu = GetSubMenu(AM_DEBUG);
		ASSERT(pSubMenu);

		pSubMenu->DeleteMenu(ID_DEBUG_RESTARTAPPFROMEXPLORER, MF_BYCOMMAND);
	}
#endif

	UpdateBackgroundColor();
	
	return TRUE;
}

BOOL CTDCMainMenu::LoadMenu()
{
	if (!CEnMenu::LoadMenu(IDR_MAINFRAME, NULL, TRUE, TRUE))
		return FALSE;

	LoadMenuCommon();

	TranslateDynamicMenuItems(ID_FILE_MRU1, ID_FILE_MRU16, _T("Recent Tasklist %d"));
	TranslateDynamicMenuItems(ID_WINDOW1, ID_WINDOW16, _T("Window %d"));
	TranslateDynamicMenuItems(ID_TOOLS_USERTOOL1, ID_TOOLS_USERTOOL50, _T("User Defined Tool %d"));
	TranslateDynamicMenuItems(ID_FILE_OPEN_USERSTORAGE1, ID_FILE_OPEN_USERSTORAGE16, _T("3rd Party Storage %d"));
	TranslateDynamicMenuItems(ID_FILE_SAVE_USERSTORAGE1, ID_FILE_SAVE_USERSTORAGE16, _T("3rd Party Storage %d"));
	TranslateDynamicMenuItems(ID_SHOWVIEW_UIEXTENSION1, ID_SHOWVIEW_UIEXTENSION16, _T("Task View %d"));
	TranslateDynamicMenuItems(ID_ACTIVATEVIEW_UIEXTENSION1, ID_ACTIVATEVIEW_UIEXTENSION16, _T("Task View %d"));
	TranslateDynamicMenuItems(ID_VIEW_ACTIVATEFILTER1, ID_VIEW_ACTIVATEFILTER24, _T("Default Filter %d"));
	TranslateDynamicMenuItems(ID_VIEW_ACTIVATEADVANCEDFILTER1, ID_VIEW_ACTIVATEADVANCEDFILTER24, _T("Find Tasks Filter %d"));

	return TRUE;
}

BOOL CTDCMainMenu::IsInRange(UINT nItem, UINT nStart, UINT nEnd)
{
	return ((nItem >= nStart) && (nItem <= nEnd));
}

BOOL CTDCMainMenu::IsDynamicItem(UINT nMenuID)
{
#define CHECKINRANGE(id, start, end) if (IsInRange(id, start, end)) return TRUE

	// Same as above
	CHECKINRANGE(nMenuID, ID_FILE_MRU1, ID_FILE_MRU16);
	CHECKINRANGE(nMenuID, ID_WINDOW1, ID_WINDOW16);
	CHECKINRANGE(nMenuID, ID_TOOLS_USERTOOL1, ID_TOOLS_USERTOOL50);
	CHECKINRANGE(nMenuID, ID_FILE_OPEN_USERSTORAGE1, ID_FILE_OPEN_USERSTORAGE16);
	CHECKINRANGE(nMenuID, ID_FILE_SAVE_USERSTORAGE1, ID_FILE_SAVE_USERSTORAGE16);
	CHECKINRANGE(nMenuID, ID_SHOWVIEW_UIEXTENSION1, ID_SHOWVIEW_UIEXTENSION16);
	CHECKINRANGE(nMenuID, ID_ACTIVATEVIEW_UIEXTENSION1, ID_ACTIVATEVIEW_UIEXTENSION16);
	CHECKINRANGE(nMenuID, ID_VIEW_ACTIVATEFILTER1, ID_VIEW_ACTIVATEFILTER24);
	CHECKINRANGE(nMenuID, ID_VIEW_ACTIVATEADVANCEDFILTER1, ID_VIEW_ACTIVATEADVANCEDFILTER24);

	// else
	return FALSE;
}

CString CTDCMainMenu::GetDynamicItemTooltip(UINT nMenuID,
											const CRecentFileList& mru,
											const CToDoCtrlMgr& mgrToDoCtrl,
											const CPreferencesDlg& prefs,
											const CTDLFilterBar& filterBar,
											const CTDLTasklistStorageMgr& mgrStorage,
											const CUIExtensionMgr& mgrUIExt) const
{
	CString sTipText;

	if (IsInRange(nMenuID, ID_FILE_MRU1, ID_FILE_MRU16))
	{
		int nMRU = (nMenuID - ID_FILE_MRU1);

		if (nMRU < mru.GetSize())
			sTipText = mru.m_arrNames[nMRU];
	}
	else if (IsInRange(nMenuID, ID_WINDOW1, ID_WINDOW16))
	{
		int nTDC = (nMenuID - ID_WINDOW1);

		if (nTDC < mgrToDoCtrl.GetCount())
			sTipText = mgrToDoCtrl.GetFriendlyProjectName(nTDC);
	}
	else if (IsInRange(nMenuID, ID_TOOLS_USERTOOL1, ID_TOOLS_USERTOOL50))
	{
		USERTOOL tool;
		int nTool = (nMenuID - ID_TOOLS_USERTOOL1);

		if (prefs.GetUserTool(nTool, tool))
			sTipText = tool.sToolName;
	}
	else if (IsInRange(nMenuID, ID_FILE_OPEN_USERSTORAGE1, ID_FILE_OPEN_USERSTORAGE16))
	{
		int nStorage = (nMenuID - ID_FILE_OPEN_USERSTORAGE1);

		if (nStorage < mgrStorage.GetNumStorage())
			sTipText = mgrStorage.GetStorageMenuText(nStorage);
	}
	else if (IsInRange(nMenuID, ID_FILE_SAVE_USERSTORAGE1, ID_FILE_SAVE_USERSTORAGE16))
	{
		int nStorage = (nMenuID - ID_FILE_SAVE_USERSTORAGE1);

		if (nStorage < mgrStorage.GetNumStorage())
			sTipText = mgrStorage.GetStorageMenuText(nStorage);
	}
	else if (IsInRange(nMenuID, ID_SHOWVIEW_UIEXTENSION1, ID_SHOWVIEW_UIEXTENSION16))
	{
		int nExt = (nMenuID - ID_SHOWVIEW_UIEXTENSION1);

		if (nExt < mgrUIExt.GetNumUIExtensions())
			sTipText = mgrUIExt.GetUIExtensionMenuText(nExt);
	}
	else if (IsInRange(nMenuID, ID_ACTIVATEVIEW_UIEXTENSION1, ID_ACTIVATEVIEW_UIEXTENSION16))
	{
		int nExt = (nMenuID - ID_ACTIVATEVIEW_UIEXTENSION1);

		if (nExt < mgrUIExt.GetNumUIExtensions())
			sTipText = mgrUIExt.GetUIExtensionMenuText(nExt);
	}
	else if (IsInRange(nMenuID, ID_VIEW_ACTIVATEFILTER1, ID_VIEW_ACTIVATEFILTER24))
	{
		int nFilter = (nMenuID - ID_VIEW_ACTIVATEFILTER1);

		if (nFilter < NUM_SHOWFILTER)
			sTipText = CEnString(SHOW_FILTERS[nFilter][0]);
	}
	else if (IsInRange(nMenuID, ID_VIEW_ACTIVATEADVANCEDFILTER1, ID_VIEW_ACTIVATEADVANCEDFILTER24))
	{
		int nFilter = (nMenuID - ID_VIEW_ACTIVATEADVANCEDFILTER1);

		if (nFilter < filterBar.GetAdvancedFilterNames().GetSize())
			sTipText = filterBar.GetAdvancedFilterNames().GetAt(nFilter);
	}
	else
	{
		ASSERT(0);
	}

	// Fallback
	if (sTipText.IsEmpty())
		sTipText = GetMenuString(nMenuID, MF_BYCOMMAND);

	// removed embedded tabs
	if (!sTipText.IsEmpty())
		sTipText.Replace('\t', ' ');

	return sTipText;
}

void CTDCMainMenu::LoadMenuCommon()
{
	ASSERT(GetSafeHmenu());

#ifndef _DEBUG
	// Exclude the debug menu in release builds
	DeleteMenu(AM_DEBUG, MF_BYPOSITION);
#endif

	// delete 'Record Bug Report' if below W7
	if (COSVersion() < OSV_WIN7)
	{
		CMenu* pSubMenu = GetSubMenu(AM_HELP);
		ASSERT(pSubMenu);

		int nPos = CEnMenu::FindMenuItem(*pSubMenu, ID_HELP_RECORDBUGREPORT);
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
									   CMenuIconMgr& mgrMenuIcons) const
{
	if (GetSubMenu(AM_FILE) == pPopupMenu)
	{
		PrepareFileMenu(pPopupMenu, prefs);
		return TRUE;
	}
	else if (GetSubMenu(AM_EDIT) == pPopupMenu)
	{
		PrepareEditMenu(pPopupMenu, tdc, prefs, FALSE);
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
			PrepareUserStorageMenu(pPopupMenu, mgrStorage, mgrMenuIcons);
			return TRUE;

		case ID_VIEW_ACTIVATEFILTER1:
			PrepareFiltersActivationMenu(pPopupMenu, filterBar);
			return TRUE;

		case ID_ACTIVATEVIEW_TASKTREE:
			PrepareTaskViewActivationMenu(pPopupMenu, tdc, mgrUIExt);
			return TRUE;

		case ID_SHOWVIEW_TASKTREE:
			PrepareTaskViewVisibilityMenu(pPopupMenu, tdc, mgrUIExt);
			return TRUE;
		}
	}

	return FALSE;
}

void CTDCMainMenu::PrepareTaskContextMenu(CMenu* pMenu,
										  const CFilteredToDoCtrl& tdc,
										  const CPreferencesDlg& prefs) const
{
	PrepareEditMenu(pMenu, tdc, prefs, TRUE);

	// Remove all 'New Task' options for views not supporting them
	if (!tdc.CanCreateNewTask(TDC_INSERTATTOP))
	{
		// Get the position of the last 'new task' option
		int nPos = CEnMenu::FindMenuItem(*pMenu, ID_NEWTASK_DEPENDENTAFTERSELECTEDTASK);

		nPos++;	// Add 1 for the trailing separator
		nPos++; // Convert to 'number of items to remove'

		while (nPos--)
			pMenu->DeleteMenu(0, MF_BYPOSITION);
	}
}

void CTDCMainMenu::PrepareTabCtrlContextMenu(CMenu* pMenu,
											 const CFilteredToDoCtrl& tdc,
											 const CPreferencesDlg& prefs) const
{
	PrepareFileMenu(pMenu, prefs);
	PrepareEditMenu(pMenu, tdc, prefs, TRUE);
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
		int nPos = CEnMenu::FindMenuItem(*pMenu, ID_SENDTASKS);

		if (nPos != -1)
		{
			// Delete menu item
			pMenu->DeleteMenu(nPos, MF_BYPOSITION);

			// And then delete separator
			pMenu->DeleteMenu(nPos, MF_BYPOSITION);
		}
	}
}

void CTDCMainMenu::PrepareTaskViewVisibilityMenu(CMenu* pMenu, const CFilteredToDoCtrl& tdc, const CUIExtensionMgr& mgrUIExt)
{
	CUIExtensionHelper helper(ID_SHOWVIEW_UIEXTENSION1, 16);
	helper.AddAllExtensionsToMenu(pMenu, mgrUIExt);
	
	// ListView visibility
	pMenu->CheckMenuItem(ID_SHOWVIEW_LISTVIEW, (tdc.IsListViewTabShowing() ? MF_CHECKED : 0));

	// UI Extension visibility
	CStringArray aTypeIDs;
	tdc.GetVisibleTaskViews(aTypeIDs);

	helper.UpdateExtensionVisibilityState(pMenu, mgrUIExt, aTypeIDs);
}

void CTDCMainMenu::PrepareTaskViewActivationMenu(CMenu* pMenu, const CFilteredToDoCtrl& tdc, const CUIExtensionMgr& mgrUIExt)
{
	CStringArray aTypeIDs;
	tdc.GetVisibleTaskViews(aTypeIDs);

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

BOOL CTDCMainMenu::HandlePostTranslateMenu(HMENU hMenu) const
{
	ASSERT(::IsMenu(hMenu));

	CMenu* pSortMenu = GetSubMenu(AM_SORT);

	if (pSortMenu && (hMenu == pSortMenu->GetSafeHmenu()))
	{
		CEnMenu::SortMenuStrings(hMenu, ID_SORTBY_DEFAULTCOLUMNS_FIRST, ID_SORTBY_CUSTOMCOLUMN_LAST);
		return TRUE;
	}

	return FALSE;
}

BOOL CTDCMainMenu::HandleDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) const
{
	if ((nIDCtl == 0) && (lpDrawItemStruct->itemID == ID_CLOSE))
	{
		VERIFY(DrawMDIButton(lpDrawItemStruct));
		return TRUE;
	}

	return FALSE;
}

BOOL CTDCMainMenu::HandleMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) const
{
	if (nIDCtl == 0 && lpMeasureItemStruct->itemID == ID_CLOSE)
	{
		VERIFY(MeasureMDIButton(lpMeasureItemStruct));
		return TRUE;
	}

	return FALSE;
}

void CTDCMainMenu::PrepareEditMenu(CMenu* pMenu, const CFilteredToDoCtrl& tdc, const CPreferencesDlg& prefs, BOOL bContextMenu)
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
					PrepareEditMenu(pPopup, tdc, prefs, bContextMenu);

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

		case ID_EDIT_OPENFILELINK1:
		case ID_EDIT_SETFILELINK:
			bDelete = !tdc.IsColumnOrEditFieldShowing(TDCC_FILELINK, TDCA_FILELINK);
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

		case ID_TASKLIST_COPYCOLUMNVALUES:
		case ID_TASKLIST_COPYSELTASKSCOLUMNVALUES:
			if (bContextMenu)
				bDelete = tdc.IsExtensionView(tdc.GetTaskView());
			break;

		case ID_VIEW_TOGGLETASKEXPANDED:
			if (bContextMenu)
				bDelete = !tdc.CanExpandTasks(TDCEC_ALL, TRUE) &&
							!tdc.CanExpandTasks(TDCEC_ALL, FALSE);
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
		{
			nCountLastSep++;
		}
	}

	// make sure last item is not a separator
	int nLastItem = (int)pMenu->GetMenuItemCount() - 1;

	if (pMenu->GetMenuItemID(nLastItem) == 0)
		pMenu->DeleteMenu(nLastItem, MF_BYPOSITION);
}

void CTDCMainMenu::PrepareSortMenu(CMenu* pMenu, const CFilteredToDoCtrl& tdc, const CPreferencesDlg& prefs)
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
		int nInsert = CEnMenu::FindMenuItem(*pMenu, ID_SORTBY_NONE) - 1;
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

void CTDCMainMenu::PrepareFiltersActivationMenu(CMenu* pMenu, const CTDLFilterBar& filterBar)
{
	AddFiltersToMenu(pMenu, ID_VIEW_ACTIVATEFILTER1, ID_VIEW_ACTIVATEFILTER24, CTDCFilter::GetDefaultFilterNames(), IDS_FILTERPLACEHOLDER);
	AddFiltersToMenu(pMenu, ID_VIEW_ACTIVATEADVANCEDFILTER1, ID_VIEW_ACTIVATEADVANCEDFILTER24, filterBar.GetAdvancedFilterNames(), IDS_ADVANCEDFILTERPLACEHOLDER);

	// Restore selection
	int nSelFilter = filterBar.GetSelectedFilter();

	if (filterBar.GetFilter() == FS_ADVANCED)
	{
		CString sFilter;
		VERIFY((filterBar.GetFilter(sFilter) == FS_ADVANCED) && !sFilter.IsEmpty());

		int nFilter = Misc::Find(sFilter, filterBar.GetAdvancedFilterNames(), FALSE, TRUE);
		ASSERT(nFilter != -1);

		nSelFilter = (NUM_SHOWFILTER + 1 + nFilter); // +1 for separator
	}

	if (nSelFilter != -1)
		pMenu->CheckMenuRadioItem(0, pMenu->GetMenuItemCount(), nSelFilter, MF_BYPOSITION);
}

void CTDCMainMenu::AddFiltersToMenu(CMenu* pMenu, UINT nStart, UINT nEnd, const CStringArray& aFilters, UINT nPlaceholderStrID)
{
	ASSERT(aFilters.GetSize() || nPlaceholderStrID);

	// Get the first item insertion point
	int nStartPos = CEnMenu::FindMenuItem(*pMenu, nStart);
	ASSERT(nStartPos >= 0);

	// Delete existing items
	for (UINT nMenuID = nStart; nMenuID <= nEnd; nMenuID++)
		pMenu->DeleteMenu(nMenuID, MF_BYCOMMAND);

	// Re-add filters or placeholder if no filters
	int nNumFilters = min(aFilters.GetSize(), 24);

	if (nNumFilters)
	{
		UINT nFlags = (MF_BYPOSITION | MF_STRING);
		CString sMenuItem;

		for (int nFilter = 0; nFilter < nNumFilters; nFilter++)
		{
			sMenuItem.Format(_T("&%s"), aFilters[nFilter]);
			sMenuItem.Replace('\t', ' '); // removed embedded tabs

			pMenu->InsertMenu((nStartPos + nFilter), nFlags, (nStart + nFilter), sMenuItem);
		}
	}
	else // if nothing to add, just re-add placeholder
	{
		pMenu->InsertMenu(nStartPos, MF_BYPOSITION | MF_STRING | MF_GRAYED, nStart, CEnString(nPlaceholderStrID));
	}
}

void CTDCMainMenu::PrepareUserStorageMenu(CMenu* pMenu, const CTDLTasklistStorageMgr& mgrStorage, CMenuIconMgr& mgrMenuIcons)
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

	CUserToolArray aTools;
	prefs.GetUserTools(aTools);

	CTDCToolsHelper(FALSE, FALSE).AddToolsToMenu(aTools, *pMenu, mgrMenuIcons, TRUE/*prefs.GetWantToolsgrouping()*/);
}
