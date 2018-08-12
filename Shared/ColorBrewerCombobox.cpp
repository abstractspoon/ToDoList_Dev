// contenttypecombobox.cpp : implementation file
//

#include "stdafx.h"
#include "ColorBrewerCombobox.h"

#include "..\shared\Misc.h"
#include "..\shared\GraphicsMisc.h"
#include "..\shared\DialogHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContentTypeComboBox

CColorBrewerComboBox::CColorBrewerComboBox(DWORD dwBrewerFlags, UINT nIDNoneString) 
	: 
	m_brewer(dwBrewerFlags)
{
	SetMinDLUHeight(9);

	if (nIDNoneString)
		m_sNone.LoadString(nIDNoneString);
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

BOOL CColorBrewerComboBox::Initialize(const CColorBrewerPaletteArray& aPalettes)
{
	if (aPalettes.GetSize() == 0)
		return FALSE;

	m_aPalettes.Copy(aPalettes);

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
		int nSel = GetCurSel();

		ResetContent();
		FillCombo();

		SetCurSel(nSel);
	}
}

void CColorBrewerComboBox::FillCombo()
{
	ASSERT(GetSafeHwnd());

	if (COwnerdrawComboBoxBase::GetCount())
		return;

	if (!m_sNone.IsEmpty())
	{
		AddString(m_sNone);
		SetItemData(0, 0);
	}

	ASSERT (m_aPalettes.GetSize() > 0);
	int nMaxColors = 0;

	for (int nPal = 0; nPal < m_aPalettes.GetSize(); nPal++)
	{
		int nItem = AddString(Misc::Format(nPal));
		SetItemData(nItem, nPal + 1); // one-based ie. zero is 'non-selected'

		nMaxColors = max(nMaxColors, m_aPalettes[nPal].GetSize());
	}

	if (nMaxColors > 0)
		SetDroppedWidth((nMaxColors * GetItemHeight(0)) + 5);
}

int CColorBrewerComboBox::GetSelectedPalette(CColorBrewerPalette& pal) const
{
	int nPal = ((int)CDialogHelper::GetSelectedItemData(*this) - 1); // zero-based

	if (nPal >= 0)
		pal = m_aPalettes[nPal];
	
	return nPal;
}

int CColorBrewerComboBox::SetSelectedPalette(const CColorBrewerPalette& pal)
{
	int nPal = m_aPalettes.Find(pal);
	
	return CDialogHelper::SelectItemByData(*this, (nPal + 1));
}

void CColorBrewerComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
										  DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
{
	if (nItem == CB_ERR)
		return;

	int nPalette = ((int)dwItemData - 1); // zero-based

	if (nPalette == -1)
	{
		COwnerdrawComboBoxBase::DrawItemText(dc, rect, nItem, nItemState, dwItemData, sItem, bList, crText);
		return;
	}

	const CColorBrewerPalette& palette = m_aPalettes[nPalette];
	int nNumColors = palette.GetSize();

	CRect rColor(rect);
	rColor.right = rColor.left + rColor.Height();

	for (int nCol = 0; nCol < nNumColors; nCol++)
	{
		COLORREF crFill = palette[nCol];
		COLORREF crBorder = GraphicsMisc::Darker(crFill, 0.3);

		GraphicsMisc::DrawRect(&dc, rColor, crFill, crBorder);
		rColor.OffsetRect(rColor.Width() + 2, 0);
	}
}


