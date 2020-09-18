#pragma once
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////

class CFilteredToDoCtrl;
class CTaskFile;

typedef void* HTASKITEM;

/////////////////////////////////////////////////////////////////////////

struct TRACKITEM
{
	TRACKITEM();

	BOOL operator==(const TRACKITEM& ti) const;
	BOOL operator!=(const TRACKITEM& ti) const;
	CString FormatTaskTitle(BOOL bWantPath) const;
	
	CString sTaskTitle;
	CString sTaskPath;
	DWORD dwTaskID;
	BOOL bParent;
};
typedef CArray<TRACKITEM, TRACKITEM&> CTrackItemArray;

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
	BOOL UpdateTasks(const CTaskFile& tasks);
	BOOL RemoveTasks(DWORD dwToRemove);

	BOOL IsTracking(DWORD dwTaskID = 0) const;

	const CFilteredToDoCtrl* pTDC;
	DWORD dwTrackedTaskID;
	BOOL bTrackingPaused;
	CTrackItemArray* pTasks;

protected:
	typedef CMap<DWORD, DWORD, int, int> CMapTaskIndex;
	void BuildTaskMap(CMapTaskIndex& mapTasks) const;
	BOOL UpdateTasks(const CTaskFile& tasks, HTASKITEM hTask, const CString& sParentPath, const CMapTaskIndex& mapTasks);
};

/////////////////////////////////////////////////////////////////////////

class CTDCTrackTasklistArray : protected CArray<TRACKTASKLIST*, TRACKTASKLIST*&>
{
public:
	CTDCTrackTasklistArray();
	virtual ~CTDCTrackTasklistArray();

	BOOL IsEmpty() const { return !GetNumTasklists(); }
	int GetNumTasklists() const;
	BOOL UpdateTracking(const CFilteredToDoCtrl* pTDC);
	BOOL UpdateTracking(TRACKTASKLIST* pTTL);

	int AddTasklist(const CFilteredToDoCtrl* pTDC, const CTaskFile& tasks);
	BOOL DeleteTasklist(const CFilteredToDoCtrl* pTDC);
	void DeleteAllTasklists();
	int FindTasklist(const CFilteredToDoCtrl* pTDC) const;

	const TRACKTASKLIST* GetTasklist(const CFilteredToDoCtrl* pTDC) const;
	TRACKTASKLIST* GetTasklist(const CFilteredToDoCtrl* pTDC);
	const TRACKTASKLIST* GetTasklist(int nTasklist) const;

	const CTrackItemArray* GetTasks(const CFilteredToDoCtrl* pTDC) const;
	CTrackItemArray* GetTasks(const CFilteredToDoCtrl* pTDC);

};

