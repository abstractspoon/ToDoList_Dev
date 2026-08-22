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
			// This class simply matches the checkbox 
			// clicking behaviour to the core app
			public ref class CheckListBox : Windows::Forms::CheckedListBox
			{
			protected:
				void WndProc(Windows::Forms::Message% m) override;
			};
		}
	}
}