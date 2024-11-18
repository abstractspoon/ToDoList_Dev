#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

#include "Translator.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			public ref class WeekLabel : Windows::Forms::Label
			{
			public:
				WeekLabel(Translator^ trans);

				property int NumDays
				{
					int get();
					void set(int value);
				}

				property DateTime StartDate
				{
					void set(DateTime value);
				}

			private: 
				DateTime m_StartDate;
				int m_NumDays;
				Translator^ m_Trans;

			protected:
				void OnPaint(Windows::Forms::PaintEventArgs^ pe) override;
			};
		}
	}
}
