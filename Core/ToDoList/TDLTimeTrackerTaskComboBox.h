#if !defined(AFX_TDLTIMETRACKERTASKCOMBOBOX_H__8D852B02_F789_4443_96FC_A08F6BF53B6E__INCLUDED_)
#define AFX_TDLTIMETRACKERTASKCOMBOBOX_H__8D852B02_F789_4443_96FC_A08F6BF53B6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLTimeTrackerTaskComboBox.h : header file
//

#include "TDLTimeTrackerDlgData.h"

#include "..\shared\ownerdrawcomboboxbase.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLTimeTrackerTaskComboBox window

class CTDLTimeTrackerTaskComboBox : public COwnerdrawComboBoxBase
{
// Construction
public:
	CTDLTimeTrackerTaskComboBox();
	virtual ~CTDLTimeTrackerTaskComboBox();

	int FindNextItem(const CString& sText, int nFrom, BOOL bForward) const;
	DWORD GetSelectedTaskID() const;
	BOOL IsSelectedTask(DWORD dwTaskID) const;

	void ResetContent();
	int Rebuild(const TRACKTASKLIST* pTTL, DWORD dwOptions);
	void UpdateRecentlyTrackedTasks(const TRACKTASKLIST* pTTL);

	BOOL SelectTask(DWORD dwTaskID);
	BOOL SelectTask(const TRACKTASKLIST* pTTL);

	const CFilteredToDoCtrl* GetToDoCtrl() const { return m_pTDC; }

protected:
	const CFilteredToDoCtrl* m_pTDC;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLTimeTrackerTaskComboBox)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDLTimeTrackerTaskComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	BOOL ItemIsHeading(int nItem, DWORD dwItemData) const;

	BOOL FindNextItem(const CString& sText, int nFrom, int nTo, int nIncrement, int& nNext) const;
	int GetRecentlyTrackedTasks(CDWordArray& aRecentlyTrackedIDs) const;
	int BuildItemMap(CMapTaskIndex& mapComboItems) const;
	int BuildItemMap(const CDWordArray& aTaskIDs, CMapTaskIndex& mapComboItems) const;
	void UpdateRecentlyTrackedTasks(const TRACKTASKLIST* pTTL, DWORD dwSelTaskID);

	static int SortProc(const void* v1, const void* v2);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLTIMETRACKERTASKCOMBOBOX_H__8D852B02_F789_4443_96FC_A08F6BF53B6E__INCLUDED_)
