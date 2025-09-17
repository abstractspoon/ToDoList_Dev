// stickiesapi.cpp : implementation file
//

#include "stdafx.h"
#include "stickieswnd.h"

#include "misc.h"
#include "Filemisc.h"
#include "Graphicsmisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const LPCTSTR STICKIES_APPWNDCLASS = _T("ZhornSoftwareStickiesMain");

/////////////////////////////////////////////////////////////////////////////
// CStickiesWnd

CStickiesWnd::CStickiesWnd(const CString& sStickiesPath) 
	: 
	m_nCommandID(1), m_stickies(sStickiesPath)

{
}

CStickiesWnd::~CStickiesWnd()
{
}


BEGIN_MESSAGE_MAP(CStickiesWnd, CWnd)
	//{{AFX_MSG_MAP(CStickiesWnd)
	ON_WM_COPYDATA()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CStickiesWnd::Initialize(CWnd* pOwner, const CString& sStickiesPath)
{
	// free current wnd
	Release();

	// verify owner callback
	ASSERT_VALID(pOwner);

	if (!pOwner || !::IsWindow(*pOwner))
		return FALSE;

	if (!Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), pOwner, (UINT)IDC_STATIC))
	{
		FileMisc::LogTextRaw(_T("CStickiesWnd::Create() failed"));
		return FALSE;
	}

	return InitStickiesAPI(sStickiesPath);
}

void CStickiesWnd::Release()
{
	if (GetSafeHwnd())
		DestroyWindow();

	// reset state
	m_nCommandID = 1;
	m_mapReplies.RemoveAll();
}

BOOL CStickiesWnd::InitStickiesAPI(const CString& sStickiesPath)
{
	ASSERT_VALID(this);
	ASSERT(GetSafeHwnd());

	if (GetSafeHwnd() == NULL)
		return FALSE;

	if (!m_stickies.Initialize(this, m_nCommandID++, sStickiesPath)) // we are the callback
	{
		DestroyWindow();
		return FALSE;
	}

	// else	test the connection
#ifdef _DEBUG
// 	CString sReply;
// 
// 	ASSERT(SendMessage(_T("do ping"), sReply) == STICKY_SUCCESS);
// 	ASSERT(!sReply.IsEmpty());
// 
// 	ASSERT(CreateSticky(_T("InitStickiesAPI(succeeded)"), sReply));
// 	ASSERT(!sReply.IsEmpty());
// 
// 	Sleep(1000);
// 	ASSERT(DeleteSticky(sReply));
#endif

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CStickiesWnd message handlers

BOOL CStickiesWnd::CreateSticky(const CString& sTitle, CString& sStickyID, const CString& sContent, BOOL bRTF)
{
	CString sReply;

	if (SendMessage(_T("do new sticky"), sReply))
	{
		CStringArray aParts;
		
		if (Misc::Split(sReply, aParts, ':', TRUE) == 2)
		{
			sStickyID = aParts[1];
			Misc::Trim(sStickyID);

			// set other sticky attributes
			if (!sStickyID.IsEmpty())
			{
				VERIFY(SetStickyAttribute(sStickyID, _T("ontop"), _T("1")));
				VERIFY(SetStickyTitle(sStickyID, sTitle));

				if (!sContent.IsEmpty())
					VERIFY(SetStickyContent(sStickyID, sContent, bRTF));
			}

			return !sStickyID.IsEmpty();
		}
	}

	// all else
	FileMisc::LogTextRaw(_T("CStickiesWnd::CreateSticky() failed"));

	return FALSE;
}

BOOL CStickiesWnd::DeleteSticky(const CString& sStickyID)
{
	return SendMessage(_T("do desktop"), CString(), sStickyID, _T("delete"));
}

BOOL CStickiesWnd::HasSticky(const CString& sStickyID)
{
	return SendMessage(_T("get desktop"), CString(), sStickyID, _T("title"));
}

BOOL CStickiesWnd::SendMessage(const CString& sCommand)
{
	return SendMessage(sCommand, CString());
}

BOOL CStickiesWnd::SendMessage(const CString& sCommand, CString& sReply, const CString& sStickyID, const CString& sExtra)
{
	sReply.Empty();

	int nCommandID = m_nCommandID++;
	
	if (!m_stickies.SendCommand(nCommandID, sCommand, sStickyID, sExtra))
		return FALSE;

	// else poll for response
	int nTry = 10;

	while (nTry--)
	{
		Sleep(50);

		if (m_mapReplies.Lookup(nCommandID, sReply))
		{
			TRACE(_T("CStickiesWnd::SendMessage(%s, %s, %s) = %s\n"), sCommand, sStickyID, sExtra.Left(300), sReply);

			m_mapReplies.RemoveKey(nCommandID);
			break; // success
		}
	}

	// no response?
	if (nTry == -1)
	{
		FileMisc::LogTextRaw(_T("CStickiesWnd::SendMessage() timed out"));
		return FALSE;
	}

	return TRUE;
}

BOOL CStickiesWnd::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	int nCommandID = pCopyDataStruct->dwData;
	
	if (nCommandID > 0 && nCommandID < m_nCommandID)
	{
		// return string is multi-byte so we let CString
		// convert it to Unicode as required
		CString sReply((LPCSTR)(pCopyDataStruct->lpData), pCopyDataStruct->cbData);
		m_mapReplies[nCommandID] = sReply;
	}

	return CWnd::OnCopyData(pWnd, pCopyDataStruct);
}

CString CStickiesWnd::FormatStickyCreationString(const CString& sTitle, time_t tAlarm, COLORREF color)
{
	const LPCTSTR HEADERFORMAT  = _T("<TITLE>%s<X>0625<Y>0505<WIDTH>%d<HEIGHT>1000");
	const LPCTSTR COLORFORMAT   = _T("<COLR>%d<COLG>%d<COLB>%d");
	const LPCTSTR ALARMFORMAT   = _T("<SLEEPING>%d<SLEEPNEXTWAKE>%ld<SLEEPALARM>%d");
	
	CString sHeader, sColor, sAlarm;
	
	// always
	sHeader.Format(HEADERFORMAT, sTitle, GraphicsMisc::ScaleByDPIFactor(300));
	
	if (color != 0)
		sColor.Format(COLORFORMAT, GetRValue(color), GetGValue(color), GetBValue(color));
	
	if (tAlarm > 0)
		sAlarm.Format(ALARMFORMAT, 1, (long int)tAlarm, 1);
	
	return (sHeader + sColor + sAlarm);
}

BOOL CStickiesWnd::SetStickyTitle(const CString& sStickyID, const CString& sTitle)
{
	if (m_stickies.GetMajorVersion() >= 8)
		return SetStickyAttribute(sStickyID, _T("utitle"), EncodeString(sTitle));
	
	// else
	return SetStickyAttribute(sStickyID, _T("title"), sTitle);
}

BOOL CStickiesWnd::SetStickyContent(const CString& sStickyID, const CString& sContent, BOOL bRTF)
{
	if (bRTF)
	{
		return SetStickyAttribute(sStickyID, _T("rtf"), sContent);
	}
	else if (m_stickies.GetMajorVersion() >= 8)
	{
		return SetStickyAttribute(sStickyID, _T("utext"), EncodeString(sContent));
	}

	// else
	return SetStickyAttribute(sStickyID, _T("text"), sContent);
}

BOOL CStickiesWnd::SetStickyAttribute(const CString& sStickyID, const CString& sAttribute, const CString& sValue)
{
	if (sStickyID.IsEmpty() || sAttribute.IsEmpty() || sValue.IsEmpty())
		return FALSE;

	return SendMessage(_T("set desktop"), CString(), sStickyID, sAttribute + ' ' + sValue);
}

CString CStickiesWnd::EncodeString(const CString& sText)
{
	CString sEncode, fmt;
	
	for (int f = 0; f < sText.GetLength(); f++)
	{
		fmt.Format(_T("%04X"), sText[f]);
		sEncode += fmt;
	}

	return sEncode;
}
