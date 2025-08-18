// WeekdayComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "DayOfWeekcombobox.h"
#include "datehelper.h"
#include "dialoghelper.h"
#include "localizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWeekdayComboBox

CDayOfWeekComboBox::CDayOfWeekComboBox()
{
}

CDayOfWeekComboBox::~CDayOfWeekComboBox()
{
}


BEGIN_MESSAGE_MAP(CDayOfWeekComboBox, CComboBox)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWeekdayComboBox message handlers

void CDayOfWeekComboBox::PreSubclassWindow() 
{
	BuildCombo();
	
	CComboBox::PreSubclassWindow();
}

void CDayOfWeekComboBox::BuildCombo()
{
	ASSERT(GetSafeHwnd());
	ASSERT(GetCount() == 0);
	ASSERT(!(GetStyle() & CBS_SORT));

	CLocalizer::EnableTranslation(*this, FALSE);

	for (int nDay = 0; nDay < 7; nDay++)
	{
		OLE_DAYOFWEEK nDOW = OLE_DAYSOFWEEK[nDay];
		CDialogHelper::AddStringT(*this, CDateHelper::GetDayOfWeekName(nDOW, FALSE), nDOW);
	}
}
