#if !defined(AFX_ODBCDATABASESELECTIONLISTCTRL_H__3119B7CB_D9CA_4644_9E11_A509FB5F639F__INCLUDED_)
#define AFX_ODBCDATABASESELECTIONLISTCTRL_H__3119B7CB_D9CA_4644_9E11_A509FB5F639F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OdbcDatabaseSelectionListCtrl.h : header file
//

#include "OdbcStruct.h"

#include "..\Shared\InputListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// COdbcDatabaseSelectionListCtrl window

class COdbcDatabaseSelectionListCtrl : public CInputListCtrl 
{
// Construction
public:
	COdbcDatabaseSelectionListCtrl();
	COdbcDatabaseSelectionListCtrl(const COdbcDatabaseSetupArray& aDbSetup);

	BOOL Initialize();
	BOOL Initialize(const COdbcDatabaseSetupArray& aDbSetup);

	BOOL AddDatabaseSetup(const ODBCDATABASESETUP& dbSetup, BOOL bDoSetup = TRUE);
	int GetDatabaseSetups(COdbcDatabaseSetupArray& aDbSetup) const;

	BOOL GetSelectedDatabaseSetup(ODBCDATABASESETUP& dbSetup) const;
	BOOL DeleteSelectedDatabaseSetup();

	BOOL SetSelectedDatabaseModifiers(const CString& sTasklistID, const CString& sAllocToID);

// Attributes
protected:
	COdbcDatabaseSetupArray m_aDbSetup;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COdbcDatabaseSelectionListCtrl)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COdbcDatabaseSelectionListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(COdbcDatabaseSelectionListCtrl)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	virtual void EditCell(int nItem, int nCol);
	CString FormatUniqueName(const ODBCDATABASESETUP& dbSetup) const;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ODBCDATABASESELECTIONLISTCTRL_H__3119B7CB_D9CA_4644_9E11_A509FB5F639F__INCLUDED_)
