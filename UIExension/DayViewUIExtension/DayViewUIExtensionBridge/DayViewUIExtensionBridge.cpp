// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include <unknwn.h>
#include <tchar.h>
#include <msclr\auto_gcroot.h>

#include "stdafx.h"
#include "DayViewUIExtensionBridge.h"
#include "resource.h"

#include "..\..\..\..\ToDoList_Dev\Interfaces\ITasklist.h"
#include "..\..\..\..\ToDoList_Dev\Interfaces\ITransText.h"
#include "..\..\..\..\ToDoList_Dev\Interfaces\IPreferences.h"
#include "..\..\..\..\ToDoList_Dev\Interfaces\UITheme.h"

////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#	using <..\..\..\Debug\DayViewUIExtensionCore.dll>
#	using <..\..\..\Debug\PluginHelpers.dll> as_friend
#else
#	using <..\..\..\Release\DayViewUIExtensionCore.dll>
#	using <..\..\..\Release\PluginHelpers.dll> as_friend
#endif

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace DayViewUIExtension;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

// REPLACE THIS WITH NEW GUID!

const LPCWSTR DAYVIEW_GUID = L"4CBCF4EA-7B02-41E1-BE65-3E03025E1FFE";
const LPCWSTR DAYVIEW_NAME = L"Day View";

////////////////////////////////////////////////////////////////////////////////////////////////

CDayViewUIExtensionBridge::CDayViewUIExtensionBridge() : m_hIcon(NULL)
{
	HMODULE hMod = LoadLibrary(L"DayViewUIExtensionBridge.dll"); // us

	m_hIcon = ::LoadIcon(hMod, MAKEINTRESOURCE(IDI_DAYVIEW));
}

void CDayViewUIExtensionBridge::Release()
{
	delete this;
}

void CDayViewUIExtensionBridge::SetLocalizer(ITransText* /*pTT*/)
{
	// TODO
}

LPCTSTR CDayViewUIExtensionBridge::GetMenuText() const
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
	CDayViewUIExtensionBridgeWindow* pExtWnd = new CDayViewUIExtensionBridgeWindow;

	if (!pExtWnd->Create(nCtrlID, nStyle, nLeft, nTop, nWidth, nHeight, hwndParent))
	{
		pExtWnd->Release();
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

CDayViewUIExtensionBridgeWindow::CDayViewUIExtensionBridgeWindow()
{

}

void CDayViewUIExtensionBridgeWindow::Release()
{
	::DestroyWindow(GetHwnd());
	delete this;
}

BOOL CDayViewUIExtensionBridgeWindow::Create(UINT nCtrlID, DWORD nStyle, 
	long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
{
	m_wnd = gcnew DayViewUIExtension::DayViewUIExtensionCore(static_cast<IntPtr>(hwndParent));

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

bool CDayViewUIExtensionBridgeWindow::SelectTask(DWORD dwTaskID)
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

void CDayViewUIExtensionBridgeWindow::UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, const IUI_ATTRIBUTE* pAttributes, int nNumAttributes)
{
	msclr::auto_gcroot<TaskList^> tasks = gcnew TaskList(pTasks);

	m_wnd->UpdateTasks(tasks.get(), UIExtension::Map(nUpdate), UIExtension::Map(pAttributes, nNumAttributes));
}

bool CDayViewUIExtensionBridgeWindow::WantEditUpdate(IUI_ATTRIBUTE nAttribute) const
{
	return m_wnd->WantEditUpdate(UIExtension::Map(nAttribute));
}

bool CDayViewUIExtensionBridgeWindow::WantSortUpdate(IUI_ATTRIBUTE nAttribute) const
{
	return m_wnd->WantSortUpdate(UIExtension::Map(nAttribute));
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

bool CDayViewUIExtensionBridgeWindow::DoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra)
{
	return m_wnd->DoAppCommand(UIExtension::Map(nCmd), dwExtra);
}

bool CDayViewUIExtensionBridgeWindow::CanDoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra) const
{
	return m_wnd->CanDoAppCommand(UIExtension::Map(nCmd), dwExtra);
}

bool CDayViewUIExtensionBridgeWindow::GetLabelEditRect(LPRECT pEdit)
{
	return m_wnd->GetLabelEditRect((Int32&)pEdit->left, (Int32&)pEdit->top, (Int32&)pEdit->right, (Int32&)pEdit->bottom);
}

IUI_HITTEST CDayViewUIExtensionBridgeWindow::HitTest(const POINT& ptScreen) const
{
	return UIExtension::Map(m_wnd->HitTest(ptScreen.x, ptScreen.y));
}

void CDayViewUIExtensionBridgeWindow::SetUITheme(const UITHEME* pTheme)
{
	msclr::auto_gcroot<UITheme^> theme = gcnew UITheme(pTheme);

	m_wnd->SetUITheme(theme.get());
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
}

void CDayViewUIExtensionBridgeWindow::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly)
{
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<String^> key = gcnew String(szKey);

	m_wnd->LoadPreferences(prefs.get(), key.get(), bAppOnly);
}

