// TDLTaskIconDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLTaskIconDlg.h"
#include "TDCImageList.h"

#include "..\shared\holdredraw.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\misc.h"
#include "..\shared\themed.h"

#include "..\Interfaces\Preferences.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef LVS_EX_DOUBLEBUFFER
#define LVS_EX_DOUBLEBUFFER 0x00010000
#endif

/////////////////////////////////////////////////////////////////////////////

const LPCTSTR NO_ICON = _T("__NONE__");

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskIconDlg dialog

CTDLTaskIconDlg::CTDLTaskIconDlg(const CTDCImageList& ilIcons, const CString& sSelName, BOOL bWantNoneItem, CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(CTDLTaskIconDlg::IDD, _T("TaskIcons"), pParent), 
	m_ilIcons(ilIcons), 
	m_sIconName(sSelName), 
	m_bMultiSel(FALSE),
	m_bWantNone(bWantNoneItem),
	m_bAllowReload(pParent != NULL)
{
}

CTDLTaskIconDlg::CTDLTaskIconDlg(const CTDCImageList& ilIcons, const CStringArray& aSelNames, CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(CTDLTaskIconDlg::IDD, _T("TaskIcons"), pParent), 
	m_ilIcons(ilIcons), 
	m_bMultiSel(TRUE),
	m_bWantNone(FALSE),
	m_bAllowReload(pParent != NULL)
{
	if (aSelNames.GetSize())
		m_sIconName = aSelNames[0];

	m_aIconNames.Copy(aSelNames);
}


void CTDLTaskIconDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLTaskIconDlg)
	DDX_Control(pDX, IDC_ICONLIST, m_lcIcons);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTDLTaskIconDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLTaskIconDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_ICONLIST, OnDblclkIconlist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ICONLIST, OnItemchangedIconlist)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_ICONLIST, OnEndlabeleditIconlist)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_EDITLABEL, OnEditlabel)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_ICONLIST, OnBeginlabeleditIconlist)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_RELOADICONS, OnReloadIcons)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskIconDlg message handlers

BOOL CTDLTaskIconDlg::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();
	
	ListView_SetImageList(m_lcIcons, m_ilIcons, LVSIL_SMALL);
	BuildListCtrl();

	GetDlgItem(IDC_RELOADICONS)->EnableWindow(m_bAllowReload);
	GetDlgItem(IDC_RELOADICONS)->ShowWindow(m_bAllowReload ? SW_SHOW : SW_HIDE);

	// disable OK button if nothing selected
	EnableDisable();
	m_lcIcons.SetFocus();

	ListView_SetExtendedListViewStyleEx(m_lcIcons, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	ListView_SetExtendedListViewStyleEx(m_lcIcons, LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);

	CThemed::SetWindowTheme(&m_lcIcons, _T("Explorer"));

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CTDLTaskIconDlg::GetIconName() const 
{ 
	if (m_sIconName == NO_ICON)
		return _T("");

	// else
	return m_sIconName; 
}

int CTDLTaskIconDlg::GetIconNames(CStringArray& aSelNames) const
{
	if (m_lcIcons.GetSafeHwnd())
	{
		// get directly from list itself
		aSelNames.RemoveAll();

		POSITION pos = m_lcIcons.GetFirstSelectedItemPosition();
		CString sImage;

		while (pos)
		{
			int nIndex = m_lcIcons.GetNextSelectedItem(pos);
			int nImage = m_lcIcons.GetItemData(nIndex);

			// weed out the empty icon if there are other icons
			if (nImage == -1)
			{
				if (aSelNames.GetSize() || pos)
					continue;

				aSelNames.Add(NO_ICON);
			}
			else
				aSelNames.Add(m_ilIcons.GetImageName(nImage));
		}
	}
	else // just copy what we've got
		aSelNames.Copy(m_aIconNames);

	return aSelNames.GetSize();
}

CString CTDLTaskIconDlg::GetUserIconName(const CString& sImage) const
{
	CPreferences prefs;
	return prefs.GetProfileString(m_sPrefsKey, sImage);
}

void CTDLTaskIconDlg::EnableDisable()
{
	GetDlgItem(IDOK)->EnableWindow(!m_sIconName.IsEmpty());
	GetDlgItem(IDC_EDITLABEL)->EnableWindow(!m_sIconName.IsEmpty() && (m_sIconName != NO_ICON));
}

void CTDLTaskIconDlg::BuildListCtrl()
{
	int nSelImage = -1;

	if (m_lcIcons.GetItemCount())
		GetIconNames(m_aIconNames); // to restore selection

	// scope this else CListCtrl::EnsureVisible will not work
	{
		CHoldRedraw hr(m_lcIcons);
		CPreferences prefs;

		m_lcIcons.DeleteAllItems();
		m_lcIcons.ModifyStyle(m_bMultiSel ? LVS_SINGLESEL : 0, LVS_SORTASCENDING | (m_bMultiSel ? 0 : LVS_SINGLESEL));
		
		int nImage;
		for (nImage = 0; nImage < m_ilIcons.GetImageCount(); nImage++)
		{
			CString sImage, sKey, sBaseName;

			sBaseName = m_ilIcons.GetImageName(nImage);

			// backwards compatibility
			sKey.Format(_T("Icon%d"), nImage + 1);
			sImage = prefs.GetProfileString(m_sPrefsKey, sKey);

			if (sImage.IsEmpty())
				sImage = prefs.GetProfileString(m_sPrefsKey, sBaseName, sBaseName);

			if (sImage != sBaseName)
				m_mapRenamedItems[sBaseName] = sImage;

			int nIndex = m_lcIcons.InsertItem(nImage, sImage, nImage);
			m_lcIcons.SetItemData(nIndex, nImage);

			// look out for selected image
			if (!m_bMultiSel)
			{
				if (!m_sIconName.IsEmpty() && m_sIconName == sBaseName)
					nSelImage = nImage;
			}
			else
			{
				if (Misc::Contains(sBaseName, m_aIconNames, FALSE, TRUE))
				{
					m_lcIcons.SetItemState(nIndex, LVIS_SELECTED, LVIS_SELECTED);

					if (nSelImage == -1)
						nSelImage = nImage;
				}
			}
		}

		// add a <none> option for turning off a task's image
		if (m_bWantNone)
		{
			ASSERT(!m_bMultiSel);

			int nIndex = m_lcIcons.InsertItem(nImage, _T("<none>"), -1);
			m_lcIcons.SetItemData(nIndex, (DWORD)-1);
		}
	}

	// Sort nicely
	m_lcIcons.SortItems(SortFunc, (DWORD)this);
	
	// restore selection
	int nSel = FindListItem(nSelImage);
	m_lcIcons.SetItemState(nSel, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	if (nSel == -1)
		m_sIconName.Empty();
	else
		m_lcIcons.EnsureVisible(nSel, FALSE);
}

void CTDLTaskIconDlg::OnDblclkIconlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMITEMACTIVATE* pNMListView = ( NMITEMACTIVATE*)pNMHDR;
	
	if (pNMListView->iItem == -1)
	{
		m_sIconName.Empty();
	}
	else
	{
		int nImage = m_lcIcons.GetItemData(pNMListView->iItem);

		if (nImage == -1)
			m_sIconName = NO_ICON;
		else 
			m_sIconName = m_ilIcons.GetImageName(nImage);
	}

	// disable OK button if nothing selected
	EnableDisable();
		
	if (!m_sIconName.IsEmpty())
		EndDialog(IDOK);

	*pResult = 0;
}

void CTDLTaskIconDlg::OnItemchangedIconlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (!(pNMListView->uOldState & LVIS_SELECTED) &&
		 (pNMListView->uNewState & LVIS_SELECTED))
	{
		int nImage = m_lcIcons.GetItemData(pNMListView->iItem);

		if (nImage == -1)
			m_sIconName = NO_ICON;
		else 
			m_sIconName = m_ilIcons.GetImageName(nImage);

		// disable OK button if nothing selected
		EnableDisable();
	}
		
	*pResult = 0;
}

void CTDLTaskIconDlg::OnBeginlabeleditIconlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	int nImage = m_lcIcons.GetItemData(pDispInfo->item.iItem);

	*pResult = (nImage == -1);
}

void CTDLTaskIconDlg::OnEndlabeleditIconlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	// update the currently selected name
	// and keep track of edited items
	if (pDispInfo->item.pszText)
	{
		int nImage = m_lcIcons.GetItemData(pDispInfo->item.iItem);
		m_sIconName = m_ilIcons.GetImageName(nImage);

		m_mapRenamedItems[m_sIconName] = pDispInfo->item.pszText;

		SaveRenamedImages();
		BuildListCtrl(); // to resort items
	}
	
	*pResult = 0; // don't save changes
}

void CTDLTaskIconDlg::OnDestroy() 
{
	// save off any saved names to the preferences
	// MUST come before OnDestroy because it deletes the prefs key
	SaveRenamedImages();

	CTDLDialog::OnDestroy();
	
	// snapshot the selected icons
	GetIconNames(m_aIconNames);
}

void CTDLTaskIconDlg::SaveRenamedImages()
{
	CPreferences prefs;
	prefs.DeleteProfileSection(m_sPrefsKey);
	
	if (!m_mapRenamedItems.GetCount())
		return;
	
	for (int nImage = 0; nImage < m_ilIcons.GetImageCount(); nImage++)
	{
		// only save the ones that have been renamed
		CString sKey, sRename, sBaseName = m_ilIcons.GetImageName(nImage);
		
		// don't save the name if it's the same as its default name
		if (m_mapRenamedItems.Lookup(sBaseName, sRename) && sRename != sBaseName)
		{
			prefs.WriteProfileString(m_sPrefsKey, sBaseName, sRename);
		}
	}

	m_mapRenamedItems.RemoveAll();
}

int CTDLTaskIconDlg::FindListItem(int nImage) const
{
	LVFINDINFO lvfi = { 0 };
	
	lvfi.lParam = nImage;
	lvfi.flags = LVFI_PARAM;
	
	return m_lcIcons.FindItem(&lvfi);
}

void CTDLTaskIconDlg::OnRepositionControls(int dx, int dy)
{
	CTDLDialog::OnRepositionControls(dx, dy);
	
	CDialogHelper::ResizeCtrl(this, IDC_ICONLIST, dx, dy);
	CDialogHelper::ResizeCtrl(this, IDC_DIVIDER, dx, 0);

	CDialogHelper::OffsetCtrl(this, IDC_EDITLABEL, 0, dy);
	CDialogHelper::OffsetCtrl(this, IDC_DIVIDER, 0, dy);
	CDialogHelper::OffsetCtrl(this, IDC_RELOADICONS, dx, dy);
	CDialogHelper::OffsetCtrl(this, IDOK, dx, dy);
	CDialogHelper::OffsetCtrl(this, IDCANCEL, dx, dy);
}

void CTDLTaskIconDlg::OnEditlabel() 
{
	int nItem = m_lcIcons.GetNextItem(-1, LVNI_SELECTED);

	if (nItem != -1)
	{
		m_lcIcons.SetFocus();
		m_lcIcons.EditLabel(nItem);
	}
}

BOOL CTDLTaskIconDlg::PreTranslateMessage(MSG* pMsg) 
{
	// Only process if NOT editing labels
	if (pMsg->hwnd != ListView_GetEditControl(m_lcIcons))
	{
		switch (pMsg->message)
		{
		case WM_KEYUP:
			switch (pMsg->wParam)
			{
			case VK_F2:
				{
					int nItem = m_lcIcons.GetNextItem(-1, LVNI_SELECTED);

					if (nItem != -1)
					{
						m_lcIcons.SetFocus();
						m_lcIcons.EditLabel(nItem);
						return TRUE;
					}
				}
				break;
			}
			break;

		case WM_KEYDOWN:
			switch (pMsg->wParam)
			{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				{
					// List View seems to assume that items beginning with the
					// same letter will be contiguous and so it stops as soon
					// as the first character changes. Unfortunately our numbered
					// icons are sorted 'naturally' and so the default searching
					// is broken.
					int nSelItem = m_lcIcons.GetNextItem(-1, LVNI_SELECTED);
					int nNumItem = m_lcIcons.GetItemCount();

					if (SelectNextMatch((TCHAR)pMsg->wParam, nSelItem + 1, nNumItem - 1))
						return TRUE;

					// Wrap around
					if (SelectNextMatch((TCHAR)pMsg->wParam, 0, nSelItem - 1))
						return TRUE;
				}
				break;
			}
			break;
		}
	}
	
	// all else
	return CTDLDialog::PreTranslateMessage(pMsg);
}

BOOL CTDLTaskIconDlg::SelectNextMatch(TCHAR cKeypress, int nFrom, int nTo)
{
	ASSERT(nFrom <= nTo);
	ASSERT(nTo < m_lcIcons.GetItemCount());

	for (int nItem = nFrom; nItem <= nTo; nItem++)
	{
		if (Misc::First(m_lcIcons.GetItemText(nItem, 0)) == cKeypress)
		{
			m_lcIcons.SetItemState(-1, 0, (LVIS_FOCUSED | LVIS_SELECTED));
			m_lcIcons.SetItemState(nItem, (LVIS_FOCUSED | LVIS_SELECTED), (LVIS_FOCUSED | LVIS_SELECTED));
			m_lcIcons.EnsureVisible(nItem, FALSE);

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CTDLTaskIconDlg::OnEraseBkgnd(CDC* pDC) 
{
	if (m_lcIcons.GetSafeHwnd())
		pDC->ExcludeClipRect(CDialogHelper::GetCtrlRect(this, IDC_ICONLIST));
	
	return CTDLDialog::OnEraseBkgnd(pDC);
}

int CALLBACK CTDLTaskIconDlg::SortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	const CTDLTaskIconDlg* pThis = (CTDLTaskIconDlg*)lParamSort;

	return Misc::NaturalCompare(pThis->GetImageName(lParam1), pThis->GetImageName(lParam2));
}

CString CTDLTaskIconDlg::GetImageName(int iImage) const
{
	CString sBaseName = m_ilIcons.GetImageName(iImage), sImage;

	if (!m_mapRenamedItems.Lookup(sBaseName, sImage) ||
		sImage.IsEmpty())
	{
		sImage = sBaseName;
	}

	return sImage;
}

void CTDLTaskIconDlg::OnReloadIcons() 
{
	ASSERT(m_pParentWnd);
	ASSERT(m_bAllowReload);

	if (m_pParentWnd && m_pParentWnd->SendMessage(WM_TDCTI_RELOADICONS))
	{
		ListView_SetImageList(m_lcIcons, m_ilIcons, LVSIL_SMALL);
		BuildListCtrl();
	}
}
