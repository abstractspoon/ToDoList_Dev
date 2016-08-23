#if !defined(AFX_TDCOLUMNLISTBOX_H__C5D9EFCE_7B59_498F_88E4_890286A344A5__INCLUDED_)
#define AFX_TDCOLUMNLISTBOX_H__C5D9EFCE_7B59_498F_88E4_890286A344A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDColumnListBox.h : header file
//

#include "tdcenum.h"
#include "tdcenumcontainers.h"

#include "..\Shared\checklistboxex.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CTDLColumnListBox window


class CTDLColumnListBox : public CCheckListBoxEx
{
// Construction
public:
	CTDLColumnListBox();

	void SetAllColumnsVisible(BOOL bVisible = TRUE);
	int GetAllColumns(CTDCColumnIDArray& aColumns) const;

	void SetColumnVisible(TDC_COLUMN nCol, BOOL bVisible = TRUE);
	void SetVisibleColumns(const CTDCColumnIDArray& aColumns);
	int GetVisibleColumns(CTDCColumnIDArray& aColumns) const;

// Attributes
protected:
	struct COLUMNVIS
	{
		COLUMNVIS() : nTDCCol(TDCC_NONE), bVisible(FALSE) {}
		COLUMNVIS(UINT nIDName, TDC_COLUMN tdcCol, BOOL bVis) 
		{ 
			sName.LoadString(nIDName); 
			nTDCCol = tdcCol;
			bVisible = bVis; 
		}

		CString sName;
		TDC_COLUMN nTDCCol;
		BOOL bVisible;
	};
	CArray<COLUMNVIS, COLUMNVIS&> m_aColumns;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLColumnListBox)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTDLColumnListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDLColumnListBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg BOOL OnReflectCheckChange();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor); // relected
	DECLARE_MESSAGE_MAP()

	int FindColumn(TDC_COLUMN nCol) const;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDCOLUMNLISTBOX_H__C5D9EFCE_7B59_498F_88E4_890286A344A5__INCLUDED_)
