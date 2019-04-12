/*********************************************************************

   Copyright (C) 2002 Smaller Animals Software, Inc.

   This software is provided 'as-is', without any express or implied
   warranty.  In no event will the authors be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.

   3. This notice may not be removed or altered from any source distribution.

   http://www.smalleranimals.com
   smallest@smalleranimals.com

   --------

   This code is based, in part, on:
   "A WTL-based Font preview combo box", Ramon Smits
   http://www.codeproject.com/wtl/rsprevfontcmb.asp

**********************************************************************/

#if !defined(AFX_FONTPREVIEWCOMBO_H__3787F1C9_E55D_4F86_A3F2_2405B523A6DB__INCLUDED_)
#define AFX_FONTPREVIEWCOMBO_H__3787F1C9_E55D_4F86_A3F2_2405B523A6DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FontPreviewCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFontPreviewCombo window

	typedef enum
	{
		FPC_NAME_ONLY = 0,		// font name, drawn in font
		FPC_NAME_GUI_FONT,		// font name, drawn in GUI font
		FPC_NAME_THEN_SAMPLE,	// font name in GUI font, then sample text in font
		FPC_SAMPLE_THEN_NAME,	// sample text in font, then font name in GUI font
		FPC_SAMPLE_ONLY			// sample text in font
	} PreviewStyle;

class CFontPreviewCombo : public CComboBox
{
// Construction
public:
	CFontPreviewCombo(PreviewStyle style = FPC_NAME_THEN_SAMPLE, LPCTSTR szSample = "abcdeABCDE",
						COLORREF clrSample = RGB(60,0,0), int nFontHeight = -1);

// Operations
public:
	
	// call this to load the font strings
	void	Init();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFontPreviewCombo)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL
	virtual void PreSubclassWindow() { Init(); };

// Implementation
public:
	virtual ~CFontPreviewCombo();
	
protected:
	CImageList m_img;	

	int m_iMaxNameWidth;
	int m_iMaxSampleWidth;
	CString m_csSample;
	COLORREF m_clrSample;
	PreviewStyle m_style;
	int m_iFontHeight;

	// Generated message map functions
	//{{AFX_MSG(CFontPreviewCombo)
	afx_msg void OnDropdown();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FONTPREVIEWCOMBO_H__3787F1C9_E55D_4F86_A3F2_2405B523A6DB__INCLUDED_)
