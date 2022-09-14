// RecurrenceEdit.h: interface for the CRecurringTaskEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDLTASKCOMBOBOX_H__4EE655E3_F4B1_44EA_8AAA_39DD459AD8A8__INCLUDED_)
#define AFX_TDLTASKCOMBOBOX_H__4EE655E3_F4B1_44EA_8AAA_39DD459AD8A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\shared\tabbedcombobox.h"

//////////////////////////////////////////////////////////////////////

class CTDLTaskComboBox : public CTabbedComboBox
{
public:
	CTDLTaskComboBox();

	DWORD GetSelectedTaskID() const;
	CString GetSelectedTaskName() const;
	int GetSelectedTaskImage() const;
	
	BOOL AddTask(const CString& sTask, DWORD dwTaskID, int nIndent, BOOL bParent, int nImage);
	BOOL InsertTask(int nPos, const CString& sTask, DWORD dwTaskID, BOOL bParent, int nIndent, int nImage);

	BOOL SetSelectedTaskID(DWORD dwTaskID);
	void SetImageList(HIMAGELIST hil) { m_hilTasks = hil; }
	void EnableParentTasks(BOOL bEnable = TRUE) { m_bEnableParents = bEnable; }
	void SetShowParentTasksAsFolders(BOOL bAsFolders = TRUE) { m_bShowParentsAsFolders = bAsFolders; }
	
	int GetItemImage(int nItem) const;
	BOOL ModifyItem(int nItem, const CString& sName, int nImage);

protected:
	HIMAGELIST m_hilTasks;
	BOOL m_bEnableParents, m_bShowParentsAsFolders;

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	// Generated message map functions
	afx_msg void OnEditChange();
	afx_msg void OnDropDown();
	afx_msg LRESULT OnReselectTaskID(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()

protected:
	struct TCB_ITEMDATA : public ODCB_ITEMDATA
	{
		TCB_ITEMDATA() : nIndent(0), nImage(-1), bParent(FALSE) {}

		int nImage;
		int nIndent;
		BOOL bParent;
	};

	virtual ODCB_ITEMDATA* NewExtItemData() const { return new TCB_ITEMDATA(); }
	virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
							  DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText);
	virtual BOOL HasIcon() const { return TRUE; }
	virtual BOOL IsSelectableItem(int nItem) const;

	int GetItemIndent(int nItem) const;
	void SelectNextFind(BOOL bForward);
};

#endif // AFX_TDLTASKCOMBOBOX_H__4EE655E3_F4B1_44EA_8AAA_39DD459AD8A8__INCLUDED_