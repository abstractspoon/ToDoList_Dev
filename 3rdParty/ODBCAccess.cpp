/*********************************************************************

   Copyright (C) 2001 Alexander Wiseman, all rights reserved
   Alexander@wisemanweb.com

   This class is provided 'as-is', without any express or implied
   warranty.  In no event must the author be held liable for any damages
   arising from the use of this class.

   This code may be used in compiled form in any way you desire.  In addition,
   you may freely change and distribute the source code subject to three
   restrictions, listed as follows:

   1. The origin of this class must not be misrepresented.  You cannot claim
		that you wrote the original code; the author's name must be included.

   2. Altered source versions must be plainly marked as such, and cannot be
		misrepresented as the original source code.

   3. This notice may not be removed or altered from any source distribution.

   The files included with this class (Recordset.*, Tables.*, and Columns.*)
   were coded by George Polouse.

   If you use this file in a commercial program, an email letting me know you
   are how you are using it would be nice, but is not required.

   If you find any bugs or, worse yet, memory leaks (aggh!), please, please don't
   hesitate to send me an email with the error.

  //HISTORY//

  1.0:	November 29-December 1 2001
	- First release.

  1.1:	March 27 2002
	- Fixed bug in EndTransaction
	- Changed all recordset openers to open sets as 'dynaset' instead of 'forwardOnly'

  1.2:	June 12 2002
	- Added new function CloseAllSets, which closes all the open recordsets, including
		the table set and columns set
	- Changed MAX_SETS to 20: you can change this if you want more sets

  1.3:	February 15, 2003
	- Added new function CreateDatabase, which creates a new .mdb file with an options
		to open and connect to the just created database
	- Added new function IsConnected, which returns true if currently connected to a database

*********************************************************************/

// ODBCAccess.cpp: implementation of the CODBCAccess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ODBCAccess.h"
#include <odbcinst.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction/Helpers
//////////////////////////////////////////////////////////////////////

//FUNCTION:	CODBCAccess (constructor)
//JOB:		initialization
CODBCAccess::CODBCAccess()
{
	//Set default settings:
	m_nLoginTimeout = 60;
	m_nQueryTimeout = 60;

	//Set internals:
	m_bDBOpen = false;
	m_bInTrans = false;
	m_bSupportTrans = false;

	//Pointers always start at NULL:
	myTables = NULL;
	myColumns = NULL;
}

//FUNCTION:	~CODBCAccess (destructor)
//JOB:		deinitialization
CODBCAccess::~CODBCAccess()
{
	//Destroy:
	Disconnect();
}

//////////////////////////////////////////////////////////////////////
// Optional Settings
//////////////////////////////////////////////////////////////////////

//FUNCTION:	SetTimouts
//JOB:		setting the timeouts for login and querying
void CODBCAccess::SetTimeouts(int nLoginSeconds, int nQuerySeconds)
{
	//Set them and return:
	m_nLoginTimeout = nLoginSeconds;
	m_nQueryTimeout = nQuerySeconds;
}

//////////////////////////////////////////////////////////////////////
// Data sources
//////////////////////////////////////////////////////////////////////

//FUNCTION: ConfigDataSource
//JOB:		allows you to add modify or delete generic data sources
BOOL CODBCAccess::ConfigDataSource(UINT nOperation, const CString &sDriver,
								   const CString &sAttributes)
{
	//Validify data:
	if(nOperation < 0 || nOperation > 2 || sDriver == "")
	{
		//Set last error:
		m_nLastReturn = RETURN_ARGUMENT;
		m_sLastErrorMsg = "Argument Invalid!";
		return false;
	}

	//Switch on nOperation:
	WORD fRequest;
	switch(nOperation) {
	case DATASOURCE_ADD: {	/*Adding data source*/ fRequest = ODBC_ADD_DSN; break; }
	case DATASOURCE_MOD: {	/*Modifying data source*/ fRequest = ODBC_CONFIG_DSN; break; }
	case DATASOURCE_DEL: {	/*Deleting data source*/ fRequest = ODBC_REMOVE_DSN; break; }
	default: { /*Invalid*/ 
		//Set last error:
		m_nLastReturn = RETURN_ARGUMENT;
		m_sLastErrorMsg = "Argument Invalid!";
		return false;
			 }
	}

	//Make the call and check the return code:
	BOOL retcode = 
		::SQLConfigDataSource(NULL, fRequest, sDriver, sAttributes);

	//Check the return code:
	if(!retcode) { m_nLastReturn = RETURN_ARGUMENT; 
				   m_sLastErrorMsg = "SQLConfigDataSource failed!  Check arguments "
					   "and driver specifications..."; 
				   return false; }

	//Success!
	SetSuccessMsg();
	return true;
}

//FUNCTION: ConfigDataSource
//JOB:		allows you to add modify or delete Access data sources
BOOL CODBCAccess::ConfigDataSource(UINT nOperation, const CString &sDSN, const CString &sDatabase,
								   BOOL bConnectAfter, const CString &sDescription, BOOL bReadOnly,
								   BOOL bExclusive)
{
	//Validify data:
	if(nOperation < 0 || nOperation > 2 || sDSN == "" || sDatabase == "")
	{
		//Set last error:
		m_nLastReturn = RETURN_ARGUMENT;
		m_sLastErrorMsg = "Argument Invalid!";
		return false;
	}

	//Configure the lpszAttributes for passing to SQLConfigDataSource:
	CString attributes = "DSN=" + sDSN + ";DBQ=" + sDatabase + ";";

	//Check optional variables:
	if(sDescription != "") { attributes = attributes + "DESCRIPTION=" + sDescription + ";"; }
	
	//Append rest of variables:
	if(bReadOnly) { attributes = attributes + "READONLY=TRUE;"; }
	else { attributes = attributes + "READONLY=FALSE;"; }
	if(bExclusive) { attributes = attributes + "EXCLUSIVE=TRUE;"; }
	else { attributes = attributes + "EXCLUSIVE=FALSE;"; }

	//Switch on nOperation:
	WORD fRequest;
	switch(nOperation) {
	case DATASOURCE_ADD: {	/*Adding data source*/ fRequest = ODBC_ADD_DSN; break; }
	case DATASOURCE_MOD: {	/*Modifying data source*/ fRequest = ODBC_CONFIG_DSN; break; }
	case DATASOURCE_DEL: {	/*Deleting data source*/ fRequest = ODBC_REMOVE_DSN; break; }
	default: { /*Invalid*/ 
		//Set last error:
		m_nLastReturn = RETURN_ARGUMENT;
		m_sLastErrorMsg = "Argument Invalid!";
		return false;
			 }
	}

	//Make the call and check the return code:
	BOOL retcode = 
		::SQLConfigDataSource(NULL, fRequest, "Microsoft Access Driver (*.mdb)", attributes);

	//Check the return code:
	if(!retcode) { m_nLastReturn = RETURN_ARGUMENT; 
				   m_sLastErrorMsg = "SQLConfigDataSource failed!  Check arguments..."; 
				   return false; }

	//Success!
	SetSuccessMsg();

	//Now check if we should connect:
	if(bConnectAfter && nOperation != DATASOURCE_DEL)
	{
		//Return the code from Connect:
		return (Connect(sDSN));
	}

	//Return success!
	return true;
}

//FUNCTION: CreateDatabase
//JOB:		creates a new database file
BOOL CODBCAccess::CreateDatabase(const CString &sFileName,  BOOL bConfigDataSource,  const CString &sDSN,  
								 BOOL bConnectAfter,  const CString &sDescription,  BOOL bReadOnly,  BOOL bExclusive)
{
	//Validify data:
	if(sFileName == "" || (bConfigDataSource && sDSN == ""))
	{
		//Set last error:
		m_nLastReturn = RETURN_ARGUMENT;
		m_sLastErrorMsg = "No filename specified, or blank DSN.";
		return false;
	}

	//Assemble the attributes:
	CString attributes = "CREATE_DB=\"" + sFileName + "\";";

	//Create DB and check return code:
	BOOL retcode = 
		::SQLConfigDataSource(NULL, ODBC_ADD_DSN, "Microsoft Access Driver (*.mdb)", attributes);

	//Check return code:
	if(!retcode) { m_nLastReturn = RETURN_ARGUMENT; 
				   m_sLastErrorMsg = "Creating database with SQLConfigDataSource failed!  Check arguments..."; 
				   return false; }

	//Success!
	SetSuccessMsg();

	//Now check to create the DSN:
	if(bConfigDataSource)
	{
		//Return the result of the config data source call:
		return (ConfigDataSource(DATASOURCE_ADD, sDSN, sFileName, bConnectAfter, sDescription, bReadOnly, bExclusive));
	}

	//Return success!
	return true;
}

//FUNCTION:	Connect
//JOB:		connects to a data source:
BOOL CODBCAccess::Connect(const CString &sDSN, const CString &sID, const CString &sPass, BOOL bExclusive, BOOL bReadOnly)
{
	//Is a database already open?
	if(m_bDBOpen) { Disconnect(); }

	//Set login and query timeouts:
	myDatabase.SetLoginTimeout(m_nLoginTimeout);
	myDatabase.SetQueryTimeout(m_nQueryTimeout);

	//Connect to the specified DSN:
	try
	{
		//Assemble the dwOptions param:
		DWORD options = CDatabase::noOdbcDialog;
		if(bExclusive) { options |= CDatabase::openExclusive; }
		if(bReadOnly) { options |= CDatabase::openReadOnly; }

		//Try it:
		myDatabase.OpenEx(_T("DSN=" + sDSN + ";UID=" + sID + ";PWD=" + sPass));
	}
	catch(CDBException* pDBE)
	{
		//Get the last error message:
		m_sLastErrorMsg = "Connect Failed!  " + pDBE->m_strError;
		m_nLastReturn = RETURN_ERRMSG;
		pDBE->Delete();
		return false;
	}

	//Success!
	SetSuccessMsg();
	m_bDBOpen = true;
	return true;
}

//FUNCTION: Connect
//JOB:		connects to a datasource using the ODBC connect dialog
BOOL CODBCAccess::Connect()
{
	//Is a database already open?
	if(m_bDBOpen) { Disconnect(); }

	//Set login and query timeouts:
	myDatabase.SetLoginTimeout(m_nLoginTimeout);
	myDatabase.SetQueryTimeout(m_nQueryTimeout);

	//Connect to the selected DSN:
	try
	{
		//Try it:
		myDatabase.OpenEx(NULL);
	}
	catch(CDBException* pDBE)
	{
		//Get the last error message:
		m_sLastErrorMsg = "Connect Failed!  " + pDBE->m_strError;
		m_nLastReturn = RETURN_ERRMSG;
		pDBE->Delete();
		return false;
	}

	//Success!
	SetSuccessMsg();
	m_bDBOpen = true;
	return true;
}

//FUNCTION:	Disconnect
//JOB:		closes the open database
void CODBCAccess::Disconnect()
{
	//Make sure not already closed:
	if(!m_bDBOpen)
		return;

	//Unload everything; temp vars:
	CString tempStr;
	CRecordsetEx *tempRec;

	//In transaction?
	if(m_bInTrans)
	{
		//Rollback changes:
		Rollback();
	}

	//Any open sets:
	POSITION pos = mySets.GetStartPosition();
	for(int h = 0; h < mySets.GetCount(); h++)
	{
		//Close this one:
		mySets.GetNextAssoc(pos, tempStr, (void*&)tempRec);
		if(tempRec->IsOpen())
		{
			tempRec->Close();
		}

		//Delete the memory:
		delete tempRec;
		tempRec = NULL;

		//And remove it:
		mySets.RemoveKey(tempStr);
	}

	//Next, if the tables or columns still exist, get rid of them:
	if(myTables)
	{
		//Close:
		if(myTables->IsOpen())
		{
			myTables->Close();
		}
		delete myTables;
		myTables = NULL;
	}
	if(myColumns)
	{
		//Close:
		if(myColumns->IsOpen())
		{
			myColumns->Close();
		}
		delete myColumns;
		myColumns = NULL;
	}

	//Close database:
	myDatabase.Close();

	//Set internals:
	m_bDBOpen = false;
}

//////////////////////////////////////////////////////////////////////
// SQL Support
//////////////////////////////////////////////////////////////////////

//FUNCTION: StartTransaction
//JOB:		begins a transaction (autohandles all errors)
BOOL CODBCAccess::StartTransaction()
{
	//Check database not open:
	if(!m_bDBOpen)
	{
		//Fails:
		m_nLastReturn = RETURN_DB;
		m_sLastErrorMsg = "Database Not Opened!";
		return false;
	}

	//Already in transaction?
	if(m_bInTrans)
	{
		//Fails:
		m_nLastReturn = RETURN_ERRMSG;
		m_sLastErrorMsg = "You are already in a transaction!";
		return false;
	}

	//Try and start the transaction:
	if(!myDatabase.CanTransact())
	{
		//Set m_bSupportTans:
		m_bSupportTrans = false;
		m_nLastReturn = RETURN_ERRMSG;
		m_sLastErrorMsg = "No Support For Transactions!";
		return false;
	}
	else
	{
		//does support:
		m_bSupportTrans = true;
	}

	//Start the transaction:
	BOOL retcode = myDatabase.BeginTrans();
	if(!retcode)
	{
		//Error!
		m_nLastReturn = RETURN_ERRMSG;
		m_sLastErrorMsg = "BeginTrans failed!  Make sure you do not have any open Recordsets!";
		m_bInTrans = false;
		return false;
	}

	//Success!
	SetSuccessMsg();
	m_bInTrans = true;
	return true;
}

//FUNCTION: SendSQL
//JOB:		sending SQL statements to the database
//			this function keeps track of transactions and will constantly error check
BOOL CODBCAccess::SendSQL(const CString &sSQL)
{
	//Check database not open:
	if(!m_bDBOpen)
	{
		//Fails:
		m_nLastReturn = RETURN_DB;
		m_sLastErrorMsg = "Database Not Opened!";
		return false;
	}

	//Check argument:
	if(sSQL == _T(""))
	{
		//Fails:
		m_nLastReturn = RETURN_ARGUMENT;
		m_sLastErrorMsg = "sSQL not valid!";
		return false;
	}

	//Try and send the SQL statement:
	try
	{
		//Execute:
		myDatabase.ExecuteSQL(sSQL);
	}
	catch(CDBException *pDBE)
	{
		//If in a transaction, roll back changes:
		if(m_bInTrans)
		{
			//Try rollback:
			if(!Rollback())
			{
				//Big errors!
				m_nLastReturn = RETURN_CRITICAL;
				m_sLastErrorMsg = "Rollback failed!  Database closed...";
				Disconnect();
				pDBE->Delete();
				return false;
			}

			//Close the transaction:
			m_bInTrans = false;
			m_nLastReturn = RETURN_ERRMSG;
			m_sLastErrorMsg = "Rollback succeeded when SQL error encountered!";
			pDBE->Delete();
			return false;
		}
		else
		{
			//Report error:
			m_nLastReturn = RETURN_ERRMSG;
			m_sLastErrorMsg = "Error encountered when trying to execute SQL statement!";
			pDBE->Delete();
			return false;
		}
	}

	//Success!
	SetSuccessMsg();
	return true;
}

//FUNCTION: Rollback
//JOB:		allows you to manually rollback any changes:
BOOL CODBCAccess::Rollback()
{
	//Check open database:
	if(!m_bDBOpen)
	{
		//Fails:
		m_nLastReturn = RETURN_DB;
		m_sLastErrorMsg = "Database Not Opened!";
		return false;
	}

	//Check m_bInTrans and m_bSupportTrans:
	if(!m_bInTrans || !m_bSupportTrans)
	{
		//Fails:
		m_nLastReturn = RETURN_ERRMSG;
		m_sLastErrorMsg = "Not in a transaction or database does not support transactions!";
		return false;
	}

	//Rollback:
	myDatabase.Rollback();

	//Close the transaction:
	m_bInTrans = false;

	//Success!
	SetSuccessMsg();
	return true;
}

//FUNCTION:	EndTransaction
//JOB:		closes the transaction (commits the changes) and rolls back if errors occur
BOOL CODBCAccess::EndTransaction()
{
	//Check open database:
	if(!m_bDBOpen)
	{
		//Fails:
		m_nLastReturn = RETURN_DB;
		m_sLastErrorMsg = "Database Not Opened!";
		return false;
	}

	//Check m_bInTrans and m_bSupportTrans:
	if(!m_bInTrans || !m_bSupportTrans)
	{
		//Fails:
		m_nLastReturn = RETURN_ERRMSG;
		m_sLastErrorMsg = "Not in a transaction or database does not support transactions!";
		return false;
	}

	//Try and commit transaction:
	try
	{
		//Commit:
		myDatabase.CommitTrans();
	}
	catch(CDBException* pDBE)
	{
		//Rollback:
		if(!Rollback())
		{
			//Big errors!
			m_nLastReturn = RETURN_CRITICAL;
			m_sLastErrorMsg = "Rollback failed!  Database closed...";
			Disconnect();
			pDBE->Delete();
			return false;
		}

		//Close the transaction:
		m_bInTrans = false;
		m_nLastReturn = RETURN_ERRMSG;
		m_sLastErrorMsg = "Rollback succeeded when CommitTrans failed!";
		pDBE->Delete();
		return false;
	}

	//Success!
	m_bInTrans = false;
	SetSuccessMsg();
	return true;
}

//////////////////////////////////////////////////////////////////////
// Recordset Methods
//////////////////////////////////////////////////////////////////////

//FUNCTION: OpenNewSet
//JOB:		opens a new set of records
BOOL CODBCAccess::OpenNewSet(const CString &sFriendlyName, const CString &sSQL, const CString &sTableName)
{
	//Check open database:
	if(!m_bDBOpen)
	{
		//Fails:
		m_nLastReturn = RETURN_DB;
		m_sLastErrorMsg = "Database Not Opened!";
		return false;
	}

	//Check too many sets open:
	if(mySets.GetCount() >= MAX_SETS)
	{
		//Fails:
		m_nLastReturn = RETURN_ERRMSG;
		m_sLastErrorMsg = "Too many sets open!";
		return false;
	}

	//Check the arguments:
	if(sFriendlyName == _T("") && sSQL == _T("") && sTableName == _T(""))
	{
		//Fails:
		m_nLastReturn = RETURN_ARGUMENT;
		m_sLastErrorMsg = "Arguments not specified!";
		return false;
	}

	//Temporary stuff:
	CRecordsetEx *dyna;

	//Check that this does not already exist:
	if(mySets.Lookup(sFriendlyName, (void*&)dyna))
	{
		//This already exists, fail it:
		m_nLastReturn = RETURN_ERRMSG;
		m_sLastErrorMsg = "That friendly name is taken!";
		dyna = NULL;
		return false;
	}

	//Create the set and save the key in the map:
	dyna = new CRecordsetEx(&myDatabase);

	//Try and open the set based on SQL or table name:
	if(sSQL != _T(""))
	{
		//ExecDirect on the set:
		if(!dyna->ExecDirect(sSQL))
		{
			//The execution failed!
			m_nLastReturn = RETURN_ERRMSG;
			m_sLastErrorMsg = "ExecDirect with SQL statement failed!";
			delete dyna;
			dyna = NULL;
			return false;
		}

		//Succeeded!
	}
	else
	{
		//ExecDirect with the table name:
		if(!dyna->ExecDirect("SELECT * FROM [" + sTableName + "]"))
		{
			//The execution failed!
			m_nLastReturn = RETURN_ERRMSG;
			m_sLastErrorMsg = "ExecDirect with table name failed!";
			delete dyna;
			dyna = NULL;
			return false;
		}

		//Succeeded!
	}

	//Add this to the map:
	mySets.SetAt(sFriendlyName, (void*&)dyna);

	//Success!
	SetSuccessMsg();
	return true;
}

//FUNCTION: CloseSet
//JOB:		closes the set specified by the friendly name
BOOL CODBCAccess::CloseSet(const CString &sFriendlyName)
{
	//Check open database:
	if(!m_bDBOpen)
	{
		//Fails:
		m_nLastReturn = RETURN_DB;
		m_sLastErrorMsg = "Database Not Opened!";
		return false;
	}

	//Check valid argument:
	if(sFriendlyName == _T(""))
	{
		//Fails:
		m_nLastReturn = RETURN_ARGUMENT;
		m_sLastErrorMsg = "sFriendlyName is invalid!";
		return false;
	}

	//Temporary variables:
	CRecordsetEx *dyna;

	//Try and look it up:
	if(!mySets.Lookup(sFriendlyName, (void*&)dyna))
	{
		//Wrong key name (it doesn't exist):
		m_nLastReturn = RETURN_ERRMSG;
		m_sLastErrorMsg = "sFriendlyName specifies a key that does not exist!";
		return false;
	}

	//Safely close it if open:
	if(dyna->IsOpen())
	{
		//Close:
		dyna->Close();
	}

	//Since CloseSet is intended to be called when done with the set, now delete
	//the memory and remove the key:
	delete dyna;
	dyna = NULL;
	mySets.RemoveKey(sFriendlyName);

	//Success!
	SetSuccessMsg();
	return true;
}

//FUNCTION: CloseAllSets
//JOB:		closes all the open recordsets
BOOL CODBCAccess::CloseAllSets()
{
	//Check open database:
	if(!m_bDBOpen)
	{
		//Fails:
		m_nLastReturn = RETURN_DB;
		m_sLastErrorMsg = "Database Not Opened!";
		return false;
	}

	//Temporary variables:
	CString tempStr;
	CRecordsetEx *tempRec;

	//Close any open sets:
	POSITION pos = mySets.GetStartPosition();
	for(int h = 0; h < mySets.GetCount(); h++)
	{
		//Close this one:
		mySets.GetNextAssoc(pos, tempStr, (void*&)tempRec);
		if(tempRec->IsOpen())
		{
			tempRec->Close();
		}

		//Delete the memory:
		delete tempRec;
		tempRec = NULL;

		//And remove it:
		mySets.RemoveKey(tempStr);
	}

	//Next, if the tables or columns still exist, get rid of them:
	if(myTables)
	{
		//Close:
		if(myTables->IsOpen())
		{
			myTables->Close();
		}
	}
	if(myColumns)
	{
		//Close:
		if(myColumns->IsOpen())
		{
			myColumns->Close();
		}
	}

	//Success!
	SetSuccessMsg();
	return true;
}

//FUNCTION: GetSet
//JOB:		gets a set with the specified friendly name
BOOL CODBCAccess::GetSet(const CString &sFriendlyName, CRecordsetEx *&dyna)
{
	//Check open database:
	if(!m_bDBOpen)
	{
		//Fails:
		m_nLastReturn = RETURN_DB;
		m_sLastErrorMsg = "Database Not Opened!";
		dyna = NULL;
		return false;
	}

	//Check valid argument:
	if(sFriendlyName == _T(""))
	{
		//Fails:
		m_nLastReturn = RETURN_ARGUMENT;
		m_sLastErrorMsg = "sFriendlyName is invalid!";
		dyna = NULL;
		return false;
	}

	//At this point, make sure the pointer is NULL:
	dyna = NULL;

	//Try and look it up:
	if(!mySets.Lookup(sFriendlyName, (void*&)dyna))
	{
		//Wrong key name (it doesn't exist):
		m_nLastReturn = RETURN_ERRMSG;
		m_sLastErrorMsg = "sFriendlyName specifies a key that does not exist!";
		dyna = NULL;
		return false;
	}

	//Success!
	SetSuccessMsg();
	return true;
}

//FUNCTION: GetSet (overload)
//JOB:		gets a set with the specified sFiendlyName
//NOTE:		if this fails, it will return NULL, so you must check if the return was NULL!
CRecordsetEx* CODBCAccess::GetSet(const CString &sFriendlyName)
{
	//Check open database:
	if(!m_bDBOpen)
	{
		//Fails:
		m_nLastReturn = RETURN_DB;
		m_sLastErrorMsg = "Database Not Opened!";
		return NULL;
	}

	//Check valid argument:
	if(sFriendlyName == _T(""))
	{
		//Fails:
		m_nLastReturn = RETURN_ARGUMENT;
		m_sLastErrorMsg = "sFriendlyName is invalid!";
		return NULL;
	}

	//Pointer which will recieve the set pointer:
	CRecordsetEx *dyna;

	//Try and look it up:
	if(!mySets.Lookup(sFriendlyName, (void*&)dyna))
	{
		//Wrong key name (it doesn't exist):
		m_nLastReturn = RETURN_ERRMSG;
		m_sLastErrorMsg = "sFriendlyName specifies a key that does not exist!";
		dyna = NULL;
		return NULL;
	}

	//Success!
	SetSuccessMsg();
	return dyna;
}

//FUNCTION: GetNamesOfOpenSets
//JOB:		fills a passed CStringArray with the names of all open sets
void CODBCAccess::GetNamesOfOpenSets(CStringArray &array)
{
	//Whether the database is open or not does not matter, because all will be handled
	//by examining the count returned by CMap::GetCount:
	array.SetSize(mySets.GetCount());

	//Save some time:
	if(mySets.GetCount() == 0)
		return;

	//Temporary variables:
	int h = 0;
	CRecordsetEx *garbage;
	CString key;
	POSITION pos = mySets.GetStartPosition();

	//Loop through and add each key name:
	while(pos != NULL)
	{
		//Get this association:
		mySets.GetNextAssoc(pos, key, (void*&)garbage);

		//Add to array:
		array.SetAt(h, key);
		h++;
	}
}

//FUNCTION: QueryOpenTables
//JOB:		opens a set which enumerates the tables in a database
BOOL CODBCAccess::QueryOpenTables()
{
	//Check open database:
	if(!m_bDBOpen)
	{
		//Fails:
		m_nLastReturn = RETURN_DB;
		m_sLastErrorMsg = "Database Not Opened!";
		return false;
	}

	//Was pointer initialized?
	if(!myTables)
	{
		//Make sure we create it:
		myTables = new CTables(&myDatabase);
	}

	//Make sure we close the set if it was previously open:
	if(myTables->IsOpen())
	{
		//Close safely:
		myTables->Close();
	}

	//Now reopen it:
	myTables->Open(NULL, NULL, NULL, NULL, CRecordset::dynaset);

	//Success!
	SetSuccessMsg();
	return true;
}

//FUNCTION: QueryOpenColumns
//JOB:		queries a table and establishes a recordset with the column names in it
BOOL CODBCAccess::QueryOpenColumns(const CString &sTableName)
{
	//Check open database:
	if(!m_bDBOpen)
	{
		//Fails:
		m_nLastReturn = RETURN_DB;
		m_sLastErrorMsg = "Database Not Opened!";
		return false;
	}

	//Check valid argument:
	if(sTableName == _T(""))
	{
		//Fails:
		m_nLastReturn = RETURN_ARGUMENT;
		m_sLastErrorMsg = "sTableName is invalid!";
		return false;
	}

	//Was pointer initialized?
	if(!myColumns)
	{
		//Make sure we create it:
		myColumns = new CColumns(&myDatabase);
	}

	//Make sure we close the set if it was previously open:
	if(myColumns->IsOpen())
	{
		//Close safely:
		myColumns->Close();
		
	}

	//Open it with the proper table name:
	myColumns->Open(NULL, NULL, sTableName, NULL, CRecordset::dynaset);

	//Success!
	SetSuccessMsg();
	return true;
}

//FUNCTION: GetTables
//JOB:		returns (in the pointer) the currently open (MUST BE OPEN!) tables
BOOL CODBCAccess::GetTables(CTables *&table)
{
	//Check open database:
	if(!m_bDBOpen)
	{
		//Fails:
		m_nLastReturn = RETURN_DB;
		m_sLastErrorMsg = "Database Not Opened!";
		return false;
	}

	//Check if tables has even been queried:
	if(!myTables)
	{
		//Cannot continue:
		m_nLastReturn = RETURN_ERRMSG;
		m_sLastErrorMsg = "QueryOpenTables was not called to create the CTables object!";
		return false;
	}

	//Check if tables is open:
	if(!myTables->IsOpen())
	{
		//Cannot continue:
		m_nLastReturn = RETURN_ERRMSG;
		m_sLastErrorMsg = "The tables recordset is not open!";
		return false;
	}

	//At this point make sure the user's pointer is null:
	table = NULL;

	//Now assign it:
	table = myTables;

	//Success!
	SetSuccessMsg();
	return true;
}

//FUNCTION: GetTables (overload)
//JOB:		returns the currently open (MUST BE OPEN!) tables pointer
//NOTE:		if this fails, it will return NULL, so you must check if the return was NULL!
CTables* CODBCAccess::GetTables()
{
	//Check open database:
	if(!m_bDBOpen)
	{
		//Fails:
		m_nLastReturn = RETURN_DB;
		m_sLastErrorMsg = "Database Not Opened!";
		return NULL;
	}

	//Check if tables has even been queried:
	if(!myTables)
	{
		//Cannot continue:
		m_nLastReturn = RETURN_ERRMSG;
		m_sLastErrorMsg = "QueryOpenTables was not called to create the CTables object!";
		return false;
	}

	//Check if tables is open:
	if(!myTables->IsOpen())
	{
		//Cannot continue:
		m_nLastReturn = RETURN_ERRMSG;
		m_sLastErrorMsg = "The tables recordset is not open!";
		return NULL;
	}

	//Success!
	SetSuccessMsg();
	return myTables;
}

//FUNCTION: GetColumns
//JOB:		returns (in the pointer) the currently open (MUST BE OPEN!) columns
BOOL CODBCAccess::GetColumns(CColumns *&cols)
{
	//Check open database:
	if(!m_bDBOpen)
	{
		//Fails:
		m_nLastReturn = RETURN_DB;
		m_sLastErrorMsg = "Database Not Opened!";
		return false;
	}

	//Check if columns has even been queried:
	if(!myColumns)
	{
		//Cannot continue:
		m_nLastReturn = RETURN_ERRMSG;
		m_sLastErrorMsg = "QueryOpenColumns was not called to create the CColumns object!";
		return false;
	}

	//Check if tables is open:
	if(!myColumns->IsOpen())
	{
		//Cannot continue:
		m_nLastReturn = RETURN_ERRMSG;
		m_sLastErrorMsg = "The tables recordset is not open!";
		return false;
	}

	//At this point make sure the user's pointer is null:
	cols = NULL;

	//Now assign it:
	cols = myColumns;

	//Success!
	SetSuccessMsg();
	return true;
}

//FUNCTION: GetColumns (overload)
//JOB:		returns the currently open (MUST BE OPEN!) columns pointer
//NOTE:		if this fails, it will return NULL, so you must check if the return was NULL!
CColumns* CODBCAccess::GetColumns()
{
	//Check open database:
	if(!m_bDBOpen)
	{
		//Fails:
		m_nLastReturn = RETURN_DB;
		m_sLastErrorMsg = "Database Not Opened!";
		return NULL;
	}

	//Check if columns has even been queried:
	if(!myColumns)
	{
		//Cannot continue:
		m_nLastReturn = RETURN_ERRMSG;
		m_sLastErrorMsg = "QueryOpenColumns was not called to create the CColumns object!";
		return false;
	}

	//Check if tables is open:
	if(!myColumns->IsOpen())
	{
		//Cannot continue:
		m_nLastReturn = RETURN_ERRMSG;
		m_sLastErrorMsg = "The tables recordset is not open!";
		return NULL;
	}

	//Success!
	SetSuccessMsg();
	return myColumns;
}