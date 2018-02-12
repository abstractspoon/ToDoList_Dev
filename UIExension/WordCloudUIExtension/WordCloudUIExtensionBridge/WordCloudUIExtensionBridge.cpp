// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include <unknwn.h>
#include <tchar.h>
#include <msclr\auto_gcroot.h>

#include "stdafx.h"
#include "WordCloudUIExtensionBridge.h"
#include "resource.h"

#include "..\..\..\..\ToDoList_Dev\Interfaces\ITasklist.h"
#include "..\..\..\..\ToDoList_Dev\Interfaces\ITransText.h"
#include "..\..\..\..\ToDoList_Dev\Interfaces\IPreferences.h"
#include "..\..\..\..\ToDoList_Dev\Interfaces\UITheme.h"

////////////////////////////////////////////////////////////////////////////////////////////////

#using <PluginHelpers.dll> as_friend

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace WordCloudUIExtension;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

// REPLACE THIS WITH NEW GUID!

const LPCWSTR WORDCLOUD_GUID = L"3BDEF4EA-7B02-41E1-BE65-3E03025E1FFE";
const LPCWSTR WORDCLOUD_NAME = L"Word Cloud";

////////////////////////////////////////////////////////////////////////////////////////////////

CWordCloudUIExtensionBridge::CWordCloudUIExtensionBridge() : m_hIcon(NULL), m_pTT(nullptr)
{
	HMODULE hMod = LoadLibrary(L"WordCloudUIExtensionBridge.dll"); // us

	m_hIcon = ::LoadIcon(hMod, MAKEINTRESOURCE(IDI_DAYVIEW));
}

void CWordCloudUIExtensionBridge::Release()
{
	delete this;
}

void CWordCloudUIExtensionBridge::SetLocalizer(ITransText* pTT)
{
	if (m_pTT == nullptr)
		m_pTT = pTT;
}

LPCWSTR CWordCloudUIExtensionBridge::GetMenuText() const
{
	return WORDCLOUD_NAME;
}

HICON CWordCloudUIExtensionBridge::GetIcon() const
{
	return m_hIcon;
}

LPCWSTR CWordCloudUIExtensionBridge::GetTypeID() const
{
	return WORDCLOUD_GUID;
}

IUIExtensionWindow* CWordCloudUIExtensionBridge::CreateExtWindow(UINT nCtrlID, 
	DWORD nStyle, long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
{
	CWordCloudUIExtensionBridgeWindow* pExtWnd = new CWordCloudUIExtensionBridgeWindow(m_pTT);

	if (!pExtWnd->Create(nCtrlID, nStyle, nLeft, nTop, nWidth, nHeight, hwndParent))
	{
		delete pExtWnd;
		pExtWnd = NULL;
	}

	return pExtWnd;
}

void CWordCloudUIExtensionBridge::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const
{
	// TODO
}

void CWordCloudUIExtensionBridge::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey)
{
	// TODO
}

////////////////////////////////////////////////////////////////////////////////////////////////

CWordCloudUIExtensionBridgeWindow::CWordCloudUIExtensionBridgeWindow(ITransText* pTT)
	: m_pTT(pTT)
{

}

BOOL CWordCloudUIExtensionBridgeWindow::Create(UINT nCtrlID, DWORD nStyle, 
	long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
{
	msclr::auto_gcroot<Translator^> trans = gcnew Translator(m_pTT);

	m_wnd = gcnew WordCloudUIExtension::WordCloudUIExtensionCore(static_cast<IntPtr>(hwndParent), trans.get());

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

HICON CWordCloudUIExtensionBridgeWindow::GetIcon() const
{
	return NULL;
}

LPCWSTR CWordCloudUIExtensionBridgeWindow::GetMenuText() const
{
	return WORDCLOUD_NAME;
}

LPCWSTR CWordCloudUIExtensionBridgeWindow::GetTypeID() const
{
	return WORDCLOUD_GUID;
}

bool CWordCloudUIExtensionBridgeWindow::SelectTask(DWORD dwTaskID)
{
	return m_wnd->SelectTask(dwTaskID);
}

bool CWordCloudUIExtensionBridgeWindow::SelectTasks(const DWORD* pdwTaskIDs, int nTaskCount)
{
	array<UInt32>^ taskIDs = gcnew array<UInt32>(nTaskCount);

	for (int i = 0; i < nTaskCount; i++)
		taskIDs[i] = pdwTaskIDs[i];

	return m_wnd->SelectTasks(taskIDs);
}

void CWordCloudUIExtensionBridgeWindow::UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, const IUI_ATTRIBUTE* pAttributes, int nNumAttributes)
{
	msclr::auto_gcroot<TaskList^> tasks = gcnew TaskList(pTasks);

	m_wnd->UpdateTasks(tasks.get(), UIExtension::Map(nUpdate), UIExtension::Map(pAttributes, nNumAttributes));
}

bool CWordCloudUIExtensionBridgeWindow::WantEditUpdate(IUI_ATTRIBUTE nAttribute) const
{
	return m_wnd->WantEditUpdate(UIExtension::Map(nAttribute));
}

bool CWordCloudUIExtensionBridgeWindow::WantSortUpdate(IUI_ATTRIBUTE nAttribute) const
{
	return m_wnd->WantSortUpdate(UIExtension::Map(nAttribute));
}

bool CWordCloudUIExtensionBridgeWindow::PrepareNewTask(ITaskList* pTask) const
{
	msclr::auto_gcroot<TaskList^> task = gcnew TaskList(pTask);

	return m_wnd->PrepareNewTask(task.get()->GetFirstTask());
}

bool CWordCloudUIExtensionBridgeWindow::ProcessMessage(MSG* pMsg)
{
	return m_wnd->ProcessMessage(IntPtr(pMsg->hwnd), 
		pMsg->message, 
		pMsg->wParam, 
		pMsg->lParam, 
		pMsg->time, 
		pMsg->pt.x,
		pMsg->pt.y);
}

bool CWordCloudUIExtensionBridgeWindow::DoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra)
{
	if (!dwExtra)
		return false;

	switch (nCmd)
	{
	case IUI_SELECTTASK:
		return m_wnd->SelectTask(dwExtra);

	case IUI_GETNEXTTASK:
		{
			UInt32 taskID = 0;
			DWORD* pTaskID = (DWORD*)dwExtra;

			if (m_wnd->GetTask(UIExtension::GetTask::GetNextTask, taskID))
			{
				*pTaskID = taskID;
				return true;
			}
		}
		break;

	case IUI_GETPREVTASK:
		{
			UInt32 taskID = 0;
			DWORD* pTaskID = (DWORD*)dwExtra;

			if (m_wnd->GetTask(UIExtension::GetTask::GetPrevTask, taskID))
			{
				*pTaskID = taskID;
				return true;
			}
		}
		break;

	case IUI_SELECTFIRSTTASK:
	case IUI_SELECTNEXTTASK:
	case IUI_SELECTNEXTTASKINCLCURRENT:
	case IUI_SELECTPREVTASK:
	case IUI_SELECTLASTTASK:
		return DoAppSelectCommand(nCmd, (IUISELECTTASK*)dwExtra);
	}

	// all else
	return false;
}

bool CWordCloudUIExtensionBridgeWindow::CanDoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra) const
{
	switch (nCmd)
	{
	case IUI_SELECTTASK:
	case IUI_GETNEXTTASK:
	case IUI_GETPREVTASK:
	case IUI_SELECTFIRSTTASK:
	case IUI_SELECTNEXTTASK:
	case IUI_SELECTNEXTTASKINCLCURRENT:
	case IUI_SELECTPREVTASK:
	case IUI_SELECTLASTTASK:
		return true;
	}

	// all else
	return false;
}

bool CWordCloudUIExtensionBridgeWindow::DoAppSelectCommand(IUI_APPCOMMAND nCmd, const IUISELECTTASK* pSelect)
{
	// Sanity check
	if (!pSelect)
	{
		//ASSERT(0);
		return false;
	}

	UIExtension::SelectTask selectWhat;

	switch (nCmd)
	{
	case IUI_SELECTFIRSTTASK:
		selectWhat = UIExtension::SelectTask::SelectFirstTask;
		break;

	case IUI_SELECTNEXTTASK:
		selectWhat = UIExtension::SelectTask::SelectNextTask;
		break;

	case IUI_SELECTNEXTTASKINCLCURRENT:
		selectWhat = UIExtension::SelectTask::SelectNextTaskInclCurrent;
		break;

	case IUI_SELECTPREVTASK:
		selectWhat = UIExtension::SelectTask::SelectPrevTask;
		break;

	case IUI_SELECTLASTTASK:
		selectWhat = UIExtension::SelectTask::SelectLastTask;
		break;

	default:
		return false;
	}

	String^ sWords = gcnew String(pSelect->szWords);

	return m_wnd->SelectTask(sWords, selectWhat, pSelect->bCaseSensitive, pSelect->bWholeWord, pSelect->bFindReplace);
}

bool CWordCloudUIExtensionBridgeWindow::GetLabelEditRect(LPRECT pEdit)
{
	return m_wnd->GetLabelEditRect((Int32&)pEdit->left, (Int32&)pEdit->top, (Int32&)pEdit->right, (Int32&)pEdit->bottom);
}

IUI_HITTEST CWordCloudUIExtensionBridgeWindow::HitTest(const POINT& ptScreen) const
{
	return UIExtension::Map(m_wnd->HitTest(ptScreen.x, ptScreen.y));
}

void CWordCloudUIExtensionBridgeWindow::SetUITheme(const UITHEME* pTheme)
{
	msclr::auto_gcroot<UITheme^> theme = gcnew UITheme(pTheme);

	m_wnd->SetUITheme(theme.get());
}

void CWordCloudUIExtensionBridgeWindow::SetReadOnly(bool bReadOnly)
{
	m_wnd->SetReadOnly(bReadOnly);
}

HWND CWordCloudUIExtensionBridgeWindow::GetHwnd() const
{
	return static_cast<HWND>(m_wnd->Handle.ToPointer());
}

void CWordCloudUIExtensionBridgeWindow::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const
{
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<String^> key = gcnew String(szKey);

	m_wnd->SavePreferences(prefs.get(), key.get());
}

void CWordCloudUIExtensionBridgeWindow::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly)
{
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<String^> key = gcnew String(szKey);

	m_wnd->LoadPreferences(prefs.get(), key.get(), bAppOnly);
}

