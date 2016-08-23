#if !defined(AFX_CONTENTTYPECOMBOBOX_H__87AB439E_A5C6_4486_9985_0B5DC1D0F530__INCLUDED_)
#define AFX_CONTENTTYPECOMBOBOX_H__87AB439E_A5C6_4486_9985_0B5DC1D0F530__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// contenttypecombobox.h : header file
//

#include "..\shared\ownerdrawcomboboxbase.h"

/////////////////////////////////////////////////////////////////////////////
// CContentTypeComboBox window

class CContentMgr;
class CONTENTFORMAT;

class CTDLContentTypeComboBox : public COwnerdrawComboBoxBase
{
// Construction
public:
	CTDLContentTypeComboBox(const CContentMgr* pContentMgr);

	int GetSelectedFormat(CONTENTFORMAT& cf) const;
	int SetSelectedFormat(LPCTSTR szTypeID);
	void SetCurSel(int nSel);
	int GetCurSel() const;	

	int GetCount() const;
	BOOL IsInitialized() const { return (m_pContentMgr != NULL); }

// Attributes
protected:
	const CContentMgr* m_pContentMgr;
	int m_nInitSel;
	CImageList m_ilContent;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContentTypeComboBox)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTDLContentTypeComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CContentTypeComboBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	void FillCombo();
	int AddItemImage(HICON hIcon, BOOL bDeleteIcon = TRUE);
	void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
						DWORD dwItemData, const CString& sItem, BOOL bList);	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTENTTYPECOMBOBOX_H__87AB439E_A5C6_4486_9985_0B5DC1D0F530__INCLUDED_)
