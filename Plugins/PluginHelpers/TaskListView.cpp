
#include "stdafx.h"
#include "TaskListView.h"
#include "Win32.h"
#include "DPIScaling.h"

#include <shared\Clipboard.h>
#include <shared\Misc.h>
#include <shared\GraphicsMisc.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Diagnostics;
using namespace System::Drawing;
using namespace System::Linq;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;

using namespace Abstractspoon::Tdl::PluginHelpers;

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
	m_ilItemHeight(nullptr),
	m_BoldFont(nullptr),
	m_ItemsHaveIcons(false),
	m_ShowParentAsFolder(false),
	m_TaskColorIsBkgnd(false),
	m_ShowCompletionCheckboxes(false),
	m_ShowMixedCompletionState(false)
{
	m_LabelTip = gcnew LabelTip(this);

	m_ilItemHeight = gcnew ImageList();
	m_ilItemHeight->ImageSize = Drawing::Size(1, DPIScaling::Scale(17)); // minimum height

	View = System::Windows::Forms::View::Details;
	MultiSelect = false;
	FullRowSelect = true;
	HideSelection = false;
	OwnerDraw = true;
	Sorting = SortOrder::Ascending;
	HeaderStyle = ColumnHeaderStyle::Clickable;
	DoubleBuffered = true;
	HotTracking = false;
	LabelEdit = true;
	StateImageList = m_ilItemHeight;
}

void TaskListView::Initialize(Translator^ trans, UIExtension::TaskIcon^ taskIcons)
{
	m_Trans = trans;
	m_TaskIcons = taskIcons;
}

IntPtr TaskListView::GetHeaderHandle()
{
	return IntPtr(Win32::SendMessage(Handle, LVM_GETHEADER, UIntPtr::Zero, IntPtr::Zero));
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

Drawing::Rectangle TaskListView::CalcLabelTextRect(Drawing::Rectangle labelRect, bool includeIdColumn)
{
	Drawing::Rectangle textRect = labelRect;

	textRect.X = 2;
	textRect.X += CheckboxOffset;
	textRect.X += TextIconOffset;

	textRect.Width = (Columns[0]->Width - textRect.X);

	if (includeIdColumn)
		textRect.Width += Columns[1]->Width;

	return textRect;
}

Drawing::Rectangle TaskListView::CalcCheckboxRect(Drawing::Rectangle labelRect)
{
	if (!ShowCompletionCheckboxes)
		return Rectangle::Empty;

	int top = (((labelRect.Top + labelRect.Bottom) / 2) - (ImageSize / 2));

	return Drawing::Rectangle(labelRect.X, top, ImageSize, ImageSize);
}

Drawing::Rectangle TaskListView::CalcIconRect(Drawing::Rectangle labelRect)
{
	if (!m_ItemsHaveIcons)
		return Drawing::Rectangle::Empty;

	if (m_ShowCompletionCheckboxes)
		labelRect.X += CheckboxOffset;

	int imageSize = ImageSize;
	int top = (((labelRect.Top + labelRect.Bottom) / 2) - (imageSize / 2));

	return Drawing::Rectangle(labelRect.X, top, imageSize, imageSize);
}

UInt32 TaskListView::SelectedTaskId::get()
{
// 	auto selItem = ASTYPE(SelectedItem, ITaskBase);
// 
// 	return ((selItem == nullptr) ? 0 : selItem->Id);
	return 0L;
}

String^ TaskListView::SelectedTaskTitle::get()
{
// 	if (SelectedTaskId::get() == 0)
// 		return String::Empty;
// 
// 	return ASTYPE(SelectedItem, ITaskBase)->Title;
	return String::Empty;
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

bool TaskListView::ShowMixedCompletionState::get()
{
	return m_ShowMixedCompletionState;
}

void TaskListView::ShowMixedCompletionState::set(bool value)
{
	if (m_ShowMixedCompletionState != value)
	{
		m_ShowMixedCompletionState = value;
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

// void TaskListView::OnMeasureItem(MeasureItemEventArgs^ e)
// {
// 	ComboBox::OnMeasureItem(e);
// 
// 	e->ItemHeight = UIExtension::TaskIcon::IconSize;
// }
// 
// void TaskListView::OnDrawItem(DrawItemEventArgs^ e)
// {
// 	ListView::OnDrawItem(e);
// 
// 	if (e->Index < 0)
// 		return;
// 
// 	auto taskItem = ASTYPE(Items[e->Index], WrappedITask);
// 
// 	if (taskItem != nullptr)
// 	{
// 		auto rect = e->Bounds;
// 
// 		bool hasIcon = (taskItem->HasIcon && m_TaskIcons->Get(taskItem->Id));
// 		bool listItem = !(e->State.HasFlag(DrawItemState::ComboBoxEdit));
// 
// 		if (listItem)
// 		{
// 			rect.X += GetListItemTextOffset(Items[e->Index]);
// 
// 			// Icon needs to be drawn BEFORE text
// 			if (hasIcon)
// 				rect.X -= UIExtension::TaskIcon::IconSize;
// 		}
// 
// 		if (hasIcon)
// 		{
// 			m_TaskIcons->Draw(e->Graphics, rect.X, rect.Y);
// 			rect.X += UIExtension::TaskIcon::IconSize;
// 		}
// 
// 		auto brush = TextBrush(e);
// 		auto font = ((!taskItem->IsNone && taskItem->IsTopLevel) ? m_BoldFont : Font);
// 
// 		e->Graphics->DrawString(taskItem->Title, font, brush, rect);
// 		e->DrawFocusRectangle();
// 	}
// }
// 

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
// 
// 	case LVM_GETTOPINDEX:
// 		{
// 			// There's a very strange bug where the first
// 			// mouseover of an item causes it to be redrawn
// 			// and it flickers regardless of whether we are
// 			// double-buffered or not. The workaround is to
// 			// not draw the item under these circumstances.
// 			// See also OnDrawItem()
// 			m_IgnoreNextItemDraw = true;
// 			base.WndProc(ref m);
// 			m_IgnoreNextItemDraw = false;
// 		}
// 		return;
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

	auto item = ASTYPE(hit->Item->Tag, ITaskBase);

	if ((item == nullptr) || item->IsLocked)
		return;

	if (CalcCheckboxRect(hit->Item->Bounds).Contains(e->Location))
	{
		EditTaskDone(this, item->Id, !item->IsDone(false));
	}
	else if (CalcIconRect(hit->Item->Bounds).Contains(e->Location))
	{
		EditTaskIcon(this, item->Id);
	}
	else if (doubleClick)
	{
		EditTaskLabel(this, item->Id);
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
