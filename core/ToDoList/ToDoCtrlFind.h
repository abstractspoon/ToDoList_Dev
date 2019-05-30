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

class TODOSTRUCTURE;
class CToDoCtrlData;
class CTDCLongestItemMap;

struct TODOITEM;
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

class CTDCLongestItemMap : public CMap<TDC_COLUMN, TDC_COLUMN, CString, LPCTSTR>
{
public:
	BOOL Initialise(const CTDCColumnIDMap& mapCols);
	BOOL CheckUpdateValue(const CTDCColumnIDMap& mapCols, TDC_COLUMN nColID, const CString& sValue);
	BOOL CheckUpdateValue(const CTDCColumnIDMap& mapCols, TDC_COLUMN nColID, const CStringArray& aValues);
	BOOL UpdateValue(TDC_COLUMN nColID, const CString& sValue);
	BOOL HasColumn(TDC_COLUMN nColID) const;
	CString GetLongestValue(TDC_COLUMN nColID) const;

	static BOOL IsSupportedColumn(TDC_COLUMN nColID);
};

//////////////////////////////////////////////////////////////////////


class CToDoCtrlFind  
{
	friend struct LONGESTITEM;

public:
	CToDoCtrlFind(CTreeCtrl& tree, const CToDoCtrlData& data);
	virtual ~CToDoCtrlFind();
	
	HTREEITEM GetItem(DWORD dwID) const;
	DWORD GetTaskID(HTREEITEM hti) const;
	const TODOITEM* GetTask(HTREEITEM hti, BOOL bTrue) const;

	int GetTaskBreadcrumbs(HTREEITEM hti, CDWordArray& aBreadcrumbs, DWORD dwFlags = TCFBC_PARENTSONLY | TCFBC_UP) const;

	int GetLongestValues(const CTDCColumnIDMap& mapCols, CTDCLongestItemMap& mapLongest, BOOL bVisibleOnly) const;
	void GetLongestValues(const CTDCColumnIDMap& mapCols, HTREEITEM hti, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CTDCLongestItemMap& mapLongest, BOOL bVisibleOnly) const;

	// generic
	CString GetLongestValue(TDC_ATTRIBUTE nAttrib, BOOL bVisibleOnly) const;
	CString GetLongestValue(TDC_ATTRIBUTE nAttrib, const CStringArray& aPossible, BOOL bVisibleOnly) const;

	// specific
	CString GetLongestCost() const;
	CString GetLongestTimeEstimate(TDC_UNITS nDefUnits, BOOL bVisibleOnly) const;
	CString GetLongestTimeSpent(TDC_UNITS nDefUnits, BOOL bVisibleOnly) const;
	CString GetLongestTimeRemaining(TDC_UNITS nDefUnits, BOOL bVisibleOnly) const;
	CString GetLongestCustomAttribute(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bVisibleOnly) const;

	DWORD GetLargestReferenceID(BOOL bVisibleOnly) const;
	float GetLargestCommentsSizeInKB(BOOL bVisibleOnly) const;
	int GetLargestFileLinkCount(BOOL bVisibleOnly) const;

	BOOL FindVisibleTaskWithDueTime() const;
	BOOL FindVisibleTaskWithStartTime() const;
	BOOL FindVisibleTaskWithDoneTime() const;
	BOOL FindVisibleTaskWithTime(TDC_DATE nDate);

	// Finds tasks only in the tree
	int FindTasks(const SEARCHPARAMS& params, CResultArray& aResults) const;
	HTREEITEM FindFirstTask(const SEARCHPARAMS& params, SEARCHRESULT& result, BOOL bForwards = TRUE) const;
	HTREEITEM FindNextTask(HTREEITEM htiStart, const SEARCHPARAMS& params, SEARCHRESULT& result, BOOL bForwards = TRUE) const;

	// For debugging
	void WalkTree(BOOL bVisibleOnly) const;

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
	
	// generic
	CString GetLongestValue(TDC_ATTRIBUTE nAttrib, HTREEITEM hti, const TODOITEM* pTDI, BOOL bVisibleOnly) const;
	CString GetLongestValue(TDC_ATTRIBUTE nAttrib, HTREEITEM hti, const TODOITEM* pTDI, const CString& sLongestPossible, BOOL bVisibleOnly) const;

	// specific
	CString GetLongestTime(TDC_UNITS nDefUnits, TDC_COLUMN nCol, BOOL bVisibleOnly) const;
 	CString GetLongestTime(HTREEITEM hti, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_UNITS nDefUnits, TDC_COLUMN nCol, BOOL bVisibleOnly) const;
 	CString GetLongestCustomAttribute(HTREEITEM hti, const TODOITEM* pTDI, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bVisibleOnly) const;
	CString GetLongestSubtaskDone(HTREEITEM hti, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bVisibleOnly) const;
	CString GetLongestPosition(HTREEITEM hti, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bVisibleOnly) const;
	CString GetLongestPath(HTREEITEM hti, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const CString& sParentPath, BOOL bVisibleOnly) const;
	
	DWORD GetLargestReferenceID(HTREEITEM hti, const TODOITEM* pTDI, BOOL bVisibleOnly) const;
	float GetLargestCommentsSizeInKB(HTREEITEM hti, const TODOITEM* pTDI, BOOL bVisibleOnly) const;
	int GetLargestFileLinkCount(HTREEITEM hti, const TODOITEM* pTDI, BOOL bVisibleOnly) const;
	BOOL WantSearchChildren(HTREEITEM hti, BOOL bVisibleOnly) const;
	BOOL CheckGetTask(HTREEITEM hti, const TODOITEM*& pTDI, BOOL bTrueTask) const;
	BOOL CheckGetTask(HTREEITEM hti, const TODOITEM*& pTDI, const TODOSTRUCTURE*& pTDS) const;

	// For debugging
	CString WalkTree(HTREEITEM hti, BOOL bVisibleOnly) const;

	static CString GetLongerString(const CString& str1, const CString& str2);
	static BOOL EqualsLongestPossible(const CString& sValue, const CString& sLongestPossible);
};

#endif // !defined(AFX_TODOCTRLTREEDATA_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_)
