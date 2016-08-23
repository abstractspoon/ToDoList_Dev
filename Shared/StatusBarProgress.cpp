// StatusBarProgress.cpp: implementation of the CStatusBarProgress class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StatusBarProgress.h"
#include "winclasses.h"
#include "wclassdefines.h"
#include "graphicsmisc.h"

#include "..\3rdparty\progressthread.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatusBarProgress::CStatusBarProgress(HWND hwndStatusBar, const CString& sPrompt, int nPane) :
	m_hwndStatusBar(NULL), m_nSBPane(0), m_pThread(NULL)
{
	BeginProgress(hwndStatusBar, sPrompt, nPane);
}

CStatusBarProgress::~CStatusBarProgress()
{
	EndProgress();
}

BOOL CStatusBarProgress::BeginProgress(HWND hwndStatusBar, const CString& sPrompt, int nPane)
{ 
	// end existing thread
	if (m_pThread)
	{
		m_pThread->EndThread(); // will self destruct
		m_pThread = NULL;
	}

	// status bar must be NULL or valid
	CStatusBar* pSB = (CStatusBar*)CWnd::FromHandlePermanent(hwndStatusBar);

	if (!pSB || !CWinClasses::IsClass(hwndStatusBar, WC_STATUSBAR))
		return FALSE;

	// start new thread
	m_sPrevPrompt = pSB->GetPaneText(nPane);
	m_hwndStatusBar = hwndStatusBar;
	m_nSBPane = nPane;

	int nLenPrompt = GraphicsMisc::GetTextWidth(sPrompt, *pSB) + 10;

	m_pThread = new CProgressThread(m_hwndStatusBar, CRect(nLenPrompt, 3, nLenPrompt + 150, 16), TRUE);
		
	if (m_pThread && m_pThread->CreateThread())
	{
		pSB->SetPaneText(m_nSBPane, sPrompt);
		m_pThread->Play(100);

		return TRUE;
	}

	return FALSE;
}

void CStatusBarProgress::EndProgress()
{
	if (m_pThread != NULL) 
	{
		// hide progress bar before updating the prompt
		m_pThread->EndThread();
		m_pThread = NULL;

		// restore previous state
		CStatusBar* pSB = (CStatusBar*)CWnd::FromHandlePermanent(m_hwndStatusBar);
		ASSERT (pSB);

		if (pSB)
			pSB->SetPaneText(m_nSBPane, m_sPrevPrompt);

		m_hwndStatusBar = NULL;
		m_sPrevPrompt.Empty();
		m_nSBPane = 0;
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatusBarProgressProxy::CStatusBarProgressProxy(CStatusBarProgress* pProxy, HWND hwndStatusBar, const CString& sPrompt, int nPane, BOOL bIgnoreIfActive)
	: m_pProxy(pProxy)
{
	if (m_pProxy)
	{
		if (bIgnoreIfActive && m_pProxy->IsActive())
			m_pProxy = NULL;
		else
			m_pProxy->BeginProgress(hwndStatusBar, sPrompt, nPane);
	}
}

CStatusBarProgressProxy::~CStatusBarProgressProxy()
{
	if (m_pProxy)
	{
		m_pProxy->EndProgress();
		m_pProxy = NULL;
	}
}

