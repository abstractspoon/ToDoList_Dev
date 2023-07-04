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
			public ref class LabelTipInfo
			{
			public:
				LabelTipInfo();

			public:
				UInt32 Id;
				String^ Text;
				Drawing::Font^ Font;
				Drawing::Rectangle Rect;
				bool MultiLine;
				int InitialDelay; // defaults = 50ms
			};

			// -----------------------------------------------------------------
			
			public interface class ILabelTipHandler
			{
				Windows::Forms::Control^ GetOwner();

				LabelTipInfo^ ToolHitTest(Drawing::Point ptScreen);
			};

			// -----------------------------------------------------------------

			public ref class LabelTip : Windows::Forms::ToolTip
			{
			public:
				LabelTip(ILabelTipHandler^ handler);

				void ProcessMessage(Windows::Forms::Message^ msg);
				int CalcTipHeight(String^ tipText, Drawing::Font^ font, int availWidth);
				Drawing::Size CalcTipSize(String^ tipText, Drawing::Font^ font, int availWidth);

			protected:
				ILabelTipHandler^ m_Handler;

				Windows::Forms::Timer^ m_HoverTimer;
				Drawing::Point m_HoverStartScreenPos;

				Drawing::Font^ m_Font;
				Drawing::Rectangle m_HitRect;
				UInt32 m_TipId;
				bool m_Multiline;

			protected:
				static void OnShowLabelTip(Object^ sender, Windows::Forms::PopupEventArgs^ args);
				static void OnDrawLabelTip(Object^ sender, Windows::Forms::DrawToolTipEventArgs^ args);

				void OnHoverTick(Object^ sender, EventArgs^ args);

				IntPtr GetHandle();
				bool IsShowing();
				void HideTooltip();

				Windows::Forms::TextFormatFlags TipFormatFlags();
				Drawing::Size CalcFinalTipSize(Drawing::Size defaultSize);
				void CheckShowTip();
			};
		}
	}
}
