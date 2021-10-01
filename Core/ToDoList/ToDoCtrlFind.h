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
class CTreeCtrlHelper;

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
	friend class CToDoCtrlFind;

public:
	BOOL Initialise(const CTDCColumnIDMap& mapCols, const CTDCCustomAttribDefinitionArray& aCustAttribDefs);
	BOOL CheckUpdateValue(TDC_COLUMN nColID, const CString& sValue);
	BOOL CheckUpdateValue(TDC_COLUMN nColID, const CStringArray& aValues);
	BOOL UpdateValue(TDC_COLUMN nColID, const CString& sValue);
	BOOL UpdateValue(TDC_COLUMN nColID, int nValue);
	BOOL HasColumn(TDC_COLUMN nColID) const;
	CString GetLongestValue(TDC_COLUMN nColID) const;

protected:
	// Used by CToDoCtrlFind
	static BOOL IsSupportedColumn(TDC_COLUMN nColID);
	static BOOL IsSupportedColumn(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef);
};

//////////////////////////////////////////////////////////////////////


class CToDoCtrlFind  
{
	friend struct LONGESTITEM;

public:
	CToDoCtrlFind(const CTreeCtrlHelper& tch, const CToDoCtrlData& data, const CTDCReminderHelper& reminders);
	virtual ~CToDoCtrlFind();
	
	DWORD GetTaskID(HTREEITEM hti) const;
	const TODOITEM* GetTask(HTREEITEM hti, BOOL bTrue) const;

	int GetTaskBreadcrumbs(HTREEITEM hti, CDWordArray& aBreadcrumbs, DWORD dwFlags = TCFBC_PARENTSONLY | TCFBC_UP) const;
	
	// generic
	int GetLongestValues(const CTDCColumnIDMap& mapCols, 
						 const CTDCCustomAttribDefinitionArray& aCustAttribDefs, 
						 CTDCLongestItemMap& mapLongest, 
						 BOOL bVisibleOnly) const;

	CString GetLongestValue(TDC_COLUMN nCol, BOOL bVisibleOnly) const;
	CString GetLongestValue(TDC_COLUMN nCol, const CStringArray& aPossible, BOOL bVisibleOnly) const;
	CString GetLongestTime(TDC_COLUMN nCol, BOOL bVisibleOnly) const;
	CString GetLongestValue(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bVisibleOnly) const;

	// specific
	CString GetLongestTimeEstimate(BOOL bVisibleOnly) const;
	CString GetLongestTimeSpent(BOOL bVisibleOnly) const;
	CString GetLongestTimeRemaining(BOOL bVisibleOnly) const;
	CString GetLargestCommentsSizeInKB(BOOL bVisibleOnly) const;

	DWORD GetLargestReferenceID(BOOL bVisibleOnly) const;
	int GetLargestFileLinkCount(BOOL bVisibleOnly) const;
	int GetLargestCustomAttributeArraySize(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bVisibleOnly) const;

	BOOL FindVisibleTaskWithDueTime() const;
	BOOL FindVisibleTaskWithStartTime() const;
	BOOL FindVisibleTaskWithDoneTime() const;
	BOOL FindVisibleTaskWithTime(TDC_DATE nDate) const;

	// Finds tasks only in the tree
	int FindTasks(const SEARCHPARAMS& params, CResultArray& aResults, BOOL bCheckDueToday) const;
	HTREEITEM FindFirstTask(const SEARCHPARAMS& params, SEARCHRESULT& result, BOOL bForwards, BOOL bCheckDueToday) const;
	HTREEITEM FindNextTask(HTREEITEM htiStart, const SEARCHPARAMS& params, SEARCHRESULT& result, BOOL bForwards, BOOL bCheckDueToday) const;

	// For debugging only
#ifdef _DEBUG
	void WalkTree(BOOL bVisibleOnly) const;
#endif

protected:
	const CTreeCtrlHelper& m_tch; 
	const CToDoCtrlData& m_data;

	CTDCTaskMatcher m_matcher;
	CTDCTaskCalculator m_calculator;
	CTDCTaskFormatter m_formatter;

protected:
	void FindTasks(HTREEITEM hti, const SEARCHPARAMS& params, CResultArray& aResults, BOOL bCheckDueToday) const;

	BOOL FindVisibleTaskWithDueTime(HTREEITEM hti) const;
	BOOL FindVisibleTaskWithStartTime(HTREEITEM hti) const;
	BOOL FindVisibleTaskWithDoneTime(HTREEITEM hti) const;
	
	// generic
	void GetLongestValues(const CTDCCustomAttribDefinitionArray& aCustAttribDefs,
						  HTREEITEM hti, 
						  const TODOITEM* pTDI, 
						  const TODOSTRUCTURE* pTDS, 
						  CTDCLongestItemMap& mapLongest, 
						  BOOL bVisibleOnly) const;

	CString GetLongestValue(TDC_COLUMN nCol, HTREEITEM hti, const TODOITEM* pTDI, BOOL bVisibleOnly) const;
	CString GetLongestValue(TDC_COLUMN nCol, HTREEITEM hti, const TODOITEM* pTDI, const CString& sLongestPossible, BOOL bVisibleOnly) const;
	CString GetLongestValue(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, HTREEITEM hti, const TODOITEM* pTDI, BOOL bVisibleOnly) const;

	// specific
 	CString GetLongestTime(HTREEITEM hti, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_COLUMN nCol, BOOL bVisibleOnly) const;
	CString GetLongestSubtaskDone(HTREEITEM hti, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bVisibleOnly) const;
	CString GetLongestPosition(HTREEITEM hti, const TODOSTRUCTURE* pTDS, BOOL bVisibleOnly) const;
	CString GetLongestPath(HTREEITEM hti, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const CString& sParentPath, BOOL bVisibleOnly) const;
	CString GetLongestCost(HTREEITEM hti, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bVisibleOnly) const;

	DWORD GetLargestReferenceID(HTREEITEM hti, const TODOITEM* pTDI, BOOL bVisibleOnly) const;
	float GetLargestCommentsSizeInKB(HTREEITEM hti, const TODOITEM* pTDI, BOOL bVisibleOnly) const;
	int GetLargestFileLinkCount(HTREEITEM hti, const TODOITEM* pTDI, BOOL bVisibleOnly) const;
	int GetLargestCustomAttributeArraySize(HTREEITEM hti, const TODOITEM* pTDI, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bVisibleOnly) const;
	BOOL GetLongestAggregatedValue(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, CString& sLongest) const;

	BOOL WantSearchChildren(HTREEITEM hti, BOOL bVisibleOnly) const;
	BOOL CheckGetTask(HTREEITEM hti, const TODOITEM*& pTDI, BOOL bTrueTask) const;
	BOOL CheckGetTask(HTREEITEM hti, const TODOITEM*& pTDI, const TODOSTRUCTURE*& pTDS) const;

	// For debugging
#ifdef _DEBUG
	CString WalkTree(HTREEITEM hti, BOOL bVisibleOnly) const;
#endif

	static CString GetLongerString(const CString& str1, const CString& str2);
	static BOOL EqualsLongestPossible(const CString& sValue, const CString& sLongestPossible);
	static CString GetLongestRecurrenceOption();
};

#endif // !defined(AFX_TODOCTRLTREEDATA_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_)
