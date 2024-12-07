#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
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
			public ref class TaskTimeLogReader
			{
			public:
				TaskTimeLogReader(String^ logFilePath);
				!TaskTimeLogReader();
				~TaskTimeLogReader();

				property int EntryCount { int get(); }

				bool GetEntry(int entry,
								 UInt32% taskID,
								 DateTime^% fromDate, 
								 DateTime^% toDate,
								 double% timeInHours,
								 String^% taskTitle,
								 String^% comment,
								 String^% person,
								 String^% path,
								 String^% type,
								 Drawing::Color% altColor);
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

				bool SetEntry(int entry,
								 UInt32 taskID,
								 DateTime^ fromDate,
								 DateTime^ toDate,
								 double timeInHours,
								 String^ taskTitle,
								 String^ comment,
								 String^ person,
								 String^ path,
								 String^ type,
								 Drawing::Color altColor);

				bool Save(String^ logFilePath);

			private:
				CTaskTimeLogItemArray* m_pItems;
			};
		}
	}
}

