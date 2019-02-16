// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Toolbars.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

void Toolbars::FixupButtonSizes(ToolStrip^ toolbar)
{
	auto imageSize = toolbar->ImageScalingSize;
	int numItems = toolbar->Items->Count;

	for (int i = 0; i < numItems; i++)
	{
		auto button = dynamic_cast<ToolStripButton^>(toolbar->Items[i]);

		if (button != nullptr)
		{
			// From 'Shared\EnToolBar.cpp'
			int xPadding = (imageSize.Width + 7 - button->Size.Width);
			int yPadding = (imageSize.Height + 7 - button->Size.Height);

			button->Padding = Padding(xPadding / 2, yPadding / 2, xPadding - xPadding / 2, yPadding - yPadding / 2);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

BaseToolbarRenderer::BaseToolbarRenderer() : m_DrawRowDividers(true)
{

}

void BaseToolbarRenderer::EnableDrawRowDividers(bool enable)
{
	m_DrawRowDividers = enable;
}

void BaseToolbarRenderer::OnRenderToolStripBackground(ToolStripRenderEventArgs^ e)
{
	ToolStripRenderer::OnRenderToolStripBackground(e);

	if (m_DrawRowDividers)
	{
		auto toolbar = e->ToolStrip;
		int numItems = toolbar->Items->Count;

		if (numItems > 0)
		{
			auto prevBtnRect = Drawing::Rectangle::Empty;
			int rowTop = toolbar->Top, rowBottom = 0;
			bool firstRow = true;

			for (int i = 0; i < numItems; i++)
			{
				auto button = dynamic_cast<ToolStripButton^>(toolbar->Items[i]);

				if (button != nullptr)
				{
					if (prevBtnRect.IsEmpty)
					{
						prevBtnRect = button->Bounds;
					}
					else
					{
						auto btnRect = button->Bounds;

						if (btnRect.Top > prevBtnRect.Top)
						{
							rowBottom = ((btnRect.Top + prevBtnRect.Bottom) / 2);

							auto rowRect = gcnew Drawing::Rectangle(toolbar->Left, rowTop, toolbar->Width, (rowBottom - rowTop));
							DrawRowBackground(e->Graphics, rowRect, firstRow, false);
							
							prevBtnRect = btnRect;
							rowTop = rowBottom + 1;
							firstRow = false;
						}
					}
				}
			}

			// Last row
			auto rowRect = gcnew Drawing::Rectangle(toolbar->Left, rowTop, toolbar->Width, (toolbar->Bottom - rowTop));
			DrawRowBackground(e->Graphics, rowRect, firstRow, true);
		}
	}
}

void BaseToolbarRenderer::DrawRowBackground(Drawing::Graphics^ g, Drawing::Rectangle^ rowRect, bool firstRow, bool lastRow)
{
	DrawRowDivider(g, rowRect, firstRow, lastRow);
}

void BaseToolbarRenderer::DrawRowDivider(Drawing::Graphics^ g, Drawing::Rectangle^ rowRect, bool firstRow, bool lastRow)
{
	// Draw highlight line at top if not first row
	if (!firstRow)
		g->DrawLine(SystemPens::ButtonHighlight, rowRect->Left, rowRect->Top, rowRect->Right, rowRect->Top);


	// Draw shadow line at bottom if not last row
	if (!lastRow)
		g->DrawLine(SystemPens::ButtonShadow, rowRect->Left, rowRect->Bottom, rowRect->Right, rowRect->Bottom);
}

////////////////////////////////////////////////////////////////////////////////////////////////
