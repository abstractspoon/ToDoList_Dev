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

	BOOL AddTask(const CString& sTask, DWORD dwTaskID, int nIndent, int nImage = -1);
	BOOL InsertTask(int nPos, const CString& sTask, DWORD dwTaskID, int nIndent, int nImage = -1);

	BOOL SetSelectedTaskID(DWORD dwTaskID);
	void SetImageList(HIMAGELIST hil) { m_hilTasks = hil; }
	BOOL SetTaskImage(DWORD dwTaskID, int nImage);
	
	int GetItemImage(int nItem) const;

protected:
	HIMAGELIST m_hilTasks;

protected:
	struct TCB_ITEMDATA : public ODCB_ITEMDATA
	{
		TCB_ITEMDATA() : nIndent(0), nImage(-1) {}

		int nImage;
		int nIndent;
	};

	virtual ODCB_ITEMDATA* NewExtItemData() const { return new TCB_ITEMDATA(); }
	virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
							  DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText);
	virtual BOOL HasIcon() const { return TRUE; }

};

#endif // AFX_TDLTASKCOMBOBOX_H__4EE655E3_F4B1_44EA_8AAA_39DD459AD8A8__INCLUDED_