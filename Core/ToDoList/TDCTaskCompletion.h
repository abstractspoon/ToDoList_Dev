#if !defined(AFX_TDCTASKCOMPLETION_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_)
#define AFX_TDCTASKCOMPLETION_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToDoCtrl.h : header file
//

#include <afxtempl.h>

#include "tdcenum.h"

/////////////////////////////////////////////////////////////////////////////

class CToDoCtrlData;

struct TDCCADATA;

/////////////////////////////////////////////////////////////////////////////

struct TDCTASKCOMPLETION
{
	TDCTASKCOMPLETION();
	TDCTASKCOMPLETION(const CToDoCtrlData& data, DWORD dwTaskID);

	BOOL IsDone() const;
	void ClearDate();

	BOOL DoneDateCausesStateChange(const COleDateTime& dtNewDone) const;
	BOOL PercentCausesStateChange(int nNewPercent) const;
	BOOL StatusCausesStateChange(const CString& sNewStatus, const CString& sDefaultStatus, const CString& sDoneStatus) const;

	DWORD dwID;
	COleDateTime dtDone;
	CString sStatus;
	int nPercent;
	BOOL bStateChange;
};

/////////////////////////////////////////////////////////////////////////////

class CTDCTaskCompletionArray : protected CArray<TDCTASKCOMPLETION, TDCTASKCOMPLETION&>
{
public:
	CTDCTaskCompletionArray(const CToDoCtrlData& data, const CString& sDoneStatus);
	virtual ~CTDCTaskCompletionArray();

	BOOL Add(DWORD dwTaskID, const COleDateTime& date);
	BOOL Add(DWORD dwTaskID, const CString& sStatus);
	BOOL Add(DWORD dwTaskID, int nPercent);
	BOOL Add(DWORD dwTaskID, TDC_ATTRIBUTE nAttribID, const TDCCADATA& data);
	BOOL Toggle(DWORD dwTaskID);

	int Add(const CDWordArray& aTaskIDs, const COleDateTime& date);
	int Add(const CDWordArray& aTaskIDs, const CString& sStatus);
	int Add(const CDWordArray& aTaskIDs, int nPercent);
	int Toggle(const CDWordArray& aTaskIDs);

	const TDCTASKCOMPLETION& operator[](INT_PTR nIndex) const {	return m_pData[nIndex]; }
	int GetSize() const { return CArray<TDCTASKCOMPLETION, TDCTASKCOMPLETION&>::GetSize(); }

	int GetTaskIDs(CDWordArray& aTasksIDs, BOOL bAppend) const;
	int GetTaskIDsForCompletion(CDWordArray& aTasksIDs) const;
	BOOL HasStateChange() const;

protected:
	const CToDoCtrlData& m_data;
	CString m_sDoneStatus;

protected:

};

#endif