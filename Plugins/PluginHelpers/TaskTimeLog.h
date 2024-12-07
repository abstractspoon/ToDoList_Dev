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
			public ref class TaskTimeLogReader
			{
			public:
				TaskTimeLogReader(String^ logFilePath);
				!TaskTimeLogReader();
				~TaskTimeLogReader();

				property int EntryCount { int get(); }

				bool GetEntry(int entry,
							  [Out] UInt32% taskID,
							  [Out] DateTime% fromDate,
							  [Out] DateTime% toDate,
							  [Out] double% timeInHours,
							  [Out] String^% taskTitle,
							  [Out] String^% comment,
							  [Out] String^% person,
							  [Out] String^% path,
							  [Out] String^% type,
							  [Out] Drawing::Color% altColor);
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

