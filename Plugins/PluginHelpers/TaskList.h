#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

#include "Translator.h"

#include <Interfaces\ITasklist.h>

////////////////////////////////////////////////////////////////////////////////////////////////

class ITransText;

struct UITHEME;

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
			ref class TaskAttributeItem;

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
					FileLink,
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
					HtmlComments,
					ReferenceId,

					// new values here ONLY

				};

				// ----------------------------------------------------------------------------

			public:
				Task(ITaskList* pTaskList, HTASKITEM hTask);        // GET & SET
				Task(const ITaskList* pTaskList, HTASKITEM hTask);  // GET ONLY
				Task(const Task^ task);

				bool IsValid();
				bool IsAttributeAvailable(Attribute attrib);

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
				bool HasIcon();

				UInt32 GetID();
				UInt32 GetParentID();
				UInt32 GetReferenceID();
				UInt32 GetColor();
				UInt32 GetPosition();
				
				Int32 GetPriority(bool calculated);
				Int32 GetRisk(bool calculated);

				System::Drawing::Color GetTextDrawingColor();
				System::Windows::Media::Color GetTextMediaColor();
				System::Drawing::Color GetPriorityDrawingColor();
				System::Windows::Media::Color GetPriorityMediaColor();

				String^ GetTextWebColor();
				String^ GetPriorityWebColor();

				List<String^>^ GetAllocatedTo();
				List<String^>^ GetCategory();
				List<String^>^ GetTag();
				List<String^>^ GetDependency();
				List<UInt32>^  GetLocalDependency();
				List<String^>^ GetFileLink(bool fullPath);

				String^ FormatAllocatedTo(String^ delimiter);
				String^ FormatCategory(String^ delimiter);
				String^ FormatTag(String^ delimiter);
				String^ FormatDependency(String^ delimiter);
				String^ FormatFileLink(String^ delimiter, bool fullPath);

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

				bool IsDone();
				bool IsDue();
				bool IsGoodAsDone();
				bool HasSomeSubtasksDone();
				bool IsFlagged(bool calculated);
				bool IsParent();
				bool IsLocked(bool calculated);
				bool IsRecurring();
				bool IsReference();

				double GetTimeEstimate(TimeUnits% cUnits, bool calculated);
				double GetTimeSpent(TimeUnits% cUnits, bool calculated);

				String^ GetAttributeValue(Attribute attrib, bool calculated, bool display);
				bool HasAttribute(Attribute attrib);
				bool HasCalculatedAttribute(Attribute attrib);

				String^ GetCustomAttributeValue(String^ sID, bool display);
				Dictionary<String^, String^>^ GetCustomAttributeValues(bool display);
				String^ GetMetaDataValue(String^ sKey);

				// SETTERS ----------------------------------------------------

				Task^ NewSubtask(String^ sTitle);

				bool SetTitle(String^ sTitle);
				bool SetComments(String^ sComments);
				bool SetAllocatedBy(String^ sAllocBy);
				bool SetStatus(String^ sStatus);
				bool SetVersion(String^ sVersion);
				bool SetExternalID(String^ sExternalID);
				bool SetCreatedBy(String^ sCreatedBy);
				bool SetPosition(String^ sPosition);
				bool SetIcon(String^ sIcon);

				bool AddAllocatedTo(String^ sAllocTo);
				bool AddCategory(String^ sCategory);
				bool AddTag(String^ sTag);
				bool AddDependency(String^ sDependency);
				bool AddFileLink(String^ sFileLink);

				bool SetColor(UInt32 color);
				bool SetPriority(Byte nPriority);
				bool SetRisk(Byte Risk);

				bool SetPercentDone(Byte nPercent);
				bool SetCost(double dCost, bool isRate);
				bool SetFlag(bool bFlag);

				bool SetLastModified(DateTime^ dtLastMod);
				bool SetDoneDate(DateTime^ dtCompletion);
				bool SetDueDate(DateTime^ dtDue);
				bool SetStartDate(DateTime^ dtStart);
				bool SetCreationDate(DateTime^ dtCreation);

				bool SetTimeEstimate(double dTime, TimeUnits cUnits);
				bool SetTimeSpent(double dTime, TimeUnits cUnits);
				bool SetCustomAttributeValue(String^ sID, String^ sValue);
				bool ClearCustomAttributeValue(String^ sID);
				bool SetMetaDataValue(String^ sKey, String^ sValue);
				bool ClearMetaDataValue(String^ sKey);

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

			public ref class CustomAttributeDefinition
			{
			public:
				enum class Attribute
				{
					Unknown = -1,
					All = -1,

					String,
					Date,
					Integer,
					Decimal,
					Boolean,
					Icon,
					FileLink,
					TimePeriod,
					Fraction,
					Calculation,
				};

				enum class List
				{
					None = -1,

					AutoSingle,
					FixedSingle,
					AutoMulti,
					FixedMulti,
				};

			public:
				CustomAttributeDefinition();

			public:
				Attribute AttributeType;
				List ListType;

				String^ Id;
				String^ Label;
				String^ ListData; // \n delimited
			};

			////////////////////////////////////////////////////////////////////////////////////////////////

			public ref class TaskAttributeItem
			{
			public:
				TaskAttributeItem()
					:
					Label(String::Empty),
					AttributeId(Task::Attribute::Unknown),
					CustomAttributeId(String::Empty),
					CustomAttributeType(CustomAttributeDefinition::Attribute::Unknown)
				{
				}

			public:
				String^ Label;
				Task::Attribute AttributeId;
				String^ CustomAttributeId;
				CustomAttributeDefinition::Attribute CustomAttributeType;
			};

			////////////////////////////////////////////////////////////////////////////////////////////////

			public ref class TaskList
			{
			public:
				TaskList(ITaskList* pTaskList);        // GET & SET
				TaskList(const ITaskList* pTaskList);  // GET ONLY

				bool IsValid();
				bool IsAttributeAvailable(Task::Attribute attrib);

				List<Task::Attribute>^ GetAvailableAttributeIds();
				List<TaskAttributeItem^>^ GetAvailableAttributes();
				List<TaskAttributeItem^>^ GetAvailableAttributes(Translator^ trans);

				String^ GetReportTitle();
				String^ GetReportDate();
				String^ GetProjectName();
				String^ GetFilePath();
				String^ GetMetaData(String^ sKey);

				bool HasCustomAttributes();
				List<CustomAttributeDefinition^>^ GetCustomAttributes();
				List<CustomAttributeDefinition^>^ GetCustomAttributes(CustomAttributeDefinition::Attribute attribType);

				UInt32 GetTaskCount();
				bool HasTask(UInt32 dwTaskID);
				Task^ FindTask(UInt32 dwTaskID);

				Task^ GetFirstTask();
				Task^ NewTask(String^ sTitle);

				bool AddCustomAttribute(String^ sID, String^ sLabel, String^ sColumn);
				bool SetMetaData(String^ sKey, String^ sValue);
				bool ClearMetaData(String^ sKey);
				
				static String^ GetAttributeName(Task::Attribute attrib);
				static String^ GetAttributeName(Task::Attribute attrib, Translator^ trans);
				
			private: // -------------------------------------------------------
				ITASKLISTBASE* m_pTaskList;
				const ITASKLISTBASE* m_pConstTaskList;

			private: // -------------------------------------------------------
				TaskList();

				UInt32  GetCustomAttributeCount();
				String^ GetCustomAttributeLabel(int nIndex);
				String^ GetCustomAttributeID(int nIndex);
				String^ GetCustomAttributeListData(int nIndex);
				bool IsCustomAttributeEnabled(int nIndex);
				String^ GetCustomAttributeValue(int nIndex, String^ sItem);

				CustomAttributeDefinition::Attribute GetCustomAttributeType(int nIndex);
				CustomAttributeDefinition::List GetCustomAttributeListType(int nIndex);
			};

			////////////////////////////////////////////////////////////////////////////////////////////////

			public ref class MultiTaskList
			{
			public:
				MultiTaskList(const IMultiTaskList* pMultiTaskList);  // GET ONLY

				String^ GetReportTitle();
				String^ GetReportDate();
				List<TaskList^>^ GetTaskLists();

			private:
				const IMultiTaskList* m_pMultiTaskList;
			};
		}
	}
}
#pragma warning( pop )

