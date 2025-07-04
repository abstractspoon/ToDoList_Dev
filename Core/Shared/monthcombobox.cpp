// monthcombobox.cpp : implementation file
//

#include "stdafx.h"
#include "monthcombobox.h"
#include "datehelper.h"
#include "dialoghelper.h"
#include "localizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

void DDX_Month(CDataExchange* pDX, CMonthComboBox& combo, int& nMonth)
{
	CDialogHelper::DDX_CBData(pDX, combo, nMonth, 1);
}

/////////////////////////////////////////////////////////////////////////////
// CMonthComboBox

CMonthComboBox::CMonthComboBox()
{
}

CMonthComboBox::~CMonthComboBox()
{
}


BEGIN_MESSAGE_MAP(CMonthComboBox, CComboBox)
	//{{AFX_MSG_MAP(CMonthComboBox)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonthComboBox message handlers

void CMonthComboBox::PreSubclassWindow() 
{
	InitCombo();
	
	CComboBox::PreSubclassWindow();
}

int CMonthComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	InitCombo();
	
	return 0;
}

void CMonthComboBox::InitCombo()
{
	ASSERT(GetSafeHwnd());

	CLocalizer::EnableTranslation(*this, FALSE);

	ResetContent();
	ModifyStyle(CBS_SORT, 0); // Unsorted

	for (int nMonth = 1; nMonth <= 12; nMonth++)
		CDialogHelper::AddStringT(*this, CDateHelper::GetMonthName(nMonth, FALSE), nMonth);
}

int CMonthComboBox::GetSelectedMonth() const
{
	return CDialogHelper::GetSelectedItemData(*this);
}

int CMonthComboBox::SetSelectedMonth(int nMonth)
{
	return CDialogHelper::SelectItemByDataT(*this, nMonth);
}
