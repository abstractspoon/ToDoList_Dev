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
				virtual property UInt32 Id { UInt32 get(); }
				virtual property String^ Title		{ String^ get(); }
				virtual property String^ Position	{ String^ get(); }
				virtual property int Depth			{ int get(); }
				virtual property bool HasIcon		{ bool get(); }
			};

			public ref class TaskComboBox : UIComponents::OwnerdrawComboBoxBase
			{
			public:
				TaskComboBox();

				void Initialise(IEnumerable<ITask^>^ taskItems, 
								UIExtension::TaskIcon^ taskIcons, 
								UInt32 selTaskId);

				void Initialise(IEnumerable<ITask^>^ taskItems,
								UIExtension::TaskIcon^ taskIcons,
								UInt32 selTaskId, 
								ITask^ noneTask);

				property UInt32 SelectedTaskId { UInt32 get(); }
				property String^ SelectedTaskTitle { String^ get(); }

			private:
				UIExtension::TaskIcon^ m_TaskIcons;
				ITask^ m_NoneTask;

			protected:
				void OnMeasureItem(Windows::Forms::MeasureItemEventArgs^ e) override;
				void OnDrawItem(Windows::Forms::DrawItemEventArgs^ e) override;
				void OnTextChanged(EventArgs^ e) override;

			protected:
				void SelectNextFind(bool forward);
				bool SelectNextItem(String^ text, bool forward);
				int FindNextItem(String^ text, int from, bool forward);
				bool FindNextItem(String^ text, int from, int to, int increment, int& next);
			};
		}
	}
}
