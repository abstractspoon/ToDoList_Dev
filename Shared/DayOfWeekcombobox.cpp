// WeekdayComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "DayOfWeekcombobox.h"
#include "datehelper.h"
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
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWeekdayComboBox message handlers

void CDayOfWeekComboBox::PreSubclassWindow() 
{
	InitCombo();
	
	CComboBox::PreSubclassWindow();
}

int CDayOfWeekComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	InitCombo();
	
	return 0;
}

void CDayOfWeekComboBox::InitCombo()
{
	ASSERT(GetSafeHwnd());

	CLocalizer::EnableTranslation(*this, FALSE);

	ResetContent();
	ModifyStyle(CBS_SORT, 0); // Unsorted

	for (int nDOW = 1; nDOW <= 7; nDOW++)
	{
		int nIndex = AddString(CDateHelper::GetDayOfWeekName(nDOW, FALSE));
		SetItemData(nIndex, nDOW);
	}
}
