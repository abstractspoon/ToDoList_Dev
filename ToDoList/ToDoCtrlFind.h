// ToDoCtrlData.h: interface for the CToDoCtrlData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TODOCTRLTREEDATA_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_)
#define AFX_TODOCTRLTREEDATA_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tdcstruct.h"
#include "tdcenum.h"
#include "ToDoCtrlDataUtils.h"

//////////////////////////////////////////////////////////////////////

class TODOITEM;
class TODOSTRUCTURE;
class CToDoCtrlData;

struct TDCCUSTOMATTRIBUTEDEFINITION;

//////////////////////////////////////////////////////////////////////

enum // GetTaskBreadcrumbs
{
	TCFBC_PARENTSONLY	= 0x01,
	TCFBC_UP			= 0x02,
	TCFBC_VISIBLEONLY	= 0x04,
	TCFBC_APPEND		= 0x08,
};

//////////////////////////////////////////////////////////////////////

class CToDoCtrlFind  
{
public:
	CToDoCtrlFind(CTreeCtrl& tree, const CToDoCtrlData& data);
	virtual ~CToDoCtrlFind();
	
	HTREEITEM GetItem(DWORD dwID) const;
	DWORD GetTaskID(HTREEITEM hti) const;
	const TODOITEM* GetTask(HTREEITEM hti, BOOL bTrue = TRUE) const;

	int GetTaskBreadcrumbs(HTREEITEM hti, CDWordArray& aBreadcrumbs, DWORD dwFlags = TCFBC_PARENTSONLY | TCFBC_UP) const;

	// generic
	CString GetLongestValue(TDC_ATTRIBUTE nAttrib, BOOL bVisibleOnly = TRUE) const;
	CString GetLongestValue(TDC_ATTRIBUTE nAttrib, const CString& sExtra, BOOL bVisibleOnly = TRUE) const;

	// specific
	CString GetLongestTimeEstimate(TDC_UNITS nDefUnits, BOOL bVisibleOnly = TRUE) const;
	CString GetLongestTimeSpent(TDC_UNITS nDefUnits, BOOL bVisibleOnly = TRUE) const;
	CString GetLongestTimeRemaining(TDC_UNITS nDefUnits, BOOL bVisibleOnly = TRUE) const;
	CString GetLongestCustomAttribute(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bVisibleOnly = TRUE) const;

	DWORD GetLargestReferenceID(BOOL bVisibleOnly = TRUE) const;
	int GetLargestFileLinkCount(BOOL bVisibleOnly = TRUE) const;

	BOOL FindVisibleTaskWithDueTime() const;
	BOOL FindVisibleTaskWithStartTime() const;
	BOOL FindVisibleTaskWithDoneTime() const;
	BOOL FindVisibleTaskWithTime(TDC_DATE nDate);

	// Finds tasks only in the tree
	int FindTasks(const SEARCHPARAMS& params, CResultArray& aResults) const;
	HTREEITEM FindFirstTask(const SEARCHPARAMS& params, SEARCHRESULT& result, BOOL bForwards = TRUE) const;
	HTREEITEM FindNextTask(HTREEITEM htiStart, const SEARCHPARAMS& params, SEARCHRESULT& result, BOOL bForwards = TRUE) const;

protected:
	CTreeCtrl& m_tree; 
	const CToDoCtrlData& m_data;
	CTDCTaskMatcher m_matcher;
	CTDCTaskCalculator m_calculator;
	CTDCTaskFormatter m_formatter;

protected:
	void FindTasks(HTREEITEM hti, const SEARCHPARAMS& params, CResultArray& aResults) const;

	BOOL FindVisibleTaskWithDueTime(HTREEITEM hti) const;
	BOOL FindVisibleTaskWithStartTime(HTREEITEM hti) const;
	BOOL FindVisibleTaskWithDoneTime(HTREEITEM hti) const;

	CString GetLongestTime(TDC_UNITS nDefUnits, TDC_COLUMN nCol, BOOL bVisibleOnly = TRUE) const;
 	CString GetLongestTime(HTREEITEM hti, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_UNITS nDefUnits, TDC_COLUMN nCol, BOOL bVisibleOnly) const;
 	CString GetLongestCustomAttribute(HTREEITEM hti, const TODOITEM* pTDI, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bVisibleOnly) const;
	CString GetLongestValue(TDC_ATTRIBUTE nAttrib, HTREEITEM hti, const TODOITEM* pTDI, BOOL bVisibleOnly) const;
	CString GetLongestValue(TDC_ATTRIBUTE nAttrib, HTREEITEM hti, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bVisibleOnly) const;
	static CString GetLongerString(const CString& str1, const CString& str2);

	DWORD GetLargestReferenceID(HTREEITEM hti, const TODOITEM* pTDI, BOOL bVisibleOnly) const;
	int GetLargestFileLinkCount(HTREEITEM hti, const TODOITEM* pTDI, BOOL bVisibleOnly) const;
	BOOL WantSearchChildren(HTREEITEM hti, BOOL bVisibleOnly) const;
	
};

#endif // !defined(AFX_TODOCTRLTREEDATA_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_)
