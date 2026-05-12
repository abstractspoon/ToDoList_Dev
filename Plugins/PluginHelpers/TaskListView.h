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
				ref class NoTrackHeaderControl : Windows::Forms::NativeWindow
				{
				public:
					NoTrackHeaderControl(TaskListView^ lv);

				protected:
					void WndProc(Windows::Forms::Message% m) override;
				};

			public:
				TaskListView();

				void Initialize(Translator^ trans, UIExtension::TaskIcon^ taskIcons);
				IntPtr GetHeaderHandle();

				Windows::Forms::ListViewItem^ AddTask(ITaskBase^ base);

				ITaskBase^ GetTask(int index);
				UInt32 GetTaskId(int index);
				UInt32 GetTaskIdEx(UIExtension::GetTask getTask);
				bool HasTaskId(UInt32 taskId);

				bool SelectTask(UInt32 taskId);
				bool SelectTaskEx(String^ words, UIExtension::SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace);
				void EnsureSelectionVisible();
				Drawing::Rectangle GetSelectedTaskLabelRect(bool screenCoords);

				property UInt32 SelectedTaskId { UInt32 get(); }
				property String^ SelectedTaskTitle { String^ get(); }
				property ITaskBase^ SelectedTask { ITaskBase^ get(); }

				property bool TaskColorIsBackground { bool get(); void set(bool value); }
				property bool ShowParentsAsFolders { bool get(); void set(bool value); }
				property bool ShowCompletionCheckboxes { bool get(); void set(bool value); }
				property bool ShowLabelTips { bool get(); void set(bool value); }

				property Drawing::Color GridlineColor { Drawing::Color get(); void set(Drawing::Color value); }
				property Drawing::Color AlternateLineColor { Drawing::Color get(); void set(Drawing::Color value); }

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

				Drawing::Font^ m_BoldFont;
				Drawing::Color m_GridlineColor;
				Drawing::Color m_AlternateLineColor;

				bool m_ItemsHaveIcons;
				bool m_ShowParentAsFolder;
				bool m_TaskColorIsBkgnd;
				bool m_ShowCompletionCheckboxes;
				int m_CheckBoxSize;

			protected:
				void WndProc(Windows::Forms::Message% m) override;

				void OnMouseDown(Windows::Forms::MouseEventArgs^ e) override;
				void OnMouseClick(Windows::Forms::MouseEventArgs^ e) override;
				void OnMouseDoubleClick(Windows::Forms::MouseEventArgs^ e) override;
				void OnMouseMove(Windows::Forms::MouseEventArgs^ e) override;
				void OnBeforeLabelEdit(Windows::Forms::LabelEditEventArgs^ e) override;
				void OnColumnWidthChanging(Windows::Forms::ColumnWidthChangingEventArgs^ e) override;
				void OnMeasureItem(Windows::Forms::MeasureItemEventArgs^ e) override;
				void OnDrawItem(Windows::Forms::DrawListViewItemEventArgs^ e) override;
				void OnDrawColumnHeader(Windows::Forms::DrawListViewColumnHeaderEventArgs^ e) override;
				void OnGotFocus(EventArgs^ e) override;
				void OnLostFocus(EventArgs^ e) override;
				void OnSizeChanged(EventArgs^ e) override;
				void OnFontChanged(EventArgs^ e) override;

			protected:
				Drawing::Rectangle CalcLabelTextRect(Drawing::Rectangle labelRect, bool includeSubItems);
				Drawing::Rectangle CalcCheckboxRect(Drawing::Rectangle labelRect);
				Drawing::Rectangle CalcIconRect(Drawing::Rectangle labelRect);

				String^ Translate(String^ text, Translator::Type type);
				void HandleMouseClick(Windows::Forms::MouseEventArgs^ e, bool doubleClick);
				Windows::Forms::ListViewItem^ FindLVItem(UInt32 taskId);
				int FindTask(String^ phrase, int startIndex, bool forward, bool caseSensitive, bool wholeWord, bool findReplace);
				Drawing::Color GetTextColor(ITaskBase^ task, bool selected);
				Drawing::Color GetBackColor(ITaskBase^ task, int row);
				UInt32 GetNextTaskId(UInt32 taskId, bool next);
				
				// Derived classes optionally override
				virtual bool TaskMatches(ITaskBase^ task, String^ phrase, bool caseSensitive, bool wholeWord, bool findReplace);
				virtual Windows::Forms::VisualStyles::CheckBoxState GetTaskCheckboxState(ITaskBase^ task);
				virtual bool IsItemSelected(Windows::Forms::ListViewItem^ lvItem) { return lvItem->Selected; }

				property UIExtension::TaskIcon^ TaskIcons { UIExtension::TaskIcon^ get() { return m_TaskIcons; } }
				property bool ItemsHaveIcons { bool get(); void set(bool value); };
				property int TextIconOffset { int get(); }
				property int CheckboxOffset	{ int get(); }
				property int ImageSize { int get(); }
			};
		}
	}
}
