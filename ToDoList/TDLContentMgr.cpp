// TDLContentMgr.cpp: implementation of the CTDLContentMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLContentMgr.h"
#include "ToDoCommentsCtrl.h"
#include "resource.h"

#include "..\shared\localizer.h"

#include "..\interfaces\ipreferences.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

class CTDCDefaultContent : public IContent
{
public:
	CTDCDefaultContent() : m_hIcon(NULL)
	{
		m_hIcon = AfxGetApp()->LoadIcon(IDI_SIMPLETEXT);
	}

	virtual ~CTDCDefaultContent()
	{
		if (m_hIcon)
			::DestroyIcon(m_hIcon);
	}

	LPCTSTR GetTypeID() const { static LPCTSTR sID = _T("PLAIN_TEXT"); return sID; }
	LPCTSTR GetTypeDescription() const { static LPCTSTR sDesc = _T("Simple Text"); return sDesc; }
	HICON GetTypeIcon() const { return m_hIcon; }

	IContentControl* CreateCtrl(unsigned short nCtrlID, unsigned long nStyle, 
						long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
	{
		CToDoCommentsCtrl* pComments = new CToDoCommentsCtrl;

		nStyle |= ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL;
		CRect rect(nLeft, nTop, nLeft + nWidth, nTop + nHeight);

		if (pComments->Create(nStyle, rect, CWnd::FromHandle(hwndParent), nCtrlID))
			return pComments;

		// else
		delete pComments;
		return NULL;
	}

	void SetLocalizer(ITransText* pTT) { CLocalizer::Initialize(pTT); }
	void Release() { delete this; }

	int ConvertToHtml(const unsigned char* /*pContent*/, int /*nLength*/, LPCTSTR /*szCharset*/,
						LPTSTR& /*szHtml*/, LPCTSTR /*szImageDir*/) { return 0; } // not supported
	void FreeHtmlBuffer(LPTSTR& /*szHtml*/) {}
	
	void SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const 
	{
		pPrefs->WriteProfileInt(szKey, _T("InlineSpellChecking"), CToDoCommentsCtrl::IsInlineSpellCheckingEnabled());
	}
	
	void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly) 
	{
		if (!bAppOnly)
			CToDoCommentsCtrl::EnableInlineSpellChecking(pPrefs->GetProfileInt(szKey, _T("InlineSpellChecking"), FALSE));

		CToDoCommentsCtrl::SetPasteSourceUrls(pPrefs->GetProfileInt(_T("Preferences"), _T("IncludeWebLinksInCommentsPaste"), TRUE));
	}

protected:
	HICON m_hIcon;
	
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDLContentMgr::CTDLContentMgr() 
{
}

CTDLContentMgr::~CTDLContentMgr()
{
}

void CTDLContentMgr::Initialize() const
{
	if (!m_bInitialized)
	{
		CContentMgr::Initialize();

		// we need a non-const pointer to update the array
		CTDLContentMgr* pMgr = const_cast<CTDLContentMgr*>(this);

		pMgr->m_aContent.InsertAt(0, new CTDCDefaultContent);
	}
}
