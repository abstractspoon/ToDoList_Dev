// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include <unknwn.h>
#include <tchar.h>

#include "stdafx.h"
#include "SampleUIExtensionBridge.h"

#include "..\..\..\Interfaces\ITasklist.h"
#include "..\..\..\Interfaces\ITransText.h"
#include "..\..\..\Interfaces\IPreferences.h"
#include "..\..\..\Interfaces\UITheme.h"

////////////////////////////////////////////////////////////////////////////////////////////////

#using <..\Debug\SampleUIExtensionCore.dll>
#include <msclr\auto_gcroot.h>

#using <..\Debug\PluginHelpers.dll> as_friend

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace SampleUIExtension;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace TDLPluginHelpers;

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

LPCTSTR CSampleUIExtensionBridge::GetMenuText() const
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

////////////////////////////////////////////////////////////////////////////////////////////////

CSampleUIExtensionBridgeWindow::CSampleUIExtensionBridgeWindow()
{

}

void CSampleUIExtensionBridgeWindow::Release()
{
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

bool CSampleUIExtensionBridgeWindow::SelectTasks(DWORD* pdwTaskIDs, int nTaskCount)
{
	array<UInt32>^ taskIDs = gcnew array<UInt32>(nTaskCount);

    return m_wnd->SelectTasks(taskIDs, nTaskCount);
}

void CSampleUIExtensionBridgeWindow::UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, IUI_ATTRIBUTEEDIT nEditAttribute)
{
	msclr::auto_gcroot<TDLTaskList^> tasks = gcnew TDLTaskList(pTasks);

	m_wnd->UpdateTasks(tasks.get(), TDLUIExtension::Map(nUpdate), TDLUIExtension::Map(nEditAttribute));
}

bool CSampleUIExtensionBridgeWindow::WantUpdate(IUI_ATTRIBUTEEDIT nAttribute) const
{
	return m_wnd->WantUpdate(TDLUIExtension::Map(nAttribute));
}

bool CSampleUIExtensionBridgeWindow::PrepareNewTask(ITaskList* pTask) const
{
	msclr::auto_gcroot<TDLTaskList^> task = gcnew TDLTaskList(pTask);

	return m_wnd->PrepareNewTask(task.get());
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

void CSampleUIExtensionBridgeWindow::DoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra)
{
	m_wnd->DoAppCommand(TDLUIExtension::Map(nCmd), dwExtra);
}

bool CSampleUIExtensionBridgeWindow::CanDoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra) const
{
	return m_wnd->CanDoAppCommand(TDLUIExtension::Map(nCmd), dwExtra);
}

bool CSampleUIExtensionBridgeWindow::GetLabelEditRect(LPRECT pEdit)
{
	return m_wnd->GetLabelEditRect((Int32&)pEdit->left, (Int32&)pEdit->top, (Int32&)pEdit->right, (Int32&)pEdit->bottom);
}

IUI_HITTEST CSampleUIExtensionBridgeWindow::HitTest(const POINT& ptScreen) const
{
	return TDLUIExtension::Map(m_wnd->HitTest(ptScreen.x, ptScreen.y));
}

void CSampleUIExtensionBridgeWindow::SetUITheme(const UITHEME* pTheme)
{
	msclr::auto_gcroot<TDLTheme^> theme = gcnew TDLTheme(pTheme);

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
	msclr::auto_gcroot<TDLPreferences^> prefs = gcnew TDLPreferences(pPrefs);
	msclr::auto_gcroot<String^> key = gcnew String(szKey);

	m_wnd->SavePreferences(prefs.get(), key.get());
}

void CSampleUIExtensionBridgeWindow::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly)
{
	msclr::auto_gcroot<TDLPreferences^> prefs = gcnew TDLPreferences(pPrefs);
	msclr::auto_gcroot<String^> key = gcnew String(szKey);

	m_wnd->LoadPreferences(prefs.get(), key.get(), bAppOnly);
}

