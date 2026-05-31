
#include "stdafx.h"
#include "TaskListView.h"
#include "Win32.h"
#include "DPIScaling.h"
#include "ColorUtil.h"
#include "PluginHelpers.h"

#include <shared\Clipboard.h>
#include <shared\Misc.h>
#include <shared\GraphicsMisc.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Diagnostics;
using namespace System::Drawing;
using namespace System::Linq;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Windows::Forms::VisualStyles;

using namespace Abstractspoon::Tdl::PluginHelpers;
using namespace Abstractspoon::Tdl::PluginHelpers::ColorUtil;

////////////////////////////////////////////////////////////////////////////////////////////////
// Private classes

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			ref class DefaultItemComparer : System::Collections::IComparer
			{
			public:
				DefaultItemComparer() : m_Column(0), m_Ascending(true) {}
				DefaultItemComparer(int column, bool ascending) : m_Column(column), m_Ascending(ascending) {}

				virtual int Compare(Object^ x, Object^ y)
				{
					int res = StringUtil::NaturalCompare(ASTYPE(x, ListViewItem)->SubItems[m_Column]->Text, 
														 ASTYPE(y, ListViewItem)->SubItems[m_Column]->Text);

					return (m_Ascending ? res : -res);
				}

				property int Column { int get() { return m_Column; } }
				property bool Ascending { bool get() { return m_Ascending; } }

			private:
				int m_Column;
				bool m_Ascending;
			};

			////////////////////////////////////////////////////////////////////////////////////////////////

			// Private class (for now)
			ref class HeaderControl : Windows::Forms::NativeWindow
			{
			public:
				HeaderControl(ListView^ list) : m_List(list)
				{
					Debug::Assert(list->IsHandleCreated);

					auto header = IntPtr(::SendMessage(Win32::GetHwnd(m_List->Handle), LVM_GETHEADER, 0, 0));
					AssignHandle(header);

					list->ColumnClick += gcnew ColumnClickEventHandler(this, &HeaderControl::OnColumnClick);
					list->DrawColumnHeader += gcnew DrawListViewColumnHeaderEventHandler(this, &HeaderControl::OnDrawColumnHeader);
					list->ColumnWidthChanging += gcnew ColumnWidthChangingEventHandler(this, &HeaderControl::OnColumnWidthChanging);
				}

			public:
				property bool EnableTracking;

			private:
				ListView^ m_List;

			protected:
				void OnDrawColumnHeader(Object^ sender, DrawListViewColumnHeaderEventArgs^ e)
				{
					Debug::Assert(sender == m_List);

					auto sorter = ASTYPE(m_List->ListViewItemSorter, DefaultItemComparer);

					if ((sorter == nullptr) || (sorter->Column != e->ColumnIndex))
					{
						e->DrawDefault = true;
						return;
					}

					e->DrawBackground();

					if (e->Header == nullptr)
						return;

					// Reimplement DrawListViewColumnHeaderEventArgs::DrawText()
					// to avoid it adding unnecessary extra padding
					HorizontalAlignment hAlign = HorizontalAlignment::Left;
					TextFormatFlags flags = (TextFormatFlags::Left | TextFormatFlags::VerticalCenter | TextFormatFlags::WordEllipsis);

					switch (e->Header->TextAlign)
					{
					case HorizontalAlignment::Center: flags = (flags | TextFormatFlags::HorizontalCenter); break;
					case HorizontalAlignment::Right: flags = (flags | TextFormatFlags::Right); break;
					}

					TextRenderer::DrawText(e->Graphics,
										   e->Header->Text,
										   e->Font,
										   Rectangle::Inflate(e->Bounds, -2/*LabelPadding*/, -1),
										   e->ForeColor,
										   flags);

					// Sort arrow
					HDC hDC = Win32::GetHdc(e->Graphics->GetHdc());
					CDC* pDC = CDC::FromHandle(hDC);

					GraphicsMisc::DrawSortArrow(pDC, 
												CRect(e->Bounds.Left, e->Bounds.Top, e->Bounds.Right, e->Bounds.Bottom),
												(sorter->Ascending ? true : false));

					e->Graphics->ReleaseHdc();
				}

				void OnColumnClick(Object^ sender, ColumnClickEventArgs^ e)
				{
					Debug::Assert(sender == m_List);

					auto sorter = ASTYPE(m_List->ListViewItemSorter, DefaultItemComparer);

					if (sorter == nullptr)
					{
						if (m_List->ListViewItemSorter != nullptr)
							return; // not 'our' sorter

						// else 
						sorter = gcnew DefaultItemComparer(e->Column, true);
					}
					else if (e->Column != sorter->Column)
					{
						sorter = gcnew DefaultItemComparer(e->Column, true);

						// Clear the previous sort arrow
						::InvalidateRect(Win32::GetHwnd(Handle), NULL, FALSE);
					}
					else
					{
						sorter = gcnew DefaultItemComparer(e->Column, !sorter->Ascending);
					}

					m_List->ListViewItemSorter = sorter;
				}

				void OnColumnWidthChanging(Object^ sender, ColumnWidthChangingEventArgs^ e)
				{
					Debug::Assert(sender == m_List);

					if (!EnableTracking && (Control::MouseButtons == Windows::Forms::MouseButtons::Left))
					{
						e->Cancel = true;
						e->NewWidth = m_List->Columns[e->ColumnIndex]->Width;
					}
				}

				void WndProc(Windows::Forms::Message% m) override
				{
					switch (m.Msg)
					{
					case WM_SETCURSOR:
						if (!EnableTracking)
						{
							Win32::SetArrowCursor();
							return;
						}
						break;
					}

					NativeWindow::WndProc(m);
				}
			};
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

const int IMAGE_SIZE = DPIScaling::Scale(16);

////////////////////////////////////////////////////////////////////////////////////////////////

TaskListView::TaskListView()
	:
	ListView(),
	m_Trans(nullptr),
	m_TaskIcons(nullptr),
	m_LabelTip(nullptr),
	m_BoldFont(nullptr),
	m_ItemsHaveIcons(false),
	m_ShowParentAsFolder(false),
	m_TaskColorIsBkgnd(false),
	m_ShowCompletionCheckboxes(false),
	m_BoundSelectionTimer(nullptr),
	m_GridlineColor(Color::Empty),
	m_AlternateLineColor(Color::Empty),
	m_EnableHeaderTracking(true),
	m_SizeTaskColumnToFit(false),
	m_ReadOnly(false),
	m_CheckBoxSize(-1)
{
	m_LabelTip = gcnew LabelTip(this);

	MinTaskColumnWidth = -1;
}

void TaskListView::Initialize(Translator^ trans, UIExtension::TaskIcon^ taskIcons)
{
	m_Trans = trans;
	m_TaskIcons = taskIcons;

	View = System::Windows::Forms::View::Details;
	FullRowSelect = true;
	HideSelection = false;
	OwnerDraw = true;
	Sorting = SortOrder::Ascending;
	HeaderStyle = ColumnHeaderStyle::Clickable;
	DoubleBuffered = true;
	HotTracking = false;
	HoverSelection = false;
}

void TaskListView::OnHandleCreated(EventArgs^ e)
{
	ListView::OnHandleCreated(e);

	m_HeaderCtrl = gcnew HeaderControl(this);
	m_HeaderCtrl->EnableTracking = m_EnableHeaderTracking;
}

bool TaskListView::EnableHeaderTracking::get()
{
	return m_EnableHeaderTracking;
}

void TaskListView::EnableHeaderTracking::set(bool value)
{
	m_EnableHeaderTracking = value;

	if (m_HeaderCtrl != nullptr)
		m_HeaderCtrl->EnableTracking = value;
}

bool TaskListView::SizeTaskColumnToFit::get()
{
	return m_SizeTaskColumnToFit;
}

void TaskListView::SizeTaskColumnToFit::set(bool value)
{
	if (value != m_SizeTaskColumnToFit)
	{
		m_SizeTaskColumnToFit = value;

		if (Columns->Count > 0)
			ResizeTaskColumnToFit();
	}
}

ListViewItem^ TaskListView::AddTask(ITaskBase^ task)
{
	auto lvItem = gcnew ListViewItem(task->Title);

	lvItem->Tag = task;
	lvItem->Selected = false;

	if ((task->IsParent && ShowParentsAsFolders) || task->HasIcon)
	{
		lvItem->ImageIndex = 1; // placeholder only
		ItemsHaveIcons = true;
	}

	return Items->Add(lvItem);
}

bool TaskListView::RemoveTask(UInt32 taskId)
{
	auto lvItem = FindItem(taskId);

	if (lvItem == nullptr)
		return false;

	Items->Remove(lvItem);
	return true;
}

UInt32 TaskListView::GetTaskIdEx(UIExtension::GetTask getTask, bool fromSelTask)
{
	bool next = true, topLevel = true;

	switch (getTask)
	{
	case UIExtension::GetTask::GetNextTask:
	case UIExtension::GetTask::GetNextVisibleTask:
		topLevel = false;
		break;

	case UIExtension::GetTask::GetNextTopLevelTask:
		break;

	case UIExtension::GetTask::GetPrevTask:
	case UIExtension::GetTask::GetPrevVisibleTask:
		next = false;
		topLevel = false;
		break;

	case UIExtension::GetTask::GetPrevTopLevelTask:
		next = false;
		break;
	};

	int startIndex = -1;

	if (fromSelTask && (SelectedTaskId != 0))
		startIndex = (next ? LastSelectedIndex : FirstSelectedIndex);
	else
		startIndex = (next ? -1 : Items->Count); // intentianally just beyond/before the item range

	return GetNextTaskId(startIndex, next, topLevel);
}

UInt32 TaskListView::GetNextTaskId(int index, bool next, bool topLevel)
{
	// Don't pre-validate 'index'; it's allowed to be just
	// beyond or before the item range
	ITaskBase^ task = nullptr;

	do 
	{
		index += (next ? 1 : -1);
		task = GetTask(index);

		if (task != nullptr)
		{
			if (!topLevel || ITaskBaseExt::IsTopLevel(task))
				return task->Id;
		}
	} 
	while (task != nullptr);

	return 0;
}

bool TaskListView::HasTaskId(UInt32 taskId)
{
	return (FindItem(taskId) != nullptr);
}

ITaskBase^ TaskListView::GetTask(int index)
{
	if ((index < 0) || (index > LastIndex))
		return nullptr;

	return ASTYPE(Items[index]->Tag, ITaskBase);
}

UInt32 TaskListView::GetTaskId(int index)
{
	auto task = GetTask(index);
	return ((task == nullptr) ? 0 : task->Id);
}

bool TaskListView::SelectTask(UInt32 taskId)
{
	auto taskIds = gcnew List<UInt32>();
	taskIds->Add(taskId);

	return SelectTasks(taskIds);
}

bool TaskListView::SelectTasks(IList<UInt32>^ taskIds)
{
	Debug::Assert((MultiSelect == true) || (taskIds->Count <= 1));

	SelectedItems->Clear();
	SelectedIndices->Clear();

	bool first = true;

	for each(auto taskId in taskIds)
	{
		ListViewItem^ lvItem = FindItem(taskId);

		if (lvItem == nullptr)
			return false;

		lvItem->Selected = true;
		lvItem->Focused = first;

		first = false;
	}

	EnsureSelectionVisible();
	return true;
}

void TaskListView::EnsureSelectionVisible()
{
	if ((SelectionCount == 0) || (Items->Count == 0))
		return;

	int itemIndex = SelectionCount;

	while (itemIndex-- > 0)
	{
		Drawing::Rectangle itemRect = SelectedItems[itemIndex]->Bounds;

		if (Drawing::Rectangle::Intersect(ClientRectangle, itemRect) == itemRect)
			return;
	}

	// else
	EnsureVisible(FirstSelectedIndex);
}

int TaskListView::SelectionCount::get() 
{ 
	return SelectedItems->Count;
}

bool TaskListView::HasSelection::get()
{
	return (SelectionCount > 0);
}

List<UInt32>^ TaskListView::SelectedTaskIds::get()
{
	auto taskIds = gcnew List<UInt32>();

	for each (int index in SelectedIndices)
		taskIds->Add(GetTaskId(index));

	return taskIds;
}

UInt32 TaskListView::SelectedTaskId::get()
{
	auto selTask = SelectedTask;
	return (selTask == nullptr ? 0 : selTask->Id);
}

String^ TaskListView::SelectedTaskTitle::get()
{
	auto selTask = SelectedTask;
	return (selTask == nullptr ? String::Empty : selTask->Title);
}

ITaskBase^ TaskListView::SelectedTask::get()
{
	Debug::Assert((MultiSelect == false) || (SelectionCount <= 1));

	if (SelectionCount == 0)
		return nullptr;

	// else 
	return ASTYPE(SelectedItems[0]->Tag, ITaskBase);
}

Drawing::Rectangle TaskListView::SelectedTaskLabelRect::get()
{
	Debug::Assert((MultiSelect == false) || (SelectionCount <= 1));

	if (SelectionCount == 0)
		return Drawing::Rectangle::Empty;

	// else 
	return CalcLabelRect(SelectedItems[0], TaskListView::LabelExtents::TitleColumn);
}

Drawing::Rectangle TaskListView::GetTaskLabelRect(UInt32 taskId)
{
	auto item = FindItem(taskId);

	if (item == nullptr)
		return Drawing::Rectangle::Empty;

	// else
	return CalcLabelRect(item, TaskListView::LabelExtents::TitleColumn);
}

Drawing::Rectangle TaskListView::GetTaskLabelRect(int index)
{
	if ((index < 0) || (index > LastIndex))
		return Drawing::Rectangle::Empty;

	// else 
	return CalcLabelRect(Items[index], TaskListView::LabelExtents::TitleColumn);
}

ListViewItem^ TaskListView::FindItem(UInt32 taskId)
{
	for each(ListViewItem^ lvItem in Items)
	{
		auto task = ASTYPE(lvItem->Tag, ITaskBase);

		if ((task != nullptr) && (task->Id == taskId))
			return lvItem;
	}

	// else
	return nullptr;
}

String^ TaskListView::Translate(String^ text, Translator::Type type)
{
	if (m_Trans == nullptr)
		return text;

	// else
	return m_Trans->Translate(text, type);
}

Windows::Forms::Control^ TaskListView::GetOwner()
{
	return this;
}

LabelTipInfo^ TaskListView::ToolHitTest(Drawing::Point ptScreen)
{
	auto pt = PointToClient(ptScreen);
	auto lvHit = HitTest(pt);

	if ((lvHit == nullptr) || (lvHit->Item == nullptr))
		return nullptr;

	// Only interested in first (label) column
	auto labelRect = CalcLabelRect(lvHit->Item, TaskListView::LabelExtents::TitleColumn);

	if (!labelRect.Contains(pt))
		return nullptr;

	auto task = ASTYPE(lvHit->Item->Tag, ITaskBase);

	if (task == nullptr)
		return nullptr;

	// Check if there's enough room already
	if (m_LabelTip->CalcTipHeight(task->Title, Font, labelRect.Width) <= labelRect.Height)
		return nullptr;

	// HACK /////////////////////////////////
	labelRect.Offset(2, 2);
	/////////////////////////////////////////

	auto tip = gcnew LabelTipInfo();

	tip->Id = task->Id;
	tip->Text = task->Title;
	tip->MultiLine = false;
	tip->Rect = labelRect;
	tip->Font = (ITaskBaseExt::IsTopLevel(task) ? m_BoldFont : Font);

	return tip;
}

void TaskListView::OnGotFocus(EventArgs^ e)
{
	ListView::OnGotFocus(e);

	Invalidate();
	//GotFocus(this, e);
}

void TaskListView::OnLostFocus(EventArgs^ e)
{
	ListView::OnLostFocus(e);

	Invalidate();
}

void TaskListView::OnSizeChanged(EventArgs^ e)
{
	ListView::OnSizeChanged(e);

	if (m_SizeTaskColumnToFit)
		ResizeTaskColumnToFit();

	Invalidate();
}

void TaskListView::OnFontChanged(EventArgs^ e)
{
	ListView::OnFontChanged(e);

	m_BoldFont = gcnew Drawing::Font(Font, FontStyle::Bold);
	Invalidate();
}

Drawing::Rectangle TaskListView::CalcLabelRect(ListViewItem^ item, TaskListView::LabelExtents extents)
{
	Drawing::Rectangle textRect = item->Bounds;

	textRect.X += 2;
	textRect.X += CheckboxOffset;
	textRect.X += TextIconOffset;

	switch (extents)
	{
	case TaskListView::LabelExtents::TitleTextOnly:
		{
			auto graphics = Graphics::FromHwnd(Handle);
			textRect.Width = (int)graphics->MeasureString(item->Text, GetFont(ASTYPE(item->Tag, ITaskBase), true)).Width;
		}
		break;

	case TaskListView::LabelExtents::TitleColumn:
		textRect.Width = (Columns[0]->Width - textRect.X);
		break;

	case TaskListView::LabelExtents::AllColumns:
		{
			textRect.Width = (Columns[0]->Width - textRect.X);

			for (int i = 1; i < Columns->Count; i++)
				textRect.Width += Columns[i]->Width;
		}
		break;
	}

	return textRect;
}

Drawing::Rectangle TaskListView::CalcCheckboxRect(Drawing::Rectangle labelRect)
{
	if (!ShowCompletionCheckboxes)
		return Rectangle::Empty;

	int top = ((labelRect.Top + labelRect.Bottom - m_CheckBoxSize) / 2) + (labelRect.Height % 2);

	return Drawing::Rectangle(labelRect.X + LabelPadding, top, m_CheckBoxSize, m_CheckBoxSize);
}

Drawing::Rectangle TaskListView::CalcIconRect(Drawing::Rectangle labelRect)
{
	if (!m_ItemsHaveIcons)
		return Drawing::Rectangle::Empty;

	int top = ((labelRect.Top + labelRect.Bottom - ImageSize) / 2) + (labelRect.Height % 2);

	return Drawing::Rectangle(labelRect.X + CheckboxOffset, top, ImageSize, ImageSize);
}

bool TaskListView::TaskColorIsBackground::get()
{ 
	return m_TaskColorIsBkgnd;
}

void TaskListView::TaskColorIsBackground::set(bool value)
{
	if (m_TaskColorIsBkgnd != value)
	{
		m_TaskColorIsBkgnd = value;
		Invalidate();
	}
}

bool TaskListView::ShowParentsAsFolders::get()
{
	return m_ShowParentAsFolder;
}

void TaskListView::ShowParentsAsFolders::set(bool value)
{
	if (m_ShowParentAsFolder != value)
	{
		m_ShowParentAsFolder = value;
		Invalidate();
	}
}

bool TaskListView::ShowCompletionCheckboxes::get()
{
	return m_ShowCompletionCheckboxes;
}

void TaskListView::ShowCompletionCheckboxes::set(bool value)
{
	if (m_ShowCompletionCheckboxes != value)
	{
		m_ShowCompletionCheckboxes = value;
		Invalidate();
	}
}

bool TaskListView::ReadOnly::get()
{
	return m_ReadOnly;
}

void TaskListView::ReadOnly::set(bool value)
{
	if (m_ReadOnly != value)
	{
		m_ReadOnly = value;
		AllowDrop = !m_ReadOnly;
	}
}

bool TaskListView::ShowLabelTips::get()
{
	return ((m_LabelTip != nullptr) ? m_LabelTip->Active : false);
}

void TaskListView::ShowLabelTips::set(bool value)
{
	if (m_LabelTip != nullptr)
		m_LabelTip->Active = value;
}

bool TaskListView::ItemsHaveIcons::get()
{
	return m_ItemsHaveIcons;
}

void TaskListView::ItemsHaveIcons::set(bool value)
{
	m_ItemsHaveIcons = value;
}

int TaskListView::ImageSize::get()
{
	return IMAGE_SIZE;
}

int TaskListView::TextIconOffset::get() 
{ 
	return (ItemsHaveIcons ? (ImageSize/* + LabelPadding*/) : 0);
}

int TaskListView::CheckboxOffset::get() 
{ 
	return (ShowCompletionCheckboxes ? (ImageSize + LabelPadding) : 0);
}

Color TaskListView::GridlineColor::get()
{ 
	return m_GridlineColor;
}

void TaskListView::GridlineColor::set(Color value)
{
	if (m_GridlineColor != value)
	{
		m_GridlineColor = value;
		Invalidate();
	}
}

Color TaskListView::AlternateLineColor::get() 
{ 
	return m_AlternateLineColor;
}

void TaskListView::AlternateLineColor::set(Color value)
{
	if (m_AlternateLineColor != value)
	{
		m_AlternateLineColor = value;
		Invalidate();
	}
}

void TaskListView::OnDrawItem(DrawListViewItemEventArgs^ e)
{
	e->DrawDefault = false;

	if (e->Item == nullptr)
		return;

	// Background color full width
	auto task = ASTYPE(e->Item->Tag, ITaskBase);

	auto backColor = GetBackColor(task, e->Item->Index);
	auto itemRect = e->Item->Bounds;

	e->Graphics->FillRectangle(gcnew SolidBrush(backColor), 0, itemRect.Top + 1, Width, itemRect.Height);

	// Horizontal grid line full width
	Pen^ gridPen = nullptr;

	if (!m_GridlineColor.IsEmpty)
	{
		gridPen = gcnew Pen(m_GridlineColor);
		e->Graphics->DrawLine(gridPen, 0, itemRect.Bottom, Width, itemRect.Bottom);
	}

	// Selection highlight
	bool selected = IsItemSelected(e->Item);

	if (selected)
	{
		Drawing::Rectangle labelRect = CalcLabelRect(e->Item, TaskListView::LabelExtents::AllColumns);

		UIExtension::SelectionRect::Draw(Handle,
										 e->Graphics,
										 labelRect.X,
										 labelRect.Y,
										 labelRect.Width,
										 labelRect.Height,
										 true); // transparent
	}

	// Finally the column values
	auto textColor = GetTextColor(task, selected);
	auto textBrush = gcnew SolidBrush(textColor);

	auto subItemRect = itemRect;

	for (int colIndex = 0; colIndex < e->Item->SubItems->Count; colIndex++)
	{
		auto subItem = e->Item->SubItems[colIndex];
		auto horzAlign = StringAlignment::Near;

		subItemRect.Width = Columns[colIndex]->Width;

		auto textRect = Rectangle::Inflate(subItemRect, -LabelPadding, -1);
		auto flags = (TextFormatFlags::SingleLine | TextFormatFlags::Bottom | TextFormatFlags::Left);

		if (colIndex == 0)
		{
			flags = (flags | TextFormatFlags::EndEllipsis);

			if (ShowCompletionCheckboxes)
			{
				if (m_CheckBoxSize == -1)
					m_CheckBoxSize = CheckBoxRenderer::GetGlyphSize(e->Graphics, CheckBoxState::UncheckedNormal).Width;

				auto checkRect = CalcCheckboxRect(itemRect);
				CheckBoxRenderer::DrawCheckBox(e->Graphics, checkRect.Location, GetTaskCheckboxState(task));

				textRect.X += CheckboxOffset;
				textRect.Width -= CheckboxOffset;
			}

			if (ItemsHaveIcons)
			{
				if (task->HasIcon && m_TaskIcons->Get(task->Id))
				{
					auto iconRect = CalcIconRect(itemRect);
					m_TaskIcons->Draw(e->Graphics, iconRect.Left, iconRect.Top);
				}

				textRect.X += TextIconOffset;
				textRect.Width -= TextIconOffset;
			}
		}
		else // numbers
		{
			flags = (flags | TextFormatFlags::Right);
		}

		TextRenderer::DrawText(e->Graphics, 
							   subItem->Text, 
							   GetFont(task, (colIndex == 0)),
							   textRect, 
							   textColor, 
							   flags);

		// Vertical gridline
		if (gridPen != nullptr)
			e->Graphics->DrawLine(gridPen, subItemRect.Right - 1, itemRect.Top, subItemRect.Right - 1, itemRect.Bottom);

		// next subitem
		subItemRect.X = subItemRect.Right;
	}
}

Drawing::Font^ TaskListView::GetFont(ITaskBase^ task, bool title)
{
	if (title && ITaskBaseExt::IsTopLevel(task))
		return m_BoldFont;

	return Font;
}

Drawing::Color TaskListView::GetTextColor(ITaskBase^ task, bool selected)
{
	if (selected)
	{
		if (SystemInformation::HighContrast)
			return SystemColors::HighlightText;

		// else
		return UIExtension::SelectionRect::GetTextColor(UIExtension::SelectionRect::Style::Selected, task->TextColor);
	}

	if (!task->TextColor.IsEmpty)
		return (TaskColorIsBackground ? DrawingColor::GetBestTextColor(task->TextColor, true) : task->TextColor);

	// else
	return SystemColors::WindowText;
}

Drawing::Color TaskListView::GetBackColor(ITaskBase^ task, int row)
{
	if (!task->TextColor.IsEmpty && TaskColorIsBackground)
		return task->TextColor;

	if (((row % 2) == 0) && !m_AlternateLineColor.IsEmpty)
		return m_AlternateLineColor;

	return SystemColors::Window;
}

void TaskListView::WndProc(Message% m)
{
	if (m_LabelTip != nullptr)
		m_LabelTip->ProcessMessage(m);

	switch (m.Msg)
	{
	case WM_LBUTTONDOWN:
		{
			Point pos = Win32::GetPoint(m.LParam);
			auto lvHit = HitTest(pos)->Item;

			if (lvHit == nullptr)
			{
				// Check for bounds selection
				if (MultiSelect && Win32::DragDetect(Handle, PointToScreen(pos)))
				{
					// Start a timer to detect the end of bound selecting
					m_BoundSelectionTimer = gcnew Timer();

					m_BoundSelectionTimer->Interval = 100;
					m_BoundSelectionTimer->Tick += gcnew EventHandler(this, &TaskListView::OnBoundSelectionTimer);
					m_BoundSelectionTimer->Enabled = true;
					m_BoundSelectionTimer->Start();

					// then default handling
					break;
				}

				// else
				Focus();
				return;
			}

			auto task = ASTYPE(lvHit->Tag, ITaskBase);

			if (IsTaskEditable(task))
			{
				if (CalcCheckboxRect(lvHit->Bounds).Contains(pos))
				{
					if (!lvHit->Selected)
						ListView::WndProc(m); // Default handling to select task

					EditTaskDone(this, task);
					return;
				}
				else if (CalcIconRect(lvHit->Bounds).Contains(pos))
				{
					if (!lvHit->Selected)
						ListView::WndProc(m); // Default handling to select task

					EditTaskIcon(this, task);
					return;
				}
				// If the item is selected but we're not focused, 
				// prevent the base class from starting a label edit
				else if (lvHit->Selected && !Focused)
				{
					Focus();
					return;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			Point pos = Win32::GetPoint(m.LParam);
			auto lvHit = HitTest(pos)->Item;

			if (lvHit == nullptr)
				break;

			Debug::Assert(lvHit->Selected);

			auto task = ASTYPE(lvHit->Tag, ITaskBase);

			if (IsTaskEditable(task) && GetTaskLabelRect(task->Id).Contains(pos))
				EditTaskLabel(this, task);

		}
		return; // always

	case WM_LBUTTONUP:
		{
			Point pos = Win32::GetPoint(m.LParam);

			if (HitTest(pos)->Item == nullptr)
				return;
		}
		break;
	}

	// else default handling
	ListView::WndProc(m);
}

void TaskListView::OnBoundSelectionTimer(Object^ sender, EventArgs^ e)
{
	if (MouseButtons == Windows::Forms::MouseButtons::None)
	{
		m_BoundSelectionTimer->Stop();
		m_BoundSelectionTimer->Enabled = false;
		m_BoundSelectionTimer = nullptr;

		BoundSelectionEnded(this, gcnew EventArgs());
	}
}

void TaskListView::OnMouseMove(MouseEventArgs^ e)
{
	ListView::OnMouseMove(e);

	auto lvHit = HitTest(e->Location)->Item;
	Windows::Forms::Cursor^ cursor = nullptr;

	if (lvHit != nullptr)
	{
		auto task = ASTYPE(lvHit->Tag, ITaskBase);

		if (task != nullptr)
		{
			if (task->IsLocked)
			{
				cursor = UIExtension::AppCursor(UIExtension::AppCursorType::LockedTask);
			}
			else if (!m_ReadOnly)
			{
				if (CalcIconRect(lvHit->Bounds).Contains(e->Location))
				{
					cursor = UIExtension::HandCursor();
				}
				else if (CalcLabelRect(lvHit, LabelExtents::AllColumns).Contains(e->Location) &&
						 !IsTaskDraggable(this, task))
				{
					cursor = UIExtension::AppCursor(UIExtension::AppCursorType::NoDrag);
				}
			}
		}
	}

	// all else
	if (cursor == nullptr)
		cursor = Cursors::Arrow;

	Cursor = cursor;
}

void TaskListView::OnItemDrag(ItemDragEventArgs^ e)
{
	auto item = ASTYPE(e->Item, ListViewItem);
	auto task = ASTYPE(item->Tag, ITaskBase);

	if (task == nullptr)
		return;

	if (!item->Selected)
		SelectTask(task->Id);

	Focus();

	if (task->IsLocked)
		return;

	if (!IsTaskDraggable(this, task))
		 return;
	
	ListView::OnItemDrag(e);
}

void TaskListView::OnBeforeLabelEdit(LabelEditEventArgs^ e)
{
	if (m_ReadOnly)
		return;

	// Mouse cursor must still be over the text label
	auto mousePos = PointToClient(MousePosition);

	if (CalcLabelRect(Items[e->Item], TaskListView::LabelExtents::TitleTextOnly).Contains(mousePos))
	{
		auto task = ASTYPE(Items[e->Item]->Tag, ITaskBase);

		if (task != nullptr)
			EditTaskLabel(this, task);
	}

	// always
	e->CancelEdit = true;
	return;
}

bool TaskListView::SelectTaskEx(String^ words, UIExtension::SelectTask selectTask, 
							    bool caseSensitive, bool wholeWord, bool findReplace)
{
	if (Items->Count == 0)
		return false;

	int matchIndex = -1;

	switch (selectTask)
	{
	case UIExtension::SelectTask::SelectFirstTask:
		{
			matchIndex = FindTask(words, 0, true, caseSensitive, wholeWord, findReplace);
		}
		break;

	case UIExtension::SelectTask::SelectNextTask:
		{
			matchIndex = FindTask(words, (LastSelectedIndex + 1), true, caseSensitive, wholeWord, findReplace);
		}
		break;

	case UIExtension::SelectTask::SelectNextTaskInclCurrent:
		{
			int selIndex = LastSelectedIndex;

			if (selIndex == -1)
				selIndex = 0;

			matchIndex = FindTask(words, selIndex, true, caseSensitive, wholeWord, findReplace);
		}
		break;

	case UIExtension::SelectTask::SelectPrevTask:
		{
			int selIndex = FirstSelectedIndex;

			if (selIndex == -1)
				selIndex = Items->Count;

			matchIndex = FindTask(words, (selIndex - 1), false, caseSensitive, wholeWord, findReplace);
		}
		break;

	case UIExtension::SelectTask::SelectLastTask:
		{
			matchIndex = FindTask(words, LastIndex, false, caseSensitive, wholeWord, findReplace);
		}
		break;
	}

	if (matchIndex != -1)
	{
		SelectedIndices->Clear();
		SelectedIndices->Add(matchIndex);

		EnsureSelectionVisible();
		return true;
	}

	return false;
}

int TaskListView::FirstSelectedIndex::get()
{
	return (HasSelection ? SelectedIndices[0] : -1);
}

int TaskListView::LastSelectedIndex::get()
{
	return (HasSelection ? SelectedIndices[SelectionCount - 1] : -1);
}

int TaskListView::LastIndex::get()
{
	return ((Items->Count > 0) ? (Items->Count - 1) : -1);
}

int TaskListView::FindTask(String^ phrase, int startIndex, bool forward, bool caseSensitive, bool wholeWord, bool findReplace)
{
	int fromIndex = startIndex;
	int toIndex = (forward ? Items->Count : -1); // intentianally just beyond/before the item range
	int increment = (forward ? 1 : -1);

	for (int i = fromIndex; i != toIndex; i += increment)
	{
		if (TaskMatches(GetTask(i), phrase, caseSensitive, wholeWord, findReplace))
			return i;
	}

	return -1; // no match
}

bool TaskListView::TaskMatches(ITaskBase^ task, String^ phrase, bool caseSensitive, bool wholeWord, bool /*findReplace*/)
{
	return ((task != nullptr) && StringUtil::Find(task->Title, phrase, caseSensitive, wholeWord));
}

CheckBoxState TaskListView::GetTaskCheckboxState(ITaskBase^ task)
{
	return (task->IsDone ? CheckBoxState::CheckedNormal : CheckBoxState::UncheckedNormal);
}

void TaskListView::ResizeTaskColumnToFit()
{
	// Resize first column to fill remaining width
	int otherColsWidth = 0;

	for (int i = 1; i < Columns->Count; i++)
		otherColsWidth += Columns[i]->Width;

	int taskColWidth = (ClientRectangle.Width - otherColsWidth - 2);
	taskColWidth = Math::Max(MinTaskColumnWidth, taskColWidth);
	taskColWidth = Math::Max(0, taskColWidth);

	Columns[0]->Width = taskColWidth;
}
