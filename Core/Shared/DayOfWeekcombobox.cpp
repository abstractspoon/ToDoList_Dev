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
	//{{AFX_MSG_MAP(CWeekdayComboBox)
// 	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWeekdayComboBox message handlers

void CDayOfWeekComboBox::PreSubclassWindow() 
{
	BuildCombo();
	
	CComboBox::PreSubclassWindow();
}

// int CDayOfWeekComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
// {
// 	if (CComboBox::OnCreate(lpCreateStruct) == -1)
// 		return -1;
// 	
// 	InitCombo();
// 	
// 	return 0;
// }

void CDayOfWeekComboBox::BuildCombo()
{
	ASSERT(GetSafeHwnd());
	ASSERT(GetCount() == 0);

	CLocalizer::EnableTranslation(*this, FALSE);

	ModifyStyle(CBS_SORT, 0); // Unsorted

	for (int nDay = 0; nDay < 7; nDay++)
	{
		OLE_DAYOFWEEK nDOW = OLE_DAYSOFWEEK[nDay];
		CDialogHelper::AddStringT(*this, CDateHelper::GetDayOfWeekName(nDOW, FALSE), nDOW);
	}
}
