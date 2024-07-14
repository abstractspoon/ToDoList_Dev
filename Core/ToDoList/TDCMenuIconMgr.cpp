// MenuIconMgr.cpp: implementation of the CTDCMenuIconMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "TDCMapping.h"
#include "TDCMenuIconMgr.h"
#include "PreferencesDlg.h"

#include "..\shared\FileIcons.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCMenuIconMgr::CTDCMenuIconMgr()
	:
	m_nNewTaskID(0), 
	m_nNewSubtaskID(0)
{
}

CTDCMenuIconMgr::~CTDCMenuIconMgr()
{
}

void CTDCMenuIconMgr::Populate(const CPreferencesDlg& prefs)
{
	if (!CMenuIconMgr::IsInitialized())
	{
		ASSERT(0);
		return;
	}

	if (HasImages())
		return;

	// Toolbar images
	CUIntArray aCmdIDs;

	// Main toolbar ---------------------------------------
	aCmdIDs.Add(ID_LOAD_NORMAL);
	aCmdIDs.Add(ID_SAVE_NORMAL);
	aCmdIDs.Add(ID_SAVEALL);

	m_nNewTaskID = GetNewTaskCmdID(prefs);
	m_nNewSubtaskID = GetNewSubtaskCmdID(prefs);

	aCmdIDs.Add(m_nNewTaskID);
	aCmdIDs.Add(m_nNewSubtaskID);

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

	AddImages(aCmdIDs, IDB_APP_EXTRA_STD, colorMagenta);

	// social images ---------------------------------------
	aCmdIDs.RemoveAll();

	aCmdIDs.Add(ID_HELP_WIKI);
	aCmdIDs.Add(ID_HELP_FORUM);

	AddImages(aCmdIDs, IDB_SOCIAL_TOOLBAR, colorMagenta);

	// Tray icon
	AddImage(ID_TRAYICON_CREATETASK, GetNewTaskCmdID(prefs));
	AddImage(ID_TRAYICON_CLOSE, ID_EXIT);

	// Displaying logged time
	AddImage(ID_SHOWTIMELOGFILE, CFileIcons::ExtractIcon(_T(".csv")));
}

void CTDCMenuIconMgr::Release()
{
	CMenuIconMgr::Release();

	// TODO
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
	if (!HasImages())
		return;

	// figure out current state
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

// menu icons
void CTDCMenuIconMgr::UpdateNewTaskIcons(const CPreferencesDlg& prefs)
{
	UINT nPrevID = m_nNewTaskID;
	m_nNewTaskID = GetNewTaskCmdID(prefs);
	ChangeImageID(nPrevID, m_nNewSubtaskID);

	nPrevID = m_nNewSubtaskID;
	m_nNewSubtaskID = GetNewSubtaskCmdID(prefs);
	ChangeImageID(nPrevID, m_nNewSubtaskID);
}

