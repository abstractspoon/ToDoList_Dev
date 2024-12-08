#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Collections::Generic;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
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
				static List<TaskTimeLogEntry^>^ Load(String^ logFilePath);
				static bool Save(String^ logFilePath, List<TaskTimeLogEntry^>^ logEntries);

			private:
				static String^ ToString(const CString& str);
				static CString ToString(String^ str);
			};
		}
	}
}

