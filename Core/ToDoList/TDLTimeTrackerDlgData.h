#pragma once
#include "afxwin.h"

#include <AFXTEMPL.H>

/////////////////////////////////////////////////////////////////////////

class CToDoCtrl;
class CTaskFile;

typedef void* HTASKITEM;

/////////////////////////////////////////////////////////////////////////

struct TRACKITEM
{
	TRACKITEM();

	BOOL operator==(const TRACKITEM& ti) const;
	BOOL operator!=(const TRACKITEM& ti) const;
	
	CString sTaskTitle;
	DWORD dwTaskID;
	BOOL bParent;
	int nLevel;
	CString sImage;
};

/////////////////////////////////////////////////////////////////////////

typedef CMap<DWORD, DWORD, int, int> CMapTaskIndex;

class CTrackItemArray : public CArray<TRACKITEM, TRACKITEM&>
{
public:
	void BuildTaskMap(CMapTaskIndex& mapTasks) const;
};

/////////////////////////////////////////////////////////////////////////

enum 
{
	TTL_REMOVEDELETED	= 0x01,
	TTL_REMOVEDONE		= 0x02,
};

struct TRACKTASKLIST
{
	TRACKTASKLIST();
	virtual ~TRACKTASKLIST();

	int SetTasks(const CTaskFile& tasks);
	BOOL UpdateTasks(const CTaskFile& tasks, CDWordArray& aModTaskIDs);
	BOOL RemoveTasks(DWORD dwToRemove);

	BOOL IsTracking(DWORD dwTaskID = 0) const;
	DWORD GetTrackedTaskID() const { return dwTrackedTaskID; }
	BOOL UpdateTracking();

	const CToDoCtrl* pTDC;
	CTrackItemArray aTasks;
	CDWordArray aRecentlyTrackedIDs;

	BOOL bNeedFullTaskUpdate;
	BOOL bNeedComboRebuild;

protected:
	DWORD dwTrackedTaskID;
	BOOL bTrackingPaused;

protected:
	int UpdateTasks(const CTaskFile& tasks, HTASKITEM hTask, int nLevel, const CMapTaskIndex& mapTasks, CDWordArray& aModTaskIDs);
};

/////////////////////////////////////////////////////////////////////////

class CTDCTrackTasklistArray : protected CArray<TRACKTASKLIST*, TRACKTASKLIST*&>
{
public:
	CTDCTrackTasklistArray();
	virtual ~CTDCTrackTasklistArray();

	BOOL IsEmpty() const { return !GetNumTasklists(); }
	int GetNumTasklists() const;
	BOOL UpdateTracking(const CToDoCtrl* pTDC);

	int AddTasklist(const CToDoCtrl* pTDC, const CTaskFile& tasks);
	BOOL DeleteTasklist(const CToDoCtrl* pTDC);
	void DeleteAllTasklists();
	int FindTasklist(const CToDoCtrl* pTDC) const;

	const TRACKTASKLIST* GetTasklist(const CToDoCtrl* pTDC) const;
	TRACKTASKLIST* GetTasklist(const CToDoCtrl* pTDC);
	const TRACKTASKLIST* GetTasklist(int nTasklist) const;

	const CTrackItemArray* GetTasks(const CToDoCtrl* pTDC) const;
	CTrackItemArray* GetTasks(const CToDoCtrl* pTDC);

};

