#pragma once

////////////////////////////////////////////////////////////////////////////

using namespace System;

using namespace UIComponents;

////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			public ref class MonthComboBox : Windows::Forms::ComboBox
			{
			public:
				MonthComboBox();

				property int SelectedMonth
				{
					int get(); // 1-12
					void set(int month); // 1-12
				}

			protected:
				void BuildCombo();
			};

			// --------------------------------------------------------------

			public ref class YearComboBox : Windows::Forms::ComboBox
			{
			public:
				YearComboBox();

				property int SelectedYear
				{
					int get();
					void set(int year);
				}

			protected:
				bool m_ListBoxHandled;
				int m_MinYear, m_MaxYear;

			protected:
				virtual void WndProc(Windows::Forms::Message% m) override;
				
				void BuildCombo();
			};
		}
	}
}
