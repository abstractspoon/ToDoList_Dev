#if !defined(AFX_TDATTRIBLISTBOX_H__C5D9EFCE_7B59_498F_88E4_890286A344A5__INCLUDED_)
#define AFX_TDATTRIBLISTBOX_H__C5D9EFCE_7B59_498F_88E4_890286A344A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDAttribListBox.h : header file
//

#include "tdcenumcontainers.h"

#include "..\Shared\checklistboxex.h"
#include "..\Shared\enstring.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CTDLAttribListBox window

class CTDLAttributeListBox : public CCheckListBoxEx
{
// Construction
public:
	CTDLAttributeListBox();

	void SetAllAttributesVisible(BOOL bVisible = TRUE);
	int GetAllAttributes(CTDCAttributeMap& mapAttrib) const;

	void SetVisibleAttributes(const CTDCAttributeMap& mapAttrib);
	int GetVisibleAttributes(CTDCAttributeMap& mapAttrib) const;

// Attributes
protected:
	struct ATTRIBVIS
	{
		ATTRIBVIS() : nTDCAttrib(TDCA_NONE), bVisible(FALSE) {}
		ATTRIBVIS(UINT nIDName, TDC_ATTRIBUTE tdcAttrib, BOOL bVis) 
		{ 
			sName.LoadString(nIDName); 
			nTDCAttrib = tdcAttrib;
			bVisible = bVis; 
		}

		CEnString sName;
		TDC_ATTRIBUTE nTDCAttrib;
		BOOL bVisible;
	};
	CArray<ATTRIBVIS, ATTRIBVIS&> m_aAttribs;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLAttribListBox)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTDLAttributeListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDLAttribListBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg BOOL OnReflectCheckChange();
	afx_msg LRESULT OnInitListBox(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

	int FindAttribute(TDC_ATTRIBUTE nAttrib) const;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDATTRIBLISTBOX_H__C5D9EFCE_7B59_498F_88E4_890286A344A5__INCLUDED_)
