#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

#include "Translator.h"
#include "UIExtension.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::Linq;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			public interface class ITask
			{
			public:
				virtual property UInt32 Id		{ UInt32 get(); }
				virtual property String^ Title	{ String^ get(); }
				virtual property int Depth		{ int get(); }
				virtual property bool HasIcon	{ bool get(); }
			};

			public ref class TaskComboBox : Windows::Forms::ComboBox
			{
			public:
				TaskComboBox();

				void Initialise(IEnumerable<ITask^>^ taskItemsSortedByPosition, 
								UIExtension::TaskIcon^ taskIcons, 
								UInt32 selTaskId);

				property UInt32 SelectedTaskId { UInt32 get(); }

			private:
				UIExtension::TaskIcon^ m_TaskIcons;

			protected:
				void OnMeasureItem(Windows::Forms::MeasureItemEventArgs^ e) override;
				void OnDrawItem(Windows::Forms::DrawItemEventArgs^ e) override;
			};
		}
	}
}
