#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

using namespace UIComponents;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			public ref class YearComboBoxEx : UIComponents::YearComboBox
			{
			public:
				YearComboBoxEx() : m_ListBoxHandled(false) {}

			protected:
				bool m_ListBoxHandled;

			protected:
				virtual void WndProc(Windows::Forms::Message% m) override;
				virtual void BuildCombo() override;
			};
		}
	}
}
