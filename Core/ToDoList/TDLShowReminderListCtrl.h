#if !defined(AFX_TDLSHOWREMINDERLISTCTRL_H__6B20922F_A2AA_4C95_B9E6_45F5EBEF18BF__INCLUDED_)
#define AFX_TDLSHOWREMINDERLISTCTRL_H__6B20922F_A2AA_4C95_B9E6_45F5EBEF18BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLShowReminderDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////

#include "tdcreminder.h"

#include "..\Shared\EnListCtrl.h"

/////////////////////////////////////////////////////////////////////////////

class CFilteredToDoCtrl;

/////////////////////////////////////////////////////////////////////////////
// CTDLShowReminderListCtrl

class CTDLShowReminderListCtrl : public CEnListCtrl
{
public:
	CTDLShowReminderListCtrl();

	BOOL Initialise();

	BOOL AddReminder(const TDCREMINDER& rem);
	BOOL UpdateReminder(const TDCREMINDER& rem);
	BOOL RemoveReminder(const TDCREMINDER& rem);
	int RemoveReminders(const CFilteredToDoCtrl& tdc);

	int GetReminders(const CFilteredToDoCtrl& tdc, CTDCReminderArray& aRem) const;
	int GetReminders(CTDCReminderArray& aRem) const;

	DWORD GetReminderID(const TDCREMINDER& rem) const;
	DWORD GetReminderID(int nItem) const { return GetItemData(nItem); }

	int FindReminder(const TDCREMINDER& rem) const;
	int GetSelectedReminder(TDCREMINDER& rem) const;
	int GetSelectedReminders(CTDCReminderArray& aRem) const;

	void DeleteAllItems();
	void UpdateColumnWidths();
	BOOL RestoreFocusToList(int nPrevSel);

protected:
	BOOL m_bHasIcons;
	DWORD m_dwNextReminderID;
	CTDCReminderMap m_mapReminders;

protected:
	virtual int CompareItems(DWORD dwItemData1, DWORD dwItemData2, int nSortColumn) const;
	virtual COLORREF GetItemTextColor(int nItem, int nSubItem, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual COLORREF GetItemBackColor(int nItem, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual void DrawCellText(CDC* pDC, int nItem, int nCol, const CRect& rText, const CString& sText, COLORREF crText, UINT nDrawTextFlags);
	virtual void DrawItemBackground(CDC* pDC, int nItem, const CRect& rItem, COLORREF crBack, BOOL bSelected, BOOL bDropHighlighted, BOOL bFocused);

protected:
	void UpdateIconStatus();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLSHOWREMINDERLISTCTRL_H__6B20922F_A2AA_4C95_B9E6_45F5EBEF18BF__INCLUDED_)
