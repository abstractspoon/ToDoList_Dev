
#ifndef _RSVARCOL_H_
#define _RSVARCOL_H_ 

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxdb.h>
#include <afxtempl.h>
// 
// rsvarcol.h : header file 
// 
//****************************************************************** 
// PROJECT NAME:	SQLPRC                                          
// MODULE NAME:		RSVARCOL.H                                      
// AUTHOR:			Phillip N. Jacobs 
// DESCRIPTION:		Variable-column recordset class definition 
// HISTORY: 
//	DATE	INITIALS	DESCRIPTION 
//	----	--------	-------------------------------------------- 
// 08/22/95		PNJ		Initial version. 
// 06/02/97		MJG		Port to VC++ 4.2/5.0 
//****************************************************************** 
 
///////////////////////////////////////////////////////////////////////////// 
// CVarRecordset recordset 

struct VARCOLUMNDEF
{
	WORD wType;			// Type of each column 
	WORD wScale;		// Scale of each column 
	BOOL bNullable;		// Nullable flag of each column 
	DWORD dwPrecision;	// Precision of each column 
	CString sName;	// Name of each column 
	CString sType;	// Type name of each column 
	CString sResult;	// Current row's results 
};
 
class CVarRecordset : public CRecordset 
{ 
public: 
	CVarRecordset(CDatabase* pdb = NULL);	//Constructor 
	DECLARE_DYNAMIC(CVarRecordset) 
	 
// Interface 
public: 
	virtual BOOL Open(UINT nOpenType = AFX_DB_USE_DEFAULT_TYPE,
					LPCTSTR lpszSQL = NULL, DWORD dwOptions = none);

	int		GetNumColumns() const;				// Number of columns 

	WORD	GetColumnType(int nCol) const;		// Column type 
	WORD	GetColumnScale(int nCol) const;		// Column scale 
	BOOL	IsColumnNullable(int nCol) const;	// Column nullable flag 
	DWORD	GetColumnPrecision(int nCol) const;	// Column precision 
	CString GetColumnName(int nCol) const;		// Column name 
	CString GetColumnTypeName(int nCol) const;	// Column type name 
	CString GetColumnResult(int nCol) const;	// Column result 

	BOOL	GetColumnDefinition(int nCol, VARCOLUMNDEF& colDef) const;
 
// CRecordset pure virtual functions 
	virtual CString GetDefaultConnect();	// Default connection string 
	virtual CString GetDefaultSQL(); 	// Default SQL for Recordset 
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX support 
 
// Overrides 
public: 
	virtual void PreBindFields();	// To obtain column attributes 
	virtual void Move(long lRows, WORD wFetchType);	// To fetch column results 
 
// Implementation 
#ifdef _DEBUG 
	virtual void AssertValid() const; 
	virtual void Dump(CDumpContext& dc) const; 
#endif 
 
private: 
	int m_nNumCols;	// Number of columns in the result set 
	CArray<VARCOLUMNDEF, VARCOLUMNDEF&> m_aColumnDefs;
	CString m_sDefaultQuery;
}; 
#endif // _RSVARCOL_H_ 
