// OdbcStruct.cpp : implementation file
//

#include "stdafx.h"
#include "OdbcStruct.h"

#include "..\Shared\enstring.h"
#include "..\Shared\misc.h"
#include "..\Shared\Timehelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const LPCTSTR ODBC_METADATAKEY = _T("ODBC_1A49027C_CC71_4EB8_8540_B319AB9A045E");

/////////////////////////////////////////////////////////////////////////////

COdbcTaskArray::~COdbcTaskArray()
{
	RemoveAll();
}

void COdbcTaskArray::RemoveAll()
{
	int i = GetSize();
	
	while (i--)
		delete GetAt(i);
	
	CArray<ODBCTASK*, ODBCTASK*&>::RemoveAll();
}

void COdbcTaskArray::Sort()
{
	if (GetSize() > 1)
		qsort(GetData(), GetSize(), sizeof(ODBCTASK*), TaskSortProc);
}

int COdbcTaskArray::TaskSortProc(const void* pV1, const void* pV2)
{
	const ODBCTASK* pTask1 = *((const ODBCTASK**)pV1);
	const ODBCTASK* pTask2 = *((const ODBCTASK**)pV2);
	
	// if one is the parent of the other then
	// the parent must precede the child
	if (pTask1->sTaskKey == pTask2->sParentKey)
		return -1;
	
	if (pTask1->sParentKey == pTask2->sTaskKey)
		return 1;
	
	// all else
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

COdbcMapKeyToTask::~COdbcMapKeyToTask()
{
	POSITION pos = GetStartPosition();

	while (pos)
	{
		CString sKey;
		ODBCTASK* pTask = NULL;

		GetNextAssoc(pos, sKey, pTask);
		ASSERT(pTask);

		delete pTask;
	}

	RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////

ODBCTASK::ODBCTASK() : dwAttribs(0), dwID(0), dwParentID(0)
{

}

void ODBCTASK::SetAttrib(CString& sVar, LPCTSTR szVal, DWORD dwAttrib) 
{ 
	sVar = szVal; 
	SetHasAttribute(dwAttrib, !sVar.IsEmpty()); 
}

void ODBCTASK::SetAttrib(long& nVar, long nVal, DWORD dwAttrib, long nNullVal)
{
	nVar = nVal; 
	SetHasAttribute(dwAttrib, (nVar != nNullVal)); 
}

void ODBCTASK::SetAttrib(double& dVar, double dVal, DWORD dwAttrib, double dNullVal)
{
	dVar = dVal; 
	SetHasAttribute(dwAttrib, (dVar != dNullVal)); 
}

CString ODBCTASK::GetMetaDataKey(LPCTSTR szSubKey)
{
	static CString sKey;

	if (szSubKey && *szSubKey)
		sKey.Format(_T("%s_%s"), ODBC_METADATAKEY, szSubKey);
	else
		sKey = ODBC_METADATAKEY;

	return sKey;
}

BOOL ODBCTASK::HasAttribute(DWORD dwAttrib) const
{
	return Misc::HasFlag(dwAttribs, dwAttrib);
}

void ODBCTASK::SetHasAttribute(DWORD dwAttrib, BOOL bSet)
{
	if (bSet)
		dwAttribs |= dwAttrib;
	else
		dwAttribs &= ~dwAttrib;
}

BOOL ODBCTASK::Save(ITaskList12* pTasks, HTASKITEM hTask, const COdbcMapKeyToID& mapTasks) const
{
	ASSERT(pTasks && hTask);

	if (!pTasks || !hTask)
		return FALSE;
	
	pTasks->SetTaskTitle(hTask, sTitle);

	if (HasAttribute(OT_ALLOCBY))
		pTasks->SetTaskAllocatedBy(hTask, sAllocBy);

	if (HasAttribute(OT_ALLOCTO))
	{
		int nItem = aAllocTo.GetSize();

		while (nItem--)
			pTasks->AddTaskAllocatedTo(hTask, aAllocTo[nItem]);
	}

	if (HasAttribute(OT_CATEGORY))
	{
		int nItem = aCategories.GetSize();

		while (nItem--)
			pTasks->AddTaskCategory(hTask, aCategories[nItem]);
	}

	if (HasAttribute(OT_COLOR))
		pTasks->SetTaskColor(hTask, color);

	if (HasAttribute(OT_COMMENTS))
		pTasks->SetTaskComments(hTask, sComments);

	if (HasAttribute(OT_COST))
		pTasks->SetTaskCost(hTask, dCost);

	if (HasAttribute(OT_CREATEDBY))
		pTasks->SetTaskCreatedBy(hTask, sCreatedBy);

	if (HasAttribute(OT_CREATIONDATE))
		pTasks->SetTaskCreationDate64(hTask, tCreation);

	if (HasAttribute(OT_DEPENDENCY))
	{
		int nItem = aDepends.GetSize();

		while (nItem--)
		{
			DWORD dwTaskID = 0;
			CString sDepends = aDepends[nItem];

			if (mapTasks.Lookup(sDepends, dwTaskID) && dwTaskID)
				pTasks->AddTaskDependency(hTask, Misc::Format(dwTaskID));
		}
	}

	if (HasAttribute(OT_DONEDATE))
		pTasks->SetTaskDoneDate64(hTask, tDone);

	if (HasAttribute(OT_DUEDATE))
		pTasks->SetTaskDueDate64(hTask, tDue);

	if (HasAttribute(OT_EXTERNALID))
		pTasks->SetTaskExternalID(hTask, sExternalID);

	if (HasAttribute(OT_FILEREF))
		pTasks->SetTaskFileLinkPath(hTask, sFileRef);

	if (HasAttribute(OT_FLAG))
		pTasks->SetTaskFlag(hTask, (bFlag != FALSE));

	if (HasAttribute(OT_ICON))
		pTasks->SetTaskIcon(hTask, sIcon);

	if (HasAttribute(OT_LASTMOD))
		pTasks->SetTaskLastModified64(hTask, tModified);

	if (HasAttribute(OT_PERCENT))
		pTasks->SetTaskPercentDone(hTask, (unsigned char)min(100, max(0, nPercent)));

	if (HasAttribute(OT_PRIORITY))
		pTasks->SetTaskPriority(hTask, nPriority);

// 	if (HasAttribute(OT_RECURRENCE))
// 		pTasks->SetTaskRecurrence(hTask, nRegularity, dwSpecific1, dwSpecific2, bRecalcFromDue, nReuse);

	if (HasAttribute(OT_RISK))
		pTasks->SetTaskRisk(hTask, nRisk);

	if (HasAttribute(OT_STARTDATE))
		pTasks->SetTaskStartDate64(hTask, tStart);

	if (HasAttribute(OT_STATUS))
		pTasks->SetTaskStatus(hTask, sStatus);

	if (HasAttribute(OT_TAGS))
	{
		int nItem = aTags.GetSize();

		while (nItem--)
			pTasks->AddTaskTag(hTask, aTags[nItem]);
	}

	if (HasAttribute(OT_TIMEEST))
		pTasks->SetTaskTimeEstimate(hTask, dTimeEst, TDCU_HOURS);

	if (HasAttribute(OT_TIMESPENT))
		pTasks->SetTaskTimeSpent(hTask, dTimeSpent, TDCU_HOURS);

	if (HasAttribute(OT_VERSION))
		pTasks->SetTaskVersion(hTask, sVersion);

	// write task key in as meta-data
	pTasks->SetTaskMetaData(hTask, GetMetaDataKey(_T("KEY")), sTaskKey);

	return TRUE;
}

BOOL ODBCTASK::Load(const ITaskList12* pTasks, HTASKITEM hTask, COdbcMapIDToKey& mapTasks)
{
	ASSERT(pTasks && hTask);

	if (!pTasks || !hTask)
		return FALSE;

	TDC_UNITS nUnits;
	CTimeHelper th;
	long nItem;
	
	// task key is meta-data
	sTaskKey = pTasks->GetTaskMetaData(hTask, GetMetaDataKey(_T("KEY")));

	dwID = pTasks->GetTaskID(hTask);
	dwParentID = pTasks->GetTaskParentID(hTask);

	// parent key we get from map
	if (dwParentID != 0)
		VERIFY(mapTasks.Lookup(dwParentID, sParentKey));

	SetAttrib(sTitle,		pTasks->GetTaskTitle(hTask),				OT_TITLE);
	SetAttrib(sAllocBy,		pTasks->GetTaskAllocatedBy(hTask),			OT_ALLOCBY);
	SetAttrib(sComments,	pTasks->GetTaskComments(hTask),				OT_COMMENTS);
	SetAttrib(sCreatedBy,	pTasks->GetTaskCreatedBy(hTask),			OT_CREATEDBY);
	SetAttrib(sExternalID,	pTasks->GetTaskExternalID(hTask),			OT_EXTERNALID);
	SetAttrib(sFileRef,		pTasks->GetTaskFileLinkPath(hTask),	OT_FILEREF);
	SetAttrib(sIcon,		pTasks->GetTaskIcon(hTask),					OT_ICON);
	SetAttrib(sStatus,		pTasks->GetTaskStatus(hTask),				OT_STATUS);
	SetAttrib(sVersion,		pTasks->GetTaskVersion(hTask),				OT_VERSION);
	SetAttrib(color,		pTasks->GetTaskColor(hTask),				OT_COLOR);
	SetAttrib(bFlag,		pTasks->IsTaskFlagged(hTask) ? 1L : 0L,		OT_FLAG);
	SetAttrib(nPercent,		pTasks->GetTaskPercentDone(hTask, FALSE),	OT_PERCENT);
	SetAttrib(nPriority,	pTasks->GetTaskPriority(hTask, FALSE),		OT_PRIORITY);
	SetAttrib(nRisk,		pTasks->GetTaskRisk(hTask, FALSE),			OT_RISK);//, -2);
	SetAttrib(dCost,		pTasks->GetTaskCost(hTask, FALSE),			OT_COST);//, -2);

 	SetAttrib(dTimeEst,		pTasks->GetTaskTimeEstimate(hTask, nUnits, FALSE),	OT_TIMEEST);
 	dTimeEst = th.GetTime(dTimeEst, MapUnitsToTHUnits(nUnits), THU_HOURS);

 	SetAttrib(dTimeSpent,	pTasks->GetTaskTimeSpent(hTask, nUnits, FALSE),		OT_TIMESPENT);
 	dTimeSpent = th.GetTime(dTimeSpent, MapUnitsToTHUnits(nUnits), THU_HOURS);
	
	SetHasAttribute(OT_CREATIONDATE,	pTasks->GetTaskCreationDate64(hTask, tCreation));
	SetHasAttribute(OT_DONEDATE,		pTasks->GetTaskDoneDate64(hTask, tDone));
	SetHasAttribute(OT_LASTMOD,			pTasks->GetTaskLastModified64(hTask, tModified));
	SetHasAttribute(OT_STARTDATE,		pTasks->GetTaskStartDate64(hTask, FALSE, tStart));
	SetHasAttribute(OT_DUEDATE,			pTasks->GetTaskDueDate64(hTask, FALSE, tDue));

	// arrays
	SetAttrib(nItem, pTasks->GetTaskAllocatedToCount(hTask), OT_ALLOCTO);
		
	while (nItem--)
		aAllocTo.Add(pTasks->GetTaskAllocatedTo(hTask, nItem));
	
	SetAttrib(nItem, pTasks->GetTaskCategoryCount(hTask), OT_CATEGORY);
	
	while (nItem--)
		aCategories.Add(pTasks->GetTaskCategory(hTask, nItem));
	
	SetAttrib(nItem, pTasks->GetTaskDependencyCount(hTask), OT_DEPENDENCY);
	
	while (nItem--)
		aDepends.Add(pTasks->GetTaskDependency(hTask, nItem));
	
	SetAttrib(nItem, pTasks->GetTaskTagCount(hTask), OT_TAGS);
	
	while (nItem--)
		aTags.Add(pTasks->GetTaskTag(hTask, nItem));

// 	SetHasAttribute(OT_RECURRENCE);
// 	 = pTasks->GetTaskRecurrence(hTask, nRegularity, dwSpecific1, dwSpecific2, bRecalcFromDue, nReuse);

	// map task ID to its key
	mapTasks[dwID] = sTaskKey;

	return TRUE;
}

TH_UNITS ODBCTASK::MapUnitsToTHUnits(TDC_UNITS nUnits)
{
	switch (nUnits)
	{
	case TDCU_MINS:		return THU_MINS;
	case TDCU_HOURS:	return THU_HOURS;
	case TDCU_DAYS:		return THU_DAYS;
	case TDCU_WEEKDAYS:	return THU_WEEKDAYS;
	case TDCU_WEEKS:	return THU_WEEKS;
	case TDCU_MONTHS:	return THU_MONTHS;
	case TDCU_YEARS:	return THU_YEARS;
	}
	
	// all else
	ASSERT(0);
	return THU_NULL;
}

/////////////////////////////////////////////////////////////////////////////

ODBCTABLESETUP::ODBCTABLESETUP()
{

}

BOOL ODBCTABLESETUP::Verify() const
{
	CStringArray aDummy;
	return Verify(_T(""), aDummy);
}

BOOL ODBCTABLESETUP::Verify(const CString& sTableDesc, CStringArray& aErrors) const
{
	int nOrgErrCount = aErrors.GetSize();

	if (sTableName.IsEmpty())
		aErrors.Add(CEnString(IDS_ERR_TABLENAME, sTableDesc));

	if (sKeyField.IsEmpty())
		aErrors.Add(CEnString(IDS_ERR_TABLEKEYFIELD, sTableDesc));

	if (sContentField.IsEmpty())
		aErrors.Add(CEnString(IDS_ERR_TABLENAMEFIELD, sTableDesc));

	// successful is no errors added
	return (aErrors.GetSize() == nOrgErrCount);
}

//////////////////////////////////////////////////////////////////////

ODBCTASKSTABLESETUP::ODBCTASKSTABLESETUP()
{

}

BOOL ODBCTASKSTABLESETUP::Verify(BOOL bVerifyTasklistField) const
{
	CStringArray aDummy;
	return Verify(_T(""), aDummy, bVerifyTasklistField);
}

BOOL ODBCTASKSTABLESETUP::Verify(const CString& sTableDesc, CStringArray& aErrors, BOOL bVerifyTasklistField) const
{
	int nOrgErrCount = aErrors.GetSize();

	// table, task key and parent key are mandatory
	if (sTableName.IsEmpty())
		aErrors.Add(CEnString(IDS_ERR_TABLENAME, sTableDesc));

	if (sTaskKeyField.IsEmpty())
		aErrors.Add(CEnString(IDS_ERR_TABLEKEYFIELD, sTableDesc));

	if (sParentKeyField.IsEmpty())
		aErrors.Add(CEnString(IDS_ERR_TASKTABLEPARENTFIELD, sTableDesc));

	// tasklist key can be optional
	if (bVerifyTasklistField && sTasklistKeyField.IsEmpty())
		aErrors.Add(CEnString(IDS_ERR_TASKTABLETASKLISTFIELD));

	// successful is no errors added
	return (aErrors.GetSize() == nOrgErrCount);
}

//////////////////////////////////////////////////////////////////////

ODBCTABLESSETUP::ODBCTABLESSETUP() 
{

}

BOOL ODBCTABLESSETUP::Verify() const
{
	CStringArray aDummy;
	return Verify(aDummy);
}

BOOL ODBCTABLESSETUP::Verify(CStringArray& aErrors) const
{
	int nOrgErrCount = aErrors.GetSize();

	// if the user has setup a 'Tasklists' table then
	// the content field of the 'Tasks' table must be set
	BOOL bVerifyTasklistID = !tableTasklists.sTableName.IsEmpty();

	tableTasks.Verify(CEnString(IDS_TASKSTABLE), aErrors, bVerifyTasklistID);

	if (bVerifyTasklistID)
		tableTasklists.Verify(CEnString(IDS_TASKLISTSTABLE), aErrors);

	if (!tableAllocTo.sTableName.IsEmpty())
		tableAllocTo.Verify(CEnString(IDS_ALLOCTOTABLE), aErrors);

	// successful is no errors added
	return (aErrors.GetSize() == nOrgErrCount);
}

//////////////////////////////////////////////////////////////////////

ODBCATTRIBUTESETUP::ODBCATTRIBUTESETUP() : nAttrib(TDCA_NONE) 
{
	
}

BOOL ODBCATTRIBUTESETUP::Verify(BOOL bVerifyLinkTasklistKey) const
{
	CStringArray aDummy;
	return Verify(aDummy, bVerifyLinkTasklistKey);
}

BOOL ODBCATTRIBUTESETUP::Verify(CStringArray& aErrors, BOOL bVerifyLinkTasklistKey) const
{
	int nOrgErrCount = aErrors.GetSize();

	// User is entitled not to map an attribute
	if (!sTaskField.IsEmpty())
	{
		// the task field may be a foreign key to another table
		if (!sAttribTable.IsEmpty())
		{
			CString sAttrib = GetTaskAttributeName();
			
			// if so, both attrib fields must be set
			if (sAttribKeyField.IsEmpty())
				aErrors.Add(CEnString(IDS_ERR_ATTRIBKEYFIELD, sAttrib));
			
			if (sAttribContentField.IsEmpty())
				aErrors.Add(CEnString(IDS_ERR_ATTRIBNAMEFIELD, sAttrib));
			
			// for multiple element attributes, the user may
			// also specify a link table
			if (!sLinkTable.IsEmpty())
			{
				// if so, both link fields must be set
				if (sLinkTaskKeyField.IsEmpty())
					aErrors.Add(CEnString(IDS_ERR_LINKTASKFIELD, sAttrib));
				
				if (sLinkAttribKeyField.IsEmpty())
					aErrors.Add(CEnString(IDS_ERR_LINKATTRIBFIELD, sAttrib));

				if (bVerifyLinkTasklistKey && sLinkTaskListKeyField.IsEmpty())
					aErrors.Add(CEnString(IDS_ERR_LINKTASKLISTFIELD, sAttrib));
			}
		}
	}
	
	// successful is no errors added
	return (aErrors.GetSize() == nOrgErrCount);
}

CString ODBCATTRIBUTESETUP::GetTaskAttributeName() const
{
	return GetTaskAttributeName(nAttrib);
}

// static
CString ODBCATTRIBUTESETUP::GetTaskAttributeName(TDC_ATTRIBUTE nAttrib)
{
	for (int nAtt = 0; nAtt < NUM_ATTRIB; nAtt++)
	{
		if (TASKATTRIB_NAMES[nAtt].nAttrib == nAttrib)
			return CEnString(TASKATTRIB_NAMES[nAtt].nIDName);
	}

	// else
	return _T("");
}

//////////////////////////////////////////////////////////////////////

ODBCSETUP::ODBCSETUP() 
{
	
}

ODBCSETUP::ODBCSETUP(const ODBCSETUP& other) 
{ 
	*this = other; 
}

ODBCSETUP& ODBCSETUP::operator=(const ODBCSETUP& other)
{
	tables = other.tables;
	aAttrib.Copy(other.aAttrib);
	
	return *this;
}

BOOL ODBCSETUP::Verify() const
{
	CStringArray aDummy;
	return Verify(aDummy);
}

BOOL ODBCSETUP::Verify(CStringArray& aErrors) const
{
	tables.Verify(aErrors);

	// attribute mapping
	int nNumAttrib = aAttrib.GetSize();

	if (nNumAttrib == 0)
	{
		aErrors.Add(CEnString(IDS_ERR_ONEORMOREATTRIB));
	}
	else
	{
		// if the user has setup a 'Tasklists' table then
		// the link table 'Tasklists' key must also be set
		BOOL bVerifyTasklistID = !tables.tableTasklists.sTableName.IsEmpty();

		for (int nAtt = 0; nAtt < nNumAttrib; nAtt++)
		{
			const ODBCATTRIBUTESETUP& attrib = aAttrib.GetData()[nAtt];
			attrib.Verify(aErrors, bVerifyTasklistID);
		}
	}

	return (aErrors.GetSize() == 0);
}

//////////////////////////////////////////////////////////////////////

ODBCDATABASESETUP::ODBCDATABASESETUP()
{

}

BOOL ODBCDATABASESETUP::Verify() const
{
	if (sDbName.IsEmpty() || sConnect.IsEmpty())
		return FALSE;

	return setup.Verify();
}

BOOL ODBCDATABASESETUP::Verify(CStringArray& aErrors) const
{
	if (sDbName.IsEmpty() || sConnect.IsEmpty())
		return FALSE;

	if (!setup.Verify(aErrors))
		return FALSE;

	// check that a valid tasklist ID has been provided
	if (setup.tables.tableTasklists.Verify() && sTasklistID.IsEmpty())
	{
		aErrors.Add(CEnString(IDS_ERR_NEEDTASKLISTID));
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////

