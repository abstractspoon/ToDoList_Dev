// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include <unknwn.h>
#include <tchar.h>
#include <msclr\auto_gcroot.h>

#include "stdafx.h"
#include "resource.h"
#include "MindMapUIExtensionBridge.h"

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

using namespace MindMapUIExtension;
using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

// REPLACE THIS WITH NEW GUID!
const LPCWSTR MINDMAP_GUID = L"14FC02B5-2DE0-4A2F-BA6A-50A66551864E";
const LPCWSTR MINDMAP_NAME = L"Mind Map";

////////////////////////////////////////////////////////////////////////////////////////////////

CMindMapUIExtensionBridge::CMindMapUIExtensionBridge() : m_hIcon(NULL), m_pTT(nullptr)
{
	HMODULE hMod = LoadLibrary(L"MindMapUIExtensionBridge.dll"); // us

	m_hIcon = (HICON)::LoadImage(hMod, MAKEINTRESOURCE(IDI_MINDMAP), IMAGE_ICON, 16, 16, LR_LOADMAP3DCOLORS);
}

void CMindMapUIExtensionBridge::Release()
{
	delete this;
}

void CMindMapUIExtensionBridge::SetLocalizer(ITransText* pTT)
{
	if (m_pTT == nullptr)
		m_pTT = pTT;
}

LPCWSTR CMindMapUIExtensionBridge::GetMenuText() const
{
	return MINDMAP_NAME;
}

HICON CMindMapUIExtensionBridge::GetIcon() const
{
	return m_hIcon;
}

LPCWSTR CMindMapUIExtensionBridge::GetTypeID() const
{
	return MINDMAP_GUID;
}

IUIExtensionWindow* CMindMapUIExtensionBridge::CreateExtWindow(UINT nCtrlID, 
	DWORD nStyle, long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
{
	CMindMapUIExtensionBridgeWindow* pExtWnd = new CMindMapUIExtensionBridgeWindow(m_pTT);

	if (!pExtWnd->Create(nCtrlID, nStyle, nLeft, nTop, nWidth, nHeight, hwndParent))
	{
		pExtWnd->Release();
		pExtWnd = NULL;
	}

	return pExtWnd;
}

void CMindMapUIExtensionBridge::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const
{
	// TODO
}

void CMindMapUIExtensionBridge::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey)
{
	// TODO
}

////////////////////////////////////////////////////////////////////////////////////////////////

CMindMapUIExtensionBridgeWindow::CMindMapUIExtensionBridgeWindow(ITransText* pTT) : m_pTT(pTT)
{

}

void CMindMapUIExtensionBridgeWindow::Release()
{
	::DestroyWindow(GetHwnd());
	delete this;
}

BOOL CMindMapUIExtensionBridgeWindow::Create(UINT nCtrlID, DWORD nStyle, 
	long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
{
	msclr::auto_gcroot<Translator^> trans = gcnew Translator(m_pTT);
	msclr::auto_gcroot<String^> typeID = gcnew String(MINDMAP_GUID);
	msclr::auto_gcroot<String^> uiName = gcnew String(MINDMAP_NAME);

	m_wnd = gcnew MindMapUIExtension::MindMapUIExtensionCore(typeID.get(), uiName.get(), static_cast<IntPtr>(hwndParent), trans.get());

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

HICON CMindMapUIExtensionBridgeWindow::GetIcon() const
{
	return NULL;
}

LPCWSTR CMindMapUIExtensionBridgeWindow::GetMenuText() const
{
	return MINDMAP_NAME;
}

LPCWSTR CMindMapUIExtensionBridgeWindow::GetTypeID() const
{
	return MINDMAP_GUID;
}

bool CMindMapUIExtensionBridgeWindow::SelectTask(DWORD dwTaskID, bool bTaskLink)
{
	return m_wnd->SelectTask(dwTaskID);
}

bool CMindMapUIExtensionBridgeWindow::SelectTasks(const DWORD* pdwTaskIDs, int nTaskCount)
{
	array<UInt32>^ taskIDs = gcnew array<UInt32>(nTaskCount);

	for (int i = 0; i < nTaskCount; i++)
		taskIDs[i] = pdwTaskIDs[i];

	return m_wnd->SelectTasks(taskIDs);
}

void CMindMapUIExtensionBridgeWindow::UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate)
{
	msclr::auto_gcroot<TaskList^> tasks = gcnew TaskList(pTasks);

	m_wnd->UpdateTasks(tasks.get(), UIExtension::MapUpdateType(nUpdate));
}

bool CMindMapUIExtensionBridgeWindow::WantTaskUpdate(TDC_ATTRIBUTE nAttribute) const
{
	return m_wnd->WantTaskUpdate(Task::MapAttribute(nAttribute));
}

bool CMindMapUIExtensionBridgeWindow::PrepareNewTask(ITaskList* pTask) const
{
	msclr::auto_gcroot<TaskList^> task = gcnew TaskList(pTask);

	return m_wnd->PrepareNewTask(task.get()->GetFirstTask());
}

bool CMindMapUIExtensionBridgeWindow::ProcessMessage(MSG* pMsg)
{
	return m_wnd->ProcessMessage(IntPtr(pMsg->hwnd), 
										pMsg->message, 
										pMsg->wParam, 
										pMsg->lParam, 
										pMsg->time, 
										pMsg->pt.x,
										pMsg->pt.y);
}

bool CMindMapUIExtensionBridgeWindow::Map(IUI_APPCOMMAND nCmd, MindMapControl::ExpandNode% expand)
{
	switch (nCmd)
	{
	case IUI_COLLAPSEALL:
		expand = MindMapControl::ExpandNode::CollapseAll;
		return true;

	case IUI_COLLAPSESELECTED:
		expand = MindMapControl::ExpandNode::CollapseSelection;
		return true;

	case IUI_EXPANDALL:
		expand = MindMapControl::ExpandNode::ExpandAll;
		return true;

	case IUI_EXPANDSELECTED:
		expand = MindMapControl::ExpandNode::ExpandSelectionAll;
		return true;
	}

	// all else
	return false;
}

bool CMindMapUIExtensionBridgeWindow::DoAppCommand(IUI_APPCOMMAND nCmd, IUIAPPCOMMANDDATA* pData)
{
	switch (nCmd)
	{
	case IUI_COLLAPSEALL:
	case IUI_COLLAPSESELECTED:
	case IUI_EXPANDALL:
	case IUI_EXPANDSELECTED:
		{
			MindMapControl::ExpandNode expand;

			if (Map(nCmd, expand))
				return m_wnd->Expand(expand);
		}
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
				msclr::auto_gcroot<String^> sImagePath = gcnew String(pData->szFilePath);

				return UIExtension::SaveImageToFile(image, sImagePath.get());
			}
		}

	case IUI_SCROLLTOSELECTEDTASK:
		return m_wnd->ScrollToSelectedTask();
	}

	return false;
}

DWORD CMindMapUIExtensionBridgeWindow::GetNextTask(IUI_APPCOMMAND nCmd, DWORD dwFromTaskID) const
{
	UIExtension::GetTask getTask;

	if (!UIExtension::MapGetTaskCmd(nCmd, getTask))
		return 0;

	UInt32 taskID = dwFromTaskID;

	if (!m_wnd->GetTask(getTask, taskID))
		return 0;

	return taskID;
}

bool CMindMapUIExtensionBridgeWindow::DoAppSelectCommand(IUI_APPCOMMAND nCmd, const IUISELECTTASK& select)
{
	UIExtension::SelectTask selectWhat;

	if (!UIExtension::MapSelectTaskCmd(nCmd, selectWhat))
		return false;

	String^ sWords = gcnew String(select.szWords);

	return m_wnd->SelectTask(sWords, selectWhat, select.bCaseSensitive, select.bWholeWord, select.bFindReplace);
}

bool CMindMapUIExtensionBridgeWindow::CanDoAppCommand(IUI_APPCOMMAND nCmd, const IUIAPPCOMMANDDATA* pData) const
{
	switch (nCmd)
	{
	case IUI_COLLAPSEALL:
	case IUI_COLLAPSESELECTED:
	case IUI_EXPANDALL:
	case IUI_EXPANDSELECTED:
		{
			MindMapControl::ExpandNode expand;

			if (Map(nCmd, expand))
				return m_wnd->CanExpand(expand);
		}
		break;

	case IUI_SELECTFIRSTTASK:
	case IUI_SELECTNEXTTASK:
	case IUI_SELECTNEXTTASKINCLCURRENT:
	case IUI_SELECTPREVTASK:
	case IUI_SELECTLASTTASK:
		return true;

	case IUI_SETFOCUS:
		return !m_wnd->Focused;

	case IUI_MOVETASK:
		if (pData)
			return m_wnd->CanMoveTask(pData->move.dwSelectedTaskID, pData->move.dwParentID, pData->move.dwAfterSiblingID);
		break;

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

bool CMindMapUIExtensionBridgeWindow::GetLabelEditRect(LPRECT pEdit)
{
	return m_wnd->GetLabelEditRect((Int32&)pEdit->left, (Int32&)pEdit->top, (Int32&)pEdit->right, (Int32&)pEdit->bottom);
}

IUI_HITTEST CMindMapUIExtensionBridgeWindow::HitTest(POINT ptScreen, IUI_HITTESTREASON nReason) const
{
	return UIExtension::MapHitTestResult(m_wnd->HitTest(ptScreen.x, ptScreen.y, UIExtension::MapHitTestReason(nReason)));
}

DWORD CMindMapUIExtensionBridgeWindow::HitTestTask(POINT ptScreen, IUI_HITTESTREASON nReason) const
{
	return m_wnd->HitTestTask(ptScreen.x, ptScreen.y, UIExtension::MapHitTestReason(nReason));
}

void CMindMapUIExtensionBridgeWindow::SetUITheme(const UITHEME* pTheme)
{
	msclr::auto_gcroot<UITheme^> theme = gcnew UITheme(pTheme);

	m_wnd->SetUITheme(theme.get());
}

void CMindMapUIExtensionBridgeWindow::SetReadOnly(bool bReadOnly)
{
	m_wnd->SetReadOnly(bReadOnly);
}

void CMindMapUIExtensionBridgeWindow::SetTaskFont(HFONT hFont)
{
	m_wnd->SetTaskFont(Win32::GetFaceName(hFont), Win32::GetPointSize(hFont));
}

HWND CMindMapUIExtensionBridgeWindow::GetHwnd() const
{
	return static_cast<HWND>(m_wnd->Handle.ToPointer());
}

void CMindMapUIExtensionBridgeWindow::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const
{
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<String^> key = gcnew String(szKey);

	m_wnd->SavePreferences(prefs.get(), key.get());
}

void CMindMapUIExtensionBridgeWindow::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly)
{
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<String^> key = gcnew String(szKey);

	m_wnd->LoadPreferences(prefs.get(), key.get(), bAppOnly);
}

