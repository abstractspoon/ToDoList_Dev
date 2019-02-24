#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

class IPreferences;
class ITaskList;
class ITaskList16;
class ITransText;

enum TDC_UNITS;

struct UITHEME;

typedef void* HTASKITEM;

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::Collections::Generic;

////////////////////////////////////////////////////////////////////////////////////////////////

#pragma warning( push )
#undef GetProfileInt
#undef GetProfileString

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			public ref class Task
			{
			public:
				enum class TimeUnits
				{
					Unknown = -1,

					Minutes,
					Hours,
					Days,
					Weekdays,
					Weeks,
					Months,
					Years,
				};

			public:
				Task(ITaskList* pTaskList, HTASKITEM hTask);        // GET & SET
				Task(const ITaskList* pTaskList, HTASKITEM hTask);  // GET ONLY
				Task(const Task^ task);

				bool IsValid();

				Task^ GetFirstSubtask();
				Task^ GetNextTask();
				Task^ GetParentTask();

				String^ GetTitle();
				String^ GetComments();
				String^ GetHtmlComments();
				String^ GetAllocatedBy();
				String^ GetStatus();
				String^ GetWebColor();
				String^ GetPriorityWebColor();
				String^ GetVersion();
				String^ GetExternalID();
				String^ GetCreatedBy();
				String^ GetPositionString();
				String^ GetPath(String^ delimiter);

				String^ GetIcon();
				Boolean HasIcon();

				UInt32 GetID();
				UInt32 GetParentID();
				UInt32 GetColor();
				UInt32 GetPosition();
				UInt32 GetPriority();
				UInt32 GetRisk();

				System::Drawing::Color GetTextDrawingColor();
				System::Windows::Media::Color GetTextMediaColor();
				System::Drawing::Color GetPriorityDrawingColor();
				System::Windows::Media::Color GetPriorityMediaColor();

				List<String^>^ GetAllocatedTo();
				List<String^>^ GetCategory();
				List<String^>^ GetTag();
				List<String^>^ GetDependency();
				List<String^>^ GetFileReference();

				String^ GetAllocatedTo(String^ delimiter);
				String^ GetCategory(String^ delimiter);
				String^ GetTag(String^ delimiter);
				String^ GetDependency(String^ delimiter);
				String^ GetFileReference(String^ delimiter);

				Byte GetPercentDone();
				double GetCost();

				DateTime GetLastModifiedDate();
				DateTime GetDoneDate();
				DateTime GetDueDate();
				DateTime GetStartDate();
				DateTime GetCreationDate();

				String^ GetDoneDateString();
				String^ GetDueDateString();
				String^ GetStartDateString();
				String^ GetCreationDateString();
				String^ GetLastModifiedDateString();
				String^ GetLastModifiedBy();

				Boolean IsDone();
				Boolean IsDue();
				Boolean IsGoodAsDone();
				Boolean HasSomeSubtasksDone();
				Boolean IsFlagged();
				Boolean IsParent();
				Boolean IsLocked();

				double GetTimeEstimate(TimeUnits% cUnits);
				double GetTimeSpent(TimeUnits% cUnits);

				String^ GetAttribute(String^ sAttrib);
				Boolean HasAttribute(String^ sAttrib);

				String^ GetCustomAttributeData(String^ sID);
				String^ GetMetaData(String^ sKey);

				// TODO
				Boolean GetRecurrence();

				// SETTERS -----------------------------------------------------

				Task^ NewSubtask(String^ sTitle);

				Boolean SetTitle(String^ sTitle);
				Boolean SetComments(String^ sComments);
				Boolean SetAllocatedBy(String^ sAllocBy);
				Boolean SetStatus(String^ sStatus);
				Boolean SetVersion(String^ sVersion);
				Boolean SetExternalID(String^ sExternalID);
				Boolean SetCreatedBy(String^ sCreatedBy);
				Boolean SetPosition(String^ sPosition);
				Boolean SetIcon(String^ sIcon);

				Boolean AddAllocatedTo(String^ sAllocTo);
				Boolean AddCategory(String^ sCategory);
				Boolean AddTag(String^ sTag);
				Boolean AddDependency(String^ sDependency);
				Boolean AddFileReference(String^ sFileLink);

				Boolean SetColor(UInt32 color);
				Boolean SetPriority(Byte nPriority);
				Boolean SetRisk(Byte Risk);

				Boolean SetPercentDone(Byte nPercent);
				Boolean SetCost(double dCost);
				Boolean SetFlag(Boolean bFlag);

				Boolean SetLastModified(DateTime^ dtLastMod);
				Boolean SetDoneDate(DateTime^ dtCompletion);
				Boolean SetDueDate(DateTime^ dtDue);
				Boolean SetStartDate(DateTime^ dtStart);
				Boolean SetCreationDate(DateTime^ dtCreation);

				Boolean SetTimeEstimate(double dTime, TimeUnits cUnits);
				Boolean SetTimeSpent(double dTime, TimeUnits cUnits);
				Boolean SetCustomAttributeData(String^ sID, String^ sValue);
				Boolean ClearCustomAttributeData(String^ sID);
				Boolean SetMetaData(String^ sKey, String^ sValue);
				Boolean ClearMetaData(String^ sKey);

				static TimeUnits Map(TDC_UNITS units);
				static TDC_UNITS Map(TimeUnits units);
				static DateTime Map(Int64 tDate);
				static Int64 Map(DateTime^ date);

			private: // -------------------------------------------------------
				ITaskList16* m_pTaskList;
				const ITaskList16* m_pConstTaskList;
				HTASKITEM m_hTask;

			private: // -------------------------------------------------------
				Task();

				String^ FormatList(List<String^>^ items, String^ delimiter);
			};

			////////////////////////////////////////////////////////////////////////////////////////////////

			public ref class TaskList
			{
			public:
				enum class TaskAttribute
				{
					Unknown = -1,

					Title = 0,
					Position,
					Id,
					ParentId,
					Path,
					Priority,
					Risk,
					Percent,
					TimeEstimate,
					TimeSpent,
					CreationDate,
					CreatedBy,
					LastModifiedDate,
					LastModifiedBy,
					StartDate,
					DueDate,
					DoneDate,
					Recurrence,
					AllocatedTo,
					AllocatedBy,
					Status,
					Category,
					Tags,
					ExternalId,
					Cost,
					Version,
					Flag,
					Dependency,
					FileLink,
					SubtaskDone,
					Comments,
					HtmlComments,
				};

			public:
				TaskList(ITaskList* pTaskList);        // GET & SET
				TaskList(const ITaskList* pTaskList);  // GET ONLY

				bool IsValid();

				String^ GetReportTitle();
				String^ GetReportDate();
				String^ GetMetaData(String^ sKey);

				UInt32  GetCustomAttributeCount();
				String^ GetCustomAttributeLabel(int nIndex);
				String^ GetCustomAttributeID(int nIndex);
				String^ GetCustomAttributeValue(int nIndex, String^ sItem);
				UInt32  GetCustomAttributeType(int nIndex);
				String^ GetCustomAttributeListData(int nIndex);
				Boolean IsCustomAttributeEnabled(int nIndex);

				UInt32 GetTaskCount();
				Task^ FindTask(UInt32 dwTaskID);

				Task^ GetFirstTask();
				Task^ NewTask(String^ sTitle);

				Boolean AddCustomAttribute(String^ sID, String^ sLabel, String^ sColumn);
				Boolean SetMetaData(String^ sKey, String^ sValue);
				Boolean ClearMetaData(String^ sKey);

				int GetAttributeList(Collections::Generic::HashSet<TaskList::TaskAttribute>^ attribs);

			private: // -------------------------------------------------------
				ITaskList16* m_pTaskList;
				const ITaskList16* m_pConstTaskList;

			private: // -------------------------------------------------------
				TaskList();

				void BuildAttributeList(Task^ task, Collections::Generic::HashSet<TaskList::TaskAttribute>^ attribs);
				void AddAttributeToList(TaskList::TaskAttribute attrib, String^ attribId, Task^ task,
												  Collections::Generic::HashSet<TaskList::TaskAttribute>^ attribs);

			};
		}
	}
}
#pragma warning( pop )

