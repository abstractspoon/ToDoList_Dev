// CToDoCtrlMgr.h : header file
//

#if !defined(AFX_TODOCTRLMGR_H__13051D32_D372_4205_BA71_05FAC2159F1C__INCLUDED_)
#define AFX_TODOCTRLMGR_H__13051D32_D372_4205_BA71_05FAC2159F1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "filteredtodoctrl.h"
#include "preferencesdlg.h"
#include "TDLbrowserdlg.h"

#include "..\shared\filemisc.h"
#include "..\shared\driveinfo.h"

#include "..\Interfaces\taskliststoragemgr.h"

/////////////////////////////////////////////////////////////////////////////

enum TDCM_PATHTYPE { TDCM_UNDEF = -1, TDCM_REMOVABLE, TDCM_FIXED, TDCM_REMOTE, TDCM_OTHER }; // drive types
enum TDCM_DUESTATUS { TDCM_NONE = -1, TDCM_PAST, TDCM_TODAY, TDCM_FUTURE }; 

///////////////////////////////////////////////////////////////////////////////////////////////////

typedef __int64 time64_t;

/////////////////////////////////////////////////////////////////////////////

class CTabCtrlEx;

/////////////////////////////////////////////////////////////////////////////
// CToDoCtrlMgr 
 
class CToDoCtrlMgr
{
	// Construction
public:
	CToDoCtrlMgr(CTabCtrlEx& tabCtrl); 
	~CToDoCtrlMgr();

	inline int GetCount() const { return m_aToDoCtrls.GetSize(); }
	inline int GetSelToDoCtrl() const { return m_tabCtrl.GetCurSel(); }

	void SetPrefs(const CPreferencesDlg* pPrefs);
	BOOL IsPristine() const;

	CFilteredToDoCtrl& GetToDoCtrl(int nIndex);
	const CFilteredToDoCtrl& GetToDoCtrl(int nIndex) const;

	BOOL IsPristine(int nIndex) const;
	int DeleteToDoCtrl(int nIndex); // returns new selection
	int AddToDoCtrl(CFilteredToDoCtrl* pCtrl, const TSM_TASKLISTINFO* pInfo = NULL);
	BOOL IsLoaded(int nIndex) const;
	BOOL FileExists(int nIndex) const;
	void SetLoaded(int nIndex);
	BOOL VerifyPassword(int nIndex, BOOL bForce = FALSE) const;
	BOOL AnyIsModified() const;
	BOOL AnyIsSourceControlled() const;

	int FindToDoCtrl(HWND hwndTDC) const;
	int FindToDoCtrl(const CFilteredToDoCtrl* pTDC) const;
	int FindToDoCtrl(LPCTSTR szFilePath) const;
	int FindToDoCtrl(const TSM_TASKLISTINFO& info) const;
	int FindPristineToDoCtrl() const;
	
	CString GetFileName(int nIndex, BOOL bStrict = TRUE) const;
	CString GetFilePath(int nIndex, BOOL bStrict = TRUE) const;
	CString GetFolderPath(int nIndex) const;
	void ClearFilePath(int nIndex);
	BOOL HasFilePath(int nIndex) const;
	TDCM_PATHTYPE GetFilePathType(int nIndex) const;
	TDCM_PATHTYPE RefreshPathType(int nIndex); 
	CString GetFriendlyProjectName(int nIndex) const;
	CString GetDisplayPath(int nIndex) const;
	CString FormatProjectNameWithFileName(int nIndex) const;
	int GetNextMostSelectableToDoCtrl(int nIndex) const;

	int GetFileNames(CStringArray& sFileNames, BOOL bStrict = TRUE) const;
	int GetFilePaths(CStringArray& sFilePaths, BOOL bStrict = TRUE) const;

	BOOL UsesStorage(int nIndex) const;
	BOOL GetStorageDetails(int nIndex, TSM_TASKLISTINFO& info) const;
	BOOL SetStorageDetails(int nIndex, const TSM_TASKLISTINFO& info);
	BOOL ClearStorageDetails(int nIndex);

	BOOL RefreshFileLastModified(int nIndex); // true if changed
	BOOL IsModified(int nIndex) const;
	int GetElapsedMinutesSinceLastMod(int nIndex) const;

	void RefreshTimeTracking(int nIndex);

	BOOL GetReadOnlyStatus(int nIndex) const;
	BOOL RefreshReadOnlyStatus(int nIndex); // true if changed
	void UpdateToDoCtrlReadOnlyUIState(int nIndex);
	void UpdateToDoCtrlReadOnlyUIState(CFilteredToDoCtrl& tdc);
	void CheckNotifyReadonly(int nIndex) const;

	void SetDueItemStatus(int nIndex, TDCM_DUESTATUS nStatus);
	TDCM_DUESTATUS GetDueItemStatus(int nIndex) const;
	BOOL ShowDueTaskNotification(int nIndex, LPCTSTR szFilePath, BOOL bBrowser);
	BOOL RefreshDueItemStatus(); // true if changed
	BOOL RefreshDueItemStatus(int nIndex); // true if changed

	BOOL GetLastCheckoutSucceeded(int nIndex) const;
	void SetLastCheckoutSucceeded(int nIndex, BOOL bSuccess);
	void RefreshLastCheckoutStatus(int nIndex);
	BOOL CanCheckOut(int nIndex) const;
	BOOL CanCheckIn(int nIndex) const;
	BOOL CanCheckInOut(int nIndex) const;
	BOOL IsSourceControlled(int nIndex) const;
	BOOL AddToSourceControl(int nIndex, BOOL bAdd = TRUE);
	BOOL CanAddToSourceControl(int nIndex, BOOL bAdd = TRUE) const;
	BOOL IsCheckedOut(int nIndex) const;
	TDC_FILE CheckOut(int nIndex);
	TDC_FILE CheckOut(int nIndex, CString& sCheckedOutTo, BOOL bForce = FALSE);
	TDC_FILE CheckIn(int nIndex);
	
	void MoveToDoCtrl(int nIndex, int nNumPlaces);
	BOOL CanMoveToDoCtrl(int nIndex, int nNumPlaces) const;

	int SortToDoCtrlsByName();
	void GetAutoListData(TDCAUTOLISTDATA& tldActive, TDCAUTOLISTDATA& tldAll, TDC_ATTRIBUTE nAttribID) const;

	CString UpdateTabItemText(int nIndex);
	CString GetTabItemText(int nIndex) const;
	CString GetTabItemTooltip(int nIndex) const;
	COLORREF GetTabColor(int nIndex) const;
	BOOL SetTabColor(int nIndex, COLORREF crTab);

	BOOL ArchiveDoneTasks(int nIndex);
	BOOL ArchiveSelectedTasks(int nIndex);
	CString GetArchivePath(int nIndex) const;

	void SetNeedsPreferenceUpdate(int nIndex, BOOL bNeed);
	BOOL GetNeedsPreferenceUpdate(int nIndex) const;
	void SetAllNeedPreferenceUpdate(BOOL bNeed, int nExcept = -1);

	BOOL DoBackup(int nIndex) const;
	void PreparePopupMenu(CMenu& menu, UINT nID1, int nMax = 20) const;

	int GetAllCustomAttributeDefinitions(CTDCCustomAttribDefinitionArray& aAttribDefs) const;
	BOOL AnyHasCustomAttributeFeature(DWORD dwFeature) const;

	// Implementation
protected:
	struct TDCITEM
	{
	public:
		TDCITEM();
		TDCITEM(CFilteredToDoCtrl* pCtrl, const TSM_TASKLISTINFO* pInfo = NULL);
		virtual ~TDCITEM();

		TDCM_PATHTYPE GetPathType() const;
		CString GetFriendlyProjectName() const;
		BOOL IsSelectable() const;
		BOOL HasFilePath() const;

		BOOL UsesStorage() const;
		void RefreshPathType();
		void ClearStorageDetails();
		void SetStorageDetails(const TSM_TASKLISTINFO& info);
		
		static TDCM_PATHTYPE TranslatePathType(int nDriveInfoType);
	
	public:
		CFilteredToDoCtrl* pTDC;
		BOOL bLastStatusReadOnly;
		time64_t tLastMod;
		BOOL bLastCheckoutSuccess;
		TDCM_PATHTYPE nPathType;
		TDCM_DUESTATUS nDueStatus;
		BOOL bNeedPrefUpdate;
		int nUntitledIndex; // creation index regardless of actual position
		BOOL bLoaded;
		COLORREF crTab;

		TSM_TASKLISTINFO storageinfo;
	};
	
	CArray<TDCITEM, TDCITEM&> m_aToDoCtrls;
	CTabCtrlEx& m_tabCtrl;
	const CPreferencesDlg* m_pPrefs;
	CMap<CFilteredToDoCtrl*, CFilteredToDoCtrl*, CTDLBrowserDlg*, CTDLBrowserDlg*> m_mapBrowsers;

protected:
	TDCITEM& GetTDCItem(int nIndex);
	const TDCITEM& GetTDCItem(int nIndex) const;
	const CPreferencesDlg& Prefs() const;

	// sort functions
	static int NameSortProc(const void* v1, const void* v2);
	static int SelectabilitySortProc(const void* v1, const void* v2);
	BOOL AreToDoCtrlsSortedByName() const;

	int UpdateTabItemImage(int nIndex) const;
	void BackupLogFiles(const CString& sTDLPath, const CString& sBackupFolder, int nKeepBackups) const;

	static BOOL CreateBackup(const CString& sPath, const CString& sBackupFolder, int nKeepBackups);
	static CFilteredToDoCtrl& GetFallbackToDoCtrl();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TODOCTRLMGR_H__13051D32_D372_4205_BA71_05FAC2159F1C__INCLUDED_)
