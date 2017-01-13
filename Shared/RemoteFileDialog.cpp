// RemoteFileDialog.cpp : implementation file
//

#include "stdafx.h"
#include "RemoteFileDialog.h"

#include "deferwndmove.h"
#include "dlgunits.h"
#include "sysimagelist.h"
#include "autoflag.h"
#include "misc.h"
#include "filemisc.h"
#include "enstring.h"

#include "..\Interfaces\ipreferences.h"

#include <afxres.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#define IDC_GRIPPER		                1002
#define IDC_CURRENTFOLDER               1003
#define IDC_FILELIST                    1004
#define IDC_FILENAME                    1005
#define IDC_FILETYPES                   1006
#define IDC_CURFOLDERLABEL              1007
#define IDC_FILENAMELABEL               1008
#define IDC_FILETYPESLABEL              1009

enum
{
	ID_VIEWMENU = 32771,
	ID_UPONELEVEL,
};

enum 
{ 
	RFDT_FOLDER, 
	RFDT_FILE 
};

LPCTSTR DEFFILTER = _T("All Files (*.*)|*.*||");

enum // columns 
{
	FILENAME,
	FILESIZE,
	MODDATE,
};

const UINT VIEWIDS[] = { ID_VIEW_LARGEICON, ID_VIEW_DETAILS, ID_VIEW_SMALLICON, ID_VIEW_LIST};
const UINT VIEWS[] = { LVS_SMALLICON, LVS_ICON, LVS_LIST, LVS_REPORT }; 

/////////////////////////////////////////////////////////////////////////////

FILERESULT::FILERESULT(LPCTSTR szFilePath, DWORD size) 
	: 
	dwSize(size),
	sFilePath(szFilePath)
{ 
	if (!Misc::IsEmpty(szFilePath))
		sFileName = FileMisc::GetFileNameFromPath(szFilePath);
}

/////////////////////////////////////////////////////////////////////////////
// CRemoteFileDialog dialog

CRemoteFileDialog::CRemoteFileDialog(CFtpConnection* pConnection, LPCTSTR szServer, LPCTSTR szFilters, LPCTSTR szInitialFolder)
	: m_pConnection(pConnection), 
	  m_sServer(szServer), 
	  m_sCurFolder(szInitialFolder), 
	  m_eCurFolder(FES_NOBROWSE | FES_FOLDERS), 
	  m_eFilename(FES_NOBROWSE), 
	  m_bRoot(FALSE),
	  m_dwFileSize(0),
 	  m_silLarge(TRUE), 
 	  m_silSmall(FALSE),
	  m_bInitReport(FALSE),
	  m_dwOptions(0),
	  m_bFilling(FALSE),
	  m_pPrefs(NULL)
{
	//{{AFX_DATA_INIT(CRemoteFileDialog)
	//}}AFX_DATA_INIT
	SetBordersDLU(3);

	AddRCControl(_T("SCROLLBAR"), _T(""), _T(""), SBS_RIGHTALIGN, 0,270,160,8,16, IDC_GRIPPER);
    AddRCControl(_T("LTEXT"), _T(""), _T("Current Folder:"), 0, 0,0,7,70,8,IDC_CURFOLDERLABEL);
    AddRCControl(_T("EDITTEXT"), _T(""), _T(""), ES_AUTOHSCROLL | ES_READONLY, 0,74,4,198,14,IDC_CURRENTFOLDER);
    AddRCControl(_T("LTEXT"), _T(""), _T("Files of &type:"), 0, 0,0,164,70,8,IDC_FILETYPESLABEL);
    AddRCControl(_T("LTEXT"), _T(""), _T("Remote file &name:"), 0, 0,0,146,70,8,IDC_FILENAMELABEL);
    AddRCControl(_T("COMBOBOX"), _T(""), _T(""), CBS_DROPDOWNLIST | CBS_SORT | WS_VSCROLL | WS_TABSTOP, 0,74,163,154,100,IDC_FILETYPES);
    AddRCControl(_T("CONTROL"), _T("SysListView32"), _T(""), LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_LIST | LVS_SHAREIMAGELISTS | WS_TABSTOP, WS_EX_CLIENTEDGE,0,22,280,114,IDC_FILELIST);
    AddRCControl(_T("EDITTEXT"), _T(""), _T(""),ES_AUTOHSCROLL | WS_TABSTOP, 0,74,144,154,13, IDC_FILENAME);
	AddRCControl(_T("DEFPUSHBUTTON"), _T(""), _T("OK"), WS_TABSTOP, 0,230,143,50,14, IDOK);
    AddRCControl(_T("PUSHBUTTON"), _T(""), _T("Cancel"), WS_TABSTOP, 0,230,162,50,14,IDCANCEL);

	InitFilterArray(szFilters);

	ASSERT(m_pConnection);
}

CRemoteFileDialog::~CRemoteFileDialog()
{
}

void CRemoteFileDialog::DoDataExchange(CDataExchange* pDX)
{
	CRuntimeDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRemoteFileDialog)
	DDX_Control(pDX, IDC_CURRENTFOLDER, m_eCurFolder);
	DDX_Control(pDX, IDC_FILETYPES, m_cbFileTypes);
	DDX_Control(pDX, IDC_FILELIST, m_lcFiles);
	DDX_Text(pDX, IDC_CURRENTFOLDER, m_sCurFolder);
	DDX_Text(pDX, IDC_FILENAME, m_sFilenames);
	DDX_Control(pDX, IDC_FILENAME, m_eFilename);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		int nSel = m_cbFileTypes.GetCurSel();

		if (nSel != CB_ERR)
			m_sFilterExt = m_aFilters[m_cbFileTypes.GetItemData(nSel)].sExt;
		else
			m_sFilterExt.Empty();
	}
}


BEGIN_MESSAGE_MAP(CRemoteFileDialog, CRuntimeDlg)
	//{{AFX_MSG_MAP(CRemoteFileDialog)
	ON_CBN_SELCHANGE(IDC_FILETYPES, OnSelchangeFiletypes)
	ON_EN_CHANGE(IDC_FILENAME, OnChangeFilename)
	ON_NOTIFY(NM_DBLCLK, IDC_FILELIST, OnDblclkFilelist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FILELIST, OnItemchangedFilelist)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, OnToolbarDropDown)
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolbarNeedText)
	ON_COMMAND(ID_VIEWMENU, OnViewMenu)
	ON_COMMAND(ID_UPONELEVEL, OnUpOneLevel)
	ON_COMMAND_RANGE(ID_VIEW_SMALLICON, ID_VIEW_DETAILS, OnChangeView)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRemoteFileDialog message handlers

BOOL CRemoteFileDialog::OnInitDialog() 
{
	CRuntimeDlg::OnInitDialog();

	ASSERT(m_pConnection);

	// init state from prefs
	if (m_sCurFolder.IsEmpty())
	{
		CString sFolderKey = m_sPrefKey + _T("LastFolder");
		m_sCurFolder = m_pPrefs->GetProfileString(sFolderKey, m_sServer);
	}

	if (m_pConnection && m_sCurFolder.IsEmpty())
		m_pConnection->GetCurrentDirectory(m_sCurFolder);

	UpdateData(FALSE);

	ModifyStyle(0, WS_CLIPCHILDREN);

	if (!m_pConnection)
		EndDialog(IDCANCEL);

	if (FolderSelect())
		GetDlgItem(IDC_FILENAMELABEL)->SetWindowText(_T("Remote folder &name:"));

	// set up list image lists
 	if (m_silLarge.Initialize())
 		m_lcFiles.SetImageList(m_silLarge.GetImageList(), LVSIL_NORMAL);
 
 	if (m_silSmall.Initialize())
 		m_lcFiles.SetImageList(m_silSmall.GetImageList(), LVSIL_SMALL);

	// init multi selection
	if (m_dwOptions & RFD_MULTISELECT)
		m_lcFiles.ModifyStyle(LVS_SINGLESEL, 0);

	// init m_cbFileTypes
	int nType = m_aFilters.GetSize();
	ASSERT (nType); // must be atleast one

	while (nType--)
	{
		int nIndex = m_cbFileTypes.InsertString(0, m_aFilters[nType].sName);
		m_cbFileTypes.SetItemData(nIndex, nType); // mapping
	}

	m_cbFileTypes.SetCurSel(0);
	m_sFilterExt = m_aFilters[m_cbFileTypes.GetItemData(0)].sExt;

	// add columns to filelist
	m_lcFiles.InsertColumn(FILENAME, _T("Name"), LVCFMT_LEFT, 150);
	m_lcFiles.InsertColumn(FILESIZE, _T("Size"), LVCFMT_RIGHT, 60);
	m_lcFiles.InsertColumn(MODDATE, _T("Last Modified"), LVCFMT_LEFT, 150);

	UpdateOKButton();

	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRemoteFileDialog::PostCreate()
{
	if (m_toolbar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE))
	{
		UINT nStyle = m_toolbar.GetBarStyle();
		nStyle &= ~(CCS_NORESIZE | CCS_NOPARENTALIGN | CBRS_BORDER_ANY);
		nStyle |= (CBRS_SIZE_FIXED | CBRS_TOOLTIPS | CBRS_FLYBY);
		m_toolbar.SetBarStyle(nStyle);

		m_toolbar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);

		int iImage = m_toolbar.GetToolBarCtrl().SendMessage(TB_LOADIMAGES, IDB_VIEW_SMALL_COLOR, (LPARAM)HINST_COMMCTRL);

		// add 'up folder' item
		TBBUTTON tbbUp = { iImage + VIEW_PARENTFOLDER, ID_UPONELEVEL, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }; 
 		m_toolbar.GetToolBarCtrl().InsertButton(0, &tbbUp);

		// add 'view' item with drop menu
		TBBUTTON tbbView = { iImage + VIEW_LIST, ID_VIEWMENU, TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, 0, 0 }; 
 		m_toolbar.GetToolBarCtrl().InsertButton(1, &tbbView);

		// move to topright corner
		CDlgUnits dlu(*this);
		CRect rToolbar, rButton;

		GetClientRect(rToolbar);
		m_toolbar.GetItemRect(1, rButton);

		rToolbar.top += dlu.ToPixelsY(4);
		rToolbar.right -= dlu.ToPixelsX(7);
		rToolbar.left = rToolbar.right - rButton.right;
		rToolbar.bottom = rToolbar.top + rButton.Height();

		m_toolbar.MoveWindow(rToolbar);

		CRect rCtrl = OffsetCtrl(IDC_CURRENTFOLDER);
		ResizeCtrl(IDC_CURRENTFOLDER, rToolbar.left - rCtrl.right - 4, 0);
	}

	// restore size
	DWORD dwSize = m_pPrefs->GetProfileInt(m_sPrefKey, _T("LastSize"), -1);

	if (dwSize != -1)
	{
		int nWidth = LOWORD(dwSize);
		int nHeight = HIWORD(dwSize);

		// centre on current position
		CRect rWindow;
		GetWindowRect(rWindow);

		rWindow.InflateRect((nWidth - rWindow.Width()) / 2, (nHeight - rWindow.Height()) / 2);
		MoveWindow(rWindow);
	}

	// restore last view
	int nView = m_pPrefs->GetProfileInt(m_sPrefKey, _T("LastView"), LVS_LIST);
	OnChangeView(VIEWIDS[nView]);

	// init resize icon
	GetDlgItem(IDC_GRIPPER)->ModifyStyle(0, SBS_SIZEGRIP | SBS_SIZEBOXTOPLEFTALIGN);

	// and fill the file list
	SetCurrentFolder(m_sCurFolder);
}

int CRemoteFileDialog::DoModal(IPreferences* pPrefs, LPCTSTR szKey, DWORD dwOptions, LPCTSTR szFilename) 
{ 
	m_pPrefs = pPrefs;
	m_sPrefKey = szKey;

	CEnString sCaption;

	if (dwOptions & RFD_UPLOAD)
	{
		if (dwOptions & RFD_FOLDERSELECT)
		{
			dwOptions &= ~RFD_MULTISELECT;
			dwOptions |= RFD_FILEMUSTEXIST;

			m_eFilename.EnableStyle(FES_FOLDERS);

			sCaption.Format(_T("Save To Folder (%s)"), m_sServer);
		}
		else
			sCaption.Format(_T("Save As (%s)"), m_sServer);
	}
	else // download
	{
		dwOptions |= RFD_FILEMUSTEXIST;

		if (dwOptions & RFD_MULTISELECT)
			sCaption.Format(_T("Select Files to Download (%s)"), m_sServer);
		else
			sCaption.Format(_T("Select File to Download (%s)"), m_sServer);
	}

	m_sFilenames = szFilename;
	m_dwOptions = dwOptions;

	return CRuntimeDlg::DoModal(sCaption, RTD_DEFSTYLE | WS_THICKFRAME | WS_CLIPCHILDREN); 
}

void CRemoteFileDialog::OnSelchangeFiletypes() 
{
	CString sCurFilterExt(m_sFilterExt); // cache

	UpdateData();
	
	// re-process current directory if the filter has changed
	if (m_sFilterExt != sCurFilterExt)
		FillFileList();
}

void CRemoteFileDialog::OnChangeFilename() 
{	
	UpdateData();

	UpdateOKButton();
}

void CRemoteFileDialog::UpdateOKButton(BOOL bRefreshResults)
{
	if (bRefreshResults)
		UpdateFileResults();

	BOOL bEnable = m_aFiles.GetSize() || ((m_dwOptions & RFD_UPLOAD) &&
					(m_dwOptions & RFD_FOLDERSELECT) && m_bRoot);

	GetDlgItem(IDOK)->EnableWindow(bEnable);
}

int CRemoteFileDialog::FindMatch(LPCTSTR szFilename)
{
	CString sFilename(szFilename);

	if (Misc::Trim(sFilename).IsEmpty())
		return -1;

	int nMatch = -1;
	LVFINDINFO lvfi = { LVFI_STRING, szFilename, 0, { 0, 0 }, VK_DOWN };

	do
	{
		nMatch = m_lcFiles.FindItem(&lvfi, nMatch);
	}
	while (nMatch != -1 && !(GetItemType(nMatch) == RFDT_FILE || FolderSelect()));

	return nMatch;
}

void CRemoteFileDialog::OnDblclkFilelist(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	int nSel = GetFirstSelectedItem();

	if (nSel != -1)
	{
		int nType = GetItemType(nSel);

		switch (nType)
		{
		case RFDT_FILE:
			if (!FolderSelect())
				OnOK();
			break;

		case RFDT_FOLDER:
			CString sFolder = m_lcFiles.GetItemText(nSel, 0);
			SetCurrentFolder(sFolder);
			break;
		}
	}
	
	*pResult = 0;
}

void CRemoteFileDialog::OnUpOneLevel()
{
	if (!(m_dwOptions & RFD_NONAVIGATE))
		SetCurrentFolder(_T(".."));
}

void CRemoteFileDialog::SetCurrentFolder(LPCTSTR szFolder)
{
	CWaitCursor cursor;

	if (m_pConnection)
	{
		if (m_pConnection->SetCurrentDirectory(szFolder))
			FillFileList();

		else if (m_pConnection->SetCurrentDirectory(_T("/")))
		{
			m_sCurFolder = _T("/");
			FillFileList();
		}
	}
}

void CRemoteFileDialog::FillFileList()
{
	CAutoFlag af(m_bFilling, TRUE);

	if (m_pConnection)
	{
		m_pConnection->GetCurrentDirectory(m_sCurFolder);
		m_bRoot = (m_sCurFolder == _T("/"));

		if (m_bRoot)
			m_sCurFolder = _T("(root)");

		UpdateData(FALSE);
		
		m_lcFiles.DeleteAllItems();
		m_lcFiles.UpdateWindow();
		
		m_lcFiles.SetRedraw(FALSE);
		m_mapItems.RemoveAll();

		CFtpFileFind ff(m_pConnection);
		int nID = 0;

		BOOL bContinue = ff.FindFile();

		while (bContinue)
		{
			bContinue = ff.FindNextFile();

			if (!ff.IsDots())
			{
				if (ff.IsDirectory())
				{
					AddFileItem(ff.GetFileName(), RFDT_FOLDER, nID++, 0, NULL, m_silLarge.GetFolderImageIndex());
				}
				else if (!FolderSelect()) // check extension matches filter
				{
					BOOL bMatch = m_sFilterExt.IsEmpty() || m_sFilterExt == _T(".*");
					
					if (!bMatch)
					{
						CString sExt;
						FileMisc::SplitPath(ff.GetFileName(), NULL, NULL, NULL, &sExt);
						
						CStringArray aFilterExt;
						Misc::Split(m_sFilterExt, aFilterExt, ';');
						
						bMatch = (Misc::Find(aFilterExt, sExt) != -1);
					}
					
					if (bMatch)
					{
						FILETIME tLastMod;
						ff.GetLastWriteTime(&tLastMod);

						/*int nIndex = */
						AddFileItem(ff.GetFileName(), RFDT_FILE, nID++, ff.GetLength(), &tLastMod);
					}
				}
			}
		}

		ff.Close();

		// resort
		m_lcFiles.SortItems(SortProc, (DWORD)&m_mapItems);
		m_lcFiles.SetRedraw(TRUE);

		// attempt to select a match
		int nSel = FindMatch(m_sFilenames);
		
		if (nSel != -1)
		{
			m_lcFiles.SetItemState(nSel, LVIS_SELECTED, LVIS_SELECTED);
			m_lcFiles.EnsureVisible(nSel, FALSE);
		}

		m_toolbar.GetToolBarCtrl().EnableButton(ID_UPONELEVEL, !m_bRoot && !(m_dwOptions & RFD_NONAVIGATE));
		UpdateOKButton();
	}
}

int CRemoteFileDialog::GetFirstSelectedItem()
{
	POSITION pos = m_lcFiles.GetFirstSelectedItemPosition();

	return pos ? m_lcFiles.GetNextSelectedItem(pos) : -1;
}

int CRemoteFileDialog::AddFileItem(LPCTSTR szFileName, int nType, UINT nUniqueID, DWORD dwFileSize, const FILETIME* pLastMod, int nImage)
{
	if (nImage == -1)
		nImage = CSysImageList().GetFileImageIndex(szFileName);
				
	int nItem = m_lcFiles.InsertItem(LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM,
									0, szFileName, 0, 0, nImage, nUniqueID);

	// add to map
	FILEITEM fi;
	fi.sFilename = szFileName;
	fi.nType = nType;
	fi.dwSize = dwFileSize;

	if (pLastMod)
	{
		FILETIME ftLocal;
		FileTimeToLocalFileTime(pLastMod, &ftLocal);

		fi.dLastMod = COleDateTime(ftLocal);
	}

	m_mapItems[nUniqueID] = fi;

	// add size and date item if a file
	if (nType == RFDT_FILE && m_bInitReport)
	{
		m_lcFiles.SetItemText(nItem, 1, FormatSize(fi.dwSize));
		m_lcFiles.SetItemText(nItem, 2, FormatDate(fi.dLastMod));
	}

	return nItem;
}

CString CRemoteFileDialog::FormatDate(double dDate)
{
	CString sDate;

	if (dDate > 0.0)
	{
		COleDateTime date(dDate);
		sDate.Format(_T("%s %s"), date.Format(VAR_DATEVALUEONLY), date.Format(VAR_TIMEVALUEONLY));
	}

	return sDate;
}

CString CRemoteFileDialog::FormatSize(DWORD dwSize)
{
	CString sSize(_T("0 KB"));

	if (dwSize > 0 && dwSize < 1024)
		sSize = _T("1 KB");
	
	else if (dwSize < (1024 * 1024))
		sSize.Format(_T("%d KB"), dwSize / 1024);
	
	else
		sSize.Format(_T("%.02f MB"), dwSize / (1024.0 * 1024.0));
	
	return sSize;
}

int CALLBACK CRemoteFileDialog::SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CMap<int, int, FILEITEM, FILEITEM&>* pMapItems = (CMap<int, int, FILEITEM, FILEITEM&>*)lParamSort;

	FILEITEM fi1, fi2;

	VERIFY (pMapItems->Lookup(lParam1, fi1));
	VERIFY (pMapItems->Lookup(lParam2, fi2));

	if (fi1.nType != fi2.nType)
		return fi1.nType - fi2.nType;

	return fi1.sFilename.CompareNoCase(fi2.sFilename);
}

int CRemoteFileDialog::GetItemType(int nItem)
{
	FILEITEM fi;
	int nUniqueID = (int)m_lcFiles.GetItemData(nItem);

	VERIFY (m_mapItems.Lookup(nUniqueID, fi));

	return fi.nType;
}

DWORD CRemoteFileDialog::GetItemSize(int nItem)
{
	if (nItem < 0 || nItem > m_lcFiles.GetItemCount())
		return 0;

	FILEITEM fi;
	int nUniqueID = (int)m_lcFiles.GetItemData(nItem);

	VERIFY (m_mapItems.Lookup(nUniqueID, fi));

	return fi.dwSize;
}

void CRemoteFileDialog::OnItemchangedFilelist(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	/*NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;*/
	*pResult = 0;

	if (m_bFilling)
		return;

	// rebuild results at the same time
	int nSelCount = m_lcFiles.GetSelectedCount();

	if (nSelCount == 1)
	{
		int nSel = GetFirstSelectedItem();

		if (GetItemType(nSel) == RFDT_FILE || FolderSelect())
		{
			m_aFiles.RemoveAll();

			// save result
			CString sFileName = m_lcFiles.GetItemText(nSel, 0), sFilePath;

			if (!m_bRoot)
				sFilePath.Format(_T("%s/%s"), m_sCurFolder, sFileName);
			else
				sFilePath = sFileName;

			FILERESULT fr(sFilePath, GetItemSize(nSel));
			m_aFiles.Add(fr);
			
			// update current filename
			m_sFilenames = sFileName;
		}
	}
	else if (nSelCount > 1)
	{
		m_sFilenames.Empty();
		POSITION pos = m_lcFiles.GetFirstSelectedItemPosition();

		while (pos)
		{
			int nItem = m_lcFiles.GetNextSelectedItem(pos);

			if (nItem != -1 && GetItemType(nItem) == RFDT_FILE)
			{
				// save result
				CString sFileName = m_lcFiles.GetItemText(nItem, 0), sFilePath;

				if (!m_bRoot)
					sFilePath.Format(_T("%s/%s"), m_sCurFolder, sFileName);
				else
					sFilePath = sFileName;

				FILERESULT fr(sFilePath, GetItemSize(nItem));
				m_aFiles.Add(fr);

				// update current filename
				CString sItem;
				sItem.Format(_T("\"%s\" "), sFileName);
				m_sFilenames += sItem;
			}
		}
	}

	UpdateData(FALSE);
	UpdateOKButton(FALSE);
}

void CRemoteFileDialog::InitFilterArray(LPCTSTR szFilters)
{
	if (szFilters)
	{
		CString sFilters(szFilters);

		int nFind = sFilters.Find('|');

		while (nFind != -1)
		{
			CString sName = sFilters.Left(nFind);
			sFilters = sFilters.Mid(nFind + 1);

			nFind = sFilters.Find('|');

			if (nFind != -1)
			{
				CString sFilter = sFilters.Left(nFind);
				sFilters = sFilters.Mid(nFind + 1);

				if (!sName.IsEmpty() && !sFilter.IsEmpty())
				{
					FILEFILTER ff(sName, sFilter);
					m_aFilters.Add(ff);
				}

				nFind = sFilters.Find('|'); // next pair
			}
		}
	}

	// add default if no filters found
	if (!m_aFilters.GetSize())
	{
		FILEFILTER ff(_T("All Files (*.*)"), _T(""));
		m_aFilters.Add(ff);
	}
}

void CRemoteFileDialog::OnOK()
{
	UpdateData();
	UpdateFileResults();

	// save the current state
	CString sFolderKey = m_sPrefKey + _T("LastFolder");
	m_pPrefs->WriteProfileString(sFolderKey, m_sServer, m_bRoot ? _T("") : m_sCurFolder);
	m_pPrefs->WriteProfileInt(m_sPrefKey, _T("LastView"), (m_lcFiles.GetStyle() & LVS_TYPEMASK));

	CRect rWindow;
	GetWindowRect(rWindow);
	m_pPrefs->WriteProfileInt(m_sPrefKey, _T("LastSize"), MAKELONG(rWindow.Width(), rWindow.Height()));

	// don't end if nothing selected or if RFD_FILEMUSTEXIST is selected
	// and no match can be found
	BOOL bOK = m_aFiles.GetSize() || ((m_dwOptions & RFD_UPLOAD) &&
					(m_dwOptions & RFD_FOLDERSELECT) && m_bRoot);

	if (bOK)
		CRuntimeDlg::OnOK();
}

void CRemoteFileDialog::OnSize(UINT nType, int cx, int cy) 
{
	CRuntimeDlg::OnSize(nType, cx, cy);

	if (!GetDlgItem(IDC_CURRENTFOLDER))
		return; // not ready
	
	// we use the cancel button as our placeholder
	CRect rCancel = OffsetCtrl(IDCANCEL);
	CDlgUnits dlu(*this);

	int nXOffset = cx - rCancel.right - dlu.ToPixelsX(7);
	int nYOffset = cy - rCancel.bottom - dlu.ToPixelsY(7);

	CDeferWndMove dwm(10);
	
	dwm.ResizeCtrl(this, IDC_CURRENTFOLDER, nXOffset, 0);   
	dwm.ResizeCtrl(this, IDC_FILELIST, nXOffset, nYOffset);        

	if (m_toolbar.GetSafeHwnd())
	{
		CRect rToolbar = OffsetCtrl(AFX_IDW_TOOLBAR);
		rToolbar.OffsetRect(nXOffset, 0);
		dwm.MoveWindow(&m_toolbar, rToolbar);
	}

	CRect rCtrl = OffsetCtrl(IDC_FILETYPES);
	rCtrl.right += nXOffset;
	rCtrl.OffsetRect(0, nYOffset);
	dwm.MoveWindow(GetDlgItem(IDC_FILETYPES), rCtrl);

	rCtrl = OffsetCtrl(IDC_FILENAME);
	rCtrl.right += nXOffset;
	rCtrl.OffsetRect(0, nYOffset);
	dwm.MoveWindow(GetDlgItem(IDC_FILENAME), rCtrl);

	dwm.OffsetCtrl(this, IDC_FILENAMELABEL, 0, nYOffset);
	dwm.OffsetCtrl(this, IDC_FILETYPESLABEL, 0, nYOffset);

	dwm.OffsetCtrl(this, IDOK, nXOffset, nYOffset);
	dwm.OffsetCtrl(this, IDCANCEL, nXOffset, nYOffset);

	CRect rGrip = OffsetCtrl(IDC_GRIPPER);
	rGrip.OffsetRect(cx - rGrip.right, cy - rGrip.bottom);
	dwm.MoveWindow(GetDlgItem(IDC_GRIPPER), rGrip);
}

void CRemoteFileDialog::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	CRuntimeDlg::OnGetMinMaxInfo(lpMMI);

	CDlgUnits dlu(*this);

	lpMMI->ptMinTrackSize.x = dlu.ToPixelsX(294);
	lpMMI->ptMinTrackSize.y = dlu.ToPixelsY(200);
}

void CRemoteFileDialog::OnToolbarDropDown(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	OnViewMenu();

	*pResult = TBDDRET_DEFAULT;
}

void CRemoteFileDialog::OnViewMenu()
{
	// build the view menu manually (because we're a non-resource dependent file)
	CMenu menu;

	if (menu.CreatePopupMenu())
	{
		menu.InsertMenu(0, MF_STRING | MF_BYPOSITION, ID_VIEW_LARGEICON, _T("Lar&ge Icons"));  
		menu.InsertMenu(1, MF_STRING | MF_BYPOSITION, ID_VIEW_SMALLICON, _T("S&mall Icons"));
		menu.InsertMenu(2, MF_STRING | MF_BYPOSITION, ID_VIEW_LIST, _T("&List"));       
		menu.InsertMenu(3, MF_STRING | MF_BYPOSITION, ID_VIEW_DETAILS, _T("&Details"));

		// check the appropriate one
		int nView = (m_lcFiles.GetStyle() & LVS_TYPEMASK);
		menu.CheckMenuRadioItem(ID_VIEW_LARGEICON, ID_VIEW_DETAILS, VIEWIDS[nView], MF_BYCOMMAND);

		// get the button rect in screen coords
		CRect rButton;

		m_toolbar.GetItemRect(1, rButton);
		m_toolbar.ClientToScreen(rButton);
		m_toolbar.GetToolBarCtrl().PressButton(ID_VIEWMENU);

		menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rButton.left, rButton.bottom, this);

		m_toolbar.GetToolBarCtrl().PressButton(ID_VIEWMENU, FALSE);
	}
}

void CRemoteFileDialog::OnChangeView(UINT nCmdID)
{
	int nView = VIEWS[nCmdID - ID_VIEW_SMALLICON];
	int nPrevView = (m_lcFiles.GetStyle() & LVS_TYPEMASK);

	m_lcFiles.ModifyStyle(nPrevView, nView);

	if (!m_bInitReport && nView == LVS_REPORT)
	{
		// add all the other column info that has not been added yet
		// because the header control was not created
		int nItem = m_lcFiles.GetItemCount();

		while (nItem--)
		{
			int nID = m_lcFiles.GetItemData(nItem);

			FILEITEM fi;
			VERIFY(m_mapItems.Lookup(nID, fi));

			if (fi.nType == RFDT_FILE)
			{
				m_lcFiles.SetItemText(nItem, 1, FormatSize(fi.dwSize));
				m_lcFiles.SetItemText(nItem, 2, FormatDate(fi.dLastMod));
			}
		}

		m_bInitReport = TRUE;
	}
}

BOOL CRemoteFileDialog::OnToolbarNeedText(UINT /*id*/, NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	TOOLTIPTEXT* pTTT = (TOOLTIPTEXT*)pNMHDR;
	UINT nID = pNMHDR->idFrom;
	
	switch (nID)
	{
	case ID_VIEWMENU:
		pTTT->lpszText = _T("View Menu");
		return TRUE;
		
	case ID_UPONELEVEL:
		pTTT->lpszText = _T("Up One Level");
		return TRUE;
	}

	return FALSE;
}

void CRemoteFileDialog::UpdateFileResults()
{
	m_aFiles.RemoveAll();

	if (m_sFilenames.IsEmpty())
		return;

	// we just parse m_sFilenames
	CString sFileNames(m_sFilenames);
	BOOL bMustExist = FileMustExist(); // for downloads
	int nStartQuote = sFileNames.Find('\"');

	if (nStartQuote == -1)
	{
		// single file
		Misc::Trim(sFileNames);

		int nFindMatch = FindMatch(sFileNames);

		if (!bMustExist || nFindMatch != -1)
		{
			CString sFilePath;

			if (!m_bRoot)
				sFilePath.Format(_T("%s/%s"), m_sCurFolder, sFileNames);
			else
				sFilePath = sFileNames;

			FILERESULT fr(sFilePath, GetItemSize(nFindMatch));
			m_aFiles.Add(fr);
		}
	}
	else // look for pairs of quotes
	{
		while (nStartQuote != -1)
		{
			int nEndQuote = sFileNames.Find('\"', nStartQuote + 1);

			if (nEndQuote != -1)
			{
				CString sFileName = sFileNames.Mid(nStartQuote + 1, nEndQuote - 1 - nStartQuote);
				Misc::Trim(sFileName);

				int nFindMatch = FindMatch(sFileName);

				if (!bMustExist || nFindMatch != -1)
				{
					CString sFilePath;

					if (!m_bRoot)
						sFilePath.Format(_T("%s/%s"), m_sCurFolder, sFileName);
					else
						sFilePath = sFileName;

					FILERESULT fr(sFilePath, GetItemSize(nFindMatch));
					m_aFiles.Add(fr);
				}

				// next pair
				nStartQuote = sFileNames.Find('\"', nEndQuote + 1);
			}
			else
				nStartQuote = -1; // we're done
		}
	}
}

CString CRemoteFileDialog::GetFirstPath()
{
	if (GetPathCount())
		return m_aFiles[0].sFilePath;

	// else
	return _T("");
}

