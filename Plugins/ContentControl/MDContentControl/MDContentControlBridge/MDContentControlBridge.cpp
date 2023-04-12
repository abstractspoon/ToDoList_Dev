// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "MDContentControlBridge.h"
#include "resource.h"

#include <unknwn.h>
#include <tchar.h>
#include <msclr\auto_gcroot.h>

#include <Interfaces\ITransText.h>
#include <Interfaces\IPreferences.h>
#include <Interfaces\UITheme.h>
#include <Interfaces\ISpellcheck.h>

////////////////////////////////////////////////////////////////////////////////////////////////

#using <PluginHelpers.dll> as_friend

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace MDContentControl;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

// REPLACE THIS WITH NEW GUID!
const LPCWSTR MARKDOWN_GUID = L"BAA4E079-268B-4B9B-B7C8-6D15CCF058A2";
const LPCWSTR MARKDOWN_NAME = L"Markdown";

////////////////////////////////////////////////////////////////////////////////////////////////

CMDContentBridge::CMDContentBridge() : m_hIcon(NULL)
{
   HMODULE hMod = LoadLibrary(L"MDContentControlBridge.dll"); // us

   m_hIcon = (HICON)::LoadImage(hMod, MAKEINTRESOURCE(IDI_MARKDOWN), IMAGE_ICON, 16, 16, LR_LOADMAP3DCOLORS);
}

CMDContentBridge::~CMDContentBridge()
{
   ::DestroyIcon(m_hIcon);
}

void CMDContentBridge::Release()
{
	delete this;
}

void CMDContentBridge::SetLocalizer(ITransText* /*pTT*/)
{
	// TODO
}

LPCWSTR CMDContentBridge::GetTypeDescription() const
{
	return MARKDOWN_NAME;
}

HICON CMDContentBridge::GetTypeIcon() const
{
   return m_hIcon;
}

LPCWSTR CMDContentBridge::GetTypeID() const
{
	return MARKDOWN_GUID;
}

IContentControl* CMDContentBridge::CreateCtrl(unsigned short nCtrlID, unsigned long nStyle, 
	long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
{
	CMDContentControlBridge* pCtrl = new CMDContentControlBridge();

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

void CMDContentBridge::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly)
{
	// TODO
}

// returns the length of the html or zero if not supported
int CMDContentBridge::ConvertToHtml(const unsigned char* pContent, int nLength,
	LPCWSTR szCharSet, LPWSTR& szHtml, LPCWSTR szImageDir)
{
	cli::array<Byte>^ content = gcnew cli::array<Byte>(nLength);

	for (int i = 0; i < nLength; i++)
		content[i] = pContent[i];

	String^ html = MDContentControlCore::ConvertToHtml(content);

	if (String::IsNullOrWhiteSpace(html))
		return 0;

	MarshalledString msHtml(html);

	int nCharLen = ((nLength / 2) + 1);
	szHtml = new WCHAR[nCharLen];
	
	ZeroMemory(szHtml, nCharLen * sizeof(WCHAR));
	CopyMemory(szHtml, msHtml, nLength);

	return nCharLen;
}

void CMDContentBridge::FreeHtmlBuffer(LPWSTR& szHtml)
{
	delete [] szHtml;
	szHtml = nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////

// This is the constructor of a class that has been exported.
// see ExporterBridge.h for the class definition
CMDContentControlBridge::CMDContentControlBridge()
{
}

BOOL CMDContentControlBridge::Create(UINT nCtrlID, DWORD nStyle, 
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

int CMDContentControlBridge::GetContent(unsigned char* pContent) const
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

bool CMDContentControlBridge::SetContent(const unsigned char* pContent, int nLength, bool bResetSelection)
{
	cli::array<Byte>^ content = gcnew cli::array<Byte>(nLength);

	for (int i = 0; i < nLength; i++)
		content[i] = pContent[i];

	return m_wnd->SetContent(content, bResetSelection);
}

LPCWSTR CMDContentControlBridge::GetTypeID() const
{
	return MARKDOWN_GUID;
}

// text content if supported. return false if not supported
int CMDContentControlBridge::GetTextContent(LPWSTR szContent, int nLength) const
{
	String^ content = m_wnd->GetTextContent();
	nLength = content->Length;

	if (szContent != nullptr)
	{
		MarshalledString msContent(content);
		CopyMemory(szContent, msContent, (nLength * sizeof(WCHAR)));
	}

	return nLength;
}

bool CMDContentControlBridge::SetTextContent(LPCWSTR szContent, bool bResetSelection)
{
	msclr::auto_gcroot<String^> content = gcnew String(szContent);

	return m_wnd->SetTextContent(content.get(), bResetSelection);
}

bool CMDContentControlBridge::InsertTextContent(LPCWSTR szContent, bool bAtEnd)
{
	msclr::auto_gcroot<String^> content = gcnew String(szContent);

	return m_wnd->InsertTextContent(content.get(), bAtEnd);
}

bool CMDContentControlBridge::FindReplaceAll(LPCWSTR szFind, LPCWSTR szReplace, bool bCaseSensitive, bool bWholeWord)
{
	// TODO
	return false;
}

void CMDContentControlBridge::SetReadOnly(bool bReadOnly)
{
	m_wnd->ReadOnly = bReadOnly;
}

void CMDContentControlBridge::Enable(bool bEnable)
{
	m_wnd->Enabled = bEnable;
}

HWND CMDContentControlBridge::GetHwnd() const
{
	return static_cast<HWND>(m_wnd->Handle.ToPointer());
}

void CMDContentControlBridge::Release()
{
	::DestroyWindow(GetHwnd());
	delete this;
}

bool CMDContentControlBridge::ProcessMessage(MSG* pMsg)
{
	return m_wnd->ProcessMessage((IntPtr)pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam, pMsg->time, pMsg->pt.x, pMsg->pt.y);
}

ISpellCheck* CMDContentControlBridge::GetSpellCheckInterface()
{
	return nullptr;
}

bool CMDContentControlBridge::Undo()
{
	return m_wnd->Undo();
}

bool CMDContentControlBridge::Redo()
{
	return m_wnd->Redo();
}

void CMDContentControlBridge::SetUITheme(const UITHEME* pTheme)
{
	m_wnd->SetUITheme(gcnew UITheme(pTheme));
}

void CMDContentControlBridge::SetContentFont(HFONT hFont)
{
	m_wnd->SetContentFont(Win32::GetFaceName(hFont), Win32::GetPointSize(hFont));
}

void CMDContentControlBridge::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const
{
	m_wnd->SavePreferences(gcnew Preferences(pPrefs), gcnew String(szKey));
}

void CMDContentControlBridge::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly)
{
	m_wnd->LoadPreferences(gcnew Preferences(pPrefs), gcnew String(szKey), bAppOnly);
}
