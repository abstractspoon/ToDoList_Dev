// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "Win32.h"
#include "ColorUtil.h"
#include "DPIScaling.h"
#include "DateUtil.h"
#include "TimeComboBox.h"

#include <Shared\WorkingWeek.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////


HostedTimeComboBox::HostedTimeComboBox(HWND hwndParent)
	:
	m_Combo(TCB_HALFHOURS | TCB_HOURSINDAY)
{
	VERIFY(m_WndOfManagedHandle.Attach(hwndParent));
}

HostedTimeComboBox* HostedTimeComboBox::Attach(HWND hwndParent, HFONT hFont)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HostedTimeComboBox* pCtrl = new HostedTimeComboBox(hwndParent);

	// Create slider to fill the entire managed client area
	CRect rClient;
	pCtrl->m_WndOfManagedHandle.GetClientRect(rClient);

	pCtrl->m_Combo.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN | CBS_HASSTRINGS | CBS_OWNERDRAWFIXED, rClient, &(pCtrl->m_WndOfManagedHandle), 1001);
	pCtrl->m_Combo.SendMessage(WM_SETFONT, (WPARAM)hFont, 0);

	return pCtrl;
}

void HostedTimeComboBox::Detach()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_WndOfManagedHandle.Detach();
	m_Combo.DestroyWindow();

	delete this;
}

double HostedTimeComboBox::Get24HourTime()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_Combo.Get24HourTime();
}

BOOL HostedTimeComboBox::Set24HourTime(double dTime)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_Combo.Set24HourTime(dTime);
}

void HostedTimeComboBox::DrawItem(WPARAM wp, LPARAM lp)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_Combo.SendMessage(WM_DRAWITEM, wp, lp);
}

BOOL HostedTimeComboBox::SetWorkingWeek(DWORD dwWeekendDays, double dLengthInHours, double dStartInHours, double dLunchStartInHours, double dLunchEndInHours)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return CWorkingWeek::Initialise(dwWeekendDays,
									dLengthInHours,
									dStartInHours,
									dLunchStartInHours,
									dLunchEndInHours);
}

////////////////////////////////////////////////////////////////////////////////////////////////

HostedTimeComboBox* Combo(IntPtr ptr)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	return (HostedTimeComboBox*)ptr.ToPointer();
}

////////////////////////////////////////////////////////////////////////////////////////////////

TimeComboBox::TimeComboBox()
{
} 

void TimeComboBox::OnHandleCreated(EventArgs^ e)
{
	Control::OnHandleCreated(e);

	m_pMFCInfo = IntPtr(HostedTimeComboBox::Attach(Win32::GetHwnd(Handle), Win32::GetHfont(Font->ToHfont())));

	CheckSetTime();
}

void TimeComboBox::OnHandleDestroyed(EventArgs^ e)
{
	Control::OnHandleDestroyed(e);

	if (m_pMFCInfo != IntPtr::Zero)
	{
		m_Time = GetTime();

		Combo(m_pMFCInfo)->Detach();
		m_pMFCInfo = IntPtr::Zero;
	}
}

TimeSpan TimeComboBox::GetTime()
{
	if (m_pMFCInfo == IntPtr::Zero)
		return m_Time;

	double time = Combo(m_pMFCInfo)->Get24HourTime();

	int hours = (int)time;
	int mins = (int)((time - hours) * 60);
	
	return TimeSpan(hours, mins, 0);
}

bool TimeComboBox::SetTime(DateTime date)
{
	return SetTime(DateUtil::TimeOnly(date));
}

bool TimeComboBox::SetTime(TimeSpan time)
{
	m_Time = time;

	return CheckSetTime();
}

bool TimeComboBox::SetWorkingWeek(WorkingWeek^ workWeek)
{
	return (HostedTimeComboBox::SetWorkingWeek(DateUtil::MapDaysOfWeek(workWeek->WeekendDays()),
											   workWeek->WorkDay()->DayLengthInHours(false),
											   workWeek->WorkDay()->StartOfDayInHours(),
											   workWeek->WorkDay()->StartOfLunchInHours(),
											   workWeek->WorkDay()->EndOfLunchInHours()) != FALSE);
}

void TimeComboBox::WndProc(Windows::Forms::Message% m)
{
	Control::WndProc(m);

	if (m_pMFCInfo == IntPtr::Zero)
		return;

	switch (m.Msg)
	{
	case WM_DRAWITEM:
		// Forward to the MFC control
		Combo(m_pMFCInfo)->DrawItem(m.WParam.ToInt32(), m.LParam.ToInt32());
		break;

	case WM_COMMAND:
		ChangeEvent(this, gcnew EventArgs());
		break;
	}
}

bool TimeComboBox::CheckSetTime()
{
	if (m_pMFCInfo == IntPtr::Zero)
		return false;

	return (Combo(m_pMFCInfo)->Set24HourTime(m_Time.Hours + (m_Time.Minutes / 60.0)) != FALSE);
}



