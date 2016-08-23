//By: George Polouse

// Tables.h: interface for the CTableFields class.
//

#if !defined(AFX_TABLECOLUMNS_H__B429EFC4_93B3_11D2_BD3E_204C4F4F5020__INCLUDED_)
#define AFX_TABLECOLUMNS_H__B429EFC4_93B3_11D2_BD3E_204C4F4F5020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTableFields - results from ::SQLTables()
#include "RecordsetEx.h"

class CTableFields : public CRecordsetEx
{
public:
	CTableFields(CDatabase* pDatabase);
	DECLARE_DYNAMIC(CTableFields)

// Operations
	BOOL Open(LPCTSTR pszTableName, UINT nOpenType = forwardOnly);

// Overrides
//	virtual CString GetDefaultConnect();
//	virtual CString GetDefaultSQL();
//	virtual void DoFieldExchange(CFieldExchange*);

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

#endif // !defined(AFX_TABLECOLUMNS_H__B429EFC4_93B3_11D2_BD3E_204C4F4F5020__INCLUDED_)