// RecurrenceEdit.h: interface for the CRecurringTaskEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDLTASKCOMBOBOX_H__4EE655E3_F4B1_44EA_8AAA_39DD459AD8A8__INCLUDED_)
#define AFX_TDLTASKCOMBOBOX_H__4EE655E3_F4B1_44EA_8AAA_39DD459AD8A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\shared\OwnerdrawComboBoxBase.h"

//////////////////////////////////////////////////////////////////////

class CTaskFile;
class CTDCImageList;

typedef void* HTASKITEM;

//////////////////////////////////////////////////////////////////////

class CTDLTaskComboBox : public COwnerdrawComboBoxBase
{
public:
	CTDLTaskComboBox();

	DWORD GetSelectedTaskID(BOOL bTrueTask) const;
	CString GetSelectedTaskName() const;
	int GetSelectedTaskImage() const;
	BOOL SetSelectedTaskID(DWORD dwTaskID);

	int Populate(const CTaskFile& tasks, const CTDCImageList& ilTasks);
	int Populate(const CTaskFile& tasks, const CTDCImageList& ilTasks, const CDWordArray& aRecentSel);

	void EnableParentTasks(BOOL bEnable = TRUE) { m_bEnableParents = bEnable; }
	void SetShowParentTasksAsFolders(BOOL bAsFolders = TRUE) { m_bShowParentsAsFolders = bAsFolders; }

protected:
	const CTDCImageList* m_pIlTasks;
	BOOL m_bEnableParents, m_bShowParentsAsFolders;

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	// Generated message map functions
	afx_msg void OnEditChange();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

protected:
	struct TCB_ITEMDATA : public ODCB_ITEMDATA
	{
		TCB_ITEMDATA() : nDepth(0), nImage(-1), bParent(FALSE), dwRefTaskID(FALSE) {}

		int nImage;
		int nDepth;
		BOOL bParent;
		DWORD dwRefTaskID;
	};

	virtual ODCB_ITEMDATA* NewExtItemData() const { return new TCB_ITEMDATA(); }
	virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
							  DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText);
	virtual void FillListItemBkgnd(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
								   DWORD dwItemData, COLORREF crBack);
	virtual int GetExtraListboxWidth() const;
	virtual int GetMaxDropWidth() const;
	virtual int CalcMinItemHeight(BOOL bList) const;
	virtual BOOL IsSelectableItem(int nItem) const;

	int GetItemDepth(int nItem) const;
	void SelectNextFind(BOOL bForward);
	DWORD GetItemRefTaskID(int nItem) const;

	void Populate(const CTaskFile& tasks, HTASKITEM hTask, int nDepth);
	BOOL InsertTask(int nPos, const CString& sTask, DWORD dwTaskID, BOOL bParent, int nDepth, int nImage, DWORD dwRefTaskID = 0);
	int GetItemImage(int nItem) const;
	BOOL ModifyItem(int nItem, const CString& sName, int nImage);
};

#endif // AFX_TDLTASKCOMBOBOX_H__4EE655E3_F4B1_44EA_8AAA_39DD459AD8A8__INCLUDED_