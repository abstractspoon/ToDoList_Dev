// TDLRegularityComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "tdcstruct.h"
#include "TDLRegularityComboBox.h"
#include "TDCRecurrence.h"

#include "..\Shared\DialogHelper.h"
#include "..\Shared\WndPrompt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLRegularityComboBox

CTDLRegularityComboBox::CTDLRegularityComboBox(BOOL bIncludeAny) : m_bIncludeAny(bIncludeAny)
{
}

CTDLRegularityComboBox::~CTDLRegularityComboBox()
{
}


BEGIN_MESSAGE_MAP(CTDLRegularityComboBox, COwnerdrawComboBoxBase)
	//{{AFX_MSG_MAP(CTDLRegularityComboBox)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLRegularityComboBox message handlers

TDC_REGULARITY CTDLRegularityComboBox::GetSelectedRegularity() const
{
	int nSel = GetCurSel();

	if (nSel == CB_ERR)
		return TDIR_NONE;

	// else
	return (TDC_REGULARITY)GetItemData(nSel);
}

int CTDLRegularityComboBox::SetSelectedRegularity(TDC_REGULARITY nRegularity)
{
	return CDialogHelper::SelectItemByData(*this, nRegularity);
}

CString CTDLRegularityComboBox::GetRegularity(TDC_REGULARITY nRegularity)
{
	return TDCRECURRENCE::GetRegularityText(nRegularity, TRUE);
}

void CTDLRegularityComboBox::PreSubclassWindow() 
{
	COwnerdrawComboBoxBase::PreSubclassWindow();

	BuildCombo();
}

int CTDLRegularityComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COwnerdrawComboBoxBase::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	BuildCombo();
	
	return 0;
}

void CTDLRegularityComboBox::BuildCombo()
{
	if (m_bIncludeAny)
		CDialogHelper::AddString(*this, CEnString(IDS_TDC_ANY), TDIR_NONE);

	CDialogHelper::AddString(*this, GetRegularity(TDIR_ONCE), TDIR_ONCE);
	CDialogHelper::AddString(*this, GetRegularity(TDIR_DAILY), TDIR_DAILY);
	CDialogHelper::AddString(*this, GetRegularity(TDIR_WEEKLY), TDIR_WEEKLY);
	CDialogHelper::AddString(*this, GetRegularity(TDIR_MONTHLY), TDIR_MONTHLY);
	CDialogHelper::AddString(*this, GetRegularity(TDIR_YEARLY), TDIR_YEARLY);
}

void CTDLRegularityComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
										DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
{
	if (nItem == -1)
		return;

	// Draw <any> in window prompt color
	if (!(nItemState & ODS_SELECTED) && !bList && (nItem == 0))
	{
		crText = CWndPrompt::GetTextColor(*this);
	}

	// all else
	COwnerdrawComboBoxBase::DrawItemText(dc, rect, nItem, nItemState, dwItemData, sItem, bList, crText);
}
