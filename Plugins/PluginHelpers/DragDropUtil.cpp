
#include "stdafx.h"
#include "DragDropUtil.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

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
