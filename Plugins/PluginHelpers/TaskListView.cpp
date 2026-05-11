
#include "stdafx.h"
#include "TaskListView.h"
#include "Win32.h"
#include "DPIScaling.h"
#include "ColorUtil.h"

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

TaskListView::NoTrackHeaderControl::NoTrackHeaderControl(TaskListView^ lv)
{
	AssignHandle(lv->GetHeaderHandle());
}

void TaskListView::NoTrackHeaderControl::WndProc(Message% m)
{
	switch (m.Msg)
	{
	case WM_SETCURSOR:
		Win32::SetArrowCursor();
		return;
	}

	NativeWindow::WndProc(m);
}

////////////////////////////////////////////////////////////////////////////////////////////////

const int IMAGE_SIZE = DPIScaling::Scale(16);

////////////////////////////////////////////////////////////////////////////////////////////////

TaskListView::TaskListView()
	:
	ListView(),
	m_TaskIcons(nullptr),
	m_LabelTip(nullptr),
	m_BoldFont(nullptr),
	m_ItemsHaveIcons(false),
	m_ShowParentAsFolder(false),
	m_TaskColorIsBkgnd(false),
	m_ShowCompletionCheckboxes(false),
	m_GridlineColor(Color::Empty),
	m_AlternateLineColor(Color::Empty),
	m_CheckBoxSize(-1)
{
	m_LabelTip = gcnew LabelTip(this);
}

void TaskListView::Initialize(Translator^ trans, UIExtension::TaskIcon^ taskIcons)
{
	m_Trans = trans;
	m_TaskIcons = taskIcons;

	View = System::Windows::Forms::View::Details;
	MultiSelect = false;
	FullRowSelect = true;
	HideSelection = false;
	OwnerDraw = true;
	Sorting = SortOrder::Ascending;
	HeaderStyle = ColumnHeaderStyle::Clickable;
	DoubleBuffered = true;
	HotTracking = false;
	HoverSelection = false;
	LabelEdit = true;
	GridLines = false;
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

IntPtr TaskListView::GetHeaderHandle()
{
	return IntPtr(Win32::SendMessage(Handle, LVM_GETHEADER, UIntPtr::Zero, IntPtr::Zero));
}

bool TaskListView::HasTaskId(UInt32 taskId)
{
	return (FindLVItem(taskId) != nullptr);
}

UInt32 TaskListView::GetNextTaskId(UInt32 taskId, bool next)
{
	auto lvItem = FindLVItem(taskId);

	if (lvItem == nullptr)
		return 0;

	int nextIndex = (next ? (lvItem->Index + 1) : (lvItem->Index - 1));
	return GetTaskId(nextIndex);
}

ITaskBase^ TaskListView::GetTask(int index)
{
	if ((index < 0) || (index >= Items->Count))
		return nullptr;

	return ASTYPE(Items[index]->Tag, ITaskBase);
}

UInt32 TaskListView::GetTaskId(int index)
{
	auto task = GetTask(index);
	return ((task == nullptr) ? 0 : task->Id);
}

UInt32 TaskListView::SelectedTaskId::get()
{
	auto selTask = SelectedTask;
	return (selTask == nullptr ? 0 : selTask->Id);
}

UInt32 TaskListView::SelectTask(UInt32 taskId)
{
	SelectedItems->Clear();
	SelectedIndices->Clear();

	if (Items->Count == 0)
		return 0;

	ListViewItem^ selLVItem = FindLVItem(taskId);

	if (selLVItem == nullptr)
		selLVItem = Items[0];

	selLVItem->Selected = true;
	EnsureSelectionVisible();

	return ASTYPE(selLVItem->Tag, ITaskBase)->Id;
}

void TaskListView::EnsureSelectionVisible()
{
	if ((SelectedIndices->Count == 0) || (Items->Count == 0))
		return;

	int itemIndex = SelectedItems->Count;

	while (itemIndex-- > 0)
	{
		Drawing::Rectangle itemRect = SelectedItems[itemIndex]->Bounds;

		if (Drawing::Rectangle::Intersect(ClientRectangle, itemRect) == itemRect)
			return;
	}

	// else
	EnsureVisible(SelectedIndices[0]);
}

String^ TaskListView::SelectedTaskTitle::get()
{
	auto selTask = SelectedTask;
	return (selTask == nullptr ? String::Empty : selTask->Title);
}

ITaskBase^ TaskListView::SelectedTask::get()
{
	if (SelectedItems->Count == 0)
		return nullptr;

	return ASTYPE(SelectedItems[0]->Tag, ITaskBase);
}

Drawing::Rectangle TaskListView::SelectedTaskLabelRect::get()
{
	auto labelRect = Drawing::Rectangle::Empty;

	if (SelectedItems->Count > 0)
		labelRect = CalcLabelTextRect(SelectedItems[0]->GetBounds(ItemBoundsPortion::Label), false);

	return labelRect;
}

ListViewItem^ TaskListView::FindLVItem(UInt32 taskId)
{
	for each(ListViewItem^ lvItem in Items)
	{
		auto item = ASTYPE(lvItem->Tag, ITaskBase);

		if ((item != nullptr) && (item->Id == taskId))
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
	auto hit = HitTest(pt);

	if ((hit == nullptr) || (hit->Item == nullptr))
		return nullptr;

	// Only interested in first (label) column
	auto labelRect = CalcLabelTextRect(hit->Item->GetBounds(ItemBoundsPortion::Entire), false);

	if (!labelRect.Contains(pt))
		return nullptr;

	auto item = ASTYPE(hit->Item->Tag, ITaskBase);

	if (item == nullptr)
		return nullptr;

	// Check if there's enough room already
	if (m_LabelTip->CalcTipHeight(item->Title, Font, labelRect.Width) <= labelRect.Height)
		return nullptr;

	labelRect.Offset(-1, -1);

	auto tip = gcnew LabelTipInfo();

	tip->Id = item->Id;
	tip->Text = item->Title;
	tip->MultiLine = false;
	tip->Rect = labelRect;
	tip->Font = Font;

	return tip;
}

void TaskListView::OnGotFocus(EventArgs^ e)
{
	ListView::OnGotFocus(e);

	Invalidate();
}

void TaskListView::OnLostFocus(EventArgs^ e)
{
	ListView::OnLostFocus(e);

	Invalidate();
}

void TaskListView::OnSizeChanged(EventArgs^ e)
{
	ListView::OnSizeChanged(e);

	Invalidate();
}

void TaskListView::OnFontChanged(EventArgs^ e)
{
	ListView::OnFontChanged(e);

	m_BoldFont = gcnew Drawing::Font(Font, FontStyle::Bold);
	Invalidate();
}

Drawing::Rectangle TaskListView::CalcLabelTextRect(Drawing::Rectangle labelRect, bool includeIdSubItems)
{
	Drawing::Rectangle textRect = labelRect;

	textRect.X = 2;
	textRect.X += CheckboxOffset;
	textRect.X += TextIconOffset;

	textRect.Width = (Columns[0]->Width - textRect.X);

	if (includeIdSubItems)
	{
		for (int i = 1; i < Columns->Count; i++)
			textRect.Width += Columns[i]->Width;
	}

	return textRect;
}

Drawing::Rectangle TaskListView::CalcCheckboxRect(Drawing::Rectangle labelRect)
{
	if (!ShowCompletionCheckboxes)
		return Rectangle::Empty;

	int top = ((labelRect.Top + labelRect.Bottom - m_CheckBoxSize) / 2) + (labelRect.Height % 2);

	return Drawing::Rectangle(labelRect.X, top, m_CheckBoxSize, m_CheckBoxSize);
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
	return (ItemsHaveIcons ? (ImageSize + 2) : 0);
}

int TaskListView::CheckboxOffset::get() 
{ 
	return (ShowCompletionCheckboxes ? ImageSize : 0);
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

void TaskListView::OnMeasureItem(Windows::Forms::MeasureItemEventArgs^ e)
{
	e->ItemHeight = Math::Max(17, Font->Height); // minimum height;
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
	if (e->Item->Selected)
	{
		Drawing::Rectangle labelRect = CalcLabelTextRect(itemRect, true);

		UIExtension::SelectionRect::Draw(Handle,
										 e->Graphics,
										 labelRect.X,
										 labelRect.Y,
										 labelRect.Width,
										 labelRect.Height,
										 true); // transparent
	}

	// Finally the column values
	auto textColor = GetTextColor(task, e->Item->Selected);
	auto textBrush = gcnew SolidBrush(textColor);

	auto subItemRect = itemRect;

	for (int colIndex = 0; colIndex < e->Item->SubItems->Count; colIndex++)
	{
		auto subItem = e->Item->SubItems[colIndex];
		auto horzAlign = StringAlignment::Near;

		subItemRect.Width = Columns[colIndex]->Width;

		auto textFont = Font;
		auto textRect = Rectangle::Inflate(subItemRect, -2, -1);
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
				if ((e->Item->ImageIndex != -1) && m_TaskIcons->Get(task->Id))
				{
					auto iconRect = CalcIconRect(itemRect);
					m_TaskIcons->Draw(e->Graphics, iconRect.Left, iconRect.Top);
				}

				textRect.X += TextIconOffset;
				textRect.Width -= TextIconOffset;
			}

			if (task->Position->IndexOf('.') == -1)
				textFont = m_BoldFont;
		}
		else // numbers
		{
			flags = (flags | TextFormatFlags::Right);
		}

		TextRenderer::DrawText(e->Graphics, 
							   subItem->Text, 
							   textFont, 
							   textRect, 
							   textColor, 
							   flags);

		// Vertical gridline
		if (gridPen != nullptr)
			e->Graphics->DrawLine(gridPen, subItemRect.Right, itemRect.Top, subItemRect.Right, itemRect.Bottom);

		// next subitem
		subItemRect.X = subItemRect.Right;
	}
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

void TaskListView::OnDrawColumnHeader(DrawListViewColumnHeaderEventArgs^ e)
{
	e->DrawDefault = true;
}

void TaskListView::WndProc(Message% m)
{
	if (m_LabelTip != nullptr)
		m_LabelTip->ProcessMessage(m);

	switch (m.Msg)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
		{
			Point pos = Point(Win32::LoWord(m.LParam), Win32::HiWord(m.LParam));

			switch (HitTest(pos)->Location)
			{
			case ListViewHitTestLocations::AboveClientArea:
			case ListViewHitTestLocations::BelowClientArea:
			case ListViewHitTestLocations::LeftOfClientArea:
			case ListViewHitTestLocations::RightOfClientArea:
			case ListViewHitTestLocations::None:
				Focus();
				return; // eat
			}
		}
		break;
	}

	// else default handling
	ListView::WndProc(m);
}

void TaskListView::OnMouseDown(MouseEventArgs^ e)
{
	// disable label editing if not on the item text
	int leftMargin = (CheckboxOffset + TextIconOffset);
	int rightMargin = Columns[0]->Width;

	LabelEdit = ((e->Location.X > leftMargin) && (e->Location.X < rightMargin));

	ListView::OnMouseDown(e);
}

void TaskListView::HandleMouseClick(MouseEventArgs^ e, bool doubleClick)
{
	if (e->Button != Windows::Forms::MouseButtons::Left)
		return;

	if (!ItemsHaveIcons && !ShowCompletionCheckboxes && !doubleClick)
		return;

	auto hit = HitTest(e->Location);

	if (hit->Item == nullptr)
		return;

	auto task = ASTYPE(hit->Item->Tag, ITaskBase);

	if ((task == nullptr) || task->IsLocked)
		return;

	if (CalcCheckboxRect(hit->Item->Bounds).Contains(e->Location))
	{
		EditTaskDone(this, task->Id, !task->IsDone);
	}
	else if (CalcIconRect(hit->Item->Bounds).Contains(e->Location))
	{
		EditTaskIcon(this, task->Id);
	}
	else if (doubleClick)
	{
		EditTaskLabel(this, task->Id);
	}
}

void TaskListView::OnMouseClick(MouseEventArgs^ e)
{
	ListView::OnMouseClick(e);

	HandleMouseClick(e, false);
}

void TaskListView::OnMouseDoubleClick(MouseEventArgs^ e)
{
	ListView::OnMouseDoubleClick(e);

	HandleMouseClick(e, true);
}

void TaskListView::OnMouseMove(MouseEventArgs^ e)
{
	ListView::OnMouseMove(e);

	auto hit = HitTest(e->Location);

	if (hit->Item != nullptr)
	{
		auto item = ASTYPE(hit->Item->Tag, ITaskBase);

		if (item != nullptr)
		{
			if (item->IsLocked)
			{
				Cursor = UIExtension::AppCursor(UIExtension::AppCursorType::LockedTask);
				return;
			}
			
			if (CalcIconRect(hit->Item->Bounds).Contains(e->Location))
			{
				Cursor = UIExtension::HandCursor();
				return;
			}
		}
	}

	// all else
	Cursor = Cursors::Arrow;
}

void TaskListView::OnBeforeLabelEdit(LabelEditEventArgs^ e)
{
	if (e->Item != -1)
	{
		auto item = ASTYPE(Items[e->Item]->Tag, ITaskBase);

		if (item != nullptr)
		{
			EditTaskLabel(this, item->Id);
			e->CancelEdit = true;

			return;
		}
	}

	ListView::OnBeforeLabelEdit(e);
}

// The other part of making NoTrackHeaderControl work
void TaskListView::OnColumnWidthChanging(ColumnWidthChangingEventArgs^ e)
{
	NativeWindow^ header = NativeWindow::FromHandle(GetHeaderHandle());

	if (ISTYPE(header, NoTrackHeaderControl))
	{
		e->Cancel = true;
		e->NewWidth = Columns[e->ColumnIndex]->Width;
	}
}

bool TaskListView::SelectTaskEx(String^ words, UIExtension::SelectTask selectTask, 
							    bool caseSensitive, bool wholeWord, bool findReplace)
{
	if (Items->Count == 0)
		return false;

	if (SelectedIndices->Count == 0)
		SelectedIndices->Add(0);

	int selIndex = SelectedIndices[0];
	int matchIndex = -1;

	switch (selectTask)
	{
	case UIExtension::SelectTask::SelectFirstTask:
		matchIndex = FindTask(words, 0, true, caseSensitive, wholeWord, findReplace);
		break;

	case UIExtension::SelectTask::SelectNextTask:
		matchIndex = FindTask(words, (selIndex + 1), true, caseSensitive, wholeWord, findReplace);
		break;

	case UIExtension::SelectTask::SelectNextTaskInclCurrent:
		matchIndex = FindTask(words, selIndex, true, caseSensitive, wholeWord, findReplace);
		break;

	case UIExtension::SelectTask::SelectPrevTask:
		matchIndex = FindTask(words, (selIndex - 1), false, caseSensitive, wholeWord, findReplace);
		break;

	case UIExtension::SelectTask::SelectLastTask:
		matchIndex = FindTask(words, (Items->Count - 1), false, caseSensitive, wholeWord, findReplace);
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

int TaskListView::FindTask(String^ phrase, int startIndex, bool forward, bool caseSensitive, bool wholeWord, bool findReplace)
{
	int fromIndex = startIndex;
	int toIndex = (forward ? Items->Count : -1);
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
