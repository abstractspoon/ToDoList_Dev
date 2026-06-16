// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "Win32.h"
#include "ColorUtil.h"
#include "DPIScaling.h"
#include "CheckComboBox.h"

#include <Shared\DialogHelper.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

const int CB_VALUECHANGE = CB_MSGMAX;

////////////////////////////////////////////////////////////////////////////////////////////////

HostedCheckComboBox::HostedCheckComboBox(HWND hwndParent)
{
	VERIFY(m_WndOfManagedHandle.Attach(hwndParent));
}

HostedCheckComboBox* HostedCheckComboBox::Attach(HWND hwndParent, HFONT hFont)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HostedCheckComboBox* pCtrl = new HostedCheckComboBox(hwndParent);

	// Create slider to fill the entire managed client area
	CRect rClient;
	pCtrl->m_WndOfManagedHandle.GetClientRect(rClient);

	DWORD dwFlags = WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN | CBS_HASSTRINGS | CBS_OWNERDRAWFIXED;

	if (!pCtrl->m_WndOfManagedHandle.IsWindowEnabled())
		dwFlags |= WS_DISABLED;

	pCtrl->m_Combo.Create(dwFlags, rClient, &(pCtrl->m_WndOfManagedHandle), 1001);
	pCtrl->m_Combo.SendMessage(WM_SETFONT, (WPARAM)hFont, 0);

	return pCtrl;
}

int HostedCheckComboBox::AddItem(LPCWSTR szItem, int nItemData, bool checked)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return CDialogHelper::AddStringT(m_Combo, szItem, nItemData);
}

bool HostedCheckComboBox::IsItemChecked(int nItemData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return (m_Combo.GetCheckByData(nItemData) == CCBC_CHECKED);
}

bool HostedCheckComboBox::SetItemChecked(int nItemData, bool checked)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return (-1 != m_Combo.SetCheckByItemData(nItemData, (checked ? CCBC_CHECKED : CCBC_UNCHECKED)));
}

void HostedCheckComboBox::RemoveAllItems()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_Combo.ResetContent();
}

int HostedCheckComboBox::GetItemCount()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_Combo.GetCount();
}

void HostedCheckComboBox::Detach()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_WndOfManagedHandle.Detach();
	m_Combo.DestroyWindow();

	delete this;
}

void HostedCheckComboBox::DrawItem(WPARAM wp, LPARAM lp)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_Combo.SendMessage(WM_DRAWITEM, wp, lp);
}

void HostedCheckComboBox::SetEnabled(bool enabled)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_Combo.EnableWindow(enabled);
}

////////////////////////////////////////////////////////////////////////////////////////////////

HostedCheckComboBox* Combo(IntPtr ptr)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	return (HostedCheckComboBox*)ptr.ToPointer();
}

////////////////////////////////////////////////////////////////////////////////////////////////

CheckComboBox::CheckComboBox()
{
	m_Items = gcnew List<ICheckComboBoxItem^>();
	m_CheckedItems = gcnew List<ICheckComboBoxItem^>();
} 

int CheckComboBox::AddItem(ICheckComboBoxItem^ item, bool checked)
{
	if (item == nullptr)
		return -1;

	m_Items->Add(item);

	if (checked)
		m_CheckedItems->Add(item);

	if (m_pMFCInfo != IntPtr::Zero)
		Combo(m_pMFCInfo)->AddItem(MS(item->Label), item->ItemData, checked);

	return (m_Items->Count - 1);
}

bool CheckComboBox::SetItemChecked(ICheckComboBoxItem^ item, bool checked)
{
	if (item == nullptr)
		return false;

	if (m_Items->IndexOf(item) == -1)
		return false;

	int iChecked = m_CheckedItems->IndexOf(item);

	if (!checked && (iChecked != -1))
	{
		m_CheckedItems->RemoveAt(iChecked);
	}
	else if (checked && (iChecked == -1))
	{
		m_CheckedItems->Add(item);
	}

	if (m_pMFCInfo != IntPtr::Zero)
		Combo(m_pMFCInfo)->SetItemChecked(item->ItemData, checked);

	return false;
}

bool CheckComboBox::IsItemChecked(ICheckComboBoxItem^ item)
{
	return m_CheckedItems->Contains(item);
}

void CheckComboBox::RemoveAllItems()
{
	if (m_pMFCInfo != IntPtr::Zero)
		Combo(m_pMFCInfo)->RemoveAllItems();
}

// ----------------------------------------------------

void CheckComboBox::OnHandleCreated(EventArgs^ e)
{
	Control::OnHandleCreated(e);

	m_pMFCInfo = IntPtr(HostedCheckComboBox::Attach(Win32::GetHwnd(Handle), Win32::GetHfont(Font->ToHfont())));

	CheckPopulateCombo();
}

void CheckComboBox::OnHandleDestroyed(EventArgs^ e)
{
	Control::OnHandleDestroyed(e);

	if (m_pMFCInfo != IntPtr::Zero)
	{
		UpdateCheckStates();

		Combo(m_pMFCInfo)->Detach();
		m_pMFCInfo = IntPtr::Zero;
	}
}

void CheckComboBox::WndProc(Message% m)
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

// 	case WM_COMMAND:
// 		switch (HIWORD(m.WParam.ToInt32()))
// 		{
// 		case CBN_SELCHANGE:
// 		case CBN_SELENDOK:
// 		case CBN_SELENDCANCEL:
// 		case CBN_EDITCHANGE:
// 		case CBN_EDITUPDATE:
// 		case CBN_CLOSEUP:
// 			::PostMessage(Win32::GetHwnd(Handle), CB_VALUECHANGE, 0, 0);
// 			break;
// 		}
// 		break;
// 
// 	case CB_VALUECHANGE:
// 		ChangeEvent(this, gcnew EventArgs());
// 		break;
	}
}

void CheckComboBox::CheckPopulateCombo()
{
	if ((m_pMFCInfo != IntPtr::Zero) &&
		(m_Items->Count != Combo(m_pMFCInfo)->GetItemCount()))
	{
		Combo(m_pMFCInfo)->RemoveAllItems();

		for each (auto item in m_Items)
		{
			bool checked = m_CheckedItems->Contains(item);
			Combo(m_pMFCInfo)->AddItem(MS(item->Label), item->ItemData, checked);
		}
	}
}

void CheckComboBox::UpdateCheckStates()
{
	if (m_pMFCInfo != IntPtr::Zero)
	{
		m_CheckedItems->Clear();

		for each (auto item in m_Items)
		{
			if (Combo(m_pMFCInfo)->IsItemChecked(item->ItemData))
				m_CheckedItems->Add(item);
		}
	}
}

void CheckComboBox::SetEnabled(bool enabled)
{
	Windows::Forms::Control::Enabled = enabled;

	if (m_pMFCInfo != IntPtr::Zero)
		Combo(m_pMFCInfo)->SetEnabled(enabled);
}


