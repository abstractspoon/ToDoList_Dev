// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "UIExtension.h"
#include "Win32.h"
#include "DPIScaling.h"

#include <Interfaces\UITheme.h>
#include <Interfaces\IUIExtension.h>
#include <Interfaces\ITasklist.h>

#include <Shared\GraphicsMisc.h>

#include <ShellAPI.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::Reflection;
using namespace System::IO;
using namespace System::Windows::Forms;
using namespace System::Windows::Forms::VisualStyles;
using namespace System::Drawing;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

UIExtension::UpdateType UIExtension::Map(IUI_UPDATETYPE type)
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
	szValue(value)
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

bool UIExtension::ParentNotify::IUITaskMod::CopyTo(IUITASKMOD& mod)
{
	mod.nAttrib = Task::MapAttribute(nAttrib);

	switch (dataType)
	{
	case DataType::Double:
		mod.dValue = dValue;
		break;

	case DataType::Date:
		if (tValue == DateTime::MinValue)
			mod.tValue = 0xffffffffffffffff;
		else
			mod.tValue = static_cast<__int64>(Task::MapDate(tValue));
		break;

	case DataType::Integer:
		mod.nValue = nValue;
		break;

	case DataType::Bool:
		mod.bValue = (bValue ? TRUE : FALSE);
		break;

	case DataType::Text:
		{
			MarshalledString^ msString = gcnew MarshalledString(szValue);
			mod.szValue = msString;
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

bool UIExtension::ParentNotify::AddMod(Task::Attribute nAttribute, DateTime date)
{
	if (Task::MapAttribute(nAttribute) == TDCA_NONE)
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(nAttribute, date));
	return true;
}

bool UIExtension::ParentNotify::AddMod(Task::Attribute nAttribute, double value)
{
	if (Task::MapAttribute(nAttribute) == TDCA_NONE)
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(nAttribute, value));
	return true;
}

bool UIExtension::ParentNotify::AddMod(Task::Attribute nAttribute, double time, Task::TimeUnits units)
{
	if (Task::MapAttribute(nAttribute) == TDCA_NONE)
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(nAttribute, time, units));
	return true;
}

bool UIExtension::ParentNotify::AddMod(Task::Attribute nAttribute, int value)
{
	if (Task::MapAttribute(nAttribute) == TDCA_NONE)
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(nAttribute, value));
	return true;
}

bool UIExtension::ParentNotify::AddMod(Task::Attribute nAttribute, bool value)
{
	if (Task::MapAttribute(nAttribute) == TDCA_NONE)
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(nAttribute, value));
	return true;
}

bool UIExtension::ParentNotify::AddMod(String^ sCustAttribID, String^ value)
{
	if (sCustAttribID->IsNullOrEmpty(sCustAttribID))
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(sCustAttribID, value));
	return true;
}

bool UIExtension::ParentNotify::AddMod(Task::Attribute nAttribute, String^ value)
{
	if (Task::MapAttribute(nAttribute) == TDCA_NONE)
		return false;

	m_TaskMods->Add(gcnew IUITaskMod(nAttribute, value));
	return true;
}

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

bool UIExtension::ParentNotify::NotifyMod(Task::Attribute nAttribute, DateTime date)
{
	ClearMods();
	AddMod(nAttribute, date);
	
	return NotifyMod(true);
}

bool UIExtension::ParentNotify::NotifyMod(Task::Attribute nAttribute, double value)
{
	ClearMods();
	AddMod(nAttribute, value);

	return NotifyMod(true);
}

bool UIExtension::ParentNotify::NotifyMod(Task::Attribute nAttribute, double time, Task::TimeUnits units)
{
	ClearMods();
	AddMod(nAttribute, time, units);

	return NotifyMod(true);
}

bool UIExtension::ParentNotify::NotifyMod(Task::Attribute nAttribute, int value)
{
	ClearMods();
	AddMod(nAttribute, value);

	return NotifyMod(true);
}

bool UIExtension::ParentNotify::NotifyMod(Task::Attribute nAttribute, bool value)
{
	ClearMods();
	AddMod(nAttribute, value);

	return NotifyMod(true);
}

bool UIExtension::ParentNotify::NotifyMod(String^ sCustAttribID, String^ value)
{
	ClearMods();
	AddMod(sCustAttribID, value);

	return NotifyMod(true);
}

bool UIExtension::ParentNotify::NotifyMod(Task::Attribute nAttribute, String^ value)
{
	ClearMods();
	AddMod(nAttribute, value);

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
	BOOL bRet = ::SendMessage(m_hwndParent, WM_IUI_SELECTTASK, pdwTaskIDs->Length, (LPARAM)p);

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

bool UIExtension::TaskIcon::Draw(Graphics^ dc, Int32 x, Int32 y)
{
	if ((m_hilTaskImages == NULL) || (m_iImage == -1))
		return false;

	// Must retrieve clip rect before getting HDC
	Drawing::Rectangle rClip = Rectangle::Truncate(dc->ClipBounds);
	HDC hDC = static_cast<HDC>(dc->GetHdc().ToPointer());

	if (hDC == NULL)
		return false;

	int saveHdc = ::SaveDC(hDC);
	::IntersectClipRect(hDC, rClip.Left, rClip.Top, rClip.Right, rClip.Bottom);

	bool bRes = (ImageList_Draw(m_hilTaskImages, m_iImage, hDC, x, y, ILD_TRANSPARENT) != FALSE);

	::RestoreDC(hDC, saveHdc);
	dc->ReleaseHdc();

	return bRes;
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool UIExtension::ShortcutOverlay::Draw(Graphics^ dc, Int32 x, Int32 y, Int32 cx, Int32 cy)
{
	HDC hDC = static_cast<HDC>(dc->GetHdc().ToPointer());
	bool bRes = false;

	if (hDC != NULL)
	{
		bRes = (FALSE != GraphicsMisc::DrawShortcutOverlay(CDC::FromHandle(hDC), CRect(x, y, x + cx, y + cy)));
		dc->ReleaseHdc();
	}

	return bRes;
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool UIExtension::SelectionRect::Draw(IntPtr hwnd, Graphics^ dc, Int32 x, Int32 y, Int32 cx, Int32 cy)
{
	HWND hWndRef = static_cast<HWND>(hwnd.ToPointer());
	bool focused = (::GetFocus() == hWndRef);

	return Draw(hwnd, dc, x, y, cx, cy, focused);
}

bool UIExtension::SelectionRect::Draw(IntPtr hwnd, Graphics^ dc, Int32 x, Int32 y, Int32 cx, Int32 cy, bool focused)
{
	// Must retrieve clip rect before getting HDC
	Drawing::Rectangle rClip = Rectangle::Truncate(dc->ClipBounds);
	HDC hDC = static_cast<HDC>(dc->GetHdc().ToPointer());

	bool bRes = false;

	if (hDC != NULL)
	{
		HWND hWndRef = static_cast<HWND>(hwnd.ToPointer());
		GM_ITEMSTATE state = (focused ? GMIS_SELECTED : GMIS_SELECTEDNOTFOCUSED);

		int saveHdc = ::SaveDC(hDC);
		::IntersectClipRect(hDC, rClip.Left, rClip.Top, rClip.Right, rClip.Bottom);

		CRect rSel(x, y, (x + cx), (y + cy));

		bRes = (FALSE != GraphicsMisc::DrawExplorerItemBkgnd(CDC::FromHandle(hDC), hWndRef, state, rSel, GMIB_THEMECLASSIC));

		::RestoreDC(hDC, saveHdc);
		dc->ReleaseHdc();
	}

	return bRes;
}

////////////////////////////////////////////////////////////////////////////////////////////////

Windows::Forms::Cursor^ UIExtension::AppCursor(UIExtension::AppCursorType cursorType)
{
	String^ appFolder = Path::GetDirectoryName(Assembly::GetExecutingAssembly()->Location);
	String^ cursorFolder = Path::Combine(appFolder, "Resources\\Cursors");
	String^ cursorFile = nullptr;

	switch (cursorType)
	{
	case UIExtension::AppCursorType::LockedTask:
		cursorFile = Path::Combine(cursorFolder, "Locked.cur");
		break;

	case UIExtension::AppCursorType::NoDrag:
		cursorFile = Path::Combine(cursorFolder, "NoDrag.cur");
		break;
	}

	if ((cursorFile != nullptr) && File::Exists(cursorFile))
	{
		HCURSOR hCursor = (HCURSOR)::LoadImage(NULL, 
											MS(cursorFile), 
											IMAGE_CURSOR, 
											GetSystemMetrics(SM_CXCURSOR), 
											GetSystemMetrics(SM_CYCURSOR), 
											LR_LOADFROMFILE | LR_MONOCHROME | LR_SHARED);
		
		return gcnew Cursor(static_cast<IntPtr>(hCursor));
	}
	
	return nullptr;
}

Windows::Forms::Cursor^ UIExtension::HandCursor()
{
	static HCURSOR hCursor = ::LoadCursor(NULL, IDC_HAND);

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
