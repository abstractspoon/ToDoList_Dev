// GanttChartExt.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "GanttChartExt.h"
#include "GanttChartWnd.h"
#include "resource.h"

#include "..\shared\misc.h"
#include "..\shared\localizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// CGanttChartExtApp Class
//////////////////////////////////////////////////////////////////////

static CGanttChartExtApp theApp;

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

CGanttChartExtApp::CGanttChartExtApp() : m_hIcon(NULL)
{
}

CGanttChartExtApp::~CGanttChartExtApp()
{

}

void CGanttChartExtApp::Release()
{
	// don't delete because we are static
}

IUIExtensionWindow* CGanttChartExtApp::CreateExtWindow(UINT nCtrlID, DWORD nStyle, 
													long nLeft, long nTop, long nWidth, long nHeight, 
													HWND hwndParent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CGanttChartWnd* pWindow = new CGanttChartWnd;

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

void CGanttChartExtApp::SetLocalizer(ITransText* pTT)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CLocalizer::Initialize(pTT);
}

BOOL CGanttChartExtApp::InitInstance()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// Set this before anything else
	CWinHelpButton::SetDefaultIcon(LoadIcon(IDI_HELP_BUTTON));
	
	m_hIcon = LoadIcon(IDR_GANTTCHART);

	return CWinApp::InitInstance();
}

void CGanttChartExtApp::SavePreferences(IPreferences* /*pPrefs*/, LPCWSTR /*szKey*/) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// TODO
}

void CGanttChartExtApp::LoadPreferences(const IPreferences* /*pPrefs*/, LPCWSTR /*szKey*/)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// TODO
}
