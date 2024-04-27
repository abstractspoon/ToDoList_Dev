#if !defined(AFX_TDATTRIBLISTBOX_H__C5D9EFCE_7B59_498F_88E4_890286A344A5__INCLUDED_)
#define AFX_TDATTRIBLISTBOX_H__C5D9EFCE_7B59_498F_88E4_890286A344A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDAttribListBox.h : header file
//

#include "tdcstruct.h"

#include "..\Shared\checklistboxex.h"
#include "..\Shared\enstring.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

class CTDCCustomAttribDefinitionArray;

/////////////////////////////////////////////////////////////////////////////
// CTDLAttribListBox window

class CTDLAttributeListBox : public CCheckListBoxEx
{
// Construction
public:
	CTDLAttributeListBox(const CTDCCustomAttribDefinitionArray& aAttribDefs, BOOL bIncludeReadonly = TRUE);

	void SetSelectedAttributes(const CTDCAttributeMap& mapAttrib, const CStringSet& mapCustomAttribIDs);

	int GetSelectedAttributes(CTDCAttributeMap& mapAttrib, CStringSet& mapCustomAttribIDs) const;
	int GetSelectedAttributes(CTDCAttributeMap& mapAttrib, BOOL bIncCustAttrib = TRUE) const;

	int GetAvailableAttributes(CTDCAttributeMap& mapAttrib, BOOL bIncCustAttrib = TRUE) const;

// Attributes
protected:
	struct ATTRIBVIS
	{
		ATTRIBVIS() : nAttributeID(TDCA_NONE), bVisible(FALSE) {}
		ATTRIBVIS(UINT nIDName, TDC_ATTRIBUTE nAttribID, BOOL bVis) 
		{ 
			sName.LoadString(nIDName); 
			nAttributeID = nAttribID;
			bVisible = bVis; 
		}

		CEnString sName;
		CString sCustAttribID;
		TDC_ATTRIBUTE nAttributeID;
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

	int FindAttribute(TDC_ATTRIBUTE nAttribID) const;
	int GetAttributes(CTDCAttributeMap& mapAttrib, BOOL bSelected, BOOL bIncCustAttrib) const;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDATTRIBLISTBOX_H__C5D9EFCE_7B59_498F_88E4_890286A344A5__INCLUDED_)
