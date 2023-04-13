#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

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
				static void FixupButtonSizes(Windows::Forms::ToolStrip^ toolbar);
				static void Sort(Windows::Forms::ToolStripItemCollection^ items);
				static int GetSelectedItem(Windows::Forms::ToolStripItemCollection^ items);

				enum class ItemState
				{
					Normal,
					Disabled,
					Pressed,
					Checked,
					Hot,
				};

				static ItemState GetItemState(Windows::Forms::ToolStripItem^ item);

			protected:
				ref class ToolStripItemComparer : Collections::IComparer
				{
				public:
					virtual int Compare(Object^ x, Object^ y);
				};

			};

			public ref class BaseToolbarRenderer : Windows::Forms::ToolStripProfessionalRenderer
			{
			public:
				BaseToolbarRenderer();

				void EnableDrawRowSeparators(bool enable);
				void EnableDrawBorders(Windows::Forms::ToolStrip^ toolbar, bool left, bool top, bool right, bool bottom);

			private:
				bool m_DrawRowSeparators;
				bool m_DrawLeftBorder, m_DrawTopBorder, m_DrawRightBorder, m_DrawBottomBorder;

			protected:
				virtual void OnRenderToolStripBackground(Windows::Forms::ToolStripRenderEventArgs^ e) override;
				virtual void OnRenderToolStripBorder(Windows::Forms::ToolStripRenderEventArgs^ e) override;
				virtual void OnRenderSeparator(Windows::Forms::ToolStripSeparatorRenderEventArgs^ e) override;

				virtual void DrawRowBackground(Drawing::Graphics^ g, Drawing::Rectangle^ rowRect, bool firstRow, bool lastRow, bool isMenuBar);
				virtual void DrawRowSeparator(Drawing::Graphics^ g, Drawing::Rectangle^ rowRect, bool firstRow, bool lastRow);

				virtual Drawing::Pen^ GetSeperatorLightPen() { return Drawing::SystemPens::ButtonHighlight; }
				virtual Drawing::Pen^ GetSeperatorDarkPen() { return Drawing::SystemPens::ButtonShadow; }
			};
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////

