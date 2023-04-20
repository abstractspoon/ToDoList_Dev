// TabbedToDoCtrl.cpp: implementation of the CTabbedToDoCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TabbedToDoCtrl.h"
#include "todoitem.h"
#include "resource.h"
#include "tdcstatic.h"
#include "tdcmsg.h"
#include "tdccustomattributeUIhelper.h"
#include "tdltaskicondlg.h"
#include "TDLTaskViewListBox.h"
#include "ToDoCtrlDataDefines.h"
#include "TDCTaskCompletion.h"

#include "..\shared\holdredraw.h"
#include "..\shared\datehelper.h"
#include "..\shared\enstring.h"
#include "..\shared\deferwndmove.h"
#include "..\shared\autoflag.h"
#include "..\shared\holdredraw.h"
#include "..\shared\osversion.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\filemisc.h"
#include "..\shared\icon.h"
#include "..\shared\ScopedTimer.h"
#include "..\shared\DlgUnits.h"

#include "..\3rdparty\dibdata.h"
#include "..\3rdparty\GdiPlus.h"

#include "..\Interfaces\uiextensionhelper.h"
#include "..\Interfaces\uiextensionmgr.h"
#include "..\Interfaces\iuiextension.h"
#include "..\Interfaces\Preferences.h"

#include <math.h>

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

#ifndef LVS_EX_DOUBLEBUFFER
#define LVS_EX_DOUBLEBUFFER 0x00010000
#endif

#ifndef LVS_EX_LABELTIP
#define LVS_EX_LABELTIP     0x00004000
#endif

//////////////////////////////////////////////////////////////////////

const UINT SORTWIDTH      = 10;
const UINT DEFTEXTFLAGS   = (DT_END_ELLIPSIS | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

//////////////////////////////////////////////////////////////////////

UINT CTabbedToDoCtrl::WM_TDC_RESTORELASTTASKVIEW = (WM_TDC_RECREATERECURRINGTASK + 1);

//////////////////////////////////////////////////////////////////////

CStringArray CTabbedToDoCtrl::s_aDefTaskViews;

//////////////////////////////////////////////////////////////////////

VIEWDATA::VIEWDATA()
	:
	bNeedResort(FALSE),
	pExtension(NULL),
	bNeedFullTaskUpdate(TRUE),
	bNeedFontUpdate(TRUE),
	bHasSelectedTask(FALSE),
	bCanPrepareNewTask(-1)
{
}

VIEWDATA::~VIEWDATA() 
{
}

BOOL VIEWDATA::WantAttribute(TDC_ATTRIBUTE nAttribID) const
{
	if ((nAttribID == TDCA_ALL) && !mapWantedAttrib.IsEmpty())
		return TRUE;

	return mapWantedAttrib.Has(nAttribID);
}

BOOL VIEWDATA::WantAnyAttribute(const CTDCAttributeMap& other) const
{
	if (mapWantedAttrib.IsEmpty())
		return FALSE;

	if (other.Has(TDCA_ALL) || other.Has(TDCA_NEWTASK))
	{
		ASSERT(other.HasOnly(TDCA_ALL) || other.HasOnly(TDCA_NEWTASK));
		return TRUE;
	}

	return mapWantedAttrib.MatchAny(other);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTabbedToDoCtrl::CTabbedToDoCtrl(CUIExtensionMgr& mgrUIExt, 
								 CTDLContentMgr& mgrContent, 
								 CShortcutManager& mgrShortcuts,
								 const CONTENTFORMAT& cfDefault, 
								 const TDCCOLEDITFILTERVISIBILITY& visDefault) 
	:
	CToDoCtrl(mgrContent, mgrShortcuts, cfDefault, visDefault), 
	m_bTreeNeedResort(FALSE),
	m_bUpdatingExtensions(FALSE),
	m_bRecreatingRecurringTasks(FALSE),
	m_nExtModifyingAttrib(TDCA_NONE),
	m_nListViewGroupBy(TDCC_NONE),
	m_dwListOptions(0),
	m_mgrUIExt(mgrUIExt),
	m_taskList(m_ilTaskIcons, m_data, TCF(), m_styles, m_tldAll, m_visColEdit.GetVisibleColumns(), m_aCustomAttribDefs)
{
	// add extra controls to implement list-view
	for (int nCtrl = 0; nCtrl < NUM_TTDCCTRLS; nCtrl++)
	{
		const TDCCONTROL& ctrl = TTDCCONTROLS[nCtrl];

		AddRCControl(_T("CONTROL"), ctrl.szClass, CString((LPCTSTR)ctrl.nIDCaption), 
					ctrl.dwStyle, ctrl.dwExStyle,
					ctrl.nX, ctrl.nY, ctrl.nCx, ctrl.nCy, ctrl.nID);
	}

	// tab is on by default
	m_styles[TDCS_SHOWTREELISTBAR] = TRUE;
	m_styles[TDCS_SHOWTASKVIEWTABCLOSEBUTTON] = TRUE;

	// Will be enabled on first showing
	m_taskList.EnableRecalcColumns(FALSE);
}

CTabbedToDoCtrl::~CTabbedToDoCtrl()
{
}

void CTabbedToDoCtrl::DoDataExchange(CDataExchange* pDX)
{
	CToDoCtrl::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TASKLISTTABCTRL, m_tabViews);
	DDX_Control(pDX, IDC_LISTVIEWGROUPBYATTRIB, m_cbListGroupBy);
	DDX_Control(pDX, IDC_LISTVIEWOPTIONS, m_cbListOptions);

	DDX_CBData(pDX, m_cbListGroupBy, m_nListViewGroupBy, TDCC_NONE);
	DDX_CheckItemData(pDX, m_cbListOptions, m_dwListOptions);
}

BEGIN_MESSAGE_MAP(CTabbedToDoCtrl, CToDoCtrl)
//{{AFX_MSG_MAP(CTabbedToDoCtrl)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_LISTVIEWGROUPBYATTRIB, OnListGroupBySelChanged)
	ON_CBN_SELCHANGE(IDC_LISTVIEWOPTIONS, OnListOptionsCheckChanged)

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TASKLISTCTRL, OnListSelChanged)
	ON_NOTIFY(NM_CLICK, IDC_TASKLISTCTRL, OnListClick)
	ON_NOTIFY(NM_RCLICK, IDC_TASKLISTTABCTRL, OnTabCtrlRClick)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TASKTREECTRL, OnTreeExpandItem)

	ON_REGISTERED_MESSAGE(WM_TDCN_VIEWPOSTCHANGE, OnPostTabViewChange)
	ON_REGISTERED_MESSAGE(WM_TDCN_VIEWPRECHANGE, OnPreTabViewChange)

	ON_REGISTERED_MESSAGE(WM_IUI_EDITSELECTEDTASKTITLE, OnUIExtEditSelectedTaskTitle)
	ON_REGISTERED_MESSAGE(WM_IUI_EDITSELECTEDTASKICON, OnUIExtEditSelectedTaskIcon)
	ON_REGISTERED_MESSAGE(WM_IUI_MODIFYSELECTEDTASK, OnUIExtModifySelectedTask)
	ON_REGISTERED_MESSAGE(WM_IUI_MOVESELECTEDTASK, OnUIExtMoveSelectedTask)
	ON_REGISTERED_MESSAGE(WM_IUI_SELECTTASK, OnUIExtSelectTask)
	ON_REGISTERED_MESSAGE(WM_IUI_SORTCHANGE, OnUIExtSortChange)
	ON_REGISTERED_MESSAGE(WM_IUI_DOHELP, OnUIExtDoHelp)
	ON_REGISTERED_MESSAGE(WM_IUI_GETTASKICON, OnUIExtGetTaskIcon)
	ON_REGISTERED_MESSAGE(WM_IUI_GETNEXTTASKOCCURRENCES, OnUIExtGetNextTaskOcurrences)
	ON_REGISTERED_MESSAGE(WM_IUI_SHOWFILELINK, OnUIExtShowFileLink)

	ON_REGISTERED_MESSAGE(WM_TLDT_DROP, OnDropObject)
	ON_REGISTERED_MESSAGE(WM_TLDT_CANDROP, OnCanDropObject)
	ON_REGISTERED_MESSAGE(WM_TDCM_GETTASKREMINDER, OnTDCGetTaskReminder)
	ON_REGISTERED_MESSAGE(WM_MIDNIGHT, OnMidnight)

	ON_MESSAGE(WM_TDC_RECREATERECURRINGTASK, OnRecreateRecurringTask)
	ON_MESSAGE(WM_TDC_RESTORELASTTASKVIEW, OnRestoreLastTaskView)

	ON_WM_DRAWITEM()
	ON_WM_ERASEBKGND()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////

void CTabbedToDoCtrl::SetDefaultTaskViews(const CStringArray& aTypeIDs)
{
	s_aDefTaskViews.Copy(aTypeIDs);
}

BOOL CTabbedToDoCtrl::OnInitDialog()
{
	CToDoCtrl::OnInitDialog();

	// create the list-list before anything else
	CRect rCtrl;
	VERIFY(GraphicsMisc::GetPrimaryMonitorScreenSpace(rCtrl));

	VERIFY(m_taskList.Create(this, rCtrl, IDC_TASKLISTCTRL, FALSE)); // !visible
	
	// list initialisation
	m_dtList.Register(&m_taskList.List(), this);
	m_taskList.SetWindowPos(&m_taskTree, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW);
	m_taskList.SetWindowPrompt(CEnString(IDS_TDC_TASKLISTPROMPT));

	// add tree and list as tabbed views
	CIcon icon;
	
	if (icon.Load(IDI_TASKTREE_STD))
		m_tabViews.AttachView(m_taskTree, FTCV_TASKTREE, CEnString(IDS_TASKTREE), icon, NULL);

	if (icon.Load(IDI_LISTVIEW_STD))
	{
		int nVertOffset = (GetCtrlRect(IDC_LISTVIEWGROUPBYATTRIB).bottom + CDlgUnits(this).ToPixelsY(5));
		m_tabViews.AttachView(m_taskList, FTCV_TASKLIST, CEnString(IDS_LISTVIEW), icon, NewViewData(), nVertOffset);
	}

	// Add rest of plugins as tabbed views
	for (int nExt = 0; nExt < m_mgrUIExt.GetNumUIExtensions(); nExt++)
		AddView(m_mgrUIExt.GetUIExtension(nExt));
	
	// Initialise the previously visible tabs
	SetVisibleTaskViews(s_aDefTaskViews);

	// Build the list-specific comboboxes
	BuildListGroupByCombo();
	BuildListOptionsCombo();

	return FALSE;
}

void CTabbedToDoCtrl::BuildListOptionsCombo()
{
	// once only
	if (!m_cbListOptions.GetCount())
	{
		m_cbListOptions.BuildCombo();

		m_dwListOptions = CTDLTaskListCtrlOptionsComboBox::LoadOptions(CPreferences(), GetPreferencesKey());
		m_cbListOptions.SetCheckedByItemData(m_dwListOptions);

		m_mgrPrompts.SetComboPrompt(m_cbListOptions, IDS_TDC_NONE);
	}
}

void CTabbedToDoCtrl::BuildListGroupByCombo()
{
	m_cbListGroupBy.ResetContent();

	AddString(m_cbListGroupBy, IDS_TDC_NONE, TDCC_NONE); // always

	for (int nCol = 0; nCol < NUM_COLUMNS; nCol++)
	{
		if (m_taskList.CanGroupBy(COLUMNS[nCol].nColID, TRUE))
			AddString(m_cbListGroupBy, COLUMNS[nCol].nIDLongName, COLUMNS[nCol].nColID);
	}
	
	for (int nAttrib = 0; nAttrib < m_aCustomAttribDefs.GetSize(); nAttrib++)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = m_aCustomAttribDefs[nAttrib];

		if (m_taskList.CanGroupBy(attribDef.GetColumnID(), TRUE))
		{
			CEnString sAttrib(IDS_CUSTOMCOLUMN, attribDef.sLabel);
			AddString(m_cbListGroupBy, sAttrib, attribDef.GetColumnID());
		}
	}
	
	if (SelectItemByData(m_cbListGroupBy, m_nListViewGroupBy) == CB_ERR)
	{
		m_nListViewGroupBy = TDCC_NONE;

		VERIFY(SelectItemByData(m_cbListGroupBy, m_nListViewGroupBy) != CB_ERR);
		m_taskList.SetGroupBy(m_nListViewGroupBy);
	}
}

void CTabbedToDoCtrl::OnListGroupBySelChanged()
{
	TDC_COLUMN nPrevGroupBy = m_nListViewGroupBy;
	UpdateData();

	if (m_nListViewGroupBy != nPrevGroupBy)
	{
		if (HasListOption(LVO_HIDENOGROUPVALUE))
		{
			// We need to rebuild the list because the <none>
			// group will be different for each attribute
			RebuildList(TRUE, m_nListViewGroupBy);
		}
		else
		{
			m_taskList.SetGroupBy(m_nListViewGroupBy);
		}
	}
}

void CTabbedToDoCtrl::OnListOptionsCheckChanged()
{
	ASSERT(InListView());
	DWORD dwPrevOptions = m_dwListOptions;

	UpdateData();

	DWORD dwNewOptions = m_dwListOptions;

	if (Misc::FlagHasChanged(LVO_SORTGROUPSASCENDING, dwPrevOptions, dwNewOptions))
	{
		m_taskList.SetSortGroupsAscending(HasListOption(LVO_SORTGROUPSASCENDING));

		// Remove sort option to simplify build option test
		Misc::SetFlag(dwPrevOptions, LVO_SORTGROUPSASCENDING, FALSE);
		Misc::SetFlag(dwNewOptions, LVO_SORTGROUPSASCENDING, FALSE);
	}

	// Ignore LVO_HIDEPARENTS if TDCS_ALWAYSHIDELISTPARENTS is enabled
	if (HasStyle(TDCS_ALWAYSHIDELISTPARENTS))
	{
		Misc::SetFlag(dwPrevOptions, LVO_HIDEPARENTS, FALSE);
		Misc::SetFlag(dwNewOptions, LVO_HIDEPARENTS, FALSE);
	}

	if (dwNewOptions != dwPrevOptions)
	{
		RebuildList();
	}
}

void CTabbedToDoCtrl::OnTreeExpandItem(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/)
{
	if (HasListOption(LVO_HIDECOLLAPSED))
	{
		if (InListView())
			RebuildList();
		else
			SetViewNeedsTaskUpdate(FTCV_TASKLIST);
	}
}

void CTabbedToDoCtrl::OnStylesUpdated(const CTDCStyleMap& styles)
{
	// Bypass base class to only allow resort if in tree view
	m_taskTree.OnStylesUpdated(styles, InTreeView());

	// ListView
	BOOL bListNeedsUpdate = styles.HasStyle(TDCS_ALWAYSHIDELISTPARENTS);

	if (bListNeedsUpdate)
	{
		// Remove or restore 'List View' combo option 
		if (HasStyle(TDCS_ALWAYSHIDELISTPARENTS))
			m_cbListOptions.RemoveOptions(LVO_HIDEPARENTS);
		else
			m_cbListOptions.RemoveOptions(0);

		// This style change has NO effect if LVO_HIDEPARENTS is enabled
		if (HasListOption(LVO_HIDEPARENTS))
			bListNeedsUpdate = FALSE;
	}

	// Only allow resort if in ListView AND NOT rebuilding
	m_taskList.OnStylesUpdated(styles, (InListView() && !bListNeedsUpdate));

	if (bListNeedsUpdate)
	{
		if (InListView())
			RebuildList();
		else
			SetViewNeedsTaskUpdate(FTCV_TASKLIST);
	}
}

void CTabbedToDoCtrl::OnTaskIconsChanged()
{
	CToDoCtrl::OnTaskIconsChanged();
	
	m_taskList.OnImageListChange();
}

LRESULT CTabbedToDoCtrl::OnTDCGetTaskReminder(WPARAM wp, LPARAM lp)
{
	UNREFERENCED_PARAMETER(wp);
	ASSERT(lp && (((HWND)wp == m_taskTree.GetSafeHwnd()) || 
					((HWND)wp == m_taskList.GetSafeHwnd())));
	
	// Base class always expects to get this from the Task Tree
	return CToDoCtrl::OnTDCGetTaskReminder((WPARAM)m_taskTree.GetSafeHwnd(), lp);
}

BOOL CTabbedToDoCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// see if an UI extension wants this
	FTC_VIEW nView = GetTaskView();
	
	switch (nView)
	{
	case FTCV_TASKLIST:
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		break;
		
	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			// Default processing
			IUIExtensionWindow* pExtWnd = GetExtensionWnd(nView);
			ASSERT(pExtWnd);

			if (CToolTipCtrlEx::WantMessage(pMsg->message))
				pExtWnd->FilterToolTipMessage(pMsg);

			BOOL bHasFocus = CDialogHelper::IsChildOrSame(pExtWnd->GetHwnd(), ::GetFocus());

			if (bHasFocus && pExtWnd->ProcessMessage(pMsg))
				return TRUE;

#if _MSC_VER <= 1200
			// Special case: If this came from a plugin and we ARE compiled 
			// with VC6 then MFC will not generate a WM_CONTEXTMENU msg 
			// from a right-button click, so we handle that.
			if ((pMsg->message == WM_RBUTTONUP) && IsExtensionView(pMsg->hwnd))
			{
				CPoint ptScreen(pMsg->lParam);
				::ClientToScreen(pMsg->hwnd, &ptScreen);

				if (pExtWnd->HitTest(ptScreen) != IUI_NOWHERE)
				{
					SendMessage(WM_CONTEXTMENU, pMsg->wParam, MAKELPARAM(ptScreen.x, ptScreen.y));
					return TRUE;
				}
			}
#endif
		}
		break;
		
	default:
		ASSERT(0);
	}

	return CToDoCtrl::PreTranslateMessage(pMsg);
}

void CTabbedToDoCtrl::SetUITheme(const CUIThemeFile& theme)
{
	CToDoCtrl::SetUITheme(theme);

	m_tabViews.SetBackgroundColor(theme.crAppBackLight);
	m_taskList.SetSplitBarColor(theme.crAppBackDark);

	// update extensions
	int nExt = m_aExtViews.GetSize();
			
	while (nExt--)
	{
		IUIExtensionWindow* pExtWnd = m_aExtViews[nExt];
		
		if (pExtWnd)
			pExtWnd->SetUITheme(&theme);
	}
}

void CTabbedToDoCtrl::EndSelectedTaskEdit()
{
	CToDoCtrl::EndSelectedTaskEdit();

	if (InExtensionView())
		SyncExtensionSelectionToTree(GetTaskView());
}

BOOL CTabbedToDoCtrl::LoadTasks(const CTaskFile& tasks)
{
	// Prevent certain operations being performed until 
	// after the views have been updated
	{
		CAutoFlag af(m_bLoadingTasks, TRUE);

		if (!CToDoCtrl::LoadTasks(tasks))
			return FALSE;
	}

	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_UNSET:
		LoadState(); // reload last view
		break;

	case FTCV_TASKTREE:
		SetViewNeedsTaskUpdate(FTCV_TASKLIST, TRUE);
		SetExtensionsNeedTaskUpdate(TRUE);
		break;

	case FTCV_TASKLIST:
		RebuildList();
		SetExtensionsNeedTaskUpdate(TRUE);
		break;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			CWaitCursor cursor;

			VIEWDATA* pVData = NULL;
			IUIExtensionWindow* pExtWnd = NULL;

			if (!GetExtensionWnd(nView, pExtWnd, pVData))
				return FALSE;

			pVData->bNeedFullTaskUpdate = FALSE;

			UpdateExtensionView(pExtWnd, tasks, IUI_ALL);
			RefreshExtensionViewSort(nView);

			SetExtensionsNeedTaskUpdate(TRUE, nView);
			SyncExtensionSelectionToTree(nView);
		}
		break;
	}

	UpdateSelectedTaskPath();
	m_taskList.SetTasklistFolder(FileMisc::GetFolderFromFilePath(m_sLastSavePath));

	return TRUE;
}

void CTabbedToDoCtrl::LoadState()
{
	CPreferences prefs;
	CString sKey = GetPreferencesKey(); // no subkey

	// restore view visibility
	CStringArray aTypeIDs;
	m_mgrUIExt.GetExtensionTypeIDs(aTypeIDs);

	if (prefs.GetProfileInt(sKey, _T("ListViewVisible"), TRUE))
		aTypeIDs.Add(LISTVIEW_TYPE);

	// remove hidden extensions from list of all extensions
	// this ensures that new extensions always appear first time
	int nExt = prefs.GetProfileInt(sKey, _T("HiddenExtensionCount"), -1);

	if (nExt >= 0)
	{
		while (nExt--)
		{
			CString sSubKey = Misc::MakeKey(_T("HiddenExt%d"), nExt);
			CString sTypeID = prefs.GetProfileString(sKey, sSubKey);

			Misc::RemoveItem(sTypeID, aTypeIDs);
		}

		SetVisibleTaskViews(aTypeIDs);
	}
	else
	{
		SetVisibleTaskViews(s_aDefTaskViews);
	}

	// Lisview options
	m_dwListOptions = CTDLTaskListCtrlOptionsComboBox::LoadOptions(prefs, sKey);
	m_cbListOptions.SetCheckedByItemData(m_dwListOptions);

	m_nListViewGroupBy = prefs.GetProfileEnum(sKey, _T("ListViewGroupBy"), TDCC_NONE);
	m_taskList.SetGroupBy(m_nListViewGroupBy, HasListOption(LVO_SORTGROUPSASCENDING));

	// Last active view
	FTC_VIEW nCurView = GetTaskView();
	FTC_VIEW nView = (FTC_VIEW)prefs.GetProfileInt(sKey, _T("View"), FTCV_UNSET);

	// Under high DPI displays (For reasons I don't understand) the plugin views 
	// which derive from CDialog have the vertical positions of their controls 
	// messed up unless we delay their creation
	if (GraphicsMisc::WantDPIScaling())
		PostMessage(WM_TDC_RESTORELASTTASKVIEW, nCurView, nView);
	else
		SendMessage(WM_TDC_RESTORELASTTASKVIEW, nCurView, nView);

	// clear the view so we don't keep restoring it
	prefs.WriteProfileInt(sKey, _T("View"), FTCV_UNSET);
}

void CTabbedToDoCtrl::SaveState()
{
	CPreferences prefs;
	CString sKey = GetPreferencesKey(); // no subkey
	
	// save view
	if (GetTaskView() != FTCV_UNSET)
		prefs.WriteProfileInt(sKey, _T("View"), GetTaskView());

	// save listview state
	prefs.WriteProfileInt(sKey, _T("ListViewVisible"), IsListViewTabShowing());
	prefs.WriteProfileInt(sKey, _T("ListViewGroupBy"), m_nListViewGroupBy);
	
	CTDLTaskListCtrlOptionsComboBox::SaveOptions(m_dwListOptions, prefs, sKey);

	// save hidden extensions
	CStringArray aVisTypeIDs, aTypeIDs;

	m_mgrUIExt.GetExtensionTypeIDs(aTypeIDs);
	GetVisibleTaskViews(aVisTypeIDs);

	// remove visible items to leave hidden ones
	Misc::RemoveItems(aVisTypeIDs, aTypeIDs);

	int nExt = aTypeIDs.GetSize();

	prefs.WriteProfileInt(sKey, _T("HiddenExtensionCount"), nExt);

	while (nExt--)
	{
		CString sSubKey = Misc::MakeKey(_T("HiddenExt%d"), nExt);
		prefs.WriteProfileString(sKey, sSubKey, aTypeIDs[nExt]);
	}

	// extension preferences
	SaveAllTaskViewPreferences();
}

void CTabbedToDoCtrl::SaveAllTaskViewPreferences()
{
	CPreferences prefs;
	CString sKey = GetPreferencesKey(); // no subkey

	// save view
	int nExt = m_aExtViews.GetSize();

	while (nExt--)
	{
		IUIExtensionWindow* pExtWnd = m_aExtViews[nExt];

		if (pExtWnd)
		{
			CString sKey = GetPreferencesKey(GetExtensionPrefsSubKey(pExtWnd));
			pExtWnd->SavePreferences(prefs, sKey);

			// Save sort state
			FTC_VIEW nExtView = GetExtensionView(nExt);

			const VIEWDATA* pVData = GetViewData(nExtView);
			ASSERT(pVData);

			if (pVData)
				pVData->sort.SaveState(prefs, sKey);
		}
	}
}

void CTabbedToDoCtrl::OnDestroy() 
{
	if (!m_bDelayLoaded)
		SaveState();

	// Destroy the extensions
	int nExt = m_aExtViews.GetSize();

	while (nExt--)
	{
		IUIExtensionWindow* pExtWnd = m_aExtViews[nExt];

		if (pExtWnd)
			::DestroyWindow(pExtWnd->GetHwnd());
	}
	m_aExtViews.RemoveAll();
			
	CToDoCtrl::OnDestroy();
}

void CTabbedToDoCtrl::UpdateVisibleColumns(const CTDCColumnIDMap& mapChanges)
{
	CToDoCtrl::UpdateVisibleColumns(mapChanges);

	m_taskList.OnColumnVisibilityChange(mapChanges);

	// See if we need to rebuild the group-by combo
	POSITION pos = mapChanges.GetStartPosition();

	while (pos)
	{
		if (m_taskList.CanGroupBy(mapChanges.GetNext(pos), FALSE))
		{
			BuildListGroupByCombo();
			break;
		}
	}
}

IUIExtensionWindow* CTabbedToDoCtrl::GetExtensionWnd(FTC_VIEW nView) const
{
	if (!IsExtensionView(nView))
	{
		ASSERT(0);
		return NULL;
	}

	int nExtension = (nView - FTCV_FIRSTUIEXTENSION);
	ASSERT(nExtension < m_aExtViews.GetSize());

	IUIExtensionWindow* pExtWnd = m_aExtViews[nExtension];
	ASSERT(pExtWnd || (m_tabViews.GetViewHwnd(nView) == NULL));

	return pExtWnd;
}

BOOL CTabbedToDoCtrl::GetExtensionWnd(FTC_VIEW nView, IUIExtensionWindow*& pExtWnd, VIEWDATA*& pVData) const
{
	pExtWnd = GetExtensionWnd(nView);

	if (!pExtWnd)
		return FALSE;

	pVData = GetViewData(nView);

	if (!pVData)
	{
		pExtWnd = NULL;
		return FALSE;
	}

	return TRUE;
}

IUIExtensionWindow* CTabbedToDoCtrl::GetCreateExtensionWnd(FTC_VIEW nView)
{
	if (!IsExtensionView(nView))
	{
		ASSERT(0);
		return NULL;
	}

	// try for existing first
	IUIExtensionWindow* pExtWnd = GetExtensionWnd(nView);

	if (pExtWnd)
		return pExtWnd;

	// sanity checks
	ASSERT(m_tabViews.GetViewHwnd(nView) == NULL);

	VIEWDATA* pVData = GetViewData(nView);

	if (!pVData)
		return NULL;
	
	// Create the extension window
	int nExtension = (nView - FTCV_FIRSTUIEXTENSION);
	UINT nCtrlID = (IDC_EXTENSIONWINDOW1 + nExtension);

	try
	{
		pExtWnd = pVData->pExtension->CreateExtWindow(nCtrlID, WS_CHILD, 0, 0, 0, 200, GetSafeHwnd());
	}
	catch (...)
	{
		pExtWnd = NULL;
	}
	
	if (pExtWnd == NULL)
		return NULL;
	
	HWND hWnd = pExtWnd->GetHwnd();
	ASSERT (hWnd);
	
	if (!hWnd)
		return NULL;
	
	pExtWnd->SetUITheme(&m_theme);
	pExtWnd->SetReadOnly(HasStyle(TDCS_READONLY) != FALSE);

	// update focus first because initializing views can take time
	::SetFocus(hWnd);
	
	// For automation
	::SetWindowText(hWnd, pExtWnd->GetTypeID());

	// Save off
	m_aExtViews[nExtension] = pExtWnd;

	// Set font before loading preferences
	pExtWnd->SetTaskFont(m_taskTree.GetFont());
	pVData->bNeedFontUpdate = FALSE;
		
	// restore state
	CPreferences prefs;
	CString sKey = GetPreferencesKey(GetExtensionPrefsSubKey(pExtWnd));
	
	pExtWnd->LoadPreferences(prefs, sKey, false);

	// sort state
	pVData->bNeedResort = pVData->sort.LoadState(prefs, sKey);
		
	// and update tab control with our new HWND
	m_tabViews.SetViewHwnd((FTC_VIEW)nView, hWnd);
	
	// initialize update state
	pVData->bNeedFullTaskUpdate = TRUE;

	// and capabilities
	if (pVData->bCanPrepareNewTask == -1)
	{
		CTaskFile task;
		VERIFY(task.NewTask(_T("Test Task"), NULL, 0, 0));

		pVData->bCanPrepareNewTask = pExtWnd->PrepareNewTask(&task);
	}
	
	PopulateExtensionViewAttributes(pExtWnd, pVData);

	// insert the view after the list in z-order
	::SetWindowPos(pExtWnd->GetHwnd(), m_taskList, 0, 0, 0, 0, (SWP_NOMOVE | SWP_NOSIZE));
	
	Invalidate();

	return pExtWnd;
}

CString CTabbedToDoCtrl::GetExtensionPrefsSubKey(const IUIExtensionWindow* pExtWnd)
{
	CString sSubKey;
	sSubKey.Format(_T("UIExtensions\\%s"), pExtWnd->GetTypeID());

	return sSubKey;
}

LRESULT CTabbedToDoCtrl::OnPreTabViewChange(WPARAM nOldTab, LPARAM nNewTab) 
{
	EndLabelEdit(FALSE);
	
	if (!HandleUnsavedComments())
		return 1L; // prevent tab change

	// notify parent
	GetParent()->SendMessage(WM_TDCN_VIEWPRECHANGE, nOldTab, nNewTab);

	// make sure something is selected in the tree
	if (GetSelectedTaskCount() == 0)
	{
		HTREEITEM hti = m_taskTree.GetSelectedItem();
		
		if (!hti)
			hti = m_taskTree.GetChildItem(NULL);
		
		if (hti && !m_taskTree.IsItemSelected(hti))
			CToDoCtrl::SelectTask(GetTaskID(hti), FALSE);
	}

	// take a note of what task is currently singly selected
	// so that we can prevent unnecessary calls to UpdateControls
	DWORD dwSelTaskID = GetSingleSelectedTaskID();
	FTC_VIEW nNewView = (FTC_VIEW)nNewTab;
	FTC_VIEW nOldView = (FTC_VIEW)nOldTab;
	
	switch (nNewView)
	{
	case FTCV_TASKTREE:
		{
			// update sort
			if (m_bTreeNeedResort)
			{
				m_bTreeNeedResort = FALSE;
				CToDoCtrl::Resort();
			}
			
			m_taskTree.EnsureSelectionVisible(TRUE);
		}
		break;

	case FTCV_TASKLIST:
		{
			// Update tasks
			VIEWDATA* pLVData = GetViewData(FTCV_TASKLIST);

			if (pLVData->bNeedFullTaskUpdate)
			{
				RebuildList();
			}
			else if (pLVData->bNeedResort)
			{
				ResortList();
			}

			if (pLVData->bNeedFontUpdate)
			{
				pLVData->bNeedFontUpdate = FALSE;
				m_taskList.SetFont(m_taskTree.GetFont());
			}

			// This does nothing if already enabled
			m_taskList.EnableRecalcColumns(TRUE);
		}
		break;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			VIEWDATA* pVData = GetViewData(nNewView);

			if (!pVData)
				return 1L; // prevent tab change

			// start progress if initializing from another view, 
			// will be cleaned up in OnPostTabViewChange
			UINT nProgressMsg = 0;

			if (nOldView != -1)
			{
				if (GetExtensionWnd(nNewView) == NULL)
				{
					nProgressMsg = IDS_INITIALISINGTABBEDVIEW;
				}
				else if (pVData->bNeedFullTaskUpdate)
				{
					nProgressMsg = IDS_UPDATINGTABBEDVIEW;
				}

				if (nProgressMsg)
					BeginExtensionProgress(pVData, nProgressMsg);
			}

			CWaitCursor cursor;
			IUIExtensionWindow* pExtWnd = GetCreateExtensionWnd(nNewView);

			if (!pExtWnd || !pExtWnd->GetHwnd())
			{
				ASSERT(0);
				EndExtensionProgress();

				// Notify User
				// TODO

				m_tabViews.ShowViewTab(nNewView, FALSE);

				return 1L; // prevent tab change
			}
			
			if (pVData->bNeedFullTaskUpdate)
			{
				pVData->bNeedFullTaskUpdate = FALSE;

				// start progress if not already
				// will be cleaned up in OnPostTabViewChange
				if (nProgressMsg == 0)
					BeginExtensionProgress(pVData);

				// Note: we want to update even if we have no tasks
				CTaskFile tasks;

				GetAllTasksForExtensionViewUpdate(pVData->mapWantedAttrib, tasks);
				UpdateExtensionView(pExtWnd, tasks, IUI_ALL);
			}

			if (pVData->bNeedFontUpdate)
			{
				pExtWnd->SetTaskFont(m_taskTree.GetFont());
				pVData->bNeedFontUpdate = FALSE;
			}

			if (pVData->bNeedResort)
				RefreshExtensionViewSort(nNewView);
		}
		break;
	}

	// update controls only if the selection has changed
	if (HasSingleSelectionChanged(dwSelTaskID))
		UpdateControls();

	return 0L; // allow tab change
}

LRESULT CTabbedToDoCtrl::OnPostTabViewChange(WPARAM nOldView, LPARAM nNewView)
{
	switch (nNewView)
	{
	case FTCV_TASKTREE:
		break;

	case FTCV_TASKLIST:
		SyncListSelectionToTree(FALSE);
		m_taskList.EnsureSelectionVisible(FALSE);
		break;
		
	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			// stop any progress
			GetParent()->SendMessage(WM_TDCM_LENGTHYOPERATION, FALSE);
			
			// resync selection
			SyncExtensionSelectionToTree((FTC_VIEW)nNewView);
		}
		break;
	}

	ShowListViewSpecificCtrls(nNewView == FTCV_TASKLIST);
	
	// If we are switching to/from a view with selection 
	// from/to a view without selection then update controls
	if (ViewHasTaskSelection((FTC_VIEW)nNewView) != ViewHasTaskSelection((FTC_VIEW)nOldView))
		UpdateControls();
	
	// notify parent
	GetParent()->SendMessage(WM_TDCN_VIEWPOSTCHANGE, nOldView, nNewView);
	
	return 0L;
}

void CTabbedToDoCtrl::ShowListViewSpecificCtrls(BOOL bShow)
{
	GetDlgItem(IDC_LISTVIEWGROUPBYLABEL)->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_LISTVIEWGROUPBYATTRIB)->ShowWindow(bShow ? SW_SHOW : SW_HIDE);

	GetDlgItem(IDC_LISTVIEWGROUPBYLABEL)->EnableWindow(bShow);
	GetDlgItem(IDC_LISTVIEWGROUPBYATTRIB)->EnableWindow(bShow);

	GetDlgItem(IDC_LISTVIEWOPTIONSLABEL)->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_LISTVIEWOPTIONS)->ShowWindow(bShow ? SW_SHOW : SW_HIDE);

	GetDlgItem(IDC_LISTVIEWOPTIONSLABEL)->EnableWindow(bShow);
	GetDlgItem(IDC_LISTVIEWOPTIONS)->EnableWindow(bShow);
}

int CTabbedToDoCtrl::GetTasks(CTaskFile& tasks, FTC_VIEW nView, const TDCGETTASKS& filter) const
{
	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			// Optimisation if only attribute part of filter is set
			if (!filter.IsSet(FALSE))
			{
				PrepareTaskfileForTasks(tasks, filter);
				AddTreeChildrenToTaskFile(NULL, tasks, NULL, filter);
			}
			else
			{
				CToDoCtrl::GetTasks(tasks, filter);
			}

			AddGlobalsToTaskFile(tasks, filter.mapAttribs);
			return tasks.GetTaskCount();
		}

	case FTCV_TASKLIST:	// Special case
		{
			PrepareTaskfileForTasks(tasks, filter);

			// we return exactly what's selected in the list and in the same order
			// so we make sure we don't include subtasks
			for (int nItem = 0; nItem < m_taskList.GetItemCount(); nItem++)
			{
				if (!m_taskList.IsGroupHeaderItem(nItem))
				{
					DWORD dwTaskID = GetTaskID(nItem);
					DWORD dwParentID = m_data.GetTaskParentID(dwTaskID);

					CToDoCtrl::AddTreeItemToTaskFile(NULL, dwTaskID, tasks, NULL, filter, FALSE, dwParentID);
				}
			}

			return tasks.GetTaskCount();
		}
	}
	
	ASSERT(0);
	return 0;
}

BOOL CTabbedToDoCtrl::GetAllTasksForExtensionViewUpdate(const CTDCAttributeMap& mapAttrib, CTaskFile& tasks) const
{
	TDCGETTASKS filter;
	filter.mapAttribs.Copy(mapAttrib);

	GetTasks(tasks, FTCV_TASKTREE, filter);

	return TRUE;
}

BOOL CTabbedToDoCtrl::AddTreeChildrenToTaskFile(HTREEITEM hti, CTaskFile& tasks, HTASKITEM hParentTask, const TDCGETTASKS& filter) const
{
	HTREEITEM htiChild = m_taskTree.GetChildItem(hti);
	HTASKITEM hPrevSiblingTask = NULL;
	int nChildren = 0;

	while (htiChild)
	{
		HTASKITEM hTask = AddTreeItemToTaskFile(htiChild, tasks, hParentTask, hPrevSiblingTask, filter);

		if (hTask == NULL)
		{
			ASSERT(0);
			return FALSE;
		}

		// next
		htiChild = m_taskTree.GetNextItem(htiChild);
		hPrevSiblingTask = hTask;
	}

	return TRUE;
}

HTASKITEM CTabbedToDoCtrl::AddTreeItemToTaskFile(HTREEITEM hti, CTaskFile& tasks, HTASKITEM hParentTask, HTASKITEM hPrevSiblingTask, const TDCGETTASKS& filter) const
{
	DWORD dwTaskID = GetTaskID(hti);
	ASSERT(!m_taskList.IsGroupHeaderTask(dwTaskID));

	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;

	if (!m_data.GetTask(dwTaskID, pTDI, pTDS))
		return FALSE;

	HTASKITEM hTask = NULL;
	
	if (hPrevSiblingTask == NULL)
		hTask = tasks.NewTask(pTDI->sTitle, hParentTask, dwTaskID, 0);
	else
		hTask = tasks.NewSiblingTask(pTDI->sTitle, hPrevSiblingTask, dwTaskID);

	if (!hTask)
	{
		ASSERT(0);
		return FALSE;
	}

	// Attributes
	m_exporter.ExportMatchingTaskAttributes(pTDI, pTDS, tasks, hTask, filter);

	// Subtasks
	AddTreeChildrenToTaskFile(hti, tasks, hTask, filter);

	return hTask;
}


int CTabbedToDoCtrl::GetSelectedTasksForExtensionViewUpdate(const CTDCAttributeMap& mapAttrib, 
															DWORD dwFlags, CTaskFile& tasks) const
{
	TDCGETTASKS filter;
	filter.dwFlags = dwFlags;

	CTDCAttributeMap mapAllAttribIDs;
	GetAllExtensionViewsWantedAttributes(mapAllAttribIDs);

	// Special cases
	if (mapAttrib.Has(TDCA_NEWTASK) || mapAttrib.Has(TDCA_ALL))
	{
		ASSERT(mapAttrib.HasOnly(TDCA_NEWTASK) || mapAttrib.HasOnly(TDCA_ALL));

		filter.mapAttribs.Copy(mapAllAttribIDs);
	}
	else
	{
		VERIFY(mapAttrib.GetIntersection(mapAllAttribIDs, filter.mapAttribs));
	}

	VERIFY(GetSelectedTasks(tasks, FTCV_TASKTREE, filter));

	// Globals
	if (mapAttrib.IsEmpty() || mapAttrib.Has(TDCA_NEWTASK))
		AddGlobalsToTaskFile(tasks, TDCA_ALL);
	else
		AddGlobalsToTaskFile(tasks, mapAttrib);

	return tasks.GetTaskCount();
}

void CTabbedToDoCtrl::UpdateExtensionView(IUIExtensionWindow* pExtWnd, const CTaskFile& tasks, IUI_UPDATETYPE nType)
{
	ASSERT(!tasks.GetFilePath().IsEmpty() || m_sLastSavePath.IsEmpty());

	CAutoFlag af(m_bUpdatingExtensions, TRUE);
	
	pExtWnd->UpdateTasks(&tasks, nType);
}

DWORD CTabbedToDoCtrl::GetSingleSelectedTaskID() const
{
	if (GetSelectedTaskCount() == 1) 
		return GetTaskID(GetSelectedItem());

	// else
	return 0;
}

BOOL CTabbedToDoCtrl::HasSingleSelectionChanged(DWORD dwSelID) const
{
	// multi-selection
	if (GetSelectedTaskCount() != 1)
		return TRUE;

	// different selection
	if (GetTaskID(GetSelectedItem()) != dwSelID)
		return TRUE;

	// dwSelID is still the only selection
	return FALSE;
}

VIEWDATA* CTabbedToDoCtrl::GetViewData(FTC_VIEW nView) const
{
	VIEWDATA* pVData = (VIEWDATA*)m_tabViews.GetViewData(nView);

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		ASSERT(pVData == NULL);
		break;
		
	case FTCV_TASKLIST:
		ASSERT(pVData && !pVData->pExtension);
		break;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		ASSERT(pVData && pVData->pExtension);
		break;

	// all else
	default:
		ASSERT(0);
	}

	return pVData;
}

VIEWDATA* CTabbedToDoCtrl::GetActiveViewData() const
{
	return GetViewData(GetTaskView());
}

void CTabbedToDoCtrl::SetTaskView(FTC_VIEW nView) 
{
	// take a note of what task is currently singly selected
	// so that we can prevent unnecessary calls to UpdateControls
	DWORD dwSelTaskID = GetSingleSelectedTaskID();
	
	if (!m_tabViews.SetActiveView(nView, TRUE))
		return;

	// update controls only if the selection has changed and 
	if (HasSingleSelectionChanged(dwSelTaskID))
		UpdateControls();
}

void CTabbedToDoCtrl::SetNextTaskView() 
{
	// take a note of what task is currently singly selected
	// so that we can prevent unnecessary calls to UpdateControls
	DWORD dwSelTaskID = GetSingleSelectedTaskID();
	
	m_tabViews.ActivateNextView();

	// update controls only if the selection has changed and 
	if (HasSingleSelectionChanged(dwSelTaskID))
		UpdateControls();
}

CString CTabbedToDoCtrl::GetTaskViewName() const
{
	FTC_VIEW nView = GetTaskView();

	if (nView == FTCV_UNSET)
		nView = FTCV_TASKTREE;
	
	return m_tabViews.GetViewName(nView);
}

LRESULT CTabbedToDoCtrl::OnUIExtSelectTask(WPARAM wParam, LPARAM lParam)
{
	if (m_bUpdatingExtensions)
		return 0;

	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
	case FTCV_TASKLIST:
		ASSERT(0);
		return 0;

	default:
		break;
	}

	CDWordArray aTaskIDs;

	if (wParam == 0)
	{
		aTaskIDs.Add(lParam);
	}
	else
	{
		LPDWORD pIDs = (LPDWORD)wParam;

		for (int nID = 0; nID < (int)lParam; nID++)
			aTaskIDs.Add(pIDs[nID]);
	}

	VIEWDATA* pVData = GetActiveViewData();
	BOOL bHadSelectedTask = pVData->bHasSelectedTask, bSelChange = FALSE;

	if (aTaskIDs.GetSize() == 1)
	{
		DWORD dwTaskID = aTaskIDs[0];
		pVData->bHasSelectedTask = (dwTaskID != 0);

		if (dwTaskID)
		{
			if (HasSingleSelectionChanged(dwTaskID))
			{
				// Call base class directly so that we don't end
				// up calling back into extension this came from
				VERIFY(CToDoCtrl::SelectTask(dwTaskID, FALSE));
			}
			else
			{
				// Update if the extension previously did NOT have a selection
				bSelChange = !bHadSelectedTask;
			}
		}
		else
		{
			// Update if the extension previously had a selection
			bSelChange = bHadSelectedTask;
		}
	}
	else
	{
		pVData->bHasSelectedTask = TRUE;

		// Call base class directly so that we don't end
		// up calling back into extension this came from
		VERIFY(CToDoCtrl::SelectTasks(aTaskIDs));
	}

	if (bSelChange)
	{
		UpdateControls();
		NotifyParentSelectionChange();
	}

	return pVData->bHasSelectedTask;
}

LRESULT CTabbedToDoCtrl::OnUIExtSortChange(WPARAM wParam, LPARAM lParam)
{
	FTC_VIEW nView = GetTaskView();
	
	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
	case FTCV_TASKLIST:
	default:
		ASSERT(0);
		break;
		
	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			VIEWDATA* pVData = GetViewData(nView);
			ASSERT(pVData);

			if (pVData)
			{
				pVData->sort.single.nBy = TDC::MapAttributeToColumn((TDC_ATTRIBUTE)lParam);
				pVData->sort.single.bAscending = wParam;
			}
		}
		break;
	}

	return 0L;
}

LRESULT CTabbedToDoCtrl::OnUIExtDoHelp(WPARAM /*wParam*/, LPARAM lParam)
{
	FTC_VIEW nView = GetTaskView();
	
	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
	case FTCV_TASKLIST:
	default:
		ASSERT(0);
		break;
		
	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		AfxGetApp()->WinHelp(lParam);
		break;
	}

	return 0L;
}

LRESULT CTabbedToDoCtrl::OnUIExtGetTaskIcon(WPARAM wParam, LPARAM lParam)
{
	// Note: Queries from extensions are processed even if
	// an extension view is not active
	ASSERT(wParam && lParam);

	if (wParam && lParam)
	{
		DWORD dwTaskID = wParam;
		int* pImageIndex = (int*)lParam;

		*pImageIndex = m_taskTree.GetTaskIconIndex(dwTaskID);

		if (*pImageIndex != -1)
			return (LRESULT)m_ilTaskIcons.GetSafeHandle();
	}

	return 0L;
}

LRESULT CTabbedToDoCtrl::OnUIExtShowFileLink(WPARAM wParam, LPARAM lParam)
{
	// Note: Queries from extensions are processed even if
	// an extension view is not active
	LPCTSTR szFileLink = (LPCTSTR)lParam;

	if (szFileLink)
		GotoFile(szFileLink);
	else
		ASSERT(0);

	return 0L;
}

LRESULT CTabbedToDoCtrl::OnUIExtGetNextTaskOcurrences(WPARAM wParam, LPARAM lParam)
{
	// Note: Queries from extensions are processed even if
	// an extension view is not active
	ASSERT(wParam && lParam);

	if (wParam && lParam)
	{
		DWORD dwTaskID = wParam;
		IUINEXTTASKOCCURRENCES* pOccurrences = (IUINEXTTASKOCCURRENCES*)lParam;

		COleDateTimeRange dtRange;

		if (!dtRange.Set(CDateHelper::GetDate(pOccurrences->tRangeStart),
						 CDateHelper::GetDate(pOccurrences->tRangeEnd)))
		{
			ASSERT(0);
			return 0;
		}

		CArray<double, double&> aDates;
		BOOL bDueDate = FALSE;

		if (!m_data.CalcNextTaskOccurences(dwTaskID, dtRange, aDates, bDueDate))
		{
			return 0;
		}

		int nNumOccur = aDates.GetSize();
		nNumOccur = min(nNumOccur, IUI_MAXNEXTOCCURRENCES);

		COleDateTime dtCur = m_data.GetTaskDate(dwTaskID, (bDueDate ? TDCD_DUE : TDCD_START));

		TDCRECURRENCE tr;
		VERIFY(m_data.GetTaskRecurrence(dwTaskID, tr));

		for (int nOccur = 0; nOccur < nNumOccur; nOccur++)
		{
			const double dDate = aDates[nOccur];
			int nOffset = (int)(dDate - dtCur.m_dt);

			IUINEXTTASKOCCURRENCES::IUITASKOCCURRENCE& occur = pOccurrences->occurrences[nOccur];

			if (bDueDate)
			{
				VERIFY(CDateHelper::GetTimeT64(dDate, occur.tEnd));

				COleDateTime dtNewStart = m_data.GetTaskDate(dwTaskID, TDCD_START);
				VERIFY(CDateHelper().OffsetDate(dtNewStart, nOffset, DHU_DAYS));

				// Note: Don't fit start date to recurring scheme
				VERIFY(CDateHelper::GetTimeT64(dtNewStart, occur.tStart));

				ASSERT((occur.tStart <= occur.tEnd) ||
					(CDateHelper::IsSameDay(dDate, dtNewStart) && !CDateHelper::DateHasTime(dDate)));
			}
			else // start date
			{
				VERIFY(CDateHelper::GetTimeT64(dDate, occur.tStart));

				COleDateTime dtNewDue = m_data.GetTaskDate(dwTaskID, TDCD_DUE);
				VERIFY(CDateHelper().OffsetDate(dtNewDue, nOffset, DHU_DAYS));

				// Note: Don't fit due date to recurring scheme
				VERIFY(CDateHelper::GetTimeT64(dtNewDue, occur.tEnd));

				ASSERT((occur.tStart <= occur.tEnd) ||
					(CDateHelper::IsSameDay(dDate, dtNewDue) && !CDateHelper::DateHasTime(dtNewDue)));
			}
		}

		pOccurrences->nNumOccurrences = nNumOccur;
		return TRUE;
	}

	return 0L;
}

LRESULT CTabbedToDoCtrl::OnUIExtEditSelectedTaskTitle(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return EditSelectedTaskTitle();
}

LRESULT CTabbedToDoCtrl::OnUIExtEditSelectedTaskIcon(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return EditSelectedTaskIcon();
}

BOOL CTabbedToDoCtrl::CanEditSelectedTask(TDC_ATTRIBUTE nAttrib, DWORD dwTaskID) const
{
	if (!CToDoCtrl::CanEditSelectedTask(nAttrib, dwTaskID))
		return FALSE;

	if (GetUpdateControlsItem() == NULL)
		return !(CTDCAttributeMap::IsTaskAttribute(nAttrib) || (nAttrib == TDCA_DELETE));

	return TRUE;
}

BOOL CTabbedToDoCtrl::CanEditSelectedTask(const IUITASKMOD& mod, DWORD& dwTaskID) const
{
	dwTaskID = mod.dwSelectedTaskID;

	if (!CanEditSelectedTask(mod.nAttrib, dwTaskID))
		return FALSE;

	if (dwTaskID && (GetSelectedTaskCount() == 1))
	{
		ASSERT(GetSelectedTaskID() == dwTaskID);
		dwTaskID = 0; // same as 'selected'
	}

	return TRUE;
}

BOOL CTabbedToDoCtrl::SplitSelectedTask(int nNumSubtasks)
{
	if (!CToDoCtrl::SplitSelectedTask(nNumSubtasks))
		return FALSE;

	if (InListView())
		RebuildList();

	return TRUE;
}

BOOL CTabbedToDoCtrl::ProcessUIExtensionMod(const IUITASKMOD& mod, CDWordArray& aModTaskIDs, 
											CTDCTaskCompletionArray& aTasksForCompletion, CTDCAttributeMap& mapModAttribs)
{
	DWORD dwTaskID = mod.dwSelectedTaskID;

	if (!CanEditSelectedTask(mod, dwTaskID))
	{
		ASSERT(0);
		return 0;
	}

	ASSERT((dwTaskID == 0) || (GetSelectedTaskCount() > 1));

	CStringArray aValues;
	DWORD dwResults = 0; 
	BOOL bChange = FALSE;
	
	// prevent the active view's change being propagated back to itself
	m_nExtModifyingAttrib = mod.nAttrib;

	switch (mod.nAttrib)
	{
	case TDCA_TASKNAME:		
		{
			if (dwTaskID)
				bChange = (SET_CHANGE == m_data.SetTaskTitle(dwTaskID, mod.szValue));
			else
				bChange = SetSelectedTaskTitle(mod.szValue, TRUE);
		}
		break;

	case TDCA_PRIORITY:		
		{
			if (dwTaskID)
				bChange = (SET_CHANGE == m_data.SetTaskPriority(dwTaskID, mod.nValue));
			else
				bChange = SetSelectedTaskPriority(mod.nValue);
		}
		break;

	case TDCA_COLOR: 		
		{
			if (dwTaskID)
				bChange = (SET_CHANGE == m_data.SetTaskColor(dwTaskID, mod.crValue));
			else
				bChange = SetSelectedTaskColor(mod.crValue);
		}
		break;

	case TDCA_ALLOCBY:		
		{
			if (dwTaskID)
				bChange = (SET_CHANGE == m_data.SetTaskAllocBy(dwTaskID, mod.szValue));
			else
				bChange = SetSelectedTaskAllocBy(mod.szValue);
		}
		break;

	case TDCA_STATUS:		
		{
			if (dwTaskID)
			{
				if (!aTasksForCompletion.Add(dwTaskID, mod.szValue))
				{
					bChange = (SET_CHANGE == m_data.SetTaskStatus(dwTaskID, mod.szValue));
				}
			}
			else
			{
				bChange = SetSelectedTaskStatus(mod.szValue);
			}
		}
		break;

	case TDCA_PERCENT:		
		{
			if (dwTaskID)
			{
				if (!aTasksForCompletion.Add(dwTaskID, mod.nValue))
				{
					bChange = (SET_CHANGE == m_data.SetTaskPercent(dwTaskID, mod.nValue));
				}
			}
			else
			{
				bChange = SetSelectedTaskPercentDone(mod.nValue);
			}
		}
		break;

	case TDCA_TIMEESTIMATE:		
		{
			TDCTIMEPERIOD time(mod.dValue, mod.nTimeUnits);

			if (dwTaskID)
				bChange = (SET_CHANGE == m_data.SetTaskTimeEstimate(dwTaskID, time));
			else
				bChange = SetSelectedTaskTimeEstimate(time);
		}
		break;

	case TDCA_TIMESPENT:		
		{
			TDCTIMEPERIOD time(mod.dValue, mod.nTimeUnits);

			if (dwTaskID)
				bChange = (SET_CHANGE == m_data.SetTaskTimeSpent(dwTaskID, time));
			else
				bChange = SetSelectedTaskTimeSpent(time);
		}
		break;

	case TDCA_COMMENTS:		
		{
			if (dwTaskID)
				bChange = (SET_CHANGE == m_data.SetTaskComments(dwTaskID, mod.szValue));
			else
				bChange = SetSelectedTaskComments(mod.szValue);
		}
		break;

	case TDCA_FLAG:			
		{
			if (dwTaskID)
				bChange = (SET_CHANGE == m_data.SetTaskFlag(dwTaskID, mod.bValue));
			else
				bChange = SetSelectedTaskFlag(mod.bValue);
		}
		break;

	case TDCA_RISK:			
		{
			if (dwTaskID)
				bChange = (SET_CHANGE == m_data.SetTaskRisk(dwTaskID, mod.nValue));
			else
				bChange = SetSelectedTaskRisk(mod.nValue);
		}
		break;

	case TDCA_EXTERNALID: 	
		{
			if (dwTaskID)
				bChange = (SET_CHANGE == m_data.SetTaskExternalID(dwTaskID, mod.szValue));
			else
				bChange = SetSelectedTaskExternalID(mod.szValue);
		}
		break;

	case TDCA_COST:			
		{
			TDCCOST cost(mod.dValue, (mod.bCostIsRate ? TRUE : FALSE));

			if (dwTaskID)
				bChange = (SET_CHANGE == m_data.SetTaskCost(dwTaskID, cost));
			else
				bChange = SetSelectedTaskCost(cost);
		}
		break;

	case TDCA_VERSION:		
		{
			if (dwTaskID)
				bChange = (SET_CHANGE == m_data.SetTaskVersion(dwTaskID, mod.szValue));
			else
				bChange = SetSelectedTaskVersion(mod.szValue);
		}
		break;

	case TDCA_DONEDATE:		
		{
			COleDateTime date(CDateHelper::GetDate(mod.tValue));

			if (dwTaskID)
				VERIFY(aTasksForCompletion.Add(dwTaskID, date));
			else
				bChange = SetSelectedTaskDate(TDCD_DONE, date);
		}
		break;

	case TDCA_STARTDATE:		
		{
			COleDateTime date(CDateHelper::GetDate(mod.tValue));

			if (dwTaskID)
				bChange = (SET_CHANGE == m_data.SetTaskDate(dwTaskID, TDCD_START, date));
			else
				bChange = SetSelectedTaskDate(TDCD_START, date);
		}
		break;

	case TDCA_DUEDATE:		
		{
			COleDateTime date(CDateHelper::GetDate(mod.tValue));

			if (dwTaskID)
				bChange = (SET_CHANGE == m_data.SetTaskDate(dwTaskID, TDCD_DUE, date));
			else
				bChange = SetSelectedTaskDate(TDCD_DUE, date);
		}
		break;
		
	case TDCA_ALLOCTO:
		{
			Misc::Split(mod.szValue, aValues, '\n');

			if (dwTaskID)
				bChange = (SET_CHANGE == m_data.SetTaskAllocTo(dwTaskID, aValues, FALSE));
			else 
				bChange = SetSelectedTaskAllocTo(aValues);

		}
		break;
		
	case TDCA_CATEGORY:
		{
			Misc::Split(mod.szValue, aValues, '\n');

			if (dwTaskID)
				bChange = (SET_CHANGE == m_data.SetTaskCategories(dwTaskID, aValues, FALSE));
			else 
				bChange = SetSelectedTaskCategories(aValues);
		}
		break;
		
	case TDCA_TAGS:
		{
			Misc::Split(mod.szValue, aValues, '\n');

			if (dwTaskID)
				bChange = (SET_CHANGE == m_data.SetTaskTags(dwTaskID, aValues, FALSE));
			else 
				bChange = SetSelectedTaskTags(aValues);
		}
		break;
		
	case TDCA_FILELINK:
		{
			Misc::Split(mod.szValue, aValues, '\n');

			if (dwTaskID)
				bChange = (SET_CHANGE == m_data.SetTaskFileLinks(dwTaskID, aValues, FALSE));
			else 
				bChange = SetSelectedTaskFileLinks(aValues);
		}
		break;
		
	case TDCA_DEPENDENCY: 
		if (dwTaskID != 0)
		{
			// Only support singly selected tasks
			ASSERT(0);
		}
		else
		{
			ASSERT(GetSingleSelectedTaskID());

			CTDCDependencyArray aDepends;
			GetSelectedTaskDependencies(aDepends);

			if (mod.dwDependID && !mod.dwPrevDependID)
			{
				VERIFY(aDepends.Add(mod.dwDependID));
			}
			else if (!mod.dwDependID && mod.dwPrevDependID)
			{
				VERIFY(aDepends.RemoveLocalDependency(mod.dwPrevDependID));
			}
			else if (mod.dwDependID && mod.dwPrevDependID)
			{
				VERIFY(aDepends.ReplaceLocalDependency(mod.dwPrevDependID, mod.dwDependID));
			}
			else
			{
				ASSERT(0);
			}

			bChange = SetSelectedTaskDependencies(aDepends);
		}
		break;
		
	case TDCA_OFFSETTASK:
		if (dwTaskID != 0)
		{
			ASSERT(0);
		}
		else
		{
			ASSERT(GetSingleSelectedTaskID());

			bChange = ExtensionMoveSelectedTaskStartAndDueDates(CDateHelper::GetDate(mod.tValue));
		}
		break;

	case TDCA_CUSTOMATTRIB:	
		{
			if (dwTaskID)
				bChange = (SET_CHANGE == m_data.SetTaskCustomAttributeData(dwTaskID, mod.szCustomAttribID, mod.szValue));
			else
				bChange = SetSelectedTaskCustomAttributeData(mod.szCustomAttribID, mod.szValue, FALSE);
		}
		break;

	case TDCA_METADATA:
		{
			IUIExtensionWindow* pExtWnd = GetExtensionWnd(GetTaskView());
			ASSERT(pExtWnd && pExtWnd->GetHwnd());

			if (pExtWnd)
			{
				if (dwTaskID)
					bChange = (SET_CHANGE == m_data.SetTaskMetaData(dwTaskID, pExtWnd->GetTypeID(), mod.szValue));
				else
					bChange = SetSelectedTaskMetaData(pExtWnd->GetTypeID(), mod.szValue);
			}
		}
		break;

	// not supported
	case TDCA_RECURRENCE: 
	case TDCA_CREATIONDATE:
	case TDCA_CREATEDBY:
	case TDCA_POSITION:
	case TDCA_POSITION_SAMEPARENT:
	case TDCA_POSITION_DIFFERENTPARENT:
		ASSERT(0);
		break;

	default:
		ASSERT(0);
		break;
	}

	m_nExtModifyingAttrib = TDCA_NONE;

	if (bChange && dwTaskID)
	{
		Misc::AddUniqueItemT(dwTaskID, aModTaskIDs);

		// Note: We only save off the attribute if (dwTaskID != 0)
		// because 'SetSelectedTask*' will already have handled it
		m_taskTree.GetAttributesAffectedByMod(mod.nAttrib, mapModAttribs);
	}

	return bChange;
}

BOOL CTabbedToDoCtrl::ExtensionMoveSelectedTaskStartAndDueDates(const COleDateTime& dtNewStart)
{
	if (GetSelectedTaskCount() > 1)
		return FALSE;

	if (!CanEditSelectedTask(TDCA_STARTDATE))
		return FALSE;

	Flush();

	IMPLEMENT_DATA_UNDO_EDIT(m_data);

	DWORD dwTaskID = GetSelectedTaskID();

	TDC_SET nRes = m_data.MoveTaskStartAndDueDates(dwTaskID, dtNewStart);

	if (nRes != SET_CHANGE)
		return FALSE;

	// else
	COleDateTime dtDue = GetSelectedTaskDate(TDCD_DUE);

	if (CDateHelper::IsDateSet(dtDue))
		m_eRecurrence.SetDefaultDate(dtDue);

	CDWordArray aModTaskIDs;
	aModTaskIDs.Add(dwTaskID);

	CToDoCtrl::SetModified(TDCA_OFFSETTASK, aModTaskIDs);

	UpdateControls(FALSE); // don't update comments

	return TRUE;
}

LRESULT CTabbedToDoCtrl::OnUIExtModifySelectedTask(WPARAM wParam, LPARAM lParam)
{
	// Prevent possibility of infinite recursion caused by an extension 
	// responding to being updated by posting another change ad infinitum
	AF_NOREENTRANT_RET(FALSE);

	if (IsReadOnly())
	{
		ASSERT(0);
		return FALSE;
	}

	HandleUnsavedComments();

	// Keep track of explicitly modified tasks and attributes
	CDWordArray aModTaskIDs;
	CTDCTaskCompletionArray aTasksForCompletion(m_data, m_sCompletionStatus);
	CTDCAttributeMap mapModAttribs;
	BOOL bChange = FALSE, bCompletionFailed = FALSE;

	// Aggregate all mods as a single edit
	{
		IMPLEMENT_DATA_UNDO_EDIT(m_data);

		try
		{
			const IUITASKMOD* pMods = (const IUITASKMOD*)lParam;
			int nNumMod = (int)wParam;

			ASSERT(nNumMod > 0);

			for (int nMod = 0; nMod < nNumMod; nMod++)
			{
				const IUITASKMOD& mod = pMods[nMod];

				if (ProcessUIExtensionMod(mod, aModTaskIDs, aTasksForCompletion, mapModAttribs))
				{
					bChange = TRUE;
				}
				else
				{
#ifdef _DEBUG
					switch (mod.nAttrib)
					{
					case TDCA_ICON:
					case TDCA_OFFSETTASK:
						// Don't assert on edits that can be cancelled
						break;

					case TDCA_STATUS:
					case TDCA_PERCENT:
					case TDCA_DONEDATE:
						// These can fail with incomplete dependencies
						ASSERT(m_taskTree.SelectionHasDependencies() ||
								TaskHasIncompleteDependencies(mod.dwSelectedTaskID, CString()) ||
								aTasksForCompletion.GetSize());
						break;

					default:
						// all else
						ASSERT(0);
						break;
					}
#endif
				}
			}
		}
		catch (...)
		{
			ASSERT(0);
			bChange = FALSE;
		}

		if (aTasksForCompletion.GetSize())
		{
			if (SetSelectedTaskCompletion(aTasksForCompletion))
			{
				aTasksForCompletion.GetTaskIDs(aModTaskIDs, TRUE);
				m_taskTree.GetAttributesAffectedByMod(TDCA_DONEDATE, mapModAttribs);

				bChange = TRUE;
			}
			else
			{
				bCompletionFailed = TRUE;
			}
		}
	}

	if (bCompletionFailed)
	{
		// We only undo if there were other valid changes else
		// we'll undo the edit before this one
		if (bChange)
			UndoLastAction(TRUE);
	}
	else if (bChange)
	{
		// Note: If 'aModTaskIDs' is NOT empty then it means that the 
		// 'SetSelectedTask...' methods were NOT used and so we need
		// to notify everyone that there have been changes
		if (aModTaskIDs.GetSize())
		{
			ASSERT(mapModAttribs.GetCount());

			SetModified(mapModAttribs, aModTaskIDs, TRUE);
			UpdateControls(FALSE);
		}
		else
		{
			ASSERT(mapModAttribs.GetCount() == 0);
		}
	}
		
	return (bChange && !bCompletionFailed);
}

LRESULT CTabbedToDoCtrl::OnUIExtMoveSelectedTask(WPARAM /*wParam*/, LPARAM lParam)
{
	// Prevent possibility of infinite recursion caused by an extension 
	// responding to being updated by posting another change ad infinitum
	AF_NOREENTRANT_RET(FALSE);

	ASSERT(lParam);
	ASSERT(!IsReadOnly());

	if (IsReadOnly())
		return FALSE;

	if (!HandleUnsavedComments())
		return FALSE;

	IMPLEMENT_DATA_UNDO(m_data, TDCUAT_MOVE);
	
	BOOL bSuccess = FALSE;

	try
	{
		const IUITASKMOVE* pMove = (const IUITASKMOVE*)lParam;

		DWORD dwSrcParentID = GetSelectedTaskParentID(); // zero for multiple parents

		if (m_taskTree.SelectionHasSameParent() && (dwSrcParentID == pMove->dwParentID))
			m_nExtModifyingAttrib = TDCA_POSITION_SAMEPARENT;
		else
			m_nExtModifyingAttrib = TDCA_POSITION_DIFFERENTPARENT;
		
		HTREEITEM htiDropItem = m_taskTree.GetItem(pMove->dwSelectedTaskID);
		ASSERT(htiDropItem == GetSelectedItem());

		HTREEITEM htiDropTarget = m_taskTree.GetItem(pMove->dwParentID);
		HTREEITEM htiDropAfter = m_taskTree.GetItem(pMove->dwAfterSiblingID);

		DD_DROPEFFECT nDrop = (pMove->bCopy ? DD_DROPEFFECT_COPY : DD_DROPEFFECT_MOVE);
		bSuccess = DropSelectedTasks(nDrop, htiDropTarget, htiDropAfter);

		if (bSuccess)
		{
			// If the extension is wanting to copy a task we must
			// update all of it's tasks because it can't know about
			// the new task IDs otherwise
			if (pMove->bCopy)
				UpdateExtensionViewsTasks(TDCA_POSITION);
			else
				SetExtensionsNeedTaskUpdate(TRUE, GetTaskView());
		}
	}
	catch (...)
	{
		ASSERT(0);
		bSuccess = FALSE;
	}

	m_nExtModifyingAttrib = TDCA_NONE;

	return bSuccess;
}

void CTabbedToDoCtrl::RebuildCustomAttributeUI()
{
	// Must remove any deleted attribute columns before resizing/redrawing
	m_taskList.OnCustomAttributeChange();

	CToDoCtrl::RebuildCustomAttributeUI();
}

void CTabbedToDoCtrl::ReposTaskTree(CDeferWndMove* pDWM, const CRect& rPos)
{
	// Tab control takes care of active view including tree/list
	m_tabViews.Resize(rPos, pDWM);

	// List-specific combos
	CRect rCtrl = GetCtrlRect(IDC_LISTVIEWGROUPBYLABEL);

	int nXOffset = (rPos.left - rCtrl.left);
	int nYOffset = (rPos.top - rCtrl.top) + CDlgUnits(this).ToPixelsY(2);

	pDWM->OffsetCtrl(this, IDC_LISTVIEWGROUPBYLABEL,	nXOffset, nYOffset);
	pDWM->OffsetCtrl(this, IDC_LISTVIEWGROUPBYATTRIB,	nXOffset, nYOffset);
	pDWM->OffsetCtrl(this, IDC_LISTVIEWOPTIONSLABEL,	nXOffset, nYOffset);
	pDWM->OffsetCtrl(this, IDC_LISTVIEWOPTIONS,			nXOffset, nYOffset);
}

void CTabbedToDoCtrl::UpdateTasklistVisibility()
{
	BOOL bTasksVis = (m_nMaxState != TDCMS_MAXCOMMENTS);
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		CToDoCtrl::UpdateTasklistVisibility();
		break;

	case FTCV_TASKLIST:
		m_taskList.ShowWindow(bTasksVis ? SW_SHOW : SW_HIDE);
		break;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		break;

	default:
		ASSERT(0);
	}

	// handle tab control
	m_tabViews.ShowWindow(bTasksVis && HasStyle(TDCS_SHOWTREELISTBAR) ? SW_SHOW : SW_HIDE);
}

BOOL CTabbedToDoCtrl::OnEraseBkgnd(CDC* pDC)
{
	// clip out ctrls
	if (m_tabViews.GetSafeHwnd())
	{
		ExcludeChild(&m_tabViews, pDC);
		ExcludeChild(&m_cbListGroupBy, pDC);
		ExcludeChild(&m_cbListOptions, pDC);

		ExcludeCtrl(this, IDC_LISTVIEWGROUPBYLABEL, pDC);
		ExcludeCtrl(this, IDC_LISTVIEWOPTIONSLABEL, pDC);
	}

	return CToDoCtrl::OnEraseBkgnd(pDC);
}

void CTabbedToDoCtrl::SetMaximizeState(TDC_MAXSTATE nState)
{
	TDC_MAXSTATE nPrevState = m_nMaxState;

	CToDoCtrl::SetMaximizeState(nState);

	if (nState != nPrevState)
	{
		FTC_VIEW nView = GetTaskView();
		
		switch (nView)
		{
		case FTCV_TASKTREE:
		case FTCV_UNSET:
			// handled above
			break;

		case FTCV_TASKLIST:
			{
				// Show/Hide list-specific controls
				int nShow = (nState != TDCMS_MAXCOMMENTS) ? SW_SHOW : SW_HIDE;

				for (int nCtrl = 0; nCtrl < NUM_TTDCCTRLS; nCtrl++)
				{
					const TDCCONTROL& ctrl = TTDCCONTROLS[nCtrl];

					if (ctrl.nID != IDC_TASKLISTTABCTRL)
						GetDlgItem(ctrl.nID)->ShowWindow(nShow);
				}
			}
			break;

		case FTCV_UIEXTENSION1:
		case FTCV_UIEXTENSION2:
		case FTCV_UIEXTENSION3:
		case FTCV_UIEXTENSION4:
		case FTCV_UIEXTENSION5:
		case FTCV_UIEXTENSION6:
		case FTCV_UIEXTENSION7:
		case FTCV_UIEXTENSION8:
		case FTCV_UIEXTENSION9:
		case FTCV_UIEXTENSION10:
		case FTCV_UIEXTENSION11:
		case FTCV_UIEXTENSION12:
		case FTCV_UIEXTENSION13:
		case FTCV_UIEXTENSION14:
		case FTCV_UIEXTENSION15:
		case FTCV_UIEXTENSION16:
			if (nState != TDCMS_MAXCOMMENTS)
			{
				IUIExtensionWindow* pExtWnd = GetExtensionWnd(nView);
				ASSERT(pExtWnd && pExtWnd->GetHwnd());

				::InvalidateRect(pExtWnd->GetHwnd(), NULL, FALSE);
				::UpdateWindow(pExtWnd->GetHwnd());
			}
			break;
			
		default:
			ASSERT(0);
		}
	}
}

BOOL CTabbedToDoCtrl::WantTaskContextMenu() const
{
	FTC_VIEW nView = GetTaskView();

	return (ViewSupportsNewTask(nView) || ViewHasTaskSelection(nView));
}

BOOL CTabbedToDoCtrl::GetSelectionBoundingRect(CRect& rSelection) const
{
	rSelection.SetRectEmpty();
	FTC_VIEW nView = GetTaskView();

	if (!ViewHasTaskSelection(nView))
		return FALSE;

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		CToDoCtrl::GetSelectionBoundingRect(rSelection);
		break;

	case FTCV_TASKLIST:
		if (m_taskList.GetSelectionBoundingRect(rSelection))
		{
			m_taskList.ClientToScreen(rSelection);
			ScreenToClient(rSelection);
		}
		break;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			IUIExtensionWindow* pExtWnd = GetExtensionWnd(nView);
			ASSERT(pExtWnd);
			
			if (pExtWnd)
			{
				pExtWnd->GetLabelEditRect(rSelection);
				ScreenToClient(rSelection);
			}
		}
		break;

	default:
		ASSERT(0);
	}

	return (!rSelection.IsRectEmpty());
}

BOOL CTabbedToDoCtrl::CanSelectAll() const
{
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		return CToDoCtrl::CanSelectAll();

	case FTCV_TASKLIST:
		return (m_taskList.GetTaskCount() > 0);

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			IUIExtensionWindow* pExtWnd = GetExtensionWnd(nView);
			ASSERT(pExtWnd);
			
			if (pExtWnd)
				return pExtWnd->CanDoAppCommand(IUI_SELECTALL);
		}
		break;

	default:
		ASSERT(0);
	}

	return FALSE;
}

void CTabbedToDoCtrl::SelectAll()
{
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		CToDoCtrl::SelectAll();
		break;

	case FTCV_TASKLIST:
		{
			int nNumItems = m_taskList.GetItemCount();
			BOOL bAllTasks = (m_taskTree.GetItemCount() == nNumItems);

			// select items in tree
			if (bAllTasks)
			{
				CToDoCtrl::SelectAll();
				m_taskList.SelectAll();
			}
			else
			{
				// save IDs only if not showing all tasks
				CDWordArray aTaskIDs;

				for (int nItem = 0; nItem < nNumItems; nItem++)
					aTaskIDs.Add(m_taskList.GetTaskID(nItem));

				m_taskList.SelectTasks(aTaskIDs);
				CToDoCtrl::SelectTasks(aTaskIDs);
			}
		}
		break;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			IUIExtensionWindow* pExtWnd = GetExtensionWnd(nView);
			ASSERT(pExtWnd);

			if (pExtWnd && pExtWnd->DoAppCommand(IUI_SELECTALL))
				CToDoCtrl::SelectAll();
		}
		break;

	default:
		ASSERT(0);
	}
}

int CTabbedToDoCtrl::GetSelectedTasks(CTaskFile& tasks, const TDCGETTASKS& filter) const
{
	return GetSelectedTasks(tasks, GetTaskView(), filter);
}

int CTabbedToDoCtrl::GetSelectedTasks(CTaskFile& tasks, FTC_VIEW nView, const TDCGETTASKS& filter) const
{
	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		return CToDoCtrl::GetSelectedTasks(tasks, filter);

	case FTCV_TASKLIST:
		{
			PrepareTaskfileForTasks(tasks, filter);

			// we return exactly what's selected as a flat list and in the same order
			CDWordArray aTaskIDs;
			int nNumSel = m_taskList.GetSelectedTaskIDs(aTaskIDs, FALSE);

			for (int nSel = 0; nSel < nNumSel; nSel++)
			{
				DWORD dwTaskID = aTaskIDs[nSel];
				DWORD dwParentID = m_data.GetTaskParentID(dwTaskID);
				
				// Add immediate parent as required.
				// Note: we can assume that the selected task is always added successfully
				if (dwParentID && filter.HasFlag(TDCGSTF_IMMEDIATEPARENT))
				{
					DWORD dwParentsParentID = m_data.GetTaskParentID(dwParentID);
					
					// Note: this may fail if the filter excludes it
					CToDoCtrl::AddTreeItemToTaskFile(NULL, dwParentID, tasks, NULL, filter, FALSE, dwParentsParentID);  // FALSE == no subtasks
				}

				VERIFY(CToDoCtrl::AddTreeItemToTaskFile(NULL, dwTaskID, tasks, NULL, filter, FALSE, dwParentID)); // FALSE == no subtasks
			}

			if (nNumSel)
			{
				AddSelectedTaskReferencesToTaskFile(filter, tasks);
				AddSelectedTaskDependentsToTaskFile(filter, tasks);
			}

			return tasks.GetTaskCount();
		}
		break;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		return CToDoCtrl::GetSelectedTasks(tasks, filter); // for now

	default:
		ASSERT(0);
	}

	return 0;
}

void CTabbedToDoCtrl::RedrawReminders()
{ 
 	CToDoCtrl::RedrawReminders();

	if (InListView())
	{
		m_taskList.OnReminderChange();
	}
	else if (m_taskList.IsSortingBy(TDCC_REMINDER))
	{
		GetViewData(FTCV_TASKLIST)->bNeedResort = TRUE;
	}
}

void CTabbedToDoCtrl::SetEditTitleTaskID(DWORD dwTaskID)
{
	CToDoCtrl::SetEditTitleTaskID(dwTaskID);
	
	m_taskList.SetEditTitleTaskID(dwTaskID);
}

DWORD CTabbedToDoCtrl::HitTestTask(const CPoint& ptScreen, BOOL bTitleColumnOnly) const
{
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		return CToDoCtrl::HitTestTask(ptScreen, bTitleColumnOnly);

	case FTCV_TASKLIST:
		return m_taskList.HitTestTask(ptScreen, bTitleColumnOnly);

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			const IUIExtensionWindow* pExtWnd = GetExtensionWnd(nView);

			if (pExtWnd)
			{
				CRect rExtWnd;
				::GetWindowRect(pExtWnd->GetHwnd(), rExtWnd);

				if (rExtWnd.PtInRect(ptScreen))
					return pExtWnd->HitTestTask(ptScreen, (bTitleColumnOnly != FALSE));
			}
		}
		break;

	default:
		ASSERT(0);
	}

	return 0;
}

DWORD CTabbedToDoCtrl::SetStyle(TDC_STYLE nStyle, BOOL bEnable)
{
	// Sanity check
#ifdef _DEBUG
	BOOL bEnabled = HasStyle(nStyle);
	ASSERT ((bEnabled && !bEnable) || (!bEnabled && bEnable));
#endif

	// base class processing
	DWORD dwResult = CToDoCtrl::SetStyle(nStyle, bEnable);

	// our processing
	switch (nStyle)
	{
	case TDCS_SHOWTREELISTBAR:
		m_tabViews.ShowTabControl(bEnable);
		dwResult |= TDCSS_WANTRESIZE;
		break;

	case TDCS_SHOWTASKVIEWTABCLOSEBUTTON:
		if (bEnable)
			m_tabViews.ModifyFlags(0, TCE_CLOSEBUTTON);
		else
			m_tabViews.ModifyFlags(TCE_CLOSEBUTTON, 0);
		break;

	case TDCS_READONLY:
		SetExtensionsReadOnly(bEnable);
		break;

	// For the rest we have two tasks:
	// 1. Keep track of tree resorts if not in tree view
	// 2. Keep track of styles which affect calculated task attributes
	case TDCS_SORTDONETASKSATBOTTOM:
		{
			m_bTreeNeedResort = !InTreeView();
		}
		break;

	case TDCS_DUEHAVEHIGHESTPRIORITY:
	case TDCS_DONEHAVELOWESTPRIORITY:
		{
			if (!InTreeView() && CToDoCtrl::IsSortingBy(TDCC_PRIORITY))
				m_bTreeNeedResort = TRUE;

			m_mapAttribsAffectedByPrefs.Add(TDCA_PRIORITY);
			m_mapAttribsAffectedByPrefs.Add(TDCA_COLOR);
		}
		break;

	case TDCS_DONEHAVELOWESTRISK:
		{
			if (!InTreeView() && CToDoCtrl::IsSortingBy(TDCC_RISK))
				m_bTreeNeedResort = TRUE;

			m_mapAttribsAffectedByPrefs.Add(TDCA_RISK);
		}
		break;

	case TDCS_COLORTEXTBYPRIORITY:
	case TDCS_COLORTEXTBYATTRIBUTE:
	case TDCS_COLORTEXTBYNONE:
	case TDCS_TASKCOLORISBACKGROUND:
	case TDCS_TREATSUBCOMPLETEDASDONE:
		{
			m_mapAttribsAffectedByPrefs.Add(TDCA_COLOR);
		}
		break;

	case TDCS_USEEARLIESTDUEDATE:
	case TDCS_USELATESTDUEDATE:
	case TDCS_NODUEDATEISDUETODAYORSTART:
		{
			m_mapAttribsAffectedByPrefs.Add(TDCA_DUEDATE);
			m_mapAttribsAffectedByPrefs.Add(TDCA_COLOR);
		}
		break;

	case TDCS_USEEARLIESTSTARTDATE:
	case TDCS_USELATESTSTARTDATE:
		{
			m_mapAttribsAffectedByPrefs.Add(TDCA_STARTDATE);
			m_mapAttribsAffectedByPrefs.Add(TDCA_COLOR);
		}
		break;

	case TDCS_USEHIGHESTPRIORITY:
	case TDCS_INCLUDEDONEINPRIORITYCALC:
		{
			m_mapAttribsAffectedByPrefs.Add(TDCA_PRIORITY);
			m_mapAttribsAffectedByPrefs.Add(TDCA_COLOR);
		}
		break;

	case TDCS_USEHIGHESTRISK:
	case TDCS_INCLUDEDONEINRISKCALC:
		{
			m_mapAttribsAffectedByPrefs.Add(TDCA_RISK);
		}
		break;

	case TDCS_AUTOCALCPERCENTDONE:
	case TDCS_WEIGHTPERCENTCALCBYNUMSUB:
	case TDCS_AVERAGEPERCENTSUBCOMPLETION:
		{
			m_mapAttribsAffectedByPrefs.Add(TDCA_PERCENT);
		}
		break;

	case TDCS_USEPERCENTDONEINTIMEEST:
		{
			m_mapAttribsAffectedByPrefs.Add(TDCA_TIMEESTIMATE);
		}
		break;
	}

	return dwResult;
}

void CTabbedToDoCtrl::SetPriorityColors(const CDWordArray& aColors)
{
	if (m_taskList.SetPriorityColors(aColors))
		m_mapAttribsAffectedByPrefs.Add(TDCA_COLOR);

	CToDoCtrl::SetPriorityColors(aColors);
}

void CTabbedToDoCtrl::SetCompletedTaskColor(COLORREF color)
{
	if (m_taskList.SetCompletedTaskColor(color))
		m_mapAttribsAffectedByPrefs.Add(TDCA_COLOR);

	CToDoCtrl::SetCompletedTaskColor(color);
}

void CTabbedToDoCtrl::SetFlaggedTaskColor(COLORREF color)
{
	if (m_taskList.SetFlaggedTaskColor(color))
		m_mapAttribsAffectedByPrefs.Add(TDCA_COLOR);

	CToDoCtrl::SetFlaggedTaskColor(color);
}

void CTabbedToDoCtrl::SetReferenceTaskColor(COLORREF color)
{
	if (m_taskList.SetReferenceTaskColor(color))
		m_mapAttribsAffectedByPrefs.Add(TDCA_COLOR);

	CToDoCtrl::SetReferenceTaskColor(color);
}

void CTabbedToDoCtrl::SetAttributeColors(TDC_ATTRIBUTE nAttrib, const CTDCColorMap& colors)
{
	if (m_taskList.SetAttributeColors(nAttrib, colors))
		m_mapAttribsAffectedByPrefs.Add(TDCA_COLOR);

	CToDoCtrl::SetAttributeColors(nAttrib, colors);
}

void CTabbedToDoCtrl::SetStartedTaskColors(COLORREF crStarted, COLORREF crStartedToday)
{
	if (m_taskList.SetStartedTaskColors(crStarted, crStartedToday))
		m_mapAttribsAffectedByPrefs.Add(TDCA_COLOR);

	CToDoCtrl::SetStartedTaskColors(crStarted, crStartedToday);
}

void CTabbedToDoCtrl::SetDueTaskColors(COLORREF crDue, COLORREF crDueToday)
{
	if (m_taskList.SetDueTaskColors(crDue, crDueToday))
		m_mapAttribsAffectedByPrefs.Add(TDCA_COLOR);

	CToDoCtrl::SetDueTaskColors(crDue, crDueToday);
}

void CTabbedToDoCtrl::SetGridlineColor(COLORREF color)
{
	m_taskList.SetGridlineColor(color);
	
	CToDoCtrl::SetGridlineColor(color);
}

void CTabbedToDoCtrl::SetAlternateLineColor(COLORREF color)
{	
	m_taskList.SetAlternateLineColor(color);

	CToDoCtrl::SetAlternateLineColor(color);
}

void CTabbedToDoCtrl::NotifyEndPreferencesUpdate()
{
	// base class
	CToDoCtrl::NotifyEndPreferencesUpdate();

	// notify extension windows
	if (HasAnyExtensionViews())
	{
		CAutoFlag af(m_bUpdatingExtensions, TRUE);

		// we need to update in 2 ways:
		// 1. Tell the extensions that application settings have changed
		// 2. Refresh tasks if one or more calculated attributes has changed
		CPreferences prefs;
		CString sKey = GetPreferencesKey(_T("UIExtensions"));
		
		int nExt = m_aExtViews.GetSize();
		FTC_VIEW nCurView = GetTaskView();

		while (nExt--)
		{
			FTC_VIEW nExtView = GetExtensionView(nExt);
			
			IUIExtensionWindow* pExtWnd = NULL;
			VIEWDATA* pVData = NULL;

			if (!GetExtensionWnd(nExtView, pExtWnd, pVData))
				continue;

			// notify all extensions of prefs change
			pExtWnd->LoadPreferences(prefs, sKey, true);

			// Check if the extension needs a task update as a
			// consequence of any preferences changes
			CTDCAttributeMap mapWantedAttribs;

			BOOL bWantTaskUpdate = (!m_mapAttribsAffectedByPrefs.IsEmpty() &&
									GetExtensionViewWantedChanges(nExt, m_mapAttribsAffectedByPrefs, mapWantedAttribs));

			if (bWantTaskUpdate)
			{
				if (nExtView == nCurView)
				{
					BeginExtensionProgress(pVData);

					CTaskFile tasks;
					CWaitCursor cursor;

					if (GetAllTasksForExtensionViewUpdate(mapWantedAttribs, tasks))
					{
						pVData->bNeedFullTaskUpdate = FALSE;
						UpdateExtensionView(pExtWnd, tasks, IUI_EDIT);
					}

					EndExtensionProgress();
				}
				else
				{
					pVData->bNeedFullTaskUpdate = TRUE;
				}
			}
		}
	}

	m_mapAttribsAffectedByPrefs.RemoveAll(); // always
}

void CTabbedToDoCtrl::BeginExtensionProgress(const VIEWDATA* pVData, UINT nMsg)
{
	ASSERT(pVData);

	if (nMsg == 0)
		nMsg = IDS_UPDATINGTABBEDVIEW;

	CEnString sMsg(nMsg, pVData->pExtension->GetMenuText());
	GetParent()->SendMessage(WM_TDCM_LENGTHYOPERATION, TRUE, (LPARAM)(LPCTSTR)sMsg);
}

void CTabbedToDoCtrl::EndExtensionProgress()
{
	GetParent()->SendMessage(WM_TDCM_LENGTHYOPERATION, FALSE);
}

BOOL CTabbedToDoCtrl::BeginTimeTracking(DWORD dwTaskID, BOOL bNotify)
{
	if (!CToDoCtrl::BeginTimeTracking(dwTaskID, bNotify))
		return FALSE;

	m_taskList.SetTimeTrackTaskID(m_timeTracking.GetTrackedTaskID());
	return TRUE;
}

void CTabbedToDoCtrl::EndTimeTracking(BOOL bAllowConfirm, BOOL bNotify)
{
	CToDoCtrl::EndTimeTracking(bAllowConfirm, bNotify);
	
	m_taskList.SetTimeTrackTaskID(m_timeTracking.GetTrackedTaskID());
}

CString CTabbedToDoCtrl::GetControlDescription(const CWnd* pCtrl) const
{
	FTC_VIEW nView = GetTaskView();
	HWND hwndView = m_tabViews.GetViewHwnd(nView);

	// Task view tab-bar just returns the active task view
	if (CDialogHelper::IsChildOrSame(m_tabViews, pCtrl->GetSafeHwnd()))
	{
		return GetControlDescription(CWnd::FromHandle(hwndView)); // RECURSIVE CALL
	}

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		break; // handled below

	case FTCV_TASKLIST:
		if (CDialogHelper::IsChildOrSame(m_taskList, pCtrl->GetSafeHwnd()))
			return CEnString(IDS_LISTVIEW);
		break;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		if (CDialogHelper::IsChildOrSame(hwndView, pCtrl->GetSafeHwnd()))
			return m_tabViews.GetViewName(nView);
		break;

	default:
		ASSERT(0);
	}

	return CToDoCtrl::GetControlDescription(pCtrl);
}

DWORD CTabbedToDoCtrl::GetNextNonSelectedTaskID() const
{
	FTC_VIEW nView = GetTaskView();
	DWORD dwNextSelID = 0;

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		dwNextSelID = CToDoCtrl::GetNextNonSelectedTaskID();
		break;

	case FTCV_TASKLIST:
	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			dwNextSelID = GetNextTaskID(GetSelectedTaskID(), TTCNT_NEXTVISIBLE, TRUE);

			if (!dwNextSelID)
				dwNextSelID = GetNextTaskID(GetSelectedTaskID(), TTCNT_PREVVISIBLE, TRUE);
		}
		break;
	}

	return dwNextSelID; // Can be '0'
}

DWORD CTabbedToDoCtrl::GetNextTaskID(DWORD dwTaskID, TTC_NEXTTASK nNext, BOOL bExcludeSelected) const
{
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		ASSERT(0); // For derived use only
		return 0;

	case FTCV_TASKLIST:
		return m_taskList.GetNextTaskID(dwTaskID, nNext, bExcludeSelected);

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			// Same as for ListView
			IUIExtensionWindow* pExtWnd = GetExtensionWnd(nView);
			ASSERT(pExtWnd);
			
			if (pExtWnd)
			{
				IUI_APPCOMMAND nCmd = MapGetNextToCommand(nNext);

				CUIExtensionAppCmdData data(dwTaskID);
				
				while (pExtWnd->DoAppCommand(nCmd, &data))
				{
					if (!data.dwTaskID || (data.dwTaskID == dwTaskID))
						break;
					
					if (bExcludeSelected)
					{
						HTREEITEM htiNext = m_taskTree.GetItem(data.dwTaskID);
						ASSERT(htiNext);
						
						if (TSH().IsItemSelected(htiNext, TRUE))
							continue;
					}
					
					// else
					return data.dwTaskID;
				}
			}
		}
		break;

	default:
		ASSERT(0);
	}

	// not found
	return 0;
}

IUI_APPCOMMAND CTabbedToDoCtrl::MapGetNextToCommand(TTC_NEXTTASK nNext)
{
	switch (nNext)
	{
	case TTCNT_NEXT:			return IUI_GETNEXTTASK;
	case TTCNT_NEXTVISIBLE:		return IUI_GETNEXTVISIBLETASK;
	case TTCNT_NEXTTOPLEVEL:	return IUI_GETNEXTTOPLEVELTASK;

	case TTCNT_PREV:			return IUI_GETPREVTASK;
	case TTCNT_PREVVISIBLE:		return IUI_GETPREVVISIBLETASK;
	case TTCNT_PREVTOPLEVEL:	return IUI_GETPREVTOPLEVELTASK;
	}

	ASSERT(0);
	return (IUI_APPCOMMAND)-1;
}

BOOL CTabbedToDoCtrl::CreateNewTask(const CString& sText, TDC_INSERTWHERE nWhere, BOOL bEditText, DWORD dwDependency)
{
	return CToDoCtrl::CreateNewTask(sText, nWhere, bEditText, dwDependency);
}

TODOITEM* CTabbedToDoCtrl::CreateNewTask(HTREEITEM htiParent)
{
	TODOITEM* pTDI = CToDoCtrl::CreateNewTask(htiParent);

	if (!pTDI)
	{
		ASSERT(0);
		return NULL;
	}

	// give active extension view a chance to initialise
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			IUIExtensionWindow* pExtWnd = GetExtensionWnd(nView);
			ASSERT(pExtWnd);

			if (pExtWnd)
			{
				CTaskFile task;
				HTASKITEM hTask = task.NewTask(pTDI->sTitle, NULL, 0, 0);

				if (pExtWnd->PrepareNewTask(&task))
				{
					// Don't overwrite default attributes
					task.MergeTaskAttributes(hTask, *pTDI);
				}
			}
		}
		break;
	}

	return pTDI;
}

BOOL CTabbedToDoCtrl::CanCreateNewTask(TDC_INSERTWHERE nInsertWhere) const
{
	FTC_VIEW nView = GetTaskView();

	// locations requiring a selected task
	switch (nInsertWhere)
	{
	case TDC_INSERTATTOPOFSELTASKPARENT:
	case TDC_INSERTATBOTTOMOFSELTASKPARENT:
	case TDC_INSERTAFTERSELTASK:
	case TDC_INSERTBEFORESELTASK:
	case TDC_INSERTATTOPOFSELTASK:
	case TDC_INSERTATBOTTOMOFSELTASK:
		if (!HasSelection())
			return FALSE;
		break;
	}

	BOOL bCanCreate = CToDoCtrl::CanCreateNewTask(nInsertWhere);

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
	case FTCV_TASKLIST:
		break;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		if (bCanCreate)
		{
			const VIEWDATA* pVData = GetViewData(nView);

			if (pVData)
				bCanCreate = pVData->bCanPrepareNewTask;
		}
		break;

	default:
		bCanCreate = FALSE;
		ASSERT(0);
		break;
	}

	return bCanCreate;
}

BOOL CTabbedToDoCtrl::CanPasteTasks(TDC_PASTE nWhere, BOOL bAsRef) const
{
	if (!CToDoCtrl::CanPasteTasks(nWhere, bAsRef))
		return FALSE;

	switch (nWhere)
	{
	case TDCP_ONSELTASK:
	case TDCP_BELOWSELTASK:
		return HasSelection();

	case TDCP_ATBOTTOM:
		return TRUE; // always
	}

	// all else
	ASSERT(0);
	return FALSE;
}

void CTabbedToDoCtrl::RebuildList(BOOL bChangeGroup, TDC_COLUMN nNewGroupBy, const void* pContext)
{
	GetViewData(FTCV_TASKLIST)->bNeedFullTaskUpdate = FALSE;

	// Get the task tree's currently visible tasks because 
	// these will always be the largest set from which we
	// determine the list's visible tasks
	CDWordArray aTaskIDs;

	BOOL bWantParents = !(HasListOption(LVO_HIDEPARENTS) || HasStyle(TDCS_ALWAYSHIDELISTPARENTS));
	BOOL bWantCollapsed = !HasListOption(LVO_HIDECOLLAPSED);

	if (!GetAllTaskIDs(aTaskIDs, bWantParents, bWantCollapsed))
	{
		m_taskList.DeleteAll(); 
		return;
	}

	BOOL bInList = InListView();

	// note: the call to SyncListSelectionToTree at the bottom fails if the 
	// list has redraw disabled so it must happen outside the scope of hr2
	{
		CHoldRedraw hr(*this);
		CHoldRedraw hr2(m_taskList);
		CWaitCursor cursor;

		// Delete all the items before changing the group
		// so there's nothing actually for the list to do
		m_taskList.DeleteAll();

		if (bChangeGroup)
			m_taskList.SetGroupBy(m_nListViewGroupBy);

		for (int nID = 0; nID < aTaskIDs.GetSize(); nID++)
		{
			DWORD dwTaskID = aTaskIDs[nID];

			if (WantAddTreeTaskToList(dwTaskID, pContext))
			{
				VERIFY (m_taskList.InsertItem(dwTaskID) >= 0);
			}	
		}

		m_taskList.SetNextUniqueTaskID(m_dwNextUniqueID);
		m_taskList.OnBuildComplete();

		ResortList();
	}
	
	// Make sure something is selected
	if (bInList)
		SyncListSelectionToTree(TRUE);
	
	BuildListGroupByCombo();
}

BOOL CTabbedToDoCtrl::WantAddTreeTaskToList(DWORD dwTaskID, const void* pContext) const
{
	ASSERT(!m_data.IsTaskParent(dwTaskID) || !(HasListOption(LVO_HIDEPARENTS) || HasStyle(TDCS_ALWAYSHIDELISTPARENTS)));
	
	BOOL bHideNoneGroup = ((m_nListViewGroupBy != TDCC_NONE) && HasListOption(LVO_HIDENOGROUPVALUE));

	if (bHideNoneGroup && !m_taskList.TaskHasGroupValue(dwTaskID))
		return FALSE;

	if (m_data.IsTaskReference(dwTaskID))
		return FALSE;

	return TRUE;
}

void CTabbedToDoCtrl::ResortList(BOOL bAllowToggle)
{
	if (m_taskList.IsSorting())
		m_taskList.Resort(bAllowToggle);
	else
		m_taskList.Unsort();

	GetViewData(FTCV_TASKLIST)->bNeedResort = FALSE;
}

void CTabbedToDoCtrl::SetExtensionsReadOnly(BOOL bReadOnly)
{
	int nExt = m_aExtViews.GetSize();

	while (nExt--)
	{
		if (m_aExtViews[nExt] != NULL)
			m_aExtViews[nExt]->SetReadOnly(bReadOnly != FALSE);
	}
}

void CTabbedToDoCtrl::SetExtensionsNeedTaskUpdate(BOOL bUpdate, FTC_VIEW nIgnore)
{
	int nExt = m_aExtViews.GetSize();

	while (nExt--)
	{
		FTC_VIEW nView = (FTC_VIEW)(FTCV_UIEXTENSION1 + nExt);
		
		if (nView == nIgnore)
			continue;

		// else
		SetViewNeedsTaskUpdate(nView, bUpdate);
	}
}

void CTabbedToDoCtrl::SetExtensionsNeedFontUpdate(BOOL bUpdate, FTC_VIEW nIgnore)
{
	int nExt = m_aExtViews.GetSize();

	while (nExt--)
	{
		FTC_VIEW nView = (FTC_VIEW)(FTCV_UIEXTENSION1 + nExt);
		
		if (nView == nIgnore)
			continue;

		// else
		VIEWDATA* pVData = GetViewData(nView);
		
		if (pVData)
			pVData->bNeedFontUpdate = bUpdate;
	}
}

void CTabbedToDoCtrl::SetListViewNeedFontUpdate(BOOL bUpdate)
{
	VIEWDATA* pVData = GetViewData(FTCV_TASKLIST);
	ASSERT(pVData);

	if (pVData)
		pVData->bNeedFontUpdate = bUpdate;
}

void CTabbedToDoCtrl::SetModified(const CTDCAttributeMap& mapAttribIDs, const CDWordArray& aModTaskIDs, BOOL bAllowResort)
{
	CToDoCtrl::SetModified(mapAttribIDs, aModTaskIDs, (bAllowResort && InTreeView()));

	// For new tasks we want to do as little processing as possible 
	// so as not to delay the appearance of the title edit field.
	// So, we don't update 'other' views until we receive a successful 
	// title edit notification unless they are the active view
	DWORD dwModTaskID = (aModTaskIDs.GetSize() ? aModTaskIDs[0] : 0);

	BOOL bNewSingleTask = (mapAttribIDs.HasOnly(TDCA_NEWTASK) && 
							(aModTaskIDs.GetSize() == 1));

	BOOL bNewTaskTitleEdit = (mapAttribIDs.HasOnly(TDCA_TASKNAME) &&
								(aModTaskIDs.GetSize() == 1) &&
								(dwModTaskID == m_dwLastAddedID));

	switch (GetTaskView())
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		if (bNewTaskTitleEdit)
		{
			UpdateListView(TDCA_NEWTASK, aModTaskIDs, FALSE);
			UpdateExtensionViews(TDCA_NEWTASK, aModTaskIDs);
		}
		else if (!bNewSingleTask)
		{
			UpdateListView(mapAttribIDs, aModTaskIDs, FALSE);
			UpdateExtensionViews(mapAttribIDs, aModTaskIDs);
		}
		break;

	case FTCV_TASKLIST:
		UpdateListView(mapAttribIDs, aModTaskIDs, bAllowResort);

		if (bNewTaskTitleEdit)
		{
			UpdateExtensionViews(TDCA_NEWTASK, aModTaskIDs);
		}
		else if (!bNewSingleTask)
		{
			UpdateExtensionViews(mapAttribIDs, aModTaskIDs);
		}
		else
		{
			// Ensure new task is selected for label editing
			SelectTask(dwModTaskID, FALSE);
		}
		break;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		UpdateExtensionViews(mapAttribIDs, aModTaskIDs);

		if (bNewTaskTitleEdit)
		{
			UpdateListView(TDCA_NEWTASK, aModTaskIDs, FALSE);
		}
		else if (!bNewSingleTask)
		{
			UpdateListView(mapAttribIDs, aModTaskIDs, FALSE);
		}
		else
		{
			// Ensure new task is selected for label editing
			SelectTask(dwModTaskID, FALSE);
		}
		break;
	}

	UpdateSortStates(mapAttribIDs, bAllowResort);
}

void CTabbedToDoCtrl::UpdateListView(const CTDCAttributeMap& mapAttribIDs, const CDWordArray& aModTaskIDs, BOOL bAllowResort)
{
	// Don't do anything if we are not active and we are waiting
	// for a full task update
	VIEWDATA* pVData = GetViewData(FTCV_TASKLIST);
	BOOL bInListView = InListView();

	if (!bInListView && pVData->bNeedFullTaskUpdate)
		return;

	if (mapAttribIDs.Has(TDCA_DELETE) ||
		mapAttribIDs.Has(TDCA_ARCHIVE))
	{
		// Deletion operations are fairly quick so we do those 
		// even if the List View is not active
		if (m_taskTree.GetItemCount())
		{
			CHoldRedraw hr(m_taskList);

			m_taskList.RemoveDeletedItems();

			if (bInListView)
				SyncListSelectionToTree(FALSE);
		}
		else
		{
			m_taskList.DeleteAll();
		}
	}
	else if (mapAttribIDs.Has(TDCA_NEWTASK) && aModTaskIDs.GetSize())
	{
		int nSel = m_taskList.GetSelectedItem();

		if (nSel != -1)
			nSel++;

		ASSERT(aModTaskIDs[0]);
		m_taskList.InsertItem(aModTaskIDs[0], nSel);
	}
	else if (mapAttribIDs.Has(TDCA_NEWTASK) ||
			 mapAttribIDs.Has(TDCA_UNDO) ||
			 mapAttribIDs.Has(TDCA_PASTE))
	{
		if (bInListView)
		{
			RebuildList();
			SyncListSelectionToTree(TRUE);
		}
		else
		{
			pVData->bNeedFullTaskUpdate = TRUE;
		}
	}
	else if (mapAttribIDs.Has(TDCA_POSITION_DIFFERENTPARENT) && (m_dwListOptions != 0))
	{
		ASSERT (!bInListView);

		pVData->bNeedFullTaskUpdate = TRUE;
	}
	else // TDCA_MERGE and all other attributes
	{
		if (bInListView)
			m_taskList.InvalidateSelection();
	}

	if (bInListView || !pVData->bNeedFullTaskUpdate)
		m_taskList.SetModified(mapAttribIDs, (bInListView && bAllowResort));
}

int CTabbedToDoCtrl::PopulateExtensionViewAttributes(const IUIExtensionWindow* pExtWnd, VIEWDATA* pData)
{
	// Sanity checks
	if (!pExtWnd || !pData || !pData->pExtension)
	{
		ASSERT(0);
		return 0;
	}
	else if (Misc::NaturalCompare(pExtWnd->GetTypeID(), pData->pExtension->GetTypeID()) != 0)
	{
		ASSERT(0);
		return 0;
	}

	// Once only
	ASSERT(pData->mapWantedAttrib.IsEmpty());

	if (pData->mapWantedAttrib.IsEmpty())
	{
		for (int nAttrib = TDCA_FIRST_ATTRIBUTE; nAttrib <= TDCA_LAST_ATTRIBUTE; nAttrib++)
		{
			if (pExtWnd->WantTaskUpdate((TDC_ATTRIBUTE)nAttrib))
				pData->mapWantedAttrib.Add((TDC_ATTRIBUTE)nAttrib);
		}

		// Misc
		if (pExtWnd->WantTaskUpdate(TDCA_CUSTOMATTRIB))
			pData->mapWantedAttrib.Add(TDCA_CUSTOMATTRIB_ALL);

		// Always
		pData->mapWantedAttrib.Add(TDCA_LOCK);
		pData->mapWantedAttrib.Add(TDCA_METADATA);

		// Include 'position' if extension supports 'unsorted'
		CUIExtensionAppCmdData data(TDCA_NONE, TRUE);

		if (pExtWnd->CanDoAppCommand(IUI_SORT, &data))
			pData->mapWantedAttrib.Add(TDCA_POSITION);
	}

	return pData->mapWantedAttrib.GetCount();
}

int CTabbedToDoCtrl::GetAllExtensionViewsWantedAttributes(CTDCAttributeMap& mapAttribIDs) const
{
	// Build a map of wanted attributes for all extensions
	mapAttribIDs.RemoveAll();
	int nExt = m_aExtViews.GetSize();
	
	while (nExt--)
	{
		FTC_VIEW nView = GetExtensionView(nExt);
		VIEWDATA* pVData = GetViewData(nView);

		if (pVData)
			mapAttribIDs.Append(pVData->mapWantedAttrib);
	}

	return mapAttribIDs.GetCount();
}

void CTabbedToDoCtrl::UpdateExtensionViews(const CTDCAttributeMap& mapAttribIDs, const CDWordArray& aModTaskIDs)
{
	if (!HasAnyExtensionViews() || mapAttribIDs.Has(TDCA_NONE))
		return;

	FTC_VIEW nCurView = GetTaskView();

	if (mapAttribIDs.Has(TDCA_NEWTASK))
	{
		// If only a single task was created then treat it like an edit
		if (aModTaskIDs.GetSize() == 1)
		{
			UpdateExtensionViewsSelection(mapAttribIDs);

			// Make sure any new dependencies and date updates 
			// get passed on too
			if (m_data.TaskHasDependents(aModTaskIDs[0]))
			{
				CTDCAttributeMap mapDepAttribIDs;
				m_taskTree.GetAttributesAffectedByMod(TDCA_DEPENDENCY, mapDepAttribIDs);

				UpdateExtensionViewsSelection(mapDepAttribIDs);
			}
		}
		else
		{
			UpdateExtensionViewsTasks(mapAttribIDs);
		}
	}
	else if (mapAttribIDs.Has(TDCA_DELETE) ||
			 mapAttribIDs.Has(TDCA_UNDO) ||
			 mapAttribIDs.Has(TDCA_PASTE) ||
			 mapAttribIDs.Has(TDCA_MERGE) ||
			 mapAttribIDs.Has(TDCA_ARCHIVE))
	{
		// These attributes require an entire refresh of the current view 
		// (if it's an extension) and mark the others as needing updates
		UpdateExtensionViewsTasks(mapAttribIDs);
	}
	else if (mapAttribIDs.Has(TDCA_POSITION) ||
			 mapAttribIDs.Has(TDCA_POSITION_SAMEPARENT) ||
			 mapAttribIDs.Has(TDCA_POSITION_DIFFERENTPARENT))
	{
		if (!mapAttribIDs.HasOnly(m_nExtModifyingAttrib))
		{
			UpdateExtensionViewsTasks(mapAttribIDs);
		}
	}
	else if (mapAttribIDs.Has(TDCA_PROJECTNAME))
	{
		UpdateExtensionViewsProjectName();
	}
	else if (mapAttribIDs.Has(TDCA_ENCRYPT) || 
			 mapAttribIDs.Has(TDCA_METADATA))
	{
		// do nothing
	}
	else // all else
	{
		// for a simple attribute change (or addition) update all extensions
		// at the same time so that they won't need updating when the user switches view
		// TDCA_TASKNAME:
		// TDCA_ALL:
		// TDCA_DONEDATE:
		// TDCA_DUEDATE:
		// TDCA_STARTDATE:
		// TDCA_PRIORITY:
		// TDCA_COLOR:
		// TDCA_ALLOCTO:
		// TDCA_ALLOCBY:
		// TDCA_STATUS:
		// TDCA_CATEGORY:
		// TDCA_TAGS:
		// TDCA_PERCENT:
		// TDCA_TIMEESTIMATE:
		// TDCA_TIMESPENT:
		// TDCA_FILELINK:
		// TDCA_COMMENTS:
		// TDCA_FLAG:
		// TDCA_LOCK:
		// TDCA_CREATIONDATE:
		// TDCA_CREATEDBY:
		// TDCA_RISK: 
		// TDCA_EXTERNALID: 
		// TDCA_COST: 
		// TDCA_DEPENDENCY: 
		// TDCA_RECURRENCE: 
		// TDCA_VERSION:
		// TDCA_CUSTOMATTRIBDEFS:
		// TDCA_ICON:
		UpdateExtensionViewsSelection(mapAttribIDs);
	}
}

void CTabbedToDoCtrl::UpdateExtensionViewsProjectName()
{
	if (AnyExtensionViewWantsChange(TDCA_PROJECTNAME))
	{
		CTaskFile tasks;
		AppendTaskFileHeader(tasks);

		tasks.SetAvailableAttributes(TDCA_PROJECTNAME);

		int nExt = m_aExtViews.GetSize();

		while (nExt--)
		{
			FTC_VIEW nExtView = GetExtensionView(nExt);
			VIEWDATA* pVData = GetViewData(nExtView);
			ASSERT(pVData);

			if (pVData && !pVData->bNeedFullTaskUpdate && pVData->WantAttribute(TDCA_PROJECTNAME))
			{
				IUIExtensionWindow* pExtWnd = m_aExtViews[nExt];
				ASSERT(pExtWnd);

				if (pExtWnd)
					pExtWnd->UpdateTasks(&tasks, IUI_EDIT);
			}
		}
	}
}

void CTabbedToDoCtrl::UpdateExtensionViewsTasks(const CTDCAttributeMap& mapAttribIDs)
{
	// Sanity checks
	ASSERT(HasAnyExtensionViews());

	ASSERT(mapAttribIDs.HasOnly(TDCA_DELETE) ||
			mapAttribIDs.HasOnly(TDCA_UNDO) ||
			mapAttribIDs.HasOnly(TDCA_PASTE) ||
			mapAttribIDs.HasOnly(TDCA_MERGE) ||
			mapAttribIDs.HasOnly(TDCA_ARCHIVE) ||
			mapAttribIDs.HasOnly(TDCA_PROJECTNAME) ||
			mapAttribIDs.HasOnly(TDCA_ENCRYPT) ||
			mapAttribIDs.HasOnly(TDCA_POSITION) ||
			mapAttribIDs.HasOnly(TDCA_POSITION_SAMEPARENT) ||
			mapAttribIDs.HasOnly(TDCA_POSITION_DIFFERENTPARENT));

	FTC_VIEW nView = GetTaskView();

	if (IsExtensionView(nView))
	{
		VIEWDATA* pVData = GetViewData(nView);
		IUIExtensionWindow* pExtWnd = GetExtensionWnd(nView);

		if (pVData && pExtWnd)
		{
			IUI_UPDATETYPE nUpdate = TDC::MapAttributesToIUIUpdateType(mapAttribIDs);

			CTaskFile tasks;
			BOOL bUpdateView = GetAllTasksForExtensionViewUpdate(pVData->mapWantedAttrib, tasks);

			// Special case: we've just deleted the last task or
			// just undone the first new task
			if (!bUpdateView)
				bUpdateView = (!m_taskTree.GetItemCount() && (mapAttribIDs.Has(TDCA_DELETE) || mapAttribIDs.Has(TDCA_UNDO)));

			if (bUpdateView)
			{
				CWaitCursor cursor;
				BeginExtensionProgress(pVData);

				// update all tasks
				pVData->bNeedFullTaskUpdate = FALSE;
				UpdateExtensionView(pExtWnd, tasks, nUpdate);

				// The only reason we DON't resync the selection is if we've just deleted
				// a task, resulting in no selection, but we know that another task selection
				// is pending because there are still tasks in the tree
				if (mapAttribIDs.Has(TDCA_DELETE) && !GetSelectedTaskID() && m_taskTree.GetItemCount())
				{
					// do nothing
				}
				else
				{
					SyncExtensionSelectionToTree(nView);
				}

				EndExtensionProgress();
			}
		}
	}

	// Mark all the rest as needing a full update
	SetExtensionsNeedTaskUpdate(TRUE, nView);
}

void CTabbedToDoCtrl::UpdateExtensionViewsSelection(const CTDCAttributeMap& mapAttribIDs)
{
	// Sanity checks
	ASSERT(HasAnyExtensionViews());

	ASSERT(!mapAttribIDs.Has(TDCA_DELETE) &&
			!mapAttribIDs.Has(TDCA_UNDO) &&
			!mapAttribIDs.Has(TDCA_PASTE) &&
			!mapAttribIDs.Has(TDCA_MERGE) &&
			!mapAttribIDs.Has(TDCA_ARCHIVE) &&
			!mapAttribIDs.Has(TDCA_PROJECTNAME) &&
			!mapAttribIDs.Has(TDCA_ENCRYPT) &&
			!mapAttribIDs.Has(TDCA_POSITION) &&
			!mapAttribIDs.Has(TDCA_POSITION_SAMEPARENT) &&
			!mapAttribIDs.Has(TDCA_POSITION_DIFFERENTPARENT));

	// If all extension views require a full task update
	// then we've nothing to do
	if (AllExtensionViewsNeedFullUpdate())
	{
		ASSERT(!IsExtensionView(GetTaskView()));
		return;
	}

	// Work out what tasks we want
	DWORD dwFlags = TDCGSTF_RESOLVEREFERENCES;
	IUI_UPDATETYPE nUpdate = IUI_EDIT; // default
	
	if (mapAttribIDs.Has(TDCA_NEWTASK))
	{
		// Always include parent chain and subtasks
		dwFlags |= TDCGSTF_ALLPARENTS;

		// Special update type
		nUpdate = IUI_NEW;
	}
	else
	{
		// We don't need to proceed if no extension wants 
		// any of the changes
		if (!AnyExtensionViewWantsChanges(mapAttribIDs))
			return;

		// Include parents if this is an undo 
		// OR there is a colour change
		// OR a calculated attribute change
		BOOL bUndo = mapAttribIDs.Has(TDCA_ALL);
		ASSERT(!bUndo || mapAttribIDs.HasOnly(TDCA_ALL));

		if (bUndo || 
			mapAttribIDs.Has(TDCA_COLOR) || 
			ModAffectsAggregatedAttributes(mapAttribIDs))
		{
			dwFlags |= TDCGSTF_ALLPARENTS;
		}

		// DONT include subtasks UNLESS the completion date
		// has changed OR this is an inherited attribute
		if (!mapAttribIDs.Has(TDCA_DONEDATE) && 
			!WantUpdateInheritedAttibutes(mapAttribIDs))
		{
			dwFlags |= TDCGSTF_NOTSUBTASKS;
		}

		// Include references to selected tasks if a 
		// 'Reference-specific' colour is not set
		if (mapAttribIDs.Has(TDCA_COLOR) && 
			!m_taskTree.HasReferenceTaskColor())
		{
			dwFlags |= TDCGSTF_APPENDREFERENCES;
		}
	
		if (mapAttribIDs.Has(TDCA_DEPENDENCY))
			dwFlags |= TDCGSTF_LOCALDEPENDENTS;
	}

	// Get the tasks for the update
	CTaskFile tasks;
	GetSelectedTasksForExtensionViewUpdate(mapAttribIDs, dwFlags, tasks);
	
	// refresh all extensions 
	int nExt = m_aExtViews.GetSize(), nNumUpdated = 0;
	
	while (nExt--)
	{
		FTC_VIEW nExtView = GetExtensionView(nExt);
		VIEWDATA* pVData = GetViewData(nExtView);
		ASSERT(pVData);

		if (pVData && pVData->WantAnyAttribute(mapAttribIDs))
		{
			IUIExtensionWindow* pExtWnd = m_aExtViews[nExt];

			if (pExtWnd)
			{
				pVData->bNeedFullTaskUpdate = FALSE;

				UpdateExtensionView(pExtWnd, tasks, nUpdate);
				nNumUpdated++;

				if (nExtView == GetTaskView())
					SyncExtensionSelectionToTree(nExtView);
			}
			else
			{
				pVData->bNeedFullTaskUpdate = TRUE;
			}
		}
	}

#ifdef _DEBUG
	CString sAttrib;

	switch (mapAttribIDs.GetCount())
	{
	case 0:
		ASSERT(0);
		break;

	case 1:
		{
			POSITION pos = mapAttribIDs.GetStartPosition();
			sAttrib = TDC::GetAttributeName(mapAttribIDs.GetNext(pos));
		}
		break;

	default: // > 1
		sAttrib = _T("multiple attributes");
		break;
	}

	TRACE(_T("UpdateExtensionViewsSelection(%s) => %d plugins updated\n"), sAttrib, nNumUpdated);
#endif
}

BOOL CTabbedToDoCtrl::ModAffectsAggregatedAttributes(const CTDCAttributeMap& mapAttribIDs) const
{
	POSITION pos = mapAttribIDs.GetStartPosition();

	while (pos)
	{
		if (ModAffectsAggregatedAttributes(mapAttribIDs.GetNext(pos)))
			return TRUE;
	}

	return FALSE;
}

BOOL CTabbedToDoCtrl::WantUpdateInheritedAttibutes(const CTDCAttributeMap& mapAttribIDs) const
{
	POSITION pos = mapAttribIDs.GetStartPosition();

	while (pos)
	{
		if (m_data.WantUpdateInheritedAttibute(mapAttribIDs.GetNext(pos)))
			return TRUE;
	}

	return FALSE;
}

BOOL CTabbedToDoCtrl::ModAffectsAggregatedAttributes(TDC_ATTRIBUTE nAttrib) const
{
	if (m_calculator.IsAggregatedAttribute(nAttrib))
		return TRUE;

	// Cross dependencies
	switch (nAttrib)
	{
	case TDCA_DONEDATE:
		if (HasStyle(TDCS_AVERAGEPERCENTSUBCOMPLETION) && 
			HasStyle(TDCS_INCLUDEDONEINAVERAGECALC))
		{
			return TRUE;
		}

		if (HasStyle(TDCS_USEHIGHESTPRIORITY) &&
			HasStyle(TDCS_DONEHAVELOWESTPRIORITY) && 
			HasStyle(TDCS_INCLUDEDONEINPRIORITYCALC))
		{
			return TRUE;
		}
		
		if (HasStyle(TDCS_USEHIGHESTRISK) &&
			HasStyle(TDCS_DONEHAVELOWESTRISK) && 
			HasStyle(TDCS_INCLUDEDONEINRISKCALC))
		{
			return TRUE;
		}
		break;

	case TDCA_DUEDATE:
		return (HasStyle(TDCS_DUEHAVEHIGHESTPRIORITY) && 
				HasStyle(TDCS_USEHIGHESTPRIORITY));
	}

	// all else
	return FALSE;
}

void CTabbedToDoCtrl::AddGlobalsToTaskFile(CTaskFile& tasks, const CTDCAttributeMap& mapAttrib) const
{
	TDCAUTOLISTDATA tld;
	POSITION pos = mapAttrib.GetStartPosition();
	
	while (pos)
	{
		TDC_ATTRIBUTE nAttrib = mapAttrib.GetNext(pos);
		GetAutoListData(tld, nAttrib);
	}
	
	if (tld.GetSize())
		tasks.SetAutoListData(tld);
}

BOOL CTabbedToDoCtrl::ExtensionViewWantsChanges(int nExt, const CTDCAttributeMap& mapAttrib) const
{
	CTDCAttributeMap mapUnused;

	return (GetExtensionViewWantedChanges(nExt, mapAttrib, mapUnused) > 0);
}

int CTabbedToDoCtrl::GetExtensionViewWantedChanges(int nExt, const CTDCAttributeMap& mapAttrib, CTDCAttributeMap& mapAttribsWanted) const
{
	ASSERT (!mapAttrib.IsEmpty());
	mapAttribsWanted.RemoveAll();

	POSITION pos = mapAttrib.GetStartPosition();
	
	while (pos)
	{
		TDC_ATTRIBUTE nAttrib = mapAttrib.GetNext(pos);
		CTDCAttributeMap mapExtAttribsWanted;

		if (ExtensionViewWantsChange(nExt, nAttrib))
			mapAttribsWanted.Add(nAttrib);
	}

	return mapAttribsWanted.GetCount();
}

BOOL CTabbedToDoCtrl::ExtensionViewWantsChange(int nExt, TDC_ATTRIBUTE nAttrib) const
{
	FTC_VIEW nCurView = GetTaskView();
	FTC_VIEW nExtView = GetExtensionView(nExt);

	const VIEWDATA* pVData = GetViewData(nExtView);

	// if the window is not active and is already marked
	// for a full update then we don't need to do
	// anything more because it will get this update when
	// it is next activated
	if (nExtView != nCurView)
	{
		if (!pVData || pVData->bNeedFullTaskUpdate)
			return FALSE;
	}
	else if (m_nExtModifyingAttrib != TDCA_NONE) // active view
	{
		// if this update has come about as a consequence
		// of this extension window modifying the specified
		// attribute, then we assume that it won't want the update.
		//
		// Unless the modification may also have auto-modified
		// the task's subtasks.
		if (AttributeMatchesExtensionMod(nAttrib) &&
			(nAttrib != TDCA_DONEDATE) &&
			!m_data.WantUpdateInheritedAttibute(nAttrib))
		{
			return FALSE;
		}
	}

	// if it's 'all' attributes then assume the answer is always 'yes'
	if (nAttrib == TDCA_ALL)
		return TRUE;
	
	return pVData->WantAttribute(nAttrib);
}

BOOL CTabbedToDoCtrl::AttributeMatchesExtensionMod(TDC_ATTRIBUTE nAttrib) const
{
	if ((m_nExtModifyingAttrib == TDCA_NONE) || (nAttrib == TDCA_NONE))
	{
		ASSERT(0);
		return FALSE;
	}

	if (nAttrib == m_nExtModifyingAttrib)
		return TRUE;

	// extra handling
	switch (m_nExtModifyingAttrib)
	{
	case TDCA_OFFSETTASK:
		return (nAttrib == TDCA_DUEDATE || nAttrib == TDCA_STARTDATE);
	}

	// all else
	return FALSE;
}

BOOL CTabbedToDoCtrl::AllExtensionViewsNeedFullUpdate() const
{
	// find the first extension not needing a full task update
	int nExt = m_aExtViews.GetSize();
	
	while (nExt--)
	{
		FTC_VIEW nExtView = GetExtensionView(nExt);
		const VIEWDATA* pVData = GetViewData(nExtView);

		if (pVData && !pVData->bNeedFullTaskUpdate)
			return FALSE;
	}

	// not found
	return TRUE;
}

BOOL CTabbedToDoCtrl::AnyExtensionViewWantsChange(TDC_ATTRIBUTE nAttrib) const
{
	// find the first extension wanting this change
	int nExt = m_aExtViews.GetSize();
	
	while (nExt--)
	{
		if (ExtensionViewWantsChange(nExt, nAttrib))
			return TRUE;
	}

	// not found
	return FALSE;
}

BOOL CTabbedToDoCtrl::AnyExtensionViewWantsChanges(const CTDCAttributeMap& mapAttrib) const
{
	CTDCAttributeMap mapUnused;

	return (GetExtensionViewsWantedChanges(mapAttrib, mapUnused) > 0);
}

int CTabbedToDoCtrl::GetExtensionViewsWantedChanges(const CTDCAttributeMap& mapAttrib, CTDCAttributeMap& mapAttribsWanted) const
{
	ASSERT (!mapAttrib.IsEmpty());
	mapAttribsWanted.RemoveAll();

	// Mandate lock state changes
	if (mapAttrib.Has(TDCA_LOCK))
	{
		mapAttribsWanted.Add(TDCA_LOCK);
		return TRUE;
	}

	int nExt = m_aExtViews.GetSize();
	CTDCAttributeMap mapExtAttribsWanted;

	while (nExt--)
	{
		if (GetExtensionViewWantedChanges(nExt, mapAttrib, mapExtAttribsWanted))
			mapAttribsWanted.Append(mapExtAttribsWanted);
	}

	return mapAttribsWanted.GetCount();
}

void CTabbedToDoCtrl::UpdateSortStates(const CTDCAttributeMap& mapAttribIDs, BOOL bAllowResort)
{
	VIEWDATA* pLVData = GetViewData(FTCV_TASKLIST);

	BOOL bTreeNeedsResort = m_taskTree.ModsNeedResort(mapAttribIDs);
	BOOL bListNeedsResort = m_taskList.ModsNeedResort(mapAttribIDs);
	
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		{
			if (bAllowResort)
				m_bTreeNeedResort = FALSE; // already done
			else
				m_bTreeNeedResort |= bTreeNeedsResort;

			pLVData->bNeedResort |= bListNeedsResort;
		}
		break;

	case FTCV_TASKLIST:
		{
			m_bTreeNeedResort |= bTreeNeedsResort;

			if (bAllowResort)
				pLVData->bNeedResort = FALSE; // already done
			else
				pLVData->bNeedResort |= bListNeedsResort;
		}
		break;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			m_bTreeNeedResort |= bTreeNeedsResort;
			pLVData->bNeedResort |= bListNeedsResort;
		}
		break;

	default:
		ASSERT(0);
		return;
	}
	
	// resort active extension and mark other extensions 
	// as needing resorting as required
	BOOL bNewTask = mapAttribIDs.Has(TDCA_NEWTASK);
	int nExt = m_aExtViews.GetSize();

	while (nExt--)
	{
		FTC_VIEW nExtView = GetExtensionView(nExt);

		VIEWDATA* pVData = GetViewData(nExtView);
		ASSERT(pVData);

		if (bNewTask || (pVData && pVData->sort.Matches(mapAttribIDs, m_styles, m_aCustomAttribDefs)))
		{
			if ((nExtView == nView) && HasStyle(TDCS_RESORTONMODIFY))
				Resort(FALSE);
			else
				pVData->bNeedResort = TRUE;
		}
	}
}

const TDSORT& CTabbedToDoCtrl::GetSort() const 
{ 
	FTC_VIEW nView = GetTaskView();
	
	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		return m_taskTree.GetSort();
		
	case FTCV_TASKLIST:
		return m_taskList.GetSort();
		
	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			VIEWDATA* pVData = GetViewData(nView);
			ASSERT(pVData);
			
			if (pVData)
				return pVData->sort; 
		}
		break;
		
	default:
		ASSERT(0);
	}

	static TDSORT sort;
	return sort;
}

TDC_COLUMN CTabbedToDoCtrl::GetSortBy() const
{
	return GetSort().single.nBy;
}

void CTabbedToDoCtrl::GetSortBy(TDSORTCOLUMNS& sort) const
{
	sort = GetSort().multi;
}

BOOL CTabbedToDoCtrl::SelectTask(DWORD dwTaskID, BOOL bTaskLink)
{	
	// Note: We update the other views first else the call to 
	// UpdateControls will not be properly synchronised
	//
	// As a result we need to preemptively process outstanding 
	// comments in case this task will not be visible in the 
	// active view, causing SetSelectedTaskComments (called by
	// CToDoCtrl::SelectedTask) to later fail
	if (!HandleUnsavedComments())
		return FALSE;

	FTC_VIEW nView = GetTaskView();
	VIEWDATA* pVData = GetViewData(nView);
	
	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		break; // Handled at bottom

	case FTCV_TASKLIST:
		{
			ASSERT(pVData);
			pVData->bHasSelectedTask = m_taskList.SelectTask(dwTaskID);
		}
		break;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			IUIExtensionWindow* pExtWnd = GetExtensionWnd(nView);
			ASSERT(pExtWnd);

			if (pExtWnd)
			{
				ASSERT(pVData);
				pVData->bHasSelectedTask = pExtWnd->SelectTask(dwTaskID, (bTaskLink != FALSE));
			}
		}
		break;

	default:
		ASSERT(0);
	}

	return CToDoCtrl::SelectTask(dwTaskID, bTaskLink);
}

int CTabbedToDoCtrl::CacheListSelection(TDCSELECTIONCACHE& cache, BOOL bIncBreadcrumbs) const
{
	return m_taskList.CacheSelection(cache, bIncBreadcrumbs);
}

BOOL CTabbedToDoCtrl::SetTreeFont(HFONT hFont)
{
	if (CToDoCtrl::SetTreeFont(hFont))
	{
		// Update other views
		FTC_VIEW nView = GetTaskView();

		switch (nView)
		{
		case FTCV_TASKTREE:
			{
				SetListViewNeedFontUpdate(TRUE);
				SetExtensionsNeedFontUpdate(TRUE);
			}
			break;

		case FTCV_TASKLIST:
			{
				SetListViewNeedFontUpdate(FALSE);
				SetExtensionsNeedFontUpdate(TRUE);

				VERIFY(m_taskList.SetFont(hFont));
			}
			break;

		case FTCV_UIEXTENSION1:
		case FTCV_UIEXTENSION2:
		case FTCV_UIEXTENSION3:
		case FTCV_UIEXTENSION4:
		case FTCV_UIEXTENSION5:
		case FTCV_UIEXTENSION6:
		case FTCV_UIEXTENSION7:
		case FTCV_UIEXTENSION8:
		case FTCV_UIEXTENSION9:
		case FTCV_UIEXTENSION10:
		case FTCV_UIEXTENSION11:
		case FTCV_UIEXTENSION12:
		case FTCV_UIEXTENSION13:
		case FTCV_UIEXTENSION14:
		case FTCV_UIEXTENSION15:
		case FTCV_UIEXTENSION16:
			{
				IUIExtensionWindow* pExtWnd = GetExtensionWnd(nView);
				ASSERT(pExtWnd);

				VIEWDATA* pData = GetViewData(nView);
				ASSERT(pData);

				if (pExtWnd && pData)
				{
					pExtWnd->SetTaskFont(m_taskTree.GetFont());
					pData->bNeedFontUpdate = FALSE;
				}

				// mark rest of extensions needing update
				SetExtensionsNeedFontUpdate(TRUE, nView);
				SetListViewNeedFontUpdate(TRUE);
			}
			break;
		}

		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CTabbedToDoCtrl::AddView(IUIExtension* pExtension)
{
	if (!pExtension)
		return FALSE;

	// remove any existing views of this type
	RemoveView(pExtension);

	// add to tab control
	HICON hIcon = pExtension->GetIcon();
	CEnString sName(pExtension->GetMenuText());

	int nIndex = m_aExtViews.GetSize();
	FTC_VIEW nView = (FTC_VIEW)(FTCV_UIEXTENSION1 + nIndex);

	VIEWDATA* pVData = NewViewData();
	ASSERT(pVData);

	pVData->pExtension = pExtension;

	// we pass NULL for the hWnd because we are going to load
	// only on demand
	if (m_tabViews.AttachView(NULL, nView, sName, hIcon, pVData))
	{
		m_aExtViews.Add(NULL); // placeholder
		return TRUE;
	}

	return FALSE;
}

BOOL CTabbedToDoCtrl::RemoveView(IUIExtension* pExtension)
{
	// search for any views having this type
	int nView = m_aExtViews.GetSize();

	while (nView--)
	{
		IUIExtensionWindow* pExtWnd = m_aExtViews[nView];

		if (pExtWnd) // can be NULL
		{
			CString sExtType = pExtension->GetTypeID();
			CString sExtWndType = pExtWnd->GetTypeID();

			if (sExtType == sExtWndType)
			{
				VERIFY (m_tabViews.DetachView(pExtWnd->GetHwnd()));
				::DestroyWindow(pExtWnd->GetHwnd());

				m_aExtViews.RemoveAt(nView);

				return TRUE;
			}
		}
	}

	return FALSE;
}

void CTabbedToDoCtrl::OnTabCtrlRClick(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	CMenu menu;

	if (menu.LoadMenu(IDR_TASKVIEWVISIBILITY))
	{
		CMenu* pPopup = menu.GetSubMenu(0);
		CPoint ptCursor(GetMessagePos());

		// prepare list view
		// NOTE: task tree is already prepared
		pPopup->CheckMenuItem(ID_SHOWVIEW_LISTVIEW, IsListViewTabShowing() ? MF_CHECKED : 0);

		// extension views
		CUIExtensionHelper helper(ID_SHOWVIEW_UIEXTENSION1, 16);
		helper.AddAllExtensionsToMenu(pPopup, m_mgrUIExt);

		CStringArray aTypeIDs;
		GetVisibleTaskViews(aTypeIDs);

		helper.UpdateExtensionVisibilityState(pPopup, m_mgrUIExt, aTypeIDs);

		UINT nCmdID = ::TrackPopupMenu(*pPopup, TPM_RETURNCMD | TPM_LEFTALIGN | TPM_LEFTBUTTON, 
										ptCursor.x, ptCursor.y, 0, GetSafeHwnd(), NULL);

		m_tabViews.Invalidate(FALSE);
		m_tabViews.UpdateWindow();

		switch (nCmdID)
		{
		case ID_SHOWVIEW_TASKTREE:
			ASSERT(0); // this is not accessible
			break;
			
		case ID_SHOWVIEW_LISTVIEW:
			ShowListViewTab(!IsListViewTabShowing());
			break;
			
		default:
			if (helper.ProcessExtensionVisibilityMenuCmd(nCmdID, m_mgrUIExt, aTypeIDs))
				SetVisibleTaskViews(aTypeIDs);
			break;
		}
	}
	
	*pResult = 0;
}

void CTabbedToDoCtrl::OnListClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	// special case - ALT key
	if (Misc::IsKeyPressed(VK_MENU))
	{
		LPNMITEMACTIVATE pNMIA = (LPNMITEMACTIVATE)pNMHDR;

		TDC_COLUMN nColID = (TDC_COLUMN)pNMIA->iSubItem;
		UINT nCtrlID = MapColumnToCtrlID(nColID);
		
		if (nCtrlID)
		{
			// make sure the edit controls are visible
			if (m_nMaxState != TDCMS_NORMAL)
				SetMaximizeState(TDCMS_NORMAL);
			
			GetDlgItem(nCtrlID)->SetFocus();
		}
		
		return;
	}

	*pResult = 0;
}

TDC_HITTEST CTabbedToDoCtrl::HitTest(const CPoint& ptScreen) const
{
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		return CToDoCtrl::HitTest(ptScreen);

	case FTCV_TASKLIST:
		return m_taskList.HitTest(ptScreen);

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			IUIExtensionWindow* pExt = GetExtensionWnd(nView);
			ASSERT(pExt);

			if (pExt)
			{
				IUI_HITTEST nHit = pExt->HitTest(ptScreen);

				switch (nHit)
				{
				case IUI_TASKLIST:		return TDCHT_TASKLIST;
				case IUI_COLUMNHEADER:	return TDCHT_COLUMNHEADER;
				case IUI_TASK:			return TDCHT_TASK;

				case IUI_NOWHERE:
				default: // fall thru
					break;
				}
			}
		}
		break;

	default:
		ASSERT(0);
	}

	// else
	return TDCHT_NOWHERE;
}

TDC_COLUMN CTabbedToDoCtrl::HitTestColumn(const CPoint& ptScreen) const
{
	FTC_VIEW nView = GetTaskView();
	
	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		return CToDoCtrl::HitTestColumn(ptScreen);

	case FTCV_TASKLIST:
		return m_taskList.HitTestColumn(ptScreen);

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		break;
		
	default:
		ASSERT(0);
	}
	return TDCC_NONE;
}

int CTabbedToDoCtrl::GetSortableColumns(CTDCColumnIDMap& mapColIDs) const
{
	mapColIDs.RemoveAll();

	FTC_VIEW nView = GetTaskView();
	
	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
	case FTCV_TASKLIST:
		return CToDoCtrl::GetSortableColumns(mapColIDs);
		
	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			for (int nAttrib = TDCA_FIRST_ATTRIBUTE; nAttrib <= TDCA_LAST_ATTRIBUTE; nAttrib++)
			{
				TDC_ATTRIBUTE nBy = (TDC_ATTRIBUTE)nAttrib;

				if (ExtensionCanSortBy(nView, nBy))
				{
					TDC_COLUMN nColID = TDC::MapAttributeToColumn(nBy);

					if (nColID != TDCC_NONE)
						mapColIDs.Add(nColID);
				}
			}
		}
		break;
		
	default:
		ASSERT(0);
	}

	return mapColIDs.GetCount();
}

void CTabbedToDoCtrl::Resort(BOOL bAllowToggle)
{
	// Don't resort the list view or extensions 
	// until after their load is complete
	if (m_bLoadingTasks && !InTreeView())
		return;

	FTC_VIEW nView = GetTaskView();
	
	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		CToDoCtrl::Resort(bAllowToggle);
		break;
		
	case FTCV_TASKLIST:
		ResortList(bAllowToggle);
		break;
		
	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		if (IsMultiSorting() || !bAllowToggle)
		{
			RefreshExtensionViewSort(nView);
		}
		else
		{
			VIEWDATA* pVData = GetViewData(nView);
			ASSERT(pVData);

			if (pVData)
				Sort(pVData->sort.single.nBy, bAllowToggle);
		}
		break;
		
	default:
		ASSERT(0);
	}
}

BOOL CTabbedToDoCtrl::IsSortingBy(TDC_COLUMN nBy) const
{
	FTC_VIEW nView = GetTaskView();
	
	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		return CToDoCtrl::IsSortingBy(nBy);
		
	case FTCV_TASKLIST:
		return m_taskList.IsSortingBy(nBy);
		
	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		return FALSE;
		
	default:
		ASSERT(0);
	}

	return FALSE;
}

BOOL CTabbedToDoCtrl::IsSorting() const
{
	return GetSort().IsSorting();
}

BOOL CTabbedToDoCtrl::IsMultiSorting() const
{
	return GetSort().bMulti;
}

void CTabbedToDoCtrl::MultiSort(const TDSORTCOLUMNS& sort)
{
	if (!sort.IsSorting() || !CanMultiSort())
	{
		ASSERT(0);
		return;
	}

	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		CToDoCtrl::MultiSort(sort);
		break;

	case FTCV_TASKLIST:
		m_taskList.MultiSort(sort);
		break;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			VIEWDATA* pVData = GetViewData(nView);

			if (pVData)
			{
				pVData->sort.SetSortBy(sort);
				RefreshExtensionViewSort(nView);
			}
		}
		break;

	default:
		ASSERT(0);
	}
}

void CTabbedToDoCtrl::RefreshExtensionViewSort(FTC_VIEW nView)
{
	VIEWDATA* pVData = GetViewData(nView);

	if (!pVData)
	{
		ASSERT(0);
		return;
	}

	if (pVData->sort.IsSorting())
	{
		if (pVData->sort.bMulti)
		{
			IUIMULTISORT multiSort;
			TDC::MapSortColumnsToIUIMultiSort(pVData->sort.multi.Cols(), multiSort);

			CUIExtensionAppCmdData data(multiSort);
			ExtensionDoAppCommand(nView, IUI_MULTISORT, data);
		}
		else
		{
			TDC_ATTRIBUTE nAttrib = TDC::MapColumnToAttribute(pVData->sort.single.nBy);
			CUIExtensionAppCmdData data(nAttrib, pVData->sort.single.bAscending);

			ExtensionDoAppCommand(nView, IUI_SORT, data);
		}
	}
	else
	{
		CUIExtensionAppCmdData data(TDCA_NONE, TRUE);
		ExtensionDoAppCommand(nView, IUI_SORT, data);
	}

	pVData->bNeedResort = FALSE;
}

BOOL CTabbedToDoCtrl::CanMultiSort() const
{
	FTC_VIEW nView = GetTaskView();
	
	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
	case FTCV_TASKLIST:
		return TRUE;
		
	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		return ExtensionCanDoAppCommand(nView, IUI_MULTISORT);
	}
	
	// all else
	ASSERT(0);
	return FALSE;
}

void CTabbedToDoCtrl::Sort(TDC_COLUMN nBy, BOOL bAllowToggle)
{
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		CToDoCtrl::Sort(nBy, bAllowToggle);
		break;

	case FTCV_TASKLIST:
		m_taskList.Sort(nBy, bAllowToggle);
		break;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			TDC_ATTRIBUTE nAttrib = TDC::MapColumnToAttribute(nBy);
			
			if ((nAttrib != TDCA_NONE) || (nBy == TDCC_NONE))
			{
				VIEWDATA* pVData = GetViewData(nView);
				ASSERT(pVData);

				BOOL bSortAscending = pVData->sort.single.bAscending;

				if (nBy != TDCC_NONE)
				{
					if (bSortAscending == -1)
					{
						bSortAscending = TRUE;
					}
					else if (bAllowToggle)
					{
						bSortAscending = !bSortAscending;
					}
				}
				else
				{
					bSortAscending = -1; // reset
				}
				 				
				CUIExtensionAppCmdData data(nAttrib, bSortAscending);

				if (ExtensionDoAppCommand(nView, IUI_SORT, data))
				{
					pVData->sort.single.nBy = nBy;
					pVData->sort.single.bAscending = bSortAscending;
				}

				pVData->bNeedResort = FALSE;
			}
		}
		break;

	default:
		ASSERT(0);
	}
}

BOOL CTabbedToDoCtrl::CanSortBy(TDC_COLUMN nBy) const
{
	FTC_VIEW nView = GetTaskView();
	
	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		return CToDoCtrl::CanSortBy(nBy);
		
	case FTCV_TASKLIST:
		return m_taskList.CanSortBy(nBy);
		
	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			TDC_ATTRIBUTE nAttrib = TDC::MapColumnToAttribute(nBy);

			if ((nAttrib == TDCA_NONE) && (nBy != TDCC_NONE))
				return FALSE;

			return ExtensionCanSortBy(nView, nAttrib);
		}
	}
	
	// else
	ASSERT(0);
	return FALSE;
}

BOOL CTabbedToDoCtrl::ExtensionCanSortBy(FTC_VIEW nView, TDC_ATTRIBUTE nBy) const
{
	if (!IsExtensionView(nView))
	{
		ASSERT(0);
		return FALSE;
	}

	// Custom attributes not currently supported
	if (nBy == TDCA_CUSTOMATTRIB)
		return FALSE;

	// all else
	CUIExtensionAppCmdData data(nBy);

	return ExtensionCanDoAppCommand(nView, IUI_SORT, data);
}

BOOL CTabbedToDoCtrl::CanMoveSelectedTask(TDC_MOVETASK nDirection) const 
{ 
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		return CToDoCtrl::CanMoveSelectedTask(nDirection);

	case FTCV_TASKLIST:
		// We don't allow any moving/dragging in the list view 
		// because is not clear where the task might end up
		return FALSE;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			if (!IsSelectedTaskMoveEnabled(TDCM_NONDRAG))
				return FALSE;

			DWORD dwSelTaskID = ((m_taskTree.GetSelectedCount() == 1) ? GetSelectedTaskID() : 0);
			DWORD dwDestParentID = 0, dwDestPrevSiblingID = 0;

			if (!GetExtensionInsertLocation(nView, nDirection, dwDestParentID, dwDestPrevSiblingID))
				return FALSE;
			
			CUIExtensionAppCmdData data(dwSelTaskID, dwDestParentID, dwDestPrevSiblingID);
			
			if (!ExtensionCanDoAppCommand(nView, IUI_MOVETASK, data))
				return FALSE;

			// Target check
			return IsValidSelectedTaskMoveTarget(dwDestParentID, DD_DROPEFFECT_MOVE);
		}
		break;
	}
	
	// else
	ASSERT(0);
	return FALSE;
}

BOOL CTabbedToDoCtrl::MoveSelectedTask(TDC_MOVETASK nDirection) 
{ 
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		return CToDoCtrl::MoveSelectedTask(nDirection);

	case FTCV_TASKLIST:
		return FALSE;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			if (!IsSelectedTaskMoveEnabled(TDCM_NONDRAG))
			{
				ASSERT(0);
				return FALSE;
			}

			////////////////////////////////////////////////////////////////
			CScopedLogTimer log(_T("CTabbedToDoCtrl::MoveSelectedTask"));
			log.LogStart();
			////////////////////////////////////////////////////////////////

			DWORD dwSelTaskID = ((m_taskTree.GetSelectedCount() == 1) ? GetSelectedTaskID() : 0);
			DWORD dwDestParentID = 0, dwDestPrevSiblingID = 0;

			if (!GetExtensionInsertLocation(nView, nDirection, dwDestParentID, dwDestPrevSiblingID))
				return FALSE;

			////////////////////////////////////////////////////////////////
			log.LogTimeElapsed(_T("GetExtensionInsertLocation"));
			////////////////////////////////////////////////////////////////

			CUIExtensionAppCmdData data(dwSelTaskID, dwDestParentID, dwDestPrevSiblingID);

			if (ExtensionDoAppCommand(nView, IUI_MOVETASK, data))
			{
				////////////////////////////////////////////////////////////////
				log.LogTimeElapsed(_T("ExtensionDoAppCommand(IUI_MOVETASK)"));
				////////////////////////////////////////////////////////////////

				IMPLEMENT_DATA_UNDO(m_data, TDCUAT_MOVE);

				// Update the underlying data
				CDWordArray aSelTaskIDs;
				m_taskTree.GetSelectedTaskIDs(aSelTaskIDs, FALSE);

				// Update the tree first because it relies on the current
				// data structure to validate the move
				HTREEITEM htiDestParent = m_taskTree.GetItem(dwDestParentID);
				HTREEITEM htiDestPrevSibling = m_taskTree.GetItem(dwDestPrevSiblingID);
				
				if (!m_taskTree.MoveSelection(htiDestParent, htiDestPrevSibling))
				{
					ASSERT(0);
					return FALSE;
				}
				
				////////////////////////////////////////////////////////////////
				log.LogTimeElapsed(_T("m_taskTree.MoveSelection"));
				////////////////////////////////////////////////////////////////
				
				if (!m_data.MoveTasks(aSelTaskIDs, dwDestParentID, dwDestPrevSiblingID))
				{
					ASSERT(0);
					return FALSE;
				}

				// Enable the move to be saved
				if (nDirection == TDCM_DOWN || nDirection == TDCM_UP)
					CToDoCtrl::SetModified(TDCA_POSITION_SAMEPARENT, aSelTaskIDs);
				else
					CToDoCtrl::SetModified(TDCA_POSITION_DIFFERENTPARENT, aSelTaskIDs);

				// Mark _other_ extensions as requiring full update
				SetExtensionsNeedTaskUpdate(TRUE, nView);

				return TRUE;
			}
		}
		break;
	}
	
	// else
	ASSERT(0);
	return FALSE;
}

BOOL CTabbedToDoCtrl::GetExtensionInsertLocation(FTC_VIEW nView, TDC_MOVETASK nDirection, DWORD& dwDestParentID, DWORD& dwDestPrevSiblingID) const
{
	if (!IsExtensionView(nView))
	{
		ASSERT(0);
		return FALSE;
	}
	
	DWORD dwSelTaskID = GetSelectedTaskID();

	switch (nDirection)
	{
	case TDCM_DOWN:
		{
			dwDestParentID = GetSelectedTaskParentID();
			dwDestPrevSiblingID = GetNextTaskID(dwSelTaskID, TTCNT_NEXT, TRUE);

			if (!ValidatePreviousSiblingTaskID(dwSelTaskID, dwDestPrevSiblingID))
				return FALSE;
		}
		break;

	case TDCM_UP:
		{
			dwDestParentID = GetSelectedTaskParentID();
			dwDestPrevSiblingID = GetNextTaskID(dwSelTaskID, TTCNT_PREV, TRUE);

			// Can't be parent because we need to look two tasks above
			if (dwDestPrevSiblingID == dwDestParentID)
				return FALSE;

			if (!ValidatePreviousSiblingTaskID(dwSelTaskID, dwDestPrevSiblingID))
				return FALSE;

			dwDestPrevSiblingID = GetNextTaskID(dwDestPrevSiblingID, TTCNT_PREV, TRUE);

			// If this is the parent task we set the sibling to zero
			// so that the task is added to the top
			if (!dwDestPrevSiblingID || (dwDestPrevSiblingID == dwDestParentID))
			{
				dwDestPrevSiblingID = 0;
			}
			else if (m_data.GetTaskParentID(dwDestPrevSiblingID) != dwDestParentID)
			{
				return FALSE;
			}
		}
		break;

	case TDCM_LEFT:
	case TDCM_RIGHT:
		// same as tree
		return m_taskTree.GetInsertLocation(nDirection, dwDestParentID, dwDestPrevSiblingID);
	}

	return TRUE;
}

BOOL CTabbedToDoCtrl::ValidatePreviousSiblingTaskID(DWORD dwTaskID, DWORD& dwPrevSiblingID) const
{
	DWORD dwOtherTaskID = dwPrevSiblingID;

	while (dwOtherTaskID != 0)
	{
		if (m_data.TaskHasSibling(dwTaskID, dwOtherTaskID, FALSE))
		{
			dwPrevSiblingID = dwOtherTaskID;
			return TRUE;
		}

		// If it's not sibling, see if it's parent is a sibling
		dwOtherTaskID = m_data.GetTaskParentID(dwOtherTaskID);
	}

	return FALSE;
}

BOOL CTabbedToDoCtrl::GotoNextTask(TDC_GOTO nDirection)
{
	if (!CanGotoNextTask(nDirection))
	{
		return FALSE;
	}

	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		return CToDoCtrl::GotoNextTask(nDirection);

	case FTCV_TASKLIST:
	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			DWORD dwTaskID = GetSelectedTaskID();
			DWORD dwNextID = GetNextTaskID(dwTaskID, MapGotoToGetNext(nDirection, FALSE), FALSE);

			if (dwNextID != dwTaskID)
				return SelectTask(dwNextID, FALSE);
		}
		break;

	default:
		ASSERT(0);
	}
	
	// else
	return FALSE;
}

TTC_NEXTTASK CTabbedToDoCtrl::MapGotoToGetNext(TDC_GOTO nDirection, BOOL bTopLevel)
{
	switch (nDirection)
	{
	case TDCG_NEXT:
		return (bTopLevel ? TTCNT_NEXTTOPLEVEL : TTCNT_NEXTVISIBLE);

	case TDCG_PREV:
		return (bTopLevel ? TTCNT_PREVTOPLEVEL : TTCNT_PREVVISIBLE);
	}

	ASSERT(0);
	return (TTC_NEXTTASK)-1;
}

BOOL CTabbedToDoCtrl::CanGotoNextTask(TDC_GOTO nDirection) const
{
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		return CToDoCtrl::CanGotoNextTask(nDirection);

	case FTCV_TASKLIST:
	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			DWORD dwTaskID = GetSelectedTaskID();
			DWORD dwNextID = GetNextTaskID(dwTaskID, MapGotoToGetNext(nDirection, FALSE), FALSE);
			
			return (dwNextID && (dwNextID != dwTaskID));
		}
		break;

	default:
		ASSERT(0);
	}
	
	// else
	return FALSE;
}

BOOL CTabbedToDoCtrl::GotoNextTopLevelTask(TDC_GOTO nDirection)
{
	ASSERT(CanGotoNextTopLevelTask(nDirection));

	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		return CToDoCtrl::GotoNextTopLevelTask(nDirection);

	case FTCV_TASKLIST:
	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			DWORD dwTaskID = GetSelectedTaskID();
			DWORD dwNextID = GetNextTaskID(dwTaskID, MapGotoToGetNext(nDirection, TRUE), FALSE);
			
			if (dwNextID != dwTaskID)
				return SelectTask(dwNextID, FALSE);
		}
		break;

	default:
		ASSERT(0);
	}

	// else
	return FALSE; // not supported
}

BOOL CTabbedToDoCtrl::CanGotoNextTopLevelTask(TDC_GOTO nDirection) const
{
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		return CToDoCtrl::CanGotoNextTopLevelTask(nDirection);

	case FTCV_TASKLIST:
	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			DWORD dwTaskID = GetSelectedTaskID();
			DWORD dwNextID = GetNextTaskID(dwTaskID, MapGotoToGetNext(nDirection, TRUE), FALSE);
			
			return (dwNextID && (dwNextID != dwTaskID));
		}
		break;

	default:
		ASSERT(0);
	}

	// else
	return FALSE; // not supported
}

void CTabbedToDoCtrl::ExpandTasks(TDC_EXPANDCOLLAPSE nWhat, BOOL bExpand)
{
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		CToDoCtrl::ExpandTasks(nWhat, bExpand);

	case FTCV_TASKLIST:
		// no can do!
		break;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		if (bExpand)
		{
			switch (nWhat)
			{
			case TDCEC_ALL:
				ExtensionDoAppCommand(nView, IUI_EXPANDALL);
				break;

			case TDCEC_SELECTED:
				ExtensionDoAppCommand(nView, IUI_EXPANDSELECTED);
				break;
			}
		}
		else // collapse
		{
			switch (nWhat)
			{
			case TDCEC_ALL:
				ExtensionDoAppCommand(nView, IUI_COLLAPSEALL);
				break;

			case TDCEC_SELECTED:
				ExtensionDoAppCommand(nView, IUI_COLLAPSESELECTED);
				break;
			}
		}
		break;

	default:
		ASSERT(0);
	}
}

BOOL CTabbedToDoCtrl::CanExpandTasks(TDC_EXPANDCOLLAPSE nWhat, BOOL bExpand) const 
{ 
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		return CToDoCtrl::CanExpandTasks(nWhat, bExpand);

	case FTCV_TASKLIST:
		// no can do!
		break;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		if (bExpand)
		{
			switch (nWhat)
			{
			case TDCEC_ALL:
				return ExtensionCanDoAppCommand(nView, IUI_EXPANDALL);

			case TDCEC_SELECTED:
				return ExtensionCanDoAppCommand(nView, IUI_EXPANDSELECTED);
			}
		}
		else // collapse
		{
			switch (nWhat)
			{
			case TDCEC_ALL:
				return ExtensionCanDoAppCommand(nView, IUI_COLLAPSEALL);

			case TDCEC_SELECTED:
				return ExtensionCanDoAppCommand(nView, IUI_COLLAPSESELECTED);
			}
		}
		break;

	default:
		ASSERT(0);
	}

	// else
	return FALSE; // not supported
}

BOOL CTabbedToDoCtrl::CanCopyTaskColumnValues(TDC_COLUMN nColID, BOOL bSelectedTasksOnly) const
{
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		return CToDoCtrl::CanCopyTaskColumnValues(nColID, bSelectedTasksOnly);

	case FTCV_TASKLIST:
		return m_taskList.CanCopyTaskColumnValues(nColID, bSelectedTasksOnly);
	}
	
	// all else (for now)
	return FALSE;
}

BOOL CTabbedToDoCtrl::CopyTaskColumnValues(TDC_COLUMN nColID, BOOL bSelectedTasksOnly) const
{
	return CToDoCtrl::CopyTaskColumnValues(nColID, bSelectedTasksOnly);
}

int CTabbedToDoCtrl::CopyTaskColumnValues(TDC_COLUMN nColID, BOOL bSelectedTasksOnly, CStringArray& aValues) const
{
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		return CToDoCtrl::CopyTaskColumnValues(nColID, bSelectedTasksOnly, aValues);

	case FTCV_TASKLIST:
		return m_taskList.CopyTaskColumnValues(nColID, bSelectedTasksOnly, aValues);
	}
	
	// all else (for now)
	return 0;
}

BOOL CTabbedToDoCtrl::ExtensionDoAppCommand(FTC_VIEW nView, IUI_APPCOMMAND nCmd)
{
	IUIExtensionWindow* pExt = GetExtensionWnd(nView);
	ASSERT(pExt);

	if (pExt)
		return (pExt->DoAppCommand(nCmd) ? TRUE : FALSE);

	// else
	return FALSE;
}

BOOL CTabbedToDoCtrl::ExtensionCanDoAppCommand(FTC_VIEW nView, IUI_APPCOMMAND nCmd) const
{
	const IUIExtensionWindow* pExt = GetExtensionWnd(nView);
	ASSERT(pExt);

	if (pExt)
		return (pExt->CanDoAppCommand(nCmd) ? TRUE : FALSE);

	return FALSE;
}

BOOL CTabbedToDoCtrl::ExtensionDoAppCommand(FTC_VIEW nView, IUI_APPCOMMAND nCmd, IUIAPPCOMMANDDATA& data)
{
	IUIExtensionWindow* pExt = GetExtensionWnd(nView);
	ASSERT(pExt);

	if (pExt)
		return (pExt->DoAppCommand(nCmd, &data) ? TRUE : FALSE);

	// else
	return FALSE;
}

BOOL CTabbedToDoCtrl::ExtensionCanDoAppCommand(FTC_VIEW nView, IUI_APPCOMMAND nCmd, const IUIAPPCOMMANDDATA& data) const
{
	const IUIExtensionWindow* pExt = GetExtensionWnd(nView);
	ASSERT(pExt);

	if (pExt)
		return (pExt->CanDoAppCommand(nCmd, &data) ? TRUE : FALSE);

	return FALSE;
}

BOOL CTabbedToDoCtrl::ViewSupportsTaskSelection(FTC_VIEW nView) const
{
	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
	case FTCV_TASKLIST:
		return TRUE;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			int nExtension = (nView - FTCV_FIRSTUIEXTENSION);
			ASSERT(nExtension < m_aExtViews.GetSize());

			return (m_mgrUIExt.SupportsTaskSelection(nExtension) ? TRUE : FALSE);
		}
		break;
	}

	// all else
	ASSERT(0);
	return FALSE;
}

BOOL CTabbedToDoCtrl::ViewSupportsNewTask(FTC_VIEW nView) const
{
	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
	case FTCV_TASKLIST:
		return TRUE;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			const VIEWDATA* pData = GetViewData(nView);
			ASSERT(pData && (pData->bCanPrepareNewTask != -1));

			if (pData)
				return pData->bCanPrepareNewTask;
		}
		break;
	}

	// all else
	ASSERT(0);
	return FALSE;
}

void CTabbedToDoCtrl::SetViewNeedsTaskUpdate(FTC_VIEW nView, BOOL bUpdate)
{
	VIEWDATA* pVData = GetViewData(nView);

	if (pVData)
		pVData->bNeedFullTaskUpdate = bUpdate;
	else
		ASSERT(0);
}

BOOL CTabbedToDoCtrl::ViewHasTaskSelection(FTC_VIEW nView) const
{
	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		return TRUE;

	case FTCV_TASKLIST:
	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		if (ViewSupportsTaskSelection(nView))
		{
			return GetViewData(nView)->bHasSelectedTask;
		}
		break;

	default:
		ASSERT(0);
	}

	// all else
	return FALSE;
}

HTREEITEM CTabbedToDoCtrl::GetUpdateControlsItem() const
{
	HTREEITEM hti = CToDoCtrl::GetUpdateControlsItem();

	if (!ViewHasTaskSelection(GetTaskView()))
		hti = NULL;

	return hti;
}

CString CTabbedToDoCtrl::FormatSelectedTaskTitles(BOOL bFullPath, TCHAR cSep, int nMaxTasks) const
{
	CString sSelTasks;
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		sSelTasks = CToDoCtrl::FormatSelectedTaskTitles(bFullPath, cSep, nMaxTasks);
		break;

	case FTCV_TASKLIST:
		sSelTasks = m_taskList.FormatSelectedTaskTitles(bFullPath, cSep, nMaxTasks);
		break;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		if (GetViewData(nView)->bHasSelectedTask)
			sSelTasks = CToDoCtrl::FormatSelectedTaskTitles(bFullPath, cSep, nMaxTasks);
		break;

	default:
		ASSERT(0);
	}

	return sSelTasks;
}

int CTabbedToDoCtrl::GetSelectedTaskCount() const
{
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKLIST:
	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		if (!GetViewData(nView)->bHasSelectedTask)
			return 0;
	}

	return CToDoCtrl::GetSelectedTaskCount();
}

void CTabbedToDoCtrl::SetFocusToTasks()
{
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		CToDoCtrl::SetFocusToTasks();
		break;

	case FTCV_TASKLIST:
		if (GetFocus() != &m_taskList)
		{
			// See CToDoCtrl::SetFocusToTasks() for why we need this
			SetFocusToComments();
			
			m_taskList.SetFocus();
		}
			
		m_taskList.EnsureSelectionVisible(TRUE);
		break;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		ExtensionDoAppCommand(nView, IUI_SETFOCUS);
		break;

	default:
		ASSERT(0);
	}
}

BOOL CTabbedToDoCtrl::TasksHaveFocus() const
{ 
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		return CToDoCtrl::TasksHaveFocus(); 

	case FTCV_TASKLIST:
		return m_taskList.HasFocus();

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		return (ExtensionCanDoAppCommand(nView, IUI_SETFOCUS) == FALSE);

	default:
		ASSERT(0);
	}
	
	return FALSE;
}

int CTabbedToDoCtrl::FindTasks(const SEARCHPARAMS& params, CResultArray& aResults) const
{
	aResults.RemoveAll();
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		return CToDoCtrl::FindTasks(params, aResults);

	case FTCV_TASKLIST:
		{
			for (int nItem = 0; nItem < m_taskList.GetItemCount(); nItem++)
			{
				DWORD dwTaskID = GetTaskID(nItem);

				if (m_taskList.IsGroupHeaderTask(dwTaskID))
					continue;

				SEARCHRESULT result;

				if (m_matcher.TaskMatches(dwTaskID, params, HasDueTodayColor(), result))
					aResults.Add(result);
			}
		}
		break;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		return CToDoCtrl::FindTasks(params, aResults);
		
	default:
		ASSERT(0);
	}

	return aResults.GetSize();
}

BOOL CTabbedToDoCtrl::SelectNextTask(CString sPart, TDC_SELECTNEXTTASK nSelect)
{
	return CToDoCtrl::SelectNextTask(sPart, nSelect);
}

BOOL CTabbedToDoCtrl::CanDoFindReplace(TDC_ATTRIBUTE nAttrib) const
{
	if (!CToDoCtrl::CanDoFindReplace(nAttrib))
		return FALSE;

	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
	case FTCV_TASKLIST:
		return TRUE; // checked above

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		return ExtensionCanDoAppCommand(nView, IUI_SELECTNEXTTASK);

	default:
		ASSERT(0);
	}

	return FALSE;
}

BOOL CTabbedToDoCtrl::SelectNextTask(const CString& sPart, TDC_SELECTNEXTTASK nSelect, TDC_ATTRIBUTE nAttrib, 
									BOOL bCaseSensitive, BOOL bWholeWord, BOOL bFindReplace)
{
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		return CToDoCtrl::SelectNextTask(sPart, nSelect, nAttrib, bCaseSensitive, bWholeWord, bFindReplace);

	case FTCV_TASKLIST:
		{
			int nStart = -1;
			BOOL bForwards = TRUE;

			switch (nSelect)
			{
			case TDC_SELECTFIRST:
				nStart = 0;
				break;
			
			case TDC_SELECTNEXT:
				nStart = (m_taskList.GetSelectedItem() + 1);
				break;
			
			case TDC_SELECTNEXTINCLCURRENT:
				nStart = m_taskList.GetSelectedItem();
				break;
			
			case TDC_SELECTPREV:
				nStart = (m_taskList.GetSelectedItem() - 1);
				bForwards = FALSE;
				break;
			
			case TDC_SELECTLAST:
				nStart = (m_taskList.GetItemCount() - 1);
				bForwards = FALSE;
				break;
			}

			int nFind = FindListTask(sPart, nAttrib, nStart, bForwards, bCaseSensitive, bWholeWord);

			if (nFind != -1)
				return SelectTask(GetTaskID(nFind), FALSE);
		}
		break;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			IUI_APPCOMMAND nCmdID = TDC::MapSelectTaskToIUICommand(nSelect);
			ASSERT(nCmdID != IUI_NOCOMMAND);

			CUIExtensionAppCmdData data(TDCA_TASKNAME, bFindReplace, sPart, bCaseSensitive, bWholeWord);

			return ExtensionDoAppCommand(nView, nCmdID, data);
		}
		break;

	default:
		ASSERT(0);
	}

	// else
	return FALSE;
}

int CTabbedToDoCtrl::FindListTask(const CString& sPart, TDC_ATTRIBUTE nAttrib, int nStart, 
									BOOL bNext, BOOL bCaseSensitive, BOOL bWholeWord) const
{
	// build a search query
	SEARCHPARAM rule(nAttrib, FOP_INCLUDES, sPart);
	rule.SetMatchWholeWord(bWholeWord);

	SEARCHPARAMS params;
	params.aRules.Add(rule);
	params.bCaseSensitive = bCaseSensitive;

	// we need to do this manually because CListCtrl::FindItem 
	// only looks at the start of the string
	SEARCHRESULT result;

	int nFrom = nStart;
	int nTo = bNext ? m_taskList.GetItemCount() : -1;
	int nInc = bNext ? 1 : -1;

	for (int nItem = nFrom; nItem != nTo; nItem += nInc)
	{
		DWORD dwTaskID = GetTaskID(nItem);

		if (m_taskList.IsGroupHeaderTask(dwTaskID))
			continue;

		if (m_matcher.TaskMatches(dwTaskID, params, HasDueTodayColor(), result))
			return nItem;
	}

	return -1; // no match
}

BOOL CTabbedToDoCtrl::SelectTasks(const CDWordArray& aTaskIDs)
{
	BOOL bRes = CToDoCtrl::SelectTasks(aTaskIDs);

	// extra processing
	if (bRes)
	{
		// If this call came from CToDoCtrl::OnRecreateRecurringTask()
		// we save off the task IDs and eat it to prevent a plugin
		// from rejecting the selection because it has not yet seen
		// the new recurring tasks
		if (m_bRecreatingRecurringTasks)
		{
			m_aRecreatedRecurringTasks.Copy(aTaskIDs);
			return TRUE;
		}

		// else
		ASSERT((m_aRecreatedRecurringTasks.GetSize() == 0) ||
				(&m_aRecreatedRecurringTasks == &aTaskIDs));

		SyncActiveViewSelectionToTree();
	}

	return bRes;
}

void CTabbedToDoCtrl::SyncActiveViewSelectionToTree()
{
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		break;

	case FTCV_TASKLIST:
		SyncListSelectionToTree(FALSE);
		break;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		SyncExtensionSelectionToTree(nView);
		break;

	default:
		ASSERT(0);
	}
}

void CTabbedToDoCtrl::SyncListSelectionToTree(BOOL bEnsureSelection)
{
	ASSERT(InListView());

	BOOL bSelChange = FALSE;
	
	// optimisation when all items selected
	if (TSH().GetCount() == m_taskTree.GetItemCount())
	{
		m_taskList.SelectAll();
		bSelChange = (m_taskList.GetItemCount() != m_taskTree.GetItemCount());
	}
	else if (m_taskList.GetItemCount())
	{
		// save current states
		TDCSELECTIONCACHE cacheList, cacheTree;
		CacheListSelection(cacheList, bEnsureSelection);
		CacheTreeSelection(cacheTree, FALSE);

		if (!cacheList.SelectionMatches(cacheTree))
		{
			// save list scroll pos before restoring
			////////////////////////////////////////////////////////////////
			// I'm not clear what case this handles and since it interferes
			// with selection history I'm disabling it to see what happens
			//
			// cacheTree.dwFirstVisibleTaskID = GetTaskID(m_taskList.List().GetTopIndex());
			////////////////////////////////////////////////////////////////
			cacheTree.dwFirstVisibleTaskID = 0;

			if (m_taskList.RestoreSelection(cacheTree, bEnsureSelection))
			{
				// now check that the tree is correctly synced with us
				// but only if we have something selected
				CacheListSelection(cacheList);

				if (!cacheList.SelectionMatches(cacheTree))
				{
					RestoreTreeSelection(cacheList);
					bSelChange = TRUE;
				}
			}
			else
			{
				bSelChange = TRUE;
			}
		}
	}

	VIEWDATA* pLVData = GetViewData(FTCV_TASKLIST);
	ASSERT(pLVData);

	pLVData->bHasSelectedTask = m_taskList.GetSelectedCount();

	m_taskList.UpdateSelectedTaskPath();

	if (bSelChange)
	{
		UpdateControls(FALSE);
		NotifyParentSelectionChange();
	}
}

void CTabbedToDoCtrl::SyncExtensionSelectionToTree(FTC_VIEW nView)
{
	if (!IsExtensionView(nView))
	{
		ASSERT(0);
		return;
	}
	else if (!ViewSupportsTaskSelection(nView))
	{
		return;
	}
	else if (m_bInSelectedTaskEdit)
	{
		return;
	}

	VIEWDATA* pVData = NULL;
	IUIExtensionWindow* pExt = NULL;
	VERIFY(GetExtensionWnd(nView, pExt, pVData));

	TDCSELECTIONCACHE cache;
	CacheTreeSelection(cache);

	BOOL bSelChange = FALSE;
	BOOL bHadSelectedTask = pVData->bHasSelectedTask;

	if (cache.IsEmpty())
	{
		pVData->bHasSelectedTask = FALSE;
		bSelChange = TRUE;
	}
	else if ((cache.aSelTaskIDs.GetSize() > 1) && pExt->SelectTasks(cache.aSelTaskIDs.GetData(), cache.aSelTaskIDs.GetSize()))
	{
		// extension has same selection as tree so no need to update controls
		pVData->bHasSelectedTask = TRUE;
	}
	else
	{
		ASSERT((cache.aSelTaskIDs.GetSize() > 1) || 
				(cache.dwFocusedTaskID == cache.aSelTaskIDs[0]));

		pVData->bHasSelectedTask = pExt->SelectTask(cache.dwFocusedTaskID, false);

		if (pVData->bHasSelectedTask)
		{
			if (!bHadSelectedTask)
				bSelChange = TRUE;
			else
				bSelChange = HasSingleSelectionChanged(cache.dwFocusedTaskID);

			VERIFY(CToDoCtrl::SelectTask(cache.dwFocusedTaskID, FALSE));
		}
		else
		{
			bSelChange = TRUE;
		}
	}

	if (bSelChange)
	{
		UpdateControls();
		NotifyParentSelectionChange();
	}
}

BOOL CTabbedToDoCtrl::SelectTasksInHistory(BOOL bForward)
{
	if (!CToDoCtrl::SelectTasksInHistory(bForward))
		return FALSE;

	SyncActiveViewSelectionToTree();

	return TRUE;
}

void CTabbedToDoCtrl::InvalidateItem(HTREEITEM hti, BOOL bUpdate)
{
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		CToDoCtrl::InvalidateItem(hti, bUpdate);
		break;

	case FTCV_TASKLIST:
		m_taskList.InvalidateItem(GetListItem(hti), bUpdate);
		break;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		break;

	default:
		ASSERT(0);
	}
}

void CTabbedToDoCtrl::OnListSelChanged(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	*pResult = 0;
	OnListSelChanged();
}

void CTabbedToDoCtrl::OnListSelChanged()
{
	if (m_bDeletingTasks)
		return;

	if (!InListView())
	{
		ASSERT(0);
		return;
	}

	TDCSELECTIONCACHE cacheTree;
	CacheTreeSelection(cacheTree, FALSE);
	
	VIEWDATA* pLVData = GetViewData(FTCV_TASKLIST);
	BOOL bListHadSelection = pLVData->bHasSelectedTask;

	TDCSELECTIONCACHE cacheList;
	pLVData->bHasSelectedTask = CacheListSelection(cacheList, FALSE);

	if (!cacheTree.SelectionMatches(cacheList, TRUE) && pLVData->bHasSelectedTask)
	{
		if (cacheList.aSelTaskIDs.GetSize() == m_taskTree.GetItemCount())
			m_taskTree.SelectAll();
		else
			m_taskTree.RestoreSelection(cacheList);
	}
	else if (Misc::StateChanged(bListHadSelection, pLVData->bHasSelectedTask))
	{
		UpdateControls();
		NotifyParentSelectionChange();
	}
}

LRESULT CTabbedToDoCtrl::OnMidnight(WPARAM wParam, LPARAM lParam)
{
	// Update extensions task colours
	BOOL bWantColorUpdate = (HasAnyExtensionViews() &&
							(m_taskTree.ModCausesTaskTextColorChange(TDCA_STARTDATE) ||
							 m_taskTree.ModCausesTaskTextColorChange(TDCA_DUEDATE)));

	if (bWantColorUpdate)
	{
		CAutoFlag af(m_bUpdatingExtensions, TRUE);

		int nExt = m_aExtViews.GetSize();
		FTC_VIEW nCurView = GetTaskView();

		while (nExt--)
		{
			if (!ExtensionViewWantsChange(nExt, TDCA_COLOR))
				continue;

			FTC_VIEW nExtView = GetExtensionView(nExt);

			IUIExtensionWindow* pExtWnd = NULL;
			VIEWDATA* pVData = NULL;

			if (!GetExtensionWnd(nExtView, pExtWnd, pVData))
				continue;

			if (nExtView == nCurView)
			{
				BeginExtensionProgress(pVData);

				CTaskFile tasks;
				CWaitCursor cursor;

				if (GetAllTasksForExtensionViewUpdate(TDCA_COLOR, tasks))
				{
					pVData->bNeedFullTaskUpdate = FALSE;
					UpdateExtensionView(pExtWnd, tasks, IUI_EDIT);
				}

				EndExtensionProgress();
			}
			else
			{
				pVData->bNeedFullTaskUpdate = TRUE;
			}
		}
	}

	return CToDoCtrl::OnMidnight(wParam, lParam);
}

HTREEITEM CTabbedToDoCtrl::GetTreeItem(int nItem) const
{
	if (nItem < 0 || nItem >= m_taskList.GetItemCount())
		return NULL;

	DWORD dwID = m_taskList.GetTaskID(nItem);
	return m_taskTree.GetItem(dwID);
}

int CTabbedToDoCtrl::GetListItem(HTREEITEM hti) const
{
	DWORD dwID = GetTaskID(hti);
	return (dwID ? m_taskList.FindTaskItem(dwID) : -1);
}

LRESULT CTabbedToDoCtrl::OnCanDropObject(WPARAM wParam, LPARAM lParam)
{
	if (InListView() && !IsReadOnly())
	{
		CWnd* pTarget = (CWnd*)lParam;

		if (pTarget == &m_taskList.List())
			lParam = (LPARAM)&m_taskTree.Tree();
	}

	// all else
	return CToDoCtrl::OnCanDropObject(wParam, lParam);
}

LRESULT CTabbedToDoCtrl::OnDropObject(WPARAM wParam, LPARAM lParam)
{
	if (IsReadOnly())
		return 0L;

	TLDT_DATA* pData = (TLDT_DATA*)wParam;
	CWnd* pTarget = (CWnd*)lParam;
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		CToDoCtrl::OnDropObject(wParam, lParam);
		break;


 	case FTCV_TASKLIST:
		if (pTarget == &m_taskList.List())
		{
 			if (pData->dwTaskID > 0)
			{
				int nItem = m_taskList.FindTaskItem(pData->dwTaskID);
				ASSERT(nItem != -1);

 				m_taskList.SelectItem(nItem);
			}

			lParam = (LPARAM)&m_taskTree.Tree();
		}

		CToDoCtrl::OnDropObject(wParam, lParam);
		break;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		CToDoCtrl::OnDropObject(wParam, lParam); // default handling
		break;

	default:
		ASSERT(0);
	}

	// else
	return 0L;
}

BOOL CTabbedToDoCtrl::GetLabelEditRect(CRect& rScreen)
{
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		return CToDoCtrl::GetLabelEditRect(rScreen);

	case FTCV_TASKLIST:
		if (m_taskList.EnsureSelectionVisible(FALSE))
			return m_taskList.GetLabelEditRect(rScreen);
		break;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		{
			IUIExtensionWindow* pExt = GetExtensionWnd(nView);
			ASSERT(pExt);

			return (pExt && pExt->GetLabelEditRect(rScreen));
		}
		break;

	default:
		ASSERT(0);
	}

	return FALSE;
}

void CTabbedToDoCtrl::UpdateSelectedTaskPath()
{
	// Disable updating the task path until after
	// loading because the task may no longer exist
	if (m_bLoadingTasks)
		return;

	CToDoCtrl::UpdateSelectedTaskPath();

	// extra processing
	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		// handled above
		break;

	case FTCV_TASKLIST:
		m_taskList.UpdateSelectedTaskPath();
		break;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		break;

	default:
		ASSERT(0);
	}
}

void CTabbedToDoCtrl::SaveTasksState(CPreferences& prefs, BOOL bRebuildingTree) const
{
	if (!bRebuildingTree)
		m_taskList.SaveState(prefs, GetPreferencesKey(_T("TaskList")));
	
	// base class
	CToDoCtrl::SaveTasksState(prefs, bRebuildingTree);
}

HTREEITEM CTabbedToDoCtrl::LoadTasksState(const CPreferences& prefs, BOOL bRebuildingTree)
{
	if (!bRebuildingTree)
	{
		m_taskList.LoadState(prefs, GetPreferencesKey(_T("TaskList")));
		m_taskList.RecalcUntrackedColumnWidths();
		
		if (m_taskList.IsSorting())
			GetViewData(FTCV_TASKLIST)->bNeedResort = TRUE;
	}

	// base class
	return CToDoCtrl::LoadTasksState(prefs, bRebuildingTree);
}

BOOL CTabbedToDoCtrl::IsViewSet() const 
{ 
	return (GetTaskView() != FTCV_UNSET); 
}

BOOL CTabbedToDoCtrl::InListView() const 
{ 
	return (GetTaskView() == FTCV_TASKLIST); 
}

BOOL CTabbedToDoCtrl::InTreeView() const 
{ 
	return ((GetTaskView() == FTCV_TASKTREE) || !IsViewSet()); 
}

BOOL CTabbedToDoCtrl::InExtensionView() const
{
	return IsExtensionView(GetTaskView());
}

BOOL CTabbedToDoCtrl::IsExtensionView(FTC_VIEW nView)
{
	return ((nView >= FTCV_UIEXTENSION1) && (nView <= FTCV_UIEXTENSION16));
}

BOOL CTabbedToDoCtrl::HasAnyExtensionViews() const
{
	int nView = m_aExtViews.GetSize();

	while (nView--)
	{
		if (m_aExtViews[nView] != NULL)
			return TRUE;
	}

	// else
	return FALSE;
}

BOOL CTabbedToDoCtrl::IsExtensionView(HWND hWnd) const
{
	int nView = m_aExtViews.GetSize();

	while (nView--)
	{
		const IUIExtensionWindow* pExtWnd = m_aExtViews[nView];

		if ((pExtWnd != NULL) && (CDialogHelper::IsChildOrSame(pExtWnd->GetHwnd(), hWnd)))
			return TRUE;
	}

	// else
	return FALSE;

}

void CTabbedToDoCtrl::ShowListViewTab(BOOL bVisible)
{
	m_tabViews.ShowViewTab(FTCV_TASKLIST, bVisible);
}

BOOL CTabbedToDoCtrl::IsListViewTabShowing() const
{
	return m_tabViews.IsViewTabShowing(FTCV_TASKLIST);
}

void CTabbedToDoCtrl::SetVisibleTaskViews(const CStringArray& aTypeIDs)
{
	// Excludes the task tree which can never be hidden
	ASSERT(GetSafeHwnd());

	int nExt = m_mgrUIExt.GetNumUIExtensions();

	while (nExt--)
	{
		FTC_VIEW nView = (FTC_VIEW)(FTCV_UIEXTENSION1 + nExt);

		VIEWDATA* pVData = GetViewData(nView);
		ASSERT(pVData);

		// update tab control
		CString sTypeID = m_mgrUIExt.GetUIExtensionTypeID(nExt);
		BOOL bVisible = Misc::Contains(sTypeID, aTypeIDs, FALSE, TRUE);

		m_tabViews.ShowViewTab(nView, bVisible);
	}

	ShowListViewTab(Misc::Contains(LISTVIEW_TYPE, aTypeIDs, FALSE, TRUE));

	// Sanity check (-1 for tree)
	ASSERT((m_tabViews.GetItemCount() - 1) == aTypeIDs.GetSize());
}

int CTabbedToDoCtrl::GetVisibleTaskViews(CStringArray& aTypeIDs) const
{
	// Excludes the task tree which can never be hidden
	ASSERT(GetSafeHwnd());

	aTypeIDs.RemoveAll();

	int nExt = m_mgrUIExt.GetNumUIExtensions();

	while (nExt--)
	{
		FTC_VIEW nView = (FTC_VIEW)(FTCV_UIEXTENSION1 + nExt);

		if (m_tabViews.IsViewTabShowing(nView))
			aTypeIDs.Add(m_mgrUIExt.GetUIExtensionTypeID(nExt));
	}

	if (IsListViewTabShowing())
		aTypeIDs.Add(LISTVIEW_TYPE);

	// Sanity check (-1 for tree)
	ASSERT((m_tabViews.GetItemCount() - 1) == aTypeIDs.GetSize());

	return aTypeIDs.GetSize();
}

BOOL CTabbedToDoCtrl::CanResizeAttributeColumnsToFit() const
{
	FTC_VIEW nView = GetTaskView();
	
	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
	case FTCV_TASKLIST:
		return !GetVisibleColumns().IsEmpty();
		
	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		return ExtensionCanDoAppCommand(nView, IUI_RESIZEATTRIBCOLUMNS);
		
	default:
		ASSERT(0);
	}

	return FALSE;
}

void CTabbedToDoCtrl::ResizeAttributeColumnsToFit()
{
	FTC_VIEW nView = GetTaskView();
	
	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		CToDoCtrl::ResizeAttributeColumnsToFit();
		break;
		
	case FTCV_TASKLIST:
		m_taskList.RecalcAllColumnWidths();
		break;
		
	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		ExtensionDoAppCommand(nView, IUI_RESIZEATTRIBCOLUMNS);
		break;
		
	default:
		ASSERT(0);
	}
}

void CTabbedToDoCtrl::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// We only handle this if it came from Linux
	if (COSVersion() == OSV_LINUX)
	{
		AfxGetMainWnd()->SendMessage(WM_CONTEXTMENU, (WPARAM)GetSafeHwnd(), MAKELPARAM(point.x, point.y)); 
		return;
	}

	CToDoCtrl::OnContextMenu(pWnd, point);
}

LRESULT CTabbedToDoCtrl::OnRestoreLastTaskView(WPARAM nCurView, LPARAM nNewView)
{
	if (((FTC_VIEW)nNewView != FTCV_UNSET) && ((FTC_VIEW)nNewView != (FTC_VIEW)nCurView))
		SetTaskView((FTC_VIEW)nNewView);
	
	return 0L;
}

LRESULT CTabbedToDoCtrl::OnRecreateRecurringTask(WPARAM wParam, LPARAM lParam)
{
	// The default implementation calls SelectTasks with the new
	// task IDs before calling SetModified which can result in
	// one of more plugins rejecting the SelectTasks call because
	// they have not yet seen the new tasks. Just a consequence of
	// CToDoCtrl having no knowledge of the plugins.

	// So we workaround it by not passing on the SelectTasks call
	// and updating the selection only after the SetModified call.
	{
		CAutoFlag af(m_bRecreatingRecurringTasks, TRUE);
		
		CToDoCtrl::OnRecreateRecurringTask(wParam, lParam);
	}

	if (!InTreeView() && m_aRecreatedRecurringTasks.GetSize())
		SelectTasks(m_aRecreatedRecurringTasks);

	// cleanup
	m_aRecreatedRecurringTasks.RemoveAll();

	return 0L;
}

BOOL CTabbedToDoCtrl::SaveTaskViewToImage(const CString& sFilePath)
{
	if (!CanSaveTaskViewToImage())
		return FALSE;

	FTC_VIEW nView = GetTaskView();
	
	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		return CToDoCtrl::SaveTaskViewToImage(sFilePath);
		
	case FTCV_TASKLIST:
		{
			CBitmap bmImage;

			if (m_taskList.SaveToImage(bmImage))
				return (CGdiPlusBitmap(bmImage).SaveAsFile(sFilePath) != FALSE);
		}
		break;
		
	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		return ExtensionDoAppCommand(nView, IUI_SAVETOIMAGE, CUIExtensionAppCmdData(sFilePath));
		
	default:
		ASSERT(0);
	}
	
	return FALSE;
}

BOOL CTabbedToDoCtrl::CanSaveTaskViewToImage() const
{
	if (!CToDoCtrl::CanSaveTaskViewToImage())
		return FALSE;

	FTC_VIEW nView = GetTaskView();
	
	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		return TRUE; // handle above
		
	case FTCV_TASKLIST:
		return m_taskList.CanSaveToImage();
		
	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		return ExtensionCanDoAppCommand(nView, IUI_SAVETOIMAGE);
		
	default:
		ASSERT(0);
	}
	
	return FALSE;
}

