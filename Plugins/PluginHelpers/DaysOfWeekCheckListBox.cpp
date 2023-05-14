// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "Win32.h"
#include "ColorUtil.h"
#include "DPIScaling.h"
#include "DateUtil.h"
#include "DaysOfWeekCheckListBox.h"

#include <Shared\WorkingWeek.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////


HostedDaysOfWeekCheckListBox::HostedDaysOfWeekCheckListBox(HWND hwndParent)
{
	VERIFY(m_WndOfManagedHandle.Attach(hwndParent));
}

HostedDaysOfWeekCheckListBox* HostedDaysOfWeekCheckListBox::Attach(HWND hwndParent, HFONT hFont)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HostedDaysOfWeekCheckListBox* pCtrl = new HostedDaysOfWeekCheckListBox(hwndParent);

	// Create control to fill the entire managed client area
	CRect rClient;
	pCtrl->m_WndOfManagedHandle.GetClientRect(rClient);

	DWORD dwFlags = WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_MULTICOLUMN | LBS_HASSTRINGS | LBS_OWNERDRAWFIXED;

	if (!pCtrl->m_WndOfManagedHandle.IsWindowEnabled())
		dwFlags |= WS_DISABLED;

	pCtrl->m_ListBox.Create(dwFlags, rClient, &(pCtrl->m_WndOfManagedHandle), 1001);
	pCtrl->m_ListBox.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	pCtrl->m_ListBox.SendMessage(WM_SETFONT, (WPARAM)hFont, 0);

	return pCtrl;
}

void HostedDaysOfWeekCheckListBox::Detach()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_WndOfManagedHandle.Detach();
	m_ListBox.DestroyWindow();

	delete this;
}

DWORD HostedDaysOfWeekCheckListBox::GetSelectedDays()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_ListBox.GetChecked();
}

void HostedDaysOfWeekCheckListBox::SetSelectedDays(DWORD dwDays)
{
	m_ListBox.SetChecked(dwDays);
}

void HostedDaysOfWeekCheckListBox::DrawItem(WPARAM wp, LPARAM lp)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_ListBox.SendMessage(WM_DRAWITEM, wp, lp);
}

void HostedDaysOfWeekCheckListBox::SetEnabled(bool enabled)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_ListBox.EnableWindow(enabled);
}

////////////////////////////////////////////////////////////////////////////////////////////////

HostedDaysOfWeekCheckListBox* DayListBox(IntPtr ptr)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	return (HostedDaysOfWeekCheckListBox*)ptr.ToPointer();
}

////////////////////////////////////////////////////////////////////////////////////////////////

DaysOfWeekCheckListBox::DaysOfWeekCheckListBox()
{
} 

void DaysOfWeekCheckListBox::OnHandleCreated(EventArgs^ e)
{
	Control::OnHandleCreated(e);

	m_pMFCInfo = IntPtr(HostedDaysOfWeekCheckListBox::Attach(Win32::GetHwnd(Handle), Win32::GetHfont(Font->ToHfont())));
	CheckSetSelectedDays();
}

void DaysOfWeekCheckListBox::OnHandleDestroyed(EventArgs^ e)
{
	Control::OnHandleDestroyed(e);

	if (m_pMFCInfo != IntPtr::Zero)
	{
		m_SelectedDays = DateUtil::MapDaysOfWeek(GetSelectedDays());

		DayListBox(m_pMFCInfo)->Detach();
		m_pMFCInfo = IntPtr::Zero;
	}
}

List<System::DayOfWeek>^ DaysOfWeekCheckListBox::GetSelectedDays()
{
	if (m_pMFCInfo != IntPtr::Zero)
		m_SelectedDays = DayListBox(m_pMFCInfo)->GetSelectedDays();

	return DateUtil::MapDaysOfWeek(m_SelectedDays);
}

void DaysOfWeekCheckListBox::SetSelectedDays(List<DayOfWeek>^ days)
{
	m_SelectedDays = DateUtil::MapDaysOfWeek(days);

	CheckSetSelectedDays();
}

void DaysOfWeekCheckListBox::SetSelectedDay(DayOfWeek day)
{
	m_SelectedDays = DateUtil::MapDayOfWeek(day);

	CheckSetSelectedDays();
}

void DaysOfWeekCheckListBox::WndProc(Message% m)
{
	Control::WndProc(m);

	if (m_pMFCInfo == IntPtr::Zero)
		return;

	switch (m.Msg)
	{
	case WM_DRAWITEM:
		// Forward to the MFC control
		DayListBox(m_pMFCInfo)->DrawItem(m.WParam.ToInt32(), m.LParam.ToInt32());
		break;

	case WM_COMMAND:
		ChangeEvent(this, gcnew EventArgs());
		break;
	}
}

void DaysOfWeekCheckListBox::CheckSetSelectedDays()
{
	if (m_pMFCInfo != IntPtr::Zero)
		DayListBox(m_pMFCInfo)->SetSelectedDays(m_SelectedDays);
}

void DaysOfWeekCheckListBox::SetEnabled(bool enabled)
{
	Windows::Forms::Control::Enabled = enabled;

	if (m_pMFCInfo != IntPtr::Zero)
		DayListBox(m_pMFCInfo)->SetEnabled(enabled);
}
