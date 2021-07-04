// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "TaskList.h"
#include "UIExtension.h"
#include "ColorUtil.h"

#include <3rdParty\T64Utils.h>

#include <Interfaces\IEnums.h>
#include <Interfaces\TasklistSchemaDef.h>

#include <OleAuto.h>
#include <time.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

TaskList::TaskList(ITaskList* pTaskList) 
	: 
	m_pTaskList(GetITLInterface<ITASKLISTBASE>(pTaskList, IID_TASKLISTBASE)), 
	m_pConstTaskList(nullptr) 
{
} 

TaskList::TaskList(const ITaskList* pTaskList) 
	: 
	m_pTaskList(nullptr), 
	m_pConstTaskList(GetITLInterface<ITASKLISTBASE>(pTaskList, IID_TASKLISTBASE)) 
{
} 

// private constructor
TaskList::TaskList() : m_pTaskList(nullptr), m_pConstTaskList(nullptr)
{

}

bool TaskList::IsValid()
{
	return (m_pConstTaskList || m_pTaskList);
}

bool TaskList::IsAttributeAvailable(Task::Attribute attrib)
{
	TDC_ATTRIBUTE nAttrib = Task::MapAttribute(attrib);

	return (m_pConstTaskList ? m_pConstTaskList->IsAttributeAvailable(nAttrib) :
		(m_pTaskList ? m_pTaskList->IsAttributeAvailable(nAttrib) : false));
}

List<Task::Attribute>^ TaskList::GetAvailableAttributes()
{
	auto attribs = gcnew List<Task::Attribute>();
	auto enumValues = System::Enum::GetValues(Task::Attribute::typeid);

	for each (auto enumVal in enumValues)
	{
		auto attrib = Task::Attribute(enumVal);

		switch (attrib)
		{
		case Task::Attribute::Unknown:
			continue;

		case Task::Attribute::OffsetTask:
			continue;
		}

		if (IsAttributeAvailable(attrib))
			attribs->Add(attrib);
	}

	return attribs;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// GETTERS

#define GETVAL(fn, errret) \
	(m_pConstTaskList ? m_pConstTaskList->fn() : (m_pTaskList ? m_pTaskList->fn() : errret))

#define GETSTR(fn) \
	gcnew String(m_pConstTaskList ? m_pConstTaskList->fn() : (m_pTaskList ? m_pTaskList->fn() : L""))

#define GETVAL_ARG(fn, arg, errret) \
	(m_pConstTaskList ? m_pConstTaskList->fn(arg) : (m_pTaskList ? m_pTaskList->fn(arg) : errret))

#define GETSTR_ARG(fn, arg) \
	gcnew String(m_pConstTaskList ? m_pConstTaskList->fn(arg) : (m_pTaskList ? m_pTaskList->fn(arg) : L""))

// -------------------------------------------------------------

String^ TaskList::GetReportTitle()
{
	return GETSTR(GetReportTitle);
}

String^ TaskList::GetReportDate()
{
	return GETSTR(GetReportDate);
}

String^ TaskList::GetProjectName()
{
	return GETSTR(GetProjectName);
}

String^ TaskList::GetMetaData(String^ sKey)
{
	return GETSTR_ARG(GetMetaData, MS(sKey));
}

Boolean TaskList::HasCustomAttributes()
{
	return (GetCustomAttributeCount() > 0);
}

List<CustomAttributeDefinition^>^ TaskList::GetCustomAttributes()
{
	return GetCustomAttributes(CustomAttributeDefinition::Attribute::All);
}

List<CustomAttributeDefinition^>^ TaskList::GetCustomAttributes(CustomAttributeDefinition::Attribute filter)
{
	auto attribs = gcnew List<CustomAttributeDefinition^>();

	int numAttrib = GetCustomAttributeCount();

	for (int i = 0; i < numAttrib; i++)
	{
		if (!IsCustomAttributeEnabled(i))
			continue;

		CustomAttributeDefinition::Attribute attribType = GetCustomAttributeType(i);

		if (attribType == CustomAttributeDefinition::Attribute::Unknown)
			continue;

		if ((filter == CustomAttributeDefinition::Attribute::All) || (attribType == filter))
		{
			auto attrib = gcnew CustomAttributeDefinition();

			attrib->AttributeType = attribType;
			attrib->Id = GetCustomAttributeID(i);
			attrib->Label = GetCustomAttributeLabel(i);
			attrib->ListData = GetCustomAttributeListData(i);
			attrib->ListType = GetCustomAttributeListType(i);

			attribs->Add(attrib);
		}
	}

	return attribs;
}

CustomAttributeDefinition::Attribute TaskList::GetCustomAttributeType(int nIndex)
{
	UInt32 types = GETVAL_ARG(GetCustomAttributeType, nIndex, 0);

	switch (types & TDCCA_DATAMASK)
	{
	case TDCCA_STRING:		return CustomAttributeDefinition::Attribute::String;
	case TDCCA_DATE:		return CustomAttributeDefinition::Attribute::Date;
	case TDCCA_INTEGER:		return CustomAttributeDefinition::Attribute::Integer;
	case TDCCA_DOUBLE:		return CustomAttributeDefinition::Attribute::Decimal;
	case TDCCA_BOOL:		return CustomAttributeDefinition::Attribute::Boolean;
	case TDCCA_ICON:		return CustomAttributeDefinition::Attribute::Icon;
	case TDCCA_FILELINK:	return CustomAttributeDefinition::Attribute::FileLink;
	case TDCCA_TIMEPERIOD:	return CustomAttributeDefinition::Attribute::TimePeriod;
	case TDCCA_FRACTION:	return CustomAttributeDefinition::Attribute::Fraction;
	}

	return CustomAttributeDefinition::Attribute::Unknown;
}

CustomAttributeDefinition::List TaskList::GetCustomAttributeListType(int nIndex)
{
	UInt32 types = GETVAL_ARG(GetCustomAttributeType, nIndex, 0);

	switch (types & TDCCA_LISTMASK)
	{
	case TDCCA_AUTOLIST:		return CustomAttributeDefinition::List::AutoSingle;
	case TDCCA_FIXEDLIST:		return CustomAttributeDefinition::List::FixedSingle;
	case TDCCA_AUTOMULTILIST:	return CustomAttributeDefinition::List::AutoMulti;
	case TDCCA_FIXEDMULTILIST:	return CustomAttributeDefinition::List::FixedMulti;
	}

	return CustomAttributeDefinition::List::None;
}

UInt32 TaskList::GetCustomAttributeCount()
{
	return GETVAL(GetCustomAttributeCount, 0);
}

String^ TaskList::GetCustomAttributeLabel(int nIndex)
{
	return GETSTR_ARG(GetCustomAttributeLabel, nIndex);
}

String^ TaskList::GetCustomAttributeID(int nIndex)
{
	return GETSTR_ARG(GetCustomAttributeID, nIndex);
}

String^ TaskList::GetCustomAttributeListData(int nIndex)
{
	return GETSTR_ARG(GetCustomAttributeListData, nIndex);
}

Boolean TaskList::IsCustomAttributeEnabled(int nIndex)
{
	return GETVAL_ARG(IsCustomAttributeEnabled, nIndex, false);
}

String^ TaskList::GetCustomAttributeValue(int nIndex, String^ sItem)
{
	LPCWSTR szValue = (m_pConstTaskList ? m_pConstTaskList->GetCustomAttributeValue(nIndex, MS(sItem)) : 
						(m_pTaskList ? m_pTaskList->GetCustomAttributeValue(nIndex, MS(sItem)) : L""));

	return gcnew String(szValue);
}

UInt32 TaskList::GetTaskCount()
{
	return GETVAL(GetTaskCount, 0);
}

Boolean TaskList::HasTask(UInt32 dwTaskID)
{
	HTASKITEM hTask = GETVAL_ARG(FindTask, dwTaskID, NULL);

	return (hTask != NULL);
}

Task^ TaskList::FindTask(UInt32 dwTaskID)
{
	HTASKITEM hTask = GETVAL_ARG(FindTask, dwTaskID, NULL);

	return gcnew Task((m_pConstTaskList ? m_pConstTaskList : m_pTaskList), hTask);
}

Task^ TaskList::GetFirstTask()
{
	if (m_pConstTaskList)
		return gcnew Task(m_pConstTaskList, m_pConstTaskList->GetFirstTask(nullptr));

	// else
	return gcnew Task(m_pTaskList, m_pTaskList->GetFirstTask(nullptr));
}

String^ TaskList::GetAttributeName(Task::Attribute attrib)
{
	switch (attrib)
	{
	case Task::Attribute::AllocatedBy:       return L"Allocated By";        
	case Task::Attribute::AllocatedTo:       return L"Allocated To";        
	case Task::Attribute::Category:			 return L"Category";            
	case Task::Attribute::Color:			 return L"Colour";  
	case Task::Attribute::Comments:          return L"Comments";  
	case Task::Attribute::Cost:              return L"Cost";                
	case Task::Attribute::CreatedBy:         return L"Created By";          
	case Task::Attribute::CreationDate:      return L"Creation Date";       
	case Task::Attribute::Dependency:        return L"Dependency";          
	case Task::Attribute::DoneDate:          return L"Completion Date";     
	case Task::Attribute::DueDate:           return L"Due Date";            
	case Task::Attribute::ExternalId:        return L"External ID";         
	case Task::Attribute::FileLink:			 return L"File Link";           
	case Task::Attribute::Flag:              return L"Flag";                
	case Task::Attribute::HtmlComments:      return L"Comments";            
	case Task::Attribute::Icon:              return L"Icon";  
	case Task::Attribute::Id:                return L"Task ID";             
	case Task::Attribute::LastModifiedBy:    return L"Last Modified By";    
	case Task::Attribute::LastModifiedDate:  return L"Last Modified Date";  
	case Task::Attribute::ParentId:          return L"Parent Task ID";      
	case Task::Attribute::Path:              return L"Path";                
	case Task::Attribute::Percent:           return L"Percent Done";        
	case Task::Attribute::Position:          return L"Position";            
	case Task::Attribute::Priority:          return L"Priority";            
	case Task::Attribute::ProjectName:       return L"Project Name";  
	case Task::Attribute::Recurrence:        return L"Recurrence";          
	case Task::Attribute::Risk:              return L"Risk";                
	case Task::Attribute::StartDate:         return L"Start Date";          
	case Task::Attribute::Status:            return L"Status";              
	case Task::Attribute::SubtaskDone:       return L"Subtask Completion";  
	case Task::Attribute::Tags:              return L"Tags";                
	case Task::Attribute::TimeEstimate:      return L"Time Estimate";       
	case Task::Attribute::TimeSpent:         return L"Time Spent";          
	case Task::Attribute::Title:             return L"Title";               
	case Task::Attribute::Version:           return L"Version";             

	case Task::Attribute::CustomAttribute:   break;
	case Task::Attribute::OffsetTask:        break;
	case Task::Attribute::MetaData:          break;
	}

	return L"";
}

/////////////////////////////////////////////////////////////////////////////////////////////

CustomAttributeDefinition::CustomAttributeDefinition()
{
	AttributeType = Attribute::Unknown;
	ListType = List::None;

	Id = nullptr;
	Label = nullptr;
	ListData = nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////

#define GETTASKVAL(fn, errret) \
	(m_pConstTaskList ? m_pConstTaskList->fn(m_hTask) : (m_pTaskList ? m_pTaskList->fn(m_hTask) : errret))

#define GETTASKDATE(fn, errret) \
	(m_pConstTaskList ? m_pConstTaskList->fn(m_hTask, date) : (m_pTaskList ? m_pTaskList->fn(m_hTask, date) : errret))

#define GETTASKATTRIB(arg) \
	gcnew String(m_pConstTaskList ? ((ITaskList*)m_pConstTaskList)->GetTaskAttribute(m_hTask, arg) : (m_pTaskList ? ((ITaskList*)m_pTaskList)->GetTaskAttribute(m_hTask, arg) : L""))

#define GETTASKSTR(fn) \
	gcnew String(GETTASKVAL(fn, L""))

#define GETTASKVAL_ARG(fn, arg, errret) \
	(m_pConstTaskList ? m_pConstTaskList->fn(m_hTask, arg) : (m_pTaskList ? m_pTaskList->fn(m_hTask, arg) : errret))

#define GETTASKSTR_ARG(fn, arg) \
	gcnew String(GETTASKVAL_ARG(fn, arg, L""))

#define GETTASKDATE_ARG(fn, arg, errret) \
	(m_pConstTaskList ? m_pConstTaskList->fn(m_hTask, arg, date) : (m_pTaskList ? m_pTaskList->fn(m_hTask, arg, date) : errret))

// ---------------------------------------------------------

Task::Task()
	: 
m_pTaskList(nullptr), 
	m_pConstTaskList(nullptr), 
	m_hTask(nullptr)
{

}

Task::Task(ITaskList* pTaskList, HTASKITEM hTask) 
	: 
m_pTaskList(GetITLInterface<ITASKLISTBASE>(pTaskList, IID_TASKLISTBASE)), 
	m_pConstTaskList(nullptr), 
	m_hTask(hTask)
{

}

Task::Task(const ITaskList* pTaskList, HTASKITEM hTask)
	: 
m_pTaskList(nullptr), 
	m_pConstTaskList(GetITLInterface<ITASKLISTBASE>(pTaskList, IID_TASKLISTBASE)), 
	m_hTask(hTask)
{

}

Task::Task(const Task^ task)
{
	m_pTaskList = task->m_pTaskList;
	m_pConstTaskList = task->m_pConstTaskList;
	m_hTask = task->m_hTask;
}

bool Task::IsValid()
{
	return ((m_pConstTaskList || m_pTaskList) && (m_hTask != nullptr));
}

Boolean Task::IsAttributeAvailable(Attribute attrib)
{
	TDC_ATTRIBUTE nAttrib = MapAttribute(attrib);

	return (m_pConstTaskList ? m_pConstTaskList->IsAttributeAvailable(nAttrib) :
			(m_pTaskList ? m_pTaskList->IsAttributeAvailable(nAttrib) : false));
}

String^ Task::GetAttributeValue(Task::Attribute attrib, bool calculated, bool display)
{
	// special cases
	switch (attrib)
	{
	case Task::Attribute::ReferenceId:
		{
			UInt32 refID = GetReferenceID();

			if (refID > 0)
				return String::Format(L"{0}", refID);
		}
		break;

	case Task::Attribute::CustomAttribute:
	case Task::Attribute::OffsetTask:
	case Task::Attribute::MetaData:
		break;

	default:
		{
			TDC_ATTRIBUTE nAttrib = MapAttribute(attrib);

			if (m_pConstTaskList)
				return gcnew String(m_pConstTaskList->GetTaskAttribute(m_hTask, nAttrib, calculated, display));
	
			if (m_pTaskList)
				return gcnew String(m_pTaskList->GetTaskAttribute(m_hTask, nAttrib, calculated, display));
		}
		break;
	}

	return gcnew String(L"");
}

bool Task::HasAttribute(Task::Attribute attrib)
{
	TDC_ATTRIBUTE nAttrib = MapAttribute(attrib);

	return (m_pConstTaskList ? m_pConstTaskList->TaskHasAttribute(m_hTask, nAttrib) :
			(m_pTaskList ? m_pTaskList->TaskHasAttribute(m_hTask, nAttrib) : false));
}

Task^ Task::GetFirstSubtask()
{
	if (m_pConstTaskList)
		return gcnew Task(m_pConstTaskList, m_pConstTaskList->GetFirstTask(m_hTask));

	if (m_pTaskList)
		return gcnew Task(m_pTaskList, m_pTaskList->GetFirstTask(m_hTask));

	return nullptr;
}

Task^ Task::GetNextTask()
{
	if (m_pConstTaskList)
		return gcnew Task(m_pConstTaskList, m_pConstTaskList->GetNextTask(m_hTask));

	if (m_pTaskList)
		return gcnew Task(m_pTaskList, m_pTaskList->GetNextTask(m_hTask));

	return nullptr;
}

Task^ Task::GetParentTask()
{
	if (m_pConstTaskList)
		return gcnew Task(m_pConstTaskList, m_pConstTaskList->GetTaskParent(m_hTask));

	if (m_pTaskList)
		return gcnew Task(m_pTaskList, m_pTaskList->GetTaskParent(m_hTask));

	return nullptr;
}

String^ Task::GetTitle()
{
	return GETTASKSTR(GetTaskTitle);
}

String^ Task::GetComments()
{
	return GETTASKSTR(GetTaskComments);
}

String^ Task::GetHtmlComments()
{
	return GETTASKATTRIB(TDL_TASKHTMLCOMMENTS);
}

String^ Task::GetAllocatedBy()
{
	return GETTASKSTR(GetTaskAllocatedBy);
}

String^ Task::GetStatus()
{
	return GETTASKSTR(GetTaskStatus);
}

String^ Task::GetWebColor()
{
	return GETTASKSTR(GetTaskWebColor);
}

String^ Task::GetVersion()
{
	return GETTASKSTR(GetTaskVersion);
}

String^ Task::GetExternalID()
{
	return GETTASKSTR(GetTaskExternalID);
}

String^ Task::GetCreatedBy()
{
	return GETTASKSTR(GetTaskCreatedBy);
}

String^ Task::GetPositionString()
{
	return GETTASKSTR(GetTaskPositionString);
}

String^ Task::GetIcon()
{
	return GETTASKSTR(GetTaskIcon);
}

Boolean Task::HasIcon()
{
	LPCWSTR szIcon = GETTASKVAL(GetTaskIcon, NULL);
	return (szIcon && *szIcon);
}

UInt32 Task::GetID()
{
	return GETTASKVAL(GetTaskID, 0);
}

UInt32 Task::GetParentID()
{
	return GETTASKVAL(GetTaskParentID, 0);
}

UInt32 Task::GetReferenceID()
{
	return GETTASKVAL(GetTaskReferenceID, 0);
}

UInt32 Task::GetColor()
{
	return GETTASKVAL(GetTaskColor, 0);
}

System::Drawing::Color Task::GetTextDrawingColor()
{
	UInt32 color = GETTASKVAL(GetTaskTextColor, 0);

	if (color == 0)
		return System::Drawing::Color::Empty;

	// else
	return ColorUtil::DrawingColor::ToColor(color);
}

System::Windows::Media::Color Task::GetTextMediaColor()
{
	UInt32 color = GETTASKVAL(GetTaskTextColor, 0);

	if (color == 0)
		return System::Windows::Media::Color::FromArgb(0, 0, 0, 0);

	return ColorUtil::MediaColor::ToColor(color);
}

System::Drawing::Color Task::GetPriorityDrawingColor()
{
	UInt32 color = GETTASKVAL(GetTaskPriorityColor, 0);

	if (color == 0)
		return System::Drawing::Color::Empty;

	return ColorUtil::DrawingColor::ToColor(color);
}

System::Windows::Media::Color Task::GetPriorityMediaColor()
{
	UInt32 color = GETTASKVAL(GetTaskPriorityColor, 0);

	if (color == 0)
		return System::Windows::Media::Color::FromArgb(0, 0, 0, 0);

	return ColorUtil::MediaColor::ToColor(color);
}

String^ Task::GetTextForeWebColor()
{
	return GETTASKATTRIB(TDL_TASKTEXTWEBCOLOR);
}

String^ Task::GetTextBackWebColor()
{
	return GETTASKATTRIB(TDL_TASKBACKWEBCOLOR);
}

String^ Task::GetPriorityWebColor()
{
	return GETTASKSTR(GetTaskPriorityWebColor);
}

UInt32 Task::GetPosition()
{
	return GETTASKVAL(GetTaskPosition, 0);
}

String^ Task::GetPath(String^ delimiter)
{
	String^ path = nullptr;
	Task^ parentTask = GetParentTask();

	while (parentTask->IsValid())
	{
		if (String::IsNullOrEmpty(path))
			path = parentTask->GetTitle();
		else
			path = (parentTask->GetTitle() + delimiter + path);

		parentTask = parentTask->GetParentTask();
	}

	return (path ? path : "");
}

Int32 Task::GetPriority(bool calculated)
{
	return GETTASKVAL_ARG(GetTaskPriority, calculated, -2);
}

Int32 Task::GetRisk(bool calculated)
{
	return GETTASKVAL_ARG(GetTaskRisk, calculated, -2);
}

List<String^>^ Task::GetAllocatedTo()
{
	auto items = gcnew System::Collections::Generic::List<String^>;
	int numItems = GETTASKVAL(GetTaskAllocatedToCount, 0);

	for (int nIndex = 0; nIndex < numItems; nIndex++)
		items->Add(GETTASKSTR_ARG(GetTaskAllocatedTo, nIndex));

	return items;
}

List<String^>^ Task::GetCategory()
{
	auto items = gcnew System::Collections::Generic::List<String^>;
	int numItems = GETTASKVAL(GetTaskCategoryCount, 0);

	for (int nIndex = 0; nIndex < numItems; nIndex++)
		items->Add(GETTASKSTR_ARG(GetTaskCategory, nIndex));

	return items;
}

List<String^>^ Task::GetTag()
{
	auto items = gcnew System::Collections::Generic::List<String^>;
	int numItems = GETTASKVAL(GetTaskTagCount, 0);

	for (int nIndex = 0; nIndex < numItems; nIndex++)
		items->Add(GETTASKSTR_ARG(GetTaskTag, nIndex));

	return items;
}

List<String^>^ Task::GetDependency()
{
	auto items = gcnew System::Collections::Generic::List<String^>;
	int numItems = GETTASKVAL(GetTaskDependencyCount, 0);

	for (int nIndex = 0; nIndex < numItems; nIndex++)
		items->Add(GETTASKSTR_ARG(GetTaskDependency, nIndex));

	return items;
}

List<UInt32>^ Task::GetLocalDependency()
{
	auto depends = GetDependency();
	auto localDepends = gcnew System::Collections::Generic::List<UInt32>;

	for (int nDepend = 0; nDepend < depends->Count; nDepend++)
	{
		UInt32 localDepend = 0;

		if (UInt32::TryParse(depends[nDepend], localDepend) && (localDepend != 0))
			localDepends->Add(localDepend);
	}

	return localDepends;
}

List<String^>^ Task::GetFileLink()
{
	auto items = gcnew System::Collections::Generic::List<String^>;
	int numItems = GETTASKVAL(GetTaskFileLinkCount, 0);

	for (int nIndex = 0; nIndex < numItems; nIndex++)
		items->Add(GETTASKSTR_ARG(GetTaskFileLink, nIndex));

	return items;
}

String^ Task::FormatAllocatedTo(String^ delimiter)
{
	return FormatList(GetAllocatedTo(), delimiter);
}

String^ Task::FormatCategory(String^ delimiter)
{
	return FormatList(GetCategory(), delimiter);
}

String^ Task::FormatTag(String^ delimiter)
{
	return FormatList(GetTag(), delimiter);
}

String^ Task::FormatDependency(String^ delimiter)
{
	return FormatList(GetDependency(), delimiter);
}

String^ Task::FormatFileLink(String^ delimiter)
{
	return FormatList(GetFileLink(), delimiter);
}

String^ Task::FormatList(List<String^>^ items, String^ delimiter)
{
	return String::Join(delimiter, items);
}

Byte Task::GetPercentDone(bool calculated)
{
	return GETTASKVAL_ARG(GetTaskPercentDone, calculated, 0);
}

double Task::GetCost(bool calculated)
{
	bool unused = false;

	if (m_pConstTaskList)
		return m_pConstTaskList->GetTaskCost(m_hTask, calculated, unused);
	
	if (m_pTaskList)
		return m_pTaskList->GetTaskCost(m_hTask, calculated, unused);

	// all else
	return false;
}

DateTime Task::GetLastModifiedDate()
{
	__int64 date = 0;
	
	if (GETTASKDATE(GetTaskLastModified64, false))
		return MapDate(date);

	// else
	return DateTime::MinValue;
}

DateTime Task::GetDoneDate()
{
	__int64 date = 0;
	
	if (GETTASKDATE(GetTaskDoneDate64, false))
		return MapDate(date);

	// else
	return DateTime::MinValue;
}

DateTime Task::GetDueDate(bool calculated)
{
	__int64 date = 0;
	
	if (GETTASKDATE_ARG(GetTaskDueDate64, calculated, false))
		return MapDate(date);

	// else
	return DateTime::MinValue;
}

DateTime Task::GetStartDate(bool calculated)
{
	__int64 date = 0;
	
	if (GETTASKDATE_ARG(GetTaskStartDate64, calculated, false))
		return MapDate(date);

	// else
	return DateTime::MinValue;
}

DateTime Task::GetCreationDate()
{
	__int64 date = 0;
	
	if (GETTASKDATE(GetTaskCreationDate64, false))
		return MapDate(date);

	// else
	return DateTime::MinValue;
}

String^ Task::GetDoneDateString()
{
	return GETTASKSTR(GetTaskDoneDateString);
}

String^ Task::GetDueDateString(bool calculated)
{
	return GETTASKSTR_ARG(GetTaskDueDateString, calculated);
}

String^ Task::GetStartDateString(bool calculated)
{
	return GETTASKSTR_ARG(GetTaskStartDateString, calculated);
}

String^ Task::GetCreationDateString()
{
	return GETTASKSTR(GetTaskCreationDateString);
}

String^ Task::GetLastModifiedDateString()
{
	return "";//GETTASKSTR(GetTaskLastModifiedBy);
}

String^ Task::GetLastModifiedBy()
{
	return GETTASKSTR(GetTaskLastModifiedBy);
}

Boolean Task::IsDone()
{
	return GETTASKVAL(IsTaskDone, false);
}

Boolean Task::IsDue()
{
	return GETTASKVAL(IsTaskDue, false);
}

Boolean Task::IsGoodAsDone()
{
	return GETTASKVAL(IsTaskGoodAsDone, false);
}

Boolean Task::HasSomeSubtasksDone()
{
	LPCWSTR szSubtasks = GETTASKVAL(GetTaskSubtaskCompletion, NULL);
	return (szSubtasks && *szSubtasks && (szSubtasks[0] != '0'));
}

Boolean Task::IsFlagged(bool calculated)
{
	return GETTASKVAL_ARG(IsTaskFlagged, calculated, false);
}

Boolean Task::IsParent()
{
	return GETTASKVAL(IsTaskParent, false);
}

Boolean Task::IsLocked(bool calculated)
{
	return GETTASKVAL_ARG(IsTaskLocked, calculated, false);
}

// ---------------------------------------------------------

double Task::GetTimeEstimate(TimeUnits% cUnits, bool calculated)
{
	TDC_UNITS nUnits = TDCU_NULL;

	double dTime = (m_pConstTaskList ? m_pConstTaskList->GetTaskTimeEstimate(m_hTask, nUnits, calculated) :
		(m_pTaskList ? m_pTaskList->GetTaskTimeEstimate(m_hTask, nUnits, calculated) : 0.0));

	cUnits = MapUnits(nUnits);
	return dTime;
}

double Task::GetTimeSpent(TimeUnits% cUnits, bool calculated)
{
	TDC_UNITS nUnits = TDCU_NULL;

	double dTime = (m_pConstTaskList ? m_pConstTaskList->GetTaskTimeSpent(m_hTask, nUnits, calculated) :
		(m_pTaskList ? m_pTaskList->GetTaskTimeSpent(m_hTask, nUnits, calculated) : 0.0));

	cUnits = MapUnits(nUnits);
	return dTime;
}

Boolean Task::IsRecurring()
{
	return GETTASKVAL(IsTaskRecurring, false);
}

String^ Task::GetCustomAttributeValue(String^ sID, bool display)
{
	LPCWSTR szValue = (m_pConstTaskList ? m_pConstTaskList->GetTaskCustomAttributeData(m_hTask, MS(sID), display) :
						(m_pTaskList ? m_pTaskList->GetTaskCustomAttributeData(m_hTask, MS(sID), display) : L""));

	return gcnew String(szValue);
}

Dictionary<String^, String^>^ Task::GetCustomAttributeValues(bool display)
{
	auto values = gcnew Dictionary<String^, String^>();

	int nIndex = GETVAL(GetCustomAttributeCount, 0);

	while (nIndex--)
	{
		auto attribId = GETSTR_ARG(GetCustomAttributeID, nIndex);
		auto attribValue = GetCustomAttributeValue(attribId, display);

		if (attribValue->Length > 0)
			values->Add(attribId, attribValue);
	}

	return values;
}

String^ Task::GetMetaDataValue(String^ sKey)
{
	LPCWSTR szValue = (m_pConstTaskList ? m_pConstTaskList->GetTaskMetaData(m_hTask, MS(sKey)) : 
						(m_pTaskList ? m_pTaskList->GetTaskMetaData(m_hTask, MS(sKey)) : L""));

	return gcnew String(szValue);
}

// TODO

////////////////////////////////////////////////////////////////////////////////////////////////
// SETTERS

Task^ TaskList::NewTask(String^ sTitle)
{
	HTASKITEM hTask = (m_pTaskList ? m_pTaskList->NewTask(MS(sTitle), nullptr, 0) : nullptr);

	return gcnew Task(m_pTaskList, hTask);
}

Boolean TaskList::AddCustomAttribute(String^ sID, String^ sLabel, String^ sColumn)
{
	return (m_pTaskList ? m_pTaskList->AddCustomAttribute(MS(sID), MS(sLabel), MS(sColumn)) : false);
}

Boolean TaskList::SetMetaData(String^ sKey, String^ sValue)
{
	return (m_pTaskList ? m_pTaskList->SetMetaData(MS(sKey), MS(sValue)) : false);
}

Boolean TaskList::ClearMetaData(String^ sKey)
{
	return (m_pTaskList ? m_pTaskList->ClearMetaData(MS(sKey)) : false);
}

// ---------------------------------------------------------

#define SETTASKVAL(fn, val) \
	(m_pTaskList ? m_pTaskList->fn(m_hTask, val) : false)

#define SETTASKSTR(fn, str) \
	(m_pTaskList ? m_pTaskList->fn(m_hTask, MS(str)) : false)

// ---------------------------------------------------------

Task^ Task::NewSubtask(String^ sTitle)
{
	HTASKITEM hTask = (m_pTaskList ? m_pTaskList->NewTask(MS(sTitle), nullptr, 0) : nullptr);

	return gcnew Task(m_pTaskList, hTask);
}

Boolean Task::SetTitle(String^ sTitle)
{
	return SETTASKSTR(SetTaskTitle, sTitle);
}

Boolean Task::SetComments(String^ sComments)
{
	return SETTASKSTR(SetTaskComments, sComments);
}

Boolean Task::SetAllocatedBy(String^ sAllocBy)
{
	return SETTASKSTR(SetTaskAllocatedBy, sAllocBy);
}

Boolean Task::SetStatus(String^ sStatus)
{
	return SETTASKSTR(SetTaskStatus, sStatus);
}

Boolean Task::SetVersion(String^ sVersion)
{
	return SETTASKSTR(SetTaskVersion, sVersion);
}

Boolean Task::SetExternalID(String^ sExternalID)
{
	return SETTASKSTR(SetTaskExternalID, sExternalID);
}

Boolean Task::SetCreatedBy(String^ sCreatedBy)
{
	return SETTASKSTR(SetTaskCreatedBy, sCreatedBy);
}

Boolean Task::SetPosition(String^ sPosition)
{
	return SETTASKSTR(SetTaskPosition, sPosition);
}

Boolean Task::SetIcon(String^ sIcon)
{
	return SETTASKSTR(SetTaskIcon, sIcon);
}

Boolean Task::AddAllocatedTo(String^ sAllocTo)
{
	return SETTASKSTR(AddTaskAllocatedTo, sAllocTo);
}

Boolean Task::AddCategory(String^ sCategory)
{
	return SETTASKSTR(AddTaskCategory, sCategory);
}

Boolean Task::AddTag(String^ sTag)
{
	return SETTASKSTR(AddTaskTag, sTag);
}

Boolean Task::AddDependency(String^ sDependency)
{
	return SETTASKSTR(AddTaskDependency, sDependency);
}

Boolean Task::AddFileLink(String^ sFileLink)
{
	return SETTASKSTR(AddTaskFileLink, sFileLink);
}

Boolean Task::SetColor(UInt32 color)
{
	return SETTASKVAL(SetTaskColor, color);
}

Boolean Task::SetPriority(Byte nPriority)
{
	return SETTASKVAL(SetTaskPriority, nPriority);
}

Boolean Task::SetRisk(Byte Risk)
{
	return SETTASKVAL(SetTaskRisk, Risk);
}

Boolean Task::SetPercentDone(Byte nPercent)
{
	return SETTASKVAL(SetTaskPercentDone, nPercent);
}

Boolean Task::SetCost(double dCost, bool isRate)
{
	return (m_pTaskList ? m_pTaskList->SetTaskCost(m_hTask, dCost, isRate) : false);
}

Boolean Task::SetFlag(Boolean bFlag)
{
	return SETTASKVAL(SetTaskFlag, bFlag);
}

Boolean Task::SetLastModified(DateTime^ dtLastMod)
{
	return SETTASKVAL(SetTaskLastModified64, MapDate(dtLastMod));
}

Boolean Task::SetDoneDate(DateTime^ dtCompletion)
{
	return SETTASKVAL(SetTaskDoneDate64, MapDate(dtCompletion));
}

Boolean Task::SetDueDate(DateTime^ dtDue)
{
	return SETTASKVAL(SetTaskDueDate64, MapDate(dtDue));
}

Boolean Task::SetStartDate(DateTime^ dtStart)
{
	return SETTASKVAL(SetTaskStartDate64, MapDate(dtStart));
}

Boolean Task::SetCreationDate(DateTime^ dtCreation)
{
	return SETTASKVAL(SetTaskCreationDate64, MapDate(dtCreation));
}

Boolean Task::SetTimeEstimate(double dTime, TimeUnits cUnits)
{
	return (m_pTaskList ? m_pTaskList->SetTaskTimeEstimate(m_hTask, dTime, MapUnits(cUnits)) : false);
}

Boolean Task::SetTimeSpent(double dTime, TimeUnits cUnits)
{
	return (m_pTaskList ? m_pTaskList->SetTaskTimeSpent(m_hTask, dTime, MapUnits(cUnits)) : false);
}

Boolean Task::SetCustomAttributeValue(String^ sID, String^ sValue)
{
	return (m_pTaskList ? m_pTaskList->SetTaskCustomAttributeData(m_hTask, MS(sID), MS(sValue)) : false);
}

Boolean Task::ClearCustomAttributeValue(String^ sID)
{
	return (m_pTaskList ? m_pTaskList->ClearTaskCustomAttributeData(m_hTask, MS(sID)) : false);
}

Boolean Task::SetMetaDataValue(String^ sKey, String^ sValue)
{
	return (m_pTaskList ? m_pTaskList->SetTaskMetaData(m_hTask, MS(sKey), MS(sValue)) : false);
}

Boolean Task::ClearMetaDataValue(String^ sKey)
{
	return (m_pTaskList ? m_pTaskList->ClearTaskMetaData(m_hTask, MS(sKey)) : false);
}

Task::TimeUnits Task::MapUnits(TDC_UNITS units)
{
	switch (units)
	{
	case TDCU_MINS:		return TimeUnits::Minutes;
	case TDCU_HOURS:	return TimeUnits::Hours;
	case TDCU_DAYS:		return TimeUnits::Days;
	case TDCU_WEEKDAYS:	return TimeUnits::Weekdays;
	case TDCU_WEEKS:	return TimeUnits::Weeks;
	case TDCU_MONTHS:	return TimeUnits::Months;
	case TDCU_YEARS:	return TimeUnits::Years;
	}

	// default
	return TimeUnits::Unknown;
}

TDC_UNITS Task::MapUnits(TimeUnits units)
{
	switch (units)
	{
	case TimeUnits::Minutes:	return TDCU_MINS;
	case TimeUnits::Hours:		return TDCU_HOURS;
	case TimeUnits::Days:		return TDCU_DAYS;
	case TimeUnits::Weekdays:	return TDCU_WEEKDAYS;
	case TimeUnits::Weeks:		return TDCU_WEEKS;
	case TimeUnits::Months:		return TDCU_MONTHS;
	case TimeUnits::Years:		return TDCU_YEARS;
	}

	// default
	return TDCU_NULL;
}

DateTime Task::MapDate(Int64 tDate)
{
	SYSTEMTIME st = { 0 };
	T64Utils::T64ToSystemTime(&tDate, &st);

	DateTime^ date = gcnew DateTime(
		st.wYear,
		st.wMonth, 
		st.wDay,
		st.wHour,
		st.wMinute,
		st.wSecond,
		DateTimeKind::Local);
	
	return *date;
}

Int64 Task::MapDate(DateTime^ date)
{
	SYSTEMTIME st = { 0 };

	st.wYear = date->Year;
	st.wMonth = date->Month;
	st.wDay = date->Day; 
	st.wHour = date->Hour;
	st.wMinute = date->Minute;
	st.wSecond = date->Second; 

	Int64 tDate;
	T64Utils::SystemTimeToT64(&st, &tDate);

	return tDate;
}

Task::Attribute Task::MapAttribute(TDC_ATTRIBUTE attrib)
{
	switch (attrib)
	{
	case TDCA_TASKNAME:			return Task::Attribute::Title;
	case TDCA_DONEDATE:			return Task::Attribute::DoneDate;
	case TDCA_DUEDATE:			return Task::Attribute::DueDate;
	case TDCA_STARTDATE:		return Task::Attribute::StartDate;
	case TDCA_PRIORITY:			return Task::Attribute::Priority;
	case TDCA_COLOR:			return Task::Attribute::Color;
	case TDCA_ALLOCTO:			return Task::Attribute::AllocatedBy;
	case TDCA_ALLOCBY:			return Task::Attribute::AllocatedTo;
	case TDCA_STATUS:			return Task::Attribute::Status;
	case TDCA_CATEGORY:			return Task::Attribute::Category;
	case TDCA_PERCENT:			return Task::Attribute::Percent;
	case TDCA_TIMEESTIMATE:		return Task::Attribute::TimeEstimate;
	case TDCA_TIMESPENT:		return Task::Attribute::TimeSpent;
	case TDCA_FILELINK:			return Task::Attribute::FileLink;
	case TDCA_COMMENTS:			return Task::Attribute::Comments;
	case TDCA_FLAG:				return Task::Attribute::Flag;
	case TDCA_CREATIONDATE:		return Task::Attribute::CreationDate;
	case TDCA_CREATEDBY:		return Task::Attribute::CreatedBy;
	case TDCA_RISK:				return Task::Attribute::Risk;
	case TDCA_EXTERNALID:		return Task::Attribute::ExternalId;
	case TDCA_COST:				return Task::Attribute::Cost;
	case TDCA_DEPENDENCY:		return Task::Attribute::Dependency;
	case TDCA_RECURRENCE:		return Task::Attribute::Recurrence;
	case TDCA_VERSION:			return Task::Attribute::Version;
	case TDCA_POSITION:			return Task::Attribute::Position;
	case TDCA_ID:				return Task::Attribute::Id;
	case TDCA_LASTMODDATE:		return Task::Attribute::LastModifiedDate;
	case TDCA_ICON:				return Task::Attribute::Icon;
	case TDCA_TAGS:				return Task::Attribute::Tags;
	case TDCA_CUSTOMATTRIB:		return Task::Attribute::CustomAttribute;
	case TDCA_OFFSETTASK:		return Task::Attribute::OffsetTask;
	case TDCA_SUBTASKDONE:		return Task::Attribute::SubtaskDone;
	case TDCA_METADATA:			return Task::Attribute::MetaData;
	case TDCA_PROJECTNAME:		return Task::Attribute::ProjectName;
		//  case TDCA_
	}

	return Attribute::Unknown;
}

TDC_ATTRIBUTE Task::MapAttribute(Task::Attribute attrib)
{
	switch (attrib)
	{
	case Task::Attribute::AllocatedBy:		return TDCA_ALLOCBY;
	case Task::Attribute::AllocatedTo:		return TDCA_ALLOCTO;
	case Task::Attribute::Category:			return TDCA_CATEGORY;
	case Task::Attribute::Color:			return TDCA_COLOR;
	case Task::Attribute::Comments:			return TDCA_COMMENTS;
	case Task::Attribute::Cost:				return TDCA_COST;
	case Task::Attribute::CreatedBy:		return TDCA_CREATEDBY;
	case Task::Attribute::CreationDate:		return TDCA_CREATIONDATE;
	case Task::Attribute::CustomAttribute:	return TDCA_CUSTOMATTRIB;
	case Task::Attribute::Dependency:		return TDCA_DEPENDENCY;
	case Task::Attribute::DoneDate:			return TDCA_DONEDATE;
	case Task::Attribute::DueDate:			return TDCA_DUEDATE;
	case Task::Attribute::ExternalId:		return TDCA_EXTERNALID;
	case Task::Attribute::FileLink:			return TDCA_FILELINK;
	case Task::Attribute::Flag:				return TDCA_FLAG;
	case Task::Attribute::Icon:				return TDCA_ICON;
	case Task::Attribute::HtmlComments:		return TDCA_HTMLCOMMENTS;
	case Task::Attribute::Id:				return TDCA_ID;
	case Task::Attribute::LastModifiedBy:	return TDCA_LASTMODBY;
	case Task::Attribute::LastModifiedDate:	return TDCA_LASTMODDATE;
	case Task::Attribute::MetaData:			return TDCA_METADATA;
	case Task::Attribute::OffsetTask:		return TDCA_OFFSETTASK;
	case Task::Attribute::ParentId:			return TDCA_PARENTID;
	case Task::Attribute::Path:				return TDCA_PATH;
	case Task::Attribute::Percent:			return TDCA_PERCENT;
	case Task::Attribute::Position:			return TDCA_POSITION;
	case Task::Attribute::Priority:			return TDCA_PRIORITY;
	case Task::Attribute::ProjectName:		return TDCA_PROJECTNAME;
	case Task::Attribute::Recurrence:		return TDCA_RECURRENCE;
	case Task::Attribute::Risk:				return TDCA_RISK;
	case Task::Attribute::StartDate:		return TDCA_STARTDATE;
	case Task::Attribute::Status:			return TDCA_STATUS;
	case Task::Attribute::SubtaskDone:		return TDCA_SUBTASKDONE;
	case Task::Attribute::Tags:				return TDCA_TAGS;
	case Task::Attribute::TimeEstimate:		return TDCA_TIMEESTIMATE;
	case Task::Attribute::TimeSpent:		return TDCA_TIMESPENT;
	case Task::Attribute::Title:			return TDCA_TASKNAME;
	case Task::Attribute::Version:			return TDCA_VERSION;
		//  case IUI_
	}

	return TDCA_NONE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
