// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include <unknwn.h>
#include <tchar.h>

#include "stdafx.h"
#include "resource.h"
#include "TaskDatesUIExtensionBridge.h"

#include <Interfaces\ITasklist.h>
#include <Interfaces\ITransText.h>
#include <Interfaces\IPreferences.h>
#include <Interfaces\UITheme.h>

////////////////////////////////////////////////////////////////////////////////////////////////

#using <PluginHelpers.dll> as_friend

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::Drawing;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

using namespace TaskDatesUIExtension;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

// REPLACE THIS WITH NEW GUID!
const LPCWSTR TASKDATES_GUID = L"F9D3B948-24B0-4F69-AD95-7E18C099ADFD";
const LPCWSTR TASKDATES_NAME = L"Task Dates";

////////////////////////////////////////////////////////////////////////////////////////////////

CTaskDatesUIExtensionBridge::CTaskDatesUIExtensionBridge() : m_hIcon(NULL), m_pTT(nullptr)
{
	m_hIcon = Win32::LoadHIcon(L"TaskDatesUIExtensionBridge.dll", IDI_TASKDATES, 16, true);
}

void CTaskDatesUIExtensionBridge::Release()
{
	delete this;
}

void CTaskDatesUIExtensionBridge::SetLocalizer(ITransText* pTT)
{
	if (m_pTT == nullptr)
		m_pTT = pTT;
}

LPCWSTR CTaskDatesUIExtensionBridge::GetMenuText() const
{
	return TASKDATES_NAME;
}

HICON CTaskDatesUIExtensionBridge::GetIcon() const
{
	return m_hIcon;
}

LPCWSTR CTaskDatesUIExtensionBridge::GetTypeID() const
{
	return TASKDATES_GUID;
}

IUIExtensionWindow* CTaskDatesUIExtensionBridge::CreateExtWindow(UINT nCtrlID, 
	DWORD nStyle, long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
{
	auto pExtWnd = new CTaskDatesUIExtensionBridgeWindow(m_pTT);

	if (!pExtWnd->Create(nCtrlID, nStyle, nLeft, nTop, nWidth, nHeight, hwndParent))
	{
		delete pExtWnd;
		pExtWnd = NULL;
	}

	return pExtWnd;
}

void CTaskDatesUIExtensionBridge::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const
{
	// TODO
}

void CTaskDatesUIExtensionBridge::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey)
{
	// TODO
}

////////////////////////////////////////////////////////////////////////////////////////////////

CTaskDatesUIExtensionBridgeWindow::CTaskDatesUIExtensionBridgeWindow(ITransText* pTT) : m_pTT(pTT)
{

}

BOOL CTaskDatesUIExtensionBridgeWindow::Create(UINT nCtrlID, DWORD nStyle, 
	long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
{
	auto trans = gcnew Translator(m_pTT);
	auto typeID = gcnew String(TASKDATES_GUID);
	auto uiName = gcnew String(TASKDATES_NAME);
	auto parent = static_cast<IntPtr>(hwndParent);

	m_wnd = gcnew TaskDatesUIExtension::TaskDatesUIExtensionCore(typeID, uiName, parent, trans);

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

HICON CTaskDatesUIExtensionBridgeWindow::GetIcon() const
{
	return NULL;
}

LPCWSTR CTaskDatesUIExtensionBridgeWindow::GetMenuText() const
{
	return TASKDATES_NAME;
}

LPCWSTR CTaskDatesUIExtensionBridgeWindow::GetTypeID() const
{
	return TASKDATES_GUID;
}

bool CTaskDatesUIExtensionBridgeWindow::SelectTask(DWORD dwTaskID, bool /*bTaskLink*/)
{
	return m_wnd->SelectTask(dwTaskID);
}

bool CTaskDatesUIExtensionBridgeWindow::SelectTasks(const DWORD* pdwTaskIDs, int nTaskCount)
{
	auto taskIDs = gcnew array<UInt32>(nTaskCount);

	for (int i = 0; i < nTaskCount; i++)
		taskIDs[i] = pdwTaskIDs[i];

	return m_wnd->SelectTasks(taskIDs);
}

void CTaskDatesUIExtensionBridgeWindow::UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate)
{
	auto tasks = gcnew TaskList(pTasks);

	m_wnd->UpdateTasks(tasks, UIExtension::MapUpdateType(nUpdate));
}

bool CTaskDatesUIExtensionBridgeWindow::WantTaskUpdate(TDC_ATTRIBUTE nAttribID) const
{
	return m_wnd->WantTaskUpdate(Task::MapAttribute(nAttribID));
}

bool CTaskDatesUIExtensionBridgeWindow::PrepareNewTask(ITaskList* pTask) const
{
	auto task = gcnew TaskList(pTask);

	return m_wnd->PrepareNewTask(task->GetFirstTask());
}

bool CTaskDatesUIExtensionBridgeWindow::ProcessMessage(MSG* pMsg)
{
	return m_wnd->ProcessMessage(IntPtr(pMsg->hwnd),
								 pMsg->message,
								 pMsg->wParam,
								 pMsg->lParam,
								 pMsg->time,
								 pMsg->pt.x,
								 pMsg->pt.y);
}

void CTaskDatesUIExtensionBridgeWindow::FilterToolTipMessage(MSG* pMsg)
{
	m_wnd->FilterToolTipMessage(IntPtr(pMsg->hwnd),
								pMsg->message,
								pMsg->wParam,
								pMsg->lParam,
								pMsg->time,
								pMsg->pt.x,
								pMsg->pt.y);
}

bool CTaskDatesUIExtensionBridgeWindow::DoIdleProcessing()
{
	return m_wnd->DoIdleProcessing();
}


bool CTaskDatesUIExtensionBridgeWindow::DoAppCommand(IUI_APPCOMMAND nCmd, IUIAPPCOMMANDDATA* pData)
{
	switch (nCmd)
	{
	case IUI_SETFOCUS:
		return m_wnd->Focus();

	case IUI_GETNEXTTASK:
	case IUI_GETPREVTASK:
	case IUI_GETNEXTVISIBLETASK:
	case IUI_GETPREVVISIBLETASK:
	case IUI_GETNEXTTOPLEVELTASK:
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

	case IUI_SAVETOIMAGE:
		if (pData)
		{
			Bitmap^ image = m_wnd->SaveToImage();

			if (image != nullptr)
			{
				auto imagePath = gcnew String(pData->szFilePath);

				return UIExtension::SaveImageToFile(image, imagePath);
			}
		}

	case IUI_SCROLLTOSELECTEDTASK:
		return m_wnd->ScrollToSelectedTask();
	}

	return false;
}

DWORD CTaskDatesUIExtensionBridgeWindow::GetNextTask(IUI_APPCOMMAND nCmd, DWORD dwFromTaskID) const
{
	UIExtension::GetTask getTask;

	if (!UIExtension::MapGetTaskCmd(nCmd, getTask))
		return 0;

	UInt32 taskID = dwFromTaskID;

	if (!m_wnd->GetTask(getTask, taskID))
		return 0;

	return taskID;
}

bool CTaskDatesUIExtensionBridgeWindow::DoAppSelectCommand(IUI_APPCOMMAND nCmd, const IUISELECTTASK& select)
{
	UIExtension::SelectTask selectWhat;

	if (!UIExtension::MapSelectTaskCmd(nCmd, selectWhat))
		return false;

	String^ sWords = gcnew String(select.szWords);

	return m_wnd->SelectTask(sWords, selectWhat, select.bCaseSensitive, select.bWholeWord, select.bFindReplace);
}

bool CTaskDatesUIExtensionBridgeWindow::CanDoAppCommand(IUI_APPCOMMAND nCmd, const IUIAPPCOMMANDDATA* pData) const
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
	case IUI_GETPREVTASK:
	case IUI_GETNEXTVISIBLETASK:
	case IUI_GETPREVVISIBLETASK:
	case IUI_GETNEXTTOPLEVELTASK:
	case IUI_GETPREVTOPLEVELTASK:
		if (pData)
		{
			DWORD dwTaskID = GetNextTask(nCmd, pData->dwTaskID);
			return ((dwTaskID != 0) && (dwTaskID != pData->dwTaskID));
		}
		break;

	case IUI_SAVETOIMAGE:
		return m_wnd->CanSaveToImage();

	case IUI_SCROLLTOSELECTEDTASK:
		return m_wnd->CanScrollToSelectedTask();
	}

	return false;
}

bool CTaskDatesUIExtensionBridgeWindow::GetLabelEditRect(LPRECT pEdit)
{
	return m_wnd->GetLabelEditRect((Int32&)pEdit->left, (Int32&)pEdit->top, (Int32&)pEdit->right, (Int32&)pEdit->bottom);
}

bool CTaskDatesUIExtensionBridgeWindow::HitTest(POINT ptScreen, IUIHITTEST& hitTest) const
{
	auto ht = gcnew UIExtension::HitTest();

	if (!m_wnd->HitTest(ptScreen.x, ptScreen.y, ht))
		return false;

	hitTest.dwTaskID = ht->taskId;
	hitTest.nResult = UIExtension::MapHitTestResult(ht->result);

	return true;
}

bool CTaskDatesUIExtensionBridgeWindow::ShowContextMenu(POINT ptScreen)
{
	return m_wnd->ShowContextMenu(ptScreen.x, ptScreen.y);
}

void CTaskDatesUIExtensionBridgeWindow::SetUITheme(const UITHEME* pTheme)
{
	auto theme = gcnew UITheme(pTheme);

	m_wnd->SetUITheme(theme);
}

void CTaskDatesUIExtensionBridgeWindow::SetReadOnly(bool bReadOnly)
{
	m_wnd->SetReadOnly(bReadOnly);
}

void CTaskDatesUIExtensionBridgeWindow::SetTaskFont(HFONT hFont)
{
	m_wnd->SetTaskFont(Win32::GetFaceName(hFont), Win32::GetPointSize(hFont));
}

HWND CTaskDatesUIExtensionBridgeWindow::GetHwnd() const
{
	return static_cast<HWND>(m_wnd->Handle.ToPointer());
}

void CTaskDatesUIExtensionBridgeWindow::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const
{
	auto prefs = gcnew Preferences(pPrefs);
	auto key = gcnew String(szKey);

	m_wnd->SavePreferences(prefs, key);
}

void CTaskDatesUIExtensionBridgeWindow::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly)
{
	auto prefs = gcnew Preferences(pPrefs);
	auto key = gcnew String(szKey);

	m_wnd->LoadPreferences(prefs, key, bAppOnly);
}

