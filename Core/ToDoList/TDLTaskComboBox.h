// RecurrenceEdit.h: interface for the CRecurringTaskEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDLTASKCOMBOBOX_H__4EE655E3_F4B1_44EA_8AAA_39DD459AD8A8__INCLUDED_)
#define AFX_TDLTASKCOMBOBOX_H__4EE655E3_F4B1_44EA_8AAA_39DD459AD8A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\shared\OwnerdrawComboBoxBase.h"
#include "..\shared\fontcache.h"

//////////////////////////////////////////////////////////////////////

class CTaskFile;
class CTDCImageList;

typedef void* HTASKITEM;

//////////////////////////////////////////////////////////////////////

class CTDLTaskComboBox : public COwnerdrawComboBoxBase
{
public:
	CTDLTaskComboBox();

	DWORD GetSelectedTaskID(BOOL bTrueTask = TRUE) const;
	BOOL SetSelectedTaskID(DWORD dwTaskID);

	CString GetSelectedTaskName() const;
	int GetSelectedTaskImage() const;

	int Populate(const CTaskFile& tasks, const CTDCImageList& ilTasks, BOOL bIncDoneTasks = TRUE);
	int Populate(const CTaskFile& tasks, const CTDCImageList& ilTasks, const CDWordArray& aRecentSel, BOOL bIncDoneTasks = TRUE);

	void EnableParentTasks(BOOL bEnable = TRUE) { m_bEnableParents = bEnable; }
	void SetShowParentTasksAsFolders(BOOL bAsFolders = TRUE) { m_bShowParentsAsFolders = bAsFolders; }
	void SetStrikethroughCompletedTasks(BOOL bStrikeThru = TRUE) { m_bStrikeThruDone = bStrikeThru; }

protected:
	const CTDCImageList* m_pIlTasks;
	CFontCache m_fonts;

	BOOL m_bEnableParents, m_bShowParentsAsFolders;
	BOOL m_bShowDoneTasks;
	BOOL m_bStrikeThruDone;

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	// Generated message map functions
	afx_msg void OnEditChange();
	DECLARE_MESSAGE_MAP()

protected:
	enum 
	{
		TCBA_PARENT		= 0x01,
		TCBA_TOPLEVEL	= 0x02,
		TCBA_DONE		= 0x04,
		TCBA_GOODASDONE = 0x08
	};

	struct TCB_ITEMDATA : public ODCB_ITEMDATA
	{
		TCB_ITEMDATA() 
			: 
			nDepth(0), 
			nImage(-1), 
			crText(CLR_NONE),
			dwAttribs(0),
			dwRefTaskID(0)
		{
		}

		BOOL HasAttrib(DWORD dwAttrib) const { return ((dwAttribs & dwAttrib) == dwAttrib); }

		int nDepth;
		int nImage;
		COLORREF crText;
		DWORD dwAttribs;
		DWORD dwRefTaskID;
	};

	virtual ODCB_ITEMDATA* NewExtItemData() const { return new TCB_ITEMDATA(); }
	virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
							  DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText);
	virtual void GetItemColors(int nItem, UINT nItemState, DWORD dwItemData,
							   COLORREF& crText, COLORREF& crBack) const;
	virtual int GetExtraListboxWidth() const;
	virtual int GetMaxDropWidth() const;
	virtual int CalcMinItemHeight(BOOL bList) const;
	virtual BOOL IsSelectableItem(int nItem) const;

	int GetItemDepth(int nItem) const;
	int GetItemImage(int nItem) const;
	DWORD GetItemRefID(int nItem) const;
	BOOL ItemHasAttrib(int nItem, DWORD dwAttrib) const;

	void SelectNextFind(BOOL bForward);

	void Populate(const CTaskFile& tasks, HTASKITEM hTask, int nDepth, BOOL bIncDoneTasks);
	BOOL InsertTask(int nPos, const CTaskFile& tasks, HTASKITEM hTask, int nDepth, BOOL bIncDoneTasks);
	BOOL InsertTask(int nPos, const CString& sTask, DWORD dwTaskID, int nDepth, int nImage, 
					COLORREF crText = CLR_NONE, DWORD dwAttribs = 0, DWORD dwRefTaskID = 0);
	BOOL ModifyItem(int nItem, const CString& sName, int nImage);
};

#endif // AFX_TDLTASKCOMBOBOX_H__4EE655E3_F4B1_44EA_8AAA_39DD459AD8A8__INCLUDED_