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

#include "..\3rdparty\XNamedColors.h"

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
const int UNDOCKED_BORDER	= GraphicsMisc::ScaleByDPIFactor(6);
const int DOCKED_BORDER		= GraphicsMisc::ScaleByDPIFactor(3);
const int MIN_LIST_SIZE		= GraphicsMisc::ScaleByDPIFactor(100);

const int DEF_BORDER_DLU	= 2;

const UINT WM_FTD_SELECTITEM = (WM_APP+1);

/////////////////////////////////////////////////////////////////////////////
// CTDLFindTasksDlg dialog

CTDLFindTasksDlg::CTDLFindTasksDlg()
	: 
	CRuntimeDlg(), 
	m_bSplitting(FALSE),
	m_bInitializing(FALSE),
	m_bAllTasklists(FALSE),
	m_nDockPos(DMP_UNDOCKED),
	m_sizeDocked(0, 0), 
	m_sizeDockedMax(0, 0),
	m_rUndocked(0, 0, 0, 0)

{
	m_sResultsLabel.LoadString(IDS_FTD_RESULTS);
	
	AddRCControl(_T("LTEXT"), _T(""), CEnString(IDS_FIND_RULES), 0, 0, 0, 49, 287, 8, IDC_RULESLABEL);
	AddRCControl(_T("CONTROL"), _T("SysListView32"), _T(""), LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED | LVS_NOCOLUMNHEADER | LVS_NOSORTHEADER | WS_TABSTOP, WS_EX_CLIENTEDGE, 0, 58, 370, 96, IDC_FINDLIST);
	AddRCControl(_T("PUSHBUTTON"), _T(""), CEnString(IDS_FIND_APPLYASFILTER), 0, 0, 0, 176, 65, 14, IDC_APPLYASFILTER);
	AddRCControl(_T("PUSHBUTTON"), _T(""), CEnString(IDS_FIND_SELECTALL), 0, 0, 72, 176, 50, 14, IDC_SELECTALL);
	AddRCControl(_T("LTEXT"), _T(""), CEnString(IDS_FIND_RESULTS), 0, 0, 133, 179, 240, 8, IDC_RESULTSLABEL);
	AddRCControl(_T("CONTROL"), _T("SysListView32"), _T(""), LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_REPORT | LVS_SHAREIMAGELISTS | WS_TABSTOP, WS_EX_CLIENTEDGE, 0, 191, 370, 94, IDC_RESULTS);
	AddRCControl(_T("COMBOBOX"), _T(""), _T(""), CBS_DROPDOWN | CBS_AUTOHSCROLL | CBS_SORT | WS_VSCROLL | WS_TABSTOP, 0, 85, 3, 71, 121, IDC_SEARCHLIST);
	AddRCControl(_T("LTEXT"), _T(""), CEnString(IDS_FIND_SEARCH), 0, 0, 0, 22, 120, 8, IDC_SEARCHLABEL);
	AddRCControl(_T("COMBOBOX"), _T(""), _T(""), CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP, 0, 0, 31, 120, 125, IDC_TASKLISTOPTIONS);
	AddRCControl(_T("LTEXT"), _T(""), CEnString(IDS_FIND_INCLUDING), 0, 0, 136, 22, 134, 8, IDC_INCLUDELABEL);
	AddRCControl(_T("COMBOBOX"), _T(""), _T(""), CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED | CBS_SORT | CBS_HASSTRINGS | WS_VSCROLL | WS_TABSTOP, 0, 136, 31, 120, 130, IDC_INCLUDE);
}

CTDLFindTasksDlg::~CTDLFindTasksDlg()
{
}

void CTDLFindTasksDlg::DoDataExchange(CDataExchange* pDX)
{
	CRuntimeDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLFindTasksDlg)
	DDX_Control(pDX, IDC_INCLUDE, m_cbInclude);
	DDX_Control(pDX, IDC_SEARCHLIST, m_cbSearches);
	DDX_Control(pDX, IDC_TASKLISTOPTIONS, m_cbTasklists);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_FINDLIST, m_lcFindSetup);
	DDX_Control(pDX, IDC_RESULTS, m_lcResults);
	DDX_CBIndex(pDX, IDC_TASKLISTOPTIONS, m_bAllTasklists);
	DDX_Text(pDX, IDC_RESULTSLABEL, m_sResultsLabel);
}


BEGIN_MESSAGE_MAP(CTDLFindTasksDlg, CRuntimeDlg)
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
	ON_COMMAND(ID_FIND_CLOSE, OnClose)
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
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_MOVE()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_CAPTURECHANGED()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLFindTasksDlg message handlers

BOOL CTDLFindTasksDlg::OnInitDialog() 
{
	CAutoFlag af(m_bInitializing, TRUE);

	CRuntimeDlg::OnInitDialog();

	if (GetStyle() & WS_POPUP)
		VERIFY(m_sbGrip.Initialize(this));

	// build 'Include' combo
	CLocalizer::EnableTranslation(m_cbInclude, FALSE);

	BuildOptionCombos();

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

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLFindTasksDlg::BuildOptionCombos()
{
	CDialogHelper::AddString(m_cbTasklists, IDS_FIND_ACTIVETASKLIST, FALSE);
	CDialogHelper::AddString(m_cbTasklists, IDS_FIND_ALLTASKLISTS, TRUE);
	
	CDialogHelper::AddString(m_cbInclude, IDS_FIND_INCLUDEDONE, FI_COMPLETED);
	CDialogHelper::AddString(m_cbInclude, IDS_FIND_INCLUDEPARENTS, FI_PARENT);
	CDialogHelper::AddString(m_cbInclude, IDS_FIND_INCLUDEFILTEREDOUT, FI_FILTEREDOUT);

	CLocalizer::EnableTranslation(m_cbInclude, FALSE);

	m_cbInclude.EnableTooltip();
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

		// Set colours first for the rescaling to work
		m_toolbar.SetHotColor(m_theme.crToolbarHot);
		m_toolbar.SetBackgroundColors(theme.crToolbarLight,
									  theme.crToolbarDark,
									  theme.nRenderStyle != UIRS_GLASS,
									  theme.nRenderStyle != UIRS_GRADIENT);

		// We have recreate the toolbar if rescaling is required
		if ((m_theme != themeCur) && GraphicsMisc::WantDPIScaling())
		{
			// change parent of combo so it doesn't get destroyed
			// will get reset in InitializeToolbar
			m_cbSearches.SetParent(this);

			m_toolbar.DestroyWindow();
			m_tbHelper.Release();

			VERIFY(InitializeToolbar());
		}

		m_toolbar.RefreshButtonStates(TRUE);

		ResizeDlg(FALSE);
		Invalidate();
	}
}

BOOL CTDLFindTasksDlg::InitializeToolbar()
{
	if (m_toolbar.GetSafeHwnd())
		return TRUE;

	if (!m_toolbar.CreateEx(this, (TBSTYLE_FLAT | TBSTYLE_WRAPABLE), WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP))
		return FALSE;

	if (CThemed::IsAppThemed())
	{
		m_toolbar.SetBackgroundColors(m_theme.crToolbarLight,
									  m_theme.crToolbarDark,
									  m_theme.nRenderStyle != UIRS_GLASS,
									  m_theme.nRenderStyle != UIRS_GRADIENT);
		m_toolbar.SetHotColor(m_theme.crToolbarHot);
	}

	if (!m_toolbar.LoadToolBar(IDR_FIND_TOOLBAR, IDB_FIND_TOOLBAR_STD, colorMagenta))
		return FALSE;

	if (!IsDocked())
	{
		// Delete close button and separator
		int nClose = m_toolbar.CommandToIndex(ID_FIND_CLOSE);

		m_toolbar.DeleteItem(nClose); // Close button
		m_toolbar.DeleteItem(nClose); // Separator
	}
	
	m_toolbar.SetBorders(4, 4, 0, 0);

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

	// clear default selection
	m_cbSearches.PostMessage(CB_SETEDITSEL, 0, MAKELPARAM(-1, 0));

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

BOOL CTDLFindTasksDlg::Create(DM_POS nPos)
{
	BOOL bWasDocked = IsDocked();
	BOOL bIsDocked = IsDocked(nPos);

	if (GetSafeHwnd())
	{
		if (nPos == m_nDockPos)
			return TRUE;

		// else
		DestroyWindow();
	}

	m_nDockPos = nPos;

	CRect rect(rectAuto);
	DWORD dwStyle = 0, dwExStyle = 0;

	if (bIsDocked)
	{
		// Parent will manage and draw a splitter rect
		// so collapse the border along the docked edge
		switch (m_nDockPos)
		{
		case DMP_LEFT:
			SetBordersDLU(DEF_BORDER_DLU, DEF_BORDER_DLU, 0, DEF_BORDER_DLU);
			break;

		case DMP_RIGHT:
			SetBordersDLU(0, DEF_BORDER_DLU, DEF_BORDER_DLU, DEF_BORDER_DLU);
			break;

		case DMP_BELOW:
			SetBordersDLU(DEF_BORDER_DLU, 0, DEF_BORDER_DLU, DEF_BORDER_DLU);
			break;
		}

		dwStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | DS_SETFONT;
		dwExStyle = WS_EX_CONTROLPARENT;
	}
	else
	{
		SetBordersDLU(DEF_BORDER_DLU);

		dwStyle = RTD_DEFSTYLE | WS_THICKFRAME;

		if (!m_rUndocked.IsRectEmpty())
		{
			rect = m_rUndocked;
			dwStyle &= ~DS_CENTER;
		}

		dwExStyle = RTD_DEFEXSTYLE;
	}

	return CRuntimeDlg::Create(CEnString(IDS_FIND_TASKS), dwStyle, dwExStyle, rect, AfxGetMainWnd(), IDC_STATIC);
}

BOOL CTDLFindTasksDlg::Create()
{
	CPreferences prefs;
	DM_POS nLastPos = prefs.GetProfileEnum(_T("FindTasks"), _T("DockPos"), DMP_UNDOCKED);

	DWORD dwTopLeft = (DWORD)prefs.GetProfileInt(_T("FindTasks"), _T("TopLeft"), -1);
	DWORD dwBottomRight = (DWORD)prefs.GetProfileInt(_T("FindTasks"), _T("BottomRight"), -1);

	m_rUndocked.SetRectEmpty();

	if ((dwBottomRight != -1) && (dwTopLeft != -1))
	{
		CRect rect;
		rect.SetRect(CPoint(dwTopLeft), CPoint(dwBottomRight));

		// ensure this intersects with the desktop by a decent amount
		const int BORDER = 200;
		rect.DeflateRect(BORDER, BORDER);

		if (NULL != MonitorFromRect(rect, MONITOR_DEFAULTTONULL))
		{
			rect.InflateRect(BORDER, BORDER);
			m_rUndocked = rect;
		}
	}

	return Create(nLastPos);
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

	int nMinWidth = GetMinSize(DMP_LEFT).cx;
	int nMinHeight = GetMinSize(DMP_BELOW).cy;

	m_sizeDocked.cx = prefs.GetProfileInt(_T("FindTasks"), _T("DockedWidth"), nMinWidth);
	m_sizeDockedMax.cx = prefs.GetProfileInt(_T("FindTasks"), _T("DockedWidthMax"), nMinWidth);

	m_sizeDocked.cy = prefs.GetProfileInt(_T("FindTasks"), _T("DockedHeight"), nMinHeight);
	m_sizeDockedMax.cy = prefs.GetProfileInt(_T("FindTasks"), _T("DockedHeightMax"), nMinHeight);

	// Backward compatibility
	if ((m_sizeDocked.cx <= 0) ||
		(m_sizeDockedMax.cx <= 0) ||
		(m_sizeDocked.cy <= 0) ||
		(m_sizeDockedMax.cy <= 0))
	{
		m_sizeDocked.cx = m_sizeDockedMax.cx = nMinWidth;
		m_sizeDocked.cy = m_sizeDockedMax.cy = nMinHeight;
	}

	// Splitter
	int nSplitPos = prefs.GetProfileInt(_T("FindTasks"), _T("SplitterPos"), -1);

	if (nSplitPos != -1)
		SetSplitterPos(nSplitPos);

	LoadSearches();

	UpdateData(FALSE);
}

CSize CTDLFindTasksDlg::GetMinSize(DM_POS nPos)
{
	CRect rMin(0, 0, 0, 0), rCtrl;

	if (nPos == DMP_BELOW)
	{
		// we need enough height to show a few results and enough width
		// to display the results list
		rCtrl = GetCtrlRect(IDC_TASKLISTOPTIONS);
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
			{
				m_lcFindSetup.SetCurSel(0, 0);
			}
			else if (m_lcFindSetup.GetItemText(0, 1).IsEmpty())
			{
				m_lcFindSetup.SetCurSel(0, 1);
			}
			else 
			{
				m_lcFindSetup.SetCurSel(0, 2);
			}

			m_lcFindSetup.SetFocus();
		}
		
	    m_toolbar.RefreshButtonStates();

		ShowWindow(SW_SHOW);
		SetForegroundWindow(); // give it the focus
	}
	else
	{
		ShowWindow(SW_HIDE);
	}
	
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

void CTDLFindTasksDlg::AddResults(const CFilteredToDoCtrl* pTDC, const CResultArray& aResults, BOOL bShowValueOnly, LPCTSTR szHeaderText)
{
	if (!GetSafeHwnd())
	{
		ASSERT(0);
		return;
	}

	if (!Misc::IsEmpty(szHeaderText))
		m_lcResults.AddHeaderRow(szHeaderText);

	int nOrgItemCount = m_lcResults.GetItemCount();

	for (int nResult = 0; nResult < aResults.GetSize(); nResult++)
	{
		const SEARCHRESULT& result = aResults[nResult];

		// Don't add what the user doesn't want to see
		// Unless the current rule set includes TDCA_DONEDATE
		if (result.HasFlag(RF_DONE) || result.HasFlag(RF_GOODASDONE))
		{
			if (!m_lcFindSetup.HasRule(TDCA_DONEDATE) && !IncludeOptionIsChecked(FI_COMPLETED))
			{
				continue;
			}
		}

		if (result.HasFlag(RF_PARENT) && !IncludeOptionIsChecked(FI_PARENT))
		{
			continue;
		}

		m_lcResults.AddResult(result, pTDC, bShowValueOnly);
	}

	if (m_lcResults.GetItemCount() > nOrgItemCount)
	{
		// update 'found' count
		m_sResultsLabel.Format(IDS_FTD_SOMERESULTS, GetResultCount());
		UpdateData(FALSE);

		// focus first item added
		if (!GetDlgItem(IDC_SELECTALL)->IsWindowEnabled())
		{
			SelectItem(0);
			m_lcResults.SetFocus();

			// update 'search results' button' state
			m_toolbar.RefreshButtonStates();

			// enable 'select all' button
			GetDlgItem(IDC_SELECTALL)->EnableWindow(TRUE);
		}
	}
}

BOOL CTDLFindTasksDlg::GetSearchAllTasklists()
{
	if (!m_bInitializing && GetSafeHwnd())
		UpdateData();
	
	return m_bAllTasklists;
}

BOOL CTDLFindTasksDlg::GetSearchIncludesCompletedTasks(LPCTSTR szName) const
{
	if (m_sActiveSearch.CompareNoCase(szName) == 0)
		return IncludeOptionIsChecked(FI_COMPLETED);

	// else
	CSearchParamArray aUnused;
	BOOL bWantDone, bUnused;

	VERIFY(LoadSearch(szName, aUnused, bWantDone, bUnused, bUnused));

	return bWantDone;
}

BOOL CTDLFindTasksDlg::SetSearchIncludesCompletedTasks(LPCTSTR szName, BOOL bIncDone)
{
	if (GetSafeHwnd())
	{
		if (Misc::Find(szName, m_aSavedSearches, FALSE, TRUE) == -1)
		{
			ASSERT(0);
			return FALSE;
		}

		if (m_sActiveSearch.CompareNoCase(szName) == 0)
			CheckIncludeOption(FI_COMPLETED, bIncDone);
	}

	CString sKey = Misc::MakeKey(_T("FindTasks\\Searches\\%s"), szName);
	CPreferences().WriteProfileInt(sKey, _T("IncludeDoneTasks"), bIncDone);

	return TRUE;
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
	CRuntimeDlg::OnClose();

	// end any current edit
	m_lcFindSetup.EndEdit();
	
	// hide
	ShowWindow(SW_HIDE);
	
	// notify parent
	GetParent()->SendMessage(WM_FTD_CLOSE);
}

void CTDLFindTasksDlg::OnSize(UINT nType, int cx, int cy) 
{
	CRuntimeDlg::OnSize(nType, cx, cy);

	// end any current edit
	if (m_lcFindSetup.GetSafeHwnd())
		m_lcFindSetup.EndEdit();
	
	ResizeDlg(FALSE, cx, cy);

	if (m_toolbar.GetSafeHwnd())
		m_toolbar.RefreshButtonStates();

	if (IsDocked())
	{
		if (AfxGetMainWnd()->IsZoomed())
		{
			if (m_nDockPos == DMP_BELOW)
				m_sizeDockedMax.cy = cy;
			else
				m_sizeDockedMax.cx = cx;
		}
		else
		{
			if (m_nDockPos == DMP_BELOW)
				m_sizeDocked.cy = cy;
			else
				m_sizeDocked.cx = cx;
		}
	}
	else
	{
		m_rUndocked.right = (m_rUndocked.left + cx);
		m_rUndocked.bottom = (m_rUndocked.top + cy);
	}
}

void CTDLFindTasksDlg::OnMove(int x, int y)
{
	if (!IsDocked())
		GetWindowRect(m_rUndocked);

	CRuntimeDlg::OnMove(x, y);
}

int CTDLFindTasksDlg::GetDockedDimension(BOOL bMaximized) const
{
	return GetDockedDimension(m_nDockPos, bMaximized);
}

int CTDLFindTasksDlg::GetDockedDimension(DM_POS nPos, BOOL bMaximized) const
{
	switch (nPos)
	{
	case DMP_LEFT:
	case DMP_RIGHT:
		return (bMaximized ? m_sizeDockedMax.cx : m_sizeDocked.cx);

	case DMP_BELOW:
		return (bMaximized ? m_sizeDockedMax.cy : m_sizeDocked.cy);
	}

	ASSERT(0);
	return 0;
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

		CRect rSearchCombo = GetCtrlRect(IDC_TASKLISTOPTIONS);
		CRect rSearchLabel = GetCtrlRect(IDC_SEARCHLABEL);
		CRect rIncludeCombo = GetCtrlRect(IDC_INCLUDE);
		CRect rIncludeLabel = GetCtrlRect(IDC_INCLUDELABEL);

		CRect rRulesLabel = GetCtrlRect(IDC_RULESLABEL);
		CRect rRules = GetCtrlRect(IDC_FINDLIST);
		CRect rResults = GetCtrlRect(IDC_RESULTS);
		CRect rApply = GetCtrlRect(IDC_APPLYASFILTER);
		
		// Toolbar - prevent the combo from being moved
		int nMinTBLen = m_toolbar.GetMinReqLength(m_toolbar.CommandToIndex(ID_FIND_NEWSEARCH) - 1);
		int nTBHeight = m_toolbar.Resize(max(cx, nMinTBLen));

		// Embedded combo
		CRect rSearches;
		m_toolbar.GetItemRect(m_toolbar.CommandToIndex(ID_SEARCHLIST), rSearches);

		rSearches.bottom = rSearches.top + 200;
		m_cbSearches.MoveWindow(rSearches);

		{
			CDeferWndMove dwm(10);
			int nXOffset = 0, nYOffset = 0;

			// Tasklist combo and label
			nYOffset = nTBHeight + 10 - rSearchLabel.top;

			rSearchCombo = dwm.OffsetCtrl(this, IDC_TASKLISTOPTIONS, 0, nYOffset);
			rSearchLabel = dwm.OffsetCtrl(this, IDC_SEARCHLABEL, 0, nYOffset);

			// Options combo and label
			if ((m_nDockPos == DMP_BELOW) || ((rSearchCombo.Width() + rIncludeCombo.Width()) <= cx))
			{
				// Place side by side
				nXOffset = (rSearchCombo.right + 10 - rIncludeCombo.left);
				nYOffset = (rSearchLabel.top - rIncludeLabel.top);
			}
			else
			{
				// Place one combo below the other
				nXOffset = rSearchCombo.left - rIncludeCombo.left;
				nYOffset = (rSearchCombo.bottom + 6 - rIncludeLabel.top);
			}

			rIncludeCombo = dwm.OffsetCtrl(this, IDC_INCLUDE, nXOffset, nYOffset);
			dwm.OffsetCtrl(this, IDC_INCLUDELABEL, nXOffset, nYOffset);

			// Rules list and label
			rRulesLabel = dwm.OffsetCtrl(this, IDC_RULESLABEL, 0, rIncludeCombo.bottom + 10 - rRulesLabel.top);
			rRules.top = rRulesLabel.bottom + 2;

			if (bOrientationChange)
			{
				// Place the splitter so that the two lists have equal height/width
				int nSplitPos = 0;

				if (bVertSplitter)
					nSplitPos = (cx / 2);
				else
					nSplitPos = ((cy - rRules.top - (rResults.top - rApply.top)) / 2);

				VERIFY(GetSplitterRect(rSplitter, nSplitPos));
			}
			
			if (bVertSplitter)
			{
				rRules.right = rSplitter.left;	
				rRules.bottom = (cy - m_rBorders.bottom);
			}
			else
			{
				rRules.right = (cx - m_rBorders.right);
				rRules.bottom = rSplitter.top;	
			}
			
			dwm.MoveWindow(&m_lcFindSetup, rRules);

			// Results controls
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
			rResults.right = (cx - m_rBorders.right);
			rResults.bottom = (cy - m_rBorders.bottom);

			dwm.MoveWindow(&m_lcResults, rResults);
		}
		
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
	if (!IsDocked())
	{
		CPoint pt(::GetMessagePos());
		m_lcResults.ScreenToClient(&pt);
	
		if (m_lcResults.HitTest(pt) != -1)
			ShowWindow(SW_HIDE);
	}
	*pResult = 0;
}

void CTDLFindTasksDlg::OnDestroy() 
{
	SaveSettings();
	DeleteAllResults();

	m_tbHelper.Release();
	
	CRuntimeDlg::OnDestroy();
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
	prefs.WriteProfileInt(_T("FindTasks"), _T("TopLeft"), MAKELPARAM(m_rUndocked.left, m_rUndocked.top));
	prefs.WriteProfileInt(_T("FindTasks"), _T("BottomRight"), MAKELPARAM(m_rUndocked.right, m_rUndocked.bottom));
 	prefs.WriteProfileInt(_T("FindTasks"), _T("DockPos"), m_nDockPos);
	prefs.WriteProfileInt(_T("FindTasks"), _T("DockedWidth"), m_sizeDocked.cx);
	prefs.WriteProfileInt(_T("FindTasks"), _T("DockedWidthMax"), m_sizeDockedMax.cx);
	prefs.WriteProfileInt(_T("FindTasks"), _T("DockedHeight"), m_sizeDocked.cy);
	prefs.WriteProfileInt(_T("FindTasks"), _T("DockedHeightMax"), m_sizeDockedMax.cy);

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
	CRuntimeDlg::OnGetMinMaxInfo(lpMMI);
	
	if (m_lcFindSetup.GetSafeHwnd() && !IsDocked())
	{
		CSize rDef = GetMinSize(m_nDockPos);

		lpMMI->ptMinTrackSize.y = rDef.cy;
		lpMMI->ptMinTrackSize.x = rDef.cx;
	}
}

BOOL CTDLFindTasksDlg::PreTranslateMessage(MSG* pMsg) 
{
	// handle enter key if results list has the focus
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			if (pMsg->hwnd == m_lcResults)
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
			else if (pMsg->hwnd == m_lcFindSetup)
			{
				OnFind();
				return TRUE;
			}
			else if (IsChildOrSame(m_cbSearches, pMsg->hwnd))
			{
				if (m_cbSearches.GetWindowTextLength())
					OnSaveSearch();

				return TRUE;
			}
			break;

		case VK_TAB:
			{
				// Incorporate the toolbar save combo in the tab order
				BOOL bReverse = Misc::IsKeyPressed(VK_SHIFT);

				CWnd* pFocus = GetFocus();
				CWnd* pNextFocus = GetNextDlgTabItem(pFocus, bReverse);
				CWnd* pFirstCtrl = GetDlgItem(IDC_TASKLISTOPTIONS);
				CWnd* pLastCtrl = GetNextDlgTabItem(pFirstCtrl, TRUE);

				if (m_cbSearches.IsChild(pFocus))
				{
					pNextFocus = (bReverse ? pLastCtrl : pFirstCtrl);
				}
				else if ((bReverse && (pNextFocus == pLastCtrl)) || 
						(!bReverse && (pNextFocus == pFirstCtrl)))
				{
					pNextFocus = &m_cbSearches;
				}

				if (pNextFocus)
				{
					// skip over empty results list
					if ((pNextFocus == &m_lcResults) && (m_lcResults.GetItemCount() == 0))
					{
						if (bReverse)
							pNextFocus = GetNextDlgTabItem(pNextFocus, TRUE);
						else
							pNextFocus = &m_cbSearches;
					}

					SendMessage(WM_NEXTDLGCTL, (WPARAM)pNextFocus->GetSafeHwnd(), TRUE);
					return TRUE;
				}
			}
			break;
		}
	}
	
	return CRuntimeDlg::PreTranslateMessage(pMsg);
}

void CTDLFindTasksDlg::OnSelectall() 
{
	GetParent()->SendMessage(WM_FTD_SELECTALL);
}

void CTDLFindTasksDlg::OnChangeDock(DM_POS nNewPos)
{
	BOOL bHadResults = m_lcResults.GetItemCount();
	DM_POS nOldPos = m_nDockPos;

	if (Create(nNewPos))
	{
		if (bHadResults)
			RefreshSearch();

		AfxGetMainWnd()->SendMessage(WM_FTD_DOCKCHANGE, nOldPos, nNewPos);
		m_toolbar.RefreshButtonStates();
	}
}

void CTDLFindTasksDlg::OnDockright() 
{
	OnChangeDock(DMP_RIGHT);
}

void CTDLFindTasksDlg::OnUpdateDockright(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(m_nDockPos == DMP_RIGHT);
}

void CTDLFindTasksDlg::OnDockleft() 
{
	OnChangeDock(DMP_LEFT);
}

void CTDLFindTasksDlg::OnUpdateDockleft(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
 	pCmdUI->SetRadio(m_nDockPos == DMP_LEFT);
}

void CTDLFindTasksDlg::OnUndock() 
{
	OnChangeDock(DMP_UNDOCKED);
}

void CTDLFindTasksDlg::OnUpdateUndock(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetRadio(m_nDockPos == DMP_UNDOCKED);
}

void CTDLFindTasksDlg::OnDockbelow() 
{
	OnChangeDock(DMP_BELOW);
}

void CTDLFindTasksDlg::OnUpdateDockbelow(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetRadio(m_nDockPos == DMP_BELOW);
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

int CTDLFindTasksDlg::GetSavedSearches(CStringArray& aNames) const
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

// Real handler
void CTDLFindTasksDlg::OnSaveSearch() 
{
	OnSaveSearch(TRUE); // Notify parent
}

// Pseudo-handler
void CTDLFindTasksDlg::OnSaveSearch(BOOL bNotifyParent)
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
	if (bNotifyParent)
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

	if (!m_sActiveSearch.IsEmpty())
		SaveSearch(m_sActiveSearch);
	else
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
	// Should never get here
	ASSERT(0);
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

HBRUSH CTDLFindTasksDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CRuntimeDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if ((nCtlColor == CTLCOLOR_STATIC) && CThemed::IsAppThemed() && m_brBkgnd.GetSafeHandle())
	{
		pDC->SetTextColor(m_theme.crAppText);
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_brBkgnd;
	}
	
	return hbr;
}

BOOL CTDLFindTasksDlg::OnEraseBkgnd(CDC* pDC) 
{
	BOOL bRes = FALSE;

	if (CThemed::IsAppThemed() && m_brBkgnd.GetSafeHandle())
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
		bRes = CRuntimeDlg::OnEraseBkgnd(pDC);
	}
	
	GraphicsMisc::DrawSplitBar(pDC, GetSplitterRect(), m_theme.crAppBackDark, FALSE);

	return bRes;
}

void CTDLFindTasksDlg::OnApplyasfilter() 
{
	CString sSearch;
	m_cbSearches.GetWindowText(sSearch);

	if (sSearch.IsEmpty())
		m_cbSearches.SetWindowText(CEnString(IDS_UNNAMEDFILTER));

	OnSaveSearch(FALSE);	// Don't notify parent 
	
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
	return (m_nDockPos == DMP_BELOW);
}

void CTDLFindTasksDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (GetSplitterRect().PtInRect(point))
	{
		m_bSplitting = TRUE;
		SetCapture();
	}

	CRuntimeDlg::OnLButtonDown(nFlags, point);
}

void CTDLFindTasksDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	ResizeDlg(TRUE);

	CRuntimeDlg::OnLButtonDown(nFlags, point);
}

void CTDLFindTasksDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	CRuntimeDlg::OnLButtonUp(nFlags, point);
}

void CTDLFindTasksDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bSplitting)
		SetSplitterPos(IsSplitterVertical() ? point.x : point.y);
	
	CRuntimeDlg::OnMouseMove(nFlags, point);
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
		UINT nIDCursor = ((m_nDockPos == DMP_BELOW) ? AFX_IDC_HSPLITBAR : AFX_IDC_VSPLITBAR);
		::SetCursor(AfxGetApp()->LoadCursor(nIDCursor));

		return TRUE;
	}
	
	// else
	return CRuntimeDlg::OnSetCursor(pWnd, nHitTest, message);
}

void CTDLFindTasksDlg::OnCaptureChanged(CWnd* pWnd)
{
	m_bSplitting = FALSE;

	CRuntimeDlg::OnCaptureChanged(pWnd);
}