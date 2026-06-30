// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include <unknwn.h>
#include <tchar.h>
#include <msclr\auto_gcroot.h>

#include "stdafx.h" 
#include "LoggedTimeUIExtensionBridge.h"
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

using namespace LoggedTimeUIExtension;
using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

const LPCWSTR TIMELOG_GUID = L"F428A59A-97AF-4CA7-92CF-B1DC4CD5920C";
const LPCWSTR TIMELOG_NAME = L"Logged Time";

////////////////////////////////////////////////////////////////////////////////////////////////

CLoggedTimeUIExtensionBridge::CLoggedTimeUIExtensionBridge() : m_hIcon(NULL), m_pTT(nullptr)
{
	m_hIcon = Win32::LoadHIcon(L"LoggedTimeUIExtensionBridge.dll", IDI_DAYVIEW, 16, true);
}

void CLoggedTimeUIExtensionBridge::Release()
{
	delete this;
}

void CLoggedTimeUIExtensionBridge::SetLocalizer(ITransText* pTT)
{
	if (m_pTT == nullptr)
		m_pTT = pTT;
}

LPCWSTR CLoggedTimeUIExtensionBridge::GetMenuText() const
{
	return TIMELOG_NAME;
}

HICON CLoggedTimeUIExtensionBridge::GetIcon() const
{
	return m_hIcon;
}

LPCWSTR CLoggedTimeUIExtensionBridge::GetTypeID() const
{
	return TIMELOG_GUID;
}

IUIExtensionWindow* CLoggedTimeUIExtensionBridge::CreateExtWindow(UINT nCtrlID, 
	DWORD nStyle, long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
{
	CLoggedTimeUIExtensionBridgeWindow* pExtWnd = new CLoggedTimeUIExtensionBridgeWindow(m_pTT);

	if (!pExtWnd->Create(nCtrlID, nStyle, nLeft, nTop, nWidth, nHeight, hwndParent))
	{
		delete pExtWnd;
		pExtWnd = NULL;
	}

	return pExtWnd;
}

void CLoggedTimeUIExtensionBridge::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const
{
	// TODO
}

void CLoggedTimeUIExtensionBridge::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey)
{
	// TODO
}

////////////////////////////////////////////////////////////////////////////////////////////////

CLoggedTimeUIExtensionBridgeWindow::CLoggedTimeUIExtensionBridgeWindow(ITransText* pTT)
	: 
	m_pTT(pTT)
{

}

BOOL CLoggedTimeUIExtensionBridgeWindow::Create(UINT nCtrlID, DWORD nStyle, 
	long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
{
	msclr::auto_gcroot<Translator^> trans = gcnew Translator(m_pTT);
	msclr::auto_gcroot<String^> typeID = gcnew String(TIMELOG_GUID);
	msclr::auto_gcroot<String^> uiName = gcnew String(TIMELOG_NAME);

	m_wnd = gcnew LoggedTimeUIExtension::LoggedTimeUIExtensionCore(typeID.get(), uiName.get(), static_cast<IntPtr>(hwndParent), trans.get());

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

HICON CLoggedTimeUIExtensionBridgeWindow::GetIcon() const
{
	return NULL;
}

LPCWSTR CLoggedTimeUIExtensionBridgeWindow::GetMenuText() const
{
	return TIMELOG_NAME;
}

LPCWSTR CLoggedTimeUIExtensionBridgeWindow::GetTypeID() const
{
	return TIMELOG_GUID;
}

void CLoggedTimeUIExtensionBridgeWindow::UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate)
{
	msclr::auto_gcroot<TaskList^> tasks = gcnew TaskList(pTasks);

	m_wnd->UpdateTasks(tasks.get(), UIExtension::MapUpdateType(nUpdate));
}

bool CLoggedTimeUIExtensionBridgeWindow::WantTaskUpdate(TDC_ATTRIBUTE nAttribID) const
{
	return m_wnd->WantTaskUpdate(Task::MapAttribute(nAttribID));
}

bool CLoggedTimeUIExtensionBridgeWindow::ProcessMessage(MSG* pMsg)
{
	return m_wnd->ProcessMessage(IntPtr(pMsg->hwnd), 
		pMsg->message, 
		pMsg->wParam, 
		pMsg->lParam, 
		pMsg->time, 
		pMsg->pt.x,
		pMsg->pt.y);
}

bool CLoggedTimeUIExtensionBridgeWindow::DoIdleProcessing()
{
	return m_wnd->DoIdleProcessing();
}

bool CLoggedTimeUIExtensionBridgeWindow::DoAppCommand(IUI_APPCOMMAND nCmd, IUIAPPCOMMANDDATA* pData)
{
	switch (nCmd)
	{
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
 		break;

	case IUI_SCROLLTOSELECTEDTASK:
		return m_wnd->ScrollToSelectedTask();
	}

	// all else
	return false;
}

bool CLoggedTimeUIExtensionBridgeWindow::CanDoAppCommand(IUI_APPCOMMAND nCmd, const IUIAPPCOMMANDDATA* pData) const
{
	switch (nCmd)
	{
	case IUI_SETFOCUS:
		return !m_wnd->Focused;

	case IUI_SAVETOIMAGE:
		return m_wnd->CanSaveToImage();

	case IUI_SCROLLTOSELECTEDTASK:
		return m_wnd->SelectTask(m_wnd->CanScrollToSelectedTask());
	}

	// all else
	return false;
}

void CLoggedTimeUIExtensionBridgeWindow::SetUITheme(const UITHEME* pTheme)
{
	msclr::auto_gcroot<UITheme^> theme = gcnew UITheme(pTheme);

	m_wnd->SetUITheme(theme.get());
}

void CLoggedTimeUIExtensionBridgeWindow::SetTaskFont(HFONT hFont)
{
	m_wnd->SetTaskFont(Win32::GetFaceName(hFont), Win32::GetPointSize(hFont));
}

void CLoggedTimeUIExtensionBridgeWindow::SetReadOnly(bool bReadOnly)
{
	m_wnd->SetReadOnly(bReadOnly);
}

HWND CLoggedTimeUIExtensionBridgeWindow::GetHwnd() const
{
	return static_cast<HWND>(m_wnd->Handle.ToPointer());
}

void CLoggedTimeUIExtensionBridgeWindow::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const
{
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<String^> key = gcnew String(szKey);

	m_wnd->SavePreferences(prefs.get(), key.get());
}

void CLoggedTimeUIExtensionBridgeWindow::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly)
{
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<String^> key = gcnew String(szKey);

	m_wnd->LoadPreferences(prefs.get(), key.get(), bAppOnly);
}

