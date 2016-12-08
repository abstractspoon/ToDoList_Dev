// Fi M_BlISlteredToDoCtrl.cpp: implementation of the CTabbedToDoCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TabbedToDoCtrl.h"
#include "todoitem.h"
#include "resource.h"
#include "tdcstatic.h"
#include "tdcmsg.h"
#include "tdccustomattributehelper.h"
#include "tdltaskicondlg.h"
#include "tdcuiextensionhelper.h"

#include "..\shared\holdredraw.h"
#include "..\shared\datehelper.h"
#include "..\shared\enstring.h"
#include "..\shared\preferences.h"
#include "..\shared\deferwndmove.h"
#include "..\shared\autoflag.h"
#include "..\shared\holdredraw.h"
#include "..\shared\osversion.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\uiextensionmgr.h"
#include "..\shared\filemisc.h"

#include "..\Interfaces\iuiextension.h"

#include <math.h>

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
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTabbedToDoCtrl::CTabbedToDoCtrl(CUIExtensionMgr& mgrUIExt, CContentMgr& mgrContent, 
								 const CONTENTFORMAT& cfDefault, const TDCCOLEDITFILTERVISIBILITY& visDefault) 
	:
	CToDoCtrl(mgrContent, cfDefault, visDefault), 
	m_bTreeNeedResort(FALSE),
	m_bTaskColorChange(FALSE),
	m_bUpdatingExtensions(FALSE),
	m_bRecreatingRecurringTasks(FALSE),
	m_nExtModifyingAttrib(IUI_NONE),
	m_mgrUIExt(mgrUIExt),
	m_taskList(m_ilTaskIcons, m_data, TCF(), m_aStyles, m_visColAttrib.GetVisibleColumns(), m_aCustomAttribDefs)
{
	// add extra controls to implement list-view
	for (int nCtrl = 0; nCtrl < NUM_FTDCCTRLS; nCtrl++)
	{
		const TDCCONTROL& ctrl = FTDCCONTROLS[nCtrl];

		AddRCControl(_T("CONTROL"), ctrl.szClass, CString((LPCTSTR)ctrl.nIDCaption), 
					ctrl.dwStyle, ctrl.dwExStyle,
					ctrl.nX, ctrl.nY, ctrl.nCx, ctrl.nCy, ctrl.nID);
	}

	// tab is on by default
	m_aStyles.SetAt(TDCS_SHOWTREELISTBAR, 1);
}

CTabbedToDoCtrl::~CTabbedToDoCtrl()
{
	// cleanup extension views
	int nView = m_aExtViews.GetSize();

	while (nView--)
	{
		IUIExtensionWindow* pExtWnd = m_aExtViews[nView];

		if (pExtWnd)
			pExtWnd->Release();
	}

	m_aExtViews.RemoveAll();
}

BEGIN_MESSAGE_MAP(CTabbedToDoCtrl, CToDoCtrl)
//{{AFX_MSG_MAP(CTabbedToDoCtrl)
	ON_WM_DESTROY()
//	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FTC_TASKLISTLIST, OnListSelChanged)
	ON_NOTIFY(NM_CLICK, IDC_FTC_TASKLISTLIST, OnListClick)
	ON_NOTIFY(NM_RCLICK, IDC_FTC_TABCTRL, OnTabCtrlRClick)

	ON_REGISTERED_MESSAGE(WM_TDCN_COLUMNEDITCLICK, OnColumnEditClick)
	ON_REGISTERED_MESSAGE(WM_TDCN_VIEWPOSTCHANGE, OnPostTabViewChange)
	ON_REGISTERED_MESSAGE(WM_TDCN_VIEWPRECHANGE, OnPreTabViewChange)

	ON_REGISTERED_MESSAGE(WM_IUI_EDITSELECTEDTASKTITLE, OnUIExtEditSelectedTaskTitle)
	ON_REGISTERED_MESSAGE(WM_IUI_MODIFYSELECTEDTASK, OnUIExtModifySelectedTask)
	ON_REGISTERED_MESSAGE(WM_IUI_SELECTTASK, OnUIExtSelectTask)
	ON_REGISTERED_MESSAGE(WM_IUI_SORTCOLUMNCHANGE, OnUIExtSortColumnChange)
	ON_REGISTERED_MESSAGE(WM_IUI_DOHELP, OnUIExtDoHelp)

	ON_REGISTERED_MESSAGE(WM_PCANCELEDIT, OnEditCancel)
	ON_REGISTERED_MESSAGE(WM_TLDT_DROP, OnDropObject)
	ON_REGISTERED_MESSAGE(WM_TDCM_GETTASKREMINDER, OnTDCGetTaskReminder)

	ON_MESSAGE(WM_TDC_RECREATERECURRINGTASK, OnRecreateRecurringTask)

	ON_WM_DRAWITEM()
	ON_WM_ERASEBKGND()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////

void CTabbedToDoCtrl::DoDataExchange(CDataExchange* pDX)
{
	CToDoCtrl::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_FTC_TABCTRL, m_tabViews);
}

BOOL CTabbedToDoCtrl::OnInitDialog()
{
	CToDoCtrl::OnInitDialog();

	// create the list-list before anything else
	CRect rect(0, 0, 0, 0);
	VERIFY(m_taskList.Create(this, rect, IDC_FTC_TASKLISTLIST));
	
	// list initialisation
	m_dtList.Register(&m_taskList.List(), this);
	m_taskList.SetWindowPos(&m_taskTree, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW);
	m_taskList.SetWindowPrompt(CEnString(IDS_TDC_TASKLISTPROMPT));

	// add tree and list as tabbed views
	m_tabViews.AttachView(m_taskTree.GetSafeHwnd(), FTCV_TASKTREE, CEnString(IDS_TASKTREE), GraphicsMisc::LoadIcon(IDI_TASKTREE_STD), NULL);
	m_tabViews.AttachView(m_taskList, FTCV_TASKLIST, CEnString(IDS_LISTVIEW), GraphicsMisc::LoadIcon(IDI_LISTVIEW_STD), NewViewData());

	for (int nExt = 0; nExt < m_mgrUIExt.GetNumUIExtensions(); nExt++)
	{
		AddView(m_mgrUIExt.GetUIExtension(nExt));
	}

	Resize();

	return FALSE;
}

void CTabbedToDoCtrl::OnStylesUpdated()
{
	CToDoCtrl::OnStylesUpdated();

	m_taskList.OnStylesUpdated();
}

void CTabbedToDoCtrl::OnTaskIconsChanged()
{
	CToDoCtrl::OnTaskIconsChanged();
	
	m_taskList.OnImageListChange();
}

LRESULT CTabbedToDoCtrl::OnTDCGetTaskReminder(WPARAM wp, LPARAM lp)
{
	UNREFERENCED_PARAMETER(lp);
	ASSERT(wp && (((HWND)lp == m_taskTree.GetSafeHwnd()) || 
					((HWND)lp == m_taskList.GetSafeHwnd())));
	
	// Base class always expects to get this from the Task Tree
	return CToDoCtrl::OnTDCGetTaskReminder(wp, (LRESULT)m_taskTree.GetSafeHwnd());
}

BOOL CTabbedToDoCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// see if an UI extension wants this
	FTC_VIEW nView = GetView();
	
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
#if _MSC_VER <= 1200
			// Special case: If this came from a plugin and we ARE compiled 
			// with VC6 then MFC will not generate a WM_CONTEXTMENU msg 
			// from a right-button click, so we handle that.
			if ((pMsg->message == WM_RBUTTONUP) && IsExtensionView(pMsg->hwnd))
			{
				CPoint ptScreen(pMsg->lParam);
				::ClientToScreen(pMsg->hwnd, &ptScreen);
				
				SendMessage(WM_CONTEXTMENU, pMsg->wParam, MAKELPARAM(ptScreen.x, ptScreen.y));
				return TRUE;
			}
#endif
			
			// Default processing
			IUIExtensionWindow* pExtWnd = GetExtensionWnd(nView);
			ASSERT(pExtWnd);

			if (pExtWnd->ProcessMessage(pMsg))
				return TRUE;
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
		{
			// prepare theme file
			CUIThemeFile themeExt(theme);

			themeExt.SetToolbarImageFile(pExtWnd->GetTypeID());
			pExtWnd->SetUITheme(&themeExt);
		}
	}
}

BOOL CTabbedToDoCtrl::LoadTasks(const CTaskFile& file)
{
	if (!CToDoCtrl::LoadTasks(file))
		return FALSE;

	m_taskList.SetTasklistFolder(FileMisc::GetFolderFromFilePath(m_sLastSavePath));

	// reload last view
	CPreferences prefs;
	
	if (!IsViewSet())
	{
		LoadPrefs();
	}
	else
	{
		FTC_VIEW nView = GetView();

		// handle extension views
		switch (nView)
		{
		case FTCV_TASKTREE:
		case FTCV_TASKLIST:
			SetExtensionsNeedUpdate(TRUE);
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
				
				VIEWDATA* pData = NULL;
				IUIExtensionWindow* pExtWnd = NULL;
				
				if (!GetExtensionWnd(nView, pExtWnd, pData))
					return FALSE;
				
				UpdateExtensionView(pExtWnd, file, IUI_ALL, pData->mapWantedAttrib);
				RefreshExtensionViewSelection();

				// mark rest of extensions needing update
				SetExtensionsNeedUpdate(TRUE, nView);
			}
			break;
		}
	}

	return TRUE;
}

void CTabbedToDoCtrl::LoadPrefs()
{
	CString sKey = GetPreferencesKey(); // no subkey
	
	if (!sKey.IsEmpty()) // not first time
	{
		CPreferences prefs;
		
		// restore view visibility
		ShowListViewTab(prefs.GetProfileInt(sKey, _T("ListViewVisible"), TRUE));
		
		// remove hidden extensions from list of all extensions
		// this ensures that new extensions always appear first time
		CStringArray aTypeIDs;
		m_mgrUIExt.GetExtensionTypeIDs(aTypeIDs);
		
		int nExt = prefs.GetProfileInt(sKey, _T("HiddenExtensionCount"), -1);
		
		if (nExt >= 0)
		{
			while (nExt--)
			{
				CString sSubKey = Misc::MakeKey(_T("HiddenExt%d"), nExt);
				CString sTypeID = prefs.GetProfileString(sKey, sSubKey);
				
				Misc::RemoveItem(sTypeID, aTypeIDs);
			}
			
			SetVisibleExtensionViews(aTypeIDs);
		}
		
		// Last active view
		FTC_VIEW nCurView = GetView();
		FTC_VIEW nView = (FTC_VIEW)prefs.GetProfileInt(sKey, _T("View"), FTCV_UNSET);
		
		if ((nView != FTCV_UNSET) && (nView != nCurView))
			SetView(nView);
		
		// clear the view so we don't keep restoring it
		prefs.WriteProfileInt(sKey, _T("View"), FTCV_UNSET);
	}
}

void CTabbedToDoCtrl::SavePrefs()
{
	CPreferences prefs;
	CString sKey = GetPreferencesKey(); // no subkey
	
	// save view
	if (!sKey.IsEmpty())
	{
		if (GetView() != FTCV_UNSET)
			prefs.WriteProfileInt(sKey, _T("View"), GetView());
		
		// save listview visibility
		prefs.WriteProfileInt(sKey, _T("ListViewVisible"), IsListViewTabShowing());
		
		// save hidden extensions
		CStringArray aVisTypeIDs, aTypeIDs;
		
		m_mgrUIExt.GetExtensionTypeIDs(aTypeIDs);
		GetVisibleExtensionViews(aVisTypeIDs);
		
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
		int nView = m_aExtViews.GetSize();
		
		if (nView)
		{
			while (nView--)
			{
				IUIExtensionWindow* pExtWnd = m_aExtViews[nView];
				
				if (pExtWnd)
				{
					CString sKey = GetPreferencesKey(GetExtensionPrefsSubKey(pExtWnd));
					pExtWnd->SavePreferences(prefs, sKey);
				}
			}
		}
	}
}

void CTabbedToDoCtrl::OnDestroy() 
{
	SavePrefs();
		
	CToDoCtrl::OnDestroy();
}

void CTabbedToDoCtrl::UpdateVisibleColumns()
{
	CToDoCtrl::UpdateVisibleColumns();

	m_taskList.OnColumnVisibilityChange();
}

IUIExtensionWindow* CTabbedToDoCtrl::GetExtensionWnd(FTC_VIEW nView) const
{
	ASSERT(nView >= FTCV_FIRSTUIEXTENSION && nView <= FTCV_LASTUIEXTENSION);

	if (nView < FTCV_FIRSTUIEXTENSION || nView > FTCV_LASTUIEXTENSION)
		return NULL;

	int nExtension = (nView - FTCV_FIRSTUIEXTENSION);
	ASSERT(nExtension < m_aExtViews.GetSize());

	IUIExtensionWindow* pExtWnd = m_aExtViews[nExtension];
	ASSERT(pExtWnd || (m_tabViews.GetViewHwnd(nView) == NULL));

	return pExtWnd;
}

BOOL CTabbedToDoCtrl::GetExtensionWnd(FTC_VIEW nView, IUIExtensionWindow*& pExtWnd, VIEWDATA*& pData) const
{
	pExtWnd = GetExtensionWnd(nView);

	if (!pExtWnd)
		return FALSE;

	pData = GetViewData(nView);

	if (!pData)
	{
		pExtWnd = NULL;
		return FALSE;
	}

	return TRUE;
}

IUIExtensionWindow* CTabbedToDoCtrl::GetCreateExtensionWnd(FTC_VIEW nView)
{
	// try for existing first
	IUIExtensionWindow* pExtWnd = GetExtensionWnd(nView);

	if (pExtWnd)
		return pExtWnd;

	// sanity checks
	ASSERT(m_tabViews.GetViewHwnd(nView) == NULL);

	VIEWDATA* pData = GetViewData(nView);

	if (!pData)
		return NULL;
	
	// Create the extension window
	int nExtension = (nView - FTCV_FIRSTUIEXTENSION);
	UINT nCtrlID = (IDC_FTC_EXTENSIONWINDOW1 + nExtension);

	try
	{
		pExtWnd = pData->pExtension->CreateExtWindow(nCtrlID, WS_CHILD, 0, 0, 0, 0, GetSafeHwnd());
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
	{
		pExtWnd->Release();
		return NULL;
	}
	
	pExtWnd->SetUITheme(&m_theme);
	pExtWnd->SetReadOnly(HasStyle(TDCS_READONLY) != FALSE);
	
	// update focus first because initializing views can take time
	::SetFocus(hWnd);
	
	// For automation
	::SetWindowText(hWnd, pExtWnd->GetTypeID());

	m_aExtViews[nExtension] = pExtWnd;
	
	// restore state
	CPreferences prefs;
	CString sKey = GetPreferencesKey(GetExtensionPrefsSubKey(pExtWnd));
	
	pExtWnd->LoadPreferences(prefs, sKey, false);
	
	// and update tab control with our new HWND
	m_tabViews.SetViewHwnd((FTC_VIEW)nView, hWnd);
	
	// initialize update state
	pData->bNeedFullTaskUpdate = TRUE;

	// and capabilities
	if (pData->bCanPrepareNewTask == -1)
	{
		CTaskFile task;
		task.NewTask(_T("Test Task"));

		pData->bCanPrepareNewTask = pExtWnd->PrepareNewTask(&task);
	}
	
	GetExtensionViewAttributes(pExtWnd, pData->mapWantedAttrib);

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
	HandleUnsavedComments();

	// notify parent
	GetParent()->SendMessage(WM_TDCN_VIEWPRECHANGE, nOldTab, nNewTab);

	// make sure something is selected
	if (GetSelectedCount() == 0)
	{
		HTREEITEM hti = m_taskTree.GetSelectedItem();
		
		if (!hti)
			hti = m_taskTree.GetChildItem(NULL);
		
		CToDoCtrl::SelectTask(GetTaskID(hti));
	}

	// take a note of what task is currently singly selected
	// so that we can prevent unnecessary calls to UpdateControls
	DWORD dwSelTaskID = GetSingleSelectedTaskID();
	FTC_VIEW nNewView = (FTC_VIEW)nNewTab;
	FTC_VIEW nOldView = (FTC_VIEW)nOldTab;
	
	switch (nNewView)
	{
	case FTCV_TASKTREE:
		// update sort
		if (m_bTreeNeedResort)
		{
			m_bTreeNeedResort = FALSE;
			CToDoCtrl::Resort();
		}

		m_taskTree.EnsureSelectionVisible();
		break;

	case FTCV_TASKLIST:
		{
			// Update tasks
			VIEWDATA* pLVData = GetViewData(FTCV_TASKLIST);

			if (pLVData->bNeedFullTaskUpdate)
			{
				RebuildList(NULL);
			}
			else if (pLVData->bNeedResort)
			{
				pLVData->bNeedResort = FALSE;
				m_taskList.Resort();
			}
			
			ResyncListSelection();
			m_taskList.EnsureSelectionVisible();
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
			VIEWDATA* pData = GetViewData(nNewView);

			if (!pData)
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
				else if (pData->bNeedFullTaskUpdate)
				{
					nProgressMsg = IDS_UPDATINGTABBEDVIEW;
				}

				if (nProgressMsg)
					BeginExtensionProgress(pData, nProgressMsg);
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
			
			if (pData->bNeedFullTaskUpdate)
			{
				// start progress if not already
				// will be cleaned up in OnPostTabViewChange
				if (nProgressMsg == 0)
					BeginExtensionProgress(pData);

				CTaskFile tasks;

				if (GetAllTasksForExtensionViewUpdate(tasks, pData->mapWantedAttrib))
					UpdateExtensionView(pExtWnd, tasks, IUI_ALL, pData->mapWantedAttrib);
				
				pData->bNeedFullTaskUpdate = FALSE;
			}
				
			ResyncExtensionSelection(nNewView);
		}
		break;
	}

	// update controls only if the selection has changed
	if (HasSingleSelectionChanged(dwSelTaskID))
		UpdateControls();

	return 0L; // allow tab change
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
				HTREEITEM hti = GetTreeItem(nItem);
				DWORD dwParentID = m_data.GetTaskParentID(GetTaskID(hti));

				CToDoCtrl::AddTreeItemToTaskFile(hti, tasks, NULL, filter, FALSE, dwParentID);
			}

			return tasks.GetTaskCount();
		}
	}
	
	ASSERT(0);
	return 0;
}

int CTabbedToDoCtrl::GetAllTasksForExtensionViewUpdate(CTaskFile& tasks, const CTDCAttributeMap& mapAttrib) const
{
	TDCGETTASKS filter;
	filter.mapAttribs.CopyAttributes(mapAttrib);

	return GetTasks(tasks, FTCV_TASKTREE, filter);
}

BOOL CTabbedToDoCtrl::AddTreeChildrenToTaskFile(HTREEITEM hti, CTaskFile& file, HTASKITEM hTask, const TDCGETTASKS& filter) const
{
	HTREEITEM htiChild = m_taskTree.GetChildItem(hti);
	int nChildren = 0;

	while (htiChild)
	{
		if (!AddTreeItemToTaskFile(htiChild, file, hTask, filter))
		{
			ASSERT(0);
			return FALSE;
		}

		// next
		htiChild = m_taskTree.GetNextItem(htiChild);
	}

	return TRUE;
}

BOOL CTabbedToDoCtrl::AddTreeItemToTaskFile(HTREEITEM hti, CTaskFile& file, HTASKITEM hParentTask, const TDCGETTASKS& filter) const
{
	DWORD dwTaskID = GetTaskID(hti);

	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;

	if (!m_data.GetTask(dwTaskID, pTDI, pTDS, FALSE))
		return FALSE;

	HTASKITEM hTask = file.NewTask(pTDI->sTitle, hParentTask, dwTaskID);

	if (!hTask)
	{
		ASSERT(0);
		return FALSE;
	}

	// Attributes
	SetTaskAttributes(pTDI, pTDS, file, hTask, filter, FALSE);

	// Subtasks
	AddTreeChildrenToTaskFile(hti, file, hTask, filter);

	return TRUE;
}


void CTabbedToDoCtrl::GetAttributesAffectedByMod(TDC_ATTRIBUTE nAttrib, CTDCAttributeMap& mapAttrib) const
{
	mapAttrib.RemoveAll();

	// Check for attribute dependencies
	switch (nAttrib)
	{
	case TDCA_ALL:
		GetAllExtensionViewsWantedAttributes(mapAttrib);
		break;

	case TDCA_DEPENDENCY:
		mapAttrib.AddAttribute(nAttrib);

		if (HasStyle(TDCS_AUTOADJUSTDEPENDENCYDATES))
		{
			mapAttrib.AddAttribute(TDCA_DUEDATE);
			mapAttrib.AddAttribute(TDCA_STARTDATE);
		}
		break;

	case TDCA_DUEDATE:
		mapAttrib.AddAttribute(nAttrib);

		// If this extension view wants due or start dates and dependents may
		// have changed then we send all tasks with dates
		if (m_taskTree.SelectionHasDependents() && 
			HasStyle(TDCS_AUTOADJUSTDEPENDENCYDATES))
		{
			mapAttrib.AddAttribute(TDCA_STARTDATE);
		}

		if (HasStyle(TDCS_DUEHAVEHIGHESTPRIORITY) && 
			HasStyle(TDCS_USEHIGHESTPRIORITY))
		{
			mapAttrib.AddAttribute(TDCA_PRIORITY);
		}

		if (HasStyle(TDCS_SYNCTIMEESTIMATESANDDATES))
		{
			mapAttrib.AddAttribute(TDCA_STARTDATE);
			mapAttrib.AddAttribute(TDCA_TIMEEST);
		}
		break;

	case TDCA_STARTDATE:
		mapAttrib.AddAttribute(nAttrib);

		if (HasStyle(TDCS_SYNCTIMEESTIMATESANDDATES))
		{
			mapAttrib.AddAttribute(TDCA_DUEDATE);
			mapAttrib.AddAttribute(TDCA_TIMEEST);
		}
		break;

	case TDCA_DONEDATE:
		mapAttrib.AddAttribute(nAttrib);

		if (m_taskTree.SelectionHasDependents() && 
			HasStyle(TDCS_AUTOADJUSTDEPENDENCYDATES))
		{
			mapAttrib.AddAttribute(TDCA_DUEDATE);
			mapAttrib.AddAttribute(TDCA_STARTDATE);
		}
		
		if (HasStyle(TDCS_AVERAGEPERCENTSUBCOMPLETION) && 
			HasStyle(TDCS_INCLUDEDONEINAVERAGECALC))
		{
			mapAttrib.AddAttribute(TDCA_PERCENT);
		}
		
		if (HasStyle(TDCS_INCLUDEDONEINRISKCALC) && 
			HasStyle(TDCS_USEHIGHESTRISK))
		{
			mapAttrib.AddAttribute(TDCA_RISK);
		}
		
		if (HasStyle(TDCS_INCLUDEDONEINPRIORITYCALC) && 
			HasStyle(TDCS_USEHIGHESTPRIORITY))
		{
			mapAttrib.AddAttribute(TDCA_PRIORITY);
		}

		if (!m_sCompletionStatus.IsEmpty())
		{
			mapAttrib.AddAttribute(TDCA_STATUS);
		}
		break;

	case TDCA_CUSTOMATTRIBDEFS:
		mapAttrib.AddAttribute(TDCA_CUSTOMATTRIB);
		break;

	default: // all else
		mapAttrib.AddAttribute(nAttrib);
		break;
	}

	// Finally check for colour change
	if (ModCausesColorChange(mapAttrib))
		mapAttrib.AddAttribute(TDCA_COLOR);
}

int CTabbedToDoCtrl::GetSelectedTasksForExtensionViewUpdate(CTaskFile& tasks, 
															const CTDCAttributeMap& mapAttrib, 
															DWORD dwFlags) const
{
	TDCGETTASKS filter;
	filter.mapAttribs.CopyAttributes(mapAttrib);

	VERIFY(GetSelectedTasks(tasks, FTCV_TASKTREE, filter, dwFlags));

	// Globals
	if (!mapAttrib.IsEmpty())
	{
		AddGlobalsToTaskFile(tasks, mapAttrib);
	}
	else
	{
		TDCAUTOLISTDATA tld;
		
		if (GetAutoListData(tld))
			tasks.SetAutoListData(tld);
	}

	return tasks.GetTaskCount();
}

LRESULT CTabbedToDoCtrl::OnPostTabViewChange(WPARAM nOldView, LPARAM nNewView)
{
	switch (nNewView)
	{
	case FTCV_TASKTREE:
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
		// stop any progress
		GetParent()->SendMessage(WM_TDCM_LENGTHYOPERATION, FALSE);

		// resync selection
		RefreshExtensionViewSelection();
		break;
	}

	// notify parent
	GetParent()->SendMessage(WM_TDCN_VIEWPOSTCHANGE, nOldView, nNewView);

	return 0L;
}

void CTabbedToDoCtrl::UpdateExtensionView(IUIExtensionWindow* pExtWnd, const CTaskFile& tasks, 
										  IUI_UPDATETYPE nType, const CTDCAttributeMap& mapAttrib)
{
	CAutoFlag af(m_bUpdatingExtensions, TRUE);

	CArray<IUI_ATTRIBUTE, IUI_ATTRIBUTE> aAttrib;
	
	if (TDC::MapAttributesToIUIAttrib(mapAttrib, aAttrib))
		pExtWnd->UpdateTasks(&tasks, nType, aAttrib.GetData(), aAttrib.GetSize());
}

void CTabbedToDoCtrl::RefreshExtensionViewSelection()
{
	FTC_VIEW nView = GetView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
	case FTCV_TASKLIST:
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
			IUIExtensionWindow* pExtWnd = GetExtensionWnd(nView);
			ASSERT(pExtWnd && pExtWnd->GetHwnd());

			CDWordArray aTaskIDs;
			int nNumSel = CToDoCtrl::GetSelectedTaskIDs(aTaskIDs);

			if (nNumSel && !pExtWnd->SelectTasks(aTaskIDs.GetData(), nNumSel))
			{
				if (!pExtWnd->SelectTask(aTaskIDs[0]))
				{
					// clear tasklist selection
					TSH().RemoveAll();
					UpdateControls();
				}
			}
		}
		break;

	default:
		ASSERT(0);
	}
}

DWORD CTabbedToDoCtrl::GetSingleSelectedTaskID() const
{
	if (GetSelectedCount() == 1) 
		return GetTaskID(GetSelectedItem());

	// else
	return 0;
}

BOOL CTabbedToDoCtrl::HasSingleSelectionChanged(DWORD dwSelID) const
{
	// multi-selection
	if (GetSelectedCount() != 1)
		return TRUE;

	// different selection
	if (GetTaskID(GetSelectedItem()) != dwSelID)
		return TRUE;

	// dwSelID is still the only selection
	return FALSE;
}

VIEWDATA* CTabbedToDoCtrl::GetViewData(FTC_VIEW nView) const
{
	VIEWDATA* pData = (VIEWDATA*)m_tabViews.GetViewData(nView);

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		ASSERT(pData == NULL);
		break;
		
	case FTCV_TASKLIST:
		ASSERT(pData && !pData->pExtension);
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
		ASSERT(pData && pData->pExtension);
		break;

	// all else
	default:
		ASSERT(0);
	}

	return pData;
}

VIEWDATA* CTabbedToDoCtrl::GetActiveViewData() const
{
	return GetViewData(GetView());
}

void CTabbedToDoCtrl::SetView(FTC_VIEW nView) 
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

void CTabbedToDoCtrl::SetNextView() 
{
	// take a note of what task is currently singly selected
	// so that we can prevent unnecessary calls to UpdateControls
	DWORD dwSelTaskID = GetSingleSelectedTaskID();
	
	m_tabViews.ActivateNextView();

	// update controls only if the selection has changed and 
	if (HasSingleSelectionChanged(dwSelTaskID))
		UpdateControls();
}

LRESULT CTabbedToDoCtrl::OnUIExtSelectTask(WPARAM wParam, LPARAM lParam)
{
	if (!m_bUpdatingExtensions)
	{
		CDWordArray aTaskIDs;

		if (wParam == 0)
		{
			// check there's an actual change
			aTaskIDs.Add(lParam);
		}
		else
		{
			LPDWORD pIDs = (LPDWORD)wParam;

			for (int nID = 0; nID < (int)lParam; nID++)
				aTaskIDs.Add(pIDs[nID]);
		}
		
		if (aTaskIDs.GetSize() == 1)
		{
			DWORD dwTaskID = aTaskIDs[0];

			if (dwTaskID == 0)
			{
				m_taskTree.DeselectAll();
				UpdateControls(TRUE);
			}
			else if (HasSingleSelectionChanged(dwTaskID))
			{
				return SelectTask(dwTaskID);	
			}
		}
		else
		{
			return SelectTasks(aTaskIDs, FALSE);
		}
	}

	// else
	return 0L;
}

LRESULT CTabbedToDoCtrl::OnUIExtSortColumnChange(WPARAM /*wParam*/, LPARAM lParam)
{
	FTC_VIEW nView = GetView();
	
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
			VIEWDATA* pData = GetViewData(nView);
			ASSERT(pData);

			if (pData)
				pData->sort.single.nBy = TDC::MapIUIEditToColumn((IUI_ATTRIBUTE)lParam);
		}
		break;
	}

	return 0L;
}

LRESULT CTabbedToDoCtrl::OnUIExtDoHelp(WPARAM /*wParam*/, LPARAM lParam)
{
	FTC_VIEW nView = GetView();
	
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

LRESULT CTabbedToDoCtrl::OnUIExtEditSelectedTaskTitle(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	BOOL bEdit = EditSelectedTask();
	ASSERT(bEdit);

	return bEdit;
}

BOOL CTabbedToDoCtrl::ProcessUIExtensionMod(const IUITASKMOD& mod)
{
	CStringArray aValues;
	CBinaryData bdEmpty;
	
	switch (mod.nAttrib)
	{
	case IUI_TASKNAME:		return SetSelectedTaskTitle(mod.szValue);
	case IUI_PRIORITY:		return SetSelectedTaskPriority(mod.nValue);
	case IUI_COLOR: 		return SetSelectedTaskColor(mod.crValue);
	case IUI_ALLOCBY:		return SetSelectedTaskAllocBy(mod.szValue);
	case IUI_STATUS:		return SetSelectedTaskStatus(mod.szValue);
	case IUI_PERCENT:		return SetSelectedTaskPercentDone(mod.nValue);
	case IUI_TIMEEST:		return SetSelectedTaskTimeEstimate(mod.dValue, mod.nTimeUnits);
	case IUI_TIMESPENT:		return SetSelectedTaskTimeSpent(mod.dValue, mod.nTimeUnits);
	case IUI_COMMENTS:		return SetSelectedTaskComments(mod.szValue, bdEmpty);
	case IUI_FLAG:			return SetSelectedTaskFlag(mod.bValue);
	case IUI_RISK:			return SetSelectedTaskRisk(mod.nValue);
	case IUI_EXTERNALID: 	return SetSelectedTaskExtID(mod.szValue);
	case IUI_COST:			return SetSelectedTaskCost(mod.dValue);
	case IUI_VERSION:		return SetSelectedTaskVersion(mod.szValue);
	case IUI_CUSTOMATTRIB:	return SetSelectedTaskCustomAttributeData(mod.szCustomAttribID, mod.szValue, FALSE);
	case IUI_DONEDATE:		return SetSelectedTaskDate(TDCD_DONE, CDateHelper::GetDate(mod.tValue));
	case IUI_STARTDATE:		return SetSelectedTaskDate(TDCD_START, CDateHelper::GetDate(mod.tValue));
	case IUI_DUEDATE:		return SetSelectedTaskDate(TDCD_DUE, CDateHelper::GetDate(mod.tValue));
		
	case IUI_ALLOCTO:
		Misc::Split(mod.szValue, aValues);
		return SetSelectedTaskAllocTo(aValues);
		
	case IUI_CATEGORY:
		Misc::Split(mod.szValue, aValues);
		return SetSelectedTaskCategories(aValues);
		
	case IUI_TAGS:
		Misc::Split(mod.szValue, aValues);
		return SetSelectedTaskTags(aValues);
		
	case IUI_FILEREF:
		Misc::Split(mod.szValue, aValues);
		return SetSelectedTaskFileRefs(aValues);
		
	case IUI_DEPENDENCY: 
		Misc::Split(mod.szValue, aValues);
		return SetSelectedTaskDependencies(aValues);

	case IUI_OFFSETTASK:
		if (GetSelectedCount() == 1)
			return MoveSelectedTaskDates(CDateHelper::GetDate(mod.tValue), TRUE);
		break;
		
	// not supported
	case IUI_RECURRENCE: 
	case IUI_CREATIONDATE:
	case IUI_CREATEDBY:
		break;
	}

	// all else
	ASSERT(0);
	return FALSE;
}

LRESULT CTabbedToDoCtrl::OnUIExtModifySelectedTask(WPARAM wParam, LPARAM lParam)
{
	// Prevent possibility of infinite recursion caused by an extension 
	// responding to being updated by posting another change ad infinitum
	AF_NOREENTRANT_RET(FALSE);

	ASSERT(!IsReadOnly());

	if (IsReadOnly())
		return FALSE;

	HandleUnsavedComments();

	BOOL bDependChange = FALSE, bMoveTask = FALSE, bSuccess = TRUE;
	
	CUndoAction ua(m_data, TDCUAT_EDIT);

	try
	{
		const IUITASKMOD* pMods = (const IUITASKMOD*)lParam;
		int nNumMod = (int)wParam;

		ASSERT(nNumMod > 0);

		for (int nMod = 0; ((nMod < nNumMod) && bSuccess); nMod++)
		{
			const IUITASKMOD& mod = pMods[nMod];

			// prevent the active view's change being propagated 
			// back to itself
			m_nExtModifyingAttrib = mod.nAttrib;

			if (!ProcessUIExtensionMod(mod))
			{
				ASSERT(0);
				bSuccess = FALSE;
			}

			m_nExtModifyingAttrib = IUI_NONE;
		}
	}
	catch (...)
	{
		ASSERT(0);
		bSuccess = FALSE;
	}

	if (bSuccess)
	{
		// since we prevented changes being propagated back to the active view
		// we may need to update more than the selected task as a consequence
		// of dependency changes or task moves
		if (bDependChange)
		{
			VIEWDATA* pData = NULL;
			IUIExtensionWindow* pExtWnd = NULL;

			if (!GetExtensionWnd(GetView(), pExtWnd, pData))
				return FALSE;

			// update all tasks
			CWaitCursor cursor;
			CTaskFile tasks;

			if (GetAllTasksForExtensionViewUpdate(tasks, pData->mapWantedAttrib))
				UpdateExtensionView(pExtWnd, tasks, IUI_EDIT, pData->mapWantedAttrib);
		}
		else if (bMoveTask)
		{
			UpdateExtensionViewsSelection(TDCA_DUEDATE);
		}
	}
	
	return bSuccess;
}

void CTabbedToDoCtrl::RebuildCustomAttributeUI()
{
	CToDoCtrl::RebuildCustomAttributeUI();

	m_taskList.OnCustomAttributeChange();
}

void CTabbedToDoCtrl::ReposTaskTree(CDeferWndMove* pDWM, const CRect& rPos)
{
	CRect rView;
	m_tabViews.Resize(rPos, pDWM, rView);

	CToDoCtrl::ReposTaskTree(pDWM, rView);
}

void CTabbedToDoCtrl::UpdateTasklistVisibility()
{
	BOOL bTasksVis = (m_nMaxState != TDCMS_MAXCOMMENTS);
	FTC_VIEW nView = GetView();

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
	// clip out tab ctrl
	if (m_tabViews.GetSafeHwnd())
		ExcludeChild(&m_tabViews, pDC);

	return CToDoCtrl::OnEraseBkgnd(pDC);
}

void CTabbedToDoCtrl::Resize(int cx, int cy, BOOL bSplitting)
{
	CToDoCtrl::Resize(cx, cy, bSplitting);
}

void CTabbedToDoCtrl::SetMaximizeState(TDC_MAXSTATE nState)
{
	TDC_MAXSTATE nPrevState = m_nMaxState;

	CToDoCtrl::SetMaximizeState(nState);

	if ((nState != nPrevState) && (nState != TDCMS_MAXCOMMENTS))
	{
		FTC_VIEW nView = GetView();
		
		switch (nView)
		{
		case FTCV_TASKTREE:
		case FTCV_UNSET:
		case FTCV_TASKLIST:
			// handled above
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
	FTC_VIEW nView = GetView();

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
		return TRUE;

	default:
		ASSERT(0);
	}

	return FALSE;
}

BOOL CTabbedToDoCtrl::GetSelectionBoundingRect(CRect& rSelection) const
{
	rSelection.SetRectEmpty();
	FTC_VIEW nView = GetView();

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

void CTabbedToDoCtrl::SelectAll()
{
	FTC_VIEW nView = GetView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		CToDoCtrl::SelectAll();
		break;

	case FTCV_TASKLIST:
		{
			int nNumItems = m_taskList.GetTaskCount();
			BOOL bAllTasks = (CToDoCtrl::GetTaskCount() == (UINT)nNumItems);

			// select items in tree
			if (bAllTasks)
			{
				CToDoCtrl::SelectAll();
				m_taskList.SelectAll();
			}
			else
			{
				// save IDs only not showing all tasks
				CDWordArray aTaskIDs;

				for (int nItem = 0; nItem < nNumItems; nItem++)
					aTaskIDs.Add(m_taskList.GetTaskID(nItem));

				m_taskList.SelectTasks(aTaskIDs);
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
		break;

	default:
		ASSERT(0);
	}
}

int CTabbedToDoCtrl::GetSelectedTasks(CTaskFile& tasks, const TDCGETTASKS& filter, DWORD dwFlags) const
{
	return GetSelectedTasks(tasks, GetView(), filter, dwFlags);
}

int CTabbedToDoCtrl::GetSelectedTasks(CTaskFile& tasks, FTC_VIEW nView, const TDCGETTASKS& filter, DWORD dwFlags) const
{
	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		return CToDoCtrl::GetSelectedTasks(tasks, filter, dwFlags);

	case FTCV_TASKLIST:
		{
			PrepareTaskfileForTasks(tasks, filter);

			// we return exactly what's selected in the list and in the same order
			POSITION pos = m_taskList.List().GetFirstSelectedItemPosition();

			while (pos)
			{
				int nItem = m_taskList.List().GetNextSelectedItem(pos);

				HTREEITEM hti = GetTreeItem(nItem);
				DWORD dwParentID = m_data.GetTaskParentID(GetTaskID(nItem));
				HTASKITEM htParent = NULL;
				
				// Add immediate parent as required.
				// Note: we can assume that the selected task is always added successfully
				if (dwParentID && (dwFlags & TDCGSTF_IMMEDIATEPARENT))
				{
					HTREEITEM htiParent = m_taskTree.GetParentItem(hti);
					DWORD dwParentsParentID = m_data.GetTaskParentID(dwParentID);
					
					if (CToDoCtrl::AddTreeItemToTaskFile(htiParent, tasks, NULL, filter, FALSE, dwParentsParentID))  // FALSE == no subtasks
						htParent = tasks.FindTask(dwParentID);
				}

				VERIFY(CToDoCtrl::AddTreeItemToTaskFile(hti, tasks, NULL, filter, FALSE, dwParentID)); // FALSE == no subtasks
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
		return CToDoCtrl::GetSelectedTasks(tasks, filter, dwFlags); // for now

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

void CTabbedToDoCtrl::SetMaxInfotipCommentsLength(int nLength)
{
	CToDoCtrl::SetMaxInfotipCommentsLength(nLength);

	m_taskList.SetMaxInfotipCommentsLength(nLength);
}

void CTabbedToDoCtrl::SetEditTitleTaskID(DWORD dwTaskID)
{
	CToDoCtrl::SetEditTitleTaskID(dwTaskID);
	
	m_taskList.SetEditTitleTaskID(dwTaskID);
}

BOOL CTabbedToDoCtrl::SetStyle(TDC_STYLE nStyle, BOOL bOn, BOOL bWantUpdate)
{
	// base class processing
	if (CToDoCtrl::SetStyle(nStyle, bOn, bWantUpdate))
	{
		// post-precessing
		switch (nStyle)
		{
		case TDCS_SHOWTREELISTBAR:
			m_tabViews.ShowTabControl(bOn);

			if (bWantUpdate)
				Resize();
			break;

		// detect preferences that can affect task text colors
		// and then handle this in NotifyEndPreferencesUpdate
		case TDCS_COLORTEXTBYPRIORITY:
		case TDCS_COLORTEXTBYATTRIBUTE:
		case TDCS_COLORTEXTBYNONE:
		case TDCS_TREATSUBCOMPLETEDASDONE:
		case TDCS_USEEARLIESTDUEDATE:
		case TDCS_USELATESTDUEDATE:
		case TDCS_USEEARLIESTSTARTDATE:
		case TDCS_USELATESTSTARTDATE:
		case TDCS_USEHIGHESTPRIORITY:
		case TDCS_INCLUDEDONEINPRIORITYCALC:
		case TDCS_DUEHAVEHIGHESTPRIORITY:
		case TDCS_DONEHAVELOWESTPRIORITY:
		case TDCS_TASKCOLORISBACKGROUND:
			m_bTaskColorChange = TRUE;
			break;

		case TDCS_READONLY:
			{
				int nView = m_aExtViews.GetSize();
				
				while (nView--)
				{
					if (m_aExtViews[nView] != NULL)
						m_aExtViews[nView]->SetReadOnly(bOn != FALSE);
				}
			}
			break;
		}

		// notify list-list to update itself
		m_taskList.OnStyleUpdated(nStyle, bOn, bWantUpdate);

		return TRUE;
	}

	return FALSE;
}

void CTabbedToDoCtrl::SetPriorityColors(const CDWordArray& aColors)
{
	if (m_taskList.SetPriorityColors(aColors))
		m_bTaskColorChange = TRUE;

	CToDoCtrl::SetPriorityColors(aColors);
}

void CTabbedToDoCtrl::SetCompletedTaskColor(COLORREF color)
{
	if (m_taskList.SetCompletedTaskColor(color))
		m_bTaskColorChange = TRUE;

	CToDoCtrl::SetCompletedTaskColor(color);
}

void CTabbedToDoCtrl::SetFlaggedTaskColor(COLORREF color)
{
	if (m_taskList.SetFlaggedTaskColor(color))
		m_bTaskColorChange = TRUE;

	CToDoCtrl::SetFlaggedTaskColor(color);
}

void CTabbedToDoCtrl::SetReferenceTaskColor(COLORREF color)
{
	if (m_taskList.SetReferenceTaskColor(color))
		m_bTaskColorChange = TRUE;

	CToDoCtrl::SetReferenceTaskColor(color);
}

void CTabbedToDoCtrl::SetAttributeColors(TDC_ATTRIBUTE nAttrib, const CTDCColorMap& colors)
{
	if (m_taskList.SetAttributeColors(nAttrib, colors))
		m_bTaskColorChange = TRUE;

	CToDoCtrl::SetAttributeColors(nAttrib, colors);
}

void CTabbedToDoCtrl::SetStartedTaskColors(COLORREF crStarted, COLORREF crStartedToday)
{
	if (m_taskList.SetStartedTaskColors(crStarted, crStartedToday))
		m_bTaskColorChange = TRUE;

	CToDoCtrl::SetStartedTaskColors(crStarted, crStartedToday);
}

void CTabbedToDoCtrl::SetDueTaskColors(COLORREF crDue, COLORREF crDueToday)
{
	if (m_taskList.SetDueTaskColors(crDue, crDueToday))
		m_bTaskColorChange = TRUE;

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

void CTabbedToDoCtrl::NotifyBeginPreferencesUpdate()
{
	// base class
	CToDoCtrl::NotifyBeginPreferencesUpdate();

	// nothing else for us to do
}

void CTabbedToDoCtrl::NotifyEndPreferencesUpdate()
{
	// base class
	CToDoCtrl::NotifyEndPreferencesUpdate();

	// notify extension windows
	if (HasAnyExtensionViews())
	{
		// we need to update in 2 ways:
		// 1. Tell the extensions that application settings have changed
		// 2. Refresh tasks if their text colour may have changed
		CPreferences prefs;
		CString sKey = GetPreferencesKey(_T("UIExtensions"));
		
		int nExt = m_aExtViews.GetSize();
		FTC_VIEW nCurView = GetView();

		while (nExt--)
		{
			FTC_VIEW nExtView = (FTC_VIEW)(FTCV_FIRSTUIEXTENSION + nExt);
			IUIExtensionWindow* pExtWnd = m_aExtViews[nExt];
			
			if (pExtWnd)
			{
				VIEWDATA* pData = GetViewData(nExtView);

				if (!pData)
					continue;

				// if this extension is active and wants a 
				// color update we want to start progress
				BOOL bWantColorUpdate = (m_bTaskColorChange && pExtWnd->WantEditUpdate(IUI_COLOR));

				if (bWantColorUpdate && nExtView == nCurView)
					BeginExtensionProgress(pData);

				// notify all extensions of prefs change
				pExtWnd->LoadPreferences(prefs, sKey, true);

				// Update task colours on the active view if necessary
				if (bWantColorUpdate)
				{
					if (nExtView == nCurView)
					{
						CWaitCursor cursor;

						CTDCAttributeMap mapAttribs;
						mapAttribs.AddAttribute(TDCA_COLOR);
						
						CTaskFile tasks;
						GetAllTasksForExtensionViewUpdate(tasks, mapAttribs);
						
						UpdateExtensionView(pExtWnd, tasks, IUI_EDIT, mapAttribs);
						pData->bNeedFullTaskUpdate = FALSE;
					}
					else // mark for update
					{
						pData->bNeedFullTaskUpdate = TRUE;
					}
				}

				// cleanup progress
				if (bWantColorUpdate && nExtView == nCurView)
					EndExtensionProgress();
			}
		}
	}

	m_bTaskColorChange = FALSE; // always
}

void CTabbedToDoCtrl::BeginExtensionProgress(const VIEWDATA* pData, UINT nMsg)
{
	ASSERT(pData);

	if (nMsg == 0)
		nMsg = IDS_UPDATINGTABBEDVIEW;

	CEnString sMsg(nMsg, pData->pExtension->GetMenuText());
	GetParent()->SendMessage(WM_TDCM_LENGTHYOPERATION, TRUE, (LPARAM)(LPCTSTR)sMsg);
}

void CTabbedToDoCtrl::EndExtensionProgress()
{
	GetParent()->SendMessage(WM_TDCM_LENGTHYOPERATION, FALSE);
}

void CTabbedToDoCtrl::BeginTimeTracking(DWORD dwTaskID, BOOL bNotify)
{
	CToDoCtrl::BeginTimeTracking(dwTaskID, bNotify);

	m_taskList.SetTimeTrackTaskID(m_dwTimeTrackTaskID);
}

void CTabbedToDoCtrl::EndTimeTracking(BOOL bAllowConfirm, BOOL bNotify)
{
	CToDoCtrl::EndTimeTracking(bAllowConfirm, bNotify);
	
	m_taskList.SetTimeTrackTaskID(m_dwTimeTrackTaskID);
}

CString CTabbedToDoCtrl::GetControlDescription(const CWnd* pCtrl) const
{
	FTC_VIEW nView = GetView();

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
		if (pCtrl)
		{
			HWND hwnd = m_tabViews.GetViewHwnd(nView);

			if (CDialogHelper::IsChildOrSame(hwnd, pCtrl->GetSafeHwnd()))
			{
				return m_tabViews.GetViewName(nView);
			}
		}
		break;

	default:
		ASSERT(0);
	}

	return CToDoCtrl::GetControlDescription(pCtrl);
}

BOOL CTabbedToDoCtrl::DeleteSelectedTask(BOOL bWarnUser, BOOL bResetSel)
{
	// Work out what to select after the deletion
	DWORD dwNextSelID = 0;

	FTC_VIEW nView = GetView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		// handled in CToDoCtrl::DeleteSelectedTask
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
			dwNextSelID = GetNextTaskID(GetSelectedTaskID(), TTCNT_NEXT, TRUE); // unselected

			if (!dwNextSelID)
				dwNextSelID = GetNextTaskID(GetSelectedTaskID(), TTCNT_PREV, TRUE); // unselected
		}
		break;

	default:
		ASSERT(0);
	}

	if (CToDoCtrl::DeleteSelectedTask(bWarnUser, bResetSel))
	{
		if (dwNextSelID)
			SelectTask(dwNextSelID, FALSE);

		return TRUE;
	}

	return FALSE;
}

DWORD CTabbedToDoCtrl::GetNextTaskID(DWORD dwTaskID, TTC_NEXTTASK nNext, BOOL bExcludeSelected) const
{
	FTC_VIEW nView = GetView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		ASSERT(0); // For derived use only
		return 0;

	case FTCV_TASKLIST:
		{
			int nSel = m_taskList.FindTaskItem(dwTaskID);
			
			if (nSel == -1)
			{
				ASSERT(0);
				return 0;
			}

			switch (nNext)
			{
			case TTCNT_NEXT:
			case TTCNT_NEXTTOPLEVEL: // Look forwards
				{
					BOOL bTopLevelOnly = (nNext == TTCNT_NEXTTOPLEVEL);
					int nNumItems = m_taskList.GetItemCount();
					
					for (int nItem = (nSel + 1); nItem < nNumItems; nItem++)
					{
						HTREEITEM htiNext = GetTreeItem(nItem);

						if (bTopLevelOnly && m_taskTree.GetParentItem(htiNext))
						{
							continue;
						}
						else if (bExcludeSelected && TSH().IsItemSelected(htiNext, TRUE))
						{
							continue;
						}
						
						// else
						return m_taskList.GetTaskID(nItem);
					}
				}
				break;
				
			case TTCNT_PREV:
			case TTCNT_PREVTOPLEVEL: // look backwards
				{
					BOOL bTopLevelOnly = (nNext == TTCNT_PREVTOPLEVEL);
					int nItem = nSel;
					
					while (nItem--)
					{
						HTREEITEM htiPrev = GetTreeItem(nItem);
						
						if (bTopLevelOnly && m_taskTree.GetParentItem(htiPrev))
						{
							continue;
						}
						else if (bExcludeSelected && TSH().IsItemSelected(htiPrev, TRUE))
						{
							continue;
						}
						
						// else
						return m_taskList.GetTaskID(nItem);
					}
				}
				break;
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
			// Same as for ListView
			IUIExtensionWindow* pExtWnd = GetExtensionWnd(nView);
			ASSERT(pExtWnd);
			
			if (pExtWnd)
			{
				IUI_APPCOMMAND nCmd = MapGetNextToCommand(nNext);
				DWORD dwNextID(dwTaskID);
				
				while (pExtWnd->DoAppCommand(nCmd, (DWORD)&dwNextID))
				{
					if (!dwNextID | (dwNextID == dwTaskID))
						break;
					
					if (bExcludeSelected)
					{
						HTREEITEM htiNext = TCF().GetItem(dwNextID);
						ASSERT(htiNext);
						
						if (TSH().IsItemSelected(htiNext, TRUE))
							continue;
					}
					
					// else
					return dwNextID;
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
	case TTCNT_NEXTTOPLEVEL:	return IUI_GETNEXTTOPLEVELTASK;
	case TTCNT_PREV:			return IUI_GETPREVTASK;
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
	ASSERT(pTDI);

	// give active extension view a chance to initialise
	FTC_VIEW nView = GetView();

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
				HTASKITEM hTask = task.NewTask(pTDI->sTitle);

				task.SetTaskAttributes(hTask, pTDI);

				if (pExtWnd->PrepareNewTask(&task))
					task.GetTaskAttributes(hTask, pTDI);

				// fall thru
			}
		}
		break;
	}

	return pTDI;
}

BOOL CTabbedToDoCtrl::CanCreateNewTask(TDC_INSERTWHERE nInsertWhere) const
{
	FTC_VIEW nView = GetView();

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
		{
			const VIEWDATA* pData = GetViewData(nView);

			if (pData)
				bCanCreate &= pData->bCanPrepareNewTask;
		}
		break;

	default:
		bCanCreate = FALSE;
		ASSERT(0);
		break;
	}

	return bCanCreate;
}

void CTabbedToDoCtrl::RebuildList(const void* pContext)
{
	if (!m_data.GetTaskCount())
	{
		m_taskList.DeleteAll(); 
		return;
	}

	// cache current selection
	TDCSELECTIONCACHE cache;

	if (GetView() == FTCV_TASKLIST)
		CacheListSelection(cache);
	else
		CacheTreeSelection(cache, FALSE);

	// note: the call to RestoreListSelection at the bottom fails if the 
	// list has redraw disabled so it must happen outside the scope of hr2
	{
		CHoldRedraw hr(GetSafeHwnd());
		CHoldRedraw hr2(m_taskList);
		CWaitCursor cursor;

		// remove all existing items
		m_taskList.DeleteAll();
		
		// rebuild the list from the tree
		AddTreeItemToList(NULL, pContext);

		m_taskList.SetNextUniqueTaskID(m_dwNextUniqueID);

		VIEWDATA* pLVData = GetViewData(FTCV_TASKLIST);
		ASSERT(pLVData);

		pLVData->bNeedFullTaskUpdate = FALSE;

		// redo last sort
		if ((GetView() == FTCV_TASKLIST) && IsSorting())
		{
			pLVData->bNeedResort = FALSE;
			m_taskList.Resort();
		}
	}
	
	// restore selection
	RestoreListSelection(cache);
	UpdateTreeSelection();
	
	// don't update controls if only one item is selected and it did not
	// change as a result of the filter
	if (!(GetSelectedCount() == 1 && 
		cache.aSelTaskIDs.GetSize() == 1 &&
		GetTaskID(GetSelectedItem()) == cache.aSelTaskIDs[0]))
	{
		UpdateControls();
	}
}

void CTabbedToDoCtrl::AddTreeItemToList(HTREEITEM hti, const void* pContext)
{
	// add task
	if (hti)
	{
		// if the add fails then it's a task reference
		if (m_taskList.InsertItem(GetTaskID(hti)) == -1)
			return; 
	}

	// children
	HTREEITEM htiChild = m_taskTree.GetChildItem(hti);

	while (htiChild)
	{
		AddTreeItemToList(htiChild, pContext);
		htiChild = m_taskTree.GetNextItem(htiChild);
	}
}

void CTabbedToDoCtrl::SetExtensionsNeedUpdate(BOOL bUpdate, FTC_VIEW nIgnore)
{
	for (int nExt = 0; nExt < m_aExtViews.GetSize(); nExt++)
	{
		FTC_VIEW nView = (FTC_VIEW)(FTCV_UIEXTENSION1 + nExt);
		
		if (nView == nIgnore)
			continue;

		// else
		VIEWDATA* pData = GetViewData(nView);
		
		if (pData)
			pData->bNeedFullTaskUpdate = bUpdate;
	}
}

void CTabbedToDoCtrl::SetModified(BOOL bMod, TDC_ATTRIBUTE nAttrib, DWORD dwModTaskID)
{
	CToDoCtrl::SetModified(bMod, nAttrib, dwModTaskID);

	if (bMod)
	{
		UpdateListView(nAttrib, dwModTaskID);
		UpdateExtensionViews(nAttrib, dwModTaskID);

		// Special case: Make sure the task is selected in the 
		// non-tree view so that the task is visible for label editing
		if ((nAttrib == TDCA_NEWTASK) && (dwModTaskID != 0))
			SelectTask(dwModTaskID);
	}
}

void CTabbedToDoCtrl::UpdateListView(TDC_ATTRIBUTE nAttrib, DWORD dwTaskID)
{
	m_taskList.SetModified(nAttrib); // always

	switch (nAttrib)
	{
	case TDCA_DELETE:
		// Deletion operations are fairly quick so we do those 
		// even if the List View is not active
		if (dwTaskID)
		{
			m_taskList.DeleteItem(dwTaskID);
		}
		else if (m_taskTree.GetItemCount())
		{
			m_taskList.RemoveDeletedItems();
		}
		else
		{
			m_taskList.DeleteAll();
		}
		break;

	case TDCA_ARCHIVE:
		m_taskList.RemoveDeletedItems();
		break;

	case TDCA_NEWTASK:
		// If this is a 'single new task' we add it to the list
		// below the currently selected item
		if (dwTaskID)
		{
			int nSel = m_taskList.GetSelectedItem();

			if (nSel != -1)
				nSel++;

			m_taskList.InsertItem(dwTaskID, nSel);
			break;
		}
		// else fall thru to rebuild list

	case TDCA_UNDO:
	case TDCA_PASTE:
		if (InListView())
		{
			// The tree will have selected the undone items
			// so that's what the list also needs to show
			TDCSELECTIONCACHE cache;
			CacheTreeSelection(cache);

			RebuildList(NULL);

			RestoreListSelection(cache);
		}
		else
		{
			GetViewData(FTCV_TASKLIST)->bNeedFullTaskUpdate = TRUE;
		}
		break;

	default: // all other attributes
		if (InListView())
			m_taskList.InvalidateSelection();
		break;
	}
}

BOOL CTabbedToDoCtrl::ModCausesColorChange(TDC_ATTRIBUTE nModType) const
{
	switch (nModType)
	{
	case TDCA_ALL:
		return TRUE;

	case TDCA_COLOR:
		return !HasStyle(TDCS_COLORTEXTBYPRIORITY) &&
				!HasStyle(TDCS_COLORTEXTBYATTRIBUTE) &&
				!HasStyle(TDCS_COLORTEXTBYNONE);

	case TDCA_CATEGORY:
	case TDCA_ALLOCBY:
	case TDCA_ALLOCTO:
	case TDCA_STATUS:
	case TDCA_VERSION:
	case TDCA_EXTERNALID:
	case TDCA_TAGS:
		return (HasStyle(TDCS_COLORTEXTBYATTRIBUTE) && (m_taskList.GetColorByAttribute() == nModType));

 	case TDCA_DONEDATE:
 		return (m_taskList.GetCompletedTaskColor() != CLR_NONE);

 	case TDCA_DUEDATE:
		{
			COLORREF crDue, crDueToday;
			m_taskList.GetDueTaskColors(crDue, crDueToday);

 			return ((crDue != CLR_NONE) || (crDueToday != CLR_NONE));
		}

	case TDCA_PRIORITY:
		return HasStyle(TDCS_COLORTEXTBYPRIORITY);
	}

	// all else
	return FALSE;
}

BOOL CTabbedToDoCtrl::ModCausesColorChange(const CTDCAttributeMap& mapAttrib) const
{
	POSITION pos = mapAttrib.GetStartPosition();
	
	while (pos)
	{
		TDC_ATTRIBUTE nAttrib = mapAttrib.GetNextAttribute(pos);

		if (ModCausesColorChange(nAttrib))
			return TRUE;
	}

	// else
	return FALSE;
}

int CTabbedToDoCtrl::GetExtensionViewAttributes(IUIExtensionWindow* pExtWnd, CTDCAttributeMap& mapAttrib)
{
	ASSERT(pExtWnd);

	mapAttrib.RemoveAll();
	
	if (pExtWnd) // specific extension
	{
		int nAttrib = TDCA_ATTRIBUTECOUNT;
		
		while (nAttrib--)
		{
			if (pExtWnd->WantEditUpdate(TDC::MapAttributeToIUIAttrib((TDC_ATTRIBUTE)nAttrib)))
				mapAttrib.AddAttribute((TDC_ATTRIBUTE)nAttrib);
		}

		// Include 'position' if extension supports 'unsorted'
		if (pExtWnd->CanDoAppCommand(IUI_SORT, IUI_NONE))
			mapAttrib.AddAttribute(TDCA_POSITION);
	}

	return mapAttrib.GetCount();
}

int CTabbedToDoCtrl::GetAllExtensionViewsWantedAttributes(CTDCAttributeMap& mapAttrib) const
{
	// Build a map of wanted attributes for all extensions
	mapAttrib.RemoveAll();
	int nExt = m_aExtViews.GetSize();
	
	while (nExt--)
	{
		IUIExtensionWindow* pExtWnd = m_aExtViews[nExt];
		
		if (pExtWnd)
		{
			CTDCAttributeMap mapExtAttrib;

			if (GetExtensionViewAttributes(pExtWnd, mapExtAttrib))
				mapAttrib.AppendAttributes(mapExtAttrib);
		}
	}

	return mapAttrib.GetCount();
}

void CTabbedToDoCtrl::UpdateExtensionViews(TDC_ATTRIBUTE nAttrib, DWORD dwTaskID)
{
	if (!HasAnyExtensionViews() || (nAttrib == TDCA_NONE))
		return;

	FTC_VIEW nCurView = GetView();

	switch (nAttrib)
	{
	// for a simple attribute change (or addition) update all extensions
	// at the same time so that they won't need updating when the user switches view
	case TDCA_TASKNAME:
	case TDCA_ALL:
	case TDCA_DONEDATE:
	case TDCA_DUEDATE:
	case TDCA_STARTDATE:
	case TDCA_PRIORITY:
	case TDCA_COLOR:
	case TDCA_ALLOCTO:
	case TDCA_ALLOCBY:
	case TDCA_STATUS:
	case TDCA_CATEGORY:
	case TDCA_TAGS:
	case TDCA_PERCENT:
	case TDCA_TIMEEST:
	case TDCA_TIMESPENT:
	case TDCA_FILEREF:
	case TDCA_COMMENTS:
	case TDCA_FLAG:
	case TDCA_CREATIONDATE:
	case TDCA_CREATEDBY:
	case TDCA_RISK: 
	case TDCA_EXTERNALID: 
	case TDCA_COST: 
	case TDCA_DEPENDENCY: 
	case TDCA_RECURRENCE: 
	case TDCA_VERSION:
	case TDCA_CUSTOMATTRIBDEFS:
		UpdateExtensionViewsSelection(nAttrib);
		break;
		
	case TDCA_NEWTASK: 
		// Special case: if dwTaskID is set then it's a single task
		// so we treat it like an edit
		if (dwTaskID)
		{
			UpdateExtensionViewsSelection(nAttrib);
			break;
		}
		// else fall thru to update all tasks

	// These attributes require an entire refresh of the current view 
	// (if it's an extension) and mark the others as needing updates
	case TDCA_DELETE:
	case TDCA_UNDO:
	case TDCA_POSITION: // == move
	case TDCA_PASTE:
	case TDCA_ARCHIVE:
		{
			int nExt = m_aExtViews.GetSize();
			
			while (nExt--)
			{
				FTC_VIEW nView = (FTC_VIEW)(FTCV_FIRSTUIEXTENSION + nExt);
				VIEWDATA* pData = GetViewData(nView);

				if (pData)
				{
					IUIExtensionWindow* pExtWnd = GetExtensionWnd(nView);

					if (pExtWnd && (nView == nCurView))
					{
						CTaskFile tasks;

						if (GetAllTasksForExtensionViewUpdate(tasks, pData->mapWantedAttrib))
						{
							CWaitCursor cursor;
							BeginExtensionProgress(pData);

							// update all tasks
							IUI_UPDATETYPE nUpdate = IUI_ALL;

							if ((nAttrib == TDCA_DELETE) || (nAttrib == TDCA_ARCHIVE))
								nUpdate = IUI_DELETE;

							UpdateExtensionView(pExtWnd, tasks, nUpdate, pData->mapWantedAttrib);
							pData->bNeedFullTaskUpdate = FALSE;

							if ((nAttrib == TDCA_NEWTASK) && dwTaskID)
								pExtWnd->SelectTask(dwTaskID);
							else
								RefreshExtensionViewSelection();

							EndExtensionProgress();
							ResyncExtensionSelection(nView);
						}
					}
					else
					{
						pData->bNeedFullTaskUpdate = TRUE;
					}
				}
			}
		}
		break;	
		
	case TDCA_PROJNAME:
	case TDCA_ENCRYPT:
		// do nothing
		break;

	default:
		if (CTDCCustomAttributeHelper::IsCustomAttribute(nAttrib))
			UpdateExtensionViewsSelection(nAttrib);
		else
			ASSERT(0);
		break;
	}
}

void CTabbedToDoCtrl::UpdateExtensionViewsSelection(TDC_ATTRIBUTE nAttrib)
{
	ASSERT(HasAnyExtensionViews());

	switch (nAttrib)
	{
	case TDCA_DELETE:
	case TDCA_UNDO:
	case TDCA_POSITION: // == move
	case TDCA_PASTE:
	case TDCA_ARCHIVE:
	case TDCA_PROJNAME:
	case TDCA_ENCRYPT:
		ASSERT(0);
		return;
	}

	// If all extension views require a full task update
	// then we've nothing to do
	if (AllExtensionViewsNeedFullUpdate())
	{
		ASSERT(!IsExtensionView(GetView()));
		return;
	}

	// Work out what tasks we want
	DWORD dwFlags = TDCGSTF_RESOLVEREFERENCES;
	CTDCAttributeMap mapAttrib;
	IUI_UPDATETYPE nUpdate = IUI_EDIT; // default
	
	switch (nAttrib)
	{
	case TDCA_NEWTASK:
		{
			// Always include parent chain but not subtasks
			dwFlags |= (TDCGSTF_ALLPARENTS | TDCGSTF_NOTSUBTASKS);

			// Special update type
			nUpdate = IUI_NEW;

			// Note: We leave mapAttrib empty to retrieve all attributes
		}
		break;

	default:
		{
			// We don't need to proceed if no extension wants 
			// any of the changes
			GetAttributesAffectedByMod(nAttrib, mapAttrib);
		
			if (!AnyExtensionViewWantsChange(mapAttrib))
				return;

			// Include parents if there is a colour change 
			// or a calculated attribute change
			if (mapAttrib.HasAttribute(TDCA_COLOR) || IsCalculatedAttribute(nAttrib))
				dwFlags |= TDCGSTF_ALLPARENTS;

			// DONT include subtasks UNLESS the completion date has changed
			if (nAttrib != TDCA_DONEDATE)
				dwFlags |= TDCGSTF_NOTSUBTASKS;
		}
	}

	// Get the actual tasks for the update
	FTC_VIEW nCurView = GetView();
	CTaskFile tasks;

	switch (nAttrib)
	{
	case TDCA_DEPENDENCY:
	case TDCA_DUEDATE:
	case TDCA_DONEDATE:
	case TDCA_STARTDATE:
		// For the moment 
		if (m_taskTree.SelectionHasDependents())
		{
			GetAllTasksForExtensionViewUpdate(tasks, mapAttrib);
			break;
		}
		// else fall thru

	default:
		GetSelectedTasksForExtensionViewUpdate(tasks, mapAttrib, dwFlags);
		break;
	}
	
	// refresh all extensions 
	int nExt = m_aExtViews.GetSize();
	
	while (nExt--)
	{
		FTC_VIEW nExtView = (FTC_VIEW)(FTCV_FIRSTUIEXTENSION + nExt);
		VIEWDATA* pData = GetViewData(nExtView);
		ASSERT(pData);
		
		IUIExtensionWindow* pExtWnd = m_aExtViews[nExt];
		
		if (pExtWnd)
		{
			if (pData)
			{
				if (nUpdate == IUI_NEW)
					mapAttrib.CopyAttributes(pData->mapWantedAttrib);
				
				pData->bNeedFullTaskUpdate = FALSE;
			}
			
			UpdateExtensionView(pExtWnd, tasks, nUpdate, mapAttrib);
		}
		else if (pData)
		{
			pData->bNeedFullTaskUpdate = TRUE;
		}
	}
}

BOOL CTabbedToDoCtrl::IsCalculatedAttribute(TDC_ATTRIBUTE nAttrib) const
{
	switch (nAttrib)
	{
	case TDCA_TASKNAME:
	case TDCA_COLOR:
	case TDCA_ALLOCTO:
	case TDCA_ALLOCBY:
	case TDCA_STATUS:
	case TDCA_CATEGORY:
	case TDCA_TAGS:
	case TDCA_FILEREF:
	case TDCA_COMMENTS:
	case TDCA_FLAG:
	case TDCA_CREATIONDATE:
	case TDCA_CREATEDBY:
	case TDCA_EXTERNALID: 
	case TDCA_DEPENDENCY: 
	case TDCA_RECURRENCE: 
	case TDCA_VERSION:
		return FALSE;

	case TDCA_DONEDATE:
		return ((HasStyle(TDCS_AVERAGEPERCENTSUBCOMPLETION) && HasStyle(TDCS_INCLUDEDONEINAVERAGECALC)) ||
				(HasStyle(TDCS_INCLUDEDONEINRISKCALC) && HasStyle(TDCS_USEHIGHESTRISK)) ||
				(HasStyle(TDCS_INCLUDEDONEINPRIORITYCALC) && HasStyle(TDCS_USEHIGHESTPRIORITY)));

	case TDCA_COST: 
	case TDCA_TIMESPENT:
	case TDCA_TIMEEST:
		return TRUE;
		
	case TDCA_RISK: 
		return HasStyle(TDCS_USEHIGHESTRISK);

	case TDCA_PRIORITY:
		return HasStyle(TDCS_USEHIGHESTPRIORITY);

	case TDCA_PERCENT:
		return HasStyle(TDCS_AVERAGEPERCENTSUBCOMPLETION);

	case TDCA_DUEDATE:
		return (HasStyle(TDCS_USEEARLIESTDUEDATE) || 
				HasStyle(TDCS_USELATESTDUEDATE) || 
				(HasStyle(TDCS_DUEHAVEHIGHESTPRIORITY) && HasStyle(TDCS_USEHIGHESTPRIORITY)));

	case TDCA_STARTDATE:
		return (HasStyle(TDCS_USEEARLIESTSTARTDATE) || HasStyle(TDCS_USELATESTSTARTDATE));

	case TDCA_ALL:
	case TDCA_NEWTASK: 
	case TDCA_DELETE:
	case TDCA_UNDO:
	case TDCA_POSITION: // == move
	case TDCA_PASTE:
	case TDCA_ARCHIVE:
	case TDCA_CUSTOMATTRIBDEFS:
	case TDCA_PROJNAME:
	case TDCA_ENCRYPT:
		return FALSE;

	default:
		{
			TDCCUSTOMATTRIBUTEDEFINITION attribDef;

			if (CTDCCustomAttributeHelper::GetAttributeDef(nAttrib, m_aCustomAttribDefs, attribDef))
			{
				return attribDef.SupportsCalculation();
			}
		}
		break;
	}

	// all else
	ASSERT(0);
	return FALSE;
}

void CTabbedToDoCtrl::AddGlobalsToTaskFile(CTaskFile& tasks, const CTDCAttributeMap& mapAttrib) const
{
	TDCAUTOLISTDATA tld;
	POSITION pos = mapAttrib.GetStartPosition();
	
	while (pos)
	{
		TDC_ATTRIBUTE nAttrib = mapAttrib.GetNextAttribute(pos);
		GetGlobals(nAttrib, tld);
	}
	
	if (tld.GetSize())
		tasks.SetAutoListData(tld);
}

int CTabbedToDoCtrl::GetGlobals(TDC_ATTRIBUTE nAttrib, TDCAUTOLISTDATA& tld) const
{
	switch (nAttrib)
	{
	case TDCA_ALLOCTO:	return m_cbAllocTo.GetItems(tld.aAllocTo);
	case TDCA_ALLOCBY:	return m_cbAllocBy.GetItems(tld.aAllocBy);
	case TDCA_CATEGORY:	return m_cbCategory.GetItems(tld.aCategory);
	case TDCA_STATUS:	return m_cbStatus.GetItems(tld.aStatus);
	case TDCA_TAGS:		return m_cbTags.GetItems(tld.aTags);
	case TDCA_VERSION:	return m_cbVersion.GetItems(tld.aVersion);
	}

	return 0;
}

BOOL CTabbedToDoCtrl::ExtensionViewWantsChange(int nExt, const CTDCAttributeMap& mapAttrib) const
{
	ASSERT (!mapAttrib.IsEmpty());

	POSITION pos = mapAttrib.GetStartPosition();
	
	while (pos)
	{
		TDC_ATTRIBUTE nAttrib = mapAttrib.GetNextAttribute(pos);

		if (ExtensionViewWantsChange(nExt, nAttrib))
			return TRUE;
	}

	// else
	return FALSE;
}

BOOL CTabbedToDoCtrl::ExtensionViewWantsChange(int nExt, TDC_ATTRIBUTE nAttrib) const
{
	FTC_VIEW nCurView = GetView();
	FTC_VIEW nExtView = (FTC_VIEW)(FTCV_FIRSTUIEXTENSION + nExt);

	// if the window is not active and is already marked
	// for a full update then we don't need to do
	// anything more because it will get this update when
	// it is next activated
	if (nExtView != nCurView)
	{
		const VIEWDATA* pData = GetViewData(nExtView);

		if (!pData || pData->bNeedFullTaskUpdate)
			return FALSE;
	}
	else // active view
	{
		// if this update has come about as a consequence
		// of this extension window modifying the specified
		// attribute, then we assume that it won't want the update
		if (TDC::MapAttributeToIUIAttrib(nAttrib) == m_nExtModifyingAttrib)
			return FALSE;
	}

	// if it's 'all' attributes then assume the answer is always 'yes'
	if (nAttrib == TDCA_ALL)
		return TRUE;
	
	IUIExtensionWindow* pExtWnd = m_aExtViews[nExt];
	ASSERT(pExtWnd);
	
	return (pExtWnd && pExtWnd->WantEditUpdate(TDC::MapAttributeToIUIAttrib(nAttrib)));
}

BOOL CTabbedToDoCtrl::AllExtensionViewsNeedFullUpdate() const
{
	// find the first extension not needing a full task update
	int nExt = m_aExtViews.GetSize();
	
	while (nExt--)
	{
		FTC_VIEW nExtView = (FTC_VIEW)(FTCV_FIRSTUIEXTENSION + nExt);
		const VIEWDATA* pData = GetViewData(nExtView);

		if (pData && !pData->bNeedFullTaskUpdate)
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

BOOL CTabbedToDoCtrl::AnyExtensionViewWantsChange(const CTDCAttributeMap& mapAttrib) const
{
	ASSERT (!mapAttrib.IsEmpty());

	// Else check one by one
	POSITION pos = mapAttrib.GetStartPosition();
	
	while (pos)
	{
		TDC_ATTRIBUTE nAttrib = mapAttrib.GetNextAttribute(pos);
		
		if (AnyExtensionViewWantsChange(nAttrib))
			return TRUE;
	}
	
	// else
	return FALSE;
}

void CTabbedToDoCtrl::ResortSelectedTaskParents()
{
	FTC_VIEW nView = GetView();
	
	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		CToDoCtrl::ResortSelectedTaskParents();
		break;
		
	case FTCV_TASKLIST:
		m_taskList.Resort(); // resort all
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
		// resorting handled by the extension itself
		break;
		
	default:
		ASSERT(0);
	}
}

BOOL CTabbedToDoCtrl::ModNeedsResort(TDC_ATTRIBUTE nModType) const
{
	if (!HasStyle(TDCS_RESORTONMODIFY))
		return FALSE;

	VIEWDATA* pLVData = GetViewData(FTCV_TASKLIST);

	BOOL bTreeNeedsResort = CToDoCtrl::ModNeedsResort(nModType);
	BOOL bListNeedsResort = m_taskList.ModNeedsResort(nModType);
	
	FTC_VIEW nView = GetView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		pLVData->bNeedResort |= bListNeedsResort;
		return bTreeNeedsResort;

	case FTCV_TASKLIST:
		m_bTreeNeedResort |= bTreeNeedsResort;
		return bListNeedsResort;

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
		// resorting handled by the extension itself
		m_bTreeNeedResort |= bTreeNeedsResort;
		pLVData->bNeedResort |= bListNeedsResort;
		break;

	default:
		ASSERT(0);
	}

	return FALSE;
}

const TDSORT& CTabbedToDoCtrl::GetSort() const 
{ 
	FTC_VIEW nView = GetView();
	
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
			VIEWDATA* pVData = GetActiveViewData();
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

BOOL CTabbedToDoCtrl::SelectTask(DWORD dwTaskID, BOOL bTrue)
{
	BOOL bRes = CToDoCtrl::SelectTask(dwTaskID, bTrue);

	// check task has not been filtered out
	FTC_VIEW nView = GetView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		// handled above
		break;

	case FTCV_TASKLIST:
		{
			CDWordArray aTaskIDs;
			aTaskIDs.Add(GetSelectedTaskID());

			m_taskList.SelectTasks(aTaskIDs, TRUE);
			m_taskList.EnsureSelectionVisible();
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
				pExtWnd->SelectTask(dwTaskID);
		}
		break;

	default:
		ASSERT(0);
	}

	return bRes;
}

int CTabbedToDoCtrl::CacheListSelection(TDCSELECTIONCACHE& cache, BOOL bIncBreadcrumbs) const
{
	return m_taskList.CacheSelection(cache, bIncBreadcrumbs);
}

void CTabbedToDoCtrl::RestoreListSelection(const TDCSELECTIONCACHE& cache)
{
	m_taskList.RestoreSelection(cache);
}

BOOL CTabbedToDoCtrl::SetTreeFont(HFONT hFont)
{
	if (CToDoCtrl::SetTreeFont(hFont))
	{
		if (!hFont) // set to our font
		{
			// for some reason i can not yet explain, our font
			// is not correctly set so we use our parent's font instead
			// hFont = (HFONT)SendMessage(WM_GETFONT);
			hFont = (HFONT)GetParent()->SendMessage(WM_GETFONT);
		}

		VERIFY(m_taskList.SetFont(hFont));

		// other views
		// TODO

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

	VIEWDATA* pData = NewViewData();
	ASSERT(pData);

	pData->pExtension = pExtension;

	// we pass NULL for the hWnd because we are going to load
	// only on demand
	if (m_tabViews.AttachView(NULL, nView, sName, hIcon, pData))
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
				pExtWnd->Release();

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
		CStringArray aTypeIDs;
		GetVisibleExtensionViews(aTypeIDs);

		CTDCUIExtensionHelper::PrepareViewVisibilityMenu(pPopup, m_mgrUIExt, aTypeIDs);

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
			if (CTDCUIExtensionHelper::ProcessViewVisibilityMenuCmd(nCmdID, m_mgrUIExt, aTypeIDs))
				SetVisibleExtensionViews(aTypeIDs);
			break;
		}
	}
	
	*pResult = 0;
}


LRESULT CTabbedToDoCtrl::OnColumnEditClick(WPARAM wParam, LPARAM lParam)
{
	if (InListView())
		UpdateTreeSelection();

	return CToDoCtrl::OnColumnEditClick(wParam, lParam);
}

void CTabbedToDoCtrl::OnListSelChanged(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	*pResult = 0;

	UpdateTreeSelection();
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
	FTC_VIEW nView = GetView();

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

TDC_COLUMN CTabbedToDoCtrl::ColumnHitTest(const CPoint& ptScreen) const
{
	return CToDoCtrl::ColumnHitTest(ptScreen);
}

void CTabbedToDoCtrl::Resort(BOOL bAllowToggle)
{
	FTC_VIEW nView = GetView();
	
	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		CToDoCtrl::Resort(bAllowToggle);
		break;
		
	case FTCV_TASKLIST:
		m_taskList.Resort(bAllowToggle);
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
			VIEWDATA* pData = GetViewData(nView);
			ASSERT(pData);

			if (pData)
				Sort(pData->sort.single.nBy, bAllowToggle);
		}
		break;
		
	default:
		ASSERT(0);
	}
}

BOOL CTabbedToDoCtrl::IsSortingBy(TDC_COLUMN nBy) const
{
	FTC_VIEW nView = GetView();
	
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
	ASSERT (sort.IsSorting());

	if (!sort.IsSorting())
		return;

	FTC_VIEW nView = GetView();

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
		break;

	default:
		ASSERT(0);
	}
}

BOOL CTabbedToDoCtrl::CanMultiSort() const
{
	FTC_VIEW nView = GetView();
	
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
		return FALSE;
	}
	
	// all else
	ASSERT(0);
	return FALSE;
}

void CTabbedToDoCtrl::Sort(TDC_COLUMN nBy, BOOL bAllowToggle)
{
	FTC_VIEW nView = GetView();

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
			IUI_ATTRIBUTE nCol = TDC::MapColumnToIUIEdit(nBy);
			
			if ((nCol != IUI_NONE) || (nBy == TDCC_NONE))
			{
				ExtensionDoAppCommand(nView, (bAllowToggle ? IUI_TOGGLABLESORT : IUI_SORT), nCol);

				VIEWDATA* pData = GetViewData(nView);
				ASSERT(pData);
			
				if (pData)
					pData->sort.single.nBy = nBy;
			}
		}
		break;

	default:
		ASSERT(0);
	}
}

BOOL CTabbedToDoCtrl::CanSortBy(TDC_COLUMN nBy) const
{
	FTC_VIEW nView = GetView();
	
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
			IUI_ATTRIBUTE nCol = TDC::MapColumnToIUIEdit(nBy);

			if ((nCol != IUI_NONE) || (nBy == TDCC_NONE))
				return ExtensionCanDoAppCommand(nView, IUI_SORT, nCol);
			
			// else
			return FALSE;
		}
		break;
	}
	
	// else
	ASSERT(0);
	return FALSE;
}

BOOL CTabbedToDoCtrl::MoveSelectedTask(TDC_MOVETASK nDirection) 
{ 
	return !InTreeView() ? FALSE : CToDoCtrl::MoveSelectedTask(nDirection); 
}

BOOL CTabbedToDoCtrl::CanMoveSelectedTask(TDC_MOVETASK nDirection) const 
{ 
	return !InTreeView() ? FALSE : CToDoCtrl::CanMoveSelectedTask(nDirection); 
}

BOOL CTabbedToDoCtrl::GotoNextTask(TDC_GOTO nDirection)
{
	if (!CanGotoNextTask(nDirection))
	{
		return FALSE;
	}

	FTC_VIEW nView = GetView();

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
				return SelectTask(dwNextID);
		}
		break;

	default:
		ASSERT(0);
	}
	
	// else
	return FALSE;
}

CTabbedToDoCtrl::TTC_NEXTTASK CTabbedToDoCtrl::MapGotoToGetNext(TDC_GOTO nDirection, BOOL bTopLevel)
{
	switch (nDirection)
	{
	case TDCG_NEXT:
		return (bTopLevel ? TTCNT_NEXTTOPLEVEL : TTCNT_NEXT);

	case TDCG_PREV:
		return (bTopLevel ? TTCNT_PREVTOPLEVEL : TTCNT_PREV);
	}

	ASSERT(0);
	return (TTC_NEXTTASK)-1;
}

BOOL CTabbedToDoCtrl::CanGotoNextTask(TDC_GOTO nDirection) const
{
	FTC_VIEW nView = GetView();

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

	FTC_VIEW nView = GetView();

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
				return SelectTask(dwNextID);
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
	FTC_VIEW nView = GetView();

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
	FTC_VIEW nView = GetView();

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
	FTC_VIEW nView = GetView();

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

void CTabbedToDoCtrl::ExtensionDoAppCommand(FTC_VIEW nView, IUI_APPCOMMAND nCmd, DWORD dwExtra)
{
	IUIExtensionWindow* pExt = GetExtensionWnd(nView);
	ASSERT(pExt);

	if (pExt)
		pExt->DoAppCommand(nCmd, dwExtra);
}

BOOL CTabbedToDoCtrl::ExtensionCanDoAppCommand(FTC_VIEW nView, IUI_APPCOMMAND nCmd, DWORD dwExtra) const
{
	const IUIExtensionWindow* pExt = GetExtensionWnd(nView);
	ASSERT(pExt);

	if (pExt)
		return pExt->CanDoAppCommand(nCmd, dwExtra);

	return FALSE;
}

void CTabbedToDoCtrl::SetFocusToTasks()
{
	FTC_VIEW nView = GetView();

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
			
		// ensure the selected list item is visible
		if (!m_taskList.EnsureSelectionVisible())
			m_taskList.SelectItem(0);
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
	FTC_VIEW nView = GetView();

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
		return ExtensionCanDoAppCommand(nView, IUI_SETFOCUS);

	default:
		ASSERT(0);
	}
	
	return FALSE;
}

int CTabbedToDoCtrl::FindTasks(const SEARCHPARAMS& params, CResultArray& aResults) const
{
	FTC_VIEW nView = GetView();

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
				SEARCHRESULT result;

				if (m_data.TaskMatches(dwTaskID, params, result))
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
		break;

	default:
		ASSERT(0);
	}

	return aResults.GetSize();
}


BOOL CTabbedToDoCtrl::SelectTask(CString sPart, TDC_SELECTTASK nSelect)
{
	int nFind = -1;
	FTC_VIEW nView = GetView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		return CToDoCtrl::SelectTask(sPart, nSelect);

	case FTCV_TASKLIST:
		switch (nSelect)
		{
		case TDC_SELECTFIRST:
			nFind = FindListTask(sPart);
			break;
			
		case TDC_SELECTNEXT:
			nFind = FindListTask(sPart, m_taskList.GetSelectedItem() + 1);
			break;
			
		case TDC_SELECTNEXTINCLCURRENT:
			nFind = FindListTask(sPart, m_taskList.GetSelectedItem());
			break;
			
		case TDC_SELECTPREV:
			nFind = FindListTask(sPart, m_taskList.GetSelectedItem() - 1, FALSE);
			break;
			
		case TDC_SELECTLAST:
			nFind = FindListTask(sPart, m_taskList.GetItemCount() - 1, FALSE);
			break;
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
		break;

	default:
		ASSERT(0);
	}

	// else
	if (nFind != -1)
		return SelectTask(GetTaskID(nFind));

	return FALSE;
}

int CTabbedToDoCtrl::FindListTask(const CString& sPart, int nStart, BOOL bNext)
{
	// build a search query
	SEARCHPARAMS params;
	params.aRules.Add(SEARCHPARAM(TDCA_TASKNAMEORCOMMENTS, FOP_INCLUDES, sPart));

	// we need to do this manually because CListCtrl::FindItem 
	// only looks at the start of the string
	SEARCHRESULT result;

	int nFrom = nStart;
	int nTo = bNext ? m_taskList.GetItemCount() : -1;
	int nInc = bNext ? 1 : -1;

	for (int nItem = nFrom; nItem != nTo; nItem += nInc)
	{
		DWORD dwTaskID = GetTaskID(nItem);

		if (m_data.TaskMatches(dwTaskID, params, result))
			return nItem;
	}

	return -1; // no match
}

BOOL CTabbedToDoCtrl::SelectTasks(const CDWordArray& aTasks)
{
	return SelectTasks(aTasks, FALSE);
}

BOOL CTabbedToDoCtrl::SelectTasks(const CDWordArray& aTasks, BOOL bTrue)
{
	BOOL bRes = CToDoCtrl::SelectTasks(aTasks, bTrue);

	// extra processing
	if (bRes)
	{
		// If this call came from CToDoCtrl::OnRecreateRecurringTask()
		// we save off the task IDs and eat it to prevent a plugin
		// from rejecting the selection because it has not yet seen
		// the new recurring tasks
		if (m_bRecreatingRecurringTasks)
		{
			m_aRecreatedRecurringTasks.Copy(aTasks);
			return TRUE;
		}

		// else
		ASSERT((m_aRecreatedRecurringTasks.GetSize() == 0) ||
				(&m_aRecreatedRecurringTasks == &aTasks));

		FTC_VIEW nView = GetView();

		switch (nView)
		{
		case FTCV_TASKTREE:
		case FTCV_UNSET:
			break;

		case FTCV_TASKLIST:
			ResyncListSelection();
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
			ResyncExtensionSelection(nView);
			break;

		default:
			ASSERT(0);
		}
	}

	return bRes;
}

void CTabbedToDoCtrl::ResyncListSelection()
{
	// optimisation when all items selected
	if (TSH().GetCount() == m_taskTree.GetItemCount())
	{
		m_taskList.SelectAll();
	}
	else
	{
		// save current states
		TDCSELECTIONCACHE cacheList, cacheTree;

		CacheListSelection(cacheList);

		// We only need the tree breadcrumbs is the list doesn't have any
		CacheTreeSelection(cacheTree, !cacheList.aBreadcrumbs.GetSize());

		if (!cacheList.SelectionMatches(cacheTree))
		{
			// now update the list selection using the tree's selection
			// but the list's breadcrumbs (if it has any), and save list 
			// scroll pos before restoring
			cacheTree.dwFirstVisibleTaskID = GetTaskID(m_taskList.List().GetTopIndex());

			if (cacheList.aBreadcrumbs.GetSize())
				cacheTree.aBreadcrumbs.Copy(cacheList.aBreadcrumbs);

			RestoreListSelection(cacheTree);
			
			// now check that the tree is correctly synced with us!
			CacheListSelection(cacheList);

			if (!cacheList.SelectionMatches(cacheTree))
				RestoreTreeSelection(cacheList);
		}
	}

	m_taskList.UpdateSelectedTaskPath();
}

void CTabbedToDoCtrl::ResyncExtensionSelection(FTC_VIEW nView)
{
	if (!IsExtensionView(nView))
	{
		ASSERT(0);
		return;
	}

	IUIExtensionWindow* pExt = GetExtensionWnd(nView);
	ASSERT(pExt);

	if (pExt)
	{
		// Get tree selection
		TDCSELECTIONCACHE cache;

		// If nothing is selected, work backwards until we find something
		BOOL bNeedUpdate = FALSE;

		while (!CacheTreeSelection(cache))
		{
			if (!m_taskTree.SelectTasksInHistory(FALSE))
				break;

			bNeedUpdate = TRUE; // selection has changed
		}

		if (!cache.IsEmpty())
		{
			// If nothing can be selected, work backwards as before
			while (!SelectExtensionTasks(pExt, cache.aSelTaskIDs, cache.dwFocusedTaskID))
			{
				cache.Clear();

				if (!m_taskTree.SelectTasksInHistory(FALSE))
					break;

				CacheTreeSelection(cache);
				bNeedUpdate = TRUE; // selection has changed
			}
		}
		
		// fallback
		if (cache.IsEmpty())
		{
			pExt->SelectTask(0);

			m_taskTree.DeselectAll();
			UpdateControls();
		}
		else if (bNeedUpdate)
		{
			UpdateControls();
		}
	}
}

BOOL CTabbedToDoCtrl::SelectExtensionTasks(IUIExtensionWindow* pExtWnd, const CDWordArray& aTaskIDs, DWORD dwFocusedTaskID)
{
	ASSERT(pExtWnd);

	if (pExtWnd == NULL)
		return FALSE;

	if ((aTaskIDs.GetSize() == 0) || (dwFocusedTaskID == 0))
		return FALSE;

	if (pExtWnd->SelectTasks(aTaskIDs.GetData(), aTaskIDs.GetSize()))
		return TRUE;

	// Try single task
	if (dwFocusedTaskID && pExtWnd->SelectTask(dwFocusedTaskID))
	{
		UpdateControls();
		return TRUE;
	}

	return FALSE;
}

void CTabbedToDoCtrl::SelectTasksInHistory(BOOL bForward)
{
	if (CanSelectTasksInHistory(bForward))
	{
		// let CToDoCtrl do it's thing
		CToDoCtrl::SelectTasksInHistory(bForward);

		// extra processing
		FTC_VIEW nView = GetView();

		switch (nView)
		{
		case FTCV_TASKTREE:
		case FTCV_UNSET:
			// handled above
			break;

		case FTCV_TASKLIST:
			// then update our own selection
			ResyncListSelection();
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
			ResyncExtensionSelection(nView);
			break;

		default:
			ASSERT(0);
		}
	}
}

void CTabbedToDoCtrl::InvalidateItem(HTREEITEM hti, BOOL bUpdate)
{
	FTC_VIEW nView = GetView();

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

void CTabbedToDoCtrl::UpdateTreeSelection()
{
	// update the tree selection as required
	TDCSELECTIONCACHE cacheTree, cacheList;

	CacheTreeSelection(cacheTree, FALSE);
	CacheListSelection(cacheList, FALSE);
	
	if (!cacheTree.SelectionMatches(cacheList, TRUE))
	{
		if (cacheList.aSelTaskIDs.GetSize() == 0)
		{
			m_taskTree.DeselectAll();
		}
		else if (cacheList.aSelTaskIDs.GetSize() == m_taskTree.GetItemCount())
		{
			m_taskTree.SelectAll();
		}
		else
		{
			m_taskTree.RestoreSelection(cacheList);
		}

		UpdateControls();
	}
}

BOOL CTabbedToDoCtrl::IsItemSelected(int nItem) const
{
	HTREEITEM hti = GetTreeItem(nItem);
	return hti ? TSH().HasItem(hti) : FALSE;
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

LRESULT CTabbedToDoCtrl::OnDropObject(WPARAM wParam, LPARAM lParam)
{
	if (IsReadOnly())
		return 0L;

	TLDT_DATA* pData = (TLDT_DATA*)wParam;
	CWnd* pTarget = (CWnd*)lParam;
	FTC_VIEW nView = GetView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		CToDoCtrl::OnDropObject(wParam, lParam); // default handling
		break;


	case FTCV_TASKLIST:
		// simply convert the list item into the corresponding tree
		// item and pass to base class
		if (pTarget == &m_taskList.List())
		{
			ASSERT (InListView());

 			if (pData->nItem != -1)
 				m_taskList.SelectItem(pData->nItem);

			pData->hti = GetTreeItem(pData->nItem);
			pData->nItem = -1;

			CToDoCtrl::OnDropObject(wParam, (LPARAM)&m_taskTree.Tree()); // default handling
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
	FTC_VIEW nView = GetView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		return CToDoCtrl::GetLabelEditRect(rScreen);

	case FTCV_TASKLIST:
		if (m_taskList.EnsureSelectionVisible())
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
	CToDoCtrl::UpdateSelectedTaskPath();

	// extra processing
	FTC_VIEW nView = GetView();

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
		m_taskList.RecalcColumnWidths();
		
		if (m_taskList.IsSorting())
		{
			VIEWDATA* pData = GetViewData(FTCV_TASKLIST);
			ASSERT(pData);
			
			pData->bNeedResort = TRUE;
		}
	}

	// base class
	return CToDoCtrl::LoadTasksState(prefs, bRebuildingTree);
}

BOOL CTabbedToDoCtrl::IsViewSet() const 
{ 
	return (GetView() != FTCV_UNSET); 
}

BOOL CTabbedToDoCtrl::InListView() const 
{ 
	return (GetView() == FTCV_TASKLIST); 
}

BOOL CTabbedToDoCtrl::InTreeView() const 
{ 
	return ((GetView() == FTCV_TASKTREE) || !IsViewSet()); 
}

BOOL CTabbedToDoCtrl::InExtensionView() const
{
	return IsExtensionView(GetView());
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

		if ((pExtWnd != NULL) && (pExtWnd->GetHwnd() == hWnd))
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

void CTabbedToDoCtrl::SetVisibleExtensionViews(const CStringArray& aTypeIDs)
{
	// update extension visibility
	int nExt = m_mgrUIExt.GetNumUIExtensions();

	while (nExt--)
	{
		FTC_VIEW nView = (FTC_VIEW)(FTCV_UIEXTENSION1 + nExt);

		VIEWDATA* pData = GetViewData(nView);
		ASSERT(pData);

		// update tab control
		CString sTypeID = m_mgrUIExt.GetUIExtensionTypeID(nExt);
		BOOL bVisible = (Misc::Find(aTypeIDs, sTypeID, FALSE, FALSE) != -1);

		m_tabViews.ShowViewTab(nView, bVisible);
	}
}

int CTabbedToDoCtrl::GetVisibleExtensionViews(CStringArray& aTypeIDs) const
{
	ASSERT(GetSafeHwnd());

	aTypeIDs.RemoveAll();

	int nExt = m_mgrUIExt.GetNumUIExtensions();

	while (nExt--)
	{
		FTC_VIEW nView = (FTC_VIEW)(FTCV_UIEXTENSION1 + nExt);

		if (m_tabViews.IsViewTabShowing(nView))
			aTypeIDs.Add(m_mgrUIExt.GetUIExtensionTypeID(nExt));
	}

	return aTypeIDs.GetSize();
}

BOOL CTabbedToDoCtrl::CanResizeAttributeColumnsToFit() const
{
	FTC_VIEW nView = GetView();
	
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
	FTC_VIEW nView = GetView();
	
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

	if (!InTreeView())
	{
		ASSERT(m_aRecreatedRecurringTasks.GetSize());
		
		SelectTasks(m_aRecreatedRecurringTasks);
	}

	// cleanup
	m_aRecreatedRecurringTasks.RemoveAll();

	return 0L;
}
