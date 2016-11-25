// CalendarExt.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Resource.h"
#include "calendarext.h"
#include "calendarwnd.h"

#include "..\shared\misc.h"
#include "..\shared\localizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////

static CCalendarExtApp theApp;

//////////////////////////////////////////////////////////////////////

DLL_DECLSPEC IUIExtension* CreateUIExtensionInterface()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return &theApp;
}

DLL_DECLSPEC int GetInterfaceVersion() 
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return IUIEXTENSION_VERSION; 
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalendarExtApp::CCalendarExtApp() : m_hIcon(NULL)
{
}

CCalendarExtApp::~CCalendarExtApp()
{
	DestroyIcon(m_hIcon);
}

void CCalendarExtApp::Release()
{
	// do nothing
}

IUIExtensionWindow* CCalendarExtApp::CreateExtWindow(UINT nCtrlID, DWORD nStyle, 
													long nLeft, long nTop, long nWidth, long nHeight, 
													HWND hwndParent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CCalendarWnd* pWindow = new CCalendarWnd;

	if (pWindow)
	{
		CRect rect(nLeft, nTop, nLeft + nWidth, nTop + nHeight);

		if (pWindow->Create(nStyle, rect, CWnd::FromHandle(hwndParent), nCtrlID))
		{
			return pWindow;
		}
	}

	delete pWindow;
	return NULL;
}

void CCalendarExtApp::SetLocalizer(ITransText* pTT)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CLocalizer::Initialize(pTT);
}

BOOL CCalendarExtApp::InitInstance()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// Set this before anything else
	CWinHelpButton::SetDefaultIcon(LoadIcon(IDI_HELP_BUTTON));
	
	m_hIcon = LoadIcon(IDR_CALENDAR);
	
	return CWinApp::InitInstance();
}

void CCalendarExtApp::SavePreferences(IPreferences* /*pPrefs*/, LPCWSTR /*szKey*/) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// TODO
}

void CCalendarExtApp::LoadPreferences(const IPreferences* /*pPrefs*/, LPCWSTR /*szKey*/)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// TODO
}
