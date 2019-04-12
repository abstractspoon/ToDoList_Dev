// TDCSourceControlHelper.h: interface for the CTDCCustomAttributeHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCSOURCECONTROL_VER2_H__4044B3B7_1EA0_4279_9620_F2035DAE87DF__INCLUDED_)
#define AFX_TDCSOURCECONTROL_VER2_H__4044B3B7_1EA0_4279_9620_F2035DAE87DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

class CToDoCtrl;
class CToDoCtrlData;
class CTaskFile;
class CToDoCtrlDataItems;

struct TODOITEM;

//////////////////////////////////////////////////////////////////////

class CTDCSourceControl  
{
public:
	CTDCSourceControl(const CToDoCtrl& tdc);
	virtual ~CTDCSourceControl();

	BOOL Initialise(const CTaskFile& tasks, BOOL bWantCheckout = FALSE);
	void PrepareTasksForSave(CTaskFile& tasks) const;

	BOOL IsSourceControlled() const;
	BOOL AddTasklistToSourceControl(BOOL bAdd = TRUE);
	BOOL CanAddTasklistToSourceControl(BOOL bAdd = TRUE) const;

	BOOL CheckOutTasklist();
	BOOL CheckOutTasklist(CString& sTasklistCheckedOutTo, BOOL bForce = FALSE);
	BOOL CheckInTasklist();

	BOOL CheckOutTask(DWORD dwTaskID, CString& sTaskCheckedOutTo);
	BOOL CheckInTask(DWORD dwTaskID, CString& sTasklistCheckedOutTo);
	BOOL CheckInAllTasks(CString& sTasklistCheckedOutTo);

	BOOL LoadCheckedOutTask(DWORD dwTaskID, TODOITEM& tdi) const;
	DWORD LoadCheckedOutTask(LPCTSTR szTaskPath, TODOITEM& tdi) const;

	int RestoreCheckedOutTasks(CToDoCtrlDataItems& tdItems) const;
	BOOL SaveCheckedOutTasks();
	BOOL HasCheckedOutTasks() const;
	
	BOOL IsTasklistCheckedOut() const;
	BOOL IsTasklistCheckedOut(CString& sTasklistCheckedOutTo) const;

	BOOL IsTaskCheckedOut(DWORD dwTaskID) const;
	BOOL IsTaskCheckedOut(DWORD dwTaskID, CString& sTaskCheckedOutTo) const;
	BOOL IsTaskCheckedOutToUs(DWORD dwTaskID) const;

	BOOL GetTasklistSourceControlPath(CString& sTasklistSSCPath) const;
	BOOL GetSourceControlFolder(CString& sSSCFolder) const;
	BOOL GetTaskSourceControlPath(DWORD dwTaskID, CString& sTaskSSCPath) const;
	
protected:
	const CToDoCtrl& m_tdc;

	mutable CMap<DWORD, DWORD, COleDateTime, COleDateTime&> m_mapTasksCheckedOut;

	BOOL m_bTasklistCheckedOut;
	BOOL m_bTasklistSourceControlled;
	
protected:
	CString GetSourceControlID() const;
	BOOL GetTasklistPath(CString& sTasklistPath) const;
	BOOL MatchesOurSourceControlID(const CString& sID) const;
	BOOL SaveCheckedOutTask(DWORD dwTaskID, const TODOITEM& tdi) const;
	BOOL SaveCheckedOutTask(const CString& sTaskSSCPath, DWORD dwTaskID, 
							const TODOITEM& tdi, BOOL bPreCheckout) const;

	int DiscardDeletedCheckedOutTasks();

	BOOL HasCheckedOutTask(DWORD dwTaskID) const;
	COleDateTime GetTaskLastSaveTime(DWORD dwTaskID) const;
	BOOL TaskModifiedSinceLastSave(DWORD dwTaskID) const;

	static BOOL CreateSentinelFile(LPCTSTR szPath);
	static BOOL DeleteSentinelFile(LPCTSTR szPath);
	static BOOL SourceControlIDsMatch(const CString& sID1, const CString& sID2);
	static BOOL IsSourceControlled(LPCTSTR szPath);
	static DWORD GetTaskIDFromFilePath(LPCTSTR szPath);
	static BOOL InitialiseSourceControlFolder(LPCTSTR szTasklistPath);
	static BOOL CheckOutTasklist(LPCTSTR szTasklistPath, LPCTSTR szXmlHeader, LPCTSTR szSourceControlID, CString& sTasklistCheckedOutTo, BOOL bForce);
	static BOOL GetSourceControlPath(LPCTSTR szTasklistPath, CString& sTaskSSCPath);
	static BOOL GetSourceControlFolder(LPCTSTR szTasklistPath, CString& sSSCFolder);

	static CString GetCheckedOutTo(LPCTSTR szPath);
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_TDCSOURCECONTROL_VER2_H__4044B3B7_1EA0_4279_9620_F2035DAE87DF__INCLUDED_)
