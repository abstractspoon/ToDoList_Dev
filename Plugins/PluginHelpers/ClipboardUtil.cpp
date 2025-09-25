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
using namespace System::Runtime::InteropServices;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

bool ClipboardUtil::GetHtmlFragment(String^% html)
{
	String^ unused;

	return GetHtmlFragment(Clipboard::GetDataObject(), html, unused);
}

bool ClipboardUtil::GetHtmlFragment(String^% html, String^% sourceUrl)
{
	return GetHtmlFragment(Clipboard::GetDataObject(), html, sourceUrl);
}

bool ClipboardUtil::GetHtmlFragment(Windows::Forms::IDataObject^ obj, String^% html)
{
	String^ unused;

	return GetHtmlFragment(obj, html, unused);
}

bool ClipboardUtil::GetHtmlFragment(Windows::Forms::IDataObject^ obj, String^% html, String^% sourceUrl)
{
	if (!obj->GetDataPresent(DataFormats::Html))
		return false;

	bool success = false;
	::IUnknown* punk = (::IUnknown*)Marshal::GetIUnknownForObject(obj).ToPointer();

	if (punk)
	{
		::IDataObject* pdata = nullptr;
		HRESULT hr = punk->QueryInterface(__uuidof(::IDataObject), (void**)&pdata);

		if (SUCCEEDED(hr))
		{
			// Note: HTML content is always returned as UTF8
			CString sHtml = CClipboard().GetText(pdata, CBF_HTML), sSourceUrl;

			// Convert to Unicode
			Misc::EncodeAsUnicode(sHtml, CP_UTF8);

			if (!CClipboard::UnpackageHTMLFragment(sHtml, sSourceUrl).IsEmpty())
			{
				html = gcnew String(sHtml);
				sourceUrl = gcnew String(sSourceUrl);

				success = true;
			}

			pdata->Release();
		}

		punk->Release();
	}

	return success;
}
