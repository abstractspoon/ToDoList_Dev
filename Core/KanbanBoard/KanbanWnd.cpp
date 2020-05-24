// KanbanWnd.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "KanbanBoard.h"
#include "KanbanWnd.h"
#include "KanbanMsg.h"

#include "..\shared\misc.h"
#include "..\shared\filemisc.h"
#include "..\shared\themed.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\dlgunits.h"
#include "..\shared\datehelper.h"
#include "..\shared\localizer.h"
#include "..\shared\autoflag.h"
#include "..\shared\holdredraw.h"

#include "..\3rdparty\T64Utils.h"
#include "..\3rdparty\dibdata.h"
#include "..\3rdparty\GdiPlus.h"

#include "..\Interfaces\ipreferences.h"
#include "..\Interfaces\IUIExtension.h"

#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const COLORREF DEF_DONECOLOR	= RGB(128, 128, 128);

/////////////////////////////////////////////////////////////////////////////
// CKanbanWnd

CKanbanWnd::CKanbanWnd(CWnd* pParent /*=NULL*/)
	: 
	CDialog(IDD_KANBANTREE_DIALOG, pParent), 
	m_bReadOnly(FALSE),
	m_nTrackedAttrib(TDCA_NONE),
	m_ctrlKanban(),
	m_dwSelTaskID(0),
#pragma warning(disable:4355)
	m_dlgPrefs(this)
#pragma warning(default:4355)
{
	m_icon.Load(IDR_KANBAN);
}

CKanbanWnd::~CKanbanWnd()
{
}

void CKanbanWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKanbanWnd)
	DDX_Control(pDX, IDC_OPTIONS, m_cbOptions);
	DDX_Control(pDX, IDC_CUSTOMATTRIB, m_cbCustomAttributes);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ATTRIBUTES, m_cbAttributes);

	m_cbAttributes.DDX(pDX, m_nTrackedAttrib);
	m_cbCustomAttributes.DDX(pDX, m_sTrackedCustomAttribID);
}

BEGIN_MESSAGE_MAP(CKanbanWnd, CDialog)
	//{{AFX_MSG_MAP(CKanbanWnd)
	ON_CBN_SELCHANGE(IDC_ATTRIBUTES, OnSelchangeTrackedAttribute)
	ON_CBN_SELCHANGE(IDC_CUSTOMATTRIB, OnSelchangeTrackedAttribute)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_KANBAN_PREFS, OnKanbanPreferences)
	ON_UPDATE_COMMAND_UI(ID_KANBAN_PREFS, OnUpdateKanbanPreferences)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_OPTIONS, OnSelchangeOptions)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, OnHelp)
	ON_WM_HELPINFO()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_GETFONT, OnGetFont)
	ON_REGISTERED_MESSAGE(WM_KBC_VALUECHANGE, OnKanbanNotifyValueChange)
	ON_REGISTERED_MESSAGE(WM_KBC_EDITTASKDONE, OnKanbanNotifyEditTaskDone)
	ON_REGISTERED_MESSAGE(WM_KBC_EDITTASKFLAG, OnKanbanNotifyEditTaskFlag)
	ON_REGISTERED_MESSAGE(WM_KBC_SELECTIONCHANGE, OnKanbanNotifySelectionChange)
	ON_REGISTERED_MESSAGE(WM_KBC_PREFSHELP, OnKanbanPrefsHelp)
	ON_REGISTERED_MESSAGE(WM_KBC_GETTASKICON, OnKanbanNotifyGetTaskIcon)
	ON_REGISTERED_MESSAGE(WM_KBC_EDITTASKTITLE, OnKanbanNotifyEditTaskTitle)
	ON_REGISTERED_MESSAGE(WM_KBC_EDITTASKICON, OnKanbanNotifyEditTaskIcon)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnToolTipNotify)
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKanbanWnd message handlers

void CKanbanWnd::OnNcDestroy()
{
	CDialog::OnNcDestroy();

	delete this;
}

LRESULT CKanbanWnd::OnGetFont(WPARAM /*wp*/, LPARAM /*lp*/)
{
	return m_ctrlKanban.SendMessage(WM_GETFONT);
}

void CKanbanWnd::OnHelp()
{
	GetParent()->SendMessage(WM_IUI_DOHELP, 0, (LPARAM)GetTypeID());
}

BOOL CKanbanWnd::OnHelpInfo(HELPINFO* /*lpHelpInfo*/)
{
	OnHelp();
	return TRUE;
}

LRESULT CKanbanWnd::OnKanbanPrefsHelp(WPARAM /*wp*/, LPARAM /*lp*/)
{
	CString sHelpID(GetTypeID());
	sHelpID += _T("_PREFS");

	GetParent()->SendMessage(WM_IUI_DOHELP, 0, (LPARAM)(LPCTSTR)sHelpID);
	return 0L;
}

LRESULT CKanbanWnd::OnKanbanNotifyGetTaskIcon(WPARAM wp, LPARAM lp)
{
	return GetParent()->SendMessage(WM_IUI_GETTASKICON, wp, lp);
}

LRESULT CKanbanWnd::OnKanbanNotifyEditTaskTitle(WPARAM wp, LPARAM lp)
{
	return GetParent()->SendMessage(WM_IUI_EDITSELECTEDTASKTITLE, wp, lp);
}

LRESULT CKanbanWnd::OnKanbanNotifyEditTaskIcon(WPARAM wp, LPARAM lp)
{
	return GetParent()->SendMessage(WM_IUI_EDITSELECTEDTASKICON, wp, lp);
}

BOOL CKanbanWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// non-translatables
	CLocalizer::EnableTranslation(*GetDlgItem(IDC_SELECTEDTASKDATES), FALSE);

	// create toolbar
	if (m_toolbar.CreateEx(this))
	{
		const COLORREF MAGENTA = RGB(255, 0, 255);
		
		VERIFY(m_toolbar.LoadToolBar(IDR_TOOLBAR, IDB_TOOLBAR_STD, MAGENTA));
		VERIFY(m_tbHelper.Initialize(&m_toolbar, this));
		
		CRect rToolbar = CDialogHelper::GetCtrlRect(this, IDC_TB_PLACEHOLDER);
		m_toolbar.Resize(rToolbar.Width(), rToolbar.TopLeft());
		m_toolbar.RefreshButtonStates(TRUE);
	}

	m_ctrlKanban.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0, 0, 0, 0), this, 101);
	m_ctrlKanban.SetFocus();

	if (m_tooltips.Create(this, (WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP)))
		m_tooltips.AddTool(&m_cbOptions);
	
	ModifyStyleEx(0, WS_EX_CONTROLPARENT, 0);
	Resize();

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKanbanWnd::SetReadOnly(bool bReadOnly)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_bReadOnly = bReadOnly;
	m_ctrlKanban.SetReadOnly(bReadOnly);
}

BOOL CKanbanWnd::Create(DWORD dwStyle, const RECT &/*rect*/, CWnd* pParentWnd, UINT nID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (CDialog::Create(IDD_KANBAN_DIALOG, pParentWnd))
	{
		SetWindowLong(*this, GWL_STYLE, dwStyle);
		SetDlgCtrlID(nID);

		return TRUE;
	}

	return FALSE;
}

void CKanbanWnd::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CString sKey(szKey);

	// Last tracked attribute
	pPrefs->WriteProfileInt(szKey, _T("LastTrackedAttribute"), m_nTrackedAttrib);

	if (m_nTrackedAttrib == TDCA_CUSTOMATTRIB)
		pPrefs->WriteProfileString(sKey, _T("CustomAttrib"), m_sTrackedCustomAttribID);
	else
		pPrefs->DeleteProfileEntry(sKey, _T("CustomAttrib"));

	// Options
	pPrefs->WriteProfileInt(sKey, _T("ShowParents"), m_ctrlKanban.HasOption(KBCF_SHOWPARENTTASKS));
	pPrefs->WriteProfileInt(sKey, _T("ShowEmptyColumns"), m_ctrlKanban.HasOption(KBCF_SHOWEMPTYCOLUMNS));

	// Preferences
	m_dlgPrefs.SavePreferences(pPrefs, sKey);
}

/*
void CKanbanWnd::SaveColumnOrder(IPreferences* pPrefs, LPCTSTR szKey, const CIntArray& aStates) const
{
	int nCol = aStates.GetSize();
	pPrefs->WriteProfileInt(szKey, _T("Count"), nCol);

	while (nCol--)
	{
		CString sItemKey;
		sItemKey.Format(_T("Item%d"), nCol);
		pPrefs->WriteProfileInt(szKey, sItemKey, aStates[nCol]);
	}
}

int CKanbanWnd::LoadColumnOrder(const IPreferences* pPrefs, LPCTSTR szKey, CIntArray& aStates) const
{
	int nCol = pPrefs->GetProfileInt(szKey, _T("Count"), 0);

	if (!nCol)
		return 0;

	aStates.SetSize(nCol);
	
	while (nCol--)
	{
		CString sItemKey;
		sItemKey.Format(_T("Item%d"), nCol);
		aStates[nCol] = pPrefs->GetProfileInt(szKey, sItemKey);
	}

	return aStates.GetSize();
}
*/

void CKanbanWnd::UpdatePriorityColors(const IPreferences* pPrefs)
{
	CDWordArray aPriorityColors;

	if (pPrefs->GetProfileInt(_T("Preferences"), _T("ColorPriority")))
	{
		const LPCTSTR COLORKEY = _T("Preferences\\Colors");

		switch (pPrefs->GetProfileInt(_T("Preferences"), _T("PriorityColorOption"), 0))
		{
		case 0:	// Individual colours
			if (pPrefs->GetProfileInt(COLORKEY, _T("P0"), -1) != -1)
			{
				for (int nColor = 0; nColor < 11; nColor++)
				{
					CString sKey = Misc::MakeKey(_T("P%d"), nColor);
					aPriorityColors.Add(pPrefs->GetProfileInt(COLORKEY, sKey));
				}
			}
			break;

		case 1:	// Colour range
			{
				COLORREF crFrom = pPrefs->GetProfileInt(COLORKEY, _T("Low"));
				COLORREF crTo = pPrefs->GetProfileInt(COLORKEY, _T("High"));
				BOOL bRGB = !pPrefs->GetProfileInt(_T("Preferences"), _T("HLSColorGradient"));

				GraphicsMisc::CalculateColorGradient(crFrom, crTo, 11, aPriorityColors, bRGB);
			}
			break;

		case 2:	// Scheme colours
			if (pPrefs->GetProfileInt(COLORKEY, _T("S0"), -1) != -1)
			{
				for (int nColor = 0; nColor < 11; nColor++)
				{
					CString sKey = Misc::MakeKey(_T("S%d"), nColor);
					aPriorityColors.Add(pPrefs->GetProfileInt(COLORKEY, sKey));
				}
			}
			break;
		}
	}
	else // grayscale
	{ 
		GraphicsMisc::CalculateColorGradient(RGB(240, 240, 240), 0, 11, aPriorityColors, TRUE);
	}

	m_ctrlKanban.SetPriorityColors(aPriorityColors);
}

void CKanbanWnd::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey, bool bAppOnly) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// application preferences
	UpdatePriorityColors(pPrefs);

	DWORD dwOptions = m_ctrlKanban.GetOptions();

	Misc::SetFlag(dwOptions, KBCF_STRIKETHRUDONETASKS, pPrefs->GetProfileInt(_T("Preferences"), _T("StrikethroughDone"), TRUE));
	Misc::SetFlag(dwOptions, KBCF_TASKTEXTCOLORISBKGND, pPrefs->GetProfileInt(_T("Preferences"), _T("ColorTaskBackground"), FALSE));
	Misc::SetFlag(dwOptions, KBCF_SHOWCOMPLETIONCHECKBOXES, pPrefs->GetProfileInt(_T("Preferences"), _T("AllowCheckboxAgainstTreeItem"), TRUE));
	Misc::SetFlag(dwOptions, KBCF_SHOWLABELTIPS, !pPrefs->GetProfileInt(_T("Preferences"), _T("ShowInfoTips"), FALSE));

	m_ctrlKanban.SetOptions(dwOptions);
	
	// Default attribute values
	m_ctrlKanban.LoadDefaultAttributeListValues(pPrefs);

	// Kanban specific options
	if (!bAppOnly)
	{
		DWORD dwComboOptions = 0;

		Misc::SetFlag(dwComboOptions, KBCF_SHOWPARENTTASKS, pPrefs->GetProfileInt(szKey, _T("ShowParents"), TRUE));
		Misc::SetFlag(dwComboOptions, KBCF_SHOWEMPTYCOLUMNS, pPrefs->GetProfileInt(szKey, _T("ShowEmptyColumns"), TRUE));
		
		m_cbOptions.SetSelectedOptions(dwComboOptions);
		OnSelchangeOptions();
		
		// Last tracked attribute
		m_dlgPrefs.LoadPreferences(pPrefs, szKey);

		m_nTrackedAttrib = (TDC_ATTRIBUTE)pPrefs->GetProfileInt(szKey, _T("LastTrackedAttribute"), TDCA_STATUS);

		switch (m_nTrackedAttrib)
		{
		// backwards compatibility
		case TDCA_NONE:
			if (m_dlgPrefs.HasFixedColumns())
				m_nTrackedAttrib = TDCA_FIXEDCOLUMNS;
			else
				m_nTrackedAttrib = TDCA_STATUS;
			break;

		case TDCA_FIXEDCOLUMNS:
			if (!m_dlgPrefs.HasFixedColumns())
				m_nTrackedAttrib = TDCA_STATUS;
			break;

		case TDCA_CUSTOMATTRIB:
			m_sTrackedCustomAttribID = pPrefs->GetProfileString(szKey, _T("CustomAttrib"));

			// fallback
			if (m_sTrackedCustomAttribID.IsEmpty())
				m_nTrackedAttrib = TDCA_STATUS;
			break;
		}

		m_cbAttributes.ShowFixedColumns(m_dlgPrefs.HasFixedColumns());
		UpdateData(FALSE);

		EnableDisableCtrls();
		UpdateKanbanCtrlPreferences(FALSE);
	}
}

void CKanbanWnd::SetUITheme(const UITHEME* pTheme)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	GraphicsMisc::VerifyDeleteObject(m_brBack);

	if (CThemed::IsAppThemed() && pTheme)
	{
		m_theme = *pTheme;
		m_brBack.CreateSolidBrush(m_theme.crAppBackLight);
		
		// intentionally set background colours to be same as ours
		m_toolbar.SetBackgroundColors(m_theme.crAppBackLight, m_theme.crAppBackLight, FALSE, FALSE);
		m_toolbar.SetHotColor(m_theme.crToolbarHot);
	}
}

bool CKanbanWnd::ProcessMessage(MSG* pMsg) 
{
	if (IsWindowEnabled())
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		
		if (m_ctrlKanban.ProcessMessage(pMsg))
			return true;
	}

	// all else
	return false;
}

void CKanbanWnd::FilterToolTipMessage(MSG* pMsg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// Our tooltips
	m_tooltips.RelayEvent(pMsg);

	// CKanbanCtrl tooltips
	m_ctrlKanban.FilterToolTipMessage(pMsg);
}

bool CKanbanWnd::PrepareNewTask(ITaskList* pTask) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return m_ctrlKanban.PrepareNewTask(pTask);
}

bool CKanbanWnd::GetLabelEditRect(LPRECT pEdit)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (m_ctrlKanban.GetLabelEditRect(pEdit))
	{
		// convert to screen coords
		m_ctrlKanban.ClientToScreen(pEdit);
		return true;
	}

	return false;
}

IUI_HITTEST CKanbanWnd::HitTest(POINT ptScreen) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// specific task
	if (m_ctrlKanban.HitTestTask(ptScreen))
		return IUI_TASK;

	// else check elsewhere in ctrl client
	CRect rKanban;
	
	m_ctrlKanban.GetClientRect(rKanban);
	m_ctrlKanban.ClientToScreen(rKanban);

	if (rKanban.PtInRect(ptScreen))
		return IUI_TASKLIST;

	// else 
	return IUI_NOWHERE;
}

DWORD CKanbanWnd::HitTestTask(POINT ptScreen, bool /*bTitleColumnOnly*/) const
{
	return m_ctrlKanban.HitTestTask(ptScreen);
}

bool CKanbanWnd::SelectTask(DWORD dwTaskID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_dwSelTaskID = dwTaskID;

	return (m_ctrlKanban.SelectTask(dwTaskID) != FALSE);
}

bool CKanbanWnd::SelectTasks(const DWORD* pdwTaskIDs, int nTaskCount)
{
	return false;
}

bool CKanbanWnd::WantTaskUpdate(TDC_ATTRIBUTE nAttribute) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return (m_ctrlKanban.WantEditUpdate(nAttribute) != FALSE);
}

void CKanbanWnd::UpdateTasks(const ITaskList* pTaskList, IUI_UPDATETYPE nUpdate)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_ctrlKanban.UpdateTasks(pTaskList, nUpdate);

	if (!m_ctrlKanban.SelectTask(m_dwSelTaskID))
		m_dwSelTaskID = m_ctrlKanban.GetSelectedTaskID();

	// Update custom attribute combo
	const ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pTaskList, IID_TASKLISTBASE);

	if (pTasks == NULL)
	{
		ASSERT(0);
		return;
	}

	const CKanbanCustomAttributeDefinitionArray& aCustDefs = m_ctrlKanban.GetCustomAttributeDefinitions();

	if (pTasks->IsAttributeAvailable(TDCA_CUSTOMATTRIB))
	{
		m_cbCustomAttributes.SetAttributeDefinitions(aCustDefs);
		m_cbCustomAttributes.SetSelectedAttributeID(m_sTrackedCustomAttribID);
	}

	m_cbAttributes.ShowCustomAttribute(aCustDefs.GetSize());

	// Validate any change in selection
	UpdateData(TRUE);

	if (m_nTrackedAttrib == TDCA_NONE)
	{
		m_nTrackedAttrib = TDCA_STATUS;
		UpdateData(FALSE);

		EnableDisableCtrls();
	}
}

bool CKanbanWnd::DoAppCommand(IUI_APPCOMMAND nCmd, IUIAPPCOMMANDDATA* pData) 
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	switch (nCmd)
	{
	case IUI_EXPANDALL:
	case IUI_COLLAPSEALL:
	case IUI_EXPANDSELECTED:
	case IUI_COLLAPSESELECTED:
	case IUI_RESIZEATTRIBCOLUMNS:
		// not handled
		break;

	case IUI_SAVETOIMAGE:
		if (pData)
		{
			CLockUpdates lock(GetSafeHwnd());
			CBitmap bmImage;

			if (m_ctrlKanban.SaveToImage(bmImage))
				return (CGdiPlusBitmap(bmImage).SaveAsFile(pData->szFilePath) != FALSE);
		}
		break;

	case IUI_SORT:
		if (pData)
		{
			m_ctrlKanban.Sort(pData->nSortBy, (pData->bSortAscending ? TRUE : FALSE));
			return true;
		}
		break;

	case IUI_SETFOCUS:
		m_ctrlKanban.SetFocus();
		return true;
		
	case IUI_SELECTTASK:
		if (pData)
			return SelectTask(pData->dwTaskID);
		break;
		
	case IUI_GETNEXTTASK:
	case IUI_GETNEXTVISIBLETASK:
	case IUI_GETNEXTTOPLEVELTASK:
	case IUI_GETPREVTASK:
	case IUI_GETPREVVISIBLETASK:
	case IUI_GETPREVTOPLEVELTASK:
		if (pData)
		{
			DWORD dwNextID =  m_ctrlKanban.GetNextTask(pData->dwTaskID, nCmd);
			
			if (dwNextID && (dwNextID != pData->dwTaskID))
			{
				pData->dwTaskID = dwNextID;
				return true;
			}
		}
		break;

	case IUI_SELECTFIRSTTASK:
	case IUI_SELECTNEXTTASK:
	case IUI_SELECTNEXTTASKINCLCURRENT:
	case IUI_SELECTPREVTASK:
	case IUI_SELECTLASTTASK:
		if (pData)
			return (m_ctrlKanban.SelectTask(nCmd, pData->select) != FALSE);
		break;
	}

	return false;
}

void CKanbanWnd::SetTaskFont(HFONT hFont)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_ctrlKanban.SendMessage(WM_SETFONT, (WPARAM)hFont, TRUE);
}

bool CKanbanWnd::CanDoAppCommand(IUI_APPCOMMAND nCmd, const IUIAPPCOMMANDDATA* pData) const 
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	switch (nCmd)
	{
	case IUI_EXPANDALL:
	case IUI_COLLAPSEALL:
	case IUI_RESIZEATTRIBCOLUMNS:
	case IUI_EXPANDSELECTED:
	case IUI_COLLAPSESELECTED:
		// not handled
		break;

	case IUI_SAVETOIMAGE:
		return (m_ctrlKanban.CanSaveToImage() != FALSE);

	case IUI_SORT:
		if (pData)
			return (m_ctrlKanban.WantSortUpdate(pData->nSortBy) != FALSE);
		break;

	case IUI_SETFOCUS:
		return (CDialogHelper::IsChildOrSame(this, GetFocus()) == FALSE);

	case IUI_SELECTTASK:
		return true;
		
	case IUI_GETNEXTVISIBLETASK:
	case IUI_GETNEXTTOPLEVELTASK:
	case IUI_GETPREVVISIBLETASK:
	case IUI_GETPREVTOPLEVELTASK:
		return (m_ctrlKanban.GetVisibleTaskCount() > 0);

	case IUI_SELECTFIRSTTASK:
	case IUI_SELECTNEXTTASK:
	case IUI_SELECTNEXTTASKINCLCURRENT:
	case IUI_SELECTPREVTASK:
	case IUI_SELECTLASTTASK:
		return (m_ctrlKanban.GetVisibleTaskCount() > 0);
	}

	// all else
	return false;
}

void CKanbanWnd::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	Resize(cx, cy);
}

void CKanbanWnd::Resize(int cx, int cy)
{
	if (m_ctrlKanban.GetSafeHwnd())
	{
		// combos and toolbar
		int nXOffset = 0;
		CRect rOptions = GetChildRect(&m_cbOptions);

		CRect rAttrib;

		if (m_nTrackedAttrib == TDCA_CUSTOMATTRIB)
			rAttrib = GetChildRect(&m_cbCustomAttributes);
		else
			rAttrib = GetChildRect(&m_cbAttributes);

		nXOffset = (rAttrib.right + 10 - rOptions.left);

		if (nXOffset != 0)
		{
			OffsetChild(&m_cbOptions, nXOffset, 0);
			OffsetCtrl(this, IDC_OPTIONS_LABEL, nXOffset, 0);
			
			OffsetChild(&m_toolbar, nXOffset, 0);
		}
		
		// main window
		if ((cx == 0) && (cy == 0))
		{
			CRect rClient;
			GetClientRect(rClient);
			
			cx = rClient.Width();
			cy = rClient.Height();
		}

		CRect rMain(0, 0, cx, cy);
		rMain.top = CDlgUnits(this).ToPixelsY(28);
		
		m_ctrlKanban.MoveWindow(rMain);
	}
}

HBRUSH CKanbanWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_STATIC && m_brBack.GetSafeHandle())
	{
		pDC->SetTextColor(m_theme.crAppText);
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_brBack;
	}

	return hbr;
}

BOOL CKanbanWnd::OnEraseBkgnd(CDC* pDC) 
{
	// clip out our children
	CDialogHelper::ExcludeChild(&m_ctrlKanban, pDC);
		
	if (m_brBack.GetSafeHandle())
	{
		CRect rClient;
		GetClientRect(rClient);

		pDC->FillSolidRect(rClient, m_theme.crAppBackLight);
		return TRUE;
	}
	
	// else
	return CDialog::OnEraseBkgnd(pDC);
}

void CKanbanWnd::SendParentSelectionUpdate()
{
	GetParent()->SendMessage(WM_IUI_SELECTTASK, 0, m_ctrlKanban.GetSelectedTaskID());
}

void CKanbanWnd::OnSetFocus(CWnd* pOldWnd) 
{
	m_ctrlKanban.SetFocus();
}

void CKanbanWnd::UpdateKanbanCtrlPreferences(BOOL bFixedColumnsToggled)
{
	DWORD dwOptions = m_ctrlKanban.GetOptions();

	Misc::SetFlag(dwOptions, KBCF_ALWAYSSHOWBACKLOG, m_dlgPrefs.GetAlwaysShowBacklog());
	Misc::SetFlag(dwOptions, KBCF_SHOWTASKCOLORASBAR, m_dlgPrefs.GetShowTaskColorAsBar());
	Misc::SetFlag(dwOptions, KBCF_COLORBARBYPRIORITY, m_dlgPrefs.GetColorBarByPriority());
	Misc::SetFlag(dwOptions, KBCF_SORTSUBTASTASKSBELOWPARENTS, m_dlgPrefs.GetSortSubtasksBelowParents());
	Misc::SetFlag(dwOptions, KBCF_INDENTSUBTASKS, m_dlgPrefs.GetIndentSubtasks());
	Misc::SetFlag(dwOptions, KBCF_HIDEEMPTYATTRIBUTES, m_dlgPrefs.GetHideEmptyAttributes());

	m_ctrlKanban.SetOptions(dwOptions);

	m_cbAttributes.ShowFixedColumns(m_dlgPrefs.HasFixedColumns());

	// If the user was previously tracking fixed columns
	// but has now deleted them then we revert to 'status'
	if (bFixedColumnsToggled)
	{
		if ((m_nTrackedAttrib == TDCA_FIXEDCOLUMNS) && !m_dlgPrefs.HasFixedColumns())
		{
			m_nTrackedAttrib = TDCA_STATUS;
			UpdateData(FALSE);
		}
		else if ((m_nTrackedAttrib != TDCA_FIXEDCOLUMNS) && m_dlgPrefs.HasFixedColumns())
		{
			m_nTrackedAttrib = (TDC_ATTRIBUTE)TDCA_FIXEDCOLUMNS;
			UpdateData(FALSE);
		}
	}

	ProcessTrackedAttributeChange();

	m_toolbar.RefreshButtonStates();
}

void CKanbanWnd::RefreshKanbanCtrlDisplayAttributes()
{
	CKanbanAttributeArray aAttrib;

	m_dlgPrefs.GetDisplayAttributes(aAttrib, m_nTrackedAttrib);
	m_ctrlKanban.SetDisplayAttributes(aAttrib);
}

LRESULT CKanbanWnd::OnKanbanNotifyValueChange(WPARAM wp, LPARAM lp)
{
	ASSERT((HWND)wp == m_ctrlKanban.GetSafeHwnd());
	ASSERT(lp);
	DWORD dwTaskID = lp;
	
	CString sCustAttribID;
	TDC_ATTRIBUTE nAttrib = TDCA_NONE;
	
	if (m_nTrackedAttrib == TDCA_FIXEDCOLUMNS)
	{
		nAttrib = m_dlgPrefs.GetFixedAttributeToTrack(sCustAttribID);
	}
	else
	{
		nAttrib = m_nTrackedAttrib;
		sCustAttribID = m_sTrackedCustomAttribID;
	}

	IUITASKMOD mod;

	mod.nAttrib = nAttrib;
	mod.dwSelectedTaskID = dwTaskID;

	CStringArray aTaskValues;
	m_ctrlKanban.GetTaskTrackedAttributeValues(mod.dwSelectedTaskID, aTaskValues);

	CString sModValue = Misc::FormatArray(aTaskValues, '\n');

	switch (nAttrib)
	{
		case TDCA_ALLOCTO:
		case TDCA_CATEGORY:
		case TDCA_TAGS:
		case TDCA_STATUS:
		case TDCA_ALLOCBY:
		case TDCA_VERSION:
			mod.szValue = sModValue; // temporary string
			break;

		case TDCA_PRIORITY:
		case TDCA_RISK:
			if (aTaskValues.GetSize() == 0)
				mod.nValue = -2; // None
			else
				mod.nValue = _ttoi(aTaskValues[0]);
			break;

		case TDCA_CUSTOMATTRIB:
			ASSERT(!sCustAttribID.IsEmpty());

			mod.szValue = sModValue;
			mod.szCustomAttribID = sCustAttribID;

			// TODO - multi value items and time periods
			break;

		default:
			ASSERT(0);
	}

	return GetParent()->SendMessage(WM_IUI_MODIFYSELECTEDTASK, 1, (LPARAM)&mod);
}

LRESULT CKanbanWnd::OnKanbanNotifySelectionChange(WPARAM wp, LPARAM /*lp*/) 
{
	m_dwSelTaskID = wp;
	SendParentSelectionUpdate();
	
	return 0L;
}

LRESULT CKanbanWnd::OnKanbanNotifyEditTaskDone(WPARAM /*wp*/, LPARAM lp) 
{
	IUITASKMOD mod = { TDCA_DONEDATE, m_dwSelTaskID, 0 };

	if (lp) // done/not done
		VERIFY(CDateHelper::GetTimeT64(CDateHelper::GetDate(DHD_NOW), mod.tValue));
	else
		mod.tValue = T64Utils::T64_NULL;

	return GetParent()->SendMessage(WM_IUI_MODIFYSELECTEDTASK, 1, (LPARAM)&mod);
}

LRESULT CKanbanWnd::OnKanbanNotifyEditTaskFlag(WPARAM /*wp*/, LPARAM lp) 
{
	IUITASKMOD mod = { TDCA_FLAG, m_dwSelTaskID, 0 };
	mod.bValue = (lp != 0);

	return GetParent()->SendMessage(WM_IUI_MODIFYSELECTEDTASK, 1, (LPARAM)&mod);
}

LRESULT CKanbanWnd::OnKanbanNotifyFlagChange(WPARAM /*wp*/, LPARAM lp) 
{
	IUITASKMOD mod = { TDCA_DONEDATE, m_dwSelTaskID, 0 };

	if (lp) // done/not done
		VERIFY(CDateHelper::GetTimeT64(CDateHelper::GetDate(DHD_NOW), mod.tValue));
	else
		mod.tValue = T64Utils::T64_NULL;

	return GetParent()->SendMessage(WM_IUI_MODIFYSELECTEDTASK, 1, (LPARAM)&mod);
}

void CKanbanWnd::OnKanbanPreferences() 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// If the user creates fixed column defs for the first time
	// we will automatically turn them on
	BOOL bHadFixedColumns = m_dlgPrefs.HasFixedColumns();
	
	if (m_dlgPrefs.DoModal(m_ctrlKanban) == IDOK)
	{
		UpdateKanbanCtrlPreferences(bHadFixedColumns != m_dlgPrefs.HasFixedColumns());
		Resize();

		m_ctrlKanban.SetFocus();
	}
}

void CKanbanWnd::EnableDisableCtrls()
{
	BOOL bCustom = (m_nTrackedAttrib == TDCA_CUSTOMATTRIB);
	
	m_cbCustomAttributes.ShowWindow(bCustom ? SW_SHOW : SW_HIDE);
	m_cbCustomAttributes.EnableWindow(bCustom);
}

void CKanbanWnd::OnUpdateKanbanPreferences(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CKanbanWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if (!bShow)
		m_ctrlKanban.CancelOperation();
}

void CKanbanWnd::OnSelchangeTrackedAttribute() 
{
	UpdateData();

	ProcessTrackedAttributeChange();
}

void CKanbanWnd::ProcessTrackedAttributeChange() 
{
	CKanbanColumnArray aColDefs;
	TDC_ATTRIBUTE nTrackAttrib;
	CString sCustomAttrib;

	if (m_nTrackedAttrib == TDCA_FIXEDCOLUMNS)
	{
		VERIFY(m_dlgPrefs.GetFixedColumnDefinitions(aColDefs));

		nTrackAttrib = m_dlgPrefs.GetFixedAttributeToTrack(sCustomAttrib);
	}
	else
	{
		// If the new type is 'custom' auto-select the first custom attribute
		// else hide the custom attribute field
		if ((m_nTrackedAttrib == TDCA_CUSTOMATTRIB) && m_sTrackedCustomAttribID.IsEmpty())
		{
			m_cbCustomAttributes.SetCurSel(0);
			m_sTrackedCustomAttribID = m_cbCustomAttributes.GetSelectedAttributeID();

			// Fallback
			if (m_sTrackedCustomAttribID.IsEmpty())
			{
				m_nTrackedAttrib = TDCA_STATUS;
				UpdateData(FALSE);
			}
		}
			
		nTrackAttrib = m_nTrackedAttrib;
		sCustomAttrib = m_sTrackedCustomAttribID;
	}

	EnableDisableCtrls();
	Resize();
	RefreshKanbanCtrlDisplayAttributes();

	// Track the new attribute
	m_ctrlKanban.TrackAttribute(nTrackAttrib, sCustomAttrib, aColDefs);
	m_ctrlKanban.SelectTask(m_dwSelTaskID);
}

void CKanbanWnd::OnSelchangeOptions() 
{
	BOOL bWasShowingParentTasks = m_ctrlKanban.HasOption(KBCF_SHOWPARENTTASKS);
	BOOL bIsShowingParentTasks = m_cbOptions.HasSelectedOption(KBCF_SHOWPARENTTASKS);

	DWORD dwOptions = m_ctrlKanban.GetOptions();

	Misc::SetFlag(dwOptions, KBCF_SHOWEMPTYCOLUMNS, m_cbOptions.HasSelectedOption(KBCF_SHOWEMPTYCOLUMNS));
	Misc::SetFlag(dwOptions, KBCF_SHOWPARENTTASKS, bIsShowingParentTasks);

	m_ctrlKanban.SetOptions(dwOptions);

	// Fixup selection if parents are being shown
	if ((bWasShowingParentTasks || bIsShowingParentTasks) && m_dwSelTaskID)
	{
		m_ctrlKanban.SelectTask(m_dwSelTaskID);
		SendParentSelectionUpdate();
	}
}

BOOL CKanbanWnd::OnToolTipNotify(UINT /*id*/, NMHDR* pNMHDR, LRESULT* /*pResult*/)
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
	case IDC_OPTIONS:
		sTooltip = m_cbOptions.GetTooltip();
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
