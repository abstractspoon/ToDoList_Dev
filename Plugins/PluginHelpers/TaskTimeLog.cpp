// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "TaskTimeLog.h"

#include <ToDoList\TDCTaskTimeLog.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

List<TaskTimeLogEntry^>^ TaskTimeLog::Load(String^ logFilePath)
{
	CTaskTimeLogItemArray aLogEntries;
	CString sUnused;

	List<TaskTimeLogEntry^>^ logEntries = gcnew List<TaskTimeLogEntry^>();

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

String^ TaskTimeLog::ToString(const CString& str)
{
	if (str.IsEmpty())
		return String::Empty;

	return gcnew String(str);
}

bool TaskTimeLog::Save(String^ logFilePath, List<TaskTimeLogEntry^>^ logEntries)
{
	CTaskTimeLogItemArray aLogEntries;
	aLogEntries.SetSize(logEntries->Count);

	for (int nEntry = 0; nEntry < aLogEntries.GetSize(); nEntry++)
	{
		TASKTIMELOGITEM& li = aLogEntries.GetAt(nEntry);
		TaskTimeLogEntry^ logEntry = logEntries[nEntry];

		li.dwTaskID = logEntry->TaskId;
		li.dtFrom = logEntry->From.ToOADate();
		li.dtTo = logEntry->To.ToOADate();
		li.dHours = logEntry->TimeInHours;
		li.sTaskTitle = ToString(logEntry->TaskTitle);
		li.sComment = ToString(logEntry->Comment);
		li.sPerson = ToString(logEntry->Person);
		li.sPath = ToString(logEntry->TaskPath);
		li.sType = ToString(logEntry->Type);

		if (logEntry->AltColor == Drawing::Color::Empty)
			li.crAltColor = CLR_NONE;
		else
			li.crAltColor = logEntry->AltColor.ToArgb();
	}

	return (CTDCTaskTimeLog::SaveLogFile(MS(logFilePath), aLogEntries, FALSE) != FALSE);
}

CString TaskTimeLog::ToString(String^ str)
{
	static CString EMPTY_STR;

	if (String::IsNullOrWhiteSpace(str))
		return EMPTY_STR;

	// else
	return CString(MS(str));
}
