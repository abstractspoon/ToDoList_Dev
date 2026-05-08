#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

#include "Translator.h"
#include "UIExtension.h"
#include "ITaskBase.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			public delegate void SearchUpdated(Object^ sender, EventArgs^ args);

			// --------------------------------------------------------

			public ref class TaskComboBox : UIComponents::OwnerdrawComboBoxBase
			{
			public:
				TaskComboBox();

				void Initialise(IEnumerable<ITaskBase^>^ taskItems, 
								UIExtension::TaskIcon^ taskIcons, 
								UInt32 selTaskId);

				void Initialise(IEnumerable<ITaskBase^>^ taskItems,
								UIExtension::TaskIcon^ taskIcons,
								UInt32 selTaskId, 
								ITaskBase^ noneTask);

				property UInt32 SelectedTaskId { UInt32 get(); }
				property String^ SelectedTaskTitle { String^ get(); }

				bool PreProcessMessage(Windows::Forms::Message% msg) override;

				event SearchUpdated^ SearchUpdated;

			private:
				UIExtension::TaskIcon^ m_TaskIcons;
				ITaskBase^ m_NoneTask;
				Drawing::Font^ m_BoldFont;

				int m_OrgSelectedIndex;
				bool m_FindEnabled;

			protected:
				void OnMeasureItem(Windows::Forms::MeasureItemEventArgs^ e) override;
				void OnDrawItem(Windows::Forms::DrawItemEventArgs^ e) override;
				void OnTextChanged(EventArgs^ e) override;

				int GetListItemTextOffset(Object^ obj) override;
				int GetListItemTextLength(Object^ obj, Drawing::Graphics^ graphics) override;

			protected:
				void SelectNextFind(bool forward);
			};
		}
	}
}
