// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "Win32.h"
#include "ColorUtil.h"
#include "DPIScaling.h"
#include "CheckComboBox.h"
#include "Translator.h"

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

// static
HostedCheckComboBox* HostedCheckComboBox::Attach(HWND hwndParent, HFONT hFont, BOOL bSorted)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HostedCheckComboBox* pHost = new HostedCheckComboBox(hwndParent);

	// Create slider to fill the entire managed client area
	CRect rClient;
	pHost->m_WndOfManagedHandle.GetClientRect(rClient);

	DWORD dwFlags = (WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_HASSTRINGS | CBS_OWNERDRAWFIXED);

	if (bSorted)
		dwFlags |= CBS_SORT;

	if (!pHost->m_WndOfManagedHandle.IsWindowEnabled())
		dwFlags |= WS_DISABLED;

	pHost->m_Combo.Create(dwFlags, rClient, &(pHost->m_WndOfManagedHandle), 1001);
	pHost->m_Combo.SendMessage(WM_SETFONT, (WPARAM)hFont, 0);
	pHost->m_Combo.EnableTooltip();

	return pHost;
}

int HostedCheckComboBox::AddItem(LPCWSTR szItem, int nUniqueId, bool bChecked)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	int nItem = m_Combo.AddString(szItem);

	if (nItem != CB_ERR)
	{
		m_Combo.SetItemData(nItem, nUniqueId);

		if (bChecked)
			m_Combo.SetCheck(nItem);
	}

	return nItem;
}

bool HostedCheckComboBox::IsItemChecked(int nUniqueId)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return (m_Combo.GetCheckByData(nUniqueId) == CCBC_CHECKED);
}

bool HostedCheckComboBox::SetItemChecked(int nUniqueId, bool bChecked)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return (-1 != m_Combo.SetCheckByItemData(nUniqueId, (bChecked ? CCBC_CHECKED : CCBC_UNCHECKED)));
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

void HostedCheckComboBox::FilterTooltipMessage(MSG* pMsg) 
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_Combo.FilterToolTipMessage(pMsg); 
}

void HostedCheckComboBox::DrawItem(WPARAM wp, LPARAM lp)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_Combo.SendMessage(WM_DRAWITEM, wp, lp);
}

void HostedCheckComboBox::SetEnabled(bool bEnabled)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_Combo.EnableWindow(bEnabled);
}

void HostedCheckComboBox::SetPrompt(LPCWSTR szPrompt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_ComboPrompt.Initialize(m_Combo, szPrompt, WM_GETTEXTLENGTH);
}

void HostedCheckComboBox::OnEditchange()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_Combo.OnEditchange();
}

void HostedCheckComboBox::OnDropdown()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_Combo.OnDropdown();
}

void HostedCheckComboBox::OnCloseUp()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_Combo.OnCloseUp();
}

////////////////////////////////////////////////////////////////////////////////////////////////

CheckComboBoxItem::CheckComboBoxItem(String^ label, int uniqueId)
	:
	m_Label(label),
	m_UniqueId(uniqueId)
{
}

CheckComboBoxItem::CheckComboBoxItem(String^ label, int uniqueId, Translator^ trans)
	:
	CheckComboBoxItem(trans->Translate(label, Translator::Type::ComboBox), uniqueId)
{
}

String^ CheckComboBoxItem::Label::get() 
{ 
	return m_Label; 
}

int CheckComboBoxItem::UniqueId::get()
{ 
	return m_UniqueId;
}

////////////////////////////////////////////////////////////////////////////////////////////////

HostedCheckComboBox* ComboHost(IntPtr ptr)
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
		ComboHost(m_pMFCInfo)->AddItem(MS(item->Label), item->UniqueId, checked);

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
		ComboHost(m_pMFCInfo)->SetItemChecked(item->UniqueId, checked);

	return false;
}

bool CheckComboBox::IsItemChecked(ICheckComboBoxItem^ item)
{
	return m_CheckedItems->Contains(item);
}

void CheckComboBox::RemoveAllItems()
{
	if (m_pMFCInfo != IntPtr::Zero)
		ComboHost(m_pMFCInfo)->RemoveAllItems();
}

// ----------------------------------------------------

bool CheckComboBox::Enabled::get() 
{ 
	return Windows::Forms::Control::Enabled; 
}

void CheckComboBox::Enabled::set(bool enabled) 
{ 
	SetEnabled(enabled);
}

bool CheckComboBox::Sorted::get()
{
	return m_Sorted;
}

void CheckComboBox::Sorted::set(bool sorted)
{
	m_Sorted = sorted;
}

void CheckComboBox::Prompt::set(String^ prompt)
{
	m_Prompt = prompt;

	if (m_pMFCInfo != IntPtr::Zero)
		ComboHost(m_pMFCInfo)->SetPrompt(MS(m_Prompt));
}

void CheckComboBox::OnHandleCreated(EventArgs^ e)
{
	Control::OnHandleCreated(e);

	m_pMFCInfo = IntPtr(HostedCheckComboBox::Attach(Win32::GetHwnd(Handle),
													Win32::GetHfont(Font->ToHfont()),
													m_Sorted));
	CheckPopulateCombo();
	ComboHost(m_pMFCInfo)->SetPrompt(MS(m_Prompt));
}

void CheckComboBox::OnHandleDestroyed(EventArgs^ e)
{
	Control::OnHandleDestroyed(e);

	if (m_pMFCInfo != IntPtr::Zero)
	{
		ComboHost(m_pMFCInfo)->Detach();
		m_pMFCInfo = IntPtr::Zero;
	}
}

void CheckComboBox::WndProc(Message% m)
{
	Control::WndProc(m);

	if (m_pMFCInfo == IntPtr::Zero)
		return;

	auto pHost = ComboHost(m_pMFCInfo);

	switch (m.Msg)
	{
	case WM_DRAWITEM:
		// Forward to the MFC control
		pHost->DrawItem(m.WParam.ToInt32(), m.LParam.ToInt32());
		break;

	case WM_COMMAND:
		switch (HIWORD(m.WParam.ToInt32()))
		{
		case CBN_SELCHANGE:
			// Note: we do not update m_CheckedItems as we go
			// in case the drop is cancelled and we need to use 
			// them to undo any changes.
			if (!pHost->IsDropped())
			{
				// Update check states
				m_CheckedItems->Clear();

				for each (auto item in m_Items)
				{
					if (pHost->IsItemChecked(item->UniqueId))
						m_CheckedItems->Add(item);
				}

				DropDownClosed(this, gcnew EventArgs());
			}
			break;

		case CBN_SELENDCANCEL:
			if (!pHost->IsDropped())
			{
				// Restore original checkstates
				for each (auto item in m_Items)
				{
					bool checked = m_CheckedItems->Contains(item);
					ComboHost(m_pMFCInfo)->SetItemChecked(item->UniqueId, checked);
				}

				DropDownClosed(this, gcnew EventArgs());
			}
			break;

		// MFC message reflection appears not to work in 
		// this configuration, so we do it ourselves
		case CBN_DROPDOWN:	pHost->OnDropdown();	break;
		case CBN_CLOSEUP:	pHost->OnCloseUp();		break;
		case CBN_EDITCHANGE:pHost->OnEditchange();	break;
		}
		break;
	}
}

void CheckComboBox::FilterTooltipMessage(Windows::Forms::Message m)
{
	if (m_pMFCInfo != IntPtr::Zero)
	{
		MSG msg;
		Win32::MapMessage(m, msg);

		ComboHost(m_pMFCInfo)->FilterTooltipMessage(&msg);
	}
}

void CheckComboBox::CheckPopulateCombo()
{
	if ((m_pMFCInfo != IntPtr::Zero) &&
		(m_Items->Count != ComboHost(m_pMFCInfo)->GetItemCount()))
	{
		ComboHost(m_pMFCInfo)->RemoveAllItems();

		for each (auto item in m_Items)
		{
			bool checked = m_CheckedItems->Contains(item);
			ComboHost(m_pMFCInfo)->AddItem(MS(item->Label), item->UniqueId, checked);
		}
	}
}

void CheckComboBox::SetEnabled(bool enabled)
{
	Windows::Forms::Control::Enabled = enabled;

	if (m_pMFCInfo != IntPtr::Zero)
		ComboHost(m_pMFCInfo)->SetEnabled(enabled);
}


