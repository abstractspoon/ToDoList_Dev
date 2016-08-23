#if !defined(AFX_TDLCUSTOMATTRIBFEATURECOMBOBOX_H__8AE72797_0FAF_4812_B789_44808C2467EA__INCLUDED_)
#define AFX_TDLCUSTOMATTRIBFEATURECOMBOBOX_H__8AE72797_0FAF_4812_B789_44808C2467EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLCustomAttribFeatureComboBox.h : header file
//

#include "tdcstruct.h"
#include "..\shared\EnCheckComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLCustomAttribFeatureComboBox window

class CTDLCustomAttribFeatureComboBox : public CCheckComboBox
{
// Construction
public:
	CTDLCustomAttribFeatureComboBox();

	BOOL SetAttributeDefintion(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef);
	DWORD GetSelectedFeatures() const;
	CString FormatSelectedFeatures() const;

	static CString FormatFeatureList(DWORD dwFeatures);

protected:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLCustomAttribFeatureComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTDLCustomAttribFeatureComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDLCustomAttribFeatureComboBox)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void BuildCombo(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef);
	void OnCheckChange(int nIndex);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLCUSTOMATTRIBFEATURECOMBOBOX_H__8AE72797_0FAF_4812_B789_44808C2467EA__INCLUDED_)
