// tdcreminder.h: interface for the tdcreminder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCREMINDER_H__F4A7DE31_EB74_490F_8092_E8D50953FBDA__INCLUDED_)
#define AFX_TDCREMINDER_H__F4A7DE31_EB74_490F_8092_E8D50953FBDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "tdcenum.h"

//////////////////////////////////////////////////////////////////////

class IPreferences;
class CFilteredToDoCtrl;

//////////////////////////////////////////////////////////////////////

struct TDCREMINDER
{
	TDCREMINDER();
	
	CString FormatWhenString() const;
	CString GetTaskTitle() const;
	CString GetTaskComments() const;
	BOOL IsTaskRecurring() const;
	BOOL IsTaskDone() const;
	BOOL TaskExists() const;
	BOOL GetReminderDate(COleDateTime& date) const;

	void Save(IPreferences* pPrefs, LPCTSTR szKey) const;
	void Load(const IPreferences* pPrefs, LPCTSTR szKey);
	
	DWORD dwTaskID;
	const CFilteredToDoCtrl* pTDC;
	COleDateTime dtAbsolute;
	BOOL bRelative;
	double dRelativeDaysLeadIn;
	double dDaysSnooze;
	TDC_REMINDER nRelativeFromWhen;
	BOOL bEnabled;
	CString sSoundFile;
	CString sStickiesID;
	int nLastSnoozeMins;
};

typedef CMap<DWORD, DWORD, TDCREMINDER, TDCREMINDER&> CTDCReminderMap;
typedef CArray<TDCREMINDER, TDCREMINDER&> CTDCReminderArray;


#endif // !defined(AFX_TDCREMINDER_H__F4A7DE31_EB74_490F_8092_E8D50953FBDA__INCLUDED_)
