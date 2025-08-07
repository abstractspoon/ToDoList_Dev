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
// CMonthCheckComboBox

CMonthCheckComboBox::CMonthCheckComboBox()
{
}

CMonthCheckComboBox::~CMonthCheckComboBox()
{
}

BEGIN_MESSAGE_MAP(CMonthCheckComboBox, CCheckComboBox)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonthCheckComboBox message handlers

void CMonthCheckComboBox::BuildCombo()
{
	ASSERT(GetSafeHwnd());
	ASSERT(GetCount() == 0);
	ASSERT(!HasStyle(CBS_SORT));

	CLocalizer::EnableTranslation(*this, FALSE);

	for (int nMonth = 1; nMonth <= 12; nMonth++)
	{
		CDialogHelper::AddStringT(*this, 
								 CDateHelper::GetMonthName(nMonth, FALSE), 
								 CDateHelper::MapMonthIndexToDHMonth(nMonth));
	}
}

DWORD CMonthCheckComboBox::GetSelectedMonths() const
{
	return GetCheckedItemData();
}

int CMonthCheckComboBox::SetSelectedMonths(DWORD dwMonths)
{
	return SetCheckedByItemData(dwMonths);
}
