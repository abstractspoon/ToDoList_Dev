
#include "stdafx.h"
#include "Win32.h"
#include "PluginHelpers.h"
#include "LabelTip.h"
#include "DPIScaling.h"

#include <math.h>
#include <CommCtrl.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Drawing;
using namespace System::Windows::Forms;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

static Point OUTSIDE(-10000, -10000);

////////////////////////////////////////////////////////////////////////////////////////////////

LabelTip::LabelTip(ILabelTipHandler^ owner) : m_Handler(owner)
{
	OwnerDraw = true;

	this->Draw += gcnew DrawToolTipEventHandler(OnDrawLabelTip);
	this->Popup += gcnew PopupEventHandler(OnShowLabelTip);

	// Stops the tooltip disappearing because the mouse is over it
	Win32::AddStyle(GetHandle(), WS_EX_TRANSPARENT, true);

	m_HoverTimer = gcnew Timer();
	m_HoverTimer->Interval = 100;// Win32::GetMouseHoverDelay();

	m_HoverTimer->Tick += gcnew EventHandler(this, &LabelTip::OnHoverTick);
	HideTooltip(); // initialises everything
}

IntPtr LabelTip::GetHandle()
{
	// Hack to get tooltip handle
	auto h = GetType()->GetProperty("Handle", System::Reflection::BindingFlags::NonPublic | System::Reflection::BindingFlags::Instance);
	
	return (IntPtr)h->GetValue(this, nullptr);
}

bool LabelTip::IsShowing()
{
	return (::IsWindowVisible(Win32::GetHwnd(GetHandle())) != FALSE);
}

void LabelTip::OnShowLabelTip(Object^ sender, PopupEventArgs^ args)
{
	LabelTip^ labelTip = ASTYPE(sender, LabelTip);

	Size textSize = TextRenderer::MeasureText(labelTip->GetToolTip(labelTip->m_Handler->GetOwner()), labelTip->m_Handler->GetFont());
	int borders = DPIScaling::Scale(3);// (args->ToolTipSize.Height - textSize.Height);

	args->ToolTipSize = Size(textSize.Width + borders, labelTip->m_TipRect.Height);
}

void LabelTip::OnDrawLabelTip(Object^ sender, DrawToolTipEventArgs^ args)
{
	LabelTip^ labelTip = ASTYPE(sender, LabelTip);

	// Don't use 'Info/InfoText' colours because these always return XP colours
	args->Graphics->FillRectangle(SystemBrushes::Window, args->Bounds);
	args->DrawBorder();

	TextRenderer::DrawText(args->Graphics, args->ToolTipText, labelTip->m_Handler->GetFont(), args->Bounds, SystemColors::WindowText, TextFormatFlags::Left | TextFormatFlags::VerticalCenter);
}

void LabelTip::ProcessMessage(Windows::Forms::Message^ msg)
{
	if (!Active)
		return;

	switch (msg->Msg)
	{
	case WM_MOUSEMOVE:
		{
			Point pos = Control::MousePosition;

			// If the mouse has moved off the current item hide the tooltip
			// else leave it alone
			if (IsShowing() && (m_TipItem > 0))
			{
				Point ptClient = m_Handler->GetOwner()->PointToClient(pos);

				if (!m_TipRect.Contains(ptClient))
					HideTooltip();
				else
					return;
			}

			// If the mouse has moved far enough away from when the timer 
			// started we update the start pos and restart the timer
			int nOffset = max(abs(m_HoverStartScreenPos.X - pos.X), abs(m_HoverStartScreenPos.Y - pos.Y));

			if (nOffset > Win32::GetMouseHoverRectSize())
			{
				m_HoverStartScreenPos = pos;
				m_HoverTimer->Start();
			}
		}
		break;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MOUSELEAVE:
		HideTooltip();
		break;
	}
}

void LabelTip::HideTooltip()
{
	Hide(m_Handler->GetOwner());

	m_HoverStartScreenPos = OUTSIDE;
	m_HoverTimer->Stop();
	m_TipItem = 0;
	m_TipRect = Rectangle::Empty;
}

void LabelTip::OnHoverTick(Object^ sender, EventArgs^ args)
{
	// Always stop the timer and allow ProcessMessage
	// to restart it as required
	m_HoverTimer->Stop();

	// Ignore if a tooltip is still visible
	if (IsShowing())
		return;
	
	Point pos = Control::MousePosition;
	int nOffset = max(abs(m_HoverStartScreenPos.X - pos.X), abs(m_HoverStartScreenPos.Y - pos.Y));

	if (nOffset > Win32::GetMouseHoverRectSize())
	{
		m_HoverStartScreenPos = pos;
		return;
	}

	// else
	Drawing::Rectangle tipItemRect;
	String^ tipText = nullptr;

	UInt32 nHit = m_Handler->HitTest(pos, tipText, tipItemRect);

	if (nHit == 0)
	{
		m_HoverStartScreenPos = pos;
		Hide(m_Handler->GetOwner());
		return;
	}

	if (TextRenderer::MeasureText(tipText, m_Handler->GetFont()).Width < tipItemRect.Width)
	{
		return;
	}
	
	if (nHit != m_TipItem)
	{
		m_HoverStartScreenPos = pos;
		Hide(m_Handler->GetOwner());
	}
	
	m_TipRect = tipItemRect;
	m_TipItem = nHit;

	Show(tipText, m_Handler->GetOwner(), tipItemRect.Location);
}
