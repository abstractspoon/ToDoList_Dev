
#include "stdafx.h"
#include "DragDropUtil.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Microsoft::VisualStudio::OLE::Interop;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

#define CFSTR_RENPRIVATEMESSAGES  L"RenPrivateMessages" 
#define CFSTR_RENPRIVATESRCFOLDER L"RenPrivateSourceFolder"
#define CFSTR_FILEDESCRIPTORW     L"FileGroupDescriptorW"
#define CFSTR_FILECONTENTS        L"FileContents"

static const CLIPFORMAT CF_RENPRIVATEMESSAGES = (CLIPFORMAT)::RegisterClipboardFormat(CFSTR_RENPRIVATEMESSAGES);
static const CLIPFORMAT CF_RENPRIVATESRCFOLDER = (CLIPFORMAT)::RegisterClipboardFormat(CFSTR_RENPRIVATESRCFOLDER);
static const CLIPFORMAT CF_FILECONTENTS = (CLIPFORMAT)::RegisterClipboardFormat(CFSTR_FILECONTENTS);
static const CLIPFORMAT CF_FILEDESCRIPTOR = (CLIPFORMAT)::RegisterClipboardFormat(CFSTR_FILEDESCRIPTORW);

////////////////////////////////////////////////////////////////////////////////////////////////

bool DragDropUtil::IsOutlookObject(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj)
{
	return (ObjectHasFormat(obj, CF_RENPRIVATEMESSAGES) &&
			ObjectHasFormat(obj, CF_RENPRIVATESRCFOLDER) &&
			ObjectHasFormat(obj, CF_FILECONTENTS) &&
			ObjectHasFormat(obj, CF_FILEDESCRIPTOR));
}

bool DragDropUtil::FormatAsOutlookUrl(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj, String^% url)
{
	if (!IsOutlookObject(obj))
		return false;
	
	// TODO
	return true;
}

bool DragDropUtil::ObjectHasFormat(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj, CLIPFORMAT cf)
{
	if (obj == nullptr)
		return false;

	// From _AfxFillFormatEtc()
	auto formatEtc = gcnew cli::array<Microsoft::VisualStudio::OLE::Interop::FORMATETC>(1);

	formatEtc[0].cfFormat = cf;
	formatEtc[0].ptd = IntPtr::Zero;
	formatEtc[0].dwAspect = DVASPECT_CONTENT;
	formatEtc[0].lindex = -1;
	formatEtc[0].tymed = (DWORD)-1;
	
	// attempt to get the data
	return (obj->QueryGetData(formatEtc) == S_OK);
}
