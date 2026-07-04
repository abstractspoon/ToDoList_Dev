#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

#include "Translator.h"
#include "UIExtension.h"
#include "ITaskBase.h"
#include "LabelTip.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::Collections::Generic;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			public delegate bool EditTaskLabelEventHandler(Object^ sender, ITaskBase^ task);
			public delegate bool EditTaskIconEventHandler(Object^ sender, ITaskBase^ task);
			public delegate bool EditTaskCompletionEventHandler(Object^ sender, ITaskBase^ task);
			public delegate bool IsTaskDraggableEventHandler(Object^ sender, ITaskBase^ task);

			// ---------------------------------------------

			// Forward declaration
			ref class HeaderControl;

			// ---------------------------------------------

			public ref class TaskListView : Windows::Forms::ListView, ILabelTipHandler
			{
			public:
				TaskListView();

				void Initialize(Translator^ trans, UIExtension::TaskIcon^ taskIcons);

				Windows::Forms::ListViewItem^ AddTask(ITaskBase^ base);
				bool RemoveTask(UInt32 taskId);

				ITaskBase^ GetTask(int index);
				ITaskBase^ HitTestTask(Drawing::Point ptScreen, bool icon);
				bool PointInHeader(Drawing::Point ptScreen);
				UInt32 GetTaskId(int index);
				UInt32 GetTaskIdEx(UIExtension::GetTask getTask, bool fromSelTask);
				UInt32 GetNextTaskId(int index, bool next, bool topLevel);
				bool HasTaskId(UInt32 taskId);
				Drawing::Rectangle GetTaskLabelRect(UInt32 taskId);
				Drawing::Rectangle GetTaskLabelRect(int index);
				void ResizeTaskColumnToFit();

				bool SelectTask(UInt32 taskId);
				bool SelectTaskEx(String^ words, UIExtension::SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace);
				bool SelectTasks(IList<UInt32>^ taskIDs);
				void EnsureSelectionVisible();

				property UInt32 SelectedTaskId { UInt32 get(); }
				property String^ SelectedTaskTitle { String^ get(); }
				property ITaskBase^ SelectedTask { ITaskBase^ get(); }

				property int SelectionCount { int get(); }
				property bool HasSelection { bool get(); }
				property List<UInt32>^ SelectedTaskIds { List<UInt32>^ get(); }

				property bool TaskColorIsBackground { bool get(); void set(bool value); }
				property bool ShowParentsAsFolders { bool get(); void set(bool value); }
				property bool ShowCompletionCheckboxes { bool get(); void set(bool value); }
				property bool ShowLabelTips { bool get(); void set(bool value); }
				property bool IsBoundSelecting { bool get() { return (m_BoundSelectionTimer != nullptr); } }
				property bool EnableHeaderTracking { bool get(); void set(bool value); }
				property bool SizeTaskColumnToFit { bool get(); void set(bool value); }
				property bool ReadOnly { bool get(); void set(bool value); }

				property int MinTaskColumnWidth;

				property Drawing::Rectangle SelectedTaskLabelRect { Drawing::Rectangle get(); }
				property Drawing::Color GridlineColor { Drawing::Color get(); void set(Drawing::Color value); }
				property Drawing::Color AlternateLineColor { Drawing::Color get(); void set(Drawing::Color value); }

				// ILabelTipHandler
				virtual Windows::Forms::Control^ GetOwner();
				virtual LabelTipInfo^ ToolHitTest(Drawing::Point ptScreen);

				// Callbacks
				event EditTaskLabelEventHandler^ EditTaskLabel;
				event EditTaskIconEventHandler^ EditTaskIcon;
				event EditTaskCompletionEventHandler^ EditTaskDone;
				event IsTaskDraggableEventHandler^ IsTaskDraggable;

				event EventHandler^ BoundSelectionEnded;

			private:
				HeaderControl^ m_HeaderCtrl;

			protected:
				Translator^ m_Trans;
				UIExtension::TaskIcon^ m_TaskIcons;
				LabelTip^ m_LabelTip;
				Windows::Forms::Timer^ m_BoundSelectionTimer;

				Drawing::Font^ m_BoldFont;
				Drawing::Color m_GridlineColor;
				Drawing::Color m_AlternateLineColor;

				bool m_ItemsHaveIcons;
				bool m_ShowParentsAsFolders;
				bool m_TaskColorIsBkgnd;
				bool m_ShowCompletionCheckboxes;
				bool m_EnableHeaderTracking;
				bool m_SizeTaskColumnToFit;
				bool m_ReadOnly;

				int m_CheckBoxSize;

			protected:
				void WndProc(Windows::Forms::Message% m) override;

				void OnMouseMove(Windows::Forms::MouseEventArgs^ e) override;
				void OnBeforeLabelEdit(Windows::Forms::LabelEditEventArgs^ e) override;
				void OnDrawItem(Windows::Forms::DrawListViewItemEventArgs^ e) override;
				void OnItemDrag(Windows::Forms::ItemDragEventArgs^ e) override;

				void OnGotFocus(EventArgs^ e) override;
				void OnLostFocus(EventArgs^ e) override;
				void OnSizeChanged(EventArgs^ e) override;
				void OnFontChanged(EventArgs^ e) override;
				void OnHandleCreated(EventArgs^ e) override;

				void OnBoundSelectionTimer(Object^ sender, EventArgs^ e);

			protected:
				enum class LabelExtents
				{
					TitleTextOnly,
					TitleColumn,
					AllColumns,
				};

				Drawing::Rectangle CalcLabelRect(Windows::Forms::ListViewItem^ item, LabelExtents extents);
				Drawing::Rectangle CalcCheckboxRect(Drawing::Rectangle labelRect);
				Drawing::Rectangle CalcIconRect(Drawing::Rectangle labelRect);

				String^ Translate(String^ text, Translator::Type type);
				bool IsTaskEditable(ITaskBase^ task) { return (!m_ReadOnly && (task != nullptr) && !task->IsLocked); }
				int FindTask(String^ phrase, int startIndex, bool forward, bool caseSensitive, bool wholeWord, bool findReplace);
				Windows::Forms::ListViewItem^ FindItem(UInt32 taskId);
				Drawing::Color GetTextColor(ITaskBase^ task, bool selected);
				Drawing::Color GetBackColor(ITaskBase^ task, int row);
				Drawing::Font^ GetFont(ITaskBase^ task, bool title);

				// Derived classes optionally override
				virtual bool TaskMatches(ITaskBase^ task, String^ phrase, bool caseSensitive, bool wholeWord, bool findReplace);
				virtual Windows::Forms::VisualStyles::CheckBoxState GetTaskCheckboxState(ITaskBase^ task);
				virtual bool IsItemSelected(Windows::Forms::ListViewItem^ lvItem) { return lvItem->Selected; }

				property UIExtension::TaskIcon^ TaskIcons { UIExtension::TaskIcon^ get() { return m_TaskIcons; } }
				property bool ItemsHaveIcons { bool get(); void set(bool value); };
				property int TextIconOffset { int get(); }
				property int CheckboxOffset	{ int get(); }
				property int ImageSize { int get(); }
				property int LabelPadding { int get() { return 2; } }

				property int FirstSelectedIndex { int get(); }
				property int LastSelectedIndex { int get(); }
				property int LastIndex { int get(); }
			};
		}
	}
}
