// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "Win32.h"
#include "DateUtil.h"
#include "ColorUtil.h"
#include "DateRangeLink.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////


HostedDateRangeLink::HostedDateRangeLink(HWND hwndParent)
{
	VERIFY(m_WndOfManagedHandle.Attach(hwndParent));
}

HostedDateRangeLink* HostedDateRangeLink::Attach(HWND hwndParent, HFONT hFont)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HostedDateRangeLink* pCtrl = new HostedDateRangeLink(hwndParent);

	// Create label to fill the entire managed client area
	CRect rClient;
	pCtrl->m_WndOfManagedHandle.GetClientRect(rClient);

	DWORD dwFlags = (WS_CHILD | WS_VISIBLE | WS_VSCROLL | SS_CENTERIMAGE | SS_NOTIFY);

	pCtrl->m_LinkLabel.Create(NULL, dwFlags, rClient, &(pCtrl->m_WndOfManagedHandle));
	pCtrl->m_LinkLabel.SendMessage(WM_SETFONT, (WPARAM)hFont, 0);

	if (DateUtil::WantRTLDates())
		pCtrl->m_LinkLabel.ModifyStyleEx(0, WS_EX_RTLREADING);

	return pCtrl;
}

void HostedDateRangeLink::Detach()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_WndOfManagedHandle.Detach();
	m_LinkLabel.DestroyWindow();

	delete this;
}

void HostedDateRangeLink::SetText(LPCWSTR szText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_LinkLabel.SetWindowTextW(szText);
}

void HostedDateRangeLink::SetBackColor(COLORREF crBack)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_brBack.DeleteObject();
	m_brBack.CreateSolidBrush(crBack);

	m_LinkLabel.Invalidate();
}

HBRUSH HostedDateRangeLink::CtlColor(HDC hDC)
{
	m_LinkLabel.SendMessage(WM_CTLCOLORSTATIC, (WPARAM)hDC, (LPARAM)m_LinkLabel.GetSafeHwnd());

	return m_brBack;
}

////////////////////////////////////////////////////////////////////////////////////////////////

HostedDateRangeLink* Link(IntPtr ptr)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	return (HostedDateRangeLink*)ptr.ToPointer();
}

////////////////////////////////////////////////////////////////////////////////////////////////

DateRangeLink::DateRangeLink() : m_IsoFormat(false)
{
	m_Start = DateTime::Now;
	m_End = m_Start.AddSeconds(-1);
} 

void DateRangeLink::OnHandleCreated(EventArgs^ e)
{
	Control::OnHandleCreated(e);

	m_pMFCInfo = IntPtr(HostedDateRangeLink::Attach(Win32::GetHwnd(Handle), Win32::GetHfont(Font->ToHfont())));

	CheckSetRange();
	SetISOFormat(m_IsoFormat);
	OnBackColorChanged(nullptr);
}

void DateRangeLink::OnHandleDestroyed(EventArgs^ e)
{
	Control::OnHandleDestroyed(e);

	if (m_pMFCInfo != IntPtr::Zero)
	{
		Link(m_pMFCInfo)->Detach();
		m_pMFCInfo = IntPtr::Zero;
	}
}

void DateRangeLink::OnBackColorChanged(EventArgs^ e)
{
	if (e != nullptr)
		Control::OnBackColorChanged(e);

	if (m_pMFCInfo != IntPtr::Zero)
		Link(m_pMFCInfo)->SetBackColor(ColorUtil::DrawingColor::ToRgb(BackColor));
}

bool DateRangeLink::SetRange(DateTime start, DateTime end)
{
	m_Start = start;
	m_End = end;

	return CheckSetRange();
}

void DateRangeLink::ClearRange()
{
	m_End = m_Start.AddSeconds(-1);

	CheckSetRange();
}

int High16(IntPtr value)
{
	return ((short)((long)value.ToInt32() >> 16));
}

void DateRangeLink::WndProc(Windows::Forms::Message% m)
{
	Control::WndProc(m);

	if (m_pMFCInfo == IntPtr::Zero)
		return;

	switch (m.Msg)
	{
	case WM_COMMAND:
		if (High16(m.WParam) == STN_CLICKED)
			ClickEvent(this, gcnew EventArgs());
		break;

	case WM_CTLCOLORSTATIC:
		m.Result = IntPtr(Link(m_pMFCInfo)->CtlColor(Win32::GetHdc(m.WParam)));
		break;
	}
}

bool DateRangeLink::CheckSetRange()
{
	if (m_pMFCInfo == IntPtr::Zero)
		return false;

	if (m_End < m_Start)
	{
		Link(m_pMFCInfo)->SetText(L"");
		return false;
	}

	// else
	String^ text = DateUtil::FormatRange(m_Start, m_End, true, m_IsoFormat);

	Link(m_pMFCInfo)->SetText((LPCWSTR)MS(text));
	return true;
}

void DateRangeLink::SetISOFormat(bool iso)
{
	m_IsoFormat = iso;

	CheckSetRange();
}


