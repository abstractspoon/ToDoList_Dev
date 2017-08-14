// ToDoCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ToDoCtrl.h"
#include "todoitem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////

CToDoCtrl::CToDoCtrl(/* ... */)
{
}

CToDoCtrl::~CToDoCtrl()
{
}

BEGIN_MESSAGE_MAP(CToDoCtrl, CRuntimeDlg)
//{{AFX_MSG_MAP(CToDoCtrl)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_TDCN_COLUMNEDITCLICK, OnColumnEditClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToDoCtrl message handlers

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

void CToDoCtrl::EnableDisableControls(HTREEITEM hti)
{
	DWORD dwTaskID = GetTaskID(hti);
	
	BOOL bMaximized = (m_nMaxState != TDCMS_NORMAL);
	BOOL bEnable = (hti && !bMaximized);
	BOOL bIsParent = TSH().ItemsAreAllParents();
	BOOL bReadOnly = IsReadOnly();
	BOOL bReadOnlyCtrls = (bReadOnly || !m_taskTree.SelectionHasUnlocked());
	BOOL bEditTime = (!bIsParent || HasStyle(TDCS_ALLOWPARENTTIMETRACKING));

	// now enable/disable appropriate controls
	int nCtrl;
	for (nCtrl = 0; nCtrl < NUM_CTRLITEMS; nCtrl++)
	{
		const CTRLITEM& ctrl = CTRLITEMS[nCtrl];
		EnableDisableControl(ctrl, dwTaskID, bEnable, bReadOnlyCtrls, bIsParent, bEditTime);
	}

	// and custom controls
	for (nCtrl = 0; nCtrl < m_aCustomControls.GetSize(); nCtrl++)
	{
		const CUSTOMATTRIBCTRLITEM& ctrl = m_aCustomControls[nCtrl];
		EnableDisableCustomControl(ctrl, dwTaskID, bEnable, bReadOnlyCtrls);
	}

	// comments
	CString sCommentsType;
	GetSelectedTaskCustomComments(sCommentsType);
	BOOL bEditComments = (m_mgrContent.FindContent(sCommentsType) != -1);
	
	BOOL bCommentsVis = IsCommentsVisible();
	RT_CTRLSTATE nCommentsState = RTCS_ENABLED, nComboState = RTCS_ENABLED;
	
	if (!bCommentsVis || !hti)
	{
		nComboState = nCommentsState = RTCS_DISABLED;
	}
	else if (bReadOnlyCtrls)
	{
		nComboState = nCommentsState = RTCS_READONLY;
	}
	else if (!bEditComments)
	{
		nCommentsState = RTCS_READONLY;
	}

	m_ctrlComments.SetCtrlStates(nComboState, nCommentsState);

	// project name
	BOOL bShowProjectName = (!bMaximized && HasStyle(TDCS_SHOWPROJECTNAME));
	RT_CTRLSTATE nCtrlState =  (!bShowProjectName ? RTCS_DISABLED : 
								(bReadOnly ? RTCS_READONLY : RTCS_ENABLED));
	SetCtrlState(this, IDC_PROJECTNAME, nCtrlState);

	RT_CTRLSTATE nLabelState = (CThemed::IsAppThemed() ? RTCS_ENABLED : RTCS_DISABLED);
	SetCtrlState(this, IDC_PROJECTLABEL, nCtrlState);
}

void CToDoCtrl::UpdateControls(BOOL bIncComments, HTREEITEM hti)
{
	if (m_bDeletingTasks)
		return;

	if (!hti)
		hti = GetUpdateControlsItem();
	
	BOOL bReadOnly = (IsReadOnly() || !m_taskTree.SelectionHasUnlocked());
	CString sCommentsType(m_cfDefault);
	int nSelCount = GetSelectedCount();
	
	if (hti)
	{
		DWORD dwTaskID = GetTrueTaskID(hti); 

		BOOL bMaximize = (m_nMaxState != TDCMS_NORMAL);
		BOOL bEnable = (hti && !bMaximize);
		BOOL bIsParent = TSH().ItemsAreAllParents();
		BOOL bAveSubTaskCompletion = HasStyle(TDCS_AVERAGEPERCENTSUBCOMPLETION) && bIsParent;
		BOOL bEditTime = !bIsParent || HasStyle(TDCS_ALLOWPARENTTIMETRACKING);
		BOOL bEditPercent = !HasStyle(TDCS_AUTOCALCPERCENTDONE) && (nSelCount > 1 || !bAveSubTaskCompletion);

		m_nPriority = GetSelectedTaskPriority();
		m_nRisk = GetSelectedTaskRisk();
		m_sAllocBy = GetSelectedTaskAllocBy();
		m_sStatus = GetSelectedTaskStatus();
		m_sExternalID = GetSelectedTaskExtID();
		m_sVersion = GetSelectedTaskVersion();
		m_crColour = GetSelectedTaskColor();
		m_dCost = GetSelectedTaskCost();

		if (m_crColour == 0)
			m_crColour = CLR_DEFAULT;

		if (bIncComments)
		{
			m_sTextComments = GetSelectedTaskComments();
			m_customComments = GetSelectedTaskCustomComments(sCommentsType);
		}
		
		CStringArray aMatched, aMixed;
		
		m_taskTree.GetSelectedTaskAllocTo(aMatched, aMixed);
		m_cbAllocTo.SetChecked(aMatched, aMixed);

		m_taskTree.GetSelectedTaskCategories(aMatched, aMixed);
		m_cbCategory.SetChecked(aMatched, aMixed);

		m_taskTree.GetSelectedTaskTags(aMatched, aMixed);
		m_cbTags.SetChecked(aMatched, aMixed);

		// special cases
		GetSelectedTaskFileRefs(m_aFileRefs, FALSE); // relative paths

		CStringArray aDepends;
		GetSelectedTaskDependencies(aDepends);
		m_sDepends = Misc::FormatArray(aDepends);

		if (bEditTime)
		{
			m_dTimeEstimate = GetSelectedTaskTimeEstimate(m_nTimeEstUnits);
			m_dTimeSpent = GetSelectedTaskTimeSpent(m_nTimeSpentUnits);
		}
		else
		{
			m_nTimeEstUnits = m_tdiDefault.nTimeEstUnits;
			m_dTimeEstimate = m_data.CalcTaskTimeEstimate(dwTaskID, m_nTimeEstUnits);

			m_nTimeSpentUnits = m_tdiDefault.nTimeSpentUnits;
			m_dTimeSpent = m_data.CalcTaskTimeSpent(dwTaskID, m_nTimeEstUnits);
		}

		// chess clock for time spent
		BOOL bCanTimeTrack = (bEditTime && 
								bEnable && 
								!bReadOnly && 
								(nSelCount == 1) && 
								m_data.IsTaskTimeTrackable(dwTaskID));

		m_eTimeSpent.CheckButton(ID_TIME_TRACK, (dwTaskID == m_dwTimeTrackTaskID));
		m_eTimeSpent.EnableButton(ID_TIME_TRACK, bCanTimeTrack);

		// dependency link button
		m_eDependency.EnableButton(ID_DEPENDS_LINK, bEnable && !m_sDepends.IsEmpty());

		// percent done
		if (IsSelectedTaskDone())
			m_nPercentDone = 100;
		
		else if (bEditPercent)
			m_nPercentDone = GetSelectedTaskPercent();
		else
			m_nPercentDone = m_data.CalcTaskPercentDone(dwTaskID);		
		
		// recurrence
		GetSelectedTaskRecurrence(m_tRecurrence);

		// custom attributes
		GetSelectedTaskCustomAttributeData(m_mapCustomCtrlData, FALSE);
	}
	else // clear controls
	{
		m_sTextComments.Empty();
		m_customComments.Empty();
		m_nPriority = 0;
		m_nRisk = 0;
		m_sAllocBy.Empty();
		m_sStatus.Empty();
		m_sExternalID.Empty();
		m_sDepends.Empty();
		m_nPercentDone = 0;
		m_dTimeEstimate = m_dTimeSpent = 0;
		m_dCost = 0;
		m_tRecurrence = TDCRECURRENCE();
		m_sVersion.Empty();
		m_crColour = CLR_DEFAULT;

		m_cbAllocTo.CheckAll(CCBC_UNCHECKED);
		m_cbCategory.CheckAll(CCBC_UNCHECKED);
		m_cbTags.CheckAll(CCBC_UNCHECKED);

		m_aFileRefs.RemoveAll();

		m_eTimeSpent.EnableButton(ID_TIME_TRACK, FALSE);
		m_eDependency.EnableButton(ID_DEPENDS_LINK, FALSE);

		m_mapCustomCtrlData.RemoveAll();
	}

	UpdateDateTimeControls(hti != NULL);

	// update data controls excluding comments
	UpdateData(FALSE);

	if (bIncComments)
	{
		// if more than one comments type is selected then sCommentsType
		// will be empty which will put the comments type combo in an
		// indeterminate state which is the desired effect since this requires
		// the user to reset the type before they can edit
		m_cfComments = sCommentsType;
		m_ctrlComments.SetSelectedFormat(m_cfComments);
		
		UpdateComments(FALSE);
	}

	// and task header
	UpdateSelectedTaskPath();
	
	// show hide controls
	EnableDisableControls(hti);
	
	m_treeDragDrop.EnableDragDrop(!bReadOnly);
}


void CToDoCtrl::UpdateTask(TDC_ATTRIBUTE nAttrib, DWORD dwFlags)
{
	if (!m_taskTree.GetSafeHwnd())
		return;
	
	HTREEITEM hti = GetSelectedItem();
	
	if (!hti)
		return;
	
	// special case to circumvent CSaveFocus else it can mess up IME input
	if (nAttrib == TDCA_COMMENTS)
	{
		m_nCommentsState = CS_PENDING;
		SetModified(TRUE, TDCA_COMMENTS, GetTaskID(hti));
		return;
	}	
	
	// else
	CSaveFocus sf;
	UpdateData();
	
	switch (nAttrib)
	{
	case TDCA_DONEDATE:
		{
			COleDateTime date;
			m_dtcDone.GetTime(date);
			
			if (SetSelectedTaskDate(TDCD_DONE, date, TRUE))
			{
				// check if we need to modify percent done also
				if (!IsSelectedTaskDone())
				{
					int nPercentDone = GetSelectedTaskPercent();
					
					if (nPercentDone == 100)
						nPercentDone = 0;
					
					SetSelectedTaskPercentDone(nPercentDone);
					m_nPercentDone = nPercentDone;
					
					UpdateData(FALSE);
				}
				else if (m_nPercentDone != 100) // make the percent field look right
				{
					m_nPercentDone = 100;
					UpdateData(FALSE);
				}
			}
			else
			{
				UpdateControls(FALSE); // don't update comments
			}

			// enable done time field depending on whether the done date is valid or not
			m_cbTimeDone.EnableWindow(CDateHelper::IsDateSet(date));
		}
		break;
		
	case TDCA_DONETIME:
		SetSelectedTaskDate(TDCD_DONETIME, m_cbTimeDone.GetOleTime(), TRUE);
		break;
		
	case TDCA_STARTDATE:
		{
			COleDateTime date;
			m_dtcStart.GetTime(date);
			
			SetSelectedTaskDate(TDCD_STARTDATE, date, TRUE);

			// enable start time field depending on whether the start date is valid or not
			m_cbTimeStart.EnableWindow(CDateHelper::IsDateSet(date));
		}
		break;
		
	case TDCA_STARTTIME:
		SetSelectedTaskDate(TDCD_STARTTIME, m_cbTimeStart.GetOleTime(), TRUE);
		break;
		
	case TDCA_DUEDATE:
		{
			COleDateTime date;
			m_dtcDue.GetTime(date);
			
			SetSelectedTaskDate(TDCD_DUEDATE, date, TRUE);

			// enable due time field depending on whether the due date is valid or not
			m_cbTimeDue.EnableWindow(CDateHelper::IsDateSet(date));
		}
		break;
		
	case TDCA_DUETIME:
		SetSelectedTaskDate(TDCD_DUETIME, m_cbTimeDue.GetOleTime(), TRUE);
		break;
		
	case TDCA_COST:
		SetSelectedTaskCost(m_dCost);
		break;
		
	case TDCA_RECURRENCE:
		SetSelectedTaskRecurrence(m_tRecurrence);
		break;
		
	case TDCA_DEPENDENCY:
		{
			CStringArray aDepends;

			Misc::Split(m_sDepends, aDepends);
			SetSelectedTaskDependencies(aDepends);

			m_eDependency.EnableButton(ID_DEPENDS_LINK, !m_sDepends.IsEmpty());
		}
		break;
		
	case TDCA_PRIORITY:
		SetSelectedTaskPriority(m_nPriority);
		break;
		
	case TDCA_RISK:
		SetSelectedTaskRisk(m_nRisk);
		break;
		
	case TDCA_COLOR:
		SetSelectedTaskColor(m_crColour);
		break;
		
	case TDCA_EXTERNALID:
		SetSelectedTaskExtID(m_sExternalID);
		break;
		
	case TDCA_ALLOCTO:
		SetSelectedTaskArray(TDCA_ALLOCTO, m_cbAllocTo);
		break;
		
	case TDCA_ALLOCBY:
		SetSelectedTaskAllocBy(m_sAllocBy);
		break;
		
	case TDCA_STATUS:
		SetSelectedTaskStatus(m_sStatus);
		break;
		
	case TDCA_VERSION:
		SetSelectedTaskVersion(m_sVersion);
		break;
		
	case TDCA_CATEGORY:
		SetSelectedTaskArray(TDCA_CATEGORY, m_cbCategory);
		break;
		
	case TDCA_TAGS:
		SetSelectedTaskArray(TDCA_TAGS, m_cbTags);
		break;
		
	case TDCA_PERCENT:
		{
			// note: we need to take account of 'done' state too because
			// we maintain the task percent at its pre-done state even
			// if the UI says its '100%'
			BOOL bWasDone = IsSelectedTaskDone();
			SetSelectedTaskPercentDone(m_nPercentDone);
			
			// check if we need to update 'done' state
			BOOL bDoneChange = (bWasDone && m_nPercentDone < 100) || (!bWasDone && m_nPercentDone == 100);
			
			if (bDoneChange)
				SetSelectedTaskDone(m_nPercentDone == 100);
		}
		break;
		
	case TDCA_TIMEEST:
		if (dwFlags & UTF_TIMEUNITSONLY)
			SetSelectedTaskTimeEstimateUnits(m_nTimeEstUnits, Misc::HasFlag(dwFlags, UTF_RECALCTIME));
		else
			SetSelectedTaskTimeEstimate(m_dTimeEstimate, m_nTimeEstUnits);
		break;
		
	case TDCA_TIMESPENT:
		if (dwFlags & UTF_TIMEUNITSONLY)
			SetSelectedTaskTimeSpentUnits(m_nTimeSpentUnits, Misc::HasFlag(dwFlags, UTF_RECALCTIME));
		else
			SetSelectedTaskTimeSpent(m_dTimeSpent, m_nTimeSpentUnits);
		break;
		
	case TDCA_FILEREF:
		if (!m_cbFileRef.GetDroppedState())
		{
			BOOL bAppend = (GetSelectedCount() > 1);
			SetSelectedTaskFileRefs(m_aFileRefs, bAppend, TRUE);
		}
		break;
		
	default:
		// handle custom attributes
		if (CTDCCustomAttributeHelper::IsCustomAttribute(nAttrib))
		{
			CString sAttribID = CTDCCustomAttributeHelper::GetAttributeTypeID(nAttrib, m_aCustomAttribDefs);
			TDCCADATA data;

			if (m_mapCustomCtrlData.Lookup(sAttribID, data))
				SetSelectedTaskCustomAttributeData(sAttribID, data, TRUE);
			else
				ClearSelectedTaskCustomAttributeData(sAttribID, TRUE);
		}
	}
}

BOOL CToDoCtrl::SetSelectedTaskLock(BOOL bLocked)
{
	// Can't use 'CanEditSelectedTask' because that
	// will prevent locked tasks being unlocked
	if (IsReadOnly() || !GetSelectedCount())
		return FALSE;

	Flush();
	
	POSITION pos = TSH().GetFirstItemPos();
	TDC_SET nRes = SET_NOCHANGE;
	DWORD dwModTaskID = 0;
	
	IMPLEMENT_UNDO_EDIT(m_data);
		
	while (pos)
	{
		DWORD dwTaskID = TSH().GetNextItemData(pos);
		TDC_SET nItemRes = m_data.SetTaskLock(dwTaskID, bLocked);
		
		if (nItemRes == SET_CHANGE)
		{
			nRes = SET_CHANGE;
			dwModTaskID = dwTaskID;
		}
	}
	
	if (nRes == SET_CHANGE)
	{
		SetModified(TRUE, TDCA_LOCK, dwModTaskID);
	
		if (IsColumnShowing(TDCC_LOCK))
		{
			if (TSH().GetCount() > 1)
				m_taskTree.RedrawColumns();
			else
				m_taskTree.InvalidateTask(dwModTaskID);
		}
	}
	
	return (nRes != SET_FAILED);
}


void CToDoCtrl::SetModified(BOOL bMod)
{
	if (!IsReadOnly())
	{
		m_bModified = bMod;

		if (bMod)
		{
			m_dtLastTaskMod = COleDateTime::GetCurrentTime();
		}
		else if (m_nCommentsState == CS_PENDING)
		{
			m_nCommentsState = CS_CHANGED;
		}
	}
}

BOOL CToDoCtrl::IsModified() const 
{ 
	if (IsReadOnly())
		return FALSE;

	if (m_bModified)
		return TRUE;
	
	// else do we have outstanding unhandled comments
	return (m_nCommentsState == CS_PENDING);
}

// internal version
void CToDoCtrl::SetModified(BOOL bMod, TDC_ATTRIBUTE nAttrib, DWORD /*dwModTaskID*/)
{
	if (IsReadOnly())
		return;
	
	SetModified(bMod);
	
	if (bMod)
	{
		m_taskTree.SetModified(nAttrib);

		if (ModNeedsResort(nAttrib))
		{
			// if the mod was a task completion and the parent completed state 
			// is based on this then we need to resort the entire tree 
			// likewise for start dates and due dates
			if ((nAttrib == TDCA_DONEDATE && HasStyle(TDCS_TREATSUBCOMPLETEDASDONE)) ||
				(nAttrib == TDCA_DUEDATE && (HasStyle(TDCS_USEEARLIESTDUEDATE) || HasStyle(TDCS_USELATESTDUEDATE))) ||
				(nAttrib == TDCA_STARTDATE && (HasStyle(TDCS_USEEARLIESTSTARTDATE) || HasStyle(TDCS_USELATESTSTARTDATE))))
			{
				Resort();
			}
			else // attributes that only have a local effect
			{
				ResortSelectedTaskParents();
			}
		}

		GetParent()->SendMessage(WM_TDCN_MODIFY, (WPARAM)GetSafeHwnd(), (LPARAM)nAttrib);

		// special cases: 
		switch (nAttrib)
		{
		case TDCA_PROJNAME:
			// if this was the project name being edited make sure
			// the focus is set back to the name
			GetDlgItem(IDC_PROJECTNAME)->SetFocus();
			break;

		case TDCA_LOCK:
			UpdateControls(FALSE);
			break;
		}
	}
}

LRESULT CToDoCtrl::OnColumnEditClick(WPARAM wParam, LPARAM lParam)
{
	TDC_COLUMN nColID = (TDC_COLUMN)wParam;
	DWORD dwTaskID = lParam;

	switch (nColID)
	{
	case TDCC_CLIENT:
		ASSERT(!IsReadOnly());
		EditSelectedTask(FALSE);
		break;
		
	case TDCC_DONE:
		ASSERT(!IsReadOnly());
		SetSelectedTaskDone(!m_data.IsTaskDone(dwTaskID));
		break;
		
	case TDCC_TRACKTIME:
		{
			HTREEITEM hti = m_taskTree.GetTreeSelectedItem();

			ASSERT (!IsReadOnly() && 
					(GetSelectedCount() == 1) && 
					IsItemSelected(hti) && 
					m_data.IsTaskTimeTrackable(dwTaskID));
			
			ToggleTimeTracking(hti);
		}
		break;
		
	case TDCC_FLAG:
		ASSERT(!IsReadOnly());
		SetSelectedTaskFlag(!m_data.IsTaskFlagged(dwTaskID));
		break;
		
	case TDCC_LOCK:
		ASSERT(!IsReadOnly());
		SetSelectedTaskLock(!m_data.IsTaskLocked(dwTaskID));
		break;

	case TDCC_ICON:
		ASSERT(!IsReadOnly());

		// Cancel any drag started by clicking on the tree item icon
		m_treeDragDrop.CancelDrag();

		EditSelectedTaskIcon();
		break;
		
	case TDCC_FILEREF:
		GotoFile(m_data.GetTaskFileRef(dwTaskID, 0));
		break;
		
	case TDCC_DEPENDENCY:
		{
			CDWordArray aLocalDepends;
			CStringArray aOtherDepends;

			if (GetAllSelectedTaskDependencies(aLocalDepends, aOtherDepends))
			{
				if (aLocalDepends.GetSize() == 0)
				{
					if (aOtherDepends.GetSize() > 0)
						ShowTaskLink(aOtherDepends[0]);
				}
				else
				{
					SelectTasks(aLocalDepends, TRUE);
				}
			}
		}
		break;
		
	case TDCC_RECURRENCE:
		ASSERT(!IsReadOnly());
		m_eRecurrence.DoEdit();
		break;
		
	case TDCC_REMINDER:
		AfxGetMainWnd()->SendMessage(WM_TDCN_CLICKREMINDERCOL);
		break;
		
	default: // try custom columns
		HandleCustomColumnClick(nColID);
		break;
	}

	m_treeDragDrop.EnableDragDrop(!IsReadOnly() && GetSelectedCount());

	return 0L;
}

TDC_FILE CToDoCtrl::CheckIn()
{
	ASSERT(m_bSourceControlled && m_bCheckedOut);

	Flush(TRUE);
	
	if (!m_bSourceControlled)
	{
		return TDCF_SSC_NOTSRCCONTROLLED;
	}
	else if (!m_bCheckedOut)
	{
		return TDCF_SSC_NOTCHECKEDOUT;
	}
	else if (m_sLastSavePath.IsEmpty()) // not yet saved
	{
		m_bCheckedOut = FALSE;
		return TDCF_SUCCESS;
	}
	
	// backup the file
	CTempFileBackup backup(m_sLastSavePath);
	
	// scope the task file so it gets closed
	// before we try to restore the backup
	int nFileErr = 0;
	{
		CWaitCursor cursor;
		CTaskFile file(m_sPassword);
		
		// snap shot mod time so we can restore it
		FILETIME ftMod = { 0 };
		VERIFY(FileMisc::GetFileLastModified(m_sLastSavePath, ftMod));
		
		// change check-out state before resaving
		m_bCheckedOut = FALSE;
		
		// resave
		BuildTasksForSave(file, FALSE);
		
		if (file.Save(m_sLastSavePath, SFEF_UTF16))
		{
			FileMisc::SetFileLastModified(m_sLastSavePath, ftMod);
			return TDCF_SUCCESS;
		}
		
		// else
		nFileErr = file.GetLastFileError();
	}
	
	// always restore backup on failure
	VERIFY(backup.RestoreBackup());
	
	// else someone else or invalid file
	return MapTaskfileError(nFileErr);
}

TDC_FILE CToDoCtrl::CheckOut()
{
	CString sTemp;
	return CheckOut(sTemp);
}

BOOL CToDoCtrl::IsCheckedOut() const 
{ 
	return (IsSourceControlled() && m_bCheckedOut); 
}

BOOL CToDoCtrl::IsSourceControlled() const 
{ 
	return m_bSourceControlled; 
}

BOOL CToDoCtrl::AddToSourceControl(BOOL bAdd)
{
	if ((bAdd && m_bSourceControlled) || (!bAdd && !m_bSourceControlled))
		return TRUE;
	
	if (!bAdd && !m_bCheckedOut)
		return FALSE;
	
	// update source control
	m_bSourceControlled = m_bCheckedOut = bAdd;
				
	// resave
	if (!m_sLastSavePath.IsEmpty())
	{
		CTaskFile file;
		BuildTasksForSave(file, FALSE);
		
		if (file.Save(m_sLastSavePath, SFEF_UTF16))
			return TRUE;
	}
	else // not yet saved
	{
		return TRUE;
	}
	
	// else restore previous state
	m_bSourceControlled = m_bCheckedOut = bAdd;
	
	return FALSE;
}

TDC_FILE CToDoCtrl::CheckOut(CString& sCheckedOutTo)
{
	ASSERT(m_bSourceControlled);
	
	if (!m_bSourceControlled)
		return TDCF_SSC_NOTSRCCONTROLLED;
	
	if (m_bDelayLoaded)
		return TDCF_SSC_DELAYLOADED;
	
	if (m_bCheckedOut)
	{
		// caller must think we're not checked out
		ASSERT(0);
		
		sCheckedOutTo = GetSourceControlID();
		return TDCF_SUCCESS;
	}
	
	// backup the file
	CTempFileBackup backup(m_sLastSavePath);
	CWaitCursor cursor;
	
	// scope the task file so it gets closed
	// before we try to restore the backup
	int nFileErr = 0;
	{
		CTaskFile file(m_sPassword);
		
		if (file.Open(m_sLastSavePath, XF_READ, FALSE) && file.LoadEx())
		{
			file.Close();
			
			sCheckedOutTo = file.GetCheckOutTo();
			
			if (sCheckedOutTo.IsEmpty())
			{
				// load tasks
				file.Decrypt();
				LoadTasks(file); // load file
				
				// update source control
				m_bCheckedOut = TRUE;
				
				// resave
				BuildTasksForSave(file, FALSE);
				
				if (file.Save(m_sLastSavePath, SFEF_UTF16))
				{
					// update modified time
					m_dtLastTaskMod = COleDateTime::GetCurrentTime();
					return TDCF_SUCCESS;
				}
				
				// restore state
				m_bCheckedOut = FALSE;
			}
		}
		
		nFileErr = file.GetLastFileError();
	}
	
	// always restore backup on failure
	VERIFY(backup.RestoreBackup());
	
	return MapTaskfileError(nFileErr);
}

BOOL CToDoCtrl::CanEditSelectedTask() const 
{ 
	return (!IsReadOnly() && GetSelectedCount() &&
			m_taskTree.SelectionHasUnlocked()); 
}

BOOL CToDoCtrl::CanEditSelectedTaskLock() const
{
	return (!IsReadOnly() && GetSelectedCount());
}

