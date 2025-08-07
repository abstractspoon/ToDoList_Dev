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
public:
	CTDLCustomAttribFeatureComboBox();
	virtual ~CTDLCustomAttribFeatureComboBox();

	BOOL SetAttributeDefinition(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef);
	BOOL SetAttributeDefinition(DWORD dwDataType, DWORD dwListType, DWORD dwSelectedFeatures);
	DWORD GetSelectedFeatures() const;

	static CString FormatFeatureList(DWORD dwFeatures);

protected:
	DECLARE_MESSAGE_MAP()

protected:
	void BuildCombo(DWORD dwDataType, DWORD dwListType, DWORD dwSelectedFeatures);
	void OnCheckChange(int nIndex);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLCUSTOMATTRIBFEATURECOMBOBOX_H__8AE72797_0FAF_4812_B789_44808C2467EA__INCLUDED_)
