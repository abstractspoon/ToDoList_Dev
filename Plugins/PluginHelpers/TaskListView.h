#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

#include "Translator.h"
#include "UIExtension.h"
#include "ITaskBase.h"
#include "LabelTip.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			public delegate bool EditTaskLabelEventHandler(Object^ sender, UInt32 taskId);
			public delegate bool EditTaskIconEventHandler(Object^ sender, UInt32 taskId);
			public delegate bool EditTaskCompletionEventHandler(Object^ sender, UInt32 taskId, bool completed);

			// ---------------------------------------------

			public ref class TaskListView : Windows::Forms::ListView, ILabelTipHandler
			{
			public:
				TaskListView();

				void Initialize(Translator^ trans, UIExtension::TaskIcon^ taskIcons);

				property UInt32 SelectedTaskId { UInt32 get(); }
				property String^ SelectedTaskTitle { String^ get(); }

				property bool TaskColorIsBackground { bool get(); void set(bool value); }
				property bool ShowParentsAsFolders { bool get(); void set(bool value); }
				property bool ShowCompletionCheckboxes { bool get(); void set(bool value); }
				property bool ShowMixedCompletionState { bool get(); void set(bool value); }
				property bool ShowLabelTips { bool get(); void set(bool value); }

				// ILabelTipHandler
				virtual Windows::Forms::Control^ GetOwner();
				virtual LabelTipInfo^ ToolHitTest(Drawing::Point ptScreen);

				// Callbacks
				event EditTaskLabelEventHandler^ EditTaskLabel;
				event EditTaskIconEventHandler^ EditTaskIcon;
				event EditTaskCompletionEventHandler^ EditTaskDone;

			private:
				Translator^ m_Trans;
				UIExtension::TaskIcon^ m_TaskIcons;
				LabelTip^ m_LabelTip;
				Windows::Forms::ImageList^ m_ilItemHeight;
				Drawing::Font^ m_BoldFont;

				bool m_ItemsHaveIcons;
				bool m_ShowParentAsFolder;
				bool m_TaskColorIsBkgnd;
				bool m_ShowCompletionCheckboxes;
				bool m_ShowMixedCompletionState;

			protected:
				void WndProc(Windows::Forms::Message% m) override;

				void OnMouseDown(Windows::Forms::MouseEventArgs^ e) override;
				void OnMouseClick(Windows::Forms::MouseEventArgs^ e) override;
				void OnMouseDoubleClick(Windows::Forms::MouseEventArgs^ e) override;
				void OnMouseMove(Windows::Forms::MouseEventArgs^ e) override;
				void OnBeforeLabelEdit(Windows::Forms::LabelEditEventArgs^ e) override;

			protected:
// 				void OnMeasureItem(Windows::Forms::MeasureItemEventArgs^ e) override;
// 				void OnDrawItem(Windows::Forms::DrawItemEventArgs^ e) override;
				Drawing::Rectangle CalcLabelTextRect(Drawing::Rectangle labelRect, bool includeIdColumn);
				Drawing::Rectangle CalcCheckboxRect(Drawing::Rectangle labelRect);
				Drawing::Rectangle CalcIconRect(Drawing::Rectangle labelRect);

				String^ Translate(String^ text, Translator::Type type);
				void HandleMouseClick(Windows::Forms::MouseEventArgs^ e, bool doubleClick);

				property UIExtension::TaskIcon^ TaskIcons { UIExtension::TaskIcon^ get() { return m_TaskIcons; } }
				property bool ItemsHaveIcons { bool get(); void set(bool value); };
				property int TextIconOffset { int get(); }
				property int CheckboxOffset	{ int get(); }
				property int ImageSize { int get(); }
			};
		}
	}
}
