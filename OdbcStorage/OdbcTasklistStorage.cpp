// OdbcTasklistStorage.cpp: implementation of the COdbcTasklistStorage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "OdbcTasklistStorage.h"
#include "OdbcDatabaseSelectiondlg.h"
#include "OdbcTaskrecordset.h"
#include "OdbcHelper.h"

#include "..\shared\localizer.h"
#include "..\shared\misc.h"
#include "..\shared\databaseex.h"
#include "..\shared\datehelper.h"

#include "..\Interfaces\IPreferences.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////

typedef CMap<CString, LPCTSTR, HTASKITEM, HTASKITEM&> CMapKeyToTask;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COdbcTasklistStorage::COdbcTasklistStorage() : m_hIcon(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_ODBCSTORAGE);
}

COdbcTasklistStorage::~COdbcTasklistStorage()
{

}

void COdbcTasklistStorage::SetLocalizer(ITransText* pTT)
{
	CLocalizer::Initialize(pTT);
}

bool COdbcTasklistStorage::RetrieveTasklist(ITS_TASKLISTINFO* pFInfo, ITaskList* pDestTaskFile, IPreferences* pPrefs, LPCTSTR szKey, bool bSilent)
{
	ITaskList12* pTasks = GetITLInterface<ITaskList12>(pDestTaskFile, IID_TASKLIST12);

	if (!pTasks)
		return false;

	COdbcDatabaseSetupArray aDbSetup;
	LoadDatabaseSetups(pPrefs, szKey, aDbSetup);

	// get the specific db to import
	ODBCDATABASESETUP dbSetup;

	CString sConnect = pFInfo->szTasklistID;
	CString sErrMsg;
	
	if (GetDatabaseSetup(sConnect, aDbSetup, dbSetup))
	{
		if (!LoadFromDatabase(dbSetup, pTasks, sErrMsg))
		{
			AfxMessageBox(sErrMsg);
			return false;
		}
	}
	else if (bSilent)
	{
		return false;
	}
	else // show dialog
	{
		COdbcDatabaseSelectionDlg dialog(aDbSetup);
		int nSelDb = -1;
		
		while (true)
		{
			if (dialog.DoModal() != IDOK) // cancelled
				return false;

			// retrieve and save updated database setup changes
			dialog.GetDatabaseSetups(aDbSetup);
			SaveDatabaseSetups(pPrefs, szKey, aDbSetup);

			// get the selected database
			nSelDb = dialog.GetSelectedDatabaseSetup(dbSetup);
			ASSERT((nSelDb >= 0) && dbSetup.Verify());

			if (LoadFromDatabase(dbSetup, pTasks, sErrMsg))
				break; // success

			// else tell the user and try again
			ASSERT(!sErrMsg.IsEmpty());
			AfxMessageBox(sErrMsg);
		}

		// update any additional constraints on the 
		// selected db before saving
		ASSERT((nSelDb >= 0) && (nSelDb < aDbSetup.GetSize()));

		aDbSetup[nSelDb] = dbSetup;
		SaveDatabaseSetups(pPrefs, szKey, aDbSetup);
	}

	// save connection info into 'pFInfo'
	_tcsncpy(pFInfo->szTasklistID, dbSetup.sConnect, ITS_TASKLISTID_LEN);
	_tcsncpy(pFInfo->szDisplayName, COdbcHelper::FormatName(dbSetup), _MAX_PATH);

	// and save connect string into tasklist 
	pTasks->SetMetaData(ODBCTASK::GetMetaDataKey(_T("CONNECT")), dbSetup.sConnect);

	return true;
}

bool COdbcTasklistStorage::StoreTasklist(ITS_TASKLISTINFO* pFInfo, const ITaskList* pSrcTaskFile, IPreferences* pPrefs, LPCTSTR szKey, bool bSilent)
{
	const ITaskList12* pTasks = GetITLInterface<ITaskList12>(pSrcTaskFile, IID_TASKLIST12);
	
	if (!pTasks)
		return false;
	
	COdbcDatabaseSetupArray aDbSetup;
	LoadDatabaseSetups(pPrefs, szKey, aDbSetup);
	
	// get the specific db to save to
	CString sConnect = pFInfo->szTasklistID;
	CString sErrMsg;

	// sanity check
	CString sMetaDataConnect = pTasks->GetMetaData(ODBCTASK::GetMetaDataKey(_T("CONNECT")));
	ASSERT((sConnect.IsEmpty() && sMetaDataConnect.IsEmpty()) || (sConnect == sMetaDataConnect));
	
	ODBCDATABASESETUP dbSetup;
	
	if (GetDatabaseSetup(sConnect, aDbSetup, dbSetup))
	{
		if (!SaveToDatabase(pTasks, dbSetup, sErrMsg))
		{
			AfxMessageBox(sErrMsg);
			return false;
		}
	}
	else if (bSilent)
	{
		return false;
	}
	else // show dialog
	{
		COdbcDatabaseSelectionDlg dialog(aDbSetup);
		int nSelDb = -1;
		
		while (true)
		{
			if (dialog.DoModal() != IDOK) // cancelled
				return false;
			
			// retrieve and save updated database setup changes
			dialog.GetDatabaseSetups(aDbSetup);
			SaveDatabaseSetups(pPrefs, szKey, aDbSetup);
			
			// get the selected database
			nSelDb = dialog.GetSelectedDatabaseSetup(dbSetup);
			ASSERT((nSelDb >= 0) && dbSetup.Verify());
			
			if (SaveToDatabase(pTasks, dbSetup, sErrMsg))
				break; // success
			
			// else tell the user and try again
			ASSERT(!sErrMsg.IsEmpty());

			if (!sErrMsg.IsEmpty())
				AfxMessageBox(sErrMsg);
		}
		
		// update any additional constraints on the 
		// selected db before saving
		ASSERT((nSelDb >= 0) && (nSelDb < aDbSetup.GetSize()));
		
		aDbSetup[nSelDb] = dbSetup;
		SaveDatabaseSetups(pPrefs, szKey, aDbSetup);
	}

	return true;
}

int COdbcTasklistStorage::LoadDatabaseSetups(IPreferences* pPrefs, LPCTSTR szKey, COdbcDatabaseSetupArray& aDbSetup) const
{
	int nNumDb = pPrefs->GetProfileInt(szKey, _T("NumDatabases"));

	for (int nDb = 0; nDb < nNumDb; nDb++)
	{
		CString sDbKey(szKey);
		sDbKey += _T("\\Database");
		sDbKey += Misc::Format(nDb);

		ODBCDATABASESETUP db;

		db.sDbName = pPrefs->GetProfileString(sDbKey, _T("Name"));
		db.sConnect = pPrefs->GetProfileString(sDbKey, _T("Connect"));
		db.sTasklistID = pPrefs->GetProfileString(sDbKey, _T("TasklistID"));
		db.sAllocToID = pPrefs->GetProfileString(sDbKey, _T("AllocToID"));

		db.setup.tables.tableTasks.sTableName = pPrefs->GetProfileString(sDbKey, _T("TasksTable"));
		db.setup.tables.tableTasks.sTaskKeyField = pPrefs->GetProfileString(sDbKey, _T("TasksKeyField"));
		db.setup.tables.tableTasks.sTasklistKeyField = pPrefs->GetProfileString(sDbKey, _T("TasklistKeyField"));
		db.setup.tables.tableTasks.sParentKeyField = pPrefs->GetProfileString(sDbKey, _T("ParentKeyField"));

		db.setup.tables.tableTasklists.sTableName = pPrefs->GetProfileString(sDbKey, _T("TasklistsTable"));
		db.setup.tables.tableTasklists.sKeyField = pPrefs->GetProfileString(sDbKey, _T("TasklistsKeyField"));
		db.setup.tables.tableTasklists.sContentField = pPrefs->GetProfileString(sDbKey, _T("TasklistsNameField"));

		db.setup.tables.tableAllocTo.sTableName = pPrefs->GetProfileString(sDbKey, _T("AllocToTable"));
		db.setup.tables.tableAllocTo.sKeyField = pPrefs->GetProfileString(sDbKey, _T("AllocToKeyField"));
		db.setup.tables.tableAllocTo.sContentField = pPrefs->GetProfileString(sDbKey, _T("AllocToNameField"));

		// attributes
		int nNumAttrib = pPrefs->GetProfileInt(sDbKey, _T("AttribCount"));

		for (int nAttrib = 0; nAttrib < nNumAttrib; nAttrib++)
		{
			ODBCATTRIBUTESETUP attrib;

			CString sEntry;
			sEntry.Format(_T("AttributeSetup%d"), nAttrib);

			CString sAttrib = pPrefs->GetProfileString(sDbKey, sEntry);

			if (sAttrib.IsEmpty())
				break;

			// else
			CStringArray aItems;

			if (Misc::Split(sAttrib, aItems, _T("::"), TRUE) != 9)
			{
				ASSERT(0);
				break;
			}

			// else
			attrib.nAttrib = (TDC_ATTRIBUTE)_ttoi(aItems[0]);
			attrib.sTaskField			= aItems[1];
			attrib.sAttribTable			= aItems[2];
			attrib.sAttribKeyField		= aItems[3];
			attrib.sAttribContentField	= aItems[4];
			attrib.sLinkTable			= aItems[5];
			attrib.sLinkTaskKeyField	= aItems[6];
			attrib.sLinkAttribKeyField	= aItems[7];
			attrib.sLinkTaskListKeyField	= aItems[8];

			db.setup.aAttrib.Add(attrib);
		}

		aDbSetup.Add(db);
	}

	return aDbSetup.GetSize();
}

void COdbcTasklistStorage::SaveDatabaseSetups(IPreferences* pPrefs, LPCTSTR szKey, const COdbcDatabaseSetupArray& aDbSetup) const
{
	int nNumDb = aDbSetup.GetSize();

	pPrefs->WriteProfileInt(szKey, _T("NumDatabases"), nNumDb);

	for (int nDb = 0; nDb < nNumDb; nDb++)
	{
		CString sDbKey(szKey);
		sDbKey += _T("\\Database");
		sDbKey += Misc::Format(nDb);

		const ODBCDATABASESETUP& db = aDbSetup.GetData()[nDb];

		pPrefs->WriteProfileString(sDbKey, _T("Name"), db.sDbName);
		pPrefs->WriteProfileString(sDbKey, _T("Connect"), db.sConnect);
		pPrefs->WriteProfileString(sDbKey, _T("TasklistID"), db.sTasklistID);
		pPrefs->WriteProfileString(sDbKey, _T("AllocToID"), db.sAllocToID);

		pPrefs->WriteProfileString(sDbKey, _T("TasksTable"), db.setup.tables.tableTasks.sTableName);
		pPrefs->WriteProfileString(sDbKey, _T("TasksKeyField"), db.setup.tables.tableTasks.sTaskKeyField);
		pPrefs->WriteProfileString(sDbKey, _T("TasklistKeyField"), db.setup.tables.tableTasks.sTasklistKeyField);
		pPrefs->WriteProfileString(sDbKey, _T("ParentKeyField"), db.setup.tables.tableTasks.sParentKeyField);

		pPrefs->WriteProfileString(sDbKey, _T("TasklistsTable"), db.setup.tables.tableTasklists.sTableName);
		pPrefs->WriteProfileString(sDbKey, _T("TasklistsKeyField"), db.setup.tables.tableTasklists.sKeyField);
		pPrefs->WriteProfileString(sDbKey, _T("TasklistsNameField"), db.setup.tables.tableTasklists.sContentField);

		pPrefs->WriteProfileString(sDbKey, _T("AllocToTable"), db.setup.tables.tableAllocTo.sTableName);
		pPrefs->WriteProfileString(sDbKey, _T("AllocToKeyField"), db.setup.tables.tableAllocTo.sKeyField);
		pPrefs->WriteProfileString(sDbKey, _T("AllocToNameField"), db.setup.tables.tableAllocTo.sContentField);

		// attributes
		int nNumAttrib = db.setup.aAttrib.GetSize();
		pPrefs->WriteProfileInt(sDbKey, _T("AttribCount"), nNumAttrib);

		for (int nAttrib = 0; nAttrib < nNumAttrib; nAttrib++)
		{
			const ODBCATTRIBUTESETUP& attrib = db.setup.aAttrib.GetData()[nAttrib];

			CString sAttrib, sEntry;

			sEntry.Format(_T("AttributeSetup%d"), nAttrib);
			sAttrib.Format(_T("%d::%s::%s::%s::%s::%s::%s::%s::%s"),
							attrib.nAttrib,
							attrib.sTaskField,
							attrib.sAttribTable,
							attrib.sAttribKeyField,
							attrib.sAttribContentField,
							attrib.sLinkTable,
							attrib.sLinkTaskKeyField,
							attrib.sLinkAttribKeyField,
							attrib.sLinkTaskListKeyField);

			pPrefs->WriteProfileString(sDbKey, sEntry, sAttrib);
		}
	}
}

bool COdbcTasklistStorage::LoadFromDatabase(const ODBCDATABASESETUP& dbSetup, ITaskList12* pTasks, CString& sErrMsg)
{
	CDatabaseEx db;

	if (db.Connect(dbSetup.sConnect))
	{
		COdbcTaskReader rs(&db);

		if (rs.Open(dbSetup) && !rs.IsBOF())
		{
			// 2-stage import:
			//
			// Stage 1 pulls the tasks into a temporary sorted flat list
			COdbcTaskArray aTasks;
			int nNumTasks = rs.GetTasks(aTasks);

			// Stage 2 creates the hierarchy
			if (nNumTasks)
			{
				// build a mapping between keys and task IDs
				// so that dependencies can be re-mapped
				COdbcMapKeyToID mapIDs;

				int nTask = nNumTasks;
				long dwTaskID = 1;

				while (nTask--)
				{
					mapIDs[aTasks[nTask]->sTaskKey] = dwTaskID++;
				}

				// build the tasklist, mapping tasks as we go
				// so we can do parent lookups
				CMapKeyToTask mapTasks;

				for (nTask = 0; nTask < nNumTasks; nTask++)
				{
					ODBCTASK* pTask = aTasks[nTask];
					
					// find parent
					HTASKITEM hParent = NULL;
					mapTasks.Lookup(pTask->sParentKey, hParent);

					// create task
					HTASKITEM hTask = pTasks->NewTask(pTask->sTitle, hParent, 0);

					// make sure we anticipated the task IDs correctly
					ASSERT(pTasks->GetTaskID(hTask) == (DWORD)(nTask + 1));

					// populate
					pTask->Save(pTasks, hTask, mapIDs);

					// map
					mapTasks[pTask->sTaskKey] = hTask;
				}

				// write Tasklist_ID in as meta-data
				if (!dbSetup.sTasklistID.IsEmpty())
				{
					pTasks->SetMetaData(ODBCTASK::GetMetaDataKey(_T("KEY")), dbSetup.sTasklistID);
				}

				// write current timestamp as meta-data so we can detect mods later
				pTasks->SetMetaData(ODBCTASK::GetMetaDataKey(_T("LOADTIME")), Misc::Format((DWORD)time(NULL)));

				return true;
			}

			// else error handling
			if (!dbSetup.sTasklist.IsEmpty() && !dbSetup.sAllocTo.IsEmpty())
			{
				sErrMsg.Format(CEnString(IDS_ERR_NOMATCHINGDATA_TASKLISTANDPERSON), dbSetup.sTasklist, dbSetup.sAllocTo);
			}
			else if (!dbSetup.sTasklist.IsEmpty())
			{
				sErrMsg.Format(CEnString(IDS_ERR_NOMATCHINGDATA_TASKLIST), dbSetup.sTasklist);
			}
			else if (!dbSetup.sAllocTo.IsEmpty())
			{
				sErrMsg.Format(CEnString(IDS_ERR_NOMATCHINGDATA_PERSON), dbSetup.sAllocTo);
			}
			else 
			{
				sErrMsg = CEnString(IDS_ERR_NOMATCHINGDATA);
			}
		}
		else
		{
			sErrMsg = CEnString(IDS_ERR_NOMATCHINGDATA);
		}
	}
	else
	{
		sErrMsg = CEnString(IDS_ERR_FAILEDTOCONNECT);
	}

	return false;
}

bool COdbcTasklistStorage::SaveToDatabase(const ITaskList12* pTasks, const ODBCDATABASESETUP& dbSetup, CString& sErrMsg)
{
	// extract meta-data to help us decide what needs updating
	time64_t tLoad = _ttoi64(pTasks->GetMetaData(ODBCTASK::GetMetaDataKey(_T("LOADTIME"))));
	CString sTasklistID = pTasks->GetMetaData(ODBCTASK::GetMetaDataKey(_T("KEY")));

	COdbcMapIDToKey mapIDs;
	CDatabaseEx db;
	
	if (db.Connect(dbSetup.sConnect))
	{
		COdbcTaskWriter rs;
		
		if (rs.Open(&db, dbSetup, sTasklistID))
		{
			return (AddOrUpdateTask(pTasks, pTasks->GetFirstTask(), tLoad, rs, dbSetup, mapIDs, TRUE) != FALSE);

#ifdef _DEBUG
			// else
			sErrMsg = CEnString(IDS_ERR_NOCHANGEDDATA);
#endif
		}
		else
		{
			sErrMsg = CEnString(IDS_ERR_FAILEDTOCONNECT);
		}
	}
	else
	{
		sErrMsg = CEnString(IDS_ERR_FAILEDTOCONNECT);
	}

	return false;
}

BOOL COdbcTasklistStorage::AddOrUpdateTask(const ITaskList12* pTasks, HTASKITEM hTask, time64_t tLoad, 
										   COdbcTaskWriter& rs, const ODBCDATABASESETUP& dbSetup, 
										   COdbcMapIDToKey& mapIDs, BOOL bAndSiblings)
{
	if (hTask == NULL)
		return FALSE;

	BOOL bAdded = FALSE;
	ODBCTASK task;
	
	if (task.Load(pTasks, hTask, mapIDs))
	{
		ASSERT((task.dwParentID == 0) || !task.sParentKey.IsEmpty());

		if (task.sTaskKey.IsEmpty()) // new task
		{
			if (rs.AddTask(task, task.sTaskKey))
			{
				bAdded = TRUE;

				// map as we go for our children
				mapIDs[task.dwID] = task.sTaskKey;
			}
			else
			{
				ASSERT(0);
			}
		}
		else if (task.tModified > tLoad)
		{
			if (rs.UpdateTask(task))
			{
				bAdded = TRUE;
			}
			else
			{
				ASSERT(0);
			}
		}

		// first child
		if (AddOrUpdateTask(pTasks, pTasks->GetFirstTask(hTask), tLoad, rs, dbSetup, mapIDs, TRUE))
			bAdded = TRUE;
	}
	else
	{
		ASSERT(0);
	}
	
	// handle siblings WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == not siblings
			if (AddOrUpdateTask(pTasks, hSibling, tLoad, rs, dbSetup, mapIDs, FALSE))
				bAdded = TRUE;
			
			hSibling = pTasks->GetNextTask(hSibling);
		}
	}
	
	return bAdded;
}

bool COdbcTasklistStorage::GetDatabaseSetup(const CString& sConnect, const COdbcDatabaseSetupArray& aDbSetup, ODBCDATABASESETUP& dbSetup) const
{
	int nDb = aDbSetup.GetSize();

	while (nDb--)
	{
		const ODBCDATABASESETUP& db = aDbSetup.GetData()[nDb];

		if (db.sConnect.CompareNoCase(sConnect) == 0)
		{
			dbSetup = db;
			return true;
		}
	}

	// not found
	return false;
}
