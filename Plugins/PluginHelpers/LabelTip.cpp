
#include "stdafx.h"
#include "Win32.h"
#include "PluginHelpers.h"
#include "LabelTip.h"
#include "DPIScaling.h"

#include <Shared\OSVersion.h>
#include <Shared\Themed.h>
#include <Shared\GraphicsMisc.h>

#include <math.h>
#include <CommCtrl.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Drawing;
using namespace System::Windows::Forms;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

static Point OUTSIDE(-10000, -10000);

static int BORDERS = 4;

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

	if (labelTip != nullptr)
		args->ToolTipSize = labelTip->CalcFinalTipSize(args->ToolTipSize);
}

Drawing::Size LabelTip::CalcFinalTipSize(Drawing::Size defaultSize)
{
	auto tipText = GetToolTip(m_Handler->GetOwner());
	auto tipFont = m_Handler->GetFont();

	auto textSize = m_HitRect.Size;

	if (m_Multiline)
	{
		textSize = TextRenderer::MeasureText(tipText, tipFont, textSize, TextFormatFlags::WordBreak);

		int nWidth = m_HitRect.Width;
		int nIncrement = (nWidth / 2);

		while ((textSize.Height + BORDERS) > m_HitRect.Height)
		{
			textSize.Width += nIncrement;
			textSize = TextRenderer::MeasureText(tipText, tipFont, textSize, TextFormatFlags::WordBreak);
		}
	}
	else 
	{
		// single line
		textSize = TextRenderer::MeasureText(tipText, tipFont);
	}

	return (textSize + Size(BORDERS, BORDERS));
}

int LabelTip::CalcTipHeight(String^ tipText, int availWidth)
{
	auto tipFont = m_Handler->GetFont();
	int textHeight = TextRenderer::MeasureText(tipText, tipFont, Size(availWidth, 0), TextFormatFlags::WordBreak).Height;

	return (textHeight + BORDERS);
}

TextFormatFlags LabelTip::TipFormatFlags()
{
	auto flags = (TextFormatFlags::Left | TextFormatFlags::VerticalCenter);

	if (m_Multiline)
		flags = (flags | TextFormatFlags::WordBreak);

	return flags;
}

void LabelTip::OnDrawLabelTip(Object^ sender, DrawToolTipEventArgs^ args)
{
	LabelTip^ labelTip = ASTYPE(sender, LabelTip);

	// We can't just use Winforms own 'Info/InfoText' colours (which simply wrap GetSysColor)
	// because these always return XP colours so we need a OS-specific solution
	Color^ textColor = SystemColors::InfoText;
	Brush^ backBrush = SystemBrushes::Info;

	if (COSVersion() >= OSV_VISTA)
	{
		CThemed th(::GetDesktopWindow(), L"Tooltip");
		COLORREF crText;

		if (th.GetThemeColor(TTP_STANDARD, 0, TMT_TEXTCOLOR, crText))
			textColor = Color::FromArgb(GetRValue(crText), GetGValue(crText), GetBValue(crText));
		else
			textColor = SystemColors::ControlDarkDark;

		backBrush = SystemBrushes::Window;
	}

	args->Graphics->FillRectangle(backBrush, args->Bounds);
	args->DrawBorder();

	TextRenderer::DrawText(args->Graphics, args->ToolTipText, labelTip->m_Handler->GetFont(), 
						   args->Bounds, *textColor, labelTip->TipFormatFlags());
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
			if (IsShowing() && (m_TipId > 0))
			{
				Point ptClient = m_Handler->GetOwner()->PointToClient(pos);

				if (!m_HitRect.Contains(ptClient))
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
	case WM_MOUSEWHEEL:
		HideTooltip();
		break;
	}
}

void LabelTip::HideTooltip()
{
	Hide(m_Handler->GetOwner());

	m_HoverStartScreenPos = OUTSIDE;
	m_HoverTimer->Stop();
	m_TipId = 0;
	m_HitRect = Rectangle::Empty;
	m_Multiline = false;
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
	Drawing::Rectangle hitRect;
	String^ tipText = nullptr;
	bool multiline = false;

	UInt32 nHit = m_Handler->ToolHitTest(pos, tipText, hitRect, multiline);

	if (nHit == 0)
	{
		m_HoverStartScreenPos = pos;
		Hide(m_Handler->GetOwner());
		return;
	}

	if (nHit != m_TipId)
	{
		m_HoverStartScreenPos = pos;
		Hide(m_Handler->GetOwner());
	}
	
	m_HitRect = hitRect;
	m_TipId = nHit;
	m_Multiline = multiline;

	// Minimum height is single line
	if (m_Multiline)
	{
		auto singleLineTextSize = TextRenderer::MeasureText(tipText, m_Handler->GetFont());

		if (hitRect.Height < (singleLineTextSize.Height + 4))
			m_Multiline = false;
	}

	Show(tipText, m_Handler->GetOwner(), hitRect.Location);
}
