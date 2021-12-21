// NetworkDiagramCtrl.h: interface for the CNetworkDiagramCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETWORKDIAGRAMCTRL_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)
#define AFX_NETWORKDIAGRAMCTRL_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetworkDiagramstruct.h"

#include "..\shared\EnListCtrl.h"
#include "..\shared\mapex.h"
#include "..\shared\GraphicsMisc.h"

#include "..\Interfaces\itasklist.h"
#include "..\Interfaces\iuiextension.h"

/////////////////////////////////////////////////////////////////////////////

#define TVN_KEYUP (TVN_FIRST-16)

/////////////////////////////////////////////////////////////////////////////

class CThemed;

struct UITHEME;

/////////////////////////////////////////////////////////////////////////////

class CNetworkDiagramCtrl : public CEnListCtrl  
{
public:
	CNetworkDiagramCtrl();
	virtual ~CNetworkDiagramCtrl();

	BOOL SaveToImage(CBitmap& bmImage);
	BOOL SetFont(HFONT hFont, BOOL bRedraw = TRUE);

	void UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate);
	bool PrepareNewTask(ITaskList* pTask) const;
	BOOL CancelOperation();

	DWORD GetSelectedTaskID() const;
	BOOL SelectTask(DWORD dwTaskID);
	BOOL SelectTask(IUI_APPCOMMAND nCmd, const IUISELECTTASK& select);
	BOOL GetSelectedTaskDates(COleDateTime& dtStart, COleDateTime& dtDue) const;
	DWORD GetNextTask(DWORD dwTaskID, IUI_APPCOMMAND nCmd) const;
	int GetTaskCount() const { return GetItemCount(); }

	BOOL CanMoveSelectedTask(const IUITASKMOVE& move) const;
	BOOL MoveSelectedTask(const IUITASKMOVE& move);

	BOOL GetSelectedTaskDependencies(CDWordArray& aDepends) const;
	BOOL AddSelectedTaskDependency(DWORD dwDependID);
	BOOL EditSelectedTaskDependency(DWORD dwFromDependID, DWORD dwToDependID);
	BOOL DeleteSelectedTaskDependency(DWORD dwDependID);

	DWORD HitTestTask(const CPoint& ptScreen, bool bTitleColumnOnly) const;

	void ScrollToSelectedTask();
	void ScrollToTask(DWORD dwTaskID);

	void SetOption(DWORD dwOption, BOOL bSet = TRUE);
	BOOL HasOption(DWORD dwOption) const { return ((m_dwOptions & dwOption) ? TRUE : FALSE); }

	void SetUITheme(const UITHEME& theme);
	void SetDefaultBarColor(COLORREF crDefault);
//	void SetMilestoneTag(const CString& sTag);
	void SetReadOnly(bool bReadOnly);

	BOOL BeginDependencyEdit(INetworkDependencyEditor* pDependEdit);
	void OnEndDepedencyEdit();

	static BOOL WantEditUpdate(TDC_ATTRIBUTE nAttrib);
	static BOOL WantSortUpdate(TDC_ATTRIBUTE nAttrib);

protected:
	NETWORKITEM m_niPreDrag;
	INetworkDependencyEditor* m_pDependEdit;

	COLORREF m_crParent, m_crBarDefault;
	CPoint m_ptDragStart, m_ptLastDependPick;
	DWORD m_dwOptions;
	DWORD m_dwMaxTaskID;
//	CString m_sMilestoneTag;

	CNetworkItemMap m_data;
	CMap<DWORD, DWORD, int, int> m_mapDependencyChainLengths;

protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
	DECLARE_MESSAGE_MAP()

protected:
	BOOL OnListLButtonDown(UINT nFlags, CPoint point);
	BOOL OnListLButtonUp(UINT nFlags, CPoint point);
	BOOL OnListLButtonDblClk(UINT nFlags, CPoint point);
	BOOL OnListMouseMove(UINT nFlags, CPoint point);

	void ClearDependencyPickLine(CDC* pDC = NULL);
	BOOL DrawDependencyPickLine(const CPoint& ptClient);
	void EndDependencyEdit();

	void BuildListColumns();
	void UpdateListColumns();

	NETWORKITEM* GetNetworkItem(DWORD dwTaskID) const;
	BOOL RestoreNetworkItem(const NETWORKITEM& niPrev);

//	DWORD HitTestTask(const CPoint& point, BOOL bScreen/*, GTLC_HITTEST& nHit, BOOL bDragging*/) const;
	DWORD DependsHitTest(const CPoint& ptClient, DWORD& dwToTaskID);
	DWORD GetTaskID(int nItem) const;

	BOOL CanDragTask(DWORD dwTaskID) const;
	BOOL StartDragging(const CPoint& ptCursor);
	BOOL EndDragging(const CPoint& ptCursor);
	BOOL UpdateDragging(const CPoint& ptCursor);
	BOOL ValidateDragPoint(CPoint& ptDrag) const;
	BOOL IsValidDragPoint(const CPoint& ptDrag) const;
	void CancelDrag(BOOL bReleaseCapture);
	BOOL IsDragging() const;
	void GetDragLimits(CRect& rLimits) const;

	void BuildData(const ITASKLISTBASE* pTasks, HTASKITEM hTask, BOOL bAndSiblings);
	BOOL UpdateData(const ITASKLISTBASE* pTasks, HTASKITEM hTask, IUI_UPDATETYPE nUpdate, BOOL bAndSiblings);
	int RemoveDeletedTasks(const ITASKLISTBASE* pTasks);
	void BuildTaskMap(const ITASKLISTBASE* pTasks, HTASKITEM hTask, CDWordSet& mapIDs, BOOL bAndSiblings);

	BOOL CalcDependencyEndPos(DWORD dwTaskID, NETWORKDEPENDENCY& depend, BOOL bTo, LPPOINT lpp = NULL) const;
	BOOL BuildDependency(DWORD dwFromTaskID, DWORD, NETWORKDEPENDENCY& depend) const;
	int BuildVisibleDependencyList(CNetworkDependArray& aDepends) const;
	BOOL IsDependencyPickLinePosValid() const;
	void ResetDependencyPickLinePos();

	BOOL IsDependencyEditing() const;
	BOOL IsPickingDependencyFromTask() const;
	BOOL IsPickingFromDependency() const;
	BOOL IsPickingDependencyToTask() const;
	BOOL IsDependencyEditingCancelled() const;
	BOOL IsDependencyEditingComplete() const;

private:
	BOOL CalcDependencyEndPos(DWORD dwTaskID, int nItem, NETWORKDEPENDENCY& depend, BOOL bTo, LPPOINT lpp) const;

};

#endif // !defined(AFX_NETWORKDIAGRAMCTRL_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)
