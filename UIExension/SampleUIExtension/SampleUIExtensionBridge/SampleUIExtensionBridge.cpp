// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include <unknwn.h>
#include <tchar.h>
#include <msclr\auto_gcroot.h>

#include "stdafx.h"
#include "SampleUIExtensionBridge.h"

#include "..\..\..\..\ToDoList_Dev\Interfaces\ITasklist.h"
#include "..\..\..\..\ToDoList_Dev\Interfaces\ITransText.h"
#include "..\..\..\..\ToDoList_Dev\Interfaces\IPreferences.h"
#include "..\..\..\..\ToDoList_Dev\Interfaces\UITheme.h"

////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#	using <..\..\..\Debug\SampleUIExtensionCore.dll>
#	using <..\..\..\Debug\PluginHelpers.dll> as_friend
#else
#	using <..\..\..\Release\SampleUIExtensionCore.dll>
#	using <..\..\..\Release\PluginHelpers.dll> as_friend
#endif

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace SampleUIExtension;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

// REPLACE THIS WITH NEW GUID!
const LPCWSTR SAMPLE_GUID = L"00000000-0000-0000-0000-000000000000";
const LPCWSTR SAMPLE_NAME = L"Sample";

////////////////////////////////////////////////////////////////////////////////////////////////

CSampleUIExtensionBridge::CSampleUIExtensionBridge()
{
}

void CSampleUIExtensionBridge::Release()
{
	delete this;
}

void CSampleUIExtensionBridge::SetLocalizer(ITransText* /*pTT*/)
{
	// TODO
}

LPCWSTR CSampleUIExtensionBridge::GetMenuText() const
{
	return SAMPLE_NAME;
}

HICON CSampleUIExtensionBridge::GetIcon() const
{
	return NULL;
}

LPCWSTR CSampleUIExtensionBridge::GetTypeID() const
{
	return SAMPLE_GUID;
}

IUIExtensionWindow* CSampleUIExtensionBridge::CreateExtWindow(UINT nCtrlID, 
	DWORD nStyle, long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
{
	CSampleUIExtensionBridgeWindow* pExtWnd = new CSampleUIExtensionBridgeWindow;

	if (!pExtWnd->Create(nCtrlID, nStyle, nLeft, nTop, nWidth, nHeight, hwndParent))
	{
		pExtWnd->Release();
		pExtWnd = NULL;
	}

	return pExtWnd;
}

void CSampleUIExtensionBridge::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const
{
	// TODO
}

void CSampleUIExtensionBridge::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey)
{
	// TODO
}

////////////////////////////////////////////////////////////////////////////////////////////////

CSampleUIExtensionBridgeWindow::CSampleUIExtensionBridgeWindow()
{

}

void CSampleUIExtensionBridgeWindow::Release()
{
	::DestroyWindow(GetHwnd());
	delete this;
}

BOOL CSampleUIExtensionBridgeWindow::Create(UINT nCtrlID, DWORD nStyle, 
	long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
{
	m_source = gcnew System::Windows::Interop::HwndSource(
		CS_VREDRAW | CS_HREDRAW,
		nStyle,
		0,
		nLeft,
		nTop,
		nWidth,
		nHeight,
		"",
		System::IntPtr(hwndParent));

	if (m_source->Handle != IntPtr::Zero)
	{
		m_wnd = gcnew SampleUIExtension::SampleUIExtensionCore();
		m_source->RootVisual = m_wnd;

		return true;
	}

	return false;
}

HICON CSampleUIExtensionBridgeWindow::GetIcon() const
{
	return NULL;
}

LPCWSTR CSampleUIExtensionBridgeWindow::GetMenuText() const
{
	return SAMPLE_NAME;
}

LPCWSTR CSampleUIExtensionBridgeWindow::GetTypeID() const
{
	return SAMPLE_GUID;
}

bool CSampleUIExtensionBridgeWindow::SelectTask(DWORD dwTaskID)
{
	return m_wnd->SelectTask(dwTaskID);
}

bool CSampleUIExtensionBridgeWindow::SelectTasks(const DWORD* pdwTaskIDs, int nTaskCount)
{
	array<UInt32>^ taskIDs = gcnew array<UInt32>(nTaskCount);

	for (int i = 0; i < nTaskCount; i++)
		taskIDs[i] = pdwTaskIDs[i];

	return m_wnd->SelectTasks(taskIDs);
}

void CSampleUIExtensionBridgeWindow::UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, const IUI_ATTRIBUTE* pAttributes, int nNumAttributes)
{
	msclr::auto_gcroot<TaskList^> tasks = gcnew TaskList(pTasks);

	m_wnd->UpdateTasks(tasks.get(), UIExtension::Map(nUpdate), UIExtension::Map(pAttributes, nNumAttributes));
}

bool CSampleUIExtensionBridgeWindow::WantEditUpdate(IUI_ATTRIBUTE nAttribute) const
{
	return m_wnd->WantEditUpdate(UIExtension::Map(nAttribute));
}

bool CSampleUIExtensionBridgeWindow::WantSortUpdate(IUI_ATTRIBUTE nAttribute) const
{
	return m_wnd->WantSortUpdate(UIExtension::Map(nAttribute));
}

bool CSampleUIExtensionBridgeWindow::PrepareNewTask(ITaskList* pTask) const
{
	msclr::auto_gcroot<TaskList^> task = gcnew TaskList(pTask);

	return m_wnd->PrepareNewTask(task.get()->GetFirstTask());
}

bool CSampleUIExtensionBridgeWindow::ProcessMessage(MSG* pMsg)
{
	return m_wnd->ProcessMessage(IntPtr(pMsg->hwnd), 
										pMsg->message, 
										pMsg->wParam, 
										pMsg->lParam, 
										pMsg->time, 
										pMsg->pt.x,
										pMsg->pt.y);
}

bool CSampleUIExtensionBridgeWindow::DoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra)
{
	return m_wnd->DoAppCommand(UIExtension::Map(nCmd), dwExtra);
}

bool CSampleUIExtensionBridgeWindow::CanDoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra) const
{
	return m_wnd->CanDoAppCommand(UIExtension::Map(nCmd), dwExtra);
}

bool CSampleUIExtensionBridgeWindow::GetLabelEditRect(LPRECT pEdit)
{
	return m_wnd->GetLabelEditRect((Int32&)pEdit->left, (Int32&)pEdit->top, (Int32&)pEdit->right, (Int32&)pEdit->bottom);
}

IUI_HITTEST CSampleUIExtensionBridgeWindow::HitTest(const POINT& ptScreen) const
{
	return UIExtension::Map(m_wnd->HitTest(ptScreen.x, ptScreen.y));
}

void CSampleUIExtensionBridgeWindow::SetUITheme(const UITHEME* pTheme)
{
	msclr::auto_gcroot<UITheme^> theme = gcnew UITheme(pTheme);

	m_wnd->SetUITheme(theme.get());
}

void CSampleUIExtensionBridgeWindow::SetReadOnly(bool bReadOnly)
{
	m_wnd->SetReadOnly(bReadOnly);
}

HWND CSampleUIExtensionBridgeWindow::GetHwnd() const
{
	return static_cast<HWND>(m_source->Handle.ToPointer());
}

void CSampleUIExtensionBridgeWindow::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const
{
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<String^> key = gcnew String(szKey);

	m_wnd->SavePreferences(prefs.get(), key.get());
}

void CSampleUIExtensionBridgeWindow::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly)
{
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<String^> key = gcnew String(szKey);

	m_wnd->LoadPreferences(prefs.get(), key.get(), bAppOnly);
}

