//By: George Polouse

#if !defined(AFX_RECORDSETEX_H__43D82455_75A6_11D2_BD7C_204C4F4F5020__INCLUDED_)
#define AFX_RECORDSETEX_H__43D82455_75A6_11D2_BD7C_204C4F4F5020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecordsetEx.h : header file
//

#include "mapex.h"

#include <afxdb.h>
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

typedef CMap<long, long, CString, CString&> CMapIDToString;
typedef CMap<CString, LPCTSTR, long, long&> CMapStringToID;
typedef CArray<CODBCFieldInfo, CODBCFieldInfo&> CODBCFieldInfoArray;

typedef __int64 time64_t;

/////////////////////////////////////////////////////////////////////////////
// CRecordsetEx recordset

class CRecordsetEx : public CRecordset
{
public:
	CRecordsetEx(CDatabase* pDatabase = NULL);
	virtual ~CRecordsetEx();

	DECLARE_DYNAMIC(CRecordsetEx)

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecordsetEx)
	public:
	virtual BOOL Open(UINT nOpenType = forwardOnly, LPCTSTR lpszSql = NULL, DWORD dwOptions = none);
	//}}AFX_VIRTUAL

	void AllocAndCacheFieldInfo();
	BOOL ExecDirect(const CString& sSQL, UINT nOpenType = forwardOnly);

	BOOL GetFieldValue(short nIndex, CString& strValue);
	CString GetFieldValue(short nIndex);

	BOOL GetFieldValue(const CString& sName, CString& strValue);
	CString GetFieldValue(const CString& sName);

	BOOL GetFieldValue(const CString& sName, CDBVariant& varValue,
		short nFieldType = DEFAULT_FIELD_TYPE);

	BOOL GetFieldValue(short nIndex, CDBVariant& varValue,
		short nFieldType = DEFAULT_FIELD_TYPE);

	BOOL GetFieldValue(short nIndex, COleDateTime& dtVal);
	BOOL GetFieldValue(short nIndex, double& dVal);
	BOOL GetFieldValue(short nIndex, long& nVal);
	BOOL GetFieldValue(short nField, time64_t& tVal);

	CString GetFieldName(short nIndex) const;
	SWORD GetFieldType(short nIndex) const;
	UDWORD GetFieldPrecision(short nIndex) const;
	SWORD GetFieldScale(short nIndex) const;
	SWORD GetFieldNullability(short nIndex) const;

	int GetAllFieldInfo(CODBCFieldInfoArray& aInfo) const;

	short GetFieldIndexByName(const CString& sName, BOOL bCaseSensitive = FALSE);

	static CString GetSQLDataType(SWORD nSQLType);

	static int BuildFieldMapping(CDatabase& db, LPCTSTR szTable, 
								LPCTSTR szField1, LPCTSTR szField2,
								CMapStringToString& mapFields,
								LPCTSTR szWhere = NULL);

	static int BuildFieldMapping(CDatabase& db, LPCTSTR szTable, 
								LPCTSTR szField1, LPCTSTR szField2,
								CMapStringToStringArray& mapFields,
								LPCTSTR szWhere = NULL);

	static int BuildFieldMapping(CDatabase& db, LPCTSTR szTable, 
								LPCTSTR szIDField, LPCTSTR szField2,
								CMapIDToString& mapFields,
								LPCTSTR szWhere = NULL);

	static int BuildFieldMapping(CDatabase& db, LPCTSTR szTable, 
								LPCTSTR szField1, LPCTSTR szIDField, 
								CMapStringToID& mapFields,
								LPCTSTR szWhere = NULL);

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
private:
	static int PASCAL GetTextLen(short nSQLType, UDWORD nPrecision);
	static void PASCAL GetLongCharDataAndCleanup(CDatabase* pdb, HSTMT hstmt,
		short nFieldIndex, long nActualSize, LPVOID* ppvData, int nLen,
		CString& strValue, short nSQLType);
	
	static BOOL PrepareQuery(LPCTSTR szTable, LPCTSTR szField1, 
							LPCTSTR szField2, LPCTSTR szWhere, CString& sQuery);

protected:
	BOOL VerifyFieldIndex(short nIndex) const;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDSETEX_H__43D82455_75A6_11D2_BD7C_204C4F4F5020__INCLUDED_)