// StatisticsExt.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "resource.h"
#include "StatisticsExt.h"
#include "StatisticsWnd.h"

#include "..\shared\misc.h"
#include "..\shared\localizer.h"

#include "..\Interfaces\iuiextension.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// CStatisticsExtApp Class
//////////////////////////////////////////////////////////////////////

static CStatisticsExtApp theApp;

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

CStatisticsExtApp::CStatisticsExtApp() : m_hIcon(NULL)
{
}

CStatisticsExtApp::~CStatisticsExtApp()
{

}

void CStatisticsExtApp::Release()
{
	// don't delete because we are static
}

IUIExtensionWindow* CStatisticsExtApp::CreateExtWindow(UINT nCtrlID, DWORD nStyle, 
													long nLeft, long nTop, long nWidth, long nHeight, 
													HWND hwndParent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CStatisticsWnd* pWindow = new CStatisticsWnd;

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

void CStatisticsExtApp::SetLocalizer(ITransText* pTT)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CLocalizer::Initialize(pTT);
}

BOOL CStatisticsExtApp::InitInstance()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_hIcon = LoadIcon(IDR_STATISTICS);
	
	return CWinApp::InitInstance();
}

void CStatisticsExtApp::SavePreferences(IPreferences* /*pPrefs*/, LPCWSTR /*szKey*/) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// TODO
}

void CStatisticsExtApp::LoadPreferences(const IPreferences* /*pPrefs*/, LPCWSTR /*szKey*/)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// TODO
}

