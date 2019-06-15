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
				static void Sort(ToolStripItemCollection^ items);

			protected:
				ref class ToolStripItemComparer : System::Collections::IComparer
				{
				public:
					virtual int Compare(System::Object^ x, System::Object^ y);
				};

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

