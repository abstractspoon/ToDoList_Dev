// KanbanWnd.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "KanbanBoard.h"
#include "KanbanWnd.h"
#include "KanbanMsg.h"

#include "..\todolist\tdcenum.h"
#include "..\todolist\tdcmsg.h"

#include "..\shared\misc.h"
#include "..\shared\themed.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\dlgunits.h"
#include "..\shared\datehelper.h"
#include "..\shared\localizer.h"
#include "..\shared\autoflag.h"
#include "..\shared\holdredraw.h"

#include "..\3rdparty\T64Utils.h"
#include "..\3rdparty\dibdata.h"

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
	m_nTrackedAttrib(IUI_NONE),
	m_ctrlKanban(),
#pragma warning(disable:4355)
	m_dlgPrefs(this)
#pragma warning(default:4355)
{
	m_icon.LoadIcon(IDR_KANBAN);
	m_cbOptions.SetMinDLUHeight(10);
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

	if (pDX->m_bSaveAndValidate)
	{
		m_nTrackedAttrib = m_cbAttributes.GetSelectedAttribute();
		m_sCustomAttribID = CDialogHelper::GetSelectedItem(m_cbCustomAttributes);
	}
	else if (m_nTrackedAttrib != IUI_NONE)
	{
		m_cbAttributes.SetSelectedAttribute(m_nTrackedAttrib);
		m_cbCustomAttributes.SelectString(-1, m_sCustomAttribID);
	}
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
	ON_REGISTERED_MESSAGE(WM_KBC_COMPLETIONCHANGE, OnKanbanNotifyCompletionChange)
	ON_REGISTERED_MESSAGE(WM_KBC_SELECTIONCHANGE, OnKanbanNotifySelectionChange)
	ON_REGISTERED_MESSAGE(WM_KBC_PREFSHELP, OnKanbanPrefsHelp)
	ON_REGISTERED_MESSAGE(WM_KBC_GETTASKICON, OnKanbanGetTaskIcon)
	ON_REGISTERED_MESSAGE(WM_KBC_EDITTASKTITLE, OnKanbanEditTaskTitle)
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

LRESULT CKanbanWnd::OnKanbanGetTaskIcon(WPARAM wp, LPARAM lp)
{
	return GetParent()->SendMessage(WM_IUI_GETTASKICON, wp, lp);
}

LRESULT CKanbanWnd::OnKanbanEditTaskTitle(WPARAM wp, LPARAM lp)
{
	return GetParent()->SendMessage(WM_IUI_EDITSELECTEDTASKTITLE, wp, lp);
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

	if (m_nTrackedAttrib == IUI_CUSTOMATTRIB)
		pPrefs->WriteProfileString(sKey, _T("CustomAttrib"), m_sCustomAttribID);
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
		if (pPrefs->GetProfileInt(_T("Preferences"), _T("IndividualPriorityColors")))
		{
			aPriorityColors.Add(pPrefs->GetProfileInt(_T("Preferences\\Colors"), _T("P0")));
			aPriorityColors.Add(pPrefs->GetProfileInt(_T("Preferences\\Colors"), _T("P1")));
			aPriorityColors.Add(pPrefs->GetProfileInt(_T("Preferences\\Colors"), _T("P2")));
			aPriorityColors.Add(pPrefs->GetProfileInt(_T("Preferences\\Colors"), _T("P3")));
			aPriorityColors.Add(pPrefs->GetProfileInt(_T("Preferences\\Colors"), _T("P4")));
			aPriorityColors.Add(pPrefs->GetProfileInt(_T("Preferences\\Colors"), _T("P5")));
			aPriorityColors.Add(pPrefs->GetProfileInt(_T("Preferences\\Colors"), _T("P6")));
			aPriorityColors.Add(pPrefs->GetProfileInt(_T("Preferences\\Colors"), _T("P7")));
			aPriorityColors.Add(pPrefs->GetProfileInt(_T("Preferences\\Colors"), _T("P8")));
			aPriorityColors.Add(pPrefs->GetProfileInt(_T("Preferences\\Colors"), _T("P9")));
			aPriorityColors.Add(pPrefs->GetProfileInt(_T("Preferences\\Colors"), _T("P10")));
		}
		else
		{
			COLORREF crFrom = pPrefs->GetProfileInt(_T("Preferences\\Colors"), _T("Low"));
			COLORREF crTo = pPrefs->GetProfileInt(_T("Preferences\\Colors"), _T("High"));
			BOOL bRGB = !pPrefs->GetProfileInt(_T("Preferences"), _T("HLSColorGradient"));
			
			GraphicsMisc::CalculateColorGradient(crFrom, crTo, 11, aPriorityColors, bRGB);
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
	m_ctrlKanban.SetOption(KBCF_STRIKETHRUDONETASKS, pPrefs->GetProfileInt(_T("Preferences"), _T("StrikethroughDone"), TRUE));
	m_ctrlKanban.SetOption(KBCF_TASKTEXTCOLORISBKGND, pPrefs->GetProfileInt(_T("Preferences"), _T("ColorTaskBackground"), FALSE));
	m_ctrlKanban.SetOption(KBCF_SHOWCOMPLETIONCHECKBOXES, pPrefs->GetProfileInt(_T("Preferences"), _T("AllowCheckboxAgainstTreeItem"), TRUE));

	UpdatePriorityColors(pPrefs);
	
	// Default attribute values
	m_ctrlKanban.LoadDefaultAttributeListValues(pPrefs);

	// Kanban specific options
	if (!bAppOnly)
	{
		CString sKey(szKey);
		
		// Options
		DWORD dwComboOptions = 0;

		if (pPrefs->GetProfileInt(sKey, _T("ShowParents"), TRUE))
			dwComboOptions |= KBCF_SHOWPARENTTASKS;

		if (pPrefs->GetProfileInt(sKey, _T("ShowEmptyColumns"),TRUE))
			dwComboOptions |= KBCF_SHOWEMPTYCOLUMNS;

		m_cbOptions.SetSelectedOptions(dwComboOptions);
		OnSelchangeOptions();
		
		// Last tracked attribute
		m_dlgPrefs.LoadPreferences(pPrefs, sKey);

		m_nTrackedAttrib = (IUI_ATTRIBUTE)pPrefs->GetProfileInt(sKey, _T("LastTrackedAttribute"), IUI_STATUS);

		switch (m_nTrackedAttrib)
		{
		case IUI_FIXEDCOLUMNS:
			if (!m_dlgPrefs.HasFixedColumns())
				m_nTrackedAttrib = IUI_STATUS;
			break;

		case IUI_CUSTOMATTRIB:
			m_sCustomAttribID = pPrefs->GetProfileString(sKey, _T("CustomAttrib"));

			// fallback
			if (m_sCustomAttribID.IsEmpty())
				m_nTrackedAttrib = IUI_STATUS;
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

IUI_HITTEST CKanbanWnd::HitTest(const POINT& ptScreen) const
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


bool CKanbanWnd::SelectTask(DWORD dwTaskID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return (m_ctrlKanban.SelectTask(dwTaskID) != FALSE);
}

bool CKanbanWnd::SelectTasks(const DWORD* pdwTaskIDs, int nTaskCount)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CDWordArray aTaskIDs;

	if (nTaskCount)
	{
		ASSERT(pdwTaskIDs);
	
		for (int nID = 0; nID < nTaskCount; nID++)
			aTaskIDs.Add(pdwTaskIDs[nID]);
	}

	return (m_ctrlKanban.SelectTasks(aTaskIDs) != FALSE);
}

bool CKanbanWnd::WantTaskUpdate(IUI_ATTRIBUTE nAttribute) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return (CKanbanCtrl::WantEditUpdate(nAttribute) != FALSE);
}

void CKanbanWnd::UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, const IUI_ATTRIBUTE* pAttributes, int nNumAttributes)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CSet<IUI_ATTRIBUTE> attrib(pAttributes, nNumAttributes);
	
	m_ctrlKanban.UpdateTasks(pTasks, nUpdate, attrib);

	// Update custom attribute combo
	CStringArray aCustAttribIDs;
	m_ctrlKanban.GetCustomAttributeIDs(aCustAttribIDs);

	if (attrib.Has(IUI_CUSTOMATTRIB))
	{
		CLocalizer::EnableTranslation(m_cbCustomAttributes, FALSE);
		CDialogHelper::SetComboBoxItems(m_cbCustomAttributes, aCustAttribIDs);
		m_cbCustomAttributes.SelectString(-1, m_sCustomAttribID);
	}

	m_cbAttributes.ShowCustomAttribute(aCustAttribIDs.GetSize());

	// Validate any change in selection
	UpdateData(TRUE);

	if (m_nTrackedAttrib == IUI_NONE)
	{
		m_nTrackedAttrib = IUI_STATUS;
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
			{
				CDibData dib;

				if (dib.CreateDIB(bmImage) && dib.SaveDIB(pData->szFilePath))
					return true;
			}
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
	case IUI_GETNEXTTOPLEVELTASK:
	case IUI_GETPREVTASK:
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

	case IUI_SETTASKFONT:
		if (pData)
		{
			m_ctrlKanban.SendMessage(WM_SETFONT, (WPARAM)pData->hFont, TRUE);
			return false;
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
			return (CKanbanCtrl::WantSortUpdate(pData->nSortBy) != FALSE);
		break;

	case IUI_SETFOCUS:
		return (CDialogHelper::IsChildOrSame(this, GetFocus()) == FALSE);

	case IUI_SELECTTASK:
		return true;
		
	case IUI_GETNEXTTASK:
	case IUI_GETNEXTTOPLEVELTASK:
	case IUI_GETPREVTASK:
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

		if (m_nTrackedAttrib == IUI_CUSTOMATTRIB)
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
	CDWordArray aSelIDs;
	m_ctrlKanban.GetSelectedTaskIDs(aSelIDs);

	switch (aSelIDs.GetSize())
	{
	case 0:
		GetParent()->SendMessage(WM_IUI_SELECTTASK, 0, 0);
		break;
			
	case 1:
		GetParent()->SendMessage(WM_IUI_SELECTTASK, 0, aSelIDs[0]);
		break;
		
	default:
		GetParent()->SendMessage(WM_IUI_SELECTTASK, (WPARAM)aSelIDs.GetData(), (LPARAM)aSelIDs.GetSize());
	}
}

void CKanbanWnd::OnSetFocus(CWnd* pOldWnd) 
{
	// DON'T call dialog base class because
	// it will forcibly set focus to first list ctrl
	m_ctrlKanban.SetFocus();
}

void CKanbanWnd::UpdateKanbanCtrlPreferences(BOOL bFixedColumnsToggled)
{
	m_ctrlKanban.SetOption(KBCF_ALWAYSSHOWBACKLOG, m_dlgPrefs.GetAlwaysShowBacklog());
	m_ctrlKanban.SetOption(KBCF_SHOWTASKCOLORASBAR, m_dlgPrefs.GetShowTaskColorAsBar());
	m_ctrlKanban.SetOption(KBCF_COLORBARBYPRIORITY, m_dlgPrefs.GetColorBarByPriority());
	m_ctrlKanban.SetOption(KBCF_SORTSUBTASTASKSBELOWPARENTS, m_dlgPrefs.GetSortSubtasksBelowParents());

	m_cbAttributes.ShowFixedColumns(m_dlgPrefs.HasFixedColumns());

	// If the user was previously tracking fixed columns
	// but has now deleted them then we revert to 'status'
	if (bFixedColumnsToggled)
	{
		if ((m_nTrackedAttrib == IUI_FIXEDCOLUMNS) && !m_dlgPrefs.HasFixedColumns())
		{
			m_nTrackedAttrib = IUI_STATUS;
			UpdateData(FALSE);
		}
		else if ((m_nTrackedAttrib != IUI_FIXEDCOLUMNS) && m_dlgPrefs.HasFixedColumns())
		{
			m_nTrackedAttrib = (IUI_ATTRIBUTE)IUI_FIXEDCOLUMNS;
			UpdateData(FALSE);
		}
	}

	ProcessTrackedAttributeChange();

	CKanbanAttributeArray aAttrib;
	m_dlgPrefs.GetDisplayAttributes(aAttrib);
	m_ctrlKanban.SetDisplayAttributes(aAttrib);

	m_toolbar.RefreshButtonStates();
}

LRESULT CKanbanWnd::OnKanbanNotifyValueChange(WPARAM wp, LPARAM lp)
{
	ASSERT((HWND)wp == m_ctrlKanban.GetSafeHwnd());
	ASSERT(lp);
	const CDWordArray* pChangedIDs = (const CDWordArray*)lp;
	
	CString sCustAttribID;
	IUI_ATTRIBUTE nAttrib = IUI_NONE;
	
	if (m_nTrackedAttrib == IUI_FIXEDCOLUMNS)
	{
		nAttrib = m_dlgPrefs.GetFixedAttributeToTrack(sCustAttribID);
	}
	else
	{
		nAttrib = m_nTrackedAttrib;
		sCustAttribID = m_sCustomAttribID;
	}

	int nNumTasks = pChangedIDs->GetSize();
	CArray<IUITASKMOD, IUITASKMOD&> aMods;
	CStringArray aTempModValues; // because we are sending pointers to temp values

	aMods.SetSize(nNumTasks);
	aTempModValues.SetSize(nNumTasks);

	for (int nTask = 0; nTask < nNumTasks; nTask++)
	{
		IUITASKMOD& mod = aMods[nTask];

		mod.nAttrib = nAttrib;
		mod.dwSelectedTaskID = pChangedIDs->GetAt(nTask);

		CStringArray aTaskValues;
		m_ctrlKanban.GetTaskTrackedAttributeValues(mod.dwSelectedTaskID, aTaskValues);

		aTempModValues[nTask] = Misc::FormatArray(aTaskValues, '\n');

		switch (nAttrib)
		{
		case IUI_ALLOCTO:
		case IUI_CATEGORY:
		case IUI_TAGS:
		case IUI_STATUS:
		case IUI_ALLOCBY:
		case IUI_VERSION:
			mod.szValue = aTempModValues[nTask];
			break;
		
		case IUI_PRIORITY:
		case IUI_RISK:
			if (aTaskValues.GetSize() == 0)
				mod.nValue = -2; // None
			else
				mod.nValue = _ttoi(aTaskValues[0]);
			break;
		
		case IUI_CUSTOMATTRIB:
			ASSERT(!sCustAttribID.IsEmpty());
		
			mod.szValue = aTaskValues[nTask];
			mod.szCustomAttribID = sCustAttribID;

			// TODO - multi value items and time periods
			break;
		
		default:
			ASSERT(0);
		}
	}
		
	return GetParent()->SendMessage(WM_IUI_MODIFYSELECTEDTASK, nNumTasks, (LPARAM)aMods.GetData());
}

LRESULT CKanbanWnd::OnKanbanNotifySelectionChange(WPARAM /*wp*/, LPARAM /*lp*/) 
{
	SendParentSelectionUpdate();
	
	return 0L;
}

LRESULT CKanbanWnd::OnKanbanNotifyCompletionChange(WPARAM /*wp*/, LPARAM lp) 
{
	IUITASKMOD mod = { IUI_DONEDATE, 0 };

	if (lp) // done/not done
		VERIFY(CDateHelper::GetTimeT64(CDateHelper::GetDate(DHD_NOW), mod.tValue));
	else
		mod.tValue = T64Utils::T64_NULL;

	return GetParent()->SendMessage(WM_IUI_MODIFYSELECTEDTASK, 1, (LPARAM)&mod);
}

void CKanbanWnd::OnKanbanPreferences() 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CStringArray aCustAttribIDs;
	m_ctrlKanban.GetCustomAttributeIDs(aCustAttribIDs);

	CKanbanAttributeValueMap mapValues;
	m_ctrlKanban.GetAttributeValues(mapValues);

	CKanbanAttributeArray aDisplayAttrib;
	m_ctrlKanban.GetDisplayAttributes(aDisplayAttrib);

	// If the user creates fixed column defs for the first time
	// we will automatically turn them on
	BOOL bHadFixedColumns = m_dlgPrefs.HasFixedColumns();
	
	if (m_dlgPrefs.DoModal(aCustAttribIDs, mapValues, aDisplayAttrib) == IDOK)
	{
		UpdateKanbanCtrlPreferences(bHadFixedColumns != m_dlgPrefs.HasFixedColumns());
		Resize();

		m_ctrlKanban.SetFocus();
	}
}

void CKanbanWnd::EnableDisableCtrls()
{
	BOOL bCustom = (m_nTrackedAttrib == IUI_CUSTOMATTRIB);
	
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
	IUI_ATTRIBUTE nTrackAttrib;
	CString sCustomAttrib;

	if (m_nTrackedAttrib == IUI_FIXEDCOLUMNS)
	{
		VERIFY(m_dlgPrefs.GetFixedColumnDefinitions(aColDefs));

		nTrackAttrib = m_dlgPrefs.GetFixedAttributeToTrack(sCustomAttrib);
	}
	else
	{
		// If the new type is 'custom' auto-select the first custom attribute
		// else hide the custom attribute field
		if ((m_nTrackedAttrib == IUI_CUSTOMATTRIB) && m_sCustomAttribID.IsEmpty())
		{
			m_sCustomAttribID = CDialogHelper::GetItem(m_cbCustomAttributes, 0);

			// Fallback
			if (m_sCustomAttribID.IsEmpty())
				m_nTrackedAttrib = IUI_STATUS;

			UpdateData(FALSE);
		}
			
		nTrackAttrib = m_nTrackedAttrib;
		sCustomAttrib = m_sCustomAttribID;
	}

	EnableDisableCtrls();
	Resize();

	// Track the new attribute
	m_ctrlKanban.TrackAttribute(nTrackAttrib, sCustomAttrib, aColDefs);
}

void CKanbanWnd::OnSelchangeOptions() 
{
	m_ctrlKanban.SetOption(KBCF_SHOWPARENTTASKS, m_cbOptions.GetCheckByData(KBCF_SHOWPARENTTASKS));
	m_ctrlKanban.SetOption(KBCF_SHOWEMPTYCOLUMNS, m_cbOptions.GetCheckByData(KBCF_SHOWEMPTYCOLUMNS));
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
