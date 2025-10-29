#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Collections::Generic;

////////////////////////////////////////////////////////////////////////////////////////////////

struct TASKTIMELOGITEM;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			ref class Translator;

			// -------------------------------------------------------

			public ref class TaskTimeLogEntry
			{
			public:
				UInt32 TaskId;
				DateTime From;
				DateTime To;
				double TimeInHours;
				String^ TaskTitle;
				String^ Comment;
				String^ Person;
				String^ TaskPath;
				String^ Type;
				Drawing::Color AltColor;
			};

			// -------------------------------------------------------

			public ref class TaskTimeLog
			{
			public:
				TaskTimeLog();
				TaskTimeLog(Translator^ trans);
				
				List<TaskTimeLogEntry^>^ LoadEntries(String^ tasklistPath);
				List<TaskTimeLogEntry^>^ LoadEntries(String^ tasklistPath, UInt32 taskId);

				bool SaveEntries(String^ tasklistPath, List<TaskTimeLogEntry^>^ logEntries);
				bool SaveEntries(String^ tasklistPath, List<TaskTimeLogEntry^>^ logEntries, UInt32 taskId);

				bool AddEntry(String^ tasklistPath, TaskTimeLogEntry^ logEntry, bool logSeparately);
				
				String^ FormatLogAccessError(bool loading);

				static String^ GetLogPath(String^ tasklistPath);
				static String^ GetLogPath(String^ tasklistPath, UInt32 taskId);
				static String^ GetLogFileFilter(String^ tasklistPath, bool logSeparately);

			private:
				Translator^ m_Trans;

			private:
				static String^ ToString(const CString& str);
				static CString ToString(String^ str);

				static void Copy(TaskTimeLogEntry^ from, TASKTIMELOGITEM& to);
			};
		}
	}
}

