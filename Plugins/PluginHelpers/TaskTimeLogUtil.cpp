// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "TaskTimeLogUtil.h"
#include "Translator.h"

#include <ToDoList\TDCTaskTimeLog.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Windows::Forms;
using namespace System::IO;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

const CString EMPTY_STR;

const LPCWSTR LOG_LOAD_ERR = L"The log file could not be loaded.";
const LPCWSTR LOG_SAVE_ERR = L"The log file could not be updated.";
const LPCWSTR LOG_ERR_SUGGEST = L"Please ensure that the file is not already open for editing and \n"
								L"that you have the correct permissions and then try again.";

////////////////////////////////////////////////////////////////////////////////////////////////

TaskTimeLogUtil::TaskTimeLogUtil() : m_Trans(nullptr)
{
}

TaskTimeLogUtil::TaskTimeLogUtil(Translator^ trans) : m_Trans(trans)
{
	if (m_Trans != nullptr)
		m_Trans->InitialiseLocalizer();
}

List<TaskTimeLogEntry^>^ TaskTimeLogUtil::LoadEntries(String^ tasklistPath)
{
	return LoadEntries(tasklistPath, 0);
}

List<TaskTimeLogEntry^>^ TaskTimeLogUtil::LoadEntries(String^ tasklistPath, UInt32 taskId)
{
	// Point any resource loading to our local copies
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

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

bool TaskTimeLogUtil::SaveEntries(String^ tasklistPath, List<TaskTimeLogEntry^>^ logEntries)
{
	return SaveEntries(tasklistPath, logEntries, 0);
}

bool TaskTimeLogUtil::SaveEntries(String^ tasklistPath, List<TaskTimeLogEntry^>^ logEntries, UInt32 taskId)
{
	// Point any resource loading to our local copies
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CTaskTimeLogItemArray aLogEntries;
	aLogEntries.SetSize(logEntries->Count);

	for (int nEntry = 0; nEntry < aLogEntries.GetSize(); nEntry++)
	{
		TASKTIMELOGITEM& li = aLogEntries.GetAt(nEntry);
		TaskTimeLogEntry^ logEntry = logEntries[nEntry];

		Copy(logEntry, li);
	}

	String^ logFilePath = GetLogPath(tasklistPath, taskId);

	// Note: We don't preserve the existing header 
	return (CTDCTaskTimeLog::SaveLogFile(MS(logFilePath), aLogEntries, FALSE) != FALSE);
}

bool TaskTimeLogUtil::AddEntry(String^ tasklistPath, TaskTimeLogEntry^ logEntry, bool logSeparately)
{
	// Point any resource loading to our local copies
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	TASKTIMELOGITEM li;
	Copy(logEntry, li);

	return (CTDCTaskTimeLog(MS(tasklistPath)).LogTime(li, logSeparately) != FALSE);
}

String^ TaskTimeLogUtil::FormatLogAccessError(bool loading)
{
	String^ part1 = gcnew String(loading ? LOG_LOAD_ERR : LOG_SAVE_ERR);
	String^ part2 = gcnew String(LOG_ERR_SUGGEST);

	if (m_Trans)
	{
		part1 = m_Trans->Translate(part1, Translator::Type::Text);
		part2 = m_Trans->Translate(part2, Translator::Type::Text);
	}

	return String::Format(L"{0}\n\n{1}", part1, part2);
}

String^ TaskTimeLogUtil::GetLogPath(String^ tasklistPath)
{
	return ToString(CTDCTaskTimeLog(MS(tasklistPath)).GetLogPath());
}

String^ TaskTimeLogUtil::GetLogPath(String^ tasklistPath, UInt32 taskId)
{
	if (taskId == 0)
		return GetLogPath(tasklistPath);

	// else
	return ToString(CTDCTaskTimeLog(MS(tasklistPath)).GetLogPath(taskId, true));
}

String^ TaskTimeLogUtil::GetLogFileFilter(String^ tasklistPath, bool logSeparately)
{
	return ToString(CTDCTaskTimeLog(MS(tasklistPath)).GetLogFileFilter(logSeparately));
}

void TaskTimeLogUtil::Copy(TaskTimeLogEntry^ from, TASKTIMELOGITEM& to)
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

CString TaskTimeLogUtil::ToString(String^ str)
{
	if (String::IsNullOrWhiteSpace(str))
		return EMPTY_STR;

	// else
	return CString(MS(str));
}

String^ TaskTimeLogUtil::ToString(const CString& str)
{
	if (str.IsEmpty())
		return String::Empty;

	return gcnew String(str);
}
