// TDLRegularityComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "tdcstruct.h"
#include "TDLRegularityComboBox.h"

#include "..\Shared\DialogHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLRegularityComboBox

CTDLRegularityComboBox::CTDLRegularityComboBox()
{
}

CTDLRegularityComboBox::~CTDLRegularityComboBox()
{
}


BEGIN_MESSAGE_MAP(CTDLRegularityComboBox, CComboBox)
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
	CComboBox::PreSubclassWindow();

	BuildCombo();
}

int CTDLRegularityComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	BuildCombo();
	
	return 0;
}

void CTDLRegularityComboBox::BuildCombo()
{
	CDialogHelper::AddString(*this, GetRegularity(TDIR_ONCE), TDIR_ONCE);
	CDialogHelper::AddString(*this, GetRegularity(TDIR_DAILY), TDIR_DAILY);
	CDialogHelper::AddString(*this, GetRegularity(TDIR_WEEKLY), TDIR_WEEKLY);
	CDialogHelper::AddString(*this, GetRegularity(TDIR_MONTHLY), TDIR_MONTHLY);
	CDialogHelper::AddString(*this, GetRegularity(TDIR_YEARLY), TDIR_YEARLY);
}
