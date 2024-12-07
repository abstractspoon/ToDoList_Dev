// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "TaskTimeLog.h"

#include <ToDoList\TDCTaskTimeLog.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

TaskTimeLogReader::TaskTimeLogReader(String^ logFilePath)
	:
	m_pItems(new CTaskTimeLogItemArray())
{
	CTDCTaskTimeLog::LoadLogFile(MS(logFilePath), *m_pItems, FALSE, CString());
}

TaskTimeLogReader::!TaskTimeLogReader()
{
	delete m_pItems;
}

TaskTimeLogReader::~TaskTimeLogReader()
{
	this->!TaskTimeLogReader();
}

int TaskTimeLogReader::EntryCount::get()
{
	if (m_pItems)
		return (m_pItems->GetSize());

	return 0;
}

bool TaskTimeLogReader::GetEntry(int entry, 
				 UInt32% taskID,
				 DateTime% fromDate,
				 DateTime% toDate,
				 double% timeInHours,
				 String^% taskTitle,
				 String^% comment,
				 String^% person,
				 String^% path,
				 String^% type,
				 Drawing::Color% altColor)
{
	if ((entry < 0) || (entry >= EntryCount))
		return false;

	const TASKTIMELOGITEM& li = m_pItems->GetAt(entry);

	taskID = li.dwTaskID;
	fromDate = DateTime::FromOADate(li.dtFrom.m_dt);
	toDate = DateTime::FromOADate(li.dtTo);
	timeInHours = li.dHours;
	taskTitle = gcnew String(li.sTaskTitle);
	comment = gcnew String(li.sComment);
	person = gcnew String(li.sPerson);
	path = gcnew String(li.sPath);
	type = gcnew String(li.sType);

	if (li.crAltColor == CLR_NONE)
		altColor = Drawing::Color::Empty;
	else
		altColor = Drawing::Color::FromArgb(li.crAltColor);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////

TaskTimeLogWriter::TaskTimeLogWriter(int entryCount)
	:
	m_pItems(new CTaskTimeLogItemArray())
{
	if (entryCount > 0)
		m_pItems->SetSize(entryCount);
}

TaskTimeLogWriter::!TaskTimeLogWriter()
{
	delete m_pItems;
}

TaskTimeLogWriter::~TaskTimeLogWriter()
{
	this->!TaskTimeLogWriter();
}

bool TaskTimeLogWriter::SetEntry(int entry,
								 UInt32 taskID,
								 DateTime^ fromDate,
								 DateTime^ toDate,
								 double timeInHours,
								 String^ taskTitle,
								 String^ comment,
								 String^ person,
								 String^ path,
								 String^ type,
								 Drawing::Color altColor)
{
	if (!m_pItems)
		return false;

	if ((entry < 0) || (entry >= m_pItems->GetSize()))
		return false;

	TASKTIMELOGITEM& li = m_pItems->GetAt(entry);

	li.dwTaskID = taskID;
	li.dtFrom.m_dt = fromDate->ToOADate();
	li.dtTo.m_dt = toDate->ToOADate();
	li.dHours = timeInHours;
	li.sTaskTitle = MS(taskTitle);
	li.sComment = MS(comment);
	li.sPerson = MS(person);
	li.sPath = MS(path);
	li.sType = MS(type);

	if (li.crAltColor == CLR_NONE)
		altColor = Drawing::Color::Empty;
	else
		altColor = Drawing::Color::FromArgb(li.crAltColor);

	return true;
}

bool TaskTimeLogWriter::Save(String^ logFilePath)
{
	return (CTDCTaskTimeLog::SaveLogFile(MS(logFilePath), *m_pItems) != FALSE);
}
