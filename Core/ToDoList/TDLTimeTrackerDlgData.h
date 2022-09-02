#pragma once
#include "afxwin.h"

#include <AFXTEMPL.H>

/////////////////////////////////////////////////////////////////////////

class CFilteredToDoCtrl;
class CTaskFile;

typedef void* HTASKITEM;

/////////////////////////////////////////////////////////////////////////

enum
{
	TTDO_FORMATTIMESASHMS = 0X01,
	TTDO_SHOWONBEGINTRACKING = 0X02,
	TTDO_ALLOWPARENTTRACKING = 0X04,
	TTDO_SHOWTASKPATH = 0X08,
};

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
	BOOL UpdateTasks(const CTaskFile& tasks);
	BOOL RemoveTasks(DWORD dwToRemove);

	BOOL IsTracking(DWORD dwTaskID = 0) const;
	DWORD GetTrackedTaskID() const { return dwTrackedTaskID; }
	BOOL UpdateTracking();

	const CFilteredToDoCtrl* pTDC;
	CTrackItemArray aTasks;
	CDWordArray aRecentlyTrackedIDs;

protected:
	DWORD dwTrackedTaskID;
	BOOL bTrackingPaused;

protected:
	BOOL UpdateTasks(const CTaskFile& tasks, HTASKITEM hTask, int nLevel, const CMapTaskIndex& mapTasks);
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

