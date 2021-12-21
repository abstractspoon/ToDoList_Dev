// NetworkDiagramStruct.h: interface for the NetworkDiagramStruct classes.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETWORKDIAGRAMSTRUCT_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_)
#define AFX_NETWORKDIAGRAMSTRUCT_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetworkDiagramenum.h"

#include "..\Shared\DateHelper.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

struct NETWORKITEM 
{ 
	NETWORKITEM();
	NETWORKITEM(const NETWORKITEM& gi);
	virtual ~NETWORKITEM();
	
	NETWORKITEM& operator=(const NETWORKITEM& gi);
	BOOL operator==(const NETWORKITEM& gi) const;
	BOOL operator!=(const NETWORKITEM& gi) const;
	
	CString sTitle;
	COleDateTime dtDone; 
	COLORREF color;
	CString sAllocTo;
	BOOL bParent;
	DWORD dwTaskID, dwRefID, dwOrgRefID;
	CDWordArray aDependIDs;
	CStringArray aTags;
	int nPercent;
	BOOL bGoodAsDone, bSomeSubtaskDone;
	int nPosition;
	BOOL bLocked, bHasIcon;
	
 	BOOL IsDone(BOOL bIncGoodAs) const;
// 	BOOL IsMilestone(const CString& sMilestoneTag) const;
// 
// 	BOOL HasStartDate() const;
// 	BOOL HasDueDate() const;
// 	BOOL HasDoneDate(BOOL bCalcParentDates) const;
	BOOL HasColor() const;
// 
// 	void SetStartDate(time64_t tDate, BOOL bAndMinMax = FALSE);
// 	void SetDueDate(time64_t tDate, BOOL bAndMinMax = FALSE);
// 	void SetStartDate(const COleDateTime& date, BOOL bAndMinMax = FALSE);
// 	void SetDueDate(const COleDateTime& date, BOOL bAndMinMax = FALSE);
// 	void SetDoneDate(time64_t tDate);
// 	
// 	void ClearStartDate(BOOL bAndMinMax = FALSE);
// 	void ClearDueDate(BOOL bAndMinMax = FALSE);
// 	void ClearDoneDate();
// 
// 	BOOL GetStartEndDates(BOOL bCalcParentDates, BOOL bCalcMissingStart, BOOL bCalcMissingDue, COleDateTime& dtStart, COleDateTime& dtDue) const;
// 	void MinMaxDates(const NETWORKITEM& giOther, BOOL bCalcParentDates, BOOL bCalcMissingStart, BOOL bCalcMissingDue);
	
	COLORREF GetTextColor(BOOL bSelected, BOOL bColorIsBkgnd) const;
	COLORREF GetTextBkColor(BOOL bSelected, BOOL bColorIsBkgnd) const;
	COLORREF GetFillColor() const;
	COLORREF GetBorderColor() const;

	static COleDateTime GetDate(time64_t tDate, BOOL bEndOfDay);

};

/////////////////////////////////////////////////////////////////////////////

class CNetworkItemMap : public CMap<DWORD, DWORD, NETWORKITEM*, NETWORKITEM*&>
{
public:
	virtual ~CNetworkItemMap();

	void RemoveAll();
	BOOL DeleteItem(DWORD dwTaskID);
	BOOL HasItem(DWORD dwTaskID) const;
	NETWORKITEM* GetItem(DWORD dwTaskID, BOOL bResolveReferences) const;
	BOOL RestoreItem(const NETWORKITEM& giPrev);

	BOOL ItemIsLocked(DWORD dwTaskID, BOOL bTreatRefsAsUnlocked) const;
	BOOL ItemIsReference(DWORD dwTaskID) const;
	BOOL ItemIsDone(DWORD dwTaskID, BOOL bIncGoodAs) const;
	BOOL ItemHasDependecies(DWORD dwTaskID) const;
	BOOL IsItemDependentOn(const NETWORKITEM& gi, DWORD dwOtherID) const;

	int BuildDependencyChainLengths(CMap<DWORD, DWORD, int, int>& mapLengths) const;
	int CalcMaxDependencyChainLength(DWORD dwTaskID) const;

protected:
};

/////////////////////////////////////////////////////////////////////////////

struct NETWORKDEPENDENCY
{
	NETWORKDEPENDENCY();

	void SetFrom(const CPoint& pt, DWORD dwTaskID = 0);
	void SetTo(const CPoint& pt, DWORD dwTaskID = 0);

	DWORD GetFromID() const;
	DWORD GetToID() const;

	BOOL Matches(DWORD dwFrom, DWORD dwTo) const;
	
	BOOL HitTest(const CRect& rect) const;
	BOOL HitTest(const CPoint& point, int nTol = 2) const;

	BOOL Draw(CDC* pDC, const CRect& rClient, BOOL bDragging);

#ifdef _DEBUG
	void Trace() const;
#endif
	
	static int STUB;
	
protected:
	CPoint ptFrom, ptTo;
	DWORD dwFromID, dwToID;
	
protected:
	void CalcDependencyPath(CPoint pts[3]) const;
	BOOL CalcBoundingRect(CRect& rect) const;
	void CalcDependencyArrow(const CPoint& pt, CPoint pts[3]) const;
	void DrawDependencyArrow(CDC* pDC, const CPoint& pt) const;
	BOOL IsFromAboveTo() const;
};

typedef CArray<NETWORKDEPENDENCY, NETWORKDEPENDENCY&> CNetworkDependArray;

/////////////////////////////////////////////////////////////////////////////

class INetworkDependencyEditor
{
public:
	virtual BOOL SetFromTask(DWORD dwFromTaskID) = 0;
	virtual BOOL SetFromDependency(DWORD dwFromTaskID, DWORD dwCurToTaskID) = 0;
	virtual BOOL SetToTask(DWORD dwToTaskID) = 0;

	virtual DWORD GetFromTask() const = 0;
	virtual DWORD GetFromDependency(DWORD& dwCurToTaskID) const = 0;
	virtual DWORD GetToTask() const = 0;

	virtual BOOL IsPicking() const = 0;
	virtual BOOL IsPickingFromTask() const = 0;
	virtual BOOL IsPickingFromDependency() const = 0;
	virtual BOOL IsPickingToTask() const = 0;
	virtual BOOL IsPickingCancelled() const = 0;
	virtual BOOL IsPickingCompleted() const = 0;

	virtual BOOL IsAdding() const = 0;
	virtual BOOL IsEditing() const = 0;
	virtual BOOL IsDeleting() const = 0;

	virtual void Cancel() = 0;
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_NETWORKDIAGRAMSTRUCT_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_)
