#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Collections::Generic;

struct TASKTIMELOGITEM;

typedef CArray<TASKTIMELOGITEM, TASKTIMELOGITEM&> CTaskTimeLogItemArray;

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

			public ref class TaskTimeLogReader
			{
			public:
				TaskTimeLogReader(String^ logFilePath);
				!TaskTimeLogReader();
				~TaskTimeLogReader();

				property int EntryCount { int get(); }
				TaskTimeLogEntry^ GetEntry(int entry);

			private:
				CTaskTimeLogItemArray* m_pItems;
			};

			// -------------------------------------------------------

			public ref class TaskTimeLogWriter
			{
			public:
				TaskTimeLogWriter(int entryCount);
				!TaskTimeLogWriter();
				~TaskTimeLogWriter();

				bool SetEntry(int entry, TaskTimeLogEntry^ logEntry);
				bool Save(String^ logFilePath);

			private:
				CTaskTimeLogItemArray* m_pItems;
			};
		}
	}
}

