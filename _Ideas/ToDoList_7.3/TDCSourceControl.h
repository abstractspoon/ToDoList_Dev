// TDCSourceControlHelper.h: interface for the CTDCCustomAttributeHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCSOURCECONTROLHELPER_H__4044B3B7_1EA0_4279_9620_F2035DAE87DF__INCLUDED_)
#define AFX_TDCSOURCECONTROLHELPER_H__4044B3B7_1EA0_4279_9620_F2035DAE87DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\shared\mapex.h"

//////////////////////////////////////////////////////////////////////

class CToDoCtrl;
class CToDoCtrlData;
class CTaskFile;
class CToDoCtrlDataItems;

struct TODOITEM;

//////////////////////////////////////////////////////////////////////

/*

class CToDoCtrl
{
	friend class CTDCSourceControl;

	...



protected:
	CTDCSourceControl m_scc;
};

*/

class CTDCSourceControl  
{
public:
	CTDCSourceControl(const CToDoCtrl& tdc);
	virtual ~CTDCSourceControl();

	BOOL CheckOutTasklist(CTaskFile& tasks, CString& sTasklistCheckedOutTo);
	BOOL CheckInTasklist();

	BOOL CheckOutTask(DWORD dwTaskID, const TODOITEM& tdi, CString& sTaskCheckedOutTo);
	BOOL CheckInTask(DWORD dwTaskID, TODOITEM& tdi, CString& sTasklistCheckedOutTo);
	BOOL CheckInAllTasks(CString& sTasklistCheckedOutTo);

	BOOL LoadCheckedOutTask(DWORD dwTaskID, TODOITEM& tdi) const;
	DWORD LoadCheckedOutTask(LPCTSTR szTaskPath, TODOITEM& tdi) const;

	int RestoreCheckedOutTasks(CToDoCtrlDataItems& tdItems);
	BOOL SaveCheckedOutTasks();
	
	BOOL IsTasklistCheckedOut() const;
	BOOL IsTasklistCheckedOut(CString& sTasklistCheckedOutTo) const;
	BOOL IsTasklistCheckedOutToUs() const;
	
	BOOL IsTaskCheckedOut(DWORD dwTaskID) const;
	BOOL IsTaskCheckedOut(DWORD dwTaskID, CString& sTaskCheckedOutTo) const;
	BOOL IsTaskCheckedOutToUs(DWORD dwTaskID) const;

	BOOL GetTasklistSourceControlPath(CString& sTasklistSCCPath) const;
	BOOL GetTaskSourceControlFolder(CString& sSCCFolder) const;
	BOOL GetTaskSourceControlPath(DWORD dwTaskID, CString& sTaskSCCPath) const;

protected:
	const CToDoCtrl& m_tdc;

	CDWordSet m_mapTasksCheckedOut;
	BOOL m_bTasklistCheckedOut;

protected:
	BOOL GetTasklistPath(CString& sTasklistPath) const;
	CString GetSourceControlID(BOOL bAlternate) const;
	BOOL MatchesOurSourceControlID(const CString& sID) const;
	BOOL SaveCheckedOutTask(DWORD dwTaskID, const TODOITEM& tdi) const;
	BOOL SaveCheckedOutTask(const CString& sTaskSCCPath, DWORD dwTaskID, const TODOITEM& tdi) const;

	int DiscardDeletedCheckedOutTasks();

	static BOOL CreateSentinelFile(LPCTSTR szPath);
	static BOOL DeleteSentinelFile(LPCTSTR szPath);
	static BOOL SourceControlIDsMatch(const CString& sID1, const CString& sID2);
	static CString GetCheckedOutTo(LPCTSTR szPath);
	static DWORD GetTaskIDFromFilePath(LPCTSTR szPath);
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_TDCSOURCECONTROLHELPER_H__4044B3B7_1EA0_4279_9620_F2035DAE87DF__INCLUDED_)
