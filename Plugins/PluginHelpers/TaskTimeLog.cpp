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

TaskTimeLogEntry^ TaskTimeLogReader::GetEntry(int entry)
{
	if ((entry < 0) || (entry >= EntryCount))
		return nullptr;

	const TASKTIMELOGITEM& li = m_pItems->GetAt(entry);

	TaskTimeLogEntry^ logEntry = gcnew TaskTimeLogEntry();

	logEntry->TaskId = li.dwTaskID;
	logEntry->From = DateTime::FromOADate(li.dtFrom.m_dt);
	logEntry->To = DateTime::FromOADate(li.dtTo);
	logEntry->TimeInHours = li.dHours;
	logEntry->TaskTitle = gcnew String(li.sTaskTitle);
	logEntry->Comment = gcnew String(li.sComment);
	logEntry->Person = gcnew String(li.sPerson);
	logEntry->TaskPath = gcnew String(li.sPath);
	logEntry->Type = gcnew String(li.sType);

	if (li.crAltColor == CLR_NONE)
		logEntry->AltColor = Drawing::Color::Empty;
	else
		logEntry->AltColor = Drawing::Color::FromArgb(li.crAltColor);

	return logEntry;
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

bool TaskTimeLogWriter::SetEntry(int entry, TaskTimeLogEntry^ logEntry)
{
	if (!m_pItems)
		return false;

	if ((entry < 0) || (entry >= m_pItems->GetSize()))
		return false;

	TASKTIMELOGITEM& li = m_pItems->GetAt(entry);

	li.dwTaskID = logEntry->TaskId;
	li.dtFrom.m_dt = logEntry->From.ToOADate();
	li.dtTo.m_dt = logEntry->To.ToOADate();
	li.dHours = logEntry->TimeInHours;
	li.sTaskTitle = MS(logEntry->TaskTitle);
	li.sComment = MS(logEntry->Comment);
	li.sPerson = MS(logEntry->Person);
	li.sPath = MS(logEntry->TaskPath);
	li.sType = MS(logEntry->Type);

	if (logEntry->AltColor == Drawing::Color::Empty)
		li.crAltColor = CLR_NONE;
	else
		li.crAltColor = logEntry->AltColor.ToArgb();

	return true;
}

bool TaskTimeLogWriter::Save(String^ logFilePath)
{
	return (CTDCTaskTimeLog::SaveLogFile(MS(logFilePath), *m_pItems) != FALSE);
}
