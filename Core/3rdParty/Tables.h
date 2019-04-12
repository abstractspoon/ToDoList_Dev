//By: George Polouse

// Tables.h: interface for the CTables class.
//

#if !defined(AFX_TABLES_H__B429EFC4_93B3_11D2_BD3E_204C4F4F5020__INCLUDED_)
#define AFX_TABLES_H__B429EFC4_93B3_11D2_BD3E_204C4F4F5020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RecordsetEx.h"

/////////////////////////////////////////////////////////////////////////////
// CTables - results from ::SQLTables()

class CTables : public CRecordsetEx
{
public:
	CTables(CDatabase* pDatabase);
	DECLARE_DYNAMIC(CTables)

// Operations
	BOOL Open(LPCTSTR pszTableQualifier = NULL,
			 LPCTSTR pszTableOwner = NULL,
			 LPCTSTR pszTableName = NULL,
			 LPCTSTR pszTableType = _T("TABLE"),
			 UINT   nOpenType = forwardOnly);

	CString GetTableQualifier() { return GetFieldValue(_T("TABLE_QUALIFIER")); }
	CString GetTableOwner() { return GetFieldValue(_T("TABLE_OWNER")); }
	CString GetTableName() { return GetFieldValue(_T("TABLE_NAME")); }
	CString GetTableType() { return GetFieldValue(_T("TABLE_TYPE")); }
	CString GetRemarks() { return GetFieldValue(_T("REMARKS")); }

	int GetTableNames(CStringArray& aTableNames);

	static int GetTableNames(CDatabase& db, CStringArray& aTableNames);

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

#endif // !defined(AFX_TABLES_H__B429EFC4_93B3_11D2_BD3E_204C4F4F5020__INCLUDED_)