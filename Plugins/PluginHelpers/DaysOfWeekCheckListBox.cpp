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

	// Create slider to fill the entire managed client area
	CRect rClient;
	pCtrl->m_WndOfManagedHandle.GetClientRect(rClient);

	pCtrl->m_ListBox.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | /*WS_BORDER |*/ LBS_MULTICOLUMN | LBS_HASSTRINGS | LBS_OWNERDRAWFIXED, rClient, &(pCtrl->m_WndOfManagedHandle), 1001);
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
}

void DaysOfWeekCheckListBox::OnHandleDestroyed(EventArgs^ e)
{
	Control::OnHandleDestroyed(e);

	if (m_pMFCInfo != IntPtr::Zero)
	{
		DayListBox(m_pMFCInfo)->Detach();
		m_pMFCInfo = IntPtr::Zero;
	}
}

List<System::DayOfWeek>^ DaysOfWeekCheckListBox::GetSelectedDays()
{
	if (m_pMFCInfo == IntPtr::Zero)
		return nullptr;

	return DateUtil::MapDaysOfWeek(DayListBox(m_pMFCInfo)->GetSelectedDays());
}

void DaysOfWeekCheckListBox::SetSelectedDays(List<System::DayOfWeek>^ days)
{
	if (m_pMFCInfo == IntPtr::Zero)
		return;

	DayListBox(m_pMFCInfo)->SetSelectedDays(DateUtil::MapDaysOfWeek(days));
}

void DaysOfWeekCheckListBox::WndProc(Windows::Forms::Message% m)
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


