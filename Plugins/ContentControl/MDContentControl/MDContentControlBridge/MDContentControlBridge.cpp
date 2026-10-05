// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "resource.h"
#include "MDContentControlBridge.h"

#include <unknwn.h>
#include <tchar.h>
#include <msclr\auto_gcroot.h>

#include <Interfaces\ITransText.h>
#include <Interfaces\IPreferences.h>
#include <Interfaces\UITheme.h>
#include <Interfaces\ISpellcheck.h>
#include <Interfaces\RichEditSpellcheck.h>

////////////////////////////////////////////////////////////////////////////////////////////////

#using <PluginHelpers.dll> as_friend

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

using namespace MDContentControl;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

// REPLACE THIS WITH NEW GUID!
const LPCWSTR MARKDOWN_GUID = L"BAA4E079-268B-4B9B-B7C8-6D15CCF058A2";
const LPCWSTR MARKDOWN_NAME = L"Markdown";

////////////////////////////////////////////////////////////////////////////////////////////////

CMDContentBridge::CMDContentBridge() : m_hIcon(NULL), m_pTT(NULL)
{
   m_hIcon = Win32::LoadHIcon(L"MDContentControlBridge.dll", IDI_MARKDOWN, 16, true);
}

CMDContentBridge::~CMDContentBridge()
{
   ::DestroyIcon(m_hIcon);
}

void CMDContentBridge::Release()
{
	delete this;
}

void CMDContentBridge::SetLocalizer(ITransText* pTT)
{
	if (m_pTT == nullptr)
		m_pTT = pTT;
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
	CMDContentControlBridge* pCtrl = new CMDContentControlBridge(m_pTT);

	if (!pCtrl->Create(nCtrlID, nStyle, nLeft, nTop, nWidth, nHeight, hwndParent))
	{
		pCtrl->Release();
		pCtrl = NULL;
	}

	return pCtrl;
}

void CMDContentBridge::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const
{
	pPrefs->WriteProfileInt(szKey, _T("InlineSpellChecking"), MDContentControlCore::InlineSpellChecking);
}

void CMDContentBridge::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly)
{
	if (!bAppOnly)
		MDContentControlCore::InlineSpellChecking = (pPrefs->GetProfileInt(szKey, _T("InlineSpellChecking"), FALSE) != FALSE);
}

// returns the length of the html or zero if not supported
int CMDContentBridge::ConvertToHtml(const unsigned char* pContent, int nLength,
	LPCWSTR szCharSet, LPWSTR& szHtml, LPCWSTR szImageDir)
{
	auto content = gcnew cli::array<Byte>(nLength);
	auto imageDir = gcnew String(szImageDir);

	for (int i = 0; i < nLength; i++)
		content[i] = pContent[i];

	String^ html = MDContentControlCore::ConvertToHtml(content, imageDir);

	if (String::IsNullOrWhiteSpace(html))
		return 0;

	MarshalledString msHtml(html);

	int nCharLen = (html->Length + 1); // Includes a NULL terminator
	szHtml = new WCHAR[nCharLen];
	
	CopyMemory(szHtml, msHtml, (html->Length * sizeof(WCHAR)));
	szHtml[html->Length] = 0;

	return nCharLen;
}

void CMDContentBridge::FreeHtmlBuffer(LPWSTR& szHtml)
{
	delete [] szHtml;
	szHtml = nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////

CMDContentControlBridge::CMDContentControlBridge(ITransText* pTT)
	: 
	m_pTT(pTT), 
	m_pSpellCheck(nullptr)
{
}

CMDContentControlBridge::~CMDContentControlBridge()
{
	delete m_pSpellCheck;
}

BOOL CMDContentControlBridge::Create(UINT nCtrlID, DWORD nStyle, 
	long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
{
	auto trans = gcnew Translator(m_pTT);
	auto parent = static_cast<IntPtr>(hwndParent);

	m_wnd = gcnew MDContentControlCore(parent, trans);

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

// IContentControl interface ///////////////////////////////////////////////

int CMDContentControlBridge::GetContent(unsigned char* pContent) const
{
	auto content = m_wnd->GetContent();
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
	auto content = gcnew cli::array<Byte>(nLength);

	for (int i = 0; i < nLength; i++)
		content[i] = pContent[i];

	return m_wnd->SetContent(content, bResetSelection);
}

LPCWSTR CMDContentControlBridge::GetTypeID() const
{
	return MARKDOWN_GUID;
}

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
	auto content = gcnew String(szContent);

	return m_wnd->SetTextContent(content, bResetSelection);
}

bool CMDContentControlBridge::InsertTextContent(LPCWSTR szContent, bool bAtEnd)
{
	auto content = gcnew String(szContent);

	return m_wnd->InsertTextContent(content, bAtEnd);
}

bool CMDContentControlBridge::DoIdleProcessing() 
{ 
	return m_wnd->DoIdleProcessing();
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
	if (m_pSpellCheck == nullptr)
		m_pSpellCheck = new CRichEditSpellCheck();

	m_pSpellCheck->Initialise(Win32::GetHwnd(m_wnd->SpellCheckHandle));

	return m_pSpellCheck;
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
	auto prefs = gcnew Preferences(pPrefs);
	auto key = gcnew String(szKey);

	m_wnd->SavePreferences(prefs, key);
}

void CMDContentControlBridge::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly)
{
	auto prefs = gcnew Preferences(pPrefs);
	auto key = gcnew String(szKey);

	m_wnd->LoadPreferences(prefs, key, bAppOnly);
}
