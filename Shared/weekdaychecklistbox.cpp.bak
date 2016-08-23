// weekdaychecklistbox.cpp : implementation file
//

#include "stdafx.h"
#include "weekdaychecklistbox.h"
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

CWeekdayCheckListBox::CWeekdayCheckListBox()
{
}

CWeekdayCheckListBox::~CWeekdayCheckListBox()
{
}


BEGIN_MESSAGE_MAP(CWeekdayCheckListBox, CCheckListBoxEx)
	//{{AFX_MSG_MAP(CWeekdayCheckListBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_MESSAGE(WM_WCLB_INIT, OnInitListbox)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWeekdayCheckListBox message handlers

void CWeekdayCheckListBox::PreSubclassWindow() 
{
	CCheckListBoxEx::PreSubclassWindow();

	// too early to do it now
	PostMessage(WM_WCLB_INIT);
}

int CWeekdayCheckListBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CCheckListBoxEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// too early to do it now
	PostMessage(WM_WCLB_INIT);
	return 0;
}

LRESULT CWeekdayCheckListBox::OnInitListbox(WPARAM /*wp*/, LPARAM /*lp*/)
{
	ASSERT(GetSafeHwnd());

	CLocalizer::EnableTranslation(*this, FALSE);

	ResetContent();

	// init weekdays
	for (int nDay = 1; nDay <= 7; nDay++)
	{
		int nIndex = AddString(CDateHelper::GetWeekdayName(nDay, FALSE));
		SetItemData(nIndex, nDay);
	}

	CDialogHelper::RefreshMaxColumnWidth(*this);
	SetChecked(m_dwChecked);

	return 0L;
}

DWORD CWeekdayCheckListBox::GetChecked() const
{
	m_dwChecked = 0;

	for (int nDay = 0; nDay < 7; nDay++)
	{
		CWeekdayCheckListBox* pCLB = const_cast<CWeekdayCheckListBox*>(this);

		if (pCLB->GetCheck(nDay))
		{
			int nDOW = pCLB->GetItemData(nDay);

			switch (nDOW)
			{
			case 1: 
				m_dwChecked |= DHW_SUNDAY;
				break;

			case 2: 
				m_dwChecked |= DHW_MONDAY;
				break;

			case 3: 
				m_dwChecked |= DHW_TUESDAY;
				break;

			case 4: 
				m_dwChecked |= DHW_WEDNESDAY;
				break;

			case 5: 
				m_dwChecked |= DHW_THURSDAY;
				break;

			case 6: 
				m_dwChecked |= DHW_FRIDAY;
				break;

			case 7: 
				m_dwChecked |= DHW_SATURDAY;
				break;
			}
		}
	}

	return m_dwChecked;
}

void CWeekdayCheckListBox::SetChecked(DWORD dwChecked)
{
	m_dwChecked = dwChecked;

	if (!GetSafeHwnd())
		return;

	if (GetCount() == 0)
		OnInitListbox(0, 0);

	for (int nDay = 0; nDay < 7; nDay++)
	{
		int nDOW = GetItemData(nDay);
		
		switch (nDOW)
		{
		case 1: 
			SetCheck(nDay, (m_dwChecked & DHW_SUNDAY) ? 1 : 0);
			break;
			
		case 2: 
			SetCheck(nDay, (m_dwChecked & DHW_MONDAY) ? 1 : 0);
			break;
			
		case 3: 
			SetCheck(nDay, (m_dwChecked & DHW_TUESDAY) ? 1 : 0);
			break;
			
		case 4: 
			SetCheck(nDay, (m_dwChecked & DHW_WEDNESDAY) ? 1 : 0);
			break;
			
		case 5: 
			SetCheck(nDay, (m_dwChecked & DHW_THURSDAY) ? 1 : 0);
			break;
			
		case 6: 
			SetCheck(nDay, (m_dwChecked & DHW_FRIDAY) ? 1 : 0);
			break;
			
		case 7: 
			SetCheck(nDay, (m_dwChecked & DHW_SATURDAY) ? 1 : 0);
			break;
		}
	}
}
