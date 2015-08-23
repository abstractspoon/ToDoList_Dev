// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "TDLUIExtension.h"

#include "..\Interfaces\UITheme.h"
#include "..\Interfaces\IUIExtension.h"

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

Windows::Media::Color TDLTheme::GetAppColor(AppColor color)
{
	switch (color)
	{
	case TDLTheme::AppColor::AppBackDark:		return GetColor(m_pTheme->crAppBackDark);
	case TDLTheme::AppColor::AppBackLight:		return GetColor(m_pTheme->crAppBackLight);
	case TDLTheme::AppColor::AppLinesDark:		return GetColor(m_pTheme->crAppLinesDark);
	case TDLTheme::AppColor::AppLinesLight:		return GetColor(m_pTheme->crAppLinesLight);
	case TDLTheme::AppColor::AppText:			return GetColor(m_pTheme->crAppText);
	case TDLTheme::AppColor::MenuBack:			return GetColor(m_pTheme->crMenuBack);
	case TDLTheme::AppColor::ToolbarDark:		return GetColor(m_pTheme->crToolbarDark);
	case TDLTheme::AppColor::ToolbarLight:		return GetColor(m_pTheme->crToolbarLight);
	case TDLTheme::AppColor::StatusBarDark:		return GetColor(m_pTheme->crStatusBarDark);
	case TDLTheme::AppColor::StatusBarLight:	return GetColor(m_pTheme->crStatusBarLight);
	case TDLTheme::AppColor::StatusBarText:		return GetColor(m_pTheme->crStatusBarText);
	}

	return System::Windows::Media::Colors::Black;
}

String^ TDLTheme::GetToolBarImagePath()
{
	return gcnew String(m_pTheme->szToolbarImage);
}

Windows::Media::Color TDLTheme::GetToolbarTransparencyColor()
{
	return GetColor(m_pTheme->crToolbarTransparency);
}

Windows::Media::Color TDLTheme::GetColor(UInt32 rgbColor)
{
	System::Windows::Media::Color^ color = gcnew System::Windows::Media::Color();
	color->FromArgb(255, GetRValue(rgbColor), GetGValue(rgbColor), GetBValue(rgbColor));

	return *color;
}

////////////////////////////////////////////////////////////////////////////////////////////////

TDLUIExtension::TaskAttribute TDLUIExtension::Map(IUI_ATTRIBUTEEDIT attrib)
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
	case IUI_ALLATTRIB:		return TDLUIExtension::TaskAttribute::All;
	//  case IUI_
	}

	return TDLUIExtension::TaskAttribute::Unknown;
}

TDLUIExtension::UpdateType TDLUIExtension::Map(IUI_UPDATETYPE type)
{
	switch (type)
	{
	case IUI_EDIT:		return TDLUIExtension::UpdateType::Edit;
	case IUI_ADD:		return TDLUIExtension::UpdateType::Add;
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

