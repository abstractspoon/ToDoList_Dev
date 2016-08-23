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

   FOR History, please see accompanying .cpp file (ODBCAccess.cpp)

*********************************************************************/

// ODBCAccess.h: interface for the CODBCAccess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ODBCACCESS_H__21B055FF_4BC2_4987_A5E3_95DBA6BCC1E7__INCLUDED_)
#define AFX_ODBCACCESS_H__21B055FF_4BC2_4987_A5E3_95DBA6BCC1E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//Data source operational defines:
#define		DATASOURCE_ADD	0	//add a datasource
#define		DATASOURCE_MOD	1	//delete "
#define		DATASOURCE_DEL	2	//modify "

//Return code defines:
#define		RETURN_SUCCESS	0	//operation succeeded
#define		RETURN_ARGUMENT	1	//there was a bad argument passed to the function
#define		RETURN_ERRMSG	2	//default exception error message, view by calling GetLastErrMsg
#define		RETURN_DB		3	//database is not open or is invalid (most likely not open)
#define		RETURN_CRITICAL	4	//critical error, database closed, examine GetLastErrMsg for details

//Constant MAX defines
#define		MAX_SETS		20	//only allowed a max of 20 sets

//Necessary includes:
#include "RecordsetEx.h"		//	}
#include "Columns.h"			//	} These three files by George Polouse
#include "Tables.h"				//	}

#include <afxcoll.h>			//
#include <afxtempl.h>			//	} MFC required classes for CStringArray

//CLASS CODBCAccess
class CODBCAccess  
{
	//PUBLIC to the class:
	public:

		// ** Constructor, destructor **
		CODBCAccess();
		virtual ~CODBCAccess();

		// ** OPTIONAL SETTINGS **

		//For setting timouts: (you dont have to call this to accept the default settings)
		//Returns: void
		void SetTimeouts(
			int nLoginSeconds,	//# of seconds for login timeout
			int nQuerySeconds);	//# of seconds for query timeout

		// ** DATA SOURCES **

		//For adding/modifying/deleting an Access driver data source:
		//Returns: TRUE if successful, false if not (you can view the last return to check errors)
		BOOL ConfigDataSource(	
			UINT nOperation,	//What you want to do to the data source
			const CString &sDSN,//The data source name (e.g "Payroll")
			const CString &sDatabase,	//The absolute path of the database
			BOOL bConnectAfter, //Connects to the data source you just configured
			const CString &sDescription = "", //Optional: description of data source
			BOOL bReadOnly = false, //Optional: read only access
			BOOL bExclusive = false); //Optional: one user at a time access

		//For adding/modifying/deleting a generic data source:
		//Returns: TRUE if successful, false if not (you can view the last return to check errors)
		BOOL ConfigDataSource(
			UINT nOperation,	//What you want to do to the data source
			const CString &sDriver,	//The driver name
			const CString &sAttributes);//The attributes for the driver	
		
		//For creating a new database file:
		//Returns: TRUE if successful, false if not (you can view the last return to check errors)
		BOOL CreateDatabase(
			const CString &sFileName,	//The filename of the new database (must contain '.mdb' extension)
			BOOL bConfigDataSource = false,	//Configures a datasource for the just-created datasource
			const CString &sDSN = "",	//The data source name (e.g. "Payroll")
			BOOL bConnectAfter = false, //Connects to the data source you just configured
			const CString &sDescription = "", //Optional: description of data source
			BOOL bReadOnly = false, //Optional: read only access
			BOOL bExclusive = false); //Optional: one user at a time access

		//For connecting to data sources
		//Returns: TRUE if successful, false if not (you can view the last return to check errors)
		BOOL Connect(
			const CString &sDSN,		//The data source name (e.g. "Payroll")
			const CString &sID = "",	//User ID for the database
			const CString &sPass = "", //Password for the database
			BOOL bExclusive = false,	//Exclusive access
			BOOL bReadOnly = false);	//Read only access

		//For connecting to data sources (this is the alternative method which
		//displays the ODBC connect dialog)
		//Returns TRUE if successful, false if not
		BOOL Connect();

		//For disconnecting from data sources
		//Returns: void
		void Disconnect();

		//For checking whether connected to a database:
		BOOL IsConnected() { return m_bDBOpen; }

		// ** SQL COMMAND SUPPORT **

		//For beginning transactions
		//Returns: TRUE if successful, false if the database/driver/etc does not support them
		//			or if BeginTrans failed
		BOOL StartTransaction();

		//For ending transactions (locking down changes)
		//Returns: TRUE if successful, false if not (automatically rolls back if false)
		BOOL EndTransaction();

		//For sending SQL statements (note: only use after calling start transaction, you can
		//use it anyway if start transaction returned false)
		//Returns TRUE if success, false if otherwise (errors are examinable from GetLastErrMsg)
		BOOL SendSQL(const CString &sSQL);

		//For rolling back changes executed by SQL statements:
		//Returns TRUE if successful, false if not
		BOOL Rollback();

		//For getting whether or not the database is in a transaction:
		//Returns: TRUE if in trans, false if not
		BOOL IsInTrans() { return m_bInTrans; }

		// ** GETTING AT THE DATABASE **

		//For getting a pointer to the database:
		//Returns: pointer to the database object:
		CDatabase* GetCDatabase() { return &myDatabase; }

		// ** RECORDSET METHODS **

		//For opening a new recordset: returns true if it worked, false if not
		//You can check the error messages if it returns false.
		BOOL OpenNewSet(const CString &sFriendlyName,			//the name by which to reference this set
						const CString &sSQL = _T(""),			//the SQL statement to execute for the set
						const CString &sTableName = _T(""));	//OR the table name

		//For closing a recordset, pass it the friendly name you used
		//when opening it:
		BOOL CloseSet(const CString &sFriendlyName);		//the name of the set (assigned on opening)

		//For closing all open recordsets:
		BOOL CloseAllSets();

		//For getting at a recordset:
		BOOL GetSet(const CString &sFriendlyName,			//the name of the set (assigned on opening)
					CRecordsetEx *&dyna);			//the pointer to recieve the set address
		CRecordsetEx* GetSet(const CString &sFriendlyName);

		//For getting the string array list of friendly names for open sets:
		void GetNamesOfOpenSets(CStringArray &array);

		//For getting the number of managed sets:
		int GetNumSets() { return mySets.GetCount(); }

		//For opening up a set of records which enumerates the TABLES in the
		//open database
		BOOL QueryOpenTables();

		//For getting at the set of open tables:
		BOOL GetTables(CTables *&table);			//the pointer to recieve the set address
		CTables* GetTables();

		//For opening up a set of records which enumerates the columns in a
		//specified table
		BOOL QueryOpenColumns(const CString &sTableName);	//the name of the table to get columns from

		//For getting at the set of open columns:
		BOOL GetColumns(CColumns *&cols);			//the pointer to reviece the address
		CColumns* GetColumns();

		// ** ERROR ANALYSIS **

		//For getting the last return code (integer):
		int GetLastErrCode() { return m_nLastReturn; }

		//For getting the string explaining the last error code:
		const CString & GetLastErrMsg() { return m_sLastErrorMsg; }

	//PRIVATE to the class:
	private:

		// ** MEMBER VARIABLES **

		// --> database handling:
		CDatabase myDatabase;	//The all important database object

		// --> handling recordsets (note, there is a max of 10 open sets)
		CMapStringToPtr mySets;

		// --> handling tables or columns in a database:
		CTables *myTables;
		CColumns *myColumns;

		// --> settings:
		int m_nLoginTimeout, m_nQueryTimeout; //See above for explaination

		// --> error analyzation:
		UINT m_nLastReturn;
		CString m_sLastErrorMsg;

		// --> internals:
		BOOL m_bDBOpen, m_bSupportTrans, m_bInTrans;

		// ** HELPER FUNCTIONS **

		// --> quick setting:
		void SetSuccessMsg() { m_nLastReturn = RETURN_SUCCESS; m_sLastErrorMsg = "Success!"; }
};

#endif // !defined(AFX_ODBCACCESS_H__21B055FF_4BC2_4987_A5E3_95DBA6BCC1E7__INCLUDED_)
