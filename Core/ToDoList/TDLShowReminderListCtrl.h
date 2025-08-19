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
	CTDLShowReminderListCtrl(LPCTSTR szPrefsKey);

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
	BOOL RestoreFocusToList(int nPrevSel);
	void SetISODateFormat(BOOL bISODates);

protected:
	int m_nTextOffset;
	BOOL m_bModifyingReminders;
	BOOL m_bISODates;

	CString m_sPrefsKey;
	DWORD m_dwNextReminderID;
	CTDCReminderMap m_mapReminders;

protected:
	virtual int CompareItems(DWORD dwItemData1, DWORD dwItemData2, int nSortColumn) const;
	virtual COLORREF GetItemTextColor(int nItem, int nSubItem, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual COLORREF GetItemBackColor(int nItem, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual void DrawCellText(CDC* pDC, int nItem, int nCol, const CRect& rText, const CString& sText, COLORREF crText, UINT nDrawTextFlags);
	virtual void DrawItemBackground(CDC* pDC, int nItem, const CRect& rItem, COLORREF crBack, BOOL bSelected, BOOL bDropHighlighted, BOOL bFocused);

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

protected:
	void RecalcTextOffset();
	int GetTextOffset(const TDCREMINDER& rem) const;
	void RecalcColumnWidths();
	void UpdateItemText(int nItem, const TDCREMINDER& rem);
	void ReformatReminderDates();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLSHOWREMINDERLISTCTRL_H__6B20922F_A2AA_4C95_B9E6_45F5EBEF18BF__INCLUDED_)
