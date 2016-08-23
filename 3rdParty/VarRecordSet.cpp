// 
//****************************************************************** 
// PROJECT NAME:	SQLPRC                                          
// MODULE NAME:		RSVARCOL.CPP                                      
// AUTHOR:			Phillip N. Jacobs 
// DESCRIPTION:		Variable-column recordset class implementation 
/* 
	CVarRecordset is derived from CRecordset and is implemented so 
	that you can either derive your own classes from it (for 
	example, if you want to bind parameters) or create CVarRecordset 
	objects and use it as is. 
	 
	I implemented the code to determine the number of result columns 
	and their attributes in an override of PreBindFields because 
	this only needs to be done once.  This means that if you derive 
	your own class from CVarRecordset and you want to override 
	PreBindFields you MUST call CVarRecordset::PreBindFields 
	function from within your override. 
 
	!!!IMPORTANT!!!	 
	You MUST call the Open function with the first parameter 
	(nOpenType) equal to CRecordset::forwardOnly. 
	 
	CVarRecordset supports all the SQL statements that CRecordset 
	does except for table names.  For example, if you had a 
	CRecordset with an SQL statement of "TableName" you would need 
	to change it to "SELECT * FROM TableName" in order to use a 
	CVarRecordset.  This is because the MFC code requires that at  
	least one result field be bound if plain table names are used. 
	Keep in mind that MFC enforces a strict syntax for stored 
	procedures which requires that the SQL statement be enclosed in 
	curly braces and that the “call” keyword be used.  For example, 
	"{call sAceTrOverF_AU 8182,’3/25/95’}". 
*/ 
// HISTORY: 
//	DATE	INITIALS	DESCRIPTION 
//	----	--------	-------------------------------------------- 
// 08/22/95		PNJ		Initial version. 
// 06/02/97     MJG     Port to version VC++ 4.2/5.0 
//****************************************************************** 
 
//Header files 
#include "stdafx.h" 
#include "VarRecordSet.h" 
 
#define MAX_COLNAME 128 // Maximum column name length 
 
#ifdef _DEBUG 
#undef THIS_FILE 
static char BASED_CODE THIS_FILE[] = __FILE__; 
#endif 

///////////////////////////////////////////////////////////////////////////// 

#define VALIDATECOL(col, ret) \
ASSERT (col >= 0 && col < m_nNumCols); \
if (col < 0 || col >= m_nNumCols) return ret;

///////////////////////////////////////////////////////////////////////////// 
// CVarRecordset 
 
IMPLEMENT_DYNAMIC(CVarRecordset, CRecordset) 
 
// Interface 
 
//****************************************************************** 
// NAME:	CVarRecordset 
// PARAMETERS:	(1) Pointer to CDatabase object 
// RETURNS:	N/A 
// DESCRIPTION:	Constructor 
//****************************************************************** 
CVarRecordset::CVarRecordset(CDatabase* pdb) 
	: 	CRecordset(pdb),	// Superclass constructor 
		m_nNumCols(0)			// Initialize number of columns to zero 
{ 
} 
 
BOOL CVarRecordset::Open(UINT nOpenType, LPCTSTR lpszSQL, DWORD dwOptions)
{
	m_sDefaultQuery = lpszSQL;

	return CRecordset::Open(forwardOnly, lpszSQL, dwOptions);
}

//****************************************************************** 
// NAME:	GetNumColumns 
// PARAMETERS:	N/A 
// RETURNS:	Number of columns in the Recordset 
// DESCRIPTION:	Only meaningful after calling Open 
//****************************************************************** 
int CVarRecordset::GetNumColumns() const
{ 
	return m_nNumCols; 
} 
 
BOOL CVarRecordset::GetColumnDefinition(int nCol, VARCOLUMNDEF& colDef) const
{
	VALIDATECOL(nCol, FALSE);

	colDef = m_aColumnDefs[nCol];
	return TRUE;
}

//****************************************************************** 
// NAME:	GetColumnName 
// PARAMETERS:	(1) Zero-based column number 
// RETURNS:	Column name or empty string is column is unnamed 
// DESCRIPTION:	Only meaningful after calling Open 
//****************************************************************** 
CString CVarRecordset::GetColumnName(int nCol) const 
{ 
	VALIDATECOL(nCol, _T(""));

	return m_aColumnDefs.GetData()[nCol].sName; 
} 
 
//****************************************************************** 
// NAME:	GetColumnType 
// PARAMETERS:	(1) Zero-based column number 
// RETURNS:	ODBC or driver-specific SQL data type 
// DESCRIPTION:	Only meaningful after calling Open 
//****************************************************************** 
WORD CVarRecordset::GetColumnType(int nCol) const 
{ 
	VALIDATECOL(nCol, 0);

	return m_aColumnDefs.GetData()[nCol].wType; 
} 
 
//****************************************************************** 
// NAME:	GetColumnTypeName 
// PARAMETERS:	(1) Zero-based column number 
// RETURNS:	Data source-dependent data type name; for example, 
//		"CHAR", "VARCHAR", "MONEY", "TIMESTAMP", etc. 
// DESCRIPTION:	Only meaningful after calling Open 
//****************************************************************** 
CString CVarRecordset::GetColumnTypeName(int nCol) const 
{ 
	VALIDATECOL(nCol, _T(""));

	return m_aColumnDefs.GetData()[nCol].sType; 
} 
 
//****************************************************************** 
// NAME:	GetColumnPrecision 
// PARAMETERS:	(1) Zero-based column number 
// RETURNS:	Column precision; the precision of a numeric column 
//		refers to the maximum number of digits used by its data 
//		type; the precision of a nonnumeric column generally refers 
//		to either the maximum length or the defined length of the 
//		column. 
// DESCRIPTION:	Only meaningful after calling Open 
//****************************************************************** 
DWORD CVarRecordset::GetColumnPrecision(int nCol) const 
{ 
	VALIDATECOL(nCol, 0);

	return m_aColumnDefs.GetData()[nCol].dwPrecision; 
} 
 
//****************************************************************** 
// NAME:	GetColumnScale 
// PARAMETERS:	(1) Zero-based column number 
// RETURNS:	Column scale; the scale of a numeric column 
//		refers to the maximum number of digits to the right of the 
//		decimal point; it is undefined for approximate floating 
//		point number columns or nonnumeric columns. 
// DESCRIPTION:	Only meaningful after calling Open 
//****************************************************************** 
WORD CVarRecordset::GetColumnScale(int nCol) const 
{ 
	VALIDATECOL(nCol, 0);

	return m_aColumnDefs.GetData()[nCol].wScale; 
} 
 
//****************************************************************** 
// NAME:	IsColumnNullable 
// PARAMETERS:	(1) Zero-based column number 
// RETURNS:	SQL_NO_NULLS if the column does not accept NULL values; 
//			SQL_NULLABLE if the column accepts NULL values; 
//			SQL_NULLABLE_UNKNOWN if it is not known. 
// DESCRIPTION:	Only meaningful after calling Open 
//****************************************************************** 
BOOL CVarRecordset::IsColumnNullable(int nCol) const 
{ 
	VALIDATECOL(nCol, FALSE);

	return m_aColumnDefs.GetData()[nCol].bNullable; 
} 
 
//****************************************************************** 
// NAME:	GetColumnResult 
// PARAMETERS:	(1) Zero-based column number 
// RETURNS:	ASCII representation of the result value 
// DESCRIPTION:	This function may be called after calling Open to 
//		obtain results for columns in the first row, and it may be 
//		called after calling MoveNext to obtain results for columns 
//		in successive rows.  Use the GetColumnType function to 
//		determine the column's data type if it is necessary to 
//		convert the ASCII representation into the natural format. 
//****************************************************************** 
CString CVarRecordset::GetColumnResult(int nCol) const 
{ 
	VALIDATECOL(nCol, _T(""));

	return m_aColumnDefs.GetData()[nCol].sResult; 
} 
 
// Implementation of CRecordset pure virtual functions 
 
//****************************************************************** 
// NAME:	GetDefaultConnect 
// PARAMETERS:	N/A 
// RETURNS:	Default data source connection string 
// DESCRIPTION:	This function must be implemented in CVarRecordset 
//		in order to be able to declare objects of this class. 
//****************************************************************** 
CString CVarRecordset::GetDefaultConnect() 
{ 
	return ""; 
} 
 
//****************************************************************** 
// NAME:	GetDefaultSQL 
// PARAMETERS:	N/A 
// RETURNS:	Default SQL statement to be used if the corresponding 
//		parameter in the Open function is NULL. 
// DESCRIPTION:	This function must be implemented in CVarRecordset 
//		in order to be able to declare objects of this class. 
//****************************************************************** 
CString CVarRecordset::GetDefaultSQL() 
{ 
	return m_sDefaultQuery; 
} 
 
//****************************************************************** 
// NAME:	DoFieldExchange 
// PARAMETERS:	Pointer to CFieldExchange object 
// RETURNS:	N/A 
// DESCRIPTION:	This function must be implemented in CVarRecordset 
//		in order to be able to declare objects of this class. 
//		The MFC implementation of CRecordset assumes that at least 
//		one result column is "bound" as a field.  I implemented a 
//		kludge to get around this. 
//****************************************************************** 
void CVarRecordset::DoFieldExchange(CFieldExchange* pFX) 
{ 
	//{{AFX_FIELD_MAP(CVarRecordset) 
	//}}AFX_FIELD_MAP 
	//Following Line commented out by MJG 06/02/97 
//	pFX->m_bFieldFound = TRUE;	// kludge 
	pFX->m_bField = TRUE; //kludge -- MJG 06/02/97 
 
} 
 
// Implementation 
 
//****************************************************************** 
// NAME:	PreBindFields 
// PARAMETERS:	N/A 
// RETURNS:	N/A 
// DESCRIPTION:	This function is called by MFC's implementation of 
//		the CRecordset::Open function immediately after executing 
//		the query but before fetching the data.  It is therefore the 
//		perfect place to put the code to determine the number of 
//		result columns and their attributes. 
// 
//		This function does not call CRecordset::PreBindFields 
//		because there are no fields to be bound. 
// 
//		NOTE!!! If you derive your own class from CVarRecordset, and 
//		you want to override PreBindFields then you MUST call 
//		CVarRecordset::PreBindFields from within your override. 
//****************************************************************** 
void CVarRecordset::PreBindFields() 
{ 
	// Determine the number of columns	 
	short nNumCols;
	RETCODE nRetCode = ::SQLNumResultCols(m_hstmt, &nNumCols); 

	if (Check(nRetCode)) 
	{
		m_nNumCols = nNumCols;

		// Size the column descriptor arrays
		m_aColumnDefs.SetSize(m_nNumCols); 
 
		// Get the column descriptor information for each column. 
		// Note that in CVarRecordset references to columns are 
		// zero-based; however, in ODBC they are one-based (column 
		// zero has a special meaning in ODBC).	 
		char szColName[MAX_COLNAME+1];	// column name 
		char szTypeName[MAX_COLNAME+1];	// column type name 
		SWORD cbColName;  // total number of bytes returned in szColName 
		SWORD fSQLType;	  // SQL data type 
		UDWORD cbPrec;    // column precision 
		SWORD cbTypeName; // total number of bytes returned in szTypeName 
		SWORD cbScale;    // column scale 
		SWORD fNullable;  // column nullable flag 
 
		for (int i = 0; i < m_nNumCols; i++) 
		{ 
			nRetCode = ::SQLDescribeCol(m_hstmt, 
										i+1, 
										(UCHAR*)szColName, 
										MAX_COLNAME, 
										&cbColName, 
										&fSQLType, 
										&cbPrec, 
										&cbScale, 
										&fNullable); 

			if (!Check(nRetCode))
				break;

			nRetCode = ::SQLColAttributes(m_hstmt, 
										i+1, 
										SQL_COLUMN_TYPE_NAME, 
										(UCHAR*)szTypeName, 
										sizeof(szTypeName), 
										&cbTypeName, 
										0); 

			if (!Check(nRetCode)) 
				break; 

			VARCOLUMNDEF& colDef = m_aColumnDefs[i];

			colDef.wType = fSQLType; 
			colDef.wScale = cbScale; 
			colDef.bNullable = fNullable; 
			colDef.dwPrecision = cbPrec; 
			colDef.sName = szColName; 
			colDef.sType = szTypeName; 
		} 
	}

	// handle failures
	if (!Check(nRetCode)) 
	{
		m_aColumnDefs.RemoveAll();
		m_nNumCols = 0;

		ThrowDBException(nRetCode); 
	}
} 
 
//****************************************************************** 
// NAME:	Move 
// PARAMETERS:	(1) Only valid value for CVarRecordset is 
//		AFX_MOVE_NEXT 
// RETURNS:	N/A 
// DESCRIPTION:	This function is called by MFC's implementation of 
//		the CRecordset::MoveNext function (and other related 
//		functions).  It is thus the perfect place to put the code to 
//		fetch the actual column results. 
// Mod Log 
// 
// MJG -- 06/02/97 -- Changed function definition to add wFetchType 
//                    in order to match MFC definition of Move. 
//                    Force value to SQL_FETCH_NEXT 
//****************************************************************** 
void CVarRecordset::Move(long lRows, WORD wFetchType) 
{ 
	RETCODE nRetCode;	// SQL function return code 
	char szData[MAX_COLNAME+1]; // SQL function data buffer 
	SDWORD cbData; // Total number of bytes returned in data buffer 
 
	//Force wFetchType = SQL_FETCH_NEXT -- MJG 
	wFetchType = SQL_FETCH_NEXT; 
	 
	// Row positioning. 
	// Only call the CRecordset::Move function if result columns 
	// are defined.  Will GPF if m_nNumCols == 0. 
	if (m_nNumCols > 0)	 
	{
		CRecordset::Move(lRows, wFetchType); 
	}
	else	// clean up - code is from MFC (DBCORE.CPP) 
	{ 
		//The following Line of Code commented out by MJG 06/02/97 
//		ReleaseCopyBuffer(); 
		m_bEOFSeen = m_bBOF = m_bEOF = TRUE; 
		m_bDeleted = FALSE; 
	} 
	 
	// Get the ASCII result value for each column. 
	for (int i = 0; i < m_nNumCols && !IsEOF(); i++) 
	{ 
		AFX_SQL_ASYNC(this, 
					::SQLGetData(m_hstmt, 
								i+1, 
								SQL_C_CHAR, 
								(UCHAR*)szData, 
								MAX_COLNAME, 
								&cbData));
		
		if (!Check(nRetCode)) 
			ThrowDBException(nRetCode); 
		 
		// remove trailing blanks 
		VARCOLUMNDEF& colDef = m_aColumnDefs[i];

		colDef.sResult = szData; 
		colDef.sResult.TrimRight();
	} 
} 
 
///////////////////////////////////////////////////////////////////////////// 
// CVarRecordset diagnostics 
 
#ifdef _DEBUG 
void CVarRecordset::AssertValid() const 
{ 
	CRecordset::AssertValid(); 
} 
 
void CVarRecordset::Dump(CDumpContext& dc) const 
{ 
	CRecordset::Dump(dc); 
} 
#endif //_DEBUG 
