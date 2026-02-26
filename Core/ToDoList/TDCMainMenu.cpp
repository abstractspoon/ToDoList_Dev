// MenuEx.cpp: implementation of the CTDCMainMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "TDCMainMenu.h"
#include "TDCStatic.h"
#include "TDCMapping.h"
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
#include "..\shared\enbrowserctrl.h"

#include "..\Interfaces\UIExtensionHelper.h"
#include "..\Interfaces\UIExtensionMgr.h"

#include <afxadv.h> // for CRecentFileList

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

#ifndef HBMMENU_MBAR_CLOSE
#	define HBMMENU_MBAR_CLOSE          ((HBITMAP)5)
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

	AddLanguageButton();
	AddTabCloseButton(prefs);

	// Right-align 'Debug' menu and don't translate
#ifdef _DEBUG
	ModifyMenu(AM_DEBUG, MF_BYPOSITION | MFT_RIGHTJUSTIFY, 0, _T("&Debug"));
	CLocalizer::EnableTranslation(::GetSubMenu(GetSafeHmenu(), AM_DEBUG), FALSE);
#endif

	return TRUE;
}

void CTDCMainMenu::AddLanguageButton()
{
	// Avoid non-supportive setups
	if (!CThemed::IsNonClientThemed() || (COSVersion() < OSV_VISTA))
		return;

	// Only have to prepare the bitmap once per session 
	// because it's not possible to change the UI language 
	// without restarting the app
	if (m_bmUILang.GetSafeHandle() == NULL)
	{
		CString sUILang = CLocalizer::GetDictionaryPath();
		CEnBitmap bmp;
		
		if (sUILang.IsEmpty())
		{
			VERIFY(bmp.LoadBitmap(IDB_UK_FLAG));
		}
		else 
		{
			CString sIconPath(sUILang);
			FileMisc::ReplaceExtension(sIconPath, _T("png"));
			
			if (!bmp.LoadImage(sIconPath))
				VERIFY(bmp.LoadBitmap(IDB_YOURLANG_FLAG));
		}

		int nReqSize = GraphicsMisc::ScaleByDPIFactor(16);
		bmp.ResizeImage(nReqSize, nReqSize, colorMagenta);

		bmp.ConvertToPARGB32(colorMagenta);
		m_bmUILang.Attach(bmp.Detach());
	}

	VERIFY(AppendMenu((MFT_RIGHTJUSTIFY | MFT_BITMAP), ID_PREFERENCES_EDITUILANGUAGE, &m_bmUILang));
}

void CTDCMainMenu::AddTabCloseButton(const CPreferencesDlg& prefs)
{
	if (!prefs.GetShowTabCloseButtons())
	{
		if (!m_bmTabClose.GetSafeHandle())
			m_bmTabClose.Attach(HBMMENU_MBAR_CLOSE);

		VERIFY(AppendMenu((MFT_RIGHTJUSTIFY | MFT_BITMAP), ID_CLOSE, &m_bmTabClose));
	}
}

BOOL CTDCMainMenu::LoadMenu()
{
	if (!CEnMenu::LoadMenu(IDR_MAINFRAME, NULL, TRUE, TRUE))
		return FALSE;

	LoadMenuCommon();
	TranslateDynamicMenuItems();

	return TRUE;
}

void CTDCMainMenu::TranslateDynamicMenuItems()
{
	CEnMenu::TranslateDynamicMenuItems(ID_FILE_MRU1, ID_FILE_MRU16, _T("Recent Tasklist %d"));
	CEnMenu::TranslateDynamicMenuItems(ID_WINDOW1, ID_WINDOW16, _T("Window %d"));
	CEnMenu::TranslateDynamicMenuItems(ID_TOOLS_USERTOOL1, ID_TOOLS_USERTOOL50, _T("User Defined Tool %d"));
	CEnMenu::TranslateDynamicMenuItems(ID_FILE_OPEN_USERSTORAGE1, ID_FILE_OPEN_USERSTORAGE16, _T("3rd Party Storage %d"));
	CEnMenu::TranslateDynamicMenuItems(ID_FILE_SAVE_USERSTORAGE1, ID_FILE_SAVE_USERSTORAGE16, _T("3rd Party Storage %d"));
	CEnMenu::TranslateDynamicMenuItems(ID_SHOWVIEW_UIEXTENSION1, ID_SHOWVIEW_UIEXTENSION16, _T("Task View %d"));
	CEnMenu::TranslateDynamicMenuItems(ID_ACTIVATEVIEW_UIEXTENSION1, ID_ACTIVATEVIEW_UIEXTENSION16, _T("Task View %d"));
	CEnMenu::TranslateDynamicMenuItems(ID_VIEW_ACTIVATEFILTER1, ID_VIEW_ACTIVATEFILTER24, _T("Default Filter %d"));
	CEnMenu::TranslateDynamicMenuItems(ID_VIEW_ACTIVATEADVANCEDFILTER1, ID_VIEW_ACTIVATEADVANCEDFILTER24, _T("Find Tasks Filter %d"));

	HMENU hSubMenu = GetSubMenu(*this, ID_VIEW_SAVETOIMAGE);

	if (hSubMenu)
	{
		CEnString sMenuItem(ID_VIEW_SAVETOIMAGE);
		sMenuItem.Replace(_T("(%s)"), _T(""));

		CEnMenu::SetMenuString(hSubMenu, ID_VIEW_SAVETOIMAGE, sMenuItem, MF_BYCOMMAND);
	}
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
		TDCUSERTOOL tool;
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

		if (nFilter < filterBar.AdvancedFilterNames().GetSize())
			sTipText = filterBar.AdvancedFilterNames().GetAt(nFilter);
	}
	else
	{
		ASSERT(0);
	}

	// Fallback
	if (sTipText.IsEmpty())
		sTipText = GetMenuString(nMenuID, MF_BYCOMMAND);

	// Add parent menu name for context
	HMENU hParentMenu = NULL;
	
	int nPos = FindMenuItem(*this, nMenuID, hParentMenu);
	ASSERT(hParentMenu);

	nPos = FindMenuItem(*this, hParentMenu, hParentMenu);
	ASSERT(hParentMenu);

	sTipText = (GetMenuString(hParentMenu, nPos, MF_BYPOSITION) + _T(" > ") + sTipText);

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
			PrepareUserStorageMenu(pPopupMenu, mgrStorage, mgrMenuIcons);
			return TRUE;

		case ID_VIEW_ACTIVATEFILTER1:
			PrepareFiltersActivationMenu(pPopupMenu, filterBar, prefs);
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
										  const CPreferencesDlg& prefs)
{
	PrepareEditMenu(pMenu, tdc, prefs);

	// Remove all 'New Task' options for views not supporting them
	if (!tdc.CanCreateNewTask(TDC_INSERTATTOP))
	{
		// Get the position of the last 'new task' option
		int nPos = CEnMenu::FindMenuItem(*pMenu, ID_NEWTASK_DEPENDENTAFTERSELECTEDTASK);

		if (nPos != -1)
		{
			nPos++;	// Add 1 for the trailing separator
			nPos++; // Convert to 'number of items to remove'

			while (nPos--)
				pMenu->DeleteMenu(0, MF_BYPOSITION);
		}
	}

	// Likewise for copying column attributes
	if (tdc.IsExtensionView(tdc.GetActiveTaskView()))
	{
		int nPos = CEnMenu::FindMenuItem(*pMenu, ID_TASKLIST_COPYCOLUMNVALUES);

		if (nPos != -1)
		{
			pMenu->DeleteMenu(nPos, MF_BYPOSITION); // ID_TASKLIST_COPYCOLUMNVALUES
			pMenu->DeleteMenu(nPos, MF_BYPOSITION); // ID_TASKLIST_COPYSELTASKSCOLUMNVALUES
			pMenu->DeleteMenu(nPos, MF_BYPOSITION); // Trailing separator
		}
	}

	// Add task expansion
	if (!tdc.CanExpandTasks(TDCEC_ALL, TRUE) &&
		!tdc.CanExpandTasks(TDCEC_ALL, FALSE))
	{
		int nPos = CEnMenu::FindMenuItem(*pMenu, ID_VIEW_TOGGLETASKEXPANDED);

		if (nPos != -1)
		{
			pMenu->DeleteMenu(nPos, MF_BYPOSITION); // ID_VIEW_TOGGLETASKEXPANDED
			pMenu->DeleteMenu(nPos, MF_BYPOSITION); // Trailing separator
		}
	}
}

void CTDCMainMenu::PrepareTabCtrlContextMenu(CMenu* pMenu,
											 const CFilteredToDoCtrl& tdc,
											 const CPreferencesDlg& prefs)
{
	PrepareFileMenu(pMenu, prefs);
	PrepareEditMenu(pMenu, tdc, prefs);
}

void CTDCMainMenu::PrepareFileMenu(CMenu* pMenu, const CPreferencesDlg& prefs)
{
	ASSERT(pMenu);

	if (!pMenu)
		return;

	BOOL bHasMinToTrayOption = prefs.HasSysTrayOptions(STO_ONCLOSE, STO_ONMINCLOSE);
	BOOL bHasMinToTrayID = (::GetMenuString(*pMenu, ID_MINIMIZETOTRAY, NULL, 0, MF_BYCOMMAND) != 0);

	int nCountLastSep = 0;

	for (int nItem = 0; nItem < (int)pMenu->GetMenuItemCount(); nItem++)
	{
		BOOL bDelete = FALSE;
		BOOL bIsSeparator = FALSE;

		UINT nMenuID = pMenu->GetMenuItemID(nItem);

		switch (nMenuID)
		{
		case ID_SEPARATOR:
			bIsSeparator = TRUE;
			bDelete = (nCountLastSep == 0);
			nCountLastSep = 0;
			break;

		case ID_PRINT:
			bDelete = !CEnBrowserCtrl::SupportsPrint();
			break;

		case ID_PRINTPREVIEW:
			bDelete = !CEnBrowserCtrl::SupportsPrintPreview();
			break;

		case ID_MINIMIZETOTRAY:
			bDelete = !bHasMinToTrayOption;
			break;

		case ID_SENDTASKS:
			bDelete = !CMSOutlookHelper::IsOutlookInstalled();
			break;

		case ID_EXIT:
			if (bHasMinToTrayOption && !bHasMinToTrayID)
			{
				pMenu->InsertMenu(ID_EXIT, MF_BYCOMMAND, ID_MINIMIZETOTRAY, CEnString(ID_MINIMIZETOTRAY));
				nItem++;
			}
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
	int nSelMenuID = TDC::MapTaskViewToActivateID(tdc.GetActiveTaskView());

	pMenu->CheckMenuRadioItem(ID_ACTIVATEVIEW_TASKTREE, ID_ACTIVATEVIEW_UIEXTENSION16, nSelMenuID, MF_BYCOMMAND);
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

void CTDCMainMenu::PrepareEditMenu(CMenu* pMenu, const CFilteredToDoCtrl& tdc, const CPreferencesDlg& prefs)
{
	ASSERT(pMenu);

	if (!pMenu)
		return;

	BOOL bVisibleColsEditsOnly = prefs.GetShowEditMenuAsColumns();
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
					PrepareEditMenu(pPopup, tdc, prefs); // RECURSIVE CALL

					// if the popup is now empty remove it too
					bDelete = !pPopup->GetMenuItemCount();
				}
			}
			break;

		case ID_EDIT_SETTASKLISTCOLOR:
		case ID_EDIT_CLEARTASKLISTCOLOR:
			{
				bDelete = !CTabCtrlEx::IsSupportedFlag(TCE_TABCOLORS);
			}
			break;

		case ID_EDIT_TASKCOLOR:
		case ID_EDIT_CLEARTASKCOLOR:
			if (prefs.GetTextColorOption() != TEXTOPT_DEFAULT)
			{
				bDelete = TRUE;
			}
			else if (bVisibleColsEditsOnly && !tdc.IsEditFieldShowing(TDCA_COLOR))
			{
				bDelete = TRUE;
			}
			break;

		case ID_EDIT_DECTASKPRIORITY:
		case ID_EDIT_INCTASKPRIORITY:
		case ID_EDIT_SETPRIORITYNONE:
			if (bVisibleColsEditsOnly)
			{
				bDelete = !tdc.IsColumnOrEditFieldShowing(TDCC_PRIORITY, TDCA_PRIORITY);
			}
			break;

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
			if (bVisibleColsEditsOnly && !tdc.IsColumnOrEditFieldShowing(TDCC_PRIORITY, TDCA_PRIORITY))
			{
				bDelete = TRUE;
			}
			else
			{
				int nPriority = (int)(nMenuID - ID_EDIT_SETPRIORITY0);
				int nNumLevels = prefs.GetNumPriorityRiskLevels();

				if (nPriority >= nNumLevels)
				{
					bDelete = TRUE;
				}
				else if ((nNumLevels < 11) && (nPriority == 0))
				{
					UINT aStrResIDs[11];
					TDC::GetPriorityRiskLevelStringResourceIDs(nNumLevels, aStrResIDs);

					for (int nLevel = 0; nLevel < nNumLevels; nLevel++)
					{
						CEnMenu::SetMenuString(*pMenu, 
											   (nLevel + ID_EDIT_SETPRIORITY0),
											   Misc::Format(_T("%d (%s)"), nLevel, CEnString(aStrResIDs[nLevel])),
											   MF_BYCOMMAND);
					}
				}
			}
			break;

		case ID_EDIT_OFFSETDATES:
			if (bVisibleColsEditsOnly)
			{
				bDelete = !(tdc.IsColumnOrEditFieldShowing(TDCC_STARTDATE, TDCA_STARTDATE) ||
							tdc.IsColumnOrEditFieldShowing(TDCC_DUEDATE, TDCA_DUEDATE) ||
							tdc.IsColumnOrEditFieldShowing(TDCC_DONEDATE, TDCA_DONEDATE));
			}
			break;

		case ID_EDIT_CLOCK_TASK:
			if (bVisibleColsEditsOnly)
			{
				bDelete = !(tdc.IsColumnShowing(TDCC_TRACKTIME) ||
							tdc.IsColumnOrEditFieldShowing(TDCC_TIMESPENT, TDCA_TIMESPENT));
			}
			break;

		case ID_SHOWTIMELOGFILE:
		case ID_ADDTIMETOLOGFILE:
			if (bVisibleColsEditsOnly && prefs.GetLogTimeTracking())
			{
				bDelete = !(tdc.IsColumnShowing(TDCC_TRACKTIME) ||
							tdc.IsColumnOrEditFieldShowing(TDCC_TIMESPENT, TDCA_TIMESPENT));
			}
			break;

		case ID_EDIT_DECTASKPERCENTDONE:
		case ID_EDIT_INCTASKPERCENTDONE:
			if (bVisibleColsEditsOnly)
			{
				bDelete = !tdc.IsColumnOrEditFieldShowing(TDCC_PERCENT, TDCA_PERCENT);
			}
			break;

		case ID_EDIT_OPENFILELINK1:
		case ID_EDIT_SETFILELINK:
			if (bVisibleColsEditsOnly)
			{
				bDelete = !tdc.IsColumnOrEditFieldShowing(TDCC_FILELINK, TDCA_FILELINK);
			}
			break;

		case ID_EDIT_FLAGTASK:
			if (bVisibleColsEditsOnly)
			{
				bDelete = !tdc.IsColumnShowing(TDCC_FLAG);
			}
			break;

		case ID_EDIT_RECURRENCE:
			if (bVisibleColsEditsOnly)
			{
				bDelete = !tdc.IsColumnOrEditFieldShowing(TDCC_RECURRENCE, TDCA_RECURRENCE);
			}
			break;

		case ID_EDIT_GOTODEPENDENCY:
			if (bVisibleColsEditsOnly)
			{
				bDelete = !tdc.IsColumnOrEditFieldShowing(TDCC_DEPENDENCY, TDCA_DEPENDENCY);
			}
			break;

		case ID_EDIT_LOCKTASK:
		case ID_EDIT_SETTASKICON:
		case ID_EDIT_CLEARTASKICON:
			// Always visible
			break;
			
		case ID_SEPARATOR:
			{
				bIsSeparator = TRUE;
				bDelete = (nCountLastSep == 0);
				nCountLastSep = 0;
			}
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
		// Always rebuild from scratch because we don't know
		// what's changed since the last time we popped up
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

			pMenu->InsertMenu(nInsert++, 
								MF_BYPOSITION, 
								nMenuID, 
								CEnString().Format(IDS_CUSTOMCOLUMN, attribDef.sLabel));
		}

		// Resort the menu if we're not translated else 
		// it'll get done in HandlePostTranslateMenu()
		if (!CLocalizer::IsInitialized())
			CEnMenu::SortMenuStrings(*pMenu, ID_SORTBY_DEFAULTCOLUMNS_FIRST, ID_SORTBY_CUSTOMCOLUMN_LAST);
	}
}

void CTDCMainMenu::PrepareFiltersActivationMenu(CMenu* pMenu, const CTDLFilterBar& filterBar, const CPreferencesDlg& prefs)
{
	AddFiltersToMenu(pMenu, ID_VIEW_ACTIVATEFILTER1, ID_VIEW_ACTIVATEFILTER24, CTDCFilter::GetDefaultFilterNames(), IDS_FILTERPLACEHOLDER);
	AddFiltersToMenu(pMenu, ID_VIEW_ACTIVATEADVANCEDFILTER1, ID_VIEW_ACTIVATEADVANCEDFILTER24, filterBar.AdvancedFilterNames(), IDS_ADVANCEDFILTERPLACEHOLDER);
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

BOOL CTDCMainMenu::GetFilterToActivate(UINT nMenuID,
									   const CTDLFilterBar& filterBar,
									   const CPreferencesDlg& prefs,
									   FILTER_SHOW& nShow,
									   CString& sAdvFilter)
{
	if (IsInRange(nMenuID, ID_VIEW_ACTIVATEFILTER1, ID_VIEW_ACTIVATEFILTER24))
	{
		int nFilter = (nMenuID - ID_VIEW_ACTIVATEFILTER1);

		if ((nFilter >= 0) && (nFilter < NUM_SHOWFILTER))
		{
			nShow = (FILTER_SHOW)SHOW_FILTERS[nFilter][1];
			sAdvFilter.Empty();

			if (!prefs.GetShowDefaultFiltersInFilterBar())
				return (nShow == FS_ALL);

			// else
			return TRUE;
		}
	}
	else if (IsInRange(nMenuID, ID_VIEW_ACTIVATEADVANCEDFILTER1, ID_VIEW_ACTIVATEADVANCEDFILTER24))
	{
		int nFilter = (nMenuID - ID_VIEW_ACTIVATEADVANCEDFILTER1);

		if ((nFilter >= 0) && (nFilter < filterBar.AdvancedFilterNames().GetSize()))
		{
			nShow = FS_ADVANCED;

			sAdvFilter = filterBar.AdvancedFilterNames()[nFilter];
			ASSERT(!sAdvFilter.IsEmpty());

			return TRUE;
		}
	}

	ASSERT(0);
	return FALSE;
}

UINT CTDCMainMenu::GetSelectedFilterMenuID(const CTDLFilterBar& filterBar)
{
	CString sAdvFilter;
	FILTER_SHOW nShow = filterBar.GetFilter(sAdvFilter);

	if (nShow == FS_ADVANCED)
	{
		int nFilter = Misc::Find(sAdvFilter, filterBar.AdvancedFilterNames(), FALSE, TRUE);
		ASSERT(nFilter != -1);

		return (ID_VIEW_ACTIVATEADVANCEDFILTER1 + nFilter);
	}

	// else
	return (ID_VIEW_ACTIVATEFILTER1 + nShow - FS_ALL);
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
			CString sMenuItem;
			CEnString sText(mgrStorage.GetStorageMenuText(nStore));

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

	CTDCUserToolArray aTools;
	prefs.GetUserTools(aTools);

	CTDCToolsHelper(FALSE).AddToolsToMenu(aTools, *pMenu, mgrMenuIcons);
}
