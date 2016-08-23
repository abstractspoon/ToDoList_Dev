// PreferencesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PreferencesDlg.h"
#include "todoitem.h"

#include "..\shared\winclasses.h"
#include "..\shared\wclassdefines.h"
#include "..\shared\enstring.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\deferwndmove.h"
#include "..\shared\localizer.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\themed.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreferencesDlg dialog

// default priority colors
const COLORREF PRIORITYLOWCOLOR = RGB(30, 225, 0);
const COLORREF PRIORITYHIGHCOLOR = RGB(255, 0, 0);
const int MIN_WIDTH = 827;
const int MIN_HEIGHT = 510;
const UINT IDC_TOPOFPAGE = (UINT)-1; // pseudo control ID

const TCHAR PATHDELIM = '>';

CPreferencesDlg::CPreferencesDlg(CShortcutManager* pShortcutMgr, 
								 const CContentMgr* pContentMgr, 
								 const CImportExportMgr* pExportMgr,
								 const CUIExtensionMgr* pMgrUIExt,
								 CWnd* pParent /*=NULL*/)
	: 
	CPreferencesDlgBase(IDD_PREFERENCES, pParent), 
	m_pageShortcuts(pShortcutMgr), 
	m_pageUI(pContentMgr, pMgrUIExt), 
	m_pageFile2(pExportMgr),
	m_pageUITasklistColors(m_defaultListData),
	m_sizeCurrent(-1, -1),
	m_btnHelp(IDD_PREFERENCES, FALSE)
{
	CPreferencesDlgBase::AddPage(&m_pageGen);
	CPreferencesDlgBase::AddPage(&m_pageMultiUser);
	CPreferencesDlgBase::AddPage(&m_pageFile);
	CPreferencesDlgBase::AddPage(&m_pageFile2);
	CPreferencesDlgBase::AddPage(&m_pageUI);
	CPreferencesDlgBase::AddPage(&m_pageUIVisibility);
	CPreferencesDlgBase::AddPage(&m_pageUITasklist);
	CPreferencesDlgBase::AddPage(&m_pageUITasklistColors);
	CPreferencesDlgBase::AddPage(&m_pageTask);
	CPreferencesDlgBase::AddPage(&m_pageTaskCalc);
	CPreferencesDlgBase::AddPage(&m_pageTaskDef);
	CPreferencesDlgBase::AddPage(&m_pageExport);
	CPreferencesDlgBase::AddPage(&m_pageTool);
	CPreferencesDlgBase::AddPage(&m_pageShortcuts);

	ForwardMessage(WM_PTP_TESTTOOL);
	ForwardMessage(WM_PGP_CLEARMRU);
	ForwardMessage(WM_PGP_CLEANUPDICTIONARY);
	ForwardMessage(WM_PPB_CTRLCHANGE);
	
	LoadPreferences();
}

CPreferencesDlg::~CPreferencesDlg()
{
}

void CPreferencesDlg::DoDataExchange(CDataExchange* pDX)
{
	CPreferencesDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferencesDlg)
	DDX_Control(pDX, IDC_PAGES, m_tcPages);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_PAGE_TITLE, m_sPageTitle);
	DDX_Control(pDX, IDC_PAGE_TITLE, m_stPageTitle);
	DDX_Control(pDX, IDC_CATEGORY_TITLE, m_stCategoryTitle);
}

BEGIN_MESSAGE_MAP(CPreferencesDlg, CPreferencesDlgBase)
	//{{AFX_MSG_MAP(CPreferencesDlg)
	ON_BN_CLICKED(IDC_HELPBUTTON, OnHelp)
	ON_NOTIFY(TVN_SELCHANGED, IDC_PAGES, OnSelchangedPages)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
	ON_WM_HELPINFO()
	ON_WM_ERASEBKGND()
	ON_REGISTERED_MESSAGE(WM_PTP_TESTTOOL, OnToolPageTestTool)
	ON_REGISTERED_MESSAGE(WM_PGP_CLEARMRU, OnGenPageClearMRU)
	ON_REGISTERED_MESSAGE(WM_PGP_CLEANUPDICTIONARY, OnGenPageCleanupDictionary)
	ON_REGISTERED_MESSAGE(WM_PPB_CTRLCHANGE, OnControlChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencesDlg message handlers

BOOL CPreferencesDlg::OnInitDialog() 
{
	CRect rClient;
	GetClientRect(rClient);
	m_sizeCurrent = rClient.Size();
	
	CRect rHost;
	GetDlgItem(IDC_HOSTFRAME)->GetWindowRect(rHost);
	ScreenToClient(rHost);
	rHost.DeflateRect(1, 1);
	
	VERIFY(CreatePPHost(rHost));

	CPreferencesDlgBase::OnInitDialog();
	
	// disable translation of category title because
	// categories will already been translated
	CLocalizer::EnableTranslation(::GetDlgItem(*this, IDC_PAGE_TITLE), FALSE);

	m_tcPages.SetIndent(0);
	CThemed::SetWindowTheme(&m_tcPages, _T("Explorer"));

	// add pages to tree control
	AddPage(&m_pageGen,					IDS_PREF_GEN);
	AddPage(&m_pageMultiUser,			IDS_PREF_MULTIUSER);
	AddPage(&m_pageMultiUser,			IDS_PREF_MULTIUSERFILE,		IDC_TOPOFPAGE);
	AddPage(&m_pageMultiUser,			IDS_PREF_MULTIUSERSS,		IDC_SSCGROUP);
	AddPage(&m_pageFile,				IDS_PREF_FILE,				IDC_TOPOFPAGE);
	AddPage(&m_pageFile,				IDS_PREF_FILELOAD,			IDC_LOADGROUP);
	AddPage(&m_pageFile,				IDS_PREF_FILEARCHIVE,		IDC_ARCHIVEGROUP);
	AddPage(&m_pageFile,				IDS_PREF_FILESWITCH,		IDC_SWITCHGROUP);
	AddPage(&m_pageFile,				IDS_PREF_FILENOTIFY,		IDC_DUEGROUP);
	AddPage(&m_pageFile2,				IDS_PREF_FILEMORE,			IDC_TOPOFPAGE);
	AddPage(&m_pageFile2,				IDS_PREF_FILEBACKUP,		IDC_BACKUPGROUP);
	AddPage(&m_pageFile2,				IDS_PREF_FILESAVE,			IDC_SAVEGROUP);
	AddPage(&m_pageUI,					IDS_PREF_UI,				IDC_TOPOFPAGE);
	AddPage(&m_pageUI,					IDS_PREF_UIFILTERING,		IDC_FILTERGROUP);
	AddPage(&m_pageUI,					IDS_PREF_UISORTING,			IDC_SORTGROUP);
	AddPage(&m_pageUI,					IDS_PREF_UITOOLBAR,			IDC_TOOLBARGROUP);
	AddPage(&m_pageUI,					IDS_PREF_UICOMMENTS,		IDC_COMMENTSGROUP);
	AddPage(&m_pageUI,					IDS_PREF_UITABBAR,			IDC_TABBARGROUP);
	AddPage(&m_pageUIVisibility,		IDS_PREF_UIVISIBILITY);
	AddPage(&m_pageUITasklist,			IDS_PREF_UITASK);
	AddPage(&m_pageUITasklistColors,	IDS_PREF_UITASKCOLOR);
	AddPage(&m_pageTask,				IDS_PREF_TASK,				IDC_TOPOFPAGE);
	AddPage(&m_pageTask,				IDS_PREF_TIMETRACK,			IDC_TRACKGROUP);
	AddPage(&m_pageTask,				IDS_PREF_TASKTIME,			IDC_TIMEGROUP);
	AddPage(&m_pageTaskCalc,			IDS_PREF_TASKCALCS);
	AddPage(&m_pageTaskDef,				IDS_PREF_TASKDEF,			IDC_TOPOFPAGE);
	AddPage(&m_pageTaskDef,				IDS_PREF_TASKDEFATTRIB,		IDC_DEFGROUP);
	AddPage(&m_pageTaskDef,				IDS_PREF_TASKDEFINHERIT,	IDC_INHERITGROUP);
	AddPage(&m_pageTaskDef,				IDS_PREF_TASKDEFLISTS,		IDC_DROPLISTGROUP);
	AddPage(&m_pageExport,				IDS_PREF_EXPORT);
	AddPage(&m_pageTool,				IDS_PREF_TOOLS);
	AddPage(&m_pageShortcuts,			IDS_PREF_SHORTCUT); 
	
	SynchronizeTree();

	GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);

	// fixup icon
	HICON hIcon = GraphicsMisc::LoadIcon(IDI_PREFERENCES_DIALOG_STD);
	SetIcon(hIcon, FALSE);

	VERIFY(m_btnHelp.Create(IDC_HELPBUTTON, this));

	m_tcPages.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
}

BOOL CPreferencesDlg::OnHelpInfo(HELPINFO* /*pHelpInfo*/)
{
	OnHelp();
	return FALSE;
}

void CPreferencesDlg::SynchronizeTree()
{
	HTREEITEM htiMap = NULL;
	
	if (m_mapPP2HTI.Lookup(GetActivePage(), htiMap))
	{
		m_tcPages.SelectItem(htiMap);
		m_tcPages.EnsureVisible(htiMap);
	}
}

BOOL CPreferencesDlg::PreTranslateMessage(MSG* pMsg) 
{
	// special handling for hotkeys
	if (CWinClasses::IsClass(pMsg->hwnd, WC_HOTKEY))
		return FALSE;
	
	return CPreferencesDlgBase::PreTranslateMessage(pMsg);
}

void CPreferencesDlg::OnHelp() 
{
	const CPreferencesPageBase* pPage = GetActivePage();
	ASSERT(pPage);

	if (pPage)
		AfxGetApp()->WinHelp(pPage->GetHelpID());
}

void CPreferencesDlg::AddPage(CPreferencesPageBase* pPage, UINT nIDPath, UINT nIDSection)
{
	CEnString sPath(nIDPath);
	
	if (FindPage(pPage) != -1) 
	{
		HTREEITEM htiParent = TVI_ROOT; // default
		CString sParent(sPath);
		
		while (Misc::Split(sParent, sPath, PATHDELIM))
		{
			// see if parent already exists
			HTREEITEM htiParentParent = htiParent;
			htiParent = m_tcPages.GetChildItem(htiParentParent);
			
			while (htiParent)
			{
				if (sParent.CompareNoCase(m_tcPages.GetItemText(htiParent)) == 0)
					break;
				
				htiParent = m_tcPages.GetNextItem(htiParent, TVGN_NEXT);
			}
			
			if (!htiParent)
			{
				htiParent = m_tcPages.InsertItem(sParent, htiParentParent);
				
				// embolden root items
				if (htiParentParent == TVI_ROOT)
					m_tcPages.SetItemState(htiParent, TVIS_BOLD, TVIS_BOLD);
			}

			// next
			sParent = sPath;
		}
		
		HTREEITEM hti = m_tcPages.InsertItem(sPath, htiParent); // whatever's left
		m_tcPages.EnsureVisible(hti);
		
		// embolden root items
		if (htiParent == TVI_ROOT)
			m_tcPages.SetItemState(hti, TVIS_BOLD, TVIS_BOLD);
		
		// map both ways
		m_tcPages.SetItemData(hti, (DWORD)pPage);

		// don't remap the page if already done
		HTREEITEM htiMap = NULL;
		
		if (!m_mapPP2HTI.Lookup(pPage, htiMap))
			m_mapPP2HTI[pPage] = hti;

		// map the treeitem to its section if it has one
		if (nIDSection)
			m_mapHTIToSection[hti] = nIDSection;

		// set page background to window back
		pPage->SetBackgroundColor(GetSysColor(COLOR_WINDOW));
	}
}

void CPreferencesDlg::OnSelchangedPages(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	HTREEITEM htiSel = m_tcPages.GetSelectedItem();
	
	// Get the section of the item FIRST
	UINT nIDSection = 0;
	m_mapHTIToSection.Lookup(htiSel, nIDSection);

	// if we got a parent item, move to the first child
	while (m_tcPages.ItemHasChildren(htiSel))
		htiSel = m_tcPages.GetChildItem(htiSel);
	
	CPreferencesPageBase* pPage = (CPreferencesPageBase*)m_tcPages.GetItemData(htiSel);
	ASSERT (pPage);
	
	if (pPage && CPreferencesDlgBase::SetActivePage(pPage))
	{
		// move to the section
		if (nIDSection == IDC_TOPOFPAGE) // pseudo control ID
		{
			m_pphost.ScrollToTop();
		}
		else if (nIDSection)
		{
			m_pphost.ScrollTo(pPage->GetDlgItem(nIDSection));
		}

		// special page handling
		if (pPage == &m_pageTaskDef)
		{
			// defaults page then update the priority colors
			CDWordArray aColors;

			m_pageUITasklistColors.GetPriorityColors(aColors);
			m_pageTaskDef.SetPriorityColors(aColors);
		}
		else if (pPage == &m_pageUITasklistColors)
		{
			// colours page then update 'colour by attribute'
// 			CStringArray aAttrib;
// 			TDC_ATTRIBUTE nAttrib = m_pageUITasklistColors.GetColorByAttribute();
// 			
// 			m_pageTaskDef.GetListItems(nAttrib, aAttrib);
// 			m_pageUITasklistColors.AddAttributes(nAttrib, aAttrib);

			GetDefaultListItems(m_defaultListData);
		}
		
		// update caption
		m_sPageTitle = GetItemPath(htiSel);
		UpdateData(FALSE);
	}
	
	m_tcPages.SetFocus();
	
	*pResult = 0;
}

BOOL CPreferencesDlg::SetActivePage(int nPage)
{
	if (CPreferencesDlgBase::SetActivePage(nPage))
	{
		SynchronizeTree();
		
		return TRUE;
	}
	
	return FALSE;
}

CString CPreferencesDlg::GetItemPath(HTREEITEM hti) const
{
	CString sPath = m_tcPages.GetItemText(hti);
	hti = m_tcPages.GetParentItem(hti);
	
	while (hti)
	{
		sPath = m_tcPages.GetItemText(hti) + " > " + sPath;
		hti = m_tcPages.GetParentItem(hti);
	}
	
	return sPath;
}

int CPreferencesDlg::GetDefaultListItems(TDCAUTOLISTDATA& tld) const 
{ 
	m_pageTaskDef.GetListItems(TDCA_CATEGORY, tld.aCategory);
	m_pageTaskDef.GetListItems(TDCA_ALLOCTO, tld.aAllocTo);
	m_pageTaskDef.GetListItems(TDCA_STATUS, tld.aStatus);
	m_pageTaskDef.GetListItems(TDCA_ALLOCBY, tld.aAllocBy);
	m_pageTaskDef.GetListItems(TDCA_VERSION, tld.aVersion);
	m_pageTaskDef.GetListItems(TDCA_TAGS, tld.aTags);

	// add in any 'unknown' default items assigned directly to task
	TODOITEM tdiDef;
	GetDefaultTaskAttributes(tdiDef);

	Misc::AddUniqueItems(tdiDef.aCategories, tld.aCategory);
	Misc::AddUniqueItems(tdiDef.aAllocTo, tld.aAllocTo);
	Misc::AddUniqueItems(tdiDef.aTags, tld.aTags);

	Misc::AddUniqueItem(tdiDef.sStatus, tld.aStatus);
	Misc::AddUniqueItem(tdiDef.sAllocBy, tld.aAllocBy);
	Misc::AddUniqueItem(tdiDef.sVersion, tld.aVersion);

	return tld.GetSize();
}

LRESULT CPreferencesDlg::OnToolPageTestTool(WPARAM wp, LPARAM lp)
{
	// forward on to main app
	return AfxGetMainWnd()->SendMessage(WM_PTP_TESTTOOL, wp, lp);
}

LRESULT CPreferencesDlg::OnGenPageClearMRU(WPARAM wp, LPARAM lp)
{
	// forward on to main app
	return AfxGetMainWnd()->SendMessage(WM_PGP_CLEARMRU, wp, lp);
}

LRESULT CPreferencesDlg::OnGenPageCleanupDictionary(WPARAM wp, LPARAM lp) 
{
	// forward on to main app
	return AfxGetMainWnd()->SendMessage(WM_PGP_CLEANUPDICTIONARY, wp, lp);
}

void CPreferencesDlg::OnApply() 
{
	CPreferencesDlgBase::OnApply();	

	GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);
}

LRESULT CPreferencesDlg::OnControlChange(WPARAM /*wp*/, LPARAM /*lp*/)
{
	GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);

	return 0L;
}

void CPreferencesDlg::OnSize(UINT nType, int cx, int cy) 
{
	CPreferencesDlgBase::OnSize(nType, cx, cy);

	if (!m_pphost.GetSafeHwnd())
		return;
	
	Resize(cx, cy);

	m_btnHelp.UpdatePosition();
}

void CPreferencesDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	CPreferencesDlgBase::OnGetMinMaxInfo(lpMMI);

	lpMMI->ptMinTrackSize.x = MIN_WIDTH;
	lpMMI->ptMinTrackSize.y = MIN_HEIGHT;
}

void CPreferencesDlg::Resize(int cx, int cy)
{
	if (cx == 0 || cy == 0)
	{
		CRect rClient;
		GetClientRect(rClient);

		cx = rClient.Width();
		cy = rClient.Height();
	}

	// calculate deltas
	int nDX = cx - m_sizeCurrent.cx;
	int nDY = cy - m_sizeCurrent.cy;

	if (nDX == 0 && nDY == 0)
		return;

	m_sizeCurrent.cx = cx;
	m_sizeCurrent.cy = cy;
	
	CDeferWndMove dwm(10);
	
	// offset buttons
	dwm.OffsetCtrl(this, IDC_HELP2, 0, nDY);
	dwm.OffsetCtrl(this, IDOK, nDX, nDY);
	dwm.OffsetCtrl(this, IDCANCEL, nDX, nDY);
	dwm.OffsetCtrl(this, IDC_APPLY, nDX, nDY);

	// resize tree
	dwm.ResizeCtrl(this, IDC_PAGES, 0, nDY);
	
	// PPHost and label and border
	dwm.ResizeCtrl(this, m_pphost.GetDlgCtrlID(), nDX, nDY);
	dwm.ResizeCtrl(this, IDC_PAGE_TITLE, nDX, 0);
	dwm.ResizeCtrl(this, IDC_HOSTFRAME, nDX, nDY);

	GetDlgItem(IDC_HOSTFRAME)->Invalidate(TRUE);
}

void CPreferencesDlg::SetUITheme(const CUIThemeFile& theme)
{
	m_theme = theme;

	SetTitleThemeColors(m_stPageTitle, theme);
	SetTitleThemeColors(m_stCategoryTitle, theme);
	
	if (GetSafeHwnd())
		Invalidate(TRUE);
}

void CPreferencesDlg::SetTitleThemeColors(CEnStatic& stTitle, const CUIThemeFile& theme)
{
	stTitle.SetColors(theme.crStatusBarText, theme.crStatusBarLight, theme.crStatusBarDark, 
						theme.HasGlass(), theme.HasGradient(), FALSE);
}

BOOL CPreferencesDlg::OnEraseBkgnd(CDC* pDC)
{
	if (m_theme.IsSet())
	{
		CRect rClient;
		GetClientRect(rClient);

		pDC->FillSolidRect(rClient, m_theme.crAppBackLight);
		return TRUE;
	}

	return CPreferencesDlgBase::OnEraseBkgnd(pDC);
}

void CPreferencesDlg::GetDefaultTaskAttributes(TODOITEM& tdiDefault) const
{
	m_pageTaskDef.GetTaskAttributes(tdiDefault);

	// extra
	tdiDefault.sCommentsTypeID = m_pageUI.GetDefaultCommentsFormat();
}
