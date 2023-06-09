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

CTDLCustomToolbar::CTDLCustomToolbar() : m_nInitBtnCount(0)
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

BOOL CTDLCustomToolbar::InitialiseButtons(const CToolbarButtonArray& aButtons,
										  const CTDCMainMenu& mainMenu,
										  const CShortcutManager& mgrShortcuts)
{
	// Must be created BUT NOT initialised
	if (!GetSafeHwnd() || 
		GetButtonCount() || 
		m_tbHelper.IsInitialized() ||
		m_ilNormal.GetSafeHandle())
	{
		ASSERT(0);
		return FALSE;
	}

	m_nInitBtnCount = 0;

	CTDCImageList ilButtons(m_crFrom);

	if (!ilButtons.LoadDefaultImages(TRUE))
	{
		ASSERT(0);
		return FALSE;
	}
	
	SetImageSize(16, 16);

	for (int nBtn = 0; nBtn < aButtons.GetSize(); nBtn++)
	{
		const TOOLBARBUTTON& tb = aButtons[nBtn];
		TBBUTTON tbb = { -1, (int)tb.nMenuID, 0, TBSTYLE_SEP, 0, 0, (UINT)-1 };
		
		if (tb.nMenuID > 0)
		{
			tbb.iBitmap = ilButtons.GetImageIndex(tb.sImageID);
			tbb.fsStyle = TBSTYLE_BUTTON;
		}

		if (!GetToolBarCtrl().InsertButton(nBtn, &tbb))
			return FALSE;
	}
	m_nInitBtnCount = aButtons.GetSize();

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
		CString sTooltip;
				
		if (GetItemTooltip(tb.nMenuID, mainMenu, sTooltip))
			m_tbHelper.SetTooltip(tb.nMenuID, sTooltip);
	}

	RefreshDisabledImageList();

	return TRUE;
}

BOOL CTDLCustomToolbar::GetItemTooltip(UINT nMenuID, const CTDCMainMenu& mainMenu, CString& sTooltip)
{
	// Only handle static non-separator items
	if (nMenuID && !mainMenu.IsDynamicItem(nMenuID))
	{
		HMENU hItemMenu = NULL;
		int nPos = CEnMenu::FindMenuItem(mainMenu, nMenuID, hItemMenu);

		ASSERT((nPos != -1) && (hItemMenu != NULL));

		sTooltip = CEnMenu::GetMenuString(hItemMenu, nPos, MF_BYPOSITION);
	}
	else
	{
		sTooltip.Empty();
	}

	return !sTooltip.IsEmpty();
}

void CTDLCustomToolbar::OnDestroy() 
{
	m_tbHelper.Release();

	CEnToolBar::OnDestroy();
}

BOOL CTDLCustomToolbar::ModifyButtonAttributes(const CToolbarButtonArray& aButtons,
												const CTDCMainMenu& mainMenu)
{
	// Must be created AND initialised
	if (!GetSafeHwnd() ||
		!m_nInitBtnCount ||
		(aButtons.GetSize() != m_nInitBtnCount) ||
		!m_tbHelper.IsInitialized() ||
		!m_ilNormal.GetSafeHandle())
	{
		ASSERT(0);
		return FALSE;
	}

	// Cache the old tooltips and remove them because
	// we will be updating in-situ
	int nBtn = m_nInitBtnCount;

	CStringArray aOldTips;
	aOldTips.SetSize(m_nInitBtnCount); // ignore trailing tools

	while (nBtn--)
	{
		UINT nOldCmdID = GetItemID(nBtn);

		if (nOldCmdID != ID_SEPARATOR)
		{
			aOldTips[nBtn] = m_tbHelper.GetTooltip(nOldCmdID);
			m_tbHelper.ClearTooltip(nOldCmdID);
		}
	}

	CTDCImageList ilButtons;
	VERIFY(ilButtons.LoadDefaultImages(TRUE));

	BOOL bRemapped = FALSE;

	for (nBtn = 0; nBtn < m_nInitBtnCount; nBtn++)
	{
		// Skip over existing separators
		// but allow buttons to be given a menu ID of 0
		if (IsItemSeparator(nBtn))
			continue;

		const TOOLBARBUTTON& tbNew = aButtons[nBtn];

		// Update commandl ID
		UINT nOldCmdID = GetItemID(nBtn);
		UINT nNewCmdID = tbNew.nMenuID;

		if (nNewCmdID != nOldCmdID)
		{
			GetToolBarCtrl().SetCmdID(nBtn, nNewCmdID);
			bRemapped = TRUE;
		}

		// Update Image
		TBBUTTONINFO tbiOld = { sizeof(TBBUTTONINFO), TBIF_IMAGE, 0 };
		VERIFY(GetToolBarCtrl().GetButtonInfo(nBtn, &tbiOld));

		TBBUTTONINFO tbiNew = tbiOld;
		tbiNew.iImage = ilButtons.GetImageIndex(tbNew.sImageID);

		if (tbiNew.iImage != tbiOld.iImage)
		{
			VERIFY(GetToolBarCtrl().SetButtonInfo(nNewCmdID, &tbiNew));
			bRemapped = TRUE;
		}

		// Update tooltip
		CString sOldTip = aOldTips[nBtn], sNewTip;
		GetItemTooltip(nNewCmdID, mainMenu, sNewTip);

		// Always set the tip because we cleared them at the start
		m_tbHelper.SetTooltip(nNewCmdID, sNewTip);

		if (sNewTip != sOldTip)
			bRemapped = TRUE;
	}

	return bRemapped;
}

BOOL CTDLCustomToolbar::RemapMenuItemIDs(const CMap<UINT, UINT, UINT, UINT&>& mapCmdIDs,
										 CToolbarButtonArray& aButtons)
{
	// Work through the buttons updating as we go
	BOOL bRemapped = FALSE;
	int nBtn = aButtons.GetSize();

	while (nBtn--)
	{
		TOOLBARBUTTON& tb = aButtons[nBtn];

		// Ignore separators
		if (tb.nMenuID == 0)
			continue;

		UINT nNewMenuID;

		if (!mapCmdIDs.Lookup(tb.nMenuID, nNewMenuID))
			continue;

		if (nNewMenuID != tb.nMenuID)
		{
			tb.nMenuID = nNewMenuID;
			bRemapped = TRUE;
		}
	}

	return bRemapped;
}

BOOL CTDLCustomToolbar::AppendTools(const CUserToolArray& aTools, BOOL bGrouped)
{
	if (m_nInitBtnCount == 0)
	{
		ASSERT(0);
		return FALSE;
	}

	RemoveTools();

	UINT nLastID = GetItemID(m_nInitBtnCount - 1);
	CTDCToolsHelper(FALSE).AddToolsToToolbar(aTools, *this, nLastID, bGrouped);

	return TRUE;
}

int CTDLCustomToolbar::RemoveTools()
{
	// Remove all buttons greater than the buttons we were initialised with
	int nBtn = GetButtonCount(), nNumRemoved = 0;

	while (nBtn > m_nInitBtnCount)
	{
		nBtn--;
		nNumRemoved++;

		GetToolBarCtrl().DeleteButton(nBtn);
	}

	return nNumRemoved;
}
