//By: George Polouse

// Tables.h: interface for the COdbcTables class.
//

#if !defined(AFX_ODBCTABLES_H__B429EFC4_93B3_11D2_BD3E_204C4F4F5020__INCLUDED_)
#define AFX_ODBCTABLES_H__B429EFC4_93B3_11D2_BD3E_204C4F4F5020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\shared\RecordsetEx.h"

/////////////////////////////////////////////////////////////////////////////
// COdbcTables - results from ::SQLTables()

class COdbcTables : public CRecordsetEx
{
public:
	COdbcTables(CDatabase* pDatabase);
	DECLARE_DYNAMIC(COdbcTables)

// Operations
	BOOL Open(LPCTSTR pszTableQualifier = NULL,
			 LPCTSTR pszTableOwner = NULL,
			 LPCTSTR pszTableName = NULL,
			 LPCTSTR pszTableType = _T("TABLE"),
			 UINT   nOpenType = forwardOnly);

	int GetTableNames(CStringArray& aTableNames);

	static int GetTableNames(CDatabase& db, CStringArray& aTableNames);

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CString GetTableQualifier() { return GetFieldValue(_T("TABLE_QUALIFIER")); }
	CString GetTableOwner() { return GetFieldValue(_T("TABLE_OWNER")); }
	CString GetTableName() { return GetFieldValue(_T("TABLE_NAME")); }
	CString GetTableType() { return GetFieldValue(_T("TABLE_TYPE")); }
	CString GetRemarks() { return GetFieldValue(_T("REMARKS")); }
};

#endif // !defined(AFX_ODBCTABLES_H__B429EFC4_93B3_11D2_BD3E_204C4F4F5020__INCLUDED_)