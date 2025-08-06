#if !defined(AFX_TDLFILEENCODINGCOMBOBOX_H__308505CA_F38B_4F5A_9463_E9DDBA2B5300__INCLUDED_)
#define AFX_TDLFILEENCODINGCOMBOBOX_H__308505CA_F38B_4F5A_9463_E9DDBA2B5300__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLFileEncodingComboBox.h : header file
//

#include "..\3rdParty\StdioFileEx.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLFileEncodingComboBox window

class CTDLFileEncodingComboBox : public CComboBox
{
// Construction
public:
	CTDLFileEncodingComboBox();
	virtual ~CTDLFileEncodingComboBox();

// Operations
public:
	BOOL SetSelectedEncoding(SFE_FORMAT nFormat);
	SFE_FORMAT GetSelectedEncoding() const;

protected:
	virtual void PreSubclassWindow();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	void BuildCombo();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLFILEENCODINGCOMBOBOX_H__308505CA_F38B_4F5A_9463_E9DDBA2B5300__INCLUDED_)
