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
#include "tdcmsg.h"

//////////////////////////////////////////////////////////////////////

class IPreferences;
class CFilteredToDoCtrl;

//////////////////////////////////////////////////////////////////////

struct TDCREMINDER
{
	TDCREMINDER();
	
	CString FormatWhenString() const;
	CString GetTaskTitle() const;
	CString GetParentTitle() const;
	CString GetTaskListName() const;
	CString GetTaskComments() const;
	CString GetTaskPath() const;
	BOOL IsTaskRecurring() const;
	BOOL IsTaskDone() const;
	BOOL TaskExists() const;
	BOOL GetReminderDate(COleDateTime& date, BOOL bIncludeSnooze = TRUE) const;
	BOOL Matches(const CFilteredToDoCtrl* pTDC, DWORD dwTaskID) const;

	void Save(IPreferences* pPrefs, LPCTSTR szKey) const;
	void Load(const IPreferences* pPrefs, LPCTSTR szKey);
	
	const CFilteredToDoCtrl* pTDC;
	DWORD dwTaskID;

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

/////////////////////////////////////////////////////////////////////////////////////////////

class CTDCReminderHelper
{
public:
	CTDCReminderHelper(const CWnd& wndCaller) : m_wndCaller(wndCaller)
	{
	}

	BOOL TaskHasReminder(DWORD dwTaskID) const
	{
		return (GetTaskReminder(dwTaskID) > 0);
	}

	time_t GetTaskReminder(DWORD dwTaskID) const
	{
		ASSERT(::IsWindow(m_wndCaller));

		return (time_t)::SendMessage(::GetParent(m_wndCaller), WM_TDCM_GETTASKREMINDER, (WPARAM)m_wndCaller.GetSafeHwnd(), dwTaskID);
	}

protected:
	const CWnd& m_wndCaller;
};

/////////////////////////////////////////////////////////////////////////////////////////////


#endif // !defined(AFX_TDCREMINDER_H__F4A7DE31_EB74_490F_8092_E8D50953FBDA__INCLUDED_)
