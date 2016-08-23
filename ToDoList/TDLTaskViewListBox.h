#if !defined(AFX_TDLTASKVIEWLISTBOX_H__22FF6F91_6279_4D7B_91EA_451C14B8DA1E__INCLUDED_)
#define AFX_TDLTASKVIEWLISTBOX_H__22FF6F91_6279_4D7B_91EA_451C14B8DA1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLTaskViewListBox.h : header file
//

#include "..\Shared\checklistboxex.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskViewListBox window

class CUIExtensionMgr;

/////////////////////////////////////////////////////////////////////////////

const LPCTSTR LISTVIEW_TYPE = _T("LISTVIEW_TYPE");

/////////////////////////////////////////////////////////////////////////////

class CTDLTaskViewListBox : public CCheckListBoxEx
{
// Construction
public:
	CTDLTaskViewListBox(const CUIExtensionMgr* pMgr = NULL);

	void SetHiddenViews(const CStringArray& aTypeIDs);
	int GetHiddenViews(CStringArray& aTypeIDs) const;
	int GetVisibleViews(CStringArray& aTypeIDs) const;

// Operations
protected:
	const CUIExtensionMgr* m_pMgrUIExt;
	CStringArray m_aHiddenViews;
	CImageList m_ilTaskViews;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLTaskViewListBox)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTDLTaskViewListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDLTaskViewListBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT OnInitListBox(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()

protected:
	void PreDrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	void BuildList();
	int GetViews(CStringArray& aTypeIDs, BOOL bVisible) const;
	int AddItemImage(HICON hIcon, BOOL bDeleteIcon = TRUE);
	int GetImageIndex(int nItem) const;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLTASKVIEWLISTBOX_H__22FF6F91_6279_4D7B_91EA_451C14B8DA1E__INCLUDED_)
