// weekdaychecklistbox.cpp : implementation file
//

#include "stdafx.h"
#include "DayOfWeekchecklistbox.h"
#include "DateHelper.h"
#include "localizer.h"
#include "dialoghelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_WCLB_INIT (WM_APP-1)
/////////////////////////////////////////////////////////////////////////////
// CWeekdayCheckListBox

CDayOfWeekCheckListBox::CDayOfWeekCheckListBox()
{
}

CDayOfWeekCheckListBox::~CDayOfWeekCheckListBox()
{
}


BEGIN_MESSAGE_MAP(CDayOfWeekCheckListBox, CCheckListBoxEx)
	//{{AFX_MSG_MAP(CWeekdayCheckListBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_MESSAGE(WM_WCLB_INIT, OnInitListbox)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWeekdayCheckListBox message handlers

void CDayOfWeekCheckListBox::PreSubclassWindow() 
{
	CCheckListBoxEx::PreSubclassWindow();

	// too early to do it now
	PostMessage(WM_WCLB_INIT);
}

int CDayOfWeekCheckListBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CCheckListBoxEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// too early to do it now
	PostMessage(WM_WCLB_INIT);
	return 0;
}

LRESULT CDayOfWeekCheckListBox::OnInitListbox(WPARAM /*wp*/, LPARAM /*lp*/)
{
	ASSERT(GetSafeHwnd());

	CLocalizer::EnableTranslation(*this, FALSE);

	ResetContent();

	// init weekdays
	for (int nDay = 1; nDay <= 7; nDay++)
	{
		int nIndex = AddString(CDateHelper::GetDayOfWeekName(nDay, FALSE));
		SetItemData(nIndex, nDay);
	}

	CDialogHelper::RefreshMaxColumnWidth(*this);
	SetChecked(m_dwChecked);

	return 0L;
}

DWORD CDayOfWeekCheckListBox::GetChecked(int& nNumChecked) const
{
	m_dwChecked = 0;
	nNumChecked = 0;

	CDayOfWeekCheckListBox* pCLB = const_cast<CDayOfWeekCheckListBox*>(this);

	for (int nDay = 0; nDay < 7; nDay++)
	{
		if (pCLB->GetCheck(nDay))
		{
			int nDOW = pCLB->GetItemData(nDay);
			DWORD dwDay = MapDOWToDay(nDOW);

			m_dwChecked |= dwDay;
			nNumChecked++;
		}
	}

	return m_dwChecked;
}

DWORD CDayOfWeekCheckListBox::GetChecked() const
{
	int nUnused = 0;
	return GetChecked(nUnused);
}

void CDayOfWeekCheckListBox::SetChecked(DWORD dwChecked)
{
	m_dwChecked = dwChecked;

	if (GetSafeHwnd())
	{
		if (GetCount() == 0)
		{
			OnInitListbox(0, 0);
		}
		else if (m_dwChecked)
		{
			for (int nDay = 0; nDay < 7; nDay++)
			{
				int nDOW = GetItemData(nDay);
				DWORD dwDay = MapDOWToDay(nDOW);

				SetCheck(nDay, (m_dwChecked & dwDay) ? 1 : 0);
			}
		}
	}
}

DWORD CDayOfWeekCheckListBox::MapDOWToDay(int nDOW)
{
	switch (nDOW)
	{
	case 1: return DHW_SUNDAY;
	case 2: return DHW_MONDAY;
	case 3: return DHW_TUESDAY;
	case 4: return DHW_WEDNESDAY;
	case 5: return DHW_THURSDAY;
	case 6: return DHW_FRIDAY;
	case 7: return DHW_SATURDAY;
	}

	// else
	ASSERT(0);
	return 0;
}
