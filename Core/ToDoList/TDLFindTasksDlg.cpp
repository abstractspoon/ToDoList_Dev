// TDLFindTasksDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "tdcmsg.h"
#include "TDLFindTasksDlg.h"
#include "TDCsearchparamHelper.h"

#include "..\shared\deferwndmove.h"
#include "..\shared\dlgunits.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\misc.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\themed.h"
#include "..\shared\dlgunits.h"
#include "..\shared\autoflag.h"
#include "..\shared\localizer.h"
#include "..\shared\osversion.h"

#include "..\Interfaces\Preferences.h"
#include "..\Interfaces\uithemefile.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

static SEARCHPARAM DEFAULTSEARCH(TDCA_TASKNAMEORCOMMENTS, FOP_INCLUDES);

/////////////////////////////////////////////////////////////////////////////

const int TABSTOPS			= GraphicsMisc::ScaleByDPIFactor(20);
const int SPLITTER_WIDTH	= GraphicsMisc::ScaleByDPIFactor(6);
const int DIALOG_BORDER		= GraphicsMisc::ScaleByDPIFactor(6);
const int MIN_LIST_SIZE		= GraphicsMisc::ScaleByDPIFactor(100);

const UINT WM_FTD_SELECTITEM = (WM_APP+1);

/////////////////////////////////////////////////////////////////////////////
// CTDLFindTasksDlg dialog

CTDLFindTasksDlg::CTDLFindTasksDlg(CWnd* pParent /*=NULL*/)
	: 
	CDialog(CTDLFindTasksDlg::IDD, pParent), 
	m_bDockable(FALSE),
	m_bSplitting(FALSE),
	m_bInitializing(FALSE),
	m_bAllTasklists(FALSE)
{
	m_sResultsLabel.LoadString(IDS_FTD_RESULTS);
	
	//{{AFX_DATA_INIT(CTDLFindTasksDlg)
	//}}AFX_DATA_INIT
}

CTDLFindTasksDlg::~CTDLFindTasksDlg()
{
}

void CTDLFindTasksDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLFindTasksDlg)
	DDX_Control(pDX, IDC_INCLUDE, m_cbInclude);
	DDX_Control(pDX, IDC_SEARCHLIST, m_cbSearches);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_FINDLIST, m_lcFindSetup);
	DDX_Control(pDX, IDC_RESULTS, m_lcResults);
	DDX_CBIndex(pDX, IDC_TASKLISTOPTIONS, m_bAllTasklists);
	DDX_Text(pDX, IDC_RESULTSLABEL, m_sResultsLabel);
}


BEGIN_MESSAGE_MAP(CTDLFindTasksDlg, CDialog)
	//{{AFX_MSG_MAP(CTDLFindTasksDlg)
	ON_COMMAND(ID_FIND_HELP, OnFindHelp)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_APPLYASFILTER, OnApplyasfilter)
	ON_BN_CLICKED(IDC_SELECTALL, OnSelectall)
	ON_CBN_EDITCHANGE(IDC_SEARCHLIST, OnEditchangeSearchlist)
	ON_CBN_SELCHANGE(IDC_SEARCHLIST, OnSelchangeSearchlist)
	ON_CBN_SELCHANGE(IDC_TASKLISTOPTIONS, OnSelchangeTasklistoptions)
	ON_CBN_SELCHANGE(IDC_INCLUDE, OnSelchangeInclude)
	ON_COMMAND(ID_DOCKBELOW, OnDockbelow)
	ON_COMMAND(ID_DOCKLEFT, OnDockleft)
	ON_COMMAND(ID_DOCKRIGHT, OnDockright)
	ON_COMMAND(ID_FIND_ADDRULE, OnAddRule)
	ON_COMMAND(ID_FIND_DELETERULE, OnDeleteRule)
	ON_COMMAND(ID_FIND_DELETESEARCH, OnDeleteSearch)
	ON_COMMAND(ID_FIND_FIND, OnFind)
	ON_COMMAND(ID_FIND_MOVERULEDOWN, OnMoveRuleDown)
	ON_COMMAND(ID_FIND_MOVERULEUP, OnMoveRuleUp)
	ON_COMMAND(ID_FIND_NEWSEARCH, OnNewSearch)
	ON_COMMAND(ID_FIND_SAVESEARCH, OnSaveSearch)
	ON_COMMAND(ID_UNDOCK, OnUndock)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_RESULTS, OnItemActivated)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FINDLIST, OnItemchangedRulelist)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_RESULTS, OnItemchangingResults)
	ON_NOTIFY(NM_CLICK, IDC_RESULTS, OnItemActivated)
	ON_NOTIFY(NM_DBLCLK, IDC_RESULTS, OnDblClkResults)
	ON_UPDATE_COMMAND_UI(ID_DOCKBELOW, OnUpdateDockbelow)
	ON_UPDATE_COMMAND_UI(ID_DOCKLEFT, OnUpdateDockleft)
	ON_UPDATE_COMMAND_UI(ID_DOCKRIGHT, OnUpdateDockright)
	ON_UPDATE_COMMAND_UI(ID_FIND_DELETERULE, OnUpdateDeleteRule)
	ON_UPDATE_COMMAND_UI(ID_FIND_DELETESEARCH, OnUpdateDeleteSearch)
	ON_UPDATE_COMMAND_UI(ID_FIND_MOVERULEDOWN, OnUpdateMoveRuleDown)
	ON_UPDATE_COMMAND_UI(ID_FIND_MOVERULEUP, OnUpdateMoveRuleUp)
	ON_UPDATE_COMMAND_UI(ID_FIND_SAVESEARCH, OnUpdateSaveSearch)
	ON_UPDATE_COMMAND_UI(ID_UNDOCK, OnUpdateUndock)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_HELPINFO()
	ON_REGISTERED_MESSAGE(WM_DM_DOCKCHANGE, OnNotifyDockChange)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_CAPTURECHANGED()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLFindTasksDlg message handlers

BOOL CTDLFindTasksDlg::OnInitDialog() 
{
	CAutoFlag af(m_bInitializing, TRUE);

	CDialog::OnInitDialog();

	VERIFY(m_sbGrip.Initialize(this));

	// build 'Include' combo
	CLocalizer::EnableTranslation(m_cbInclude, FALSE);

	BuildOptionCombo();

	m_icon.Load(IDI_FIND_DIALOG_STD);
	SetIcon(m_icon, FALSE);

	m_mgrPrompts.SetComboEditPrompt(m_cbSearches, IDS_FT_SAVESEARCHPROMPT);

	// always add a default search
	m_lcFindSetup.SetSearchParams(DEFAULTSEARCH);

	// toolbar
	VERIFY (InitializeToolbar());

	GetDlgItem(IDC_SELECTALL)->EnableWindow(FALSE);
	CenterWindow();

	LoadSettings();
	
    m_toolbar.RefreshButtonStates();
	
	ResizeDlg(FALSE);
	EnableToolTips();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLFindTasksDlg::BuildOptionCombo()
{
	int nIndex = m_cbInclude.AddString(CEnString(IDS_FINDINCLUDE_DONE));
	
	if (nIndex != CB_ERR)
		m_cbInclude.SetItemData(nIndex, FI_COMPLETED);

	nIndex = m_cbInclude.AddString(CEnString(IDS_FINDINCLUDE_PARENTS));
	
	if (nIndex != CB_ERR)
		m_cbInclude.SetItemData(nIndex, FI_PARENT);

	nIndex = m_cbInclude.AddString(CEnString(IDS_FINDINCLUDE_FILTEREDOUT));
	
	if (nIndex != CB_ERR)
		m_cbInclude.SetItemData(nIndex, FI_FILTEREDOUT);
}

BOOL CTDLFindTasksDlg::IncludeOptionIsChecked(FIND_INCLUDE nOption) const
{
	if (m_cbInclude.GetSafeHwnd())
	{
		int nItem = FindItemByData(m_cbInclude, nOption);
		
		if (nItem != CB_ERR)
			return m_cbInclude.GetCheck(nItem);
	}

	return FALSE;
}

void CTDLFindTasksDlg::CheckIncludeOption(FIND_INCLUDE nOption, BOOL bCheck)
{
	int nItem = FindItemByData(m_cbInclude, nOption);

	if (nItem != CB_ERR)
		m_cbInclude.SetCheck(nItem, (bCheck ? CCBC_CHECKED : CCBC_UNCHECKED));
}

void CTDLFindTasksDlg::SetUITheme(const CUIThemeFile& theme)
{
	ASSERT (GetSafeHwnd());

	if (CThemed::IsAppThemed())
	{
		// cache current theme
		CUIThemeFile themeCur = m_theme;
		m_theme = theme;

		m_sbGrip.SetBackgroundColor(theme.crAppBackLight);

		GraphicsMisc::VerifyDeleteObject(m_brBkgnd);
		m_brBkgnd.CreateSolidBrush(theme.crAppBackLight);

		// do we have recreate the toolbar?
		if (m_theme != themeCur)
		{
			// change parent of combo so it doesn't get destroyed
			// will get reset in InitializeToolbar
			m_cbSearches.SetParent(this);

			m_toolbar.DestroyWindow();
			m_tbHelper.Release();

			VERIFY (InitializeToolbar());
		}

		m_toolbar.SetBackgroundColors(theme.crToolbarLight, 
									theme.crToolbarDark, 
									theme.nRenderStyle != UIRS_GLASS, 
									theme.nRenderStyle != UIRS_GRADIENT);
		m_toolbar.SetHotColor(m_theme.crToolbarHot);
		m_toolbar.RefreshButtonStates(TRUE);

		ResizeDlg(FALSE);
		Invalidate();
	}
}

BOOL CTDLFindTasksDlg::InitializeToolbar()
{
	if (m_toolbar.GetSafeHwnd())
		return TRUE;

	if (!m_toolbar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP))
		return FALSE;

	if (!m_toolbar.LoadToolBar(IDR_FIND_TOOLBAR))
		return FALSE;
	
	m_toolbar.SetBorders(4, 4, 0, 0);

	if (CThemed::IsAppThemed())
	{
		m_toolbar.SetBackgroundColors(m_theme.crToolbarLight, 
									m_theme.crToolbarDark, 
									m_theme.nRenderStyle != UIRS_GLASS, 
									m_theme.nRenderStyle != UIRS_GRADIENT);
		m_toolbar.SetHotColor(m_theme.crToolbarHot);
	}

	m_toolbar.SetImage(IDB_FIND_TOOLBAR_STD, RGB(255, 0, 255));
	
	VERIFY(m_tbHelper.Initialize(&m_toolbar, this));
	
	// very important - turn OFF all the auto positioning and sizing
	// by default have no borders
	UINT nStyle = m_toolbar.GetBarStyle();
	nStyle &= ~(CCS_NORESIZE | CCS_NOPARENTALIGN | CBRS_BORDER_ANY);
	nStyle |= (CBRS_SIZE_FIXED | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_toolbar.SetBarStyle(nStyle);
	
	// insert combobox beside the separator to the right of ID_UNDOCK
	int nCombo = m_toolbar.CommandToIndex(ID_UNDOCK) + 2;
	TBBUTTON btnCombo = { 0, ID_SEARCHLIST, 0, TBSTYLE_SEP, 0, NULL };
	m_toolbar.GetToolBarCtrl().InsertButton(nCombo, &btnCombo);

	CRect rCombo;
	m_cbSearches.GetClientRect(rCombo);
	m_cbSearches.SetParent(&m_toolbar); 
	
	TBBUTTONINFO tbi;
	tbi.cbSize = sizeof( TBBUTTONINFO );
	tbi.cx = (WORD)rCombo.Width();
	tbi.dwMask = TBIF_SIZE;  // By index
	m_toolbar.GetToolBarCtrl().SetButtonInfo(ID_SEARCHLIST, &tbi );

	// reposition combo
	m_toolbar.GetToolBarCtrl().GetRect(ID_SEARCHLIST, rCombo);
	rCombo.bottom = rCombo.top + 200;
	m_cbSearches.MoveWindow(rCombo);

	return TRUE;
}

void CTDLFindTasksDlg::OnAddRule() 
{
	m_lcFindSetup.AddRule();
	m_lcFindSetup.SetFocus();
	EnableApplyAsFilterButton();
}

void CTDLFindTasksDlg::OnDeleteRule() 
{
	m_lcFindSetup.DeleteSelectedRule();
	EnableApplyAsFilterButton();
}

void CTDLFindTasksDlg::OnUpdateDeleteRule(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_lcFindSetup.CanDeleteSelectedRule());
}

/////////////////////////////////////////////////////////////////////////////////////

BOOL CTDLFindTasksDlg::Create(CWnd* pParent, BOOL bDockable)
{
	ASSERT (pParent || !bDockable);
	
	if (!pParent && bDockable)
		return FALSE;
	
	if (GetSafeHwnd())
		return TRUE;
	
	m_bDockable = bDockable;

	DPI_AWARENESS_CONTEXT nPrev = GraphicsMisc::SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);

	BOOL bResult = CDialog::Create(CTDLFindTasksDlg::IDD, pParent);

	GraphicsMisc::SetThreadDpiAwarenessContext(nPrev);

	return bResult;
}

void CTDLFindTasksDlg::LoadSettings()
{
	CPreferences prefs;

	m_bAllTasklists = prefs.GetProfileInt(_T("FindTasks"), _T("SearchAllTaskLists"), FALSE);

	// update list ctrl
	if (m_bAllTasklists)
		m_lcFindSetup.SetCustomAttributes(m_aAllTDCAttribDefs);
	else
		m_lcFindSetup.SetCustomAttributes(m_aActiveTDCAttribDefs);

	// results column widths
	CIntArray aWidths;
	
	aWidths.Add(prefs.GetProfileInt(_T("FindTasks"), _T("Col0Width"), 250));
	aWidths.Add(prefs.GetProfileInt(_T("FindTasks"), _T("Col1Width"), 150));
	aWidths.Add(prefs.GetProfileInt(_T("FindTasks"), _T("Col2Width"), 100));

	VERIFY(m_lcResults.SetColumnWidths(aWidths));

	// include options
	CheckIncludeOption(FI_COMPLETED, prefs.GetProfileInt(_T("FindTasks"), _T("IncludeDoneTasks"), FALSE));
	CheckIncludeOption(FI_PARENT, prefs.GetProfileInt(_T("FindTasks"), _T("IncludeParentTasks"), TRUE));
	CheckIncludeOption(FI_FILTEREDOUT, prefs.GetProfileInt(_T("FindTasks"), _T("IncludeFilteredOutTasks"), TRUE));

	if (m_bDockable)
	{
		DM_POS nPos = DMP_UNDOCKED, nLastPos = DMP_RIGHT;
		DWORD dwTopLeft = (DWORD)prefs.GetProfileInt(_T("FindTasks"), _T("TopLeft"), -1);
		DWORD dwBottomRight = (DWORD)prefs.GetProfileInt(_T("FindTasks"), _T("BottomRight"), -1);
		
		if (dwBottomRight != -1 && dwTopLeft != -1)
		{
			nPos = prefs.GetProfileEnum(_T("FindTasks"), _T("DockPos"), DMP_UNDOCKED);
			nLastPos = prefs.GetProfileEnum(_T("FindTasks"), _T("LastDockPos"), DMP_RIGHT);

			CRect rect(GET_X_LPARAM(dwTopLeft), GET_Y_LPARAM(dwTopLeft), 
					GET_X_LPARAM(dwBottomRight), GET_Y_LPARAM(dwBottomRight));

			// ensure this intersects with the desktop by a decent amount
			int BORDER = 200;
			rect.DeflateRect(BORDER, BORDER);

			if (NULL != MonitorFromRect(rect, MONITOR_DEFAULTTONULL))
			{
				rect.InflateRect(BORDER, BORDER);
				MoveWindow(rect);
			}
		}
		
		// use the default size of the page host to initialize the 
		// docked width/height
		CSize rDef = GetMinDockedSize(nPos);
		
		int nWidthDocked = prefs.GetProfileInt(_T("FindTasks"), _T("DockedWidth"), rDef.cx);
		int nWidthDockedMax = prefs.GetProfileInt(_T("FindTasks"), _T("DockedWidthMax"), -1);
		
		int nHeightDocked = prefs.GetProfileInt(_T("FindTasks"), _T("DockedHeight"), rDef.cy);
		int nHeightDockedMax = prefs.GetProfileInt(_T("FindTasks"), _T("DockedHeightMax"), -1);
		
		m_dockMgr.Initialize(GetParent(), this, nPos, nLastPos,
							nWidthDocked, nWidthDockedMax, nHeightDocked, nHeightDockedMax);

		// Splitter
		int nSplitPos = prefs.GetProfileInt(_T("FindTasks"), _T("SplitterPos"), -1);

		if (nSplitPos != -1)
			SetSplitterPos(nSplitPos);
	}

	LoadSearches();

	UpdateData(FALSE);
}

CSize CTDLFindTasksDlg::GetMinDockedSize(DM_POS nPos)
{
	CRect rMin(0, 0, 0, 0), rCtrl;

	if (nPos == DMP_BELOW)
	{
		// we need enough height to show a few results and enough width
		// to display the results list
		rCtrl = GetCtrlRect(this, IDC_TASKLISTOPTIONS);
		rMin.bottom = rCtrl.bottom + CDlgUnits(this).ToPixelsY(60);

		rCtrl = GetChildRect(&m_lcFindSetup);
		rMin.right = rCtrl.right + CDlgUnits(this).ToPixelsX(60);
	}
	else
	{
		// we need enough width to show the toolbar and enough height
		// to display a few results
		rCtrl = GetChildRect(&m_lcFindSetup);
		rMin.bottom = rCtrl.bottom + CDlgUnits(this).ToPixelsY(90);

		rMin.right = m_toolbar.GetMinReqLength();
	}

	// add border
	int nBorder = CDlgUnits(this).ToPixelsX(7);
	rMin.InflateRect(nBorder, nBorder);
	
	// allow for non-client border
	rMin.left = rMin.top = 0;
	CalcWindowRect(rMin);
	
	return rMin.Size();
}

BOOL CTDLFindTasksDlg::Show(BOOL bShow)
{
	if (!GetSafeHwnd())
		return FALSE;
	
	if (bShow)
	{
		// reset focus if dialog was previously hidden
		if (!IsWindowVisible())
		{
			if (m_lcFindSetup.GetItemText(0, 0).IsEmpty())
				m_lcFindSetup.SetCurSel(0, 0);

			else if (m_lcFindSetup.GetItemText(0, 1).IsEmpty())
				m_lcFindSetup.SetCurSel(0, 1);

			else 
				m_lcFindSetup.SetCurSel(0, 2);

			m_lcFindSetup.SetFocus();
		}
		
	    m_toolbar.RefreshButtonStates();

		ShowWindow(SW_SHOW);
		SetForegroundWindow(); // give it the focus
	}
	else
		ShowWindow(SW_HIDE);
	
	return TRUE;
}

void CTDLFindTasksDlg::SetCustomAttributes(const CTDCCustomAttribDefinitionArray& aActiveTasklistAttribDefs,
											const CTDCCustomAttribDefinitionArray& aAllTasklistsAttribDefs)
{
	m_aActiveTDCAttribDefs.Copy(aActiveTasklistAttribDefs); // always
	m_aAllTDCAttribDefs.Copy(aAllTasklistsAttribDefs); // always

	// update list ctrl
	if (m_bAllTasklists)
		m_lcFindSetup.SetCustomAttributes(aAllTasklistsAttribDefs);
	else
		m_lcFindSetup.SetCustomAttributes(aActiveTasklistAttribDefs);
}

void CTDLFindTasksDlg::SetAttributeListData(const TDCAUTOLISTDATA& tldActive, const TDCAUTOLISTDATA& tldAll, TDC_ATTRIBUTE nAttribID)
{
	m_tldActive.Copy(tldActive, nAttribID);
	m_tldAll.Copy(tldAll, nAttribID);

	// update list ctrl
	if (m_bAllTasklists)
		m_lcFindSetup.SetAttributeListData(tldAll, nAttribID);
	else
		m_lcFindSetup.SetAttributeListData(tldActive, nAttribID);
}

void CTDLFindTasksDlg::SetActiveTasklist(const CString& sTasklist, BOOL bWantDefaultIcons)
{
	m_lcFindSetup.SetActiveTasklist(sTasklist, bWantDefaultIcons);
}

void CTDLFindTasksDlg::AddResult(const SEARCHRESULT& result, const CFilteredToDoCtrl* pTDC)
{
	if (GetSafeHwnd())
	{
		// Don't add what the user doesn't want to see
		// Unless the current rule set includes TDCA_DONEDATE
		if (result.HasFlag(RF_DONE) || result.HasFlag(RF_GOODASDONE))
		{
			if (!m_lcFindSetup.HasRule(TDCA_DONEDATE) && !IncludeOptionIsChecked(FI_COMPLETED))
			{
				return;
			}
		}

		if (result.HasFlag(RF_PARENT) && !IncludeOptionIsChecked(FI_PARENT))
		{
			return;
		}

		// else
		int nIndex = m_lcResults.AddResult(result, pTDC);

		if (nIndex != -1)
		{
			// update 'found' count
			m_sResultsLabel.Format(IDS_FTD_SOMERESULTS, GetResultCount());
			UpdateData(FALSE);

			// focus first item added
			if (!GetDlgItem(IDC_SELECTALL)->IsWindowEnabled())
			{
				SelectItem(nIndex);
				m_lcResults.SetFocus();

				// update 'search results' button' state
				m_toolbar.RefreshButtonStates();

				// enable 'select all' button
				GetDlgItem(IDC_SELECTALL)->EnableWindow(TRUE);
			}
		}
	}
}

void CTDLFindTasksDlg::AddHeaderRow(LPCTSTR szText)
{
	m_lcResults.AddHeaderRow(szText);
}

BOOL CTDLFindTasksDlg::GetSearchAllTasklists()
{
	if (!m_bInitializing && GetSafeHwnd())
		UpdateData();
	
	return m_bAllTasklists;
}

int CTDLFindTasksDlg::GetSearchParams(LPCTSTR szName, TDCADVANCEDFILTER& filter) const
{
	if (GetSearchParams(szName, filter.params))
	{
		filter.sName = szName;
		Misc::SetFlag(filter.dwFlags, FO_HIDEDONE, filter.params.bIgnoreDone);
	}

	return filter.params.aRules.GetSize();
}

int CTDLFindTasksDlg::GetSearchParams(TDCADVANCEDFILTER& filter)
{
	if (GetSearchParams(filter.params))
		Misc::SetFlag(filter.dwFlags, FO_HIDEDONE, filter.params.bIgnoreDone);

	return filter.params.aRules.GetSize();
}

int CTDLFindTasksDlg::GetSearchParams(SEARCHPARAMS& params) 
{ 
	if (!m_bInitializing && GetSafeHwnd())
		UpdateData();
	
	m_lcFindSetup.GetSearchParams(params.aRules); 

	params.bIgnoreOverDue = FALSE;
	params.bIgnoreDone = !IncludeOptionIsChecked(FI_COMPLETED);
	params.bIgnoreFilteredOut = !IncludeOptionIsChecked(FI_FILTEREDOUT);

	// if the the search params include TDCA_DONE date then 
	// forcibly remove FT_HIDEDONE flag
	if (params.bIgnoreDone && params.HasAttribute(TDCA_DONEDATE))
		params.bIgnoreDone = FALSE;

	// associated custom attribute defs
	if (m_bAllTasklists)
		params.aAttribDefs.Copy(m_aAllTDCAttribDefs);
	else
		params.aAttribDefs.Copy(m_aActiveTDCAttribDefs);

	return params.aRules.GetSize();
}

int CTDLFindTasksDlg::GetSearchParams(LPCTSTR szName, SEARCHPARAMS& params) const
{
	BOOL bWantDone, bWantParents, bWantFilteredOut;

	if (LoadSearch(szName, params.aRules, bWantDone, bWantParents, bWantFilteredOut))
	{
		// associated custom attribute defs
		if (m_bAllTasklists)
			params.aAttribDefs.Copy(m_aAllTDCAttribDefs);
		else
			params.aAttribDefs.Copy(m_aActiveTDCAttribDefs);

		params.bIgnoreOverDue = FALSE;
		params.bIgnoreDone = !bWantDone;
		params.bIgnoreFilteredOut = !IncludeOptionIsChecked(FI_FILTEREDOUT);

		return params.aRules.GetSize();
	}

	return 0;
}

int CTDLFindTasksDlg::GetResultCount() const
{
	return m_lcResults.GetResultCount();
}

int CTDLFindTasksDlg::GetResultCount(const CFilteredToDoCtrl* pTDC) const
{
	return m_lcResults.GetResultCount(pTDC);
}

int CTDLFindTasksDlg::GetAllResults(CFTDResultsArray& aResults) const
{
	return m_lcResults.GetAllResults(aResults);
}

int CTDLFindTasksDlg::GetResults(const CFilteredToDoCtrl* pTDC, CFTDResultsArray& aResults) const
{
	return m_lcResults.GetResults(pTDC, aResults);
}

int CTDLFindTasksDlg::GetResultIDs(const CFilteredToDoCtrl* pTDC, CDWordArray& aTaskIDs) const
{
	return m_lcResults.GetResultIDs(pTDC, aTaskIDs);
}

void CTDLFindTasksDlg::RefreshSearch()
{
	ASSERT (GetSafeHwnd());
	
	OnFind();
}

void CTDLFindTasksDlg::OnFind() 
{
	m_lcFindSetup.EndEdit();
	UpdateData();

	// notify parent
	GetParent()->PostMessage(WM_FTD_FIND);
	
	// clear results
	DeleteAllResults();
	
	// disable 'select all' button
	GetDlgItem(IDC_SELECTALL)->EnableWindow(FALSE);
}

void CTDLFindTasksDlg::OnClose() 
{
	CDialog::OnClose();

	// end any current edit
	m_lcFindSetup.EndEdit();
	
	// hide
	ShowWindow(SW_HIDE);
	
	// notify parent
	GetParent()->SendMessage(WM_FTD_CLOSE);
}

void CTDLFindTasksDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	// end any current edit
	if (m_lcFindSetup.GetSafeHwnd())
		m_lcFindSetup.EndEdit();
	
	ResizeDlg(FALSE, cx, cy);

	if (m_toolbar.GetSafeHwnd())
		m_toolbar.RefreshButtonStates();
}

LRESULT CTDLFindTasksDlg::OnNotifyDockChange(WPARAM wp, LPARAM lp)
{
	ResizeDlg((wp == DMP_BELOW) || (lp == DMP_BELOW));

	return 0L;
}

void CTDLFindTasksDlg::ResizeDlg(BOOL bOrientationChange, int cx, int cy)
{
	if (m_lcResults.GetSafeHwnd())
	{
		if (!cx && !cy)
		{
			CRect rClient;
			GetClientRect(rClient);
			
			cx = rClient.right;
			cy = rClient.bottom;
			
			// check again 
			if (!cx && !cy)
				return;
		}

		BOOL bVertSplitter = IsSplitterVertical();
		CRect rSplitter = GetSplitterRect();

		CRect rRules = GetCtrlRect(this, IDC_FINDLIST);
		CRect rResults = GetCtrlRect(this, IDC_RESULTS);
		CRect rApply = GetCtrlRect(this, IDC_APPLYASFILTER);

		if (bOrientationChange)
		{
			// Place the splitter so that the two lists have equal height/width
			int nSplitPos = 0;

			if (bVertSplitter)
				nSplitPos = (cx / 2);
			else
				nSplitPos = ((cy - DIALOG_BORDER + rRules.top - (rResults.top - rApply.top)) / 2);

			VERIFY(GetSplitterRect(rSplitter, nSplitPos));
		}
		
		{
			CDeferWndMove dwm(10);

			// Rules list
			if (bVertSplitter)
			{
				rRules.right = rSplitter.left;	
				rRules.bottom = (cy - DIALOG_BORDER);	
			}
			else
			{
				rRules.right = (cx - DIALOG_BORDER);	
				rRules.bottom = rSplitter.top;	
			}
			
			dwm.MoveWindow(&m_lcFindSetup, rRules);

			// Results controls
			int nXOffset = 0, nYOffset = 0;
			
			if (bVertSplitter)
			{
				nXOffset = (rSplitter.right - rResults.left);
				nYOffset = (rSplitter.top - rResults.top);
			}
			else
			{
				nXOffset = (rSplitter.left - rApply.left);
				nYOffset = (rSplitter.bottom + SPLITTER_WIDTH - rApply.top);
			}
			
			dwm.OffsetCtrl(this, IDC_APPLYASFILTER, nXOffset, nYOffset);
			dwm.OffsetCtrl(this, IDC_SELECTALL, nXOffset, nYOffset);
			dwm.OffsetCtrl(this, IDC_RESULTSLABEL, nXOffset, nYOffset);
			
			rResults = dwm.OffsetCtrl(this, IDC_RESULTS, nXOffset, nYOffset);
			rResults.right = (cx - DIALOG_BORDER);	
			rResults.bottom = (cy - DIALOG_BORDER);	

			dwm.MoveWindow(&m_lcResults, rResults);
		}
		
		m_toolbar.Resize(cx);
		Invalidate();
	}	
}

void CTDLFindTasksDlg::OnItemActivated(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (pNMListView->iItem >= 0)
	{
		FTDRESULT* pRes = m_lcResults.GetResult(pNMListView->iItem);
		
		if (pRes)
			GetParent()->SendMessage(WM_FTD_SELECTRESULT, 0, (LPARAM)pRes);
	}
	
	*pResult = 0;
}


void CTDLFindTasksDlg::OnDblClkResults(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	CPoint pt(::GetMessagePos());
	m_lcResults.ScreenToClient(&pt);
	
	// int nHit = m_lcResults.HitTest(pt);
	// 
	// if (nHit != -1)
	ShowWindow(SW_HIDE);
	*pResult = 0;
}

void CTDLFindTasksDlg::OnDestroy() 
{
	SaveSettings();
	DeleteAllResults();
	
	CDialog::OnDestroy();
}

void CTDLFindTasksDlg::SaveSettings()
{
	UpdateData();

	CPreferences prefs;

	prefs.WriteProfileInt(_T("FindTasks"), _T("SearchAllTaskLists"), m_bAllTasklists);

	// include settings
	prefs.WriteProfileInt(_T("FindTasks"), _T("IncludeDoneTasks"), IncludeOptionIsChecked(FI_COMPLETED));
	prefs.WriteProfileInt(_T("FindTasks"), _T("IncludeParentTasks"), IncludeOptionIsChecked(FI_PARENT));
	prefs.WriteProfileInt(_T("FindTasks"), _T("IncludeFilteredOutTasks"), IncludeOptionIsChecked(FI_FILTEREDOUT));

	// pos
	CRect rDialog = m_dockMgr.GetUnDockedRect();
	
	prefs.WriteProfileInt(_T("FindTasks"), _T("TopLeft"), MAKELPARAM(rDialog.left, rDialog.top));
	prefs.WriteProfileInt(_T("FindTasks"), _T("BottomRight"), MAKELPARAM(rDialog.right, rDialog.bottom));
	prefs.WriteProfileInt(_T("FindTasks"), _T("DockPos"), m_dockMgr.GetDockPos());
	prefs.WriteProfileInt(_T("FindTasks"), _T("LastDockPos"), m_dockMgr.GetLastDockPos());
	prefs.WriteProfileInt(_T("FindTasks"), _T("DockedWidth"), m_dockMgr.GetDockedWidth(FALSE));
	prefs.WriteProfileInt(_T("FindTasks"), _T("DockedWidthMax"), m_dockMgr.GetDockedWidth(TRUE));
	prefs.WriteProfileInt(_T("FindTasks"), _T("DockedHeight"), m_dockMgr.GetDockedHeight(FALSE));
	prefs.WriteProfileInt(_T("FindTasks"), _T("DockedHeightMax"), m_dockMgr.GetDockedHeight(TRUE));

	// Splitter
	CPoint ptSplitter = GetSplitterRect().CenterPoint();
	prefs.WriteProfileInt(_T("FindTasks"), _T("SplitterPos"), (IsSplitterVertical() ? ptSplitter.x : ptSplitter.y));

	// results column widths
	CIntArray aWidths;
	VERIFY(m_lcResults.GetColumnWidths(aWidths) == 3);

	prefs.WriteProfileInt(_T("FindTasks"), _T("Col0Width"), aWidths[0]);
	prefs.WriteProfileInt(_T("FindTasks"), _T("Col1Width"), aWidths[1]);
	prefs.WriteProfileInt(_T("FindTasks"), _T("Col2Width"), aWidths[2]);

	// searches
	SaveSearches();
}

void CTDLFindTasksDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	CDialog::OnGetMinMaxInfo(lpMMI);
	
	if (m_dockMgr.Initialized())
	{
		CSize rDef = GetMinDockedSize(m_dockMgr.GetDockPos());

		lpMMI->ptMinTrackSize.y = rDef.cy;
		lpMMI->ptMinTrackSize.x = rDef.cx;
	}
}

BOOL CTDLFindTasksDlg::PreTranslateMessage(MSG* pMsg) 
{
	// handle enter key if results list has the focus
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN && pMsg->hwnd == m_lcResults)
	{
		int nSel = m_lcResults.GetCurSel();
		
		if (nSel != -1)
		{
			FTDRESULT* pRes = m_lcResults.GetResult(nSel);
			
			if (pRes)
				GetParent()->SendMessage(WM_FTD_SELECTRESULT, 0, (LPARAM)pRes);
			
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CTDLFindTasksDlg::OnSelectall() 
{
	GetParent()->SendMessage(WM_FTD_SELECTALL);
}

void CTDLFindTasksDlg::OnDockright() 
{
	// toggle docked state
	if (m_dockMgr.GetDockPos() == DMP_RIGHT)
		m_dockMgr.UnDock();
	else
		m_dockMgr.Dock(DMP_RIGHT);

    m_toolbar.RefreshButtonStates();
}

void CTDLFindTasksDlg::OnUpdateDockright(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(m_dockMgr.GetDockPos() == DMP_RIGHT);
}

void CTDLFindTasksDlg::OnDockleft() 
{
	// toggle docked state
	if (m_dockMgr.GetDockPos() == DMP_LEFT)
		m_dockMgr.UnDock();
	else
		m_dockMgr.Dock(DMP_LEFT);

    m_toolbar.RefreshButtonStates();
}

void CTDLFindTasksDlg::OnUpdateDockleft(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(m_dockMgr.GetDockPos() == DMP_LEFT);
}

void CTDLFindTasksDlg::OnUndock() 
{
	m_dockMgr.UnDock();
    m_toolbar.RefreshButtonStates();
}

void CTDLFindTasksDlg::OnUpdateUndock(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(m_dockMgr.GetDockPos() == DMP_UNDOCKED);
}

void CTDLFindTasksDlg::OnDockbelow() 
{
	// toggle docked state
	if (m_dockMgr.GetDockPos() == DMP_BELOW)
		m_dockMgr.UnDock();
	else
		m_dockMgr.Dock(DMP_BELOW);

    m_toolbar.RefreshButtonStates();
}

void CTDLFindTasksDlg::OnUpdateDockbelow(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(m_dockMgr.GetDockPos() == DMP_BELOW);
}

void CTDLFindTasksDlg::DeleteResults(const CFilteredToDoCtrl* pTDC)
{
	if (!GetSafeHwnd())
		return;

	m_lcResults.DeleteResults(pTDC);

	// update 'found' count
	m_sResultsLabel.Format(IDS_FTD_SOMERESULTS, GetResultCount());
	UpdateData(FALSE);
}

void CTDLFindTasksDlg::DeleteAllResults()
{
	if (!GetSafeHwnd())
		return;

	m_lcResults.DeleteAllResults();

	// clear 'found' count
	m_sResultsLabel.LoadString(IDS_FTD_RESULTS);
	UpdateData(FALSE);
}

void CTDLFindTasksDlg::OnItemchangingResults(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	// we're interested in keyboard selection state changes only
	if (pNMListView->uChanged & LVIF_STATE)
	{
		BOOL bWasSel = (pNMListView->uOldState & LVIS_SELECTED);
		BOOL bIsSel = (pNMListView->uNewState & LVIS_SELECTED);
		
		// has a header row just become selected?
		if (bIsSel && !bWasSel && pNMListView->lParam == 0)
		{
			if (Misc::IsKeyPressed(VK_LBUTTON))
			{
				// restore previous selection
				SelectItem(m_nCurSel);
			}
			else // keyboard
			{
				// we need to get the next valid item in the direction
				// the user is moving
				BOOL bDown = (GetSelectedItem() < pNMListView->iItem);
				int nNextItem = GetNextResult(pNMListView->iItem, bDown);
				
				if (nNextItem != -1)
					SelectItem(nNextItem);
				else
				{
					SelectItem(m_nCurSel);

					// make sure the non-selectable item is visible
					m_lcResults.EnsureVisible(pNMListView->iItem, FALSE);
				}
			}
			
			*pResult = TRUE;
			return;
		}
	}

	m_nCurSel = pNMListView->iItem;
	
	*pResult = 0;
}

int CTDLFindTasksDlg::GetNextResult(int nItem, BOOL bDown)
{
	int nNext = nItem + (bDown ? 1 : -1); // next item

	while (nNext >= 0 && nNext < m_lcResults.GetItemCount())
	{
		if (m_lcResults.GetResult(nNext))
			return nNext;

		nNext += bDown ? 1 : -1; // next item
	}

	// no next item
	return -1;
}

void CTDLFindTasksDlg::SelectItem(int nItem)
{
	m_lcResults.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED, 
							 		 LVIS_FOCUSED | LVIS_SELECTED);

	m_nCurSel = nItem;
}

int CTDLFindTasksDlg::GetSelectedItem()
{
	return m_nCurSel;
}

void CTDLFindTasksDlg::OnItemchangedRulelist(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
//	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (m_toolbar.GetSafeHwnd())
		m_toolbar.RefreshButtonStates();

	// enable 'set as filter' provided there is something to set
	EnableApplyAsFilterButton();

	*pResult = 0;
}

void CTDLFindTasksDlg::EnableApplyAsFilterButton()
{
	SEARCHPARAMS params;
	GetDlgItem(IDC_APPLYASFILTER)->EnableWindow(GetSearchParams(params));
}

int CTDLFindTasksDlg::GetSavedSearches(CStringArray& aNames)
{
	aNames.Copy(m_aSavedSearches);

	if (aNames.GetSize() == 0 && GetSafeHwnd() == NULL)
	{
		CPreferences prefs;
		int nNumItems = prefs.GetProfileInt(_T("FindTasks\\Searches"), _T("NumSearches"), 0);

		for (int nItem = 0; nItem < nNumItems; nItem++)
		{
			CString sKey = Misc::MakeKey(_T("Search%d"), nItem);
			CString sSearch = prefs.GetProfileString(_T("FindTasks\\Searches"), sKey);

			aNames.Add(sSearch);
		}
	}
	
	// always sort by name
	Misc::SortArray(aNames);

	return aNames.GetSize();
}

void CTDLFindTasksDlg::OnSaveSearch() 
{
	m_lcFindSetup.EndEdit();
	
	CString sSearch;
	m_cbSearches.GetWindowText(sSearch);

	SaveSearch(sSearch);
	m_sActiveSearch = sSearch;

	// is it a new search?
	BOOL bNewSearch = (m_cbSearches.FindStringExact(-1, sSearch) == CB_ERR);

	if (bNewSearch)
	{
		m_cbSearches.AddString(sSearch); // yes
		Misc::AddUniqueItem(sSearch, m_aSavedSearches);
	}
		
	// notify parent
	GetParent()->SendMessage((bNewSearch ? WM_FTD_ADDSEARCH : WM_FTD_SAVESEARCH), 0, (LPARAM)(LPCTSTR)sSearch);
}

BOOL CTDLFindTasksDlg::LoadSearch(LPCTSTR szName)
{
	CSearchParamArray params;
	BOOL bDone, bParents, bFilteredOut;

	if (LoadSearch(szName, params, bDone, bParents, bFilteredOut))
	{
		m_lcFindSetup.SetSearchParams(params);

		CheckIncludeOption(FI_COMPLETED, bDone);
		CheckIncludeOption(FI_PARENT, bParents);
		CheckIncludeOption(FI_FILTEREDOUT, bFilteredOut);

		return TRUE;
	}

	return FALSE;
}

BOOL CTDLFindTasksDlg::LoadSearch(LPCTSTR szName, CSearchParamArray& params, BOOL& bDone, BOOL& bParents, BOOL& bFilteredOut) const
{
	params.RemoveAll();

	if (Misc::IsEmpty(szName))
		return FALSE;

	CPreferences prefs;
	CString sKey = Misc::MakeKey(_T("FindTasks\\Searches\\%s"), szName);

	int nNumRules = prefs.GetProfileInt(sKey, _T("NumRules"), 0);
	SEARCHPARAM rule;

	for (int nRule = 0; nRule < nNumRules; nRule++)
	{
		CString sRule = Misc::MakeKey(_T("\\Rule%d"), nRule, sKey);
		
		// try active tasklist custom attributes
		if (!CTDCSearchParamHelper::LoadRule(prefs, sRule, m_aActiveTDCAttribDefs, rule))
		{
			// if that failed and it was not a custom ID then 
			// treat it as a dodgy rule
			if (!rule.IsCustomAttribute())
			{
				break; // invalid attribute
			}

			// retry with all available custom attributes
			CTDCSearchParamHelper::LoadRule(prefs, sRule, m_aAllTDCAttribDefs, rule);
		}
		
		params.Add(rule);
	}

	// load include settings
	bDone = prefs.GetProfileInt(sKey, _T("IncludeDoneTasks"), IncludeOptionIsChecked(FI_COMPLETED));
	bParents = prefs.GetProfileInt(sKey, _T("IncludeParentTasks"), IncludeOptionIsChecked(FI_PARENT));
	bFilteredOut = prefs.GetProfileInt(sKey, _T("IncludeFilteredOutTasks"), IncludeOptionIsChecked(FI_FILTEREDOUT));

	return (params.GetSize() > 0);
}

BOOL CTDLFindTasksDlg::SaveSearch(LPCTSTR szName)
{
	CPreferences prefs;
	CString sKey = Misc::MakeKey(_T("FindTasks\\Searches\\%s"), szName);

	// delete old search of this name
	prefs.DeleteProfileSection(sKey, TRUE);

	// save rules
	CSearchParamArray params;
	int nNumRules = m_lcFindSetup.GetSearchParams(params), nRule;

	for (nRule = 0; nRule < nNumRules; nRule++)
	{
		const SEARCHPARAM& rule = params[nRule];
		CString sRule = Misc::MakeKey(_T("\\Rule%d"), nRule, sKey);

		if (!CTDCSearchParamHelper::SaveRule(prefs, sRule, rule))
		{
			break;
		}
	}

	// write only the number of 'real' rules
	prefs.WriteProfileInt(sKey, _T("NumRules"), nRule);

	// save include settings
	prefs.WriteProfileInt(sKey, _T("IncludeDoneTasks"), IncludeOptionIsChecked(FI_COMPLETED));
	prefs.WriteProfileInt(sKey, _T("IncludeParentTasks"), IncludeOptionIsChecked(FI_PARENT));
	prefs.WriteProfileInt(sKey, _T("IncludeFilteredOutTasks"), IncludeOptionIsChecked(FI_FILTEREDOUT));

	return TRUE;
}

int CTDLFindTasksDlg::LoadSearches()
{
	CPreferences prefs;

	m_cbSearches.ResetContent();

	int nNumItems = prefs.GetProfileInt(_T("FindTasks\\Searches"), _T("NumSearches"), 0);

	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		CString sKey = Misc::MakeKey(_T("Search%d"), nItem);
		CString sSearch = prefs.GetProfileString(_T("FindTasks\\Searches"), sKey);

		// only add to the list if it can be loaded
		CSearchParamArray dummy;
		BOOL bDummy;

		if (LoadSearch(sSearch, dummy, bDummy, bDummy, bDummy))
		{
			m_aSavedSearches.Add(sSearch);
			m_cbSearches.AddString(sSearch);
		}
	}

	// restore last named search
	CString sSearch = prefs.GetProfileString(_T("FindTasks\\Searches"), _T("Current"));

	// backup
	if (!sSearch.IsEmpty() && m_cbSearches.FindStringExact(-1, sSearch) == CB_ERR)
	{
		if (m_cbSearches.GetCount())
			m_cbSearches.GetLBText(0, sSearch);
	}
	
	if (!sSearch.IsEmpty() && m_cbSearches.SelectString(-1, sSearch) != CB_ERR)
	{
		if (LoadSearch(sSearch))
			m_sActiveSearch = sSearch;
	}
	else
	{
		LoadSearch(_T("_last_search_"));
		m_sActiveSearch.Empty();
	}

	RefreshMaxDropWidth(m_cbSearches);

	return m_cbSearches.GetCount();
}

int CTDLFindTasksDlg::SaveSearches()
{
	CPreferences prefs;

	int nNumSearches = m_cbSearches.GetCount();
	prefs.WriteProfileInt(_T("FindTasks\\Searches"), _T("NumSearches"), nNumSearches);

	for (int nItem = 0; nItem < nNumSearches; nItem++)
	{
		CString sSearch, sKey = Misc::MakeKey(_T("Search%d"), nItem);
		m_cbSearches.GetLBText(nItem, sSearch);
		prefs.WriteProfileString(_T("FindTasks\\Searches"), sKey, sSearch);
	}

	// save last active named search
	prefs.WriteProfileString(_T("FindTasks\\Searches"), _T("Current"), m_sActiveSearch);

	// save _last_ search
	SaveSearch(_T("_last_search_"));

	return m_cbSearches.GetCount();
}

void CTDLFindTasksDlg::OnSelchangeSearchlist() 
{
	int nSel = m_cbSearches.GetCurSel();	

	if (nSel != CB_ERR)
	{
		CString sName;
		m_cbSearches.GetLBText(nSel, sName);

		if (LoadSearch(sName))
			m_sActiveSearch = sName;
	}

	m_toolbar.RefreshButtonStates(FALSE);
}

CString CTDLFindTasksDlg::GetCurrentSearch()
{
	CString sSearch;
	m_cbSearches.GetWindowText(sSearch);

	return sSearch;
}

void CTDLFindTasksDlg::OnNewSearch() 
{
	m_cbSearches.SetCurSel(-1); // clears the combo edit
	m_lcFindSetup.ClearSearch();
	m_sActiveSearch.Empty();

	m_toolbar.RefreshButtonStates();
}

void CTDLFindTasksDlg::OnDeleteSearch() 
{
	CString sDeletedSearch = GetCurrentSearch();

	if (!sDeletedSearch.IsEmpty())
	{
		// Remove deleted search
		int nSearch = m_cbSearches.FindStringExact(-1, sDeletedSearch);
		
		if (nSearch != CB_ERR)
			m_cbSearches.DeleteString(nSearch);

		Misc::RemoveItem(sDeletedSearch, m_aSavedSearches);
		
		m_lcFindSetup.ClearSearch();
		
		// set the selection to the next search
		if (nSearch >= m_cbSearches.GetCount())
			nSearch = m_cbSearches.GetCount() - 1;
		
		if (nSearch != CB_ERR)
		{
			CString sNextSearch;
			m_cbSearches.GetLBText(nSearch, sNextSearch);
			
			CSearchParamArray params;
			
			if (LoadSearch(sNextSearch))
			{
				m_sActiveSearch = sNextSearch;
			}
			else
			{
				nSearch = -1;
				m_sActiveSearch.Empty();
			}
		}
		
		m_cbSearches.SetCurSel(nSearch);
	}
	
	m_toolbar.RefreshButtonStates();

	// notify parent
	GetParent()->SendMessage(WM_FTD_DELETESEARCH, 0, (LPARAM)(LPCTSTR)sDeletedSearch);
}

void CTDLFindTasksDlg::OnEditchangeSearchlist() 
{
	m_toolbar.RefreshButtonStates();
}

void CTDLFindTasksDlg::OnUpdateDeleteSearch(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetCurrentSearch().IsEmpty());
}

void CTDLFindTasksDlg::OnUpdateSaveSearch(CCmdUI* pCmdUI) 
{
	// must have a name
	BOOL bEnable = !GetCurrentSearch().IsEmpty();

	// and be different from currently saved search
	// TODO

	pCmdUI->Enable(bEnable);
}

void CTDLFindTasksDlg::OnOK() 
{ 
	OnFind();
}

void CTDLFindTasksDlg::OnCancel() 
{ 
	if (m_lcFindSetup.IsEditing())
		m_lcFindSetup.CancelEdit();
	else
		ShowWindow(SW_HIDE); 
}

void CTDLFindTasksDlg::OnMoveRuleUp() 
{
	m_lcFindSetup.MoveSelectedRuleUp();
}

void CTDLFindTasksDlg::OnUpdateMoveRuleUp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_lcFindSetup.CanMoveSelectedRuleUp());
}

void CTDLFindTasksDlg::OnMoveRuleDown() 
{
	m_lcFindSetup.MoveSelectedRuleDown();
}

void CTDLFindTasksDlg::OnUpdateMoveRuleDown(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_lcFindSetup.CanMoveSelectedRuleDown());
}

BOOL CTDLFindTasksDlg::OnToolTipNotify(UINT /*id*/, NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
    // Get the tooltip structure.
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;

    UINT nID = pNMHDR->idFrom;

    // Check once again that the idFrom holds handle itself.
    if (pTTT->uFlags & TTF_IDISHWND)
        nID = ::GetDlgCtrlID((HWND)nID);

	static CString sTooltip;
	sTooltip.Empty();

	switch( nID )
	{
	case IDC_INCLUDE:
		sTooltip = m_cbInclude.GetTooltip();
		break;
	}
	
	if (!sTooltip.IsEmpty())
	{
		// Set the multiline tooltip text
		::SendMessage(pNMHDR->hwndFrom, TTM_SETMAXTIPWIDTH, 0, 300);
		pTTT->lpszText = (LPTSTR)(LPCTSTR)sTooltip;
		return TRUE;
	}

    // Not handled.
    return FALSE;
}

HBRUSH CTDLFindTasksDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if ((nCtlColor == CTLCOLOR_STATIC) && CThemed::IsAppThemed())
	{
		ASSERT (m_brBkgnd.GetSafeHandle());

		pDC->SetTextColor(m_theme.crAppText);
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_brBkgnd;
	}
	
	return hbr;
}

BOOL CTDLFindTasksDlg::OnEraseBkgnd(CDC* pDC) 
{
	BOOL bRes = FALSE;

	if (CThemed::IsAppThemed())
	{
		if (COSVersion() >= OSV_VISTA)
			ExcludeCtrls(this, pDC);

		CRect rClient;
		GetClientRect(rClient);
		pDC->FillSolidRect(rClient, m_theme.crAppBackLight);

		// draw a bevel below the toolbar
		CRect rToolbar;
		m_toolbar.GetWindowRect(rToolbar);
		ScreenToClient(rToolbar);

		pDC->FillSolidRect(0, rToolbar.bottom, rToolbar.Width(), 1, m_theme.crAppLinesDark);
		pDC->FillSolidRect(0, rToolbar.bottom + 1, rToolbar.Width(), 1, m_theme.crAppLinesLight);

		bRes = TRUE;
	}
	else
	{
		bRes = CDialog::OnEraseBkgnd(pDC);
	}
	
	GraphicsMisc::DrawSplitBar(pDC, GetSplitterRect(), m_theme.crAppBackDark, FALSE);

	return bRes;
}

void CTDLFindTasksDlg::OnApplyasfilter() 
{
	if (m_sActiveSearch.IsEmpty())
	{
		m_sActiveSearch = CEnString(IDS_UNNAMEDFILTER);
		m_cbSearches.SetWindowText(m_sActiveSearch);
	}

	OnSaveSearch();

	GetParent()->SendMessage(WM_FTD_APPLYASFILTER, 0, (LPARAM)(LPCTSTR)m_sActiveSearch);
}

void CTDLFindTasksDlg::OnSelchangeTasklistoptions() 
{
	UpdateData();

	// update list ctrl
	if (m_bAllTasklists)
	{
		m_lcFindSetup.SetCustomAttributes(m_aAllTDCAttribDefs);
		m_lcFindSetup.SetAttributeListData(m_tldAll, TDCA_ALL);
	}
	else
	{
		m_lcFindSetup.SetCustomAttributes(m_aActiveTDCAttribDefs);
		m_lcFindSetup.SetAttributeListData(m_tldActive, TDCA_ALL);
	}
}

void CTDLFindTasksDlg::OnSelchangeInclude() 
{
	// re-run the search once the combo closes
	if (!m_cbInclude.GetDroppedState())
		OnFind();
}

void CTDLFindTasksDlg::OnFindHelp() 
{
	AfxGetApp()->WinHelp(IDD_FINDTASKS_DIALOG);
}

BOOL CTDLFindTasksDlg::OnHelpInfo(HELPINFO* /*pHelpInfo*/)
{
	OnFindHelp();
	return TRUE;
}

CRect CTDLFindTasksDlg::GetSplitterRect() const
{
	CRect rRules = GetChildRect(&m_lcFindSetup), rSplitter(rRules);
	
	if (IsSplitterVertical())
	{
		rSplitter.left = rRules.right;
		rSplitter.right = (rSplitter.left + SPLITTER_WIDTH - 2);
	}
	else
	{
		rSplitter.top = rRules.bottom;
		rSplitter.bottom = (rSplitter.top + SPLITTER_WIDTH);
	}

	return rSplitter;
}

BOOL CTDLFindTasksDlg::GetSplitterRect(CRect& rSplitter, int nSplitPos) const
{
	CRect rClient;
	GetClientRect(rClient);

	rSplitter = GetSplitterRect();

	if (IsSplitterVertical())
	{
		// Check for available size
		if ((nSplitPos < MIN_LIST_SIZE) || (nSplitPos > (rClient.right - MIN_LIST_SIZE)))
			return FALSE;

		rSplitter.OffsetRect((nSplitPos - rSplitter.CenterPoint().x), 0);
	}
	else
	{
		// Check for available size
		CRect rRules = GetChildRect(&m_lcFindSetup);
		CRect rResults = GetChildRect(&m_lcResults);

		if (nSplitPos < (rRules.top + MIN_LIST_SIZE))
			return FALSE;

		if (nSplitPos > (rClient.bottom - MIN_LIST_SIZE - (rResults.top - rSplitter.bottom)))
			return FALSE;

		rSplitter.OffsetRect(0, (nSplitPos - rSplitter.CenterPoint().y));
	}
	
	return TRUE;
}

BOOL CTDLFindTasksDlg::IsSplitterVertical() const
{
	return (m_dockMgr.GetDockPos() == DMP_BELOW);
}

void CTDLFindTasksDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (GetSplitterRect().PtInRect(point))
	{
		m_bSplitting = TRUE;
		SetCapture();
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CTDLFindTasksDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	ResizeDlg(TRUE);

	CDialog::OnLButtonDown(nFlags, point);
}

void CTDLFindTasksDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	CDialog::OnLButtonUp(nFlags, point);
}

void CTDLFindTasksDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bSplitting)
		SetSplitterPos(IsSplitterVertical() ? point.x : point.y);
	
	CDialog::OnMouseMove(nFlags, point);
}

BOOL CTDLFindTasksDlg::SetSplitterPos(int nSplitPos)
{
	CRect rSplitter;

	if (!GetSplitterRect(rSplitter, nSplitPos))
		return FALSE;

	CRect rClient, rRules = GetChildRect(&m_lcFindSetup);
	GetClientRect(rClient);

	nSplitPos = (nSplitPos - (SPLITTER_WIDTH / 2));

	if (IsSplitterVertical())
		ResizeChild(&m_lcFindSetup, (nSplitPos - rRules.right), 0);
	else
		ResizeChild(&m_lcFindSetup, 0, (nSplitPos - rRules.bottom));

	ResizeDlg(FALSE);
	UpdateWindow();

	return TRUE;
}

BOOL CTDLFindTasksDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CPoint ptCursor(GetCurrentMessage()->pt);
	ScreenToClient(&ptCursor);

	if (GetSplitterRect().PtInRect(ptCursor))
	{ 
		UINT nIDCursor = ((m_dockMgr.GetDockPos() == DMP_BELOW) ? AFX_IDC_HSPLITBAR : AFX_IDC_VSPLITBAR);
		::SetCursor(AfxGetApp()->LoadCursor(nIDCursor));

		return TRUE;
	}
	
	// else
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CTDLFindTasksDlg::OnCaptureChanged(CWnd* pWnd)
{
	m_bSplitting = FALSE;

	CDialog::OnCaptureChanged(pWnd);
}