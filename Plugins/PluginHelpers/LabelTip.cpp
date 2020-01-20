
#include "stdafx.h"
#include "Win32.h"
#include "PluginHelpers.h"
#include "LabelTip.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Drawing;
using namespace System::Windows::Forms;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

LabelTip::LabelTip(Control^ owner) : m_Owner(owner), m_Font(owner->Font)
{
	OwnerDraw = true;

	this->Draw += gcnew DrawToolTipEventHandler(OnDrawLabelTip);
	this->Popup += gcnew PopupEventHandler(OnShowLabelTip);

	// Hack to get tooltip handle
	auto h = GetType()->GetProperty("Handle", System::Reflection::BindingFlags::NonPublic | System::Reflection::BindingFlags::Instance);
	auto handle = (IntPtr)h->GetValue(this, nullptr);

	Win32::AddStyle(handle, WS_EX_TRANSPARENT, true);
}

void LabelTip::OnShowLabelTip(Object^ sender, PopupEventArgs^ args)
{
	LabelTip^ labelTip = ASTYPE(sender, LabelTip);

	Size textSize = TextRenderer::MeasureText(labelTip->GetToolTip(labelTip->m_Owner), labelTip->m_Font);
	//int borders = (TopItem.Bounds.Height - textSize.Height);

	args->ToolTipSize = Size(textSize.Width/* + borders*/, labelTip->m_Font->Height);

}

void LabelTip::OnDrawLabelTip(Object^ sender, DrawToolTipEventArgs^ args)
{
	LabelTip^ labelTip = ASTYPE(sender, LabelTip);

	args->Graphics->FillRectangle(SystemBrushes::Window, args->Bounds);
	args->DrawBorder();

	StringFormat^ format = gcnew StringFormat();

	format->Alignment = StringAlignment::Center;
	format->LineAlignment = StringAlignment::Center;
	format->FormatFlags = StringFormatFlags::NoWrap;

	RectangleF^ rect = gcnew RectangleF((float)args->Bounds.Left, 
										(float)args->Bounds.Top, 
										(float)args->Bounds.Width, 
										(float)args->Bounds.Height);

	args->Graphics->DrawString(args->ToolTipText, labelTip->m_Font, SystemBrushes::InfoText, *rect, format);
}


