// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "UIExtension.h"

#include "..\..\ToDoList_Dev\Interfaces\UITheme.h"
#include "..\..\ToDoList_Dev\Interfaces\IUIExtension.h"
#include "..\..\ToDoList_Dev\Interfaces\ITasklist.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

UIExtension::TaskAttribute UIExtension::Map(IUI_ATTRIBUTE attrib)
{
	switch (attrib)
	{
	case IUI_TASKNAME:		return UIExtension::TaskAttribute::Title;
	case IUI_DONEDATE:		return UIExtension::TaskAttribute::DoneDate;
	case IUI_DUEDATE:		return UIExtension::TaskAttribute::DueDate;
	case IUI_STARTDATE:		return UIExtension::TaskAttribute::StartDate;
	case IUI_PRIORITY:		return UIExtension::TaskAttribute::Priority;
	case IUI_COLOR:			return UIExtension::TaskAttribute::Color;
	case IUI_ALLOCTO:		return UIExtension::TaskAttribute::AllocBy;
	case IUI_ALLOCBY:		return UIExtension::TaskAttribute::AllocTo;
	case IUI_STATUS:		return UIExtension::TaskAttribute::Status;
	case IUI_CATEGORY:		return UIExtension::TaskAttribute::Category;
	case IUI_PERCENT:		return UIExtension::TaskAttribute::Percent;
	case IUI_TIMEEST:		return UIExtension::TaskAttribute::TimeEstimate;
	case IUI_TIMESPENT:		return UIExtension::TaskAttribute::TimeSpent;
	case IUI_FILEREF:		return UIExtension::TaskAttribute::FileReference;
	case IUI_COMMENTS:		return UIExtension::TaskAttribute::Comments;
	case IUI_FLAG:			return UIExtension::TaskAttribute::Flag;
	case IUI_CREATIONDATE:	return UIExtension::TaskAttribute::CreationDate;
	case IUI_CREATEDBY:		return UIExtension::TaskAttribute::CreatedBy;
	case IUI_RISK:			return UIExtension::TaskAttribute::Risk;			
	case IUI_EXTERNALID:	return UIExtension::TaskAttribute::ExternalId;	
	case IUI_COST:			return UIExtension::TaskAttribute::Cost;			
	case IUI_DEPENDENCY:	return UIExtension::TaskAttribute::Dependency;	
	case IUI_RECURRENCE:	return UIExtension::TaskAttribute::Recurrence;	
	case IUI_VERSION:		return UIExtension::TaskAttribute::Version;		
	case IUI_POSITION:		return UIExtension::TaskAttribute::Position;
	case IUI_ID:			return UIExtension::TaskAttribute::Id;
	case IUI_LASTMOD:		return UIExtension::TaskAttribute::LastModified;
	case IUI_ICON:			return UIExtension::TaskAttribute::Icon;
	case IUI_TAGS:			return UIExtension::TaskAttribute::Tag;
	case IUI_CUSTOMATTRIB:	return UIExtension::TaskAttribute::CustomAttribute;
	case IUI_OFFSETTASK:	return UIExtension::TaskAttribute::OffsetTask;
	//  case IUI_
	}

	return UIExtension::TaskAttribute::Unknown;
}

IUI_ATTRIBUTE UIExtension::Map(UIExtension::TaskAttribute attrib)
{
	switch (attrib)
	{
	case UIExtension::TaskAttribute::Title:				return IUI_TASKNAME;	
	case UIExtension::TaskAttribute::DoneDate:			return IUI_DONEDATE;	
	case UIExtension::TaskAttribute::DueDate:			return IUI_DUEDATE;		
	case UIExtension::TaskAttribute::StartDate:			return IUI_STARTDATE;	
	case UIExtension::TaskAttribute::Priority:			return IUI_PRIORITY;	
	case UIExtension::TaskAttribute::Color:				return IUI_COLOR;		
	case UIExtension::TaskAttribute::AllocBy:			return IUI_ALLOCTO;		
	case UIExtension::TaskAttribute::AllocTo:			return IUI_ALLOCBY;		
	case UIExtension::TaskAttribute::Status:				return IUI_STATUS;		
	case UIExtension::TaskAttribute::Category:			return IUI_CATEGORY;	
	case UIExtension::TaskAttribute::Percent:			return IUI_PERCENT;		
	case UIExtension::TaskAttribute::TimeEstimate:		return IUI_TIMEEST;		
	case UIExtension::TaskAttribute::TimeSpent:			return IUI_TIMESPENT;	
	case UIExtension::TaskAttribute::FileReference:		return IUI_FILEREF;		
	case UIExtension::TaskAttribute::Comments:			return IUI_COMMENTS;	
	case UIExtension::TaskAttribute::Flag:				return IUI_FLAG;		
	case UIExtension::TaskAttribute::CreationDate:		return IUI_CREATIONDATE;
	case UIExtension::TaskAttribute::CreatedBy:			return IUI_CREATEDBY;	
	case UIExtension::TaskAttribute::Risk:				return IUI_RISK;		
	case UIExtension::TaskAttribute::ExternalId:			return IUI_EXTERNALID;	
	case UIExtension::TaskAttribute::Cost:				return IUI_COST;		
	case UIExtension::TaskAttribute::Dependency:			return IUI_DEPENDENCY;	
	case UIExtension::TaskAttribute::Recurrence:			return IUI_RECURRENCE;	
	case UIExtension::TaskAttribute::Version:			return IUI_VERSION;		
	case UIExtension::TaskAttribute::Position:			return IUI_POSITION;	
	case UIExtension::TaskAttribute::Id:					return IUI_ID;			
	case UIExtension::TaskAttribute::LastModified:		return IUI_LASTMOD;		
	case UIExtension::TaskAttribute::Icon:				return IUI_ICON;		
	case UIExtension::TaskAttribute::Tag:				return IUI_TAGS;		
	case UIExtension::TaskAttribute::CustomAttribute:	return IUI_CUSTOMATTRIB;
	case UIExtension::TaskAttribute::OffsetTask:			return IUI_OFFSETTASK;	
		//  case IUI_
	}

	return IUI_NONE;
}

Collections::Generic::HashSet<UIExtension::TaskAttribute>^ UIExtension::Map(const IUI_ATTRIBUTE* pAttrib, int numAttrib)
{
	Collections::Generic::HashSet<TaskAttribute>^ attribs = gcnew(Collections::Generic::HashSet<TaskAttribute>);

	for (int attrib = 0; attrib < numAttrib; attrib++)
		attribs->Add(Map(pAttrib[attrib]));

	return attribs;
}

UIExtension::UpdateType UIExtension::Map(IUI_UPDATETYPE type)
{
	switch (type)
	{
	case IUI_EDIT:		return UIExtension::UpdateType::Edit;
	case IUI_NEW:		return UIExtension::UpdateType::New;
	case IUI_DELETE:	return UIExtension::UpdateType::Delete;
	case IUI_MOVE:		return UIExtension::UpdateType::Move;
	case IUI_ALL:		return UIExtension::UpdateType::All;
	//  case IUI_
	}

	return UIExtension::UpdateType::Unknown;
}

UIExtension::AppCommand UIExtension::Map(IUI_APPCOMMAND cmd)
{
	switch (cmd)
	{
	case IUI_EXPANDALL:				return UIExtension::AppCommand::ExpandAll;
	case IUI_COLLAPSEALL:			return UIExtension::AppCommand::CollapseAll;
	case IUI_EXPANDSELECTED:		return UIExtension::AppCommand::ExpandSelected;
	case IUI_COLLAPSESELECTED:		return UIExtension::AppCommand::CollapseSelected;
	case IUI_SORT:					return UIExtension::AppCommand::Sort;
	case IUI_TOGGLABLESORT:			return UIExtension::AppCommand::ToggleSort;
	case IUI_SETFOCUS:				return UIExtension::AppCommand::SetFocus;
	case IUI_RESIZEATTRIBCOLUMNS:	return UIExtension::AppCommand::ResizeAttributeColumns;
	case IUI_SELECTTASK:			return UIExtension::AppCommand::SelectTask;
	//  case IUI_
	}

	return UIExtension::AppCommand::Unknown;
}

IUI_HITTEST UIExtension::Map(UIExtension::HitResult test)
{
	switch (test)
	{
	case UIExtension::HitResult::Nowhere:		return IUI_NOWHERE;
	case UIExtension::HitResult::Tasklist:		return IUI_TASKLIST;
	case UIExtension::HitResult::ColumnHeader:	return IUI_COLUMNHEADER;
	case UIExtension::HitResult::Task:			return IUI_TASK;
	}

	return IUI_NOWHERE;
}

////////////////////////////////////////////////////////////////////////////////////////////////

UIExtension::ParentNotify::ParentNotify(IntPtr hwndParent) : m_hwndParent(NULL)
{
	m_hwndParent = static_cast<HWND>(hwndParent.ToPointer());
}

bool UIExtension::ParentNotify::NotifyMod(UIExtension::TaskAttribute nAttribute, DateTime date)
{
	IUITASKMOD mod = { UIExtension::Map(nAttribute), 0 };
	mod.tValue = static_cast<__int64>(Task::Map(date));
	
	return DoNotify(&mod, 1);
}

bool UIExtension::ParentNotify::NotifyMod(UIExtension::TaskAttribute nAttribute, double value)
{
	IUITASKMOD mod = { UIExtension::Map(nAttribute), 0 };
	mod.dValue = value;

	return DoNotify(&mod, 1);
}

bool UIExtension::ParentNotify::NotifyMod(UIExtension::TaskAttribute nAttribute, double time, Task::TimeUnits units)
{
	IUITASKMOD mod = { UIExtension::Map(nAttribute), 0 };

	mod.dValue = time;
	mod.nTimeUnits = Task::Map(units);

	return DoNotify(&mod, 1);
}

bool UIExtension::ParentNotify::NotifyMod(UIExtension::TaskAttribute nAttribute, int value)
{
	IUITASKMOD mod = { UIExtension::Map(nAttribute), 0 };
	mod.nValue = value;

	return DoNotify(&mod, 1);
}

bool UIExtension::ParentNotify::NotifyMod(UIExtension::TaskAttribute nAttribute, bool value)
{
	IUITASKMOD mod = { UIExtension::Map(nAttribute), 0 };
	mod.bValue = (value ? TRUE : FALSE);

	return DoNotify(&mod, 1);
}

bool UIExtension::ParentNotify::NotifyMod(String^ sCustAttribID, String^ value)
{
	IUITASKMOD mod = { IUI_CUSTOMATTRIB, 0 };

	mod.szValue = MS(value);
	mod.szCustomAttribID = MS(sCustAttribID);

	return DoNotify(&mod, 1);
}

bool UIExtension::ParentNotify::NotifyMod(UIExtension::TaskAttribute nAttribute, String^ value)
{
	IUITASKMOD mod = { UIExtension::Map(nAttribute), 0 };
	mod.szValue = MS(value);

	return DoNotify(&mod, 1);
}

bool UIExtension::ParentNotify::NotifyMod(UIExtension::TaskAttribute nAttribute, cli::array<String^>^ aValues)
{
	IUITASKMOD mod = { UIExtension::Map(nAttribute), 0 };
	//mod.szValue = MS(value);

	return DoNotify(&mod, 1);
}

bool UIExtension::ParentNotify::DoNotify(const IUITASKMOD* pMod, int numMod)
{
	if (!IsWindow(m_hwndParent))
		return false;

	BOOL bRet = ::SendMessage(m_hwndParent, WM_IUI_MODIFYSELECTEDTASK, numMod, (LPARAM)pMod);

	return (bRet != FALSE);
}

bool UIExtension::ParentNotify::NotifySelChange(UInt32 taskID)
{
	if (!IsWindow(m_hwndParent))
		return false;

	BOOL bRet = ::SendMessage(m_hwndParent, WM_IUI_SELECTTASK, 0, taskID);

	return (bRet != FALSE);
}

bool UIExtension::ParentNotify::NotifySelChange(cli::array<UInt32>^ pdwTaskIDs)
{
	if (!IsWindow(m_hwndParent) || !pdwTaskIDs->Length)
		return false;

	pin_ptr<UInt32> p = &pdwTaskIDs[0];
	BOOL bRet = ::SendMessage(m_hwndParent, WM_IUI_SELECTTASK, pdwTaskIDs->Length, (LPARAM)p);

	return (bRet != FALSE);
}

////////////////////////////////////////////////////////////////////////////////////////////////

UIExtension::TaskIcon::TaskIcon(IntPtr hwndParent) : m_hwndParent(NULL), m_hilTaskImages(NULL), m_iImage(-1)
{
	m_hwndParent = static_cast<HWND>(hwndParent.ToPointer());
}

bool UIExtension::TaskIcon::Get(UInt32 dwTaskID)
{
	pin_ptr<int> p = &m_iImage;
	m_hilTaskImages = (HIMAGELIST)::SendMessage(m_hwndParent, WM_IUI_GETTASKICON, dwTaskID, (LPARAM)p);

	if ((m_hilTaskImages != NULL) && (m_iImage != -1))
		return true;

	m_hilTaskImages = NULL;
	m_iImage = -1;

	return false;
}

bool UIExtension::TaskIcon::Draw(System::Drawing::Graphics^ dc, Int32 x, Int32 y)
{
	if ((m_hilTaskImages == NULL) || (m_iImage == -1))
		return false;

	HDC hDC = static_cast<HDC>(dc->GetHdc().ToPointer());

	if (hDC == NULL)
		return false;

	bool bRes = (ImageList_Draw(m_hilTaskImages, m_iImage, hDC, x, y, ILD_TRANSPARENT) != FALSE);

	dc->ReleaseHdc();

	return bRes;
}

////////////////////////////////////////////////////////////////////////////////////////////////

