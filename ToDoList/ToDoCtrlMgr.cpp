// CToDoCtrlMgr.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ToDoCtrlMgr.h"
#include "tdstringres.h"
#include "tasktimelog.h"
#include "TDLContentMgr.h"

#include "..\shared\holdredraw.h"
#include "..\shared\enstring.h"
#include "..\shared\filemisc.h"
#include "..\shared\preferences.h"
#include "..\shared\ContentMgr.h"
#include "..\shared\uiextensionmgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const LPCTSTR ARCHIVE_ID = _T(".done");

/////////////////////////////////////////////////////////////////////////////

enum STATUS_IMAGE
{ 
	IM_NONE = -1, 
	IM_READONLY, 
	IM_CHECKEDIN, 
	IM_CHECKEDOUT, 
	IM_NOTLOADED,
	IM_TIMETRACKING
};

/////////////////////////////////////////////////////////////////////////////

#define VALIDINDEX(i) ((i >= 0) && (i < GetCount()))

/////////////////////////////////////////////////////////////////////////////

#define CHECKVALIDINDEX(i)        \
if (!VALIDINDEX(i))               \
{                                 \
	ASSERT(0);                    \
	return;                       \
}

/////////////////////////////////////////////////////////////////////////////

#define CHECKVALIDINDEXRET(i, ret)\
if (!VALIDINDEX(i))               \
{                                 \
	ASSERT(0);                    \
	return ret;                   \
}

/////////////////////////////////////////////////////////////////////////////
// CToDoCtrlMgr dialog

CToDoCtrlMgr::CToDoCtrlMgr(CTabCtrlEx& tabCtrl) : m_tabCtrl(tabCtrl), m_pPrefs(NULL)
{
}

CToDoCtrlMgr::~CToDoCtrlMgr()
{
}

void CToDoCtrlMgr::SetPrefs(const CPreferencesDlg* pPrefs)
{
	m_pPrefs = pPrefs;
}

const CPreferencesDlg& CToDoCtrlMgr::Prefs() const
{
	ASSERT (m_pPrefs);
	return *m_pPrefs;
}

CFilteredToDoCtrl& CToDoCtrlMgr::GetToDoCtrl(int nIndex)
{
	CHECKVALIDINDEXRET(nIndex, GetFallbackToDoCtrl());

	return *(m_aToDoCtrls[nIndex].pTDC);
}

const CFilteredToDoCtrl& CToDoCtrlMgr::GetToDoCtrl(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, GetFallbackToDoCtrl());

	return *(m_aToDoCtrls.GetData()[nIndex].pTDC);
}

CFilteredToDoCtrl& CToDoCtrlMgr::GetFallbackToDoCtrl()
{
	// Only instantiated if called
	static CUIExtensionMgr extm;
	static CTDLContentMgr cm;
	static CONTENTFORMAT cf;
	static TDCCOLEDITFILTERVISIBILITY vis;
	
	static CFilteredToDoCtrl tdcFallback(extm, cm, cf, vis);

	return tdcFallback;
}

CToDoCtrlMgr::TDCITEM& CToDoCtrlMgr::GetTDCItem(int nIndex)
{
	ASSERT(VALIDINDEX(nIndex));

	return m_aToDoCtrls[nIndex];
}

const CToDoCtrlMgr::TDCITEM& CToDoCtrlMgr::GetTDCItem(int nIndex) const
{
	ASSERT(VALIDINDEX(nIndex));
	
	return m_aToDoCtrls.GetData()[nIndex];
}

BOOL CToDoCtrlMgr::GetFileNameAndExt(int nIndex, CString& sFileName, CString& sExt, BOOL bStrict) const
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	CString sPath = GetFilePath(nIndex, bStrict);
	FileMisc::SplitPath(sPath, NULL, NULL, &sFileName, &sExt);

	return (!sFileName.IsEmpty() && !sExt.IsEmpty());
}

CString CToDoCtrlMgr::GetFileName(int nIndex, BOOL bStrict) const
{
	CHECKVALIDINDEXRET(nIndex, _T(""));
	
	return FileMisc::GetFileNameFromPath(GetFilePath(nIndex, bStrict));
}

CString CToDoCtrlMgr::GetFilePath(int nIndex, BOOL bStrict) const
{
	CHECKVALIDINDEXRET(nIndex, _T(""));

	CString sPath = GetToDoCtrl(nIndex).GetFilePath();

	if (sPath.IsEmpty() && !bStrict)
	{
		sPath = m_tabCtrl.GetItemText(nIndex);
		FileMisc::ValidateFileName(sPath);

		// make it a full path
		FileMisc::MakeFullPath(sPath, FileMisc::GetAppFolder());

		// ensure appropriate extension
		FileMisc::ReplaceExtension(sPath, (Prefs().GetEnableTDLExtension() ? _T("tdl") : _T("xml")));
	}

	return sPath;
}

CString CToDoCtrlMgr::GetFolderPath(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, _T(""));
	
	CString sPath = GetToDoCtrl(nIndex).GetFilePath();
	return FileMisc::GetFolderFromFilePath(sPath);
}

CString CToDoCtrlMgr::GetDisplayPath(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, _T(""));

	TSM_TASKLISTINFO storageInfo;

	if (GetStorageDetails(nIndex, storageInfo))
		return storageInfo.szDisplayName;

	// else
	return GetFilePath(nIndex);
}

BOOL CToDoCtrlMgr::UsesStorage(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, FALSE);
	
	return GetTDCItem(nIndex).UsesStorage();
}

BOOL CToDoCtrlMgr::GetStorageDetails(int nIndex, TSM_TASKLISTINFO& info) const
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	if (!UsesStorage(nIndex))
		return FALSE;

	info = GetTDCItem(nIndex).storageinfo;
	return TRUE;
}

BOOL CToDoCtrlMgr::SetStorageDetails(int nIndex, const TSM_TASKLISTINFO& info)
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	GetTDCItem(nIndex).SetStorageDetails(info);

	// reset file timestamp info
	RefreshFileLastModified(nIndex);

	return TRUE;
}

BOOL CToDoCtrlMgr::ClearStorageDetails(int nIndex)
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	if (!UsesStorage(nIndex))
		return FALSE;

	GetTDCItem(nIndex).ClearStorageDetails();
	return TRUE;
}

CString CToDoCtrlMgr::GetFriendlyProjectName(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, _T(""));

	return GetTDCItem(nIndex).GetFriendlyProjectName();
}

CString CToDoCtrlMgr::FormatProjectNameWithFileName(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, _T(""));

	const CFilteredToDoCtrl& tdc = GetToDoCtrl(nIndex);

	CString sName, sFileName = GetFileName(nIndex), sProject = tdc.GetProjectName();

	if (!sProject.IsEmpty() && !sFileName.IsEmpty())
	{
		sName.Format(_T("%s (%s)"), sProject, sFileName);
	}
	else if (!sFileName.IsEmpty())
	{
		sName = sFileName;
	}
	else
	{
		sName = GetFriendlyProjectName(nIndex);
	}

	return sName;
}


void CToDoCtrlMgr::ClearFilePath(int nIndex)
{
	CHECKVALIDINDEX(nIndex);

	GetToDoCtrl(nIndex).ClearFilePath();
}

BOOL CToDoCtrlMgr::HasFilePath(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	return !GetFilePath(nIndex).IsEmpty();
}

TDCM_PATHTYPE CToDoCtrlMgr::GetFilePathType(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, TDCM_UNDEF);

	return GetTDCItem(nIndex).GetPathType();
}

TDCM_PATHTYPE CToDoCtrlMgr::RefreshPathType(int nIndex) 
{
	CHECKVALIDINDEXRET(nIndex, TDCM_UNDEF);

	TDCITEM& tdci = GetTDCItem(nIndex);

	tdci.RefreshPathType();
	return tdci.nPathType;
}

BOOL CToDoCtrlMgr::IsPristine(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, FALSE);
	
	return (!HasFilePath(nIndex) && !IsModified(nIndex));
}

BOOL CToDoCtrlMgr::IsLoaded(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	return GetTDCItem(nIndex).bLoaded;
}

BOOL CToDoCtrlMgr::IsModified(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, FALSE);
	
	return GetToDoCtrl(nIndex).IsModified();
}

void CToDoCtrlMgr::SetLoaded(int nIndex, BOOL bLoaded)
{
	CHECKVALIDINDEX(nIndex);

	TDCITEM& tdci = GetTDCItem(nIndex);

	if (tdci.bLoaded != bLoaded)
	{
		tdci.bLoaded = bLoaded;
		UpdateTabItemImage(nIndex);
	}
}

void CToDoCtrlMgr::RefreshTimeTracking(int nIndex)
{
	CHECKVALIDINDEX(nIndex);
	ASSERT(GetTDCItem(nIndex).bLoaded);
	
	UpdateTabItemImage(nIndex);
}

BOOL CToDoCtrlMgr::RefreshFileLastModified(int nIndex)
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	TDCITEM& tdci = GetTDCItem(nIndex);

	time_t timeNow = FileMisc::GetFileLastModified(tdci.pTDC->GetFilePath());
	time_t timePrev = tdci.tLastMod;

	tdci.tLastMod = timeNow;

	// if the tasklist is checked out then always return FALSE because
	// noone else should be able to modify it (by any means).
	if (tdci.pTDC->IsCheckedOut())
		return FALSE;

	// else
	return (timeNow > 0 && timePrev > 0 && timeNow != timePrev);
}

BOOL CToDoCtrlMgr::RefreshReadOnlyStatus(int nIndex)
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	TDCITEM& tdci = GetTDCItem(nIndex);

	BOOL bReadOnlyNow = CDriveInfo::IsReadonlyPath(tdci.pTDC->GetFilePath()) > 0;
	BOOL bReadOnlyPrev = tdci.bLastStatusReadOnly;

	GetTDCItem(nIndex).bLastStatusReadOnly = bReadOnlyNow;

	BOOL bChange = ((bReadOnlyNow != -1) && (bReadOnlyPrev != -1) && (bReadOnlyNow != bReadOnlyPrev));

	if (bChange)
		UpdateTabItemImage(nIndex);

	return bChange;
}

BOOL CToDoCtrlMgr::GetReadOnlyStatus(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	return GetTDCItem(nIndex).bLastStatusReadOnly;
}

void CToDoCtrlMgr::SetDueItemStatus(int nIndex, TDCM_DUESTATUS nStatus)
{
	CHECKVALIDINDEX(nIndex);

	GetTDCItem(nIndex).nDueStatus = nStatus;
}

BOOL CToDoCtrlMgr::ShowDueTaskNotification(int nIndex, LPCTSTR szFilePath, BOOL bBrowser)
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	CBrowserDlg* pBrowser = NULL;
	const TDCITEM& tdci = GetTDCItem(nIndex);

	if (!m_mapBrowsers.Lookup(tdci.pTDC, pBrowser))
	{
		// give each a unique key for preferences
		CString sKey = tdci.pTDC->GetPreferencesKey(_T("DueTaskViewer"));
	
		pBrowser = new CBrowserDlg(bBrowser, sKey);
		m_mapBrowsers[tdci.pTDC] = pBrowser;

		CEnString sTitle(IDS_DUETASKS_TITLE, GetFriendlyProjectName(nIndex));
		
		return pBrowser->Create(sTitle, szFilePath, AfxGetMainWnd());
	}

	// else existing browser window
	ASSERT(pBrowser->GetSafeHwnd());

	pBrowser->SetDisplayInBrowser(bBrowser);
	pBrowser->SetHideOnClose();

	if (!pBrowser->Navigate(szFilePath))
	{
		pBrowser->ShowWindow(SW_HIDE);
		return FALSE;
	}
			
	pBrowser->ShowWindow(SW_SHOW);
	return TRUE;
}

BOOL CToDoCtrlMgr::VerifyPassword(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	CEnString sExplanation(IDS_SELECTENCRYPTED, GetFriendlyProjectName(nIndex));
		
	return GetToDoCtrl(nIndex).VerifyPassword(sExplanation);
}

TDCM_DUESTATUS CToDoCtrlMgr::GetDueItemStatus(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, TDCM_NONE);

	return GetTDCItem(nIndex).nDueStatus;
}

BOOL CToDoCtrlMgr::GetLastCheckoutSucceeded(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	return GetTDCItem(nIndex).bLastCheckoutSuccess;
}

void CToDoCtrlMgr::SetLastCheckoutSucceeded(int nIndex, BOOL bCheckedOut)
{
	CHECKVALIDINDEX(nIndex);

	GetTDCItem(nIndex).bLastCheckoutSuccess = bCheckedOut;
}

void CToDoCtrlMgr::RefreshLastCheckoutStatus(int nIndex)
{
	CHECKVALIDINDEX(nIndex);
	
	if (IsCheckedOut(nIndex))
		GetTDCItem(nIndex).bLastCheckoutSuccess = TRUE;
}

BOOL CToDoCtrlMgr::CanCheckOut(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	return (CanCheckInOut(nIndex) && !IsCheckedOut(nIndex));
}

BOOL CToDoCtrlMgr::CanCheckIn(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, FALSE);
	
	return (CanCheckInOut(nIndex) && IsCheckedOut(nIndex));
}

TDC_FILE CToDoCtrlMgr::CheckOut(int nIndex)
{
	CHECKVALIDINDEXRET(nIndex, TDCF_UNSET);
	
	CString sTemp;
	return CheckOut(nIndex, sTemp);
}

TDC_FILE CToDoCtrlMgr::CheckOut(int nIndex, CString& sCheckedOutTo)
{
	CHECKVALIDINDEXRET(nIndex, TDCF_UNSET);
	ASSERT(CanCheckOut(nIndex));
	
	CFilteredToDoCtrl& tdc = GetToDoCtrl(nIndex);
	
	TDC_FILE nCheckout = tdc.CheckOut(sCheckedOutTo);
	
	if (nCheckout == TDCF_SUCCESS)
	{
		// re-sync
		UpdateToDoCtrlReadOnlyUIState(nIndex);				
		RefreshFileLastModified(nIndex);
		UpdateTabItemText(nIndex);
		UpdateTabItemImage(nIndex);
	}
	
	SetLastCheckoutSucceeded(nIndex, nCheckout == TDCF_SUCCESS);
	
	return nCheckout;
}

TDC_FILE CToDoCtrlMgr::CheckIn(int nIndex)
{
	CHECKVALIDINDEXRET(nIndex, TDCF_UNSET);
	CFilteredToDoCtrl& tdc = GetToDoCtrl(nIndex);
	
	// sanity check
	ASSERT(GetLastCheckoutSucceeded(nIndex));
	
	TDC_FILE nCheckin = tdc.CheckIn();
	
	if (nCheckin == TDCF_SUCCESS)
	{
		// re-sync
		UpdateToDoCtrlReadOnlyUIState(nIndex);				
		RefreshFileLastModified(nIndex);
		UpdateTabItemText(nIndex);
		UpdateTabItemImage(nIndex);
	}
	
	return nCheckin;
}

BOOL CToDoCtrlMgr::CanCheckInOut(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	const CFilteredToDoCtrl& tdc = GetToDoCtrl(nIndex);
	
	BOOL bCanCheckInOut = tdc.IsSourceControlled();
	
	if (bCanCheckInOut)
	{
		bCanCheckInOut = (tdc.CompareFileFormat() != TDCFF_NEWER);
		
		if (bCanCheckInOut)
			bCanCheckInOut &= !GetReadOnlyStatus(nIndex);
	}

	return bCanCheckInOut;
}

int CToDoCtrlMgr::GetElapsedMinutesSinceLastMod(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, -1);

	const CFilteredToDoCtrl& tdc = GetToDoCtrl(nIndex);
	COleDateTimeSpan dtSpan(COleDateTime::GetCurrentTime() - tdc.GetLastTaskModified());
				
	return (int)dtSpan.GetTotalMinutes();
}

void CToDoCtrlMgr::UpdateToDoCtrlReadOnlyUIState(int nIndex)
{
	CHECKVALIDINDEX(nIndex);

	CFilteredToDoCtrl& tdc = GetToDoCtrl(nIndex);
	UpdateToDoCtrlReadOnlyUIState(tdc);
}

void CToDoCtrlMgr::UpdateToDoCtrlReadOnlyUIState(CFilteredToDoCtrl& tdc)
{
	BOOL bReadOnly = (CDriveInfo::IsReadonlyPath(tdc.GetFilePath()) > 0);
	
    if (!bReadOnly)
        bReadOnly = (tdc.CompareFileFormat() == TDCFF_NEWER);
	
	if (!bReadOnly)
		bReadOnly = (tdc.IsSourceControlled() && !tdc.IsCheckedOut());
	
	tdc.SetReadonly(bReadOnly);
}

void CToDoCtrlMgr::CheckNotifyReadonly(int nIndex) const
{
	CHECKVALIDINDEX(nIndex);
	
	const CPreferencesDlg& userPrefs = Prefs();
	
	if (nIndex >= 0 && userPrefs.GetNotifyReadOnly())
	{
		CString sDisplayPath = GetDisplayPath(nIndex);
		CString sFilePath = GetFilePath(nIndex);
		const CFilteredToDoCtrl& tdc = GetToDoCtrl(nIndex);
		
		CEnString sMessage;

		if (CDriveInfo::IsReadonlyPath(sFilePath) > 0)
		{
			sMessage.Format(IDS_OPENREADONLY, sDisplayPath);
		}
		else if (tdc.CompareFileFormat() == TDCFF_NEWER)
		{
			sMessage.Format(IDS_OPENNEWER, sDisplayPath);
		}
		
		if (!sMessage.IsEmpty())
			AfxMessageBox(sMessage);
	}
}

int CToDoCtrlMgr::RemoveToDoCtrl(int nIndex, BOOL bDelete)
{
	CHECKVALIDINDEXRET(nIndex, -1);

	int nSel = GetSelToDoCtrl(), nNewSel = -1;

	CFilteredToDoCtrl& tdc = GetToDoCtrl(nIndex);
	TDCITEM& tdci = GetTDCItem(nIndex);

	if (bDelete)
	{
		// checkin as our final task
		if (tdci.bLoaded && tdc.IsCheckedOut() && Prefs().GetCheckinOnClose())
			tdc.CheckIn();
	}
	
	m_aToDoCtrls.RemoveAt(nIndex);
	m_tabCtrl.DeleteItem(nIndex);

	// set new selection as close to previous as possible *before* deleting the tasklist
	if (GetCount())
	{
		if (nIndex <= nSel)
			nNewSel = max(0, nSel - 1);
		else
			nNewSel = nSel;

		// and try to find an already loaded tasklist
		if (!IsLoaded(nNewSel))
		{
			nIndex = GetCount();

			while (nIndex--)
			{
				if (IsLoaded(nIndex))
				{
					nNewSel = nIndex;
					break;
				}
			}
		}

		m_tabCtrl.SetCurSel(nNewSel);
	}

	if (bDelete) // else it's going to be re-added
	{
		// cleanup browser
		CBrowserDlg* pBrowser = NULL;

		if (m_mapBrowsers.Lookup(&tdc, pBrowser))
		{
			pBrowser->DestroyWindow();
			delete pBrowser;

			m_mapBrowsers.RemoveKey(&tdc);

			// delete any temp due task notification files
			CString sTempFile;
			sTempFile.Format(_T("ToDoList.due.%d"), nIndex);
			
			FileMisc::DeleteFile(FileMisc::GetTempFilePath(sTempFile, _T("html")), TRUE);
			FileMisc::DeleteFile(FileMisc::GetTempFilePath(sTempFile, _T("txt")), TRUE);
		}

		tdc.DestroyWindow();
		delete &tdc;
	}

	return nNewSel;
}

int CToDoCtrlMgr::AddToDoCtrl(CFilteredToDoCtrl* pCtrl, const TSM_TASKLISTINFO* pInfo, BOOL bLoaded)
{
	TDCITEM tdci(pCtrl, bLoaded, pInfo);

	// add to tab
	int nSel = m_aToDoCtrls.Add(tdci);
	m_tabCtrl.InsertItem(nSel, _T(""));

	RefreshPathType(nSel);
	RefreshLastCheckoutStatus(nSel);

	UpdateTabItemImage(nSel);
	UpdateTabItemText(nSel);

	return nSel;
}

void CToDoCtrlMgr::MoveToDoCtrl(int nIndex, int nNumPlaces)
{
	CHECKVALIDINDEX(nIndex);

	if (nNumPlaces == 0)
		return;

	int nOrgCount = m_tabCtrl.GetItemCount();

	if (!CanMoveToDoCtrl(nIndex, nNumPlaces))
		return;

	// cache selection so we can restore it afterwards
	int nSel = GetSelToDoCtrl(), nNewSel = nSel;

	// work out what the new selection should be
	if (nIndex == nSel)
		nNewSel = (nIndex + nNumPlaces);

	else if (nIndex > nSel && (nIndex + nNumPlaces) <= nSel)
		nNewSel++;

	else if (nIndex < nSel && (nIndex + nNumPlaces) >= nSel)
		nNewSel--;

	// make copies of existing
	TCITEM tci;
	TCHAR szText[128];
	tci.mask = TCIF_TEXT | TCIF_IMAGE;
	tci.pszText = szText;
	tci.cchTextMax = 127;
		
	m_tabCtrl.GetItem(nIndex, &tci);
	TDCITEM tdci = GetTDCItem(nIndex);

	// remove and re-add
	RemoveToDoCtrl(nIndex);

	nIndex += nNumPlaces;

	m_aToDoCtrls.InsertAt(nIndex, tdci);
	m_tabCtrl.InsertItem(nIndex, &tci);

	// set selection
	m_tabCtrl.SetCurSel(nNewSel);

	ASSERT (nOrgCount == m_tabCtrl.GetItemCount());
}

BOOL CToDoCtrlMgr::CanMoveToDoCtrl(int nIndex, int nNumPlaces) const
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	if (nNumPlaces == 0)
		return FALSE;

	nIndex += nNumPlaces;

	return (nIndex >= 0 && nIndex < GetCount());
}

BOOL CToDoCtrlMgr::ArchiveDoneTasks(int nIndex)
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	CFilteredToDoCtrl& tdc = GetToDoCtrl(nIndex);

	TDC_ARCHIVE nRemove = TDC_REMOVENONE;
	
	if (Prefs().GetRemoveArchivedTasks())
	{
		if (Prefs().GetRemoveOnlyOnAbsoluteCompletion())
			nRemove = TDC_REMOVEIFSIBLINGSANDSUBTASKSCOMPLETE;
		else
			nRemove = TDC_REMOVEALL;
	}
	
	return tdc.ArchiveDoneTasks(nRemove, !Prefs().GetDontRemoveFlagged());
}

BOOL CToDoCtrlMgr::ArchiveSelectedTasks(int nIndex)
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	CFilteredToDoCtrl& tdc = GetToDoCtrl(nIndex);
	
	return tdc.ArchiveSelectedTasks(Prefs().GetRemoveArchivedTasks());
}

CString CToDoCtrlMgr::GetArchivePath(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, _T(""));

	CString sArchivePath;
	GetToDoCtrl(nIndex).GetArchivePath(sArchivePath);

	return sArchivePath;
}

int CToDoCtrlMgr::SortToDoCtrlsByName()
{
	int nSel = GetSelToDoCtrl();

	if (!AreToDoCtrlsSorted())
	{
		// save off current selection 
		CFilteredToDoCtrl* pTDC = &GetToDoCtrl(nSel);

		qsort(m_aToDoCtrls.GetData(), m_aToDoCtrls.GetSize(), sizeof(TDCITEM), SortProc);

		nSel = -1;

		// redo the tab text
		for (int nTDC = 0; nTDC < GetCount(); nTDC++)
		{
			UpdateTabItemText(nTDC);

			// check if this was the selected item
			if (pTDC == &GetToDoCtrl(nTDC))
				nSel = nTDC;
		}

		// set selection
		ASSERT (nSel != -1);
	}

	return nSel;
}

int CToDoCtrlMgr::SortProc(const void* v1, const void* v2)
{
	TDCITEM* pTDCI1 = (TDCITEM*)v1;
	TDCITEM* pTDCI2 = (TDCITEM*)v2;

	CString sName1 = pTDCI1->GetFriendlyProjectName();
	CString sName2 = pTDCI2->GetFriendlyProjectName();

	return sName1.CompareNoCase(sName2);
}

BOOL CToDoCtrlMgr::AreToDoCtrlsSorted() const
{
	if (GetCount() <= 1)
		return TRUE;

	// check that each item is 'less' than the next
	CString sPrev = GetTDCItem(0).GetFriendlyProjectName();

	for (int nTDC = 1; nTDC < GetCount(); nTDC++)
	{
		CString sTDC = GetTDCItem(nTDC).GetFriendlyProjectName();

		if (sPrev.CompareNoCase(sTDC) > 0)
			return FALSE;

		sPrev = sTDC;
	}

	return TRUE;
}

void CToDoCtrlMgr::GetAutoListData(TDCAUTOLISTDATA& tldActive, TDCAUTOLISTDATA& tldAll) const
{
	tldActive.RemoveAll();
	tldAll.RemoveAll();

	int nActive = GetSelToDoCtrl(), nTDC = GetCount();

	while (nTDC--)
	{
		const CFilteredToDoCtrl& tdc = GetToDoCtrl(nTDC);

		if (nTDC == nActive)
		{
			tdc.GetAutoListData(tldActive);
			tldAll.AppendUnique(tldActive);
		}
		else
		{
			TDCAUTOLISTDATA tld;

			tdc.GetAutoListData(tld);
			tldAll.AppendUnique(tld);
		}
	}
}

CString CToDoCtrlMgr::UpdateTabItemText(int nIndex)
{
	CHECKVALIDINDEXRET(nIndex, _T(""));

	if (nIndex < 0)
	{
		nIndex = GetSelToDoCtrl();
		
		if (nIndex < 0)
			return _T("");
	}
	
	TDCITEM& tdci = GetTDCItem(nIndex);
	
	// project name
	CString sProjectName = tdci.GetFriendlyProjectName();

	if (tdci.pTDC->IsModified() && !tdci.pTDC->IsReadOnly())
		sProjectName += "*";

    // update tab array
	m_tabCtrl.SetItemText(nIndex, sProjectName);

	// cleanup
	sProjectName.Replace(_T("&&"), _T("&"));
	Misc::Trim(sProjectName);
	
	return sProjectName;
}

BOOL CToDoCtrlMgr::AddToSourceControl(int nIndex, BOOL bAdd)
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	// if removing, try to checkout the tasklist if not already done
	if (!bAdd && !IsCheckedOut(nIndex) && (CheckOut(nIndex) != TDCF_SUCCESS))
		return FALSE;

	if (!GetToDoCtrl(nIndex).AddToSourceControl(bAdd))
		return FALSE;

	UpdateToDoCtrlReadOnlyUIState(nIndex);
	UpdateTabItemText(nIndex);
	UpdateTabItemImage(nIndex);
	RefreshFileLastModified(nIndex);
	RefreshReadOnlyStatus(nIndex);
	RefreshPathType(nIndex);

	return TRUE;
}

CString CToDoCtrlMgr::GetTabItemText(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, _T(""));

	CString sText = m_tabCtrl.GetItemText(nIndex);
	
	// cleanup
	sText.Replace(_T("&&"), _T("&"));

	return sText;
}

int CToDoCtrlMgr::UpdateTabItemImage(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, IM_NONE);

	int nImage = IM_NONE;
	const TDCITEM& tci = GetTDCItem(nIndex);
	
	if (!tci.bLoaded)
	{
		nImage = IM_NOTLOADED;
	}
	else if (tci.bLastStatusReadOnly > 0)
	{
		nImage = IM_READONLY;	
	}
	else if (tci.pTDC->CompareFileFormat() == TDCFF_NEWER)
	{
		nImage = IM_READONLY;	
	}
	else if (tci.pTDC->IsActivelyTimeTracking()) // takes priority
	{
		nImage = IM_TIMETRACKING;
	}
	else if ((tci.bLastStatusReadOnly == 0) && IsSourceControlled(nIndex))
	{
		nImage = (tci.pTDC->IsCheckedOut() ? IM_CHECKEDOUT : IM_CHECKEDIN);	
	}

	// update tab array
	if (nImage != m_tabCtrl.GetItemImage(nIndex))
		m_tabCtrl.SetItemImage(nIndex, nImage);

	return nImage;
}

CString CToDoCtrlMgr::GetTabItemTooltip(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, _T(""));

	CEnString sTooltip;
	const TDCITEM& tci = GetTDCItem(nIndex);

	switch (UpdateTabItemImage(nIndex))
	{
	case IM_READONLY:
		if (tci.pTDC->CompareFileFormat() == TDCFF_NEWER)
		{
			sTooltip.LoadString(IDS_STATUSNEWERFORMAT);
		}
		else
		{
			sTooltip.LoadString(IDS_STATUSREADONLY);
		}
		break;

	case IM_CHECKEDIN:
		sTooltip.LoadString(IDS_STATUSCHECKEDIN);
		break;

	case IM_CHECKEDOUT: 
		sTooltip.LoadString(IDS_STATUSCHECKEDOUT);
		break;

	case IM_NOTLOADED:	
		sTooltip.LoadString(IDS_TABTIP_NOTLOADED);
		break;
		
// 	case IM_TIMETRACKING:	
// 		sTooltip.LoadString(IDS_TABTIP_TIMETRACKING);
// 		break;
	}

	return sTooltip;
}

BOOL CToDoCtrlMgr::IsCheckedOut(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, FALSE);
	
	return GetToDoCtrl(nIndex).IsCheckedOut();
}

BOOL CToDoCtrlMgr::IsSourceControlled(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, FALSE);
	
	return GetToDoCtrl(nIndex).IsSourceControlled();
}

void CToDoCtrlMgr::SetNeedsPreferenceUpdate(int nIndex, BOOL bNeed)
{
	CHECKVALIDINDEX(nIndex);
	
	GetTDCItem(nIndex).bNeedPrefUpdate = bNeed;
}

BOOL CToDoCtrlMgr::GetNeedsPreferenceUpdate(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, FALSE);
	
	return GetTDCItem(nIndex).bNeedPrefUpdate;
}

void CToDoCtrlMgr::SetAllNeedPreferenceUpdate(BOOL bNeed, int nExcept)
{
	for (int nIndex = 0; nIndex < m_aToDoCtrls.GetSize(); nIndex++)
	{
		if (nIndex != nExcept)
			GetTDCItem(nIndex).bNeedPrefUpdate = bNeed;
	}
}

int CToDoCtrlMgr::FindToDoCtrl(LPCTSTR szFilePath) const
{
	int nCtrl = GetCount();
	
	while (nCtrl--)
	{
		if (GetFilePath(nCtrl).CompareNoCase(szFilePath) == 0)
			return nCtrl;
	}

	return -1;
}

int CToDoCtrlMgr::FindToDoCtrl(HWND hwndTDC) const
{
	ASSERT(hwndTDC && ::IsWindow(hwndTDC));

	if ((hwndTDC == NULL) || !::IsWindow(hwndTDC))
		return -1;

	int nCtrl = GetCount();
	
	while (nCtrl--)
	{
		if (hwndTDC == GetTDCItem(nCtrl).pTDC->GetSafeHwnd())
			return nCtrl;
	}
	
	return -1;
}

int CToDoCtrlMgr::FindToDoCtrl(const CFilteredToDoCtrl* pTDC) const
{
	ASSERT(pTDC);

	if (pTDC == NULL)
		return -1;

	return FindToDoCtrl(pTDC->GetSafeHwnd());
}

int CToDoCtrlMgr::FindToDoCtrl(const TSM_TASKLISTINFO& info) const
{
	int nCtrl = GetCount();
	
	while (nCtrl--)
	{
		TSM_TASKLISTINFO ctrlInfo;

		if (GetStorageDetails(nCtrl, ctrlInfo))
		{
			if (_tcscmp(ctrlInfo.szTasklistID, info.szTasklistID) == 0)
				return nCtrl;
		}
	}

	// not found
	return -1;
}

COleDateTime CToDoCtrlMgr::GetMostRecentEdit() const
{
	COleDateTime dtRecent;

	int nCtrl = GetCount();
	
	while (nCtrl--)
	{
		COleDateTime dtLastEdit = GetTDCItem(nCtrl).pTDC->GetLastTaskModified();

		if (!CDateHelper::IsDateSet(dtRecent))
			dtRecent = dtLastEdit;
		else
			dtRecent = min(dtLastEdit, dtRecent);
	}

	return dtRecent;	
}

void CToDoCtrlMgr::PreparePopupMenu(CMenu& menu, UINT nID1, int nMax) const
{
	int nTDC = 0;

	for (nTDC = 0; nTDC < GetCount() && nTDC < nMax; nTDC++)
	{
		CString sTaskList = GetFriendlyProjectName(nTDC);

		menu.ModifyMenu(nID1 + nTDC, MF_BYCOMMAND, nID1 + nTDC, sTaskList);
	}

	// delete any unused items
	for (; nTDC < nMax; nTDC++)
		menu.RemoveMenu(nID1 + nTDC, MF_BYCOMMAND);

}

BOOL CToDoCtrlMgr::HasTasks(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, FALSE);
	
	return GetToDoCtrl(nIndex).GetTaskCount();
}

BOOL CToDoCtrlMgr::AnyHasTasks() const
{
	int nCtrl = GetCount();
	
	while (nCtrl--)
	{
		if (HasTasks(nCtrl))
			return TRUE;
	}
	
	return FALSE;
}

BOOL CToDoCtrlMgr::AnyIsModified() const
{
	int nCtrl = GetCount();
	
	while (nCtrl--)
	{
		if (GetToDoCtrl(nCtrl).IsModified())
			return TRUE;
	}
	
	return FALSE;
}

BOOL CToDoCtrlMgr::DoBackup(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	if (!Prefs().GetBackupOnSave())
		return TRUE;
	
	CString sTDLPath = GetFilePath(nIndex);
	
	if (sTDLPath.IsEmpty())
		return TRUE; // not yet saved

	CString sBackupFolder(Prefs().GetBackupLocation(sTDLPath));
	int nKeepBackups = Prefs().GetKeepBackupCount();

	if (CreateBackup(sTDLPath, sBackupFolder, nKeepBackups))
	{
		// backup task time log files
		if (Prefs().GetLogTaskTimeSeparately())
		{
			FileMisc::TerminatePath(sBackupFolder);
			sBackupFolder += FileMisc::GetFileNameFromPath(sTDLPath, FALSE);
						
#define MAGIC_TASKID	    999999999
#define MAGIC_TASKIDSTR _T("999999999")
			
			// get a file filter
			CString sLogPath = CTDCTaskTimeLog(sTDLPath).GetLogPath(MAGIC_TASKID, TRUE);
			sLogPath.Replace(MAGIC_TASKIDSTR, _T("*"));
			
			CString sDrive, sFolder, sFName, sExt;
			
			FileMisc::SplitPath(sLogPath, &sDrive, &sFolder, &sFName, &sExt);
			FileMisc::MakePath(sLogPath, sDrive, sFolder);
			
			CStringArray aLogFiles;
			int nFile = FileMisc::FindFiles(sFolder, aLogFiles, FALSE, sFName + sExt);

			while (nFile--)
				CreateBackup(aLogFiles[nFile], sBackupFolder, nKeepBackups);
		}
		else // single log file
		{
			CString sLogPath = CTDCTaskTimeLog(sTDLPath).GetLogPath();
			
			CreateBackup(sLogPath, sBackupFolder, nKeepBackups);
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CToDoCtrlMgr::CreateBackup(const CString& sPath, const CString& sBackupFolder, int nKeepBackups)
{
	// NOTE: We encode the app version in the backup name to ensure that
	// in the event that an update contains a breaking bug which corrupts 
	// tasklists we haven't already deleted 'good' backups of the previous 
	// version before the bug is discovered.

	// However, this means that we could accumulate 'nKeepBackups'
	// for as many versions as this user has updated. ie. the number of 
	// backups can grow indefinitely, so we also cull the number of the
	// previous version's backups to a MINIMUM OF ONE.
	if (nKeepBackups)
	{
		CString sBackupPath = CFileBackup::BuildBackupPath(sPath, sBackupFolder, FBS_APPVERSION, _T(""));
		CString sDrive, sFolder, sFName, sExt;
		
		FileMisc::SplitPath(sBackupPath, &sDrive, &sFolder, &sFName, &sExt);
		FileMisc::MakePath(sBackupPath, sDrive, sFolder);
		
		CStringArray aFiles;
		int nNumFiles = FileMisc::FindFiles(sFolder, aFiles, FALSE, sFName + _T("*") + sExt);

		if (nNumFiles >= nKeepBackups)
		{
			Misc::SortArray(aFiles); // sorts oldest backups first
			
			// cull as required
			while (aFiles.GetSize() >= nKeepBackups)
			{
				FileMisc::DeleteFile(aFiles[0], TRUE);
				aFiles.RemoveAt(0);
			}
		}
		else // cull previous version's backups except the last
		{
			nKeepBackups -= nNumFiles;
			nNumFiles = 0;

			// get current app version once only
			CString sAppVer = FileMisc::GetAppVersion('_');
			ASSERT(sFName.Find(sAppVer) != -1);

			// find the first previous version having backups
			CDWordArray aPrevVer;
			
			while (FileMisc::GetPrevAppVersion(aPrevVer, 10, 20) && !nNumFiles) 
			{
				// replace current app ver with previous ver in filename
				CString sPrevVer = Misc::FormatArray(aPrevVer, '_');
				sFName.Replace(sAppVer, sPrevVer);

				nNumFiles = FileMisc::FindFiles(sFolder, aFiles, FALSE, sFName + _T("*") + sExt);

				switch (nNumFiles)
				{
				case 0:
					// keep going
					sAppVer = sPrevVer;
					break;

				case 1:
					// always leave last backup of last version used
					break;

				default:
					Misc::SortArray(aFiles); // see above
					
					// always leave last backup
					while ((aFiles.GetSize() >= nKeepBackups) &&
							(aFiles.GetSize() > 1))
					{
						FileMisc::DeleteFile(aFiles[0], TRUE);
						aFiles.RemoveAt(0);
					}
				}
			}
		}
	}
	
	CFileBackup backup;
	return backup.MakeBackup(sPath, sBackupFolder, FBS_APPVERSION | FBS_TIMESTAMP, _T(""));
}
