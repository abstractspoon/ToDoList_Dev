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

#define RELEASE_INTERFACE(i) if (i) { i->Release(); i = NULL; }

////////////////////////////////////////////////////////////////////////////////////////////////

DataObjectEx::DataObjectEx() 
	: 
	m_Obj(nullptr)
{
}

DataObjectEx::DataObjectEx(Windows::Forms::IDataObject^ obj) 
	: 
	m_Obj(obj)
{
}

Object^ DataObjectEx::GetData(String^ format)
{
	return GetData(format, false);
}

Object^ DataObjectEx::GetData(Type^ format)
{
	if (m_Obj)
		return m_Obj->GetData(format);

	// else
	return Clipboard::GetDataObject()->GetData(format);
}

Object^ DataObjectEx::GetData(String^ format, bool autoConvert)
{
	if ((format != DataFormats::Html) || 
		!GetDataPresent(DataFormats::Html, autoConvert))
	{
		if (m_Obj)
			return m_Obj->GetData(format, autoConvert);

		// else
		return Clipboard::GetDataObject()->GetData(format, autoConvert);
	}

	// else use Win32 API to properly handle UTF8 to Unicode conversion
	CString sHtml;

	if (m_Obj)
	{
		auto objEx = gcnew OleDataObjectEx(m_Obj);

		if (objEx->IsValid())
			sHtml = CClipboard::GetText(objEx->Data(), CBF_HTML);
	}
	else
	{
		sHtml = CClipboard().GetText(CBF_HTML);
	}

	// HTML content is always returned as UTF8 So we need to convert to Unicode
	Misc::EncodeAsUnicode(sHtml, CP_UTF8);

	return gcnew String(sHtml);
}

bool DataObjectEx::GetDataPresent(String^ format)
{
	if (m_Obj)
		return m_Obj->GetDataPresent(format);

	// else
	return Clipboard::GetDataObject()->GetDataPresent(format);
}

bool DataObjectEx::GetDataPresent(Type^ format)
{
	if (m_Obj)
		return m_Obj->GetDataPresent(format);

	// else
	return Clipboard::GetDataObject()->GetDataPresent(format);
}

bool DataObjectEx::GetDataPresent(String^ format, bool autoConvert)
{
	if (m_Obj)
		return m_Obj->GetDataPresent(format, autoConvert);

	// else
	return Clipboard::GetDataObject()->GetDataPresent(format);
}

cli::array<String^>^ DataObjectEx::GetFormats()
{
	if (m_Obj)
		return m_Obj->GetFormats();

	// else
	return Clipboard::GetDataObject()->GetFormats();
}

cli::array<String^>^ DataObjectEx::GetFormats(bool autoConvert)
{
	if (m_Obj)
		return m_Obj->GetFormats(autoConvert);

	// else
	return Clipboard::GetDataObject()->GetFormats(autoConvert);
}

void DataObjectEx::SetData(Object^ data)
{
	if (m_Obj)
		m_Obj->SetData(data);

	// else
	Clipboard::GetDataObject()->SetData(data);
}

void DataObjectEx::SetData(String^ format, Object^ data)
{
	if (m_Obj)
		m_Obj->SetData(format, data);

	// else
	Clipboard::GetDataObject()->SetData(format, data);
}

void DataObjectEx::SetData(Type^ format, Object^ data)
{
	if (m_Obj)
		m_Obj->SetData(format, data);

	// else
	Clipboard::GetDataObject()->SetData(format, data);
}

void DataObjectEx::SetData(String^ format, bool autoConvert, Object^ data)
{
	if (m_Obj)
		m_Obj->SetData(format, autoConvert, data);


	// else
	Clipboard::GetDataObject()->SetData(format, autoConvert, data);
}

////////////////////////////////////////////////////////////////////////////////////////////////

#define INIT_UNK_DATA(obj)                                                          \
m_pUnk = (::IUnknown*)Marshal::GetIUnknownForObject(obj).ToPointer();               \
if (m_pUnk) { ::IDataObject* pdata = nullptr;                                       \
	if (SUCCEEDED(m_pUnk->QueryInterface(__uuidof(::IDataObject), (void**)&pdata))) \
		m_pData = pdata; }

////////////////////////////////////////////////////////////////////////////////////////////////

OleDataObjectEx::OleDataObjectEx(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj)
	:
	m_pUnk(nullptr),
	m_pData(nullptr)
{
	INIT_UNK_DATA(obj)
}

OleDataObjectEx::OleDataObjectEx(Windows::Forms::IDataObject^ obj)
	:
	m_pUnk(nullptr),
	m_pData(nullptr)
{
	INIT_UNK_DATA(obj)
}

OleDataObjectEx::~OleDataObjectEx()
{
	if (m_pData)
		m_pData->Release();

	if (m_pUnk)
		m_pUnk->Release();
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
	auto data = GetDataObject(obj)->GetData(DataFormats::Html, true);

	if ((data == nullptr) || !ISTYPE(data, String))
		return false;

	CString sHtml(MS(data->ToString())), sSourceUrl;

	if (CClipboard::UnpackageHTMLFragment(sHtml, sSourceUrl).IsEmpty())
		return false;

	html = gcnew String(sHtml);
	sourceUrl = gcnew String(sSourceUrl);

	return true;
}

Windows::Forms::IDataObject^ ClipboardUtil::GetDataObject(Windows::Forms::IDataObject^ obj)
{
	if (ISTYPE(obj, DataObjectEx))
		return obj;

	return gcnew DataObjectEx(obj);
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

