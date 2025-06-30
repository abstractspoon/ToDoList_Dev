// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "Win32.h"
#include "ColorUtil.h"
#include "DPIScaling.h"
#include "DateUtil.h"
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

	return pCtrl;
}

void HostedRangeSliderCtrl::Detach()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_WndOfManagedHandle.Detach();
	m_Slider.DestroyWindow();

	delete this;
}

void HostedRangeSliderCtrl::UpdateSize()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CRect rSlider;
	m_WndOfManagedHandle.GetClientRect(rSlider);

	m_Slider.MoveWindow(rSlider);
}

int HostedRangeSliderCtrl::ResizeToFit(int nMaxWidth)
{
	return m_Slider.ResizeToFit(nMaxWidth);
}

BOOL HostedRangeSliderCtrl::GetMinMax(double& min, double& max)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_Slider.GetMinMax(min, max);
	return (max > min);
}

BOOL HostedRangeSliderCtrl::SetMinMax(double min, double max)
{
	if (max <= min)
		return FALSE;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_Slider.SetMinMax(min, max);
	return TRUE;
}

BOOL HostedRangeSliderCtrl::SetSelectedRange(double dFrom, double dTo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (dTo <= dFrom)
		return FALSE;

	if ((dFrom < m_Slider.GetMin()) || (dTo > m_Slider.GetMax()))
		return FALSE;
	
	m_Slider.SetRange(dFrom, dTo);
	return TRUE;
}

BOOL HostedRangeSliderCtrl::HasSelectedRange()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_Slider.HasRange();
}

BOOL HostedRangeSliderCtrl::GetSelectedRange(double& dFrom, double& dTo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!HasSelectedRange())
		return FALSE;

	m_Slider.GetRange(dFrom, dTo);
	return (dFrom < dTo);
}

void HostedRangeSliderCtrl::ClearSelectedRange()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_Slider.ClearRange();
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

void RangeSliderCtrl::OnSizeChanged(EventArgs^ e)
{
	Control::OnSizeChanged(e);

	if (m_pMFCInfo != IntPtr::Zero)
		Slider(m_pMFCInfo)->UpdateSize();
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

bool RangeSliderCtrl::GetMinMax(double% dFrom, double% dTo)
{
	if (m_pMFCInfo == IntPtr::Zero)
		return false;

	double from, to;
	
	if (!Slider(m_pMFCInfo)->GetMinMax(from, to))
		return false;

	dFrom = from;
	dTo = to;
	return true;
}

bool RangeSliderCtrl::HasSelectedRange()
{
	if (m_pMFCInfo == IntPtr::Zero)
		return false;

	return (Slider(m_pMFCInfo)->HasSelectedRange() != FALSE);
}

bool RangeSliderCtrl::SetSelectedRange(double dFrom, double dTo)
{
	if (m_pMFCInfo == IntPtr::Zero)
		return false;

	return (Slider(m_pMFCInfo)->SetSelectedRange(dFrom, dTo) != FALSE);
}

bool RangeSliderCtrl::GetSelectedRange(double% dFrom, double% dTo)
{
	if (m_pMFCInfo == IntPtr::Zero)
		return false;

	double from, to;
	
	if (!Slider(m_pMFCInfo)->GetSelectedRange(from, to))
		return false;

	dFrom = from;
	dTo = to;
	return true;
}

void RangeSliderCtrl::ClearSelectedRange()
{
	if (m_pMFCInfo == IntPtr::Zero)
		return;

	Slider(m_pMFCInfo)->ClearSelectedRange();
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

void RangeSliderCtrl::ResizeToFit(int nMaxWidth)
{
	if (m_pMFCInfo != IntPtr::Zero)
		this->Width = Slider(m_pMFCInfo)->ResizeToFit(nMaxWidth);
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

bool MonthRangeSliderCtrl::SetMinMax(DateTime dtFrom, DateTime dtTo)
{
	bool hasRange = HasSelectedRange();

	int nFromMonth = DateUtil::DateInMonths(dtFrom);
	int nToMonth = DateUtil::DateInMonths(dtTo) + 1; // Inclusive

	if (!RangeSliderCtrl::SetMinMax(nFromMonth, nToMonth))
		return false;

	if (!hasRange)
		RangeSliderCtrl::SetSelectedRange(nFromMonth, nToMonth);

	return true;;
}

bool MonthRangeSliderCtrl::GetMinMax(DateTime% dtFrom, DateTime% dtTo)
{
	double dFrom, dTo;

	if (!RangeSliderCtrl::GetMinMax(dFrom, dTo))
		return false;

	dtFrom = DateUtil::DateFromMonths((int)dFrom);
	dtTo = DateUtil::DateFromMonths((int)dTo);

	return true;
}

bool MonthRangeSliderCtrl::GetSelectedRange(DateTime% dtFrom, DateTime% dtTo)
{
	double dFrom, dTo;

	if (!RangeSliderCtrl::GetSelectedRange(dFrom, dTo))
		return false;

	dtFrom = DateUtil::DateFromMonths((int)dFrom);
	dtTo = DateUtil::DateFromMonths((int)dTo);

	return true;
}

bool MonthRangeSliderCtrl::SetSelectedRange(DateTime dtFrom, DateTime dtTo)
{
	// Convert dates to month equivalents
	int nFromMonth = DateUtil::DateInMonths(dtFrom);
	int nToMonth = DateUtil::DateInMonths(dtTo) + 1; // Inclusive

	return RangeSliderCtrl::SetSelectedRange(nFromMonth, nToMonth);
}

String^ MonthRangeSliderCtrl::FormatRange()
{
	DateTime from, to;

	if (!GetSelectedRange(from, to) && !GetMinMax(from, to))
		return String::Empty;

	to = to.AddMonths(-1);

	return DateUtil::FormatDateOnlyRange(from, to, "MMM yyyy");
}

////////////////////////////////////////////////////////////////////////////////////////////////
