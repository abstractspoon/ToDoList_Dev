// MenuIconMgr.cpp: implementation of the CTDCMenuIconMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "TDCMapping.h"
#include "TDCMenuIconMgr.h"
#include "TDLCustomToolbar.h"
#include "PreferencesDlg.h"
#include "ToDoCtrl.h"

#include "..\shared\FileIcons.h"
#include "..\shared\Misc.h"
#include "..\shared\GraphicsMisc.h"
#include "..\shared\PasswordDialog.h"

#include "..\Interfaces\SpellCheckDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCMenuIconMgr::CTDCMenuIconMgr()
	:
	m_nNewTaskCmdID(0), 
	m_nNewSubtaskCmdID(0)
{
}

CTDCMenuIconMgr::~CTDCMenuIconMgr()
{
}

void CTDCMenuIconMgr::Populate(const CPreferencesDlg& prefs)
{
	if (HasImages())
		return;

	// Toolbar images
	CUIntArray aCmdIDs;

	// Main toolbar ---------------------------------------
	aCmdIDs.Add(ID_LOAD_NORMAL);
	aCmdIDs.Add(ID_SAVE_NORMAL);
	aCmdIDs.Add(ID_SAVEALL);

	m_nNewTaskCmdID = GetNewTaskCmdID(prefs);
	m_nNewSubtaskCmdID = GetNewSubtaskCmdID(prefs);

	aCmdIDs.Add(m_nNewTaskCmdID);
	aCmdIDs.Add(m_nNewSubtaskCmdID);

	aCmdIDs.Add(ID_EDIT_TASKTEXT);
	aCmdIDs.Add(ID_EDIT_SETTASKICON);
	aCmdIDs.Add(ID_EDIT_SETREMINDER);
	aCmdIDs.Add(ID_EDIT_UNDO);
	aCmdIDs.Add(ID_EDIT_REDO);
	aCmdIDs.Add(ID_VIEW_MAXTASKLIST);
	aCmdIDs.Add(ID_VIEW_EXPANDTASK);
	aCmdIDs.Add(ID_VIEW_COLLAPSETASK);
	aCmdIDs.Add(ID_VIEW_PREV_SEL);
	aCmdIDs.Add(ID_VIEW_NEXT_SEL);
	aCmdIDs.Add(ID_EDIT_FINDTASKS);
	aCmdIDs.Add(ID_SORT);
	aCmdIDs.Add(ID_DELETETASK);
	aCmdIDs.Add(ID_TOOLS_TOGGLECHECKIN); // placeholder
	aCmdIDs.Add(ID_TOOLS_TOGGLEDARKMODE);
	aCmdIDs.Add(ID_PREFERENCES);
	aCmdIDs.Add(ID_HELP_WIKI);

	AddImages(aCmdIDs, IDB_APP_TOOLBAR_STD, colorMagenta);

	// extra -----------------------------------------------
	aCmdIDs.RemoveAll();

	aCmdIDs.Add(ID_HELP_DONATE);
	aCmdIDs.Add(ID_TOOLS_SELECTINEXPLORER);
	aCmdIDs.Add(ID_TOOLS_ADDTOSOURCECONTROL);
	aCmdIDs.Add(ID_TOOLS_REMOVEFROMSOURCECONTROL);
	aCmdIDs.Add(ID_VIEW_INCREMENTTASKVIEWFONTSIZE);
	aCmdIDs.Add(ID_VIEW_DECREMENTTASKVIEWFONTSIZE);
	aCmdIDs.Add(ID_EXIT);
	aCmdIDs.Add(ID_FILE_ENCRYPT);
	aCmdIDs.Add(ID_EDIT_PASTEATTRIBUTES);
	aCmdIDs.Add(ID_TOOLS_IMPORT);
	aCmdIDs.Add(ID_TOOLS_EXPORT);
	aCmdIDs.Add(ID_TOOLS_ANONYMIZE_TASKLIST);

	AddImages(aCmdIDs, IDB_APP_EXTRA_STD, colorMagenta);
	AddImage(ID_TOOLS_ANONYMIZE_TASKTIMELOG, ID_TOOLS_ANONYMIZE_TASKLIST); // copy

	// social images ---------------------------------------
	aCmdIDs.RemoveAll();

	aCmdIDs.Add(ID_HELP_WIKI);
	aCmdIDs.Add(ID_HELP_FORUM);

	AddImages(aCmdIDs, IDB_SOCIAL_TOOLBAR, colorMagenta);

	// Insert date/time ------------------------------------
	aCmdIDs.RemoveAll();

	aCmdIDs.Add(ID_EDIT_INSERTDATE);
	aCmdIDs.Add(ID_EDIT_INSERTTIME);
	aCmdIDs.Add(ID_EDIT_INSERTDATETIME);

	AddImages(aCmdIDs, IDB_DATETIME_TOOLBAR_STD, colorMagenta);

	// Tray icon -------------------------------------------
	AddImage(ID_TRAYICON_CREATETASK, GetNewTaskCmdID(prefs));
	AddImage(ID_TRAYICON_CLOSE, ID_EXIT);

	// Displaying logged time ------------------------------
	AddImage(ID_SHOWTIMELOGFILE, CFileIcons::ExtractIcon(_T(".csv")));
}

void CTDCMenuIconMgr::ClearImages()
{
	CMenuIconMgr::ClearImages();

	m_nNewTaskCmdID = m_nNewSubtaskCmdID = 0;
	m_aCustomToolbarCmdIDs.RemoveAll();
}

UINT CTDCMenuIconMgr::GetNewTaskCmdID(const CPreferencesDlg& prefs)
{
	return TDC::MapNewTaskPosToCmdID(prefs.GetNewTaskPos(), FALSE);
}

UINT CTDCMenuIconMgr::GetNewSubtaskCmdID(const CPreferencesDlg& prefs)
{
	return TDC::MapNewTaskPosToCmdID(prefs.GetNewSubtaskPos(), TRUE);
}

void CTDCMenuIconMgr::UpdateSourceControlStatus(BOOL bIsDisabled, BOOL bIsCheckedOut)
{
	if (HasImages())
	{
		// figure out previous state
		BOOL bWasDisabled = HasImageID(ID_TOOLS_TOGGLECHECKIN);
		BOOL bWasCheckedOut = (!bWasDisabled && HasImageID(ID_TOOLS_CHECKIN));

		if (bIsDisabled)
		{
			if (bWasDisabled)
			{
				return; // no change
			}
			else if (bWasCheckedOut)
			{
				ChangeImageID(ID_TOOLS_CHECKIN, ID_TOOLS_TOGGLECHECKIN);
			}
			else // checked in
			{
				ChangeImageID(ID_TOOLS_CHECKOUT, ID_TOOLS_TOGGLECHECKIN);
			}
		}
		else if (bIsCheckedOut)
		{
			if (bWasDisabled)
			{
				ChangeImageID(ID_TOOLS_TOGGLECHECKIN, ID_TOOLS_CHECKIN);
			}
			else if (bWasCheckedOut)
			{
				return; // no change
			}
			else // checked in
			{
				ChangeImageID(ID_TOOLS_CHECKOUT, ID_TOOLS_CHECKIN);
			}
		}
		else // new == checked in
		{
			if (bWasDisabled)
			{
				ChangeImageID(ID_TOOLS_TOGGLECHECKIN, ID_TOOLS_CHECKOUT);
			}
			else if (bWasCheckedOut)
			{
				ChangeImageID(ID_TOOLS_CHECKIN, ID_TOOLS_CHECKOUT);
			}
			else // checked in
			{
				return; // no change
			}
		}
	}
}

void CTDCMenuIconMgr::UpdateNewTaskIcons(const CPreferencesDlg& prefs)
{
	if (HasImages())
	{
		UINT nPrevID = m_nNewTaskCmdID;
		m_nNewTaskCmdID = GetNewTaskCmdID(prefs);
		ChangeImageID(nPrevID, m_nNewTaskCmdID);

		nPrevID = m_nNewSubtaskCmdID;
		m_nNewSubtaskCmdID = GetNewSubtaskCmdID(prefs);
		ChangeImageID(nPrevID, m_nNewSubtaskCmdID);
	}
}

void CTDCMenuIconMgr::UpdateCustomToolbar(const CToolBar& toolbar)
{
	if (HasImages() && toolbar.GetSafeHwnd())
	{
		// 1. Remove icons for the previous custom toolbar setup
		RemoveImages(m_aCustomToolbarCmdIDs);
		m_aCustomToolbarCmdIDs.RemoveAll();

		// 2. Snapshot the current IDs so we can work out what was added
		CUIntSet mapExistIDs;
		Misc::GetKeysT<UINT, HICON, CUIntSet>(m_mapID2Icon, mapExistIDs);

		// 3. Do the add - this will skip any command IDs already being managed
		AddImages(toolbar);

		// 4. Save off only what was actually added
		CUIntSet mapNewIDs;
		Misc::GetKeysT<UINT, HICON, CUIntSet>(m_mapID2Icon, mapNewIDs);

		CUIntSet mapDiffIDs;

		if (mapNewIDs.GetDifferences(mapExistIDs, mapDiffIDs))
			mapDiffIDs.CopyTo(m_aCustomToolbarCmdIDs);
	}
}

void CTDCMenuIconMgr::UpdateStaticDialogIcons()
{
	CToDoCtrl::SetDialogIcons(GetIcon(ID_EDIT_SETTASKICON),
							  GetIcon(ID_EDIT_DEPENDENCY),
							  GetIcon(ID_EDIT_RECURRENCE),
							  GetIcon(ID_ADDTIMETOLOGFILE));

	CPasswordDialog::SetIcon(GetIcon(ID_FILE_ENCRYPT));
}

