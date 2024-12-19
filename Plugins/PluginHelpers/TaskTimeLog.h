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
				static List<TaskTimeLogEntry^>^ Load(String^ tasklistPath);
				static bool Save(String^ tasklistPath, List<TaskTimeLogEntry^>^ logEntries);
				static bool Add(String^ tasklistPath, TaskTimeLogEntry^ logEntry, bool logSeparately);

				static String^ GetPath(String^ tasklistPath);
				static String^ GetPath(String^ tasklistPath, UInt32 taskId, bool logSeparately);

				static property String^ LogAccessErrorMsg { String^ get(); }

			private:
				static String^ ToString(const CString& str);
				static CString ToString(String^ str);

				static void Copy(TaskTimeLogEntry^ from, TASKTIMELOGITEM& to);
			};
		}
	}
}

