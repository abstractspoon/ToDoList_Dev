// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "TaskTimeLog.h"

#include <ToDoList\TDCTaskTimeLog.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

List<TaskTimeLogEntry^>^ TaskTimeLog::Load(String^ tasklistPath)
{
	CTaskTimeLogItemArray aLogEntries;
	CString sUnused;

	List<TaskTimeLogEntry^>^ logEntries = gcnew List<TaskTimeLogEntry^>();
	String^ logFilePath = GetPath(tasklistPath);

	if (CTDCTaskTimeLog::LoadLogFile(MS(logFilePath), aLogEntries, FALSE, sUnused))
	{
		for (int nEntry = 0; nEntry < aLogEntries.GetSize(); nEntry++)
		{
			const TASKTIMELOGITEM& li = aLogEntries.GetAt(nEntry);
			TaskTimeLogEntry^ logEntry = gcnew TaskTimeLogEntry();

			logEntry->TaskId = li.dwTaskID;
			logEntry->From = DateTime::FromOADate(li.dtFrom.m_dt);
			logEntry->To = DateTime::FromOADate(li.dtTo);
			logEntry->TimeInHours = li.dHours;
			logEntry->TaskTitle = ToString(li.sTaskTitle);
			logEntry->Comment = ToString(li.sComment);
			logEntry->Person = ToString(li.sPerson);
			logEntry->TaskPath = ToString(li.sPath);
			logEntry->Type = ToString(li.sType);

			if (li.crAltColor == CLR_NONE)
				logEntry->AltColor = Drawing::Color::Empty;
			else
				logEntry->AltColor = Drawing::Color::FromArgb(li.crAltColor);

			logEntries->Add(logEntry);
		}
	}
	
	return logEntries;
}

bool TaskTimeLog::Save(String^ tasklistPath, List<TaskTimeLogEntry^>^ logEntries)
{
	CTaskTimeLogItemArray aLogEntries;
	aLogEntries.SetSize(logEntries->Count);

	for (int nEntry = 0; nEntry < aLogEntries.GetSize(); nEntry++)
	{
		TASKTIMELOGITEM& li = aLogEntries.GetAt(nEntry);
		TaskTimeLogEntry^ logEntry = logEntries[nEntry];

		Copy(logEntry, li);
	}

	String^ logFilePath = GetPath(tasklistPath);

	return (CTDCTaskTimeLog::SaveLogFile(MS(logFilePath), aLogEntries, FALSE) != FALSE);
}

bool TaskTimeLog::Add(String^ tasklistPath, TaskTimeLogEntry^ logEntry, bool logSeparately)
{
	TASKTIMELOGITEM li;
	Copy(logEntry, li);

	return (CTDCTaskTimeLog(MS(tasklistPath)).LogTime(li, logSeparately) != FALSE);
}

String^ TaskTimeLog::GetPath(String^ tasklistPath)
{
	return ToString(CTDCTaskTimeLog(MS(tasklistPath)).GetLogPath());
}

String^ TaskTimeLog::GetPath(String^ tasklistPath, UInt32 taskId, bool logSeparately)
{
	return ToString(CTDCTaskTimeLog(MS(tasklistPath)).GetLogPath(taskId, logSeparately));
}

void TaskTimeLog::Copy(TaskTimeLogEntry^ from, TASKTIMELOGITEM& to)
{
	to.dwTaskID = from->TaskId;
	to.dtFrom = from->From.ToOADate();
	to.dtTo = from->To.ToOADate();
	to.dHours = from->TimeInHours;
	to.sTaskTitle = ToString(from->TaskTitle);
	to.sComment = ToString(from->Comment);
	to.sPerson = ToString(from->Person);
	to.sPath = ToString(from->TaskPath);
	to.sType = ToString(from->Type);

	if (from->AltColor == Drawing::Color::Empty)
		to.crAltColor = CLR_NONE;
	else
		to.crAltColor = from->AltColor.ToArgb();
}

CString TaskTimeLog::ToString(String^ str)
{
	static CString EMPTY_STR;

	if (String::IsNullOrWhiteSpace(str))
		return EMPTY_STR;

	// else
	return CString(MS(str));
}

String^ TaskTimeLog::ToString(const CString& str)
{
	if (str.IsEmpty())
		return String::Empty;

	return gcnew String(str);
}
