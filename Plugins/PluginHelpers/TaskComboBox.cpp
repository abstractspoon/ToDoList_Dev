
#include "stdafx.h"
#include "TaskComboBox.h"
#include "Win32.h"

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
// Private class to override ToString()

ref class WrappedITask : ITask
{
public:
	WrappedITask(ITask^ task)
	{
		m_ITask = task;
	}

	String^ ToString() override { return Title; }

	virtual property UInt32 Id { UInt32 get()			{ return m_ITask->Id ; } }
	virtual property String^ Title { String^ get()		{ return m_ITask->Title; }; }
	virtual property String^ Position { String^ get()	{ return m_ITask->Position; } }
	virtual property int Depth { int get()				{ return m_ITask->Depth; } }
	virtual property bool HasIcon { bool get()			{ return m_ITask->HasIcon; } }

private: 
	ITask^ m_ITask;
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
		auto wrap = gcnew WrappedITask(noneTask);
		Items->Add(wrap);

		m_NoneTask = wrap;
	}

	auto sortedTasks = Enumerable::ToList(taskItems);
	sortedTasks->Sort(gcnew TaskPosComparer());

	for each(auto task in sortedTasks)
	{
		auto wrap = gcnew WrappedITask(task);
		Items->Add(wrap);

		if (task->Id == selTaskId)
			SelectedItem = wrap;
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

void TaskComboBox::OnTextChanged(EventArgs^ e)
{
	SelectNextFind(true);
}

void TaskComboBox::SelectNextFind(bool bForward)
{
	// Have to use raw Win32 here because the behaviour when 
	// setting the edit selection works differently in Winforms
	String^ editText = Win32::GetWindowText(Handle);
	DWORD dwSel = ::SendMessage(Win32::GetHwnd(Handle), CB_GETEDITSEL, 0, 0);

	if (SelectNextItem(editText, bForward))
	{
		// Restore the text and selection because the
		// selection change will have overwritten it
		Win32::SetWindowText(Handle, editText);
		::SendMessage(Win32::GetHwnd(Handle), CB_SETEDITSEL, 0, dwSel);
	}
}

