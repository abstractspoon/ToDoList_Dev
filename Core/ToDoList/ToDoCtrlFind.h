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

class CToDoCtrlFind  
{
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
	
	int FindTasks(const SEARCHPARAMS& params, CResultArray& aResults, BOOL bCheckDueToday) const;
	HTREEITEM FindFirstTask(const SEARCHPARAMS& params, SEARCHRESULT& result, BOOL bForwards, BOOL bCheckDueToday) const;
	HTREEITEM FindNextTask(HTREEITEM htiStart, const SEARCHPARAMS& params, SEARCHRESULT& result, BOOL bForwards, BOOL bCheckDueToday) const;

protected:
	const CTreeCtrlHelper& m_tch; 
	const CToDoCtrlData& m_data;
	const CContentMgr& m_mgrContent;
	const CTDCCustomAttribDefinitionArray& m_aCustAttribDefs;

	CTDCTaskMatcher m_matcher;
	CTDCTaskCalculator m_calculator;

protected:
	void FindTasks(HTREEITEM hti, const SEARCHPARAMS& params, CResultArray& aResults, BOOL bCheckDueToday) const;

	BOOL WantSearchChildren(HTREEITEM hti, BOOL bVisibleOnly) const;
	BOOL CheckGetTask(HTREEITEM hti, const TODOITEM*& pTDI, BOOL bTrueTask) const;
	BOOL CheckGetTask(HTREEITEM hti, const TODOITEM*& pTDI, const TODOSTRUCTURE*& pTDS) const;

};

#endif // !defined(AFX_TODOCTRLTREEDATA_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_)
