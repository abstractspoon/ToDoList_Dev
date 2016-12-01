// stickiesapi.cpp : implementation file
//

#include "stdafx.h"
#include "stickiesapi.h"
#include "misc.h"
#include "filemisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const LPCTSTR STICKIES_APPWNDCLASS = _T("ZhornSoftwareStickiesMain");

/////////////////////////////////////////////////////////////////////////////
// CStickiesAPI

CStickiesAPI::CStickiesAPI(LPCTSTR szStickiesPath) 
	: 
	m_sStickiesPath(szStickiesPath),
	m_nInitialCmdID(0),
	m_hwndCallback(NULL)
{
}

CStickiesAPI::~CStickiesAPI()
{
}

BOOL CStickiesAPI::Initialize(CWnd* pCallback, int nCommandID, LPCTSTR szStickiesPath)
{
	if (!Misc::IsEmpty(szStickiesPath))
		m_sStickiesPath = szStickiesPath;

	if (!FileMisc::FileExists(m_sStickiesPath))
	{
		ASSERT(0);
		return FALSE;
	}

	// verify owner callback
	ASSERT_VALID(pCallback);

	if (!pCallback || !::IsWindow(*pCallback))
		return FALSE;

	// verify Stickies is up and running
	HWND hwndStickies = GetStickiesWindow();
	
	if (hwndStickies == NULL)
	{
		m_sStickiesPath.Empty();
		return FALSE;
	}
	
	// else
	m_sAppVer = FileMisc::GetModuleVersion(m_sStickiesPath);

	// register for callbacks if we have a callback window
	m_hwndCallback = pCallback->GetSafeHwnd();
	m_nInitialCmdID = nCommandID;

	if (m_hwndCallback != NULL)
	{
		LRESULT lr = SendCommand(m_nInitialCmdID, _T("do register"));
						
		if (lr != STICKY_SUCCESS)
		{
			m_sStickiesPath.Empty();
			m_hwndCallback = NULL;
			m_sAppVer.Empty();

			return FALSE;
		}
	}

	// success
	return TRUE;
}

void CStickiesAPI::Release()
{
	if (GetStickiesWindow(FALSE) == NULL)
		return; // nothing to do

	SendCommand(0, _T("do deregister"));

	m_hwndCallback = NULL;
	m_sStickiesPath.Empty();
	m_nInitialCmdID = 1;
	m_sAppVer.Empty();
}


/////////////////////////////////////////////////////////////////////////////
// CStickiesAPI message handlers

LRESULT CStickiesAPI::SendCommand(int nCommandID, LPCTSTR szCommand, LPCTSTR szStickyID, LPCTSTR szExtra) const
{
	HWND hwndStickies = GetStickiesWindow();

	if (hwndStickies == NULL)
		return FALSE;

	// format command string
	CString sCommand = FormatCommandString(szCommand, szStickyID, szExtra);

	// send command string
	COPYDATASTRUCT cpd = { 0 };

	cpd.dwData = nCommandID;
	cpd.cbData = sCommand.GetLength();

#ifdef _UNICODE
	LPSTR szMBCommand = Misc::WideToMultiByte(sCommand);
	cpd.lpData = (void*)szMBCommand;
#else
	cpd.lpData = (void*)(LPCTSTR)sCommand;
#endif
	
	LRESULT lr = ::SendMessage(hwndStickies, WM_COPYDATA, (WPARAM)m_hwndCallback, (LPARAM)&cpd);

#ifdef _UNICODE
	delete [] szMBCommand;
#endif

	return lr;
}

HWND CStickiesAPI::GetStickiesWindow(BOOL bAutoStart) const
{
	// first see if Stickies is already up and running
	HWND hwndStickies = ::FindWindow(NULL, STICKIES_APPWNDCLASS);

	if (hwndStickies == NULL && !m_sStickiesPath.IsEmpty() && bAutoStart)
	{
		// try (re)starting stickies
		LRESULT lr = (LRESULT)ShellExecute(NULL, NULL, m_sStickiesPath, NULL, NULL, SW_SHOWMINIMIZED);

		if (lr > 32)
		{
			int nTry = 10;

			while (nTry-- && hwndStickies == NULL)
			{
				Sleep(50);

				hwndStickies = ::FindWindow(NULL, STICKIES_APPWNDCLASS);
			}
		}
	}

	return hwndStickies;
}

CString CStickiesAPI::FormatCommandString(LPCTSTR szCommand, LPCTSTR szStickyID, LPCTSTR szExtra)
{
	CString sCommand(_T("api "));
	
	ASSERT(!Misc::IsEmpty(szCommand));
	sCommand += szCommand;

	if (!Misc::IsEmpty(szStickyID))
	{
		sCommand += ' ';
		sCommand += szStickyID;
	}

	if (!Misc::IsEmpty(szExtra))
	{
		sCommand += ' ';
		sCommand += szExtra;
	}

	return sCommand;
}
