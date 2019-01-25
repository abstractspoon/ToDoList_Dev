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
				static bool ProcessMenuShortcut(System::Windows::Forms::Keys keyPress, System::Windows::Forms::ToolStripItemCollection^ items);
			};
		}
	}
}