#if !defined(AFX_TDLICONCOMBOBOX_H__0DAA207C_F8BC_4BDD_ADCB_E840D2C9A73F__INCLUDED_)
#define AFX_TDLICONCOMBOBOX_H__0DAA207C_F8BC_4BDD_ADCB_E840D2C9A73F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// tdliconcombobox.h : header file
//

#include "..\shared\encheckcombobox.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLIconComboBox window

class CTDCImageList;

class CTDLIconComboBox : public CEnCheckComboBox
{
	DECLARE_DYNAMIC(CTDLIconComboBox);

// Construction
public:
	CTDLIconComboBox(const CTDCImageList& ilImages, BOOL bMultiSel, BOOL bFilter);

	int SelectImage(const CString& sImage);
	CString GetSelectedImage() const;

	int SetStrings(const CStringArray& aItems);
	int GetItems(CStringArray& aItems) const; 

	int GetChecked(CStringArray& aItems, CCB_CHECKSTATE nCheck = CCBC_CHECKED) const;
	BOOL SetChecked(const CStringArray& aItems);
	BOOL SetChecked(const CStringArray& aChecked, const CStringArray& aMixed);

// Attributes
protected:
	const CTDCImageList& m_ilImages;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLIconComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTDLIconComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDLIconComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
								DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText);
	virtual BOOL HasIcon() const { return TRUE; }

	int EncodeImageTags(const CStringArray& aImages, CStringArray& aEncodedTags, BOOL bAdding) const;
	int DecodeImageTags(const CStringArray& aImages, CStringArray& aDecodedTags) const;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLICONCOMBOBOX_H__0DAA207C_F8BC_4BDD_ADCB_E840D2C9A73F__INCLUDED_)
