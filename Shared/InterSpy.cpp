// InterSpy.cpp: implementation of the CInterSpy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InterSpy.h"
#include "ISMsgManager.h"

#include "afxpriv.h"
#include <atlbase.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__; 
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

CInterSpy::CInterSpy() : m_dwFlags(0), m_hwndOutput(NULL)
{
}

CInterSpy::~CInterSpy()
{
}

BOOL CInterSpy::Initialize(DWORD dwFlags)
{
	if (GetInstance().InitHooks(HM_CALLWNDPROC | HM_GETMESSAGE))
	{
		GetInstance().SetFlags(dwFlags);

		return TRUE;
	}

	return FALSE;
}

void CInterSpy::Release()
{
  GetInstance().Release();
}

void CInterSpy::IncludeMsg(UINT uMsg)
{
	GetInstance().CInterSpy::IncludeMsg(uMsg, TRUE);
}

void CInterSpy::ExcludeMsg(UINT uMsg)
{
	GetInstance().CInterSpy::IncludeMsg(uMsg, FALSE);
}

BOOL CInterSpy::OnCallWndProc(const MSG& msg)
{
	return ProcessMessage(msg);
}

BOOL CInterSpy::OnGetMessage(const MSG& msg)
{
	return ProcessMessage(msg);
}

void CInterSpy::SetFlags(DWORD dwFlags) 
{
	m_dwFlags = dwFlags;

	if (m_dwFlags & IS_NOKICKIDLE)
		IncludeMsg(WM_KICKIDLE, FALSE);

	HWND hwndOutput = GetOutputWnd(TRUE);

	if ((m_dwFlags & IS_RESETONSTART) && hwndOutput)
	{
		// build copy data msg
		IS_COPYDATA data;
		data.uCmd = IS_CMD_RESET;
		
		COPYDATASTRUCT cds = { 1, sizeof(data), &data};
		
		::SendMessage(hwndOutput, WM_COPYDATA, 0, (LPARAM)&cds);
	}
}

void CInterSpy::IncludeMsg(UINT uMsg, BOOL bInclude)
{
	if (bInclude)
		m_mapExclusions.RemoveKey(uMsg);
	else
		m_mapExclusions[uMsg] = 1;
}

HWND CInterSpy::GetOutputWnd(BOOL bStartup)
{
	static BOOL bReentrancy = FALSE;
	if (bReentrancy)
		return NULL;

	bReentrancy = TRUE;

	if (!m_hwndOutput || !::IsWindow(m_hwndOutput))
		m_hwndOutput = NULL;

	if (!m_hwndOutput)
		m_hwndOutput = ::FindWindow("InterspyWnd", NULL);

	if (bStartup && !m_hwndOutput && (m_dwFlags & IS_AUTOSTARTOUTPUT))
	{
		// lookup cached path in registry
		if (m_sOutputWndPath.IsEmpty())
		{
			CRegKey reg;
			
			if (reg.Open(HKEY_LOCAL_MACHINE, "Software\\AbstractSpoon\\InterSpy") == ERROR_SUCCESS)
			{
				char szPath[MAX_PATH + 1];
				DWORD dwLen = MAX_PATH;
				
				if (reg.QueryValue(szPath, "InterSpyWndPath", &dwLen) == ERROR_SUCCESS)
					m_sOutputWndPath = szPath;
			}
		}
		
		BOOL bCancelled = FALSE;

		do
		{
			if (m_sOutputWndPath.IsEmpty())
			{
				if (IDCANCEL == MessageBox(NULL, "Interspy could not determine the location of the InterSpy output window.\n\nClick 'OK' to browse to the location of the InterSpy output window, \nor 'Cancel' if you which to start the output window manually", 
					"InterSpy", MB_OKCANCEL))
				{
					bCancelled = TRUE;
				}
				else
				{
					CFileDialog dialog(TRUE, NULL, m_sOutputWndPath, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
						"InterSpy output windows|InterSpyWnd.exe||");
					
					if (dialog.DoModal() == IDOK)
					{
						m_sOutputWndPath = dialog.GetPathName();
					}
					else
						bCancelled = TRUE;
				}
			}

			if (!bCancelled && !m_sOutputWndPath.IsEmpty())
			{
				// try to run the exe
				TRACE ("running '%s'\n", m_sOutputWndPath);
				if ((int)ShellExecute(NULL, NULL, m_sOutputWndPath, NULL, NULL, SW_SHOW) > 32)
				{
					int nCount = 10;
					
					while (nCount-- && !m_hwndOutput)
					{
						m_hwndOutput = ::FindWindow("InterspyWnd", NULL);
						Sleep(100);
					}
				}
			}
		}
		while (!m_hwndOutput && !bCancelled);

		if (bCancelled)
			m_dwFlags &= ~IS_AUTOSTARTOUTPUT;
		else
		{
			// write to registry
			CRegKey reg;
			
			reg.SetValue(HKEY_LOCAL_MACHINE, "Software\\AbstractSpoon\\InterSpy", m_sOutputWndPath, "InterSpyWndPath");
		}
	}

	bReentrancy = FALSE;

	return m_hwndOutput;
}

BOOL CInterSpy::ProcessMessage(const MSG& msg)
{
	// check exclusions
	static UINT uTemp;

	if (m_mapExclusions.Lookup(msg.message, uTemp))
		return FALSE;

	// check duplicates
	if (m_dwFlags & IS_NODUPLICATES)
	{
		UINT uLastMsg;

		if (m_mapLastMsgs.Lookup(msg.hwnd, uLastMsg) && uLastMsg == msg.message)
      return FALSE;
	}

	m_mapLastMsgs[msg.hwnd] = msg.message;

	HWND hwndOutput = GetOutputWnd();

	if (!hwndOutput)
    return FALSE;

	// else
	CString sMsg;
	CStringArray aParams;

	if (CISMsgManager::HandleMsg(&msg, sMsg, &aParams))
	{
		// build copy data msg
		IS_COPYDATA data;

		data.uCmd = IS_CMD_ADDMSG;
		data.wParam = (WPARAM)msg.hwnd;
		data.lParam = msg.message;

		int nParams = aParams.GetSize();

		if (nParams)
		{
			sMsg += '|';

			for (int nParam = 0; nParam < nParams; nParam++)
			{
				sMsg += aParams[nParam];

				if (nParam < nParams - 1)
					sMsg += '|'; // delimeter
			}
		}

		lstrcpy(data.sMsg, sMsg);

		COPYDATASTRUCT cds = { 1, sizeof(data), &data};

		::SendMessage(hwndOutput, WM_COPYDATA, 0, (LPARAM)&cds);
    return TRUE;
	}

  return FALSE;
}

