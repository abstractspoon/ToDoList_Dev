#if !defined(AFX_COLORBREWERCOMBOBOX_H__87AB439E_A5C6_4486_9985_0B5DC1D0F530__INCLUDED_)
#define AFX_COLORBREWERCOMBOBOX_H__87AB439E_A5C6_4486_9985_0B5DC1D0F530__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CColorBrewerComboBox.h : header file
//

#include "ownerdrawcomboboxbase.h"
#include "ColorBrewer.h"

/////////////////////////////////////////////////////////////////////////////
// CColorBrewerComboBox window

class CContentMgr;
class CONTENTFORMAT;

class CColorBrewerComboBox : public COwnerdrawComboBoxBase
{
public:
	CColorBrewerComboBox(DWORD dwBrewerFlags = 0, UINT nIDNoneString = 0);
	virtual ~CColorBrewerComboBox();

	BOOL Initialize();
	BOOL Initialize(COLORBREWER_PALETTETYPE nType);
	BOOL Initialize(COLORBREWER_PALETTETYPE nType, int nNumColors);
	BOOL Initialize(int nNumColors);
	BOOL Initialize(LPCTSTR szName, BOOL bPartial = TRUE);
	BOOL Initialize(const CColorBrewerPaletteArray& aPalettes);

	int GetSelectedPalette() const;
	int GetSelectedPalette(CColorBrewerPalette& palette) const;
	int SetSelectedPalette(const CColorBrewerPalette& palette);
	int SetSelectedPalette(int nPal);
	int GetSelectedPalette(CDWordArray& aColors) const;
	int SetSelectedPalette(const CDWordArray& aColors);

	void DDX(CDataExchange* pDX, CDWordArray& aColors);

protected:
	CColorBrewer m_brewer;
	CColorBrewerPaletteArray m_aPalettes;
	CString m_sNone;

protected:
	DECLARE_MESSAGE_MAP()

	virtual void BuildCombo();
	virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
								DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText);	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORBREWERCOMBOBOX_H__87AB439E_A5C6_4486_9985_0B5DC1D0F530__INCLUDED_)
