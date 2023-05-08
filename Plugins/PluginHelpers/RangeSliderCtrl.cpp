// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "Win32.h"
#include "ColorUtil.h"
#include "DPIScaling.h"
#include "RangeSliderCtrl.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

HostedRangeSliderCtrl::HostedRangeSliderCtrl() : m_Slider(TBS_BOTTOM)
{
}

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
	rClient.bottom = RangeSliderCtrl::GetRequiredHeight();

	pCtrl->m_Slider.Create(WS_CHILD | WS_VISIBLE, rClient, &(pCtrl->m_WndOfManagedHandle), 1001);

	// Further app related initialisation
	// TODO


	return pCtrl;
}

void HostedRangeSliderCtrl::Detach()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_WndOfManagedHandle.Detach();
	m_Slider.DestroyWindow();

	delete this;
}

BOOL HostedRangeSliderCtrl::SetMinMax(double min, double max)
{
	if (max <= min)
		return FALSE;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_Slider.SetMinMax(min, max);
	return TRUE;
}

BOOL HostedRangeSliderCtrl::SetRange(double left, double right)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (right <= left)
		return FALSE;

	if ((left < m_Slider.GetMin()) || (right > m_Slider.GetMax()))
		return FALSE;
	
	m_Slider.SetRange(left, right);
	return TRUE;
}

BOOL HostedRangeSliderCtrl::SetStep(double step)
{
	if (step < -1)
		return FALSE;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_Slider.SetStep(step);
	return TRUE;
}

void HostedRangeSliderCtrl::SetParentBackColor(COLORREF color)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_Slider.SetParentBackgroundColor(color);
}

BOOL HostedRangeSliderCtrl::SetMinMaxRangeWidths(double dMinWidth, double dMaxWidth)
{
	if (dMinWidth < 0)
		return FALSE;

	if ((dMaxWidth != -1) && (dMaxWidth < dMinWidth))
		return FALSE;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_Slider.SetMinMaxRangeWidths(dMinWidth, dMaxWidth);
	return TRUE;
}

BOOL HostedRangeSliderCtrl::SetMinTickSpacing(int nPixels)
{
	if (nPixels < 2)
		return FALSE;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_Slider.SetMinTickSpacing(nPixels);
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
	Control::OnHandleDestroyed(e);

	if (m_pMFCInfo != IntPtr::Zero)
	{
		Slider(m_pMFCInfo)->Detach();
		m_pMFCInfo = IntPtr::Zero;
	}
}

int RangeSliderCtrl::GetRequiredHeight()
{
	return DPIScaling::Scale(21);
}

bool RangeSliderCtrl::SetMinMax(double min, double max)
{
	if (m_pMFCInfo == IntPtr::Zero)
		return false;

	return (Slider(m_pMFCInfo)->SetMinMax(min, max) != FALSE);
}

bool RangeSliderCtrl::SetRange(double left, double right)
{
	if (m_pMFCInfo == IntPtr::Zero)
		return false;

	return (Slider(m_pMFCInfo)->SetRange(left, right) != FALSE);
}

bool RangeSliderCtrl::SetStep(double step)
{
	if (m_pMFCInfo == IntPtr::Zero)
		return false;

	return (Slider(m_pMFCInfo)->SetStep(step) != FALSE);
}

void RangeSliderCtrl::SetParentBackColor(System::Drawing::Color color)
{
	if (m_pMFCInfo == IntPtr::Zero)
		return;

	Control::BackColor = color;
	Slider(m_pMFCInfo)->SetParentBackColor(ColorUtil::DrawingColor::ToRgb(color));
}

bool RangeSliderCtrl::SetMinMaxRangeWidths(double dMinWidth, double dMaxWidth)
{
	if (m_pMFCInfo == IntPtr::Zero)
		return false;

	return (Slider(m_pMFCInfo)->SetMinMaxRangeWidths(dMinWidth, dMaxWidth) != FALSE);
}

bool RangeSliderCtrl::SetMinTickSpacing(int nPixels)
{
	if (m_pMFCInfo == IntPtr::Zero)
		return false;

	return (Slider(m_pMFCInfo)->SetMinTickSpacing(nPixels) != FALSE);
}

void RangeSliderCtrl::WndProc(Windows::Forms::Message% m)
{
	Control::WndProc(m);

	// Handle slider event
	if (m.Msg == RANGE_CHANGED)
		ChangeEvent(this, gcnew EventArgs());
}

////////////////////////////////////////////////////////////////////////////////////////////////

MonthRangeSliderCtrl::MonthRangeSliderCtrl()
{

}

void MonthRangeSliderCtrl::OnHandleCreated(EventArgs^ e)
{
	RangeSliderCtrl::OnHandleCreated(e);

	SetMinMaxRangeWidths(1.0, -1.0); // min = one month, max = unset
	SetStep(1.0);
}

bool MonthRangeSliderCtrl::SetMinMax(DateTime^ dtFrom, DateTime^ dtTo)
{
	// Convert dates to month equivalents
	int nFromMonth = DateToMonths(dtFrom);
	int nToMonth = DateToMonths(dtTo) + 1; // Inclusive

	if (nFromMonth >= nToMonth)
		return false;

	RangeSliderCtrl::SetMinMax(nFromMonth, nToMonth);
	SetRange(nFromMonth, nToMonth);

	return true;;
}

int MonthRangeSliderCtrl::DateToMonths(DateTime^ date)
{
	return ((date->Year * 12) + date->Month);
}

DateTime^ MonthRangeSliderCtrl::MonthsToDate(int nMonths)
{
	int nYear = (nMonths / 12);
	int nMonth = (nMonths % 12);

	return gcnew DateTime(nYear, nMonth, 1);
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
