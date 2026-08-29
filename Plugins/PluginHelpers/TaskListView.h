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
			public delegate void SelectionChangeEventHandler(Object^ sender, IList<UInt32>^ taskIds);

			// ---------------------------------------------

			// Forward declaration
			ref class HeaderControl;

			// ---------------------------------------------

			public ref class TaskListView : Windows::Forms::ListView, ILabelTipHandler
			{

			public:
				interface class IItemComparer : System::Collections::IComparer
				{
				public:
					virtual property int Column { int get(); void set(int col); }
					virtual property bool Ascending { bool get(); void set(bool ascending); }
				};

				// ---------------------------------------------------

			public:
				TaskListView();

				void Initialize(Translator^ trans, UIExtension::TaskIcon^ taskIcons);
				void Initialize(Translator^ trans, UIExtension::TaskIcon^ taskIcons, IItemComparer^ comparer);

				Windows::Forms::ListViewItem^ AddTask(ITaskBase^ base);
				Windows::Forms::ListViewItem^ AddTask(ITaskBase^ base, String^ key);
				bool RemoveTask(UInt32 taskId);

				ITaskBase^ GetTask(int index);
				bool HitTest(Drawing::Point ptScreen, UIExtension::HitTest^ hitTest);
				UInt32 GetTaskId(int index);
				UInt32 GetTaskIdEx(UIExtension::GetTask getTask, bool fromSelTask);
				UInt32 GetNextTaskId(int index, bool next, bool topLevel);
				bool HasTaskId(UInt32 taskId);
				Drawing::Rectangle GetTaskLabelRect(UInt32 taskId);
				void ResizeTaskColumnToFit();

				Drawing::Bitmap^ SaveToImage();
				Drawing::Bitmap^ SaveToImage(int reqWidth);
				int GetRequiredWidthForImage();

				bool SelectTask(UInt32 taskId);
				bool SelectTaskEx(String^ words, UIExtension::SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace);
				bool SelectTasks(IList<UInt32>^ taskIds);
				void EnsureSelectionVisible();

				property UInt32 SelectedTaskId { UInt32 get(); }
				property String^ SelectedTaskTitle { String^ get(); }
				property ITaskBase^ SelectedTask { ITaskBase^ get(); }

				property int SelectionCount { int get(); }
				property bool HasSelection { bool get(); }
				property IList<UInt32>^ SelectedTaskIds { IList<UInt32>^ get(); }
				property IList<ITaskBase^>^ SelectedTasks { IList<ITaskBase^>^ get(); }

				property bool TaskColorIsBackground { bool get(); void set(bool value); }
				property bool ShowParentsAsFolders { bool get(); void set(bool value); }
				property bool ShowCompletionCheckboxes { bool get(); void set(bool value); }
				property bool StrikeThruCompletedTasks { bool get(); void set(bool value); }
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
				event SelectionChangeEventHandler^ SelectionChange;

				// -----------------------------------------------

				ref class UpdateState
				{
				public:
					System::Collections::IComparer^ Sorter;
					Windows::Forms::ListViewItem^ TopItem;
					IList<UInt32>^ SelectedTaskIds;
				};

				UpdateState^ BeginUpdate();
				void EndUpdate(UpdateState^ state);

			private:
				HeaderControl^ m_HeaderCtrl;

			protected:
				Translator^ m_Trans;
				UIExtension::TaskIcon^ m_TaskIcons;
				LabelTip^ m_LabelTip;
				Windows::Forms::Timer^ m_BoundSelectionTimer;

				Drawing::Color m_GridlineColor;
				Drawing::Color m_AlternateLineColor;

				bool m_ItemsHaveIcons;
				bool m_ShowParentsAsFolders;
				bool m_TaskColorIsBkgnd;
				bool m_ShowCompletionCheckboxes;
				bool m_StrikeThruCompletedTasks;
				bool m_EnableHeaderTracking;
				bool m_SizeTaskColumnToFit;
				bool m_ReadOnly;
				bool m_SavingToImage;

				int m_CheckBoxSize;

			protected:
				void WndProc(Windows::Forms::Message% m) override;

				void OnMouseMove(Windows::Forms::MouseEventArgs^ e) override;
				void OnBeforeLabelEdit(Windows::Forms::LabelEditEventArgs^ e) override;
				void OnDrawItem(Windows::Forms::DrawListViewItemEventArgs^ e) override;
				void OnItemDrag(Windows::Forms::ItemDragEventArgs^ e) override;
				void OnItemSelectionChanged(Windows::Forms::ListViewItemSelectionChangedEventArgs^ e) override;
				void OnKeyUp(Windows::Forms::KeyEventArgs^ e) override;

				void OnGotFocus(EventArgs^ e) override;
				void OnLostFocus(EventArgs^ e) override;
				void OnSizeChanged(EventArgs^ e) override;
				void OnFontChanged(EventArgs^ e) override;
				void OnHandleCreated(EventArgs^ e) override;
				void OnSelectedIndexChanged(EventArgs^ e) override;

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
				void CheckNotifySelectionChanged();
				void ResizeTaskColumnToFit(int width);

				// Derived classes optionally override
				virtual bool TaskMatches(ITaskBase^ task, String^ phrase, bool caseSensitive, bool wholeWord, bool findReplace);
				virtual bool IsItemSelected(Windows::Forms::ListViewItem^ lvItem) { return (!m_SavingToImage && lvItem->Selected); }
				virtual Windows::Forms::VisualStyles::CheckBoxState GetTaskCheckboxState(ITaskBase^ task);
				virtual Windows::Forms::TextFormatFlags GetTextAlignment(int column) { return Windows::Forms::TextFormatFlags::Left; }

				property UIExtension::TaskIcon^ TaskIcons { UIExtension::TaskIcon^ get() { return m_TaskIcons; } }
				property bool ItemsHaveIcons { bool get(); void set(bool value); };
				property int TextIconOffset { int get(); }
				property int CheckboxOffset	{ int get(); }
				property int LabelPadding { int get() { return 2; } }

				property int FirstSelectedIndex { int get(); }
				property int LastSelectedIndex { int get(); }
				property int LastIndex { int get(); }

				// ---------------------------------------------------

				ref class DefaultItemComparer : IItemComparer
				{
				public:
					DefaultItemComparer();

					// IComparer
					virtual int Compare(Object^ x, Object^ y);

					// IItemComparer
					virtual property int Column { int get(); void set(int col); }
					virtual property bool Ascending { bool get(); void set(bool ascending); }

				protected:
					virtual int CompareItems(Windows::Forms::ListViewItem^ lvi1,
											 Windows::Forms::ListViewItem^ lvi2);
				private:
					int m_Column;
					bool m_Ascending;
				};

			};

			///////////////////////////////////////////////////////////


		}
	}
}
