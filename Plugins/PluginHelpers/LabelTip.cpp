
#include "stdafx.h"
#include "Win32.h"
#include "PluginHelpers.h"
#include "LabelTip.h"
#include "DPIScaling.h"

#include <Shared\Themed.h>
#include <Shared\GraphicsMisc.h>
#include <Shared\DarkMode.h>

#include <3rdParty\OSVersion.h>

#include <math.h>
#include <CommCtrl.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Diagnostics;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

static Point OUTSIDE(-10000, -10000);

static int BORDERS = 4;

static int DEF_INITIAL_DELAY = 50;
static int DEF_AUTOPOP_DELAY = 5000;

enum TIMER_ID
{
	TIMER_INITIAL,
	TIMER_AUTOPOP,
};

////////////////////////////////////////////////////////////////////////////////////////////////

LabelTipInfo::LabelTipInfo()
	:
	Id(0), 
	Text(nullptr), 
	Font(nullptr), 
	Rect(Drawing::Rectangle::Empty), 
	MultiLine(false),
	InitialDelay(DEF_INITIAL_DELAY)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////

LabelTip::LabelTip(ILabelTipHandler^ handler) 
	: 
	m_Handler(handler),
	m_Multiline(false),
	m_TipId(0)
{
	OwnerDraw = true;

	Draw += gcnew DrawToolTipEventHandler(OnDrawLabelTip);
	Popup += gcnew PopupEventHandler(OnShowLabelTip);

	// Stops the tooltip disappearing because the mouse is over it
	Win32::AddStyle(GetHandle(), WS_EX_TRANSPARENT, true);

	m_HoverTimer = gcnew Timer();
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
	auto singleLineTextSize = TextRenderer::MeasureText(tipText, m_Font);

	Size textSize(0, 0);
	
	if (m_Multiline && (m_HitRect.Height > singleLineTextSize.Height))
	{
		textSize = TextRenderer::MeasureText(tipText, m_Font, m_HitRect.Size, TextFormatFlags::WordBreak);

		// if the measured text height exceeds the height
		// of the available rectangle, then we keep widening the
		// available rectangle until the height fits
		Size availSize = m_HitRect.Size;
		int nIncrement = (availSize.Width / 2);

		while ((textSize.Height + BORDERS) > m_HitRect.Height)
		{
			availSize.Width += nIncrement;
			textSize = TextRenderer::MeasureText(tipText, m_Font, availSize, TextFormatFlags::WordBreak);
		}
	}
	else 
	{
		// single line
		textSize = singleLineTextSize;
	}

	return (textSize + Size(BORDERS, BORDERS));
}

int LabelTip::CalcTipHeight(String^ tipText, Font^ font, int availWidth)
{
	int textHeight = TextRenderer::MeasureText(tipText, font, Size(availWidth, 0), TextFormatFlags::WordBreak).Height;

	return (textHeight + BORDERS);
}

Size LabelTip::CalcTipSize(String^ tipText, Font^ font, int availWidth)
{
	return TextRenderer::MeasureText(tipText, font, Size(availWidth, 0), TextFormatFlags::WordBreak);
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

	if ((COSVersion() >= OSV_VISTA) && !CDarkMode::IsEnabled())
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

	TextRenderer::DrawText(args->Graphics, 
						   args->ToolTipText, 
						   labelTip->m_Font, 
						   args->Bounds, 
						   *textColor, 
						   labelTip->TipFormatFlags());
}

void LabelTip::ProcessMessage(Windows::Forms::Message^ msg)
{
	if (!Active)
		return;

	switch (msg->Msg)
	{
	case WM_MOUSEMOVE:
		{
			auto pos = Control::MousePosition;
			auto tip = m_Handler->ToolHitTest(pos);

			if ((tip == nullptr) || (tip->Id == 0))
			{
				// If the mouse is no longer over an item
				// hide the tooltip
				HideTooltip();
				return;
			}

			if (tip->Id != m_TipId)
			{
				bool visible = IsShowing();

				// If the mouse has moved onto a new item
				// and hide the tooltip 
				HideTooltip();
				
				// If the tooltip was also visible then 
				// immediately show the new tip
				if (visible)
				{
					m_HoverStartScreenPos = pos;

					CheckShowTip();
					return;
				}
			}

			// Still over the same item.
			// If a tooltip is not currently visible and we moved far
			// enough away from when the timer started restart the timer
			if (!IsShowing())
			{
				int nOffset = max(abs(m_HoverStartScreenPos.X - pos.X), abs(m_HoverStartScreenPos.Y - pos.Y));

				if (nOffset > SystemInformation::MouseHoverSize.Width)
				{
					m_HoverStartScreenPos = pos;

					m_HoverTimer->Tag = gcnew int(TIMER_INITIAL);
					m_HoverTimer->Interval = tip->InitialDelay;
					m_HoverTimer->Start();
				}
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

	int timerId = *ASTYPE(m_HoverTimer->Tag, int);

	switch (timerId)
	{
	case TIMER_INITIAL:
		if (!IsShowing())
			CheckShowTip();
		break;
	
	case TIMER_AUTOPOP:
		if (IsShowing())
			Hide(m_Handler->GetOwner());
		break;
	}
}

void LabelTip::CheckShowTip()
{
	Point pos = Control::MousePosition;
	int nOffset = max(abs(m_HoverStartScreenPos.X - pos.X), abs(m_HoverStartScreenPos.Y - pos.Y));

	if (nOffset > SystemInformation::MouseHoverSize.Width)
	{
		m_HoverStartScreenPos = pos;
		return;
	}

	// else
	auto tip = m_Handler->ToolHitTest(pos);

	if ((tip == nullptr) || (tip->Id == 0))
	{
		m_HoverStartScreenPos = pos;
		Hide(m_Handler->GetOwner());

		return;
	}

	if (tip->Id != m_TipId)
	{
		m_HoverStartScreenPos = pos;
		Hide(m_Handler->GetOwner());
	}
	
	m_HitRect = tip->Rect;
	m_TipId = tip->Id;
	m_Multiline = tip->MultiLine;
	m_Font = tip->Font;

	// Minimum height is single line
	if (m_Multiline)
	{
		auto singleLineTextSize = TextRenderer::MeasureText(tip->Text, m_Font);

		if (m_HitRect.Height < (singleLineTextSize.Height + 4))
			m_Multiline = false;
	}

	Show(tip->Text, m_Handler->GetOwner(), m_HitRect.Location);

	// Restart the timer
	m_HoverTimer->Tag = gcnew int (TIMER_AUTOPOP);
	m_HoverTimer->Interval = DEF_AUTOPOP_DELAY;
	m_HoverTimer->Start();
}
