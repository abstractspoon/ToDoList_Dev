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
			public ref class LabelTip : Windows::Forms::ToolTip
			{
			public:
				LabelTip(Windows::Forms::Control^ owner);

				void SetFont(Drawing::Font^ font) { m_Font = font; }

			protected:
				Windows::Forms::Control^ m_Owner;
				Drawing::Font^ m_Font;

			protected:
				static void OnShowLabelTip(Object^ sender, Windows::Forms::PopupEventArgs^ args);
				static void OnDrawLabelTip(Object^ sender, Windows::Forms::DrawToolTipEventArgs^ args);
			};
		}
	}
}
