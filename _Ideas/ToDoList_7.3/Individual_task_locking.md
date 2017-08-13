## Notes ##

* I propose to re-purpose the task lock attribute (and column) for checking-in/out tasks when a tasklist has been 'added' to source control (Menu > Source Control > Add Tasklist to Source Control)
	* I will probably hide 'Menu > Edit > Lock Task Attributes' and add new menu items for checking-in/out tasks to the Source Control menu
		* 'Check-out Selected Task'
		* 'Check-in Selected Task'
		* 'Check-in All Checked-out Tasks'
* Checking-out the entire tasklist will still be required for adding, deleting and moving tasks
	* Having the tasklist checked out will still require individual tasks to be checked-out in order to modify their content.
* Files for each checked-out task will be stored in a folder having the name '[Tasklist].ssc' (aka 'Simple Source Control').
* Change CToDoCtrl::CanEditSelectedTask() to take a TDC_ATTRIBUTE argument so we can distinguish between Attribute edits, Lock edits and Create/Move/Delete operations

## Processes ##

#### Checking Out a Tasklist ####
1. Attempt to create an empty file having the name '[TaskList].tsc' using the 'CREATE_NEW' file creation flag
	* If this fails it means someone else has checked-out the tasklist so fail the check-out and notify the user
2. Replace this empty file with the header of the tasklist updated to include the check-out name
3. No need to re-save the tasklist with the check-out name
4. Mark the tasklist as checked-out as before

#### Checking In a Tasklist ####
1. Check tasklist in as before
2. No need to re-save the tasklist without the check-out name
3. Delete '[TaskList].tsc'

#### Checking Out a Single Task ####
1. Attempt to create an empty file having the name '[TaskID].tsc' using the 'CREATE_NEW' file creation flag
	* If this fails it means someone else has checked-out the task so fail the check-out and notify the user
2. Replace this empty file with the existing task state, saved as as a minimal tasklist ie. Just the task itself and the check-out name
3. Change task state to 'unlocked' in the UI
4. Keep a list of checked-out tasks per tasklist

#### Checking In a Task ####
1. Save current task state to '[TaskID].tsc' file(s) as a minimal tasklist
2. Clear tasklist 'Modified' flag
3. Check out entire tasklist
	* If that fails then fail the check-in and notify the user
4. Merge '[TaskID].tsc' file(s) into tasklist 
5. Check in tasklist
6. Delete '[TaskID].tsc'
7. Stop tracking checked-in task(s)
8. Change task(s) state to 'locked'

#### Loading Tasklists ####
1. Load base tasklist 
2. Mark all tasks as 'locked'
3. Load any checked out tasks ('[TaskID].tsc' files) belonging to 'us', merge them into the base tasklist, and mark them as 'unlocked'

#### Saving Tasklists ####
1. Save checked-out task(s) to '[TaskID].tsc' file(s)
2. If the tasklist is checked-out then save the entire tasklist.

## Pseudo-Code Fragments ##

#### Checking for Edit Capabilities ####

Note: `CToDoCtrlMgr::UpdateToDoCtrlReadOnlyUIState()` will have to be modified to only check the read-only status of the file on disk ie. Not include the check-in/out status.
```
BOOL CToDoCtrl::CanEditSelectedTask(TDC_ATTRIBUTE nAttrib)
{
	if (IsReadOnly())
	  return FALSE;

	switch (nAttrib)
	{
	case TDCA_:
	case TDCA_:
	case TDCA_:
	case TDCA_:
	case TDCA_:
	case TDCA_:
		return m_taskTree.SelectionHasUnlocked();
	
	case TDCA_NEWTASK:
		return m_bCheckedOut;
	  
	case TDCA_DELETE:
	case TDCA_POSITION: // move
		return (m_bCheckedOut && GetSelectedCount());
	
	case TDCA_LOCK:
		return GetSelectedCount();
	  
	default:
		if (CTDCCustomAttributeHelper::IsCustomAttribute(nAttrib))
			return m_taskTree.SelectionHasUnlocked();
		break;
	}
	
	// all else
	ASSERT(0);
	return FALSE;
}
```

#### Checking-in/out a Task ####
```
BOOL CToDoCtrl::IsTaskLocked(DWORD dwTaskID) const
{
	if (IsSourceControlled())
		return !m_aCheckedOutTasks.Has(dwTaskID);
	
	// else
	return m_data.IsTaskLocked(dwTaskID);
}

CString CToDoCtrl::GetTaskSourceControlFolder() const
{
	if (m_sLastSavePath.IsEmpty())
		return _T("");
	
	CString sPath(m_sLastSavePath);
	FileMisc::ReplaceExtension(_T(".ssc"));
	
	return sPath;
}

CString CToDoCtrl::GetTaskSourceControlPath(DWORD dwTaskID) const
{
	CString sFolder(GetTaskSourceControlFolder());
	
	if (sFolder.IsEmpty())
		return sFolder;
		
	CString sPath;
	sPath.Format(_T("%s\\%ld.tsc"), sFolder, dwTaskID);
	return sPath;
)

void CToDoCtrl::ToggleSelectedTaskCheckOut()
{
	BOOL bResult = FALSE;
	POSITION pos = TSH().GetFirstItemPos();
		
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		VERIFY(ToggleTaskCheckout(dwTaskID));			
	}
}

BOOL CToDoCtrl::ToggleTaskCheckOut(DWORD dwTaskID)
{
	ASSERT(IsSourceControlled());
	
	if (m_mapCheckedOutIDs.Has(dwTaskID))
		return CheckinTask(dwTaskID);
	
	// else
	return CheckOutTask(dwTaskID);
}

BOOL CToDoCtrl::CheckOutTask(DWORD dwTaskID)
{
	ASSERT (!m_mapCheckedOutIDs.Has(dwTaskID));

	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	
	if (!GetTrueTask(dwTaskID, pTDI, pTDS))
		return FALSE;

	CString sTaskPath = GetTaskSourceControlPath(dwTaskID); 

	HFILE hFile = CreateFile(sTaskPath,		// name of the write
				 GENERIC_WRITE,		// open for writing
				 0,			// do not share
				 NULL,			// default security
				 CREATE_NEW,		// create new file only
				 FILE_ATTRIBUTE_NORMAL,	// normal file
				 NULL);			// no attr. template

	if (hFile == INVALID_HANDLE_VALUE) 
	{ 
		// ERROR_FILE_EXISTS
		return FALSE;
	}
	
	::CloseHandle(hFile);

	// Save minimal tasklist
	CTaskFile task;
	HTASKITEM hTask = task.NewTask(pTDI->sTitle, NULL, dwTaskID, 0);
	
	if (!hTask)
	{
		ASSERT(0);
		return FALSE;
	}
	
	task.SetTaskAttributes(hTask, *pTDI);
	task.SetCheckedOutTo(GetSourceControlID());
	task.SetXmlHeader(m_sXmlHeader);
	task.SetFileFormat(FILEFORMAT_CURRENT);

	if (!task.Save(sTaskPath, SFEF_UTF16))
		return FALSE;
	
	m_data.SetTaskLocked(dwTaskID, FALSE);
	m_mapCheckedOutIDs.Add(dwTaskID);
	
	return TRUE;
}

BOOL CToDoCtrl::CheckInTask(DWORD dwTaskID)
{
	ASSERT (m_mapCheckedOutIDs.Has(dwTaskID));
	
	TODOITEM tdi;
	
	if (!LoadCheckedOutTask(dwTaskID, tdi))
	{
		m_mapCheckedOutTasks.Remove(dwTaskID);
		// TODO
	
		return FALSE;
	}
	
	if (!IsCheckedOut() && !CheckOut())
	{
		// TODO
		return FALSE;
	}
	
	m_data.SetTaskAttributes(dwTaskID, tdi);
	
	if (!CheckIn())
	{
		// TODO
		return FALSE;
	}
	
	m_data.SetTaskLocked(dwTaskID, TRUE);
	m_mapCheckedOutIDs.Remove(dwTaskID);
	
	VERIFY(FileMisc::DeleteFile(sTaskPath));
	
	return TRUE;
}

BOOL CToDoCtrl::LoadCheckedOutTask(DWORD dwTaskID, TODOITEM& tdi) const
{
	CString sTaskPath = GetTaskSourceControlPath(dwTaskID);
	
	return LoadCheckedOutTask(sTaskPath, dwTaskID, tdi);
}

BOOL CToDoCtrl::LoadCheckedOutTask(const CString& sPath, DWORD& dwTaskID, TODOITEM& tdi) const
{
	CTaskFile task;
	
	if (!task.Load(sPath))
		return FALSE;
	
	if (!task.IsCheckedOutTo(GetSourceControlID())) 
		return FALSE;
	
	if (!task.GetTaskAttributes(task.GetFirstTask(), tdi))
		return FALSE;
	
	dwTaskID = _ttol(FileMisc::GetFilenameFromPath(sPath);
	tdi.bLocked = FALSE;
	
	return TRUE;
}

BOOL CToDoCtrl::CheckInAllTasks()
{
	if (m_mapCheckedOutTasks.IsEmpty())
		return TRUE;
	
	BOOL bWasCheckedOut = IsCheckedOut();
	
	if (!WasCheckedOut)
	{
		if (!SaveAllCheckedOutTasks()) // because check-out will overwrite changes
		{
			// TODO
			return FALSE;
		}
	}
	else
	{
		m_bModified = FALSE; // else Check-out will fail
		
		if (!CheckOut())
		{
			// TODO
			return FALSE;
		}
	}
	
	POSITION pos = m_mapCheckedOutTasks.GetStartPosition();
	TODOITEM tdi;
	
	while (pos)
	{
		DWORD dwTaskID = m_mapCheckedOutTasks.GetNext(pos);
	
		if (!bWasCheckedOut)
		{
			if (LoadCheckedOutTask(dwTaskID, tdi))
			{
				tdi.bLock = TRUE;
				m_data.SetTaskAttributes(dwTaskID, tdi);
			
				VERIFY(FileMisc::DeleteFile(sTaskPath));
			}
		}
		else
		{
			m_data.SetTaskLock(dwTaskID, TRUE);
		}
		
		m_mapCheckedOutIDs.Remove(dwTaskID);
	}
		
	if (!bWasCheckedOut && !CheckIn())
	{
		// TODO
		return FALSE;
	}
	
	return TRUE;
}
```

#### Merging Tasks ####
```
int CToDoCtrl::ReloadCheckedOutTasks()
{
	CStringArray aTaskFiles;
	int nFile = FileMisc::FindFiles(GetTaskSourceControlFolder(), aTaskFiles, FALSE, _T("*.tsc"));

	TODOITEM tdi;
	DWORD dwTaskID;
	int nLoaded = 0;
	
	while (nFile--)
	{
		if (LoadCheckedOutTask(aTaskFiles[nFile], dwTaskID, tdi))
		{
			m_data.SetTaskAttributes([taskID], tdi);
			nLoaded++;
		}
	}
	
	return nLoaded;
}

```

