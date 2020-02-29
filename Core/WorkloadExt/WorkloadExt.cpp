// WorkloadExt.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "WorkloadExt.h"
#include "WorkloadWnd.h"
#include "resource.h"

#include "..\shared\misc.h"
#include "..\shared\localizer.h"

#include "..\3rdParty\GdiPlus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// CWorkloadExtApp Class
//////////////////////////////////////////////////////////////////////

static CWorkloadExtApp theApp;

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

CWorkloadExtApp::CWorkloadExtApp()
{
}

CWorkloadExtApp::~CWorkloadExtApp()
{

}

void CWorkloadExtApp::Release()
{
	// Don't delete because we are a static instance
	CGdiPlus::Free();
}

IUIExtensionWindow* CWorkloadExtApp::CreateExtWindow(UINT nCtrlID, DWORD nStyle, 
													long nLeft, long nTop, long nWidth, long nHeight, 
													HWND hwndParent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CWorkloadWnd* pWindow = new CWorkloadWnd;

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

void CWorkloadExtApp::SetLocalizer(ITransText* pTT)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CLocalizer::Initialize(pTT);
}

BOOL CWorkloadExtApp::InitInstance()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// Set this before anything else
	CWinHelpButton::SetDefaultIcon(LoadIcon(IDI_HELP_BUTTON));
	
	m_icon.Load(IDR_WORKLOAD);

	return CWinApp::InitInstance();
}

void CWorkloadExtApp::SavePreferences(IPreferences* /*pPrefs*/, LPCWSTR /*szKey*/) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// TODO
}

void CWorkloadExtApp::LoadPreferences(const IPreferences* /*pPrefs*/, LPCWSTR /*szKey*/)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// TODO
}
