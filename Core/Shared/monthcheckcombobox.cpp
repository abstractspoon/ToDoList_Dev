// monthcombobox.cpp : implementation file
//

#include "stdafx.h"
#include "monthcheckcombobox.h"
#include "datehelper.h"
#include "dialoghelper.h"
#include "localizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

void DDX_Months(CDataExchange* pDX, CMonthCheckComboBox& combo, DWORD& dwMonths)
{
	DDX_CheckItemData(pDX, combo, dwMonths);
}

/////////////////////////////////////////////////////////////////////////////
// CMonthCheckComboBox

CMonthCheckComboBox::CMonthCheckComboBox()
{
}

CMonthCheckComboBox::~CMonthCheckComboBox()
{
}

BEGIN_MESSAGE_MAP(CMonthCheckComboBox, CCheckComboBox)
	//{{AFX_MSG_MAP(CMonthCheckComboBox)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonthCheckComboBox message handlers

void CMonthCheckComboBox::PreSubclassWindow() 
{
	InitCombo();
	
	CCheckComboBox::PreSubclassWindow();
}

int CMonthCheckComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CCheckComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	InitCombo();
	
	return 0;
}

void CMonthCheckComboBox::InitCombo()
{
	ASSERT(GetSafeHwnd());

	CLocalizer::EnableTranslation(*this, FALSE);

	ResetContent();
	ModifyStyle(CBS_SORT, 0); // Unsorted

	for (int nMonth = 1; nMonth <= 12; nMonth++)
	{
		CDialogHelper::AddStringT(*this, 
								 CDateHelper::GetMonthName(nMonth, FALSE), 
								 CDateHelper::MapMonthIndexToDHMonth(nMonth));
	}
}

DWORD CMonthCheckComboBox::GetSelectedMonths() const
{
	return CDialogHelper::GetSelectedItemData(*this);
}

int CMonthCheckComboBox::SetSelectedMonths(DWORD dwMonths)
{
	return CDialogHelper::SelectItemByDataT(*this, dwMonths);
}
