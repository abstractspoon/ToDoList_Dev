// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "Win32.h"
#include "ColorUtil.h"
#include "RangeSliderCtrl.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

HostedRangeSliderCtrl* HostedRangeSliderCtrl::Attach(IntPtr handleManaged)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HostedRangeSliderCtrl* pCtrl = new HostedRangeSliderCtrl();
	HWND hwndManaged = Win32::GetHwnd(handleManaged);

	pCtrl->m_WndOfManagedHandle.Attach(hwndManaged);

	// Create slider to fill the entire managed client area
	CRect rClient;
	pCtrl->m_WndOfManagedHandle.GetClientRect(rClient);

	// But height to match Core app
	// TODO
	// rClient.bottom = ;

	pCtrl->m_Slider.Create(WS_CHILD | WS_VISIBLE, rClient, &(pCtrl->m_WndOfManagedHandle), 1001);

	return pCtrl;
}

void HostedRangeSliderCtrl::Detach()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_WndOfManagedHandle.Detach();
	m_Slider.DestroyWindow();

	delete this;
}

void HostedRangeSliderCtrl::SetParentBackColor(COLORREF color)
{
	m_Slider.SetParentBackgroundColor(color);
}

////////////////////////////////////////////////////////////////////////////////////////////////

HostedRangeSliderCtrl* Slider(IntPtr ptr)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	return (HostedRangeSliderCtrl*)ptr.ToPointer();
}

////////////////////////////////////////////////////////////////////////////////////////////////

RangeSliderCtrl::RangeSliderCtrl()
{
} 

void RangeSliderCtrl::OnHandleCreated(EventArgs^ e)
{
	Control::OnHandleCreated(e);

	m_pMFCInfo = IntPtr(HostedRangeSliderCtrl::Attach(Handle));
}

void RangeSliderCtrl::OnHandleDestroyed(EventArgs^ e)
{
	Control::OnHandleCreated(e);

	if (m_pMFCInfo != IntPtr::Zero)
		Slider(m_pMFCInfo)->Detach();
}

void RangeSliderCtrl::SetBackColor(System::Drawing::Color color)
{
	Control::BackColor = color;

	if (m_pMFCInfo != IntPtr::Zero)
		Slider(m_pMFCInfo)->SetParentBackColor(ColorUtil::DrawingColor::ToRgb(color));
}

////////////////////////////////////////////////////////////////////////////////////////////////

MonthRangeSliderCtrl::MonthRangeSliderCtrl()
{

}

bool MonthRangeSliderCtrl::SetDataRange(DateTime^ dtFrom, DateTime^ dtTo)
{
	// TODO
	return false;
}

bool MonthRangeSliderCtrl::HasSelectedRange()
{
	// TODO
	return false;
}

bool MonthRangeSliderCtrl::GetSelectedRange(DateTime^% dtFrom, DateTime^% dtTo)
{
	// TODO
	return false;
}

bool MonthRangeSliderCtrl::SetSelectedRange(DateTime^ dtFrom, DateTime^ dtTo)
{
	// TODO
	return false;
}

void MonthRangeSliderCtrl::ClearSelectedRange()
{
	// TODO
}

String^ MonthRangeSliderCtrl::FormatRange(char cDelim)
{
	// TODO
	return gcnew String("");
}

////////////////////////////////////////////////////////////////////////////////////////////////
