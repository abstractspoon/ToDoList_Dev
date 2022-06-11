// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "ContentControl.h"
#include "UrlParser.h"
#include "OutlookUtil.h"
#include "Win32.h"

#include <Interfaces\UITheme.h>
#include <Interfaces\IContentControl.h>

#include <Shared\FileMisc.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::Diagnostics;

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

bool ContentControlWnd::ParentNotify::NotifySimple(UINT nMsg)
{
	if (!IsWindow(m_hwndParent))
		return false;

	::SendMessage(m_hwndParent, nMsg, 0, (LPARAM)GetFrom());
	return true;
}

bool ContentControlWnd::ParentNotify::NotifyChange()
{
	return NotifySimple(WM_ICC_CONTENTCHANGE);
}

bool ContentControlWnd::ParentNotify::NotifyKillFocus()
{
	return NotifySimple(WM_ICC_KILLFOCUS);
}

String^ ContentControlWnd::ParentNotify::GetLinkTooltip(String^ sLink)
{
	if (IsWindow(m_hwndParent))
	{
		MarshalledString link(ValidateLink(sLink));
		LPCWSTR szTitle = (LPCWSTR)::SendMessage(m_hwndParent, WM_ICC_GETLINKTOOLTIP, 0, (LPARAM)(LPCWSTR)link);

		if (szTitle)
			return gcnew String(szTitle);
	}

	// all else
	return gcnew String("");
}

List<String^>^ ContentControlWnd::ParentNotify::GetAttributeValues(Task::Attribute attrib)
{
	if (IsWindow(m_hwndParent))
	{
		TDC_ATTRIBUTE nAttrib = Task::MapAttribute(attrib);
		LPCWSTR szList = (LPCWSTR)::SendMessage(m_hwndParent, WM_ICC_GETATTRIBUTELIST, nAttrib, (LPARAM)'\n');

		String^ attribs = gcnew String(szList);

		if (!String::IsNullOrWhiteSpace(attribs))
			return gcnew List<String^>(attribs->Split('\n'));
	}

	// all else
	return gcnew List<String^>();
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
	return NotifySimple(WM_ICC_HASCLIPBOARD);
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
	return NotifySimple(WM_ICC_WANTSPELLCHECK);
}

HWND ContentControlWnd::ParentNotify::GetFrom()
{
	return (m_hwndFrom ? m_hwndFrom : m_hwndParent);
}

bool ContentControlWnd::GoToLink(String^ sLink, IntPtr hwndParent, IntPtr hwndFrom)
{
	ParentNotify notify(hwndParent, hwndFrom);
	UrlParser parser;

	int nProtocol = parser.MatchProtocol(sLink);

	// If it's an unknown protocol OR our parent wants to handle it
	// just forward it on
	if ((nProtocol == -1) || parser.ProtocolWantsNotification(nProtocol))
	{
		notify.NotifyFailedLink(sLink);
		return true;
	}

	// Handle Outlook manually because under Windows 10 ShellExecute 
	// will succeed even if Outlook is not installed
	if (OutlookUtil::IsOutlookUrl(sLink))
	{
		if (OutlookUtil::HandleUrl(sLink, hwndParent))
			return true;
	}
	else
	{
		if (parser.IsFileProtocol(sLink))
			sLink = parser.GetUrlAsFile(sLink);

		if (FileMisc::Run(Win32::GetHwnd(hwndParent), MS(sLink)) >= SE_ERR_SUCCESS)
			return true;
	}

	// All else forward to parent as failures
	notify.NotifyFailedLink(sLink);
	return false;
}

String^ ContentControlWnd::HandleNeedLinkTooltip(String^ sLink, IntPtr hwndParent, IntPtr hwndFrom)
{
	auto notify = gcnew ContentControlWnd::ParentNotify(hwndParent, hwndFrom);

	return notify->GetLinkTooltip(sLink);
}

List<String^>^ ContentControlWnd::HandleNeedAttributeList(Task::Attribute attrib, IntPtr hwndParent, IntPtr hwndFrom)
{
	auto notify = gcnew ContentControlWnd::ParentNotify(hwndParent, hwndFrom);
	
	return notify->GetAttributeValues(attrib);
}
