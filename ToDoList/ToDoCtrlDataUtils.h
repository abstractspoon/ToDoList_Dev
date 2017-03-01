// ToDoCtrlData.h: interface for the CToDoCtrlData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TODOCTRLDATAUTILS_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_)
#define AFX_TODOCTRLDATAUTILS_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tdcstruct.h"

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

class CToDoCtrlData;
class CBinaryData;

class TODOITEM;
class TODOSTRUCTURE;

//////////////////////////////////////////////////////////////////////

class CTDCTaskMatcher
{
public:
	CTDCTaskMatcher(const CToDoCtrlData& data);

	int FindTasks(const SEARCHPARAMS& query, CResultArray& aResults) const;
	int FindTasks(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const SEARCHPARAMS& query, CResultArray& aResults) const;

	BOOL TaskMatches(DWORD dwTaskID, const SEARCHPARAMS& params, SEARCHRESULT& result) const;
	BOOL TaskMatches(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const SEARCHPARAMS& params, SEARCHRESULT& result) const;

protected:
	const CToDoCtrlData& m_data;

protected:
	BOOL ValueMatches(const COleDateTime& date, const SEARCHPARAM& rule, SEARCHRESULT& result, BOOL bIncludeTime, TDC_DATE nDate) const;
	BOOL ValueMatches(const CString& sText, const SEARCHPARAM& rule, SEARCHRESULT& result) const;
	BOOL ValueMatches(double dValue, const SEARCHPARAM& rule, SEARCHRESULT& result) const;
	BOOL ValueMatches(int nValue, const SEARCHPARAM& rule, SEARCHRESULT& result) const;
	BOOL ValueMatches(const TDCCADATA& data, DWORD dwAttribType, const SEARCHPARAM& rule, SEARCHRESULT& result) const;
	BOOL ValueMatches(const CString& sComments, const CBinaryData& customComments, const SEARCHPARAM& rule, SEARCHRESULT& result) const;

	BOOL ArrayMatches(const CStringArray& aItems, const SEARCHPARAM& rule, SEARCHRESULT& result, BOOL bMatchWholeWordsOnly = TRUE) const;
	BOOL ValueMatchesAsArray(const CString& sText, const SEARCHPARAM& rule, SEARCHRESULT& result, BOOL bMatchWholeWordsOnly) const;

	CString FormatResultDate(const COleDateTime& date) const;
};

class CTDCTaskComparer
{
public:
	CTDCTaskComparer(const CToDoCtrlData& data);

	int CompareTasks(DWORD dwTask1ID, DWORD dwTask2ID, TDC_COLUMN nSortBy, BOOL bAscending, BOOL bSortDueTodayHigh, BOOL bIncTime = FALSE) const;
	int CompareTasks(DWORD dwTask1ID, DWORD dwTask2ID, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bAscending) const;

protected:
	const CToDoCtrlData& m_data;

protected:
	static int Compare(const COleDateTime& date1, const COleDateTime& date2, BOOL bIncTime, TDC_DATE nDate);
	static int Compare(const CString& sText1, const CString& sText2, BOOL bCheckEmpty = FALSE);
	static int Compare(int nNum1, int nNum2);
	static int Compare(double dNum1, double dNum2);

};

#endif // !defined(AFX_TODOCTRLDATAUTILS_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_)
