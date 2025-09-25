// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "ClipboardUtil.h"

#include <Shared\Clipboard.h>
#include <Shared\Misc.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::Diagnostics;
using namespace System::Windows::Forms;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

bool ClipboardUtil::GetHtml(String^% html, String^% sourceUrl)
{
	return GetHtml(Clipboard::GetDataObject(), html, sourceUrl);
}

bool ClipboardUtil::GetHtml(Windows::Forms::IDataObject^ obj, String^% html, String^% sourceUrl)
{
	if (!obj->GetDataPresent(DataFormats::Html))
		return false;

	::IUnknown* punk = (::IUnknown*)System::Runtime::InteropServices::Marshal::GetIUnknownForObject(obj).ToPointer();

	::IDataObject* pdata = nullptr;
	HRESULT hr = punk->QueryInterface(__uuidof(::IDataObject), (void**)&pdata);

	bool success = false;

	if (SUCCEEDED(hr))
	{
		CString sHtml = CClipboard().GetText(pdata, CBF_HTML); // Returned in UTF8
		Misc::EncodeAsUnicode(sHtml, CP_UTF8);

		CString sSourceUrl;
		CClipboard::UnpackageHTMLFragment(sHtml, sSourceUrl);

		if (!sHtml.IsEmpty())
		{
			html = gcnew String(sHtml);
			sourceUrl = gcnew String(sSourceUrl);

			success = true;
		}

		pdata->Release();
	}

	return success;
}
