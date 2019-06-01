//By: George Polouse

// Tables.h: interface for the COdbcFields class.
//

#if !defined(AFX_TABLECOLUMNS_H__B429EFC4_93B3_11D2_BD3E_204C4F4F5020__INCLUDED_)
#define AFX_TABLECOLUMNS_H__B429EFC4_93B3_11D2_BD3E_204C4F4F5020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\shared\RecordsetEx.h"

/////////////////////////////////////////////////////////////////////////////

class COdbcFields : public CRecordsetEx
{
public:
	COdbcFields(CDatabase* pDatabase);
	DECLARE_DYNAMIC(COdbcFields)


// Operations
	BOOL Open(LPCTSTR pszTableName,
				LPCTSTR pszTableQualifier = NULL,
				LPCTSTR pszTableOwner = NULL,
				LPCTSTR pszTableType = NULL,
				UINT nOpenType = forwardOnly);

	int GetFieldNames(CStringArray& aFieldNames);

	static int GetFieldNames(CDatabase& db, const CString& sTableName, CStringArray& aFieldNames);

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CString GetFieldName() { return GetFieldValue(_T("COLUMN_NAME")); }
};

#endif // !defined(AFX_TABLECOLUMNS_H__B429EFC4_93B3_11D2_BD3E_204C4F4F5020__INCLUDED_)