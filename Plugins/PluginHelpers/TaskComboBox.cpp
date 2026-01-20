
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
// Private class which wraps the caller's ITask just so that 
// we can control what gets returned from ToString()

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

bool TaskComboBox::SelectNextItem(String^ text, bool bForward)
{
	if (String::IsNullOrEmpty(text))
	{
		SelectedIndex = -1;
		return false;
	}

	// else
	int nSel = SelectedIndex;
	int from = (bForward ? (nSel + 1) : (nSel - 1));

	int nNext = FindNextItem(text, from, bForward);

	if (nNext == nSel)
		return false;

	SelectedIndex = nNext;
	return true;
}

int TaskComboBox::FindNextItem(String^ text, int from, bool bForward)
{
	int nNumItems = Items->Count;

	if (nNumItems == 0)
		return CB_ERR;

	int nNext = CB_ERR;

	if (bForward)
	{
		if ((from < 0) || (from >= nNumItems))
			from = 0;

		// From from to end of combo
		if (FindNextItem(text, from, (nNumItems - 1), 1, nNext))
			return nNext;

		// From start of combo to from
		if ((from > 0) && FindNextItem(text, 0, (from - 1), 1, nNext))
			return nNext;
	}
	else // backwards
	{
		if ((from < 0) || (from >= nNumItems))
			from = (nNumItems - 1);

		// From from to start of combo
		if (FindNextItem(text, from, 0, -1, nNext))
			return nNext;

		// From end of combo to from
		if (FindNextItem(text, (nNumItems - 1), (from + 1), -1, nNext))
			return nNext;
	}

	// else
	return CB_ERR;
}

bool TaskComboBox::FindNextItem(String^ text, int from, int to, int increment, int& next)
{
	// Sanity checks
	if (!(((increment == 1) && (to >= from)) || ((increment == -1) && (to <= from))))
	{
		ASSERT(0);
		return false;
	}

	next = CB_ERR;
	to += increment; // so the != will work to stop the loop

	for (int nItem = from; nItem != to; nItem += increment)
	{
		if (Misc::Find(text, Items[nItem]->ToString()) != -1)
		{
			next = nItem;
			break;
		}
	}

	return (next != CB_ERR);
}
