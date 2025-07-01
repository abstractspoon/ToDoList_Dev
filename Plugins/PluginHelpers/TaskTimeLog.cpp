// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "TaskTimeLog.h"
#include "Translator.h"

#include <ToDoList\TDCTaskTimeLog.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Windows::Forms;
using namespace System::IO;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

const CString EMPTY_STR;

////////////////////////////////////////////////////////////////////////////////////////////////

List<TaskTimeLogEntry^>^ TaskTimeLog::LoadEntries(String^ tasklistPath)
{
	return LoadEntries(tasklistPath, 0);
}

List<TaskTimeLogEntry^>^ TaskTimeLog::LoadEntries(String^ tasklistPath, UInt32 taskId)
{
	String^ logFilePath = GetLogPath(tasklistPath, taskId);
	CTaskTimeLogItemArray aLogEntries;
	CString sUnused;

	if (CTDCTaskTimeLog::LoadLogFile(MS(logFilePath), aLogEntries, FALSE, sUnused) == FALSE)
	{
		// If the log file existed and failed to load return null
		// else we return an empty list by falling through
		if (File::Exists(logFilePath))
			return nullptr;
	}

	List<TaskTimeLogEntry^>^ logEntries = gcnew List<TaskTimeLogEntry^>();

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
	
	return logEntries;
}

bool TaskTimeLog::SaveEntries(String^ tasklistPath, List<TaskTimeLogEntry^>^ logEntries)
{
	return SaveEntries(tasklistPath, logEntries, 0);
}

bool TaskTimeLog::SaveEntries(String^ tasklistPath, List<TaskTimeLogEntry^>^ logEntries, UInt32 taskId)
{
	CTaskTimeLogItemArray aLogEntries;
	aLogEntries.SetSize(logEntries->Count);

	for (int nEntry = 0; nEntry < aLogEntries.GetSize(); nEntry++)
	{
		TASKTIMELOGITEM& li = aLogEntries.GetAt(nEntry);
		TaskTimeLogEntry^ logEntry = logEntries[nEntry];

		Copy(logEntry, li);
	}

	String^ logFilePath = GetLogPath(tasklistPath, taskId);

	return (CTDCTaskTimeLog::SaveLogFile(MS(logFilePath), aLogEntries, FALSE) != FALSE);
}

bool TaskTimeLog::AddEntry(String^ tasklistPath, TaskTimeLogEntry^ logEntry, bool logSeparately)
{
	TASKTIMELOGITEM li;
	Copy(logEntry, li);

	return (CTDCTaskTimeLog(MS(tasklistPath)).LogTime(li, logSeparately) != FALSE);
}

String^ TaskTimeLog::FormatLogAccessError(Translator^ trans, bool loading)
{
	String^ part1 = trans->Translate(loading ?
									 "The log file could not be loaded." :
									 "The log file could not be updated.",
									 Translator::Type::Text);
	String^ part2 = trans->Translate("Please ensure that the file is not already open for editing and \n"
									 "that you have the correct permissions and then try again.",
									 Translator::Type::Text);

	return String::Format(L"{0}\n\n{1}", part1, part2);
}

String^ TaskTimeLog::GetLogPath(String^ tasklistPath)
{
	return ToString(CTDCTaskTimeLog(MS(tasklistPath)).GetLogPath());
}

String^ TaskTimeLog::GetLogPath(String^ tasklistPath, UInt32 taskId)
{
	if (taskId == 0)
		return GetLogPath(tasklistPath);

	// else
	return ToString(CTDCTaskTimeLog(MS(tasklistPath)).GetLogPath(taskId, true));
}

String^ TaskTimeLog::GetLogFileFilter(String^ tasklistPath, bool logSeparately)
{
	return ToString(CTDCTaskTimeLog(MS(tasklistPath)).GetLogFileFilter(logSeparately));
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
