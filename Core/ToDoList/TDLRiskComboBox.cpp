// TDLRiskComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "TDLRiskComboBox.h"
#include "resource.h"
#include "tdcenum.h"
#include "tdcstatic.h"

#include "..\shared\holdredraw.h"
#include "..\shared\EnString.h"
#include "..\shared\Wndprompt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLRiskComboBox

CTDLRiskComboBox::CTDLRiskComboBox(BOOL bIncludeAny) 
	: 
	m_bIncludeAny(bIncludeAny),
	m_nNumLevels(11)
{
}

CTDLRiskComboBox::~CTDLRiskComboBox()
{
}


BEGIN_MESSAGE_MAP(CTDLRiskComboBox, COwnerdrawComboBoxBase)
	//{{AFX_MSG_MAP(CTDLRiskComboBox)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLRiskComboBox message handlers

int CTDLRiskComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COwnerdrawComboBoxBase::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	BuildCombo();
	
	return 0;
}

void CTDLRiskComboBox::PreSubclassWindow() 
{
	COwnerdrawComboBoxBase::PreSubclassWindow();

 	BuildCombo();
}

int CTDLRiskComboBox::GetSelectedRisk() const
{
	int nSel = GetCurSel(), nRisk = nSel;

	switch (nSel)
	{
	case 0:
		nRisk = (m_bIncludeAny ? FM_ANYRISK : FM_NORISK);
		break;

	case 1:
		nRisk = (m_bIncludeAny ? FM_NORISK : (nSel - 1));
		break;

	default:
		if (nSel != CB_ERR)
			nRisk = (m_bIncludeAny ? (nSel - 2) : (nSel - 1));
		break;
	}

	return nRisk;
}

void CTDLRiskComboBox::SetSelectedRisk(int nRisk) // -2 -> m_nNumLevels
{
	int nSel = CB_ERR;

	switch (nRisk)
	{
	case FM_ANYRISK:
		if (m_bIncludeAny)
			nSel = 0;
		break;

	case FM_NORISK:
		nSel = (m_bIncludeAny ? 1 : 0);
		break;

	default:
		if ((nRisk >= 0) && (nRisk < m_nNumLevels))
			nSel = (m_bIncludeAny ? (nRisk + 2) : (nRisk + 1));
		break;
	}

	SetCurSel(nSel);
}

void CTDLRiskComboBox::BuildCombo()
{
	ASSERT(GetSafeHwnd());
	CHoldRedraw hr(*this);
	
	int nSel = GetCurSel(); // so we can restore it
	ResetContent();
	
	// first items are 'Any' and 'None'
	if (m_bIncludeAny)
		AddString(CEnString(IDS_TDC_ANY));
	
	AddString(CEnString(IDS_TDC_NONE));
	
	UINT aStrResIDs[11];
	TDC::GetPriorityRiskLevelStringResourceIDs(m_nNumLevels, aStrResIDs);

	for (int nLevel = 0; nLevel < m_nNumLevels; nLevel++)
	{
		CString sRisk;
		sRisk.Format(_T("%d (%s)"), nLevel, CEnString(aStrResIDs[nLevel]));
		AddString(sRisk);
	}
	
	SetCurSel(nSel);
}

void CTDLRiskComboBox::DDX(CDataExchange* pDX, int& nRisk)
{
	if (pDX->m_bSaveAndValidate)
		nRisk = GetSelectedRisk();
	else
		SetSelectedRisk(nRisk);
}

void CTDLRiskComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
	DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
{
	// Draw <any> or <none> in window prompt color
	if (!(nItemState & ODS_SELECTED) && !bList && (nItem == 0))
	{
		crText = CWndPrompt::GetTextColor();
	}

	// all else
	COwnerdrawComboBoxBase::DrawItemText(dc, rect, nItem, nItemState, dwItemData, sItem, bList, crText);
}

void CTDLRiskComboBox::SetNumLevels(int nNumLevels)
{
	ASSERT(TDC::IsValidNumPriorityResourceLevels(nNumLevels));

	if (nNumLevels != m_nNumLevels)
	{
		m_nNumLevels = nNumLevels;

		if (GetSafeHwnd())
			BuildCombo();
	}
}

