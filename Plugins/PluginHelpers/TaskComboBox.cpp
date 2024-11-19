
#include "stdafx.h"
#include "TaskComboBox.h"

#include <shared\Clipboard.h>
#include <shared\Misc.h>
#include <shared\GraphicsMisc.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Diagnostics;
using namespace System::Drawing;
using namespace System::Windows::Forms;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

TaskComboBox::TaskComboBox()
{
	DrawMode = Windows::Forms::DrawMode::OwnerDrawFixed;
}

void TaskComboBox::Initialise(IEnumerable<ITask^>^ taskItemsSortedByPosition, UIExtension::TaskIcon^ taskIcons, UInt32 selTaskId)
{
	m_TaskIcons = taskIcons;

	// Populate combo
	for each(auto task in taskItemsSortedByPosition)
	{
		Items->Add(task);

		if (task->Id == selTaskId)
			SelectedItem = task;
	}
}

UInt32 TaskComboBox::SelectedTaskId::get()
{
	auto selItem = ASTYPE(SelectedItem, ITask);
	return ((selItem == nullptr) ? 0 : selItem->Id);
}

void TaskComboBox::OnMeasureItem(MeasureItemEventArgs^ e)
{
	ComboBox::OnMeasureItem(e);

	e->ItemHeight = UIExtension::TaskIcon::IconSize;
}

void TaskComboBox::OnDrawItem(DrawItemEventArgs^ e)
{
	if (e->Index < 0)
		return;

	auto taskItem = ASTYPE(Items[e->Index], ITask);

	if (taskItem != nullptr)
	{
		e->DrawBackground();

		auto rect = e->Bounds;
		bool listItem = !(e->State.HasFlag(DrawItemState::ComboBoxEdit));

		if (listItem)
		{
			for (int i = 0; i < taskItem->Depth; i++)
				rect.X += UIExtension::TaskIcon::IconSize;
		}

		if (taskItem->HasIcon && m_TaskIcons->Get(taskItem->Id))
		{
			m_TaskIcons->Draw(e->Graphics, rect.X, rect.Y);
			rect.X += UIExtension::TaskIcon::IconSize;
		}
		else if (listItem)
		{
			rect.X += UIExtension::TaskIcon::IconSize;
		}

		auto brush = gcnew SolidBrush(e->ForeColor);

		e->Graphics->DrawString(taskItem->Title, Font, brush, rect);
		e->DrawFocusRectangle();

		delete brush;
	}
}
