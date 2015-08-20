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
using namespace PluginHelpers;

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
   return false;
}

bool CSampleUIExtensionBridgeWindow::SelectTasks(DWORD* pdwTaskIDs, int nTaskCount)
{
   return false;
}

void CSampleUIExtensionBridgeWindow::UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, int nEditAttribute)
{

}

bool CSampleUIExtensionBridgeWindow::WantUpdate(int nAttribute) const
{
   return true; // everything
}

bool CSampleUIExtensionBridgeWindow::PrepareNewTask(ITaskList* pTask) const
{
   return false;
}

bool CSampleUIExtensionBridgeWindow::ProcessMessage(MSG* pMsg)
{
//    switch (pMsg->message)
//    {
//    default:
//      break;
//    }

   return false;
}

void CSampleUIExtensionBridgeWindow::DoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra)
{

}

bool CSampleUIExtensionBridgeWindow::CanDoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra) const
{
   return true;
}

bool CSampleUIExtensionBridgeWindow::GetLabelEditRect(LPRECT pEdit)
{
   return false;
}

IUI_HITTEST CSampleUIExtensionBridgeWindow::HitTest(const POINT& ptScreen) const
{
   return IUI_NOWHERE;
}

void CSampleUIExtensionBridgeWindow::SetUITheme(const UITHEME* pTheme)
{
   System::Windows::Media::Color^ bkColor = gcnew System::Windows::Media::Color();

   bkColor->R = GetRValue(pTheme->crAppBackLight);
   bkColor->G = GetGValue(pTheme->crAppBackLight);
   bkColor->B = GetBValue(pTheme->crAppBackLight);
   bkColor->A = 255;
   
   m_wnd->SetBackgroundColor(*bkColor);
}

void CSampleUIExtensionBridgeWindow::SetReadOnly(bool bReadOnly)
{

}

HWND CSampleUIExtensionBridgeWindow::GetHwnd() const
{
   return static_cast<HWND>(m_source->Handle.ToPointer());
}

void CSampleUIExtensionBridgeWindow::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const
{

}

void CSampleUIExtensionBridgeWindow::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, BOOL bAppOnly)
{

}

