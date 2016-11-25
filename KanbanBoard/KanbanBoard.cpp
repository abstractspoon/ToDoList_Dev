// KanbanBoard.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "KanbanBoard.h"
#include "KanbanWnd.h"
#include "resource.h"

#include "..\shared\Localizer.h"
#include "..\shared\TimeHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKanbanBoardApp

static CKanbanExtApp theApp;

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

CKanbanExtApp::CKanbanExtApp() : m_hIcon(NULL)
{
}

CKanbanExtApp::~CKanbanExtApp()
{
	
}

void CKanbanExtApp::Release()
{
}

IUIExtensionWindow* CKanbanExtApp::CreateExtWindow(UINT nCtrlID, DWORD nStyle, 
												   long nLeft, long nTop, long nWidth, long nHeight, 
												   HWND hwndParent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CKanbanWnd* pWindow = new CKanbanWnd;
	
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

void CKanbanExtApp::SetLocalizer(ITransText* pTT)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CLocalizer::Initialize(pTT);

	// Initialise time units
	CTimeHelper::SetUnits(THU_MINS,		CLocalizer::TranslateText(_T("m")));
	CTimeHelper::SetUnits(THU_HOURS,	CLocalizer::TranslateText(_T("H")));
	CTimeHelper::SetUnits(THU_DAYS,		CLocalizer::TranslateText(_T("D")));
	CTimeHelper::SetUnits(THU_WEEKDAYS, CLocalizer::TranslateText(_T("d")));
	CTimeHelper::SetUnits(THU_WEEKS,	CLocalizer::TranslateText(_T("W")));
	CTimeHelper::SetUnits(THU_MONTHS,	CLocalizer::TranslateText(_T("M")));
	CTimeHelper::SetUnits(THU_YEARS,	CLocalizer::TranslateText(_T("Y")));
}

BOOL CKanbanExtApp::InitInstance()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// Set this before anything else
	CWinHelpButton::SetDefaultIcon(LoadIcon(IDI_HELP_BUTTON));

	m_hIcon = LoadIcon(IDR_KANBAN);

	if (m_pszAppName)
		free((void*)m_pszAppName);

	m_pszAppName = _tcsdup(GetMenuText());
	
	return CWinApp::InitInstance();
}

void CKanbanExtApp::SavePreferences(IPreferences* /*pPrefs*/, LPCWSTR /*szKey*/) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// TODO
}

void CKanbanExtApp::LoadPreferences(const IPreferences* /*pPrefs*/, LPCWSTR /*szKey*/)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// TODO
}
