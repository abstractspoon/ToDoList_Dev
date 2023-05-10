// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "Win32.h"
#include "ColorUtil.h"
#include "DPIScaling.h"
#include "DateUtil.h"
#include "TimeComboBox.h"

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

	pCtrl->m_Combo.Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWN |  CBS_HASSTRINGS | CBS_OWNERDRAWFIXED, rClient, &(pCtrl->m_WndOfManagedHandle), 1001);
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

	return m_Combo.SetOleTime(dTime);
}

void HostedTimeComboBox::DrawItem(WPARAM wp, LPARAM lp)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_Combo.SendMessage(WM_DRAWITEM, wp, lp);
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
}

void TimeComboBox::OnHandleDestroyed(EventArgs^ e)
{
	Control::OnHandleDestroyed(e);

	if (m_pMFCInfo != IntPtr::Zero)
	{
		Combo(m_pMFCInfo)->Detach();
		m_pMFCInfo = IntPtr::Zero;
	}
}

TimeSpan TimeComboBox::GetTime()
{
	if (m_pMFCInfo == IntPtr::Zero)
		return TimeSpan(0, 0, 0);

	double time = Combo(m_pMFCInfo)->Get24HourTime();

	int hours = (int)time;
	int mins = (int)((time - hours) * 60);
	
	return TimeSpan(hours, mins, 0);
}

bool TimeComboBox::SetTime(TimeSpan time)
{
	if (m_pMFCInfo == IntPtr::Zero)
		return false;

	return (Combo(m_pMFCInfo)->Set24HourTime(time.Hours + (time.Minutes / 60.0)) != FALSE);
}

bool TimeComboBox::SetTime(DateTime date)
{
	return SetTime(date - date.Date);
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

