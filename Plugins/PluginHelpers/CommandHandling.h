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
			public ref class CommandHandling
			{
			public:
				static void ClearChecked(Windows::Forms::ToolStripItemCollection^ items);
				static bool HideCommand(String^ commandId, Windows::Forms::ToolStripItemCollection^ items);
				static bool ProcessMenuShortcut(Windows::Forms::Keys keyPress, Windows::Forms::ToolStripItemCollection^ items);
				static bool SetMenuShortcut(String^ commandId, Windows::Forms::Keys keyPress, Windows::Forms::ToolStripItemCollection^ items);

				static Windows::Forms::Keys GetMenuShortcut(String^ commandId, Windows::Forms::ToolStripItemCollection^ items);
				static Windows::Forms::ToolStripItem^ GetCommandItem(String^ commandId, Windows::Forms::ToolStripItemCollection^ items);
				static Windows::Forms::ToolStripMenuItem^ GetMenuItem(String^ commandId, Windows::Forms::ToolStripItemCollection^ items);
				static Windows::Forms::ToolStripMenuItem^ GetMenuItem(Windows::Forms::Keys keyPress, Windows::Forms::ToolStripItemCollection^ items);

			protected:

			};
		}
	}
}