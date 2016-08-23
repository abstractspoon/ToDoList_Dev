//By: George Polouse

// Columns.h: interface for the CColumns class.
//

#if !defined(AFX_COLUMNS_H__23260973_9781_11D2_BD48_204C4F4F5020__INCLUDED_)
#define AFX_COLUMNS_H__23260973_9781_11D2_BD48_204C4F4F5020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CColumns - results from ::SQLColumns()
#include "RecordsetEx.h"

class CColumns : public CRecordsetEx
{
public:
	CColumns(CDatabase* pDatabase);
	DECLARE_DYNAMIC(CColumns)

// Field Data
	CString m_strTableQualifier;        
	CString m_strTableOwner;        
	CString m_strTableName;        
	CString m_strColumnName;        
	int m_nDataType;        
	CString m_strTypeName;        
	long m_nPrecision;        
	long m_nLength;        
	int m_nScale;        
	int m_nRadix;        
	int m_fNullable;        
	CString m_strRemarks;        

// Operations
	BOOL Open(LPCSTR pszTableQualifier = NULL,
			 LPCSTR pszTableOwner = NULL,
			 LPCSTR pszTableName = NULL,
			 LPCSTR pszColumnName = NULL,
			 UINT   nOpenType = CRecordset::dynaset);
// Overrides
	virtual CString GetDefaultConnect();
	virtual CString GetDefaultSQL();
	virtual void DoFieldExchange(CFieldExchange*);

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

#endif // !defined(AFX_COLUMNS_H__23260973_9781_11D2_BD48_204C4F4F5020__INCLUDED_)