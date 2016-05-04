// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include <unknwn.h>
#include <tchar.h>

#include "stdafx.h"
#include "resource.h"
#include "StatsUIExtensionBridge.h"

#include "..\..\..\..\Interfaces\ITasklist.h"
#include "..\..\..\..\Interfaces\ITransText.h"
#include "..\..\..\..\Interfaces\IPreferences.h"
#include "..\..\..\..\Interfaces\UITheme.h"

////////////////////////////////////////////////////////////////////////////////////////////////

#using <..\..\..\Debug\StatsUIExtensionCore.dll>
#include <msclr\auto_gcroot.h>

#using <..\..\..\Debug\PluginHelpers.dll> as_friend

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace StatsUIExtension;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace TDLPluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

// REPLACE THIS WITH NEW GUID!
const LPCWSTR SAMPLE_GUID = L"00000000-0000-0000-0000-000000000000";
const LPCWSTR SAMPLE_NAME = L"Tasklist Statistics";

////////////////////////////////////////////////////////////////////////////////////////////////

CStatsUIExtensionBridge::CStatsUIExtensionBridge() : m_hIcon(NULL)
{
   HMODULE hMod = LoadLibrary(L"StatsUIExtensionBridge.dll"); // us

   m_hIcon = ::LoadIcon(hMod, MAKEINTRESOURCE(IDI_STATS));
}

CStatsUIExtensionBridge::~CStatsUIExtensionBridge()
{
   ::DestroyIcon(m_hIcon);
}

void CStatsUIExtensionBridge::Release()
{
	delete this;
}

void CStatsUIExtensionBridge::SetLocalizer(ITransText* /*pTT*/)
{
	// TODO
}

LPCTSTR CStatsUIExtensionBridge::GetMenuText() const
{
	return SAMPLE_NAME;
}

HICON CStatsUIExtensionBridge::GetIcon() const
{
   return m_hIcon;
}

LPCWSTR CStatsUIExtensionBridge::GetTypeID() const
{
   return SAMPLE_GUID;
}

IUIExtensionWindow* CStatsUIExtensionBridge::CreateExtWindow(UINT nCtrlID, 
    DWORD nStyle, long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
{
   CStatsUIExtensionBridgeWindow* pExtWnd = new CStatsUIExtensionBridgeWindow;

   if (!pExtWnd->Create(nCtrlID, nStyle, nLeft, nTop, nWidth, nHeight, hwndParent))
   {
      pExtWnd->Release();
      pExtWnd = NULL;
   }

   return pExtWnd;
}

void CStatsUIExtensionBridge::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const
{
	// TODO
}

void CStatsUIExtensionBridge::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey)
{
	// TODO
}

////////////////////////////////////////////////////////////////////////////////////////////////

CStatsUIExtensionBridgeWindow::CStatsUIExtensionBridgeWindow()
{

}

void CStatsUIExtensionBridgeWindow::Release()
{
   delete this;
}

BOOL CStatsUIExtensionBridgeWindow::Create(UINT nCtrlID, DWORD nStyle, 
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
      m_wnd = gcnew StatsUIExtension::StatsUIExtensionCore();
      m_source->RootVisual = m_wnd;

      return true;
   }

   return false;
}

HICON CStatsUIExtensionBridgeWindow::GetIcon() const
{
   return NULL;
}

LPCWSTR CStatsUIExtensionBridgeWindow::GetMenuText() const
{
	return SAMPLE_NAME;
}

LPCWSTR CStatsUIExtensionBridgeWindow::GetTypeID() const
{
   return SAMPLE_GUID;
}

bool CStatsUIExtensionBridgeWindow::SelectTask(DWORD dwTaskID)
{
   return m_wnd->SelectTask(dwTaskID);
}

bool CStatsUIExtensionBridgeWindow::SelectTasks(DWORD* pdwTaskIDs, int nTaskCount)
{
	array<UInt32>^ taskIDs = gcnew array<UInt32>(nTaskCount);

	for (int i = 0; i < nTaskCount; i++)
		taskIDs[i] = pdwTaskIDs[i];

    return m_wnd->SelectTasks(taskIDs);
}

void CStatsUIExtensionBridgeWindow::UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, IUI_ATTRIBUTE nEditAttribute)
{
	msclr::auto_gcroot<TDLTaskList^> tasks = gcnew TDLTaskList(pTasks);

	m_wnd->UpdateTasks(tasks.get(), TDLUIExtension::Map(nUpdate), TDLUIExtension::Map(nEditAttribute));
}

bool CStatsUIExtensionBridgeWindow::WantEditUpdate(IUI_ATTRIBUTE nAttribute) const
{
	return m_wnd->WantEditUpdate(TDLUIExtension::Map(nAttribute));
}

bool CStatsUIExtensionBridgeWindow::WantSortUpdate(IUI_ATTRIBUTE nAttribute) const
{
	return m_wnd->WantSortUpdate(TDLUIExtension::Map(nAttribute));
}

bool CStatsUIExtensionBridgeWindow::PrepareNewTask(ITaskList* pTask) const
{
	msclr::auto_gcroot<TDLTaskList^> task = gcnew TDLTaskList(pTask);

	return m_wnd->PrepareNewTask(task.get());
}

bool CStatsUIExtensionBridgeWindow::ProcessMessage(MSG* pMsg)
{
	return m_wnd->ProcessMessage(IntPtr(pMsg->hwnd), 
								 pMsg->message, 
								 pMsg->wParam, 
								 pMsg->lParam, 
								 pMsg->time, 
								 pMsg->pt.x,
								 pMsg->pt.y);
}

bool CStatsUIExtensionBridgeWindow::DoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra)
{
	return m_wnd->DoAppCommand(TDLUIExtension::Map(nCmd), dwExtra);
}

bool CStatsUIExtensionBridgeWindow::CanDoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra) const
{
	return m_wnd->CanDoAppCommand(TDLUIExtension::Map(nCmd), dwExtra);
}

bool CStatsUIExtensionBridgeWindow::GetLabelEditRect(LPRECT pEdit)
{
	return m_wnd->GetLabelEditRect((Int32&)pEdit->left, (Int32&)pEdit->top, (Int32&)pEdit->right, (Int32&)pEdit->bottom);
}

IUI_HITTEST CStatsUIExtensionBridgeWindow::HitTest(const POINT& ptScreen) const
{
	return TDLUIExtension::Map(m_wnd->HitTest(ptScreen.x, ptScreen.y));
}

void CStatsUIExtensionBridgeWindow::SetUITheme(const UITHEME* pTheme)
{
	msclr::auto_gcroot<TDLTheme^> theme = gcnew TDLTheme(pTheme);

    m_wnd->SetUITheme(theme.get());
}

void CStatsUIExtensionBridgeWindow::SetReadOnly(bool bReadOnly)
{
	m_wnd->SetReadOnly(bReadOnly);
}

HWND CStatsUIExtensionBridgeWindow::GetHwnd() const
{
   return static_cast<HWND>(m_source->Handle.ToPointer());
}

void CStatsUIExtensionBridgeWindow::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const
{
	msclr::auto_gcroot<TDLPreferences^> prefs = gcnew TDLPreferences(pPrefs);
	msclr::auto_gcroot<String^> key = gcnew String(szKey);

	m_wnd->SavePreferences(prefs.get(), key.get());
}

void CStatsUIExtensionBridgeWindow::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly)
{
	msclr::auto_gcroot<TDLPreferences^> prefs = gcnew TDLPreferences(pPrefs);
	msclr::auto_gcroot<String^> key = gcnew String(szKey);

	m_wnd->LoadPreferences(prefs.get(), key.get(), bAppOnly);
}

