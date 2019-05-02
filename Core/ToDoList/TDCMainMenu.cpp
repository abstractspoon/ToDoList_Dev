// MenuEx.cpp: implementation of the CTDCMainMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "TDCMainMenu.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCMainMenu::CTDCMainMenu()
{

}

CTDCMainMenu::~CTDCMainMenu()
{
}

BOOL CTDCMainMenu::LoadMenu(BOOL bTranslate, BOOL bRecursiveTranslate)
{
	return LoadMenu(NULL, bTranslate, bRecursiveTranslate);
}

BOOL CTDCMainMenu::LoadMenu(HWND hwndRef, BOOL bTranslate, BOOL bRecursiveTranslate)
{
	if (!CEnMenu::LoadMenu(IDR_MAINFRAME, hwndRef, bTranslate, bRecursiveTranslate))
		return FALSE;

	// else
	if (bTranslate && bRecursiveTranslate)
	{
		TranslateDynamicMenuItems(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE16, _T("Recent Tasklist %d"));
		TranslateDynamicMenuItems(ID_WINDOW1, ID_WINDOW16, _T("Window %d"));
		TranslateDynamicMenuItems(ID_TOOLS_USERTOOL1, ID_TOOLS_USERTOOL50, _T("User Defined Tool %d"));
		TranslateDynamicMenuItems(ID_FILE_OPEN_USERSTORAGE1, ID_FILE_OPEN_USERSTORAGE16, _T("3rd Party Storage %d"));
		TranslateDynamicMenuItems(ID_FILE_SAVE_USERSTORAGE1, ID_FILE_SAVE_USERSTORAGE16, _T("3rd Party Storage %d"));
		TranslateDynamicMenuItems(ID_SHOWVIEW_UIEXTENSION1, ID_SHOWVIEW_UIEXTENSION16, _T("Task View Visibility %d"));
	}

	return TRUE;
}

