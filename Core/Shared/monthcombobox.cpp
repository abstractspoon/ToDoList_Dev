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
// CMonthComboBox

CMonthComboBox::CMonthComboBox()
{
}

CMonthComboBox::~CMonthComboBox()
{
}


BEGIN_MESSAGE_MAP(CMonthComboBox, CComboBox)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonthComboBox message handlers

void CMonthComboBox::PreSubclassWindow() 
{
	BuildCombo();
	
	CComboBox::PreSubclassWindow();
}

void CMonthComboBox::BuildCombo()
{
	ASSERT(GetSafeHwnd());
	ASSERT(GetCount() == 0);

	CLocalizer::EnableTranslation(*this, FALSE);

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

void CMonthComboBox::DDX(CDataExchange* pDX, int& nMonth)
{
	CDialogHelper::DDX_CBData(pDX, *this, nMonth, 1);
}
