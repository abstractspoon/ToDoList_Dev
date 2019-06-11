#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Windows::Forms;
using namespace System::Drawing;

////////////////////////////////////////////////////////////////////////////////////////////////

struct UITHEME;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			public ref class Toolbars
			{
			public:
				static void FixupButtonSizes(ToolStrip^ toolbar);

				static bool IsButton(ToolStripItem^ item);
				static bool IsMenu(ToolStripItem^ item);
				static bool IsSeparator(ToolStripItem^ item);

				static ToolStripButton^ AsButton(ToolStripItem^ item);
				static ToolStripMenuItem^ AsMenu(ToolStripItem^ item);
				static ToolStripSeparator^ AsSeparator(ToolStripItem^ item);
			};

			public ref class BaseToolbarRenderer : ToolStripProfessionalRenderer
			{
			public:
				BaseToolbarRenderer();

				void EnableDrawRowDividers(bool enable);
				void EnableDrawBorders(ToolStrip^ toolbar, bool left, bool top, bool right, bool bottom);

			private:
				bool m_DrawRowDividers;
				bool m_DrawLeftBorder, m_DrawTopBorder, m_DrawRightBorder, m_DrawBottomBorder;

			protected:
				virtual void OnRenderToolStripBackground(ToolStripRenderEventArgs^ e) override;
				virtual void OnRenderToolStripBorder(ToolStripRenderEventArgs^ e) override;

				virtual void DrawRowBackground(Drawing::Graphics^ g, Drawing::Rectangle^ rowRect, bool firstRow, bool lastRow);
				virtual void DrawRowDivider(Drawing::Graphics^ g, Drawing::Rectangle^ rowRect, bool firstRow, bool lastRow);
			};
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////

