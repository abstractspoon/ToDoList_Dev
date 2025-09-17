
#include "stdafx.h"
#include "TaskComboBox.h"

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

public ref class TaskPosComparer : public IComparer<ITask^>
{
public:
	virtual int Compare(ITask^ task1, ITask^ task2)
	{
		return StringUtil::NaturalCompare(task1->Position, task2->Position);
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////

TaskComboBox::TaskComboBox()
	:
	OwnerdrawComboBoxBase(true), // fixed
	m_NoneTask(nullptr)
{
	Sorted = false; // we control the order
}

void TaskComboBox::Initialise(IEnumerable<ITask^>^ taskItems,
							  UIExtension::TaskIcon^ taskIcons, 
							  UInt32 selTaskId)
{
	Initialise(taskItems, taskIcons, selTaskId, nullptr);
}

void TaskComboBox::Initialise(IEnumerable<ITask^>^ taskItems, 
							  UIExtension::TaskIcon^ taskIcons, 
							  UInt32 selTaskId, 
							  ITask^ noneTask)
{
	m_TaskIcons = taskIcons;

	if (noneTask != nullptr)
	{
		Items->Add(noneTask);
		m_NoneTask = noneTask;
	}

	auto sortedTasks = Enumerable::ToList(taskItems);
	sortedTasks->Sort(gcnew TaskPosComparer());

	for each(auto task in sortedTasks)
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

String^ TaskComboBox::SelectedTaskTitle::get()
{
	if (SelectedTaskId::get() == 0)
		return String::Empty;

	return ASTYPE(SelectedItem, ITask)->Title;
}

void TaskComboBox::OnMeasureItem(MeasureItemEventArgs^ e)
{
	ComboBox::OnMeasureItem(e);

	e->ItemHeight = UIExtension::TaskIcon::IconSize;
}

void TaskComboBox::OnDrawItem(DrawItemEventArgs^ e)
{
	OwnerdrawComboBoxBase::OnDrawItem(e);

	if (e->Index < 0)
		return;

	auto taskItem = ASTYPE(Items[e->Index], ITask);

	if (taskItem != nullptr)
	{
		auto rect = e->Bounds;

		if (taskItem != m_NoneTask)
		{
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
		}

		auto brush = TextBrush(e);

		e->Graphics->DrawString(taskItem->Title, Font, brush, rect);
		e->DrawFocusRectangle();

		delete brush;
	}
}
