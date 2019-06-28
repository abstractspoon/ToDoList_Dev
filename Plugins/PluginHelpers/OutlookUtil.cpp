
#include "stdafx.h"

#include "OutlookUtil.h"
#include "DragDropUtil.h"
#include "PluginHelpers.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Microsoft::VisualStudio::OLE::Interop;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

#define CFSTR_RENPRIVATEMESSAGES  L"RenPrivateMessages" 
#define CFSTR_RENPRIVATESRCFOLDER L"RenPrivateSourceFolder"
#define CFSTR_FILEDESCRIPTORW     L"FileGroupDescriptorW"
#define CFSTR_FILECONTENTS        L"FileContents"

////////////////////////////////////////////////////////////////////////////////////////////////

static const CLIPFORMAT CF_RENPRIVATEMESSAGES = (CLIPFORMAT)::RegisterClipboardFormat(CFSTR_RENPRIVATEMESSAGES);
static const CLIPFORMAT CF_RENPRIVATESRCFOLDER = (CLIPFORMAT)::RegisterClipboardFormat(CFSTR_RENPRIVATESRCFOLDER);
static const CLIPFORMAT CF_FILECONTENTS = (CLIPFORMAT)::RegisterClipboardFormat(CFSTR_FILECONTENTS);
static const CLIPFORMAT CF_FILEDESCRIPTOR = (CLIPFORMAT)::RegisterClipboardFormat(CFSTR_FILEDESCRIPTORW);

////////////////////////////////////////////////////////////////////////////////////////////////

bool OutlookUtil::IsOutlookItem(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj)
{
	return (DragDropUtil::ObjectHasFormat(obj, CF_RENPRIVATEMESSAGES) &&
			DragDropUtil::ObjectHasFormat(obj, CF_RENPRIVATESRCFOLDER) &&
			DragDropUtil::ObjectHasFormat(obj, CF_FILECONTENTS) &&
			DragDropUtil::ObjectHasFormat(obj, CF_FILEDESCRIPTOR));
}

String^ OutlookUtil::FormatItemAsUrl(String^ title, String^ id, bool forceId)
{
	// TODO
	return String::Format(L"outlook:{0}", id);
}

