// TDCSourceControlHelper.h: interface for the CTDCCustomAttributeHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCSOURCECONTROLHELPER_H__4044B3B7_1EA0_4279_9620_F2035DAE87DF__INCLUDED_)
#define AFX_TDCSOURCECONTROLHELPER_H__4044B3B7_1EA0_4279_9620_F2035DAE87DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "tdcenum.h"

//////////////////////////////////////////////////////////////////////

class CToDoCtrl;
class CToDoCtrlData;
class CTaskFile;
class CToDoCtrlDataItems;

struct TODOITEM;

//////////////////////////////////////////////////////////////////////

class CTDCSourceControl  
{
public:
	CTDCSourceControl(const CToDoCtrl& tdc);
	virtual ~CTDCSourceControl();

	void InitialiseState(const CTaskFile& tasks);
	void PrepareTasksForSave(CTaskFile& tasks) const;

	BOOL IsSourceControlled() const;
	BOOL CanAddToSourceControl(BOOL bAdd = TRUE) const;
	BOOL CanCheckOut() const;

	TDC_FILE AddToSourceControl(BOOL bAdd = TRUE);
	TDC_FILE CheckIn();
	TDC_FILE CheckIn(CTaskFile& tasks);
	TDC_FILE CheckOut(CTaskFile& tasks /*in-out*/, CString& sCheckedOutTo/*out*/, BOOL bForce = FALSE, LPCTSTR szTasklistPath = NULL);

	BOOL IsCheckedOut() const;

protected:
	const CToDoCtrl& m_tdc;

	BOOL m_bCheckedOut;
	BOOL m_bSourceControlled;
		
protected:
	CString GetSourceControlID() const;
	BOOL GetTasklistPath(CString& sTasklistPath) const;
	BOOL MatchesOurSourceControlID(const CString& sID) const;

	static BOOL SourceControlIDsMatch(const CString& sID1, const CString& sID2);
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_TDCSOURCECONTROLHELPER_H__4044B3B7_1EA0_4279_9620_F2035DAE87DF__INCLUDED_)
