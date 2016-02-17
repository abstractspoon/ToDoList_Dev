// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include <unknwn.h>
#include <tchar.h>

#include "stdafx.h"
#include "MDContentControlBridge.h"

#include "..\..\..\Interfaces\ITransText.h"
#include "..\..\..\Interfaces\IPreferences.h"

////////////////////////////////////////////////////////////////////////////////////////////////

#using <..\Debug\MDContentControlCore.dll>
#include <msclr\auto_gcroot.h>

#using <..\Debug\PluginHelpers.dll> as_friend

using namespace MDContentControl;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace TDLPluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

// REPLACE THIS WITH NEW GUID!
const LPCWSTR SAMPLE_GUID = L"00000000-0000-0000-0000-000000000000";
const LPCWSTR SAMPLE_NAME = L"Markdown";

////////////////////////////////////////////////////////////////////////////////////////////////

CMDContentBridge::CMDContentBridge()// : m_hIcon(NULL)
{
//    HMODULE hMod = LoadLibrary(L"MDContentBridge.dll"); // us
// 
//    m_hIcon = ::LoadIcon(hMod, MAKEINTRESOURCE(IDI_STATS));
}

CMDContentBridge::~CMDContentBridge()
{
//    ::DestroyIcon(m_hIcon);
}

void CMDContentBridge::Release()
{
   delete this;
}

void CMDContentBridge::SetLocalizer(ITransText* /*pTT*/)
{
   // TODO
}

LPCTSTR CMDContentBridge::GetTypeDescription() const
{
   return SAMPLE_NAME;
}

// HICON CMDContentBridge::GetIcon() const
// {
//    return m_hIcon;
// }

LPCWSTR CMDContentBridge::GetTypeID() const
{
   return SAMPLE_GUID;
}

IContentControl* CMDContentBridge::CreateCtrl(unsigned short nCtrlID, unsigned long nStyle, 
                                             long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
{
   CMDContentBridgeControl* pCtrl = new CMDContentBridgeControl;

   if (!pCtrl->Create(nCtrlID, nStyle, nLeft, nTop, nWidth, nHeight, hwndParent))
   {
      pCtrl->Release();
      pCtrl = NULL;
   }

   return pCtrl;
}


////////////////////////////////////////////////////////////////////////////////////////////////

// This is the constructor of a class that has been exported.
// see ExporterBridge.h for the class definition
CMDContentBridgeControl::CMDContentBridgeControl()
{
}

BOOL CMDContentBridgeControl::Create(UINT nCtrlID, DWORD nStyle, 
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
      m_wnd = gcnew MDContentControl::MDContentControlCore();
      m_source->RootVisual = m_wnd;

      return true;
   }

   return false;
}

int CMDContentBridgeControl::GetContent(unsigned char* pContent) const
{
   return 0;
}

bool CMDContentBridgeControl::SetContent(const unsigned char* pContent, int nLength, BOOL bResetSelection)
{
   return false;
}

LPCWSTR CMDContentBridgeControl::GetTypeID() const
{
   return L"";
}

// text content if supported. return false if not supported
int CMDContentBridgeControl::GetTextContent(LPWSTR szContent, int nLength) const
{
   return 0;
}

bool CMDContentBridgeControl::SetTextContent(LPCWSTR szContent, BOOL bResetSelection)
{
   return false;
}

void CMDContentBridgeControl::SetReadOnly(bool bReadOnly)
{

}

HWND CMDContentBridgeControl::GetHwnd() const
{
   return NULL;
}

void CMDContentBridgeControl::Release()
{

}

bool CMDContentBridgeControl::ProcessMessage(MSG* pMsg)
{
   return false;
}

ISpellCheck* CMDContentBridgeControl::GetSpellCheckInterface()
{
   return nullptr;
}

bool CMDContentBridgeControl::Undo()
{
   return false;
}

bool CMDContentBridgeControl::Redo()
{
   return false;
}

void CMDContentBridgeControl::SetUITheme(const UITHEME* pTheme)
{

}

void CMDContentBridgeControl::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const
{

}

void CMDContentBridgeControl::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey)
{

}
