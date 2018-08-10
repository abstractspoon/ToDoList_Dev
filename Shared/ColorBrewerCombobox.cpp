// contenttypecombobox.cpp : implementation file
//

#include "stdafx.h"
#include "ColorBrewerCombobox.h"

#include "..\shared\GraphicsMisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContentTypeComboBox

CColorBrewerComboBox::CColorBrewerComboBox(DWORD dwBrewerFlags) 
	: m_nInitSel(0), m_brewer(dwBrewerFlags)
{
	SetMinDLUHeight(9);
}

CColorBrewerComboBox::~CColorBrewerComboBox()
{
}


BEGIN_MESSAGE_MAP(CColorBrewerComboBox, COwnerdrawComboBoxBase)
	//{{AFX_MSG_MAP(CContentTypeComboBox)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContentTypeComboBox message handlers

BOOL CColorBrewerComboBox::Initialize()
{
	if (!m_brewer.GetAllPalettes(m_aPalettes))
		return FALSE;

	RebuildCombo();
	return TRUE;
}

BOOL CColorBrewerComboBox::Initialize(COLORBREWER_PALETTETYPE nType)
{
	if (!m_brewer.GetPalettes(nType, m_aPalettes))
		return FALSE;

	RebuildCombo();
	return TRUE;
}

BOOL CColorBrewerComboBox::Initialize(COLORBREWER_PALETTETYPE nType, int nNumColors)
{
	if (nNumColors < 0)
	{
		ASSERT(0);
		return FALSE;
	}

	if (!m_brewer.GetPalettes(nType, m_aPalettes, nNumColors))
		return FALSE;

	RebuildCombo();
	return TRUE;
}

BOOL CColorBrewerComboBox::Initialize(int nNumColors)
{
	if (!m_brewer.GetPalettes(nNumColors, m_aPalettes))
		return FALSE;

	RebuildCombo();
	return TRUE;
}

BOOL CColorBrewerComboBox::Initialize(LPCTSTR szName, BOOL bPartial)
{
	if (!m_brewer.GetPalettes(szName, m_aPalettes, bPartial))
		return FALSE;

	RebuildCombo();
	return TRUE;
}

int CColorBrewerComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COwnerdrawComboBoxBase::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_aPalettes.GetSize())
		FillCombo();
	
	return 0;
}

void CColorBrewerComboBox::PreSubclassWindow() 
{
	if (m_aPalettes.GetSize())
		FillCombo();

	COwnerdrawComboBoxBase::PreSubclassWindow();
}

void CColorBrewerComboBox::RebuildCombo()
{
	if (GetSafeHwnd())
	{
		ResetContent();
		FillCombo();
	}
}

void CColorBrewerComboBox::FillCombo()
{
	ASSERT(GetSafeHwnd());

	if (COwnerdrawComboBoxBase::GetCount())
		return;

	ASSERT (m_aPalettes.GetSize() > 0);
	int nMaxColors = 0;

	for (int nPal = 0; nPal < m_aPalettes.GetSize(); nPal++)
	{
		int nItem = AddString(_T(""));
		SetItemData(nItem, nPal);

		nMaxColors = max(nMaxColors, m_aPalettes[nPal].GetSize());
	}

	if (m_nInitSel < GetCount())
		SetCurSel(m_nInitSel);

	if (nMaxColors > 0)
		SetDroppedWidth(nMaxColors * (GetItemHeight(0) + 2));
}

int CColorBrewerComboBox::GetSelectedPalette(CColorBrewerPalette& pal) const
{
	int nSel = GetCurSel();

	if (nSel != CB_ERR)
		pal = m_aPalettes[nSel];

	return nSel;
}

int CColorBrewerComboBox::SetSelectedPalette(const CColorBrewerPalette& pal)
{
	int nSel = m_aPalettes.Find(pal);
	SetCurSel(nSel);

	return nSel;
}

void CColorBrewerComboBox::SetCurSel(int nSel)
{
	if (GetSafeHwnd())
		COwnerdrawComboBoxBase::SetCurSel(nSel);
	else
		m_nInitSel = nSel;
}

int CColorBrewerComboBox::GetCurSel() const
{
	if (GetSafeHwnd())
		return COwnerdrawComboBoxBase::GetCurSel();
	
	// else
	return m_nInitSel;
}

void CColorBrewerComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
										  DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
{
	if (nItem != CB_ERR)
	{
		int nPalette = (int)dwItemData;
	
		if (nPalette != -1)
		{
			const CColorBrewerPalette& palette = m_aPalettes[nPalette];
			int nNumColors = palette.GetSize();

			CRect rColor(rect);
			//rColor.DeflateRect(2, 2);
			rColor.right = rColor.left + rColor.Height();

			for (int nCol = 0; nCol < nNumColors; nCol++)
			{
				COLORREF crFill = palette[nCol];
				COLORREF crBorder = GraphicsMisc::Darker(crFill, 0.3);

				GraphicsMisc::DrawRect(&dc, rColor, crFill, crBorder);
				rColor.OffsetRect(rColor.Width() + 2, 0);
			}
		}
	}
}


