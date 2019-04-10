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

ContentControlWnd::ParentNotify::ParentNotify(IntPtr hwndParent) : m_hwndParent(NULL), m_hwndFrom(NULL)
{
	m_hwndParent = static_cast<HWND>(hwndParent.ToPointer());
}

ContentControlWnd::ParentNotify::ParentNotify(IntPtr hwndParent, IntPtr hwndFrom) : m_hwndParent(NULL), m_hwndFrom(NULL)
{
	m_hwndParent = static_cast<HWND>(hwndParent.ToPointer());
	m_hwndFrom = static_cast<HWND>(hwndFrom.ToPointer());
}

bool ContentControlWnd::ParentNotify::NotifyChange()
{
	if (!IsWindow(m_hwndParent))
		return false;

	::SendMessage(m_hwndParent, WM_ICC_CONTENTCHANGE, 0, (LPARAM)GetFrom());
	return true;
}

bool ContentControlWnd::ParentNotify::NotifyKillFocus()
{
	if (!IsWindow(m_hwndParent))
		return false;

	::SendMessage(m_hwndParent, WM_ICC_KILLFOCUS, 0, (LPARAM)GetFrom());
	return true;
}

String^ ContentControlWnd::ParentNotify::GetTaskLinkTooltip(String^ sLink)
{
	if (IsWindow(m_hwndParent) && sLink->StartsWith("tdl://"))
	{
		ICCTASKLINKTOOLTIP tt = { 0 };
		tt.szTaskLink = MS(ValidateLink(sLink));

		if (::SendMessage(m_hwndParent, WM_ICC_GETTASKLINKTOOLTIP, 0, (LPARAM)&tt))
			return gcnew String(tt.szTooltip);
	}

	// all else
	return gcnew String("");
}

bool ContentControlWnd::ParentNotify::NotifyFailedLink(String^ sFailedLink)
{
	if (!IsWindow(m_hwndParent))
		return false;

	sFailedLink = ValidateLink(sFailedLink);
	::SendMessage(m_hwndParent, WM_ICC_FAILEDLINK, (WPARAM)GetFrom(), (LPARAM)(LPCWSTR)MS(sFailedLink));

	return true;
}

bool ContentControlWnd::ParentNotify::NotifyTaskLink(String^ sTaskLink)
{
	if (!IsWindow(m_hwndParent))
		return false;

	sTaskLink = ValidateLink(sTaskLink);
	::SendMessage(m_hwndParent, WM_ICC_TASKLINK, 0, (LPARAM)(LPCWSTR)MS(sTaskLink));

	return true;
}

String^ ContentControlWnd::ParentNotify::ValidateLink(String^ sLink)
{
	// Outlook links
	if (sLink->StartsWith("outlook://") && !sLink->StartsWith("outlook:///"))
	{
		return sLink->Replace("outlook://", "outlook:///");
	}

	// Task links
	if (sLink->StartsWith("tdl:///"))
	{
		return sLink->Replace("tdl:///", "tdl://")->Trim('/');
	}
	else if (sLink->StartsWith("tdl://"))
	{
		return sLink->Trim('/');
	}

	// all else
	return sLink;
}

bool ContentControlWnd::ParentNotify::HasClipboard()
{
	if (!IsWindow(m_hwndParent))
		return false;

	::SendMessage(m_hwndParent, WM_ICC_HASCLIPBOARD, 0, (LPARAM)GetFrom());
	return true;
}

/*
bool ContentControlWnd::ParentNotify::GetClipboard()
{
	if (!IsWindow(m_hwndParent))
		return false;

	::SendMessage(m_hwndParent, WM_ICC_GETCLIPBOARD, 0, (LPARAM)GetFrom());
	return true;
}
*/

bool ContentControlWnd::ParentNotify::DoHelp(String^ sHelpKey)
{
	if (!IsWindow(m_hwndParent))
		return false;

	::SendMessage(m_hwndParent, WM_ICC_DOHELP, 0, (LPARAM)(LPCWSTR)MS(sHelpKey));
	return true;
}

bool ContentControlWnd::ParentNotify::NotifyWantSpellcheck()
{
	if (!IsWindow(m_hwndParent))
		return false;

	::SendMessage(m_hwndParent, WM_ICC_WANTSPELLCHECK, 0, (LPARAM)GetFrom());
	return true;
}

HWND ContentControlWnd::ParentNotify::GetFrom()
{
	return (m_hwndFrom ? m_hwndFrom : m_hwndParent);
}


