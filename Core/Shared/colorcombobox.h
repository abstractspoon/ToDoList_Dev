#if !defined(AFX_COLORCOMBOBOX_H__47DE226A_7C73_48BA_AE5B_E43B90D752A9__INCLUDED_)
#define AFX_COLORCOMBOBOX_H__47DE226A_7C73_48BA_AE5B_E43B90D752A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// colorcombobox.h : header file
//

#include "autocombobox.h"

/////////////////////////////////////////////////////////////////////////////


enum 
{
	CCBS_ROUNDRECT		= 0x01,
	CCBS_DRAWNOCOLOR	= 0x02,
};

/////////////////////////////////////////////////////////////////////////////
// CColorComboBox window

class CColorComboBox : public CAutoComboBox
{
// Construction
public:
	CColorComboBox(BOOL dwFlags = FALSE, DWORD dwAutoComboFlags = 0);
	virtual ~CColorComboBox();

	int AddColor(COLORREF color, LPCTSTR szDescription = NULL);
	int InsertColor(int nIndex, COLORREF color, LPCTSTR szDescription = NULL);

	COLORREF SetColor(int nIndex, COLORREF color);
	COLORREF SetColor(LPCTSTR szDescription, COLORREF color);

// Attributes
protected:
	DWORD m_dwFlags;

protected:
	DECLARE_MESSAGE_MAP()

protected:
	virtual DWORD GetNewItemData() const { return CLR_NONE; }
	virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState, 
								DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText);	

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORCOMBOBOX_H__47DE226A_7C73_48BA_AE5B_E43B90D752A9__INCLUDED_)
