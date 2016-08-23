// CToDoCtrlMgr.h : header file
//

#if !defined(AFX_TODOCTRLMGR_H__13051D32_D372_4205_BA71_05FAC2159F1C__INCLUDED_)
#define AFX_TODOCTRLMGR_H__13051D32_D372_4205_BA71_05FAC2159F1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "filteredtodoctrl.h"
#include "preferencesdlg.h"

#include "..\shared\filemisc.h"
#include "..\shared\driveinfo.h"
#include "..\shared\taskliststoragemgr.h"
#include "..\shared\browserdlg.h"

/////////////////////////////////////////////////////////////////////////////
// CToDoCtrlMgr dialog
 
enum TDCM_PATHTYPE { TDCM_UNDEF = -1, TDCM_REMOVABLE, TDCM_FIXED, TDCM_REMOTE, TDCM_OTHER }; // drive types
enum TDCM_DUESTATUS { TDCM_NONE = -1, TDCM_PAST, TDCM_TODAY, TDCM_FUTURE }; 

class CPreferences;
class CTabCtrlEx;

class CToDoCtrlMgr
{
	// Construction
public:
	CToDoCtrlMgr(CTabCtrlEx& tabCtrl); 
	~CToDoCtrlMgr();

	inline int GetCount() const { return m_aToDoCtrls.GetSize(); }
	inline int GetSelToDoCtrl() const { return m_tabCtrl.GetCurSel(); }

	void SetPrefs(const CPreferencesDlg* pPrefs);

	CFilteredToDoCtrl& GetToDoCtrl(int nIndex);
	const CFilteredToDoCtrl& GetToDoCtrl(int nIndex) const;

	int RemoveToDoCtrl(int nIndex, BOOL bDelete = FALSE); // returns new selection
	int AddToDoCtrl(CFilteredToDoCtrl* pCtrl, const TSM_TASKLISTINFO* pInfo = NULL, BOOL bLoaded = TRUE);
	BOOL IsPristine(int nIndex) const;
	BOOL IsLoaded(int nIndex) const;
	void SetLoaded(int nIndex, BOOL bLoaded = TRUE);
	BOOL VerifyPassword(int nIndex) const;

	BOOL HasTasks(int nIndex) const;
	BOOL AnyHasTasks() const;
	BOOL AnyIsModified() const;

	int FindToDoCtrl(HWND hwndTDC) const;
	int FindToDoCtrl(const CFilteredToDoCtrl* pTDC) const;
	int FindToDoCtrl(LPCTSTR szFilePath) const;
	int FindToDoCtrl(const TSM_TASKLISTINFO& info) const;

	CString GetFileName(int nIndex, BOOL bStrict = TRUE) const;
	CString GetFilePath(int nIndex, BOOL bStrict = TRUE) const;
	CString GetFolderPath(int nIndex) const;
	void ClearFilePath(int nIndex);
	BOOL HasFilePath(int nIndex) const;
	TDCM_PATHTYPE GetFilePathType(int nIndex) const;
	TDCM_PATHTYPE RefreshPathType(int nIndex); 
	CString GetFriendlyProjectName(int nIndex) const;
	CString GetDisplayPath(int nIndex) const;
	BOOL GetFileNameAndExt(int nIndex, CString& sFileName, CString& sExt, BOOL bStrict = TRUE) const;
	CString FormatProjectNameWithFileName(int nIndex) const;

	BOOL UsesStorage(int nIndex) const;
	BOOL GetStorageDetails(int nIndex, TSM_TASKLISTINFO& info) const;
	BOOL SetStorageDetails(int nIndex, const TSM_TASKLISTINFO& info);
	BOOL ClearStorageDetails(int nIndex);

	BOOL RefreshFileLastModified(int nIndex); // true if changed
	BOOL IsModified(int nIndex) const;
	int GetElapsedMinutesSinceLastMod(int nIndex) const;

	void RefreshTimeTracking(int nIndex);

	int GetReadOnlyStatus(int nIndex) const;
	BOOL RefreshReadOnlyStatus(int nIndex); // true if changed
	void UpdateToDoCtrlReadOnlyUIState(int nIndex);
	void UpdateToDoCtrlReadOnlyUIState(CFilteredToDoCtrl& tdc);
	void CheckNotifyReadonly(int nIndex) const;

	void SetDueItemStatus(int nIndex, TDCM_DUESTATUS nStatus);
	TDCM_DUESTATUS GetDueItemStatus(int nIndex) const;
	BOOL ShowDueTaskNotification(int nIndex, LPCTSTR szFilePath, BOOL bBrowser);

	int GetLastCheckoutSucceeded(int nIndex) const;
	void SetLastCheckoutSucceeded(int nIndex, BOOL bSuccess);
	void RefreshLastCheckoutStatus(int nIndex);
	int CanCheckOut(int nIndex) const;
	int CanCheckIn(int nIndex) const;
	int CanCheckInOut(int nIndex) const;
	BOOL IsSourceControlled(int nIndex) const;
	BOOL AddToSourceControl(int nIndex, BOOL bAdd = TRUE);
	BOOL IsCheckedOut(int nIndex) const;
	TDC_FILE CheckOut(int nIndex);
	TDC_FILE CheckOut(int nIndex, CString& sCheckedOutTo);
	TDC_FILE CheckIn(int nIndex);
	
	void MoveToDoCtrl(int nIndex, int nNumPlaces);
	BOOL CanMoveToDoCtrl(int nIndex, int nNumPlaces) const;

	int SortToDoCtrlsByName();
	void GetAutoListData(TDCAUTOLISTDATA& tldActive, TDCAUTOLISTDATA& tldAll) const;

	CString UpdateTabItemText(int nIndex);
	CString GetTabItemText(int nIndex) const;
	CString GetTabItemTooltip(int nIndex) const;

	BOOL ArchiveDoneTasks(int nIndex);
	BOOL ArchiveSelectedTasks(int nIndex);
	CString GetArchivePath(int nIndex) const;

	void SetNeedsPreferenceUpdate(int nIndex, BOOL bNeed);
	BOOL GetNeedsPreferenceUpdate(int nIndex) const;
	void SetAllNeedPreferenceUpdate(BOOL bNeed, int nExcept = -1);

	void PreparePopupMenu(CMenu& menu, UINT nID1, int nMax = 20) const;
	COleDateTime GetMostRecentEdit() const;

	BOOL DoBackup(int nIndex) const;

	// Implementation
protected:
	struct TDCITEM
	{
		TDCITEM() 
		{ 
			pTDC = NULL; 
			bModified = FALSE; 
			bLastStatusReadOnly = -1; 
			tLastMod = 0; 
			bLastCheckoutSuccess = -1;
            nPathType = TDCM_UNDEF;
			nDueStatus = TDCM_NONE;
			bNeedPrefUpdate = TRUE;
			nUntitled = -1;
			bLoaded = TRUE;
		}
		
		TDCITEM(CFilteredToDoCtrl* pCtrl, BOOL loaded, const TSM_TASKLISTINFO* pInfo = NULL) 
		{ 
			static int nNextUntitledIndex = 0;
			nUntitled = (pCtrl->GetFilePath().IsEmpty() ? nNextUntitledIndex++ : -1);
			
			pTDC = pCtrl; 
			bLoaded = loaded;
			bModified = FALSE; 
			bLastStatusReadOnly = -1;
			tLastMod = 0;
			bLastCheckoutSuccess = -1;
			nDueStatus = TDCM_NONE;
			bNeedPrefUpdate = TRUE;

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

				if (pCtrl->IsCheckedOut())
					bLastCheckoutSuccess = TRUE;
			}
			
            RefreshPathType();
		}

		virtual ~TDCITEM()
		{
		}
		
		CFilteredToDoCtrl* pTDC;
		BOOL bModified;
		BOOL bLastStatusReadOnly;
		time_t tLastMod;
		BOOL bLastCheckoutSuccess;
        TDCM_PATHTYPE nPathType;
		TDCM_DUESTATUS nDueStatus;
		BOOL bNeedPrefUpdate;
		int nUntitled; // creation index regardless of actual position
		BOOL bLoaded;

		TSM_TASKLISTINFO storageinfo;
		
		inline TDCM_PATHTYPE GetPathType() const { return nPathType; }
        
		BOOL UsesStorage() const { return !storageinfo.sStorageID.IsEmpty(); }

        void RefreshPathType() 
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
		
		void ClearStorageDetails()
		{
			storageinfo.Reset();
			pTDC->SetAlternatePreferencesKey(_T(""));
		}

		void SetStorageDetails(const TSM_TASKLISTINFO& info)
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

		CString GetFriendlyProjectName() const 
		{ 
 			if (UsesStorage() && pTDC->GetProjectName().IsEmpty())
 				return storageinfo.szDisplayName;

			// else
			return pTDC->GetFriendlyProjectName(nUntitled); 
		}

		static TDCM_PATHTYPE TranslatePathType(int nDriveInfoType)
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
	};
	
	CArray<TDCITEM, TDCITEM&> m_aToDoCtrls;
	CTabCtrlEx& m_tabCtrl;
	const CPreferencesDlg* m_pPrefs;
	CMap<CFilteredToDoCtrl*, CFilteredToDoCtrl*, CBrowserDlg*, CBrowserDlg*> m_mapBrowsers;

protected:
	TDCITEM& GetTDCItem(int nIndex);
	const TDCITEM& GetTDCItem(int nIndex) const;

	// sort function
	static int SortProc(const void* v1, const void* v2);
	BOOL AreToDoCtrlsSorted() const;

	BOOL PathTypeSupportsSourceControl(TDCM_PATHTYPE nType) const;
	const CPreferencesDlg& Prefs() const;

	int UpdateTabItemImage(int nIndex) const;

	static BOOL CreateBackup(const CString& sPath, const CString& sBackupFolder, int nKeepBackups);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TODOCTRLMGR_H__13051D32_D372_4205_BA71_05FAC2159F1C__INCLUDED_)
