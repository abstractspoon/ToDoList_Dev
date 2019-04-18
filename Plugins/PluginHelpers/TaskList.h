#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

#include <Interfaces\ITasklist.h>

////////////////////////////////////////////////////////////////////////////////////////////////

class ITransText;

struct UITHEME;

enum TDC_UNITS;
enum TDC_ATTRIBUTE;

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

				enum class Attribute
				{
					Unknown = -1,

					Title = 0,
					DoneDate,
					DueDate,
					StartDate,
					Priority,
					Color,
					AllocatedTo,
					AllocatedBy,
					Status,
					Category,
					Percent,
					TimeEstimate,
					TimeSpent,
					FileReference,
					Comments,
					Flag,
					CreationDate,
					CreatedBy,
					Risk,
					ExternalId,
					Cost,
					Dependency,
					Recurrence,
					Version,
					Position,
					Id,
					LastModifiedDate,
					Icon,
					Tags,
					CustomAttribute,
					OffsetTask,
					SubtaskDone,
					MetaData,
					ProjectName,
					ParentId,
					LastModifiedBy,
					Path,

					// new values here ONLY

				};

				// ----------------------------------------------------------------------------

			public:
				Task(ITaskList* pTaskList, HTASKITEM hTask);        // GET & SET
				Task(const ITaskList* pTaskList, HTASKITEM hTask);  // GET ONLY
				Task(const Task^ task);

				Boolean IsValid();

				Task^ GetFirstSubtask();
				Task^ GetNextTask();
				Task^ GetParentTask();

				String^ GetTitle();
				String^ GetComments();
				String^ GetHtmlComments();
				String^ GetAllocatedBy();
				String^ GetStatus();
				String^ GetWebColor();
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
				
				Int32 GetPriority(bool calculated);
				Int32 GetRisk(bool calculated);

				System::Drawing::Color GetTextDrawingColor();
				System::Windows::Media::Color GetTextMediaColor();
				System::Drawing::Color GetPriorityDrawingColor();
				System::Windows::Media::Color GetPriorityMediaColor();

				String^ GetTextForeWebColor();
				String^ GetTextBackWebColor();
				String^ GetPriorityWebColor();

				List<String^>^ GetAllocatedTo();
				List<String^>^ GetCategory();
				List<String^>^ GetTag();
				List<String^>^ GetDependency();
				List<String^>^ GetFileReference();

				String^ FormatAllocatedTo(String^ delimiter);
				String^ FormatCategory(String^ delimiter);
				String^ FormatTag(String^ delimiter);
				String^ FormatDependency(String^ delimiter);
				String^ FormatFileReference(String^ delimiter);

				Byte GetPercentDone(bool calculated);
				double GetCost(bool calculated);

				DateTime GetDoneDate();
				DateTime GetDueDate(bool calculated);
				DateTime GetStartDate(bool calculated);
				DateTime GetCreationDate();
				DateTime GetLastModifiedDate();

				String^ GetDoneDateString();
				String^ GetDueDateString(bool calculated);
				String^ GetStartDateString(bool calculated);
				String^ GetCreationDateString();
				String^ GetLastModifiedDateString();
				String^ GetLastModifiedBy();

				Boolean IsDone();
				Boolean IsDue();
				Boolean IsGoodAsDone();
				Boolean HasSomeSubtasksDone();
				Boolean IsFlagged(bool calculated);
				Boolean IsParent();
				Boolean IsLocked(bool calculated);

				double GetTimeEstimate(TimeUnits% cUnits, bool calculated);
				double GetTimeSpent(TimeUnits% cUnits, bool calculated);

				String^ GetAttribute(Attribute attrib);
				Boolean HasAttribute(Attribute attrib);

				String^ GetCustomAttributeData(String^ sID);
				String^ GetMetaData(String^ sKey);

				// TODO
				Boolean GetRecurrence();

				// SETTERS ----------------------------------------------------

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

				// MISC -------------------------------------------------------

				static TimeUnits MapUnits(TDC_UNITS units);
				static TDC_UNITS MapUnits(TimeUnits units);
				static DateTime MapDate(Int64 tDate);
				static Int64 MapDate(DateTime^ date);
				static Attribute MapAttribute(TDC_ATTRIBUTE attrib);
				static TDC_ATTRIBUTE MapAttribute(Attribute attrib);

			private: // -------------------------------------------------------
				ITASKLISTBASE* m_pTaskList;
				const ITASKLISTBASE* m_pConstTaskList;
				HTASKITEM m_hTask;

			private: // -------------------------------------------------------
				Task();

				String^ FormatList(List<String^>^ items, String^ delimiter);
			};

			////////////////////////////////////////////////////////////////////////////////////////////////

			public ref class TaskList
			{
			public:
				TaskList(ITaskList* pTaskList);        // GET & SET
				TaskList(const ITaskList* pTaskList);  // GET ONLY

				bool IsValid();
				bool HasAttribute(Task::Attribute attrib);

				String^ GetReportTitle();
				String^ GetReportDate();
				String^ GetProjectName();
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

			private: // -------------------------------------------------------
				ITASKLISTBASE* m_pTaskList;
				const ITASKLISTBASE* m_pConstTaskList;

			private: // -------------------------------------------------------
				TaskList();

			};
		}
	}
}
#pragma warning( pop )

