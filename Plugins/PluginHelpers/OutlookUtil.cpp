
#include "stdafx.h"

#include "OutlookUtil.h"
#include "DragDropUtil.h"
#include "PluginHelpers.h"
#include "Win32.h"

#include <Shared\MSOutlookHelper.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Microsoft::VisualStudio::OLE::Interop;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

#define CF_RENPRIVATEMESSAGES_STR  L"RenPrivateMessages" 
#define CF_RENPRIVATESRCFOLDER_STR L"RenPrivateSourceFolder"
#define CF_FILEDESCRIPTORW_STR     L"FileGroupDescriptorW"
#define CF_FILECONTENTS_STR        L"FileContents"

////////////////////////////////////////////////////////////////////////////////////////////////

static const CLIPFORMAT CF_RENPRIVATEMESSAGES	= (CLIPFORMAT)::RegisterClipboardFormat(CF_RENPRIVATEMESSAGES_STR);
static const CLIPFORMAT CF_RENPRIVATESRCFOLDER	= (CLIPFORMAT)::RegisterClipboardFormat(CF_RENPRIVATESRCFOLDER_STR);
static const CLIPFORMAT CF_FILECONTENTS			= (CLIPFORMAT)::RegisterClipboardFormat(CF_FILECONTENTS_STR);
static const CLIPFORMAT CF_FILEDESCRIPTOR		= (CLIPFORMAT)::RegisterClipboardFormat(CF_FILEDESCRIPTORW_STR);

////////////////////////////////////////////////////////////////////////////////////////////////

bool OutlookUtil::IsOutlookItem(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj)
{
	return (DragDropUtil::ObjectHasFormat(obj, CF_RENPRIVATEMESSAGES) &&
			DragDropUtil::ObjectHasFormat(obj, CF_RENPRIVATESRCFOLDER) &&
			DragDropUtil::ObjectHasFormat(obj, CF_FILECONTENTS) &&
			DragDropUtil::ObjectHasFormat(obj, CF_FILEDESCRIPTOR));
}

String^ OutlookUtil::FormatItemAsUrl(String^ id)
{
	return String::Format(L"outlook:{0}", id);
}

bool OutlookUtil::HandleUrl(String^ url, IntPtr hwnd)
{
	return (CMSOutlookHelper::HandleUrl(Win32::GetHwnd(hwnd), MS(url)) != FALSE);
}

bool OutlookUtil::IsOutlookUrl(String^ url)
{
	return (CMSOutlookHelper::IsOutlookUrl(MS(url)) != FALSE);
}

bool OutlookUtil::IsOutlookInstalled()
{
	return (CMSOutlookHelper::IsOutlookInstalled() != FALSE);
}

