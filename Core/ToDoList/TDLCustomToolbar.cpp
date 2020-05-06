// TDLCustomToolbar.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLCustomToolbar.h"
#include "TDCMainMenu.h"

#include "..\Shared\ShortcutManager.h"
#include "..\Shared\EnMenu.h"
#include "..\Shared\GraphicsMisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLCustomToolbar

CTDLCustomToolbar::CTDLCustomToolbar()
{
}

CTDLCustomToolbar::~CTDLCustomToolbar()
{
}


BEGIN_MESSAGE_MAP(CTDLCustomToolbar, CEnToolBar)
	//{{AFX_MSG_MAP(CTDLCustomToolbar)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLCustomToolbar message handlers

BOOL CTDLCustomToolbar::SetButtons(const CToolbarButtonArray& aButtons, 
								   const CTDCMainMenu& mainMenu,
								   const CShortcutManager& mgrShortcuts)
{
	if (!GetSafeHwnd())
	{
		ASSERT(0);
		return FALSE;
	}

	CTDCImageList ilButtons;

	if (!ilButtons.LoadDefaultImages(TRUE))
	{
		ASSERT(0);
		return FALSE;
	}
	
	SetImageSize(16, 16);

	for (int nBtn = 0; nBtn < aButtons.GetSize(); nBtn++)
	{
		const TOOLBARBUTTON& tb = aButtons[nBtn];
		TBBUTTON tbb = { -1, tb.nMenuID, 0, TBSTYLE_SEP, 0, 0, (UINT)-1 };
		
		if (tb.nMenuID > 0)
		{
			tbb.iBitmap = ilButtons.GetImageIndex(tb.sImageID);
			tbb.fsStyle = TBSTYLE_BUTTON;
		}

		if (!GetToolBarCtrl().InsertButton(nBtn, &tbb))
			return FALSE;
	}

	m_ilNormal.Attach(ilButtons.Detach());
	GetToolBarCtrl().SetImageList(&m_ilNormal);
	
	// tooltips
	if (!m_tbHelper.Initialize(this, GetParent(), &mgrShortcuts))
	{
		ASSERT(0);
		return FALSE;
	}
	
	for (int nTip = 0; nTip < aButtons.GetSize(); nTip++)
	{
		const TOOLBARBUTTON& tb = aButtons[nTip];
		
		// Only handle static non-separator items
		if (tb.nMenuID && !mainMenu.IsDynamicItem(tb.nMenuID))
		{
			HMENU hItemMenu = NULL;
			int nPos = CEnMenu::FindMenuItem(mainMenu, tb.nMenuID, hItemMenu);
			ASSERT((nPos != -1) && (hItemMenu != NULL));

			CString sTooltip = CEnMenu::GetMenuString(hItemMenu, nPos, MF_BYPOSITION);
			m_tbHelper.SetTooltip(tb.nMenuID, sTooltip);
		}
	}

	RefreshDisabledImageList();

	return TRUE;
}

void CTDLCustomToolbar::OnDestroy() 
{
	m_tbHelper.Release();

	CEnToolBar::OnDestroy();
}
