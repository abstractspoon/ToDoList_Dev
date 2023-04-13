// CToDoCtrlMgr.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ToDoCtrlMgr.h"
#include "tdstringres.h"
#include "TDCTasktimelog.h"
#include "TDLContentMgr.h"

#include "..\shared\holdredraw.h"
#include "..\shared\enstring.h"
#include "..\shared\filemisc.h"

#include "..\Interfaces\Preferences.h"
#include "..\Interfaces\ContentMgr.h"
#include "..\Interfaces\uiextensionmgr.h"

/////////////////////////////////////////////////////////////////////////////

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
	IM_TIMETRACKING,
	IM_NOTFOUND
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

CToDoCtrlMgr::TDCITEM::TDCITEM() 
{ 
	pTDC = NULL; 
	bLastStatusReadOnly = -1; 
	tLastMod = 0; 
	bLastCheckoutSuccess = -1;
	nPathType = TDCM_UNDEF;
	nDueStatus = TDCM_NONE;
	bNeedPrefUpdate = TRUE;
	nUntitledIndex = -1;
	bLoaded = TRUE;
	crTab = CLR_NONE;
}

CToDoCtrlMgr::TDCITEM::TDCITEM(CFilteredToDoCtrl* pCtrl, const TSM_TASKLISTINFO* pInfo) 
{ 
	pTDC = pCtrl; 
	bLoaded = (pCtrl->HasFilePath() && !pTDC->IsDelayLoaded());
	bLastStatusReadOnly = -1;
	tLastMod = 0;
	bLastCheckoutSuccess = -1;
	nDueStatus = TDCM_NONE;
	bNeedPrefUpdate = TRUE;
	nUntitledIndex = -1;
	crTab = CLR_NONE;

	if (pInfo && pInfo->HasInfo())
	{
		SetStorageDetails(*pInfo);
	}
	else
	{
		CString sFilePath = pCtrl->GetFilePath();

		if (!sFilePath.IsEmpty())
		{
			bLastStatusReadOnly = (CDriveInfo::IsReadonlyPath(sFilePath) > 0);
			tLastMod = FileMisc::GetFileLastModified(sFilePath);
		}
		else if (!pTDC->IsPristine())
		{
			// Set a unique index for non-pristine, non-empty tasklists
			static int nNextUntitledIndex = 1;

			nUntitledIndex = nNextUntitledIndex++;
		}

		if (pCtrl->IsCheckedOut())
			bLastCheckoutSuccess = TRUE;
	}

	RefreshPathType();
}

CToDoCtrlMgr::TDCITEM::~TDCITEM()
{
}

BOOL CToDoCtrlMgr::TDCITEM::IsSelectable() const
{
	return (!HasFilePath() || bLoaded || FileMisc::FileExists(pTDC->GetFilePath()));
}

BOOL CToDoCtrlMgr::TDCITEM::HasFilePath() const
{
	return pTDC->HasFilePath();
}

TDCM_PATHTYPE CToDoCtrlMgr::TDCITEM::GetPathType() const 
{ 
	return nPathType; 
}

BOOL CToDoCtrlMgr::TDCITEM::UsesStorage() const 
{ 
	return !storageinfo.sStorageID.IsEmpty(); 
}

void CToDoCtrlMgr::TDCITEM::RefreshPathType() 
{ 
	// special case
	if (UsesStorage())
	{
		nPathType = TDCM_OTHER;
	}
	else
	{
		LPCTSTR szFilePath = pTDC->GetFilePath();
		nPathType = TranslatePathType(CDriveInfo::GetPathType(szFilePath));
	}
}

void CToDoCtrlMgr::TDCITEM::ClearStorageDetails()
{
	storageinfo.Reset();
	pTDC->SetAlternatePreferencesKey(_T(""));
}

void CToDoCtrlMgr::TDCITEM::SetStorageDetails(const TSM_TASKLISTINFO& info)
{
	if (info.HasInfo())
	{
		storageinfo = info;

		// set filename and alternate pref name to be the display name
		pTDC->SetFilePath(info.szDisplayName);
		pTDC->SetAlternatePreferencesKey(info.szDisplayName);
	}
	else
		ClearStorageDetails();
}

CString CToDoCtrlMgr::TDCITEM::GetFriendlyProjectName() const 
{ 
	if (UsesStorage() && pTDC->GetProjectName().IsEmpty())
		return storageinfo.szDisplayName;

	// else
	return pTDC->GetFriendlyProjectName(nUntitledIndex); 
}

TDCM_PATHTYPE CToDoCtrlMgr::TDCITEM::TranslatePathType(int nDriveInfoType)
{
	switch (nDriveInfoType)
	{
	case DRIVE_REMOTE:
		return TDCM_REMOTE;

	case DRIVE_REMOVABLE:
	case DRIVE_CDROM:
		return TDCM_REMOVABLE;

	case DRIVE_FIXED:
		return TDCM_FIXED;
	}

	// all else
	return TDCM_UNDEF;
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
	static CShortcutManager sm;
	static CONTENTFORMAT cf;
	static TDCCOLEDITFILTERVISIBILITY vis;
	
	static CFilteredToDoCtrl tdcFallback(extm, cm, sm, cf, vis);

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

int CToDoCtrlMgr::GetFileNames(CStringArray& aFileNames, BOOL bStrict) const
{
	int nPath = GetFilePaths(aFileNames, bStrict);
	
	while (nPath--)
		aFileNames[nPath] = FileMisc::GetFileNameFromPath(aFileNames[nPath]);
	
	return aFileNames.GetSize();
}

int CToDoCtrlMgr::GetFilePaths(CStringArray& aFilePaths, BOOL bStrict) const
{
	aFilePaths.RemoveAll();

	for (int nTDC = 0; nTDC < GetCount(); nTDC++)
	{
		CString sPath = GetFilePath(nTDC, bStrict);

		if (!sPath.IsEmpty())
			aFilePaths.Add(sPath);
	}

	return aFilePaths.GetSize();
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

int CToDoCtrlMgr::GetNextMostSelectableToDoCtrl(int nIndex) const
{
	int nNextIndex = -1;

	// Sort tasklists in preferred order of selection
	CArray<TDCITEM, TDCITEM&> aTemp;

	aTemp.Copy(m_aToDoCtrls);
	aTemp.RemoveAt(nIndex);

	if (aTemp.GetSize() > 0)
	{
		Misc::SortArrayT<TDCITEM>(aTemp, SelectabilitySortProc);

		// First item is the one we want
		nNextIndex = FindToDoCtrl(aTemp[0].pTDC);

		ASSERT(nNextIndex != -1);

		// Check it's selectable
		if (!GetTDCItem(nNextIndex).IsSelectable())
			return -1;
	}

	return nNextIndex;
}

void CToDoCtrlMgr::ClearFilePath(int nIndex)
{
	CHECKVALIDINDEX(nIndex);

	GetToDoCtrl(nIndex).ClearFilePath();
}

BOOL CToDoCtrlMgr::HasFilePath(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	return GetTDCItem(nIndex).HasFilePath();
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

BOOL CToDoCtrlMgr::IsPristine() const
{
	return ((GetCount() == 1) && IsPristine(0));
}

BOOL CToDoCtrlMgr::IsPristine(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	return GetTDCItem(nIndex).pTDC->IsPristine();
}

BOOL CToDoCtrlMgr::IsLoaded(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	return GetTDCItem(nIndex).bLoaded;
}

BOOL CToDoCtrlMgr::FileExists(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	return FileMisc::FileExists(GetTDCItem(nIndex).pTDC->GetFilePath());
}

BOOL CToDoCtrlMgr::IsModified(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, FALSE);
	
	return GetToDoCtrl(nIndex).IsModified();
}

void CToDoCtrlMgr::SetLoaded(int nIndex)
{
	CHECKVALIDINDEX(nIndex);

	TDCITEM& tdci = GetTDCItem(nIndex);
	
	if (!tdci.bLoaded)
	{
		ASSERT(!tdci.pTDC->IsDelayLoaded());

		tdci.bLoaded = TRUE;

		UpdateTabItemImage(nIndex);
		UpdateTabItemText(nIndex);
	}
}

void CToDoCtrlMgr::RefreshTimeTracking(int nIndex)
{
	CHECKVALIDINDEX(nIndex);

#ifdef _DEBUG
	const TDCITEM& tdci = GetTDCItem(nIndex);
	ASSERT(tdci.bLoaded || !tdci.HasFilePath());
#endif

	UpdateTabItemImage(nIndex);
}

BOOL CToDoCtrlMgr::RefreshFileLastModified(int nIndex)
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	TDCITEM& tdci = GetTDCItem(nIndex);

	time64_t timeNow = FileMisc::GetFileLastModified(tdci.pTDC->GetFilePath());
	time64_t timePrev = tdci.tLastMod;

	tdci.tLastMod = timeNow;

	// if the tasklist is checked out then always return FALSE because
	// noone else should be able to modify it (by any means).
	if (tdci.pTDC->IsCheckedOut())
		return FALSE;

	// else
	return ((timeNow > 0) && (timePrev > 0) && (timeNow != timePrev));
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

BOOL CToDoCtrlMgr::RefreshDueItemStatus()
{
	BOOL bChanged = FALSE;

	for (int nCtrl = 0; nCtrl <= GetCount(); nCtrl++)
		bChanged |= RefreshDueItemStatus(nCtrl);

	return bChanged;
}

BOOL CToDoCtrlMgr::RefreshDueItemStatus(int nIndex)
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	if (IsLoaded(nIndex))
	{
		const CFilteredToDoCtrl& tdc = GetToDoCtrl(nIndex);
		TDCM_DUESTATUS nStatus = TDCM_NONE;

		if (tdc.HasOverdueTasks()) // takes priority
		{
			nStatus = TDCM_PAST;
		}
		else if (tdc.HasDueTodayTasks())
		{
			nStatus = TDCM_TODAY;
		}

		if (nStatus != GetDueItemStatus(nIndex))
		{
			SetDueItemStatus(nIndex, nStatus);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CToDoCtrlMgr::ShowDueTaskNotification(int nIndex, LPCTSTR szFilePath, BOOL bBrowser)
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	CTDLBrowserDlg* pBrowser = NULL;
	CFilteredToDoCtrl& tdc = GetToDoCtrl(nIndex);

	if (!m_mapBrowsers.Lookup(&tdc, pBrowser))
	{
		pBrowser = new CTDLBrowserDlg(bBrowser);
		m_mapBrowsers[&tdc] = pBrowser;

		CEnString sTitle(IDS_DUETASKS_TITLE, GetFriendlyProjectName(nIndex));
		
		if (!pBrowser->Create(sTitle, szFilePath, AfxGetMainWnd()))
			return FALSE;

		CPreferences prefs;
		pBrowser->RestorePosition(prefs, tdc.GetPreferencesKey(_T("DueTaskViewer")));

		return TRUE;
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

BOOL CToDoCtrlMgr::VerifyPassword(int nIndex, BOOL bForce) const
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	const CFilteredToDoCtrl& tdc = GetToDoCtrl(nIndex);

	if (!bForce && !tdc.WantPasswordReprompting())
		return TRUE;

	// else
	CEnString sExplanation(IDS_SELECTENCRYPTED, GetFriendlyProjectName(nIndex));
		
	return tdc.VerifyPassword(sExplanation);
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

TDC_FILE CToDoCtrlMgr::CheckOut(int nIndex, CString& sCheckedOutTo, BOOL bForce)
{
	CHECKVALIDINDEXRET(nIndex, TDCF_UNSET);
	ASSERT(CanCheckOut(nIndex));
	
	TDC_FILE nCheckout = GetToDoCtrl(nIndex).CheckOut(sCheckedOutTo, bForce);
	
	if (nCheckout == TDCF_SUCCESS)
	{
		// re-sync
		UpdateToDoCtrlReadOnlyUIState(nIndex);				
		RefreshFileLastModified(nIndex);
		UpdateTabItemText(nIndex);
		UpdateTabItemImage(nIndex);
	}
	
	SetLastCheckoutSucceeded(nIndex, (nCheckout == TDCF_SUCCESS));
	
	return nCheckout;
}

TDC_FILE CToDoCtrlMgr::CheckIn(int nIndex)
{
	CHECKVALIDINDEXRET(nIndex, TDCF_UNSET);
	ASSERT(GetLastCheckoutSucceeded(nIndex));
	
	TDC_FILE nCheckin = GetToDoCtrl(nIndex).CheckIn();
	
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
	
	if (!tdc.IsSourceControlled())
		return FALSE;

	if (tdc.CompareFileFormat() == TDCFF_NEWER)
		return FALSE;

	return !GetReadOnlyStatus(nIndex);
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

int CToDoCtrlMgr::DeleteToDoCtrl(int nIndex)
{
	CHECKVALIDINDEXRET(nIndex, -1);

	CPreferences prefs;
	int nSel = GetSelToDoCtrl(), nNewSel = -1;

	CFilteredToDoCtrl& tdc = GetToDoCtrl(nIndex);
	TDCITEM& tdci = GetTDCItem(nIndex);

	if (tdci.HasFilePath())
	{
		// checkin as our final task
		if (tdci.bLoaded && tdc.IsCheckedOut() && Prefs().GetCheckinOnClose())
			tdc.CheckIn();

		// Save tasklist tab colour if any
		CString sKey = tdc.GetPreferencesKey();

		if (!sKey.IsEmpty())
		{
			if (tdci.crTab == CLR_NONE)
				prefs.DeleteProfileEntry(sKey, _T("TabColor"));
			else
				prefs.WriteProfileInt(sKey, _T("TabColor"), tdci.crTab);
		}
	}
	
	m_aToDoCtrls.RemoveAt(nIndex);
	m_tabCtrl.DeleteItem(nIndex);

	// set new selection as close to previous as possible BEFORE deleting the tasklist
	if (GetCount())
	{
		if (nIndex <= nSel)
			nNewSel = max(0, nSel - 1);
		else
			nNewSel = nSel;

		// If it's not loaded just look for next most selectable
		if (HasFilePath(nNewSel) && !IsLoaded(nNewSel))
		{
			int nAltSel = GetNextMostSelectableToDoCtrl(nNewSel);

			if (nAltSel != -1)
				nNewSel = nAltSel;
		}

		m_tabCtrl.SetCurSel(nNewSel);
	}

	// cleanup browser
	CTDLBrowserDlg* pBrowser = NULL;

	if (m_mapBrowsers.Lookup(&tdc, pBrowser))
	{
		CPreferences prefs;
		pBrowser->SavePosition(prefs, tdc.GetPreferencesKey(_T("DueTaskViewer")));

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

	return nNewSel;
}

int CToDoCtrlMgr::AddToDoCtrl(CFilteredToDoCtrl* pTDC, const TSM_TASKLISTINFO* pInfo)
{
	TDCITEM tdci(pTDC, pInfo);

	// Sanity check - Can only be one pristine tasklist
	if (pTDC->IsPristine() && (FindPristineToDoCtrl() != -1))
	{
		ASSERT(0);
		return -1;
	}

	// Restore tasklist tab colour if any
	CString sKey = pTDC->GetPreferencesKey();
	
	if (!sKey.IsEmpty())
		tdci.crTab = (COLORREF)CPreferences().GetProfileInt(sKey, _T("TabColor"), CLR_NONE);

	// Assume prefs are set
	tdci.bNeedPrefUpdate = FALSE;

	// add to tab
	int nSel = m_aToDoCtrls.Add(tdci);
	m_tabCtrl.InsertItem(nSel, _T(""));

	RefreshPathType(nSel);
	RefreshLastCheckoutStatus(nSel);
	CheckNotifyReadonly(nSel);

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
	{
		nNewSel = (nIndex + nNumPlaces);
	}
	else if (nIndex > nSel && (nIndex + nNumPlaces) <= nSel)
	{
		nNewSel++;
	}
	else if (nIndex < nSel && (nIndex + nNumPlaces) >= nSel)
	{
		nNewSel--;
	}

	// make copies of existing
	TCITEM tci;
	TCHAR szText[128];
	tci.mask = TCIF_TEXT | TCIF_IMAGE;
	tci.pszText = szText;
	tci.cchTextMax = 127;
		
	m_tabCtrl.GetItem(nIndex, &tci);
	TDCITEM tdci = GetTDCItem(nIndex); // copy

	// remove and re-add
	m_aToDoCtrls.RemoveAt(nIndex);
	m_tabCtrl.DeleteItem(nIndex);

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

	BOOL bRemoveFlagged = !Prefs().GetDontRemoveFlagged();
	TDC_ARCHIVE nRemove = TDC_REMOVENONE;
	
	if (Prefs().GetRemoveArchivedTasks())
	{
		if (Prefs().GetRemoveOnlyOnAbsoluteCompletion())
			nRemove = TDC_REMOVEIFSIBLINGSANDSUBTASKSCOMPLETE;
		else
			nRemove = TDC_REMOVEALL;
	}
	
	return GetToDoCtrl(nIndex).ArchiveDoneTasks(nRemove, bRemoveFlagged);
}

BOOL CToDoCtrlMgr::ArchiveSelectedTasks(int nIndex)
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	BOOL bRemove = Prefs().GetRemoveArchivedTasks();
	
	return GetToDoCtrl(nIndex).ArchiveSelectedTasks(bRemove);
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

	if (!AreToDoCtrlsSortedByName())
	{
		// save off current selection 
		CFilteredToDoCtrl* pTDC = &GetToDoCtrl(nSel);

		qsort(m_aToDoCtrls.GetData(), m_aToDoCtrls.GetSize(), sizeof(TDCITEM), NameSortProc);

		nSel = -1;

		// redo the tab text and image
		for (int nTDC = 0; nTDC < GetCount(); nTDC++)
		{
			UpdateTabItemImage(nTDC);
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

int CToDoCtrlMgr::NameSortProc(const void* v1, const void* v2)
{
	const TDCITEM* pTDCI1 = (TDCITEM*)v1;
	const TDCITEM* pTDCI2 = (TDCITEM*)v2;

	CString sName1 = pTDCI1->GetFriendlyProjectName();
	CString sName2 = pTDCI2->GetFriendlyProjectName();

	return sName1.CompareNoCase(sName2);
}

int CToDoCtrlMgr::SelectabilitySortProc(const void* v1, const void* v2)
{
	const TDCITEM* pTDCI1 = (TDCITEM*)v1;
	const TDCITEM* pTDCI2 = (TDCITEM*)v2;

	// Order of select-ability
	//
	// 1. Not previously saved
	CString sPath1 = pTDCI1->pTDC->GetFilePath();
	CString sPath2 = pTDCI2->pTDC->GetFilePath();

	if (sPath1.IsEmpty())
		return -1;

	if (sPath2.IsEmpty())
		return 1;

	// 2a. Loaded (not encrypted)
	// 2b. Loaded (encrypted)
	if (pTDCI1->bLoaded && pTDCI1->bLoaded)
	{
		if (!pTDCI1->pTDC->IsEncrypted())
			return -1;

		if (!pTDCI2->pTDC->IsEncrypted())
			return 1;
	}

	if (pTDCI1->bLoaded)
		return -1;

	if (pTDCI2->bLoaded)
		return 1;

	// 3. File exists
	if (FileMisc::FileExists(pTDCI1->pTDC->GetFilePath()))
		return -1;

	if (FileMisc::FileExists(pTDCI2->pTDC->GetFilePath()))
		return 1;
	
	// else
	return 0;
}

BOOL CToDoCtrlMgr::AreToDoCtrlsSortedByName() const
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

void CToDoCtrlMgr::GetAutoListData(TDCAUTOLISTDATA& tldActive, TDCAUTOLISTDATA& tldAll, TDC_ATTRIBUTE nAttribID) const
{
	tldActive.RemoveAll(nAttribID);
	tldAll.RemoveAll(nAttribID);

	int nActive = GetSelToDoCtrl(), nTDC = GetCount();

	while (nTDC--)
	{
		const CFilteredToDoCtrl& tdc = GetToDoCtrl(nTDC);

		if (nTDC == nActive)
		{
			tdc.GetAutoListData(tldActive, nAttribID);
			tldAll.AppendUnique(tldActive, nAttribID);
		}
		else
		{
			TDCAUTOLISTDATA tld;

			tdc.GetAutoListData(tld, nAttribID);
			tldAll.AppendUnique(tld, nAttribID);
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
	CString sNewTabText = tdci.GetFriendlyProjectName();

	if (tdci.pTDC->IsModified() && !tdci.pTDC->IsReadOnly())
		sNewTabText += "*";

    // update tab array if the text has changed
	CString sCurTabText = Misc::Trim(m_tabCtrl.GetItemText(nIndex));

	if (sNewTabText != sCurTabText)
		m_tabCtrl.SetItemText(nIndex, sNewTabText);

	// cleanup
	sNewTabText.Replace(_T("&&"), _T("&"));
	
	return sNewTabText;
}

BOOL CToDoCtrlMgr::AddToSourceControl(int nIndex, BOOL bAdd)
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

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

BOOL CToDoCtrlMgr::CanAddToSourceControl(int nIndex, BOOL bAdd) const
{
	CHECKVALIDINDEXRET(nIndex, FALSE);

	return GetToDoCtrl(nIndex).CanAddToSourceControl(bAdd);
}

CString CToDoCtrlMgr::GetTabItemText(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, _T(""));

	CString sText = m_tabCtrl.GetItemText(nIndex);
	
	// cleanup
	sText.Replace(_T("&&"), _T("&"));

	return Misc::Trim(sText);
}

int CToDoCtrlMgr::UpdateTabItemImage(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, IM_NONE);

	int nImage = IM_NONE;
	const TDCITEM& tdci = GetTDCItem(nIndex);
	
	if (!tdci.HasFilePath())
	{
		// IM_NONE
	}
	else if (!tdci.bLoaded)
	{
		if (!FileMisc::FileExists(tdci.pTDC->GetFilePath()))
		{
			nImage = IM_NOTFOUND;
		}
		else
		{
			ASSERT(tdci.pTDC->IsDelayLoaded());
			nImage = IM_NOTLOADED;
		}
	}
	else if (tdci.bLastStatusReadOnly > 0)
	{
		nImage = IM_READONLY;	
	}
	else if (tdci.pTDC->CompareFileFormat() == TDCFF_NEWER)
	{
		nImage = IM_READONLY;	
	}
	else if (tdci.pTDC->IsActivelyTimeTracking()) // takes priority
	{
		nImage = IM_TIMETRACKING;
	}
	else if ((tdci.bLastStatusReadOnly == 0) && IsSourceControlled(nIndex))
	{
		nImage = (tdci.pTDC->IsCheckedOut() ? IM_CHECKEDOUT : IM_CHECKEDIN);	
	}

	// update tab array
	if (nImage != m_tabCtrl.GetItemImage(nIndex))
		m_tabCtrl.SetItemImage(nIndex, nImage);

	return nImage;
}

COLORREF CToDoCtrlMgr::GetTabColor(int nIndex) const
{
	if (!m_tabCtrl.IsSupportedFlag(TCE_TABCOLORS))
	{
		ASSERT(0);
		return FALSE;
	}
	
	CHECKVALIDINDEXRET(nIndex, CLR_NONE);
	
	return GetTDCItem(nIndex).crTab;
}

BOOL CToDoCtrlMgr::SetTabColor(int nIndex, COLORREF crTab)
{
	if (!m_tabCtrl.IsSupportedFlag(TCE_TABCOLORS))
	{
		ASSERT(0);
		return FALSE;
	}

	CHECKVALIDINDEXRET(nIndex, FALSE);
	
	GetTDCItem(nIndex).crTab = crTab;
	m_tabCtrl.Invalidate();

	return TRUE;
}

CString CToDoCtrlMgr::GetTabItemTooltip(int nIndex) const
{
	CHECKVALIDINDEXRET(nIndex, _T(""));

	CEnString sTooltip;
	const TDCITEM& tdci = GetTDCItem(nIndex);

	switch (UpdateTabItemImage(nIndex))
	{
	case IM_READONLY:
		if (tdci.pTDC->CompareFileFormat() == TDCFF_NEWER)
			sTooltip.LoadString(IDS_STATUSNEWERFORMAT);
		else
			sTooltip.LoadString(IDS_STATUSREADONLY);
		break;

	case IM_CHECKEDIN:
		sTooltip.LoadString(IDS_STATUSCHECKEDIN);
		break;

	case IM_CHECKEDOUT: 
		sTooltip.LoadString(IDS_STATUSCHECKEDOUT);
		break;

	case IM_NOTLOADED:	
		sTooltip.Format(IDS_TABTIP_NOTLOADED, tdci.pTDC->GetFilePath());
		break;

	case IM_NOTFOUND:
		sTooltip.Format(IDS_TABTIP_NOTFOUND, tdci.pTDC->GetFilePath());
		break;
		
	case IM_TIMETRACKING:	
		sTooltip.LoadString(IDS_TABTIP_TIMETRACKING);
		break;

	case IM_NONE:
		sTooltip = tdci.pTDC->GetFilePath();
		break;
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

int CToDoCtrlMgr::FindPristineToDoCtrl() const
{
	int nCtrl = GetCount();
	
	while (nCtrl--)
	{
		if (IsPristine(nCtrl))
			return nCtrl;
	}

	// not found
	return -1;
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

int CToDoCtrlMgr::GetAllCustomAttributeDefinitions(CTDCCustomAttribDefinitionArray& aAttribDefs) const
{
	aAttribDefs.RemoveAll();
	int nTDC = GetCount();

	while (nTDC--)
		aAttribDefs.Append(GetToDoCtrl(nTDC).GetCustomAttributeDefs());

	return aAttribDefs.GetSize();
}

BOOL CToDoCtrlMgr::AnyHasCustomAttributeFeature(DWORD dwFeature) const
{
	int nTDC = GetCount();

	while (nTDC--)
	{
		if (GetToDoCtrl(nTDC).GetCustomAttributeDefs().AnyHasFeature(dwFeature))
			return TRUE;
	}

	return FALSE;
}

BOOL CToDoCtrlMgr::AnyIsModified() const
{
	int nCtrl = GetCount();
	
	while (nCtrl--)
	{
		if (IsModified(nCtrl))
			return TRUE;
	}
	
	return FALSE;
}

BOOL CToDoCtrlMgr::AnyIsSourceControlled() const
{
	int nCtrl = GetCount();

	while (nCtrl--)
	{
		if (GetToDoCtrl(nCtrl).IsSourceControlled())
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
		// Back up task time log files
		FileMisc::TerminatePath(sBackupFolder);
		sBackupFolder += FileMisc::GetFileNameFromPath(sTDLPath, FALSE);
		
		BackupLogFiles(sTDLPath, sBackupFolder, nKeepBackups);

		return TRUE;
	}

	return FALSE;
}

void CToDoCtrlMgr::BackupLogFiles(const CString& sTDLPath, const CString& sBackupFolder, int nKeepBackups) const
{
	if (Prefs().GetLogTaskTimeSeparately())
	{
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
}

BOOL CToDoCtrlMgr::CreateBackup(const CString& sPath, const CString& sBackupFolder, int nKeepBackups)
{
	// No need to create a backup of non-existent file
	if (!FileMisc::FileExists(sPath))
		return FALSE;

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
		CString sBackupPath = CFileBackup::BuildBackupPath(sPath, FBS_APPVERSION, sBackupFolder, _T(""));
		CString sDrive, sFolder, sFName, sExt;
		
		FileMisc::SplitPath(sBackupPath, &sDrive, &sFolder, &sFName, &sExt);
		FileMisc::MakePath(sBackupPath, sDrive, sFolder);
		
		CStringArray aFiles;
		int nNumFiles = FileMisc::FindFiles(sBackupPath, aFiles, FALSE, sFName + _T("*") + sExt);

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

				nNumFiles = FileMisc::FindFiles(sBackupPath, aFiles, FALSE, sFName + _T("*") + sExt);

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
	return backup.MakeBackup(sPath, FBS_APPVERSION | FBS_DATETIMESTAMP, sBackupFolder, _T(""));
}
