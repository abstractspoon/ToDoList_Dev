#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

class ITransText;

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
				static bool HideCommand(String^ commandId, System::Windows::Forms::ToolStripItemCollection^ items);
				static void HideDuplicateSeparators(System::Windows::Forms::ToolStripItemCollection^ items);
				static bool ProcessMenuShortcut(System::Windows::Forms::Keys keyPress, System::Windows::Forms::ToolStripItemCollection^ items);
				static bool SetMenuShortcut(String^ commandId, System::Windows::Forms::Keys keyPress, System::Windows::Forms::ToolStripItemCollection^ items);

				static System::Windows::Forms::Keys GetMenuShortcut(String^ commandId, System::Windows::Forms::ToolStripItemCollection^ items);
				static System::Windows::Forms::ToolStripItem^ GetCommandItem(String^ commandId, System::Windows::Forms::ToolStripItemCollection^ items);
				static System::Windows::Forms::ToolStripMenuItem^ GetMenuItem(String^ commandId, System::Windows::Forms::ToolStripItemCollection^ items);
				static System::Windows::Forms::ToolStripMenuItem^ GetMenuItem(System::Windows::Forms::Keys keyPress, System::Windows::Forms::ToolStripItemCollection^ items);
			};
		}
	}
}