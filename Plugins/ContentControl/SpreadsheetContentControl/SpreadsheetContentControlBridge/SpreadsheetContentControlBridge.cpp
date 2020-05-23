// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "SpreadsheetContentControlBridge.h"
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

using namespace SpreadsheetContentControl;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

// REPLACE THIS WITH NEW GUID!
const LPCWSTR SPREADSHEET_GUID = L"BBDCAEDF-B297-4E09-BBFB-B308358628B9";
const LPCWSTR SPREADSHEET_NAME = L"Spreadsheet";

////////////////////////////////////////////////////////////////////////////////////////////////

CSpreadsheetContentBridge::CSpreadsheetContentBridge() : m_hIcon(NULL), m_pTT(NULL)
{
   HMODULE hMod = LoadLibrary(L"SpreadsheetContentControlBridge.dll"); // us

   m_hIcon = (HICON)::LoadImage(hMod, MAKEINTRESOURCE(IDI_SPREADSHEET), IMAGE_ICON, 16, 16, LR_LOADMAP3DCOLORS);
}

CSpreadsheetContentBridge::~CSpreadsheetContentBridge()
{
   ::DestroyIcon(m_hIcon);
}

void CSpreadsheetContentBridge::Release()
{
	delete this;
}

void CSpreadsheetContentBridge::SetLocalizer(ITransText* pTT)
{
	if (m_pTT == nullptr)
		m_pTT = pTT;
}

LPCWSTR CSpreadsheetContentBridge::GetTypeDescription() const
{
	return SPREADSHEET_NAME;
}

HICON CSpreadsheetContentBridge::GetTypeIcon() const
{
   return m_hIcon;
}

LPCWSTR CSpreadsheetContentBridge::GetTypeID() const
{
	return SPREADSHEET_GUID;
}

IContentControl* CSpreadsheetContentBridge::CreateCtrl(unsigned short nCtrlID, unsigned long nStyle, 
	long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
{
	CSpreadsheetContentControlBridge* pCtrl = new CSpreadsheetContentControlBridge(m_pTT);

	if (!pCtrl->Create(nCtrlID, nStyle, nLeft, nTop, nWidth, nHeight, hwndParent))
	{
		pCtrl->Release();
		pCtrl = NULL;
	}

	return pCtrl;
}

void CSpreadsheetContentBridge::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const
{
	// TODO

}

void CSpreadsheetContentBridge::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly)
{
	// TODO
}

// returns the length of the html or zero if not supported
int CSpreadsheetContentBridge::ConvertToHtml(const unsigned char* pContent, int nLength,
	LPCWSTR szCharSet, LPWSTR& szHtml, LPCWSTR szImageDir)
{
	szHtml = nullptr;
	return 0;
}

void CSpreadsheetContentBridge::FreeHtmlBuffer(LPWSTR& szHtml)
{
	delete [] szHtml;
	szHtml = nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////

// This is the constructor of a class that has been exported.
// see ExporterBridge.h for the class definition
CSpreadsheetContentControlBridge::CSpreadsheetContentControlBridge(ITransText* pTT) : m_pTT(pTT)
{
}

BOOL CSpreadsheetContentControlBridge::Create(UINT nCtrlID, DWORD nStyle, 
	long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
{
	msclr::auto_gcroot<Translator^> trans = gcnew Translator(m_pTT);
	msclr::auto_gcroot<String^> typeID = gcnew String(SPREADSHEET_GUID);

	m_wnd = gcnew SpreadsheetContentControl::SpreadsheetContentControlCore(typeID.get(), static_cast<IntPtr>(hwndParent), trans.get());

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

int CSpreadsheetContentControlBridge::GetContent(unsigned char* pContent) const
{
	if (pContent == NULL)
	{
		// Caller is just requesting length but it's inefficient
		// for us to request the full content just for its length
		// so we cache it for the subsequent actual content request
		cli::array<Byte>^ content = m_wnd->GetContent();
		int nLength = content->Length;

		m_lastRequestedContent = gcnew cli::array<unsigned char>(nLength);

		if (nLength)
			Array::Copy(content, 0, m_lastRequestedContent, 0, nLength);

		return nLength;
	}
	else if (m_lastRequestedContent == nullptr)
	{
		return 0;
	}

	// else
	int nLength = m_lastRequestedContent->Length;

	if (nLength)
	{
		pin_ptr<Byte> ptrContent = &m_lastRequestedContent[m_lastRequestedContent->GetLowerBound(0)];
		CopyMemory(pContent, ptrContent, nLength);
	}

	return nLength;
}

bool CSpreadsheetContentControlBridge::SetContent(const unsigned char* pContent, int nLength, bool bResetSelection)
{
	cli::array<Byte>^ content = gcnew cli::array<Byte>(nLength);

	for (int i = 0; i < nLength; i++)
		content[i] = pContent[i];

	return m_wnd->SetContent(content, bResetSelection);
}

LPCWSTR CSpreadsheetContentControlBridge::GetTypeID() const
{
	return SPREADSHEET_GUID;
}

// text content if supported. return false if not supported
int CSpreadsheetContentControlBridge::GetTextContent(LPWSTR szContent, int nLength) const
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

bool CSpreadsheetContentControlBridge::SetTextContent(LPCWSTR szContent, bool bResetSelection)
{
	msclr::auto_gcroot<String^> content = gcnew String(szContent);

	return m_wnd->SetTextContent(content.get(), bResetSelection);
}

bool CSpreadsheetContentControlBridge::FindReplaceAll(LPCWSTR szFind, LPCWSTR szReplace, bool bCaseSensitive, bool bWholeWord)
{
	msclr::auto_gcroot<String^> find = gcnew String(szFind);
	msclr::auto_gcroot<String^> replace = gcnew String(szReplace);

	return (m_wnd->FindReplaceAll(find.get(), replace.get(), bWholeWord, bCaseSensitive) > 0);
}

bool CSpreadsheetContentControlBridge::InsertTextContent(LPCWSTR szContent, bool bAtEnd)
{
	msclr::auto_gcroot<String^> content = gcnew String(szContent);

	return m_wnd->InsertTextContent(content.get(), bAtEnd);
}

void CSpreadsheetContentControlBridge::SetReadOnly(bool bReadOnly)
{
	m_wnd->SetReadOnly(bReadOnly);
}

void CSpreadsheetContentControlBridge::Enable(bool bEnable)
{
	m_wnd->Enabled = bEnable;
}

HWND CSpreadsheetContentControlBridge::GetHwnd() const
{
	return static_cast<HWND>(m_wnd->Handle.ToPointer());
}

void CSpreadsheetContentControlBridge::Release()
{
	::DestroyWindow(GetHwnd());
	delete this;
}

bool CSpreadsheetContentControlBridge::ProcessMessage(MSG* pMsg)
{
	return m_wnd->ProcessMessage((IntPtr)pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam, pMsg->time, pMsg->pt.x, pMsg->pt.y);
}

ISpellCheck* CSpreadsheetContentControlBridge::GetSpellCheckInterface()
{
	return nullptr;
}

bool CSpreadsheetContentControlBridge::Undo()
{
	return m_wnd->Undo();
}

bool CSpreadsheetContentControlBridge::Redo()
{
	return m_wnd->Redo();
}

void CSpreadsheetContentControlBridge::SetUITheme(const UITHEME* pTheme)
{
	msclr::auto_gcroot<UITheme^> theme = gcnew UITheme(pTheme);

	m_wnd->SetUITheme(theme.get());
}

void CSpreadsheetContentControlBridge::SetContentFont(HFONT hFont)
{
	m_wnd->SetContentFont(Win32::GetFaceName(hFont), Win32::GetPointSize(hFont));
}

void CSpreadsheetContentControlBridge::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const
{
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<String^> key = gcnew String(szKey);

	m_wnd->SavePreferences(prefs.get(), key.get());
}

void CSpreadsheetContentControlBridge::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly)
{
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<String^> key = gcnew String(szKey);

	m_wnd->LoadPreferences(prefs.get(), key.get(), bAppOnly);
}
