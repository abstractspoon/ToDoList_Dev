#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			public interface class ILabelTipHandler
			{
				Drawing::Font^ GetFont();
				Windows::Forms::Control^ GetOwner();

				UInt32 HitTest(Drawing::Point ptScreen, String^% tipText, Drawing::Rectangle% tipRect);
			};

			// -----------------------------------------------------------------

			public ref class LabelTip : Windows::Forms::ToolTip
			{
			public:
				LabelTip(ILabelTipHandler^ handler);

				void ProcessMessage(Windows::Forms::Message^ msg);

			protected:
				ILabelTipHandler^ m_Handler;

				Windows::Forms::Timer^ m_HoverTimer;
				Drawing::Point m_HoverStartScreenPos;

				Drawing::Rectangle m_TipRect;
				UInt32 m_TipItem;

			protected:
				static void OnShowLabelTip(Object^ sender, Windows::Forms::PopupEventArgs^ args);
				static void OnDrawLabelTip(Object^ sender, Windows::Forms::DrawToolTipEventArgs^ args);

				void OnHoverTick(Object^ sender, EventArgs^ args);

				IntPtr GetHandle();
				bool IsShowing();
				void HideTooltip();
			};
		}
	}
}
