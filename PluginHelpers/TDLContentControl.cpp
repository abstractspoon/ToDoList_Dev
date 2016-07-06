// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "TDLContentControl.h"

#include "..\..\Interfaces\UITheme.h"
#include "..\..\Interfaces\IContentControl.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace TDLPluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

TDLContentControl::TDLNotify::TDLNotify(IntPtr hwndParent) : m_hwndParent(NULL), m_hwndFrom(NULL)
{
	m_hwndParent = static_cast<HWND>(hwndParent.ToPointer());
}

TDLContentControl::TDLNotify::TDLNotify(IntPtr hwndParent, IntPtr hwndFrom) : m_hwndParent(NULL), m_hwndFrom(NULL)
{
	m_hwndParent = static_cast<HWND>(hwndParent.ToPointer());
	m_hwndFrom = static_cast<HWND>(hwndFrom.ToPointer());
}

bool TDLContentControl::TDLNotify::NotifyChange()
{
	if (!IsWindow(m_hwndParent))
		return false;

	::SendMessage(m_hwndParent, WM_ICC_COMMENTSCHANGE, 0, (LPARAM)m_hwndFrom);
	return true;
}

bool TDLContentControl::TDLNotify::NotifyKillFocus()
{
	if (!IsWindow(m_hwndParent))
		return false;

	::SendMessage(m_hwndParent, WM_ICC_COMMENTSKILLFOCUS, 0, (LPARAM)m_hwndFrom);
	return true;
}
