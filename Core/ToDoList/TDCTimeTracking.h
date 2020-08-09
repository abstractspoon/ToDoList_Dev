#if !defined(AFX_TDCTIMETRACKING_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_)
#define AFX_TDCTIMETRACKING_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToDoCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
const double TICKS2HOURS = 10.0 / (1000 * 3600);
#else
const double TICKS2HOURS = 1.0 / (1000 * 3600);
#endif

/////////////////////////////////////////////////////////////////////////////

class CToDoCtrlData;

/////////////////////////////////////////////////////////////////////////////

class CTDCTimeTracking
{
// Construction
public:
	CTDCTimeTracking(const CToDoCtrlData& data, const CTreeSelectionHelper& tsh);
	virtual ~CTDCTimeTracking();

	BOOL PauseTracking(BOOL bPause = TRUE);
	BOOL CanTrackTask(DWORD dwTaskID) const;
	BOOL CanTrackSelectedTask() const;
	BOOL IsTrackingTask(DWORD dwTaskID, BOOL bActive = TRUE) const;
	BOOL IsTrackingTaskOrSubtask(DWORD dwTaskID, BOOL bActive = TRUE) const;
	BOOL IsTrackingSelectedTask(BOOL bActive = TRUE) const;
	BOOL IsTracking(BOOL bActive = TRUE) const;
	BOOL EndTracking();
	BOOL BeginTracking(DWORD dwTaskID);
	BOOL IsReminderDue() const;

	DWORD GetTrackedTaskID(BOOL bActive = TRUE) const;

	void SetTrackingReminderInterval(int nMinutes);
	void ResetReminderIsDue();

	double IncrementTrackedTime();
	CString FormatElapsedTime() const;

	static void SetUpdateInterval(DWORD dwTicks);

protected:
	const CToDoCtrlData& m_data;
	const CTreeSelectionHelper& m_tsh;

	DWORD m_dwTimeTrackReminderIntervalTicks;
	DWORD m_dwTimeTrackElapsedTicks;
	DWORD m_dwTimeTrackTaskID;
	DWORD m_dwLastTimeTrackTaskID;
	DWORD m_dwTimeTrackTickLast;

	BOOL m_bTimeTrackingPaused;

	static DWORD s_dwUpdateIntervalTicks;

protected:
	DWORD GetSelectedTaskID() const;
	DWORD GetElapsedTicks() const;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDCTIMETRACKING_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_)
