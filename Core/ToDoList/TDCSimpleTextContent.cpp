// tdlcommentsctrl.cpp : implementation file
//

#include "stdafx.h"
#include "TDCSimpleTextContent.h"
#include "TDLSimpleTextContentCtrl.h"
#include "resource.h"

#include "..\shared\localizer.h"

#include "..\Interfaces\ipreferences.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

CTDCSimpleTextContent::CTDCSimpleTextContent()
{
	m_icon.Load(IDI_SIMPLETEXT);
}

CTDCSimpleTextContent::~CTDCSimpleTextContent()
{
}

LPCTSTR CTDCSimpleTextContent::GetTypeID() const 
{ 
	static LPCTSTR sID = _T("PLAIN_TEXT"); 
	return sID; 
}

LPCTSTR CTDCSimpleTextContent::GetTypeDescription() const 
{ 
	static LPCTSTR sDesc = _T("Simple Text"); 
	return sDesc; 
}

HICON CTDCSimpleTextContent::GetTypeIcon() const 
{ 
	return m_icon; 
}

IContentControl* CTDCSimpleTextContent::CreateCtrl(unsigned short nCtrlID, unsigned long nStyle, 
													long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
{
	CTDLSimpleTextContentCtrl* pComments = new CTDLSimpleTextContentCtrl;
	CRect rect(nLeft, nTop, nLeft + nWidth, nTop + nHeight);

	if (pComments->Create(nStyle, rect, CWnd::FromHandle(hwndParent), nCtrlID))
		return pComments;

	// else
	delete pComments;
	return NULL;
}

void CTDCSimpleTextContent::SetLocalizer(ITransText* pTT) 
{ 
	CLocalizer::Initialize(pTT); 
}

void CTDCSimpleTextContent::Release() 
{ 
	delete this; 
}

int CTDCSimpleTextContent::ConvertToHtml(const unsigned char* /*pContent*/, int /*nLength*/, LPCTSTR /*szCharset*/,
										LPTSTR& /*szHtml*/, LPCTSTR /*szImageDir*/) 
{ 
	return 0; // not supported
}

void CTDCSimpleTextContent::FreeHtmlBuffer(LPTSTR& /*szHtml*/) 
{
	// not required
}

void CTDCSimpleTextContent::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const 
{
	pPrefs->WriteProfileInt(szKey, _T("InlineSpellChecking"), CTDLSimpleTextContentCtrl::IsInlineSpellCheckingEnabled());
}

void CTDCSimpleTextContent::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly) 
{
	if (!bAppOnly)
		CTDLSimpleTextContentCtrl::EnableInlineSpellChecking(pPrefs->GetProfileInt(szKey, _T("InlineSpellChecking"), FALSE));

	CTDLSimpleTextContentCtrl::SetPasteSourceUrls(pPrefs->GetProfileInt(_T("Preferences"), _T("IncludeWebLinksInCommentsPaste"), TRUE));
}

