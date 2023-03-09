// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include <unknwn.h>
#include <tchar.h>
#include <msclr\auto_gcroot.h>

#include "stdafx.h" 
#include "DayViewUIExtensionBridge.h"
#include "resource.h"

#include <Interfaces\ITasklist.h>
#include <Interfaces\ITransText.h>
#include <Interfaces\IPreferences.h>
#include <Interfaces\UITheme.h>

////////////////////////////////////////////////////////////////////////////////////////////////

#using <PluginHelpers.dll> as_friend

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace System::Drawing;

using namespace DayViewUIExtension;
using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

const LPCWSTR DAYVIEW_OLDGUID = L"4CBCF4EA-7B02-41E1-BE65-3E03025E1FFE";

const LPCWSTR DAYVIEW_GUID = L"AD05F169-0203-4962-92CD-6E28F8E1A35B";
const LPCWSTR DAYVIEW_NAME = L"Week Planner";

////////////////////////////////////////////////////////////////////////////////////////////////

CDayViewUIExtensionBridge::CDayViewUIExtensionBridge() : m_hIcon(NULL), m_pTT(nullptr)
{
	HMODULE hMod = LoadLibrary(L"DayViewUIExtensionBridge.dll"); // us

	m_hIcon = (HICON)::LoadImage(hMod, MAKEINTRESOURCE(IDI_DAYVIEW), IMAGE_ICON, 16, 16, LR_LOADMAP3DCOLORS);
}

void CDayViewUIExtensionBridge::Release()
{
	delete this;
}

void CDayViewUIExtensionBridge::SetLocalizer(ITransText* pTT)
{
	if (m_pTT == nullptr)
		m_pTT = pTT;
}

LPCWSTR CDayViewUIExtensionBridge::GetMenuText() const
{
	return DAYVIEW_NAME;
}

HICON CDayViewUIExtensionBridge::GetIcon() const
{
	return m_hIcon;
}

LPCWSTR CDayViewUIExtensionBridge::GetTypeID() const
{
	return DAYVIEW_GUID;
}

IUIExtensionWindow* CDayViewUIExtensionBridge::CreateExtWindow(UINT nCtrlID, 
	DWORD nStyle, long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
{
	CDayViewUIExtensionBridgeWindow* pExtWnd = new CDayViewUIExtensionBridgeWindow(m_pTT);

	if (!pExtWnd->Create(nCtrlID, nStyle, nLeft, nTop, nWidth, nHeight, hwndParent))
	{
		delete pExtWnd;
		pExtWnd = NULL;
	}

	return pExtWnd;
}

void CDayViewUIExtensionBridge::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const
{
	// TODO
}

void CDayViewUIExtensionBridge::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey)
{
	// TODO
}

////////////////////////////////////////////////////////////////////////////////////////////////

CDayViewUIExtensionBridgeWindow::CDayViewUIExtensionBridgeWindow(ITransText* pTT)
	: m_pTT(pTT), m_hasOldSettings(false)
{

}

BOOL CDayViewUIExtensionBridgeWindow::Create(UINT nCtrlID, DWORD nStyle, 
	long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
{
	msclr::auto_gcroot<Translator^> trans = gcnew Translator(m_pTT);
	msclr::auto_gcroot<String^> typeID = gcnew String(DAYVIEW_GUID);
	msclr::auto_gcroot<String^> uiName = gcnew String(DAYVIEW_NAME);

	m_wnd = gcnew DayViewUIExtension::DayViewUIExtensionCore(typeID.get(), uiName.get(), static_cast<IntPtr>(hwndParent), trans.get());

	HWND hWnd = GetHwnd();

	if (hWnd)
	{
		::SetParent(hWnd, hwndParent);
		::SetWindowLong(hWnd, GWL_ID, nCtrlID);
		::MoveWindow(hWnd, nLeft, nTop, nWidth, nHeight, FALSE);

		return true;
	}

	return false;
}

HICON CDayViewUIExtensionBridgeWindow::GetIcon() const
{
	return NULL;
}

LPCWSTR CDayViewUIExtensionBridgeWindow::GetMenuText() const
{
	return DAYVIEW_NAME;
}

LPCWSTR CDayViewUIExtensionBridgeWindow::GetTypeID() const
{
	return DAYVIEW_GUID;
}

bool CDayViewUIExtensionBridgeWindow::SelectTask(DWORD dwTaskID, bool bTaskLink)
{
	return m_wnd->SelectTask(dwTaskID);
}

bool CDayViewUIExtensionBridgeWindow::SelectTasks(const DWORD* pdwTaskIDs, int nTaskCount)
{
	array<UInt32>^ taskIDs = gcnew array<UInt32>(nTaskCount);

	for (int i = 0; i < nTaskCount; i++)
		taskIDs[i] = pdwTaskIDs[i];

	return m_wnd->SelectTasks(taskIDs);
}

void CDayViewUIExtensionBridgeWindow::UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate)
{
	msclr::auto_gcroot<TaskList^> tasks = gcnew TaskList(pTasks);

	m_wnd->UpdateTasks(tasks.get(), UIExtension::Map(nUpdate));
}

bool CDayViewUIExtensionBridgeWindow::WantTaskUpdate(TDC_ATTRIBUTE nAttribute) const
{
	return m_wnd->WantTaskUpdate(Task::MapAttribute(nAttribute));
}

bool CDayViewUIExtensionBridgeWindow::PrepareNewTask(ITaskList* pTask) const
{
	msclr::auto_gcroot<TaskList^> task = gcnew TaskList(pTask);

	return m_wnd->PrepareNewTask(task.get()->GetFirstTask());
}

bool CDayViewUIExtensionBridgeWindow::ProcessMessage(MSG* pMsg)
{
	return m_wnd->ProcessMessage(IntPtr(pMsg->hwnd), 
		pMsg->message, 
		pMsg->wParam, 
		pMsg->lParam, 
		pMsg->time, 
		pMsg->pt.x,
		pMsg->pt.y);
}

bool CDayViewUIExtensionBridgeWindow::DoAppCommand(IUI_APPCOMMAND nCmd, IUIAPPCOMMANDDATA* pData)
{
	switch (nCmd)
	{
	case IUI_GETNEXTTASK:
	case IUI_GETNEXTVISIBLETASK:
	case IUI_GETNEXTTOPLEVELTASK:
	case IUI_GETPREVTASK:
	case IUI_GETPREVVISIBLETASK:
	case IUI_GETPREVTOPLEVELTASK:
		if (pData)
		{
			UInt32 taskID = GetNextTask(nCmd, pData->dwTaskID);

			if ((taskID != 0) && (taskID != pData->dwTaskID))
			{
				pData->dwTaskID = taskID;
				return true;
			}
		}
		break;

	case IUI_SELECTFIRSTTASK:
	case IUI_SELECTNEXTTASK:
	case IUI_SELECTNEXTTASKINCLCURRENT:
	case IUI_SELECTPREVTASK:
	case IUI_SELECTLASTTASK:
		if (pData)
			return DoAppSelectCommand(nCmd, pData->select);
		break;

	case IUI_SETFOCUS:
		return m_wnd->Focus();

	case IUI_SAVETOIMAGE:
		if (pData)
		{
			Bitmap^ image = m_wnd->SaveToImage();

			if (image != nullptr)
			{
				msclr::auto_gcroot<String^> sImagePath = gcnew String(pData->szFilePath);

				return UIExtension::SaveImageToFile(image, sImagePath.get());
			}
		}
	}

	// all else
	return false;
}

bool CDayViewUIExtensionBridgeWindow::CanDoAppCommand(IUI_APPCOMMAND nCmd, const IUIAPPCOMMANDDATA* pData) const
{
	switch (nCmd)
	{
	case IUI_SELECTFIRSTTASK:
	case IUI_SELECTNEXTTASK:
	case IUI_SELECTNEXTTASKINCLCURRENT:
	case IUI_SELECTPREVTASK:
	case IUI_SELECTLASTTASK:
		return true;

	case IUI_SETFOCUS:
		return !m_wnd->Focused;

	case IUI_GETNEXTTASK:
	case IUI_GETNEXTVISIBLETASK:
	case IUI_GETNEXTTOPLEVELTASK:
	case IUI_GETPREVTASK:
	case IUI_GETPREVVISIBLETASK:
	case IUI_GETPREVTOPLEVELTASK:
		if (pData)
		{
			DWORD dwTaskID = GetNextTask(nCmd, pData->dwTaskID);
			return ((dwTaskID != 0) && (dwTaskID != pData->dwTaskID));
		}
		break;

	case IUI_SAVETOIMAGE:
		return m_wnd->CanSaveToImage();
	}

	// all else
	return false;
}

DWORD CDayViewUIExtensionBridgeWindow::GetNextTask(IUI_APPCOMMAND nCmd, DWORD dwFromTaskID) const
{
	UIExtension::GetTask getTask;

	if (!UIExtension::Map(nCmd, getTask))
		return 0;

	UInt32 taskID = dwFromTaskID;

	if (!m_wnd->GetTask(getTask, taskID))
		return 0;

	return taskID;
}

bool CDayViewUIExtensionBridgeWindow::DoAppSelectCommand(IUI_APPCOMMAND nCmd, const IUISELECTTASK& select)
{
	UIExtension::SelectTask selectWhat;

	if (!UIExtension::Map(nCmd, selectWhat))
		return false;

	String^ sWords = gcnew String(select.szWords);

	return m_wnd->SelectTask(sWords, selectWhat, select.bCaseSensitive, select.bWholeWord, select.bFindReplace);
}

bool CDayViewUIExtensionBridgeWindow::GetLabelEditRect(LPRECT pEdit)
{
	return m_wnd->GetLabelEditRect((Int32&)pEdit->left, (Int32&)pEdit->top, (Int32&)pEdit->right, (Int32&)pEdit->bottom);
}

IUI_HITTEST CDayViewUIExtensionBridgeWindow::HitTest(POINT ptScreen) const
{
	return UIExtension::Map(m_wnd->HitTest(ptScreen.x, ptScreen.y));
}

DWORD CDayViewUIExtensionBridgeWindow::HitTestTask(POINT ptScreen, bool /*bTitleColumnOnly*/) const
{
	return m_wnd->HitTestTask(ptScreen.x, ptScreen.y);
}

void CDayViewUIExtensionBridgeWindow::SetUITheme(const UITHEME* pTheme)
{
	msclr::auto_gcroot<UITheme^> theme = gcnew UITheme(pTheme);

	m_wnd->SetUITheme(theme.get());
}

void CDayViewUIExtensionBridgeWindow::SetTaskFont(HFONT hFont)
{
	m_wnd->SetTaskFont(Win32::GetFaceName(hFont), Win32::GetPointSize(hFont));
}

void CDayViewUIExtensionBridgeWindow::SetReadOnly(bool bReadOnly)
{
	m_wnd->SetReadOnly(bReadOnly);
}

HWND CDayViewUIExtensionBridgeWindow::GetHwnd() const
{
	return static_cast<HWND>(m_wnd->Handle.ToPointer());
}

void CDayViewUIExtensionBridgeWindow::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const
{
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<String^> key = gcnew String(szKey);

	m_wnd->SavePreferences(prefs.get(), key.get());

	// Delete old settings
	if (m_hasOldSettings)
	{
		auto oldKey = key->Replace(gcnew String(DAYVIEW_GUID), gcnew String(DAYVIEW_OLDGUID));
		prefs->DeleteProfileSection(oldKey, true);
	}
}

void CDayViewUIExtensionBridgeWindow::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly)
{
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<String^> key = gcnew String(szKey);

	// Backwards compatibility because of TypeID change
	auto oldKey = key->Replace(gcnew String(DAYVIEW_GUID), gcnew String(DAYVIEW_OLDGUID));
	
	if (prefs->HasProfileSection(oldKey))
	{
		m_hasOldSettings = true;
		key = oldKey;
	}
	
	m_wnd->LoadPreferences(prefs.get(), key.get(), bAppOnly);
}

