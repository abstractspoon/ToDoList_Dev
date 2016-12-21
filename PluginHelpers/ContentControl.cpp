// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "ContentControl.h"

#include "..\..\ToDoList_Dev\Interfaces\UITheme.h"
#include "..\..\ToDoList_Dev\Interfaces\IContentControl.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

ContentControl::ParentNotify::ParentNotify(IntPtr hwndParent) : m_hwndParent(NULL), m_hwndFrom(NULL)
{
	m_hwndParent = static_cast<HWND>(hwndParent.ToPointer());
}

ContentControl::ParentNotify::ParentNotify(IntPtr hwndParent, IntPtr hwndFrom) : m_hwndParent(NULL), m_hwndFrom(NULL)
{
	m_hwndParent = static_cast<HWND>(hwndParent.ToPointer());
	m_hwndFrom = static_cast<HWND>(hwndFrom.ToPointer());
}

bool ContentControl::ParentNotify::NotifyChange()
{
	if (!IsWindow(m_hwndParent))
		return false;

	::SendMessage(m_hwndParent, WM_ICC_COMMENTSCHANGE, 0, (LPARAM)m_hwndFrom);
	return true;
}

bool ContentControl::ParentNotify::NotifyKillFocus()
{
	if (!IsWindow(m_hwndParent))
		return false;

	::SendMessage(m_hwndParent, WM_ICC_COMMENTSKILLFOCUS, 0, (LPARAM)m_hwndFrom);
	return true;
}
