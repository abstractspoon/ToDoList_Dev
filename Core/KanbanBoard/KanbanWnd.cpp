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
#include "..\3rdparty\XNamedColors.h"

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
	m_dlgPrefs(this)
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
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_OPTIONS, m_cbOptions);
	DDX_Control(pDX, IDC_ATTRIBUTES, m_cbAttributes);

	m_cbAttributes.DDX(pDX, m_nTrackedAttrib, m_sTrackedCustomAttribID);
}

BEGIN_MESSAGE_MAP(CKanbanWnd, CDialog)
	//{{AFX_MSG_MAP(CKanbanWnd)
	ON_CBN_SELCHANGE(IDC_ATTRIBUTES, OnSelchangeTrackedAttribute)
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
	ON_REGISTERED_MESSAGE(WM_KBC_SORTCHANGE, OnKanbanNotifySortChange)
	ON_REGISTERED_MESSAGE(WM_KBC_SHOWFILELINK, OnKanbanNotifyShowFileLink)
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

LRESULT CKanbanWnd::OnKanbanNotifySortChange(WPARAM wp, LPARAM lp)
{
	return GetParent()->SendMessage(WM_IUI_SORTCHANGE, wp, lp);
}

LRESULT CKanbanWnd::OnKanbanNotifyShowFileLink(WPARAM /*wp*/, LPARAM lp)
{
	return GetParent()->SendMessage(WM_IUI_SHOWFILELINK, 0, lp);
}

BOOL CKanbanWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// non-translatables
	CLocalizer::EnableTranslation(*GetDlgItem(IDC_SELECTEDTASKDATES), FALSE);

	// create toolbar
	if (m_toolbar.CreateEx(this))
	{
		VERIFY(m_toolbar.LoadToolBar(IDR_TOOLBAR, IDB_TOOLBAR_STD, colorMagenta));
		VERIFY(m_tbHelper.Initialize(&m_toolbar, this));
		
		CRect rToolbar = CDialogHelper::GetCtrlRect(this, IDC_TB_PLACEHOLDER);
		m_toolbar.Resize(rToolbar.Width(), rToolbar.TopLeft());
		m_toolbar.RefreshButtonStates(TRUE);
	}

	m_ctrlKanban.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0, 0, 0, 0), this, 101);
	m_ctrlKanban.SetFocus();

	m_mgrPrompts.SetComboPrompt(m_cbOptions, CEnString(IDS_OPTIONS_NONE));
	
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
	pPrefs->WriteProfileInt(sKey, _T("HideParents"), m_ctrlKanban.HasOption(KBCF_HIDEPARENTTASKS));
	pPrefs->WriteProfileInt(sKey, _T("HideEmptyColumns"), m_ctrlKanban.HasOption(KBCF_HIDEEMPTYCOLUMNS));
	pPrefs->WriteProfileInt(sKey, _T("HideSubtasks"), m_ctrlKanban.HasOption(KBCF_HIDESUBTASKS));

	// Preferences
	m_dlgPrefs.SavePreferences(pPrefs, sKey);
	m_ctrlKanban.SavePreferences(pPrefs, szKey);
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
	Misc::SetFlag(dwOptions, KBCF_DUEHAVEHIGHESTPRIORITYRISK, pPrefs->GetProfileInt(_T("Preferences"), _T("DueTasksHaveHighestPriority"), FALSE));
	Misc::SetFlag(dwOptions, KBCF_DONEHAVELOWESTPRIORITYRISK, pPrefs->GetProfileInt(_T("Preferences"), _T("DoneTasksHaveLowestPriority"), FALSE));
	Misc::SetFlag(dwOptions, KBCF_PARENTSSHOWHIGHESTPRIORITYRISK, pPrefs->GetProfileInt(_T("Preferences"), _T("UseHighestPriority"), FALSE));
	Misc::SetFlag(dwOptions, KBCF_COLUMNHEADERSORTING, pPrefs->GetProfileInt(_T("Preferences"), _T("EnableColumnHeaderSorting"), TRUE));

	m_ctrlKanban.SetOptions(dwOptions);
	
	// Default attribute values
	m_ctrlKanban.LoadPreferences(pPrefs, szKey, bAppOnly);

	// Kanban specific options
	if (!bAppOnly)
	{
		// Backwards compatibility
		BOOL bHideParents = pPrefs->GetProfileInt(szKey, _T("HideParents"), -1);

		if (bHideParents == -1)
			bHideParents = !pPrefs->GetProfileInt(szKey, _T("ShowParents"), TRUE);

		BOOL bHideEmptyCols = pPrefs->GetProfileInt(szKey, _T("HideEmptyColumns"), -1);

		if (bHideEmptyCols == -1)
			bHideEmptyCols = !pPrefs->GetProfileInt(szKey, _T("ShowEmptyColumns"), TRUE);

		DWORD dwComboOptions = 0;

		Misc::SetFlag(dwComboOptions, KBCF_HIDEPARENTTASKS, bHideParents);
		Misc::SetFlag(dwComboOptions, KBCF_HIDEEMPTYCOLUMNS, bHideEmptyCols);
		Misc::SetFlag(dwComboOptions, KBCF_HIDESUBTASKS, pPrefs->GetProfileInt(szKey, _T("HideSubtasks"), FALSE));
		
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

		// Rescale images because background colour has changed
		if (GraphicsMisc::WantDPIScaling())
			m_toolbar.SetImage(IDB_TOOLBAR_STD, colorMagenta);
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

	m_ctrlKanban.FilterToolTipMessage(pMsg);
	m_cbOptions.FilterToolTipMessage(pMsg);
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
	BOOL bHeader;
	int nCol = m_ctrlKanban.HitTestColumn(ptScreen, bHeader);

	if ((nCol != -1) && !bHeader)
		return IUI_TASKLIST;

	// else 
	return IUI_NOWHERE;
}

DWORD CKanbanWnd::HitTestTask(POINT ptScreen, bool /*bTitleColumnOnly*/) const
{
	return m_ctrlKanban.HitTestTask(ptScreen);
}

bool CKanbanWnd::SelectTask(DWORD dwTaskID, bool bTaskLink)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return SelectTasks(&dwTaskID, 1);
}

bool CKanbanWnd::SelectTasks(const DWORD* pdwTaskIDs, int nTaskCount)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CDWordArray aTaskIDs;
	aTaskIDs.SetSize(nTaskCount);

	for (int nID = 0; nID < nTaskCount; nID++)
		aTaskIDs[nID] = pdwTaskIDs[nID];

	if (m_ctrlKanban.SelectTasks(aTaskIDs))
	{
		m_ctrlKanban.GetSelectedTaskIDs(m_aSelTaskIDs);
		return true;
	}

	// if only one task was being selected, cache it so that
	// if the visibility options are the cause then we can
	// restore the selection later if options change again
	if (nTaskCount == 1)
		m_aSelTaskIDs.Copy(aTaskIDs);
	else
		m_aSelTaskIDs.RemoveAll();

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

	if (!m_ctrlKanban.SelectTasks(m_aSelTaskIDs))
		m_ctrlKanban.GetSelectedTaskIDs(m_aSelTaskIDs);

	// Update custom attribute combo
	const ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pTaskList, IID_TASKLISTBASE);

	if (pTasks == NULL)
	{
		ASSERT(0);
		return;
	}

	m_cbAttributes.SetAttributeDefinitions(m_ctrlKanban.GetCustomAttributeDefinitions());
	m_cbAttributes.SetSelectedAttribute(m_nTrackedAttrib, m_sTrackedCustomAttribID);

	// Validate any change in selection
	UpdateData(TRUE);

	if (m_nTrackedAttrib == TDCA_NONE)
	{
		m_nTrackedAttrib = TDCA_STATUS;
		UpdateData(FALSE);
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

	case IUI_SELECTALL:
		return (m_ctrlKanban.SelectAllInSelectedColumn() != FALSE);
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
	GetParent()->SendMessage(WM_IUI_SELECTTASK, (WPARAM)m_aSelTaskIDs.GetData(), m_aSelTaskIDs.GetSize());
}

void CKanbanWnd::OnSetFocus(CWnd* /*pOldWnd*/) 
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
			m_sTrackedCustomAttribID.Empty();
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
	ASSERT(wp && lp);
	
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

	int nNumMods = (int)wp;
	LPDWORD pTaskIDs = (LPDWORD)lp;

	CArray<IUITASKMOD, IUITASKMOD&> aMods;
	aMods.SetSize(nNumMods);

	CStringArray aTemp; // because string items are passed as pointers

	for (int nMod = 0; nMod < nNumMods; nMod++)
	{
		IUITASKMOD& mod = aMods[nMod];

		mod.nAttrib = nAttrib;
		mod.dwSelectedTaskID = pTaskIDs[nMod];

		CStringArray aTaskValues;
		int nNumVal = m_ctrlKanban.GetTaskTrackedAttributeValues(mod.dwSelectedTaskID, aTaskValues);

		switch (nAttrib)
		{
			case TDCA_ALLOCTO:
			case TDCA_CATEGORY:
			case TDCA_TAGS:
			case TDCA_STATUS:
			case TDCA_ALLOCBY:
			case TDCA_VERSION:
				aTemp.Add(Misc::FormatArray(aTaskValues, '\n'));
				mod.szValue = Misc::Last(aTemp);
				break;

			case TDCA_PRIORITY:
			case TDCA_RISK:
				mod.nValue = ((nNumVal == 0) ? -2 : _ttoi(aTaskValues[0]));
				break;

			case TDCA_CUSTOMATTRIB:
				ASSERT(!sCustAttribID.IsEmpty());

				aTemp.Add(Misc::FormatArray(aTaskValues, '\n'));

				mod.szValue = Misc::Last(aTemp);
				mod.szCustomAttribID = sCustAttribID;

				// TODO - multi value items and time periods
				break;

			default:
				ASSERT(0);
		}
	}

	return GetParent()->SendMessage(WM_IUI_MODIFYSELECTEDTASK, nNumMods, (LPARAM)aMods.GetData());
}

LRESULT CKanbanWnd::OnKanbanNotifySelectionChange(WPARAM /*wp*/, LPARAM /*lp*/) 
{
	m_ctrlKanban.GetSelectedTaskIDs(m_aSelTaskIDs);
	SendParentSelectionUpdate();
	
	return 0L;
}

LRESULT CKanbanWnd::OnKanbanNotifyEditTaskDone(WPARAM /*wp*/, LPARAM lp) 
{
	// This is complicated by the fact that we don't
	// want to change any existing completion dates
	int nNumSel = m_aSelTaskIDs.GetSize();
	
	CArray<IUITASKMOD, IUITASKMOD&> aMods;
	IUITASKMOD mod = { TDCA_DONEDATE, 0, 0 };

	for (int nSel = 0, nMod = 0; nSel < nNumSel; nSel++)
	{
		mod.dwSelectedTaskID = m_aSelTaskIDs[nSel];

		if (lp && m_ctrlKanban.IsTaskDone(mod.dwSelectedTaskID))
			continue;

		if (lp) // done
			VERIFY(CDateHelper::GetTimeT64(CDateHelper::GetDate(DHD_NOW), mod.tValue));
		else
			mod.tValue = T64Utils::T64_NULL;

		aMods.Add(mod);
	}

	return GetParent()->SendMessage(WM_IUI_MODIFYSELECTEDTASK, aMods.GetSize(), (LPARAM)aMods.GetData());
}

LRESULT CKanbanWnd::OnKanbanNotifyEditTaskFlag(WPARAM /*wp*/, LPARAM lp) 
{
	IUITASKMOD mod = { TDCA_FLAG, 0, 0 };
	mod.bValue = (lp != FALSE);

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

		m_ctrlKanban.SetFocus();
	}
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
	TDC_ATTRIBUTE nTrackAttrib = m_nTrackedAttrib;
	CString sCustomAttrib = m_sTrackedCustomAttribID;

	CKanbanColumnArray aColDefs;

	if (m_nTrackedAttrib == TDCA_FIXEDCOLUMNS)
	{
		VERIFY(m_dlgPrefs.GetFixedColumnDefinitions(aColDefs));

		nTrackAttrib = m_dlgPrefs.GetFixedAttributeToTrack(sCustomAttrib);
	}

	RefreshKanbanCtrlDisplayAttributes();

	// Track the new attribute
	m_ctrlKanban.TrackAttribute(nTrackAttrib, sCustomAttrib, aColDefs);

	if (!m_ctrlKanban.SelectTasks(m_aSelTaskIDs))
	{
		if (m_ctrlKanban.GetSelectedTaskIDs(m_aSelTaskIDs))
			SendParentSelectionUpdate();
	}
}

void CKanbanWnd::OnSelchangeOptions() 
{
	DWORD dwCurOptions = m_ctrlKanban.GetOptions(), dwNewOptions = dwCurOptions;

	Misc::SetFlag(dwNewOptions, KBCF_HIDEEMPTYCOLUMNS, m_cbOptions.HasSelectedOption(KBCF_HIDEEMPTYCOLUMNS));
	Misc::SetFlag(dwNewOptions, KBCF_HIDEPARENTTASKS, m_cbOptions.HasSelectedOption(KBCF_HIDEPARENTTASKS));
	Misc::SetFlag(dwNewOptions, KBCF_HIDESUBTASKS, m_cbOptions.HasSelectedOption(KBCF_HIDESUBTASKS));

	if (dwNewOptions != dwCurOptions)
	{
		CDWordArray aSelTaskIDs;
		aSelTaskIDs.Copy(m_aSelTaskIDs);

		m_ctrlKanban.SetOptions(dwNewOptions);

		if (!m_ctrlKanban.GetSelectedCount())
		{
			BOOL bWasShowingParents = !Misc::HasFlag(dwCurOptions, KBCF_HIDEPARENTTASKS);
			BOOL bIsShowingParents = !Misc::HasFlag(dwNewOptions, KBCF_HIDEPARENTTASKS);

			BOOL bWasShowingSubtasks = !Misc::HasFlag(dwCurOptions, KBCF_HIDESUBTASKS);
			BOOL bIsShowingSubtasks = !Misc::HasFlag(dwNewOptions, KBCF_HIDESUBTASKS);

			if ((bWasShowingParents && !bIsShowingParents) ||
				(bWasShowingSubtasks && !bIsShowingSubtasks))
			{
				// If task visibility was being turned off and now
				// nothing is selected cache the old selection in case
				// we can restore it if the visibility is restored
				if (m_aSelTaskIDs.GetSize() == 0)
					m_aSelTaskIDs.Copy(aSelTaskIDs);
			}
			else if ((!bWasShowingParents && bIsShowingParents) ||
					 (!bWasShowingSubtasks && bIsShowingSubtasks))
			{
				// If task visibility is being turned on and nothing
				// is selected but we have something cached then try
				// to restore it
				if (m_ctrlKanban.SelectTasks(m_aSelTaskIDs))
					SendParentSelectionUpdate();
			}
		}
	}
}
