// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "TDLUIExtension.h"

#include "..\..\Interfaces\UITheme.h"
#include "..\..\Interfaces\IUIExtension.h"
#include "..\..\Interfaces\ITasklist.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace TDLPluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

TDLTheme::TDLTheme() : m_pTheme(nullptr)
{
	m_pTheme = new UITHEME;
	ZeroMemory(m_pTheme, sizeof(UITHEME));
}

TDLTheme::TDLTheme(const UITHEME* pTheme)
{
	m_pTheme = new UITHEME;
	ZeroMemory(m_pTheme, sizeof(UITHEME));

	if (pTheme)
		*m_pTheme = *pTheme;
}

TDLTheme::RenderStyle TDLTheme::GetRenderStyle()
{
	switch (m_pTheme->nRenderStyle)
	{
	case UIRS_GRADIENT:				return TDLTheme::RenderStyle::Gradient;
	case UIRS_GLASS:				return TDLTheme::RenderStyle::Glass;
	case UIRS_GLASSWITHGRADIENT:	return TDLTheme::RenderStyle::GlassWithGradient;
	}

	return TDLTheme::RenderStyle::Gradient;
}

Windows::Media::Color TDLTheme::GetAppColorAsMedia(AppColor color)
{
	return GetMediaColor(GetColor(color));
}

System::Drawing::Color TDLTheme::GetAppColorAsDrawing(AppColor color)
{
	return GetDrawingColor(GetColor(color));
}

String^ TDLTheme::GetToolBarImagePath()
{
	return gcnew String(m_pTheme->szToolbarImage);
}

Windows::Media::Color TDLTheme::GetToolbarTransparencyColorAsMedia()
{
	return GetMediaColor(m_pTheme->crToolbarTransparency);
}

Drawing::Color TDLTheme::GetToolbarTransparencyColorAsDrawing()
{
	return GetDrawingColor(m_pTheme->crToolbarTransparency);
}

UInt32 TDLTheme::GetColor(AppColor color)
{
	switch (color)
	{
	case TDLTheme::AppColor::AppBackDark:		return m_pTheme->crAppBackDark;
	case TDLTheme::AppColor::AppBackLight:		return m_pTheme->crAppBackLight;
	case TDLTheme::AppColor::AppLinesDark:		return m_pTheme->crAppLinesDark;
	case TDLTheme::AppColor::AppLinesLight:		return m_pTheme->crAppLinesLight;
	case TDLTheme::AppColor::AppText:			return m_pTheme->crAppText;
	case TDLTheme::AppColor::MenuBack:			return m_pTheme->crMenuBack;
	case TDLTheme::AppColor::ToolbarDark:		return m_pTheme->crToolbarDark;
	case TDLTheme::AppColor::ToolbarLight:		return m_pTheme->crToolbarLight;
	case TDLTheme::AppColor::StatusBarDark:		return m_pTheme->crStatusBarDark;
	case TDLTheme::AppColor::StatusBarLight:	return m_pTheme->crStatusBarLight;
	case TDLTheme::AppColor::StatusBarText:		return m_pTheme->crStatusBarText;
	}

	return 0; // black
}

Windows::Media::Color TDLTheme::GetMediaColor(UInt32 rgbColor)
{
	System::Windows::Media::Color^ color = 
      System::Windows::Media::Color::FromArgb(255, (Byte)GetRValue(rgbColor), (Byte)GetGValue(rgbColor), (Byte)GetBValue(rgbColor));

	return *color;
}

System::Drawing::Color TDLTheme::GetDrawingColor(UInt32 rgbColor)
{
	System::Drawing::Color^ color = 
		System::Drawing::Color::FromArgb(255, (Byte)GetRValue(rgbColor), (Byte)GetGValue(rgbColor), (Byte)GetBValue(rgbColor));

	return *color;
}

////////////////////////////////////////////////////////////////////////////////////////////////

TDLUIExtension::TaskAttribute TDLUIExtension::Map(IUI_ATTRIBUTE attrib)
{
	switch (attrib)
	{
	case IUI_TASKNAME:		return TDLUIExtension::TaskAttribute::Title;
	case IUI_DONEDATE:		return TDLUIExtension::TaskAttribute::DoneDate;
	case IUI_DUEDATE:		return TDLUIExtension::TaskAttribute::DueDate;
	case IUI_STARTDATE:		return TDLUIExtension::TaskAttribute::StartDate;
	case IUI_PRIORITY:		return TDLUIExtension::TaskAttribute::Priority;
	case IUI_COLOR:			return TDLUIExtension::TaskAttribute::Color;
	case IUI_ALLOCTO:		return TDLUIExtension::TaskAttribute::AllocBy;
	case IUI_ALLOCBY:		return TDLUIExtension::TaskAttribute::AllocTo;
	case IUI_STATUS:		return TDLUIExtension::TaskAttribute::Status;
	case IUI_CATEGORY:		return TDLUIExtension::TaskAttribute::Category;
	case IUI_PERCENT:		return TDLUIExtension::TaskAttribute::Percent;
	case IUI_TIMEEST:		return TDLUIExtension::TaskAttribute::TimeEstimate;
	case IUI_TIMESPENT:		return TDLUIExtension::TaskAttribute::TimeSpent;
	case IUI_FILEREF:		return TDLUIExtension::TaskAttribute::FileReference;
	case IUI_COMMENTS:		return TDLUIExtension::TaskAttribute::Comments;
	case IUI_FLAG:			return TDLUIExtension::TaskAttribute::Flag;
	case IUI_CREATIONDATE:	return TDLUIExtension::TaskAttribute::CreationDate;
	case IUI_CREATEDBY:		return TDLUIExtension::TaskAttribute::CreatedBy;
	case IUI_RISK:			return TDLUIExtension::TaskAttribute::Risk;			
	case IUI_EXTERNALID:	return TDLUIExtension::TaskAttribute::ExternalId;	
	case IUI_COST:			return TDLUIExtension::TaskAttribute::Cost;			
	case IUI_DEPENDENCY:	return TDLUIExtension::TaskAttribute::Dependency;	
	case IUI_RECURRENCE:	return TDLUIExtension::TaskAttribute::Recurrence;	
	case IUI_VERSION:		return TDLUIExtension::TaskAttribute::Version;		
	case IUI_POSITION:		return TDLUIExtension::TaskAttribute::Position;
	case IUI_ID:			return TDLUIExtension::TaskAttribute::Id;
	case IUI_LASTMOD:		return TDLUIExtension::TaskAttribute::LastModified;
	case IUI_ICON:			return TDLUIExtension::TaskAttribute::Icon;
	case IUI_TAGS:			return TDLUIExtension::TaskAttribute::Tag;
	case IUI_CUSTOMATTRIB:	return TDLUIExtension::TaskAttribute::CustomAttribute;
	case IUI_OFFSETTASK:	return TDLUIExtension::TaskAttribute::OffsetTask;
	//  case IUI_
	}

	return TDLUIExtension::TaskAttribute::Unknown;
}

IUI_ATTRIBUTE TDLUIExtension::Map(TDLUIExtension::TaskAttribute attrib)
{
	switch (attrib)
	{
	case TDLUIExtension::TaskAttribute::Title:				return IUI_TASKNAME;	
	case TDLUIExtension::TaskAttribute::DoneDate:			return IUI_DONEDATE;	
	case TDLUIExtension::TaskAttribute::DueDate:			return IUI_DUEDATE;		
	case TDLUIExtension::TaskAttribute::StartDate:			return IUI_STARTDATE;	
	case TDLUIExtension::TaskAttribute::Priority:			return IUI_PRIORITY;	
	case TDLUIExtension::TaskAttribute::Color:				return IUI_COLOR;		
	case TDLUIExtension::TaskAttribute::AllocBy:			return IUI_ALLOCTO;		
	case TDLUIExtension::TaskAttribute::AllocTo:			return IUI_ALLOCBY;		
	case TDLUIExtension::TaskAttribute::Status:				return IUI_STATUS;		
	case TDLUIExtension::TaskAttribute::Category:			return IUI_CATEGORY;	
	case TDLUIExtension::TaskAttribute::Percent:			return IUI_PERCENT;		
	case TDLUIExtension::TaskAttribute::TimeEstimate:		return IUI_TIMEEST;		
	case TDLUIExtension::TaskAttribute::TimeSpent:			return IUI_TIMESPENT;	
	case TDLUIExtension::TaskAttribute::FileReference:		return IUI_FILEREF;		
	case TDLUIExtension::TaskAttribute::Comments:			return IUI_COMMENTS;	
	case TDLUIExtension::TaskAttribute::Flag:				return IUI_FLAG;		
	case TDLUIExtension::TaskAttribute::CreationDate:		return IUI_CREATIONDATE;
	case TDLUIExtension::TaskAttribute::CreatedBy:			return IUI_CREATEDBY;	
	case TDLUIExtension::TaskAttribute::Risk:				return IUI_RISK;		
	case TDLUIExtension::TaskAttribute::ExternalId:			return IUI_EXTERNALID;	
	case TDLUIExtension::TaskAttribute::Cost:				return IUI_COST;		
	case TDLUIExtension::TaskAttribute::Dependency:			return IUI_DEPENDENCY;	
	case TDLUIExtension::TaskAttribute::Recurrence:			return IUI_RECURRENCE;	
	case TDLUIExtension::TaskAttribute::Version:			return IUI_VERSION;		
	case TDLUIExtension::TaskAttribute::Position:			return IUI_POSITION;	
	case TDLUIExtension::TaskAttribute::Id:					return IUI_ID;			
	case TDLUIExtension::TaskAttribute::LastModified:		return IUI_LASTMOD;		
	case TDLUIExtension::TaskAttribute::Icon:				return IUI_ICON;		
	case TDLUIExtension::TaskAttribute::Tag:				return IUI_TAGS;		
	case TDLUIExtension::TaskAttribute::CustomAttribute:	return IUI_CUSTOMATTRIB;
	case TDLUIExtension::TaskAttribute::OffsetTask:			return IUI_OFFSETTASK;	
		//  case IUI_
	}

	return IUI_NONE;
}

Collections::Generic::HashSet<TDLUIExtension::TaskAttribute>^ TDLUIExtension::Map(const IUI_ATTRIBUTE* pAttrib, int numAttrib)
{
	Collections::Generic::HashSet<TaskAttribute>^ attribs = gcnew(Collections::Generic::HashSet<TaskAttribute>);

	for (int attrib = 0; attrib < numAttrib; attrib++)
		attribs->Add(Map(pAttrib[attrib]));

	return attribs;
}

TDLUIExtension::UpdateType TDLUIExtension::Map(IUI_UPDATETYPE type)
{
	switch (type)
	{
	case IUI_EDIT:		return TDLUIExtension::UpdateType::Edit;
	case IUI_NEW:		return TDLUIExtension::UpdateType::New;
	case IUI_DELETE:	return TDLUIExtension::UpdateType::Delete;
	case IUI_MOVE:		return TDLUIExtension::UpdateType::Move;
	case IUI_ALL:		return TDLUIExtension::UpdateType::All;
	//  case IUI_
	}

	return TDLUIExtension::UpdateType::Unknown;
}

TDLUIExtension::AppCommand TDLUIExtension::Map(IUI_APPCOMMAND cmd)
{
	switch (cmd)
	{
	case IUI_EXPANDALL:				return TDLUIExtension::AppCommand::ExpandAll;
	case IUI_COLLAPSEALL:			return TDLUIExtension::AppCommand::CollapseAll;
	case IUI_EXPANDSELECTED:		return TDLUIExtension::AppCommand::ExpandSelected;
	case IUI_COLLAPSESELECTED:		return TDLUIExtension::AppCommand::CollapseSelected;
	case IUI_SORT:					return TDLUIExtension::AppCommand::Sort;
	case IUI_TOGGLABLESORT:			return TDLUIExtension::AppCommand::ToggleSort;
	case IUI_SETFOCUS:				return TDLUIExtension::AppCommand::SetFocus;
	case IUI_RESIZEATTRIBCOLUMNS:	return TDLUIExtension::AppCommand::ResizeAttributeColumns;
	//  case IUI_
	}

	return TDLUIExtension::AppCommand::Unknown;
}

IUI_HITTEST TDLUIExtension::Map(TDLUIExtension::HitResult test)
{
	switch (test)
	{
	case TDLUIExtension::HitResult::Nowhere:		return IUI_NOWHERE;
	case TDLUIExtension::HitResult::Tasklist:		return IUI_TASKLIST;
	case TDLUIExtension::HitResult::ColumnHeader:	return IUI_COLUMNHEADER;
	case TDLUIExtension::HitResult::Task:			return IUI_TASK;
	}

	return IUI_NOWHERE;
}

////////////////////////////////////////////////////////////////////////////////////////////////

TDLNotify::TDLNotify(IntPtr hwndParent) : m_hwndParent(NULL), m_hwndFrom(NULL)
{
	m_hwndParent = static_cast<HWND>(hwndParent.ToPointer());
}

TDLNotify::TDLNotify(IntPtr hwndParent, IntPtr hwndFrom) : m_hwndParent(NULL), m_hwndFrom(NULL)
{
	m_hwndParent = static_cast<HWND>(hwndParent.ToPointer());
	m_hwndFrom = static_cast<HWND>(hwndFrom.ToPointer());
}

bool TDLNotify::NotifyMod(TDLUIExtension::TaskAttribute nAttribute, DateTime date)
{
	IUITASKMOD mod = { TDLUIExtension::Map(nAttribute), 0 };
	mod.tValue = static_cast<__int64>(TDLTask::Map(date));
	
	return DoNotify(&mod, 1);
}

bool TDLNotify::NotifyMod(TDLUIExtension::TaskAttribute nAttribute, double value)
{
	IUITASKMOD mod = { TDLUIExtension::Map(nAttribute), 0 };
	mod.dValue = value;

	return DoNotify(&mod, 1);
}

bool TDLNotify::NotifyMod(TDLUIExtension::TaskAttribute nAttribute, double time, TDLTask::TimeUnits units)
{
	IUITASKMOD mod = { TDLUIExtension::Map(nAttribute), 0 };

	mod.dValue = time;
	mod.nTimeUnits = TDLTask::Map(units);

	return DoNotify(&mod, 1);
}

bool TDLNotify::NotifyMod(TDLUIExtension::TaskAttribute nAttribute, int value)
{
	IUITASKMOD mod = { TDLUIExtension::Map(nAttribute), 0 };
	mod.nValue = value;

	return DoNotify(&mod, 1);
}

bool TDLNotify::NotifyMod(TDLUIExtension::TaskAttribute nAttribute, bool value)
{
	IUITASKMOD mod = { TDLUIExtension::Map(nAttribute), 0 };
	mod.bValue = (value ? TRUE : FALSE);

	return DoNotify(&mod, 1);
}

bool TDLNotify::NotifyMod(String^ sCustAttribID, String^ value)
{
	IUITASKMOD mod = { IUI_CUSTOMATTRIB, 0 };

	mod.szValue = MS(value);
	mod.szCustomAttribID = MS(sCustAttribID);

	return DoNotify(&mod, 1);
}

bool TDLNotify::NotifyMod(TDLUIExtension::TaskAttribute nAttribute, String^ value)
{
	IUITASKMOD mod = { TDLUIExtension::Map(nAttribute), 0 };
	mod.szValue = MS(value);

	return DoNotify(&mod, 1);
}

bool TDLNotify::NotifyMod(TDLUIExtension::TaskAttribute nAttribute, cli::array<String^>^ aValues)
{
	IUITASKMOD mod = { TDLUIExtension::Map(nAttribute), 0 };
	//mod.szValue = MS(value);

	return DoNotify(&mod, 1);
}

bool TDLNotify::DoNotify(const IUITASKMOD* pMod, int numMod)
{
	if (!IsWindow(m_hwndParent))
		return false;

	::SendMessage(m_hwndParent, WM_IUI_MODIFYSELECTEDTASK, numMod, (LPARAM)pMod);
	return true;
}

bool TDLNotify::NotifySelChange(UInt32 taskID)
{
	if (!IsWindow(m_hwndParent))
		return false;

	::SendMessage(m_hwndParent, WM_IUI_SELECTTASK, 0, taskID);
	return true;
}

bool TDLNotify::NotifySelChange(cli::array<UInt32>^ pdwTaskIDs)
{
	if (!IsWindow(m_hwndParent) || !pdwTaskIDs->Length)
		return false;

	pin_ptr<UInt32> p = &pdwTaskIDs[0];
	::SendMessage(m_hwndParent, WM_IUI_SELECTTASK, pdwTaskIDs->Length, (LPARAM)p);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////

