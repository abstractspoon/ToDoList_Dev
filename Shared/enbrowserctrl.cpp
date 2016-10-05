// enbrowserctrl.cpp : implementation file
//

#include "stdafx.h"
#include "enbrowserctrl.h"
#include "regkey.h"
#include "misc.h"

//fabio_2005
#if _MSC_VER >= 1300
#	include <comdef.h>
#else
#	include <comutil.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

enum { EBC_NONE, EBC_PRINT, EBC_PRINTPREVIEW };

/////////////////////////////////////////////////////////////////////////////

#ifndef OLECMDID_PRINT2
#	define OLECMDID_PRINT2 49
#	define OLECMDID_PRINTPREVIEW2 50
#endif

/////////////////////////////////////////////////////////////////////////////

const LPCTSTR REG_PAGESETUP = _T("Software\\Microsoft\\Internet Explorer\\PageSetup");

/////////////////////////////////////////////////////////////////////////////
// CEnBrowserCtrl

CEnBrowserCtrl::CEnBrowserCtrl() : m_nAction(EBC_NONE), m_bPrevPrintBkgnd(-1)
{
}

CEnBrowserCtrl::~CEnBrowserCtrl()
{
	RestorePrintBkgnd();
}


BEGIN_MESSAGE_MAP(CEnBrowserCtrl, CWebBrowserCtrl)
	//{{AFX_MSG_MAP(CEnBrowserCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CEnBrowserCtrl, CWebBrowserCtrl)
    //{{AFX_EVENTSINK_MAP(CBrowserDlg)
	//}}AFX_EVENTSINK_MAP
//	ON_EVENT_REFLECT(CEnBrowserCtrl, 252, OnNavigateComplete2, VTS_DISPATCH VTS_PVARIANT)
	ON_EVENT_REFLECT(CEnBrowserCtrl, 259, OnDocumentComplete, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnBrowserCtrl message handlers

void CEnBrowserCtrl::InitPrintBkgnd(BOOL bPrintBkgnd)
{
	RestorePrintBkgnd();

	if (bPrintBkgnd != -1)
	{
		CRegKey2 reg;

		if (reg.Open(HKEY_CURRENT_USER, REG_PAGESETUP, FALSE) == ERROR_SUCCESS)
		{
			CString sYesNo;
			reg.Read(_T("Print_Background"), sYesNo);

			BOOL bYes = (sYesNo == _T("yes"));

			if ((bYes && !bPrintBkgnd) || (!bYes && bPrintBkgnd))
			{
				m_bPrevPrintBkgnd = bYes;
				reg.Write(_T("Print_Background"), (bPrintBkgnd ? _T("yes") : _T("no")));
			}
		}
	}
}

void CEnBrowserCtrl::RestorePrintBkgnd()
{
	if (m_bPrevPrintBkgnd != -1)
	{
		CRegKey2 reg;
		
		if (reg.Open(HKEY_CURRENT_USER, REG_PAGESETUP, FALSE) == ERROR_SUCCESS)
		{
			reg.Write(_T("Print_Background"), (m_bPrevPrintBkgnd ? _T("yes") : _T("no")));
		}
		
		m_bPrevPrintBkgnd = -1;
	}
}

void CEnBrowserCtrl::Print(LPCTSTR szFile, BOOL bPrintBkgnd)
{
	InitPrintBkgnd(bPrintBkgnd);

	if (Misc::IsEmpty(szFile))
	{
		SafeExecWB(OLECMDID_PRINT, OLECMDEXECOPT_DODEFAULT, NULL, NULL);
	}
	else
	{
		COleVariant vFlags(0L), vFrame(_T("")), vData(_T("")), vHeaders(_T(""));
			
		// navigate to print file
		m_nAction = EBC_PRINT;
		Navigate(szFile, vFlags, vFrame, vData, vHeaders);
	}
}

void CEnBrowserCtrl::PrintPreview(LPCTSTR szFile, BOOL bPrintBkgnd)
{
	InitPrintBkgnd(bPrintBkgnd);
	
	if (Misc::IsEmpty(szFile))
	{
		SafeExecWB(OLECMDID_PRINTPREVIEW, OLECMDEXECOPT_DODEFAULT, NULL, NULL);
	}
	else
	{
		COleVariant vFlags(0L), vFrame(_T("")), vData(_T("")), vHeaders(_T(""));
			
		// navigate to print file
		m_nAction = EBC_PRINTPREVIEW;
		Navigate(szFile, vFlags, vFrame, vData, vHeaders);
	}
}

void CEnBrowserCtrl::OnDocumentComplete(LPDISPATCH /*pDisp*/, VARIANT FAR* /*URL*/) 
//void CEnBrowserCtrl::OnNavigateComplete2(LPDISPATCH /*pDisp*/, VARIANT FAR* /*URL*/) 
{
	switch (m_nAction)
	{
	case EBC_PRINT:
		SafeExecWB(OLECMDID_PRINT2, OLECMDEXECOPT_DODEFAULT, NULL, NULL);
		break;
		
	case EBC_PRINTPREVIEW:
		SafeExecWB(OLECMDID_PRINTPREVIEW2, OLECMDEXECOPT_DODEFAULT, NULL, NULL);
		break;
	}

	m_nAction = EBC_NONE;
}

BOOL CEnBrowserCtrl::SafeExecWB(long cmdID, long cmdexecopt, VARIANT* pvaIn, VARIANT* pvaOut) 
{
	try
	{
		ExecWB(cmdID, cmdexecopt, pvaIn, pvaOut);
	}
	catch (CException* /*e*/)
	{
		// fallback for unhandled options
		switch (cmdID)
		{
		case OLECMDID_PRINT2:
			return SafeExecWB(OLECMDID_PRINT, cmdexecopt, pvaIn, pvaOut);

		case OLECMDID_PRINTPREVIEW2:
			return SafeExecWB(OLECMDID_PRINTPREVIEW, cmdexecopt, pvaIn, pvaOut);

		default:;
			// fall thru
		}

		return FALSE;
	}

	return TRUE;
}