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

	BOOL SetSelectedTaskID(DWORD dwTaskID);
	BOOL AddTask(const CString& sTask, DWORD dwTaskID, int nLevel, int nImage = -1);
	void SetImageList(HIMAGELIST hil) { m_hilTasks = hil; }
	BOOL SetTaskImage(DWORD dwTaskID, int nImage);
	BOOL SetItemImage(int nItem, int nImage);

protected:
	HIMAGELIST m_hilTasks;

protected:
	struct TCB_ITEMDATA : public ODCB_ITEMDATA
	{
		TCB_ITEMDATA() : nImage(-1) {}

		int nImage;
	};

	virtual ODCB_ITEMDATA* NewExtItemData() const { return new TCB_ITEMDATA(); }
};

#endif // AFX_TDLTASKCOMBOBOX_H__4EE655E3_F4B1_44EA_8AAA_39DD459AD8A8__INCLUDED_