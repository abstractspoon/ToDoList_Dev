// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "HTMLContentControlBridge.h"
#include "resource.h"

#include <unknwn.h>
#include <tchar.h>
#include <msclr\auto_gcroot.h>

#include <Interfaces\ITransText.h>
#include <Interfaces\UITheme.h>
#include <Interfaces\IPreferences.h>
#include <Interfaces\ISpellcheck.h>

////////////////////////////////////////////////////////////////////////////////////////////////

#using <PluginHelpers.dll> as_friend

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace HTMLContentControl;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

// REPLACE THIS WITH NEW GUID!
const LPCWSTR HTML_GUID = L"FE0B6B6E-2B61-4AEB-AA0D-98DBE5942F02";
const LPCWSTR HTML_NAME = L"Html";

////////////////////////////////////////////////////////////////////////////////////////////////

CHTMLContentBridge::CHTMLContentBridge() : m_hIcon(NULL), m_pTT(NULL)
{
   HMODULE hMod = LoadLibrary(L"HTMLContentControlBridge.dll"); // us

   m_hIcon = (HICON)::LoadImage(hMod, MAKEINTRESOURCE(IDI_HTML), IMAGE_ICON, 16, 16, LR_LOADMAP3DCOLORS);
}

CHTMLContentBridge::~CHTMLContentBridge()
{
   ::DestroyIcon(m_hIcon);
}

void CHTMLContentBridge::Release()
{
	delete this;
}

void CHTMLContentBridge::SetLocalizer(ITransText* pTT)
{
	if (m_pTT == nullptr)
		m_pTT = pTT;
}

LPCWSTR CHTMLContentBridge::GetTypeDescription() const
{
	return HTML_NAME;
}

HICON CHTMLContentBridge::GetTypeIcon() const
{
   return m_hIcon;
}

LPCWSTR CHTMLContentBridge::GetTypeID() const
{
	return HTML_GUID;
}

IContentControl* CHTMLContentBridge::CreateCtrl(unsigned short nCtrlID, unsigned long nStyle, 
	long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
{
	CHTMLContentControlBridge* pCtrl = new CHTMLContentControlBridge(m_pTT);

	if (!pCtrl->Create(nCtrlID, nStyle, nLeft, nTop, nWidth, nHeight, hwndParent))
	{
		pCtrl->Release();
		pCtrl = NULL;
	}

	return pCtrl;
}

void CHTMLContentBridge::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const
{
	// TODO

}

void CHTMLContentBridge::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly)
{
	// TODO
}

// returns the length of the html or zero if not supported
int CHTMLContentBridge::ConvertToHtml(const unsigned char* pContent, int nLength,
	LPCWSTR /*szCharSet*/, LPWSTR& szHtml, LPCWSTR szImageDir)
{
	if (nLength == 0)
		return 0;

	// pContent is already the HTML body
	int nCharLen = ((nLength / 2) + 1);
	szHtml = new WCHAR[nCharLen];

	ZeroMemory(szHtml, nCharLen * sizeof(WCHAR));
	CopyMemory(szHtml, pContent, nLength);
	
	return nCharLen;
}

void CHTMLContentBridge::FreeHtmlBuffer(LPWSTR& szHtml)
{
	delete [] szHtml;
	szHtml = nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////

// This is the constructor of a class that has been exported.
// see ExporterBridge.h for the class definition
CHTMLContentControlBridge::CHTMLContentControlBridge(ITransText* pTT)
	: m_pTT(pTT)
{
}

BOOL CHTMLContentControlBridge::Create(UINT nCtrlID, DWORD nStyle, 
	long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
{
	msclr::auto_gcroot<Translator^> trans = gcnew Translator(m_pTT);
	msclr::auto_gcroot<String^> typeID = gcnew String(HTML_GUID);

	m_wnd = gcnew HTMLContentControl::HTMLContentControlCore(typeID.get(), static_cast<IntPtr>(hwndParent), trans.get());

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

int CHTMLContentControlBridge::GetContent(unsigned char* pContent) const
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

bool CHTMLContentControlBridge::SetContent(const unsigned char* pContent, int nLength, bool bResetSelection)
{
	cli::array<Byte>^ content = gcnew cli::array<Byte>(nLength);

	for (int i = 0; i < nLength; i++)
		content[i] = pContent[i];

	return m_wnd->SetContent(content, bResetSelection);
}

bool CHTMLContentControlBridge::FindReplaceAll(LPCWSTR szFind, LPCWSTR szReplace, bool bCaseSensitive, bool bWholeWord)
{
	msclr::auto_gcroot<String^> find = gcnew String(szFind);
	msclr::auto_gcroot<String^> replace = gcnew String(szReplace);

	return (m_wnd->FindReplaceAll(find.get(), replace.get(), bWholeWord, bCaseSensitive) > 0);
}

LPCWSTR CHTMLContentControlBridge::GetTypeID() const
{
	return HTML_GUID;
}

// text content if supported. return false if not supported
int CHTMLContentControlBridge::GetTextContent(LPWSTR szContent, int nLength) const
{
	String^ content = m_wnd->GetTextContent();
	nLength = content->Length;

	if (szContent && nLength)
	{
		MarshalledString msContent(content);
		CopyMemory(szContent, msContent, (nLength * sizeof(WCHAR)));
	}

	return nLength;
}

bool CHTMLContentControlBridge::SetTextContent(LPCWSTR szContent, bool bResetSelection)
{
	msclr::auto_gcroot<String^> content = gcnew String(szContent);

	return m_wnd->SetTextContent(content.get(), bResetSelection);
}

bool CHTMLContentControlBridge::InsertTextContent(LPCWSTR szContent, bool bAtEnd)
{
	msclr::auto_gcroot<String^> content = gcnew String(szContent);

	return m_wnd->InsertTextContent(content.get(), bAtEnd);
}

void CHTMLContentControlBridge::SetReadOnly(bool bReadOnly)
{
	m_wnd->SetReadOnly(bReadOnly);
}

void CHTMLContentControlBridge::Enable(bool bEnable)
{
	m_wnd->Enabled = bEnable;
}

HWND CHTMLContentControlBridge::GetHwnd() const
{
	return static_cast<HWND>(m_wnd->Handle.ToPointer());
}

void CHTMLContentControlBridge::Release()
{
	::DestroyWindow(GetHwnd());
	delete this;
}

bool CHTMLContentControlBridge::ProcessMessage(MSG* pMsg)
{
	return m_wnd->ProcessMessage((IntPtr)pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam, pMsg->time, pMsg->pt.x, pMsg->pt.y);
}

ISpellCheck* CHTMLContentControlBridge::GetSpellCheckInterface()
{
	return nullptr;
}

bool CHTMLContentControlBridge::Undo()
{
	return m_wnd->Undo();
}

bool CHTMLContentControlBridge::Redo()
{
	return m_wnd->Redo();
}

void CHTMLContentControlBridge::SetUITheme(const UITHEME* pTheme)
{
	msclr::auto_gcroot<UITheme^> theme = gcnew UITheme(pTheme);

	m_wnd->SetUITheme(theme.get());
}

void CHTMLContentControlBridge::SetContentFont(HFONT hFont)
{
	m_wnd->SetContentFont(Win32::GetFaceName(hFont), Win32::GetPointSize(hFont));
}

void CHTMLContentControlBridge::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const
{
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<String^> key = gcnew String(szKey);

	m_wnd->SavePreferences(prefs.get(), key.get());
}

void CHTMLContentControlBridge::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly)
{
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<String^> key = gcnew String(szKey);

	m_wnd->LoadPreferences(prefs.get(), key.get(), bAppOnly);
}
