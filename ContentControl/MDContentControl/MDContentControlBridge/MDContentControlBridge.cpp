// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include <unknwn.h>
#include <tchar.h>

#include "stdafx.h"
#include "MDContentControlBridge.h"

#include "..\..\..\..\Interfaces\ITransText.h"
#include "..\..\..\..\Interfaces\IPreferences.h"
#include "..\..\..\..\Interfaces\UITheme.h"
#include "..\..\..\..\Interfaces\ISpellcheck.h"

////////////////////////////////////////////////////////////////////////////////////////////////

#using <..\..\..\Debug\MDContentControlCore.dll>
#include <msclr\auto_gcroot.h>

#using <..\..\..\Debug\PluginHelpers.dll> as_friend

using namespace MDContentControl;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace TDLPluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

// REPLACE THIS WITH NEW GUID!
const LPCWSTR MARKDOWN_GUID = L"BAA4E079-268B-4B9B-B7C8-6D15CCF058A2";
const LPCWSTR MARKDOWN_NAME = L"Markdown";

////////////////////////////////////////////////////////////////////////////////////////////////

CMDContentBridge::CMDContentBridge()// : m_hIcon(NULL)
{
	//    HMODULE hMod = LoadLibrary(L"MDContentBridge.dll"); // us
	// 
	//    m_hIcon = ::LoadIcon(hMod, MAKEINTRESOURCE(IDI_STATS));
}

CMDContentBridge::~CMDContentBridge()
{
	//    ::DestroyIcon(m_hIcon);
}

void CMDContentBridge::Release()
{
	delete this;
}

void CMDContentBridge::SetLocalizer(ITransText* /*pTT*/)
{
	// TODO
}

LPCTSTR CMDContentBridge::GetTypeDescription() const
{
	return MARKDOWN_NAME;
}

// HICON CMDContentBridge::GetIcon() const
// {
//    return m_hIcon;
// }

LPCWSTR CMDContentBridge::GetTypeID() const
{
	return MARKDOWN_GUID;
}

IContentControl* CMDContentBridge::CreateCtrl(unsigned short nCtrlID, unsigned long nStyle, 
	long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
{
	CMDContentBridgeControl* pCtrl = new CMDContentBridgeControl();

	if (!pCtrl->Create(nCtrlID, nStyle, nLeft, nTop, nWidth, nHeight, hwndParent))
	{
		pCtrl->Release();
		pCtrl = NULL;
	}

	return pCtrl;
}

void CMDContentBridge::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const
{
	// TODO

}

void CMDContentBridge::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey/*, bool bAppOnly*/)
{
	// TODO
}

// returns the length of the html or zero if not supported
int CMDContentBridge::ConvertToHtml(const unsigned char* pContent, int nLength,
	LPCWSTR szCharSet, LPWSTR& szHtml, LPCWSTR szImageDir)
{
	szHtml = nullptr;
	return 0;
}

void CMDContentBridge::FreeHtmlBuffer(LPWSTR& szHtml)
{
	delete [] szHtml;
	szHtml = nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////

// This is the constructor of a class that has been exported.
// see ExporterBridge.h for the class definition
CMDContentBridgeControl::CMDContentBridgeControl()
{
}

BOOL CMDContentBridgeControl::Create(UINT nCtrlID, DWORD nStyle, 
	long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
{
	m_wnd = gcnew MDContentControl::MDContentControlCore(static_cast<IntPtr>(hwndParent));

	HWND hWnd = GetHwnd();

	if (hWnd)
	{
		::SetParent(hWnd, hwndParent);
		::SetWindowLong(hWnd, GWL_ID, nCtrlID);
		::SetWindowLong(hWnd, GWL_STYLE, nStyle);
		::MoveWindow(hWnd, nLeft, nTop, nWidth, nHeight, FALSE);

		return true;
	}

	return false;
}

int CMDContentBridgeControl::GetContent(unsigned char* pContent) const
{
	cli::array<Byte>^ content = m_wnd->GetContent();
	int nLength = content->Length;

	if (pContent && nLength)
	{
		pin_ptr<Byte> ptrContent = &content[content->GetLowerBound(0)];
		CopyMemory(pContent, ptrContent, nLength);
	}

	return nLength;
}

bool CMDContentBridgeControl::SetContent(const unsigned char* pContent, int nLength, bool bResetSelection)
{
	cli::array<Byte>^ content = gcnew cli::array<Byte>(nLength);

	for (int i = 0; i < nLength; i++)
		content[i] = pContent[i];

	return m_wnd->SetContent(content, bResetSelection);
}

LPCWSTR CMDContentBridgeControl::GetTypeID() const
{
	return MARKDOWN_GUID;
}

// text content if supported. return false if not supported
int CMDContentBridgeControl::GetTextContent(LPWSTR szContent, int nLength) const
{
	String^ content = m_wnd->GetTextContent();
	nLength = content->Length;

	if (szContent != nullptr)
	{
		TDLPluginHelpers::TDLMarshalledString msContent(content);
		CopyMemory(szContent, msContent, (nLength * sizeof(WCHAR)));
	}

	return nLength;
}

bool CMDContentBridgeControl::SetTextContent(LPCWSTR szContent, bool bResetSelection)
{
	msclr::auto_gcroot<String^> content = gcnew String(szContent);

	return m_wnd->SetTextContent(content, bResetSelection);
}

void CMDContentBridgeControl::SetReadOnly(bool bReadOnly)
{

}

HWND CMDContentBridgeControl::GetHwnd() const
{
	//return NULL;
	return static_cast<HWND>(m_wnd->Handle.ToPointer());
}

void CMDContentBridgeControl::Release()
{
	delete this;
}

bool CMDContentBridgeControl::ProcessMessage(MSG* pMsg)
{
	return false;
}

ISpellCheck* CMDContentBridgeControl::GetSpellCheckInterface()
{
	return nullptr;
}

bool CMDContentBridgeControl::Undo()
{
	return false;
}

bool CMDContentBridgeControl::Redo()
{
	return false;
}

void CMDContentBridgeControl::SetUITheme(const UITHEME* pTheme)
{

}

void CMDContentBridgeControl::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const
{

}

void CMDContentBridgeControl::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey)
{

}
