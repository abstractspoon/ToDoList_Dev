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

#define INIT_INTERFACES(obj)                                                        \
m_pUnk = (::IUnknown*)Marshal::GetIUnknownForObject(obj).ToPointer();               \
if (m_pUnk) { ::IDataObject* pdata = nullptr;                                       \
	if (SUCCEEDED(m_pUnk->QueryInterface(__uuidof(::IDataObject), (void**)&pdata))) \
		m_pData = pdata; }

#define RELEASE_INTERFACE(i) if (i) { i->Release(); i = NULL; }

////////////////////////////////////////////////////////////////////////////////////////////////

OleDataObjectEx::OleDataObjectEx(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj)
	:
	m_pUnk(nullptr),
	m_pData(nullptr)
{
	INIT_INTERFACES(obj)
}

OleDataObjectEx::OleDataObjectEx(Windows::Forms::IDataObject^ obj)
	:
	m_pUnk(nullptr),
	m_pData(nullptr)
{
	INIT_INTERFACES(obj)
}

OleDataObjectEx::~OleDataObjectEx()
{
	RELEASE_INTERFACE(m_pData)
	RELEASE_INTERFACE(m_pUnk)
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool ClipboardUtil::GetHtmlFragment(String^% html)
{
	String^ unused;

	return GetHtmlFragment(html, unused);
}

bool ClipboardUtil::GetHtmlFragment(String^% html, String^% sourceUrl)
{
	return GetHtmlFragment(nullptr, html, sourceUrl);
}

bool ClipboardUtil::GetHtmlFragment(Windows::Forms::IDataObject^ obj, String^% html)
{
	String^ unused;

	return GetHtmlFragment(obj, html, unused);
}

bool ClipboardUtil::GetHtmlFragment(Windows::Forms::IDataObject^ obj, String^% html, String^% sourceUrl)
{
	auto objEx = gcnew OleDataObjectEx(obj);

	if (!objEx->IsValid())
		return false;

	auto temp = ClipboardUtil::GetHtml(objEx);
	CString sHtml = MS(temp), sSourceUrl;

	if (CClipboard::UnpackageHTMLFragment(sHtml, sSourceUrl).IsEmpty())
		return false;

	html = gcnew String(sHtml);
	sourceUrl = gcnew String(sSourceUrl);

	return true;
}

bool ClipboardUtil::IsDropFile(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj)
{
	auto objEx = gcnew OleDataObjectEx(obj);

	if (!objEx->IsValid())
		return false;

	return (CClipboard::HasFormat(objEx->Data(), CF_HDROP) != FALSE);
}

cli::array<String^>^ ClipboardUtil::GetDropFiles(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj)
{
	auto objEx = gcnew OleDataObjectEx(obj);

	if (!objEx->IsValid())
		return nullptr;

	CStringArray aFilePaths;
	int nNumFiles = CClipboard::GetDropFilePaths(objEx->Data(), aFilePaths);

	auto filePaths = gcnew cli::array<String^>(nNumFiles);

	for (int nFile = 0; nFile < nNumFiles; nFile++)
		filePaths[nFile] = gcnew String(aFilePaths[nFile]);

	return filePaths;
}

bool ClipboardUtil::IsRtf(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj)
{
	OleDataObjectEx^ objEx = gcnew OleDataObjectEx(obj);

	if (!objEx->IsValid())
		return false;

	return ((CClipboard::HasFormat(objEx->Data(), CBF_RTF) != FALSE) || 
			(CClipboard::HasFormat(objEx->Data(), CBF_RETEXTOBJ) != FALSE));
}

String^ ClipboardUtil::GetRtf(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj)
{
	OleDataObjectEx^ objEx = gcnew OleDataObjectEx(obj);

	if (objEx->IsValid())
	{
		CString sRtf = CClipboard::GetText(objEx->Data(), CBF_RTF);

		if (sRtf.IsEmpty())
			sRtf = CClipboard().GetText(objEx->Data(), CBF_RETEXTOBJ);

		// RTF content is always returned as UTF8 So we need to convert to Unicode
		return gcnew String(Misc::EncodeAsUnicode(sRtf, CP_UTF8));
	}

	// else
	return String::Empty;
}

bool ClipboardUtil::IsHtml(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj)
{
	OleDataObjectEx^ objEx = gcnew OleDataObjectEx(obj);

	if (!objEx->IsValid())
		return false;

	return (CClipboard::HasFormat(objEx->Data(), CBF_HTML) != FALSE);
}

String^ ClipboardUtil::GetHtml()
{
	CString sHtml = CClipboard().GetText(CBF_HTML);

	// HTML content is always returned as UTF8 So we need to convert to Unicode
	return gcnew String(Misc::EncodeAsUnicode(sHtml, CP_UTF8));
}

String^ ClipboardUtil::GetHtml(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj)
{
	return GetHtml(gcnew OleDataObjectEx(obj));
}

String^ ClipboardUtil::GetHtml(OleDataObjectEx^ objEx)
{
	if (objEx && objEx->IsValid())
	{
		CString sHtml = CClipboard::GetText(objEx->Data(), CBF_HTML);

		// HTML content is always returned as UTF8 So we need to convert to Unicode
		return gcnew String(Misc::EncodeAsUnicode(sHtml, CP_UTF8));
	}

	// else
	return String::Empty;
}
