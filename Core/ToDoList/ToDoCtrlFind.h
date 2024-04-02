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
class CContentMgr;

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
	CToDoCtrlFind(const CTreeCtrlHelper& tch, 
				  const CToDoCtrlData& data, 
				  const CTDCCustomAttribDefinitionArray& aCustAttribDefs,
				  const CTDCReminderHelper& reminders,
				  const CContentMgr& mgrContent);

	virtual ~CToDoCtrlFind();
	
	DWORD GetTaskID(HTREEITEM hti) const;
	const TODOITEM* GetTask(HTREEITEM hti, BOOL bTrue) const;

	int GetTaskBreadcrumbs(HTREEITEM hti, CDWordArray& aBreadcrumbs, DWORD dwFlags = TCFBC_PARENTSONLY | TCFBC_UP) const;
	
	// generic
	int GetLongestValues(const CTDCColumnIDMap& mapCols, const CDWordArray& aTaskIDs, CTDCLongestItemMap& mapLongest) const;

	CString GetLongestValue(TDC_COLUMN nCol, const CDWordArray& aTaskIDs) const;
	CString GetLongestValue(TDC_COLUMN nCol, const CStringArray& aPossible, const CDWordArray& aTaskIDs) const;
	CString GetLongestTime(TDC_COLUMN nCol, const CDWordArray& aTaskIDs) const;
	CString GetLongestValue(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, const CDWordArray& aTaskIDs) const;

	// specific
	CString GetLongestTimeEstimate(const CDWordArray& aTaskIDs) const;
	CString GetLongestTimeSpent(const CDWordArray& aTaskIDs) const;
	CString GetLongestTimeRemaining(const CDWordArray& aTaskIDs) const;
	CString GetLargestCommentsSizeInKB(const CDWordArray& aTaskIDs) const;

	DWORD GetLargestReferenceID(const CDWordArray& aTaskIDs) const;
	int GetLargestFileLinkCount(const CDWordArray& aTaskIDs) const;
	int GetLargestCustomAttributeArraySize(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, const CDWordArray& aTaskIDs) const;

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
	const CContentMgr& m_mgrContent;
	const CTDCCustomAttribDefinitionArray& m_aCustAttribDefs;

	CTDCTaskMatcher m_matcher;
	CTDCTaskCalculator m_calculator;
	CTDCTaskFormatter m_formatter;

protected:
	void FindTasks(HTREEITEM hti, const SEARCHPARAMS& params, CResultArray& aResults, BOOL bCheckDueToday) const;

	// generic
	void GetLongestValues(const TODOITEM* pTDI,
						  const TODOSTRUCTURE* pTDS,
						  const CTDCCustomAttribDefinitionArray& aCustAttribDefs,
						  CTDCLongestItemMap& mapLongest) const;

	CString GetLongestValue(TDC_COLUMN nCol, const CString& sLongestPossible, const CDWordArray& aTaskIDs) const;

	// specific
	CString GetLongestSubtaskDone(const CDWordArray& aTaskIDs) const;
	CString GetLongestPosition(const CDWordArray& aTaskIDs) const;
	CString GetLongestPath(const CDWordArray& aTaskIDs) const;
	CString GetLongestCost(const CDWordArray& aTaskIDs) const;

	BOOL GetLongestAggregatedValue(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, const CDWordArray& aTaskIDs, CString& sLongest) const;

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
