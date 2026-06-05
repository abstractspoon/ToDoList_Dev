// EisenhowerUIExtensionControls.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
//#include "EisenhowerUIExtensionControls.h"
//#include "Win32.h"
// #include "ColorUtil.h"
// #include "DPIScaling.h"
//#include "DateUtil.h"
#include "EisenhowerFilterSetupListCtrl.h"

//#include <Shared\WorkingWeek.h>

////////////////////////////////////////////////////////////////////////////////////////////////

//#using <PluginHelpers.dll> as_friend

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

// using namespace Abstractspoon::Tdl::EisenhowerUIExtensionControls;
using namespace EisenhowerUIExtension::EisenhowerUIExtensionControls;

////////////////////////////////////////////////////////////////////////////////////////////////

HWND GetHwnd(IntPtr hWnd)
{
	return static_cast<HWND>(hWnd.ToPointer());
}

////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CEisenhowerSetupListCtrl, CInputListCtrl)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CEisenhowerSetupListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CInputListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	AddCol(L"'Urgent' Variable", 150, ILCT_COMBO);
	AddCol(L"'Urgent' Cutoff", 100, ILCT_TEXT);
	AddCol(L"'Important' Variable", 150, ILCT_COMBO);
	AddCol(L"'Important' Cutoff", 100, ILCT_TEXT);
	
	int nRow = AddRow(L"Priority");
	SetItemText(nRow, 1, L"<mid-point>");
	SetItemText(nRow, 2, L"Risk");
	SetItemText(nRow, 3, L"<mid-point>");

	AutoAdd(TRUE, FALSE);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////

HostedEisenhowerSetupListCtrl::HostedEisenhowerSetupListCtrl(HWND hwndParent)
{
	VERIFY(m_WndOfManagedHandle.Attach(hwndParent));
}

HostedEisenhowerSetupListCtrl* HostedEisenhowerSetupListCtrl::Attach(HWND hwndParent, HFONT hFont)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HostedEisenhowerSetupListCtrl* pCtrl = new HostedEisenhowerSetupListCtrl(hwndParent);

	// Create control to fill the entire managed client area
	CRect rClient;
	pCtrl->m_WndOfManagedHandle.GetClientRect(rClient);

	DWORD dwFlags = WS_CHILD | WS_VISIBLE | WS_VSCROLL | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED | LVS_NOSORTHEADER;

	if (!pCtrl->m_WndOfManagedHandle.IsWindowEnabled())
		dwFlags |= WS_DISABLED;

	pCtrl->m_ListCtrl.Create(dwFlags, rClient, &(pCtrl->m_WndOfManagedHandle), 1001);
	pCtrl->m_ListCtrl.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	pCtrl->m_ListCtrl.SendMessage(WM_SETFONT, (WPARAM)hFont, 0);

	return pCtrl;
}

void HostedEisenhowerSetupListCtrl::Detach()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_WndOfManagedHandle.Detach();
	m_ListCtrl.DestroyWindow();

	delete this;
}

// DWORD HostedEisenhowerSetupListCtrl::GetSelectedDays()
// {
// 	AFX_MANAGE_STATE(AfxGetStaticModuleState());
// 
// 	return m_ListBox.GetChecked();
// }
// 
// void HostedEisenhowerSetupListCtrl::SetSelectedDays(DWORD dwDays)
// {
// 	m_ListBox.SetChecked(dwDays);
// }

void HostedEisenhowerSetupListCtrl::DrawItem(WPARAM wp, LPARAM lp)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_ListCtrl.SendMessage(WM_DRAWITEM, wp, lp);
}

void HostedEisenhowerSetupListCtrl::UpdateSize()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CRect rSlider;
	m_WndOfManagedHandle.GetClientRect(rSlider);

	m_ListCtrl.MoveWindow(rSlider);
}

// void HostedEisenhowerSetupListCtrl::SetEnabled(bool enabled)
// {
// 	AFX_MANAGE_STATE(AfxGetStaticModuleState());
// 
// 	m_ListBox.EnableWindow(enabled);
// }

////////////////////////////////////////////////////////////////////////////////////////////////

HostedEisenhowerSetupListCtrl* ListCtrl(IntPtr ptr)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	return (HostedEisenhowerSetupListCtrl*)ptr.ToPointer();
}

HFONT GetHfont(IntPtr hFont)
{
	return static_cast<HFONT>(hFont.ToPointer());
}

////////////////////////////////////////////////////////////////////////////////////////////////

EisenhowerFilterSetupListCtrl::EisenhowerFilterSetupListCtrl()
{
} 

void EisenhowerFilterSetupListCtrl::OnHandleCreated(EventArgs^ e)
{
	Control::OnHandleCreated(e);

	m_pMFCInfo = IntPtr(HostedEisenhowerSetupListCtrl::Attach(GetHwnd(Handle), GetHfont(Font->ToHfont())));
	//CheckSetSelectedDays();
}

void EisenhowerFilterSetupListCtrl::OnHandleDestroyed(EventArgs^ e)
{
	Control::OnHandleDestroyed(e);

	if (m_pMFCInfo != IntPtr::Zero)
	{
		//m_SelectedDays = DateUtil::MapDaysOfWeek(GetSelectedDays());

		ListCtrl(m_pMFCInfo)->Detach();
		m_pMFCInfo = IntPtr::Zero;
	}
}

void EisenhowerFilterSetupListCtrl::OnSizeChanged(EventArgs^ e)
{
	Control::OnSizeChanged(e);

	if (m_pMFCInfo != IntPtr::Zero)
		ListCtrl(m_pMFCInfo)->UpdateSize();
}

// List<System::DayOfWeek>^ EisenhowerFilterSetupListCtrl::GetSelectedDays()
// {
// 	if (m_pMFCInfo != IntPtr::Zero)
// 		m_SelectedDays = ListCtrl(m_pMFCInfo)->GetSelectedDays();
// 
// 	return DateUtil::MapDaysOfWeek(m_SelectedDays);
// }
// 
// void EisenhowerFilterSetupListCtrl::SetSelectedDays(List<DayOfWeek>^ days)
// {
// 	m_SelectedDays = DateUtil::MapDaysOfWeek(days);
// 
// 	CheckSetSelectedDays();
// }
// 
// void EisenhowerFilterSetupListCtrl::SetSelectedDay(DayOfWeek day)
// {
// 	m_SelectedDays = DateUtil::MapDayOfWeek(day);
// 
// 	CheckSetSelectedDays();
// }

void EisenhowerFilterSetupListCtrl::WndProc(Message% m)
{
	Control::WndProc(m);

	if (m_pMFCInfo == IntPtr::Zero)
		return;

	switch (m.Msg)
	{
	case WM_DRAWITEM:
		// Forward to the MFC control
		ListCtrl(m_pMFCInfo)->DrawItem(m.WParam.ToInt32(), m.LParam.ToInt32());
		break;
// 
// 	case WM_COMMAND:
// 		ChangeEvent(this, gcnew EventArgs());
// 		break;
	}
}

// void EisenhowerFilterSetupListCtrl::CheckSetSelectedDays()
// {
// 	if (m_pMFCInfo != IntPtr::Zero)
// 		ListCtrl(m_pMFCInfo)->SetSelectedDays(m_SelectedDays);
// }
// 
// void EisenhowerFilterSetupListCtrl::SetEnabled(bool enabled)
// {
// 	Windows::Forms::Control::Enabled = enabled;
// 
// 	if (m_pMFCInfo != IntPtr::Zero)
// 		ListCtrl(m_pMFCInfo)->SetEnabled(enabled);
// }
