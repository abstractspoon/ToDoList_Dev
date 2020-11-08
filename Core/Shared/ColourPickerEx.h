#if !defined(AFX_COLOURPICKEREX_H__ED8B58B5_F898_4295_999D_0017643D46C7__INCLUDED_)
#define AFX_COLOURPICKEREX_H__ED8B58B5_F898_4295_999D_0017643D46C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColourPickerEx.h : header file
//

#include "..\3rdParty\ColourPicker.h"

/////////////////////////////////////////////////////////////////////////////
// CColourPickerEx window

class CColourPickerEx : public CColourPicker
{
// Construction
public:
	CColourPickerEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColourPickerEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColourPickerEx();

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CColourPickerEx)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual CColourPopup* NewColourPopup(CPoint pt);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLOURPICKEREX_H__ED8B58B5_F898_4295_999D_0017643D46C7__INCLUDED_)
