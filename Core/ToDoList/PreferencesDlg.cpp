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
#include "..\shared\autoflag.h"
#include "..\shared\HookMgr.h"
#include "..\shared\holdredraw.h"
#include "..\shared\CtrlTextHighlighter.h"
#include "..\shared\FileIcons.h"
#include "..\shared\OSVersion.h"

#include "..\Interfaces\TasklistStorageMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const COLORREF HILITE_BACKCOLOUR = RGB(255, 255, 64); // yellow
const COLORREF HILITE_TEXTCOLOUR = GraphicsMisc::GetBestTextColor(HILITE_BACKCOLOUR);

/////////////////////////////////////////////////////////////////////////////
// Private class for tracking mouse middle-button clicking
// so we can copy the clicked items text to the clipboard

class CPreferencesDlgCopyHookMgr : public CHookMgr<CPreferencesDlgCopyHookMgr>  
{
	friend class CHookMgr<CPreferencesDlgCopyHookMgr>;
	
public:
	virtual ~CPreferencesDlgCopyHookMgr()
	{
	}
	
	static BOOL Initialize(HWND hwndNotify)
	{
		if (GetInstance().InitHooks(HM_MOUSE))
		{
			GetInstance().m_hwndNotify = hwndNotify;
			return TRUE;
		}
		
		// else
		return FALSE;
	}
	
	static void Release()
	{
		GetInstance().ReleaseHooks();
	}
	
protected:
	CPreferencesDlgCopyHookMgr()  : m_hwndNotify(NULL)
	{
	}
	
	static CPreferencesDlgCopyHookMgr& Instance() 
	{ 
		return CHookMgr<CPreferencesDlgCopyHookMgr>::GetInstance(); 
	}
	
protected:
	HWND m_hwndNotify;
	
protected:
	virtual BOOL OnMouseEx(UINT uMouseMsg, const MOUSEHOOKSTRUCTEX& info)
	{
		switch (uMouseMsg)
		{
		case WM_MBUTTONDOWN:
			{
				HWND hwndHit = CDialogHelper::GetWindowFromPoint(::GetParent(info.hwnd), info.pt);
#ifdef _DEBUG
				CString sMouseClass = CWinClasses::GetClass(info.hwnd);
				CString sHitClass = CWinClasses::GetClass(hwndHit);
#endif
				if (CDialogHelper::IsChildOrSame(m_hwndNotify, hwndHit))
				{
					::SendMessage(m_hwndNotify, WM_COPY, ::GetDlgCtrlID(hwndHit), (LPARAM)hwndHit);
					return TRUE;
				}
			}
			break;
		}
		
		return FALSE;
	}
};

/////////////////////////////////////////////////////////////////////////////

// default priority colors
const COLORREF PRIORITYLOWCOLOR = RGB(30, 225, 0);
const COLORREF PRIORITYHIGHCOLOR = RGB(255, 0, 0);

const UINT IDC_TOPOFPAGE = (UINT)-1; // pseudo control ID

const TCHAR PATHDELIM = '>';
const LPCTSTR PREFSKEY = _T("Preferences");

enum SEARCH_BTNS { BTN_UPDATE = 1, BTN_CLEAR };

/////////////////////////////////////////////////////////////////////////////
// CPreferencesDlg dialog

CPreferencesDlg::CPreferencesDlg(CShortcutManager* pShortcutMgr, 
								 const CTDCContentMgr* pContentMgr, 
								 const CTDCImportExportMgr* pExportMgr,
								 const CUIExtensionMgr* pMgrUIExt,
								 const CTasklistStorageMgr* pMgrStorage,
								 CWnd* pParent /*=NULL*/)
	: 
	CPreferencesDlgBase(IDD_PREFERENCES, IDC_HOSTFRAME, IDI_PREFERENCES_DIALOG_STD, 0, pParent), 
	m_pageShortcuts(m_mgrMenuIcons, pShortcutMgr), 
	m_pageUI(pMgrUIExt), 
	m_pageTaskDef(pContentMgr), 
	m_pageFile2(pExportMgr),
	m_pageUICustomToolbar(m_ilIcons),
	m_iconSearch(IDI_SEARCH_PREFS, 16, FALSE),
	m_iconReset(IDI_RESET, 16, FALSE),
	m_bInitialisingDialog(FALSE),
	m_bBuildingTree(FALSE),
	m_pMgrStorage(pMgrStorage)
{
	m_eSearchText.AddButton(BTN_UPDATE, m_iconSearch, CEnString(IDS_SEARCHPREFS_PROMPT));
	m_eSearchText.AddButton(BTN_CLEAR, m_iconReset, CEnString(IDS_CLEARSEARCH_TIP));
	m_eSearchText.SetDefaultButton(BTN_UPDATE);

	CPreferencesDlgBase::AddPage(&m_pageGen);
	CPreferencesDlgBase::AddPage(&m_pageMultiUser);
	CPreferencesDlgBase::AddPage(&m_pageFile);
	CPreferencesDlgBase::AddPage(&m_pageFile2);
	CPreferencesDlgBase::AddPage(&m_pageUI);
	CPreferencesDlgBase::AddPage(&m_pageUIVisibility);
	CPreferencesDlgBase::AddPage(&m_pageUITasklist);
	CPreferencesDlgBase::AddPage(&m_pageUITasklistColors);
	CPreferencesDlgBase::AddPage(&m_pageUICustomToolbar);
	CPreferencesDlgBase::AddPage(&m_pageTask);
	CPreferencesDlgBase::AddPage(&m_pageTaskCalc);
	CPreferencesDlgBase::AddPage(&m_pageTaskDef);
	CPreferencesDlgBase::AddPage(&m_pageTaskDef2);
	CPreferencesDlgBase::AddPage(&m_pageExport);
	CPreferencesDlgBase::AddPage(&m_pageTools);
	CPreferencesDlgBase::AddPage(&m_pageShortcuts);

	m_ppHost.ForwardMessage(WM_PTP_TESTTOOL);
	m_ppHost.ForwardMessage(WM_PGP_CLEARMRU);
	m_ppHost.ForwardMessage(WM_PGP_EDITLANGFILE);
	m_ppHost.ForwardMessage(WM_PPB_CTRLCHANGE);
	m_ppHost.ForwardMessage(WM_PUITCP_TEXTCOLOROPTION);
	m_ppHost.ForwardMessage(WM_PUITCP_NUMPRORITYRISKLEVELS);
	
	LoadPreferences(m_prefs, PREFSKEY);
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
	DDX_Control(pDX, IDC_SEARCH, m_eSearchText);
}

BEGIN_MESSAGE_MAP(CPreferencesDlg, CPreferencesDlgBase)
	//{{AFX_MSG_MAP(CPreferencesDlg)
	ON_NOTIFY(TVN_SELCHANGED, IDC_PAGES, OnTreeSelChanged)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PAGES, OnTreeCustomDraw)
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_REGISTERED_MESSAGE(WM_EE_BTNCLICK, OnUpdateSearch)
	ON_REGISTERED_MESSAGE(WM_PTP_TESTTOOL, OnToolPageTestTool)
	ON_REGISTERED_MESSAGE(WM_PGP_CLEARMRU, OnGenPageClearMRU)
	ON_REGISTERED_MESSAGE(WM_PGP_EDITLANGFILE, OnGenPageEditLangFile)
	ON_REGISTERED_MESSAGE(WM_PPB_CTRLCHANGE, OnControlChange)
	ON_REGISTERED_MESSAGE(WM_PUITCP_TEXTCOLOROPTION, OnColorPageTextOption)
	ON_REGISTERED_MESSAGE(WM_PUITCP_NUMPRORITYRISKLEVELS, OnNumPriorityRiskLevels)
	ON_MESSAGE(WM_COPY, OnCopy)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencesDlg message handlers

int CPreferencesDlg::DoModal(int nInitPage, UINT nInitCtrlID) 
{ 
	return CPreferencesDlgBase::DoModal(m_prefs, PREFSKEY, nInitPage, nInitCtrlID); 
}

void CPreferencesDlg::LoadPreferences(const IPreferences* prefs, LPCTSTR szKey)
{
	// 'Temporary' hack to prevent prefs being reloaded 
	// by base class in OnInitDialog
	if (!m_bInitialisingDialog)
	{
		CPreferencesDlgBase::LoadPreferences(prefs, szKey);

		OnColorPageTextOption(0, 0);
		OnNumPriorityRiskLevels(0, 0);
	}
}

BOOL CPreferencesDlg::OnInitDialog() 
{
	CAutoFlag af(m_bInitialisingDialog, TRUE);

	CPreferencesDlgBase::OnInitDialog();
	CPreferencesDlgCopyHookMgr::Initialize(*this);

	m_mgrPrompts.SetEditPrompt(m_eSearchText, CEnString(IDS_SEARCHPREFS_PROMPT));
	
	// disable translation of category title because
	// categories will already been translated
	CLocalizer::EnableTranslation(::GetDlgItem(*this, IDC_PAGE_TITLE), FALSE);

	m_tcPages.SetIndent(0);
	CThemed::SetWindowTheme(&m_tcPages, _T("Explorer"));

	GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);
	AddPagesToTree(FALSE); // all pages

	return FALSE;  // return TRUE unless you set the focus to a control
}

void CPreferencesDlg::OnDestroy()
{
	CPreferencesDlgCopyHookMgr::Release();

	CDialog::OnDestroy();
}

LRESULT CPreferencesDlg::OnCopy(WPARAM wp, LPARAM lp)
{
	if (lp)
	{
		LPCTSTR szMenu = _T("Tools > Preferences");

		CString sTreePath = GetItemPath(m_tcPages.GetSelectedItem()), sItemText, sCopyText;
		CWnd::FromHandle((HWND)lp)->GetWindowText(sItemText);

		switch (wp)
		{
		case IDC_PAGES:
			sCopyText.Format(_T("%s > %s"), szMenu, sTreePath);
			break;

		case IDC_PAGE_TITLE:
		case IDC_CATEGORY_TITLE:
		case IDOK:
		case IDCANCEL:
			sCopyText.Format(_T("%s > %s"), szMenu, sItemText);
			break;

		default:
			if (!sItemText.IsEmpty())
				sCopyText.Format(_T("%s > %s > %s"), szMenu, sTreePath, sItemText);
			break;
		}

		if (!sCopyText.IsEmpty())
			Misc::CopyTexttoClipboard(sCopyText, *this);
	}

	return 0L;
}

void CPreferencesDlg::SynchronizeTree()
{
	HTREEITEM htiMap = NULL;
	
	if (!m_mapPP2HTI.Lookup(GetActivePage(), htiMap))
		htiMap = m_tcPages.GetChildItem(NULL);

	m_tcPages.SelectItem(htiMap);
	m_tcPages.EnsureVisible(htiMap);
	m_tcPages.SetFocus();
}

BOOL CPreferencesDlg::PreTranslateMessage(MSG* pMsg) 
{
	// special handling for hotkeys
	if (CWinClasses::IsClass(pMsg->hwnd, WC_HOTKEY))
		return FALSE;

	// F1 handling
	if (((pMsg->message == WM_KEYDOWN) || (pMsg->message == WM_SYSKEYDOWN)) && 
		CDialogHelper::IsChildOrSame(*this, pMsg->hwnd))
	{
		switch (pMsg->wParam)
		{
			case VK_F1:
				OnHelp();
				break;

			case 'F':
			case 'f':
				if (Misc::ModKeysArePressed(MKS_CTRL))
					m_eSearchText.SetFocus();
				break;
		}
	}
	
	return CPreferencesDlgBase::PreTranslateMessage(pMsg);
}

void CPreferencesDlg::AddPagesToTree(BOOL bDoSearch)
{
	// scope to allow SynchroniseTree to work
	{
		CAutoFlag af(m_bBuildingTree, TRUE);

		m_tcPages.DeleteAllItems();
		m_mapHTIToSection.RemoveAll();
		m_mapPP2HTI.RemoveAll();

		AddPageToTree(&m_pageGen, IDS_PREF_GEN, IDC_TOPOFPAGE, bDoSearch);

		if (AddPageToTree(&m_pageMultiUser, IDS_PREF_MULTIUSER, IDC_TOPOFPAGE, bDoSearch))
		{
			AddPageToTree(&m_pageMultiUser, IDS_PREF_MULTIUSERFILE, IDC_TOPOFPAGE, FALSE);
			AddPageToTree(&m_pageMultiUser, IDS_PREF_MULTIUSERSS, IDC_SSCGROUP, FALSE);
		}

		if (AddPageToTree(&m_pageFile, IDS_PREF_FILE, IDC_TOPOFPAGE, bDoSearch))
		{
			AddPageToTree(&m_pageFile, IDS_PREF_FILELOAD, IDC_LOADGROUP, FALSE);
			AddPageToTree(&m_pageFile, IDS_PREF_FILEARCHIVE, IDC_ARCHIVEGROUP, FALSE);
			AddPageToTree(&m_pageFile, IDS_PREF_FILESWITCH, IDC_SWITCHGROUP, FALSE);
			AddPageToTree(&m_pageFile, IDS_PREF_FILENOTIFY, IDC_DUEGROUP, FALSE);
		}

		if (AddPageToTree(&m_pageFile2, IDS_PREF_FILEMORE, IDC_TOPOFPAGE, bDoSearch))
		{
			AddPageToTree(&m_pageFile2, IDS_PREF_FILEBACKUP, IDC_BACKUPGROUP, FALSE);
			AddPageToTree(&m_pageFile2, IDS_PREF_FILESAVE, IDC_SAVEGROUP, FALSE);
		}

		if (AddPageToTree(&m_pageUI, IDS_PREF_UI, IDC_TOPOFPAGE, bDoSearch))
		{
			AddPageToTree(&m_pageUI, IDS_PREF_UIFILTERING, IDC_FILTERGROUP, FALSE);
			AddPageToTree(&m_pageUI, IDS_PREF_UISORTING, IDC_SORTGROUP, FALSE);
			AddPageToTree(&m_pageUI, IDS_PREF_UITOOLBAR, IDC_TOOLBARGROUP, FALSE);
			AddPageToTree(&m_pageUI, IDS_PREF_UICOMMENTS, IDC_COMMENTSGROUP, FALSE);
			AddPageToTree(&m_pageUI, IDS_PREF_UITABBAR, IDC_TABBARGROUP, FALSE);
			AddPageToTree(&m_pageUI, IDS_PREF_TASKVIEWVISIBILITY, IDC_TASKVIEWSGROUP, FALSE);
		}

		AddPageToTree(&m_pageUICustomToolbar, IDS_PREF_TOOLBAR, IDC_TOPOFPAGE, bDoSearch);
		AddPageToTree(&m_pageUIVisibility, IDS_PREF_UIVISIBILITY, IDC_TOPOFPAGE, bDoSearch);
		AddPageToTree(&m_pageUITasklist, IDS_PREF_UITASK, IDC_TOPOFPAGE, bDoSearch);
		AddPageToTree(&m_pageUITasklistColors, IDS_PREF_UITASKCOLOR, IDC_TOPOFPAGE, bDoSearch);

		if (AddPageToTree(&m_pageTask, IDS_PREF_TASK, IDC_TOPOFPAGE, bDoSearch))
		{
			AddPageToTree(&m_pageTask, IDS_PREF_TIMETRACK, IDC_TRACKGROUP, FALSE);
			AddPageToTree(&m_pageTask, IDS_PREF_TASKTIME, IDC_TIMEGROUP, FALSE);
		}

		AddPageToTree(&m_pageTaskCalc, IDS_PREF_TASKCALCS, IDC_TOPOFPAGE, bDoSearch);
		AddPageToTree(&m_pageTaskDef, IDS_PREF_TASKDEFATTRIB, IDC_TOPOFPAGE, bDoSearch);

		if (AddPageToTree(&m_pageTaskDef2, IDS_PREF_TASKDEFINHERIT, IDC_TOPOFPAGE, bDoSearch))
			AddPageToTree(&m_pageTaskDef2, IDS_PREF_TASKDEFLISTS, IDC_DROPLISTGROUP, FALSE);

		AddPageToTree(&m_pageExport, IDS_PREF_EXPORT, IDC_TOPOFPAGE, bDoSearch);
		AddPageToTree(&m_pageTools, IDS_PREF_TOOLS, IDC_TOPOFPAGE, bDoSearch);
		AddPageToTree(&m_pageShortcuts, IDS_PREF_SHORTCUT, IDC_TOPOFPAGE, bDoSearch);
	}

	SynchronizeTree();
}

BOOL CPreferencesDlg::AddPageToTree(CPreferencesPageBase* pPage, UINT nIDPath, UINT nIDSection, BOOL bDoSearch)
{
	if (FindPage(pPage) == -1) 
	{
		ASSERT(0);
		return FALSE;
	}

	CStringArray aPath;
	VERIFY(Misc::Split(CEnString(nIDPath), aPath, PATHDELIM) >= 1);

	if (bDoSearch)
	{
		if (m_aSearchTerms.GetSize())
		{
			// Check path first
			int nPath = aPath.GetSize();

			while (nPath--)
			{
				if (CCtrlTextHighlighter::TextContainsOneOf(aPath[nPath], m_aSearchTerms))
					break;
			}

			if (!pPage->HighlightUIText(m_aSearchTerms, HILITE_BACKCOLOUR) && (nPath == -1))
				return FALSE;
		}
		else
		{
			pPage->ClearHighlights();
		}
	}

	// Add parent 'folders' first
	HTREEITEM htiParent = TVI_ROOT; // default
	int nNumPath = aPath.GetSize();

	for (int nPath = 0; nPath < (nNumPath - 1); nPath++)
	{
		// see if parent already exists
		const CString& sParent = aPath[nPath];

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
	}

	// Add actual 'leaf' page
	CString sPage = aPath[nNumPath - 1];
	HTREEITEM hti = m_tcPages.InsertItem(sPage, htiParent);
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

	return TRUE;
}

void CPreferencesDlg::OnTreeSelChanged(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	if (m_bBuildingTree)
		return;

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
		CWnd* pScrollTo = NULL;

		// If searching, move to the first match
		if (m_aSearchTerms.GetSize())
			pScrollTo = pPage->GetFirstHighlightedItem();

		// else move to the section
		if ((pScrollTo == NULL) && (nIDSection > 0))
			pScrollTo = pPage->GetDlgItem(nIDSection);

		if (pScrollTo)
			m_ppHost.ScrollTo(pScrollTo);
		else
			m_ppHost.ScrollToTop();

		// Custom page handling
		OnShowPage(pPage);
		
		// update caption
		m_sPageTitle = GetItemPath(htiSel);

		UpdateData(FALSE);
		UpdatePageTitleTextColors();
	}
	
	m_tcPages.SetFocus();
	
	*pResult = 0;
}

void CPreferencesDlg::OnShowPage(const CPreferencesPageBase* pPage)
{
	switch (m_ppHost.GetPageIndex(pPage))
	{
	case PREFPAGE_TASKDEF:
		{
			ASSERT(pPage == &m_pageTaskDef);

			CDWordArray aColors;

			m_pageUITasklistColors.GetPriorityColors(aColors);
			m_pageTaskDef.SetPriorityColors(aColors);

			UpdateTaskDefaultCommentsFont();
		}
		break;

	case PREFPAGE_UIFONTCOLOR:
		{
			ASSERT(pPage == &m_pageUITasklistColors);

			TDCAUTOLISTDATA defaultListData;
			GetDefaultListItems(defaultListData);

			defaultListData.AppendUnique(m_autoListData, TDCA_ALL);
			m_pageUITasklistColors.SetDefaultListData(defaultListData);
		}
		break;

	case PREFPAGE_TOOL:
		{
			ASSERT(pPage == &m_pageTools);
			m_pageTools.SetCustomAttributeDefs(m_aCustomAttribDefs);

			// Remove all UDT icons from menu icons mgr
			// will get re-added if/when m_pageShortcuts is shown
			if (m_mgrMenuIcons.HasImages())
			{
				for (int nCmdID = ID_TOOLS_USERTOOL1; nCmdID <= ID_TOOLS_USERTOOL50; nCmdID++)
					m_mgrMenuIcons.RemoveImage(nCmdID);
			}
		}
		break;

	case PREFPAGE_SHORTCUT:
		{
			ASSERT(pPage == &m_pageShortcuts);

			if (!m_mgrMenuIcons.HasImages())
			{
				m_mgrMenuIcons.Populate(*this);
				m_ilIcons.LoadDefaultImages(TRUE);

				if (m_pMgrStorage)
				{
					int nStorage = m_pMgrStorage->GetNumStorage();

					while (nStorage--)
					{
						HICON hIcon = m_pMgrStorage->GetStorageIcon(nStorage);

						m_mgrMenuIcons.AddImage(ID_FILE_OPEN_USERSTORAGE1 + nStorage, hIcon);
						m_mgrMenuIcons.AddImage(ID_FILE_SAVE_USERSTORAGE1 + nStorage, hIcon);
					}
				}
			}

			// Add toolbar images
			CTDCToolbarButtonArray aButtons;
			int nBtn = GetCustomToolbarButtons(aButtons);

			while (nBtn--)
			{
				const TDCCUSTOMTOOLBARBUTTON& tbb = aButtons[nBtn];

				if (!tbb.IsSeparator())
					m_mgrMenuIcons.AddImage(tbb.nMenuID, m_ilIcons, m_ilIcons.GetImageIndex(tbb.sImageID));
			}

			// Add UDT images
			CTDCUserToolArray aTools;
			int nTool = GetUserTools(aTools);

			while (nTool--)
			{
				const TDCUSERTOOL& tut = aTools[nTool];

				// Try built-in icons first (simple numbers)
				HICON hIcon = m_ilIcons.ExtractIcon(m_ilIcons.GetImageIndex(tut.sIconPath));

				if (!hIcon)
				{
					// Then image path
					if (!tut.sIconPath.IsEmpty())
						hIcon = CFileIcons::ExtractIcon(tut.sIconPath);

					// Then tool path
					if (!hIcon)
						hIcon = CFileIcons::ExtractIcon(CTDCToolsHelper::GetToolPath(tut));
				}

				if (hIcon)
					m_mgrMenuIcons.SetImage((nTool + ID_TOOLS_USERTOOL1), hIcon);
			}

			// Refresh 'New Task' icons
			m_mgrMenuIcons.UpdateNewTaskIcons(*this);

			InvalidateAllCtrls(pPage);
		}
		break;

	case PREFPAGE_TOOLBAR:
		{
			ASSERT(pPage == &m_pageUICustomToolbar);

			if (!m_ilIcons.GetSafeHandle())
				m_ilIcons.LoadDefaultImages(TRUE);

			// Remove all toolbar icons from menu icons mgr
			// will get re-added if/when m_pageShortcuts is shown
			if (m_mgrMenuIcons.HasImages())
			{
				CTDCToolbarButtonArray aButtons;
				int nBtn = m_pageUICustomToolbar.GetToolbarButtons(aButtons);

				while (nBtn--)
					m_mgrMenuIcons.RemoveImage(aButtons[nBtn].nMenuID);
			}
		}
		break;
	}
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
	m_pageTaskDef2.GetListItems(TDCA_CATEGORY, tld.aCategory);
	m_pageTaskDef2.GetListItems(TDCA_ALLOCTO, tld.aAllocTo);
	m_pageTaskDef2.GetListItems(TDCA_STATUS, tld.aStatus);
	m_pageTaskDef2.GetListItems(TDCA_ALLOCBY, tld.aAllocBy);
	m_pageTaskDef2.GetListItems(TDCA_VERSION, tld.aVersion);
	m_pageTaskDef2.GetListItems(TDCA_TAGS, tld.aTags);

	// add in any 'unknown' default items assigned directly to task
	TODOITEM tdiDef;
	GetDefaultTaskAttributes(tdiDef);

	Misc::AppendItems(tdiDef.aCategories, tld.aCategory, TRUE);
	Misc::AppendItems(tdiDef.aAllocTo, tld.aAllocTo, TRUE);
	Misc::AppendItems(tdiDef.aTags, tld.aTags, TRUE);

	Misc::AddUniqueItem(tdiDef.sStatus, tld.aStatus);
	Misc::AddUniqueItem(tdiDef.sAllocBy, tld.aAllocBy);
	Misc::AddUniqueItem(tdiDef.sVersion, tld.aVersion);

	return tld.GetSize();
}

void CPreferencesDlg::SetAutoListData(const TDCAUTOLISTDATA& autoListData)
{
	m_autoListData.Copy(autoListData, TDCA_ALL);
}

void CPreferencesDlg::SetCustomAttributeDefs(const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	m_aCustomAttribDefs.Copy(aAttribDefs);
}

BOOL CPreferencesDlg::EnableCustomAttributeInheritance()
{
	if (!m_pageTaskDef2.EnableCustomAttributeInheritance())
		return FALSE;

	CPreferencesPageBase* pPage = &m_pageTaskDef2;
	pPage->SavePreferences(m_prefs, PREFSKEY);

	return TRUE;
}

DWORD CPreferencesDlg::RemapMenuItemIDs(const CMap<UINT, UINT, UINT, UINT&>& mapMenuIDs)
{
	DWORD dwRes = 0;

	if (m_pageUICustomToolbar.RemapMenuItemIDs(mapMenuIDs))
	{
		CPreferencesPageBase* pPage = &m_pageUICustomToolbar;
		pPage->SavePreferences(m_prefs, PREFSKEY);

		dwRes |= PREFS_REMAPPEDTOOLBAR;
	}

	if (m_pageShortcuts.RemapMenuItemIDs(mapMenuIDs))
	{
		CPreferencesPageBase* pPage = &m_pageShortcuts;
		pPage->SavePreferences(m_prefs, PREFSKEY);

		dwRes |= PREFS_REMAPPEDSHORTCUTS;
	}

	return dwRes;
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

LRESULT CPreferencesDlg::OnGenPageEditLangFile(WPARAM /*wp*/, LPARAM /*lp*/)
{
	EndDialog(IDOK);

	// forward on to main app
	return AfxGetMainWnd()->PostMessage(WM_PGP_EDITLANGFILE);
}

void CPreferencesDlg::OnApply() 
{
	CPreferencesDlgBase::OnApply();	

	GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);
}

LRESULT CPreferencesDlg::OnControlChange(WPARAM wp, LPARAM lp)
{
	GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);

	// Per-page handling
	const CPreferencesPageBase* pPage = (const CPreferencesPageBase*)wp;

	if (pPage == &m_pageUITasklistColors)
	{
		switch (lp)
		{
		case IDC_COMMENTSFONTSIZE:
		case IDC_COMMENTSFONTLIST:
			UpdateTaskDefaultCommentsFont();
			break;
		}
	}

	return 0L;
}

void CPreferencesDlg::UpdateTaskDefaultCommentsFont()
{
	CString sFaceName;
	int nPointSize = -1;

	if (!GetCommentsFont(sFaceName, nPointSize))
	{
		if (!GetCommentsUseTreeFont() || !GetTreeFont(sFaceName, nPointSize))
		{
			nPointSize = GraphicsMisc::GetFontNameAndPointSize(*this, sFaceName);
		}
	}

	m_pageTaskDef.SetDefaultCommentsFont(sFaceName, nPointSize);

}

void CPreferencesDlg::ReposContents(CDeferWndMove& dwm, int nDX, int nDY)
{
	CPreferencesDlgBase::ReposContents(dwm, nDX, nDY);

	// resize tree
	dwm.ResizeCtrl(this, IDC_PAGES, 0, nDY);

	// page title
	dwm.ResizeCtrl(this, IDC_PAGE_TITLE, nDX, 0);

}

void CPreferencesDlg::SetUITheme(const CUIThemeFile& theme)
{
	m_theme = theme;

	m_sizer.SetBackgroundColor(theme.crAppBackLight);
	m_stPageTitle.SetBkgndColors(theme.crStatusBarLight, theme.crStatusBarDark);
	m_stPageTitle.SetBkgndStyle(theme.HasGlass(), theme.HasGradient());
	
	UpdatePageTitleTextColors();
	
	if (GetSafeHwnd())
		Invalidate(TRUE);
}

void CPreferencesDlg::UpdatePageTitleTextColors()
{
	COLORREF crText = m_theme.crStatusBarText, crBack = CLR_NONE;

	if (CCtrlTextHighlighter::TextContainsOneOf(m_sPageTitle, m_aSearchTerms))
	{
		crText = 0;
		crBack = HILITE_BACKCOLOUR;
	}

	m_stPageTitle.SetTextColors(crText, crBack);
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
}

BOOL CPreferencesDlg::IncrementTreeFontSize(BOOL bLarger, HFONT hFontFallback) 
{ 
	if (m_pageUITasklistColors.IncrementTreeFontSize(bLarger, hFontFallback))
	{
		CPreferencesPageBase* pPage = &m_pageUITasklistColors;
		pPage->SavePreferences(m_prefs, PREFSKEY);

		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CPreferencesDlg::RestoreTreeFontSize(HFONT hFontFallback) 
{ 
	if (m_pageUITasklistColors.RestoreTreeFontSize(hFontFallback))
	{
		CPreferencesPageBase* pPage = &m_pageUITasklistColors;
		pPage->SavePreferences(m_prefs, PREFSKEY);
		
		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CPreferencesDlg::SetAutoHideTabbar(BOOL bAutoHide)
{
	if (m_pageUI.SetAutoHideTabbar(bAutoHide))
	{
		CPreferencesPageBase* pPage = &m_pageUI;
		pPage->SavePreferences(m_prefs, PREFSKEY);
		
		return TRUE;
	}

	// else
	return FALSE;
}

FILTER_TITLE CPreferencesDlg::GetTitleFilterOption() const
{
	switch (m_pageUI.GetTitleFilterOption())
	{
	case PUIP_MATCHONTITLECOMMENTS:	return FT_FILTERONTITLECOMMENTS;
	case PUIP_MATCHONANYTEXT:		return FT_FILTERONANYTEXT;
	case PUIP_MATCHONTITLE:			return FT_FILTERONTITLEONLY;
	}

	ASSERT(0);
	return FT_FILTERONTITLEONLY;
}

LRESULT CPreferencesDlg::OnColorPageTextOption(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	TDCCOLEDITFILTERVISIBILITY vis;
	m_pageUIVisibility.GetColumnAttributeVisibility(vis);

	vis.ShowColorEditIfAsColumns(m_pageUITasklistColors.GetTextColorOption() == TEXTOPT_DEFAULT);
	m_pageUIVisibility.SetColumnAttributeVisibility(vis);

	return 0L;
}

LRESULT CPreferencesDlg::OnNumPriorityRiskLevels(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	int nNumLevels = m_pageUITasklist.GetNumPriorityRiskLevels();

	m_pageUITasklistColors.SetNumPriorityRiskLevels(nNumLevels);
	m_pageTaskDef.SetNumPriorityRiskLevels(nNumLevels);

	CDWordArray aColors;
	m_pageUITasklistColors.GetPriorityColors(aColors);

	m_pageTaskDef.SetPriorityColors(aColors);

	return 0L;
}

LRESULT CPreferencesDlg::OnUpdateSearch(WPARAM wParam, LPARAM lParam)
{
	ASSERT(wParam == IDC_SEARCH);
	UNREFERENCED_PARAMETER(wParam);

	// Only act if something has changed
	CStringArray aSearchText;

	switch (lParam)
	{
	case BTN_UPDATE:
		{
			CString sSearchText = GetCtrlText(&m_eSearchText);
			Misc::Split(sSearchText, aSearchText, ' ');
		}
		break;

	case BTN_CLEAR:
		m_eSearchText.SetWindowText(_T(""));
		break;

	default:
		ASSERT(0);
		return 0L;
	}

	if (!Misc::MatchAll(aSearchText, m_aSearchTerms))
	{
		m_aSearchTerms.Copy(aSearchText);

		if (!m_ppHost.PagesAreAllCreated())
		{
			CHoldRedraw hr(m_ppHost);
			CHoldRedraw hr2(m_stPageTitle);

			CWaitCursor cursor;

			// Forcibly create all pages and translate them
			int nPage = m_ppHost.GetPageCount();

			while (nPage--)
			{
				CPropertyPage* pPage = m_ppHost.GetPage(nPage);
				ASSERT(pPage);

				if (!pPage->GetSafeHwnd())
				{
					if (!EnsurePageCreated(nPage))
						return FALSE;

					// Showing the page triggers translation
					// as well as ensuring all content is ready
					pPage->ShowWindow(SW_SHOWNOACTIVATE);
					pPage->ShowWindow(SW_HIDE);
				}
			}
		}

		CHoldRedraw hr(m_tcPages);
		AddPagesToTree(TRUE);

		if (!m_tcPages.GetCount())
			AddPagesToTree(FALSE); // add all pages

		Resize();
	}

	return 0L;
}

void CPreferencesDlg::OnTreeCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = CDRF_DODEFAULT; // catch all

	NMTVCUSTOMDRAW* pTVCD = (NMTVCUSTOMDRAW*)pNMHDR;
	HTREEITEM hti = (HTREEITEM)pTVCD->nmcd.dwItemSpec;

	switch (pTVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:
		if (m_tcPages.GetSelectedItem() == hti)
		{
			pTVCD->clrText = GraphicsMisc::GetExplorerItemSelectionTextColor(CLR_NONE, GMIS_SELECTED, GMIB_THEMECLASSIC);
			*pResult = CDRF_NEWFONT;

			if (!CThemed::AreControlsThemed() || (COSVersion() < OSV_VISTA))
			{
				CDC* pDC = CDC::FromHandle(pTVCD->nmcd.hdc);
				BOOL bFocused = (GetFocus() == &m_tcPages);
				GM_ITEMSTATE nState = (bFocused ? GMIS_SELECTED : GMIS_SELECTEDNOTFOCUSED);

				GraphicsMisc::DrawExplorerItemSelection(pDC, m_tcPages, nState, pTVCD->nmcd.rc, GMIB_THEMECLASSIC);

				pTVCD->clrTextBk = GraphicsMisc::GetExplorerItemSelectionBackColor(nState, GMIB_THEMECLASSIC);
			}
		}
		
		if (m_aSearchTerms.GetSize())
		{
			*pResult |= CDRF_NOTIFYPOSTPAINT;
		}
		break;

	case CDDS_ITEMPOSTPAINT:
		{
			ASSERT(m_aSearchTerms.GetSize());

			CString sPage = m_tcPages.GetItemText(hti);

			if (CCtrlTextHighlighter::TextContainsOneOf(sPage, m_aSearchTerms))
			{
				CRect rText;
				m_tcPages.GetItemRect(hti, rText, TRUE);

				CDC* pDC = CDC::FromHandle(pTVCD->nmcd.hdc);
				
				pDC->SetTextColor(HILITE_TEXTCOLOUR);
				pDC->SetBkColor(HILITE_BACKCOLOUR);
				pDC->SetBkMode(OPAQUE);
				pDC->DrawText(sPage, rText, (DT_CENTER | DT_VCENTER | DT_SINGLELINE));

				*pResult = CDRF_SKIPDEFAULT;
				break;
			}
		}
		break;
	}
}

