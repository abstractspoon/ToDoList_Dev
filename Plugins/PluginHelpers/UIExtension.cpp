// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "UIExtension.h"
#include "Win32.h"
#include "DPIScaling.h"
#include "ColorUtil.h"

#include <Interfaces\UITheme.h>
#include <Interfaces\IUIExtension.h>
#include <Interfaces\ITasklist.h>

#include <Shared\GraphicsMisc.h>
#include <Shared\Misc.h>

#include <ShellAPI.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::Reflection;
using namespace System::IO;
using namespace System::Windows::Forms;
using namespace System::Windows::Forms::VisualStyles;
using namespace System::Drawing;
using namespace System::Drawing::Drawing2D;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

UIExtension::UpdateType UIExtension::MapUpdateType(IUI_UPDATETYPE type)
{
	switch (type)
	{
	case IUI_EDIT:		return UIExtension::UpdateType::Edit;
	case IUI_NEW:		return UIExtension::UpdateType::New;
	case IUI_DELETE:	return UIExtension::UpdateType::Delete;
	case IUI_ALL:		return UIExtension::UpdateType::All;
	//  case IUI_
	}

	return UIExtension::UpdateType::Unknown;
}

IUI_HITTEST UIExtension::MapHitTestResult(UIExtension::HitTestResult result)
{
	switch (result)
	{
	case UIExtension::HitTestResult::Nowhere:		return IUI_NOWHERE;
	case UIExtension::HitTestResult::Tasklist:		return IUI_TASKLIST;
	case UIExtension::HitTestResult::ColumnHeader:	return IUI_COLUMNHEADER;
	case UIExtension::HitTestResult::Task:			return IUI_TASK;
	}

	return IUI_NOWHERE;
}

UIExtension::HitTestReason UIExtension::MapHitTestReason(IUI_HITTESTREASON reason)
{
	switch (reason)
	{
	case IUI_NONE:			return UIExtension::HitTestReason::None;
	case IUI_INFOTIP:		return UIExtension::HitTestReason::InfoTip;
	case IUI_CONTEXTMENU:	return UIExtension::HitTestReason::ContextMenu;
	}

	ASSERT(0);
	return UIExtension::HitTestReason::None;
}

bool UIExtension::MapGetTaskCmd(IUI_APPCOMMAND nCmd, UIExtension::GetTask% getTask)
{
	switch (nCmd)
	{
	case IUI_GETNEXTTASK:
		getTask = UIExtension::GetTask::GetNextTask;
		return true;

	case IUI_GETPREVTASK:
		getTask = UIExtension::GetTask::GetPrevTask;
		return true;

	case IUI_GETNEXTVISIBLETASK:
		getTask = UIExtension::GetTask::GetNextVisibleTask;
		return true;

	case IUI_GETPREVVISIBLETASK:
		getTask = UIExtension::GetTask::GetPrevVisibleTask;
		return true;

	case IUI_GETNEXTTOPLEVELTASK:
		getTask = UIExtension::GetTask::GetNextTopLevelTask;
		return true;

	case IUI_GETPREVTOPLEVELTASK:
		getTask = UIExtension::GetTask::GetPrevTopLevelTask;
		return true;
	}

	// all else
	return false;
}

bool UIExtension::MapSelectTaskCmd(IUI_APPCOMMAND nCmd, UIExtension::SelectTask% selectTask)
{
	switch (nCmd)
	{
	case IUI_SELECTFIRSTTASK:
		selectTask = UIExtension::SelectTask::SelectFirstTask;
		return true;

	case IUI_SELECTNEXTTASK:
		selectTask = UIExtension::SelectTask::SelectNextTask;
		return true;

	case IUI_SELECTNEXTTASKINCLCURRENT:
		selectTask = UIExtension::SelectTask::SelectNextTaskInclCurrent;
		return true;

	case IUI_SELECTPREVTASK:
		selectTask = UIExtension::SelectTask::SelectPrevTask;
		return true;

	case IUI_SELECTLASTTASK:
		selectTask = UIExtension::SelectTask::SelectLastTask;
		return true;
	}

	// all else
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////

UIExtension::ParentNotify::IUITaskMod::IUITaskMod(Task::Attribute attrib, DateTime value)
	:
	nAttrib(attrib),
	dataType(DataType::Date),
	tValue(value)
{

}

UIExtension::ParentNotify::IUITaskMod::IUITaskMod(Task::Attribute attrib, double value)
	:
	nAttrib(attrib),
	dataType(DataType::Double),
	dValue(value)
{

}

UIExtension::ParentNotify::IUITaskMod::IUITaskMod(Task::Attribute attrib, double time, Task::TimeUnits units)
	:
	nAttrib(attrib),
	dataType(DataType::Time),
	dValue(time),
	nTimeUnits(units)
{

}

UIExtension::ParentNotify::IUITaskMod::IUITaskMod(Task::Attribute attrib, int value)
	:
	nAttrib(attrib),
	dataType(DataType::Integer),
	nValue(value)
{

}

UIExtension::ParentNotify::IUITaskMod::IUITaskMod(Task::Attribute attrib, bool value)
	:
	nAttrib(attrib),
	dataType(DataType::Bool),
	bValue(value)
{

}

UIExtension::ParentNotify::IUITaskMod::IUITaskMod(Task::Attribute attrib, String^ value)
	:
	nAttrib(attrib),
	dataType(DataType::Text),
	szValue(value),
	bAppend(false)
{

}

UIExtension::ParentNotify::IUITaskMod::IUITaskMod(Task::Attribute attrib, double value, bool append)
	:
	nAttrib(attrib),
	dataType(DataType::Double),
	dValue(value),
	bAppend(append)
{

}

UIExtension::ParentNotify::IUITaskMod::IUITaskMod(Task::Attribute attrib, double time, Task::TimeUnits units, bool append)
	:
	nAttrib(attrib),
	dataType(DataType::Time),
	dValue(time),
	nTimeUnits(units),
	bAppend(append)
{

}

UIExtension::ParentNotify::IUITaskMod::IUITaskMod(Task::Attribute attrib, int value, bool append)
	:
	nAttrib(attrib),
	dataType(DataType::Integer),
	nValue(value),
	bAppend(append)
{

}

UIExtension::ParentNotify::IUITaskMod::IUITaskMod(Task::Attribute attrib, String^ value, bool append)
	:
	nAttrib(attrib),
	dataType(DataType::Text),
	szValue(value),
	bAppend(append)
{

}

UIExtension::ParentNotify::IUITaskMod::IUITaskMod(String^ customAttribId, String^ value)
	:
	nAttrib(Task::Attribute::CustomAttribute),
	dataType(DataType::Custom),
	szCustomAttribID(customAttribId),
	szValue(value)
{
}

// ---------------------------------------------------------------------------

UIExtension::ParentNotify::IUITaskMod::IUITaskMod(UInt32 taskID, Task::Attribute attrib, DateTime value)
	:
	dwSelectedTaskID(taskID),
	nAttrib(attrib),
	dataType(DataType::Date),
	tValue(value)
{

}

UIExtension::ParentNotify::IUITaskMod::IUITaskMod(UInt32 taskID, Task::Attribute attrib, double value)
	:
	dwSelectedTaskID(taskID),
	nAttrib(attrib),
	dataType(DataType::Double),
	dValue(value)
{

}

UIExtension::ParentNotify::IUITaskMod::IUITaskMod(UInt32 taskID, Task::Attribute attrib, double time, Task::TimeUnits units)
	:
	dwSelectedTaskID(taskID),
	nAttrib(attrib),
	dataType(DataType::Time),
	dValue(time),
	nTimeUnits(units)
{

}

UIExtension::ParentNotify::IUITaskMod::IUITaskMod(UInt32 taskID, Task::Attribute attrib, int value)
	:
	dwSelectedTaskID(taskID),
	nAttrib(attrib),
	dataType(DataType::Integer),
	nValue(value)
{

}

UIExtension::ParentNotify::IUITaskMod::IUITaskMod(UInt32 taskID, Task::Attribute attrib, bool value)
	:
	dwSelectedTaskID(taskID),
	nAttrib(attrib),
	dataType(DataType::Bool),
	bValue(value)
{

}

UIExtension::ParentNotify::IUITaskMod::IUITaskMod(UInt32 taskID, Task::Attribute attrib, String^ value)
	:
	dwSelectedTaskID(taskID),
	nAttrib(attrib),
	dataType(DataType::Text),
	szValue(value),
	bAppend(false)
{

}

UIExtension::ParentNotify::IUITaskMod::IUITaskMod(UInt32 taskID, Task::Attribute attrib, double value, bool append)
	:
	dwSelectedTaskID(taskID),
	nAttrib(attrib),
	dataType(DataType::Double),
	dValue(value),
	bAppend(append)
{

}

UIExtension::ParentNotify::IUITaskMod::IUITaskMod(UInt32 taskID, Task::Attribute attrib, double time, Task::TimeUnits units, bool append)
	:
	dwSelectedTaskID(taskID),
	nAttrib(attrib),
	dataType(DataType::Time),
	dValue(time),
	nTimeUnits(units),
	bAppend(append)
{

}

UIExtension::ParentNotify::IUITaskMod::IUITaskMod(UInt32 taskID, Task::Attribute attrib, int value, bool append)
	:
	dwSelectedTaskID(taskID),
	nAttrib(attrib),
	dataType(DataType::Integer),
	nValue(value),
	bAppend(append)
{

}

UIExtension::ParentNotify::IUITaskMod::IUITaskMod(UInt32 taskID, Task::Attribute attrib, String^ value, bool append)
	:
	dwSelectedTaskID(taskID),
	nAttrib(attrib),
	dataType(DataType::Text),
	szValue(value),
	bAppend(append)
{

}

UIExtension::ParentNotify::IUITaskMod::IUITaskMod(UInt32 taskID, String^ customAttribId, String^ value)
	:
	dwSelectedTaskID(taskID),
	nAttrib(Task::Attribute::CustomAttribute),
	dataType(DataType::Custom),
	szCustomAttribID(customAttribId),
	szValue(value)
{
}

// ---------------------------------------------------------------------------

bool UIExtension::ParentNotify::IUITaskMod::CopyTo(IUITASKMOD& mod)
{
	mod.dwSelectedTaskID = dwSelectedTaskID;
	mod.nAttributeID = Task::MapAttribute(nAttrib);

	switch (dataType)
	{
	case DataType::Double:
		mod.dValue = dValue;
		mod.bAppend = bAppend;
		break;

	case DataType::Date:
		if (tValue == DateTime::MinValue)
			mod.tValue = 0xffffffffffffffff;
		else
			mod.tValue = static_cast<__int64>(Task::MapDate(tValue));
		break;

	case DataType::Integer:
		mod.nValue = nValue;
		mod.bAppend = bAppend;
		break;

	case DataType::Bool:
		mod.bValue = (bValue ? TRUE : FALSE);
		break;

	case DataType::Text:
		{
			MarshalledString^ msString = gcnew MarshalledString(szValue);
			mod.szValue = msString;
			mod.bAppend = bAppend;
		}
		break;

	case DataType::Custom:
		{
			MarshalledString^ msString1 = gcnew MarshalledString(szCustomAttribID);
			mod.szCustomAttribID = msString1;

			MarshalledString^ msString2 = gcnew MarshalledString(szValue);
			mod.szValue = msString2;
		}
		break;

	case DataType::Color:
		mod.crValue = mod.crValue;
		break;

	case DataType::Time:
		mod.dValue = dValue;
		mod.nTimeUnits = Task::MapUnits(nTimeUnits);
		mod.bAppend = bAppend;
		break;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////

UIExtension::ParentNotify::ParentNotify(IntPtr hwndParent) 
	: 
	m_hwndParent(NULL)
{
	m_hwndParent = static_cast<HWND>(hwndParent.ToPointer());
	m_TaskMods = gcnew List<IUITaskMod^>();
}

// -----------------------------------------------------------------------------------

bool UIExtension::ParentNotify::AddMod(Task::Attribute attrib, DateTime date)
{
	if (Task::MapAttribute(attrib) == TDCA_NONE)
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(attrib, date));
	return true;
}

bool UIExtension::ParentNotify::AddMod(Task::Attribute attrib, double value)
{
	if (Task::MapAttribute(attrib) == TDCA_NONE)
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(attrib, value));
	return true;
}

bool UIExtension::ParentNotify::AddMod(Task::Attribute attrib, double time, Task::TimeUnits units)
{
	if (Task::MapAttribute(attrib) == TDCA_NONE)
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(attrib, time, units));
	return true;
}

bool UIExtension::ParentNotify::AddMod(Task::Attribute attrib, int value)
{
	if (Task::MapAttribute(attrib) == TDCA_NONE)
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(attrib, value));
	return true;
}

bool UIExtension::ParentNotify::AddMod(Task::Attribute attrib, bool value)
{
	if (Task::MapAttribute(attrib) == TDCA_NONE)
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(attrib, value));
	return true;
}

bool UIExtension::ParentNotify::AddMod(String^ sCustAttribID, String^ value)
{
	if (sCustAttribID->IsNullOrEmpty(sCustAttribID))
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(sCustAttribID, value));
	return true;
}

bool UIExtension::ParentNotify::AddMod(Task::Attribute attrib, String^ value)
{
	if (Task::MapAttribute(attrib) == TDCA_NONE)
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(attrib, value));
	return true;
}

bool UIExtension::ParentNotify::AddMod(Task::Attribute attrib, double value, bool append)
{
	if (Task::MapAttribute(attrib) == TDCA_NONE)
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(attrib, value, append));
	return true;
}

bool UIExtension::ParentNotify::AddMod(Task::Attribute attrib, double time, Task::TimeUnits units, bool append)
{
	if (Task::MapAttribute(attrib) == TDCA_NONE)
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(attrib, time, units, append));
	return true;
}

bool UIExtension::ParentNotify::AddMod(Task::Attribute attrib, int value, bool append)
{
	if (Task::MapAttribute(attrib) == TDCA_NONE)
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(attrib, value, append));
	return true;
}

bool UIExtension::ParentNotify::AddMod(Task::Attribute attrib, String^ value, bool append)
{
	if (Task::MapAttribute(attrib) == TDCA_NONE)
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(attrib, value, append));
	return true;
}

// -----------------------------------------------------------------------------------

bool UIExtension::ParentNotify::AddMod(UInt32 taskID, Task::Attribute attrib, DateTime date)
{
	if (Task::MapAttribute(attrib) == TDCA_NONE)
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(taskID, attrib, date));
	return true;
}

bool UIExtension::ParentNotify::AddMod(UInt32 taskID, Task::Attribute attrib, double value)
{
	if (Task::MapAttribute(attrib) == TDCA_NONE)
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(taskID, attrib, value));
	return true;
}

bool UIExtension::ParentNotify::AddMod(UInt32 taskID, Task::Attribute attrib, double time, Task::TimeUnits units)
{
	if (Task::MapAttribute(attrib) == TDCA_NONE)
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(taskID, attrib, time, units));
	return true;
}

bool UIExtension::ParentNotify::AddMod(UInt32 taskID, Task::Attribute attrib, int value)
{
	if (Task::MapAttribute(attrib) == TDCA_NONE)
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(taskID, attrib, value));
	return true;
}

bool UIExtension::ParentNotify::AddMod(UInt32 taskID, Task::Attribute attrib, bool value)
{
	if (Task::MapAttribute(attrib) == TDCA_NONE)
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(taskID, attrib, value));
	return true;
}

bool UIExtension::ParentNotify::AddMod(UInt32 taskID, String^ sCustAttribID, String^ value)
{
	if (sCustAttribID->IsNullOrEmpty(sCustAttribID))
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(taskID, sCustAttribID, value));
	return true;
}

bool UIExtension::ParentNotify::AddMod(UInt32 taskID, Task::Attribute attrib, String^ value)
{
	if (Task::MapAttribute(attrib) == TDCA_NONE)
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(taskID, attrib, value));
	return true;
}

bool UIExtension::ParentNotify::AddMod(UInt32 taskID, Task::Attribute attrib, double value, bool append)
{
	if (Task::MapAttribute(attrib) == TDCA_NONE)
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(taskID, attrib, value, append));
	return true;
}

bool UIExtension::ParentNotify::AddMod(UInt32 taskID, Task::Attribute attrib, double time, Task::TimeUnits units, bool append)
{
	if (Task::MapAttribute(attrib) == TDCA_NONE)
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(taskID, attrib, time, units, append));
	return true;
}

bool UIExtension::ParentNotify::AddMod(UInt32 taskID, Task::Attribute attrib, int value, bool append)
{
	if (Task::MapAttribute(attrib) == TDCA_NONE)
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(taskID, attrib, value, append));
	return true;
}

bool UIExtension::ParentNotify::AddMod(UInt32 taskID, Task::Attribute attrib, String^ value, bool append)
{
	if (Task::MapAttribute(attrib) == TDCA_NONE)
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(taskID, attrib, value, append));
	return true;
}

// -----------------------------------------------------------------------------------
// External
bool UIExtension::ParentNotify::NotifyMod()
{
	return NotifyMod(true);
}

// Internal
bool UIExtension::ParentNotify::NotifyMod(bool bClearModsAlways)
{
	int nNumMods = m_TaskMods->Count;

	if (nNumMods == 0)
		return false;

	IUITASKMOD* pMods = new IUITASKMOD[nNumMods];
	ZeroMemory(pMods, (sizeof(IUITASKMOD) * nNumMods));
	
	for (int nMod = 0; nMod < nNumMods; nMod++)
	{
		IUITaskMod^ pMod = m_TaskMods[nMod];
		IUITASKMOD& mod = pMods[nMod];

		pMod->CopyTo(mod);
	}

	bool bSuccess = DoNotify(pMods, nNumMods);

	delete [] pMods; // always

	if (bClearModsAlways || bSuccess)
		ClearMods();

	return bSuccess;
}

bool UIExtension::ParentNotify::NotifyMod(Task::Attribute attrib, DateTime date)
{
	ClearMods();
	AddMod(attrib, date);
	
	return NotifyMod(true);
}

bool UIExtension::ParentNotify::NotifyMod(Task::Attribute attrib, double value)
{
	ClearMods();
	AddMod(attrib, value);

	return NotifyMod(true);
}

bool UIExtension::ParentNotify::NotifyMod(Task::Attribute attrib, double time, Task::TimeUnits units)
{
	ClearMods();
	AddMod(attrib, time, units);

	return NotifyMod(true);
}

bool UIExtension::ParentNotify::NotifyMod(Task::Attribute attrib, int value)
{
	ClearMods();
	AddMod(attrib, value);

	return NotifyMod(true);
}

bool UIExtension::ParentNotify::NotifyMod(Task::Attribute attrib, bool value)
{
	ClearMods();
	AddMod(attrib, value);

	return NotifyMod(true);
}

bool UIExtension::ParentNotify::NotifyMod(String^ sCustAttribID, String^ value)
{
	ClearMods();
	AddMod(sCustAttribID, value);

	return NotifyMod(true);
}

bool UIExtension::ParentNotify::NotifyMod(Task::Attribute attrib, String^ value)
{
	ClearMods();
	AddMod(attrib, value);

	return NotifyMod(true);
}

bool UIExtension::ParentNotify::NotifyMod(Task::Attribute attrib, double value, bool append)
{
	ClearMods();
	AddMod(attrib, value, append);

	return NotifyMod(true);
}

bool UIExtension::ParentNotify::NotifyMod(Task::Attribute attrib, double time, Task::TimeUnits units, bool append)
{
	ClearMods();
	AddMod(attrib, time, units, append);

	return NotifyMod(true);
}

bool UIExtension::ParentNotify::NotifyMod(Task::Attribute attrib, int value, bool append)
{
	ClearMods();
	AddMod(attrib, value, append);

	return NotifyMod(true);
}

bool UIExtension::ParentNotify::NotifyMod(Task::Attribute attrib, String^ value, bool append)
{
	ClearMods();
	AddMod(attrib, value, append);

	return NotifyMod(true);
}

bool UIExtension::ParentNotify::NotifyMove(UInt32 taskID, UInt32 parentTaskID, UInt32 afterSiblingID)
{
	IUITASKMOVE move = { 0 };

	move.dwSelectedTaskID = taskID;
	move.dwParentID = parentTaskID;
	move.dwAfterSiblingID = afterSiblingID;
	move.bCopy = false;

	return DoNotify(&move);
}

bool UIExtension::ParentNotify::NotifyCopy(UInt32 taskID, UInt32 parentTaskID, UInt32 afterSiblingID)
{
	IUITASKMOVE copy = { 0 };

	copy.dwSelectedTaskID = taskID;
	copy.dwParentID = parentTaskID;
	copy.dwAfterSiblingID = afterSiblingID;
	copy.bCopy = true;

	return DoNotify(&copy);
}

void UIExtension::ParentNotify::ClearMods()
{
	m_TaskMods->Clear();
}

bool UIExtension::ParentNotify::DoNotify(const IUITASKMOD* pMod, int numMod)
{
	if (!IsWindow(m_hwndParent))
		return false;

	BOOL bRet = ::SendMessage(m_hwndParent, WM_IUI_MODIFYSELECTEDTASK, numMod, (LPARAM)pMod);

	return (bRet != FALSE);
}

bool UIExtension::ParentNotify::DoNotify(const IUITASKMOVE* pMove)
{
	if (!IsWindow(m_hwndParent))
		return false;

	BOOL bRet = ::SendMessage(m_hwndParent, WM_IUI_MOVESELECTEDTASK, 0, (LPARAM)pMove);

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
	BOOL bRet = ::SendMessage(m_hwndParent, WM_IUI_SELECTTASK, (WPARAM)p, pdwTaskIDs->Length);

	return (bRet != FALSE);
}

bool UIExtension::ParentNotify::NotifyEditLabel()
{
	return (FALSE != ::SendMessage(m_hwndParent, WM_IUI_EDITSELECTEDTASKTITLE, 0, 0));
}

bool UIExtension::ParentNotify::NotifyEditIcon()
{
	return (FALSE != ::SendMessage(m_hwndParent, WM_IUI_EDITSELECTEDTASKICON, 0, 0));
}

bool UIExtension::ParentNotify::NotifyDoHelp(String^ helpID)
{
	return (FALSE != ::SendMessage(m_hwndParent, WM_IUI_DOHELP, 0, (LPARAM)(LPCWSTR)MS(helpID)));
}

bool UIExtension::ParentNotify::NotifyTasklistMetaData(String^ metaData)
{
	return (FALSE != ::SendMessage(m_hwndParent, WM_IUI_SETTASKLISTMETADATA, 0, (LPARAM)(LPCWSTR)MS(metaData)));
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

bool UIExtension::TaskIcon::Draw(Graphics^ g, Int32 x, Int32 y)
{
	if ((m_hilTaskImages == NULL) || (m_iImage == -1))
		return false;

	// Must retrieve clip rect before getting HDC
	Drawing::Rectangle rClip = Rectangle::Truncate(g->ClipBounds);
	HDC hDC = Win32::GetHdc(g->GetHdc());

	if (hDC == NULL)
		return false;

	int nSaveDC = ::SaveDC(hDC);

	::IntersectClipRect(hDC, rClip.Left, rClip.Top, rClip.Right, rClip.Bottom);

	bool bRes = (ImageList_Draw(m_hilTaskImages, m_iImage, hDC, x, y, ILD_TRANSPARENT) != FALSE);

	::RestoreDC(hDC, nSaveDC);
	g->ReleaseHdc();
	g->SetClip(rClip); // restore clip rect

	return bRes;
}

int UIExtension::TaskIcon::IconSize::get() 
{ 
	return DPIScaling::Scale(16); 
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool UIExtension::ShortcutOverlay::Draw(Graphics^ g, Int32 x, Int32 y, Int32 cx, Int32 cy)
{
	HDC hDC = Win32::GetHdc(g->GetHdc());

	if (hDC == NULL)
		return false;

	bool bRes = (FALSE != GraphicsMisc::DrawShortcutOverlay(CDC::FromHandle(hDC), CRect(x, y, x + cx, y + cy)));
	g->ReleaseHdc();

	return bRes;
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool UIExtension::SelectionRect::Draw(IntPtr hwnd, Graphics^ g, Int32 x, Int32 y, Int32 cx, Int32 cy, bool transparent)
{
	HWND hWndRef = Win32::GetHwnd(hwnd);
	bool focused = (::GetFocus() == hWndRef);
	Style style = (focused ? Style::Selected : Style::SelectedNotFocused);

	return Draw(hwnd, g, x, y, cx, cy, style, transparent);
}

bool UIExtension::SelectionRect::Draw(IntPtr hwnd, Graphics^ g, Int32 x, Int32 y, Int32 cx, Int32 cy, Style style, bool transparent)
{
	return Draw(hwnd, g, x, y, cx, cy, style, transparent, false, false);
}

bool UIExtension::SelectionRect::Draw(IntPtr hwnd, Graphics^ g, Int32 x, Int32 y, Int32 cx, Int32 cy, Style style, bool transparent, bool clipLeft, bool clipRight)
{
	if (style == Style::None)
		return false;

	// Must retrieve clip rect before getting HDC
	Drawing::Rectangle clipRect = Rectangle::Truncate(g->ClipBounds);
	HDC hDC = Win32::GetHdc(g->GetHdc());

	if (hDC == NULL)
		return false;

	GM_ITEMSTATE state = Map(style);
	int nSaveDC = ::SaveDC(hDC);

	::IntersectClipRect(hDC, clipRect.Left, clipRect.Top, clipRect.Right, clipRect.Bottom);

	DWORD flags = (GMIB_THEMECLASSIC | (transparent ? GMIB_PREDRAW | GMIB_POSTDRAW : 0));

	if (clipLeft)
		flags |= GMIB_CLIPLEFT;

	if (clipRight)
		flags |= GMIB_CLIPRIGHT;

	CRect rItem(x, y, (x + cx), (y + cy));
	LPCRECT prcClip = ((clipLeft || clipRight) ? &rItem : NULL);

	BOOL bRes = GraphicsMisc::DrawExplorerItemSelection(CDC::FromHandle(hDC), 
														Win32::GetHwnd(hwnd), 
														state, 
														rItem, 
														flags,
														prcClip);
	::RestoreDC(hDC, nSaveDC);
	g->ReleaseHdc();
	g->SetClip(clipRect); // restore clip rect

	return (bRes != FALSE);
}

GM_ITEMSTATE UIExtension::SelectionRect::Map(Style style)
{
	switch (style)
	{
	case Style::Selected:
		return GMIS_SELECTED;

	case Style::SelectedNotFocused:
		return GMIS_SELECTEDNOTFOCUSED;

	case Style::DropHighlighted:
		return GMIS_DROPHILITED;
	}

	return GMIS_NONE;
}

Drawing::Color UIExtension::SelectionRect::GetColor(Style style)
{
	GM_ITEMSTATE state = Map(style);
	COLORREF color = GraphicsMisc::GetExplorerItemSelectionBackColor(state, GMIB_THEMECLASSIC);

	return ColorUtil::DrawingColor::ToColor(color);
}

Drawing::Color UIExtension::SelectionRect::GetBorderColor(Style style)
{
	GM_ITEMSTATE state = Map(style);
	COLORREF color = GraphicsMisc::GetExplorerItemSelectionBorderColor(state, GMIB_THEMECLASSIC);

	return ColorUtil::DrawingColor::ToColor(color);
}

Drawing::Color UIExtension::SelectionRect::GetTextColor(Style style, Drawing::Color baseColor)
{
	GM_ITEMSTATE state = Map(style);
	COLORREF crBase = (baseColor.IsEmpty ? CLR_NONE : ColorUtil::DrawingColor::ToRgb(baseColor));

	COLORREF color = GraphicsMisc::GetExplorerItemSelectionTextColor(crBase, state, GMIB_THEMECLASSIC);

	return ColorUtil::DrawingColor::ToColor(color);
}

////////////////////////////////////////////////////////////////////////////////////////////////

Windows::Forms::Cursor^ UIExtension::AppCursor(UIExtension::AppCursorType cursorType)
{
	LPCWSTR szCursor = NULL;

	switch (cursorType)
	{
	case UIExtension::AppCursorType::LockedTask:
		szCursor = L"locked";
		break;

	case UIExtension::AppCursorType::NoDrag:
		szCursor = L"nodrag";
		break;
	}

	if (szCursor != NULL)
	{
		HCURSOR hCursor = GraphicsMisc::LoadAppCursor(szCursor, L"Resources\\Cursors");
		
		if (hCursor != NULL)
	 		return gcnew Cursor(static_cast<IntPtr>(hCursor));
	}

	return nullptr;
}

Windows::Forms::Cursor^ UIExtension::OleDragCursor(OleDragCursorType cursorType)
{
	HCURSOR hCursor = NULL;
	
	switch (cursorType)
	{
	case UIExtension::OleDragCursorType::No:
		hCursor = GraphicsMisc::LoadDragDropCursor(GMOC_NO);
		break;

	case UIExtension::OleDragCursorType::Copy:
		hCursor = GraphicsMisc::LoadDragDropCursor(GMOC_COPY);
		break;

	case UIExtension::OleDragCursorType::Move:
		hCursor = GraphicsMisc::LoadDragDropCursor(GMOC_MOVE);
		break;

	case UIExtension::OleDragCursorType::Link:
		hCursor = GraphicsMisc::LoadDragDropCursor(GMOC_LINK);
		break;
	}
	
	if (hCursor != NULL)
		return gcnew Cursor(static_cast<IntPtr>(hCursor));

	return nullptr;
}

Windows::Forms::Cursor^ UIExtension::HandCursor()
{
	HCURSOR hCursor = ::LoadCursor(NULL, IDC_HAND);

	return gcnew Cursor(static_cast<IntPtr>(hCursor));
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool UIExtension::SaveImageToFile(Bitmap^ image, String^ filepath)
{
	if (image == nullptr)
		return false;

	String^ ext = System::IO::Path::GetExtension(filepath)->ToLowerInvariant();

	try
	{
		if (ext == L".png")
		{
			image->Save(filepath, Imaging::ImageFormat::Png);
		}
		else if (ext == L".bmp")
		{
			image->Save(filepath, Imaging::ImageFormat::Bmp);
		}
		else if (ext == L".tif")
		{
			image->Save(filepath, Imaging::ImageFormat::Tiff);
		}
		else if ((ext == L".jpg") || (ext == L".jpeg"))
		{
			image->Save(filepath, Imaging::ImageFormat::Jpeg);
		}
		else if (ext == L".gif")
		{
			image->Save(filepath, Imaging::ImageFormat::Gif);
		}
		else
		{
			return false;
		}
	}
	catch (...)
	{
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////

UIExtension::TaskRecurrences::TaskRecurrences(IntPtr hwndParent) : m_hwndParent(NULL)
{
	m_hwndParent = static_cast<HWND>(hwndParent.ToPointer());
}

List<Tuple<DateTime, DateTime>^>^ UIExtension::TaskRecurrences::Get(UInt32 dwTaskID, DateTime^ rangeStart, DateTime^ rangeEnd)
{
	if (!IsWindow(m_hwndParent))
		return nullptr;

	IUINEXTTASKOCCURRENCES iuiNext = { 0 };

	iuiNext.tRangeStart = static_cast<__int64>(Task::MapDate(rangeStart));
	iuiNext.tRangeEnd = static_cast<__int64>(Task::MapDate(rangeEnd));

	if (::SendMessage(m_hwndParent, WM_IUI_GETNEXTTASKOCCURRENCES, dwTaskID, (LPARAM)&iuiNext) == 0)
		return nullptr;

	if (iuiNext.nNumOccurrences == 0)
		return nullptr;

	auto dates = gcnew List<Tuple<DateTime, DateTime>^>();

	for (int item = 0; item < iuiNext.nNumOccurrences; item++)
	{
		const auto& occur = iuiNext.occurrences[item];

		if ((occur.tStart != 0) && (occur.tEnd >= occur.tStart))
		{
			DateTime startDate = Task::MapDate(occur.tStart);
			DateTime endDate = Task::MapDate(occur.tEnd);

			dates->Add(gcnew Tuple<DateTime, DateTime>(startDate, endDate));
		}
	}

	return dates;
}

////////////////////////////////////////////////////////////////////////////////////////////////

cli::array<Point>^ UIExtension::ArrowHeads::Calculate(int x, int y, int size, int offset, Direction dir)
{
	auto arrow = gcnew cli::array<Point>(3);

	for (int i = 0; i < 3; i++)
	{
		arrow[i].X = x;
		arrow[i].Y = y;
	}

	switch (dir)
	{
	case Direction::Left:
		arrow[0].Offset(size, -size);
		arrow[2].Offset(size, size);
		break;

	case Direction::Up:
		arrow[0].Offset(-size, size);
		arrow[2].Offset(size, size);
		break;

	case Direction::Right:
		arrow[0].Offset(-size, -size);
		arrow[2].Offset(-size, size);
		break;

	case Direction::Down:
		arrow[0].Offset(-size, -size);
		arrow[2].Offset(size, -size);
		break;

	default:
		arrow = nullptr;
	}

	return Offset(arrow, offset, dir);
}
void UIExtension::ArrowHeads::Draw(Graphics^ graphics, Pen^ pen, int x, int y, int size, Direction dir)
{
	Draw(graphics, pen, x, y, size, 0, dir);
}

void UIExtension::ArrowHeads::Draw(Graphics^ graphics, Pen^ pen, int x, int y, int size, float angleDegrees)
{
	Draw(graphics, pen, x, y, size, 0, angleDegrees);
}

void UIExtension::ArrowHeads::Draw(Graphics^ graphics, Pen^ pen, int x, int y, int size, int offset, Direction dir)
{
	auto arrow = Calculate(x, y, size, offset, dir);
	
	if (arrow != nullptr)
		graphics->DrawLines(pen, arrow);
}

void UIExtension::ArrowHeads::Draw(Graphics^ graphics, Pen^ pen, int x, int y, int size, int offset, float angleDegrees)
{
	// Calculate arrow at the origin
	auto arrow = Calculate(0, 0, size, offset, Direction::Up);

	if (arrow != nullptr)
	{
		// Calculate the rotation matrix
		auto matrix = gcnew Matrix();

		matrix->Rotate(angleDegrees, MatrixOrder::Append);
		matrix->Translate((float)x, (float)y, MatrixOrder::Append);

		graphics->Transform = matrix;
		graphics->DrawLines(pen, arrow);
		graphics->ResetTransform();
	}
}

cli::array<Drawing::Point>^ UIExtension::ArrowHeads::Offset(cli::array<Drawing::Point>^ arrow, int amount, Direction dir)
{
	if (amount != 0)
	{
		amount = abs(amount);

		for (int i = 0; i < 3; i++)
		{
			switch (dir)
			{
			case Direction::Left:
				arrow[i].Offset(-amount, 0);
				break;

			case Direction::Up:
				arrow[i].Offset(0, amount);
				break;

			case Direction::Right:
				arrow[i].Offset(amount, 0);
				break;

			case Direction::Down:
				arrow[i].Offset(0, -amount);
				break;
			}
		}
	}

	return arrow;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void UIExtension::DependencyArrows::Draw(Drawing::Graphics^ graphics, int x, int y, Drawing::Font^ font, Direction dir)
{
	auto arrow = Calculate(x, y, Size(font), 0, dir);

	graphics->DrawLines(Pens::Black, arrow);

	// Offset and draw again
	graphics->DrawLines(Pens::Black, Offset(arrow, 1, dir));
}

int UIExtension::DependencyArrows::Size(Drawing::Font^ font)
{
	// Size to match Gantt Chart
	return ((font->Height / 4) + 1);
}

