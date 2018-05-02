// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include <unknwn.h>
#include <tchar.h>
#include <msclr\auto_gcroot.h>

#include "stdafx.h"
#include "resource.h"
#include "WorkloadUIExtensionBridge.h"

#include "..\..\..\..\ToDoList_Dev\Interfaces\ITasklist.h"
#include "..\..\..\..\ToDoList_Dev\Interfaces\ITransText.h"
#include "..\..\..\..\ToDoList_Dev\Interfaces\IPreferences.h"
#include "..\..\..\..\ToDoList_Dev\Interfaces\UITheme.h"

////////////////////////////////////////////////////////////////////////////////////////////////

#using <PluginHelpers.dll> as_friend

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::Drawing;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

using namespace WorkloadUIExtension;
using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

// REPLACE THIS WITH NEW GUID!
const LPCWSTR WORKLOAD_GUID = L"3872BFCB-2512-439F-8C63-DF72C0C3844C";
const LPCWSTR WORKLOAD_NAME = L"Workload";

////////////////////////////////////////////////////////////////////////////////////////////////

CWorkloadUIExtensionBridge::CWorkloadUIExtensionBridge() : m_hIcon(NULL), m_pTT(nullptr)
{
	HMODULE hMod = LoadLibrary(L"WorkloadUIExtensionBridge.dll"); // us

	m_hIcon = ::LoadIcon(hMod, MAKEINTRESOURCE(IDI_WORKLOAD));
}

void CWorkloadUIExtensionBridge::Release()
{
	delete this;
}

void CWorkloadUIExtensionBridge::SetLocalizer(ITransText* pTT)
{
	if (m_pTT == nullptr)
		m_pTT = pTT;
}

LPCWSTR CWorkloadUIExtensionBridge::GetMenuText() const
{
	return WORKLOAD_NAME;
}

HICON CWorkloadUIExtensionBridge::GetIcon() const
{
	return m_hIcon;
}

LPCWSTR CWorkloadUIExtensionBridge::GetTypeID() const
{
	return WORKLOAD_GUID;
}

IUIExtensionWindow* CWorkloadUIExtensionBridge::CreateExtWindow(UINT nCtrlID, 
	DWORD nStyle, long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
{
	CWorkloadUIExtensionBridgeWindow* pExtWnd = new CWorkloadUIExtensionBridgeWindow(m_pTT);

	if (!pExtWnd->Create(nCtrlID, nStyle, nLeft, nTop, nWidth, nHeight, hwndParent))
	{
		pExtWnd->Release();
		pExtWnd = NULL;
	}

	return pExtWnd;
}

void CWorkloadUIExtensionBridge::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const
{
	// TODO
}

void CWorkloadUIExtensionBridge::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey)
{
	// TODO
}

////////////////////////////////////////////////////////////////////////////////////////////////

CWorkloadUIExtensionBridgeWindow::CWorkloadUIExtensionBridgeWindow(ITransText* pTT) : m_pTT(pTT)
{

}

void CWorkloadUIExtensionBridgeWindow::Release()
{
	::DestroyWindow(GetHwnd());
	delete this;
}

BOOL CWorkloadUIExtensionBridgeWindow::Create(UINT nCtrlID, DWORD nStyle, 
	long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
{
	msclr::auto_gcroot<Translator^> trans = gcnew Translator(m_pTT);

	m_wnd = gcnew WorkloadUIExtension::WorkloadUIExtensionCore(static_cast<IntPtr>(hwndParent), trans.get());

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

HICON CWorkloadUIExtensionBridgeWindow::GetIcon() const
{
	return NULL;
}

LPCWSTR CWorkloadUIExtensionBridgeWindow::GetMenuText() const
{
	return WORKLOAD_NAME;
}

LPCWSTR CWorkloadUIExtensionBridgeWindow::GetTypeID() const
{
	return WORKLOAD_GUID;
}

bool CWorkloadUIExtensionBridgeWindow::SelectTask(DWORD dwTaskID)
{
	return m_wnd->SelectTask(dwTaskID);
}

bool CWorkloadUIExtensionBridgeWindow::SelectTasks(const DWORD* pdwTaskIDs, int nTaskCount)
{
	array<UInt32>^ taskIDs = gcnew array<UInt32>(nTaskCount);

	for (int i = 0; i < nTaskCount; i++)
		taskIDs[i] = pdwTaskIDs[i];

	return m_wnd->SelectTasks(taskIDs);
}

void CWorkloadUIExtensionBridgeWindow::UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, const IUI_ATTRIBUTE* pAttributes, int nNumAttributes)
{
	msclr::auto_gcroot<TaskList^> tasks = gcnew TaskList(pTasks);

	m_wnd->UpdateTasks(tasks.get(), UIExtension::Map(nUpdate), UIExtension::Map(pAttributes, nNumAttributes));
}

bool CWorkloadUIExtensionBridgeWindow::WantTaskUpdate(IUI_ATTRIBUTE nAttribute) const
{
	return m_wnd->WantTaskUpdate(UIExtension::Map(nAttribute));
}

bool CWorkloadUIExtensionBridgeWindow::PrepareNewTask(ITaskList* pTask) const
{
	msclr::auto_gcroot<TaskList^> task = gcnew TaskList(pTask);

	return m_wnd->PrepareNewTask(task.get()->GetFirstTask());
}

bool CWorkloadUIExtensionBridgeWindow::ProcessMessage(MSG* pMsg)
{
	return m_wnd->ProcessMessage(IntPtr(pMsg->hwnd), 
										pMsg->message, 
										pMsg->wParam, 
										pMsg->lParam, 
										pMsg->time, 
										pMsg->pt.x,
										pMsg->pt.y);
}

bool CWorkloadUIExtensionBridgeWindow::DoAppCommand(IUI_APPCOMMAND nCmd, IUIAPPCOMMANDDATA* pData)
{
	switch (nCmd)
	{
	case IUI_COLLAPSEALL:
		return m_wnd->Expand(WorkloadControl::ExpandNode::CollapseAll);

	case IUI_COLLAPSESELECTED:
		return m_wnd->Expand(WorkloadControl::ExpandNode::CollapseSelection);

	case IUI_EXPANDALL:
		return m_wnd->Expand(WorkloadControl::ExpandNode::ExpandAll);

	case IUI_EXPANDSELECTED:
		return m_wnd->Expand(WorkloadControl::ExpandNode::ExpandSelection);

	case IUI_SELECTTASK:
		if (pData)
			return m_wnd->SelectTask(pData->dwTaskID);
		break;

	case IUI_SETFOCUS:
		return m_wnd->Focus();

	case IUI_MOVETASK:
		if (pData)
		{
			return m_wnd->MoveTask(pData->move.dwSelectedTaskID, pData->move.dwParentID, pData->move.dwAfterSiblingID);
		}
		return true;

	case IUI_GETNEXTTASK:
	case IUI_GETPREVTASK:
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

	case IUI_SAVETOIMAGE:
		if (pData)
		{
			Bitmap^ image = m_wnd->SaveToImage();

			if (image != nullptr)
			{
				// Always save as png
				msclr::auto_gcroot<String^> sImagePath = gcnew String(pData->szFilePath);
				sImagePath = System::IO::Path::ChangeExtension(sImagePath.get(), ".png");

				image->Save(sImagePath.get(), System::Drawing::Imaging::ImageFormat::Png);

				MarshalledString temp(sImagePath.get());
				lstrcpyn(pData->szFilePath, temp, MAX_PATH);

				return true;
			}
		}
	}

	return false;
}

DWORD CWorkloadUIExtensionBridgeWindow::GetNextTask(IUI_APPCOMMAND nCmd, DWORD dwFromTaskID) const
{
	UIExtension::GetTask getTask;

	switch (nCmd)
	{
		case IUI_GETNEXTTASK:
			getTask = UIExtension::GetTask::GetNextTask;
			break;

		case IUI_GETPREVTASK:
			getTask = UIExtension::GetTask::GetPrevTask;
			break;

		case IUI_GETNEXTTOPLEVELTASK:
			getTask = UIExtension::GetTask::GetNextTopLevelTask;
			break;

		case IUI_GETPREVTOPLEVELTASK:
			getTask = UIExtension::GetTask::GetPrevTopLevelTask;
			break;

		default:
			return 0;
	}

	UInt32 taskID = dwFromTaskID;

	if (m_wnd->GetTask(getTask, taskID))
		return taskID;

	return 0;
}

bool CWorkloadUIExtensionBridgeWindow::CanDoAppCommand(IUI_APPCOMMAND nCmd, const IUIAPPCOMMANDDATA* pData) const
{
	switch (nCmd)
	{
	case IUI_COLLAPSEALL:
		return m_wnd->CanExpand(WorkloadControl::ExpandNode::CollapseAll);

	case IUI_COLLAPSESELECTED:
		return m_wnd->CanExpand(WorkloadControl::ExpandNode::CollapseSelection);

	case IUI_EXPANDALL:
		return m_wnd->CanExpand(WorkloadControl::ExpandNode::ExpandAll);

	case IUI_EXPANDSELECTED:
		return m_wnd->CanExpand(WorkloadControl::ExpandNode::ExpandSelection);

	case IUI_SELECTTASK:
		return true;

	case IUI_SETFOCUS:
		return m_wnd->Focused;

	case IUI_MOVETASK:
		if (pData)
			return m_wnd->CanMoveTask(pData->move.dwSelectedTaskID, pData->move.dwParentID, pData->move.dwAfterSiblingID);
		break;

	case IUI_GETNEXTTASK:
	case IUI_GETPREVTASK:
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
 	}

	return false;
}

bool CWorkloadUIExtensionBridgeWindow::GetLabelEditRect(LPRECT pEdit)
{
	return m_wnd->GetLabelEditRect((Int32&)pEdit->left, (Int32&)pEdit->top, (Int32&)pEdit->right, (Int32&)pEdit->bottom);
}

IUI_HITTEST CWorkloadUIExtensionBridgeWindow::HitTest(const POINT& ptScreen) const
{
	return UIExtension::Map(m_wnd->HitTest(ptScreen.x, ptScreen.y));
}

void CWorkloadUIExtensionBridgeWindow::SetUITheme(const UITHEME* pTheme)
{
	msclr::auto_gcroot<UITheme^> theme = gcnew UITheme(pTheme);

	m_wnd->SetUITheme(theme.get());
}

void CWorkloadUIExtensionBridgeWindow::SetReadOnly(bool bReadOnly)
{
	m_wnd->SetReadOnly(bReadOnly);
}

HWND CWorkloadUIExtensionBridgeWindow::GetHwnd() const
{
	return static_cast<HWND>(m_wnd->Handle.ToPointer());
}

void CWorkloadUIExtensionBridgeWindow::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const
{
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<String^> key = gcnew String(szKey);

	m_wnd->SavePreferences(prefs.get(), key.get());
}

void CWorkloadUIExtensionBridgeWindow::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly)
{
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<String^> key = gcnew String(szKey);

	m_wnd->LoadPreferences(prefs.get(), key.get(), bAppOnly);
}

